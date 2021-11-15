#ifndef MODULECONFIG_H
#define MODULECONFIG_H

void createchannelslotlist(struct skin* channelslotlist, struct skin* listbox, int slot)
{
	struct channelslot *node = channelslot;
	struct skin* tmp = NULL;

	while(node != NULL)
	{
		if(node->slot == slot)
		{
			struct channel* chnode = getchannel(node->serviceid, node->transponderid);
			if(chnode != NULL)
			{
				tmp = addlistbox(channelslotlist, listbox, tmp, 1);
				if(tmp != NULL)
				{
					changetext(tmp, chnode->name);
					tmp->handle = (char*)node;
				}
			}
		}
		node = node->next;
	}
}

void screenchannelslot(int slot)
{
	int rcret = 0;
	struct skin* channelslotlist = getscreen("channelslotlist");
	struct skin* listbox = getscreennode(channelslotlist, "listbox");
	char* tmpstr = NULL, *tmpchannellist = NULL;

	if(status.servicetype == 0)
		tmpchannellist = ostrcat(getconfig("channellist", NULL), NULL, 0, 0);
	else
		tmpchannellist = ostrcat(getconfig("rchannellist", NULL), NULL, 0, 0);

	channelslotlist->aktline = 1;
	channelslotlist->aktpage = -1;

	createchannelslotlist(channelslotlist, listbox, slot);

	drawscreen(channelslotlist, 0, 0);
	addscreenrc(channelslotlist, listbox);

	while(1)
	{
		rcret = waitrc(channelslotlist, 0, 0);

		if(rcret == getrcconfigint("rcok", NULL)) break;
		if(rcret == getrcconfigint("rcred", NULL) && listbox->select != NULL && listbox->select->handle != NULL)
		{
			if(delchannelslot(((struct channelslot*)listbox->select->handle)->serviceid, ((struct channelslot*)listbox->select->handle)->transponderid) == 0)
			{
				listbox->aktline--;
				listbox->aktpage = -1;
				delmarkedscreennodes(channelslotlist, 1);
				createchannelslotlist(channelslotlist, listbox, slot);
				drawscreen(channelslotlist, 0, 0);
			}
		}
		if(rcret == getrcconfigint("rcgreen", NULL))
		{
			clearscreen(channelslotlist);
			int saveservicetype = status.servicetype;
			struct channel* tmpchnode = NULL;

			screenchannellist(&tmpchnode, &tmpchannellist, 1);
			status.servicetype = saveservicetype;

			if(tmpchnode != NULL && getchannelslot(tmpchnode->serviceid, tmpchnode->transponderid) == NULL)
			{
				tmpstr = ostrcat(oitoa(tmpchnode->transponderid), "#", 1, 0);
				tmpstr = ostrcat(tmpstr, ollutoa(tmpchnode->serviceid), 1, 1);
				tmpstr = ostrcat(tmpstr, "#", 1, 0);
				tmpstr = ostrcat(tmpstr, oitoa(slot), 1, 1);
				if(addchannelslot(tmpstr, 1, NULL) != NULL)
				{
					delmarkedscreennodes(channelslotlist, 1);
					createchannelslotlist(channelslotlist, listbox, slot);
				}
				free(tmpstr); tmpstr = NULL;
			}

			drawscreen(channelslotlist, 0, 0);
		}
		if(rcret == getrcconfigint("rcyellow", NULL))
		{
			clearscreen(channelslotlist);
			struct mainbouquet* mbouquet = screenmainbouquet();
			
			mainbouquet2channelslot(mbouquet, slot);
			delmarkedscreennodes(channelslotlist, 1);
			createchannelslotlist(channelslotlist, listbox, slot);
			
			drawscreen(channelslotlist, 0, 0);
		}
	}

	delmarkedscreennodes(channelslotlist, 1);
	delownerrc(channelslotlist);
	clearscreen(channelslotlist);
}

