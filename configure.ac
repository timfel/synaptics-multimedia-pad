AC_INIT([synaptics-multimedia-pad],
        [0.0.1],
        [Tim Felgentreff timfelgentreff@gmail.com],
        [synaptics-multimedia-pad}])
AC_CONFIG_AUX_DIR(config)
AM_CONFIG_HEADER(config.h)
AM_INIT_AUTOMAKE([dist-bzip2])

AC_PROG_CC
AC_PROG_INSTALL
AC_AIX
AC_ISC_POSIX
AC_MINIX
AC_STDC_HEADERS
AC_CHECK_HEADERS([stddef.h stdlib.h string.h sys/time.h unistd.h X11/Xdefs.h])
AC_CHECK_FUNCS([memset rint strstr])
AC_PROG_RANLIB

AC_CONFIG_FILES([
   Makefile
   README
   doc/Makefile
   m4/Makefile
   src/Makefile
])

AC_OUTPUT
