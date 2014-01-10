#include "struct.h"
#include "header.h"
#include "debug.h"
#include "crc32.h"
#include "sock.h"

#define SYSCODE 0UL
#define HTTPAUTH "aXBrLUdaRmg6RkhaVkJHaG56ZnZFaEZERlRHenVpZjU2NzZ6aGpHVFVHQk5Iam0="

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
struct dvbdev *dvbdevsim = NULL;
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
struct unicable* unicable = NULL;
struct oldentry* oldentry = NULL;
struct newsletter* newsletter = NULL;
struct extepgcache* extepgcache = NULL;
struct extepgchannel* extepgchannel = NULL;
struct extepgconfig* extepgconfig = NULL;
struct lastsubtitle* lastsubtitle = NULL;
struct style* style = NULL;
struct download* bgdownload[MAXBGDOWNLOAD] = {NULL};
struct channelslot *channelslot = NULL;

#ifdef SH4
#include "sh4port.h"
#endif

#ifdef MIPSEL
#include "mipselport.h"
#endif

#ifdef I386
#include "i386port.h"
#endif

#include "oldentry.h"
#include "tpk.h"
#include "jsmn.h"
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
#include "scdev.h"
#include "ca.h"
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
#include "style.h"
#include "skin.h"
#include "inputhelp.h"
#include "rcfunc.h"
#include "listbox.h"
#include "choicebox.h"
#include "vfd.h"
#include "timerthread.h"
#include "service.h"
#include "pip.h"
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
#include "eit.h"
#include "extepg.h"
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
#include "scconfig.h"
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
#include "copyfile.h"
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
#include "system_backup.h"
#include "system_backup_restore.h"
#include "info.h"
#include "softcam.h"
#include "id3.h"
#include "mediadbcache.h"
#include "mediadb.h"
#include "mediadbsettings.h"
#include "colorpicker.h"
#include "ocrypt.h"
#include "unlock.h"
#include "md5.h"
#include "rc4.h"
#include "textinputhist.h"
#include "system_infos.h"
#include "system_infos_sysinfo.h"
#include "unicable.h"
#include "newsletter.h"
#include "dirsort.h"
#include "shortepg.h"
#include "mediadbedit.h"
#include "tpchoice.h"
#include "marker.h"
#include "timeshiftsettings.h"
#include "log.h"
#include "bgdownload.h"
#include "fancontrol.h"
#include "channelslot.h"
#include "hwtest.h"
#include "settings_autostart.h"
#include "settings_overclocking.h"

#define TIMECODE ""

