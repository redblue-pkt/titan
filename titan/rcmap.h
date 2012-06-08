#ifndef RCMAP_H
#define RCMAP_H

struct rcmap* addrcmap(char *line, int count, struct rcmap* last)
{
	debug(1000, "in");
	struct rcmap *newnode = NULL, *prev = NULL, *node = rcmap;
	char *name = NULL, *key = NULL, *newkey = NULL;
	int ret = 0;

	newnode = (struct rcmap*)malloc(sizeof(struct rcmap));	
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
	key = malloc(MINMALLOC);
	if(key == NULL)
	{
		err("no memory");
		free(newnode);
		free(name);
		return NULL;
	}
	newkey = malloc(MINMALLOC);
	if(newkey == NULL)
	{
		err("no memory");
		free(newnode);
		free(name);
		free(key);
		return NULL;
	}

	memset(newnode, 0, sizeof(struct rcmap));

	ret = sscanf(line, "%[^#]#%[^#]#%[^#]", name, key, newkey);
	if(ret != 3)
	{
		if(count > 0)
		{
			err("rcmap line %d not ok", count);
		}
		else
		{
			err("add rcmap");
		}
		free(name);
		free(key);
		free(newkey);
		free(newnode);
		return NULL;
	}

	newnode->name = ostrcat(name, NULL, 1, 0);
	newnode->key = getrcconfigint(key, NULL);
	free(key); key = NULL;
	newnode->newkey = getrcconfigint(newkey, NULL);
	free(newkey); newkey = NULL;

	if(last == NULL)
	{
		while(node != NULL && strcoll(newnode->name, node->name) > 0)
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
		rcmap = newnode;
	else
		prev->next = newnode;
	newnode->next = node;

	debug(1000, "out");
	return newnode;
}

int readrcmap(const char* filename)
{
	debug(1000, "in");
	FILE *fd = NULL;
	char *fileline = NULL;
	int linecount = 0, len = 0;
	struct rcmap* last = NULL, *tmplast = NULL;

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
		last = addrcmap(fileline, linecount, last);
		if(last != NULL) tmplast = last;
	}

	free(fileline);
	fclose(fd);
	return 0;
}

void delrcmap(char* name)
{
	debug(1000, "in");
	struct rcmap *node = rcmap, *prev = rcmap;

	while(node != NULL)
	{
		if(ostrcmp(node->name, name) == 0)
		{
			if(node == rcmap)
				rcmap = node->next;
			else
				prev->next = node->next;

			free(node->name);
			node->name = NULL;

			free(node);
			node = NULL;
			break;
		}

		prev = node;
		node = node->next;
	}
	debug(1000, "out");
}

struct rcmap* getrcmap(char* name, int key)
{
	debug(1000, "in");
	struct rcmap *node = rcmap;

	while(node != NULL)
	{
		if(ostrcmp(node->name, name) == 0 && node->key == key)
		{
			debug(1000, "out");
			return node;
		}

		node = node->next;
	}
	debug(100, "rcmap not found (name=%s, key=%d)", name, key);
	return NULL;
}

void freercmap()
{
	debug(1000, "in");
	struct rcmap *node = rcmap, *prev = rcmap;

	while(node != NULL)
	{
		prev = node;
		node = node->next;
		if(prev != NULL)
			delrcmap(prev->name);
	}
	debug(1000, "out");
}

#endif
