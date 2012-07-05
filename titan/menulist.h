#ifndef MENULIST_H
#define MENULIST_H

void freemenulist(struct menulist* mlist, int delparam)
{
	debug(1000, "in");
	struct menulist *node = mlist, *prev = mlist;

	while(node != NULL)
	{
		prev = node;
		node = node->next;
		if(prev != NULL)
		{
			free(prev->name);
			prev->name = NULL;
			
			free(prev->text);
			prev->text = NULL;

			free(prev->pic);
			prev->pic = NULL;

			if(delparam == 1)
			{
				free(prev->param);
				prev->param = NULL;

				free(prev->param1);
				prev->param1 = NULL;
			}

			free(prev);
			prev = NULL;
		}
	}
	debug(1000, "out");
}

void changemenulistparam(struct menulist* mlist, char* param, char* param1)
{
	if(mlist != NULL)
	{
		free(mlist->param);
		mlist->param = ostrcat(param, NULL, 0, 0);

		free(mlist->param1);
		mlist->param1 = ostrcat(param1, NULL, 0, 0);
	}
}

void setmenulistdefault(struct menulist* mlist, char* defaultentry)
{
	while(mlist != NULL)
	{
		if(ostrcmp(defaultentry, mlist->name) == 0)
			mlist->defaultentry = 1;
		mlist = mlist->next;
	}
}

void addmenulistall(struct menulist** mlist, char* allname, char* pic, int deaktiv, char* defaultentry)
{
	char* saveptr = NULL, *token = NULL, *tmpstr = NULL;

	tmpstr = ostrcat(allname, NULL, 0, 0);
	if(tmpstr == NULL) return;

	token = strtok_r(tmpstr, "\n", &saveptr);
	while(token != NULL)
	{
		if(ostrcmp(defaultentry, token) == 0)
			addmenulist(mlist, token, NULL, pic, deaktiv, 1);
		else
			addmenulist(mlist, token, NULL, pic, deaktiv, 0);
		token = strtok_r(NULL, "\n", &saveptr);
	}

	free(tmpstr); tmpstr = NULL;
}

struct menulist* addmenulist(struct menulist** mlist, char* name, char* text, char* pic, int deaktiv, int defaultentry)
{
	struct menulist *newnode = NULL, *prev = NULL, *node = *mlist;

	if(mlist == NULL) return NULL;

	newnode = (struct menulist*)malloc(sizeof(struct menulist));	
	if(newnode == NULL)
	{
		err("no memory");
		return NULL;
	}

	memset(newnode, 0, sizeof(struct menulist));

	newnode->name = ostrcat(name, NULL, 0, 0);
	newnode->text = ostrcat(text, NULL, 0, 0);
	newnode->pic = ostrcat(pic, NULL, 0, 0);
	newnode->deaktiv = deaktiv;
	newnode->defaultentry = defaultentry;

	while(node != NULL)
	{
		prev = node;
		node = node->next;
	}

	if(prev == NULL)
		*mlist = newnode;
	else
		prev->next = newnode;
	newnode->next = node;

	return newnode;
}

// showpng = 0 (no icon)
// showpng = 1 (smal icon)
// showpng = 2 (big icon)
//flag 1: rcgreen = subchannel
struct menulist* menulistboxext(struct menulist* mlist, char* paramskinname, char* skintitle, char* paramskinpath, char* defaultpic, int showpng, int* rcreturn, int flag)
{
	debug(1000, "in");
	int rcret = 0, fromthread = 0;
	struct skin* framebuffer = getscreen("framebuffer");
	struct skin* tmp = NULL;
	struct menulist* ret = NULL;
	char* bg = NULL;
	char* skinname = NULL;
	char* skinpath = NULL;
	char* tmppic = NULL;
	char* tmpstr = NULL;

	if(pthread_self() != status.mainthread)
	{
		fromthread = 1;
		flag = 0; //in thread modus only flag 0 alowed (flag 1 not thread save)
	}

	if(paramskinname == NULL)
		skinname = ostrcat("menulist", NULL, 0, 0);
	else
		skinname = ostrcat(paramskinname, NULL, 0, 0);

	if(paramskinpath == NULL)
		skinpath = ostrcat("skin", NULL, 0, 0);
	else
		skinpath = ostrcat(paramskinpath, NULL, 0, 0);

	struct skin* screen = getscreen(skinname);
	struct skin* listbox = getscreennode(screen, "listbox");
	struct skin* titletext = getscreennode(screen, "titletext");
	
	listbox->aktpage = -1;
	listbox->aktline = 1;

	changetitle(screen, _(skintitle));
	if(titletext != status.skinerr) changetext(titletext, _(skintitle));

