#include "../titan/struct.h"
#include "../titan/debug.h"
#include "../titan/header.h"
#include "stock.h"

char pluginname[] = "Stock";
char plugindesc[] = "Stock";
char pluginpic[] = "%pluginpath%/stock/plugin.png";

int pluginaktiv = 0;
int pluginversion = PLUGINVERSION;
struct skin* pluginmenu = NULL;
int pluginflag = 0;

//wird beim laden ausgefuehrt
void init(void)
{
	pluginaktiv = 1;
	char* tmpstr = NULL;
	
	tmpstr = createpluginpath("/stock/skin.xml", 0);
	readscreen(tmpstr, 220, 1);
	free(tmpstr); tmpstr = NULL;
  
	debug(10, "Stock Plugin loadet !!!");
}

//wird beim entladen ausgefuehrt
void deinit(void)
{
	delmarkedscreen(220);
	pluginaktiv = 0;
	debug(10, "Stock Plugin removed !!!");
}

//wird in der Pluginverwaltung bzw Menue ausfeguehrt
void start(void)
{
	screenstock();
}
