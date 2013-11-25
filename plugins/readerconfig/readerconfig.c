#include "../titan/struct.h"
#include "../titan/debug.h"
#include "../titan/header.h"
#include "readerconfig.h"

char pluginname[] = "Reader Config";
char plugindesc[] = "Reader Config";
char pluginpic[] = "%pluginpath%/readerconfig/plugin.png";

int pluginaktiv = 0;
int pluginversion = PLUGINVERSION;
int pluginflag = 1; //don't show the plugin in pluginmanager

//wird beim laden ausgefuehrt
void init(void)
{
	char* tmpstr = NULL;

	tmpstr = createpluginpath("/readerconfig/skin.xml", 0);
	readscreen(tmpstr, 243, 1);
	free(tmpstr); tmpstr = NULL;

	pluginaktiv = 1;
	debug(10, "Reader Config Plugin loadet !!!");
}

//wird beim entladen ausgefuehrt
void deinit(void)
{
	delmarkedscreen(243);
	pluginaktiv = 0;
	debug(10, "Reader Config Plugin removed !!!");
}

//wird in der Pluginverwaltung bzw Menue ausfeguehrt
void start(void)
{
	screenoscam(NULL);
}
