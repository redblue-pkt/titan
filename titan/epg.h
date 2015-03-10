#ifndef EPG_H
#define EPG_H

int screensingleepg(struct channel* chnode, struct epg* epgnode, int flag)
{
	int rcret = 0, ret = 0, epgscreenconf = 0, end = 0;
	struct skin* singleepg = getscreen("singleepg");
	struct skin* channelname = getscreennode(singleepg, "channelname");
	struct skin* epgdesc = getscreennode(singleepg, "epgdesc");
	struct skin* listbox = getscreennode(singleepg, "listbox");
	struct skin* b2 = getscreennode(singleepg, "b2");
	struct skin* b3 = getscreennode(singleepg, "b3");
	struct skin* b4 = getscreennode(singleepg, "b4");
	struct skin* tmp = NULL;
	char* tmpstr = NULL, *buf = NULL;
	struct tm *loctime = NULL;
	struct epg* tmpepg = NULL;

	epgscreenconf = getconfigint("epg_screen", NULL);
	listbox->aktline = 1;
	listbox->aktpage = -1;

	if(chnode == NULL) chnode = status.aktservice->channel;
	if(chnode == NULL) return 2;
	if(epgnode == NULL) epgnode = getepgakt(chnode);
	if(epgnode == NULL) epgnode = getepgnext(chnode);
	tmpepg = epgnode;

	status.epgchannel = chnode;

	changetext(channelname, chnode->name);
	tmpstr = epgdescunzip(epgnode);
	changetext(epgdesc, tmpstr);
	free(tmpstr); tmpstr = NULL;

start:
	buf = malloc(MINMALLOC);
	if(buf == NULL)
	{
		err("no mem");
		return 2;
	}

	tmp = NULL;
	delmarkedscreennodes(singleepg, 1);
	delownerrc(singleepg);
	epgnode = tmpepg;

	while(epgnode != NULL)
	{
		tmp = addlistbox(singleepg, listbox, tmp, 1);
		if(tmp != NULL)
		{
			tmp->type = MULTIPROGRESSBAR;
			tmp->progresscol = listbox->progresscol;
			tmp->epgrecord = getepgrecord(chnode, epgnode);

			loctime = localtime(&epgnode->starttime);
			ostrftime(buf, MINMALLOC, listbox->param1, loctime);
			buf = translate_time(buf, 1);
			changetext(tmp, buf);
			changetext2(tmp, epgnode->title);
			tmp->handle = (char*)epgnode;
		}
		epgnode = epgnode->next;
	}

	free(buf); buf = NULL;

	if(flag == 0 && epgscreenconf == 1)
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

	drawscreen(singleepg, 0, 0);
	addscreenrc(singleepg, listbox);

	while(1)
	{
		status.epgchannel = chnode;
		rcret = waitrc(singleepg, 0, 2);
		if((rcret == getrcconfigint("rcexit", NULL)) || (rcret == getrcconfigint("rcepg", NULL))) break;
		if(rcret == getrcconfigint("rcinfo", NULL)) break;
		if(rcret == getrcconfigint("rcok", NULL))
		{	
			int ret = 0;
			if(status.servicetype == 0)
				ret = servicestart(chnode, getconfig("channellist", NULL), NULL, 0);
			else
				ret = servicestart(chnode, getconfig("rchannellist", NULL), NULL, 0);
			if(ret == 20) writeconfigtmp();
			servicecheckret(ret, 0);
			end = 1;
			break;
		}
		if(flag == 0 && epgscreenconf == 1 && rcret == getrcconfigint("rcgreen", NULL))
		{
			if(listbox->select != NULL)
			{
				clearscreen(singleepg);
				end = screenepg(chnode, (struct epg*)listbox->select->handle, 0);
				//drawscreen(singleepg, 0, 0);
				break;
			}
		}
		if(flag == 0 && epgscreenconf == 1 && rcret == getrcconfigint("rcyellow", NULL))
		{
			clearscreen(singleepg);
			end = screenmultiepg(chnode, NULL, 0);
			//drawscreen(singleepg, 0, 0);
			break;
		}
		if(flag == 0 && epgscreenconf == 1 && rcret == getrcconfigint("rcblue", NULL))
		{
			clearscreen(singleepg);
			end = screengmultiepg(chnode, NULL, 0);
			//drawscreen(singleepg, 0, 0);
			break;
		}
		if(rcret == getrcconfigint("rc0", NULL))
		{
			if(listbox->select != NULL && listbox->select->handle != NULL)
			{
				screenepgsearch(((struct epg*)listbox->select->handle)->title);
				resettvpic();
				drawscreen(singleepg, 0, 0);
				continue;
			}
		}
		if(listbox->select != NULL)
		{
			tmpstr = epgdescunzip((struct epg*)listbox->select->handle);
			changetext(epgdesc, tmpstr);
			free(tmpstr); tmpstr = NULL;
			if(rcret == getrcconfigint("rcred", NULL))
			{
				clearscreen(singleepg);
				ret = addrecepg(chnode, (struct epg*)listbox->select->handle, NULL);
				goto start;
			}
			drawscreen(singleepg, 0, 0);
		}
	}

	status.epgchannel = NULL;
	delmarkedscreennodes(singleepg, 1);
	delownerrc(singleepg);
	clearscreen(singleepg);

	return end;
}

