/*****************************************************/
/* this file is part of the tiTan / tiTanNIT Project */
/* and allowed only for use with this.               */
/*                                                   */
/* copyright by NIT                                  */
/*****************************************************/

#include "../titan/struct.h"
#include "../titan/debug.h"
#include "../titan/header.h"
#include "rgui.h"

char pluginname[] = "RGUI";
char plugindesc[] = "Remote GUI";
char pluginpic[] = "%pluginpath%/rgui/plugin.png";

int pluginaktiv = 0;
int pluginversion = PLUGINVERSION;

//wird beim laden ausgefuehrt
void init(void)
{
	char* tmpstr = NULL;

	tmpstr = createpluginpath("/rgui/skin.xml", 0);
	readscreen(tmpstr, 197, 1);
	free(tmpstr); tmpstr = NULL;

	pluginaktiv = 1;
	debug(10, "RGUI Plugin loadet !!!");
}

//wird beim entladen ausgefuehrt
void deinit(void)
{
	delmarkedscreen(197);
	pluginaktiv = 0;
	debug(10, "RGUI Plugin removed !!!");
}

//wird in der Pluginverwaltung bzw Menue ausfeguehrt
void start(void)
{
	screenrguimenu();
}
