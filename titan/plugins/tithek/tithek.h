#ifndef TITHEK_H
#define TITHEK_H

#define TITHEKPATH "/tmp/tithek"
int tithekdownloadrun = 0;
int tithekdownloadcount = 0;
int tithekrun = 0;
int tithekexit = 0;

//flag 0	- menu
//flag 1	- menu pay hidden tithek_pay=0/1 0=hidden
//flag 2	- http (default streamurl)
//flag 4	- youtube
//flag 5	- rtl2now
//flag 6	- superrtlnow
//flag 7	- rtlnow
//flag 8	- voxnow
//flag 9	- youtube suche 10 
//flag 10	- youtube suche 25
//flag 11	- youtube suche 50
//flag 12	- myvideo
//flag 13	- myvideo search 50
//flag 14	- kinox putlocker/sockshare
//flag 15	- kinox filenuke
//flag 16	- superrtlnow pay
//flag 17	- rtlnow pay
//flag 18	- voxnow pay
//flag 19	- rtl2now pay
//flag 20	- kinox StreamCloud
//flag 21	- kinox search
//flag 22	- kinox hoster
//flag 23	- kinox hoster serie
//flag 24	- kinox flashx
//flag 25	- kinox vidstream
//flag 26	- kinox xvidstage
//flag 27	- kinox nowvideo
//flag 50	- beeg
//flag 66   - coming soon dummy
//flag 1000 - menu pincode
//flag 9999 - menu hidden codecpack

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


void freetithekcontent(struct tithek* node)
{
	if(node == NULL) return;

	free(node->title); node->title = NULL;
	node->link = NULL;
	node->pic = NULL;
	node->localname = NULL;
	node->menutitle = NULL;
	node->flag = 0;
}

int addtithekcontent(struct tithek* node, char *line, int len, int count, int pay)
{
	int ret = 0, i = 0, skip = 0;
	char* tmpstr = NULL, *flag = NULL, *cmd = NULL;

	if(node == NULL) return 1;

	if(len > 0) tmpstr = malloc(len + 1);
	if(tmpstr != NULL)
	{
		memcpy(tmpstr, line, len);
		tmpstr[len] = '\0';

		node->title = tmpstr;

		while(tmpstr[0] != '\0')
		{
			if(tmpstr[0] == '#')
			{
				tmpstr[0] = '\0';
				tmpstr++;
				switch(ret)
				{
					case 0: node->link = tmpstr; break;
					case 1: node->pic = tmpstr; break;
					case 2: node->localname = tmpstr; break;
					case 3: node->menutitle = tmpstr; break;
					case 4: flag = tmpstr; break;
				}

				ret++;
			}
			else
				tmpstr++;
		}
	}

	if(ret != 5)
	{
		if(count > 0)
		{
			err("tithek line %d not ok (ret=%d)", count, ret);
		}
		else
		{
			err("add tithek (ret=%d)", ret);
		}
		freetithekcontent(node);
		return 1;
	}

	if(flag != NULL) node->flag = atoi(flag);

	if(node->flag == 9999 && !file_exist("/var/swap/etc/.codecpack"))
		skip = 1;
	else if(node->flag == 16 && pay == 0)
		skip = 1;
	else if(node->flag == 17 && pay == 0)
		skip = 1;
	else if(node->flag == 18 && pay == 0)
		skip = 1;
	else if(node->flag == 19 && pay == 0)
		skip = 1;
	else if(node->flag == 1 && pay == 0)
		skip = 1;
	else if(node->flag == 9999)
	{
		char* tmp = NULL;

		//cmd = ostrcat(cmd, "wget -s http://", 1, 0);
		cmd = ostrcat(cmd, "kin", 1, 0);
		cmd = ostrcat(cmd, "ox", 1, 0);
		cmd = ostrcat(cmd, ".", 1, 0);
		cmd = ostrcat(cmd, "to", 1, 0);

		//if(system(cmd) != 0)
		for(i = 0; i < 3; i++)
		{
			free(tmp); tmp = NULL;
			tmp = gethttp(cmd, "/", 80, NULL, NULL, NULL, 0);
			if(tmp != NULL) break;
		}
		if(tmp == NULL)
			skip = 1;

		free(tmp); tmp = NULL;
		free(cmd), cmd = NULL;
	}

	if(skip == 1)
	{
		freetithekcontent(node);
		return 1;
	}

	return 0;
}

struct tithek* addtithek(char *line, int len, int count, struct tithek* last, int pay)
{
	//debug(1000, "in");
	struct tithek *newnode = NULL, *prev = NULL, *node = NULL;
	int ret = 0;

	if(line == NULL) return NULL;

	newnode = (struct tithek*)calloc(1, sizeof(struct tithek));
	if(newnode == NULL)
	{
		err("no memory");
		return NULL;
	}

	ret = addtithekcontent(newnode, line, len, count, pay);
	if(ret == 1)
	{
		free(newnode);
		return NULL;
	}

	node = tithek;

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

struct tithek* createtithek(struct tithek* update, char* title, char* link, char* pic, char* localname, char* menutitle, int flag)
{
	int pay = 0;
	struct tithek* tnode = NULL;
	char* tmpstr = NULL;
	
	pay = getconfigint("tithek_pay", NULL);

	title = stringreplacechar(title, '#', ' ');
	link = stringreplacechar(link, '#', ' ');
	pic = stringreplacechar(pic, '#', ' ');
	localname = stringreplacechar(localname, '#', ' ');
	menutitle = stringreplacechar(menutitle, '#', ' ');
	
