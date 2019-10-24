#ifndef E2STARTER_H
#define E2STARTER_H

extern struct skin* skin;
struct fb *fb = NULL;

void screene2starter()
{
	int rcret = -1, ret = 0, rcsaverwait = 0, count = 0, screensaver_delay = 0, rcwait = 1000;
	char* cmd = NULL, *buf = NULL, *tmpstr = NULL, *tmpstr1 = NULL;
	int fifo = -1;
	struct skin* e2starter = getscreen("e2starter");
	struct skin* guiname = getscreennode(e2starter, "guiname");
	struct skin* install = getscreennode(e2starter, "install");
	struct skin* loading = getscreen("loading");
	
	ret = servicestop(status.aktservice, 1, 1);
	if(ret == 1) return;

	drawscreen(skin, 0, 0);
	drawscreen(e2starter, 0, 0);

	cmd = ostrcat(cmd, "/sbin/e2.sh", 1, 0);

	//start gui
	debug(10, "cmd: %s", cmd);
	system(cmd);
	free(cmd), cmd = NULL;

	tmpstr = ostrcat(tmpstr, _("Enigma2 Gui Starter"), 1, 0);
	changetext(guiname, tmpstr);
	free(tmpstr), tmpstr = NULL;

	tmpstr = ostrcat(_("Wait for install or press EXIT"), NULL, 0, 0);
	changetext(install, tmpstr);

	changepic(e2starter, "%pluginpath%/e2starter/skin/background.jpg");					
	drawscreen(e2starter, 0, 0);

	while(1)
	{
		rcret = waitrcext(NULL, rcwait, 0, 0);

 		if(rcret == getrcconfigint("rcexit", NULL)) break;
		if(rcret == getrcconfigint("rcok", NULL)) break;
	}

	//stop renderer
	system("killall -9 e2.sh");
		
	if(status.lastservice != NULL)
		servicestart(status.lastservice->channel, NULL, NULL, 0);

	free(tmpstr), tmpstr = NULL;

	delownerrc(e2starter);
	clearscreen(loading);
	clearscreen(e2starter);
}

#endif
