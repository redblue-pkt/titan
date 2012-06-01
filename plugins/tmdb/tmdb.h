#ifndef TMDB_H
#define TMDB_H

#define TMPTMDBPIC "/tmp/tmptmdb.jpg"

char* convertreturntopspace(char* value)
{
	debug(1000, "in");
	int i = 0;

	while(value[i] != '\0')
	{
		if(value[i] == '\n')
			value[i] = ' ';
		i++;
	}

	debug(1000, "out");
	return value;
}

struct tmdb
{
	char* title;
	char* year;
	char* rated;
	char* released;
	char* genre;
	char* runtime;
	char* plot;
	char* poster;
	char* rating;
	char* votes;
	char* id;
	char* backdrop;
	char* name;
	char* imdbid;
	char* orgname;
	char* language;
	char* type;
	char* score;
	char* cover;
	char* thumb;
};

void freetmdb(struct tmdb* node)
{
	if(node == NULL) return;

	free(node->title); node->title = NULL;
	free(node->year); node->year = NULL;
	free(node->rated); node->rated = NULL;
	free(node->released); node->released = NULL;
	free(node->genre); node->genre = NULL;
	free(node->runtime); node->runtime = NULL;
	free(node->plot); node->plot = NULL;
	free(node->poster); node->poster = NULL;
	free(node->rating); node->rating = NULL;
	free(node->votes); node->votes = NULL;
	free(node->id); node->id = NULL;
	free(node->orgname); node->orgname = NULL;
	free(node->name); node->name = NULL;
	free(node->imdbid); node->imdbid = NULL;
	free(node->backdrop); node->backdrop = NULL;
	free(node->language); node->language = NULL;
	free(node->type); node->type = NULL;
	free(node->score); node->score = NULL;
	free(node->cover); node->cover = NULL;
	free(node->thumb); node->thumb = NULL;

	unlink(TMPTMDBPIC);

	free(node); node = NULL;
}

// flag 0 = title search
// flag 1 = imdbid search
// flag 2 = imdbid search and save

struct tmdb* gettmdb(char* title, int flag)
{
	struct tmdb* tmdb = NULL;
	char* tmpstr = NULL;
	char* tmpsearch = NULL;
	char* apikey = NULL;
	char* searchurl = NULL;
	char* imdburl = NULL;
	char* lang = NULL;
	char* path = NULL;
	char* savefile = NULL;
	char* cmd = NULL;

	int skip = 1;
			
	apikey = ostrcat(apikey, "7bcd34bb47bc65d20a49b6b446a32866", 1, 0);
	searchurl = ostrcat(searchurl, "Movie.search", 1, 0);
	imdburl = ostrcat(imdburl, "Movie.imdbLookup", 1, 0);
	lang = ostrcat(lang, "de", 1, 0);
	
start:
	tmpsearch = ostrcat("2.1/", NULL, 0, 0);
	if(flag == 0)
		tmpsearch = ostrcat(tmpsearch, searchurl, 1, 0);
	else
		tmpsearch = ostrcat(tmpsearch, imdburl, 1, 0);
		
	tmpsearch = ostrcat(tmpsearch, "/", 1, 0);
	tmpsearch = ostrcat(tmpsearch, lang, 1, 0);
	tmpsearch = ostrcat(tmpsearch, "/xml/", 1, 0);
	tmpsearch = ostrcat(tmpsearch, apikey, 1, 0);
	tmpsearch = ostrcat(tmpsearch, "/", 1, 0);
	tmpsearch = ostrcat(tmpsearch, title, 1, 0);
	tmpsearch = stringreplacechar(tmpsearch, ' ', '+');

	tmpstr = gethttp("api.themoviedb.org", tmpsearch, 80, NULL, NULL, NULL, 0);
	
