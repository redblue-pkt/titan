#include "../../titan/struct.h"
#include "../../titan/debug.h"
#include "../../titan/header.h"

char pluginname[] = "Permanent Time";
char plugindesc[] = "Extensions";
char pluginpic[] = "%pluginpath%/permtime/plugin.png";

int pluginaktiv = 0;
int pluginversion = 999999;
//struct skin* pluginmenu = NULL;
//int pluginflag = 1; //don't show the plugin in pluginmanager

struct stimerthread* permtimethread = NULL;

void permtime_thread()
{
	
	char* tmpstr = NULL;
	struct skin* framebuffer = getscreen("framebuffer");
	struct skin* permtime = getscreen("permtime");
	char* bg = NULL;
	
	int posx = getconfigint("permtime_posx", NULL);
	int posy = getconfigint("permtime_posy", NULL);
	
	if(getconfigint("permtime_trans", NULL) == 1)
		permtime->bgcol = -1;
 
	//if (posx == 0) posx = 3;
	//if (posy == 0) posy = 2;
	permtime->prozposx = 1;
	permtime->prozposy = 1;
	permtime->posx = posx;
	permtime->posy = posy;
	
	
	setnodeattr(permtime, framebuffer, 0);
	bg = savescreen(permtime);
	
	while (permtimethread->aktion != STOP) {
		tmpstr = gettime(NULL, "%H:%M"); 
		changetext(permtime, tmpstr);
		if(status.tuxtxt == 0)
			drawscreen(permtime, 0, 0);
		free(tmpstr); tmpstr = NULL;
		sleep(1); 
	}
 	restorescreen(bg, permtime);
	blitfb(0);
	permtimethread = NULL;
  return;
}

void permtime_main()
{
	if(permtimethread == NULL)
	{
		permtimethread = addtimer(&permtime_thread, START, 10000, 1, NULL, NULL, NULL);
		addconfig("permtime_plugin_running", "yes");
	}
	else
	{
		permtimethread->aktion = STOP;
		addconfig("permtime_plugin_running", "no");
	}
}	
			
//wird beim laden ausgefuehrt
void init(void)
{
	char* tmpstr = NULL;
	pluginaktiv = 1;
	
	tmpstr = createpluginpath("/permtime/skin.xml", 0);
	readscreen(tmpstr, 115, 1);
	free(tmpstr); tmpstr = NULL;
	tmpstr = getconfig("permtime_plugin_running", NULL);
	if(ostrcmp(tmpstr, "yes") == 0)
		permtime_main();
	tmpstr=NULL;
	
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
	char* tmpstr = NULL;
	int rcret;
 
	struct skin* pospermtime = getscreen("pospermtime");
 
	int posx = getconfigint("permtime_posx", NULL);
	int posy = getconfigint("permtime_posy", NULL);
	int trans = getconfigint("permtime_trans", NULL);
	int bgc = pospermtime->bgcol;
 
	//if (posx == 0) posx = 3;
	//if (posy == 0) posy = 2;
	pospermtime->prozposx = 1;
	pospermtime->prozposy = 1;
	
	tmpstr = getconfig("permtime_plugin_running", NULL);
	if(ostrcmp(tmpstr, "yes") == 0) {
		tmpstr=NULL;
		permtime_main();
	}
	else {
		tmpstr=NULL;
		while(1)
		{
			pospermtime->posx = posx;
			pospermtime->posy = posy;
			if(trans == 1)
				pospermtime->bgcol = -1;
			else
				pospermtime->bgcol = bgc;
			drawscreen(pospermtime, 0, 0);
			rcret = waitrc(pospermtime, 0, 0);
			clearscreen(pospermtime);
			if(rcret == getrcconfigint("rcup", NULL)) {
				posy = posy - 2;
				if(posy <= 0) posy = 0;
			}
			else if(rcret == getrcconfigint("rcdown", NULL)) {
				posy = posy + 2;
				if(posy >=96) posy = 96;
			}
			else if(rcret == getrcconfigint("rcleft", NULL)) {
				posx = posx - 3;
				if(posx <= 0) posx = 0;
			}
			else if(rcret == getrcconfigint("rcright", NULL)) {
				posx = posx + 3;
				if(posx >= 97) posx = 97;
			}
			else if(rcret == getrcconfigint("rc0", NULL)) {
				if(trans == 1)
					trans = 0;
				else
					trans = 1;
			}
			else if(rcret == getrcconfigint("rcok", NULL)) {
				addconfigint("permtime_posx", posx);
				addconfigint("permtime_posy", posy);
				addconfigint("permtime_trans", trans);
				break;
			}
		}
		permtime_main();
		
	}
}