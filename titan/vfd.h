#ifndef VFD_H_
#define VFD_H_

// Internal definitions (no one outside this file will need them)
#define VFDBRIGHTNESS 0xc0425a03
#define VFDDISPLAYWRITEONOFF 0xc0425a05
#define	VFDDISPLAYCHARS 0xc0425a00
#define	VFDICONDISPLAYONOFF 0xc0425a0a

struct stimerthread* VFD_Recordthread = NULL;
struct channel* oldvfdchannel = NULL;
int oldvfdchannelnr = -1;

// Struct to handle ioctl calls (no one outside this file will need this)
struct vfdioctl
{
	unsigned char start;
	unsigned char data[64];
	unsigned char length;
};

// Struct to remember the current state of an icon (no one outside this file will need this)
struct vfdiconstate
{
	vfdicons icon;
	char state;  /* 0=Off, 1=ON */
	struct vfdiconstate* next;
};

// Start pointer of linked icon state list
struct vfdiconstate* vfdiconstates = 0;

// Currently displayed text in VFD:
char vfdtext[64];
int durchlauf = 0;

// Set the brightness of the VFD
int setvfdbrightness(int value)
{
	char *vfdbrightnessdev;
	if(checkchipset("3798MV200") == 1)
	{
		vfdbrightnessdev = getconfig("vfdbrightnessdev", NULL);

		if(vfdbrightnessdev != NULL && value != 0)
		{
			debug(10, "set %s to %d", vfdbrightnessdev, value * 25);
			return writesysint(vfdbrightnessdev, value * 25, 0);
		}
	
		return 0;
	}

	if(checkbox("UFS913") == 1)
	{
		if(value == 0)
			value = 1;
		if(value == 7)
			value = 6;
	}

#ifdef MIPSEL
	FILE *f=fopen("/proc/stb/lcd/oled_brightness", "w");
	if (!f)
	{
		*f=fopen("/proc/stb/fp/oled_brightness", "w");
		if (!f)
			return 1;
	}
	int brightness = value * 36;
	if(checkbox("VUZERO4K") == 1)
		brightness = value * 15;
	fprintf(f, "%d", brightness);
	fclose(f);
	return 0;
#endif

	struct vfdioctl data;
	int fd;
	char *vfddev;

	if(status.updatevfd == STOP) return 0;

	vfddev = getconfig("vfddev", NULL);

	if(vfddev != NULL)
	{
		memset(&data, 0, sizeof(struct vfdioctl));

		data.start = value & 0x07;
		data.length = 0;

		m_lock(&status.vfdmutex, 3);
		fd = open(vfddev, O_WRONLY);
		ioctl(fd, VFDBRIGHTNESS, &data);
		close(fd);
		m_unlock(&status.vfdmutex, 3);
	}

	return 0;
}

int setvfdlight(int on)
{
	char *vfddev;
	struct vfdioctl data;
	int fd;

	if(status.updatevfd == STOP) return 0;

	vfddev = getconfig("vfddev", NULL);

	if(vfddev != NULL)
	{
		memset(&data, 0, sizeof(struct vfdioctl));

		if(on)
			data.start = 0x01;
		else
			data.start = 0x00;
		data.length = 0;

		m_lock(&status.vfdmutex, 3);
		fd = open(vfddev, O_WRONLY);
		ioctl(fd, VFDDISPLAYWRITEONOFF, &data);
		close(fd);
		m_unlock(&status.vfdmutex, 3);
	}

	return 0;
}

// Display center text on VFD
void writecentervfd(char* value)
{
	int len = 0, ws = 0, max = 0;
	char* out = NULL;

	if(value == NULL) return;

	max = getconfigint("vfdlen", NULL);
	if(max == 0) max = 16;

	out = (char *) malloc(max);
	if(out == NULL)
	{
		err("no mem");
		return;
	}
	memset(out, ' ', max);

	len = strlen(value);

	if(len < max) ws = (max - len) / 2;

	memcpy(out + ws, value, max - ws);
	writevfd(out);
	free(out);
}

