#include "struct.h"
#ifdef CONFIG_ION
#include "ion.h"
#endif
#include "header.h" 
#include "debug.h" 
#include "crc32.h"
#include "sock.h" 

#define SYSCODE 0UL
#define HTTPAUTH "aXBrLUdaRmg6RkhaVkJHaG56ZnZFaEZERlRHenVpZjU2NzZ6aGpHVFVHQk5Iam0="

#if defined(OVBUILD) || defined (OEBUILD)
#define IPKG_LIB
#include "../libipkg/config.h"
#include "../libipkg/libipkg.h"
#include "../libipkg/args.h"
#include "../libipkg/ipkg_cmd.h"
#endif

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
struct fb* oledskinfb = NULL;
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

#ifdef OEBUILD 
struct fb_var_screeninfo save_var_screeninfo;
#endif

int timeokw = 0;
int timeokz = 0;
int frameb1 = 0;
int vubox1 = 0;
int fbctuner = 0;
int ciplusrun = 0;
char* tunermerkmale = NULL;

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
#if defined(OVBUILD) || defined (OEBUILD)
#include "ipkg.h"
#endif
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
#include "security.h"
#include "dvbdev.h"
#include "rotorcalc.h"
#include "frontenddev.h"
#include "dmxdev.h"
#include "videodev.h"
#include "audiodev.h"
#include "cidev.h"
#include "stream.h"
#ifndef SIMULATE
#include "cadev.h"
#endif
#include "scdev.h"
#ifndef SIMULATE
#include "cacc.h"
#endif
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
#include "multiboot.h"
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
#include "settings_pluginbutton.h"
#ifndef SIMULATE
#include "bcm.h"
#endif
#include "settings_mediabutton.h"
#include "oled.h"
#ifdef MIPSEL
#include "cec.h"
#include "encoder.h"
#endif

#define TIMECODE "1667234340"
#if !defined(HAVE_HISILICON_ACCEL)
// mipsel start
/* Apparently, surfaces must be 64-byte aligned */
#define ACCEL_ALIGNMENT_SHIFT	6
#define ACCEL_ALIGNMENT_MASK	((1<<ACCEL_ALIGNMENT_SHIFT)-1)

#define ACCEL_DEBUG

//gAccel *gAccel::instance;
#define BCM_ACCEL
#endif

#ifdef HAVE_HISILICON_ACCEL 
extern int  dinobot_accel_init(void);
extern void dinobot_accel_close(void);
extern void dinobot_accel_blit(
		int src_addr, int src_width, int src_height, int src_stride, int src_format,
		int dst_addr, int dst_width, int dst_height, int dst_stride,
		int src_x, int src_y, int width, int height,
		int dst_x, int dst_y, int dwidth, int dheight,
		int pal_addr,int pal_size, int flags);
extern void dinobot_accel_fill(
		int dst_addr, int dst_width, int dst_height, int dst_stride,
		int x, int y, int width, int height,
		unsigned long color);
extern bool dinobot_accel_has_alphablending();
#endif

#ifndef SIMULATE
#ifdef BCM_ACCEL
extern int bcm_accel_init(void);
extern void bcm_accel_close(void);
extern void bcm_accel_blit(
		int src_addr, int src_width, int src_height, int src_stride, int src_format,
		int dst_addr, int dst_width, int dst_height, int dst_stride,
		int src_x, int src_y, int width, int height,
		int dst_x, int dst_y, int dwidth, int dheight,
		int pal_addr, int flags);
extern void bcm_accel_fill(
		int dst_addr, int dst_width, int dst_height, int dst_stride,
		int x, int y, int width, int height,
		unsigned long color);
extern bool bcm_accel_has_alphablending();
#endif
#endif
// mipsel end

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

	ret = checkshutdown(flag);
	if(ret == 1) return;
//	if(vubox1 ==1)
		ret = servicestop(status.aktservice, 1, 0);
