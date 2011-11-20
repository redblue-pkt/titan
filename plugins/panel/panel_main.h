#ifndef AAFPANEL_MAIN_H
#define AAFPANEL_MAIN_H

struct skin* aafpanel_menu(struct skin* aafpanel_menu)
{
	debug(1000, "in");
	int rcret = 0, ret = 0;
	struct skin* listbox = getscreennode(aafpanel_menu, "listbox");
//	struct skin* aafpanel_softcam_menu = getscreen("aafpanel_softcam");

	if(listbox == NULL || aafpanel_menu == NULL)
	{
		debug(1000, "out -> NULL detect");
		return NULL;
	}

//	if(checkemu() != 1)
//		aafpanel_softcam_menu->hidden == YES;
//	else
//		aafpanel_softcam_menu->hidden == NO;

	drawscreen(aafpanel_menu, 0);
	addscreenrc(aafpanel_menu, listbox);

	while(1)
	{
		rcret = waitrc(aafpanel_menu, 0, 0);

		if (rcret == getrcconfigint("rcexit", NULL)) break;
		if(rcret == getrcconfigint("rcok", NULL))
		{
			clearscreen(aafpanel_menu);
			ret = aafpanel_menucall(listbox->select);
			if(ret != 0)
				textbox(_("Message"), _("Menu not implemented !"), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, NULL, 0, 600, 200, 0, 0);
			drawscreen(aafpanel_menu, 0);
		}
	}

	delownerrc(aafpanel_menu);
	clearscreen(aafpanel_menu);
	debug(1000, "out");
	return listbox->select;
}

