#ifndef PLAYLIST_H
#define PLAYLIST_H

void debugplaylist()
{
	struct mainplaylist* mainplaylistnode = mainplaylist;
	struct playlist* playlistnode = NULL;

	while(mainplaylistnode != NULL)
	{
		printf("%s\n", mainplaylistnode->name);
		playlistnode = mainplaylistnode->playlist;
		while(playlistnode != NULL)
		{
			printf("%p <%p >%p\n", playlistnode, playlistnode->prev, playlistnode->next);
			playlistnode = playlistnode->next;
		}
		mainplaylistnode = mainplaylistnode->next;
	}
}

struct playlist* getlastplaylist(struct playlist* node)
{
	debug(1000, "in");
	struct playlist *prev = NULL;

	while(node != NULL)
	{
		prev = node;
		node = node->next;
	}

	debug(1000, "out");
	return prev;
}

int moveplaylistdown(struct playlist* node)
{
	struct playlist* prev = NULL, *next = NULL;
	struct mainplaylist* mainplaylistnode = NULL;

	if(node == NULL)
	{
		debug(1000, "NULL detect");
		return 1;
	}

	mainplaylistnode = getmainplaylistbyplaylistpointer(node);
	if(mainplaylistnode == NULL)
	{
		debug(1000, "NULL detect");
		err("NULL detect");
		return 1;
	}
	
	//only one node
	if(node->prev == NULL && node->next == NULL)
		return 0;

	//last node
	if(node->next == NULL)
	{
		if(node->prev != NULL)
			node->prev->next = NULL;
		node->prev = NULL;
		node->next = mainplaylistnode->playlist;
		mainplaylistnode->playlist->prev = node;
		mainplaylistnode->playlist = node;
		return 0;
	}

	//haenge node aus 
	if(node->prev != NULL)
		node->prev->next = node->next;
	else
		mainplaylistnode->playlist = node->next;
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

	status.writeplaylist = 1;
	return 0;
}

int moveplaylistup(struct playlist* node)
{
	struct playlist* prev = NULL, *next = NULL, *last = NULL;
	struct mainplaylist* mainplaylistnode = NULL;

	if(node == NULL)
	{
		debug(1000, "NULL detect");
		return 1;
	}

	mainplaylistnode = getmainplaylistbyplaylistpointer(node);
	if(mainplaylistnode == NULL)
	{
		debug(1000, "NULL detect");
		return 1;
	}
	
	//only one node
	if(node->prev == NULL && node->next == NULL)
		return 0;

	//first node
	if(node->prev == NULL)
	{
		last = getlastplaylist(mainplaylistnode->playlist);

		if(node->next != NULL)
			node->next->prev = NULL;
		mainplaylistnode->playlist = node->next;
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
		mainplaylistnode->playlist = node;
	prev->prev = node;

	status.writeplaylist = 1;
	return 0;
}

struct playlist* getplaylist(char* file)
{
	struct mainplaylist* mainplaylistnode = mainplaylist;
	struct playlist* playlistnode = NULL;

	while(mainplaylistnode != NULL)
	{
		playlistnode = mainplaylistnode->playlist;
		while(playlistnode != NULL)
		{
			if(ostrcmp(playlistnode->file, file) == 0)
				return playlistnode;

			playlistnode = playlistnode->next;
		}
		mainplaylistnode = mainplaylistnode->next;
	}

	return NULL;
}

struct playlist* addplaylist(struct playlist **firstnode, char *line, int count, struct playlist* last)
{
	//debug(1000, "in");
	struct playlist *newnode = NULL, *prev = NULL, *node = *firstnode;
	int ret = 0;
	char* file = NULL;

	newnode = (struct playlist*)malloc(sizeof(struct playlist));	
	if(newnode == NULL)
	{
		err("no memory");
		return NULL;
	}

	memset(newnode, 0, sizeof(struct playlist));

	file = malloc(MINMALLOC);
	if(file == NULL)
	{
		err("no memory");
		free(newnode);
		return NULL;
	}

	status.writeplaylist = 1;

	ret = sscanf(line, "%s", file);
	if(ret != 1)
	{
		if(count > 0)
		{
			err("playlist line %d not ok", count);
		}
		else
		{
			err("add playlist");
		}
		free(newnode);
		free(file);
		return NULL;
	}

	free(file);
	newnode->file = ostrcat(line, NULL, 0, 0);

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
		*firstnode = newnode;
	else
	{
		prev->next = newnode;
		newnode->prev = prev;
	}
	newnode->next = node;
	if(node != NULL) node->prev = newnode;

	//debug(1000, "out");
	return newnode;
}

int readplaylist(char* filename, struct playlist** firstnode)
{
	debug(1000, "in");
	FILE *fd = NULL;
	char *fileline = NULL;
	int linecount = 0, len = 0;
	struct playlist* last = NULL, *tmplast = NULL;

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
		if(fileline[len] == '\n')
			fileline[len] = '\0';
		if(fileline[len] == '\r')
			fileline[len] = '\0';

		linecount++;

		if(last == NULL) last = tmplast;
		last = addplaylist(firstnode, fileline, linecount, last);
		if(last != NULL) tmplast = last;
	}

	status.writeplaylist = 0;
	free(fileline);
	fclose(fd);
	return 0;
}

