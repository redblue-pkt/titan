#ifndef OPERA_H
#define OPERA_H

struct hbbtvfav
{
	char* name;
	char* addr;
	struct hbbtvfav* next;
};

struct hbbtvfav *hbbtvfav = NULL;

#define NAME "YWNATIVE"
#define CONTROL_PIPE_W "/tmp/"NAME"_control_w"
#define CONTROL_PIPE_R "/tmp/"NAME"_control_r"
#define RC_TITAN "/tmp/rc_enigma2"

extern struct skin* skin;

void operareceivercb(char* cmd);

int operarcsockfd = -1;
int operarcconnfd = -1;
int opera_control_r_fd = -1;
int operarcthread_ok = 0;
int operareceiverthread_ok = 0;
//100 = live play
//0 = pause
//1 = play
//2 = stop
int operaservicestate = 100;
char* operaplayurl = NULL;

struct hbbtvfav* addhbbtvfav(char *line, int count, struct hbbtvfav* last)
{
	//debug(1000, "in");
	struct hbbtvfav *newnode = NULL, *prev = NULL, *node = hbbtvfav;
	int ret = 0;

	newnode = (struct hbbtvfav*)calloc(1, sizeof(struct hbbtvfav));
	if(newnode == NULL)
	{
		err("no memory");
		return NULL;
	}

	newnode->name = malloc(1024);
	if(newnode->name == NULL)
	{
		err("no mem");
		free(newnode);
		return NULL;
	}

	newnode->addr = malloc(1024);
	if(newnode->addr == NULL)
	{
		err("no mem");
		free(newnode->name);
		free(newnode);
		return NULL;
	}

	ret = sscanf(line, "%[^#]#%s", newnode->name, newnode->addr);
	if(ret != 2)
	{
		if(count > 0)
		{
			err("hbbtvfav line %d not ok", count);
		}
		else
		{
			err("add hbbtvfav");
		}
		free(newnode->name);
		free(newnode->addr);
		free(newnode);
		return NULL;
	}

	newnode->name = ostrshrink(newnode->name);
	newnode->addr = ostrshrink(newnode->addr);

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
		hbbtvfav = newnode;
	else
		prev->next = newnode;
	newnode->next = node;

	//debug(1000, "out");
	return newnode;
}

int readhbbtvfav(const char* filename)
{
	debug(1000, "in");
	FILE *fd = NULL;
	char *fileline = NULL;
	int linecount = 0, len = 0;
	struct hbbtvfav* last = NULL, *tmplast = NULL;

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
		len = strlen(fileline) - 1;
		if(len >= 0 && fileline[len] == '\n')
			fileline[len] = '\0';
		len--;
		if(len >= 0 && fileline[len] == '\r')
			fileline[len] = '\0';

		linecount++;

		if(last == NULL) last = tmplast;
		last = addhbbtvfav(fileline, linecount, last);
		if(last != NULL) tmplast = last;
	}

	free(fileline);
	fclose(fd);
	return 0;
}

int delhbbtvfav(char* name)
{
	debug(1000, "in");
	int ret = 1;
	struct hbbtvfav *node = hbbtvfav, *prev = hbbtvfav;

	while(node != NULL)
	{
		if(ostrcmp(node->name, name) == 0)
		{
			if(node == hbbtvfav)
				hbbtvfav = node->next;
			else
				prev->next = node->next;

			free(node);
			node = NULL;
			ret = 0;
			break;
		}

		prev = node;
		node = node->next;
	}
	debug(1000, "out");
	return ret;
}

void freehbbtvfav()
{
	debug(1000, "in");
	struct hbbtvfav *node = hbbtvfav, *prev = hbbtvfav;

	while(node != NULL)
	{
		prev = node;
		node = node->next;
		if(prev != NULL)
			delhbbtvfav(prev->name);
	}
	debug(1000, "out");
}

void operarcthread()
{
	int ret = 0;

	debug(788, "opera create rc socket");

	ret = sockcreate(&operarcsockfd, RC_TITAN, 1);
	if(ret != 0) return;

	//closeonexec(operarcsockfd);

	debug(788, "opera wait for rc accept");
	operarcthread_ok = 1;
	operarcconnfd = sockaccept(&operarcsockfd, 0);
	debug(788, "opera got rc accept");
}

