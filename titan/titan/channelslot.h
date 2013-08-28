#ifndef CHANNELSLOT_H
#define CHANNELSLOT_H

struct channelslot* addchannelslot(char *line, int count, struct channelslot* last)
{
	STARTFUNC
	struct channelslot *newnode = NULL, *prev = NULL, *node = channelslot;
	int ret = 0;

	newnode = (struct channelslot*)calloc(1, sizeof(struct channelslot));
	if(newnode == NULL)
	{
		err("no memory");
		return NULL;
	}

	ret = sscanf(line, "%llu#%d#%d", &newnode->transponderid, &newnode->serviceid, &newnode->slot);
	if(ret != 3)
	{
		if(count > 0)
		{
			err("channelslot line %d not ok", count);
		}
		else
		{
			err("add channelslot");
		}
		free(newnode);
		return NULL;
	}

	status.writechannelslot = 1;

	if(last == NULL)
	{
		while(node != NULL)
		{
			prev = node;
			node = node->next;
		}
	}
	else
	{
		prev = last;
		node = last->next;
	}

	if(prev == NULL)
		channelslot = newnode;
	else
		prev->next = newnode;

	newnode->next = node;

	return newnode;
}

struct channelslot* getchannelslot(int serviceid, uint64_t transponderid)
{
	STARTFUNC
	struct channelslot *node = channelslot;
	
	while(node != NULL)
	{
		if(node->serviceid == serviceid && node->transponderid == transponderid)
			return node;
		node = node->next;	
	}
	
	return NULL;
}

int readchannelslot(char* filename)
{
	STARTFUNC
	FILE *fd = NULL;
	char* fileline = NULL;
	int linecount = 0, len = 0;
	struct channelslot* last = NULL, *tmplast = NULL;
	
	fileline = malloc(MINMALLOC);
	if(fileline == NULL)
	{
		err("no memory");
		return 1;
	}

	fd = fopen(filename, "r");
	if(fd == NULL)
	{
		perr("can't open %s", filename);
		free(fileline);
		return 1;
	}

	while(fgets(fileline, MINMALLOC, fd) != NULL)
	{
		if(fileline[0] == '#' || fileline[0] == '\n')
			continue;
		len = strlen(fileline) - 1;
		if(len >= 0 && fileline[len] == '\n')
			fileline[len] = '\0';
		len--;
		if(len >= 0 && fileline[len] == '\r')
			fileline[len] = '\0';

		linecount++;

		if(last == NULL) last = tmplast;
		last = addchannelslot(fileline, linecount, last);
		if(last != NULL) tmplast = last;
	}

  status.writechannelslot = 0;
	free(fileline);
	fclose(fd);
	return 0;
}

int delchannelslot(int serviceid, uint64_t transponderid)
{
	STARTFUNC
	int ret = 1;
	struct channelslot *node = channelslot, *prev = channelslot;

	while(node != NULL)
	{
		if(node->serviceid == serviceid && node->transponderid == transponderid)
		{
			status.writechannelslot = 1;
			if(node == channelslot)
				channelslot = node->next;
			else
				prev->next = node->next;

			free(node);
			node = NULL;
			ret = 0;
			break;
		}

		prev = node;
		node = node->next;
	}
	return ret;
}

void freechannelslot()
{
	STARTFUNC
	struct channelslot *node = channelslot, *prev = channelslot;

	while(node != NULL)
	{
		prev = node;
		node = node->next;
		if(prev != NULL)
			delchannelslot(prev->serviceid, prev->transponderid);
	}
}

int writechannelslot(char *filename)
{
	STARTFUNC
	FILE *fd = NULL;
	struct channelslot *node = channelslot;
	int ret = 0;

	fd = fopen(filename, "w");
	if(fd == NULL)
	{
		perr("can't open %s", filename);
		return 1;
	}

	while(node != NULL)
	{
		ret = fprintf(fd, "%llu#%d#%d\n", node->transponderid, node->serviceid, node->slot);
		if(ret < 0)
		{
			perr("writting file %s", filename);
		}
		node = node->next;
	}

	fclose(fd);
	return 0;
}

#endif
