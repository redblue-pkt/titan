#ifndef CHANNEL_H
#define CHANNEL_H

void debugchannel()
{
	struct channel* node = channel;

	while(node != NULL)
	{
		printf("%s %p <%p >%p\n", node->name, node, node->prev, node->next);
		node = node->next;
	}
}

int channelnottunable(struct channel* node)
{
	if(node == NULL) return 1;
	if(node->transponder == NULL) return 1;
	if(node->transponder->tunablestatus == 0)
	{
		if(fegetfree(node->transponder, 1, NULL, node) == NULL)
		{
			node->transponder->tunablestatus = 2;
			return 1;
		}
		else
		{
			node->transponder->tunablestatus = 1;
			return 0;
		}
	}
	else if(node->transponder->tunablestatus == 2)
		return 1;

	return 0;
}

//flag 0: lock
//flag 1: no lock
struct channel* getlastchannel(struct channel* node, int flag)
{
	struct channel *prev = NULL;

	if(flag == 0) m_lock(&status.channelmutex, 5);
	while(node != NULL)
	{
		prev = node;
		node = node->next;
	}
	if(flag == 0) m_unlock(&status.channelmutex, 5);

	return prev;
}

struct channel* gettmpchannel()
{
	m_lock(&status.channelmutex, 5);
  struct channel *node = channel;
  
	while(node != NULL)
	{
		if(node->servicetype == 99)
			break;
		node = node->next;
	}
	m_unlock(&status.channelmutex, 5);

	return node;
}

struct channel* getprevchannelbyservicetype(struct channel* node)
{
	struct channel* prev = NULL;

	if(node == NULL) node = channel;
	if(node == NULL) return NULL;

	prev = node->prev;
	while(prev != NULL && prev->servicetype != status.servicetype)
		prev = prev->prev;
	if(prev == NULL)
	{
		prev = getlastchannel(channel, 0);
		while(prev != NULL && prev->servicetype != status.servicetype)
			prev = prev->prev;
	}

	return prev;
}

struct channel* getnextchannelbyservicetype(struct channel* node)
{
	struct channel* next = NULL;

	if(node == NULL) node = channel;
	if(node == NULL) return NULL;

	next = node->next;

	while(next != NULL && next->servicetype != status.servicetype)
		next = next->next;
	if(next == NULL)
	{
		next = channel;
		while(next != NULL && next->servicetype != status.servicetype)
			next = next->next;
	}

	return next;
}

int movechannelblockdown(struct channel* node)
{
	int i = 0, ret = 0;
	struct channel* prev = NULL;

	if(node == NULL || channel == NULL) return 1;

	for(i = 0; i < status.moveblockcount; i++)
	{
		if(node == NULL) break;
		node = node->next;
		while(node != NULL && node->servicetype != status.servicetype)
			node = node->next;
	}

	for(i = 0; i < status.moveblockcount + 1; i++)
	{
		if(node == NULL) break;
		prev = getprevchannelbyservicetype(node);
		ret = movechanneldown(node);

		while(node->prev != NULL && node->prev->servicetype != status.servicetype)
			ret = movechanneldown(node);

		node = prev;
	}

	return ret;
}

int movechanneldown(struct channel* node)
{
	struct channel* prev = NULL, *next = NULL;

	if(node == NULL || channel == NULL) return 1;

	m_lock(&status.channelmutex, 5);

	//last node
	if(node->next == NULL)
	{
		if(node->prev != NULL)
			node->prev->next = NULL;
		node->prev = NULL;
		node->next = channel;
		channel->prev = node;
		channel = node;
		m_unlock(&status.channelmutex, 5);
		return 99;
	}

	//haenge node aus 
	if(node->prev != NULL)
		node->prev->next = node->next;
	else
		channel = node->next;
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

	m_unlock(&status.channelmutex, 5);

	status.writechannel = 1;
	return 0;
}

int movechannelblockup(struct channel* node)
{
	int i = 0, ret = 0;
	struct channel* next = NULL;

	if(node == NULL || channel == NULL) return 1;

	for(i = 0; i < status.moveblockcount + 1; i++)
	{
		if(node == NULL) break;
		next = getnextchannelbyservicetype(node);
		ret = movechannelup(node);

		while(node->next != NULL && node->next->servicetype != status.servicetype)
			ret = movechannelup(node);

		node = next;
	}

	return ret;
}

