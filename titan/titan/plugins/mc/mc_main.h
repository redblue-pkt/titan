#ifndef MC_MAIN_H
#define MC_MAIN_H

int mc_menucall(struct skin* menuentry)
{
	debug(1000, "in");
	int ret = 0;
//	int style = getconfigint("style", NULL);
	int style = 0;

	if(menuentry == NULL){
		debug(1000, "out -> NULL detect");
		return 1;
	}

	if(ostrcmp("mc_videoplayer", menuentry->name) == 0)
	{
		drawscreen(skin, 0);		
		screenmc_videoplayer(NULL);
		drawscreen(skin, 0);
		if (style == 0)
			singlepicstart("/var/usr/local/share/titan/plugins/mc/skin/bgVideo.mvi");

	} 
	else if(ostrcmp("mc_audioplayer", menuentry->name) == 0)
	{
		drawscreen(skin, 0);
		servicestop(status.aktservice, 1, 1);	
		screenmc_audioplayer(NULL);
		if (style == 0)
			singlepicstart("/var/usr/local/share/titan/plugins/mc/skin/bgMusic.mvi");
	} 
	else if(ostrcmp("mc_pictureplayer", menuentry->name) == 0)
	{
		drawscreen(skin, 0);
		servicestop(status.aktservice, 1, 1);	
		screenmc_pictureplayer(NULL);
		if (style == 0)
			singlepicstart("/var/usr/local/share/titan/plugins/mc/skin/bgMusic.mvi");
	} 
	else if(ostrcmp("mc_appletrailer", menuentry->name) == 0)
	{
		textbox(_("Message"), _("comming soon"), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, NULL, 0, 600, 200, 0, 0);
		screenmc_appletrailer();
	} 
	else if(ostrcmp("mc_myvideo", menuentry->name) == 0)
	{
		textbox(_("Message"), _("comming soon"), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, NULL, 0, 600, 200, 0, 0);
		screenmc_myvideo();
	}
	else if(ostrcmp("mc_vlcplayer", menuentry->name) == 0)
	{
		textbox(_("Message"), _("comming soon"), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, NULL, 0, 600, 200, 0, 0);
		screenmc_vlcplayer();
	}
	else if(ostrcmp("mc_dateibrowser", menuentry->name) == 0)
	{
		textbox(_("Message"), _("comming soon"), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, NULL, 0, 600, 200, 0, 0);	
		screenmc_dateibrowser();
	}
	else if(ostrcmp("mc_internetbrowser", menuentry->name) == 0)
	{
		if(file_exist("/var/bin/browser/browser"))
		{
			if(textbox(_("Message"), _("start QtBrowser now ?"), _("EXIT"), getrcconfigint("rcexit", NULL), _("OK"), getrcconfigint("rcok", NULL), NULL, 0, NULL, 0, 600, 200, 0, 0) == 2){
				oshutdown(20, 1);
			}
		}
		else
			textbox(_("Message"), _("Install QtBrowser Ipk first !"), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, NULL, 0, 600, 200, 0, 0);
//		screenmc_internetbrowser();
	}
	else if(ostrcmp("mc_radio", menuentry->name) == 0)
	{
		textbox(_("Message"), _("comming soon"), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, NULL, 0, 600, 200, 0, 0);	
		screenmc_radio();
	}
	else if(ostrcmp("mc_webradio", menuentry->name) == 0)
	{
		textbox(_("Message"), _("comming soon"), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, NULL, 0, 600, 200, 0, 0);	
		screenmc_webradio();
	}
	else if(ostrcmp("mc_wetterinfo", menuentry->name) == 0)
	{
		textbox(_("Message"), _("comming soon"), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, NULL, 0, 600, 200, 0, 0);
		screenmc_wetterinfo();
	}
	else if(ostrcmp("mc_settings", menuentry->name) == 0)
	{
		drawscreen(skin, 0);
		servicestop(status.aktservice, 1, 1);	
		screenmc_settings(NULL);
		if (style == 0)
			singlepicstart("/var/usr/local/share/titan/plugins/mc/skin/bgMusic.mvi");
	}
	else
		ret = 1;

	debug(1000, "out");
	return ret;
}

