#include "../titan/struct.h"
#include "../titan/debug.h"
#include "../titan/header.h"
#include "networkbrowser_header.h"
#include "netlib/nbtscan.h"
#include "netlib/showmount.h"
#include "netlib/smbinfo.h"
#include "networkbrowser.h"
#include "networkbrowser_menulist.h"

char pluginname[] = "Networkbrowser";
char plugindesc[] = "Networkbrowser";
char pluginpic[] = "%pluginpath%/networkbrowser/plugin.png";

int pluginaktiv = 0;
struct skin* pluginmenu = NULL, *pluginmenu1 = NULL;
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

	child = getscreennode(networkmenu, "networkbrowser");
	if(child != NULL)
		changeinput(child, "start");
	pluginmenu = child;

	child = getscreennode(networkmenu, "sharesettings");
	if(child != NULL)
		changeinput(child, "start1");
	pluginmenu1 = child;

	tmpstr = ostrcat(getconfig("pluginpath", NULL), "/networkbrowser/skin.xml", 0, 0);
	readscreen(tmpstr, 100, 1);
	free(tmpstr); tmpstr = NULL;

	debug(70, "Networkbrowser Plugin loadet !!!");
}

//wird beim entladen ausgefuehrt
void deinit(void)
{
	pluginaktiv = 0;
	debug(70, "Networkbrowser Plugin removed !!!");
}


//wird in der Pluginverwaltung bzw Menue ausfeguehrt
void start(void)
{
	screennetworkbrowser();
}

void start1(void)
{
	screennetworkbrowser_readshare(0, NULL, NULL);
}
