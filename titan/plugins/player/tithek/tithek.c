#define HTTPAUTH "aXBrLUdaRmg6RkhaVkJHaG56ZnZFaEZERlRHenVpZjU2NzZ6aGpHVFVHQk5Iam0="
#define HTTPUSER "aUtzhFRTzuDFa"
#define HTTPPASS "JNHZbghnjuz"


#include "../titan/struct.h"
#include "../titan/debug.h"
#include "../titan/header.h"
#include "jsmn.h"
#include "tithek_header.h"
#include "tithek.h"
#include "tithek_settings.h"
#include "tithek_global.h"
//#include "solarmovie.h"
//#include "beeg.h"
#include "kinox.h"
//#include "myvideo.h"
//#include "movie4k.h"
//#include "mlehd.h"
#include "xvideos.h"
#include "ard.h"
//#include "zdf.h"
#include "putlocker.h"
#include "filenuke.h"
//#include "youtube.h"
#include "streamcloud.h"
#include "flashx.h"
#include "vidstream.h"
#include "xvidstage.h"
#include "nowvideo.h"
#include "movshare.h"
#include "movreel.h"
#include "novamov.h"
#include "divxstage.h"
#include "primeshare.h"
#include "faststream.h"
#include "played.h"
#include "videoweed.h"
#include "internetradio.h"
#include "internettv.h"
//#include "giga.h"
//#include "tectime.h"
#include "netzkino.h"
#include "filmon.h"
//#include "tvtoast.h"
#include "firedrive.h"
#include "shared.h"
#include "thefile.h"
#include "promptfile.h"
#include "letwatch.h"
#include "vidbull.h"
#include "vodlocker.h"
#include "vidto.h"
//#include "nowtv.h"
//#include "amazon.h"
#include "thevideo.h"
#include "mightyupload.h"
#include "cloudzilla.h"
#include "vivo.h"
#include "cricfree.h"
#include "streamlive.h"
//#include "movie2k.h"
//#include "nbastream.h"
//#include "nbaondemand.h"
#include "p2pcast.h"
#include "vidzi.h"
#include "vidag.h"

char pluginname[] = "Titan Mediathek";
char plugindesc[] = "Titan Mediathek";
char pluginpic[] = "%pluginpath%/tithek/plugin.png";

int pluginaktiv = 0;
int pluginversion = PLUGINVERSION;

//wird beim laden ausgefuehrt
void init(void)
{
	char* tmpstr = NULL;

	tmpstr = createpluginpath("/tithek/skin.xml", 0);
	readscreen(tmpstr, 195, 1);
	free(tmpstr); tmpstr = NULL;

	mkdir("/tmp/tithek", 777);
	pluginaktiv = 1;
	debug(10, "Tithek Plugin loadet !!!");
}

//wird beim entladen ausgefuehrt
void deinit(void)
{
	delmarkedscreen(195);
	delallfiles("/tmp/tithek", NULL);
	pluginaktiv = 0;
	debug(10, "Tithek Plugin removed !!!");
}

//wird in der Pluginverwaltung bzw Menue ausfeguehrt
void start(void)
{
	if(checkbox("ATEMIO510") == 0 && checkbox("ATEMIO7600") == 0 && checkbox("UFS912") == 0 && checkbox("ATEVIO700") == 0 && checkbox("ATEVIO7000") == 0 && checkbox("WHITEBOX") == 0 && checkbox("ATEMIO520") == 0 && checkbox("ATEMIO530") == 0)
	{
		if(file_exist("/var/bin/audio.elf") || file_exist("/var/swap/bin/audio.elf") || file_exist("/mnt/swapextension/bin/audio.elf"))
			textbox(_("Message"), _("Alternativ Audio Firmware not working korrekt with all videos (DTSDOWNMIX)!"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 1000, 200, 0, 0);
	}

	int aktplayerbuffersize = getconfigint("playerbuffersize", NULL);
	status.hangtime = 99999;
	tithekdownloadrun = 0;
	tithekdownloadcount = 0;
	tithekrun = 1;
	tithekexit = 0;
//	tithekmovie4k = 1;
	tithekkinox = 1;
//	titheksolarmovie = 1;
	//tithekmlehd = 1;
//	amazonlogin = 0;
	python = 0;
	ytbgdownload = 0;
	hlsbgdownload = 0;
	
//change markcolor
	long tmplistboxselectcol = status.listboxselectcol;
	status.listboxselectcol = convertcol("tithek_selectcol");

	if(getconfig("tithek_kinox_url", NULL) == NULL)
		addconfigtmp("tithek_kinox_url", "https://kinos.to");

	if(getconfig("tithek_vavoo_url", NULL) == NULL)
		addconfigtmp("tithek_vavoo_url", "https://www2.vavoo.to");

	if(getconfig("tithek_kinox_localhoster", NULL) == NULL)
		addconfigtmp("tithek_kinox_localhoster", "1");

	if(getconfig("tithek_kinox_pic", NULL) == NULL)
		addconfigtmp("tithek_kinox_pic", "0");

//	if(checkbox("WHITEBOX") == 1)
//		screentithekplay("http://openaaf.dyndns.tv/mediathek/mainmenu.crenova.list", "Operator - Mainmenu", 1);
//	else
//		screentithekplay("http://ks387398.kimsufi.com/mediathek/mainmenu.list", _("Tithek - Mainmenu"), 1);
//		screentithekmenu("http://openaaf.dyndns.tv/mediathek/mainmenu.list", _("Tithek - Mainmenu"), 1);
		screentithekplay("http://openaaf.dyndns.tv/mediathek/mainmenu.list", _("Tithek - Mainmenu"), 1);

	//reset markcolor  
	status.listboxselectcol = tmplistboxselectcol;
	
	tithekrun = 0;
	addconfigint("playerbuffersize", aktplayerbuffersize);
	status.hangtime = getconfigint("hangtime", NULL);
}
