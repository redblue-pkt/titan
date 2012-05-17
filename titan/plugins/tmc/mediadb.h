#ifndef MEDIADB_H
#define MEDIADB_H

struct mediadb
{
	char* name;
	char* file;
	char* pic;
	struct mediadb* prev;
	struct mediadb* next;
};

struct mediadb *mediadb = NULL;

struct mediadb* getlastmediadb(struct mediadb* node)
{
	debug(1000, "in");
	struct mediadb *prev = NULL;

	while(node != NULL)
	{
		prev = node;
		node = node->next;
	}

	debug(1000, "out");
	return prev;
}

int movemediadbdown(struct mediadb* node)
{
	struct mediadb* prev = NULL, *next = NULL;

	if(node == NULL || mediadb == NULL)
	{
		debug(1000, "NULL detect");
		return 1;
	}

	//last node
	if(node->next == NULL)
	{
		if(node->prev != NULL)
			node->prev->next = NULL;
		node->prev = NULL;
		node->next = mediadb;
		mediadb->prev = node;
		mediadb = node;
		return 0;
	}

	//haenge node aus 
	if(node->prev != NULL)
		node->prev->next = node->next;
	else
		mediadb = node->next;
	node->next->prev = node->prev;

	//save nodes next and prev
	next = node->next;
	prev = node->prev;

	//haenge es eine pos nacher ein
	node->next = next->next;
	node->prev = next;
	
	if(next->next != NULL)
		next->next->prev = node;
	next->next = node;

	//status.writemediadb = 1;
	return 0;
}

int movemediadbup(struct mediadb* node)
{
	struct mediadb* prev = NULL, *next = NULL, *last = NULL;

	if(node == NULL || mediadb == NULL)
	{
		debug(1000, "NULL detect");
		return 1;
	}

	//first node
	if(node->prev == NULL)
	{
		last = getlastmediadb(mediadb);

		if(node->next != NULL)
			node->next->prev = NULL;
		mediadb = node->next;
		node->next = NULL;
		last->next = node;
		node->prev = last;
		return 0;
	}

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
		mediadb = node;
	prev->prev = node;

	//status.writemediadb = 1;
	return 0;
}

struct mediadb* addmediadb(char *line, int count, struct mediadb* last)
{
	//debug(1000, "in");
	struct mediadb *newnode = NULL, *prev = NULL, *node = mediadb;
	char *name = NULL, *file = NULL, *pic = NULL;
	int ret = 0;

	if(line == NULL) return NULL;

	newnode = (struct mediadb*)malloc(sizeof(struct mediadb));	
	if(newnode == NULL)
	{
		err("no memory");
		return NULL;
	}

	name = malloc(256);
	if(name == NULL)
	{
		err("no memory");
		free(newnode);
		return NULL;
	}
	file = malloc(256);
	if(file == NULL)
	{
		err("no memory");
		free(name);
		free(newnode);
		return NULL;
	}
	pic = malloc(256);
	if(pic == NULL)
	{
		err("no memory");
		free(file);
		free(name);
		free(newnode);
		return NULL;
	}

	memset(newnode, 0, sizeof(struct mediadb));

	//ret = sscanf(line, "%[^#]#%lu#%d#%d#%d#%"SCNu8"#%"SCNu8"#%"SCNu8"#%"SCNu16"#%"SCNu16"#%"SCNu8, name, &newnode->transponderid, &newnode->providerid, &newnode->serviceid, &newnode->servicetype, &newnode->flag, &newnode->videocodec, &newnode->audiocodec, &newnode->videopid, &newnode->audiopid, &newnode->protect);
	ret = sscanf(line, "%[^#]#%[^#]#%[^#]", name, file, pic);
	if(ret != 3)
	{
		if(count > 0)
		{
			err("mediadb line %d not ok or double", count);
		}
		else
		{
			err("add mediadb");
		}
		free(pic);
		free(file);
		free(name);
		free(newnode);
		return NULL;
	}

	newnode->name = ostrcat(name, "", 1, 0);
	newnode->file = ostrcat(file, "", 1, 0);
	newnode->pic = ostrcat(pic, "", 1, 0);

