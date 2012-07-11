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
	pluginaktiv = 1;
	debug(10, "GmediaRender Plugin loadet !!!");
}

//wird beim entladen ausgefuehrt
void deinit(void)
{
	pluginaktiv = 0;
	debug(10, "GmediaRender Plugin removed !!!");
}

//wird in der Pluginverwaltung bzw Menue ausfeguehrt
void start(void)
{
	screengmediarender();
}
