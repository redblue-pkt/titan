#ifndef STRUCT_H
#define STRUCT_H

#define _GNU_SOURCE

#ifdef SIMULATE
#include <mcheck.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <libgen.h>
#include <fcntl.h>
#include <ctype.h>
#include <linux/fb.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <bpamem.h>
#include <png.h>
#include <jpeglib.h>
#include <libmmeimage.h>
#include <libintl.h>
#include <locale.h>
#include <math.h>
#include <linux/input.h>
#include <sys/wait.h>
#include <time.h>
#include <dirent.h>
#include <fnmatch.h>
#include <dlfcn.h>
#include <pthread.h>
#include <linux/dvb/frontend.h>
#include <linux/dvb/dmx.h>
#include <linux/dvb/audio.h>
#include <linux/dvb/video.h>
#include <sys/vfs.h>
#include <inttypes.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <zlib.h>
#include <net/if.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <errno.h>
#include <linux/dvb/version.h>
#include <setjmp.h>
#include <sys/mount.h>
#include <execinfo.h>
#include <ifaddrs.h>
#include <linux/dvb/ca.h>
#include <poll.h>

#ifndef SIMULATE
#include <linux/stmfb.h>
#endif

//for freetype
#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_CACHE_H

//for eplayer
#ifdef EPLAYER3
#include <common.h>
#endif

#ifdef EPLAYER4
#include <gst/gst.h>
#include <gst/pbutils/missing-plugins.h>
#endif

#define VERSION "1.2"
#define PROGNAME "titan"
#define COPYRIGHT "NIT"
#define CRONTRIBUT "obi, black, dvboxer, oxygen, gost"

#define _(x) gettext(x)
#define MINMALLOC 4096
#define SCROLLBARWIDTH 15
#define SCROLLBARBORDERSIZE 1
#define FONTPOSYOFFSET -3
#define FB "fb0"
#define FB1 "fb1"
#define SKINFB "skinfb"
#define ACCELFB "accelfb"
#define CONFIGFILE PROGNAME".cfg"
#define KILLPROG "killall -9 "PROGNAME
#define REBOOT "init 6"
#define FILELISTDELMARK 255
#define PLUGINDELMARK 254
#define MAXDVBADAPTER 2
#define MAXFRONTENDDEV 4
#define MAXDEMUXDEV 4
#define MAXDEMUXDEVOPEN 10
#define MAXAUDIODEV 4
#define MAXVIDEODEV 4
#define MAXCIDEV 4
#define MAXCADEV 4
#define MAXDVRDEV 4
#define RCTIMEOUT 999999
#define RCTHREADSTANDBY 999998
#define MAXSERVICETYPE 10
#define TSSIZE 188
#define CHANNELCACHEMAX 1000
#define MAXHTTPDCONN 20
#define MAXHTMLLINE 300
#define LISTHASHSIZE 27
#define MAXCASESSION 16
#define MAXCASERVICE 20

//CA Defines
#define T_SB 0x80		//sb (h<--m)
#define T_RCV 0x81		//receive (h-->m)
#define T_CREATE_T_C 0x82	//create transport connection (h-->m)
#define T_C_T_C_REPLY 0x83	//ctc reply (h<--m)
#define T_DELETE_T_C 0x84	//delete tc (h<->m)
#define T_D_T_C_REPLY 0x85	//dtc reply (h<->m)
#define T_REQUEST_T_C 0x86	//request transport connection (h<--m)
#define T_NEW_T_C 0x87		//new tc - reply to t_request (h-->m)
#define T_T_C_ERROR 0x77	//error creating tc (h-->m)
#define T_DATA_LAST 0xA0	//convey data from higher (h<->m)
#define T_DATA_MORE 0xA1	//convey data from higher (h<->m)

