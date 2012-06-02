#ifndef IMDB_H
#define IMDB_H

#define TMPIMDBPIC "/tmp/tmpimdb.jpg"
extern struct imdb* imdb;

void freeimdb(struct imdb* node)
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
	unlink(TMPIMDBPIC);

	free(node); node = NULL;
}

//flag 0 = title search
//flag 1 = imdbid search
//flag1 0 = save in tmp
//flag1 1 = save in imdbpath
//flag2 0 = get pic
//flag2 1 = don't get pic
struct imdb* getimdb(char* title, int flag, int flag1, int flag2)
{
	struct imdb* imdb = NULL;
	char* tmpstr = NULL;
	char* tmpsearch = NULL;
	char* savefile = NULL;

	if(flag == 0)
		tmpsearch = ostrcat("?i=&t=", title, 0, 0);
	else
		tmpsearch = ostrcat("?i=", title, 0, 0);

	tmpsearch = stringreplacechar(tmpsearch, ' ', '+');

	tmpstr = gethttp("www.imdbapi.com", tmpsearch, 80, NULL, NULL, NULL, 0);
	free(tmpsearch); tmpsearch = NULL;

	if(tmpstr != NULL)
	{
		imdb = (struct imdb*)malloc(sizeof(struct imdb));
		if(imdb == NULL)
		{
			err("no mem");
			free(tmpstr); tmpstr = NULL;
			return NULL;
		}
		memset(imdb, 0, sizeof(struct imdb));

		imdb->title = getxmlentry(tmpstr, "\"Title\":");
		imdb->year = getxmlentry(tmpstr, "\"Year\":");
		imdb->rated = getxmlentry(tmpstr, "\"Rated\":");
		imdb->released = getxmlentry(tmpstr, "\"Released\":");
		imdb->genre = getxmlentry(tmpstr, "\"Genre\":");
		imdb->director = getxmlentry(tmpstr, "\"Director\":");
		imdb->writer = getxmlentry(tmpstr, "\"Writer\":");
		imdb->actors = getxmlentry(tmpstr, "\"Actors\":");
		imdb->plot = getxmlentry(tmpstr, "\"Plot\":");
		imdb->poster = getxmlentry(tmpstr, "\"Poster\":");
		imdb->runtime = getxmlentry(tmpstr, "\"Runtime\":");
		imdb->rating = getxmlentry(tmpstr, "\"imdbRating\":");
		imdb->votes = getxmlentry(tmpstr, "\"imdbVotes\":");
		imdb->id = getxmlentry(tmpstr, "\"imdbID\":");

		if(imdb->poster != NULL && flag2 == 0)
		{
			char* ip = NULL, *pos = NULL, *path = NULL;
			ip = string_replace("http://", "", imdb->poster, 0);

			if(ip != NULL)
				pos = strchr(ip, '/');
			if(pos != NULL)
			{
				pos[0] = '\0';
				path = pos + 1;
			}

			if(flag1 == 1)
			{
				savefile = ostrcat(getconfig("mediadbpath", NULL), "/", 0, 0);
				savefile = ostrcat(savefile, imdb->id, 1, 0);
				savefile = ostrcat(savefile, "_poster.jpg", 1, 0);
				gethttp(ip, path, 80, savefile, NULL, NULL, 0);
				free(imdb->poster);
				imdb->poster = savefile;
			}
			else
			{
				gethttp(ip, path, 80, TMPIMDBPIC, NULL, NULL, 0);
				free(imdb->poster);
				imdb->poster = ostrcat(TMPIMDBPIC, NULL, 0, 0);
			}

			free(ip); ip = NULL;
		}
		else
		{
			free(imdb->poster);
			imdb->poster = NULL;
		}

		free(tmpstr); tmpstr = NULL;
	}

	return imdb;
}

void screenimdb(char* title)
{
	int rcret = 0;
	struct skin* imdbskin = getscreen("imdb");
	struct skin* skin_plot = getscreennode(imdbskin, "plot");
	struct skin* skin_title = getscreennode(imdbskin, "title");
	struct skin* skin_director = getscreennode(imdbskin, "director");
	struct skin* skin_writers = getscreennode(imdbskin, "writers");
	struct skin* skin_genre = getscreennode(imdbskin, "genre");
	struct skin* skin_releasetime = getscreennode(imdbskin, "releasetime");
	struct skin* skin_cover = getscreennode(imdbskin, "cover");
	struct skin* skin_actors = getscreennode(imdbskin, "actors");
	struct imdb* node = NULL;
	char* search = NULL;

	setfbtransparent(255);
	status.hangtime = 99999;

	if(title == NULL) title = getepgakttitle(NULL);

	node = getimdb(title, 0, 0, 0);
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
		changepic(skin_cover, node->poster);
	}

	drawscreen(imdbskin, 0);

	while(1)
	{
		rcret = waitrc(imdbskin, 0, 0);
	
		if(rcret == getrcconfigint("rcexit", NULL)) break;
		if(rcret == getrcconfigint("rcok", NULL)) break;

		if(rcret == getrcconfigint("rcred", NULL))
    {
      search = textinput("Search", NULL);
      if(search != NULL)
      {
				freeimdb(node); node = NULL;
				node = getimdb(search, 0, 0, 0);
        free(search); search = NULL;
				goto start;
      }
      drawscreen(imdbskin, 0);
      continue;
    }
	}

	freeimdb(node); node = NULL;
	setosdtransparent(getskinconfigint("osdtransparent", NULL));
	status.hangtime = getconfigint("hangtime", NULL);
	clearscreen(imdbskin);
}

#endif
