#include "../titan/struct.h"
#include "../titan/debug.h"
#include "../titan/header.h"
#include "hbbtv.h"

char pluginname[] = "hbbtv Browser";
char plugindesc[] = "hbbtv Browser";
char pluginpic[] = "%pluginpath%/hbbtv/plugin.png";

int pluginaktiv = 0;

//wird beim laden ausgefuehrt
void init(void)
{
	pluginaktiv = 1;
	debug(10, "hbbtv Plugin loadet !!!");
}

//wird beim entladen ausgefuehrt
void deinit(void)
{
	pluginaktiv = 0;
	debug(10, "hbbtv Plugin removed !!!");
}

//wird in der Pluginverwaltung bzw Menue ausfeguehrt
void start(void)
{
//	screenopera("http://itv.mit-xperts.com/hbbtvtest/index.php");
//	screenopera("http://itv.ard.de/ardstart/index.html");
	if(aktservice->channel != NULL && aktservice->channel->hbbtvurl != NULL)
	{
		debug(788, "hbbtvurl=%s", aktservice->channel->hbbtvurl);
		screenopera(aktservice->channel->hbbtvurl);
	}
}
