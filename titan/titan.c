#include "struct.h"
#include "header.h"
#include "debug.h"
#include "crc32.h"
#include "sock.h"

struct clist *config[LISTHASHSIZE] = {NULL};
struct clist *ownconfig[LISTHASHSIZE] = {NULL};
struct clist *rcconfig[LISTHASHSIZE] = {NULL};
struct clist *skinconfig[LISTHASHSIZE] = {NULL};
struct skin *skin = NULL;
struct fb *fb = NULL;
struct fb* skinfb = NULL;
struct fb* accelfb = NULL;
struct fb* fb1 = NULL;
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
struct hdd* hdd = NULL;
struct queue* queue = NULL;

#include "queue.h"
#include "channelcache.h"
#include "strconvert.h"
#include "textinput.h"
#include "radiotext.h"
#include "list.h"
#include "config.h"
#include "defaults.h"
#include "ownconfig.h"
#include "rcconfig.h"
#include "skinconfig.h"
#include "global.h"
#include "stream.h"
#include "dvbdev.h"
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
#include "bluekeyactions.h"
#include "redkeyactions.h"
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
#include "moduleconfig.h"
#include "rectimer.h"
#include "pin.h"
#include "plugin.h"
#include "recordpath.h"
#include "subtitle.h"
#include "audiotrack.h"
#include "powerofftimer.h"
#include "skinselector.h"
#include "play.h"
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

	return 0;
}

//exitcode: 1 power off
//exitcode: 2 restart
//exitcode: 3 Gui restart
void oshutdown(int exitcode, int flag)
{
	debug(1000, "in");
	struct service* servicenode = service;
	void* threadstatus;
	int i = 0, faststop = 0, ret = 0;
	char* tmpstr = NULL;

	//check if record running
	if(flag == 1 && status.recording > 0)
	{
		if(textbox(_("Message"), _("Found running Record.\nRealy shutdown ?"), _("EXIT"), getrcconfigint("rcexit", NULL), _("OK"), getrcconfigint("rcok", NULL), NULL, 0, NULL, 0, 600, 200, 0, 1) == 1)
			return;
	}
	
	ret = servicestop(status.aktservice, 1, 0);
	if(ret == 1) return;

	//stop all records
	while(servicenode != NULL)
	{
		servicenode->recendtime = 1;
		servicenode = servicenode->next;
	}

	faststop = getconfigint("faststop", NULL);

	clearfball();

	// Free memory, semaphores, etc. and say goodbye
	if(faststop == 0) cleanupvfd();

	tmpstr = oitoa(status.servicetype);
	addconfig("servicetype", tmpstr);
	free(tmpstr); tmpstr = NULL;

	status.sec = 0;

	writeallconfig(0);

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
		else
			pthread_join(status.timerthread, &threadstatus);
		pthread_attr_destroy(&status.timerthreadattr);

		// free timerthread struct and stop all timer sub threads
		freetimer();

		status.skinerr = NULL;
		free(status.gateway);
		free(status.dnsserver1);
		free(status.dnsserver2);
		free(status.oldchannellist);
		free(status.oldrchannellist);
		free(status.boxtype);

		freeservice();

		freescreen();
		freepic();
		freefont();
		deinitfont();

		freerectimer();
		freesat();
		freeallplaylist();
		freemainplaylist();
		freeallbouquet();
		freemainbouquet();
		freechannel();
		freetransponder();
		freeprovider();
		freeownconfig();
		freeepgscanlist();
		freercconfig();
		freeskinconfig();
		freequeue();
		freehdd();

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

	initmutex(0);
	starthttpd(0);

	debug(1000, "out");
	exit(exitcode);
}