// VFD icons supported (more or less) on all boxes:
typedef enum { VFD_USB = 0x10, VFD_HD, VFD_HDD, VFD_LOCK, VFD_BT, VFD_MP3, VFD_MUSIC, VFD_DD, VFD_MAIL, VFD_MUTE, VFD_PLAY, VFD_PAUSE, VFD_FF, VFD_FR, VFD_REC, VFD_CLOCK } vfdicons;

#define MIN(a,b) (a < b ? a : b)
#define MAX(a,b) (a > b ? a : b)

#define bcdtoint(i) ((((i & 0xf0) >> 4) * 10) + (i & 0x0f))

#define alpha_composite(composite, fg, alpha, bg) { \
unsigned short temp = ((unsigned short)(fg)*(unsigned short)(alpha) +   \
(unsigned short)(bg)*(unsigned short)(255 - (unsigned short)(alpha)) +  \
(unsigned short)128);  \
(composite) = (unsigned char)((temp + (temp >> 8)) >> 8); }

enum {LEFT=20000, CENTER, RIGHT};
enum {TOP=20000, MIDDLE, BOTTOM};
enum {BOTTOMLEFT, BOTTOMRIGHT, TOPLEFT, TOPRIGHT};
enum {LEFTRIGHT=1, TOPBOTTOM, LEFTMIDDLE, TOPMIDDLE};
enum {NO, YES, AUTONO, AUTOYES};
enum {UNDEF, LISTBOX, TEXTBOX, CHOICEBOX, INPUTBOX, INPUTBOXNUM, FILELIST, PROGRESSBAR, MENU, MULTIPROGRESSBAR, GRID, GRIDBR, TEXTBOXGRIDBR};
enum {FRONTENDDEV, DEMUXDEV, VIDEODEV, AUDIODEV, CADEV, CIDEV, FRONTENDDEVDUMMY, DVRDEV};
enum {TV, RADIO};
enum {RECDIRECT, RECTIMER, RECTIMESHIFT, RECSTREAM, RECPLAY};
enum {AC3 = 0, MPEGA = 1, DTS = 2, LPCM = 6, AAC = 8, AACHE = 9};
enum {MPEGV = 0, MPEG4V = 4, H264 = 1};
enum {ALLCHANNEL, SATCHANNEL, PROVIDERCHANNEL, AZCHANNEL, BOUQUETCHANNEL, SATLIST, PROVIDERLIST, AZLIST, MAINBOUQUETLIST};
enum {NOMODE, MVMODE, RMMODE, CPMODE, PROTECTMODE};
enum {NOTHING, CHANNEL, STILLPIC, RECORDDIRECT, RECORDTIMER, RECORDTIMESHIFT, RECORDSTREAM, TMPOTHER, RECORDPLAY};
enum {START, PAUSE, STOP};
enum {DEACTIVE, ACTIVE, INPAUSE, ERROR};
enum {FUNCTEXT, FUNCPIC, FUNCPROGRESS};
enum {CASESSIONCREATE, CASESSIONBUSY, CASESSIONDEL, CASESSIONSTART, CASESSIONFINAL, CARESFIRSTENQUIRY, CARESCHANGE, CARESENQUIRY, CADATETIMESEND, CAMMIIDLE, CAMMIDISPLAYREPLAY, CAMMIFAKEOK};

struct queue
{
	//0-99 for ca module
	int type;
	int flag;
	int len;
	void* data;
	struct queue* prev;
	struct queue* next;
};

struct casession
{
	int inuse;
	int sessionnr;
	int action;
	int state;
	int mmimanager;
	int camanager;
	int datetimemanager;
	int appmanager;
	int resmanager;
	char* mmititle;
	char* mmisubtitle;
	char* mmitext;
	char* mmibottom;
};

struct caslot
{
	struct casession casession[MAXCASESSION];
	int connid;
	int poll;
	int status;
	int rlen;
	unsigned char* rbuf;
	char* name;
	int fastrun;
	char* caids;
};

struct hdd
{
	char* device;
	int partition;
	unsigned long size;
	char* filesystem;
	char* label;
	char* uuid;
	char* vendor;
	char* model;
	int removable;
	int read;
	int write;
	int sleeptime;
	int notchanged;
	struct hdd* prev;
	struct hdd* next;
};

