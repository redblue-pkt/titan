/*****************************************************/
/* this file is part of the tiTan / tiTanNIT Project */
/* and allowed only for use with this.               */
/*                                                   */
/* copyright by NIT                                  */
/*****************************************************/

#ifndef STRUCT_H
#define STRUCT_H

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

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
#include <sys/sysmacros.h>
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
#include <regex.h>
#include <mntent.h>
#include <termios.h>
//#ifndef SH4
#include <sys/stat.h>
//#endif
#include <openssl/aes.h>
#include <openssl/dh.h>
#include <openssl/rsa.h>
#include <openssl/sha.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <stdint.h>
#include <inttypes.h>
#include <arpa/inet.h>
#include <time.h>
#include <linux/dvb/ca.h>
#include <sys/types.h>

#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>

#include <openssl/pem.h>
#include <openssl/x509.h>
#include <openssl/x509v3.h>
#include <openssl/sha.h>
#include <openssl/aes.h>

//for freetype
#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_CACHE_H

//for eplayer
#ifdef EPLAYER3
#include <common.h>
//#ifndef BETA
//#include <subtitle.h>
//#endif
#endif

#ifdef EPLAYER4
#include <gst/gst.h>
#include <gst/pbutils/missing-plugins.h>
#endif

#ifdef DVDPLAYER
#include <ddvdlib.h>
#endif

#define DRIVER ""
#define OVERSION "1.99"
#define PROGNAME "titan"
#define COPYRIGHT "NIT"
#define CRONTRIBUT "obi, gost"
#define PLUGINVERSION 1

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
#define MAXDVBADAPTER 1
#ifndef MIPSEL
#define MAXFRONTENDDEV 4
#else
#define MAXFRONTENDDEV 10
#endif
#define MAXDEMUXDEVOPEN 10
#define MAXAUDIODEV 4
#define MAXVIDEODEV 4
#define MAXCIDEV 4
#define MAXCADEV 8
#ifndef MIPSEL
#define MAXDVRDEV 4
#define MAXDEMUXDEV 4
#else
#define MAXDVRDEV 9
#define MAXDEMUXDEV 9
#define MAXENCODERDEV 2
//#define DVB_API_VERSION 5
#endif	 
#define MAXSCDEV 2
#define RCTIMEOUT 999999
#define RCTHREADSTANDBY 999998
#define MAXSERVICETYPE 10
#define CHANNELCACHEMAX 1000
#define MEDIADBCACHEMAX 1000
#define TRANSPONDERCACHEMAX 500
#define MAXHTTPDCONN 20
#define MAXHTMLLINE 300
#define MAXRGUIDCONN 1
#define LISTHASHSIZE 27
#define MAXCASESSION 16
#define MAXCASERVICE 20
#define MAXLONGKEY 10
#define MAXSAT 64
#define MAXBGDOWNLOAD 10
#ifdef MIPSEL
#define EPGSCANLOG "/mnt/logs/epgscan.log"
#else
#define EPGSCANLOG "/tmp/epgscan.log"
#endif
#define MAXCHANNELHISTORY 6
#define HILO(x) (x##_hi << 8 | x##_lo)
#define HILO32(x) (x##_hi << 24 | x##_mh << 16 | x##_ml << 8 | x##_lo)
#define SERIALDEV "/dev/ttyAS0"
#define MAXTOKENS 256
#define MAXSTACKTRACE 30
#define MAXSTACKTRACEFUNC 20

#define USBDEVICE_SUPER_MAGIC 0x9fa2
#define EXT2_SUPER_MAGIC 0xEF53
#define EXT3_SUPER_MAGIC 0xEF53
#define SMB_SUPER_MAGIC 0x517B
#define NFS_SUPER_MAGIC 0x6969
#define MSDOS_SUPER_MAGIC 0x4d44

#ifndef VIDEO_SOURCE_HDMI
#define VIDEO_SOURCE_HDMI 2
#endif
#ifndef AUDIO_SOURCE_HDMI
#define AUDIO_SOURCE_HDMI 2
#endif

