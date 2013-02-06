#ifndef CHANNELHISTORY_H
#define CHANNELHISTORY_H

void freechannelhistory()
{
	int i = 0;

	for(i = 0; i < MAXCHANNELHISTORY; i++)
	{
		channelhistory[i].chnode = NULL;
		free(channelhistory[i].channellist);
		channelhistory[i].channellist = NULL;
	}
}

void delchannelhistory(struct channel* chnode)
{
	int i = 0;
	if(chnode == NULL) return;

	for(i = 0; i < MAXCHANNELHISTORY; i++)
	{
		if(channelhistory[i].chnode == chnode)		
		{
			channelhistory[i].chnode = NULL;
			free(channelhistory[i].channellist);
			channelhistory[i].channellist = NULL;
		}
	}
}

void addchannelhistory(struct channel* chnode, char* channellist)
{
	int i = 0;
	struct channel* tmpchannel = NULL;
	char* tmpstr = NULL;

	if(chnode == NULL || channellist == NULL) return;

	for(i = 0; i < MAXCHANNELHISTORY - 1; i++)
	{
		if(channelhistory[i].chnode == chnode)		
		{
			tmpchannel = channelhistory[i].chnode;
			tmpstr = channelhistory[i].channellist;
			channelhistory[i].chnode = channelhistory[i + 1].chnode;
			channelhistory[i].channellist = channelhistory[i + 1].channellist;
			channelhistory[i + 1].chnode = tmpchannel;
			channelhistory[i + 1].channellist = tmpstr;
		}
	}

	free(channelhistory[MAXCHANNELHISTORY - 1].channellist);
	channelhistory[MAXCHANNELHISTORY - 1].channellist = NULL;
	for(i = MAXCHANNELHISTORY - 1; i > 0 ; i--)
	{
		channelhistory[i].chnode = channelhistory[i - 1].chnode;
		channelhistory[i].channellist = channelhistory[i - 1].channellist;
	}

	channelhistory[0].chnode = chnode;
	channelhistory[0].channellist = ostrcat(channellist, NULL, 0, 0);
}

void screenchannelhistory()
{
	int rcret = -1, i = 0;
	struct skin* chistory = getscreen("channelhistory");
	struct skin* listbox = getscreennode(chistory, "listbox");
	struct skin* tmp = NULL;
	struct epg* epgnode = NULL;
	char* tmpstr = NULL;

	listbox->aktline = 1;
	listbox->aktpage = -1;

	for(i = 1; i < MAXCHANNELHISTORY; i++)
	{
		if(channelhistory[i].chnode != NULL && channelhistory[i].channellist != NULL)
		{
			tmp = addlistbox(chistory, listbox, tmp, 1);
			if(tmp != NULL)
			{
				changetext(tmp, channelhistory[i].chnode->name);

				epgnode = getepgakt(channelhistory[i].chnode);
				if(epgnode != NULL)
				{
					tmpstr = ostrcat(tmpstr, " - ", 1, 0);
					tmpstr = ostrcat(tmpstr, epgnode->title, 1, 0);
					changetext2(tmp, tmpstr);
					free(tmpstr); tmpstr = NULL;
				}

				tmp->handle = (char*)channelhistory[i].chnode;
				tmp->handle1 = channelhistory[i].channellist;
			}
		}
	}

	addscreenrc(chistory, listbox);
	drawscreen(chistory, 0, 0);

	while(1)
	{
		rcret = waitrc(chistory, 0, 0);

		if(rcret == getrcconfigint("rcexit", NULL)) break;
		if(rcret == getrcconfigint("rcok", NULL) || rcret == getrcconfigint("rcrecall", NULL))
		{
			if(listbox->select != NULL && listbox->select->handle != NULL && listbox->select->handle1 != NULL)
				servicecheckret(servicestart((struct channel*)listbox->select->handle, listbox->select->handle1, NULL, 0), 0);

			break;
		}
	}

	delmarkedscreennodes(chistory, 1);
	delownerrc(chistory);
	clearscreen(chistory);
	drawscreen(skin, 0, 0);
}

#endif
