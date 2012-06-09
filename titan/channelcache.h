#ifndef CHANNELCACHE_H
#define CHANNELCACHE_H

void debugchannelcache()
{
	int i, count = 0, maxcount = 0;;
	struct channelcache* node = NULL;
	
	for(i = 0; i < CHANNELCACHEMAX; i++)
	{
		node = channelcache[i];
		count = 0;
		while(node != NULL)
		{
			maxcount++;
			count++;
			node = node->next;
		}
		printf("hash=%d, count=%d\n", i, count);
	}
	printf("maxcount=%d\n", maxcount);
}

struct channel* getchannel(int serviceid, unsigned long transponderid)
{
	unsigned int hash; 
	struct channelcache* node = NULL;

	hash = (transponderid + serviceid) % CHANNELCACHEMAX;
	if(hash < 0 || hash >= CHANNELCACHEMAX) hash = 0;

	m_lock(&status.channelmutex, 5);
	node = channelcache[hash];

	while(node != NULL)
	{
		if(node->serviceid == serviceid && node->transponderid == transponderid)
		{
			m_unlock(&status.channelmutex, 5);
			return node->chnode;
		}

		node = node->next;
	}

	m_unlock(&status.channelmutex, 5);
	return NULL;
}

struct channelcache* modifychannelcache(int serviceid, unsigned long transponderid, struct channel* chnode)
{
	unsigned int hash; 
	//struct channelcache* node = NULL, *prev = NULL, *newnode = NULL;
	struct channelcache* node = NULL, *newnode = NULL;

	hash = (transponderid + serviceid) % CHANNELCACHEMAX;
	if(hash < 0 || hash >= CHANNELCACHEMAX) hash = 0;

	newnode = (struct channelcache*)calloc(1, sizeof(struct channelcache));
	if(newnode == NULL)
	{
		err("no memory");
		return NULL;
	}

	newnode->serviceid = serviceid;
	newnode->transponderid = transponderid;
	newnode->chnode = chnode;

	node = channelcache[hash];
/*
	prev = channelcache[hash];
	
	if(node != NULL)
	{
		while(node != NULL)
		{
			prev = node;
			node = node->next;
		}
		prev->next = newnode;
	}
	else
		channelcache[hash] = newnode;
*/
	channelcache[hash] = newnode;
	newnode->next = node;

	return newnode;
}

void delchannelcache(int serviceid, unsigned long transponderid)
{
	unsigned int hash; 
	struct channelcache *node = NULL, *prev = NULL;

	hash = (transponderid + serviceid) % CHANNELCACHEMAX;
	if(hash < 0 || hash >= CHANNELCACHEMAX) hash = 0;
	node = channelcache[hash];
	prev = channelcache[hash];

	while(node != NULL)
	{
		if(serviceid == node->serviceid && transponderid == node->transponderid)
		{
			if(node == channelcache[hash])
				channelcache[hash] = node->next;
			else
				prev->next = node->next;

			free(node);
			node = NULL;
			break;
		}

		prev = node;
		node = node->next;
	}
}

#endif
