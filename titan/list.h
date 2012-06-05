#ifndef LIST_H
#define LIST_H

void debuglist(struct clist* node, char* search)
{
	while(node != NULL)
	{
		if(search != NULL)
		{	
			if(ostrncmp(node->key, search, strlen(search)) == 0)
				printf("key=%s, tmp=%s, val=%s, def=%s\n", node->key, node->tmp, node->value, node->def);
			
		}
		else
			printf("key=%s, tmp=%s, val=%s, def=%s\n", node->key, node->tmp, node->value, node->def);
		node = node->next;
	}
}

struct clist* addlisttmp(struct clist **clist, char *key1, char *value1)
{
	debug(1000, "in");
	m_lock(&status.clistmutex, 12);
	struct clist *newnode = NULL, *node = NULL, *prev = NULL;
	char *key = NULL, *value = NULL;
	int hash = 0;

	if(key1 == NULL || value1 == NULL)
	{
		debug(1000, "out -> NULL dedect");
		m_unlock(&status.clistmutex, 12);
		return NULL;
	}

	key = ostrcat(key1, NULL, 0, 0);
	if(key == NULL)
	{
		err("no memory");
		m_unlock(&status.clistmutex, 12);
		return NULL;
	}

	value = ostrcat(value1, NULL, 0, 0);
	if(value == NULL)
	{
		err("no memory");
		free(key);
		m_unlock(&status.clistmutex, 12);
		return NULL;
	}

	hash = key1[0] - 96;
	if(hash < 0 || hash >= LISTHASHSIZE) hash = 0;
	node = clist[hash];
	prev = clist[hash];

	newnode = (struct clist*)malloc(sizeof(struct clist));	
	if(newnode == NULL)
	{
		err("no memory");
		free(key);
		free(value);
		m_unlock(&status.clistmutex, 12);
		return NULL;
	}

	memset(newnode, 0, sizeof(struct clist));
	newnode->key = key;
	newnode->tmp = value;

	if(node != NULL)
	{
		while(node != NULL)
		{
			if(ostrcmp(node->key, key) == 0)
			{
				free(key); key = NULL;
				free(newnode); newnode = NULL;
				if(ostrcmp(node->tmp, value) == 0)
				{
					free(value);
					m_unlock(&status.clistmutex, 12);
					return NULL;
				}
				free(node->tmp);
				node->tmp = value;
				debug(1000, "out -> key %s found, use change instread add", key);
				m_unlock(&status.clistmutex, 12);
				return newnode;
			}
			prev = node;
			node = node->next;
		}
		prev->next = newnode;
	}
	else
		clist[hash] = newnode;

	debug(1000, "out");
	m_unlock(&status.clistmutex, 12);
	return newnode;
}

struct clist* addlistdef(struct clist **clist, char *key1, char *value1)
{
	debug(1000, "in");
	m_lock(&status.clistmutex, 12);
	struct clist *newnode = NULL, *node = NULL, *prev = NULL;
	char *key = NULL, *value = NULL;
	int hash = 0;

	if(key1 == NULL || value1 == NULL)
	{
		debug(1000, "out -> NULL dedect");
		m_unlock(&status.clistmutex, 12);
		return NULL;
	}

	key = ostrcat(key1, NULL, 0, 0);
	if(key == NULL)
	{
		err("no memory");
		m_unlock(&status.clistmutex, 12);
		return NULL;
	}

	value = ostrcat(value1, NULL, 0, 0);
	if(value == NULL)
	{
		err("no memory");
		free(key);
		m_unlock(&status.clistmutex, 12);
		return NULL;
	}

	hash = key1[0] - 96;
	if(hash < 0 || hash >= LISTHASHSIZE) hash = 0;
	node = clist[hash];
	prev = clist[hash];

	newnode = (struct clist*)malloc(sizeof(struct clist));	
	if(newnode == NULL)
	{
		err("no memory");
		free(key);
		free(value);
		m_unlock(&status.clistmutex, 12);
		return NULL;
	}

	memset(newnode, 0, sizeof(struct clist));
	newnode->key = key;
	newnode->def = value;

