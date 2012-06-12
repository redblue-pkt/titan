#ifndef MULTIEPG_H
#define MULTIEPG_H

#define TEXTPOSEPG 250

struct epg* readepgdesc(struct channel* curchannel, int epgnr)
{
	int i = 0;
	struct epg* epgnode = NULL;

	epgnode = getepgakt(curchannel);
			
	while(epgnode != NULL){
		if(i >= epgnr) break;
		epgnode = epgnode->next;
		i++;
	}
	return epgnode;
}

int selectchannelmepg(struct skin* listbox)
{
	struct skin* node = listbox;
	struct channel* chnode = NULL;
	listbox->aktpage = -1;
	listbox->aktline = 1;

	listbox->select = NULL;

	if(status.servicetype == 0)
		chnode = getchannel(getconfigint("serviceid", NULL), getconfiglu("transponderid", NULL));
	else
		chnode = getchannel(getconfigint("rserviceid", NULL), getconfiglu("rtransponderid", NULL));

	if(chnode == NULL)
	{
		debug(1000, "out -> NULL detect");
		return 1;
	}

	while(node != NULL)
	{
		if(node->deaktivcol > -1)
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

int showallmepgchannel(struct skin* multiepg, struct skin* listbox, int epgnr)
{
	int treffer = 0;
	struct skin* chnode = NULL;
	struct channel* tmpchannel = channel;
	struct epg* epgnode = NULL;
	struct tm *loctime = NULL;
	char* tmpstr = NULL, *buf = NULL;

	if(multiepg != NULL)
		delmarkedscreennodes(multiepg, 1);

	buf = malloc(MINMALLOC);
	if(buf == NULL)
	{
		err("no mem");
		return 1;
	}

	while(tmpchannel != NULL)
	{
		if(tmpchannel->servicetype == status.servicetype)
		{
			chnode = addlistbox(multiepg, listbox, chnode, 1);
			if(chnode != NULL)
			{
				changetext(chnode, tmpchannel->name);

				epgnode = readepgdesc(tmpchannel, epgnr);
				if(epgnode != NULL)
				{
					treffer = 1;

					chnode->type = MULTIPROGRESSBAR;
					chnode->progresscol = listbox->progresscol;
					chnode->epgrecord = getepgrecord(tmpchannel, epgnode);
			
					loctime = localtime(&epgnode->starttime);
					ostrftime(buf, MINMALLOC, listbox->param1, loctime);
					tmpstr = ostrcat(tmpstr, buf, 1, 0);
					loctime = localtime(&epgnode->endtime);
					ostrftime(buf, MINMALLOC, listbox->param2, loctime);
					tmpstr = ostrcat(tmpstr, buf, 1, 0);
					tmpstr = ostrcat(tmpstr, " - ", 1, 0);
					tmpstr = ostrcat(tmpstr, epgnode->title, 1, 0);
					changetext2(chnode, tmpstr);
					free(tmpstr); tmpstr = NULL;
					chnode->textposx2 = TEXTPOSEPG;					
					chnode->handle1 = (char*) epgnode;
				}

				chnode->handle = (char*) tmpchannel;
				if(channelnottunable(tmpchannel) == 1)
					chnode->deaktivcol = convertcol("deaktivcol");
			}
		}
		tmpchannel = tmpchannel->next;
	}
	free(buf);
	return treffer;
}

int showbouquetmepgchannel(struct skin* multiepg, struct skin* listbox, struct bouquet* firstbouquet, struct mainbouquet* mbouquet, int epgnr)
{
	int treffer = 0;
	struct skin* chnode = NULL;
	struct bouquet* tmpbouquet = firstbouquet;
	struct epg* epgnode = NULL;
	struct tm *loctime = NULL;
	char* tmpstr = NULL, *tmpnr = NULL, *buf = NULL;

	if(multiepg != NULL)
		delmarkedscreennodes(multiepg, 1);

	buf = malloc(MINMALLOC);
	if(buf == NULL)
	{
		err("no mem");
		return 1;
	}

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
				chnode = addlistbox(multiepg, listbox, chnode, 1);
				if(chnode != NULL)
				{
					tmpnr = oitoa(tmpbouquet->nr);
					changeret(chnode, tmpnr);
					tmpstr = ostrcat(tmpstr, tmpnr, 1, 1);
					tmpstr = ostrcat(tmpstr, "  ", 1, 0);
					tmpstr = ostrcat(tmpstr, tmpbouquet->channel->name, 1, 0);
					changetext(chnode, tmpstr);
					free(tmpstr); tmpstr = NULL;

					epgnode = readepgdesc(tmpbouquet->channel, epgnr);
					if(epgnode != NULL)
					{
						treffer = 1;
						
						chnode->type = MULTIPROGRESSBAR;
						chnode->progresscol = listbox->progresscol;
						chnode->epgrecord = getepgrecord(tmpbouquet->channel, epgnode);
			
						loctime = localtime(&epgnode->starttime);
						ostrftime(buf, MINMALLOC, listbox->param1, loctime);
						tmpstr = ostrcat(tmpstr, buf, 1, 0);

						loctime = localtime(&epgnode->endtime);
						ostrftime(buf, MINMALLOC, listbox->param2, loctime);
						tmpstr = ostrcat(tmpstr, buf, 1, 0);
						tmpstr = ostrcat(tmpstr, " - ", 1, 0);
						tmpstr = ostrcat(tmpstr, epgnode->title, 1, 0);
						changetext2(chnode, tmpstr);
						free(tmpstr); tmpstr = NULL;
						chnode->textposx2 = TEXTPOSEPG;
						chnode->handle1 = (char*) epgnode;
					}
					chnode->handle = (char*) tmpbouquet->channel;
					if(channelnottunable(tmpbouquet->channel) == 1)
						chnode->deaktivcol = convertcol("deaktivcol");
				}
			}
		}
		tmpbouquet = tmpbouquet->next;
	}
	free(buf);
	return treffer;
}