struct scaninfo
{
	struct dvbdev* fenode;
	struct transponder* tpnode;
	struct skin* scanscreen;
	struct skin* listbox;
	int threadend;
	int scantype;
	int fetype;
	int orbitalpos;
	int timeout;
	int onlyfree;
	int clear;
	char* satname;
	int tpcount;
	int tvcount;
	int newtvcount;
	int radiocount;
	int newradiocount;
	int datacount;
	int newdatacount;
} scaninfo;

struct screensaver
{
	int type;
	int speed;
	int newposx;
	int newposy;
	struct skin* screen;
	struct skin* screennode;
	struct skin* filelist;
	struct skin* aktnode;
	struct pic* pic;
	char* value;
};

struct playlist
{
	char* file;
	struct playlist* prev;
	struct playlist* next;
};

struct mainplaylist
{
	char* name;
	char* filename;
	struct playlist* playlist;
	struct mainplaylist* prev;
	struct mainplaylist* next;
};

struct epgrecord
{
	time_t begin;
	time_t end;
	int posx;
	int size;
	struct epgrecord* prev;
	struct epgrecord* next;
};

struct download
{
	char* host;
	char* page;
	int port;
	char* filename;
	int connfd;
	int proz;
	unsigned int maxkb;
	unsigned int aktkb;
	int ret;
};

struct epgscanlist
{
	int serviceid;
	unsigned long transponderid;
	time_t scantime;
	struct epgscanlist* prev;
	struct epgscanlist* next;
};

struct rgba
{
	char r;
	char g;
	char b;
	char a;
};

struct subclutentry
{
	unsigned char Y, Cr, Cb, T;
	unsigned char valid;
};

struct subclut
{
	unsigned char clutid;
	unsigned char clutversionnumber;
	struct subclutentry entries2bit[4];
	struct subclutentry entries4bit[16];
	struct subclutentry entries8bit[256];
	struct subclut *next;
};

struct subpagereg
{
	int regid;
	int reghorizontaladdress;
	int regverticaladdress;
	int scaleposx, scaleposy;
	struct subpagereg *next;
};

struct subregobj
{
	int objid;
	int objtype;
	int objproviderflag;
	int objhorizontalpos;
	int objverticalpos;
	// not supported right now...
	int foregroundpixel;
	int backgroundpixel;
	struct subregobj *next;
};

struct subreg
{
	int regid;
	int versionnumber;
	int height, width;
	int scaleheight, scalewidth;
	int depth;
	unsigned char *buf;
	struct rgba *palette;
	int clutid;
	int committed;
	struct subregobj *objects;
	struct subreg *next;
};

struct subpage
{
	int pageid;
	time_t pagetimeout;
	int pageversionnumber;
	int state;
	int pcssize;
	struct subpagereg *pageregions;
	struct subreg *regions;
	struct subclut *cluts;
	struct subpage *next;
};

struct bitstream
{
        unsigned char *data;
        int size;
        int avail;
        int consumed;
};

//there are a copyrectimer function
//if you change the struct you must change this funktion
struct rectimer
{
	char* name;
	time_t begin;
	time_t end;
	int repeate;
	int afterevent;
	char* pincode;
	int disabled;
	int justplay;
	char* recpath;
	int servicetype;
	char* channellist;
	int serviceid;
	//0 = nothing
	//1 = running
	//2 = end
	//3 = error
	int status;
	char* errstr;
	struct service* servicenode;
	unsigned long transponderid;
	struct rectimer *prev;
	struct rectimer *next;
};

struct inetwork
{
	int id;
	char* device;
	char* ip;
	char* netmask;
	char* mac;
	char* broadcast;
	int dhcp;
	struct inetwork *prev;
	struct inetwork *next;
};

struct rcmap
{
	char* name;
	int key;
	int newkey;
	struct rcmap* next;
};

struct splitstr
{
	char* part;
};

