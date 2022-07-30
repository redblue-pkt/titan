#ifndef CHANNELLIST_H
#define CHANNELLIST_H

int selectchannel(struct skin* listbox, int sid, uint64_t tid)
{
	struct skin* node = listbox;
	struct channel* chnode = NULL;
	listbox->aktpage = -1;
	listbox->aktline = 1;

	listbox->select = NULL;

	if(status.servicetype == 0)
	{
		if(sid != 0 && tid != 0)
			chnode = getchannel(sid, tid);
		else
			chnode = getchannel(getconfigint("serviceid", NULL), getconfigllu("transponderid", NULL));
	}
	else
	{
		if(sid != 0 && tid != 0)
			chnode = getchannel(sid, tid);
		else
			chnode = getchannel(getconfigint("rserviceid", NULL), getconfigllu("rtransponderid", NULL));
	}

	if(chnode == NULL)
	{
		debug(1000, "out -> NULL detect");
		return 1;
	}

	while(node != NULL)
	{
		if(node->deaktivcol > -1 || node->hidden == YES)
		{
			node = node->next;
			continue;
		}
		if(chnode == (struct channel*) node->handle)
			return 0;
		if(node->del == 1) listbox->aktline++;
		node = node->next;
	}
	listbox->aktline = 1;
	return 1;
}

void changechannelepg(struct channel* chnode, struct skin* node)
{
	struct epg* epgnode = NULL;
	char* tmpstr = NULL;

	epgnode = getepgakt(chnode);
	if(epgnode != NULL)
	{
		tmpstr = ostrcat(tmpstr, " - ", 1, 0);
		tmpstr = ostrcat(tmpstr, epgnode->title, 1, 0);
		changetext2(node, tmpstr);
		free(tmpstr); tmpstr = NULL;
	}
}

void showallchannel(struct skin* channellist, struct skin* listbox, struct skin* channeltimeline, int mode)
{
	struct skin* chnode = NULL;
	struct channel* tmpchannel = channel;
	long long deaktivcol = convertcol("deaktivcol");

	while(tmpchannel != NULL)
	{
		if(tmpchannel->servicetype == status.servicetype)
		{
			chnode = addlistbox(channellist, listbox, chnode, 1);
			if(chnode != NULL)
			{
				changechannelepg(tmpchannel, chnode);
				if(checkbouquet(tmpchannel) == 1)
					chnode->fontcol = convertcol("favcol");
				if(tmpchannel->protect > 0)
					chnode->fontcol = convertcol("protectcol");
				changetext(chnode, tmpchannel->name);
				chnode->handle = (char*) tmpchannel;
				chnode->handle1 = (char*) tmpchannel;
				if(mode == 0 && channelnottunable(tmpchannel) == 1)
				{
					if(status.channellistview == 0)
						chnode->hidden = YES;
					else
						chnode->deaktivcol = deaktivcol;
				}
				if(status.showchanneltimeline == 1 && channeltimeline != status.skinerr)
				{
					chnode->textposx = channeltimeline->width + 10;
					struct skin* chnode1 = NULL;
					chnode1 = addlistbox(channellist, chnode, chnode, 1);
					if(chnode1 != NULL)
					{
						chnode1->progresscol = channeltimeline->progresscol;
						chnode1->skinfunc = convertfunc("getepgchanneltimeline", &chnode1->funcrettype);
						chnode1->handle = (char*) tmpchannel;
						chnode1->posy = channeltimeline->posy;
						chnode1->width = channeltimeline->width;
						chnode1->height = channeltimeline->height;
						chnode1->bordersize = channeltimeline->bordersize;
						chnode1->bordercol = channeltimeline->bordercol;
						chnode1->prozwidth = 0;
					}
				}
			}
		}
		tmpchannel = tmpchannel->next;
	}
}

int checkbouquet(struct channel* tmpchannel)
{
	struct mainbouquet* mainbouquetnode = mainbouquet;
	struct bouquet* bouquetnode = NULL;
	while(mainbouquetnode != NULL)
	{
		bouquetnode = mainbouquetnode->bouquet;
		while(bouquetnode != NULL)
		{
			if(bouquetnode->channel != NULL && bouquetnode->channel->serviceid == tmpchannel->serviceid && bouquetnode->channel->transponderid == tmpchannel->transponderid)
				return 1;

			bouquetnode = bouquetnode->next;
		}
		mainbouquetnode = mainbouquetnode->next;
	}
	return 0;	
}

void showbouquetchannel(struct skin* channellist, struct skin* listbox, struct skin* channeltimeline, struct bouquet* firstbouquet, struct mainbouquet* mbouquet, int mode)
{
	struct skin* chnode = NULL;
	struct bouquet* tmpbouquet = firstbouquet;
	char* tmpstr = NULL, *tmpnr = NULL;
	long long deaktivcol = convertcol("deaktivcol");

	if(mbouquet != NULL && ostrcmp(mbouquet->filename, "0") == 0)
	{
		mostzaptobouquet(mbouquet);
		tmpbouquet = mbouquet->bouquet;
	}

	while(tmpbouquet != NULL)
	{
		if(tmpbouquet->channel != NULL)
		{
			if(tmpbouquet->channel->servicetype == status.servicetype)
			{
				chnode = addlistbox(channellist, listbox, chnode, 1);
				if(chnode != NULL)
				{
					tmpnr = oitoa(tmpbouquet->nr);
					changeret(chnode, tmpnr);
					tmpstr = ostrcat(tmpstr, tmpnr, 1, 1);
					tmpstr = ostrcat(tmpstr, "  ", 1, 0);
					tmpstr = ostrcat(tmpstr, tmpbouquet->channel->name, 1, 0);
					changetext(chnode, tmpstr);
					free(tmpstr); tmpstr = NULL;
					if(tmpbouquet->channel->protect > 0)
						chnode->fontcol = convertcol("protectcol");

					changechannelepg(tmpbouquet->channel, chnode);
					chnode->handle = (char*) tmpbouquet->channel;
					chnode->handle1 = (char*) tmpbouquet;
					if(mode == 0 && channelnottunable(tmpbouquet->channel) == 1)
					{
						if(status.channellistview == 0)
							chnode->hidden = YES;
						else
							chnode->deaktivcol = deaktivcol;
					}
					if(status.showchanneltimeline == 1 && channeltimeline != status.skinerr)
					{
						chnode->textposx = channeltimeline->width + 10;
						struct skin* chnode1 = NULL;
						chnode1 = addlistbox(channellist, chnode, chnode, 1);
						if(chnode1 != NULL)
						{
							chnode1->progresscol = channeltimeline->progresscol;
							chnode1->skinfunc = convertfunc("getepgchanneltimeline", &chnode1->funcrettype);
							chnode1->handle = (char*)tmpbouquet->channel;
							chnode1->posy = channeltimeline->posy;
							chnode1->width = channeltimeline->width;
							chnode1->height = channeltimeline->height;
							chnode1->bordersize = channeltimeline->bordersize;
							chnode1->bordercol = channeltimeline->bordercol;
							chnode1->prozwidth = 0;
						}
					}
				}
			}
		}
		tmpbouquet = tmpbouquet->next;
	}
}

void showproviderchannel(struct skin* channellist, struct skin* listbox, struct skin* channeltimeline, struct provider* providernode, int mode)
{
	struct skin* chnode = NULL;
	struct channel* tmpchannel = channel;
	long long deaktivcol = convertcol("deaktivcol");

	while(tmpchannel != NULL)
	{
		if(tmpchannel->provider == providernode)
		{
			if(tmpchannel->servicetype == status.servicetype)
			{
				chnode = addlistbox(channellist, listbox, chnode, 1);
				if(chnode != NULL)
				{
					changechannelepg(tmpchannel, chnode);
					changetext(chnode, tmpchannel->name);
					if(tmpchannel->protect > 0)
						chnode->fontcol = convertcol("protectcol");
					chnode->handle = (char*) tmpchannel;
					chnode->handle1 = (char*) tmpchannel;
					if(mode == 0 && channelnottunable(tmpchannel) == 1)
					{
						if(status.channellistview == 0)
							chnode->hidden = YES;
						else
							chnode->deaktivcol = deaktivcol;
					}
					if(status.showchanneltimeline == 1 && channeltimeline != status.skinerr)
					{
						chnode->textposx = channeltimeline->width + 10;
						struct skin* chnode1 = NULL;
						if(chnode1 != NULL)
						{
							chnode1 = addlistbox(channellist, chnode, chnode, 1);
							chnode1->progresscol = channeltimeline->progresscol;
							chnode1->skinfunc = convertfunc("getepgchanneltimeline", &chnode1->funcrettype);
							chnode1->handle = (char*) tmpchannel;
							chnode1->posy = channeltimeline->posy;
							chnode1->width = channeltimeline->width;
							chnode1->height = channeltimeline->height;
							chnode1->bordersize = channeltimeline->bordersize;
							chnode1->bordercol = channeltimeline->bordercol;
							chnode1->prozwidth = 0;
						}
					}
				}
			}
		}
		tmpchannel = tmpchannel->next;
	}
}

