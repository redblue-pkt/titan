/*
 * TopfieldVFD.h
 *
 *  Created on: Jul 30, 2011
 *      Author: topfi
 */

#ifndef TOPFIELDVFD_H_
#define TOPFIELDVFD_H_

#ifndef dword
typedef unsigned long	dword;
#endif
#ifndef word
typedef unsigned short	word;
#endif
#ifndef byte
typedef unsigned char	byte;
#endif

typedef struct
{
  word    year;
  word    month;
  word    day;
  word    dow;
  char    sdow[4];
  word    hour;
  word    min;
  word    sec;
  dword   now;
} frontpanel_ioctl_time;

typedef struct
{
  byte    bright;
} frontpanel_ioctl_brightness;

typedef struct
{
  byte    reason;
} frontpanel_ioctl_bootreason;

typedef struct
{
  byte    onoff;
} frontpanel_ioctl_irfilter;

typedef struct
{
  dword   Icons1;
  dword   Icons2;
  byte    BlinkMode;
} frontpanel_ioctl_icons;

typedef struct
{
  byte    TypematicDelay;
} frontpanel_ioctl_typematicdelay;

typedef struct
{
  byte    TypematicRate;
} frontpanel_ioctl_typematicrate;

typedef struct
{
  byte    KeyEmulation;
} frontpanel_ioctl_keyemulation;

typedef struct
{
  byte    AllCaps;
} frontpanel_ioctl_allcaps;

typedef struct
{
  byte    ScrollMode;
  byte    ScrollPause;
  byte    ScrollDelay;
} frontpanel_ioctl_scrollmode;

#define FRONTPANELGETTIME               0x40003a00 | (sizeof(frontpanel_ioctl_time) << 16)
#define FRONTPANELSETTIME               0x40003a01
#define FRONTPANELSYNCSYSTIME           0x40003a02
#define FRONTPANELCLEARTIMER            0x40003a03
#define FRONTPANELSETTIMER              0x40003a04
#define FRONTPANELBRIGHTNESS            0x40003a05 | (sizeof(frontpanel_ioctl_brightness) << 16)
#define FRONTPANELIRFILTER1             0x40003a06
#define FRONTPANELIRFILTER2             0x40003a07
#define FRONTPANELIRFILTER3             0x40003a08
#define FRONTPANELIRFILTER4             0x40003a09
#define FRONTPANELPOWEROFF              0x40003a0a
#define FRONTPANELBOOTREASON            0x40003a0b | (sizeof(frontpanel_ioctl_bootreason) << 16)
#define FRONTPANELCLEAR                 0x40003a0c
#define FRONTPANELTYPEMATICDELAY        0x40003a0d | (sizeof(frontpanel_ioctl_typematicdelay) << 16)
#define FRONTPANELTYPEMATICRATE         0x40003a0e | (sizeof(frontpanel_ioctl_typematicrate) << 16)
#define FRONTPANELKEYEMULATION          0x40003a0f | (sizeof(frontpanel_ioctl_keyemulation) << 16)
#define FRONTPANELREBOOT                0x40003a10
#define FRONTPANELSYNCFPTIME            0x40003a11
#define FRONTPANELSETGMTOFFSET          0x40003a12
#define FRONTPANELRESEND                0x40003a13
#define FRONTPANELALLCAPS               0x40003a14 | (sizeof(frontpanel_ioctl_allcaps) << 16)
#define FRONTPANELSCROLLMODE            0x40003a15 | (sizeof(frontpanel_ioctl_scrollmode) << 16)
#define FRONTPANELICON                  0x40003a20 | (sizeof(frontpanel_ioctl_icons) << 16)

frontpanel_ioctl_icons ioOffOn          = {0x00000002,0x00000000,0x0f};
frontpanel_ioctl_icons ioOffFlush       = {0x00000002,0x00000000,0x06};
frontpanel_ioctl_icons ioOffOff         = {0x00000002,0x00000000,0x00};

frontpanel_ioctl_icons ioRec1On         = {0x00001000,0x00000000,0x0f};
frontpanel_ioctl_icons ioRec2On         = {0x00002000,0x00000000,0x0f};
frontpanel_ioctl_icons ioRecBothOn      = {0x00003000,0x00000000,0x0f};
frontpanel_ioctl_icons ioRec1Flush      = {0x00001000,0x00000000,0x06};
frontpanel_ioctl_icons ioRec2Flush      = {0x00002000,0x00000000,0x06};
frontpanel_ioctl_icons ioRecBothFlush   = {0x00003000,0x00000000,0x06};
frontpanel_ioctl_icons ioRec1Off        = {0x00001000,0x00000000,0x00};
frontpanel_ioctl_icons ioRec2Off        = {0x00002000,0x00000000,0x00};
frontpanel_ioctl_icons ioRecBothOff     = {0x00003000,0x00000000,0x00};

