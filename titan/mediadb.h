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

	int r = getrandomnum(maxentry);
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
		err("NULL detect");
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
		err("NULL detect");
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

	newnode->name = ostrshrink(name);

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
	return newnode;
}

int addmediadbcontent(struct mediadb* node, char *line, int len, int count)
{
	int ret = 0;
	char* tmpstr = NULL, *type = NULL, *year = NULL, *rating = NULL;
	char* votes = NULL, *timestamp = NULL, *flag = NULL, *backdropcount = NULL;

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
					case 15: node->shortname = tmpstr; break;
					case 16: node->fileinfo = tmpstr; break;	
					case 17: timestamp = tmpstr; break;
					case 18: flag = tmpstr; break;
					case 19: backdropcount = tmpstr; break;
				}

				ret++;
			}
			else
				tmpstr++;
		}
	}

	if(ret != 20)
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
	if(backdropcount != NULL) node->backdropcount = atoi(backdropcount);

	return 0;
}

//flag 0: with lock
//flag 1: without lock
struct mediadb* addmediadb(char *line, int len, int count, struct mediadb* last, int sort, int flag)
{
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
	return newnode;
}

struct mediadb* createmediadb(struct mediadb* update, char* id, int type, char* title, char* year, char* released, char* runtime, char* genre, char* director, char* writer, char* actors, char* plot, char* poster, char* rating, char* votes, char* path, char* file, char* shortname, char* fileinfo, int flag, int backdropcount)
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
	shortname = stringreplacechar(shortname, '#', ' ');
	fileinfo = stringreplacechar(fileinfo, '#', ' ');
	
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
	tmpstr = ostrcat(tmpstr, shortname, 1, 0);
	tmpstr = ostrcat(tmpstr, "#", 1, 0);
	tmpstr = ostrcat(tmpstr, fileinfo, 1, 0);
	tmpstr = ostrcat(tmpstr, "#", 1, 0);
	tmpstr = ostrcat(tmpstr, olutoa(time(NULL)), 1, 1);
	tmpstr = ostrcat(tmpstr, "#", 1, 0);
	tmpstr = ostrcat(tmpstr, oitoa(flag), 1, 1);
	tmpstr = ostrcat(tmpstr, "#", 1, 0);
	tmpstr = ostrcat(tmpstr, oitoa(backdropcount), 1, 1);

	tmpstr = string_replace_all("\n", "", tmpstr, 1);
	tmpstr = string_replace_all("\r", "", tmpstr, 1);

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
	node->shortname = NULL;
	node->fileinfo = NULL;
	node->timestamp = 0;
	node->flag = 0;
	node->backdropcount = 0;
}

//flag 0: with lock
//flag 1: without lock
int delmediadbfilter(struct mediadbfilter* mnode, int flag)
{
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
	return ret;
}

int delmediadbcategory(struct mediadbcategory* mnode, int flag)
{
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
	return ret;
}

int delmediadb(struct mediadb* mnode, int flag)
{
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
	return ret;
}

void freemediadbfilter(int flag)
{
	struct mediadbfilter *node = mediadbfilter, *prev = mediadbfilter;

	while(node != NULL)
	{
		prev = node;
		node = node->next;
		if(prev != NULL)
			delmediadbfilter(prev, flag);
	}
}

void freemediadbcategory(int flag)
{
	struct mediadbcategory *node = mediadbcategory, *prev = mediadbcategory;

	while(node != NULL)
	{
		prev = node;
		node = node->next;
		if(prev != NULL)
			delmediadbcategory(prev, flag);
	}
}

void freemediadb(int flag)
{
	struct mediadb *node = mediadb, *prev = mediadb;

	while(node != NULL)
	{
		prev = node;
		node = node->next;
		if(prev != NULL)
			delmediadb(prev, flag);
	}

	status.writemediadb = 0;
}

int writemediadbcategory(const char *filename)
{
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
	return 0;
}

