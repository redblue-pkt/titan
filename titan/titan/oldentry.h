#ifndef OLDENTRY_H
#define OLDENTRY_H

void oldentrythreadfunc(struct stimerthread* self)
{
	debug(401, "start oldentry thread");

	m_lock(&status.oldentrymutex, 18);
	struct oldentry* prev = oldentry, *node = oldentry;

	while(node != NULL)
	{
		prev = node;
		node = node->next;
		if(prev->del < time(NULL))
			deloldentry(prev, 1);
	}

	m_unlock(&status.oldentrymutex, 18);
	debug(401, "end oldentry thread");
}

//type 0: epg
//type 1: download
//type 2: copyfile
struct oldentry* addoldentry(void* entry, int type, time_t del, struct oldentry* last)
{
	struct oldentry *newnode = NULL, *node = NULL;

	newnode = (struct oldentry*)malloc(sizeof(struct oldentry));	
	if(newnode == NULL)
	{
		err("no mem");
		return NULL;
	}

	memset(newnode, 0, sizeof(struct oldentry));

	newnode->entry = entry;
	newnode->type = type;
	newnode->del = del;

	m_lock(&status.oldentrymutex, 18);
	node = oldentry;

	if(node != NULL)
	{
		if(last == NULL)
		{
			while(node->next != NULL)
				node = node->next;
			node->next = newnode;
		}
		else
			last->next = newnode;
	}
	else
		oldentry = newnode;

	m_unlock(&status.oldentrymutex, 18);
	return newnode;
}

//flag 0: lock
//flag 1: no lock
void deloldentry(struct oldentry *entry, int flag)
{
	if(flag == 0) m_lock(&status.oldentrymutex, 18);
	struct oldentry *node = oldentry, *prev = oldentry;

	while(node != NULL)
	{
		if(node == entry)
		{
			if(node == oldentry)
				oldentry = node->next;
			else
				prev->next = node->next;

			if(node->type == 0)
				deloldentryepg(node->entry);
			else if(node->type == 1)
				free(node->entry);
			else if(node->type == 2)
				free(node->entry);
			
			node->entry = NULL;

			free(node);
			node = NULL;
			break;
		}

		prev = node;
		node = node->next;
	}

	if(flag == 0) m_unlock(&status.oldentrymutex, 18);
}

void freeoldentry()
{
	m_lock(&status.oldentrymutex, 18);
	struct oldentry *node = oldentry, *prev = oldentry;

	while(node != NULL)
	{
		prev = node;
		node = node->next;
		if(prev != NULL)
			deloldentry(prev, 1);
	}

	m_unlock(&status.oldentrymutex, 18);
}

#endif