// Display a scroll text on VFD
void writescrollvfd(char* value)
{
	int i, len = 0, max = 0;
	char* out = NULL;

	if(value == NULL) return;

	max = getconfigint("vfdlen", NULL);
	if(max == 0) max = 16;
	
	out = (char *) malloc(max);
	if(out == NULL)
	{
		err("no mem");
		return;
	}

	len = strlen(value);

	// scroll text till end
	for(i = 0; i <= (len - max); i++)
	{
		memset(out, ' ', max);
		memcpy(out, value + i, max);
		writevfd(out);
		usleep(200000);
	}
	// scroll text with whitespaces from right
	for(i = 1; i < max; i++)
	{
		memset(out, ' ', max);
		memcpy(out, value + len + i - max, max - i);
		writevfd(out);
		usleep(200000);
	}

	memcpy(out, value, max); // display first 16 chars after scrolling
	writevfd(out);
	free(out);
	return;
}

// does not work on ipbox
// Display a text on VFD
int writevfdioctl(char *value)
{
	char *vfddev;
	int ret=0;

	if(status.updatevfd == STOP) return 0;

	vfddev = getconfig("vfddev", NULL);

	if(vfddev != NULL && value != NULL && strncmp(value, vfdtext, 63))
	{
		int fd;
		struct vfdioctl data;

		debug(100, "set %s to %s", vfddev, value);

		data.length = strlen (value);
		if(data.length > 63) data.length = 63;

		memset(data.data, ' ', 63);
		memcpy(data.data, value, data.length);
		data.start = 0;

		m_lock(&status.vfdmutex, 3);
		fd = open(vfddev, O_WRONLY);
		ret = ioctl(fd, VFDDISPLAYCHARS, &data);
		close(fd);
		m_unlock(&status.vfdmutex, 3);

		strncpy(vfdtext, value, 63);
	}

	return ret;
}

int writevfdmenu(char *value)
{
	int ret = 0;

	if(checkbox("ATEMIO530") == 0 && checkbox("ATEMIO520") == 0 && checkbox("IPBOX91") == 0 && checkbox("ATEMIO6000") == 0 && checkbox("ATEMIO6100") == 0 && checkbox("SPARK") == 0 && checkbox("SPARK7162") == 0)
		ret = writevfd(value);
	return ret;
}

