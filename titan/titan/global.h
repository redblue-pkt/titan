#ifndef GLOBAL_H
#define GLOBAL_H

//flag 0: video+gui+freez
//flag 1: video+gui
//flag 2: video
//flag 3: gui 

void screenshoot(int flag)
{
	char* cmd = NULL, *file = NULL;
	if(flag == 0 || flag == 2)
	{
		videofreeze(status.aktservice->videodev);
		sleep(1);
	}

	if(flag == 0 || flag == 1)
		cmd = ostrcat("grab -j 100 -r 960", NULL, 0, 0);
	else if(flag == 2)
		cmd = ostrcat("grab -v -j 100 -r 960", NULL, 0, 0);
	else if(flag == 3)
		cmd = ostrcat("grab -o -j 100 -r 960", NULL, 0, 0);
		
	if(checkchipset("3798MV200") == 1)
		cmd = ostrcat(cmd, " > /tmp/screenshot.jpg", 1, 0);

	if(cmd != NULL)
	{
		unlink("/tmp/screenshot.jpg");
		system(cmd);
		printf("cmd1: %s\n", cmd);
		free(cmd), cmd = NULL;

		status.screenshot++;
		if(status.screenshot < 10)
			file = ostrcat("screenshot00", oitoa(status.screenshot), 0, 1);
		else if(status.screenshot < 100)
			file = ostrcat("screenshot0", oitoa(status.screenshot), 0, 1);
		else
			file = ostrcat("screenshot", oitoa(status.screenshot), 0, 1);
		file = ostrcat(file, ".jpg", 1, 0);

		cmd = ostrcat("mv -f /tmp/screenshot.jpg /tmp/", file, 0, 0);
		printf("cmd2: %s\n", cmd);
		system(cmd);

		free(cmd), cmd = NULL;
		cmd = ostrcat("ln -sf /tmp/", file, 0, 0);
		cmd = ostrcat(cmd, " /tmp/screenshot.jpg", 1, 0);
		printf("cmd3: %s\n", cmd);
		system(cmd);
	}
	if(flag == 0 || flag == 2)
		videocontinue(status.aktservice->videodev);

	free(file), file = NULL;
	free(cmd), cmd = NULL;
}

//flag 0: playerstart
//flag 1: playerstop
void set_player_sound(int flag)
{
#ifdef SH4
	char* vol = NULL, *cmd = NULL;

	if(flag == 0)
		vol = ostrcat(getconfig("vol_playerstart", NULL), NULL, 0, 0);
	else if(flag == 1)
		vol = ostrcat(getconfig("vol_playerstop", NULL), NULL, 0, 0);

	cmd = ostrcat("amixer -c 1 set Analog playback '", vol, 0, 0);
	if(status.mute == 1)
		cmd = ostrcat(cmd, "%' mute &", 1, 0);
	else
		cmd = ostrcat(cmd, "%' unmute &", 1, 0);
	system(cmd);
	free(cmd), cmd = NULL;

	cmd = ostrcat("amixer -c 1 set SPDIF playback '", vol, 0, 0);
	if(status.mute == 1)
		cmd = ostrcat(cmd, "%' mute &", 1, 0);
	else
		cmd = ostrcat(cmd, "%' unmute &", 1, 0);
	system(cmd);
	free(cmd), cmd = NULL;

	cmd = ostrcat("amixer -c 1 set HDMI playback '", vol, 0, 0);
	if(status.mute == 1)
		cmd = ostrcat(cmd, "%' mute &", 1, 0);
	else
		cmd = ostrcat(cmd, "%' unmute &", 1, 0);
	system(cmd);
	free(cmd), cmd = NULL;
	free(vol), vol = NULL;
#endif
}

//flag 0: with wait message
//flag 1: without wait message
void waitmsgbar(int sec, int exit, char* text, int flag)
{
	if(sec < 1) return;
	int maxsec = sec, rcret = -1;

	char* tmpstr = NULL;
	struct skin* waitmsgbar = getscreen("waitmsgbar");
	struct skin* load = getscreen("loading");

	waitmsgbar->progresssize = 0;
	if(text == NULL)
		tmpstr = ostrcat("0%", NULL, 0, 0);
	else
		tmpstr = ostrcat(text, " (0%)", 0, 0);
	changetext(waitmsgbar, tmpstr);
	free(tmpstr); tmpstr = NULL;

	if(flag == 0) drawscreen(load, 0, 0);

	while(sec > 0)
	{
		drawscreen(waitmsgbar, 0, 0);

		if(exit == 0)
			sleep(1);
		else
		{
			rcret = waitrc(0, 1000, 0);
			if(rcret == getrcconfigint("rcexit", NULL))
				break;

			if(rcret != RCTIMEOUT) continue;
		}

		sec--;
		waitmsgbar->progresssize = ((maxsec - sec) * 100) / maxsec;

		if(text == NULL)
			tmpstr = ostrcat(oitoa(waitmsgbar->progresssize), "%", 1, 0);
		else
		{
			tmpstr = ostrcat(text, " (", 0, 0);
			tmpstr = ostrcat(tmpstr, oitoa(waitmsgbar->progresssize), 1, 1);
			tmpstr = ostrcat(tmpstr, "%)", 1, 0);
		}
		changetext(waitmsgbar, tmpstr);
		free(tmpstr); tmpstr = NULL;
	}

	free(tmpstr); tmpstr = NULL;
	if(flag == 0) clearscreen(load);
	clearscreen(waitmsgbar);
	drawscreen(skin, 0, 0);
}

//flag 0: get pluginpath an add text
//flag 1: get pluginpath and change "%pluginpath%/"
char* createpluginpath(char* text, int flag)
{
	char* tmpstr = NULL;
	if(text == NULL) return NULL;

	if(flag == 0)
	{



/////////////////
	struct dirent **filelist;

	int count = 0;
	char *pluginpath = NULL;
	char *tmpstr = NULL;
	char *tmppath = NULL;

	//pluginpath
	tmppath = getconfig("pluginpath", NULL);

	if(tmppath == NULL)
		return NULL;

	count = scandir(tmppath, &filelist, 0, 0);
	if(count < 0)
	{
		perr("scandir1");
		return NULL;
	}

	while(count--)
	{
		//check if link is a dir
		if(filelist[count]->d_type == DT_LNK || filelist[count]->d_type == DT_UNKNOWN)
		{
			tmpstr = createpath(tmppath, filelist[count]->d_name);
			if(isdir(tmpstr) == 1)
				filelist[count]->d_type = DT_DIR;

			free(tmpstr); tmpstr = NULL;
		}

		if(filelist[count]->d_type == DT_DIR && ostrcmp(filelist[count]->d_name, ".") != 0 && ostrcmp(filelist[count]->d_name, "..") != 0)
		{
			tmppath = createpath(tmppath, filelist[count]->d_name);
            tmpstr = ostrcat(tmppath, text, 0, 0);
//printf("tmpstr: %s\n", tmpstr);
            if(file_exist(tmpstr) == 1)
            {
                printf("file exist tmpstr: %s\n", tmpstr);
                return tmpstr;
            }
		}
		free(filelist[count]);
	}
	free(filelist);

/////////////
//	  tmpstr = ostrcat(getconfig("pluginpath", NULL), text, 0, 0);
//	  if(file_exist(tmpstr) == 1) return tmpstr;

	  free(tmpstr); tmpstr = NULL;
	  tmpstr = ostrcat(getconfig("pluginpath1", NULL), text, 0, 0);
	  if(file_exist(tmpstr) == 1) return tmpstr;

	  free(tmpstr); tmpstr = NULL;
	  tmpstr = ostrcat(getconfig("pluginpath2", NULL), text, 0, 0);
	  if(file_exist(tmpstr) == 1) return tmpstr;
	}
	else
	{
		if(strlen(text) < 13) return NULL;

/////////////////
	struct dirent **filelist;

	int count = 0;
	char *pluginpath = NULL;
	char *tmpstr = NULL;
	char *tmppath = NULL;

	//pluginpath
	tmppath = getconfig("pluginpath", NULL);

	if(tmppath == NULL)
		return NULL;

	count = scandir(tmppath, &filelist, 0, 0);
	if(count < 0)
	{
		perr("scandir1");
		return NULL;
	}

	while(count--)
	{
		//check if link is a dir
		if(filelist[count]->d_type == DT_LNK || filelist[count]->d_type == DT_UNKNOWN)
		{
			tmpstr = createpath(tmppath, filelist[count]->d_name);
			if(isdir(tmpstr) == 1)
				filelist[count]->d_type = DT_DIR;

			free(tmpstr); tmpstr = NULL;
		}

		if(filelist[count]->d_type == DT_DIR && ostrcmp(filelist[count]->d_name, ".") != 0 && ostrcmp(filelist[count]->d_name, "..") != 0)
		{
			tmppath = createpath(tmppath, filelist[count]->d_name);
            tmpstr = ostrcat(tmppath, &text[12], 0, 0);
//printf("tmpstr2: %s\n", tmpstr);
            if(file_exist(tmpstr) == 1)
            {
                printf("file exist tmpstr2: %s\n", tmpstr);
                return tmpstr;
            }
		}
		free(filelist[count]);
	}
	free(filelist);

/////////////

//		tmpstr = ostrcat(getconfig("pluginpath", NULL), &text[12], 0, 0);
//		if(file_exist(tmpstr) == 1) return tmpstr;

		free(tmpstr); tmpstr = NULL;
		tmpstr = ostrcat(getconfig("pluginpath1", NULL), &text[12], 0, 0);
		if(file_exist(tmpstr) == 1) return tmpstr;

		free(tmpstr); tmpstr = NULL;
		tmpstr = ostrcat(getconfig("pluginpath2", NULL), &text[12], 0, 0);
		if(file_exist(tmpstr) == 1) return tmpstr;
	}

	free(tmpstr); tmpstr = NULL;
	return NULL;
}

int osystem(char* cmd, int timeout)
{
	int ret = 0;
	char* tmpstr = NULL;

//  busybox 1.31
	int irev = 0;
	char* brev = NULL;

	brev = string_newline(command("busybox | grep 'BusyBox v' | cut -d'.' -f2"));
printf("busybox version: %s\n", brev);
	if(brev != NULL) irev = atoi(brev);

	free(brev), brev = NULL;
printf("busybox version: %d\n", irev);

	if(irev >= 31)
	{
		tmpstr = ostrcat(tmpstr, "timeout -s 9 ", 1, 0);
		tmpstr = ostrcat(tmpstr, oitoa(timeout), 1, 1);
		tmpstr = ostrcat(tmpstr, " ", 1, 0);
	}
	else
	{
	tmpstr = ostrcat(tmpstr, "timeout -t ", 1, 0);
	tmpstr = ostrcat(tmpstr, oitoa(timeout), 1, 1);
	tmpstr = ostrcat(tmpstr, " -s 9 ", 1, 0);
	}

	tmpstr = ostrcat(tmpstr, cmd, 1, 0);
printf("cmd: %s\n", tmpstr);
	ret = system(tmpstr);
	free(tmpstr); tmpstr = NULL;

	return ret;
}

int wlanlinkquality()
{
	char dev[20];
	int tmp = 0, linkquality = 0;
	FILE *fd = NULL;
	char *fileline = NULL;

	struct inetwork* wlandev = getinetworkfirstwlan();
	if(wlandev == NULL) return 0;

	fileline = malloc(MINMALLOC);
	if(fileline == NULL)
	{
		err("no mem");
		return 0;
	}

	fd = fopen("/proc/net/wireless", "r");
	if(fd == NULL)
	{
		perr("can't open /proc/net/wireless");
		free(fileline);
		return 0;
	}

	while(fgets(fileline, MINMALLOC, fd) != NULL)
	{
		if(ostrstr(fileline, wlandev->device) != NULL)
			sscanf(fileline, "%[^:]: %d %d", dev, &tmp, &linkquality);
	}

	free(fileline);
	fclose(fd);
	return linkquality;
}

char* getispip()
{
	char* tmpstr = NULL;
	char* ip = NULL;
	tmpstr = gethttp("checkip.dyndns.org", "", 80, NULL, NULL, 10000, NULL, 0);

	if(oregex(".*<body>Current IP Address: ([0-9]{1,3}.[0-9]{1,3}.[0-9]{1,3}.[0-9]{1,3})</body>.*", tmpstr))
	{
		ip = oregex(".*<body>Current IP Address: ([0-9]{1,3}.[0-9]{1,3}.[0-9]{1,3}.[0-9]{1,3})</body>.*", tmpstr);
	}
	free(tmpstr), tmpstr = NULL;
	return ip;
}

//flag 0: leave standby
//flag 1: set standby
int setcecstandby(int flag)
{
	if(getconfigint("usecec", NULL) == 1 || getconfigint("cec_on", NULL) == 1)
	{
#ifndef MIPSEL
		if(flag == 0)
			writesys("/proc/stb/cec/onetouchplay", "0", 1);
		else
		{
			char* tmpstr = readsys("/proc/stb/cec/state_cecaddress", 1);
			char* tmpstr1 = readsys("/proc/stb/cec/state_activesource", 1);

			if(ostrcmp(tmpstr, tmpstr1) == 0)
				writesys("/proc/stb/cec/systemstandby", "0", 1);

			free(tmpstr); tmpstr = NULL;
			free(tmpstr1); tmpstr1 = NULL;
		}
#else
		if(flag == 0)
		{
			if(getconfigint("workpink", NULL) == 1)
				setvideomode(getconfig("576p", NULL), 0);
			cecwakeup();
			if(getconfigint("workpink", NULL) == 1)
			{
				sleep(3);
				setvideomode(getconfig("av_videomode", NULL), 0);
			}
		}
		else
		{
			cecstandby();
			sleep(1);
			cecstandby();
		}
#endif
	}

	return 0;
}

char* mask(char* input, int count, char* maskchar)
{
	char* tmpstr = NULL;
	int i = 0;

	tmpstr = ostrcat(input, NULL, 1, 0);

	if(tmpstr != NULL)
	{
		int len = strlen(tmpstr);
		for(i = 0; i < count - len; i++)
			tmpstr = ostrcat(maskchar, tmpstr, 0, 1);
	}
	else
	{
		for(i = 0; i < count; i++)
			tmpstr = ostrcat(maskchar, tmpstr, 0, 1);
	}

	return tmpstr;
}

int checkinternet()
{
	int skip = 0, i = 0;
	char* tmp = NULL, *cmd = NULL;

	cmd = ostrcat(cmd, "google.de", 1, 0);

	//if(system(cmd) != 0)
	for(i = 0; i < 3; i++)
	{
		free(tmp); tmp = NULL;
		tmp = gethttp(cmd, "/", 80, NULL, NULL, 5000, NULL, 0);
		if(tmp != NULL) break;
	}
	if(tmp == NULL)
		skip = 1;

	free(tmp); tmp = NULL;
	free(cmd), cmd = NULL;

	return skip;
}

int checkpng(char* filename)
{
	FILE* fd = NULL;
	int ret = 0;
	png_bytep sig = NULL;

	fd = fopen(filename, "rb");
	if(fd == NULL)
	{
		perr("open file %s", filename);
		return 0;
	}

	sig = malloc(8);
	if(sig == NULL)
	{
		err("no mem");
		fclose(fd);
		return 0;
	}

	fread(sig, 1, 8, fd);
	ret = png_check_sig(sig, 8);
	if(ret == 0)
	{
		free(sig);
		fclose(fd);
		return 0;
	}

	free(sig);
	fclose(fd);
	return 1;
}

char **str_split(char *string, char *delim)
{
	char **tokens = NULL;
	char *working = NULL;
	char *token = NULL;
	int idx = 0;

	if(string == NULL) return NULL;

	tokens = malloc(sizeof(char*) * MAXTOKENS);
	if(tokens == NULL)
		return NULL;

	working = malloc(sizeof(char) * strlen(string) + 1);
	if(working == NULL)
		return NULL;

	 /* to make sure, copy string to a safe place */
	strcpy(working, string);
	for(idx = 0; idx < MAXTOKENS; idx++)
		tokens[idx] = NULL;

	token = strtok(working, delim);
	idx = 0;

	/* always keep the last entry NULL termindated */
	while(idx < (MAXTOKENS - 1) && token != NULL)
	{
		tokens[idx] = malloc(sizeof(char) * strlen(token) + 1);
		if(tokens[idx] != NULL)
		{
			strcpy(tokens[idx], token);
			idx++;
			token = strtok(NULL, delim);
		}
	}

	free(working);
	return tokens;
}

char* readfromlinetoline(char* str, int start, int end, int flag)
{
	if(str == NULL) return str;

	char* tmpstr = NULL;
	int count = 0;
	int i = 0;
	struct splitstr* ret = NULL;
	ret = strsplit(str, "\n", &count);

	for(i = 0; i < count; i++)
	{
		if(i >= start && i <= end)
		{
			tmpstr = ostrcat(tmpstr, (&ret[i])->part, 1, 0);
			tmpstr = ostrcat(tmpstr, "\n", 1, 0);
		}
	}
	free(ret), ret = NULL;

	if(flag == 1)
	{
		free(str), str = NULL;
	}
	return tmpstr;
}

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

	return ostrshrink(str);
}

char* unhexlify(char *hexstr)
{
	int len = 0, tmpint = 0;
	char *p, *q, *binstr = NULL;

	if(hexstr == NULL) return NULL;

	len = strlen(hexstr);
	if(len == 0 || len % 2 != 0) return NULL;

	binstr = calloc(1, (len / 2) + 1);
	if(binstr == NULL)
	{
		err("no mem");
		return NULL;
	}

	for(p = hexstr, q = binstr; *p; p += 2, q++)
	{
		sscanf(p, "%2x", &tmpint);
		*q = tmpint;
	}

	return binstr;
}

int getsupermagic(char* filename)
{
	struct statfs64 s;

	if(statfs64(filename, &s) >= 0)
		return s.f_type;

	return 0;
}

int getrandomnum(int max)
{
	srand(time(NULL));
	return (rand() % max);
}

char* delmountpart(char* filename, int free1)
{
	struct mntent ent;
	FILE *fd = NULL;
	char* ret = NULL, *buf = NULL, *treffer = NULL, *rpath = NULL;
	int mntdirlen = 0;

	if(filename == NULL) return NULL;

	buf = malloc(MINMALLOC);
	if(buf == NULL)
	{
		err("no mem");
		return NULL;
	}

	rpath = realpath(filename, NULL);
	if(rpath == NULL) return NULL;

	fd = setmntent("/proc/mounts", "r");
	if(fd != NULL)
	{
		while(getmntent_r(fd, &ent, buf, MINMALLOC) != 0)
		{
			if(ent.mnt_dir != NULL)
			{
				mntdirlen = strlen(ent.mnt_dir);
				if(mntdirlen > 1 && ostrstr(rpath, ent.mnt_dir) == rpath)
				{
					if(treffer == NULL || strlen(treffer) < mntdirlen)
					{
						free(treffer); treffer = NULL;
						treffer = ostrcat(ent.mnt_dir, NULL, 0, 0);
					}
				}
			}
		}
	}

	if(treffer != NULL)
		ret = string_replace(treffer, NULL, rpath, 0);

	endmntent(fd);
	if(free1 == 1) free(filename);
	free(buf);
	free(treffer);
	free(rpath);
	return ret;
}

char* addmountpart(char* filename, int free1)
{
	struct mntent ent;
	FILE *fd = NULL;
	char* ret = NULL, *buf = NULL;

	if(filename == NULL) return NULL;

	buf = malloc(MINMALLOC);
	if(buf == NULL)
	{
		err("no mem");
		return NULL;
	}

	fd = setmntent("/proc/mounts", "r");
	if(fd != NULL)
	{
		while(getmntent_r(fd, &ent, buf, MINMALLOC))
		{
			if(ent.mnt_dir != NULL && strlen(ent.mnt_dir) > 1)
			{
				ret = ostrcat(ent.mnt_dir, filename, 0, 0);
				if(file_exist(ret))
				{
					if(free1 == 1) free(filename);
					endmntent(fd);
					free(buf);
					return ret;
				}
				free(ret); ret = NULL;
			}
		}
	}

	endmntent(fd);
	free(buf);
	return ret;
}

unsigned int gethash(char* str)
{
	unsigned int hash = 0;
	int c = 0;

	if(str == NULL) return 0;

	while((c = *str++))
		hash = ((hash << 5) + hash) ^ c;

	return hash;
}

void freeregexstruct(struct regex* node)
{
	if(node == NULL) return;

	free(node->match1); node->match1 = NULL;
	free(node->match2); node->match2 = NULL;
	free(node->match3); node->match3 = NULL;
	free(node->match4); node->match4 = NULL;
	free(node->match5); node->match5 = NULL;
	free(node->match6); node->match6 = NULL;
	free(node->match7); node->match7 = NULL;
	free(node->match8); node->match8 = NULL;
	free(node->match9); node->match9 = NULL;
	free(node->match10); node->match10 = NULL;

	free(node); node = NULL;
}

struct regex* regexstruct(char* regex, char* str)
{
	regex_t preg = {NULL};
	regmatch_t pmatch[11] = {{0}};
	size_t rm = 0, i = 0, len = 0;
	char* ret = NULL;
	struct regex* node = NULL;

	if(regex == NULL || str == NULL) return NULL;

	rm = regcomp(&preg, regex, REG_EXTENDED);
	if(rm != 0) //error in regex
	{
		regfree(&preg);
		return NULL;
	}

	rm = regexec(&preg, str, 11, pmatch, 0);
	if(rm != 0) //no match
	{
		regfree(&preg);
		return NULL;
	}

	node = (struct regex*)calloc(1, sizeof(struct regex));
	if(node == NULL)
	{
		err("no mem");
		regfree(&preg);
		return NULL;
	}

	for(i = 1; !rm && i <= preg.re_nsub; i++)
	{
		len = pmatch[i].rm_eo - pmatch[i].rm_so;
		if(len < 1) continue;

		ret = malloc(len + 1);
		if(ret != NULL)
		{
			memcpy(ret, str + pmatch[i].rm_so, len);
			ret[len] = '\0';
		}

		if(i == 1) node->match1 = ret;
		else if(i == 2) node->match2 = ret;
		else if(i == 3) node->match3 = ret;
		else if(i == 4) node->match4 = ret;
		else if(i == 5) node->match5 = ret;
		else if(i == 6) node->match6 = ret;
		else if(i == 7) node->match7 = ret;
		else if(i == 8) node->match8 = ret;
		else if(i == 9) node->match9 = ret;
		else if(i == 10) node->match10 = ret;

		ret = NULL;
	}

	regfree(&preg);
	return node;
}

char* oregex(char* regex, char* str)
{
	regex_t preg = {NULL};
	regmatch_t pmatch[2] = {{0}};
	size_t rm = 0, len = 0;
	char* ret = NULL;

	if(regex == NULL || str == NULL) return NULL;

	rm = regcomp(&preg, regex, REG_EXTENDED);
	if(rm != 0) //error in regex
	{
		regfree(&preg);
		return NULL;
	}

	rm = regexec(&preg, str, 2, pmatch, 0);
	if(rm != 0) //no match
	{
		regfree(&preg);
		return NULL;
	}

	len = pmatch[1].rm_eo - pmatch[1].rm_so;
	if(len < 1)
	{
		regfree(&preg);
		return NULL;
	}

	ret = malloc(len + 1);
	if(ret != NULL)
	{
		memcpy(ret, str + pmatch[1].rm_so, len);
		ret[len] = '\0';
	}

	regfree(&preg);
	return ret;
}

int oregexint(char* regex, char* str)
{
	int iret = 0;
	char* ret = NULL;

	ret = oregex(regex, str);
	if(ret != NULL) iret = atoi(ret);
	free(ret);

	return iret;
}

struct splitstr* oregexsplit(char* regex, char *str, char *tok, int* count)
{
	char *tmpstr = NULL;
	struct splitstr *array = NULL, *tmparray = NULL;
	*count = 0;

	if(str == NULL || tok == NULL)
		return NULL;

	tmpstr = strtok(str, tok);
	while(tmpstr != NULL)
	{
		*count = *count + 1;
		tmparray = array; array = (struct splitstr*)realloc(array, sizeof(struct splitstr*) * (*count));
		if(array == NULL)
		{
			err("no mem");
			free(tmparray);
			return NULL;
		}

		(&array[(*count) - 1])->part = oregex(regex, tmpstr);
		tmpstr = strtok(NULL, tok);
	}

	return array;
}

void freeoregexsplit(struct splitstr* tmparray, int len)
{
	if(tmparray == NULL) return;
	int i = 0;

	for(i = 0; i < len; i++)
	{
		free(tmparray[i].part);
		tmparray[i].part = NULL;
	}

	free(tmparray);
}

int getmaxsat(char* feshortname)
{
	char *tmpstr = NULL;
	int maxsat = 1;

	if(feshortname != NULL)
	{
		tmpstr = ostrcat(feshortname, "_maxsat", 0, 0);
		maxsat = getconfigint(tmpstr, NULL);
		free(tmpstr); tmpstr = NULL;
	}
	if(maxsat < 0) maxsat = 1;

	return maxsat;
}

void autochangechannelname()
{
	uint8_t lastsecnr = 0xff;
	int secnr = 0;
	unsigned char* buf = NULL;

	if(status.aktservice->fedev == NULL) return;

	while(secnr <= lastsecnr && secnr <= 256)
	{
		buf = dvbgetsdt(status.aktservice->fedev, secnr, 2000000);
		if(buf != NULL)
			findchannel(status.aktservice->fedev, NULL, buf, &lastsecnr, NULL, NULL, 1, 1);
		else
			break;
		free(buf); buf = NULL;
		secnr++;
	}
}