void operareceiverthread(struct stimerthread* self)
{
	int ret = 0, control_w_fd = -1;
	fd_set rfds;
	struct timeval tv;
	unsigned char* buf = NULL;

	if(self == NULL) return;

	control_w_fd = open(CONTROL_PIPE_W, O_RDONLY);
	if(control_w_fd < 0)
	{
		perr("open or create %s", CONTROL_PIPE_W);
		return;
	}

	fcntl(control_w_fd, F_SETFL, fcntl(control_w_fd, F_GETFL) | O_NONBLOCK);
	//closeonexec(control_w_fd);

	buf = malloc(MINMALLOC);
	if(buf == NULL)
	{
		close(control_w_fd);
		err("no mem");
		return;
	}

	debug(788, "opera receive thread start");

	operareceiverthread_ok = 1;
	while(self->aktion != STOP)
	{
		tv.tv_sec = 1;
		tv.tv_usec = 0;
		FD_ZERO(&rfds);
		FD_SET(control_w_fd, &rfds);

		ret = TEMP_FAILURE_RETRY(select(control_w_fd + 1, &rfds, NULL, NULL, &tv));

		if(ret > 0)
		{
			memset(buf, 0, MINMALLOC);
			unsigned char* pbuf = buf;
			while(pbuf - buf < MINMALLOC)
			{
				unsigned char c;

				ret = read(control_w_fd, &c, 1);
				if(ret == 1)
				{
					if(c == '\n') break;
					*pbuf = c;
					pbuf++;
				}
			}
			if(pbuf != buf)
				operareceivercb((char*)buf);
		}
		else if(ret == 0) //timeout
			continue;
		else //error
		{
			perr("select failed");
			usleep(10000);
		}
	 }

	 close(control_w_fd); control_w_fd = -1;
	 free(buf); buf = NULL;
	 debug(788, "opera receive thread end");
}

void operasendkey(char* rckey)
{
	if(operarcconnfd > -1 && rckey != NULL)
		socksend(&operarcconnfd, (unsigned char*)rckey, strlen(rckey), -1);
	else
	{
		err("no opera rc context sockfd=%d", operarcconnfd);
	}
}

//flag 0: stop live tv and start play service
//flag 1: stop play and start live service
//flag 2: stop play
int operaservice(char* url, int flag)
{
	char* tmpstr = NULL;

	//stop live tv service and start play
	if(flag == 0)
	{
		if(operaservicestate == 100)
		{
			int ret = servicestop(status.aktservice, 1, 1);
			if(ret == 1) return 1;
		}

		free(operaplayurl); operaplayurl = NULL;
		operaplayurl = ostrcat(url, NULL, 0, 0);
		playerstop();
		playerafterend();
		playerstart(operaplayurl);
		operaservicestate = 1;
	}

	//stop play and start live service
	if(flag == 1 && operaservicestate != 100)
	{
		//stop player if running
		playerstop();
		playerafterend();
		free(operaplayurl); operaplayurl = NULL;

		if(status.aktservice->channel != NULL)
		{
			tmpstr = ostrcat(status.aktservice->channellist, NULL, 0, 0);
			servicestart(status.aktservice->channel, tmpstr, NULL, 3);
		}
		else
		{
			tmpstr = ostrcat(status.lastservice->channellist, NULL, 0, 0);
			servicestart(status.lastservice->channel, tmpstr, NULL, 0);
		}
		free(tmpstr); tmpstr = NULL;
		operaservicestate = 100;
	}

	//stop play
	if(flag == 2 && operaservicestate != 100)
	{
		playerstop();
		playerafterend();
		free(operaplayurl); operaplayurl = NULL;
		operaservicestate = 2;
	}

	return 0;
}

