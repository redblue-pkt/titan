#ifndef MENU_H
#define MENU_H

//flag 1: fist call
struct skin* menu(struct skin* menu, int flag)
{
	debug(1000, "in");
	int rcret = 0, ret = 0;
	struct skin* listbox = getscreennode(menu, "listbox");
	struct skin* child = NULL;
	char* tmpstr = NULL;

	if(listbox == NULL || menu == NULL)
	{
		debug(1000, "out -> NULL detect");
		return NULL;
	}

	//check for hidden menus
	child = menu->child;
	while(child != NULL)
	{
		if(child->type & MENU)
		{	
			tmpstr = ostrcat("protect_", child->name, 0, 0);
			
			if(getconfigint(tmpstr, NULL) == 2)
				child->hidden = YES;	
			else
			{
				child->hidden = NO;	
				if(menucall(menu, child, 1) == 1)
					child->hidden = YES;	
				else
					child->hidden = NO;	
			}
				
			free(tmpstr); tmpstr = NULL;
		}
		child = child->next;
	}

	if(flag == 1)
		drawscreen(menu, 4);
	else
		drawscreen(menu, 0);
	addscreenrc(menu, listbox);
	if(listbox->select != NULL)
	{
		status.updatevfd = PAUSE;
		writevfd(listbox->select->text);
	}

