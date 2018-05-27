#include "../titan/struct.h"
#include "../titan/debug.h"
#include "../titan/header.h"
#include "wm2018.h"

char pluginname[] = "WM2018";
char plugindesc[] = "WM2018";
char pluginpic[] = "%pluginpath%/wm2018/plugin.png";

int pluginaktiv = 0;
//int pluginversion = PLUGINVERSION;
int pluginversion = 999999; // = nopluginversion

//wird beim Laden ausgefuehrt
void init(void) {
	char* tmpstr = NULL;

	tmpstr = createpluginpath("/wm2018/skin.xml", 0);
	readscreen(tmpstr, 208, 1);
	free(tmpstr); tmpstr = NULL;

	pluginaktiv = 1;
	debug(10, "wm2018 Plugin loaded !!!");
}

//wird beim Entladen ausgefuehrt
void deinit(void) {
	delmarkedscreen(208);
	pluginaktiv = 0;
	debug(10, "wm2018 Plugin removed !!!");
}

//wird in der Pluginverwaltung bzw Menue ausgefuehrt
void start(void) {
	screenwm2018();
}
