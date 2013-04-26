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

	drawscreen(panel_menu, 0, 0);
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
			drawscreen(panel_menu, 0, 0);
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

	if(ostrcmp("panel_settings", panel_menuentry->name) == 0)
	{
		struct skin* screen = getscreen("panel_settings_main");

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
//	else if(ostrcmp("panel_settings_wizard", panel_menuentry->name) == 0)
//		screenpanel_wizard();
	else if(ostrcmp("panel_settings_autostart", panel_menuentry->name) == 0)
	{
		struct skin* screen = getscreen("panel_autostart_main");
		panel_menu(screen);
	}
	else if(ostrcmp("panel_settings_fancontrol", panel_menuentry->name) == 0)
		screenpanel_settings_fancontrol();
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

	addscreenrc(panel_main, listbox);

	listbox->aktpage = -1;
	listbox->aktline = 1;

	drawscreen(panel_main, 0, 0);
	if(listbox->select != NULL)
	{
		status.updatevfd = PAUSE;
		writevfdmenu(listbox->select->text);
	}

	while(1)
	{
		rcret = waitrc(panel_main, 0, 1);

		if (rcret == getrcconfigint("rcexit", NULL)) break;

		if(rcret == getrcconfigint("rcok", NULL))
		{
			clearscreen(panel_main);
			ret = panel_menucall(listbox->select);
			if(ret != 0)
				textbox(_("Message"), _("Menu not implemented !"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 0, 0);
			drawscreen(panel_main, 0, 0);
		}
		debug(10, "[panal_main] (select) menu=%s", listbox->select->name);
		if(listbox->select != NULL && (rcret == getrcconfigint("rcup", NULL) || rcret == getrcconfigint("rcdown", NULL)))
		{
			debug(10, "[panal_main] (select) menu=%s", listbox->select->name);
			if(ostrcmp(listbox->select->name, "panel_settings") == 0)
				changepic(panel_main_selected, "panel/skin/panel_settings.png");
			else if(ostrcmp(listbox->select->name, "panel_infos") == 0)
				changepic(panel_main_selected, "panel/skin/panel_infos.png");
		}
		drawscreen(panel_main, 0, 0);
		if(listbox->select != NULL)
			writevfdmenu(listbox->select->text);
	}

	delownerrc(panel_main);
	clearscreen(panel_main);
	//delscreen(panel_main);
	status.hangtime = getconfigint("hangtime", NULL);
	debug(1000, "out");
}


#endif
