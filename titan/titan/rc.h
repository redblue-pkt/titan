#ifndef RC_H
#define RC_H

int sendtuxtxt(int code)
{
	int rc;
    
	if((rc = open("/tmp/block.tmp", O_RDONLY)) >= 0)
	{
		close(rc);
        
		if(status.fdrctxt <= 0)
		{
			struct sockaddr_un vAddr;
			vAddr.sun_family = AF_UNIX;
			strcpy(vAddr.sun_path, "/tmp/rc.socket");
			status.fdrctxt = socket(PF_UNIX, SOCK_STREAM, 0);
        
			if(status.fdrctxt <= 0)
			{
				err("open tuxtxt rc socket");
				return 1;
			};

			if(connect(status.fdrctxt, (struct sockaddr *)&vAddr, sizeof(vAddr)) !=0)
			{
				close(status.fdrctxt);
				status.fdrctxt = -1;
				err("connect to tuxtxt rc socket");
				return 1;
			}
		}
        
		if(status.fdrctxt > 0)
		{
			char* tmpstr = malloc(8);
			if(tmpstr != NULL)
			{
				sprintf(tmpstr, "%08d", code);

				if(write(status.fdrctxt, tmpstr, 8) <= 0)
				{
					err("forwarding to rc socket");
					close(status.fdrctxt);
					status.fdrctxt = -1;
					free(tmpstr); tmpstr = NULL;
					return 1;
				}
			}
			free(tmpstr); tmpstr = NULL;
		}
		else
		{
			err("forwarding to rc socket");
		}
		return 0;
	}

	if(status.fdrctxt != -1)
	{
		close(status.fdrctxt);
		status.fdrctxt = -1;
	}
	return 0;
}

int maprc(int rccode, struct skin* owner)
{
	if(owner != NULL)
	{
		struct rcmap* rcmapnode = getrcmap(owner->name, rccode);
		if(rcmapnode != NULL)
			rccode = rcmapnode->newkey;
	}

	return rccode;
}

struct rc* getrc(int key, struct skin* owner)
{
	debug(1000, "in");
	struct rc *node = rc;

	while(node != NULL)
	{
		if(node->key == key && node->owner == owner)
		{
			debug(1000, "out");
			return node;
		}

		node = node->next;
	}

	node = rc;

	while(node != NULL)
	{
		if(node->key == key && node->owner == NULL)
		{
			debug(1000, "out");
			return node;
		}

		node = node->next;
	}
	return NULL;
}

int openrc()
{
	debug(1000, "in");
	char *rcdev = NULL;
	int i = 0;

	status.fdrc = -1;

	rcdev = getconfig("rcdev", NULL);
	if(rcdev != NULL)
	{
		while(status.fdrc < 0 && i < 3)
		{
			status.fdrc = open(rcdev, O_RDWR);
			if(status.fdrc < 0) sleep(1);
			i++;
		}

		if(status.fdrc < 0)
		{
			err("open input device %s", rcdev);
			return 1;
		}
		closeonexec(status.fdrc);
	}
	else
	{
		err("input device not found in config");
		return 1;
	}

	debug(1000, "out");
	return 0;
}

void closerc()
{
	debug(1000, "in");

	if(status.fdrc != -1)
		close(status.fdrc);

	debug(1000, "out");
}

int islongkey(int keycode)
{
	int i = 0;

	for(i = 0; i < MAXLONGKEY; i++)
	{
		if(status.longkeycode[i] == keycode)
			return 1;
	}
	return 0;
}

int writerc(int keycode)
{
	struct input_event rcdata;
	struct timeval akttime;

	gettimeofday(&akttime, 0);
	sendtuxtxt(keycode);

	rcdata.time = akttime;
	rcdata.type = EV_KEY;
	rcdata.code = keycode;
	rcdata.value = 1;
	write(status.fdrc, &rcdata, sizeof(rcdata));

	rcdata.time = akttime;
	rcdata.type = EV_KEY;
	rcdata.code = keycode;
	rcdata.value = 0;
	
	return  write(status.fdrc, &rcdata, sizeof(rcdata));
}

int flushrc(unsigned int timeout)
{
	fd_set rfds;
	struct timeval tv;
	struct input_event rcdata;
	int rest = 0, count = 0;
	int ret = 0, len = 0;

	rest = timeout % 1000;
	timeout = (timeout - rest) / 1000;

	while(count < 10)
	{
		count++;
		tv.tv_sec = timeout;
		tv.tv_usec = rest * 1000;
		FD_ZERO(&rfds);
		FD_SET(status.fdrc, &rfds);

		ret = TEMP_FAILURE_RETRY(select(status.fdrc + 1, &rfds, NULL, NULL, &tv));

		if(ret < 1) break;
		if(FD_ISSET(status.fdrc, &rfds))
		{
			count = 0;
			len = TEMP_FAILURE_RETRY(read(status.fdrc, &rcdata, sizeof(struct input_event)));
		}
	}

	return 0;
}

