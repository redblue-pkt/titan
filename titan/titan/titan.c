#include "struct.h"
#include "header.h"
#include "debug.h"
#include "crc32.h"
#include "sock.h"

#define SYSCODE 0

struct clist *config[LISTHASHSIZE] = {NULL};
struct clist *ownconfig[LISTHASHSIZE] = {NULL};
struct clist *rcconfig[LISTHASHSIZE] = {NULL};
struct clist *skinconfig[LISTHASHSIZE] = {NULL};
struct skin *skin = NULL;
struct fb *fb = NULL;
struct fb* skinfb = NULL;
struct fb* accelfb = NULL;
struct fb* fb1 = NULL;
struct fb* lcdskinfb = NULL;
struct font *font = NULL;
struct rc *rc = NULL;
struct dvbdev *dvbdev = NULL;
struct channel *channel = NULL;
struct transponder *transponder = NULL;
struct provider *provider = NULL;
struct sat *sat = NULL;
struct service *service = NULL;
struct mainbouquet *mainbouquet = NULL;
struct mainplaylist *mainplaylist = NULL;
struct pic *pic = NULL;
struct rcmap *rcmap = NULL;
struct inetwork *inetwork = NULL;
struct stimerthread *stimerthread = NULL;
struct rectimer *rectimer = NULL;
struct subpage* subpage = NULL, *oldsubpage = NULL;
struct epgscanlist* epgscanlist = NULL;
struct screensaver* screensaver = NULL;
struct channelcache* channelcache[CHANNELCACHEMAX] = {NULL};
struct transpondercache* transpondercache[TRANSPONDERCACHEMAX] = {NULL};
struct hdd* hdd = NULL;
struct queue* queue = NULL;
struct caservice caservice[MAXCASERVICE];
struct channelhistory channelhistory[MAXCHANNELHISTORY];
struct mostzap* mostzap = NULL;
struct mediadbfilter* mediadbfilter = NULL;
struct mediadbcategory* mediadbcategory = NULL;
struct mediadb* mediadb = NULL;
struct mediadbcache* mediadbcache[MEDIADBCACHEMAX] = {NULL};

#ifdef SH4
#include "sh4port.h"
#endif

#ifdef MIPSEL
#include "mipselport.h"
#endif

#ifdef I386
#include "i386port.h"
#endif

