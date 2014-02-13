#ifndef CADEV_H
#define CADEV_H

struct dvbdev* caopen(int adapter)
{
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
			debug(201, "open ca failed %s", node->dev);
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

int caopendirect(char *cadev)
{
	int fd = -1;

	if((fd = open(cadev, O_RDWR | O_NONBLOCK)) < 0)
	{
		debug(201, "open ca failed %s", cadev);
	}

	closeonexec(fd);
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
		err("NULL detect");
		return 1;
	}

	//caps.slot_num
	//caps.slot_type
	//caps.descr_num
	//caps.descr_type

	debug(201, "CA_GET_CAP");
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
		err("NULL detect");
		return 1;
	}

	debug(201, "CA_GET_MSG");
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
		err("NULL detect");
		return 1;
	}

	debug(201, "CA_SEND_MSG");
	if(ioctl(node->fd, CA_SEND_MSG, msg) < 0)
 	{
		perr("CA_SEND_MSG");
		return 1;
	}

	return 0;
}

int cagetdesc(struct dvbdev* node, struct ca_descr *descr)
{
	if(node == NULL)
	{
		err("NULL detect");
		return 1;
	}

	debug(201, "CA_GET_DESCR_INFO");
	if(ioctl(node->fd, CA_GET_DESCR_INFO, descr) < 0)
	{
		perr("CA_GET_DESCR_INFO");
		return 1;
	}

	return 0;
}

int casetdesc(struct dvbdev* node, struct ca_descr *descr)
{
	if(node == NULL)
	{
		err("NULL detect");
		return 1;
	}

	debug(201, "CA_SET_DESCR");
	if(ioctl(node->fd, CA_SET_DESCR, descr) < 0)
	{
		perr("CA_SET_DESCR");
		return 1;
	}

	return 0;
}

int casetpid(struct dvbdev* node, struct ca_pid *pid)
{
	if(node == NULL)
	{
		err("NULL detect");
		return 1;
	}

	debug(201, "CA_SET_PID");
	if(ioctl(node->fd, CA_SET_PID, pid) < 0)
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
		err("NULL detect");
		return 1;
	}

	debug(201, "CA_RESET");
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
		err("NULL detect");
		return 1;
	}

	//info.num
	//info.type
	//info.flags (1:no, 2:present, 3:ready)

	debug(201, "CA_GET_SLOT_INFO");
	if(ioctl(node->fd, CA_GET_SLOT_INFO, info) < 0)
	{
		perr("CA_GET_SLOT_INFO");
		return 1;
	}

	return 0;
}

//workaround for atemio500 and orf1 with unicam and orf card
//without this artefakte
void cainit(int fd)
{
	ca_descr_t cadescr;
	unsigned char buf[8];

	if(fd < 0) return;

	memset(buf, 1, 8);
	memset(&cadescr, 0, sizeof(cadescr));

	cadescr.index = 0;
	cadescr.parity = 0;
	memcpy(cadescr.cw, buf, 8);

	ioctl(fd, CA_SET_DESCR, &cadescr);

	cadescr.parity = 1;
	ioctl(fd, CA_SET_DESCR, &cadescr);
}

int cagetdev()
{
	int i, y, fd = -1, count = 0;
	char *buf = NULL, *cadev = NULL;

	cadev = getconfig("cadev", NULL);
	if(cadev == NULL)
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

	for(i = 0; i < MAXDVBADAPTER; i++)
	{
		for(y = 0; y < MAXCADEV; y++)
		{
			sprintf(buf, cadev, i, y);
			fd = caopendirect(buf);
			if(fd >= 0)
			{
				cainit(fd);
				caclose(NULL, fd);
				count++;
				adddvbdev(buf, i, y, -1, CADEV, NULL, NULL, 0);
			}
		}
	}

	free(buf);
	return count;
}

#endif
