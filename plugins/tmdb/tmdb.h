#ifndef TMDB_H
#define TMDB_H

#define TMPTMDBPIC1 "/tmp/tmptmdb1.jpg"
#define TMPTMDBPIC2 "/tmp/tmptmdb2.jpg"
#define TMPTMDBPIC3 "/tmp/tmptmdb3.jpg"
#define TMPTMDBPIC4 "/tmp/tmptmdb4.jpg"
#define TMPTMDBPIC5 "/tmp/tmptmdb5.mvi"

extern struct tmdb* tmdb;

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
	free(node->postermid); node->postermid = NULL;
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
	free(node->mvi); node->mvi = NULL;

	unlink(TMPTMDBPIC1);
	unlink(TMPTMDBPIC2);
	unlink(TMPTMDBPIC3);
	unlink(TMPTMDBPIC4);
	unlink(TMPTMDBPIC5);

	free(node); node = NULL;
}

// flag 0 = title search
// flag 1 = imdbid search
// flag 2 = imdbid search and save

struct tmdb* gettmdb(char* title, int flag, int flag1, int flag2)
{
	struct tmdb* tmdb = NULL;
	char* tmpstr = NULL;
	char* tmpsearch = NULL;
	char* savefile = NULL;
	
start:
	tmpsearch = ostrcat("2.1/", NULL, 0, 0);
	if(flag == 0)
		tmpsearch = ostrcat(tmpsearch, "Movie.search", 1, 0);
	else
		tmpsearch = ostrcat(tmpsearch, "Movie.imdbLookup", 1, 0);
		
	tmpsearch = ostrcat(tmpsearch, "/", 1, 0);
	tmpsearch = ostrcat(tmpsearch, "de", 1, 0);
	tmpsearch = ostrcat(tmpsearch, "/xml/", 1, 0);
	tmpsearch = ostrcat(tmpsearch, "7bcd34bb47bc65d20a49b6b446a32866", 1, 0);
	tmpsearch = ostrcat(tmpsearch, "/", 1, 0);
	tmpsearch = ostrcat(tmpsearch, title, 1, 0);
	tmpsearch = stringreplacechar(tmpsearch, ' ', '+');

	tmpstr = gethttp("api.themoviedb.org", tmpsearch, 80, NULL, NULL, NULL, 0);
	
	debug(133, "tmpsearch: %s", tmpsearch);
//	debug(133, "tmpstr: %s", tmpstr);
				
	free(tmpsearch); tmpsearch = NULL;

