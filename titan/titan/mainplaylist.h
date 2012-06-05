#ifndef MAINPLAYLIST_H
#define MAINPLAYLIST_H

void debugmainplaylist()
{
	struct mainplaylist* node = mainplaylist;

	while(node != NULL)
	{
		printf("mainplaylist = %s\n", node->name);	
		node = node->next;
	}
}

struct mainplaylist* getlastmainplaylist(struct mainplaylist* node)
{
	debug(1000, "in");
	struct mainplaylist *prev = NULL;

	while(node != NULL)
	{
		prev = node;
		node = node->next;
	}

	debug(1000, "out");
	return prev;
}

int movemainplaylistdown(struct mainplaylist* node)
{
	struct mainplaylist* prev = NULL, *next = NULL;

	if(node == NULL || mainplaylist == NULL)
	{
		debug(1000, "NULL detect");
		return 1;
	}

	//last node
	if(node->next == NULL)
	{
		if(node->prev != NULL)
			node->prev->next = NULL;
		node->prev = NULL;
		node->next = mainplaylist;
		mainplaylist->prev = node;
		mainplaylist = node;
		return 0;
	}

	//haenge node aus 
	if(node->prev != NULL)
		node->prev->next = node->next;
	else
		mainplaylist = node->next;
	node->next->prev = node->prev;

	//save nodes next and prev
	next = node->next;
	prev = node->prev;

	//haenge es eine pos nacher ein
	node->next = next->next;
	node->prev = next;
	
	if(next->next != NULL)
		next->next->prev = node;
	next->next = node;

	status.writemainplaylist = 1;
	return 0;
}

int movemainplaylistup(struct mainplaylist* node)
{
	struct mainplaylist* prev = NULL, *next = NULL, *last = NULL;

	if(node == NULL || mainplaylist == NULL)
	{
		debug(1000, "NULL detect");
		return 1;
	}

	//first node
	if(node->prev == NULL)
	{
		last = getlastmainplaylist(mainplaylist);

		if(node->next != NULL)
			node->next->prev = NULL;
		mainplaylist = node->next;
		node->next = NULL;
		last->next = node;
		node->prev = last;
		return 0;
	}

	//haenge node aus 
	node->prev->next = node->next;
	if(node->next != NULL)
		node->next->prev = node->prev;

	//save nodes next and prev
	next = node->next;
	prev = node->prev;

	//haenge es eine pos voher ein
	node->next = prev;
	node->prev = prev->prev;
	
	if(prev->prev != NULL)
		prev->prev->next = node;
	else
		mainplaylist = node;
	prev->prev = node;

	status.writemainplaylist = 1;
	return 0;
}

struct mainplaylist* addmainplaylist(char *line, int count, struct mainplaylist* last)
{
	debug(1000, "in");
	struct mainplaylist *newnode = NULL, *prev = NULL, *node = mainplaylist;
	char *name = NULL, *filename = NULL;
	int ret = 0;

	newnode = (struct mainplaylist*)malloc(sizeof(struct mainplaylist));	
	if(newnode == NULL)
	{
		err("no memory");
		return NULL;
	}

	name = malloc(MINMALLOC);
	if(name == NULL)
	{
		err("no memory");
		free(newnode);
		return NULL;
	}

	filename = malloc(MINMALLOC);
	if(filename == NULL)
	{
		err("no memory");
		free(newnode);
		free(name);
		return NULL;
	}

	memset(newnode, 0, sizeof(struct mainplaylist));
	status.writemainplaylist = 1;

	ret = sscanf(line, "%[^#]#%[^#]", name, filename);
	if(ret != 2)
	{
		if(count > 0)
		{
			err("mainplaylist line %d not ok", count);
		}
		else
		{
			err("add mainplaylist");
		}
		free(name);
		free(filename);
		free(newnode);
		return NULL;
	}

	newnode->name = ostrcat(name, NULL, 1, 0);
	newnode->filename = ostrcat(filename, NULL, 1, 0);

	if(last == NULL)
	{
		while(node != NULL)
		{
			prev = node;
			node = node->next;
		}
	}
	else
	{
		prev = last;
		node = last->next;
	}

	if(prev == NULL)
		mainplaylist = newnode;
	else
	{
		prev->next = newnode;
		newnode->prev = prev;
	}
	newnode->next = node;
	if(node != NULL) node->prev = newnode;

	debug(1000, "out");
	return newnode;
}

int readmainplaylist(char* filename)
{
	debug(1000, "in");
	FILE *fd = NULL;
	char *fileline = NULL;
	int linecount = 0;
	struct mainplaylist* last = NULL, *tmplast = NULL;

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
		if(fileline[strlen(fileline) - 1] == '\n')
			fileline[strlen(fileline) - 1] = '\0';
		if(fileline[strlen(fileline) - 1] == '\r')
			fileline[strlen(fileline) - 1] = '\0';

		linecount++;

		if(last == NULL) last = tmplast;
		last = addmainplaylist(fileline, linecount, last);
		if(last != NULL) tmplast = last;
	}

	status.writemainplaylist = 0;
	free(fileline);
	fclose(fd);
	return 0;
}

int readallplaylist()
{
	int ret = 0;
	struct mainplaylist* node = mainplaylist;

	while(node != NULL)
	{
		if(readplaylist(node->filename, &node->playlist) != 0)
			ret = 1;
		node = node->next;
	}
	
	return ret;
}

struct mainplaylist* getmainplaylistbyplaylistpointer(struct playlist* playlistnode)
{
	debug(1000, "in");
	struct mainplaylist *node = mainplaylist;
	struct playlist *bnode = NULL;

