#ifndef BGDOWNLOAD_H
#define BGDOWNLOAD_H

void delbgdownload(int nr)
{
	if(nr >= MAXBGDOWNLOAD) return;
	
	struct download* dnode = bgdownload[nr];
	
	if(dnode != NULL)
	{
		free(dnode->host); dnode->host = NULL;
		free(dnode->page); dnode->page = NULL;
		free(dnode->filename); dnode->filename = NULL;
		free(dnode->auth); dnode->auth = NULL;
		
		sockclose(&dnode->connfd);
		dnode->port = -1;
		dnode->ret = -1;
		
		free(dnode); dnode = NULL;
		bgdownload[nr] = NULL;
	}
}

void freebgdownload()
{
	int i = 0;
	
	for(i = 0; i < MAXBGDOWNLOAD; i++)
		delbgdownload(i);
}

void screenbgdownload()
{
	debug(1000, "in");
	int rcret = -1, i = 0;
	struct skin* screenbgdownload = getscreen("bgdownload");
	struct skin* listbox = getscreennode(screenbgdownload, "listbox");
	struct skin* progress = getscreennode(screenbgdownload, "progress");
	struct skin* tmp = NULL;
	struct download* dnode = NULL;

	for(i = 0; i < MAXBGDOWNLOAD; i++)
	{
		tmp = addlistbox(screenbgdownload, listbox, NULL, 1);
		if(tmp != NULL)
		{
			if(bgdownload[i] != NULL)
			{
				changetext(tmp, bgdownload[i]->filename);
				tmp->progresssize = dnode->proz;
				tmp->handle = (void*)i;
			}
			else
			{
				changetext(tmp, _("unknown"));
				tmp->progresssize = 0;
				tmp->handle = (void*)i;
			}
			
			tmp->textposx = progress->width + 10;
			struct skin* tmp1 = NULL;
			tmp1 = addlistbox(screenbgdownload, tmp, tmp, 1);
			if(tmp1 != NULL)
			{
				tmp1->progresscol = progress->progresscol;
				tmp1->posy = progress->posy;
				tmp1->width = progress->width;
				tmp1->height = progress->height;
				tmp1->bordersize = progress->bordersize;
				tmp1->bordercol = progress->bordercol;
				tmp1->prozwidth = 0;		
			}
		}
	}

	drawscreen(screenbgdownload, 0, 0);

	while(1)
	{
		rcret = waitrc(screenbgdownload, 1000, 0);

		if(rcret == RCTIMEOUT)
		{
			tmp = listbox;
			while(tmp != NULL)
			{
				if(tmp->del == 1)
				{
					int nr = (int)tmp->handle;
					if(nr < MAXBGDOWNLOAD)
					{
						struct download* dnode = bgdownload[nr];
						if(dnode != NULL)
						{
							changetext(tmp, dnode->filename);
							tmp->progresssize = dnode->proz;
						}
					}
			  }
				tmp = tmp->next;
			}
		}
		drawscreen(screenbgdownload, 0, 0);
		
		if(rcret == getrcconfigint("rcexit", NULL)) break;
		
		if(rcret == getrcconfigint("rcred", NULL) && listbox->select != NULL && listbox->select->handle != NULL) //stop download
		{
			int nr = (int)listbox->select->handle;
			//textbox
			delbgdownload(nr);
		}
		
		if(rcret == getrcconfigint("rcyellow", NULL) && listbox->select != NULL && listbox->select->handle != NULL) //play download
		{
			int nr = (int)listbox->select->handle;
			if(nr < MAXBGDOWNLOAD)
			{
				struct download* dnode = bgdownload[nr];
				if(dnode != NULL)
				{
					//play
				}
			}
		}
	}

	delmarkedscreennodes(screenbgdownload, 1);
	clearscreen(screenbgdownload);
	debug(1000, "out");
}

//flag 0: normal download
//flag 1: playable download
int startbgdownload(char* title, char* host, char* page, int port, char* filename, char* auth, int flag)
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
		else if(bgdownload[i]->ret > 0)
		{
			delbgdownload(i);
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
	dnode->host = host;
	dnode->page = page;
	dnode->port = port;
	dnode->filename = filename;
	dnode->auth = auth;
	dnode->connfd = -1;
	dnode->ret = -1;
	dnode->flag = flag;
	
	addtimer(&gethttpstruct, START, 1000, 1, (void*)dnode, NULL, NULL);

	return 0;
}

#endif
