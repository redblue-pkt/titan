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
	
	startextern = 0;
	if( ! file_exist("/mnt/plugin/ps01sispm"))
		sispmctl_start(2);
	else
		startextern = 1;
	
	if(file_exist("/mnt/plugin/pe01sispm"))
		stopextern = 1;
	else
		stopextern = 0;
	
	if(sispmctl_checkthread == NULL)
		sispmctl_checkthread = addtimer(&sispmctl_check_thread, START, 10000, 1, NULL, NULL, NULL);
}

//wird beim entladen ausgefuehrt
void deinit(void)
{
	if(sispmctl_checkthread != NULL)
		sispmctl_checkthread->aktion = STOP;
	if( ! file_exist("/mnt/plugin/pe01sispm"))
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
