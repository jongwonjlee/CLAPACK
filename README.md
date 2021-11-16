## Fork of CLAPACK 3.2.1 (f2c'ed version of LAPACK)
> [http://www.netlib.org/clapack/](http://www.netlib.org/clapack/)

## How to execute on your host machine
```
$ chmod +x build_host.sh
$ ./build_host.sh
```

## How to execute on the target machine (i.e., NXP's MPC5748G) (***Under development***)

Prerequisite:
- NXP's S32 Design Studio

```
$ cp make.inc.target make.inc
$ make f2clib
$ make blaslib
$ make blas_testing
$ make lapacklib
$ make lapack_testing
```