int movechannelup(struct channel* node)
{
	struct channel* prev = NULL, *next = NULL, *last = NULL;

	if(node == NULL || channel == NULL) return 1;

	m_lock(&status.channelmutex, 5);

	//first node
	if(node->prev == NULL)
	{
		last = getlastchannel(channel, 1);

		if(node->next != NULL)
			node->next->prev = NULL;
		channel = node->next;
		node->next = NULL;
		last->next = node;
		node->prev = last;
		m_unlock(&status.channelmutex, 5);
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
		channel = node;
	prev->prev = node;

	m_unlock(&status.channelmutex, 5);

	status.writechannel = 1;
	return 0;
}

struct channel* addchannel(char *line, int count, struct channel* last)
{
	struct channel *newnode = NULL, *prev = NULL, *node = NULL;
	char *name = NULL, *streamurl = NULL, *epgurl = NULL;

	int ret = 0;

	if(line == NULL) return NULL;

	newnode = (struct channel*)calloc(1, sizeof(struct channel));
	if(newnode == NULL)
	{
		err("no memory");
		return NULL;
	}

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

	name = malloc(MINMALLOC);
	if(name == NULL)
	{
		err("no memory");
		free(newnode);
		return NULL;
	}

printf("line: %s\n", line);

//    ret = sscanf(line, "%[^#]#%llu#%d#%d#%d#%"SCNu8"#%"SCNu8"#%"SCNu8"#%"SCNu16"#%"SCNu16"#%"SCNu8"#%"SCNu16"#%[^\n]\n", name, &newnode->transponderid, &newnode->providerid, &newnode->serviceid, &newnode->servicetype, &newnode->flag, &newnode->videocodec, &newnode->audiocodec, &newnode->videopid, &newnode->audiopid, &newnode->protect, &newnode->pcrpid, streamurl);
    ret = sscanf(line, "%[^#]#%llu#%d#%d#%d#%"SCNu8"#%"SCNu8"#%"SCNu8"#%"SCNu16"#%"SCNu16"#%"SCNu8"#%"SCNu16"#%[^#]#%s", name, &newnode->transponderid, &newnode->providerid, &newnode->serviceid, &newnode->servicetype, &newnode->flag, &newnode->videocodec, &newnode->audiocodec, &newnode->videopid, &newnode->audiopid, &newnode->protect, &newnode->pcrpid, streamurl, epgurl);

printf("ret: %d\n", ret);

	if(ret == 11)
	{
		newnode->pcrpid = -1;
		ret++;
	}

	if(ret == 13)
	{
        if(streamurl != NULL && ostrcmp("(null)", streamurl) == 0)
        {
        	newnode->streamurl = ostrcat(streamurl, NULL, 0, 0);
            printf("set newnode->streamurl: %s\n", streamurl);

        }
        else
            newnode->streamurl = NULL;
	}

	if(ret == 14)
	{
        if(streamurl != NULL && ostrcmp("(null)", streamurl) != 0)
        {
        	newnode->streamurl = ostrcat(streamurl, NULL, 0, 0);
            printf("set newnode->streamurl: %s\n", streamurl);

        }
        else
            newnode->streamurl = NULL;

        if(epgurl != NULL && ostrcmp("(null)", epgurl) != 0)
        {
        	newnode->epgurl = ostrcat(streamurl, NULL, 0, 0);
            printf("set newnode->epgurl: %s\n", epgurl);
        }
        else
            newnode->epgurl = NULL;
	}

	if(ret == 12)
	{
		ret++;
	}

	if(ret == 13)
	{
		ret++;
	}

	if(ret != 14 || getchannel(newnode->serviceid, newnode->transponderid) != NULL)
	{
		if(count > 0)
		{
			err("channellist line %d not ok or double", count);
		}
		else
		{
			err("add channel");
		}
		free(name);
		free(streamurl);
		free(epgurl);
		free(newnode);
		return NULL;
	}


	newnode->name = ostrshrink(name);
	//99 = tmp channel
	if(newnode->servicetype != 99)
	{
		newnode->transponder = gettransponder(newnode->transponderid);
		newnode->provider = getprovider(newnode->providerid);
		status.writechannel = 1;
	}

	m_lock(&status.channelmutex, 5);
	node = channel;

	modifychannelcache(newnode->serviceid, newnode->transponderid, newnode);

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
		channel = newnode;
	else
	{
		prev->next = newnode;
		newnode->prev = prev;
	}
	newnode->next = node;
	if(node != NULL) node->prev = newnode;
	
	m_unlock(&status.channelmutex, 5);
	return newnode;
}

struct channel* createchannel(char* name, uint64_t transponderid, int providerid, int serviceid, int servicetype, int flag, int videocodec, int audiocodec, int videopid, int audiopid, int protect, int pcrpid, char *streamurl, char *epgurl)
{
	struct channel* chnode = NULL;
	char* tmpstr = NULL;

	tmpstr = ostrcat(tmpstr, name, 1, 0);
	tmpstr = ostrcat(tmpstr, "#", 1, 0);
	tmpstr = ostrcat(tmpstr, ollutoa(transponderid), 1, 1);
	tmpstr = ostrcat(tmpstr, "#", 1, 0);
	tmpstr = ostrcat(tmpstr, oitoa(providerid), 1, 1);
	tmpstr = ostrcat(tmpstr, "#", 1, 0);
	tmpstr = ostrcat(tmpstr, oitoa(serviceid), 1, 1);
	tmpstr = ostrcat(tmpstr, "#", 1, 0);
	tmpstr = ostrcat(tmpstr, oitoa(servicetype), 1, 1);
	tmpstr = ostrcat(tmpstr, "#", 1, 0);
	tmpstr = ostrcat(tmpstr, oitoa(flag), 1, 1);
	tmpstr = ostrcat(tmpstr, "#", 1, 0);
	tmpstr = ostrcat(tmpstr, oitoa(videocodec), 1, 1);
	tmpstr = ostrcat(tmpstr, "#", 1, 0);
	tmpstr = ostrcat(tmpstr, oitoa(audiocodec), 1, 1);
	tmpstr = ostrcat(tmpstr, "#", 1, 0);
	tmpstr = ostrcat(tmpstr, oitoa(videopid), 1, 1);
	tmpstr = ostrcat(tmpstr, "#", 1, 0);
	tmpstr = ostrcat(tmpstr, oitoa(audiopid), 1, 1);
	tmpstr = ostrcat(tmpstr, "#", 1, 0);
	tmpstr = ostrcat(tmpstr, oitoa(protect), 1, 1);
	tmpstr = ostrcat(tmpstr, "#", 1, 0);
	tmpstr = ostrcat(tmpstr, oitoa(pcrpid), 1, 1);
	tmpstr = ostrcat(tmpstr, "#", 1, 0);
	tmpstr = ostrcat(tmpstr, streamurl, 1, 0);
	tmpstr = ostrcat(tmpstr, "#", 1, 0);
	tmpstr = ostrcat(tmpstr, epgurl, 1, 0);

	chnode = addchannel(tmpstr, 1, NULL);

	free(tmpstr);
	return chnode;
}

int readchannel(const char* filename)
{
	FILE *fd = NULL;
	char *fileline = NULL;
	int linecount = 0, len = 0;
	struct channel* last = NULL, *tmplast = NULL;

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
		last = addchannel(fileline, linecount, last);
		if(last != NULL) tmplast = last;
	}

	status.writechannel = 0;
	free(fileline);
	fclose(fd);
	return 0;
}

