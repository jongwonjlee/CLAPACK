#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#ifdef USE_CLAPACK
  #include "f2c.h"
  #include "clapack.h"
#endif

#include "levmar.h"
#include "utils.h"

#ifndef LM_DBL_PREC
#error Example program assumes that levmar has been compiled with double precision, see LM_DBL_PREC!
#endif

void quatfunc(double *q, double *x, int m, int n, void *v_list)
{
  register int i;
  
  double* ref_addr = (double*)v_list;
  
  for(i=0; i<n; i+=3){
    double* v = malloc(3*sizeof(double));
    v[0] = *(ref_addr+i+0); 
    v[1] = *(ref_addr+i+1); 
    v[2] = *(ref_addr+i+2);

    double* qv = malloc(3*sizeof(double));
    qv = matmul(quat2Rot(q), v);
    x[i+0] = qv[0];
    x[i+1] = qv[1];
    x[i+2] = qv[2];

    free(v);
    free(qv);
  }
}

void jacquatfunc(double *q, double *jac, int m, int n, void *v_list)
{
  register int i, j;
  
  double* ref_addr = (double*)v_list;
  
  for(i=j=0; i<n; i+=3){
    double* v = malloc(3*sizeof(double));
    v[0] = *(ref_addr+i+0); 
    v[1] = *(ref_addr+i+1); 
    v[2] = *(ref_addr+i+2);

    jac[j++] = 2*q[0]*v[0] + 2*q[2]*v[2] - 2*q[3]*v[1];
    jac[j++] = 2*q[1]*v[0] + 2*q[2]*v[1] + 2*q[3]*v[2];
    jac[j++] = 2*q[0]*v[2] + 2*q[1]*v[1] - 2*q[2]*v[0];
    jac[j++] = -2*q[0]*v[1] + 2*q[1]*v[2] - 2*q[3]*v[0];
    
    jac[j++] = 2*q[0]*v[1] - 2*q[1]*v[2] + 2*q[3]*v[0];
    jac[j++] = -2*q[0]*v[2] - 2*q[1]*v[1] + 2*q[2]*v[0];
    jac[j++] = 2*q[1]*v[0] + 2*q[2]*v[1] + 2*q[3]*v[2];
    jac[j++] = 2*q[0]*v[0] + 2.0*q[2]*v[2] - 2*q[3]*v[1];

    jac[j++] = 2*q[0]*v[2] + 2*q[1]*v[1] - 2*q[2]*v[0];
    jac[j++] = 2*q[0]*v[1] - 2*q[1]*v[2] + 2*q[3]*v[0];
    jac[j++] = -2*q[0]*v[0] - 2*q[2]*v[2] + 2*q[3]*v[1];
    jac[j++] = 2*q[1]*v[0] + 2*q[2]*v[1] + 2*q[3]*v[2];

    free(v);
  }
}

int main() {
  const int num_data = 500;
  const int vect_dim = 3;

  const int n=num_data*vect_dim, m=4; // 1500 measurements, 4 parameters
  double p[m], x[n], opts[LM_OPTS_SZ], info[LM_INFO_SZ];
  register int i;
  int ret;
  
  // generate ground-truth and initial guess
  AngleAxis aa_gt = {gen_noise(30, 5), gen_random_uvec()};
  double* q_gt = angleaxis2quat(aa_gt, 1);
  AngleAxis aa = {gen_noise(0, 0), gen_random_uvec()};
  double* q = angleaxis2quat(aa, 1);
  
  printf("[true val] q: (%f, %f, %f, %f) \n", q_gt[0], q_gt[1], q_gt[2], q_gt[3]);
  printf("[iter 0] q: (%f, %f, %f, %f) \n", q[0], q[1], q[2], q[3]);

  // generate data
  double v_list[num_data][vect_dim];
  double qv_list[num_data][vect_dim];

  for(i=0; i<num_data; i++){
    double* v = gen_random_uvec();

    AngleAxis aa_noise = {gen_noise(0, 0), gen_random_uvec()};
    double* q_noise = angleaxis2quat(aa_noise, 1);

    v_list[i][0] = v[0];
    v_list[i][1] = v[1];
    v_list[i][2] = v[2];

    double* qv = matmul(quat2Rot(q_noise), matmul(quat2Rot(q_gt), v));
    
    qv_list[i][0] = qv[0];
    qv_list[i][1] = qv[1];
    qv_list[i][2] = qv[2];

    free(v);
    free(qv);
    free(q_noise);
  }

  // flatten qv_list to be x (1d array of measurement)
  for(i=0; i<n; i+=3){
    x[i+0] = qv_list[i][0];
    x[i+1] = qv_list[i][1];
    x[i+2] = qv_list[i][2];
  }

  // run optimization

  /* optimization control parameters; passing to levmar NULL instead of opts reverts to defaults */
  opts[0]=LM_INIT_MU; opts[1]=1E-15; opts[2]=1E-15; opts[3]=1E-20;
  opts[4]=LM_DIFF_DELTA; // relevant only if the finite difference Jacobian version is used 

  /* invoke the optimization function */
  ret=dlevmar_der(quatfunc, jacquatfunc, q, x, m, n, 1000, opts, info, NULL, NULL, v_list); // with analytic Jacobian
  //ret=dlevmar_dif(expfunc, p, x, m, n, 1000, opts, info, NULL, NULL, NULL); // without Jacobian
  printf("Levenberg-Marquardt returned in %g iter, reason %g, sumsq %g [%g]\n", info[5], info[6], info[1], info[0]);
  printf("Best fit parameters: %.7g %.7g %.7g %.7g\n", q[0], q[1], q[2], q[3]);

  exit(0);
}
