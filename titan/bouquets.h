#ifndef BOUQUETS_H
#define BOUQUETS_H

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
	struct bouquet *prev = NULL;

	while(node != NULL)
	{
		prev = node;
		node = node->next;
	}

	return prev;
}

struct bouquet* getprevbouquetbyservicetype(struct bouquet* first, struct bouquet* node)
{
	struct bouquet* prev = NULL;

	if(node == NULL) node = first;
	if(node == NULL) return NULL;

	prev = node->prev;
	while(prev != NULL && prev->channel != NULL && prev->channel->servicetype != status.servicetype)
		prev = prev->prev;
	if(prev == NULL)
	{
		prev = getlastbouquet(first);
		while(prev != NULL && prev->channel != NULL && prev->channel->servicetype != status.servicetype)
			prev = prev->prev;
	}

	return prev;
}

struct bouquet* getnextbouquetbyservicetype(struct bouquet* first, struct bouquet* node)
{
	struct bouquet* next = NULL;

	if(node == NULL) node = first;
	if(node == NULL) return NULL;

	next = node->next;

	while(next != NULL && next->channel != NULL && next->channel->servicetype != status.servicetype)
		next = next->next;
	if(next == NULL)
	{
		next = first;
		while(next != NULL && next->channel != NULL && next->channel->servicetype != status.servicetype)
			next = next->next;
	}

	return next;
}

int movebouquetblockdown(struct bouquet* node)
{
	int i = 0, ret = 0;
	struct bouquet* prev = NULL;
	struct mainbouquet* mainbouquetnode = NULL;

	mainbouquetnode = getmainbouquetbybouquetpointer(node);
	if(mainbouquetnode == NULL)
	{
		err("NULL detect");
		return 1;
	}

	if(node == NULL) return 1;

	for(i = 0; i < status.moveblockcount; i++)
	{
		if(node == NULL) break;
		node = node->next;
		while(node->prev != NULL && node->prev->channel != NULL && node->prev->channel->servicetype != status.servicetype)
			node = node->next;
	}

	for(i = 0; i < status.moveblockcount + 1; i++)
	{
		if(node == NULL) break;
		prev = getprevbouquetbyservicetype(mainbouquetnode->bouquet, node);
		ret = movebouquetdown(node);

	while(node->prev != NULL && node->prev->channel != NULL && node->prev->channel->servicetype != status.servicetype)
			ret = movebouquetdown(node);

		node = prev;
	}

	return ret;
}

