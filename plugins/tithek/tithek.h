#ifndef TITHEK_H
#define TITHEK_H

#define TITHEKPATH "/tmp/tithek"

//flag 0: not used
//flag 1: not used
//flag 2: streamlink allgemein
//flag 4: streamlink youtube
//flag 1000: menu with pin
struct tithek
{
	char* title;
	char* link;
	char* pic;
	char* localname;
	char* menutitle;	
	int flag;
	struct tithek* prev;
	struct tithek* next;
};
struct tithek *tithek = NULL;

struct tithek* addtithek(char *line, int count, struct tithek* last)
{
	//debug(1000, "in");
	struct tithek *newnode = NULL, *prev = NULL, *node = tithek;
	char *link = NULL, *pic = NULL, *title = NULL, *localname = NULL, *menutitle = NULL;
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

	menutitle = malloc(MINMALLOC);
	if(menutitle == NULL)
	{
		err("no memory");
		free(newnode);
		free(link);
		free(pic);
		free(title);
		free(localname);		
		return NULL;
	}

	memset(newnode, 0, sizeof(struct tithek));

	ret = sscanf(line, "%[^#]#%[^#]#%[^#]#%[^#]#%[^#]#%d", title, link, pic, localname, menutitle, &newnode->flag);
	if(newnode->flag == 9999 && !file_exist("/var/swap/etc/.codecpack"))
	{
		free(link);
		free(pic);
		free(title);
		free(localname);
		free(menutitle);
		free(newnode);
		return NULL;
	}
	
	if(ret != 6)
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
		free(menutitle);
		free(newnode);
		return NULL;
	}

	newnode->link = ostrcat(link, "", 1, 0);
	newnode->pic = ostrcat(pic, "", 1, 0);
	newnode->title = ostrcat(title, "", 1, 0);
	newnode->localname = ostrcat(localname, "", 1, 0);
	newnode->menutitle = ostrcat(menutitle, "", 1, 0);

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
		return 0;
	}

	fd = fopen(filename, "r");
	if(fd == NULL)
	{
		perr("can't open %s", filename);
		free(fileline);
		return 0;
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
	return linecount;
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

			free(node->menutitle);
			node->menutitle = NULL;

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

char* tithekdownload(char* link, char* localname, char* pw, int pic, int flag)
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
		{
			if(pic == 1)
				gethttp(ip, path, 80, localfile, pw, NULL, 0);
			else
				gethttp(ip, path, 80, localfile, pw, NULL, 0);
		}
	}
	else
	{
		if(file_exist(localfile))
			ret = textbox(_("Message"), _("File exist, overwrite?"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 0, 0);

		if(ret == 1)
			screendownload("Download", ip, path, 80, localfile, pw, 0);
	}

	free(ip); ip = NULL;
	free(tmpstr); tmpstr = NULL;

	return localfile;
}

int createtithekplay(char* titheklink, struct skin* grid, struct skin* listbox, struct skin* countlabel)
{
	int gridbr = 0, posx = 0, count = 0, sumcount = 0, count1 = 0, pagecount = 0, skip = 0;
	struct skin* tmp = NULL;
	char* tithekfile = NULL;
	char* tmpstr = NULL;

	if(ostrstr(titheklink, "http://") != NULL)
		tithekfile = tithekdownload(titheklink, NULL, HTTPAUTH, 0, 0);
	else
		tithekfile = ostrcat(titheklink, NULL, 0, 0);

	delmarkedscreennodes(grid, 1);
	freetithek();
//	if(readtithek(tithekfile) != 0) return 1;
	int linecount = readtithek(tithekfile);
	if(linecount == 0) return 1;
	
	struct tithek* titheknode = tithek;

	int height = 280;
	int width = 390;
	int picheight = 230;
	int picwidth = 370;
	int zcount = 3;
	int fontsize = 20;
	int pcount = 6;
	 
	if(linecount > 8 && getconfigint("tithek_view", NULL) == 1)
	{
		height = 180;
		width = 295;
		picheight = 130;
		picwidth = 270;
		zcount = 4;
		fontsize = 18;
		pcount = 12;
	}

	while(titheknode != NULL)
	{
		tmp = addlistbox(grid, listbox, tmp, 1);
		if(tmp != NULL)
		{
			skip = 0;
			sumcount++;
			count++;
			count1++;
			if(gridbr == 0)
				tmp->type = GRIDBR;
			gridbr = 1;
			tmp->wrap = YES;

			tmp->picheight = picheight;
			tmp->picwidth = picwidth;

			tmp->fontsize = fontsize;
			tmp->height = height;
			tmp->width = width;
			tmp->prozwidth = 0;
			//tmp->bgcol = 0xffffff;
			tmp->bgspace = 1;
			tmp->vspace = 10;
			tmp->hspace = 10;
			tmp->posx = posx;
			//tmp->fontcol = 0x0000ff;
			tmp->halign = CENTER;
			tmp->valign = TEXTBOTTOM;
			changetext(tmp, titheknode->title);
			tmp->handle = (char*)titheknode;
			posx += tmp->width;
			if(count >= zcount)
			{
				count = 0;
				posx = 0;
				gridbr = 0;
			}
			
			if(count1 >= pcount)
			{
				count1 = 0;
				pagecount++;
				skip = 1;
			}
		}
		titheknode = titheknode->next;
	}

	if(skip == 0)
		pagecount++;

	tmpstr = oitoa(sumcount);

	char* tmpstr1 = ostrcat(_("found"), NULL, 0, 0);
	tmpstr1 = ostrcat(tmpstr1, " ", 1, 0);
	tmpstr1 = ostrcat(tmpstr1, tmpstr, 1, 0);
	free(tmpstr); tmpstr = NULL;
	tmpstr1 = ostrcat(tmpstr1, " ", 1, 0);
	tmpstr1 = ostrcat(tmpstr1, _("Results"), 1, 0);
	changetext(countlabel, tmpstr1);
	free(tmpstr1); tmpstr1 = NULL;
	
	free(tithekfile); tithekfile = NULL;
	return pagecount;
}