int showprovidermepgchannel(struct skin* multiepg, struct skin* listbox, struct provider* providernode, int epgnr)
{
	int treffer = 0;
	struct skin* chnode = NULL;
	struct channel* tmpchannel = channel;
	struct epg* epgnode = NULL;
	struct tm *loctime = NULL;
	char* tmpstr = NULL, *buf = NULL;

	if(multiepg != NULL)
		delmarkedscreennodes(multiepg, 1);

	buf = malloc(MINMALLOC);
	if(buf == NULL)
	{
		err("no mem");
		return 1;
	}

	while(tmpchannel != NULL)
	{
		if(tmpchannel->provider == providernode)
		{
			if(tmpchannel->servicetype == status.servicetype)
			{
				chnode = addlistbox(multiepg, listbox, chnode, 1);
				if(chnode != NULL)
				{
					changetext(chnode, tmpchannel->name);

					epgnode = readepgdesc(tmpchannel, epgnr);
					if(epgnode != NULL)
					{
						treffer = 1;
						
						chnode->type = MULTIPROGRESSBAR;
						chnode->progresscol = listbox->progresscol;
						chnode->epgrecord = getepgrecord(tmpchannel, epgnode);
						
						loctime = localtime(&epgnode->starttime);
						ostrftime(buf, MINMALLOC, listbox->param1, loctime);
						tmpstr = ostrcat(tmpstr, buf, 1, 0);
						loctime = localtime(&epgnode->endtime);
						ostrftime(buf, MINMALLOC, listbox->param2, loctime);
						tmpstr = ostrcat(tmpstr, buf, 1, 0);
						tmpstr = ostrcat(tmpstr, " - ", 1, 0);
						tmpstr = ostrcat(tmpstr, epgnode->title, 1, 0);
						changetext2(chnode, tmpstr);
						free(tmpstr); tmpstr = NULL;
						chnode->textposx2 = TEXTPOSEPG;					
						chnode->handle1 = (char*) epgnode;
					}
					chnode->handle = (char*) tmpchannel;
					if(channelnottunable(tmpchannel) == 1)
						chnode->deaktivcol = convertcol("deaktivcol");
				}
			}
		}
		tmpchannel = tmpchannel->next;
	}
	free(buf);
	return treffer;
}

