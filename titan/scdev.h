#ifndef SCDEV_H
#define SCDEV_H

#ifndef IOCTL_SET_RESET
#define IOCTL_SET_RESET	_IOW('s', 1, uint32_t)
#endif

#ifndef IOCTL_SET_MODES
#define IOCTL_SET_MODES	_IOW('s', 2, scimodes)
#endif

#ifndef IOCTL_GET_MODES
#define IOCTL_GET_MODES	_IOW('s', 3, scimodes)
#endif

#ifndef IOCTL_SET_PARAMETERS
#define IOCTL_SET_PARAMETERS	_IOW('s', 4, sciparameters)
#endif

#ifndef IOCTL_GET_PARAMETERS
#define IOCTL_GET_PARAMETERS	_IOW('s', 5, sciparameters)
#endif

#ifndef IOCTL_SET_CLOCK_START
#define IOCTL_SET_CLOCK_START	_IOW('s', 6, uint32_t)
#endif

#ifndef IOCTL_SET_CLOCK_STOP
#define IOCTL_SET_CLOCK_STOP	_IOW('s', 7, uint32_t)
#endif

#ifndef IOCTL_GET_IS_CARD_PRESENT
#define IOCTL_GET_IS_CARD_PRESENT	_IOW('s', 8, uint32_t)
#endif

#ifndef IOCTL_GET_IS_CARD_ACTIVATED
#define IOCTL_GET_IS_CARD_ACTIVATED	_IOW('s', 9, uint32_t)
#endif

#ifndef IOCTL_SET_DEACTIVATE
#define IOCTL_SET_DEACTIVATE	_IOW('s', 10, uint32_t)
#endif

#ifndef IOCTL_SET_ATR_READY
#define IOCTL_SET_ATR_READY	_IOW('s', 11, uint32_t)
#endif

#ifndef IOCTL_GET_ATR_STATUS
#define IOCTL_GET_ATR_STATUS	_IOW('s', 12, uint32_t)
#endif

#ifndef IOCTL_DUMP_REGS
#define IOCTL_DUMP_REGS	_IOW('s', 20, uint32_t)
#endif

#ifndef IOCTL_SET_CLOCK
#define IOCTL_SET_CLOCK	_IOW('s', 13, uint32_t)
#endif

#ifndef IOCTL_SET_ONLY_RESET
#define IOCTL_SET_ONLY_RESET	_IOW('s', 100, uint32_t)
#endif

struct dvbdev* scopen()
{
	int fd = -1;
	struct dvbdev* node = dvbdev;

	while(node != NULL)
	{
		if(node->fd == -1 && node->type == SCDEV)
			break;
		node = node->next;
	}

