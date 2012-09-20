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
	char *link = NULL, *pic = NULL, *title = NULL, *localname = NULL, *menutitle = NULL, *cmd = NULL;
	int ret = 0, skip = 0;

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
		skip = 1;
	}
	else if(newnode->flag == 9999)
	{
		cmd = ostrcat(cmd, "wget -s http://", 1, 0);
		cmd = ostrcat(cmd, "kin", 1, 0);
		cmd = ostrcat(cmd, "ox", 1, 0);
		cmd = ostrcat(cmd, ".", 1, 0);
		cmd = ostrcat(cmd, "to", 1, 0);

		if(system(cmd) != 0)
			skip = 1;

		free(cmd), cmd = NULL;
	}

	if(skip == 1)
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

					off64_t checkpic = getfilesize(tithekpic);
					printf("getfilesize1: %d\n", checkpic);
			
					if(checkpic < 1500)
					{
						free(tithekpic); tithekpic = NULL;
						tithekpic = ostrcat("/var/usr/local/share/titan/plugins/tithek/default.jpg", NULL, 0, 0);
					}
			
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

					off64_t checkpic = getfilesize(tithekpic);
					printf("getfilesize2: %d\n", checkpic);

					if(checkpic < 1500)
					{
						free(tithekpic); tithekpic = NULL;
						tithekpic = ostrcat("/var/usr/local/share/titan/plugins/tithek/default.jpg", NULL, 0, 0);
					}

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
						if(tmpstr != NULL) tmpstr1 = youtube(tmpstr, NULL, NULL, 1);
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

		if(rcret == getrcconfigint("rcok", NULL))
		{
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
						if(tmpstr != NULL) tmpstr1 = youtube(tmpstr, NULL, NULL, 1);
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
							if(tmpstr != NULL) tmpstr1 = rtl2now(tmpstr, "http://rtl2now.rtl2.de", "rtl2now", 1);
						}
						else if(((struct tithek*)listbox->select->handle)->flag == 6)
						{
							if(tmpstr != NULL) tmpstr1 = rtl2now(tmpstr, "http://www.superrtlnow.de", "superrtlnow", 1);
						}
						else if(((struct tithek*)listbox->select->handle)->flag == 7)
						{
							if(tmpstr != NULL) tmpstr1 = rtl2now(tmpstr, "http://rtl-now.rtl.de", "rtlnow", 1);
						}
						else if(((struct tithek*)listbox->select->handle)->flag == 8)
						{
							if(tmpstr != NULL) tmpstr1 = rtl2now(tmpstr, "http://www.voxnow.de", "voxnow", 1);
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
//										line = ostrcat(line, "#http://www.youtube.com/watch?v=", 1, 0);
										line = ostrcat(line, "#http://www.youtube.com/get_video_info?&video_id=", 1, 0);
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
						if(tmpstr != NULL) tmpstr1 = myvideo(tmpstr, NULL, NULL, 1);
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
						if(tmpstr != NULL) tmpstr1 = kinox(tmpstr, NULL, NULL, 1);
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
				else if(((struct tithek*)listbox->select->handle)->flag == 15)
				{
					if(status.security == 1)
					{
						char* tmpstr = ostrcat(((struct tithek*)listbox->select->handle)->link, NULL, 0, 0);
						char* tmpstr1 = NULL;
						if(tmpstr != NULL) tmpstr1 = kinox(tmpstr, NULL, NULL, 2);
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
				sleep(1);
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

#endif
