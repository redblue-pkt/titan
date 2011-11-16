#ifndef GLOBAL_H
#define GLOBAL_H

//can use to disable a menu for a box (node type must be MENU) 	 	 

int checkmenuforbox(char *name)
{
	if(checkbox("atemio500") == 1)
	{
		if(ostrcmp("vfdisplay", name) == 0) return 0;
		if(ostrcmp("savesettings", name) == 0) return 0;
		if(ostrcmp("recordpath", name) == 0) return 0;
		if(ostrcmp("videosettings", name) == 0) return 0;
		if(ostrcmp("timer", name) == 0) return 0;
		if(ostrcmp("scartrecorder", name) == 0) return 0;
		if(ostrcmp("information", name) == 0) return 0;
		if(ostrcmp("plugins", name) == 0) return 0;
		if(ostrcmp("vfdisplay", name) == 0) return 0;
		if(ostrcmp("aafpanel_system_aafwizard", name) == 0) return 0;
		if(ostrcmp("aafpanel_settings_overclocking", name) == 0) return 0;
		if(ostrcmp("aafpanel_settings_fancontrol", name) == 0) return 0;
		if(ostrcmp("aafpanel_settings_automount", name) == 0) return 0;
		if(ostrcmp("aafpanel_settings_autostart", name) == 0) return 0;
		if(ostrcmp("aafpanel_settings_autostart", name) == 0) return 0;												
	}
	return 1;
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

		status.servicetype = chnode->servicetype;
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
			tmpstr = olutoa(status.aktservice->channel->transponderid);
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
			tmpstr = olutoa(status.aktservice->channel->transponderid);
			addconfig("rtransponderid", tmpstr);
                	free(tmpstr); tmpstr = NULL;
		}
}

char* createpiconpath(struct channel* chnode)
{
	char* tmpstr = NULL, *tmpnr = NULL;

	if(chnode != NULL)
	{
		tmpstr = ostrcat(tmpstr, getconfig("piconpath", NULL), 1, 0);
		tmpstr = ostrcat(tmpstr, "/", 1, 0);

		tmpnr = oitoa(chnode->serviceid);
		tmpstr = ostrcat(tmpstr, tmpnr, 1, 1);
		tmpnr = NULL;
		tmpstr = ostrcat(tmpstr, "-", 1, 0);
		tmpnr = olutoa(chnode->transponderid);
		tmpstr = ostrcat(tmpstr, tmpnr, 1, 1);
		tmpnr = NULL;
		tmpstr = ostrcat(tmpstr, ".png", 1, 0);
	}
	if(!isfile(tmpstr))
	{
		free(tmpstr); tmpstr = NULL;
		tmpstr = ostrcat(tmpstr, getconfig("piconpath", NULL), 1, 0);
		tmpstr = ostrcat(tmpstr, "/", 1, 0);
		tmpstr = ostrcat(tmpstr, "default.png", 1, 0);
	}

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
			status.videosizevalid = 1;
	}
	
	return 1;
}

int ostrftime(char* buf, int count, char* format, struct tm* t)
{
	int ret = -1;

	if(format != NULL)
		ret = strftime(buf, count, format, t);
	else
		ret = strftime(buf, count, "%d-%m-%Y %H:%M", t);

	return ret;
}

//flag 0 = write all
//flag 1 = don't write epg
int writeallconfig(int flag)
{
	debug(1000, "in");
	char* tmpstr = NULL;
	int ret = 0;

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
	if(status.writeconfig == 1)
		if(writeconfig(status.configfile) != 0)
			ret = 1;
	if(flag == 0 && time(NULL) > 1072224000) // 01.01.2004
	{
		tmpstr = createpath(getconfig("epg_path", NULL), "epg.dat");
		if(writeepg(tmpstr) != 0)
			ret = 1;
		free(tmpstr); tmpstr = NULL;
	}

	sync();

	debug(1000, "out");
	return ret;
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

	buf = malloc(MINMALLOC);
	if(buf == NULL)
	{
		err("no mem");
		return NULL;
	}
	memset(buf, 0, MINMALLOC);
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

	buf = ostrcat(buf, NULL, 1, 0);
	return buf;
}

void htmldecode(char* to, char* from)
{
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
	debug(1000, "in");
	char* wakeuptimerprog = NULL;
	struct tm *loctime = NULL;
	char *buf = NULL;
	int ret = -1;

	wakeuptimerprog = getconfig("wakeuptimerprog", NULL);

	if(wakeuptimerprog == NULL)
	{
		debug(1000, "out -> NULL detect");
		return ret;
	}

	buf = malloc(MINMALLOC);
	if(buf == NULL)
	{
		err("no memory");
		return ret;
	}

	loctime = olocaltime(&waketime);

	if(loctime != NULL)
		ostrftime(buf, MINMALLOC, wakeuptimerprog, loctime);

	if(buf != NULL)
	{
		debug(100, "set wake up timer (%s)", buf);
		ret = system(buf);
	}

	free(loctime);
	free(buf);
	debug(1000, "out");
	return ret;
}