void screenopera(char* url)
{
	int rcret = 0, i = 0;
	char* tmpstr = NULL, *savedir = NULL, *dirbuf = NULL;;
	struct stimerthread* operareceiver = NULL;
	char* opera_root, *opera_bin = NULL, *opera_dir = NULL;
	char* opera_home = NULL, *opera_fonts = NULL, *opera_widgets = NULL;
	
	textbox(_("Message"), _("You can leave HBBTV with Record Key."), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 1000, 200, 15, 0);
	
  if(isdir("/var/usr/local/share/titan/plugins/player/hbbtv/opera"))
		opera_root = ostrcat("/var/usr/local/share/titan/plugins/player/hbbtv/opera", NULL, 0, 0);
	else if(isdir("/var/swap/usr/local/share/titan/plugins/player/hbbtv/opera"))
		opera_root = ostrcat("/var/swap/usr/local/share/titan/plugins/player/hbbtv/opera", NULL, 0, 0);
	else
		opera_root = ostrcat("/mnt/swapextensions/usr/local/share/titan/plugins/player/hbbtv/opera", NULL, 0, 0);

	opera_bin = ostrcat(opera_root, "/bin/opera", 0, 0);
	opera_dir = ostrcat(opera_root, "/opera_dir", 0, 0);
	opera_home = ostrcat(opera_root, "/opera_home/", 0, 0);
	opera_fonts = ostrcat(opera_root, "/fonts", 0, 0);
	opera_widgets = ostrcat(opera_root, "/widgets", 0, 0);

	resettvpic();
	drawscreen(skin, 0, 0);

	debug(788, "step 1, set env");
	//setenv("DFBARGS", "pixelformat=ARGB,no-cursor,bg-none,no-linux-input-grab,no-vt", 1);
	setenv("OPERA_MULTITAP", "NO", 1);
	setenv("OPERA_SHOW_STATUSWINDOW", "NO", 1);
	setenv("OPERA_FB_BORDERWIDTH", "0", 1);
	setenv("OPERA_SHOW_IMEWINDOW", "NO", 1);
	setenv("OPERA_SHOW_NAVIGATIONWINDOW", "NO", 1);
	setenv("OPERA_SHOW_MOUSEPOINTER", "NO", 1);
	setenv("OPERA_SHOW_HIGHLIGHT", "NO", 1);
	setenv("OPERA_ESC_EXIT", "YES", 1);
	setenv("FREETYPE_FONT_SET", "YES", 1);
	setenv("OPERA_ROOT", opera_root, 1);
	setenv("OPERA_FB_SIZE", "1280x720", 1);
	setenv("OPERA_DIR", opera_dir, 1);
	setenv("OPERA_HOME", opera_home, 1);
	setenv("OPERA_FONTS", opera_fonts, 1);
	setenv("OPERA_WIDGETS", opera_widgets, 1);
	setenv("LD_LIBRARY_PATH", opera_root, 1); // + ":" + os.environ["LD_LIBRARY_PATH"]

	unlink(CONTROL_PIPE_W);
	unlink(CONTROL_PIPE_R);
	unlink(RC_TITAN);

	mkfifo(CONTROL_PIPE_W, 0666);
	mkfifo(CONTROL_PIPE_R, 0666);
	//mkfifo(RC_TITAN, 0666);

	debug(788, "step 2, start opera");
	//writesys("/proc/cpu/alignment", "0", 0);

	operarcthread_ok = 0;
	operareceiverthread_ok = 0;
	addtimer(&operarcthread, START, 10, 1, NULL, NULL, NULL);
	operareceiver = addtimer(&operareceiverthread, START, 10, 1, NULL, NULL, NULL);

	//wait for threads
	int count = 0;
	while(operarcthread_ok == 0 || operareceiverthread_ok == 0)
	{
		usleep(100000);
		count++;
		if(count > 20) break;
	}

	//save working dir
	dirbuf = malloc(PATH_MAX);
	if(dirbuf != NULL)
	{
		savedir = getcwd(dirbuf, PATH_MAX);
		chdir(opera_root);
	}

	fbsave();

	tmpstr = ostrcat(tmpstr, opera_bin, 1, 0);
	tmpstr = ostrcat(tmpstr, " -u ", 1, 0);
	tmpstr = ostrcat(tmpstr, url, 1, 0);
	tmpstr = ostrcat(tmpstr, " --dfb:mode=1280x720,no-debug,no-vt,no-vt-switch &", 1, 0);
	system(tmpstr);
	free(tmpstr); tmpstr = NULL;

	//free all opera vars
	free(opera_root); opera_root = NULL;
	free(opera_bin); opera_bin = NULL;
	free(opera_dir); opera_dir = NULL;
	free(opera_home); opera_home = NULL;
	free(opera_fonts); opera_fonts = NULL;
	free(opera_widgets); opera_widgets = NULL;

	//reset working dir
	if(savedir != NULL)
		chdir(dirbuf);
	free(dirbuf); dirbuf = NULL;

	while(1)
	{
		rcret = waitrc(NULL, 1000, 0);

		//check for player EOF
		if(operaservicestate < 2 && !playerisplaying())
			operaservice(NULL, 1);

		if(rcret == getrcconfigint("rchbbtv", NULL) || rcret == getrcconfigint("rcrecord", NULL))
		{
			//operasendkey("ESC\n");
			break;
		}
		else if(rcret == getrcconfigint("rcexit", NULL))
			operasendkey("BACK\n");
		else if(rcret == getrcconfigint("rcred", NULL))
			operasendkey("RED\n");
		else if(rcret == getrcconfigint("rcgreen", NULL))
			operasendkey("GREEN\n");
		else if(rcret == getrcconfigint("rcyellow", NULL))
			operasendkey("YELLOW\n");
		else if(rcret == getrcconfigint("rcblue", NULL))
			operasendkey("BLUE\n");
		else if(rcret == getrcconfigint("rcok", NULL))
		{
			operasendkey("OK\n");
			operasendkey("ENTER\n");
		}
		else if(rcret == getrcconfigint("rcup", NULL))
			operasendkey("UP\n");
		else if(rcret == getrcconfigint("rcdown", NULL))
			operasendkey("DOWN\n");
		else if(rcret == getrcconfigint("rcleft", NULL))
			operasendkey("LEFT\n");
		else if(rcret == getrcconfigint("rcright", NULL))
			operasendkey("RIGHT\n");
		else if(rcret == getrcconfigint("rc1", NULL))
			operasendkey("1\n");
		else if(rcret == getrcconfigint("rc2", NULL))
			operasendkey("2\n");
		else if(rcret == getrcconfigint("rc3", NULL))
			operasendkey("3\n");
		else if(rcret == getrcconfigint("rc4", NULL))
			operasendkey("4\n");
		else if(rcret == getrcconfigint("rc5", NULL))
			operasendkey("5\n");
		else if(rcret == getrcconfigint("rc6", NULL))
			operasendkey("6\n");
		else if(rcret == getrcconfigint("rc7", NULL))
			operasendkey("7\n");
		else if(rcret == getrcconfigint("rc8", NULL))
			operasendkey("8\n");
		else if(rcret == getrcconfigint("rc9", NULL))
			operasendkey("9\n");
		else if(rcret == getrcconfigint("rc0", NULL))
			operasendkey("0\n");
	}

	if(operareceiver != NULL)
	{
		operareceiver->aktion = STOP;
		while(operareceiver != NULL && operareceiver->status != DEACTIVE)
		{
			usleep(100000);
			i++; if(i > 20) break;
		}

		if(i > 20)
		{
			err("detect hanging timer sub thread (operareceiver)");
		}
	}

	sockclose(&operarcsockfd);
	sockclose(&operarcconnfd);
	close(opera_control_r_fd);

	debug(788, "killall -9 opera");
	tmpstr = ostrcat(tmpstr, "killall -9 opera", 1, 0);
	system(tmpstr);
	free(tmpstr); tmpstr = NULL;

	free(operaplayurl); operaplayurl = NULL;

	//writesys("/proc/cpu/alignment", "3", 0);

	//setvideomode("720p50", 0);
	//changefbresolution("720p50");
	//sleep(3);
	fbrestore();

	operaservice(NULL, 1); //stop play, start live tv

	//reset tv pic size
	status.tvpic = 1;
	resettvpic();
	clearfball();
}

