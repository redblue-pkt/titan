#include "../titan/struct.h"
#include "../titan/debug.h"
#include "../titan/header.h"
#include "weather.h"

char pluginname[] = "Weather";
char plugindesc[] = "Weather";
char pluginpic[] = "%pluginpath%/weather/plugin.png";

int pluginaktiv = 0;
int pluginversion = PLUGINVERSION;

//wird beim laden ausgefuehrt
void init(void)
{
	char* tmpstr = NULL;

	tmpstr = createpluginpath("/weather/skin.xml", 0);
	readscreen(tmpstr, 208, 1);
	free(tmpstr); tmpstr = NULL;

	pluginaktiv = 1;
	debug(10, "Weather Plugin loadet !!!");
}

//wird beim entladen ausgefuehrt
void deinit(void)
{
	delmarkedscreen(208);
	pluginaktiv = 0;
	debug(10, "Weather Plugin removed !!!");
}

//wird in der Pluginverwaltung bzw Menue ausfeguehrt
void start(void)
{
	screenweather();
}
