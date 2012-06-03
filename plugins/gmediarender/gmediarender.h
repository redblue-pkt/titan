#ifndef GMEDIARENDER_H
#define GMEDIARENDER_H

extern struct skin* skin;

void screengmediarender()
{
	int rcret = -1, ret = 0;

	ret = servicestop(status.aktservice, 1, 0);
	if(ret == 1) return;

	drawscreen(skin, 0);

	//start renderer

	textbox(_("Message"), _("Wait for connect or press EXIT"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 7, 0);

	while(1)
	{
		rcret = waitrc(NULL, 0, 0);

		if(rcret == getrcconfigint("rcexit", NULL)) break;
		if(rcret == getrcconfigint("rcok", NULL)) break;
	}

	//stop renderer

	if(status.lastservice != NULL)
		servicestart(status.lastservice->channel, NULL, NULL, 0);
}

#endif
