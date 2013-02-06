/*
 * TopfieldVFD.c
 *
 *  Created on: Jul 30, 2011
 *      Author: topfi
 */

#include "../titan/struct.h"
#include "../titan/debug.h"
#include "../titan/header.h"

char pluginname[] = "TopfieldVFD";
char plugindesc[] = "Topfield specific functions of VFD";
char pluginpic[] = "%pluginpath%/TopfieldVFD/plugin.png";

#include "TopfieldVFD.h"

int pluginaktiv = 0;
int pluginversion = PLUGINVERSION;

struct stimerthread *TopfieldVFDstimerthread = NULL;

void updateTopfieldVfd()
{
	updateRec(0);
	updateProgress(0);
	updateHDD(0);
	updateUSB(0);
	updateEth(0);
	updateTimeshift(0);
}

//wird beim laden ausgefuehrt
void init(void)
{
	if(checkbox("TF7700"))
	{
		pluginaktiv = 1;
		debug(10, "TopfieldVFD loaded !!!");
		TopfieldVFDstimerthread = addtimer(&updateTopfieldVfd, START, 1000, -1, NULL, NULL, NULL);
	}
	else
	{
		debug(10, "TopfieldVFD Plugin: Not a Topfield TF7700PVR, plugin not started!");
	}
}

//wird beim entladen ausgefuehrt
void deinit(void)
{
	if(checkbox("TF7700"))
	{
		deltimer(TopfieldVFDstimerthread, 1);
		pluginaktiv = 0;
		debug(10, "TopfieldVFD removed !!!");
	}
	else
	{
		debug(10, "TopfieldVFD Plugin: Not a Topfield TF7700PVR, plugin not removed!");
	}
}

//wird in der Pluginverwaltung bzw Menue ausfefuehrt
void start(void)
{
	if(checkbox("TF7700"))
	{
		textbox(_("Message"), _("TopfieldVFD settings dialog not implemented yet!\n\nComing soon ..."), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), '\0', 0, '\0', 0, 600, 200, 0, 0);
	}
	else
	{
		textbox(_("Message"), _("TopfieldVFD plugin not available, wrong box!"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), '\0', 0, '\0', 0, 600, 200, 0, 0);
	}
}
