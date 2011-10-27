#ifndef TIMESHIFT_H
#define TIMESHIFT_H

void timeshiftpause()
{
	int ret = 0;

	if(status.timeshift == 0)
	{
		ret = servicestop(status.aktservice, 0, 2);
		if(ret == 0)
		{
			ret = recordstart(status.aktservice->channel, -1, 0, RECTIMESHIFT, 0, NULL);
			if(ret != 0)
			{
				status.timeshift = 0;
				struct channel* tmpchnode = status.aktservice->channel;
				status.aktservice->channel = NULL;
				servicecheckret(servicestart(tmpchnode, NULL, 0), 0);
			}
			recordcheckret(NULL, ret, 6);
		}
	}
	else
		playerpausets();
}

//flag 0: stop from rcstop
//flag 1: stop from servicestop
void timeshiftstop(int flag)
{
	int i = 0, ret = 0;
	char* file = NULL;
	struct service* snode = getservice(RECORDTIMESHIFT, flag);

	playerstopts(1, flag);

	if(snode != NULL)
	{
		file = ostrcat(file, snode->recname, 1, 0);
		snode->recendtime = 1;

		ret = textbox(_("Message"), _("Should Timeshift File deleted ?"), _("EXIT"), getrcconfigint("rcexit", NULL), _("OK"), getrcconfigint("rcok", NULL), NULL, 0, NULL, 0, 600, 400, 10, 0);

		if(ret == 0 || ret == 2) unlink(file);
		free(file); file = NULL;
	}

	if(flag == 0)
	{
		while(status.timeshift == 1)
		{
			usleep(100000);
			i++; if(i > 20) break;
		}

		struct channel* tmpchnode = status.aktservice->channel;
		status.aktservice->channel = NULL;
		servicecheckret(servicestart(tmpchnode, NULL, 0), 0);
	}
}

void timeshiftplay()
{
	int ret = 1;
	struct service* snode = getservice(RECORDTIMESHIFT, 0);

	if(status.playing == 0)
	{
		if(snode != NULL) ret = playerstartts(snode->recname, 1);
		if(ret != 0)
		{
			textbox(_("Message"), _("Can't start timeshift play !"), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, NULL, 0, 600, 200, 0, 0);
		}
	}
	else
		playercontinuets();
}

#endif