void screentithekplay(char* titheklink, char* title, int first)
{
	int rcret = -1, oaktline = 1, oaktpage = -1, ogridcol = 0;

	if(file_exist("/var/bin/audio.elf") || file_exist("/var/swap/bin/audio.elf"))
		textbox(_("Message"), _("Alternativ Audio Firmware not working korrekt with all videos (DTSDOWNMIX)!"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 1000, 200, 0, 0);
	
	if(first == 1)
	{
		delallfiles("/tmp/tithek", NULL); 
		mkdir("/tmp/tithek", 777); 
		rcret = servicestop(status.aktservice, 1, 1);
		if(rcret == 1) return;
	}
	status.hangtime = 99999;

	struct skin* grid = getscreen("titheklist");
	struct skin* listbox = getscreennode(grid, "listbox");
	struct skin* countlabel = getscreennode(grid, "countlabel");
	struct skin* countpage = getscreennode(grid, "countpage");
	struct skin* load = getscreen("loading");
	struct skin* tmp = NULL;
	char* tithekpic = NULL;
	
	if(titheklink == NULL) return;
	
	listbox->aktpage = -1;
	listbox->aktline = 1;
	listbox->gridcol = 0;
	listbox->select = NULL;

//	if(createtithekplay(titheklink, grid, listbox, countlabel) != 0) return;
	int pagecount = createtithekplay(titheklink, grid, listbox, countlabel);
	if(pagecount == 0) return;

	drawscreen(grid, 0, 0);
	addscreenrc(grid, listbox);
				
	while(1)
	{
		changetitle(grid, _(title));
		if(listbox->select != NULL && listbox->select->handle != NULL)
		{
			tmp = listbox->select;
			while(tmp != NULL)
			{

				if(tmp->pagecount != listbox->aktpage) break;

				char* tmpstr = ostrcat(_("Page"), NULL, 0, 0);
				tmpstr = ostrcat(tmpstr, " ( ", 1, 0);
				tmpstr = ostrcat(tmpstr, oitoa(tmp->pagecount), 1, 0);
				tmpstr = ostrcat(tmpstr, " / ", 1, 0);
				tmpstr = ostrcat(tmpstr, oitoa(pagecount), 1, 0);	
				tmpstr = ostrcat(tmpstr, _(" )"), 1, 0);
				changetext(countpage, tmpstr);
				free(tmpstr); tmpstr = NULL;
	
				if(tmp->handle != NULL)
				{
					tithekpic = tithekdownload(((struct tithek*)tmp->handle)->pic, ((struct tithek*)tmp->handle)->localname, "aXBrLUdaRmg6RkhaVkJHaG56ZnZFaEZERlRHenVpZjU2NzZ6aGpHVFVHQk5Iam0=", 1, 0);
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
					tithekpic = tithekdownload(((struct tithek*)tmp->handle)->pic, ((struct tithek*)tmp->handle)->localname, "aXBrLUdaRmg6RkhaVkJHaG56ZnZFaEZERlRHenVpZjU2NzZ6aGpHVFVHQk5Iam0=", 1, 0);
					changepic(tmp, tithekpic);
					free(tithekpic); tithekpic = NULL;
				}
				tmp = tmp->next;
			}

			drawscreen(grid, 0, 0);
		}
		
		int count = getfilecount(TITHEKPATH);
		if(count > 500)
			delallfiles(TITHEKPATH, ".jpg");
		
		rcret = waitrc(grid, 0, 0);

		if(rcret == getrcconfigint("rcexit", NULL)) break;
		if(rcret == getrcconfigint("rcmenu", NULL))
		{
			screentithek_settings();
		}
		
		if(rcret == getrcconfigint("rcred", NULL))
		{
			if(listbox->select != NULL && listbox->select->handle != NULL)
			{
				if(((struct tithek*)listbox->select->handle)->flag == 2)
				{
					if(status.security == 1)
					{
						char* tmpstr = tithekdownload((((struct tithek*)listbox->select->handle)->link), NULL, NULL, 0, 1);
						free(tmpstr); tmpstr = NULL;
						drawscreen(grid, 0, 0);
					}
					else
						textbox(_("Message"), _("Registration needed, please contact Atemio !"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 1000, 200, 0, 0);
				}
				else if(((struct tithek*)listbox->select->handle)->flag == 4)
				{
					if(status.security == 1)
					{
						char* tmpstr = NULL, *tmpstr1 = NULL, *tmpstr2 = NULL;
						tmpstr = ostrcat(((struct tithek*)listbox->select->handle)->link, NULL, 0, 0);
						if(tmpstr != NULL) tmpstr1 = getstreamurl(tmpstr, NULL, NULL, 1);
						tmpstr2 = changefilenameext(((struct tithek*)tmp->handle)->localname, ".mp4");
						free(tmpstr); tmpstr = NULL;
							
						if(tmpstr1 != NULL)
						{
							char* tmpstr = tithekdownload(tmpstr1, tmpstr2, NULL, 0, 1);
							free(tmpstr); tmpstr = NULL;
							drawscreen(grid, 0, 0);
						}
						else
							textbox(_("Message"), _("Can't get Streamurl !"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 0, 0);
						free(tmpstr1); tmpstr1 = NULL;
						free(tmpstr2); tmpstr2 = NULL;
					}
					else
						textbox(_("Message"), _("Registration needed, please contact Atemio !"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 1000, 200, 0, 0);
				}
			}
		}
				printf("wooooooooooooooooooo11111111111111111111\n");
		if(rcret == getrcconfigint("rcok", NULL))
		{
						printf("wooooooooooooooooooo22222222222222222\n");
			if(listbox->select != NULL && listbox->select->handle != NULL)
			{
				clearscreen(grid);

				if(((struct tithek*)listbox->select->handle)->flag == 2)
				{
					if(status.security == 1)
					{
						if(textbox(_("Message"), _("Start playback"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 0, 0) == 1)
						{
							if(ostrcmp(title, "TiThek - Internet Radio") == 0)
							{
								screenplay((((struct tithek*)listbox->select->handle)->link), 2, 4);
							}
							else
								screenplay((((struct tithek*)listbox->select->handle)->link), 2, 0);
						}			
					}
					else
						textbox(_("Message"), _("Registration needed, please contact Atemio !"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 1000, 200, 0, 0);
				}
				else if(((struct tithek*)listbox->select->handle)->flag == 4)
				{
					if(status.security == 1)
					{
						char* tmpstr = ostrcat(((struct tithek*)listbox->select->handle)->link, NULL, 0, 0);
						char* tmpstr1 = NULL;
						if(tmpstr != NULL) tmpstr1 = getstreamurl(tmpstr, NULL, NULL, 1);
						free(tmpstr); tmpstr = NULL;
							
						if(tmpstr1 != NULL)
						{
							if(textbox(_("Message"), _("Start playback"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 0, 0) == 1)
								screenplay(tmpstr1, 2, 0);				
						}
						else
							textbox(_("Message"), _("Can't get Streamurl !"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 0, 0);
						free(tmpstr1); tmpstr1 = NULL;
					}
					else
						textbox(_("Message"), _("Registration needed, please contact Atemio !"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 1000, 200, 0, 0);			
				}
				else if((((struct tithek*)listbox->select->handle)->flag == 5) || (((struct tithek*)listbox->select->handle)->flag == 6) || (((struct tithek*)listbox->select->handle)->flag == 7) || (((struct tithek*)listbox->select->handle)->flag == 8))
				{
					if(status.security == 1)
					{
						char* tmpstr = ostrcat(((struct tithek*)listbox->select->handle)->link, NULL, 0, 0);
						char* tmpstr1 = NULL;
						
						if(((struct tithek*)listbox->select->handle)->flag == 5)
						{
							if(tmpstr != NULL) tmpstr1 = getstreamurl(tmpstr, "http://rtl2now.rtl2.de", "rtl2now", 2);
						}
						else if(((struct tithek*)listbox->select->handle)->flag == 6)
						{
							if(tmpstr != NULL) tmpstr1 = getstreamurl(tmpstr, "http://www.superrtlnow.de", "superrtlnow", 2);
						}
						else if(((struct tithek*)listbox->select->handle)->flag == 7)
						{
							if(tmpstr != NULL) tmpstr1 = getstreamurl(tmpstr, "http://rtl-now.rtl.de", "rtlnow", 2);
						}
						else if(((struct tithek*)listbox->select->handle)->flag == 8)
						{
							if(tmpstr != NULL) tmpstr1 = getstreamurl(tmpstr, "http://www.voxnow.de", "voxnow", 2);
						}
	
						free(tmpstr); tmpstr = NULL;
							
						if(tmpstr1 != NULL)
						{
							if(textbox(_("Message"), _("Start playback"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 0, 0) == 1)
								screenplay(tmpstr1, 2, 0);				
						}
						else
							textbox(_("Message"), _("Can't get Streamurl !"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 0, 0);
						free(tmpstr1); tmpstr1 = NULL;
					}
					else
						textbox(_("Message"), _("Registration needed, please contact Atemio !"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 1000, 200, 0, 0);			
				}
				else if((((struct tithek*)listbox->select->handle)->flag == 9) || (((struct tithek*)listbox->select->handle)->flag == 10) || (((struct tithek*)listbox->select->handle)->flag == 11))
				{
					char* search = textinput("Search", NULL);
					if(search != NULL)
					{
						drawscreen(load, 0, 0);
						search = stringreplacechar(search, ' ', '+');
						char* id = NULL;
						char* line = NULL;
						char* pic = NULL;
						char* title = NULL;	
						char* ip = ostrcat("gdata.youtube.com", NULL, 0, 0);
						char* path = ostrcat("feeds/api/videos?q=", search, 0, 0);
						if(((struct tithek*)listbox->select->handle)->flag == 9)
							path = ostrcat(path, "&max-results=10", 1, 0);
						else if(((struct tithek*)listbox->select->handle)->flag == 10)
							path = ostrcat(path, "&max-results=25", 1, 0);
						else if(((struct tithek*)listbox->select->handle)->flag == 11)
							path = ostrcat(path, "&max-results=50", 1, 0);
									
						char* tmpstr = gethttp(ip, path, 80, NULL, NULL, NULL, 0);
						tmpstr = string_replace_all("media:thumbnail", "\nthumbnail", tmpstr, 1);
		
						int count = 0;
						int incount = 0;
						int i = 0;
						struct splitstr* ret1 = NULL;
						ret1 = strsplit(tmpstr, "\n", &count);
		
						if(ret1 != NULL)
						{
							int max = count;
							for(i = 0; i < max; i++)
							{
								if(ostrstr(ret1[i].part, "http://i.ytimg.com/vi/") != NULL)
								{
									pic = oregex(".*thumbnail url=\'(http://i.ytimg.com/vi/.*/0.jpg).*", ret1[i].part);
									id = oregex(".*thumbnail url=\'http://i.ytimg.com/vi/(.*)/0.jpg.*", ret1[i].part);
		
									if(id != NULL)
									{
										incount += 1;
										ip = ostrcat("www.youtube.com", NULL, 0, 0);
										path = ostrcat("watch?v=", id, 0, 0);
										title = gethttp(ip, path, 80, NULL, NULL, NULL, 0);
										title = string_resub("<meta name=\"title\" content=\"", "\">", title, 0);
		
										line = ostrcat(line, title, 1, 0);
										line = ostrcat(line, "#http://www.youtube.com/watch?v=", 1, 0);
										line = ostrcat(line, id, 1, 0);
										line = ostrcat(line, "#", 1, 0);
										line = ostrcat(line, pic, 1, 0);
										line = ostrcat(line, "#youtube_search_", 1, 0);
										line = ostrcat(line, oitoa(incount + time(NULL)), 1, 0);
										line = ostrcat(line, ".jpg#YouTube - Search#4\n", 1, 0);
										free(ip), ip = NULL;
										free(path), path = NULL;
										free(title), title = NULL;
									}
								}
							}
							free(ret1), ret1 = NULL;
		
							if(line != NULL)
							{
								writesys("/tmp/tithek/youtube.search.list", line, 0);
								free(line), line = NULL;
//								if(createtithekplay("/tmp/tithek/youtube.search.list", grid, listbox, countlabel) != 0) return;
								int pagecount = createtithekplay("/tmp/tithek/youtube.search.list", grid, listbox, countlabel);
								if(pagecount == 0) return;
								
								drawscreen(grid, 0, 0);
							}
						}
						free(tmpstr), tmpstr = NULL;
					}
					free(search), search = NULL;
					continue;
				}
				else if(((struct tithek*)listbox->select->handle)->flag == 12)
				{
					if(status.security == 1)
					{
						char* tmpstr = ostrcat(((struct tithek*)listbox->select->handle)->link, NULL, 0, 0);
						char* tmpstr1 = NULL;
						if(tmpstr != NULL) tmpstr1 = getstreamurl(tmpstr, NULL, NULL, 4);
						free(tmpstr); tmpstr = NULL;
							
						if(tmpstr1 != NULL)
						{
							if(textbox(_("Message"), _("Start playback"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 0, 0) == 1)
							{
								screenplay(tmpstr1, 2, 0);				
							}
						}
						else
							textbox(_("Message"), _("Can't get Streamurl !"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 0, 0);
						free(tmpstr1); tmpstr1 = NULL;
					}
					else
						textbox(_("Message"), _("Registration needed, please contact Atemio !"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 1000, 200, 0, 0);			
				}
				else if((((struct tithek*)listbox->select->handle)->flag == 13))
				{
					char* search = textinput("Search", NULL);
					if(search != NULL)
					{
						drawscreen(load, 0, 0);
						search = stringreplacechar(search, ' ', '+');
						char* id = NULL;
						char* line = NULL;
						char* pic = NULL;
						char* title = NULL;	
						char* ip = ostrcat("www.myvideo.de", NULL, 0, 0);
						char* path = ostrcat("Videos_A-Z?searchWord=", search, 0, 0);
									
						char* tmpstr = gethttp(ip, path, 80, NULL, NULL, NULL, 0);
						tmpstr = string_replace_all("<", "\n", tmpstr, 1);
						tmpstr = string_replace_all(">", "\n", tmpstr, 1);
 		
						int count = 0;
						int incount = 0;
						int i = 0;
						struct splitstr* ret1 = NULL;
						ret1 = strsplit(tmpstr, "\n", &count);
		
						if(ret1 != NULL)
						{
							int max = count;
							for(i = 0; i < max; i++)
							{								
								if(ostrstr(ret1[i].part, "img id='i") != NULL)
								{
								printf("ret1[i].part: %s",ret1[i].part);
								
									pic = oregex(".*longdesc='(.*)' class='vThumb'.*", ret1[i].part);
									id = oregex(".*img id='i(.*)' onload=.*", ret1[i].part);
									title = oregex(".*alt='(.*)' onmouseover=.*", ret1[i].part);
									debug(99, "---------------------------");
									debug(99, "title: %s", title);
									debug(99, "pic: %s", pic);
									debug(99, "id: %s", id);
									debug(99, "---------------------------");
	
									if(id != NULL)
									{
										incount += 1;
										line = ostrcat(line, title, 1, 0);
										line = ostrcat(line, "#http://www.myvideo.de/dynamic/get_player_video_xml.php?flash_playertype=SER&ID=", 1, 0);
										line = ostrcat(line, id, 1, 0);
										line = ostrcat(line, "&_countlimit=4&autorun=yes;pageUrl=http://www.myvideo.de/watch/", 1, 0);										
										line = ostrcat(line, id, 1, 0);
										line = ostrcat(line, "/;playpath=flv:movie24/a0/", 1, 0);
										line = ostrcat(line, id, 1, 0);
										line = ostrcat(line, ";", 1, 0);
										line = ostrcat(line, id, 1, 0);																				
										line = ostrcat(line, "#", 1, 0);
										line = ostrcat(line, pic, 1, 0);
										line = ostrcat(line, "#myvideo_search_", 1, 0);
										line = ostrcat(line, oitoa(incount + time(NULL)), 1, 0);
										line = ostrcat(line, ".jpg#MyVideo - Search#12\n", 1, 0);
										free(ip), ip = NULL;
										free(path), path = NULL;
										free(title), title = NULL;
									}

								}
							}
							free(ret1), ret1 = NULL;
		
							if(line != NULL)
							{
								writesys("/tmp/tithek/myvideo.search.list", line, 0);
								free(line), line = NULL;
//								if(createtithekplay("/tmp/tithek/myvideo.search.list", grid, listbox, countlabel) != 0) return;
								int pagecount = createtithekplay("/tmp/tithek/myvideo.search.list", grid, listbox, countlabel);
								if(pagecount == 0) return;

								drawscreen(grid, 0, 0);
							}
						}
						free(tmpstr), tmpstr = NULL;
					}
					free(search), search = NULL;
					continue;
				}
				else if(((struct tithek*)listbox->select->handle)->flag == 14)
				{
					if(status.security == 1)
					{
						char* tmpstr = ostrcat(((struct tithek*)listbox->select->handle)->link, NULL, 0, 0);
						char* tmpstr1 = NULL;
						if(tmpstr != NULL) tmpstr1 = getstreamurl(tmpstr, NULL, NULL, 5);
						free(tmpstr); tmpstr = NULL;
							
						if(tmpstr1 != NULL)
						{
							if(textbox(_("Message"), _("Start playback"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 0, 0) == 1)
								screenplay(tmpstr1, 2, 0);
								
//							writesys("/media/test.m3u", tmpstr1, 0);
//							char* test = readfiletomem("/media/test.m3u", 0);		
//							screenplay(test, 2, 0);

//							if(textbox(_("Message"), _("Start playback"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 0, 0) == 1)
//								playerstart(tmpstr1);	

						}
						else
							textbox(_("Message"), _("Can't get Streamurl !"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 0, 0);
						free(tmpstr1); tmpstr1 = NULL;
					}
					else
						textbox(_("Message"), _("Registration needed, please contact Atemio !"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 1000, 200, 0, 0);			
				}
				else if(((struct tithek*)listbox->select->handle)->flag == 15)
				{
				printf("wooooooooooooooooooo\n");
					if(status.security == 1)
					{
						char* tmpstr = ostrcat(((struct tithek*)listbox->select->handle)->link, NULL, 0, 0);
						char* tmpstr1 = NULL;
						if(tmpstr != NULL) tmpstr1 = getstreamurl(tmpstr, NULL, NULL, 6);
						free(tmpstr); tmpstr = NULL;
							
						if(tmpstr1 != NULL)
						{
							if(textbox(_("Message"), _("Start playback"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 0, 0) == 1)
								screenplay(tmpstr1, 2, 0);
								
//							writesys("/media/test.m3u", tmpstr1, 0);
//							char* test = readfiletomem("/media/test.m3u", 0);		
//							screenplay(test, 2, 0);

//							if(textbox(_("Message"), _("Start playback"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 0, 0) == 1)
//								playerstart(tmpstr1);	

						}
						else
							textbox(_("Message"), _("Can't get Streamurl !"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 0, 0);
						free(tmpstr1); tmpstr1 = NULL;
					}
					else
						textbox(_("Message"), _("Registration needed, please contact Atemio !"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 1000, 200, 0, 0);			
				}
				else
				{
					int pincheck = 0;
					if(((struct tithek*)listbox->select->handle)->flag == 1000)
						pincheck = screenpincheck(0, NULL);
					if(pincheck == 0)
					{
						oaktpage = listbox->aktpage;
						oaktline = listbox->aktline;
						ogridcol = listbox->gridcol;
						char* tmpstr = ostrcat(((struct tithek*)listbox->select->handle)->link, NULL, 0, 0);
						char* tmpstr1 = ostrcat(((struct tithek*)listbox->select->handle)->menutitle, " - ", 0, 0);					
						char* tmpstr2 = ostrcat(tmpstr1, ((struct tithek*)listbox->select->handle)->title, 1, 0);
						screentithekplay(tmpstr, tmpstr2, 0);
						free(tmpstr); tmpstr = NULL;
						free(tmpstr2); tmpstr2 = NULL;					
//						if(createtithekplay(titheklink, grid, listbox, countlabel) != 0) break;
						int pagecount = createtithekplay(titheklink, grid, listbox, countlabel);
						if(pagecount == 0) break;

						listbox->aktpage = oaktpage;
						listbox->aktline = oaktline;
						listbox->gridcol = ogridcol;
						addscreenrc(grid, listbox);
					}
				}
				drawscreen(grid, 0, 0);
			}			
		}
	}

	freetithek();
	delmarkedscreennodes(grid, 1);
	delownerrc(grid);
	clearscreen(grid);

	if(first == 1)
	{
		delallfiles("/tmp/tithek", ".list");
		servicecheckret(servicestart(status.lastservice->channel, NULL, NULL, 0), 0);
	}
	status.hangtime = getconfigint("hangtime", NULL);
}

/*
char* ltostr(char* str, long val, unsigned base)
{
	ldiv_t r;

	if(base > 36 || str == NULL)
		return NULL;

	if(val < 0) *str++ = '-';
	r = ldiv(labs(val), base);

	if(r.quot > 0) str = ltostr(str, r.quot, base);

	*str++ = "0123456789abcdefghijklmnopqrstuvwxyz"[(int)r.rem];
	*str = '\0';
	return str;
}

char* oltostr(long val, unsigned base)
{
	char* str = NULL;

	str = calloc(1, MINMALLOC);
	if(str == NULL)
	{
		err("no mem");
		return NULL;
	}

	ltostr(str, val, base);

	return ostrcat(str, NULL, 1, 0);
}
*/

char* filenuke(char* input)
{	
	printf("running\n");
	char* p = NULL;
	char* k = NULL;

	struct splitstr* ret0 = NULL;
	int count0 = 0;
	ret0 = strsplit(input, "\n", &count0);
	p = ostrcat((&ret0[0])->part, NULL, 0, 0);
	k = ostrcat((&ret0[1])->part, NULL, 0, 0);
	free(ret0), ret0 = NULL;
	
	k = string_replace_all("||", "| |", k, 1);
printf("p in: %s\n", p);
printf("k in: %s\n", k);

//	char* replace = NULL;
	struct splitstr* ret1 = NULL;
	int count = 0;
	int i = 0;
	ret1 = strsplit(k, "|", &count);
	int max = count;
//	int first = 1;


	char* base = NULL;
	char* search = NULL;
	for(i = 0; i < max; i++)
	{
//		printf("(%d): %s\n", i, (&ret1[i])->part);
		if(ostrstr((&ret1[i])->part, " ") != NULL)
		{
			printf("continue\n");
			continue;
		}
		char* x = oltostr(i, 36);

		base = ostrcat(base, "'", 1, 0);
		base = ostrcat(base, x, 1, 0);
		base = ostrcat(base, "\\", 1, 0);		
		search = ostrcat(search, "'", 1, 0);
		search = ostrcat(search, (&ret1[i])->part, 1, 0);
		search = ostrcat(search, "\\", 1, 0);
		p = string_replace(base, search, p, 1);
		free(base), base = NULL;
		free(search), search = NULL;
		
		base = ostrcat(base, "'", 1, 0);
		base = ostrcat(base, x, 1, 0);
		base = ostrcat(base, "'", 1, 0);		
		search = ostrcat(search, "'", 1, 0);
		search = ostrcat(search, (&ret1[i])->part, 1, 0);
		search = ostrcat(search, "'", 1, 0);
		p = string_replace(base, search, p, 1);
		free(base), base = NULL;
		free(search), search = NULL;

		base = ostrcat(base, "'", 1, 0);
		base = ostrcat(base, x, 1, 0);
		base = ostrcat(base, ":", 1, 0);		
		search = ostrcat(search, "'", 1, 0);
		search = ostrcat(search, (&ret1[i])->part, 1, 0);
		search = ostrcat(search, ":", 1, 0);
		p = string_replace(base, search, p, 1);
		free(base), base = NULL;
		free(search), search = NULL;

// 2mal

		base = ostrcat(base, "'", 1, 0);
		base = ostrcat(base, x, 1, 0);
		base = ostrcat(base, ":", 1, 0);		
		search = ostrcat(search, "'", 1, 0);
		search = ostrcat(search, (&ret1[i])->part, 1, 0);
		search = ostrcat(search, ":", 1, 0);
		p = string_replace(base, search, p, 1);
		free(base), base = NULL;
		free(search), search = NULL;
				
		base = ostrcat(base, "/", 1, 0);
		base = ostrcat(base, x, 1, 0);
		base = ostrcat(base, "/", 1, 0);
		search = ostrcat(search, "/", 1, 0);
		search = ostrcat(search, (&ret1[i])->part, 1, 0);
		search = ostrcat(search, "/", 1, 0);
		p = string_replace(base, search, p, 1);
		free(base), base = NULL;
		free(search), search = NULL;

		base = ostrcat(base, ".", 1, 0);
		base = ostrcat(base, x, 1, 0);
		base = ostrcat(base, ".", 1, 0);
		search = ostrcat(search, ".", 1, 0);
		search = ostrcat(search, (&ret1[i])->part, 1, 0);
		search = ostrcat(search, ".", 1, 0);
		p = string_replace(base, search, p, 1);
		free(base), base = NULL;
		free(search), search = NULL;

		base = ostrcat(base, ".", 1, 0);
		base = ostrcat(base, x, 1, 0);
		base = ostrcat(base, "/", 1, 0);
		search = ostrcat(search, ".", 1, 0);
		search = ostrcat(search, (&ret1[i])->part, 1, 0);
		search = ostrcat(search, "/", 1, 0);
		p = string_replace(base, search, p, 1);
		free(base), base = NULL;
		free(search), search = NULL;

		base = ostrcat(base, ".", 1, 0);
		base = ostrcat(base, x, 1, 0);
		base = ostrcat(base, "'", 1, 0);
		search = ostrcat(search, ".", 1, 0);
		search = ostrcat(search, (&ret1[i])->part, 1, 0);
		search = ostrcat(search, "'", 1, 0);
		p = string_replace(base, search, p, 1);
		free(base), base = NULL;
		free(search), search = NULL;
	
		base = ostrcat(base, "/", 1, 0);
		base = ostrcat(base, x, 1, 0);
		base = ostrcat(base, ".", 1, 0);
		search = ostrcat(search, "/", 1, 0);
		search = ostrcat(search, (&ret1[i])->part, 1, 0);
		search = ostrcat(search, ".", 1, 0);
		p = string_replace(base, search, p, 1);
		free(base), base = NULL;
		free(search), search = NULL;

		base = ostrcat(base, ".", 1, 0);
		base = ostrcat(base, x, 1, 0);
		base = ostrcat(base, "(", 1, 0);
		search = ostrcat(search, ".", 1, 0);
		search = ostrcat(search, (&ret1[i])->part, 1, 0);
		search = ostrcat(search, "(", 1, 0);
		p = string_replace(base, search, p, 1);
		free(base), base = NULL;
		free(search), search = NULL;

		base = ostrcat(base, " ", 1, 0);
		base = ostrcat(base, x, 1, 0);
		base = ostrcat(base, "(", 1, 0);
		search = ostrcat(search, " ", 1, 0);
		search = ostrcat(search, (&ret1[i])->part, 1, 0);
		search = ostrcat(search, "(", 1, 0);
		p = string_replace(base, search, p, 1);
		free(base), base = NULL;
		free(search), search = NULL;

		base = ostrcat(base, "=", 1, 0);
		base = ostrcat(base, x, 1, 0);
		base = ostrcat(base, " ", 1, 0);
		search = ostrcat(search, "=", 1, 0);
		search = ostrcat(search, (&ret1[i])->part, 1, 0);
		search = ostrcat(search, " ", 1, 0);
		p = string_replace(base, search, p, 1);
		free(base), base = NULL;
		free(search), search = NULL;

		base = ostrcat(base, " ", 1, 0);
		base = ostrcat(base, x, 1, 0);
		base = ostrcat(base, "=", 1, 0);
		search = ostrcat(search, " ", 1, 0);
		search = ostrcat(search, (&ret1[i])->part, 1, 0);
		search = ostrcat(search, "=", 1, 0);
		p = string_replace(base, search, p, 1);
		free(base), base = NULL;

		base = ostrcat(base, ";", 1, 0);
		base = ostrcat(base, x, 1, 0);
		base = ostrcat(base, ".", 1, 0);
		search = ostrcat(search, ";", 1, 0);
		search = ostrcat(search, (&ret1[i])->part, 1, 0);
		search = ostrcat(search, ".", 1, 0);
		p = string_replace(base, search, p, 1);
		free(base), base = NULL;
		free(search), search = NULL;

		base = ostrcat(base, ".", 1, 0);
		base = ostrcat(base, x, 1, 0);
		base = ostrcat(base, "\\", 1, 0);
		search = ostrcat(search, ".", 1, 0);
		search = ostrcat(search, (&ret1[i])->part, 1, 0);
		search = ostrcat(search, "\\", 1, 0);
		p = string_replace(base, search, p, 1);
		free(base), base = NULL;
		free(search), search = NULL;

		
/*				
//		base = ostrcat(base, " ", 1, 0);
		base = ostrcat(base, x, 1, 0);
		base = ostrcat(base, " ", 1, 0);
//		search = ostrcat(search, " ", 1, 0);
		search = ostrcat(search, (&ret1[i])->part, 1, 0);
		search = ostrcat(search, " ", 1, 0);
		p = string_replace(base, search, p, 1);
		free(base), base = NULL;
		free(search), search = NULL;
*/
//		printf("x: %s - %d\n",x,i);

free(x);

	}

	
	printf("max1: %d\n", max);
	printf("count: %d\n", count);
	printf("p00 in: %s\n", p);
	printf("####################################\n");
//	printf("p1 out: %s\n", p1);
	printf("####################################\n");

	char* tmpstr = NULL;
	tmpstr = oregex(".*file.*(http:.*video.flv).*image.*", p);
	printf("tmpstr: %s\n", tmpstr);
								
    return tmpstr;
}


// flag 1 = youtube streamlink
// flag 2 = rtlnow streamlinksrc
// flag 3 = rtlnow streamlink
// flag 4 = myvideo streamlink
// flag 5 = kinox streamlink
char* getstreamurl(char* link, char* url, char* name, int flag)
{
	debug(99, "link(%d): %s", flag, link);
	char* ip = NULL, *pos = NULL, *path = NULL, *pageUrl = NULL, *playpath = NULL, *video_id = NULL, *source = NULL, *streamurl = NULL;

	if(flag == 4)
	{
		int count = 0;
		struct splitstr* ret1 = NULL;
		ret1 = strsplit(link, ";", &count);
		if(ret1 != NULL && count >= 4)
		{
			link = ostrcat(ret1[0].part, NULL, 0, 0);
			pageUrl = ostrcat(pageUrl, ret1[1].part, 1, 0);
			playpath = ostrcat(playpath, ret1[2].part, 1, 0);
			video_id = ostrcat(video_id, ret1[3].part, 1, 0);

			printf("link: %s\n", link);
			printf("pageUrl: %s\n", pageUrl);
			printf("playpath: %s\n", playpath);											
			printf("video_id: %s\n", video_id);											
		}
		free(ret1), ret1 = NULL;
	}	

	if(flag == 5)
	{
		int count = 0;
		struct splitstr* ret1 = NULL;
		ret1 = strsplit(link, ";", &count);
		if(ret1 != NULL && count >= 3)
		{			
			video_id = ostrcat(video_id, ret1[1].part, 1, 0);
			printf("video_id: %s\n", video_id);

			source = ostrcat(source, ret1[2].part, 1, 0);
			printf("source: %s\n", source);
	
			char* cmd = ostrcat("/var/usr/local/share/titan/plugins/tithek/putlocker.sh ", source, 0, 0);
			cmd = ostrcat(cmd, " ", 1, 0);
			cmd = ostrcat(cmd, video_id, 1, 0);
			printf("cmd: %s\n", cmd);
			
			streamurl = string_newline(command(cmd));
			printf("streamurl: %s\n", streamurl);

//			htmldecode(streamurl, streamurl);
//			printf("streamurl: %s\n", streamurl);

			printf("link: %s\n", link);
			free(cmd), cmd = NULL;
			free(video_id), video_id = NULL;
			free(source), source = NULL;
		}
		free(ret1), ret1 = NULL;
		debug(99, "streamurl: %s", streamurl);
		return streamurl;
	}

	if(flag == 6)
	{
		int count = 0;
		struct splitstr* ret1 = NULL;
		ret1 = strsplit(link, ";", &count);
		if(ret1 != NULL && count >= 3)
		{			
			video_id = ostrcat(video_id, ret1[1].part, 1, 0);
			printf("video_id: %s\n", video_id);

			source = ostrcat(source, ret1[2].part, 1, 0);
			printf("source: %s\n", source);
	
			char* cmd = ostrcat("/var/usr/local/share/titan/plugins/tithek/filenuke.sh ", source, 0, 0);
			cmd = ostrcat(cmd, " ", 1, 0);
			cmd = ostrcat(cmd, video_id, 1, 0);
			printf("cmd: %s\n", cmd);
			
			char* tmpstr = string_newline(command(cmd));

			streamurl = filenuke(tmpstr);

			printf("streamurl: %s\n", streamurl);

//			htmldecode(streamurl, streamurl);
//			printf("streamurl: %s\n", streamurl);

			printf("link: %s\n", link);
			free(cmd), cmd = NULL;
			free(video_id), video_id = NULL;
			free(source), source = NULL;
			
			
		}
		free(ret1), ret1 = NULL;
		debug(99, "streamurl: %s", streamurl);
		return streamurl;
	}
		
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
		
	if(flag == 1)
	{
		tmpstr = string_resub("\": \"url=", "\", \"", tmpstr, 0);
	
		while(ostrstr(tmpstr, ",url=") != NULL)
			tmpstr = string_replace(",url=", "\nurl=", tmpstr, 1);
	
		tmpstr = string_decode(tmpstr, 0);
	
		int count = 0;
		int i = 0;
		struct splitstr* ret1 = NULL;
		ret1 = strsplit(tmpstr, "\n", &count);
		if(ret1 != NULL)
		{
			int max = count;
			for(i = 0; i < max; i++)
			{
				if(ostrstr(ret1[i].part, "type=video/mp4") != NULL)
				{
					streamurl = ostrcat(streamurl, ret1[i].part, 1, 0);
					int count2 = 0;
					struct splitstr* ret2 = NULL;
					ret2 = strsplit(ret1[i].part, "+", &count2);
					if(ret2 != NULL)
					{
						free(streamurl);
						streamurl = ostrcat("", ret2[0].part, 0, 0);
						free(ret2); ret2 = NULL;
					}
				}
			}
			free(ret1); ret1 = NULL;
			
			streamurl = string_replace("url=", "", streamurl, 1);
		}
		free(tmpstr); tmpstr = NULL;
	}
	else if(flag == 2)
	{
		tmpstr = string_resub("data:'", "',", tmpstr, 0);
		debug(99, "tmpstr: %s", tmpstr);

		htmldecode(tmpstr, tmpstr);
		tmpstr = ostrcat(url, tmpstr, 0, 1);
		debug(99, "streamurl: %s", tmpstr);
		streamurl = getstreamurl(tmpstr, url, name, 3);
		free(tmpstr); tmpstr = NULL;
	}
	else if(flag == 3)
	{
//		string_resub("delivery=\"streaming\"><![CDATA[", "]]></filename>", tmpstr, 0);
		tmpstr = string_resub("rtmpe://", ".f4v", tmpstr, 0);
		char* tmpstr9 = NULL;
		tmpstr9 = ostrcat(tmpstr9, tmpstr, 1, 0);
		free(tmpstr), tmpstr = NULL;
		tmpstr = ostrcat("rtmpe://", tmpstr9, 0, 0);
		tmpstr = ostrcat(tmpstr, ".f4v", 1, 0);		
		free(tmpstr9), tmpstr9 = NULL;

		debug(99, "tmpstr: %s", tmpstr);

		int count = 0;
		int i = 0;
		struct splitstr* ret1 = NULL;
		ret1 = strsplit(tmpstr, "/", &count);
		if(ret1 != NULL)
		{
			int max = count;
			char* link = NULL;
			char* path = NULL;
			for(i = 0; i < max; i++)
			{
				if(i < 3)
				{
					if(count > i)
						link = ostrcat(link, (&ret1[i])->part, 1, 0);

					if(i == 0)
						link = ostrcat(link, "//", 1, 0);
					else
						link = ostrcat(link, "/", 1, 0);
				}
				else
				{
					if(count > i)
						path = ostrcat(path, (&ret1[i])->part, 1, 0);
					if(i != max - 1)
						path = ostrcat(path, "/", 1, 0);
				}
			}
			free(ret1), ret1 = NULL;

			debug(99, "link: %s", link);
			debug(99, "path: %s", path);
	
			streamurl = ostrcat(link, " swfVfy=1 playpath=mp4:", 0, 0);
			streamurl = ostrcat(streamurl, path, 1, 0);
			streamurl = ostrcat(streamurl, " app=", 1, 0);
			streamurl = ostrcat(streamurl, name, 1, 0);
			streamurl = ostrcat(streamurl, "/_definst_ pageUrl=", 1, 0);
			streamurl = ostrcat(streamurl, url, 1, 0);
			streamurl = ostrcat(streamurl, "/p/ tcUrl=", 1, 0);
			streamurl = ostrcat(streamurl, link, 1, 0);
			streamurl = ostrcat(streamurl, " swfUrl=", 1, 0);
			streamurl = ostrcat(streamurl, url, 1, 0);
			streamurl = ostrcat(streamurl, "/includes/vodplayer.swf", 1, 0);		
	
			if(link != NULL)
				free(link), link = NULL;
	
			if(path != NULL)
				free(path), path = NULL;
		}
		free(tmpstr); tmpstr = NULL;
		debug(99, "streamurl: %s", streamurl);
	}
	if(flag == 4)
	{
// somtimes segfault
//		writesys("/tmp/tithek/list", tmpstr, 0);
		char* tmpstr_uni = NULL;
		char* b64 = NULL;
		char* key = NULL;

		int count = 0;
		struct splitstr* ret1 = NULL;
		ret1 = strsplit(tmpstr, "=", &count);
		int hlen = 0;

		if(ret1 != NULL && count >= 2)
		{
			debug(99, "ret1[1].part=%s", (ret1[1]).part);
			hlen = strlen(ret1[1].part);
			tmpstr_uni = unhexlify(ret1[1].part);
		}
		free(ret1), ret1 = NULL;

		b64 = ostrcat("c8407a08b3c71ea418ec9dc662f2a56e40cbd6d5a114aa50fb1e1079e17f2b83", MDString(video_id), 0, 1);
		debug(99, "b64=%s", b64);

		key = MDString(b64);
		int slen = 0;
		int klen = 0;
		if(tmpstr_uni != NULL) slen = strlen(tmpstr_uni);
		if(key != NULL) klen = strlen(key);
		
		if(tmpstr_uni != NULL)
		{
			debug(99, "hexlen=%d", hlen);
			hlen /= 2;
			debug(99, "binlen=%d", hlen);
			debug(99, "keylen=%d", klen);
			debug(99, "b64=%s", b64);
			debug(99, "key=%s", key);

			rc4(tmpstr_uni, hlen, key, klen);

			debug(99, "encrypted=%s", tmpstr_uni);		
// somtimes segfault
//			writesys("/tmp/tithek/list_key", tmpstr_uni, 1);			

			debug(99, "pageUrl: %s\n", pageUrl);
			debug(99, "playpath: %s\n", playpath);
			debug(99, "video_id: %s\n", video_id);
			//printf("tmpstr_uni: %s\n",tmpstr_uni);

			htmldecode(tmpstr_uni, tmpstr_uni);
	
			if(ostrstr(tmpstr_uni, "connectionurl='rtmp"))
			{
				printf("found rtmpe:// stream\n");
				source = string_resub("source='", ".flv'", tmpstr_uni, 0);

				url = string_resub("connectionurl='", "'", tmpstr_uni, 0);
	
				if(ostrstr(url, "myvideo2flash"))
				{
					printf("change to rtmpt:// stream\n");
					url = string_replace("rtmpe://", "rtmpt://", url, 1);
				}
	
				streamurl = ostrcat(url, NULL, 0, 0);
				streamurl = ostrcat(streamurl, " ", 1, 0);
				streamurl = ostrcat(streamurl, "tcUrl=", 1, 0);
				streamurl = ostrcat(streamurl, url, 1, 0);
				streamurl = ostrcat(streamurl, " swfVfy=http://is4.myvideo.de/de/player/mingR11q/ming.swf ", 1, 0);
				streamurl = ostrcat(streamurl, pageUrl, 1, 0);
				streamurl = ostrcat(streamurl, " ", 1, 0);
				streamurl = ostrcat(streamurl, "playpath=flv:", 1, 0);
				streamurl = ostrcat(streamurl, source, 1, 0);		
			}
			else
			{		
				printf("rtmpe not found, change to *.flv stream\n");
				source = string_resub("source='", "'", tmpstr_uni, 0);
	
				url = string_resub("path='", "'", tmpstr_uni, 0);
	
				streamurl = ostrcat(url, source, 0, 0);
			}
		}

		free(key); key = NULL;		
		free(b64); b64 = NULL;		
		free(url); url = NULL;
		free(source); source = NULL;		
		free(tmpstr_uni); tmpstr_uni = NULL;
		free(tmpstr); tmpstr = NULL;
		free(pageUrl); pageUrl = NULL;		
		free(playpath); playpath = NULL;
		debug(99, "streamurl: %s", streamurl);
	}
	return streamurl;
}

#endif