void screenchannelslotdel(int slot)
{
	int rcret = 0;
	struct skin* channelslotlist = getscreen("channelslotlist");
	struct skin* listbox = getscreennode(channelslotlist, "listbox");
	struct skin* titeltext = getscreennode(channelslotlist, "titletext");
	char* tmpstr = NULL, *tmpchannellist = NULL;

	changetext(titeltext, "exclude Sender");
	
	if(status.servicetype == 0)
		tmpchannellist = ostrcat(getconfig("channellist", NULL), NULL, 0, 0);
	else
		tmpchannellist = ostrcat(getconfig("rchannellist", NULL), NULL, 0, 0);

	channelslotlist->aktline = 1;
	channelslotlist->aktpage = -1;

	createchannelslotlist(channelslotlist, listbox, slot);

	drawscreen(channelslotlist, 0, 0);
	addscreenrc(channelslotlist, listbox);

	while(1)
	{
		rcret = waitrc(channelslotlist, 0, 0);

		if(rcret == getrcconfigint("rcok", NULL)) break;
		if(rcret == getrcconfigint("rcred", NULL) && listbox->select != NULL && listbox->select->handle != NULL)
		{
			if(delchannelslot(((struct channelslot*)listbox->select->handle)->serviceid, ((struct channelslot*)listbox->select->handle)->transponderid) == 0)
			{
				listbox->aktline--;
				listbox->aktpage = -1;
				delmarkedscreennodes(channelslotlist, 1);
				createchannelslotlist(channelslotlist, listbox, slot);
				drawscreen(channelslotlist, 0, 0);
			}
		}
		if(rcret == getrcconfigint("rcgreen", NULL))
		{
			clearscreen(channelslotlist);
			int saveservicetype = status.servicetype;
			struct channel* tmpchnode = NULL;

			screenchannellist(&tmpchnode, &tmpchannellist, 1);
			status.servicetype = saveservicetype;

			if(tmpchnode != NULL && getchannelslot(tmpchnode->serviceid, tmpchnode->transponderid) == NULL)
			{
				tmpstr = ostrcat(oitoa(tmpchnode->transponderid), "#", 1, 0);
				tmpstr = ostrcat(tmpstr, ollutoa(tmpchnode->serviceid), 1, 1);
				tmpstr = ostrcat(tmpstr, "#", 1, 0);
				tmpstr = ostrcat(tmpstr, oitoa(slot), 1, 1);
				if(addchannelslot(tmpstr, 1, NULL) != NULL)
				{
					delmarkedscreennodes(channelslotlist, 1);
					createchannelslotlist(channelslotlist, listbox, slot);
				}
				free(tmpstr); tmpstr = NULL;
			}

			drawscreen(channelslotlist, 0, 0);
		}
		if(rcret == getrcconfigint("rcyellow", NULL))
		{
			clearscreen(channelslotlist);
			struct mainbouquet* mbouquet = screenmainbouquet();
			
			mainbouquet2channelslot(mbouquet, slot);
			delmarkedscreennodes(channelslotlist, 1);
			createchannelslotlist(channelslotlist, listbox, slot);
			
			drawscreen(channelslotlist, 0, 0);
		}
	}

	delmarkedscreennodes(channelslotlist, 1);
	delownerrc(channelslotlist);
	clearscreen(channelslotlist);
}

int checkdoublecaid(struct dvbdev* excdvbnode, char* caid)
{
	struct dvbdev* dvbnode = dvbdev;

	if(caid == NULL) return 0;

	while(dvbnode != NULL)
	{
		if(dvbnode->type == CIDEV && dvbnode->caslot != NULL && dvbnode->caslot->caids != NULL && dvbnode != excdvbnode)
		{
			if(ostrstr(dvbnode->caslot->caids, caid) != NULL)
				return 1;

		}
		dvbnode = dvbnode->next;
	}
	return 0;
}

