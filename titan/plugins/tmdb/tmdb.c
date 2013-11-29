#include "../titan/struct.h"
#include "../titan/debug.h"
#include "../titan/header.h"
#include "tmdb.h"

char pluginname[] = "TMDb";
char plugindesc[] = "Extensions";
char pluginpic[] = "%pluginpath%/tmdb/plugin.png";

int pluginaktiv = 0;
int pluginversion = PLUGINVERSION;
struct skin* pluginmenu = NULL;
int pluginflag = 1; //don't show the plugin in pluginmanager

//wird beim laden ausgefuehrt
void init(void)
{
	pluginaktiv = 1;
	char* tmpstr = NULL;
	struct skin* mainmenu = getscreen("mainmenu");
	struct skin* listbox = getscreennode(mainmenu, "listbox");
	struct skin* child = NULL;

	if(mainmenu == NULL || listbox == NULL)
		return;

	child = checkscreennode(mainmenu, "tmdb");
	if(child != NULL)
		changeinput(child, "start");

	pluginmenu = child;
	tmpstr = createpluginpath("/tmdb/skin.xml", 0);
	readscreen(tmpstr, 201, 1);
	free(tmpstr); tmpstr = NULL;
	debug(10, "TMDb Plugin loadet !!!");
}

//wird beim entladen ausgefuehrt
void deinit(void)
{
	delmarkedscreen(202);
	pluginaktiv = 0;
	debug(10, "TMDb Plugin removed !!!");
}

//wird in der Pluginverwaltung bzw Menue ausfeguehrt
void start(void)
{
	screentmdb(NULL, NULL, 0, NULL, NULL);
}
