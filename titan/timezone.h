#ifndef TIMEZONE_H
#define TIMEZONE_H

int readtimezone(struct skin* timezone, struct skin* listbox)
{
	FILE *fd = NULL;
	char *fileline = NULL, *filename = NULL;
	char *pos = NULL, *name = NULL, *zone = NULL;
	struct skin* node = NULL;
	int len = 0;

	filename = getconfig("timezonefile", NULL);

	fileline = malloc(MINMALLOC);
	if(fileline == NULL)
	{
		err("no memory");	
		return 1;
	}

	fd = fopen(filename, "r");
	if(fd == NULL)
	{
		perr("can't open %s", filename);
		free(fileline);
		return 1;
	}

	while(fgets(fileline, MINMALLOC, fd) != NULL)
	{
		if(fileline[0] == '#' || fileline[0] == '\n')
			continue;
		len = strlen(fileline) - 1;
		if(len >= 0 && fileline[len] == '\n')
			fileline[len] = '\0';
		len--;
		if(len >= 0 && fileline[len] == '\r')
			fileline[len] = '\0';

		pos = ostrstr(fileline, "<zone ");
		if(pos == NULL)
			continue;

		name = getxmlentry(fileline, " name=");
		if(name == NULL)
			continue;
		zone = getxmlentry(fileline, " zone=");
		if(zone == NULL)
		{
			free(name);
			continue;
		}

		node = addlistbox(timezone, listbox, node, 1);
		if(node != NULL)
		{
			changetext(node, name);
			changename(node, name);
			changeinput(node, zone);
		}

		free(name);
		free(zone);
	}

	fclose(fd);
	free(fileline);
	return 0;
}

void screentimezone()
{
	int rcret = 0, ret = 0;
	struct skin* timezone = getscreen("timezone");
	struct skin* listbox = getscreennode(timezone, "listbox");

	readtimezone(timezone, listbox);

	setlistboxselection(listbox, getconfig("timezonename", NULL));
	drawscreen(timezone, 0, 0);
	addscreenrc(timezone, listbox);

	while(1)
	{
		rcret = waitrc(timezone, 0, 0);

		if(rcret == getrcconfigint("rcexit", NULL)) break;
		if(rcret == getrcconfigint("rcok", NULL))
		{
			if(listbox->select != NULL)
			{
				ret = settimezone(listbox->select->input);
				if(ret == 0)
				{
					addconfig("timezone", listbox->select->input);
					addconfig("timezonename", listbox->select->name);
				}
			}
			break;
		}
	}

	delmarkedscreennodes(timezone, 1);
	delownerrc(timezone);
	clearscreen(timezone);
}

#endif