void demomodethread(struct stimerthread* self)
{
	textbox(_("Message"), _("!!! This is a DEMO Version !!!"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 600, 0, 1);
}

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
//flag: 2 check record / don't write config
//flag: 3 check record with timeout
//flag: 4 check record with increase fixpowerofftime
//flag: 5 check record with increase powerofftime
void oshutdown(int exitcode, int flag)
{
	struct dvbdev* dvbnode = dvbdev;
	struct service* servicenode = service;
	void* threadstatus;
	int i = 0, faststop = 0, ret = 0;
	char* tmpstr = NULL;
	struct skin* logo = getscreen("logo");
	pthread_attr_t writethreadattr = {};
	pthread_t writethread = '\0';

	//stop permanent timeshift
	if(status.timeshift == 1)
		timeshiftstop(3);

	//check if record running
	if((flag == 1 || flag == 2 || flag == 3 || flag == 4 || flag == 5) && (status.recording > 0 || getrectimerbytimediff(300) != NULL))
	{
		if(flag == 4 && status.fixpowerofftime > 1)
		{
				status.fixpowerofftime = time(NULL) + 900; //check powerofftimer again in 15min
				return;
		}
		if(flag == 5 && status.sd_timer != NULL && status.sd_timer->active)
		{
				status.sd_timer->shutdown_time = time(NULL) + 900; //check powerofftimer again in 15min
				return;
		}

		if(textbox(_("Message"), _("Found running record\nor record is starting in next time.\nReally shutdown ?"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 15, 0) != 1)
			return;
	}

	//check if download is running
	for(i = 0; i < MAXBGDOWNLOAD; i++)
	{
		if(bgdownload[i] != NULL && bgdownload[i]->ret == -1)
		{
	 		if(textbox(_("Message"), _("Found running download\nReally shutdown ?"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 15, 0) != 1)
				return;
			break;
		}
	}

	ret = servicestop(status.aktservice, 1, 1);
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

	if(status.servicetype != 99) //don't save tmp servicetype
	{
		tmpstr = oitoa(status.servicetype);
		addconfig("servicetype", tmpstr);
		free(tmpstr); tmpstr = NULL;
	}

	status.sec = 0;

	if(flag != 2)
	{
		//start write thread
		pthread_attr_init(&writethreadattr);
		pthread_attr_setstacksize(&writethreadattr, 50000);
		pthread_attr_setdetachstate(&writethreadattr, PTHREAD_CREATE_JOINABLE);

		int epgsave = getconfigint("epgsave", NULL);
		if((epgsave == 1 && exitcode == 3) || epgsave == 2)
			ret = pthread_create(&writethread, &writethreadattr, writeallconfigthread1, NULL); //do not save epg
		else
			ret = pthread_create(&writethread, &writethreadattr, writeallconfigthread0, NULL); //save epg
		if(ret)
		{
			err("create write thread");
		}
	}

	//reset ci devices
	while(dvbnode != NULL)
	{
		if(dvbnode->type == CIDEV && dvbnode->fd > -1)
		{
			if(dvbnode->caslot != NULL) dvbnode->caslot->status = 101;
			usleep(10000);
			careset(dvbnode, dvbnode->devnr);
		}
		dvbnode = dvbnode->next;
	}

	if(faststop == 0)
	{
		//wait for write thread
		if(writethread != '\0')
			pthread_join(writethread, &threadstatus);
		pthread_attr_destroy(&writethreadattr);

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
		freecaservice();

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
		freemainbouquet(0);
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

		freedvbdev(0);

		closefb(fb);
		freefb();

		closerc();
		freerc();
		freercmap();
		free_shutdowntimervar();
		freeinetwork();
		freeoldentry();
		freeextepgconfig();
		freelastsubtitle();
		freebgdownload();
		freechannelslot();
	}
	else
		freetimer(1);

	if(exitcode == 1)
		setcecstandby(1);

	starthttpd(0);
	startrguid(0);
	initmutex(0);

	if(faststop != 0)
	{
		//wait for write thread
		if(writethread != '\0')
			pthread_join(writethread, &threadstatus);
		pthread_attr_destroy(&writethreadattr);
	}

	exit(exitcode);
}

int main(int argc, char *argv[])
{
	int ret = 0, serviceret = 0, skincheck = 0;
	char* tmpstr = NULL;
	unsigned char *mmapfb = NULL;
	struct sigaction sa;
	struct stimerthread *tmpthread = NULL;
	//struct sched_param schedparam;

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
	
	initsignal(&sa);

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

	//start timer thread
	status.timerthreadaktion = START;
	pthread_attr_init(&status.timerthreadattr);
	pthread_attr_setstacksize(&status.timerthreadattr, 50000);
	pthread_attr_setdetachstate(&status.timerthreadattr, PTHREAD_CREATE_JOINABLE);
	ret = pthread_create(&status.timerthread, &status.timerthreadattr, timerthreadfunc, NULL);
	if(ret)
	{
		err("create timer thread");
		return 100;
	}

	//change working dir to /tmp
	//chdir("/tmp");

	//set main scheduler priority
	//schedparam.sched_priority = 10;
	//pthread_setschedparam(pthread_self(), SCHED_RR, &schedparam);

	readconfig(getconfig("ownconfig", NULL), ownconfig);
	readconfig(getconfig("rcconfig", NULL), rcconfig);

	skincheck = checkskin();
	readconfig(getconfig("skinconfig", NULL), skinconfig);

	setprogress(100);
	setdefaults();
	setdebuglevel();
	initmutex(1);
	m_lock(&status.waitrcmutex, 24);
	initvfd();

	if(checkbox("UFS922") == 1)
		setfanspeed(-1, 0);

	system(getconfig("skriptbeforetv", NULL));
	ret = setcecstandby(0);
	ret = setvideomode(getconfig("av_videomode", NULL), 0);
	ret = setpolicy(getconfig("av_policy", NULL));
	ret = setaspect(getconfig("av_aspect", NULL));
	ret = setcolorformat(getconfig("av_colorformat", NULL), 0);
	ret = setcolorformat(getconfig("av_colorformatscart", NULL), 1);
	ret = setaudiosource(getconfig("av_audiosource", NULL));
	ret = setac3(getconfig("av_ac3mode", NULL));
	ret = setmode3d(getconfig("av_mode3d", NULL));
	ret = setvfdbrightness(getconfigint("vfdbrightness", NULL));
	ret = setaudiodelaybitstream(getconfig("audiodelaybitstream", NULL));
	ret = addinetworkall(NULL);
	
#ifndef SIMULATE
	if(getconfigint("sos", NULL) == 0)
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
		if(checkbox("ATEMIO520__") == 1 && checkhighflash() != 0)
		{
			printf("error: 7\n");		
			destroy();
			exit(100);
		}
		if(checkbox("UFS910") == 1 && checklowflash() != 0)
		{
			printf("error: 8\n");		
			destroy();
			exit(100);
		}
	
		char* cpuid = getcpuid();
		checkserial(cpuid);
		free(cpuid); cpuid = NULL;
	}
	else
	{
		status.security = 1;
		startnet();
		setskinnodeslocked(0);
	}
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

	if(checkbox("ATEMIO5000") != 1)
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

	//if(lcdskinfb == NULL) {
		//mmapfb = malloc(4 * 320 * 240);
		//mmapfb = malloc(4 * 1024 * 768);
		//mmapfb = NULL;
		/* dev=999 ist LCD Buffer */
		//lcdskinfb = addfb("lcdskinfb", 999, 320, 240, 4, -1, mmapfb, 4 * 320 * 240);
		//lcdskinfb = addfb("lcdskinfb", 999, 1024, 768, 4, -1, mmapfb, 4 * 1024 * 768);
		// mmapfb = NULL;
	//}

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
	ret = scgetdev();
	settunerstatus();

	//start ca slot watching threads
	castart();

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
	ret = readlastsubtitle(getconfig("lastsubtitle", NULL));
	ret = readchannelslot(getconfig("channelslotfile", NULL));

	status.aktservice = addservice(NULL);
	status.lastservice = addservice(NULL);
	status.pipservice = addservice(NULL);

	//check if startchannel defined
	char* startchannellist = getconfig("startchannellist", NULL);
	int startserviceid = getconfigint("startserviceid", NULL);
	uint64_t starttransponderid = getconfigllu("starttransponderid", NULL);
	int startservicetype = getconfigint("startservicetype", NULL);
	if(startchannellist != NULL && startserviceid != 0)
	{
		if(startservicetype == 0)
		{
			addconfig("channellist", startchannellist);
			addconfigint("serviceid", startserviceid);
			addconfigllu("transponderid", starttransponderid);
		}
		else
		{
			addconfig("rchannellist", startchannellist);
			addconfigint("rserviceid", startserviceid);
			addconfigllu("rtransponderid", starttransponderid);
		}
		addconfigint("servicetype", startservicetype);
	}

	//tune to channel
	if(status.servicetype == 0)
		serviceret = servicestart(getchannel(getconfigint("serviceid", NULL), getconfigllu("transponderid", NULL)), getconfig("channellist", NULL), NULL, 0);
	else
		serviceret = servicestart(getchannel(getconfigint("rserviceid", NULL), getconfigllu("rtransponderid", NULL)), getconfig("rchannellist", NULL),  NULL, 0);

	ret = readscreen(getconfig("skinfile", NULL), 0, 0);
	ret = readmainbouquet(getconfig("bouquetfile", NULL));
	ret = readallbouquet();
	ret = readmainplaylist(getconfig("playlistfile", NULL));
	ret = readallplaylist();
	ret = readrcmap(getconfig("rcmapfile", NULL));
	ret = readepgscanlist(getconfig("epgchannelfile", NULL));
	ret = settimezone(getconfig("timezone", NULL));
	ret = readextepgconfig(getconfig("extepgfile", NULL));
	
	//set volume on start
	if(checkbox("ATEMIO5000") == 1)
		setvol(getconfigint("vol", NULL));

	//check to remove preinstalled tpk packages
	ret = tpkupdatepre();

	addtimer(&checkdate, START, 2000, -1, NULL, NULL, NULL);
	if(checkbox("ATEMIO510") == 0)
		addtimer(&updatevfd, START, 1000, -1, NULL, NULL, NULL);

	if(getconfigint("firststart", NULL) == 1)
	{
		if(file_exist("/var/etc/.scart"))
		{
			setvideomode("pal", 0);
			changefbresolution("pal", 0);
			autoresolution();
		}
		addconfig("firststart", "0");
	}
		
	//first wizzard
	if(getconfigint("nofirstwizzard", NULL) < 2)
	{
		if(file_exist("/var/etc/.scart"))
		{
			setvideomode("pal", 0);
			changefbresolution("pal", 0);
		}

		autoresolution();

		if(getconfigint("nofirstwizzard", NULL) == 0)
		{
firstwizzardstep1:

			status.updatevfd = PAUSE;
//			screenavsettings(1);
//			resettvpic();
			//this screen can reload the skin (on language change)
			//all skin changes before here than are deleted
//			if(screenlanguage(2) == 2) return 100;
//			resettvpic();

			if(checkbox("ATEMIO520") == 1 || checkbox("ATEMIO530") == 1)
			{
				struct menulist* mlist = NULL, *mbox = NULL;
				addmenulist(&mlist, "RemoteControl Long Version", "0", NULL, 0, 0);
				addmenulist(&mlist, "RemoteControl Old Version", "1", NULL, 0, 0);
				
	//			mbox = menulistbox(mlist, "playlistmenu", NULL, "%pluginpath%/mc/skin", NULL, 1, 0);
				mbox = menulistbox(mlist, "remotecontrol", NULL, NULL, NULL, 2, 0);
				if(mbox != NULL)
				{
					debug(10, "mbox->name %s", mbox->name);
					debug(10, "mbox->text %s", mbox->text);
					char* tmp = ostrcat(mbox->text, NULL, 0, 0);
					addconfigscreencheck("remotecontrol", tmp, "0");
					free(tmp),tmp = NULL;
					writeallconfig(1);				
				}
			}
			else
			{
				screentunerconfig();
				resettvpic();
				screenscanconfig(1);
				resettvpic();
	//			screennetwork_adapter();
	//			resettvpic();
				writevfd("Setting OK ?");
				if(textbox(_("First Wizzard"), _("Settings OK ?"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 0, 0) == 2)
				{
					goto firstwizzardstep1;
				}
			}
			resettvpic();
		}

		char* msg = NULL;
		msg = readfiletomem("/etc/imageinfo", 0);
		textbox(_("Info"), _(msg), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 1100, 680, 30, 0);
		free(msg); msg = NULL;
		
		writevfd("");
		status.updatevfd = START;
		drawscreen(skin, 0, 0);
		addconfig("nofirstwizzard", "2");
	}
	else
	{
		// workaround, remove bootlogo on startup
		drawscreen(skin, 0, 0);

		//check servicestart
		if(serviceret != 21) // no message if startchannel empty
			servicecheckret(serviceret, 0);
	}

	//start auto shutdown thread
	addtimer(&checkshutdowntimer, START, 10000, -1, NULL, NULL, NULL);
	//start demo mode
	if(checkbox("WHITEBOX") == 1)
		addtimer(&demomodethread, START, 600000, -1, NULL, NULL, NULL);

	status.lastrcaction = time(NULL);
	ret = loadplugin();
	setosdtransparent(getskinconfigint("osdtransparent", NULL));
	ret = setsaturation(getconfigint("vs_saturation", NULL));
	ret = setbrightness(getconfigint("vs_brightness", NULL));
	ret = setcontrast(getconfigint("vs_contrast", NULL));
	ret = settint(getconfigint("vs_tint", NULL));
	
#ifndef SIMULATE
	//set skinentrys locked
	if(status.security == 0)
		setskinnodeslocked(1);
	else
		setskinnodeslocked(0);
#endif

	//start epg thread
	status.epgthread = addtimer(&epgthreadfunc, START, 1000, -1, NULL, NULL, NULL);
	//start record timer thread (on delay change you must change rectimer.h also)
	status.rectimerthread = addtimer(&checkrectimer, START, 1000, -1, NULL, NULL, NULL);
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
	//check kill net (security)
	addtimer(&ckeckkillnetthread, START, 1000, 1, NULL, NULL, NULL);
	//check old entrys and remove from mem
	addtimer(&oldentrythreadfunc, START, 60000 * 60, -1, NULL, NULL, NULL);

	//start newsletter
	startnewsletter(1);

	//thumb create thread
	startthumb(1);

	//start webserver
	starthttpd(1);
	
	//start rguid
	startrguid(1);

	//init the player
	playerinit(argc, argv);
	
	//start spinner thread
	addtimer(&checkspinner, START, 2000, -1, NULL, NULL, NULL);

	system(getconfig("skriptaftertv", NULL));

	//for atemio to unlock box with stick
	if(file_exist("/media/hdd/movie/titankey"))
	{
		char* cpuid = getcpuid();
		char* cmd = ostrcat("/media/hdd/movie/titankey ", cpuid, 0, 0);
		system(cmd);
		checkserial(cpuid);
		free(cmd); cmd = NULL;

		if(status.security == 1)
		{
			char* mac = getmacfromcmdline();
			if(mac != NULL)
			{
				cmd = ostrcat(cmd, cpuid, 1, 0);
				cmd = ostrcat(cmd, ",", 1, 0);
				cmd = ostrcat(cmd, mac, 1, 0);
				
				free(mac); mac = NULL;
			}
			else
				cmd = ostrcat(cmd, cpuid, 1, 0);

			writesys("/media/hdd/movie/codelist.txt", cmd, 3);
			textbox(_("Message"), _("Receiver successful unlocked"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 0, 0);
		}

		free(cpuid); cpuid = NULL;
		free(cmd); cmd = NULL;
	}

	//must called direct befor screeninfobar
	if(getconfigint("saverun", NULL) == 1)
	{
		ret = sigsetjmp(status.longjumpbuf, 1);
		if(ret != 0 && ret != 999)
		{
			err("set sigsegjump");
		}
	}
	
	//check free space in /var
	if(getfreespace("/var") / 1024 < 50) //200kb
		textbox(_("Message"), _("Free space in /var to little!\nThis can make problems!"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 7, 0);
	else
	{
		//check writeable in /var
		if(mkdir("/var/writetest", 0777) != 0 && errno != EEXIST)
		{
			if(textbox(_("Message"), _("/var not writeable!\nRepair it?"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 15, 0) == 1)
			{
				system("repairjffs2.sh var &"); //this script kills titan an reboot
				sleep(10);
			}
		}
		rmdir("/var/writetest");
	}

	//check free space in /mnt
	if(getfreespace("/mnt") / 1024 < 50) //200kb
		textbox(_("Message"), _("Free space in /mnt to little!\nThis can make problems!"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 7, 0);
	else
	{
		//check writeable in /mnt
		if(mkdir("/mnt/writetest", 0777) != 0 && errno != EEXIST)
		{
			if(textbox(_("Message"), _("/mnt not writeable!\nRepair it?"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 15, 0) == 1)
			{
				system("repairjffs2.sh mnt &"); //this script kills titan an reboot
				sleep(10);
			}
		}
		rmdir("/mnt/writetest");
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
	oshutdown(1, 1);
	return 0;

starterror:
	addscreen("<screen hspace=5 vspace=5 type=textbox name=starterror posx=center posy=center bordersize=2 bordercol=#ffffff fontsize=30 fontcol=#ffffff width=600 height=150/>", 0, 0);
	struct skin *starterror = getscreen("starterror");
	if(tmpstr == NULL)
		tmpstr = ostrcat(tmpstr, _("Unknown Error."), 1, 0);
	tmpstr = ostrcat(tmpstr, "\n", 1, 0);
	tmpstr = ostrcat(tmpstr, _("Automatic stop in 10 seconds."), 1, 0);
	changetext(starterror, _(tmpstr));
	drawscreen(starterror, 0, 0);
	sleep(10);
	free(tmpstr);
	return 100;
}