int waitrcext(struct skin* owner, unsigned int timeout, int screencalc, int filelistview)
{
	debug(1000, "in");
	fd_set rfds;
	struct timeval tv;
	struct input_event rcdata;
	int treffer = 0, rest = 0;
	int ret = 0, len = 0, fromthread = 0, longpress = 0, longpresscount = 0;
	struct rc *node;

	if(pthread_self() != status.mainthread)
		fromthread = 1;

	rest = timeout % 1000;
	timeout = (timeout - rest) / 1000;

	if(timeout == 0 && rest == 0) timeout = 0x7fffffff;

	while(treffer == 0)
	{
		tv.tv_sec = timeout;
		tv.tv_usec = rest * 1000;
		FD_ZERO(&rfds);
		FD_SET(status.fdrc, &rfds);

		if(fromthread == 0) status.sec = 0;
		if((fromthread == 0 && status.rckey == 0) || fromthread == 1)
		{
			ret = TEMP_FAILURE_RETRY(select(status.fdrc + 1, &rfds, NULL, NULL, &tv));
			if(status.rcowner != NULL && status.rcowner != owner)
			{
				usleep(100000);
				continue;
			}
		}
		if(fromthread == 0) status.sec = time(NULL);

		if((fromthread == 0 && ret > 0 && status.rckey == 0) || (fromthread == 1 && ret > 0))
		{
			if(FD_ISSET(status.fdrc, &rfds))
				len = TEMP_FAILURE_RETRY(read(status.fdrc, &rcdata, sizeof(struct input_event)));

			if(rcdata.type != EV_KEY)
			{
				usleep(10000);
				continue;
			}
			if(rcdata.value == 0) //release
			{
				usleep(10000);
				continue;
			}

			if(islongkey(rcdata.code))
			{
				longpress = rcdata.code;
				longpresscount++;
				timeout = 0;
				rest = 500;
				usleep(10000);
				continue;
			}

			if(time(NULL) - rcdata.time.tv_sec > 1)
			{
				usleep(10000);
				continue;
			}

			debug(100, "key code=%d, type=%d, value=%d", rcdata.code, rcdata.type, rcdata.value);
			treffer = 1;
			rcdata.code = maprc(rcdata.code, owner);

			node = getrc(rcdata.code, owner);
			if(node != NULL && node->rcfunc != NULL) 
			{
				if(fromthread == 0)
 					node->rcfunc(node->screen, node->screennode, screencalc, filelistview, 0);
				else
 					node->rcfunc(node->screen, node->screennode, screencalc, filelistview, 2);
			}
			
			ret = rcdata.code;
#ifdef SIMULATE
			if(ret == 16) treffer = 0;
#endif
		}
		else if((fromthread == 0 && ret == 0 && status.rckey == 0) || (fromthread == 1 && ret == 0)) //timeout
		{
			rcdata.code = 0;
			if(longpress > 0)
			{
				rcdata.code = longpress;
				if(longpresscount > 5) rcdata.code = longpress + 3000;
			}
			treffer = 1;
			rcdata.code = maprc(rcdata.code, owner);

			node = getrc(rcdata.code, NULL);
			if(node != NULL && node->rcfunc != NULL) 
			{
				if(fromthread == 0)
 					node->rcfunc(node->screen, node->screennode, screencalc, filelistview, 0);
				else
 					node->rcfunc(node->screen, node->screennode, screencalc, filelistview, 2);
			}
			if(longpress > 0)
				ret = rcdata.code;
			else
				ret = RCTIMEOUT;
		}
		else if(fromthread == 0 && status.rckey != 0)
		{
			treffer = 1;
			rcdata.code = maprc(rcdata.code, owner);

			node = getrc(status.rckey, owner);
			if(node != NULL && node->rcfunc != NULL) 
			{
				if(fromthread == 0)
 					node->rcfunc(node->screen, node->screennode, screencalc, filelistview, 0);
				else
 					node->rcfunc(node->screen, node->screennode, screencalc, filelistview, 2);
			}
			ret = status.rckey;
			status.rckey = 0;
		}
		else
		{ 
			treffer = 1;
			perr("select failed");
		}
	}

	debug(1000, "out");
	status.lastrcaction = time(NULL);
	return ret;
}

int waitrc(struct skin* owner, unsigned int timeout, int screencalc)
{
	return waitrc(owner, timeout, screencalc, 0);
}

struct rc* addrc(int key, void *func, struct skin* screen, struct skin *screennode)
{
	debug(1000, "in");
	struct rc *newnode, *node = rc;
	struct rc **nodeaddr;

	newnode = (struct rc*)malloc(sizeof(struct rc));	
	if(newnode == NULL)
	{
		err("no memory");
		return NULL;
	}

	memset(newnode, 0, sizeof(struct rc));
	newnode->key = key;
	newnode->rcfunc = func;
	newnode->screen = screen;
	newnode->screennode = screennode;
	newnode->owner = screen;

	if(node != NULL)
	{
		while(node != NULL)
		{
			if(node->key == key && node->owner == screen)
			{
				node->rcfunc = func;
				node->screen = screen;
				node->screennode = screennode;
				free(newnode);
				newnode = NULL;
				debug(1000, "out -> key %d found, use change instread add", key);
				return node;
			}
			nodeaddr = &node->next;
			node = node->next;
		}
		*nodeaddr = newnode;
	}
	else
		rc = newnode;

	debug(1000, "out");
	return newnode;
}

void delrc(int key, struct skin* owner, struct skin* screennode)
{
	debug(1000, "in");
	struct rc *node = rc, *prev = rc;

	while(node != NULL)
	{
		if(node->key == key && node->owner == owner && node->screennode == screennode)
		{
			if(node == rc)
				rc = node->next;
			else
				prev->next = node->next;

			free(node);
			node = NULL;
			break;
		}

		prev = node;
		node = node->next;
	}
	debug(1000, "out");
}

void delownerrc(struct skin* owner)
{
	debug(1000, "in");
	struct rc *node = rc, *prev = rc;

	while(node != NULL)
	{
		prev = node;
		node = node->next;
		if(prev != NULL && prev->owner == owner)
			delrc(prev->key, prev->owner, prev->screennode);
	}
	debug(1000, "out");
}

void freerc()
{
	debug(1000, "in");
	struct rc *node = rc, *prev = rc;

	while(node != NULL)
	{
		prev = node;
		node = node->next;
		if(prev != NULL)
			delrc(prev->key, prev->owner, prev->screennode);
	}
	debug(1000, "out");
}

#endif