int screenepg(struct channel* chnode, struct epg* epgnode, int flag)
{
	int rcret = 0, ret = 0, epgscreenconf = 0, min = 0, end = 0;
	struct skin* screenepg = getscreen("epg");
	struct skin* channelname = getscreennode(screenepg, "channelname");
	struct skin* channelnr = getscreennode(screenepg, "channelnr");
	struct skin* epgtitle = getscreennode(screenepg, "epgtitle");
	struct skin* epgsubtitle = getscreennode(screenepg, "epgsubtitle");
	struct skin* epgstart = getscreennode(screenepg, "epgstart");
	struct skin* epgend = getscreennode(screenepg, "epgend");
	struct skin* epgtimeremaining = getscreennode(screenepg, "epgtimeremaining");
	struct skin* epgdesc = getscreennode(screenepg, "epgdesc");
	struct skin* rectimeline = getscreennode(screenepg, "rectimeline");
	struct skin* b2 = getscreennode(screenepg, "b2");
	struct skin* b3 = getscreennode(screenepg, "b3");
	struct skin* b4 = getscreennode(screenepg, "b4");
	struct skin* b5 = getscreennode(screenepg, "b5");
	struct tm* loctime = NULL;
	char* tmpstr = NULL, *buf = NULL;
	struct tmdb* (*startplugin)(char*, char*, int);

	if(getconfigint("epgbutton", NULL) == 0)
		changetext(b5, _("Single (EPG)"));
	else
		changetext(b5, _("Multi (EPG)"));

	if(chnode == NULL) chnode = status.aktservice->channel;
	if(chnode == NULL) return 2;
	if(epgnode == NULL) epgnode = getepgakt(chnode);
	if(epgnode == NULL) epgnode = getepgnext(chnode);

	status.epgchannel = chnode;
	epgscreenconf = getconfigint("epg_screen", NULL);

start:
	epgdesc->aktpage = 1;
	freeepgrecord(&rectimeline->epgrecord);
	changetext(channelname, NULL);
	changetext(epgtitle, NULL);
	changetext(epgsubtitle, NULL);
	changetext(epgdesc, NULL);
	changetext(epgstart, NULL);
	changetext(epgend, NULL);
	changetext(epgtimeremaining, NULL);
	changetext(channelnr, NULL);

	if(flag == 0 && epgscreenconf == 0)
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

	if(epgnode != NULL)
	{
		buf = malloc(MINMALLOC);
		if(buf == NULL)
		{
			err("no memory");
			return 2;
		}

		changetext(channelname, chnode->name);
		changetext(epgtitle, epgnode->title);
		changetext(epgsubtitle, epgnode->subtitle);
		tmpstr = epgdescunzip(epgnode);
		changetext(epgdesc, tmpstr);
		free(tmpstr); tmpstr = NULL;

		loctime = olocaltime(&epgnode->starttime);
		if(loctime != NULL)
		{
			ostrftime(buf, MINMALLOC, epgstart->param1, loctime);
			buf = translate_time(buf, 0);
			changetext(epgstart, buf);
			free(loctime); loctime = NULL;
		}
		loctime = olocaltime(&epgnode->endtime);
		if(loctime != NULL)
		{
			ostrftime(buf, MINMALLOC, epgend->param1, loctime);
			buf = translate_time(buf, 0);
			changetext(epgend, buf);
			free(loctime); loctime = NULL;
		}

		if(epgnode->starttime <= time(NULL))
			min = ((epgnode->endtime - (epgnode->endtime % 60)) - (time(NULL) - (time(NULL) % 60))) / 60;
		else
			min = ((epgnode->endtime - (epgnode->endtime % 60)) - (epgnode->starttime - (epgnode->starttime % 60))) / 60;
		if(min < 0) min = 0;
		snprintf(buf, MINMALLOC, epgtimeremaining->param1, min);
		changetext(epgtimeremaining, buf);

		free(buf); buf = NULL;

		tmpstr = getchannelnr(NULL, chnode);
		changetext(channelnr, tmpstr);
		free(tmpstr); tmpstr = NULL;

		rectimeline->epgrecord = getepgrecord(chnode, epgnode);
	}

	drawscreen(screenepg, 0, 0);
	addscreenrc(screenepg, epgdesc);

	while(1)
	{
		status.epgchannel = chnode;
		rcret = waitrc(screenepg, 0, 0);
		if(rcret == getrcconfigint("rcexit", NULL)) break;
		if(rcret == getrcconfigint("rcinfo", NULL)) break;
		if(rcret == getrcconfigint("rcok", NULL))
		{
			if(channelnottunable(chnode) == 0)
			{
				int ret = 0;
				if(status.servicetype == 0)
					ret = servicestart(chnode, getconfig("channellist", NULL), NULL, 0);
				else
					ret = servicestart(chnode, getconfig("rchannellist", NULL), NULL, 0);
				if(ret == 20) writeconfigtmp();
				servicecheckret(ret, 0);
				end = 1;
				break;
			}
		}
		if(rcret == getrcconfigint("rcright", NULL))
		{
			if(epgnode != NULL && epgnode->next != NULL)
				epgnode = epgnode->next;
			goto start;
		}
		if(rcret == getrcconfigint("rcleft", NULL))
		{
			if(epgnode != NULL && epgnode->prev != NULL && epgnode->endtime > time(NULL))
				epgnode = epgnode->prev;
			goto start;
		}
		if(flag == 0 && epgscreenconf == 0 && rcret == getrcconfigint("rcgreen", NULL))
		{
			clearscreen(screenepg);
			end = screensingleepg(chnode, NULL, 0);
			//drawscreen(screenepg, 0, 0);
			break;
		}
		if(flag == 0 && epgscreenconf == 0 && rcret == getrcconfigint("rcyellow", NULL))
		{
			clearscreen(screenepg);
			end = screenmultiepg(chnode, NULL, 0);
			//drawscreen(screenepg, 0, 0);
			break;
		}
		if(flag == 0 && epgscreenconf == 0 && rcret == getrcconfigint("rcblue", NULL))
		{
			clearscreen(screenepg);
			end = screengmultiepg(chnode, NULL, 0);
			//drawscreen(screenepg, 0, 0);
			break;
		}
		if(rcret == getrcconfigint("rc0", NULL))
		{
			screenepgsearch(epgnode->title);
			resettvpic();
			drawscreen(screenepg, 0, 0);
			continue;
		}
		if(rcret == getrcconfigint("rcred", NULL))
		{
			clearscreen(screenepg);
			ret = addrecepg(chnode, epgnode, NULL);
			//drawscreen(screenepg, 0, 0);
			goto start;
		}
		if(rcret == getrcconfigint("rcepg", NULL))
		{
			if(getconfigint("epgbutton", NULL) == 0)
			{
				clearscreen(screenepg);
				end = screensingleepg(chnode, NULL, 0);
				//drawscreen(screenepg, 0, 0);
				break;
			}
			else
			{
				clearscreen(screenepg);
				end = screenmultiepg(chnode, NULL, 0);
				//drawscreen(screenepg, 0, 0);
				break;
			}
		}
		if(rcret == getrcconfigint("rctext", NULL))
		{
			struct skin* pluginnode = getplugin("TMDb");

			if(pluginnode != NULL)
			{
				startplugin = dlsym(pluginnode->pluginhandle, "screentmdb");
				if(startplugin != NULL && epgnode != NULL)
				{
					clearscreen(screenepg);
					startplugin(epgnode->title, NULL, 0);
					drawscreen(screenepg, 0, 0);
				}
			}
		}
	}

	status.epgchannel = NULL;
	freeepgrecord(&rectimeline->epgrecord);
	delownerrc(screenepg);
	clearscreen(screenepg);

	return end;
}

int epgchoice(struct channel* chnode)
{
	int epgscreenconf = 0, ret = 0;

	epgscreenconf = getconfigint("epg_screen", NULL);
	if(epgscreenconf == 1)
		ret = screensingleepg(chnode, NULL, 0);
	else if(epgscreenconf == 2)
		ret = screenmultiepg(chnode, NULL, 0);
	else if(epgscreenconf == 3)
		ret = screengmultiepg(chnode, NULL, 0);
	else
		ret = screenepg(chnode, NULL, 0);

	return ret;
}

#endif
