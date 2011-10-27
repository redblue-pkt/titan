#ifndef DVRDEV_H
#define DVRDEV_H

struct dvbdev* dvropen(struct dvbdev* fenode)
{
	debug(1000, "in");
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

	debug(1000, "out");
	return node;
}

int dvropendirect(char *dvrdev)
{
	debug(1000, "in");
	int fd = -1;

	if((fd = open(dvrdev, O_WRONLY)) < 0)
	{
		debug(200, "open dvr failed %s", dvrdev);
	}

	closeonexec(fd);
	debug(1000, "out");
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
	debug(1000, "in");
	int i, y, fd = -1, count = 0;
	char *buf = NULL, *dvrdev = NULL;

	dvrdev = getconfig("dvrdev", NULL);
	if(dvrdev == NULL)
	{
		debug(1000, "out -> NULL detect");
		return count;
	}

	buf = malloc(MINMALLOC);
	if(buf == NULL)
	{
		err("no memory");
		return count;
	}

	for(i = 0; i < MAXDVBADAPTER; i++)
	{
		for(y = 0; y < MAXDVRDEV; y++)
		{
			sprintf(buf, dvrdev, i, y);
			fd = dvropendirect(buf);
			if(fd >= 0)
			{
				dvrclose(NULL, fd);
				count++;
				adddvbdev(buf, i, y, -1, DVRDEV, NULL, NULL);
			}
		}
	}

	free(buf);
	debug(1000, "out");
	return count;
}

#endif
