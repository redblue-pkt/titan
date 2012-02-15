#ifndef TITHEK_H
#define TITHEK_H

#define TITHEKPATH "/tmp/tithek"

struct tithek
{
	char* title;
	char* link;
	char* pic;
	int flag;
	char* localname;
	struct tithek* prev;
	struct tithek* next;
};
struct tithek *tithek = NULL;

struct tithek* addtithek(char *line, int count, struct tithek* last)
{
	//debug(1000, "in");
	struct tithek *newnode = NULL, *prev = NULL, *node = tithek;
	char *link = NULL, *pic = NULL, *title = NULL, *localname = NULL;
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

	localname = malloc(MINMALLOC);
	if(localname == NULL)
	{
		err("no memory");
		free(newnode);
		free(link);
		free(pic);
		free(title);
		return NULL;
	}

	memset(newnode, 0, sizeof(struct tithek));

	ret = sscanf(line, "%[^#]#%[^#]#%[^#]#%d#%[^#]", title, link, pic, &newnode->flag, localname);
	if(ret != 5)
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
		free(localname);
		free(newnode);
		return NULL;
	}

	newnode->link = ostrcat(link, "", 1, 0);
	newnode->pic = ostrcat(pic, "", 1, 0);
	newnode->title = ostrcat(title, "", 1, 0);
	newnode->localname = ostrcat(localname, "", 1, 0);

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

			free(node->localname);
			node->localname = NULL;

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