struct channelcache
{
	int serviceid;
	unsigned long transponderid;
	struct channel* chnode;
	struct channelcache* next;
};

struct filelist
{
	int type;
	char* name;
	char* shortname;
	char* path;
	char* imdbpath;
	off64_t size;
	time_t date;
};

struct skin
{
	char* name;
	uint8_t type;
	uint8_t wrap;
	uint8_t bordersize;
	uint8_t scrollbar;
	uint16_t halign;
	uint16_t valign;
	int16_t posx;
	int16_t posy;
	int16_t width;
	int16_t height;
	long bordercol;
	long deaktivcol;
	long shadowcol;
	uint8_t shadowsize;
	uint8_t shadowpos;
	uint8_t fontsize;
	uint8_t fontsize2;
	long fontcol;
	long fontcol2;
	char* font;
	int16_t zorder;
	int16_t titlealign;
	char* title;
	char* text;
	char* text2;
	long bgcol;
	long bgcol2;
	long titlebgcol;
	long titlebgcol2;
	long progresscol;
	char* pic;
	int16_t picwidth;
	int16_t picheight;
	uint16_t hspace;
	uint16_t vspace;
	char* (*skinfunc)(void*, void*);
	char* param1;
	char* param2;
	char* parent;
	char* input;
	char* mask;
	uint16_t textposx;
	uint16_t textposx2;
	int16_t gradient;
	int16_t titlegradient;
	uint8_t transparent;
	uint8_t hidden;
	uint8_t funcrettype;
	int8_t charspace;
	int8_t bordertype;
	int16_t borderradius;
	uint16_t bgspace;

	uint8_t titlesize;
	struct filelist* filelist;
	struct skin* parentpointer;
	char* ret;
	char* choiceboxvalue;
	unsigned int linecount;
	unsigned int pagecount;
	unsigned int poscount;
	unsigned int gridcol;
	int aktpage;
	int aktline;
	uint8_t scrollbarbordersize;
	uint8_t prozposx;
	uint8_t prozposy;
	uint8_t prozwidth;
	uint8_t prozheight;
	uint8_t picprozwidth;
	uint8_t del;
	uint8_t picprozheight;
	int16_t scrollbarheight;
	int16_t progresssize;
	int16_t scrollbarwidth;
	int16_t scrollbarpos;
	int16_t rposx;
	int16_t rposy;
	int16_t rwidth;
	int16_t rheight;
	int16_t rpicwidth;
	int16_t rpicheight;
	int16_t iposx;
	int16_t iposy;
	int16_t iwidth;
	int16_t iheight;
	char* handle;
	char* handle1;
	struct epgrecord* epgrecord;
	char* pluginhandle;
	struct skin *select;
	struct skin *child;
	struct skin *prev;
	struct skin *next;
};

struct epg
{
	int eventid;
	int version;
	time_t starttime;
	time_t endtime;
	char* title;
	char* subtitle;
	char* desc;
	int desclen;
	int desccomplen;
	int parentalrating;
	struct epg* prev;
	struct epg* next;
};

struct shutdowntimer
{
	int active,type; //type: 0=DeepStandby, 1=Standby
	long long shutdown_time;
};

struct dvbdev
{
	char* dev;
	int fd;
	uint8_t type;
	uint8_t adapter;
	uint8_t devnr;
	uint8_t felock;
	struct transponder* felasttransponder;
	struct transponder* feakttransponder;
	int feunicable;
	int feloffrequency;
	int feaktband;
	int feaktpolarization;
	int fedmxsource;
	char* feaktnr;
	char* feshortname;
	struct dvb_frontend_info* feinfo;
	struct caslot* caslot;
	struct dvbdev *next;
};

