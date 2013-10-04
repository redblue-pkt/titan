#include "../../titan/struct.h"
#include "../../titan/debug.h"
#include "../../titan/header.h"

char pluginname[] = "KeyLock";
char plugindesc[] = "Extensions";
char pluginpic[] = "%pluginpath%/keylock/plugin.png";

int pluginaktiv = 0;
int pluginversion = PLUGINVERSION;
//struct skin* pluginmenu = NULL;
//int pluginflag = 1; //don't show the plugin in pluginmanager

void keylock_main()
{
	struct skin* framebuffer = getscreen("framebuffer");
	struct skin* keylock = getscreen("keylock");
	int t1 = 0, t2 = 0, rcret = 0;
	char* bg = NULL;
	
	setnodeattr(keylock, framebuffer, 0);
	bg = savescreen(keylock);
	drawscreen(keylock, 0, 0);
	
	while(1)
	{
		rcret = waitrc(keylock, 0, 0);
		if(rcret == getrcconfigint("rcred", NULL)) {
			if(t2 == 0) {
				t1 = t1 + 1;
				if(t1 < 3)
					continue;
			}
		}
		if(rcret == getrcconfigint("rcgreen", NULL)) {
			if(t1 == 2)
				break;
		}
		t1 = 0;
		t2 = 0;
	}
	
	restorescreen(bg, keylock);
	blitfb(0);
}
			
//wird beim laden ausgefuehrt
void init(void)
{
	char* tmpstr = NULL;
	pluginaktiv = 1;
	
	tmpstr = createpluginpath("/keylock/skin.xml", 0);
	readscreen(tmpstr, 117, 1);
	free(tmpstr); tmpstr = NULL;
	
	debug(10, "KeyLock Plugin loadet !!!");
}

//wird beim entladen ausgefuehrt
void deinit(void)
{
	delmarkedscreen(117);
	pluginaktiv = 0;
	debug(10, "KeyLock Plugin removed !!!");
}

//wird in der Pluginverwaltung bzw Menue ausfeguehrt
void start(void)
{
	keylock_main();
}
