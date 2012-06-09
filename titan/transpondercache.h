#ifndef TRANSPONDERCACHE_H
#define TRANSPONDERCACHE_H

void debugtranspondercache()
{
	int i, count = 0, maxcount = 0;;
	struct transpondercache* node = NULL;
	
	for(i = 0; i < TRANSPONDERCACHEMAX; i++)
	{
		node = transpondercache[i];
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

struct transponder* gettransponder(unsigned long transponderid)
{
	unsigned int hash; 
	struct transpondercache* node = NULL;

	hash = (transponderid) % TRANSPONDERCACHEMAX;
	if(hash < 0 || hash >= TRANSPONDERCACHEMAX) hash = 0;
	node = transpondercache[hash];

	while(node != NULL)
	{
		if(node->transponderid == transponderid)
			return node->tpnode;

		node = node->next;
	}
	return NULL;
}

struct transpondercache* modifytranspondercache(unsigned long transponderid, struct transponder* tpnode)
{
	unsigned int hash; 
	//struct transpondercache* node = NULL, *prev = NULL, *newnode = NULL;
	struct transpondercache* node = NULL, *newnode = NULL;

	hash = (transponderid) % TRANSPONDERCACHEMAX;
	if(hash < 0 || hash >= TRANSPONDERCACHEMAX) hash = 0;

	newnode = (struct transpondercache*)calloc(1, sizeof(struct transpondercache));
	if(newnode == NULL)
	{
		err("no memory");
		return NULL;
	}

	newnode->transponderid = transponderid;
	newnode->tpnode = tpnode;

	node = transpondercache[hash];
/*
	prev = transpondercache[hash];
	
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
		transpondercache[hash] = newnode;
*/
	transpondercache[hash] = newnode;
	newnode->next = node;

	return newnode;
}

void deltranspondercache(unsigned long transponderid, struct transponder* tpnode)
{
	unsigned int hash; 
	struct transpondercache *node = NULL, *prev = NULL;

	hash = (transponderid) % TRANSPONDERCACHEMAX;
	if(hash < 0 || hash >= TRANSPONDERCACHEMAX) hash = 0;
	node = transpondercache[hash];
	prev = transpondercache[hash];

	while(node != NULL)
	{
		if((tpnode == NULL && transponderid == node->transponderid) || (tpnode != NULL && tpnode == node->tpnode))
		{
			if(node == transpondercache[hash])
				transpondercache[hash] = node->next;
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