//flag 0: del bouquet
//flag 1: don't del bouquet
int delchannel(int serviceid, uint64_t transponderid, int flag)
{
	int ret = 1;
	struct provider* providernode = NULL;

	m_lock(&status.channelmutex, 5);
    struct channel *node = channel, *prev = channel;

	while(node != NULL)
	{
		if(node->serviceid == serviceid && node->transponderid == transponderid && getservicebychannel(node) == NULL)
		{
			ret = 0;
			if(node->servicetype != 99) status.writechannel = 1;
			if(node == channel)
			{
				channel = node->next;
				if(channel != NULL)
					channel->prev = NULL;
			}
			else
			{
				prev->next = node->next;
				if(node->next != NULL)
					node->next->prev = prev;
			}

			if(flag == 0)
			{
				delbouquetbychannel(node->serviceid, node->transponderid);
				delepgscanlist(node->serviceid, node->transponderid);
			}
			else
				setbouquetchanneltonullmain(node->serviceid, node->transponderid);

			providernode = node->provider;
			delchannelcache(node->serviceid, node->transponderid);
			delchannelhistory(node);
			delmostzap(node->serviceid, node->transponderid, 0);

			freeaudiotrack(node);
			free(node->audiotrack);
			node->audiotrack = NULL;

			freesubtitle(node);
			free(node->subtitle);
			node->subtitle = NULL;
			
			freelinkedchannel(node);
			free(node->linkedchannel);
			node->linkedchannel = NULL;

			freepmt(node);
			free(node->pmt);
			node->pmt = NULL;

			freecadesc(node);
			free(node->cadesc);
			node->cadesc = NULL;

			freeesinfo(node);
			free(node->esinfo);
			node->esinfo = NULL;

			freeepg(node);
			node->epg = NULL;

			free(node->name);
			node->name = NULL;
      
            free(node->hbbtvurl);
            node->hbbtvurl = NULL;

			free(node);
			node = NULL;

			delprovidernotused(providernode);
			break;
		}

		prev = node;
		node = node->next;
	}

	recalcbouquetnr();
	m_unlock(&status.channelmutex, 5);
	return ret;
}

