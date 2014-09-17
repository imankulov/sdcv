SUBDIRS=lib
bin_PROGRAMS = sdcv
sdcv_SOURCES = \
	sdcv.cpp \
	libwrapper.cpp libwrapper.hpp \
	readline.cpp readline.hpp \
	utils.cpp utils.hpp

sdcv_DEPENDENCIES = lib/libstardict.a
sdcv_LDADD =  lib/libstardict.a @SDCV_LIBS@ @LIBINTL@ @LIBREADLINE@
localedir = $(datadir)/locale

INCLUDES = @SDCV_CFLAGS@  -I$(top_builddir) -Ilib
DEFS=-DLOCALEDIR=\"$(localedir)\" @DEFS@
