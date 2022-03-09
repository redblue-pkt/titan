#include "../titan/struct.h"
#include "../titan/debug.h"
#include "../titan/header.h"
#include "dlna.h"

char pluginname[] = "DLNA";
char plugindesc[] = "DLNA Settings";
char pluginpic[] = "%pluginpath%/dlna/plugin.png";

int pluginaktiv = 0;
int pluginversion = PLUGINVERSION;

//wird beim laden ausgefuehrt
void init(void)
{
	char* tmpstr = NULL;

	tmpstr = createpluginpath("/dlna/skin.xml", 0);
	readscreen(tmpstr, 207, 1);
	free(tmpstr); tmpstr = NULL;

	pluginaktiv = 1;
	debug(10, "DLNA Plugin loadet !!!");
}

//wird beim entladen ausgefuehrt
void deinit(void)
{
	delmarkedscreen(207);
	pluginaktiv = 0;
	debug(10, "DLNA Plugin removed !!!");
}

//wird in der Pluginverwaltung bzw Menue ausfeguehrt
void start(void)
{
	screendlna();
}