int movebouquetdown(struct bouquet* node)
{
	struct bouquet* prev = NULL, *next = NULL;
	struct mainbouquet* mainbouquetnode = NULL;

	if(node == NULL) return 1;

	mainbouquetnode = getmainbouquetbybouquetpointer(node);
	if(mainbouquetnode == NULL)
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
		node->next = mainbouquetnode->bouquet;
		mainbouquetnode->bouquet->prev = node;
		mainbouquetnode->bouquet = node;
		return 99;
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

int movebouquetblockup(struct bouquet* node)
{
	int i = 0, ret = 0;
	struct bouquet* next = NULL;
	struct mainbouquet* mainbouquetnode = NULL;

	mainbouquetnode = getmainbouquetbybouquetpointer(node);
	if(mainbouquetnode == NULL)
	{
		err("NULL detect");
		return 1;
	}

	if(node == NULL) return 1;

	for(i = 0; i < status.moveblockcount + 1; i++)
	{
		if(node == NULL) break;
		next = getnextbouquetbyservicetype(mainbouquetnode->bouquet, node);
		ret = movebouquetup(node);

		while(node->next != NULL && node->next->channel != NULL && node->next->channel->servicetype != status.servicetype)
			ret = movebouquetup(node);

		node = next;
	}

	return ret;
}

int movebouquetup(struct bouquet* node)
{
	struct bouquet* prev = NULL, *next = NULL, *last = NULL;
	struct mainbouquet* mainbouquetnode = NULL;

	if(node == NULL) return 1;

	mainbouquetnode = getmainbouquetbybouquetpointer(node);
	if(mainbouquetnode == NULL)
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
		last = getlastbouquet(mainbouquetnode->bouquet);

		if(node->next != NULL)
			node->next->prev = NULL;
		mainbouquetnode->bouquet = node->next;
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
		mainbouquetnode->bouquet = node;
	prev->prev = node;

	status.writebouquet = 1;
	return 0;
}

struct bouquet* getbouquetbychannel(struct bouquet* node, int serviceid, uint64_t transponderid)
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
	struct bouquet *newnode = NULL, *prev = NULL, *node = *firstnode;
	int ret = 0;

	char *streamurl = NULL, *epgurl = NULL;

	streamurl = malloc(MINMALLOC);
	if(streamurl == NULL)
	{
		err("no memory");
		free(newnode);
		return NULL;
	}

	epgurl = malloc(MINMALLOC);
	if(epgurl == NULL)
	{
		err("no memory");
		free(newnode);
		return NULL;
	}

	newnode = (struct bouquet*)calloc(1, sizeof(struct bouquet));
	if(newnode == NULL)
	{
		err("no memory");
		return NULL;
	}

	status.writebouquet = 1;
	ret = sscanf(line, "%d#%llu#%[^#]#%s", &newnode->serviceid, &newnode->transponderid, streamurl, epgurl);

	if(ret == 3)
	{
        if(streamurl != NULL && ostrcmp("(null)", streamurl) == 0)
        {
        	newnode->streamurl = ostrcat(streamurl, NULL, 0, 0);
    		debug(10, "set newnode->streamurl: %s", streamurl);
        }
        else
            newnode->streamurl = NULL;
	}

	if(ret == 4)
	{
        if(streamurl != NULL && ostrcmp("(null)", streamurl) != 0)
        {
        	newnode->streamurl = ostrcat(streamurl, NULL, 0, 0);
    		debug(10, "set newnode->streamurl: %s", streamurl);
        }
        else
            newnode->streamurl = NULL;

        if(epgurl != NULL && ostrcmp("(null)", epgurl) != 0)
        {
        	newnode->epgurl = ostrcat(streamurl, NULL, 0, 0);
    		debug(10, "set newnode->epgurl: %s", epgurl);
        }
        else
            newnode->epgurl = NULL;
	}

	if(ret == 2)
	{
		ret++;
	}

	if(ret == 3)
	{
		ret++;
	}

	if(ret != 4)
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

		free(streamurl);
		free(epgurl);

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
	FILE *fd = NULL;
	char *fileline = NULL;
	int linecount = 0, len = 0;
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
		len = strlen(fileline) - 1;
		if(len >= 0 && fileline[len] == '\n')
			fileline[len] = '\0';
		len--;
		if(len >= 0 && fileline[len] == '\r')
			fileline[len] = '\0';

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

void delbouquet(int serviceid, uint64_t transponderid, struct bouquet** firstnode)
{
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
}

void setbouquetchanneltonullmain(int serviceid, uint64_t transponderid)
{
	struct mainbouquet* node = mainbouquet;
	struct bouquet* bouquetnode = NULL;

	while(node != NULL)
	{
		bouquetnode = node->bouquet;
		while(bouquetnode != NULL)
		{
			if(bouquetnode->channel != NULL && bouquetnode->channel->serviceid == serviceid && bouquetnode->channel->transponderid == transponderid)
				bouquetnode->channel = NULL;
			bouquetnode = bouquetnode->next;
		}
		node = node->next;
	}
}

void delbouquetbychannel(int serviceid, uint64_t transponderid)
{
	struct mainbouquet* node = mainbouquet;

	while(node != NULL)
	{
		if(node->bouquet != NULL)
			delbouquet(serviceid, transponderid, &node->bouquet);
		node = node->next;
	}
}

//flag: 0 = del bouquet
//flag: 1 = do not del bouquet
void delunusedbouquetchannels(int flag)
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
			if(chnode == NULL && flag == 0)
				delbouquet(prev->serviceid, prev->transponderid, &mainbouquetnode->bouquet);
			else
				prev->channel = chnode;
		}
		mainbouquetnode = mainbouquetnode->next;
	}
}

void freebouquet(struct bouquet** firstnode)
{
	struct bouquet *node = *firstnode, *prev = *firstnode;

	while(node != NULL)
	{
		prev = node;
		node = node->next;
		if(prev != NULL)
			delbouquet(prev->serviceid, prev->transponderid, firstnode);
	}
}

struct bouquet* sortbouquet(struct bouquet **nodeaddr)
{
	struct bouquet *nodea = NULL, *nodeb = NULL, *nodec = NULL, *noded = NULL;
	struct bouquet *nodetmp = NULL;

	if(nodeaddr == NULL || *nodeaddr == NULL)
	{
		err("NULL detect");
		return NULL;
	}
	
	struct bouquet *node = *nodeaddr;