int writemediadb(const char *filename, struct mediadb* cmediadb)
{
	FILE *fd = NULL;
	struct mediadb *node = NULL;
	int ret = 0;

	if(cmediadb == NULL)
		fd = fopen(filename, "w");
	else
		fd = fopen(filename, "a");
	if(fd == NULL)
	{
		perr("can't open %s", filename);
		return 1;
	}

	int run = 0, videocount = 0, audiocount = 0, picturecount = 0;
	if(status.mediadbfiles > 0)
	{
		getmediadbcounts(&videocount, &audiocount, &picturecount);
		run = 1;
	}

	m_lock(&status.mediadbmutex, 17);
	if(cmediadb == NULL)
		node = mediadb;
	else
		node = cmediadb;

	while(node != NULL)
	{
		ret = fprintf(fd, "%s#%d#%s#%d#%s#%s#%s#%s#%s#%s#%s#%s#%d#%d#%s#%s#%s#%s#%lu#%d#%d\n", node->id, node->type, node->title, node->year, node->released, node->runtime, node->genre, node->director, node->writer, node->actors, node->plot, node->poster, node->rating, node->votes, node->path, node->file, node->shortname, node->fileinfo, node->timestamp, node->flag, node->backdropcount);

		if(ret < 0)
		{
			perr("writting file %s", filename);
		}
		if(cmediadb != NULL) break;
		node = node->next;
	}

	if(run == 1 && status.mediadbfiles > 0)
	{	
		char* tmpstr = NULL;
		tmpstr = ostrcat(tmpstr, "add(", 1, 0);
		tmpstr = ostrcat(tmpstr, oitoa(videocount), 1, 1);
		tmpstr = ostrcat(tmpstr, "/", 1, 0);
		tmpstr = ostrcat(tmpstr, oitoa(status.mediadbfiles), 1, 1);
		tmpstr = ostrcat(tmpstr, ")", 1, 0);
		writevfdmenu(tmpstr);
		free(tmpstr),tmpstr = NULL;
	}

	m_unlock(&status.mediadbmutex, 17);

	fclose(fd);
	status.writemediadb = 0;
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

//flag: bit 31 = 0 (rekursive), 1 (no recursive)
void mediadbscanthread(struct stimerthread* self, char* path, int flag)
{
	struct mediadb *node = mediadb, *prev = mediadb;
	struct mediadbcategory *cnode = NULL;
	//struct hdd *hddnode = NULL;
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

	int type = flag;
	int onlydir = checkbit(flag, 31);

	type = clearbit(type, 31);

	if(type > 999)
	{
		type = type - 1000;
	
		char* tmpstr = NULL;
		tmpstr = ostrcat(tmpstr, "scanning (", 1, 0);
		tmpstr = ostrcat(tmpstr, "0", 1, 0);
		tmpstr = ostrcat(tmpstr, "/", 1, 0);
		tmpstr = ostrcat(tmpstr, oitoa(status.mediadbfiles), 1, 1);
		tmpstr = ostrcat(tmpstr, ")", 1, 0);
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
		tmpstr = ostrcat(tmpstr, _("scan Directory"), 1, 0);
		tmpstr = ostrcat(tmpstr, ": \t\t\t", 1, 0);
		tmpstr = ostrcat(tmpstr, path, 1, 0);
		tmpstr = ostrcat(tmpstr, "\n  ", 1, 0);		
		tmpstr = ostrcat(tmpstr, _("MediaDB place"), 1, 0);
		tmpstr = ostrcat(tmpstr, ": \t\t\t", 1, 0);				
		tmpstr = ostrcat(tmpstr, getconfig("mediadbpath", NULL), 1, 0);
		tmpstr = ostrcat(tmpstr, "\n  ", 1, 0);		
		tmpstr = ostrcat(tmpstr, _("MediaDB Debug"), 1, 0);
		tmpstr = ostrcat(tmpstr, ": \t\t\t", 1, 0);
		if(ostrcmp(getconfig("mediadbdebug", NULL), "1") == 0)
			tmpstr = ostrcat(tmpstr, _("yes"), 1, 0);
		else
			tmpstr = ostrcat(tmpstr, _("no"), 1, 0);
		tmpstr = ostrcat(tmpstr, "\n  ", 1, 0);		
		tmpstr = ostrcat(tmpstr, _("Backdrop Download Count"), 1, 0);
		tmpstr = ostrcat(tmpstr, ": \t\t", 1, 0);
		if(getconfigint("mediadbbackdrop", NULL) == 0)
			tmpstr = ostrcat(tmpstr, _("all"), 1, 0);
		else
			tmpstr = ostrcat(tmpstr, oitoa(getconfigint("mediadbbackdrop", NULL)), 1, 1);

		int count = 0;

		while(count < 60)
		{
			sleep(1);
			count++;
		}

		if(!file_exist("/tmp/.autoscan"))
		{
			free(path); path = NULL;
			return;
		}
									
		textbox(_("Message"), tmpstr, _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 1100, 500, 10, 0);
		free(tmpstr), tmpstr = NULL;
		writevfdmenu("iMDB start");
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

	node = mediadb;
	prev = mediadb;

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

	status.mediadbsavetime = 0;
	status.mediadbthreadstatus = 2;

	//find media files
	if(path == NULL)
	{
		findfiles("/media/usb", type, onlydir, 0, 1);
		findfiles("/media/net", type, onlydir, 0, 1);
		/*
		addhddall();
		hddnode = hdd;

		while(hddnode != NULL)
		{
			if(hddnode->partition != 0)
			{
				tmpstr = ostrcat("/autofs/", hddnode->device, 0, 0);
				findfiles(tmpstr, type, onlydir, 0, 1);
				free(tmpstr); tmpstr = NULL;
			}
			hddnode = hddnode->next;
		}
		*/
	}
	else
		findfiles(path, type, onlydir, 0, 1);

	free(path); path = NULL;

	writemediadb(getconfig("mediadbfile", NULL), NULL);

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

	textbox(_("Message"), _("MediaDB scan finished"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 10, 0);
	writevfdmenu("iMDB Done !");

	status.mediadbsavetime = 0;
	status.mediadbfiles = 0;
	status.mediadbthread = NULL;
	status.mediadbthreadstatus = 0;
	debug(777, "mediadb scanthread end");
}

// flag 1 = shortname
// flag 2 = fileinfo
char* createshortname(char* file, int *isrec, int *iscam, int flag)
{
	char* fileinfo = NULL, *shortname = NULL, *tmpstr = NULL, *tmpstr1 = NULL;
	//create imdb search name
	shortname = ostrcat(file, NULL, 0, 0);

	// create filelist info
	tmpstr = ostrcat(tmpstr, file, 1, 0);

printf("###### strip () strings start ############################################\n");

	printf("shortname first : %s\n", shortname);
	if(cmpfilenameext(tmpstr, ".ts") != 0)
	{
		shortname = string_replace_remove_last_chars("(", "", shortname, 1);
		printf("filename strip remove all chars after (... from: %s\n", shortname);
	}
// strip () strings
// for (channel)-movie-(..).ts name
	tmpstr1 = string_resub("(", ")", shortname, 0);
	if(tmpstr1 != NULL)
	{
printf("found recnew filename strip (channel)-movie-(..).ts name from: %s\n", tmpstr);
		tmpstr1 = ostrcat("(", tmpstr1, 0, 1);
		tmpstr1 = ostrcat(tmpstr1, ")", 1, 0);
printf("shortname: %s\n", shortname);
printf("tmpstr1: %s\n", tmpstr1);
		shortname = string_replace(tmpstr1, "", shortname, 1);
printf("shortname stripped: %s\n", shortname);
printf("--------------------------------------------------\n");
		if(fileinfo != NULL)
			fileinfo = ostrcat(fileinfo, " ", 1, 0);
		fileinfo = ostrcat(fileinfo, "recnew", 1, 0);
		*isrec = 0;
		*iscam = 0;
	}
	free(tmpstr1); tmpstr1 = NULL;

// for movie-(channel-...).ts name
	tmpstr1 = string_resub("(", ")", shortname, 0);
	if(tmpstr1 != NULL)
	{
printf("found recnew filename strip movie-(channel-...).ts name from: %s\n", tmpstr);
		tmpstr1 = ostrcat("(", tmpstr1, 0, 1);
		tmpstr1 = ostrcat(tmpstr1, ")", 1, 0);
printf("shortname: %s\n", shortname);
printf("tmpstr1: %s\n", tmpstr1);
		shortname = string_replace(tmpstr1, "", shortname, 1);
printf("shortname stripped: %s\n", shortname);
printf("--------------------------------------------------\n");
		if(fileinfo != NULL)
			fileinfo = ostrcat(fileinfo, " ", 1, 0);
		fileinfo = ostrcat(fileinfo, "recnew", 1, 0);
		*isrec = 0;
		*iscam = 0;
	}
	free(tmpstr1); tmpstr1 = NULL;
// end
printf("###### strip () strings end ############################################\n");

	tmpstr1 = oregex(".*([0-9]{14,14}).*", tmpstr);
	if(tmpstr1 != NULL)
	{
		shortname = string_replace(tmpstr1, "", shortname, 1);
		if(fileinfo != NULL)
			fileinfo = ostrcat(fileinfo, " ", 1, 0);
		fileinfo = ostrcat(fileinfo, "rec", 1, 0);
		*isrec = 1;
	}			
	free(tmpstr1); tmpstr1 = NULL;

	tmpstr1 = oregex(".*([0-9]{8,8}.*[0-9]{4,4}).*", tmpstr);
	if(tmpstr1 != NULL && *isrec == 0)
	{
		shortname = string_replace(tmpstr1, "", shortname, 1);
		if(fileinfo != NULL)
			fileinfo = ostrcat(fileinfo, " ", 1, 0);
		fileinfo = ostrcat(fileinfo, "recold", 1, 0);
		*isrec = 1;
	}
	free(tmpstr1); tmpstr1 = NULL;

	tmpstr1 = oregex(".*([0-9]{5,5}).*", tmpstr);
	if(tmpstr1 != NULL && *isrec == 0)
	{
		*iscam = 1;
		if(fileinfo != NULL)
			fileinfo = ostrcat(fileinfo, " ", 1, 0);
		fileinfo = ostrcat(fileinfo, "cam", 1, 0);
	}
	free(tmpstr1), tmpstr1 = NULL;

	if(*isrec == 0 && *iscam == 0)
	{
		string_tolower(shortname);
		shortname = string_shortname(shortname, 2);
		string_removechar(shortname);
		strstrip(shortname);
	}
	else
	{
		char* cut = ostrcat(".", getfilenameext(tmpstr), 0, 1);
		cut = ostrcat(cut, "\0", 1, 0);
		shortname = string_replace(cut, "\0", shortname, 1);
		free(cut); cut = NULL;

		string_removechar(shortname);
		strstrip(shortname);
	}

	string_tolower(tmpstr);

	if((ostrstr(tmpstr, ".special.extended.edition.") != NULL) || (ostrstr(tmpstr, ".sse.") != NULL))
	{
		if(fileinfo != NULL)
			fileinfo = ostrcat(fileinfo, " ", 1, 0);
		fileinfo = ostrcat(fileinfo, "SEE-Version", 1, 0);
		shortname = string_replace(".special.extended.edition.", "", shortname, 1);
	}
	if((ostrstr(tmpstr, ".extended.version.") != NULL) || (ostrstr(tmpstr, ".extended.") != NULL) || (ostrstr(tmpstr, ".ed.") != NULL))
	{
		if(fileinfo != NULL)
			fileinfo = ostrcat(fileinfo, " ", 1, 0);
		fileinfo = ostrcat(fileinfo, "ED-Version", 1, 0);
		shortname = string_replace(".extended.version.", "", shortname, 1);
	}
	if((ostrstr(tmpstr, ".uncut.") != NULL) || (ostrstr(tmpstr, ".uc.") != NULL))
	{
		if(fileinfo != NULL)
			fileinfo = ostrcat(fileinfo, " ", 1, 0);
		fileinfo = ostrcat(fileinfo, "UC-Version", 1, 0);
	}
	if((ostrstr(tmpstr, ".unrated.") != NULL) || (ostrstr(tmpstr, ".ur.") != NULL))
	{
		if(fileinfo != NULL)
			fileinfo = ostrcat(fileinfo, " ", 1, 0);
		fileinfo = ostrcat(fileinfo, "UR-Version", 1, 0);
	}
	if((ostrstr(tmpstr, ".telesync.") != NULL) || (ostrstr(tmpstr, ".ts.") != NULL))
	{
		if(fileinfo != NULL)
			fileinfo = ostrcat(fileinfo, " ", 1, 0);			
		fileinfo = ostrcat(fileinfo, "telesync", 1, 0);
	}
	if((ostrstr(tmpstr, ".telecine.") != NULL) || (ostrstr(tmpstr, ".tc.") != NULL))
	{
		if(fileinfo != NULL)
			fileinfo = ostrcat(fileinfo, " ", 1, 0);
		fileinfo = ostrcat(fileinfo, "telecine", 1, 0);
	}
	if((ostrstr(tmpstr, ".dts.") != NULL) || (ostrstr(tmpstr, ".dtshd.") != NULL))
	{
		if(fileinfo != NULL)
			fileinfo = ostrcat(fileinfo, " ", 1, 0);
		fileinfo = ostrcat(fileinfo, "dts", 1, 0);
	}
	if((ostrstr(tmpstr, ".ac3.") != NULL) || (ostrstr(tmpstr, ".ac3d.") != NULL) || (ostrstr(tmpstr, ".ac3hd.") != NULL))
	{
		if(fileinfo != NULL)
			fileinfo = ostrcat(fileinfo, " ", 1, 0);
		fileinfo = ostrcat(fileinfo, "ac3", 1, 0);
	}
	if(ostrstr(tmpstr, ".r5.") != NULL)
	{
		if(fileinfo != NULL)
			fileinfo = ostrcat(fileinfo, " ", 1, 0);
		fileinfo = ostrcat(fileinfo, "r5", 1, 0);
	}
	if(ostrstr(tmpstr, ".r3.") != NULL)
	{
		if(fileinfo != NULL)
			fileinfo = ostrcat(fileinfo, " ", 1, 0);
		fileinfo = ostrcat(fileinfo, "r3", 1, 0);
	}
	if(ostrstr(tmpstr, ".r1.") != NULL)
	{
		if(fileinfo != NULL)
			fileinfo = ostrcat(fileinfo, " ", 1, 0);
		fileinfo = ostrcat(fileinfo, "r1", 1, 0);
	}			
	if(ostrstr(tmpstr, ".sample.") != NULL)
	{
		if(fileinfo != NULL)
			fileinfo = ostrcat(fileinfo, " ", 1, 0);
		fileinfo = ostrcat(fileinfo, "sample", 1, 0);
	}

	tmpstr1 = oregex(".*([0-9]{4,4}).*", tmpstr);
	if(tmpstr1 != NULL && *isrec == 0 && *iscam == 0)
	{
		if(fileinfo != NULL)
			fileinfo = ostrcat(fileinfo, " ", 1, 0);
		fileinfo = ostrcat(fileinfo, tmpstr1, 1, 0);
	}
	free(tmpstr1); tmpstr1 = NULL;
	
	tmpstr1 = oregex(".*(cd[0-9]{1,3}).*", tmpstr);
	if(tmpstr1 != NULL)
	{
		if(fileinfo != NULL)
			fileinfo = ostrcat(fileinfo, " ", 1, 0);
		fileinfo = ostrcat(fileinfo, tmpstr1, 1, 0);
		shortname = string_replace(tmpstr1, "", shortname, 1);
	}
	free(tmpstr1); tmpstr1 = NULL;

	tmpstr1 = oregex(".*(dvd[0-9]{1,2}).*", tmpstr);
	if(tmpstr1 != NULL)
	{
		if(fileinfo != NULL)
			fileinfo = ostrcat(fileinfo, " ", 1, 0);
		fileinfo = ostrcat(fileinfo, tmpstr1, 1, 0);
		shortname = string_replace(tmpstr1, "", shortname, 1);
	}
	free(tmpstr1); tmpstr1 = NULL;

	tmpstr1 = oregex(".*(s[0-9]{1,2}e[0-9]{1,2}e[0-9]{1,2}).*", tmpstr);
	if(tmpstr1 != NULL)
	{
		if(fileinfo != NULL)
			fileinfo = ostrcat(fileinfo, " ", 1, 0);
		fileinfo = ostrcat(fileinfo, tmpstr1, 1, 0);
		shortname = string_replace(tmpstr1, "", shortname, 1);
	}
	free(tmpstr1); tmpstr1 = NULL;

	tmpstr1 = oregex(".*(s[0-9]{1,2}e[0-9]{1,2}).*", tmpstr);
	if(tmpstr1 != NULL)
	{
		if(fileinfo != NULL)
			fileinfo = ostrcat(fileinfo, " ", 1, 0);
		fileinfo = ostrcat(fileinfo, tmpstr1, 1, 0);
		shortname = string_replace(tmpstr1, "", shortname, 1);
	}
	free(tmpstr1); tmpstr1 = NULL;

	tmpstr1 = oregex(".*(disc[0-9]{1,2}).*", tmpstr);
	if(tmpstr1 != NULL)
	{
		if(fileinfo != NULL)
			fileinfo = ostrcat(fileinfo, " ", 1, 0);
		fileinfo = ostrcat(fileinfo, tmpstr1, 1, 0);
		shortname = string_replace(tmpstr1, "", shortname, 1);
	}
	free(tmpstr1); tmpstr1 = NULL;

	tmpstr1 = oregex(".*(season[0-9]{1,2}).*", tmpstr);
	if(tmpstr1 != NULL)
	{
		if(fileinfo != NULL)
			fileinfo = ostrcat(fileinfo, " ", 1, 0);
		fileinfo = ostrcat(fileinfo, tmpstr1, 1, 0);
		shortname = string_replace(tmpstr1, "", shortname, 1);
	}
	free(tmpstr1); tmpstr1 = NULL;

	tmpstr1 = oregex(".*(episode[0-9]{1,2}).*", tmpstr);
	if(tmpstr1 != NULL)
	{
		if(fileinfo != NULL)
			fileinfo = ostrcat(fileinfo, " ", 1, 0);
		fileinfo = ostrcat(fileinfo, tmpstr1, 1, 0);
		shortname = string_replace(tmpstr1, "", shortname, 1);
	}
	free(tmpstr1); tmpstr1 = NULL;

	tmpstr1 = oregex(".*(staffel[0-9]{1,2}).*", tmpstr);
	if(tmpstr1 != NULL)
	{
		if(fileinfo != NULL)
			fileinfo = ostrcat(fileinfo, " ", 1, 0);
		fileinfo = ostrcat(fileinfo, tmpstr1, 1, 0);
		shortname = string_replace(tmpstr1, "", shortname, 1);
	}
	free(tmpstr1); tmpstr1 = NULL;

	tmpstr1 = oregex(".*(part[0-9]{1,2}).*", tmpstr);
	if(tmpstr1 != NULL)
	{
		if(fileinfo != NULL)
			fileinfo = ostrcat(fileinfo, " ", 1, 0);
		fileinfo = ostrcat(fileinfo, tmpstr1, 1, 0);
		shortname = string_replace(tmpstr1, "", shortname, 1);
	}
	free(tmpstr1); tmpstr1 = NULL;

	strstrip(shortname);

	if(fileinfo != NULL)
		fileinfo = ostrcat(fileinfo, " ", 1, 0);
	fileinfo = ostrcat(fileinfo, getfilenameext(tmpstr), 1, 1);
	free(tmpstr); tmpstr = NULL;

	if(flag == 1)
	{
		free(fileinfo);
		return shortname;
	}
	else if(flag == 2)
	{
		free(shortname);
		return fileinfo;
	}

	free(fileinfo);
	free(shortname);
	return NULL;
}

struct mediadb* mediadbcheckfile(char* file, char* path, char* shortpath)
{
	char* tmpstr = NULL;
	struct mediadb *node = NULL;

	if(file == NULL || path == NULL || shortpath == NULL) return NULL;
	m_lock(&status.mediadbmutex, 17);

	//check if entry exist
	node = mediadb;
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

	return node;
}

int mediadbffmpeg1(char* file, char* path, char* timestamp, char* logfile)
{
	char* cmd = NULL;
	debug(133, "backdrop1.jpg timestamp %s", timestamp);
	debug(133, "backdrop1.jpg file %s", file);
	debug(133, "backdrop1.jpg path %s", path);

	if(file == NULL || path == NULL || timestamp == NULL) return 1;

	cmd = ostrcat(cmd, "ffmpeg -i -ss 30 \"", 1, 0);
	cmd = ostrcat(cmd, path, 1, 0);
	cmd = ostrcat(cmd, "/", 1, 0);
	cmd = ostrcat(cmd, file, 1, 0);
//cmd = ostrcat(cmd, "\" -vframes 1 -s 1920x1080 ", 1, 0);
	cmd = ostrcat(cmd, "\" -vframes 1 -s 1280x720 ", 1, 0);
	cmd = ostrcat(cmd, getconfig("mediadbpath", NULL), 1, 0);
	cmd = ostrcat(cmd, "/", 1, 0);
	cmd = ostrcat(cmd, timestamp, 1, 0);
	cmd = ostrcat(cmd, "_backdrop1.jpg", 1, 0);

	if(logfile != NULL && getconfigint("mediadbdebug", NULL) == 1)
	{
		filedebug(logfile, "#############\nLocalfile: %s/%s_backdrop1.jpg\n#############", getconfig("mediadbpath", NULL), timestamp);
		cmd = ostrcat(cmd, " >> ", 1, 0);
		cmd = ostrcat(cmd, logfile, 1, 0);
		cmd = ostrcat(cmd, " 2>&1", 1, 0);
	}
	else
		cmd = ostrcat(cmd, " > /dev/null 2>&1", 1, 0);

	debug(133, "cmd %s", cmd);
	system(cmd);
	free(cmd); cmd = NULL;

	return 0;
}

int mediadbffmpeg2(char* file, char* path, char* timestamp, char* logfile)
{
	char* cmd = NULL;
	debug(133, "thumb.jpg timestamp %s", timestamp);
	debug(133, "thumb.jpg file %s", file);
	debug(133, "thumb.jpg path %s", path);

	if(file == NULL || path == NULL || timestamp == NULL) return 1;

	cmd = ostrcat(cmd, "ffmpeg -i -ss 40 \"", 1, 0);
	cmd = ostrcat(cmd, path, 1, 0);
	cmd = ostrcat(cmd, "/", 1, 0);
	cmd = ostrcat(cmd, file, 1, 0);
	cmd = ostrcat(cmd, "\" -vframes 1 -s 160x120 ", 1, 0);
	cmd = ostrcat(cmd, getconfig("mediadbpath", NULL), 1, 0);
	cmd = ostrcat(cmd, "/", 1, 0);
	cmd = ostrcat(cmd, timestamp, 1, 0);
	cmd = ostrcat(cmd, "_thumb.jpg", 1, 0);

	if(logfile != NULL && getconfigint("mediadbdebug", NULL) == 1)
	{
		filedebug(logfile, "#############\nLocalfile: %s/%s_thumb.jpg\n#############", getconfig("mediadbpath", NULL), timestamp);
		cmd = ostrcat(cmd, " >> ", 1, 0);
		cmd = ostrcat(cmd, logfile, 1, 0);
		cmd = ostrcat(cmd, " 2>&1", 1, 0);
	}
	else
		cmd = ostrcat(cmd, " > /dev/null 2>&1", 1, 0);

	debug(133, "cmd %s", cmd);
	system(cmd);
	free(cmd); cmd = NULL;

	return 0;
}

int mediadbffmpeg3(char* file, char* path, char* timestamp, char* logfile)
{
	char* cmd = NULL;
	debug(133, "cover.jpg timestamp %s", timestamp);
	debug(133, "cover.jpg file %s", file);
	debug(133, "cover.jpg path %s", path);

	if(file == NULL || path == NULL || timestamp == NULL) return 1;

	cmd = ostrcat(cmd, "ffmpeg -i -ss 50 -i \"", 1, 0);
	cmd = ostrcat(cmd, path, 1, 0);
	cmd = ostrcat(cmd, "/", 1, 0);
	cmd = ostrcat(cmd, file, 1, 0);
	cmd = ostrcat(cmd, "\" -vframes 1 -s 500x400 ", 1, 0);
	cmd = ostrcat(cmd, getconfig("mediadbpath", NULL), 1, 0);
	cmd = ostrcat(cmd, "/", 1, 0);
	cmd = ostrcat(cmd, timestamp, 1, 0);
	cmd = ostrcat(cmd, "_cover.jpg", 1, 0);

	if(logfile != NULL && getconfigint("mediadbdebug", NULL) == 1)
	{
		filedebug(logfile, "#############\nLocalfile: %s/%s_cover.jpg\n#############", getconfig("mediadbpath", NULL), timestamp);
		cmd = ostrcat(cmd, " >> ", 1, 0);
		cmd = ostrcat(cmd, logfile, 1, 0);
		cmd = ostrcat(cmd, " 2>&1", 1, 0);
	}
	else
		cmd = ostrcat(cmd, " > /dev/null 2>&1", 1, 0);

	debug(133, "cmd %s", cmd);
	system(cmd);
	free(cmd); cmd = NULL;

	return 0;
}

int mediadbffmpeg4(char* timestamp, char* tmpmeta)
{
	char* cmd = NULL;

	if(timestamp == NULL || tmpmeta == NULL) return 1;

	cmd = ostrcat(cmd, "ffmpeg -i ", 1, 0);
	cmd = ostrcat(cmd, getconfig("mediadbpath", NULL), 1, 0);
	cmd = ostrcat(cmd, "/", 1, 0);
	cmd = ostrcat(cmd, timestamp, 1, 0);
	cmd = ostrcat(cmd, "_backdrop1.jpg", 1, 0);
	cmd = ostrcat(cmd, " > ", 1, 0);
	cmd = ostrcat(cmd, tmpmeta, 1, 0);
	cmd = ostrcat(cmd, " 2>&1", 1, 0);

	debug(133, "cmd %s", cmd);
	system(cmd);
	free(cmd); cmd = NULL;

	return 0;
}

int mediadbffmpeg5(char* tmpjpg, char* tmpmpg, char* timestamp, char* file, char* size, off64_t filesize, char* logfile)
{
	char* cmd = NULL;

	if(tmpjpg == NULL || tmpmpg == NULL) return 1;

#ifdef MIPSEL_OLD_GIT
	cmd = ostrcat(cmd, "jpeg2yuv -v 1 -f 25 -n1 -I p -j ", 1, 0);
	cmd = ostrcat(cmd, tmpjpg, 1, 0);
	cmd = ostrcat(cmd, " | mpeg2enc -v 1 -x 1280 -y 720 -a 3 -f12 -4 1 -2 1 -q 1 -H -o ", 1, 0);
	cmd = ostrcat(cmd, tmpmpg, 1, 0);
#else
	cmd = ostrcat(cmd, "ffmpeg -y -f image2 -i ", 1, 0);
	cmd = ostrcat(cmd, tmpjpg, 1, 0);
	cmd = ostrcat(cmd, " ", 1, 0);
	cmd = ostrcat(cmd, tmpmpg, 1, 0);
#endif

	if(logfile != NULL && getconfigint("mediadbdebug", NULL) == 1)
	{
		filedebug(logfile, "#############\nLocalfile: %s/%s_backdrop1.jpg size=(%s) filesize(%lld) (%s)\n#############", getconfig("mediadbpath", NULL), timestamp, size, filesize, file);
		cmd = ostrcat(cmd, " >> ", 1, 0);
		cmd = ostrcat(cmd, logfile, 1, 0);
		cmd = ostrcat(cmd, " 2>&1", 1, 0);
	}
	else
		cmd = ostrcat(cmd, " > /dev/null 2>&1", 1, 0);

	debug(133, "cmd %s", cmd);
	system(cmd);
	free(cmd); cmd = NULL;

	return 0;
}

int mediadbcp(char* timestamp, char* poster)
{
	char* cmd = NULL;
	debug(133, "timestamp %s", timestamp);
	debug(133, "poster %s", poster);

	if(timestamp == NULL || poster == NULL) return 1;

	cmd = ostrcat(cmd, "cp -a ", 1, 0);
	cmd = ostrcat(cmd, getconfig("mediadbpath", NULL), 1, 0);
	cmd = ostrcat(cmd, "/", 1, 0);
	cmd = ostrcat(cmd, timestamp, 1, 0);
	cmd = ostrcat(cmd, "_cover.jpg", 1, 0);
	cmd = ostrcat(cmd, " ", 1, 0);
	cmd = ostrcat(cmd, getconfig("mediadbpath", NULL), 1, 0);
	cmd = ostrcat(cmd, "/", 1, 0);
	cmd = ostrcat(cmd, timestamp, 1, 0);
	cmd = ostrcat(cmd, poster, 1, 0);

	debug(133, "cmd %s", cmd);
	system(cmd);
	free(cmd); cmd = NULL;

	return 0;
}

int mediadbcprec(char* timestamp, char* poster)
{
	char* cmd = NULL;

	if(timestamp == NULL || poster == NULL) return 1;

	cmd = ostrcat(cmd, "cp -a /tmp/screenshot", 1, 0);
	cmd = ostrcat(cmd, poster, 1, 0);
	cmd = ostrcat(cmd, " ", 1, 0);
	cmd = ostrcat(cmd, getconfig("mediadbpath", NULL), 1, 0);
	cmd = ostrcat(cmd, "/", 1, 0);
	cmd = ostrcat(cmd, timestamp, 1, 0);
	cmd = ostrcat(cmd, poster, 1, 0);

	debug(133, "cmd %s", cmd);
	system(cmd);
	free(cmd); cmd = NULL;

	cmd = ostrcat(cmd, "rm -f /tmp/screenshot", 1, 0);
	cmd = ostrcat(cmd, poster, 1, 0);
	debug(133, "cmd %s", cmd);
	system(cmd);
	free(cmd); cmd = NULL;
	
	return 0;
}

void mediadbcptmdb(struct tmdb* tmdb, struct imdb* imdb)
{
	if(tmdb == NULL || imdb == NULL) return;

	if(tmdb->imdbid != NULL) imdb->id = ostrcat(tmdb->imdbid, NULL, 0, 0);
	if(tmdb->title != NULL) imdb->title = ostrcat(tmdb->title, NULL, 0, 0);
	if(tmdb->genre != NULL) imdb->genre = ostrcat(tmdb->genre, NULL, 0, 0);
	if(tmdb->rating != NULL) imdb->rating = ostrcat(tmdb->rating, NULL, 0, 0);
	if(tmdb->votes != NULL) imdb->votes = ostrcat(tmdb->votes, NULL, 0, 0);
	if(tmdb->runtime != NULL) imdb->runtime = ostrcat(tmdb->runtime, NULL, 0, 0);
	if(tmdb->plot != NULL) imdb->plot = ostrcat(tmdb->plot, NULL, 0, 0);
	if(tmdb->released != NULL) imdb->released = ostrcat(tmdb->released, NULL, 0, 0);
	if(tmdb->postermid != NULL) imdb->poster = ostrcat(tmdb->postermid, NULL, 0, 0);
	if(tmdb->thumb != NULL) imdb->thumb = ostrcat(tmdb->thumb, NULL, 0, 0);
	if(tmdb->year != NULL) imdb->year = ostrcat(tmdb->year, NULL, 0, 0);
}

void mediadbcpimdbapi(struct imdbapi* imdbapi, struct imdb* imdb)
{
	if(imdbapi == NULL || imdb == NULL) return;

	if(imdbapi->id != NULL) imdb->id = ostrcat(imdbapi->id, NULL, 0, 0);
	if(imdbapi->title != NULL) imdb->title = ostrcat(imdbapi->title, NULL, 0, 0);
	if(imdbapi->genre != NULL) imdb->genre = ostrcat(imdbapi->genre, NULL, 0, 0);
	if(imdbapi->writer != NULL) imdb->writer = ostrcat(imdbapi->writer, NULL, 0, 0);
	if(imdbapi->director != NULL) imdb->director = ostrcat(imdbapi->director, NULL, 0, 0);
	if(imdbapi->actors != NULL) imdb->actors = ostrcat(imdbapi->actors, NULL, 0, 0);
	if(imdbapi->rating != NULL) imdb->rating = ostrcat(imdbapi->rating, NULL, 0, 0);
	if(imdbapi->votes != NULL) imdb->votes = ostrcat(imdbapi->votes, NULL, 0, 0);
	if(imdbapi->runtime != NULL) imdb->runtime = ostrcat(imdbapi->runtime, NULL, 0, 0);
	if(imdbapi->plot != NULL) imdb->plot = ostrcat(imdbapi->plot, NULL, 0, 0);
	if(imdbapi->released != NULL) imdb->released = ostrcat(imdbapi->released, NULL, 0, 0);
	if(imdbapi->year != NULL) imdb->year = ostrcat(imdbapi->year, NULL, 0, 0);
}

void mediadbcptmdb2(struct tmdb* tmdb, struct imdb* imdb)
{
	if(tmdb == NULL || imdb == NULL) return;

	if(imdb->id == NULL) imdb->id = ostrcat(imdb->id, tmdb->imdbid, 1, 0);
	if(imdb->title == NULL) imdb->title = ostrcat(imdb->title, tmdb->title, 1, 0);
	if(imdb->genre == NULL) imdb->genre = ostrcat(imdb->genre, tmdb->genre, 1, 0);
	if(imdb->rating == NULL) imdb->rating = ostrcat(imdb->rating, tmdb->rating, 1, 0);
	if(imdb->votes == NULL) imdb->votes = ostrcat(imdb->votes, tmdb->votes, 1, 0);
	if(imdb->runtime == NULL) imdb->runtime = ostrcat(imdb->runtime, tmdb->runtime, 1, 0);
	if(imdb->plot == NULL) imdb->plot = ostrcat(imdb->plot, tmdb->plot, 1, 0);
	if(imdb->released == NULL) imdb->released = ostrcat(imdb->released, tmdb->released, 1, 0);
	if(imdb->poster == NULL) imdb->poster = ostrcat(imdb->poster, tmdb->postermid, 1, 0);
	if(imdb->thumb == NULL) imdb->thumb = ostrcat(imdb->thumb, tmdb->thumb, 1, 0);
	if(imdb->year == NULL) imdb->year = ostrcat(imdb->year, tmdb->year, 1, 0);
}

void mediadbcpimdbapi2(struct imdbapi* imdbapi, struct imdb* imdb)
{
	if(imdbapi == NULL || imdb == NULL) return;

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
	if(imdb->year == NULL) imdb->year = ostrcat(imdb->year, imdbapi->year, 1, 0);
}

int mediadbjpegtran(char* tmpjpg, char* timestamp)
{
	char* cmd = NULL;

	if(tmpjpg == NULL || timestamp == NULL) return 1;

#ifdef MIPSEL_OLD_GIT
	cmd = ostrcat(cmd, "cp -a ", 1, 0);
	cmd = ostrcat(cmd, getconfig("mediadbpath", NULL), 1, 0);
	cmd = ostrcat(cmd, "/", 1, 0);
	cmd = ostrcat(cmd, timestamp, 1, 0);
	cmd = ostrcat(cmd, "_backdrop1.jpg", 1, 0);
	cmd = ostrcat(cmd, " ", 1, 0);
	cmd = ostrcat(cmd, tmpjpg, 1, 0);
#else
	cmd = ostrcat(cmd, "jpegtran -outfile ", 1, 0);
	cmd = ostrcat(cmd, tmpjpg, 1, 0);
	cmd = ostrcat(cmd, " -copy none ", 1, 0);
	cmd = ostrcat(cmd, getconfig("mediadbpath", NULL), 1, 0);
	cmd = ostrcat(cmd, "/", 1, 0);
	cmd = ostrcat(cmd, timestamp, 1, 0);
	cmd = ostrcat(cmd, "_backdrop1.jpg", 1, 0);
#endif

	debug(133, "cmd %s", cmd);
	system(cmd);
	free(cmd); cmd = NULL;

	return 0;
}

int mediadbmv(char* tmpmpg, char* timestamp)
{
	char* cmd = NULL;

	if(tmpmpg == NULL || timestamp == NULL) return 1;

	cmd = ostrcat(cmd, "mv -f ", 1, 0);
	cmd = ostrcat(cmd, tmpmpg, 1, 0);
	cmd = ostrcat(cmd, " ", 1, 0);
	cmd = ostrcat(cmd, getconfig("mediadbpath", NULL), 1, 0);
	cmd = ostrcat(cmd, "/", 1, 0);
	cmd = ostrcat(cmd, timestamp, 1, 0);
	cmd = ostrcat(cmd, "_backdrop1.mvi", 1, 0);

	debug(133, "cmd %s", cmd);
	system(cmd);
	free(cmd); cmd = NULL;

	return 0;
}

// flag 0 = autoscan
// flag 1 = manual scan imdb
// flag 2 = manual scan tmdb
// flag 3 = manual scan imdbapi
void mediadbfindfilecb(char* path, char* file, int type, char* id, int flag)
{
	debug(133, "path: %s",path);
	debug(133, "file: %s",file);
	debug(133, "type: %d", type);
	debug(133, "id: %s",id);
	debug(133, "flag: %d", flag);
	
	int isrec = 0;
	int iscam = 0;
	int backdrop = 0;
	char* shortpath = NULL, *tmpstr = NULL, *tmpid = NULL;
	struct mediadb *node = NULL, *cmediadb = NULL;

	if(id != NULL)
		tmpid = ostrcat(tmpid, id, 1, 0);
		
	shortpath = delmountpart(path, 0);
	if(shortpath == NULL) return; //no mountpart found

	//check if entry exist
	node = mediadbcheckfile(file, path, shortpath);

	int tout = getconfigint("mediadbscantimeout", NULL);

	if(node == NULL || (node != NULL && checkbit(node->flag, 31) == 0 && tout == 0) || (node != NULL && checkbit(node->flag, 31) == 0 && time(NULL) > node->timestamp + (tout * 86400)))
	{
		if(type == 0)
		{
			struct imdb* imdb = NULL;
			struct imdbapi* imdbapi = NULL;
			struct tmdb* tmdb = NULL;
			
			char* shortname = createshortname(file, &isrec, &iscam, 1);
			char* fileinfo = createshortname(file, &isrec, &iscam, 2);

			char* logdir = NULL, *logfile = NULL, *tmpjpg = NULL, *tmpmpg = NULL, *tmpmeta = NULL, *timen = NULL;

			logdir = ostrcat(getconfig("mediadbpath", NULL), "/.mediadbdebug", 0, 0);
			if(getconfigint("mediadbscandelall", NULL) == 1)
				unlink(logdir);
				
			if(!file_exist(logdir))
				mkdir(logdir, 0777);
			logfile = ostrcat(logdir, "/imdb-scan.log", 0, 0);

			timen = ostrcat(oitoa(time(NULL)), NULL, 1, 0);
			
			tmpjpg = ostrcat("/tmp/backdrop.resize.", timen, 0, 0);
			tmpjpg = ostrcat(tmpjpg, ".jpg", 1, 0);
			
			tmpmpg = ostrcat("/tmp/backdrop.resize.", timen, 0, 0);
#ifdef MIPSEL_OLD_GIT
			tmpmpg = ostrcat(tmpmpg, ".mvi", 1, 0);
#else
			tmpmpg = ostrcat(tmpmpg, ".mpg", 1, 0);
#endif			
			tmpmeta = ostrcat("/tmp/mediadb.", timen, 0, 0);
			tmpmeta = ostrcat(tmpmeta, ".meta", 1, 0);
			
			if(getconfigint("mediadbdebug", NULL) == 1)
				filedebug(logfile, "#############\nfile: %s\nshortname: %s\nfileinfo: %s\n#############", file, shortname, fileinfo);
	
			//got imdb infos
			struct skin* imdbplugin = getplugin("IMDb");
			if(imdbplugin != NULL)
			{
				struct imdb* (*startplugin)(struct imdb**, char*, int, int, int);
				startplugin = dlsym(imdbplugin->pluginhandle, "getimdb");
				if(startplugin != NULL)
				{
				 	if((flag == 1) || (flag == 2))
					{
						tmpid = string_replace("tt", "", tmpid, 1);
//						imdb = startplugin(&imdb, tmpid, 2, 0, 0); //load with imdbid with save
						imdb = startplugin(&imdb, tmpid, 2, 1, 0); //load with imdbid with save
					}
					else
						imdb = startplugin(&imdb, shortname, 0, 1, 0);
				}
			}

			struct skin* imdbapiplugin = NULL;
			struct skin* tmdbplugin = NULL;

			if(flag > 0 || (isrec == 0 && iscam == 0 && (imdb != NULL && imdb->id != NULL)))
			{
				if(flag == 2 && imdb != NULL && id != NULL)
					imdb->id = ostrcat(id, NULL, 0, 0);
	
				if(flag == 3 && imdb != NULL && id != NULL)
					imdb->id = ostrcat(id, NULL, 0, 0);
		
				imdbapiplugin = getplugin("IMDb-API");
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
	
				tmdbplugin = getplugin("TMDb");
				if(tmdbplugin != NULL)
				{
					struct tmdb* (*startplugin)(struct tmdb**, char*, int, int);
					startplugin = dlsym(tmdbplugin->pluginhandle, "gettmdb");
					if(startplugin != NULL)
					{
						if(imdb != NULL && imdb->id != NULL && flag > 0)
							tmdb = startplugin(&tmdb, imdb->id, 1, 2);
						else if(imdbapi != NULL && imdbapi->id != NULL && flag > 0)
							tmdb = startplugin(&tmdb, imdbapi->id, 1, 2);
						else if(imdb != NULL && imdb->id != NULL)
							tmdb = startplugin(&tmdb, imdb->id, 1, 1);
						else if(imdbapi != NULL && imdbapi->id != NULL)
							tmdb = startplugin(&tmdb, imdbapi->id, 1, 1);
					}
				}
	
				debugimdbnode(imdb);
				if(flag == 2) mediadbcptmdb(tmdb, imdb); // manuel tmdb
				debugimdbnode(imdb);
				if(flag == 3) mediadbcpimdbapi(imdbapi, imdb); // manuel imdbapi
				mediadbcptmdb2(tmdb, imdb);
				debugimdbnode(imdb);
				mediadbcpimdbapi2(imdbapi, imdb);

				if(tmdb != NULL && tmdb->mvi != NULL) backdrop = atoi(tmdb->mvi);
			}
			else if((cmpfilenameext(file, ".ts") == 0) || (cmpfilenameext(file, ".mts") == 0) || id == NULL)
			{
				char* poster = NULL, *plot = NULL, *timestamp = NULL, *cmd = NULL, *tmpstr1 = NULL;

				timestamp = oregex(".*([0-9]{14,14}).*", file);
				if(timestamp == NULL)
					timestamp = ostrcat(oitoa(time(NULL)), NULL, 1, 0);
				
				shortname = string_replace("   ", " - ", shortname, 1);
				shortname = string_replace("  ", " - ", shortname, 1);

				poster = ostrcat(getconfig("mediadbpath", NULL), "/", 0, 0);
				poster = ostrcat(poster, timestamp, 1, 0);
				poster = ostrcat(poster, "_poster.jpg", 1, 0);

				tmpstr = ostrcat(tmpstr, path, 1, 0);
				tmpstr = ostrcat(tmpstr, "/", 1, 0);
				tmpstr = ostrcat(tmpstr, file, 1, 0);
				tmpstr1 = changefilenameext(tmpstr, ".epg");
				free(tmpstr); tmpstr = NULL;
				cmd = readfiletomem(tmpstr1, 1);
				free(tmpstr1); tmpstr1 = NULL;
				plot = ostrcat(plot, cmd, 1, 0);
				free(cmd); cmd = NULL;

				if(file_exist("/tmp/screenshot_backdrop1.jpg"))
					mediadbcprec(timestamp, "_backdrop1.jpg");
				else
					mediadbffmpeg1(file, path, timestamp, logfile);

// bilder alle unscharf
/*
				int channels = 0;
				unsigned long width = 0, height = 0, rowbytes = 0;
				unsigned char* buf = NULL;

				cmd = ostrcat(cmd, getconfig("mediadbpath", NULL), 1, 0);
				cmd = ostrcat(cmd, "/", 1, 0);
				cmd = ostrcat(cmd, timestamp, 1, 0);
				cmd = ostrcat(cmd, "_backdrop1.jpg", 1, 0);
				buf = loadjpg(cmd, &width, &height, &rowbytes, &channels, 16);
				free(cmd), cmd = NULL;
				cmd = ostrcat(getconfig("mediadbpath", NULL), "/", 0, 0);
				cmd = ostrcat(cmd, timestamp, 1, 0);
				cmd = ostrcat(cmd, "_thumb.jpg", 1, 0);
				buf = savejpg(cmd, width, height, channels, 91, 140, 70, buf);
				free(cmd); cmd = NULL;
				free(buf); buf = NULL;

				cmd = ostrcat(cmd, getconfig("mediadbpath", NULL), 1, 0);
				cmd = ostrcat(cmd, "/", 1, 0);
				cmd = ostrcat(cmd, timestamp, 1, 0);
				cmd = ostrcat(cmd, "_backdrop1.jpg", 1, 0);
				buf = loadjpg(cmd, &width, &height, &rowbytes, &channels, 16);
				free(cmd), cmd = NULL;
				cmd = ostrcat(getconfig("mediadbpath", NULL), "/", 0, 0);
				cmd = ostrcat(cmd, timestamp, 1, 0);
				cmd = ostrcat(cmd, "_cover.jpg", 1, 0);
				buf = savejpg(cmd, width, height, channels, 185, 264, 70, buf);
				free(cmd); cmd = NULL;
				free(buf); buf = NULL;

				cmd = ostrcat(cmd, getconfig("mediadbpath", NULL), 1, 0);
				cmd = ostrcat(cmd, "/", 1, 0);
				cmd = ostrcat(cmd, timestamp, 1, 0);
				cmd = ostrcat(cmd, "_backdrop1.jpg", 1, 0);
				buf = loadjpg(cmd, &width, &height, &rowbytes, &channels, 16);
				free(cmd), cmd = NULL;
				cmd = ostrcat(getconfig("mediadbpath", NULL), "/", 0, 0);
				cmd = ostrcat(cmd, timestamp, 1, 0);
				cmd = ostrcat(cmd, "_poster.jpg", 1, 0);
				buf = savejpg(cmd, width, height, channels, 400, 450, 70, buf);
				free(cmd); cmd = NULL;
				free(buf); buf = NULL;

				cmd = ostrcat(cmd, getconfig("mediadbpath", NULL), 1, 0);
				cmd = ostrcat(cmd, "/", 1, 0);
				cmd = ostrcat(cmd, timestamp, 1, 0);
				cmd = ostrcat(cmd, "_backdrop1.jpg", 1, 0);
				buf = loadjpg(cmd, &width, &height, &rowbytes, &channels, 16);
				free(cmd), cmd = NULL;
				cmd = ostrcat(getconfig("mediadbpath", NULL), "/", 0, 0);
				cmd = ostrcat(cmd, timestamp, 1, 0);
				cmd = ostrcat(cmd, "_postermid.jpg", 1, 0);
				buf = savejpg(cmd, width, height, channels, 400, 450, 70, buf);
				free(cmd); cmd = NULL;
				free(buf); buf = NULL;
*/

				if(file_exist("/tmp/screenshot_thumb.jpg"))
					mediadbcprec(timestamp, "_thumb.jpg");
				else
					mediadbffmpeg2(file, path, timestamp, logfile);

				if(file_exist("/tmp/screenshot_cover.jpg"))
					mediadbcprec(timestamp, "_cover.jpg");
				else
					mediadbffmpeg3(file, path, timestamp, logfile);

				mediadbcp(timestamp, "_poster.jpg");
				mediadbcp(timestamp, "_postermid.jpg");

				cmd = ostrcat(cmd, getconfig("mediadbpath", NULL), 1, 0);
				cmd = ostrcat(cmd, "/", 1, 0);
				cmd = ostrcat(cmd, timestamp, 1, 0);
				cmd = ostrcat(cmd, "_backdrop1.jpg", 1, 0);
				
				off64_t filesize = getfilesize(cmd);
				debug(133, "filesize %lld", filesize);
				free(cmd); cmd = NULL;
			
				if(filesize < 1500000)
				{
					char* size = NULL;
#ifndef MIPSEL_OLD_GIT
					mediadbffmpeg4(timestamp, tmpmeta);
						
					cmd = ostrcat(cmd, "cat ", 1, 0);
					cmd = ostrcat(cmd, tmpmeta, 1, 0);
					cmd = ostrcat(cmd, " | grep Stream | awk '{print $6}' | cut -d'x' -f1", 1, 0);
					size = string_newline(command(cmd));
					free(cmd); cmd= NULL;
					debug(133, "size %s", size);
					if(size != NULL)
					{
						debug(133, "size %d", atoi(size));
						int picsize = atoi(size);
	
						if(picsize < 2000)
						{
							debug(133, "size ok %d", picsize);
#endif
							mediadbjpegtran(tmpjpg, timestamp);
						
							if(file_exist(tmpjpg))
							{
								mediadbffmpeg5(tmpjpg, tmpmpg, timestamp, file, size, filesize, logfile);

								if(file_exist(tmpmpg))
								{
									mediadbmv(tmpmpg, timestamp);
									backdrop = 1;
									writesysint("/proc/sys/vm/drop_caches", 3, 0);

									debug(777, "imdb id %s", timestamp);
									if(cmediadb == NULL) cmediadb = createmediadb(node, timestamp, type, shortname, NULL, NULL, NULL, NULL, NULL, NULL, NULL, plot, poster, NULL, NULL, shortpath, file, shortname, fileinfo, 0, backdrop);
								}	
							}
#ifndef MIPSEL_OLD_GIT
						}
						else
						{
							debug(133, "ERROR Localfile size to big skipped %d", picsize);
	
							if(getconfigint("mediadbdebug", NULL) == 1)
								filedebug(logfile, "#############\nERROR Localfile size to big skipped: %s/%s_backdrop1.jpg size=(%s) filesize(%lld) (%s)\n#############", getconfig("mediadbpath", NULL), timestamp, size, filesize, file);
						}
					}
					else
					{
						debug(133, "ERROR Localfile size is NULL skipped %s", size);
	
						if(getconfigint("mediadbdebug", NULL) == 1)
							filedebug(logfile, "#############\nERROR Localfile size is NULL skipped: %s/%s_backdrop1.jpg size=(%s) filesize(%lld) (%s)\n#############", getconfig("mediadbpath", NULL), timestamp, size, filesize, file);
					}
					free(size); size = NULL;
#endif
					unlink(tmpmeta);
					unlink(tmpjpg);
					unlink(tmpmpg);
					
					cmd = ostrcat(cmd, getconfig("mediadbpath", NULL), 1, 0);
					cmd = ostrcat(cmd, "/", 1, 0);	
					cmd = ostrcat(cmd, timestamp, 1, 0);
					cmd = ostrcat(cmd, "_backdrop1.jpg", 1, 0);
					unlink(cmd);
				}
				else
				{
					debug(133, "ERROR Localfile filesize to BIG skipped %lld", filesize);
		
					if(getconfigint("mediadbdebug", NULL) == 1)
						filedebug(logfile, "#############\nERROR Localfile filesize to BIG skipped: %s/%s_backdrop1.jpg filesize(%lld) (%s)\n#############", getconfig("mediadbpath", NULL), timestamp, filesize, file);
				}
				free(cmd); cmd = NULL;
				free(timestamp); timestamp = NULL;
				free(poster); poster = NULL;
				free(plot); plot = NULL;
			}
	
			free(logdir); logdir = NULL;
			free(logfile); logfile = NULL;
			free(timen); timen = NULL;
			free(tmpjpg); tmpjpg = NULL;
			free(tmpmpg); tmpmpg = NULL;
			free(tmpmeta); tmpmeta = NULL;

			debugimdbnode(imdb);
			
			if(cmediadb == NULL)
			{
				debug(777, "add video: %s/%s", shortpath, file);
				if(imdb != NULL)
				{
					debug(777, "imdb id %s", imdb->id);
					cmediadb = createmediadb(node, imdb->id, type, imdb->title, imdb->year, imdb->released, imdb->runtime, imdb->genre, imdb->director, imdb->writer, imdb->actors, imdb->plot, imdb->id, imdb->rating, imdb->votes, shortpath, file, shortname, fileinfo, 0, backdrop);
					if(tmpid != NULL)
					{
						char* tmpstr = NULL;
						tmpstr = ostrcat(tmpstr, _("file"), 1, 0);
						tmpstr = ostrcat(tmpstr, ": ", 1, 0);
						tmpstr = ostrcat(tmpstr, file, 1, 0);							
						tmpstr = ostrcat(tmpstr, "\n", 1, 0);
						tmpstr = ostrcat(tmpstr, _("path"), 1, 0);
						tmpstr = ostrcat(tmpstr, ": ", 1, 0);
						tmpstr = ostrcat(tmpstr, path, 1, 0);							
						tmpstr = ostrcat(tmpstr, "\n", 1, 0);
						tmpstr = ostrcat(tmpstr, _("Title"), 1, 0);
						tmpstr = ostrcat(tmpstr, ": ", 1, 0);
						tmpstr = ostrcat(tmpstr, imdb->title, 1, 0);							
						tmpstr = ostrcat(tmpstr, "\n", 1, 0);
						tmpstr = ostrcat(tmpstr, _("imdbid"), 1, 0);
						tmpstr = ostrcat(tmpstr, ": ", 1, 0);
						tmpstr = ostrcat(tmpstr, id, 1, 0);																										
						textbox(_("Add iMDB manuel"), tmpstr, _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 800, 500, 10, 0);
						free(tmpstr); tmpstr = NULL;
					}
				}
				else
					cmediadb = createmediadb(node, NULL, type, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, shortpath, file, shortname, fileinfo, 0, 0);
			}

			debug(777, "shortname: %s", shortname);
			debug(133, "shortname: %s", shortname);
			free(shortname); shortname = NULL;

			debug(777, "fileinfo: %s", fileinfo);
			debug(133, "fileinfo: %s", fileinfo);
			free(fileinfo); fileinfo = NULL;
						
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
			struct id3tag* id3tag = NULL;

			debug(777, "add audio: %s/%s", shortpath, file);

			// create filelist info
			free(tmpstr), tmpstr = NULL;
			tmpstr = ostrcat(tmpstr, file, 1, 0);
			string_tolower(tmpstr);
			char* fileinfo = NULL;
			fileinfo = ostrcat(fileinfo, getfilenameext(tmpstr), 1, 1);
			free(tmpstr), tmpstr = NULL;

			char* tmpfile = ostrcat(path, "/", 0, 0);
			tmpfile = ostrcat(tmpfile, file, 1, 0);

			int hash = gethash(tmpfile);
			char* tmphash = olutoa(hash);

			id3tag = getid3(tmpfile, tmphash, 1);

			if(id3tag != NULL)
			{
				if(id3tag->poster != NULL)
					cmediadb = createmediadb(node, tmphash, type, id3tag->title, id3tag->year, NULL, NULL, id3tag->genretext, NULL, NULL, id3tag->artist, id3tag->album, tmphash, NULL, NULL, shortpath, file, NULL, fileinfo, 0, 0);
				else
					cmediadb = createmediadb(node, tmphash, type, id3tag->title, id3tag->year, NULL, NULL, id3tag->genretext, NULL, NULL, id3tag->artist, id3tag->album, NULL, NULL, NULL, shortpath, file, NULL, fileinfo, 0, 0);
			}
			else
				cmediadb = createmediadb(node, NULL, type, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, shortpath, file, NULL, NULL, 0, 0);

			free(tmpfile); tmpfile = NULL;
			free(tmphash); tmphash = NULL;
			free(fileinfo); fileinfo = NULL;
			freeid3(id3tag); id3tag = NULL;
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

			cmediadb = createmediadb(node, NULL, type, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, shortpath, file, NULL, NULL, 0, 0);
		}
	}
	free(shortpath); shortpath = NULL;

	//first time save full mediadb, rest save only akt entry
	if(status.mediadbsavetime == 0)
	{
		writemediadb(getconfig("mediadbfile", NULL), NULL);
		status.mediadbsavetime = 1;
	}
	else if(cmediadb != NULL)
		writemediadb(getconfig("mediadbfile", NULL), cmediadb);
}

int findfiles(char* dirname, int type, int onlydir, int onlycount, int first)
{
	debug(777, "dir=%s type=%d onlydir=%d, onlycount=%d", dirname, type, onlydir, onlycount);
	DIR *d;
	char* tmpstr = NULL;

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

		//for nfs mounts if file type is unknown use stat
		if(entry->d_type == DT_UNKNOWN)
		{
			tmpstr = ostrcat(dirname, "/", 0, 0);
			tmpstr = ostrcat(tmpstr, entry->d_name, 1, 0);
			entry->d_type = getfiletype(tmpstr);
			free(tmpstr); tmpstr = NULL;
		}

		//check if link is a dir
		if(first == 1 && entry->d_type == DT_LNK)
		{
			tmpstr = createpath(path, entry->d_name);
			if(isdir(tmpstr) == 1)
				entry->d_type = DT_DIR;

			free(tmpstr); tmpstr = NULL;
		}

		//See if entry is a subdirectory of d
		if(entry->d_type == DT_DIR)
		{
			//Check that the directory is not d or d's parent
			if(entry->d_name != NULL && entry->d_name[0] != '.' && entry->d_name[0] != '$')
			{
				path_length = snprintf(path, PATH_MAX, "%s/%s", dirname, entry->d_name);
				if(path_length >= PATH_MAX)
				{
					err("path length has got too long");
					if(d) closedir(d);
					return -1;
				}
				//Recursively call findfiles with the new path
				if(onlydir == 0)
					findfiles(path, type, onlydir, onlycount, 0);
			}
		}
		else //File
		{
			//TODO: add extensions
			
			if(file_exist("/mnt/swapextensions/etc/.codecpack") || file_exist("/var/swap/etc/.codecpack") || file_exist("/var/etc/.codecpack"))
			{
				if(!filelistflt(".avi .mkv .mp4 .ts .mov .flv .mpg .m2ts .vob .evo .wmv .asf .mp2 .m4v .fla .divx .dat .mpeg .trp .mts .vdr .ogg .wtv .asx .mvi .ram .rm .3gp .rmvb .rm .webm .opus .mpd", entry->d_name)) //video
				{				
					if(type == 0 || type == 100 || type == 90 || type == 91)
					{					
						if(onlycount == 0)
							mediadbfindfilecb(path, entry->d_name, 0, NULL, 0);
						else
							count += 1;
					}
				}
				else if(!filelistflt(".aac .flac .mp3 .wma .m4a .ogg .wav .ra .amr", entry->d_name)) //audio
				{
					if(type == 1 || type == 100 || type == 90 || type == 92)
					{
						if(onlycount == 0)
							mediadbfindfilecb(path, entry->d_name, 1, NULL, 0);
						else
							count += 1;
					}
				}
				else if(!filelistflt(".jpg .png", entry->d_name)) //picture
				{
					if(type == 2 || type == 100 || type == 91 || type == 92)
					{
						if(onlycount == 0)
							mediadbfindfilecb(path, entry->d_name, 2, NULL, 0);
						else
							count += 1;
					}
				}
			}
			else
			{
				if(!filelistflt(".avi .mkv .mpg .mpeg .ts", entry->d_name)) //video
				{				
					if(type == 0 || type == 100 || type == 90 || type == 91)
					{
						if(onlycount == 0)
							mediadbfindfilecb(path, entry->d_name, 0, NULL, 0);
						else
							count += 1;
					}
				}
				else if(!filelistflt(".mp3 .flac .ogg", entry->d_name)) //audio
				{
					if(type == 1 || type == 100 || type == 90 || type == 92)
					{
						if(onlycount == 0)
							mediadbfindfilecb(path, entry->d_name, 1, NULL, 0);
						else
							count += 1;
					}
				}
				else if(!filelistflt(".jpg .png", entry->d_name)) //picture
				{
					if(type == 2 || type == 100 || type == 91 || type == 92)
					{
						if(onlycount == 0)
							mediadbfindfilecb(path, entry->d_name, 2, NULL, 0);
						else
							count += 1;
					}
				}
			}
		}
	}

	//After going through all the entries, close the directory
	if(d && closedir(d))
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