//there are a copytransponder and createtransponder function
//if you change this struct you must change the function
struct transponder
{
	unsigned long id;
	uint8_t fetype;
	uint8_t polarization;
	uint8_t modulation;
	uint8_t fec;
	uint8_t pilot;
	uint8_t rolloff;
	uint8_t inversion;
	uint8_t system;
	uint8_t flag;
	// 0 = unknown
	// 1 = tunable
	// 2 = not tunable
	uint8_t tunablestatus;
	int orbitalpos;
	unsigned int frequency;
	int symbolrate;
	struct transponder *next;
};

struct provider
{
	int providerid;
	char* name;
	int flag;
	struct provider *prev;
	struct provider *next;
};

struct subtitle
{
	char* name;
	int16_t pid;
	int16_t type;
	int16_t id1;
	int16_t id2;
	struct subtitle* next;
};

struct esinfo
{
	int streamtype;
	int pid;
	struct esinfo* next;
};

struct cadesc
{
	int len;
	int systemid;
	int reserved;
	int pid;
	char* privat;
	struct cadesc* next;
};

struct pmt
{
	int programnumber;
	int versionnumber;
	int currentnextindicator;
};

struct audiotrack
{
	char* name;
	uint8_t audiocodec;
	int16_t audiopid;
	struct audiotrack* next;
};

struct linkedchannel
{
	unsigned long transponderid;
	int serviceid;
	struct linkedchannel* next;
}

//there are a createchannel function
//if you change this struct you must change the function
struct channel
{
	char* name;
	unsigned long transponderid;
	int providerid;
	int serviceid;
	int servicetype;
	uint8_t flag;
	uint8_t crypt;
	int8_t videocodec;
	int8_t audiocodec;
	int16_t videopid;
	int16_t audiopid;
	int16_t ac3audiopid;
	int16_t txtpid;
	int16_t pcrpid;
	int16_t pmtpid;
	uint8_t protect;
	struct transponder *transponder;
	struct provider *provider;
	struct audiotrack *audiotrack;
	struct subtitle *subtitle;
	struct linkedchannel *linkedchannel;
	struct pmt *pmt;
	struct cadesc *cadesc;
	struct esinfo *esinfo;
	struct epg *epg;
	struct channel *prev;
	struct channel *next;
};

struct sat
{
	char* name;
	int orbitalpos;
	int flag;
	int fetype;
	int scan;
	struct sat *prev;
	struct sat *next;
};

struct service
{
	int type;
	char* channellist;
	struct dvbdev *fedev;
	struct dvbdev *dmxaudiodev;
	struct dvbdev *dmxvideodev;
	struct dvbdev *dmxpcrdev;
	struct dvbdev *dmxsubtitledev;
	struct dvbdev *audiodev;
	struct dvbdev *videodev;
	struct transponder *transponder;
	struct channel *channel;
	unsigned char *pmtbuf;
	int pmtlen;
	int recdmxstart;
	int recdstfd;
	int recsrcfd;
	time_t recendtime;
	size_t reclastsync;
	off64_t rectotal;
	int reccount;
	char* recname;
	struct service *next;
};

struct caservice
{
	struct service* service;
	struct channel* channel;
	int count;
	int camsockfd;
	struct caslot* caslot;
	int camanager;
};

struct stimerthread
{
	int delay;
	int count;
	int notfirst;
	pthread_attr_t attr;
	pthread_t thread;
	int status;
	int aktion;
	void* param1;
	void* param2;
	void* (*func)(struct stimerthread*, void*, void*);
	struct stimerthread* next;
};

struct hddparm
{
	char* device;
	int read;
	int write;
	int sleeptime;
	int notchanged;
};

