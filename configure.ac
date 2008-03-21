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

############### DEBUG ##############
AC_ARG_ENABLE(debug-mode,
	         [  --enable-debug-mode	enable debug mode],
			 debug="yes", debug="no")
			 
if test $debug = "yes" ; then
	AC_DEFINE(DEBUG, 1, [ ])
fi

############### ALSA ##############
AC_ARG_ENABLE(alsa,
			 [[  --disable-alsa	do not use ALSA for on/off notfication]],
			 alsa="no", alsa="yes")

if test $alsa = "yes" ; then
	AC_CHECK_HEADER(alsa/asoundlib.h,
				[AC_DEFINE(ALSA, 1, [ ])], alsa="no")
fi
			 


AC_CONFIG_FILES([
   Makefile
   doc/Makefile
   m4/Makefile
   src/Makefile
])

AC_OUTPUT

#####################################################################
## Info

echo
echo
echo
echo "------------------------------------------------------------------------"
echo "$PACKAGE $VERSION"
echo "------------------------------------------------------------------------"
echo
echo "Configuration Options Summary:"
echo
echo "DEBUG ....................... $debug"
echo "ALSA ........................ $alsa"
echo
echo
echo "Now type make, make install"
