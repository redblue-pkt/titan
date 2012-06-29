#ifndef TFILE_H
#define TFILE_H

void screentfile()
{
	int rcret = 0, aktfilelist = 0, ret = 0;
	struct skin* tfile = getscreen("tfile");
	struct skin* tfile1 = getscreen("tfile1");
	struct skin* tfile2 = getscreen("tfile2");
	struct skin* filelistpath1 = getscreennode(tfile1, "filelistpath");
	struct skin* filelist1 = getscreennode(tfile1, "filelist");
	struct skin* filelistpath2 = getscreennode(tfile2, "filelistpath");
	struct skin* filelist2 = getscreennode(tfile2, "filelist");
	char* file1 = NULL, *cmd = NULL, *tmpstr = NULL;

	filelist1->aktline = 0;
	filelist1->aktpage = 0;
	changemask(filelist1, "*");
	changeinput(filelist1, "/");
	changetext(filelistpath1, filelist1->input);
	
	filelist2->aktline = 0;
	filelist2->aktpage = 0;
	changemask(filelist2, "");
	changeinput(filelist2, "/");
	changetext(filelistpath2, filelist2->input);

	delmarkedscreennodes(tfile1, FILELISTDELMARK);
	delmarkedscreennodes(tfile2, FILELISTDELMARK);
	createfilelist(tfile1, filelist1, 0);
	createfilelist(tfile2, filelist2, 0);

	filelistpath1->bgcol = filelistpath1->bordercol;
	
	drawscreen(tfile, 0, 1);
	drawscreen(tfile1, 0, 1);
	drawscreen(tfile2, 0, 0);
	addscreenrc(tfile1, filelist1);
	
	while(1)
	{
		if(aktfilelist == 0)
			rcret = waitrc(tfile1, 0, 0);
		else
			rcret = waitrc(tfile2, 0, 0);
		
		if(rcret == getrcconfigint("rcexit", NULL)) break;
		
		if((rcret == getrcconfigint("rcgreen", NULL) || rcret == getrcconfigint("rcyellow", NULL))&& filelist1->select != NULL && ostrcmp(filelist1->select->text, "..") != 0) //copy - move
		{
			file1 = createpath(filelistpath1->text, filelist1->select->text);
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
				
				tmpstr = ostrcat(tmpstr, _("\n\nFrom: "), 1, 0);
				tmpstr = ostrcat(tmpstr, file1, 1, 0);
				tmpstr = ostrcat(tmpstr, _("\nTo: "), 1, 0);
				tmpstr = ostrcat(tmpstr, filelistpath2->text, 1, 0);
				ret = textbox(_("Message"), tmpstr, _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 1000, 300, 0, 0);
				free(tmpstr); tmpstr = NULL;
				
				cmd = ostrcat(cmd, file1, 1, 0);
				cmd = ostrcat(cmd, " ", 1, 0);
				cmd = ostrcat(cmd, filelistpath2->text, 1, 0);
				if(ret == 1) system(cmd);
				free(cmd); cmd = NULL;
			}
			free(file1); file1 = NULL;
			
			if(ret == 1)
			{
				delmarkedscreennodes(tfile1, FILELISTDELMARK);
				delmarkedscreennodes(tfile2, FILELISTDELMARK);
				createfilelist(tfile1, filelist1, 0);
				createfilelist(tfile2, filelist2, 0);
			}
			drawscreen(tfile1, 0, 1);
			drawscreen(tfile2, 0, 0);
		}
		
		if(rcret == getrcconfigint("rcred", NULL) && filelist1->select != NULL && ostrcmp(filelist1->select->text, "..") != 0) //delete
		{
			file1 = createpath(filelistpath1->text, filelist1->select->text);
			if(file1 != NULL)
			{
				tmpstr = ostrcat(tmpstr, _("Realy delete this file/dir?\n\n"), 1, 0);
				tmpstr = ostrcat(tmpstr, file1, 1, 0);
				ret = textbox(_("Message"), tmpstr, _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 1000, 200, 0, 0);
				free(tmpstr); tmpstr = NULL;
				
				cmd = ostrcat(cmd, "rm -r ", 1, 0);
				cmd = ostrcat(cmd, file1, 1, 0);
				if(ret == 1) system(cmd);
				free(cmd); cmd = NULL;			
			}
			free(file1); file1 = NULL;
			
			if(ret == 1)
			{
				delmarkedscreennodes(tfile1, FILELISTDELMARK);
				delmarkedscreennodes(tfile2, FILELISTDELMARK);
				createfilelist(tfile1, filelist1, 0);
				createfilelist(tfile2, filelist2, 0);
			}
			drawscreen(tfile1, 0, 1);
			drawscreen(tfile2, 0, 0);
		}
		
		if(rcret == getrcconfigint("rcblue", NULL) && filelist1->select != NULL && ostrcmp(filelist1->select->text, "..") != 0) //view
		{
			file1 = createpath(filelistpath1->text, filelist1->select->text);
			tmpstr = readfiletomem(file1, 0);
			if(tmpstr != NULL)
			{
				textbox(file1, tmpstr, _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 1000, 600, 0, 0);

				drawscreen(tfile, 0, 1);
				drawscreen(tfile1, 0, 1);
				drawscreen(tfile2, 0, 0);
			}
			free(tmpstr); tmpstr = NULL;
			free(file1); file1 = NULL;
		}
		
		if(rcret == getrcconfigint("rcff", NULL) || rcret == getrcconfigint("rcfr", NULL)) //change filelist
		{
			if(aktfilelist == 0)
			{
				aktfilelist = 1;
				delownerrc(tfile1);
				delownerrc(tfile2);
				addscreenrc(tfile2, filelist2);			
				filelistpath1->bgcol = 0;
				filelistpath2->bgcol = filelistpath2->bordercol;
			}
			else
			{
				aktfilelist = 0;
				delownerrc(tfile1);
				delownerrc(tfile2);
				addscreenrc(tfile1, filelist1);			
				filelistpath1->bgcol = filelistpath1->bordercol;
				filelistpath2->bgcol = 0;
			}
			drawscreen(tfile1, 0, 1);
			drawscreen(tfile2, 0, 0);
		}
	
	}

	filelistpath1->bgcol = 0;
	filelistpath2->bgcol = 0;
	delownerrc(tfile1);
	delownerrc(tfile2);
	clearscreen(tfile);
}

#endif
