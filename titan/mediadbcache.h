#ifndef MEDIADBCACHE_H
#define MEDIADBCACHE_H

void debugmediadbcache()
{
	int i, count = 0, maxcount = 0;;
	struct mediadbcache* node = NULL;
	
	for(i = 0; i < MEDIADBCACHEMAX; i++)
	{
		node = mediadbcache[i];
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

// flag 0 path + file
// flag 1 file
struct mediadb* getmediadb(char* file, int flag)
{
	unsigned int hash; 
	struct mediadbcache* node = NULL;

	hash = gethash(file) % MEDIADBCACHEMAX;
	if(hash < 0 || hash >= MEDIADBCACHEMAX) hash = 0;

	m_lock(&status.mediadbmutex, 17);
	node = mediadbcache[hash];

	while(node != NULL)
	{
		if(flag == 0 && ostrcmp(file, node->file) == 0)
		{
			m_unlock(&status.mediadbmutex, 17);
			return node->mediadbnode;
		}
		else if(flag == 1 && ostrcmp(file, basename(node->file)) == 0)
		{					
			m_unlock(&status.mediadbmutex, 17);
			return node->mediadbnode;
		}

		node = node->next;
	}

	m_unlock(&status.mediadbmutex, 17);
	return NULL;
}

struct mediadbcache* modifymediadbcache(char* file, struct mediadb* mnode)
{
	unsigned int hash; 
	//struct mediadbcache* node = NULL, *prev = NULL, *newnode = NULL;
	struct mediadbcache* node = NULL, *newnode = NULL;

	hash = gethash(file) % MEDIADBCACHEMAX;
	if(hash < 0 || hash >= MEDIADBCACHEMAX) hash = 0;

	newnode = (struct mediadbcache*)calloc(1, sizeof(struct mediadbcache));
	if(newnode == NULL)
	{
		err("no memory");
		return NULL;
	}

  //TODO: copy file???
	newnode->file = file;
	newnode->mediadbnode = mnode;

	node = mediadbcache[hash];
/*
	prev = mediadbcache[hash];
	
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
		mediadbcache[hash] = newnode;
*/
	mediadbcache[hash] = newnode;
	newnode->next = node;

	return newnode;
}

void delmediadbcache(char* file)
{
	unsigned int hash; 
	struct mediadbcache *node = NULL, *prev = NULL;

	hash = gethash(file) % MEDIADBCACHEMAX;
	if(hash < 0 || hash >= MEDIADBCACHEMAX) hash = 0;
	node = mediadbcache[hash];
	prev = mediadbcache[hash];

	while(node != NULL)
	{
		if(ostrcmp(file, node->file) == 0)
		{
			if(node == mediadbcache[hash])
				mediadbcache[hash] = node->next;
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
