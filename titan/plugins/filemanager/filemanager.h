#ifndef FILEMANAGER_H
#define FILEMANAGER_H

void filemanagerrename(int aktfilelist, struct skin* filelist1, struct skin* filelistpath1, struct skin* filelist2, struct skin* filelistpath2)
{
	char* tmpstr = NULL, *file1 = NULL, *cmd = NULL;

	if(filelistpath1 == NULL || filelistpath2 == NULL)
		return;

	if(filelist1 == NULL || filelist1->select == NULL)
		return;

	if(filelist2 == NULL || filelist2->select == NULL)
		return;

	if(aktfilelist == 0)
		tmpstr = ostrcat(filelist1->select->text, NULL, 0, 0);
	else
		tmpstr = ostrcat(filelist2->select->text, NULL, 0, 0);
		
	char* search = textinput(_("Rename"), tmpstr);
	free(tmpstr); tmpstr = NULL;

	if(search != NULL)
	{
		if(aktfilelist == 0)
		{
			file1 = createpath(filelistpath1->text, filelist1->select->text);
			tmpstr = createpath(filelistpath1->text, search);
		}
		else
		{
			file1 = createpath(filelistpath2->text, filelist2->select->text);
			tmpstr = createpath(filelistpath2->text, search);
		}

		if(!file_exist(tmpstr))
		{
			cmd = ostrcat(cmd, "mv \"", 1, 0);
			cmd = ostrcat(cmd, file1, 1, 0);
			cmd = ostrcat(cmd, "\" \"", 1, 0);
			cmd = ostrcat(cmd, tmpstr, 1, 0);
			cmd = ostrcat(cmd, "\"", 1, 0);
			system(cmd);
			free(cmd); cmd = NULL;
		}

		free(tmpstr); tmpstr = NULL;
		free(search); search = NULL;
	}
}

void filemanagercreatefolder(int aktfilelist, struct skin* filelist1, struct skin* filelistpath1, struct skin* filelist2, struct skin* filelistpath2)
{
	char* tmpstr = NULL;

	if(filelistpath1 == NULL || filelistpath2 == NULL)
		return;

	if(filelist1 == NULL || filelist1->select == NULL)
		return;

	if(filelist2 == NULL || filelist2->select == NULL)
		return;

	char* search = textinputhist(_("Create Folder"), " ", "searchhist");
	if(search != NULL)
	{
		if(aktfilelist == 0)
			tmpstr = createpath(filelistpath1->text, search);
		else
			tmpstr = createpath(filelistpath2->text, search);

		if(!file_exist(tmpstr))
			mkdir(tmpstr, 0777);

	}

	free(tmpstr); tmpstr = NULL;
	free(search); search = NULL;
}
			
void filemanagermenu(int aktfilelist, struct skin* filelist1, struct skin* filelistpath1, struct skin* filelist2, struct skin* filelistpath2)
{
	struct menulist* mlist = NULL, *mbox = NULL;
	char* skintitle = "Menu";

	addmenulist(&mlist, "Rename", NULL, NULL, 0, 0);
	addmenulist(&mlist, "Create Folder", NULL, NULL, 0, 0);

	mbox = menulistbox(mlist, NULL, skintitle, NULL, NULL, 1, 0);
	if(mbox != NULL)
	{
		if(ostrcmp(mbox->name, "Rename") == 0)
			filemanagerrename(aktfilelist, filelist1, filelistpath1, filelist2, filelistpath1);
		else if(ostrcmp(mbox->name, "Create Folder") == 0)
			filemanagercreatefolder(aktfilelist, filelist1, filelistpath1, filelist2, filelistpath1);

	}

	freemenulist(mlist, 1); mlist = NULL;
}

