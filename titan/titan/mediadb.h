#ifndef MEDIADB_H
#define MEDIADB_H

void debugimdbnode(struct imdb* node)
{
  if(node != NULL)
  {
  	debug(133, "----------------------mediadb start----------------------");
  	debug(133, "use id: %s", node->id);
  	debug(133, "use title: %s", node->title);
  	debug(133, "use genre: %s", node->genre);
  	debug(133, "use writer: %s", node->writer);
  	debug(133, "use director: %s", node->director);
  	debug(133, "use released: %s", node->released);
  	debug(133, "use actors: %s", node->actors);
  	debug(133, "use plot: %s", node->plot);
  	debug(133, "use poster: %s", node->poster);
  	debug(133, "use rating: %s", node->rating);
  	debug(133, "use votes: %s", node->votes);
  	debug(133, "use runtime: %s", node->runtime);
  	debug(133, "use year: %s", node->year);
  	debug(133, "use rated: %s", node->rated);
  	debug(133, "----------------------mediadb end----------------------");
  }
}

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
	status.writemediadb = 1;
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
	status.writemediadb = 1;
	return 0;
}

struct mediadbfilter* addmediadbfilter(struct mediadb* mnode, int nr, int count, struct mediadbfilter* last)
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
	newnode->count = nr;

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

int addmediadbcontent(struct mediadb* node, char *line, int len, int count)
{
	int ret = 0;
	char* tmpstr = NULL, *type = NULL, *year = NULL, *rating = NULL;
	char* votes = NULL, *timestamp = NULL, *flag = NULL;

	if(node == NULL) return 1;

	if(len > 0) tmpstr = malloc(len + 1);
	if(tmpstr != NULL)
	{
		memcpy(tmpstr, line, len);
		tmpstr[len] = '\0';

		node->id = tmpstr;

		while(tmpstr[0] != '\0')
		{
			if(tmpstr[0] == '#')
			{
				tmpstr[0] = '\0';
				tmpstr++;
				switch(ret)
				{
					case 0: type = tmpstr; break;
					case 1: node->title = tmpstr; break;
					case 2: year = tmpstr; break;
					case 3: node->released = tmpstr; break;
					case 4: node->runtime = tmpstr; break;
					case 5: node->genre = tmpstr; break;
					case 6: node->director = tmpstr; break;
					case 7: node->writer = tmpstr; break;
					case 8: node->actors = tmpstr; break;
					case 9: node->plot = tmpstr; break;
					case 10: node->poster = tmpstr; break;
					case 11: rating = tmpstr; break;
					case 12: votes = tmpstr; break;
					case 13: node->path = tmpstr; break;
					case 14: node->file = tmpstr; break;
					case 15: timestamp = tmpstr; break;
					case 16: flag = tmpstr; break;
				}

				ret++;
			}
			else
				tmpstr++;
		}
	}

	if(ret != 17)
	{
		if(count > 0)
		{
			err("mediadb line %d not ok (ret=%d)", count, ret);
		}
		else
		{
			err("add mediadb (ret=%d)", ret);
		}
		freemediadbcontent(node);
		return 1;
	}

	if(type != NULL) node->type = atoi(type);
	if(year != NULL) node->year = atoi(year);
	if(rating != NULL) node->rating = atoi(rating);
	if(votes != NULL) node->votes = atoi(votes);
	if(timestamp != NULL) node->timestamp = strtoul(timestamp, NULL, 10);
	if(flag != NULL) node->flag = atoi(flag);

	return 0;
}

//flag 0: with lock
//flag 1: without lock
struct mediadb* addmediadb(char *line, int len, int count, struct mediadb* last, int sort, int flag)
{
	//debug(1000, "in");
	struct mediadb *newnode = NULL, *prev = NULL, *node = NULL;
	int ret = 0;

	if(line == NULL) return NULL;

	newnode = (struct mediadb*)calloc(1, sizeof(struct mediadb));
	if(newnode == NULL)
	{
		err("no memory");
		return NULL;
	}

	ret = addmediadbcontent(newnode, line, len, count);
	if(ret == 1)
	{
		free(newnode);
		return NULL;
	}

	if(flag == 0) m_lock(&status.mediadbmutex, 17);
	node = mediadb;

