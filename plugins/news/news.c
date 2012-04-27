#include "../titan/struct.h"
#include "../titan/debug.h"
#include "../titan/header.h"
#include "news.h"

char pluginname[] = "News";
char plugindesc[] = "News";
char pluginpic[] = "%pluginpath%/news/plugin.png";

int pluginaktiv = 0;

//wird beim laden ausgefuehrt
void init(void)
{
	char* tmpstr = NULL;

	tmpstr = ostrcat(getconfig("pluginpath", NULL), "/news/skin.xml", 0, 0);
	readscreen(tmpstr, 210, 1);
	free(tmpstr); tmpstr = NULL;

	pluginaktiv = 1;
	debug(10, "News Plugin loadet !!!");
}

//wird beim entladen ausgefuehrt
void deinit(void)
{
	delmarkedscreen(210);
	pluginaktiv = 0;
	debug(10, "News Plugin removed !!!");
}

//wird in der Pluginverwaltung bzw Menue ausfeguehrt
void start(void)
{
	screenwins3();
}
