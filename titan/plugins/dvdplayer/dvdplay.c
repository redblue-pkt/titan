#include "../titan/struct.h"
#include "../titan/debug.h"
#include "../titan/header.h"
#include "dvdplay.h"

char pluginname[] = "DVD Player";
char plugindesc[] = "DVD Player";
char pluginpic[] = "%pluginpath%/dvdplayer/plugin.png";

int pluginaktiv = 0;
int pluginversion = PLUGINVERSION;

//wird beim laden ausgefuehrt
void init(void)
{
	pluginaktiv = 1;
	debug(10, "DVD Player Plugin loadet !!!");
}

//wird beim entladen ausgefuehrt
void deinit(void)
{
	pluginaktiv = 0;
	debug(10, "DVD Player Plugin removed !!!");
}

//wird in der Pluginverwaltung bzw Menue ausfeguehrt
void start(void)
{
	screendvdplay(NULL, 0);
}