	status.writemediadb = 1;
	modifymediadbcache(newnode->path, newnode->file, newnode);

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

	if(flag == 0) m_unlock(&status.mediadbmutex, 17);
	//debug(1000, "out");
	return newnode;
}

struct mediadb* createmediadb(struct mediadb* update, char* id, int type, char* title, char* year, char* released, char* runtime, char* genre, char* director, char* writer, char* actors, char* plot, char* poster, char* rating, char* votes, char* path, char* file, int flag)
{
	struct mediadb* mnode = NULL;
	char* tmpstr = NULL;

	if(path == NULL || file == NULL) return NULL;
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
	path = stringreplacechar(path, '#', ' ');
	file = stringreplacechar(file, '#', ' ');

	tmpstr = ostrcat(tmpstr, id, 1, 0);
	tmpstr = ostrcat(tmpstr, "#", 1, 0);
	tmpstr = ostrcat(tmpstr, oitoa(type), 1, 1);
	tmpstr = ostrcat(tmpstr, "#", 1, 0);
	tmpstr = ostrcat(tmpstr, title, 1, 0);
	tmpstr = ostrcat(tmpstr, "#", 1, 0);
	if(year != NULL) tmpstr = ostrcat(tmpstr, oitoa(atoi(year)), 1, 1);
	tmpstr = ostrcat(tmpstr, "#", 1, 0);
	tmpstr = ostrcat(tmpstr, released, 1, 0);
	tmpstr = ostrcat(tmpstr, "#", 1, 0);
	tmpstr = ostrcat(tmpstr, runtime, 1, 0);
	tmpstr = ostrcat(tmpstr, "#", 1, 0);
	tmpstr = ostrcat(tmpstr, genre, 1, 0);
	tmpstr = ostrcat(tmpstr, "#", 1, 0);
	tmpstr = ostrcat(tmpstr, director, 1, 0);
	tmpstr = ostrcat(tmpstr, "#", 1, 0);
	tmpstr = ostrcat(tmpstr, writer, 1, 0);
	tmpstr = ostrcat(tmpstr, "#", 1, 0);
	tmpstr = ostrcat(tmpstr, actors, 1, 0);
	tmpstr = ostrcat(tmpstr, "#", 1, 0);
	tmpstr = ostrcat(tmpstr, plot, 1, 0);
	tmpstr = ostrcat(tmpstr, "#", 1, 0);
	tmpstr = ostrcat(tmpstr, poster, 1, 0);
	tmpstr = ostrcat(tmpstr, "#", 1, 0);
	if(rating != NULL) tmpstr = ostrcat(tmpstr, oitoa(atoi(rating)), 1, 1);
	tmpstr = ostrcat(tmpstr, "#", 1, 0);
	if(votes != NULL) tmpstr = ostrcat(tmpstr, oitoa(atoi(votes)), 1, 1);
	tmpstr = ostrcat(tmpstr, "#", 1, 0);
	tmpstr = ostrcat(tmpstr, path, 1, 0);
	tmpstr = ostrcat(tmpstr, "#", 1, 0);
	tmpstr = ostrcat(tmpstr, file, 1, 0);
	tmpstr = ostrcat(tmpstr, "#", 1, 0);
	tmpstr = ostrcat(tmpstr, olutoa(time(NULL)), 1, 1);
	tmpstr = ostrcat(tmpstr, "#", 1, 0);
	tmpstr = ostrcat(tmpstr, oitoa(flag), 1, 1);

	if(update != NULL)
	{
		m_lock(&status.mediadbmutex, 17);
		delmediadbcache(update->file, update);
		freemediadbcontent(update);
		addmediadbcontent(update, tmpstr, strlen(tmpstr), 1);
		modifymediadbcache(update->path, update->file, update);
		m_unlock(&status.mediadbmutex, 17);
		mnode = update;
	}
	else
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

	status.writemediadb = 0;

	free(fileline);
	fclose(fd);
	m_unlock(&status.mediadbmutex, 17);
	return 0;
}