	tmpstr = ostrcat(tmpstr, title, 1, 0);
	tmpstr = ostrcat(tmpstr, "#", 1, 0);
	tmpstr = ostrcat(tmpstr, link, 1, 0);
	tmpstr = ostrcat(tmpstr, "#", 1, 0);
	tmpstr = ostrcat(tmpstr, pic, 1, 0);
	tmpstr = ostrcat(tmpstr, "#", 1, 0);
	tmpstr = ostrcat(tmpstr, localname, 1, 0);
	tmpstr = ostrcat(tmpstr, "#", 1, 0);
	tmpstr = ostrcat(tmpstr, menutitle, 1, 0);
	tmpstr = ostrcat(tmpstr, "#", 1, 0);
	tmpstr = ostrcat(tmpstr, oitoa(flag), 1, 1);

	tmpstr = string_replace_all("\n", "", tmpstr, 1);
	tmpstr = string_replace_all("\r", "", tmpstr, 1);

	if(update != NULL)
	{
		freetithekcontent(update);
		addtithekcontent(update, tmpstr, strlen(tmpstr), 1, pay);
		tnode = update;
	}
	else
		tnode = addtithek(tmpstr, strlen(tmpstr), 1, NULL, pay);

	free(tmpstr);

	return tnode;
}

/*
struct tithek* addtithek(char *line, int count, struct tithek* last)
{
	//debug(1000, "in");
	struct tithek *newnode = NULL, *prev = NULL, *node = tithek;
	char *link = NULL, *pic = NULL, *title = NULL, *localname = NULL, *menutitle = NULL, *cmd = NULL;
	int ret = 0, skip = 0, i = 0;

	if(line == NULL) return NULL;

	newnode = (struct tithek*)calloc(1, sizeof(struct tithek));
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

	int pay = getconfigint("tithek_pay", NULL);

	ret = sscanf(line, "%[^#]#%[^#]#%[^#]#%[^#]#%[^#]#%d", title, link, pic, localname, menutitle, &newnode->flag);
				
	if(newnode->flag == 9999 && !file_exist("/var/swap/etc/.codecpack"))
		skip = 1;
	else if(newnode->flag == 16 && pay == 0)
		skip = 1;
	else if(newnode->flag == 17 && pay == 0)
		skip = 1;
	else if(newnode->flag == 18 && pay == 0)
		skip = 1;
	else if(newnode->flag == 19 && pay == 0)
		skip = 1;
	else if(newnode->flag == 1 && pay == 0)
		skip = 1;
	else if(newnode->flag == 9999)
	{
		char* tmp = NULL;

		//cmd = ostrcat(cmd, "wget -s http://", 1, 0);
		cmd = ostrcat(cmd, "kin", 1, 0);
		cmd = ostrcat(cmd, "ox", 1, 0);
		cmd = ostrcat(cmd, ".", 1, 0);
		cmd = ostrcat(cmd, "to", 1, 0);

		//if(system(cmd) != 0)
		for(i = 0; i < 3; i++)
		{
			free(tmp); tmp = NULL;
			tmp = gethttp(cmd, "/", 80, NULL, NULL, NULL, 0);
			if(tmp != NULL) break;
		}
		if(tmp == NULL)
			skip = 1;

		free(tmp); tmp = NULL;
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

	newnode->link = ostrshrink(link);
	newnode->pic = ostrshrink(pic);
	newnode->title = ostrshrink(title);
	newnode->localname = ostrshrink(localname);
	newnode->menutitle = ostrshrink(menutitle);

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
*/

int readtithek(const char* filename)
{
	debug(1000, "in");
	FILE *fd = NULL;
	char *fileline = NULL;
	int linecount = 0, len = 0, pay = 0;
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

	pay = getconfigint("tithek_pay", NULL);

	while(fgets(fileline, MINMALLOC, fd) != NULL)
	{
		if(fileline[0] == '\n')
			continue;
		len = strlen(fileline) - 1;
		if(fileline[len] == '\n')
			fileline[len] = '\0';
		if(fileline[len - 1] == '\r')
			fileline[len - 1] = '\0';

		linecount++;

		if(last == NULL) last = tmplast;
		last = addtithek(fileline, len + 2, linecount, last, pay);
		if(last != NULL) tmplast = last;
	}

	free(fileline);
	fclose(fd);
	return linecount;
}

/*
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
*/

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

