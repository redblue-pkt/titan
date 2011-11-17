#ifndef UNICABLE_H
#define UNICALE_H

void freeunicable(struct unicable* node)
{
	int i = 0;

	if(node == NULL) return;

	for(i = 0; i < MAXUNICABLE; i++)
		free(node[i].name);

	free(node);
}

struct unicable* readunicable(const char* filename)
{
	debug(1000, "in");
	FILE *fd = NULL;
	char *fileline = NULL, *name = NULL;
	int linecount = 2, ret = 0;
	struct unicable *unicable = NULL;

	unicable = malloc(sizeof(struct unicable) * MAXUNICABLE);
	if(unicable == NULL)
	{
		err("no mem");
		return NULL;
	}
	memset(unicable, 0, sizeof(struct unicable) * MAXUNICABLE);

	fileline = malloc(MINMALLOC);
	if(fileline == NULL)
	{
		free(unicable);
		err("no mem");
		return NULL;
	}

	name = malloc(MINMALLOC);
	if(name == NULL)
	{
		free(unicable);
		free(fileline);
		err("no mem");
		return NULL;
	}

	fd = fopen(filename, "r");
	if(fd == NULL)
	{
		perr("can't open %s", filename);
		free(unicable);
		free(fileline);
		free(name);
		return NULL;
	}

	while(fgets(fileline, MINMALLOC, fd) != NULL)
	{
		if(fileline[0] == '#' || fileline[0] == '\n')
			continue;
		if(fileline[strlen(fileline) - 1] == '\n')
			fileline[strlen(fileline) - 1] = '\0';
		if(fileline[strlen(fileline) - 1] == '\r')
			fileline[strlen(fileline) - 1] = '\0';

		if(linecount >= MAXUNICABLE) break;

		ret = sscanf(fileline, "%[^#]#%d#%d#%d#%d#%d#%d#%d#%d#%d#%d#%d", name, &unicable[linecount].satcr1, &unicable[linecount].satcr2, &unicable[linecount].satcr3, &unicable[linecount].satcr4, &unicable[linecount].satcr5, &unicable[linecount].satcr6, &unicable[linecount].satcr7, &unicable[linecount].satcr8, &unicable[linecount].pos, &unicable[linecount].lofl, &unicable[linecount].lofh);

		if(ret != 12)
		{
			err("unicable line %d not ok or double", linecount - 1);
			continue;
		}

		unicable[linecount].name = ostrcat(name, NULL, 0, 0); 
		linecount++;
	}

	unicable[0].name = ostrcat(_("undefined"), NULL, 0, 0);
	unicable[0].satcr1 = 0;
	unicable[0].satcr2 = 0;
	unicable[0].satcr3 = 0;
	unicable[0].satcr4 = 0;
	unicable[0].satcr5 = 0;
	unicable[0].satcr6 = 0;
	unicable[0].satcr7 = 0;
	unicable[0].satcr8 = 0;

	unicable[1].name = ostrcat(_("Universal"), NULL, 0, 0);
	unicable[1].satcr1 = 1284;
	unicable[1].satcr2 = 1400;
	unicable[1].satcr3 = 1516;
	unicable[1].satcr4 = 1632;
	unicable[1].satcr5 = 1748;
	unicable[1].satcr6 = 1864;
	unicable[1].satcr7 = 1980;
	unicable[1].satcr8 = 2096;

	free(fileline);
	free(name);
	fclose(fd);
	return unicable;
}

void changeunicable(char* usel, struct unicable* ucable, struct skin* tmp)
{
	int i = 0;

	changeinput(tmp, NULL);
	changechoiceboxvalue(tmp, NULL);
	if(ucable == NULL) return;
	if(tmp == NULL) return;

	tmp->aktline = 0;
	tmp->aktpage = 0;

	for(i = 0; i < MAXUNICABLE; i++)
	{
		if(ostrcmp(usel, ucable[i].name) == 0)
			break;
	}
	if(i >= MAXUNICABLE) return; //nothing found

	if(ucable[i].satcr1 != 0) addchoicebox(tmp, "0", "1");
	if(ucable[i].satcr2 != 0) addchoicebox(tmp, "1", "2");
	if(ucable[i].satcr3 != 0) addchoicebox(tmp, "2", "3");
	if(ucable[i].satcr4 != 0) addchoicebox(tmp, "3", "4");
	if(ucable[i].satcr5 != 0) addchoicebox(tmp, "4", "5");
	if(ucable[i].satcr6 != 0) addchoicebox(tmp, "5", "6");
	if(ucable[i].satcr7 != 0) addchoicebox(tmp, "6", "7");
	if(ucable[i].satcr8 != 0) addchoicebox(tmp, "7", "8");
}

