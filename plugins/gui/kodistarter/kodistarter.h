#ifndef KODISTARTER_H
#define KODISTARTER_H

extern struct skin* skin;

void screenkodistarter()
{
	int ret = 0;
	char* cmd = NULL, *tmpstr = NULL;

	struct skin* kodistarter = getscreen("kodistarter");
	struct skin* guiname = getscreennode(kodistarter, "guiname");
	struct skin* install = getscreennode(kodistarter, "install");
	struct skin* loading = getscreen("loading");

	drawscreen(skin, 0, 0);
	drawscreen(kodistarter, 0, 0);

#ifdef OEBUILD         
	saveframebuffer();
#endif

	ret = servicestop(status.aktservice, 1, 0);
	if(ret == 1) return;
	setfbtransparent(255);

	status.sec = 0; //deaktivate spinner
	status.hangtime = 1;

	tmpstr = ostrcat(tmpstr, _("Kodi Gui Starter"), 1, 0);
	changetext(guiname, tmpstr);
	free(tmpstr), tmpstr = NULL;

//	tmpstr = ostrcat(_("Wait for install or press EXIT"), NULL, 0, 0);
	tmpstr = ostrcat(_("Wait for install"), NULL, 0, 0);
	changetext(install, tmpstr);
	free(tmpstr), tmpstr = NULL;

	changepic(kodistarter, "%pluginpath%/kodistarter/skin/background.jpg");					
	drawscreen(kodistarter, 0, 0);
	drawscreen(loading, 0, 0);

	cmd = ostrcat("kodi.sh", NULL, 0, 0);
	debug(10, "cmd: %s", cmd);
	system(cmd);
	free(cmd), cmd = NULL;

	resettvpic();

	setosdtransparent(getskinconfigint("osdtransparent", NULL));
	if(status.aktservice->channel != NULL)
		servicestart(status.aktservice->channel, NULL, NULL, 3);
	else
		servicestart(status.lastservice->channel, NULL, NULL, 0);
	flushrc(500);

//	delownerrc(kodistarter);
//	clearscreen(loading);
//	clearscreen(kodistarter);

	status.hangtime = getconfigint("hangtime", NULL);

#ifdef OEBUILD 
	restoreframebuffer();
#endif

}

#endif
