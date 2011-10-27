#ifndef BOUQUETS_H
#define BOUQUETS

void debugbouquet()
{
	struct mainbouquet* mainbouquetnode = mainbouquet;
	struct bouquet* bouquetnode = NULL;

	while(mainbouquetnode != NULL)
	{
		printf("%s\n", mainbouquetnode->name);
		bouquetnode = mainbouquetnode->bouquet;
		while(bouquetnode != NULL)
		{
			printf("nr=%d sid=%d %p <%p >%p\n", bouquetnode->nr, bouquetnode->serviceid, bouquetnode, bouquetnode->prev, bouquetnode->next);
			bouquetnode = bouquetnode->next;
		}
		mainbouquetnode = mainbouquetnode->next;
	}
}

struct bouquet* getlastbouquet(struct bouquet* node)
{
	debug(1000, "in");
	struct bouquet *prev = NULL;

	while(node != NULL)
	{
		prev = node;
		node = node->next;
	}

	debug(1000, "out");
	return prev;
}

int movebouquetdown(struct bouquet* node)
{
	struct bouquet* prev = NULL, *next = NULL;
	struct mainbouquet* mainbouquetnode = NULL;

	if(node == NULL)
	{
		debug(1000, "NULL detect");
		return 1;
	}

	mainbouquetnode = getmainbouquetbybouquetpointer(node);
	if(mainbouquetnode == NULL)
	{
		debug(1000, "NULL detect");
		err("NULL detect");
		return 1;
	}

	//last node
	if(node->next == NULL)
	{
		if(node->prev != NULL)
			node->prev->next = NULL;
		node->prev = NULL;
		node->next = mainbouquetnode->bouquet;
		mainbouquetnode->bouquet->prev = node;
		mainbouquetnode->bouquet = node;
		return 0;
	}

	//haenge node aus 
	if(node->prev != NULL)
		node->prev->next = node->next;
	else
		mainbouquetnode->bouquet = node->next;
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

	status.writebouquet = 1;
	return 0;
}