char* tithekdownload(char* link, char* localname, int flag)
{
	int ret = 1;
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

	if(flag == 0)
	{
		localfile = ostrcat(TITHEKPATH, "/", 0, 0);
		if(localname == NULL)
			localfile = ostrcat(localfile, basename(tmpstr), 1, 0);
		else
			localfile = ostrcat(localfile, localname, 1, 0);
	}
	else
	{
		localfile = ostrcat(getconfig("rec_path", NULL), "/", 0, 0);
		if(localname == NULL)
			localfile = ostrcat(localfile, basename(tmpstr), 1, 0);
		else
			localfile = ostrcat(localfile, localname, 1, 0);
	}

	if(flag == 0)
	{
		if(!file_exist(localfile))
			gethttp(ip, path, 80, localfile, NULL, NULL, 0);
	}
	else
	{
		if(file_exist(localfile))
			ret = textbox(_("Message"), _("File exist, overwrite?"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 0, 0);

		if(ret == 1)
			screendownload("Download", ip, path, 80, localfile, NULL, 0);
	}

	free(ip); ip = NULL;
	free(tmpstr); tmpstr = NULL;

	return localfile;
}

int createtithekmenu(char* titheklink, struct skin* menu, struct skin* listbox)
{
	struct skin* tmp = NULL;
	char* tithekfile = NULL;
	char* tithekpic = NULL;

	tithekfile = tithekdownload(titheklink, NULL, 0);

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
			tithekpic = tithekdownload(titheknode->pic, titheknode->localname, 0);
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
					screentithekplay(tmpstr, 0); 
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

int createtithekplay(char* titheklink, struct skin* grid, struct skin* listbox, struct skin* countlabel)
{
	int gridbr = 0, posx = 0, count = 0, sumcount = 0;
	struct skin* tmp = NULL;
	char* tithekfile = NULL;
	char* tithekpic = NULL;
	char* tmpstr = NULL;

	tithekfile = tithekdownload(titheklink, NULL, 0);

	delmarkedscreennodes(grid, 1);
	freetithek();
	if(readtithek(tithekfile) != 0) return 1;

	struct tithek* titheknode = tithek;

	while(titheknode != NULL)
	{
		tmp = addlistbox(grid, listbox, tmp, 1);
		if(tmp != NULL)
		{
			sumcount++;
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

	tmpstr = oitoa(sumcount);
	changetext(countlabel, tmpstr);
	free(tmpstr); tmpstr = NULL;

	free(tithekfile); tithekfile = NULL;
	return 0;
}

void screentithekplay(char* titheklink, int first)
{
	int rcret = -1, oaktline = 1, oaktpage = -1, ogridcol = 0;
	
	if(first == 1)
	{
		rcret = servicestop(status.aktservice, 1, 1);
		if(rcret == 1) return;
	}

	struct skin* grid = getscreen("titheklist");
	struct skin* listbox = getscreennode(grid, "listbox");
	struct skin* countlabel = getscreennode(grid, "countlabel");
	struct skin* tmp = NULL;
	char* tithekpic = NULL;
	
	if(titheklink == NULL) return;

	listbox->aktpage = -1;
	listbox->aktline = 1;
	listbox->gridcol = 0;

	if(createtithekplay(titheklink, grid, listbox, countlabel) != 0) return;

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
					tithekpic = tithekdownload(((struct tithek*)tmp->handle)->pic, ((struct tithek*)tmp->handle)->localname, 0);
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
					tithekpic = tithekdownload(((struct tithek*)tmp->handle)->pic, ((struct tithek*)tmp->handle)->localname, 0);
					changepic(tmp, tithekpic);
					free(tithekpic); tithekpic = NULL;
				}
				tmp = tmp->next;
			}

			drawscreen(grid, 0);
		}
		
		int count = getfilecount(TITHEKPATH);
		if(count > 500)
			delallfiles(TITHEKPATH, ".jpg");
		
		rcret = waitrc(grid, 0, 0);

		if(rcret == getrcconfigint("rcexit", NULL))
		{
			unlink("/tmp/tithek/*");		
			break;
		}
		if(rcret == getrcconfigint("rcred", NULL))
		{
			if(listbox->select != NULL && listbox->select->handle != NULL)
			{
				if(((struct tithek*)listbox->select->handle)->flag == 2)
				{
					char* tmpstr = tithekdownload((((struct tithek*)listbox->select->handle)->link), ((struct tithek*)tmp->handle)->localname, 1);
					free(tmpstr); tmpstr = NULL;
					drawscreen(grid, 0);
				}
			}
		}

		if(rcret == getrcconfigint("rcok", NULL))
		{
			if(listbox->select != NULL && listbox->select->handle != NULL)
			{
				clearscreen(grid);
				if(((struct tithek*)listbox->select->handle)->flag == 2 && status.security == 1)
				{
					if(textbox(_("Message"), _("Start playback"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 0, 0) == 1)
						screenplay((((struct tithek*)listbox->select->handle)->link), 2, 0);
				}
				else if(((struct tithek*)listbox->select->handle)->flag == 4 && status.security == 1)
				{
					char* tmpstr = ostrcat(((struct tithek*)listbox->select->handle)->link, NULL, 0, 0);
					char* tmpstr1 = NULL;
					if(tmpstr != NULL) tmpstr1 = getstreamurl(tmpstr, 1);
					free(tmpstr); tmpstr = NULL;
						
					if(tmpstr1 != NULL)
						if(textbox(_("Message"), _("Start playback"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 0, 0) == 1)
							screenplay(tmpstr1, 2, 0);
					
					free(tmpstr1); tmpstr1 = NULL;
				}
				else
				{
					oaktpage = listbox->aktpage;
					oaktline = listbox->aktline;
					ogridcol = listbox->gridcol;
					char* tmpstr = ostrcat(((struct tithek*)listbox->select->handle)->link, NULL, 0, 0);
					screentithekplay(tmpstr, 0);
					free(tmpstr); tmpstr = NULL;
					if(createtithekplay(titheklink, grid, listbox, countlabel) != 0) break;
					listbox->aktpage = oaktpage;
					listbox->aktline = oaktline;
					listbox->gridcol = ogridcol;
					addscreenrc(grid, listbox);
				}
				drawscreen(grid, 0);
			}
		}
	}

	freetithek();
	delmarkedscreennodes(grid, 1);
	delownerrc(grid);
	clearscreen(grid);

	if(first == 1)
		servicestart(status.lastservice->channel, NULL, NULL, 0);
}

char* getstreamurl(char* link, int flag)
{
	char* ip = NULL, *pos = NULL, *path = NULL;
	ip = string_replace("http://", "", (char*)link, 0);

	if(ip != NULL)
		pos = strchr(ip, '/');
	if(pos != NULL)
	{
		pos[0] = '\0';
		path = pos + 1;
	}
	
	char* tmpstr = NULL;
	tmpstr = gethttp(ip, path, 80, NULL, NULL, NULL, 0);

	string_resub("\": \"url=","\", \"",tmpstr);

	while(string_find(",url=",tmpstr))
	{
		tmpstr = string_replace(",url=", "\nurl=", tmpstr, 1);
	}

	tmpstr = string_decode(tmpstr,0);

	int count = 0;
	int i = 0;
	struct splitstr* ret1 = NULL;
	ret1 = strsplit(tmpstr, "\n", &count);
	int max = count;
	char* streamurl = NULL;
	for( i = 0; i < max; i++){
		if(string_find("type=video/mp4",(&ret1[i])->part))
		{
			streamurl = ostrcat(streamurl, (&ret1[i])->part, 1, 0);
			int count2 = 0;
			struct splitstr* ret2 = NULL;
			ret2 = strsplit((&ret1[i])->part, "+", &count2);
			streamurl = ostrcat("", (&ret2[0])->part, 0, 0);
			free(ret2), ret2 = NULL;
		}
	}
	free(ret1), ret1 = NULL;
	free(tmpstr), tmpstr = NULL;
		
	streamurl = string_replace("url=", "", streamurl, 1);
	return streamurl;
}

#endif