frontpanel_ioctl_icons ioClockFlush     = {0x00000020,0x00000000,0x06};
frontpanel_ioctl_icons ioClockOff       = {0x00000020,0x00000000,0x00};

frontpanel_ioctl_icons ioHddClear       = {0x00000000,0x00ff8000,0x00};

frontpanel_ioctl_icons ioCDClear        = {0x00000000,0x00001ffe,0x00};

frontpanel_ioctl_icons ioEthOn          = {0x00400000,0x00000000,0x0b};
frontpanel_ioctl_icons ioEthOff         = {0x00400000,0x00000000,0x00};

frontpanel_ioctl_icons ioUsbOn          = {0x0c000000,0x00000000,0x0f};
frontpanel_ioctl_icons ioUsbOff         = {0x0c000000,0x00000000,0x00};

frontpanel_ioctl_icons ioTimeshiftOn    = {0x00000080,0x00000000,0x0f};
frontpanel_ioctl_icons ioTimeshiftOff   = {0x00000080,0x00000000,0x00};


void sendIoctl(unsigned long cmd, void* data)
{
	char* vfddev = getconfig("vfddev", NULL);

	if(status.updatevfd == STOP) return;

	m_lock(&status.vfdmutex, 3);
	int fd = open (vfddev, O_WRONLY);
	ioctl(fd, cmd, data);
	close (fd);
	m_unlock(&status.vfdmutex, 3);
}

void updateRec(int force)
{
	static int lastRec = -1;
	int curRec = status.recording;

	if(force)
	{
		lastRec=-1;
	}

	if(lastRec != curRec)
	{
		if(lastRec > curRec)
		{
			sendIoctl(FRONTPANELICON, &ioRecBothOff);
		}

		if(curRec == 0)
		{
			sendIoctl(FRONTPANELICON, &ioClockOff);
		}
		else
		{
			if( getconfigint("blinking_clock_during_recording",pluginname))
			{
				switch(curRec)
				{
					case 1: sendIoctl(FRONTPANELICON, &ioRec1Flush);
							sendIoctl(FRONTPANELICON, &ioClockFlush);
							break;
					case 2: sendIoctl(FRONTPANELICON, &ioRec2Flush);
							sendIoctl(FRONTPANELICON, &ioClockFlush);
							break;
				}
			}
			else
			{
				switch(curRec)
				{
					case 1: sendIoctl(FRONTPANELICON, &ioRec1On);
							break;
					case 2: sendIoctl(FRONTPANELICON, &ioRec2On);
							break;
				}
			}
		}
		lastRec = curRec;
	}
}

void updateProgress(int force)
{
	static int lastProgress = -1;

	if(force)
	{
		lastProgress=-1;
	}

	if(lastProgress==-1)
	{
		// To be done.
		sendIoctl(FRONTPANELICON, &ioCDClear);

		lastProgress = 0;
	}
}

void updateHDD(int force)
{
	static int lastHDD = -1;

	if(force)
	{
		lastHDD=-1;
	}

	if(lastHDD==-1)
	{
		// To be done.
		sendIoctl(FRONTPANELICON, &ioHddClear);

		lastHDD = 0;
	}
}

void updateUSB(int force)
{
	static int lastUSB = -1;

	if(force)
	{
		lastUSB=-1;
	}

	if(lastUSB==-1)
	{
		// To be done.
		sendIoctl(FRONTPANELICON, &ioUsbOff);

		lastUSB = 0;
	}
}

void updateEth(int force)
{
	static int lastEth = -1;

	if(force)
	{
		lastEth=-1;
	}

	if(lastEth==-1)
	{
		// To be done.
		sendIoctl(FRONTPANELICON, &ioEthOff);

		lastEth = 0;
	}
}

void updateTimeshift(int force)
{
	static int lastTS = -1;

	if(force)
	{
		lastTS=-1;
	}

	if(lastTS==-1)
	{
		// To be done.
		sendIoctl(FRONTPANELICON, &ioTimeshiftOff);

		lastTS = 0;
	}
}

#endif /* TOPFIELDVFD_H_ */