void freemediadbcontent(struct mediadb* node)
{
	if(node == NULL) return;

	free(node->id); node->id = NULL;
	node->type = 0;
	node->title = NULL;
	node->year = 0;
	node->released = NULL;
	node->runtime = NULL;
	node->genre = NULL;
	node->director = NULL;
	node->writer = NULL;
	node->actors = NULL;
	node->plot = NULL;
	node->poster = NULL;
	node->rating = 0;
	node->votes = 0;
	node->file = NULL;
	node->timestamp = 0;
	node->flag = 0;
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
			status.writemediadb = 1;
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
			delmediadbcache(node->file, node);
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

	status.writemediadb = 0;
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
		ret = fprintf(fd, "%s#%d#%s#%d#%s#%s#%s#%s#%s#%s#%s#%s#%d#%d#%s#%s#%lu#%d\n", node->id, node->type, node->title, node->year, node->released, node->runtime, node->genre, node->director, node->writer, node->actors, node->plot, node->poster, node->rating, node->votes, node->path, node->file, node->timestamp, node->flag);

		if(ret < 0)
		{
			perr("writting file %s", filename);
		}
		node = node->next;
	}

	m_unlock(&status.mediadbmutex, 17);

	fclose(fd);
	status.writemediadb = 0;
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
//flag 8: no imdb info
int createmediadbfilter(int type, char* search, int flag)
{
	int isearch = 0, count = 0;
	struct mediadb* node = mediadb;
	struct mediadbfilter* last = NULL;

	if(status.mediadbthreadstatus == 1) return 0;

	if(flag == 1 || flag == 5)
	{
		if(search == NULL) return 0;
		isearch = atoi(search);
	}
	if(flag == 7 && search == NULL) return 0;

	freemediadbfilter(0);
	while(node != NULL)
	{
		if(node->type == type)
		{
			if(flag == 0)
			{
				last = addmediadbfilter(node, count, 1, last);
				count++;
			}
			else if(flag == 1 && node->year == isearch)
			{
				last = addmediadbfilter(node, count, 1, last);
				count++;
			}
			else if(flag == 2 && ostrstrcase(node->director, search) != NULL)
			{
				last = addmediadbfilter(node, count, 1, last);
				count++;
			}
			else if(flag == 3 && ostrstrcase(node->actors, search) != NULL)
			{
				last = addmediadbfilter(node, count, 1, last);
				count++;
			}
			else if(flag == 4 && ostrstrcase(node->file, search) != NULL)
			{
				last = addmediadbfilter(node, count, 1, last);
				count++;
			}
			else if(flag == 5 && node->rating == isearch)
			{
				last = addmediadbfilter(node, count, 1, last);
				count++;
			}
			else if(flag == 6 && ostrstrcase(node->genre, search) != NULL)
			{
				last = addmediadbfilter(node, count, 1, last);
				count++;
			}
			else if(flag == 7 && node->title != NULL && node->title[0] == search[0])
			{
				last = addmediadbfilter(node, count, 1, last);
				count++;
			}
			else if(flag == 8 && (node->id == NULL || strlen(node->id) == 0))
			{
				last = addmediadbfilter(node, count, 1, last);
				count++;
			}
		}
		node = node->next;
	}

	return count;
}

