#ifndef TITHEK_H
#define TITHEK_H

struct tithek
{
	char* title;
	char* link;
	char* pic;
	int flag;
	struct tithek* prev;
	struct tithek* next;
};
struct tithek *tithek = NULL;

struct tithek* getlasttithek(struct tithek* node)
{
	debug(1000, "in");
	struct tithek *prev = NULL;

	while(node != NULL)
	{
		prev = node;
		node = node->next;
	}

	debug(1000, "out");
	return prev;
}

struct tithek* addtithek(char *line, int count, struct tithek* last)
{
	//debug(1000, "in");
	struct tithek *newnode = NULL, *prev = NULL, *node = tithek;
	char *link = NULL, *pic = NULL, *title = NULL;
	int ret = 0;

	if(line == NULL) return NULL;

	newnode = (struct tithek*)malloc(sizeof(struct tithek));	
	if(newnode == NULL)
	{
		err("no memory");
		return NULL;
	}

	link = malloc(MINMALLOC);
	if(link == NULL)
	{
		err("no memory");
		free(newnode);
		return NULL;
	}

	pic = malloc(MINMALLOC);
	if(pic == NULL)
	{
		err("no memory");
		free(newnode);
		free(link);
		return NULL;
	}

	title = malloc(MINMALLOC);
	if(title == NULL)
	{
		err("no memory");
		free(newnode);
		free(link);
		free(pic);
		return NULL;
	}

	memset(newnode, 0, sizeof(struct tithek));

	//TODO; change title#link#pic#flag
	ret = sscanf(line, "%[^#]#%[^#]#%[^#]#%d", title, link, pic, &newnode->flag);
	if(ret != 4)
	{
		if(count > 0)
		{
			err("tithek line %d not ok or double", count);
		}
		else
		{
			err("add tithek");
		}
		free(link);
		free(pic);
		free(title);
		free(newnode);
		return NULL;
	}

	newnode->link = ostrcat(link, "", 1, 0);
	newnode->pic = ostrcat(pic, "", 1, 0);
	newnode->title = ostrcat(title, "", 1, 0);

	if(last == NULL)
	{
		while(node != NULL)
		{
			prev = node;
			node = node->next;
		}
	}
	else
	{
		prev = last;
		node = last->next;
	}

	if(prev == NULL)
		tithek = newnode;
	else
	{
		prev->next = newnode;
		newnode->prev = prev;
	}
	newnode->next = node;
	if(node != NULL) node->prev = newnode;
	
	//debug(1000, "out");
	return newnode;
}

int readtithek(const char* filename)
{
	debug(1000, "in");
	FILE *fd = NULL;
	char *fileline = NULL;
	int linecount = 0;
	struct tithek* last = NULL, *tmplast = NULL;

	fileline = malloc(MINMALLOC);
	if(fileline == NULL)
	{
		err("no memory");
		return 1;
	}

	fd = fopen(filename, "r");
	if(fd == NULL)
	{
		perr("can't open %s", filename);
		free(fileline);
		return 1;
	}

	while(fgets(fileline, MINMALLOC, fd) != NULL)
	{
		if(fileline[0] == '#' || fileline[0] == '\n')
			continue;
		if(fileline[strlen(fileline) - 1] == '\n')
			fileline[strlen(fileline) - 1] = '\0';
		if(fileline[strlen(fileline) - 1] == '\r')
			fileline[strlen(fileline) - 1] = '\0';

		linecount++;

		if(last == NULL) last = tmplast;
		last = addtithek(fileline, linecount, last);
		if(last != NULL) tmplast = last;
	}

	free(fileline);
	fclose(fd);
	return 0;
}

int deltithek(char* link)
{
	debug(1000, "in");
	int ret = 1;
	struct tithek *node = tithek, *prev = tithek;

	while(node != NULL)
	{
		if(ostrcmp(link, node->link) == 0)
		{
			ret = 0;
			if(node == tithek)
			{
				tithek = node->next;
				if(tithek != NULL)
					tithek->prev = NULL;
			}
			else
			{
				prev->next = node->next;
				if(node->next != NULL)
					node->next->prev = prev;
			}

			free(node->link);
			node->link = NULL;

			free(node->pic);
			node->pic = NULL;

			free(node->title);
			node->title = NULL;

			free(node);
			node = NULL;

			break;
		}

		prev = node;
		node = node->next;
	}

	debug(1000, "out");
	return ret;
}

void freetithek()
{
	debug(1000, "in");
	struct tithek *node = tithek, *prev = tithek;

	while(node != NULL)
	{
		prev = node;
		node = node->next;
		if(prev != NULL)
			deltithek(prev->link);
	}
	debug(1000, "out");
}

