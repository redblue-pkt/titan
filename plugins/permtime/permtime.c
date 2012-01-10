#include "../../titan/struct.h"
#include "../../titan/debug.h"
#include "../../titan/header.h"

char pluginname[] = "Permanent Time";
char plugindesc[] = "Extensions";
char pluginpic[] = "%pluginpath%/permtime/plugin.png";

int pluginaktiv = 0;
//struct skin* pluginmenu = NULL;
//int pluginflag = 1; //don't show the plugin in pluginmanager

struct stimerthread* permtimethread = NULL;

void permtime_thread()
{
	
	char* tmpstr = NULL;
	struct skin* framebuffer = getscreen("framebuffer");
	struct skin* permtime = getscreen("permtime");
	char* bg = NULL;
	
	setnodeattr(permtime, framebuffer);
	bg = savescreen(permtime);
	
	while (permtimethread->aktion != STOP) {
		tmpstr = gettime("%H:%M"); 
		changetext(permtime, tmpstr);
		drawscreen(permtime, 0);
		free(tmpstr); tmpstr = NULL;
		sleep(1); 
	}
 	restorescreen(bg, permtime);
	blitfb();
	permtimethread = NULL;
  return;
}

void permtime_main()
{
	if(permtimethread == NULL)
	{
		permtimethread = addtimer(&permtime_thread, START, 10000, 1, NULL, NULL, NULL);
	}
	else
	{
		permtimethread->aktion = STOP;
	}
}	
			
//wird beim laden ausgefuehrt
void init(void)
{
	char* tmpstr = NULL;
	pluginaktiv = 1;
	
	tmpstr = ostrcat(getconfig("pluginpath", NULL), "/permtime/skin.xml", 0, 0);
	readscreen(tmpstr, 115, 1);
	free(tmpstr); tmpstr = NULL;
	
	debug(10, "Permanent Time Plugin loadet !!!");
}

//wird beim entladen ausgefuehrt
void deinit(void)
{
	delmarkedscreen(115);
	pluginaktiv = 0;
	debug(10, "Permanent Time removed !!!");
}


//wird in der Pluginverwaltung bzw Menue ausfeguehrt
void start(void)
{
	permtime_main();
}
