#ifndef MENU_H
#define MENU_H

void menucheckentry(struct skin* child)
{
	struct dvbdev* dvbnode = dvbdev;

	if(child == NULL) return;

	// Hide SoftCam Panel when no Emu's installed
	if(ostrcmp(child->name, "softcam") == 0 || ostrcmp(child->name, "autostart_emu") == 0)
	{
		if(status.security == 0 || checkemu() == 0)
			child->hidden = YES;
		else
			child->hidden = NO;

		return;
	}

	// Hide Cable scan when no cable tuner is installed
	if(ostrcmp(child->name, "manualsearch_dvbc") == 0)
	{
		child->hidden = YES;
		while(dvbnode != NULL)
		{
			if(dvbnode->type == FRONTENDDEV && dvbnode->feinfo != NULL && dvbnode->feinfo->type == FE_QAM)
			{
				child->hidden = NO;
				break;
			}
			dvbnode = dvbnode->next;
		}
		return;
	}

	// Hide Terr scan when no terr tuner is installed
	if(ostrcmp(child->name, "manualsearch_dvbt") == 0)
	{
		child->hidden = YES;
		while(dvbnode != NULL)
		{
			if(dvbnode->type == FRONTENDDEV && dvbnode->feinfo != NULL && dvbnode->feinfo->type == FE_OFDM)
			{
				child->hidden = NO;
				break;
			}
			dvbnode = dvbnode->next;
		}
		return;
	}

	// Hide Sat scan when no sat tuner is installed
	if(ostrcmp(child->name, "manualsearch") == 0)
	{
		child->hidden = YES;
		while(dvbnode != NULL)
		{
			if(dvbnode->type == FRONTENDDEV && dvbnode->feinfo != NULL && dvbnode->feinfo->type == FE_QPSK)
			{
				child->hidden = NO;
				break;
			}
			dvbnode = dvbnode->next;
		}
		return;
	}

	// Hide rotor setup when no sat tuner is installed
	if(ostrcmp(child->name, "rotorsettings") == 0)
	{
		child->hidden = YES;
		while(dvbnode != NULL)
		{
			if(dvbnode->type == FRONTENDDEV && dvbnode->feinfo != NULL && dvbnode->feinfo->type == FE_QPSK)
			{
				child->hidden = NO;
				break;
			}
			dvbnode = dvbnode->next;
		}
		return;
	}
}

//flag 1: fist call
struct skin* menu(struct skin* menu, int flag)
{
	int rcret = 0, ret = 0;
	struct skin* listbox = getscreennode(menu, "listbox");
	struct skin* menutext = getscreennode(menu, "menutext");
	struct skin* details = getscreennode(menu, "details");

	struct skin* child = NULL;
	char* tmpstr = NULL;

	if(listbox == NULL || menu == NULL)
	{
		err("NULL detect");
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

		menucheckentry(child);
		child = child->next;
	}

	if(flag == 1)
		drawscreen(menu, 0, 4);
	else
		drawscreen(menu, 0, 0);
	addscreenrc(menu, listbox);
	if(listbox->select != NULL)
	{
		status.updatevfd = PAUSE;
		writevfdmenu(listbox->select->text);
		if(menutext != status.skinerr) changetext(menutext,_(listbox->select->text));
		if(details != status.skinerr) changetext(details, listbox->select->text2);
	}

