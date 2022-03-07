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
    browser_netsurf/Makefile

    games_catcatch/Makefile
    games_sudoku/Makefile
    games_wins3/Makefile

    gui_e2starter/Makefile
    gui_kodistarter/Makefile

    infos_imdb/Makefile
    infos_imdbapi/Makefile
    infos_mboxinfo/Makefile
    infos_stock/Makefile
    infos_streaminfo/Makefile
    infos_tmdb/Makefile
    infos_tinews/Makefile
    infos_weather/Makefile
    infos_wm2018/Makefile

    network_dlna/Makefile
    network_facebook/Makefile
    network_ipsec/Makefile
    network_minisatip/Makefile
    network_networkbrowser/Makefile
    network_networkbrowser/netlib/Makefile
    network_satipclient/Makefile
    network_xupnpd/Makefile

    player_dvdplayer/Makefile
    player_gmediarender/Makefile
    player_mc/Makefile
    player_tithek/Makefile
    player_tmc/Makefile

    tools_autotimer/Makefile
    tools_callmonitor1/Makefile
    tools_filemanager/Makefile
    tools_hello/Makefile
    tools_instar/Makefile
    tools_keylock/Makefile
    tools_kravencfg/Makefile
    tools_lcdpearl1/Makefile
    tools_lcdsamsung/Makefile
    tools_multiimage/Makefile
    tools_optimize/Makefile
    tools_panel/Makefile
    tools_permtime/Makefile
    tools_readerconfig/Makefile
    tools_rgui/Makefile
    tools_scriptexec/Makefile
    tools_sispmctl/Makefile
    tools_stopifnotused/Makefile
    tools_tiwakeup/Makefile
    tools_TopfieldVFD/Makefile
    tools_tsSchnitt/Makefile
    tools_usbreset/Makefile
    tools_wlandisplay/Makefile
    tools_zapback/Makefile
])