#include "ipkg.h"
#include "queue.h"
#include "channelcache.h"
#include "transpondercache.h"
#include "strconvert.h"
#include "numinput.h"
#include "textinput.h"
#include "radiotext.h"
#include "list.h"
#include "config.h"
#include "defaults.h"
#include "ownconfig.h"
#include "rcconfig.h"
#include "skinconfig.h"
#include "thumb.h"
#include "global.h"
#include "stream.h"
#include "dvbdev.h"
#include "rotorcalc.h"
#include "frontenddev.h"
#include "dmxdev.h"
#include "videodev.h"
#include "audiodev.h"
#include "cidev.h"
#include "cadev.h"
#ifdef CAMSUPP
#include "ca.h"
#endif
#include "dvrdev.h"
#include "cam.h"
#include "dvb.h"
#include "fb.h"
#include "font.h"
#include "rcmap.h"
#include "rc.h"
#include "sat.h"
#include "transponder.h"
#include "provider.h"
#include "channel.h"
#include "bouquets.h"
#include "mainbouquets.h"
#include "playlist.h"
#include "mainplaylist.h"
#include "pic.h"
#include "filelist.h"
#include "skinfunc.h"
#include "skin.h"
#include "inputhelp.h"
#include "rcfunc.h"
#include "listbox.h"
#include "choicebox.h"
#include "vfd.h"
#include "timerthread.h"
#include "service.h"
#include "player.h"
#include "dvdplayer.h"
#include "timeshift.h"
#include "record.h"
#include "zap.h"
#include "showiframe.h"
#include "epgrecord.h"
#include "multiepg.h"
#include "gmultiepg.h"
#include "epg.h"
#include "epgscan.h"
#include "menulist.h"
#include "videomode.h"
#include "inetwork.h"
#include "keyactions.h"
#include "scart.h"
#include "standby.h"
#include "dir.h"
#include "spinner.h"
#include "lnbconfig.h"
#include "diseqcconfig.h"
#include "tunerconfig.h"
#include "epgsettings.h"
#include "listedit.h"
#include "channellist.h"
#include "vfdisplay.h"
#include "videosettings.h"
#include "textbox.h"
#include "autores.h"
#include "language.h"
#include "about.h"
#include "serviceinfo.h"
#include "avsettings.h"
#include "mute.h"
#include "volume.h"
#include "timezone.h"
#include "adjust.h"
#include "skinadjust.h"
#include "blindscanadjust.h"
#include "moduleconfig.h"
#include "possetup.h"
#include "rectimer.h"
#include "pin.h"
#include "plugin.h"
#include "recordpath.h"
#include "subtitle.h"
#include "audiotrack.h"
#include "linkedchannel.h"
#include "powerofftimer.h"
#include "skinselector.h"
#include "play.h"
#include "satfinder.h"
#include "menu.h"
#include "channelbynr.h"
#include "infobar.h"
#include "network.h"
#include "harddisk.h"
#include "httpdfunc.h"
#include "httpd.h"
#include "download.h"
#include "screensaver.h"
#include "screensaveradjust.h"
#include "scan.h"
#include "restoredefault.h"
#include "help.h"
#include "httpdsettings.h"
#include "epgsearch.h"
#include "inadyn.h"
#include "rguid.h"
#include "channelhistroy.h"
#include "mostzap.h"
#include "settings_redbutton.h"
#include "settings_bluebutton.h"
#include "extensions.h"
#include "system_update.h"
#include "eraseswap.h"
#include "system_backup.h"
#include "system_backup_restore.h"
#include "info.h"
#include "softcam.h"
#include "mediadbcache.h"
#include "mediadb.h"
#include "mediadbsettings.h"

//#include "cardreader.h"
//#include "sci.h"

#define TIMECODE ""

int createstartscreen()
{
	addscreen("<screen name=framebuffer/>", 0, 0);
	if(skin == NULL)
	{
		err("can't set framebuffer skin");
		return 1;
	}
	addscreen("<screen name=skinerror/>", 0, 0);
	status.skinerr = getscreen("skinerror");
	skin->width = skinfb->width;
	skin->height = skinfb->height;
	skin->iwidth = skinfb->width;
	skin->iheight = skinfb->height;
	skin->rwidth = skinfb->width;
	skin->rheight = skinfb->height;
	skin->bgcol = 0;
	skin->transparent = 255;
	skin->flag = 1;

	return 0;
}

