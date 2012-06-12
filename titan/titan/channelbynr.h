#ifndef CHANNELBYNR_H
#define CHANNELBYNR_H

void setchannelkeycode(struct skin* channelbynr, char* value)
{
	char* tmpstr = NULL;

	tmpstr = ostrcat(channelbynr->text, value, 0, 0);
	changetext(channelbynr, tmpstr);
	free(tmpstr); tmpstr = NULL;
	drawscreen(channelbynr, 0, 0);
}

void screenchannelbynr(int rcret)
{
	struct skin* channelbynr = getscreen("channelbynr");
	struct bouquet* bouquetnode = NULL;
	struct mainbouquet* mainbouquetnode = NULL;
	int count = 0, ret = 0;
	char* tmpstr = NULL;
	
	status.channelswitch = 1;

	status.rckey = rcret;
	drawscreen(channelbynr, 0, 0);

	while(1)
	{
		rcret = waitrc(channelbynr, 1000, 0);
		count++;

		if(rcret == getrcconfigint("rc0", NULL))
			setchannelkeycode(channelbynr, "0");
		if(rcret == getrcconfigint("rc1", NULL))
			setchannelkeycode(channelbynr, "1");
		if(rcret == getrcconfigint("rc2", NULL))
			setchannelkeycode(channelbynr, "2");
		if(rcret == getrcconfigint("rc3", NULL))
			setchannelkeycode(channelbynr, "3");
		if(rcret == getrcconfigint("rc4", NULL))
			setchannelkeycode(channelbynr, "4");
		if(rcret == getrcconfigint("rc5", NULL))
			setchannelkeycode(channelbynr, "5");
		if(rcret == getrcconfigint("rc6", NULL))
			setchannelkeycode(channelbynr, "6");
		if(rcret == getrcconfigint("rc7", NULL))
			setchannelkeycode(channelbynr, "7");
		if(rcret == getrcconfigint("rc8", NULL))
			setchannelkeycode(channelbynr, "8");
		if(rcret == getrcconfigint("rc9", NULL))
			setchannelkeycode(channelbynr, "9");
		if(rcret == getrcconfigint("rcexit", NULL)) break;
		if(rcret == RCTIMEOUT || count > 5 || rcret == getrcconfigint("rcok", NULL))
		{
			if(channelbynr->text != NULL)
				bouquetnode = getbouquetbynr(atoi(channelbynr->text));
			if(bouquetnode != NULL && bouquetnode->channel != NULL && bouquetnode->channel->servicetype == status.servicetype && channelnottunable(bouquetnode->channel) == 0)
			{
				clearscreen(channelbynr);
				drawscreen(skin, 0, 0);

				mainbouquetnode = getmainbouquetbybouquetpointer(bouquetnode);
				if(mainbouquetnode != NULL)
				{
					tmpstr = ostrcat(tmpstr, "(BOUQUET)-", 0, 0);
					tmpstr = ostrcat(tmpstr, mainbouquetnode->name, 1, 0);
				}

				ret = servicestart(bouquetnode->channel, tmpstr, NULL, 0);
				servicecheckret(ret, 0);
				free(tmpstr); tmpstr = NULL;
			}
			break;
		}
	}

	changetext(channelbynr, NULL);
	delownerrc(channelbynr);
	clearscreen(channelbynr);
	status.channelswitch = 0;
}

#endif
