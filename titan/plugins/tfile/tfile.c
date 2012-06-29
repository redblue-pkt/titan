#include "../titan/struct.h"
#include "../titan/debug.h"
#include "../titan/header.h"
#include "tfile.h"

char pluginname[] = "File Manager";
char plugindesc[] = "File Manager";
char pluginpic[] = "%pluginpath%/tfile/plugin.png";

int pluginaktiv = 0;
struct skin* pluginmenu = NULL;
int pluginflag = 0;

//wird beim laden ausgefuehrt
void init(void)
{
	pluginaktiv = 1;
	char* tmpstr = NULL;
	
	tmpstr = ostrcat(getconfig("pluginpath", NULL), "/tfile/skin.xml", 0, 0);
	readscreen(tmpstr, 240, 1);
	free(tmpstr); tmpstr = NULL;
  
	debug(10, "File Manager Plugin loadet !!!");
}

//wird beim entladen ausgefuehrt
void deinit(void)
{
	delmarkedscreen(240);
	pluginaktiv = 0;
	debug(10, "File Manager Plugin removed !!!");
}

//wird in der Pluginverwaltung bzw Menue ausfeguehrt
void start(void)
{
	screentfile();
}