int isbase64(char c)
{
	if((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9') || c == '+' || c == '/' || c == '=')
		return 1;
	return 0;
}

char decodebase64(char c)
{
	if(c >= 'A' && c <= 'Z') return(c - 'A');
	if(c >= 'a' && c <= 'z') return(c - 'a' + 26);
	if(c >= '0' && c <= '9') return(c - '0' + 52);
	if(c == '+') return(62);
	return 63;
}

int b64dec(char* dest, char* src)
{
	if(src && *src)
	{
		char* p = dest;
		int k, l = strlen(src) + 1;
		char* buf = NULL;

		buf = malloc(l);
		if(buf == NULL) return 0;

		for(k = 0, l = 0; src[k]; k++)
		{
			if(isbase64(src[k]))
				buf[l++] = src[k];
		}

		for(k = 0; k < l; k += 4)
		{
			char c1 = 'A', c2 = 'A', c3 = 'A', c4 = 'A';
			char b1 = 0, b2 = 0, b3 = 0, b4 = 0;

			c1 = buf[k];
			if(k + 1 < l)
				c2 = buf[k + 1];
			if(k + 2 < l)
				c3 = buf[k + 2];
			if(k + 3 < l)
				c4 = buf[k + 3];

			b1 = decodebase64(c1);
			b2 = decodebase64(c2);
			b3 = decodebase64(c3);
			b4 = decodebase64(c4);

			*p++ = ((b1 << 2) | (b2 >> 4));

			if(c3 != '=')
				*p++ = (((b2 & 0xf) << 4) | (b3 >> 2));
			if(c4 != '=')
				*p++ = (((b3 & 0x3) << 6) | b4);

			// strip last unchanged chars
			if(k == l-4)
				*p++ = '\0';

		}

		free(buf);
		return(p - dest);
	}
	return 0;
}

void changechannellist(struct channel* chnode, char* channellist)
{
		char* tmpstr = NULL;

		if(chnode == NULL || status.aktservice->channel == NULL) return;

		if(chnode->servicetype != 99) status.servicetype = chnode->servicetype;
		if(status.servicetype == 0)
		{
			if(channellist != NULL)
			{
				tmpstr = ostrcat(channellist, NULL, 0, 0);
				free(status.aktservice->channellist);
				status.aktservice->channellist = tmpstr;
				addconfig("channellist", tmpstr);
			}
			tmpstr = oitoa(status.aktservice->channel->serviceid);
			addconfig("serviceid", tmpstr);
			free(tmpstr); tmpstr = NULL;
			tmpstr = ollutoa(status.aktservice->channel->transponderid);
			addconfig("transponderid", tmpstr);
			free(tmpstr); tmpstr = NULL;
		}
		else
		{
			if(channellist != NULL)
			{
				tmpstr = ostrcat(channellist, NULL, 0, 0);
				free(status.aktservice->channellist);
				status.aktservice->channellist = tmpstr;
				addconfig("rchannellist", tmpstr);
			}
			tmpstr = oitoa(status.aktservice->channel->serviceid);
			addconfig("rserviceid", tmpstr);
			free(tmpstr); tmpstr = NULL;
			tmpstr = ollutoa(status.aktservice->channel->transponderid);
			addconfig("rtransponderid", tmpstr);
			free(tmpstr); tmpstr = NULL;
		}
}

//flag 0: normal picons
//flag 1: alternate picons
char* createpiconpath(struct channel* chnode, int flag)
{
	char* tmpstr = NULL, *picon = NULL;

	if(chnode != NULL)
	{
		picon = ostrcat("/", oitoa(chnode->serviceid), 0, 1);
		picon = ostrcat(picon, "-", 1, 0);
		picon = ostrcat(picon, ollutoa(chnode->transponderid), 1, 1);
		picon = ostrcat(picon, ".png", 1, 0);

		if(flag == 1)
			tmpstr = ostrcat("/mnt/swapextensions/usr/local/share/titan/picons/alternate", picon, 0, 0);
		else
			tmpstr = ostrcat("/mnt/swapextensions/usr/local/share/titan/picons", picon, 0, 0);
		if(isfile(tmpstr))
		{
			free(picon);
			return tmpstr;
		}

		free(tmpstr); tmpstr = NULL;
		if(flag == 1)
			tmpstr = ostrcat("/var/swap/usr/local/share/titan/picons/alternate", picon, 0, 0);
		else
			tmpstr = ostrcat("/var/swap/usr/local/share/titan/picons", picon, 0, 0);
		if(isfile(tmpstr))
		{
			free(picon);
			return tmpstr;
		}

		free(tmpstr); tmpstr = NULL;
		if(flag == 1)
			tmpstr = ostrcat("/var/usr/local/share/titan/picons/alternate", picon, 0, 0);
		else
			tmpstr = ostrcat("/var/usr/local/share/titan/picons", picon, 0, 0);
		if(isfile(tmpstr))
		{
			free(picon);
			return tmpstr;
		}
	}

	free(tmpstr); tmpstr = NULL;
	tmpstr = ostrcat(tmpstr, getconfig("defskinpath", NULL), 1, 0);
	tmpstr = ostrcat(tmpstr, "/skin/", 1, 0);
	tmpstr = ostrcat(tmpstr, "defpicon.png", 1, 0);

	free(picon);
	return tmpstr;
}

int checkskin()
{
	int ret = 0;
	char* defskinpath = NULL;
	char* defskinconfig = NULL;
	char* defskinfile = NULL;

	if(file_exist(getconfig("skinpath", NULL)) == 0)
		ret = 1;
	else if(file_exist(getconfig("skinconfig", NULL)) == 0)
		ret = 2;
	else if(file_exist(getconfig("skinfile", NULL)) == 0)
		ret = 3;

	if(ret != 0)
	{
		err("skin not found code=%d, change to default", ret);

		defskinpath = getconfig("defskinpath", NULL);
		if(defskinpath != NULL) addconfig("skinpath", defskinpath);
		defskinconfig = getconfig("defskinconfig", NULL);
		if(defskinconfig != NULL) addconfig("skinconfig", defskinconfig);
		defskinfile = getconfig("defskinfile", NULL);
		if(defskinfile != NULL) addconfig("skinfile", defskinfile);

		ret = 0;
		if(file_exist(getconfig("skinpath", NULL)) == 0)
			ret = 1;
		else if(file_exist(getconfig("skinconfig", NULL)) == 0)
			ret = 2;
		else if(file_exist(getconfig("skinfile", NULL)) == 0)
			ret = 3;
	}

	if(ret != 0)
	{
		err("default skin not found code=%d", ret);
	}

	return ret;
}

int getaktvideosize()
{
	//wait 3 sek after zap, befor videsize is valid
	//driver takes a little time
	if(status.videosizevalid == 1)
		return 0;
	else if(status.videosizevalid + 3 > time(NULL))
		return 1;
	else
	{
#ifndef SIMULATE
		if(videoreadqwidth(status.aktservice->videodev) == 0)
#endif
		{
			status.videosizevalid = 1;
			return 0;
		}
	}

	return 1;
}

int ostrftime(char* buf, int count, char* format, struct tm* t)
{
	int ret = -1;

	if(buf == NULL || t == NULL) return ret;

	if(format != NULL)
		ret = strftime(buf, count, format, t);
	else
		ret = strftime(buf, count, "%d-%m-%Y %H:%M", t);

	return ret;
}

//flag 0 = write all
//flag 1 = don't write epg
//flag 2 = only write epg
//flag 3 = only write config
int writeallconfig(int flag)
{
	char* tmpstr = NULL;
	int ret = 0;

	if(flag == 0 || flag == 1)
	{
		if(status.writerectimer == 1)
			if(writerectimer(getconfig("rectimerfile", NULL), 0) != 0)
				ret = 1;
		if(status.writesat == 1)
			if(writesat(getconfig("satfile", NULL)) != 0)
				ret = 1;
		if(status.writeplaylist == 1)
			if(writeallplaylist() != 0)
				ret = 1;
		if(status.writemainplaylist == 1)
			if(writemainplaylist(getconfig("playlistfile", NULL)) != 0)
				ret = 1;
		if(status.writebouquet == 1)
			if(writeallbouquet() != 0)
				ret = 1;
		if(status.writemainbouquet == 1)
			if(writemainbouquet(getconfig("bouquetfile", NULL)) != 0)
				ret = 1;
		if(status.writechannel == 1)
			if(writechannel(getconfig("channelfile", NULL)) != 0)
				ret = 1;
		if(status.writetransponder == 1)
			if(writetransponder(getconfig("transponderfile", NULL)) != 0)
				ret = 1;
		if(status.writeprovider == 1)
			if(writeprovider(getconfig("providerfile", NULL)) != 0)
				ret = 1;
		if(status.writeownconfig == 1)
			if(writeownconfig(getconfig("ownconfig", NULL)) != 0)
				ret = 1;
		if(status.writeepgscanlist == 1)
			if(writeepgscanlist(getconfig("epgchannelfile", NULL)) != 0)
				ret = 1;
		if(status.writercconfig == 1)
			if(writercconfig(getconfig("rcconfig", NULL)) != 0)
				ret = 1;
		if(status.writeskinconfig == 1)
			if(writeskinconfig(getconfig("skinconfig", NULL)) != 0)
				ret = 1;
		if(status.writemostzap == 1)
			if(writemostzap(getconfig("mostzapfile", NULL)) != 0)
				ret = 1;
		if(status.writelastsubtitle == 1)
			if(writelastsubtitle(getconfig("lastsubtitle", NULL)) != 0)
				ret = 1;
		if(status.writechannelslot == 1)
			if(writechannelslot(getconfig("channelslotfile", NULL)) != 0)
				ret = 1;
	}
	if(flag == 0 || flag == 1 || flag == 3)
	{
		if(status.writeconfig == 1)
		{
			addconfigint("vol", getvol());
			if(writeconfig(status.configfile) != 0)
				ret = 1;
		}
	}
	if((flag == 0 || flag == 2) && time(NULL) > 1072224000) // 01.01.2004
	{
		tmpstr = createpath(getconfig("epg_path", NULL), "epg.dat");
		if(writeepg(tmpstr) != 0)
			ret = 1;
		free(tmpstr); tmpstr = NULL;
	}

	sync();

	return ret;
}

void* writeallconfigthread0()
{
	writeallconfig(0);
	pthread_exit(NULL);
}

void* writeallconfigthread1()
{
	writeallconfig(1);
	pthread_exit(NULL);
}

void initmutex(int flag)
{
	if(flag == 1)
	{
		pthread_mutex_init(&status.drawingmutex, NULL);
		pthread_mutex_init(&status.rectimermutex, NULL);
		pthread_mutex_init(&status.servicemutex, NULL);
		pthread_mutex_init(&status.epgmutex, NULL);
		pthread_mutex_init(&status.vfdmutex, NULL);
		pthread_mutex_init(&status.channelmutex, NULL);
		pthread_mutex_init(&status.timerthreadmutex, NULL);
		pthread_mutex_init(&status.audiotrackmutex, NULL);
		pthread_mutex_init(&status.subtitlemutex, NULL);
		pthread_mutex_init(&status.dmxdevmutex, NULL);
		pthread_mutex_init(&status.rcmutex, NULL);
		pthread_mutex_init(&status.queuemutex, NULL);
		pthread_mutex_init(&status.clistmutex, NULL);
		pthread_mutex_init(&status.hddmutex, NULL);
		pthread_mutex_init(&status.linkedchannelmutex, NULL);
		pthread_mutex_init(&status.tsseekmutex, NULL);
		pthread_mutex_init(&status.accelfbmutex, NULL);
		pthread_mutex_init(&status.mediadbmutex, NULL);
		pthread_mutex_init(&status.oldentrymutex, NULL);
		pthread_mutex_init(&status.newslettermutex, NULL);
		pthread_mutex_init(&status.tithekmutex, NULL);
		pthread_mutex_init(&status.inetworkmutex, NULL);
		pthread_mutex_init(&status.textboxmutex, NULL);
		pthread_mutex_init(&status.setaktresmutex, NULL);
		pthread_mutex_init(&status.waitrcmutex, NULL);
		pthread_mutex_init(&status.oledmutex, NULL);
		pthread_mutex_init(&status.cecmutex, NULL);
	}
	else
	{
		pthread_mutex_destroy(&status.drawingmutex);
		pthread_mutex_destroy(&status.rectimermutex);
		pthread_mutex_destroy(&status.servicemutex);
		pthread_mutex_destroy(&status.epgmutex);
		pthread_mutex_destroy(&status.vfdmutex);
		pthread_mutex_destroy(&status.channelmutex);
		pthread_mutex_destroy(&status.timerthreadmutex);
		pthread_mutex_destroy(&status.audiotrackmutex);
		pthread_mutex_destroy(&status.subtitlemutex);
		pthread_mutex_destroy(&status.dmxdevmutex);
		pthread_mutex_destroy(&status.rcmutex);
		pthread_mutex_destroy(&status.queuemutex);
		pthread_mutex_destroy(&status.clistmutex);
		pthread_mutex_destroy(&status.hddmutex);
		pthread_mutex_destroy(&status.linkedchannelmutex);
		pthread_mutex_destroy(&status.tsseekmutex);
		pthread_mutex_destroy(&status.accelfbmutex);
		pthread_mutex_destroy(&status.mediadbmutex);
		pthread_mutex_destroy(&status.oldentrymutex);
		pthread_mutex_destroy(&status.newslettermutex);
		pthread_mutex_destroy(&status.tithekmutex);
		pthread_mutex_destroy(&status.inetworkmutex);
		pthread_mutex_destroy(&status.textboxmutex);
		pthread_mutex_destroy(&status.setaktresmutex);
		pthread_mutex_destroy(&status.waitrcmutex);
		pthread_mutex_destroy(&status.oledmutex);
		pthread_mutex_destroy(&status.cecmutex);
	}
}

int hexit(char c)
{
	if(c >= '0' && c <= '9')
		return c - '0';
	if(c >= 'a' && c <= 'f')
		return c - 'a' + 10;
	if(c >= 'A' && c <= 'F')
		return c - 'A' + 10;

	return 0;
}

char* htmlencode(char* from)
{
	int buflen = 0;
	char* buf = NULL, *to = NULL;

	buf = calloc(1, MINMALLOC);
	if(buf == NULL)
	{
		err("no mem");
		return NULL;
	}
	to = buf;

	for(buflen = 0; *from != '\0' && buflen < MINMALLOC; ++from)
	{
		if(isalnum(*from) || strchr("/_.-~", *from) != NULL)
		{
			*to = *from;
			++to;
			++buflen;
		}
		else
		{
			sprintf(to, "%%%02x", (int)*from & 0xff);
			to += 3;
			buflen += 3;
		}
	}
	*to = '\0';

	buf = ostrshrink(buf);
	return buf;
}

void htmldecode(char* to, char* from)
{
	if(to == NULL || from == NULL) return;

	for(; *from != '\0'; ++to, ++from)
	{
		if(from[0] == '%' && isxdigit(from[1]) && isxdigit(from[2]))
		{
			*to = hexit(from[1]) * 16 + hexit(from[2]);
			from += 2;
		}
		else
			*to = *from;
	}
	*to = '\0';
}

void htmldecode2(char* to, char* from)
{
	if(to == NULL || from == NULL) return;

	for(; *from != '\0'; ++to, ++from)
	{
		if(from[0] == '%' && isxdigit(from[1]) && isxdigit(from[2]) && isxdigit(from[3]) && isxdigit(from[4]))
		{
			*to = hexit(from[1]) * 16 + hexit(from[2]) * 16 + hexit(from[3]) * 16 + hexit(from[4]);
			from += 4;
		}
		else if(from[0] == '%' && isxdigit(from[1]) && isxdigit(from[2]) && isxdigit(from[3]))
		{
			*to = hexit(from[1]) * 16 + hexit(from[2]) * 16 + hexit(from[3]);
			from += 3;
		}
		else if(from[0] == '%' && isxdigit(from[1]) && isxdigit(from[2]))
		{
			*to = hexit(from[1]) * 16 + hexit(from[2]);
			from += 2;
		}
		else
			*to = *from;
	}
	*to = '\0';
}

void htmldecode3(char* to, char* from)
{
	if(to == NULL || from == NULL) return;

	for(; *from != '\0'; ++to, ++from)
	{
		if(from[0] == '\\' && from[1] == 'u' && isxdigit(from[2]) && isxdigit(from[3]) && isxdigit(from[4]) && isxdigit(from[5]) && isxdigit(from[6]) && isxdigit(from[7]))
		{
			*to = hexit(from[2]) * 1048576 + hexit(from[3]) * 65536 + hexit(from[4]) * 4096 + hexit(from[5]) * 256 + hexit(from[6]) * 16 + hexit(from[7]);
			from += 7;
		}
		if(from[0] == '\\' && from[1] == 'u' && isxdigit(from[2]) && isxdigit(from[3]) && isxdigit(from[4]) && isxdigit(from[5]))
		{
			*to = hexit(from[2]) * 4096 + hexit(from[3]) * 256 + hexit(from[4]) * 16 + hexit(from[5]);
			from += 5;
		}
		else
			*to = *from;
	}
	*to = '\0';
}

void setosdtransparent(int value)
{
	int ret = 255;
	if(value < 0) value = 0;
	if(value > 100) value = 100;

	ret -= value * 2.55;

	setfbtransparent(ret);
}

struct tm* olocaltime(time_t *value)
{
	struct tm *loctime, *loctime1 = NULL;

	loctime1 = (struct tm*)malloc(sizeof(struct tm));
	if(loctime1 == NULL)
	{
		err("no mem");
		return NULL;
	}
	loctime = localtime_r(value, loctime1);

	return loctime;
}

int setwakeuptimer(time_t waketime)
{
	char* wakeuptimerprog = NULL;
	struct tm *loctime = NULL;
	char *buf = NULL;
	int ret = -1;

	wakeuptimerprog = getconfig("wakeuptimerprog", NULL);

	if(wakeuptimerprog == NULL)
	{
		err("NULL detect");
		return ret;
	}

	buf = malloc(MINMALLOC);
	if(buf == NULL)
	{
		err("no mem");
		return ret;
	}

	loctime = olocaltime(&waketime);

	if(loctime != NULL)
		ostrftime(buf, MINMALLOC, wakeuptimerprog, loctime);

	if(buf != NULL)
	{
		debug(10, "set wake up timer (%s)", buf);
		ret = system(buf);
	}

	free(loctime);
	free(buf);
	return ret;
}

int autoresolution()
{
	char *hotpluginfo = NULL;
	char *value = NULL;

	if(getconfig("av_videomode", NULL) != NULL || getconfig("av_colorformat", NULL))
		return 1;

	hotpluginfo = getconfig("hotpluginfo", NULL);

	if(hotpluginfo == NULL)
	{
		err("NULL detect");
		return 1;
	}

	value = readsys(hotpluginfo, 1);
	if(value == NULL)
	{
		err("NULL detect");
		return 1;
	}

	if(value[0] == 'y')
	{
		setvideomode("720p50", 0);
		changefbresolution("720p50", 0);
		setcolorformat("hdmi_rgb", 0);
	}
	else
	{
		setvideomode("576i50", 0);
		changefbresolution("576i50", 0);
		setcolorformat("rgb", 1);
	}

	free(value);
	return 0;
}

//TODO: can remove if new function work
/*
int writelengthfield(unsigned char *buf, unsigned int len)
{
	if(len < 127)
	{
		*buf++ = len;
		return 1;
	}
	else if(len < 256)
	{
		*buf++ = 0x81;
		*buf++ = len;
		return 2;
	}
	else if(len < 65535)
	{
		*buf++ = 0x82;
		*buf++ = len >> 8;
		*buf++ = len;
		return 3;
	}

	return 0;
}
*/

size_t writelengthfield(unsigned char * buf, unsigned int len)
{
	if(buf == NULL) return 0;

	if(len < 128)
	{
		buf[0] = len;
		return 1;
	}
	else
	{
		unsigned int pos = 0;
		unsigned int shiftby = 8;
		unsigned char lenfieldsize = 1;

		while((len >> shiftby) != 0)
		{
			lenfieldsize++;
			shiftby += 8;
		}

		buf[pos++] = ((1 << 7) | lenfieldsize);

		while(shiftby != 0)
		{
			shiftby -= 8;
			buf[pos++] = len >> shiftby;
		}
		return pos;
	}
}

//flag 0: del non alpha/num
//flag 1: change all /
//flag 2: change all non filename conform chars
//flag 3: replace non alpha/num with _
void delspezchar(char* text, int flag)
{
	char* tmpstr = text;

	if(text == NULL) return;

	while(*tmpstr != '\0')
	{
		if(flag == 1 || flag == 2)
		{
			if(tmpstr[0] == '/') tmpstr[0] = '-';
			if(flag == 2)
			{
				//if(tmpstr[0] == '\A7') tmpstr[0] = '-';
				if(tmpstr[0] == 0xA7) tmpstr[0] = '-';
				if(tmpstr[0] == '<') tmpstr[0] = '-';
 				if(tmpstr[0] == '>') tmpstr[0] = '-';
				if(tmpstr[0] == ':') tmpstr[0] = '-';
				if(tmpstr[0] == '"') tmpstr[0] = '-';
				if(tmpstr[0] == '\\') tmpstr[0] = '-';
				if(tmpstr[0] == '|') tmpstr[0] = '-';
				if(tmpstr[0] == '*') tmpstr[0] = '-';
				if(tmpstr[0] == '?') tmpstr[0] = '-';
				if(tmpstr[0] == '[') tmpstr[0] = '-';
				if(tmpstr[0] == ']') tmpstr[0] = '-';
				if(tmpstr[0] == '=') tmpstr[0] = '-';
				if(tmpstr[0] == '%') tmpstr[0] = '-';
				if(tmpstr[0] == '$') tmpstr[0] = '-';
				if(tmpstr[0] == ',') tmpstr[0] = '-';
				if(tmpstr[0] == ';') tmpstr[0] = '-';
				if(tmpstr[0] == '~') tmpstr[0] = '-';
				if(tmpstr[0] == '&') tmpstr[0] = '-';
			}
		}
		else if(flag == 3)
		{
			if(!isalnum(tmpstr[0]) && tmpstr[0] != '-')
				tmpstr[0] = '_';
		}
		else
		{
			if(!isalnum(tmpstr[0]) && tmpstr[0] != '-' && tmpstr[0] != '.')
				tmpstr[0] = ' ';
		}
		tmpstr++;
	}
}

void m_lock(pthread_mutex_t *mutex, int flag)
{
	switch(flag)
	{
		case 0: debug(900, "drawingmutex lock"); break;
		case 1: debug(900, "rectimermutex lock"); break;
		case 2: debug(900, "servicemutex lock"); break;
		case 3: debug(900, "vfdmutex lock"); break;
		case 4: debug(900, "epgmutex lock"); break;
		case 5: debug(900, "channelmutex lock"); break;
		case 6: debug(900, "timerthreadmutex lock"); break;
		case 7: debug(900, "audiotrackmutex lock"); break;
		case 8: debug(900, "subtitlemutex lock"); break;
		case 9: debug(900, "dmxdevmutex lock"); break;
		case 10: debug(900, "rcmutex lock"); break;
		case 11: debug(900, "queuemutex lock"); break;
		case 12: debug(900, "clistmutex lock"); break;
		case 13: debug(900, "hddmutex lock"); break;
		case 14: debug(900, "linkedchannelmutex lock"); break;
		case 15: debug(900, "tsseekmutex lock"); break;
		case 16: debug(900, "accelfbmutex lock"); break;
		case 17: debug(900, "mediadbmutex lock"); break;
		case 18: debug(900, "oldentrymutex lock"); break;
		case 19: debug(900, "newslettermutex lock"); break;
		case 20: debug(900, "tithekmutex lock"); break;
		case 21: debug(900, "inetworkmutex lock"); break;
		case 22: debug(900, "textboxmutex lock"); break;
		case 23: debug(900, "setaktresmutex lock"); break;
		case 24: debug(900, "waitrcmutex lock"); break;
		case 25: debug(900, "oledmutex lock"); break;
		case 26: debug(900, "cecmutex lock"); break;
		default: debug(900, "unknown mutex lock"); break;
	}
	pthread_mutex_lock(mutex);
}

void m_unlock(pthread_mutex_t *mutex, int flag)
{
	switch(flag)
	{
		case 0: debug(900, "drawingmutex unlock"); break;
		case 1: debug(900, "rectimermutex unlock"); break;
		case 2: debug(900, "servicemutex unlock"); break;
		case 3: debug(900, "vfdmutex unlock"); break;
		case 4: debug(900, "epgmutex unlock"); break;
		case 5: debug(900, "channelmutex unlock"); break;
		case 6: debug(900, "timerthreadmutex unlock"); break;
		case 7: debug(900, "audiotrackmutex unlock"); break;
		case 8: debug(900, "subtitlemutex unlock"); break;
		case 9: debug(900, "dmxdevmutex unlock"); break;
		case 10: debug(900, "rcmutex unlock"); break;
		case 11: debug(900, "queuemutex unlock"); break;
		case 12: debug(900, "clistmutex unlock"); break;
		case 13: debug(900, "hddmutex unlock"); break;
		case 14: debug(900, "linkedchannelmutex unlock"); break;
		case 15: debug(900, "tsseekmutex unlock"); break;
		case 16: debug(900, "accelfbmutex unlock"); break;
		case 17: debug(900, "mediadbmutex unlock"); break;
		case 18: debug(900, "oldentrymutex unlock"); break;
		case 19: debug(900, "newslettermutex unlock"); break;
		case 20: debug(900, "tithekmutex unlock"); break;
		case 21: debug(900, "inetworkmutex unlock"); break;
		case 22: debug(900, "textboxmutex unlock"); break;
		case 23: debug(900, "setaktresmutex unlock"); break;
		case 24: debug(900, "waitrcmutex unlock"); break;
		case 25: debug(900, "oledmutex unlock"); break;
		case 26: debug(900, "cecmutex unlock"); break;
		default: debug(900, "unknown mutex unlock"); break;
	}
	pthread_mutex_unlock(mutex);
}

void debugstack(int sig, void* address, void* address1)
{
	Dl_info info, info1;
	void* trace[20];
	size_t size;
	size = backtrace(trace, 20);

	FILE* fd = NULL;
	void* akttrace[2];
	int i = 0;
	char **strings;
	char **aktstring;
	time_t rawtime;
#ifdef BETA
	int y = 0;
	Dl_info info2;
#endif

	char* imgversion = NULL;
	struct stimerthread* tnode = NULL;

	if(isfile(getconfig("imagenamefile", NULL))	!= 0)
		imgversion = readsys(getconfig("imagenamefile", NULL), 1);
	else
		imgversion = ostrcat("unknown", NULL, 0, 0);

	tnode = gettimerbythread(pthread_self());
	if(tnode != NULL)
		dladdr(tnode->func, &info);

	dladdr(address, &info1);

	strings = backtrace_symbols(trace, size);
	akttrace[0] = (void*)address1;
	akttrace[1] = (void*)address;
	aktstring = backtrace_symbols(akttrace, 2); //get fault funktion name

	fprintf(stderr, "--------------------------------------\n");
	fprintf(stderr, "Box: %s\n", getboxtype());
	fprintf(stderr, "Image: %s\n", imgversion);
	fprintf(stderr, "Signal: %d (%s)\n", sig, strsignal(sig));
	if(info1.dli_fname != NULL)
		fprintf(stderr, "Error in File: %s\n", info1.dli_fname);
	fprintf(stderr, "MainThread: %x\n", (unsigned int)status.mainthread);
	if(tnode != NULL && info.dli_sname != NULL)
		fprintf(stderr, "Error in Thread: %x (%s)\n", (unsigned int)pthread_self(), info.dli_sname);
	else
		fprintf(stderr, "Error in Thread: %x\n", (unsigned int)pthread_self());

	fprintf(stderr, "\nObtaining %zd stack frames:\n\n", size);
	for(i = 0; i < size; i++)
		fprintf(stderr, "%s\n", strings[i]);

	fprintf(stderr, "\nLast functions:\n\n");
	fprintf(stderr, "%s\n", aktstring[0]);
	fprintf(stderr, "%s\n", aktstring[1]);

#ifdef BETA
	for(i = 0; i < MAXSTACKTRACE; i++)
	{
		if(stacktrace[i].pos > 0)
		{
			fprintf(stderr, "\nObtaining %d Thread Stack frames [%p]:\n\n", stacktrace[i].pos, (void*)stacktrace[i].thread);
			for(y = stacktrace[i].pos - 1; y >= 0; y--)
			{
				dladdr(stacktrace[i].func[y], &info2);
        fprintf(stderr, "%s(%s) [%p]\n", info2.dli_fname, info2.dli_sname, stacktrace[i].func[y]);
			}
		}
	}
#endif

	fprintf(stderr, "--------------------------------------\n");

	char* logdir = ostrcat(getconfig("tracelog", NULL), NULL, 0, 0);
	if(logdir != NULL)
	{
		char* tmplogdir = dirname(logdir);
		if(tmplogdir != NULL)
		{
			if(!file_exist(tmplogdir))
				mkdir(tmplogdir, 0777);
		}
		free(logdir); logdir = NULL;
	}

	//del file if greater 100KB
	if(getfilesize(getconfig("tracelog", NULL)) > 102400)
		unlink(getconfig("tracelog", NULL));

	fd = fopen(getconfig("tracelog", NULL), "a");
	if(fd != NULL)
	{
		time(&rawtime);
		fprintf(fd, "Date: %s", ctime(&rawtime));
		fprintf(fd, "Box: %s\n", getboxtype());
		fprintf(fd, "Image: %s\n", imgversion);
		fprintf(fd, "Signal: %d (%s)\n", sig, strsignal(sig));
		if(info1.dli_fname != NULL)
			fprintf(fd, "Error in File: %s\n", info1.dli_fname);
		fprintf(fd, "MainThread: %x\n", (unsigned int)status.mainthread);
		if(tnode != NULL && info.dli_sname != NULL)
			fprintf(fd, "Error in Thread: %x (%s)\n", (unsigned int)pthread_self(), info.dli_sname);
		else
			fprintf(fd, "Error in Thread: %x\n", (unsigned int)pthread_self());

		fprintf(fd, "\nObtaining %zd stack frames:\n\n", size);
		for(i = 0; i < size; i++)
			fprintf(fd, "%s\n", strings[i]);

		fprintf(fd, "\nLast functions:\n\n");
		fprintf(fd, "%s\n", aktstring[0]);
		fprintf(fd, "%s\n", aktstring[1]);

#ifdef BETA
		for(i = 0; i < MAXSTACKTRACE; i++)
		{
			if(stacktrace[i].pos > 0)
			{
				fprintf(fd, "\nObtaining %d Thread Stack frames [%p]:\n\n", stacktrace[i].pos, (void*)stacktrace[i].thread);
				for(y = stacktrace[i].pos - 1; y >= 0; y--)
				{
					dladdr(stacktrace[i].func[y], &info2);
	        fprintf(fd, "%s(%s) [%p]\n", info2.dli_fname, info2.dli_sname, stacktrace[i].func[y]);
				}
			}
		}
#endif

		fprintf(fd, "--------------------------------------\n\n");
		fclose(fd);
		sync();
	}
	else
		perr("open %s", getconfig("tracelog", NULL));

	free(imgversion);
	free(strings);
	free(aktstring);
}

int mountauto(const char *file, const char *dir, unsigned long int flag, const void *data)
{
	int ret = -1;
	char* saveptr = NULL, *filesystems = NULL, *tmpfilesystems = NULL, *token = NULL;

	filesystems = getconfig("filesystems", NULL);
	tmpfilesystems = ostrcat(tmpfilesystems, filesystems, 0, 0);

	token = strtok_r(tmpfilesystems, ";", &saveptr);
	while(token != NULL)
	{
		ret = mount(file, dir, token, flag, data);
		if(ret == 0) break;
		token = strtok_r(NULL, ";", &saveptr);
	}

	free(tmpfilesystems);
	return ret;
}

int setwaswakuptimer(int value)
{
	char *waswakeuptimerdev = NULL, *tmpstr = NULL;
	int ret = 0;

	waswakeuptimerdev = getconfig("waswakeuptimerdev", NULL);

	if(waswakeuptimerdev != NULL)
	{
		debug(100, "set %s to %d", waswakeuptimerdev, value);
		tmpstr = oitoa(value);
		ret = writesys(waswakeuptimerdev, tmpstr, 0);
		free(tmpstr); tmpstr = NULL;
		return ret;
	}

	return 0;
}

int getwaswakuptimer()
{
	int ret = 0;
	char *waswakeuptimerdev = NULL;
	char *value = NULL;

	waswakeuptimerdev = getconfig("waswakeuptimerdev", NULL);

	if(waswakeuptimerdev == NULL)
	{
		err("NULL detect");
		return 0;
	}

	value = readsys(waswakeuptimerdev, 1);
	if(value == NULL)
	{
		err("NULL detect");
		return 0;
	}

	// start from timer
	if(atoi(value) == 1) ret = 1;

	free(value);
	return ret;
}

int getrtctime()
{
	int ret = 0;
	char *rtctimedev = NULL;
	char *value = NULL;

	rtctimedev = getconfig("rtctimedev", NULL);

	if(rtctimedev == NULL)
	{
		err("NULL detect");
		return 0;
	}

	value = readsys(rtctimedev, 1);
	if(value == NULL)
	{
		err("NULL detect");
		return 0;
	}

	// start from timer
	if(atoi(value) == 1) ret = 1;

	free(value);
	return ret;
}

void checkboxstart()
{
	struct rectimer* node = rectimer;
	int timediff = getconfigint("rectimer_timediff", NULL);
	int test = 0;

	if(node == NULL)
	{
		setcecstandby(0);
		return; 						//no record
	}

	while(node != NULL)
	{
		if(node->status < 2 || node->status == 4)
		{
			time_t akttime = time(NULL);
			time_t begin = node->begin - getconfigint("wakeuptimerdevdiff", NULL);

			debug(400, "begin=%ld akttime1=%ld akttime2=%ld", begin, akttime - timediff, akttime + timediff);

			if(begin > akttime - timediff && begin < akttime + timediff)
			{
				debug(400, "found rectimer who has start the box");
				setwaswakuptimer(1);
				setcecstandby(1);
				test = 1;
			}
		}
		node = node->next;
	}
	if(test == 0)
		setcecstandby(0);
}

int setwakeuptimerdev(time_t value)
{
	char* wakeuptimerdev, *tmpstr = NULL;
	int ret = 0;
	int diff = getconfigint("wakeuptimerdevdiff", NULL);

	if(value != 0x7FFFFFFF && value - diff > time(NULL))
		value -= diff;

	wakeuptimerdev = getconfig("wakeuptimerdev", NULL);

	if(wakeuptimerdev != NULL && value >= time(NULL))
	{
		debug(10, "set %s to %ld", wakeuptimerdev, value);
		tmpstr = olutoa(value);
		ret = writesys(wakeuptimerdev, tmpstr, 0);
		free(tmpstr); tmpstr = NULL;
		return ret;
	}

	return 0;
}

int setrtctime(int value)
{
	char *rtctimedev = NULL, *tmpstr = NULL;
	int ret = 0;

	value += 7200;

	rtctimedev = getconfig("rtctimedev", NULL);

	if(rtctimedev != NULL)
	{
		debug(10, "set %s to %d", rtctimedev, value);
		tmpstr = oitoa(value);
		ret = writesys(rtctimedev, tmpstr, 0);
		free(tmpstr); tmpstr = NULL;
		return ret;
	}

	return 0;
}

int setrtctimemips()
{
	char *rtctimedev = NULL, *tmpstr = NULL;
	int ret = 0;
//	int value = 0;
	
	time_t t = time(NULL);
	struct tm *local = localtime(&t);
 	time_t rawlocal = mktime(local);
 	
 	t = time(NULL);
	struct tm *gmt = gmtime(&t);
	time_t rawgmt = mktime(gmt);
  
	int offset = difftime(rawlocal, rawgmt);

	tmpstr = oitoa(offset);
	rtctimedev = getconfig("rtctime_offsetdev", NULL);
	if(rtctimedev != NULL)
		ret = writesys(rtctimedev, tmpstr, 0);
	else
		ret = writesys("/proc/stb/fp/rtc_offset", tmpstr, 0);
	free(tmpstr); tmpstr = NULL;
	rtctimedev = NULL;
	
	if(ret == 0)
	{
		tmpstr = oitoa(rawlocal);
		rtctimedev = getconfig("rtctimedev", NULL);
		if(rtctimedev != NULL)
			ret = writesys(rtctimedev, tmpstr, 0);
		else
			ret = writesys("/proc/stb/fp/rtc", tmpstr, 0);
		free(tmpstr); tmpstr = NULL;
		rtctimedev = NULL;
	}
	return ret;
}

int changepolicy()
{
	char *tmppolicy = NULL, *tmpstr = NULL;

 	tmppolicy = getpolicy();

#ifdef MIPSEL
/*
	char *aspect = NULL;
	aspect = getaspect();

	if(!ostrncmp("16:9", aspect, 4))
	{
		if(ostrcmp(_("16:9_set_bestfit_to_policy_show_justscale"), _(tmppolicy)) == 0)
			tmpstr = ostrcat(_("16:9_set_letterbox_to_policy_show_panscan"), NULL, 0, 0);
		else if(ostrcmp(_("16:9_set_letterbox_to_policy_show_panscan"), _(tmppolicy)) == 0)
			tmpstr = ostrcat(_("16:9_set_panscan_to_policy_show_pillarbox"), NULL, 0, 0);
		else if(ostrcmp(_("16:9_set_panscan_to_policy_show_pillarbox"), _(tmppolicy)) == 0)
			tmpstr = ostrcat(_("16:9_set_bestfit_to_policy_show_justscale"), NULL, 0, 0);

	}
	else
	{
		if(ostrcmp(_("4:3_set_bestfit_to_policy2_show_justscale"), _(tmppolicy)) == 0)
			tmpstr = ostrcat(_("4:3_set_letterbox_to_policy2_show_letterbox"), NULL, 0, 0);
		else if(ostrcmp(_("4:3_set_letterbox_to_policy2_show_letterbox"), _(tmppolicy)) == 0)
			tmpstr = ostrcat(_("4:3_set_panscan_to_policy2_show_panscan"), NULL, 0, 0);
		else if(ostrcmp(_("4:3_set_panscan_to_policy2_show_panscan"), _(tmppolicy)) == 0)
			tmpstr = ostrcat(_("4:3_set_bestfit_to_policy2_show_justscale"), NULL, 0, 0);
	}
	free(aspect), aspect = NULL;
*/
	if(ostrcmp(_("16:9_set_bestfit_to_policy_show_justscale"), _(tmppolicy)) == 0)
		tmpstr = ostrcat(_("16:9_set_letterbox_to_policy_show_panscan"), NULL, 0, 0);
	else if(ostrcmp(_("16:9_set_letterbox_to_policy_show_panscan"), _(tmppolicy)) == 0)
		tmpstr = ostrcat(_("16:9_set_panscan_to_policy_show_pillarbox"), NULL, 0, 0);
	else if(ostrcmp(_("16:9_set_panscan_to_policy_show_pillarbox"), _(tmppolicy)) == 0)
		tmpstr = ostrcat(_("4:3_set_bestfit_to_policy2_show_justscale"), NULL, 0, 0);
	else if(ostrcmp(_("4:3_set_bestfit_to_policy2_show_justscale"), _(tmppolicy)) == 0)
		tmpstr = ostrcat(_("4:3_set_letterbox_to_policy2_show_letterbox"), NULL, 0, 0);
	else if(ostrcmp(_("4:3_set_letterbox_to_policy2_show_letterbox"), _(tmppolicy)) == 0)
		tmpstr = ostrcat(_("4:3_set_panscan_to_policy2_show_panscan"), NULL, 0, 0);
	else if(ostrcmp(_("4:3_set_panscan_to_policy2_show_panscan"), _(tmppolicy)) == 0)
		tmpstr = ostrcat(_("16:9_set_bestfit_to_policy_show_justscale"), NULL, 0, 0);
#else
	if(!ostrncmp("letterbox", tmppolicy, 8))
		tmpstr = ostrcat(tmpstr, "panscan", 1, 0);
	else if(!ostrncmp("panscan", tmppolicy, 7))
		tmpstr = ostrcat(tmpstr, "non", 1, 0);
	else if(!ostrncmp("non", tmppolicy, 3))
		tmpstr = ostrcat(tmpstr, "bestfit", 1, 0);
	else if(!ostrncmp("bestfit", tmppolicy, 7))
		tmpstr = ostrcat(tmpstr, "letterbox", 1, 0);
#endif

	setpolicy(tmpstr);

	free(tmpstr); tmpstr = NULL;
	free(tmppolicy); tmppolicy = NULL;
	return 0;
}

char* getmac(char* dev)
{
	int sock = -1, i = 0;
	struct ifreq buf;
	char* tmpstr = NULL;
	char tmpbuf[4] = {};

	if(dev == NULL) return NULL;

	sock = socket(PF_INET, SOCK_DGRAM, 0);
	if(sock >= 0)
	{
		memset(&buf, 0x00, sizeof(buf));
		strcpy(buf.ifr_name, dev);
		ioctl(sock, SIOCGIFHWADDR, &buf);
		close(sock);

		for(i = 0; i < 6; i++)
		{
			snprintf(tmpbuf, 4, "%.2X:", (unsigned char)buf.ifr_hwaddr.sa_data[i]);
			tmpstr = ostrcat(tmpstr, tmpbuf, 1, 0);
		}
		if(tmpbuf != NULL)
			tmpstr[strlen(tmpstr) - 1] = '\0';
	}

	return tmpstr;
}

char* getdefaultgw()
{
	char* name = NULL;
	unsigned long def, gw, m;
	int flags, ref, use, metric, mtu, win, ir;
	struct in_addr ip;

	FILE *fd = fopen("/proc/net/route", "r");
	if(fd == NULL)
		return NULL;

	if(fscanf(fd, "%*[^\n]\n") < 0)
	{
		fclose(fd);
		return NULL;
	}

	name = malloc(64);
	if(name == NULL)
	{
		err("no mem");
		return NULL;
	}

	while(1)
	{
		int r = fscanf(fd, "%63s%lx%lx%x%d%d%d%lx%d%d%d\n", name, &def, &gw, &flags, &ref, &use, &metric, &m, &mtu, &win, &ir);
		if(def == 0) break;
		if(r != 11)
		{
			fclose(fd);
			free(name);
			return NULL;
		}
		if(!(flags & 0x0001)) continue; //is interface down
	}

	fclose(fd);
	free(name);

	ip.s_addr = gw;
	if(def == 0)
		return inet_ntoa(ip);

	return NULL;
}

char* changefilenameext(char* filename, char* ext)
{
	char* newfilename = NULL;
	char* zeichen = NULL;

	if(filename == NULL) return NULL;

	newfilename = ostrcat(filename, NULL, 0, 0);

	zeichen = strrchr(newfilename, '.');
	if(zeichen != NULL)
	{
		zeichen[0] = '\0';
		newfilename = ostrcat(newfilename, ext, 1, 0);
	}

	return newfilename;
}

int cmpfilenameext(char* filename, char* ext)
{
	char* zeichen = NULL;

	if(filename == NULL) return 1;

	zeichen = strrchr(filename, '.');
	if(zeichen != NULL)
	{
		if(ostrcasecmp(zeichen, ext) == 0) return 0;
	}

	return 1;
}

char* getfilenameext(char* filename)
{
	char* zeichen = NULL;

	if(filename == NULL) return NULL;

	zeichen = strrchr(filename, '.');
	if(zeichen != NULL)
	{
		return ostrcat(zeichen + 1, NULL, 0, 0);
	}

	return NULL;
}

char* getfilenamepng(char* filename)
{
	char* tmpstr = NULL;

	if(filename == NULL) return NULL;

	if(!filelistflt(".avi .dat .divx .flv .mkv .m4v .mp4 .mov .mpg .mpeg .mts .m2ts .trp .ts .vdr .vob .wmv .rm", filename))
		tmpstr = ostrcat("movie", NULL, 0, 0);
	else if(!filelistflt(".mp3 .flac .ogg .wma .ra .wav", filename))
		tmpstr = ostrcat("musik", NULL, 0, 0);
	else if(!filelistflt(".jpg .png", filename))
		tmpstr = ostrcat("picture", NULL, 0, 0);
	else
		tmpstr = ostrcat(getfilenameext(filename), NULL, 1, 0);

	if(tmpstr != NULL)
	{
		return tmpstr;
	}

	return NULL;
}

char* getcurrentdir(char* path)
{
	char* zeichen = NULL;

	if(path == NULL) return NULL;

	zeichen = strrchr(path, '/');
	if(zeichen != NULL)
	{
		return ostrcat(zeichen + 1, NULL, 0, 0);
	}

	return NULL;
}

char* convert_timesec(int sec)
{
	int hour = 0, min = 0, seconds = 0;
	char* buf = NULL;

	buf = malloc(9);
	if(buf == NULL)
	{
		err("no mem");
		return NULL;
	}

	seconds = sec % 60;
	min = (sec / 60) % 60;
	hour = sec / 3600;

	if(seconds < 0) seconds = 0;
	if(min < 0) min = 0;
	if(hour < 0 || hour > 23)
	{
		hour = 0;
		min = 0;
		seconds = 0;
	}

	snprintf(buf, 9, "%02d:%02d:%02d", hour, min, seconds);

	return buf;
}

char* convert_dtimesec(int sec)
{
	int hour = 0, min = 0, seconds = 0, days = 0;
	char* buf = NULL;

	buf = malloc(12);
	if(buf == NULL)
	{
		err("no mem");
		return NULL;
	}

	seconds = sec % 60;
	min = (sec / 60) % 60;
	hour = sec / 3600;
	if(hour > 23)
	{
		days = hour / 24;
		hour = hour % 24;
	}

	if(seconds < 0) seconds = 0;
	if(min < 0) min = 0;
	if(hour < 0/* || hour > 23*/)
	{
		hour = 0;
		min = 0;
		seconds = 0;
	}

	snprintf(buf, 12, "%02d_%02d:%02d:%02d",days, hour, min, seconds);

	return buf;
}

int checkdate() 
{ 
	time_t dvbtime = 0;
	if(timeokw == 0 || time(NULL) < 1072224000 || status.timeupdatecount > 3600) // 01.01.2004
	{
		printf("--> check dvbtime");
		if(timeokw == 0)
			sleep(4);
		if(dvbgetdate(&dvbtime, 10000000) == 0) //10 sek
		{
			printf("--> set dvbtime");
			setsystime(&dvbtime);
#if defined(OEBUILD) && defined(SH4)
			setrtctimemips();
#endif
#ifdef MIPSEL
			setrtctimemips();
//			if(file_exist("/bin/fake-hwclock"))
//			{
//				system("/bin/fake-hwclock save force");
//				system("/bin/fake-hwclock load force");
//			}
#endif
			status.timeupdatecount = 0;
			if(timeokw == 0)
			{
				if(timeokz == 0)
					timeokw = 1;
				else
					timeokz = timeokz + 1;
			}
			return 0;
		}
		return 1;
	}
	return 0;
}

int setdate()
{
	time_t dvbtime = 0;
	if(dvbgetdate(&dvbtime, 10000000) == 0) //10 sek
	{
		setsystime(&dvbtime);
#if defined(OEBUILD) && defined(SH4)
		setrtctimemips();
#endif
#ifdef MIPSEL
		setrtctimemips();
#endif
		return 0;
	}
	return 1;
}

void closeonexec(int fd)
{
	if(fd > -1)
		fcntl(fd, F_SETFD, fcntl(fd, F_GETFD) | FD_CLOEXEC);
}

void startnewsletter(int flag)
{
	if(flag == 1)
	{
		if(getconfigint("newsletter", NULL) == 1 && status.newsletterthread == NULL)
			status.newsletterthread = addtimer(&newsletterthreadfunc, START, 1000, 1, NULL, NULL, NULL);
	}
	else if(status.newsletterthread != NULL)
	{
		status.newsletterthread->aktion = STOP;
		status.newsletterthread = NULL;
	}
}

void startthumb(int flag)
{
	if(flag == 1)
	{
		if(getconfigint("createthumb", NULL) == 1 && status.thumbthread == NULL)
			status.thumbthread = addtimer(&thumbthread, START, 1000, 1, NULL, NULL, NULL);
	}
	else if(status.thumbthread != NULL)
	{
		status.thumbthread->aktion = STOP;
		status.thumbthread = NULL;
	}
}

void starthttpd(int flag)
{
	if(flag == 1)
	{
		if(getconfigint("httpdstart", NULL) == 1 && status.httpthread == NULL)
		{
			status.httpthread = addtimer(&httpdthreadfunc, START, 10000, -1, NULL, NULL, NULL);
			if(status.httpthread != NULL)
				status.httpthread->flag = setbit(status.httpthread->flag, 0);
		}
	}
	else if(status.httpthread != NULL)
	{
		status.httpthread->aktion = STOP;
		status.httpthread = NULL;
	}
}

void startrguid(int flag)
{
	if(flag == 1)
	{
		if(getconfigint("rguidstart", NULL) == 1 && status.rguithread == NULL)
		{
			status.rguithread = addtimer(&rguidthreadfunc, START, 10000, -1, NULL, NULL, NULL);
			if(status.rguithread != NULL)
				status.rguithread->flag = setbit(status.rguithread->flag, 0);
		}
	}
	else if(status.rguithread != NULL)
	{
		status.rguithread->aktion = STOP;
		status.rguithread = NULL;
	}
}

int setoverclockfreq(int mode)
{
	// mode 0 = Set Standby Freq
	// mode 1 = Set Freq

	if(ostrcmp(getownconfig("standby_overclock"), "0") == 0)
		return 1;

	if(isfile("/proc/cpu_frequ/pll0_ndiv_mdiv") == 0)
		return 1;

	char* tmpstr = NULL;

	if(mode == 0)
		tmpstr = getownconfig("standby_freq");
	else
		tmpstr = getownconfig("pll0_ndiv_mdiv");

	return writesys("/proc/cpu_frequ/pll0_ndiv_mdiv", tmpstr, 1);
}

/*
int checkdev(char* dev)
{
	char* cmd = NULL;
	cmd = ostrcat(cmd, "cat ", 1, 0);
	cmd = ostrcat(cmd, dev, 1, 0);

	char* tmpstr = NULL;
	tmpstr = string_newline(command(cmd));
	free(cmd), cmd = NULL;

	if(tmpstr == NULL)
	{
		return 0;
	}

	if(ostrcmp(tmpstr, "Segmentation fault") == 0)
	{
		return 0;
	}

	free(tmpstr), tmpstr = NULL;
	return 1;
}
*/

int setsaturation(int value)
{
#ifndef OEBUILD
	char* saturationdev;

	saturationdev = getconfig("saturationdev", NULL);

	if(saturationdev != NULL /*&& checkdev(saturationdev)*/)
	{
		debug(100, "set %s to %d", saturationdev, value);
		return writesysint(saturationdev, value, 1);
	}
#endif
	return 0;
}

int setbrightness(int value)
{
#ifndef OEBUILD
	char* brightnessdev;

	brightnessdev = getconfig("brightnessdev", NULL);

	if(brightnessdev != NULL /*&& checkdev(brightnessdev)*/)
	{
		debug(100, "set %s to %d", brightnessdev, value);
		return writesysint(brightnessdev, value, 1);
	}
#endif
	return 0;
}

int setcontrast(int value)
{
#ifndef OEBUILD
	char* contrastdev;

	contrastdev = getconfig("contrastdev", NULL);

	if(contrastdev != NULL /*&& checkdev(contrastdev)*/)
	{
		debug(100, "set %s to %d", contrastdev, value);
		return writesysint(contrastdev, value, 1);
	}
#endif
	return 0;
}

int settint(int value)
{
#ifndef OEBUILD
	char* tintdev;

	tintdev = getconfig("tintdev", NULL);

	if(tintdev != NULL /*&& checkdev(tintdev)*/)
	{
		debug(100, "set %s to %d", tintdev, value);
		return writesysint(tintdev, value, 1);
	}
#endif
	return 0;
}

int ozip(char* inbuf, int inlen, char** outbuf, int* outlen, int level)
{
	int ret = 0;
	z_stream stream;

	if(inbuf == NULL || outbuf == NULL || inlen == 0) return 1;

	stream.zalloc = Z_NULL;
	stream.zfree = Z_NULL;
	stream.opaque = Z_NULL;
	stream.avail_in = 0;
	stream.next_in = Z_NULL;
	stream.avail_out = 0;
	stream.next_out = Z_NULL;

	ret = deflateInit(&stream, level);
	if(ret != Z_OK)
		return 1;

	*outbuf = malloc(inlen);
	if(*outbuf == NULL)
	{
		err("no mem");
		(void)deflateEnd(&stream);
		return 1;
	}

	stream.avail_in = inlen;
	stream.next_in = (void*)inbuf;

	do
	{
		stream.avail_out = inlen;
		stream.next_out = (void*)*outbuf;

		ret = deflate(&stream, Z_FINISH);
		if(ret == Z_STREAM_ERROR)
		{
			free(*outbuf); *outbuf = NULL;
			(void)deflateEnd(&stream);
			return 1;
		}
	}
	while(stream.avail_out == 0);

	char* tmpoutbuf = *outbuf;
	*outlen = inlen - stream.avail_out;
	*outbuf = realloc(*outbuf, *outlen);
	if(*outbuf == NULL)
	{
		if(*outlen > 0)
		{
			err("no mem");
			free(tmpoutbuf);
		}
		*outlen = 0;
		(void)deflateEnd(&stream);
		return 1;
	}

	(void)deflateEnd(&stream);
	return 0;
}

//flag 0: malloc mem
//flag 1: don't malloc mem
//flag 2: malloc mem (gzip)
//flag 3: don't malloc mem (gzip)
int ounzip(char* inbuf, int inlen, char** outbuf, int* outlen, int maxbuf, int flag)
{
	int ret = 0;
	z_stream stream;

	if(inbuf == NULL || outbuf == NULL || maxbuf == 0) return 1;
	if(flag == 1 && *outbuf == NULL) return 1;

	stream.zalloc = Z_NULL;
	stream.zfree = Z_NULL;
	stream.opaque = Z_NULL;
	stream.avail_in = 0;
	stream.next_in = Z_NULL;
	stream.avail_out = 0;
	stream.next_out = Z_NULL;

	if(flag == 2 || flag == 3)
	{
		ret = inflateInit2(&stream, 16 + MAX_WBITS);
		if(flag == 2) flag = 0;
		else if(flag == 3) flag = 1;
	}
	else
		ret = inflateInit(&stream);
	if(ret != Z_OK)
		return 1;

	if(flag == 0)
	{
		*outbuf = malloc(maxbuf);
		if(*outbuf == NULL)
		{
			err("no mem");
			(void)inflateEnd(&stream);
			return 1;
		}
	}

	stream.avail_in = inlen;
	stream.next_in = (void*)inbuf;

	int round = 0;
	do
	{
		stream.avail_out = maxbuf;

		if(flag == 0 && round > 0)
		{
			char* tmpoutbuf = *outbuf;
			*outbuf = realloc(*outbuf, maxbuf * (round + 1));
			if(*outbuf == NULL)
			{
				err("no mem");
				free(tmpoutbuf);
				(void)inflateEnd(&stream);
				return 1;
			}
			stream.next_out = (void*)(*outbuf) + maxbuf * round;
		}
		else
			stream.next_out = (void*)*outbuf;

		ret = inflate(&stream, Z_NO_FLUSH);
		if(ret == Z_NEED_DICT || ret == Z_DATA_ERROR || ret == Z_MEM_ERROR)
		{
			if(flag == 0)
			{
				free(*outbuf);
				*outbuf = NULL;
			}
			(void)inflateEnd(&stream);
			return 1;
		}

		round++;
	}
	while(stream.avail_out == 0);

	if(flag == 0)
	{
		char* tmpoutbuf = *outbuf;
		*outlen = (maxbuf * round) - stream.avail_out;
		*outbuf = realloc(*outbuf, *outlen);
		if(*outbuf == NULL)
		{
			err("no mem");
			if(*outlen > 0) free(tmpoutbuf);
			*outlen = 0;
			(void)inflateEnd(&stream);
			return 1;
		}
	}
	else
		*outlen = maxbuf - stream.avail_out;

	(void)inflateEnd(&stream);
	return 0;
}

void endianswapshort(unsigned short* value)
{
	*value = (*value >> 8) | (*value << 8);
}

int rcnumber(int rcret)
{
	if(rcret == getrcconfigint("rc0", NULL) ||
	rcret == getrcconfigint("rc1", NULL) ||
	rcret == getrcconfigint("rc2", NULL) ||
	rcret == getrcconfigint("rc3", NULL) ||
	rcret == getrcconfigint("rc4", NULL) ||
	rcret == getrcconfigint("rc5", NULL) ||
	rcret == getrcconfigint("rc6", NULL) ||
	rcret == getrcconfigint("rc7", NULL) ||
	rcret == getrcconfigint("rc8", NULL) ||
	rcret == getrcconfigint("rc9", NULL))
		return rcret;
	else
		return -9999;
}

int checkdirext(char* dir, char* ext)
{
	struct dirent *dirent = NULL;
	DIR *pdir = NULL;

	pdir = opendir(dir);
	if(pdir != NULL)
	{
		while((dirent = readdir(pdir)) != NULL)
		{
			if(ostrcmp(".", dirent->d_name) != 0 && ostrcmp("..", dirent->d_name) != 0)
			{
				if(ostrstr(dirent->d_name, ext) != NULL)
				{
					closedir(pdir);
					return 1;
				}
			}
		}

		closedir(pdir);
	}

	return 0;
}

int delallfiles(char* dir, char* ext)
{
	struct dirent *dirent = NULL;
	DIR *pdir = NULL;
	char* tmpstr = NULL;

	pdir = opendir(dir);
	if(pdir != NULL)
	{
		while((dirent = readdir(pdir)) != NULL)
		{
			if(ostrcmp(".", dirent->d_name) != 0 && ostrcmp("..", dirent->d_name) != 0)
			{
				if(ext == NULL || ostrstr(dirent->d_name, ext) != NULL)
				{
					tmpstr = ostrcat(dir, "/", 0, 0);
					tmpstr = ostrcat(tmpstr, dirent->d_name, 1, 0);
					unlink(tmpstr);
					free(tmpstr); tmpstr = NULL;
				}
			}
		}

		closedir(pdir);
	}

	return 0;
}

unsigned long getfilecount(char* dir)
{
	unsigned long count = 0;
	struct dirent *dirent = NULL;
	DIR *pdir = NULL;

	pdir = opendir(dir);
	if(pdir != NULL)
	{
		while((dirent = readdir(pdir)) != NULL)
			count++;

		closedir(pdir);
	}

	return count;
}

int getfiletype(char* filename)
{
	struct stat64 s;

	if(filename == NULL) return -1;

	if(stat64(filename, &s) >= 0)
	{
		if(S_ISREG(s.st_mode)) return DT_REG;
		if(S_ISDIR(s.st_mode)) return DT_DIR;
		if(S_ISCHR(s.st_mode)) return DT_CHR;
		if(S_ISBLK(s.st_mode)) return DT_BLK;
		if(S_ISFIFO(s.st_mode)) return DT_FIFO;
		if(S_ISLNK(s.st_mode)) return DT_LNK;
	}

	return DT_UNKNOWN;
}

int getlfiletype(char* filename)
{
	struct stat64 s;

	if(filename == NULL) return -1;

	if(lstat64(filename, &s) >= 0)
	{
		if(S_ISREG(s.st_mode)) return DT_REG;
		if(S_ISDIR(s.st_mode)) return DT_DIR;
		if(S_ISCHR(s.st_mode)) return DT_CHR;
		if(S_ISBLK(s.st_mode)) return DT_BLK;
		if(S_ISFIFO(s.st_mode)) return DT_FIFO;
		if(S_ISLNK(s.st_mode)) return DT_LNK;
	}

	return DT_UNKNOWN;
}

unsigned long long getfullspace(char* dir)
{
	struct statfs64 s;
	unsigned long long fullsize = 0;

	if(statfs64(dir, &s) >= 0)
	{
		fullsize = s.f_blocks;
		fullsize *= s.f_bsize;
	}

	return fullsize;
}

unsigned long long getfreespace(char* dir)
{
	struct statfs64 s;
	unsigned long long freesize = 0;

	if(statfs64(dir, &s) >= 0)
	{
		freesize = s.f_bfree;
		freesize *= s.f_bsize;
	}

	return freesize;
}

int checkbit(int value, int bitpos)
{
	int ret = 0;

	ret = value & (1 << bitpos) ? 1: 0;
	return ret;
}

int tooglebit(int value, int bitpos)
{
	value ^= 1 << bitpos;
	return value;
}

int clearbit(int value, int bitpos)
{
	value &= ~(1 << bitpos);
	return value;
}

int setbit(int value, int bitpos)
{
	value |= 1 << bitpos;
	return value;
}

#ifdef OEBUILD
int stime(const time_t *t)
{
    struct timeval tv;
    tv.tv_sec = *t;
    tv.tv_usec = 0;
    return settimeofday(&tv, NULL);
}
#endif

int setsystime(time_t* newtime)
{
	if (stime(newtime))
	{
		err("can't set system time");
		return 1;
	}
	return 0;
}

off64_t getfilesize(char* name)
{
	struct stat64 sbuf;

	if(lstat64(name, &sbuf) == -1)
		return 0;

	return sbuf.st_size;
}

time_t getfiletime(char* name, int type)
{
	struct stat64 sbuf;

	if(lstat64(name, &sbuf) == -1)
		return 0;

	switch(type)
	{
		case 1: return sbuf.st_mtime;
		case 2: return sbuf.st_ctime;
		default: return sbuf.st_atime;
	}
}

int isfile(char* name)
{
	struct stat64 sbuf;

	if(lstat64(name, &sbuf) == -1)
		return 0;

	if(S_ISREG(sbuf.st_mode))
		return 1;

	return 0;
}

int islink(char* name)
{
	struct stat sbuf;

	if(lstat(name, &sbuf) == -1)
		return 0;

	if(S_ISLNK(sbuf.st_mode))
		return 1;

	return 0;
}

int isdir(char* name)
{
	struct stat64 sbuf;
	char *rpath = NULL;

	if(lstat64(name, &sbuf) == -1)
		return 0;

	if(S_ISDIR(sbuf.st_mode))
		return 1;

	if(S_ISLNK(sbuf.st_mode))
	{
		rpath = realpath(name, NULL);
		if(lstat64(rpath, &sbuf) == -1)
		{
			free(rpath);
			return 0;
		}
		free(rpath);
		if(S_ISDIR(sbuf.st_mode))
			return 1;
	}

	return 0;
}

char* getmaxsatstring(int maxsat)
{
	char* tmpstr = NULL, *tmpnr = NULL;
	int i;

	tmpstr = ostrcat(tmpstr, "1", 1, 0);
	for(i = 2; i <= maxsat; i++)
	{
		tmpnr = oitoa(i);
		tmpstr = ostrcat(tmpstr, "\n", 1, 0);
		tmpstr = ostrcat(tmpstr, tmpnr, 1, 1);
	}
	return tmpstr;
}

void ostrcatbig(char** value1, char* value2, int* maxlen, int* pos)
{
	int len = 0;

	if(value2 == NULL)
		return;

	len = strlen(value2) + 1;

	if(*value1 != NULL && maxlen == 0)
		*maxlen = strlen(*value1);

	if(*value1 == NULL || *pos + len > *maxlen)
	{
		*maxlen = *maxlen + len + (MINMALLOC * 10);
		*value1 = realloc(*value1, *maxlen);
		if(*value1 == NULL)
		{
			err("no mem");
			return;
		}
	}

	memcpy(*value1 + *pos, value2, len);
	*pos = *pos + (len - 1);
}

char* ostrshrink(char* value)
{
	int len = 0;
	char* buf = NULL;

	if(value == NULL) return NULL;

	len = strlen(value);
	if(len == 0) return value;

	buf = malloc(len + 1);
	if(buf == NULL)
		return value;

	memcpy(buf, value, len);
	free(value);
	buf[len] = '\0';

	return buf;
}

char* ostrcat(char* value1, char* value2, int free1, int free2)
{
	int len = 0, len1 = 0, len2 = 0;
	char* buf = NULL;

	if(value1 == NULL && value2 == NULL) return NULL;

	if(value1 != NULL) len1 = strlen(value1);
	if(value2 != NULL) len2 = strlen(value2);

	len = len1 + len2 + 1;

	if(free1 == 1)
		buf = realloc(value1, len);
	else
		buf = malloc(len);
	if(buf == NULL)
	{
		if(free1 == 1) free(value1);
		if(free2 == 1) free(value2);
		return NULL;
	}

	if(free1 == 0 && len1 > 0) memcpy(buf, value1, len1);
	if(len2 > 0) memcpy(buf + len1, value2, len2);
	buf[len - 1] = '\0';

	if(free2 == 1) free(value2);

	//helpfull for memleak detect
	//if(buf != NULL && strlen(buf) == 0x0b - 0x01)
	//	printf("******** memleak string (%s) (%p) ********\n", buf, buf);

	return buf;
}

char* ollutoa(uint64_t value)
{
	char *buf = NULL;

	buf = malloc(MINMALLOC);
	if(buf == NULL)
	{
		err("no mem");
		return NULL;
	}

	sprintf(buf, "%llu", value);
	buf = ostrshrink(buf);

	return buf;
}

char* olutoa(unsigned long value)
{
	char *buf = NULL;

	buf = malloc(MINMALLOC);
	if(buf == NULL)
	{
		err("no mem");
		return NULL;
	}

	sprintf(buf, "%lu", value);
	buf = ostrshrink(buf);

	return buf;
}

char* oitoax(int value)
{
	char *buf = NULL;

	buf = malloc(MINMALLOC);
	if(buf == NULL)
	{
		err("no mem");
		return NULL;
	}

	sprintf(buf, "%x", value);
	buf = ostrshrink(buf);

	return buf;
}

char* oitoa(int value)
{
	char *buf = NULL;

	buf = malloc(MINMALLOC);
	if(buf == NULL)
	{
		err("no mem");
		return NULL;
	}

	sprintf(buf, "%d", value);
	buf = ostrshrink(buf);

	return buf;
}

char* oitoa64(off64_t value)
{
	char *buf = NULL;

	buf = malloc(MINMALLOC);
	if(buf == NULL)
	{
		err("no mem");
		return NULL;
	}

	sprintf(buf, "%lld", value);
	buf = ostrshrink(buf);

	return buf;
}

char* oftoa64(double value, char* count)
{
	char *buf = NULL;
	char* tmpstr = NULL;

	buf = malloc(MINMALLOC);
	if(buf == NULL)
	{
		err("no mem");
		return NULL;
	}

	tmpstr = ostrcat("%.", count, 0, 0);
	tmpstr = ostrcat(tmpstr, "f", 1, 0);

	sprintf(buf, tmpstr, value);
	buf = ostrshrink(buf);

	free(tmpstr);
	return buf;
}

int ostrncmp(char* value1, char* value2, int count)
{
	int ret = 1;

	if(value1 != NULL && value2 != NULL)
		ret = strncmp(value1, value2, count);

	return ret;
}

int ostrcmp(char* value1, char* value2)
{
	int ret = 1;

	if(value1 != NULL && value2 != NULL)
		ret = strcmp(value1, value2);

	return ret;
}

int ostrcasecmp(char* value1, char* value2)
{
	int ret = 1;

	if(value1 != NULL && value2 != NULL)
		ret = strcasecmp(value1, value2);

	return ret;
}

char* createpath(char* dir, char* file)
{
	char *absdir = NULL;
	char *pos = NULL;
	char* tmpdir = NULL;

	if(dir == NULL || file == NULL)
	{
		err("NULL detect");
		return NULL;
	}

	tmpdir = ostrcat(dir, NULL, 0, 0);
	if(tmpdir == NULL)
	{
		err("NULL detect");
		return NULL;
	}

	while((tmpdir[strlen(tmpdir) - 1] == '.' && tmpdir[strlen(tmpdir) - 2] != '.') || tmpdir[strlen(tmpdir) - 1] == '/')
		tmpdir[strlen(tmpdir) - 1] = '\0';

	while(tmpdir[strlen(tmpdir) - 1] == '.' && tmpdir[strlen(tmpdir) - 2] == '.')
	{
		tmpdir[strlen(tmpdir) - 1] = '\0';
		tmpdir[strlen(tmpdir) - 1] = '\0';
		if(tmpdir[strlen(tmpdir) - 1] == '/')
			tmpdir[strlen(tmpdir) - 1] = '\0';
		pos = strrchr(tmpdir, '/');
		if(pos != NULL)
			pos[0] = '\0';
	}

	absdir = malloc(strlen(tmpdir) + strlen(file) + 2);
	if(absdir == NULL)
	{
		free(tmpdir);
		err("no mem");
		return NULL;
	}

	if(strlen(file) > 0 || strlen(tmpdir) == 0)
		sprintf(absdir, "%s/%s", tmpdir, file);
	else
		sprintf(absdir, "%s", tmpdir);

	free(tmpdir);
	return absdir;
}

int settimezone(char* zone)
{
	int ret = 0;
	char* tmpzone = NULL, *zonepath = NULL;

	zonepath = getconfig("zonepath", NULL);
	if(zonepath == NULL)
	{
		err("config zonepath not defined");
		return 1;
	}

	tmpzone = createpath(zonepath, zone);
	if(tmpzone == NULL)
		return 1;

	setenv("TZ", zone, 1);

	ret = unlink(getconfig("localtimefile", NULL));
	if(ret != 0)
		perr("unlink");
	ret = symlink(tmpzone, getconfig("localtimefile", NULL));
	if(ret != 0)
		perr("link");

	free(tmpzone);
	return ret;
}

int delchar(char** text, int pos)
{
	char *tmptext = NULL;
	int i = 0, y = 0, len = 0;

	if(text == NULL || *text == NULL)
		return pos;

	len = strlen(*text);
	if(len == 0) return pos;

	tmptext = malloc(strlen(*text));
	if(tmptext == NULL)
	{
		err("no mem");
		return pos;
	}

	for(i = 0; i < strlen(*text); i++)
	{
		if(i == pos - 1)
			y++;

		tmptext[i] = (*text)[y];
		y++;
	}

	if(i > 0) tmptext[i - 1] = '\0';
	if(pos >= strlen(tmptext)) pos = strlen(tmptext);

	free(*text);
	*text = tmptext;
	return pos;
}


int insertchar(char** text, char zeichen, int pos)
{
	char *tmptext = NULL;
	int i, y = 0;

	if(text == NULL || *text == NULL)
		return pos;

	tmptext = malloc(strlen(*text) + 2);
	if(tmptext == NULL)
	{
		err("no mem");
		return pos;
	}

	if(strlen(*text) == 0)
	{
		tmptext[0] = zeichen;
		i = 1;
		pos = 0;
	}
	else
	{
		for(i = 0; i < strlen(*text) + 1; i++)
		{
			if(i == pos)
				tmptext[i] = zeichen;
			else
			{
				tmptext[i] = (*text)[y];
				y++;
			}
		}
	}
	tmptext[i] = '\0';

	free(*text);
	*text = tmptext;
	return pos + 1;
}

void calctext(char* buf, char* buf1, unsigned int* linecount, unsigned int* pagecount, unsigned int* poscount, int pagelen, int page)
{
	unsigned int tmpposcount = 0;
	unsigned int pagepagelen = 0;

	*poscount = 0;
	*linecount = 1;
	*pagecount = 1;

	page--;
	if(page < 0) page = 0;
	pagepagelen = pagelen * page;

	if(buf != NULL)
	{
		while(*buf != '\0')
		{
			if(*buf == '\n')
			{
				if(*(buf + 1) == '\0') break;
				(*linecount)++;

				if(*linecount - 1 == pagepagelen)
					*poscount = tmpposcount + 1;
			}
			buf++;
			tmpposcount++;
		}
	}

	if(buf1 != NULL)
	{
		while(*buf1 != '\0')
		{
			if(*buf1 == '\n')
			{
				if(*(buf1 + 1) == '\0') break;
				(*linecount)++;

				if(*linecount - 1 == pagepagelen)
					*poscount = tmpposcount + 1;
			}
			buf1++;
			tmpposcount++;
		}
	}

	if(pagelen > 0)
		*pagecount = (int)ceil(((float)*linecount / pagelen));
}

int initlocale(char *localepath)
{
	debug(10, "localepath: %s", localepath);
	setlocale(LC_ALL, "");
	if(bindtextdomain(PROGNAME, localepath) == NULL)
	{
		debug(10, "err set bindtextdomain");
		err("set bindtextdomain");
		return 1;
	}
	if(textdomain(PROGNAME) == NULL)
	{
		debug(10, "err set textdomain");
		err("set textdomain");
		return 1;
	}
	return 0;
}

//for langage you must
//mkdir /usr/lib/locale
//ln -s %titanpath%/po /usr/lib/locale/po
//copy SYS_LC_MESSAGES from other language into LC_MESSAGE
int setlang(char *lang)
{
	debug(10, "lang: %s", lang);
	char *ret;

#ifdef OEBUILD
	// old: po/de new: de_DE
	char* tmpstr = NULL, *src = NULL, *dest = NULL, *cmd = NULL;
	tmpstr = ostrcat(lang, NULL, 0, 0);
	int count = 0;
	struct splitstr* ret1 = NULL;
	ret1 = strsplit(tmpstr, "/", &count);
	if(count > 1)
	{
		lang = ostrcat(ret1[1].part, NULL, 0, 0);
		lang = string_replace("de", "de_DE", lang, 1);
		lang = string_replace("el", "el_GR", lang, 1);
		lang = string_replace("en", "en_EN", lang, 1);
		lang = string_replace("es", "es_ES", lang, 1);
		lang = string_replace("fr", "fr_FR", lang, 1);
		lang = string_replace("it", "it_IT", lang, 1);
		lang = string_replace("lt", "lt_LT", lang, 1);
		lang = string_replace("nl", "nl_NL", lang, 1);
		lang = string_replace("pl", "pl_PL", lang, 1);
		lang = string_replace("ru", "ru_RU", lang, 1);
		lang = string_replace("vi", "vi_VN", lang, 1);
		debug(10, "lang changed: %s", lang);

		dest = ostrcat("/var/usr/local/share/titan/po/", ret1[1].part, 0, 0);
		src = ostrcat("/var/usr/local/share/titan/po/", lang, 0, 0);

		debug(10, "lang link: %s > %s", dest, src);
		symlink(dest, src);

		free(dest), dest = NULL;
		dest = ostrcat("/usr/lib/locale/", lang, 0, 0);
		debug(10, "lang check system locale: %s", dest);
		if(!file_exist(dest))
		{
			cmd = ostrcat("cp -a /usr/lib/locale/en_EN ", dest, 0, 0);
			debug(10, "lang create system locale: %s", dest);
			debug(10, "lang cmd: %s", cmd);
			system(cmd);
		}
	}
	free(ret1), ret1 = NULL;
	free(dest), dest = NULL;
	free(src), src = NULL;
	free(cmd), cmd = NULL;
	free(tmpstr), tmpstr = NULL;
#endif

	setenv("LANG", lang, 1);
	setenv("LANGUAGE", lang, 1);
	ret = setlocale(LC_MESSAGES, lang);
	if(ret == NULL)
	{
		printf("can't set LC_MESSAGES to %s\n", lang);
		err("can't set LC_MESSAGES to %s", lang);
		return 1;
	}
	ret = setlocale(LC_CTYPE, lang);
	return 0;
}

unsigned long readsysul(const char *filename, int line)
{
	int i = 0, len = 0;
	unsigned long ret = 0;
	FILE *fd = NULL;
	char *fileline = NULL;
	char *buf1 = NULL;

	fileline = malloc(MINMALLOC);
	if(fileline == NULL)
	{
		err("no mem");
		return 0;
	}

	fd = fopen(filename, "r");
	if(fd == NULL)
	{
		perr("can't open %s", filename);
		free(fileline);
		return 0;
	}

	for(i = 0; i < line; i++)
	{
		memset(fileline, 0, MINMALLOC);
		fgets(fileline, MINMALLOC, fd);
	}

	len = strlen(fileline) - 1;
	if(len >= 0 && fileline[len] == '\n')
		fileline[len] = '\0';
	buf1 = ostrshrink(fileline);

	fclose(fd);

	if(buf1 != NULL)
	{
		ret = strtoul(buf1, NULL, 10);
		free(buf1); buf1 = NULL;
	}

	return ret;
}

unsigned long long readsysull(const char *filename, int line)
{
	int i = 0, len = 0;
	unsigned long long ret = 0;
	FILE *fd = NULL;
	char *fileline = NULL;
	char *buf1 = NULL;

	fileline = malloc(MINMALLOC);
	if(fileline == NULL)
	{
		err("no mem");
		return 0;
	}

	fd = fopen(filename, "r");
	if(fd == NULL)
	{
		perr("can't open %s", filename);
		free(fileline);
		return 0;
	}

	for(i = 0; i < line; i++)
	{
		memset(fileline, 0, MINMALLOC);
		fgets(fileline, MINMALLOC, fd);
	}

	len = strlen(fileline) - 1;
	if(len >= 0 && fileline[len] == '\n')
		fileline[len] = '\0';
	buf1 = ostrshrink(fileline);

	fclose(fd);

	if(buf1 != NULL)
	{
		ret = strtoull(buf1, NULL, 10);
		free(buf1); buf1 = NULL;
	}

	return ret;
}

char* readsys(const char *filename, int line)
{
	int i = 0, len = 0;
	FILE *fd = NULL;
	char *fileline = NULL;
	char *buf1 = NULL;

	fileline = malloc(MINMALLOC);
	if(fileline == NULL)
	{
		err("no mem");
		return NULL;
	}

	fd = fopen(filename, "r");
	if(fd == NULL)
	{
		perr("can't open %s", filename);
		free(fileline);
		return NULL;
	}

	for(i = 0; i < line; i++)
	{
		memset(fileline, 0, MINMALLOC);
		fgets(fileline, MINMALLOC, fd);
	}

	len = strlen(fileline) - 1;
	if(len >= 0 && fileline[len] == '\n')
		fileline[len] = '\0';
	buf1 = ostrshrink(fileline);

	fclose(fd);
	return buf1;
}

//flag 0: without \n
//flag 1: with \n
//flag 2: append without \n
//flag 3: append with \n
int writesys(const char *filename, char *value, int flag)
{
	FILE *fd = NULL;
	char* tmpstr = NULL;
	int ret;

	if(value == NULL)
	{
		err("NULL detect");
		return 1;
	}

#ifdef OEBUILD
	// workaround for vuboxes dont send a \n to procfs
	if(!ostrncmp("/proc/", (char*)filename, 6) && flag == 1)
		flag = 0;
#endif

	if(flag == 2 || flag == 3)
		fd = fopen(filename, "a");
	else
		fd = fopen(filename, "w");
	if(fd == NULL)
	{
		perr("can't open %s", filename);
		return 1;
	}

	if(flag == 1 || flag == 3)
		tmpstr = ostrcat(value, "\n", 0, 0);
	else
		tmpstr = ostrcat(value, NULL, 0, 0);

	ret = fwrite(tmpstr, strlen(tmpstr), 1, fd);
	if(ret != 1)
	{
		perr("writting to %s", filename);
		free(tmpstr);
		fclose(fd);
		return 1;
	}

	free(tmpstr);
	fclose(fd);
	return 0;
}

//flag 0: without \n
//flag 1: with \n
//flag 2: append without \n
//flag 3: append with \n
int writesysint(const char *filename, int value, int flag)
{
	char* tmpstr = NULL;
	int ret = 0;

	tmpstr = oitoa(value);
	ret = writesys(filename, tmpstr, flag);

	free(tmpstr);
	return ret;
}

char* getdevcontent(char* devconfig)
{
	char *dev = NULL;
	char *value = NULL;

	dev = getconfig(devconfig, NULL);

	if(dev == NULL)
	{
		err("NULL detect");
		return NULL;
	}

	value = readsys(dev, 1);
	if(value == NULL)
	{
		err("NULL detect");
		return NULL;
	}

	return value;
}

char* convertspacetolf(char* value)
{
	int i = 0;

	while(value[i] != '\0')
	{
		if(value[i] == ' ')
			value[i] = '\n';
		i++;
	}

	return value;
}

//flag 0: all
//flag 1: hdmi
//flag 2: not hdmi
char* getcolorformatchoices(int flag)
{
	char *colorformatchoicesdev = NULL;
	char *value = NULL;

	colorformatchoicesdev = getconfig("colorformatchoicesdev", NULL);

	if(colorformatchoicesdev == NULL)
	{
		err("NULL detect");
		return NULL;
	}

	value = readsys(colorformatchoicesdev, 1);
	if(value == NULL)
	{
		err("NULL detect");
		return NULL;
	}

	if(flag == 1)
	{
		value = string_replace("rgb", NULL, value, 1);
		value = string_replace("  ", " ", value, 1);
		value = string_replace("cvbs", NULL, value, 1);
		value = string_replace("  ", " ", value, 1);
		value = string_replace("svideo", NULL, value, 1);
		value = string_replace("  ", " ", value, 1);
		value = string_replace("yuv", NULL, value, 1);
		value = string_replace("  ", " ", value, 1);
	}
	else if(flag == 2)
	{
		value = string_replace("hdmi_rgb", NULL, value, 1);
		value = string_replace("  ", " ", value, 1);
		value = string_replace("hdmi_yuv", NULL, value, 1);
		value = string_replace("  ", " ", value, 1);
		value = string_replace("hdmi_422", NULL, value, 1);
		value = string_replace("  ", " ", value, 1);
	}

	value = strstrip(value);
	value = convertspacetolf(value);

	return value;
}

char* getaudiosourcechoices()
{
	char *audiosourcechoicesdev = NULL;
	char *value = NULL;

	audiosourcechoicesdev = getconfig("audiosourcechoicesdev", NULL);

	if(audiosourcechoicesdev == NULL)
	{
		err("NULL detect");
		return NULL;
	}

	value = readsys(audiosourcechoicesdev, 1);
	if(value == NULL)
	{
		err("NULL detect");
		return NULL;
	}

	value = convertspacetolf(value);

	return value;
}

char* getac3pluschoices()
{
	char *ac3pluschoicesdev = NULL;
	char *value = NULL;

	ac3pluschoicesdev = getconfig("ac3pluschoicesdev", NULL);

	if(ac3pluschoicesdev == NULL)
	{
		addconfig("ac3pluschoicesdev", "/proc/stb/audio/ac3plus_choices");
		ac3pluschoicesdev = getconfig("ac3pluschoicesdev", NULL);
	}

	value = readsys(ac3pluschoicesdev, 1);
	if(value == NULL)
	{
		err("NULL detect");
		return NULL;
	}

	value = convertspacetolf(value);

	return value;
}

char* getdtshdchoices()
{
	char *dtshdchoicesdev = NULL;
	char *value = NULL;

	dtshdchoicesdev = getconfig("dtshdchoicesdev", NULL);

	if(dtshdchoicesdev == NULL)
	{
		addconfig("dtshdchoicesdev", "/proc/stb/audio/dtshd_choices");
		dtshdchoicesdev = getconfig("dtshdchoicesdev", NULL);
	}

	value = readsys(dtshdchoicesdev, 1);
	if(value == NULL)
	{
		err("NULL detect");
		return NULL;
	}

	value = convertspacetolf(value);

	return value;
}

char* getwmaprochoices()
{
	char *wmaprochoicesdev = NULL;
	char *value = NULL;

	wmaprochoicesdev = getconfig("wmaprochoicesdev", NULL);

	if(wmaprochoicesdev == NULL)
	{
		addconfig("wmaprochoicesdev", "/proc/stb/audio/wmapro_choices");
		wmaprochoicesdev = getconfig("wmaprochoicesdev", NULL);
	}

	value = readsys(wmaprochoicesdev, 1);
	if(value == NULL)
	{
		err("NULL detect");
		return NULL;
	}

	value = convertspacetolf(value);

	return value;
}

char* getac3choices()
{
	char *ac3choicesdev = NULL;
	char *value = NULL;

	ac3choicesdev = getconfig("ac3choicesdev", NULL);

	if(ac3choicesdev == NULL)
	{
		err("NULL detect");
		return NULL;
	}

	value = readsys(ac3choicesdev, 1);
	if(value == NULL)
	{
		err("NULL detect");
		return NULL;
	}

	value = convertspacetolf(value);

	return value;
}

char* getaacchoices()
{
	char *aacchoicesdev = NULL;
	char *value = NULL;

	aacchoicesdev = getconfig("aacchoicesdev", NULL);

	if(aacchoicesdev == NULL)
	{
		err("NULL detect");
		return NULL;
	}

	value = readsys(aacchoicesdev, 1);
	if(value == NULL)
	{
		err("NULL detect");
		return NULL;
	}

	value = convertspacetolf(value);

	return value;
}

char* getwsschoices()
{
	char *wsschoicesdev = NULL;
	char *value = NULL;

	wsschoicesdev = getconfig("wsschoicesdev", NULL);

	if(wsschoicesdev == NULL)
	{
		err("NULL detect");
		return NULL;
	}

	value = readsys(wsschoicesdev, 1);
	if(value == NULL)
	{
		err("NULL detect");
		return NULL;
	}

	value = convertspacetolf(value);

	return value;
}

int setciclock(int slotnr, char* value)
{
	char* ciclockdev = NULL, *tmpstr = NULL;
	int ret = 0;

	ciclockdev = getconfig("ciclockdev", NULL);

	if(ciclockdev != NULL && value != NULL)
	{
		tmpstr = malloc(MINMALLOC);
		if(tmpstr == NULL)
		{
			err("no mem");
			return 1;
		}

		snprintf(tmpstr, MINMALLOC, ciclockdev, slotnr);
		debug(100, "set %s to %s", tmpstr, value);
		ret = writesys(tmpstr, value, 0);

		free(tmpstr); tmpstr = NULL;
		return ret;
	}

	return 0;
}

//routes stream from tuner to ci or ci to ci
int setciinput(int slotnr, char* value)
{
	char* ciinputdev = NULL, *tmpstr = NULL;
	int ret = 0;

	ciinputdev = getconfig("ciinputdev", NULL);

	if(ciinputdev != NULL && value != NULL)
	{
		tmpstr = malloc(MINMALLOC);
		if(tmpstr == NULL)
		{
			err("no mem");
			return 1;
		}

		snprintf(tmpstr, MINMALLOC, ciinputdev, slotnr);
		debug(100, "set %s to %s", tmpstr, value);
		ret = writesys(tmpstr, value, 0);

		free(tmpstr); tmpstr = NULL;
		return ret;
	}

	return 0;
}

//routes stream from tuner or ci to linux
int setcisource(int tunernr, char* value)
{
	char* cisourcedev = NULL, *tmpstr = NULL;
	int ret = 0;

	cisourcedev = getconfig("cisourcedev", NULL);

	if(cisourcedev != NULL && value != NULL)
	{
		tmpstr = malloc(MINMALLOC);
		if(tmpstr == NULL)
		{
			err("no mem");
			return 1;
		}

		snprintf(tmpstr, MINMALLOC, cisourcedev, tunernr);
		debug(100, "set %s to %s", tmpstr, value);
		ret = writesys(tmpstr, value, 0);

		free(tmpstr); tmpstr = NULL;
		return ret;
	}

	return 0;
}

int setinput(char* value)
{
	char* inputdev = NULL;
	int ret = 0;

	inputdev = getconfig("inputdev", NULL);

	if(inputdev != NULL && value != NULL)
	{
		debug(100, "set %s to %s", inputdev, value);
		ret = writesys(inputdev, value, 0);
		return ret;
	}

	return 0;
}

char* getinput()
{
	char *inputdev = NULL;
	char *value = NULL;

	inputdev = getconfig("inputdev", NULL);

	if(inputdev == NULL)
	{
		err("NULL detect");
		return NULL;
	}

	value = readsys(inputdev, 1);
	if(value == NULL)
	{
		err("NULL detect");
		return NULL;
	}

	return value;
}

int setac3plus(char* value)
{
	char* ac3plusdev = NULL;
	int ret = 0;

	ac3plusdev = getconfig("ac3plusdev", NULL);
	if(ac3plusdev == NULL)
	{	
		addconfig("ac3plusdev", "/proc/stb/audio/ac3plus");
		ac3plusdev = getconfig("ac3plusdev", NULL);
	}

	if(ac3plusdev != NULL && value != NULL)
	{
		debug(100, "set %s to %s", ac3plusdev, value);
		ret = writesys(ac3plusdev, value, 0);
		if(ret == 0) addconfig("av_ac3plusmode", value);
		return ret;
	}

	return 0;
}

int setdtshd(char* value)
{
	char* dtshddev = NULL;
	int ret = 0;

	dtshddev = getconfig("dtshddev", NULL);
	if(dtshddev == NULL)
	{	
		addconfig("dtshddev", "/proc/stb/audio/dtshd");
		dtshddev = getconfig("dtshddev", NULL);
	}

	if(dtshddev != NULL && value != NULL)
	{
		debug(100, "set %s to %s", dtshddev, value);
		ret = writesys(dtshddev, value, 0);
		if(ret == 0) addconfig("av_dtshdmode", value);
		return ret;
	}

	return 0;
}

int setwmapro(char* value)
{
	char* wmaprodev = NULL;
	int ret = 0;

	wmaprodev = getconfig("wmaprodev", NULL);
	if(wmaprodev == NULL)
	{	
		addconfig("wmaprodev", "/proc/stb/audio/wmapro");
		wmaprodev = getconfig("wmaprodev", NULL);
	}

	if(wmaprodev != NULL && value != NULL)
	{
		debug(100, "set %s to %s", wmaprodev, value);
		ret = writesys(wmaprodev, value, 0);
		if(ret == 0) addconfig("av_wmapromode", value);
		return ret;
	}

	return 0;
}

int setac3(char* value)
{
	char* ac3dev = NULL;
	int ret = 0;

	ac3dev = getconfig("ac3dev", NULL);

	if(ac3dev != NULL && value != NULL)
	{
		debug(100, "set %s to %s", ac3dev, value);
		ret = writesys(ac3dev, value, 0);
		if(ret == 0) addconfig("av_ac3mode", value);
		if(ostrcmp(value, "passthrough") == 0)
			status.downmix = 0;
		else
			status.downmix = 1;
		return ret;
	}

	return 0;
}

int setaac(char* value)
{
	char* aacdev = NULL;
	int ret = 0;

	aacdev = getconfig("aacdev", NULL);

	if(aacdev != NULL && value != NULL)
	{
		debug(100, "set %s to %s", aacdev, value);
		ret = writesys(aacdev, value, 0);
		if(ret == 0) addconfig("av_aacmode", value);
		return ret;
	}

	return 0;
}

int setwss(char* value)
{
	char* wssdev = NULL;
	int ret = 0;

	wssdev = getconfig("wssdev", NULL);

	if(wssdev != NULL && value != NULL)
	{
		debug(100, "set %s to %s", wssdev, value);
		ret = writesys(wssdev, value, 0);
		if(ret == 0) addconfig("av_wssmode", value);
		return ret;
	}
	else if(wssdev != NULL && value == NULL)
	{
		debug(100, "set %s to %s", wssdev, "auto");
		ret = writesys(wssdev, "auto", 0);
		return ret;
	}

	return 0;
}

char* getac3plus()
{
	char *ac3plusdev = NULL;
	char *value = NULL;

	ac3plusdev = getconfig("ac3plusdev", NULL);

	if(ac3plusdev == NULL)
	{
		err("NULL detect");
		return NULL;
	}

	value = readsys(ac3plusdev, 1);
	if(value == NULL)
	{
		err("NULL detect");
		return NULL;
	}

	return value;
}

char* getdtshd()
{
	char *dtshddev = NULL;
	char *value = NULL;

	dtshddev = getconfig("dtshddev", NULL);

	if(dtshddev == NULL)
	{
		err("NULL detect");
		return NULL;
	}

	value = readsys(dtshddev, 1);
	if(value == NULL)
	{
		err("NULL detect");
		return NULL;
	}

	return value;
}

char* getwmapro()
{
	char *wmaprodev = NULL;
	char *value = NULL;

	wmaprodev = getconfig("wmaprodev", NULL);

	if(wmaprodev == NULL)
	{
		err("NULL detect");
		return NULL;
	}

	value = readsys(wmaprodev, 1);
	if(value == NULL)
	{
		err("NULL detect");
		return NULL;
	}

	return value;
}

char* getac3()
{
	char *ac3dev = NULL;
	char *value = NULL;

	ac3dev = getconfig("ac3dev", NULL);

	if(ac3dev == NULL)
	{
		err("NULL detect");
		return NULL;
	}

	value = readsys(ac3dev, 1);
	if(value == NULL)
	{
		err("NULL detect");
		return NULL;
	}

	return value;
}

char* getaac()
{
	char *aacdev = NULL;
	char *value = NULL;

	aacdev = getconfig("aacdev", NULL);

	if(aacdev == NULL)
	{
		err("NULL detect");
		return NULL;
	}

	value = readsys(aacdev, 1);
	if(value == NULL)
	{
		err("NULL detect");
		return NULL;
	}

	return value;
}

char* getwss()
{
	char *wssdev = NULL;
	char *value = NULL;

	wssdev = getconfig("wssdev", NULL);

	if(wssdev == NULL)
	{
		err("NULL detect");
		return NULL;
	}

	value = readsys(wssdev, 1);
	if(value == NULL)
	{
		err("NULL detect");
		return NULL;
	}

	return value;
}

int setaudiodelaybitstream(char* value)
{
	char* audiodelaybitstreamdev = NULL;
	int ret = 0;

	audiodelaybitstreamdev = getconfig("audiodelaybitstreamdev", NULL);

	if(audiodelaybitstreamdev != NULL && value != NULL)
	{
		debug(100, "set %s to %s", audiodelaybitstreamdev, value);
		ret = writesys(audiodelaybitstreamdev, value, 0);
		if(ret == 0) addconfig("audiodelaybitstream", value);
		return ret;
	}

	return 0;
}

char* getaudiodelaybitstream()
{
	char *audiodelaybitstreamdev = NULL;
	char *value = NULL;

	audiodelaybitstreamdev = getconfig("audiodelaybitstreamdev", NULL);

	if(audiodelaybitstreamdev == NULL)
	{
		err("NULL detect");
		return NULL;
	}

	value = readsys(audiodelaybitstreamdev, 1);
	if(value == NULL)
	{
		err("NULL detect");
		return NULL;
	}

	return value;
}

int setaudiodelaypcm(char* value)
{
	char* audiodelaypcmdev = NULL;
	int ret = 0;

	audiodelaypcmdev = getconfig("audiodelaypcmdev", NULL);

	if(audiodelaypcmdev != NULL && value != NULL)
	{
		debug(100, "set %s to %s", audiodelaypcmdev, value);
		ret = writesys(audiodelaypcmdev, value, 0);
		if(ret == 0) addconfig("audiodelaypcm", value);
		return ret;
	}

	return 0;
}

char* getaudiodelaypcm()
{
	char *audiodelaypcmdev = NULL;
	char *value = NULL;

	audiodelaypcmdev = getconfig("audiodelaypcmdev", NULL);

	if(audiodelaypcmdev == NULL)
	{
		err("NULL detect");
		return NULL;
	}

	value = readsys(audiodelaypcmdev, 1);
	if(value == NULL)
	{
		err("NULL detect");
		return NULL;
	}

	return value;
}


char* getpolicychoices()
{
	char *policychoicesdev = NULL;
	char *value = NULL;

	policychoicesdev = getconfig("policychoicesdev", NULL);

	if(policychoicesdev == NULL)
	{
		err("NULL detect");
		return NULL;
	}

	value = readsys(policychoicesdev, 1);
	if(value == NULL)
	{
		err("NULL detect");
		return NULL;
	}

#ifdef MIPSEL
	free(value), value = NULL;
//	value = ostrcat("16:9_set_bestfit_to_policy_show_justscale 16:9_set_letterbox_to_policy_show_panscan 16:9_set_panscan_to_policy_show_pillarbox 4:3_set_bestfit_to_policy2_show_justscale 4:3_set_letterbox_to_policy2_show_letterbox 4:3_set_panscan_to_policy2_show_panscan", NULL, 0, 0);
	value = ostrcat(_("16:9_set_bestfit_to_policy_show_justscale"), NULL, 0, 0);
	value = ostrcat(value, "\n", 1, 0);
	value = ostrcat(value, _("16:9_set_letterbox_to_policy_show_panscan"), 1, 0);
	value = ostrcat(value, "\n", 1, 0);
	value = ostrcat(value, _("16:9_set_panscan_to_policy_show_pillarbox"), 1, 0);
	value = ostrcat(value, "\n", 1, 0);
	value = ostrcat(value, _("4:3_set_bestfit_to_policy2_show_justscale"), 1, 0);
	value = ostrcat(value, "\n", 1, 0);
	value = ostrcat(value, _("4:3_set_letterbox_to_policy2_show_letterbox"), 1, 0);
	value = ostrcat(value, "\n", 1, 0);
	value = ostrcat(value, _("4:3_set_panscan_to_policy2_show_panscan"), 1, 0);
#else
	value = convertspacetolf(value);
#endif

	return value;
}

char* getpolicy()
{
	char *policydev = NULL;
	char *value = NULL;

	policydev = getconfig("policydev", NULL);

	if(policydev == NULL)
	{
		err("NULL detect");
		return NULL;
	}

	value = readsys(policydev, 1);

#ifdef MIPSEL
	char *aspect = NULL;
	aspect = getaspect();
	if(!ostrncmp("16:9", aspect, 4))
	{
		if(ostrcmp("bestfit", value) == 0)
		{
			free(value), value = NULL;
			value = ostrcat(_("16:9_set_bestfit_to_policy_show_justscale"), NULL, 0, 0);
		}
		else if(ostrcmp("letterbox", value) == 0)
		{
			free(value), value = NULL;
			value = ostrcat(_("16:9_set_letterbox_to_policy_show_panscan"), NULL, 0, 0);
		}
		else if(ostrcmp("panscan", value) == 0)
		{
			free(value), value = NULL;
			value = ostrcat(_("16:9_set_panscan_to_policy_show_pillarbox"), NULL, 0, 0);
		}
	}
	else
	{
		policydev = getconfig("policy2dev", NULL);
		value = readsys(policydev, 1);

		if(ostrcmp("bestfit", value) == 0)
		{
			free(value), value = NULL;
			value = ostrcat(_("4:3_set_bestfit_to_policy2_show_justscale"), NULL, 0, 0);
		}
		else if(ostrcmp("letterbox", value) == 0)
		{
			free(value), value = NULL;
			value = ostrcat(_("4:3_set_letterbox_to_policy2_show_letterbox"), NULL, 0, 0);
		}
		else if(ostrcmp("panscan", value) == 0)
		{
			free(value), value = NULL;
			value = ostrcat(_("4:3_set_panscan_to_policy2_show_panscan"), NULL, 0, 0);
		}
	}
	free(aspect), aspect = NULL;

#endif

	if(value == NULL)
	{
		err("NULL detect");
		return NULL;
	}

	return value;
}

int setpolicy(char* value)
{
	char* policydev, *tmpstr = NULL, *tmpstr2 = NULL;
	int ret = 0;

	policydev = getconfig("policydev", NULL);

	if(policydev != NULL && value != NULL)
	{
		debug(10, "set %s to %s", policydev, value);

#ifdef MIPSEL
		if(ostrcmp(_("16:9_set_bestfit_to_policy_show_justscale"), _(value)) == 0)
		{
			tmpstr = ostrcat("bestfit", NULL, 0, 0);
			tmpstr2 = ostrcat("16:9_set_bestfit_to_policy_show_justscale", NULL, 0, 0);
			setaspect("set_16:9");
		}
		else if(ostrcmp(_("16:9_set_letterbox_to_policy_show_panscan"), _(value)) == 0)
		{
			tmpstr = ostrcat("letterbox", NULL, 0, 0);
			tmpstr2 = ostrcat("16:9_set_letterbox_to_policy_show_panscan", NULL, 0, 0);
			setaspect("set_16:9");
		}
		else if(ostrcmp(_("16:9_set_panscan_to_policy_show_pillarbox"), _(value)) == 0)
		{
			tmpstr = ostrcat("panscan", NULL, 0, 0);
			tmpstr2 = ostrcat("16:9_set_panscan_to_policy_show_pillarbox", NULL, 0, 0);
			setaspect("set_16:9");
		}
		else if(ostrcmp(_("4:3_set_bestfit_to_policy2_show_justscale"), _(value)) == 0)
		{
			policydev = getconfig("policy2dev", NULL);
			tmpstr = ostrcat("bestfit", NULL, 0, 0);
			tmpstr2 = ostrcat("4:3_set_bestfit_to_policy2_show_justscale", NULL, 0, 0);
			setaspect("set_4:3");
		}
		else if(ostrcmp(_("4:3_set_letterbox_to_policy2_show_letterbox"), _(value)) == 0)
		{
			policydev = getconfig("policy2dev", NULL);
			tmpstr = ostrcat("letterbox", NULL, 0, 0);
			tmpstr2 = ostrcat("4:3_set_letterbox_to_policy2_show_letterbox", NULL, 0, 0);
			setaspect("set_4:3");
		}
		else if(ostrcmp(_("4:3_set_panscan_to_policy2_show_panscan"), _(value)) == 0)
		{
			policydev = getconfig("policy2dev", NULL);
			tmpstr = ostrcat("panscan", NULL, 0, 0);
			tmpstr2 = ostrcat("4:3_set_panscan_to_policy2_show_panscan", NULL, 0, 0);
			setaspect("set_4:3");
		}
#else
		tmpstr = ostrcat(value, NULL, 0, 0);
#endif
		debug(10, "set change %s to %s", policydev, tmpstr);
		ret = writesys(policydev, tmpstr, 0);

#ifdef MIPSEL
		if(ret == 0)
		{
			debug(10, "write %s to /mnt/config/titan.cfg", tmpstr2);
			addconfig("av_policy", tmpstr2);
		}
#else
		if(ret == 0) addconfig("av_policy", value);
		{
			debug(10, "write %s to /mnt/config/titan.cfg", value);
			addconfig("av_policy", value);
		}
#endif
	}

	free(tmpstr), tmpstr = NULL;
	free(tmpstr2), tmpstr2 = NULL;
	return ret;
}

char* getaspectchoices()
{
	char *aspectchoicesdev = NULL;
	char *value = NULL;

	aspectchoicesdev = getconfig("aspectchoicesdev", NULL);

	if(aspectchoicesdev == NULL)
	{
		err("NULL detect");
		return NULL;
	}

	value = readsys(aspectchoicesdev, 1);
	if(value == NULL)
	{
		err("NULL detect");
		return NULL;
	}

	value = convertspacetolf(value);

	return value;
}

char* getaspect()
{
	char *aspectdev = NULL;
	char *value = NULL;

	aspectdev = getconfig("aspectdev", NULL);

	if(aspectdev == NULL)
	{
		err("NULL detect");
		return NULL;
	}

	value = readsys(aspectdev, 1);
	if(value == NULL)
	{
		err("NULL detect");
		return NULL;
	}

	return value;
}

int setaspect(char* value)
{
	char* aspectdev;
	int ret = 0;

	aspectdev = getconfig("aspectdev", NULL);

	if(aspectdev != NULL && value != NULL)
	{
		debug(10, "set %s to %s", aspectdev, value);

		if(!ostrncmp("set_", value, 4))
		{
			char* tmpstr = NULL;
			tmpstr = ostrcat(value, NULL, 0, 0);
			tmpstr = string_replace("set_", "", tmpstr, 1);
			debug(10, "change %s to %s", aspectdev, tmpstr);
			ret = writesys(aspectdev, tmpstr, 0);
			if(ret == 0) addconfig("av_aspect", tmpstr);
			free(tmpstr); tmpstr = NULL;
			return ret;
		}
		else
		{
			ret = writesys(aspectdev, value, 0);

			if(ret == 0)
			{
				addconfig("av_aspect", value);

				//set policy new after change aspect
				char* tmpstr = getpolicy();
				setpolicy(tmpstr);
				free(tmpstr); tmpstr = NULL;
			}
		}
		return ret;
	}

	return 0;
}

char* getvideomodechoices()
{
	char *videomodechoicesdev = NULL;
	char *value = NULL;

	videomodechoicesdev = getconfig("videomodechoicesdev", NULL);

	if(videomodechoicesdev == NULL)
	{
		err("NULL detect");
		return NULL;
	}

	value = readsys(videomodechoicesdev, 1);
	if(value == NULL)
	{
		err("NULL detect");
		return NULL;
	}

	value = string_replace("pal", NULL, value, 1);
	value = string_replace("  ", " ", value, 1);

	if(status.mcaktiv == 0)
	{
		value = string_replace("1080p60", NULL, value, 1);
		value = string_replace("  ", " ", value, 1);
		value = string_replace("1080p59", NULL, value, 1);
		value = string_replace("  ", " ", value, 1);
		value = string_replace("1080p30", NULL, value, 1);
		value = string_replace("  ", " ", value, 1);
		value = string_replace("1080p29", NULL, value, 1);
		value = string_replace("  ", " ", value, 1);
		value = string_replace("1080p25", NULL, value, 1);
		value = string_replace("  ", " ", value, 1);
		value = string_replace("1080p24", NULL, value, 1);
		value = string_replace("  ", " ", value, 1);
		value = string_replace("1080p23", NULL, value, 1);
		value = string_replace("  ", " ", value, 1);
		value = string_replace("1080i60", NULL, value, 1);
		value = string_replace("  ", " ", value, 1);
		value = string_replace("1080i59", NULL, value, 1);
		value = string_replace("  ", " ", value, 1);
		value = string_replace("1080i60", NULL, value, 1);
		value = string_replace("  ", " ", value, 1);
		value = string_replace("720p60", NULL, value, 1);
		value = string_replace("  ", " ", value, 1);
		value = string_replace("576p50", NULL, value, 1);
		value = string_replace("  ", " ", value, 1);
		value = string_replace("1280x1024_75", NULL, value, 1);
		value = string_replace("  ", " ", value, 1);
		value = string_replace("1280x1024_70", NULL, value, 1);
		value = string_replace("  ", " ", value, 1);
		value = string_replace("1280x1024_60", NULL, value, 1);
		value = string_replace("  ", " ", value, 1);
		value = string_replace("1600x1200_60", NULL, value, 1);
		value = string_replace("  ", " ", value, 1);
		value = string_replace("1024x768_100", NULL, value, 1);
		value = string_replace("  ", " ", value, 1);
		value = string_replace("1024x768_90", NULL, value, 1);
		value = string_replace("  ", " ", value, 1);
		value = string_replace("1024x768_75", NULL, value, 1);
		value = string_replace("  ", " ", value, 1);
		value = string_replace("1024x768_70", NULL, value, 1);
		value = string_replace("  ", " ", value, 1);
		value = string_replace("1024x768_60", NULL, value, 1);
		value = string_replace("  ", " ", value, 1);
	}

	value = strstrip(value);
	value = convertspacetolf(value);

	return value;
}

char* getmode3dchoices()
{
	char *mode3dchoicesdev = NULL;
	char *value = NULL;

	mode3dchoicesdev = getconfig("mode3dchoicesdev", NULL);

	if(mode3dchoicesdev == NULL)
	{
		return ostrcat("off\nsbs\ntab", "", 0, 0);
	}

	value = readsys(mode3dchoicesdev, 1);
	if(value == NULL)
	{
		err("NULL detect");
		return NULL;
	}

	value = convertspacetolf(value);

	return value;
}

char* getmode3d()
{
	char *mode3ddev = NULL;
	char *value = NULL;

	mode3ddev = getconfig("mode3ddev", NULL);

	if(mode3ddev == NULL)
	{
		err("NULL detect");
		return NULL;
	}

	value = readsys(mode3ddev, 1);
	if(value == NULL)
	{
		err("NULL detect");
		return NULL;
	}

	return value;
}

int setmode3d(char* value)
{
	char* mode3ddev;
	int ret = 0;

	mode3ddev = getconfig("mode3ddev", NULL);

	if(mode3ddev != NULL && value != NULL)
	{
		debug(100, "set %s to %s", mode3ddev, value);
		ret = writesys(mode3ddev, value, 0);
		if(ret == 0)
		{
			addconfig("av_mode3d", value);
			char* res = getvideomode();
			if(res != NULL)
			{
				setvideomode("576p", 0);
				sleep(1);
				setvideomode(res, 0);
			}
			free(res); res = NULL;
		}
		return ret;
	}

	return 0;
}

char* getvideomode()
{
	char *videomodedev = NULL;
	char *value = NULL;

	videomodedev = getconfig("videomodedev", NULL);

	if(videomodedev == NULL)
	{
		err("NULL detect");
		return NULL;
	}

	value = readsys(videomodedev, 1);
	if(value == NULL)
	{
		err("NULL detect");
		return NULL;
	}

	return value;
}

void switchvideomode()
{
	int rcret = 0;
	char* tmpstr = NULL, *tmpstr2 = NULL, *tmpstr3 = NULL;
	tmpstr = getvideomode();
	struct skin* playpolicy = getscreen("playpolicy");
	tmpstr2 = getcolorformat(2);

	if(tmpstr != NULL)
	{
		if((ostrcmp("pal", tmpstr) == 0 || ostrncmp("576", tmpstr, 3) == 0) && ostrncmp("yuv", tmpstr2, 3) == 0)
		{
			setvideomode("720p50", 0);
			changefbresolution("720p50", 0);
			changetext(playpolicy, "720p50 (rgb)");
			writevfdmenu("720p50 (rgb)");
			setcolorformat("rgb", 1);
			unlink("/var/etc/.scart");
		}
		else if((ostrcmp("pal", tmpstr) == 0 || ostrncmp("576", tmpstr, 3) == 0) && ostrncmp("rgb", tmpstr2, 3) == 0)
		{
			setvideomode("720p50", 0);
			changefbresolution("720p50", 0);
			changetext(playpolicy, "720p50 (yuv)");
			writevfdmenu("720p50 (yuv)");
			setcolorformat("yuv", 1);
			unlink("/var/etc/.scart");
		}
		else if(ostrncmp("720", tmpstr, 3) == 0)
		{
			setvideomode("1080i50", 0);
			changefbresolution("1080i50", 0);
			tmpstr3 = ostrcat("1080i50 (", tmpstr2, 0, 0);
			tmpstr3 = ostrcat(tmpstr3, ")", 1, 0);
			changetext(playpolicy, tmpstr3);
			writevfdmenu(tmpstr3);
			unlink("/var/etc/.scart");
		}
		else if(ostrncmp("1080", tmpstr, 4) == 0)
		{
			setvideomode("576i50", 0);
			changefbresolution("576i50", 0);
			changetext(playpolicy, "576i50 (rgb)");
			writevfdmenu("576i50 (rgb)");
			setcolorformat("rgb", 1);
			writesys("/var/etc/.scart", "0", 0);
		}
		/*
		int ret = textbox(_("Message"), _("Is this Videomode ok ?"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 1000, 200, 10, 0);
		if(ret == 1 || ret == 2)
		{
			setvideomode(tmpstr, 0);
			changefbresolution(tmpstr, 0);
		}
		*/
		drawscreen(playpolicy, 0, 0);
		while(1)
		{
			rcret = waitrc(playpolicy, 5000, 0);
			break;
		}
		clearscreen(playpolicy);
	}
	free(tmpstr);
	free(tmpstr2);
	free(tmpstr3);
}

//flag 0: write videomode to config
//flag 1: don't write videomode to config
int setvideomode(char* value, int flag)
{
	char* videomodedev;
	char* videomodedev_24hz = NULL,*videomodedev_50hz = NULL, *videomodedev_60hz = NULL;
	int ret = 0;

	videomodedev = getconfig("videomodedev", NULL);
	videomodedev_24hz = ostrcat(getconfig("videomodedev", NULL), "_24hz", 0, 0);
	videomodedev_50hz = ostrcat(getconfig("videomodedev", NULL), "_50hz", 0, 0);
	videomodedev_60hz = ostrcat(getconfig("videomodedev", NULL), "_60hz", 0, 0);

	if(videomodedev != NULL && value != NULL)
	{
		if(file_exist(videomodedev_24hz))
		{
			debug(100, "set %s to %s", videomodedev_24hz, value);
			ret = writesys(videomodedev_24hz, value, 0);
			free(videomodedev_24hz), videomodedev_24hz = NULL;
		}
		if(file_exist(videomodedev_50hz))
		{
			debug(100, "set %s to %s", videomodedev_50hz, value);
			ret = writesys(videomodedev_50hz, value, 0);
			free(videomodedev_50hz), videomodedev_50hz = NULL;
		}
		if(file_exist(videomodedev_60hz))
		{
			debug(100, "set %s to %s", videomodedev_60hz, value);
			ret = writesys(videomodedev_60hz, value, 0);
			free(videomodedev_60hz), videomodedev_60hz = NULL;
		}
		if(file_exist(videomodedev))
		{
			debug(100, "set %s to %s", videomodedev, value);
			ret = writesys(videomodedev, value, 0);
		}
		if(ret == 0 && flag == 0) addconfig("av_videomode", value);
		return ret;
	}

	return 0;
}

//flag 0 = hdmi
//flag 1 = scart
int setcolorformat(char* value, int flag)
{
	char* colorformatdev;
	int ret = 0;

	colorformatdev = getconfig("colorformatdev", NULL);

	if(colorformatdev != NULL && value != NULL)
	{
		debug(100, "set %s to %s", colorformatdev, value);
		ret = writesys(colorformatdev, value, 0);
		if(ret == 0)
		{
			if(flag == 0)
				addconfig("av_colorformat", value);
			else
				addconfig("av_colorformatscart", value);
		}
		return ret;
	}

	return 0;
}

char* getcolorformat(int line)
{
	char *colorformatdev = NULL;
	char *value = NULL;

	colorformatdev = getconfig("colorformatdev", NULL);

	if(colorformatdev == NULL)
	{
		err("NULL detect");
		return NULL;
	}

	value = readsys(colorformatdev, line);
	if(value == NULL)
	{
		err("NULL detect");
		return NULL;
	}

	return value;
}

int setaudiosource(char* value)
{
	char* audiosourcedev;
	int ret = 1;

	audiosourcedev = getconfig("audiosourcedev", NULL);

	if(audiosourcedev != NULL && value != NULL)
	{
		debug(100, "set %s to %s", audiosourcedev, value);
		ret = writesys(audiosourcedev, value, 0);
		if(ret == 0) addconfig("av_audiosource", value);
		return ret;
	}

	return 0;
}

char* getaudiosource()
{
	char *audiosourcedev = NULL;
	char *value = NULL;

	audiosourcedev = getconfig("audiosourcedev", NULL);

	if(audiosourcedev == NULL)
	{
		err("NULL detect");
		return NULL;
	}

	value = readsys(audiosourcedev, 1);
	if(value == NULL)
	{
		err("NULL detect");
		return NULL;
	}

	return value;
}

char* getsataswitch()
{
	char *sataswitchdev = NULL;
	char *value = NULL;

	sataswitchdev = getconfig("sataswitchdev", NULL);

	if(sataswitchdev == NULL)
	{
		err("NULL detect");
		return NULL;
	}

	value = readsys(sataswitchdev, 1);
	if(value == NULL)
	{
		err("NULL detect");
		return NULL;
	}

	return value;
}

int setsataswitch(char* value)
{
	char* sataswitchdev;
	int ret = 1;

	sataswitchdev = getconfig("sataswitchdev", NULL);

	if(sataswitchdev != NULL && value != NULL)
	{
		debug(100, "set %s to %s", sataswitchdev, value);
		ret = writesys(sataswitchdev, value, 0);
		return ret;
	}

	return 0;
}

int setprogress(int value)
{
	char *progressdev;

	progressdev = getconfig("progressdev", NULL);

	if(progressdev != NULL)
	{
		debug(100, "set %s to %d",progressdev, value);
		if(checkbox("DM7020HD") == 1 || checkbox("DM7020HDV2") == 1 || checkbox("DM900") == 1 || checkbox("DM920") == 1 || checkbox("WETEK") == 1 || checkbox("DM520") == 1 || checkbox("DM525") == 1)
			return writesysint(progressdev, value, 1);
		else
			return writesysint(progressdev, value, 0);
	}

	return 0;
}

int setmute(int value)
{
	char* mutedev;
	int tmpvol, ret = 0;

	//don't set mute 2x
	if(value == 1 && status.mute == 1) return 0;

//	printf("mute value: %d\n", value);

	if(value == 2)
	{
		tmpvol = getvol();
		tmpvol = tmpvol * 50 / 100;
		status.mute = value;
		setvol(tmpvol);
	}
	else
	{
		mutedev = getconfig("mutedev", NULL);

		if(mutedev != NULL)
		{
			debug(100, "set %s to %d", mutedev, value);
			//if(status.volautochangevalue != 0 && value == 0) setvol(getvol());
#ifdef MIPSEL
			struct dvbdev *audionode = NULL;
			int openaudio = 0;
			if(status.aktservice != NULL)
			{
				if(status.aktservice->audiodev == NULL)
				{
					audionode = audioopen(0); //we must open the audio device for set mute in external player
					openaudio = 1;
				}
				else
					audionode = status.aktservice->audiodev;
			}
			ret = audiosetmute(audionode, value);
			if(openaudio == 1)
				audioclose(audionode, -1);
#else
			ret = writesysint(mutedev, value, 0);
#endif

			if(ret == 0) status.mute = value;
			if(status.volautochangevalue != 0 && value == 0) setvol(getvol());
//			if(value == 1)
//			{
//				printf("mute\n");
//				system("amixer -c 1 set HDMI mute &");
//				system("amixer -c 1 set Analog mute &");
//				system("amixer -c 1 set SPDIF mute &");
//				sleep(1);
//			}
//			else
//			{
//				printf("unmute\n");
//				system("amixer -c 1 set HDMI unmute &");
//				system("amixer -c 1 set Analog unmute &");
//				system("amixer -c 1 set SPDIF unmute &");
//			}

			return ret;
		}
	}
	return 0;
}

int setvol(int value)
{
	char* voldev;
	int ret = 0, tmpvol = value;

	voldev = getconfig("voldev", NULL);

	if(voldev != NULL)
	{
		if(value > 100) value = 100;
		if(value < 0) value = 0;
		if(status.volautochangevalue != 0 && value != 0)
		{
			if(status.volautochange == 0)
				value = value - (status.volautochangevalue * value / 100);
		}
		if(checkchipset("3798MV200") != 1)
			value = 63 - value * 63 / 100;
		debug(100, "set %s to %d", voldev, value);
		ret = 0;
		if(status.mute == 1)
			status.volmute = value;
		else
		{
			status.volmute = -1;
			if(file_exist(voldev))
				ret = writesysint(voldev, value, 0);
			else
				ret = 0;

			if(checkchipset("3798MV200") == 1)
			{
				if(ret == 0)
					status.volume = value;

				if(ret == 0 && status.mute != 2) addconfigint("vol", tmpvol);
				return ret;
			}
#ifdef MIPSEL
			struct dvbdev *audionode = NULL;
			int openaudio = 0;

			if(ret == 0 && status.aktservice != NULL)
			{
				if(status.aktservice->audiodev == NULL)
				{
					audionode = audioopen(0); //we must open the audio device for change volume in external player
					openaudio = 1;
				}
				else
					audionode = status.aktservice->audiodev;

				if(ret == 0 && audionode != NULL)
					ret = setmixer(audionode, value, value);

				if(ret == 0)
					status.volume = value;

				if(openaudio == 1)
					audioclose(audionode, -1);
			}
#else
			if(ret == 0 && status.aktservice != NULL)
				ret = setmixer(status.aktservice->audiodev, value, value);
#endif
		}
		if(ret == 0 && status.mute != 2) addconfigint("vol", tmpvol);
		return ret;
	}

	return 0;
}

int getvol()
{
	char *voldev = NULL;
	char *value = NULL;
	int tmpvol = -1;

	voldev = getconfig("voldev", NULL);

	if(voldev == NULL)
	{
		err("NULL detect");
		return 0;
	}
	if(status.volmute == -1)
	{
		if(file_exist(voldev))
			value = readsys(voldev, 1);
	}
	else
		tmpvol = status.volmute;
	if((value == NULL && status.volume == -1) && tmpvol == -1)
	{
		err("NULL detect");
		return 0;
	}
	if(status.volmute == -1)
	{
		if(file_exist(voldev))
			tmpvol = atoi(value);
		else
			tmpvol = status.volume;
	}
	free(value);
	if(checkchipset("3798MV200") != 1)
		tmpvol = 100 - tmpvol * 100 / 63;
	if(status.volautochangevalue != 0)
	{
		if(status.volautochange == 0 && status.volautochangevalue < 100)
			tmpvol = tmpvol + ((tmpvol * status.volautochangevalue) / (100 - status.volautochangevalue));
	}
	return tmpvol;
}

void setdebuglevel()
{
	debug_level = getconfigint("debuglevel", NULL);
	debug(0, "set debug level to %d", debug_level);
}

char* getxmlentry(char *line, char *searchstr)
{
	char *buf = NULL, *buf1 = NULL, *buf2 = NULL;

	buf = ostrstr(line, searchstr);
	if(buf == NULL)
		return NULL;

	buf = buf + strlen(searchstr);
	if(buf[0] == '"')
	{
		buf = buf + 1;
		if(buf[0] == '\\')
		{
			buf = buf + 1;
			if(buf[0] == '"')
				buf2 = strchr(buf+1, '"');
			else
				buf2 = strchr(buf, '"');
		}
		else
			buf2 = strchr(buf, '"');
		if(buf2 == NULL)
		{
			err("strchr returns NULL");
			return NULL;
		}
		buf1 = strndup(buf, buf2 - buf);
		if(buf1 == NULL)
		{
			err("strndup failed");
			return NULL;
		}
	}
	else if(buf[0] == '\'')
	{
		buf = buf + 1;
		buf2 = strchr(buf, '\'');
		if(buf2 == NULL)
		{
			err("strchr returns NULL");
			return NULL;
		}
		buf1 = strndup(buf, buf2 - buf);
		if(buf1 == NULL)
		{
			err("strndup failed");
			return NULL;
		}
	}
	else
	{
		buf2 = strchr(buf, ' ');
		if(buf2 == NULL)
		{
			buf2 = ostrstr(buf, "/>");
			if(buf2 == NULL)
			{
				buf2 = strchr(buf, '>');
				if(buf2 == NULL)
				{
					err("strchr returns NULL");
					return NULL;
				}
			}
		}
		buf1 = strndup(buf, buf2 - buf);
		if(buf1 == NULL)
		{
			err("strndup failed");
			return NULL;
		}
	}

	return buf1;
}

char* readbintomem(const char* filename, size_t size)
{
	FILE *fd = NULL;
	char *fileline = NULL;

	fileline = calloc(1, size + 1);
	if(fileline == NULL)
	{
		err("no mem");
		return NULL;
	}

	fd = fopen(filename, "rb");
	if(fd == NULL)
	{
		perr("can't open %s", filename);
		free(fileline);
		return NULL;
	}

	fread(fileline, size, 1, fd);

	fclose(fd);
	return fileline;
}

char* readfiletomem(const char* filename, int flag)
{
	FILE *fd = NULL;
	char *fileline = NULL, *buf = NULL, *tmpbuf = NULL;
	int bufsize = 0, bufoldsize = 0;

	fileline = malloc(MINMALLOC);
	if(fileline == NULL)
	{
		err("no mem");
		return NULL;
	}

	fd = fopen(filename, "r");
	if(fd == NULL)
	{
		perr("can't open %s", filename);
		free(fileline);
		return NULL;
	}

	while(fgets(fileline, MINMALLOC, fd) != NULL)
	{
		if(flag == 1)
			if(fileline[0] == '#' || fileline[0] == '\n')
				continue;

		bufoldsize = bufsize;
		bufsize += strlen(fileline);
		tmpbuf = buf;	buf = realloc(buf, bufsize + 1);
		if(buf == NULL)
		{
			err("no mem");
			free(fileline);
			free(tmpbuf);
			fclose(fd);
			return NULL;
		}

		sprintf(buf + bufoldsize, "%s", fileline);
	}

	free(fileline);
	fclose(fd);
	return buf;
}

char* readeittomem(const char* filename)
{
	unsigned char byte;
	FILE *fil = NULL;
	char *zeichen = NULL, *buf = NULL, *buf1 = NULL, *tmpbuf1 = NULL;
	int buf1size = 0, buf1oldsize = 0;
	int Beschreibung;
	int len;

	zeichen = malloc(255);
	if(zeichen == NULL)
	{
		err("no mem");
		return NULL;
	}
	buf = malloc(255);
	if(buf == NULL)
	{
		free(zeichen);
		err("no mem");
		return NULL;
	}

	fil = fopen(filename, "r");
	if(fil == NULL)
	{
		err("can't open %s", filename);
		free(zeichen);
		free(buf);
		return NULL;
	}
	Beschreibung = 0;
	fseek(fil, 12, SEEK_SET); //ersten 12 Byte nicht relevant
	while(!feof(fil))
	{
		byte = fgetc(fil);

		if(byte == 0x4D)
		{
			fseek(fil, 4,SEEK_CUR);
			byte = fgetc(fil);
			len = byte + 0;
			byte = fgetc(fil);
			fgets(zeichen, len, fil);
			if(byte != 0x05)
				sprintf(buf, "%c%s\n", byte, zeichen);
			else
				sprintf(buf, "%s\n", zeichen);

			buf1oldsize = buf1size;
			buf1size += strlen(buf);
			tmpbuf1 = buf1; buf1 = realloc(buf1, buf1size + 1);
			if(buf1 == NULL)
			{
				err("no mem");
				free(zeichen);
				free(buf);
				free(tmpbuf1);
				fclose(fil);
				return NULL;
			}
			sprintf(buf1 + buf1oldsize, "%s", buf);

 			//printf("T %s\n", zeichen);
			byte = fgetc(fil);
			len = byte + 0;
			byte = fgetc(fil);
			fgets(zeichen, len, fil);
			if(byte != 0x05)
				sprintf(buf,"%c%s\n\n", byte, zeichen);
			else
				sprintf(buf,"%s\n\n", zeichen);

			buf1oldsize = buf1size;
			buf1size += strlen(buf);
			tmpbuf1 = buf1; buf1 = realloc(buf1, buf1size + 1);
			if(buf1 == NULL)
			{
				err("no mem");
				free(zeichen);
				free(buf);
				free(tmpbuf1);
				fclose(fil);
				return NULL;
			}
			sprintf(buf1 + buf1oldsize, "%s", buf);

		}
		else if(byte == 0x4E)
		{
			fseek(fil, 6, SEEK_CUR);
			byte = fgetc(fil);
			len = byte;
			byte = fgetc(fil);
			fgets(zeichen, len, fil);
			if(Beschreibung == 0)
			{
				if(byte != 0x05)
					sprintf(buf, "%c%s", byte, zeichen);
				else
					sprintf(buf, "%s", zeichen);
				Beschreibung = 1;
			}
			else
			{
				if(byte != 0x05)
					sprintf(buf, "%c%s", byte, zeichen);
				else
					sprintf(buf, "%s", zeichen);
			}

			buf1oldsize = buf1size;
			buf1size += strlen(buf);
			tmpbuf1 = buf1; buf1 = realloc(buf1, buf1size + 1);
			if(buf1 == NULL)
			{
				err("no mem");
				free(zeichen);
				free(buf);
				free(tmpbuf1);
				fclose(fil);
				return NULL;
			}
			sprintf(buf1 + buf1oldsize, "%s", buf);

		}
		else
		{
			byte = fgetc(fil);
			len= byte;
			fgets(zeichen, len + 1, fil);
		}
	}
	free(zeichen);
	free(buf);
	fclose(fil);
	return buf1;
}

char* command(char* input)
{
	int maxlen = 0, pos = 0;
	char* tmpstr = NULL, *fileline = NULL;
	FILE *iopipe = NULL;

	if(input == NULL) return NULL;

	fileline = malloc(MINMALLOC);
	if(fileline == NULL)
	{
		err("no mem");
		return NULL;
	}

	if((iopipe = popen(input, "r")) == NULL)
	{
		free(fileline);
		return NULL;
	}

	while(!feof(iopipe))
	{
		if(fgets(fileline, MINMALLOC, iopipe) != NULL)
			ostrcatbig(&tmpstr, fileline, &maxlen, &pos);
	}

	if(pos > 0)
	{
		char* tmp = tmpstr;
		tmpstr = realloc(tmpstr, pos + 1);
		if(tmpstr == NULL)
			tmpstr = tmp;
	}

	free(fileline);
	pclose(iopipe);
	return tmpstr;
}

char* string_tolower(char *str)
{
/*
	char *p1 = str;

	if(str == NULL) return NULL;

	while(*p1 != '\0')
		*p1++ = tolower(*p1);

	return str;
*/
	if(str == NULL) return NULL;

	int i=0;
	while (str[i])
	{
		str[i] = tolower(str[i]);
		i++;
	}
	return str;
}

char* string_toupper(char *str)
{
/*
	char *p1 = str;

	if(str == NULL) return NULL;

	while(*p1 != '\0')
		*p1++ = toupper(*p1);

*/
	if(str == NULL) return NULL;

	int i=0;
	while (str[i])
	{
		str[i] = toupper(str[i]);
		i++;
	}
	return str;
}

char* stringreplacecharonce(char *str, char c1, char c2)
{
	char *p1 = str;

	if(str == NULL) return NULL;

	while(*p1 != '\0')
	{
		if(*p1 == c1)
		{
			*p1 = c2;
			break;
		}
		p1++;
	}

	return str;
}

char* stringreplacechar(char *str, char c1, char c2)
{
	char *p1 = str;

	if(str == NULL) return NULL;

	while(*p1 != '\0')
	{
		if(*p1 == c1) *p1 = c2;
		p1++;
	}

	return str;
}


char* string_removechar(char *str)
{
	char *p1 = str;

	if(str == NULL) return NULL;

	while(*p1 != '\0')
	{
		if(*p1 == '.') *p1 = ' ';
		else if(*p1 == '-') *p1 = ' ';
		else if(*p1 == '_') *p1 = ' ';
		else if(*p1 == '/') *p1 = ' ';
		p1++;
	}

	return str;
}

char* string_withchars2return(char *str)
{
	char *p1 = str;

	if(str == NULL) return NULL;

	while(*p1 != '\0')
	{
		if(*p1 == ' ') *p1 = '\n';
		p1++;
	}

	return str;
}

char* string_remove_whitechars(char *text)
{
	char *p1 = text, *p2 = text;

	if(text == NULL) return NULL;

	while(*p1 != '\0')
	{
		if(*p1 == ' ')
			++p1;
		else
			*p2++ = *p1++;
	}
	*p2 = '\0';

	return text;
}

char* strstrip(char *text)
{
	char* tmpstr = text;

	if(text == NULL) return NULL;
	int len = strlen(text);

	while(isspace(tmpstr[len - 1])) tmpstr[--len] = '\0';
	while(*tmpstr && isspace(*tmpstr)) ++tmpstr, --len;

	if(text != tmpstr) memmove(text, tmpstr, len + 1);

	return text;
}

char* string_strip_whitechars(char *text)
{
	char *p1 = text, *p2 = text;

	if(text == NULL)
		return NULL;

	while(*p1 != '\0')
	{
		if(*p1 == ' ' && *(p1 + 1) == ' ')
			++p1;
		else
			*p2++ = *p1++;
	}
	*p2 = '\0';

	return text;
}

char* string_replace_all(char *search, char *replace, char *string, int free1)
{
	char* tmpstr = NULL;
	char* searchpos = NULL;

	if(string == NULL || search == NULL)
	{
		tmpstr = ostrcat(tmpstr, string, 1, 0);
		if(free1 == 1) free(string);
		return tmpstr;
	}

	searchpos = strstr(string, search);
	if(searchpos == NULL)
	{
		tmpstr = ostrcat(tmpstr, string, 1, 0);
		if(free1 == 1) free(string);
		return tmpstr;
	}

	int count = 0;
	int stringlen = strlen(string);
	int searchlen = strlen(search);
	int replacelen = strlen(replace);

	while(searchpos != NULL)
	{
		count++;
		searchpos = strstr(searchpos + searchlen, search);
	}

	int len = stringlen - (searchlen * count) + (replacelen * count);
	tmpstr = calloc(1, len + 1);
	if(tmpstr == NULL)
	{
		err("no mem");
		tmpstr = ostrcat(tmpstr, string, 1, 0);
		if(free1 == 1) free(string);
		return tmpstr;
	}

	len = 0;
	char* str = string;
	char* tmp = tmpstr;
	searchpos = strstr(str, search);
	while(searchpos != NULL)
	{
		len = searchpos - str;
		memcpy(tmp, str, len);
		memcpy(tmp + len, replace, replacelen);
		tmp += len + replacelen;
		str += len + searchlen;
		searchpos = strstr(str, search);
	}
	memcpy(tmp, str, strlen(str));

	if(free1 == 1) free(string);

	return tmpstr;
}

char* string_replace_remove_last_chars(char *search, char *replace, char *string, int free1)
{
	char* searchpos = NULL;
	char* tmpstr = NULL;

	if(string == NULL || search == NULL)
	{
		tmpstr = ostrcat(tmpstr, string, 1, 0);
		if(free1 == 1) free(string);
		return tmpstr;
	}

	searchpos = ostrstr(string, search);

	if(searchpos == NULL)
	{
		tmpstr = ostrcat(tmpstr, string, 1, 0);
		if(free1 == 1) free(string);
		return tmpstr;
	}

	tmpstr = strndup(string, searchpos - string);
	if(replace != NULL)
		tmpstr = ostrcat(tmpstr, replace, 1, 0);

	if(free1 == 1) free(string);

	return tmpstr;
}

char* string_replace(char *search, char *replace, char *string, int free1)
{
	char* searchpos = NULL;
	char* tmpstr = NULL;

	if(string == NULL || search == NULL)
	{
		tmpstr = ostrcat(tmpstr, string, 1, 0);
		if(free1 == 1) free(string);
		return tmpstr;
	}

	searchpos = ostrstr(string, search);

	if(searchpos == NULL)
	{
		tmpstr = ostrcat(tmpstr, string, 1, 0);
		if(free1 == 1) free(string);
		return tmpstr;
	}

	tmpstr = strndup(string, searchpos - string);
	if(replace != NULL)
		tmpstr = ostrcat(tmpstr, replace, 1, 0);
	tmpstr = ostrcat(tmpstr, string + (searchpos - string) + strlen(search), 1, 0);

	if(free1 == 1) free(string);

	return tmpstr;
}

//flag 0: search full str
//flag 1: search only end of string
char* ostrrstrcase(char* str, char* search, int len, int flag)
{
	int slen = 0;
	char* tmpstr = NULL;

	if(str == NULL || search == NULL) return NULL;

	if(len == -1) len = strlen(str);
	slen = strlen(search);
	if(slen > len) return NULL;

	for(tmpstr = str + len - slen; tmpstr >= str; tmpstr--)
	{
		if(strncasecmp(tmpstr, search, slen) == 0)
			return tmpstr;
		if(flag == 1) return NULL;
	}

	return NULL;
}

//flag 0: search full str
//flag 1: search only end of string
char* ostrrstr(char* str, char* search, int len, int flag)
{
	int slen = 0;
	char* tmpstr = NULL;

	if(str == NULL || search == NULL) return NULL;

	if(len == -1) len = strlen(str);
	slen = strlen(search);
	if(slen > len) return NULL;

	for(tmpstr = str + len - slen; tmpstr >= str; tmpstr--)
	{
		if(strncmp(tmpstr, search, slen) == 0)
			return tmpstr;
		if(flag == 1) return NULL;
	}

	return NULL;
}

char* ostrstr(char* str, char* search)
{
	char* ret = NULL;

	if(str == NULL || search == NULL) return NULL;
	ret = strstr(str, search);

	return ret;
}

int file_exist(char* filename)
{
	if(access(filename, F_OK) == 0)
		return 1;
	else
		return 0;
}

char* string_newline(char* str)
{
	if(str == NULL) return NULL;

	int size = strlen(str);

	if(str[size - 1] == '\n')
		str[size - 1] = '\0';

	return str;
}

char* string_quote(char* str)
{
	char* tmpstr = NULL;

	tmpstr = ostrcat("\"", str, 0, 0);
	tmpstr = ostrcat(tmpstr, "\"", 1, 0);

	return tmpstr;
}

struct splitstr* strsplit(char *str, char *tok, int* count)
{
	char *tmpstr = NULL;
	struct splitstr *array = NULL, *tmparray = NULL;
	*count = 0;

	if(str == NULL || tok == NULL)
		return NULL;

	tmpstr = strtok(str, tok);
	while(tmpstr != NULL)
	{
		*count = *count + 1;
		tmparray = array; array = (struct splitstr*)realloc(array, sizeof(struct splitstr*) * (*count));
		if(array == NULL)
		{
			err("no mem");
			free(tmparray);
			return NULL;
		}

		(&array[(*count) - 1])->part = tmpstr;
		tmpstr = strtok(NULL, tok);
	}

	return array;
}

char* string_shortname(char *tmpfilename, int mode)
{
	debug(50, "in %s", tmpfilename);

//	char replacelist[] = "avi mkv x264 se disc0 disc1 disc2 disc3 disc4 0disc 1disc 2disc 3disc 4disc season0 season1 season2 season3 season4 season5 season6 season7 season8 season9 hdtv 720p 1080i 1080p uncut cd0 cd1 cd2 cd3 cd4 cd5 cd6 cd7 cd8 cd9 dvd0 dvd1 dvd2 dvd3 dvd4 ac3d ac3 bdrip bluray cam camrip complete custom cut dc directors dl doku dts dvdr dvdrip dvdscr dvdscreener extended french finnish german hd hddvd hddvdrip hdtv int internal int ld limited multi multisubs nordic ntsc pal pl r1 r5 recut remastered repack rip screener se see special.edition sse stv subbed swedish staffel tc telecine telesync ts unrated ws xxx italian";
	char* str = NULL;

	if(mode==1)
	{
		char* replacelist = "avi mkv x264 xvid se uncut ac3d ac3hd ac3 bdrip bluray cam camrip complete custom cut dc directors dl doku dts dvdr dvdrip dvdscr dvdscreener ecc extended french finnish german hd hddvd hddvdrip hdtv int internal int ld limited multi multisubs nordic ntsc pal pl r1 r5 recut remastered repack rip screener se see sse stv subbed swedish staffel tc telecine telesync ts unrated ws xxx italian";
		str = ostrcat(str, replacelist, 1, 0);
	}
	else if(mode==2)
	{
		char* replacelist = "1080i 1080p 720p ac3 ac3d ac3hd avi bdrip bluray cam camrip complete custom cut dat dc directors divx dl doku dts dvdr dvdrip dvdscr dvdscreener ecc ed extended finnish flv french german hd hddvd hddvdrip hdtv int internal ld limited linedubbed m2ts m4v md mkv mov mp4 mpeg mpg mts multi multisubs nordic ntsc pal pl proper r1 r3 r5 recut remastered repack rip rm screener se sse staffel stv subbed svcd swedish tc telecine telesync trp ts uc uncut unrated ur vcd vdr vob wmv ws x264 xvid xxx";
		str = ostrcat(str, replacelist, 1, 0);
	}
	else
	{
		char* replacelist = "disc0 disc1 disc2 disc3 disc4 0disc 1disc 2disc 3disc 4disc season0 season1 season2 season3 season4 season5 season6 season7 season8 season9 hdtv 720p 1080i 1080p cd0 cd1 cd2 cd3 cd4 cd5 cd6 cd7 cd8 cd9 dvd0 dvd1 dvd2 dvd3 dvd4";
		str = ostrcat(str, replacelist, 1, 0);
	}

	char* replace = NULL;
	struct splitstr* ret1 = NULL;
	int count = 0;
	int i = 0;
	ret1 = strsplit(str, " ", &count);
	int max = count - 1;
	int first = 1;

	for(i = 0; i < max; i++)
	{
		struct splitstr* ret2 = NULL;
		int count2 = 0;
		int j = 0;
		char *tmpstr = NULL;
		tmpstr = ostrcat(tmpstr, tmpfilename, 1, 0);
		ret2 = strsplit(tmpstr, " ,.-_", &count2);

		for(j = 0; j < count2; j++)
		{
			if(j > 0)
			{
				if(ostrcmp((&ret1[i])->part, (&ret2[j])->part) == 0)
				{
					if(mode==1)
					{
						tmpfilename = string_replace((&ret2[j])->part, replace, tmpfilename, 1);
						continue;
					}
					else if(mode==2)
					{
						tmpfilename = string_replace_remove_last_chars((&ret2[j])->part, replace, tmpfilename, 1);
						//break;
						continue;
					}
					else
					{
						tmpfilename = string_replace((&ret2[j])->part, replace, tmpfilename, 1);
						continue;
					}
				}
				else if(first == 1 && mode == 2)
				{
//					printf("zahl: %s\n", (&ret2[j])->part);
				 	int theCharacter = atoi((&ret2[j])->part);
					if(theCharacter != 0)
					{
//						printf("found zahl: %s\n", (&ret2[j])->part);
						if(theCharacter > 1800 && theCharacter < 2100)
						{
//							printf("found year: %s\n", (&ret2[j])->part);
							tmpfilename = string_replace_remove_last_chars((&ret2[j])->part, "", tmpfilename, 1);
							break;
						}
					}
				}
			}
		}
		free(ret2); ret2 = NULL;
		first = 0;
		free(tmpstr), tmpstr = NULL;
	}

	free(ret1); ret1 = NULL;
	free(replace); replace = NULL;
	free(str); str = NULL;

	debug(50, "out %s", tmpfilename);
	return tmpfilename;
}

char* get_uuid(char* device)
{
	debug(60, "in %s", device);
	char* cmd = NULL, *tmpstr = NULL;

	if(device == NULL) return NULL;
#ifdef MIPSEL
	cmd = ostrcat(cmd, "blkid -w /dev/null -c /dev/null -s UUID /dev/", 1, 0);
#else
	cmd = ostrcat(cmd, "/bin/blkid -w /dev/null -c /dev/null -s UUID /dev/", 1, 0);
#endif
	cmd = ostrcat(cmd, device, 1, 0);
	cmd = ostrcat(cmd, " | cut -d'\"' -f2", 1, 0);

	tmpstr = string_newline(command(cmd));

	if(ostrcmp(string_newline(tmpstr), "") == 0)
	{
		free(tmpstr); tmpstr = NULL;
	}

	debug(60, "out %s", cmd);
	free(cmd); cmd = NULL;
	return tmpstr;
}

char* get_label(char* device)
{
	debug(60, "in %s", device);
	char* cmd = NULL, *tmpstr = NULL;

	if(device == NULL) return NULL;
#ifdef MIPSEL
	cmd = ostrcat(cmd, "blkid -w /dev/null -c /dev/null -s LABEL /dev/", 1, 0);
#else
	cmd = ostrcat(cmd, "/bin/blkid -w /dev/null -c /dev/null -s LABEL /dev/", 1, 0);
#endif
	cmd = ostrcat(cmd, device, 1, 0);
	cmd = ostrcat(cmd, " | cut -d'\"' -f2", 1, 0);

	tmpstr = string_newline(command(cmd));

	if(tmpstr == NULL)
		tmpstr = ostrcat(tmpstr, "NONLABEL", 1, 0);

	if(ostrcmp(string_newline(tmpstr), "") == 0)
		tmpstr = ostrcat(tmpstr, "NONLABEL", 1, 0);

	debug(60, "out %s", cmd);
	free(cmd); cmd = NULL;
	return tmpstr;
}

char* get_filesystem(char* device)
{
	debug(60, "in %s", device);
	char* cmd = NULL, *tmpstr = NULL;

	if(device == NULL) return NULL;
#ifdef MIPSEL
	cmd = ostrcat(cmd, "blkid -w /dev/null -c /dev/null -s TYPE /dev/", 1, 0);
#else
	cmd = ostrcat(cmd, "/bin/blkid -w /dev/null -c /dev/null -s TYPE /dev/", 1, 0);
#endif
	cmd = ostrcat(cmd, device, 1, 0);
	cmd = ostrcat(cmd, " | cut -d'\"' -f2", 1, 0);

	tmpstr = string_newline(command(cmd));

	if(tmpstr == NULL)
		tmpstr = ostrcat(tmpstr, "NONTYPE", 1, 0);

	if(ostrcmp(string_newline(tmpstr), "") == 0)
		tmpstr = ostrcat(tmpstr, "NONTYPE", 1, 0);

	debug(60, "out %s", cmd);
	free(cmd); cmd = NULL;
	return tmpstr;
}

//flag 0: convert ip to 000.000.000.000
//flag 1: convert ip to 0.0.0.0
char* fixip(char* ipinput, int flag)
{
	debug(60, "in %s", ipinput);
	int ret = 0;
	char* ipout = NULL;
	unsigned char ip[4];

	ip[0] = 0;
	ip[1] = 0;
	ip[2] = 0;
	ip[3] = 0;

	if(ipinput == NULL) return NULL;
	ret = sscanf(ipinput, "%hhu.%hhu.%hhu.%hhu", &ip[0], &ip[1], &ip[2], &ip[3]);
	if(ret != 4) return NULL;

	ipout = malloc(16);
	if(ipout == NULL)
	{
		err("no mem");
		return NULL;
	}

	if(flag == 1)
		snprintf(ipout, 16, "%d.%d.%d.%d", ip[0], ip[1], ip[2], ip[3]);
	else
		snprintf(ipout, 16, "%03d.%03d.%03d.%03d", ip[0], ip[1], ip[2], ip[3]);

	return ipout;
}

void setfanspeed(int speed, int aktion)
{
	char* speedWert = NULL;
	char* speedSet = NULL;
	int base = 0;

	if(checkbox("UFS922") == 1)
		base = 100;
	else
		base = -15;


	if(speed < 0)
	{
		if(speed == -1)
			speedWert = getconfig("fanspeed", NULL);
		else
			speedWert = getconfig("fanspeedstandby", NULL);
		if(speedWert == NULL)
			speedSet = ostrcat(speedSet, oitoa(base + 70), 1, 1);
		else
			speedSet = ostrcat(speedSet, speedWert, 1, 0);
	}
	else
	{
		if(speed == 0)
			speedSet = ostrcat(speedSet, oitoa(base + 15), 1, 1);
		else if(speed == 25)
			speedSet = ostrcat(speedSet, oitoa(base + 30), 1, 1);
		else if(speed == 50)
			speedSet = ostrcat(speedSet, oitoa(base + 45), 1, 1);
		else if(speed == 75)
			speedSet = ostrcat(speedSet, oitoa(base + 55), 1, 1);
		else
			speedSet = ostrcat(speedSet, oitoa(base + 70), 1, 1);
	}

	if(checkbox("UFS922") == 1)
		writesys("/proc/stb/fan/fan_ctrl", speedSet, 1);
	else
		writesys("/proc/stb/fp/fan_pwm", speedSet, 1);

	if(aktion == 1)
		addconfig("fanspeed", speedSet);
	else if(aktion == 2)
		addconfig("fanspeedstandby", speedSet);

	free(speedSet); speedSet=NULL;
}

void setaktres()
{
	int m_width = 0;
	char* res = NULL;
	char* res_akt = NULL;
	char* res_sd = NULL;
	int count = 1;
	int sec = 0;

	m_lock(&status.setaktresmutex, 23);

	if(status.restimer == NULL)
		goto end;

	sec = (int)status.restimer->param1;

	if(sec > 0)
	{
		while(status.restimer->aktion == START && count <= (sec * 10))
		{
			usleep(100000);
			if(status.restimer->aktion != START)
				goto end;
			count++;
		}
	}
	if(status.restimer->aktion != START) goto end;

	if(videoreadqwidth(status.aktservice->videodev) == 0)
	{
		if(status.restimer->aktion != START) goto end;
		m_width = status.videosize.w;
		if(m_width == 720)
		{
			res_sd = getconfig("av_videomode_autores_sd", NULL);
			if(res_sd == NULL)
				res = ostrcat(res, "576i50", 1, 0);
			else
				res = ostrcat(res, res_sd, 1, 0);
		}
		else if (m_width == 1280)
			res = ostrcat(res, "720p50", 1, 0);
		else if (m_width == 1920)
			res = ostrcat(res, "1080i50", 1, 0);
		else
			m_width = 0;

		if(m_width > 0)
		{
			if(status.restimer->aktion != START) goto end;
			res_akt = getvideomode();
			if(status.restimer->aktion != START) goto end;
			if(ostrcmp(res_akt, res) != 0)
			{
				if(status.restimer->aktion != START) goto end;
				setvideomode(res, 1);
				changefbresolution(res, 1);
/*
				if(status.restimer->aktion != START) goto end;

				count = 0;
				while(status.restimer->aktion == START && count < 10)
				{
					usleep(100000);
					if(status.restimer->aktion != START)
						goto end;
					count++;
				}

				if(status.restimer->aktion != START) goto end;
*/
				screenautores(res, 5, 0);
			}
		}
	}
	else
	{
		if(status.restimer->aktion != START) goto end;
		if(status.servicetype == 0)
			textbox(_("Message"), _("ERROR cant read res"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 1000, 200, 5, 0);
	}

end:
	free(res); res = NULL;
	status.restimer = NULL;
	m_unlock(&status.setaktresmutex, 23);
	return;
}

char* gettimestamp()
{
	char* timestamp = NULL;
	struct timeval numtime;

	gettimeofday(&numtime, 0);
	timestamp = ostrcat(timestamp, olutoa(numtime.tv_sec), 1, 1);
	timestamp = ostrcat(timestamp, olutoa(numtime.tv_usec), 1, 1);

	return timestamp;
}

char* string_decode(char* input, int flag)
{
	if(input == NULL) return input;

	while(ostrstr(input, "\\u00") != NULL)
	{
		debug(210, "input: %s", input);
		input = string_replace("\\u00", "%", input, 1);
		debug(210, "input: %s", input);
	}

	while(ostrstr(input, "&amp;") != NULL)
	{
		debug(210, "input: %s", input);
		input = string_replace("&amp;", "und", input, 1);
		debug(210, "input: %s", input);
	}

	while(ostrstr(input, "&gt;") != NULL)
	{
		debug(210, "input: %s", input);
		input = string_replace("&gt;", ">", input, 1);
		debug(210, "input: %s", input);
	}

	while(ostrstr(input, "&lt;") != NULL)
	{
		debug(210, "input: %s", input);
		input = string_replace("&lt;", "<", input, 1);
		debug(210, "input: %s", input);
	}

	while(ostrstr(input, "&quot;") != NULL)
	{
		debug(210, "input: %s", input);
		input = string_replace("&quot;", "\"", input, 1);
		debug(210, "input: %s", input);
	}

	while(ostrstr(input, "&#x") != NULL)
	{
		debug(210, "out %s", input);
		input = string_replace("&#x", "%", input, 1);
		debug(210, "input: %s", input);
	}

	while(ostrstr(input, "&#") != NULL)
	{
		debug(210, "input: %s", input);
		input = string_replace("&#", "%", input, 1);
		debug(210, "input: %s", input);
	}

	if(flag == 1)
		htmldecode2(input, input);
	else
		htmldecode(input, input);

	while(ostrstr(input, ";") != NULL)
	{
		debug(210, "input: %s", input);
		input = string_replace(";", NULL, input, 1);
		debug(210, "input: %s", input);
	}

	return input;
}

char* string_deltags(char* str)
{
	int i = 0, y = 0, len = 0;

	if(str == NULL) return 0;

	len = strlen(str);

	int skip = 0;
	for(i = 0; i < len; i++)
	{
		if(str[i] == '<')
			skip = 1;
		else if(str[i] == '>')
			skip = 0;

		if(skip == 0 && str[i] != '>')
		{
			str[y] = str[i];
			y++;
		}
	}
	str[y] = '\0';

	return str;
}

char* string_striptags(char* str)
{
	int i = 0, len = 0;

	if(str == NULL) return 0;

	len = strlen(str);

	int skip = 0;
	for(i = 0; i < len; i++)
	{
		if(str[i] == '<')
			skip = 1;
		else if(str[i] == '>')
		{
			skip = 0;
			str[i] = ' ';
		}
		if(skip == 1)
			str[i] = ' ';
	}

	return strstrip(str);
}

char* string_resub(char* str, char* str2, char* input, int dir)
{
	char* tmpstr = NULL, *pos = NULL, *pos2 = NULL;

	if(str == NULL || str2 == NULL || input == NULL) return NULL;

	if(dir == 0)
	{
		pos = ostrstr(input, str);
		if(pos == NULL) return NULL;
		pos += strlen(str);

		pos2 = ostrstr(pos, str2);
		if(pos2 == NULL) return NULL;
	}
	else
	{
		pos2 = ostrstr(input, str2);
		if(pos2 == NULL) return NULL;

		pos = ostrrstr(input, str, pos2 - input, 0);
		if(pos == NULL) return NULL;
		pos += strlen(str);
	}

	tmpstr = strndup(pos, pos2 - pos);

	return strstrip(tmpstr);
}

char* ostrstrcase(char* str, char* sub)
{
	size_t len = 0;

	if(str == NULL || sub == NULL) return NULL;

	len = strlen(sub);
	while(*str)
	{
		if(toupper(*str) == toupper(*sub) && strncasecmp(str, sub, len) == 0)
			return str;
		++str;
	}
	return NULL;
}

//flag 0: convert port to 00021
//flag 1: convert port to 21
char* fixport(char* input, int flag)
{
	char* tmpstr = NULL;

	if(flag == 0)
	{
		int plen = strlen(input);
		if(plen < 5)
		{
			int i;
			for(i = 0; i < 5 - plen; i++)
				tmpstr = ostrcat(tmpstr, "0", 1, 0);
		}
		tmpstr = ostrcat(tmpstr, input, 1, 0);
	}
	else
	{
		while (*input && *input == '0')
		input++;

		tmpstr = ostrcat(tmpstr, input, 1, 0);
	}
	return tmpstr;
}

//flag 0: Mon > Montag
//flag 1: Mon > Mon
//flag 2: Monday > Montag
//flag 3: January > Januar
char* translate_time(char* input, int flag)
{
	char* tmpstr = NULL;
	if(input == NULL) return tmpstr;
	tmpstr = ostrcat(tmpstr, input, 1, 1);

	if(flag == 0)
	{
		tmpstr = string_replace_all("Mon", _("Monday"), tmpstr, 1);
		tmpstr = string_replace_all("Tue", _("Tuesday"), tmpstr, 1);
		tmpstr = string_replace_all("Wed", _("Wednesday"), tmpstr, 1);
		tmpstr = string_replace_all("Thu", _("Thursday"), tmpstr, 1);
		tmpstr = string_replace_all("Fri", _("Friday"), tmpstr, 1);
		tmpstr = string_replace_all("Sat", _("Saturday"), tmpstr, 1);
		tmpstr = string_replace_all("Sun", _("Sunday"), tmpstr, 1);
	}
	else if(flag == 2)
	{
		tmpstr = string_replace_all("Monday", _("Monday"), tmpstr, 1);
		tmpstr = string_replace_all("Tuesday", _("Tuesday"), tmpstr, 1);
		tmpstr = string_replace_all("Wednesday", _("Wednesday"), tmpstr, 1);
		tmpstr = string_replace_all("Thursday", _("Thursday"), tmpstr, 1);
		tmpstr = string_replace_all("Friday", _("Friday"), tmpstr, 1);
		tmpstr = string_replace_all("Saturday", _("Saturday"), tmpstr, 1);
		tmpstr = string_replace_all("Sunday", _("Sunday"), tmpstr, 1);
	}
	else if(flag == 3)
	{
		tmpstr = string_replace_all("January", _("January"), tmpstr, 1);
		tmpstr = string_replace_all("February", _("February"), tmpstr, 1);
		tmpstr = string_replace_all("March", _("March"), tmpstr, 1);
		tmpstr = string_replace_all("April", _("April"), tmpstr, 1);
		tmpstr = string_replace_all("May", _("May"), tmpstr, 1);
		tmpstr = string_replace_all("June", _("June"), tmpstr, 1);
		tmpstr = string_replace_all("July", _("July"), tmpstr, 1);
		tmpstr = string_replace_all("August", _("August"), tmpstr, 1);
		tmpstr = string_replace_all("September", _("September"), tmpstr, 1);
		tmpstr = string_replace_all("October", _("October"), tmpstr, 1);
		tmpstr = string_replace_all("November", _("November"), tmpstr, 1);
		tmpstr = string_replace_all("December", _("December"), tmpstr, 1);
	}
	else
	{
		tmpstr = string_replace("Mon", _("Mon"), tmpstr, 1);
		tmpstr = string_replace("Tue", _("Tue"), tmpstr, 1);
		tmpstr = string_replace("Wed", _("Wed"), tmpstr, 1);
		tmpstr = string_replace("Thu", _("Thu"), tmpstr, 1);
		tmpstr = string_replace("Fri", _("Fri"), tmpstr, 1);
		tmpstr = string_replace("Sat", _("Sat"), tmpstr, 1);
		tmpstr = string_replace("Sun", _("Sun"), tmpstr, 1);
	}

	return tmpstr;
}

char* gethypridtunerchoices(int dev)
{
	char *value = NULL;
	char *hypridtunerchoices = NULL;
	char *tmpstr = NULL;
	char *tmpstr1 = NULL;
	char* start = NULL;

#ifdef MIPSEL
	char *tmpstr2 = NULL;
	char *tmpstr3 = NULL;
	char *tmpstr4 = NULL;
	char *tmpstr5 = NULL;
#endif

#if defined DTV_ENUM_DELSYS
	if(file_exist(getconfig("tempnim", NULL)) == 1)
		hypridtunerchoices = getconfig("tempnim", NULL);	
	else
#endif
		hypridtunerchoices = getconfig("hypridtunerchoices", NULL);

	if(hypridtunerchoices == NULL)
	{
		err("NULL detect");
		return NULL;
	}

	tmpstr = readfiletomem(hypridtunerchoices, 1);
	if(tmpstr == NULL)
	{
		err("NULL detect");
		return NULL;
	}

	start = ostrcat("NIM Socket ", oitoa(dev), 0, 0);
	start = ostrcat(start, ":", 1, 0);

#ifdef MIPSEL
	tmpstr1 = string_resub(start, "I2C_Device", tmpstr, 0);
	
#if defined DTV_ENUM_DELSYS
	tmpstr5 = ostrstr(tmpstr1, "Mode ");
	if(tmpstr5 != NULL)
	{
		tmpstr2 = string_resub("Mode ", ":", tmpstr5, 0);
		value = ostrcat(tmpstr2, "\n", 0, 0);
		tmpstr5[0] = '\n';
	}
	tmpstr5 = ostrstr(tmpstr5, "Mode ");
	if(tmpstr5 != NULL)
	{
		tmpstr3 = string_resub("Mode ", ":", tmpstr5, 0);
		value = ostrcat(value, tmpstr3, 1, 0);
		value = ostrcat(value, "\n", 1, 0);
		tmpstr5[0] = '\n';
	}
		tmpstr5 = ostrstr(tmpstr5, "Mode ");
	if(tmpstr5 != NULL)
	{
		tmpstr4 = string_resub("Mode ", ":", tmpstr5, 0);
		value = ostrcat(value, tmpstr4, 1, 0);
		value = ostrcat(value, "\n", 1, 0);
		tmpstr5[0] = '\n';
	}
#else

	tmpstr2 = string_resub("Mode ", ":", tmpstr1, 0);
	tmpstr3 = oregex(".*Mode ([0-9]{1}):.*", tmpstr1);

	if(tmpstr2 != NULL)
		value = ostrcat(tmpstr2, "\n", 0, 0);
	if(tmpstr3 != NULL)
		value = ostrcat(value, tmpstr3, 1, 0);
#endif

	free(start), start = NULL;
	free(tmpstr), tmpstr = NULL;
	free(tmpstr1), tmpstr1 = NULL;
	free(tmpstr2), tmpstr2 = NULL;
	free(tmpstr3), tmpstr3 = NULL;
	free(tmpstr4), tmpstr4 = NULL;
	tmpstr5 = NULL;
#else
	tmpstr1 = string_resub(start, "Frontend_Device", tmpstr, 0);
	free(start), start = NULL;

	printf("---gethypridtunerchoices--------------------------------\n");
	printf("tmpstr1: %s\n", tmpstr1);

	if(ostrstr(tmpstr1, "(T/C)") != NULL)
	{
		value = ostrcat("t", "\n", 0, 0);
		value = ostrcat(value, "c", 1, 0);
	}
	if(ostrstr(tmpstr1, "(T/T2/C)") != NULL)
	{
		value = ostrcat("t", "\n", 0, 0);
		value = ostrcat(value, "c", 1, 0);
	}
	printf("value: %s\n", value);
	printf("---gethypridtunerchoices--------------------------------\n");

	free(tmpstr1), tmpstr1 = NULL;
#endif
	return value;
}

char* gethypridtunerchoicesvalue(int dev)
{
	char* hypridtunerchoices = NULL, *value = NULL, *tmpstr = NULL, *tmpstr1 = NULL, *start = NULL;

#ifdef MIPSEL
	char* tmpstr2 = NULL, *hypridlist = NULL;
#endif

#if defined DTV_ENUM_DELSYS
	if(file_exist(getconfig("tempnim", NULL)) == 1)
		hypridtunerchoices = getconfig("tempnim", NULL);	
	else
#endif
	
		hypridtunerchoices = getconfig("hypridtunerchoices", NULL);

	if(hypridtunerchoices == NULL)
	{
		err("NULL detect");
		return NULL;
	}

	tmpstr = readfiletomem(hypridtunerchoices, 1);
	if(tmpstr == NULL)
	{
		err("NULL detect");
		return NULL;
	}

	start = ostrcat("NIM Socket ", oitoa(dev), 0, 0);
	start = ostrcat(start, ":", 1, 0);
#ifdef MIPSEL
	tmpstr1 = string_resub(start, "I2C_Device", tmpstr, 0);
	free(start), start = NULL;

	hypridlist = gethypridtunerchoices(dev);

	int count = 0;
	int i = 0;
	struct splitstr* ret1 = NULL;
	ret1 = strsplit(hypridlist, "\n", &count);

	if(ret1 != NULL)
	{
		int max = count;
		for(i = 0; i < max; i++)
		{
			start = ostrcat("Mode ", ret1[i].part, 0, 0);
			start = ostrcat(start, ":", 1, 0);
			tmpstr2 = string_resub(start, "\n", tmpstr1, 0);

			if(i > 0)
				value = ostrcat(value, "\n", 1, 0);

			if(tmpstr2 != NULL)
				value = ostrcat(value, tmpstr2, 1, 0);

			free(tmpstr2), tmpstr2 = NULL;
			free(start), start = NULL;
		}
	}
	free(ret1), ret1 = NULL;
	free(tmpstr1), tmpstr1 = NULL;
	free(hypridlist), hypridlist = NULL;
#else
	tmpstr1 = string_resub(start, "Frontend_Device", tmpstr, 0);
	free(start), start = NULL;

	printf("---gethypridtunerchoicesvalue--------------------------------\n");
	printf("tmpstr1: %s\n", tmpstr1);

	if(ostrstr(tmpstr1, "(T/C)") != NULL)
	{
		value = ostrcat("DVB-T", "\n", 0, 0);
		value = ostrcat(value, "DVB-C", 1, 0);
	}
	if(ostrstr(tmpstr1, "(T/T2/C)") != NULL)
	{
		value = ostrcat("DVB-T", "\n", 0, 0);
		value = ostrcat(value, "DVB-C", 1, 0);
	}
	printf("value: %s\n", value);
	printf("---gethypridtunerchoicesvalue--------------------------------\n");
	free(tmpstr1), tmpstr1 = NULL;
#endif

	return value;
}

char* gethypridtunerchoicesvaluename(int dev, char* hyprid)
{

	char* value = NULL;
#ifdef MIPSEL
	char* hypridtunerchoices = NULL, *tmpstr = NULL, *tmpstr1 = NULL, *tmpstr2 = NULL, *start = NULL;
	
#if defined DTV_ENUM_DELSYS
	if(file_exist(getconfig("tempnim", NULL)) == 1)
		hypridtunerchoices = getconfig("tempnim", NULL);	
	else
#endif

	hypridtunerchoices = getconfig("hypridtunerchoices", NULL);

	if(hypridtunerchoices == NULL)
	{
		err("NULL detect");
		return NULL;
	}

	tmpstr = readfiletomem(hypridtunerchoices, 1);
	if(tmpstr == NULL)
	{
		err("NULL detect");
		return NULL;
	}

	start = ostrcat("NIM Socket ", oitoa(dev), 0, 0);
	start = ostrcat(start, ":", 1, 0);
	tmpstr1 = string_resub(start, "I2C_Device", tmpstr, 0);
	free(start), start = NULL;

	start = ostrcat("Mode ", hyprid, 0, 0);
	start = ostrcat(start, ":", 1, 0);

	tmpstr2 = string_resub(start, "\n", tmpstr1, 0);

	if(tmpstr2 != NULL)
		value = ostrcat(value, tmpstr2, 1, 0);

	free(tmpstr1), tmpstr1 = NULL;
	free(tmpstr2), tmpstr2 = NULL;
	free(start), start = NULL;
#else
	printf("---gethypridtunerchoicesvalue--------------------------------\n");
	if(ostrstr(hyprid, "t") != NULL)
		value = ostrcat("DVB-T", NULL, 0, 0);
	else if(ostrstr(hyprid, "c") != NULL)
		value = ostrcat("DVB-C", NULL, 0, 0);
	printf("value: %s\n", value);
	printf("---gethypridtunerchoicesvalue--------------------------------\n");

#endif
	return value;
}

#ifdef MIPSEL
int sethypridtunernew(struct dvbdev* tuner, char* value)
{
	int ret = 0;

	char* tmpstr = NULL;
	char* buf1 = NULL;
	
	tmpstr = ostrcat(tuner->feshortname, "_hyprid_last", 0, 0);
	if(ostrcmp(getconfigdef(tmpstr, NULL, "0"), value) == 0)
	{
		printf("set hyprid.. new value %s eq old value %s\n", value, getconfig(tmpstr, "0"));
		free(tmpstr), tmpstr = NULL;
		return 0;
	}
	
	if(fechangetype(tuner, value) == 1)
	{
		if(getconfigint("need_delivery_system_workaround" , NULL) == 1)
		{	
			buf1 = readsys("/sys/module/dvb_core/parameters/dvb_shutdown_timeout",1);
			ret = writesys("/sys/module/dvb_core/parameters/dvb_shutdown_timeout", "0", 1);
			if(ret != 0)
				printf("no /sys/module/dvb_core/parameters/dvb_shutdown_timeout available\n");
			if(tuner->fd > -1)
			{
				feclose(tuner, -1);
				tuner->fd = feopen(tuner, NULL);
			}
			writesys("/sys/module/dvb_core/parameters/dvb_shutdown_timeout", buf1, 1);	
			free(tmpstr); tmpstr = NULL;
			free(buf1); buf1 = NULL;
		}
		else
			printf("Hinweis -> hypridtuner workaround NOT active\n");
			
		addconfig(tmpstr, value);
	}
	free(tmpstr), tmpstr = NULL;

	return 0;
}
#endif

int sethypridtuner(int dev, char* value)
{
	int ret = 0;

#ifdef MIPSEL
	char* buf = NULL, *hypridtuner = NULL, *tmpstr = NULL;

	hypridtuner = getconfig("hypridtuner", NULL);

	if(hypridtuner != NULL)
	{
		buf = malloc(MINMALLOC);
		if(buf == NULL)
		{
			err("no memory");
			return 0;
		}
	}

	sprintf(buf, hypridtuner, dev);
	if(buf != NULL)
	{
		printf("old -> set %s to %s\n", buf, value);
		ret = writesys(buf, value, 0);
		free(tmpstr); tmpstr = NULL;
		return ret;
	}
#else
	printf("---sethypridtuner--------------------------------\n");

	addconfigtmp("hypridtuner", value);
	ret = writeconfigtmp();
	writeallconfig(1);

	printf("value: %s\n", value);
	printf("read titan.cfg: hypridtuner=%s\n", getconfig("frontenddev", NULL));
	printf("---sethypridtuner--------------------------------\n");

	return ret;
#endif
	return 0;
}

int phpkit_userauth(char* link, char* user, char* pass)
{
	debug(99, "phpkit user: %s", user);
	debug(99, "phpkit pass: %s", pass);
	debug(99, "phpkit url: %s", link);

	int skip = 0;
	char* ip = NULL, *tmphost = NULL, *tmppath = NULL, *tmpstr = NULL, *send = NULL, *hash = NULL, *cookie1 = NULL, *cookie2 = NULL, *tmplink = NULL, *pos = NULL, *path = NULL, *hashlen = NULL;

	tmplink = ostrcat(link, NULL, 0, 0);

	tmphost = string_replace("http://", "", tmplink, 0);
	free(tmplink) , tmplink = NULL;

	if(tmphost != NULL)
		pos = strchr(tmphost, '/');
	if(pos != NULL)
	{
		pos[0] = '\0';
		path = pos + 1;
	}

	tmppath = ostrcat("/", path, 0, 0);

	send = ostrcat(send, "GET ", 1, 0);
	send = ostrcat(send, tmppath, 1, 0);
	send = ostrcat(send, " HTTP/1.1\r\nHost: ", 1, 0);
	send = ostrcat(send, tmphost, 1, 0);
	send = ostrcat(send, "\r\nUser-Agent: Mozilla/5.0 (X11; Linux i686) AppleWebKit/535.1 (KHTML, like Gecko) Chrome/13.0.782.99 Safari/535.1\r\nConnection: close\r\nAccept-Encoding: gzip\r\n\r\n", 1, 0);
	debug(99, "#############################################################################################################");
	debug(99, "send1: %s", send);
	debug(99, "#############################################################################################################");

	tmpstr = gethttpreal(tmphost, tmppath, 80, NULL, NULL, NULL, 0, send, NULL, 5000, 1);
	debug(99, "tmpstr: %s", tmpstr);

	free(send), send = NULL;
	free(tmpstr), tmpstr = NULL;

	if(user == NULL || pass == NULL || link == NULL) return 1;

	hash = ostrcat("login=1&user=", user, 0, 0);
	hash = ostrcat(hash, "&userpw=", 1, 0);
	hash = ostrcat(hash, pass, 1, 1);
	hashlen = oitoa(strlen(hash));

	send = ostrcat(send, "POST ", 1, 0);
	send = ostrcat(send, tmppath, 1, 0);
	send = ostrcat(send, " HTTP/1.1\r\nContent-Length: ", 1, 0);
	send = ostrcat(send, hashlen, 1, 0);
	send = ostrcat(send, "\r\nAccept-Encoding: gzip\r\nConnection: close\r\nUser-Agent: Mozilla/5.0 (X11; Linux i686) AppleWebKit/535.1 (KHTML, like Gecko) Chrome/13.0.782.99 Safari/535.1\r\nHost: ", 1, 0);
	send = ostrcat(send, tmphost, 1, 0);
	send = ostrcat(send, "\r\nCookie: pageredir=", 1, 0);
	send = ostrcat(send, cookie1, 1, 0);
	send = ostrcat(send, "; PHPSESSID=", 1, 0);
	send = ostrcat(send, cookie2, 1, 0);
	send = ostrcat(send, "\r\nContent-Type: application/x-www-form-urlencoded\r\n\r\n", 1, 0);
	send = ostrcat(send, hash, 1, 0);
	free(hash); hash = NULL;
	free(hashlen); hashlen = NULL;

	debug(99, "#############################################################################################################");
	debug(99, "send1: %s", send);
	debug(99, "#############################################################################################################");

	tmpstr = gethttpreal(tmphost, tmppath, 80, NULL, NULL, NULL, 0, send, NULL, 5000, 1);
	debug(99, "tmpstr: %s", tmpstr);

	free(cookie1); cookie1 = NULL;
	free(cookie2); cookie2 = NULL;
	free(tmphost); tmphost = NULL;
	free(send); send = NULL;
	free(ip); ip = NULL;
	if(tmpstr == NULL) skip = 1;
	free(tmpstr); tmpstr = NULL;
	if(skip == 1) return 1;
	return 0;
}

void changetunername(struct skin* tunernode, int adapter, int devnr, char* name, char* fehyprid)
{
	char* tmpnr = NULL, *tmpstr = NULL;
	tmpnr = oitoa(adapter);
	tmpstr = ostrcat(_("Tuner "), tmpnr, 0, 1);
	tmpstr = ostrcat(tmpstr, "/", 1, 0);
	tmpnr = oitoa(devnr);
	tmpstr = ostrcat(tmpstr, tmpnr, 1, 1);
	tmpstr = ostrcat(tmpstr, ": ", 1, 0);
	tmpstr = ostrcat(tmpstr, name, 1, 0);
	if(fehyprid != NULL)
	{
		tmpstr = ostrcat(tmpstr, " (", 1, 0);
		tmpstr = ostrcat(tmpstr, _("Multituner adjustable"), 1, 0);
		tmpstr = ostrcat(tmpstr, ")", 1, 0);
	}
	changetext(tunernode, tmpstr);
	free(tmpstr); tmpstr = NULL;
}

char* gethypridtunername(int dev, char* hyprid)
{
	char* hypridtunerchoices = NULL, *value = NULL, *tmpstr = NULL, *tmpstr1 = NULL, *tmpstr2 = NULL, *start = NULL;

#if defined DTV_ENUM_DELSYS
	if(file_exist(getconfig("tempnim", NULL)) == 1)
		hypridtunerchoices = getconfig("tempnim", NULL);	
	else
#endif

	hypridtunerchoices = getconfig("hypridtunerchoices", NULL);

	if(hypridtunerchoices == NULL)
	{
		err("NULL detect");
		return NULL;
	}

	tmpstr = readfiletomem(hypridtunerchoices, 1);
	if(tmpstr == NULL)
	{
		err("NULL detect");
		return NULL;
	}

	start = ostrcat("NIM Socket ", oitoa(dev), 0, 0);
	start = ostrcat(start, ":", 1, 0);
	tmpstr1 = string_resub(start, "I2C_Device", tmpstr, 0);
	free(start), start = NULL;

	tmpstr2 = string_resub("Name: ", "\n", tmpstr1, 0);

	if(tmpstr2 != NULL)
		value = ostrcat(value, tmpstr2, 1, 0);

	free(tmpstr1), tmpstr1 = NULL;
	free(tmpstr2), tmpstr2 = NULL;
	free(start), start = NULL;

	return value;
}

void convertsettings()
{
	struct menulist* mlist = NULL, *mbox = NULL;
	struct skin* load = getscreen("loading");
	int flag = 0, ret = 0;
	char* tmpstr = NULL;

	addmenulist(&mlist, "Create Satellites/Provider (Sat)", _("Create Satellites/Provider (Sat)"), NULL, 0, 0);
	addmenulist(&mlist, "Create Satellites/Provider (Cable)", _("Create Satellites/Provider (Cable)"), NULL, 0, 0);
	addmenulist(&mlist, "Create Satellites/Provider (Terrestrial)", _("Create Satellites/Provider (Terrestrial)"), NULL, 0, 0);
	addmenulist(&mlist, "Create Satellites/Provider (All)", _("Create Satellites/Provider (All)"), NULL, 0, 0);

	mbox = menulistbox(mlist, "createsettings", _("Import mode"), NULL, NULL, NULL, 1, 0);
	if(mbox != NULL) tmpstr = mbox->name;

	if(ostrcmp(tmpstr, "Create Satellites/Provider (Sat)") == 0)
		flag = 0;
	else if(ostrcmp(tmpstr, "Create Satellites/Provider (Cable)") == 0)
		flag = 1;
	else if(ostrcmp(tmpstr, "Create Satellites/Provider (Terrestrial)") == 0)
		flag = 2;
	else if(ostrcmp(tmpstr, "Create Satellites/Provider (All)") == 0)
		flag = 3;
	else
	{
		freemenulist(mlist, 1); mlist = NULL;
		return;
	}

	freemenulist(mlist, 1); mlist = NULL;

	drawscreen(load, 0, 0);

	if(flag == 0)
	{
		ret = converte2settings(0);
		if(ret == 0) return;
		system("cp -a /tmp/satellites.sat /mnt/settings/satellites");
		system("cp -a /tmp/transponder.sat /mnt/settings/transponder");
	}
	else if(flag == 1)
	{
		ret = converte2settings(1);
		if(ret == 0) return;
		system("cp -a /tmp/satellites.cable /mnt/settings/satellites");
		system("cp -a /tmp/transponder.cable /mnt/settings/transponder");
	}
	else if(flag == 2)
	{
		converte2settings(2);
		system("cp -a /tmp/satellites.ter /mnt/settings/satellites");
		system("cp -a /tmp/transponder.ter /mnt/settings/transponder");
	}
	else if(flag == 3)
	{
		ret = converte2settings(0);
		if(ret == 0) return;
		ret = converte2settings(1);
		if(ret == 0) return;
		ret = converte2settings(2);
		if(ret == 0) return;
		system("cp -a /tmp/satellites.sat /mnt/settings/satellites");
		system("cp -a /tmp/transponder.sat /mnt/settings/transponder");

		system("cat /tmp/satellites.cable >> /mnt/settings/satellites");
		system("cat /tmp/transponder.cable >> /mnt/settings/transponder");

		system("cat /tmp/satellites.ter >> /mnt/settings/satellites");
		system("cat /tmp/transponder.ter >> /mnt/settings/transponder");
	}

//	free(tmpstr), tmpstr = NULL;
	textbox(_("Message"), _("Satellite/Provider import done, Titan will be restarted!\nPlease check tuner config and\nexecute a channel search after restart."), _("OK"), getrcconfigint("rcok", NULL), NULL, 0, NULL, 0, NULL, 0, 1100, 200, 0, 0);
	//sync usb
	system("sync");
	//enable autoscan after guirestart
	addconfig("autoscan", "1");
	//write only config file
	writeallconfig(3);
	//gui restart and write no config
	oshutdown(3, 2);
}

// flag 0 = sat
// flag 1 = cable
// flag 2 = ter
int converte2settings(int flag)
{
	char* path = NULL, *buf = NULL, *tmpstr = NULL, *tmpstr1 = NULL, *tmpstr2 = NULL, *line = NULL, *name = NULL, *orbitalpos = NULL, *fetype = NULL, *flags = NULL, *outfile = NULL, *start = NULL, *end = NULL, *filename = NULL, *transponderfile = NULL, *satfile = NULL, *tmpline = NULL;
	int incount = 0;

	if(flag == 0)
	{
		system("rm -rf /tmp/transponder.sat");
		system("rm -rf /tmp/satellites.sat");
		system("cp -a /var/etc/tuxbox/satellites.xml /tmp/satellites.xml");
		start = ostrcat("<sat ", NULL, 0, 0);
		end = ostrcat("</sat>", NULL, 0, 0);
		filename = ostrcat("/tmp/satellites.xml", NULL, 0, 0);
		path = ostrcat("/transponder/satellites.xml", NULL, 0, 0);
		transponderfile = ostrcat("/tmp/transponder.sat", NULL, 0, 0);
		satfile = ostrcat("/tmp/satellites.sat", NULL, 0, 0);
		fetype = ostrcat("0", NULL, 0, 0);
	}
	else if(flag == 1)
	{
		system("rm -rf /tmp/transponder.cable");
		system("rm -rf /tmp/satellites.cable");
		system("cp -a /var/etc/tuxbox/cables.xml /tmp/cables.xml");
		start = ostrcat("<cable ", NULL, 0, 0);
		end = ostrcat("</cable>", NULL, 0, 0);
		filename = ostrcat("/tmp/cables.xml", NULL, 0, 0);
		path = ostrcat("/transponder/cables.xml", NULL, 0, 0);
		transponderfile = ostrcat("/tmp/transponder.cable", NULL, 0, 0);
		satfile = ostrcat("/tmp/satellites.cable", NULL, 0, 0);
		fetype = ostrcat("1", NULL, 0, 0);
		incount = 4999;
	}
	else if(flag == 2)
	{
		system("rm -rf /tmp/transponder.ter");
		system("rm -rf /tmp/satellites.ter");
		system("cp -a /var/etc/tuxbox/terrestrial.xml /tmp/terrestrial.xml");
		start = ostrcat("<terrestrial ", NULL, 0, 0);
		end = ostrcat("</terrestrial>", NULL, 0, 0);
		filename = ostrcat("/tmp/terrestrial.xml", NULL, 0, 0);
		path = ostrcat("/transponder/terrestrial.xml", NULL, 0, 0);
		transponderfile = ostrcat("/tmp/transponder.ter", NULL, 0, 0);
		satfile = ostrcat("/tmp/satellites.ter", NULL, 0, 0);
		fetype = ostrcat("2", NULL, 0, 0);
		incount = 9999;
	}

	if(!file_exist(filename))
		gethttp("openaaf.dyndns.tv", path, 80, filename, HTTPAUTH, 5000, NULL, 0);

	char* tmptext = NULL;
	tmptext = ostrcat(_("Error: Transponder Source file not found"), "\nfilename: ", 0, 0);
	tmptext = ostrcat(tmptext, transponderfile, 1, 0);

	if(!file_exist(filename))
	{
		textbox(_("Message"), tmptext, _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 1000, 200, 0, 0);
		free(tmptext), tmptext = NULL;
		free(path), path = NULL;
		free(filename), filename = NULL;
		free(satfile), satfile = NULL;
		free(fetype), fetype = NULL;
		free(start), start = NULL;
		free(end), end = NULL;
		free(transponderfile), transponderfile = NULL;
		return 0;
	}
	free(tmptext), tmptext = NULL;

	buf = readfiletomem(filename, 1);
	if(buf == NULL)
	{
		printf("buf: %s\n", buf);
		tmptext = ostrcat(_("Error: Transponder Source file no data"), "\nfilename: ", 0, 0);
		tmptext = ostrcat(tmptext, transponderfile, 1, 0);
		textbox(_("Message"), tmptext, _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 1000, 200, 0, 0);
		free(tmptext), tmptext = NULL;
		free(path), path = NULL;
		free(filename), filename = NULL;
		free(satfile), satfile = NULL;
		free(fetype), fetype = NULL;
		free(start), start = NULL;
		free(end), end = NULL;
		free(transponderfile), transponderfile = NULL;
		return 0;
	}

//	writesys("/tmp/convert.log", buf, 1);

	while(ostrstr(buf, start) != NULL)
	{
		incount++;
		tmpstr = string_resub(start, end, buf, 0);
		tmpstr1 = ostrcat(tmpstr, NULL, 0, 0);

		//printf("name: %s\n", getxmlentry(ret1[i].part, "name="));
		//printf("position: %s\n", getxmlentry(ret1[i].part, "position="));

		name = getxmlentry(tmpstr, "name=");
		if(flag == 0)
			orbitalpos = getxmlentry(tmpstr, "position=");
		else
			orbitalpos = ostrcat(oitoa(incount), NULL, 1, 0);

		flags = getxmlentry(tmpstr, "flags=");
		//string_decode(name, 0);
		name = string_replace_all("&amp;", "und", name, 1);

		line = ostrcat(line, name, 1, 0); //name
		line = ostrcat(line, "#", 1, 0);
//		line = ostrcat(line, flags, 1, 0); //flag
// 0 for all sat/ter/cab ???
		line = ostrcat(line, "0", 1, 0);
		line = ostrcat(line, "#", 1, 0);
		line = ostrcat(line, orbitalpos, 1, 0); //orbitalpos
		line = ostrcat(line, "#", 1, 0);
		line = ostrcat(line, fetype, 1, 0); //fetype
		printf("%s: %s\n", satfile, line);
		writesys(satfile, line, 3);
		free(line), line = NULL;

		int count = 0;

		int i = 0;
		struct splitstr* ret1 = NULL;
		ret1 = strsplit(tmpstr1, "\n", &count);
		if(ret1 != NULL)
		{
			int max = count;
			for(i = 0; i < max; i++)
			{
				if(i == 0) continue;
				//id
				tmpline = ostrcat(tmpline, "0", 1, 0);
				tmpline = ostrcat(tmpline, "#", 1, 0);

				//fetype
				tmpline = ostrcat(tmpline, fetype, 1, 0);
				tmpline = ostrcat(tmpline, "#", 1, 0);

				//frequency
				if(ostrstr((ret1[i]).part, "frequency=") != NULL)
					tmpline = ostrcat(tmpline, getxmlentry(ret1[i].part, "frequency="), 1, 0);
				else
					tmpline = ostrcat(tmpline, "-1", 1, 0);
				tmpline = ostrcat(tmpline, "#", 1, 0);

				//polarization
				if(ostrstr((ret1[i]).part, "polarization=") != NULL)
					tmpline = ostrcat(tmpline, getxmlentry(ret1[i].part, "polarization="), 1, 0);
				else
					tmpline = ostrcat(tmpline, "-1", 1, 0);
				tmpline = ostrcat(tmpline, "#", 1, 0);

				//orbitalpos
				if(orbitalpos != NULL)
					tmpline = ostrcat(tmpline, orbitalpos, 1, 0);
				else
					tmpline = ostrcat(tmpline, "-1", 1, 0);
				tmpline = ostrcat(tmpline, "#", 1, 0);

				//symbolrate
				if(ostrstr((ret1[i]).part, "symbol_rate=") != NULL)
					tmpline = ostrcat(tmpline, getxmlentry(ret1[i].part, "symbol_rate="), 1, 0);
				else
				{
					if(flag == 2)
					{
						if(ostrstr((ret1[i]).part, "bandwidth=") != NULL)
							tmpline = ostrcat(tmpline, getxmlentry(ret1[i].part, "bandwidth="), 1, 0);
						else
							tmpline = ostrcat(tmpline, "-1", 1, 0);
					}
					else
						tmpline = ostrcat(tmpline, "-1", 1, 0);
				}
				tmpline = ostrcat(tmpline, "#", 1, 0);

				//modulation
				if(ostrstr((ret1[i]).part, "modulation=") != NULL)
					tmpline = ostrcat(tmpline, getxmlentry(ret1[i].part, "modulation="), 1, 0);
				else
				{
					if(flag == 2)
					{
						if(ostrstr((ret1[i]).part, "constellation=") != NULL)
							tmpline = ostrcat(tmpline, getxmlentry(ret1[i].part, "constellation="), 1, 0);
						else
							tmpline = ostrcat(tmpline, "3", 1, 0);
					}
					else
						tmpline = ostrcat(tmpline, "0", 1, 0);
				}
				tmpline = ostrcat(tmpline, "#", 1, 0);

				//fec
				if(ostrstr((ret1[i]).part, "fec_inner=") != NULL)
					tmpline = ostrcat(tmpline, getxmlentry(ret1[i].part, "fec_inner="), 1, 0);
				else
				{
					if(flag == 2)
#if DVB_API_VERSION >= 5
						tmpline = ostrcat(tmpline, "-1", 1, 0);
#else
						tmpline = ostrcat(tmpline, "5", 1, 0);
#endif
					else
						tmpline = ostrcat(tmpline, "0", 1, 0);
				}
				tmpline = ostrcat(tmpline, "#", 1, 0);

				//pilot
				if(checkbox("UFS910") == 1)
					tmpline = ostrcat(tmpline, "1", 1, 0);
				else if(flag == 2)
#if DVB_API_VERSION >= 5
					tmpline = ostrcat(tmpline, "-1", 1, 0);
#else
					tmpline = ostrcat(tmpline, "2", 1, 0);
#endif
				else
					tmpline = ostrcat(tmpline, "2", 1, 0);
				tmpline = ostrcat(tmpline, "#", 1, 0);

				//rolloff
				if(checkbox("UFS910") == 1)
					tmpline = ostrcat(tmpline, "0", 1, 0);
				else if(flag == 2)
#if DVB_API_VERSION >= 5
					tmpline = ostrcat(tmpline, "-1", 1, 0);
#else
					tmpline = ostrcat(tmpline, "3", 1, 0);
#endif
				else
					tmpline = ostrcat(tmpline, "3", 1, 0);
				tmpline = ostrcat(tmpline, "#", 1, 0);

				//inversion
				if(flag == 2)
#if DVB_API_VERSION >= 5
					tmpline = ostrcat(tmpline, "-1", 1, 0);
#else
					tmpline = ostrcat(tmpline, "2", 1, 0);
#endif
				else
					tmpline = ostrcat(tmpline, "2", 1, 0);
				tmpline = ostrcat(tmpline, "#", 1, 0);

				//system
				if(flag == 2)
				{
					// workaround we need 0 + 1 for hd channels
					line = ostrcat(line, tmpline, 1, 0);
					line = ostrcat(line, "0", 1, 0);
					line = ostrcat(line, "\n", 1, 0);

					line = ostrcat(line, tmpline, 1, 0);
					line = ostrcat(line, "1", 1, 0);
					line = ostrcat(line, "\n", 1, 0);
				}
				else if(ostrstr((ret1[i]).part, "system=") != NULL)
				{
					tmpline = ostrcat(tmpline, getxmlentry(ret1[i].part, "system="), 1, 0);
					tmpline = ostrcat(tmpline, "\n", 1, 0);
					line = ostrcat(line, tmpline, 1, 0);
				}
				else
				{
					line = ostrcat(line, tmpline, 1, 0);
					line = ostrcat(line, "0", 1, 0);
					line = ostrcat(line, "\n", 1, 0);
				}

				free(tmpline), tmpline = NULL;
			}
		}

		tmpstr2 = ostrcat(start, tmpstr, 0, 0);

		buf = string_replace(tmpstr2, NULL, buf, 1);

	//	outfile = ostrcat("/tmp/convert.", oitoa(incount), 0, 1);
	//	outfile = ostrcat(outfile, ".log", 1, 0);
	//	writesys(outfile, buf, 2);
	//	writesys("/tmp/convert.log", buf, 3);

		writesys(transponderfile, line, 2);
		free(line), line = NULL;

		free(tmpstr), tmpstr = NULL;
		free(tmpstr1), tmpstr1 = NULL;
		free(tmpstr2), tmpstr2 = NULL;
		free(ret1), ret1 = NULL;
		free(name), name = NULL;
		free(orbitalpos), orbitalpos = NULL;
		free(flags), flags = NULL;
		free(outfile), outfile = NULL;
	}

	free(path), path = NULL;
	free(filename), filename = NULL;
	free(satfile), satfile = NULL;
	free(transponderfile), transponderfile = NULL;

	free(buf), buf = NULL;
	free(start), start = NULL;
	free(end), end = NULL;
	free(fetype), fetype = NULL;

	return 1;
}

char* system_infos(int mode)
{
	char* tmpstr = NULL, *tmpstr1 = NULL, *tmpstr2 = NULL;

	if(mode == 0)
	{
		tmpstr1 = ostrcat("Date = ", "", 0, 0);
		tmpstr1 = ostrcat(tmpstr1, gettime(NULL, "%d %B %Y"), 1, 1);
		tmpstr = ostrcat(tmpstr, tmpstr1, 1, 1);

		tmpstr1 = ostrcat("\nTime = ", "", 0, 0);
		tmpstr1 = ostrcat(tmpstr1, gettime(NULL, "%H:%M:%S"), 1, 1);
		tmpstr = ostrcat(tmpstr, tmpstr1, 1, 1);

		tmpstr1 = ostrcat("\nUptime = ", "", 0, 0);
		tmpstr2 = command("uptime");
		if(tmpstr2 != NULL) tmpstr2 = strtok(tmpstr2, ",");
		tmpstr1 = ostrcat(tmpstr1, tmpstr2, 1, 1);
		tmpstr = ostrcat(tmpstr, tmpstr1, 1, 1);

		tmpstr1 = ostrcat("\nBoxtype = ", "", 0, 0);
		tmpstr2 = string_toupper(command("cat /etc/model"));
		tmpstr1 = ostrcat(tmpstr1, tmpstr2, 1, 1);
		tmpstr = ostrcat(tmpstr, tmpstr1, 1, 1);

		tmpstr1 = ostrcat("\nLoad = ", "", 0, 0);
		tmpstr2 = command("cat /proc/loadavg");
		tmpstr1 = ostrcat(tmpstr1, tmpstr2, 1, 1);
		tmpstr = ostrcat(tmpstr, tmpstr1, 1, 1);
	}

	if(mode == 1)
	{
		system("ls /media/usb/* >/dev/null");
		system("ls /media/net/* >/dev/null");
		system("ls /var/swap/* >/dev/null");
		system("ls /mnt/swapextension/* >/dev/null");
		system("ls /var/backup/* >/dev/null");
		system("ls /media/hdd/* >/dev/null");
		tmpstr = command("df -h");
	}

	if(mode == 2)
		tmpstr = command("cat /proc/version");

	if(mode == 3)
	{
		system("ls /media/usb/* >/dev/null");
		system("ls /media/net/* >/dev/null");
		system("ls /var/swap/* >/dev/null");
		system("ls /mnt/swapextension/* >/dev/null");
		system("ls /var/backup/* >/dev/null");
		system("ls /media/hdd/* >/dev/null");
		tmpstr = command("mount");
	}

	if(mode == 4)
	{
		tmpstr1 = command("ifconfig");
		tmpstr2 = command("route -n");
		tmpstr = ostrcat(tmpstr1, tmpstr2, 1, 1);
	}

	if(mode == 5)
		tmpstr = command("free");

	return tmpstr;
}

char* system_infos_sysinfo(int mode)
{
	char* tmpstr = NULL;

	if(mode == 0)
		tmpstr = command("cat /proc/cpuinfo | sed 's/\t\t/\t/'");
	else if(mode == 1)
		tmpstr = command("cat /proc/meminfo");
	else if(mode == 2)
		tmpstr = command("cat /proc/mtd");
	else if(mode == 3)
		tmpstr = command("cat /proc/modules");
	else if(mode == 4)
		tmpstr = command("cat /proc/devices");
	else if(mode == 5)
	{
		char* tmpstr1 = NULL, **tmpstr2 = NULL, **tmpstr3 = NULL;
		int i = 0, ii = 0;
		char* swap[] = {"Name: ", "Type: ", "Size: ", "Used: ", "Prio: "};

		tmpstr1 = command("cat /proc/swaps | sed 's/\t/ /g; s/[ ]* / /g'");
		tmpstr2 = str_split(tmpstr1, "\n");
		if(tmpstr2 != NULL)
		{
			free(tmpstr2[0]); tmpstr2[0] = NULL;

			for(i = 1; tmpstr2[i] != NULL; i++)
			{
				tmpstr3 = str_split(tmpstr2[i], " ");
				if(tmpstr3 != NULL)
				{

					for(ii = 0; tmpstr3[ii] != NULL; ii++)
					{
						tmpstr = ostrcat(tmpstr, swap[ii], 1, 0);
						tmpstr = ostrcat(tmpstr, tmpstr3[ii], 1, 1);
						tmpstr = ostrcat(tmpstr, "\n", 1, 0);
					}
				}

				tmpstr = ostrcat(tmpstr, "\n", 1, 0);
				free(tmpstr2[i]); tmpstr2[i] = NULL;
				free(tmpstr3); tmpstr3 = NULL;
			}
		}

		free(tmpstr3); tmpstr3 = NULL;
		free(tmpstr2); tmpstr2 = NULL;
		free(tmpstr1); tmpstr1 = NULL;
	}
	else if(mode == 6)
		tmpstr = command("top -b -n1");
	else if(mode == 7)
		tmpstr = command("ps");
	else if(mode == 8)
	{
		tmpstr = command("lsusb");
		tmpstr = ostrcat(tmpstr, "\n-------------------------------------------\n", 1, 0);
		char* tmpstr2 = NULL;
		tmpstr2 = command("cat /proc/bus/usb/devices");
		tmpstr = ostrcat(tmpstr, tmpstr2, 1, 1);
	}
	return tmpstr;
}

char* system_logs(int mode)
{
	char* tmpstr = NULL, *tmpstr1 = NULL, *tmpstr2 = NULL, *tmpstr3 = NULL, *path = NULL, *boxversion = NULL;

	path = ostrcat("/svn/image/changelog.", NULL, 0, 0);

	if(mode == 0)
	{
		tmpstr1 = readfiletomem("/etc/motd", 0);
		if(tmpstr1 != NULL) tmpstr2 = strstr(tmpstr1, "wElc0me");
		tmpstr3 = readfiletomem("/etc/imageinfo", 0);
		if(tmpstr2 == NULL)
			tmpstr = ostrcat(tmpstr3, NULL, 1, 0);
		else
			tmpstr = ostrcat(tmpstr2, tmpstr3, 1, 1);
	}
	else if(mode == 1)
	{
		if(isfile("/etc/model")	== 0) return NULL;
		boxversion = string_tolower(readsys("/etc/model", 1));

		path = ostrcat(path, boxversion, 1, 0);
		path = ostrcat(path, ".titan", 1, 0);

#ifdef OEBUILD
		tmpstr1 = gethttp("openaaf.dyndns.tv", path, 80, NULL, HTTPAUTH, 5000, NULL, 0);
#ifdef SH4
		tmpstr = readfromlinetoline(tmpstr1, 37, 537, 1);
#else
		tmpstr = readfromlinetoline(tmpstr1, 37, 29000, 1);
#endif
#else
		tmpstr1 = gethttp("openaaf.dyndns.tv", path, 80, NULL, HTTPAUTH, 5000, NULL, 0);
		tmpstr = readfromlinetoline(tmpstr1, 37, 537, 1);
#endif
	}
	else if(mode == 2)
	{
#ifdef MIPSEL
		path = ostrcat(path, "git.mipsel", 1, 0);
#elif ARM
		path = ostrcat(path, "git.mipsel", 1, 0);
#else
		path = ostrcat(path, "git.sh4", 1, 0);
#endif

#ifdef OEBUILD
		tmpstr1 = gethttp("openaaf.dyndns.tv", path, 80, NULL, HTTPAUTH, 5000, NULL, 0);
#ifdef SH4
		tmpstr = readfromlinetoline(tmpstr1, 0, 500, 1);
#else
//		tmpstr = readfiletomem(tmpstr1, 0);
		tmpstr = readfromlinetoline(tmpstr1, 0, 29000, 1);
#endif
#else
		tmpstr1 = gethttp("openaaf.dyndns.tv", path, 80, NULL, HTTPAUTH, 5000, NULL, 0);
		tmpstr = readfromlinetoline(tmpstr1, 0, 500, 1);
#endif
	}
	else if(mode == 3)
	{
		tmpstr = readfiletomem(getconfig("tracelog", NULL), 0);
	}

	free(path), path = NULL;
	free(boxversion), boxversion = NULL;
//	free(tmpstr1), tmpstr1 = NULL;
//	free(tmpstr2), tmpstr2 = NULL;
//	free(tmpstr3), tmpstr3 = NULL;

	return tmpstr;
}

char* getimgnamereal()
{
	char* tmpstr = NULL, *tmpstr2 = NULL;


	tmpstr = string_newline(readfiletomem(getconfig("imagenamefile", NULL), 1));
	debug(40, "tmpstr=%s", tmpstr);

#ifdef OEBUILD
	if(file_exist("/boot/STARTUP"))
	{
		tmpstr2 = string_newline(command("/sbin/startup.sh label"));
		debug(40, "tmpstr2=%s", tmpstr2);

		tmpstr = ostrcat(tmpstr, " (", 1, 0);
		tmpstr = ostrcat(tmpstr, tmpstr2, 1, 1);
		tmpstr = ostrcat(tmpstr, ")", 1, 0);
	}
#endif
	debug(40, "tmpstr=%s", tmpstr);

	return tmpstr;
}

char* getmoviedev()
{
	char* tmpstr = NULL, *buf = NULL;

#ifdef OEBUILD
	tmpstr = readfiletomem("/media/.moviedev", 1);
#else
	tmpstr = readfiletomem("/tmp/.moviedev", 1);
#endif
	printf("tmpstr: %s\n", tmpstr);

	buf = oregex("sd([a-z]{1,1}).*", tmpstr);
	printf("buf: %s\n", buf);

	free(tmpstr), tmpstr = NULL;

	return buf;
}

void wakeup_record_device()
{
	char* cmd = NULL, *dev = NULL;

	if(file_exist(getconfig("skriptbeforerec", NULL)))
	{
		cmd = ostrcat(getconfig("skriptbeforerec", NULL), NULL, 0, 0);
		printf("cmd: %s\n", cmd);
		system(cmd);
		free(cmd), cmd = NULL;
	}

	dev = getmoviedev();
	if(dev == NULL)
	{
		cmd = ostrcat("ls -al ", getconfig("rec_path", NULL), 0, 0);
		cmd = ostrcat(cmd," > /dev/null 2>&1", 1, 0);
	}
	else
		cmd = ostrcat("sdparm -C START /dev/sd", dev, 0, 0);

//	cmd = ostrcat(cmd, " >/dev/null", 1, 0);
	printf("cmd: %s\n", cmd);
	system(cmd);
	free(dev), dev = NULL;
	free(cmd), cmd = NULL;
}

void setdefaultbouquets()
{
	if(textbox(_("Message"), _("Start Restore Default Bouquets ?"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 1000, 200, 0, 0) == 1)
	{
		struct skin* load = getscreen("loading");
		drawscreen(load, 0, 0);

		system("rm -f /mnt/settings/bouquets.* > /dev/null 2>&1");
		system("cp -a /etc/titan.restore/mnt/settings/bouquets* /mnt/settings > /dev/null 2>&1");

		textbox(_("Message"), _("Titan will be restarted!"), _("OK"), getrcconfigint("rcok", NULL), NULL, 0, NULL, 0, NULL, 0, 1000, 200, 0, 0);
		//sync usb
		system("sync");
		//write only config file
		writeallconfig(3);
		//gui restart and write no config
		oshutdown(3, 2);
	}
}

void setdefaultsettings()
{
	if(textbox(_("Message"), _("Start Restore Default Channel Settings ?"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 1000, 200, 0, 0) == 1)
	{
		struct skin* load = getscreen("loading");
		drawscreen(load, 0, 0);

		system("rm -rf /mnt/settings/* > /dev/null 2>&1");
		system("cp -a /etc/titan.restore/mnt/settings/* /mnt/settings > /dev/null 2>&1");

		textbox(_("Message"), _("Titan will be restarted!"), _("OK"), getrcconfigint("rcok", NULL), NULL, 0, NULL, 0, NULL, 0, 1000, 200, 0, 0);
		//sync usb
		system("sync");
		//write only config file
		writeallconfig(3);
		//gui restart and write no config
		oshutdown(3, 2);
	}
}

void setdefaultproviders()
{
	if(textbox(_("Message"), _("Start Restore Default Providers ?"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 1000, 200, 0, 0) == 1)
	{
		struct skin* load = getscreen("loading");
		drawscreen(load, 0, 0);

		system("rm -f /mnt/settings/provider > /dev/null 2>&1");
		system("cp -a /etc/titan.restore/mnt/settings/provider /mnt/settings > /dev/null 2>&1");

		textbox(_("Message"), _("Titan will be restarted!"), _("OK"), getrcconfigint("rcok", NULL), NULL, 0, NULL, 0, NULL, 0, 1000, 200, 0, 0);
		//sync usb
		system("sync");
		//write only config file
		writeallconfig(3);
		//gui restart and write no config
		oshutdown(3, 2);
	}
}

void setdefaultchannels()
{
	if(textbox(_("Message"), _("Start Restore Default Channels ?"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 1000, 200, 0, 0) == 1)
	{
		struct skin* load = getscreen("loading");
		drawscreen(load, 0, 0);

		system("rm -f /mnt/settings/channel > /dev/null 2>&1");
		system("cp -a /etc/titan.restore/mnt/settings/channel /mnt/settings > /dev/null 2>&1");

		textbox(_("Message"), _("Titan will be restarted!"), _("OK"), getrcconfigint("rcok", NULL), NULL, 0, NULL, 0, NULL, 0, 1000, 200, 0, 0);
		//sync usb
		system("sync");
		//write only config file
		writeallconfig(3);
		//gui restart and write no config
		oshutdown(3, 2);
	}
}

void setdefaultsats()
{
	if(textbox(_("Message"), _("Start Restore Default Sats ?"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 1000, 200, 0, 0) == 1)
	{
		struct skin* load = getscreen("loading");
		drawscreen(load, 0, 0);

		system("rm -f /mnt/settings/satellites > /dev/null 2>&1");
		system("cp -a /etc/titan.restore/mnt/settings/satellites /mnt/settings > /dev/null 2>&1");

		textbox(_("Message"), _("Titan will be restarted!"), _("OK"), getrcconfigint("rcok", NULL), NULL, 0, NULL, 0, NULL, 0, 1000, 200, 0, 0);
		//sync usb
		system("sync");
		//write only config file
		writeallconfig(3);
		//gui restart and write no config
		oshutdown(3, 2);
	}
}

char* create_backup(char* input, int flag)
{
	char* infotext = ostrcat(_("The backup image will be saved to /tmp or /var/backup. In case of insufficient internal\nmemory, a USB Stick or HDD must be used. A folder named backup must exist on\nthe target drive."), NULL, 0, 0);
	if(flag == 0) return infotext;

	char* tmpstr = NULL, *cmd = NULL;
	int merksec = status.sec;

	status.sec = 0; //deaktivate spinner
	if(checkbox("DM900") == 1 || checkbox("DM920") == 1)
		cmd = ostrcat(cmd, "backupdream.sh ", 1, 0);
	else
		cmd = ostrcat(cmd, "backup.sh ", 1, 0);
	cmd = ostrcat(cmd, input, 1, 0);

	if(flag == 2)
		cmd = ostrcat(cmd, " 1", 1, 0);

	if(file_exist("/var/swap"))
	{
		if(!file_exist("/var/swap/logs"))
			 mkdir("/var/swap/logs", 777);

		if(file_exist("/etc/.beta") && file_exist("/var/swap/logs"))
			cmd = ostrcat(cmd, " > /var/swap/logs/backup_debug.log 2>&1", 1, 0);
	}
	else if(checkbox("ATEMIO510") != 1 && checkbox("UFS910") != 1 && checkbox("UFS922") != 1 && checkbox("ATEVIO700") != 1 && checkbox("ATEVIO7000") != 1 && checkbox("IPBOX91") != 1 && checkbox("IPBOX900") != 1 && checkbox("IPBOX910") != 1 && checkbox("IPBOX9000") != 1)
	{
		if(!file_exist("/mnt/logs"))
			 mkdir("/mnt/logs", 777);

		if(file_exist("/etc/.beta") && file_exist("/mnt/logs"))
			cmd = ostrcat(cmd, " > /mnt/logs/backup_debug.log 2>&1", 1, 0);
	}
	//tmpstr = command(cmd);
	if(system(cmd) == 0)
	//if(tmpstr == NULL)
		tmpstr = ostrcat(tmpstr, _("Backup created successfully"), 1, 0);
	else
		tmpstr = ostrcat(tmpstr, _("Backup ended with error"), 1, 0);

	free(cmd); cmd = NULL;
	status.sec = merksec;

	return tmpstr;
}

char* create_backuprestore()
{
	char* tmpstr = ostrcat(_("Press -restore- and your saved settings will be restored from your swapstick / recording hdd! The Box will restart automatically for restoring!\n\nPress -backup- to save your actual settings to swapstick / recording hdd.\nWARNING: The old backup will be deleted!"), NULL, 0, 0);
	return tmpstr;
}
char* randomstring(int n)
{
    int i, wert;
    time_t t;
    char* tmp = malloc(n+1);

    /* Intializes random number generator */
    srand((unsigned) time(&t));
    for( i = 0 ; i < n ; i++ )
    {
        wert = rand() % 9;
        sprintf(tmp+i, "%d", wert);
    }
    tmp[i] = '\0';
    return tmp;
}


int putmsgbuffer()
{
	if(file_exist("/tmp/textbox_standby") != 1)
		return 1;

	FILE *fp = fopen("/tmp/textbox_standby" , "r");
	if(fp == NULL)
	{
		printf("Error opening file: /tmp/textbox_standby\n");
		return(-1);
	}

	char* caption = malloc(255);
	char* body = malloc(255);
	char* comtext1 = malloc(20);
	char* com1 = malloc(10);
	char* comtext2 = malloc(20);
	char* com2 = malloc(10);
	char* comtext3 = malloc(20);
	char* com3 = malloc(10);
	char* comtext4 = malloc(20);
	char* com4 = malloc(10);
	char* width = malloc(10);
	char* height = malloc(10);
	char* timeout = malloc(10);
	char* flag = malloc(10);

	while(fgets(caption, 255, fp)!=NULL)
	{
		string_newline(caption);

		fgets(body, 255, fp);
		string_newline(body);
		body = string_replace_all("\t", "\n", body, 1);

		if(comtext1 == NULL)
			comtext1 = malloc(20);
		fgets(comtext1, 20, fp);
		string_newline(comtext1);
		fgets(com1, 10, fp);
		string_newline(com1);

		if(comtext2 == NULL)
			comtext2 = malloc(20);
		fgets(comtext2, 20, fp);
		string_newline(comtext2);
		fgets(com2, 10, fp);
		string_newline(com2);

		if(comtext3 == NULL)
			comtext3 = malloc(20);
		fgets(comtext3, 20, fp);
		string_newline(comtext3);
		fgets(com3, 10, fp);
		string_newline(com3);

		if(comtext4 == NULL)
			comtext4 = malloc(20);
		fgets(comtext4, 20, fp);
		string_newline(comtext4);
		fgets(com4, 10, fp);
		string_newline(com4);

		fgets(width, 10, fp);
		string_newline(width);

		fgets(height, 10, fp);
		string_newline(height);

		fgets(timeout, 10, fp);
		string_newline(timeout);

		fgets(flag, 10, fp);
		string_newline(flag);

		if(atoi(com1) == 0)
		{
			free(comtext1);
			comtext1 = NULL;
		}
		if(atoi(com2) == 0)
		{
			free(comtext2);
			comtext2 = NULL;
		}
		if(atoi(com3) == 0)
		{
			free(comtext3);
			comtext3 = NULL;
		}
		if(atoi(com4) == 0)
		{
			free(comtext4);
			comtext4 = NULL;
		}
		textbox(caption, body, comtext1, atoi(com1), comtext2, atoi(com2), comtext3, atoi(com3), comtext4, atoi(com4), atoi(width), atoi(height), atoi(timeout), atoi(flag));
	}

	fclose(fp);
	system("rm /tmp/textbox_standby");

	free(caption);
	free(body);
	free(comtext1);
	free(com1);
	free(comtext2);
	free(com2);
	free(comtext3);
	free(com3);
	free(comtext4);
	free(com4);
	free(width);
	free(height);
	free(timeout);
	free(flag);

	return 0;
}

void delunusedsat()
{
	debug(10, "delunusedsat: start");
	struct sat* node = sat;
	struct sat* prev = NULL;

	while(node != NULL)
	{
		prev = node;
		node = node->next;

		if(prev->scan != 1)
			delsat(prev->name);
	}
	debug(10, "delunusedsat: end");
}

void delunusedtransponder()
{
	debug(10, "delunusedtransponder: start");
	struct transponder* node = transponder;
	struct transponder* prev = NULL;

	while(node != NULL)
	{
		prev = node;
		node = node->next;

		if(getsatbyorbitalpos(prev->orbitalpos) == NULL)
			deltransponder(prev);
	}
	debug(10, "delunusedtransponder: end");
}

void delunusedchannel()
{
	debug(10, "delunusedchannel: start");
	struct channel* node = channel;
	struct channel* prev = NULL;

	while(node != NULL)
	{
		prev = node;
		node = node->next;

		if(prev->transponder == NULL)
			delchannel(prev->serviceid, prev->transponderid, 0);
	}
	debug(10, "delunusedchannel: end");
}

void delunusedbouquet()
{
	debug(10, "delunusedbouquet: start");
	struct mainbouquet* mnode = mainbouquet;
	struct bouquet* first = NULL;
	struct bouquet* node = NULL;
	struct bouquet* prev = NULL;

	while(mnode != NULL)
	{
		first = mnode->bouquet;
		node = mnode->bouquet;
		while(node != NULL)
		{
			prev = node;
			node = node->next;
			if(getchannel(prev->serviceid, prev->transponderid) == NULL)
				delbouquet(prev->serviceid, prev->transponderid, &first);
		}
		mnode = mnode->next;
	}
	debug(10, "delunusedbouquet: end");
}

void delunusedepgchannel()
{
	debug(10, "delunusedepgchannel: start");
	struct epgscanlist* node = epgscanlist;
	struct epgscanlist* prev = NULL;

	while(node != NULL)
	{
		prev = node;
		node = node->next;
		if(getchannel(prev->serviceid, prev->transponderid) == NULL)
			delepgscanlist(node->serviceid, node->transponderid);
	}
	debug(10, "delunusedepgchannel: end");
}

void delunusedprovider()
{
	debug(10, "delunusedprovider: start");
	struct provider* node = provider;
	struct provider* prev = NULL;

	while(node != NULL)
	{
		prev = node;
		node = node->next;
		delprovidernotused(prev);
	}
	debug(10, "delunusedprovider: end");
}

int checkshutdown(int flag)
{
	int i = 0;

	//check if record running
	if((flag == 1 || flag == 2 || flag == 3 || flag == 4 || flag == 5 || flag == 6) && (status.streaming > 0 || status.recording > 0 || getrectimerbytimediff(300) != NULL))
	{
		if(flag == 4 && status.fixpowerofftime > 1)
		{
				status.fixpowerofftime = time(NULL) + 900; //check powerofftimer again in 15min
				return 1;
		}
		if(flag == 5 && status.sd_timer != NULL && status.sd_timer->active)
		{
				status.sd_timer->shutdown_time = time(NULL) + 900; //check powerofftimer again in 15min
				return 1;
		}
		else if(flag == 6)
		{
			if(textbox(_("Message"), _("Found running record\nor record is starting soon.\nProceed System Update?\nOK to proceed, Exit to cancel"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 15, 0) != 1)
				return 1;
		}
		else
		{
			if(textbox(_("Message"), _("Found running record\nor record is starting soon.\nProceed shutdown?\nOK to proceed, Exit to cancel"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 15, 0) != 1)
				return 1;
		}
	}

	//check if download is running
	for(i = 0; i < MAXBGDOWNLOAD; i++)
	{
		if(bgdownload[i] != NULL && bgdownload[i]->ret == -1)
		{
			if(flag == 6)
			{
				if(textbox(_("Message"), _("Found running record\nor record is starting soon.\nProceed System Update?\nOK to proceed, Exit to cancel"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 15, 0) != 1)
					return 1;
			}
			else
			{
		 		if(textbox(_("Message"), _("Found running download\nProceed shutdown?\nOK to proceed, Exit to cancel"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 15, 0) != 1)
					return 1;
			}
			break;

		}
	}

	return 0;
}

char* getzapmode()
{
	char *zapmodedev = NULL;
	char *value = NULL;

	zapmodedev = getconfig("zapmodedev", NULL);

	if(zapmodedev == NULL)
	{
		err("NULL detect");
		return NULL;
	}

	value = readsys(zapmodedev, 1);
	if(value == NULL)
	{
		err("NULL detect");
		return NULL;
	}

	return value;
}

int setzapmode(char* value)
{
	char* zapmodedev;
	int ret = 0;

	zapmodedev = getconfig("zapmodedev", NULL);

	if(zapmodedev != NULL && value != NULL)
	{
		debug(10, "set %s to %s", zapmodedev, value);

		ret = writesys(zapmodedev, value, 0);
		return ret;
	}

	return 0;
}

int multiboot()
{
	int ret = 1;
	struct menulist* mlist = NULL, *mbox = NULL;

	char* skintitle = _("Select Multiboot");
	char* cmd = NULL;
	char* active = NULL;
		
#ifdef OEBUILD
	char* devicelist = command("ls -1 /boot/STARTUP_* | sed 's!/boot/!!g' | grep -v _RECOVERY | sed -e '/^ *$/d'");
	char* rootpart = string_newline(command("cat /proc/cmdline | sed -nr 's/.*root=\\/dev\\/([^\\/]+) kernel=.*/\\1/p' | sed 's! rootsubdir=!/!g' | sed 's! rootfstype=.*!!'"));
	char* activelabel = string_newline(command("/sbin/startup.sh"));
#else
	char* devicelist = command("cat /proc/diskstats | awk {'print $3'} | grep 'sd[a-z][0-9]'");
	char* rootpart = string_newline(command("cat /proc/cmdline | sed 's/^.*root=//;s/ .*$//' | sed 's!/dev/!!'"));
#endif
	debug(40, "devicelist=%s", devicelist);
	debug(40, "rootpart=%s", rootpart);

	if(devicelist != NULL && strlen(devicelist) != 0)
	{
		char* pch;
		char* label = NULL;
		char* showname = NULL;
		char* version = NULL;
		char* issue = NULL;
		char* pic = NULL;
		char* pchroot = NULL;
		pch = strtok (devicelist, "\n");
		int count = 0;
		while(pch != NULL)
		{
			count += 1;
#ifdef OEBUILD
			label= ostrcat(pch, NULL, 0, 0);
#else
			label = get_label(pch);
#endif
			if(ostrstr(label, "MINI") != NULL || ostrstr(label, "STARTUP") != NULL)
			{
#ifdef OEBUILD
				cmd = ostrcat("cat /boot/", pch, 0, 0);
				cmd = ostrcat(cmd, " | sed -nr 's/.*root=\\/dev\\/([^\\/]+) kernel=.*/\\1/p' | sed 's! rootsubdir=!/!g' | sed 's! rootfstype=.*!!'", 1, 0);
				pchroot = string_newline(command(cmd));
				free(cmd), cmd = NULL;

				cmd = ostrcat("cat /autofs/", pchroot, 0, 0);
#else
				cmd = ostrcat("cat /autofs/", pch, 0, 0);
#endif
				cmd = ostrcat(cmd, "/etc/version-svn", 1, 0);
				version = command(cmd);
				free(cmd), cmd = NULL;

				if(version == NULL)
				{
#ifdef OEBUILD
					cmd = ostrcat("date -r /autofs/", pchroot, 0, 0);
#else
					cmd = ostrcat("date -r /autofs/", pch, 0, 0);
#endif
					cmd = ostrcat(cmd, "/etc/inittab '+%Y-%m-%d'", 1, 0);
					version = command(cmd);
					free(cmd), cmd = NULL;
				}
#ifdef OEBUILD
				cmd = ostrcat("cat /autofs/", pchroot, 0, 0);
#else
				cmd = ostrcat("cat /autofs/", pch, 0, 0);
#endif
				cmd = ostrcat(cmd, "/etc/issue | sed 's/Welcome to //' | tr ' ' '\n' | sort -uf | sed 's/\\\\n//g' | sed 's/\\\\l//g' | sed -e '/^ *$/d' | sort -r | tr '\n' ' ' | sed 's/[ \t]*$//'", 1, 0);
				issue = command(cmd);
				free(cmd), cmd = NULL;

				showname = ostrcat(label, " ", 0, 0);
				showname = string_replace("STARTUP_", "Startup_", showname, 1);
				showname = string_replace("_LINUX_", "_", showname, 1);
				showname = string_replace("_BOXMODE_", "_B", showname, 1);

				if(issue != NULL)
				{
					showname = ostrcat(showname, "(", 1, 0);
					showname = ostrcat(showname, strstrip(issue), 1, 0);
					showname = ostrcat(showname, ") ", 1, 0);
					issue = stringreplacecharonce(issue, ' ', '\0');
					string_tolower(issue);
					pic = ostrcat(getconfig("skinpath", NULL), "/skin/", 0, 0);
					pic = ostrcat(pic, issue, 1, 0);
					pic = ostrcat(pic, ".png", 1, 0);

					if(version != NULL)
					{
						showname = ostrcat(showname, "(", 1, 0);
						showname = ostrcat(showname, strstrip(version), 1, 0);
						showname = ostrcat(showname, ")", 1, 0);
					}
				}
				else if(version == NULL)
				{
					showname = ostrcat(showname, "(", 1, 0);
#ifdef OEBUILD
					showname = ostrcat(showname, pchroot, 1, 0);
#else
					showname = ostrcat(showname, pch, 1, 0);
#endif
					showname = ostrcat(showname, ") ", 1, 0);
					showname = ostrcat(showname, _("non-version"), 1, 0);
				}
#ifdef OEBUILD
//				if(ostrcmp(pchroot, rootpart) == 0)
				if(ostrcmp(label, activelabel) == 0)
#else
				if(ostrcmp(pch, rootpart) == 0)
#endif
				{
					active = ostrcat(showname, NULL, 0, 0);
					free(pic), pic = NULL;
					pic = ostrcat(getconfig("skinpath", NULL), "/skin/active.png", 0, 0);
				}
#ifdef OEBUILD
				debug(40, "addchoicebox: device=%s, label=%s activelabel=%s showname=%s pic=%s", pchroot, label, activelabel, showname, pic);
#else
				debug(40, "addchoicebox: device=%s, label=%s showname=%s pic=%s", pch, label, showname, pic);
#endif

#ifdef OEBUILD
				cmd = ostrcat("/autofs/", pchroot, 0, 0);
#else
				cmd = ostrcat("/autofs/", pch, 0, 0);
#endif
				cmd = ostrcat(cmd, "/etc", 1, 0);

				// need switch label > showname from system_update.h function
				if(file_exist(cmd))
					addmenulist(&mlist, showname, label, pic, 0, 0);

				free(cmd), cmd = NULL;
				free(showname), showname = NULL;
				free(version), version = NULL;
				free(issue), issue = NULL;
				free(pic), pic = NULL;
			}

			pch = strtok (NULL, "\n");
			free(label), label = NULL;
			free(pchroot), pchroot = NULL;
		}
		free(pch), pch = NULL;

	}
	else
		addmenulist(&mlist, "no device found", _("no device found"), NULL, 0, 0);

	free(devicelist), devicelist = NULL;
	free(rootpart), rootpart = NULL;
#ifdef OEBUILD
	free(activelabel), activelabel = NULL;
#endif
	setmenulistdefault(mlist, active);

	mbox = menulistbox(mlist, NULL, skintitle, _("Choose your Multiboot STARTUP entry from the following list"), NULL, NULL, 1, 0);
	if(mbox != NULL)
	{
		cmd = ostrcat(cmd, "cp /boot/", 1, 0);
		cmd = ostrcat(cmd, mbox->text, 1, 0);
		cmd = ostrcat(cmd, " /boot/STARTUP", 1, 0);
		debug(40, "cmd=%s", cmd);

		system(cmd);
		free(cmd), cmd = NULL;

		oshutdown(2, 1);
	}

	free(active), active = NULL;
	freemenulist(mlist, 1); mlist = NULL;
	drawscreen(skin, 0, 0);
	resettvpic();

	return ret;
}

#endif
