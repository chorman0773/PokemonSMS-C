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


AC_CONFIG_FILES([Makefile libconinteract/Makefile random/Makefile util/Makefile util/io/Makefile util/shadenbt/Makefile util/net/Makefile sqlite/Makefile])

AC_CONFIG_COMMANDS([json-c],[
	mkdir -p json-c
	pushd $ac_top_srcdir/json-c
	./autogen.sh
	popd
	pushd ./json-c
	../$ac_top_srcdir/json-c/configure
	popd
	
])

AC_CONFIG_COMMANDS([openssl],[
	mkdir -p openssl
	pushd ./openssl
	../$ac_top_srcdir/openssl/config
	popd
])


AC_CONFIG_COMMANDS([lua],[
	cp -r $ac_top_srcdir/lua ./lua
])





AC_OUTPUT
