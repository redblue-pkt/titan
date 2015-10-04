#include "../../titan/struct.h"
#include "../../titan/debug.h"
#include "../../titan/header.h"

#include "instar.h"


char pluginname[] = "IP-Kamera";
char plugindesc[] = "IP-Kamera zB. Firma INSTAR";
char pluginpic[] = "%pluginpath%/instar/instar.png";
//char pluginscript[] = "%pluginpath%/callmonitor1/fritzbox_msg_new.sh";

int pluginaktiv = 0;
int pluginversion = PLUGINVERSION;
//int pluginversion = 999999;

//wird beim laden ausgefuehrt
void init(void)
{
	char* tmpstr = NULL;
	pluginaktiv = 1;
	
	if(checkbox("ATEMIO5200") == 1)
			addconfig("pic_scale", "1");
	tmpstr = createpluginpath("/instar/skin.xml", 0);
	readscreen(tmpstr, 118, 1);
	free(tmpstr); tmpstr = NULL;
	if(checkbox("ATEMIO5200") == 1)
			addconfig("pic_scale", "0");

	debug(10, "INSTAR Plugin loaded !!!");
}

//wird beim entladen ausgefuehrt
void deinit(void)
{
	pluginaktiv = 0;
	debug(10, "INSTAR removed !!!");
	delmarkedscreen(118);
}

//wird in der Pluginverwaltung bzw Menue ausfeguehrt
void start(void)
{
	instar_main();
}
