#ifndef TEXTINPUTHIST_H
#define TEXTINPUTHIST_H

void removehistory(char* text, char* histname)
{
	int count = 0, i = 0;
	char* tmpstr = NULL, *tmpstr1 = NULL;
	struct splitstr* ret = NULL;

	if(histname == NULL || text == NULL || strlen(text) == 0) return;

	tmpstr = readfiletomem(getconfig(histname, NULL), 0);
	
	ret = strsplit(tmpstr, "\n", &count);

	if(ret != NULL)
	{
		for(i = 0; i < count; i++)
		{
			if(ostrstr(ret[i].part, text) == NULL)
			{
				tmpstr1 = ostrcat(tmpstr1, ret[i].part, 1, 0);
				tmpstr1 = ostrcat(tmpstr1, "\n", 1, 0);
			}
		}
	}

	if(tmpstr1 != NULL && strlen(tmpstr1) > 0)
		tmpstr1[strlen(tmpstr1) - 1] = '\0';

	if(tmpstr1 == NULL)
		writesys(getconfig(histname, NULL), "", 0);
	else
		writesys(getconfig(histname, NULL), tmpstr1, 0);

	free(ret); ret = NULL;
	free(tmpstr); tmpstr = NULL;
	free(tmpstr1); tmpstr1 = NULL;
}

void savehistory(char* text, char* histname)
{
	int count = 0, i = 0;
	char* tmpstr = NULL, *tmpstr1 = NULL;
	struct splitstr* ret = NULL;

	if(histname == NULL || text == NULL || strlen(text) == 0) return;

	tmpstr1 = ostrcat(tmpstr1, text, 1, 0);
	tmpstr1 = ostrcat(tmpstr1, "\n", 1, 0);

	tmpstr = readfiletomem(getconfig(histname, NULL), 0);
	
	ret = strsplit(tmpstr, "\n", &count);

	if(ret != NULL)
	{
		for(i = 0; i < count; i++)
		{
			if(ostrstr(ret[i].part, text) == NULL)
			{
				tmpstr1 = ostrcat(tmpstr1, ret[i].part, 1, 0);
				tmpstr1 = ostrcat(tmpstr1, "\n", 1, 0);
			}
		}
	}

	if(tmpstr1 != NULL && strlen(tmpstr1) > 0)
		tmpstr1[strlen(tmpstr1) - 1] = '\0';

	writesys(getconfig(histname, NULL), tmpstr1, 0);

	free(ret); ret = NULL;
	free(tmpstr); tmpstr = NULL;
	free(tmpstr1); tmpstr1 = NULL;
}

void readhistory(struct skin* textinputhist, struct skin* listbox, char* histname)
{
	int count = 0, i = 0;
	char* tmpstr = NULL;
	struct skin* tmp = NULL;
	struct splitstr* ret = NULL;

	if(histname == NULL) return;

	listbox->aktline = 1;
	listbox->aktpage = -1;

	tmpstr = readfiletomem(getconfig(histname, NULL), 0);
	ret = strsplit(tmpstr, "\n", &count);

	if(ret != NULL)
	{
		for(i = 0; i < count; i++)
		{
			tmp = addlistbox(textinputhist, listbox, tmp, 1);
			if(tmp != NULL)
			{
				changetext(tmp, ret[i].part);
				changename(tmp, ret[i].part);
			}
		}
	}

	free(ret); ret = NULL;
	free(tmpstr); tmpstr = NULL;
}

