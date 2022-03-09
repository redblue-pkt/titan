#ifndef USBRESET_H
#define USBRESET_H

#define USBRESETPATH "/sys/bus/usb/drivers/usb"
#define USBRESETUNBIND USBRESETPATH"/unbind"
#define USBRESETBIND USBRESETPATH"/bind"

int addusbreset(char* dirname, struct menulist** mlist)
{
	int ret = 0;
	DIR *d;
	char* tmpstr = NULL;

	//Open the directory specified by dirname
	d = opendir(dirname);

	//Check it was opened
	if(! d)
	{
		perr("Cannot open directory %s", dirname);
		return 1;
	}

	while(1)
	{
		struct dirent* entry;
		int path_length;
		char path[PATH_MAX];

		snprintf(path, PATH_MAX, "%s", dirname);
		//Readdir gets subsequent entries from d
		entry = readdir(d);

		if(!entry) //There are no more entries in this directory, so break out of the while loop
			break;

		//See if entry is a subdirectory of d
		if(entry->d_type == DT_LNK)
		{
			//Check that the directory is not d or d's parent
			if(entry->d_name != NULL && ostrcmp(entry->d_name, ".") != 0 && ostrcmp(entry->d_name, "..") != 0)
			{
				path_length = snprintf(path, PATH_MAX, "%s/%s/product", dirname, entry->d_name);
				if(path_length >= PATH_MAX)
				{
					err("path length has got too long");
					ret = 1;
					break;
				}
				
				tmpstr = readsys(path, 1);
				if(tmpstr != NULL && path[1] == '-')
				{
					tmpstr = ostrcat(tmpstr, " (", 1, 0);
					tmpstr = ostrcat(tmpstr, entry->d_name, 1, 0);
					tmpstr = ostrcat(tmpstr, ")", 1, 0);
					struct menulist* m = addmenulist(mlist, tmpstr, NULL, NULL, 0, 0);
					changemenulistparam(m, entry->d_name, NULL, NULL, NULL);
				}
				free(tmpstr); tmpstr = NULL;
				
			}
		}
	}

	//After going through all the entries, close the directory
	if(d && closedir(d))
	{
		perr("Could not close %s", dirname);
		ret = 1;
	}

	return ret;
}

void screenusbreset()
{
	struct menulist* mlist = NULL, *mbox = NULL;	
	struct skin* load = getscreen("loading");
	
	addusbreset(USBRESETPATH, &mlist);
	
	mbox = menulistbox(mlist, NULL, _("USB Reset"), _("Choose your Devices from the following list"), NULL, NULL, 1, 0);
	
	if(mbox != NULL)
	{
		if(mbox->param != NULL)
		{
			drawscreen(load, 0, 0);
		
			writesys(USBRESETUNBIND, mbox->param, 0); //unbind
			sleep(2);
			writesys(USBRESETBIND, mbox->param, 0); //bind
			
			clearscreen(load);
		}
	}
	
	freemenulist(mlist, 1); mlist = NULL;
}

#endif
