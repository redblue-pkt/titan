#ifndef EPGSEARCH_H
#define EPGSEARCH_H

int searchepg(char* search, char* search1, char* search2, int searchtype, struct skin* epgsearch, struct skin* listbox)
{
	int allepg = 1, count = 0;
	char* result = NULL, *epgdesc = NULL, *tmpstr = NULL;
	char* buf = NULL;
	struct channel* chnode = channel;
	struct epg* epgnode = NULL;
	struct skin* tmp = NULL;
	struct tm *loctime = NULL;
	time_t akttime = time(NULL) + 7200;
  
	if(search == NULL || strlen(search) < 2) return 1;

	listbox->aktline = 1;
	listbox->aktpage = -1;

	buf = malloc(MINMALLOC);
	if(buf == NULL)
	{
		err("no mem");
		return 1;
	}
	
	if(textbox(_("Message"), _("Search only aktual epg ?"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 0, 0) == 1)
		allepg = 0;

	while(chnode != NULL)
	{
		epgnode = getepgakt(chnode);
		while(epgnode != NULL)
		{
			if(allepg == 0 && epgnode->starttime > akttime) break;
		
			result = NULL;
			count = 0;
			
start:
			if(searchtype == 0 || searchtype == 2 || searchtype == 3)
			{
				if(epgnode->title != NULL)
					result = ostrstrcase(epgnode->title, search);
			}
			if(searchtype == 3 && result == NULL)
			{
				if(epgnode->subtitle != NULL)
					result = ostrstrcase(epgnode->subtitle, search);
			}
			if((searchtype == 1 || searchtype == 2 || searchtype == 3) && result == NULL)
			{
				epgdesc = epgdescunzip(epgnode);
				if(epgdesc != NULL)
				{
					result = ostrstrcase(epgdesc, search);
					free(epgdesc); epgdesc = NULL;
				}
			}
			
			if(result == NULL && count == 0 && search1 != NULL)
			{
				count = 1;
				if(ostrcmp(search, search1) != 0)
				{
					search = search1;
					goto start;
				}
			}
			
			if(result == NULL && count == 1 && search2 != NULL)
			{
				count = 2;
				search = search2;
				goto start;
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

void screenepgsearch(char* text)
{
	int rcret = -1, ret = 0, genre = 0;
	struct skin* epgsearch = getscreen("epgsearch");
	struct skin* listbox = getscreennode(epgsearch, "listbox");
	char* search = NULL;
	struct skin* tmp = NULL;

	if(text != NULL)
	{
		delmarkedscreennodes(epgsearch, 1);
		searchepg(text, NULL, NULL, 0, epgsearch, listbox);
	}

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
				if(genre == 0)
					servicecheckret(servicestart((struct channel*)listbox->select->handle, NULL, NULL, 0), 0);
				else
				{
					genre = 0;
					char* s1 = ostrcat(listbox->select->text, NULL, 0, 0);
					char* s2 = ostrcat(listbox->select->handle, NULL, 0, 0);
					char* s3 = ostrcat(listbox->select->handle1, NULL, 0, 0);
				  delmarkedscreennodes(epgsearch, 1);
				  searchepg(s1, s2, s3, 3, epgsearch, listbox);
				  free(s1); s1 = NULL;
				  free(s2); s2 = NULL;
				  free(s3); s3 = NULL;
				  drawscreen(epgsearch, 0, 0);
					continue;
				}
			}
			break;
		}
    
		if(rcret == getrcconfigint("rcred", NULL))
		{
			if(listbox->select != NULL && genre == 0)
			{
				ret = addrecepg((struct channel*)listbox->select->handle, (struct epg*)listbox->select->handle1, NULL);
				drawscreen(epgsearch, 0, 0);
				continue;
			}
		}
    
		if(rcret == getrcconfigint("rcgreen", NULL))
		{
			search = textinput(_("Search"), NULL);
			if(search != NULL)
			{
				genre = 0;
				delmarkedscreennodes(epgsearch, 1);
				searchepg(search, NULL, NULL, 0, epgsearch, listbox);
				free(search); search = NULL;
			}
			drawscreen(epgsearch, 0, 0);
			continue;
		}
    
		if(rcret == getrcconfigint("rcyellow", NULL))
		{
			search = textinput(_("Search"), NULL);
			if(search != NULL)
			{
				genre = 0;
				delmarkedscreennodes(epgsearch, 1);
				searchepg(search, NULL, NULL, 1, epgsearch, listbox);
				free(search); search = NULL;
			}
			drawscreen(epgsearch, 0, 0);
			continue;
		}
    
		if(rcret == getrcconfigint("rcblue", NULL))
		{
			search = textinput(_("Search"), NULL);
			if(search != NULL)
			{
				genre = 0;
				delmarkedscreennodes(epgsearch, 1);
				searchepg(search, NULL, NULL, 2, epgsearch, listbox);
				free(search); search = NULL;
			}
			drawscreen(epgsearch, 0, 0);
			continue;
		}
		
		if(rcret == getrcconfigint("rcmenu", NULL))
		{
			tmp = NULL;
			genre = 1;
			delmarkedscreennodes(epgsearch, 1);
			
			tmp = addlistbox(epgsearch, listbox, tmp, 1);
			if(tmp != NULL)
			{
				changetext(tmp, _("Action"));
				tmp->handle = "Action";
				tmp->handle1 = NULL;
			}
			
			tmp = addlistbox(epgsearch, listbox, tmp, 1);
			if(tmp != NULL)
			{ 
				changetext(tmp, _("Animation"));
				tmp->handle = "Animation";
				tmp->handle1 = NULL;
			}
			
			tmp = addlistbox(epgsearch, listbox, tmp, 1);
			if(tmp != NULL)
			{
				changetext(tmp, _("History"));
				tmp->handle = "History";
				tmp->handle1 = NULL;
			}
			
			tmp = addlistbox(epgsearch, listbox, tmp, 1);
			if(tmp != NULL)
			{
				changetext(tmp, _("Doku"));
				tmp->handle = "Doku";
				tmp->handle1 = NULL;
			}
			
			tmp = addlistbox(epgsearch, listbox, tmp, 1);
			if(tmp != NULL)
			{
				changetext(tmp, _("Thriller"));
				tmp->handle = "Thriller";
				tmp->handle1 = NULL;
			}
			
			tmp = addlistbox(epgsearch, listbox, tmp, 1);
			if(tmp != NULL)
			{
				changetext(tmp, _("Child"));
				tmp->handle = "Child";
				tmp->handle1 = NULL;
			}
			
			tmp = addlistbox(epgsearch, listbox, tmp, 1);
			if(tmp != NULL)
			{
				changetext(tmp, _("Fantasy"));
				tmp->handle = "Fantasy";
				tmp->handle1 = NULL;
			}
			
			tmp = addlistbox(epgsearch, listbox, tmp, 1);
			if(tmp != NULL)
			{
				changetext(tmp, _("Horror"));
				tmp->handle = "Horror";
				tmp->handle1 = NULL;
			}
			
			tmp = addlistbox(epgsearch, listbox, tmp, 1);
			if(tmp != NULL)
			{
				changetext(tmp, _("Comedy"));
				tmp->handle = "Comedy";
				tmp->handle1 = NULL;
			}
			
			tmp = addlistbox(epgsearch, listbox, tmp, 1);
			if(tmp != NULL)
			{
				changetext(tmp, _("War"));
				tmp->handle = "War";
				tmp->handle1 = NULL;
			}
			
			tmp = addlistbox(epgsearch, listbox, tmp, 1);
			if(tmp != NULL)
			{
				changetext(tmp, _("Scifi"));
				tmp->handle = "Scifi";
				tmp->handle1 = "Fiction";
			}
			
			tmp = addlistbox(epgsearch, listbox, tmp, 1);
			if(tmp != NULL)
			{
				changetext(tmp, _("Sport"));
				tmp->handle = "Sport";
				tmp->handle1 = NULL;
			}
			
			tmp = addlistbox(epgsearch, listbox, tmp, 1);
			if(tmp != NULL)
			{
				changetext(tmp, _("Western"));
				tmp->handle = "Western";
				tmp->handle1 = NULL;
			}
			
			tmp = addlistbox(epgsearch, listbox, tmp, 1);
			if(tmp != NULL)
			{
				changetext(tmp, _("Eastern"));
				tmp->handle = "Eastern";
				tmp->handle1 = NULL;
			}
			
			tmp = addlistbox(epgsearch, listbox, tmp, 1);
			if(tmp != NULL)
			{
				changetext(tmp, _("Music"));
				tmp->handle = "Music";
				tmp->handle1 = NULL;
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