	if(node != NULL)
	{
		while(node != NULL)
		{
			if(ostrcmp(node->key, key) == 0)
			{
				free(key); key = NULL;
				free(newnode); newnode = NULL;
				if(ostrcmp(node->def, value) == 0)
				{
					free(value);
					m_unlock(&status.clistmutex, 12);
					return NULL;
				}
				free(node->def);
				node->def = value;
				debug(1000, "out -> key %s found, use change instread add", key);
				m_unlock(&status.clistmutex, 12);
				return newnode;
			}
			prev = node;
			node = node->next;
		}
		prev->next = newnode;
	}
	else
		clist[hash] = newnode;

	debug(1000, "out");
	m_unlock(&status.clistmutex, 12);
	return newnode;
}

struct clist* addlist(struct clist **clist, char *key1, char *value1)
{
	debug(1000, "in");
	m_lock(&status.clistmutex, 12);
	struct clist *newnode = NULL, *node = NULL, *prev = NULL;
	char *key = NULL, *value = NULL;
	int hash = 0;

	if(key1 == NULL || value1 == NULL)
	{
		debug(1000, "out -> NULL dedect");
		m_unlock(&status.clistmutex, 12);
		return NULL;
	}

	key = ostrcat(key1, NULL, 0, 0);
	if(key == NULL)
	{
		err("no memory");
		m_unlock(&status.clistmutex, 12);
		return NULL;
	}

	value = ostrcat(value1, NULL, 0, 0);
	if(value == NULL)
	{
		err("no memory");
		free(key);
		m_unlock(&status.clistmutex, 12);
		return NULL;
	}

	hash = key1[0] - 96;
	if(hash < 0 || hash >= LISTHASHSIZE) hash = 0;
	node = clist[hash];
	prev = clist[hash];

	newnode = (struct clist*)malloc(sizeof(struct clist));	
	if(newnode == NULL)
	{
		err("no memory");
		free(key);
		free(value);
		m_unlock(&status.clistmutex, 12);
		return NULL;
	}

	memset(newnode, 0, sizeof(struct clist));
	newnode->key = key;
	newnode->value = value;

	if(node != NULL)
	{
		while(node != NULL)
		{
			if(ostrcmp(node->key, key) == 0)
			{
				free(key); key = NULL;
				free(newnode); newnode = NULL;
				if(ostrcmp(node->value, value) == 0)
				{
					free(value);
					m_unlock(&status.clistmutex, 12);
					return NULL;
				}
				if(node->value == NULL && ostrcmp(node->def, value) == 0)
				{
					free(value);
					m_unlock(&status.clistmutex, 12);
					return NULL;
				}
				free(node->value);
				node->value = value;
				debug(1000, "out -> key %s found, use change instread add", key);
				m_unlock(&status.clistmutex, 12);
				return node;
			}
			prev = node;
			node = node->next;
		}
		prev->next = newnode;
	}
	else
		clist[hash] = newnode;

	debug(1000, "out");
	m_unlock(&status.clistmutex, 12);
	return newnode;
}

int writelist(struct clist **clist, const char *filename)
{
	debug(1000, "in");
	m_lock(&status.clistmutex, 12);
	FILE *fd = NULL;
	struct clist *node = NULL;
	int ret = 0, i;

	fd = fopen(filename, "w");
	if(fd == NULL)
	{
		perr("can't open %s", filename);
		m_unlock(&status.clistmutex, 12);
		return 1;
	}

	for(i = 0; i < LISTHASHSIZE; i++)
	{
		node = clist[i];

		while(node != NULL)
		{
			if(node->value == NULL)
			{
				node = node->next;
				continue;
			}
			if(ostrcmp(node->value, node->def) == 0 || strlen(node->value) == 0)
			{
				node = node->next;
				continue;
			}
			ret = fprintf(fd, "%s=%s\n", node->key, node->value);
			if(ret < 0)
			{
				perr("writting file %s (ret = %d)", filename, ret);
			}
			node = node->next;
		}
	}
	
	fclose(fd);
	debug(1000, "out");
	m_unlock(&status.clistmutex, 12);
	return 0;
}