int showsatmepgchannel(struct skin* multiepg, struct skin* listbox, struct sat* satnode, int epgnr)
{
	int treffer = 0;
	struct skin* chnode = NULL;
	struct channel* tmpchannel = channel;
	struct epg* epgnode = NULL;
	struct tm *loctime = NULL;
	char* tmpstr = NULL, *buf = NULL;
	
	if(satnode == NULL)
		return 1;

	if(multiepg != NULL)
		delmarkedscreennodes(multiepg, 1);

	buf = malloc(MINMALLOC);
	if(buf == NULL)
	{
		err("no mem");
		return 1;
	}

	while(tmpchannel != NULL)
	{
		if(tmpchannel->transponder != NULL && tmpchannel->transponder->orbitalpos == satnode->orbitalpos)
		{
			if(tmpchannel->servicetype == status.servicetype)
			{
				chnode = addlistbox(multiepg, listbox, chnode, 1);
				if(chnode != NULL)
				{
					changetext(chnode, tmpchannel->name);

					epgnode = readepgdesc(tmpchannel, epgnr);
					if(epgnode != NULL)
					{
						treffer = 1;
						
						chnode->type = MULTIPROGRESSBAR;
						chnode->progresscol = listbox->progresscol;
						chnode->epgrecord = getepgrecord(tmpchannel, epgnode);
						
						loctime = localtime(&epgnode->starttime);
						ostrftime(buf, MINMALLOC, listbox->param1, loctime);
						tmpstr = ostrcat(tmpstr, buf, 1, 0);
						loctime = localtime(&epgnode->endtime);
						ostrftime(buf, MINMALLOC, listbox->param2, loctime);
						tmpstr = ostrcat(tmpstr, buf, 1, 0);
						tmpstr = ostrcat(tmpstr, " - ", 1, 0);
						tmpstr = ostrcat(tmpstr, epgnode->title, 1, 0);
						changetext2(chnode, tmpstr);
						free(tmpstr); tmpstr = NULL;
						chnode->textposx2 = TEXTPOSEPG;					
						chnode->handle1 = (char*) epgnode;
					}
					chnode->handle = (char*) tmpchannel;
					if(channelnottunable(tmpchannel) == 1)
						chnode->deaktivcol = convertcol("deaktivcol");
				}
			}
		}
		tmpchannel = tmpchannel->next;
	}
	free(buf);
	return treffer;
}

int showazmepgchannel(struct skin* multiepg, struct skin* listbox, int character, int epgnr)
{
	int treffer = 0;
	struct skin* chnode = NULL;
	struct channel* tmpchannel = channel;
	struct epg* epgnode = NULL;
	struct tm *loctime = NULL;
	char* tmpstr = NULL, *buf = NULL;

	if(multiepg != NULL)
		delmarkedscreennodes(multiepg, 1);

	buf = malloc(MINMALLOC);
	if(buf == NULL)
	{
		err("no mem");
		return 1;
	}

	while(tmpchannel != NULL)
	{
		if(tmpchannel->name != NULL && (tmpchannel->name[0] == character || tmpchannel->name[0] == character + 32))
		{
			if(tmpchannel->servicetype == status.servicetype)
			{
				chnode = addlistbox(multiepg, listbox, chnode, 1);
				if(chnode != NULL)
				{
					changetext(chnode, tmpchannel->name);
					epgnode = readepgdesc(tmpchannel, epgnr);
					if(epgnode != NULL)
					{
						treffer = 1;
						
						chnode->type = MULTIPROGRESSBAR;
						chnode->progresscol = listbox->progresscol;
						chnode->epgrecord = getepgrecord(tmpchannel, epgnode);
						
						loctime = localtime(&epgnode->starttime);
						ostrftime(buf, MINMALLOC, listbox->param1, loctime);
						tmpstr = ostrcat(tmpstr, buf, 1, 0);
						loctime = localtime(&epgnode->endtime);
						ostrftime(buf, MINMALLOC, listbox->param2, loctime);
						tmpstr = ostrcat(tmpstr, buf, 1, 0);
						tmpstr = ostrcat(tmpstr, " - ", 1, 0);
						tmpstr = ostrcat(tmpstr, epgnode->title, 1, 0);
						changetext2(chnode, tmpstr);
						free(tmpstr); tmpstr = NULL;
						chnode->textposx2 = TEXTPOSEPG;					
						chnode->handle1 = (char*) epgnode;
					}
					chnode->handle = (char*) tmpchannel;
					if(channelnottunable(tmpchannel) == 1)
						chnode->deaktivcol = convertcol("deaktivcol");
				}
			}
		}
		tmpchannel = tmpchannel->next;
	}
	free(buf);
	return treffer;
}