	while(mlist != NULL)
	{
		tmp = addlistbox(screen, listbox, tmp, 1);
		
		if(tmp != NULL)
		{
			changetext(tmp, _(mlist->name));
			changename(tmp, mlist->name);
			
			if(mlist->text != NULL)
			{
				changetext2(tmp, _(mlist->text));
				tmp->textposx2 = 270;
				tmp->type = TEXTBOX;
				tmp->wrap = YES;
			}
			
			tmp->handle = (char*)mlist;

			if(showpng > 0 && mlist->name != NULL) 
			{
				tmp->textposx = 120;
				tmp->height = 50;
				tmp->valign = convertxmlentry("middle", 0);
				tmp->hspace = 5;
				
				if(showpng == 2)
				{
					tmp->valign = convertxmlentry("middle", 0);
					tmp->textposx = 250;
					tmp->height = 170;
				}

				if(mlist->pic == NULL)
				{
					mlist->pic = ostrcat(mlist->name, ".png", 0, 0);	
					stringreplacechar(mlist->pic, ' ', '_');
					stringreplacechar(mlist->pic, ':', '_');
					stringreplacechar(mlist->pic, '/', '_');
					string_tolower(mlist->pic);
				}

				if(mlist->pic != NULL)
				{
          if(mlist->pic[0] != '/')
          {
            tmppic = ostrcat(skinpath, "/", 0, 0);	
            tmppic = ostrcat(tmppic, mlist->pic, 1, 0);
          }
          else
            tmppic = ostrcat(mlist->pic, NULL, 0, 0);
          
					tmpstr = changepicpath(tmppic);
					if(!file_exist(tmpstr))
					{
						free(mlist->pic); mlist->pic = NULL;
						free(tmppic); tmppic = NULL;
					}
					free(tmpstr); tmpstr = NULL;
				}

				if(mlist->pic == NULL)
				{
					if(defaultpic == NULL)
						tmppic = ostrcat(getconfig("skinpath", NULL), "/skin/default.png", 0, 0);
					else
						tmppic = ostrcat(getconfig("skinpath", NULL), defaultpic, 0, 0);
				}
				
				changepic(tmp, tmppic);
				free(tmppic); tmppic = NULL;
			}

			if(mlist->deaktiv == 1)
				tmp->deaktivcol = convertcol("deaktivcol");
		}

		if(mlist->defaultentry == 1)
			setlistboxselection(listbox, mlist->name);

		mlist = mlist->next;
	}

	if(fromthread == 1)
	{
		m_lock(&status.drawingmutex, 0);
		m_lock(&status.rcmutex, 10);
		setnodeattr(screen, framebuffer, 2);
		status.rcowner = screen;
		bg = savescreen(screen);
		drawscreen(screen, 0, 2);
	}
	else
		drawscreen(screen, 0, 0);

	addscreenrc(screen, listbox);

	while(1)
	{
		rcret = waitrc(screen, 0, 0);
		if(rcreturn != NULL) *rcreturn = rcret;

		if(rcret == getrcconfigint("rcexit", NULL)) break;
		if(flag == 1 && rcret == getrcconfigint("rcgreen", NULL))
		{
			clearscreen(screen);
			screenlinkedchannel();
			break;
		}
		if(rcret == getrcconfigint("rcok", NULL) || rcret == getrcconfigint("rcred", NULL) || rcret == getrcconfigint("rcgreen", NULL) || rcret == getrcconfigint("rcyellow", NULL) || rcret == getrcconfigint("rcblue", NULL))
		{
			if(rcreturn == NULL && rcret != getrcconfigint("rcok", NULL))
				continue;

			if(listbox->select != NULL)
				ret = (struct menulist*)listbox->select->handle;
			break;
		}
	}

	free(skinname); skinname = NULL;
	free(skinpath); skinpath = NULL;
	delownerrc(screen);
	delmarkedscreennodes(screen, 1);

	if(fromthread == 1)
	{
		clearscreennolock(screen);
		restorescreen(bg, screen);
		blitfb(0);
		sleep(1);
		status.rcowner = NULL;
		m_unlock(&status.rcmutex, 3);
		m_unlock(&status.drawingmutex, 0);
	}
	else
		clearscreen(screen);

	debug(1000, "out");
	return ret;
}

struct menulist* menulistbox(struct menulist* mlist, char* paramskinname, char* skintitle, char* paramskinpath, char* defaultpic, int showpng, int flag)
{
	return menulistboxext(mlist, paramskinname, skintitle, paramskinpath, defaultpic, showpng, NULL, flag);
}
#endif