	if(tmpstr != NULL)
	{
		tmdb = (struct tmdb*)calloc(1, sizeof(struct tmdb));
		if(tmdb == NULL)
		{
			err("no mem");
			free(tmpstr); tmpstr = NULL;
			return NULL;
		}

		if(ostrstr(tmpstr, "<movies>Nothing found.</movies>") != NULL)
		{
			free(tmpstr), tmpstr = NULL;
		}
		else if(ostrstr(tmpstr, "<opensearch:totalResults>1</opensearch:totalResults>") == NULL && flag1 == 0)
		{
			char* tmpstr1 = NULL;			
			char* tmpstr2 = NULL;
			char* tmpstr3 = NULL;

			tmpstr1 = ostrcat(tmpstr, NULL, 0, 0);
	
			struct menulist* mlist = NULL, *mbox = NULL;
		
			int count = 0;
			while(ostrstr(tmpstr1, "<imdb_id>") != NULL)
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
	
	if(tmpstr != NULL && ostrstr(tmpstr, "<opensearch:totalResults>1</opensearch:totalResults>") != NULL)
	{
		if(ostrstr(tmpstr, "<name>") != NULL)
		{
			tmdb->title = string_resub("<name>", "</name>", tmpstr);
		}

		if(ostrstr(tmpstr, "<language>") != NULL)
		{
			tmdb->language = string_resub("<language>", "</language>", tmpstr);
		}

		if(ostrstr(tmpstr, "<type>") != NULL)
		{
			tmdb->type = string_resub("<type>", "</type>", tmpstr);
		}

		if(ostrstr(tmpstr, "<original_name>") != NULL)
		{
			tmdb->orgname = string_resub("<original_name>", "</original_name>", tmpstr);
		}

		if(ostrstr(tmpstr, "<score>") != NULL)
		{
			tmdb->score = string_resub("<score>", "</score>", tmpstr);
		}

		if(ostrstr(tmpstr, "<rating>") != NULL)
		{
			tmdb->rated = string_resub("<rating>", "</rating>", tmpstr);
		}

		if(ostrstr(tmpstr, "<released>") != NULL)
		{
			tmdb->released = string_resub("<released>", "</released>", tmpstr);
		}

		if(ostrstr(tmpstr, "<categories>") != NULL)
		{
			tmdb->genre = string_resub("<category type=\"genre\" name=\"", "\" url=", tmpstr);
			while(ostrstr(tmdb->genre, "<category type=") != NULL)
			{
				tmdb->genre = string_resub("<category type=\"genre\" name=\"", "\" url=", tmdb->genre);
			}
		}

		if(ostrstr(tmpstr, "<runtime>") != NULL)
		{
			tmdb->runtime = string_resub("<runtime>", "</runtime>", tmpstr);
		}

		if(ostrstr(tmpstr, "<overview>") != NULL)
		{
			tmdb->plot = string_resub("<overview>", "</overview>", tmpstr);
		}

		if(ostrstr(tmpstr, "type=\"poster\"") != NULL)
		{
			tmdb->thumb = string_resub("<image type=\"poster\" url=\"", "\" size=\"thumb\"", tmpstr);
			while(ostrstr(tmdb->thumb, "<image type=") != NULL)
			{
				tmdb->thumb = string_resub("<image type=\"poster\" url=\"", "\" size=\"thumb\"", tmdb->thumb);
			}
		}

		if(ostrstr(tmpstr, "type=\"poster\"") != NULL)
		{
			tmdb->cover = string_resub("<image type=\"poster\" url=\"", "\" size=\"cover\"", tmpstr);
			while(ostrstr(tmdb->cover, "<image type=") != NULL)
			{
				tmdb->cover = string_resub("<image type=\"poster\" url=\"", "\" size=\"cover\"", tmdb->cover);
			}
		}

		if(ostrstr(tmpstr, "type=\"poster\"") != NULL)
		{
			tmdb->postermid = string_resub("<image type=\"poster\" url=\"", "\" size=\"mid\"", tmpstr);
			while(ostrstr(tmdb->postermid, "<image type=") != NULL)
			{
				tmdb->postermid = string_resub("<image type=\"poster\" url=\"", "\" size=\"mid\"", tmdb->postermid);
			}
		}

		if(ostrstr(tmpstr, "type=\"backdrop\"") != NULL)
		{
			tmdb->backdrop = string_resub("<image type=\"backdrop\" url=\"", "\" size=\"original\"", tmpstr);
			while(ostrstr(tmdb->backdrop, "<image type=") != NULL)
			{
				tmdb->backdrop = string_resub("<image type=\"backdrop\" url=\"", "\" size=\"original\"", tmdb->backdrop);
			}
		}

		if(ostrstr(tmpstr, "<rating>") != NULL)
		{
			tmdb->rating = string_resub("<rating>", "</rating>", tmpstr);
		}

		if(ostrstr(tmpstr, "<votes>") != NULL)
		{
			tmdb->votes = string_resub("<votes>", "</votes>", tmpstr);
		}

		if(ostrstr(tmpstr, "<id>") != NULL)
		{
			tmdb->id = string_resub("<id>", "</id>", tmpstr);
		}

		if(ostrstr(tmpstr, "<imdb_id>") != NULL)
		{
			tmdb->imdbid = string_resub("<imdb_id>", "</imdb_id>", tmpstr);
		}

		if(flag2 == 0 && tmdb->imdbid != NULL)
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
	
				if(flag1 == 1)
				{
					savefile = ostrcat(getconfig("mediadbpath", NULL), "/", 0, 0);
					savefile = ostrcat(savefile, tmdb->imdbid, 1, 0);
					savefile = ostrcat(savefile, "_thumb.jpg", 1, 0);
					gethttp(ip, path, 80, savefile, NULL, NULL, 0);
					free(tmdb->thumb);
					tmdb->thumb = savefile;
				}
				else
				{
					gethttp(ip, path, 80, TMPTMDBPIC1, NULL, NULL, 0);
					free(tmdb->thumb);
					tmdb->thumb = ostrcat(TMPTMDBPIC1, NULL, 0, 0);
				}
	
				free(ip); ip = NULL;
			}
			if(tmdb->cover != NULL)
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
	
				if(flag1 == 1)
				{
					savefile = ostrcat(getconfig("mediadbpath", NULL), "/", 0, 0);
					savefile = ostrcat(savefile, tmdb->imdbid, 1, 0);
					savefile = ostrcat(savefile, "_cover.jpg", 1, 0);
					gethttp(ip, path, 80, savefile, NULL, NULL, 0);
					free(tmdb->cover);
					tmdb->cover = savefile;
				}
				else
				{
					gethttp(ip, path, 80, TMPTMDBPIC2, NULL, NULL, 0);
					free(tmdb->cover);
					tmdb->cover = ostrcat(TMPTMDBPIC2, NULL, 0, 0);
				}
	
				free(ip); ip = NULL;
			}
			if(tmdb->postermid != NULL)
			{ 		
				char* ip = NULL, *pos = NULL, *path = NULL;
				ip = string_replace("http://", "", tmdb->postermid, 0);
	
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
					savefile = ostrcat(savefile, tmdb->imdbid, 1, 0);
					savefile = ostrcat(savefile, "_postermid.jpg", 1, 0);
					gethttp(ip, path, 80, savefile, NULL, NULL, 0);
					free(tmdb->postermid);
					tmdb->postermid = savefile;
				}
				else
				{
					gethttp(ip, path, 80, TMPTMDBPIC3, NULL, NULL, 0);
					free(tmdb->postermid);
					tmdb->postermid = ostrcat(TMPTMDBPIC3, NULL, 0, 0);
				}
	
				free(ip); ip = NULL;
			}
			if(tmdb->backdrop != NULL)
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
	