void mc_main()
{
	status.hangtime = 99999;
	int rcret = 0, ret = 0;
	writevfd("Mediacenter");

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

//	int style = getconfigint("style", NULL);
	int style = 0;
	debug(50, "[mc_main] style: %d", style);
	if(style == 0)
		mc_main = getscreen("mc_main_mvi"); 
	else
		mc_main = getscreen("mc_main");
		
	struct skin* listbox = getscreennode(mc_main, "listbox");
	struct skin* mc_main_selected = getscreennode(mc_main, "mc_main_selected_pic");

	if(style == 0)
		singlepicstart("/var/usr/local/share/titan/plugins/mc/skin/bgMusic.mvi");
	else if(style == 1)
	{
		changepicmem(mc_main_selected, "%pluginpath%/mc/skin/bgVideo.jpg", 1001); 
		changepicmem(mc_main_selected, "%pluginpath%/mc/skin/bgMusic.jpg", 1002); 
		changepicmem(mc_main_selected, "%pluginpath%/mc/skin/bgPicture.jpg", 1003);
		changepicmem(mc_main_selected, "%pluginpath%/mc/skin/bgDVD.jpg", 1004);
		changepicmem(mc_main_selected, "%pluginpath%/mc/skin/bgBrowser.jpg", 1005);
		changepicmem(mc_main_selected, "%pluginpath%/mc/skin/bgVLC.jpg", 1006);
		changepicmem(mc_main_selected, "%pluginpath%/mc/skin/bgSettings.jpg", 1007);
		changepicmem(mc_main_selected, "%pluginpath%/mc/skin/bgRadio.jpg", 1008);
		changepicmem(mc_main_selected, "%pluginpath%/mc/skin/bgWeather.jpg", 1009);
	}

	drawscreen(mc_main, 0);
	addscreenrc(mc_main, listbox);

	while(1)
	{
		status.screencalc = 1;
		rcret = waitrc(mc_main, 0, 0);
		status.screencalc = 0;

		if (rcret == getrcconfigint("rcexit", NULL)) break;

// menu is slow if enable
//		writevfd(listbox->select->text);

		if(rcret == getrcconfigint("rcok", NULL))
		{
			clearscreen(mc_main);
			ret = mc_menucall(listbox->select);
			if(ret != 0)
				textbox(_("Message"), _("Menu not implemented !"), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, NULL, 0, 600, 200, 0, 0);

//			chk_style = getconfig("style", NULL);
//			debug(50, "[mc_main] chk_style: %d", chk_style);

//			style = getconfigint("style", NULL);
			style = 0;
			if(style == 0)
				mc_main = getscreen("mc_main_mvi"); 
			else
				mc_main = getscreen("mc_main");

			listbox = getscreennode(mc_main, "listbox");
			mc_main_selected = getscreennode(mc_main, "mc_main_selected_pic");

			if (style == 1)
			{
				changepicmem(mc_main_selected, "%pluginpath%/mc/skin/bgVideo.jpg", 1001); 
				changepicmem(mc_main_selected, "%pluginpath%/mc/skin/bgMusic.jpg", 1002); 
				changepicmem(mc_main_selected, "%pluginpath%/mc/skin/bgPicture.jpg", 1003);
				changepicmem(mc_main_selected, "%pluginpath%/mc/skin/bgDVD.jpg", 1004);
				changepicmem(mc_main_selected, "%pluginpath%/mc/skin/bgBrowser.jpg", 1005);
				changepicmem(mc_main_selected, "%pluginpath%/mc/skin/bgVLC.jpg", 1006);
				changepicmem(mc_main_selected, "%pluginpath%/mc/skin/bgSettings.jpg", 1007);
				changepicmem(mc_main_selected, "%pluginpath%/mc/skin/bgRadio.jpg", 1008);
				changepicmem(mc_main_selected, "%pluginpath%/mc/skin/bgWeather.jpg", 1009);
			}

			drawscreen(mc_main, 0);
			debug(50, "ret: %d", ret);
			addscreenrc(mc_main, listbox);

		}

		if(listbox->select != NULL && (rcret == getrcconfigint("rcup", NULL) || rcret == getrcconfigint("rcdown", NULL)))
		{
			if(ostrcmp(listbox->select->name, "mc_videoplayer") == 0)
			{
				if(style == 0)
					singlepicstart("/var/usr/local/share/titan/plugins/mc/skin/bgVideo.mvi");
				else
					changepicmem(mc_main_selected, "%pluginpath%/mc/skin/bgVideo.jpg", 1); 
			}
			else if(ostrcmp(listbox->select->name, "mc_audioplayer") == 0)
			{
				if(style == 0)
					singlepicstart("/var/usr/local/share/titan/plugins/mc/skin/bgMusic.mvi");
				else
					changepicmem(mc_main_selected, "%pluginpath%/mc/skin/bgMusic.jpg", 2); 
			}
			else if(ostrcmp(listbox->select->name, "mc_pictureplayer") == 0)
			{
				if(style == 0)
					singlepicstart("/var/usr/local/share/titan/plugins/mc/skin/bgPicture.mvi");
				else
					changepicmem(mc_main_selected, "%pluginpath%/mc/skin/bgPicture.jpg", 3);
			}
			else if(ostrcmp(listbox->select->name, "mc_appletrailer") == 0)
			{
				if(style == 0)
					singlepicstart("/var/usr/local/share/titan/plugins/mc/skin/bgVideo.mvi");
				else
					changepicmem(mc_main_selected, "%pluginpath%/mc/skin/bgDVD.jpg", 4);
			}
			else if(ostrcmp(listbox->select->name, "mc_myvideo") == 0)
			{
				if(style == 0)	
					singlepicstart("/var/usr/local/share/titan/plugins/mc/skin/bgBrowser.mvi");
				else
					changepicmem(mc_main_selected, "%pluginpath%/mc/skin/bgBrowser.jpg", 5);
			}
			else if(ostrcmp(listbox->select->name, "mc_vlcplayer") == 0)
			{
				if(style == 0)
					singlepicstart("/var/usr/local/share/titan/plugins/mc/skin/bgVLC.mvi");
				else
					changepicmem(mc_main_selected, "%pluginpath%/mc/skin/bgVLC.jpg", 6);
			}
			else if(ostrcmp(listbox->select->name, "mc_dateibrowser") == 0)
			{
				if(style == 0)
					singlepicstart("/var/usr/local/share/titan/plugins/mc/skin/bgSettings.mvi");
				else
					changepicmem(mc_main_selected, "%pluginpath%/mc/skin/bgSettings.jpg", 7);
			}
			else if(ostrcmp(listbox->select->name, "mc_internetbrowser") == 0)
			{
				if(style == 0)
					singlepicstart("/var/usr/local/share/titan/plugins/mc/skin/bgBrowser.mvi");
				else
					changepicmem(mc_main_selected, "%pluginpath%/mc/skin/bgBrowser.jpg", 5);
			}
			else if(ostrcmp(listbox->select->name, "mc_radio") == 0)
			{
				if(style == 0)
					singlepicstart("/var/usr/local/share/titan/plugins/mc/skin/bgRadio.mvi");
				else
					changepicmem(mc_main_selected, "%pluginpath%/mc/skin/bgRadio.jpg", 8);
			}
			else if(ostrcmp(listbox->select->name, "mc_webradio") == 0)
			{
				if(style == 0)
					singlepicstart("/var/usr/local/share/titan/plugins/mc/skin/bgRadio.mvi");
				else
					changepicmem(mc_main_selected, "%pluginpath%/mc/skin/bgRadio.jpg", 8);
			}
			else if(ostrcmp(listbox->select->name, "mc_wetterinfo") == 0)
			{
				if(style == 0)
					singlepicstart("/var/usr/local/share/titan/plugins/mc/skin/bgWeather.mvi");
				else
					changepicmem(mc_main_selected, "%pluginpath%/mc/skin/bgWeather.jpg", 9);
			}
			else if(ostrcmp(listbox->select->name, "mc_settings") == 0)
			{
				if(style == 0)
					singlepicstart("/var/usr/local/share/titan/plugins/mc/skin/bgSettings.mvi");
				else
					changepicmem(mc_main_selected, "%pluginpath%/mc/skin/bgSettings.jpg", 7);
			}
			drawscreen(mc_main, 0);
		}
	}


	delownerrc(mc_main);
	clearscreen(mc_main);
	drawscreen(skin, 0);

	if(style == 1)
	{
		delmarkedscreen(101);
		delmarkedpic(1001);
		delmarkedpic(1002);
		delmarkedpic(1003);
		delmarkedpic(1004);
		delmarkedpic(1005);
		delmarkedpic(1006);
		delmarkedpic(1007);
		delmarkedpic(1008);
		delmarkedpic(1009);
	}
//	servicestop(status.aktservice, 1, 1);
//	singlepicstart("");
	debug(50, "drop_caches");
	writesysint("/proc/sys/vm/drop_caches", 3, 0);
	debug(1000, "out");
	debug(50, "startservice: lastservice");
	servicestart(status.lastservice->channel, status.lastservice->channellist, NULL, 0);
	debug(50, "set default osdtransparent");
	setosdtransparent(getskinconfigint("osdtransparent", NULL));
	status.hangtime = getconfigint("hangtime", NULL);
}

#endif
