AC_INIT([synaptics-multimedia-pad],
        [0.0.2],
        [Tim Felgentreff timfelgentreff@gmail.com],
        [synaptics-multimedia-pad])
AC_CONFIG_AUX_DIR(config)
AM_CONFIG_HEADER(config.h)
AM_INIT_AUTOMAKE([dist-bzip2])

AC_PROG_CC
AC_PROG_INSTALL
AC_AIX
AC_ISC_POSIX
AC_MINIX
AC_STDC_HEADERS
AC_CHECK_HEADERS([stddef.h stdlib.h string.h sys/time.h unistd.h X11/Xdefs.h pthread.h])
AC_CHECK_FUNCS([memset rint strstr])
AC_PROG_RANLIB
AM_GNU_GETTEXT([external])

AC_SUBST(OPTLIBS)	# Optional Libraries go here
AC_SUBST(OPTCFLAGS)	# Optional flags go here

if test "x${prefix}" = "xNONE"; then
   PREFIX="${ac_default_prefix}"
else
   PREFIX="${prefix}"
fi

AC_SUBST(PREFIX)

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

############## GSTREAMER #########
AC_ARG_ENABLE(gstreamer,
		[[  --disable-gstreamer	do not use Gstreamer for on/off notification]],
		gstreamer="no", gstreamer="yes")

if test $gstreamer = "yes" ; then
	PKG_CHECK_MODULES(GSTREAMER,
			[ gstreamer-0.10 ],
			[AC_DEFINE(GSTREAMER, 1, [ ])],
			[ gstreamer="no" ])
fi

if test $gstreamer = "yes" ; then
	OPTLIBS="$OPTLIBS `pkg-config --libs gstreamer-0.10`"
	OPTCFLAGS="$OPTCFLAGS `pkg-config --cflags gstreamer-0.10`"
	alsa="no (using Gstreamer)"
fi

############## ALSA ##############
if test $alsa = "yes" ; then
	AC_CHECK_HEADER(alsa/asoundlib.h,
				[AC_DEFINE(ALSA, 1, [ ])], alsa="no")
fi

if test $alsa = "yes" ; then
	OPTLIBS="$OPTLIBS -lasound"
fi

############### XOSD ##############
AC_ARG_ENABLE(xosd,
			 [[  --disable-xosd	do not use XOSD for visualisation]],
			 xosd="no", xosd="yes")

if test $xosd = "yes" ; then
	AC_CHECK_HEADER(xosd.h,
				[AC_DEFINE(XOSD, 1, [ ])], xosd="no")
fi

if test $xosd = "yes"; then
	OPTLIBS="$OPTLIBS -lxosd -lpthread -lXext -lX11 -lXinerama"
fi

AC_CONFIG_FILES([ po/Makefile.in
   Makefile
   doc/Makefile
   m4/Makefile
   src/Makefile
   data/Makefile
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
echo "GSTREAMER ................... $gstreamer"
echo "XOSD ........................ $xosd"
echo
echo "Files will be installed under $PREFIX"
echo
echo "Now type make, make install"
