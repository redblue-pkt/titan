#include "../../struct.h"
#include "../../debug.h"
#include "../../header.h"
#include "../../list.h"
#include "callmon_main.h"


char pluginname[] = "CallMonitor";
char plugindesc[] = "Anzeigen von einghenden Anrufen an der Fritzbox";
char pluginpic[] = "%pluginpath%/callmonitor1/callmon1.png";
char pluginscript[] = "%pluginpath%/callmonitor1/fritzbox_msg_new.sh";

int pluginaktiv = 0;
struct skin* pluginmenu = NULL;

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

	child = getscreennode(mainmenu, "callmon_main");
	if(child != NULL)
		changeinput(child, "start");

	pluginmenu = child;
	tmpstr = ostrcat(getconfig("pluginpath", NULL), "/callmonitor1/skin.xml", 0, 0);
	readscreen(tmpstr, 100, 1);
	free(tmpstr); tmpstr = NULL;
	debug(10, "CallMonitor1 Plugin loadet !!!");
	tmpstr = ostrcat(getconfig("pluginpath", NULL), "/callmonitor1/fritzbox_msg_new.sh start", 0, 0);
	system(tmpstr);
	free(tmpstr); tmpstr = NULL;
}

//wird beim entladen ausgefuehrt
void deinit(void)
{
	char* tmpstr = NULL;

	tmpstr = ostrcat(getconfig("pluginpath", NULL), "/callmonitor1/fritzbox_msg_new.sh stop", 0, 0);
	system(tmpstr);
	pluginaktiv = 0;
	debug(10, "CallMonitor1 removed !!!");
	free(tmpstr); tmpstr = NULL;
}

	//wird in der Pluginverwaltung bzw Menue ausfeguehrt
void start(void)
{
	callmon_main();
}
