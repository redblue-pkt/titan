#ifndef QUEUE_H
#define QUEUE_H

void debugqueue()
{
	struct queue* node = queue;

	while(node != NULL)
	{
		printf("%s %p <%p >%p\n", (char*)node->data, node, node->prev, node->next);
		node = node->next;
	}
}

struct queue* addqueue(int type, void* data, int len, int flag, struct queue* last)
{
	//debug(1000, "in");

	m_lock(&status.queuemutex, 11);

	struct queue *newnode = NULL, *prev = NULL, *node = queue;
	void* tmpdata = NULL;

	newnode = (struct queue*)malloc(sizeof(struct queue));	
	if(newnode == NULL)
	{
		err("no memory");
		m_unlock(&status.queuemutex, 11);
		return NULL;
	}

	tmpdata = malloc(len);
	if(tmpdata == NULL)
	{
		err("no memory");
		free(newnode);
		m_unlock(&status.queuemutex, 11);
		return NULL;
	}
	memset(newnode, 0, sizeof(struct queue));

	newnode->type = type;
	newnode->len = len;
	newnode->flag = flag;
	newnode->data = memcpy(tmpdata, data, len);

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
		queue = newnode;
	else
	{
		prev->next = newnode;
		newnode->prev = prev;
	}
	newnode->next = node;
	if(node != NULL) node->prev = queue;

	//debug(1000, "out");
	m_unlock(&status.queuemutex, 11);
	return newnode;
}

//flag: 0 = lock
//flag: 1 = no lock
void delqueue(struct queue* queuenode, int flag)
{
	debug(1000, "in");
	if(flag == 0) m_lock(&status.queuemutex, 11);
	struct queue *node = queue, *prev = queue;

	while(node != NULL)
	{
		if(node == queuenode)
		{
			if(node == queue)
			{
				queue = node->next;
				if(queue != NULL)
					queue->prev = NULL;
			}
			else
			{
				prev->next = node->next;
				if(node->next != NULL)
					node->next->prev = prev;
			}

			free(node->data);
			node->data = NULL;

			free(node);
			node = NULL;
			break;
		}

		prev = node;
		node = node->next;
	}
	if(flag == 0) m_unlock(&status.queuemutex, 11);
	debug(1000, "out");
}

struct queue* getqueue(int type)
{
	//debug(1000, "in");
	m_lock(&status.queuemutex, 11);
	struct queue *node = queue;

	while(node != NULL)
	{
		if(node->type == type)
		{
			//debug(1000, "out");
			m_unlock(&status.queuemutex, 11);
			return node;
		}

		node = node->next;
	}
	m_unlock(&status.queuemutex, 11);
	return NULL;
}

void freequeue()
{
	debug(1000, "in");
	m_lock(&status.queuemutex, 11);
	struct queue *node = queue, *prev = queue;

	while(node != NULL)
	{
		prev = node;
		node = node->next;
		if(prev != NULL)
			delqueue(prev, 1);
	}
	m_unlock(&status.queuemutex, 11);
	debug(1000, "out");
}

#endif