//cacc cert check
#define ROOT_CERT     "/mnt/swapextensions/etc/pem/root.pem"
#define CUSTOMER_CERT "/mnt/swapextensions/etc/pem/customer.pem"
#define DEVICE_CERT   "/mnt/swapextensions/etc/pem/device.pem"
#define MAX_ELEMENTS    33
#ifdef MIPSEL
#define CA_SET_DESCR_DATA_DREAM _IOW('o', 137, struct ca_descr_data)
#define CA_SET_DESCR_DATA _IOW('o', 10, struct ca_descr_data)
#endif
#ifdef SH4NEW
#define CA_SET_DESCR_DATA _IOW('o', 137, struct ca_descr_data)
#endif
#ifdef SIMULATE
#define CA_SET_DESCR_DATA _IOW('o', 137, struct ca_descr_data)
#endif

//Start Function Entry Point
#define STARTFUNC

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

//Alarm Signal used as timeout
#define ALARMTIMEOUT 1
#define ALARMSTARTTIME(x) if(ALARMTIMEOUT == 1) {alarm(x);}
#define ALARMSTOPTIME if(ALARMTIMEOUT == 1) {alarm(0);}

// VFD icons supported (more or less) on all boxes:
typedef enum { VFD_USB = 0x10, VFD_HD, VFD_HDD, VFD_LOCK, VFD_BT, VFD_MP3, VFD_MUSIC, VFD_DD, VFD_MAIL, VFD_MUTE, VFD_PLAY, VFD_PAUSE, VFD_FF, VFD_FR, VFD_REC, VFD_CLOCK } vfdicons;

#define OMIN(a,b) (a < b ? a : b)
#define OMAX(a,b) (a > b ? a : b)

#define bcdtoint(i) ((((i & 0xf0) >> 4) * 10) + (i & 0x0f))

#define alpha_composite(composite, fg, alpha, ralpha, bg) { (composite) = ((fg)*(alpha) + (bg)*(ralpha)) >> 8; }

enum {LEFT=20000, CENTER, RIGHT, TEXTCENTER, TEXTRIGHT};
enum {TOP=20000, MIDDLE, BOTTOM, TEXTMIDDLE, TEXTBOTTOM};
enum {BOTTOMLEFT, BOTTOMRIGHT, TOPLEFT, TOPRIGHT};
enum {LEFTRIGHT=1, TOPBOTTOM, LEFTMIDDLE, TOPMIDDLE};
enum {NO, YES, AUTONO, AUTOYES};
enum {UNDEF=0, LISTBOX=1, TEXTBOX=2, CHOICEBOX=4, INPUTBOX=8, INPUTBOXNUM=16, FILELIST=32, PROGRESSBAR=64, MENU=128, MULTIPROGRESSBAR=256, GRID=512, GRIDBR=1024, PASSWORD=2048};
enum {FRONTENDDEV, DEMUXDEV, VIDEODEV, AUDIODEV, CADEV, CIDEV, FRONTENDDEVDUMMY, DVRDEV, SCDEV, ENCODERDEV};
enum {TV, RADIO};
enum {RECDIRECT, RECTIMER, RECTIMESHIFT, RECSTREAM, RECPLAY, RECSTREAMENC};
#ifdef DREAMBOX
enum {MPEGV = 0, MPEG4V = 4, H264 = 1, VC1 = 10, H265 = 22};
enum {AC3 = 0, MPEGA = 1, DTS = 2, LPCM = 6, AAC = 8, AACHE = 9, DTSHD = 16, DDP = 7};
#else
enum {AC3 = 0, MPEGA = 1, DTS = 2, LPCM = 6, AAC = 8, AACHE = 9, DTSHD = 16, DDP = 34};
enum {MPEGV = 0, MPEG4V = 4, H264 = 1, VC1 = 10, H265 = 7};
#endif
enum {ALLCHANNEL, SATCHANNEL, PROVIDERCHANNEL, AZCHANNEL, BOUQUETCHANNEL, SATLIST, PROVIDERLIST, AZLIST, MAINBOUQUETLIST};
enum {NOMODE, MVMODE, RMMODE, CPMODE, PROTECTMODE, EDITMODE, STREAMMODE};
enum {NOTHING, CHANNEL, STILLPIC, RECORDDIRECT, RECORDTIMER, RECORDTIMESHIFT, RECORDSTREAM, TMPOTHER, RECORDPLAY, HDMIIN};
enum {START, PAUSE, STOP};
enum {DEACTIVE, ACTIVE, INPAUSE, ERROR};
enum {FUNCTEXT, FUNCPIC, FUNCPROGRESS,FUNCPICOLED};
enum {CASESSIONCREATE, CASESSIONBUSY, CASESSIONDEL, CASESSIONSTART, CASESSIONFINAL, CARESFIRSTENQUIRY, CARESCHANGE, CARESENQUIRY, CADATETIMESEND, CAMMIIDLE, CAMMIDISPLAYREPLAY, CAMMIFAKEOK};
enum {feSatellite, feCable, feTerrestrial, feATSC};

