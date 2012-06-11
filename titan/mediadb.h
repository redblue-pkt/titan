#ifndef MEDIADB_H
#define MEDIADB_H

//flag 0: with lock
//flag 1: without lock
struct mediadbfilter* getmediadbfilter(struct mediadb* mnode, int flag)
{
	if(flag == 0) m_lock(&status.mediadbmutex, 17);
	struct mediadbfilter *node = mediadbfilter;

	while(node != NULL)
	{
		if(node->node == mnode)
		{
			if(flag == 0) m_unlock(&status.mediadbmutex, 17);
			return node;
		}
		node = node->next;
	}

	if(flag == 0) m_unlock(&status.mediadbmutex, 17);
	return NULL;
}

int getmediadbfiltercount()
{
	int count = 0;

	m_lock(&status.mediadbmutex, 17);
	struct mediadbfilter *node = mediadbfilter;

	while(node != NULL)
	{
		count++;
		node = node->next;
	}

	m_unlock(&status.mediadbmutex, 17);
	return count;
}

struct mediadbfilter* getmediadbfilterrandom(int maxentry)
{
	int count = 0;

	if(maxentry < 1) return NULL;

	m_lock(&status.mediadbmutex, 17);
	struct mediadbfilter *node = mediadbfilter;

	srand(time(NULL));
	int r = rand() % maxentry;
	r++;

	while(node != NULL)
	{
		count++;
		if(count == r) break;

		node = node->next;
	}

	m_unlock(&status.mediadbmutex, 17);
	return node;
}

//flag 0: with lock
//flag 1: without lock
struct mediadbfilter* getlastmediadbfilter(struct mediadbfilter* node, int flag)
{
	if(flag == 0) m_lock(&status.mediadbmutex, 17);
	struct mediadbfilter *prev = NULL;

	while(node != NULL)
	{
		prev = node;
		node = node->next;
	}

	if(flag == 0) m_unlock(&status.mediadbmutex, 17);
	return prev;
}

void getmediadbcounts(int* video, int* audio, int* picture)
{
	m_lock(&status.mediadbmutex, 17);
	struct mediadb* node = mediadb;

	while(node != NULL)
	{
		if(node->type == 0)
			(*video)++;
		else if(node->type == 1)
			(*audio)++;
		else if(node->type == 2)
			(*picture)++;

		node = node->next;
	}

	m_unlock(&status.mediadbmutex, 17);
}

//flag 0: with lock
//flag 1: without lock
struct mediadb* getlastmediadb(struct mediadb* node, int flag)
{
	struct mediadb *prev = NULL;

	if(flag == 0) m_lock(&status.mediadbmutex, 17);
	while(node != NULL)
	{
		prev = node;
		node = node->next;
	}

	if(flag == 0) m_unlock(&status.mediadbmutex, 17);
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

	m_lock(&status.mediadbmutex, 17);