int main(int argc, char *argv[])
{
	debug(1000, "in");
	int ret = 0, serviceret = 0, skincheck = 0;
	char* tmpstr = NULL;
	struct sigaction sa;

#ifdef SIMULATE
	// for mem leak debug
	setenv("MALLOC_TRACE", "/home/nit/titan/m.txt", 1);
	mtrace();
#endif

	printf("[%s] copyright by %s - version %s\n", PROGNAME, COPYRIGHT, VERSION);
	printf("[%s] crontribut: %s\n", PROGNAME, CRONTRIBUT);

	sa.sa_handler = (void *)sighandler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	sigaction(SIGUSR1, &sa, NULL);
	sigaction(SIGSEGV, &sa, NULL);
	sigaction(SIGBUS, &sa, NULL);
	sigaction(SIGABRT, &sa, NULL);

	status.sec = time(NULL);
	status.mainthread = pthread_self();

	if(argc > 1)
		status.configfile = ostrcat(argv[1], "", 0, 0);
	else
		status.configfile = ostrcat(CONFIGFILE, "", 0, 0);

	printf("[%s] using config: %s\n", PROGNAME, status.configfile);
	ret = readconfig(status.configfile, config);
	if(ret != 0)
		return 100;
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
			
	if(getconfig("timetosleep", NULL) == NULL)
		settimetosleep(0);
	else 
		settimetosleep(atoi(getconfig("timetosleep", NULL)));
	
	if(getconfig("av_videomode_default", NULL) == NULL)
		ret = setvideomode(getconfig("av_videomode", NULL));
	else
		ret = setvideomode(getconfig("av_videomode_default", NULL));
		
	ret = setpolicy(getconfig("av_policy", NULL));
	ret = setaspect(getconfig("av_aspect", NULL));
	ret = setcolorformat(getconfig("av_colorformat", NULL));
	ret = setaudiosource(getconfig("av_audiosource", NULL));
	ret = setac3(getconfig("av_ac3mode", NULL));
	ret = setmode3d(getconfig("av_mode3d", NULL));
	ret = setvfdbrightness(getconfigint("vfdbrightness", NULL));

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

	tmpstr = getconfig("fb1dev", NULL);
	if(tmpstr != NULL)
		fb1 = openfb(tmpstr, 1);
	tmpstr = NULL;

	if(status.usedirectfb != 1)
	{
		skinfb = addfb(SKINFB, 0, getconfigint("skinfbwidth", NULL), getconfigint("skinfbheight", NULL), 4, 0, NULL, 0);
		if(skinfb != NULL)
		{
			ret = getfbsize(0);
			if(ret > 0)
				accelfb = addfb(ACCELFB, 0, ret / 4, 1, 4, 0, NULL, 0);
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
			accelfb = addfb(ACCELFB, 0, ret / 4, 1, 4, 0, NULL, 0);
	}

	ret = createstartscreen();
	if(ret != 0) return 100;
	
	//from here we can use starterror screen
	ret = openrc();
	if(ret != 0)
	{
		tmpstr = ostrcat(tmpstr, "Error: open rc device !!", 1, 0);
		goto starterror;
	}
	ret = openrcsim();
	if(ret != 0)
	{
		tmpstr = ostrcat(tmpstr, "Error: open rcsim device !!", 1, 0);
		goto starterror;
	}

	if(fegetdev() < 1)
	{
		tmpstr = ostrcat(tmpstr, "Error: no frontend device found !!", 1, 0);
		err("no frontend device found");
		goto starterror;
	}
	if(dmxgetdev() < 1)
	{
		tmpstr = ostrcat(tmpstr, "Error: no demux device found !!", 1, 0);
		err("no demux device found");
		goto starterror;
	}
	if(videogetdev() < 1)
	{
		tmpstr = ostrcat(tmpstr, "Error: no video device found !!", 1, 0);
		err("no video device found");
		goto starterror;
	}
	if(audiogetdev() < 1)
	{
		tmpstr = ostrcat(tmpstr, "Error: no audio device found !!", 1, 0);
		err("no audio device found");
		goto starterror;
	}
	ret = fecreatedummy();
	ret = cigetdev();
	ret = cagetdev();
	ret = dvrgetdev();
	
	//check skin
	if(skincheck > 0)
	{
		tmpstr = ostrcat(tmpstr, "Error: skin not found !!", 1, 0);
		goto starterror;
	}

	ret = readsat(getconfig("satfile", NULL));
	ret = readtransponder(getconfig("transponderfile", NULL));
	ret = readprovider(getconfig("providerfile", NULL));
	ret = readchannel(getconfig("channelfile", NULL));

	status.aktservice = addservice(NULL);
	status.lastservice = addservice(NULL);
	
	//check if startchannel defined
	char* startchannellist = getconfig("startchannellist", NULL);
	int startserviceid = getconfigint("startserviceid", NULL);
	int starttransponderid = getconfigint("starttransponderid", NULL);
	int startservicetype = getconfigint("startservicetype", NULL);
	if(startchannellist != NULL && startserviceid != 0)
	{
		if(startservicetype == 0)
		{
			addconfig("channellist", startchannellist);
			addconfigint("serviceid", startserviceid);
			addconfigint("transponderid", starttransponderid);
		}
		else
		{
			addconfig("rchannellist", startchannellist);
			addconfigint("rserviceid", startserviceid);
			addconfigint("rtransponderid", starttransponderid);
		}
		addconfigint("servicetype", startservicetype);
	}

	//tune to channel
	if(status.servicetype == 0)
		serviceret = servicestart(getchannel(getconfigint("serviceid", NULL), getconfigint("transponderid", NULL)), NULL, 0);
	else
		serviceret = servicestart(getchannel(getconfigint("rserviceid", NULL), getconfigint("rtransponderid", NULL)), NULL, 0);

	ret = readscreen(getconfig("skinfile", NULL), 0, 0);
	ret = readmainbouquet(getconfig("bouquetfile", NULL));
	ret = readallbouquet();
	ret = readmainplaylist(getconfig("playlistfile", NULL));
	ret = readallplaylist();
	ret = readrcmap(getconfig("rcmapfile", NULL));
	ret = readepgscanlist(getconfig("epgchannelfile", NULL));
	ret = settimezone(getconfig("timezone", NULL));
	ret = addinetworkall();

	//start timer thread
	status.timerthreadaktion = START;
	pthread_attr_init(&status.timerthreadattr);
	pthread_attr_setstacksize(&status.timerthreadattr, 50000);
	pthread_attr_setdetachstate(&status.timerthreadattr, PTHREAD_CREATE_JOINABLE);
	ret = pthread_create(&status.timerthread, &status.timerthreadattr, timerthreadfunc, NULL);
	if(ret)
	{
		tmpstr = ostrcat(tmpstr, "Error: create timer thread !!", 1, 0);
		err("create timer thread");
		goto starterror;
	}

	addtimer(&checkdate, START, 1000, -1, NULL, NULL, NULL);
	addtimer(&updatevfd, START, 1000, -1, NULL, NULL, NULL);

	//first wizzard
	if(getconfigint("nofirstwizzard", NULL) == 0)
	{
		if(file_exist("/tmp/.scart"))
		{
			setvideomode("pal"); 
			changefbresolution("pal");
		}

		autoresolution();
firstwizzardstep1:
		status.updatevfd = PAUSE;
		screenavsettings(1);
		//this screen can reload the skin (on language change)
		//all skin changes before here than are deleted
		if(screenlanguage(2) == 2) return 100; 
		screentunerconfig(); 
		screennetwork_adapter(); 
 	 	
		writevfd("Setting OK ?");
		if(textbox(_("First Wizzard"), _("Settings OK ?"), _("EXIT"), getrcconfigint("rcexit", NULL), _("OK"), getrcconfigint("rcok", NULL), NULL, 0, NULL, 0, 600, 200, 0, 0) == 1)
		{
			goto firstwizzardstep1;
		}

		writevfd("");
		status.updatevfd = START;
		drawscreen(skin, 0);
		addconfig("nofirstwizzard", "1");
	}
	
	//start spinner thread
	addtimer(&checkspinner, START, 1000, -1, NULL, NULL, NULL);
	//start auto shutdown thread
	addtimer(&checkshutdowntimer, START, 1000, -1, NULL, NULL, NULL);

	//check servicestart
	if(serviceret != 21) // no message if startchannel empty
		servicecheckret(serviceret, 0);

	ret = loadplugin();
	setosdtransparent(getskinconfigint("osdtransparent", NULL));
	ret = readrectimer(getconfig("rectimerfile", NULL));
	ret = setsaturation(getconfigint("vs_saturation", NULL));
	ret = setbrightness(getconfigint("vs_brightness", NULL));
	ret = setcontrast(getconfigint("vs_contrast", NULL));
	ret = settint(getconfigint("vs_tint", NULL));

	//start epg thread
	status.epgthread = addtimer(&epgthreadfunc, START, 1000, -1, NULL, NULL, NULL);
	//start record timer thread
	addtimer(&checkrectimer, START, 10000, -1, NULL, NULL, NULL);
	//check if cam socket connected
	addtimer(&checkcam, START, 1000, -1, NULL, NULL, NULL);
	//start stream server
	addtimer(&streamthreadfunc, START, 10000, -1, NULL, NULL, NULL);
	//start epg scanlist
	status.epgscanlistthread = addtimer(&epgscanlistthread, START, 1000, 1, NULL, NULL, NULL);
	//get pmt
	addtimer(&dvbgetpmtthread, START, 1000, -1, NULL, NULL, NULL);
	//check hdd (in the moment not needed, but can aktivated when needed)
	//addtimer(&addhddall, START, 3000, -1, NULL, NULL, NULL);
#ifdef CAMSUPP
	//start ca slot watching threads
	castart();
#endif

	//start webserver
	starthttpd(1);
	
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

//createcapmt(status.aktservice->channel);
//screeninputhelp();
//recrepeatecheck();
//sleep(1000);
//gethttp(NULL, NULL, 80, "test.html");
//screendownload("test", "www.orf.at", NULL, 80, "test.html", 0);
//	screentunerconfig();
//screenmanualscan();
	screeninfobar();

	//for testign screens
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

	debug(1000, "out");
	oshutdown(1, 1);
	return 0;
	
starterror:
	addscreen("<screen hspace=5 vspace=5 type=textbox name=starterror posx=center posy=center bordersize=2 bordercol=#ffffff fontsize=30 fontcol=#ffffff width=600 height=150/>", 0, 0);
	struct skin *starterror = getscreen("starterror");
	if(tmpstr == NULL)
		tmpstr = ostrcat(tmpstr, "Unknown Error.", 1, 0);
	tmpstr = ostrcat(tmpstr, "\nAutomatic stop in 5 seconds.", 1, 0);
	changetext(starterror, _(tmpstr));
	drawscreen(starterror, 0);
	sleep(5);
	free(tmpstr);
	return 100;
}
