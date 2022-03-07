#include "../titan/struct.h"
#include "../titan/debug.h"
#include "../titan/header.h"
#include "optimize.h"

char pluginname[] = "Optimize";
char plugindesc[] = "Optimize";
char pluginpic[] = "%pluginpath%/optimize/plugin.png";

int pluginaktiv = 0;
int pluginversion = PLUGINVERSION;

//wird beim laden ausgefuehrt
void init(void)
{
	char* tmpstr = NULL;

	tmpstr = createpluginpath("/optimize/skin.xml", 0);
	readscreen(tmpstr, 198, 1);
	free(tmpstr); tmpstr = NULL;

	pluginaktiv = 1;
	debug(10, "Optimize Plugin loadet !!!");
}

//wird beim entladen ausgefuehrt
void deinit(void)
{
	delmarkedscreen(198);
	pluginaktiv = 0;
	debug(10, "Optimize Plugin removed !!!");
}

//wird in der Pluginverwaltung bzw Menue ausfeguehrt
void start(void)
{
	screenoptimize();
}