void delchannelbytransponder(uint64_t transponderid)
{
	struct channel *node = channel, *prev = channel;

	while(node != NULL)
	{
		prev = node;
		node = node->next;
		if(prev != NULL && prev->transponderid == transponderid)
			delchannel(prev->serviceid, prev->transponderid, 0);
	}
}

//flag 0: del bouquet
//flag 1: don't del bouquet
void freechannel(int flag)
{
	struct channel *node = channel, *prev = channel;

	while(node != NULL)
	{
		prev = node;
		node = node->next;
		if(prev != NULL)
			delchannel(prev->serviceid, prev->transponderid, flag);
	}
}

/*
struct channel* sortchannel()
{
	struct channel *node = channel;
	struct channel *nodea = NULL, *nodeb = NULL, *nodec = NULL, *noded = NULL;
	struct channel *nodetmp = NULL;

	if(node == NULL) return NULL;
	
	struct channel **nodeaddr = &channel;

	if(node != NULL)
	{
		while(noded != node->next)
		{
			nodec = node;
			nodea = node;
			nodeb = nodea->next;

			while(nodea != noded)
			{
				if(strcasecmp(nodea->name, nodeb->name) > 0)
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
	struct channel* prev = NULL;
	nodetmp = node;
	while(nodetmp != NULL)
	{
		nodetmp->prev = prev;
		prev = nodetmp;

		nodetmp = nodetmp->next;
	}

	status.writechannel = 1;
	return channel;
}
*/

struct channel* sortchannel()
{
	struct channel* tmpnode[10] = {0};
	struct channel* node = channel, *tnode = NULL;
	struct channel *next = NULL, *prev = NULL;
	struct channel **tnodeaddr = NULL;
	
	if(node == NULL) return NULL;

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

		while(tnode != NULL && strcasecmp(node->name, tnode->name) > 0)
		{
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
				channel = tmpnode[i];
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

	status.writechannel = 1;
	return channel;
}

int writechannel(const char *filename)
{
	FILE *fd = NULL;
	struct channel *node = NULL;
	int ret = 0;

	fd = fopen(filename, "w");
	if(fd == NULL)
	{
		perr("can't open %s", filename);
		return 1;
	}

	m_lock(&status.channelmutex, 5);
	node = channel;

	while(node != NULL)
	{
		if(node->servicetype == 99)
		{
			node = node->next;
			continue;
		}
		ret = fprintf(fd, "%s#%llu#%d#%d#%d#%d#%d#%d#%d#%d#%d#%d#%s#%s\n", node->name, node->transponderid, node->providerid, node->serviceid, node->servicetype, node->flag, node->videocodec, node->audiocodec, node->videopid, node->audiopid, node->protect, node->pcrpid, node->streamurl, node->epgurl);
		if(ret < 0)
		{
			perr("writting file %s", filename);
		}
		node = node->next;
	}
	m_unlock(&status.channelmutex, 5);

	fclose(fd);
	return 0;
}

#endif