	debug(133, "tmpsearch: %s", tmpsearch);
	debug(133, "tmpstr: %s", tmpstr);
				
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
		if(string_find("<movies>Nothing found.</movies>", tmpstr))
		{
			free(tmpstr), tmpstr = NULL;
			tmdb->title = ostrcat("Nothing found.", NULL, 0, 0);
		}
		else if(!string_find("<opensearch:totalResults>1</opensearch:totalResults>", tmpstr))
		{
			char* tmpstr1 = NULL;			
			char* tmpstr2 = NULL;
			char* tmpstr3 = NULL;

			tmpstr1 = ostrcat(tmpstr, NULL, 0, 0);
	
			struct menulist* mlist = NULL, *mbox = NULL;
		
			int count = 0;
			while(string_find("<imdb_id>", tmpstr1))
			{
				tmpstr2 = string_resub("<imdb_id>", "</imdb_id>", tmpstr1);				
				tmpstr3 = string_resub("<name>", "</name>", tmpstr1);

				debug(133, "result(%d) %s - %s", count, tmpstr2, tmpstr3);
				addmenulist(&mlist, tmpstr3, tmpstr2, NULL, 0, 0);
				tmpstr1 = string_replace("<imdb_id>", " ", tmpstr1, 1);
				tmpstr1 = string_replace("<name>", " ", tmpstr1, 1);
				free(tmpstr2), tmpstr2 = NULL;
				free(tmpstr3), tmpstr3 = NULL;
			}

			mbox = menulistbox(mlist, NULL, NULL, NULL, NULL, 1, 0);
			if(mbox != NULL)
			{
				free(title), title = NULL;	
				free(tmpstr), tmpstr = NULL;				
				free(tmpstr1), tmpstr1 = NULL;

				debug(133, "mbox->name %s", mbox->name);
				debug(133, "mbox->text %s", mbox->text);
						
				title = ostrcat(mbox->text, NULL, 0, 0);
				flag = 1;
				goto start;
			}
					
		}			
	}
	
	if(tmpstr != NULL && string_find("<opensearch:totalResults>1</opensearch:totalResults>", tmpstr))
	{
		if(string_find("<name>", tmpstr))
		{
			tmdb->title = string_resub("<name>", "</name>", tmpstr);
		}

		if(string_find("<language>", tmpstr))
		{
			tmdb->language = string_resub("<language>", "</language>", tmpstr);
		}

		if(string_find("<type>", tmpstr))
		{
			tmdb->type = string_resub("<type>", "</type>", tmpstr);
		}

		if(string_find("<original_name>", tmpstr))
		{
			tmdb->orgname = string_resub("<original_name>", "</original_name>", tmpstr);
		}

		if(string_find("<score>", tmpstr))
		{
			tmdb->score = string_resub("<score>", "</score>", tmpstr);
		}

		if(string_find("<rating>", tmpstr))
		{
			tmdb->rated = string_resub("<rating>", "</rating>", tmpstr);
		}

		if(string_find("<released>", tmpstr))
		{
			tmdb->released = string_resub("<released>", "</released>", tmpstr);
		}

		if(string_find("<categories>", tmpstr))
		{
			tmdb->genre = string_resub("<category type=\"genre\" name=\"", "\" url=", tmpstr);
			while(string_find("<category type=", tmdb->genre))
			{
				tmdb->genre = string_resub("<category type=\"genre\" name=\"", "\" url=", tmdb->genre);
			}
		}

		if(string_find("<runtime>", tmpstr))
		{
			tmdb->runtime = string_resub("<runtime>", "</runtime>", tmpstr);
		}

		if(string_find("<overview>", tmpstr))
		{
			tmdb->plot = string_resub("<overview>", "</overview>", tmpstr);
		}

		if(string_find("type=\"poster\"", tmpstr))
		{
			tmdb->thumb = string_resub("<image type=\"poster\" url=\"", "\" size=\"thumb\"", tmpstr);
			while(string_find("<image type=", tmdb->thumb))
			{
				tmdb->thumb = string_resub("<image type=\"poster\" url=\"", "\" size=\"thumb\"", tmdb->thumb);
			}
		}

		if(string_find("type=\"poster\"", tmpstr))
		{
			tmdb->cover = string_resub("<image type=\"poster\" url=\"", "\" size=\"cover\"", tmpstr);
			while(string_find("<image type=", tmdb->cover))
			{
				tmdb->cover = string_resub("<image type=\"poster\" url=\"", "\" size=\"cover\"", tmdb->cover);
			}
		}

		if(string_find("type=\"poster\"", tmpstr))
		{
			tmdb->poster = string_resub("<image type=\"poster\" url=\"", "\" size=\"mid\"", tmpstr);
			while(string_find("<image type=", tmdb->poster))
			{
				tmdb->poster = string_resub("<image type=\"poster\" url=\"", "\" size=\"mid\"", tmdb->poster);
			}
		}

		if(string_find("type=\"backdrop\"", tmpstr))
		{
			tmdb->backdrop = string_resub("<image type=\"backdrop\" url=\"", "\" size=\"original\"", tmpstr);
			while(string_find("<image type=", tmdb->backdrop))
			{
				tmdb->backdrop = string_resub("<image type=\"backdrop\" url=\"", "\" size=\"original\"", tmdb->backdrop);
			}
		}

		if(string_find("<rating>", tmpstr))
		{
			tmdb->rating = string_resub("<rating>", "</rating>", tmpstr);
		}

		if(string_find("<votes>", tmpstr))
		{
			tmdb->votes = string_resub("<votes>", "</votes>", tmpstr);
		}

		if(string_find("<id>", tmpstr))
		{
			tmdb->id = string_resub("<id>", "</id>", tmpstr);
		}

		if(string_find("<imdb_id>", tmpstr))
		{
			tmdb->imdbid = string_resub("<imdb_id>", "</imdb_id>", tmpstr);
		}

		if(flag == 2)
		{
			path = ostrcat(path, getconfig("mediadb", NULL), 1, 0);
			if(path != NULL && file_exist(path))
			{
				path = ostrcat(path, "/mediadb", 1, 0);					
				if(!file_exist(path))
				{
					cmd = ostrcat("mkdir ", path, 0, 0);
					system(cmd);
					free(cmd), cmd = NULL;
				}
				if(file_exist(path))
					skip = 0;
			}
		}
						
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

			if(flag == 2 && skip == 0)
			{
				savefile = ostrcat(path, "/", 0, 0);
				savefile = ostrcat(savefile, tmdb->id, 1, 0);
				savefile = ostrcat(savefile, "_poster_mid.jpg", 1, 0);
				if(!file_exist(savefile))
					gethttp(ip, path, 80, savefile, NULL, NULL, 0);
				free(savefile), savefile = NULL;
			}
			else
				gethttp(ip, path, 80, TMPTMDBPIC, NULL, NULL, 0);

			free(ip); ip = NULL;
		}
		
		if(flag == 2 && skip == 0)
		{
			if(tmdb->thumb != NULL)
			{
				char* ip = NULL, *pos = NULL, *path = NULL;
				ip = string_replace("http://", "", tmdb->thumb, 0);
	
				if(ip != NULL)
					pos = strchr(ip, '/');
				if(pos != NULL)
				{
					pos[0] = '\0';
					path = pos + 1;
				}
	
				savefile = ostrcat(path, "/", 0, 0);
				savefile = ostrcat(savefile, tmdb->id, 1, 0);
				savefile = ostrcat(savefile, "_poster_thumb.jpg", 1, 0);
		
				if(!file_exist(savefile))
					gethttp(ip, path, 80, savefile, NULL, NULL, 0);
	
				free(savefile), savefile = NULL;
				free(ip); ip = NULL;
			}
	
			if(tmdb->cover != NULL && flag == 2)
			{
				char* ip = NULL, *pos = NULL, *path = NULL;
				ip = string_replace("http://", "", tmdb->cover, 0);
	
				if(ip != NULL)
					pos = strchr(ip, '/');
				if(pos != NULL)
				{
					pos[0] = '\0';
					path = pos + 1;
				}
	
				savefile = ostrcat(path, "/", 0, 0);
				savefile = ostrcat(savefile, "/", 1, 0);
				savefile = ostrcat(savefile, tmdb->id, 1, 0);
				savefile = ostrcat(savefile, "_poster_cover.jpg", 1, 0);
		
				if(!file_exist(savefile))
					gethttp(ip, path, 80, savefile, NULL, NULL, 0);
	
				free(savefile), savefile = NULL;
				free(ip); ip = NULL;
			}
	
			if(tmdb->backdrop != NULL && flag == 2)
			{
				char* ip = NULL, *pos = NULL, *path = NULL;
				ip = string_replace("http://", "", tmdb->backdrop, 0);
	
				if(ip != NULL)
					pos = strchr(ip, '/');
				if(pos != NULL)
				{
					pos[0] = '\0';
					path = pos + 1;
				}
	
				savefile = ostrcat(path, "/", 0, 0);
				savefile = ostrcat(savefile, tmdb->id, 1, 0);
				savefile = ostrcat(savefile, "_backdrop.jpg", 1, 0);
		
				if(!file_exist(savefile))
				{
					gethttp(ip, path, 80, savefile, NULL, NULL, 0);
					if(file_exist(savefile))
					{
						cmd = ostrcat(cmd, "jpegtran -outfile /tmp/backdrop.resize.jpg -copy none ", 1, 0);
						cmd = ostrcat(cmd, savefile, 1, 0);
						system(cmd);
						free(cmd), cmd = NULL;
						system('ffmpeg -y -f image2 -i /tmp/backdrop.resize.jpg /tmp/backdrop.resize.mpg');
						cmd = ostrcat(cmd, "mv /tmp/backdrop.resize.mpg ", 1, 0);
						cmd = ostrcat(cmd, path, 1, 0);
						cmd = ostrcat(cmd, "/", 1, 0);
						cmd = ostrcat(cmd, tmdb->id, 1, 0);
						cmd = ostrcat(cmd, "_backdrop.mvi", 1, 0);
						system(cmd);
						free(cmd), cmd = NULL;
					}
				}
				free(savefile), savefile = NULL;
				free(ip); ip = NULL;
			}
		}

		free(tmpstr); tmpstr = NULL;
		debug(133, "title: %s", tmdb->title);
		debug(133, "language: %s", tmdb->language);
		debug(133, "type: %s", tmdb->type);
		debug(133, "orgname: %s", tmdb->orgname);
		debug(133, "score: %s", tmdb->score);
		debug(133, "rating: %s", tmdb->rating);
		debug(133, "released: %s", tmdb->released);
		debug(133, "genre: %s", tmdb->genre);
		debug(133, "runtime: %s", tmdb->runtime);
		debug(133, "plot: %s", tmdb->plot);
		debug(133, "thumb: %s", tmdb->thumb);
		debug(133, "cover: %s", tmdb->cover);
		debug(133, "poster: %s", tmdb->poster);
		debug(133, "backdrop: %s", tmdb->backdrop);
		debug(133, "rating: %s", tmdb->rating);
		debug(133, "votes: %s", tmdb->votes);
		debug(133, "id: %s", tmdb->id);	
		debug(133, "imdbid: %s", tmdb->imdbid);
	}
	else
		tmdb->title = ostrcat("Nothing found.", NULL, 0, 0);

	return tmdb;
}