void screencaidlock(struct dvbdev* dvbnode)
{
	int rcret = -1, count = 0, i = 0;
	struct skin* caidlock = getscreen("caidlock");
	struct skin* listbox = getscreennode(caidlock, "listbox");
	struct skin* tmp = NULL;
	char* tmpstr = NULL, *configname = NULL, *caids = NULL;
	struct splitstr* ret = NULL;

	listbox->aktline = 1;
	listbox->aktpage = -1;

	if(dvbnode == NULL || dvbnode->caslot == NULL || dvbnode->caslot->caids == NULL) return;

	configname = ostrcat("camblacklist_", oitoa(dvbnode->devnr), 0, 1);
	char* lockcaids = getconfig(configname, NULL);

	caids = ostrcat(dvbnode->caslot->caids, NULL, 0, 0);
	ret = strsplit(caids, "#", &count);

	if(ret != NULL)
	{
		for(i = 0; i < count; i++)
		{
			tmp = addlistbox(caidlock, listbox, tmp, 1);
			if(tmp != NULL)
			{
				tmp->type = CHOICEBOX;

				// show caid in hex
				char* caid_hex = ostrcat(ret[i].part, NULL, 0, 0);
				int caid_dec = atoi(ret[i].part);
				sprintf(caid_hex, "%x", caid_dec);

				if(checkdoublecaid(dvbnode, ret[i].part) == 1)
				{
					tmpstr = ostrcat(tmpstr, ret[i].part, 1, 0);
					if(caid_hex != NULL)
					{
						tmpstr = ostrcat(tmpstr, " (0x", 1, 0);
						tmpstr = ostrcat(tmpstr, caid_hex, 1, 0);
						tmpstr = ostrcat(tmpstr, ") (", 1, 0);
					}
					else
						tmpstr = ostrcat(tmpstr, " (", 1, 0);

					tmpstr = ostrcat(tmpstr, _("double"), 1, 0);
					tmpstr = ostrcat(tmpstr, ")", 1, 0);
				}
				else
				{
					tmpstr = ostrcat(tmpstr, ret[i].part, 1, 0);
					if(caid_hex != NULL)
					{
						tmpstr = ostrcat(tmpstr, " (0x", 1, 0);
						tmpstr = ostrcat(tmpstr, caid_hex, 1, 0);
						tmpstr = ostrcat(tmpstr, ")", 1, 0);
					}
				}
				free(caid_hex);

				changetext(tmp, tmpstr);
				free(tmpstr); tmpstr = NULL;

				addchoicebox(tmp, "0", _("activate"));
				addchoicebox(tmp, "1", _("deactivate"));
				if(ostrstr(lockcaids, ret[i].part) != NULL)
					setchoiceboxselection(tmp, "1");
				else
					setchoiceboxselection(tmp, "0");
				changename(tmp, ret[i].part);
			}
		}
	}

	free(caids); caids = NULL;
	free(ret); ret = NULL;
	drawscreen(caidlock, 0, 0);
	addscreenrc(caidlock, listbox);

	tmp = listbox->select;
	while(1)
	{
		addscreenrc(caidlock, tmp);
		rcret = waitrc(caidlock, 0, 0);
		tmp = listbox->select;

		if(rcret == getrcconfigint("rcexit", NULL)) break;
		if(rcret == getrcconfigint("rcok", NULL))
		{
			tmp = listbox;
			while(tmp != NULL)
			{
				if(tmp->del == 1 && ostrcmp(tmp->ret, "1") == 0)
				{
					tmpstr = ostrcat(tmpstr, "#", 1, 0);
					tmpstr = ostrcat(tmpstr, tmp->name, 1, 0);
					tmpstr = ostrcat(tmpstr, "#", 1, 0);
				}
				tmp = tmp->next;
			}

			if(tmpstr != NULL)
				addconfig(configname, tmpstr);
			else
				delconfig(configname);

			free(tmpstr); tmpstr = NULL;
			break;
		}
	}

	free(configname); configname = NULL;
	delmarkedscreennodes(caidlock, 1);
	delownerrc(caidlock);
	clearscreen(caidlock);
}