char* getlistbyval(struct clist **clist, char *value, char *ext)
{
	//debug(1000, "in");
	m_lock(&status.clistmutex, 12);
	struct clist *node = NULL;
	char* tmpstr = NULL, *tmpval = NULL;
	int i;

	if(ext != NULL)
		tmpstr = ostrcat(value, ext, 0, 0);
	else
		tmpstr = ostrcat(NULL, value, 0, 0);

	for(i = 0; i < LISTHASHSIZE; i++)
	{
		node = clist[i];
		while(node != NULL)
		{
			if(node->tmp != NULL)
				tmpval = node->tmp;
			else if(node->value != NULL)
				tmpval = node->value;
			else if(node->def != NULL)
				tmpval = node->def;

			if(ostrcmp(tmpval, tmpstr) == 0)
			{
				free(tmpstr);
				//debug(1000, "out");
				m_unlock(&status.clistmutex, 12);
				return node->key;
			}

			node = node->next;
		}
	}
	free(tmpstr);
	debug(100, "value not found (%s)", value);
	m_unlock(&status.clistmutex, 12);
	return NULL;
}

char* getlistnotmp(struct clist **clist, char *key, char *ext)
{
	//debug(1000, "in");
	m_lock(&status.clistmutex, 12);
	struct clist *node = NULL;
	char* tmpstr = NULL;
	int hash = 0;

	if(key == NULL)
	{
		m_unlock(&status.clistmutex, 12);
		return NULL;
	}
	if(ext != NULL)
		tmpstr = ostrcat(key, ext, 0, 0);
	else
		tmpstr = ostrcat(NULL, key, 0, 0);

	hash = key[0] - 96;
	if(hash < 0 || hash >= LISTHASHSIZE) hash = 0;
	node = clist[hash];

	while(node != NULL)
	{
		if(ostrcmp(node->key, tmpstr) == 0)
		{
			free(tmpstr);
			//debug(1000, "out");
			if(node->value != NULL)
			{
				m_unlock(&status.clistmutex, 12);
				return node->value;
			}
			else
			{
				m_unlock(&status.clistmutex, 12);
				return node->def;
			}
		}

		node = node->next;
	}
	free(tmpstr);
	debug(100, "key not found (%s)", key);
	m_unlock(&status.clistmutex, 12);
	return NULL;
}

char* getlist(struct clist **clist, char *key, char *ext)
{
	//debug(1000, "in");
	m_lock(&status.clistmutex, 12);
	struct clist *node = NULL;
	char* tmpstr = NULL;
	int hash = 0;
	
	if(key == NULL)
	{
		m_unlock(&status.clistmutex, 12);
		return NULL;
	}
	if(ext != NULL)
		tmpstr = ostrcat(key, ext, 0, 0);
	else
		tmpstr = ostrcat(NULL, key, 0, 0);

	hash = key[0] - 96;
	if(hash < 0 || hash >= LISTHASHSIZE) hash = 0;
	node = clist[hash];

	while(node != NULL)
	{
		if(ostrcmp(node->key, tmpstr) == 0)
		{
			free(tmpstr);
			//debug(1000, "out");
			if(node->tmp != NULL && strlen(node->tmp) > 0)
			{
				m_unlock(&status.clistmutex, 12);
				return node->tmp;
			}
			else if(node->value != NULL)
			{
				m_unlock(&status.clistmutex, 12);
				return node->value;
			}
			else
			{
				m_unlock(&status.clistmutex, 12);
				return node->def;
			}
		}

		node = node->next;
	}
	free(tmpstr);
	debug(100, "key not found (%s)", key);
	m_unlock(&status.clistmutex, 12);
	return NULL;
}

