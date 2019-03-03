#ifndef DVRDEV_H
#define DVRDEV_H

struct dvbdev* dvropen(struct dvbdev* fenode)
{
	int fd = -1;
	struct dvbdev* node = dvbdev;

	if(fenode == NULL) return NULL;
		
	while(node != NULL)
	{
		if(node->type == DVRDEV && node->adapter == fenode->adapter && node->devnr == fenode->devnr)
			break;
		node = node->next;
	}

	if(node != NULL)
	{
		debug(200, "open dvr %s", node->dev);
		if((fd = open(node->dev, O_WRONLY)) < 0)
		{
			debug(200, "open dvr failed %s", node->dev);
			node = NULL;
		}
		else
		{
			fcntl(fd, F_SETFL, fcntl(fd, F_GETFL) | O_NONBLOCK);
			node->fd = fd;
			closeonexec(fd);
		}
	}

	return node;
}

int dvropendirect(char *dvrdev)
{
	int fd = -1;
	
	debug(200, "open direct dvr %s", dvrdev);
	if((fd = open(dvrdev, O_WRONLY)) < 0)
	{
		debug(200, "open dvr failed %s", dvrdev);
	}

	closeonexec(fd);
	return fd;
}

void dvrclose(struct dvbdev* node, int fd)
{
	if(node != NULL)
	{
		close(node->fd);
		node->fd = -1;
	}
	else
		close(fd);
}

int dvrgetdev()
{
	int i, y, fd = -1, count = 0;
	char *buf = NULL, *dvrdev = NULL;

	dvrdev = getconfig("dvrdev", NULL);
	if(dvrdev == NULL)
	{
		err("NULL detect");
		return count;
	}

	buf = malloc(MINMALLOC);
	if(buf == NULL)
	{
		err("no memory");
		return count;
	}
	
	//workaround HD51 kann nur max 4 dvr ... warum??
	int help = MAXDVRDEV;
	if(checkrealbox("HD51") == 1 || checkrealbox("HD60") == 1 || checkrealbox("HD61") == 1)
		help = 4;
	//
	for(i = 0; i < MAXDVBADAPTER; i++)
	{
		//for(y = 0; y < MAXDVRDEV; y++)
		for(y = 0; y < help; y++)
		{
			sprintf(buf, dvrdev, i, y);
			fd = dvropendirect(buf);
			if(fd >= 0)
			{
				dvrclose(NULL, fd);
				count++;
				adddvbdev(buf, i, y, -1, DVRDEV, NULL, NULL, NULL, 0);
			}
		}
	}

	free(buf);
	return count;
}

#endif
