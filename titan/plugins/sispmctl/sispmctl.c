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
}

//wird beim entladen ausgefuehrt
void deinit(void)
{
	sispmctl_stop(2);
	pluginaktiv = 0;
	debug(10, "sispmctl removed !!!");
	delmarkedscreen(281);
}

//wird in der Pluginverwaltung bzw Menue ausfeguehrt
void start(void)
{
	sispmctl_main();
}
