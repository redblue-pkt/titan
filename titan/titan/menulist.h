#ifndef MENULIST_H
#define MENULIST_H

// showpng = 0 (no icon)
// showpng = 1 (smal icon)

//flag 1: rcgreen = subchannel
char* menulistbox(char* defaultstr, char* str, char* skinname, char* skintitle, char* skinpath, int showpng, int flag)
{
	debug(1000, "in");
	char* skinname1 = NULL;
	char* skinpath1 = NULL;
	char* tmpskinpath = NULL;
	int rcret = 0, tmpscreencalc = 0, fromthread = 0;
	struct skin* framebuffer = getscreen("framebuffer");
        char* bg = NULL;

	if(str == NULL) return 0;

	if(pthread_self() != status.mainthread)
		fromthread = 1;

	if(skinname == NULL)
	{
		debug(60, "skinname default=menulist");
		skinname1 = ostrcat(skinname, "menulist", 1, 0);
	}
	else
	{
		debug(60, "skinname changed=%s", skinname);
		skinname1 = ostrcat(skinname, "", 0, 0);
	}

	if(skinpath == NULL)
	{
		debug(60, "skinpath default=skin/");
		skinpath1 = ostrcat(skinpath, "skin/", 1, 0);
	}
	else
	{
		debug(60, "skinpath changed=%s", skinpath);
		skinpath1 = ostrcat(skinpath, "", 0, 0);
	}

	struct skin* screen = getscreen(skinname1);
	struct skin* listbox = getscreennode(screen, "listbox");
	
	listbox->aktpage = -1;
	listbox->aktline = 1;

	if(skintitle != NULL)
	{
		debug(60, "skintitle changed=%s", skintitle);
		changetitle(screen, _(skintitle));
	}

	struct skin* tmp = NULL;
	char* tmpstr = NULL;
	char* defaultdir = NULL;
	char* tmpsplit = NULL;
	char* tmpck = NULL;
	char* tmpstr1 = NULL;
	char* tmpstr4 = NULL;
	char* tmpstr5 = NULL;
	char* tmppng = NULL;

	struct splitstr* ret1 = NULL;
	struct splitstr* ret2 = NULL;
	struct splitstr* ret3 = NULL;
	int count = 0;
	int i = 0;
	ret1 = strsplit(str, "\n", &count);

	for(i = 0; i < count; i++)
	{
		int count2 = 0;
		tmpsplit = ostrcat(tmpsplit, (&ret1[i])->part, 1, 0);
		free(ret2); ret2 = NULL;
		ret2 = strsplit(tmpsplit, "-", &count2);

		tmppng = ostrcat("", (&ret1[i])->part, 0, 0);

		stringreplacechar(tmppng, ' ', '_');
		stringreplacechar(tmppng, ':', '_');
		string_tolower(tmppng);
		
		tmp = addlistbox(screen, listbox, tmp, 1);
		
		if(tmp != NULL)
		{
			changetext(tmp, _((&ret1[i])->part));
			changename(tmp, (&ret1[i])->part);

			tmp->del = 1;
			if(showpng == 1) 
			{
				tmp->textposx = 120;
				tmp->height = 50;
//				tmp->fontsize = 30;
				tmp->valign = convertxmlentry("middle", 0);
				tmp->hspace = 5;
				debug(60, "showpng changed=%d", showpng);
			}
			else
			{
				debug(60, "showpng default=%d", showpng);
			}

			if(ostrcmp((&ret1[i])->part, "\t") == 0)
				tmp->deaktivcol = convertcol("deaktivcol");
			else
			{
				if(showpng == 1)
				{
					if(string_find("%pluginpath%",skinpath1)){
						struct splitstr* ret6 = NULL;
						int count6 = 0;
						char* tmpstr6 = NULL;
						tmpstr6 = ostrcat("", skinpath1, 0, 0);
						ret6 = strsplit(tmpstr6, "%", &count6);

						defaultdir = ostrcat(getconfig("skinpath", NULL), "/skin/", 0, 0);
						defaultdir = ostrcat(defaultdir, tmppng, 1, 0);
						defaultdir = ostrcat(defaultdir, ".png", 1, 0);
						debug(60, "defaultdir1 %s", defaultdir);
						if(!file_exist(defaultdir))
						{
							defaultdir = ostrcat(getconfig("pluginpath", NULL), (&ret6[1])->part, 0, 0);
							defaultdir = ostrcat(defaultdir, tmppng, 1, 0);
							defaultdir = ostrcat(defaultdir, ".png", 1, 0);
						}

						free(ret6), ret6 = NULL;
						debug(60, "defaultdir2 %s", defaultdir);
					}
					else
					{
						defaultdir = ostrcat(getconfig("skinpath", NULL), "/", 0, 0);
						defaultdir = ostrcat(defaultdir, skinpath1, 1, 0);
						defaultdir = ostrcat(defaultdir, tmppng, 1, 0);
						defaultdir = ostrcat(defaultdir, ".png", 1, 0);
						debug(60, "defaultdir3 %s", defaultdir);
					}

					debug(60, "defaultdir4 %s", defaultdir);
					if(file_exist(defaultdir))
					{
						tmpskinpath = ostrcat("", defaultdir, 0, 0);
						changepic(tmp, tmpskinpath);
						free(tmpskinpath); tmpskinpath = NULL;
					}
					else
					{
						tmpskinpath = ostrcat(skinpath1, "default.png", 0, 0);
						changepic(tmp, tmpskinpath);
						free(tmpskinpath); tmpskinpath = NULL;
					}
					free(defaultdir); defaultdir = NULL;
				}
			}
			if(defaultstr != NULL)
			{
				setlistboxselection(listbox, defaultstr);
			//	if(ostrcmp(defaultstr, (&ret1[i])->part) == 0)
			//		listbox->aktline = i + 1;
			}
		}
		free(tmppng); tmppng = NULL;
	}

	free(ret1); ret1 = NULL;
	free(ret2); ret2 = NULL;
	free(ret3); ret3 = NULL;
	free(tmpck); tmpck = NULL;
	free(tmpsplit); tmpsplit = NULL;
	free(tmpstr1); tmpstr1 = NULL;
	free(tmpstr4); tmpstr4 = NULL;
	free(tmpstr5); tmpstr5 = NULL;

	listbox->aktpage = -1;

	if(fromthread == 1)
	{
		m_lock(&status.drawingmutex, 0);
		m_lock(&status.rcmutex, 10);
		status.screencalc = 2;
		setnodeattr(screen, framebuffer);
		status.screencalc = 0;
		status.rcowner = screen;
		bg = savescreen(screen);
		tmpscreencalc = status.screencalc;
		status.screencalc = 0;
		drawscreen(screen, 2);
	}
	else
		drawscreen(screen,0);

	addscreenrc(screen, listbox);

	while (1)
	{
		rcret = waitrc(screen, 0, 0);

		if(rcret==getrcconfigint("rcexit",NULL)) break;
		if(listbox->select != NULL && rcret==getrcconfigint("rcok",NULL))
		{
			tmpstr = ostrcat(tmpstr, listbox->select->name, 1, 0);
			break;
		}
	}

	free(skinname1); skinname1 = NULL;
	free(skinpath1); skinpath1 = NULL;
	delownerrc(screen);
	delmarkedscreennodes(screen, 1);

	if(fromthread == 1)
	{
		clearscreennolock(screen);
		restorescreen(bg, screen);
		blitfb();
		status.screencalc = tmpscreencalc;
		sleep(1);
		status.rcowner = NULL;
		m_unlock(&status.rcmutex, 3);
		m_unlock(&status.drawingmutex, 0);
	}
	else
		clearscreen(screen);

	debug(1000, "out");
	return tmpstr;
}

#endif