int aafpanel_menucall(struct skin* aafpanel_menuentry)
{
	debug(1000, "in");
//	void (*startplugin) (void);
	int ret = 0;

	if(aafpanel_menuentry == NULL)
	{
		debug(1000, "out -> NULL detect");
		return 1;
	}

	printf("MENU: %s\n", aafpanel_menuentry->name);	// must be removed later

// softcam
	if(ostrcmp("aafpanel_softcam", aafpanel_menuentry->name) == 0){
		screenaafpanel_softcam();
// settings
	} else if(ostrcmp("aafpanel_settings", aafpanel_menuentry->name) == 0){
		struct skin* screen = getscreen("aafpanel_settings_main");

		if(checkbeta() != 1)
			delscreennode(screen, "aafpanel_settings_betacfg");

		if(checkbox("UFS922") != 1)
			delscreennode(screen, "aafpanel_settings_fancontrol");

		if(isfile("/proc/cpu_frequ/pll0_ndiv_mdiv") != 1)
			delscreennode(screen, "aafpanel_settings_overclocking");

		aafpanel_menu(screen);

	} else if(ostrcmp("aafpanel_settings_overclocking", aafpanel_menuentry->name) == 0){
		screenaafpanel_settings_overclocking();
	} else if(ostrcmp("aafpanel_settings_betacfg", aafpanel_menuentry->name) == 0){
		screenaafpanel_settings(0);
	} else if(ostrcmp("aafpanel_settings_automount", aafpanel_menuentry->name) == 0){
		screenaafpanel_settings(1);
	} else if(ostrcmp("aafpanel_settings_redbutton", aafpanel_menuentry->name) == 0){
		screenaafpanel_settings_redbutton();
	} else if(ostrcmp("aafpanel_settings_bluebutton", aafpanel_menuentry->name) == 0){
		screenaafpanel_settings_bluebutton();
	} else if(ostrcmp("aafpanel_settings_autostart", aafpanel_menuentry->name) == 0){
		struct skin* screen = getscreen("aafpanel_autostart_main");
		aafpanel_menu(screen);
	} else if(ostrcmp("aafpanel_settings_fancontrol", aafpanel_menuentry->name) == 0){
		screenaafpanel_settings_fancontrol();
// system
	} else if(ostrcmp("aafpanel_system", aafpanel_menuentry->name) == 0){
		struct skin* screen = getscreen("aafpanel_system_main");
		aafpanel_menu(screen);
	} else if(ostrcmp("aafpanel_system_restore", aafpanel_menuentry->name) == 0){
		screenaafpanel_system_restore();
	} else if(ostrcmp("aafpanel_system_backup", aafpanel_menuentry->name) == 0){
		screenaafpanel_system_backup();
	} else if(ostrcmp("aafpanel_system_imageinfo", aafpanel_menuentry->name) == 0){
		screenaafpanel_infos(0);
	} else if(ostrcmp("aafpanel_system_update_flash_online", aafpanel_menuentry->name) == 0){
		screenaafpanel_system_update(0);
	} else if(ostrcmp("aafpanel_system_update_flash_tmp", aafpanel_menuentry->name) == 0){
		screenaafpanel_system_update(1);
	} else if(ostrcmp("aafpanel_system_update_usb_online", aafpanel_menuentry->name) == 0){
		screenaafpanel_system_update(2);
	} else if(ostrcmp("aafpanel_system_update_usb_tmp", aafpanel_menuentry->name) == 0){
		screenaafpanel_system_update(3);
// extensions
	} else if(ostrcmp("aafpanel_extensions", aafpanel_menuentry->name) == 0){
		struct skin* screen = getscreen("aafpanel_extensions_main");
		aafpanel_menu(screen);
	} else if(ostrcmp("aafpanel_extensions_menu", aafpanel_menuentry->name) == 0){
		screenplugin();
	} else if(ostrcmp("aafpanel_extensions_ipkinstall", aafpanel_menuentry->name) == 0){
		screenaafpanel_extensions(0);
	} else if(ostrcmp("aafpanel_extensions_ipkremove", aafpanel_menuentry->name) == 0){
		screenaafpanel_extensions(1);
	} else if(ostrcmp("aafpanel_extensions_tmpipkinstall", aafpanel_menuentry->name) == 0){
		screenaafpanel_extensions(2);
// infos main
	} else if(ostrcmp("aafpanel_infos", aafpanel_menuentry->name) == 0){
		struct skin* screen = getscreen("aafpanel_infos_main");
		aafpanel_menu(screen);
	} else if(ostrcmp("aafpanel_infos_aaf", aafpanel_menuentry->name) == 0){
		screenaafpanel_infos(0);
	} else if(ostrcmp("aafpanel_infos_default", aafpanel_menuentry->name) == 0){
		screenaafpanel_infos(1);
	} else if(ostrcmp("aafpanel_infos_free", aafpanel_menuentry->name) == 0){
		screenaafpanel_infos(2);
	} else if(ostrcmp("aafpanel_infos_kernel", aafpanel_menuentry->name) == 0){
		screenaafpanel_infos(3);
	} else if(ostrcmp("aafpanel_infos_mounts", aafpanel_menuentry->name) == 0){
		screenaafpanel_infos(4);
	} else if(ostrcmp("aafpanel_infos_network", aafpanel_menuentry->name) == 0){
		screenaafpanel_infos(5);
	} else if(ostrcmp("aafpanel_infos_ram", aafpanel_menuentry->name) == 0){
		screenaafpanel_infos(6);
// infos sysinfos
	} else if(ostrcmp("aafpanel_infos_sysinfo", aafpanel_menuentry->name) == 0){
		struct skin* screen = getscreen("aafpanel_sysinfos_main");
		aafpanel_menu(screen);
	} else if(ostrcmp("aafpanel_sysinfos_cpu", aafpanel_menuentry->name) == 0){
		screenaafpanel_infos_sysinfo(0);
	} else if(ostrcmp("aafpanel_sysinfos_mem", aafpanel_menuentry->name) == 0){
		screenaafpanel_infos_sysinfo(1);
	} else if(ostrcmp("aafpanel_sysinfos_mtd", aafpanel_menuentry->name) == 0){
		screenaafpanel_infos_sysinfo(2);
	} else if(ostrcmp("aafpanel_sysinfos_module", aafpanel_menuentry->name) == 0){
		screenaafpanel_infos_sysinfo(3);
	} else if(ostrcmp("aafpanel_sysinfos_devices", aafpanel_menuentry->name) == 0){
		screenaafpanel_infos_sysinfo(4);
	} else if(ostrcmp("aafpanel_sysinfos_swap", aafpanel_menuentry->name) == 0){
		screenaafpanel_infos_sysinfo(5);
	} else if(ostrcmp("aafpanel_sysinfos_top", aafpanel_menuentry->name) == 0){
		screenaafpanel_infos_sysinfo(6);
// autostart
	} else if(ostrcmp("aafpanel_autostart_default", aafpanel_menuentry->name) == 0){
		screenaafpanel_settings_autostart(0);
	} else if(ostrcmp("aafpanel_autostart_audiovideo", aafpanel_menuentry->name) == 0){
		screenaafpanel_settings_autostart(1);
	} else if(ostrcmp("aafpanel_autostart_usb", aafpanel_menuentry->name) == 0){
		screenaafpanel_settings_autostart(2);
	} else if(ostrcmp("aafpanel_autostart_emu", aafpanel_menuentry->name) == 0){
		screenaafpanel_settings_autostart(3);
	} else if(ostrcmp("aafpanel_autostart_network", aafpanel_menuentry->name) == 0){
		screenaafpanel_settings_autostart(4);
	} else if(ostrcmp("aafpanel_autostart_safety", aafpanel_menuentry->name) == 0){
		screenaafpanel_settings_autostart(5);
// videotune
	} else if(ostrcmp("aafpanel_settings_videotune", aafpanel_menuentry->name) == 0){
		screenvideosettings();
	} else {
		ret = 1;
	}

	debug(1000, "out");
	return ret;
}

