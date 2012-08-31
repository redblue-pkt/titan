#include "../titan/struct.h"
#include "../titan/debug.h"
#include "../titan/header.h"

extern struct fb* fb;

#include "../titan/player.h"
#include "mc_header.h"
#include "mc_global.h"
#include "mc_mounter.h"
#include "mc_imdb.h"

#include "mc_videoplayer.h"
#include "mc_audioplayer.h"
#include "mc_pictureplayer.h"
#include "mc_appletrailer.h"
#include "mc_myvideo.h"
#include "mc_vlcplayer.h"
#include "mc_dateibrowser.h"
#include "mc_internetbrowser.h"
#include "mc_mediathek.h"
#include "mc_radio.h"
#include "mc_webradio.h"
#include "mc_wetterinfo.h"
#include "mc_settings.h"
#include "mc_view.h"
#include "mc_sort.h"
#include "mc_main.h"
#include "mc_audioplayer_settings.h"
#include "mc_videoplayer_settings.h"
#include "mc_pictureplayer_settings.h"

char pluginname[] = "Media Center";
char plugindesc[] = "AAF Multi Mediacenter";
char pluginpic[] = "%pluginpath%/mediacenter/plugin.png";

int pluginaktiv = 0;
int pluginversion = PLUGINVERSION;
struct skin* pluginmenu = NULL;
int pluginflag = 1; //don't show the plugin in pluginmanager

//wird beim laden ausgefuehrt
void init(void)
{
	pluginaktiv = 1;
	struct skin* mainmenu = getscreen("mainmenu");
	struct skin* listbox = getscreennode(mainmenu, "listbox");
	struct skin* child = NULL;

	if(mainmenu == NULL || listbox == NULL)
		return;

	child = getscreennode(mainmenu, "mediacenter");
	if(child != NULL)
		changeinput(child, "start");

	pluginmenu = child;
	debug(10, "MC Plugin loadet !!!");
}

//wird beim entladen ausgefuehrt
void deinit(void)
{
	pluginaktiv = 0;
	debug(10, "MC Plugin removed !!!");
}

//wird in der Pluginverwaltung bzw Menue ausfeguehrt
void start(void)
{
	mc_main();
}
