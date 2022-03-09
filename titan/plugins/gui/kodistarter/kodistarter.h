#ifndef KODISTARTER_H
#define KODISTARTER_H

extern struct skin* skin;
struct fb *fb = NULL;

void screenkodistarter()
{
	int rcret = -1, ret = 0, rcsaverwait = 0, count = 0, screensaver_delay = 0, rcwait = 1000;
	char* cmd = NULL, *buf = NULL, *tmpstr = NULL, *tmpstr1 = NULL;
	int fifo = -1;
	struct skin* kodistarter = getscreen("kodistarter");
	struct skin* guiname = getscreennode(kodistarter, "guiname");
	struct skin* install = getscreennode(kodistarter, "install");
	struct skin* loading = getscreen("loading");
	
	ret = servicestop(status.aktservice, 1, 1);
	if(ret == 1) return;

	drawscreen(skin, 0, 0);
	drawscreen(kodistarter, 0, 0);

	cmd = ostrcat(cmd, "/sbin/kodi.sh &", 1, 0);
	status.hangtime = 1;

	subtitlepause(1);

	int tmprcret = -1;

#ifdef MIPSEL
	delrc(getrcconfigint("rcvolup", NULL), NULL, NULL);
	delrc(getrcconfigint("rcvoldown", NULL), NULL, NULL);
#endif

	tmpstr = ostrcat(tmpstr, _("Kodi Gui Starter"), 1, 0);
	changetext(guiname, tmpstr);
	free(tmpstr), tmpstr = NULL;

	tmpstr = ostrcat(_("Wait for install or press EXIT"), NULL, 0, 0);
	changetext(install, tmpstr);

	changepic(kodistarter, "%pluginpath%/kodistarter/skin/background.jpg");					
	drawscreen(kodistarter, 0, 0);

	drawscreen(loading, 0, 0);

	//start gui
	debug(10, "cmd: %s", cmd);
	system(cmd);
	free(cmd), cmd = NULL;

//	disablemanualblit();

	while(1)
	{
		printf("screenkodistarter while\n");
		rcret = waitrcext(NULL, rcwait, 0, 0);
 		if(rcret == getrcconfigint("rcexit", NULL)) break;
	}

	//stop install script
	system("killall -9 kodi.sh");
	system("rm /tmp/kodistarter.sh");
	system("killall -9 kodistarter.sh");
		
	if(status.lastservice != NULL)
		servicestart(status.lastservice->channel, NULL, NULL, 0);

	free(tmpstr), tmpstr = NULL;

	delownerrc(kodistarter);
	clearscreen(loading);
	clearscreen(kodistarter);

#ifdef MIPSEL
//	enablemanualblit();
	addrc(getrcconfigint("rcvolup", NULL), screenvolumeup, NULL, NULL);
	addrc(getrcconfigint("rcvoldown", NULL), screenvolumedown, NULL, NULL);
#endif

	status.hangtime = getconfigint("hangtime", NULL);
}

#endif