//flag 0: set lock
//flag 1: no lock
void dellist(struct clist **clist, char *key, int flag)
{
	debug(1000, "in");
	if(flag == 0) m_lock(&status.clistmutex, 12);
	struct clist *node = NULL, *prev = NULL;
	int hash = 0;

	if(key == NULL)
	{
		if(flag == 0) m_unlock(&status.clistmutex, 12);
		return;
	}
	hash = key[0] - 96;
	if(hash < 0 || hash >= LISTHASHSIZE) hash = 0;
	node = clist[hash];
	prev = clist[hash];

	while(node != NULL)
	{
		if(ostrcmp(node->key, key) == 0)
		{
			if(node == clist[hash])
				clist[hash] = node->next;
			else
				prev->next = node->next;

			free(node->key);
			node->key = NULL;

			free(node->value);
			node->value = NULL;

			free(node->def);
			node->def = NULL;

			free(node->tmp);
			node->tmp = NULL;

			free(node);
			node = NULL;
			break;
		}

		prev = node;
		node = node->next;
	}
	debug(1000, "out");
	if(flag == 0) m_unlock(&status.clistmutex, 12);
}

void dellisttmpall(struct clist **clist)
{
	debug(1000, "in");
	m_lock(&status.clistmutex, 12);
	struct clist *node = NULL, *prev = NULL;
	int i;

	for(i = 0; i < LISTHASHSIZE; i++)
	{
		node = clist[i];
		while(node != NULL)
		{
			free(node->tmp);
			node->tmp = NULL;

			if(node->tmp != NULL && strlen(node->tmp) == 0)
			{
				free(node->tmp);
				node->tmp = NULL;
			}

			if(node->value == NULL && node->tmp == NULL && node->def == NULL)
				dellist(clist, node->key, 1);

			prev = node;
			node = node->next;
		}
	}
	debug(1000, "out");
	m_unlock(&status.clistmutex, 12);
}

void dellisttmp(struct clist **clist, char *key)
{
	debug(1000, "in");
	m_lock(&status.clistmutex, 12);
	struct clist *node = NULL, *prev = NULL;
	int hash = 0;

	if(key == NULL)
	{
		m_unlock(&status.clistmutex, 12);
		return;
	}
	hash = key[0] - 96;
	if(hash < 0 || hash >= LISTHASHSIZE) hash = 0;
	node = clist[hash];
	prev = clist[hash];

	while(node != NULL)
	{
		if(ostrcmp(node->key, key) == 0)
		{
			free(node->tmp);
			node->tmp = NULL;
			break;
		}

		prev = node;
		node = node->next;
	}
	debug(1000, "out");
	m_unlock(&status.clistmutex, 12);
}

void freelist(struct clist** clist)
{
	debug(1000, "in");
	m_lock(&status.clistmutex, 12);
	struct clist *node = NULL, *prev = NULL;
	int i;

	for(i = 0; i < LISTHASHSIZE; i++)
	{
		node = clist[i];
		while(node != NULL)
		{
			prev = node;
			node = node->next;
			if(prev != NULL)
				dellist(clist, prev->key, 1);
		}
	}
	debug(1000, "out");
	m_unlock(&status.clistmutex, 12);
}

int writelisttmp(struct clist **clist)
{
	debug(1000, "in");
	m_lock(&status.clistmutex, 12);
	struct clist *node = NULL;
	int ret = 1, i;

	for(i = 0; i < LISTHASHSIZE; i++)
	{
		node = clist[i];
		while(node != NULL)
		{
			if(node->tmp != NULL && strlen(node->tmp) == 0)
			{
				free(node->tmp); node->tmp = NULL;
				free(node->value); node->value = NULL;
				ret = 0;
			}
			if(node->value == NULL && node->tmp == NULL && node->def == NULL)
			{
				dellist(clist, node->key, 1);
				node = node->next;
				ret = 0;
				continue;
			}
			if(ostrcmp(node->tmp, node->def) == 0)
			{
				node = node->next;
				continue;
			}
			if(node->tmp != NULL)
			{
				free(node->value);
				node->value = node->tmp;
				node->tmp = NULL;
				ret = 0;
			}
			node = node->next;
		}
	}

	debug(1000, "out");
	m_unlock(&status.clistmutex, 12);
	return ret;
}

#endif
