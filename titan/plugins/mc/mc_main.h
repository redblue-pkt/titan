#ifndef MC_MAIN_H
#define MC_MAIN_H

int mc_menucall(struct skin* menuentry)
{
	debug(1000, "in");
	int ret = 0;

	if(menuentry == NULL){
		debug(1000, "out -> NULL detect");
		return 1;
	}

	if(ostrcmp("mc_videoplayer", menuentry->name) == 0)
	{
		drawscreen(skin, 0, 0);
		screenmc_videoplayer(NULL);
		drawscreen(skin, 0, 0);
		singlepicstart("/var/usr/local/share/titan/plugins/mc/skin/bgVideo.mvi", 0);
	} 
	else if(ostrcmp("mc_audioplayer", menuentry->name) == 0)
	{
		drawscreen(skin, 0, 0);
		servicestop(status.aktservice, 1, 1);	
		screenmc_audioplayer(NULL);
		singlepicstart("/var/usr/local/share/titan/plugins/mc/skin/bgMusic.mvi", 0);
	} 
	else if(ostrcmp("mc_pictureplayer", menuentry->name) == 0)
	{
		drawscreen(skin, 0, 0);
		servicestop(status.aktservice, 1, 1);	
		screenmc_pictureplayer(NULL);
		singlepicstart("/var/usr/local/share/titan/plugins/mc/skin/bgPicture.mvi", 0);
	} 
	else if(ostrcmp("mc_appletrailer", menuentry->name) == 0)
	{
		textbox(_("Message"), _("comming soon"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 0, 0);
		screenmc_appletrailer();
	} 
	else if(ostrcmp("mc_myvideo", menuentry->name) == 0)
	{
		textbox(_("Message"), _("comming soon"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 0, 0);
		screenmc_myvideo();
	}
	else if(ostrcmp("mc_vlcplayer", menuentry->name) == 0)
	{
		textbox(_("Message"), _("comming soon"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 0, 0);
		screenmc_vlcplayer();
	}
	else if(ostrcmp("mc_dateibrowser", menuentry->name) == 0)
	{
		drawscreen(skin, 0, 0);
		servicestop(status.aktservice, 1, 1);	
		screenmc_dateibrowser(NULL);
		singlepicstart("/var/usr/local/share/titan/plugins/mc/skin/bgSettings.mvi", 0);
	}
	else if(ostrcmp("mc_internetbrowser", menuentry->name) == 0)
	{
		drawscreen(skin, 0, 0);
		servicestop(status.aktservice, 1, 1);	
		screenmc_internetbrowser(NULL);
		singlepicstart("/var/usr/local/share/titan/plugins/mc/skin/bgBrowser.mvi", 0);
	}
	else if(ostrcmp("mc_mediathek", menuentry->name) == 0)
	{
		drawscreen(skin, 0, 0);
		servicestop(status.aktservice, 1, 1);	
		screenmc_mediathek(NULL);
		singlepicstart("/var/usr/local/share/titan/plugins/mc/skin/bgVideo.mvi", 0);
	}
	else if(ostrcmp("mc_radio", menuentry->name) == 0)
	{
		textbox(_("Message"), _("comming soon"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 0, 0);	
		screenmc_radio();
	}
	else if(ostrcmp("mc_webradio", menuentry->name) == 0)
	{
		textbox(_("Message"), _("comming soon"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 0, 0);	
		screenmc_webradio();
	}
	else if(ostrcmp("mc_wetterinfo", menuentry->name) == 0)
	{
		drawscreen(skin, 0, 0);
		servicestop(status.aktservice, 1, 1);	
		screenmc_wetterinfo(NULL);
		singlepicstart("/var/usr/local/share/titan/plugins/mc/skin/bgBrowser.mvi", 0);
	}
	else if(ostrcmp("mc_settings", menuentry->name) == 0)
	{
		drawscreen(skin, 0, 0);
		servicestop(status.aktservice, 1, 1);	
		screenmc_settings(NULL);
		singlepicstart("/var/usr/local/share/titan/plugins/mc/skin/bgMusic.mvi", 0);
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
	
	system("/usr/bin/amixer -c 1 set Analog playback '40%' unmute &");
	system("/usr/bin/amixer -c 1 set SPDIF playback '40%' unmute &");
	system("/usr/bin/amixer -c 1 set HDMI playback '40%' unmute &");
	
	if(!file_exist(getconfig("mediadbpath", NULL)))
		mkdir(getconfig("mediadbpath", NULL), 0777);
		
	debug(50, "drop_caches");
	writesysint("/proc/sys/vm/drop_caches", 3, 0);
	
	debug(50, "[mc_main] umounting: fuse, (rarfs,isofs,imgfs)");
//	system("/bin/umount -fl `mount | grep fuse | grep rarfs | grep -v '/dev/' | cut -d ' ' -f3`; killall -9 rarfs");
//	system("/bin/umount -fl `mount | grep fuse | grep -v '/dev/' | cut -d ' ' -f3`; /bin/umount -fl `mount | grep iso9660 | cut -d ' ' -f3`; killall -9 rarfs fusesmb curlftpfs");
	system("umount -a -f -t fuse.rarfs,iso9660,fuse.djmount,fuse.fusesmb,fuse.curlftpfs");
	
	char* tmpstr = NULL;
	tmpstr = createpluginpath("/mc/skin.xml", 0);
	readscreen(tmpstr, 101, 1);
	free(tmpstr); tmpstr = NULL;
	
	struct skin* mc_main = NULL;
	mc_main = getscreen("mc_main_mvi"); 
		
	struct skin* listbox = getscreennode(mc_main, "listbox");
	struct skin* mc_main_selected = getscreennode(mc_main, "mc_main_selected_pic");

	singlepicstart("/var/usr/local/share/titan/plugins/mc/skin/bgMusic.mvi", 0);
			
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
				singlepicstart("/var/usr/local/share/titan/plugins/mc/skin/bgVideo.mvi", 0);
			}
			else if(ostrcmp(listbox->select->name, "mc_audioplayer") == 0)
			{
				singlepicstart("/var/usr/local/share/titan/plugins/mc/skin/bgMusic.mvi", 0);
			}
			else if(ostrcmp(listbox->select->name, "mc_pictureplayer") == 0)
			{
				singlepicstart("/var/usr/local/share/titan/plugins/mc/skin/bgPicture.mvi", 0);
			}
			else if(ostrcmp(listbox->select->name, "mc_myvideo") == 0)
			{
				singlepicstart("/var/usr/local/share/titan/plugins/mc/skin/bgBrowser.mvi", 0);
			}
			else if(ostrcmp(listbox->select->name, "mc_dateibrowser") == 0)
			{
				singlepicstart("/var/usr/local/share/titan/plugins/mc/skin/bgSettings.mvi", 0);
			}
			else if(ostrcmp(listbox->select->name, "mc_internetbrowser") == 0)
			{
				singlepicstart("/var/usr/local/share/titan/plugins/mc/skin/bgBrowser.mvi", 0);
			}
			else if(ostrcmp(listbox->select->name, "mc_mediathek") == 0)
			{
				singlepicstart("/var/usr/local/share/titan/plugins/mc/skin/bgVideo.mvi", 0);
			}
			else if(ostrcmp(listbox->select->name, "mc_wetterinfo") == 0)
			{
				singlepicstart("/var/usr/local/share/titan/plugins/mc/skin/bgBrowser.mvi", 0);
			}	
			first = 0;
		}
		
		rcret = waitrc(mc_main, 0, 1);
		if (rcret == getrcconfigint("rcexit", NULL)) break;		

		if (rcret == getrcconfigint("rcrecord", NULL))
		{
			system("grab -j 100");
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
				singlepicstart("/var/usr/local/share/titan/plugins/mc/skin/bgVideo.mvi", 0);
			}
			else if(ostrcmp(listbox->select->name, "mc_audioplayer") == 0)
			{
				singlepicstart("/var/usr/local/share/titan/plugins/mc/skin/bgMusic.mvi", 0);
			}
			else if(ostrcmp(listbox->select->name, "mc_pictureplayer") == 0)
			{
				singlepicstart("/var/usr/local/share/titan/plugins/mc/skin/bgPicture.mvi", 0);
			}
			else if(ostrcmp(listbox->select->name, "mc_myvideo") == 0)
			{
				singlepicstart("/var/usr/local/share/titan/plugins/mc/skin/bgBrowser.mvi", 0);
			}
			else if(ostrcmp(listbox->select->name, "mc_dateibrowser") == 0)
			{
				singlepicstart("/var/usr/local/share/titan/plugins/mc/skin/bgSettings.mvi", 0);
			}
			else if(ostrcmp(listbox->select->name, "mc_internetbrowser") == 0)
			{
				singlepicstart("/var/usr/local/share/titan/plugins/mc/skin/bgBrowser.mvi", 0);
			}
			else if(ostrcmp(listbox->select->name, "mc_mediathek") == 0)
			{
				singlepicstart("/var/usr/local/share/titan/plugins/mc/skin/bgVideo.mvi", 0);
			}
			else if(ostrcmp(listbox->select->name, "mc_wetterinfo") == 0)
			{
				singlepicstart("/var/usr/local/share/titan/plugins/mc/skin/bgWeather.mvi", 0);
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

	system("/usr/bin/amixer -c 1 set Analog playback '70%' unmute &");
	system("/usr/bin/amixer -c 1 set SPDIF playback '70%' unmute &");
	system("/usr/bin/amixer -c 1 set HDMI playback '70%' unmute &");

	free(videomode),videomode = NULL;
	free(currvideomode),currvideomode = NULL;
}

#endif
