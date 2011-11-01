#ifndef EPGSCAN_H
#define EPGSCAN_H

struct epgscanlist* getepgscanlistbytransponder(int transponderid)
{
	//debug(1000, "in");
	struct epgscanlist *node = epgscanlist;

	while(node != NULL)
	{
		if(node->transponderid == transponderid)
		{
			//debug(1000, "out");
			return node;
		}

		node = node->next;
	}
	debug(100, "epgscanlist not found (transponderid=%d)", transponderid);
	return NULL;
}

void epgscanlistthread(struct stimerthread* self)
{
	struct epgscanlist* node = epgscanlist, *tmpepgscannode = NULL;
	struct dvbdev* fenode = NULL;
	struct channel* chnode = NULL;
	
	debug(400, "epgscan thread start");

	if(status.standby == 0)
	{
		if(status.epglistmode == 0 || status.epglistmode == 2)
			goto end;
	}
	else if(getconfig("epg_refreshtime", NULL) == NULL)
			goto end;

	//wait for right time
	while(self->aktion != STOP && time(NULL) < 1072224000) // 01.01.2004
		usleep(1 * 1000000);

	while(node != NULL)
	{
		chnode = getchannel(node->serviceid, node->transponderid);
		if(chnode == NULL || chnode == status.aktservice->channel)
		{
			node = node->next;
			debug(400, "epgscan channel not found");
			continue;
		}

		tmpepgscannode = getepgscanlistbytransponder(node->transponderid);
		if(tmpepgscannode != NULL && tmpepgscannode->scantime != 0)
		{
			node = node->next;
			debug(400, "epgscan transponer already scanned");
			continue;
		}

		if(status.standby > 0)
			fenode = fegetfree(chnode->transponder, 0, NULL);
		else
			fenode = fegetfree(chnode->transponder, 2, NULL);
		if(fenode == NULL || (status.standby == 0 && fenode == status.aktservice->fedev))
		{
			node = node->next;
			debug(400, "epgscan no free frontend found");
			continue;
		}

		if(fenode->felock == 0)
		{
			//frontend tune
			if(fenode->feinfo->type == FE_QPSK)
			{
				feset(fenode, chnode->transponder);
				fetunedvbs(fenode, chnode->transponder);
			}
			else if(fenode->feinfo->type == FE_QAM)
				fetunedvbc(fenode, chnode->transponder);
			else if(fenode->feinfo->type == FE_OFDM)
				fetunedvbt(fenode, chnode->transponder);
			else
			{
				node = node->next;
				continue;
			}

			int festatus = fewait(fenode);
			if(festatus != 0)
			{
				node = node->next;
				debug(400, "epgscan frontend tune failed");
				continue;
			}

			if(chnode->name != NULL)
				debug(400, "epgscan scanning channel=%s, frontend=%s", chnode->name, fenode->feshortname);
			readeit(self, chnode, fenode, 1);
			node->scantime = time(NULL);
		}

		node = node->next;
	}

end:
	status.epgscanlistthread = NULL;
	debug(400, "epgscan thread end");
}

struct epgscanlist* addepgscanlist(char *line, int count, struct epgscanlist* last)
{
	//debug(1000, "in");
	struct epgscanlist *newnode = NULL, *prev = NULL, *node = epgscanlist;
	int ret = 0;

	newnode = (struct epgscanlist*)malloc(sizeof(struct epgscanlist));	
	if(newnode == NULL)
	{
		err("no memory");
		return NULL;
	}

	memset(newnode, 0, sizeof(struct epgscanlist));
	status.writeepgscanlist = 1;

	ret = sscanf(line, "%d#%lu", &newnode->serviceid, &newnode->transponderid);
        if(ret != 2)
        {
		if(count > 0)
		{
			err("epgscanlist line %d not ok", count);
		}
		else
		{
			err("add epgscanlist");
		}
		free(newnode);
		return NULL;
	}

	if(getchannel(newnode->serviceid, newnode->transponderid) == NULL)
        {
                err("epgscanlist line %d not ok (channel not found)", count);
                free(newnode);
                return NULL;
        }

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
		epgscanlist = newnode;
	else
	{
		prev->next = newnode;
		newnode->prev = prev;
	}
	newnode->next = node;
	if(node != NULL) node->prev = newnode;

	//debug(1000, "out");
	return newnode;
}

int readepgscanlist(const char* filename)
{
	debug(1000, "in");
	FILE *fd = NULL;
	char *fileline = NULL;
	int linecount = 0;
	struct epgscanlist* last = NULL, *tmplast = NULL;

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
		if(fileline[strlen(fileline) - 1] == '\n')
			fileline[strlen(fileline) - 1] = '\0';
		if(fileline[strlen(fileline) - 1] == '\r')
			fileline[strlen(fileline) - 1] = '\0';

		linecount++;

		if(last == NULL) last = tmplast;
		last = addepgscanlist(fileline, linecount, last);
		if(last != NULL) tmplast = last;
	}

	status.writeepgscanlist = 0;
	free(fileline);
	fclose(fd);
	return 0;
}

void delepgscanlist(int serviceid, int transponderid)
{
	debug(1000, "in");
	struct epgscanlist *node = epgscanlist, *prev = epgscanlist;

	while(node != NULL)
	{
		if(node->serviceid == serviceid && node->transponderid == transponderid)
		{
			status.writeepgscanlist = 1;
			if(node == epgscanlist)
			{
				epgscanlist = node->next;
				if(epgscanlist != NULL)
					epgscanlist->prev = NULL;
			}
			else
			{
				prev->next = node->next;
				if(node->next != NULL)
					node->next->prev = prev;
			}

			free(node);
			node = NULL;
			break;
		}

		prev = node;
		node = node->next;
	}
	debug(1000, "out");
}

struct epgscanlist* getepgscanlist(int serviceid, int transponderid)
{
	//debug(1000, "in");
	struct epgscanlist *node = epgscanlist;

	while(node != NULL)
	{
		if(node->serviceid == serviceid && node->transponderid == transponderid)
		{
			//debug(1000, "out");
			return node;
		}

		node = node->next;
	}
	debug(100, "epgscanlist not found (serviceid=%d, transponderid=%d)", serviceid, transponderid);
	return NULL;
}

void freeepgscanlist()
{
	debug(1000, "in");
	struct epgscanlist *node = epgscanlist, *prev = epgscanlist;

	while(node != NULL)
	{
		prev = node;
		node = node->next;
		if(prev != NULL)
			delepgscanlist(prev->serviceid, prev->transponderid);
	}
	debug(1000, "out");
}

int writeepgscanlist(const char *filename)
{
	debug(1000, "in");
	FILE *fd = NULL;
	struct epgscanlist *node = epgscanlist;
	int ret = 0;

	fd = fopen(filename, "w");
	if(fd == NULL)
	{
		perr("can't open %s", filename);
		return 1;
	}

	while(node != NULL)
	{
		ret = fprintf(fd, "%d#%lu\n", node->serviceid, node->transponderid);
		if(ret < 0)
		{
			perr("writting file %s", filename);
		}
		node = node->next;
	}

	fclose(fd);
	debug(1000, "out");
	return 0;
}

#endif
