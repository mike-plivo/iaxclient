ACLOCAL_AMFLAGS = -I m4

SUBDIRS = lib simpleclient
DIST_SUBDIRS = lib simpleclient

m4datadir = $(datadir)/aclocal

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = iaxclient.pc

EXTRA_DIST = \
	README.VisualStudio	\
	autogen.sh		\
	contrib			\
	doc			\
	Doxyfile		\
	m4

dist-hook:
	find $(distdir) "(" -name ".svn" -o -name ".deps" ")" \
			-type d -print0 | xargs -0 rm -rf
	find $(distdir) "(" -name "*.lo" -o -name ".dirstamp" ")" \
			-type f -print0 | xargs -0 rm -f
