#ifndef EPGSETTINGS_H
#define EPGSETTINGS_H

void createepgscanlist(struct skin* scanlist, struct skin* listbox)
{
	struct epgscanlist *node = epgscanlist;
	struct skin* tmp = NULL;

	while(node != NULL)
	{
		struct channel* chnode = getchannel(node->serviceid, node->transponderid);
		if(chnode != NULL)
		{
			tmp = addlistbox(scanlist, listbox, tmp, 1);
			if(tmp != NULL)
			{
				changetext(tmp, chnode->name);
				tmp->handle = (char*)node;
			}
		}
		node = node->next;
	}
}

void screenepgscanlist()
{
	int rcret = 0;
	struct skin* scanlist = getscreen("epgscanlist");
	struct skin* listbox = getscreennode(scanlist, "listbox");
	char* tmpstr = NULL, *tmpchannellist = NULL;

	if(status.servicetype == 0)
		tmpchannellist = ostrcat(getconfig("channellist", NULL), NULL, 0, 0);
	else
		tmpchannellist = ostrcat(getconfig("rchannellist", NULL), NULL, 0, 0);

	scanlist->aktline = 1;
	scanlist->aktpage = -1;

	createepgscanlist(scanlist, listbox);

	drawscreen(scanlist, 0, 0);
	addscreenrc(scanlist, listbox);

	while(1)
	{
		rcret = waitrc(scanlist, 0, 0);

		if(rcret == getrcconfigint("rcok", NULL)) break;
		if(rcret == getrcconfigint("rcred", NULL) && listbox->select != NULL && listbox->select->handle != NULL)
		{
			if(delepgscanlist(((struct epgscanlist*)listbox->select->handle)->serviceid, ((struct epgscanlist*)listbox->select->handle)->transponderid) == 0)
			{
				listbox->aktline--;
				listbox->aktpage = -1;
				delmarkedscreennodes(scanlist, 1);
				createepgscanlist(scanlist, listbox);
				drawscreen(scanlist, 0, 0);
			}
		}
		if(rcret == getrcconfigint("rcgreen", NULL))
		{
			clearscreen(scanlist);
			int saveservicetype = status.servicetype;
			struct channel* tmpchnode = NULL;

			screenchannellist(&tmpchnode, &tmpchannellist, 1);
			status.servicetype = saveservicetype;

			if(tmpchnode != NULL && getepgscanlist(tmpchnode->serviceid, tmpchnode->transponderid) == NULL)
			{
				tmpstr = ostrcat(oitoa(tmpchnode->serviceid), "#", 1, 0);
				tmpstr = ostrcat(tmpstr, ollutoa(tmpchnode->transponderid), 1, 1);
				if(addepgscanlist(tmpstr, 1, NULL) != NULL)
				{
					delmarkedscreennodes(scanlist, 1);
					createepgscanlist(scanlist, listbox);
				}
				free(tmpstr); tmpstr = NULL;
			}

			drawscreen(scanlist, 0, 0);
		}
		if(rcret == getrcconfigint("rcyellow", NULL))
		{
			clearscreen(scanlist);
			struct mainbouquet* mbouquet = screenmainbouquet();
			
			mainbouquet2epgscanlist(mbouquet);
			delmarkedscreennodes(scanlist, 1);
			createepgscanlist(scanlist, listbox);
			
			drawscreen(scanlist, 0, 0);
		}
	}

	delmarkedscreennodes(scanlist, 1);
	delownerrc(scanlist);
	clearscreen(scanlist);
}

