#include "../titan/struct.h"
#include "../titan/debug.h"
#include "../titan/header.h"
#include "usbreset.h"

char pluginname[] = "USB Reset";
char plugindesc[] = "USB Reset";
char pluginpic[] = "%pluginpath%/usbreset/plugin.png";

int pluginaktiv = 0;
int pluginversion = PLUGINVERSION;
struct skin* pluginmenu = NULL;
int pluginflag = 0;

//wird beim laden ausgefuehrt
void init(void)
{
	pluginaktiv = 1;
  
	debug(10, "Usbreset Plugin loadet !!!");
}

//wird beim entladen ausgefuehrt
void deinit(void)
{
	pluginaktiv = 0;
	debug(10, "Usbreset Plugin removed !!!");
}

//wird in der Pluginverwaltung bzw Menue ausfeguehrt
void start(void)
{
	screenusbreset();
}
