#ifndef BGDOWNLOAD_H
#define BGDOWNLOAD_H

//flag 0: no msg
//flag 1: with msg
void delbgdownload(int nr, int flag)
{
	if(nr >= MAXBGDOWNLOAD) return;
	
	int count = 0;
	struct skin* load = getscreen("loading");
	struct download* dnode = bgdownload[nr];
	
	if(dnode != NULL)
	{
		if(dnode->connfd > -1)
		{
			if(flag == 1) drawscreen(load, 0, 0);
			sockclose(&dnode->connfd);
			while(dnode->ret < 0 && count < 10)
			{
				count++;
				sleep(1);
			}
			if(flag == 1) clearscreen(load);
		}
		
		free(dnode->host); dnode->host = NULL;
		free(dnode->page); dnode->page = NULL;
		free(dnode->filename); dnode->filename = NULL;
		free(dnode->auth); dnode->auth = NULL;
		
		dnode->port = -1;
		dnode->ret = -1;
		dnode->timeout = 0;
		dnode->flag = 0;
		
		if(count < 10)
		{
			free(dnode);
			dnode = NULL;
		}
		else
			addoldentry((void*)dnode, 1, time(NULL) + 7200, NULL);
		
		bgdownload[nr] = NULL;
	}
}

void freebgdownload()
{
	int i = 0;
	
	for(i = 0; i < MAXBGDOWNLOAD; i++)
		delbgdownload(i, 0);
}