int checkprovider(struct provider* providernode)
{
	struct channel* tmpchannel = channel;

	while(tmpchannel != NULL)
	{
		if(tmpchannel->provider == providernode)
		{
			if(tmpchannel->servicetype == status.servicetype && channelnottunable(tmpchannel) != 1)
				return 1;
		}
		tmpchannel = tmpchannel->next;
	}
	return 0;
}

void showprovider(struct skin* channellist, struct skin* listbox)
{
	struct provider *node = provider;
	struct skin* providernode = NULL;

	while(node != NULL)
	{
		if(checkprovider(node) == 1)
		{
			node->flag = 1;
			providernode = addlistbox(channellist, listbox, providernode, 2);
			if(providernode != NULL)
			{
				changetext(providernode, node->name);
				changename(providernode, node->name);
				providernode->handle = (char*) node;
				providernode->handle1 = (char*) node;
			}
		}
		else
			node->flag = 0;

		node = node->next;
	}
}

void showsatchannel(struct skin* channellist, struct skin* listbox, struct skin* channeltimeline, struct sat* satnode, int mode)
{
	struct skin* chnode = NULL;
	struct channel* tmpchannel = channel;
	long long deaktivcol = convertcol("deaktivcol");

	if(satnode == NULL)
		return;

	while(tmpchannel != NULL)
	{
		if(tmpchannel->transponder != NULL && tmpchannel->transponder->orbitalpos == satnode->orbitalpos)
		{
			if(tmpchannel->servicetype == status.servicetype)
			{
				chnode = addlistbox(channellist, listbox, chnode, 1);
				if(chnode != NULL)
				{
					changechannelepg(tmpchannel, chnode);
					changetext(chnode, tmpchannel->name);
					if(tmpchannel->protect > 0)
						chnode->fontcol = convertcol("protectcol");
					chnode->handle = (char*) tmpchannel;
					chnode->handle1 = (char*) tmpchannel;
					if(mode == 0 && channelnottunable(tmpchannel) == 1)
					{
						if(status.channellistview == 0)
							chnode->hidden = YES;
						else
							chnode->deaktivcol = deaktivcol;
					}
					if(status.showchanneltimeline == 1 && channeltimeline != status.skinerr)
					{
						chnode->textposx = channeltimeline->width + 10;
						struct skin* chnode1 = NULL;
						if(chnode1 != NULL)
						{
							chnode1 = addlistbox(channellist, chnode, chnode, 1);
							chnode1->progresscol = channeltimeline->progresscol;
							chnode1->skinfunc = convertfunc("getepgchanneltimeline", &chnode1->funcrettype);
							chnode1->handle = (char*) tmpchannel;
							chnode1->posy = channeltimeline->posy;
							chnode1->width = channeltimeline->width;
							chnode1->height = channeltimeline->height;
							chnode1->bordersize = channeltimeline->bordersize;
							chnode1->bordercol = channeltimeline->bordercol;
							chnode1->prozwidth = 0;
						}
					}
				}
			}
		}
		tmpchannel = tmpchannel->next;
	}
}

void showsat(struct skin* channellist, struct skin* listbox)
{
	int i = 0, treffer = 0;
	struct sat *node = sat;
	struct dvbdev *dvbnode = NULL;
	struct skin* satnode = NULL;
	char* tmpstr = NULL, *tmpnr = NULL;

	while(node != NULL)
	{
		treffer = 0;
		dvbnode = dvbdev;
		while(dvbnode != NULL)
		{
			if(dvbnode->type == FRONTENDDEV && dvbnode->feshortname != NULL)
			{
				tmpstr = ostrcat(dvbnode->feshortname, "_sat", 0, 0);
				for(i = 1; i <= getmaxsat(dvbnode->feshortname); i++)
				{
					tmpnr = oitoa(i);
					if(getconfigint(tmpstr, tmpnr) == node->orbitalpos)
					{
						free(tmpnr); tmpnr = NULL;
						treffer = 1;
						break;
					}
					free(tmpnr); tmpnr = NULL;
				}
				free(tmpstr); tmpstr = NULL;
			}
			if(treffer == 1) break;
			dvbnode = dvbnode->next;
		}

		if(treffer == 0) 
		{
			node = node->next;
			continue;
		}

		satnode = addlistbox(channellist, listbox, satnode, 2);
		if(satnode != NULL)
		{
			changetext(satnode, node->name);
			changename(satnode, node->name);
			satnode->handle = (char*) node;
			satnode->handle1 = (char*) node;
		}
		node = node->next;
	}
}

void showazchannel(struct skin* channellist, struct skin* listbox, struct skin* channeltimeline, int character, int mode)
{
	struct skin* chnode = NULL;
	struct channel* tmpchannel = channel;
	long long deaktivcol = convertcol("deaktivcol");

	while(tmpchannel != NULL)
	{
		if(tmpchannel->name != NULL && (tmpchannel->name[0] == character || tmpchannel->name[0] == character + 32))
		{
			if(tmpchannel->servicetype == status.servicetype)
			{
				chnode = addlistbox(channellist, listbox, chnode, 1);
				if(chnode != NULL)
				{
					changechannelepg(tmpchannel, chnode);
					changetext(chnode, tmpchannel->name);
					if(tmpchannel->protect > 0)
						chnode->fontcol = convertcol("protectcol");
					chnode->handle = (char*) tmpchannel;
					chnode->handle1 = (char*) tmpchannel;
					if(mode == 0 && channelnottunable(tmpchannel) == 1)
					{
						if(status.channellistview == 0)
							chnode->hidden = YES;
						else
							chnode->deaktivcol = deaktivcol;
					}
					if(status.showchanneltimeline == 1 && channeltimeline != status.skinerr)
					{
						chnode->textposx = channeltimeline->width + 10;
						struct skin* chnode1 = NULL;
						if(chnode1 != NULL)
						{
							chnode1 = addlistbox(channellist, chnode, chnode, 1);
							chnode1->progresscol = channeltimeline->progresscol;
							chnode1->skinfunc = convertfunc("getepgchanneltimeline", &chnode1->funcrettype);
							chnode1->handle = (char*) tmpchannel;
							chnode1->posy = channeltimeline->posy;
							chnode1->width = channeltimeline->width;
							chnode1->height = channeltimeline->height;
							chnode1->bordersize = channeltimeline->bordersize;
							chnode1->bordercol = channeltimeline->bordercol;
							chnode1->prozwidth = 0;
						}
					}
				}
			}
		}
		tmpchannel = tmpchannel->next;
	}
}

void showaz(struct skin* channellist, struct skin* listbox)
{
	struct skin* node = NULL;
	int i;
	char* tmpstr = NULL;

	tmpstr = malloc(2);
	if(tmpstr == NULL)
	{
		err("no memory");
		return;
	}

	for(i = 65; i < 91; i++)
	{
		node = addlistbox(channellist, listbox, node, 2);
		if(node != NULL)
		{
			snprintf(tmpstr, 2, "%c", i);
			changetext(node, tmpstr);
			changename(node, tmpstr);
			node->handle = (void*) i;
		}
	}

	free(tmpstr);
}

void showmainbouquet(struct skin* channellist, struct skin* listbox)
{
	struct mainbouquet *node = mainbouquet;
	struct skin* bouquetnode = NULL;

	while(node != NULL)
	{
		if(node->type == status.servicetype)
		{
			bouquetnode = addlistbox(channellist, listbox, bouquetnode, 2);
			if(bouquetnode != NULL)
			{
				changetext(bouquetnode, node->name);
				changename(bouquetnode, node->name);
				bouquetnode->handle = (char*) node->bouquet;
				bouquetnode->handle1 = (char*) node;
			}
		}
		node = node->next;
	}
}

void drawchannellist(struct skin* channellist, int list, struct skin* listbox)
{
	status.markedchannel = NULL;
	if(list == ALLCHANNEL || list == SATCHANNEL || list == PROVIDERCHANNEL || list == AZCHANNEL || list == BOUQUETCHANNEL)
	{
		if(listbox->select == NULL)
			drawscreen(channellist, 2, 0);

		if(listbox->select != NULL)
			status.markedchannel = (struct channel*)listbox->select->handle;
	}
	drawscreen(channellist, 0, 0);
}

void recalclist(int list, void* aktlist, int listmode, struct skin* channellist, struct skin* listbox, struct skin* channeltimeline)
{
	if(list == ALLCHANNEL)
		showallchannel(channellist, listbox, channeltimeline, listmode);
	if(list == SATCHANNEL)
		showsatchannel(channellist, listbox, channeltimeline, (struct sat*)aktlist, listmode);
	if(list == PROVIDERCHANNEL)
		showproviderchannel(channellist, listbox, channeltimeline, (struct provider*)aktlist, listmode);
	if(list == AZCHANNEL)
		showazchannel(channellist, listbox, channeltimeline, (int)aktlist, listmode);
	if(list == BOUQUETCHANNEL)
		showbouquetchannel(channellist, listbox, channeltimeline, ((struct mainbouquet*)aktlist)->bouquet, (struct mainbouquet*)aktlist, listmode);
	if(list == MAINBOUQUETLIST)
		showmainbouquet(channellist, listbox);
	if(list == SATLIST)
		showsat(channellist, listbox);
	if(list == PROVIDERLIST)
		showprovider(channellist, listbox);
}