int autoresolution()
{
	debug(1000, "in");
	char *hotpluginfo = NULL;
	char *value = NULL;
	
	if(getconfig("av_videomode", NULL) != NULL || getconfig("av_colorformat", NULL))
		return 1;

	hotpluginfo = getconfig("hotpluginfo", NULL);

	if(hotpluginfo == NULL)
	{
		debug(1000, "out -> NULL detect");
		return 1;
	}

	value = readsys(hotpluginfo, 1);
	if(value == NULL)
	{
		debug(1000, "out -> NULL detect");
		return 1;
	}
	
	if(value[0] == 'y')
	{
		setvideomode("720p50"); 
		changefbresolution("720p50");
		setcolorformat("hdmi_rgb");
	}
	else
	{
		setvideomode("576i50"); 
		changefbresolution("576i50");
		setcolorformat("rgb");
	}

	free(value);
	debug(1000, "out");
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
void delspezchar(char* text, int flag)
{
	char* tmpstr = text;

	while(*tmpstr != '\0')
	{
		if(flag == 1)
		{
			if(tmpstr[0] == '/')
				tmpstr[0] = '-';
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
		default: debug(900, " unknown mutex lock"); break;
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
		default: debug(900, " unknown mutex unlock"); break;
	}
	pthread_mutex_unlock(mutex);
}

void debugstack(void* address, void* address1)
{
	void* trace[10];
	size_t size;
	size = backtrace(trace, 10);

	FILE* fd = NULL;
	void* akttrace[2];
	int i;
	char **strings;
	char **aktstring;
	time_t rawtime;

	strings = backtrace_symbols(trace, size);
	akttrace[0] = (void*)address1;
	akttrace[1] = (void*)address;
	aktstring = backtrace_symbols(akttrace, 2); //get fault funktion name

	printf("--------------------------------------\n");
	printf("Obtaining %zd stack frames:\n\n", size);

	for(i = 0; i < size; i++)
		printf("%s\n", strings[i]);

	printf("\nLast functions:\n\n");
	printf("%s\n", aktstring[0]);
	printf("%s\n", aktstring[1]);

	printf("--------------------------------------\n");

	fd = fopen(getconfig("tracelog", NULL), "a");
	if(fd != NULL)
	{
		time(&rawtime);
		fprintf(fd, "Date: %s", ctime(&rawtime));
		fprintf(fd, "Obtaining %zd stack frames:\n\n", size);
		for(i = 1; i < size; i++)
			fprintf(fd, "%s\n", strings[i]);
		fprintf(fd, "\nLast functions:\n\n");
		fprintf(fd, "%s\n", aktstring[0]);
		fprintf(fd, "%s\n", aktstring[1]);
		fprintf(fd, "--------------------------------------\n\n");
		fclose(fd);
		sync();
	}
	else
		perr("open %s", getconfig("tracelog", NULL));

	free(strings);
	free(aktstring);
}

int mountauto(const char *file, const char *dir, unsigned long int flag, const void *data)
{
	int ret = -1;
	char* saveptr = NULL, *filesystems = NULL, *tmpfilesystems = NULL, *token = NULL;
	
	filesystems = getconfig("filesystems", NULL);
	tmpfilesystems = ostrcat(tmpfilesystems, filesystems, 0, 0);

	token = strtok_r(tmpfilesystems, ";" , &saveptr);
	while(token != NULL)
	{
		ret = mount(file, dir, token, flag, data);
		if(ret == 0) break;
		token = strtok_r(NULL, ";" , &saveptr);
	}

	free(tmpfilesystems);
	return ret;
}

int getwaswakuptimer()
{
	debug(1000, "in");
	int ret = 0;
	char *waswakeuptimerdev = NULL;
	char *value = NULL;

	waswakeuptimerdev = getconfig("waswakeuptimerdev", NULL);

	if(waswakeuptimerdev == NULL)
	{
		debug(1000, "out -> NULL detect");
		return 0;
	}

	value = readsys(waswakeuptimerdev, 1);
	if(value == NULL)
	{
		debug(1000, "out -> NULL detect");
		return 0;
	}

	// start from timer
	if(atoi(value) == 1) ret = 1;

	free(value);
	debug(1000, "out");
	return ret;
}

int setwakeuptimerdev(time_t value)
{
	debug(1000, "in");
	char* wakeuptimerdev, *tmpstr = NULL;
	int ret = 0;

	wakeuptimerdev = getconfig("wakeuptimerdev", NULL);

	if(wakeuptimerdev != NULL && value >= time(NULL))
	{
		debug(100, "set %s to %ld", wakeuptimerdev, value);
		tmpstr = olutoa(value);
		ret = writesys(wakeuptimerdev, tmpstr, 0);
		free(tmpstr); tmpstr = NULL;
		return ret;
	}

	debug(1000, "out");
	return 0;
}

int changepolicy()
{
	debug(1000, "in");
	char *tmppolicy = NULL, *tmpstr = NULL;

 	tmppolicy = getpolicy();

	if(!ostrncmp("letterbox", tmppolicy, 8))
		tmpstr = ostrcat(tmpstr, "panscan", 1, 0);
	else if(!ostrncmp("panscan", tmppolicy, 7))
		tmpstr = ostrcat(tmpstr, "non", 1, 0);
	else if(!ostrncmp("non", tmppolicy, 3))
		tmpstr = ostrcat(tmpstr, "bestfit", 1, 0);
	else if(!ostrncmp("bestfit", tmppolicy, 7))
		tmpstr = ostrcat(tmpstr, "letterbox", 1, 0);

	setpolicy(tmpstr);

	free(tmpstr); tmpstr = NULL;
	free(tmppolicy); tmppolicy = NULL;
	debug(1000, "out");
	return 0;
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

	newfilename = ostrcat(filename, "", 0, 0);

	zeichen = strrchr(newfilename, '.');
	if(zeichen != NULL)
	{
		zeichen[0] = '\0';
		newfilename = ostrcat(newfilename, ".epg", 1, 0);
	}

	return newfilename;
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
	if(hour < 0) hour = 0;

	snprintf(buf, 9, "%02d:%02d:%02d", hour, min, seconds);

	return buf;
}

int checkdate()
{
	time_t dvbtime = 0;

	if(time(NULL) < 1072224000 || status.timeupdatecount > 3600) // 01.01.2004
	{
		if(dvbgetdate(&dvbtime, 10000000) == 0) //10 sek
		{
			setsystime(&dvbtime);
			status.timeupdatecount = 0;
			return 0;
		}
		return 1;
	}
	return 0;
}

void closeonexec(fd)
{
	if(fd > -1)
		fcntl(fd, F_SETFD, fcntl(fd, F_GETFD) | FD_CLOEXEC);
}

void starthttpd(flag)
{
	if(flag == 1)
	{
		if(getconfigint("httpdstart", NULL) == 1 && status.httpthread == NULL)
		{
			status.httpthread = addtimer(&httpdthreadfunc, START, 10000, -1, NULL, NULL, NULL);
		}
	}
	else if(status.httpthread != NULL)
	{
		status.httpthread->aktion = STOP;
		status.httpthread = NULL;
	}
}

int checkbox(char* box)
{
	char* boxversion = NULL;
	int ret = 0;
	
	if(status.boxtype == NULL)
	{
		if(isfile("/etc/model")	== 0) return 0;
		boxversion = string_toupper(readsys("/etc/model", 1));
	}
	else
		boxversion = status.boxtype;

	if(ostrcmp(boxversion, box) == 0)
	{
		status.boxtype = boxversion;
		ret = 1;
	}

	box = ostrcat(box, "\n", 0, 0);
	if(ostrcmp(boxversion, box) == 0)
	{
		status.boxtype = boxversion;
		ret = 1;
	}

	if(status.boxtype == NULL)
		free(boxversion); boxversion = NULL;
	free(box); box = NULL;

	return ret;
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

int setsaturation(int value)
{
	debug(1000, "in");
	char* saturationdev;

	saturationdev = getconfig("saturationdev", NULL);

	if(saturationdev != NULL)
	{
		debug(100, "set %s to %d", saturationdev, value);
		return writesysint(saturationdev, value, 1);
	}

	debug(1000, "out");
	return 0;
}

int setbrightness(int value)
{
	debug(1000, "in");
	char* brightnessdev;

	brightnessdev = getconfig("brightnessdev", NULL);

	if(brightnessdev != NULL)
	{
		debug(100, "set %s to %d", brightnessdev, value);
		return writesysint(brightnessdev, value, 1);
	}

	debug(1000, "out");
	return 0;
}

int setcontrast(int value)
{
	debug(1000, "in");
	char* contrastdev;

	contrastdev = getconfig("contrastdev", NULL);

	if(contrastdev != NULL)
	{
		debug(100, "set %s to %d", contrastdev, value);
		return writesysint(contrastdev, value, 1);
	}

	debug(1000, "out");
	return 0;
}

int settint(int value)
{
	debug(1000, "in");
	char* tintdev;

	tintdev = getconfig("tintdev", NULL);

	if(tintdev != NULL)
	{
		debug(100, "set %s to %d", tintdev, value);
		return writesysint(tintdev, value, 1);
	}

	debug(1000, "out");
	return 0;
}

int zip(char* inbuf, int inlen, char** outbuf, int* outlen, int level)
{
	int ret = 0;
	z_stream stream;

	if(inlen > MINMALLOC)
		return 1;

	stream.zalloc = Z_NULL;
	stream.zfree = Z_NULL;
	stream.opaque = Z_NULL;

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
	*outlen = inlen - stream.avail_out;
	*outbuf = realloc(*outbuf, *outlen);

	(void)deflateEnd(&stream);
	return 0;
}

int unzip(char* inbuf, int inlen, char** outbuf, int* outlen)
{
	int ret = 0;
	z_stream stream;

	stream.zalloc = Z_NULL;
	stream.zfree = Z_NULL;
	stream.opaque = Z_NULL;
	stream.avail_in = 0;
	stream.next_in = Z_NULL;

	ret = inflateInit(&stream);
	if(ret != Z_OK)
		return 1;

	*outbuf = malloc(MINMALLOC);
	if(*outbuf == NULL)
	{
		err("no mem");
		(void)inflateEnd(&stream);
		return 1;
	}

	stream.avail_in = inlen;
	stream.next_in = (void*)inbuf;

	do
	{
		stream.avail_out = MINMALLOC;
		stream.next_out = (void*)*outbuf;

		ret = inflate(&stream, Z_NO_FLUSH);
		if(ret == Z_NEED_DICT || ret == Z_DATA_ERROR || ret == Z_MEM_ERROR)
		{
			free(*outbuf); *outbuf = NULL;
			(void)inflateEnd(&stream);
			return 1;
		}

	}
	while(stream.avail_out == 0);
	*outlen = MINMALLOC - stream.avail_out;
	*outbuf = realloc(*outbuf, *outlen);

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

long long getfreespace(char* dir)
{
	struct statfs64 s;
	long long freesize = 0;

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

char* getmaxsatstring()
{
	char* tmpstr = NULL, *tmpnr = NULL;
	int i;

	tmpstr = ostrcat(tmpstr, "1", 1, 0);
	for(i = 2; i <= status.maxsat; i++)
	{
		tmpnr = oitoa(i);
		tmpstr = ostrcat(tmpstr, "\n", 1, 0);
		tmpstr = ostrcat(tmpstr, tmpnr, 1, 1);
	}
	return tmpstr;
}

void ostrcatbig(char** value1, char* value2, int* maxlen, int* pos)
{
//	debug(1000, "in");
	int len = 0;

	if(value2 == NULL)
	{
		debug(1000, "out -> NULL detect");
		return; 
	}
	len = strlen(value2);

	if(len > MINMALLOC)
	{
		err("string to long");
		return; 
	}

	if(*value1 != NULL && maxlen == 0)
		*maxlen = strlen(*value1);

	if(*value1 == NULL || *pos + len + 1 > *maxlen)
	{
		*maxlen = *maxlen + (MINMALLOC * 10);
		*value1 = realloc(*value1, *maxlen);
		if(*value1 == NULL)
		{
			err("no memory");
			return;
		}
	}

	memcpy(*value1 + *pos, value2, len + 1);
	*pos = *pos + len;

	//debug(1000, "out");
}

char* ostrcat(char* value1, char* value2, int free1, int free2)
{
//	debug(1000, "in");
	char* buf = NULL;

	if(value1 == NULL && value2 == NULL) return NULL;

	if(value2 == NULL)
	{
		value2 = malloc(1);
		if(value2 == NULL)
		{
			err("no memory");
			return NULL;
		}
		free2 = 1;
		value2[0] = '\0';
	}

	if(value1 == NULL)
	{
		value1 = malloc(1);
		if(value1 == NULL)
		{
			err("no memory");
			return NULL;
		}
		free1 = 1;
		value1[0] = '\0';
	}

	buf = malloc(strlen(value1) + strlen(value2) + 1);
	if(buf == NULL)
	{
		err("no memory");
		return NULL;
	}

	sprintf(buf, "%s%s", value1, value2);

	if(free1 == 1) {free(value1); value1 = NULL;}
	if(free2 == 1) {free(value2); value2 = NULL;}

	//helpfull for memleak detect
	//if(buf != NULL && strlen(buf) == 0x0b - 0x01)
	//	printf("******** memleak string (%s) (%p) ********\n", buf, buf);

	//debug(1000, "out");
	return buf;
}

char* olutoa(unsigned long value)
{
	debug(1000, "in");
	char *buf = NULL, *buf1 = NULL;

	buf = malloc(MINMALLOC);
	if(buf == NULL)
	{
		err("no memory");
		return NULL;
	}

	sprintf(buf, "%lu", value);
	buf1 = ostrcat(buf, "", 1, 0);

	debug(1000, "out");
	return buf1;
}

char* oitoa(int value)
{
	debug(1000, "in");
	char *buf = NULL, *buf1 = NULL;

	buf = malloc(MINMALLOC);
	if(buf == NULL)
	{
		err("no memory");
		return NULL;
	}

	sprintf(buf, "%d", value);
	buf1 = ostrcat(buf, "", 1, 0);

	debug(1000, "out");
	return buf1;
}

char* oitoa64(off64_t value)
{
	debug(1000, "in");
	char *buf = NULL, *buf1 = NULL;

	buf = malloc(MINMALLOC);
	if(buf == NULL)
	{
		err("no memory");
		return NULL;
	}

	sprintf(buf, "%lld", value);
	buf1 = ostrcat(buf, "", 1, 0);

	debug(1000, "out");
	return buf1;
}

char* oftoa64(double value, char* count)
{
	debug(1000, "in");
	char *buf = NULL, *buf1 = NULL;
	char* tmpstr = NULL;

	buf = malloc(MINMALLOC);
	if(buf == NULL)
	{
		err("no memory");
		return NULL;
	}

	tmpstr = ostrcat("%.", count, 0, 0);
	tmpstr = ostrcat(tmpstr, "f", 1, 0);

	sprintf(buf, tmpstr, value);
	buf1 = ostrcat(buf, "", 1, 0);

	free(tmpstr);
	debug(1000, "out");
	return buf1;
}

int ostrncmp(char* value1, char* value2, int count)
{
	//debug(1000, "in");
	int ret = 1;

	if(value1 != NULL && value2 != NULL)
		ret = strncmp(value1, value2, count);

	//debug(1000, "out");
	return ret;
}

int ostrcmp(char* value1, char* value2)
{
//	debug(1000, "in");
	int ret = 1;

	if(value1 != NULL && value2 != NULL)
		ret = strcmp(value1, value2);

//	debug(1000, "out");
	return ret;
}

char* createpath(char* dir, char* file)
{
	debug(1000, "in");
	char *absdir = NULL;
	char *pos = NULL;
	char* tmpdir = NULL;

	if(dir == NULL || file == NULL)
	{
		debug(1000, "out -> NULL detect");
		return NULL;
	}

	tmpdir = ostrcat(dir, "", 0, 0);
	if(tmpdir == NULL)
	{
		debug(1000, "out -> NULL detect");
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
		err("no memory");
		return NULL;
	}

	if(strlen(file) > 0 || strlen(tmpdir) == 0)
		sprintf(absdir, "%s/%s", tmpdir, file);
	else
		sprintf(absdir, "%s", tmpdir);

	debug(1000, "out");
	free(tmpdir);
	return absdir;
}

int settimezone(char* zone)
{
	debug(1000, "in");
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
	debug(1000, "out");
	return ret;
}

int delchar(char** text, int pos)
{
	debug(1000, "in");
	char *tmptext = NULL;
	int i, y = 0;

	if(*text == NULL)
	{
		debug(1000, "out -> NULL detect");
		return pos;
	}

	tmptext = malloc(strlen(*text));
	if(tmptext == NULL)
	{
		err("no memory");
		return pos;
	}

	for(i = 0; i < strlen(*text); i++)
	{
		if(i == pos - 1)
			y++;

		tmptext[i] = (*text)[y];
		y++;
	}

	tmptext[i] = '\0';
	if(pos >= strlen(tmptext)) pos = strlen(tmptext);

	free(*text);
	*text = tmptext;
	debug(1000, "out");
	return pos;
}


int insertchar(char** text, char zeichen, int pos)
{
	debug(1000, "in");
	char *tmptext = NULL;
	int i, y = 0;

	if(*text == NULL)
	{
		debug(1000, "out -> NULL detect");
		return pos;
	}

	tmptext = malloc(strlen(*text) + 2);
	if(tmptext == NULL)
	{
		err("no memory");
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
	debug(1000, "out");
	return pos + 1;
}

void sighandler(int sig, struct sigcontext ctx)
{
	debug(1000, "in");
	switch(sig)
	{
		case SIGUSR1:
		{
			//todo all configs
			reloadconfig(status.configfile);
			reloadconfig(getconfig("ownconfig", NULL));
			break;
		}
		case SIGSEGV:
		case SIGBUS:
		case SIGABRT:
		{
#ifdef SIMULATE
			//intel
			debugstack((void *)ctx.eip, NULL);
			err("got signal %d, fault address 0x%lx from 0x%lx", sig, ctx.cr2, ctx.eip);
#else
			/* sh4
			unsigned long sc_regs[16];
			unsigned long sc_pc; //programm counter register
			unsigned long sc_pr; //procedure register
			unsigned long sc_sr; //status register
			unsigned long sc_gbr;
			unsigned long sc_mach;
			unsigned long sc_macl; */

			debugstack((void *)ctx.sc_pr, (void *)ctx.sc_pc);
			err("got signal %d, programm counter reg: 0x%lx,  procedure reg: 0x%lx", sig, ctx.sc_pc, ctx.sc_pr);
#endif
			if(getconfigint("saverun", NULL) == 1 && status.longjumpbuf != NULL)
				siglongjmp(status.longjumpbuf, 1);
			else
				exit(100);
			break;
		}
	}
	debug(1000, "out");
}

void calctext(char* buf, char* buf1, unsigned int* linecount, unsigned int* pagecount, unsigned int* poscount, int pagelen, int page)
{
	debug(1000, "in");
	unsigned int tmpposcount = 0;

	*poscount = 0;
	*linecount = 1;
	*pagecount = 1;

	page--;
	if(page < 0) page = 0;

	if(buf != NULL)
	{
		while(*buf != '\0')
		{
			if(*buf == '\n')
			{
				if(*(buf + 1) == '\0') break;
				(*linecount)++;

				if(*linecount - 1 == pagelen * page)
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

				if(*linecount - 1 == pagelen * page)
					*poscount = tmpposcount + 1;
			}
			buf1++;
			tmpposcount++;
		}
	}

	if(pagelen > 0)
		*pagecount = (int)ceil(((float)*linecount / pagelen));

	debug(1000, "out");
}

int initlocale(char *localepath)
{
	debug(1000, "in");
	setlocale(LC_ALL, "");
	if(bindtextdomain(PROGNAME, localepath) == NULL)
	{
		err("set bindtextdomain");
		return 1;
	}
	if(textdomain(PROGNAME) == NULL)
	{
		err("set textdomain");
		return 1;
	}
	debug(1000, "out");
	return 0;
}

//for langage you must
//mkdir /usr/lib/locale
//ln -s %titanpath%/po /usr/lib/locale/po
//copy SYS_LC_MESSAGES from other language into LC_MESSAGE
int setlang(char *lang)
{
	debug(1000, "in");
	char *ret;

	setenv("LANG", lang, 1);
	setenv("LANGUAGE", lang, 1);
	ret = setlocale(LC_MESSAGES, lang);
	if(ret == NULL)
	{
		err("can't set LC_MESSAGES to %s", lang);
		return 1;
	}
	debug(1000, "out");
	return 0;
}

unsigned long readsysul(const char *filename, int line)
{
	debug(1000, "in");
	int i = 0;
	unsigned long ret = 0;
	FILE *fd = NULL;
	char *fileline = NULL;
	char *buf1 = NULL;

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

	for(i = 0; i < line; i++)
	{
		memset(fileline, 0, MINMALLOC);
		fgets(fileline, MINMALLOC, fd);
	}

	if(fileline[strlen(fileline) - 1] == '\n')
		fileline[strlen(fileline) - 1] = '\0';
	buf1 = ostrcat(fileline, "", 1, 0);

	fclose(fd);

	if(buf1 != NULL)
	{
		ret = strtoul(buf1, NULL, 10);
		free(buf1); buf1 = NULL;
	}

	debug(1000, "out");
	return ret;
}


char* readsys(const char *filename, int line)
{
	debug(1000, "in");
	int i = 0;
	FILE *fd = NULL;
	char *fileline = NULL;
	char *buf1 = NULL;

	fileline = malloc(MINMALLOC);
	if(fileline == NULL)
	{
		err("no memory");
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

	if(fileline[strlen(fileline) - 1] == '\n')
		fileline[strlen(fileline) - 1] = '\0';
	buf1 = ostrcat(fileline, "", 1, 0);

	fclose(fd);

	debug(1000, "out");
	return buf1;
}

int writesys(const char *filename, char *value, int flag)
{
	debug(1000, "in");
	FILE *fd = NULL;
	char* tmpstr = NULL;
	int ret;

	if(value == NULL)
	{
		debug(1000, "out -> NULL detect");
		return 1;
	}

	fd = fopen(filename, "w");
	if(fd == NULL)
	{
		perr("can't open %s", filename);
		return 1;
	}

	if(flag == 1)
		tmpstr = ostrcat(value, "\n", 0, 0);
	else
		tmpstr = ostrcat(value, "", 0, 0);

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
	debug(1000, "out");
	return 0;
}

int writesysint(const char *filename, int value, int flag)
{
	debug(1000, "in");
	char* tmpstr = NULL;
	int ret = 0;

	tmpstr = oitoa(value);
	ret = writesys(filename, tmpstr, flag);

	free(tmpstr);
	debug(1000, "out");
	return ret;
}

char* convertspacetolf(char* value)
{
	debug(1000, "in");
	int i = 0;

	while(value[i] != '\0')
	{
		if(value[i] == ' ')
			value[i] = '\n';
		i++;
	}

	debug(1000, "out");
	return value;
}

char* getcolorformatchoices()
{
	debug(1000, "in");
	char *colorformatchoicesdev = NULL;
	char *value = NULL;

	colorformatchoicesdev = getconfig("colorformatchoicesdev", NULL);

	if(colorformatchoicesdev == NULL)
	{
		debug(1000, "out -> NULL detect");
		return NULL;
	}

	value = readsys(colorformatchoicesdev, 1);
	if(value == NULL)
	{
		debug(1000, "out -> NULL detect");
		return NULL;
	}

	value = convertspacetolf(value);

	debug(1000, "out");
	return value;
}

char* getaudiosourcechoices()
{
	debug(1000, "in");
	char *audiosourcechoicesdev = NULL;
	char *value = NULL;

	audiosourcechoicesdev = getconfig("audiosourcechoicesdev", NULL);

	if(audiosourcechoicesdev == NULL)
	{
		debug(1000, "out -> NULL detect");
		return NULL;
	}

	value = readsys(audiosourcechoicesdev, 1);
	if(value == NULL)
	{
		debug(1000, "out -> NULL detect");
		return NULL;
	}

	value = convertspacetolf(value);

	debug(1000, "out");
	return value;
}

char* getac3choices()
{
	debug(1000, "in");
	char *ac3choicesdev = NULL;
	char *value = NULL;

	ac3choicesdev = getconfig("ac3choicesdev", NULL);

	if(ac3choicesdev == NULL)
	{
		debug(1000, "out -> NULL detect");
		return NULL;
	}

	value = readsys(ac3choicesdev, 1);
	if(value == NULL)
	{
		debug(1000, "out -> NULL detect");
		return NULL;
	}

	value = convertspacetolf(value);

	debug(1000, "out");
	return value;
}

int setciclock(int slotnr, char* value)
{
	debug(1000, "in");
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

	debug(1000, "out");
	return 0;
}

int setciinput(int slotnr, char* value)
{
	debug(1000, "in");
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

	debug(1000, "out");
	return 0;
}

int setcisource(int tunernr, char* value)
{
	debug(1000, "in");
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

	debug(1000, "out");
	return 0;
}

int setinput(char* value)
{
	debug(1000, "in");
	char* inputdev = NULL;
	int ret = 0;

	inputdev = getconfig("inputdev", NULL);

	if(inputdev != NULL && value != NULL)
	{
		debug(100, "set %s to %s", inputdev, value);
		ret = writesys(inputdev, value, 0);
		return ret;
	}

	debug(1000, "out");
	return 0;
}

char* getinput()
{
	debug(1000, "in");
	char *inputdev = NULL;
	char *value = NULL;

	inputdev = getconfig("inputdev", NULL);

	if(inputdev == NULL)
	{
		debug(1000, "out -> NULL detect");
		return NULL;
	}

	value = readsys(inputdev, 1);
	if(value == NULL)
	{
		debug(1000, "out -> NULL detect");
		return NULL;
	}

	debug(1000, "out");
	return value;
}

int setac3(char* value)
{
	debug(1000, "in");
	char* ac3dev = NULL;
	int ret = 0;

	ac3dev = getconfig("ac3dev", NULL);

	if(ac3dev != NULL && value != NULL)
	{
		debug(100, "set %s to %s", ac3dev, value);
		ret = writesys(ac3dev, value, 0);
		if(ret == 0) addconfig("av_ac3mode", value);
		return ret;
	}

	debug(1000, "out");
	return 0;
}

char* getac3()
{
	debug(1000, "in");
	char *ac3dev = NULL;
	char *value = NULL;

	ac3dev = getconfig("ac3dev", NULL);

	if(ac3dev == NULL)
	{
		debug(1000, "out -> NULL detect");
		return NULL;
	}

	value = readsys(ac3dev, 1);
	if(value == NULL)
	{
		debug(1000, "out -> NULL detect");
		return NULL;
	}

	debug(1000, "out");
	return value;
}

char* getpolicychoices()
{
	debug(1000, "in");
	char *policychoicesdev = NULL;
	char *value = NULL;

	policychoicesdev = getconfig("policychoicesdev", NULL);

	if(policychoicesdev == NULL)
	{
		debug(1000, "out -> NULL detect");
		return NULL;
	}

	value = readsys(policychoicesdev, 1);
	if(value == NULL)
	{
		debug(1000, "out -> NULL detect");
		return NULL;
	}

	value = convertspacetolf(value);

	debug(1000, "out");
	return value;
}

char* getpolicy()
{
	debug(1000, "in");
	char *policydev = NULL;
	char *value = NULL;

	policydev = getconfig("policydev", NULL);

	if(policydev == NULL)
	{
		debug(1000, "out -> NULL detect");
		return NULL;
	}

	value = readsys(policydev, 1);
	if(value == NULL)
	{
		debug(1000, "out -> NULL detect");
		return NULL;
	}

	debug(1000, "out");
	return value;
}

int setpolicy(char* value)
{
	debug(1000, "in");
	char* policydev;
	int ret = 0;

	policydev = getconfig("policydev", NULL);

	if(policydev != NULL && value != NULL)
	{
		debug(100, "set %s to %s", policydev, value);
		ret = writesys(policydev, value, 0);
		if(ret == 0) addconfig("av_policy", value);
		return ret;
	}

	debug(1000, "out");
	return 0;
}

char* getaspectchoices()
{
	debug(1000, "in");
	char *aspectchoicesdev = NULL;
	char *value = NULL;

	aspectchoicesdev = getconfig("aspectchoicesdev", NULL);

	if(aspectchoicesdev == NULL)
	{
		debug(1000, "out -> NULL detect");
		return NULL;
	}

	value = readsys(aspectchoicesdev, 1);
	if(value == NULL)
	{
		debug(1000, "out -> NULL detect");
		return NULL;
	}

	value = convertspacetolf(value);

	debug(1000, "out");
	return value;
}

char* getaspect()
{
	debug(1000, "in");
	char *aspectdev = NULL;
	char *value = NULL;

	aspectdev = getconfig("aspectdev", NULL);

	if(aspectdev == NULL)
	{
		debug(1000, "out -> NULL detect");
		return NULL;
	}

	value = readsys(aspectdev, 1);
	if(value == NULL)
	{
		debug(1000, "out -> NULL detect");
		return NULL;
	}

	debug(1000, "out");
	return value;
}

int setaspect(char* value)
{
	debug(1000, "in");
	char* aspectdev;
	int ret = 0;

	aspectdev = getconfig("aspectdev", NULL);

	if(aspectdev != NULL && value != NULL)
	{
		debug(100, "set %s to %s", aspectdev, value);
		ret = writesys(aspectdev, value, 0);
		if(ret == 0) addconfig("av_aspect", value);
		return ret;
	}

	debug(1000, "out");
	return 0;
}

char* getvideomodechoices()
{
	debug(1000, "in");
	char *videomodechoicesdev = NULL;
	char *value = NULL;

	videomodechoicesdev = getconfig("videomodechoicesdev", NULL);

	if(videomodechoicesdev == NULL)
	{
		debug(1000, "out -> NULL detect");
		return NULL;
	}

	value = readsys(videomodechoicesdev, 1);
	if(value == NULL)
	{
		debug(1000, "out -> NULL detect");
		return NULL;
	}

	value = convertspacetolf(value);

	debug(1000, "out");
	return value;
}

char* getmode3dchoices()
{
	debug(1000, "in");
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
		debug(1000, "out -> NULL detect");
		return NULL;
	}

	value = convertspacetolf(value);

	debug(1000, "out");
	return value;
}

char* getmode3d()
{
	debug(1000, "in");
	char *mode3ddev = NULL;
	char *value = NULL;

	mode3ddev = getconfig("mode3ddev", NULL);

	if(mode3ddev == NULL)
	{
		debug(1000, "out -> NULL detect");
		return NULL;
	}

	value = readsys(mode3ddev, 1);
	if(value == NULL)
	{
		debug(1000, "out -> NULL detect");
		return NULL;
	}

	debug(1000, "out");
	return value;
}

int setmode3d(char* value)
{
	debug(1000, "in");
	char* mode3ddev;
	int ret = 0;

	mode3ddev = getconfig("mode3ddev", NULL);

	if(mode3ddev != NULL && value != NULL)
	{
		debug(100, "set %s to %s", mode3ddev, value);
		ret = writesys(mode3ddev, value, 0);
		if(ret == 0) addconfig("av_mode3d", value);
		return ret;
	}

	debug(1000, "out");
	return 0;
}

char* getvideomode()
{
	debug(1000, "in");
	char *videomodedev = NULL;
	char *value = NULL;

	videomodedev = getconfig("videomodedev", NULL);

	if(videomodedev == NULL)
	{
		debug(1000, "out -> NULL detect");
		return NULL;
	}

	value = readsys(videomodedev, 1);
	if(value == NULL)
	{
		debug(1000, "out -> NULL detect");
		return NULL;
	}

	debug(1000, "out");
	return value;
}

int setvideomode(char* value)
{
	debug(1000, "in");
	char* videomodedev;
	int ret = 0;

	videomodedev = getconfig("videomodedev", NULL);

	if(videomodedev != NULL && value != NULL)
	{
		debug(100, "set %s to %s", videomodedev, value);
		ret = writesys(videomodedev, value, 0);
		if(ret == 0) addconfig("av_videomode", value);
		return ret;
	}

	debug(1000, "out");
	return 0;
}

int setcolorformat(char* value)
{
	debug(1000, "in");
	char* colorformatdev;
	int ret = 0;

	colorformatdev = getconfig("colorformatdev", NULL);

	if(colorformatdev != NULL && value != NULL)
	{
		debug(100, "set %s to %s", colorformatdev, value);
		ret = writesys(colorformatdev, value, 0);
		if(ret == 0) addconfig("av_colorformat", value);
		return ret;
	}

	debug(1000, "out");
	return 0;
}

char* getcolorformat(int line)
{
	debug(1000, "in");
	char *colorformatdev = NULL;
	char *value = NULL;

	colorformatdev = getconfig("colorformatdev", NULL);

	if(colorformatdev == NULL)
	{
		debug(1000, "out -> NULL detect");
		return NULL;
	}

	value = readsys(colorformatdev, line);
	if(value == NULL)
	{
		debug(1000, "out -> NULL detect");
		return NULL;
	}

	debug(1000, "out");
	return value;
}

int setaudiosource(char* value)
{
	debug(1000, "in");
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

	debug(1000, "out");
	return 0;
}

char* getaudiosource()
{
	debug(1000, "in");
	char *audiosourcedev = NULL;
	char *value = NULL;

	audiosourcedev = getconfig("audiosourcedev", NULL);

	if(audiosourcedev == NULL)
	{
		debug(1000, "out -> NULL detect");
		return NULL;
	}

	value = readsys(audiosourcedev, 1);
	if(value == NULL)
	{
		debug(1000, "out -> NULL detect");
		return NULL;
	}

	debug(1000, "out");
	return value;
}

int setprogress(value)
{
	debug(1000, "in");
	char *progressdev;

	progressdev = getconfig("progressdev", NULL);

	if(progressdev != NULL)
	{
		debug(100, "set %s to %d",progressdev, value);
		return writesysint(progressdev, value, 0);
	}

	debug(1000, "out");
	return 0;
}

int setmute(int value)
{
	debug(1000, "in");
	char* mutedev;

	mutedev = getconfig("mutedev", NULL);

	if(mutedev != NULL)
	{
		debug(100, "set %s to %d", mutedev, value);
		return writesysint(mutedev, value, 0);
	}

	debug(1000, "out");
	return 0;
}

int setvol(int value)
{
	debug(1000, "in");
	char* voldev;
	int ret = 0, tmpvol = value;

	voldev = getconfig("voldev", NULL);

	if(voldev != NULL)
	{
		value = 63 - value * 63 / 100;
		debug(100, "set %s to %d", voldev, value);
		ret = writesysint(voldev, value, 0);
		if(ret == 0) addconfigint("vol", tmpvol);
		return ret;
	}

	debug(1000, "out");
	return 0;
}

int getvol()
{
	debug(1000, "in");
	char *voldev = NULL;
	char *value = NULL;
	int tmpvol = 0;

	voldev = getconfig("voldev", NULL);

	if(voldev == NULL)
	{
		debug(1000, "out -> NULL detect");
		return 0;
	}

	value = readsys(voldev, 1);
	if(value == NULL)
	{
		debug(1000, "out -> NULL detect");
		return 0;
	}

	tmpvol = atoi(value);
	free(value);
	tmpvol = 100 - tmpvol * 100 / 63;

	debug(1000, "out");
	return tmpvol;
}

void setdebuglevel()
{
	debug(1000, "in");

	debug_level = getconfigint("debuglevel", NULL);
	debug(0, "set debug level to %d", debug_level);

	debug(1000, "out");
}

char* getxmlentry(char *line, char *searchstr)
{
	//debug(1000, "in");
	char *buf = NULL, *buf1 = NULL, *buf2 = NULL;

	buf = strstr(line, searchstr);
	if(buf == NULL)
	{
		//debug(1000, "out -> searchstr=\"%s\" not found in line=\"%s\"", searchstr, line);
		return NULL;
	}
	buf = buf + strlen(searchstr);
	if(buf[0] == '"')
	{
		buf = buf + 1;
		buf1 = ostrcat(buf, "", 0, 0);
		if(buf1 == NULL)
		{
			err("ostrcat failed");
			return NULL;
		}
		buf2 = strchr(buf1, '"');
		if(buf2 == NULL)
		{
			err("strchr returns NULL");
			free(buf1);
			return NULL;
		}
		buf2[0] = '\0';
	}
	else
	{
		buf1 = ostrcat(buf, "", 0, 0);
		if(buf1 == NULL)
		{
			err("ostrcat failed");
			return NULL;
		}
		buf2 = strchr(buf1, ' ');
		if(buf2 == NULL)
		{
			buf2 = strstr(buf1, "/>");
			if(buf2 == NULL)
			{
				buf2 = strchr(buf1, '>');
				if(buf2 == NULL)
				{
					err("strchr returns NULL");
					free(buf1);
					return NULL;
				}
			}
		}
		buf2[0] = '\0';
	}
	//debug(1000, "out");
	return buf1;
}

char* readfiletomem(const char* filename, int flag)
{
	debug(1000, "in");
	FILE *fd = NULL;
	char *fileline = NULL, *buf = NULL, *buf1 = NULL;
	int buf1size = 0, buf1oldsize = 0;

	fileline = malloc(MINMALLOC);
	if(fileline == NULL)
	{
		err("no memory");
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
		buf = fileline;

		if(flag == 1)
			if(buf[0] == '#' || buf[0] == '\n')
				continue;

		buf1oldsize = buf1size;
		buf1size += strlen(buf);
		buf1 = realloc(buf1, buf1size + 1);
		if(buf1 == NULL)
		{
			err("no memory");
			free(fileline);
			fclose(fd);
			return NULL;
		}

		sprintf(buf1 + buf1oldsize, "%s", buf);
	}

	free(fileline);
	fclose(fd);
	debug(1000, "out");
	return buf1;
}

char* command(char* input)
{
	debug(1000, "in");
	char* tmpstr = NULL, *fileline = NULL;
	FILE *iopipe;

	fileline = malloc(MINMALLOC);
	if(fileline == NULL)
	{
		err("no memory");
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
			tmpstr = ostrcat(tmpstr, fileline, 1, 0);
	}

	free(fileline);
	pclose(iopipe);
	debug(1000, "out");
	return tmpstr;
}

char* string_tolower(char *str)
{
	debug(1000, "in");
	int i;
	
	if(str == NULL) return NULL;

	for( i = 0; i < strlen(str); i++)
		str[i] = tolower(str[i]);

	debug(1000, "out");
	return str;
}

char* string_toupper(char *str)
{
	debug(1000, "in");
	int i;
	
	if(str == NULL) return NULL;

	for( i = 0; i < strlen(str); i++)
		str[i] = toupper(str[i]);

	debug(1000, "out");
	return str;
}

char* stringreplacecharonce(char *str, char c1, char c2)
{
	debug(1000, "in");
	int i;

	if(str == NULL) return NULL;

	for( i = 0; i < strlen(str); i++)
	{
		if(str[i] == c1)
		{
			str[i] = c2;
			break;
		}
	}

	debug(1000, "out");
	return str;
}

char* stringreplacechar(char *str, char c1, char c2)
{
	debug(1000, "in");
	int i;

	if(str == NULL) return NULL;

	for( i = 0; i < strlen(str); i++)
		if(str[i] == c1) str[i] = c2;

	debug(1000, "out");
	return str;
}

char* string_removechar(char *str)
{
	debug(1000, "in");
	int i;

	if(str == NULL) return NULL;

	for( i = 0; i < strlen(str); i++)
	{
		if(str[i] == '.') str[i] = ' ';
		if(str[i] == '-') str[i] = ' ';
		if(str[i] == '_') str[i] = ' ';
		if(str[i] == '/') str[i] = ' ';
	}
	debug(1000, "out");
	return str;
}

char* string_withchars2return(char *str)
{
	debug(1000, "in");
	int i;

	for( i = 0; i < strlen(str); i++)
	{
		if(str[i] == ' ') str[i] = '\n';
	}

	debug(1000, "out");
	return str;
}

char* string_remove_whitechars(char *text)
{
	debug(1000, "in");
	char *p1 = text, *p2 = text;

	while(*p1 != '\0')
	{
		if(*p1 == ' ')
			++p1;
		else
			*p2++ = *p1++;
	}
	*p2 = '\0';

	debug(1000, "out");
	return text;
}

char* strstrip(char *text)
{
	debug(1000, "in");
	char* tmpstr = text;

	if(text == NULL) return NULL;
	int len = strlen(text);

	while(isspace(tmpstr[len - 1])) tmpstr[--len] = '\0';
	while(*tmpstr && isspace(*tmpstr)) ++tmpstr, --len;

	memmove(text, tmpstr, len + 1);

	debug(1000, "out");
	return text;
}

char* string_strip_whitechars(char *text)
{
	debug(1000, "in");
	int i;

	if(text == NULL)
	{
		debug(1000, "out -> NULL detect");
		return NULL;
	}

	text = string_removechar(text);

	for(i = 0; text[i] != '\0'; i++)
	{
		if(text[i] == ' ' && text[i + 1] == ' ')
		{
			int i2 = i + 1;
			for(; text[i2] != '\0'; i2++)
			{
				text[i] = text[i2];
				i++;
			}
			text[i2 - 1] = '\0';
			i = -1;
		}
	}

	debug(1000, "out");
	return text;
}

char* string_replace_remove_last_chars(char *search, char *replace, char *string, int free1)
{
	debug(1000, "in");
	char* searchpos = NULL;
	char* tmpstr = NULL;

	searchpos = strstr(string, search);

	if(searchpos == NULL || string == NULL)
		return string;

	tmpstr = strndup(string, searchpos - string);
	if(replace != NULL)
		tmpstr = ostrcat(tmpstr, replace, 1, 0);

	if(free1 == 1) free(string);

	debug(1000, "out");
	return tmpstr;
}

char* string_replace(char *search, char *replace, char *string, int free1)
{
	debug(1000, "in");
	char* searchpos = NULL;
	char* tmpstr = NULL;

	searchpos = strstr(string, search);

	if(searchpos == NULL || string == NULL)
		return string;

	tmpstr = strndup(string, searchpos - string);
	if(replace == NULL)
		tmpstr = ostrcat(tmpstr, "", 1, 0);
	else
		tmpstr = ostrcat(tmpstr, replace, 1, 0);
	tmpstr = ostrcat(tmpstr, string + (searchpos - string) + strlen(search), 1, 0);

	if(free1 == 1) free(string);

	debug(1000, "out");
	return tmpstr;
}

int string_find(char* str, char* filename)
{
	debug(1000, "in");
	int i, len_str, len_filename;
	
	if(str == NULL || filename == NULL) return 0;

	len_str = strlen(str);
	len_filename = strlen(filename);

	for(i = 0; (i + len_str) < len_filename; i++)
		if(strncmp(filename + i, str, len_str) == 0)
			return 1;

	debug(1000, "out");
	return 0;
}

int file_exist(char* filename)
{
	debug(1000, "in");
	if (access(filename, F_OK) == 0)
		return 1;
	else
		return 0;
}

char* string_newline(char* str)
{
	debug(1000, "in");
	if(str == NULL) return NULL;

	int size = strlen(str);

	if(str[size - 1] == '\n')
		str[size - 1] = '\0';
	debug(1000, "out");
	return str;
}

char* string_quote(char* str)
{
	debug(1000, "in");
	char* tmpstr = NULL;

	tmpstr = ostrcat("\"", str, 0, 0);
	tmpstr = ostrcat(tmpstr, "\"", 1, 0);
	debug(1000, "out");
	return tmpstr;
}

struct splitstr* strsplit(char *str, char *tok, int* count)
{
	debug(1000, "in");
	char *tmpstr = NULL;
	struct splitstr *tmparray = NULL;
	*count = 0;

	if(tok == NULL)
		return NULL;

	tmpstr = strtok(str, tok);
	while(tmpstr != NULL)
	{
		*count = *count + 1;
		tmparray = (struct splitstr*)realloc(tmparray, sizeof(struct splitstr*) * (*count));
		if(tmparray == NULL)
			return NULL;
		(&tmparray[(*count) - 1])->part = tmpstr;
		tmpstr = strtok(NULL, tok);
	}
	debug(1000, "out");
	return tmparray;
}

char* string_shortname(char *tmpfilename, int mode)
{
	debug(50, "in %s",tmpfilename);

//	char replacelist[] = "avi mkv x264 se disc0 disc1 disc2 disc3 disc4 0disc 1disc 2disc 3disc 4disc season0 season1 season2 season3 season4 season5 season6 season7 season8 season9 hdtv 720p 1080i 1080p uncut cd0 cd1 cd2 cd3 cd4 cd5 cd6 cd7 cd8 cd9 dvd0 dvd1 dvd2 dvd3 dvd4 ac3d ac3 bdrip bluray cam camrip complete custom cut dc directors dl doku dts dvdr dvdrip dvdscr dvdscreener extended french finnish german hd hddvd hddvdrip hdtv int internal int ld limited multi multisubs nordic ntsc pal pl r1 r5 recut remastered repack rip screener se see special.edition sse stv subbed swedish staffel tc telecine telesync ts unrated ws xxx italian";
	char* str = NULL;

	if (mode==1){
		char* replacelist = "avi mkv x264 se uncut ac3d ac3hd ac3 bdrip bluray cam camrip complete custom cut dc directors dl doku dts dvdr dvdrip dvdscr dvdscreener extended french finnish german hd hddvd hddvdrip hdtv int internal int ld limited multi multisubs nordic ntsc pal pl r1 r5 recut remastered repack rip screener se see special.edition sse stv subbed swedish staffel tc telecine telesync ts unrated ws xxx italian";
		str = ostrcat(str, replacelist, 0, 0);
	} else {
		char* replacelist = "disc0 disc1 disc2 disc3 disc4 0disc 1disc 2disc 3disc 4disc season0 season1 season2 season3 season4 season5 season6 season7 season8 season9 hdtv 720p 1080i 1080p cd0 cd1 cd2 cd3 cd4 cd5 cd6 cd7 cd8 cd9 dvd0 dvd1 dvd2 dvd3 dvd4";
		str = ostrcat(str, replacelist, 0, 0);
	}

	char* replace = NULL;
	struct splitstr* ret1 = NULL;
	int count = 0;
	int i = 0;
	ret1 = strsplit(str, " ", &count);
	int max = count - 1;

	for( i = 0; i < max; i++){
		struct splitstr* ret2 = NULL;
		int count2 = 0;
		int j = 0;
		char *tmpstr = NULL;
		tmpstr = ostrcat(tmpstr, tmpfilename, 1, 0);
		ret2 = strsplit(tmpstr, " ,.-_", &count2);

		for( j = 0; j < count2; j++){
			if(j > 0){
				if(ostrcmp((&ret1[i])->part, (&ret2[j])->part) == 0){
					if (mode==1){
						tmpfilename = string_replace((&ret2[j])->part, replace, tmpfilename, 1);
						continue;
					} else {
//						tmpfilename = string_replace_remove_last_chars((&ret2[j])->part, replace, tmpfilename, 1);
//						break;
						tmpfilename = string_replace((&ret2[j])->part, replace, tmpfilename, 1);
						continue;
					}
				}
			}
		}
		free(ret2); ret2 = NULL;
	}

	free(ret1); ret1 = NULL;
	free(replace); replace = NULL;
	free(str); str = NULL;

	debug(50, "out %s", tmpfilename);
	return tmpfilename;
}

char* get_ipk_section()
{
	debug(60, "in");
	return command("ipkg list | awk '{ print $1 }' | cut -d '-' -f3 | sed 's/Successfully//' | sort -u");
}

char* get_ipk_list(char* section)
{
	debug(60, "in %s",section);
	char* cmd = NULL, *tmpstr = NULL;
	
	if(section == NULL) return NULL;
	
	cmd = ostrcat(cmd, "ipkg list *-", 1, 0);
	cmd = ostrcat(cmd, section, 1, 0);
	cmd = ostrcat(cmd, "-* | sed 's/Successfully terminated.//'", 1, 0);

	tmpstr = command(cmd);

	debug(60, "out %s",cmd);
	free(cmd); cmd = NULL;
	return tmpstr;
}

/* disable cant handling 2 feeds....
char* get_ipk_install(char* ipk)
{
	debug(60, "in %s",ipk);
	char* tmpstr1 = NULL;
	char* tmpstr2 = NULL;
	char* tmpstr3 = NULL;
	
	if(ipk == NULL) return NULL;
	
	struct splitstr* ret1 = NULL;
	int count1 = 0;
	tmpstr1 = ostrcat("", ipk, 0, 0);
	ret1 = strsplit(tmpstr1, " ", &count1);
	tmpstr2 = ostrcat("", "", 0, 0);
	tmpstr3 = ostrcat(tmpstr3 , (&ret1[1])->part, 1, 0);
	debug(60, "tmpstr3: %s", tmpstr3);

	strcpy(tmpstr2, tmpstr3 + 2);
	debug(60, "tmpstr2: %s", tmpstr2);

	char* ipkname = NULL, *tmpstr = NULL;
	ipkname = ostrcat(ipkname, "/titan-plugin-", 1, 0);
	ipkname = ostrcat(ipkname, (&ret1[0])->part, 1, 0);
	ipkname = ostrcat(ipkname, "_", 1, 0);
	ipkname = ostrcat(ipkname, tmpstr2, 1, 0);
	ipkname = ostrcat(ipkname, "_sh4.ipk", 1, 0);

	char* ip = NULL;
	char* cmd = NULL;
	char* path = NULL;

	cmd = ostrcat(cmd, "cat /var/etc/ipkg/official-feed.conf | cut -d '/' -f4", 1, 0);
	debug(60, "cmd: %s", cmd);
	cmd = strstrip(string_newline(command(cmd)));
	debug(60, "cmd: %s", cmd);

	ip = ostrcat("", cmd, 0, 0);
	debug(60, "ip: %s", ip);

	cmd = ostrcat("", "cat /var/etc/ipkg/official-feed.conf | cut -d ' ' -f3 | sed 's!http://", 0, 0);
	cmd = ostrcat(cmd, ip, 1, 0);
	cmd = ostrcat(cmd, "/!!'", 1, 0);
	debug(60, "cmd: %s", cmd);
	cmd = strstrip(string_newline(command(cmd)));
	debug(60, "cmd: %s", cmd);

	path = ostrcat(path, cmd, 1, 0);
	path = ostrcat(path, ipkname, 1, 0);
	debug(60, "path: %s", path);

	screendownload("Download", ip, path, 80, "/tmp/tmp.ipk", 0);

	tmpstr = command("ipkg install /tmp/tmp.ipk");

	debug(60, "out %s",cmd);
	free(ipkname); ipkname = NULL;
	free(tmpstr1); tmpstr1 = NULL;
	free(tmpstr2); tmpstr2 = NULL;
	free(tmpstr3); tmpstr3 = NULL;
	return tmpstr;
}
*/
char* get_ipk_install(char* ipk)
{
	debug(60, "in %s",ipk);

	char* cmd = NULL, *tmpstr = NULL;
	cmd = ostrcat(cmd, "ipkg install titan-plugin-", 1, 0);
	cmd = ostrcat(cmd, ipk, 1, 0);

	tmpstr = command(cmd);

	debug(60, "out %s",cmd);
	free(cmd); cmd = NULL;
	return tmpstr;
}

char* get_ipk_listinstall()
{
	debug(60, "in");
	char* cmd = NULL, *tmpstr = NULL;
	
	cmd = ostrcat(cmd, "ipkg list_installed | awk '{ print $1 }' | sed 's/Successfully//' | sed 's/titan-plugin-//'", 1, 0);

	tmpstr = command(cmd);

	debug(60, "out %s",cmd);
	free(cmd); cmd = NULL;
	return tmpstr;
}

char* get_ipk_remove(char* ipk)
{
	debug(60, "in %s",ipk);
	char* cmd = NULL, *tmpstr = NULL;
	
	if(ipk == NULL) return NULL;
	
	cmd = ostrcat(cmd, "ipkg remove titan-plugin-", 1, 0);
	cmd = ostrcat(cmd, ipk, 1, 0);

	tmpstr = command(cmd);

	debug(60, "out");
	free(cmd); cmd = NULL;
	return tmpstr;
}

char* get_ipk_update()
{
	debug(60, "in");
	char* tmpstr1 = NULL;

	if(!file_exist("/tmp/Packages.preview.tar.gz"))
	{
		char* ip = NULL;
		char* cmd = NULL;
		char* path = NULL;

		cmd = ostrcat(cmd, "cat /var/etc/ipkg/official-feed.conf | cut -d '/' -f4", 1, 0);
		debug(60, "cmd: %s", cmd);
		cmd = strstrip(string_newline(command(cmd)));
		debug(60, "cmd: %s", cmd);

		struct splitstr* ret1 = NULL;
		int count1 = 0;
		tmpstr1 = ostrcat("", cmd, 0, 0);
		ret1 = strsplit(tmpstr1, "\n", &count1);
		int max = count1;
		int i = 0;
		for( i = 0; i < max; i++){
			system("rm -rf /tmp/Packages.preview.tar.gz");
			ip = ostrcat("", (&ret1[i])->part, 0, 0);
			debug(60, "ip: %s", ip);
			cmd = ostrcat("", "cat /var/etc/ipkg/official-feed.conf | grep ", 0, 0);
			cmd = ostrcat(cmd, ip, 1, 0);
			cmd = ostrcat(cmd, " | cut -d ' ' -f3 | sed 's!http://", 1, 0);
			cmd = ostrcat(cmd, ip, 1, 0);
			cmd = ostrcat(cmd, "/!!'", 1, 0);
			debug(60, "cmd: %s", cmd);
			cmd = strstrip(string_newline(command(cmd)));
			debug(60, "cmd: %s", cmd);

			path = ostrcat("", cmd, 0, 0);
			path = ostrcat(path, "/Packages.preview.tar.gz", 1, 0);
			debug(60, "path: %s", path);

			screendownload("Download", ip, path, 80, "/tmp/Packages.preview.tar.gz", 0);

			system("tar -zxvf /tmp/Packages.preview.tar.gz -C /tmp");
			
		}

		free(cmd), cmd = NULL;
		free(ip), ip = NULL;
		free(path), path = NULL;
	}
	debug(60, "out");
	return command("ipkg update");
}

char* get_ipk_info(char* section)
{
	debug(60, "in %s",section);
	char* cmd = NULL, *tmpstr = NULL;
	
	if(section == NULL) return NULL;
	
	cmd = ostrcat(cmd, "ipkg list *-", 1, 0);
	cmd = ostrcat(cmd, section, 1, 0);
	cmd = ostrcat(cmd, " | cut -d'-' -f6 | sed 's/Successfully terminated.//'", 1, 0);

	tmpstr = command(cmd);

	debug(60, "out %s",cmd);
	free(cmd); cmd = NULL;
	return tmpstr;
}

char* get_uuid(char* device)
{
	debug(60, "in %s", device);

	char* cmd = NULL, *tmpstr = NULL;
	cmd = ostrcat(cmd, "/bin/blkid -w /dev/null -c /dev/null -s UUID /dev/", 1, 0);
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
	
	cmd = ostrcat(cmd, "/bin/blkid -w /dev/null -c /dev/null -s LABEL /dev/", 1, 0);
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
	
	cmd = ostrcat(cmd, "/bin/blkid -w /dev/null -c /dev/null -s TYPE /dev/", 1, 0);
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

char* fixip(char* ipinput, int flag)
{
	debug(60, "in %s", ipinput);
	char* ipout = NULL;
	unsigned char ip[4];
	
	if(ipinput == NULL)	return NULL;
	sscanf(ipinput, "%hhu.%hhu.%hhu.%hhu", &ip[0], &ip[1], &ip[2], &ip[3]);

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
	
	if(speed < 0) 
	{
		speedWert = getconfig("fanspeed", NULL);
		if(speedWert == NULL) 
			speedSet = ostrcat(speedSet, "170", 1, 0);
		else
			speedSet = ostrcat(speedSet, speedWert, 1, 0);
	}
	else
	{
		if(speed == 0)
			speedSet = ostrcat(speedSet, "115", 1, 0);
		else if(speed == 25)
			speedSet = ostrcat(speedSet, "130", 1, 0);
		else if(speed == 50)
			speedSet = ostrcat(speedSet, "145", 1, 0);	
		else if(speed == 75)
			speedSet = ostrcat(speedSet, "155", 1, 0);
		else
			speedSet = ostrcat(speedSet, "170", 1, 0);
	}
	
	writesys("/proc/stb/fan/fan_ctrl", speedSet, 1);
	
	if(aktion == 1)
		addconfig("fanspeed", speedSet);
	
	free(speedSet); speedSet=NULL;
}	

void setaktres()
{
	int m_width;
	char* res = NULL;
	char* res_akt = NULL;
	char* res_sd = NULL;
	int count=1;
	int sec = 0;
	
	if(status.restimer == NULL) return;
	sec = (int)status.restimer->param1;

	if(sec > 0)
	{	
		while(status.restimer->aktion == START && count <= sec)
		{
			sleep(1);
			if(status.restimer->aktion != START)
			{
				status.restimer = NULL;
				return;
			}
			count++;
		}
	}

	if(videoreadqwidth(status.aktservice->videodev) == 0)
	{
		m_width = status.videosize.w;
		if (m_width == 720) {
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
		if ( m_width > 0)
		{	
			res_akt = getvideomode();
			if (ostrcmp(res_akt, res) != 0) 
			{
				setvideomode(res);
	  		changefbresolution(res);
				sleep(1);
				screenautores(res, 5, 0);
			}		
		}
	}
	else
		textbox(_("Message"), _("ERROR cant read res"), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, NULL, 0, 600, 200, 5, 0);

	free(res);
	res = NULL;
	status.restimer = NULL;
	return;
}				

#endif
