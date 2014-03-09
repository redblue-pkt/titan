#ifndef TITHEK_H
#define TITHEK_H

#define TITHEKPATH "/tmp/tithek"
int tithekdownloadrun = 0;
int tithekdownloadcount = 0;
int tithekrun = 0;
int tithekexit = 0;
int tithekmovie4k = 0;
int tithekkinox = 0;
int titheksolarmovie = 0;
int tithekmlehd = 0;

//flag 0	- menu
//flag 1	- menu pay hidden tithek_pay=0/1 0=hidden
//flag 2	- http (default streamurl)
//flag 3	- menu cover
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
//flag 14	- kinox play
//flag 15	- unbenutzt... old: hoster filenuke
//flag 16	- superrtlnow pay
//flag 17	- rtlnow pay
//flag 18	- voxnow pay
//flag 19	- rtl2now pay
//flag 20	- unbenutzt... old: hoster StreamCloud
//flag 21	- kinox search
//flag 22	- kinox hoster
//flag 23	- kinox hoster serie
//flag 24	- internetradio local serach
//flag 25	- kinox local serach
//flag 26	- movie4k local serach
//flag 27	- solarmovie local serach
//flag 28	- solarmovie hoster
//flag 29	- solarmovie search movie
//flag 30	- solarmovie search serie
//flag 31	- kinox search cast
//flag 32	- kinox search cast results
//flag 33	- movie4k search : serie > open seasons and list episode flag=40 > hoster listed flag=34
//flag 34	- movie4k hoster de
//flag 35	- movie4k hoster other
//flag 36	- movie4k series de > open seasons and list episode flag=39 > hoster listed flag=34 
//flag 37	- movie4k series other > open seasons and list episode flag=39 > hoster listed flag=34
//flag 38	- mlehd
//flag 39   - movie4k_hoster_listed
//flag 40   - movie4k_hoster_series
//flag 41   - movie4k
//flag 42   - xvideos
//flag 43	- solarmovie
//flag 44	- myvideo Single charts
//flag 45	- ard
//flag 46	- ard
//flag 47   - internettv local serach
//flag 48   - youtube local serach
//flag 49   - myvideo local serach
//flag 50	- beeg
//flag 51   - ard local serach
//flag 52   - zdf local serach
//flag 53   - tectime local serach
//flag 54   - giga local serach
//flag 55   - beeg local serach
//flag 56   - rtl2now local serach
//flag 57   - rtlnow local serach
//flag 58   - superrtlnow local serach
//flag 59   - voxnow local serach
//flag 60   - xvideos local serach
//flag 61   - mlehd local serach
//flag 62   - netzkino local serach

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
	int ret = 0, i = 0, skip = 0, hid = 0, sos = 0;
	char* tmpstr = NULL, *flag = NULL, *cmd = NULL, *tmp = NULL;
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
	
	sos = getconfigint("sos", NULL);
	hid = getconfigint("tithek_hid_xxx", NULL);
	if(node->flag == 1000 && hid == 1)
		skip = 1;
	else if((node->flag == 9996 || node->flag == 9997 || node->flag == 9998 || node->flag == 9999) && !file_exist("/mnt/swapextensions/etc/.codecpack") && !file_exist("/var/swap/etc/.codecpack") && !file_exist("/var/etc/.codecpack"))
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
	else if(node->flag == 35 && pay == 0)
		skip = 1;
	else if(titheksolarmovie == 1 && node->flag == 9996 && sos == 0)
	{
		cmd = ostrcat(cmd, "ww", 1, 0);
		cmd = ostrcat(cmd, "w.sola", 1, 0);
		cmd = ostrcat(cmd, "rmovi", 1, 0);
		cmd = ostrcat(cmd, "e.", 1, 0);
		cmd = ostrcat(cmd, "so", 1, 0);
		
		for(i = 0; i < 3; i++)
		{
			free(tmp); tmp = NULL;
			tmp = gethttpreal(cmd, "/", 80, NULL, NULL, NULL, 0, NULL, NULL, 5000, 2);
			if(tmp != NULL) break;
		}
		if(tmp == NULL)
			skip = 1;

		titheksolarmovie = 0;
	}
	else if(tithekmovie4k == 1 && node->flag == 9997 && sos == 0)
	{
		cmd = ostrcat(cmd, "ww", 1, 0);
		cmd = ostrcat(cmd, "w.mov", 1, 0);
		cmd = ostrcat(cmd, "ie4k", 1, 0);
		cmd = ostrcat(cmd, ".", 1, 0);
		cmd = ostrcat(cmd, "to", 1, 0);
		
		for(i = 0; i < 3; i++)
		{
			free(tmp); tmp = NULL;
			tmp = gethttpreal(cmd, "/", 80, NULL, NULL, NULL, 0, NULL, NULL, 5000, 2);
			if(tmp != NULL) break;
		}
		if(tmp == NULL)
			skip = 1;

		tithekmovie4k = 0;
	}		
	else if(tithekmlehd == 1 && node->flag == 9998 && sos == 0)
	{
		cmd = ostrcat(cmd, "ww", 1, 0);
		cmd = ostrcat(cmd, "w.mle", 1, 0);
		cmd = ostrcat(cmd, "-hd", 1, 0);
		cmd = ostrcat(cmd, ".", 1, 0);
		cmd = ostrcat(cmd, "se", 1, 0);
		
		for(i = 0; i < 3; i++)
		{
			free(tmp); tmp = NULL;
			tmp = gethttpreal(cmd, "/", 80, NULL, NULL, NULL, 0, NULL, NULL, 5000, 2);
			if(tmp != NULL) break;
		}
		if(tmp == NULL)
			skip = 1;

		tithekmlehd = 0;
	}
	else if(tithekkinox == 1 && node->flag == 9999 && sos == 0)
	{
		cmd = ostrcat(cmd, "kin", 1, 0);
		cmd = ostrcat(cmd, "ox", 1, 0);
		cmd = ostrcat(cmd, ".", 1, 0);
		cmd = ostrcat(cmd, "to", 1, 0);
		
		for(i = 0; i < 3; i++)
		{
			free(tmp); tmp = NULL;
			tmp = gethttpreal(cmd, "/", 80, NULL, NULL, NULL, 0, NULL, NULL, 5000, 2);
			if(tmp != NULL) break;
		}
		if(tmp == NULL)
			skip = 1;

		tithekkinox = 0;
	}

	if(skip == 1)
	{
		freetithekcontent(node);
		return 1;
	}

	free(tmp); tmp = NULL;
	free(cmd), cmd = NULL;
		
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

		gethttpreal(node->host, node->page, node->port, node->filename, node->auth, NULL, 0, NULL, NULL, node->timeout, 0);

		if(tithekrun == 0)
			unlink(node->filename);
		else
		{
			//check file size
			/*
			off64_t checkpic = getfilesize(node->filename);
			if(checkpic < 200) defpic = 1;
			*/

			//check file is gif or html
			if(defpic == 0)
			{
				char* tmp = NULL;
				tmp = readbintomem(node->filename, 3);
				if(ostrncmp("GIF", tmp, 3) == 0) defpic = 1; //gif
				if(ostrncmp("<", tmp, 1) == 0) defpic = 1; //html
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
	int ret = 1, port = 80, timeout = 10000;
	char* ip = NULL, *pos = NULL, *path = NULL;
	char* tmpstr = NULL, *localfile = NULL;

	if(link == NULL) return NULL;
	if(ostrncmp("http://", link, 7)) return NULL;

	ip = string_replace("http://", "", (char*)link, 0);
	// tithek security
	ip = string_replace_all("imageshack.us/md/up/grd/", "atemio.dyndns.tv/", ip, 1);

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

/*
	debug(99, "---------------------------------------");
	debug(99, "link: %s", link);
	debug(99, "localname: %s", localname);
	debug(99, "---------------------------------------");
	debug(99, "ip: %s", ip);
	debug(99, "port: %d", port);
	debug(99, "path: %s", path);
	debug(99, "localfile: %s", localfile);
//	debug(99, "pw: %s", pw);
	debug(99, "---------------------------------------");
*/
	if(flag == 0)
	{
		if(localfile != NULL && !file_exist(localfile))
		{
			if(pic == 1)
			{
				if(tithekdownloadcount >= 24) //start max 24 threads
					gethttp(ip, path, port, localfile, pw, timeout, NULL, 0);
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
						dnode->timeout = timeout;
						addtimer(&tithekdownloadthread, START, 100, 1, (void*)dnode, NULL, NULL);
					}
				}
			}
			else
				gethttp(ip, path, port, localfile, pw, timeout, NULL, 0);			
		}
	}
	else
	{
	
		if(localfile != NULL && file_exist(localfile))
			ret = textbox(_("Message"), _("File exist, overwrite?"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 0, 0);

		if(localfile != NULL && ret == 1)
			screendownload("Download", ip, path, port, localfile, pw, timeout, 0);
	}

	free(ip); ip = NULL;
	free(tmpstr); tmpstr = NULL;

	return localfile;
}