void mediadbscanthread(struct stimerthread* self, char* path, int type)
{
	struct mediadb *node = mediadb, *prev = mediadb;
	struct mediadbcategory *cnode = NULL;
	struct hdd *hddnode = NULL;
	struct splitstr* ret = NULL;
	char* tmpstr = NULL, *tmpsplit = NULL;

	if(status.mediadbthread != NULL || self == NULL)
	{
		free(path); path = NULL;
		return;
	}

	debug(777, "mediadb scanthread start");
	status.mediadbthreadstatus = 1;
	status.mediadbthread = self;

	if(type > 999)
	{
		type = type - 1000;
	
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
		
		while(count < 5)
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
			tmpstr = ostrcat(prev->path, "/", 0, 0);
			tmpstr = ostrcat(tmpstr, prev->file, 1, 0);
			tmpstr = addmountpart(tmpstr, 1);
			if(tmpstr == NULL)
			{
				//TODO: unlink all jpg to node
				delmediadb(prev, 0);
			}
			free(tmpstr); tmpstr = NULL;
		}
	}

	status.mediadbsavetime = time(NULL) + 60;
	status.mediadbthreadstatus = 2;

	//find media files
	if(path == NULL)
	{
		findfiles("/media/autofs", type);
		/*
		addhddall();
		hddnode = hdd;

		while(hddnode != NULL)
		{
			if(hddnode->partition != 0)
			{
				tmpstr = ostrcat("/autofs/", hddnode->device, 0, 0);
				findfiles(tmpstr, type);
				free(tmpstr); tmpstr = NULL;
			}
			hddnode = hddnode->next;
		}
		*/
	}
	else
		findfiles(path, type);

	free(path); path = NULL;

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
		tmpsplit = ostrcat(node->path, NULL, 0, 0);
		ret = strsplit(tmpsplit, "/", &count);
		
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
	char* shortpath = NULL, *tmpstr = NULL;
	struct mediadb *node = NULL;
	
	shortpath = delmountpart(path, 0);
	if(shortpath == NULL) return; //no mountpart found

	m_lock(&status.mediadbmutex, 17);
	node = mediadb;
	//check if entry exist
	while(node != NULL)
	{
		if(ostrcmp(node->file, file) == 0)
		{
			//check directory
			if(ostrcmp(shortpath, node->path) == 0) //same file
				break;

			//check file size
			tmpstr = ostrcat(path, "/", 0, 0);
			tmpstr = ostrcat(tmpstr, file, 1, 0);
			off64_t s1 = getfilesize(tmpstr);
			free(tmpstr); tmpstr = NULL;
			
			tmpstr = ostrcat(node->path, "/", 0, 0);
			tmpstr = ostrcat(tmpstr, node->file, 1, 0);
			tmpstr = addmountpart(tmpstr, 1);
			off64_t s2 = getfilesize(tmpstr);
			free(tmpstr); tmpstr = NULL;
			
			if(s1 == s2) //seems the same file
				break;
		}
		node = node->next;
	}
	m_unlock(&status.mediadbmutex, 17);

	int tout = getconfigint("mediadbscantimeout", NULL);

	if(node == NULL || (node != NULL && checkbit(node->flag, 31) == 0 && tout == 0) || (node != NULL && checkbit(node->flag, 31) == 0 && time(NULL) > node->timestamp + (tout * 86400)))
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

			//got imdb infos
			struct skin* imdbplugin = getplugin("IMDb");
			if(imdbplugin != NULL)
			{
				struct imdb* (*startplugin)(struct imdb**, char*, int, int, int);
				startplugin = dlsym(imdbplugin->pluginhandle, "getimdb");
				if(startplugin != NULL)
					imdb = startplugin(&imdb, shortname, 0, 1, 0);
			}

			struct skin* imdbapiplugin = getplugin("IMDb-API");
			if(imdbplugin != NULL)
			{
				struct imdbapi* (*startplugin)(struct imdbapi**, char*, int, int);
				startplugin = dlsym(imdbapiplugin->pluginhandle, "getimdbapi");
				if(startplugin != NULL)
				{
					if(imdb == NULL)
						imdbapi = startplugin(&imdbapi, shortname, 0, 1);
					else if(imdb->id != NULL)
						imdbapi = startplugin(&imdbapi, imdb->id, 1, 1);
				}
			}

			struct skin* tmdbplugin = NULL;
			tmdbplugin = getplugin("TMDb");
			if(tmdbplugin != NULL)
			{
				struct tmdb* (*startplugin)(struct tmdb**, char*, int, int);
				startplugin = dlsym(tmdbplugin->pluginhandle, "gettmdb");
				if(startplugin != NULL)
				{
					if(imdb != NULL && imdb->id != NULL)
						tmdb = startplugin(&tmdb, imdb->id, 1, 1);
					else if(imdbapi != NULL && imdbapi->id != NULL)
						tmdb = startplugin(&tmdb, imdbapi->id, 1, 1);
				}
			}

      debugimdbnode(imdb);
			
			if(imdb != NULL && tmdb != NULL)
			{
				if(imdb->id == NULL) imdb->id = ostrcat(imdb->id, tmdb->imdbid, 1, 0);			
				if(imdb->title == NULL) imdb->title = ostrcat(imdb->title, tmdb->title, 1, 0);	
				if(imdb->genre == NULL) imdb->genre = ostrcat(imdb->genre, tmdb->genre, 1, 0);
//				if(imdb->writer == NULL) imdb->writer = ostrcat(imdb->writer, tmdb->writer, 1, 0);
//				if(imdb->director == NULL) imdb->director = ostrcat(imdb->director, tmdb->director, 1, 0);
//				if(imdb->actors == NULL) imdb->actors = ostrcat(imdb->actors, tmdb->actors, 1, 0);
				if(imdb->rating == NULL) imdb->rating = ostrcat(imdb->rating, tmdb->rating, 1, 0);
				if(imdb->votes == NULL) imdb->votes = ostrcat(imdb->votes, tmdb->votes, 1, 0);
				if(imdb->runtime == NULL) imdb->runtime = ostrcat(imdb->runtime, tmdb->runtime, 1, 0);
				if(imdb->plot == NULL) imdb->plot = ostrcat(imdb->plot, tmdb->plot, 1, 0);
				if(imdb->released == NULL) imdb->released = ostrcat(imdb->released, tmdb->released, 1, 0);
				if(imdb->poster == NULL) imdb->poster = ostrcat(imdb->poster, tmdb->postermid, 1, 0);
				if(imdb->thumb == NULL) imdb->thumb = ostrcat(imdb->thumb, tmdb->thumb, 1, 0);
				if(imdb->year == NULL) imdb->year = ostrcat(imdb->year, tmdb->year, 1, 0);				
			}

      debugimdbnode(imdb);
      
			if(imdb != NULL && imdbapi != NULL)
			{
				if(imdb->id == NULL) imdb->id = ostrcat(imdb->id, imdbapi->id, 1, 0);			
				if(imdb->title == NULL) imdb->title = ostrcat(imdb->title, imdbapi->title, 1, 0);	
				if(imdb->genre == NULL) imdb->genre = ostrcat(imdb->genre, imdbapi->genre, 1, 0);
				if(imdb->writer == NULL) imdb->writer = ostrcat(imdb->writer, imdbapi->writer, 1, 0);
				if(imdb->director == NULL) imdb->director = ostrcat(imdb->director, imdbapi->director, 1, 0);
				if(imdb->actors == NULL) imdb->actors = ostrcat(imdb->actors, imdbapi->actors, 1, 0);
				if(imdb->rating == NULL) imdb->rating = ostrcat(imdb->rating, imdbapi->rating, 1, 0);
				if(imdb->votes == NULL) imdb->votes = ostrcat(imdb->votes, imdbapi->votes, 1, 0);
				if(imdb->runtime == NULL) imdb->runtime = ostrcat(imdb->runtime, imdbapi->runtime, 1, 0);
				if(imdb->plot == NULL) imdb->plot = ostrcat(imdb->plot, imdbapi->plot, 1, 0);
				if(imdb->released == NULL) imdb->released = ostrcat(imdb->released, imdbapi->released, 1, 0);
				if(imdb->poster == NULL) imdb->poster = ostrcat(imdb->poster, imdbapi->poster, 1, 0);
//				if(imdb->thumb == NULL) imdb->thumb = ostrcat(imdb->thumb, imdbapi->thumb, 1, 0);
				if(imdb->year == NULL) imdb->year = ostrcat(imdb->year, imdbapi->year, 1, 0);
			}

			debug(777, "shortname: %s", shortname);
			free(shortname); shortname = NULL;
			
      debugimdbnode(imdb);
			
			debug(777, "add video: %s/%s", shortpath, file);
			if(imdb != NULL)
			{
				debug(777, "imdb id %s", imdb->id);
				createmediadb(node, imdb->id, type, imdb->title, imdb->year, imdb->released, imdb->runtime, imdb->genre, imdb->director, imdb->writer, imdb->actors, imdb->plot, imdb->id, imdb->rating, imdb->votes, shortpath, file, 0);
			}
			else
				createmediadb(node, NULL, type, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, shortpath, file, 0);

			if(imdbplugin != NULL)
			{
				void (*startplugin)(struct imdb**, int flag);
				startplugin = dlsym(imdbplugin->pluginhandle, "freeimdb");
				if(startplugin != NULL)
					startplugin(&imdb, 0);
			}
			imdb = NULL;
			
			if(imdbapiplugin != NULL)
			{
				void (*startplugin)(struct imdbapi**, int);
				startplugin = dlsym(imdbapiplugin->pluginhandle, "freeimdbapi");
				if(startplugin != NULL)
					startplugin(&imdbapi, 0);
			}
			imdbapi = NULL;

			if(tmdbplugin != NULL)
			{
				void (*startplugin)(struct tmdb**, int);
				startplugin = dlsym(tmdbplugin->pluginhandle, "freetmdb");
				if(startplugin != NULL)
					startplugin(&tmdb, 0);
			}
			tmdb = NULL;
		}
		else if(type == 1)
		{
			debug(777, "add audio: %s/%s", shortpath, file);
			createmediadb(node, NULL, type, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, shortpath, file, 0);
		}
		else if(type == 2)
		{
			char* thumbfile = NULL;

			debug(777, "add pic: %s/%s", shortpath, file);
			if(status.thumbthread != NULL)
			{
				//check if thumb exists
				thumbfile = checkthumb(path, file);
				if(thumbfile == NULL)
					addqueue(101, (void*)path, strlen(path) + 1, (void*)file, strlen(file) + 1, 0, NULL);
			}
      free(thumbfile); thumbfile = NULL;

			createmediadb(node, NULL, type, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, shortpath, file, 0);
		}
	}
	free(shortpath); shortpath = NULL;

	//save mediadb all 60 sek
	if(status.mediadbsavetime < time(NULL))
	{
		writemediadb(getconfig("mediadbfile", NULL));
		status.mediadbsavetime = time(NULL) + 60;
	}
}