#ifdef BETA
struct stacktrace
{
	int pos;
	pthread_t thread;
	void* func[MAXSTACKTRACEFUNC];
};
struct stacktrace stacktrace[MAXSTACKTRACE];
#endif

struct lastsubtitle
{
	uint64_t transponderid;
	int serviceid;
	int16_t subtitlepid;
	int16_t subtitleid1;
	int16_t subtitleid2;
	struct lastsubtitle* next;
};

struct newsletter
{
	unsigned long nr;
	char* date;
	char* title;
	char* text;
	struct newsletter* next;
};

struct oldentry
{
	void* entry;
	//0 = epg
	int type;
	time_t del;
	struct oldentry* next;
};

struct unicable
{
	char* manufacturer;
	char* product;
	int scr1;
	int scr2;
	int scr3;
	int scr4;
	int scr5;
	int scr6;
	int scr7;
	int scr8;
	int scr9;
	int scr10;
	int scr11;
	int scr12;
	int scr13;
	int scr14;
	int scr15;
	int scr16;
	int scr17;
	int scr18;
	int scr19;
	int scr20;
	int scr21;
	int scr22;
	int scr23;
	int scr24;
	int scr25;
	int scr26;
	int scr27;
	int scr28;
	int scr29;
	int scr30;
	int scr31;
	int scr32;
	int lofl;
	int lofh;
	struct unicable* next;
	struct unicable* prev;
};

struct id3tag
{
	int version;
	char* title;
	char* artist;
	char* album;
	char* year;
	char* comment;
	char* genrecode;
	char* genretext;
	char* poster;
	int track;
	int len;
	int picturetype;
	unsigned int picturepos;
	unsigned int picturelen;
};

struct id3genre
{
	int code;
	char text[30];
};

struct regex
{
	char* match1;
	char* match2;
	char* match3;
	char* match4;
	char* match5;
	char* match6;
	char* match7;
	char* match8;
	char* match9;
	char* match10;
};

struct imdb
{
	char* title;
	char* year;
	char* rated;
	char* released;
	char* genre;
	char* director;
	char* writer;
	char* actors;
	char* plot;
	char* poster;
	char* runtime;
	char* rating;
	char* votes;
	char* id;
	char* thumb;
};

struct imdbapi
{
	char* title;
	char* year;
	char* rated;
	char* released;
	char* genre;
	char* director;
	char* writer;
	char* actors;
	char* plot;
	char* poster;
	char* runtime;
	char* rating;
	char* votes;
	char* id;
};

struct tmdb
{
	char* title;
	char* year;
	char* rated;
	char* released;
	char* genre;
	char* runtime;
	char* plot;
	char* postermid;
	char* rating;
	char* votes;
	char* id;
	char* backdrop;
	char* imdbid;
	char* orgname;
	char* language;
	char* type;
	char* score;
	char* cover;
	char* thumb;
	char* mvi;
	struct tmdb* prev;
	struct tmdb* next;
};

struct mediadbfilter
{
	struct mediadb* node;
	int count;
	struct mediadbfilter* prev;
	struct mediadbfilter* next;
};

struct mediadbcategory
{
	int type;
	char* name;
	struct mediadbcategory* prev;
	struct mediadbcategory* next;
};

struct mediadb
{
	char* id;
	int type;
	char* title;
	int year;
	char* released;
	char* runtime;
	char* genre;
	char* director;
	char* writer;
	char* actors;
	char* plot;
	char* poster;
	int rating;
	int votes;
	char* path;
	char* file;
	char* shortname;
	char* fileinfo;
	time_t timestamp;
	//bit 31: manual change
	int flag;
	int backdropcount;
	struct mediadb* prev;
	struct mediadb* next;
};

