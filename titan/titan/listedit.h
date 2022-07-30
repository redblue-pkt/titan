#ifndef LISTEDIT_H
#define LISTEDIT_H

int screenlistedit(int list, struct channel* chnode, void* aktlist)
{
	int rcret = 0, ret = NOMODE, startchannel = 0;
	struct skin* listedit = getscreen("listedit");
	struct skin* listbox = getscreennode(listedit, "listbox");
	struct skin* cpmode = getscreennode(listedit, "cpmode");
	struct skin* rmmode = getscreennode(listedit, "rmmode");
	struct skin* movemode = getscreennode(listedit, "movemode");
	struct skin* protectmode = getscreennode(listedit, "protectmode");
	struct skin* streammode = getscreennode(listedit, "streammode");
	struct skin* addmode = getscreennode(listedit, "addmode");
	struct skin* editmode = getscreennode(listedit, "editmode");
	struct skin* sortmode = getscreennode(listedit, "sortmode");
	struct skin* setstartchannel = getscreennode(listedit, "setstartchannel");
	struct skin* delstartchannel = getscreennode(listedit, "delstartchannel");
	struct skin* load = getscreen("loading");
	char* newentry = NULL, *tmpstr = NULL, *tmpnr = NULL;

	listbox->aktline = 1;
	listbox->aktpage = -1;

	cpmode->hidden = NO;
	rmmode->hidden = NO;
	movemode->hidden = NO;
	protectmode->hidden = NO;
	streammode->hidden = NO;
	addmode->hidden = YES;
	editmode->hidden = YES;
	sortmode->hidden = YES;
	setstartchannel->hidden = YES;
	delstartchannel->hidden = YES;

	if(list == SATCHANNEL || list == PROVIDERCHANNEL || list == AZCHANNEL)
		movemode->hidden = YES;
	if(list == SATLIST || list == PROVIDERLIST || list == MAINBOUQUETLIST)
	{
		cpmode->hidden = YES;
		protectmode->hidden = YES;
		streammode->hidden = YES;
	}
	if(list == SATLIST || list == PROVIDERLIST)
		cpmode->hidden = NO;
	if(list == MAINBOUQUETLIST)
	{
		addmode->hidden = NO;
		editmode->hidden = NO;
	}

	if(list == PROVIDERLIST)
	{
		sortmode->hidden = NO;
	}

	if(list == ALLCHANNEL || list == SATCHANNEL || list == PROVIDERCHANNEL || list == AZCHANNEL || list == BOUQUETCHANNEL)
	{
		if(chnode != NULL)
		{
			startchannel = 1;
			setstartchannel->hidden = NO;
		}
		delstartchannel->hidden = NO;
		editmode->hidden = NO;
		sortmode->hidden = NO;
	}

	drawscreen(listedit, 0, 0);
	addscreenrc(listedit, listbox);

	while(1)
	{
		rcret = waitrc(listedit, 0, 0);
	
		if(rcret == getrcconfigint("rcexit", NULL)) break;
		if(rcret == getrcconfigint("rcok", NULL) && listbox->select != NULL)
		{
			if(ostrcmp(listbox->select->name, "movemode") == 0)
				ret = MVMODE;
			if(ostrcmp(listbox->select->name, "rmmode") == 0)
				ret = RMMODE;
			if(ostrcmp(listbox->select->name, "cpmode") == 0)
				ret = CPMODE;
			if(ostrcmp(listbox->select->name, "protectmode") == 0)
				ret = PROTECTMODE;
			if(ostrcmp(listbox->select->name, "streammode") == 0)
				ret = STREAMMODE;
			if(ostrcmp(listbox->select->name, "editmode") == 0)
				ret = EDITMODE;
			if(ostrcmp(listbox->select->name, "addmode") == 0)
			{
				newentry = textinput(_("Bouquet Name"), "bouquet");
				if(newentry != NULL)
				{
					newentry = stringreplacechar(newentry, '#', '_');
					tmpstr = ostrcat(newentry, "#", 0, 0);
					tmpnr = oitoa(status.servicetype);
					tmpstr = ostrcat(tmpstr, tmpnr, 1, 0);
					free(tmpnr); tmpnr = NULL;
					tmpstr = ostrcat(tmpstr, "#", 1, 0);
					
					char* dname = realpath(getconfig("bouquetfile", NULL), NULL);
					if(dname != NULL)
					{ 
						dname = dirname(dname);
						tmpstr = ostrcat(tmpstr, dname, 1, 0);
						free(dname); dname = NULL;

						tmpstr = ostrcat(tmpstr, "/", 1, 0);
						tmpstr = ostrcat(tmpstr, "bouquets.", 1, 0);
						tmpstr = ostrcat(tmpstr, newentry, 1, 1);
						addmainbouquet(tmpstr, 1, NULL);
					}
					free(tmpstr); tmpstr = NULL;
				}
			}
			if(startchannel == 1 && ostrcmp(listbox->select->name, "setstartchannel") == 0)
			{
				addconfig("startchannellist", getconfig("channellist", NULL));
				addconfigint("startserviceid", chnode->serviceid);
				addconfigllu("starttransponderid", chnode->transponderid);
				addconfig("startservicetype", getconfig("servicetype", NULL));
			}
			if(startchannel == 1 && ostrcmp(listbox->select->name, "delstartchannel") == 0)
			{
				delconfig("startchannellist");
				delconfig("startserviceid");
				delconfig("starttransponderid");
				delconfig("startservicetype");
			}
			if(ostrcmp(listbox->select->name, "sortmode") == 0)
			{
				drawscreen(load, 0, 0);
				if(list == BOUQUETCHANNEL)
				{
					if(aktlist != NULL)
					{
						sortbouquet(&((struct mainbouquet*)aktlist)->bouquet);
						recalcbouquetnr();
					}
				}
				else if(list == PROVIDERLIST)
					sortprovider();
				else
					sortchannel();
				clearscreen(load);
			}
			break;
		}
	}

	delownerrc(listedit);
	clearscreen(listedit);

	if(ret == NOMODE) status.moveblockcount = 0;
	return ret;
}

#endif
