#include "../titan/struct.h"
#include "../titan/debug.h"
#include "../titan/header.h"
#include "oscam.h"

char pluginname[] = "Oscam Config";
char plugindesc[] = "Oscam Config";
char pluginpic[] = "%pluginpath%/oscam/plugin.png";

int pluginaktiv = 0;
int pluginversion = PLUGINVERSION;

//wird beim laden ausgefuehrt
void init(void)
{
	char* tmpstr = NULL;

	tmpstr = ostrcat(getconfig("pluginpath", NULL), "/oscam/skin.xml", 0, 0);
	readscreen(tmpstr, 243, 1);
	free(tmpstr); tmpstr = NULL;

	pluginaktiv = 1;
	debug(10, "Oscam Config Plugin loadet !!!");
}

//wird beim entladen ausgefuehrt
void deinit(void)
{
	delmarkedscreen(243);
	pluginaktiv = 0;
	debug(10, "Oscam Config Plugin removed !!!");
}

//wird in der Pluginverwaltung bzw Menue ausfeguehrt
void start(void)
{
	screenoscam();
}