void screenepgsettings()
{
	int rcret = 0;
	char* ret = NULL;
	struct skin* epgsettings = getscreen("epgsettings");
	struct skin* listbox = getscreennode(epgsettings, "listbox");
	struct skin* epgpath = getscreennode(epgsettings, "epgpath");
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
	struct skin* delepgbeforescan = getscreennode(epgsettings, "delepgbeforescan");
	struct skin* mhwepg = getscreennode(epgsettings, "mhwepg");
	struct skin* mhw2epg = getscreennode(epgsettings, "mhw2epg");
	struct skin* opentv = getscreennode(epgsettings, "opentv");
	struct skin* epg_afterevent = getscreennode(epgsettings, "epg_afterevent");
	struct skin* b4 = getscreennode(epgsettings, "b4");

	struct skin* load = getscreen("loading");
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
	addchoicebox(epglistmode, "3", _("scan and whitelist"));
	setchoiceboxselection(epglistmode, getconfig("epg_listmode", NULL));

	addchoicebox(epgrefresh, "0", _("deaktiv"));
	addchoicebox(epgrefresh, "01:00", "01:00");
	addchoicebox(epgrefresh, "02:00", "02:00");
	addchoicebox(epgrefresh, "03:00", "03:00");
	addchoicebox(epgrefresh, "04:00", "04:00");
	addchoicebox(epgrefresh, "05:00", "05:00");
	addchoicebox(epgrefresh, "06:00", "06:00");
	addchoicebox(epgrefresh, "07:00", "07:00");
	addchoicebox(epgrefresh, "08:00", "08:00");
	addchoicebox(epgrefresh, "09:00", "09:00");
	addchoicebox(epgrefresh, "10:00", "10:00");
	addchoicebox(epgrefresh, "11:00", "11:00");
	addchoicebox(epgrefresh, "12:00", "12:00");
	addchoicebox(epgrefresh, "13:00", "13:00");
	addchoicebox(epgrefresh, "14:00", "14:00");
	addchoicebox(epgrefresh, "15:00", "15:00");
	addchoicebox(epgrefresh, "16:00", "16:00");
	addchoicebox(epgrefresh, "17:00", "17:00");
	addchoicebox(epgrefresh, "18:00", "18:00");
	addchoicebox(epgrefresh, "19:00", "19:00");
	addchoicebox(epgrefresh, "20:00", "20:00");
	addchoicebox(epgrefresh, "21:00", "21:00");
	addchoicebox(epgrefresh, "22:00", "22:00");
	addchoicebox(epgrefresh, "23:00", "23:00");
	addchoicebox(epgrefresh, "23:59", "24:00");
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

	addchoicebox(delepgbeforescan, "0", _("no"));
	addchoicebox(delepgbeforescan, "1", _("yes"));
	setchoiceboxselection(delepgbeforescan, getconfig("delepgbeforescan", NULL));

	addchoicebox(mhwepg, "0", _("no"));
	addchoicebox(mhwepg, "1", _("only for scan"));
	addchoicebox(mhwepg, "2", _("always"));
	setchoiceboxselection(mhwepg, getconfig("mhwepg", NULL));

	addchoicebox(mhw2epg, "0", _("no"));
	addchoicebox(mhw2epg, "1", _("only for scan"));
	addchoicebox(mhw2epg, "2", _("always"));
	setchoiceboxselection(mhw2epg, getconfig("mhw2epg", NULL));

	addchoicebox(opentv, "0", _("no"));
	addchoicebox(opentv, "1", _("only for scan"));
	addchoicebox(opentv, "2", _("always"));
	setchoiceboxselection(opentv, getconfig("opentv", NULL));

	addchoicebox(epg_afterevent, "0", _("nothing"));
	addchoicebox(epg_afterevent, "1", _("poweroff"));
	setchoiceboxselection(epg_afterevent, getconfig("epg_afterevent", NULL));

	drawscreen(epgsettings, 0, 0);
	addscreenrc(epgsettings, listbox);

	tmp = listbox->select;
	while(1)
	{
		addscreenrc(epgsettings, tmp);
		rcret = waitrc(epgsettings, 0, 0);
		tmp = listbox->select;

		if(ostrcmp(listbox->select->name, "epgpath") == 0)
			b4->hidden = NO;
		else
			b4->hidden = YES;

		if(rcret == getrcconfigint("rcexit", NULL)) break;
		if(rcret == getrcconfigint("rcok", NULL))
		{
			addconfigscreen("epg_path", epgpath);
			addconfigscreen("epg_days", epgdays);
			status.epgdays = getconfigint("epg_days", NULL);
			addconfigscreen("epg_del", epgdel);
			addconfigscreen("epg_screen", epgscreen);
			addconfigscreen("epgfreespace", epgfreespace);
			addconfigscreencheck("epg_listmode", epglistmode, "0");
			status.epglistmode = getconfigint("epg_listmode", NULL);
			addconfigscreencheck("epg_refreshtime", epgrefresh, "0");
			addconfigscreencheck("epgbutton", epgbutton, "0");
			addconfigscreen("gmultiepgzoom", epgzoom);
			addconfigscreencheck("epgpicon", epgpicon, "0");
			addconfigscreencheck("epgsave", epgsave, "0");
			addconfigscreencheck("delepgbeforescan", delepgbeforescan, "0");
			addconfigscreencheck("mhwepg", mhwepg, "0");
			addconfigscreencheck("mhw2epg", mhw2epg, "0");
			addconfigscreencheck("opentv", opentv, "0");
			addconfigscreencheck("epg_afterevent", epg_afterevent, "0");

			if(getconfig("epg_refreshtime", NULL) == NULL || ostrcmp(getconfig("epg_refreshtime", NULL), "0") == 0)
				epgscandeltimer();
			else
				epgscancreatetimer();
			
			if(getconfig("epg_refreshtime", NULL) != NULL || getconfigint("epg_afterevent", NULL) != 0)
			{   	
				if(getconfigint("epg_listmode", NULL) == 1 || getconfigint("epg_listmode", NULL) == 3)
				{
					textbox(_("Message"), _("Deavtivate Refresh Time and After EPG in scan-mode"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 800, 200, 15, 0);
					delconfig("epg_refreshtime");
					delconfig("epg_afterevent");		
				}
			}

			break;
		}
		if(rcret == getrcconfigint("rcgreen", NULL))
		{
			if(listbox->select != NULL && ostrcmp(listbox->select->name, "epgpath") == 0)
			{
				clearscreen(epgsettings);
				ret = screendir(listbox->select->ret, "", NULL, NULL, NULL, NULL, 0, "SELECT", 0, NULL, 0, NULL, 0, 750, 0, 650, 0, 0);
				if(ret != NULL)
					changeinput(listbox->select, ret);
				free(ret);

				drawscreen(epgsettings, 0, 0);
			}
		}
		if(rcret == getrcconfigint("rcred", NULL))
		{
			drawscreen(load, 0, 0);
			resetepg(0);
			clearscreen(load);
			textbox(_("Message"), _("EPG resetet and now clear."), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 0, 0);
			drawscreen(epgsettings, 0, 0);
		}
		if(rcret == getrcconfigint("rcyellow", NULL))
		{
			clearscreen(epgsettings);
			screenepgscanlist();
			drawscreen(epgsettings, 0, 0);
		}
		if(rcret == getrcconfigint("rcblue", NULL))
		{
			clearscreen(epgsettings);
			char* log = readfiletomem(EPGSCANLOG, 0);
			if(log == NULL) log = ostrcat("No log infos available.", NULL, 0, 0);
			textbox(_("EPG Scan Log"), _(log), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 1000, 600, 0, 0);
			free(log); log = NULL;
			drawscreen(epgsettings, 0, 0);
		}
	}

	delownerrc(epgsettings);
	clearscreen(epgsettings);
}

#endif
