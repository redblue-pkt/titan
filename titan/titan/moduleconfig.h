#ifndef MODULECONFIG_H
#define MODULECONFIG_H

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

				if(checkdoublecaid(dvbnode, ret[i].part) == 1)
				{
					tmpstr = ostrcat(tmpstr, ret[i].part, 1, 0);
					tmpstr = ostrcat(tmpstr, " (", 1, 0);
					tmpstr = ostrcat(tmpstr, _("double"), 1, 0);
					tmpstr = ostrcat(tmpstr, ")", 1, 0);
				}
				else
					tmpstr = ostrcat(tmpstr, ret[i].part, 1, 0);

				changetext(tmp, tmpstr);
				free(tmpstr); tmpstr = NULL;

				addchoicebox(tmp, "0", _("activate"));
				addchoicebox(tmp, "1", _("deaktivate"));
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

	if(checkbox("ATEMIO7600") == 1 || checkbox("ATEMIO510") == 1)
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

	addchoicebox(extmoduldetect, "0", _("yes"));
	addchoicebox(extmoduldetect, "1", _("no"));
	setchoiceboxselection(extmoduldetect, getownconfig("extmoduldetect", NULL));

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

			if(checkbox("ATEMIO7600") == 1 || checkbox("ATEMIO510") == 1)
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
	struct skin* load = getscreen("loading");
	struct skin* tmp = NULL;
	struct dvbdev* dvbnode = NULL;
	char* tmpstr = NULL, *tmpnr = NULL;

	listbox->aktline = 1;
	listbox->aktpage = -1;

	b3->hidden = NO;
	b4->hidden = NO;
	b5->hidden = NO;

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
	while(1)
	{
		if(listbox->select != NULL && (listbox->select->type & CHOICEBOX))
			addscreenrc(moduleconfig, tmp);
		rcret = waitrc(moduleconfig, 2000, 0);
		tmp = listbox->select;

		if(listbox->select != NULL && listbox->select->handle != NULL)
		{
			b3->hidden = NO;
			b4->hidden = NO;
			b5->hidden = NO;
		}
		else
		{
			b3->hidden = YES;
			b4->hidden = YES;
			b5->hidden = YES;
		}
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
		if(rcret == getrcconfigint("rcblue", NULL))
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