void operareceivercb(char* cmd)
{
	int count = 0;
	char* tmpstr = NULL;
	struct splitstr* ret = NULL;

	debug(788, "cmd=%s", cmd);

	tmpstr = ostrcat(cmd, NULL, 0, 0);
	ret = strsplit(tmpstr, " ", &count);

	if(ret != NULL && count > 0)
	{
		if(ostrcmp("HELLO", (&ret[0])->part) == 0)
		{
			opera_control_r_fd = open(CONTROL_PIPE_R, O_WRONLY);
			if(opera_control_r_fd < 0)
			{
				perr("open or create "CONTROL_PIPE_R);
			}
		}
		else if(ostrcmp("AvSetWin", (&ret[0])->part) == 0)
		{
			int x = 0, y = 0, w = 0, h = 0;
			float xw = 720.0 / 1280.0;
			float yh = 576.0 / 720.0;
			char* position = NULL;

			if(count > 1) x = atoi((&ret[1])->part) * xw;
			if(count > 2) y = atoi((&ret[2])->part) * yh;
			if(count > 3) w = atoi((&ret[3])->part) * xw;
			if(count > 4) h = atoi((&ret[4])->part) * yh;

			if(w == 0 && h == 0)
			{
				w = 720;
				h = 576;
			}

			position = ostrcat(position, oitoax(x), 1, 1);
			position = ostrcat(position, " ", 1, 0);
			position = ostrcat(position, oitoax(y), 1, 1);
			position = ostrcat(position, " ", 1, 0);
			position = ostrcat(position, oitoax(w), 1, 1);
			position = ostrcat(position, " ", 1, 0);
			position = ostrcat(position, oitoax(h), 1, 1);

			debug(788, "change tv pic to: %s", position);
			writesys("/proc/stb/vmpeg/0/dst_all", position, 0);
		}
		else if(ostrcmp("AvGetFullScreen", (&ret[0])->part) == 0)
		{
			char* w = NULL, *h = NULL;

			w = readsys("/proc/stb/vmpeg/0/dst_width", 1);
			h = readsys("/proc/stb/vmpeg/0/dst_height", 1);

			if(ostrcmp(w, "2d0") == 0 && ostrcmp(h, "240") == 0)
				write(opera_control_r_fd, "AvGetFullScreen 1\n", 18);
			else
				write(opera_control_r_fd, "AvGetFullScreen 0\n", 18);

			free(w); w = NULL;
			free(h); h = NULL;
		}
		else if(ostrcmp("AvGetVisible", (&ret[0])->part) == 0)
		{
			unsigned long val = readsysul("/proc/stb/video/alpha", 1);

			if(val == 0)
				write(opera_control_r_fd, "AvGetVisible 1\n", 15);
			else
				write(opera_control_r_fd, "AvGetVisible 0\n", 15);
		}
		/*
		else if(ostrncmp("AvSetVisible", (&ret[0])->part) == 0)
		{
			//if(count > 1 && ostrcmp("1", (&ret[1])->part) == 0)
			//{
			//	writesys("/proc/stb/video/alpha", "255", 0);
			//else
			//	writesys("/proc/stb/video/alpha", "0", 0);
			//}
		}
		*/
		else if(ostrcmp("AvPlay", (&ret[0])->part) == 0)
		{
			if(count > 2)
			{
				if(ostrcmp("100", (&ret[2])->part) == 0) //play or continue
				{
					if(ostrcmp(operaplayurl, (&ret[1])->part) != 0) //different url, so play
					{
						operaservice((&ret[1])->part, 0); //stop live tv and play
					}
					else
					{
						playercontinue();
						operaservicestate = 1;
					}
				}
				else if(ostrcmp("0", (&ret[2])->part) == 0) //pause
				{
					playerpause();
					operaservicestate = 0;
				}
			}
		}
		else if(ostrcmp("AvStop", (&ret[0])->part) == 0)
		{
			operaservice(NULL, 2); //stop play
		}
		else if(ostrcmp("AvGetPos", (&ret[0])->part) == 0)
		{
			char* tmppos = NULL;
			unsigned long pos = 0;

			pos = playergetpts() / 90000;

			tmppos = ostrcat(tmppos, "AvGetPos ", 1, 0);
			tmppos = ostrcat(tmppos, olutoa(pos), 1, 1);
			tmppos = ostrcat(tmppos, "\n", 1, 0);

			if(tmppos != NULL)
				write(opera_control_r_fd, tmppos, strlen(tmppos));

			free(tmppos); tmppos = NULL;
		}
		else if(ostrcmp("AvGetDuration", (&ret[0])->part) == 0)
		{
			char* tmplen = NULL;
			unsigned long len = 0;

			len = playergetlength();

			tmplen = ostrcat(tmplen, "AvGetDuration ", 1, 0);
			tmplen = ostrcat(tmplen, olutoa(len), 1, 1);
			tmplen = ostrcat(tmplen, "\n", 1, 0);

			if(tmplen != NULL)
				write(opera_control_r_fd, tmplen, strlen(tmplen));

			free(tmplen); tmplen = NULL;
		}
		else if(ostrcmp("AvGetState", (&ret[0])->part) == 0)
		{
			char* tmpstate = NULL;

			tmpstate = ostrcat(tmpstate, "AvGetState ", 1, 0);
			if(operaservicestate == 1)
				tmpstate = ostrcat(tmpstate, "1", 1, 0);
			else
				tmpstate = ostrcat(tmpstate, "0", 1, 0);
			tmpstate = ostrcat(tmpstate, "\n", 1, 0);

			if(tmpstate != NULL)
				write(opera_control_r_fd, tmpstate, strlen(tmpstate));

			free(tmpstate); tmpstate = NULL;
		}
		else if(ostrcmp("ReleaseHandle", (&ret[0])->part) == 0)
		{
			if(count > 1 && ostrcmp("1", (&ret[1])->part) == 0) //VOD
			{
				operaservice(NULL, 1); //stop play, start live tv
			}
		}
	}

	free(tmpstr); tmpstr = NULL;
	free(ret); ret = NULL;
}

