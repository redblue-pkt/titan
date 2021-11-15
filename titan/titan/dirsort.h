#ifndef DIRSORT_H
#define DIRSORT_H

int screendirsort()
{
	int tmpsort = 0;
	int rcret = 0;

	struct skin* dirsort = getscreen("dirsort");
	struct skin* listbox = getscreennode(dirsort, "listbox");
	struct skin* alpha = getscreennode(dirsort, "alpha");
	struct skin* reversealpha = getscreennode(dirsort, "reversealpha");
	struct skin* size = getscreennode(dirsort, "size");
	struct skin* reversesize = getscreennode(dirsort, "reversesize");
	struct skin* date = getscreennode(dirsort, "date");
	struct skin* reversedate = getscreennode(dirsort, "reversedate");
	
	addchoicebox(alpha, "0", "");
	addchoicebox(reversealpha, "1", "");
	addchoicebox(size, "2", "");
	addchoicebox(reversesize, "3", "");
	addchoicebox(date, "4", "");
	addchoicebox(reversedate, "5", "");

	drawscreen(dirsort, 0, 0);
	addscreenrc(dirsort, listbox);

	while(1)
	{
		rcret = waitrc(dirsort, 0, 0);

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
	
	delownerrc(dirsort);
	clearscreen(dirsort);
	return tmpsort;
}

#endif
