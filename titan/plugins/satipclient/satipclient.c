#include "../titan/struct.h"
#include "../titan/debug.h"
#include "../titan/header.h"
#include "satipclient.h"

char pluginname[] = "SAT-IP Client";
char plugindesc[] = "SAT-IP Client Settings";
char pluginpic[] = "%pluginpath%/satipclient/plugin.png";

int pluginaktiv = 0;
int pluginversion = PLUGINVERSION;

//wird beim laden ausgefuehrt
void init(void)
{
	char* tmpstr = NULL;

	tmpstr = createpluginpath("/satipclient/skin.xml", 0);
	readscreen(tmpstr, 207, 1);
	free(tmpstr); tmpstr = NULL;

	pluginaktiv = 1;
	debug(10, "satipclient Plugin loaded !!!");
}

//wird beim entladen ausgefuehrt
void deinit(void)
{
	delmarkedscreen(207);
	pluginaktiv = 0;
	debug(10, "satipclient Plugin removed !!!");
}

//wird in der Pluginverwaltung bzw Menue ausfeguehrt
void start(void)
{
	screensatipclient();
}