char* textinputhist(char* title, char* text, char* histname)
{
	int rcret = -1, fromthread = 0, height = 0;
	struct skin* textinputhist = getscreen("textinputhist");
	struct skin* listbox = getscreennode(textinputhist, "listbox");
	struct skin* input = getscreennode(textinputhist, "input");
	struct skin* framebuffer = getscreen("framebuffer");
	char* ret = NULL, *bg = NULL, *tmpstr = NULL;

	if(pthread_self() != status.mainthread)
	fromthread = 1;

	if(title == NULL) title = ostrcat(_("Input field"), NULL, 0, 0);

	changetitle(textinputhist, _(title));

	height = textinputhist->height;
	if(title != NULL)
		textinputhist->height += textinputhist->fontsize + 6 + (textinputhist->bordersize * 2);

	if(text != NULL)
		tmpstr = ostrcat(tmpstr, text, 1, 0);

	changeinput(input, tmpstr);
	readhistory(textinputhist, listbox, histname);

	if(fromthread == 1)
	{
		delrc(getrcconfigint("rcvolup", NULL), NULL, NULL);
		delrc(getrcconfigint("rcvoldown", NULL), NULL, NULL);
		delrc(getrcconfigint("rcmute", NULL), NULL, NULL);
		m_lock(&status.drawingmutex, 0);
		m_lock(&status.rcmutex, 10);
		setnodeattr(textinputhist, framebuffer, 2);
		status.rcowner = textinputhist;
		bg = savescreen(textinputhist);
		drawscreen(textinputhist, 0, 2);
	}
	else
		drawscreen(textinputhist, 0, 0);

	addrc(getrcconfigint("rcup", NULL), listboxup, textinputhist, listbox);
	addrc(getrcconfigint("rcdown", NULL), listboxdown, textinputhist, listbox);
	addscreenrc(textinputhist, input);

	while(1)
	{
		rcret = waitrc(textinputhist, 0, 0);
		if(rcret == getrcconfigint("rcexit", NULL)) break;
		
		if(rcret == getrcconfigint("rcok", NULL) && tmpstr != NULL && listbox->select != NULL && (input->input == NULL || ostrcmp(input->input, " ") == 0))
		{
//			changeinput(input, listbox->select->name);
			changeinput(input, listbox->select->text);

			if(fromthread == 1)
				drawscreen(textinputhist, 0, 2);
			else
				drawscreen(textinputhist, 0, 0);
			
//			ret = ostrcat(listbox->select->name, NULL, 0, 0);
			ret = ostrcat(listbox->select->text, NULL, 0, 0);
			strstrip(ret);
			savehistory(ret, histname);
			break;
		}
		else if(rcret == getrcconfigint("rcok", NULL) && input->input != NULL)
		{
			ret = ostrcat(input->input, NULL, 0, 0);
			strstrip(ret);
			savehistory(ret, histname);
			break;
		}
		
		if(rcret == getrcconfigint("rcyellow", NULL) && listbox->select != NULL)
		{
			changeinput(input, listbox->select->name);
			if(fromthread == 1)
				drawscreen(textinputhist, 0, 2);
			else
				drawscreen(textinputhist, 0, 0);
			
			writerc(getrcconfigint("rctext", NULL));
		}

		if(rcret == getrcconfigint("rcred", NULL))
		{
			free(tmpstr), tmpstr = NULL;
			tmpstr = ostrcat(tmpstr, NULL, 1, 0);
			changeinput(input, tmpstr);
			if(fromthread == 1)
				drawscreen(textinputhist, 0, 2);
			else
				drawscreen(textinputhist, 0, 0);
		}

		if(rcret == getrcconfigint("rcgreen", NULL) && listbox->select != NULL)
		{
			ret = ostrcat(listbox->select->name, NULL, 0, 0);
			removehistory(ret, histname);
			free(ret); ret = NULL;
			listbox->aktline = 1;
			listbox->aktpage = -1;
			delmarkedscreennodes(textinputhist, 1);
			readhistory(textinputhist, listbox, histname);
			if(fromthread == 1)
				drawscreen(textinputhist, 0, 2);
			else
				drawscreen(textinputhist, 0, 0);
		}
	}

	free(tmpstr), tmpstr = NULL;
	delownerrc(textinputhist);
	delrc(getrcconfigint("rcup", NULL), textinputhist, listbox);
	delrc(getrcconfigint("rcdown", NULL), textinputhist, listbox);

	if(fromthread == 1)
	{
		clearscreennolock(textinputhist);
		restorescreen(bg, textinputhist);
		blitfb(0);
		sleep(1);
		status.rcowner = NULL;
		m_unlock(&status.rcmutex, 3);
		m_unlock(&status.drawingmutex, 0);
		addrc(getrcconfigint("rcvolup", NULL), screenvolumeup, NULL, NULL);
		addrc(getrcconfigint("rcvoldown", NULL), screenvolumedown, NULL, NULL);
		addrc(getrcconfigint("rcmute", NULL), screenmute, NULL, NULL);
	}
	else
	{
		clearscreen(textinputhist);
		drawscreen(skin, 0, 0);
	}

	textinputhist->height = height;
	delmarkedscreennodes(textinputhist, 1);
	return ret;
}

#endif
