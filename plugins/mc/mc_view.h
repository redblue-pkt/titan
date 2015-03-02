#ifndef MC_VIEW_H
#define MC_VIEW_H

int screenmc_view()
{
	int rcret = 0, tmpview = 0;
	struct skin* mc_view = getscreen("mc_view");
	struct skin* listbox = getscreennode(mc_view, "listbox");
	struct skin* defaults = getscreennode(mc_view, "defaults");
	struct skin* big = getscreennode(mc_view, "big");
	struct skin* fullcover = getscreennode(mc_view, "fullcover");
	struct skin* details = getscreennode(mc_view, "details");
	struct skin* small = getscreennode(mc_view, "small");
	struct skin* fast = getscreennode(mc_view, "fast");
	struct skin* tmp = NULL;
	
	addchoicebox(defaults, "0", (""));
	addchoicebox(big, "1", (""));
	addchoicebox(fullcover, "2", (""));
	addchoicebox(details, "3", (""));
	addchoicebox(small, "4", (""));
	addchoicebox(fast, "5", (""));

	drawscreen(mc_view, 0, 0);
	addscreenrc(mc_view, listbox);

	tmp = listbox->select;
	while(1)
	{
		addscreenrc(mc_view, tmp);
		rcret = waitrc(mc_view, 0, 0);
		tmp = listbox->select;

		if(rcret == getrcconfigint("rcexit", NULL)) break;
		if(rcret == getrcconfigint("rcok", NULL))
		{
			if(listbox->select != NULL)
			{
				if(!ostrncmp("defaults", listbox->select->name, 7))
					tmpview = 0;
				else if(!ostrncmp("big", listbox->select->name, 3))
					tmpview = 1;
				else if(!ostrncmp("fullcover", listbox->select->name, 9))
					tmpview = 2;
				else if(!ostrncmp("details", listbox->select->name, 7))
					tmpview = 3;
				else if(!ostrncmp("small", listbox->select->name, 4))
					tmpview = 4;
				else if(!ostrncmp("fast", listbox->select->name, 4))
					tmpview = 5;
			}
			break;
		}
	}
	
	delownerrc(mc_view);
	clearscreen(mc_view);
	return tmpview;
}

#endif
