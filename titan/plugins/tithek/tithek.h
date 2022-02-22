
#ifndef TITHEK_H
#define TITHEK_H

#define TITHEKPATH "/tmp/tithek"
int tithekdownloadrun = 0;
int tithekdownloadcount = 0;
int tithekrun = 0;
int tithekexit = 0;
//int tithekmovie4k = 0;
int tithekkinox = 0;
//int titheksolarmovie = 0;
//int tithekmlehd = 0;
//int amazonlogin = 0;
int python = 0;
int ytbgdownload = 0;
int hlsbgdownload = 0;

//flag 0	- menu
//flag 1	- menu pay hidden tithek_pay=0/1 0=hidden
//flag 2	- http (default streamurl)
//flag 3	- http radio
//flag 4	- youtube
//flag 5	- nowtv //rtl2now
//flag 6	- nowtv pay //superrtlnow
//flag 7	- nowtv local serach //rtlnow
//flag 8	- nowtv search //voxnow
//flag 9	- youtube suche 10
//flag 10	- youtube suche 25
//flag 11	- youtube suche 50
//flag 12	- myvideo
//flag 13	- myvideo search 50
//flag 14	- kinox play
//flag 15	- giga old: hoster filenuke
//flag 16	- superrtlnow pay
//flag 17	- rtlnow pay
//flag 18	- voxnow pay
//flag 19	- rtl2now pay
//flag 20	- netstream old: hoster StreamCloud
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
//flag 63   - filmon local serach
//flag 64   - tvtoast
//flag 65   - tvtoast local serach
//flag 66   - coming soon dummy
//flag 67   - rtlnitronow
//flag 68   - rtlnitronow pay
//flag 69   - rtlnitronow local search
//flag 70   - ntvnow
//flag 71   - ntvnow pay
//flag 72   - ntvnow local search
//flag 73   - watchmovies movie
//flag 74   - watchmovies series
//flag 75   - watchmovies local search
//flag 75   - amazon
//flag 76   - amazon movie search
//flag 77   - amazon series search
//flag 78   - amazon series listed search
//flag 79   - amazon watchlist movie
//flag 80   - amazon watchlist tv
//flag 81   - amazon popularity.rank movie
//flag 82   - amazon next page
//flag 83   - amazon popularity.rank tv
//flag 84   - amazon popularity.kids movie
//flag 85   - amazon popularity.kids tv
//flag 86   - amazon currently movie
//flag 87   - amazon genre movie
//flag 88   - amazon genre movie listed
//flag 89   - amazon genre tv
//flag 90   - amazon genre tv listed
//flag 91   - streamlive
//flag 92   - cricfree
//flag 93   - liveonlinetv
//flag 94   - movie2k 41 movie4k
//flag 95	- movie2k 34 movie4k hoster de
//flag 96	- movie2k 35 movie4k hoster other
//flag 97   - movie2k 26 movie4k local serach
//flag 98   - nbaondemand

//flag 100  - all local search
//flag 111  - local parser parser.sh hoster/play
//flag 112  - local parser parser.sh search
//flag 1000 - menu pincode
//flag 9999 - menu hidden codecpack

// redmenu
//check 1	- screenbgdownload(1)
//check 2  	- Search on KinoX
//check 8  	- Search on KinoX (local)
//check 3  	- Search on Solarmovies (movie)
//check 4  	- Search on Solarmovies (serie)
//check 9  	- Search on Solarmovies (local)
//check 5  	- Search on Youtube
//check 10  - Search on Youtube (local)
//check 6  	- Search on MyVideo
//check 11  - Search on MyVideo (local)
//check 7  	- Search on Movie4k
//check 12  - Search on Movie4k (local)
//check 13  - Search on InternetRadio (local)
//check 14  - Search on InternetTv (local)
//check 15 	- Search on ARD (local)
//check 16 	- Search on ZDF (local)"
//check 17 	- Search on TecTime (local)
//check 18 	- Search on Giga (local)
//check 19 	- Search on Beeg (local)
//check 20 	- Search on NowTv (local)
//check 21 	- Search on All (local)

//check 24 	- Search on Xvideos (local)
//check 25 	- Search on Mle-HD (local)
//check 26 	- Search on Netzkino (local)
//check 27 	- Search on FilmOn (local)

//removed
//check 20 	- Search on Rtl2-Now (local)
//check 21 	- Search on Rtl-Now (local)
//check 22 	- Search on SuperRTL-Now (local)
//check 23 	- Search on VOX-Now (local)
//check 28 	- Search on RtlNitro-Now (local)
//check 29 	- Search on NTV-Now (local)

struct tithek
{
	char* title;
	char* link;
	char* pic;
	char* localname;
	char* menutitle;
	char* description;
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
	node->description = NULL;
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
					case 5: node->description = tmpstr; break;
				}

				ret++;
			}
			else
				tmpstr++;
		}
	}

	if(ret != 5 && ret != 6)
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
	else if(node->flag == 6 && pay == 0)
		skip = 1;
	else if(node->flag == 1 && pay == 0)
		skip = 1;
	else if(node->flag == 35 && pay == 0)
		skip = 1;
	else if(node->flag == 96 && pay == 0)
		skip = 1;
