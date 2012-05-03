#ifndef SCREENSAVER_H
#define SCREENSAVER_H

void screensaversetfontcol(bgcol)
{
	if(screensaver == NULL) return;

	if(screensaver->type == 0)
	{
		if(bgcol == 0x0)
			screensaver->screennode->fontcol = 0xffffff;
		else if(bgcol == 0x00ff00)
			screensaver->screennode->fontcol = 0xffffff;
		else if(bgcol == 0x0000ff)
			screensaver->screennode->fontcol = 0xffffff;
		else
			screensaver->screennode->fontcol = 0;
	}
}

//flag 0: textsaver jump
//flag 1: picsaver jump
//flag 2: picsaver scroll
int showscreensaver()
{
	int mixedcol[] = {0, 0x020202, 0xBEBEBE, 0xffffff, 0xffff00, 0xff0000, 0xFFD39B, 0x00ff00, 0x0000ff};
	int overscanx = 60, overscany = 60;
	if(screensaver->type == 3)
		drawscreen(skin, 0);
	if(screensaver == NULL) return 1;

	if(screensaver->type != 3)
	{
		srand(time(NULL));
		if(screensaver->type == 0)
			changetext(screensaver->screennode, screensaver->value);

		if(screensaver->newposx == screensaver->screennode->posx && screensaver->newposy == screensaver->screennode->posy)
		{
			screensaver->newposx = rand() % (skinfb->width - screensaver->screennode->width - (overscanx * 2));
			screensaver->newposy = rand() % (skinfb->height - screensaver->screennode->height - (overscany * 2));
			screensaver->newposx += overscanx;
			screensaver->newposy += overscany;
		}

		if(screensaver->type == 0 || screensaver->type == 1)
		{
			screensaver->screennode->posx = screensaver->newposx;
			screensaver->screennode->posy = screensaver->newposy;
		}

		if(screensaver->type == 2)
		{
			if(screensaver->newposx > screensaver->screennode->posx) screensaver->screennode->posx += 1;
			if(screensaver->newposx < screensaver->screennode->posx) screensaver->screennode->posx -= 1;
			if(screensaver->newposy > screensaver->screennode->posy) screensaver->screennode->posy += 1;
			if(screensaver->newposy < screensaver->screennode->posy) screensaver->screennode->posy -= 1;
		}

		if(ostrcmp(getconfig("screensaver_background_color", NULL), "mixed") == 0)
		{
			int maxcol = sizeof(mixedcol) / sizeof(int);
			screensaver->screen->bgcol = mixedcol[rand() % maxcol];
			screensaversetfontcol(screensaver->screen->bgcol);
		}
		else
		{
			screensaver->screen->bgcol = getconfigint("screensaver_background_color", NULL);
			screensaversetfontcol(screensaver->screen->bgcol);
		}
		drawscreen(screensaver->screen, 0);
	}

	if(screensaver->type == 3)
	{
		if(file_exist(screensaver->path))
		{
			char* path = NULL;
		
			if(screensaver->aktnode == NULL)
				screensaver->aktnode = screensaver->filelist;
			while(screensaver->aktnode != NULL)
			{
				if(screensaver->aktnode->del == FILELISTDELMARK && screensaver->aktnode->text != NULL && screensaver->aktnode->input == 0)
				{
					path = ostrcat(path, screensaver->path, 1, 0);
					path = ostrcat(path, "/", 1, 0);
					path = ostrcat(path, screensaver->aktnode->text, 1, 0);
					debug(90, "singlepicstart %s", path);
					if(screensaver->flag == 0)
						singlepicstart(path, 0);
					else if(screensaver->flag == 1)
						singlepicstart(path, 1);
					free(path); path = NULL;
					screensaver->aktnode = screensaver->aktnode->next;
					break;
				}
				screensaver->aktnode = screensaver->aktnode->next;
			}
		}
	}
	
	return 0;
}

int deinitscreensaver()
{
	if(screensaver != NULL)
	{
		changetext(screensaver->screennode, NULL);
		changepicmem(screensaver->screennode, NULL, 0, 0);
		if((screensaver->type == 1 || screensaver->type == 2) && screensaver->pic != NULL)
			delpic(screensaver->pic->name);
		if(screensaver->type == 3)
		{
			delmarkedscreennodes(screensaver->screen, FILELISTDELMARK);
		}
		free(screensaver);
		screensaver = NULL;
	}

	return 0;
}

int initscreensaver()
{
	screensaver = (struct screensaver*)malloc(sizeof(struct screensaver));
	if(screensaver == NULL)
	{
		err("no mem");
		return 1;
	}
	memset(screensaver, 0, sizeof(struct screensaver));

	if(ostrcmp(getconfig("screensaver_type", NULL), "0000000000") == 0)
	{
		screensaver->type = 0;
		screensaver->speed = getconfigint("screensaver_interval", NULL) * 1000;
		screensaver->screen = getscreen("screensaver");
		screensaver->screennode = getscreennode(screensaver->screen, "textsaver");
	}
	else if(ostrcmp(getconfig("screensaver_type", NULL), "1111111111") == 0)
	{
		screensaver->type = 1;
		screensaver->speed = getconfigint("screensaver_interval", NULL) * 1000;
		screensaver->screen = getscreen("screensaver");
		screensaver->screennode = getscreennode(screensaver->screen, "picsaver");

		changepicmem(screensaver->screennode, getconfig("screensaver_pic", NULL), 0, 0);
		screensaver->pic = getpic(getconfig("screensaver_pic", NULL));
		if(screensaver->pic != NULL)
		{
			screensaver->screennode->width = screensaver->pic->width;
			screensaver->screennode->height = screensaver->pic->height;
		}
		else
		{
			screensaver->screennode->width = 200;
			screensaver->screennode->height = 200;
		}
	}
	else if(ostrcmp(getconfig("screensaver_type", NULL), "2222222222") == 0)
	{
		screensaver->type = 2;
		screensaver->speed = getconfigint("screensaver_interval", NULL);
		screensaver->screen = getscreen("screensaver");
		screensaver->screennode = getscreennode(screensaver->screen, "picsaver");

		changepicmem(screensaver->screennode, getconfig("screensaver_pic", NULL), 0, 0);
		screensaver->pic = getpic(getconfig("screensaver_pic", NULL));
		if(screensaver->pic != NULL)
		{
			screensaver->screennode->width = screensaver->pic->width;
			screensaver->screennode->height = screensaver->pic->height;
		}
		else
		{
			screensaver->screennode->width = 200;
			screensaver->screennode->height = 200;
		}
	}
	else
	{
		screensaver->type = 3;
		screensaver->speed = getconfigint("screensaver_interval", NULL) * 1000;
		screensaver->path = getconfig("screensaver_type", NULL);
		screensaver->screen = getscreen("screensaveradjust");
		screensaver->filelist = getscreennode(screensaver->screen, "filelist");
		delmarkedscreennodes(screensaver->screen, FILELISTDELMARK);
		changemask(screensaver->filelist, "*.mvi");
		changeinput(screensaver->filelist, screensaver->path);
		createfilelist(screensaver->screen, screensaver->filelist, 0);
		
	}

	return 0;
}

#endif