	while(node != NULL)
	{
		bnode = node->playlist;
		while(bnode != NULL)
		{
			if(bnode == playlistnode)
			{
				debug(1000, "out");
				return node;
			}
			bnode = bnode->next;
		}
		node = node->next;
	}
	debug(100, "mainplaylist not found (%p)", playlistnode);
	return NULL;
}

struct mainplaylist* getmainplaylist(char *name)
{
	debug(1000, "in");
	struct mainplaylist *node = mainplaylist;

	while(node != NULL)
	{
		if(ostrcmp(node->name, name) == 0)
		{
			debug(1000, "out");
			return node;
		}

		node = node->next;
	}
	debug(100, "mainplaylist not found (%s)", name);
	return NULL;
}

void delmainplaylist(char *name)
{
	debug(1000, "in");
	struct mainplaylist *node = mainplaylist, *prev = mainplaylist;

	while(node != NULL)
	{
		if(ostrcmp(node->name, name) == 0)
		{
			status.writemainplaylist = 1;
			if(node == mainplaylist)
			{
				mainplaylist = node->next;
				if(mainplaylist != NULL)
					mainplaylist->prev = NULL;
			}
			else
			{
				prev->next = node->next;
				if(node->next != NULL)
					node->next->prev = prev;
			}

			free(node->name);
			node->name = NULL;

			free(node->filename);
			node->filename = NULL;

			free(node);
			node = NULL;
			break;
		}

		prev = node;
		node = node->next;
	}
	debug(1000, "out");
}

void freemainplaylist()
{
	debug(1000, "in");
	struct mainplaylist *node = mainplaylist, *prev = mainplaylist;

	while(node != NULL)
	{
		prev = node;
		node = node->next;
		if(prev != NULL)
			delmainplaylist(prev->name);
	}
	debug(1000, "out");
}

void freeallplaylist()
{
	struct mainplaylist* node = mainplaylist;

	while(node != NULL)
	{
		freeplaylist(&node->playlist);
		node = node->next;
	}
}

int writemainplaylist(const char *filename)
{
	debug(1000, "in");
	FILE *fd = NULL;
	struct mainplaylist *node = mainplaylist;
	int ret = 0;

	fd = fopen(filename, "w");
	if(fd == NULL)
	{
		perr("can't open %s", filename);
		return 1;
	}

	while(node != NULL)
	{
		ret = fprintf(fd, "%s#%s\n", node->name, node->filename);
		if(ret < 0)
		{
			perr("writting file %s", filename);
		}
		node = node->next;
	}

	fclose(fd);
	debug(1000, "out");
	return 0;
}

int writeallplaylist()
{
	struct mainplaylist* node = mainplaylist;
	int ret = 0;

	while(node != NULL)
	{
		if(writeplaylist(node->filename, node->playlist) != 0)
			ret = 1;
		node = node->next;
	}

	return ret;
}

//flag 0: editmodus
//flag 1: showmodus
struct mainplaylist* screenmainplaylist(int flag)
{
	int rcret = 0;
	struct skin* playlist = getscreen("playlist");
	struct skin* listbox = getscreennode(playlist, "listbox");
	struct skin* b3 = getscreennode(playlist, "b3");
	struct skin* b4 = getscreennode(playlist, "b4");
	struct skin* tmp = NULL;
	struct mainplaylist* node = mainplaylist;
	struct mainplaylist* ret = NULL;
	char* newentry = NULL, *tmpstr = NULL;

	if(flag == 0)
	{
		b3->hidden = NO;
		b4->hidden = NO;
	}
	else
	{
		b3->hidden = YES;
		b4->hidden = YES;
	}

start:
	tmp = NULL;
	node = mainplaylist;
	delmarkedscreennodes(playlist, 1);

	while(node != NULL)
	{
		tmp = addlistbox(playlist, listbox, tmp, 1);
		if(tmp != NULL)
		{
			changetext(tmp, node->name);
			tmp->handle = (char*)node;
			tmp->del = 1;
		}

		node = node->next;
	}

	drawscreen(playlist, 0);
	addscreenrc(playlist, listbox);

	while(1)
	{
		rcret = waitrc(playlist, 0, 0);

		if(rcret == getrcconfigint("rcexit", NULL)) break;
		if(rcret == getrcconfigint("rcok", NULL))
		{
			if(listbox->select != NULL)
			{
				if(flag == 0)
				{
					clearscreen(playlist);
					screenplaylist((struct mainplaylist*)listbox->select->handle);
					goto start;
				}
				else
				{
					ret = (struct mainplaylist*)listbox->select->handle;
					break;
				}
			}
		}
		if(flag == 0 && rcret == getrcconfigint("rcred", NULL))
		{
			if(listbox->select != NULL && listbox->select->handle != NULL)
				delmainplaylist(((struct mainplaylist*)listbox->select->handle)->name);

			goto start;
		}
		if(flag == 0 && rcret == getrcconfigint("rcgreen", NULL))
		{
			//clearscreen(playlist);
			newentry = textinput(NULL, "playlist");
			if(newentry != NULL)
			{
				newentry = stringreplacechar(newentry, '#', '_');
				tmpstr = ostrcat(newentry, "#", 0, 0);
				tmpstr = ostrcat(tmpstr, getconfig("playlistpath", NULL), 1, 0);
				tmpstr = ostrcat(tmpstr, "/", 1, 0);
				tmpstr = ostrcat(tmpstr, newentry, 1, 1);
				tmpstr = ostrcat(tmpstr, ".pls", 1, 0);
				addmainplaylist(tmpstr, 1, NULL);
			}
			free(tmpstr); tmpstr = NULL;
			goto start;
		}
	}

	delmarkedscreennodes(playlist, 1);
	delownerrc(playlist);
	clearscreen(playlist);
	return ret;
}

#endif
