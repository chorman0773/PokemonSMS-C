AC_INIT([pokemonsms-c], [1.0], [])

AC_CANONICAL_TARGET

AC_PROG_CC
AC_PROG_INSTALL

AC_ARG_WITH([build-type],[--with-build-type=<build type>. Allows you to configure the type of build. Defaults to release. Acceptable values are debug, debug-sanitize, and release],
	[
		build_type=$withval
	],
	[
		build_type=release
	])
AC_SUBST([build_type])

shell=/bin/bash
AC_SUBST([shell])

os=${target_os}
AC_SUBST([os])


AC_CONFIG_FILES([Makefile libconinteract/Makefile random/Makefile util/Makefile util/io/Makefile util/shadenbt/Makefile util/net/Makefile util/collect/Makefile sqlite/Makefile])

AC_CONFIG_COMMANDS([json-c],[
	mkdir -p json-c
	pushd $ac_top_srcdir/json-c
	test -e ./configure || ./autogen.sh
	popd
	pushd ./json-c
	../$ac_top_srcdir/json-c/configure
	popd
	
])

AC_CONFIG_COMMANDS([openssl],[
	mkdir -p openssl
	pushd ./openssl
	../$ac_top_srcdir/openssl/config
	ln -sfv ../$ac_top_srcdir/include ./
	popd
])


AC_CONFIG_COMMANDS([lua],[
	cp -r $ac_top_srcdir/lua .
])

AC_CONFIG_COMMANDS([sqlite],[
	mkdir -p sqlite
	pushd ./sqlite
	../$ac_top_srcdir/sqlite/configure
	touch ../$ac_top_srcdir/sqlite/manifest{,.uuid} # sqlite for some reason wanted a manifest at its source directory. Make sure they exist as part of the configure steps
	popd
])



AC_CONFIG_COMMANDS([SDL],[
	mkdir -p SDL
	pushd ./SDL
	../$ac_top_srcdir/SDL/configure
	popd
])



AC_OUTPUT