//flag: bit 31 = 0 (rekursive), 1 (no recursive)
//flag: bit 30 = 0 (scan files and start), 1 (count files)
int findfiles(char* dirname, int flag)
{
	int type = flag;
	int onlydir = checkbit(flag, 31);
	int onlycount = checkbit(flag, 30);

	type = clearbit(type, 31);
	type = clearbit(type, 30);

	debug(777, "dir=%s type=%d onlydir=%d, onlycount=%d\n", dirname, type, onlydir, onlycount);
	DIR *d;
	//Open the directory specified by dirname
	d = opendir(dirname);

	//Check it was opened
	if(! d)
	{
		perr("Cannot open directory %s", dirname);
		return -1;
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
			if(entry->d_name != NULL && entry->d_name[0] != '.' && entry->d_name[0] != '$')
			{
				path_length = snprintf(path, PATH_MAX, "%s/%s", dirname, entry->d_name);
				if(path_length >= PATH_MAX)
				{
					err("path length has got too long");
					return -1;
				}
				//Recursively call findfiles with the new path
				if(onlydir == 0)
					findfiles(path, type);
			}
		}
		else //File
		{
			//TODO: add extensions
			if(!filelistflt(".avi .mkv", entry->d_name)) //video
			{
				if(type == 0 || type == 100 || type == 90 || type == 91)
				{
					if(onlycount == 0)
						mediadbfindfilecb(path, entry->d_name, 0);
					else
						count += 1;
				}
			}
			else if(!filelistflt(".mp3", entry->d_name)) //audio
			{
				if(type == 1 || type == 100 || type == 90 || type == 92)
				{
					if(onlycount == 0)
						mediadbfindfilecb(path, entry->d_name, 1);
					else
						count += 1;
				}
			}
			else if(!filelistflt(".jpg", entry->d_name)) //picture
			{
				if(type == 2 || type == 100 || type == 91 || type == 92)
				{
					if(onlycount == 0)
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
		return -1;
	}

	if(onlycount == 1)
		return count;
	return 0;
}

//type 0=video, 1=audio, 2=pic, 90=video/audio, 91=video/pic, 92=audio/pic, 100=all
//flag: 0 = scan recursive
//flag: 1 = not scan recursive
void mediadbscan(char* path, int type, int flag)
{
	int count = 0;

	if(flag == 1) type = type | 0x80000000;

	//param1 (path) is freed in thread
	addtimer(&mediadbscanthread, START, 1000, 1, (void*)ostrcat(path, NULL, 0, 0), (void*)type, NULL);

	//block a little
	while(status.mediadbthread != NULL && count < 20)
	{
		usleep(100000);
		count++;
	}
}

#endif
