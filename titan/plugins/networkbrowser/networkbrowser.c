#include "../titan/struct.h"
#include "../titan/debug.h"
#include "../titan/header.h"
#include "netlib/nbtscan.h"
#include "netlib/showmount.h"
#include "netlib/smbinfo.h"
#include "networkbrowser.h"

char pluginname[] = "Networkbrowser";
char plugindesc[] = "Networkbrowser";
char pluginpic[] = "%pluginpath%/networkbrowser/plugin.png";

int pluginaktiv = 0;
int pluginversion = PLUGINVERSION;
struct skin* pluginmenu = NULL;
int pluginflag = 1; //don't show the plugin in pluginmanager

//wird beim laden ausgefuehrt
void init(void)
{
	pluginaktiv = 1;
	char* tmpstr = NULL;
	struct skin* networkmenu = getscreen("networkmenu");
	struct skin* listbox = getscreennode(networkmenu, "listbox");
	struct skin* child = NULL;

	if(networkmenu == NULL || listbox == NULL)
		return;

	child = checkscreennode(networkmenu, "networkbrowser");
	if(child != NULL)
		changeinput(child, "start");
	pluginmenu = child;

	tmpstr = createpluginpath("/networkbrowser/skin.xml", 0);
	readscreen(tmpstr, 100, 1);
	free(tmpstr); tmpstr = NULL;

	debug(70, "Networkbrowser Plugin loaded !!!");
}

//wird beim entladen ausgefuehrt
void deinit(void)
{
	delmarkedscreen(100);
	pluginaktiv = 0;
	debug(70, "Networkbrowser Plugin removed !!!");
}

//wird in der Pluginverwaltung bzw Menue ausfeguehrt
void start(void)
{
	screennetworkbrowser();
}