// Display a text on VFD
int writevfd(char *value)
{
	char *vfddev, *tmpvalue = NULL;
	int ret=0, len = 0;

	if(status.updatevfd == STOP) return 0;
	
	if(checkbox("TF7700") == 1 || checkbox("DM520") == 1 || checkbox("DM525") == 1)
		return writevfdioctl(value);
		
	if(checkchipset("BCM7424") == 1 || checkbox("DM7020HD") == 1 || checkbox("DM7020HDV2") == 1 || checkbox("DM900") == 1 || checkbox("DM920") == 1  || checkbox("VUDUO4K") == 1) //inihdp
	//if(checkchipset("BCM7424") == 1 || checkbox("DM7020HD") == 1 || checkbox("DM7020HDV2") == 1) //inihdp
	{
		int oledret = 0;
		m_lock(&status.vfdmutex, 3);
		oledret = oledtext(value);
		m_unlock(&status.vfdmutex, 3);
		return oledret;
	}
	
	if(durchlauf > 4)
	{
		*vfdtext='\0';
		durchlauf = 0;
	}
	else
		durchlauf = durchlauf + 1;
		
	vfddev = getconfig("vfddev", NULL);

	if(vfddev != NULL && value != NULL && strncmp(value, vfdtext, 63))
	{
		debug(100, "set %s to %s", vfddev, value);

		tmpvalue = malloc(63);
		if(tmpvalue == NULL)
		{
			err("no mem");
			return 1;
		}
		memset(tmpvalue, ' ', 63);

		len = strlen(value);

		if(checkbox("ATEMIO530") != 1  && checkbox("ATEMIO520") != 1 && checkbox("IPBOX91") != 1 && checkbox("ATEMIO6000") != 1 && checkbox("ATEMIO6100") != 1 && checkbox("SPARK") != 1 && checkbox("ATEMIO6200") != 1 && checkbox("SPARK7162") != 1 && checkchipset("3798MV200") != 1 && len > 63)
		{
			memcpy(tmpvalue, value, 63);
			tmpvalue[62] = '\0';
		}
		else if(status.standby > 0 && getconfigint("vfdisplaystandby", NULL) == 0 && (checkbox("ATEMIO6000") == 1 || checkbox("ATEMIO6100") == 1 || checkchipset("3798MV200") == 1))
		{	
			memcpy(tmpvalue, value, len);
			tmpvalue[5] = '\0';
		}
		else if((checkbox("ATEMIO6000") == 1 || checkbox("ATEMIO6100") == 1 || checkchipset("3798MV200") == 1) && getconfigint("vfdisplay", NULL) == 3)
		{	
			memcpy(tmpvalue, value, len);
			tmpvalue[5] = '\0';
		}
		else if((checkbox("ATEMIO530") == 1 || checkbox("ATEMIO520") == 1 || checkbox("IPBOX91") == 1 || checkbox("ATEMIO6000") == 1 || checkbox("ATEMIO6100") == 1 || checkbox("SPARK") == 1 || checkchipset("3798MV200") == 1) && len > 4)
		{
			memcpy(tmpvalue, value, 4);
			tmpvalue[4] = '\0';
		}
		else if((checkbox("ATEMIO530") == 1 || checkbox("ATEMIO520") == 1 || checkbox("IPBOX91") == 1 || checkbox("ATEMIO6000") == 1 || checkbox("ATEMIO6100") == 1 || checkbox("SPARK") == 1 || checkchipset("3798MV200") == 1) && len < 4)
		{
			memcpy(tmpvalue, value, len);
			tmpvalue[4] = '\0';
		}
		else if(checkbox("SPARK7162") == 1 || (checkbox("ATEMIO6200") == 1 && getconfigint("vfd_scroll", NULL) != 1 && len > 8))
		{
			memcpy(tmpvalue, value, len);
			tmpvalue[8] = '\0';
		}
		else if((checkrealbox("HD51") == 1 || checkrealbox("HD60") == 1 || checkrealbox("HD61") == 1) && len > 16 && getconfigint("vfd_scroll", NULL) != 1)
		{
			memcpy(tmpvalue, value, len);
			tmpvalue[16] = '\0';
		}
		else
		{
			memcpy(tmpvalue, value, len);
			tmpvalue[len] = '\0';
		}

		m_lock(&status.vfdmutex, 3);
		writesys(vfddev, tmpvalue, 0);
		m_unlock(&status.vfdmutex, 3);

		strncpy(vfdtext, value, 63);
	}

	free(tmpvalue);
	return ret;
}

// Get the icon state struct of a given icon id
struct vfdiconstate *getvfdiconstate(vfdicons icon)
{
	struct vfdiconstate *curstate = vfdiconstates;

	while(curstate)
	{
		if(curstate->icon == icon)
			return curstate;
		curstate = curstate->next;
	}
	return 0;
}

