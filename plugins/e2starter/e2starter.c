#include "../titan/struct.h"
#include "../titan/debug.h"
#include "../titan/header.h"
#include "e2starter.h"

char pluginname[] = "E2Starter";
char plugindesc[] = "E2Starter";
char pluginpic[] = "%pluginpath%/e2starter/plugin.png";

int pluginaktiv = 0;
int pluginversion = PLUGINVERSION;

//wird beim laden ausgefuehrt
void init(void)
{
	char* tmpstr = NULL;

	tmpstr = createpluginpath("/e2starter/skin.xml", 0);
	readscreen(tmpstr, 279, 1);
	free(tmpstr); tmpstr = NULL;

	pluginaktiv = 1;
	debug(10, "E2Starter Plugin loadet !!!");
}

//wird beim entladen ausgefuehrt
void deinit(void)
{
	delmarkedscreen(279);
	pluginaktiv = 0;
	debug(10, "E2Starter Plugin removed !!!");
}

//wird in der Pluginverwaltung bzw Menue ausfeguehrt
void start(void)
{
	screene2starter();
}
