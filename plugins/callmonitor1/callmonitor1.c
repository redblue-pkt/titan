#include "../../titan/struct.h"
#include "../../titan/debug.h"
#include "../../titan/header.h"
#include "callmon_main.h"


char pluginname[] = "CallMonitor";
char plugindesc[] = "Anzeigen von einghenden Anrufen an der Fritzbox";
char pluginpic[] = "%pluginpath%/callmonitor1/callmon1.png";
char pluginscript[] = "%pluginpath%/callmonitor1/fritzbox_msg_new.sh";

int pluginaktiv = 0;
//int pluginversion = PLUGINVERSION;
int pluginversion = 999999;

//wird beim laden ausgefuehrt
void init(void)
{
	pluginaktiv = 1;
	char* tmpstr = NULL;
	coldstart = 1;
	
	tmpstr = createpluginpath("/callmonitor1/skin.xml", 0);
	readscreen(tmpstr, 121, 1);
	free(tmpstr); tmpstr = NULL;
	
	CallmonStartThread = addtimer(&callmonstart, START, 10000, 1, NULL, NULL, NULL);
	addtimer(&callmontimeout, START, 10000, 1, NULL, NULL, NULL);
	
	debug(10, "CallMonitor1 Plugin loadet !!!");
}

//wird beim entladen ausgefuehrt
void deinit(void)
{
	char* tmpstr = NULL;

	tmpstr = createpluginpath("/callmonitor1/fritzbox_msg_new.sh", 0);
	tmpstr = ostrcat(tmpstr, " stop", 1, 0);
	system(tmpstr);
	pluginaktiv = 0;
	debug(10, "CallMonitor1 removed !!!");
	free(tmpstr); tmpstr = NULL;
	delmarkedscreen(121);
}

//wird in der Pluginverwaltung bzw Menue ausfeguehrt
void start(void)
{
	callmon_main();
}
