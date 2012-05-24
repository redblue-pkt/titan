#ifndef PANEL_MAIN_H
#define PANEL_MAIN_H

struct skin* panel_menu(struct skin* panel_menu)
{
	debug(1000, "in");
	int rcret = 0, ret = 0;
	struct skin* listbox = getscreennode(panel_menu, "listbox");

	if(listbox == NULL || panel_menu == NULL)
	{
		debug(1000, "out -> NULL detect");
		return NULL;
	}

	drawscreen(panel_menu, 0);
	addscreenrc(panel_menu, listbox);

	while(1)
	{
		rcret = waitrc(panel_menu, 0, 0);

		if (rcret == getrcconfigint("rcexit", NULL)) break;
		if(rcret == getrcconfigint("rcok", NULL))
		{
			clearscreen(panel_menu);
			ret = panel_menucall(listbox->select);
			if(ret != 0)
				textbox(_("Message"), _("Menu not implemented !"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 0, 0);
			drawscreen(panel_menu, 0);
		}
	}

	delownerrc(panel_menu);
	clearscreen(panel_menu);
	debug(1000, "out");
	return listbox->select;
}

int panel_menucall(struct skin* panel_menuentry)
{
	debug(1000, "in");
//	void (*startplugin) (void);
	int ret = 0;

	if(panel_menuentry == NULL)
	{
		debug(1000, "out -> NULL detect");
		return 1;
	}
	
	if(!(panel_menuentry->type & MENU)) return 1;

	printf("MENU: %s\n", panel_menuentry->name);	// must be removed later

	if(ostrcmp("panel_softcam", panel_menuentry->name) == 0)
		screenpanel_softcam();
	else if(ostrcmp("panel_settings", panel_menuentry->name) == 0)
	{
		struct skin* screen = getscreen("panel_settings_main");

		if(checkbeta() != 1)
			delscreennode(screen, "panel_settings_betacfg");

		if(checkbox("UFS922") != 1)
			delscreennode(screen, "panel_settings_fancontrol");

		if(isfile("/proc/cpu_frequ/pll0_ndiv_mdiv") != 1)
			delscreennode(screen, "panel_settings_overclocking");

		panel_menu(screen);
	}
	else if(ostrcmp("panel_settings_overclocking", panel_menuentry->name) == 0)
		screenpanel_settings_overclocking();
	else if(ostrcmp("panel_settings_automount", panel_menuentry->name) == 0)
		screenpanel_settings(0);
	else if(ostrcmp("panel_settings_autostart", panel_menuentry->name) == 0)
	{
		struct skin* screen = getscreen("panel_autostart_main");
		panel_menu(screen);
	}
	else if(ostrcmp("panel_settings_fancontrol", panel_menuentry->name) == 0)
		screenpanel_settings_fancontrol();
	else if(ostrcmp("panel_system", panel_menuentry->name) == 0)
	{
		struct skin* screen = getscreen("panel_system_main");
		panel_menu(screen);
	}
	else if(ostrcmp("panel_system_eraseswap", panel_menuentry->name) == 0)
		screenpanel_system_eraseswap();		
	else if(ostrcmp("panel_system_wizard", panel_menuentry->name) == 0)
		screenpanel_system_wizard();
	else if(ostrcmp("panel_system_restore", panel_menuentry->name) == 0)
		screenpanel_system_restore();
	else if(ostrcmp("panel_system_backup", panel_menuentry->name) == 0)
		screenpanel_system_backup();
	else if(ostrcmp("panel_system_imageinfo", panel_menuentry->name) == 0)
		screenpanel_infos(0);
	else if(ostrcmp("panel_system_getserial", panel_menuentry->name) == 0)
		screenpanel_system_getserial();
	else if(ostrcmp("panel_extensions", panel_menuentry->name) == 0)
	{
		struct skin* screen = getscreen("panel_extensions_main");
		panel_menu(screen);
	}
	else if(ostrcmp("panel_extensions_menu", panel_menuentry->name) == 0)
		screenplugin();
	else if(ostrcmp("panel_infos_main", panel_menuentry->name) == 0)
	{
		struct skin* screen = getscreen("panel_infos_main");
		panel_menu(screen);
	}
	else if(ostrcmp("panel_infos_default", panel_menuentry->name) == 0)
		screenpanel_infos(0);
	else if(ostrcmp("panel_infos_free", panel_menuentry->name) == 0)
		screenpanel_infos(1);
	else if(ostrcmp("panel_infos_kernel", panel_menuentry->name) == 0)
		screenpanel_infos(2);
	else if(ostrcmp("panel_infos_mounts", panel_menuentry->name) == 0)
		screenpanel_infos(3);
	else if(ostrcmp("panel_infos_network", panel_menuentry->name) == 0)
		screenpanel_infos(4);
	else if(ostrcmp("panel_infos_ram", panel_menuentry->name) == 0)
		screenpanel_infos(5);
	else if(ostrcmp("panel_infos_sysinfo", panel_menuentry->name) == 0)
	{
		struct skin* screen = getscreen("panel_sysinfos_main");
		panel_menu(screen);
	}
	else if(ostrcmp("panel_sysinfos_cpu", panel_menuentry->name) == 0)
		screenpanel_infos_sysinfo(0);
	else if(ostrcmp("panel_sysinfos_mem", panel_menuentry->name) == 0)
		screenpanel_infos_sysinfo(1);
	else if(ostrcmp("panel_sysinfos_mtd", panel_menuentry->name) == 0)
		screenpanel_infos_sysinfo(2);
	else if(ostrcmp("panel_sysinfos_module", panel_menuentry->name) == 0)
		screenpanel_infos_sysinfo(3);
	else if(ostrcmp("panel_sysinfos_devices", panel_menuentry->name) == 0)
		screenpanel_infos_sysinfo(4);
	else if(ostrcmp("panel_sysinfos_swap", panel_menuentry->name) == 0)
		screenpanel_infos_sysinfo(5);
	else if(ostrcmp("panel_sysinfos_top", panel_menuentry->name) == 0)
		screenpanel_infos_sysinfo(6);
	else if(ostrcmp("panel_autostart_default", panel_menuentry->name) == 0)
		screenpanel_settings_autostart(0);
	else if(ostrcmp("panel_autostart_audiovideo", panel_menuentry->name) == 0)
		screenpanel_settings_autostart(1);
	else if(ostrcmp("panel_autostart_usb", panel_menuentry->name) == 0)
		screenpanel_settings_autostart(2);
	else if(ostrcmp("panel_autostart_emu", panel_menuentry->name) == 0)
		screenpanel_settings_autostart(3);
	else if(ostrcmp("panel_autostart_network", panel_menuentry->name) == 0)
		screenpanel_settings_autostart(4);
	else if(ostrcmp("panel_autostart_safety", panel_menuentry->name) == 0)
		screenpanel_settings_autostart(5);
	else if(ostrcmp("panel_settings_videotune", panel_menuentry->name) == 0)
		screenvideosettings();
	else
		ret = 1;

	debug(1000, "out");
	return ret;
}

void panel_main()
{
	debug(1000, "in");
	status.hangtime = 99999;
	int rcret = 0, ret = 0;

	struct skin* panel_main = getscreen("panel_main");
	struct skin* listbox = getscreennode(panel_main, "listbox");

	struct skin* panel_main_selected = getscreennode(panel_main, "panel_main_selected_pic");
	struct skin* child = NULL;

	drawscreen(panel_main, 1);
	addscreenrc(panel_main, listbox);

	listbox->aktpage = -1;
	listbox->aktline = 1;
	// Hide SoftCam Panel when no Emu's installed
	child = listbox->next;
	while(child != NULL)
	{
		if(ostrcmp(child->name, "panel_softcam") == 0)
		{
			if((checkemu() == 0) || (status.security == 0))
				child->hidden = YES;
			else
				child->hidden = NO;
			listbox->aktline = 1;
			listbox->aktpage = -1;
			break;
		}
		child = child->next;
	}

	drawscreen(panel_main, 0);
	if(listbox->select != NULL)
	{
		status.updatevfd = PAUSE;
		writevfd(listbox->select->text);
	}

	while(1)
	{
		status.screencalc = 1;
		rcret = waitrc(panel_main, 0, 0);
		status.screencalc = 0;

		if (rcret == getrcconfigint("rcexit", NULL)) break;

		if(rcret == getrcconfigint("rcok", NULL))
		{
			clearscreen(panel_main);
			ret = panel_menucall(listbox->select);
			if(ret != 0)
				textbox(_("Message"), _("Menu not implemented !"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 0, 0);
			drawscreen(panel_main, 0);
		}
		debug(10, "[panal_main] (select) menu=%s", listbox->select->name);
		if(listbox->select != NULL && (rcret == getrcconfigint("rcup", NULL) || rcret == getrcconfigint("rcdown", NULL)))
		{
			debug(10, "[panal_main] (select) menu=%s", listbox->select->name);
			if(ostrcmp(listbox->select->name, "panel_softcam") == 0)
				changepic(panel_main_selected, "panel/skin/panel_softcam.png");
			else if(ostrcmp(listbox->select->name, "panel_settings") == 0)
				changepic(panel_main_selected, "panel/skin/panel_settings.png");
			else if(ostrcmp(listbox->select->name, "panel_system") == 0)
				changepic(panel_main_selected, "panel/skin/panel_system.png");
			else if(ostrcmp(listbox->select->name, "panel_extensions") == 0)
				changepic(panel_main_selected, "panel/skin/panel_extensions.png");
			else if(ostrcmp(listbox->select->name, "panel_infos") == 0)
				changepic(panel_main_selected, "panel/skin/panel_infos.png");
		}
		drawscreen(panel_main, 0);
		if(listbox->select != NULL)
			writevfd(listbox->select->text);
	}

	delownerrc(panel_main);
	clearscreen(panel_main);
	//delscreen(panel_main);
	status.hangtime = getconfigint("hangtime", NULL);
	debug(1000, "out");
}


#endif
