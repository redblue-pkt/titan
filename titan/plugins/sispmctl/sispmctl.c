#include "../../titan/struct.h"
#include "../../titan/debug.h"
#include "../../titan/header.h"

#include "sispmctl.h"


char pluginname[] = "GEMBIRD Powermanager";
char plugindesc[] = "schalten der Steckdosenleiste von GEMBRIT";
char pluginpic[] = "%pluginpath%/sispmctl/sispmctl.png";

int pluginaktiv = 0;
//int pluginversion = PLUGINVERSION;
int pluginversion = 999999;

//wird beim laden ausgefuehrt
void init(void)
{
	char* tmpstr = NULL;
	pluginaktiv = 1;

	tmpstr = createpluginpath("/sispmctl/skin.xml", 0);
	readscreen(tmpstr, 281, 1);
	
	
	sispmctlconf = createpluginpath("/sispmctl/sispmctl.conf", 0);
	readconfig(sispmctlconf, myconfig);

	debug(10, "sispmctl Plugin loaded !!!");
	
	sispmctl_start(2);
	
	if(sispmctl_checkthread == NULL)
		sispmctl_checkthread = addtimer(&sispmctl_check_thread, START, 10000, 1, NULL, NULL, NULL);
}

//wird beim entladen ausgefuehrt
void deinit(void)
{
	if(sispmctl_checkthread != NULL)
		sispmctl_checkthread->aktion = STOP;
	sispmctl_stop(2);
	if(sispmctl_checkthread != NULL)
		sleep(2);
	pluginaktiv = 0;
	debug(10, "sispmctl removed !!!");
	delmarkedscreen(281);
}

//wird in der Pluginverwaltung bzw Menue ausfeguehrt
void start(void)
{
	sispmctl_main();
}