void aafpanel_main()
{
	debug(1000, "in");
	status.hangtime = 99999;
	int rcret = 0, ret = 0;

	struct skin* aafpanel_main = getscreen("aafpanel_main");
	struct skin* listbox = getscreennode(aafpanel_main, "listbox");

	struct skin* aafpanel_main_selected = getscreennode(aafpanel_main, "aafpanel_main_selected_pic");
	struct skin* child = NULL;

	drawscreen(aafpanel_main, 1);
	addscreenrc(aafpanel_main, listbox);

	listbox->aktpage = -1;
	listbox->aktline = 1;
	// Hide SoftCam Panel when no Emu's installed
	child = listbox->next;
	while(child != NULL)
	{
		if(ostrcmp(child->name, "aafpanel_softcam") == 0){
			if(checkemu() == 0){
				child->hidden = YES;
			} else{
				child->hidden = NO;
			}
			listbox->aktline = 1;
			listbox->aktpage = -1;
			break;
			}
		child = child->next;
	}

	drawscreen(aafpanel_main, 0);
	if(listbox->select != NULL)
	{
		status.updatevfd = PAUSE;
		writevfd(listbox->select->text);
	}

	while(1)
	{
		status.screencalc = 1;
		rcret = waitrc(aafpanel_main, 0, 0);
		status.screencalc = 0;

		if (rcret == getrcconfigint("rcexit", NULL)) break;

		if(rcret == getrcconfigint("rcok", NULL))
		{
			clearscreen(aafpanel_main);
			ret = aafpanel_menucall(listbox->select);
			if(ret != 0)
				textbox(_("Message"), _("Menu not implemented !"), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, NULL, 0, 600, 200, 0, 0);
			drawscreen(aafpanel_main, 0);
		}
			debug(10, "[aafpanal_main] (select) menu=%s", listbox->select->name);
		if(listbox->select != NULL && (rcret == getrcconfigint("rcup", NULL) || rcret == getrcconfigint("rcdown", NULL)))
		{

			debug(10, "[aafpanal_main] (select) menu=%s", listbox->select->name);
			if(ostrcmp(listbox->select->name, "aafpanel_softcam") == 0){
				changepic(aafpanel_main_selected, "panel/skin/aafpanel_softcam.png");
			} else if(ostrcmp(listbox->select->name, "aafpanel_settings") == 0){
				changepic(aafpanel_main_selected, "panel/skin/aafpanel_settings.png");
			} else if(ostrcmp(listbox->select->name, "aafpanel_system") == 0){
				changepic(aafpanel_main_selected, "panel/skin/aafpanel_system.png");
			} else if(ostrcmp(listbox->select->name, "aafpanel_extensions") == 0){
				changepic(aafpanel_main_selected, "panel/skin/aafpanel_extensions.png");
			} else if(ostrcmp(listbox->select->name, "aafpanel_infos") == 0){
				changepic(aafpanel_main_selected, "panel/skin/aafpanel_infos.png");
			}
		}
		drawscreen(aafpanel_main, 0);
		if(listbox->select != NULL)
			writevfd(listbox->select->text);
	}

	delownerrc(aafpanel_main);
	clearscreen(aafpanel_main);
	//delscreen(aafpanel_main);
	status.hangtime = getconfigint("hangtime", NULL);
	debug(1000, "out");
}


#endif
