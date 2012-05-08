/*****************************************************/
/* this file is part of the tiTan / tiTanNIT Project */
/* and allowed only for use with this.               */
/*                                                   */
/* copyright by NIT                                  */
/*****************************************************/

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
#include <png.h>
#include <jpeglib.h>
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
#include <sys/utsname.h>

#define IPKG_LIB
#include "libipkg/config.h"
#include "libipkg/libipkg.h"
#include "libipkg/args.h"
#include "libipkg/ipkg_cmd.h"

//for freetype
#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_CACHE_H

//for eplayer
#ifdef EPLAYER3
#include <common.h>
#include <subtitle.h>
#endif

#ifdef EPLAYER4
#include <gst/gst.h>
#include <gst/pbutils/missing-plugins.h>
#endif

#ifdef DVDPLAYER
#include <ddvdlib.h>
#endif

#define OVERSION "1.13"
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
#define CHANNELCACHEMAX 1000
#define TRANSPONDERCACHEMAX 500
#define MAXHTTPDCONN 20
#define MAXHTMLLINE 300
#define MAXRGUIDCONN 1
#define LISTHASHSIZE 27
#define MAXCASESSION 16
#define MAXCASERVICE 20
#define MAXLONGKEY 10
#define MAXSAT 64
#define EPGSCANLOG "/tmp/epgscan.log"
#define MAXCHANNELHISTORY 6
#define HILO(x) (x##_hi << 8 | x##_lo)
#define HTTPAUTH "aXBrLUdaRmg6RkhaVkJHaG56ZnZFaEZERlRHenVpZjU2NzZ6aGpHVFVHQk5Iam0="

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

#define OMIN(a,b) (a < b ? a : b)
#define OMAX(a,b) (a > b ? a : b)

#define bcdtoint(i) ((((i & 0xf0) >> 4) * 10) + (i & 0x0f))

#define alpha_composite(composite, fg, alpha, bg) { \
unsigned short temp = ((unsigned short)(fg)*(unsigned short)(alpha) +   \
(unsigned short)(bg)*(unsigned short)(255 - (unsigned short)(alpha)) +  \
(unsigned short)128);  \
(composite) = (unsigned char)((temp + (temp >> 8)) >> 8); }

enum {LEFT=20000, CENTER, RIGHT, TEXTCENTER, TEXTRIGHT};
enum {TOP=20000, MIDDLE, BOTTOM, TEXTMIDDLE, TEXTBOTTOM};
enum {BOTTOMLEFT, BOTTOMRIGHT, TOPLEFT, TOPRIGHT};
enum {LEFTRIGHT=1, TOPBOTTOM, LEFTMIDDLE, TOPMIDDLE};
enum {NO, YES, AUTONO, AUTOYES};
enum {UNDEF=0, LISTBOX=1, TEXTBOX=2, CHOICEBOX=4, INPUTBOX=8, INPUTBOXNUM=16, FILELIST=32, PROGRESSBAR=64, MENU=128, MULTIPROGRESSBAR=256, GRID=512, GRIDBR=1024};
enum {FRONTENDDEV, DEMUXDEV, VIDEODEV, AUDIODEV, CADEV, CIDEV, FRONTENDDEVDUMMY, DVRDEV};
enum {TV, RADIO};
enum {RECDIRECT, RECTIMER, RECTIMESHIFT, RECSTREAM, RECPLAY};
enum {AC3 = 0, MPEGA = 1, DTS = 2, LPCM = 6, AAC = 8, AACHE = 9};
enum {MPEGV = 0, MPEG4V = 4, H264 = 1, VC1 = 10};
enum {ALLCHANNEL, SATCHANNEL, PROVIDERCHANNEL, AZCHANNEL, BOUQUETCHANNEL, SATLIST, PROVIDERLIST, AZLIST, MAINBOUQUETLIST};
enum {NOMODE, MVMODE, RMMODE, CPMODE, PROTECTMODE, EDITMODE};
enum {NOTHING, CHANNEL, STILLPIC, RECORDDIRECT, RECORDTIMER, RECORDTIMESHIFT, RECORDSTREAM, TMPOTHER, RECORDPLAY};
enum {START, PAUSE, STOP};
enum {DEACTIVE, ACTIVE, INPAUSE, ERROR};
enum {FUNCTEXT, FUNCPIC, FUNCPROGRESS};
enum {CASESSIONCREATE, CASESSIONBUSY, CASESSIONDEL, CASESSIONSTART, CASESSIONFINAL, CARESFIRSTENQUIRY, CARESCHANGE, CARESENQUIRY, CADATETIMESEND, CAMMIIDLE, CAMMIDISPLAYREPLAY, CAMMIFAKEOK};

struct menulist
{
	char* name;
	char* text;
	char* pic;
	char* param;
	char* param1;
	int deaktiv;
	int defaultentry;
	struct menulist* next;
};

struct channelhistory
{
	struct channel* chnode;
	char* channellist;
};

