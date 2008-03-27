INCLUDES = 
AM_LDFLAGS = 
AM_CFLAGS = $(OPTCFLAGS)
AM_CPPFLAGS = -DLOCALEDIR=\"$(localedir)\"
LDADD = @LIBINTL@
localedir = $(datadir)/locale

bin_PROGRAMS = symultimediad
symultimediad_SOURCES = syn-multimedia.c synaptics.h \
			audio.c audio.h \
			alsa.c alsa.h \
			syn.c syn.h \
			conf.c conf.h \
			onscr.c onscr.h \
			gstreamer.c gstreamer.h \
			common.h
symultimediad_DEPENDENCIES = 
symultimediad_LDADD = -lm $(OPTLIBS)