char* tithekdownload(char* link)
{
	char* ip = NULL, *pos = NULL, *path = NULL;
	char* tmpstr = NULL, *localfile = NULL;

	if(link == NULL) return NULL;

	ip = string_replace("http://", "", (char*)link, 0);

	if(ip != NULL)
		pos = strchr(ip, '/');
	if(pos != NULL)
	{
		pos[0] = '\0';
		path = pos + 1;
	}

	tmpstr = ostrcat(path, NULL, 0, 0);
	localfile = ostrcat("/tmp/", basename(tmpstr), 0, 0);

	if(! file_exists(localfile))
		gethttp(ip, path, 80, localfile, NULL, NULL);

	free(ip); ip = NULL;
	free(tmpstr); tmpstr = NULL;

	return localfile;
}

int createtithekmenu(char* titheklink, struct skin* menu, struct skin* listbox)
{
	struct skin* tmp = NULL;
	char* tithekfile = NULL;
	char* tithekpic = NULL;

	tithekfile = tithekdownload(titheklink);

	delmarkedscreennodes(menu, 1);
	freetithek();
	if(readtithek(tithekfile) != 0) return 1;

	struct tithek* titheknode = tithek;

	while(titheknode != NULL)
	{
		tmp = addlistbox(menu, listbox, tmp, 1);
		if(tmp != NULL)
		{
			tmp->type = MENU;
			tmp->textposx = 200;
			tmp->valign = MIDDLE;
			changetext(tmp, titheknode->title);
			tmp->height = 200;
			tmp->hspace = 5;
			tithekpic = tithekdownload(titheknode->pic);
			changepic(tmp, tithekpic);
			free(tithekpic); tithekpic = NULL;
			tmp->handle = (char*)titheknode;
		}
		titheknode = titheknode->next;
	}

	free(tithekfile); tithekfile = NULL;
	return 0;
}

void screentithekmenu(char* titheklink)
{
	int rcret = -1, oaktline = 1, oaktpage = -1;

	struct skin* menu = getscreen("tithekmenu");
	struct skin* listbox = getscreennode(menu, "listbox");

	if(titheklink == NULL) return;

	listbox->aktpage = -1;
	listbox->aktline = 1;

	if(createtithekmenu(titheklink, menu, listbox) != 0) return;
	drawscreen(menu, 0);
	addscreenrc(menu, listbox);

	while(1)
	{
		rcret = waitrc(menu, 0, 0);

		if(rcret == getrcconfigint("rcexit", NULL)) break;

		if(rcret == getrcconfigint("rcok", NULL))
		{
			if(listbox->select != NULL && listbox->select->handle != NULL)
			{
				oaktpage = listbox->aktpage;
				oaktline = listbox->aktline;
				clearscreen(menu);
				char* tmpstr = ostrcat(((struct tithek*)listbox->select->handle)->link, NULL, 0, 0);
				if(((struct tithek*)listbox->select->handle)->flag == 1)
					screentithekplay(tmpstr);
				else
					screentithekmenu(tmpstr);

				free(tmpstr); tmpstr = NULL;
				if(createtithekmenu(titheklink, menu, listbox) != 0) break;
				listbox->aktpage = oaktpage;
				listbox->aktline = oaktline;
                        	drawscreen(menu, 0);
				addscreenrc(menu, listbox);
			}
                }
	}

	freetithek();
	delmarkedscreennodes(menu, 1);
	delownerrc(menu);
	clearscreen(menu);
}

