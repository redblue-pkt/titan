#include "../titan/struct.h"
#include "../titan/debug.h"
#include "../titan/header.h"
#include "tmc.h"

char pluginname[] = "Titan Media Center";
char plugindesc[] = "Titan Media Center";
char pluginpic[] = "%pluginpath%/tmc/plugin.png";

int pluginaktiv = 0;
int pluginversion = PLUGINVERSION;
//struct skin* pluginmenu = NULL;
//int pluginflag = 0;

//wird beim laden ausgefuehrt
void init(void)
{
	pluginaktiv = 1;
	char* tmpstr = NULL;
	
	tmpstr = createpluginpath("/tmc/skin.xml", 0);
	readscreen(tmpstr, 230, 1);
	free(tmpstr); tmpstr = NULL;
  
	debug(10, "Titan Media Center Plugin loadet !!!");
}

//wird beim entladen ausgefuehrt
void deinit(void)
{
	delmarkedscreen(230);
	pluginaktiv = 0;
	debug(10, "Titan Media Center Plugin removed !!!");
}

//wird in der Pluginverwaltung bzw Menue ausfeguehrt
void start(void)
{
	screentmcmenu();
}
