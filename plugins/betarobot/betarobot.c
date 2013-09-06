#include "../titan/struct.h"
#include "../titan/debug.h"
#include "../titan/header.h"
#include "betarobot.h"

char pluginname[] = "Beta Robot";
char plugindesc[] = "Beta Robot";
char pluginpic[] = "%pluginpath%/betarobot/plugin.png";

int pluginaktiv = 0;
int pluginversion = PLUGINVERSION;
struct skin* pluginmenu = NULL;
int pluginflag = 0;

//wird beim laden ausgefuehrt
void init(void)
{
	pluginaktiv = 1;
  
	debug(10, "BetaRobot Plugin loadet !!!");
}

//wird beim entladen ausgefuehrt
void deinit(void)
{
	pluginaktiv = 0;
	debug(10, "BetaRobot Plugin removed !!!");
}

//wird in der Pluginverwaltung bzw Menue ausfeguehrt
void start(void)
{
	screenbetarobot();
}
