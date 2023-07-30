#ifndef MC_MAIN_H
#define MC_MAIN_H

int mc_menucall(struct skin* menuentry)
{
	debug(1000, "in");
	int ret = 0;
	char* tmpstr = NULL;

	if(menuentry == NULL){
		debug(1000, "out -> NULL detect");
		return 1;
	}

printf("menuentry->name: %s\n", menuentry->name);

	if(ostrcmp("mc_videoplayer", menuentry->name) == 0)
	{
		drawscreen(skin, 0, 0);
		screenmc_videoplayer(NULL);
		drawscreen(skin, 0, 0);
//		singlepicstart("/var/usr/local/share/titan/plugins/player/mc/skin/bgVideo.mvi", 0);
		tmpstr = createpluginpath("/mc/skin/bgVideo.mvi", 0);
		singlepicstart(tmpstr, 0);
		free(tmpstr); tmpstr = NULL;
	} 
	else if(ostrcmp("mc_audioplayer", menuentry->name) == 0)
	{
		drawscreen(skin, 0, 0);
		screenmc_audioplayer(NULL);
//		singlepicstart("/var/usr/local/share/titan/plugins/player/mc/skin/bgMusic.mvi", 0);
		tmpstr = createpluginpath("/mc/skin/bgMusic.mvi", 0);
		singlepicstart(tmpstr, 0);
		free(tmpstr); tmpstr = NULL;
	} 
	else if(ostrcmp("mc_iptvplayer", menuentry->name) == 0)
	{
		drawscreen(skin, 0, 0);
		screenmc_iptvplayer(NULL);
//		singlepicstart("/var/usr/local/share/titan/plugins/player/mc/skin/bgIptv.mvi", 0);
		tmpstr = createpluginpath("/mc/skin/bgIptv.mvi", 0);
		singlepicstart(tmpstr, 0);
		free(tmpstr); tmpstr = NULL;
	}
	else if(ostrcmp("mc_pictureplayer", menuentry->name) == 0)
	{
		drawscreen(skin, 0, 0);
//#ifdef MIPSEL
//		servicestop(status.aktservice, 0, 4);	
//#else
		servicestop(status.aktservice, 1, 1);	
//#endif
		screenmc_pictureplayer(NULL);
//		singlepicstart("/var/usr/local/share/titan/plugins/player/mc/skin/bgPicture.mvi", 0);
		tmpstr = createpluginpath("/mc/skin/bgPicture.mvi", 0);
		singlepicstart(tmpstr, 0);
		free(tmpstr); tmpstr = NULL;
	}
	else if(ostrcmp("mc_dateibrowser", menuentry->name) == 0)
	{
		drawscreen(skin, 0, 0);
		servicestop(status.aktservice, 1, 1);	
		screenmc_dateibrowser(NULL);
//		singlepicstart("/var/usr/local/share/titan/plugins/player/mc/skin/bgSettings.mvi", 0);
		tmpstr = createpluginpath("/mc/skin/bgSettings.mvi", 0);
		singlepicstart(tmpstr, 0);
		free(tmpstr); tmpstr = NULL;
	}
	else if(ostrcmp("mc_internetbrowser", menuentry->name) == 0)
	{
		drawscreen(skin, 0, 0);
		servicestop(status.aktservice, 1, 1);	
		screenmc_internetbrowser(NULL);
//		singlepicstart("/var/usr/local/share/titan/plugins/player/mc/skin/bgBrowser.mvi", 0);
		tmpstr = createpluginpath("/mc/skin/bgBrowser.mvi", 0);
		singlepicstart(tmpstr, 0);
		free(tmpstr); tmpstr = NULL;
	}
	else if(ostrcmp("mc_mediathek", menuentry->name) == 0)
	{
		drawscreen(skin, 0, 0);
		if(file_exist("/proc/stb/info/vumodel"))
		{
			tmpstr = createpluginpath("/mc/skin/black.mvi", 0);
			singlepicstart(tmpstr, 0);
			free(tmpstr); tmpstr = NULL;
			sleep(1);
		}
		servicestop(status.aktservice, 1, 1);	
		screenmc_mediathek(NULL);
//		singlepicstart("/var/usr/local/share/titan/plugins/player/mc/skin/bgVideo.mvi", 0);
		tmpstr = createpluginpath("/mc/skin/bgVideo.mvi", 0);
		singlepicstart(tmpstr, 0);
		free(tmpstr); tmpstr = NULL;
	}
	else if(ostrcmp("mc_wetterinfo", menuentry->name) == 0)
	{
		drawscreen(skin, 0, 0);
		servicestop(status.aktservice, 1, 1);	
		screenmc_wetterinfo(NULL);
//		singlepicstart("/var/usr/local/share/titan/plugins/player/mc/skin/bgBrowser.mvi", 0);
		tmpstr = createpluginpath("/mc/skin/bgBrowser.mvi", 0);
		singlepicstart(tmpstr, 0);
		free(tmpstr); tmpstr = NULL;
	}
	else if(ostrcmp("mc_settings", menuentry->name) == 0)
	{
		drawscreen(skin, 0, 0);
		servicestop(status.aktservice, 1, 1);	
		screenmc_settings(NULL);
//		singlepicstart("/var/usr/local/share/titan/plugins/player/mc/skin/bgMusic.mvi", 0);
		tmpstr = createpluginpath("/mc/skin/bgMusic.mvi", 0);
		singlepicstart(tmpstr, 0);
		free(tmpstr); tmpstr = NULL;
	}
	else
		ret = 1;

	debug(1000, "out");
	return ret;
}

