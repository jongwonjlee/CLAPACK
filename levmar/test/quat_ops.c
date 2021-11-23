#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "utils.h"


int main() {
  // generate an angle axis representation, convert it into quaternion, and put it back to original
  double angle = gen_noise(30, 0);
  double* axis = gen_random_uvec();
  printf("angle: %f, axis: (%f, %f, %f) \n", angle, axis[0], axis[1], axis[2]);

  struct AngleAxis angleaxis;
  angleaxis.angle = angle;
  angleaxis.axis = axis;
  
  double* q = angleaxis2quat(angleaxis, 1);
  printf("q: (%f, %f, %f, %f) \n", q[0], q[1], q[2], q[3]);
  
  struct AngleAxis angleaxis_after = quat2angleaxis(q, 1);
  printf("angle: %f, axis: (%f, %f, %f) \n", angleaxis_after.angle, angleaxis_after.axis[0], angleaxis_after.axis[1], angleaxis_after.axis[2]);
  
  // show matrix representation
  double** R = quat2Rot(q);
  printf("R[0][0]: %f, R[0][1]: %f, R[0][2]: %f \n", R[0][0], R[0][1], R[0][2]);
  printf("R[1][0]: %f, R[1][1]: %f, R[1][2]: %f \n", R[1][0], R[1][1], R[1][2]);
  printf("R[2][0]: %f, R[2][1]: %f, R[2][2]: %f \n", R[2][0], R[2][1], R[2][2]);

  // test matrix multiplication
  double* v = gen_random_uvec();
  double* Rv = matmul(R, v);
  printf("v: (%f, %f, %f) -> Rv: (%f, %f, %f) \n", v[0], v[1], v[2], Rv[0], Rv[1], Rv[2]);

  exit(0);
}