struct status
{
	//aktivate extra featers
	int expertmodus;
	//http user:passwd
	char* httpauth;
	//set to 1 if channellist or channelnr open
	int channelswitch;
	//deaktivates child protect if greater then akttime
	time_t protecttime;
	char* boxtype;
	pthread_t mainthread;
	//videosize from stream
	video_size_t videosize;
	time_t videosizevalid;
	//radius for rounded border
	int borderradius;
	int picbordersize;
	int titlelinesize;
	//0 border
	//1 fullbar
	int listboxselecttype;
	//0 camd.socket
	//1 pmt.tmp
	int pmtmode;
	//timer for del old epg entrys
	time_t deloldepg;
	//0 dektiv
	//1 only scan
	//2 only whitlist
	//3 scan and whitelist
	int epglistmode;
	// 1 manual
	// 2 from standby
	int startmode;
	jmp_buf longjumpbuf;
	pthread_mutex_t drawingmutex;
	pthread_mutex_t rectimermutex;
	pthread_mutex_t servicemutex;
	pthread_mutex_t epgmutex;
	pthread_mutex_t channelmutex;
	pthread_mutex_t timerthreadmutex;
	pthread_mutex_t audiotrackmutex;
	pthread_mutex_t subtitlemutex;
	pthread_mutex_t dmxdevmutex;
	pthread_mutex_t rcmutex;
	pthread_mutex_t queuemutex;
	pthread_mutex_t clistmutex;
	pthread_mutex_t hddmutex;
	// mutex for VFD handling
	pthread_mutex_t vfdmutex;
	off64_t recsplitsize;
	char* gateway;
	char* dnsserver1;
	char* dnsserver2;
	int fasttextrender;
	// 1 release all menus
	int menurelease;
	int play;
	int pause;
	int playspeed;
	int random;
	int repeat;
	int epgdays;
	int spinnertime;
	int spinnerspeed;
	int hangtime;
	int timeupdatecount;
	int maxsat;
	int markmodus;
	// 1 from main prog
	// 2 from a thread
	int standby;
	char* tmp;
	// rc filedescriptor
	int fdrc;
	// rc filedescriptor for simulating keypress (fifo)
	int fdrcsim;
	int writeconfig;
	int writeownconfig;
	int writercconfig;
	int writeskinconfig;
	int writechannel;
	int writetransponder;
	int writeprovider;
	int writemainbouquet;
	int writebouquet;
	int writemainplaylist;
	int writeplaylist;
	int writesat;
	int writerectimer;
	int writeepgscanlist;
	// 1 mute is aktiv
	int mute;
	// 1 spinner is aktiv
	int spinner;
	// 1 infobar is aktiv
	// 2 infobar is aktiv (with wait befor show)
	// 3 infobar is aktiv (no autohide)
	int infobar;
	// with infobar to display
	int infobarnr;
	// 0 is for mute screen
	struct skin *drawallways[1];
	char* drawallwaysbg[1];
	int usedirectfb;
	// count how long last rcwait (shows spinner if to long)
	time_t sec;
	// if set to a value != 0, rcwait returns this number
	int rckey;
	struct skin* skinerr;
	// 0 calc and draw
	// 1 calc only without hidden nodes
	// 2 calc only with hidden nodes
	int screencalc;
	// 0 deaktiv
	// 1 struct filelist filled
	// 2 struct filelist and show filesize
	// 3 struct filelist and show filedate
	int filelistextend;
	pthread_attr_t timerthreadattr;
	pthread_t timerthread;
	int timerthreadstatus;
	int timerthreadaktion;
	pthread_attr_t subthreadattr;
	pthread_t subthread;
	int subthreadstatus;
	int subthreadaktion;
	int subthreadpid;
	// epg thread pointer
	struct stimerthread* epgthread;
	// epgscanlist thread pointer
	struct stimerthread* epgscanlistthread;
	// httpd thread pointer
	struct stimerthread* httpthread;
	struct service* aktservice;
	struct service* lastservice;
	struct channel* markedchannel;
	char* configfile;
	// for bouquet channel numbering
	int bouquetnr[MAXSERVICETYPE];
	int servicetype;
	struct shutdowntimer *sd_timer;
	// akt pos in channel cache
	int channelcachepos;
	// aktivate/deaktivate auto updatevfd
	int updatevfd;
	// count for recording
	int recording;
	// count for streaming
	int streaming;
	// timeshift running = 1
	int timeshift;
	// ts playing running = 1
	int playing;
	// if set to a skin (screenpointer) only this screen has rc
	struct skin* rcowner;
	// timerstuktur fuer autores
	struct stimerthread* restimer;
	//spindown USB/HDD
	struct hddparm hdd[3];
	struct stimerthread* addhddall;

} status;