/*
	else if(titheksolarmovie == 1 && node->flag == 9996 && sos == 0)
	{
		cmd = ostrcat(cmd, "www.thesolarmovie.me", 1, 0);

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
*/
/*
	else if(tithekmovie4k == 1 && node->flag == 9997 && sos == 0)
	{
		cmd = ostrcat(cmd, "www.movie4k.to", 1, 0);

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
*/
/*
	else if(tithekmlehd == 1 && node->flag == 9998 && sos == 0)
	{
		cmd = ostrcat(cmd, "www.mle-hd.se", 1, 0);

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
*/
	else if(tithekkinox == 1 && node->flag == 9999 && sos == 0)
	{
		cmd = ostrcat(cmd, "kinox.me", 1, 0);

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

struct tithek* createtithek(struct tithek* update, char* title, char* link, char* pic, char* localname, char* menutitle, char* description, int flag)
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
	description = stringreplacechar(description, '#', ' ');

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
	tmpstr = ostrcat(tmpstr, "#", 1, 0);
	tmpstr = ostrcat(tmpstr, description, 1, 0);

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

int readtithek(char* filename)
{
	debug(1000, "in");
	FILE *fd = NULL;
	char *fileline = NULL, *file = NULL;
	int linecount = 0, len = 0, pay = 0;
	struct tithek* last = NULL, *tmplast = NULL;

	fileline = malloc(MINMALLOC);
	if(fileline == NULL)
	{
		err("no memory");
		return 1;
	}

//	if(cmpfilenameext(filename, ".sh") == 0)
	if(ostrstr(filename, ".sh ") != NULL && ostrstr(filename, ".sh init") == NULL)
	{
		printf("[tithek] cmd: %s\n", filename);
		debug(10, "cmd: %s", filename);
		file = command(filename);
		file = string_newline(file);
		printf("[tithek] file: %s\n", file);
		debug(10, "file: %s", file);
	}
	else
		file = ostrcat(filename, NULL, 0, 0);

	fd = fopen(file, "r");
	if(fd == NULL)
	{
		perr("can't open %s", file);
		free(fileline);
		return 1;
	}

	pay = getconfigint("tithek_pay", NULL);

	char* tmpstr = NULL;

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
		free(tmpstr), tmpstr = NULL;
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

		if(ostrcmp(node->filename, "/media/hdd/.tithek/python.tar") == 0 || ostrcmp(node->filename, "/var/swap/.tithek/python.tar") == 0 || ostrcmp(node->filename, "/mnt/.tithek/python.tar") == 0)
		{
			printf("[tithek] download start in: 10s\n");
			sleep(10);
			printf("[tithek] download start: %s\n", node->filename);
		}

		if(node->cmd != NULL)
			system(node->cmd);
		else if(node->link != NULL)
			gethttps(node->link, node->filename, NULL, NULL, NULL, NULL, 0);
		else
			gethttpreal(node->host, node->page, node->port, node->filename, node->auth, NULL, 0, NULL, NULL, node->timeout, 0);
		if(ostrcmp(node->filename, "/media/hdd/.tithek/python.tar") == 0 || ostrcmp(node->filename, "/var/swap/.tithek/python.tar") == 0 || ostrcmp(node->filename, "/mnt/.tithek/python.tar") == 0)
			printf("[tithek] download done: %s\n", node->filename);

		char* cmd = NULL;



		if(ostrcmp(node->filename, "/media/hdd/.tithek/python.tar") == 0)
			cmd = ostrcat("rm -rf `find /media/hdd/.tithek/lib -type f -name '*.py' -size +1k`; tar -xf /media/hdd/.tithek/python.tar -C /media/hdd/.tithek/", NULL, 0, 0);
		else if(ostrcmp(node->filename, "/var/swap/.tithek/python.tar") == 0)
			cmd = ostrcat("rm -rf `find /var/swap/.tithek/lib -type f -name '*.py' -size +1k`; tar -xf /var/swap/.tithek/python.tar -C /var/swap/.tithek/", NULL, 0, 0);
		else if(ostrcmp(node->filename, "/mnt/.tithek/python.tar") == 0)
			cmd = ostrcat("rm -rf `find /mnt/.tithek/lib -type f -name '*.py' -size +1k`; tar -xf /mnt/.tithek/python.tar -C /mnt/.tithek/", NULL, 0, 0);
		else if(ostrcmp(node->filename, "/tmp/python.tar") == 0)
			cmd = ostrcat("tar -xf /tmp/python.tar -C /tmp/localhoster/", NULL, 0, 0);

		if(cmd != NULL)
		{
			printf("[tithek] untar start cmd: %s\n", cmd);
			system(cmd);
			printf("[tithek] untar ende cmd: %s\n", cmd);
			python = 1;
			free(cmd), cmd = NULL;

			if(ostrcmp(node->filename, "/tmp/python.tar") == 0 || getconfigint("tithek_autoupdate", NULL) == 1)
			{
				printf("[tithek] remove: %s\n", node->filename);
				unlink(node->filename);
			}
		}
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

		free(node->cmd); node->cmd = NULL;
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
	int ret = 1, port = 80, timeout = 10000, ssl = 0, usecmd = 0;
	char* ip = NULL, *pos = NULL, *path = NULL;
	char* tmpstr = NULL, *localfile = NULL;

/*
printf("link1: %s\n", link);
    if(!ostrncmp("curl ", link, 5))
        link = command(link);

printf("link2: %s\n", link);
*/

	if(link == NULL) return NULL;
	if(ostrncmp("http://", link, 7) && ostrncmp("https://", link, 8) && ostrncmp("/tmp/localhoster/hoster.sh", link, 26)) return NULL;

	link = stringreplacecharonce(link, ' ', '\0');

	if(!ostrncmp("/tmp/localhoster/hoster.sh", link, 26))
		usecmd = 1;
	else if(!ostrncmp("https://", link, 8))
		ssl = 1;
	else
	{
		ip = string_replace("http://", "", (char*)link, 0);

		// tithek security
		ip = string_replace_all("imageshack.us/md/up/grd/", "openaaf.dyndns.tv/", ip, 1);

//		ip = string_replace_all("kinox.to", "kinox.me", ip, 1);
		//ip = string_replace_all("movie4k.to", "movie4k.me", ip, 1);

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
	free(tmpstr); tmpstr = NULL;

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
				if(ssl == 1)
					gethttps(link, localfile, NULL, NULL, NULL, NULL, 0);
				else if(usecmd == 0 && tithekdownloadcount >= 24) //start max 24 threads
//				if(usecmd == 0 && ssl == 0 && tithekdownloadcount >= 24) //start max 24 threads
					gethttp(ip, path, port, localfile, pw, timeout, NULL, 0);
				else
				{
					//dnode is freed in thread
					struct download* dnode = calloc(1, sizeof(struct download));
					if(dnode != NULL)
					{
//        				if(ssl == 1)
//							dnode->link = ostrcat(link, NULL, 0, 0);
						if(usecmd == 1)
							dnode->cmd = ostrcat(link, NULL, 0, 0);
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
			{
				if(usecmd == 1)
				{
					debug(99, "cmd: %s", link);
					system(link);
				}
				else if(ssl == 1)
					gethttps(link, localfile, NULL, NULL, NULL, NULL, 0);
				else
					gethttp(ip, path, port, localfile, pw, timeout, NULL, 0);
			}
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


int createtithekmenu(char* titheklink, struct skin* menu, struct skin* listbox)
{
	struct skin* tmp = NULL;
	char* tithekfile = NULL;
	char* tithekpic = NULL;
	int localfile = 0;

//	tithekfile = tithekdownload(titheklink);
	if(!ostrncmp("http://", titheklink, 7))
		tithekfile = tithekdownload(titheklink, NULL, HTTPAUTH, 0, 0);
	else
	{
		tithekfile = ostrcat(titheklink, NULL, 0, 0);
		localfile = 1;
	}

	delmarkedscreennodes(menu, 1);

	freetithek();

//	if(readtithek(tithekfile) != 0) return 1;
	int linecount = readtithek(tithekfile);

	struct tithek* titheknode = tithek;

	while(titheknode != NULL)
	{
		tmp = addlistbox(menu, listbox, tmp, 1);
		if(tmp != NULL)
		{
			tmp->type = MENU;
			tmp->textposx = 80;
			tmp->valign = MIDDLE;
			changetext(tmp, titheknode->title);
			tmp->height = 50;
			tmp->hspace = 10;
			tithekpic = tithekdownload(titheknode->pic, NULL, HTTPAUTH, 0, 0);
			tmp->picwidth = 1;
			tmp->picheight = 1;

			changepic(tmp, tithekpic);
			free(tithekpic); tithekpic = NULL;
			tmp->handle = (char*)titheknode;
		}
		titheknode = titheknode->next;
	}

	if(localfile == 0)
		unlink(tithekfile);

	free(tithekfile); tithekfile = NULL;
	return 0;
}

void screentithekmenu(char* titheklink, char* title, int first)
{
	int rcret = -1, oaktline = 1, oaktpage = -1;

	struct skin* menu = getscreen("tithekmenu");
	struct skin* listbox = getscreennode(menu, "listbox");
	if(titheklink == NULL) return;

	listbox->aktpage = -1;
	listbox->aktline = 1;

	if(createtithekmenu(titheklink, menu, listbox) != 0) return;
	drawscreen(menu, 2, 0);
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
					screentithekplay(tmpstr, title, first);
				else
					screentithekmenu(tmpstr, title, first);

				free(tmpstr); tmpstr = NULL;
				if(createtithekmenu(titheklink, menu, listbox) != 0) break;

				listbox->aktpage = oaktpage;
				listbox->aktline = oaktline;
				drawscreen(menu, 0, 0);
				addscreenrc(menu, listbox);
			}
		}
	}

	freetithek();
	delmarkedscreennodes(menu, 1);
	delownerrc(menu);
	clearscreen(menu);
}


int createtithekplay(char* titheklink, struct skin* grid, struct skin* listbox, struct skin* countlabel, char* title, int flag)
{
	int gridbr = 0, posx = 0, count = 0, sumcount = 0, count1 = 0, pagecount = 0, skip = 0;
	int localfile = 0;
	struct skin* tmp = NULL;
	char* tithekfile = NULL;
	char* tmpstr = NULL, *tmpstr1 = NULL, *tmpstr2 = NULL;

	if(!ostrncmp("http://", titheklink, 7))
		tithekfile = tithekdownload(titheklink, NULL, HTTPAUTH, 0, 0);
	else
	{
		tithekfile = ostrcat(titheklink, NULL, 0, 0);
		localfile = 1;
	}

//	amazon_init(titheklink, tithekfile);
//init locale parser
	localparser_init(titheklink, tithekfile, flag);

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

	int fontsize2 = 20;
	int posy = 50;
	int textposx2 = 270;
	int textposx = 230;
	int usedescriptionview = 0;

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

	if((getconfigint("tithek_cover", NULL) == 1 && flag == 0) || (getconfigint("tithek_view", NULL) == 1 && flag != 0))
	{
		height = 500;
		width = 590;
		picheight = 480;
		picwidth = 570;
		zcount = 2;
		fontsize = 25;
		pcount = 2;
	}

	if((getconfigint("tithek_cover", NULL) == 2 && flag == 0) || (getconfigint("tithek_view", NULL) == 2 && flag != 0))
	{
		height = 280;
		width = 390;
		picheight = 230;
		picwidth = 370;
		zcount = 3;
		fontsize = 20;
		pcount = 6;
	}

	if((getconfigint("tithek_cover", NULL) == 3 && flag == 0) || (getconfigint("tithek_view", NULL) == 3 && flag != 0))
	{
		height = 180;
		width = 295;
		picheight = 130;
		picwidth = 270;
		zcount = 4;
		fontsize = 18;
		pcount = 12;
	}

	if((getconfigint("tithek_cover", NULL) == 4 && flag == 0) || (getconfigint("tithek_view", NULL) == 4 && flag != 0))
	{
		height = 140;
		width = 235;
		picheight = 90;
		picwidth = 210;
		zcount = 5;
		fontsize = 16;
		pcount = 20;
	}

	if((getconfigint("tithek_cover", NULL) == 5 && flag == 0) || (getconfigint("tithek_view", NULL) == 5 && flag != 0))
	{
		height = 111;
		width = 196;
		picheight = 60;
		picwidth = 150;
		zcount = 6;
		fontsize = 14;
		pcount = 30;
	}

	if((getconfigint("tithek_cover", NULL) == 6 && flag == 0) || (getconfigint("tithek_view", NULL) == 6 && flag != 0))
	{
		height = 50;
		width = 1160;
		picheight = 48;
		picwidth = 1100;
		zcount = 1;
		fontsize = 25;
		pcount = 1;
		height += status.fontsizeadjust;
	}

	if((getconfigint("tithek_cover", NULL) == 7 && flag == 0) || (getconfigint("tithek_view", NULL) == 7 && flag != 0))
	{
		height = 500;
		width = 1160;
		picheight = 480;
		picwidth = 570;
		zcount = 1;
		fontsize = 25;
		pcount = 1;
		height += status.fontsizeadjust;
		fontsize2 = 20;
		posy = 20;
		textposx = 370;
		textposx2 = textposx + 30;
	}

	fontsize += status.fontsizeadjust;
	fontsize2 += status.fontsizeadjust;
	height += status.fontsizeadjust;

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

//listbox dummy
			if(getconfigint("tithek_cover", NULL) >= 8 && getconfigint("tithek_view", NULL) >= 8)
			{
				gridbr = 0;
				posx = 0;

				if(getconfigint("tithek_cover", NULL) == 7 || getconfigint("tithek_view", NULL) == 7)
				{
					char* tithekpic = tithekdownload(titheknode->pic, titheknode->localname, NULL, 0, 0);
					changepic(tmp, tithekpic);
					free(tithekpic); tithekpic = NULL;
				}
				
				tmp->valign = convertxmlentry("middle", 0);
				tmp->textposx = textposx;
				tmp->wrap = YES;
#ifndef SIMULATE
				tmp->picheight = 1;
				tmp->picwidth = 1;
#endif
				tmp->bgspace = 1;
				tmp->vspace = 10;
				tmp->hspace = 10;

// das lasst den text2 eine zeile weiter springen
				tmp->type = TEXTBOX;
// schiebt text nach oben und text2 eine zeile darunter
//				tmp->valign = TOPBOTTOM;
				tmp->posy = posy;
				tmp->textposx2 = textposx2;
				tmp->height = height;
				tmp->fontcol2 = convertcol("yellow");
				tmp->fontsize2 = fontsize2;
			}

			if(titheknode->description != NULL && (getconfigint("tithek_cover", NULL) == 0 || getconfigint("tithek_view", NULL) == 0))
			{
//				usedescriptionview = 1;
			}

			if(usedescriptionview == 1 || (getconfigint("tithek_cover", NULL) == 7 && getconfigint("tithek_view", NULL) == 7))
			{
				skip = 0;
				sumcount++;
				count++;
				count1++;
				if(gridbr == 0)
					tmp->type = GRIDBR;
				gridbr = 1;
				tmp->wrap = YES;
#ifndef SIMULATE
				tmp->picheight = picheight;
				tmp->picwidth = picwidth;
#endif
				tmp->fontsize = fontsize;
				tmp->height = height;
				tmp->width = width;
				//tmp->bgcol = 0xffffff;
				tmp->bgspace = 1;
				tmp->vspace = 10;
				tmp->hspace = 10;
				tmp->posx = posx;
				tmp->posy = posy;
				tmp->prozwidth = 0;
				tmp->valign = convertxmlentry("middle", 0);
				tmp->textposx = textposx;
				tmp->textposx2 = textposx2;
				tmp->fontcol2 = convertcol("yellow");
				tmp->fontsize2 = fontsize2;
				posx = 0;
				if(titheknode->description == NULL) 
					titheknode->description = ostrcat(_("No Description found..."), NULL, 0, 0);
//                else if(!ostrncmp("curl ", titheknode->description, 5))
 //               {
 //                   addconfigtmp("tithek_cmd", ostrcat(titheknode->description, NULL, 0, 0));
//					titheknode->description = ostrcat(_("Description is loading..."), NULL, 0, 0);
//                }
			}
			else
			{
				skip = 0;
				sumcount++;
				count++;
				count1++;
				if(gridbr == 0)
					tmp->type = GRIDBR;
				gridbr = 1;
				tmp->wrap = YES;
#ifndef SIMULATE
				tmp->picheight = picheight;
				tmp->picwidth = picwidth;
#endif
				tmp->fontsize = fontsize;
				tmp->height = height;
				tmp->width = width;
				tmp->prozwidth = 0;
				//tmp->bgcol = 0xffffff;
				tmp->bgspace = 1;
				tmp->vspace = 10;
				tmp->hspace = 10;
				tmp->posx = posx;

				tmp->prozwidth = 0;
				tmp->halign = CENTER;
				tmp->valign = TEXTBOTTOM;
			}


			debug(99, "--- START ----------------------------------------------------");
			if(title != NULL)
				tmpstr1 = ostrcat(title, " - ", 0, 0);
			else
				tmpstr1 = ostrcat(titheknode->menutitle, " - ", 0, 0);

			tmpstr1 = ostrcat(tmpstr1, titheknode->title, 1, 0);

//			tmpstr2 = createfilename(tmpstr1, titheknode->menutitle);
			tmpstr2 = createfilename(tmpstr1, NULL, 0);
			int new = 0;
			if(file_exist(tmpstr2))
			{
				new = 1;
				debug(99, "found oldfile %s", tmpstr2);
			}
			free(tmpstr2), tmpstr2 = NULL;

			tmpstr2 = createfilename(tmpstr1, NULL, 1);
			int fav = 0;
			if(file_exist(tmpstr2))
			{
				fav = 1;
				debug(99, "found oldfav %s", tmpstr2);
			}
			free(tmpstr2), tmpstr2 = NULL;

			free(tmpstr1), tmpstr1 = NULL;
			
			debug(99, "--- END -----------------------------------------------------");

			if(fav == 1)
//				tmp->fontcol = 0x0091c9; //get favcol=0091c9
				tmp->fontcol = convertcol("favcol");
			else if(new == 1)
				tmp->fontcol = convertcol("green");
			else if(python == 0 && (titheknode->flag == 14 || titheknode->flag == 21 || titheknode->flag == 111 || titheknode->flag == 112))
				tmp->fontcol = convertcol("red");
			else if(python == 1 && (titheknode->flag == 14 || titheknode->flag == 21 || titheknode->flag == 111 || titheknode->flag == 112))
				tmp->fontcol = convertcol("white");
			else
				tmp->fontcol = convertcol("white");

			if(getconfigint("tithek_cover", NULL) != 7 || getconfigint("tithek_view", NULL) != 7)
			{
				tmp->halign = CENTER;
				tmp->valign = TEXTBOTTOM;
			}
			changetext(tmp, titheknode->title);
			changename(tmp, titheknode->title);
            if(ostrncmp("curl ", titheknode->description, 5) && ostrncmp("/tmp/localhoster/hoster.sh get", titheknode->description, 30))
          	    changetext2(tmp, titheknode->description);

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

	tmpstr1 = ostrcat(_("found"), NULL, 0, 0);
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

//void removefav(char* title, char* link, char* pic, char* localname, char* menutitle, char* fullmenutitle, int flag)
void removefav(char* title, char* link, char* pic, char* localname, char* menutitle, char* description, int flag)
{
debug(99, "removefav title: %s", title);
debug(99, "removefav menutitle: %s", menutitle);

	int count = 0, i = 0;
	char* tmpstr = NULL, *tmpstr1 = NULL, *tmpstr2 = NULL, *input = NULL;
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
	input = ostrcat(input, "#", 1, 0);
	input = ostrcat(input, description, 1, 0);

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
			{
				printf("remove: %s\n", ret[i].part);

//				tmpstr2 = createfilename(fullmenutitle, title, 1);
				tmpstr2 = createfilename(menutitle, title, 1);

				debug(99, "tmpstr2: %s", tmpstr2);

				if(file_exist(tmpstr2))
				{
					unlink(tmpstr2);
					debug(99, "remove favorite color entry %s", tmpstr2);
				}
				free(tmpstr2), tmpstr2 = NULL;
			}
		}
	}

	if(tmpstr1 != NULL && strlen(tmpstr1) > 0)
		tmpstr1[strlen(tmpstr1) - 1] = '\0';

	if(tmpstr1 == NULL)
		writesys(getconfig("tithek_fav", NULL), "", 0);
	else
		writesys(getconfig("tithek_fav", NULL), tmpstr1, 0);

	if(!ostrncmp("/", link, 1))
	{
		free(tmpstr1); tmpstr1 = NULL;
		tmpstr1 = ostrcat("rm -f ", link, 0, 0);
		system(tmpstr1);
	}

	free(ret); ret = NULL;
	free(tmpstr); tmpstr = NULL;
	free(tmpstr1); tmpstr1 = NULL;
	free(input); input = NULL;
}

//void addfav(char* title, char* link, char* pic, char* localname, char* menutitle, char* fullmenutitle, int flag)
void addfav(char* title, char* link, char* pic, char* localname, char* menutitle, char* description, int flag)
{
	int count = 0, i = 0;
	char* tmpstr = NULL, *tmpstr1 = NULL, *input = NULL;
	struct splitstr* ret = NULL;

	char* savefile = NULL;

	struct menulist* mlist = NULL, *mbox = NULL;
	char* favlist = command("ls -1 /mnt/config/favorite*");
	printf("favlist: %s\n", favlist);
	ret = strsplit(favlist, "\n", &count);

	if(ret != NULL)
	{
		for(i = 0; i < count; i++)
		{
			char* cmd = ostrcat("cat ", getconfig("tithek_fav", NULL), 0, 0);
			cmd = ostrcat(cmd, " | grep '#", 1, 0);
			cmd = ostrcat(cmd, (ret[i]).part, 1, 0);

			cmd = ostrcat(cmd, "#' | cut -d'#' -f1", 1, 0);
			printf("cmd: %s\n", cmd);
			tmpstr = command(cmd);
			if(tmpstr == NULL)
				tmpstr = ostrcat(_("Favorites"), NULL, 0, 0);
			addmenulist(&mlist, tmpstr, (ret[i]).part, NULL, 0, 0);
			printf("(ret[i]).part: %s\n", (ret[i]).part);
			printf("tmpstr: %s\n", tmpstr);
			free(tmpstr), tmpstr = NULL;
			free(cmd), cmd = NULL;
		}

	}
	else
		addmenulist(&mlist, "Favorites", getconfig("tithek_fav", NULL), NULL, 0, 0);

	free(ret), ret = NULL;

	mbox = menulistbox(mlist, NULL, NULL, NULL, NULL, NULL, 1, 0);
	if(mbox != NULL)
	{
		printf("mbox->name %s\n", mbox->name);
		printf("mbox->text %s\n", mbox->text);
		savefile = ostrcat(mbox->text, NULL, 0, 0);
	}
	else
		savefile = ostrcat(getconfig("tithek_fav", NULL), NULL, 0, 0);

//	char* title2 = ostrcat(((struct tithek*)listbox->select->handle)->title, NULL, 0, 0);
//	char* filename = createfilename(title, title2);
//		char* filename = createfilename(title, ((struct tithek*)listbox->select->handle)->title);


//	debug(99, "fullmenutitle: %s", fullmenutitle);
	debug(99, "menutitle: %s", menutitle);
	debug(99, "title: %s", title);


	tmpstr = createfilename(menutitle, title, 1);
//	tmpstr = createfilename(fullmenutitle, title, 1);

	debug(99, "tmpstr: %s", tmpstr);

	if(!file_exist(tmpstr))
	{
		writesys(tmpstr, "1", 0);
		debug(99, "add favorite color entry %s", tmpstr);
	}
	free(tmpstr), tmpstr = NULL;

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
	input = ostrcat(input, "#", 1, 0);
	input = ostrcat(input, description, 1, 0);

	// tithek security
	input = string_replace_all("http://openaaf.dyndns.tv/", "http://imageshack.us/md/up/grd/", input, 1);

	tmpstr1 = ostrcat(tmpstr1, input, 1, 0);
	tmpstr1 = ostrcat(tmpstr1, "\n", 1, 0);

	tmpstr = readfiletomem(savefile, 0);

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

	writesys(savefile, tmpstr1, 0);

	if(!ostrncmp("/", link, 1))
	{
		free(tmpstr1); tmpstr1 = NULL;
		tmpstr1 = ostrcat("touch ", link, 0, 0);
		system(tmpstr1);
	}

	free(mbox); mbox = NULL;
	free(ret); ret = NULL;
	free(tmpstr); tmpstr = NULL;
	free(tmpstr1); tmpstr1 = NULL;
	free(input); input = NULL;
}

//void editfav(char* title, char* link, char* pic, char* localname, char* menutitle, char* fullmenutitle, int flag)
void editfav(char* title, char* link, char* pic, char* localname, char* menutitle, char* description, int flag)
{
	int rcret = 0, type = 0;
	struct skin* tithekedit = getscreen("tithekedit");
	struct skin* listbox = getscreennode(tithekedit, "listbox");
	struct skin* skin_title = getscreennode(tithekedit, "title");
	struct skin* skin_link = getscreennode(tithekedit, "link");
	struct skin* skin_pic = getscreennode(tithekedit, "pic");
	struct skin* skin_localname = getscreennode(tithekedit, "localname");
	struct skin* skin_menutitle = getscreennode(tithekedit, "menutitle");
	struct skin* skin_type = getscreennode(tithekedit, "type");
	struct skin* skin_description = getscreennode(tithekedit, "description");

	struct skin* tmp = NULL;
	struct skin* load = getscreen("loading");

	changeinput(skin_title, title);
	changeinput(skin_link, link);
	changeinput(skin_pic, pic);
	changeinput(skin_localname, localname);
	changeinput(skin_menutitle, menutitle);
	changeinput(skin_type, oitoa(type));
	changeinput(skin_description, description);

	addscreenrc(tithekedit, listbox);
	drawscreen(tithekedit, 0, 0);

	tmp = listbox->select;
	while(1)
	{
		addscreenrc(tithekedit, tmp);
		rcret = waitrcext(tithekedit, 0, 0, 1000);
		delownerrc(tithekedit);
		addscreenrc(tithekedit, listbox);
		tmp = listbox->select;

		if(rcret == getrcconfigint("rcexit", NULL)) break;
		if(rcret == getrcconfigint("rcok", NULL))
		{
			drawscreen(load, 0, 0);
//			addfav(skin_title->ret, skin_link->ret, skin_pic->ret, skin_localname->ret, skin_menutitle->ret, fullmenutitle, atoi(skin_type->ret));
			addfav(skin_title->ret, skin_link->ret, skin_pic->ret, skin_localname->ret, skin_menutitle->ret, skin_description->ret, atoi(skin_type->ret));
			clearscreen(load);
			break;
		}
	}

	delownerrc(tithekedit);
	clearscreen(tithekedit);
}

int showinfo(struct skin* listbox, char* title, char* link, char* pic, char* localname, char* menutitle, char* description, int flag)
{
	int ret = 1;
	char* tmpstr = NULL;
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
	tmpstr = ostrcat(tmpstr, "#", 1, 0);
	tmpstr = ostrcat(tmpstr, description, 1, 0);

	// tithek security
//	tmpstr = string_replace_all("http://openaaf.dyndns.tv/", "http://imageshack.us/md/up/grd/", tmpstr, 1);
	tmpstr = ostrcat(tmpstr, "\n", 1, 0);
	
	char* savefile = ostrcat("/tmp/tithek/infomenu.list", NULL, 0, 0);

	writesys(savefile, tmpstr, 0);
	free(tmpstr), tmpstr = NULL;

	if(file_exist(savefile))
	{
		struct tithek* tnode = (struct tithek*)listbox->select->handle;
		createtithek(tnode, tnode->title, savefile, tnode->pic, tnode->localname, tnode->menutitle, tnode->description, tnode->flag);
		ret = 0;
	}

	return ret;
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


void backgroundytdl(char* link, char* filename)
{
	int ret = 0;
	char *file = NULL, *cmd = NULL;

	file = ostrcat(getconfig("rec_streampath", NULL), "/", 0, 0);
	file = ostrcat(file, filename, 1, 0);

	cmd = ostrcat("/tmp/localhoster/hoster.sh youtube_dlbg \"", link, 0, 0);
	cmd = ostrcat(cmd, "\" \"", 1, 0);
	cmd = ostrcat(cmd, file, 1, 0);
	cmd = ostrcat(cmd, "\" &", 1, 0);

	printf("cmd: %s\n", cmd);
	ret = system(cmd);
	free(cmd), cmd = NULL;
	if(ret == 1)
		textbox(_("Message"), _("Can't start download.\nPlease try later."), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 0, 0);
	else
		ytbgdownload = 1;
}

void backgroundhlsdl(char* link, char* filename)
{
	int ret = 0;
	char *file = NULL, *cmd = NULL;

	file = ostrcat(getconfig("rec_streampath", NULL), "/", 0, 0);
	file = ostrcat(file, filename, 1, 0);

	cmd = ostrcat("/tmp/localhoster/hoster.sh hlsdl \"", link, 0, 0);
	cmd = ostrcat(cmd, "\" \"", 1, 0);
	cmd = ostrcat(cmd, file, 1, 0);
	cmd = ostrcat(cmd, "\" &", 1, 0);

	printf("cmd: %s\n", cmd);
	ret = system(cmd);
	free(cmd), cmd = NULL;
	if(ret == 1)
		textbox(_("Message"), _("Can't start download.\nPlease try later."), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 0, 0);
	else
		hlsbgdownload = 1;
}

void backgroundcurldl(char* link, char* filename)
{
	int ret = 0;
	char *file = NULL, *cmd = NULL;

	file = ostrcat(getconfig("rec_streampath", NULL), "/", 0, 0);
	file = ostrcat(file, filename, 1, 0);

	cmd = ostrcat("/tmp/localhoster/hoster.sh curldl \"", link, 0, 0);
	cmd = ostrcat(cmd, "\" \"", 1, 0);
	cmd = ostrcat(cmd, file, 1, 0);
	cmd = ostrcat(cmd, "\" &", 1, 0);

	printf("cmd: %s\n", cmd);
	ret = system(cmd);
	free(cmd), cmd = NULL;
	if(ret == 1)
		textbox(_("Message"), _("Can't start download.\nPlease try later."), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 0, 0);
//	else
//		curlbgdownload = 1;
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

	if(ostrstr(path, "|User-Agent=") != NULL)
	{
		stringreplacechar(path, '|', '\0');
		printf("page changed: %s\n", path);

		char* cmd = NULL;
		cmd = ostrcat("wget --user-agent=\'Mozilla/5.0 (X11; Ubuntu; Linux i686; rv:30.0) Gecko/20100101 Firefox/30.0\' \'http://", host, 0, 0);
		cmd = ostrcat(cmd, "/", 1, 0);
		cmd = ostrcat(cmd, path, 1, 0);
		cmd = ostrcat(cmd, "\' -O \'", 1, 0);
		cmd = ostrcat(cmd, file, 1, 0);
		cmd = ostrcat(cmd, "\' &", 1, 0);
		printf("cmd: %s\n", cmd);
		ret = system(cmd);
		free(cmd), cmd = NULL;
	}
	else
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
//	int debuglevel = getconfigint("debuglevel", NULL);
	char* tmpstr = NULL, *tmpstr1 = NULL, *tmpstr2 = NULL;
	drawscreen(load, 0, 0);
	tmpstr = ostrcat(((struct tithek*)listbox->select->handle)->link, NULL, 0, 0);
	tmpstr2 = ostrcat(((struct tithek*)listbox->select->handle)->title, NULL, 0, 0);

	if(((struct tithek*)listbox->select->handle)->flag == 2)
	{
		if(tmpstr != NULL) tmpstr1 = ostrcat(tmpstr, NULL, 0, 0);
	}
	else if(((struct tithek*)listbox->select->handle)->flag == 3)
	{
		flag = 4;
		if(tmpstr != NULL) tmpstr1 = ostrcat(tmpstr, NULL, 0, 0);
	}
/*
	else if(((struct tithek*)listbox->select->handle)->flag == 4)
	{
		if(tmpstr != NULL) tmpstr1 = youtube_hoster(tmpstr);
	}
*/
/*
	else if(((struct tithek*)listbox->select->handle)->flag == 5)
	{
		if(tmpstr != NULL) tmpstr1 = nowtv(tmpstr);
	}
	else if(((struct tithek*)listbox->select->handle)->flag == 6)
	{
		if(tmpstr != NULL) tmpstr1 = nowtv(tmpstr);
	}
*/
/*
	else if(((struct tithek*)listbox->select->handle)->flag == 12)
	{
		if(tmpstr != NULL) tmpstr1 = myvideo_hoster(tmpstr);
	}
*/
	else if(((struct tithek*)listbox->select->handle)->flag == 14)
	{
		if(tmpstr != NULL) tmpstr1 = kinox(tmpstr);
	}
/*
	else if(((struct tithek*)listbox->select->handle)->flag == 15)
	{
		if(tmpstr != NULL) tmpstr1 = giga(tmpstr);
	}
*/
/*
	else if(((struct tithek*)listbox->select->handle)->flag == 20)
	{
		if(tmpstr != NULL) tmpstr1 = nbastream(tmpstr);
	}
*/
/*
	else if(((struct tithek*)listbox->select->handle)->flag == 38)
	{
		if(tmpstr != NULL) tmpstr1 = mlehd(tmpstr);
	}
*/
/*
	else if(((struct tithek*)listbox->select->handle)->flag == 41)
	{
		if(tmpstr != NULL) tmpstr1 = movie4k(tmpstr);
	}
*/
	else if(((struct tithek*)listbox->select->handle)->flag == 42)
	{
		if(tmpstr != NULL) tmpstr1 = xvideos(tmpstr);
	}
/*
	else if(((struct tithek*)listbox->select->handle)->flag == 43)
	{
		if(tmpstr != NULL) tmpstr1 = solarmovie(tmpstr);
	}
*/
	else if(((struct tithek*)listbox->select->handle)->flag == 45)
	{
		if(tmpstr != NULL) tmpstr1 = ard(tmpstr);
	}
/*
	else if(((struct tithek*)listbox->select->handle)->flag == 46)
	{
		if(tmpstr != NULL) tmpstr1 = zdf(tmpstr);
	}
*/
/*
	else if(((struct tithek*)listbox->select->handle)->flag == 50)
	{
		if(tmpstr != NULL) tmpstr1 = beeg(tmpstr);
	}
*/
/*
	else if(((struct tithek*)listbox->select->handle)->flag == 64)
	{
		if(tmpstr != NULL) tmpstr1 = tvtoast(tmpstr);
	}
*/
/*
	else if(((struct tithek*)listbox->select->handle)->flag == 75)
	{
		if(tmpstr != NULL) tmpstr1 = amazon(tmpstr);
	}
*/
	else if(((struct tithek*)listbox->select->handle)->flag == 91)
	{
		if(tmpstr != NULL) tmpstr1 = streamlive(tmpstr, -1);
	}
	else if(((struct tithek*)listbox->select->handle)->flag == 92)
	{
		if(tmpstr != NULL) tmpstr1 = cricfree(tmpstr, -1);
	}
	else if(((struct tithek*)listbox->select->handle)->flag == 93)
	{
		if(tmpstr != NULL) tmpstr1 = liveonlinetv(tmpstr, -1);
	}
/*
	else if(((struct tithek*)listbox->select->handle)->flag == 94)
	{
		if(tmpstr != NULL) tmpstr1 = movie2k(tmpstr);
	}
*/
/*
	else if(((struct tithek*)listbox->select->handle)->flag == 98)
	{
		if(tmpstr != NULL) tmpstr1 = nbaondemand(tmpstr);
	}
*/
	else if(((struct tithek*)listbox->select->handle)->flag == 111)
	{
		if(tmpstr != NULL) tmpstr1 = localparser_hoster(tmpstr);
	}

	free(tmpstr); tmpstr = NULL;
	free(tmpstr2); tmpstr2 = NULL;

	debug(10, "Streamurl check: %s", tmpstr1);
	debug(99, "Streamurl check: %s", tmpstr1);
	tmpstr = ostrcat(tmpstr1, NULL, 0, 0);
	tmpstr1 = list_hoster_streams(tmpstr);
	if(ostrcmp(tmpstr, tmpstr1) != 0)
	{
		debug(10, "Streamurl changed to: %s", tmpstr1);
		debug(99, "Streamurl changed to: %s", tmpstr1);
	}
	else
	{
		debug(10, "Streamurl check ok: %s", tmpstr1);
		debug(99, "Streamurl check ok: %s", tmpstr1);
	}

	free(tmpstr), tmpstr = NULL;

	if(ostrstr(title, "Internet Radio") != NULL)
		flag = 4;

	if(ostrstr(tmpstr1, "&amp;") != NULL)
	{
		debug(99, "change streamurl from: %s", tmpstr1);
		tmpstr1 = string_replace_all("&amp;", "&", tmpstr1, 1);
		debug(99, "change streamurl to: %s", tmpstr1);
	}

	if(!ostrncmp("errormsg=", tmpstr1, 9))
	{
		tmpstr = ostrcat(_("Found error Msg:"), "\n\n", 0, 0);
		tmpstr = ostrcat(tmpstr, tmpstr1, 1, 0);
		tmpstr = string_replace("errormsg=", "", tmpstr, 1);

		debug(99, "Found error Msg: %s", tmpstr1);
		textbox(_("Message"), tmpstr, _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 1100, 300, 0, 2);
		free(tmpstr); tmpstr = NULL;
		free(tmpstr1); tmpstr1 = NULL;
	}
	else if(!ostrncmp("skip", tmpstr1, 4))
	{
		debug(99, "skip playback screen, cancel by user");
		free(tmpstr); tmpstr = NULL;
		free(tmpstr1); tmpstr1 = NULL;
	}
	else if(ostrncmp("http", tmpstr1, 4) && ostrncmp("rtmp", tmpstr1, 4) && ostrncmp("mms", tmpstr1, 3) && ostrncmp("rtsp", tmpstr1, 4))
	{
		tmpstr = ostrcat(_("Parsing Error cant find http*|rtmp*|mms*|rtsp* Stream Link"), "\n\n", 0, 0);
		tmpstr = ostrcat(tmpstr, tmpstr1, 1, 0);
		debug(99, "Parsing Error cant find http*|rtmp*|mms*|rtsp* Stream Link: %s", tmpstr1);
		textbox(_("Message"), tmpstr, _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 1100, 600, 0, 2);
		free(tmpstr); tmpstr = NULL;
		free(tmpstr1); tmpstr1 = NULL;
	}
	else if(tmpstr1 != NULL)
	{
//		char* title2 = ostrcat(((struct tithek*)listbox->select->handle)->title, NULL, 0, 0);
//		char* filename = createfilename(title, title2, 0);
//		char* filename = createfilename(title, ((struct tithek*)listbox->select->handle)->title);

		debug(99, "::: START :::::::::::::::::::::::::::::::::::::::::::::::::::");
		debug(99, "flag: %d", ((struct tithek*)listbox->select->handle)->flag);
		debug(99, "title: %s", ((struct tithek*)listbox->select->handle)->title);
		debug(99, "menutitle: %s", ((struct tithek*)listbox->select->handle)->menutitle);

		char* filename = NULL;
		filename = createfilename(title, ((struct tithek*)listbox->select->handle)->title, 0);

		if(!file_exist(filename))
		{
			writesys(filename, "1", 0);
			debug(99, "create newfile %s", filename);
		}
		free(filename), filename = NULL;

//		filename = createfilename(((struct tithek*)listbox->select->handle)->menutitle, ((struct tithek*)listbox->select->handle)->title, ((struct tithek*)listbox->select->handle)->flag);
//		free(filename), filename = NULL;

//		filename = createfilename(title, NULL, ((struct tithek*)listbox->select->handle)->flag);
//		free(filename), filename = NULL;

		filename = createfilename(title, ((struct tithek*)listbox->select->handle)->title, ((struct tithek*)listbox->select->handle)->flag);
		debug(99, "::: END ::::::::::::::::::::::::::::::::::::::::::::::::::::");

		char* keyconf = NULL;
		char* skintitle = _("Choice Playback");
		struct menulist* mlist = NULL, *mbox = NULL;

#ifdef MIPSEL
//		if(checkbox("VUSOLO2") == 0)
			addmenulist(&mlist, "Streaming Playback (default)", _("Streaming Playback (default)"), NULL, 0, 0);
#endif

#ifndef MIPSEL
		addmenulist(&mlist, "Streaming Playback (default)", _("Streaming Playback (default)"), NULL, 0, 0);
#else
		// mipsel work, disable http direct streams without buffer, after 3mins no memory (memleak in player.h ?)
		if(ostrncmp("http://", tmpstr1, 7) && ostrncmp("https://", tmpstr1, 8))
			addmenulist(&mlist, "Streaming Playback (default)", _("Streaming Playback (default)"), NULL, 0, 0);
#endif
		if(!ostrncmp("http://", tmpstr1, 7) || !ostrncmp("https://", tmpstr1, 8))
		{
			// wakeup hdd for downloading
			wakeup_record_device();
			if(flag == 4)
			{
//#ifdef EPLAYER3
//				addmenulist(&mlist, "Streaming Playback Caching (1MB)", _("Streaming Playback Caching (1MB)"), NULL, 0, 0);
//#else
#ifdef MIPSEL
				// mipsel work for radio
				addmenulist(&mlist, "Streaming Playback (default)", _("Streaming Playback (default)"), NULL, 0, 0);
#endif
//#endif
			}
			else if(!ostrncmp("http://", tmpstr1, 7) || !ostrncmp("https://", tmpstr1, 8))
			{
//#ifdef EPLAYER3
				if(checkbox("UFS910") == 1 && !file_exist("/var/swapdir/swapfile"))
					addmenulist(&mlist, "Streaming Playback Caching (5MB)", _("Streaming Playback Caching (5MB)"), NULL, 0, 0);
				else
				{
					addmenulist(&mlist, "Streaming Playback Caching (10MB)", _("Streaming Playback Caching (10MB)"), NULL, 0, 0);
				}
//#endif
//				if(!ostrncmp("http", tmpstr1, 4) && (file_exist(getconfig("rec_streampath", NULL)) && (file_exist("/mnt/swapextensions/etc/.codecpack") || file_exist("/var/swap/etc/.codecpack") || file_exist("/var/etc/.codecpack"))))
				if(!ostrncmp("http://", tmpstr1, 7) && (file_exist(getconfig("rec_streampath", NULL)) && (file_exist("/mnt/swapextensions/etc/.codecpack") || file_exist("/var/swap/etc/.codecpack") || file_exist("/var/etc/.codecpack"))))
				{
/*
#ifndef EPLAYER3
					addmenulist(&mlist, "File Caching Playback (10MB / 120s)", _("File Caching Playback (10MB / 120s)"), NULL, 0, 0);
					addmenulist(&mlist, "File Caching Playback (20MB / 240s)", _("File Caching Playback (20MB / 240s)"), NULL, 0, 0);
					addmenulist(&mlist, "File Caching Playback (30MB / 360s)", _("File Caching Playback (30MB / 360s)"), NULL, 0, 0);
#endif
*/
					addmenulist(&mlist, "Download Full File", _("Download Full File"), NULL, 0, 0);
					addmenulist(&mlist, "Download Full File (background)", _("Download Full File (background)"), NULL, 0, 0);
				}

				if(ostrstr(tmpstr1, "hls") != NULL && file_exist(getconfig("rec_streampath", NULL)) && (file_exist("/mnt/swapextensions/etc/.codecpack") || file_exist("/var/swap/etc/.codecpack") || file_exist("/var/etc/.codecpack")))
					addmenulist(&mlist, "Download Full File (hlsdl)", _("Download Full File (hlsdl)"), NULL, 0, 0);
				if(file_exist(getconfig("rec_streampath", NULL)) && (file_exist("/mnt/swapextensions/etc/.codecpack") || file_exist("/var/swap/etc/.codecpack") || file_exist("/var/etc/.codecpack")))
					addmenulist(&mlist, "Download Full File (curldl)", _("Download Full File (curldl)"), NULL, 0, 0);

				if(python == 1 && file_exist(getconfig("rec_streampath", NULL)) && (file_exist("/mnt/swapextensions/etc/.codecpack") || file_exist("/var/swap/etc/.codecpack") || file_exist("/var/etc/.codecpack")))
					addmenulist(&mlist, "Download Full File (youtube_dl)", _("Download Full File (youtube_dl)"), NULL, 0, 0);

			}
		}
		mbox = menulistbox(mlist, NULL, skintitle, _("Choose your Streaming Playback Modus from the following list"), NULL, NULL, 1, 0);
		if(mbox != NULL) keyconf = mbox->name;
		debug(99, "tmpstr1: %s filename: %s flag: %d", tmpstr1, filename, flag);
		if(ostrcmp(keyconf, "Streaming Playback (default)") == 0)
		{
			addconfigtmp("playerbuffersize", "0");
			screenplay(tmpstr1, filename, 2, flag);
			delconfigtmp("playerbuffersize");
			if(status.mcaktiv == 0 && getconfigint("tithek_delservice", NULL) == 0)
				servicecheckret(servicestart(status.lastservice->channel, NULL, NULL, 0), 0);
		}
		else if(ostrcmp(keyconf, "Streaming Playback Caching (0.5MB)") == 0)
		{
			addconfigtmp("playerbuffersize", "524288");
			screenplay(tmpstr1, filename, 2, flag);
			delconfigtmp("playerbuffersize");
			if(status.mcaktiv == 0 && getconfigint("tithek_delservice", NULL) == 0)
				servicecheckret(servicestart(status.lastservice->channel, NULL, NULL, 0), 0);
		}
		else if(ostrcmp(keyconf, "Streaming Playback Caching (1MB)") == 0)
		{
			addconfigtmp("playerbuffersize", "1048576");
			screenplay(tmpstr1, filename, 2, flag);
			delconfigtmp("playerbuffersize");
			if(status.mcaktiv == 0 && getconfigint("tithek_delservice", NULL) == 0)
				servicecheckret(servicestart(status.lastservice->channel, NULL, NULL, 0), 0);
		}
		else if(ostrcmp(keyconf, "Streaming Playback Caching (2MB)") == 0)
		{
			addconfigtmp("playerbuffersize", "2097152");
			screenplay(tmpstr1, filename, 2, flag);
			delconfigtmp("playerbuffersize");
			if(status.mcaktiv == 0 && getconfigint("tithek_delservice", NULL) == 0)
				servicecheckret(servicestart(status.lastservice->channel, NULL, NULL, 0), 0);
		}
		else if(ostrcmp(keyconf, "Streaming Playback Caching (3MB)") == 0)
		{
			addconfigtmp("playerbuffersize", "3145728");
			screenplay(tmpstr1, filename, 2, flag);
			delconfigtmp("playerbuffersize");
			if(status.mcaktiv == 0 && getconfigint("tithek_delservice", NULL) == 0)
				servicecheckret(servicestart(status.lastservice->channel, NULL, NULL, 0), 0);
		}
		else if(ostrcmp(keyconf, "Streaming Playback Caching (4MB)") == 0)
		{
			addconfigtmp("playerbuffersize", "4194304");
			screenplay(tmpstr1, filename, 2, flag);
			delconfigtmp("playerbuffersize");
			if(status.mcaktiv == 0 && getconfigint("tithek_delservice", NULL) == 0)
				servicecheckret(servicestart(status.lastservice->channel, NULL, NULL, 0), 0);
		}
		else if(ostrcmp(keyconf, "Streaming Playback Caching (5MB)") == 0)
		{
			addconfigtmp("playerbuffersize", "5242880");
			screenplay(tmpstr1, filename, 2, flag);
			delconfigtmp("playerbuffersize");
			if(status.mcaktiv == 0 && getconfigint("tithek_delservice", NULL) == 0)
				servicecheckret(servicestart(status.lastservice->channel, NULL, NULL, 0), 0);
		}
		else if(ostrcmp(keyconf, "Streaming Playback Caching (7.5MB)") == 0)
		{
			addconfigtmp("playerbuffersize", "7864320");
			screenplay(tmpstr1, filename, 2, flag);
			delconfigtmp("playerbuffersize");
			if(status.mcaktiv == 0 && getconfigint("tithek_delservice", NULL) == 0)
				servicecheckret(servicestart(status.lastservice->channel, NULL, NULL, 0), 0);
		}
		else if(ostrcmp(keyconf, "Streaming Playback Caching (10MB)") == 0)
		{
			addconfigtmp("playerbuffersize", "10485760");
			screenplay(tmpstr1, filename, 2, flag);
			delconfigtmp("playerbuffersize");
			if(status.mcaktiv == 0 && getconfigint("tithek_delservice", NULL) == 0)
				servicecheckret(servicestart(status.lastservice->channel, NULL, NULL, 0), 0);
		}
		else if(ostrcmp(keyconf, "File Caching Playback (10MB / 120s)") == 0)
		{
			cacheplay(tmpstr1, filename, 1);
			if(status.mcaktiv == 0 && getconfigint("tithek_delservice", NULL) == 0)
				servicecheckret(servicestart(status.lastservice->channel, NULL, NULL, 0), 0);
		}
		else if(ostrcmp(keyconf, "File Caching Playback (20MB / 240s)") == 0)
		{
			cacheplay(tmpstr1, filename, 2);
			if(status.mcaktiv == 0 && getconfigint("tithek_delservice", NULL) == 0)
				servicecheckret(servicestart(status.lastservice->channel, NULL, NULL, 0), 0);
		}
		else if(ostrcmp(keyconf, "File Caching Playback (30MB / 360s)") == 0)
		{
			cacheplay(tmpstr1, filename, 3);
			if(status.mcaktiv == 0 && getconfigint("tithek_delservice", NULL) == 0)
				servicecheckret(servicestart(status.lastservice->channel, NULL, NULL, 0), 0);
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
					if(status.mcaktiv == 0 && getconfigint("tithek_delservice", NULL) == 0)
						servicecheckret(servicestart(status.lastservice->channel, NULL, NULL, 0), 0);
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
		else if(ostrcmp(keyconf, "Download Full File (youtube_dl)") == 0)
		{
			char* search = textinput(_("Filename"), filename);
			if(search != NULL)
				backgroundytdl(tmpstr1, search);
			free(search), search = NULL;
		}
		else if(ostrcmp(keyconf, "Download Full File (hlsdl)") == 0)
		{
			char* search = textinput(_("Filename"), filename);
			if(search != NULL)
				backgroundhlsdl(tmpstr1, search);
			free(search), search = NULL;
		}
		else if(ostrcmp(keyconf, "Download Full File (curldl)") == 0)
		{
			char* search = textinput(_("Filename"), filename);
			if(search != NULL)
				backgroundcurldl(tmpstr1, search);
			free(search), search = NULL;
		}

		free(filename), filename = NULL;
		freemenulist(mlist, 1); mlist = NULL;
	}
	else
		textbox(_("Message"), _("Can't get Streamurl !"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 0, 0);

	free(tmpstr1); tmpstr1 = NULL;
}

void screentithekplay(char* titheklink, char* title, int first)
{
	int savevol = 0;
	if(!file_exist("/mnt/swapextensions/player"))
		mkdir("/mnt/swapextensions/player", 0777);

	char* disclaimer = NULL;
	disclaimer = ostrcat(disclaimer, "/mnt/swapextensions/player/tithek_disclaimer_accepted", 1, 0);

	if(!file_exist(disclaimer))
	{
		char* tmpstr = gethttp("openaaf.dyndns.tv", "/mediathek/disclaimer.txt", 80, NULL, HTTPAUTH, 5000, NULL, 0);
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
		savevol = getvol();

		mkdir("/tmp/tithek", 777);
		if(status.mcaktiv == 0 && getconfigint("tithek_delservice", NULL) == 1)
		{
			rcret = servicestop(status.aktservice, 1, 1);
			if(rcret == 1) return;
		}
    	gethttp("openaaf.dyndns.tv", "/mediathek/menu/default.jpg", 80, "/tmp/tithek/default.jpg", HTTPAUTH, 5000, NULL, 0);
	}

	struct skin* grid = getscreen("titheklist");
	struct skin* listbox = getscreennode(grid, "listbox");
	struct skin* countlabel = getscreennode(grid, "countlabel");
	struct skin* countpage = getscreennode(grid, "countpage");
	struct skin* b4 = getscreennode(grid, "b4");
	struct skin* b5 = getscreennode(grid, "b5");
	struct skin* b6 = getscreennode(grid, "b6");
	struct skin* b8 = getscreennode(grid, "b8");
	struct skin* load = getscreen("loading");
	struct skin* tmp = NULL;
	char* tithekpic = NULL;

	b4->usesavebg = 1;
	b5->usesavebg = 1;
	b6->usesavebg = 1;
	b8->usesavebg = 1;
	drawscreen(grid, 2, 0);
/*
//	if(ostrcmp(title, _("Tithek - Mainmenu - Favoriten")) == 0)
	if(ostrstr(title, _("Tithek - Mainmenu - Favoriten")) != NULL)
	{
		changetext(b4, _("EDIT FAV"));
		changetext(b5, _("DEL FAV"));
		changetext(b6, _("Repeat"));
		b5->hidden = NO;
	}
	else
	{
		changetext(b4, _("ADD FAV"));
		changetext(b5, _("Repeat"));
		changetext(b6, _("Exit Tithek"));
//		b5->hidden = YES;
	}

*/
	if(ostrstr(title, _("Tithek - Mainmenu - Favoriten")) != NULL)
	{
		changetext(b4, _("EDIT FAV"));
		changetext(b5, _("DEL FAV"));
		if(status.repeat == 0)
			changetext(b6, _("Repeat"));
		else
			changetext(b6, _("Repeat-On"));
//			b5->hidden = NO;
	}
	else
	{
		changetext(b4, _("ADD FAV"));
//			b5->hidden = YES;
		if(status.repeat == 0)
			changetext(b5, _("Repeat"));
		else
			changetext(b5, _("Repeat-On"));

		changetext(b6, _("Exit Tithek"));
	}

	drawscreen(load, 0, 0);

	if(titheklink == NULL) return;

	listbox->aktpage = -1;
	listbox->aktline = 1;
	listbox->gridcol = 0;
	listbox->select = NULL;

/*
	if(getconfigint("tithek_cover", NULL) >= 6 || getconfigint("tithek_view", NULL) >= 6)
	{
		listbox = getscreennode(grid, "listbox");
	}
	else
	{
		listbox = getscreennode(grid, "grid");
	}
*/
	pagecount = createtithekplay(titheklink, grid, listbox, countlabel, title, first);
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

        		if(tmp->handle != NULL && ((struct tithek*)tmp->handle)->description == NULL)
            		b8->hidden = YES;
                else
             		b8->hidden = NO;

				if(tmp->handle != NULL && getconfigint("tithek_view", NULL) != 6 && getconfigint("tithek_cover", NULL) != 6)
				{
                    if(!ostrncmp("curl ", ((struct tithek*)tmp->handle)->pic, 5) || !ostrncmp("echo ", ((struct tithek*)tmp->handle)->pic, 5) || !ostrncmp("/tmp/localhoster/hoster.sh get", ((struct tithek*)tmp->handle)->pic, 30))
                    {
                        debug(99, "pic cmd: %s", ((struct tithek*)tmp->handle)->pic);
                        ((struct tithek*)tmp->handle)->pic = command(((struct tithek*)tmp->handle)->pic);
                        debug(99, "pic change to: %s", ((struct tithek*)tmp->handle)->pic);
                    }
                    if(cmpfilenameext(((struct tithek*)tmp->handle)->pic, ".png") != cmpfilenameext(((struct tithek*)tmp->handle)->localname, ".png"))
                    {
                        debug(99, "pic png: %s", ((struct tithek*)tmp->handle)->pic);
                        ((struct tithek*)tmp->handle)->localname = ostrcat(((struct tithek*)tmp->handle)->localname, ".png", 0, 0);
                        debug(99, "pic change localname: %s", ((struct tithek*)tmp->handle)->localname);
                    }

					tithekpic = tithekdownload(((struct tithek*)tmp->handle)->pic, ((struct tithek*)tmp->handle)->localname, "aXBrLUdaRmg6RkhaVkJHaG56ZnZFaEZERlRHenVpZjU2NzZ6aGpHVFVHQk5Iam0=", 1, 0);

					/* not working with thread download
					off64_t checkpic = getfilesize(tithekpic);

					if(checkpic < 1000)
					{
						free(tithekpic); tithekpic = NULL;
						tithekpic = ostrcat("/var/usr/local/share/titan/plugins/tithek/default.jpg", NULL, 0, 0);
					}
					*/
/*
                    if(!file_exist(tithekpic))
					{
                        debug(99, "not found: %s use default /var/usr/local/share/titan/plugins/tithek/default.jpg", tithekpic);
						free(tithekpic); tithekpic = NULL;
						tithekpic = ostrcat("/var/usr/local/share/titan/plugins/tithek/default.jpg", NULL, 0, 0);
					}
*/
					if(getconfigint("tithek_view", NULL) != 6 && getconfigint("tithek_cover", NULL) != 6)
					{
                        debug(99, "changepic: %s", tithekpic);
						changepic(tmp, tithekpic);
					}
					free(tithekpic); tithekpic = NULL;
				}
				tmp = tmp->prev;
			}
			tmp = listbox->select;
			if(tmp != NULL) tmp = tmp->next;
			while(tmp != NULL)
			{
        		if(tmp->handle != NULL && ((struct tithek*)tmp->handle)->description == NULL)
            		b8->hidden = YES;
                else
             		b8->hidden = NO;

				if(tmp->pagecount != listbox->aktpage) break;

				if(tmp->handle != NULL && getconfigint("tithek_view", NULL) != 6 && getconfigint("tithek_cover", NULL) != 6)
				{
                    if(!ostrncmp("curl ", ((struct tithek*)tmp->handle)->pic, 5) || !ostrncmp("echo ", ((struct tithek*)tmp->handle)->pic, 5) || !ostrncmp("/tmp/localhoster/hoster.sh get", ((struct tithek*)tmp->handle)->pic, 30))
                    {
                        debug(99, "pic cmd: %s", ((struct tithek*)tmp->handle)->pic);
                        ((struct tithek*)tmp->handle)->pic = command(((struct tithek*)tmp->handle)->pic);
                        debug(99, "pic change to: %s", ((struct tithek*)tmp->handle)->pic);
                    }
                    if(cmpfilenameext(((struct tithek*)tmp->handle)->pic, ".png") != cmpfilenameext(((struct tithek*)tmp->handle)->localname, ".png"))
                    {
                        debug(99, "pic png: %s", ((struct tithek*)tmp->handle)->pic);
                        ((struct tithek*)tmp->handle)->localname = ostrcat(((struct tithek*)tmp->handle)->localname, ".png", 0, 0);
                        debug(99, "pic change localname: %s", ((struct tithek*)tmp->handle)->localname);
                    }

					tithekpic = tithekdownload(((struct tithek*)tmp->handle)->pic, ((struct tithek*)tmp->handle)->localname, "aXBrLUdaRmg6RkhaVkJHaG56ZnZFaEZERlRHenVpZjU2NzZ6aGpHVFVHQk5Iam0=", 1, 0);

					/* not working with thread download
					off64_t checkpic = getfilesize(tithekpic);

					if(checkpic < 1000)
					{
						free(tithekpic); tithekpic = NULL;
						tithekpic = ostrcat("/var/usr/local/share/titan/plugins/tithek/default.jpg", NULL, 0, 0);
					}
					*/
/*
                    if(!file_exist(tithekpic))
					{
                        debug(99, "not found: %s use default /var/usr/local/share/titan/plugins/tithek/default.jpg", tithekpic);
						free(tithekpic); tithekpic = NULL;
						tithekpic = ostrcat("/var/usr/local/share/titan/plugins/tithek/default.jpg", NULL, 0, 0);
					}
*/
					if(getconfigint("tithek_view", NULL) != 6 && getconfigint("tithek_cover", NULL) != 6)
					{
                        debug(99, "changepic: %s", tithekpic);
						changepic(tmp, tithekpic);
					}
					free(tithekpic); tithekpic = NULL;
				}
				tmp = tmp->next;
			}
		}

		int count = getfilecount(TITHEKPATH);
		if(count > 500)
			delallfiles(TITHEKPATH, ".jpg");

waitrcstart:

		// reload selected pic
		if(getconfigint("tithek_description", NULL) == 1 || (getconfigint("tithek_view", NULL) != 6 && getconfigint("tithek_cover", NULL) != 6))
		{
			if(listbox->select != NULL && listbox->select->handle != NULL)
			{
        		if(((struct tithek*)listbox->select->handle)->description == NULL)
            		b8->hidden = YES;
                else
             		b8->hidden = NO;

				if(((struct tithek*)listbox->select->handle)->pic != NULL && ((struct tithek*)listbox->select->handle)->localname != NULL)
				{
                    if(!ostrncmp("curl ", ((struct tithek*)listbox->select->handle)->pic, 5) || !ostrncmp("echo ", ((struct tithek*)listbox->select->handle)->pic, 5) || !ostrncmp("/tmp/localhoster/hoster.sh get", ((struct tithek*)listbox->select->handle)->pic, 30))
                    {
                        debug(99, "pic cmd: %s", ((struct tithek*)listbox->select->handle)->pic);
                        ((struct tithek*)listbox->select->handle)->pic = command(((struct tithek*)listbox->select->handle)->pic);
                        debug(99, "pic change to: %s", ((struct tithek*)listbox->select->handle)->pic);
                    }
                    if(cmpfilenameext(((struct tithek*)listbox->select->handle)->pic, ".png") != cmpfilenameext(((struct tithek*)listbox->select->handle)->localname, ".png"))
                    {
                        debug(99, "pic png: %s", ((struct tithek*)listbox->select->handle)->pic);
                        ((struct tithek*)listbox->select->handle)->localname = ostrcat(((struct tithek*)listbox->select->handle)->localname, ".png", 0, 0);
                        debug(99, "pic change localname: %s", ((struct tithek*)listbox->select->handle)->localname);
                    }
					tithekpic = tithekdownload(((struct tithek*)listbox->select->handle)->pic, ((struct tithek*)listbox->select->handle)->localname, "aXBrLUdaRmg6RkhaVkJHaG56ZnZFaEZERlRHenVpZjU2NzZ6aGpHVFVHQk5Iam0=", 1, 0);

					if(file_exist(tithekpic))
					{
                        debug(99, "changepic: %s", tithekpic);
						changepic(listbox->select, tithekpic);
					}
/*
					else
					{
                        debug(99, "not found: %s use default /tmp/tithek/default.jpg", tithekpic);
                        if(!file_exist("/tmp/tithek/default.jpg"))
    	                    gethttp("openaaf.dyndns.tv", "/mediathek/menu/default.jpg", 80, "/tmp/tithek/default.jpg", HTTPAUTH, 5000, NULL, 0);
						free(tithekpic); tithekpic = NULL;
						tithekpic = ostrcat("/tmp/tithek/default.jpg", NULL, 0, 0);
						changepic(listbox->select, tithekpic);
					}
*/
//					free(tithekpic); tithekpic = NULL;
				}

        		if(getconfigint("tithek_description", NULL) == 1 && ((struct tithek*)listbox->select->handle)->description != NULL && (!ostrncmp("curl ", ((struct tithek*)listbox->select->handle)->description, 5) || !ostrncmp("/tmp/localhoster/hoster.sh get", ((struct tithek*)listbox->select->handle)->description, 30)))
                {
                    debug(99, "found description cmd: %s", ((struct tithek*)listbox->select->handle)->description);
                    char * desc = NULL;
                    desc = command(((struct tithek*)listbox->select->handle)->description);
                    if(desc != NULL)
                    {
        				desc = string_replace_all("#", ":", desc, 1);
                        ((struct tithek*)listbox->select->handle)->description = desc;
                        debug(99, "set description: %s", desc);
                        changetext2(listbox->select, desc);
                    }
                    free(desc), desc = NULL;
					if(file_exist(tithekpic))
					{
                        debug(99, "changepic: %s", tithekpic);
						changepic(listbox->select, tithekpic);
					}
                }
					free(tithekpic); tithekpic = NULL;

			}
		}

		drawscreen(grid, 0, 0);

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

//		if(rcret == getrcconfigint("rcblue", NULL))
//		{
//			tithekexit = 1;
//			break;
//		}

		if(rcret == getrcconfigint("rcexit", NULL))
		{
			delconfigtmp("tithek_description");
			break;
		}
		if(rcret == getrcconfigint("rcmenu", NULL))
		{
			screentithek_settings();
/*
			if(getconfigint("tithek_cover", NULL) >= 6 || getconfigint("tithek_view", NULL) >= 6)
			{
				listbox = getscreennode(grid, "listbox");
			}
				else
			{
				listbox = getscreennode(grid, "grid");
			}
*/
//			pagecount = createtithekplay(titheklink, grid, listbox, countlabel, first);
			pagecount = createtithekplay(titheklink, grid, listbox, countlabel, title, first + 1);

			if(pagecount == 0 || tithekexit == 1) break;

			listbox->aktpage = -1;
			listbox->aktline = 1;
			listbox->gridcol = 1;
			addscreenrc(grid, listbox);
//			drawscreen(grid, 0, 0);
		}

//		if(ostrcmp(title, _("Tithek - Mainmenu - Favoriten")) == 0)
		if(ostrstr(title, _("Tithek - Mainmenu - Favoriten")) != NULL)
		{
//			changetext(b4, _("EDIT FAV"));
//			b5->hidden = NO;
		}
		else
		{
//			changetext(b4, _("ADD FAV"));
//			b5->hidden = YES;
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
				if(check == 0 || check == 1000)
				{
					char* search = textinputhist(_("Create sub folder"), " ", "searchhist");
					if(search != NULL)
					{
						strstrip(search);
						char* tmpstr = ostrcat(search, NULL, 0, 0);
						tmpstr = string_replace_all(" ", "_", tmpstr, 1);
						tmpstr = string_replace_all(".", "_", tmpstr, 1);
						tmpstr = string_replace_all("-", "_", tmpstr, 1);
						tmpstr = string_replace_all("._.", "_", tmpstr, 1);
						tmpstr = string_replace_all("__", "_", tmpstr, 1);
						string_tolower(tmpstr);
						strstrip(tmpstr);
						char* tmpstr1 = NULL;
						tmpstr1 = ostrcat(getconfig("tithek_fav", NULL), "_", 0, 0);
						tmpstr1 = ostrcat(tmpstr1, tmpstr, 1, 0);
						if(textbox(_("Message"), _("Add this sub folder as Favorite ?"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 5, 0) == 1)
							addfav(search, tmpstr1, "http://imageshack.us/md/up/grd/mediathek/menu/default.jpg", tmpstr, title, NULL, check);
						free(tmpstr), tmpstr = NULL;
						free(tmpstr1), tmpstr1 = NULL;
					}
					free(search), search = NULL;

					pagecount = createtithekplay(titheklink, grid, listbox, countlabel, title, 0);
					if(pagecount == 0) return;
				}
				else if(check == 2)
				{
					if(kinox_search(grid, listbox, countlabel, load, ((struct tithek*)listbox->select->handle)->link, "KinoX - Search", tmpstr, 0) == 0)
						if(screenlistbox(grid, listbox, countlabel, "KinoX - Search", titheklink, &pagecount, &tithekexit, &oaktpage, &oaktline, &ogridcol, 0, 0) == 0) break;
				}
/*
				else if(check == 3)
				{
					if(solarmovie_search(grid, listbox, countlabel, load, ((struct tithek*)listbox->select->handle)->link, "SolarMovies - Movie - Search", tmpstr, 0) == 0)
						if(screenlistbox(grid, listbox, countlabel, "SolarMovies - Movie - Search", titheklink, &pagecount, &tithekexit, &oaktpage, &oaktline, &ogridcol, 0, 0) == 0) break;
				}
				else if(check == 4)
				{
					if(solarmovie_search(grid, listbox, countlabel, load, ((struct tithek*)listbox->select->handle)->link, "SolarMovies - Tv - Search", tmpstr, 1) == 0)
						if(screenlistbox(grid, listbox, countlabel, "SolarMovies - Tv - Search", titheklink, &pagecount, &tithekexit, &oaktpage, &oaktline, &ogridcol, 0, 0) == 0) break;
				}
*/
/*
				else if(check == 5)
				{
					if(youtube_search(grid, listbox, countlabel, load, ((struct tithek*)listbox->select->handle)->link, "Youtube - Search", tmpstr) == 0)
						if(screenlistbox(grid, listbox, countlabel, "Youtube - Search", titheklink, &pagecount, &tithekexit, &oaktpage, &oaktline, &ogridcol, 0, 0) == 0) break;
				}
*/
/*
				else if(check == 6)
				{
					if(myvideo_search(grid, listbox, countlabel, load, ((struct tithek*)listbox->select->handle)->link, "MyVideo - Search", tmpstr, 0) == 0)
						if(screenlistbox(grid, listbox, countlabel, "MyVideo - Search", titheklink, &pagecount, &tithekexit, &oaktpage, &oaktline, &ogridcol, 0, 0) == 0) break;
				}
*/
/*
				else if(check == 7)
				{
					if(movie4k_search(grid, listbox, countlabel, load, ((struct tithek*)listbox->select->handle)->link, "KinoX - Search", tmpstr, 0) == 0)
						if(screenlistbox(grid, listbox, countlabel, "Movie4k - Search", titheklink, &pagecount, &tithekexit, &oaktpage, &oaktline, &ogridcol, 0, 0) == 0) break;
				}
*/
				else if(check == 8)
				{
					if(kinox_search_local(grid, listbox, countlabel, load, ((struct tithek*)listbox->select->handle)->link, "KinoX - Search (local)", tmpstr, 0) == 0)
						if(screenlistbox(grid, listbox, countlabel, "KinoX - Search (local)", titheklink, &pagecount, &tithekexit, &oaktpage, &oaktline, &ogridcol, 0, 0) == 0) break;
				}
/*
				else if(check == 9)
				{
					if(solarmovie_search_local(grid, listbox, countlabel, load, ((struct tithek*)listbox->select->handle)->link, "Solarmovies - Search (local)", tmpstr, 0) == 0)
						if(screenlistbox(grid, listbox, countlabel, "Solarmovies - Search (local)", titheklink, &pagecount, &tithekexit, &oaktpage, &oaktline, &ogridcol, 0, 0) == 0) break;
				}
*/
/*
				else if(check == 10)
				{
					if(youtube_search_local(grid, listbox, countlabel, load, ((struct tithek*)listbox->select->handle)->link, "Youtube - Search (local)", tmpstr, 0) == 0)
						if(screenlistbox(grid, listbox, countlabel, "Youtube - Search (local)", titheklink, &pagecount, &tithekexit, &oaktpage, &oaktline, &ogridcol, 0, 0) == 0) break;
				}
*/
/*
				else if(check == 11)
				{
					if(myvideo_search_local(grid, listbox, countlabel, load, ((struct tithek*)listbox->select->handle)->link, "MyVideo - Search (local)", tmpstr, 0) == 0)
						if(screenlistbox(grid, listbox, countlabel, "MyVideo - Search (local)", titheklink, &pagecount, &tithekexit, &oaktpage, &oaktline, &ogridcol, 0, 0) == 0) break;
				}
*/
/*
				else if(check == 12)
				{
					if(movie4k_search_local(grid, listbox, countlabel, load, ((struct tithek*)listbox->select->handle)->link, "Movie4k - Search (local)", tmpstr, 0) == 0)
						if(screenlistbox(grid, listbox, countlabel, "Movie4k - Search (local)", titheklink, &pagecount, &tithekexit, &oaktpage, &oaktline, &ogridcol, 0, 0) == 0) break;
				}
*/
				else if(check == 13)
				{
					if(internetradio_search_local(grid, listbox, countlabel, load, ((struct tithek*)listbox->select->handle)->link, "Internetradio - Search (local)", tmpstr, 0) == 0)
						if(screenlistbox(grid, listbox, countlabel, "Internetradio - Search (local)", titheklink, &pagecount, &tithekexit, &oaktpage, &oaktline, &ogridcol, 0, 0) == 0) break;
				}
				else if(check == 14)
				{
					if(internettv_search_local(grid, listbox, countlabel, load, ((struct tithek*)listbox->select->handle)->link, "Internettv - Search (local)", tmpstr, 0) == 0)
						if(screenlistbox(grid, listbox, countlabel, "Internettv - Search (local)", titheklink, &pagecount, &tithekexit, &oaktpage, &oaktline, &ogridcol, 0, 0) == 0) break;
				}
				else if(check == 15)
				{
					if(ard_search_local(grid, listbox, countlabel, load, ((struct tithek*)listbox->select->handle)->link, "ARD - Search (local)", tmpstr, 0) == 0)
						if(screenlistbox(grid, listbox, countlabel, "ARD - Search (local)", titheklink, &pagecount, &tithekexit, &oaktpage, &oaktline, &ogridcol, 0, 0) == 0) break;
				}
/*
				else if(check == 16)
				{
					if(zdf_search_local(grid, listbox, countlabel, load, ((struct tithek*)listbox->select->handle)->link, "ZDF - Search (local)", tmpstr, 0) == 0)
						if(screenlistbox(grid, listbox, countlabel, "ZDF - Search (local)", titheklink, &pagecount, &tithekexit, &oaktpage, &oaktline, &ogridcol, 0, 0) == 0) break;
				}
*/
/*
				else if(check == 17)
				{
					if(tectime_search_local(grid, listbox, countlabel, load, ((struct tithek*)listbox->select->handle)->link, "TecTime - Search (local)", tmpstr, 0) == 0)
						if(screenlistbox(grid, listbox, countlabel, "TecTime - Search (local)", titheklink, &pagecount, &tithekexit, &oaktpage, &oaktline, &ogridcol, 0, 0) == 0) break;
				}
*/
/*
				else if(check == 18)
				{
					if(giga_search_local(grid, listbox, countlabel, load, ((struct tithek*)listbox->select->handle)->link, "Giga - Search (local)", tmpstr, 0) == 0)
						if(screenlistbox(grid, listbox, countlabel, "Giga - Search (local)", titheklink, &pagecount, &tithekexit, &oaktpage, &oaktline, &ogridcol, 0, 0) == 0) break;
				}
*/
/*
				else if(check == 19)
				{
					if(beeg_search_local(grid, listbox, countlabel, load, ((struct tithek*)listbox->select->handle)->link, "Beeg - Search (local)", tmpstr, 0) == 0)
						if(screenlistbox(grid, listbox, countlabel, "Beeg - Search (local)", titheklink, &pagecount, &tithekexit, &oaktpage, &oaktline, &ogridcol, 0, 0) == 0) break;
				}
*/
/*
				else if(check == 20)
				{
					if(nowtv_search_local(grid, listbox, countlabel, load, ((struct tithek*)listbox->select->handle)->link, "Rtl2Now - Search (local)", tmpstr, 0) == 0)
						if(screenlistbox(grid, listbox, countlabel, "NowTv - Search (local)", titheklink, &pagecount, &tithekexit, &oaktpage, &oaktline, &ogridcol, 0, 0) == 0) break;
				}
*/
				else if(check == 21)
				{
					if(all_search_local(grid, listbox, countlabel, load, ((struct tithek*)listbox->select->handle)->link, "Rtl-Now - Search (local)", tmpstr, 0) == 0)
						if(screenlistbox(grid, listbox, countlabel, "All - Search (local)", titheklink, &pagecount, &tithekexit, &oaktpage, &oaktline, &ogridcol, 0, 0) == 0) break;
				}
				else if(check == 24)
				{
					if(xvideos_search_local(grid, listbox, countlabel, load, ((struct tithek*)listbox->select->handle)->link, "Xvideos - Search (local)", tmpstr, 0) == 0)
						if(screenlistbox(grid, listbox, countlabel, "Xvideos - Search (local)", titheklink, &pagecount, &tithekexit, &oaktpage, &oaktline, &ogridcol, 0, 0) == 0) break;
				}
/*
				else if(check == 25)
				{
					if(mlehd_search_local(grid, listbox, countlabel, load, ((struct tithek*)listbox->select->handle)->link, "Mle-HD - Search (local)", tmpstr, 0) == 0)
						if(screenlistbox(grid, listbox, countlabel, "Mle-HD - Search (local)", titheklink, &pagecount, &tithekexit, &oaktpage, &oaktline, &ogridcol, 0, 0) == 0) break;
				}
*/
				else if(check == 26)
				{
					if(netzkino_search_local(grid, listbox, countlabel, load, ((struct tithek*)listbox->select->handle)->link, "Netzkino - Search (local)", tmpstr, 0) == 0)
						if(screenlistbox(grid, listbox, countlabel, "Netzkino - Search (local)", titheklink, &pagecount, &tithekexit, &oaktpage, &oaktline, &ogridcol, 0, 0) == 0) break;
				}
				else if(check == 27)
				{
					if(filmon_search_local(grid, listbox, countlabel, load, ((struct tithek*)listbox->select->handle)->link, "FilmOn - Search (local)", tmpstr, 0) == 0)
						if(screenlistbox(grid, listbox, countlabel, "FilmOn - Search (local)", titheklink, &pagecount, &tithekexit, &oaktpage, &oaktline, &ogridcol, 0, 0) == 0) break;
				}
/*
why ?
				else if((((struct tithek*)listbox->select->handle)->flag == 13))
				{
					if(myvideo_search(grid, listbox, countlabel, load, ((struct tithek*)listbox->select->handle)->link, "MyVideo - Search", tmpstr, 0) == 0)
						if(screenlistbox(grid, listbox, countlabel, title, titheklink, &pagecount, &tithekexit, &oaktpage, &oaktline, &ogridcol, 0, 0) == 0) break;
				}
*/
				free(tmpstr), tmpstr = NULL;
			}
			else
			{
				int check = playrcred(NULL, NULL, 1, 0, 199);
				if(check == 0 || check == 1000)
				{
					char* search = textinputhist(_("Create sub folder"), " ", "searchhist");
					if(search != NULL)
					{
						strstrip(search);
						char* tmpstr = ostrcat(search, NULL, 0, 0);
						tmpstr = string_replace_all(" ", "_", tmpstr, 1);
						tmpstr = string_replace_all(".", "_", tmpstr, 1);
						tmpstr = string_replace_all("-", "_", tmpstr, 1);
						tmpstr = string_replace_all("._.", "_", tmpstr, 1);
						tmpstr = string_replace_all("__", "_", tmpstr, 1);
						string_tolower(tmpstr);
						strstrip(tmpstr);
						char* tmpstr1 = NULL;
						tmpstr1 = ostrcat(getconfig("tithek_fav", NULL), "_", 0, 0);
						tmpstr1 = ostrcat(tmpstr1, tmpstr, 1, 0);
						if(textbox(_("Message"), _("Add this sub folder as Favorite ?"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 5, 0) == 1)
							addfav(search, tmpstr1, "http://imageshack.us/md/up/grd/mediathek/menu/default.jpg", tmpstr, title, NULL, check);
						free(tmpstr), tmpstr = NULL;
						free(tmpstr1), tmpstr1 = NULL;
					}
					free(search), search = NULL;

					pagecount = createtithekplay(titheklink, grid, listbox, countlabel, title , 0);
					if(pagecount == 0) return;
				}
			}
		}
		else if(rcret == getrcconfigint("rcok", NULL))
		{
			if(listbox->select != NULL && listbox->select->handle != NULL)
			{
				clearscreen(grid);

				if(((struct tithek*)listbox->select->handle)->flag == 2 || ((struct tithek*)listbox->select->handle)->flag == 3 || ((struct tithek*)listbox->select->handle)->flag == 4 || ((struct tithek*)listbox->select->handle)->flag == 5 || ((struct tithek*)listbox->select->handle)->flag == 6 || ((struct tithek*)listbox->select->handle)->flag == 12 || ((struct tithek*)listbox->select->handle)->flag == 14 || ((struct tithek*)listbox->select->handle)->flag == 15 || ((struct tithek*)listbox->select->handle)->flag == 20 || ((struct tithek*)listbox->select->handle)->flag == 38 || ((struct tithek*)listbox->select->handle)->flag == 42 || ((struct tithek*)listbox->select->handle)->flag == 45 || ((struct tithek*)listbox->select->handle)->flag == 46 || ((struct tithek*)listbox->select->handle)->flag == 64 || ((struct tithek*)listbox->select->handle)->flag == 50 || ((struct tithek*)listbox->select->handle)->flag == 41 || ((struct tithek*)listbox->select->handle)->flag == 43 || ((struct tithek*)listbox->select->handle)->flag == 75 || ((struct tithek*)listbox->select->handle)->flag == 91 || ((struct tithek*)listbox->select->handle)->flag == 92 || ((struct tithek*)listbox->select->handle)->flag == 93 || ((struct tithek*)listbox->select->handle)->flag == 94 || ((struct tithek*)listbox->select->handle)->flag == 98 || ((struct tithek*)listbox->select->handle)->flag == 111)
				{
					submenu(listbox, load, title);
//					drawscreen(grid, 0, 0);
				}
/*
				else if((((struct tithek*)listbox->select->handle)->flag == 9) || (((struct tithek*)listbox->select->handle)->flag == 10) || (((struct tithek*)listbox->select->handle)->flag == 11))
				{
					if(youtube_search(grid, listbox, countlabel, load, ((struct tithek*)listbox->select->handle)->link, ((struct tithek*)listbox->select->handle)->title, NULL) == 0)
						if(screenlistbox(grid, listbox, countlabel, title, titheklink, &pagecount, &tithekexit, &oaktpage, &oaktline, &ogridcol, 0, 0) == 0) break;
				}
*/
/*
				else if(((struct tithek*)listbox->select->handle)->flag == 13)
				{
					if(myvideo_search(grid, listbox, countlabel, load, ((struct tithek*)listbox->select->handle)->link, ((struct tithek*)listbox->select->handle)->title, NULL, 0) == 0)
						if(screenlistbox(grid, listbox, countlabel, title, titheklink, &pagecount, &tithekexit, &oaktpage, &oaktline, &ogridcol, 0, 0) == 0) break;
				}
*/
				else if(((struct tithek*)listbox->select->handle)->flag == 24)
				{
					if(internetradio_search_local(grid, listbox, countlabel, load, ((struct tithek*)listbox->select->handle)->link, ((struct tithek*)listbox->select->handle)->title, NULL, 0) == 0)
						if(screenlistbox(grid, listbox, countlabel, title, titheklink, &pagecount, &tithekexit, &oaktpage, &oaktline, &ogridcol, 0, 0) == 0) break;
				}
				else if(((struct tithek*)listbox->select->handle)->flag == 25)
				{
					if(kinox_search_local(grid, listbox, countlabel, load, ((struct tithek*)listbox->select->handle)->link, ((struct tithek*)listbox->select->handle)->title, NULL, 0) == 0)
						if(screenlistbox(grid, listbox, countlabel, title, titheklink, &pagecount, &tithekexit, &oaktpage, &oaktline, &ogridcol, 0, 0) == 0) break;
				}
/*
				else if(((struct tithek*)listbox->select->handle)->flag == 26)
				{
					if(movie4k_search_local(grid, listbox, countlabel, load, ((struct tithek*)listbox->select->handle)->link, ((struct tithek*)listbox->select->handle)->title, NULL, 0) == 0)
						if(screenlistbox(grid, listbox, countlabel, title, titheklink, &pagecount, &tithekexit, &oaktpage, &oaktline, &ogridcol, 0, 0) == 0) break;
				}
*/
/*
				else if(((struct tithek*)listbox->select->handle)->flag == 27)
				{
					if(solarmovie_search_local(grid, listbox, countlabel, load, ((struct tithek*)listbox->select->handle)->link, ((struct tithek*)listbox->select->handle)->title, NULL, 0) == 0)
						if(screenlistbox(grid, listbox, countlabel, title, titheklink, &pagecount, &tithekexit, &oaktpage, &oaktline, &ogridcol, 0, 0) == 0) break;
				}
*/
				else if(((struct tithek*)listbox->select->handle)->flag == 47)
				{
					if(internettv_search_local(grid, listbox, countlabel, load, ((struct tithek*)listbox->select->handle)->link, ((struct tithek*)listbox->select->handle)->title, NULL, 0) == 0)
						if(screenlistbox(grid, listbox, countlabel, title, titheklink, &pagecount, &tithekexit, &oaktpage, &oaktline, &ogridcol, 0, 0) == 0) break;
				}
/*
				else if(((struct tithek*)listbox->select->handle)->flag == 48)
				{
					if(youtube_search_local(grid, listbox, countlabel, load, ((struct tithek*)listbox->select->handle)->link, ((struct tithek*)listbox->select->handle)->title, NULL, 0) == 0)
						if(screenlistbox(grid, listbox, countlabel, title, titheklink, &pagecount, &tithekexit, &oaktpage, &oaktline, &ogridcol, 0, 0) == 0) break;
				}
*/
/*
				else if(((struct tithek*)listbox->select->handle)->flag == 49)
				{
					if(myvideo_search_local(grid, listbox, countlabel, load, ((struct tithek*)listbox->select->handle)->link, ((struct tithek*)listbox->select->handle)->title, NULL, 0) == 0)
						if(screenlistbox(grid, listbox, countlabel, title, titheklink, &pagecount, &tithekexit, &oaktpage, &oaktline, &ogridcol, 0, 0) == 0) break;
				}
*/
				else if(((struct tithek*)listbox->select->handle)->flag == 51)
				{
					if(ard_search_local(grid, listbox, countlabel, load, ((struct tithek*)listbox->select->handle)->link, ((struct tithek*)listbox->select->handle)->title, NULL, 0) == 0)
						if(screenlistbox(grid, listbox, countlabel, title, titheklink, &pagecount, &tithekexit, &oaktpage, &oaktline, &ogridcol, 0, 0) == 0) break;
				}
/*
				else if(((struct tithek*)listbox->select->handle)->flag == 52)
				{
					if(zdf_search_local(grid, listbox, countlabel, load, ((struct tithek*)listbox->select->handle)->link, ((struct tithek*)listbox->select->handle)->title, NULL, 0) == 0)
						if(screenlistbox(grid, listbox, countlabel, title, titheklink, &pagecount, &tithekexit, &oaktpage, &oaktline, &ogridcol, 0, 0) == 0) break;
				}
*/
/*
				else if(((struct tithek*)listbox->select->handle)->flag == 53)
				{
					if(tectime_search_local(grid, listbox, countlabel, load, ((struct tithek*)listbox->select->handle)->link, ((struct tithek*)listbox->select->handle)->title, NULL, 0) == 0)
						if(screenlistbox(grid, listbox, countlabel, title, titheklink, &pagecount, &tithekexit, &oaktpage, &oaktline, &ogridcol, 0, 0) == 0) break;
				}
*/
/*
				else if(((struct tithek*)listbox->select->handle)->flag == 54)
				{
					if(giga_search_local(grid, listbox, countlabel, load, ((struct tithek*)listbox->select->handle)->link, ((struct tithek*)listbox->select->handle)->title, NULL, 0) == 0)
						if(screenlistbox(grid, listbox, countlabel, title, titheklink, &pagecount, &tithekexit, &oaktpage, &oaktline, &ogridcol, 0, 0) == 0) break;
				}
*/
/*
				else if(((struct tithek*)listbox->select->handle)->flag == 55)
				{
					if(beeg_search_local(grid, listbox, countlabel, load, ((struct tithek*)listbox->select->handle)->link, ((struct tithek*)listbox->select->handle)->title, NULL, 0) == 0)
						if(screenlistbox(grid, listbox, countlabel, title, titheklink, &pagecount, &tithekexit, &oaktpage, &oaktline, &ogridcol, 0, 0) == 0) break;
				}
*/
				else if(((struct tithek*)listbox->select->handle)->flag == 100)
				{
					if(all_search_local(grid, listbox, countlabel, load, ((struct tithek*)listbox->select->handle)->link, ((struct tithek*)listbox->select->handle)->title, NULL, 0) == 0)
						if(screenlistbox(grid, listbox, countlabel, title, titheklink, &pagecount, &tithekexit, &oaktpage, &oaktline, &ogridcol, 0, 0) == 0) break;
				}
				else if(((struct tithek*)listbox->select->handle)->flag == 60)
				{
					if(xvideos_search_local(grid, listbox, countlabel, load, ((struct tithek*)listbox->select->handle)->link, ((struct tithek*)listbox->select->handle)->title, NULL, 0) == 0)
						if(screenlistbox(grid, listbox, countlabel, title, titheklink, &pagecount, &tithekexit, &oaktpage, &oaktline, &ogridcol, 0, 0) == 0) break;
				}
/*
				else if(((struct tithek*)listbox->select->handle)->flag == 61)
				{
					if(mlehd_search_local(grid, listbox, countlabel, load, ((struct tithek*)listbox->select->handle)->link, ((struct tithek*)listbox->select->handle)->title, NULL, 0) == 0)
						if(screenlistbox(grid, listbox, countlabel, title, titheklink, &pagecount, &tithekexit, &oaktpage, &oaktline, &ogridcol, 0, 0) == 0) break;
				}
*/
				else if(((struct tithek*)listbox->select->handle)->flag == 62)
				{
					if(netzkino_search_local(grid, listbox, countlabel, load, ((struct tithek*)listbox->select->handle)->link, ((struct tithek*)listbox->select->handle)->title, NULL, 0) == 0)
						if(screenlistbox(grid, listbox, countlabel, title, titheklink, &pagecount, &tithekexit, &oaktpage, &oaktline, &ogridcol, 0, 0) == 0) break;
				}
				else if(((struct tithek*)listbox->select->handle)->flag == 63)
				{
					if(filmon_search_local(grid, listbox, countlabel, load, ((struct tithek*)listbox->select->handle)->link, ((struct tithek*)listbox->select->handle)->title, NULL, 0) == 0)
						if(screenlistbox(grid, listbox, countlabel, title, titheklink, &pagecount, &tithekexit, &oaktpage, &oaktline, &ogridcol, 0, 0) == 0) break;
				}
/*
				else if(((struct tithek*)listbox->select->handle)->flag == 65)
				{
					if(tvtoast_search_local(grid, listbox, countlabel, load, ((struct tithek*)listbox->select->handle)->link, ((struct tithek*)listbox->select->handle)->title, NULL, 0) == 0)
						if(screenlistbox(grid, listbox, countlabel, title, titheklink, &pagecount, &tithekexit, &oaktpage, &oaktline, &ogridcol, 0, 0) == 0) break;
				}
*/
/*
				else if(((struct tithek*)listbox->select->handle)->flag == 44)
				{
					if(myvideo_search(grid, listbox, countlabel, load, ((struct tithek*)listbox->select->handle)->link, ((struct tithek*)listbox->select->handle)->title, NULL, 1) == 0)
						if(screenlistbox(grid, listbox, countlabel, title, titheklink, &pagecount, &tithekexit, &oaktpage, &oaktline, &ogridcol, 0, 0) == 0) break;
				}
*/
				else if((((struct tithek*)listbox->select->handle)->flag == 21))
				{
					if(kinox_search(grid, listbox, countlabel, load, ((struct tithek*)listbox->select->handle)->link, ((struct tithek*)listbox->select->handle)->title, NULL, 0) == 0)
						if(screenlistbox(grid, listbox, countlabel, title, titheklink, &pagecount, &tithekexit, &oaktpage, &oaktline, &ogridcol, 0, 0) == 0) break;
				}
/*
				else if((((struct tithek*)listbox->select->handle)->flag == 33))
				{
					if(movie4k_search(grid, listbox, countlabel, load, ((struct tithek*)listbox->select->handle)->link, ((struct tithek*)listbox->select->handle)->title, NULL, 0) == 0)
						if(screenlistbox(grid, listbox, countlabel, title, titheklink, &pagecount, &tithekexit, &oaktpage, &oaktline, &ogridcol, 0, 0) == 0) break;
				}
*/
/*
				else if(((struct tithek*)listbox->select->handle)->flag == 29)
				{
					if(solarmovie_search(grid, listbox, countlabel, load, ((struct tithek*)listbox->select->handle)->link, ((struct tithek*)listbox->select->handle)->title, NULL, 0) == 0)
						if(screenlistbox(grid, listbox, countlabel, title, titheklink, &pagecount, &tithekexit, &oaktpage, &oaktline, &ogridcol, 0, 0) == 0) break;
				}
				else if(((struct tithek*)listbox->select->handle)->flag == 30)
				{
					if(solarmovie_search(grid, listbox, countlabel, load, ((struct tithek*)listbox->select->handle)->link, ((struct tithek*)listbox->select->handle)->title, NULL, 1) == 0)
						if(screenlistbox(grid, listbox, countlabel, title, titheklink, &pagecount, &tithekexit, &oaktpage, &oaktline, &ogridcol, 0, 0) == 0) break;
				}
*/
				else if((((struct tithek*)listbox->select->handle)->flag == 31))
				{
					if(kinox_search_cast(grid, listbox, countlabel, load, ((struct tithek*)listbox->select->handle)->link, ((struct tithek*)listbox->select->handle)->title, NULL) == 0)
						if(screenlistbox(grid, listbox, countlabel, title, titheklink, &pagecount, &tithekexit, &oaktpage, &oaktline, &ogridcol, 0, 0) == 0) break;
				}
				else if((((struct tithek*)listbox->select->handle)->flag == 32))
				{
					if(kinox_search(grid, listbox, countlabel, load, ((struct tithek*)listbox->select->handle)->link, ((struct tithek*)listbox->select->handle)->title, NULL, 1) == 0)
						if(screenlistbox(grid, listbox, countlabel, title, titheklink, &pagecount, &tithekexit, &oaktpage, &oaktline, &ogridcol, 0, 0) == 0) break;
				}
/*
				else if((((struct tithek*)listbox->select->handle)->flag == 33))
				{
					if(movie4k_search(grid, listbox, countlabel, load, ((struct tithek*)listbox->select->handle)->link, ((struct tithek*)listbox->select->handle)->title, NULL, 1) == 0)
						if(screenlistbox(grid, listbox, countlabel, title, titheklink, &pagecount, &tithekexit, &oaktpage, &oaktline, &ogridcol, 0, 3) == 0) break;
				}
				else if((((struct tithek*)listbox->select->handle)->flag == 34) || (((struct tithek*)listbox->select->handle)->flag == 35))
				{
					if(movie4k_hoster(grid, listbox, countlabel, load, ((struct tithek*)listbox->select->handle)->link, ((struct tithek*)listbox->select->handle)->title) == 0)
						if(screenlistbox(grid, listbox, countlabel, title, titheklink, &pagecount, &tithekexit, &oaktpage, &oaktline, &ogridcol, 0, 0) == 0) break;
				}
*/
				else if((((struct tithek*)listbox->select->handle)->flag == 22))
				{
					if(kinox_hoster(grid, listbox, countlabel, load, ((struct tithek*)listbox->select->handle)->link, ((struct tithek*)listbox->select->handle)->title) == 0)
						if(screenlistbox(grid, listbox, countlabel, title, titheklink, &pagecount, &tithekexit, &oaktpage, &oaktline, &ogridcol, 0, 3) == 0) break;
				}
/*
				else if((((struct tithek*)listbox->select->handle)->flag == 28))
				{
					if(solarmovie_hoster(grid, listbox, countlabel, load, ((struct tithek*)listbox->select->handle)->link, ((struct tithek*)listbox->select->handle)->title) == 0)
						if(screenlistbox(grid, listbox, countlabel, title, titheklink, &pagecount, &tithekexit, &oaktpage, &oaktline, &ogridcol, 0, 3) == 0) break;
				}
*/
				else if((((struct tithek*)listbox->select->handle)->flag == 23))
				{
					if(kinox_hoster_series(grid, listbox, countlabel, load, ((struct tithek*)listbox->select->handle)->link, ((struct tithek*)listbox->select->handle)->title) == 0)
						if(screenlistbox(grid, listbox, countlabel, title, titheklink, &pagecount, &tithekexit, &oaktpage, &oaktline, &ogridcol, 0, 0) == 0) break;
				}
/*
				else if((((struct tithek*)listbox->select->handle)->flag == 36) || (((struct tithek*)listbox->select->handle)->flag == 37))
				{
					if(movie4k_series(grid, listbox, countlabel, load, ((struct tithek*)listbox->select->handle)->link, ((struct tithek*)listbox->select->handle)->title) == 0)
						if(screenlistbox(grid, listbox, countlabel, title, titheklink, &pagecount, &tithekexit, &oaktpage, &oaktline, &ogridcol, 0, 0) == 0) break;
				}
				else if((((struct tithek*)listbox->select->handle)->flag == 39))
				{
					if(movie4k_series_listed(grid, listbox, countlabel, load, ((struct tithek*)listbox->select->handle)->link, ((struct tithek*)listbox->select->handle)->title) == 0)
						if(screenlistbox(grid, listbox, countlabel, title, titheklink, &pagecount, &tithekexit, &oaktpage, &oaktline, &ogridcol, 0, 0) == 0) break;
				}
				else if((((struct tithek*)listbox->select->handle)->flag == 40))
				{
					if(movie4k_hoster_series(grid, listbox, countlabel, load, ((struct tithek*)listbox->select->handle)->link, ((struct tithek*)listbox->select->handle)->title) == 0)
						if(screenlistbox(grid, listbox, countlabel, title, titheklink, &pagecount, &tithekexit, &oaktpage, &oaktline, &ogridcol, 0, 0) == 0) break;
				}
*/
/*
				else if(((struct tithek*)listbox->select->handle)->flag == 76)
				{
					if(amazon_search(grid, listbox, countlabel, load, ((struct tithek*)listbox->select->handle)->link, ((struct tithek*)listbox->select->handle)->title, NULL, 0) == 0)
						if(screenlistbox(grid, listbox, countlabel, title, titheklink, &pagecount, &tithekexit, &oaktpage, &oaktline, &ogridcol, 0, 0) == 0) break;
				}
				else if(((struct tithek*)listbox->select->handle)->flag == 77)
				{
					if(amazon_search(grid, listbox, countlabel, load, ((struct tithek*)listbox->select->handle)->link, ((struct tithek*)listbox->select->handle)->title, NULL, 1) == 0)
						if(screenlistbox(grid, listbox, countlabel, title, titheklink, &pagecount, &tithekexit, &oaktpage, &oaktline, &ogridcol, 0, 0) == 0) break;
				}
				else if(((struct tithek*)listbox->select->handle)->flag == 78)
				{
					if(amazon_search(grid, listbox, countlabel, load, ((struct tithek*)listbox->select->handle)->link, ((struct tithek*)listbox->select->handle)->title, NULL, 2) == 0)
						if(screenlistbox(grid, listbox, countlabel, title, titheklink, &pagecount, &tithekexit, &oaktpage, &oaktline, &ogridcol, 0, 0) == 0) break;
				}
				else if(((struct tithek*)listbox->select->handle)->flag == 79)
				{
					if(amazon_search(grid, listbox, countlabel, load, ((struct tithek*)listbox->select->handle)->link, ((struct tithek*)listbox->select->handle)->title, NULL, 3) == 0)
						if(screenlistbox(grid, listbox, countlabel, title, titheklink, &pagecount, &tithekexit, &oaktpage, &oaktline, &ogridcol, 0, 0) == 0) break;
				}
				else if(((struct tithek*)listbox->select->handle)->flag == 80)
				{
					if(amazon_search(grid, listbox, countlabel, load, ((struct tithek*)listbox->select->handle)->link, ((struct tithek*)listbox->select->handle)->title, NULL, 4) == 0)
						if(screenlistbox(grid, listbox, countlabel, title, titheklink, &pagecount, &tithekexit, &oaktpage, &oaktline, &ogridcol, 0, 0) == 0) break;
				}
				else if(((struct tithek*)listbox->select->handle)->flag == 81)
				{
					if(amazon_search(grid, listbox, countlabel, load, ((struct tithek*)listbox->select->handle)->link, ((struct tithek*)listbox->select->handle)->title, NULL, 5) == 0)
						if(screenlistbox(grid, listbox, countlabel, title, titheklink, &pagecount, &tithekexit, &oaktpage, &oaktline, &ogridcol, 0, 0) == 0) break;
				}
				else if(((struct tithek*)listbox->select->handle)->flag == 82)
				{
					if(amazon_search(grid, listbox, countlabel, load, ((struct tithek*)listbox->select->handle)->link, ((struct tithek*)listbox->select->handle)->title, NULL, 6) == 0)
						if(screenlistbox(grid, listbox, countlabel, title, titheklink, &pagecount, &tithekexit, &oaktpage, &oaktline, &ogridcol, 0, 0) == 0) break;
				}
				else if(((struct tithek*)listbox->select->handle)->flag == 83)
				{
					if(amazon_search(grid, listbox, countlabel, load, ((struct tithek*)listbox->select->handle)->link, ((struct tithek*)listbox->select->handle)->title, NULL, 7) == 0)
						if(screenlistbox(grid, listbox, countlabel, title, titheklink, &pagecount, &tithekexit, &oaktpage, &oaktline, &ogridcol, 0, 0) == 0) break;
				}
				else if(((struct tithek*)listbox->select->handle)->flag == 84)
				{
					if(amazon_search(grid, listbox, countlabel, load, ((struct tithek*)listbox->select->handle)->link, ((struct tithek*)listbox->select->handle)->title, NULL, 8) == 0)
						if(screenlistbox(grid, listbox, countlabel, title, titheklink, &pagecount, &tithekexit, &oaktpage, &oaktline, &ogridcol, 0, 0) == 0) break;
				}
				else if(((struct tithek*)listbox->select->handle)->flag == 85)
				{
					if(amazon_search(grid, listbox, countlabel, load, ((struct tithek*)listbox->select->handle)->link, ((struct tithek*)listbox->select->handle)->title, NULL, 9) == 0)
						if(screenlistbox(grid, listbox, countlabel, title, titheklink, &pagecount, &tithekexit, &oaktpage, &oaktline, &ogridcol, 0, 0) == 0) break;
				}
				else if(((struct tithek*)listbox->select->handle)->flag == 86)
				{
					if(amazon_search(grid, listbox, countlabel, load, ((struct tithek*)listbox->select->handle)->link, ((struct tithek*)listbox->select->handle)->title, NULL, 10) == 0)
						if(screenlistbox(grid, listbox, countlabel, title, titheklink, &pagecount, &tithekexit, &oaktpage, &oaktline, &ogridcol, 0, 0) == 0) break;
				}
				else if(((struct tithek*)listbox->select->handle)->flag == 87)
				{
					if(amazon_search(grid, listbox, countlabel, load, ((struct tithek*)listbox->select->handle)->link, ((struct tithek*)listbox->select->handle)->title, NULL, 11) == 0)
						if(screenlistbox(grid, listbox, countlabel, title, titheklink, &pagecount, &tithekexit, &oaktpage, &oaktline, &ogridcol, 0, 0) == 0) break;
				}
				else if(((struct tithek*)listbox->select->handle)->flag == 88)
				{
					if(amazon_search(grid, listbox, countlabel, load, ((struct tithek*)listbox->select->handle)->link, ((struct tithek*)listbox->select->handle)->title, NULL, 12) == 0)
						if(screenlistbox(grid, listbox, countlabel, title, titheklink, &pagecount, &tithekexit, &oaktpage, &oaktline, &ogridcol, 0, 0) == 0) break;
				}
				else if(((struct tithek*)listbox->select->handle)->flag == 89)
				{
					if(amazon_search(grid, listbox, countlabel, load, ((struct tithek*)listbox->select->handle)->link, ((struct tithek*)listbox->select->handle)->title, NULL, 13) == 0)
						if(screenlistbox(grid, listbox, countlabel, title, titheklink, &pagecount, &tithekexit, &oaktpage, &oaktline, &ogridcol, 0, 0) == 0) break;
				}
				else if(((struct tithek*)listbox->select->handle)->flag == 90)
				{
					if(amazon_search(grid, listbox, countlabel, load, ((struct tithek*)listbox->select->handle)->link, ((struct tithek*)listbox->select->handle)->title, NULL, 14) == 0)
						if(screenlistbox(grid, listbox, countlabel, title, titheklink, &pagecount, &tithekexit, &oaktpage, &oaktline, &ogridcol, 0, 0) == 0) break;
				}
*/
/*
				else if((((struct tithek*)listbox->select->handle)->flag == 95) || (((struct tithek*)listbox->select->handle)->flag == 96))
				{
					if(movie2k_hoster(grid, listbox, countlabel, load, ((struct tithek*)listbox->select->handle)->link, ((struct tithek*)listbox->select->handle)->title) == 0)
						if(screenlistbox(grid, listbox, countlabel, title, titheklink, &pagecount, &tithekexit, &oaktpage, &oaktline, &ogridcol, 0, 0) == 0) break;
				}
				else if(((struct tithek*)listbox->select->handle)->flag == 97)
				{
					if(movie2k_search_local(grid, listbox, countlabel, load, ((struct tithek*)listbox->select->handle)->link, ((struct tithek*)listbox->select->handle)->title, NULL, 0) == 0)
						if(screenlistbox(grid, listbox, countlabel, title, titheklink, &pagecount, &tithekexit, &oaktpage, &oaktline, &ogridcol, 0, 0) == 0) break;
				}
*/
				else if((((struct tithek*)listbox->select->handle)->flag == 66))
				{
					textbox(_("Message"), _("The hoster is not yet supported !"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 5, 0);
					continue;
				}
				else if((((struct tithek*)listbox->select->handle)->flag == 112))
				{
//					if(localparser_search(grid, listbox, countlabel, load, ((struct tithek*)listbox->select->handle)->link, ((struct tithek*)listbox->select->handle)->title) == 0)
//						if(screenlistbox(grid, listbox, countlabel, title, titheklink, &pagecount, &tithekexit, &oaktpage, &oaktline, &ogridcol, 0, 0) == 0) break;
					if(localparser_search(grid, listbox, countlabel, load, ((struct tithek*)listbox->select->handle)->link, ((struct tithek*)listbox->select->handle)->title, NULL, 0) == 0)
						if(screenlistbox(grid, listbox, countlabel, title, titheklink, &pagecount, &tithekexit, &oaktpage, &oaktline, &ogridcol, 0, 0) == 0) break;
				}
				else if((((struct tithek*)listbox->select->handle)->flag == 3))
				{
					int pincheck = 0;
					if(((struct tithek*)listbox->select->handle)->flag == 1000)
						pincheck = screenpincheck(0, NULL);
					if(pincheck == 0)
//						if(screenlistbox(grid, listbox, countlabel, title, titheklink, &pagecount, &tithekexit, &oaktpage, &oaktline, &ogridcol, 0, 3) == 0) break;
						if(screenlistbox(grid, listbox, countlabel, title, titheklink, &pagecount, &tithekexit, &oaktpage, &oaktline, &ogridcol, 0, ((struct tithek*)listbox->select->handle)->flag) == 0) break;
				}
				else
				{
					int pincheck = 0;
					if(((struct tithek*)listbox->select->handle)->flag == 1000)
						pincheck = screenpincheck(0, NULL);
					if(pincheck == 0)
//						if(screenlistbox(grid, listbox, countlabel, title, titheklink, &pagecount, &tithekexit, &oaktpage, &oaktline, &ogridcol, 0, 3) == 0) break;
						if(screenlistbox(grid, listbox, countlabel, title, titheklink, &pagecount, &tithekexit, &oaktpage, &oaktline, &ogridcol, 0, ((struct tithek*)listbox->select->handle)->flag) == 0) break;
				}
// new osd musst disable this
				drawscreen(grid, 0, 0);
			}
		}
		else if(rcret == getrcconfigint("rcinfo", NULL))
		{

			if(listbox->select != NULL && listbox->select->handle != NULL)
			{
//				if(showinfo(listbox, ((struct tithek*)listbox->select->handle)->title, ((struct tithek*)listbox->select->handle)->link, ((struct tithek*)listbox->select->handle)->pic, ((struct tithek*)listbox->select->handle)->localname, ((struct tithek*)listbox->select->handle)->menutitle, ((struct tithek*)listbox->select->handle)->description, ((struct tithek*)listbox->select->handle)->flag) == 0)
//					if(screenlistbox(grid, listbox, countlabel, title, titheklink, &pagecount, &tithekexit, &oaktpage, &oaktline, &ogridcol, 0, 0) == 0) break;

				int lastview = getconfigint("tithek_view", NULL);
				int lastcover = getconfigint("tithek_cover", NULL);
				addconfiginttmp("tithek_view", 7);
				addconfiginttmp("tithek_cover", 7);
				addconfiginttmp("tithek_description", 1);
				char* selected = ostrcat(listbox->select->text, NULL, 0, 0);

				pagecount = createtithekplay(titheklink, grid, listbox, countlabel, title, first + 1);

				if(pagecount == 0 || tithekexit == 1) break;

				addscreenrc(grid, listbox);
				setlistboxselection(listbox, selected);
				free(selected), selected = NULL;
					
				delconfigtmp("tithek_view");
				delconfigtmp("tithek_cover");
			}

		}
//		else if(rcret == getrcconfigint("rcyellow", NULL) && ostrcmp(title, _("Tithek - Mainmenu - Favoriten")) == 0)
		else if(rcret == getrcconfigint("rcyellow", NULL) && ostrstr(title, _("Tithek - Mainmenu - Favoriten")) != NULL)
		{
			if(listbox->select != NULL && listbox->select->handle != NULL)
			{
				if(textbox(_("Message"), _("Remove this Favorite ?"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 800, 200, 5, 0) == 1)
				{
					removefav(((struct tithek*)listbox->select->handle)->title, ((struct tithek*)listbox->select->handle)->link, ((struct tithek*)listbox->select->handle)->pic, ((struct tithek*)listbox->select->handle)->localname, ((struct tithek*)listbox->select->handle)->menutitle, ((struct tithek*)listbox->select->handle)->description, ((struct tithek*)listbox->select->handle)->flag);
//					removefav(((struct tithek*)listbox->select->handle)->title, ((struct tithek*)listbox->select->handle)->link, ((struct tithek*)listbox->select->handle)->pic, ((struct tithek*)listbox->select->handle)->localname, title, ((struct tithek*)listbox->select->handle)->flag);

					pagecount = createtithekplay(titheklink, grid, listbox, countlabel, title, 0);
					if(pagecount == 0) return;

//					drawscreen(grid, 0, 0);
				}
			}
		}
		else if(rcret == getrcconfigint("rcyellow", NULL))
		{
			if(listbox->select != NULL && listbox->select->handle != NULL)
			{
				if(status.repeat == 0)
					status.repeat = 1;
				else
					status.repeat = 0;
			}
		}
		else if(rcret == getrcconfigint("rcblue", NULL) && ostrstr(title, _("Tithek - Mainmenu - Favoriten")) != NULL)
		{
			if(listbox->select != NULL && listbox->select->handle != NULL)
			{
				if(status.repeat == 0)
					status.repeat = 1;
				else
					status.repeat = 0;
			}
		}
		else if(rcret == getrcconfigint("rcblue", NULL))
		{
			tithekexit = 1;
			break;
		}
//		else if(rcret == getrcconfigint("rcgreen", NULL) && ostrcmp(title, _("Tithek - Mainmenu - Favoriten")) != 0)
//		else if(rcret == getrcconfigint("rcgreen", NULL) && (ostrcmp(title, _("Tithek - Mainmenu - Favoriten")) == 0 || !ostrncmp("/", ((struct tithek*)listbox->select->handle)->link, 1)))
		else if(rcret == getrcconfigint("rcgreen", NULL) && ostrstr(title, _("Tithek - Mainmenu - Favoriten")) == NULL)
		{
			if(listbox->select != NULL && listbox->select->handle != NULL)
			{
				if(textbox(_("Message"), _("Add this link as Favorite ?"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 800, 200, 5, 0) == 1)
				{
//					addfav(((struct tithek*)listbox->select->handle)->title, ((struct tithek*)listbox->select->handle)->link, ((struct tithek*)listbox->select->handle)->pic, ((struct tithek*)listbox->select->handle)->localname, ((struct tithek*)listbox->select->handle)->menutitle, title, ((struct tithek*)listbox->select->handle)->flag);
					addfav(((struct tithek*)listbox->select->handle)->title, ((struct tithek*)listbox->select->handle)->link, ((struct tithek*)listbox->select->handle)->pic, ((struct tithek*)listbox->select->handle)->localname, title, ((struct tithek*)listbox->select->handle)->description, ((struct tithek*)listbox->select->handle)->flag);
				}
			}
		}
//		else if(rcret == getrcconfigint("rcgreen", NULL) && ostrcmp(title, _("Tithek - Mainmenu - Favoriten")) == 0)
//		else if(rcret == getrcconfigint("rcgreen", NULL) && (ostrcmp(title, _("Tithek - Mainmenu - Favoriten")) == 0 || !ostrncmp("/", ((struct tithek*)listbox->select->handle)->link, 1)))
		else if(rcret == getrcconfigint("rcgreen", NULL) && ostrstr(title, _("Tithek - Mainmenu - Favoriten")) != NULL)
		{
			if(listbox->select != NULL && listbox->select->handle != NULL)
			{
				if(textbox(_("Message"), _("Edit this Favorite ?"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 800, 200, 5, 0) == 1)
				{
//					editfav(((struct tithek*)listbox->select->handle)->title, ((struct tithek*)listbox->select->handle)->link, ((struct tithek*)listbox->select->handle)->pic, ((struct tithek*)listbox->select->handle)->localname, ((struct tithek*)listbox->select->handle)->menutitle, title, ((struct tithek*)listbox->select->handle)->flag);
					editfav(((struct tithek*)listbox->select->handle)->title, ((struct tithek*)listbox->select->handle)->link, ((struct tithek*)listbox->select->handle)->pic, ((struct tithek*)listbox->select->handle)->localname, ((struct tithek*)listbox->select->handle)->menutitle, ((struct tithek*)listbox->select->handle)->description, ((struct tithek*)listbox->select->handle)->flag);
					pagecount = createtithekplay(titheklink, grid, listbox, countlabel, title, 0);
					if(pagecount == 0) return;

//					drawscreen(grid, 0, 0);
				}
			}
		}

//		if(ostrcmp(title, _("Tithek - Mainmenu - Favoriten")) == 0)
		if(ostrstr(title, _("Tithek - Mainmenu - Favoriten")) != NULL)
		{
			changetext(b4, _("EDIT FAV"));
			changetext(b5, _("DEL FAV"));
			if(status.repeat == 0)
				changetext(b6, _("Repeat"));
			else
				changetext(b6, _("Repeat-On"));
//			b5->hidden = NO;
		}
		else
		{
			changetext(b4, _("ADD FAV"));
//			b5->hidden = YES;
			if(status.repeat == 0)
				changetext(b5, _("Repeat"));
			else
				changetext(b5, _("Repeat-On"));

			changetext(b6, _("Exit Tithek"));
		}



// new osd musst disable this
		drawscreen(grid, 0, 0);

	}

//	amazon_deinit();

	delmarkedscreennodes(grid, 1);
	delownerrc(grid);
	clearscreen(grid);
	status.repeat = 0;

	if(first == 1)
	{
		setvol(savevol);

		freetithek();
		delallfiles("/tmp/tithek", NULL);
		if(ytbgdownload == 0 && hlsbgdownload == 0)
			system("rm -rf /tmp/localhoster");
		system("rm -rf /tmp/localparser");
		system("rm -rf /tmp/localcache");
		system("rm -rf /tmp/parser");

		python = 0;

		if(status.mcaktiv == 0 && getconfigint("tithek_delservice", NULL) == 1)
			servicecheckret(servicestart(status.lastservice->channel, NULL, NULL, 0), 0);
	}
}

#endif
