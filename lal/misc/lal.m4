dnl lal.m4

AC_DEFUN([LAL_ENABLE_GCC_FLAGS],
[AC_ARG_ENABLE([gcc_flags],
  AC_HELP_STRING([--enable-gcc-flags],[turn on strict gcc warning flags (default=yes)]),
  [case "${enableval}" in
     yes) DO_ENABLE_LAL_GCC_FLAGS;;
     no) ;;
     *) DO_ENABLE_LAL_GCC_FLAGS;;
   esac ],
   [ DO_ENABLE_LAL_GCC_FLAGS ] )
])

AC_DEFUN([DO_ENABLE_LAL_GCC_FLAGS],
[
  lal_gcc_flags="-g3 -O4 -Wall -W -Wmissing-prototypes -Wstrict-prototypes -Wshadow -Wpointer-arith -Wcast-qual -Wcast-align -Wwrite-strings -fno-common -Wnested-externs -Wno-format-zero-length"
  if test "${cuda}" != "true"; then
    case $host_cpu-$host_os in
      *i386-darwin*) lal_gcc_flags="${lal_gcc_flags} -pedantic" ;;
      *x86_64-darwin*) lal_gcc_flags="${lal_gcc_flags} -pedantic" ;;
      *) lal_gcc_flags="${lal_gcc_flags} -pedantic-errors" ;;
    esac
  fi
])

AC_DEFUN([LAL_WITH_EXTRA_CPPFLAGS],
[AC_ARG_WITH(
  [extra_cppflags],
  AC_HELP_STRING([--with-extra-cppflags=CPPFLAGS],[additional C preprocessor flags]),
  [ if test -n "${with_extra_cppflags}"
    then
      CPPFLAGS="$CPPFLAGS ${with_extra_cppflags}";
    fi
  ],)
])

AC_DEFUN([LAL_WITH_CFLAGS],
[AC_ARG_WITH(
  [cflags],
  AC_HELP_STRING([--with-cflags=CFLAGS],[C compiler flags]),
  [ if test -n "${with_cflags}"
    then
      CFLAGS="${with_cflags}";
    fi
  ],)
])

AC_DEFUN([LAL_WITH_EXTRA_CFLAGS],
[AC_ARG_WITH(
  [extra_cflags],
  AC_HELP_STRING([--with-extra-cflags=CFLAGS],[additional C compiler flags]),
  [ if test -n "${with_extra_cflags}"
    then
      CFLAGS="$CFLAGS ${with_extra_cflags}";
    fi
  ],)
])

AC_DEFUN([LAL_WITH_EXTRA_LDFLAGS],
[AC_ARG_WITH(
  [extra_ldflags],
  AC_HELP_STRING([--with-extra-ldflags=LDFLAGS],[additional linker flags]),
  [ if test -n "${with_extra_ldflags}"
    then
      LDFLAGS="$LDFLAGS ${with_extra_ldflags}";
    fi
  ],)
])

AC_DEFUN([LAL_WITH_EXTRA_LIBS],
[AC_ARG_WITH(
  [extra_libs],
  AC_HELP_STRING([--with-extra-libs=LIBS],[additional -l and -L linker flags]),
  [ if test -n "${with_extra_libs}"
    then
      LIBS="$LIBS ${with_extra_libs}";
    fi
  ],)
])

AC_DEFUN([LAL_WITH_CC],
[AC_ARG_WITH(
  [cc],
  AC_HELP_STRING([--with-cc=CC],[use the CC C compiler]),
  [ if test -n "${with_cc}"
    then
      CC="${with_cc}";
    fi
  ],)
])

AC_DEFUN([LAL_ENABLE_INTELFFT],
[AC_ARG_ENABLE(
  [intelfft],
  AC_HELP_STRING([--enable-intelfft],[use Intel FFT libraries insted of FFTW [default=no]]),
  [ case "${enableval}" in
      yes) intelfft=true ;;
      no)  intelfft=false ;;
      condor) intelfft=true ; qthread=true ; AC_DEFINE(LAL_QTHREAD, 1, Use fake qthread library for MKL Condor compatibility) ;;
      *) AC_MSG_ERROR(bad value ${enableval} for --enable-intelfft) ;;
    esac
  ], [ intelfft=false ] )
])

