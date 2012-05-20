#include "../titan/struct.h"
#include "../titan/debug.h"
#include "../titan/header.h"
#include "opera.h"

char pluginname[] = "Opera Browser";
char plugindesc[] = "Opera Browser";
char pluginpic[] = "%pluginpath%/opera/plugin.png";

int pluginaktiv = 0;

//wird beim laden ausgefuehrt
void init(void)
{
	pluginaktiv = 1;
	debug(10, "Opera Plugin loadet !!!");
}

//wird beim entladen ausgefuehrt
void deinit(void)
{
	pluginaktiv = 0;
	debug(10, "Opera Plugin removed !!!");
}

//wird in der Pluginverwaltung bzw Menue ausfeguehrt
void start(void)
{
	screenopera("http://www.orf.at");
}
