#ifndef CIDEV_H
#define CIDEV_H

struct dvbdev* ciopen(struct dvbdev* node)
{
	int fd = -1;

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

	return node;
}

int ciopendirect(char *cidev)
{
	int fd = -1;

	if((fd = open(cidev, O_RDWR | O_NONBLOCK)) < 0)
	{
		debug(200, "open ci failed %s", cidev);
	}

	closeonexec(fd);
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
	int i = 0, y = 0, fd = -1, count = 0;
	char *buf = NULL, *cidev = NULL;

	cidev = getconfig("cidev", NULL);
	if(cidev == NULL)
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

#ifdef MIPSEL
	for(y = 0; y < MAXCIDEV; y++)
	{
		sprintf(buf, cidev, y);
		fd = ciopendirect(buf);
		if(fd >= 0)
		{
			count++;
			adddvbdev(buf, i, y, fd, CIDEV, NULL, NULL, 0);
		}
	}
#else
	for(i = 0; i < MAXDVBADAPTER; i++)
	{
		for(y = 0; y < MAXCIDEV; y++)
		{
			sprintf(buf, cidev, i, y);
			fd = ciopendirect(buf);
			if(fd >= 0)
			{
				count++;
				adddvbdev(buf, i, y, fd, CIDEV, NULL, NULL, 0);
			}
		}
	}
#endif

	free(buf);
	return count;
}

#endif
