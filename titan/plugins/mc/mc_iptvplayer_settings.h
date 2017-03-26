#ifndef MC_iptvplayer_SETTINGS_H
#define MC_iptvplayer_SETTINGS_H

void screenmc_iptvplayer_settings()
{
	int rcret = 0;
	char* ret = NULL;
	struct skin* tmp = NULL;

	struct skin* mc_iptvplayer_settings = getscreen("mc_iptvplayer_settings");
	struct skin* listbox = getscreennode(mc_iptvplayer_settings, "listbox");
	struct skin* dirsort = getscreennode(mc_iptvplayer_settings, "sort");
	struct skin* view = getscreennode(mc_iptvplayer_settings, "view");
	struct skin* autoscan = getscreennode(mc_iptvplayer_settings, "autoscan");
	struct skin* autostart_playlist = getscreennode(mc_iptvplayer_settings, "autostart_playlist");
	struct skin* defaultdir = getscreennode(mc_iptvplayer_settings, "defaultdir");
	struct skin* uselastdir = getscreennode(mc_iptvplayer_settings, "uselastdir");
	struct skin* backdrop = getscreennode(mc_iptvplayer_settings, "backdrop");

	struct skin* loadmediadb = getscreen("loading");
	struct skin* blackscreen = getscreen("blackscreen");

	addchoicebox(dirsort, "0", _("alpha"));	
	addchoicebox(dirsort, "1", _("reverse alpha"));	
	addchoicebox(dirsort, "2", _("size"));
	addchoicebox(dirsort, "3", _("reverse size"));
	addchoicebox(dirsort, "4", _("date"));
	addchoicebox(dirsort, "5", _("reverse date"));
	setchoiceboxselection(dirsort, getconfig("mc_ip_dirsort", NULL));

	addchoicebox(view, "0", _("fast"));
	addchoicebox(view, "1", _("big"));	
	addchoicebox(view, "2", _("cover"));
	addchoicebox(view, "3", _("fullcover"));	
	addchoicebox(view, "4", _("size (default)"));
	addchoicebox(view, "5", _("datetime"));
	setchoiceboxselection(view, getconfig("mc_ip_view", NULL));

	addchoicebox(autostart_playlist, "0", _("no"));	
	addchoicebox(autostart_playlist, "1", _("yes"));
	setchoiceboxselection(autostart_playlist, getconfig("mc_ip_autostart_playlist", NULL));

	addchoicebox(autoscan, "0", _("no"));	
	addchoicebox(autoscan, "1", _("yes"));
	setchoiceboxselection(autoscan, getconfig("mc_ip_autoscan", NULL));

	changeinput(defaultdir, getconfig("mc_ip_defaultdir", NULL));

	addchoicebox(uselastdir, "0", _("no"));
	addchoicebox(uselastdir, "1", _("yes"));
	setchoiceboxselection(uselastdir, getconfig("mc_ip_uselastdir", NULL));

	addchoicebox(backdrop, "0", _("off"));
	addchoicebox(backdrop, "1", _("1s"));	
	addchoicebox(backdrop, "2", _("2s"));
	addchoicebox(backdrop, "3", _("3s"));	
	addchoicebox(backdrop, "4", _("4s"));
	addchoicebox(backdrop, "5", _("5s (default)"));
	setchoiceboxselection(backdrop, getconfig("mc_ip_backdrop_interval", NULL));
		
	drawscreen(mc_iptvplayer_settings, 0, 0);
	addscreenrc(mc_iptvplayer_settings, listbox);

	tmp = listbox->select;
	while(1)
	{
		addscreenrc(mc_iptvplayer_settings, tmp);
		rcret = waitrc(mc_iptvplayer_settings, 0, 0);
		tmp = listbox->select;
	
		if(rcret == getrcconfigint("rcexit", NULL))
			break;
		else if(rcret == getrcconfigint("rcok", NULL))
		{
			addconfigscreencheck("mc_ip_dirsort", dirsort, NULL);
			addconfigscreencheck("mc_ip_view", view, NULL);
			addconfigscreencheck("mc_ip_autoscan", autoscan, NULL);			
			addconfigscreencheck("iptv_autostart_playlist", autostart_playlist, NULL);		
			addconfigscreen("mc_ip_defaultdir", defaultdir);
			addconfigscreencheck("mc_ip_uselastdir", uselastdir, NULL);
			addconfigscreencheck("mc_ip_backdrop_interval", backdrop, NULL);
			break;
		}
		else if(rcret == getrcconfigint("rcred", NULL))
		{
			struct skin* screen = getscreen("networkmenu");
			menu(screen);
			drawscreen(mc_iptvplayer_settings, 0, 0);
		}
		else if(rcret == getrcconfigint("rcgreen", NULL))
		{
			if(listbox->select != NULL && ostrcmp(listbox->select->name, "defaultdir") == 0)
			{
				clearscreen(mc_iptvplayer_settings);
				ret = screendir(listbox->select->ret, "", NULL, NULL, NULL, NULL, 0, "SELECT", 0, NULL, 0, NULL, 0, 750, 0, 650, 0, 0);
				if(ret != NULL)
					changeinput(listbox->select, ret);
				free(ret);

				drawscreen(mc_iptvplayer_settings, 0, 0);
			}
		}		
	}
	
	drawscreen(blackscreen, 0, 0);
	drawscreen(loadmediadb, 0, 0);
	writeallconfig(0);
	delownerrc(mc_iptvplayer_settings);
	clearscreen(mc_iptvplayer_settings);
//	free(ext2);
//	free(ext3);
//	free(vfat);
//	free(ntfsmount);
//	free(ntfs3g);
//	free(cifs);
//	free(nfs);
	resettvpic();
}

#endif