	while(1)
	{
		rcret = waitrc(menu, 0, 0);

		if(rcret == getrcconfigint("rcexit", NULL)) break;
		if(rcret == getrcconfigint("rcmenu", NULL))
		{
			status.menurelease = 1;
			break;
		}

		if(rcret == getrcconfigint("rcok", NULL))
		{
			clearscreen(menu);
			ret = menucall(menu, listbox->select, 0);
			if(status.standby == 1 || status.menurelease == 1) break;

			if(ret != 0)
				textbox(_("Message"), _("Menu not implemented !"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 0, 0);
			drawscreen(menu, 0);
		}
		if(listbox->select != NULL)
			writevfd(listbox->select->text);
	}

	delownerrc(menu);
	clearscreen(menu);
	debug(1000, "out");
	return listbox->select;
}

int menucall(struct skin* menunode, struct skin* menuentry, int check)
{
	debug(1000, "in");
	void (*startplugin) (void);
	int ret = 0, pincheck = 0;
	char* tmpstr = NULL;

	if(menuentry == NULL)
	{
		debug(1000, "out -> NULL detect");
		return 1;
	}
	
	if(!(menuentry->type & MENU)) return 1;

	debug(1000, "menuentry->name=%s", menuentry->name);

	tmpstr = ostrcat("protect_", menuentry->name, 0, 0);
	pincheck = getconfigint(tmpstr, 0);
	free(tmpstr); tmpstr = NULL;
	if(pincheck == 1 && check == 0)
	{
		pincheck = screenpincheck(2, NULL);
		if(pincheck == 1)
			return 0;
	}

	if(ostrcmp("settings", menuentry->name) == 0)
	{
		if(check == 1) return 0;
		struct skin* screen = getscreen("settingsmenu");
		menu(screen, 0);
	}
	if(ostrcmp("extensionsmenu", menuentry->name) == 0)
	{
		if(check == 1) return 0;
		struct skin* screen = getscreen("extensionsmenu");
		menu(screen, 0);
	}
	if(ostrcmp("systemupdatemenu", menuentry->name) == 0)
	{
		if(check == 1) return 0;
		struct skin* screen = getscreen("systemupdatemenu");
		menu(screen, 0);
	}
	else if(ostrcmp("timermenu", menuentry->name) == 0)
	{
		if(check == 1) return 0;
		struct skin* screen = getscreen("timermenu");
		menu(screen, 0);
	}
	else if(ostrcmp("information", menuentry->name) == 0)
	{
		if(check == 1) return 0;
		struct skin* screen = getscreen("infomenu");
		menu(screen, 0);
	}
	else if(ostrcmp("channelservice", menuentry->name) == 0)
	{
		if(check == 1) return 0;
		struct skin* screen = getscreen("channelmenu");
		menu(screen, 0);
	}
	else if(ostrcmp("system", menuentry->name) == 0)
	{
		if(check == 1) return 0;
		struct skin* screen = getscreen("systemmenu");
		menu(screen, 0);
	}
	else if(ostrcmp("standby", menuentry->name) == 0)
	{
		if(check == 1) return 0;
		struct skin* screen = getscreen("standbymenu");
		menu(screen, 0);
	}
	else if(ostrcmp("harddisk", menuentry->name) == 0)
	{
		if(check == 1) return 0;
		struct skin* screen = getscreen("harddiskmenu");
		menu(screen, 0);
	}
	else if(ostrcmp("network", menuentry->name) == 0)
	{
		if(check == 1) return 0;
		struct skin* screen = getscreen("networkmenu");
		menu(screen, 0);
	}
	else if(ostrcmp("httpdsettings", menuentry->name) == 0)
	{
		if(check == 1) return 0;
		screenhttpdsettings();
	}
	else if(ostrcmp("mediaplayer", menuentry->name) == 0)
	{
		if(check == 1) return 0;
		screenplay(NULL, 0, 0);
	}
	else if(ostrcmp("restoredefault", menuentry->name) == 0)
	{
		if(check == 1) return 0;
		screenrestoredefault();
	}
	else if(ostrcmp("singleepg", menuentry->name) == 0)
	{
		if(check == 1) return 0;
		screensingleepg(NULL, NULL, 1);
	}
	else if(ostrcmp("multiepg", menuentry->name) == 0)
	{
		if(check == 1) return 0;
		screenmultiepg(NULL, NULL, 1);
	}
	else if(ostrcmp("gmultiepg", menuentry->name) == 0)
	{
		if(check == 1) return 0;
		screengmultiepg(NULL, NULL, 1);
	}
	else if(ostrcmp("formathdd", menuentry->name) == 0)
	{
		if(check == 1) return 0;
		screenharddisk(0);
	}
	else if(ostrcmp("filesystemcheck", menuentry->name) == 0)
	{
		if(check == 1) return 0;
		screenharddisk(1);
	}
	else if(ostrcmp("configurehdd", menuentry->name) == 0)
	{
		if(check == 1) return 0;
		screenharddisk(2);
	}
	else if(ostrcmp("timetosleep", menuentry->name) == 0)
	{
		if(check == 1) return 0;
		screenharddisksleep();
	}
	else if(ostrcmp("addaptersettings", menuentry->name) == 0)
	{
		if(check == 1) return 0;
		screennetwork_adapter();
	}
	else if(ostrcmp("networktest", menuentry->name) == 0)
	{
		if(check == 1) return 0;
		screennetwork_test();
	}
	else if(ostrcmp("networkrestart", menuentry->name) == 0)
	{
		if(check == 1) return 0;
		screennetwork_restart(NULL);
	}
	else if(ostrcmp("sambasettings", menuentry->name) == 0)
	{
		if(check == 1) return 0;
		screennetwork(5);
	}
	else if(ostrcmp("nfssettings", menuentry->name) == 0)
	{
		if(check == 1) return 0;
		screennetwork(6);
	}
	else if(ostrcmp("poweroff", menuentry->name) == 0)
	{
		if(check == 1) return 0;
		oshutdown(1, 1);
	}
	else if(ostrcmp("restart", menuentry->name) == 0)
	{
		if(check == 1) return 0;
		oshutdown(2, 1);
	}
	else if(ostrcmp("guirestart", menuentry->name) == 0)
	{
		if(check == 1) return 0;
		oshutdown(3, 1);
	}
	else if(ostrcmp("powerofftimer", menuentry->name) == 0)
	{
		if(check == 1) return 0;
  		screenpowerofftimer();
	}
	else if(ostrcmp("about", menuentry->name) == 0)
	{
		if(check == 1) return 0;
		ret = screenabout();
	}
	else if(ostrcmp("language", menuentry->name) == 0)
	{
		if(check == 1) return 0;
		screenlanguage(0);
	}
	else if(ostrcmp("avsettings", menuentry->name) == 0)
	{
		if(check == 1) return 0;
		screenavsettings(0);
	}
	else if(ostrcmp("timezone", menuentry->name) == 0)
	{
		if(check == 1) return 0;
		screentimezone();
	}
	else if(ostrcmp("adjust", menuentry->name) == 0)
	{
		if(check == 1) return 0;
		screenadjust();
	}
	else if(ostrcmp("skinadjust", menuentry->name) == 0)
	{
		if(check == 1) return 0;
		screenskinadjust();
	}
	else if(ostrcmp("plugins", menuentry->name) == 0)
	{
		if(check == 1) return 0;
		screenplugin();
	}
	else if(ostrcmp("recordpath", menuentry->name) == 0)
	{
		if(check == 1) return 0;
		screenrecordpath();
	}
	else if(ostrcmp("epgsettings", menuentry->name) == 0)
	{
		if(check == 1) return 0;
		screenepgsettings();
	}
	else if(ostrcmp("tunerconfig", menuentry->name) == 0)
	{
		if(check == 1) return 0;
		screentunerconfig();
	}
	else if(ostrcmp("vfdisplay", menuentry->name) == 0)
	{
		if(check == 1) return 0;
		screenvfdisplay();
	}
	else if(ostrcmp("serviceinfo", menuentry->name) == 0)
	{
		if(check == 1) return 0;
		screenserviceinfo();
	}
	else if(ostrcmp("skinselect", menuentry->name) == 0)
	{
		if(check == 1) return 0;
		screenskinselect();
	}
	else if(ostrcmp("videosettings", menuentry->name) == 0)
	{
		if(check == 1) return 0;
		screenvideosettings();
	}
	else if(ostrcmp("childprotection", menuentry->name) == 0)
	{
		if(check == 1) return 0;
		screenpin();
	}
	else if(ostrcmp("dostandby", menuentry->name) == 0)
	{
		if(check == 1) return 0;
		status.standby = 1;
	}
	else if(ostrcmp("scartrecorder", menuentry->name) == 0)
	{
		if(check == 1) return 0;
		screenscart();
	}
	else if(ostrcmp("timer", menuentry->name) == 0)
	{
		if(check == 1) return 0;
		screenrectimer();
	}
	else if(ostrcmp("streaming", menuentry->name) == 0)
	{
		if(check == 1) return 0;
		screenstreaming();
	}
	else if(ostrcmp("screensaveradjust", menuentry->name) == 0)
	{
		if(check == 1) return 0;
		screenscreensaveradjust();
	}
	else if(ostrcmp("manualsearch", menuentry->name) == 0)
	{
		if(check == 1) return 0;
		screenscanconfig(0);
	}
	else if(ostrcmp("autosearch", menuentry->name) == 0)
	{
		if(check == 1) return 0;
		screenscanconfig(1);
	}
	else if(ostrcmp("rotorsettings", menuentry->name) == 0)
	{
		if(check == 1) return 0;
		screenpossetup();
	}
	else if(ostrcmp("satfinder", menuentry->name) == 0)
	{
		if(check == 1) return 0;
		screensatfinder();
	}
	else if(ostrcmp("cinterface", menuentry->name) == 0)
	{
		if(check == 1) return 0;
		screenmoduleconfig();
	}
	else if(ostrcmp("epgsearch", menuentry->name) == 0)
	{
		if(check == 1) return 0;
		screenepgsearch();
	}
	else if(ostrcmp("channeledit", menuentry->name) == 0)
	{
		if(check == 1) return 0;
		screenchannellist(NULL, NULL, 3);
	}
	else if(ostrcmp("wlansettings", menuentry->name) == 0)
	{
		if(check == 1) return 0;
		screennetwork_wlan();
	}
	else if(ostrcmp("inadynsettings", menuentry->name) == 0)
	{
		if(check == 1) return 0;
		screennetwork_inadyn();
	}
	else if(ostrcmp("blindscanadjust", menuentry->name) == 0)
	{
		if(check == 1) return 0;
		screenblindscanadjust();
	}
	else if(ostrcmp("settings_redbutton", menuentry->name) == 0)
	{
		if(check == 1) return 0;
		screen_settings_redbutton();
	}
	else if(ostrcmp("settings_bluebutton", menuentry->name) == 0)
	{
		if(check == 1) return 0;
		screen_settings_bluebutton();
	}
	else if(ostrcmp("feed", menuentry->name) == 0)
	{
		if(check == 1) return 0;
		screen_feed();
	}
	else if(ostrcmp("extensions_ipkinstall", menuentry->name) == 0)
	{
		if(check == 1) return 0;
		screen_extensions(0, NULL);
	}
	else if(ostrcmp("extensions_ipkremove", menuentry->name) == 0)
	{
		if(check == 1) return 0;
		screen_extensions(1, NULL);
	}
	else if(ostrcmp("extensions_tmpipkinstall", menuentry->name) == 0)
	{
		if(check == 1) return 0;
		screen_extensions(2, NULL);
	}
	else if(ostrcmp("extensions_ipkupgrade", menuentry->name) == 0)
	{
		if(check == 1) return 0;
		screen_extensions(3, NULL);
	}
	else if(ostrcmp("extensions_mediaipkinstall", menuentry->name) == 0)
	{
		if(check == 1) return 0;
		screen_extensions_check(1);
	}
	else if(ostrcmp("systemupdate_flash_online", menuentry->name) == 0)
	{
		if(check == 1) return 0;
		screensystemupdate(0);
	}
	else if(ostrcmp("systemupdate_flash_tmp", menuentry->name) == 0)
	{
		if(check == 1) return 0;
		screensystemupdate(1);
	}
	else if(ostrcmp("systemupdate_usb_online", menuentry->name) == 0)
	{
		if(check == 1) return 0;
		screensystemupdate(2);
	}
	else if(ostrcmp("systemupdate_usb_tmp", menuentry->name) == 0)
	{
		if(check == 1) return 0;
		screensystemupdate(3);
	}
	else if(ostrcmp("infos_serial", menuentry->name) == 0)
	{
		if(check == 1) return 0;
		screengetserial(0);
	}
	else if(ostrcmp("infos_imageinfo", menuentry->name) == 0)
	{
		if(check == 1) return 0;
		screensysteminfo(0);
	}
	else if(ostrcmp("system_eraseswap", menuentry->name) == 0)
	{
		if(check == 1) return 0;
		screen_system_eraseswap();
	}
	else if(ostrcmp("system_wizard", menuentry->name) == 0)
	{
		if(check == 1) return 0;
		screen_system_wizard();
	}
	else if(ostrcmp("system_backup", menuentry->name) == 0)
	{
		if(check == 1) return 0;
		screen_system_backup();
	}
	else if(ostrcmp("system_backup_restore", menuentry->name) == 0)
	{
		if(check == 1) return 0;
		screen_system_backup_restore();
	}
	else if(menuentry->pluginhandle != NULL)
	{
		if(menuentry->input != NULL)
		{
			startplugin = dlsym(menuentry->pluginhandle, menuentry->input);
			if(startplugin != NULL)
			{
				if(check == 1) return 0;
				startplugin();
			}
		}
	}
	else if(ostrcmp("savesettings", menuentry->name) == 0)
	{
		if(check == 1) return 0;
		debug(10, "SAVING SETTINGS !!!");
		if(writeallconfig(1) != 0)
			textbox(_("Save Settings"), _("Error in Save Settings"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 0, 0);
		else
			textbox(_("Save Settings"), _("Settings succesfull saved"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 0, 0);
	}
	else
		ret = 1;

	debug(1000, "out");
	return ret;
}

#endif