struct mediadbcache
{
	char* path;
	char* file;
	struct mediadb* mediadbnode;
	struct mediadbcache* next;
};

struct mostzap
{
	int serviceid;
	uint64_t transponderid;
	int count;
	struct mostzap* prev;
	struct mostzap* next;
};

struct menulist
{
	char* name;
	char* text;
	char* pic;
	char* param;
	char* param1;
	char* param2;
	char* param3;
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
	int ccmanager;
#ifdef MIPSEL
	int hostmanager;
	int mmiappmanager;
	int hostlacmanager;
	int upgrademanager;
#endif
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
	// cacc
	unsigned char lastKey[32];
	uint8_t lastParity;
//	unsigned char scrambled;
	int scrambled;
	void *private_data;
	int ccmgr_ready;
	// not in used currently
	int inuse;
	int sidblacklisted;
};

struct hdd
{
	char* device;
	int partition;
	unsigned long long size;
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
	int tpnew;
	int tpdel;
	int tvcount;
	int newtvcount;
	int radiocount;
	int newradiocount;
	int datacount;
	int newdatacount;
	int blindcount;
	int newblindcount;
	int changename;
	int cleartransponder;
	unsigned int blindmax;
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
	char* link;
	char* cmd;
	char* host;
	char* page;
	int port;
	char* filename;
	char* auth;
	char* user;
	char* pass;
	int connfd;
	int proz;
	unsigned int maxkb;
	unsigned int aktkb;
	int ret;
	int timeout;
	int flag;
};

struct copyfile
{
	char* from;
	char* to;
	int proz;
	int filecount;
	int maxfilecount;
	off64_t maxkb;
	off64_t aktkb;
	int stop;
	int ret;
	int flag;
};

struct epgscanlist
{
	int serviceid;
	uint64_t transponderid;
	time_t scantime;
	struct epgscanlist* prev;
	struct epgscanlist* next;
	char* name;
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
	//4 = epgscan timer activ
	//5 = epgscan timer not activ
	int status;
	char* errstr;
	struct service* servicenode;
	uint64_t transponderid;
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
	//0 = static
	//1 = dhcp
	//2 = off
	int type;
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
	uint64_t transponderid;
	struct channel* chnode;
	struct channelcache* next;
};

struct transpondercache
{
	uint64_t transponderid;
	struct transponder* tpnode;
	struct transpondercache* next;
};

struct filelist
{
	int type;
	int view;
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
	uint8_t picquality;

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
	uint8_t picmem;
	//bit 0: 1 = node is drawed
	//bit 1: 1 = node is changed
	uint8_t flag;
	int hash;
	struct skin *select;
	struct skin *child;
	struct skin *prev;
	struct skin *next;
	struct skin *drawnext;
	// usesavebg=0 disable 
	// usesavebg=1 save always
	// usesavebg=2 save one times then free
	// usesavebg=3 dummy for filelist 
	int usesavebg;
	// savebg is saved date
	char* savebg;
	// fuer blinken
	int nodestyle;
};

struct style
{
	char* name;
	int16_t posx;
	int16_t posy;
	int16_t width;
	int16_t height;
	int16_t picwidth;
	int16_t picheight;
	uint16_t textposx;
	uint16_t textposx2;
	uint16_t halign;
	uint16_t valign;
	uint16_t hspace;
	uint16_t vspace;
	uint16_t bgspace;
	int16_t borderradius;
	int16_t titlealign;
	int16_t zorder;
	int16_t gradient;
	int16_t titlegradient;
	uint8_t picquality;
	uint8_t hidden;
	uint8_t wrap;
	uint8_t scrollbar;
	uint8_t bordersize;
	int8_t bordertype;
	uint8_t shadowsize;
	uint8_t shadowpos;
	uint8_t fontsize;
	uint8_t fontsize2;
	int8_t charspace;
	uint8_t transparent;
	uint8_t prozposx;
	uint8_t prozposy;
	uint8_t prozwidth;
	uint8_t prozheight;
	uint8_t picprozwidth;
	uint8_t picprozheight;
	uint8_t funcrettype;
	uint8_t picmem;
	long bordercol;
	long deaktivcol;
	long progresscol;
	long shadowcol;
	long fontcol;
	long fontcol2;
	long titlebgcol;
	long titlebgcol2;
	long bgcol;
	long bgcol2;
	char* font;
	char* pic;
	char* param1;
	char* param2;
	char* input;
	char* mask;
	char* (*skinfunc)(struct skin*, void*, void*);
	struct style* next;
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
	uint8_t deactive;
	uint8_t decoder;
	struct transponder* felasttransponder;
	struct transponder* feakttransponder;
	int feunicable;
	unsigned int feloffrequency;
	unsigned int feloffrequency_uni;
	int feaktband;
	int feaktpolarization;
	fe_sec_voltage_t feaktvolt;
	fe_sec_tone_mode_t feakttone;
	int fedmxsource;
	char* feaktnr;
	char* feshortname;
	char* fehyprid;
	struct dvb_frontend_info* feinfo;
	struct caslot* caslot;
	struct dvbdev *next;
};

