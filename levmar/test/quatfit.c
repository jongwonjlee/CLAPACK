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


/* model to be fitted to measurements: x_i = p[0]*exp(-p[1]*i) + p[2], i=0...n-1 */
void expfunc(double *p, double *x, int m, int n, void *data)
{
register int i;

  for(i=0; i<n; ++i){
    x[i]=p[0]*exp(-p[1]*i) + p[2];
  }
}

/* Jacobian of expfunc() */
void jacexpfunc(double *p, double *jac, int m, int n, void *data)
{   
register int i, j;
  
  /* fill Jacobian row by row */
  for(i=j=0; i<n; ++i){
    jac[j++]=exp(-p[1]*i);
    jac[j++]=-p[0]*i*exp(-p[1]*i);
    jac[j++]=1.0;
  }
}

int main() {
  double angle = gen_noise(30, 0);
  double* axis = gen_random_uvec();
  printf("angle: %f, axis[0]: %f, axis[1]: %f, axis[2]: %f \n", angle, axis[0], axis[1], axis[2]);

  struct AngleAxis angleaxis;
  angleaxis.angle = angle;
  angleaxis.axis = axis;
  
  double* q = angleaxis2quat(angleaxis, 1);
  printf("q[0]: %f, q[1]: %f, q[2]: %f, q[3]: %f \n", q[0], q[1], q[2], q[3]);
  
  struct AngleAxis angleaxis_after = quat2angleaxis(q, 1);
  printf("angle: %f, axis[0]: %f, axis[1]: %f, axis[2]: %f \n", angleaxis_after.angle, angleaxis_after.axis[0], angleaxis_after.axis[1], angleaxis_after.axis[2]);
  
  double** R = quat2Rot(q);
  printf("R[0][0]: %f, R[0][1]: %f, R[0][2]: %f \n", R[0][0], R[0][1], R[0][2]);
  printf("R[1][0]: %f, R[1][1]: %f, R[1][2]: %f \n", R[1][0], R[1][1], R[1][2]);
  printf("R[2][0]: %f, R[2][1]: %f, R[2][2]: %f \n", R[2][0], R[2][1], R[2][2]);

  exit(0);
}
