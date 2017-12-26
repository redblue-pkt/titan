#ifndef RC_H
#define RC_H

#ifdef ARM
#define SECINPUT
#endif

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
	struct rc *node = rc;

	while(node != NULL)
	{
		if(node->key == key && node->owner == owner)
			return node;

		node = node->next;
	}

	node = rc;

	while(node != NULL)
	{
		if(node->key == key && node->owner == NULL)
			return node;

		node = node->next;
	}
	return NULL;
}

int openrc()
{
	char *rcdev = NULL;
	int i = 0;

	status.fdrc = -1;
	status.fdrc1 = -1;

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
	
	rcdev = getconfig("rcfrontdev", NULL);
	if(rcdev != NULL)
	{
		i = 0;
		while(status.fdrc1 < 0 && i < 3)
		{
			status.fdrc1 = open(rcdev, O_RDWR);
			if(status.fdrc1 < 0) sleep(1);
			i++;
		}
		if(status.fdrc1 > -1)
			closeonexec(status.fdrc1);
	}

	return 0;
}

void closerc()
{
	if(status.fdrc != -1)
		close(status.fdrc);
	if(status.fdrc1 != -1)
		close(status.fdrc1);
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
	int ret = 0;
	struct input_event rcdata;
	struct timeval akttime;

	gettimeofday(&akttime, 0);
	sendtuxtxt(keycode);

	rcdata.time = akttime;
	rcdata.type = EV_KEY;
	rcdata.code = keycode;
	rcdata.value = 1;
	ret = write(status.fdrc, &rcdata, sizeof(rcdata));

#ifdef MIPSEL	
	rcdata.time = akttime;
	rcdata.type = 0;
	rcdata.code = 0;
	rcdata.value = 0;
	ret = write(status.fdrc, &rcdata, sizeof(rcdata));
#endif

	rcdata.time = akttime;
	rcdata.type = EV_KEY;
	rcdata.code = keycode;
	rcdata.value = 0;
	ret = write(status.fdrc, &rcdata, sizeof(rcdata));

#ifdef MIPSEL	
	rcdata.time = akttime;
	rcdata.type = 0;
	rcdata.code = 0;
	rcdata.value = 0;
	ret = write(status.fdrc, &rcdata, sizeof(rcdata));
#endif
	
	return ret;
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

void rcsignal(int flag)
{
	if((checkbox("DM900") == 1 || checkbox("DM520") == 1 || checkbox("DM525") == 1) && status.standby == 0)
	{
		int art = getconfigint("dm900frontrun", NULL);
		if(flag == 1)
		{
			if(art == 0)
			 	writesys("/proc/stb/fp/led1_pattern", "ffffffff", 1);
			else if(art == 1)
				writesys("/proc/stb/fp/led0_pattern", "ffffffff", 1);
			else if(art == 2)	
				writesys("/proc/stb/fp/led1_pattern", "ffffffff", 1);
		}
		if(flag == 0)
		{
			if(art == 0)
			 	writesys("/proc/stb/fp/led1_pattern", "00000000", 1);
			else if(art == 1)
				writesys("/proc/stb/fp/led0_pattern", "00000000", 1);
			else if(art == 2)	
				writesys("/proc/stb/fp/led1_pattern", "00000000", 1);
		}
	}
} 	

int waitrcext(struct skin* owner, unsigned int timeout, int screencalc, int filelistview)
{
	fd_set rfds;
	fd_set rfds1;
	struct timeval tv;
	struct input_event rcdata;
	int treffer = 0, rest = 0, j = 0, front = 0;
	int ret = 0, len = 0, fromthread = 0, longpress = 0, longpresscount = 0;
	struct rc *node;
	struct timeval aktrctime;
	
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
		front = 0;

		if(fromthread == 0) status.sec = 0;
		if((fromthread == 0 && status.rckey == 0) || fromthread == 1)
		{
			if(fromthread == 0) m_unlock(&status.waitrcmutex, 24);
#ifdef SECINPUT			
			if(status.fdrc1 > -1 && timeout > 1)
			{
				for(j = 0; j < timeout / 2; j++)
				{
					tv.tv_sec = 2;
					tv.tv_usec = 0;
					FD_ZERO(&rfds);
					FD_SET(status.fdrc, &rfds);
					ret = TEMP_FAILURE_RETRY(select(status.fdrc + 1, &rfds, NULL, NULL, &tv));
					if(ret == 0)
					{
						tv.tv_sec = 0;
						tv.tv_usec = 1000;
						FD_ZERO(&rfds1);
						FD_SET(status.fdrc1, &rfds1);
						ret = TEMP_FAILURE_RETRY(select(status.fdrc1 + 1, &rfds1, NULL, NULL, &tv));
						front = 1;
					}
					if(ret != 0)
						break;
				}
			}
			else
			{
#endif
				tv.tv_sec = timeout;
				tv.tv_usec = rest * 1000;
				FD_ZERO(&rfds);
				FD_SET(status.fdrc, &rfds);
				ret = TEMP_FAILURE_RETRY(select(status.fdrc + 1, &rfds, NULL, NULL, &tv));
#ifdef SECINPUT				
				if(ret == 0 && status.fdrc1 > -1)
				{
					tv.tv_sec = 0;
					tv.tv_usec = 1000;
					FD_ZERO(&rfds1);
					FD_SET(status.fdrc1, &rfds1);
					ret = TEMP_FAILURE_RETRY(select(status.fdrc1 + 1, &rfds1, NULL, NULL, &tv));
					front = 1;
				}
			}
#endif
				
			if(fromthread == 0) m_lock(&status.waitrcmutex, 24);

			if((status.rcowner != NULL && status.rcowner != owner) || (status.rcstandby != NULL && status.rcstandby != owner)) 
			{
				if(status.rcstandby == NULL || status.rcstandby != owner)
				{
					usleep(100000);
					continue;
				}
			}
		}
		if(fromthread == 0) status.sec = time(NULL);

		if((fromthread == 0 && ret > 0 && status.rckey == 0) || (fromthread == 1 && ret > 0))
		{
			if(front == 0 && FD_ISSET(status.fdrc, &rfds))
				len = TEMP_FAILURE_RETRY(read(status.fdrc, &rcdata, sizeof(struct input_event)));
			else if(front == 1 && FD_ISSET(status.fdrc1, &rfds1))
			{
				len = TEMP_FAILURE_RETRY(read(status.fdrc1, &rcdata, sizeof(struct input_event)));
				if(rcdata.code  == getrcconfigint("rcpower", NULL))
					rcdata.code =  getrcconfigint("rcfrontpower", NULL);
			}

			if(rcdata.type != EV_KEY)
			{
				usleep(10000);
				continue;
			}
			if(rcdata.value == 0) //release
			{
				rcsignal(0);
#ifdef DREAMBOX
				if(longpresscount > 0)
				{
					rcdata.code = longpress;
					if(longpresscount > 3) rcdata.code = longpress + 3000;
					ret = rcdata.code;	
					break;
				}
#endif
				usleep(10000);
				continue;
			}
			else
				rcsignal(1);

			if(islongkey(rcdata.code))
			{
				longpress = rcdata.code;
				longpresscount++;
				timeout = 0;
#ifdef DREAMBOX
				if(longpresscount == 1)
					rest = 700;
				else
					rest = 500;
#else
				rest = 500;
#endif								
				usleep(10000);
				continue;
			}
			
			gettimeofday(&aktrctime, NULL);
			if(status.rcskipms > 0 && ((aktrctime.tv_sec - rcdata.time.tv_sec) * 1000000LL + aktrctime.tv_usec - rcdata.time.tv_usec) > status.rcskipms)
			{
				usleep(10000);
				continue;
			}
			
//			printf(".\n");
			
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

	if(ret != RCTIMEOUT)
		status.lastrcaction = time(NULL);
	return ret;
}

int waitrc(struct skin* owner, unsigned int timeout, int screencalc)
{
	return waitrcext(owner, timeout, screencalc, 0);
}

struct rc* addrc(int key, void *func, struct skin* screen, struct skin *screennode)
{
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
				return node;
			}
			nodeaddr = &node->next;
			node = node->next;
		}
		*nodeaddr = newnode;
	}
	else
		rc = newnode;

	return newnode;
}

void delrc(int key, struct skin* owner, struct skin* screennode)
{
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
}

void delownerrc(struct skin* owner)
{
	struct rc *node = rc, *prev = rc;

	while(node != NULL)
	{
		prev = node;
		node = node->next;
		if(prev != NULL && prev->owner == owner)
			delrc(prev->key, prev->owner, prev->screennode);
	}
}

void freerc()
{
	struct rc *node = rc, *prev = rc;

	while(node != NULL)
	{
		prev = node;
		node = node->next;
		if(prev != NULL)
			delrc(prev->key, prev->owner, prev->screennode);
	}
}

#endif