void changebutton(int listmode, struct skin* b1, struct skin* b2, struct skin* b3, struct skin* b4, struct skin* b5, struct skin* b6, struct skin* b7, struct skin* b8, struct skin* b9, struct skin* b10, struct skin* b11, struct skin* b12, struct skin* b13, struct skin* b14, struct skin* b15, int flag)
{
	if(listmode == NOMODE)
	{
		b1->hidden = NO;
		b2->hidden = NO;
		b3->hidden = NO;
		b4->hidden = NO;
		b5->hidden = NO;
		b6->hidden = NO;
		b7->hidden = NO;
		b8->hidden = NO;
		b9->hidden = NO;
		b10->hidden = NO;
		b11->hidden = YES;
		b12->hidden = YES;
		b13->hidden = YES;
		b14->hidden = YES;
		b15->hidden = NO;
	}
	else if(listmode == MVMODE)
	{
		b1->hidden = NO;
		b2->hidden = NO;
		b3->hidden = YES;
		b4->hidden = YES;
		b5->hidden = YES;
		b6->hidden = YES;
		b7->hidden = YES;
		b8->hidden = YES;
		b9->hidden = YES;
		b10->hidden = YES;
		b11->hidden = NO;
		b12->hidden = NO;
		b13->hidden = YES;
		b14->hidden = NO;
		b15->hidden = YES;
	}
	else
	{
		b1->hidden = NO;
		b2->hidden = NO;
		b3->hidden = YES;
		b4->hidden = YES;
		b5->hidden = YES;
		b6->hidden = YES;
		b7->hidden = YES;
		b8->hidden = YES;
		b9->hidden = YES;
		b10->hidden = YES;
		b11->hidden = NO;
		b12->hidden = NO;
		b13->hidden = NO;
		b14->hidden = YES;
		b15->hidden = YES;
	}
	
	if(flag == 1)
	{
		b8->hidden = YES;
		b9->hidden = YES;
		b10->hidden = YES;
		b11->hidden = YES;
		b15->hidden = YES;	
	}
}

void changechanneltitle(struct skin* channellist, struct skin* listbox, int listmode, char** oldtitle, long* oldfontcol, long* oldbgcol)
{
	struct skin* titletext = getscreennode(channellist, "titletext");
	char* tmpstr = NULL;

	if(*oldtitle == NULL)
		*oldtitle = ostrcat(channellist->title, NULL, 0, 0);
	if(*oldtitle == NULL)
		*oldtitle = ostrcat(titletext->text, NULL, 0, 0);

	if(listmode == MVMODE)
	{
		tmpstr = ostrcat(*oldtitle, " - ", 0, 0);
		tmpstr = ostrcat(tmpstr, _("Move mode"), 1, 0);
		listbox->bgcol = convertcol("mvmode_bgcol");
		listbox->fontcol = convertcol("mvmode_fontcol");
		changetitle(channellist, tmpstr);
	}
	else if(listmode == RMMODE)
	{
		tmpstr = ostrcat(*oldtitle, " - ", 0, 0);
		tmpstr = ostrcat(tmpstr, _("Remove mode"), 1, 0);
		listbox->bgcol = convertcol("rmmode_bgcol");
		listbox->fontcol = convertcol("rmmode_fontcol");
		changetitle(channellist, tmpstr);
	}
	else if(listmode == CPMODE)
	{
		tmpstr = ostrcat(*oldtitle, " - ", 0, 0);
		tmpstr = ostrcat(tmpstr, _("Copy mode"), 1, 0);
		listbox->bgcol = convertcol("cpmode_bgcol");
		listbox->fontcol = convertcol("cpmode_fontcol");
		changetitle(channellist, tmpstr);
	}
	else if(listmode == PROTECTMODE)
	{
		tmpstr = ostrcat(*oldtitle, " - ", 0, 0);
		tmpstr = ostrcat(tmpstr, _("Protect mode"), 1, 0);
		listbox->bgcol = convertcol("protectmode_bgcol");
		listbox->fontcol = convertcol("protectmode_fontcol");
		changetitle(channellist, tmpstr);
	}
	else if(listmode == EDITMODE)
	{
		tmpstr = ostrcat(*oldtitle, " - ", 0, 0);
		tmpstr = ostrcat(tmpstr, _("Edit mode"), 1, 0);
		listbox->bgcol = convertcol("editmode_bgcol");
		listbox->fontcol = convertcol("editmode_fontcol");
		changetitle(channellist, tmpstr);
	}
	else if(listmode == STREAMMODE)
	{
		tmpstr = ostrcat(*oldtitle, " - ", 0, 0);
		tmpstr = ostrcat(tmpstr, _("Stream mode"), 1, 0);
		listbox->bgcol = convertcol("protectmode_bgcol");
		listbox->fontcol = convertcol("protectmode_fontcol");
		changetitle(channellist, tmpstr);
	}
	else
	{	
		changetitle(channellist, *oldtitle);
		*oldtitle = NULL;
		listbox->fontcol = *oldfontcol;
		listbox->bgcol = *oldbgcol;
	}
}

void addscreenrcchannellist(struct skin* channellist, struct skin* listbox)
{
	addscreenrc(channellist, listbox);

	//deaktivate for blockmove
	delrc(getrcconfigint("rcchup", NULL), channellist, listbox);
	delrc(getrcconfigint("rcchdown", NULL), channellist, listbox);
}