AC_DEFUN([LAL_WITH_CUDA],
[AC_ARG_WITH(
  [cuda],
  AC_HELP_STRING([--with-cuda=PATH],[specify location of CUDA [/opt/cuda]]),
  [ case "$with_cuda" in
    no)
      cuda=false
      ;;
    yes)
      AC_MSG_WARN([No path for CUDA specifed, using /opt/cuda])
      cuda=true
      CUDA_LIBS="-L/opt/cuda/lib -lcufft -lcudart"
      CUDA_CFLAGS="-I/opt/cuda/include"
      LIBS="$LIBS $CUDA_LIBS"
      CFLAGS="$CFLAGS $CUDA_CFLAGS"
      AC_SUBST(CUDA_LIBS)
      AC_SUBST(CUDA_CFLAGS)
      ;;
    *)
      AC_MSG_NOTICE([Using ${with_cuda} as CUDA path])
      cuda=true
      CUDA_LIBS="-L${with_cuda}/lib -lcufft -lcudart"
      CUDA_CFLAGS="-I${with_cuda}/include"
      LIBS="$LIBS $CUDA_LIBS"
      CFLAGS="$CFLAGS $CUDA_CFLAGS"
      AC_SUBST(CUDA_LIBS)
      AC_SUBST(CUDA_CFLAGS)
    esac
  ], [ cuda=false ])
])

AC_DEFUN([LAL_ENABLE_BOINC],
[AC_ARG_ENABLE(
  [boinc],
  AC_HELP_STRING([--enable-boinc],[enable BOINC support [default=no]]),
  [ case "${enableval}" in
      yes) boinc=true;;
      no) boinc=false;;
      *) AC_MSG_ERROR(bad value ${enableval} for --enable-boinc);;
    esac
  ], [ boinc=false ] )
AC_ARG_VAR([BOINC_PREFIX],[BOINC installation directory (optional)])
])

AC_DEFUN([LAL_ENABLE_DEBUG],
[AC_ARG_ENABLE(
  [debug],
  AC_HELP_STRING([--enable-debug],[include standard LAL debugging code [default=yes]]),
  [ case "${enableval}" in
      yes) ;;
      no) AC_DEFINE(LAL_NDEBUG, 1, Suppress debugging code) ;;
      *) AC_MSG_ERROR(bad value for ${enableval} for --enable-debug) ;;
    esac
  ], )
])

AC_DEFUN([LAL_ENABLE_MACROS],
[AC_ARG_ENABLE(
  [macros],
  AC_HELP_STRING([--enable-macros],[use LAL macros [default=yes]]),
  [ case "${enableval}" in
      yes) ;;
      no) AC_DEFINE(NOLALMACROS, 1, Use functions rather than macros) ;;
      *) AC_MSG_ERROR(bad value for ${enableval} for --enable-debug) ;;
    esac
  ], )
])

AC_DEFUN([LAL_ENABLE_NIGHTLY],
[AC_ARG_ENABLE(
  [nightly],
  AC_HELP_STRING([--enable-nightly],[nightly build [default=no]]),
  [ case "${enableval}" in
      yes) NIGHTLY_VERSION=`date +"%Y%m%d"`
           VERSION="${VERSION}.${NIGHTLY_VERSION}" ;;
      no) NIGHTLY_VERSION="";;
      *) NIGHTLY_VERSION="${enableval}"
         VERSION="${VERSION}.${NIGHTLY_VERSION}" ;;
      esac ],
  [ NIGHTLY_VERSION="" ] )
  AC_SUBST(NIGHTLY_VERSION)
])

AC_DEFUN([LAL_ENABLE_PTHREAD_LOCK],
[AC_ARG_ENABLE(
  [pthread_lock],
  AC_HELP_STRING([--enable-pthread-lock],[use pthread mutex lock for threadsafety [default=no]]),
  [ case "${enableval}" in
      yes) lal_pthread_lock=true; AC_DEFINE(LAL_PTHREAD_LOCK, 1, Use pthread mutex lock for threadsafety) ;;
      no) lal_pthread_lock=false ;;
      *) AC_MSG_ERROR(bad value for ${enableval} for --enable-pthread-lock) ;;
    esac
  ], [ lal_pthread_lock=false ] )
])

