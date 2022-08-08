#ifndef DIR_H
#define DIR_H

void readlabelext(struct skin* label, char* filename, char* ext)
{
	char *tmpstr = NULL, *tmpstr1 = NULL;

	tmpstr = changefilenameext(filename, ext);
	tmpstr1 = readfiletomem(tmpstr, 0);
	if(tmpstr1 == NULL && ostrcmp(ext, ".epg") == 0)
	{
		free(tmpstr); tmpstr = NULL;
		tmpstr = changefilenameext(filename, ".eit");
		tmpstr1 = readeittomem(tmpstr);
	}
	
	changetext(label, tmpstr1);

	free(tmpstr); tmpstr = NULL;
	free(tmpstr1); tmpstr1 = NULL;
}

//flag 1: add selected dir to path
//flag 2-15: add selected dir to path and use dirrcret for dirs
//flag 16-127: use dirrcret for dirs
char* screendirreal(char* path, char* mask, char* selection, int *dirrcret, char* ext, char* b1, int rc1, char* b2, int rc2, char* b3, int rc3, char* b4, int rc4, int width, int prozwidth, int height, int prozheight, int holdselection, int flag)
{
	int rcret = 0;
	struct skin* dir = NULL;

	if(ext != NULL)
		dir = getscreen("dirlabel");
	else
		dir = getscreen("dir");

	struct skin* filelistpath = getscreennode(dir, "filelistpath");
	struct skin* filelist = getscreennode(dir, "filelist");
	struct skin* label = getscreennode(dir, "label");
	struct skin* thumb = getscreennode(dir, "thumb");
	struct skin* b5 = getscreennode(dir, "b5");
	struct skin *button = NULL;
	struct skin* load = getscreen("loading");
	drawscreen(load, 0, 0);

	char *ret = NULL;
	char *tmppath = NULL;

	if(path == NULL || strlen(path) == 0 || !isdir(path))
		tmppath = ostrcat("/", NULL, 0, 0);
	else
		tmppath = ostrcat(path, NULL, 0, 0);

	if(holdselection == 0)
	{
		filelist->aktline = 0;
		filelist->aktpage = 0;
	}
	changemask(filelist, mask);
	changeinput(filelist, tmppath);
	changetext(filelistpath, filelist->input);
	dir->width = width;
	dir->prozwidth = prozwidth;
	dir->height = height;
	dir->prozheight = prozheight;

	free(tmppath);
	delmarkedscreennodes(dir, FILELISTDELMARK);
	createfilelist(dir, filelist, 0);
	if(holdselection == 0) setlistboxselection(filelist, selection);

	if(ext != NULL)
	{
		ret = createpath(filelist->input, selection);
		readlabelext(label, ret, ext);
		free(ret); ret = NULL;
	}

	button = getscreennode(dir, "b1");
	if(b1 != NULL)
		changetext(button, _(b1));
	else
	{
		changetext(button, _("---"));
		button->hidden = YES;
	}

	button = getscreennode(dir, "b2");
	if(b2 != NULL)
		changetext(button, _(b2));
	else
	{
		changetext(button, _("---"));
		button->hidden = YES;
	}

	button = getscreennode(dir,  "b3");
	if(b3 != NULL)
		changetext(button, _(b3));
	else
	{
		changetext(button, _("---"));
		button->hidden = YES;
	}

	button = getscreennode(dir, "b4");
	if(b4 != NULL)
		changetext(button, _(b4));
	else
	{
		changetext(button, _("---"));
		button->hidden = YES;
	}

	if(flag == 64)
		b5->hidden = NO;
	else
		b5->hidden = YES;

	changepic(thumb, "playpic.png");

	clearscreen(load);
	drawscreen(dir, 0, 0);
	addscreenrc(dir, filelist);

	while(1)
	{
		if(ext != NULL)
			rcret = waitrc(dir, 0, 2);
		else
			rcret = waitrc(dir, 0, 0);

		if(rcret == getrcconfigint("rcexit", NULL))
		{
			free(ret); ret = NULL;
			if(dirrcret != NULL) *dirrcret = 5;
			break;
		}

		if(rcret == getrcconfigint("rcup", NULL) || rcret == getrcconfigint("rcdown", NULL) || rcret == getrcconfigint("rcright", NULL) || rcret == getrcconfigint("rcleft", NULL))
		{
			if(ext != NULL && filelist->select != NULL && filelist->select->input != NULL) //dir
			{
				changetext(label, NULL);
				drawscreen(dir, 0, 0);
			}
			else if(filelist->select != NULL && filelist->select->input == NULL) //file
			{
				ret = createpath(filelistpath->text, filelist->select->text);
				if(ext != NULL)
				{
					char* tmpstr = NULL, *pic = NULL;
					struct mediadb* mnode = getmediadb(filelistpath->text, filelist->select->text, 0);
					if(mnode != NULL)
					{
						if(mnode->id != NULL)
						{
							tmpstr = ostrcat(tmpstr, getconfig("mediadbpath", NULL), 1, 0);
							tmpstr = ostrcat(tmpstr, "/", 1, 0);
							tmpstr = ostrcat(tmpstr, mnode->id, 1, 0);

							pic = ostrcat(tmpstr, "_poster.jpg", 0, 0);
							changepic(thumb, pic);
						}
						else
							changepic(thumb, "playpic.png");
					}
					else
						changepic(thumb, "playpic.png");
											
					free(pic), pic = NULL;
					free(tmpstr), tmpstr = NULL;

					readlabelext(label, ret, ext);
					drawscreen(dir, 0, 0);
				}
				free(ret); ret = NULL;
			}
			continue;
		}
		if(rcret == getrcconfigint("rcok", NULL) || (b1 != NULL && rcret == getrcconfigint("rcred", NULL)) || (b3 != NULL && rcret == getrcconfigint("rcyellow", NULL)) || (b4 != NULL && rcret == getrcconfigint("rcblue", NULL)))
		{
			if(filelist->select != NULL && filelist->select->input != NULL) //dir
			{
				if(dirrcret != NULL)
				{
					if(((flag & 2) || (flag & 16)) && b1 != NULL && rcret == getrcconfigint("rcred", NULL))
					{
						*dirrcret = 1;
						break;
					}
					if(((flag & 4) || (flag & 32)) && b3 != NULL && rcret == getrcconfigint("rcyellow", NULL))
					{
						*dirrcret = 4;
						break;
					}
					if(((flag & 8) || (flag & 64)) && b4 != NULL && rcret == getrcconfigint("rcblue", NULL))
					{
						*dirrcret = 4;
						break;
					}
					if(flag == 64 && b1 != NULL && rcret == getrcconfigint("rcred", NULL)) // remove folder
					{
						ret = createpath(filelistpath->text, filelist->select->text);
						*dirrcret = 1;
						break;
					}
					
				}
				if(ext != NULL)
					drawscreen(dir, 0, 0);
				continue;
			}
			else if(filelist->select != NULL && filelist->select->input == NULL) //file
			{
				ret = createpath(filelistpath->text, filelist->select->text);
				if(dirrcret != NULL)
				{
					if(b1 != NULL && rcret == getrcconfigint("rcred", NULL))
						*dirrcret = 1;
					else if(b3 != NULL && rcret == getrcconfigint("rcyellow", NULL))
						*dirrcret = 3;
					else if(b4 != NULL && rcret == getrcconfigint("rcblue", NULL))
						*dirrcret = 4;
				}
				break;
			}
			else if(ext != NULL)
				drawscreen(dir, 0, 0);
		}

		if(rcret == getrcconfigint("rcgreen", NULL))
		{
			if(filelistpath != NULL)
			{
				if(filelist->mask == NULL || strlen(filelist->mask) == 0)
				{
					if(flag >= 1 && flag <= 15)
						ret = createpath(filelistpath->text, filelist->select->text);
					else
						ret = createpath(filelistpath->text, "");
					if(dirrcret != NULL) *dirrcret = 2;
					break;
				}
				else if(filelist->select != NULL && filelist->select->input == NULL)
				{
					ret = createpath(filelistpath->text, filelist->select->text);
					if(dirrcret != NULL) *dirrcret = 2;
					break;
				}
			}
		}

		if(rcret == getrcconfigint("rcinfo", NULL) && status.play == 0 && flag == 64)
		{
			char* file = createpath(filelistpath->text, filelist->select->text);
			if(playrcred(file, NULL, NULL, NULL, 0, 1, 1) == 9999)
			{
				delmarkedscreennodes(dir, FILELISTDELMARK);
				createfilelist(dir, filelist, 0);
			}
			free(file); file = NULL;
			drawscreen(dir, 0, 0);
		}
	}

	delmarkedscreennodes(dir, FILELISTDELMARK);
	delownerrc(dir);
	clearscreen(dir);
	return ret;
}

char* screendir(char* path, char* mask, char* selection, int *dirrcret, char* ext, char* b1, int rc1, char* b2, int rc2, char* b3, int rc3, char* b4, int rc4, int width, int prozwidth, int height, int prozheight, int flag)
{
	return screendirreal(path, mask, selection, dirrcret, ext, b1, rc1, b2, rc2, b3, rc3, b4, rc4, width, prozwidth, height, prozheight, 0, flag);
}

#endif