//	else
//		ret = servicestop(status.aktservice, 1, 1);
	if(ret == 1) return;
	
	if(exitcode == 1)
		tmpstr = getconfig("userpowerofflogo", NULL);
	else if(exitcode == 2)
		tmpstr = getconfig("userrestartlogo", NULL);
	else if(exitcode == 3)
		tmpstr = getconfig("userguirestartlogo", NULL);
	
	if(tmpstr != NULL) {
		if(file_exist(tmpstr) == 0) 
			tmpstr = NULL;
	}	
	if(tmpstr == NULL) {	
		if(exitcode == 1) 
			tmpstr = getconfig("powerofflogo", NULL);
		else if(exitcode == 2)
			tmpstr = getconfig("restartlogo", NULL);
		else if(exitcode == 3)
			tmpstr = getconfig("guirestartlogo", NULL);
	}
			
	if(tmpstr != NULL)
	{
		changepic(logo, tmpstr);
		tmpstr = NULL;
		drawscreen(logo, 0, 0);
	}
	else
		clearfball();
		
	if(exitcode == 1)
		setcecstandby(1);

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
	
	//stop fan 
	if(checkchipset("BCM7424") == 1) //inihdp
		writesys("/proc/stb/fp/fan", "1", 1);
 		
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

	//Damit deinit ausgef\FChrt wird
	if(getplugin("GEMBIRD Powermanager") != NULL)
		delplugin("GEMBIRD Powermanager");
	
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

	printf("[%s] copyright by %s - version %s (%d)\n", PROGNAME, COPYRIGHT, OVERSION, PLUGINVERSION);
	printf("[%s] crontribut: %s\n", PROGNAME, CRONTRIBUT);

//workaround fuer time im neuen Image
timeokw = 1;
//#ifdef OEBUILD 
//	if(file_exist("/bin/fake-hwclock"))
//	{
		//printf("--> loesche fake-hwclock");
		//system("opkg remove fake-hwclock --force-depends");
		//oshutdown(2, 1);