			freetithekcontent(node);

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

/*
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
*/

void tithekdownloadthread(struct stimerthread* timernode, struct download* node, int flag)
{
	int defpic = 0;

	tithekdownloadcount++;

	if(node != NULL)
	{
		m_lock(&status.tithekmutex, 20);
		if(file_exist(node->filename))
		{
			m_unlock(&status.tithekmutex, 20);
			goto end;
		}

		FILE *fd; fd = fopen(node->filename, "w");
		if(fd != NULL) fclose(fd);
		m_unlock(&status.tithekmutex, 20);

		gethttpreal(node->host, node->page, node->port, node->filename, node->auth, NULL, 0, NULL, NULL, 20000, 0);

		if(tithekrun == 0)
			unlink(node->filename);
		else
		{
			//check file size
			off64_t checkpic = getfilesize(node->filename);
			if(checkpic < 1000) defpic = 1;

			//check file is gif or html
			if(defpic == 0)
			{
				char* tmp = NULL;
				tmp = readbintomem(node->filename, 3);
				if(ostrcmp("GIF", tmp) == 0) defpic = 1; //gif
				if(ostrcmp("<", tmp) == 0) defpic = 1; //html
				free(tmp); tmp = NULL;
			}

			if(defpic == 1)
			{
				m_lock(&status.tithekmutex, 20);
				unlink(node->filename);
				symlink("/var/usr/local/share/titan/plugins/tithek/default.jpg", node->filename);
				m_unlock(&status.tithekmutex, 20);
			}
		}

end:
		free(node->host); node->host = NULL;
		free(node->page); node->page = NULL;
		free(node->filename); node->filename = NULL;
		free(node->auth); node->auth = NULL;
	}

	free(node); node = NULL;

	tithekdownloadcount--;
	tithekdownloadrun = 1;
}

char* tithekdownload(char* link, char* localname, char* pw, int pic, int flag)
{
	int ret = 1, port = 80;
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

	if(ostrstr(ip, ":") != NULL)
	{
		ip = oregex("http://(.*):.*", link);
		port = atoi(oregex("http://.*:(.*)/.*", link));
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
		localfile = ostrcat(getconfig("rec_streampath", NULL), "/", 0, 0);
		if(localname == NULL)
			localfile = ostrcat(localfile, basename(tmpstr), 1, 0);
		else
			localfile = ostrcat(localfile, localname, 1, 0);
	}

	debug(99, "---------------------------------------");
	debug(99, "link: %s", link);
	debug(99, "localname: %s", localname);
	debug(99, "---------------------------------------");
	debug(99, "ip: %s", ip);
	debug(99, "port: %d", port);
	debug(99, "path: %s", path);
	debug(99, "localfile: %s", localfile);
	debug(99, "pw: %s", pw);
	debug(99, "---------------------------------------");

	if(flag == 0)
	{
		if(localfile != NULL && !file_exist(localfile))
		{
			if(pic == 1)
			{
				if(tithekdownloadcount >= 24) //start max 24 threads
					gethttp(ip, path, port, localfile, pw, NULL, 0);
				else
				{
					//dnode is freed in thread
					struct download* dnode = calloc(1, sizeof(struct download));
					if(dnode != NULL)
					{
						dnode->host = ostrcat(ip, NULL, 0, 0);
						dnode->page = ostrcat(path, NULL, 0, 0);
						dnode->port = port;
						dnode->filename = ostrcat(localfile, NULL, 0, 0);
						dnode->auth = ostrcat(pw, NULL, 0, 0);
						dnode->connfd = -1;
						dnode->ret = -1;
						addtimer(&tithekdownloadthread, START, 100, 1, (void*)dnode, NULL, NULL);
					}
				}
			}
			else
				gethttp(ip, path, port, localfile, pw, NULL, 0);			
		}
	}
	else
	{
	
		if(localfile != NULL && file_exist(localfile))
			ret = textbox(_("Message"), _("File exist, overwrite?"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 0, 0);

		if(localfile != NULL && ret == 1)
			screendownload("Download", ip, path, port, localfile, pw, 0);
	}

	free(ip); ip = NULL;
	free(tmpstr); tmpstr = NULL;

	return localfile;
}

int createtithekplay(char* titheklink, struct skin* grid, struct skin* listbox, struct skin* countlabel)
{
	int gridbr = 0, posx = 0, count = 0, sumcount = 0, count1 = 0, pagecount = 0, skip = 0;
	int localfile = 0;
	struct skin* tmp = NULL;
	char* tithekfile = NULL;
	char* tmpstr = NULL;

	if(!ostrncmp("http://", titheklink, 7))
		tithekfile = tithekdownload(titheklink, NULL, HTTPAUTH, 0, 0);
	else
	{
		tithekfile = ostrcat(titheklink, NULL, 0, 0);
		localfile = 1;
	}

	delmarkedscreennodes(grid, 1);
	freetithek();
//	if(readtithek(tithekfile) != 0) return 1;
	int linecount = readtithek(tithekfile);
//	if(linecount == 0) return 1;
	
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

	if(localfile == 0)
		unlink(tithekfile);
	
	free(tithekfile); tithekfile = NULL;
	return pagecount;
}

void removefav(char* title, char* link, char* pic, char* localname, char* menutitle, int flag)
{
	int count = 0, i = 0;
	char* tmpstr = NULL, *tmpstr1 = NULL, *input = NULL;
	struct splitstr* ret = NULL;

	input = ostrcat(input, title, 1, 0);
	input = ostrcat(input, "#", 1, 0);
	input = ostrcat(input, link, 1, 0);
	input = ostrcat(input, "#", 1, 0);
	input = ostrcat(input, pic, 1, 0);
	input = ostrcat(input, "#", 1, 0);
	input = ostrcat(input, localname, 1, 0);
	input = ostrcat(input, "#", 1, 0);
	input = ostrcat(input, menutitle, 1, 0);
	input = ostrcat(input, "#", 1, 0);
	input = ostrcat(input, oitoa(flag), 1, 1);
	
	tmpstr = readfiletomem(getconfig("tithek_fav", NULL), 0);
	
	ret = strsplit(tmpstr, "\n", &count);

	if(ret != NULL)
	{
		for(i = 0; i < count; i++)
		{
			if(ostrcmp((ret[i]).part, input) != 0)
			{
				tmpstr1 = ostrcat(tmpstr1, ret[i].part, 1, 0);
				tmpstr1 = ostrcat(tmpstr1, "\n", 1, 0);
			}
			else
				printf("remove: %s\n", ret[i].part);
		}
	}

	if(tmpstr1 != NULL && strlen(tmpstr1) > 0)
		tmpstr1[strlen(tmpstr1) - 1] = '\0';

	writesys(getconfig("tithek_fav", NULL), tmpstr1, 0);

	free(ret); ret = NULL;
	free(tmpstr); tmpstr = NULL;
	free(tmpstr1); tmpstr1 = NULL;
	free(input); input = NULL;
}

void addfav(char* title, char* link, char* pic, char* localname, char* menutitle, int flag)
{
	int count = 0, i = 0;
	char* tmpstr = NULL, *tmpstr1 = NULL, *input = NULL;
	struct splitstr* ret = NULL;

	input = ostrcat(input, title, 1, 0);
	input = ostrcat(input, "#", 1, 0);
	input = ostrcat(input, link, 1, 0);
	input = ostrcat(input, "#", 1, 0);
	input = ostrcat(input, pic, 1, 0);
	input = ostrcat(input, "#", 1, 0);
	input = ostrcat(input, localname, 1, 0);
	input = ostrcat(input, "#", 1, 0);
	input = ostrcat(input, menutitle, 1, 0);
	input = ostrcat(input, "#", 1, 0);
	input = ostrcat(input, oitoa(flag), 1, 1);
	
	tmpstr1 = ostrcat(tmpstr1, input, 1, 0);
	tmpstr1 = ostrcat(tmpstr1, "\n", 1, 0);

	tmpstr = readfiletomem(getconfig("tithek_fav", NULL), 0);
	
	ret = strsplit(tmpstr, "\n", &count);

	if(ret != NULL)
	{
		for(i = 0; i < count; i++)
		{
			if(ostrcmp((ret[i]).part, input) != 0)
			{
				tmpstr1 = ostrcat(tmpstr1, ret[i].part, 1, 0);
				tmpstr1 = ostrcat(tmpstr1, "\n", 1, 0);
			}
		}
	}

	if(tmpstr1 != NULL && strlen(tmpstr1) > 0)
		tmpstr1[strlen(tmpstr1) - 1] = '\0';

	writesys(getconfig("tithek_fav", NULL), tmpstr1, 0);

	free(ret); ret = NULL;
	free(tmpstr); tmpstr = NULL;
	free(tmpstr1); tmpstr1 = NULL;
	free(input); input = NULL;
}

void cacheplay(char* link, char* filename, int flag)
{
	struct skin* load = getscreen("loadingproz");
	struct skin* proztext = getscreennode(load, "proztext");

	drawscreen(load, 0, 0);	
	int port = 80, count = 0, mcount = 0;
	off64_t size = 0, msize = 0;
	char* host = NULL, *pos = NULL, *path = NULL, *file = NULL, *tmpstr = NULL;
	host = string_replace("http://", "", (char*)link, 0);

	if(host != NULL)
		pos = strchr(host, '/');
	if(pos != NULL)
	{
		pos[0] = '\0';
		path = pos + 1;
	}

	file = ostrcat(getconfig("rec_streampath", NULL), "/.cache.", 0, 0);
	file = ostrcat(file, filename, 1, 0);

	if(ostrstr(host, ":") != NULL)
	{
		host = oregex("http://(.*):.*", link);
		port = atoi(oregex("http://.*:(.*)/.*", link));
	}

	debug(99, "---------------------------------------");
	debug(99, "link: %s", link);
	debug(99, "---------------------------------------");
	debug(99, "host: %s", host);
	debug(99, "port: %d", port);
	debug(99, "path: %s", path);
	debug(99, "local: %s", file);
	debug(99, "---------------------------------------");
	
	
	struct download* dnode = NULL;
	dnode = calloc(1, sizeof(struct download));
	if(dnode == NULL)
	{
		err("no mem");
		return;
	}		
	dnode->host = host;
	dnode->page = path;
	dnode->port = port;
	dnode->filename = file;
	dnode->auth = NULL;
	dnode->connfd = -1;
	dnode->ret = -1;
	
	addtimer(&gethttpstruct, START, 1000, 1, (void*)dnode, NULL, NULL);

	if(flag == 1)
	{
		mcount = 120;
		msize = 10485760;
	}
	else if(flag == 2)
	{
		mcount = 240; 
		msize = 20971520;
	}
	else if(flag == 3)
	{
		mcount = 360; 
		msize = 31457280;
	}
			
	while(count < mcount || size >= msize)
	{
		sleep(1);
		count++;
		if(file_exist(file))
			size = getfilesize(file);

		int proz = 0;
		int proz1 = size * 100 / msize;
		debug(99, "size (%dprozent)", proz1);

		int proz2 = count * 100 / mcount;
		debug(99, "time (%dprozent)", proz2);
		
		if(proz1 > proz2)
			proz = proz1;
		else
			proz = proz2;

		debug(99, "cacheing...(%lldkb) (%dprozent)", size / 1024, proz);

		if(size >= msize)
			break;
		if(count >= mcount)
			break;
		
		tmpstr = ostrcat(_("please wait..."), " (", 0, 0);
		tmpstr = ostrcat(tmpstr, oitoa(proz), 1, 1);
		tmpstr = ostrcat(tmpstr, "%)", 1, 0);
		clearscreen(load);
		changetext(proztext, tmpstr);
		drawscreen(load, 0, 0);
		free(tmpstr), tmpstr = NULL;
	}

	screenplay(file, 2, 0);
	sockclose(&dnode->connfd);
	free(dnode); dnode = NULL;

	tmpstr = ostrcat(tmpstr, _("Remove Cachefile ?"), 1, 0);
	tmpstr = ostrcat(tmpstr, "\n\n", 1, 0);
	tmpstr = ostrcat(tmpstr, file, 1, 0);
	
	if(textbox(_("Message"), tmpstr, _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 0, 0) == 1)
	{
		unlink(file);
	}
	free(tmpstr), tmpstr = NULL;
	free(file), file = NULL;
	free(host), host = NULL;
}
	
void submenu(struct skin* listbox, struct skin* load, char* title)
{
	int flag = 0;
	if(status.security == 1)
	{
		drawscreen(load, 0, 0);	
		char* tmpstr = ostrcat(((struct tithek*)listbox->select->handle)->link, NULL, 0, 0);
		char* tmpstr1 = NULL;

		if(((struct tithek*)listbox->select->handle)->flag == 2)
		{
			if(tmpstr != NULL) tmpstr1 = ostrcat(tmpstr, NULL, 0, 0);
		}						
		else if(((struct tithek*)listbox->select->handle)->flag == 4)
		{
			if(tmpstr != NULL) tmpstr1 = youtube(tmpstr, NULL, NULL, 1);
		}						
		else if(((struct tithek*)listbox->select->handle)->flag == 5)
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
		else if(((struct tithek*)listbox->select->handle)->flag == 12)
		{
			if(tmpstr != NULL) tmpstr1 = myvideo(tmpstr, NULL, NULL, 1);
		}
		else if(((struct tithek*)listbox->select->handle)->flag == 14)
		{
			if(tmpstr != NULL) tmpstr1 = kinox(tmpstr, NULL, NULL, 1);
		}
		else if(((struct tithek*)listbox->select->handle)->flag == 15)
		{
			if(tmpstr != NULL) tmpstr1 = kinox(tmpstr, NULL, NULL, 2);
		}
		else if(((struct tithek*)listbox->select->handle)->flag == 16)
		{
			if(tmpstr != NULL) tmpstr1 = rtl2now(tmpstr, "http://www.superrtlnow.de", "superrtlnow", 1);
		}
		else if(((struct tithek*)listbox->select->handle)->flag == 17)
		{
			if(tmpstr != NULL) tmpstr1 = rtl2now(tmpstr, "http://rtl-now.rtl.de", "rtlnow", 1);
		}
		else if(((struct tithek*)listbox->select->handle)->flag == 18)
		{
			if(tmpstr != NULL) tmpstr1 = rtl2now(tmpstr, "http://www.voxnow.de", "voxnow", 1);
		}
		else if(((struct tithek*)listbox->select->handle)->flag == 19)
		{
			if(tmpstr != NULL) tmpstr1 = rtl2now(tmpstr, "http://rtl2now.rtl2.de", "rtl2now", 1);
		}
		else if(((struct tithek*)listbox->select->handle)->flag == 20)
		{
			if(tmpstr != NULL) tmpstr1 = kinox(tmpstr, NULL, NULL, 3);
		}
		else if(((struct tithek*)listbox->select->handle)->flag == 24)
		{
			if(tmpstr != NULL) tmpstr1 = kinox(tmpstr, NULL, NULL, 4);
		}
		else if(((struct tithek*)listbox->select->handle)->flag == 25)
		{
			if(tmpstr != NULL) tmpstr1 = kinox(tmpstr, NULL, NULL, 5);
		}
		else if(((struct tithek*)listbox->select->handle)->flag == 26)
		{
			if(tmpstr != NULL) tmpstr1 = kinox(tmpstr, NULL, NULL, 6);
		}
		else if(((struct tithek*)listbox->select->handle)->flag == 27)
		{
			if(tmpstr != NULL) tmpstr1 = kinox(tmpstr, NULL, NULL, 7);
		}
		else if(((struct tithek*)listbox->select->handle)->flag == 50)
		{
			if(tmpstr != NULL) tmpstr1 = beeg(tmpstr);
		}

		free(tmpstr); tmpstr = NULL;

		if(ostrcmp(title, "TiThek - Internet Radio") == 0)
			flag = 4;

		if(tmpstr1 != NULL)
		{
			char* filename = ostrcat(title, "_", 0, 0);
			filename = ostrcat(filename, ((struct tithek*)listbox->select->handle)->title, 1, 0);
			filename = ostrcat(filename, ".mp4", 1, 0);
			filename = string_replace_all(" ", ".", filename, 1);
			filename = string_replace_all("-", "_", filename, 1);
			filename = string_replace_all("._.", "_", filename, 1);
			debug(99, "filename: %s", filename);
				
			char* keyconf = NULL;
			char* skintitle = "Choice Playback";
			struct menulist* mlist = NULL, *mbox = NULL;
			
			addmenulist(&mlist, "Streaming Playback (default)", NULL, NULL, 0, 0);
			
			if(!ostrncmp("http://", tmpstr1, 7))
			{
				addmenulist(&mlist, "Streaming Playback Caching (1MB)", NULL, NULL, 0, 0);
				addmenulist(&mlist, "Streaming Playback Caching (2MB)", NULL, NULL, 0, 0);
				addmenulist(&mlist, "Streaming Playback Caching (3MB)", NULL, NULL, 0, 0);
				addmenulist(&mlist, "Streaming Playback Caching (4MB)", NULL, NULL, 0, 0);
				addmenulist(&mlist, "Streaming Playback Caching (5MB)", NULL, NULL, 0, 0);
				if(file_exist(getconfig("rec_streampath", NULL)) && flag != 4)
				{
//					addmenulist(&mlist, "File Caching Playback (10MB / 120s)", NULL, NULL, 0, 0);
//					addmenulist(&mlist, "File Caching Playback (20MB / 240s)", NULL, NULL, 0, 0);
//					addmenulist(&mlist, "File Caching Playback (30MB / 360s)", NULL, NULL, 0, 0);
					addmenulist(&mlist, "Download Full File", NULL, NULL, 0, 0);
				}
			}
			mbox = menulistbox(mlist, NULL, skintitle, NULL, NULL, 1, 0);
			if(mbox != NULL) keyconf = mbox->name;
			if(ostrcmp(keyconf, "Streaming Playback (default)") == 0)
			{
				addconfigtmp("playerbuffersize", "0");
				screenplay(tmpstr1, 2, flag);
				delconfigtmp("playerbuffersize");
			}
			else if(ostrcmp(keyconf, "Streaming Playback Caching (0.5MB)") == 0)
			{
				addconfigtmp("playerbuffersize", "524288");
				screenplay(tmpstr1, 2, flag);
				delconfigtmp("playerbuffersize");
			}
			else if(ostrcmp(keyconf, "Streaming Playback Caching (1MB)") == 0)
			{
				addconfigtmp("playerbuffersize", "1048576");
				screenplay(tmpstr1, 2, flag);
				delconfigtmp("playerbuffersize");
			}
			else if(ostrcmp(keyconf, "Streaming Playback Caching (2MB)") == 0)
			{
				addconfigtmp("playerbuffersize", "2097152");
				screenplay(tmpstr1, 2, flag);
				delconfigtmp("playerbuffersize");
			}
			else if(ostrcmp(keyconf, "Streaming Playback Caching (3MB)") == 0)
			{
				addconfigtmp("playerbuffersize", "3145728");
				screenplay(tmpstr1, 2, flag);
				delconfigtmp("playerbuffersize");
			}
			else if(ostrcmp(keyconf, "Streaming Playback Caching (4MB)") == 0)
			{
				addconfigtmp("playerbuffersize", "4194304");
				screenplay(tmpstr1, 2, flag);
				delconfigtmp("playerbuffersize");
			}
			else if(ostrcmp(keyconf, "Streaming Playback Caching (5MB)") == 0)
			{
				addconfigtmp("playerbuffersize", "5242880");
				screenplay(tmpstr1, 2, flag);
				delconfigtmp("playerbuffersize");
			}
			else if(ostrcmp(keyconf, "File Caching Playback (10MB / 120s)") == 0)
			{
				cacheplay(tmpstr1, filename, 1);
			}
			else if(ostrcmp(keyconf, "File Caching Playback (20MB / 240s)") == 0)
			{
				cacheplay(tmpstr1, filename, 2);
			}
			else if(ostrcmp(keyconf, "File Caching Playback (30MB / 360s)") == 0)
			{
				cacheplay(tmpstr1, filename, 3);
			}
			else if(ostrcmp(keyconf, "Download Full File") == 0)
			{
				char* search = textinput("Search", filename);
				if(search != NULL)
				{	
					char* tmpstr2 = tithekdownload(tmpstr1, search, NULL, 0, 1);
//						drawscreen(grid, 0, 0);
					free(tmpstr2); tmpstr2 = NULL;
				
					if(textbox(_("Message"), _("Start playback"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 0, 0) == 1)
					{
						tmpstr2 = ostrcat(getconfig("rec_streampath", NULL), "/", 0, 0);
						tmpstr2 = ostrcat(tmpstr2, search, 1, 0);
						screenplay(tmpstr2, 2, flag);
						free(tmpstr2); tmpstr2 = NULL;
					}
				}
				free(search), search = NULL;
			}
			free(filename), filename = NULL;
			freemenulist(mlist, 1); mlist = NULL;
		}
		else
			textbox(_("Message"), _("Can't get Streamurl !"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 0, 0);
		
		free(tmpstr1); tmpstr1 = NULL;
	}
	else
		textbox(_("Message"), _("Registration needed, please contact Atemio !"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 1000, 200, 0, 0);
}

void screentithekplay(char* titheklink, char* title, int first)
{
	int rcret = -1, oaktline = 1, oaktpage = -1, ogridcol = 0;

	if(!file_exist("/var/swap/player"))
		mkdir("/var/swap/player", 0777); 

	writesysint("/proc/sys/vm/drop_caches", 3, 0); 
	
	if(first == 1)
	{
		delallfiles("/tmp/tithek", NULL); 
		mkdir("/tmp/tithek", 777);
		if(status.mcaktiv == 0)
		{
			rcret = servicestop(status.aktservice, 1, 1);
			if(rcret == 1) return;
		}
	}

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

	changetitle(grid, _(title));
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
				tmpstr = ostrcat(tmpstr, oitoa(tmp->pagecount), 1, 1);
				tmpstr = ostrcat(tmpstr, " / ", 1, 0);
				tmpstr = ostrcat(tmpstr, oitoa(pagecount), 1, 1);
				tmpstr = ostrcat(tmpstr, _(" )"), 1, 0);
				changetext(countpage, tmpstr);
				free(tmpstr); tmpstr = NULL;
	
				if(tmp->handle != NULL)
				{
					tithekpic = tithekdownload(((struct tithek*)tmp->handle)->pic, ((struct tithek*)tmp->handle)->localname, "aXBrLUdaRmg6RkhaVkJHaG56ZnZFaEZERlRHenVpZjU2NzZ6aGpHVFVHQk5Iam0=", 1, 0);

					/* not working with thread download
					off64_t checkpic = getfilesize(tithekpic);
			
					if(checkpic < 1000)
					{
						free(tithekpic); tithekpic = NULL;
						tithekpic = ostrcat("/var/usr/local/share/titan/plugins/tithek/default.jpg", NULL, 0, 0);
					}
					*/
			
					changepic(tmp, tithekpic);
					free(tithekpic); tithekpic = NULL;
				}
				tmp = tmp->prev;
			}
			tmp = listbox->select;
			if(tmp != NULL) tmp = tmp->next;
			while(tmp != NULL)
			{
				if(tmp->pagecount != listbox->aktpage) break;
				if(tmp->handle != NULL)
				{
					tithekpic = tithekdownload(((struct tithek*)tmp->handle)->pic, ((struct tithek*)tmp->handle)->localname, "aXBrLUdaRmg6RkhaVkJHaG56ZnZFaEZERlRHenVpZjU2NzZ6aGpHVFVHQk5Iam0=", 1, 0);

					/* not working with thread download
					off64_t checkpic = getfilesize(tithekpic);

					if(checkpic < 1000)
					{
						free(tithekpic); tithekpic = NULL;
						tithekpic = ostrcat("/var/usr/local/share/titan/plugins/tithek/default.jpg", NULL, 0, 0);
					}
					*/

					changepic(tmp, tithekpic);
					free(tithekpic); tithekpic = NULL;
				}
				tmp = tmp->next;
			}
		}
		
		int count = getfilecount(TITHEKPATH);
		if(count > 500)
			delallfiles(TITHEKPATH, ".jpg");

		drawscreen(grid, 0, 0);
waitrcstart:
		rcret = waitrc(grid, 2000, 2);

		if(rcret == RCTIMEOUT)
		{
			if(tithekdownloadrun == 1)
			{
				tithekdownloadrun = 0;
				drawscreen(grid, 0, 0);
			}
			goto waitrcstart;
		}

		if(rcret == getrcconfigint("rcblue", NULL))
		{
			tithekexit = 1;
			break;
		}

		if(rcret == getrcconfigint("rcexit", NULL)) break;
		if(rcret == getrcconfigint("rcmenu", NULL))
		{
			screentithek_settings();
			createtithekplay(titheklink, grid, listbox, countlabel);
			drawscreen(grid, 0, 0);
		}
		
		if(rcret == getrcconfigint("rcred", NULL))
		{
			if(listbox->select != NULL && listbox->select->handle != NULL)
			{

				char* tmpstr = ostrcat(((struct tithek*)listbox->select->handle)->title, NULL, 0, 0);
				debug(99, "tmpstr: %s", tmpstr);
				tmpstr = string_replace("-1 (de)", "", tmpstr, 1);
				tmpstr = string_replace("-1 (en)", "", tmpstr, 1);
				tmpstr = string_replace("-1 (\?\?)", "", tmpstr, 1);
				tmpstr = string_replace("-2 (de)", "", tmpstr, 1);
				tmpstr = string_replace("-2 (en)", "", tmpstr, 1);
				tmpstr = string_replace("-2 (\?\?)", "", tmpstr, 1);
				tmpstr = string_replace("-3 (de)", "", tmpstr, 1);
				tmpstr = string_replace("-3 (en)", "", tmpstr, 1);
				tmpstr = string_replace("-3 (\?\?)", "", tmpstr, 1);
				tmpstr = string_replace(" (de)", "", tmpstr, 1);
				tmpstr = string_replace(" (en)", "", tmpstr, 1);
				tmpstr = string_replace(" (\?\?)", "", tmpstr, 1);
				debug(99, "tmpstr: %s", tmpstr);

				playrcred(tmpstr, 1, 0, 99);
				free(tmpstr), tmpstr = NULL;
			}
		}
		else if(rcret == getrcconfigint("rcok", NULL))
		{
			if(listbox->select != NULL && listbox->select->handle != NULL)
			{
				clearscreen(grid);

				if((((struct tithek*)listbox->select->handle)->flag == 2) || (((struct tithek*)listbox->select->handle)->flag == 4) || (((struct tithek*)listbox->select->handle)->flag == 5) || (((struct tithek*)listbox->select->handle)->flag == 6) || (((struct tithek*)listbox->select->handle)->flag == 7) || (((struct tithek*)listbox->select->handle)->flag == 8) || (((struct tithek*)listbox->select->handle)->flag == 12) || (((struct tithek*)listbox->select->handle)->flag == 14) || (((struct tithek*)listbox->select->handle)->flag == 15) || (((struct tithek*)listbox->select->handle)->flag == 16) || (((struct tithek*)listbox->select->handle)->flag == 17) || (((struct tithek*)listbox->select->handle)->flag == 18) || (((struct tithek*)listbox->select->handle)->flag == 19) || (((struct tithek*)listbox->select->handle)->flag == 20) || (((struct tithek*)listbox->select->handle)->flag == 24) || (((struct tithek*)listbox->select->handle)->flag == 25) || (((struct tithek*)listbox->select->handle)->flag == 26) || (((struct tithek*)listbox->select->handle)->flag == 27) || (((struct tithek*)listbox->select->handle)->flag == 50))
				{
					submenu(listbox, load, title);
					drawscreen(grid, 0, 0);
				}
				else if((((struct tithek*)listbox->select->handle)->flag == 9) || (((struct tithek*)listbox->select->handle)->flag == 10) || (((struct tithek*)listbox->select->handle)->flag == 11))
				{
					if(youtube_search(grid, listbox, countlabel, load, ((struct tithek*)listbox->select->handle)->link, ((struct tithek*)listbox->select->handle)->title) == 0)
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
//					if(createtithekplay(titheklink, grid, listbox, countlabel) != 0) break;
						int pagecount = createtithekplay(titheklink, grid, listbox, countlabel);
						if(pagecount == 0 || tithekexit == 1) break;

						listbox->aktpage = oaktpage;
						listbox->aktline = oaktline;
						listbox->gridcol = ogridcol;
						addscreenrc(grid, listbox);
					}
				}
				else if((((struct tithek*)listbox->select->handle)->flag == 13))
				{
					if(myvideo_search(grid, listbox, countlabel, load, ((struct tithek*)listbox->select->handle)->link, ((struct tithek*)listbox->select->handle)->title) == 0)
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
//					if(createtithekplay(titheklink, grid, listbox, countlabel) != 0) break;
						int pagecount = createtithekplay(titheklink, grid, listbox, countlabel);
						if(pagecount == 0 || tithekexit == 1) break;

						listbox->aktpage = oaktpage;
						listbox->aktline = oaktline;
						listbox->gridcol = ogridcol;
						addscreenrc(grid, listbox);
					}
				}
				else if((((struct tithek*)listbox->select->handle)->flag == 21))
				{
					if(kinox_search(grid, listbox, countlabel, load, ((struct tithek*)listbox->select->handle)->link, ((struct tithek*)listbox->select->handle)->title) == 0)
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
//					if(createtithekplay(titheklink, grid, listbox, countlabel) != 0) break;
						int pagecount = createtithekplay(titheklink, grid, listbox, countlabel);
						if(pagecount == 0 || tithekexit == 1) break;

						listbox->aktpage = oaktpage;
						listbox->aktline = oaktline;
						listbox->gridcol = ogridcol;
						addscreenrc(grid, listbox);
					}
				}
				else if((((struct tithek*)listbox->select->handle)->flag == 22))
				{
					if(kinox_hoster(grid, listbox, countlabel, load, ((struct tithek*)listbox->select->handle)->link, ((struct tithek*)listbox->select->handle)->title) == 0)
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
//					if(createtithekplay(titheklink, grid, listbox, countlabel) != 0) break;
						int pagecount = createtithekplay(titheklink, grid, listbox, countlabel);
						if(pagecount == 0 || tithekexit == 1) break;

						listbox->aktpage = oaktpage;
						listbox->aktline = oaktline;
						listbox->gridcol = ogridcol;
						addscreenrc(grid, listbox);
					}
				}			 
				else if((((struct tithek*)listbox->select->handle)->flag == 23))
				{
					if(kinox_hoster_series(grid, listbox, countlabel, load, ((struct tithek*)listbox->select->handle)->link, ((struct tithek*)listbox->select->handle)->title) == 0)
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
//					if(createtithekplay(titheklink, grid, listbox, countlabel) != 0) break;
						int pagecount = createtithekplay(titheklink, grid, listbox, countlabel);
						if(pagecount == 0 || tithekexit == 1) break;

						listbox->aktpage = oaktpage;
						listbox->aktline = oaktline;
						listbox->gridcol = ogridcol;
						addscreenrc(grid, listbox);
					}
				}
				else if((((struct tithek*)listbox->select->handle)->flag == 66))
				{
					textbox(_("Message"), _("The hoster is not yet supported for the time !"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 5, 0);
					continue;
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
						if(pagecount == 0 || tithekexit == 1) break;

						listbox->aktpage = oaktpage;
						listbox->aktline = oaktline;
						listbox->gridcol = ogridcol;
						addscreenrc(grid, listbox);
					}
				}
				drawscreen(grid, 0, 0);
//				sleep(1);
			}			
		}
		else if(rcret == getrcconfigint("rcyellow", NULL) && ostrcmp(title, "TiThek - Favoriten") == 0)
		{
			if(textbox(_("Message"), _("Remove this Favorite ?"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 5, 0) == 1)
			{
				removefav(((struct tithek*)listbox->select->handle)->title, ((struct tithek*)listbox->select->handle)->link, ((struct tithek*)listbox->select->handle)->pic, ((struct tithek*)listbox->select->handle)->localname, ((struct tithek*)listbox->select->handle)->menutitle, ((struct tithek*)listbox->select->handle)->flag);		
				int pagecount = createtithekplay(getconfig("tithek_fav", NULL), grid, listbox, countlabel);
				if(pagecount == 0) return;
					
				drawscreen(grid, 0, 0);
			}
		}
		else if(rcret == getrcconfigint("rcgreen", NULL) && ostrcmp(title, "TiThek - Favoriten") != 0)
		{
			if(textbox(_("Message"), _("Add this link as Favorite ?"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 5, 0) == 1)
			{
				addfav(((struct tithek*)listbox->select->handle)->title, ((struct tithek*)listbox->select->handle)->link, ((struct tithek*)listbox->select->handle)->pic, ((struct tithek*)listbox->select->handle)->localname, ((struct tithek*)listbox->select->handle)->menutitle, ((struct tithek*)listbox->select->handle)->flag);		
			}
		}
	}

	delmarkedscreennodes(grid, 1);
	delownerrc(grid);
	clearscreen(grid);

	if(first == 1)
	{
		freetithek();
		delallfiles("/tmp/tithek", ".list");
		if(status.mcaktiv == 0)
			servicecheckret(servicestart(status.lastservice->channel, NULL, NULL, 0), 0);
	}
}

#endif
