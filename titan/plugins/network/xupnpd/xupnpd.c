#include "../titan/struct.h"
#include "../titan/debug.h"
#include "../titan/header.h"
#include "xupnpd.h"

char pluginname[] = "xupnpd";
char plugindesc[] = "xupnpd Settings";
char pluginpic[] = "%pluginpath%/xupnpd/plugin.png";

int pluginaktiv = 0;
int pluginversion = PLUGINVERSION;

//wird beim laden ausgefuehrt
void init(void)
{
	char* tmpstr = NULL;

	tmpstr = createpluginpath("/xupnpd/skin.xml", 0);
	readscreen(tmpstr, 207, 1);
	free(tmpstr); tmpstr = NULL;

	pluginaktiv = 1;
	debug(10, "xupnpd Plugin loaded !!!");
}

//wird beim entladen ausgefuehrt
void deinit(void)
{
	delmarkedscreen(207);
	pluginaktiv = 0;
	debug(10, "xupnpd Plugin removed !!!");
}

//wird in der Pluginverwaltung bzw Menue ausfeguehrt
void start(void)
{
	screenxupnpd();
}