void screenoperafav()
{
	//int ret = 0;
	struct menulist* mlist = NULL, *mbox = NULL, *tmpmbox = NULL;
	struct hbbtvfav *node = NULL;

	//ret = servicestop(status.aktservice, 1, 0);
	//if(ret == 1) return;
	//setfbtransparent(255);

	readhbbtvfav(getconfig("hbbtvfavfile", NULL));
	node = hbbtvfav;

	if(status.aktservice->channel != NULL && status.aktservice->channel->hbbtvurl != NULL)
	{
		debug(788, "hbbtvurl=%s", status.aktservice->channel->hbbtvurl);
		tmpmbox = addmenulist(&mlist, _("Channel HBBTV"), NULL, NULL, 0, 0);
		if(tmpmbox != NULL)
			tmpmbox->param = ostrcat(status.aktservice->channel->hbbtvurl, NULL, 0, 0);
	}

	while(node != NULL)
	{
		tmpmbox = addmenulist(&mlist, node->name, NULL, NULL, 0, 0);
		if(tmpmbox != NULL)
			tmpmbox->param = ostrcat(node->addr, NULL, 0, 0);

		node = node->next;
	}

start:
	drawscreen(skin, 0, 0);
	mbox = menulistbox(mlist, "menulist", _("HBBTV Favoriten"), _("Choose your HBBTV Favorite from the following list"), NULL, NULL, 0, 0);
	if(mbox != NULL)
	{
		if(mbox->param != NULL)
		{
			drawscreen(skin, 0, 0);
			screenopera(mbox->param);
		}
		goto start;
	}

	freemenulist(mlist, 1);
	freehbbtvfav();
	//setosdtransparent(getskinconfigint("osdtransparent", NULL));
	//if(status.lastservice != NULL)
	//	servicestart(status.lastservice->channel, NULL, NULL, 0);
	//flushrc(500);
}

#endif
