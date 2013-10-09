#ifndef MAINBOUQUETS_H
#define MAINBOUQUETS_H

struct mainbouquet* screenmainbouquet()
{
	int rcret = 0;
	struct skin* bouquetlist = getscreen("bouquetlist");
	struct skin* listbox = getscreennode(bouquetlist, "listbox");
	struct skin* tmp = NULL;
	struct mainbouquet* node = mainbouquet;
	struct mainbouquet* ret = NULL;

	while(node != NULL)
	{
		if(node->type == status.servicetype)
		{
			tmp = addlistbox(bouquetlist, listbox, tmp, 1);
			if(tmp != NULL)
			{
				changetext(tmp, node->name);
				tmp->handle = (char*)node;
				tmp->del = 1;
			}
		}
		node = node->next;
	}

	drawscreen(bouquetlist, 0, 0);
	addscreenrc(bouquetlist, listbox);

	while(1)
	{
		rcret = waitrc(bouquetlist, 0, 0);

		if(rcret == getrcconfigint("rcexit", NULL)) break;
		if(rcret == getrcconfigint("rcok", NULL))
		{
			if(listbox->select != NULL)
				ret = (struct mainbouquet*)listbox->select->handle;
			break;
		}
	}

	delmarkedscreennodes(bouquetlist, 1);
	delownerrc(bouquetlist);
	clearscreen(bouquetlist);
	return ret;
}

void debugmainbouquet()
{
	struct mainbouquet* node = mainbouquet;

	while(node != NULL)
	{
		printf("mainbouquet = %s\n", node->name);	
		node = node->next;
	}
}

struct mainbouquet* getlastmainbouquet(struct mainbouquet* node)
{
	struct mainbouquet *prev = NULL;

	while(node != NULL)
	{
		prev = node;
		node = node->next;
	}

	return prev;
}

struct mainbouquet* getprevmainbouquetbyservicetype(struct mainbouquet* node)
{
	struct mainbouquet* prev = NULL;

	if(node == NULL) node = mainbouquet;
	if(node == NULL) return NULL;

	prev = node->prev;
	while(prev != NULL && prev->type != status.servicetype)
		prev = prev->prev;
	if(prev == NULL)
	{
		prev = getlastmainbouquet(mainbouquet);
		while(prev != NULL && prev->type != status.servicetype)
			prev = prev->prev;
	}

	return prev;
}

struct mainbouquet* getnextmainbouquetbyservicetype(struct mainbouquet* node)
{
	struct mainbouquet* next = NULL;

	if(node == NULL) node = mainbouquet;
	if(node == NULL) return NULL;

	next = node->next;

	while(next != NULL && next->type != status.servicetype)
		next = next->next;
	if(next == NULL)
	{
		next = mainbouquet;
		while(next != NULL && next->type != status.servicetype)
			next = next->next;
	}

	return next;
}

int movemainbouquetblockdown(struct mainbouquet* node)
{
	int i = 0, ret = 0;
	struct mainbouquet* prev = NULL;

	if(node == NULL || mainbouquet == NULL)
	{
		err("NULL detect");
		return 1;
	}

	for(i = 0; i < status.moveblockcount; i++)
	{
		if(node == NULL) break;
		node = node->next;
		while(node != NULL && node->type != status.servicetype)
			node = node->next;
	}

	for(i = 0; i < status.moveblockcount + 1; i++)
	{
		if(node == NULL) break;
		prev = getprevmainbouquetbyservicetype(node);
		ret = movemainbouquetdown(node);

		while(node->prev != NULL && node->prev->type != status.servicetype)
			ret = movemainbouquetdown(node);

		node = prev;
	}

	return ret;
}

int movemainbouquetdown(struct mainbouquet* node)
{
	struct mainbouquet* prev = NULL, *next = NULL;

	if(node == NULL || mainbouquet == NULL)
	{
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
		node->next = mainbouquet;
		mainbouquet->prev = node;
		mainbouquet = node;
		return 99;
	}

	//haenge node aus 
	if(node->prev != NULL)
		node->prev->next = node->next;
	else
		mainbouquet = node->next;
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

	status.writemainbouquet = 1;
	return 0;
}

int movemainbouquetblockup(struct mainbouquet* node)
{
	int i = 0, ret = 0;
	struct mainbouquet* next = NULL;

	if(node == NULL || mainbouquet == NULL)
	{
		err("NULL detect");
		return 1;
	}

	for(i = 0; i < status.moveblockcount + 1; i++)
	{
		if(node == NULL) break;
		next = getnextmainbouquetbyservicetype(node);
		ret = movemainbouquetup(node);

		while(node->next != NULL && node->next->type != status.servicetype)
			ret = movemainbouquetup(node);

		node = next;
	}

	return ret;
}