	if(node != NULL)
	{
		if((fd = open(node->dev, O_RDWR)) < 0)
		{
			debug(200, "open sc failed %s", node->dev);
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

int scopendirect(char *scdev)
{
	int fd = -1;

	if((fd = open(scdev, O_RDWR)) < 0)
	{
		debug(200, "open sc failed %s", scdev);
	}

	closeonexec(fd);
	return fd;
}

void scclose(struct dvbdev* node, int fd)
{
	if(node != NULL)
	{
		close(node->fd);
		node->fd = -1;
	}
	else
		close(fd);
}

int scsetreset(struct dvbdev* node)
{
	if(node == NULL)
	{
		err("NULL detect");
		return 1;
	}

	debug(200, "IOCTL_SET_RESET");
	if(ioctl(node->fd, IOCTL_SET_RESET) < 0)
	{
		perr("IOCTL_SET_RESET");
		return 1;
	}

	return 0;
}

int scsetmodes(struct dvbdev* node, struct sci_modes* modes)
{
	if(node == NULL || modes == NULL)
	{
		err("NULL detect");
		return 1;
	}

	debug(200, "IOCTL_SET_MODES");
	if(ioctl(node->fd, IOCTL_SET_MODES, modes) < 0)
	{
		perr("IOCTL_SET_MODES");
		return 1;
	}

	return 0;
}

int scgetmodes(struct dvbdev* node, struct sci_modes* modes)
{
	if(node == NULL || modes == NULL)
	{
		err("NULL detect");
		return 1;
	}

	debug(200, "IOCTL_GET_MODES");
	if(ioctl(node->fd, IOCTL_GET_MODES, modes) < 0)
	{
		perr("IOCTL_GET_MODES");
		return 1;
	}

	return 0;
}

int scsetparameters(struct dvbdev* node, struct sci_parameters* parameters)
{
	if(node == NULL || parameters == NULL)
	{
		err("NULL detect");
		return 1;
	}

	debug(200, "IOCTL_SET_PARAMETERS");
	if(ioctl(node->fd, IOCTL_SET_PARAMETERS, parameters) < 0)
	{
		perr("IOCTL_SET_PARAMETERS");
		return 1;
	}

	return 0;
}

int scgetparameters(struct dvbdev* node, struct sci_parameters* parameters)
{
	if(node == NULL || parameters == NULL)
	{
		err("NULL detect");
		return 1;
	}

	debug(200, "IOCTL_GET_PARAMETERS");
	if(ioctl(node->fd, IOCTL_GET_PARAMETERS, parameters) < 0)
	{
		perr("IOCTL_GET_PARAMETERS");
		return 1;
	}

	return 0;
}

int scsetclockstart(struct dvbdev* node)
{
	if(node == NULL)
	{
		err("NULL detect");
		return 1;
	}

	debug(200, "IOCTL_SET_CLOCK_START");
	if(ioctl(node->fd, IOCTL_SET_CLOCK_START) < 0)
	{
		perr("IOCTL_SET_CLOCK_START");
		return 1;
	}

	return 0;
}

int scsetclockstop(struct dvbdev* node)
{
	if(node == NULL)
	{
		err("NULL detect");
		return 1;
	}

	debug(200, "IOCTL_SET_CLOCK_STOP");
	if(ioctl(node->fd, IOCTL_SET_CLOCK_STOP) < 0)
	{
		perr("IOCTL_SET_CLOCK_STOP");
		return 1;
	}

	return 0;
}

int scgetiscardpresent(struct dvbdev* node, uint32_t* status)
{
	if(node == NULL)
	{
		err("NULL detect");
		return 1;
	}

	debug(200, "IOCTL_GET_IS_CARD_PRESENT");
	if(ioctl(node->fd, IOCTL_GET_IS_CARD_PRESENT, status) < 0)
	{
		perr("IOCTL_GET_IS_CARD_PRESENT");
		return 1;
	}

	return 0;
}

int scgetiscardactivated(struct dvbdev* node, uint32_t* status)
{
	if(node == NULL)
	{
		err("NULL detect");
		return 1;
	}

	debug(200, "IOCTL_GET_IS_CARD_ACTIVATED");
	if(ioctl(node->fd, IOCTL_GET_IS_CARD_ACTIVATED, status) < 0)
	{
		perr("IOCTL_GET_IS_CARD_ACTIVATED");
		return 1;
	}

	return 0;
}

int scsetdeactivate(struct dvbdev* node)
{
	if(node == NULL)
	{
		err("NULL detect");
		return 1;
	}

	debug(200, "IOCTL_SET_DEACTIVATE");
	if(ioctl(node->fd, IOCTL_SET_DEACTIVATE) < 0)
	{
		perr("IOCTL_SET_DEACTIVATE");
		return 1;
	}

	return 0;
}

int scsetatrready(struct dvbdev* node)
{
	if(node == NULL)
	{
		err("NULL detect");
		return 1;
	}

	debug(200, "IOCTL_SET_ATR_READY");
	if(ioctl(node->fd, IOCTL_SET_ATR_READY) < 0)
	{
		perr("IOCTL_SET_ATR_READY");
		return 1;
	}

	return 0;
}

int scgetatrstatus(struct dvbdev* node, uint32_t* status)
{
	if(node == NULL)
	{
		err("NULL detect");
		return 1;
	}

	debug(200, "IOCTL_GET_ATR_STATUS");
	if(ioctl(node->fd, IOCTL_GET_ATR_STATUS, status) < 0)
	{
		perr("IOCTL_GET_ATR_STATUS");
		return 1;
	}

	return 0;
}

int scsetclock(struct dvbdev* node, uint32_t* clock)
{
	if(node == NULL)
	{
		err("NULL detect");
		return 1;
	}

	debug(200, "IOCTL_SET_CLOCK");
	if(ioctl(node->fd, IOCTL_SET_CLOCK, clock) < 0)
	{
		perr("IOCTL_SET_CLOCK");
		return 1;
	}

	return 0;
}

int scsetonlyreset(struct dvbdev* node)
{
	if(node == NULL)
	{
		err("NULL detect");
		return 1;
	}

	debug(200, "IOCTL_SET_ONLY_RESET");
	if(ioctl(node->fd, IOCTL_SET_ONLY_RESET) < 0)
	{
		perr("IOCTL_SET_ONLY_RESET");
		return 1;
	}

	return 0;
}

int scgetdev()
{
	int y, fd = -1, count = 0;
	char *buf = NULL, *scdev = NULL;

	scdev = getconfig("scdev", NULL);
	if(scdev == NULL)
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

	for(y = 0; y < MAXSCDEV; y++)
	{
		sprintf(buf, scdev, y);
		fd = scopendirect(buf);
		if(fd >= 0)
		{
			scclose(NULL, fd);
			count++;
			adddvbdev(buf, 0, y, -1, SCDEV, NULL, NULL, 0);
		}
	}

	free(buf);
	return count;
}

#endif