int movebouquetup(struct bouquet* node)
{
	struct bouquet* prev = NULL, *next = NULL, *last = NULL;
	struct mainbouquet* mainbouquetnode = NULL;

	if(node == NULL)
	{
		debug(1000, "NULL detect");
		return 1;
	}

	mainbouquetnode = getmainbouquetbybouquetpointer(node);
	if(mainbouquetnode == NULL)
	{
		debug(1000, "NULL detect");
		return 1;
	}

	//first node
	if(node->prev == NULL)
	{
		last = getlastbouquet(mainbouquetnode->bouquet);

		if(node->next != NULL)
			node->next->prev = NULL;
		mainbouquetnode->bouquet = node->next;
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
		mainbouquetnode->bouquet = node;
	prev->prev = node;

	status.writebouquet = 1;
	return 0;
}

struct bouquet* getbouquetbychannel(struct bouquet* node, int serviceid, int transponderid)
{
	while(node != NULL)
	{
		if(node->channel != NULL && node->channel->serviceid == serviceid && node->channel->transponderid == transponderid)
			return node;
		node = node->next;
	}
	return NULL;
}

struct bouquet* getbouquetbynr(int nr)
{
	struct mainbouquet* mainbouquetnode = mainbouquet;
	struct bouquet* bouquetnode = NULL;

	while(mainbouquetnode != NULL)
	{
		bouquetnode = mainbouquetnode->bouquet;
		while(bouquetnode != NULL)
		{
			if(bouquetnode->channel != NULL && bouquetnode->channel->servicetype == status.servicetype && bouquetnode->nr == nr)
				return bouquetnode;

			bouquetnode = bouquetnode->next;
		}
		mainbouquetnode = mainbouquetnode->next;
	}

	return NULL;
}

struct bouquet* addbouquet(struct bouquet **firstnode, char *line, int type, int count, struct bouquet* last)
{
	//debug(1000, "in");
	struct bouquet *newnode = NULL, *prev = NULL, *node = *firstnode;
	int ret = 0;

	newnode = (struct bouquet*)malloc(sizeof(struct bouquet));	
	if(newnode == NULL)
	{
		err("no memory");
		return NULL;
	}

	memset(newnode, 0, sizeof(struct bouquet));
	status.writebouquet = 1;

	ret = sscanf(line, "%d#%ld", &newnode->serviceid, &newnode->transponderid);
	if(ret != 2)
	{
		if(count > 0)
		{
			err("bouquet line %d not ok", count);
		}
		else
		{
			err("add bouquet");
		}
		free(newnode);
		return NULL;
	}

	newnode->channel = getchannel(newnode->serviceid, newnode->transponderid);
	if(newnode->channel == NULL)
	{
		err("bouquet line %d not ok (channel not found)", count);
		free(newnode);
		return NULL;
	}

	if(type < MAXSERVICETYPE)
	{
		status.bouquetnr[type]++;
		newnode->nr = status.bouquetnr[type];
	}

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

void recalcbouquetnr()
{
	int i;
	struct mainbouquet* node = mainbouquet;
	struct bouquet* bouquetnode = NULL;

	for(i = 0; i < MAXSERVICETYPE; i++)
		status.bouquetnr[i] = 0;

	while(node != NULL)
	{
		bouquetnode = node->bouquet;
		while(bouquetnode != NULL)
		{
			if(node->type < MAXSERVICETYPE)
			{
				status.bouquetnr[node->type]++;
				bouquetnode->nr = status.bouquetnr[node->type];
			}
			bouquetnode = bouquetnode->next;
		}
		node = node->next;
	}
}

int readbouquet(char* filename, struct bouquet** firstnode, int type)
{
	debug(1000, "in");
	FILE *fd = NULL;
	char *fileline = NULL;
	int linecount = 0;
	struct bouquet* last = NULL, *tmplast = NULL;

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
		last = addbouquet(firstnode, fileline, type, linecount, last);
		if(last != NULL) tmplast = last;
	}

	status.writebouquet = 0;
	free(fileline);
	fclose(fd);
	return 0;
}

void delbouquet(int serviceid, int transponderid, struct bouquet** firstnode)
{
	debug(1000, "in");
	struct bouquet *node = *firstnode, *prev = *firstnode;

	while(node != NULL)
	{
		if(node->serviceid == serviceid && node->transponderid == transponderid)
		{
			status.writebouquet = 1;
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

			free(node);
			node = NULL;
			break;
		}

		prev = node;
		node = node->next;
	}
	debug(1000, "out");
}

struct bouquet* getbouquetbychannelmain(int serviceid, int transponderid)
{
	struct mainbouquet* node = mainbouquet;
	struct bouquet* bouquetnode = NULL;

	while(node != NULL)
	{
		if(node->bouquet != NULL)
		{
			bouquetnode = getbouquetbychannel(node->bouquet, serviceid, transponderid);
			if(bouquetnode != NULL) return bouquetnode;
		}
		node = node->next;
	}
	return NULL;
}

void delbouquetbychannel(int serviceid, int transponderid)
{
	struct mainbouquet* node = mainbouquet;

	while(node != NULL)
	{
		if(node->bouquet != NULL)
			delbouquet(serviceid, transponderid, &node->bouquet);
		node = node->next;
	}
}

void delunusedbouquetchannels()
{
	struct mainbouquet* mainbouquetnode = mainbouquet;
	struct bouquet *node = NULL, *prev = NULL;
	struct channel* chnode = NULL;

	while(mainbouquetnode != NULL)
	{
		prev = mainbouquetnode->bouquet;
		node = mainbouquetnode->bouquet;
		while(node != NULL)
		{
			prev = node;
			node = node->next;
			chnode = getchannel(prev->serviceid, prev->transponderid);
			if(chnode == NULL)
				delbouquet(prev->serviceid, prev->transponderid, &mainbouquetnode->bouquet);
			else
				prev->channel = chnode;
		}
		mainbouquetnode = mainbouquetnode->next;
	}
}

void freebouquet(struct bouquet** firstnode)
{
	debug(1000, "in");
	struct bouquet *node = *firstnode, *prev = *firstnode;

	while(node != NULL)
	{
		prev = node;
		node = node->next;
		if(prev != NULL)
			delbouquet(prev->serviceid, prev->transponderid, firstnode);
	}
	debug(1000, "out");
}

int writebouquet(const char *filename, struct bouquet *node)
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
		ret = fprintf(fd, "%d#%lu\n", node->serviceid, node->transponderid);
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