int movemainbouquetup(struct mainbouquet* node)
{
	struct mainbouquet* prev = NULL, *next = NULL, *last = NULL;

	if(node == NULL || mainbouquet == NULL)
	{
		err("NULL detect");
		return 1;
	}
	
	//only one node
	if(node->prev == NULL && node->next == NULL)
		return 0;

	//first node
	if(node->prev == NULL)
	{
		last = getlastmainbouquet(mainbouquet);

		if(node->next != NULL)
			node->next->prev = NULL;
		mainbouquet = node->next;
		node->next = NULL;
		last->next = node;
		node->prev = last;
		return 99;
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
		mainbouquet = node;
	prev->prev = node;

	status.writemainbouquet = 1;
	return 0;
}

struct mainbouquet* addmainbouquet(char *line, int count, struct mainbouquet* last)
{
	struct mainbouquet *newnode = NULL, *prev = NULL, *node = mainbouquet;
	char *name = NULL, *filename = NULL;
	int ret = 0;

	newnode = (struct mainbouquet*)calloc(1, sizeof(struct mainbouquet));
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

	status.writemainbouquet = 1;

	ret = sscanf(line, "%[^#]#%d#%[^#]", name, &newnode->type, filename);
	if(ret != 3)
	{
		if(count > 0)
		{
			err("mainbouquet line %d not ok", count);
		}
		else
		{
			err("add mainbouquet");
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
		mainbouquet = newnode;
	else
	{
		prev->next = newnode;
		newnode->prev = prev;
	}
	newnode->next = node;
	if(node != NULL) node->prev = newnode;

	return newnode;
}

int readmainbouquet(char* filename)
{
	FILE *fd = NULL;
	char *fileline = NULL, *tmpstr = NULL, *tmpstr0 = NULL, *tmpstr1 = NULL;
	int linecount = 0, treffer0 = 1, treffer1 = 1, len = 0;
	struct mainbouquet* last = NULL, *tmplast = NULL;
	
	tmpstr0 = getconfig("channellist", NULL);
	tmpstr1 = getconfig("rchannellist", NULL);
	if(ostrncmp("(BOUQUET)-", tmpstr0, 10) == 0) treffer0 = 0;
	if(ostrncmp("(BOUQUET)-", tmpstr1, 10) == 0) treffer1 = 0;

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

		linecount++;

		if(last == NULL) last = tmplast;
		last = addmainbouquet(fileline, linecount, last);
		if(last != NULL)
		{
			tmplast = last;
			tmpstr = ostrcat("(BOUQUET)-", last->name, 0, 0);

			if(treffer0 == 0 && last->type == 0 && ostrcmp(tmpstr, tmpstr0) == 0)
				treffer0 = 1;
			if(treffer1 == 0 && last->type == 1 && ostrcmp(tmpstr, tmpstr1) == 0)
				treffer1 = 1;
			free(tmpstr); tmpstr = NULL;
		}
	}

	if(treffer0 == 0) delconfig("channellist");
	if(treffer1 == 0) delconfig("rchannellist");

	status.writemainbouquet = 0;
	free(fileline);
	fclose(fd);
	return 0;
}

int readallbouquet()
{
	int ret = 0;
	struct mainbouquet* node = mainbouquet;

	while(node != NULL)
	{
		if(ostrcmp(node->filename, "0") != 0)
		{
			if(readbouquet(node->filename, &node->bouquet, node->type) != 0)
				ret = 1;
		}
		node = node->next;
	}
	
	return ret;
}

struct mainbouquet* getmainbouquetbybouquetpointer(struct bouquet* bouquetnode)
{
	struct mainbouquet *node = mainbouquet;
	struct bouquet *bnode = NULL;

	while(node != NULL)
	{
		bnode = node->bouquet;
		while(bnode != NULL)
		{
			if(bnode == bouquetnode)
				return node;

			bnode = bnode->next;
		}
		node = node->next;
	}
	debug(100, "mainbouquet not found (%p)", bouquetnode);
	return NULL;
}

struct mainbouquet* getmainbouquetbyfilename(char *filename)
{
	struct mainbouquet *node = mainbouquet;

	while(node != NULL)
	{
		if(ostrcmp(node->filename, filename) == 0)
			return node;

		node = node->next;
	}
	debug(100, "mainbouquet not found (%s)", filename);
	return NULL;
}

struct mainbouquet* getmainbouquet(char *name)
{
	struct mainbouquet *node = mainbouquet;

	while(node != NULL)
	{
		if(ostrcmp(node->name, name) == 0)
			return node;

		node = node->next;
	}
	debug(100, "mainbouquet not found (%s)", name);
	return NULL;
}

//flag 0: don't unlink bouquet
//flag 1: unlink bouquet
void delmainbouquet(char *name, int flag)
{
	struct mainbouquet *node = mainbouquet, *prev = mainbouquet;

	while(node != NULL)
	{
		if(ostrcmp(node->name, name) == 0)
		{
			status.writemainbouquet = 1;
			if(node == mainbouquet)
			{
				mainbouquet = node->next;
				if(mainbouquet != NULL)
					mainbouquet->prev = NULL;
			}
			else
			{
				prev->next = node->next;
				if(node->next != NULL)
					node->next->prev = prev;
			}

			free(node->name);
			node->name = NULL;

			freebouquet(&node->bouquet);
			if(flag == 1) unlink(node->filename);

			free(node->filename);
			node->filename = NULL;

			free(node);
			node = NULL;
			break;
		}

		prev = node;
		node = node->next;
	}
}

//flag 0: don't unlink bouquet
//flag 1: unlink bouquet
void freemainbouquet(int flag)
{
	struct mainbouquet *node = mainbouquet, *prev = mainbouquet;

	while(node != NULL)
	{
		prev = node;
		node = node->next;
		if(prev != NULL)
			delmainbouquet(prev->name, flag);
	}
}

void freeallbouquet()
{
	struct mainbouquet* node = mainbouquet;

	while(node != NULL)
	{
		freebouquet(&node->bouquet);
		node = node->next;
	}
}

int writemainbouquet(const char *filename)
{
	FILE *fd = NULL;
	struct mainbouquet *node = mainbouquet;
	int ret = 0;

	fd = fopen(filename, "w");
	if(fd == NULL)
	{
		perr("can't open %s", filename);
		return 1;
	}

	while(node != NULL)
	{
		ret = fprintf(fd, "%s#%d#%s\n", node->name, node->type, node->filename);
		if(ret < 0)
		{
			perr("writting file %s", filename);
		}
		node = node->next;
	}

	fclose(fd);
	return 0;
}

int mainbouquet2channelslot(struct mainbouquet* mnode, int slot)
{
	struct bouquet* bnode = NULL;
	char* tmpstr = NULL;

	if(mnode != NULL)
	{
		bnode = mnode->bouquet;
		while(bnode != NULL)
		{
			if(getchannelslot(bnode->serviceid, bnode->transponderid) == NULL)
			{
				tmpstr = ostrcat(tmpstr, ollutoa(bnode->transponderid), 1, 1);
				tmpstr = ostrcat(tmpstr, "#", 1, 0);
				tmpstr = ostrcat(tmpstr, oitoa(bnode->serviceid), 1, 1);
				tmpstr = ostrcat(tmpstr, "#", 1, 0);
				tmpstr = ostrcat(tmpstr, oitoa(slot), 1, 1);
				
				addchannelslot(tmpstr, 1, NULL);
				free(tmpstr); tmpstr = NULL;
			}
			bnode = bnode->next;
		}
	}

	return 0;
}

int mainbouquet2epgscanlist(struct mainbouquet* mnode)
{
	struct bouquet* bnode = NULL;
	char* tmpstr = NULL;

	if(mnode != NULL)
	{
		bnode = mnode->bouquet;
		while(bnode != NULL)
		{
			if(getepgscanlist(bnode->serviceid, bnode->transponderid) == NULL)
			{
				tmpstr = ostrcat(tmpstr, oitoa(bnode->serviceid), 1, 1);
				tmpstr = ostrcat(tmpstr, "#", 1, 0);
				tmpstr = ostrcat(tmpstr, ollutoa(bnode->transponderid), 1, 1);
				
				addepgscanlist(tmpstr, 1, NULL);
				free(tmpstr); tmpstr = NULL;
			}
			bnode = bnode->next;
		}
	}

	return 0;
}

int writeallbouquet()
{
	struct mainbouquet* node = mainbouquet;
	int ret = 0;

	while(node != NULL)
	{
		if(ostrcmp(node->filename, "0") != 0)
		{
			if(writebouquet(node->filename, node->bouquet) != 0)
				ret = 1;
		}
		node = node->next;
	}

	return ret;
}

#endif