void delplaylist(char* file, struct playlist** firstnode)
{
	debug(1000, "in");
	struct playlist *node = *firstnode, *prev = *firstnode;

	while(node != NULL)
	{
		if(ostrcmp(node->file, file) == 0)
		{
			status.writeplaylist = 1;
			if(node == *firstnode)
			{
				*firstnode = node->next;
				if(*firstnode != NULL)
					node->next->prev = NULL;
			}
			else
			{
				prev->next = node->next;
				if(node->next != NULL)
					node->next->prev = prev;
			}

			free(node->file);
			node->file = NULL;

			free(node);
			node = NULL;
			break;
		}

		prev = node;
		node = node->next;
	}
	debug(1000, "out");
}

void freeplaylist(struct playlist** firstnode)
{
	debug(1000, "in");
	struct playlist *node = *firstnode, *prev = *firstnode;

	while(node != NULL)
	{
		prev = node;
		node = node->next;
		if(prev != NULL)
			delplaylist(prev->file, firstnode);
	}
	debug(1000, "out");
}

int writeplaylist(const char *filename, struct playlist *node)
{
	debug(1000, "in");
	FILE *fd = NULL;
	int ret = 0;

	fd = fopen(filename, "w");
	if(fd == NULL)
	{
		perr("can't open %s", filename);
		return 1;
	}

	while(node != NULL)
	{
		ret = fprintf(fd, "%s\n", node->file);
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

void screenplaylist(struct mainplaylist* mplaylist)
{
	int rcret = 0;
	struct skin* playlist = getscreen("playlist_main");
	struct skin* listbox = getscreennode(playlist, "listbox");
	struct skin* tmp = NULL;
	struct playlist* node = NULL;
	struct playlist* movesel = NULL;
	char* ret = NULL;
	char* tmpstr = NULL;
	//int count = 0;

	if(mplaylist == NULL) return;
start:
	node = mplaylist->playlist;
	tmp = NULL;
	delmarkedscreennodes(playlist, 1);

	while(node != NULL)
	{
		tmp = addlistbox(playlist, listbox, tmp, 1);
		if(tmp != NULL)
		{
/*
			count ++;
			char* tmpstr1 = NULL;
			char* title = NULL;
			struct splitstr* ret1 = NULL;
			int count1 = 0;
			tmpstr1 = ostrcat(NULL, node->file, 0, 0);
			int i = 0;
			ret1 = strsplit(tmpstr1, "/", &count1);
			if(count1 >= 1)
				i = count1 - 1;
			title = ostrcat(NULL, (&ret1[i])->part, 0, 0);
			changetext(tmp, title);
*/			

			debug(50, "node->file: %s", node->file);
			debug(50, "basename(node->file): %s", basename(node->file));
						
			changetext(tmp, basename(node->file));			
//			changetext(tmp, node->file);
			tmp->handle = (char*)node;
			tmp->del = 1;
/*
			free(title), title = NULL;
			free(ret1), ret1 = NULL;
			free(tmpstr1), tmpstr1 = NULL;
*/
		}
		node = node->next;
	}

	drawscreen(playlist, 0);
	if(movesel == NULL) addscreenrc(playlist, listbox);

	while(1)
	{
		rcret = waitrc(playlist, 0, 0);

		if(rcret == getrcconfigint("rcexit", NULL)) break;
		if(movesel != NULL && rcret == getrcconfigint("rcup", NULL))
		{
			moveplaylistup(movesel);
			goto start;
		}
		if(movesel != NULL && rcret == getrcconfigint("rcdown", NULL))
		{
			moveplaylistdown(movesel);
			goto start;
		}
		if(rcret == getrcconfigint("rcok", NULL))
		{
			if(listbox->select != NULL)
			{
				if(movesel == NULL)
				{
					status.markmodus = 1;
					delrc(getrcconfigint("rcright", NULL), playlist, listbox);
					delrc(getrcconfigint("rcleft", NULL), playlist, listbox);
					movesel = (struct playlist*)listbox->select->handle;
				}
				else
				{
					status.markmodus = 0;
					addscreenrc(playlist, listbox);
					movesel = NULL;
				}
				drawscreen(playlist, 0);
			}
		}
		if(movesel == NULL && rcret == getrcconfigint("rcgreen", NULL))
		{
			clearscreen(playlist);
			ret = screendir(getconfig("addplaylistpath", NULL), "*.mp3 *.flac *.ogg *.wma *.ra *.avi *.dat *.divx *.flv *.mkv *.m4v *.mp4 *.mov *.mpg *.mpeg *.mts *.m2ts *.pls *.trp *.ts *.vdr *.vob *.wmv *.rm", NULL, NULL, NULL, NULL, 0, "SELECT", 0, NULL, 0, NULL, 0, 1200, 0, 600, 0, 0);

			tmpstr = ostrcat(ret, NULL, 0, 0);
			if(tmpstr != NULL) addconfig("addplaylistpath", dirname(tmpstr));
				free(tmpstr); tmpstr = NULL;

			if(ret != NULL)
				addplaylist(&mplaylist->playlist, ret, 1, NULL);
			free(ret);
			goto start;
		}
		if(movesel == NULL && rcret == getrcconfigint("rcred", NULL))
		{
			if(listbox->select != NULL && listbox->select->handle != NULL)
				delplaylist(((struct playlist*)listbox->select->handle)->file, &mplaylist->playlist);

			goto start;
		}
	}

	status.markmodus = 0;
	delmarkedscreennodes(playlist, 1);
	delownerrc(playlist);
	clearscreen(playlist);
}

void getplaylistmax(struct skin* playlist, int* maxdirs, int* maxfiles)
{
	struct skin* node = playlist;

	while(node != NULL)
	{
		if(node->del == 2)
		{
			if(node->input != NULL)
				(*maxdirs)++;
			else
				(*maxfiles)++;
		}
		node = node->next;
	}
}

struct skin* getplaylistrandom(struct skin* playlist, int maxdirs, int maxfiles)
{
	int count = 0;
	struct skin* node = playlist;
	
	if(maxfiles < 1) return NULL;

	srand(time(NULL));
	int r = rand() % maxfiles;
	r++;

	while(node != NULL)
	{
		if(node->del == 2 && node->input == NULL)
		{
			count++;
			if(count == r) break;
		}

		node = node->next;
	}

	return node;
}

#endif
