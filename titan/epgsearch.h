#ifndef EPGSEARCH_H
#define EPGSEARCH_H

int searchepg(char* search, int searchtype, struct skin* epgsearch, struct skin* listbox)
{
	char* result = NULL, *epgdesc = NULL, *tmpstr = NULL;
	char* buf = NULL;
	struct channel* chnode = channel;
	struct epg* epgnode = NULL;
	struct skin* tmp = NULL;
	struct tm *loctime = NULL;
  
	if(search == NULL || strlen(search) < 2) return 1;

	listbox->aktline = 1;
	listbox->aktpage = -1;

	buf = malloc(MINMALLOC);
	if(buf == NULL)
	{
		err("no mem");
		return 1;
	}

	while(chnode != NULL)
	{
		epgnode = getepgakt(chnode);
		while(epgnode != NULL)
		{
			result = NULL;
			if(searchtype == 0 || searchtype == 2)
			{
				if(epgnode->title != NULL)
					result = ostrstrcase(epgnode->title, search);
			}
			if((searchtype == 1 || searchtype == 2) && result == NULL)
			{
				epgdesc = epgdescunzip(epgnode);
				if(epgdesc != NULL)
				{
					result = ostrstrcase(epgdesc, search);
					free(epgdesc); epgdesc = NULL;
				}
			}
			if(result != NULL)
			{
				tmp = addlistbox(epgsearch, listbox, tmp, 1);
				if(tmp != NULL)
				{
					tmpstr = ostrcat(chnode->name, " - ", 0, 0);
					changetext(tmp, tmpstr);
					free(tmpstr); tmpstr = NULL;

					tmpstr = ostrcat(tmpstr, epgnode->title, 1, 0);

					loctime = olocaltime(&epgnode->starttime);
					if(loctime != NULL)
						strftime(buf, MINMALLOC, "%d-%m-%Y %H:%M -", loctime);
					free(loctime); loctime = NULL;
					loctime = olocaltime(&epgnode->endtime);
					if(loctime != NULL)
						strftime(&buf[18], MINMALLOC - 19, " %H:%M", loctime);
					free(loctime); loctime = NULL;

					tmpstr = ostrcat(tmpstr, " (", 1, 0);
					tmpstr = ostrcat(tmpstr, buf, 1, 0);
					tmpstr = ostrcat(tmpstr, ")", 1, 0);

					changetext2(tmp, tmpstr);
					free(tmpstr); tmpstr = NULL;
					tmp->handle = (char*)chnode;
					tmp->handle1 = (char*)epgnode;
				}       
      
			}
			epgnode = epgnode->next;
		}
		chnode = chnode->next;
	}

	free(buf);
	return 0;
}

void screenepgsearch()
{
	int rcret = -1, ret = 0;
	struct skin* epgsearch = getscreen("epgsearch");
	struct skin* listbox = getscreennode(epgsearch, "listbox");
	char* search = NULL;

	listbox->aktline = 1;
	listbox->aktpage = -1;
  
	addscreenrc(epgsearch, listbox);
	drawscreen(epgsearch, 0, 0);
  
	while(1)
	{
		rcret = waitrc(epgsearch, 0, 0);
    
		if(rcret == getrcconfigint("rcexit", NULL)) break;
		if(rcret == getrcconfigint("rcok", NULL))
		{
			if(listbox->select != NULL)
			{
				servicecheckret(servicestart((struct channel*)listbox->select->handle, NULL, NULL, 0), 0);
			}
			break;
		}
    
		if(rcret == getrcconfigint("rcred", NULL))
		{
			if(listbox->select != NULL)
			{
				ret = addrecepg((struct channel*)listbox->select->handle, (struct epg*)listbox->select->handle1, NULL);
				drawscreen(epgsearch, 0, 0);
				continue;
			}
		}
    
		if(rcret == getrcconfigint("rcgreen", NULL))
		{
			search = textinput("Search", NULL);
			if(search != NULL)
			{
				delmarkedscreennodes(epgsearch, 1);
				searchepg(search, 0, epgsearch, listbox);
				free(search); search = NULL;
			}
			drawscreen(epgsearch, 0, 0);
			continue;
		}
    
		if(rcret == getrcconfigint("rcyellow", NULL))
		{
			search = textinput("Search", NULL);
			if(search != NULL)
			{
				delmarkedscreennodes(epgsearch, 1);
				searchepg(search, 1, epgsearch, listbox);
				free(search); search = NULL;
			}
			drawscreen(epgsearch, 0, 0);
			continue;
		}
    
		if(rcret == getrcconfigint("rcblue", NULL))
		{
			search = textinput("Search", NULL);
			if(search != NULL)
			{
				delmarkedscreennodes(epgsearch, 1);
				searchepg(search, 2, epgsearch, listbox);
				free(search); search = NULL;
			}
			drawscreen(epgsearch, 0, 0);
			continue;
		}
  
	}
  
	delmarkedscreennodes(epgsearch, 1);
	delownerrc(epgsearch);
	clearscreen(epgsearch);
}

#endif
