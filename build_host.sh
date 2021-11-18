#!/bin/bash

# rename make.inc.host to make.inc
cp make.inc.host make.inc

# Build f2c libraries
make f2clib

# Build CBLAS (Fortran-free) libraries
make blaslib

# Build CBLAS testing libraries
make blas_testing

# Build CLAPACK (Fortran-free) libraries
make lapacklib

# Build CLAPACK testing libraries
make lapack_testing

# copy f2clib to root directory
cp F2CLIBS/libf2c.a .

# [optional] build clapack tutorial code
# gcc -o clapack_tutorial clapack_tutorial.c -IINCLUDE -lclapack -lcblas -lf2c -lm -L.
# ./clapack_tutorial

# Clean all
# make cleanall