void screenfilemanager()
{
	int rcret = 0, aktfilelist = 0, ret = 0;
	long bgcol1 = 0, bgcol2 = 0;
	struct skin* filemanager = getscreen("filemanager");
	struct skin* filemanager1 = getscreen("filemanager1");
	struct skin* filemanager2 = getscreen("filemanager2");
	struct skin* filelistpath1 = getscreennode(filemanager1, "filelistpath");
	struct skin* filelist1 = getscreennode(filemanager1, "filelist");
	struct skin* filelistpath2 = getscreennode(filemanager2, "filelistpath");
	struct skin* filelist2 = getscreennode(filemanager2, "filelist");
	char* file1 = NULL, *cmd = NULL, *tmpstr = NULL;
	struct skin* tmpfilelist = filelist1;

	filelist1->aktline = 0;
	filelist1->aktpage = 0;
	changemask(filelist1, "*");
	changeinput(filelist1, "/");
	changetext(filelistpath1, filelist1->input);
	
	filelist2->aktline = 0;
	filelist2->aktpage = 0;
	changemask(filelist2, "*");
	changeinput(filelist2, "/");
	changetext(filelistpath2, filelist2->input);

	delmarkedscreennodes(filemanager1, FILELISTDELMARK);
	delmarkedscreennodes(filemanager2, FILELISTDELMARK);
	createfilelist(filemanager1, filelist1, 0);
	createfilelist(filemanager2, filelist2, 0);

	bgcol1 = filelistpath1->bgcol;
	bgcol2 = filelistpath2->bgcol;

	filelistpath1->bgcol = convertcol("markcol");
	
	drawscreen(filemanager, 0, 1);
	drawscreen(filemanager1, 0, 1);
	drawscreen(filemanager2, 0, 0);
	addscreenrc(filemanager1, filelist1);
	delrc(getrcconfigint("rcff", NULL), filemanager1, filelist1);
	delrc(getrcconfigint("rcfr", NULL), filemanager1, filelist1);

	while(1)
	{
		if(aktfilelist == 0)
		{
			rcret = waitrc(filemanager1, 0, 0);
			tmpfilelist = filelist1;
		}
		else
		{
			rcret = waitrc(filemanager2, 0, 0);
			tmpfilelist = filelist2;
		}
		
		if(rcret == getrcconfigint("rcexit", NULL)) break;

		if(status.security == 1)
		{
			if((rcret == getrcconfigint("rcgreen", NULL) || rcret == getrcconfigint("rcyellow", NULL)) && tmpfilelist->select != NULL && ostrcmp(tmpfilelist->select->text, "..") != 0) //copy - move
			{
				if(aktfilelist == 0)
					file1 = createpath(filelistpath1->text, filelist1->select->text);
				else
					file1 = createpath(filelistpath2->text, filelist2->select->text);

				if(file1 != NULL)
				{
					if(rcret == getrcconfigint("rcgreen", NULL))
					{
						tmpstr = ostrcat(tmpstr, _("Realy copy this file/dir?"), 1, 0);
						cmd = ostrcat(cmd, "cp -r ", 1, 0);
					}
					if(rcret == getrcconfigint("rcyellow", NULL))
					{
						tmpstr = ostrcat(tmpstr, _("Realy move this file/dir?"), 1, 0);
						cmd = ostrcat(cmd, "mv ", 1, 0);
					}
					
					tmpstr = ostrcat(tmpstr, "\n\n", 1, 0);
					tmpstr = ostrcat(tmpstr, _("From"), 1, 0);
					tmpstr = ostrcat(tmpstr, ": ", 1, 0);				
					tmpstr = ostrcat(tmpstr, file1, 1, 0);
					tmpstr = ostrcat(tmpstr, "\n", 1, 0);
					tmpstr = ostrcat(tmpstr, _("To"), 1, 0);
					tmpstr = ostrcat(tmpstr, ": ", 1, 0);
					if(aktfilelist == 0)
						tmpstr = ostrcat(tmpstr, filelistpath2->text, 1, 0);
					else
						tmpstr = ostrcat(tmpstr, filelistpath1->text, 1, 0);
					ret = textbox(_("Message"), tmpstr, _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 1000, 300, 0, 0);
					free(tmpstr); tmpstr = NULL;
					
					cmd = ostrcat(cmd, "\"", 1, 0);
					cmd = ostrcat(cmd, file1, 1, 0);
					cmd = ostrcat(cmd, "\" \"", 1, 0);
					if(aktfilelist == 0)
						cmd = ostrcat(cmd, filelistpath2->text, 1, 0);
					else
						cmd = ostrcat(cmd, filelistpath1->text, 1, 0);
					cmd = ostrcat(cmd, "\"", 1, 0);
					if(ret == 1) system(cmd);
					free(cmd); cmd = NULL;
				}
				free(file1); file1 = NULL;
				
				if(ret == 1)
				{
					delmarkedscreennodes(filemanager1, FILELISTDELMARK);
					delmarkedscreennodes(filemanager2, FILELISTDELMARK);
					createfilelist(filemanager1, filelist1, 0);
					createfilelist(filemanager2, filelist2, 0);
				}
				drawscreen(filemanager, 0, 1);
				drawscreen(filemanager1, 0, 1);
				drawscreen(filemanager2, 0, 0);
			}
			
			if(rcret == getrcconfigint("rcred", NULL) && tmpfilelist->select != NULL && ostrcmp(tmpfilelist->select->text, "..") != 0) //delete
			{
				if(aktfilelist == 0)
					file1 = createpath(filelistpath1->text, filelist1->select->text);
				else
					file1 = createpath(filelistpath2->text, filelist2->select->text);
				if(file1 != NULL)
				{
					tmpstr = ostrcat(tmpstr, _("Realy delete this file/dir?"), 1, 0);
					tmpstr = ostrcat(tmpstr, "\n\n", 1, 0);
					tmpstr = ostrcat(tmpstr, file1, 1, 0);
					ret = textbox(_("Message"), tmpstr, _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 1000, 200, 0, 0);
					free(tmpstr); tmpstr = NULL;
					
					cmd = ostrcat(cmd, "rm -r ", 1, 0);
					cmd = ostrcat(cmd, "\"", 1, 0);
					cmd = ostrcat(cmd, file1, 1, 0);
					cmd = ostrcat(cmd, "\"", 1, 0);
					if(ret == 1) system(cmd);
					free(cmd); cmd = NULL;			
				}
				free(file1); file1 = NULL;
				
				if(ret == 1)
				{
					delmarkedscreennodes(filemanager1, FILELISTDELMARK);
					delmarkedscreennodes(filemanager2, FILELISTDELMARK);
					createfilelist(filemanager1, filelist1, 0);
					createfilelist(filemanager2, filelist2, 0);
				}
				drawscreen(filemanager, 0, 1);
				drawscreen(filemanager1, 0, 1);
				drawscreen(filemanager2, 0, 0);
			}
			
			if(rcret == getrcconfigint("rcblue", NULL) && tmpfilelist->select != NULL && ostrcmp(tmpfilelist->select->text, "..") != 0) //view
			{
				if(aktfilelist == 0)
					file1 = createpath(filelistpath1->text, filelist1->select->text);
				else
					file1 = createpath(filelistpath2->text, filelist2->select->text);

				if(cmpfilenameext(file1, ".txt") == 0 || cmpfilenameext(file1, ".sh") == 0 || cmpfilenameext(file1, ".cfg") == 0 || cmpfilenameext(file1, ".conf") == 0)
				{
					tmpstr = readfiletomem(file1, 0);
					if(tmpstr != NULL)
					{
						textbox(file1, tmpstr, _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 1000, 600, 0, 0);

						drawscreen(filemanager, 0, 1);
						drawscreen(filemanager1, 0, 1);
						drawscreen(filemanager2, 0, 0);
					}
					free(tmpstr); tmpstr = NULL;
				}
				else
				{
					textbox(_("Message"), _("Can't show this file!"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 10, 0);

					drawscreen(filemanager, 0, 1);
					drawscreen(filemanager1, 0, 1);
					drawscreen(filemanager2, 0, 0);
				}
				free(file1); file1 = NULL;
			}

			//menu
			if(rcret == getrcconfigint("rcmenu", NULL))
			{
				filemanagermenu(aktfilelist, filelist1, filelistpath1, filelist2, filelistpath2);

				delmarkedscreennodes(filemanager1, FILELISTDELMARK);
				delmarkedscreennodes(filemanager2, FILELISTDELMARK);
				createfilelist(filemanager1, filelist1, 0);
				createfilelist(filemanager2, filelist2, 0);
					
				drawscreen(filemanager, 0, 1);
				drawscreen(filemanager1, 0, 1);
				drawscreen(filemanager2, 0, 0);
			}
		}
		else
			textbox(_("Message"), _("Registration needed, please contact Atemio !"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 1000, 200, 0, 0);
		
		if(rcret == getrcconfigint("rcff", NULL) || rcret == getrcconfigint("rcfr", NULL)) //change filelist
		{
			if(aktfilelist == 0)
			{
				aktfilelist = 1;
				delownerrc(filemanager1);
				delownerrc(filemanager2);
				addscreenrc(filemanager2, filelist2);
				delrc(getrcconfigint("rcff", NULL), filemanager2, filelist2);
				delrc(getrcconfigint("rcfr", NULL), filemanager2, filelist2);
				filelistpath1->bgcol = 0;
				filelistpath2->bgcol = convertcol("markcol");
			}
			else
			{
				aktfilelist = 0;
				delownerrc(filemanager1);
				delownerrc(filemanager2);
				addscreenrc(filemanager1, filelist1);
				delrc(getrcconfigint("rcff", NULL), filemanager1, filelist1);
				delrc(getrcconfigint("rcfr", NULL), filemanager1, filelist1);
				filelistpath1->bgcol = convertcol("markcol");
				filelistpath2->bgcol = 0;
			}
			drawscreen(filemanager1, 0, 1);
			drawscreen(filemanager2, 0, 0);
		}
	
	}
	
	delmarkedscreennodes(filemanager1, FILELISTDELMARK);
	delmarkedscreennodes(filemanager2, FILELISTDELMARK);
	filelistpath1->bgcol = bgcol1;
	filelistpath2->bgcol = bgcol2;
	delownerrc(filemanager1);
	delownerrc(filemanager2);
	clearscreen(filemanager);
}

#endif
