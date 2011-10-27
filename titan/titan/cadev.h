#ifndef CADEV_H
#define CADEV_H

struct dvbdev* caopen(int adapter)
{
	debug(1000, "in");
	int fd = -1;
	struct dvbdev* node = dvbdev;

	while(node != NULL)
	{
		if(node->fd == -1 && node->type == CADEV && node->adapter == adapter)
			break;
		node = node->next;
	}

	if(node != NULL)
	{
		if((fd = open(node->dev, O_RDWR | O_NONBLOCK)) < 0)
		{
			debug(200, "open ca failed %s", node->dev);
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

int caopendirect(char *cadev)
{
	debug(1000, "in");
	int fd = -1;

	if((fd = open(cadev, O_RDWR | O_NONBLOCK)) < 0)
	{
		debug(200, "open ca failed %s", cadev);
	}

	closeonexec(fd);
	debug(1000, "out");
	return fd;
}

void caclose(struct dvbdev* node, int fd)
{
	if(node != NULL)
	{
		close(node->fd);
		node->fd = -1;
	}
	else
		close(fd);
}

int cagetcaps(struct dvbdev* node, struct ca_caps* caps)
{
	if(node == NULL)
	{
		debug(1000, "out-> NULL detect");
		return 1;
	}

	//caps.slot_num
	//caps.slot_type
	//caps.descr_num
	//caps.descr_type

	debug(200, "CA_GET_CAP");
	if(ioctl(node->fd, CA_GET_CAP, caps) < 0)
	{
		perr("CA_GET_CAP");
		return 1;
	}

	return 0;
}

int cagetmsg(struct dvbdev* node, struct ca_msg *msg)
{
	if(node == NULL)
	{
		debug(1000, "out-> NULL detect");
		return 1;
	}

	debug(200, "CA_GET_MSG");
	if(ioctl(node->fd, CA_GET_MSG, msg) < 0)
	{
		perr("CA_GET_MSG");
		return 1;
	}

	return 0;
}

int casendmsg(struct dvbdev* node, struct ca_msg *msg)
{
	if(node == NULL)
	{
		debug(1000, "out-> NULL detect");
		return 1;
	}

	debug(200, "CA_SEND_MSG");
	if(ioctl(node->fd, CA_SEND_MSG, msg) < 0)
 	{
		perr("CA_SEND_MSG");
		return 1;
	}

	return 0;
}

int cagetdesc(struct dvbdev* node)
{
	if(node == NULL)
	{
		debug(1000, "out-> NULL detect");
		return 1;
	}

	debug(200, "CA_GET_DESCR_INFO");
	if(ioctl(node->fd, CA_GET_DESCR_INFO) < 0)
	{
		perr("CA_GET_DESCR_INFO");
		return 1;
	}

	return 0;
}

int casetpid(struct dvbdev* node)
{
	if(node == NULL)
	{
		debug(1000, "out-> NULL detect");
		return 1;
	}

	debug(200, "CA_SET_PID");
	if(ioctl(node->fd, CA_SET_PID) < 0)
	{
		perr("CA_SET_PID");
		return 1;
	}

	return 0;
}

int careset(struct dvbdev* node, int slot)
{
	if(node == NULL)
	{
		debug(1000, "out-> NULL detect");
		return 1;
	}

	debug(200, "CA_RESET");
	if(ioctl(node->fd, CA_RESET, slot) < 0)
	{
		perr("CA_RESET");
		return 1;
	}

	return 0;
}

int cagetslotinfo(struct dvbdev* node, ca_slot_info_t* info)
{
	if(node == NULL)
	{
		debug(1000, "out-> NULL detect");
		return 1;
	}

	//info.num
	//info.type
	//info.flags (1:no, 2:present, 3:ready)

	debug(200, "CA_GET_SLOT_INFO");
	if(ioctl(node->fd, CA_GET_SLOT_INFO, info) < 0)
	{
		perr("CA_GET_SLOT_INFO");
		return 1;
	}

	return 0;
}

int cagetdev()
{
	debug(1000, "in");
	int i, y, fd = -1, count = 0;
	char *buf = NULL, *cadev = NULL;

	cadev = getconfig("cadev", NULL);
	if(cadev == NULL)
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
		for(y = 0; y < MAXCADEV; y++)
		{
			sprintf(buf, cadev, i, y);
			fd = caopendirect(buf);
			if(fd >= 0)
			{
				caclose(NULL, fd);
				count++;
				adddvbdev(buf, i, y, -1, CADEV, NULL, NULL);
			}
		}
	}

	free(buf);
	debug(1000, "out");
	return count;
}

#endif