struct queue
{
	//0-99 for ca module
	//101 for thumb thread
	int type;
	int flag;
	int len;
	void* data;
	void* data1;
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
	int networkscan;
	int clear;
	int blindscan;
	char* satname;
	int tpcount;
	int tpmax;
	int tvcount;
	int newtvcount;
	int radiocount;
	int newradiocount;
	int datacount;
	int newdatacount;
	int blindcount;
	int newblindcount;
	int blindmax;
} scaninfo;

struct screensaver
{
	int type;
	int speed;
	int newposx;
	int newposy;
	int flag;
	struct skin* screen;
	struct skin* screennode;
	struct skin* filelist;
	struct skin* aktnode;
	struct pic* pic;
	char* value;
	char* path;
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
	char* auth;
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
	char* timestamp;
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
	char* device;
	char* ip;
	char* netmask;
	char* mac;
	char* broadcast;
	int dhcp;
	int found;
	//flag 1 = used in titan
	int flag;
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

struct transpondercache
{
	unsigned long transponderid;
	struct transponder* tpnode;
	struct transpondercache* next;
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
	unsigned int type;
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
	char* selectpic;
	char* pic;
	int16_t picwidth;
	int16_t picheight;
	uint16_t hspace;
	uint16_t vspace;
	char* (*skinfunc)(struct skin*, void*, void*);
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
	uint8_t locked;
	//bit 0: 1 = node is drawed
	uint8_t flag;
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
	unsigned int feloffrequency;
	int feaktband;
	int feaktpolarization;
	fe_sec_voltage_t feaktvolt;
	fe_sec_tone_mode_t feakttone;
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
	unsigned int symbolrate;
	int encoding;
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
};

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
	int tssize;
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
	//bit 0: 1 = stop allwas on titan end
	int flag;
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
	int longkeycode[MAXLONGKEY];
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
	pthread_mutex_t linkedchannelmutex;
	pthread_mutex_t dmxdevmutex;
	pthread_mutex_t rcmutex;
	pthread_mutex_t queuemutex;
	pthread_mutex_t clistmutex;
	pthread_mutex_t hddmutex;
	pthread_mutex_t tsseekmutex;
	pthread_mutex_t accelfbmutex;
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
	// rguid thread pointer
	struct stimerthread* rguithread;
	// rguid sock filedescriptor for rguid
	int rguidfd;
	struct service* aktservice;
	struct service* lastservice;
	struct channel* markedchannel;
	//if we are in a epg screen, this is the showen channel
	struct channel* epgchannel;
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
	// timeshift seeking <> 0
	int timeshiftseek;
	// if set to a skin (screenpointer) only this screen has rc
	struct skin* rcowner;
	// timerstuktur fuer autores
	struct stimerthread* restimer;
	// spindown USB/HDD
	struct hddparm hdd[3];
	struct stimerthread* addhddall;
	// save old rotor position
	int rotoroldorbitalpos;
	// security check
	int security;
	// set to 1 if tvpic size is changed
	int tvpic;
	// timestamp for rc action
	time_t lastrcaction;
	// show timeline in channellist for each channel
	int showchanneltimeline;
	// see player.h for details
	int playercan;
		//Code von infobar.h aktiv = 1
	int infobaraktiv;
	//Name der Datei die abgespielt wird
	char* playfile;
	//Code MC aktiv = 1
	int mcaktiv;
	//greater 0 aktivate screen animation
	int screenanim;
	int screenanimspeed;
	//for auto increase audio vol
	int volautochange;
	int volautochangevalue;
	//for powerofftimer
	time_t fixpowerofftime;
	//for write FB in File
	int write_png;
	int write_jpg;
	//holds oversize value for blitter
	int leftoffset;
	int rightoffset;
	int topoffset;
	int bottomoffset;
	//background picture for all screens
	char* bgpic;
	//should thumb thread start
	int createthumb;
	//time for next picmem timeout check
	time_t picmemtimeout;
	//show not tunable channel hidden (0)) or deaktive (1) in channellist
	int channellistview;
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
  int timeout;
  time_t lastaccess;
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

//structure for write fb to File
struct writeFBfile
{
	unsigned char* ActBuf;
	unsigned char* buf1;
	unsigned char* buf2;
} writeFBfile;

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
	u_char lang_code1                    /*:8*/;
	u_char lang_code2                    /*:8*/;
	u_char lang_code3                    /*:8*/;
	u_char rating                        /*:8*/;
};
#define PARENTALRATINGITEMLEN sizeof(struct parentalratingitem)

struct rst //table id 0x71
{ 
	u_char table_id                     /*:8*/;
	u_char section_syntax_indicator         :1;
	u_char reserved_1                       :1;
	u_char reserved_2                       :2;
	u_int section_length                   :12;
};
#define RSTLEN sizeof(struct rst)

struct rstitem
{ 
	u_char transponder_stream_id_hi     /*:8*/;
	u_char transponder_stream_id_lo     /*:8*/;
	u_char original_network_id_hi       /*:8*/;
	u_char original_network_id_lo       /*:8*/;
	u_char service_id_hi                /*:8*/;
	u_char service_id_lo                /*:8*/;
	u_char event_id_hi                  /*:8*/;
	u_char event_id_lo                  /*:8*/;
	u_char reserved_1                       :5;
	u_char running_status                   :3;
};
#define RSTITEMLEN sizeof(struct rstitem)

#endif