struct fb
{
	int dev;
	char *name;
	int fd;
	unsigned char *fb;
	unsigned long *fblong;
	int width;
	int height;
	int pitch;
	unsigned long fixfbsize;
	unsigned long varfbsize;
	int colbytes;
	struct fb *next;
};

struct rc
{
	int key;
	void (*rcfunc)(struct skin*, struct skin*, int flag);
	struct skin *screen;
	struct skin *screennode;
	struct skin *owner;
	struct rc *next;
};

struct clist
{
	char *key;
	char *value;
	char *def;
	char *tmp;
	struct clist *next;
};

struct bouquet
{
	int nr;
	int serviceid;
	unsigned long transponderid;
	struct channel *channel;
	struct bouquet *prev;
	struct bouquet *next;
};

struct mainbouquet
{
	char *name;
	int type;
	char *filename;
	struct bouquet *bouquet;
	struct mainbouquet *prev;
	struct mainbouquet *next;
};

struct pic
{
	char* name;
	unsigned char* picbuf;
	// < 0 when picbuf is normal mem (hw jpg)
	int memfd;
	unsigned long width;
	unsigned long height;
	unsigned long rowbytes;
	int channels;
	int del;
	struct pic* next;
};

struct font
{
	char *name;
	FT_Error error;
	FTC_Manager manager;
	FTC_SBitCache cache;
	FTC_ImageTypeRec desc;
	FT_Face face;
	FT_UInt prev_glyphindex;
	FT_Bool use_kerning;
	struct font *next;
};

#define HILO(x) (x##_hi << 8 | x##_lo)

/*
#define HILO2(x) (x##1 << 8 | x##2)
#define HILO3(x) (x##1 << 16 | x##2 << 8 | x##3)
#define HILO4(x) (x##4 << 24 | x##2 << 16 | x##3 << 8 | x##4)

#define MjdToEpochTime(x) ((HILO(x)-40587)*86400)
#define BcdTimeToSeconds(x) ((3600 * ((10*((x##_h & 0xF0)>>4)) + (x##_h & 0xF))) + \
                             (60 * ((10*((x##_m & 0xF0)>>4)) + (x##_m & 0xF))) + \
                             ((10*((x##_s & 0xF0)>>4)) + (x##_s & 0xF)))
#define BcdTimeToMinutes(x) ((60 * ((10*((x##_h & 0xF0)>>4)) + (x##_h & 0xF))) + \
                             (((10*((x##_m & 0xF0)>>4)) + (x##_m & 0xF))))
#define BcdCharToInt(x) (10*((x & 0xF0)>>4) + (x & 0xF))
#define CheckBcdChar(x) ((((x & 0xF0)>>4) <= 9) && \
                         ((x & 0x0F) <= 9))
#define CheckBcdSignedChar(x) ((((x & 0xF0)>>4) >= 0) && (((x & 0xF0)>>4) <= 9) && \
                         ((x & 0x0F) >= 0) && ((x & 0x0F) <= 9))

#define GetSectionLength(x) HILO(((si_tab_t *)(x))->section_length)
#define GetServiceId(x) HILO(((eit_t *)(x))->service_id)
#define GetLastTableId(x) ((eit_t *)(x))->segment_last_table_id
#define GetSegmentLastSectionNumber(x) ((eit_t *)(x))->segment_last_section_number
#define GetPrivateDataSpecifier(x) HILO4(((descr_private_data_specifier_t *)x)->private_data_specifier)
*/

