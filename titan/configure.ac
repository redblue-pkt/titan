AC_INIT(tuxbox-apps-titan,0.0.1,,tuxbox-titan)
AM_INIT_AUTOMAKE(tuxbox-titan,0.0.1)

TUXBOX_BOXTYPE

AC_PROG_CC
AC_PROG_CXX
#AM_PROG_LIBTOOL

AC_ARG_ENABLE([multicom324],
	[AS_HELP_STRING(--enable-multicom324,    enable multicom324 stuff)],
	[case "${enableval}" in
		yes) enable_multicom324=true ;;
		no) enable_multicom324=false ;;
		*) AC_MSG_ERROR([bad value ${enableval} for --enable-multicom324]) ;;
	esac])
AM_CONDITIONAL([ENABLE_MULTICOM324], [test x$enable_multicom324 = xtrue])
AC_SUBST(MULTICOM324)

AC_ARG_ENABLE(eplayer3,
	AS_HELP_STRING(--enable-eplayer3,    enable eplayer3 support),
	[case "${enableval}" in
		yes) EPLAYER3=eplayer3 ;;
	esac],[enable_eplayer3=no])
AM_CONDITIONAL(ENABLE_EPLAYER3,test "$enable_eplayer3" = "yes")
AC_SUBST(EPLAYER3)

AC_MSG_RESULT([
summary:
----------------------------------------
Boxtype          : $BOXTYPE
Multicom324      : $enable_multicom324
libeplayer3      : $enable_eplayer3
])

AC_OUTPUT([
Makefile
titan/Makefile
])
