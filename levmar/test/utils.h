#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/* the following macros concern the initialization of a random number generator for adding noise */
#undef REPEATABLE_RANDOM
#define DBL_RAND_MAX (double)(RAND_MAX)

#ifdef _MSC_VER // MSVC
#include <process.h>
#define GETPID  _getpid
#elif defined(__GNUC__) // GCC
#include <sys/types.h>
#include <unistd.h>
#define GETPID  getpid
#else
#warning Do not know the name of the function returning the process id for your OS/compiler combination
#define GETPID  0
#endif /* _MSC_VER */

#ifdef REPEATABLE_RANDOM
#define INIT_RANDOM(seed) srandom(seed)
#else
#define INIT_RANDOM(seed) srandom((int)GETPID()) // seed unused
#endif

/* Get 2-norm of `data` with length of `size` */
double get_norm(double* data, int size) {
    double val = 0.0;
    for(int i=0; i < size; i++) val += data[i] * data[i];
    
    return sqrt(val);
}

/* Gaussian noise with mean m and variance s, uses the Box-Muller transformation */
double gen_noise(double m, double s) {
    double r1, r2, val;
    r1=((double)random())/DBL_RAND_MAX;
    r2=((double)random())/DBL_RAND_MAX;

    val=sqrt(-2.0*log(r1))*cos(2.0*M_PI*r2);
    val=s*val+m;

    return val;
}

/* generate random unit vector with size of 3 */
double* gen_random_uvec() {
    static double v[3];
    for(int i=0; i<3; i++) v[i] = gen_noise(0.0, 1.0);
    
    double vnorm = get_norm(v, 3);
    for(int i=0; i<3; i++) v[i] /= vnorm;

    return v;
}

struct AngleAxis {
    double angle;
    double* axis;
};

/* convert angle axis representation (float, (np.array (3,))) into quaternion (np.array (4,)) */
double* angleaxis2quat(struct AngleAxis angle_axis, int isDeg) {
    double angle = angle_axis.angle;
    double* axis = angle_axis.axis;
    
    double eps = 1e-10;
    double axisvec_norm = get_norm(axis, 3);
    if (!(abs(axisvec_norm - 1.0) < eps)) {
        for (int i=0; i<3; i++) axis[i] /=  axisvec_norm;
    }

    if (isDeg) angle = angle * M_PI / 180;
        
    static double q[4];
    q[0] = cos(angle/2);
    q[1] = axis[0] * sin(angle/2);
    q[2] = axis[1] * sin(angle/2);
    q[3] = axis[2] * sin(angle/2);

    return q;
}

/* convert quaternion (array size of 4) into angle axis representation (array size of 3) */
struct AngleAxis quat2angleaxis(double* q, int isDeg) {
    double angle = 2 * acos(q[0]);
    double axis[3];
    for (int i=0; i<3; i++) axis[i] = q[i+1] / sin(angle / 2);
    
    if(isDeg) angle = angle * 180 * M_1_PI;

    struct AngleAxis angle_axis;
    angle_axis.angle = angle;
    angle_axis.axis = axis;

    return angle_axis;
}

/* convert quaternion (array size of 4) into rotation matrix (array size of 3x3) */
double** quat2Rot(double* q) {
    double* values = calloc(3*3, sizeof(double));
    double** R = malloc(3*sizeof(double*));
    for (int i=0; i<3; ++i) R[i] = values + i*3;

    R[0][0] = q[0]*q[0]  + q[1]*q[1] - q[2]*q[2] - q[3]*q[3];
    R[0][1] = 2*(q[1]*q[2] - q[0]*q[3]);
    R[0][2] = 2*(q[0]*q[2] + q[1]*q[3]);
    R[1][0] = 2*(q[1]*q[2] + q[0]*q[3]);
    R[1][1] = q[0]*q[0] - q[1]*q[1] + q[2]*q[2] - q[3]*q[3];
    R[1][2] = 2*(q[2]*q[3] - q[0]*q[1]);
    R[2][0] = 2*(q[1]*q[3] - q[0]*q[2]);
    R[2][1] = 2*(q[0]*q[1] + q[2]*q[3]);
    R[2][2] = q[0]*q[0] - q[1]*q[1] - q[2]*q[2] + q[3]*q[3];
    return R;
}