void screenmoduleadjust()
{
	int rcret = 0;

	struct skin* moduleadjust = getscreen("moduleadjust");
	struct skin* listbox = getscreennode(moduleadjust, "listbox");
	struct skin* nocamsg = getscreennode(moduleadjust, "nocamsg");
	struct skin* camwait = getscreennode(moduleadjust, "camwait");
	struct skin* checkcamdecrypt = getscreennode(moduleadjust, "checkcamdecrypt");
	struct skin* caskipprivat = getscreennode(moduleadjust, "caskipprivat");
	struct skin* casendallcaids = getscreennode(moduleadjust, "casendallcaids");
	struct skin* extmoduldetect = getscreennode(moduleadjust, "extmoduldetect");

	struct skin* tmp = NULL;

	if(checkbox("ATEMIO7600") == 1 || checkbox("ATEMIO510") == 1 || checkbox("ATEVIO700") == 1 || checkbox("ATEVIO7000") == 1 || checkbox("WHITEBOX") == 1 || checkbox("ATEMIO520") == 1 || checkbox("ATEMIO530") == 1)
		extmoduldetect->hidden = NO;
	else
		extmoduldetect->hidden = YES;

	addchoicebox(nocamsg, "0", _("yes"));
	addchoicebox(nocamsg, "1", _("no"));
	setchoiceboxselection(nocamsg, getconfig("nocamsg", NULL));

	addchoicebox(camwait, "25", _("slow"));
	addchoicebox(camwait, "15", _("middle"));
	addchoicebox(camwait, "5", _("fast"));
	setchoiceboxselection(camwait, getconfig("camwait", NULL));

	addchoicebox(checkcamdecrypt, "0", _("no"));
	addchoicebox(checkcamdecrypt, "1", _("yes"));
	setchoiceboxselection(checkcamdecrypt, getconfig("checkcamdecrypt", NULL));

	addchoicebox(caskipprivat, "0", _("no"));
	addchoicebox(caskipprivat, "1", _("yes"));
	setchoiceboxselection(caskipprivat, getconfig("caskipprivat", NULL));

	addchoicebox(casendallcaids, "0", _("no"));
	addchoicebox(casendallcaids, "1", _("yes"));
	setchoiceboxselection(casendallcaids, getconfig("casendallcaids", NULL));

	addchoicebox(extmoduldetect, "0", _("no"));
	addchoicebox(extmoduldetect, "1", _("yes"));
	setchoiceboxselection(extmoduldetect, getownconfig("extmoduldetect"));

	drawscreen(moduleadjust, 0, 0);
	addscreenrc(moduleadjust, listbox);

	tmp = listbox->select;
	while(1)
	{
		addscreenrc(moduleadjust, tmp);
		rcret = waitrc(moduleadjust, 0, 0);
		tmp = listbox->select;

		if(rcret == getrcconfigint("rcexit", NULL)) break;
		if(rcret == getrcconfigint("rcok", NULL))
		{
			addconfigscreencheck("nocamsg", nocamsg, "0");
			addconfigscreencheck("camwait", camwait, "25");
			addconfigscreencheck("checkcamdecrypt", checkcamdecrypt, "0");
			addconfigscreencheck("caskipprivat", caskipprivat, "0");
			status.caskipprivat = getconfigint("caskipprivat", NULL);
			addconfigscreencheck("casendallcaids", casendallcaids, "0");
			status.casendallcaids = getconfigint("casendallcaids", NULL);

			if(checkbox("ATEMIO7600") == 1 || checkbox("ATEMIO510") == 1 || checkbox("ATEVIO700") == 1 || checkbox("ATEVIO7000") == 1 || checkbox("WHITEBOX") == 1 || checkbox("ATEMIO520") == 1 || checkbox("ATEMIO530") == 1)
			{
				if(extmoduldetect->ret != NULL)
					addownconfig("extmoduldetect", extmoduldetect->ret);
			}

			break;
		}
	}

	delownerrc(moduleadjust);
	clearscreen(moduleadjust);
}