AC_DEFUN([LAL_INTEL_MKL_QTHREAD_WARNING],
[echo "**************************************************************"
 echo "* LAL will be linked against the fake POSIX thread library!  *"
 echo "*                                                            *"
 echo "* This build of LAL will not be thread safe and cannot be    *"
 echo "* linked against the system pthread library.                 *"
 echo "*                                                            *"
 echo "* The environment variables                                  *"
 echo "*                                                            *"
 echo "*    MKL_SERIAL=YES                                          *"
 echo "*    KMP_LIBRARY=serial                                      *"
 echo "*                                                            *"
 echo "* must be set before running executables linked against this *"
 echo "* build of LAL.                                              *"
 echo "*                                                            *"
 echo "* Please see the documention of the FFT package for details. *"
 echo "**************************************************************"
])

AC_DEFUN([LAL_INTEL_FFT_LIBS_MSG_ERROR],
[echo "**************************************************************"
 echo "* The --enable-static and --enable-shared options are        *"
 echo "* mutually exclusive with Intel FFT libraries.               *"
 echo "*                                                            *"
 echo "* Please reconfigure with:                                   *"
 echo "*                                                            *"
 echo "*   --enable-static --disable-shared                         *"
 echo "*                                                            *"
 echo "* for static libraries or                                    *"
 echo "*                                                            *"
 echo "*   --disable-static --enable-shared                         *"
 echo "*                                                            *"
 echo "* for shared libraries.                                      *"
 echo "*                                                            *"
 echo "* Please see the instructions in the file INSTALL.           *"
 echo "**************************************************************"
AC_MSG_ERROR([Intel FFT must use either static or shared libraries])
])

AC_DEFUN([LAL_CHECK_GSL_VERSION],
[
  lal_min_gsl_version=ifelse([$1], ,1.0,$1)
  AC_MSG_CHECKING(for GSL version >= $lal_min_gsl_version)
  AC_TRY_RUN([
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gsl/gsl_version.h>
int main(void)
{
  int required_major, required_minor;
  int major, minor;
  char required_version[] = "$lal_min_gsl_version";
  char version[] = GSL_VERSION;
  if ( strcmp(GSL_VERSION, gsl_version) ) {
    printf("error\n*** mismatch between header and library versions of GSL\n" );
    printf("\n*** header  has version %s\n", GSL_VERSION);
    printf("\n*** library has version %s\n", gsl_version);
    exit(1);
  }
  sscanf(required_version, "%d.%d", &required_major, &required_minor);
  sscanf(version, "%d.%d", &major, &minor);
  if ( major < required_major || (major == required_major && minor < required_minor) ) {
    printf("no\n*** found version %s of GSL but minimum version is %d.%d\n", GSL_VERSION, required_major, required_minor );
    exit(1);
  }
  return 0;
}
  ], [AC_MSG_RESULT(yes)], [AC_MSG_ERROR(could not find required version of GSL)], [echo $ac_n "cross compiling; assumed OK... $ac_c"])
])

dnl This is AC_CHECK_SIZEOF but prepends LAL.

AC_DEFUN([LAL_CHECK_SIZEOF],
[changequote(<<, >>)dnl
dnl The name to #define.
define(<<LAL_TYPE_NAME>>, translit(lal_sizeof_$1, [a-z *], [A-Z_P]))dnl
dnl The cache variable name.
define(<<LAL_CV_NAME>>, translit(lal_cv_sizeof_$1, [ *], [_p]))dnl
changequote([, ])dnl
AC_MSG_CHECKING([size of $1])
AC_CACHE_VAL(LAL_CV_NAME,
[AC_TRY_RUN([#include <stdio.h>
main()
{
  FILE *f=fopen("conftestval", "w");
  if (!f) exit(1);
  fprintf(f, "%d\n", sizeof($1));
  exit(0);
}], LAL_CV_NAME=`cat conftestval`, LAL_CV_NAME=0, ifelse([$2], , , LAL_CV_NAME=$2))
])dnl
AC_MSG_RESULT([$LAL_CV_NAME])
AC_DEFINE_UNQUOTED(LAL_TYPE_NAME, $LAL_CV_NAME)
undefine([LAL_TYPE_NAME])dnl
undefine([LAL_CV_NAME])dnl
])
