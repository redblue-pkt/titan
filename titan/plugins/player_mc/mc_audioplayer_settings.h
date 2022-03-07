#ifndef MC_AUDIOPLAYER_SETTINGS_H
#define MC_AUDIOPLAYER_SETTINGS_H

void screenmc_audioplayer_settings()
{
	int rcret = 0;
	char* ret = NULL;
	struct skin* mc_audioplayer_settings = getscreen("mc_audioplayer_settings");
	struct skin* listbox = getscreennode(mc_audioplayer_settings, "listbox");
	struct skin* dirsort = getscreennode(mc_audioplayer_settings, "sort");
	struct skin* view = getscreennode(mc_audioplayer_settings, "view");
	struct skin* ap_autostart_playlist = getscreennode(mc_audioplayer_settings, "autostart_playlist");
	struct skin* defaultdir = getscreennode(mc_audioplayer_settings, "defaultdir");
	struct skin* uselastdir = getscreennode(mc_audioplayer_settings, "uselastdir");
	struct skin* autoscan = getscreennode(mc_audioplayer_settings, "autoscan");
	struct skin* loadmediadb = getscreen("loading");
	struct skin* blackscreen = getscreen("blackscreen");
	
//	struct skin* dimmer = getscreennode(mc_audioplayer_settings, "dimmer");
//	struct skin* dimmer_delay = getscreennode(mc_audioplayer_settings, "dimmer_delay");
//	struct skin* autosleep = getscreennode(mc_audioplayer_settings, "autosleep");
//	struct skin* autosleep_delay = getscreennode(mc_audioplayer_settings, "autosleep_delay");
//	struct skin* repeat = getscreennode(mc_audioplayer_settings, "repeat");

	struct skin* tmp = NULL;

	addchoicebox(dirsort, "0", _("alpha"));	
	addchoicebox(dirsort, "1", _("reverse alpha"));	
	addchoicebox(dirsort, "2", _("size"));
	addchoicebox(dirsort, "3", _("reverse size"));
	addchoicebox(dirsort, "4", _("date"));
	addchoicebox(dirsort, "5", _("reverse date"));
	setchoiceboxselection(dirsort, getconfig("mc_ap_dirsort", NULL));

	addchoicebox(view, "0", _("fast"));
	addchoicebox(view, "1", _("big"));	
	addchoicebox(view, "2", _("cover"));	
	addchoicebox(view, "3", _("fullcover"));
	addchoicebox(view, "4", _("size (default)"));
	addchoicebox(view, "5", _("datetime"));

	setchoiceboxselection(view, getconfig("mc_ap_view", NULL));


	addchoicebox(ap_autostart_playlist, "0", _("no"));	
	addchoicebox(ap_autostart_playlist, "1", _("yes"));
	setchoiceboxselection(ap_autostart_playlist, getconfig("mc_ap_autostart_playlist", NULL));

	changeinput(defaultdir, getconfig("mc_ap_defaultdir", NULL));

	addchoicebox(uselastdir, "0", _("no"));
	addchoicebox(uselastdir, "1", _("yes"));
	setchoiceboxselection(uselastdir, getconfig("mc_ap_uselastdir", NULL));

	addchoicebox(autoscan, "0", _("no"));	
	addchoicebox(autoscan, "1", _("yes"));
	setchoiceboxselection(autoscan, getconfig("mc_ap_autoscan", NULL));

//	addchoicebox(dimmer, "0", _("no"));	
//	addchoicebox(dimmer, "1", _("yes"));
//	setchoiceboxselection(dimmer, getconfig("mc_ap_dimmer", NULL));

//	changeinput(dimmer_delay, "10\n20\n30\n40\n50\n60\n70\n80\n90\n100");
//	setchoiceboxselection(dimmer_delay, getconfig("mc_ap_dimmer_delay", NULL));

//	addchoicebox(autosleep, "0", _("no"));	
//	addchoicebox(autosleep, "1", _("yes"));
//	setchoiceboxselection(autosleep, getconfig("autosleep", NULL));

//	changeinput(autosleep_delay, "300\n600\n900\n1200\n1500\n1800\n2100\n2400");
//	setchoiceboxselection(autosleep_delay, getconfig("mc_ap_autosleep_delay", NULL));

//	addchoicebox(repeat, "0", _("no"));	
//	addchoicebox(repeat, "1", _("yes"));
//	setchoiceboxselection(repeat, getconfig("mc_ap_repeat", NULL));


	drawscreen(mc_audioplayer_settings, 0, 0);
	addscreenrc(mc_audioplayer_settings, listbox);

	tmp = listbox->select;
	while(1)
	{
		addscreenrc(mc_audioplayer_settings, tmp);
		rcret = waitrc(mc_audioplayer_settings, 0, 0);
		tmp = listbox->select;
	
		if(rcret == getrcconfigint("rcexit", NULL))
			break;
		else if(rcret == getrcconfigint("rcok", NULL))
		{
			addconfigscreencheck("mc_ap_dirsort", dirsort, NULL);
			addconfigscreencheck("mc_ap_view", view, NULL);
			addconfigscreencheck("mc_ap_autoscan", autoscan, NULL);	
			addconfigscreencheck("mc_ap_autostart_playlist", ap_autostart_playlist, NULL);
			addconfigscreen("mc_ap_defaultdir", defaultdir);
			addconfigscreencheck("mc_ap_uselastdir", uselastdir, NULL);
//			addconfigscreencheck("mc_ap_dimmer", dimmer, NULL);
//			addconfigscreencheck("mc_ap_dimmer_delay", dimmer_delay, NULL);
//			addconfigscreencheck("mc_ap_autosleep", autosleep, NULL);
//			addconfigscreencheck("mc_ap_autosleep_delay", autosleep_delay, NULL);
//			addconfigscreencheck("mc_ap_repeat", repeat, NULL);
			break;
		}
		else if(rcret == getrcconfigint("rcred", NULL))
		{
			struct skin* screen = getscreen("networkmenu");
			menu(screen);
			drawscreen(mc_audioplayer_settings, 0, 0);
		}
		else if(rcret == getrcconfigint("rcgreen", NULL))
		{
			if(listbox->select != NULL && ostrcmp(listbox->select->name, "defaultdir") == 0)
			{
				clearscreen(mc_audioplayer_settings);
				ret = screendir(listbox->select->ret, "", NULL, NULL, NULL, NULL, 0, "SELECT", 0, NULL, 0, NULL, 0, 750, 0, 650, 0, 0);
				if(ret != NULL)
					changeinput(listbox->select, ret);
				free(ret);

				drawscreen(mc_audioplayer_settings, 0, 0);
			}
		}
		else if(rcret == getrcconfigint("rcyellow", NULL))
		{
			screenscreensaveradjust();
			drawscreen(mc_audioplayer_settings, 0, 0);
		}
	}

	drawscreen(blackscreen, 0, 0);
	drawscreen(loadmediadb, 0, 0);
	writeallconfig(0);
	delownerrc(mc_audioplayer_settings);
	clearscreen(mc_audioplayer_settings);
	resettvpic();
}

#endif
