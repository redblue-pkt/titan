#include "../../titan/struct.h"
#include "../../titan/debug.h"
#include "../../titan/header.h"

char pluginname[] = "zapback (Werbezapper)";
char plugindesc[] = "Extensions";
char pluginpic[] = "%pluginpath%/zapback/plugin.png";

int pluginaktiv = 0;
int pluginversion = PLUGINVERSION;
//struct skin* pluginmenu = NULL;
//int pluginflag = 1; //don't show the plugin in pluginmanager

struct stimerthread* zapbackthread = NULL;
struct service* merkservice;
int waitminute = 0;

void akttomerk()
{
	if(status.aktservice->fedev != NULL && status.aktservice->fedev->type == FRONTENDDEVDUMMY) return;
	merkservice->fedev = status.aktservice->fedev;
	merkservice->dmxaudiodev = status.aktservice->dmxaudiodev;
	merkservice->dmxvideodev = status.aktservice->dmxvideodev;
	merkservice->dmxpcrdev = status.aktservice->dmxpcrdev;
	merkservice->dmxsubtitledev = status.aktservice->dmxsubtitledev;
	merkservice->audiodev = status.aktservice->audiodev;
	merkservice->videodev = status.aktservice->videodev;
	merkservice->transponder = status.aktservice->transponder;
	merkservice->channel = status.aktservice->channel;
	free(merkservice->channellist);
	merkservice->channellist = ostrcat(status.aktservice->channellist, NULL, 0, 0);
}

void zapback_thread()
{
	
	char* tmpstr = NULL;
	int merkminute = 0;
	int helpminute = 0;
	int countminute = 0;
	
	merkminute = atoi(gettime(NULL, "%M"));
	
	while(zapbackthread->aktion != STOP)
	{
		helpminute = atoi(gettime(NULL, "%M"));
		if(helpminute != merkminute)
		{
			countminute = countminute + 1;
			merkminute = helpminute;
		}
		if(countminute >= waitminute)
		{
			if(merkservice->channel != NULL)
			{
				tmpstr = ostrcat(merkservice->channellist, NULL, 0, 0);
				servicecheckret(servicestart(merkservice->channel, tmpstr, NULL, 0), 0);
				free(tmpstr); tmpstr = NULL;
				break;
			}
		}
		sleep(60);
	}
	zapbackthread = NULL;
	return;
}
			
void zapback_main()
{
	int rcret= 0;
	
	struct skin* zapback_main = getscreen("zapback_main");
	struct skin* minuten = getscreennode(zapback_main, "minuten");
//	struct skin* b1 = getscreennode(zapback_main, "b1");
	char *temp = NULL;
	
	if(zapbackthread != NULL)
	{
		if(textbox(_("Message"), _("Zapback bereits aktiv ... Stoppen?"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("exit", NULL), NULL, 0, NULL, 0, 600, 200, 5, 0) == 1)
			zapbackthread->aktion = STOP;
		else
			return;
	}
	
	waitminute = getconfigint("zapbackwait", NULL);
	if(waitminute == 0)
		waitminute = 9;
	temp = oitoa(waitminute);
	changetext(minuten, temp);
	free(temp); temp = NULL;
	while(1)
	{
		drawscreen(zapback_main, 0, 0);
		rcret = waitrc(zapback_main, 0, 0);
		
		if(rcret == getrcconfigint("rc1", NULL))
		{
			changetext(minuten, "1");
			waitminute = 1;
		}
		if(rcret == getrcconfigint("rc2", NULL))
		{
			changetext(minuten, "2");
			waitminute = 2;
		}
		if(rcret == getrcconfigint("rc3", NULL))
		{
			changetext(minuten, "3");
			waitminute = 3;
		}
		if(rcret == getrcconfigint("rc4", NULL))
		{
			changetext(minuten, "4");
			waitminute = 4;
		}
		if(rcret == getrcconfigint("rc5", NULL))
		{
			changetext(minuten, "5");
			waitminute = 5;
		}
		if(rcret == getrcconfigint("rc6", NULL))
		{
			changetext(minuten, "6");
			waitminute = 6;
		}
		if(rcret == getrcconfigint("rc7", NULL))
		{
			changetext(minuten, "7");
			waitminute = 7;
		}
		if(rcret == getrcconfigint("rc8", NULL))
		{
			changetext(minuten, "8");
			waitminute = 8;
		}
		if(rcret == getrcconfigint("rc9", NULL))
		{
			changetext(minuten, "9");
			waitminute = 9;
		}
		if(rcret == getrcconfigint("rcok", NULL))
		{
			addconfigint("zapbackwait", waitminute);
			akttomerk();
			zapbackthread = addtimer(&zapback_thread, START, 10000, 1, NULL, NULL, NULL);
			break;
		}
		if(rcret == getrcconfigint("rcexit", NULL))
		{
			break;
		}
	}
	clearscreen(zapback_main);
}	
			
//wird beim laden ausgefuehrt
void init(void)
{
	char* tmpstr = NULL;
	pluginaktiv = 1;
	tmpstr = createpluginpath("/zapback/skin.xml", 0);
	readscreen(tmpstr, 115, 1);
	free(tmpstr); tmpstr = NULL;
	debug(10, "zapback Plugin loadet !!!");
	merkservice = addservice(NULL);
}

//wird beim entladen ausgefuehrt
void deinit(void)
{
	free(merkservice->channellist);
	delservice(merkservice, 0);
	merkservice = NULL;
	delmarkedscreen(115);
	pluginaktiv = 0;
	debug(10, "zapback Time removed !!!");
}

//wird in der Pluginverwaltung bzw Menue ausfeguehrt
void start(void)
{
	zapback_main();
}