// Switch an icon on the VFD on or off
int setvfdicon(vfdicons id, int onoff)
{
	char *vfddev;
	struct vfdioctl data;
	int fd;
	int ret = 0;

	if(status.updatevfd == STOP) return 0;

	struct vfdiconstate *curstate = getvfdiconstate(id);
	if(curstate)
	{
		if(curstate->state == onoff) // no action, already set
			return 0;
		curstate->state = onoff;
	}

	vfddev = getconfig("vfddev", NULL);

	memset(&data, 0, sizeof(struct vfdioctl));

	data.start = 0x00;
	data.data[0] = id & 0x0f;
	data.data[4] = onoff;
	data.length = 5;

	m_lock(&status.vfdmutex, 3);
	fd = open (vfddev, O_WRONLY);
	ret = ioctl(fd, VFDICONDISPLAYONOFF, &data);
	close (fd);
	m_unlock(&status.vfdmutex, 3);

	return ret;
}

// Set all VFD symbols to either on or off
void setallvfdsymbols(int onoff)
{
	struct vfdiconstate *curstate = vfdiconstates;

	while(curstate)
	{
		setvfdicon(curstate->icon, onoff);
		curstate = curstate->next;
	}
}

// Add an icon state struct to the linked list
void addvfdiconstate(vfdicons icon)
{
	struct vfdiconstate *newIconState = malloc(sizeof(struct vfdiconstate));
	newIconState->icon = icon;
	newIconState->state = -1;

	if(vfdiconstates)
		newIconState->next = vfdiconstates;
	else
		newIconState->next = 0;
	vfdiconstates=newIconState;
}

// Initialize all VFD relevant parts (e.g. the linked list of all supported VFD icons, etc)
void initvfd()
{
	char* tmpstr = NULL;

	if(vfdiconstates!= 0)
		return; // Do not initialize twice

	*vfdtext='\0';
	durchlauf = 0;
	tmpstr = ostrcat(tmpstr, "<", 1, 0);
	tmpstr = ostrcat(tmpstr, PROGNAME, 1, 0);
	tmpstr = ostrcat(tmpstr, ">", 1, 0);
	if(checkbox("ATEMIO530") != 1 && checkbox("ATEMIO520") != 1 && checkbox("IPBOX91") != 1 && checkbox("ATEMIO6000") != 1 && checkbox("ATEMIO6100") != 1 && checkbox("SPARK") != 1)
		writevfd(string_toupper(tmpstr));
	free(tmpstr); tmpstr = NULL;

	addvfdiconstate(VFD_USB);
	addvfdiconstate(VFD_HD);
	addvfdiconstate(VFD_HDD);
	addvfdiconstate(VFD_LOCK);
	addvfdiconstate(VFD_BT);
	addvfdiconstate(VFD_MP3);
	addvfdiconstate(VFD_MUSIC);
	addvfdiconstate(VFD_DD);
	addvfdiconstate(VFD_MAIL);
	addvfdiconstate(VFD_MUTE);
	addvfdiconstate(VFD_PLAY);
	addvfdiconstate(VFD_PAUSE);
	addvfdiconstate(VFD_FF);
	addvfdiconstate(VFD_FR);
	addvfdiconstate(VFD_REC);
	addvfdiconstate(VFD_CLOCK);

	if(checkbox("DM7020HD") == 1 || checkbox("DM7020HDV2") == 1  || checkbox("VUDUO4K") == 1)
		initOLEDdream1();
	else if(checkbox("DM900") == 1 || checkbox("DM920") == 1)
		setled(1);
	else
		setallvfdsymbols(0);
	
	if(checkbox("ATEVIO7000") == 1 || checkbox("ATEMIO7600") == 1 || checkbox("SPARK") == 1)
	{
		if(getconfig("at7000frontrun", NULL) != NULL)
		{
			tmpstr = ostrcat("fp_control -P ",getconfig("at7000frontrun", NULL), 0, 0);
			system(tmpstr);
			free(tmpstr); tmpstr=NULL;
		}
	}
}

void cleanupvfd()
{
	setallvfdsymbols(0);
	writevfd("SEE YOU");

	status.updatevfd = STOP;

	struct vfdiconstate *curstate=vfdiconstates;
	struct vfdiconstate *tmpstate=0;

	while(curstate)
	{
		tmpstate=curstate;
		curstate=curstate->next;
		free(tmpstate);
	}
	vfdiconstates = NULL;
}

