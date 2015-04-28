/*****************************************************/
/* this file is part of the tiTan / tiTanNIT Project */
/* and allowed only for use with this.               */
/*                                                   */
/* copyright by NIT                                  */
/*****************************************************/

#include "../titan/struct.h"
#include "../titan/debug.h"
#include "../titan/header.h"

char pluginname[] = "Hello";
char plugindesc[] = "Hello World Demo";
char pluginpic[] = "%pluginpath%/hello/plugin.png";

int pluginaktiv = 0;
int pluginversion = PLUGINVERSION;


//wird beim laden ausgefuehrt
void init(void)
{
	pluginaktiv = 1;
	debug(10, "Hello Plugin loadet !!!");
}

//wird beim entladen ausgefuehrt
void deinit(void)
{
	pluginaktiv = 0;
	debug(10, "Hello Plugin removed !!!");
}

//wird in der Pluginverwaltung bzw Menue ausfeguehrt
void start(void)
{
	textbox(_("Message"), _("Hello World !"), _("EXIT"), getrcconfigint("rcexit", NULL), '\0', 0, '\0', 0, '\0', 0, 600, 200, 0, 0);
}
