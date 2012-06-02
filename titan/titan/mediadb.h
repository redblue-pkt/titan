#ifndef MEDIADB_H
#define MEDIADB_H

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
	struct mediadbfilter *newnode = NULL, *prev = NULL, *node = mediadbfilter;

	if(mnode == NULL) return NULL;

	newnode = (struct mediadbfilter*)malloc(sizeof(struct mediadbfilter));
	if(newnode == NULL)
	{
		err("no memory");
		return NULL;
	}

	memset(newnode, 0, sizeof(struct mediadbfilter));
	newnode->node = mnode;

	m_lock(&status.mediadbmutex, 17);

	if(last == NULL)
	{
		while(node != NULL && strcasecmp(newnode->node->title, node->node->title) > 0)
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
	struct mediadbcategory *newnode = NULL, *prev = NULL, *node = mediadbcategory;
	char* name = NULL;
	int ret = 0;

	if(line == NULL) return NULL;

	newnode = (struct mediadbcategory*)malloc(sizeof(struct mediadbcategory));
	if(newnode == NULL)
	{
		err("no memory");
		return NULL;
	}

	memset(newnode, 0, sizeof(struct mediadbcategory));
	
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

//flag 0: with lock
//flag 1: without lock
struct mediadb* addmediadb(char *line, int count, struct mediadb* last, int flag)
{
	//debug(1000, "in");
	struct mediadb *newnode = NULL, *prev = NULL, *node = mediadb;
	char *id = NULL, *title = NULL, *year = NULL, *released = NULL;
	char *runtime = NULL, *genre = NULL, *director = NULL, *writer = NULL;
	char *actors = NULL, *plot = NULL, *poster = NULL, *rating = NULL;
	char *votes = NULL, *file = NULL;
	int ret = 0;

	if(line == NULL) return NULL;

	newnode = (struct mediadb*)malloc(sizeof(struct mediadb));
	if(newnode == NULL)
	{
		err("no memory");
		return NULL;
	}

	id = malloc(256);
	if(id == NULL)
	{
		err("no memory");
		free(newnode);
		return NULL;
	}
	title = malloc(256);
	if(title == NULL)
	{
		err("no memory");
		freemediadbcontent(newnode);
		free(newnode);
		return NULL;
	}
	year = malloc(256);
	if(year == NULL)
	{
		err("no memory");
		freemediadbcontent(newnode);
		free(newnode);
		return NULL;
	}
	released = malloc(256);
	if(released == NULL)
	{
		err("no memory");
		freemediadbcontent(newnode);
		free(newnode);
		return NULL;
	}
	runtime = malloc(256);
	if(runtime == NULL)
	{
		err("no memory");
		freemediadbcontent(newnode);
		free(newnode);
		return NULL;
	}
	genre = malloc(256);
	if(genre == NULL)
	{
		err("no memory");
		freemediadbcontent(newnode);
		free(newnode);
		return NULL;
	}
	director = malloc(256);
	if(director == NULL)
	{
		err("no memory");
		freemediadbcontent(newnode);
		free(newnode);
		return NULL;
	}
	writer = malloc(256);
	if(writer == NULL)
	{
		err("no memory");
		freemediadbcontent(newnode);
		free(newnode);
		return NULL;
	}
	actors = malloc(256);
	if(actors == NULL)
	{
		err("no memory");
		freemediadbcontent(newnode);
		free(newnode);
		return NULL;
	}
	plot = malloc(MINMALLOC);
	if(plot == NULL)
	{
		err("no memory");
		freemediadbcontent(newnode);
		free(newnode);
		return NULL;
	}
	poster = malloc(256);
	if(poster == NULL)
	{
		err("no memory");
		freemediadbcontent(newnode);
		free(newnode);
		return NULL;
	}
	rating = malloc(256);
	if(rating == NULL)
	{
		err("no memory");
		freemediadbcontent(newnode);
		free(newnode);
		return NULL;
	}
	votes = malloc(256);
	if(votes == NULL)
	{
		err("no memory");
		freemediadbcontent(newnode);
		free(newnode);
		return NULL;
	}
	file = malloc(256);
	if(file == NULL)
	{
		err("no memory");
		freemediadbcontent(newnode);
		free(newnode);
		return NULL;
	}

	memset(newnode, 0, sizeof(struct mediadb));

	ret = sscanf(line, "%[^#]#%d#%[^#]#%[^#]#%[^#]#%[^#]#%[^#]#%[^#]#%[^#]#%[^#]#%[^#]#%[^#]#%[^#]#%[^#]#%[^#]#%lu", id, &newnode->type, title, year, released, runtime, genre, director, writer, actors, plot, poster, rating, votes, file, &newnode->timestamp);

	if(ret != 16)
	{
		if(count > 0)
		{
			err("mediadb line %d not ok or double", count);
		}
		else
		{
			err("add mediadb");
		}
		freemediadbcontent(newnode);
		free(newnode);
		return NULL;
	}

	newnode->id = ostrcat(id, NULL, 1, 0);
	newnode->title = ostrcat(title, NULL, 1, 0);
	newnode->year = ostrcat(year, NULL, 1, 0);
	newnode->released = ostrcat(released, NULL, 1, 0);
	newnode->runtime = ostrcat(runtime, NULL, 1, 0);
	newnode->genre = ostrcat(genre, NULL, 1, 0);
	newnode->director = ostrcat(director, NULL, 1, 0);
	newnode->writer = ostrcat(writer, NULL, 1, 0);
	newnode->actors = ostrcat(actors, NULL, 1, 0);
	newnode->plot = ostrcat(plot, NULL, 1, 0);
	newnode->poster = ostrcat(poster, NULL, 1, 0);
	newnode->rating = ostrcat(rating, NULL, 1, 0);
	newnode->votes = ostrcat(votes, NULL, 1, 0);
	newnode->file = ostrcat(file, NULL, 1, 0);

	if(flag == 0) m_lock(&status.mediadbmutex, 17);

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

struct mediadb* createmediadb(char* id, int type, char* title, char* year, char* released, char* runtime, char* genre, char* director, char* writer, char* actors, char* plot, char* poster, char* rating, char* votes, char* fullfile, char* file)
{
	struct mediadb* mnode = NULL;
	char* tmpstr = NULL;

	if(fullfile == NULL) return;

	if(id == NULL) id = "0";
	if(file == NULL) file = "n/a";
	if(title == NULL) title = file;
	if(year == NULL) year = "1900";
	if(released == NULL) released = "1 Jan 1900";
	if(runtime == NULL) runtime = "1 h 30 min";
	if(genre == NULL) genre = "n/a";
	if(director == NULL) director = "n/a";
	if(writer == NULL) writer = "n/a";
	if(actors == NULL) actors = "n/a";
	if(plot == NULL) plot = "n/a";
	if(poster == NULL) poster = "0";
	if(rating == NULL) rating = "0";
	if(votes == NULL) votes = "0";

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
	rating = stringreplacechar(rating, '#', ' ');
	votes = stringreplacechar(votes, '#', ' ');
	fullfile = stringreplacechar(fullfile, '#', ' ');

	tmpstr = ostrcat(tmpstr, id, 1, 0);
	tmpstr = ostrcat(tmpstr, "#", 1, 0);
	tmpstr = ostrcat(tmpstr, oitoa(type), 1, 1);
	tmpstr = ostrcat(tmpstr, "#", 1, 0);
	tmpstr = ostrcat(tmpstr, title, 1, 0);
	tmpstr = ostrcat(tmpstr, "#", 1, 0);
	tmpstr = ostrcat(tmpstr, year, 1, 0);
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
	tmpstr = ostrcat(tmpstr, rating, 1, 0);
	tmpstr = ostrcat(tmpstr, "#", 1, 0);
	tmpstr = ostrcat(tmpstr, votes, 1, 0);
	tmpstr = ostrcat(tmpstr, "#", 1, 0);
	tmpstr = ostrcat(tmpstr, fullfile, 1, 0);
	tmpstr = ostrcat(tmpstr, "#", 1, 0);
	tmpstr = ostrcat(tmpstr, olutoa(time(NULL)), 1, 1);

	mnode = addmediadb(tmpstr, 1, NULL, 0);

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
	int linecount = 0;
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
		if(fileline[0] == '#' || fileline[0] == '\n')
			continue;
		if(fileline[strlen(fileline) - 1] == '\n')
			fileline[strlen(fileline) - 1] = '\0';
		if(fileline[strlen(fileline) - 1] == '\r')
			fileline[strlen(fileline) - 1] = '\0';

		linecount++;

		if(flag == 0)
		{
			if(last == NULL) last = tmplast;
			last = addmediadb(fileline, linecount, last, 1);
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
	return 0;
}

void freemediadbcontent(struct mediadb* node)
{
	if(node == NULL) return;

	free(node->id); node->id = NULL;
	free(node->title); node->title = NULL;
	free(node->year); node->year = NULL;
	free(node->released); node->released = NULL;
	free(node->runtime); node->runtime = NULL;
	free(node->genre); node->genre = NULL;
	free(node->director); node->director = NULL;
	free(node->writer); node->writer = NULL;
	free(node->actors); node->actors = NULL;
	free(node->plot); node->plot = NULL;
	free(node->poster); node->poster = NULL;
	free(node->rating); node->rating = NULL;
	free(node->votes); node->votes = NULL;
	free(node->file); node->file = NULL;
}

int delmediadbfilter(struct mediadbfilter* mnode, int flag)
{
	debug(1000, "in");
	int ret = 1;
	struct mediadbfilter *node = mediadbfilter, *prev = mediadbfilter;

	m_lock(&status.mediadbmutex, 17);

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

	m_unlock(&status.mediadbmutex, 17);
	debug(1000, "out");
	return ret;
}

int delmediadbcategory(struct mediadbcategory* mnode, int flag)
{
	debug(1000, "in");
	int ret = 1;
	struct mediadbcategory *node = mediadbcategory, *prev = mediadbcategory;

	m_lock(&status.mediadbmutex, 17);

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
	struct mediadb *node = mediadb, *prev = mediadb;

	m_lock(&status.mediadbmutex, 17);

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
	struct mediadbcategory *node = mediadbcategory;
	int ret = 0;

	fd = fopen(filename, "w");
	if(fd == NULL)
	{
		perr("can't open %s", filename);
		return 1;
	}

	m_lock(&status.mediadbmutex, 17);

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
	struct mediadb *node = mediadb;
	int ret = 0;

	fd = fopen(filename, "w");
	if(fd == NULL)
	{
		perr("can't open %s", filename);
		return 1;
	}

	m_lock(&status.mediadbmutex, 17);

	while(node != NULL)
	{
		ret = fprintf(fd, "%s#%d#%s#%s#%s#%s#%s#%s#%s#%s#%s#%s#%s#%s#%s#%lu\n", node->id, node->type, node->title, node->year, node->released, node->runtime, node->genre, node->director, node->writer, node->actors, node->plot, node->poster, node->rating, node->votes, node->file, node->timestamp);
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
void createmediadbfilter(int type, char* search, int flag)
{
	struct mediadb* node = mediadb;

	freemediadbfilter(0);
	while(node != NULL)
	{
		if(node->type == type)
		{
			if(flag == 0)
				addmediadbfilter(node, 1, NULL);
			else if(flag == 1 && ostrstrcase(node->year, search) != NULL)
				addmediadbfilter(node, 1, NULL);
			else if(flag == 2 && ostrstrcase(node->director, search) != NULL)
				addmediadbfilter(node, 1, NULL);
			else if(flag == 3 && ostrstrcase(node->actors, search) != NULL)
				addmediadbfilter(node, 1, NULL);
			else if(flag == 4 && ostrstrcase(node->file, search) != NULL)
				addmediadbfilter(node, 1, NULL);
			else if(flag == 5 && ostrstrcase(node->rating, search) != NULL)
				addmediadbfilter(node, 1, NULL);
			else if(flag == 6 && ostrstrcase(node->genre, search) != NULL)
				addmediadbfilter(node, 1, NULL);
		}
		node = node->next;
	}
}

void mediadbscanthread(struct stimerthread* self)
{
	struct mediadb *node = mediadb, *prev = mediadb;
	struct mediadbcategory *cnode = NULL;
	struct hdd *hddnode = NULL;
	struct splitstr* ret = NULL;
	char* tmpstr = NULL, *tmpsplit = NULL;

	if(status.mediadbthread != NULL || self == NULL) return;

        debug(777, "mediadb scanthread start");
        status.mediadbthread = self;

	//clear all other db in mem
	freemediadbfilter(0);
	freemediadbcategory(0);

	if(mediadb == NULL)
		readmediadb(getconfig("mediadbfile", NULL), 0, 0);

	//check mediadb for not exist file
	if(getconfigint("mediadbcheckdel", NULL) == 1)
	{
		while(node != NULL)
		{
			prev = node;
			node = node->next;
			if(!file_exist(prev->file))
				delmediadb(prev, 0);
		}
	}

	//find media files
	addhddall();
	hddnode = hdd;

	while(hddnode != NULL)
	{
		if(hddnode->partition != 0)
		{
			tmpstr = ostrcat("/autofs/", hddnode->device, 0, 0);
			findfiles(tmpstr);
			free(tmpstr); tmpstr = NULL;
		}
		hddnode = hddnode->next;
	}
	writemediadb(getconfig("mediadbfile", NULL));

	//create year
	node = mediadb;
	while(node != NULL)
	{
		int treffer = 1;
		cnode = mediadbcategory;
		while(cnode != NULL)
		{
			if(ostrcmp(cnode->name, node->year) == 0)
				treffer = 0;
			cnode = cnode->next;
		}

		if(treffer == 1)
		{
			tmpstr = ostrcat(oitoa(node->type), "#", 1, 0);
			tmpstr = ostrcat(tmpstr, node->year, 1, 0);
			debug(777, "add year %s", node->year);
			addmediadbcategory(tmpstr, node->type, 1, NULL, 0);
			free(tmpstr); tmpstr = NULL;
		}

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
		int treffer = 1;
		cnode = mediadbcategory;
		while(cnode != NULL)
		{
			if(ostrcmp(cnode->name, node->rating) == 0)
				treffer = 0;
			cnode = cnode->next;
		}

		if(treffer == 1)
		{
			tmpstr = ostrcat(oitoa(node->type), "#", 1, 0);
			tmpstr = ostrcat(tmpstr, node->rating, 1, 0);
			debug(777, "add rating %s", node->rating);
			addmediadbcategory(tmpstr, node->type, 1, NULL, 0);
			free(tmpstr); tmpstr = NULL;
		}

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

	status.mediadbthread = NULL;
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
			treffer = 1;
		node = node->next;
	}
	
	if(treffer == 0)
	{
		//create imdb search name
		char* shortname = ostrcat(file, NULL, 0, 0);
		string_tolower(shortname);
		shortname = string_shortname(shortname, 1);
		shortname = string_shortname(shortname, 2);
		string_removechar(shortname);
		strstrip(shortname);

		//TODO: got imdb infos
		struct imdb* imdb = NULL;
		struct skin pluginnode = NULL;

		pluginnode = getplugin("Imdb");
		if(pluginnode != NULL)
			imdb = getimdb(shortname, 0, 1, 0);

		debug(777, "add file %s (%s)", tmpstr, shortname);
		free(shortname); shortname = NULL;

		if(imdb != NULL)
		{
			debug(777, "imdb id %s", imdb->id);
			createmediadb(imdb->id, type, imdb->title, imdb->year, imdb->released, imdb->runtime, imdb->genre, imdb->director, imdb->writer, imdb->actors, imdb->plot, imdb->id, imdb->rating, imdb->votes, tmpstr, file);
		}
		else
			createmediadb("0", type, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, tmpstr, file);

		freeimdb(imdb); imdb = NULL;
	}
	free(tmpstr); tmpstr = NULL;
}


int findfiles(char* dirname)
{
	DIR *d;

	//Open the directory specified by dirname
	d = opendir(dirname);

	//Check it was opened
	if(! d)
	{
		perr("Cannot open directory %s", dirname);
		return 1;
	}

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
			if(strcmp(entry->d_name, "..") != 0 && strcmp (entry->d_name, ".") != 0)
			{
				path_length = snprintf(path, PATH_MAX, "%s/%s", dirname, entry->d_name);
				if(path_length >= PATH_MAX)
				{
					err("path length has got too long");
					return 1;
				}
				//Recursively call list_dir with the new path
				findfiles(path);
			}
		}
		else //File
		{
			//TODO: add extensions
			//video
			if(!filelistflt("*.avi", entry->d_name))
				mediadbfindfilecb(path, entry->d_name, 0);
			//audio
			if(!filelistflt("*.mp3", entry->d_name))
				mediadbfindfilecb(path, entry->d_name, 1);
			//picture
			if(!filelistflt("*.jpg", entry->d_name))
				mediadbfindfilecb(path, entry->d_name, 2);
		}
	}

	//After going through all the entries, close the directory
	if(closedir(d))
	{
		perr("Could not close %s", dirname);
		return 1;
	}

	return 0;
}

void mediadbscan()
{
	addtimer(&mediadbscanthread, START, 1000, 1, NULL, NULL, NULL);
}

#endif