int createtithekplay(char* titheklink, struct skin* grid, struct skin* listbox, struct skin* countlabel, int flag)
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

	int height = 500;
	int width = 590;
	int picheight = 480;
	int picwidth = 570;
	int zcount = 2;
	int fontsize = 25;
	int pcount = 2;

	if(getconfigint("tithek_cover", NULL) == 0 || getconfigint("tithek_view", NULL) == 0)
	{
		if(linecount > 2)
		{
			height = 280;
			width = 390;
			picheight = 230;
			picwidth = 370;
			zcount = 3;
			fontsize = 20;
			pcount = 6;
		}
	 
		if(linecount > 8)
		{
			height = 180;
			width = 295;
			picheight = 130;
			picwidth = 270;
			zcount = 4;
			fontsize = 18;
			pcount = 12;
		}

		if(linecount > 19)
		{
			height = 140;
			width = 235;
			picheight = 90;
			picwidth = 210;
			zcount = 5;
			fontsize = 16;
			pcount = 20;
		}
	
		if(linecount > 25)
		{
			height = 111;
			width = 196;
			picheight = 60;
			picwidth = 150;
			zcount = 6;
			fontsize = 14;
			pcount = 30;
		}
	}
	
	if((getconfigint("tithek_cover", NULL) == 1 && flag == 3) || (getconfigint("tithek_view", NULL) == 1 && flag != 3))
	{
		height = 500;
		width = 590;
		picheight = 480;
		picwidth = 570;
		zcount = 2;
		fontsize = 25;
		pcount = 2;
	}

	if((getconfigint("tithek_cover", NULL) == 2 && flag == 3) || (getconfigint("tithek_view", NULL) == 2 && flag != 3))
	{
		height = 280;
		width = 390;
		picheight = 230;
		picwidth = 370;
		zcount = 3;
		fontsize = 20;
		pcount = 6;
	}

	if((getconfigint("tithek_cover", NULL) == 3 && flag == 3) || (getconfigint("tithek_view", NULL) == 3 && flag != 3))
	{
		height = 180;
		width = 295;
		picheight = 130;
		picwidth = 270;
		zcount = 4;
		fontsize = 18;
		pcount = 12;
	}

	if((getconfigint("tithek_cover", NULL) == 4 && flag == 3) || (getconfigint("tithek_view", NULL) == 4 && flag != 3))
	{
		height = 140;
		width = 235;
		picheight = 90;
		picwidth = 210;
		zcount = 5;
		fontsize = 16;
		pcount = 20;
	}

	if((getconfigint("tithek_cover", NULL) == 5 && flag == 3) || (getconfigint("tithek_view", NULL) == 5 && flag != 3))
	{
		height = 111;
		width = 196;
		picheight = 60;
		picwidth = 150;
		zcount = 6;
		fontsize = 14;
		pcount = 30;
	}

	if(getconfigint("tithek_pic_ratio", NULL) == 1)
	{
		picwidth = 2;
		picheight = 2;
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

	if(tmpstr1 == NULL)
		writesys(getconfig("tithek_fav", NULL), "", 0);
	else
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
	// tithek security
	input = string_replace_all("http://atemio.dyndns.tv/", "http://imageshack.us/md/up/grd/", input, 1);
	
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
	dnode->timeout = 30000;
	
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

	screenplay(file, filename, 2, 0);
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

void backgrounddl(char* link, char* filename)
{
	int port = 80, ret = 0;
	char* host = NULL, *pos = NULL, *path = NULL, *file = NULL, *tmpstr = NULL;
	host = string_replace("http://", "", (char*)link, 0);

	if(host != NULL)
		pos = strchr(host, '/');
	if(pos != NULL)
	{
		pos[0] = '\0';
		path = pos + 1;
	}

	file = ostrcat(getconfig("rec_streampath", NULL), "/", 0, 0);
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
	
	ret = startbgdownload(host, path, port, file, NULL, 30000, 1);
	if(ret == 1)
		textbox(_("Message"), _("Can't start download.\nPlease try later."), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 0, 0);
					
	free(tmpstr), tmpstr = NULL;
	free(file), file = NULL;
	free(host), host = NULL;
}
	
void submenu(struct skin* listbox, struct skin* load, char* title)
{
	int flag = 0;
	if(status.security == 1 || checkbox("WHITEBOX") == 1)
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
			if(tmpstr != NULL) tmpstr1 = kinox(tmpstr);
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
		else if(((struct tithek*)listbox->select->handle)->flag == 38)
		{
			if(tmpstr != NULL) tmpstr1 = mlehd(tmpstr);
		}
		else if(((struct tithek*)listbox->select->handle)->flag == 41)
		{
			if(tmpstr != NULL) tmpstr1 = movie4k(tmpstr);
		}
		else if(((struct tithek*)listbox->select->handle)->flag == 42)
		{
			if(tmpstr != NULL) tmpstr1 = xvideos(tmpstr);
		}
		else if(((struct tithek*)listbox->select->handle)->flag == 43)
		{
			if(tmpstr != NULL) tmpstr1 = solarmovie(tmpstr);
		}
		else if(((struct tithek*)listbox->select->handle)->flag == 45)
		{
			if(tmpstr != NULL) tmpstr1 = ard(tmpstr);
		}
		else if(((struct tithek*)listbox->select->handle)->flag == 46)
		{
			if(tmpstr != NULL) tmpstr1 = zdf(tmpstr);
		}
		else if(((struct tithek*)listbox->select->handle)->flag == 50)
		{
			if(tmpstr != NULL) tmpstr1 = beeg(tmpstr);
		}
		free(tmpstr); tmpstr = NULL;

		if(ostrstr(title, "Internet Radio") != NULL)
			flag = 4;

		if(ostrstr(tmpstr1, "&") != NULL)
		{
			printf("change streamurl from: %s\n", tmpstr1);
			tmpstr1 = string_replace_all("&amp;", "&", tmpstr1, 1);
			printf("change streamurl to: %s\n", tmpstr1);
		}
		
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
			char* skintitle = _("Choice Playback");
			struct menulist* mlist = NULL, *mbox = NULL;

			// needed for autopo
			char* tmptxt = NULL;
			tmptxt = ostrcat(tmptxt, _("Streaming Playback (default)"), 1, 0);
			tmptxt = ostrcat(tmptxt, _("Streaming Playback Caching (0.5MB)"), 1, 0);
			tmptxt = ostrcat(tmptxt, _("Streaming Playback Caching (1MB)"), 1, 0);
			tmptxt = ostrcat(tmptxt, _("Streaming Playback Caching (2MB)"), 1, 0);
			tmptxt = ostrcat(tmptxt, _("Streaming Playback Caching (3MB)"), 1, 0);
			tmptxt = ostrcat(tmptxt, _("Streaming Playback Caching (4MB)"), 1, 0);
			tmptxt = ostrcat(tmptxt, _("Streaming Playback Caching (5MB)"), 1, 0);
			tmptxt = ostrcat(tmptxt, _("Streaming Playback Caching (7.5MB)"), 1, 0);
			tmptxt = ostrcat(tmptxt, _("Streaming Playback Caching (10MB)"), 1, 0);
			tmptxt = ostrcat(tmptxt, _("File Caching Playback (10MB / 120s)"), 1, 0);
			tmptxt = ostrcat(tmptxt, _("File Caching Playback (20MB / 240s)"), 1, 0);
			tmptxt = ostrcat(tmptxt, _("File Caching Playback (30MB / 360s)"), 1, 0);
			tmptxt = ostrcat(tmptxt, _("Download Full File"), 1, 0);
			tmptxt = ostrcat(tmptxt, _("Download Full File (background)"), 1, 0);
			free(tmptxt), tmptxt = NULL;
	
			addmenulist(&mlist, "Streaming Playback (default)", NULL, NULL, 0, 0);

			if(!ostrncmp("http://", tmpstr1, 7))
			{
				if(flag == 4)
				{
					addmenulist(&mlist, "Streaming Playback Caching (0.5MB)", NULL, NULL, 0, 0);
					addmenulist(&mlist, "Streaming Playback Caching (1MB)", NULL, NULL, 0, 0);
				}	
				else if(!ostrncmp("http://", tmpstr1, 7))
				{
//					addmenulist(&mlist, "Streaming Playback Caching (1MB)", NULL, NULL, 0, 0);
//					addmenulist(&mlist, "Streaming Playback Caching (2MB)", NULL, NULL, 0, 0);
//					addmenulist(&mlist, "Streaming Playback Caching (3MB)", NULL, NULL, 0, 0);
//					addmenulist(&mlist, "Streaming Playback Caching (4MB)", NULL, NULL, 0, 0);
					addmenulist(&mlist, "Streaming Playback Caching (5MB)", NULL, NULL, 0, 0);
					addmenulist(&mlist, "Streaming Playback Caching (7.5MB)", NULL, NULL, 0, 0);
					addmenulist(&mlist, "Streaming Playback Caching (10MB)", NULL, NULL, 0, 0);
					if(file_exist(getconfig("rec_streampath", NULL)))
					{
//						addmenulist(&mlist, "File Caching Playback (10MB / 120s)", NULL, NULL, 0, 0);
//						addmenulist(&mlist, "File Caching Playback (20MB / 240s)", NULL, NULL, 0, 0);
//						addmenulist(&mlist, "File Caching Playback (30MB / 360s)", NULL, NULL, 0, 0);
						if(status.expertmodus >= 11 || file_exist("/mnt/swapextensions/etc/.codecpack") || file_exist("/var/swap/etc/.codecpack") || file_exist("/var/etc/.codecpack"))
						{
							addmenulist(&mlist, "Download Full File", NULL, NULL, 0, 0);
							addmenulist(&mlist, "Download Full File (background)", NULL, NULL, 0, 0);
						}
					}
				}
			}
			mbox = menulistbox(mlist, NULL, skintitle, NULL, NULL, 1, 0);
			if(mbox != NULL) keyconf = mbox->name;
			debug(99, "tmpstr1: %s filename: %s flag: %d", tmpstr1, filename, flag);
			if(ostrcmp(keyconf, "Streaming Playback (default)") == 0)
			{
				addconfigtmp("playerbuffersize", "0");
				screenplay(tmpstr1, filename, 2, flag);
				delconfigtmp("playerbuffersize");
			}
			else if(ostrcmp(keyconf, "Streaming Playback Caching (0.5MB)") == 0)
			{
				addconfigtmp("playerbuffersize", "524288");
				screenplay(tmpstr1, filename, 2, flag);
				delconfigtmp("playerbuffersize");
			}
			else if(ostrcmp(keyconf, "Streaming Playback Caching (1MB)") == 0)
			{
				addconfigtmp("playerbuffersize", "1048576");
				screenplay(tmpstr1, filename, 2, flag);
				delconfigtmp("playerbuffersize");
			}
			else if(ostrcmp(keyconf, "Streaming Playback Caching (2MB)") == 0)
			{
				addconfigtmp("playerbuffersize", "2097152");
				screenplay(tmpstr1, filename, 2, flag);
				delconfigtmp("playerbuffersize");
			}
			else if(ostrcmp(keyconf, "Streaming Playback Caching (3MB)") == 0)
			{
				addconfigtmp("playerbuffersize", "3145728");
				screenplay(tmpstr1, filename, 2, flag);
				delconfigtmp("playerbuffersize");
			}
			else if(ostrcmp(keyconf, "Streaming Playback Caching (4MB)") == 0)
			{
				addconfigtmp("playerbuffersize", "4194304");
				screenplay(tmpstr1, filename, 2, flag);
				delconfigtmp("playerbuffersize");
			}
			else if(ostrcmp(keyconf, "Streaming Playback Caching (5MB)") == 0)
			{
				addconfigtmp("playerbuffersize", "5242880");
				screenplay(tmpstr1, filename, 2, flag);
				delconfigtmp("playerbuffersize");
			}
			else if(ostrcmp(keyconf, "Streaming Playback Caching (7.5MB)") == 0)
			{
				addconfigtmp("playerbuffersize", "7864320");
				screenplay(tmpstr1, filename, 2, flag);
				delconfigtmp("playerbuffersize");
			}
			else if(ostrcmp(keyconf, "Streaming Playback Caching (10MB)") == 0)
			{
				addconfigtmp("playerbuffersize", "10485760");
				screenplay(tmpstr1, filename, 2, flag);
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
				char* search = textinput(_("Filename"), filename);
				if(search != NULL)
				{	
					char* tmpstr2 = tithekdownload(tmpstr1, search, NULL, 0, 1);
//						drawscreen(grid, 0, 0);
					free(tmpstr2); tmpstr2 = NULL;
				
					if(textbox(_("Message"), _("Start playback"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 0, 0) == 1)
					{
						tmpstr2 = ostrcat(getconfig("rec_streampath", NULL), "/", 0, 0);
						tmpstr2 = ostrcat(tmpstr2, search, 1, 0);
						screenplay(tmpstr2, filename, 2, flag);
						free(tmpstr2); tmpstr2 = NULL;
					}
				}
				free(search), search = NULL;
			}
			else if(ostrcmp(keyconf, "Download Full File (background)") == 0)
			{
				char* search = textinput(_("Filename"), filename);
				if(search != NULL)
					backgrounddl(tmpstr1, search);
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
	if(!file_exist("/mnt/swapextensions/player"))
		mkdir("/mnt/swapextensions/player", 0777); 

	char* disclaimer = NULL;
	disclaimer = ostrcat(disclaimer, "/mnt/swapextensions/player/tithek_disclaimer_accepted", 1, 0);
				
	if(!file_exist(disclaimer))
	{
		char* tmpstr = gethttp("atemio.dyndns.tv", "/mediathek/disclaimer.txt", 80, NULL, HTTPAUTH, 5000, NULL, 0);
		if(textbox(_("TitanNit Tithek disclaimer"), _(tmpstr), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 1100, 650, 0, 0) == 1)
		{
			writesys(disclaimer, tmpstr, 0);				
			free(tmpstr), tmpstr = NULL;
		}
		else
		{
			free(tmpstr), tmpstr = NULL;
			free(disclaimer), disclaimer = NULL;
			return;
		}
	}
	free(disclaimer), disclaimer = NULL;

	int rcret = -1, oaktline = 1, oaktpage = -1, ogridcol = 0, pagecount = 0;

	writesysint("/proc/sys/vm/drop_caches", 3, 0); 
	
	if(first == 1)
	{ 
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
	struct skin* b5 = getscreennode(grid, "b4");
	struct skin* b4 = getscreennode(grid, "b5");
	struct skin* load = getscreen("loading");
	struct skin* tmp = NULL;
	char* tithekpic = NULL;

	b4->hidden = YES;
	b5->hidden = YES;


	if (ostrcmp(title, "TiThek - Favoriten") == 0)
	{
		b4->hidden = NO;
		b5->hidden = YES;
	}
	else
	{
		b4->hidden = YES;
		b5->hidden = NO;
	}
	
	drawscreen(load, 0, 0);
	
	if(titheklink == NULL) return;
	
	listbox->aktpage = -1;
	listbox->aktline = 1;
	listbox->gridcol = 0;
	listbox->select = NULL;

	pagecount = createtithekplay(titheklink, grid, listbox, countlabel, first);
	if(pagecount == 0) return;

	changetitle(grid, _(title));
	drawscreen(grid, 0, 0);
	addscreenrc(grid, listbox);
				
	while(1)
	{
		changetitle(grid, _(title));
		changetext(countpage, NULL);
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
				tmpstr = ostrcat(tmpstr, " )", 1, 0);
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

			pagecount = createtithekplay(titheklink, grid, listbox, countlabel, first);
			if(pagecount == 0 || tithekexit == 1) break;

			listbox->aktpage = -1;
			listbox->aktline = 1;
			listbox->gridcol = 1;
			addscreenrc(grid, listbox);
			drawscreen(grid, 0, 0);
		}

		if (ostrcmp(title, "TiThek - Favoriten") == 0)
		{
			b4->hidden = NO;
			b5->hidden = YES;
		}
		else
		{
			b4->hidden = YES;
			b5->hidden = NO;
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
				tmpstr = string_replace_all("_", " ", tmpstr, 1);
				tmpstr = string_replace_all("-", " ", tmpstr, 1);
				tmpstr = string_replace_all(".", " ", tmpstr, 1);				
				debug(99, "tmpstr: %s", tmpstr);

				int check = playrcred(tmpstr, NULL, 1, 0, 99);
				if(check == 2)
				{
					if(kinox_search(grid, listbox, countlabel, load, ((struct tithek*)listbox->select->handle)->link, "KinoX - Search", tmpstr, 0) == 0)
					{
						oaktpage = listbox->aktpage;
						oaktline = listbox->aktline;
						ogridcol = listbox->gridcol;
						char* tmpstr = ostrcat(((struct tithek*)listbox->select->handle)->link, NULL, 0, 0);
						char* tmpstr1 = ostrcat("KinoX - Search", " - ", 0, 0);
						char* tmpstr2 = ostrcat(tmpstr1, ((struct tithek*)listbox->select->handle)->title, 1, 0);
						screentithekplay(tmpstr, tmpstr2, 0);
						free(tmpstr); tmpstr = NULL;
						free(tmpstr2); tmpstr2 = NULL;
			
						pagecount = createtithekplay(titheklink, grid, listbox, countlabel, 0);
						if(pagecount == 0 || tithekexit == 1) break;

						listbox->aktpage = oaktpage;
						listbox->aktline = oaktline;
						listbox->gridcol = ogridcol;
						addscreenrc(grid, listbox);
					}
				}
				else if(check == 3)
				{
					if(solarmovie_search(grid, listbox, countlabel, load, ((struct tithek*)listbox->select->handle)->link, "SolarMovies - Movie - Search", tmpstr, 0) == 0)
					{
						oaktpage = listbox->aktpage;
						oaktline = listbox->aktline;
						ogridcol = listbox->gridcol;
						char* tmpstr = ostrcat(((struct tithek*)listbox->select->handle)->link, NULL, 0, 0);
						char* tmpstr1 = ostrcat("SolarMovies - Movie - Search", " - ", 0, 0);
						char* tmpstr2 = ostrcat(tmpstr1, ((struct tithek*)listbox->select->handle)->title, 1, 0);
						screentithekplay(tmpstr, tmpstr2, 0);
						free(tmpstr); tmpstr = NULL;
						free(tmpstr2); tmpstr2 = NULL;
			
						pagecount = createtithekplay(titheklink, grid, listbox, countlabel, 0);
						if(pagecount == 0 || tithekexit == 1) break;

						listbox->aktpage = oaktpage;
						listbox->aktline = oaktline;
						listbox->gridcol = ogridcol;
						addscreenrc(grid, listbox);
					}
				}
				else if(check == 4)
				{
					if(solarmovie_search(grid, listbox, countlabel, load, ((struct tithek*)listbox->select->handle)->link, "SolarMovies - Tv - Search", tmpstr, 1) == 0)
					{
						oaktpage = listbox->aktpage;
						oaktline = listbox->aktline;
						ogridcol = listbox->gridcol;
						char* tmpstr = ostrcat(((struct tithek*)listbox->select->handle)->link, NULL, 0, 0);
						char* tmpstr1 = ostrcat("SolarMovies - Tv - Search", " - ", 0, 0);
						char* tmpstr2 = ostrcat(tmpstr1, ((struct tithek*)listbox->select->handle)->title, 1, 0);
						screentithekplay(tmpstr, tmpstr2, 0);
						free(tmpstr); tmpstr = NULL;
						free(tmpstr2); tmpstr2 = NULL;
			
						pagecount = createtithekplay(titheklink, grid, listbox, countlabel, 0);
						if(pagecount == 0 || tithekexit == 1) break;

						listbox->aktpage = oaktpage;
						listbox->aktline = oaktline;
						listbox->gridcol = ogridcol;
						addscreenrc(grid, listbox);
					}
				}
				else if(check == 5)
				{
					if(youtube_search(grid, listbox, countlabel, load, ((struct tithek*)listbox->select->handle)->link, "Youtube - Search", tmpstr) == 0)
					{
						oaktpage = listbox->aktpage;
						oaktline = listbox->aktline;
						ogridcol = listbox->gridcol;
						char* tmpstr = ostrcat(((struct tithek*)listbox->select->handle)->link, NULL, 0, 0);
						char* tmpstr1 = ostrcat("Youtube - Search", " - ", 0, 0);
						char* tmpstr2 = ostrcat(tmpstr1, ((struct tithek*)listbox->select->handle)->title, 1, 0);
						screentithekplay(tmpstr, tmpstr2, 0);
						free(tmpstr); tmpstr = NULL;
						free(tmpstr2); tmpstr2 = NULL;
			
						pagecount = createtithekplay(titheklink, grid, listbox, countlabel, 0);
						if(pagecount == 0 || tithekexit == 1) break;

						listbox->aktpage = oaktpage;
						listbox->aktline = oaktline;
						listbox->gridcol = ogridcol;
						addscreenrc(grid, listbox);
					}
				}
				else if(check == 6)
				{
					if(myvideo_search(grid, listbox, countlabel, load, ((struct tithek*)listbox->select->handle)->link, "MyVideo - Search", tmpstr, 0) == 0)
					{
						oaktpage = listbox->aktpage;
						oaktline = listbox->aktline;
						ogridcol = listbox->gridcol;
						char* tmpstr = ostrcat(((struct tithek*)listbox->select->handle)->link, NULL, 0, 0);
						char* tmpstr1 = ostrcat("MyVideo - Search", " - ", 0, 0);
						char* tmpstr2 = ostrcat(tmpstr1, ((struct tithek*)listbox->select->handle)->title, 1, 0);
						screentithekplay(tmpstr, tmpstr2, 0);
						free(tmpstr); tmpstr = NULL;
						free(tmpstr2); tmpstr2 = NULL;
			
						pagecount = createtithekplay(titheklink, grid, listbox, countlabel, 0);
						if(pagecount == 0 || tithekexit == 1) break;

						listbox->aktpage = oaktpage;
						listbox->aktline = oaktline;
						listbox->gridcol = ogridcol;
						addscreenrc(grid, listbox);
					}
				}
				else if(check == 7)
				{
					if(movie4k_search(grid, listbox, countlabel, load, ((struct tithek*)listbox->select->handle)->link, "KinoX - Search", tmpstr, 0) == 0)
					{
						oaktpage = listbox->aktpage;
						oaktline = listbox->aktline;
						ogridcol = listbox->gridcol;
						char* tmpstr = ostrcat(((struct tithek*)listbox->select->handle)->link, NULL, 0, 0);
						char* tmpstr1 = ostrcat("Movie4k - Search", " - ", 0, 0);
						char* tmpstr2 = ostrcat(tmpstr1, ((struct tithek*)listbox->select->handle)->title, 1, 0);
						screentithekplay(tmpstr, tmpstr2, 0);
						free(tmpstr); tmpstr = NULL;
						free(tmpstr2); tmpstr2 = NULL;
			
						pagecount = createtithekplay(titheklink, grid, listbox, countlabel, 0);
						if(pagecount == 0 || tithekexit == 1) break;

						listbox->aktpage = oaktpage;
						listbox->aktline = oaktline;
						listbox->gridcol = ogridcol;
						addscreenrc(grid, listbox);
					}
				}
				else if(check == 8)
				{
					if(kinox_search_local(grid, listbox, countlabel, load, ((struct tithek*)listbox->select->handle)->link, "KinoX - Search (local)", tmpstr, 0) == 0)
					{
						oaktpage = listbox->aktpage;
						oaktline = listbox->aktline;
						ogridcol = listbox->gridcol;
						char* tmpstr = ostrcat(((struct tithek*)listbox->select->handle)->link, NULL, 0, 0);
						char* tmpstr1 = ostrcat("KinoX - Search (local)", " - ", 0, 0);
						char* tmpstr2 = ostrcat(tmpstr1, ((struct tithek*)listbox->select->handle)->title, 1, 0);
						screentithekplay(tmpstr, tmpstr2, 0);
						free(tmpstr); tmpstr = NULL;
						free(tmpstr2); tmpstr2 = NULL;
			
						pagecount = createtithekplay(titheklink, grid, listbox, countlabel, 0);
						if(pagecount == 0 || tithekexit == 1) break;

						listbox->aktpage = oaktpage;
						listbox->aktline = oaktline;
						listbox->gridcol = ogridcol;
						addscreenrc(grid, listbox);
					}
				}
				else if(check == 9)
				{
					if(solarmovie_search_local(grid, listbox, countlabel, load, ((struct tithek*)listbox->select->handle)->link, "Solarmovies - Search (local)", tmpstr, 0) == 0)
					{
						oaktpage = listbox->aktpage;
						oaktline = listbox->aktline;
						ogridcol = listbox->gridcol;
						char* tmpstr = ostrcat(((struct tithek*)listbox->select->handle)->link, NULL, 0, 0);
						char* tmpstr1 = ostrcat("Solarmovies - Search (local)", " - ", 0, 0);
						char* tmpstr2 = ostrcat(tmpstr1, ((struct tithek*)listbox->select->handle)->title, 1, 0);
						screentithekplay(tmpstr, tmpstr2, 0);
						free(tmpstr); tmpstr = NULL;
						free(tmpstr2); tmpstr2 = NULL;
			
						pagecount = createtithekplay(titheklink, grid, listbox, countlabel, 0);
						if(pagecount == 0 || tithekexit == 1) break;

						listbox->aktpage = oaktpage;
						listbox->aktline = oaktline;
						listbox->gridcol = ogridcol;
						addscreenrc(grid, listbox);
					}
				}
				else if(check == 10)
				{
					if(youtube_search_local(grid, listbox, countlabel, load, ((struct tithek*)listbox->select->handle)->link, "Youtube - Search (local)", tmpstr, 0) == 0)
					{
						oaktpage = listbox->aktpage;
						oaktline = listbox->aktline;
						ogridcol = listbox->gridcol;
						char* tmpstr = ostrcat(((struct tithek*)listbox->select->handle)->link, NULL, 0, 0);
						char* tmpstr1 = ostrcat("Youtube - Search (local)", " - ", 0, 0);
						char* tmpstr2 = ostrcat(tmpstr1, ((struct tithek*)listbox->select->handle)->title, 1, 0);
						screentithekplay(tmpstr, tmpstr2, 0);
						free(tmpstr); tmpstr = NULL;
						free(tmpstr2); tmpstr2 = NULL;
			
						pagecount = createtithekplay(titheklink, grid, listbox, countlabel, 0);
						if(pagecount == 0 || tithekexit == 1) break;

						listbox->aktpage = oaktpage;
						listbox->aktline = oaktline;
						listbox->gridcol = ogridcol;
						addscreenrc(grid, listbox);
					}
				}
				else if(check == 11)
				{
					if(myvideo_search_local(grid, listbox, countlabel, load, ((struct tithek*)listbox->select->handle)->link, "MyVideo - Search (local)", tmpstr, 0) == 0)
					{
						oaktpage = listbox->aktpage;
						oaktline = listbox->aktline;
						ogridcol = listbox->gridcol;
						char* tmpstr = ostrcat(((struct tithek*)listbox->select->handle)->link, NULL, 0, 0);
						char* tmpstr1 = ostrcat("MyVideo - Search (local)", " - ", 0, 0);
						char* tmpstr2 = ostrcat(tmpstr1, ((struct tithek*)listbox->select->handle)->title, 1, 0);
						screentithekplay(tmpstr, tmpstr2, 0);
						free(tmpstr); tmpstr = NULL;
						free(tmpstr2); tmpstr2 = NULL;
			
						pagecount = createtithekplay(titheklink, grid, listbox, countlabel, 0);
						if(pagecount == 0 || tithekexit == 1) break;

						listbox->aktpage = oaktpage;
						listbox->aktline = oaktline;
						listbox->gridcol = ogridcol;
						addscreenrc(grid, listbox);
					}
				}
				else if(check == 12)
				{
					if(movie4k_search_local(grid, listbox, countlabel, load, ((struct tithek*)listbox->select->handle)->link, "Movie4k - Search (local)", tmpstr, 0) == 0)
					{
						oaktpage = listbox->aktpage;
						oaktline = listbox->aktline;
						ogridcol = listbox->gridcol;
						char* tmpstr = ostrcat(((struct tithek*)listbox->select->handle)->link, NULL, 0, 0);
						char* tmpstr1 = ostrcat("Movie4k - Search (local)", " - ", 0, 0);
						char* tmpstr2 = ostrcat(tmpstr1, ((struct tithek*)listbox->select->handle)->title, 1, 0);
						screentithekplay(tmpstr, tmpstr2, 0);
						free(tmpstr); tmpstr = NULL;
						free(tmpstr2); tmpstr2 = NULL;
			
						pagecount = createtithekplay(titheklink, grid, listbox, countlabel, 0);
						if(pagecount == 0 || tithekexit == 1) break;

						listbox->aktpage = oaktpage;
						listbox->aktline = oaktline;
						listbox->gridcol = ogridcol;
						addscreenrc(grid, listbox);
					}
				}
				else if(check == 13)
				{
					if(internetradio_search_local(grid, listbox, countlabel, load, ((struct tithek*)listbox->select->handle)->link, "Internetradio - Search (local)", tmpstr, 0) == 0)
					{
						oaktpage = listbox->aktpage;
						oaktline = listbox->aktline;
						ogridcol = listbox->gridcol;
						char* tmpstr = ostrcat(((struct tithek*)listbox->select->handle)->link, NULL, 0, 0);
						char* tmpstr1 = ostrcat("Internetradio - Search (local)", " - ", 0, 0);
						char* tmpstr2 = ostrcat(tmpstr1, ((struct tithek*)listbox->select->handle)->title, 1, 0);
						screentithekplay(tmpstr, tmpstr2, 0);
						free(tmpstr); tmpstr = NULL;
						free(tmpstr2); tmpstr2 = NULL;
			
						pagecount = createtithekplay(titheklink, grid, listbox, countlabel, 0);
						if(pagecount == 0 || tithekexit == 1) break;

						listbox->aktpage = oaktpage;
						listbox->aktline = oaktline;
						listbox->gridcol = ogridcol;
						addscreenrc(grid, listbox);
					}
				}
				else if(check == 14)
				{
					if(internettv_search_local(grid, listbox, countlabel, load, ((struct tithek*)listbox->select->handle)->link, "Internettv - Search (local)", tmpstr, 0) == 0)
					{
						oaktpage = listbox->aktpage;
						oaktline = listbox->aktline;
						ogridcol = listbox->gridcol;
						char* tmpstr = ostrcat(((struct tithek*)listbox->select->handle)->link, NULL, 0, 0);
						char* tmpstr1 = ostrcat("Internettv - Search (local)", " - ", 0, 0);
						char* tmpstr2 = ostrcat(tmpstr1, ((struct tithek*)listbox->select->handle)->title, 1, 0);
						screentithekplay(tmpstr, tmpstr2, 0);
						free(tmpstr); tmpstr = NULL;
						free(tmpstr2); tmpstr2 = NULL;
			
						pagecount = createtithekplay(titheklink, grid, listbox, countlabel, 0);
						if(pagecount == 0 || tithekexit == 1) break;

						listbox->aktpage = oaktpage;
						listbox->aktline = oaktline;
						listbox->gridcol = ogridcol;
						addscreenrc(grid, listbox);
					}
				}
				else if(check == 15)
				{
					if(ard_search_local(grid, listbox, countlabel, load, ((struct tithek*)listbox->select->handle)->link, "ARD - Search (local)", tmpstr, 0) == 0)
					{
						oaktpage = listbox->aktpage;
						oaktline = listbox->aktline;
						ogridcol = listbox->gridcol;
						char* tmpstr = ostrcat(((struct tithek*)listbox->select->handle)->link, NULL, 0, 0);
						char* tmpstr1 = ostrcat("ARD - Search (local)", " - ", 0, 0);
						char* tmpstr2 = ostrcat(tmpstr1, ((struct tithek*)listbox->select->handle)->title, 1, 0);
						screentithekplay(tmpstr, tmpstr2, 0);
						free(tmpstr); tmpstr = NULL;
						free(tmpstr2); tmpstr2 = NULL;
			
						pagecount = createtithekplay(titheklink, grid, listbox, countlabel, 0);
						if(pagecount == 0 || tithekexit == 1) break;

						listbox->aktpage = oaktpage;
						listbox->aktline = oaktline;
						listbox->gridcol = ogridcol;
						addscreenrc(grid, listbox);
					}
				}
				else if(check == 16)
				{
					if(zdf_search_local(grid, listbox, countlabel, load, ((struct tithek*)listbox->select->handle)->link, "ZDF - Search (local)", tmpstr, 0) == 0)
					{
						oaktpage = listbox->aktpage;
						oaktline = listbox->aktline;
						ogridcol = listbox->gridcol;
						char* tmpstr = ostrcat(((struct tithek*)listbox->select->handle)->link, NULL, 0, 0);
						char* tmpstr1 = ostrcat("ZDF - Search (local)", " - ", 0, 0);
						char* tmpstr2 = ostrcat(tmpstr1, ((struct tithek*)listbox->select->handle)->title, 1, 0);
						screentithekplay(tmpstr, tmpstr2, 0);
						free(tmpstr); tmpstr = NULL;
						free(tmpstr2); tmpstr2 = NULL;
			
						pagecount = createtithekplay(titheklink, grid, listbox, countlabel, 0);
						if(pagecount == 0 || tithekexit == 1) break;

						listbox->aktpage = oaktpage;
						listbox->aktline = oaktline;
						listbox->gridcol = ogridcol;
						addscreenrc(grid, listbox);
					}
				}
				else if(check == 17)
				{
					if(tectime_search_local(grid, listbox, countlabel, load, ((struct tithek*)listbox->select->handle)->link, "TecTime - Search (local)", tmpstr, 0) == 0)
					{
						oaktpage = listbox->aktpage;
						oaktline = listbox->aktline;
						ogridcol = listbox->gridcol;
						char* tmpstr = ostrcat(((struct tithek*)listbox->select->handle)->link, NULL, 0, 0);
						char* tmpstr1 = ostrcat("TecTime - Search (local)", " - ", 0, 0);
						char* tmpstr2 = ostrcat(tmpstr1, ((struct tithek*)listbox->select->handle)->title, 1, 0);
						screentithekplay(tmpstr, tmpstr2, 0);
						free(tmpstr); tmpstr = NULL;
						free(tmpstr2); tmpstr2 = NULL;
			
						pagecount = createtithekplay(titheklink, grid, listbox, countlabel, 0);
						if(pagecount == 0 || tithekexit == 1) break;

						listbox->aktpage = oaktpage;
						listbox->aktline = oaktline;
						listbox->gridcol = ogridcol;
						addscreenrc(grid, listbox);
					}
				}
				else if(check == 18)
				{
					if(giga_search_local(grid, listbox, countlabel, load, ((struct tithek*)listbox->select->handle)->link, "Giga - Search (local)", tmpstr, 0) == 0)
					{
						oaktpage = listbox->aktpage;
						oaktline = listbox->aktline;
						ogridcol = listbox->gridcol;
						char* tmpstr = ostrcat(((struct tithek*)listbox->select->handle)->link, NULL, 0, 0);
						char* tmpstr1 = ostrcat("Giga - Search (local)", " - ", 0, 0);
						char* tmpstr2 = ostrcat(tmpstr1, ((struct tithek*)listbox->select->handle)->title, 1, 0);
						screentithekplay(tmpstr, tmpstr2, 0);
						free(tmpstr); tmpstr = NULL;
						free(tmpstr2); tmpstr2 = NULL;
			
						pagecount = createtithekplay(titheklink, grid, listbox, countlabel, 0);
						if(pagecount == 0 || tithekexit == 1) break;

						listbox->aktpage = oaktpage;
						listbox->aktline = oaktline;
						listbox->gridcol = ogridcol;
						addscreenrc(grid, listbox);
					}
				}
				else if(check == 19)
				{
					if(beeg_search_local(grid, listbox, countlabel, load, ((struct tithek*)listbox->select->handle)->link, "Beeg - Search (local)", tmpstr, 0) == 0)
					{
						oaktpage = listbox->aktpage;
						oaktline = listbox->aktline;
						ogridcol = listbox->gridcol;
						char* tmpstr = ostrcat(((struct tithek*)listbox->select->handle)->link, NULL, 0, 0);
						char* tmpstr1 = ostrcat("Beeg - Search (local)", " - ", 0, 0);
						char* tmpstr2 = ostrcat(tmpstr1, ((struct tithek*)listbox->select->handle)->title, 1, 0);
						screentithekplay(tmpstr, tmpstr2, 0);
						free(tmpstr); tmpstr = NULL;
						free(tmpstr2); tmpstr2 = NULL;
			
						pagecount = createtithekplay(titheklink, grid, listbox, countlabel, 0);
						if(pagecount == 0 || tithekexit == 1) break;

						listbox->aktpage = oaktpage;
						listbox->aktline = oaktline;
						listbox->gridcol = ogridcol;
						addscreenrc(grid, listbox);
					}
				}
				else if(check == 20)
				{
					if(rtl2now_search_local(grid, listbox, countlabel, load, ((struct tithek*)listbox->select->handle)->link, "Rtl2Now - Search (local)", tmpstr, 0) == 0)
					{
						oaktpage = listbox->aktpage;
						oaktline = listbox->aktline;
						ogridcol = listbox->gridcol;
						char* tmpstr = ostrcat(((struct tithek*)listbox->select->handle)->link, NULL, 0, 0);
						char* tmpstr1 = ostrcat("Rtl2Now - Search (local)", " - ", 0, 0);
						char* tmpstr2 = ostrcat(tmpstr1, ((struct tithek*)listbox->select->handle)->title, 1, 0);
						screentithekplay(tmpstr, tmpstr2, 0);
						free(tmpstr); tmpstr = NULL;
						free(tmpstr2); tmpstr2 = NULL;
			
						pagecount = createtithekplay(titheklink, grid, listbox, countlabel, 0);
						if(pagecount == 0 || tithekexit == 1) break;

						listbox->aktpage = oaktpage;
						listbox->aktline = oaktline;
						listbox->gridcol = ogridcol;
						addscreenrc(grid, listbox);
					}
				}
				else if(check == 21)
				{
					if(rtlnow_search_local(grid, listbox, countlabel, load, ((struct tithek*)listbox->select->handle)->link, "Rtl-Now - Search (local)", tmpstr, 0) == 0)
					{
						oaktpage = listbox->aktpage;
						oaktline = listbox->aktline;
						ogridcol = listbox->gridcol;
						char* tmpstr = ostrcat(((struct tithek*)listbox->select->handle)->link, NULL, 0, 0);
						char* tmpstr1 = ostrcat("Rtl-Now - Search (local)", " - ", 0, 0);
						char* tmpstr2 = ostrcat(tmpstr1, ((struct tithek*)listbox->select->handle)->title, 1, 0);
						screentithekplay(tmpstr, tmpstr2, 0);
						free(tmpstr); tmpstr = NULL;
						free(tmpstr2); tmpstr2 = NULL;
			
						pagecount = createtithekplay(titheklink, grid, listbox, countlabel, 0);
						if(pagecount == 0 || tithekexit == 1) break;

						listbox->aktpage = oaktpage;
						listbox->aktline = oaktline;
						listbox->gridcol = ogridcol;
						addscreenrc(grid, listbox);
					}
				}
				else if(check == 22)
				{
					if(superrtlnow_search_local(grid, listbox, countlabel, load, ((struct tithek*)listbox->select->handle)->link, "SuperRtlNow - Search (local)", tmpstr, 0) == 0)
					{
						oaktpage = listbox->aktpage;
						oaktline = listbox->aktline;
						ogridcol = listbox->gridcol;
						char* tmpstr = ostrcat(((struct tithek*)listbox->select->handle)->link, NULL, 0, 0);
						char* tmpstr1 = ostrcat("SuperRtlNow - Search (local)", " - ", 0, 0);
						char* tmpstr2 = ostrcat(tmpstr1, ((struct tithek*)listbox->select->handle)->title, 1, 0);
						screentithekplay(tmpstr, tmpstr2, 0);
						free(tmpstr); tmpstr = NULL;
						free(tmpstr2); tmpstr2 = NULL;
			
						pagecount = createtithekplay(titheklink, grid, listbox, countlabel, 0);
						if(pagecount == 0 || tithekexit == 1) break;

						listbox->aktpage = oaktpage;
						listbox->aktline = oaktline;
						listbox->gridcol = ogridcol;
						addscreenrc(grid, listbox);
					}
				}
				else if(check == 23)
				{
					if(voxnow_search_local(grid, listbox, countlabel, load, ((struct tithek*)listbox->select->handle)->link, "VoxNow - Search (local)", tmpstr, 0) == 0)
					{
						oaktpage = listbox->aktpage;
						oaktline = listbox->aktline;
						ogridcol = listbox->gridcol;
						char* tmpstr = ostrcat(((struct tithek*)listbox->select->handle)->link, NULL, 0, 0);
						char* tmpstr1 = ostrcat("VoxNow - Search (local)", " - ", 0, 0);
						char* tmpstr2 = ostrcat(tmpstr1, ((struct tithek*)listbox->select->handle)->title, 1, 0);
						screentithekplay(tmpstr, tmpstr2, 0);
						free(tmpstr); tmpstr = NULL;
						free(tmpstr2); tmpstr2 = NULL;
			
						pagecount = createtithekplay(titheklink, grid, listbox, countlabel, 0);
						if(pagecount == 0 || tithekexit == 1) break;

						listbox->aktpage = oaktpage;
						listbox->aktline = oaktline;
						listbox->gridcol = ogridcol;
						addscreenrc(grid, listbox);
					}
				}
				else if(check == 24)
				{
					if(xvideos_search_local(grid, listbox, countlabel, load, ((struct tithek*)listbox->select->handle)->link, "Xvideos - Search (local)", tmpstr, 0) == 0)
					{
						oaktpage = listbox->aktpage;
						oaktline = listbox->aktline;
						ogridcol = listbox->gridcol;
						char* tmpstr = ostrcat(((struct tithek*)listbox->select->handle)->link, NULL, 0, 0);
						char* tmpstr1 = ostrcat("Xvideos - Search (local)", " - ", 0, 0);
						char* tmpstr2 = ostrcat(tmpstr1, ((struct tithek*)listbox->select->handle)->title, 1, 0);
						screentithekplay(tmpstr, tmpstr2, 0);
						free(tmpstr); tmpstr = NULL;
						free(tmpstr2); tmpstr2 = NULL;
			
						pagecount = createtithekplay(titheklink, grid, listbox, countlabel, 0);
						if(pagecount == 0 || tithekexit == 1) break;

						listbox->aktpage = oaktpage;
						listbox->aktline = oaktline;
						listbox->gridcol = ogridcol;
						addscreenrc(grid, listbox);
					}
				}
				else if(check == 25)
				{
					if(mlehd_search_local(grid, listbox, countlabel, load, ((struct tithek*)listbox->select->handle)->link, "Mle-HD - Search (local)", tmpstr, 0) == 0)
					{
						oaktpage = listbox->aktpage;
						oaktline = listbox->aktline;
						ogridcol = listbox->gridcol;
						char* tmpstr = ostrcat(((struct tithek*)listbox->select->handle)->link, NULL, 0, 0);
						char* tmpstr1 = ostrcat("Mle-HD - Search (local)", " - ", 0, 0);
						char* tmpstr2 = ostrcat(tmpstr1, ((struct tithek*)listbox->select->handle)->title, 1, 0);
						screentithekplay(tmpstr, tmpstr2, 0);
						free(tmpstr); tmpstr = NULL;
						free(tmpstr2); tmpstr2 = NULL;
			
						pagecount = createtithekplay(titheklink, grid, listbox, countlabel, 0);
						if(pagecount == 0 || tithekexit == 1) break;

						listbox->aktpage = oaktpage;
						listbox->aktline = oaktline;
						listbox->gridcol = ogridcol;
						addscreenrc(grid, listbox);
					}
				}
				else if(check == 26)
				{
					if(netzkino_search_local(grid, listbox, countlabel, load, ((struct tithek*)listbox->select->handle)->link, "Netzkino - Search (local)", tmpstr, 0) == 0)
					{
						oaktpage = listbox->aktpage;
						oaktline = listbox->aktline;
						ogridcol = listbox->gridcol;
						char* tmpstr = ostrcat(((struct tithek*)listbox->select->handle)->link, NULL, 0, 0);
						char* tmpstr1 = ostrcat("Netzkino - Search (local)", " - ", 0, 0);
						char* tmpstr2 = ostrcat(tmpstr1, ((struct tithek*)listbox->select->handle)->title, 1, 0);
						screentithekplay(tmpstr, tmpstr2, 0);
						free(tmpstr); tmpstr = NULL;
						free(tmpstr2); tmpstr2 = NULL;
			
						pagecount = createtithekplay(titheklink, grid, listbox, countlabel, 0);
						if(pagecount == 0 || tithekexit == 1) break;

						listbox->aktpage = oaktpage;
						listbox->aktline = oaktline;
						listbox->gridcol = ogridcol;
						addscreenrc(grid, listbox);
					}
				}
/*
why ?
				else if((((struct tithek*)listbox->select->handle)->flag == 13))
				{
					if(myvideo_search(grid, listbox, countlabel, load, ((struct tithek*)listbox->select->handle)->link, "MyVideo - Search", tmpstr, 0) == 0)
					{
						oaktpage = listbox->aktpage;
						oaktline = listbox->aktline;
						ogridcol = listbox->gridcol;
						char* tmpstr = ostrcat(((struct tithek*)listbox->select->handle)->link, NULL, 0, 0);
						char* tmpstr1 = ostrcat("MyVideo - Search", " - ", 0, 0);
						char* tmpstr2 = ostrcat(tmpstr1, ((struct tithek*)listbox->select->handle)->title, 1, 0);
						screentithekplay(tmpstr, tmpstr2, 0);
						free(tmpstr); tmpstr = NULL;
						free(tmpstr2); tmpstr2 = NULL;
			
						pagecount = createtithekplay(titheklink, grid, listbox, countlabel, 0);
						if(pagecount == 0 || tithekexit == 1) break;

						listbox->aktpage = oaktpage;
						listbox->aktline = oaktline;
						listbox->gridcol = ogridcol;
						addscreenrc(grid, listbox);
					}
				}
*/
				free(tmpstr), tmpstr = NULL;
			}
		}
		else if(rcret == getrcconfigint("rcok", NULL))
		{
			if(listbox->select != NULL && listbox->select->handle != NULL)
			{
				clearscreen(grid);

				if((((struct tithek*)listbox->select->handle)->flag == 2) || (((struct tithek*)listbox->select->handle)->flag == 4) || (((struct tithek*)listbox->select->handle)->flag == 5) || (((struct tithek*)listbox->select->handle)->flag == 6) || (((struct tithek*)listbox->select->handle)->flag == 7) || (((struct tithek*)listbox->select->handle)->flag == 8) || (((struct tithek*)listbox->select->handle)->flag == 12) || (((struct tithek*)listbox->select->handle)->flag == 14) || (((struct tithek*)listbox->select->handle)->flag == 15) || (((struct tithek*)listbox->select->handle)->flag == 16) || (((struct tithek*)listbox->select->handle)->flag == 17) || (((struct tithek*)listbox->select->handle)->flag == 18) || (((struct tithek*)listbox->select->handle)->flag == 19) || (((struct tithek*)listbox->select->handle)->flag == 20) || (((struct tithek*)listbox->select->handle)->flag == 38) || (((struct tithek*)listbox->select->handle)->flag == 42) || (((struct tithek*)listbox->select->handle)->flag == 45) || (((struct tithek*)listbox->select->handle)->flag == 46) || (((struct tithek*)listbox->select->handle)->flag == 50) || (((struct tithek*)listbox->select->handle)->flag == 41) || (((struct tithek*)listbox->select->handle)->flag == 43))
				{
					submenu(listbox, load, title);
					drawscreen(grid, 0, 0);
				}
				else if((((struct tithek*)listbox->select->handle)->flag == 9) || (((struct tithek*)listbox->select->handle)->flag == 10) || (((struct tithek*)listbox->select->handle)->flag == 11))
				{
					if(youtube_search(grid, listbox, countlabel, load, ((struct tithek*)listbox->select->handle)->link, ((struct tithek*)listbox->select->handle)->title, NULL) == 0)
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
			
						pagecount = createtithekplay(titheklink, grid, listbox, countlabel, 0);
						if(pagecount == 0 || tithekexit == 1) break;

						listbox->aktpage = oaktpage;
						listbox->aktline = oaktline;
						listbox->gridcol = ogridcol;
						addscreenrc(grid, listbox);
					}
				}
				else if(((struct tithek*)listbox->select->handle)->flag == 13)
				{
					if(myvideo_search(grid, listbox, countlabel, load, ((struct tithek*)listbox->select->handle)->link, ((struct tithek*)listbox->select->handle)->title, NULL, 0) == 0)
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
			
						pagecount = createtithekplay(titheklink, grid, listbox, countlabel, 0);
						if(pagecount == 0 || tithekexit == 1) break;

						listbox->aktpage = oaktpage;
						listbox->aktline = oaktline;
						listbox->gridcol = ogridcol;
						addscreenrc(grid, listbox);
					}
				}
				else if(((struct tithek*)listbox->select->handle)->flag == 24)
				{
					if(internetradio_search_local(grid, listbox, countlabel, load, ((struct tithek*)listbox->select->handle)->link, ((struct tithek*)listbox->select->handle)->title, NULL, 0) == 0)
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
			
						pagecount = createtithekplay(titheklink, grid, listbox, countlabel, 0);
						if(pagecount == 0 || tithekexit == 1) break;

						listbox->aktpage = oaktpage;
						listbox->aktline = oaktline;
						listbox->gridcol = ogridcol;
						addscreenrc(grid, listbox);
					}
				}
				else if(((struct tithek*)listbox->select->handle)->flag == 25)
				{
					if(kinox_search_local(grid, listbox, countlabel, load, ((struct tithek*)listbox->select->handle)->link, ((struct tithek*)listbox->select->handle)->title, NULL, 0) == 0)
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
			
						pagecount = createtithekplay(titheklink, grid, listbox, countlabel, 0);
						if(pagecount == 0 || tithekexit == 1) break;

						listbox->aktpage = oaktpage;
						listbox->aktline = oaktline;
						listbox->gridcol = ogridcol;
						addscreenrc(grid, listbox);
					}
				}
				else if(((struct tithek*)listbox->select->handle)->flag == 26)
				{
					if(movie4k_search_local(grid, listbox, countlabel, load, ((struct tithek*)listbox->select->handle)->link, ((struct tithek*)listbox->select->handle)->title, NULL, 0) == 0)
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
			
						pagecount = createtithekplay(titheklink, grid, listbox, countlabel, 0);
						if(pagecount == 0 || tithekexit == 1) break;

						listbox->aktpage = oaktpage;
						listbox->aktline = oaktline;
						listbox->gridcol = ogridcol;
						addscreenrc(grid, listbox);
					}
				}
				else if(((struct tithek*)listbox->select->handle)->flag == 27)
				{
					if(solarmovie_search_local(grid, listbox, countlabel, load, ((struct tithek*)listbox->select->handle)->link, ((struct tithek*)listbox->select->handle)->title, NULL, 0) == 0)
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
			
						pagecount = createtithekplay(titheklink, grid, listbox, countlabel, 0);
						if(pagecount == 0 || tithekexit == 1) break;

						listbox->aktpage = oaktpage;
						listbox->aktline = oaktline;
						listbox->gridcol = ogridcol;
						addscreenrc(grid, listbox);
					}
				}
				else if(((struct tithek*)listbox->select->handle)->flag == 47)
				{
					if(internettv_search_local(grid, listbox, countlabel, load, ((struct tithek*)listbox->select->handle)->link, ((struct tithek*)listbox->select->handle)->title, NULL, 0) == 0)
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
			
						pagecount = createtithekplay(titheklink, grid, listbox, countlabel, 0);
						if(pagecount == 0 || tithekexit == 1) break;

						listbox->aktpage = oaktpage;
						listbox->aktline = oaktline;
						listbox->gridcol = ogridcol;
						addscreenrc(grid, listbox);
					}
				}
				else if(((struct tithek*)listbox->select->handle)->flag == 48)
				{
					if(youtube_search_local(grid, listbox, countlabel, load, ((struct tithek*)listbox->select->handle)->link, ((struct tithek*)listbox->select->handle)->title, NULL, 0) == 0)
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
			
						pagecount = createtithekplay(titheklink, grid, listbox, countlabel, 0);
						if(pagecount == 0 || tithekexit == 1) break;

						listbox->aktpage = oaktpage;
						listbox->aktline = oaktline;
						listbox->gridcol = ogridcol;
						addscreenrc(grid, listbox);
					}
				}
				else if(((struct tithek*)listbox->select->handle)->flag == 49)
				{
					if(myvideo_search_local(grid, listbox, countlabel, load, ((struct tithek*)listbox->select->handle)->link, ((struct tithek*)listbox->select->handle)->title, NULL, 0) == 0)
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
			
						pagecount = createtithekplay(titheklink, grid, listbox, countlabel, 0);
						if(pagecount == 0 || tithekexit == 1) break;

						listbox->aktpage = oaktpage;
						listbox->aktline = oaktline;
						listbox->gridcol = ogridcol;
						addscreenrc(grid, listbox);
					}
				}
				else if(((struct tithek*)listbox->select->handle)->flag == 51)
				{
					if(ard_search_local(grid, listbox, countlabel, load, ((struct tithek*)listbox->select->handle)->link, ((struct tithek*)listbox->select->handle)->title, NULL, 0) == 0)
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
			
						pagecount = createtithekplay(titheklink, grid, listbox, countlabel, 0);
						if(pagecount == 0 || tithekexit == 1) break;

						listbox->aktpage = oaktpage;
						listbox->aktline = oaktline;
						listbox->gridcol = ogridcol;
						addscreenrc(grid, listbox);
					}
				}
				else if(((struct tithek*)listbox->select->handle)->flag == 52)
				{
					if(zdf_search_local(grid, listbox, countlabel, load, ((struct tithek*)listbox->select->handle)->link, ((struct tithek*)listbox->select->handle)->title, NULL, 0) == 0)
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
			
						pagecount = createtithekplay(titheklink, grid, listbox, countlabel, 0);
						if(pagecount == 0 || tithekexit == 1) break;

						listbox->aktpage = oaktpage;
						listbox->aktline = oaktline;
						listbox->gridcol = ogridcol;
						addscreenrc(grid, listbox);
					}
				}
				else if(((struct tithek*)listbox->select->handle)->flag == 53)
				{
					if(tectime_search_local(grid, listbox, countlabel, load, ((struct tithek*)listbox->select->handle)->link, ((struct tithek*)listbox->select->handle)->title, NULL, 0) == 0)
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
			
						pagecount = createtithekplay(titheklink, grid, listbox, countlabel, 0);
						if(pagecount == 0 || tithekexit == 1) break;

						listbox->aktpage = oaktpage;
						listbox->aktline = oaktline;
						listbox->gridcol = ogridcol;
						addscreenrc(grid, listbox);
					}
				}
				else if(((struct tithek*)listbox->select->handle)->flag == 54)
				{
					if(giga_search_local(grid, listbox, countlabel, load, ((struct tithek*)listbox->select->handle)->link, ((struct tithek*)listbox->select->handle)->title, NULL, 0) == 0)
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
			
						pagecount = createtithekplay(titheklink, grid, listbox, countlabel, 0);
						if(pagecount == 0 || tithekexit == 1) break;

						listbox->aktpage = oaktpage;
						listbox->aktline = oaktline;
						listbox->gridcol = ogridcol;
						addscreenrc(grid, listbox);
					}
				}
				else if(((struct tithek*)listbox->select->handle)->flag == 55)
				{
					if(beeg_search_local(grid, listbox, countlabel, load, ((struct tithek*)listbox->select->handle)->link, ((struct tithek*)listbox->select->handle)->title, NULL, 0) == 0)
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
			
						pagecount = createtithekplay(titheklink, grid, listbox, countlabel, 0);
						if(pagecount == 0 || tithekexit == 1) break;

						listbox->aktpage = oaktpage;
						listbox->aktline = oaktline;
						listbox->gridcol = ogridcol;
						addscreenrc(grid, listbox);
					}
				}
				else if(((struct tithek*)listbox->select->handle)->flag == 56)
				{
					if(rtl2now_search_local(grid, listbox, countlabel, load, ((struct tithek*)listbox->select->handle)->link, ((struct tithek*)listbox->select->handle)->title, NULL, 0) == 0)
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
			
						pagecount = createtithekplay(titheklink, grid, listbox, countlabel, 0);
						if(pagecount == 0 || tithekexit == 1) break;

						listbox->aktpage = oaktpage;
						listbox->aktline = oaktline;
						listbox->gridcol = ogridcol;
						addscreenrc(grid, listbox);
					}
				}
				else if(((struct tithek*)listbox->select->handle)->flag == 57)
				{
					if(rtlnow_search_local(grid, listbox, countlabel, load, ((struct tithek*)listbox->select->handle)->link, ((struct tithek*)listbox->select->handle)->title, NULL, 0) == 0)
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
			
						pagecount = createtithekplay(titheklink, grid, listbox, countlabel, 0);
						if(pagecount == 0 || tithekexit == 1) break;

						listbox->aktpage = oaktpage;
						listbox->aktline = oaktline;
						listbox->gridcol = ogridcol;
						addscreenrc(grid, listbox);
					}
				}
				else if(((struct tithek*)listbox->select->handle)->flag == 58)
				{
					if(superrtlnow_search_local(grid, listbox, countlabel, load, ((struct tithek*)listbox->select->handle)->link, ((struct tithek*)listbox->select->handle)->title, NULL, 0) == 0)
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
			
						pagecount = createtithekplay(titheklink, grid, listbox, countlabel, 0);
						if(pagecount == 0 || tithekexit == 1) break;

						listbox->aktpage = oaktpage;
						listbox->aktline = oaktline;
						listbox->gridcol = ogridcol;
						addscreenrc(grid, listbox);
					}
				}
				else if(((struct tithek*)listbox->select->handle)->flag == 59)
				{
					if(voxnow_search_local(grid, listbox, countlabel, load, ((struct tithek*)listbox->select->handle)->link, ((struct tithek*)listbox->select->handle)->title, NULL, 0) == 0)
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
			
						pagecount = createtithekplay(titheklink, grid, listbox, countlabel, 0);
						if(pagecount == 0 || tithekexit == 1) break;

						listbox->aktpage = oaktpage;
						listbox->aktline = oaktline;
						listbox->gridcol = ogridcol;
						addscreenrc(grid, listbox);
					}
				}
				else if(((struct tithek*)listbox->select->handle)->flag == 60)
				{
					if(xvideos_search_local(grid, listbox, countlabel, load, ((struct tithek*)listbox->select->handle)->link, ((struct tithek*)listbox->select->handle)->title, NULL, 0) == 0)
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
			
						pagecount = createtithekplay(titheklink, grid, listbox, countlabel, 0);
						if(pagecount == 0 || tithekexit == 1) break;

						listbox->aktpage = oaktpage;
						listbox->aktline = oaktline;
						listbox->gridcol = ogridcol;
						addscreenrc(grid, listbox);
					}
				}
				else if(((struct tithek*)listbox->select->handle)->flag == 61)
				{
					if(mlehd_search_local(grid, listbox, countlabel, load, ((struct tithek*)listbox->select->handle)->link, ((struct tithek*)listbox->select->handle)->title, NULL, 0) == 0)
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
			
						pagecount = createtithekplay(titheklink, grid, listbox, countlabel, 0);
						if(pagecount == 0 || tithekexit == 1) break;

						listbox->aktpage = oaktpage;
						listbox->aktline = oaktline;
						listbox->gridcol = ogridcol;
						addscreenrc(grid, listbox);
					}
				}
				else if(((struct tithek*)listbox->select->handle)->flag == 62)
				{
					if(netzkino_search_local(grid, listbox, countlabel, load, ((struct tithek*)listbox->select->handle)->link, ((struct tithek*)listbox->select->handle)->title, NULL, 0) == 0)
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
			
						pagecount = createtithekplay(titheklink, grid, listbox, countlabel, 0);
						if(pagecount == 0 || tithekexit == 1) break;

						listbox->aktpage = oaktpage;
						listbox->aktline = oaktline;
						listbox->gridcol = ogridcol;
						addscreenrc(grid, listbox);
					}
				}
				else if(((struct tithek*)listbox->select->handle)->flag == 44)
				{
					if(myvideo_search(grid, listbox, countlabel, load, ((struct tithek*)listbox->select->handle)->link, ((struct tithek*)listbox->select->handle)->title, NULL, 1) == 0)
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
			
						pagecount = createtithekplay(titheklink, grid, listbox, countlabel, 0);
						if(pagecount == 0 || tithekexit == 1) break;

						listbox->aktpage = oaktpage;
						listbox->aktline = oaktline;
						listbox->gridcol = ogridcol;
						addscreenrc(grid, listbox);
					}
				}
				else if((((struct tithek*)listbox->select->handle)->flag == 21))
				{
					if(kinox_search(grid, listbox, countlabel, load, ((struct tithek*)listbox->select->handle)->link, ((struct tithek*)listbox->select->handle)->title, NULL, 0) == 0)
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

						pagecount = createtithekplay(titheklink, grid, listbox, countlabel, 0);
						if(pagecount == 0 || tithekexit == 1) break;

						listbox->aktpage = oaktpage;
						listbox->aktline = oaktline;
						listbox->gridcol = ogridcol;
						addscreenrc(grid, listbox);
					}
				}
				else if((((struct tithek*)listbox->select->handle)->flag == 33))
				{
					if(movie4k_search(grid, listbox, countlabel, load, ((struct tithek*)listbox->select->handle)->link, ((struct tithek*)listbox->select->handle)->title, NULL, 0) == 0)
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

						pagecount = createtithekplay(titheklink, grid, listbox, countlabel, 0);
						if(pagecount == 0 || tithekexit == 1) break;

						listbox->aktpage = oaktpage;
						listbox->aktline = oaktline;
						listbox->gridcol = ogridcol;
						addscreenrc(grid, listbox);
					}
				}				
				else if(((struct tithek*)listbox->select->handle)->flag == 29)
				{
					if(solarmovie_search(grid, listbox, countlabel, load, ((struct tithek*)listbox->select->handle)->link, ((struct tithek*)listbox->select->handle)->title, NULL, 0) == 0)
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

						pagecount = createtithekplay(titheklink, grid, listbox, countlabel, 0);
						if(pagecount == 0 || tithekexit == 1) break;

						listbox->aktpage = oaktpage;
						listbox->aktline = oaktline;
						listbox->gridcol = ogridcol;
						addscreenrc(grid, listbox);
					}
				}
				else if(((struct tithek*)listbox->select->handle)->flag == 30)
				{
					if(solarmovie_search(grid, listbox, countlabel, load, ((struct tithek*)listbox->select->handle)->link, ((struct tithek*)listbox->select->handle)->title, NULL, 1) == 0)
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

						pagecount = createtithekplay(titheklink, grid, listbox, countlabel, 0);
						if(pagecount == 0 || tithekexit == 1) break;

						listbox->aktpage = oaktpage;
						listbox->aktline = oaktline;
						listbox->gridcol = ogridcol;
						addscreenrc(grid, listbox);
					}
				}
				else if((((struct tithek*)listbox->select->handle)->flag == 31))
				{
					if(kinox_search_cast(grid, listbox, countlabel, load, ((struct tithek*)listbox->select->handle)->link, ((struct tithek*)listbox->select->handle)->title, NULL) == 0)
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

						pagecount = createtithekplay(titheklink, grid, listbox, countlabel, 0);
						if(pagecount == 0 || tithekexit == 1) break;

						listbox->aktpage = oaktpage;
						listbox->aktline = oaktline;
						listbox->gridcol = ogridcol;
						addscreenrc(grid, listbox);
					}
				}
				else if((((struct tithek*)listbox->select->handle)->flag == 32))
				{
					if(kinox_search(grid, listbox, countlabel, load, ((struct tithek*)listbox->select->handle)->link, ((struct tithek*)listbox->select->handle)->title, NULL, 1) == 0)
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

						pagecount = createtithekplay(titheklink, grid, listbox, countlabel, 0);
						if(pagecount == 0 || tithekexit == 1) break;

						listbox->aktpage = oaktpage;
						listbox->aktline = oaktline;
						listbox->gridcol = ogridcol;
						addscreenrc(grid, listbox);
					}
				}
				else if((((struct tithek*)listbox->select->handle)->flag == 33))
				{
					if(movie4k_search(grid, listbox, countlabel, load, ((struct tithek*)listbox->select->handle)->link, ((struct tithek*)listbox->select->handle)->title, NULL, 1) == 0)
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

						pagecount = createtithekplay(titheklink, grid, listbox, countlabel, 3);
						if(pagecount == 0 || tithekexit == 1) break;

						listbox->aktpage = oaktpage;
						listbox->aktline = oaktline;
						listbox->gridcol = ogridcol;
						addscreenrc(grid, listbox);
					}
				}
				else if((((struct tithek*)listbox->select->handle)->flag == 34) || (((struct tithek*)listbox->select->handle)->flag == 35))
				{
					if(movie4k_hoster(grid, listbox, countlabel, load, ((struct tithek*)listbox->select->handle)->link, ((struct tithek*)listbox->select->handle)->title) == 0)
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

						pagecount = createtithekplay(titheklink, grid, listbox, countlabel, 0);
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

						pagecount = createtithekplay(titheklink, grid, listbox, countlabel, 3);
						if(pagecount == 0 || tithekexit == 1) break;

						listbox->aktpage = oaktpage;
						listbox->aktline = oaktline;
						listbox->gridcol = ogridcol;
						addscreenrc(grid, listbox);
					}
				}
				else if((((struct tithek*)listbox->select->handle)->flag == 28))
				{
					if(solarmovie_hoster(grid, listbox, countlabel, load, ((struct tithek*)listbox->select->handle)->link, ((struct tithek*)listbox->select->handle)->title) == 0)
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
			
						pagecount = createtithekplay(titheklink, grid, listbox, countlabel, 3);
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
			
						pagecount = createtithekplay(titheklink, grid, listbox, countlabel, 0);
						if(pagecount == 0 || tithekexit == 1) break;

						listbox->aktpage = oaktpage;
						listbox->aktline = oaktline;
						listbox->gridcol = ogridcol;
						addscreenrc(grid, listbox);
					}
				}
				else if((((struct tithek*)listbox->select->handle)->flag == 36) || (((struct tithek*)listbox->select->handle)->flag == 37))
				{
					if(movie4k_series(grid, listbox, countlabel, load, ((struct tithek*)listbox->select->handle)->link, ((struct tithek*)listbox->select->handle)->title) == 0)
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
			
						pagecount = createtithekplay(titheklink, grid, listbox, countlabel, 0);
						if(pagecount == 0 || tithekexit == 1) break;

						listbox->aktpage = oaktpage;
						listbox->aktline = oaktline;
						listbox->gridcol = ogridcol;
						addscreenrc(grid, listbox);
					}
				}
				else if((((struct tithek*)listbox->select->handle)->flag == 39))
				{
					if(movie4k_series_listed(grid, listbox, countlabel, load, ((struct tithek*)listbox->select->handle)->link, ((struct tithek*)listbox->select->handle)->title) == 0)
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
			
						pagecount = createtithekplay(titheklink, grid, listbox, countlabel, 0);
						if(pagecount == 0 || tithekexit == 1) break;

						listbox->aktpage = oaktpage;
						listbox->aktline = oaktline;
						listbox->gridcol = ogridcol;
						addscreenrc(grid, listbox);
					}
				}
				else if((((struct tithek*)listbox->select->handle)->flag == 40))
				{
					if(movie4k_hoster_series(grid, listbox, countlabel, load, ((struct tithek*)listbox->select->handle)->link, ((struct tithek*)listbox->select->handle)->title) == 0)
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
			
						pagecount = createtithekplay(titheklink, grid, listbox, countlabel, 0);
						if(pagecount == 0 || tithekexit == 1) break;

						listbox->aktpage = oaktpage;
						listbox->aktline = oaktline;
						listbox->gridcol = ogridcol;
						addscreenrc(grid, listbox);
					}
				}
				else if((((struct tithek*)listbox->select->handle)->flag == 66))
				{
					textbox(_("Message"), _("The hoster is not yet supported !"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 5, 0);
					continue;
				}	
				else if((((struct tithek*)listbox->select->handle)->flag == 3))
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
						screentithekplay(tmpstr, tmpstr2, 3);
						free(tmpstr); tmpstr = NULL;
						free(tmpstr2); tmpstr2 = NULL;	

						pagecount = createtithekplay(titheklink, grid, listbox, countlabel, 0);
						if(pagecount == 0 || tithekexit == 1) break;

						listbox->aktpage = oaktpage;
						listbox->aktline = oaktline;
						listbox->gridcol = ogridcol;
						addscreenrc(grid, listbox);
					}
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

						pagecount = createtithekplay(titheklink, grid, listbox, countlabel, 0);
						if(pagecount == 0 || tithekexit == 1) break;

						listbox->aktpage = oaktpage;
						listbox->aktline = oaktline;
						listbox->gridcol = ogridcol;
						addscreenrc(grid, listbox);
					}
				}
				drawscreen(grid, 0, 0);
			}			
		}
		else if(rcret == getrcconfigint("rcyellow", NULL) && ostrcmp(title, "TiThek - Favoriten") == 0)
		{
			if(listbox->select != NULL && listbox->select->handle != NULL)
			{
				if(textbox(_("Message"), _("Remove this Favorite ?"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 5, 0) == 1)
				{
					removefav(((struct tithek*)listbox->select->handle)->title, ((struct tithek*)listbox->select->handle)->link, ((struct tithek*)listbox->select->handle)->pic, ((struct tithek*)listbox->select->handle)->localname, ((struct tithek*)listbox->select->handle)->menutitle, ((struct tithek*)listbox->select->handle)->flag);		
					pagecount = createtithekplay(titheklink, grid, listbox, countlabel, 0);
					if(pagecount == 0) return;
						
					drawscreen(grid, 0, 0);
				}
			}
		}
		else if(rcret == getrcconfigint("rcgreen", NULL) && ostrcmp(title, "TiThek - Favoriten") != 0)
		{
			if(listbox->select != NULL && listbox->select->handle != NULL)
			{
				if(textbox(_("Message"), _("Add this link as Favorite ?"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 5, 0) == 1)
				{
					addfav(((struct tithek*)listbox->select->handle)->title, ((struct tithek*)listbox->select->handle)->link, ((struct tithek*)listbox->select->handle)->pic, ((struct tithek*)listbox->select->handle)->localname, ((struct tithek*)listbox->select->handle)->menutitle, ((struct tithek*)listbox->select->handle)->flag);		
				}
			}
		}

		if (ostrcmp(title, "TiThek - Favoriten") == 0)
		{
			b4->hidden = NO;
			b5->hidden = YES;
		}
		else
		{
			b4->hidden = YES;
			b5->hidden = NO;
		}
	}

	delmarkedscreennodes(grid, 1);
	delownerrc(grid);
	clearscreen(grid);

	if(first == 1)
	{
		freetithek();
		delallfiles("/tmp/tithek", NULL);
		if(status.mcaktiv == 0)
			servicecheckret(servicestart(status.lastservice->channel, NULL, NULL, 0), 0);
	}
}

#endif