				if(flag1 == 1)
				{
					savefile = ostrcat(getconfig("mediadbpath", NULL), "/", 0, 0);
					savefile = ostrcat(savefile, tmdb->imdbid, 1, 0);
					savefile = ostrcat(savefile, "_backdrop.jpg", 1, 0);
					gethttp(ip, path, 80, savefile, NULL, NULL, 0);

					free(tmpstr), tmpstr = NULL;
					tmpstr = ostrcat(tmpstr, savefile, 1, 0);
					
					free(tmdb->backdrop);
					tmdb->backdrop = savefile;
					char* cmd = NULL;
					cmd = ostrcat(cmd, "jpegtran -outfile /tmp/backdrop.resize.jpg -copy none ", 1, 0);
					cmd = ostrcat(cmd, tmpstr, 1, 0);

					debug(133, "cmd %s", cmd);
					system(cmd);
					free(cmd), cmd = NULL;

					cmd = ostrcat(cmd, "ffmpeg -y -f image2 -i /tmp/backdrop.resize.jpg /tmp/backdrop.resize.mpg", 1, 0);
					debug(133, "cmd %s", cmd);
					system(cmd);
					free(cmd), cmd = NULL;

					char* tmpmvi= NULL;
					tmpmvi = ostrcat(getconfig("mediadbpath", NULL), "/", 0, 0);
					tmpmvi = ostrcat(tmpmvi, tmdb->imdbid, 1, 0);
					tmpmvi = ostrcat(tmpmvi, "_backdrop.mvi", 1, 0);

					cmd = ostrcat(cmd, "mv /tmp/backdrop.resize.mpg ", 1, 0);
					cmd = ostrcat(cmd, tmpmvi, 1, 0);
					debug(133, "cmd %s", cmd);
					system(cmd);
					free(cmd), cmd = NULL;

//					free(tmdb->mvi);
					tmdb->mvi = tmpmvi;

				}
				else
				{
					gethttp(ip, path, 80, TMPTMDBPIC4, NULL, NULL, 0);
					free(tmdb->backdrop);
					tmdb->backdrop = ostrcat(TMPTMDBPIC4, NULL, 0, 0);
					tmdb->mvi = ostrcat(TMPTMDBPIC4, NULL, 0, 0);
				}
				free(ip); ip = NULL;
			}
		}
		else
		{
			if(tmdb->thumb == NULL)
			{
				free(tmdb->thumb);
				tmdb->thumb = NULL;
			}
			if(tmdb->cover == NULL)
			{
				free(tmdb->cover);
				tmdb->cover = NULL;
			}
			if(tmdb->postermid == NULL)
			{
				free(tmdb->postermid);
				tmdb->postermid = NULL;
			}
			if(tmdb->backdrop == NULL)
			{
				free(tmdb->backdrop);
				tmdb->backdrop = NULL;
			}
			if(tmdb->mvi == NULL)
			{
				free(tmdb->mvi);
				tmdb->mvi = NULL;
			}
		}

		if(tmpstr != NULL)
		{
			free(tmpstr); tmpstr = NULL;
		}

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
		debug(133, "postermid: %s", tmdb->postermid);
		debug(133, "backdrop: %s", tmdb->backdrop);
		debug(133, "mvi: %s", tmdb->mvi);
		debug(133, "rating: %s", tmdb->rating);
		debug(133, "votes: %s", tmdb->votes);
		debug(133, "id: %s", tmdb->id);	
		debug(133, "imdbid: %s", tmdb->imdbid);
	}

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

	node = gettmdb(title, 0, 0, 0);
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
		changepic(skin_cover, TMPTMDBPIC3);
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
				node = gettmdb(search, 0, 0, 0);
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
