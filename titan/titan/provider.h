#ifndef PROVIDER_H
#define PROVIDER_H

int getlastproviderid()
{
	int lastid = 0;
	struct provider *node = provider, *prev = NULL;

	while(node != NULL)
	{
		if(lastid < node->providerid)
			lastid = node->providerid;
		prev = node;
		node = node->next;
	}

	return lastid;
}

struct provider* getlastprovider(struct provider* node)
{
	struct provider *prev = NULL;

	while(node != NULL)
	{
		prev = node;
		node = node->next;
	}

	return prev;
}

int moveproviderblockdown(struct provider* node)
{
	int i = 0, ret = 0;
	struct provider* prev = NULL;

	if(node == NULL)
	{
		err("NULL detect");
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
		err("NULL detect");
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
		err("NULL detect");
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
	struct provider *newnode = NULL, *prev = NULL, *node = provider;
	char *name = NULL;
	int ret = 0;

	newnode = (struct provider*)calloc(1, sizeof(struct provider));
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

	newnode->name = ostrshrink(name);

	if(last == NULL)
	{
		while(node != NULL && strcasecmp(newnode->name, node->name) > 0)
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

	return newnode;
}

int readprovider(const char* filename)
{
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
		if(len >= 0 && fileline[len] == '\n')
			fileline[len] = '\0';
		len--;
		if(len >= 0 && fileline[len] == '\r')
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
	struct provider *node = provider;

	while(node != NULL)
	{
		if(ostrcmp(node->name, name) == 0)
			return node;

		node = node->next;
	}
	debug(100, "provider not found (name=%s)", name);
	return NULL;
}

struct provider* getprovider(int providerid)
{
	struct provider *node = provider;

	while(node != NULL)
	{
		if(node->providerid == providerid)
			return node;

		node = node->next;
	}
	debug(100, "provider not found (providerid=%d)", providerid);
	return NULL;
}

//flag 0: add all provider
//flag 1: add only provider with channel
int provider2bouquet(int providerid, int flag)
{
	int treffer = 0;
	struct provider* pnode = NULL;
	struct mainbouquet* mnode = NULL;
	struct channel* chnode = channel;
	char* tmpstr = NULL;
	char* path = NULL;
	
	pnode = getprovider(providerid);
	if(pnode == NULL) return 1;

	tmpstr = ostrcat(tmpstr, pnode->name, 1, 0);
	tmpstr = ostrcat(tmpstr, "#", 1, 0);
	tmpstr = ostrcat(tmpstr, oitoa(status.servicetype), 1, 1);
	tmpstr = ostrcat(tmpstr, "#", 1, 0);

	path = realpath(getconfig("bouquetfile", NULL), NULL);
	if(path != NULL)
	{
		path = dirname(path);
		path = ostrcat(path, "/bouquets.", 1, 0);
					
		path = ostrcat(path, pnode->name, 1, 0);
		if(status.servicetype == 0) path = ostrcat(path, "_tv", 1, 0);
		if(status.servicetype == 1) path = ostrcat(path, "_radio", 1, 0);

		if(file_exist(path))
		{
			free(tmpstr); tmpstr = NULL;
			free(path); path = NULL;
			return 1;
		}

		tmpstr = ostrcat(tmpstr, path, 1, 0);
		mnode = addmainbouquet(tmpstr, 1, NULL);
	}
	free(tmpstr); tmpstr = NULL;

	if(mnode != NULL)
	{
		while(chnode != NULL)
		{
			if(chnode->providerid == providerid && chnode->servicetype == status.servicetype)
			{
				tmpstr = ostrcat(tmpstr, oitoa(chnode->serviceid), 1, 1);
				tmpstr = ostrcat(tmpstr, "#", 1, 0);
				tmpstr = ostrcat(tmpstr, ollutoa(chnode->transponderid), 1, 1);
				treffer = 1;
				addbouquet(&mnode->bouquet, tmpstr, status.servicetype, 1, NULL);
				free(tmpstr); tmpstr = NULL;
			}
			chnode = chnode->next;
		}
	}
	
	if(treffer == 0 && flag == 1)
		delmainbouquet(path, 1);
 
	free(path); path = NULL;
	return 0;
}

void freeprovider()
{
	struct provider *node = provider, *prev = provider;

	while(node != NULL)
	{
		prev = node;
		node = node->next;
		if(prev != NULL)
			delprovider(prev->providerid);
	}
}

int writeprovider(const char *filename)
{
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
	return 0;
}

#endif

