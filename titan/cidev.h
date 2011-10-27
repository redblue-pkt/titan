#ifndef CIDEV_H
#define CIDEV_H

struct dvbdev* ciopen(int adapter)
{
	debug(1000, "in");
	int fd = -1;
	struct dvbdev* node = dvbdev;

	while(node != NULL)
	{
		if(node->fd == -1 && node->type == CIDEV && node->adapter == adapter)
			break;
		node = node->next;
	}

	if(node != NULL)
	{
		if((fd = open(node->dev, O_RDWR | O_NONBLOCK)) < 0)
		{
			debug(200, "open ci failed %s", node->dev);
			node = NULL;
		}
		else
		{
			node->fd = fd;
			closeonexec(fd);
		}
	}

	debug(1000, "out");
	return node;
}

int ciopendirect(char *cidev)
{
	debug(1000, "in");
	int fd = -1;

	if((fd = open(cidev, O_RDWR | O_NONBLOCK)) < 0)
	{
		debug(200, "open ci failed %s", cidev);
	}

	closeonexec(fd);
	debug(1000, "out");
	return fd;
}

void ciclose(struct dvbdev* node, int fd)
{
	if(node != NULL)
	{
		close(node->fd);
		node->fd = -1;
	}
	else
		close(fd);
}

int cigetdev()
{
	debug(1000, "in");
	int i, y, fd = -1, count = 0;
	char *buf = NULL, *cidev = NULL;

	cidev = getconfig("cidev", NULL);
	if(cidev == NULL)
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
		for(y = 0; y < MAXCIDEV; y++)
		{
			sprintf(buf, cidev, i, y);
			fd = ciopendirect(buf);
			if(fd >= 0)
			{
				count++;
				adddvbdev(buf, i, y, fd, CIDEV, NULL, NULL);
			}
		}
	}

	free(buf);
	debug(1000, "out");
	return count;
}

#endif