int recstar = 0;

void updatevfd()
{
	char* tmpstr = NULL;

	// Set VFD icons:
	if(checkchipset("BCM7424") != 1 && checkbox("DM7020HD") != 1 && checkbox("DM7020HDV2") != 1 && checkbox("DM900") != 1 && checkbox("DM920") != 1 && checkbox("VUDUO4K") != 1) //inihdp
		setvfdicon(VFD_REC, status.recording > 0);
	if(status.standby == 0 && checkbox("DM900") != 1 && checkbox("DM920") != 1 && checkbox("DM7020HD") != 1 && checkbox("DM7020HDV2") != 1 && checkbox("ATEMIO530") != 1 && checkbox("ATEMIO520") != 1 && checkbox("IPBOX91") != 1 && checkbox("ATEMIO6000") != 1 && checkbox("ATEMIO6100") != 1  && checkbox("VUDUO4K") != 1)
	{
		//setvfdicon(VFD_USB, 0);
		if(getaktvideosize() == 0) //videosize is ok
			setvfdicon(VFD_HD, status.videosize.h > 576);
		else
			setvfdicon(VFD_HD, 0);
		//setvfdicon(VFD_HDD, 0);
		setvfdicon(VFD_LOCK, status.aktservice->channel && status.aktservice->channel->crypt > 0);
		//setvfdicon(VFD_BT, 0);
		//setvfdicon(VFD_MP3, 0);
		//setvfdicon(VFD_MUSIC, 0);
		setvfdicon(VFD_DD, status.aktservice->channel && status.aktservice->channel->audiocodec == AC3);
		//setvfdicon(VFD_MAIL, 0);
		setvfdicon(VFD_MUTE, status.mute == 1);
		setvfdicon(VFD_PLAY, status.play == 1);
		setvfdicon(VFD_PAUSE, status.pause == 1);
		setvfdicon(VFD_FF, status.playspeed > 0);
		setvfdicon(VFD_FR, status.playspeed < 0);
		//setvfdicon(VFD_CLOCK, 0);
	}

	if(status.updatevfd != START)
		return;

	tmpstr = ostrcat(tmpstr, NULL, 1, 0);
	
	if(status.standby == 0)
	{
		if(status.aktservice->type != CHANNEL && status.aktservice->type != HDMIIN)
		{
			free(tmpstr);
			return;
		}

		switch(getconfigint("vfdisplay", NULL))
		{
			case 1: // channel + time
				if(status.aktservice->channel != NULL)
					tmpstr = ostrcat(tmpstr, status.aktservice->channel->name, 1, 0);
				tmpstr = ostrcat(tmpstr, gettime(NULL, " %H:%M"), 1, 1);
				break;
			case 2: // time + channel
				tmpstr = ostrcat(tmpstr, gettime(NULL, "%H:%M "), 1, 1);
				if(status.aktservice->channel != NULL)
					tmpstr = ostrcat(tmpstr, status.aktservice->channel->name, 1, 0);
				break;
			case 3: // only time
				tmpstr = ostrcat(tmpstr, gettime(NULL, "%H:%M"), 1, 1);
				break;
			case 4: // only time (small)
				tmpstr = ostrcat(tmpstr, gettime(NULL, "%H%M"), 1, 1);
				break;
			case 5: // only channel number
				if(oldvfdchannel != status.aktservice->channel)
				{
					tmpstr = ostrcat(tmpstr, "....", 1, 0);
					if(status.aktservice->channel != NULL)
					{
						oldvfdchannel = status.aktservice->channel;
						if(ostrncmp("(BOUQUET)-", status.aktservice->channellist, 10) == 0 && strlen(status.aktservice->channellist) > 10)
						{
							struct mainbouquet* mnode = getmainbouquet(status.aktservice->channellist + 10);
							if(mnode != NULL)
							{
								struct bouquet* bnode = getbouquetbychannel(mnode->bouquet, status.aktservice->channel->serviceid, status.aktservice->channel->transponderid);
								if(bnode != NULL)
								{
									free(tmpstr); tmpstr=NULL;
									tmpstr = ostrcat(tmpstr, oitoa(bnode->nr), 1, 1);
									oldvfdchannelnr = bnode->nr;
								}
							}
						}
					}
				}
				else if(oldvfdchannelnr != -1)
					tmpstr = ostrcat(tmpstr, oitoa(oldvfdchannelnr), 1, 1);
				break;
			default: // only channel name
				if(status.aktservice->channel != NULL)
					tmpstr = ostrcat(tmpstr, status.aktservice->channel->name, 1, 0);
		}
	}
	else
	{
		switch(getconfigint("vfdisplaystandby", NULL))
		{
			case 1: // off
				tmpstr = ostrcat(tmpstr, " ", 1, 0);
				if(checkchipset("BCM7424") == 1 || checkbox("DM7020HD") == 1 || checkbox("DM7020HDV2") || checkbox("DM900") == 1 || checkbox("DM920") == 1  || checkbox("VUDUO4K") == 1) //inihdp
				{
					tmpstr = ostrcat(tmpstr, " ", 1, 0);
					//free(tmpstr);
					//return;
				}
				break;
			case 2: // date + time
				if(checkchipset("BCM7424") == 1 || checkbox("DM7020HD") == 1 || checkbox("DM7020HDV2") == 1 || checkbox("DM900") == 1 || checkbox("DM920") == 1  || checkbox("VUDUO4K") == 1) //inihdp
					tmpstr = ostrcat(tmpstr, gettime(NULL, "%d.%m - %H:%M"), 1, 1);
				else
					tmpstr = ostrcat(tmpstr, gettime(NULL, "%d.%m %H:%M"), 1, 1);
				break;
			case 3: // date
				tmpstr = ostrcat(tmpstr, gettime(NULL, "%d.%m.%y"), 1, 1);
				break;
			default: // time
				if(checkbox("ATEMIO530") == 1 || checkbox("ATEMIO520") == 1 || checkbox("IPBOX91") == 1 || checkbox("SPARK") == 1)
					tmpstr = ostrcat(tmpstr, gettime(NULL, "%H%M"), 1, 1);
				else
					tmpstr = ostrcat(tmpstr, gettime(NULL, "%H:%M"), 1, 1);
				break;
		}

		// Switch off all VFD icons:
		if(checkchipset("BCM7424") != 1 || checkbox("DM7020HD") == 1 || checkbox("DM7020HDV2") == 1 || checkbox("DM900") == 1 || checkbox("DM920") == 1  || checkbox("VUDUO4K") == 1) //inihdp
			setallvfdsymbols(0);
	}
	if(VFD_Recordthread != NULL && getconfigint("vfdisplayrecord", NULL) == 3 && status.recording > 0)
	{ }
	else if(VFD_Recordthread != NULL && getconfigint("vfdisplayrecord", NULL) == 4 && status.recording > 0)
	{	
		tmpstr = ostrcat("* ", tmpstr, 0, 0);
		writevfd(tmpstr);
	}
	else if(VFD_Recordthread != NULL && getconfigint("vfdisplayrecord", NULL) == 5 && status.recording > 0)
	{	
		if(recstar == 0)
		{
			tmpstr = ostrcat("* ", tmpstr, 0, 0);
			recstar = 1;
		}
		else
		{
			tmpstr = ostrcat("  ", tmpstr, 0, 0);
			recstar = 0;
		}
		writevfd(tmpstr);
	}
	else	
		writevfd(tmpstr);
	free(tmpstr);
}

