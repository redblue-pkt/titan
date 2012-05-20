#include "../titan/struct.h"
#include "../titan/debug.h"
#include "../titan/header.h"
#include "panel_header.h"
#include "panel_global.h"
#include "panel_softcam.h"
#include "panel_settings.h"
#include "panel_system.h"
#include "panel_extensions.h"
#include "panel_infos.h"
#include "panel_infos_sysinfo.h"
#include "panel_settings_bluebutton.h"
#include "panel_settings_redbutton.h"
#include "panel_settings_autostart.h"
#include "panel_settings_overclocking.h"
#include "panel_settings_fancontrol.h"
#include "panel_system_restore.h"
#include "panel_system_backup.h"
#include "panel_feed.h"
#include "panel_main.h"

char pluginname[] = "Panel";
char plugindesc[] = "Extensions";
char pluginpic[] = "%pluginpath%/panel/plugin.png";

int pluginaktiv = 0;
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

	child = getscreennode(mainmenu, "panel");
	if(child != NULL)
		changeinput(child, "start");

	pluginmenu = child;
	tmpstr = ostrcat(getconfig("pluginpath", NULL), "/panel/skin.xml", 0, 0);
	readscreen(tmpstr, 107, 1);
	free(tmpstr); tmpstr = NULL;
	debug(10, "Panel Plugin loadet !!!");

	screenpanel_extensions_check ();
}

//wird beim entladen ausgefuehrt
void deinit(void)
{
	delmarkedscreen(107);
	pluginaktiv = 0;
	debug(10, "Panel Plugin removed !!!");
}


//wird in der Pluginverwaltung bzw Menue ausfeguehrt
void start(void)
{
	panel_main();
}
