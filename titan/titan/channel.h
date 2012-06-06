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
		if(fegetfree(node->transponder, 1, NULL) == NULL)
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
	debug(1000, "in");
	struct channel *prev = NULL;

	if(flag == 0) m_lock(&status.channelmutex, 5);
	while(node != NULL)
	{
		prev = node;
		node = node->next;
	}
	if(flag == 0) m_unlock(&status.channelmutex, 5);

	debug(1000, "out");
	return prev;
}

struct channel* gettmpchannel()
{
	debug(1000, "in");

	m_lock(&status.channelmutex, 5);
  struct channel *node = channel;
  
	while(node != NULL)
	{
		if(node->servicetype == 99)
			break;
		node = node->next;
	}
	m_unlock(&status.channelmutex, 5);

	debug(1000, "out");
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

	if(node == NULL || channel == NULL)
	{
		debug(1000, "NULL detect");
		return 1;
	}

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

	if(node == NULL || channel == NULL)
	{
		debug(1000, "NULL detect");
		return 1;
	}

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

	if(node == NULL || channel == NULL)
	{
		debug(1000, "NULL detect");
		return 1;
	}

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

	if(node == NULL || channel == NULL)
	{
		debug(1000, "NULL detect");
		return 1;
	}

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
	//debug(1000, "in");
	struct channel *newnode = NULL, *prev = NULL, *node = NULL;
	char *name = NULL;
	int ret = 0;

	if(line == NULL) return NULL;

	newnode = (struct channel*)malloc(sizeof(struct channel));	
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

	memset(newnode, 0, sizeof(struct channel));

	ret = sscanf(line, "%[^#]#%lu#%d#%d#%d#%"SCNu8"#%"SCNu8"#%"SCNu8"#%"SCNu16"#%"SCNu16"#%"SCNu8, name, &newnode->transponderid, &newnode->providerid, &newnode->serviceid, &newnode->servicetype, &newnode->flag, &newnode->videocodec, &newnode->audiocodec, &newnode->videopid, &newnode->audiopid, &newnode->protect);
	if(ret != 11 || getchannel(newnode->serviceid, newnode->transponderid) != NULL)
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
		free(newnode);
		return NULL;
	}

	newnode->name = ostrcat(name, NULL, 1, 0);
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
	//debug(1000, "out");
	return newnode;
}

struct channel* createchannel(char* name, unsigned long transponderid, int providerid, int serviceid, int servicetype, int flag, int videocodec, int audiocodec, int videopid, int audiopid, int protect)
{
	struct channel* chnode = NULL;
	char* tmpstr = NULL;

	tmpstr = ostrcat(tmpstr, name, 1, 0);
	tmpstr = ostrcat(tmpstr, "#", 1, 0);
	tmpstr = ostrcat(tmpstr, olutoa(transponderid), 1, 1);
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

	chnode = addchannel(tmpstr, 1, NULL);

	free(tmpstr);
	return chnode;
}

int readchannel(const char* filename)
{
	debug(1000, "in");
	FILE *fd = NULL;
	char *fileline = NULL;
	int linecount = 0;
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
		if(fileline[strlen(fileline) - 1] == '\n')
			fileline[strlen(fileline) - 1] = '\0';
		if(fileline[strlen(fileline) - 1] == '\r')
			fileline[strlen(fileline) - 1] = '\0';

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
int delchannel(int serviceid, unsigned long transponderid, int flag)
{
	debug(1000, "in");
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
	debug(1000, "out");
	return ret;
}

void delchannelbytransponder(unsigned long transponderid)
{
	debug(1000, "in");
	struct channel *node = channel, *prev = channel;

	while(node != NULL)
	{
		prev = node;
		node = node->next;
		if(prev != NULL && prev->transponderid == transponderid)
			delchannel(prev->serviceid, prev->transponderid, 0);
	}
	debug(1000, "out");
}

//flag 0: del bouquet
//flag 1: don't del bouquet
void freechannel(int flag)
{
	debug(1000, "in");
	struct channel *node = channel, *prev = channel;

	while(node != NULL)
	{
		prev = node;
		node = node->next;
		if(prev != NULL)
			delchannel(prev->serviceid, prev->transponderid, flag);
	}
	debug(1000, "out");
}

int writechannel(const char *filename)
{
	debug(1000, "in");
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
		ret = fprintf(fd, "%s#%lu#%d#%d#%d#%d#%d#%d#%d#%d#%d\n", node->name, node->transponderid, node->providerid, node->serviceid, node->servicetype, node->flag, node->videocodec, node->audiocodec, node->videopid, node->audiopid, node->protect);
		if(ret < 0)
		{
			perr("writting file %s", filename);
		}
		node = node->next;
	}
	m_unlock(&status.channelmutex, 5);

	fclose(fd);
	debug(1000, "out");
	return 0;
}

#endif