struct eit
{
	u_char table_id                               /*:8*/;
	u_char section_length_hi                      :4;
	u_char                                        :3;
	u_char section_syntax_indicator               :1;
	u_char section_length_lo                      /*:8*/;
	u_char service_id_hi                          /*:8*/;
	u_char service_id_lo                          /*:8*/;
	u_char current_next_indicator                 :1;
	u_char version_number                         :5;
	u_char                                        :2;
	u_char section_number                         /*:8*/;
	u_char last_section_number                    /*:8*/;
	u_char transport_stream_id_hi                 /*:8*/;
	u_char transport_stream_id_lo                 /*:8*/;
	u_char original_network_id_hi                 /*:8*/;
	u_char original_network_id_lo                 /*:8*/;
	u_char segment_last_section_number            /*:8*/;
	u_char segment_last_table_id                  /*:8*/;
	u_char data[]; /* struct eitevent */
};
#define EITLEN sizeof(struct eit)
#define GETEITSECLEN(x) HILO(((struct eit*)(x))->section_length)

struct eitevent
{
	u_char event_id_hi                            /*:8*/;
	u_char event_id_lo                            /*:8*/;
	u_char mjd_hi                                 /*:8*/;
	u_char mjd_lo                                 /*:8*/;
	u_char start_time_h                           /*:8*/;
	u_char start_time_m                           /*:8*/;
	u_char start_time_s                           /*:8*/;
	u_char duration_h                             /*:8*/;
	u_char duration_m                             /*:8*/;
	u_char duration_s                             /*:8*/;
	u_char descriptors_loop_length_hi             :4;
	u_char free_ca_mode                           :1;
	u_char running_status                         :3;
	u_char descriptors_loop_length_lo             /*:8*/;
	u_char data[];
};
#define EITEVENTLEN sizeof(struct eitevent)
#define GETEITDESCLEN(x) HILO(((struct eitevent *)x)->descriptors_loop_length)

struct eitshortevent
{
	u_char descriptor_tag                         /*:8*/;
	u_char descriptor_length                      /*:8*/;
	u_char lang_code1                             /*:8*/;
	u_char lang_code2                             /*:8*/;
	u_char lang_code3                             /*:8*/;
	u_char event_name_length                      /*:8*/;
	u_char data[];
};
#define EITSHORTEVENTLEN sizeof(struct eitshortevent)

struct eitlongevent
{
	u_char descriptor_tag                         /*:8*/;
	u_char descriptor_length                      /*:8*/;
	u_char last_descriptor_number                 :4;
	u_char descriptor_number                      :4;
	u_char lang_code1                             /*:8*/;
	u_char lang_code2                             /*:8*/;
	u_char lang_code3                             /*:8*/;
	u_char length_of_items                        /*:8*/;
	u_char data[]; /* struct eitlongeventitem */
};
#define EITLONGEVENTLEN sizeof(struct eitlongevent)

struct eitlongeventitem
{
	u_char item_description_length               /*:8*/;
	u_char data[];
};
#define EITLONGEVENTITEMLEN sizeof(struct eitlongeventitem)

struct eitlinkage
{
        u_char descriptor_tag                 /*:8*/;
        u_char descriptor_length              /*:8*/;
        u_char transport_stream_id_hi         /*:8*/;
        u_char transport_stream_id_lo         /*:8*/;
        u_char original_network_id_hi         /*:8*/;
        u_char original_network_id_lo         /*:8*/;
        u_char service_id_hi                  /*:8*/;
        u_char service_id_lo                  /*:8*/;
        u_char linkage_type                   /*:8*/;
};
#define EITLLINKAGELEN sizeof(struct eitlinkage)

struct parentalrating
{
	u_char descriptor_tag                 /*:8*/;
	u_char descriptor_length              /*:8*/;
	u_char data[]; //struct parental_rating_item
};
#define PARENTALRATINGLEN sizeof(struct parentalrating)

struct parentalratingitem
{ 
   u_char lang_code1                          /*:8*/;
   u_char lang_code2                          /*:8*/;
   u_char lang_code3                          /*:8*/;
   u_char rating                              /*:8*/;
};
#define PARENTALRATINGITEMLEN sizeof(struct parentalratingitem)

#endif