	while(1)
	{
		rcret = waitrc(menu, 0, 2);

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
			drawscreen(menu, 0, 0);
		}

/*
		if(rcret == getrcconfigint("rcpower", NULL))
		{
			subtitlepause(1);
			status.infobar = 0;
			status.infobaraktiv = 0;
			clearscreen(menu);
			drawscreen(skin, 0, 0);
			switch(getconfigint("poweraktion", NULL))
			{
				case 1: //power off
					oshutdown(1, 1);
					break;
				case 2: //standby
					status.standby = 1;
					break;
				case 3: //restart
					oshutdown(2, 1);
					break;
				case 4: //Gui restart
					oshutdown(3, 1);
					break;
				default:
//not working !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
					listbox->select = "standby";
					ret = menucall(menu, listbox->select, 0);
					if(status.standby == 1 || status.menurelease == 1) break;
		
					if(ret != 0)
						textbox(_("Message"), _("Menu not implemented !"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 0, 0);
					drawscreen(menu, 0, 0);
					break;
			}
			status.updatevfd = START;
			drawscreen(skin, 0, 0);
			status.infobaraktiv = 1;
			subtitlepause(0);
			continue;
		}
*/
		if(rcret == getrcconfigint("rcshoot", NULL))
		{
			subtitlepause(1);
			status.infobar = 0;
			status.infobaraktiv = 0;
			clearscreen(menu);
			screenshoot(0);
			status.infobaraktiv = 1;
			status.updatevfd = START;
			drawscreen(skin, 0, 0);
			subtitlepause(0);
			textbox(_("Message"), _("Shooting Background done !\nSave Screenshoot Path: /tmp/screenshot.jpg"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 800, 200, 0, 0);
			continue;
		}
	
		if(listbox->select != NULL)
		{
			writevfdmenu(listbox->select->text);
			if(menutext != status.skinerr) changetext(menutext,_(listbox->select->text));
			if(details != status.skinerr) changetext(details, listbox->select->text2);
		}
		drawscreen(menu, 0, 0);	
	}

	resettvpic();

	delownerrc(menu);
	clearscreen(menu);
	return listbox->select;
}

