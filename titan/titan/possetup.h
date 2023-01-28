#ifndef POSSETUP_H
#define POSSETUP_H

void possearch(struct skin* possetup, struct dvbdev* dvbnode, int dir)
{
	int rcret = 0;
	uint16_t snr = 0;

	while(1)
	{
		rcret = waitrc(possetup, 500, 0);
		if(rcret == getrcconfigint("rcok", NULL))
		{
			fediseqcrotor(dvbnode, NULL, 0, 0);
			break;
		}

		if(dir == 0)
			fediseqcrotor(dvbnode, NULL, 1, 10);
		else
			fediseqcrotor(dvbnode, NULL, 1, 9);

		usleep(300000);
		drawscreen(possetup, 0, 0);
		snr = fereadsnr(status.aktservice->fedev);
		snr = (snr * 100) / 0xffff;
		if(snr > 50) break;
	}
}

void poschangebutton(int type, struct skin* b1, struct skin* b2, struct skin* b3, struct skin* b4)
{
	if(type == 0)
	{
		changetext(b1, _("move west"));
		changetext(b2, _("search west"));
		changetext(b3, _("search east"));
		changetext(b4, _("move east"));
	}
	else if(type == 1)
	{
		changetext(b1, NULL);
		changetext(b2, _("step west"));
		changetext(b3, _("step east"));
		changetext(b4, NULL);
	}
	else if(type == 2)
	{
		changetext(b1, _("limits off"));
		changetext(b2, _("limit west"));
		changetext(b3, _("limit east"));
		changetext(b4, _("limits on"));
	}
	else if(type == 3)
	{
		changetext(b1, NULL);
		changetext(b2, _("store position"));
		changetext(b3, _("goto position"));
		changetext(b4, NULL);
	}
	else if(type == 4)
	{
		changetext(b1, _("startposition"));
		changetext(b2, NULL);
		changetext(b3, NULL);
		changetext(b4, NULL);
	}
	else
	{
		changetext(b1, NULL);
		changetext(b2, NULL);
		changetext(b3, NULL);
		changetext(b4, NULL);
	}
}

void screenpossetup()
{
	int rcret = 0, i = 0;
	struct skin* possetup = getscreen("possetup");
	struct skin* listbox = getscreennode(possetup, "listbox");
	//struct skin* move = getscreennode(possetup, "move");
	//struct skin* finemove = getscreennode(possetup, "finemove");
	//struct skin* limit = getscreennode(possetup, "limit");
	struct skin* storagepos = getscreennode(possetup, "storagepos");
	//struct skin* goto0 = getscreennode(possetup, "goto0");
	struct skin* b1 = getscreennode(possetup, "b1");
	struct skin* b2 = getscreennode(possetup, "b2");
	struct skin* b3 = getscreennode(possetup, "b3");
	struct skin* b4 = getscreennode(possetup, "b4");
	struct skin* tmp = NULL;
	char* tmpnr = NULL;
	//TODO: select tuner
	struct dvbdev* dvbnode = status.aktservice->fedev;

	if(status.recording > 0 || (status.play != 2 && status.streaming > 0))
	{
		textbox(_("Message"), _("Position Setup is not allowed if record\nor stream is running !"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 0, 0);
		return;
	}

	listbox->aktline = 1;
	listbox->aktpage = -1;

	for(i = 1; i < 255; i++)
	{
		tmpnr = oitoa(i);
		addchoicebox(storagepos, tmpnr, tmpnr);
		free(tmpnr); tmpnr = NULL;
	}

	poschangebutton(0, b1, b2, b3, b4);

	drawscreen(possetup, 0, 0);
	addscreenrc(possetup, listbox);

	tmp = listbox->select;
	while(1)
	{
		if(rcret != RCTIMEOUT) addscreenrc(possetup, tmp);
		rcret = waitrc(possetup, 1000, 0);
		if(rcret == RCTIMEOUT)
		{
			drawscreen(possetup, 0, 0);
			continue;
		}
		tmp = listbox->select;

		if(listbox->select != NULL)
		{
			if(ostrcmp(listbox->select->name, "move") == 0)
				poschangebutton(0, b1, b2, b3, b4);
			if(ostrcmp(listbox->select->name, "finemove") == 0)
				poschangebutton(1, b1, b2, b3, b4);
			if(ostrcmp(listbox->select->name, "limit") == 0)
				poschangebutton(2, b1, b2, b3, b4);
			if(ostrcmp(listbox->select->name, "storagepos") == 0)
				poschangebutton(3, b1, b2, b3, b4);
			if(ostrcmp(listbox->select->name, "goto0") == 0)
				poschangebutton(4, b1, b2, b3, b4);
		}

		if(rcret == getrcconfigint("rcexit", NULL))
		{
			fediseqcrotor(dvbnode, NULL, 0, 0);
			break;
		}
		if(rcret == getrcconfigint("rcok", NULL))
			fediseqcrotor(dvbnode, NULL, 0, 0);
		if(listbox->select != NULL)
		{
			if(rcret == getrcconfigint("rcred", NULL))
			{
				if(ostrcmp(listbox->select->name, "move") == 0)
					fediseqcrotor(dvbnode, NULL, 0, 6);
				if(ostrcmp(listbox->select->name, "limit") == 0)
					fediseqcrotor(dvbnode, NULL, 0, 1);
				if(ostrcmp(listbox->select->name, "goto0") == 0)
					fediseqcrotor(dvbnode, NULL, 0, 8);
			}
			if(rcret == getrcconfigint("rcgreen", NULL))
			{
				if(ostrcmp(listbox->select->name, "move") == 0)
					possearch(possetup, dvbnode, 0);
				if(ostrcmp(listbox->select->name, "finemove") == 0)
					fediseqcrotor(dvbnode, NULL, 1, 10);
				if(ostrcmp(listbox->select->name, "limit") == 0)
					fediseqcrotor(dvbnode, NULL, 0, 4);
				if(ostrcmp(listbox->select->name, "storagepos") == 0)
				{
					if(listbox->select->ret != NULL)
					{
						int pos = atoi(listbox->select->ret);
						fediseqcrotor(dvbnode, NULL, pos, 7);
					}
				}
			}
			if(rcret == getrcconfigint("rcyellow", NULL))
			{
				if(ostrcmp(listbox->select->name, "move") == 0)
					possearch(possetup, dvbnode, 1);
				if(ostrcmp(listbox->select->name, "finemove") == 0)
					fediseqcrotor(dvbnode, NULL, 1, 9);
				if(ostrcmp(listbox->select->name, "limit") == 0)
					fediseqcrotor(dvbnode, NULL, 0, 3);
				if(ostrcmp(listbox->select->name, "storagepos") == 0)
				{
					if(listbox->select->ret != NULL)
					{
						int pos = atoi(listbox->select->ret);
						fediseqcrotor(dvbnode, NULL, pos, 8);
					}
				}
			}
			if(rcret == getrcconfigint("rcblue", NULL))
			{
				if(ostrcmp(listbox->select->name, "move") == 0)
					fediseqcrotor(dvbnode, NULL, 0, 5);
				if(ostrcmp(listbox->select->name, "limit") == 0)
					fediseqcrotor(dvbnode, NULL, 0, 2);
			}
		}

		drawscreen(possetup, 0, 0);
	}

	delownerrc(possetup);
	clearscreen(possetup);
}

#endif