void screenmoduleconfig()
{
	int rcret = 0, i = 0, reset = 0, allready = 1;
	struct skin* moduleconfig = getscreen("moduleconfig");
	struct skin* listbox = getscreennode(moduleconfig, "listbox");
	struct skin* b3 = getscreennode(moduleconfig, "b3");
	struct skin* b4 = getscreennode(moduleconfig, "b4");
	struct skin* b5 = getscreennode(moduleconfig, "b5");
	struct skin* b6 = getscreennode(moduleconfig, "b6");
	struct skin* load = getscreen("loading");
	struct skin* tmp = NULL;
	struct dvbdev* dvbnode = NULL;
	char* tmpstr = NULL, *tmpnr = NULL;

	listbox->aktline = 1;
	listbox->aktpage = -1;

	b3->usesavebg = 1;
	b4->usesavebg = 1;
	b5->usesavebg = 1;
	b6->usesavebg = 1;

start:
	i = 0, allready = 1;
	dvbnode = dvbdev;
	tmp = NULL;
	delmarkedscreennodes(moduleconfig, 1);

	while(dvbnode != NULL)
	{
		if(dvbnode->type == CIDEV)
		{
			i++;
			tmp = addlistbox(moduleconfig, listbox, tmp, 1);
			if(tmp != NULL)
			{
				tmpstr = ostrcat(tmpstr, _("Slot "), 1, 0);
				tmpnr = oitoa(i);
				tmpstr = ostrcat(tmpstr, tmpnr, 1, 1);
				tmpstr = ostrcat(tmpstr, ": ", 1, 0);
				if(dvbnode->caslot != NULL)
				{
					if(dvbnode->caslot->name == NULL || dvbnode->caslot->status == 0)
						tmpstr = ostrcat(tmpstr, _("unknown"), 1, 0);
					else
						tmpstr = ostrcat(tmpstr, dvbnode->caslot->name, 1, 0);
				}
				tmpstr = ostrcat(tmpstr, "\n", 1, 0);
				tmpstr = ostrcat(tmpstr, _("Status: "), 1, 0);
				if(dvbnode->caslot != NULL)
				{
					if(dvbnode->caslot->status == 0 || dvbnode->caslot->status == 100)
						tmpstr = ostrcat(tmpstr, _("empty"), 1, 0);
					if(dvbnode->caslot->status == 1)
						tmpstr = ostrcat(tmpstr, _("initializing"), 1, 0);
					if(dvbnode->caslot->status == 2)
						tmpstr = ostrcat(tmpstr, _("ready"), 1, 0);
				}
				if(dvbnode->caslot->status != 2) allready = 0;
				changetext(tmp, tmpstr);
				free(tmpstr); tmpstr = NULL;
				tmp->height = (listbox->fontsize * 2) + (2 * 2);
				tmp->type = TEXTBOX;
				tmp->handle = (void*)dvbnode;
			}
			tmp = addlistbox(moduleconfig, listbox, tmp, 1);
			if(tmp != NULL)
			{
				tmpstr = ostrcat(tmpstr, "camtype_", 1, 0);
				tmpstr = ostrcat(tmpstr, oitoa(i - 1), 1, 1);
				tmp->type = CHOICEBOX;
				changetext(tmp, _("Module Type"));
				addchoicebox(tmp, "0", _("Single Service"));
				addchoicebox(tmp, "1", _("Multiple Service"));
				setchoiceboxselection(tmp, getconfig(tmpstr, NULL));
				changename(tmp, tmpstr);
				free(tmpstr); tmpstr = NULL;
			}
			tmp = addlistbox(moduleconfig, listbox, tmp, 1);
			if(tmp != NULL)
			{
				tmp->height = 20;
				tmp->deaktivcol = 1;
			}
		}
		dvbnode = dvbnode->next;
	}
	if(allready == 1) reset = 0;


	clearscreen(load);
	drawscreen(moduleconfig, 0, 0);
	if(reset > 0)
	{
		reset--;
		drawscreen(load, 0, 0);
	}
	addscreenrc(moduleconfig, listbox);

	tmp = listbox->select;
	int count = 0;
	while(1)
	{
		if(listbox->select != NULL && (listbox->select->type & CHOICEBOX))
			addscreenrc(moduleconfig, tmp);
		rcret = waitrc(moduleconfig, 2000, 0);
		tmp = listbox->select;
		
		printf("listbox->select->text: %s\n",listbox->select->text);
		printf("listbox->select->name: %s\n",listbox->select->name);

		if(listbox->select != NULL && listbox->select->handle != NULL && ostrstr(listbox->select->text, "Status: empty") == NULL)
//		if(listbox->select != NULL && listbox->select->handle != NULL)
		{
			b3->hidden = NO;
			b4->hidden = NO;
			b5->hidden = NO;
			b6->hidden = NO;
		}
		else
		{
			b3->hidden = YES;
			b4->hidden = YES;
			b5->hidden = YES;
			b6->hidden = YES;
		}
		count++;
		printf("count: %d\n",count);
		printf("reset: %d\n",reset);

		clearscreen(load);
		drawscreen(moduleconfig, 0, 0);
		if(reset > 0) drawscreen(load, 0, 0);

		if(rcret == getrcconfigint("rcexit", NULL)) break;
		if(rcret == getrcconfigint("rcok", NULL))
		{
			tmp = listbox;
			while(tmp != NULL)
			{
				if(ostrncmp("camtype_", tmp->name, 8) == 0)
					addconfigscreencheck(tmp->name, tmp, "0");
				tmp = tmp->next;
			}
			break;
		}
		if(listbox->select != NULL && listbox->select->handle != NULL && rcret == getrcconfigint("rcred", NULL))
		{
			if(((struct dvbdev*)listbox->select->handle)->caslot != NULL)
			{
				((struct dvbdev*)listbox->select->handle)->caslot->status = 100;
				reset = 5;
				drawscreen(load, 0, 0);
			}
		}
		if(listbox->select != NULL && listbox->select->handle != NULL && rcret == getrcconfigint("rcgreen", NULL))
		{
			if((struct dvbdev*)listbox->select->handle != NULL)
			{
				drawscreen(load, 0, 0);
				caappmenu((struct dvbdev*)listbox->select->handle);
				sleep(7);
				reset = 0;
			}
		}
		if(listbox->select != NULL && listbox->select->handle != NULL && rcret == getrcconfigint("rcyellow", NULL))
		{
			if((struct dvbdev*)listbox->select->handle != NULL)
			{
				screencaidlock((struct dvbdev*)listbox->select->handle);
				reset = 0;
			}
		}
		if(listbox->select != NULL && listbox->select->handle != NULL && rcret == getrcconfigint("rcblue", NULL))
		{
			clearscreen(moduleconfig);
			screenchannelslot(((struct dvbdev*)listbox->select->handle)->devnr);
			drawscreen(moduleconfig, 0, 0);
		}
		if(listbox->select != NULL && listbox->select->handle != NULL && rcret == getrcconfigint("rc0", NULL))
		{
			clearscreen(moduleconfig);
			screenchannelslotdel(5);
			drawscreen(moduleconfig, 0, 0);
		}
		if(rcret == getrcconfigint("rcmenu", NULL))
		{
			screenmoduleadjust();
			reset = 0;
			drawscreen(moduleconfig, 0, 0);
		}
		
		if(rcret == RCTIMEOUT) goto start;
	}

	delmarkedscreennodes(moduleconfig, 1);
	delownerrc(moduleconfig);
	clearscreen(load);
	clearscreen(moduleconfig);
}

#endif