void mc_main()
{
	status.mcaktiv = 1;
	status.hangtime = 99999;
	int rcret = 0, ret = 0;
	char* videomode = NULL, *currvideomode = NULL;

	int savevol = getvol();
	set_player_sound(0);

	writevfdmenu("Mediacenter");
	resettvpic();
	videomode = getvideomode();
	debug(50, "save videomode: %s", videomode);

	debug(50, "setfbtransparent 255");
	setfbtransparent(255);
	debug(50, "servicestop: lastservice");
	ret = servicestop(status.aktservice, 1, 1);
	if(ret == 1) return;
	ret = 0;

	if(getconfigint("emucontrol", NULL) == 1)
		system("emu.sh halt");
	
	if(!file_exist(getconfig("mediadbpath", NULL)))
		mkdir(getconfig("mediadbpath", NULL), 0777);
		
	debug(50, "drop_caches");
	writesysint("/proc/sys/vm/drop_caches", 3, 0);
	
	debug(50, "[mc_main] umounting: fuse, (rarfs,isofs,imgfs)");
	system("umount -fl `mount | grep fuse | grep rarfs | grep -v '/dev/' | cut -d ' ' -f3` > /dev/null 2>&1; killall -9 rarfs > /dev/null 2>&1");
	system("umount -fl `mount | grep fuse | grep -v '/dev/' | cut -d ' ' -f3` > /dev/null 2>&1; /bin/umount -fl `mount | grep iso9660 | cut -d ' ' -f3` `mount | grep udf | cut -d ' ' -f3` > /dev/null 2>&1; killall -9 rarfs fusesmb curlftpfs > /dev/null 2>&1");
	system("umount -a -f -t fuse.rarfs,iso9660,udf,fuse.djmount,fuse.fusesmb,fuse.curlftpfs > /dev/null 2>&1");
printf("start1\n");
#ifdef OEBUILD
//	system("/etc/init.d/djmount start > /dev/null 2>&1");
//	system("/etc/init.d/smbnetfs.sh start > /dev/null 2>&1");
printf("start2\n");
	system("/etc/init.d/djmount start");
	system("/etc/init.d/smbnetfs.sh start");
#endif
	char* tmpstr = NULL;
	tmpstr = createpluginpath("/mc/skin.xml", 0);
	readscreen(tmpstr, 101, 1);
	free(tmpstr); tmpstr = NULL;
	
	struct skin* mc_main = NULL;
	mc_main = getscreen("mc_main_mvi"); 
		
	struct skin* listbox = getscreennode(mc_main, "listbox");
	listbox->width += status.fontsizeadjust;

	struct skin* mc_main_selected = getscreennode(mc_main, "mc_main_selected_pic");

	struct skin* mc_audioplayer = getscreennode(mc_main, "mc_audioplayer");
	struct skin* mc_videoplayer = getscreennode(mc_main, "mc_videoplayer");
	struct skin* mc_pictureplayer = getscreennode(mc_main, "mc_pictureplayer");
	struct skin* mc_mediathek = getscreennode(mc_main, "mc_mediathek");
	struct skin* mc_iptvplayer = getscreennode(mc_main, "mc_iptvplayer");
	struct skin* mc_dateibrowser = getscreennode(mc_main, "mc_dateibrowser");
	struct skin* mc_internetbrowser = getscreennode(mc_main, "mc_internetbrowser");
	struct skin* mc_wetterinfo = getscreennode(mc_main, "mc_wetterinfo");

//	singlepicstart("/var/usr/local/share/titan/plugins/player/mc/skin/bgMusic.mvi", 0);
	tmpstr = createpluginpath("/mc/skin/bgMusic.mvi", 0);
	singlepicstart(tmpstr, 0);
	free(tmpstr); tmpstr = NULL;

	mc_audioplayer->usesavebg = 2;
	mc_videoplayer->usesavebg = 2;
	mc_pictureplayer->usesavebg = 2;
	mc_mediathek->usesavebg = 2;
	mc_iptvplayer->usesavebg = 2;
	mc_dateibrowser->usesavebg = 2;
	mc_internetbrowser->usesavebg = 2;
	mc_wetterinfo->usesavebg = 2;

	mc_audioplayer->height += status.fontsizeadjust;
	mc_videoplayer->height += status.fontsizeadjust;
	mc_pictureplayer->height += status.fontsizeadjust;
	mc_mediathek->height += status.fontsizeadjust;
	mc_iptvplayer->height += status.fontsizeadjust;
	mc_dateibrowser->height += status.fontsizeadjust;
	mc_internetbrowser->height += status.fontsizeadjust;
	mc_wetterinfo->height += status.fontsizeadjust;

	drawscreen(mc_main, 0, 0);
	addscreenrc(mc_main, listbox);
	int first = 1;

	while(1)
	{
		if(first == 1 && listbox->select != NULL)
		{
	// menu is slow if enable
			writevfdmenu(listbox->select->text);
		
			if(ostrcmp(listbox->select->name, "mc_videoplayer") == 0)
			{
//				singlepicstart("/var/usr/local/share/titan/plugins/player/mc/skin/bgVideo.mvi", 0);
				tmpstr = createpluginpath("/mc/skin/bgVideo.mvi", 0);
				singlepicstart(tmpstr, 0);
				free(tmpstr); tmpstr = NULL;
			}
			else if(ostrcmp(listbox->select->name, "mc_audioplayer") == 0)
			{
//				singlepicstart("/var/usr/local/share/titan/plugins/player/mc/skin/bgMusic.mvi", 0);
				tmpstr = createpluginpath("/mc/skin/bgMusic.mvi", 0);
				singlepicstart(tmpstr, 0);
				free(tmpstr); tmpstr = NULL;
			}
			else if(ostrcmp(listbox->select->name, "mc_pictureplayer") == 0)
			{
//				singlepicstart("/var/usr/local/share/titan/plugins/player/mc/skin/bgPicture.mvi", 0);
				tmpstr = createpluginpath("/mc/skin/bgPicture.mvi", 0);
				singlepicstart(tmpstr, 0);
				free(tmpstr); tmpstr = NULL;
			}
			else if(ostrcmp(listbox->select->name, "mc_iptvplayer") == 0)
			{
//				singlepicstart("/var/usr/local/share/titan/plugins/player/mc/skin/bgIptv.mvi", 0);
				tmpstr = createpluginpath("/mc/skin/bgIptv.mvi", 0);
				singlepicstart(tmpstr, 0);
				free(tmpstr); tmpstr = NULL;
			}
			else if(ostrcmp(listbox->select->name, "mc_dateibrowser") == 0)
			{
//				singlepicstart("/var/usr/local/share/titan/plugins/player/mc/skin/bgSettings.mvi", 0);
				tmpstr = createpluginpath("/mc/skin/bgSettings.mvi", 0);
				singlepicstart(tmpstr, 0);
				free(tmpstr); tmpstr = NULL;
			}
			else if(ostrcmp(listbox->select->name, "mc_internetbrowser") == 0)
			{
//				singlepicstart("/var/usr/local/share/titan/plugins/player/mc/skin/bgBrowser.mvi", 0);
				tmpstr = createpluginpath("/mc/skin/bgBrowser.mvi", 0);
				singlepicstart(tmpstr, 0);
				free(tmpstr); tmpstr = NULL;
			}
			else if(ostrcmp(listbox->select->name, "mc_mediathek") == 0)
			{
//				singlepicstart("/var/usr/local/share/titan/plugins/player/mc/skin/bgVideo.mvi", 0);
				tmpstr = createpluginpath("/mc/skin/bgVideo.mvi", 0);
				singlepicstart(tmpstr, 0);
				free(tmpstr); tmpstr = NULL;
			}
			else if(ostrcmp(listbox->select->name, "mc_wetterinfo") == 0)
			{
//				singlepicstart("/var/usr/local/share/titan/plugins/player/mc/skin/bgBrowser.mvi", 0);
				tmpstr = createpluginpath("/mc/skin/bgBrowser.mvi", 0);
				singlepicstart(tmpstr, 0);
				free(tmpstr); tmpstr = NULL;
			}	
			first = 0;
		}
		
		rcret = waitrc(mc_main, 0, 1);
		if (rcret == getrcconfigint("rcexit", NULL)) break;		

		if (rcret == getrcconfigint("rcrecord", NULL) || rcret == getrcconfigint("rcshoot", NULL))
		{
            char* cmd = NULL;
	        if(checkchipset("3798MV200") == 1)
        		cmd = ostrcat("grab -v -j 100 -r 960 > /tmp/screenshot.jpg", NULL, 0, 0);
            else
        		cmd = ostrcat("grab -v -j 100 -r 160:120 /tmp/screenshot.jpg", NULL, 0, 0);
		    debug(133, "cmd: %s", cmd);
            system(cmd);
            free(cmd), cmd = NULL;
			textbox(_("Message"), _("Shooting Background done !\nSave Screenshoot Path: /tmp/screenshot.jpg"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 800, 200, 0, 0);
			drawscreen(mc_main, 0, 0);
		}

		if(rcret == getrcconfigint("rcok", NULL))
		{
			clearscreen(mc_main);
			ret = mc_menucall(listbox->select);
			if(ret != 0)
				textbox(_("Message"), _("Menu not implemented !"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 0, 0);

			mc_main = getscreen("mc_main_mvi"); 

			listbox = getscreennode(mc_main, "listbox");
			mc_main_selected = getscreennode(mc_main, "mc_main_selected_pic");

			drawscreen(mc_main, 0, 0);
			debug(50, "ret: %d", ret);
			addscreenrc(mc_main, listbox);

		}

		drawscreen(mc_main, 0, 0);
// menu is slow if enable
		writevfdmenu(listbox->select->text);
		
		if(listbox->select != NULL && (rcret == getrcconfigint("rcleft", NULL) || rcret == getrcconfigint("rcright", NULL) || rcret == getrcconfigint("rcup", NULL) || rcret == getrcconfigint("rcdown", NULL)))
		{
			if(ostrcmp(listbox->select->name, "mc_videoplayer") == 0)
			{
//				singlepicstart("/var/usr/local/share/titan/plugins/player/mc/skin/bgVideo.mvi", 0);
				tmpstr = createpluginpath("/mc/skin/bgVideo.mvi", 0);
				singlepicstart(tmpstr, 0);
				free(tmpstr); tmpstr = NULL;
			}
			else if(ostrcmp(listbox->select->name, "mc_audioplayer") == 0)
			{
//				singlepicstart("/var/usr/local/share/titan/plugins/player/mc/skin/bgMusic.mvi", 0);
				tmpstr = createpluginpath("/mc/skin/bgMusic.mvi", 0);
				singlepicstart(tmpstr, 0);
				free(tmpstr); tmpstr = NULL;
			}
			else if(ostrcmp(listbox->select->name, "mc_pictureplayer") == 0)
			{
//				singlepicstart("/var/usr/local/share/titan/plugins/player/mc/skin/bgPicture.mvi", 0);
				tmpstr = createpluginpath("/mc/skin/bgPicture.mvi", 0);
				singlepicstart(tmpstr, 0);
				free(tmpstr); tmpstr = NULL;
			}
			else if(ostrcmp(listbox->select->name, "mc_iptvplayer") == 0)
			{
//				singlepicstart("/var/usr/local/share/titan/plugins/player/mc/skin/bgIptv.mvi", 0);
				tmpstr = createpluginpath("/mc/skin/bgIptv.mvi", 0);
				singlepicstart(tmpstr, 0);
				free(tmpstr); tmpstr = NULL;
			}
			else if(ostrcmp(listbox->select->name, "mc_dateibrowser") == 0)
			{
//				singlepicstart("/var/usr/local/share/titan/plugins/player/mc/skin/bgSettings.mvi", 0);
				tmpstr = createpluginpath("/mc/skin/bgSettings.mvi", 0);
				singlepicstart(tmpstr, 0);
				free(tmpstr); tmpstr = NULL;
			}
			else if(ostrcmp(listbox->select->name, "mc_internetbrowser") == 0)
			{
//				singlepicstart("/var/usr/local/share/titan/plugins/player/mc/skin/bgBrowser.mvi", 0);
				tmpstr = createpluginpath("/mc/skin/bgBrowser.mvi", 0);
				singlepicstart(tmpstr, 0);
				free(tmpstr); tmpstr = NULL;
			}
			else if(ostrcmp(listbox->select->name, "mc_mediathek") == 0)
			{
//				singlepicstart("/var/usr/local/share/titan/plugins/player/mc/skin/bgVideo.mvi", 0);
				tmpstr = createpluginpath("/mc/skin/bgVideo.mvi", 0);
				singlepicstart(tmpstr, 0);
				free(tmpstr); tmpstr = NULL;
			}
			else if(ostrcmp(listbox->select->name, "mc_wetterinfo") == 0)
			{
//				singlepicstart("/var/usr/local/share/titan/plugins/player/mc/skin/bgWeather.mvi", 0);
				tmpstr = createpluginpath("/mc/skin/bgWeather.mvi", 0);
				singlepicstart(tmpstr, 0);
				free(tmpstr); tmpstr = NULL;
			}
			drawscreen(mc_main, 0, 0);
		}
	}


	delownerrc(mc_main);
	clearscreen(mc_main);
	drawscreen(skin, 0, 0);

	debug(50, "drop_caches");
	writesysint("/proc/sys/vm/drop_caches", 3, 0);
	debug(1000, "out");
	debug(50, "startservice: lastservice");
	tmpstr = ostrcat(status.lastservice->channellist, NULL, 0, 0);
	servicestart(status.lastservice->channel, tmpstr, NULL, 0);
	free(tmpstr); tmpstr = NULL;
	debug(50, "set default osdtransparent");
	setosdtransparent(getskinconfigint("osdtransparent", NULL));
	status.hangtime = getconfigint("hangtime", NULL);
	status.mcaktiv = 0;

	if(getconfigint("emucontrol", NULL) == 1)
		system("emu.sh unhalt");
		
	if(checkbox("DM900") == 1 || checkbox("DM7020HD") == 1 || checkbox("DM7020HDV2") == 1)
	{
		servicestop(status.aktservice, 1, 1);
		tmpstr = ostrcat(status.lastservice->channellist, NULL, 0, 0);
		servicestart(status.lastservice->channel, tmpstr, NULL, 0);
		free(tmpstr); tmpstr = NULL;
	}

	currvideomode = getvideomode();
	if(videomode != NULL && currvideomode != NULL && ostrcmp(videomode, currvideomode) != 0)
	{
		debug(50, "detected videomode change: %s > %s", videomode, currvideomode);
		debug(50, "reset videomode: %s", videomode);
		setvideomode(videomode, 0); 
		changefbresolution(videomode, 0);
		writeallconfig(1);
		if((ostrncmp("576", videomode, 3) == 0) || (ostrncmp("pal", videomode, 3) == 0))
			writesys("/var/etc/.scart", "0", 0);
		else
			unlink("/var/etc/.scart");
	}

	set_player_sound(1);
	setvol(savevol);

	free(videomode),videomode = NULL;
	free(currvideomode),currvideomode = NULL;
}

#endif
