#ifndef OPERA_H
#define OPERA_H

#define NAME "YWNATIVE"
#define CONTROL_PIPE_W "/tmp/"NAME"_control_w"
#define CONTROL_PIPE_R "/tmp/"NAME"_control_r"
#define RC_TITAN "/tmp/rc_enigma2"
#define OPERA_ROOT "/var/swap/titanplugins/hbbtv/opera"
#define OPERA_BIN OPERA_ROOT"/bin/opera"

extern struct skin* skin;

void operareceivercb(char* cmd);

int operarcsockfd = -1;
int operarcconnfd = -1;
int control_r_fd = -1;
char* operaplayurl = NULL;

void operarcthread()
{
	int ret = 0;

	debug(788, "opera create rc socket");

	ret = sockcreate(&operarcsockfd, RC_TITAN, 1);
	if(ret != 0) return;

	debug(788, "opera wait for rc accept");
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
	closeonexec(control_w_fd);

	buf = malloc(MINMALLOC);
	if(buf == NULL)
	{
		close(control_w_fd);
		err("no mem");
		return;
	}

	debug(788, "opera receive thread start");

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
					*pbuf = c;
					if(c == '\n') break;
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
			usleep(100000);
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

void screenopera(char* url)
{
	int rcret = 0, i = 0;
	char* tmpstr = NULL;
	void* threadstatus;
	struct stimerthread* operareceiver = NULL;

	drawscreen(skin, 0);

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
	setenv("OPERA_ROOT", OPERA_ROOT, 1);
	setenv("OPERA_FB_SIZE", "1280x720", 1);
	setenv("OPERA_DIR", OPERA_ROOT"/opera_dir", 1);
	setenv("OPERA_HOME", OPERA_ROOT"/opera_home/", 1);
	setenv("OPERA_FONTS", OPERA_ROOT"/fonts", 1);
	setenv("OPERA_WIDGETS", OPERA_ROOT"/widgets", 1);
	setenv("LD_LIBRARY_PATH", OPERA_ROOT, 1); // + ":" + os.environ["LD_LIBRARY_PATH"]

	unlink(CONTROL_PIPE_W);
	unlink(CONTROL_PIPE_R);
	unlink(RC_TITAN);

	mkfifo(CONTROL_PIPE_W, 0666);
	mkfifo(CONTROL_PIPE_R, 0666);
	//mkfifo(RC_TITAN, 0666);

	debug(788, "step 2, start opera");
	writesys("/proc/cpu/alignment", "0", 0);

	addtimer(&operarcthread, START, 10, 1, NULL, NULL, NULL);
	operareceiver = addtimer(&operareceiverthread, START, 10, 1, NULL, NULL, NULL);

	//TODO change working dir to OPERA_ROOT
	chdir(OPERA_ROOT);

	tmpstr = ostrcat(tmpstr, OPERA_BIN, 1, 0);
	tmpstr = ostrcat(tmpstr, " -u ", 1, 0);
	tmpstr = ostrcat(tmpstr, url, 1, 0);
	tmpstr = ostrcat(tmpstr, " --dfb:mode=1280x720,no-debug,no-vt,no-vt-switch &", 1, 0);
	system(tmpstr);
	free(tmpstr); tmpstr = NULL;

	while(1)
	{
		rcret = waitrc(NULL, 0, 0);

		if(rcret == getrcconfigint("rcexit", NULL))
		{
			//operasendkey("ESC");
			break;
		}
			//TODO
			//operasendkey("BACK");
		else if(rcret == getrcconfigint("rcred", NULL))
			operasendkey("RED");
		else if(rcret == getrcconfigint("rcgreen", NULL))
			operasendkey("GREEN");
		else if(rcret == getrcconfigint("rcyellow", NULL))
			operasendkey("YELLOW");
		else if(rcret == getrcconfigint("rcblue", NULL))
			operasendkey("BLUE");
		else if(rcret == getrcconfigint("rcok", NULL))
		{
			operasendkey("OK");
			operasendkey("ENTER");
		}
		else if(rcret == getrcconfigint("rcup", NULL))
			operasendkey("UP");
		else if(rcret == getrcconfigint("rcdown", NULL))
			operasendkey("DOWN");
		else if(rcret == getrcconfigint("rcleft", NULL))
			operasendkey("LEFT");
		else if(rcret == getrcconfigint("rcright", NULL))
			operasendkey("RIGHT");
		else if(rcret == getrcconfigint("rc1", NULL))
			operasendkey("1");
		else if(rcret == getrcconfigint("rc2", NULL))
			operasendkey("2");
		else if(rcret == getrcconfigint("rc3", NULL))
			operasendkey("3");
		else if(rcret == getrcconfigint("rc4", NULL))
			operasendkey("4");
		else if(rcret == getrcconfigint("rc5", NULL))
			operasendkey("5");
		else if(rcret == getrcconfigint("rc6", NULL))
			operasendkey("6");
		else if(rcret == getrcconfigint("rc7", NULL))
			operasendkey("7");
		else if(rcret == getrcconfigint("rc8", NULL))
			operasendkey("8");
		else if(rcret == getrcconfigint("rc9", NULL))
			operasendkey("9");
		else if(rcret == getrcconfigint("rc0", NULL))
			operasendkey("0");
	}

	if(operareceiver != NULL)
	{
		operareceiver->aktion = STOP;
		while(operareceiver->status != DEACTIVE)
		{
			usleep(100000);
			i++; if(i > 20) break;
		}

		if(i > 20)
		{
			err("detect hanging timer sub thread (operareceiver)");
		}
		else if(operareceiver->count < 0 && operareceiver->thread != '\0')
			pthread_join(operareceiver->thread, &threadstatus);
		pthread_attr_destroy(&operareceiver->attr);
	}

	sockclose(&operarcsockfd);
	sockclose(&operarcconnfd);
	close(control_r_fd);
	free(operaplayurl); operaplayurl = NULL;

	tmpstr = ostrcat(tmpstr, "killall opera", 1, 0);
	system(tmpstr);
	free(tmpstr); tmpstr = NULL;

	writesys("/proc/cpu/alignment", "1", 0);

	//setvideomode("720p50", 0);
	//changefbresolution("720p50");

	drawscreen(skin, 0);
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
			control_r_fd = open(CONTROL_PIPE_R, O_WRONLY);
			if(control_r_fd < 0)
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

			//TODO
			//position = "%x %x %x %x" % (x, y, w, h)
			writesys("/proc/stb/vmpeg/0/dst_all", position, 0);
		}
		else if(ostrcmp("AvGetFullScreen", (&ret[0])->part) == 0)
		{
			char* w = NULL, *h = NULL;

			w = readsys("/proc/stb/vmpeg/0/dst_width", 1);
			h = readsys("/proc/stb/vmpeg/0/dst_height", 1);

			if(ostrcmp(w, "2d0") == 0 && ostrcmp(h, "240") == 0)
				write(control_r_fd, "AvGetFullScreen 1\n", 18);
			else
				write(control_r_fd, "AvGetFullScreen 0\n", 18);

			free(w); w = NULL;
			free(h); h = NULL;
		}
		else if(ostrcmp("AvGetVisible", (&ret[0])->part) == 0)
		{
			unsigned long val = readsysul("/proc/stb/video/alpha", 1);

			if(val == 0)
				write(control_r_fd, "AvGetVisible 1\n", 15);
			else
				write(control_r_fd, "AvGetVisible 0\n", 15);
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
			//TODO
			if(count > 2)
			{
				if(ostrcmp("100", (&ret[2])->part) == 0) //PLAY OR RESUME
				{
					if(ostrcmp(operaplayurl, (&ret[1])->part) != 0) //DIFFERENT URI SO PLAY
					{
						free(operaplayurl); operaplayurl = NULL;
						operaplayurl = ostrcat((&ret[1])->part, NULL, 0, 0);
						//self.mp.stopStream()
						//self.mp.startStream(operaplayurl)
					}
					else
					{
						//self.mp.resumeStream()
					}
				}
				else if(ostrcmp("0", (&ret[2])->part) == 0) //PAUSE
				{
					//self.mp.pauseStream()
				}
			}
		}
		else if(ostrcmp("AvStop", (&ret[0])->part) == 0)
		{
			//TODO
			//self.mp.stopStream()
		}
		else if(ostrcmp("AvGetPos", (&ret[0])->part) == 0)
		{
			char* tmppos = NULL;
			unsigned long long int pos = 0;
			//TODO
			//pos = self.mp.getPosition()

			tmppos = ostrcat(tmppos, "AvGetPos ", 1, 0);
			tmppos = ostrcat(tmppos, oitoa(pos), 1, 1);
			tmppos = ostrcat(tmppos, "\n", 1, 0);
			if(tmppos != NULL)
				write(control_r_fd, tmppos, strlen(tmppos));
			free(tmppos); tmppos = NULL;
		}
		else if(ostrcmp("AvGetDuration", (&ret[0])->part) == 0)
		{
			char* tmplen = NULL;
			unsigned long long int  len = 0;
			//TODO
			//len = self.mp.getDuration()

			tmplen = ostrcat(tmplen, "AvGetDuration ", 1, 0);
			tmplen = ostrcat(tmplen, oitoa(len), 1, 1);
			tmplen = ostrcat(tmplen, "\n", 1, 0);
			if(tmplen != NULL)
				write(control_r_fd, tmplen, strlen(tmplen));
			free(tmplen); tmplen = NULL;
		}
		else if(ostrcmp("AvGetState", (&ret[0])->part) == 0)
		{
			char* tmpstate = NULL;
			int state = 0;
			//TODO
			//state = self.mp.getState()

			tmpstate = ostrcat(tmpstate, "AvGetState ", 1, 0);
			tmpstate = ostrcat(tmpstate, oitoa(state), 1, 1);
			tmpstate = ostrcat(tmpstate, "\n", 1, 0);
			if(tmpstate != NULL)
				write(control_r_fd, tmpstate, strlen(tmpstate));
			free(tmpstate); tmpstate = NULL;
		}
		else if(ostrcmp("ReleaseHandle", (&ret[0])->part) == 0)
		{
			if(count > 1 && ostrcmp("1", (&ret[1])->part) == 0) //VOD
			{
				//Switch back to live tv after vod session ended
				//TODO
				//gSession.nav.playService(self.service)
			}
		}
	}

	free(tmpstr); tmpstr = NULL;
	free(ret); ret = NULL;
}

#endif
