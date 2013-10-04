#include "../titan/struct.h"
#include "../titan/debug.h"
#include "../titan/header.h"
#include "gmediarender.h"

char pluginname[] = "GmediaRender";
char plugindesc[] = "GmediaRender";
char pluginpic[] = "%pluginpath%/gmediarender/plugin.png";

int pluginaktiv = 0;
int pluginversion = PLUGINVERSION;

//wird beim laden ausgefuehrt
void init(void)
{
	char* tmpstr = NULL;

	tmpstr = createpluginpath("/gmediarender/skin.xml", 0);
	readscreen(tmpstr, 279, 1);
	free(tmpstr); tmpstr = NULL;

	pluginaktiv = 1;
	debug(10, "GmediaRender Plugin loadet !!!");
}

//wird beim entladen ausgefuehrt
void deinit(void)
{
	delmarkedscreen(279);
	pluginaktiv = 0;
	debug(10, "GmediaRender Plugin removed !!!");
}

//wird in der Pluginverwaltung bzw Menue ausfeguehrt
void start(void)
{
	screengmediarender();
}