//		timeokw = 0;
//		timeokz = 0;
//	}
//	if(getconfigint("nontpdate", NULL) == 0) 
//		system("(ntpdate -b ptbtime1.ptb.de; time=`date +%s`; echo -e `expr $time + 7200`  > /proc/stb/fp/rtc) &");
//#endif

	if(checkbox("VUSOLO2") == 1 || checkbox("VUDUO") == 1 || checkbox("VUDUO4K") == 1 || checkbox("VUZERO4K") == 1 || checkbox("VUZERO") == 1  || checkbox("VUUNO4K") == 1 || checkbox("VUUNO4KSE") == 1  || checkbox("VUULTIMO4K") == 1)
	{
		//Standart Service abarbeitung
		vubox1 = 1;
		//fbset 1920x1080-32 
		frameb1 = 1;
	}
	else
	{
		vubox1 = 0;
		frameb1 = 0;
	}
	printf("[titan] box=%s vubox1=%i frameb1=%i\n", getboxtype(), vubox1, frameb1);
	
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
	{
		printf("error: status.configfile ret=%d\n", ret);
		return 100;
	}
	//Workaround da ansonsten DVR4 nicht funktioniert (Treiberproblem)
	status.setdvr0 = 0;
	
	//start timer thread
	status.timerthreadaktion = START;
	pthread_attr_init(&status.timerthreadattr);
	pthread_attr_setstacksize(&status.timerthreadattr, 50000);
	pthread_attr_setdetachstate(&status.timerthreadattr, PTHREAD_CREATE_JOINABLE);
	ret = pthread_create(&status.timerthread, &status.timerthreadattr, timerthreadfunc, NULL);
	if(ret)
	{
		err("create timer thread");
		printf("error: create timer thread\n");
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
	
	fbctuner = 0;
	char* hypridtunerchoices = ostrcat(getconfig("hypridtunerchoices", NULL), NULL, 0, 0);
	if(hypridtunerchoices == NULL)
	{
		err("hypridtunerchoices NULL detect");
	}
	else
	{	
		tunermerkmale = readfiletomem(hypridtunerchoices, 1);
		if(tunermerkmale == NULL)
		{
			err("tunermerkmale NULL detect");
		}
		else
		{
			if(ostrstr(tunermerkmale, "45308X") != NULL)
				fbctuner = 1;
		}
		free(hypridtunerchoices); hypridtunerchoices=NULL;
	}

	//setze groesse Framebuffer
	if(checkchipset("BCM7424") == 1 || checkchipset("BCM7358") == 1 || checkchipset("BCM7362") == 1)
		setframebuffer(1280, 720, 1280, 1440, 32);
	if(frameb1 == 1)
	{
		printf("---> setframebuffer(1920, 1080, 1920, 2160, 32)\n");
		setframebuffer(1920, 1080, 1920, 2160, 32);
	}
	if(checkchipset("HI3798MV200") == 1)
	{
		printf("---> setframebuffer(1280, 720, 1280, 2880, 32)\n");
		setframebuffer(1280, 720, 1280, 2880, 32);
	}
	
	if(checkbox("VUDUO4K") == 1 || checkbox("VUUNO4KSE") == 1  || checkbox("VUULTIMO4K") == 1)
		initOLEDdream1();

	if(checkbox("UFS922") == 1)
		setfanspeed(-1, 0);
	if(checkchipset("BCM7424") == 1) //inihdp
	{
		if(getconfigint("fanmode", NULL) == 0)
			addconfig("fanmode", "3");
		writesys("/proc/stb/fp/fan", getconfig("fanmode", NULL), 1);
	}
	if(getconfigint("wol", NULL) == 1)
		system("echo enable > /proc/stb/fp/wol");
	else
		system("echo disable > /proc/stb/fp/wol");

#ifdef MIPSEL
	addtimer(&cecinit, START, 10000, 1, NULL, NULL, NULL);
	//cecinit();
#endif	
	system(getconfig("skriptbeforetv", NULL));
//	ret = setcecstandby(0); --- > jetzt in checkboxstart
	ret = setvideomode(getconfig("av_videomode", NULL), 0);
#ifndef MIPSEL
	ret = setaspect(getconfig("av_aspect", NULL));
#endif
	ret = setpolicy(getconfig("av_policy", NULL));
	ret = setcolorformat(getconfig("av_colorformat", NULL), 0);
	ret = setcolorformat(getconfig("av_colorformatscart", NULL), 1);
	ret = setaudiosource(getconfig("av_audiosource", NULL));
	ret = setac3(getconfig("av_ac3mode", NULL));

	if(checkrealbox("HD51") == 1 || checkrealbox("HD60") == 1 || checkrealbox("HD61") == 1 || checkbox("DM900") == 1 || checkbox("DM920") == 1 || checkbox("DM520") == 1 || checkbox("DM525") == 1)
	{
		if(isfile("/proc/stb/audio/ac3plus_choices"))
		{
			if(getconfig("av_ac3plusmode", NULL) == NULL)
				addconfig("av_ac3plusmode", "force_ac3");
			addconfig("av_can_ac3plusmode", "1");
			ret = setac3plus(getconfig("av_ac3plusmode", NULL));
		}
		else
			addconfig("av_can_ac3plusmode", "0");

		if(isfile("/proc/stb/audio/dtshd_choices"))
		{
			if(getconfig("av_dtshdmode", NULL) == NULL)
				addconfig("av_dtshdmode", "downmix");
			addconfig("av_can_dtshdmode", "1");
			ret = setdtshd(getconfig("av_dtshdmode", NULL));
		}
		else
			addconfig("av_can_dtshdmode", "0");
			
		if(isfile("/proc/stb/audio/wmapro_choices"))
		{
			if(getconfig("av_wmapromode", NULL) == NULL)
				addconfig("av_wmapromode", "downmix");
			addconfig("av_can_wmapromode", "1");
			ret = setwmapro(getconfig("av_wmapromode", NULL));
		}
		else
			addconfig("av_can_wmapromode", "0");	
		addconfig("mode3ddev", "/proc/stb/fb/primary/3d");     //24.02.17
	}
		
#ifdef MIPSEL
	ret = setaac(getconfig("av_aacmode", NULL));
	ret = setwss(getconfig("av_wssmode", NULL));
#endif
	ret = setpolicy(getconfig("av_policy", NULL));
	ret = setmode3d(getconfig("av_mode3d", NULL));
	ret = setvfdbrightness(getconfigint("vfdbrightness", NULL));
	ret = setaudiodelaybitstream(getconfig("audiodelaybitstream", NULL));
	ret = addinetworkall(NULL);
	
	ret = 1;
	if(file_exist("/var/etc/.usbimage"))
		ret = system("mount | grep titan");
  
#ifndef SIMULATE
	if(checkrealbox("HD51") == 1 || checkrealbox("HD60") == 1 || checkrealbox("HD61") == 1 || checkrealbox("DM900") == 1 || checkbox("DM920") == 1 || checkbox("DM520") == 1 || checkbox("DM525") == 1 || checkbox("VUSOLO2") == 1 || vubox1 == 1)
		ret = 0;

	// set pvr 1 = allowed , 0 = disabled
	status.pvr = 1;	
//	if(file_exist("/etc/.stable") && (checkbox("ATEMIO6000") == 1 || checkbox("ATEMIO6100") == 1 || checkbox("ATEMIO6200") == 1))
//		status.pvr = 0;

	int sos = getconfigint("sos", NULL);
  sos = 1; //workaround
  if(sos == 0)
	//if(getconfigint("sos", NULL) == 0)
	{
		if(ostrcmp(string_newline(gettimeinfo()), TIMECODE) == 1)
		{
			printf("error: 1\n");
//			destroy();
//			exit(100);
		}
		if(ostrcmp(string_newline(gettimeinfovar()), TIMECODE) == 1)
		{
			printf("error: 2\n");		
//			destroy();
//			exit(100);
		}
		if(checkreseller() != 0)
		{
			printf("error: 3\n");		
//			destroy();
//			exit(100);
		}
		if(ret > 0)
		{
			if(getsysinfo() != SYSCODE)
			{
				printf("error: 4\n");		
//				destroy();
//				exit(100);
			}
		}
		if(file_exist("/mnt/swapextensions/etc/.vnumber") == 1)
		{
			printf("error: 5\n");		
//			destroy();
//			exit(100);
		}
		if(ret > 0)
		{
			if(checkflash() != 0)
			{
				printf("error: 6\n");		
//				destroy();
//				exit(100);
			}
		}
		if(ret > 0)
		{
			if((checkchipset("BCM7424") == 1 || checkchipset("BCM7358") == 1 || checkchipset("BCM7362") == 1 || checkbox("UFS913") == 1 || checkbox("SPARK") == 1 || checkbox("SPARK7162") == 1 || checkbox("IPBOX91") == 1 || checkbox("IPBOX900") == 1 || checkbox("IPBOX910") == 1 || checkbox("IPBOX9000") == 1) && checkhighflash() != 0)
			{
				printf("error: 7\n");		
//				destroy();
//				exit(100);
			}
		}
		if(checkbox("UFS910") == 1 && checklowflash() != 0)
		{
			printf("error: 8\n");		
//			destroy();
//			exit(100);
		}
	
		char* cpuid = getcpuid();
		checkserial(cpuid);
		free(cpuid); cpuid = NULL;
	}
	else
	{
		status.security = 5;
		startnet();
		setskinnodeslocked(0);
	}
#endif

	if(file_exist("/mnt/config/dualboot.enigma2"))
	{
		printf("error: found /mnt/config/dualboot.enigma2\n");
		return 0;
	}

	ret = initfont();
	if(ret != 0)
	{
		printf("error: initfont\n");
		return 100;
	}
	ret = openfont(getconfig("fontfile1", NULL));
	if(ret != 0)
	{
		err("open fontfile1 font");
		printf("error: open fontfile1 font\n");
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


	status.volume = -1;
	setvol(getconfigint("vol", NULL));

	
	setlang(getconfig("lang", NULL));
	initlocale(getconfig("localepath", NULL));

//#ifdef MIPSEL
#ifdef BCM_ACCEL
	bcm_accel_init();
#endif
#ifdef HAVE_HISILICON_ACCEL
	dinobot_accel_init();
#endif
//#endif

	fb = openfb(getconfig("fbdev", NULL), 0);
	if(fb == NULL)
	{
		printf("error: openfb %s\n", getconfig("fbdev", NULL));
		return 100;
	}
	clearfball();
	enablemanualblit();
	
	tmpstr = getconfig("fb1dev", NULL);
	if(tmpstr != NULL)
		fb1 = openfb(tmpstr, 1);
	tmpstr = NULL;

#ifdef MIPSEL
	waitvsync();
	if(checkbox("DM7020HD") == 0 && checkbox("DM7020HDV2") == 0 && checkbox("VUSOLO2") == 0 && checkbox("DM900") == 0 && checkbox("DM920") == 0 && checkbox("DM520") == 0 && checkbox("DM525") == 0 && vubox1 == 1)
		setfbosd();
	if(checkrealbox("HD51") == 1 || checkrealbox("HD60") == 1 || checkrealbox("HD61") == 1 || checkbox("SF4008") == 1 || checkchipset("HI3798MV200") == 1)
	{
		setfbosdnull();
	}
	status.usedirectfb = 1;
#endif
#ifndef CONFIG_ION
	if(status.usedirectfb != 1)
	{
		skinfb = addfb(SKINFB, 0, getconfigint("skinfbwidth", NULL), getconfigint("skinfbheight", NULL), 4, fb->fd, fb->fb + fb->varfbsize, fb->fixfbsize, 0);
		if(skinfb != NULL)
		{
			ret = getfbsize(0);
			if(ret > 0)
				accelfb = addfb(ACCELFB, 0, ret / 4, 1, 4, fb->fd, skinfb->fb + skinfb->varfbsize, fb->fixfbsize, 0);
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
#ifndef MIPSEL
		ret = getfbsize(0);
		if(ret > 0) 
			accelfb = addfb(ACCELFB, 0, ret / 4, 1, 4, fb->fd, skinfb->fb + skinfb->varfbsize, fb->fixfbsize, fb->data_phys + fb->varfbsize);
	}
#else
		ret = getfbsize(0);
		printf("[titan.c] fbsize = %i\n", ret);
		if(ret > 0 && status.bcm == 1) 
		{
			skinfb = addfb(SKINFB, 0, getconfigint("skinfbwidth", NULL), getconfigint("skinfbheight", NULL), 4, fb->fd, fb->fb + fb->varfbsize, fb->fixfbsize, fb->data_phys + fb->varfbsize);
			if(skinfb != NULL)
			{
				status.usedirectfb = 0;
				ret = getfbsize(0);
				if(ret > 0)
					accelfb = addfb(ACCELFB, 0, ret / 4, 1, 4, fb->fd, skinfb->fb + skinfb->varfbsize, fb->fixfbsize, skinfb->data_phys + skinfb->varfbsize);
			}
		}
	}
#endif	
#else
	status.usedirectfb = 0;
	//SetMode();
#endif
	

	//if(lcdskinfb == NULL) {
		//mmapfb = malloc(4 * 320 * 240);
		//mmapfb = malloc(4 * 1024 * 768);
		//mmapfb = NULL;
		/* dev=999 ist LCD Buffer */
		//lcdskinfb = addfb("lcdskinfb", 999, 320, 240, 4, -1, mmapfb, 4 * 320 * 240);
		//lcdskinfb = addfb("lcdskinfb", 999, 1024, 768, 4, -1, mmapfb, 4 * 1024 * 768);
		// mmapfb = NULL;
	//}

	if(ostrcmp(string_newline(command("echo 'pipetest:ok' | cut -d':' -f2")), "ok") == 0)
		printf("[titan] skip Bash Pipe Bug detection\n");
	else
	{	
		printf("[titan] found Bash Pipe Bug, init Gui Restart\n");
		system("touch /tmp/.found.bash.pipe.bug.init.gui.restart");
		oshutdown(3, 1);
	}

	ret = createstartscreen();
	if(ret != 0)
	{
		printf("error: createstartscreen\n");
		return 100;
	}
	//from here we can use starterror screen
	ret = openrc();
	if(ret != 0)
	{
		//teste andere Geräte
		addconfig("rcdev", "/dev/input/event1");
		ret = openrc();
	  if(ret != 0)
	  {
	  	addconfig("rcdev", "/dev/input/event2");
			ret = openrc();
	  	if(ret != 0)
	  	{
	  		addconfig("rcdev", "/dev/input/event3");
				ret = openrc();
	  		if(ret != 0)
	  		{
					tmpstr = ostrcat(tmpstr, _("Error: open rc device !!"), 1, 0);
					goto starterror;
				}
			}
		}
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
	status.videodevs = videogetdev();
	if(status.videodevs < 1)
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
#ifdef MIPSEL
	if(checkchipset("BCM7424") == 1) //inihdp
		ret = encodergetdev();
#endif

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
	setvol(getconfigint("vol", NULL));

	//ciplus workaround.... muss als erstes starten
	
	if(getconfigint("ciplusrun", NULL) == 1)
	{
		printf("ci workaround start\n");
		startchannellist = getconfig("channellist", NULL);
		startserviceid = getconfigint("serviceid", NULL);
		starttransponderid = getconfigllu("transponderid", NULL);
		serviceret = servicestart(getchannel(getconfigint("ciplus_serviceid", NULL), getconfigllu("ciplus_transponderid", NULL)), getconfig("ciplus_channellist", NULL), NULL, 0);
		sleep(2); 
		addconfig("channellist", startchannellist);
		addconfigint("serviceid", startserviceid);
		addconfigllu("transponderid", starttransponderid);
		status.aktservice = addservice(NULL);
		status.lastservice = addservice(NULL);
		status.pipservice = addservice(NULL);
		printf("ci workaround1 ende\n");
	}

	//tune to channel
	if(status.servicetype == 0)
		serviceret = servicestart(getchannel(getconfigint("serviceid", NULL), getconfigllu("transponderid", NULL)), getconfig("channellist", NULL), NULL, 0);
	else
		serviceret = servicestart(getchannel(getconfigint("rserviceid", NULL), getconfigllu("rtransponderid", NULL)), getconfig("rchannellist", NULL),  NULL, 0);

	if(checkbox("IPBOX9000") == 1 && !file_exist("/tmp/.opticum9600.workaround"))
	{
		printf("opticum.workaround start\n");
		system("touch /tmp/.opticum9600.workaround");
		oshutdown(3, 1);
		printf("opticum.workaround end\n");
	}

	ret = readscreen(getconfig("skinfile", NULL), 0, 0);
	if(getconfig("keyskin_file", NULL) == NULL)
		ret = readscreen("/var/usr/local/share/titan/skin/default/keyskin.xml", 0, 0);
	else
		ret = readscreen(getconfig("keyskin_file", NULL), 0, 0);
	if(checkchipset("BCM7424") == 1 || checkbox("DM7020HD") == 1 || checkbox("DM7020HDV2") == 1 || checkbox("DM900") == 1 || checkbox("DM920") == 1 || checkbox("VUDUO4K") == 1 || checkbox("VUUNO4KSE") == 1  || checkbox("VUULTIMO4K") == 1)  //inihdp
	{
		if(getconfig("oledskin_path", NULL) == NULL)
			ret = readscreen("/var/usr/local/share/titan/skin/default/oledskin.xml", 0, 0);
		else
		{
			tmpstr = ostrcat(getconfig("oledskin_path", NULL),"/oledskin.xml", 0, 0);
			ret = readscreen(tmpstr, 0, 0);
			free(tmpstr);tmpstr=NULL;
		}
	}
	ret = readmainbouquet(getconfig("bouquetfile", NULL));
	ret = readallbouquet();
	ret = readmainplaylist(getconfig("playlistfile", NULL));
	ret = readallplaylist();
	ret = readrcmap(getconfig("rcmapfile", NULL));
	ret = readepgscanlist(getconfig("epgchannelfile", NULL));
	ret = settimezone(getconfig("timezone", NULL));
	ret = readextepgconfig(getconfig("extepgfile", NULL));

	//check to remove preinstalled tpk packages
	ret = tpkupdatepre();
	addtimer(&checkdate, START, 2000, -1, NULL, NULL, NULL);
	if(checkbox("ATEMIO510") == 0)
		addtimer(&updatevfd, START, 1000, -1, NULL, NULL, NULL);

//			setvideomode("720", 0);
//			changefbresolution("720", 0);
//			autoresolution();

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

	if(getconfig("remotecontrol", NULL) == NULL)
	{
		if(checkbox("ATEMIO520") == 1 || checkbox("ATEMIO530") == 1)
		{
			//autopo work
			char* tmptxt = NULL;
			tmptxt = ostrcat(tmptxt, _("RemoteControl Old Version"), 0, 0);
			tmptxt = ostrcat(tmptxt, _("RemoteControl Long Version"), 0, 0);
			free(tmptxt), tmptxt = NULL;

			struct menulist* mlist = NULL, *mbox = NULL;
			addmenulist(&mlist, "RemoteControl Old Version", "0", NULL, 0, 0);
			addmenulist(&mlist, "RemoteControl Long Version", "1", NULL, 0, 0);
			// remotecontrol try menulist
			mbox = menulistbox(mlist, "menulist", _("Select Your Remote Control"), _("Choose your Remotecontrol Model from the following list"), NULL, NULL, 3, 0);
			if(mbox != NULL)
			{
				debug(10, "mbox->name %s", mbox->name);
				debug(10, "mbox->text %s", mbox->text);
				addconfig("remotecontrol", mbox->text);
				writeallconfig(1);				
			}
		}
		if(checkbox("DM7020HD") == 1 || checkbox("DM7020HDV2") == 1 || checkbox("DM900") == 1 || checkbox("DM920") == 1 || checkbox("DM520") == 1 || checkbox("DM525") == 1)
		{
			addconfig("remotecontrol", "0");
			writeallconfig(1);	
		}			
	}

	if(getconfigint("autoscan", NULL) == 1)
	{
		status.updatevfd = PAUSE;
		resettvpic();
		screentunerconfig();
		resettvpic();
		screenscanconfig(1);
		resettvpic();
		writevfd("");
		status.updatevfd = START;
		drawscreen(skin, 0, 0);
		addconfig("autoscan", "0");
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

// fixt manual start 
	resettvpic();

	ret = setzapmode(getconfig("zapmode", NULL));

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
	videoApplySettings();
	
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

	// work
	resettvpic();

	char* cmd = NULL;
#ifdef SH4
	cmd = ostrcat("/media/hdd/movie/titankey.sh4", NULL, 0, 0);
#endif

#ifdef MIPSEL
	cmd = ostrcat("/media/hdd/movie/titankey.mipsel", NULL, 0, 0);
#endif

	//for atemio to unlock box with stick
	if(file_exist(cmd))
	{
		char* cpuid = getcpuid();
		cmd = ostrcat(cmd, " ", 1, 0);
		cmd = ostrcat(cmd, cpuid, 1, 0);

		system(cmd);
		checkserial(cpuid);
		free(cmd); cmd = NULL;

		if(status.security >= 1)
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
	}
	free(cmd); cmd = NULL;

	//must called direct befor screeninfobar
	if(getconfigint("saverun", NULL) == 1)
	{
		ret = sigsetjmp(status.longjumpbuf, 1);
		if(ret != 0 && ret != 999)
		{
			err("set sigsegjump");
		}
	}

	if(checkbox("UFS910") == 0 && checkbox("UFS922") == 0)
	{
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
					if(file_exist("/mnt/swapextensions/epg.dat"))
					{
						unlink("/mnt/swapextensions/epg.dat");
						system("sync");
						oshutdown(3, 0);
					}
					else
					{
						system("repairjffs2.sh mnt &"); //this script kills titan an reboot
						sleep(10);
					}
				}
			}
			rmdir("/mnt/writetest");
		}
	}

	addtimer(&guestthread, START, 1000, 1, NULL, NULL, NULL);
	addtimer(&setdate, START, 10000, 1, NULL, NULL, NULL);
	addtimer(&servicebouquetsthread, START, 1000, 1, NULL, (void*)1, NULL);

	startinternreader(1);
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
	printf("error: starterror=%s\n", tmpstr);
	free(tmpstr);
	return 100;
}
