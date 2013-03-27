#ifndef LOG_H
#define LOG_H

void screenlog()
{
	int rcret = -1, ret = 0;
	char* tmpstr = NULL;
	
	struct skin* log = getscreen("log");
	struct skin* text = getscreennode(log, "textbox");
	struct skin* b3 = getscreennode(log, "b3");
	struct skin* b4 = getscreennode(log, "b4");
	struct skin* load = getscreen("loading");

start:	
	tmpstr = readfiletomem(getconfig("tracelog", NULL), 0);
	changetext(text, tmpstr);
	
	if(tmpstr == NULL || strlen(tmpstr) < 1)
	{
		b3->hidden = YES;
		b4->hidden = YES;
	}
	else
	{
		b3->hidden = NO;
		b4->hidden = NO;
	}
	
	free(tmpstr); tmpstr = NULL;
		
	drawscreen(log, 0, 0);
	addscreenrc(log, text);
	
	while(1)
	{
		rcret = waitrc(log, 0, 0);
		
		if(rcret == getrcconfigint("rcexit", NULL)) break;
		if(rcret == getrcconfigint("rcok", NULL)) break;
		if(b3->hidden == NO && rcret == getrcconfigint("rcred", NULL))
		{
			drawscreen(load, 0, 0);
			ret = sendmail("mailserver.at", "m@x.at", "m@x.at", "Titan Error LOG", 25, getconfig("tracelog", NULL), NULL, 1000 * 1000, 5000 * 1000);
			if(ret == 0)
				textbox(_("Message"), "Mail send succesfull", _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 0, 0);
			else
				textbox(_("Message"), "Can't send Mail", _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 0, 0);
			clearscreen(load);
			drawscreen(log, 0, 0);
		}
		if(b4->hidden == NO && rcret == getrcconfigint("rcgreen", NULL))
		{
			unlink(getconfig("tracelog", NULL));
			goto start;
		}
	}

	changetext(text, NULL);
	delownerrc(log);
	clearscreen(log);;
}

#endif
