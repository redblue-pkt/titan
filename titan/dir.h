#ifndef DIR_H
#define DIR_H

void readlabelext(struct skin* label, char* filename, char* ext)
{
	char *tmpstr = NULL, *tmpstr1 = NULL;

	tmpstr = changefilenameext(filename, ext);
	tmpstr1 = readfiletomem(tmpstr, 0);
	changetext(label, tmpstr1);

	free(tmpstr); tmpstr = NULL;
	free(tmpstr1); tmpstr1 = NULL;
}

char* screendir(char* path, char* mask, char* selection, int *dirrcret, char* ext, char* b1, int rc1, char* b2, int rc2, char* b3, int rc3, char* b4, int rc4, int width, int prozwidth, int height, int prozheight, int flag)
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
	struct skin *button = NULL;
	char *ret = NULL;
	char *tmppath = NULL;

	if(path == NULL || strlen(path) == 0 || !isdir(path))
		tmppath = ostrcat("/", "", 0, 0);
	else
		tmppath = ostrcat(path, "", 0, 0);

	filelist->aktline = 0;
	filelist->aktpage = 0;
	changemask(filelist, mask);
	changeinput(filelist, tmppath);
	changetext(filelistpath, filelist->input);
	dir->width = width;
	dir->prozwidth = prozwidth;
	dir->height = height;
	dir->prozheight = prozheight;

	free(tmppath);
	delmarkedscreennodes(dir, FILELISTDELMARK);
	createfilelist(dir, filelist);
	setlistboxselection(filelist, selection);

	if(ext != NULL)
	{
		ret = createpath(filelist->input, selection);
		readlabelext(label, ret, ext);
		free(ret); ret = NULL;
	}

	button = getscreennode(dir, "b1");
	if(b1 != NULL)
		changetext(button, b1);
	else
		changetext(button, _("---"));

	button = getscreennode(dir, "b2");
	if(b2 != NULL)
		changetext(button, b2);
	else
		changetext(button, _("---"));

	button = getscreennode(dir,  "b3");
	if(b3 != NULL)
		changetext(button, b3);
	else
		changetext(button, _("---"));

	button = getscreennode(dir, "b4");
	if(b4 != NULL)
		changetext(button, b4);
	else
		changetext(button, _("---"));

	drawscreen(dir, 0);
	addscreenrc(dir, filelist);

	while(1)
	{
		if(ext != NULL) status.screencalc = 2;
		rcret = waitrc(dir, 0, 0);
		if(ext != NULL) status.screencalc = 0;

		if(rcret == getrcconfigint("rcexit", NULL))
		{
			free(ret); ret = NULL;
			if(dirrcret != NULL) *dirrcret = 5;
			break;
		}

		if(rcret == getrcconfigint("rcup", NULL) || rcret == getrcconfigint("rcdown", NULL) || rcret == getrcconfigint("rcright", NULL) || rcret == getrcconfigint("rcleft", NULL))
		{
			if(ext != NULL && filelist->select != NULL && filelist->select->input != NULL) //dir
				changetext(label, NULL);
			else if(filelist->select != NULL && filelist->select->input == NULL) //file
			{

				ret = createpath(filelistpath->text, filelist->select->text);
				if(ext != NULL) readlabelext(label, ret, ext);
				free(ret); ret = NULL;
			}
			drawscreen(dir, 0);
			continue;
		}

		if(rcret == getrcconfigint("rcok", NULL) || (b1 != NULL && rcret == getrcconfigint("rcred", NULL)) || (b3 != NULL && rcret == getrcconfigint("rcyellow", NULL)) || (b4 != NULL && rcret == getrcconfigint("rcblue", NULL)))
		{
			if(filelist->select != NULL && filelist->select->input != NULL) //dir
			{
				if(ext != NULL)
					drawscreen(dir, 0);
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
				drawscreen(dir, 0);
		}

		if(rcret == getrcconfigint("rcgreen", NULL))
		{
			if(filelistpath != NULL)
			{
				if(filelist->mask == NULL || strlen(filelist->mask) == 0)
				{
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
	}

	delmarkedscreennodes(dir, FILELISTDELMARK);
	delownerrc(dir);
	clearscreen(dir);
	return ret;
}

#endif