void screentithekplay(char* titheklink)
{
	int rcret = -1, gridbr = 0, posx = 0, count = 0;

	rcret = servicestop(status.aktservice, 1, 0);
	if(rcret == 1) return;

	struct skin* grid = getscreen("titheklist");
	struct skin* listbox = getscreennode(grid, "listbox");

	struct skin* tmp = NULL;

	char* tithekfile = NULL;
	char* tithekpic = NULL;
	char* oldtitheklink = NULL;
start:
	if(titheklink == NULL) return;

	listbox->aktpage = -1;
        listbox->aktline = 1;

	tithekfile = tithekdownload(titheklink);

	freetithek();
	if(readtithek(tithekfile) != 0) return;
	struct tithek* titheknode = tithek;

	while(titheknode != NULL)
	{
		tmp = addlistbox(grid, listbox, tmp, 1);
		if(tmp != NULL)
		{
			count++;
			if(gridbr == 0)
				tmp->type = GRIDBR;
				//tmp->type = TEXTBOXGRIDBR;
			//else
				//tmp->type = TEXTBOX;
			gridbr = 1;
			tmp->wrap = YES;
			
			tmp->height = 280;
			tmp->width = 390;
			tmp->prozwidth = 0;
			//tmp->bgcol = 0xffffff;
			tmp->bgspace = 1;
			tmp->vspace = 40;
			tmp->hspace = 2;
			tmp->posx = posx;
			//tmp->fontcol = 0x0000ff;
			tmp->halign = CENTER;
			tmp->valign = TEXTBOTTOM;
			changetext(tmp, titheknode->title);
			tmp->handle = (char*)titheknode;
			posx += tmp->width;
			if(count >= 3)
			{
				count = 0;
				posx = 0;
				gridbr = 0;
			}
		}
		titheknode = titheknode->next;
	}

	drawscreen(grid, 0);
	addscreenrc(grid, listbox);

	while(1)
	{
		if(listbox->select != NULL && listbox->select->handle != NULL)
		{
			tmp = listbox->select;
			while(tmp != NULL)
			{
				if(tmp->pagecount != listbox->aktpage) break;
				if(tmp->handle != NULL)
				{
					tithekpic = tithekdownload(((struct tithek*)tmp->handle)->pic);
					changepic(tmp, tithekpic);
					free(tithekpic); tithekpic = NULL;
				}
				tmp = tmp->prev;
			}
			tmp = listbox->select;
			while(tmp != NULL)
			{
				if(tmp->pagecount != listbox->aktpage) break;
				if(tmp->handle != NULL)
				{
					tithekpic = tithekdownload(((struct tithek*)tmp->handle)->pic);
					changepic(tmp, tithekpic);
					free(tithekpic); tithekpic = NULL;
				}
				tmp = tmp->next;
			}

			drawscreen(grid, 0);
		}
		rcret = waitrc(grid, 0, 0);

		if(rcret == getrcconfigint("rcexit", NULL))
		{
			if(oldtitheklink == NULL)
				break;
			else
			{
				struct splitstr* ret1 = NULL;
				int count1 = 0;
				char* tmpstr1 = NULL;
				tmpstr1 = ostrcat(tmpstr1, oldtitheklink, 1, 0);
				ret1 = strsplit(tmpstr1, "#", &count1);
				int max = count1;
				int update = 1;
				int i = 0;
				char* tmpstr2 = NULL;
				for( i = 0; i < max; i++){
					debug(170, "ret1 (1)-(%d/%d)=%s", i, max, (&ret1[i])->part);
					if(max > 1)
					{
						debug(170, "ret1 (2)-(%d/%d)=%s", i, max, (&ret1[i])->part);
						tmpstr2 = ostrcat(tmpstr2, (&ret1[i])->part, 1, 0);
					}
					else if(max = 2)
					{
						debug(170, "ret1 (3)-(%d/%d)=%s", i, max, (&ret1[i])->part);
						titheklink = ostrcat((&ret1[i])->part, NULL, 0, 0);
						break;
					}
					if(max > 2 && i<max-2)
					{
						debug(170, "ret1 (4)-(%d/%d)=%s", i, max, (&ret1[i])->part);
						tmpstr2 = ostrcat(tmpstr2, "#", 1, 0);
					}
					else
					{
						debug(170, "ret1 (5)-(%d/%d)=%s", i, max, (&ret1[i+1])->part);
						titheklink = ostrcat((&ret1[i+1])->part, NULL, 0, 0);
						break;
					}
				}

				debug(170, "oldtitheklink1=%s", oldtitheklink);
				debug(170, "oldtitheklink2=%s", tmpstr2);						
				
				oldtitheklink = ostrcat(tmpstr2, NULL, 0, 0);

				if(titheklink == NULL)
				{
					debug(170, "oldtitheklink3=http://atemio.dyndns.tv/mediathek/mainmenu.list");
					titheklink = ostrcat("http://atemio.dyndns.tv/mediathek/mainmenu.list", NULL, 0, 0);
				}

				free(ret1),ret1 = NULL;
				free(tmpstr1),tmpstr1 = NULL;
				free(tmpstr2),tmpstr2 = NULL;				
				tmp = NULL;
				free(tithekfile); tithekfile = NULL;
				freetithek();
				delmarkedscreennodes(grid, 1);
				delownerrc(grid);
				clearscreen(grid);
				rcret = -1, gridbr = 0, posx = 0, count = 0;
				goto start;
			}
		}

		if(rcret == getrcconfigint("rcok", NULL))
		{
			if(listbox->select != NULL && listbox->select->handle != NULL)
			{
					if(oldtitheklink == NULL)
						oldtitheklink = ostrcat(oldtitheklink, titheklink, 1, 0);
					else
					{
						oldtitheklink = ostrcat(oldtitheklink, "#", 1, 0);
						oldtitheklink = ostrcat(oldtitheklink, titheklink, 1, 0);
					}

					titheklink = ostrcat(((struct tithek*)listbox->select->handle)->link, NULL, 0, 0);

					if(((struct tithek*)listbox->select->handle)->flag == 2)
					{
						textbox(_("Message"), _("start, Playback"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 0, 1);				
					}
					
					tmp = NULL;
					free(tithekfile); tithekfile = NULL;
					freetithek();
					delmarkedscreennodes(grid, 1);
					delownerrc(grid);
					clearscreen(grid);
					rcret = -1, gridbr = 0, posx = 0, count = 0;
					goto start;
			}
       	}
	}

	free(tithekfile); tithekfile = NULL;
	freetithek();
	delmarkedscreennodes(grid, 1);
	delownerrc(grid);
	clearscreen(grid);

	servicestart(status.lastservice->channel, NULL, NULL, 0);
}

#endif