void vfdrecordthread()
{
	int action = 0;
	int brightness = 0;
	int aufab = 0;
	int brightnesshelp = 0;
	char* merkvfd = NULL;

	if(VFD_Recordthread == NULL) return;
	
	while(VFD_Recordthread->aktion != STOP && getconfigint("vfdisplayrecord", NULL) != 0 && status.recording > 0)
	{
		//brightness = getconfigint("vfdbrightness", NULL);
		//if( brightness == 0 )
			brightness = 7;
		//brightnesshelp = brightness;
		free(merkvfd);merkvfd=NULL;
		merkvfd = ostrcat(vfdtext, "", 0, 0);
				
		while(VFD_Recordthread->aktion != STOP && getconfigint("vfdisplayrecord", NULL) == 1 && status.recording > 0)
		{
			action = 1;
			setvfdbrightness(0);
			sleep(1);
			setvfdbrightness(brightness);
			sleep(1);
		}
		
		while(VFD_Recordthread->aktion != STOP && getconfigint("vfdisplayrecord", NULL) == 2 && status.recording > 0)
		{
			action = 1;
			if(brightnesshelp == brightness)
				aufab = 1;
			if(brightnesshelp == 0)
				aufab = 0;
				
			if(aufab == 1)
				brightnesshelp--;
			if(aufab == 0)
				brightnesshelp++;	 	
			
			setvfdbrightness(brightnesshelp);
			usleep(500000);
		}
		
		while(VFD_Recordthread->aktion != STOP && getconfigint("vfdisplayrecord", NULL) == 3 && status.recording > 0)
		{
			action = 2;
			if(checkchipset("BCM7358") == 1 || checkbox("ATEMIO6200") == 1 || checkchipset("BCM7424") == 1 || checkbox("DM7020HD") == 1 || checkbox("DM7020HDV2") == 1 || checkbox("DM900") == 1 || checkbox("DM920") == 1  || checkbox("VUDUO4K") == 1) //inihde inihdp
				writevfd("RECORD");
			else if(checkbox("ATEMIO6000") == 1 || checkbox("ATEMIO6100") == 1 || checkbox("SPARK") == 1)
				writevfd("REC");
			else
				writecentervfd("RECORD");
			sleep(1);
		}
		
		while(VFD_Recordthread->aktion != STOP && getconfigint("vfdisplayrecord", NULL) == 4 && status.recording > 0)
		{
			action = 0;
			sleep(1);
		}
		
		while(VFD_Recordthread->aktion != STOP && getconfigint("vfdisplayrecord", NULL) == 5 && status.recording > 0)
		{
			action = 0;
			sleep(1);
		}
		
		if(action == 1)
		{
			brightness = getconfigint("vfdbrightness", NULL);
			if( brightness == 0 )
				brightness = 7;
			setvfdbrightness(brightness);
			action = 0;
			aufab = 0;
		}
		else if(action == 2)
			writevfd(merkvfd);
		
		sleep(1);
	}
	VFD_Recordthread = NULL;
	free(merkvfd);merkvfd=NULL;
} 		

void setled(int typ)
{
	int wert = 0;
	if(typ == 1)
	{
		writesys("/proc/stb/fp/led_pattern_speed", "0", 1);
		if(status.standby == 0)
			wert = getconfigint("dm900frontrun", NULL);	
		else
			wert = getconfigint("dm900frontsleep", NULL);
		if(wert == 0)
		{
			writesys("/proc/stb/fp/led0_pattern", "00000000", 1);
			writesys("/proc/stb/fp/led1_pattern", "00000000", 1);
		}
		if(wert == 1)
		{
			writesys("/proc/stb/fp/led0_pattern", "00000000", 1);
			writesys("/proc/stb/fp/led1_pattern", "ffffffff", 1);
		}
		if(wert == 2)
		{
			writesys("/proc/stb/fp/led0_pattern", "ffffffff", 1);
			writesys("/proc/stb/fp/led1_pattern", "00000000", 1);
		}
	}
}
			 	

#endif
