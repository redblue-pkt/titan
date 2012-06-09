#ifndef MOSTZAP_H
#define MOSTZAP_H

void mostzaptobouquet(struct mainbouquet* mbouquet)
{
	int count = 0;
	struct mostzap* mnode = mostzap;
	struct bouquet* last = NULL;
	char* tmpstr = NULL;

	if(mbouquet == NULL) return;

	freebouquet(&mbouquet->bouquet);
	while(mnode != NULL)
	{
		tmpstr = ostrcat(tmpstr, oitoa(mnode->serviceid), 1, 1);
		tmpstr = ostrcat(tmpstr, "#", 1, 0);
		tmpstr = ostrcat(tmpstr, olutoa(mnode->transponderid), 1, 1);
		last = addbouquet(&mbouquet->bouquet, tmpstr, 0, 1, last);
		free(tmpstr); tmpstr = NULL;

		count++;
		if(count > 29) break;

		mnode = mnode->next;
	}
}

struct mostzap* getmostzap(int serviceid, unsigned long transponderid)
{
	struct mostzap* mnode = mostzap;

	while(mnode != NULL)
	{
		if(mnode->transponderid == transponderid && mnode->serviceid == serviceid)
			return mnode;
		mnode = mnode->next;
	}

	return NULL;
}

int movemostzapup(struct mostzap* node)
{
	struct mostzap* prev = NULL, *next = NULL;

	if(node == NULL || mostzap == NULL)
	{
		debug(1000, "NULL detect");
		return 1;
	}

	//first node
	if(node->prev == NULL)
		return 0;

	//haenge node aus 
	node->prev->next = node->next;
	if(node->next != NULL)
		node->next->prev = node->prev;

	//save nodes next and prev
	next = node->next;
	prev = node->prev;

	//haenge es eine pos voher ein
	node->next = prev;
	node->prev = prev->prev;
	
	if(prev->prev != NULL)
		prev->prev->next = node;
	else
		mostzap = node;
	prev->prev = node;

	status.writemostzap = 1;
	return 0;
}

struct mostzap* addmostzap(char* line, int count, struct mostzap* last)
{
	//debug(1000, "in");
	struct mostzap *newnode = NULL, *prev = NULL, *node = mostzap;
	int ret = 0;

	if(line == NULL) return NULL;

	newnode = (struct mostzap*)calloc(1, sizeof(struct mostzap));
	if(newnode == NULL)
	{
		err("no memory");
		return NULL;
	}

	ret = sscanf(line, "%d#%lu#%d", &newnode->serviceid, &newnode->transponderid, &newnode->count);

	status.writemostzap = 1;

	if(last == NULL)
	{
		while(node != NULL && newnode->count < node->count)
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
		mostzap = newnode;
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

struct mostzap* createmostzap(int serviceid, unsigned long transponderid)
{
	struct mostzap* mnode = NULL;
	char* tmpstr = NULL;

	mnode = getmostzap(serviceid, transponderid);
	if(mnode != NULL)
	{
		mnode->count++;
		while(mnode->prev != NULL && mnode->prev->count < mnode->count)
			movemostzapup(mnode);
		return mnode;
	}

	tmpstr = ostrcat(tmpstr, oitoa(serviceid), 1, 1);
	tmpstr = ostrcat(tmpstr, "#", 1, 0);
	tmpstr = ostrcat(tmpstr, olutoa(transponderid), 1, 1);
	tmpstr = ostrcat(tmpstr, "#", 1, 0);
	tmpstr = ostrcat(tmpstr, "0", 1, 0);

	mnode = addmostzap(tmpstr, 1, NULL);
	free(tmpstr);
	return mnode;
}

int readmostzap(const char* filename)
{
	debug(1000, "in");
	FILE *fd = NULL;
	char *fileline = NULL;
	int linecount = 0, len = 0;

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

		addmostzap(fileline, linecount, NULL);
	}

	status.writemostzap = 0;
	free(fileline);
	fclose(fd);
	return 0;
}

int delmostzap(int serviceid, unsigned long transponderid, int flag)
{
	debug(1000, "in");
	int ret = 1;
	struct mostzap *node = mostzap, *prev = mostzap;

	while(node != NULL)
	{
		if(node->serviceid == serviceid && node->transponderid == transponderid)
		{
			ret = 0;
			if(node == mostzap)
			{
				mostzap = node->next;
				if(mostzap != NULL)
					mostzap->prev = NULL;
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
	return ret;
}

void freemostzap(int flag)
{
	debug(1000, "in");
	struct mostzap *node = mostzap, *prev = mostzap;

	while(node != NULL)
	{
		prev = node;
		node = node->next;
		if(prev != NULL)
			delmostzap(prev->serviceid, prev->transponderid, flag);
	}
	debug(1000, "out");
}

int writemostzap(const char *filename)
{
	debug(1000, "in");
	FILE *fd = NULL;
	struct mostzap *node = mostzap;
	int ret = 0, count = 0;;

	fd = fopen(filename, "w");
	if(fd == NULL)
	{
		perr("can't open %s", filename);
		return 1;
	}

	while(node != NULL)
	{
		ret = fprintf(fd, "%d#%lu#%d\n", node->serviceid, node->transponderid, node->count);
		if(ret < 0)
		{
			perr("writting file %s", filename);
		}

		count++;
		if(count > 29) break;

		node = node->next;
	}

	fclose(fd);
	debug(1000, "out");
	return 0;
}

#endif
