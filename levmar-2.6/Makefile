#
# Unix/Linux GCC Makefile for Levenberg - Marquardt minimization
# Under windows, use Makefile.vc for MSVC
#

# basic directory
VLE_TOOLCHAIN_DIR = $(HOME)/NXP/S32DS_Power_v2.1/S32DS/build_tools/powerpc-eabivle-4_9
VLE_EWL_DIR = $(VLE_TOOLCHAIN_DIR)/../e200_ewl2
CROSS_PREFIX = powerpc-eabivle
SYSROOT = $(VLE_EWL_DIR)
SPECS = $(VLE_EWL_DIR)/lib/ewl_c9x_hosted.specs

# WHEN USING LAPACK, CHANGE THIS TO WHERE YOUR COMPILED LIBS ARE!
LAPACKLIBS_PATH=$(HOME)/workspace/CLAPACK-3.2.1
CC = $(VLE_TOOLCHAIN_DIR)/bin/$(CROSS_PREFIX)-gcc
CONFIGFLAGS=#-ULINSOLVERS_RETAIN_MEMORY
#ARCHFLAGS=-march=pentium4 # YOU MIGHT WANT TO UNCOMMENT THIS FOR P4
INCLUDE_PATH = -I$(SYSROOT)/EWL_C/include -I$(SYSROOT)/EWL_C/include/pa -I$(SYSROOT)/EWL_C/include/extras -I$(LAPACKLIBS_PATH)/INCLUDE
CFLAGS= $(CONFIGFLAGS) $(ARCHFLAGS) $(INCLUDE_PATH) -std=c99 -O3 -specs=$(SPECS) --sysroot=$(SYSROOT) -funroll-loops -Wall #-g #-ffast-math #-pg
LDFLAGS=-L$(LAPACKLIBS_PATH) -L.
LIBOBJS=lm.o Axb.o misc.o lmlec.o lmbc.o lmblec.o lmbleic.o
LIBSRCS=lm.c Axb.c misc.c lmlec.c lmbc.c lmblec.c lmbleic.c
DEMOBJS=lmdemo.o
DEMOSRCS=lmdemo.c
AR = ${VLE_TOOLCHAIN_DIR}/bin/${CROSS_PREFIX}-ar
RANLIB=ranlib
LAPACKLIBS=-llapack -lblas -lf2c # comment this line if you are not using LAPACK.
                                 # On systems with a FORTRAN (not f2c'ed) version of LAPACK, -lf2c is
                                 # not necessary; on others, -lf2c is equivalent to -lF77 -lI77

#LAPACKLIBS=-L/usr/local/atlas/lib -llapack -lcblas -lf77blas -latlas -lf2c # This works with the ATLAS updated lapack and Linux_P4SSE2
                                                                            # from http://www.netlib.org/atlas/archives/linux/

#LAPACKLIBS=-llapack -lgoto2 -lpthread -lf2c # This works with GotoBLAS
                                             # from http://www.tacc.utexas.edu/research-development/tacc-projects/

#LAPACKLIBS=-L/opt/intel/mkl/8.0.1/lib/32/ -lmkl_lapack -lmkl_ia32 -lguide -lf2c # This works with MKL 8.0.1 from
                                            # http://www.intel.com/cd/software/products/asmo-na/eng/perflib/mkl/index.htm
                                            # check the MKL link line advisor for more information:
                                            # http://software.intel.com/en-us/articles/intel-mkl-link-line-advisor/

LIBS=$(LAPACKLIBS)

all: liblevmar.a # lmdemo

liblevmar.a: $(LIBOBJS)
	$(AR) crv liblevmar.a $(LIBOBJS)
	$(RANLIB) liblevmar.a

lmdemo: $(DEMOBJS) liblevmar.a
	$(CC) $(CFLAGS) $(LDFLAGS) $(DEMOBJS) -o lmdemo -llevmar $(LIBS) -lm

lm.o: lm.c lm_core.c levmar.h misc.h compiler.h
Axb.o: Axb.c Axb_core.c levmar.h misc.h
misc.o: misc.c misc_core.c levmar.h misc.h
lmlec.o: lmlec.c lmlec_core.c levmar.h misc.h
lmbc.o: lmbc.c lmbc_core.c levmar.h misc.h compiler.h
lmblec.o: lmblec.c lmblec_core.c levmar.h misc.h
lmbleic.o: lmbleic.c lmbleic_core.c levmar.h misc.h

lmdemo.o: levmar.h

clean:
	@rm -f $(LIBOBJS) $(DEMOBJS)

cleanall: clean
	@rm -f lmdemo
	@rm -f liblevmar.a

depend:
	makedepend -f Makefile $(LIBSRCS) $(DEMOSRCS)

# DO NOT DELETE THIS LINE -- make depend depends on it.

