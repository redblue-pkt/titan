#ifndef MEDIADB_H
#define MEDIADB_H

struct mediadbfilter
{
	struct mediadb* node;
	struct mediadbfilter* prev;
	struct mediadbfilter* next;
};

struct mediadb
{
	char* id;
	int type;
	char* title;
	char* year;
	char* released;
	char* runtime;
	char* genre;
	char* director;
	char* writer;
	char* actors;
	char* plot;
	char* poster;
	char* rating;
	char* votes;
	char* file;
	struct mediadb* prev;
	struct mediadb* next;
};

struct mediadb *mediadb = NULL;
struct mediadbfilter *mediadbfilter = NULL;

struct mediadbfilter* getlastmediadbfilter(struct mediadbfilter* node)
{
	debug(1000, "in");
	struct mediadbfilter *prev = NULL;

	while(node != NULL)
	{
		prev = node;
		node = node->next;
	}

	debug(1000, "out");
	return prev;
}

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
	
	//debug(1000, "out");
	return newnode;
}

struct mediadb* addmediadb(char *line, int count, struct mediadb* last)
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
	plot = malloc(256);
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

	ret = sscanf(line, "%[^#]#%d#%[^#]#%[^#]#%[^#]#%[^#]#%[^#]#%[^#]#%[^#]#%[^#]#%[^#]#%[^#]#%[^#]#%[^#]#%[^#]", id, &newnode->type, title, year, released, runtime, genre, director, writer, actors, plot, poster, rating, votes, file);
	if(ret != 15)
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

	//modifymediadbcache(newnode->serviceid, newnode->transponderid, newnode);

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
	
	//debug(1000, "out");
	return newnode;
}

struct mediadb* createmediadb(char* id, int type, char* title, char* year, char* released, char* runtime, char* genre, char* director, char* writer, char* actors, char* plot, char* poster, char* rating, char* votes, char* file)
{
	struct mediadb* mnode = NULL;
	char* tmpstr = NULL;

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
	tmpstr = ostrcat(tmpstr, file, 1, 0);

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

	debug(1000, "out");
	return ret;
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
			freemediadbcontent(node);

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
		ret = fprintf(fd, "%s#%d#%s#%s#%s#%s#%s#%s#%s#%s#%s#%s#%s#%s#%s\n", node->id, node->type, node->title, node->year, node->released, node->runtime, node->genre, node->director, node->writer, node->actors, node->plot, node->poster, node->rating, node->votes, node->file);
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

void createmediadbfilter(int type, int sort)
{
	struct mediadb* node = mediadb;

	freemediadbfilter(0);
	while(node != NULL)
	{
		if(node->type == type)
		{
			addmediadbfilter(node, 1, NULL);
		}
		node = node->next;
	}
}

#endif