int menucall(struct skin* menunode, struct skin* menuentry, int check)
{
	void (*startplugin) (void);
	int ret = 0, pincheck = 0;
	char* tmpstr = NULL;

	if(menuentry == NULL)
	{
		err("NULL detect");
		return 1;
	}
	
	if(!(menuentry->type & MENU)) return 1;
	
	debug(100, "menuentry->name=%s", menuentry->name);
		
	tmpstr = ostrcat("protect_", menuentry->name, 0, 0);
	pincheck = getconfigint(tmpstr, 0);
	free(tmpstr); tmpstr = NULL;
	if(pincheck == 1 && check == 0)
	{
		pincheck = screenpincheck(2, NULL);
		if(pincheck == 1)
			return 0;
	}

	if(ostrcmp("mainmenulist", menuentry->name) == 0)
	{
		if(check == 1) return 0;
		resettvpic();
		struct skin* screen = getscreen("mainmenulist");
		menu(screen, 0);
	}
	else if(ostrcmp("settings", menuentry->name) == 0)
	{
		if(check == 1) return 0;
		resettvpic();
		struct skin* screen = getscreen("settingsmenu");
		menu(screen, 0);
	}
	else if(ostrcmp("autostart", menuentry->name) == 0)
	{
		if(check == 1) return 0;
		resettvpic();
		struct skin* screen = getscreen("autostartmenu");
		menu(screen, 0);
	}
	else if(ostrcmp("settings_grid", menuentry->name) == 0)
	{
		if(check == 1) return 0;
		struct skin* screen = getscreen("settingsmenu_grid");
		menu(screen, 0);
	}
	else if(ostrcmp("extensionsmenu", menuentry->name) == 0)
	{
		if(check == 1) return 0;
		resettvpic();
		struct skin* screen = getscreen("extensionsmenu");
		menu(screen, 0);
	}
	else if(ostrcmp("extensionsmenu_grid", menuentry->name) == 0)
	{
		if(check == 1) return 0;
		struct skin* screen = getscreen("extensionsmenu_grid");
		menu(screen, 0);
	}
	else if(ostrcmp("systemupdatemenu", menuentry->name) == 0)
	{
		if(check == 1) return 0;
		resettvpic();
		struct skin* screen = getscreen("systemupdatemenu");
		menu(screen, 0);
	}
	else if(ostrcmp("systemupdatemenu_grid", menuentry->name) == 0)
	{
		if(check == 1) return 0;
		struct skin* screen = getscreen("systemupdatemenu_grid");
		menu(screen, 0);
	}
	else if(ostrcmp("timermenu", menuentry->name) == 0)
	{
		if(check == 1) return 0;
		resettvpic();
		struct skin* screen = getscreen("timermenu");
		menu(screen, 0);
	}
	else if(ostrcmp("timermenu_grid", menuentry->name) == 0)
	{
		if(check == 1) return 0;
		struct skin* screen = getscreen("timermenu_grid");
		menu(screen, 0);
	}
	else if(ostrcmp("information", menuentry->name) == 0)
	{
		if(check == 1) return 0;
		resettvpic();
		struct skin* screen = getscreen("infomenu");
		menu(screen, 0);
	}
	else if(ostrcmp("information_grid", menuentry->name) == 0)
	{
		if(check == 1) return 0;
		struct skin* screen = getscreen("infomenu_grid");
		menu(screen, 0);
	}
	else if(ostrcmp("channelservice", menuentry->name) == 0)
	{
		if(check == 1) return 0;
		resettvpic();
		struct skin* screen = getscreen("channelmenu");
		menu(screen, 0);
	}
	else if(ostrcmp("channelservice_grid", menuentry->name) == 0)
	{
		if(check == 1) return 0;
		struct skin* screen = getscreen("channelmenu_grid");
		menu(screen, 0);
	}
	else if(ostrcmp("system", menuentry->name) == 0)
	{
		if(check == 1) return 0;
		resettvpic();
		struct skin* screen = getscreen("systemmenu");
		menu(screen, 0);
	}
	else if(ostrcmp("setdefaultsettings", menuentry->name) == 0)
	{
		if(check == 1) return 0;
		setdefaultsettings();
	}
	else if(ostrcmp("system_grid", menuentry->name) == 0)
	{
		if(check == 1) return 0;
		struct skin* screen = getscreen("systemmenu_grid");
		menu(screen, 0);
	}
	else if(ostrcmp("standby", menuentry->name) == 0)
	{
		if(check == 1) return 0;
		resettvpic();
		struct skin* screen = getscreen("standbymenu");
		menu(screen, 0);
	}
	else if(ostrcmp("standby_grid", menuentry->name) == 0)
	{
		if(check == 1) return 0;
		struct skin* screen = getscreen("standbymenu_grid");
		menu(screen, 0);
	}
	else if(ostrcmp("harddisk", menuentry->name) == 0)
	{
		if(check == 1) return 0;
		resettvpic();
		struct skin* screen = getscreen("harddiskmenu");
		menu(screen, 0);
	}
	else if(ostrcmp("harddisk_grid", menuentry->name) == 0)
	{
		if(check == 1) return 0;
		struct skin* screen = getscreen("harddiskmenu_grid");
		menu(screen, 0);
	}
	else if(ostrcmp("network", menuentry->name) == 0)
	{
		if(check == 1) return 0;
		resettvpic();
		struct skin* screen = getscreen("networkmenu");
		menu(screen, 0);
	}
	else if(ostrcmp("network_grid", menuentry->name) == 0)
	{
		if(check == 1) return 0;
		struct skin* screen = getscreen("networkmenu_grid");
		menu(screen, 0);
	}	
	else if(ostrcmp("httpdsettings", menuentry->name) == 0)
	{
		if(check == 1) return 0;
		screenhttpdsettings();
	}
	else if(ostrcmp("log", menuentry->name) == 0)
	{
		if(check == 1) return 0;
		screenlog();
	}
	else if(ostrcmp("mediaplayer", menuentry->name) == 0)
	{
		if(check == 1) return 0;
		screenplay(NULL, NULL, 0, 0);
	}
	else if(ostrcmp("restoredefault", menuentry->name) == 0)
	{
		if(check == 1) return 0;
		screenrestoredefaultchoice();
	}
	else if(ostrcmp("shortepg", menuentry->name) == 0)
	{
		if(check == 1) return 0;
		screenshortepg(NULL, NULL, 1);
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
	else if(ostrcmp("adaptersettings", menuentry->name) == 0)
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
		screennetwork_restart(NULL, 0);
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
	else if(ostrcmp("overclocking", menuentry->name) == 0)
	{
		if(check == 1) return 0;
		screensettings_overclocking();
	}
	else if(ostrcmp("autostart_default", menuentry->name) == 0)
	{
		if(check == 1) return 0;
		screensettings_autostart_default();
	}
	else if(ostrcmp("autostart_audiovideo", menuentry->name) == 0)
	{
		if(check == 1) return 0;
		screensettings_autostart_audiovideo();
	}
	else if(ostrcmp("autostart_usb", menuentry->name) == 0)
	{
		if(check == 1) return 0;
		screensettings_autostart_usb();
	}
	else if(ostrcmp("autostart_emu", menuentry->name) == 0)
	{
		if(check == 1) return 0;
		screensettings_autostart_emu();
	}
	else if(ostrcmp("autostart_network", menuentry->name) == 0)
	{
		if(check == 1) return 0;
		screensettings_autostart_network();
	}
	else if(ostrcmp("autostart_safety", menuentry->name) == 0)
	{
		if(check == 1) return 0;
		screensettings_autostart_safety();
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
	else if(ostrcmp("rescue", menuentry->name) == 0)
	{
		if(check == 1)
		{
//			if(checkbox("DM900") == 1 || checkbox("DM920") == 1 || checkbox("DM520") == 1 || checkbox("DM525") == 1)
			if(file_exist("/proc/stb/fp/boot_mode") || file_exist("/boot/STARTUP_RECOVERY"))
				return 0;
			else
				return 1;
		}

		if(file_exist("/proc/stb/fp/boot_mode"))
			writesys("/proc/stb/fp/boot_mode", "rescue", 1);
		
		if(file_exist("/boot/STARTUP_RECOVERY"))
			system("cp /boot/STARTUP_RECOVERY /boot/STARTUP");

		oshutdown(2, 1);
	}
	else if(ostrcmp("multiboot", menuentry->name) == 0)
	{
		if(check == 1)
		{
//			if(checkrealbox("HD51") == 1 || checkrealbox("HD60") == 1 || checkrealbox("HD61") == 1 || checkbox("SF4008") == 1 || checkchipset("HI3798MV200") == 1)
			if(file_exist("/boot/STARTUP"))
				return 0;
			else
				return 1;
		}
		multiboot();
// install via systemupdate
//		screenmultiboot();
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
	else if(ostrcmp("timeshiftsettings", menuentry->name) == 0)
	{
		if(check == 1) return 0;
		screentimeshiftsettings();
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
	else if(ostrcmp("fancontrol", menuentry->name) == 0)
	{
		if(check == 1) return 0;
#ifndef MIPSEL
		screensettings_fancontrol();
#else
		screensettings_fancontrol2();
#endif
	}
#ifdef MIPSEL 
	else if(ostrcmp("ceccontrol", menuentry->name) == 0) 
	{ 
		if(check == 1) return 0; 
			screencec(); 
	} 
#endif
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
	else if(ostrcmp("manualsearch_dvbc", menuentry->name) == 0)
	{
		if(check == 1) return 0;
		screenscanconfig(2);
	}
	else if(ostrcmp("manualsearch_dvbt", menuentry->name) == 0)
	{
		if(check == 1) return 0;
		screenscanconfig(3);
	}
	else if(ostrcmp("createsettings", menuentry->name) == 0)
	{
		if(check == 1) return 0;
		convertsettings();
	}
	else if(ostrcmp("createfav", menuentry->name) == 0)
	{
		if(check == 1) return 0;
		setdefaultbouquets();
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
	else if(ostrcmp("scconfig", menuentry->name) == 0)
	{
		if(check == 1) return 0;
		screenscconfig();
	}
	else if(ostrcmp("epgsearch", menuentry->name) == 0)
	{
		if(check == 1) return 0;
		screenepgsearch(NULL);
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
	else if(ostrcmp("passwordsettings", menuentry->name) == 0)
	{
		if(check == 1) return 0;
		screennetwork_password();
	}
	else if(ostrcmp("blindscanadjust", menuentry->name) == 0)
	{
		if(check == 1) return 0;
		screenblindscanadjust();
	}
	else if(ostrcmp("settings_redbutton", menuentry->name) == 0)
	{
		if(check == 1) return 0;
		screensettings_redbutton();
	}
	else if(ostrcmp("settings_bluebutton", menuentry->name) == 0)
	{
		if(check == 1) return 0;
		screensettings_bluebutton();
	}
	else if(ostrcmp("settings_pluginbutton", menuentry->name) == 0)
	{
		if(check == 1) return 0;
		screensettings_pluginbutton();
	}
	else if(ostrcmp("settings_mediabutton", menuentry->name) == 0)
	{
		if(check == 1) return 0;
		screensettings_mediabutton();
	}
	else if(ostrcmp("settings_pip", menuentry->name) == 0)
	{
		if(check == 1) return 0;
		pipsetup();
	}
	else if(ostrcmp("feed", menuentry->name) == 0)
	{
		if(check == 1) return 0;
		screenfeed(0);
	}
#ifdef DDTBUILD
	else if(ostrcmp("extensions_tpkinstall", menuentry->name) == 0)
	{
		if(check == 1) return 0;
		screenextensions(0, NULL, NULL, 1);
	}
	else if(ostrcmp("extensions_tpkremove", menuentry->name) == 0)
	{
		if(check == 1) return 0;
		screenextensions(1, NULL, NULL, 1);
	}
	else if(ostrcmp("extensions_tmptpkinstall", menuentry->name) == 0)
	{
		if(check == 1) return 0;
		screenextensions(2, NULL, NULL, 1);
	}
	else if(ostrcmp("extensions_tpkupgrade", menuentry->name) == 0)
	{
		if(check == 1) return 0;
		screenextensions(3, NULL, NULL, 1);
	}
	else if(ostrcmp("extensions_mediatpkinstall", menuentry->name) == 0)
	{
		if(check == 1) return 0;
		screenextensions_check(1);
	}
#else
	else if(ostrcmp("feed_ipk", menuentry->name) == 0)
	{
		if(check == 1) return 0;
		screenfeed_ipk(0);
	}
	else if(ostrcmp("extensions_ipkinstall", menuentry->name) == 0)
	{
		if(check == 1) return 0;
		screenextensions_ipk(0, NULL, NULL, 1);
	}
	else if(ostrcmp("extensions_ipkremove", menuentry->name) == 0)
	{
		if(check == 1) return 0;
		screenextensions_ipk(1, NULL, NULL, 1);
	}
	else if(ostrcmp("extensions_tmpipkinstall", menuentry->name) == 0)
	{
		if(check == 1) return 0;
		screenextensions_ipk(2, NULL, NULL, 1);
	}
	else if(ostrcmp("extensions_ipkupgrade", menuentry->name) == 0)
	{
		if(check == 1) return 0;
		screenextensions_ipk(3, NULL, NULL, 1);
	}
	else if(ostrcmp("extensions_mediaipkinstall", menuentry->name) == 0)
	{
		if(check == 1) return 0;
		screenextensions_check_ipk(1);
	}
#endif
	else if(ostrcmp("system_update_flash_online", menuentry->name) == 0)
	{
		if(check == 1) return 0;
		addconfigtmp("dirsort", "4");
		screensystem_update(0);
		delconfigtmp("dirsort");
	}
	else if(ostrcmp("system_update_flash_tmp", menuentry->name) == 0)
	{
		if(check == 1) return 0;
		screensystem_update(1);
	}
	else if(ostrcmp("system_update_usb_online", menuentry->name) == 0)
	{
		if(check == 1) return 0;
		addconfigtmp("dirsort", "4");
		screensystem_update(2);
		delconfigtmp("dirsort");
	}
	else if(ostrcmp("system_update_usb_tmp", menuentry->name) == 0)
	{
		if(check == 1) return 0;
		screensystem_update(3);
	}
	else if(ostrcmp("system_infos_default", menuentry->name) == 0)
	{
		if(check == 1) return 0;
		screensystem_infos(0);
	}
	else if(ostrcmp("system_infos_freespace", menuentry->name) == 0)
	{
		if(check == 1) return 0;
		screensystem_infos(1);
	}
	else if(ostrcmp("system_infos_kernel", menuentry->name) == 0)
	{
		if(check == 1) return 0;
		screensystem_infos(2);
	}
	else if(ostrcmp("system_infos_mounts", menuentry->name) == 0)
	{
		if(check == 1) return 0;
		screensystem_infos(3);
	}
	else if(ostrcmp("system_infos_network", menuentry->name) == 0)
	{
		if(check == 1) return 0;
		screensystem_infos(4);
	}
	else if(ostrcmp("system_infos_ram", menuentry->name) == 0)
	{
		if(check == 1) return 0;
		screensystem_infos(5);
	}
	else if(ostrcmp("system_infos", menuentry->name) == 0)
	{
		if(check == 1) return 0;
		resettvpic();
		struct skin* screen = getscreen("system_infos_main");
		menu(screen, 0);
	}
	else if(ostrcmp("system_infos_sysinfo", menuentry->name) == 0)
	{
		if(check == 1) return 0;
		resettvpic();
		struct skin* screen = getscreen("system_sysinfos_main");
		menu(screen, 0);
	}
	else if(ostrcmp("system_sysinfos_cpu", menuentry->name) == 0)
	{
		if(check == 1) return 0;
		screensystem_infos_sysinfo(0);
	}
	else if(ostrcmp("system_sysinfos_mem", menuentry->name) == 0)
	{
		if(check == 1) return 0;
		screensystem_infos_sysinfo(1);
	}
	else if(ostrcmp("system_sysinfos_mtd", menuentry->name) == 0)
	{
		if(check == 1) return 0;
		screensystem_infos_sysinfo(2);
	}
	else if(ostrcmp("system_sysinfos_module", menuentry->name) == 0)
	{
		if(check == 1) return 0;
		screensystem_infos_sysinfo(3);
	}
	else if(ostrcmp("system_sysinfos_devices", menuentry->name) == 0)
	{
		if(check == 1) return 0;
		screensystem_infos_sysinfo(4);
	}
	else if(ostrcmp("system_sysinfos_swap", menuentry->name) == 0)
	{
		if(check == 1) return 0;
		screensystem_infos_sysinfo(5);
	}
	else if(ostrcmp("system_sysinfos_top", menuentry->name) == 0)
	{
		if(check == 1) return 0;
		screensystem_infos_sysinfo(6);
	}
	else if(ostrcmp("system_sysinfos_ps", menuentry->name) == 0)
	{
		if(check == 1) return 0;
		screensystem_infos_sysinfo(7);
	}
	else if(ostrcmp("system_sysinfos_usb", menuentry->name) == 0)
	{
		if(check == 1) return 0;
		screensystem_infos_sysinfo(8);
	}
	else if(ostrcmp("infos_serial", menuentry->name) == 0)
	{
		if(check == 1) return 0;
		screengetserial(0);
	}
	else if(ostrcmp("infos_imageinfo", menuentry->name) == 0)
	{
		if(check == 1) return 0;
		screensystem_logs(0);
	}
	else if(ostrcmp("infos_titaninfo", menuentry->name) == 0)
	{
		if(check == 1) return 0;
		screensystem_logs(1);
	}
	else if(ostrcmp("infos_gitinfo", menuentry->name) == 0)
	{
		if(check == 1) return 0;
		screensystem_logs(2);
	}
	else if(ostrcmp("system_backup", menuentry->name) == 0)
	{
		if(check == 1) return 0;
		screensystem_backup();
	}
	else if(ostrcmp("system_backup_restore", menuentry->name) == 0)
	{
		if(check == 1) return 0;
		screensystem_backup_restore();
	}
	else if(ostrcmp("softcam", menuentry->name) == 0)
	{
		if(check == 1) return 0;
		screensoftcam();
	}
	else if(ostrcmp("mediadbsettings", menuentry->name) == 0)
	{
		if(check == 1) return 0;
		screenmediadbsettings();
	}
	else if(ostrcmp("extensionslist", menuentry->name) == 0)
	{
		if(check == 1) return 0;
		screenkeyactions(1, 0);
	}
	else if(ostrcmp("unlock", menuentry->name) == 0)
	{
		if(check == 1) return 0;
		screenunlock();
	}
	else if(ostrcmp("newsletter", menuentry->name) == 0)
	{
		if(check == 1) return 0;
		screennewsletter();
	}
	else if(menuentry->pluginhandle != NULL)
	{
		if(menuentry->input != NULL)
		{
			startplugin = dlsym(menuentry->pluginhandle, menuentry->input);
			if(startplugin != NULL)
			{
				if(check == 1) return 0;
				resettvpic();
				startplugin();
				resettvpic();
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

	resettvpic();
	return ret;
}

#endif
