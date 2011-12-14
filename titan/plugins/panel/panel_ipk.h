#ifndef PANEL_IPK_H
#define PANEL_IPK_H

// showpng = 0 (no icon)
// showpng = 1 (smal icon)
// showpng = 2 (big icon) (ipk)
// showpng = 3 (networkbrowser)
// showpng = 4 same as 1 the first arg splittet ")( " (networkbrowser/harddisk)

char* ipklistbox(char* defaultstr, char* str, char* skinname, char* skintitle, char* skinpath, int showpng)
{
	debug(1000, "in");
	char* skinname1 = NULL;
	char* skinpath1 = NULL;
	char* tmpskinpath = NULL;
	int rcret = 0;

	if(str == NULL)
		return 0;

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

	if(skinpath == NULL
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
	char* tmpinfo = NULL;
	char* tmptitle = NULL;
	char* tmpsplit = NULL;
	char* tmpck = NULL;
	char* tmpstr1 = NULL;
	char* tmpstr4 = NULL;
	char* tmpstr5 = NULL;

	struct splitstr* ret1 = NULL;
	struct splitstr* ret2 = NULL;
	struct splitstr* ret3 = NULL;
	int count = 0;
	int i = 0;
	ret1 = strsplit(str, "\n", &count);

	if(showpng == 2)
		tmpck = get_ipk_listinstall();
//		tmpck = ostrcat(tmpck, get_ipk_listinstall(), 1, 0);

	for( i = 0; i < count; i++)
	{
		if(showpng == 2)
		{
			int count3 = 0;
			int a = 0;

			tmpstr1 = ostrcat("", tmpck, 0, 0);

			debug(60, "tmpck: (%d) %s", i, tmpstr1);

			ret3 = strsplit(tmpstr1, "\n", &count3);
			int skip = 0;

			for(a = 0; a < count3; a++)
			{
				debug(60, "check1: (%d)-(%d) %s", i, a, (&ret3[a])->part);
				debug(60, "check2: (%d)-(%d) %s", i, a, (&ret1[i])->part);
				if(string_find((&ret3[a])->part,(&ret1[i])->part))
				{
					debug(60, "set skiped=1");
					skip = 1;
					continue;
				}
			}
			if(skip == 1)
			{
				skip = 0;
				debug(60, "skipped listing: %s",(&ret1[i])->part);
				continue;
			}
		}

		int count2 = 0;
		tmpsplit = ostrcat(tmpsplit, (&ret1[i])->part, 1, 0);
		free(ret2); ret2 = NULL;
		ret2 = strsplit(tmpsplit, "-", &count2);

		tmp = addlistbox(screen, listbox, tmp, 1);
		
		if(tmp != NULL)
		{
			changetext(tmp, _((&ret1[i])->part));
			changename(tmp, (&ret1[i])->part);
			if(showpng == 2)
			{
				// ipk list
				// section 2
				// name 3
				// version 4
				// description 5
				int j = 0;
				for(j = 0; j < count2; j++)
				{
					if(j > 0)
					{
						tmptitle = NULL;
						tmptitle = ostrcat(tmptitle, (&ret2[3])->part, 1, 0);
						tmptitle = ostrcat(tmptitle, " v.", 1, 0);
						tmptitle = ostrcat(tmptitle, strstrip((&ret2[4])->part), 1, 0);
						changetext(tmp, _(tmptitle));
						changename(tmp, tmptitle);

						tmpinfo = NULL;
						tmpinfo = ostrcat(tmpinfo, "\nSection: ", 1, 0);
						tmpinfo = ostrcat(tmpinfo, (&ret2[2])->part, 1, 0);
						tmpinfo = ostrcat(tmpinfo, "\nDescription:\n", 1, 0);
						tmpinfo = ostrcat(tmpinfo, strstrip((&ret2[5])->part), 1, 0);
						changetext2(tmp, _(tmpinfo));

						(&ret1[i])->part = string_remove_whitechars((&ret2[3])->part);

						free(tmpinfo); tmpinfo = NULL;
						free(tmptitle); tmptitle = NULL;
					}
				}
			}

			tmp->del = 1;
			if((showpng == 1) || (showpng == 3) || (showpng == 4)) 
			{
				tmp->textposx = 120;
				tmp->height = 50;
//				tmp->fontsize = 30;
				tmp->valign = convertxmlentry("middle", 0);
				tmp->hspace = 5;
				debug(60, "showpng changed=%d", showpng);
			}
			else if(showpng == 2)
			{
				tmp->valign = convertxmlentry("middle", 0);
				tmp->textposx = 250;
//				tmp->height = 160;
				tmp->height = 170;
//				tmp->fontsize = 30;
//				tmp->fontcol2 = convertcol("ff0000"); 
 				tmp->textposx2 = 270;
//				tmp->fontsize2 = 22;
				tmp->type=TEXTBOX;
				tmp->wrap=YES;
				tmp->hspace = 5;
				debug(60, "showpng changed=%d", showpng);
			}
			else
				debug(60, "showpng default=%d", showpng);

			if(ostrcmp((&ret1[i])->part, "\t") == 0)
				tmp->deaktivcol = convertcol("deaktivcol");
			else
			{
				if(showpng == 1)
				{
					if(string_find("%pluginpath%",skinpath1))
					{
						struct splitstr* ret6 = NULL;
						int count6 = 0;
						char* tmpstr6 = NULL;
						tmpstr6 = ostrcat("", skinpath1, 0, 0);
						ret6 = strsplit(tmpstr6, "%", &count6);
						defaultdir = ostrcat(getconfig("skinpath", NULL), "/skin/panel_", 0, 0);
						defaultdir = ostrcat(defaultdir, (&ret1[i])->part, 1, 0);
						defaultdir = ostrcat(defaultdir, ".png", 1, 0);

						if(!file_exist(defaultdir))
						{
							defaultdir = ostrcat(getconfig("pluginpath", NULL), (&ret6[1])->part, 0, 0);
							defaultdir = ostrcat(defaultdir, "panel_", 0, 0);
							defaultdir = ostrcat(defaultdir, (&ret1[i])->part, 1, 0);
							defaultdir = ostrcat(defaultdir, ".png", 1, 0);
						}
						free(ret6), ret6 = NULL;
					}
					else
					{
						defaultdir = ostrcat(getconfig("skinpath", NULL), skinpath1, 0, 0);
						defaultdir = ostrcat(defaultdir, "/skin/panel_", 0, 0);
						defaultdir = ostrcat(defaultdir, (&ret1[i])->part, 1, 0);
						defaultdir = ostrcat(defaultdir, ".png", 1, 0);
					}

					debug(60, "defaultdir %s", defaultdir);
					if(file_exist(defaultdir))
					{
						tmpskinpath = ostrcat("", defaultdir, 0, 0);
						changepic(tmp, tmpskinpath);
						free(tmpskinpath); tmpskinpath = NULL;
					}
					else
					{
						tmpskinpath = ostrcat(skinpath1, "panel_default.png", 0, 0);
						changepic(tmp, tmpskinpath);
						free(tmpskinpath); tmpskinpath = NULL;
					}
					free(defaultdir); defaultdir = NULL;
				}
				else if(showpng == 2)
				{		
					struct splitstr* ret4 = NULL;
					int count4 = 0;
					tmpstr4 = ostrcat("", (&ret1[i])->part, 0, 0);
					ret4 = strsplit(tmpstr4, ".", &count4);

					defaultdir = ostrcat("", skinpath1, 0, 0);
					defaultdir = ostrcat(defaultdir, "titan-pluginpreview-", 1, 0);
					defaultdir = ostrcat(defaultdir, (&ret4[0])->part, 1, 0);
					defaultdir = ostrcat(defaultdir, ".png", 1, 0);
		
					debug(60, "defaultdir %s", defaultdir);

					if(file_exist(defaultdir))
					{
						debug(60, "defaultdir found");
						tmpskinpath = ostrcat(skinpath1, "titan-pluginpreview-", 0, 0);
						tmpskinpath = ostrcat(tmpskinpath, (&ret4[0])->part, 1, 0);
						tmpskinpath = ostrcat(tmpskinpath, ".png", 1, 0);
						changepic(tmp, tmpskinpath);
						free(tmpskinpath); tmpskinpath = NULL;
					}	
					else
					{
						debug(60, "defaultdir not found use default.png");
						tmpskinpath = ostrcat("", "skin/plugin.png", 0, 0);
						changepic(tmp, tmpskinpath);
						free(tmpskinpath); tmpskinpath = NULL;
					}
					free(defaultdir); defaultdir = NULL;
					free(ret4), ret4 = NULL;
				}
				else if(showpng == 3)
				{
					tmpskinpath = ostrcat(skinpath1, "scanshares.png", 0, 0);
					changepic(tmp, tmpskinpath);
					free(tmpskinpath); tmpskinpath = NULL;
				}
				else if(showpng == 4)
				{
					struct splitstr* ret5 = NULL;
					int count5 = 0;
					tmpstr5 = ostrcat("", (&ret1[i])->part, 0, 0);
					ret5 = strsplit(tmpstr5, ")( ", &count5);
					debug(60, "(&ret5[0])->part %s", (&ret5[0])->part);

					if(ostrcmp((&ret5[0])->part, "add") == 0)
					{
						defaultdir = ostrcat("", (&ret1[i])->part, 0, 0);
						defaultdir = ostrcat(defaultdir, ".png", 1, 0);
					}
					else
					{
						defaultdir = ostrcat("", (&ret5[0])->part, 0, 0);
						defaultdir = ostrcat(defaultdir, ".png", 1, 0);
					}

					tmpskinpath = ostrcat(skinpath1, defaultdir, 0, 0);
					changepic(tmp, tmpskinpath);
					free(tmpskinpath); tmpskinpath = NULL;
					free(defaultdir); defaultdir = NULL;
					free(ret5), ret5 = NULL;
				}
			}
			if(defaultstr != NULL)
			{
				setlistboxselection(listbox, defaultstr);
			//	if(ostrcmp(defaultstr, (&ret1[i])->part) == 0)
			//		listbox->aktline = i + 1;
			}
		}
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
	clearscreen(screen);
	debug(1000, "out");
	return tmpstr;
}

#endif
