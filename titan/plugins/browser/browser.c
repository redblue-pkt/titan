#include "../titan/struct.h"
#include "../titan/debug.h"
#include "../titan/header.h"
#include "browser.h"

char pluginname[] = "Internet Browser";
char plugindesc[] = "Internet Browser";
char pluginpic[] = "%pluginpath%/browser/plugin.png";

int pluginaktiv = 0;
int pluginversion = PLUGINVERSION;

//wird beim laden ausgefuehrt
void init(void)
{
	pluginaktiv = 1;
	debug(10, "Browser Plugin loadet !!!");
}

//wird beim entladen ausgefuehrt
void deinit(void)
{
	pluginaktiv = 0;
	debug(10, "Browser Plugin removed !!!");
}

//wird in der Pluginverwaltung bzw Menue ausfeguehrt
void start(void)
{
	if(!file_exist("/var/swap/player"))   
		mkdir("/var/swap/player", 0777);

	screenbrowser();
}
