#include "../titan/struct.h"
#include "../titan/debug.h"
#include "../titan/header.h"
#include "streaminfo.h"

char pluginname[] = "Streaminfo";
char plugindesc[] = "Streaminfo";
char pluginpic[] = "%pluginpath%/streaminfo/plugin.png";

int pluginaktiv = 0;
int pluginversion = PLUGINVERSION;
struct skin* pluginmenu = NULL;
int pluginflag = 0;

//wird beim laden ausgefuehrt
void init(void)
{
	pluginaktiv = 1;
	char* tmpstr = NULL;
	
	tmpstr = createpluginpath("/streaminfo/skin.xml", 0);
	readscreen(tmpstr, 225, 1);
	free(tmpstr); tmpstr = NULL;
  
	debug(10, "Streaminfo Plugin loadet !!!");
}

//wird beim entladen ausgefuehrt
void deinit(void)
{
	delmarkedscreen(225);
	pluginaktiv = 0;
	debug(10, "Streaminfo Plugin removed !!!");
}

//wird in der Pluginverwaltung bzw Menue ausfeguehrt
void start(void)
{
	screenstreaminfo();
}
