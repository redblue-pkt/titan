#ifndef EPGSETTINGS_H
#define EPGSETTINGS_H

void screenepgsettings()
{
	int rcret = 0;
	char* ret = NULL;
	struct skin* epgsettings = getscreen("epgsettings");
	struct skin* listbox = getscreennode(epgsettings, "listbox");
	struct skin *epgpath = getscreennode(epgsettings, "epgpath");
	struct skin* epgdays = getscreennode(epgsettings, "epgdays");
	struct skin* epgdel = getscreennode(epgsettings, "epgdel");
	struct skin* epgscreen = getscreennode(epgsettings, "epgscreen");
	struct skin* epgfreespace = getscreennode(epgsettings, "epgfreespace");
	struct skin* epglistmode = getscreennode(epgsettings, "epglistmode");
	struct skin* epgrefresh = getscreennode(epgsettings, "epgrefresh");
	struct skin* epgbutton = getscreennode(epgsettings, "epgbutton");
	struct skin* epgzoom = getscreennode(epgsettings, "epgzoom");
	struct skin* epgpicon = getscreennode(epgsettings, "epgpicon");
	struct skin* epgsave = getscreennode(epgsettings, "epgsave");
	struct skin* tmp = NULL;

	changeinput(epgpath, getconfig("epg_path", NULL));

	changeinput(epgdays, "0\n2\n4\n6\n8\n10\n12\n14\n16\n18\n20");
	setchoiceboxselection(epgdays, getconfig("epg_days", NULL));

	addchoicebox(epgdel, "0", _("no"));
	addchoicebox(epgdel, "1", _("yes"));
	setchoiceboxselection(epgdel, getconfig("epg_del", NULL));

	addchoicebox(epgscreen, "0", _("Channel EPG"));
	addchoicebox(epgscreen, "1", _("Single EPG"));
	addchoicebox(epgscreen, "2", _("Multi EPG"));
	addchoicebox(epgscreen, "3", _("Graphic Multi EPG"));
	setchoiceboxselection(epgscreen, getconfig("epg_screen", NULL));

	changeinput(epgfreespace, "50\n100\n200\n300\n400\n500\n600\n700\n800\n900\n1000");
	setchoiceboxselection(epgfreespace, getconfig("epgfreespace", NULL));

	addchoicebox(epglistmode, "0", _("deaktiv"));
	addchoicebox(epglistmode, "1", _("scan"));
	addchoicebox(epglistmode, "2", _("whitelist"));
	addchoicebox(epglistmode, "2", _("scan and whitelist"));
	setchoiceboxselection(epglistmode, getconfig("epg_listmode", NULL));

	char* tmpstr = NULL;
	tmpstr = ostrcat(_("deaktiv"), "\n", 0, 0);
	tmpstr = ostrcat(tmpstr, "deaktiv\n01:00\n02:00\n03:00\n04:00\n05:00\n06:00\n07:00\n08:00\n09:00\n10:00\n11:00\n12:00\n13:00\n14:00\n15:00\n16:00\n17:00\n18:00\n19:00\n20:00\n21:00\n22:00\n23:00\n24:00", 1, 0);		
	changeinput(epgrefresh, tmpstr);
	free(tmpstr), tmpstr = NULL;
	setchoiceboxselection(epgrefresh, getconfig("epg_refreshtime", NULL));
		
	addchoicebox(epgbutton, "0", _("Single EPG")); 	 	 
	addchoicebox(epgbutton, "1", _("Multi EPG")); 	 	 
	setchoiceboxselection(epgbutton, getconfig("epgbutton", NULL));

	changeinput(epgzoom, "1\n2\n3\n4\n5\n6\n7");
	setchoiceboxselection(epgzoom, getconfig("gmultiepgzoom", NULL));
	
	addchoicebox(epgpicon, "0", _("no"));
	addchoicebox(epgpicon, "1", _("yes"));
	setchoiceboxselection(epgpicon, getconfig("epgpicon", NULL));
	
	addchoicebox(epgsave, "0", _("always"));
	addchoicebox(epgsave, "1", _("only on power off / restart"));
	addchoicebox(epgsave, "2", _("never"));
	setchoiceboxselection(epgsave, getconfig("epgsave", NULL));

	drawscreen(epgsettings, 0);
	addscreenrc(epgsettings, listbox);

	tmp = listbox->select;
	while(1)
	{
		addscreenrc(epgsettings, tmp);
		rcret = waitrc(epgsettings, 0, 0);
		tmp = listbox->select;

		if(rcret == getrcconfigint("rcexit", NULL)) break;
		if(rcret == getrcconfigint("rcgreen", NULL))
		{
			addconfigscreen("epg_path", epgpath);
			addconfigscreen("epg_days", epgdays);
			status.epgdays = getconfigint("epg_days", NULL);
			addconfigscreen("epg_del", epgdel);
			addconfigscreen("epg_screen", epgscreen);
			addconfigscreen("epgfreespace", epgfreespace);
			addconfigscreencheck("epg_listmode", epglistmode, "0");
			status.epglistmode = getconfigint("epg_listmode", NULL);
			addconfigscreencheck("epg_refreshtime", epgrefresh, "deaktiv");
			addconfigscreencheck("epgbutton", epgbutton, "0");
			addconfigscreen("gmultiepgzoom", epgzoom);
			addconfigscreencheck("epgpicon", epgpicon, "0");
			addconfigscreencheck("epgsave", epgsave, "0");
			break;
		}
		if(rcret == getrcconfigint("rcok", NULL))
		{
			if(listbox->select != NULL && ostrcmp(listbox->select->name, "epgpath") == 0)
			{
				clearscreen(epgsettings);
				ret = screendir(listbox->select->ret, "", NULL, NULL, NULL, NULL, 0, "SELECT", 0, NULL, 0, NULL, 0, 700, 0, 650, 0, 0);
				if(ret != NULL)
					changeinput(listbox->select, ret);
				free(ret);

				drawscreen(epgsettings, 0);
			}
		}
		if(rcret == getrcconfigint("rcred", NULL))
		{
			resetepg();
			textbox(_("Message"), _("EPG resetet and now clear."), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, NULL, 0, 600, 200, 0, 0);
			drawscreen(epgsettings, 0);
		}
	}

	delownerrc(epgsettings);
	clearscreen(epgsettings);
}

#endif
