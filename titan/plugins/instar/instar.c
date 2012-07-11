#include "../../titan/struct.h"
#include "../../titan/debug.h"
#include "../../titan/header.h"
#include "../../titan/list.h"

#include "instar.h"


char pluginname[] = "IP-Kammera";
char plugindesc[] = "IP-Kammera zB. Firma INSTAR";
char pluginpic[] = "%pluginpath%/instar/instar.png";
//char pluginscript[] = "%pluginpath%/callmonitor1/fritzbox_msg_new.sh";

int pluginaktiv = 0;
int pluginversion = PLUGINVERSION;

//wird beim laden ausgefuehrt
void init(void)
{
	pluginaktiv = 1;
	char* tmpstr = NULL;
	tmpstr = ostrcat(getconfig("pluginpath", NULL), "/instar/skin.xml", 0, 0);
	readscreen(tmpstr, 118, 1);

	debug(10, "INSTAR Plugin loadet !!!");
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
