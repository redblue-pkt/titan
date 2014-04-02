#include "../titan/struct.h"
#include "../titan/debug.h"
#include "../titan/header.h"
#include "jsmn.h"
#include "facebook.h"

char pluginname[] = "Facebook";
char plugindesc[] = "Facebook";
char pluginpic[] = "%pluginpath%/facebook/plugin.png";

int pluginaktiv = 0;
int pluginversion = PLUGINVERSION;

//wird beim laden ausgefuehrt
void init(void)
{
	char* tmpstr = NULL;

	tmpstr = createpluginpath("/facebook/skin.xml", 0);
	readscreen(tmpstr, 242, 1);
	free(tmpstr); tmpstr = NULL;

	pluginaktiv = 1;
	debug(10, "Facebook Plugin loadet !!!");
}

//wird beim entladen ausgefuehrt
void deinit(void)
{
	delmarkedscreen(242);
	pluginaktiv = 0;
	debug(10, "Facebook Plugin removed !!!");
}

//wird in der Pluginverwaltung bzw Menue ausfeguehrt
void start(void)
{
	screenface();
}
