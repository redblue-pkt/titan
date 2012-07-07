#ifndef TIMERTHREAD_H
#define TIMERTHREAD_H

struct stimerthread* gettimer(struct stimerthread* timernode)
{
	//debug(1000, "in");
	struct stimerthread *node = stimerthread;

	while(node != NULL)
	{
		if(node == timernode)
		{
			//debug(1000, "out");
			return node;
		}

		node = node->next;
	}
	debug(100, "timerthread not found (timer=%p)", timernode);
	return NULL;
}

struct stimerthread* addtimer(void* func, int aktion, int delay, int count, void* param1, void* param2, struct stimerthread* last)
{
	debug(1000, "in");
	struct stimerthread *newnode = NULL, *prev = NULL, *node = NULL;

	newnode = (struct stimerthread*)malloc(sizeof(struct stimerthread));	
	if(newnode == NULL)
	{
		err("no memory");
		return NULL;
	}

	memset(newnode, 0, sizeof(struct stimerthread));

	newnode->func = func;
	newnode->aktion = aktion;
	newnode->delay = delay;
	newnode->count = count;
	newnode->param1 = param1;
	newnode->param2 = param2;

	m_lock(&status.timerthreadmutex, 6);
	node = stimerthread;
  
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
		stimerthread = newnode;
	else
		prev->next = newnode;
	newnode->next = node;

	m_unlock(&status.timerthreadmutex, 6);
	debug(1000, "out");
	return newnode;
}

void deltimer(struct stimerthread *tnode)
{
	debug(1000, "in");
	int i = 0;
	void* threadstatus;

	m_lock(&status.timerthreadmutex, 6);
	struct stimerthread *node = stimerthread, *prev = stimerthread;

	while(node != NULL)
	{
		if(node == tnode)
		{
			//stop timer sub thread
			node->aktion = STOP;
			while(node->status != DEACTIVE)
			{
				usleep(100000);
				i++; if(i > 20) break;
			}

			if(i > 20)
			{
				err("detect hanging timer sub thread");
			}
			else if(node->count < 0 && node->thread != '\0')
				pthread_join(node->thread, &threadstatus);
			pthread_attr_destroy(&node->attr);

			if(node == stimerthread)
				stimerthread = node->next;
			else
				prev->next = node->next;

			free(node);
			node = NULL;
			break;
		}

		prev = node;
		node = node->next;
	}

	m_unlock(&status.timerthreadmutex, 6);
	debug(1000, "out");
}

//flag 0 = stop all
//flag 1 = stop only threads with thread->flag bit 0 = 1
void freetimer(int flag)
{
	debug(1000, "in");
	struct stimerthread *node = stimerthread, *prev = stimerthread;

	while(node != NULL)
	{
		prev = node;
		node = node->next;
		if(prev != NULL)
		{
			if(flag == 0)
				deltimer(prev);
			else if(flag == 1 && checkbit(prev->flag, 0) == 1)
				deltimer(prev);
		}
	}
	debug(1000, "out");
}

void* timerthreadsubfunc(void *param)
{
	debug(1000, "in");
	int count = 0;
	struct stimerthread* node = (struct stimerthread*)param;

	if(param == NULL)
	{
		debug(1000, "out -> NULL detect");
		pthread_exit(NULL);
	}

	node->status = ACTIVE;
	while(node->aktion != STOP)
	{
		if(node->aktion != PAUSE)
		{
			node->status = ACTIVE;
			node->func(node, node->param1, node->param2);
			node->notfirst = 1;
			if(node->count > -1 && node->aktion != PAUSE) node->count--;
			if(node->count == 0) break;
		}
		else
			node->status = INPAUSE;

		if(node->delay > 1000)
		{
			count = node->delay;
			while(count > 1000)
			{
				usleep(1000000);
				count = count - 1000;
				if(node->aktion == STOP || node->aktion == PAUSE) count = 0;
			}
			usleep(count);
		}
		else
			usleep(node->delay * 1000);
	}
	node->status = DEACTIVE;

	if(node->count > -1) deltimer(node);

	debug(1000, "out");
	pthread_exit(NULL);
}

void* timerthreadfunc(void *param)
{
	debug(1000, "in");
	int ret = 0;
	struct stimerthread* node = stimerthread;

	status.timerthreadstatus = ACTIVE;
	while(status.timerthreadaktion != STOP)
	{
		if(status.timerthreadaktion != PAUSE)
		{
			status.timerthreadstatus = ACTIVE;
			node = stimerthread;
			while(node != NULL)
			{
				if(node->status == DEACTIVE && node->aktion == START)
				{
					pthread_attr_init(&node->attr);
					pthread_attr_setstacksize(&node->attr, 50000);
					pthread_attr_setdetachstate(&node->attr, PTHREAD_CREATE_JOINABLE);
					node->status = ACTIVE;
					debug(10, "start timer sub thread");
					ret = pthread_create(&node->thread, &node->attr, timerthreadsubfunc, (void*)node);
					if(ret)
					{
						node->status = ERROR;
						perr("create timer sub thread");
						pthread_attr_destroy(&node->attr);
					}
					node = stimerthread;
				}
				node = node->next;
			}

		}
		else
			status.timerthreadstatus = INPAUSE;

		usleep(1 * 1000000);
	}
	status.timerthreadstatus = DEACTIVE;

	debug(1000, "out");
	pthread_exit(NULL);
}

#endif