void screenunicable()
{
	int rcret = 0, i = 0;
	struct skin* unicable = getscreen("unicable");
	struct skin* listbox = getscreennode(unicable, "listbox");
	struct skin* tmp = NULL;
	struct dvbdev* dvbnode = dvbdev;
	struct unicable* ucable = NULL;
	char* tmpstr = NULL, *tmpnr = NULL, *usel = NULL;

	listbox->aktline = 1;
	listbox->aktpage = -1;

	ucable = readunicable(getconfig("unicableconfig", NULL));
	if(ucable == NULL) return;

	while(dvbnode != NULL)
	{
		if(dvbnode->type == FRONTENDDEV && dvbnode->feinfo != NULL && dvbnode->feinfo->type == FE_QPSK)
		{
			tmp = addlistbox(unicable, listbox, tmp, 1);
			if(tmp != NULL)
			{
				tmp->handle = (char*)dvbnode;

				tmpnr = oitoa(dvbnode->adapter);
				tmpstr = ostrcat(_("Tuner "), tmpnr, 0, 1);
				tmpstr = ostrcat(tmpstr, "/", 1, 0);
				tmpnr = oitoa(dvbnode->devnr);
				tmpstr = ostrcat(tmpstr, tmpnr, 1, 1);
				tmpstr = ostrcat(tmpstr, ": ", 1, 0);
				changetext(tmp, tmpstr);
				free(tmpstr); tmpstr = NULL;
				changename(tmp, dvbnode->feshortname);
				tmp->type = CHOICEBOX;
				
				for(i = 0; i < MAXUNICABLE; i++)
				{
					if(ucable[i].name != NULL)
						addchoicebox(tmp, ucable[i].name, ucable[i].name);
				}
				setchoiceboxselection(tmp, getconfig("unicable", NULL));
			}

			status.screencalc = 2;
			drawscreen(unicable, 0);
			status.screencalc = 0;
			usel = tmp->ret;

			tmp = addlistbox(unicable, listbox, tmp, 1);
			if(tmp != NULL)
			{
				changetext(tmp, "SATCR:");
				tmp->type = CHOICEBOX;
				changeunicable(usel, ucable, tmp);
			}

			tmp = addlistbox(unicable, listbox, tmp, 1);
			if(tmp != NULL)
			{
				tmp->deaktivcol = 1;
				tmp->height = 20;
			}
		}
		dvbnode = dvbnode->next;
	}

	drawscreen(unicable, 0);
	addscreenrc(unicable, listbox);

	tmp = listbox->select;
	while(1)
	{
		addscreenrc(unicable, tmp);
		rcret = waitrc(unicable, 0, 0);
		tmp = listbox->select;

		drawscreen(unicable, 0);

		if(rcret == getrcconfigint("rcexit", NULL)) break;
		if(rcret == getrcconfigint("rcok", NULL)) break;
		if(rcret == getrcconfigint("rcgreen", NULL))
		{
			break;
		}
		if(listbox->select != NULL && listbox->select->handle != NULL)
		{
			if(rcret == getrcconfigint("rcleft", NULL) || rcret == getrcconfigint("rcright", NULL))
			{
				changeunicable(listbox->select->ret, ucable, listbox->select->next);
				drawscreen(unicable, 0);
			}
		}
	}

	free(ucable);
	delconfigtmpall();
	delmarkedscreennodes(unicable, 1);
	delownerrc(unicable);
	clearscreen(unicable);
}

#endif