void drawchannellistmepg(struct skin* multiepg, int list, struct skin* listbox)
{
	status.markedchannel = NULL;

	if(list == ALLCHANNEL || list == SATCHANNEL || list == PROVIDERCHANNEL || list == AZCHANNEL || list == BOUQUETCHANNEL)
	{
		if(listbox->select == NULL)
			drawscreen(multiepg, 2, 0);

		if(listbox->select != NULL)
			status.markedchannel = (struct channel*)listbox->select->handle;
	}
	drawscreen(multiepg, 0, 0);
}

void screenmultiepg(struct channel* chnode, struct epg* epgnode, int flag)
{
	int rcret = 0, ret = 0, epgscreenconf = 0;
	struct skin* multiepg = getscreen("multiepg");
	struct skin* listbox = getscreennode(multiepg, "listbox");
	struct skin* b2 = getscreennode(multiepg, "b2");
	struct skin* b3 = getscreennode(multiepg, "b3");
	struct skin* b4 = getscreennode(multiepg, "b4");
	
	int list = ALLCHANNEL;
	char* tmpstr = NULL, *tmpstr1 = NULL;
	void* aktlist = NULL;
	int epgnr = 0;
	int character = 0;
	struct sat* satnode = NULL;
	struct provider* providernode = NULL;
	int nochanneltitle = getskinconfigint("nochanneltitle", NULL);

	epgscreenconf = getconfigint("epg_screen", NULL);

	if(status.servicetype == 0)
		tmpstr = getconfig("channellist", NULL);
	else
		tmpstr = getconfig("rchannellist", NULL);
	if(ostrncmp("(BOUQUET)-", tmpstr, 10) == 0 && strlen(tmpstr) > 10)
	{
		struct mainbouquet* mainbouquetnode = NULL;
		mainbouquetnode = getmainbouquet(tmpstr + 10);
		if(mainbouquetnode != NULL && mainbouquetnode->bouquet != NULL)
		{
			tmpstr1 = ostrcat(tmpstr1, _("MULTI EPG - Bouquets"), 0, 0);
			tmpstr1 = ostrcat(tmpstr1, " - ", 1, 0);
			tmpstr1 = ostrcat(tmpstr1, tmpstr + 10, 1, 0);
			if(nochanneltitle == 0) changetitle(multiepg, tmpstr1);
			free(tmpstr1); tmpstr1 = NULL;
			list = BOUQUETCHANNEL;
			aktlist = (void*)mainbouquetnode;
			showbouquetmepgchannel(multiepg, listbox, mainbouquetnode->bouquet, mainbouquetnode, epgnr);
			selectchannelmepg(listbox);
		}
	}
	else if(ostrncmp("(A-Z)-", tmpstr, 6) == 0 && strlen(tmpstr) > 6)
	{
		tmpstr1 = ostrcat(tmpstr1, _("MULTI EPG - Channel"), 0, 0);
		tmpstr1 = ostrcat(tmpstr1, " - ", 1, 0);
		tmpstr1 = ostrcat(tmpstr1, tmpstr + 6, 1, 0);
		if(nochanneltitle == 0) changetitle(multiepg, tmpstr1);
		free(tmpstr1); tmpstr1 = NULL;
		list = AZCHANNEL;
		character = (int)tmpstr[6];
		aktlist = (void*)(int)tmpstr[6];
		showazmepgchannel(multiepg, listbox, character, epgnr);
		selectchannelmepg(listbox);
	}
	else if(ostrncmp("(SAT)-", tmpstr, 6) == 0 && strlen(tmpstr) > 6)
	{
		tmpstr1 = ostrcat(tmpstr1, _("MULTI EPG - Satellites"), 0, 0);
		tmpstr1 = ostrcat(tmpstr1, " - ", 1, 0);
		tmpstr1 = ostrcat(tmpstr1, tmpstr + 6, 1, 0);
		if(nochanneltitle == 0) changetitle(multiepg, tmpstr1);
		free(tmpstr1); tmpstr1 = NULL;
		satnode = getsat(tmpstr + 6);
		list = SATCHANNEL;
		aktlist = (void*)satnode;
		showsatmepgchannel(multiepg, listbox, satnode, epgnr);
		selectchannelmepg(listbox);
	}
	else if(ostrncmp("(PROVIDER)-", tmpstr, 11) == 0 && strlen(tmpstr) > 6)
	{
		tmpstr1 = ostrcat(tmpstr1, _("MULTI EPG - Provider"), 0, 0);
		tmpstr1 = ostrcat(tmpstr1, " - ", 1, 0);
		tmpstr1 = ostrcat(tmpstr1, tmpstr + 11, 1, 0);
		if(nochanneltitle == 0) changetitle(multiepg, tmpstr1);
		free(tmpstr1); tmpstr1 = NULL;
		providernode = getproviderbyname(tmpstr + 11);
		list = PROVIDERCHANNEL;
		aktlist = (void*)providernode;
		showprovidermepgchannel(multiepg, listbox, providernode, epgnr);
		selectchannelmepg(listbox);
	}
	else
	{
		if(nochanneltitle == 0) changetitle(multiepg, _("MULTI EPG - All Channels"));
		list = ALLCHANNEL;
		showallmepgchannel(multiepg, listbox, epgnr);
		selectchannelmepg(listbox);
	}

	if(flag == 0 && epgscreenconf == 2)
	{
		b2->hidden = NO;
		b3->hidden = NO;
		b4->hidden = NO;
	}
	else
	{
		b2->hidden = YES;
		b3->hidden = YES;
		b4->hidden = YES;
	}

	tmpstr = NULL;
	drawscreen(multiepg, 2, 0);
	addscreenrc(multiepg, listbox);

	drawchannellistmepg(multiepg, list, listbox);

	while(1)
	{
		rcret = waitrc(multiepg, 0, 2);

		if((rcret == getrcconfigint("rcexit", NULL)) || (rcret == getrcconfigint("rcepg", NULL))) break;
		//if(rcret == getrcconfigint("rcinfo", NULL)) break;
		if(rcret == getrcconfigint("rcok", NULL))
		{
			servicecheckret(servicestart((struct channel*)listbox->select->handle, NULL, NULL, 0), 0);
			break;
		}
		if(rcret == getrcconfigint("rcinfo", NULL))
		{
			if(listbox->select != NULL)
			{
				clearscreen(multiepg);
				screenepg((struct channel*)listbox->select->handle, (struct epg*)listbox->select->handle1, 1);
				drawscreen(multiepg, 0, 0);
			}
		}
		
		if(flag == 0 && epgscreenconf == 2 && rcret == getrcconfigint("rcgreen", NULL))
		{
			if(listbox->select != NULL)
			{
				clearscreen(multiepg);
				screenepg((struct channel*)listbox->select->handle, (struct epg*)listbox->select->handle1, 0);
				//drawscreen(multiepg, 0, 0);
				break;
			}
		}
		if(flag == 0 && epgscreenconf == 2 && rcret == getrcconfigint("rcyellow", NULL))
		{
			if(listbox->select != NULL)
			{
				clearscreen(multiepg);
				screensingleepg((struct channel*)listbox->select->handle, NULL, 0);
				//drawscreen(multiepg, 0, 0);
				break;
			}
		}
		if(flag == 0 && epgscreenconf == 2 && rcret == getrcconfigint("rcblue", NULL))
		{
			if(listbox->select != NULL)
			{
				clearscreen(multiepg);
				screengmultiepg((struct channel*)listbox->select->handle, NULL, 0);
				//drawscreen(multiepg, 0, 0);
				break;
			}
		}

		if(rcret == getrcconfigint("rcff", NULL))
		{
			epgnr++;

			if(list == BOUQUETCHANNEL)
			{
				if(showbouquetmepgchannel(multiepg, listbox, ((struct mainbouquet*)aktlist)->bouquet, (struct mainbouquet*)aktlist, epgnr) == 0)
				{
					epgnr--;
					if(epgnr < 0)
						epgnr = 0;
					else
						showbouquetmepgchannel(multiepg, listbox, ((struct mainbouquet*)aktlist)->bouquet, (struct mainbouquet*)aktlist, epgnr);

				}
			}
			else if(list == ALLCHANNEL)
			{
				if(showallmepgchannel(multiepg, listbox, epgnr) == 0)
				{
					epgnr--;
					if(epgnr < 0)
						epgnr = 0;
					else
						showallmepgchannel(multiepg, listbox, epgnr);
				}
			}
			else if(list == AZCHANNEL)
			{
				if(showazmepgchannel(multiepg, listbox, character, epgnr) == 0)
				{
					epgnr--;
					if(epgnr < 0)
						epgnr = 0;
					else
						showazmepgchannel(multiepg, listbox, character, epgnr);
				}
			}
			else if(list == SATCHANNEL)
			{
				if(showsatmepgchannel(multiepg, listbox, satnode, epgnr) == 0)
				{
					epgnr--;
					if(epgnr < 0)
						epgnr = 0;
					else
						showsatmepgchannel(multiepg, listbox, satnode, epgnr);
				}
			}
			else if(list == PROVIDERCHANNEL)
			{
				if(showprovidermepgchannel(multiepg, listbox, providernode, epgnr) == 0)
				{
					epgnr--;
					if(epgnr < 0)
						epgnr = 0;
					else
						showprovidermepgchannel(multiepg, listbox, providernode, epgnr);
				}
			}
			drawscreen(multiepg, 0, 0);
			continue;
		}

		if(rcret == getrcconfigint("rcfr", NULL))
		{
			epgnr--;
			if(epgnr < 0) epgnr = 0;

			if(list == BOUQUETCHANNEL)
				showbouquetmepgchannel(multiepg, listbox, ((struct mainbouquet*)aktlist)->bouquet, (struct mainbouquet*)aktlist, epgnr);
			else if(list == ALLCHANNEL)
				showallmepgchannel(multiepg, listbox, epgnr);
			else if(list == AZCHANNEL)
				showazmepgchannel(multiepg, listbox, character, epgnr);
			else if(list == SATCHANNEL)
				showsatmepgchannel(multiepg, listbox, satnode, epgnr);
			else if(list == PROVIDERCHANNEL)
				showprovidermepgchannel(multiepg, listbox, providernode, epgnr);
			drawscreen(multiepg, 0, 0);
			continue;
		}

		if(rcret == getrcconfigint("rcred", NULL) && listbox->select != NULL)
		{
			clearscreen(multiepg);
			ret = addrecepg((struct channel*)listbox->select->handle, readepgdesc((struct channel*)listbox->select->handle, epgnr), NULL);
			drawscreen(multiepg, 0, 0);
			continue;
		}

		drawscreen(multiepg, 0, 0);
	}

	status.markedchannel = NULL;
	status.markmodus = 0;
	delmarkedscreennodes(multiepg, 1);
	delownerrc(multiepg);
	clearscreen(multiepg);
}

#endif
