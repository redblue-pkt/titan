#include "../titan/struct.h"
#include "../titan/debug.h"
#include "../titan/header.h"
#include "minisatip.h"

char pluginname[] = "minisatip";
char plugindesc[] = "minisatip Settings";
char pluginpic[] = "%pluginpath%/minisatip/plugin.png";

int pluginaktiv = 0;
int pluginversion = PLUGINVERSION;

//wird beim laden ausgefuehrt
void init(void)
{
	char* tmpstr = NULL;

	tmpstr = createpluginpath("/minisatip/skin.xml", 0);
	readscreen(tmpstr, 207, 1);
	free(tmpstr); tmpstr = NULL;

	pluginaktiv = 1;
	debug(10, "minisatip Plugin loaded !!!");
}

//wird beim entladen ausgefuehrt
void deinit(void)
{
	delmarkedscreen(207);
	pluginaktiv = 0;
	debug(10, "minisatip Plugin removed !!!");
}

//wird in der Pluginverwaltung bzw Menue ausfeguehrt
void start(void)
{
	screenminisatip();
}
