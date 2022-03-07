#include "../titan/struct.h"
#include "../titan/debug.h"
#include "../titan/header.h"
#include "kodistarter.h"

char pluginname[] = "KodiStarter";
char plugindesc[] = "KodiStarter";
char pluginpic[] = "%pluginpath%/kodistarter/plugin.png";

int pluginaktiv = 0;
int pluginversion = PLUGINVERSION;

//wird beim laden ausgefuehrt
void init(void)
{
	char* tmpstr = NULL;

	tmpstr = createpluginpath("/kodistarter/skin.xml", 0);
	readscreen(tmpstr, 279, 1);
	free(tmpstr); tmpstr = NULL;

	pluginaktiv = 1;
	debug(10, "KodiStarter Plugin loadet !!!");
}

//wird beim entladen ausgefuehrt
void deinit(void)
{
	delmarkedscreen(279);
	pluginaktiv = 0;
	debug(10, "KodiStarter Plugin removed !!!");
}

//wird in der Pluginverwaltung bzw Menue ausfeguehrt
void start(void)
{
	screenkodistarter();
}