//exitcode: 1 power off
//exitcode: 2 restart
//exitcode: 3 Gui restart
//flag: 0 do not check record
//flag: 1 check record
//flag: 2 check record / do write config
//flag: 3 check record with timeout
//flag: 4 check record with increase fixpowerofftime
void oshutdown(int exitcode, int flag)
{
	debug(1000, "in");
	struct service* servicenode = service;
	void* threadstatus;
	int i = 0, faststop = 0, ret = 0;
	char* tmpstr = NULL;
	struct skin* logo = getscreen("logo");

	//check if record running
	if((flag == 1 || flag == 2 || flag == 3 || flag == 4) && (status.recording > 0 || getrectimerbytimediff(300) != NULL))
	{
		if(flag == 4 && status.fixpowerofftime > 1)
		{
				status.fixpowerofftime = time(NULL) + 900; //check powerofftimer again in 15min
				return;
		}
		int timeout = 0;
		if(flag == 3 || flag == 4) timeout = 15;
		if(textbox(_("Message"), _("Found running record\nor record is starting in next time.\nRealy shutdown ?"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, timeout, 1) == 2)
			return;
	}

	ret = servicestop(status.aktservice, 1, 0);
	if(ret == 1) return;
	
	if(exitcode == 1)
		tmpstr = getconfig("powerofflogo", NULL);
	else if(exitcode == 2)
		tmpstr = getconfig("restartlogo", NULL);
	else if(exitcode == 3)
		tmpstr = getconfig("guirestartlogo", NULL);
		
	if(tmpstr != NULL)
	{
		changepic(logo, tmpstr);
		tmpstr = NULL;
		drawscreen(logo, 0, 0);
	}
	else
		clearfball();

	//stop all records
	while(servicenode != NULL)
	{
		servicenode->recendtime = 1;
		servicenode = servicenode->next;
	}

	faststop = getconfigint("faststop", NULL);

	// Free memory, semaphores, etc. and say goodbye
	if(faststop == 0) cleanupvfd();

	tmpstr = oitoa(status.servicetype);
	addconfig("servicetype", tmpstr);
	free(tmpstr); tmpstr = NULL;

	status.sec = 0;

	if(flag != 2)
	{
		int epgsave = getconfigint("epgsave", NULL);
		if((epgsave == 1 && exitcode == 3) || epgsave == 2)
			writeallconfig(1); //do not save epg
		else
			writeallconfig(0); //save epg
	}

	if(faststop == 0)
	{
		//stop timer thread
		i = 0;
		status.timerthreadaktion = STOP;
		while(status.timerthreadstatus != DEACTIVE)
		{
			usleep(100000);
			i++; if(i > 20) break;
		}

		if(i > 20)
		{
			err("detect hanging timer thread");
		}
		else if(status.timerthread != '\0')
			pthread_join(status.timerthread, &threadstatus);
		pthread_attr_destroy(&status.timerthreadattr);

		// free timerthread struct and stop all timer sub threads
		freetimer(0);

		status.skinerr = NULL;
		free(status.gateway);
		free(status.dnsserver1);
		free(status.dnsserver2);
		free(status.boxtype);
		free(status.httpauth);
		free(status.bgpic);

		freeservice();

		freescreen();
		freepic();
		freefont();
		deinitfont();

		freerectimer();
		freemostzap(0);
		freechannelhistory();
		freesat();
		freeallplaylist();
		freemainplaylist();
		freeallbouquet();
		freemainbouquet();
		freechannel(0);
		freetransponder();
		freeprovider();
		freeownconfig();
		freeepgscanlist();
		freercconfig();
		freeskinconfig();
		freequeue();
		freehdd(0);

		free(status.configfile);
		freeconfig();

		freedvbdev();

		closefb(fb);
		freefb();

		closerc();
		freerc();
		freercmap();
		free_shutdowntimervar();
		freeinetwork();
	}
	else
		freetimer(1);

	starthttpd(0);
	startrguid(0);
	initmutex(0);

	debug(1000, "out");
	exit(exitcode);
}

int main(int argc, char *argv[])
{
	debug(1000, "in");
	int ret = 0, serviceret = 0, skincheck = 0;
	char* tmpstr = NULL;
	unsigned char *mmapfb = NULL;
	struct sigaction sa;
	struct stimerthread *tmpthread = NULL;

#ifdef SIMULATE
	// for mem leak debug
	setenv("MALLOC_TRACE", "/home/nit/titan/m.txt", 1);
	mtrace();
	status.security = 1;
#endif

	printf("[%s] copyright by %s - version %s\n", PROGNAME, COPYRIGHT, OVERSION);
	printf("[%s] crontribut: %s\n", PROGNAME, CRONTRIBUT);

	sa.sa_handler = (void *)sighandler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	sigaction(SIGUSR1, &sa, NULL);
	sigaction(SIGSEGV, &sa, NULL);
	sigaction(SIGBUS, &sa, NULL);
	sigaction(SIGABRT, &sa, NULL);
	sigaction(SIGPIPE, &sa, NULL);

	status.rguidfd = -1;
	status.sec = time(NULL);
	status.mainthread = pthread_self();

	if(argc > 1)
		status.configfile = ostrcat(argv[1], NULL, 0, 0);
	else
		status.configfile = ostrcat(CONFIGFILE, NULL, 0, 0);

	printf("[%s] using config: %s\n", PROGNAME, status.configfile);
	ret = readconfig(status.configfile, config);
	if(ret != 0)
		return 100;

	//change working dir to /tmp
	//chdir("/tmp");

	readconfig(getconfig("ownconfig", NULL), ownconfig);
	readconfig(getconfig("rcconfig", NULL), rcconfig);

	skincheck = checkskin();
	readconfig(getconfig("skinconfig", NULL), skinconfig);

	setprogress(100);
	setdefaults();
	setdebuglevel();
	initmutex(1);
	initvfd();

	if(checkbox("UFS922") == 1)
		setfanspeed(-1, 0);

	ret = setvideomode(getconfig("av_videomode", NULL), 0);

	ret = setpolicy(getconfig("av_policy", NULL));
	ret = setaspect(getconfig("av_aspect", NULL));
	ret = setcolorformat(getconfig("av_colorformat", NULL));
	ret = setaudiosource(getconfig("av_audiosource", NULL));
	ret = setac3(getconfig("av_ac3mode", NULL));
	ret = setmode3d(getconfig("av_mode3d", NULL));
	ret = setvfdbrightness(getconfigint("vfdbrightness", NULL));
	ret = addinetworkall(NULL);
	
#ifndef SIMULATE
	if(getconfigint("ss", NULL) == 0)
	{
		if(ostrcmp(string_newline(gettimeinfo()), TIMECODE) == 1)
		{
			printf("error: 1\n");
			destroy();
			exit(100);
		}
		if(ostrcmp(string_newline(gettimeinfovar()), TIMECODE) == 1)
		{
			printf("error: 2\n");		
			destroy();
			exit(100);
		}
		if(checkreseller() != 0)
		{
			printf("error: 3\n");		
			destroy();
			exit(100);
		}
		if(getsysinfo() != SYSCODE)
		{
			printf("error: 4\n");		
			destroy();
			exit(100);
		}
		if(file_exist("/mnt/swapextensions/etc/.vnumber") == 1)
		{
			printf("error: 5\n");		
			destroy();
			exit(100);
		}
		if(checkflash() != 0)
		{
			printf("error: 6\n");		
			destroy();
			exit(100);
		}
		char* tmpstr2 = NULL;
		tmpstr2 = getcpuid();
		checkserial(tmpstr2);
		free(tmpstr2), tmpstr2 = NULL;
	}
	else
		status.security = 1;
#endif

	ret = initfont();
	if(ret != 0)
		return 100;
	ret = openfont(getconfig("fontfile1", NULL));
	if(ret != 0)
	{
		err("open fontfile1 font");
		return 100;
	}
	openfont(getconfig("fontfile2", NULL));
	openfont(getconfig("fontfile3", NULL));
	openfont(getconfig("fontfile4", NULL));
	openfont(getconfig("fontfile5", NULL));

	//skin defined fonts
	openfont(getskinconfig("fontfile1", NULL));
	openfont(getskinconfig("fontfile2", NULL));
	openfont(getskinconfig("fontfile3", NULL));
	openfont(getskinconfig("fontfile4", NULL));
	openfont(getskinconfig("fontfile5", NULL));

	setvol(getconfigint("vol", NULL));

	setlang(getconfig("lang", NULL));
	initlocale(getconfig("localepath", NULL));

	fb = openfb(getconfig("fbdev", NULL), 0);
	if(fb == NULL)
		return 100;
	clearfball();
	enablemanualblit();

	tmpstr = getconfig("fb1dev", NULL);
	if(tmpstr != NULL)
		fb1 = openfb(tmpstr, 1);
	tmpstr = NULL;

	if(status.usedirectfb != 1)
	{
		skinfb = addfb(SKINFB, 0, getconfigint("skinfbwidth", NULL), getconfigint("skinfbheight", NULL), 4, fb->fd, fb->fb + fb->varfbsize, fb->fixfbsize);
		if(skinfb != NULL)
		{
			ret = getfbsize(0);
			if(ret > 0)
				accelfb = addfb(ACCELFB, 0, ret / 4, 1, 4, fb->fd, skinfb->fb + skinfb->varfbsize, fb->fixfbsize);
		}
		else
		{
			debug(100, "use directfb");
			status.usedirectfb = 1;
		}
	}

	if(status.usedirectfb == 1)
	{
		skinfb = fb;
		ret = getfbsize(0);
		if(ret > 0) 
			accelfb = addfb(ACCELFB, 0, ret / 4, 1, 4, fb->fd, skinfb->fb + skinfb->varfbsize, fb->fixfbsize);
	}

	if(lcdskinfb == NULL) {
		mmapfb = malloc(4 * 320 * 240);
		/* dev=999 ist LCD Buffer */
		lcdskinfb = addfb("lcdskinfb", 999, 320, 240, 4, -1, mmapfb, 4 * 320 * 240);
		// mmapfb = NULL;
	}

	ret = createstartscreen();
	if(ret != 0) return 100;

	//from here we can use starterror screen
	ret = openrc();
	if(ret != 0)
	{
		tmpstr = ostrcat(tmpstr, _("Error: open rc device !!"), 1, 0);
		goto starterror;
	}

	if(fegetdev() < 1)
	{
		tmpstr = ostrcat(tmpstr, _("Error: no frontend device found !!"), 1, 0);
		err("no frontend device found");
		goto starterror;
	}
	if(dmxgetdev() < 1)
	{
		tmpstr = ostrcat(tmpstr, _("Error: no demux device found !!"), 1, 0);
		err("no demux device found");
		goto starterror;
	}
	if(videogetdev() < 1)
	{
		tmpstr = ostrcat(tmpstr, _("Error: no video device found !!"), 1, 0);
		err("no video device found");
		goto starterror;
	}
	if(audiogetdev() < 1)
	{
		tmpstr = ostrcat(tmpstr, _("Error: no audio device found !!"), 1, 0);
		err("no audio device found");
		goto starterror;
	}
	ret = fecreatedummy();
	ret = cagetdev();
	ret = cigetdev();
	ret = dvrgetdev();

	//check skin
	if(skincheck > 0)
	{

		tmpstr = ostrcat(tmpstr, _("Error: skin not found !!"), 1, 0);
		err("skin not found");
		goto starterror;
	}

	ret = readsat(getconfig("satfile", NULL));
	ret = readtransponder(getconfig("transponderfile", NULL));
	ret = readprovider(getconfig("providerfile", NULL));
	ret = readchannel(getconfig("channelfile", NULL));
	ret = readtransponderencoding(getconfig("transponderencodingfile", NULL));
	ret = readmostzap(getconfig("mostzapfile", NULL));

	status.aktservice = addservice(NULL);
	status.lastservice = addservice(NULL);

	//check if startchannel defined
	char* startchannellist = getconfig("startchannellist", NULL);
	int startserviceid = getconfigint("startserviceid", NULL);
	unsigned long starttransponderid = getconfiglu("starttransponderid", NULL);
	int startservicetype = getconfigint("startservicetype", NULL);
	if(startchannellist != NULL && startserviceid != 0)
	{
		if(startservicetype == 0)
		{
			addconfig("channellist", startchannellist);
			addconfigint("serviceid", startserviceid);
			addconfiglu("transponderid", starttransponderid);
		}
		else
		{
			addconfig("rchannellist", startchannellist);
			addconfigint("rserviceid", startserviceid);
			addconfiglu("rtransponderid", starttransponderid);
		}
		addconfigint("servicetype", startservicetype);
	}

	//tune to channel
	if(status.servicetype == 0)
		serviceret = servicestart(getchannel(getconfigint("serviceid", NULL), getconfiglu("transponderid", NULL)), getconfig("channellist", NULL), NULL, 0);
	else
		serviceret = servicestart(getchannel(getconfigint("rserviceid", NULL), getconfiglu("rtransponderid", NULL)), getconfig("rchannellist", NULL),  NULL, 0);

	ret = readscreen(getconfig("skinfile", NULL), 0, 0);
	ret = readmainbouquet(getconfig("bouquetfile", NULL));
	ret = readallbouquet();
	ret = readmainplaylist(getconfig("playlistfile", NULL));
	ret = readallplaylist();
	ret = readrcmap(getconfig("rcmapfile", NULL));
	ret = readepgscanlist(getconfig("epgchannelfile", NULL));
	ret = settimezone(getconfig("timezone", NULL));

	//start timer thread
	status.timerthreadaktion = START;
	pthread_attr_init(&status.timerthreadattr);
	pthread_attr_setstacksize(&status.timerthreadattr, 50000);
	pthread_attr_setdetachstate(&status.timerthreadattr, PTHREAD_CREATE_JOINABLE);
	ret = pthread_create(&status.timerthread, &status.timerthreadattr, timerthreadfunc, NULL);
	if(ret)
	{
		tmpstr = ostrcat(tmpstr, _("Error: create timer thread !!"), 1, 0);
		err("create timer thread");
		goto starterror;
	}

	addtimer(&checkdate, START, 2000, -1, NULL, NULL, NULL);
	if((checkbox("ATEMIO500") == 0) && (checkbox("ATEMIO510") == 0))
		addtimer(&updatevfd, START, 1000, -1, NULL, NULL, NULL);

	//first wizzard
	if(getconfigint("nofirstwizzard", NULL) == 0)
	{
		if(file_exist("/tmp/.scart"))
		{
			setvideomode("pal", 0);
			changefbresolution("pal");
		}

		autoresolution();
firstwizzardstep1:
		if(checkbox("ATEMIO510") == 0)
		{
			status.updatevfd = PAUSE;
			screenavsettings(1);
			resettvpic();
			//this screen can reload the skin (on language change)
			//all skin changes before here than are deleted
			if(screenlanguage(2) == 2) return 100;
			resettvpic();
			screentunerconfig();
			resettvpic();
			screennetwork_adapter();
			resettvpic();
			writevfd("Setting OK ?");
			if(textbox(_("First Wizzard"), _("Settings OK ?"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 0, 0) == 2)
			{
				goto firstwizzardstep1;
			}
			resettvpic();
		}
		char* msg = NULL;
		msg = command("cat /etc/imageinfo");
		textbox(_("Info"), _(msg), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 1100, 680, 30, 0);
		free(msg), msg = NULL;
//		getserial();
		
		writevfd("");
		status.updatevfd = START;
		drawscreen(skin, 0, 0);
		addconfig("nofirstwizzard", "1");
	}
	else
	{
		// workaround, remove bootlogo on startup
		drawscreen(skin, 0, 0);

		//check servicestart
		if(serviceret != 21) // no message if startchannel empty
			servicecheckret(serviceret, 0);
	}

	//start spinner thread
	addtimer(&checkspinner, START, 2000, -1, NULL, NULL, NULL);
	//start auto shutdown thread
	addtimer(&checkshutdowntimer, START, 10000, -1, NULL, NULL, NULL);

	status.lastrcaction = time(NULL);
	ret = loadplugin();
	setosdtransparent(getskinconfigint("osdtransparent", NULL));
	ret = readrectimer(getconfig("rectimerfile", NULL));
	ret = setsaturation(getconfigint("vs_saturation", NULL));
	ret = setbrightness(getconfigint("vs_brightness", NULL));
	ret = setcontrast(getconfigint("vs_contrast", NULL));
	ret = settint(getconfigint("vs_tint", NULL));
	
	//set skinentrys locked
#ifndef SIMULATE
	if(status.security == 0) setskinnodeslocked(1);	
#endif

	//start epg thread
	status.epgthread = addtimer(&epgthreadfunc, START, 1000, -1, NULL, NULL, NULL);
	//start record timer thread
	addtimer(&checkrectimer, START, 10000, -1, NULL, NULL, NULL);
	//check if cam socket connected
	addtimer(&checkcam, START, 3000, -1, NULL, NULL, NULL);
	//start stream server
	tmpthread = addtimer(&streamthreadfunc, START, 10000, -1, NULL, NULL, NULL);
	if(tmpthread != NULL)
	{
		tmpthread->flag = setbit(tmpthread->flag, 0);
		tmpthread = NULL;
	}
	//start epg scanlist
	status.epgscanlistthread = addtimer(&epgscanlistthread, START, 1000, 1, NULL, NULL, NULL);
	//get pmt
	if(getconfigint("checkpmtalways", NULL) == 1)
		addtimer(&dvbgetpmtthread, START, 2000, -1, NULL, NULL, NULL);
	//check hdd
	status.addhddall = addtimer(&addhddall, START, 6000, -1, NULL, NULL, NULL);
	//check net
	addtimer(&addinetworkall, START, 15000, -1, NULL, NULL, NULL);
#ifdef CAMSUPP
	//start ca slot watching threads
	castart();
#endif
	//check skin nodes locked
	addtimer(&ckeckskinnodeslockedthread, START, 1000, 1, NULL, NULL, NULL);
	//check if box starts from a record
	addtimer(&checkboxstartthread, START, 1000, 1, NULL, NULL, NULL);
	//check kill net (security)
	addtimer(&ckeckkillnetthread, START, 1000, 1, NULL, NULL, NULL);

	//thumb create thread
	starthumb(1);

	//start webserver
	starthttpd(1);
	
	//start rguid
	startrguid(1);

	//init the player
	playerinit(argc, argv);

	system(getconfig("skriptaftertv", NULL));

	//must called direct befor screeninfobar
	if(getconfigint("saverun", NULL) == 1)
	{
		if(sigsetjmp(status.longjumpbuf, 1) != 0)
		{
			err("set sigsegjump");
		}
	}
	
	//check free space in /var
	if(getfreespace("/var") / 1024 < 5) //5kb
	{
		textbox(_("Message"), _("Free space in /var to little!\nThis can make problems!"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 7, 0);
	}

	screeninfobar();

	//for testign screens
	//screenmanualscan();
	//screeninputhelp();
	//screendownload("test", "www.orf.at", NULL, 80, "test.html", 0);
	//screenrectimer();
	//screenepgsettings();
	//screenlnb("1");
	//screendiseqc("1");
	//screenlanguage();
	//screenvideosettings();
	//screenskinselect();
	//screenrectimer();
	//screennetwork_adapter(2);
	//screenpin();
	//screenchannellist();
	//recordstart(channel, RECDIRECT);
	//mc_main();
	//screenmc_videoplayer();
	//screenserviceinfo();
	//screentunerconfig();
	//screenvfdisplay();
	//screenavsettings(1);
	//screenadjust();
	//subtitlestart();

	if(mmapfb != NULL) {
		free(mmapfb); mmapfb=NULL;
	}
	debug(1000, "out");
	oshutdown(1, 1);
	return 0;

starterror:
	addscreen("<screen hspace=5 vspace=5 type=textbox name=starterror posx=center posy=center bordersize=2 bordercol=#ffffff fontsize=30 fontcol=#ffffff width=600 height=150/>", 0, 0);
	struct skin *starterror = getscreen("starterror");
	if(tmpstr == NULL)
		tmpstr = ostrcat(tmpstr, _("Unknown Error."), 1, 0);
	tmpstr = ostrcat(tmpstr, "\n", 1, 0);
	tmpstr = ostrcat(tmpstr, _("Automatic stop in 5 seconds."), 1, 0);
	changetext(starterror, _(tmpstr));
	drawscreen(starterror, 0, 0);
	sleep(5);
	free(tmpstr);
	return 100;
}