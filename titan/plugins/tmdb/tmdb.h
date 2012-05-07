#ifndef TMDB_H
#define TMDB_H

#define TMPTMDBPIC "/tmp/tmptmdb.jpg"

struct tmdb
{
	char* title;
	char* year;
	char* rated;
	char* released;
	char* genre;
	char* director;
	char* writer;
	char* actors;
	char* plot;
	char* poster;
	char* runtime;
	char* rating;
	char* votes;
	char* id;
};

void freetmdb(struct tmdb* node)
{
	if(node == NULL) return;

	free(node->title); node->title = NULL;
	free(node->year); node->year = NULL;
	free(node->rated); node->rated = NULL;
	free(node->released); node->released = NULL;
	free(node->genre); node->genre = NULL;
	free(node->director); node->director = NULL;
	free(node->writer); node->writer = NULL;
	free(node->actors); node->actors = NULL;
	free(node->plot); node->plot = NULL;
	free(node->poster); node->poster = NULL;
	free(node->runtime); node->runtime = NULL;
	free(node->rating); node->rating = NULL;
	free(node->votes); node->votes = NULL;
	free(node->id); node->id = NULL;
	unlink(TMPTMDBPIC);

	free(node); node = NULL;
}

struct tmdb* gettmdb(char* title)
{
	struct tmdb* tmdb = NULL;
	char* tmpstr = NULL;
	char* tmpsearch = NULL;
	char* apikey = NULL;
	
//http://api.themoviedb.org/2.1/Movie.search/de/xml/7bcd34bb47bc65d20a49b6b446a32866/Transformers
	apikey = ostrcat(apikey, "7bcd34bb47bc65d20a49b6b446a32866", 1, 0);
	tmpsearch = ostrcat("2.1/Movie.search/de/xml/", apikey, 0, 1);
	tmpsearch = ostrcat(tmpsearch, "/", 1, 0);
	tmpsearch = ostrcat(tmpsearch, title, 1, 0);

	tmpsearch = stringreplacechar(tmpsearch, ' ', '+');
printf("tmpsearch: %s\n",tmpsearch);
	tmpstr = gethttp("api.themoviedb.org", tmpsearch, 80, NULL, NULL, NULL, 0);
	free(tmpsearch); tmpsearch = NULL;

	if(tmpstr != NULL)
	{
		tmdb = (struct tmdb*)malloc(sizeof(struct tmdb));
		if(tmdb == NULL)
		{
			err("no mem");
			free(tmpstr); tmpstr = NULL;
			return NULL;
		}
		memset(tmdb, 0, sizeof(struct tmdb));

		tmdb->title = string_resub("<name>", "</name>", tmpstr);
		tmdb->year = string_resub("<name>", "</name>", tmpstr);
		tmdb->rated = string_resub("<name>", "</name>", tmpstr);
		tmdb->released = string_resub("<name>", "</name>", tmpstr);
		tmdb->genre = string_resub("<name>", "</name>", tmpstr);
		tmdb->director = string_resub("<name>", "</name>", tmpstr);
		tmdb->writer = string_resub("<name>", "</name>", tmpstr);
		tmdb->actors = string_resub("<name>", "</name>", tmpstr);
		tmdb->plot = string_resub("<overview>", "</overview>", tmpstr);
		tmdb->poster = string_resub("<name>", "</name>", tmpstr);
		tmdb->runtime = string_resub("<name>", "</name>", tmpstr);
		tmdb->rating = string_resub("<name>", "</name>", tmpstr);
		tmdb->votes = string_resub("<votes>", "</votes>", tmpstr);
		tmdb->id = string_resub("<id>", "</id>", tmpstr);

		if(tmdb->poster != NULL)
		{
			char* ip = NULL, *pos = NULL, *path = NULL;
			ip = string_replace("http://", "", tmdb->poster, 0);

			if(ip != NULL)
				pos = strchr(ip, '/');
			if(pos != NULL)
			{
				pos[0] = '\0';
				path = pos + 1;
			}

			gethttp(ip, path, 80, TMPTMDBPIC, NULL, NULL, 0);
			free(ip); ip = NULL;

		}
		free(tmpstr); tmpstr = NULL;
	}

	return tmdb;
}

void screentmdb(char* title)
{
	int rcret = 0;
	struct skin* tmdbskin = getscreen("tmdb");
	struct skin* skin_plot = getscreennode(tmdbskin, "plot");
	struct skin* skin_title = getscreennode(tmdbskin, "title");
	struct skin* skin_director = getscreennode(tmdbskin, "director");
	struct skin* skin_writers = getscreennode(tmdbskin, "writers");
	struct skin* skin_genre = getscreennode(tmdbskin, "genre");
	struct skin* skin_releasetime = getscreennode(tmdbskin, "releasetime");
	struct skin* skin_cover = getscreennode(tmdbskin, "cover");
	struct skin* skin_actors = getscreennode(tmdbskin, "actors");
	struct tmdb* node = NULL;
	char* search = NULL;

	setfbtransparent(255);
	status.hangtime = 99999;

	if(title == NULL) title = getepgakttitle(NULL);

	node = gettmdb(title);
start:
	if(node != NULL)
	{
		changetext(skin_plot, node->plot);
		changetext(skin_title, node->title);
		changetext(skin_director, node->director);
		changetext(skin_writers, node->writer);
		changetext(skin_genre, node->genre);
		changetext(skin_releasetime, node->released);
		changetext(skin_actors, node->actors);
		changepic(skin_cover, TMPTMDBPIC);
	}

	drawscreen(tmdbskin, 0);

	while(1)
	{
		rcret = waitrc(tmdbskin, 0, 0);
	
		if(rcret == getrcconfigint("rcexit", NULL)) break;
		if(rcret == getrcconfigint("rcok", NULL)) break;

		if(rcret == getrcconfigint("rcred", NULL))
    {
      search = textinput("Search", NULL);
      if(search != NULL)
      {
				freetmdb(node); node = NULL;
				node = gettmdb(search);
        free(search); search = NULL;
				goto start;
      }
      drawscreen(tmdbskin, 0);
      continue;
    }
	}

	freetmdb(node); node = NULL;
	setosdtransparent(getskinconfigint("osdtransparent", NULL));
	status.hangtime = getconfigint("hangtime", NULL);
	clearscreen(tmdbskin);
}

#endif