	//modifymediadbcache(newnode->serviceid, newnode->transponderid, newnode);

	if(last == NULL)
	{
		while(node != NULL && strcasecmp(newnode->name, node->name) > 0)
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
		mediadb = newnode;
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

struct mediadb* createmediadb(char* name, unsigned long transponderid, int providerid, int serviceid, int servicetype, int flag, int videocodec, int audiocodec, int videopid, int audiopid, int protect)
{
	struct mediadb* mnode = NULL;
	char* tmpstr = NULL;

	tmpstr = ostrcat(tmpstr, name, 1, 0);
	tmpstr = ostrcat(tmpstr, "#", 1, 0);
	tmpstr = ostrcat(tmpstr, olutoa(transponderid), 1, 1);
	tmpstr = ostrcat(tmpstr, "#", 1, 0);
	tmpstr = ostrcat(tmpstr, oitoa(providerid), 1, 1);
	tmpstr = ostrcat(tmpstr, "#", 1, 0);
	tmpstr = ostrcat(tmpstr, oitoa(serviceid), 1, 1);
	tmpstr = ostrcat(tmpstr, "#", 1, 0);
	tmpstr = ostrcat(tmpstr, oitoa(servicetype), 1, 1);
	tmpstr = ostrcat(tmpstr, "#", 1, 0);
	tmpstr = ostrcat(tmpstr, oitoa(flag), 1, 1);
	tmpstr = ostrcat(tmpstr, "#", 1, 0);
	tmpstr = ostrcat(tmpstr, oitoa(videocodec), 1, 1);
	tmpstr = ostrcat(tmpstr, "#", 1, 0);
	tmpstr = ostrcat(tmpstr, oitoa(audiocodec), 1, 1);
	tmpstr = ostrcat(tmpstr, "#", 1, 0);
	tmpstr = ostrcat(tmpstr, oitoa(videopid), 1, 1);
	tmpstr = ostrcat(tmpstr, "#", 1, 0);
	tmpstr = ostrcat(tmpstr, oitoa(audiopid), 1, 1);
	tmpstr = ostrcat(tmpstr, "#", 1, 0);
	tmpstr = ostrcat(tmpstr, oitoa(protect), 1, 1);

	mnode = addmediadb(tmpstr, 1, NULL);

	free(tmpstr);
	return mnode;
}

int readmediadb(const char* filename)
{
	debug(1000, "in");
	FILE *fd = NULL;
	char *fileline = NULL;
	int linecount = 0;
	struct mediadb* last = NULL, *tmplast = NULL;

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
		last = addmediadb(fileline, linecount, last);
		if(last != NULL) tmplast = last;
	}

	//status.writemediadb = 0;
	free(fileline);
	fclose(fd);
	return 0;
}

int delmediadb(struct mediadb* mnode, int flag)
{
	debug(1000, "in");
	int ret = 1;
	struct mediadb *node = mediadb, *prev = mediadb;

	while(node != NULL)
	{
		if(node == mnode)
		{
			ret = 0;
			if(node == mediadb)
			{
				mediadb = node->next;
				if(mediadb != NULL)
					mediadb->prev = NULL;
			}
			else
			{
				prev->next = node->next;
				if(node->next != NULL)
					node->next->prev = prev;
			}

			//delmediadbcache(node->serviceid, node->transponderid);

			free(node->name);
			node->name = NULL;

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

void freemediadb(int flag)
{
	debug(1000, "in");
	struct mediadb *node = mediadb, *prev = mediadb;

	while(node != NULL)
	{
		prev = node;
		node = node->next;
		if(prev != NULL)
			delmediadb(prev, flag);
	}
	debug(1000, "out");
}

int writemediadb(const char *filename)
{
	debug(1000, "in");
	FILE *fd = NULL;
	struct mediadb *node = mediadb;
	int ret = 0;

	fd = fopen(filename, "w");
	if(fd == NULL)
	{
		perr("can't open %s", filename);
		return 1;
	}

	while(node != NULL)
	{
		ret = fprintf(fd, "%s#%s#%s\n", node->name, node->file, node->pic);
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
