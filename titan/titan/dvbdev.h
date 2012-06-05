#ifndef DVBDEV_H
#define DVBDEV

struct dvbdev* adddvbdev(char *dev, int adapter, int devnr, int fd, int type, struct dvb_frontend_info* feinfo, struct dvbdev* last)
{
	debug(1000, "in");
	struct dvbdev *newnode = NULL, *node = dvbdev;
	char* tmp = NULL, *tmp1 = NULL;

	newnode = (struct dvbdev*)malloc(sizeof(struct dvbdev));	
	if(newnode == NULL)
	{
		err("no mem");
		return NULL;
	}

	memset(newnode, 0, sizeof(struct dvbdev));

	newnode->dev = ostrcat(dev, NULL, 0, 0);
	newnode->fd = -1;
	newnode->type = type;
	newnode->feinfo = feinfo;
	newnode->adapter = adapter;
	newnode->devnr = devnr;

	if(type == CIDEV)
	{
		newnode->fd = fd;
		newnode->caslot = (struct caslot*)malloc(sizeof(struct caslot));	
		if(newnode->caslot == NULL)
		{
			err("no mem");
		}
		else
			memset(newnode->caslot, 0, sizeof(struct caslot));
	}

	if(type == FRONTENDDEV || type == FRONTENDDEVDUMMY)
	{
		newnode->fd = fd;
		tmp1 = oitoa(adapter);
		tmp = ostrcat("fe_", tmp1, 0, 1);
		tmp1 = oitoa(devnr);
		tmp = ostrcat(tmp, tmp1, 1, 1);
		newnode->feshortname = tmp;
		if(type == FRONTENDDEV)
		{
			switch(devnr)
			{
				case 0: newnode->fedmxsource = DMX_SOURCE_FRONT0; break;
				case 1: newnode->fedmxsource = DMX_SOURCE_FRONT1; break;
				case 2: newnode->fedmxsource = DMX_SOURCE_FRONT2; break;
				case 3: newnode->fedmxsource = DMX_SOURCE_FRONT3; break;
			}
		}
		if(type == FRONTENDDEVDUMMY)
		{
			switch(devnr)
			{
				case 0: newnode->fedmxsource = DMX_SOURCE_DVR0; break;
				case 1: newnode->fedmxsource = DMX_SOURCE_DVR1; break;
				case 2: newnode->fedmxsource = DMX_SOURCE_DVR2; break;
				case 3: newnode->fedmxsource = DMX_SOURCE_DVR3; break;
			}
		}
	}

	if(node != NULL)
	{
		if(last == NULL)
		{
			while(node->next != NULL)
				node = node->next;
			node->next = newnode;
		}
		else
			last->next = newnode;
	}
	else
		dvbdev = newnode;

	debug(1000, "out");
	return newnode;
}

void deldvbdev(char *dev)
{
	debug(1000, "in");
	struct dvbdev *node = dvbdev, *prev = dvbdev;

	while(node != NULL)
	{
		if(ostrcmp(node->dev, dev) == 0)
		{
			if(node == dvbdev)
				dvbdev = node->next;
			else
				prev->next = node->next;

			close(node->fd);

			free(node->dev);
			node->dev = NULL;

			free(node->feinfo);
			node->feinfo = NULL;

			free(node->feshortname);
			node->feshortname = NULL;

			free(node->feaktnr);
			node->feaktnr = NULL;
			
			free(node->caslot);
			node->caslot = NULL;

			free(node);
			node = NULL;
			break;
		}

		prev = node;
		node = node->next;
	}
	debug(1000, "out");
}

void freedvbdev()
{
	debug(1000, "in");
	struct dvbdev *node = dvbdev, *prev = dvbdev;

	while(node != NULL)
	{
		prev = node;
		node = node->next;
		if(prev != NULL)
			deldvbdev(prev->dev);
	}
	debug(1000, "out");
}

#endif
