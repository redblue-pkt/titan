#include "../titan/struct.h"
#include "../titan/debug.h"
#include "../titan/header.h"
#include "aafpanel_header.h"
#include "aafpanel_global.h"
#include "aafpanel_softcam.h"
#include "aafpanel_settings.h"
#include "aafpanel_system.h"
#include "aafpanel_extensions.h"
#include "aafpanel_infos.h"
#include "aafpanel_infos_sysinfo.h"
#include "aafpanel_settings_bluebutton.h"
#include "aafpanel_settings_redbutton.h"
#include "aafpanel_settings_autostart.h"
#include "aafpanel_settings_overclocking.h"
#include "aafpanel_settings_fancontrol.h"
#include "aafpanel_system_restore.h"
#include "aafpanel_system_backup.h"
#include "aafpanel_ipk.h"
#include "aafpanel_main.h"

char pluginname[] = "AAF Panel";
char plugindesc[] = "AAF Extensions";
char pluginpic[] = "%pluginpath%/aafpanel/plugin.png";

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

	child = getscreennode(mainmenu, "aafpanel");
	if(child != NULL)
		changeinput(child, "start");

	pluginmenu = child;
	tmpstr = ostrcat(getconfig("pluginpath", NULL), "/aafpanel/skin.xml", 0, 0);
	readscreen(tmpstr, 100, 1);
	free(tmpstr); tmpstr = NULL;
	debug(10, "AAF Plugin loadet !!!");
}

//wird beim entladen ausgefuehrt
void deinit(void)
{
	delmarkedscreen(100);
	pluginaktiv = 0;
	debug(10, "AAF Plugin removed !!!");
}


//wird in der Pluginverwaltung bzw Menue ausfeguehrt
void start(void)
{
	aafpanel_main();
}
