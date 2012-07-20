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
		singlepicstart("/var/usr/local/share/titan/plugins/mc/skin/bgMusic.mvi", 0);
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
		textbox(_("Message"), _("comming soon"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 0, 0);	
		screenmc_dateibrowser();
	}
	else if(ostrcmp("mc_internetbrowser", menuentry->name) == 0)
	{
		drawscreen(skin, 0, 0);
		servicestop(status.aktservice, 1, 1);	
		screenmc_internetbrowser(NULL);
		singlepicstart("/var/usr/local/share/titan/plugins/mc/skin/bgBrowser.mvi", 0);
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
		textbox(_("Message"), _("comming soon"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 0, 0);
		screenmc_wetterinfo();
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
	writevfd("Mediacenter");
	resettvpic();

	debug(50, "setfbtransparent 255");
	setfbtransparent(255);
	debug(50, "servicestop: lastservice");
	ret = servicestop(status.aktservice, 1, 1);
	if(ret == 1) return;
	ret = 0;

	debug(50, "drop_caches");
	writesysint("/proc/sys/vm/drop_caches", 3, 0);
	
	debug(50, "[mc_main] umounting: fuse, (rarfs,isofs,imgfs)");
	system("/bin/umount -fl `mount | grep fuse | grep rarfs | grep -v '/dev/' | cut -d ' ' -f3`; killall -9 rarfs");
	system("/bin/umount -fl `mount | grep fuse | grep -v '/dev/' | cut -d ' ' -f3`; /bin/umount -fl `mount | grep iso9660 | cut -d ' ' -f3`; killall -9 rarfs fusesmb curlftpfs");

	char* tmpstr = NULL;
	tmpstr = ostrcat(getconfig("pluginpath", NULL), "/mc/skin.xml", 0, 0);
	readscreen(tmpstr, 101, 1);
	free(tmpstr); tmpstr = NULL;
	
	struct skin* mc_main = NULL;
	mc_main = getscreen("mc_main_mvi"); 
		
	struct skin* listbox = getscreennode(mc_main, "listbox");
	struct skin* mc_main_selected = getscreennode(mc_main, "mc_main_selected_pic");

	singlepicstart("/var/usr/local/share/titan/plugins/mc/skin/bgMusic.mvi", 0);

	drawscreen(mc_main, 0, 0);
	addscreenrc(mc_main, listbox);

	while(1)
	{
		rcret = waitrc(mc_main, 0, 1);

		if (rcret == getrcconfigint("rcexit", NULL)) break;

// menu is slow if enable
//		writevfd(listbox->select->text);

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
		else if(rcret == getrcconfigint("rcgreen", NULL))
		{
			mediadbscan("/media", 0);
		}	
		if(listbox->select != NULL && (rcret == getrcconfigint("rcup", NULL) || rcret == getrcconfigint("rcdown", NULL)))
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
			else if(ostrcmp(listbox->select->name, "mc_appletrailer") == 0)
			{
				singlepicstart("/var/usr/local/share/titan/plugins/mc/skin/bgVideo.mvi", 0);
			}
			else if(ostrcmp(listbox->select->name, "mc_myvideo") == 0)
			{
				singlepicstart("/var/usr/local/share/titan/plugins/mc/skin/bgBrowser.mvi", 0);
			}
			else if(ostrcmp(listbox->select->name, "mc_vlcplayer") == 0)
			{
				singlepicstart("/var/usr/local/share/titan/plugins/mc/skin/bgVLC.mvi", 0);
			}
			else if(ostrcmp(listbox->select->name, "mc_dateibrowser") == 0)
			{
				singlepicstart("/var/usr/local/share/titan/plugins/mc/skin/bgSettings.mvi", 0);
			}
			else if(ostrcmp(listbox->select->name, "mc_internetbrowser") == 0)
			{
				singlepicstart("/var/usr/local/share/titan/plugins/mc/skin/bgBrowser.mvi", 0);
			}
			else if(ostrcmp(listbox->select->name, "mc_radio") == 0)
			{
				singlepicstart("/var/usr/local/share/titan/plugins/mc/skin/bgRadio.mvi", 0);
			}
			else if(ostrcmp(listbox->select->name, "mc_webradio") == 0)
			{
				singlepicstart("/var/usr/local/share/titan/plugins/mc/skin/bgRadio.mvi", 0);
			}
			else if(ostrcmp(listbox->select->name, "mc_wetterinfo") == 0)
			{
				singlepicstart("/var/usr/local/share/titan/plugins/mc/skin/bgWeather.mvi", 0);
			}
			else if(ostrcmp(listbox->select->name, "mc_settings") == 0)
			{
				singlepicstart("/var/usr/local/share/titan/plugins/mc/skin/bgSettings.mvi", 0);
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
}

#endif
