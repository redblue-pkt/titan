AC_INIT(tuxbox-apps-titan-plugins,0.0.1,,tuxbox-titan-plugins)
AM_INIT_AUTOMAKE(tuxbox-titan-plugins,0.0.1)

TUXBOX_APPS

AC_PROG_CC
AC_PROG_CXX
AM_PROG_LIBTOOL

AC_ARG_ENABLE([multicom322], 
        [AS_HELP_STRING(--enable-multicom322,    enable multicom322 stuff)], 
        [case "${enableval}" in 
                yes) enable_multicom322=true ;; 
                no) enable_multicom322=false ;; 
                *) AC_MSG_ERROR([bad value ${enableval} for --enable-multicom322]) ;; 
        esac]) 
AM_CONDITIONAL([ENABLE_MULTICOM322], [test x$enable_multicom322 = xtrue])
AC_SUBST(MULTICOM322)

AC_ARG_ENABLE([multicom324], 
        [AS_HELP_STRING(--enable-multicom324,    enable multicom324 stuff)], 
        [case "${enableval}" in 
                yes) enable_multicom324=true ;; 
                no) enable_multicom324=false ;; 
                *) AC_MSG_ERROR([bad value ${enableval} for --enable-multicom324]) ;; 
        esac]) 
AM_CONDITIONAL([ENABLE_MULTICOM324], [test x$enable_multicom324 = xtrue])
AC_SUBST(MULTICOM324)



AC_OUTPUT([
    Makefile
    browser/Makefile
    browser/netsurf/Makefile

    games/Makefile
    games/catcatch/Makefile
    games/sudoku/Makefile
    games/wins3/Makefile

    gui/Makefile
    gui/e2starter/Makefile
    gui/kodistarter/Makefile

    infos/Makefile
    infos/imdb/Makefile
    infos/imdbapi/Makefile
    infos/mboxinfo/Makefile
    infos/stock/Makefile
    infos/streaminfo/Makefile
    infos/tmdb/Makefile
    infos/tinews/Makefile
    infos/weather/Makefile
    infos/wm2018/Makefile

    network/Makefile
    network/dlna/Makefile
    network/facebook/Makefile
    network/ipsec/Makefile
    network/minisatip/Makefile
    network/networkbrowser/Makefile
    network/networkbrowser/netlib/Makefile
    network/satipclient/Makefile
    network/xupnpd/Makefile

    player/Makefile
    player/dvdplayer/Makefile
    player/gmediarender/Makefile
    player/mc/Makefile
    player/tithek/Makefile
    player/tmc/Makefile

    tools/Makefile
    tools/autotimer/Makefile
    tools/callmonitor1/Makefile
    tools/filemanager/Makefile
    tools/hello/Makefile
    tools/instar/Makefile
    tools/keylock/Makefile
    tools/kravencfg/Makefile
    tools/lcdpearl1/Makefile
    tools/lcdsamsung/Makefile
    tools/multiimage/Makefile
    tools/optimize/Makefile
    tools/panel/Makefile
    tools/permtime/Makefile
    tools/readerconfig/Makefile
    tools/rgui/Makefile
    tools/scriptexec/Makefile
    tools/sispmctl/Makefile
    tools/stopifnotused/Makefile
    tools/tiwakeup/Makefile
    tools/TopfieldVFD/Makefile
    tools/tsschnitt/Makefile
    tools/usbreset/Makefile
    tools/wlandisplay/Makefile
    tools/zapback/Makefile
])
