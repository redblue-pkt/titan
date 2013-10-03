/*****************************************************/
/* this file is part of the tiTan / tiTanNIT Project */
/* and allowed only for use with this.               */
/*                                                   */
/* copyright by NIT                                  */
/*****************************************************/

#include "../titan/struct.h"
#include "../titan/debug.h"
#include "../titan/header.h"
#include "catcatch.h"

char pluginname[] = "Cat Catch";
char plugindesc[] = "Cat Catch";
char pluginpic[] = "%pluginpath%/catcatch/plugin.png";

int pluginaktiv = 0;
int pluginversion = PLUGINVERSION;

//wird beim laden ausgefuehrt
void init(void)
{
	char* tmpstr = NULL;

	tmpstr = createpluginpath("/catcatch/skin.xml", 0);
	readscreen(tmpstr, 241, 1);
	free(tmpstr); tmpstr = NULL;

	pluginaktiv = 1;
	debug(10, "Cat Catch Plugin loadet !!!");
}

//wird beim entladen ausgefuehrt
void deinit(void)
{
	delmarkedscreen(241);
	pluginaktiv = 0;
	debug(10, "Cat Catch Plugin removed !!!");
}

//wird in der Pluginverwaltung bzw Menue ausfeguehrt
void start(void)
{
	screencatcatch();
}