//flag 1: called from recordtimer screen
//flag 2: rcfav (open bouquetlist)
//flag 3: edit modus
int screenchannellist(struct channel** retchannel, char** retchannellist, int flag)
{
	char* channellist_sel = getskinconfig("channellist_selection", NULL);
	debug(10, "get initial channellist=%s", channellist_sel);

	if(channellist_sel == NULL) {
		channellist_sel = ostrcat(channellist_sel, "channellist", 1, 0); // fallback to default
		debug(10, "fallback to default, set channellist=%s", channellist_sel);
	}

	struct skin* channellist = getscreen(channellist_sel);
	debug(10, "final channellist=%s", channellist_sel);

//	struct skin* channellist = getscreen("channellist");
	struct skin* listbox = getscreennode(channellist, "listbox");
	struct skin* channeltimeline = getscreennode(channellist, "channeltimeline");
	struct skin* b1 = getscreennode(channellist, "b1");
	struct skin* b2 = getscreennode(channellist, "b2");
	struct skin* b3 = getscreennode(channellist, "b3");
	struct skin* b4 = getscreennode(channellist, "b4");
	struct skin* b5 = getscreennode(channellist, "b5");
	struct skin* b6 = getscreennode(channellist, "b6");
	struct skin* b7 = getscreennode(channellist, "b7");
	struct skin* b8 = getscreennode(channellist, "b8");
	struct skin* b9 = getscreennode(channellist, "b9");
	struct skin* b10 = getscreennode(channellist, "b10");
	struct skin* b11 = getscreennode(channellist, "b11");
	struct skin* b12 = getscreennode(channellist, "b12");
	struct skin* b13 = getscreennode(channellist, "b13");
	struct skin* b14 = getscreennode(channellist, "b14");
	struct skin* b15 = getscreennode(channellist, "b15");
	struct skin* tmpskin;
	int rcret, ret, listmode, newmodus, list;
	char* tmpstr = NULL, *tmpstr1 = NULL, *oldtitle = NULL;
	void* movesel = NULL, *aktlist = NULL;
	int nochanneltitle = getskinconfigint("nochanneltitle", NULL);
	int firstdraw = 0;
	long oldfontcol = 0;
	long oldbgcol = 0;
	
	status.channelswitch = 1;

start:
	rcret = 0, ret = -1, list = ALLCHANNEL, listmode = NOMODE, newmodus = 0;
	tmpstr = NULL, tmpstr1 = NULL, movesel = NULL, aktlist = NULL, tmpskin = NULL;
	if(nochanneltitle == 0) 
	{
		changetitle(channellist, "");
	}

	oldbgcol = listbox->bgcol;
	oldfontcol = listbox->fontcol;

	if(status.servicetype == 0)
	{
		tmpstr = getconfig("channellist", NULL);
		changetext(b8, _("Radio"));
	}
	else
	{
		tmpstr = getconfig("rchannellist", NULL);
		changetext(b8, _("TV"));
	}
	if(ostrncmp("(BOUQUET)-", tmpstr, 10) == 0 && strlen(tmpstr) > 10)
	{
		struct mainbouquet* mainbouquetnode = NULL;
		mainbouquetnode = getmainbouquet(tmpstr + 10);
		if(mainbouquetnode != NULL && mainbouquetnode->bouquet != NULL)
		{
			tmpstr1 = ostrcat(tmpstr1, _("Bouquets"), 0, 0);
			tmpstr1 = ostrcat(tmpstr1, " - ", 1, 0);
			tmpstr1 = ostrcat(tmpstr1, tmpstr + 10, 1, 0);
			if(nochanneltitle == 0)
			{
				changetitle(channellist, tmpstr1);
			}
			free(tmpstr1); tmpstr1 = NULL;
			list = BOUQUETCHANNEL;
			aktlist = (void*)mainbouquetnode;
			showbouquetchannel(channellist, listbox, channeltimeline, mainbouquetnode->bouquet, mainbouquetnode, flag);
			selectchannel(listbox, 0, 0);
		}
	}
	else if(ostrncmp("(A-Z)-", tmpstr, 6) == 0 && strlen(tmpstr) > 6)
	{
		tmpstr1 = ostrcat(tmpstr1, _("Channel"), 0, 0);
		tmpstr1 = ostrcat(tmpstr1, " - ", 1, 0);
		tmpstr1 = ostrcat(tmpstr1, tmpstr + 6, 1, 0);
		if(nochanneltitle == 0)
		{
			changetitle(channellist, tmpstr1);
		}
		free(tmpstr1); tmpstr1 = NULL;
		list = AZCHANNEL;
		aktlist = (void*)(int)tmpstr[6];
		showazchannel(channellist, listbox, channeltimeline, (int)tmpstr[6], flag);
		selectchannel(listbox, 0, 0);
	}
	else if(ostrncmp("(SAT)-", tmpstr, 6) == 0 && strlen(tmpstr) > 6)
	{
		tmpstr1 = ostrcat(tmpstr1, _("Satellites"), 0, 0);
		tmpstr1 = ostrcat(tmpstr1, " - ", 1, 0);
		tmpstr1 = ostrcat(tmpstr1, tmpstr + 6, 1, 0);
		if(nochanneltitle == 0)
		{
			changetitle(channellist, tmpstr1);
		}
		free(tmpstr1); tmpstr1 = NULL;
		struct sat* satnode = getsat(tmpstr + 6);
		list = SATCHANNEL;
		aktlist = (void*)satnode;
		showsatchannel(channellist, listbox, channeltimeline, satnode, flag);
		selectchannel(listbox, 0, 0);
	}
	else if(ostrncmp("(PROVIDER)-", tmpstr, 11) == 0 && strlen(tmpstr) > 6)
	{
		tmpstr1 = ostrcat(tmpstr1, _("Provider"), 0, 0);
		tmpstr1 = ostrcat(tmpstr1, " - ", 1, 0);
		tmpstr1 = ostrcat(tmpstr1, tmpstr + 11, 1, 0);
		if(nochanneltitle == 0)
		{
			changetitle(channellist, tmpstr1);
		}
		free(tmpstr1); tmpstr1 = NULL;
		struct provider* providernode = getproviderbyname(tmpstr + 11);
		list = PROVIDERCHANNEL;
		aktlist = (void*)providernode;
		showproviderchannel(channellist, listbox, channeltimeline, providernode, flag);
		selectchannel(listbox, 0, 0);
	}
	else
	{
		if(nochanneltitle == 0)
		{
			changetitle(channellist, _("All Channels"));
		}
		list = ALLCHANNEL;
		showallchannel(channellist, listbox, channeltimeline, flag);
		selectchannel(listbox, 0, 0);
	}

	tmpstr = NULL;
	if(status.screenanim > 0 && firstdraw == 0)
	{
		firstdraw = 1;
		drawscreen(channellist, 0, 4);
	}
	else
		drawscreen(channellist, 2, 0);

	addscreenrcchannellist(channellist, listbox);

	if(flag == 3)
	{
		if(listbox->select != NULL)
		{
			resettvpic();
			listmode = screenlistedit(list, (struct channel*)listbox->select->handle, aktlist);
		}

		if(listmode == NOMODE) goto end;
		if(nochanneltitle == 0) changechanneltitle(channellist, listbox, listmode, &oldtitle, &oldfontcol, &oldbgcol);
	}
	if(flag != 2)
	{
		changebutton(listmode, b1, b2, b3, b4, b5, b6, b7, b8, b9, b10, b11, b12, b13, b14, b15, flag);
		drawchannellist(channellist, list, listbox);
	}
	
	status.updatevfd = PAUSE;
	
	while(1)
	{
		if(status.markedchannel != NULL && (checkchipset("BCM7424") == 1 || checkbox("DM900") == 1 || checkbox("DM920") == 1 || checkbox("DM520") == 1 || checkbox("DM525") == 1 || checkbox("DM7020HD") == 1 || checkbox("DM7020HDV2") == 1))
			writevfdmenu(status.markedchannel->name);
		if(flag == 2)
		{
			rcret = getrcconfigint("rcblue", NULL);
			flag = 0;
		}
		else
			rcret = waitrc(channellist, 10000, 2);

		//read epg new
		if(listbox != NULL && listbox->select != NULL && (list == ALLCHANNEL || list == SATCHANNEL || list == PROVIDERCHANNEL || list == AZCHANNEL || list == BOUQUETCHANNEL))
		{
			tmpskin = listbox->select;
			while(tmpskin != NULL)
			{
				if(tmpskin->pagecount != listbox->aktpage) break;
				changechannelepg((struct channel*)tmpskin->handle, tmpskin);
				tmpskin = tmpskin->prev;
			}
			tmpskin = listbox->select;
			while(tmpskin != NULL)
			{
				if(tmpskin->pagecount != listbox->aktpage) break;
				changechannelepg((struct channel*)tmpskin->handle, tmpskin);
				tmpskin = tmpskin->next;
			}
		}

		if(rcret == getrcconfigint("rcup", NULL) || rcret == getrcconfigint("rcdown", NULL) || rcret == getrcconfigint("rcleft", NULL) || rcret == getrcconfigint("rcright", NULL))
		{
			if(listmode != MVMODE || (listmode == MVMODE && status.markmodus == 0))
			{
				changebutton(listmode, b1, b2, b3, b4, b5, b6, b7, b8, b9, b10, b11, b12, b13, b14, b15, flag);
				drawchannellist(channellist, list, listbox);
			}
		}

		if(rcret == RCTIMEOUT)
		{
			changebutton(listmode, b1, b2, b3, b4, b5, b6, b7, b8, b9, b10, b11, b12, b13, b14, b15, flag);
			drawscreen(channellist, 0, 0);
		}

		if(rcret == getrcconfigint("rcexit", NULL)) break;

		if((flag == 0 || flag == 3) && listmode > NOMODE)
		{
			if(rcret == getrcconfigint("rcmenu", NULL))
			{
				int sid = 0;
				uint64_t tid = 0;
				if((list == ALLCHANNEL || list == SATCHANNEL || list == PROVIDERCHANNEL || list == AZCHANNEL || list == BOUQUETCHANNEL) && listbox->select != NULL && listbox->select->handle != NULL)
				{
					sid = ((struct channel*)listbox->select->handle)->serviceid;
					tid = ((struct channel*)listbox->select->handle)->transponderid;
				}

				status.markmodus = 0;
				movesel = NULL;
				clearscreen(channellist);
				resettvpic();

				listmode = screenlistedit(list, NULL, aktlist);
				if(listmode == MVMODE)
				{
					delrc(getrcconfigint("rcright", NULL), channellist, listbox);
					delrc(getrcconfigint("rcleft", NULL), channellist, listbox);
				}
				else
					addscreenrcchannellist(channellist, listbox);
				if(listmode == NOMODE && flag == 3) flag = 0;

				if(nochanneltitle == 0) changechanneltitle(channellist, listbox, listmode, &oldtitle, &oldfontcol, &oldbgcol);

				delmarkedscreennodes(channellist, 1);
				delmarkedscreennodes(channellist, 2);
				recalclist(list, aktlist, listmode, channellist, listbox, channeltimeline);
				selectchannel(listbox, sid, tid);
				changebutton(listmode, b1, b2, b3, b4, b5, b6, b7, b8, b9, b10, b11, b12, b13, b14, b15, flag);
				drawscreen(channellist, 0, 0);
			}
			if(listmode == PROTECTMODE && listbox->select != NULL && listbox->select->handle != NULL && rcret == getrcconfigint("rcok", NULL))
			{
				if((list == ALLCHANNEL || list == SATCHANNEL || list == PROVIDERCHANNEL || list == AZCHANNEL || list == BOUQUETCHANNEL) && listbox->select->handle != NULL)
				{
					if(((struct channel*)listbox->select->handle)->protect == 0)
						((struct channel*)listbox->select->handle)->protect = 1;
					else
					{
						if(screenpincheck(1, NULL) == 0)
							((struct channel*)listbox->select->handle)->protect = 0;
					}
					status.writechannel = 1;
				}
				delmarkedscreennodes(channellist, 1);
				delmarkedscreennodes(channellist, 2);
				recalclist(list, aktlist, listmode, channellist, listbox, channeltimeline);
				changebutton(listmode, b1, b2, b3, b4, b5, b6, b7, b8, b9, b10, b11, b12, b13, b14, b15, flag);
				drawscreen(channellist, 0, 0);
			}

			if(listmode == STREAMMODE && listbox->select != NULL && listbox->select->handle != NULL && rcret == getrcconfigint("rcok", NULL))
			{
				if((list == ALLCHANNEL || list == SATCHANNEL || list == PROVIDERCHANNEL || list == AZCHANNEL || list == BOUQUETCHANNEL) && listbox->select->handle != NULL)
				{
                    char* localparser = NULL, *streamurl = NULL;
                    localparser = ostrcat("/tmp/localparser/vavoo.sh", NULL, 0, 0);

		            struct skin* tithekplugin = getplugin("Titan Mediathek");
		            if(tithekplugin != NULL)
		            {
			            struct tithek* (*startplugin)(char*, char*);

			            startplugin = dlsym(tithekplugin->pluginhandle, "localparser_search_file");
			            if(startplugin != NULL)
			            {
printf("status.streamurl1: %s\n", status.streamurl);
printf("status.streamurl1: %s\n", ((struct channel*)listbox->select->handle)->name);
                        streamurl = (char*)startplugin(localparser, ((struct channel*)listbox->select->handle)->name);
printf("streamurl: %s\n", streamurl);
  			            }
		            }
                    free(localparser), localparser = NULL;

                    if (streamurl != NULL)
                        ((struct channel*)listbox->select->handle)->streamurl = ostrcat(streamurl, NULL, 0, 0);

                    free(streamurl), streamurl = NULL;
					status.writechannel = 1;
				}
				delmarkedscreennodes(channellist, 1);
				delmarkedscreennodes(channellist, 2);
				recalclist(list, aktlist, listmode, channellist, listbox, channeltimeline);
				changebutton(listmode, b1, b2, b3, b4, b5, b6, b7, b8, b9, b10, b11, b12, b13, b14, b15, flag);
				drawscreen(channellist, 0, 0);
			}
			if(listmode == CPMODE && listbox->select != NULL && listbox->select->handle != NULL && rcret == getrcconfigint("rcok", NULL))
			{
				if(list == PROVIDERLIST)
				{
					if(provider2bouquet(((struct provider*)listbox->select->handle1)->providerid, 0) == 0)
						textbox(_("Message"), _("Copy providerlist successfull"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 10, 0);
					else
						textbox(_("Message"), _("Copy providerlist not ok"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 10, 0);
				}
				else if(list == SATLIST)
				{
					if(sat2bouquet(((struct sat*)listbox->select->handle1)->orbitalpos, 0) == 0)
						textbox(_("Message"), _("Copy satlist successfull"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 10, 0);
					else
						textbox(_("Message"), _("Copy satlist not ok"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 10, 0);
				}
				else
				{
					struct mainbouquet* mbouquet = screenmainbouquet();
					if(mbouquet != NULL && getbouquetbychannel(mbouquet->bouquet, ((struct channel*)listbox->select->handle)->serviceid, ((struct channel*)listbox->select->handle)->transponderid) == NULL)
					{
						tmpstr1 = oitoa(((struct channel*)listbox->select->handle)->serviceid);
						tmpstr = ostrcat(tmpstr, tmpstr1, 1, 1); tmpstr1 = NULL;
						tmpstr = ostrcat(tmpstr, "#", 1, 0);
						tmpstr1 = ollutoa(((struct channel*)listbox->select->handle)->transponderid);
						tmpstr = ostrcat(tmpstr, tmpstr1, 1, 1); tmpstr1 = NULL;
						addbouquet(&mbouquet->bouquet, tmpstr, status.servicetype, 0, NULL);
						free(tmpstr); tmpstr = NULL;
						recalcbouquetnr();
						if(list == BOUQUETCHANNEL)
						{
							delmarkedscreennodes(channellist, 1);
							delmarkedscreennodes(channellist, 2);
							showbouquetchannel(channellist, listbox, channeltimeline, ((struct mainbouquet*)aktlist)->bouquet, (struct mainbouquet*)aktlist, 1);
						}
					}
				}
				changebutton(listmode, b1, b2, b3, b4, b5, b6, b7, b8, b9, b10, b11, b12, b13, b14, b15, flag);
				drawscreen(channellist, 0, 0);
			}
			if(listmode == EDITMODE && listbox->select != NULL && listbox->select->handle1 != NULL && rcret == getrcconfigint("rcok", NULL))
			{
				if(list == ALLCHANNEL || list == SATCHANNEL || list == PROVIDERCHANNEL || list == AZCHANNEL)
				{
					struct channel* oldentry = ((struct channel*)listbox->select->handle1);
					char* newentry = textinput(NULL, oldentry->name);
					if(newentry != NULL)
					{
						newentry = stringreplacechar(newentry, '#', '_');
						free(oldentry->name);
						oldentry->name = newentry;
						status.writechannel = 1;
					}
				}
				if(list == MAINBOUQUETLIST)
				{
					struct mainbouquet* oldentry = ((struct mainbouquet*)listbox->select->handle1);
					char* newentry = textinput(NULL, oldentry->name);
					if(newentry != NULL)
					{
						newentry = stringreplacechar(newentry, '#', '_');
						free(oldentry->name);
						oldentry->name = newentry;
						status.writemainbouquet = 1;
					}
				}
				if(list == BOUQUETCHANNEL)
				{
					struct channel* oldentry = getchannel(((struct bouquet*)listbox->select->handle1)->serviceid, ((struct bouquet*)listbox->select->handle1)->transponderid);
					if(oldentry != NULL)
					{
						char* newentry = textinput(NULL, oldentry->name);
						if(newentry != NULL)
						{
							newentry = stringreplacechar(newentry, '#', '_');
							free(oldentry->name);
							oldentry->name = newentry;
							status.writechannel = 1;
						}
					}
				}
				delmarkedscreennodes(channellist, 1);
				delmarkedscreennodes(channellist, 2);
				status.markedchannel = NULL;
				recalclist(list, aktlist, listmode, channellist, listbox, channeltimeline);
				changebutton(listmode, b1, b2, b3, b4, b5, b6, b7, b8, b9, b10, b11, b12, b13, b14, b15, flag);
				drawscreen(channellist, 0, 0);
			}
			if(listmode == RMMODE && listbox->select != NULL && listbox->select->handle1 != NULL && rcret == getrcconfigint("rcok", NULL))
			{
				if(list == ALLCHANNEL || list == SATCHANNEL || list == PROVIDERCHANNEL || list == AZCHANNEL)
				{
					if(delchannel(((struct channel*)listbox->select->handle1)->serviceid, ((struct channel*)listbox->select->handle1)->transponderid, 0) == 0)
					{
						listbox->aktline--;
						listbox->aktpage = -1;
					}
					else
						textbox(_("Message"), _("Can't remove akive Service"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 7, 0);
				}
				if(list == SATLIST)
				{
					delsat(((struct sat*)listbox->select->handle1)->name);
					listbox->aktline--;
					listbox->aktpage = -1;
				}
				if(list == PROVIDERLIST)
				{
					delprovider(((struct provider*)listbox->select->handle1)->providerid);
					listbox->aktline--;
					listbox->aktpage = -1;
				}
				if(list == MAINBOUQUETLIST)
				{
					delmainbouquet(((struct mainbouquet*)listbox->select->handle1)->name, 1);
					recalcbouquetnr();
					listbox->aktline--;
					listbox->aktpage = -1;
				}
				if(list == BOUQUETCHANNEL)
				{
					delbouquet(((struct bouquet*)listbox->select->handle1)->serviceid, ((struct bouquet*)listbox->select->handle1)->transponderid, &((struct mainbouquet*)aktlist)->bouquet);
					recalcbouquetnr();
					listbox->aktline--;
					listbox->aktpage = -1;
				}
				delmarkedscreennodes(channellist, 1);
				delmarkedscreennodes(channellist, 2);
				status.markedchannel = NULL;
				recalclist(list, aktlist, listmode, channellist, listbox, channeltimeline);
				changebutton(listmode, b1, b2, b3, b4, b5, b6, b7, b8, b9, b10, b11, b12, b13, b14, b15, flag);
				drawscreen(channellist, 0, 0);
			}
			if(listmode == MVMODE && listbox->select != NULL && rcret == getrcconfigint("rcok", NULL))
			{
				if(movesel == NULL)
				{
					status.markmodus = 1;
					movesel = listbox->select->handle1;
				}
				else
				{
					status.markmodus = 0;
					status.moveblockcount = 0;
					movesel = NULL;
				}
				changebutton(listmode, b1, b2, b3, b4, b5, b6, b7, b8, b9, b10, b11, b12, b13, b14, b15, flag);
				drawscreen(channellist, 0, 0);
			}
			if(listmode == MVMODE && listbox->select != NULL && movesel != NULL && (rcret == getrcconfigint("rcup", NULL) || rcret == getrcconfigint("rcdown", NULL) || rcret == getrcconfigint("rcchdown", NULL) || rcret == getrcconfigint("rcchup", NULL)))
			{
				if(rcret == getrcconfigint("rcchdown", NULL) && listbox->linecount != (listbox->aktline + status.moveblockcount))
				{
					struct skin* tmp = listbox->select;
					int i = 0, count = 0;
					status.moveblockcount++;
					for(i = 0; i < status.moveblockcount; i++)
					{
						tmp = tmp->next;
						if(tmp == NULL)
						{
							status.moveblockcount = count;
							break;
						}
						count++;
					}
					if(status.moveblockcount > 10)
						status.moveblockcount = 10;
					drawscreen(channellist, 0, 0);
					continue;
				}
				if(rcret == getrcconfigint("rcchup", NULL))
				{
					status.moveblockcount--;
					if(status.moveblockcount < 0)
						status.moveblockcount = 0;
					drawscreen(channellist, 0, 0);
					continue;
				}
				if(rcret == getrcconfigint("rcup", NULL))
				{
					if(list == ALLCHANNEL)
					{
						int ret = movechannelblockup(movesel);
						if(ret == 99) // wrap
							listbox->aktline -= (status.moveblockcount);
						//movechannelup(movesel);
						//while(((struct channel*)movesel)->next != NULL && ((struct channel*)movesel)->next->servicetype != status.servicetype)
						//	movechannelup(movesel);
					}
					if(list == SATLIST)
					{
						int ret = movesatblockup(movesel);
						if(ret == 99) // wrap
							listbox->aktline -= (status.moveblockcount);
						//movesatup(movesel);
					}
					if(list == PROVIDERLIST)
					{
						int ret = moveproviderblockup(movesel);
						if(ret == 99) // wrap
							listbox->aktline -= (status.moveblockcount);
						//moveproviderup(movesel);
					}
					if(list == MAINBOUQUETLIST)
					{
						int ret = movemainbouquetblockup(movesel);
						if(ret == 99) // wrap
							listbox->aktline -= (status.moveblockcount);
						//movemainbouquetup(movesel);
						//while(((struct mainbouquet*)movesel)->next != NULL && ((struct mainbouquet*)movesel)->next->type != status.servicetype)
						//	movemainbouquetup(movesel);
						recalcbouquetnr();
					}
					if(list == BOUQUETCHANNEL)
					{
						int ret = movebouquetblockup(movesel);
						if(ret == 99) // wrap
							listbox->aktline -= (status.moveblockcount);
						//movebouquetup(movesel);
						//while(((struct bouquet*)movesel)->next != NULL && ((struct bouquet*)movesel)->next->channel != NULL && ((struct bouquet*)movesel)->next->channel->servicetype != status.servicetype)
						//	movebouquetup(movesel);
						recalcbouquetnr();
					}
				}
				if(rcret == getrcconfigint("rcdown", NULL))
				{
					if(list == ALLCHANNEL)
					{
						int ret = movechannelblockdown(movesel);
						if(ret == 99) // wrap
						{
							listbox->aktpage = -1;
							listbox->aktline = 1;
						}
						//movechanneldown(movesel);
						//while(((struct channel*)movesel)->prev != NULL && ((struct channel*)movesel)->prev->servicetype != status.servicetype)
						//	movechanneldown(movesel);
					}
					if(list == SATLIST)
					{
						int ret = movesatblockdown(movesel);
						if(ret == 99) // wrap
						{
							listbox->aktpage = -1;
							listbox->aktline = 1;
						}
						//movesatdown(movesel);
					}
					if(list == PROVIDERLIST)
					{
						int ret = moveproviderblockdown(movesel);
						if(ret == 99) // wrap
						{
							listbox->aktpage = -1;
							listbox->aktline = 1;
						}
						//moveproviderdown(movesel);
					}
					if(list == MAINBOUQUETLIST)
					{
						int ret = movemainbouquetblockdown(movesel);
						if(ret == 99) // wrap
						{
							listbox->aktpage = -1;
							listbox->aktline = 1;
						}
						//movemainbouquetdown(movesel);
						//while(((struct mainbouquet*)movesel)->prev != NULL && ((struct mainbouquet*)movesel)->prev->type != status.servicetype)
						//	movemainbouquetdown(movesel);
						recalcbouquetnr();
					}
					if(list == BOUQUETCHANNEL)
					{
						int ret = movebouquetblockdown(movesel);
						if(ret == 99) // wrap
						{
							listbox->aktpage = -1;
							listbox->aktline = 1;
						}
						//movebouquetdown(movesel);
						//while(((struct bouquet*)movesel)->prev != NULL && ((struct bouquet*)movesel)->prev->channel != NULL && ((struct bouquet*)movesel)->prev->channel->servicetype != status.servicetype)
						//	movebouquetdown(movesel);
						recalcbouquetnr();
					}
				}
				delmarkedscreennodes(channellist, 1);
				delmarkedscreennodes(channellist, 2);
				recalclist(list, aktlist, listmode, channellist, listbox, channeltimeline);
				changebutton(listmode, b1, b2, b3, b4, b5, b6, b7, b8, b9, b10, b11, b12, b13, b14, b15, flag);
				drawscreen(channellist, 0, 0);
			}
			continue;
		}

		if(rcret == getrcconfigint("rcred", NULL))
		{
			list = ALLCHANNEL;
			if(status.servicetype == 0)
				addconfigtmp("channellist", "(ALL)");
			else
				addconfigtmp("rchannellist", "(ALL)");
			if(nochanneltitle == 0)
			{
				changetitle(channellist, _("All Channels"));
			}
			delmarkedscreennodes(channellist, 1);
			delmarkedscreennodes(channellist, 2);
			showallchannel(channellist, listbox, channeltimeline, flag);
			selectchannel(listbox, 0, 0);
			changebutton(listmode, b1, b2, b3, b4, b5, b6, b7, b8, b9, b10, b11, b12, b13, b14, b15, flag);
			drawchannellist(channellist, list, listbox);
			continue;
		}
		if(rcret == getrcconfigint("rcblue", NULL) || rcret == getrcconfigint("rcfav", NULL))
		{
			list = MAINBOUQUETLIST;
			if(nochanneltitle == 0) 
			{
				changetitle(channellist, _("Bouquets"));
			}
			delmarkedscreennodes(channellist, 1);
			delmarkedscreennodes(channellist, 2);
			showmainbouquet(channellist, listbox);
			if(status.servicetype == 0)
			{
				delconfigtmp("channellist");
				tmpstr = getconfig("channellist", NULL);
			}
			else
			{
				delconfigtmp("rchannellist");
				tmpstr = getconfig("rchannellist", NULL);
			}
			if(ostrncmp("(BOUQUET)-", tmpstr, 10) != 0) tmpstr = NULL;
			if(tmpstr != NULL && strlen(tmpstr) > 10)
				setlistboxselection(listbox, tmpstr + 10);
			else
			{
				listbox->aktpage = -1;
				listbox->aktline = 1;
			}
			tmpstr = NULL;
			changebutton(listmode, b1, b2, b3, b4, b5, b6, b7, b8, b9, b10, b11, b12, b13, b14, b15, flag);
			drawchannellist(channellist, list, listbox);
			continue;
		}
		if(rcret == getrcconfigint("rcgreen", NULL))
		{
			list = SATLIST;
			if(nochanneltitle == 0)
			{
				changetitle(channellist, _("Satellites"));
			}	
			delmarkedscreennodes(channellist, 1);
			delmarkedscreennodes(channellist, 2);
			showsat(channellist, listbox);
			if(status.servicetype == 0)
			{
				delconfigtmp("channellist");
				tmpstr = getconfig("channellist", NULL);
			}
			else
			{
				delconfigtmp("rchannellist");
				tmpstr = getconfig("rchannellist", NULL);
			}
			if(ostrncmp("(SAT)-", tmpstr, 6) != 0) tmpstr = NULL;
			if(tmpstr != NULL && strlen(tmpstr) > 6)
				setlistboxselection(listbox, tmpstr + 6);
			else
			{
				listbox->aktpage = -1;
				listbox->aktline = 1;
			}
			tmpstr = NULL;
			changebutton(listmode, b1, b2, b3, b4, b5, b6, b7, b8, b9, b10, b11, b12, b13, b14, b15, flag);
			drawchannellist(channellist, list, listbox);
			continue;
		}
		if(rcret == getrcconfigint("rctext", NULL))
		{
			list = AZLIST;
			if(nochanneltitle == 0)
			{
				changetitle(channellist, _("Channels A-Z"));
			}
			delmarkedscreennodes(channellist, 1);
			delmarkedscreennodes(channellist, 2);
			showaz(channellist, listbox);
			if(status.servicetype == 0)
			{
				delconfigtmp("channellist");
				tmpstr = getconfig("channellist", NULL);
			}
			else
			{
				delconfigtmp("rchannellist");
				tmpstr = getconfig("rchannellist", NULL);
			}
			if(ostrncmp("(A-Z)-", tmpstr, 6) != 0) tmpstr = NULL;
			if(tmpstr != NULL && strlen(tmpstr) > 6)
				setlistboxselection(listbox, &tmpstr[6]);
			else
			{
				listbox->aktpage = -1;
				listbox->aktline = 1;
			}
			tmpstr = NULL;
			changebutton(listmode, b1, b2, b3, b4, b5, b6, b7, b8, b9, b10, b11, b12, b13, b14, b15, flag);
			drawchannellist(channellist, list, listbox);
			continue;
		}
		if(rcret == getrcconfigint("rcyellow", NULL))
		{
			list = PROVIDERLIST;
			if(nochanneltitle == 0)
			{
				changetitle(channellist, _("Provider"));
			}	
			delmarkedscreennodes(channellist, 1);
			delmarkedscreennodes(channellist, 2);
			showprovider(channellist, listbox);
			if(status.servicetype == 0)
			{
				delconfigtmp("channellist");
				tmpstr = getconfig("channellist", NULL);
			}
			else
			{
				delconfigtmp("rchannellist");
				tmpstr = getconfig("rchannellist", NULL);
			}
			if(ostrncmp("(PROVIDER)-", tmpstr, 11) != 0) tmpstr = NULL;
			if(tmpstr != NULL && strlen(tmpstr) > 11)
				setlistboxselection(listbox, tmpstr + 11);
			else
			{
				listbox->aktpage = -1;
				listbox->aktline = 1;
			}
			tmpstr = NULL;
			changebutton(listmode, b1, b2, b3, b4, b5, b6, b7, b8, b9, b10, b11, b12, b13, b14, b15, flag);
			drawchannellist(channellist, list, listbox);
			continue;
		}
		if((list == ALLCHANNEL || list == SATCHANNEL || list == PROVIDERCHANNEL || list == AZCHANNEL || list == BOUQUETCHANNEL) && rcret == getrcconfigint("rcok", NULL))
		{ 
			if(listbox->select != NULL && listbox->select->handle != NULL)
			{
				if(flag == 1)
				{
					if(retchannel != NULL)
						*retchannel = (struct channel*)listbox->select->handle;
					if(retchannellist != NULL)
					{
						if(status.servicetype == 0)
							*retchannellist = ostrcat(getconfig("channellist", NULL), NULL, 0, 0);
						else
							*retchannellist = ostrcat(getconfig("rchannellist", NULL), NULL, 0, 0);
					}
					break;
				}

				clearscreen(channellist);
				changebutton(listmode, b1, b2, b3, b4, b5, b6, b7, b8, b9, b10, b11, b12, b13, b14, b15, flag);
				resettvpic();
				drawscreen(skin, 0, 0);
				if(status.servicetype == 0)
					ret = servicestart((struct channel*)listbox->select->handle, getconfig("channellist", NULL), NULL, 0);
				else
					ret = servicestart((struct channel*)listbox->select->handle, getconfig("rchannellist", NULL), NULL, 0);
				if(ret == 20) writeconfigtmp();
				servicecheckret(ret, 0);
				break;
			}
			continue;
		}
		if(list == BOUQUETCHANNEL && (rcret == getrcconfigint("rcff", NULL) || rcret == getrcconfigint("rcfr", NULL) || rcret == getrcconfigint("rclast", NULL) || rcret == getrcconfigint("rcprev", NULL) || rcret == getrcconfigint("rcnext", NULL))) 
		{
			struct mainbouquet* tmpaktlist = ((struct mainbouquet*)aktlist);

			int round = 0;
			while(tmpaktlist != NULL)
			{
				if(rcret == getrcconfigint("rcff", NULL) || rcret == getrcconfigint("rcprev", NULL) || rcret == getrcconfigint("rcnext", NULL))
				{
					tmpaktlist = tmpaktlist->next;
					if(tmpaktlist == NULL && round == 0)	
					{
						round = 1;
						tmpaktlist = mainbouquet;
					}
				}
				else if(rcret == getrcconfigint("rcfr", NULL) || rcret == getrcconfigint("rclast", NULL))
				{
					tmpaktlist = tmpaktlist->prev;
					if(tmpaktlist == NULL && round == 0)	
					{
						round = 1;
						tmpaktlist = getlastmainbouquet(mainbouquet);
					}
				}
				if(tmpaktlist == NULL) break;
				if(tmpaktlist->type != status.servicetype) continue;

				delmarkedscreennodes(channellist, 1);
				struct skin* tmpnode = addlistbox(channellist, listbox, NULL, 2);
				if(tmpnode != NULL)
				{
					listbox->aktline = 1;
					listbox->aktpage = -1;

					drawscreen(channellist, 2, 0);
					changetext(tmpnode, tmpaktlist->name);
					tmpnode->handle = (char*)tmpaktlist->bouquet;
					tmpnode->handle1 = (char*)tmpaktlist;
					rcret = getrcconfigint("rcok", NULL);
					list = MAINBOUQUETLIST;
					break;
				}
			}
		}
		if(list == MAINBOUQUETLIST && rcret == getrcconfigint("rcok", NULL))
		{
			if(listbox->select != NULL)
			{
				list = BOUQUETCHANNEL;
				tmpstr = ostrcat(tmpstr, _("Bouquets"), 0, 0);
				tmpstr = ostrcat(tmpstr, " - ", 1, 0);
				tmpstr = ostrcat(tmpstr, listbox->select->text, 1, 0);
				if(nochanneltitle == 0)
				{
					changetitle(channellist, tmpstr);
				}
				free(tmpstr); tmpstr = NULL;
				tmpstr = ostrcat(tmpstr, "(BOUQUET)-", 0, 0);
				tmpstr = ostrcat(tmpstr, listbox->select->text, 1, 0);
				if(status.servicetype == 0)
					addconfigtmp("channellist", tmpstr);
				else
					addconfigtmp("rchannellist", tmpstr);
				free(tmpstr); tmpstr = NULL;
				aktlist = listbox->select->handle1;
				showbouquetchannel(channellist, listbox, channeltimeline, (struct bouquet*)listbox->select->handle, (struct mainbouquet*)aktlist, flag);
				delmarkedscreennodes(channellist, 2);
				selectchannel(listbox, 0, 0);
				changebutton(listmode, b1, b2, b3, b4, b5, b6, b7, b8, b9, b10, b11, b12, b13, b14, b15, flag);
				drawchannellist(channellist, list, listbox);
			}
			continue;
		}
		if(list == SATCHANNEL && (rcret == getrcconfigint("rcff", NULL) || rcret == getrcconfigint("rcfr", NULL))) 
		{
			struct sat* tmpaktlist = ((struct sat*)aktlist);

			int round = 0;
			while(tmpaktlist != NULL)
			{
				if(rcret == getrcconfigint("rcff", NULL))
				{
					tmpaktlist = tmpaktlist->next;
					if(tmpaktlist == NULL && round == 0)	
					{
						round = 1;
						tmpaktlist = sat;
					}
				}
				else if(rcret == getrcconfigint("rcfr", NULL))
				{
					tmpaktlist = tmpaktlist->prev;
					if(tmpaktlist == NULL && round == 0)	
					{
						round = 1;
						tmpaktlist = getlastsat(sat);
					}
				}
				if(tmpaktlist == NULL) break;

				delmarkedscreennodes(channellist, 1);
				struct skin* tmpnode = addlistbox(channellist, listbox, NULL, 2);
				if(tmpnode != NULL)
				{
					listbox->aktline = 1;
					listbox->aktpage = -1;

					drawscreen(channellist, 2, 0);
					changetext(tmpnode, tmpaktlist->name);
					tmpnode->handle = (char*)tmpaktlist;
					tmpnode->handle1 = (char*)tmpaktlist;
					rcret = getrcconfigint("rcok", NULL);
					list = SATLIST;
					break;
				}
			}
		}
		if(list == SATLIST && rcret == getrcconfigint("rcok", NULL))
		{
			if(listbox->select != NULL)
			{
				list = SATCHANNEL;
				tmpstr = ostrcat(tmpstr, _("Satellites"), 0, 0);
				tmpstr = ostrcat(tmpstr, " - ", 1, 0);
				tmpstr = ostrcat(tmpstr, listbox->select->text, 1, 0);
				if(nochanneltitle == 0)
				{
					changetitle(channellist, tmpstr);
				}
				free(tmpstr); tmpstr = NULL;
				tmpstr = ostrcat(tmpstr, "(SAT)-", 0, 0);
				tmpstr = ostrcat(tmpstr, listbox->select->text, 1, 0);
				if(status.servicetype == 0)
					addconfigtmp("channellist", tmpstr);
				else
					addconfigtmp("rchannellist", tmpstr);
				free(tmpstr); tmpstr = NULL;
				aktlist = listbox->select->handle;
				showsatchannel(channellist, listbox, channeltimeline, (struct sat*)listbox->select->handle, flag);
				delmarkedscreennodes(channellist, 2);
				selectchannel(listbox, 0, 0);
				changebutton(listmode, b1, b2, b3, b4, b5, b6, b7, b8, b9, b10, b11, b12, b13, b14, b15, flag);
				drawchannellist(channellist, list, listbox);
			}
			continue;
		}
		if(list == AZCHANNEL && (rcret == getrcconfigint("rcff", NULL) || rcret == getrcconfigint("rcfr", NULL))) 
		{
			int tmpaktlist = (int)aktlist;

			if(rcret == getrcconfigint("rcff", NULL))
			{
				tmpaktlist++;
				if(tmpaktlist > 90) tmpaktlist = 65;
			}
			else if(rcret == getrcconfigint("rcfr", NULL))
			{
				tmpaktlist--;
				if(tmpaktlist < 65) tmpaktlist = 90;
			}

			delmarkedscreennodes(channellist, 1);
			struct skin* tmpnode = addlistbox(channellist, listbox, NULL, 2);
			if(tmpnode != NULL)
			{
				listbox->aktline = 1;
				listbox->aktpage = -1;

				drawscreen(channellist, 2, 0);
				tmpstr = malloc(2);
				if(tmpstr != NULL)
				{
					snprintf(tmpstr, 2, "%c", tmpaktlist);
					changetext(tmpnode, tmpstr);
					free(tmpstr); tmpstr = NULL;
				}
				tmpnode->handle = (char*)tmpaktlist;
				tmpnode->handle1 = (char*)tmpaktlist;
				rcret = getrcconfigint("rcok", NULL);
				list = AZLIST;
			}
		}
		if(list == AZLIST && rcret == getrcconfigint("rcok", NULL))
		{
			if(listbox->select != NULL)
			{
				list = AZCHANNEL;
				tmpstr = ostrcat(tmpstr, _("Channel"), 0, 0);
				tmpstr = ostrcat(tmpstr, " - ", 1, 0);
				tmpstr = ostrcat(tmpstr, listbox->select->text, 1, 0);
				if(nochanneltitle == 0)
				{
					changetitle(channellist, tmpstr);
				}
				free(tmpstr); tmpstr = NULL;
				tmpstr = ostrcat(tmpstr, "(A-Z)-", 0, 0);
				tmpstr = ostrcat(tmpstr, listbox->select->text, 1, 0);
				if(status.servicetype == 0)
					addconfigtmp("channellist", tmpstr);
				else
					addconfigtmp("rchannellist", tmpstr);
				free(tmpstr); tmpstr = NULL;
				aktlist = listbox->select->handle;
				showazchannel(channellist, listbox, channeltimeline, (int)listbox->select->handle, flag);
				delmarkedscreennodes(channellist, 2);
				selectchannel(listbox, 0, 0);
				changebutton(listmode, b1, b2, b3, b4, b5, b6, b7, b8, b9, b10, b11, b12, b13, b14, b15, flag);
				drawchannellist(channellist, list, listbox);
			}
			continue;
		}
		if(list == PROVIDERCHANNEL && (rcret == getrcconfigint("rcff", NULL) || rcret == getrcconfigint("rcfr", NULL))) 
		{
			struct provider* tmpaktlist = ((struct provider*)aktlist);

			int round = 0;
			while(tmpaktlist != NULL)
			{
				if(rcret == getrcconfigint("rcff", NULL))
				{
					tmpaktlist = tmpaktlist->next;
					if(tmpaktlist == NULL && round == 0)	
					{
						round = 1;
						tmpaktlist = provider;
					}
				}
				else if(rcret == getrcconfigint("rcfr", NULL))
				{
					tmpaktlist = tmpaktlist->prev;
					if(tmpaktlist == NULL && round == 0)	
					{
						round = 1;
						tmpaktlist = getlastprovider(provider);
					}
				}
				if(tmpaktlist == NULL) break;

				if(checkprovider(tmpaktlist) == 1)
				{
					delmarkedscreennodes(channellist, 1);
					struct skin* tmpnode = addlistbox(channellist, listbox, NULL, 2);
					if(tmpnode != NULL)
					{
						listbox->aktline = 1;
						listbox->aktpage = -1;
	
						drawscreen(channellist, 2, 0);
						changetext(tmpnode, tmpaktlist->name);
						tmpnode->handle = (char*)tmpaktlist;
						tmpnode->handle1 = (char*)tmpaktlist;
						rcret = getrcconfigint("rcok", NULL);
						list = PROVIDERLIST;
						break;
					}
				}
			}
		}
		if(list == PROVIDERLIST && rcret == getrcconfigint("rcok", NULL))
		{
			if(listbox->select != NULL)
			{
				list = PROVIDERCHANNEL;
				tmpstr = ostrcat(tmpstr, _("Provider"), 0, 0);
				tmpstr = ostrcat(tmpstr, " - ", 1, 0);
				tmpstr = ostrcat(tmpstr, listbox->select->text, 1, 0);
				if(nochanneltitle == 0)
				{
					changetitle(channellist, tmpstr);
				}
				free(tmpstr); tmpstr = NULL;
				tmpstr = ostrcat(tmpstr, "(PROVIDER)-", 0, 0);
				tmpstr = ostrcat(tmpstr, listbox->select->text, 1, 0);
				if(status.servicetype == 0)
					addconfigtmp("channellist", tmpstr);
				else
					addconfigtmp("rchannellist", tmpstr);
				free(tmpstr); tmpstr = NULL;
				aktlist = listbox->select->handle;
				showproviderchannel(channellist, listbox, channeltimeline, (struct provider*) listbox->select->handle, flag);
				delmarkedscreennodes(channellist, 2);
				selectchannel(listbox, 0, 0);
				changebutton(listmode, b1, b2, b3, b4, b5, b6, b7, b8, b9, b10, b11, b12, b13, b14, b15, flag);
				drawchannellist(channellist, list, listbox);
			}
			continue;
		}
		if(flag == 0 && rcret == getrcconfigint("rcmenu", NULL))
		{
			if(list == AZLIST) continue;

			int sid = 0;
			uint64_t tid = 0;
			if((list == ALLCHANNEL || list == SATCHANNEL || list == PROVIDERCHANNEL || list == AZCHANNEL || list == BOUQUETCHANNEL) && listbox->select != NULL && listbox->select->handle != NULL)
			{
				sid = ((struct channel*)listbox->select->handle)->serviceid;
				tid = ((struct channel*)listbox->select->handle)->transponderid;
			}

			clearscreen(channellist);
			resettvpic();
printf("status.streamurlc2: %s\n", status.streamurl);

			if(listbox->select != NULL)
				listmode = screenlistedit(list, (struct channel*)listbox->select->handle, aktlist);
			else
				listmode = screenlistedit(list, NULL, aktlist);
                                
			if(listmode == MVMODE)
			{
				delrc(getrcconfigint("rcright", NULL), channellist, listbox);
				delrc(getrcconfigint("rcleft", NULL), channellist, listbox);
			}
			else
				addscreenrcchannellist(channellist, listbox);

			if(nochanneltitle == 0) changechanneltitle(channellist, listbox, listmode, &oldtitle, &oldfontcol, &oldbgcol);

			if(listmode == NOMODE && flag == 3) flag = 0;
			delmarkedscreennodes(channellist, 1);
			delmarkedscreennodes(channellist, 2);
			recalclist(list, aktlist, listmode, channellist, listbox, channeltimeline);
			selectchannel(listbox, sid, tid);
			changebutton(listmode, b1, b2, b3, b4, b5, b6, b7, b8, b9, b10, b11, b12, b13, b14, b15, flag);
			drawscreen(channellist, 0, 0);
			continue;
		}
		if(flag == 0 && rcret == getrcconfigint("rcepg", NULL) && (list == ALLCHANNEL || list == SATCHANNEL || list == PROVIDERCHANNEL || list == AZCHANNEL || list == BOUQUETCHANNEL))
		{
			int ret = 0;
			if(listbox->select != NULL)
			{
				clearscreen(channellist);
				resettvpic();
				struct channel* tmpmarkedchannel = status.markedchannel;
				ret = epgchoice((struct channel*)listbox->select->handle);
				//epg ends with no zap
				if(ret == 0)
				{
					status.markedchannel = tmpmarkedchannel;
					changebutton(listmode, b1, b2, b3, b4, b5, b6, b7, b8, b9, b10, b11, b12, b13, b14, b15, flag);
					drawchannellist(channellist, list, listbox);
				}
			}
			if(ret == 1)
				break;
			else
				continue;
		}
		if(flag == 0 && rcret == getrcconfigint("rc0", NULL) && (list == ALLCHANNEL || list == SATCHANNEL || list == PROVIDERCHANNEL || list == AZCHANNEL || list == BOUQUETCHANNEL))
		{
			if(listbox->select != NULL)
			{
				clearscreen(channellist);
				resettvpic();
				struct channel* tmpmarkedchannel = status.markedchannel;
				screenshortepg((struct channel*)listbox->select->handle, NULL, 0);
				status.markedchannel = tmpmarkedchannel;
				changebutton(listmode, b1, b2, b3, b4, b5, b6, b7, b8, b9, b10, b11, b12, b13, b14, b15, flag);
				drawscreen(channellist, 0, 0);
			}
			continue;
		}
		if(flag == 0 && rcret == getrcconfigint("rcinfo", NULL))
		{
			if(status.servicetype == 0)
			{
				status.servicetype = 1;
				changetext(b8, _("TV"));
			}
			else
			{
				status.servicetype = 0;
				changetext(b8, _("Radio"));
			}
			newmodus = 1;
			break;
		}
	}

end:
	status.updatevfd = START;
	listbox->fontcol = oldfontcol;
	listbox->bgcol = oldbgcol;
	status.moveblockcount = 0;
	free(oldtitle);
	status.markedchannel = NULL;
	status.markmodus = 0;
	delmarkedscreennodes(channellist, 1);
	delmarkedscreennodes(channellist, 2);
	delownerrc(channellist);
	delconfigtmp("channellist");
	delconfigtmp("rchannellist");
	clearscreen(channellist);
	if(newmodus == 1) goto start;
	status.channelswitch = 0;
	resettvpic();
	return ret;
}

#endif
