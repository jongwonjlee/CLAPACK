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

# Clean all
# make cleanall