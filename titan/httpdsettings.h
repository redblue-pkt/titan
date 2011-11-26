#ifndef HTTDPSETTINGS_H
#define HTTPDSETTINGS_H

void screenhttpdsettings()
{
	int rcret = 0;
	struct skin* httpdsettings = getscreen("httpdsettings");
	struct skin* listbox = getscreennode(httpdsettings, "listbox");
	struct skin* httpdstart = getscreennode(httpdsettings, "httpdstart");
	struct skin* user = getscreennode(httpdsettings, "user");
	struct skin* password = getscreennode(httpdsettings, "password");
	struct skin* httpdport = getscreennode(httpdsettings, "httpdport");
	struct skin* streamport = getscreennode(httpdsettings, "streamport");
	struct skin* tmp = NULL;
	char* tmpstr = NULL, *pos = NULL;

	addchoicebox(httpdstart, "0", _("no"));
        addchoicebox(httpdstart, "1", _("yes"));
        setchoiceboxselection(httpdstart, getconfig("httpdstart", NULL));

	tmpstr = ostrcat(getconfig("httpauth", NULL), NULL, 0, 0);
	if(tmpstr != NULL)
	{
		pos = strstr(tmpstr, ":");
		if(pos != NULL)
		{
			pos[0] = '\0';
			changeinput(user, tmpstr);
			pos++; 
			changeinput(password, pos);
		}
	}
	free(tmpstr); tmpstr = NULL;

        changemask(httpdport, "00000");
        changeinput(httpdport, getconfig("httpdport", NULL));

        changemask(streamport, "00000");
        changeinput(streamport, getconfig("streamport", NULL));

	drawscreen(httpdsettings, 0);
	addscreenrc(httpdsettings, listbox);

	tmp = listbox->select;
	while(1)
	{
		addscreenrc(httpdsettings, tmp);
		rcret = waitrc(httpdsettings, 0, 0);
		tmp = listbox->select;

		if(rcret == getrcconfigint("rcexit", NULL)) break;
		if(rcret == getrcconfigint("rcok", NULL))
		{
			if(httpdstart->ret != NULL)
			{
				if(ostrcmp(getconfig("httpdstart", NULL), httpdstart->ret) != 0)
				{
					addconfigscreen("httpdstart", httpdstart);
					if(ostrcmp(httpdstart->ret, "0") == 0)
						starthttpd(0);
					else
						starthttpd(1);
				}
			}
			if(user->ret != NULL && password->ret != NULL && strlen(user->ret) > 0 && strlen(password->ret) > 0)
			{
				tmpstr = ostrcat(tmpstr, user->ret, 1, 0);
				tmpstr = ostrcat(tmpstr, ":", 1, 0);
				tmpstr = ostrcat(tmpstr, password->ret, 1, 0);
				addconfig("httpauth", tmpstr);
				free(tmpstr); tmpstr = NULL;
			}
			else
				delconfig("httpauth");
			free(status.httpauth);
			status.httpauth = ostrcat(getconfig("httpauth", NULL), NULL, 0, 0);
			addconfigscreencheck("httpdport", httpdport, "80");
			addconfigscreencheck("streamport", streamport, "22222");
			break;
		}
	}

	delownerrc(httpdsettings);
	clearscreen(httpdsettings);
}

#endif