	//last node
	if(node->next == NULL)
	{
		if(node->prev != NULL)
			node->prev->next = NULL;
		node->prev = NULL;
		node->next = mediadb;
		mediadb->prev = node;
		mediadb = node;
		m_unlock(&status.mediadbmutex, 17);
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

	m_unlock(&status.mediadbmutex, 17);
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

	m_lock(&status.mediadbmutex, 17);

	//first node
	if(node->prev == NULL)
	{
		last = getlastmediadb(mediadb, 1);

		if(node->next != NULL)
			node->next->prev = NULL;
		mediadb = node->next;
		node->next = NULL;
		last->next = node;
		node->prev = last;
		m_unlock(&status.mediadbmutex, 17);
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

	m_unlock(&status.mediadbmutex, 17);
	//status.writemediadb = 1;
	return 0;
}

struct mediadbfilter* addmediadbfilter(struct mediadb* mnode, int count, struct mediadbfilter* last)
{
	struct mediadbfilter *newnode = NULL, *prev = NULL, *node = NULL;

	if(mnode == NULL) return NULL;

	newnode = (struct mediadbfilter*)calloc(1, sizeof(struct mediadbfilter));
	if(newnode == NULL)
	{
		err("no memory");
		return NULL;
	}

	newnode->node = mnode;

	m_lock(&status.mediadbmutex, 17);
	node = mediadbfilter;

	if(last == NULL)
	{
		//while(node != NULL && strcasecmp(newnode->node->title, node->node->title) > 0)
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
		mediadbfilter = newnode;
	else
	{
		prev->next = newnode;
		newnode->prev = prev;
	}
	newnode->next = node;
	if(node != NULL) node->prev = newnode;
	
	m_unlock(&status.mediadbmutex, 17);
	//debug(1000, "out");
	return newnode;
}

//flag 0: with lock
//flag 1: without lock
struct mediadbcategory* addmediadbcategory(char* line, int type, int count, struct mediadbcategory* last, int flag)
{
	struct mediadbcategory *newnode = NULL, *prev = NULL, *node = NULL;
	char* name = NULL;
	int ret = 0;

	if(line == NULL) return NULL;

	newnode = (struct mediadbcategory*)calloc(1, sizeof(struct mediadbcategory));
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
	
	ret = sscanf(line, "%d#%[^#]", &newnode->type, name);
	if(ret != 2)
	{
		if(count > 0)
		{
			err("mediadbcategory line %d not ok or double", count);
		}
		else
		{
			err("add mediadbcategory");
		}
		free(name);
		free(newnode);
		return NULL;
	}
	
	if(newnode->type != type)
	{
		free(name);
		free(newnode);
		return NULL;	
	}

	newnode->name = ostrcat(name, NULL, 1, 0);

	if(flag == 0) m_lock(&status.mediadbmutex, 17);
	node = mediadbcategory;

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
		mediadbcategory = newnode;
	else
	{
		prev->next = newnode;
		newnode->prev = prev;
	}
	newnode->next = node;
	if(node != NULL) node->prev = newnode;
	
	if(flag == 0) m_unlock(&status.mediadbmutex, 17);
	//debug(1000, "out");
	return newnode;
}

//flag1 0: with lock
//flag1 1: without lock
struct mediadb* addmediadb(char *line, int len, int count, struct mediadb* last, int sort, int flag1)
{
	//debug(1000, "in");
	struct mediadb *newnode = NULL, *prev = NULL, *node = NULL;
	int ret = 0;
	char* tmpstr = NULL, *type = NULL, *year = NULL, *rating = NULL;
	char* votes = NULL, *timestamp = NULL, *flag = NULL;

	if(line == NULL) return NULL;

	newnode = (struct mediadb*)calloc(1, sizeof(struct mediadb));
	if(newnode == NULL)
	{
		err("no memory");
		return NULL;
	}

	if(len > 0) tmpstr = malloc(len + 1);
	if(tmpstr != NULL)
	{
		memcpy(tmpstr, line, len);
		tmpstr[len] = '\0';

		newnode->id = tmpstr;

		while(tmpstr[0] != '\0')
		{
			if(tmpstr[0] == '#')
			{
				tmpstr[0] = '\0';
				tmpstr++;
				switch(ret)
				{
					case 0: type = tmpstr; break;
					case 1: newnode->title = tmpstr; break;
					case 2: year = tmpstr; break;
					case 3: newnode->released = tmpstr; break;
					case 4: newnode->runtime = tmpstr; break;
					case 5: newnode->genre = tmpstr; break;
					case 6: newnode->director = tmpstr; break;
					case 7: newnode->writer = tmpstr; break;
					case 8: newnode->actors = tmpstr; break;
					case 9: newnode->plot = tmpstr; break;
					case 10: newnode->poster = tmpstr; break;
					case 11: rating = tmpstr; break;
					case 12: votes = tmpstr; break;
					case 13: newnode->file = tmpstr; break;
					case 14: timestamp = tmpstr; break;
					case 15: flag = tmpstr; break;
				}

				ret++;
			}
			else
				tmpstr++;
		}
	}

	if(ret != 16)
	{
		if(count > 0)
		{
			err("mediadb line %d not ok (ret=%d)", count, ret);
		}
		else
		{
			err("add mediadb (ret=%d)", ret);
		}
		freemediadbcontent(newnode);
		free(newnode);
		return NULL;
	}

	if(type != NULL) newnode->type = atoi(type);
	if(year != NULL) newnode->year = atoi(year);
	if(rating != NULL) newnode->rating = atoi(rating);
	if(votes != NULL) newnode->votes = atoi(votes);
	if(timestamp != NULL) newnode->timestamp = strtoul(timestamp, NULL, 10);
	if(flag != NULL) newnode->flag = atoi(flag);

	if(flag1 == 0) m_lock(&status.mediadbmutex, 17);
	node = mediadb;

	modifymediadbcache(newnode->file, newnode);

	if(last == NULL)
	{
		if(sort == 1)
		{
			while(node != NULL && strcasecmp(newnode->title, node->title) > 0)
			{
				prev = node;
				node = node->next;
			}
		}
		else
		{
			while(node != NULL)
			{
				prev = node;
				node = node->next;
			}
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

	if(flag1 == 0) m_unlock(&status.mediadbmutex, 17);
	//debug(1000, "out");
	return newnode;
}

struct mediadb* createmediadb(struct mediadb* update, char* id, int type, char* title, char* year, char* released, char* runtime, char* genre, char* director, char* writer, char* actors, char* plot, char* poster, char* rating, char* votes, char* fullfile, char* file, int flag)
{
	struct mediadb* mnode = NULL;
	char* tmpstr = NULL;

	if(fullfile == NULL) return NULL;

	if(title == NULL) title = file;

	id = stringreplacechar(id, '#', ' ');
	title = stringreplacechar(title, '#', ' ');
	year = stringreplacechar(year, '#', ' ');
	released = stringreplacechar(released, '#', ' ');
	runtime = stringreplacechar(runtime, '#', ' ');
	genre = stringreplacechar(genre, '#', ' ');
	director = stringreplacechar(director, '#', ' ');
	writer = stringreplacechar(writer, '#', ' ');
	actors = stringreplacechar(actors, '#', ' ');
	plot = stringreplacechar(plot, '#', ' ');
	poster = stringreplacechar(poster, '#', ' ');
	rating = stringreplacechar(rating, ',', '.');
	votes = stringreplacechar(votes, ',', '.');
	fullfile = stringreplacechar(fullfile, '#', ' ');
printf("11\n");
	tmpstr = ostrcat(tmpstr, id, 1, 0);
printf("112\n");
	tmpstr = ostrcat(tmpstr, "#", 1, 0);
printf("113\n");
	tmpstr = ostrcat(tmpstr, oitoa(type), 1, 1);
printf("114\n");
	tmpstr = ostrcat(tmpstr, "#", 1, 0);
printf("115\n");
	tmpstr = ostrcat(tmpstr, title, 1, 0);
printf("116\n");
	tmpstr = ostrcat(tmpstr, "#", 1, 0);
printf("117\n");
	if(year != NULL) tmpstr = ostrcat(tmpstr, oitoa(atoi(year)), 1, 1);
printf("118\n");
	tmpstr = ostrcat(tmpstr, "#", 1, 0);
printf("119\n");
	tmpstr = ostrcat(tmpstr, released, 1, 0);
printf("1110\n");
	tmpstr = ostrcat(tmpstr, "#", 1, 0);
printf("1111\n");
	tmpstr = ostrcat(tmpstr, runtime, 1, 0);
printf("1112\n");
	tmpstr = ostrcat(tmpstr, "#", 1, 0);
printf("1113\n");
	tmpstr = ostrcat(tmpstr, genre, 1, 0);
printf("1114\n");
	tmpstr = ostrcat(tmpstr, "#", 1, 0);
printf("1115\n");
	tmpstr = ostrcat(tmpstr, director, 1, 0);
printf("1116\n");
	tmpstr = ostrcat(tmpstr, "#", 1, 0);
printf("1117\n");
	tmpstr = ostrcat(tmpstr, writer, 1, 0);
printf("1118\n");
	tmpstr = ostrcat(tmpstr, "#", 1, 0);
printf("1119\n");
	tmpstr = ostrcat(tmpstr, actors, 1, 0);
printf("1120\n");
	tmpstr = ostrcat(tmpstr, "#", 1, 0);
printf("1121\n");
	tmpstr = ostrcat(tmpstr, plot, 1, 0);
printf("1122\n");
	tmpstr = ostrcat(tmpstr, "#", 1, 0);
printf("1123\n");
	tmpstr = ostrcat(tmpstr, poster, 1, 0);
printf("1124\n");
	tmpstr = ostrcat(tmpstr, "#", 1, 0);
printf("1125\n");
	if(rating != NULL) tmpstr = ostrcat(tmpstr, oitoa(atoi(rating)), 1, 1);
printf("1126\n");
	tmpstr = ostrcat(tmpstr, "#", 1, 0);
printf("1127\n");
	if(votes != NULL) tmpstr = ostrcat(tmpstr, oitoa(atoi(votes)), 1, 1);
printf("1128\n");
	tmpstr = ostrcat(tmpstr, "#", 1, 0);
printf("1129\n");
	tmpstr = ostrcat(tmpstr, fullfile, 1, 0);
printf("1130\n");
	tmpstr = ostrcat(tmpstr, "#", 1, 0);
printf("1131\n");
	tmpstr = ostrcat(tmpstr, olutoa(time(NULL)), 1, 1);
printf("1132\n");
	tmpstr = ostrcat(tmpstr, "#", 1, 0);
printf("1133\n");
	tmpstr = ostrcat(tmpstr, oitoa(flag), 1, 1);
printf("1134\n");
	if(update != NULL)
		delmediadb(update, 0);

	mnode = addmediadb(tmpstr, strlen(tmpstr), 1, NULL, 1, 0);
	free(tmpstr);

	return mnode;
}

//flag 0: read mediadb
//flag 1: read mediadbcategory
int readmediadb(const char* filename, int type, int flag)
{
	debug(1000, "in");
	FILE *fd = NULL;
	char *fileline = NULL;
	int linecount = 0, len = 0;
	struct mediadb* last = NULL, *tmplast = NULL;
	struct mediadbcategory* lastcategory = NULL, *tmplastcategory = NULL;

	m_lock(&status.mediadbmutex, 17);

	if(flag == 0 && mediadb != NULL)
	{
		m_unlock(&status.mediadbmutex, 17);
		return 1;
	}

	fileline = malloc(MINMALLOC);
	if(fileline == NULL)
	{
		err("no memory");
		m_unlock(&status.mediadbmutex, 17);
		return 1;
	}

	fd = fopen(filename, "r");
	if(fd == NULL)
	{
		perr("can't open %s", filename);
		free(fileline);
		m_unlock(&status.mediadbmutex, 17);
		return 1;
	}

printf("xxxxxxxxxxxxxxxxxx %lu\n", time(NULL));
	while(fgets(fileline, MINMALLOC, fd) != NULL)
	{
		if(fileline[0] == '\n')
			continue;
		len = strlen(fileline) - 1;
		if(fileline[len] == '\n')
			fileline[len] = '\0';
		if(fileline[len - 1] == '\r')
			fileline[len - 1] = '\0';

		linecount++;

		if(flag == 0)
		{
			if(last == NULL) last = tmplast;
			last = addmediadb(fileline, len + 2, linecount, last, 0, 1);
			if(last != NULL) tmplast = last;
		}
		else
		{
			if(lastcategory == NULL) lastcategory = tmplastcategory;
			lastcategory = addmediadbcategory(fileline, type, linecount, lastcategory, 1);
			if(lastcategory != NULL) tmplastcategory = lastcategory;
		}
	}

	//status.writemediadb = 0;

	free(fileline);
	fclose(fd);
	m_unlock(&status.mediadbmutex, 17);
printf("xxxxxxxxxxxxxxxxxx %lu\n", time(NULL));
	return 0;
}

void freemediadbcontent(struct mediadb* node)
{
	if(node == NULL) return;

	free(node->id); node->id = NULL;
}

//flag 0: with lock
//flag 1: without lock
int delmediadbfilter(struct mediadbfilter* mnode, int flag)
{
	debug(1000, "in");
	int ret = 1;

	if(flag == 0) m_lock(&status.mediadbmutex, 17);
	struct mediadbfilter *node = mediadbfilter, *prev = mediadbfilter;

	while(node != NULL)
	{
		if(node == mnode)
		{
			ret = 0;
			if(node == mediadbfilter)
			{
				mediadbfilter = node->next;
				if(mediadbfilter != NULL)
					mediadbfilter->prev = NULL;
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

	if(flag == 0) m_unlock(&status.mediadbmutex, 17);
	debug(1000, "out");
	return ret;
}

int delmediadbcategory(struct mediadbcategory* mnode, int flag)
{
	debug(1000, "in");
	int ret = 1;

	m_lock(&status.mediadbmutex, 17);
  struct mediadbcategory *node = mediadbcategory, *prev = mediadbcategory;

	while(node != NULL)
	{
		if(node == mnode)
		{
			ret = 0;
			if(node == mediadbcategory)
			{
				mediadbcategory = node->next;
				if(mediadbcategory != NULL)
					mediadbcategory->prev = NULL;
			}
			else
			{
				prev->next = node->next;
				if(node->next != NULL)
					node->next->prev = prev;
			}
			
			free(node->name);
			node->name = NULL;

			free(node);
			node = NULL;

			break;
		}

		prev = node;
		node = node->next;
	}

	m_unlock(&status.mediadbmutex, 17);
	debug(1000, "out");
	return ret;
}

int delmediadb(struct mediadb* mnode, int flag)
{
	debug(1000, "in");
	int ret = 1;

	m_lock(&status.mediadbmutex, 17);
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

			struct mediadbfilter* mfnode = getmediadbfilter(node, 1);
			if(mfnode != NULL)
				delmediadbfilter(mfnode, 1);
			delmediadbcache(node->file);
			freemediadbcontent(node);

			free(node);
			node = NULL;

			break;
		}

		prev = node;
		node = node->next;
	}

	m_unlock(&status.mediadbmutex, 17);
	debug(1000, "out");
	return ret;
}

void freemediadbfilter(int flag)
{
	debug(1000, "in");
	struct mediadbfilter *node = mediadbfilter, *prev = mediadbfilter;

	while(node != NULL)
	{
		prev = node;
		node = node->next;
		if(prev != NULL)
			delmediadbfilter(prev, flag);
	}
	debug(1000, "out");
}

void freemediadbcategory(int flag)
{
	debug(1000, "in");
	struct mediadbcategory *node = mediadbcategory, *prev = mediadbcategory;

	while(node != NULL)
	{
		prev = node;
		node = node->next;
		if(prev != NULL)
			delmediadbcategory(prev, flag);
	}
	debug(1000, "out");
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

int writemediadbcategory(const char *filename)
{
	debug(1000, "in");
	FILE *fd = NULL;
	struct mediadbcategory *node = NULL;
	int ret = 0;

	fd = fopen(filename, "w");
	if(fd == NULL)
	{
		perr("can't open %s", filename);
		return 1;
	}

	m_lock(&status.mediadbmutex, 17);
	node = mediadbcategory;

	while(node != NULL)
	{
		ret = fprintf(fd, "%d#%s\n", node->type, node->name);
		if(ret < 0)
		{
			perr("writting file %s", filename);
		}
		node = node->next;
	}

	m_unlock(&status.mediadbmutex, 17);

	fclose(fd);
	debug(1000, "out");
	return 0;
}

int writemediadb(const char *filename)
{
	debug(1000, "in");
	FILE *fd = NULL;
	struct mediadb *node = NULL;
	int ret = 0;

	fd = fopen(filename, "w");
	if(fd == NULL)
	{
		perr("can't open %s", filename);
		return 1;
	}

	m_lock(&status.mediadbmutex, 17);
	node = mediadb;

	while(node != NULL)
	{
		ret = fprintf(fd, "%s#%d#%s#%d#%s#%s#%s#%s#%s#%s#%s#%s#%d#%d#%s#%lu#%d\n", node->id, node->type, node->title, node->year, node->released, node->runtime, node->genre, node->director, node->writer, node->actors, node->plot, node->poster, node->rating, node->votes, node->file, node->timestamp, node->flag);

		if(ret < 0)
		{
			perr("writting file %s", filename);
		}
		node = node->next;
	}

	m_unlock(&status.mediadbmutex, 17);

	fclose(fd);
	debug(1000, "out");
	return 0;
}

//flag 0: all
//flag 1: year
//flag 2: director
//flag 3: actors
//flag 4: category
//flag 5: rating
//flag 6: genre
//flag 7: a-z
void createmediadbfilter(int type, char* search, int flag)
{
	int isearch = 0;
	struct mediadb* node = mediadb;
	struct mediadbfilter* last = NULL;

	if(status.mediadbthreadstatus == 1) return;

	if(flag == 1 || flag == 5)
	{
		if(search == NULL) return;
		isearch = atoi(search);
	}
	if(flag == 7 && search == NULL) return;

	freemediadbfilter(0);
	while(node != NULL)
	{
		if(node->type == type)
		{
			if(flag == 0)
				last = addmediadbfilter(node, 1, last);
			else if(flag == 1 && node->year == isearch)
				last = addmediadbfilter(node, 1, last);
			else if(flag == 2 && ostrstrcase(node->director, search) != NULL)
				last = addmediadbfilter(node, 1, last);
			else if(flag == 3 && ostrstrcase(node->actors, search) != NULL)
				last = addmediadbfilter(node, 1, last);
			else if(flag == 4 && ostrstrcase(node->file, search) != NULL)
				last = addmediadbfilter(node, 1, last);
			else if(flag == 5 && node->rating == isearch)
				last = addmediadbfilter(node, 1, last);
			else if(flag == 6 && ostrstrcase(node->genre, search) != NULL)
				last = addmediadbfilter(node, 1, last);
			else if(flag == 7 && node->title != NULL && node->title[0] == search[0])
				last = addmediadbfilter(node, 1, last);
		}
		node = node->next;
	}
}

void mediadbscanthread(struct stimerthread* self, char* path, int type)
{
	struct mediadb *node = mediadb, *prev = mediadb;
	struct mediadbcategory *cnode = NULL;
	struct hdd *hddnode = NULL;
	struct splitstr* ret = NULL;
	char* tmpstr = NULL, *tmpsplit = NULL;

	if(status.mediadbthread != NULL || self == NULL) return;

	debug(777, "mediadb scanthread start");
	status.mediadbthreadstatus = 1;
	status.mediadbthread = self;

	if(type == 1000)
	{
		type = 0;
	
		char* tmpstr = NULL;
		tmpstr = ostrcat(tmpstr, _("MediaDB directory scan started in Background !"), 1, 0);
		tmpstr = ostrcat(tmpstr, "\n\n  ", 1, 0);
		tmpstr = ostrcat(tmpstr, _("Delete MediaDB before scan"), 1, 0);
		tmpstr = ostrcat(tmpstr, ": \t", 1, 0);
		if(ostrcmp(getconfig("mediadbscandelall", NULL), "1") == 0)
			tmpstr = ostrcat(tmpstr, _("yes"), 1, 0);
		else
			tmpstr = ostrcat(tmpstr, _("no"), 1, 0);
		tmpstr = ostrcat(tmpstr, "\n  ", 1, 0);			
		tmpstr = ostrcat(tmpstr, _("Delete unused entrys before scan"), 1, 0);
		tmpstr = ostrcat(tmpstr, ": \t", 1, 0);		
		if(ostrcmp(getconfig("mediadbscandelnotfound", NULL), "1") == 0)
			tmpstr = ostrcat(tmpstr, _("yes"), 1, 0);
		else
			tmpstr = ostrcat(tmpstr, _("no"), 1, 0);
		tmpstr = ostrcat(tmpstr, "\n  ", 1, 0);
		tmpstr = ostrcat(tmpstr, _("scan Directory:"), 1, 0);
		tmpstr = ostrcat(tmpstr, " \t\t\t", 1, 0);
		tmpstr = ostrcat(tmpstr, path, 1, 0);
		tmpstr = ostrcat(tmpstr, "\n  ", 1, 0);		
		tmpstr = ostrcat(tmpstr, _("MediaDB place:"), 1, 0);
		tmpstr = ostrcat(tmpstr, " \t\t\t", 1, 0);				
		tmpstr = ostrcat(tmpstr, getconfig("mediadbpath", NULL), 1, 0);
		int count = 0;
		
		while(count < 30)
		{
			sleep(1);
			count++;
		}
		textbox(_("Message"), tmpstr, _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 800, 500, 10, 0);
		free(tmpstr), tmpstr = NULL;
		count = 0;
	}
	
	//clear all other db in mem
	freemediadbfilter(0);

	if(getconfigint("mediadbscandelall", NULL) == 1)
	{
		delallfiles(getconfig("mediadbpath", NULL), ".jpg");
		delallfiles(getconfig("mediadbpath", NULL), ".mvi");
		freemediadb(0);
	}
	else
		readmediadb(getconfig("mediadbfile", NULL), 0, 0);

	//check mediadb for not exist file
	if(getconfigint("mediadbscandelnotfound", NULL) == 1)
	{
		while(node != NULL)
		{
			prev = node;
			node = node->next;
			if(!file_exist(prev->file))
			{
				//TODO: unlink all jpg to node
				delmediadb(prev, 0);
			}
		}
	}

	status.mediadbthreadstatus = 2;

	//find media files
	if(path == NULL)
	{
		addhddall();
		hddnode = hdd;

#ifdef SIMULATE
		findfiles("/home/nit/titan/x", 100, 1, 0);
#else
		while(hddnode != NULL)
		{
			if(hddnode->partition != 0)
			{
				tmpstr = ostrcat("/autofs/", hddnode->device, 0, 0);
				findfiles(tmpstr, type, 1, 0);
				free(tmpstr); tmpstr = NULL;
			}
			hddnode = hddnode->next;
		}
#endif
	}
	else
		findfiles(path, type, 0, 0);

	writemediadb(getconfig("mediadbfile", NULL));

	status.mediadbthreadstatus = 3;
	sleep(3); //wait a little if other thread read category

	freemediadbcategory(0);

	//create year
	node = mediadb;
	while(node != NULL)
	{
		char* year = oitoa(node->year);
		int treffer = 1;
		cnode = mediadbcategory;
		while(cnode != NULL)
		{
			if(ostrcmp(cnode->name, year) == 0)
				treffer = 0;
			cnode = cnode->next;
		}

		if(treffer == 1)
		{
			tmpstr = ostrcat(oitoa(node->type), "#", 1, 0);
			tmpstr = ostrcat(tmpstr, year, 1, 0);
			debug(777, "add year %d", node->year);
			addmediadbcategory(tmpstr, node->type, 1, NULL, 0);
			free(tmpstr); tmpstr = NULL;
		}

		free(year); year = NULL;
		node = node->next;
	}
	tmpstr = ostrcat(getconfig("mediadbfile", NULL), ".year", 0, 0);
	writemediadbcategory(tmpstr);
	free(tmpstr); tmpstr = NULL;
	freemediadbcategory(0);

	//create director
	node = mediadb;
	while(node != NULL)
	{
		if(node->director == NULL)
		{
			node = node->next;
			continue;
		}

		int treffer = 1;
		cnode = mediadbcategory;
		while(cnode != NULL)
		{
			if(ostrcmp(cnode->name, node->director) == 0)
				treffer = 0;
			cnode = cnode->next;
		}
	
			if(treffer == 1)
		{
			tmpstr = ostrcat(oitoa(node->type), "#", 1, 0);
			tmpstr = ostrcat(tmpstr, node->director, 1, 0);
			debug(777, "add director %s", node->director);
			addmediadbcategory(tmpstr, node->type, 1, NULL, 0);
			free(tmpstr); tmpstr = NULL;
		}

		node = node->next;
	}
	tmpstr = ostrcat(getconfig("mediadbfile", NULL), ".director", 0, 0);
	writemediadbcategory(tmpstr);
	free(tmpstr); tmpstr = NULL;
	freemediadbcategory(0);

	//create rating
	node = mediadb;
	while(node != NULL)
	{
		char* rating = oitoa(node->rating);
		int treffer = 1;
		cnode = mediadbcategory;
		while(cnode != NULL)
		{
			if(ostrcmp(cnode->name, rating) == 0)
				treffer = 0;
			cnode = cnode->next;
		}

		if(treffer == 1)
		{
			tmpstr = ostrcat(oitoa(node->type), "#", 1, 0);
			tmpstr = ostrcat(tmpstr, rating, 1, 0);
			debug(777, "add rating %d", node->rating);
			addmediadbcategory(tmpstr, node->type, 1, NULL, 0);
			free(tmpstr); tmpstr = NULL;
		}

		free(rating); rating = NULL;
		node = node->next;
	}
	tmpstr = ostrcat(getconfig("mediadbfile", NULL), ".rating", 0, 0);
	writemediadbcategory(tmpstr);
	free(tmpstr); tmpstr = NULL;
	freemediadbcategory(0);
	
	//create actors
	node = mediadb;
	while(node != NULL)
	{
		if(node->actors == NULL)
		{
			node = node->next;
			continue;
		}

		//split
		int i = 0, count = 0;
		tmpsplit = ostrcat(node->actors, NULL, 0, 0);
		ret = strsplit(tmpsplit, ",", &count);
		
		if(ret != NULL)
		{
			for(i = 0; i < count; i++)
			{
				int treffer = 1;
				strstrip((&ret[i])->part);
				cnode = mediadbcategory;
				while(cnode != NULL)
				{
					if(ostrcmp(cnode->name, (&ret[i])->part) == 0)
						treffer = 0;
					cnode = cnode->next;
				}
		
				if(treffer == 1)
				{
					tmpstr = ostrcat(oitoa(node->type), "#", 1, 0);
					tmpstr = ostrcat(tmpstr, (&ret[i])->part, 1, 0);
					debug(777, "add actor %s", (&ret[i])->part);
					addmediadbcategory(tmpstr, node->type, 1, NULL, 0);
					free(tmpstr); tmpstr = NULL;
				}
			}
		}

		free(tmpsplit); tmpsplit = NULL;
		free(ret); ret = NULL;
		node = node->next;
	}
	tmpstr = ostrcat(getconfig("mediadbfile", NULL), ".actors", 0, 0);
	writemediadbcategory(tmpstr);
	free(tmpstr); tmpstr = NULL;
	freemediadbcategory(0);
	
	//create category
	node = mediadb;
	while(node != NULL)
	{
		//split
		int i = 0, count = 0;
		tmpsplit = ostrcat(node->file, NULL, 0, 0);
		ret = strsplit(tmpsplit, "/", &count);
		
		if(ret != NULL)
		{
			for(i = 0; i < count - 1; i++)
			{
				int treffer = 1;
				strstrip((&ret[i])->part);
				cnode = mediadbcategory;
				while(cnode != NULL)
				{
					if(ostrcmp(cnode->name, (&ret[i])->part) == 0)
						treffer = 0;
					cnode = cnode->next;
				}
		
				if(treffer == 1)
				{
					tmpstr = ostrcat(oitoa(node->type), "#", 1, 0);
					tmpstr = ostrcat(tmpstr, (&ret[i])->part, 1, 0);
					debug(777, "add category %s", (&ret[i])->part);
					addmediadbcategory(tmpstr, node->type, 1, NULL, 0);
					free(tmpstr); tmpstr = NULL;
				}
			}
		}

		free(tmpsplit); tmpsplit = NULL;
		free(ret); ret = NULL;
		node = node->next;
	}
	tmpstr = ostrcat(getconfig("mediadbfile", NULL), ".category", 0, 0);
	writemediadbcategory(tmpstr);
	free(tmpstr); tmpstr = NULL;
	freemediadbcategory(0);

	//create genre
	node = mediadb;
	while(node != NULL)
	{
		if(node->genre == NULL)
		{
			node = node->next;
			continue;
		}

		//split
		int i = 0, count = 0;
		tmpsplit = ostrcat(node->genre, NULL, 0, 0);
		ret = strsplit(tmpsplit, ",", &count);
		
		if(ret != NULL)
		{
			for(i = 0; i < count; i++)
			{		
				int treffer = 1;
				strstrip((&ret[i])->part);
				cnode = mediadbcategory;
				while(cnode != NULL)
				{
					if(ostrcmp(cnode->name, (&ret[i])->part) == 0)
						treffer = 0;
					cnode = cnode->next;
				}
		
				if(treffer == 1)
				{
					tmpstr = ostrcat(oitoa(node->type), "#", 1, 0);
					tmpstr = ostrcat(tmpstr, (&ret[i])->part, 1, 0);
					debug(777, "add genre %s", (&ret[i])->part);
					addmediadbcategory(tmpstr, node->type, 1, NULL, 0);
					free(tmpstr); tmpstr = NULL;
				}
			}
		}

		free(tmpsplit); tmpsplit = NULL;
		free(ret); ret = NULL;
		node = node->next;
	}
	tmpstr = ostrcat(getconfig("mediadbfile", NULL), ".genre", 0, 0);
	writemediadbcategory(tmpstr);
	free(tmpstr); tmpstr = NULL;
	freemediadbcategory(0);

	//create az
	int i = 0;
	char* tmpbuf = malloc(2);
	if(tmpbuf != NULL)
	{
		for(i = 65; i < 91; i++)
		{
			snprintf(tmpbuf, 2, "%c", i);
			tmpstr = ostrcat("0#", tmpbuf, 0, 0);
			addmediadbcategory(tmpstr, 0, 1, NULL, 0);
			free(tmpstr); tmpstr = NULL;
		}
		for(i = 65; i < 91; i++)
		{
			snprintf(tmpbuf, 2, "%c", i);
			tmpstr = ostrcat("1#", tmpbuf, 0, 0);
			addmediadbcategory(tmpstr, 1, 1, NULL, 0);
			free(tmpstr); tmpstr = NULL;
		}
		for(i = 65; i < 91; i++)
		{
			snprintf(tmpbuf, 2, "%c", i);
			tmpstr = ostrcat("2#", tmpbuf, 0, 0);
			addmediadbcategory(tmpstr, 2, 1, NULL, 0);
			free(tmpstr); tmpstr = NULL;
		}
	}
	free(tmpbuf); tmpbuf = NULL;
	tmpstr = ostrcat(getconfig("mediadbfile", NULL), ".az", 0, 0);
	writemediadbcategory(tmpstr);
	free(tmpstr); tmpstr = NULL;
	freemediadbcategory(0);

	status.mediadbthread = NULL;
	status.mediadbthreadstatus = 0;
	debug(777, "mediadb scanthread end");
}

void mediadbfindfilecb(char* path, char* file, int type)
{
	int treffer = 0;
	char* tmpstr = NULL;
	struct mediadb *node = mediadb;
	
	tmpstr = ostrcat(path, "/", 0, 0);
	tmpstr = ostrcat(tmpstr, file, 1, 0);

	//check if entry exist
	while(node != NULL)
	{
		if(ostrcmp(node->file, tmpstr) == 0)
		{
			treffer = 1;
			break;
		}
		node = node->next;
	}

	int tout = getconfigint("mediadbscantimeout", NULL);

	if(treffer == 0 || (treffer == 1 && node != NULL && tout > 0 && time(NULL) > node->timestamp + (tout * 86400)))
	{
		if(type == 0)
		{
			struct imdb* imdb = NULL;
			struct imdbapi* imdbapi = NULL;
			struct tmdb* tmdb = NULL;

			//create imdb search name
			char* shortname = ostrcat(file, NULL, 0, 0);
			string_tolower(shortname);
			shortname = string_shortname(shortname, 1);
			shortname = string_shortname(shortname, 2);
			string_removechar(shortname);
			strstrip(shortname);

			//TODO: got imdb infos

#ifdef SIMULATE
			imdb = getimdb(shortname, 0, 1, 0);
#else
			struct skin* imdbplugin = getplugin("IMDb");
			if(imdbplugin != NULL)
			{
				struct imdb* (*startplugin)(char*, int, int, int);
				startplugin = dlsym(imdbplugin->pluginhandle, "getimdb");
				if(startplugin != NULL)
					imdb = startplugin(shortname, 0, 1, 0);
			}
#endif

#ifdef SIMULATE
			if(imdb == NULL)
				imdbapi = getimdbapi(shortname, 0, 1, 0);
			else
				imdbapi = getimdbapi(imdb->id, 1, 1, 0);
#else
			struct skin* imdbapiplugin = getplugin("IMDb-API");
			if(imdbplugin != NULL)
			{
				struct imdbapi* (*startplugin)(char*, int, int, int);
				startplugin = dlsym(imdbapiplugin->pluginhandle, "getimdbapi");
				if(startplugin != NULL)
				{
					if(imdb == NULL)
						imdbapi = startplugin(shortname, 0, 1, 0);
					else
						imdbapi = startplugin(imdb->id, 1, 1, 0);
				}
			}
#endif

#ifdef SIMULATE
			if(imdb != NULL)
				tmdb = gettmdb(imdb->id, 1, 1, 0);
			else if(imdbapi != NULL)
				tmdb = gettmdb(imdbapi->id, 1, 1, 0);
#else
			struct skin* tmdbplugin = NULL;
			if(imdb != NULL)
			{
				tmdbplugin = getplugin("TMDb");
				if(tmdbplugin != NULL)
				{
					struct tmdb* (*startplugin)(char*, int, int, int);
					startplugin = dlsym(tmdbplugin->pluginhandle, "gettmdb");
					if(startplugin != NULL)
					{
						if(imdb != NULL)
							tmdb = startplugin(imdb->id, 1, 1, 0);
						else if(imdbapi != NULL)
							tmdb = startplugin(imdbapi->id, 1, 1, 0);
					}
				}
			}
#endif

printf("wo1\n");

			if(imdb != NULL && tmdb != NULL)
			{
				if(imdb->id == NULL) ostrcat(imdb->id, tmdb->imdbid, 1, 0);			
				if(imdb->title == NULL) ostrcat(imdb->title, tmdb->title, 1, 0);	
				if(imdb->genre == NULL) ostrcat(imdb->genre, tmdb->genre, 1, 0);
//				if(imdb->writer == NULL) ostrcat(imdb->writer, tmdb->writer, 1, 0);
//				if(imdb->director == NULL) ostrcat(imdb->director, tmdb->director, 1, 0);
//				if(imdb->actors == NULL) ostrcat(imdb->actors, tmdb->actors, 1, 0);
				if(imdb->rating == NULL) ostrcat(imdb->rating, tmdb->rating, 1, 0);
				if(imdb->votes == NULL) ostrcat(imdb->votes, tmdb->votes, 1, 0);
				if(imdb->runtime == NULL) ostrcat(imdb->runtime, tmdb->runtime, 1, 0);
				if(imdb->plot == NULL) ostrcat(imdb->plot, tmdb->plot, 1, 0);
				if(imdb->released == NULL) ostrcat(imdb->released, tmdb->released, 1, 0);
				if(imdb->poster == NULL) ostrcat(imdb->poster, tmdb->postermid, 1, 0);
				if(imdb->thumb == NULL) ostrcat(imdb->thumb, tmdb->thumb, 1, 0);
			}

			if(imdb != NULL && imdbapi != NULL)
			{
				if(imdb->id == NULL) ostrcat(imdb->id, imdbapi->id, 1, 0);			
				if(imdb->title == NULL) ostrcat(imdb->title, imdbapi->title, 1, 0);	
				if(imdb->genre == NULL) ostrcat(imdb->genre, imdbapi->genre, 1, 0);
				if(imdb->writer == NULL) ostrcat(imdb->writer, imdbapi->writer, 1, 0);
				if(imdb->director == NULL) ostrcat(imdb->director, imdbapi->director, 1, 0);
				if(imdb->actors == NULL) ostrcat(imdb->actors, imdbapi->actors, 1, 0);
				if(imdb->rating == NULL) ostrcat(imdb->rating, imdbapi->rating, 1, 0);
				if(imdb->votes == NULL) ostrcat(imdb->votes, imdbapi->votes, 1, 0);
				if(imdb->runtime == NULL) ostrcat(imdb->runtime, imdbapi->runtime, 1, 0);
				if(imdb->plot == NULL) ostrcat(imdb->plot, imdbapi->plot, 1, 0);
				if(imdb->released == NULL) ostrcat(imdb->released, imdbapi->released, 1, 0);
				if(imdb->poster == NULL) ostrcat(imdb->poster, imdbapi->poster, 1, 0);
//				if(imdb->thumb == NULL) ostrcat(imdb->thumb, imdbapi->thumb, 1, 0);
			}
/*
			if(imdb != NULL && tmdb != NULL && ostrcmp(tmdb->title, "Nothing found.") != 0)
			{
				free(imdb->title);
				imdb->title = ostrcat(tmdb->title, NULL, 0, 0);
				free(imdb->plot);
				imdb->plot = ostrcat(tmdb->plot, NULL, 0, 0);			
				free(imdb->plot);
				imdb->plot = ostrcat(tmdb->plot, NULL, 0, 0);
			}
*/
printf("wo2\n");
			debug(777, "shortname: %s", shortname);
			free(shortname); shortname = NULL;

			debug(133, "use id: %s", imdbapi->id);
			debug(133, "use title: %s", imdbapi->title);
			debug(133, "use genre: %s", imdbapi->genre);
			debug(133, "use writer: %s", imdbapi->writer);
			debug(133, "use director: %s", imdbapi->director);
			debug(133, "use released: %s", imdbapi->released);
			debug(133, "use actors: %s", imdbapi->actors);
			debug(133, "use plot: %s", imdbapi->plot);
			debug(133, "use poster: %s", imdbapi->poster);
			debug(133, "use rating: %s", imdbapi->rating);
			debug(133, "use votes: %s", imdbapi->votes);
			debug(133, "use runtime: %s", imdbapi->runtime);
			debug(133, "use year: %s", imdbapi->year);
			debug(133, "use rated: %s", imdbapi->rated);
		
			debug(777, "add video: %s", tmpstr);
			if(imdb != NULL)
			{
printf("wo2.1\n");
				debug(777, "imdb id %s", imdb->id);
printf("wo2.2\n");

				createmediadb(node, imdb->id, type, imdb->title, imdb->year, imdb->released, imdb->runtime, imdb->genre, imdb->director, imdb->writer, imdb->actors, imdb->plot, imdb->id, imdb->rating, imdb->votes, tmpstr, file, 0);
printf("wo2.3\n");

			}
			else
				createmediadb(node, NULL, type, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, tmpstr, file, 0);
printf("wo3\n");

#ifdef SIMULATE
			freeimdb(imdb);
#else
			if(imdbplugin != NULL)
			{
				void (*startplugin)(struct imdb*);
				startplugin = dlsym(imdbplugin->pluginhandle, "freeimdb");
				if(startplugin != NULL)
					startplugin(imdb);
			}
#endif
			imdb = NULL;
printf("wo4\n");			
#ifdef SIMULATE
			freeimdbapi(imdbapi);
#else
			if(imdbapiplugin != NULL)
			{
				void (*startplugin)(struct imdbapi*);
				startplugin = dlsym(imdbapiplugin->pluginhandle, "freeimdbapi");
				if(startplugin != NULL)
					startplugin(imdbapi);
			}
#endif
			imdbapi = NULL;
printf("wo5\n");

#ifdef SIMULATE
			freetmdb(tmdb);
#else
			if(tmdbplugin != NULL)
			{
				void (*startplugin)(struct tmdb*);
				startplugin = dlsym(tmdbplugin->pluginhandle, "freetmdb");
				if(startplugin != NULL)
					startplugin(tmdb);
			}
#endif

			tmdb = NULL;
printf("wo6\n");
		}
		else if(type == 1)
		{
			debug(777, "add audio: %s", tmpstr);
			createmediadb(node, NULL, type, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, tmpstr, file, 0);
		}
		else if(type == 2)
		{
			char* thumbfile = NULL;

			debug(777, "add pic: %s", tmpstr);
			if(status.createthumb == 1)
			{
				//check if thumb exists
				thumbfile = checkthumb(path, file);
				if(thumbfile == NULL)
				{
					thumbfile = ostrcat(path, "/.Thumbnails/", 0, 0);
					thumbfile = ostrcat(thumbfile, file, 1, 0);
					addqueue(101, (void*)path, strlen(path) + 1, (void*)file, strlen(file) + 1, 0, NULL);
					thumbthread(NULL);
				}
			}

			createmediadb(node, NULL, type, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, thumbfile, NULL, NULL, tmpstr, file, 0);
			free(thumbfile); thumbfile = NULL;
		}
	}
	free(tmpstr); tmpstr = NULL;
}

// flag = 0 (rekursive deaktive)
// flag = 1 (rekursive active)
// flag1 = 0 (scan files and start)
// flag1 = 1 (count files)
int findfiles(char* dirname, int type, int flag, int flag1)
{
	debug(777, "dir=%s type=%d\n", dirname, type);
	DIR *d;
	//Open the directory specified by dirname
	d = opendir(dirname);

	//Check it was opened
	if(! d)
	{
		perr("Cannot open directory %s", dirname);
		return 1;
	}

	int count = 0;

	while(1)
	{
		struct dirent* entry;
		int path_length;
		char path[PATH_MAX];

		snprintf(path, PATH_MAX, "%s", dirname);
		//Readdir gets subsequent entries from d
		entry = readdir(d);

		if(!entry) //There are no more entries in this directory, so break out of the while loop
			break;

		//See if entry is a subdirectory of d
		if(entry->d_type & DT_DIR)
		{
			//Check that the directory is not d or d's parent
			//if(strcmp(entry->d_name, "..") != 0 && strcmp (entry->d_name, ".") != 0)
			if(entry->d_name != NULL && entry->d_name[0] != '.')
			{
				path_length = snprintf(path, PATH_MAX, "%s/%s", dirname, entry->d_name);
				if(path_length >= PATH_MAX)
				{
					err("path length has got too long");
					return 1;
				}
				//Recursively call findfiles with the new path
				if(flag == 0)
					findfiles(path, type, 1, 0);
			}
		}
		else //File
		{
			//TODO: add extensions
			//video
			if(!filelistflt("*.avi *.mkv", entry->d_name))
			{
				if(type == 0 || type == 100 || type == 90 || type == 91)
				{
					if(flag1 == 0)
						mediadbfindfilecb(path, entry->d_name, 0);
					else
						count += 1;
				}
			}
			//audio
			if(!filelistflt("*.mp3", entry->d_name))
			{
				if(type == 1 || type == 100 || type == 90 || type == 92)
				{
					if(flag1 == 0)
						mediadbfindfilecb(path, entry->d_name, 1);
					else
						count += 1;
				}
			}
			//picture
			if(!filelistflt("*.jpg", entry->d_name))
			{
				if(type == 2 || type == 100 || type == 91 || type == 92)
				{
					if(flag1 == 0)
						mediadbfindfilecb(path, entry->d_name, 2);
					else
						count += 1;
				}
			}
		}
	}

	//After going through all the entries, close the directory
	if(closedir(d))
	{
		perr("Could not close %s", dirname);
		return 1;
	}

	if(flag1 == 1)
		return count;
	return 0;
}

//type 0=video, 1=audio, 2=pic, 90=video/audio, 91=video/pic, 92=audio/pic, 100=all
void mediadbscan(char* path, int type, int flag)
{
	int count = 0;

	if(flag == 1)
		type = 1000;

	addtimer(&mediadbscanthread, START, 1000, 1, (void*)path, (void*)type, NULL);

	//block a little
	while(status.mediadbthread != NULL && count < 20)
	{
		usleep(100000);
		count++;
	}
}

#endif
