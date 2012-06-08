#ifndef PROVIDER_H
#define PROVIDER_H

int getlastproviderid()
{
	debug(1000, "in");
	int lastid = 0;
	struct provider *node = provider, *prev = NULL;

	while(node != NULL)
	{
		if(lastid < node->providerid)
			lastid = node->providerid;
		prev = node;
		node = node->next;
	}

	debug(1000, "out");
	return lastid;
}

struct provider* getlastprovider(struct provider* node)
{
	debug(1000, "in");
	struct provider *prev = NULL;

	while(node != NULL)
	{
		prev = node;
		node = node->next;
	}

	debug(1000, "out");
	return prev;
}

int moveproviderblockdown(struct provider* node)
{
	int i = 0, ret = 0;
	struct provider* prev = NULL;

	if(node == NULL)
	{
		debug(1000, "NULL detect");
		return 1;
	}

	for(i = 0; i < status.moveblockcount; i++)
	{
		if(node == NULL) break;
		node = node->next;
	}

	for(i = 0; i < status.moveblockcount + 1; i++)
	{
		if(node == NULL) break;
		prev = node->prev;
		ret = moveproviderdown(node);
		node = prev;
	}

	return ret;
}

int moveproviderdown(struct provider* node)
{
	struct provider* prev = NULL, *next = NULL;

	if(node == NULL || provider == NULL)
	{
		debug(1000, "NULL detect");
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
		node->next = provider;
		provider->prev = node;
		provider = node;
		return 99;
	}

	//haenge node aus 
	if(node->prev != NULL)
		node->prev->next = node->next;
	else
		provider = node->next;
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

	status.writeprovider = 1;
	return 0;
}

int moveproviderblockup(struct provider* node)
{
	int i = 0, ret = 0;
	struct provider* next = NULL;

	if(node == NULL)
	{
		debug(1000, "NULL detect");
		return 1;
	}

	for(i = 0; i < status.moveblockcount + 1; i++)
	{
		if(node == NULL) break;
		next = node->next;
		ret = moveproviderup(node);
		node = next;
	}

	return ret;
}

int moveproviderup(struct provider* node)
{
	struct provider* prev = NULL, *next = NULL, *last = NULL;

	if(node == NULL || provider == NULL)
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
		last = getlastprovider(provider);

		if(node->next != NULL)
			node->next->prev = NULL;
		provider = node->next;
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
		provider = node;
	prev->prev = node;

	status.writeprovider = 1;
	return 0;
}

struct provider* addprovider(char *line, int count, struct provider* last)
{
	//debug(1000, "in");
	struct provider *newnode = NULL, *prev = NULL, *node = provider;
	char *name = NULL;
	int ret = 0;

	newnode = (struct provider*)malloc(sizeof(struct provider));	
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

	memset(newnode, 0, sizeof(struct provider));
	status.writeprovider = 1;

	ret = sscanf(line, "%d#%[^#]#%d", &newnode->providerid, name, &newnode->flag);
	if(ret != 3)
	{
		if(count > 0)
		{
			err("providerlist line %d not ok", count);
		}
		else
		{
			err("add provider");
		}
		free(name);
		free(newnode);
		return NULL;
	}

	newnode->name = ostrcat(name, NULL, 1, 0);

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
		provider = newnode;
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

int readprovider(const char* filename)
{
	debug(1000, "in");
	FILE *fd = NULL;
	char *fileline = NULL, *tmpstr = NULL, *tmpstr0 = NULL, *tmpstr1 = NULL;
	int linecount = 0, treffer0 = 1, treffer1 = 1, len = 0;
	struct provider* last = NULL, *tmplast = NULL;
	
	tmpstr0 = getconfig("channellist", NULL);
	tmpstr1 = getconfig("rchannellist", NULL);
	if(ostrncmp("(PROVIDER)-", tmpstr0, 11) == 0) treffer0 = 0;
	if(ostrncmp("(PROVIDER)-", tmpstr1, 11) == 0) treffer1 = 0;

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
		last = addprovider(fileline, linecount, last);
		if(last != NULL) 
		{
			tmplast = last;
			tmpstr = ostrcat("(PROVIDER)-", last->name, 0, 0);

			if(treffer0 == 0 && ostrcmp(tmpstr, tmpstr0) == 0)
				treffer0 = 1;
			if(treffer1 == 0 && ostrcmp(tmpstr, tmpstr1) == 0)
				treffer1 = 1;
			free(tmpstr); tmpstr = NULL;
		}
	}
	
	if(treffer0 == 0) delconfig("channellist");
	if(treffer1 == 0) delconfig("rchannellist");

	status.writeprovider = 0;
	free(fileline);
	fclose(fd);
	return 0;
}

void delprovider(int providerid)
{
	debug(1000, "in");
	struct provider *node = provider, *prev = provider;

	while(node != NULL)
	{
		if(node->providerid == providerid)
		{
			status.writeprovider = 1;
			if(node == provider)
			{
				provider = node->next;
				if(provider != NULL)
					provider->prev = NULL;
			}
			else
			{
				prev->next = node->next;
				if(node->next != NULL)
					node->next->prev = prev;
			}

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

void delprovidernotused(struct provider* node)
{
	struct channel* chnode = channel;

	if(node == NULL) return;

	while(chnode != NULL)
	{
		if(chnode->providerid == node->providerid)
			return;
		chnode = chnode->next;
	}
	delprovider(node->providerid);
}

struct provider* getproviderbyname(char* name)
{
	debug(1000, "in");
	struct provider *node = provider;

	while(node != NULL)
	{
		if(ostrcmp(node->name, name) == 0)
		{
			debug(1000, "out");
			return node;
		}

		node = node->next;
	}
	debug(100, "provider not found (name=%s)", name);
	return NULL;
}

struct provider* getprovider(int providerid)
{
	//debug(1000, "in");
	struct provider *node = provider;

	while(node != NULL)
	{
		if(node->providerid == providerid)
		{
			//debug(1000, "out");
			return node;
		}

		node = node->next;
	}
	debug(100, "provider not found (providerid=%d)", providerid);
	return NULL;
}

void freeprovider()
{
	debug(1000, "in");
	struct provider *node = provider, *prev = provider;

	while(node != NULL)
	{
		prev = node;
		node = node->next;
		if(prev != NULL)
			delprovider(prev->providerid);
	}
	debug(1000, "out");
}

int writeprovider(const char *filename)
{
	debug(1000, "in");
	FILE *fd = NULL;
	struct provider *node = provider;
	int ret;

	fd = fopen(filename, "w");
	if(fd == NULL)
	{
		perr("can't open %s", filename);
		return 1;
	}

	while(node != NULL)
	{
		ret = fprintf(fd, "%d#%s#%d\n", node->providerid, node->name, node->flag);
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

#endif