//flag 0: with play
//flag 1: without play
void screenbgdownload(int flag)
{
	int rcret = -1, i = 0;
	struct skin* screenbgdownload = getscreen("bgdownload");
	struct skin* listbox = getscreennode(screenbgdownload, "listbox");
	struct skin* progress = getscreennode(screenbgdownload, "progress");
	struct skin* b4 = getscreennode(screenbgdownload, "b4");;
	struct skin* tmp = NULL;
	char* tmpstr = NULL;

	if(flag == 0)
		b4->hidden = NO;
	else
		b4->hidden = YES;

	for(i = 0; i < MAXBGDOWNLOAD; i++)
	{
		tmp = addlistbox(screenbgdownload, listbox, NULL, 1);
		if(tmp != NULL)
		{
			if(bgdownload[i] != NULL)
			{
				if(bgdownload[i]->ret == 0)
					tmpstr = ostrcat(tmpstr, "OK - ", 1, 0);
				else if(bgdownload[i]->ret == 1)
					tmpstr = ostrcat(tmpstr, "ERR - ", 1, 0);
				tmpstr = ostrcat(tmpstr, bgdownload[i]->filename, 1, 0);
				changetext(tmp, tmpstr);
				free(tmpstr); tmpstr = NULL;
				tmp->handle = (void*)(i + 1);
			}
			else
			{
				changetext(tmp, _("unknown"));
				tmp->handle = (void*)(i + 1);
			}
			
			tmp->textposx = progress->width + 10;
			struct skin* tmp1 = NULL;
			tmp1 = addlistbox(screenbgdownload, tmp, tmp, 2);
			if(tmp1 != NULL)
			{
				tmp1->progresscol = progress->progresscol;
				tmp1->posy = progress->posy;
				tmp1->width = progress->width;
				tmp1->height = progress->height;
				tmp1->bordersize = progress->bordersize;
				tmp1->bordercol = progress->bordercol;
				tmp1->prozwidth = 0;
				if(bgdownload[i] != NULL)
					tmp1->progresssize = bgdownload[i]->proz;
				else
					tmp1->progresssize = 0;		
				tmp1->handle = (void*)(i + 1);
			}
		}
	}

	drawscreen(screenbgdownload, 0, 0);
	addscreenrc(screenbgdownload, listbox);

	while(1)
	{
		rcret = waitrc(screenbgdownload, 1000, 0);

		if(rcret == RCTIMEOUT)
		{
			tmp = listbox;
			while(tmp != NULL)
			{
				if(tmp->del > 0)
				{
					int nr = ((int)tmp->handle) - 1;
					if(nr > -1 && nr < MAXBGDOWNLOAD)
					{
						struct download* dnode = bgdownload[nr];
						if(dnode != NULL)
						{
							if(tmp->del == 1)
							{
								if(dnode->ret == 0)
									tmpstr = ostrcat(tmpstr, "OK - ", 1, 0);
								else if(dnode->ret == 1)
									tmpstr = ostrcat(tmpstr, "ERR - ", 1, 0);
								tmpstr = ostrcat(tmpstr, dnode->filename, 1, 0);
								changetext(tmp, tmpstr);
								free(tmpstr); tmpstr = NULL;
							}
							if(tmp->del == 2)
								tmp->progresssize = dnode->proz;
						}
						else
						{
							if(tmp->del == 1)
								changetext(tmp, _("unknown"));
							if(tmp->del == 2)
								tmp->progresssize = 0;
						}
					}
				}
				tmp = tmp->next;
			}
		}
		drawscreen(screenbgdownload, 0, 0);
		
		if(rcret == getrcconfigint("rcexit", NULL)) break;
		if(rcret == getrcconfigint("rcok", NULL)) break;
		
		if(rcret == getrcconfigint("rcred", NULL) && listbox->select != NULL && listbox->select->handle != NULL) //stop download
		{
			int nr = ((int)listbox->select->handle) - 1;
			if(nr > -1 && nr < MAXBGDOWNLOAD)
			{
				struct download* dnode = bgdownload[nr];
				if(dnode != NULL && dnode->ret == -1)
				{
					if(textbox(_("Message"), _("Really stop download ?"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 0, 0) == 1)
						delbgdownload(nr, 1);
					drawscreen(screenbgdownload, 0, 0);
				}
			}
		}
		
		if(flag == 0 && rcret == getrcconfigint("rcyellow", NULL) && listbox->select != NULL && listbox->select->handle != NULL) //play download
		{
			int nr = ((int)listbox->select->handle) -1;
			if(nr > -1 && nr < MAXBGDOWNLOAD)
			{
				struct download* dnode = bgdownload[nr];
				if(dnode != NULL && dnode->flag == 1)
				{
					if(status.mcaktiv == 0)
					{
						int ret = servicestop(status.aktservice, 1, 1);
						if(ret == 1) return;
					}
					screenplay(dnode->filename, dnode->filename, 2, 0);
					if(status.mcaktiv == 0)
						servicecheckret(servicestart(status.lastservice->channel, NULL, NULL, 0), 0);
					drawscreen(screenbgdownload, 0, 0);
				}
			}
		}
	}

	delmarkedscreennodes(screenbgdownload, 2);
	delmarkedscreennodes(screenbgdownload, 1);
	delownerrc(screenbgdownload);
	clearscreen(screenbgdownload);
}

//flag 0: normal download
//flag 1: playable download
int startbgdownload(char* host, char* page, int port, char* filename, char* auth, int timeout, int flag)
{
	int i = 0;
	struct download* dnode = NULL;

	if(filename == NULL)
	{
		err("filename = NULL");
		return 1;
	}
	
	for(i = 0; i < MAXBGDOWNLOAD; i++)
	{
		if(bgdownload[i] == NULL) 
			break; //found unused slot
		else if(bgdownload[i]->ret >= 0)
		{
			delbgdownload(i, 0);
			break; //found unused slot
		}
	}
	
	if(i >= MAXBGDOWNLOAD) return 1;

	dnode = calloc(1, sizeof(struct download));
	if(dnode == NULL)
	{
		err("no mem");
		return 1;
	}
	
	bgdownload[i] = dnode;
	dnode->host = ostrcat(host, NULL, 0, 0);
	dnode->page = ostrcat(page, NULL, 0, 0);
	dnode->port = port;
	dnode->filename = ostrcat(filename, NULL, 0, 0);
	dnode->auth = ostrcat(auth, NULL, 0, 0);
	dnode->connfd = -1;
	dnode->ret = -1;
	dnode->timeout = timeout;
	dnode->flag = flag;
	
	addtimer(&gethttpstructmsg, START, 1000, 1, (void*)dnode, NULL, NULL);

	return 0;
}

#endif
