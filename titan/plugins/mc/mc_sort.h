#ifndef MC_SORT_H
#define MC_SORT_H

int screenmc_sort()
{
	int tmpsort = 0;
	int rcret = 0;

	struct skin* mc_sort = getscreen("mc_sort");	
	struct skin* listbox = getscreennode(mc_sort, "listbox");
	struct skin* alpha = getscreennode(mc_sort, "alpha");
	struct skin* reversealpha = getscreennode(mc_sort, "reversealpha");
	struct skin* size = getscreennode(mc_sort, "size");
	struct skin* reversesize = getscreennode(mc_sort, "reversesize");
	struct skin* date = getscreennode(mc_sort, "date");
	struct skin* reversedate = getscreennode(mc_sort, "reversedate");
	struct skin* tmp = NULL;
	
	addchoicebox(alpha, "0", _(""));
	addchoicebox(reversealpha, "1", _(""));
	addchoicebox(size, "2", _(""));
	addchoicebox(reversesize, "3", _(""));
	addchoicebox(date, "4", _(""));
	addchoicebox(reversedate, "5", _(""));

	drawscreen(mc_sort, 0, 0);
	addscreenrc(mc_sort, listbox);

	tmp = listbox->select;
	while(1)
	{
		addscreenrc(mc_sort, tmp);
		rcret = waitrc(mc_sort, 0, 0);
		tmp = listbox->select;

		if(rcret == getrcconfigint("rcexit", NULL)) break;
		if(rcret == getrcconfigint("rcok", NULL))
		{
			if(listbox->select != NULL)
			{
				if(!ostrncmp("alpha", listbox->select->name, 5))
					tmpsort = 0;
				else if(!ostrncmp("reversealpha", listbox->select->name, 12))
					tmpsort = 1;
				else if(!ostrncmp("size", listbox->select->name, 4))
					tmpsort = 2;
				else if(!ostrncmp("reversesize", listbox->select->name, 11))
					tmpsort = 3;
				else if(!ostrncmp("date", listbox->select->name, 4))
					tmpsort = 4;
				else if(!ostrncmp("reversedate", listbox->select->name, 11))
					tmpsort = 5;
			}
			break;
		}
	}
	
	delownerrc(mc_sort);
	clearscreen(mc_sort);
	return tmpsort;
}

#endif