void screentmdb(char* title)
{
	int rcret = 0;
	struct skin* tmdbskin = getscreen("tmdb");
	struct skin* skin_plot = getscreennode(tmdbskin, "plot");
	struct skin* skin_title = getscreennode(tmdbskin, "title");
	struct skin* skin_orgname = getscreennode(tmdbskin, "orgname");
	struct skin* skin_rating = getscreennode(tmdbskin, "rating");
	struct skin* skin_genre = getscreennode(tmdbskin, "genre");
	struct skin* skin_released = getscreennode(tmdbskin, "released");
	struct skin* skin_cover = getscreennode(tmdbskin, "cover");
	struct skin* skin_votes = getscreennode(tmdbskin, "votes");
	struct tmdb* node = NULL;
	char* search = NULL;

	setfbtransparent(255);
	status.hangtime = 99999;

	if(title == NULL) title = getepgakttitle(NULL);

	node = gettmdb(title, 0);
start:
	if(node != NULL)
	{
		changetext(skin_plot, node->plot);
		changetext(skin_title, node->title);
		changetext(skin_orgname, node->orgname);
		changetext(skin_rating, node->rating);
		changetext(skin_genre, node->genre);
		changetext(skin_released, node->released);
		changetext(skin_votes, node->votes);
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
				node = gettmdb(search, 0);
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