//there are a copytransponder and createtransponder function
//if you change this struct you must change the function
struct transponder
{
	uint64_t id;
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
	time_t lastepg;
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
	int16_t subtype;
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
	int pid;
	int len;
	int systemid;
	int reserved;
	int capid;
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
	int16_t rdspid;
	struct audiotrack* next;
};

struct linkedchannel
{
	uint64_t transponderid;
	int serviceid;
	time_t starttime;
	time_t endtime;
	struct linkedchannel* next;
};

//there are a createchannel function
//if you change this struct you must change the function
struct channel
{
	char* name;
	uint64_t transponderid;
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
	int16_t aitpid;
	uint8_t protect;
	char* hbbtvurl;
	char* streamurl;
	char* epgurl;
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
	struct dvbdev *encoderdev;
	struct transponder *transponder;
	struct channel *channel;
	unsigned char *pmtbuf;
	int pmtlen;
	int recdmxstart;
	int recdstfd;
	int recsrcfd;
	time_t recendtime;
	char* rectimestamp;
	size_t reclastsync;
	off64_t rectotal;
	int reccount;
	int tssize;
	char* recname;
	unsigned long long lenpts;
	unsigned long long startpts;
	unsigned long long endpts;
	unsigned long long bitrate;
	off64_t endoffile;
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
	char* capmt;
	int capmtlen;
	int cmdpos;
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

struct marker
{
	off64_t pos;
	off64_t time;
	char* timetext;
	struct marker* prev;
	struct marker* next;
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
	char* realboxtype;
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
	//write epg periodic to medium
	time_t writeperiodicepg;
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
	pthread_mutex_t mediadbmutex;
	pthread_mutex_t oldentrymutex;
	pthread_mutex_t newslettermutex;
	pthread_mutex_t tithekmutex;
	pthread_mutex_t inetworkmutex;
	pthread_mutex_t textboxmutex;
	pthread_mutex_t setaktresmutex;
	pthread_mutex_t waitrcmutex;
	// mutex for VFD handling
	pthread_mutex_t vfdmutex;
	// mutex for OLED handling
	pthread_mutex_t oledmutex;
		// mutex for cec handling
	pthread_mutex_t cecmutex;
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
	int slowspeed;
	int prefillbuffer;
	int playpic;
	int singlepicfd;
	int singlepicthreadstatus;
#ifdef EPLAYER4
	int prefillbuffercount;
	int bufferpercent;
	int cleaninfobar;
//	gint avgInRate;
//	gint avgOutRate;
//	gint64 bufferingLeft;
	gint64 pts;
#endif
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
	// rc filedescriptor for tuxtxt
	int fdrctxt;
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
	int writemostzap;
	int writemediadb;
	int writelastsubtitle;
	int writechannelslot;
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
	pthread_attr_t timerthreadattr;
	pthread_t timerthread;
	int timerthreadstatus;
	int timerthreadaktion;
	pthread_attr_t subthreadattr;
	pthread_t subthread;
	int subthreadstatus;
	int subthreadaktion;
	int subthreadpid;
	int subthreadid1;
	int subthreadid2;
	// epg thread pointer
	struct stimerthread* epgthread;
	// epgscanlist thread pointer
	struct stimerthread* epgscanlistthread;
	// httpd thread pointer
	struct stimerthread* httpthread;
	// rguid thread pointer
	struct stimerthread* rguithread;
	// mediadb thread pointer
	struct stimerthread* mediadbthread;
	// newsletter thread pointer
	struct stimerthread* newsletterthread;
	// thumb thread pointer
	struct stimerthread* thumbthread;
	// rectimer thread
	struct stimerthread* rectimerthread;
	// singlepic thread
	struct stimerthread* singlepicthread;
    // service thread
    struct stimerthread* startservicethread;
	// rguid sock filedescriptor for rguid
	int rguidfd;
	struct service* aktservice;
	struct service* lastservice;
	struct service* pipservice;
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
	// pvr allowed = 1, disable = 0
	int pvr;
	// timeshift running = 1, ending = 2
	int timeshift;
	// 0 = normal, 1 = permanent timeshift
	int timeshifttype;
	// position in timeshiftfile to seek
	off64_t timeshiftpos;
	// ask if timeshift is running and switch channel
	int asktimeshift;
	// ts playing running = 1
	int playing;
	// if set to a skin (screenpointer) only this screen has rc
	struct skin* rcowner;
	struct skin* rcstandby;
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
	//time for next picmem timeout check
	time_t picmemtimeout;
	time_t defpicmemtimeout;
	//show not tunable channel hidden (0)) or deaktive (1) in channellist
	int channellistview;
	//for blockmove in channellist
	int moveblockcount;
	//name ob file who played from webig
	char* webplayfile;
	//status of mediadbthread
	int mediadbthreadstatus;
	//for save mediadb in scan
	time_t mediadbsavetime;
	//colors / pic for selection
	long markcol;
	long filelistselectcol;
	long listboxselectcol;
	char* selectpic;
	//check if cam can decrypt channel
	int checkcamdecrypt;
	//don't send privat part of cadescriptor in capmt
	int caskipprivat;
	//send all caids or only caids cam can
	int casendallcaids;
	//show hidden files in filelist
	int showhiddenfiles;
	//mediadb
	int mediadbfiles;
	//show infobar on program change
	int infobarprogram;
	//for auto increase audio vol (mute)
	int volmute;
	int secondzap;
	//pointer to marker for recordings
	struct marker* playmarker;
	int autosubtitle;
	//save timeshift start time
	time_t timeshiftstart;
	int crosscontrol;
	int autochangechannelname;
	//tuxtext is running
	int tuxtxt;
	//on some fat32 hdd it takes very long to get free size
	int showrecfreesize;
	int firstunicablewait;
	//path for imdbfolder
	char* imdbfolderpath;
	int virtualzap;
	int fontsizeadjust;
	//count for drawing screen
	unsigned int drawscreencount;
	int autoseek;
	//skip all rc input events older than this value in millisec
	int rcskipms;
	//Workaround da ansonsten DVR4 nicht funktioniert (Treiberproblem)
	int setdvr0;
#ifdef MIPSEL
	//cec mipsel
	int cec_fixedAddress;
	int hdmiFd;
	int webencode;
#endif
	//pip Einstellungen
	int pipzap;
	// guest
	int whilecount;
	int sleepcount;
	int stats;
	char* url;
	//externer plugin Aufruf
	char* extplugin;
	//node oled blinken
	int skinblink;
#ifndef SIMULATE
	//BCM accel interface 
	int bcm;
#endif
	//Volume
	int volume;
	int screenshot;
	// cacc
	int certchecked;
	int certok;
	int liveslot;
	// dynamic default skin infobar
	int getepgaktstart;
	int getepgaktend;
	int getepgakttitle;
	int getepgakttimeremaining;
	int getepgnextstart;
	int getepgnextend;
	int getepgnexttitle;
	int getepgnexttimeremaining;
	// downmix switch in audio menu (audiotrack.h)
	int downmix;
	// channels for recording
	struct channel* recchnode[9];
	// frontdev filedescriptor
	int fdrc1;
	// Anzah VideoDevs
	int videodevs;
	char* streamurl;
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
	unsigned long data_phys;
	struct fb *next;
};

struct rc
{
	int key;
	void (*rcfunc)(struct skin*, struct skin*, int screencalc, int filelistview, int flag);
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
	uint64_t transponderid;
    char* streamurl;
    char* epgurl;
	struct channel *channel;
	struct bouquet *prev;
	struct bouquet *next;
};

struct channelslot
{
	int slot;
	int serviceid;
	uint64_t transponderid;
	struct channelslot *next;
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

struct mhwchannel
{
	u_char	network_id_hi;
	u_char	network_id_lo;
	u_char	transponder_id_hi;
	u_char	transponder_id_lo;
	u_char	channel_id_hi;
	u_char	channel_id_lo;
	u_char	name[16];
};
#define MHWCHANNELLEN sizeof(struct mhwchannel)

struct mhwtitle
{
	u_char table_id                       :8;
	u_char section_length_hi              :4;
	u_char                                :2;
	u_char dummy                          :1;
	u_char section_syntax_indicator       :1;
	u_char section_length_lo              :8;
	u_char channel_id                     :8;
	u_char theme_id                       :8;
	u_char hours		                      :5;
	u_char day                            :3;
	u_char summary_available              :1;
	u_char		                            :1;
	u_char minutes                        :6;
	u_char                                :8;
	u_char                                :8;
	u_char duration_hi                    :8;
	u_char duration_lo                    :8;
	u_char title                          [23];
	u_char ppv_id_hi                      :8;
	u_char ppv_id_mh                      :8;
	u_char ppv_id_ml                      :8;
	u_char ppv_id_lo                      :8;
	u_char program_id_hi                  :8;
	u_char program_id_mh                  :8;
	u_char program_id_ml                  :8;
	u_char program_id_lo                  :8;
	u_char                                :8;
	u_char                                :8;
	u_char                                :8;
	u_char                                :8;
};
#define MHWTITLELEN sizeof(struct mhwtitle)

struct mhwsummary
{
	u_char table_id                       :8;
	u_char section_length_hi              :4;
	u_char                                :2;
	u_char dummy                          :1;
	u_char section_syntax_indicator       :1;
	u_char section_length_lo              :8;
	u_char program_id_hi                  :8;
	u_char program_id_mh                  :8;
	u_char program_id_ml                  :8;
	u_char program_id_lo                  :8;
	u_char                                :8;
	u_char                                :8;
	u_char                                :8;
	u_char nb_replays                     :8;
};
#define MHWSUMMARYLEN sizeof(struct mhwsummary)

struct mhw2channel
{
	u_char network_id_hi;
	u_char network_id_lo;
	u_char transponder_id_hi;
	u_char transponder_id_lo;
	u_char channel_id_hi;
	u_char channel_id_lo;
	u_char                                :8;
	u_char                                :8;
};

struct extepgcache
{
	long id;
	struct epg* epgnode;
	struct extepgcache* next;
};

struct extepgchannel
{
	long id;
	int serviceid;
	uint64_t transponderid;
	struct extepgchannel* next;
};

struct extepgconfig
{
	uint64_t transponderid;
	int type;
	char* file;
	int channelpid;
	int titlepid[8];
	int summarypid[8];
	struct extepgconfig* next;
};

struct blacklist
{
  char* file;
  struct splitstr* splitstr;
  int count;
};

typedef struct sci_modes
{
	int emv2000;
	int dma;
	int man_act;
	int rw_mode;
} scimodes;

typedef struct sci_parameters
{
	u_char T;
	unsigned long fs;
	unsigned long ETU;
	unsigned long WWT;
	unsigned long CWT;
	unsigned long BWT;
	unsigned long EGT;
	unsigned long clock_stop_polarity;
	u_char check;
	u_char P;
	u_char I;
	u_char U;
} sciparameters;

struct update
{
	char* auth;
	char* type;
	char* skinname;
	char* filemask;
	char* filepath;
	int imgtype;
};

struct musicdat
{
	char* dummy;
	char* thumb;
	char* title;
	char* album;
	char* realname;
	char* actors;
	char* genre;
	char* year;
	int   act;
} musicdat;

#ifdef EPLAYER4
typedef enum { stUnknown, stPlainText, stSSA, stASS, stSRT, stVOB, stPGS } subtype_t;
typedef enum {atUnknown, atMPEG, atMP3, atAC3, atDTS, atAAC, atPCM, atOGG, atFLAC, atWMA} audiotype_t;
typedef struct _CustomData {
//typedef struct CustomData {
  GstElement *playbin2;   /* Our one and only element */
   
  gint n_video;          /* Number of embedded video streams */
  gint n_audio;          /* Number of embedded audio streams */
  gint n_text;           /* Number of embedded subtitle streams */
   
  gint current_video;    /* Currently playing video stream */
  gint current_audio;    /* Currently playing audio stream */
  gint current_text;     /* Currently playing subtitle stream */

  gboolean is_live;
  GstElement *pipeline;
  GMainLoop *loop;
} CustomData;

typedef enum {
	GST_PLAY_FLAG_VIDEO = (1 << 0),
	GST_PLAY_FLAG_AUDIO = (1 << 1),
	GST_PLAY_FLAG_TEXT = (1 << 2),
	GST_PLAY_FLAG_NATIVE_VIDEO = (1 << 3)
} GstPlayFlags;
#endif

#ifdef MIPSEL
//fuer cec Mipsel

#ifdef DREAMBOX
struct cec_message
{
	unsigned char address;
	unsigned char data[16];
	unsigned char length;
	unsigned char flag;
}__attribute__((packed));
struct cec_rx_message
{
	unsigned char address;
	unsigned char destination;
	unsigned char data[16];
	unsigned char length;
}__attribute__((packed));


#else
struct cec_message
{
unsigned char address;
unsigned char length;
unsigned char data[256];
}__attribute__((packed));
struct cec_rx_message
{
	unsigned char address;
	unsigned char destination;
	unsigned char data[16];
	unsigned char length;
}__attribute__((packed));
#endif

unsigned char cec_physicalAddress[2];
unsigned char cec_logicalAddress;
unsigned char cec_deviceType;

#endif

// cacc
/* CI+ certificates */

struct cert_ctx {
	X509_STORE *store;