	if(node != NULL)
	{
		while(noded != node->next)
		{
			nodec = node;
			nodea = node;
			nodeb = nodea->next;

			while(nodea != noded)
			{
				struct channel *chnode_a = getchannel(nodea->serviceid, nodea->transponderid);
				struct channel *chnode_b = getchannel(nodeb->serviceid, nodeb->transponderid);
				if(chnode_a != NULL && chnode_b != NULL && strcasecmp(chnode_a->name, chnode_b->name) > 0)
				{
					if(nodea == node)
					{
						nodetmp = nodeb->next;
						nodeb->next = nodea;
						nodea->next = nodetmp;
						node = nodeb;
						*nodeaddr = nodeb;
						nodec = nodeb;
					}
					else
					{
						nodetmp = nodeb->next;
						nodeb->next = nodea;
						nodea->next = nodetmp;
						nodec->next = nodeb;
						nodec = nodeb;
					}
				}
				else
				{
					nodec = nodea;
					nodea = nodea->next;
				}
				nodeb = nodea->next;
				if (nodeb == noded)
					noded = nodea;
			}
		}
	}

	//calc prev
	struct bouquet* prev = NULL;
	nodetmp = node;
	while(nodetmp != NULL)
	{
		nodetmp->prev = prev;
		prev = nodetmp;

		nodetmp = nodetmp->next;
	}

	status.writebouquet = 1;
	return *nodeaddr;
}

/*
struct bouquet* sortbouquet(struct bouquet **nodeaddr)
{
	struct bouquet *tmpnode[10] = {0};
	struct bouquet *tnode = NULL;
	struct bouquet *next = NULL, *prev = NULL;
	struct bouquet **tnodeaddr = NULL;
	
	if(nodeaddr == NULL || *nodeaddr == NULL) return NULL;
	
	struct bouquet *node = *nodeaddr; 
	while(node != NULL)
	{
		next = node->next;
		prev = NULL;

		if(strcasecmp("c", node->name) > 0) {tnode = tmpnode[0]; tnodeaddr = &tmpnode[0];}
		else if(strcasecmp("f", node->name) > 0) {tnode = tmpnode[1]; tnodeaddr = &tmpnode[1];}
		else if(strcasecmp("i", node->name) > 0) {tnode = tmpnode[2]; tnodeaddr = &tmpnode[2];}
		else if(strcasecmp("l", node->name) > 0) {tnode = tmpnode[3]; tnodeaddr = &tmpnode[3];}
		else if(strcasecmp("o", node->name) > 0) {tnode = tmpnode[4]; tnodeaddr = &tmpnode[4];}
		else if(strcasecmp("r", node->name) > 0) {tnode = tmpnode[5]; tnodeaddr = &tmpnode[5];}
		else if(strcasecmp("u", node->name) > 0) {tnode = tmpnode[6]; tnodeaddr = &tmpnode[6];}
		else if(strcasecmp("x", node->name) > 0) {tnode = tmpnode[7]; tnodeaddr = &tmpnode[7];}
		else if(strcasecmp("z", node->name) > 0) {tnode = tmpnode[8]; tnodeaddr = &tmpnode[8];}
		else {tnode = tmpnode[9]; tnodeaddr = &tmpnode[9];}

		struct channel *chnode_a = getchannel(node->serviceid, node->transponderid);
		while(tnode != NULL)
		{
			struct channel *chnode_b = getchannel(tnode->serviceid, tnode->transponderid);
			if(chnode_a != NULL && chnode_b != NULL && strcasecmp(chnode_a->name, chnode_b->name) <= 0) break;
			prev = tnode;
			tnode = tnode->next;
		}

		if(prev == NULL)
		{
			*tnodeaddr = node;
			node->prev = NULL;
		}
		else
		{
			prev->next = node;
			node->prev = prev;
		}
		node->next = tnode;
		if(tnode != NULL) tnode->prev = node;

		node = next;
	}

	int i = 0, first = 0;
	prev = NULL; node = NULL;
	for(i = 0; i < 10; i++)
	{
		if(tmpnode[i] != NULL)
		{
			if(prev != NULL)
			{
				prev->next = tmpnode[i];
				tmpnode[i]->prev = prev;
			}

			if(first == 0)
			{
				*nodeaddr = tmpnode[i];
				first = 1;
			}

			node = tmpnode[i];
			while(node != NULL)
			{
				prev = node;
				node = node->next;
			}
		}
	}

	status.writebouquet = 1;
	return *nodeaddr;
}
*/

int writebouquet(const char *filename, struct bouquet *node)
{
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
		ret = fprintf(fd, "%d#%llu#%s#%s\n", node->serviceid, node->transponderid, node->streamurl, node->epgurl);
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