	/* Host */
	X509 *cust_cert;
	X509 *device_cert;

	/* Module */
	X509 *ci_cust_cert;
	X509 *ci_device_cert;
};

#ifdef MIPSEL
enum ca_descr_data_type {
	CA_DATA_IV,
	CA_DATA_KEY,
};

enum ca_descr_parity {
	CA_PARITY_EVEN,
	CA_PARITY_ODD,
};

struct ca_descr_data {
	unsigned int index;
	enum ca_descr_parity parity;
	enum ca_descr_data_type data_type;
	unsigned int length;
	unsigned char *data;
};

#endif

#ifdef SH4NEW
enum ca_descr_data_type {
	CA_DATA_IV,
	CA_DATA_KEY,
};

enum ca_descr_parity {
	CA_PARITY_EVEN,
	CA_PARITY_ODD,
};

struct ca_descr_data {
	unsigned int index;
	enum ca_descr_parity parity;
	enum ca_descr_data_type data_type;
	unsigned int length;
	unsigned char *data;
};

#endif

#ifdef SIMULATE
enum ca_descr_data_type {
	CA_DATA_IV,
	CA_DATA_KEY,
};

enum ca_descr_parity {
	CA_PARITY_EVEN,
	CA_PARITY_ODD,
};

struct ca_descr_data {
	unsigned int index;
	enum ca_descr_parity parity;
	enum ca_descr_data_type data_type;
	unsigned int length;
	unsigned char *data;
};

#endif

struct aes_xcbc_mac_ctx {
	uint8_t K[3][16];
	uint8_t IV[16];
	AES_KEY key;
	int buflen;
};

uint32_t datatype_sizes[MAX_ELEMENTS] = {
	0, 50, 0, 0, 0, 8, 8, 0,
	0, 0, 0, 0, 32, 256, 256, 0,
	0, 256, 256, 32, 8, 8, 32, 32,
	0, 8, 2, 32, 1, 32, 1, 0,
	32
};

struct element {
	uint8_t *data;
	uint32_t size;
	/* buffer valid */
	int valid;
};

struct cc_ctrl_data {
	/* parent */
	struct caslot* slot;

	/* ci+ credentials */
	struct element elements[MAX_ELEMENTS];

	/* DHSK */
	uint8_t dhsk[256];

	/* KS_host */
	uint8_t ks_host[32];

	/* derived keys */
	uint8_t sek[16];
	uint8_t sak[16];

	/* AKH checks - module performs 5 tries to get correct AKH */
	unsigned int akh_index;

	/* authentication data */
	uint8_t dh_exp[256];

	/* certificates */
	struct cert_ctx *cert_ctx;

	/* private key of device-cert */
	RSA *rsa_device_key;
};

#endif
