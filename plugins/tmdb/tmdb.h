#ifndef TMDB_H
#define TMDB_H

#define TMPTMDBPIC1 "/tmp/tmptmdb1.jpg"
#define TMPTMDBPIC2 "/tmp/tmptmdb2.jpg"
#define TMPTMDBPIC3 "/tmp/tmptmdb3.jpg"
#define TMPTMDBPIC4 "/tmp/tmptmdb4.jpg"
#define TMPTMDBPIC5 "/tmp/tmptmdb5.mvi"

extern struct tmdb* tmdb;

struct tmdb* addtmdb(int count, struct tmdb* last)
{
	//debug(1000, "in");
	struct tmdb *newnode = NULL, *prev = NULL, *node = tmdb;

	newnode = (struct tmdb*)calloc(1, sizeof(struct tmdb));
	if(newnode == NULL)
	{
		err("no memory");
		return NULL;
	}

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
		tmdb = newnode;
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

int deltmdb(struct tmdb* tnode, int flag)
{
	debug(1000, "in");
	int ret = 1;

	struct tmdb *node = tmdb, *prev = tmdb;

	while(node != NULL)
	{
		if(node == tnode)
		{
			ret = 0;
			if(node == tmdb)
			{
				tmdb = node->next;
				if(tmdb != NULL)
					tmdb->prev = NULL;
			}
			else
			{
				prev->next = node->next;
				if(node->next != NULL)
					node->next->prev = prev;
			}

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
			free(node->imdbid); node->imdbid = NULL;
			free(node->backdrop); node->backdrop = NULL;
			free(node->language); node->language = NULL;
			free(node->type); node->type = NULL;
			free(node->score); node->score = NULL;
			free(node->cover); node->cover = NULL;
			free(node->thumb); node->thumb = NULL;
			free(node->mvi); node->mvi = NULL;

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

void freetmdb(int flag)
{
	debug(1000, "in");
	struct tmdb *node = tmdb, *prev = tmdb;

	while(node != NULL)
	{
		prev = node;
		node = node->next;
		if(prev != NULL)
			deltmdb(prev, flag);
	}

	unlink(TMPTMDBPIC1);
	unlink(TMPTMDBPIC2);
	unlink(TMPTMDBPIC3);
	unlink(TMPTMDBPIC4);
	unlink(TMPTMDBPIC5);

	debug(1000, "out");
}

char* savetmdbpic(char* imdbid, char* url, char* tmppic, char* pic, int flag1)
{
debug(133, "00000000000in save\n");
debug(133, "url: %s", url);
	char* ip = NULL, *pos = NULL, *path = NULL, *ret = NULL;
	char* savefile = NULL;

	if(imdbid == NULL || url == NULL) return NULL;
	ip = string_replace("http://", "", url, 0);

	if(ip != NULL)
		pos = strchr(ip, '/');
	if(pos != NULL)
	{
		pos[0] = '\0';
		path = pos + 1;
	}

debug(133, "11111111in save");

	if(flag1 == 1)
	{
debug(133, "22222222in save");
	
		savefile = ostrcat(getconfig("mediadbpath", NULL), "/", 0, 0);
		savefile = ostrcat(savefile, imdbid, 1, 0);
		savefile = ostrcat(savefile, pic, 1, 0);

debug(133, "savefile: %s", savefile);

		if(!file_exist(savefile))
			gethttp(ip, path, 80, savefile, NULL, NULL, 0);
		ret = savefile;
	}
	else
	{
		gethttp(ip, path, 80, tmppic, NULL, NULL, 0);
		ret = ostrcat(tmppic, NULL, 0, 0);
	}

	free(ip); ip = NULL;
	return ret;
}

// flag: 0 = title search
// flag:1 = imdbid search
// flag1: 0 = save in tmp
// flag1: 1 = save in mediadb path
struct tmdb* gettmdb(char* title, int flag, int flag1)
{
	struct tmdb* tnode = NULL;
	char* tmpstr = NULL, *tmpstr1 = NULL;
	char* tmpsearch = NULL, *savefile = NULL;
	int count = 0;
	
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
		if(ostrstr(tmpstr, "<movies>Nothing found.</movies>") != NULL)
		{
			debug(133, "<movies>Nothing found.</movies>");
			free(tmpstr); tmpstr = NULL;
			return NULL;
		}

		tmpstr1 = ostrstr(tmpstr, "<movie>");

		while(tmpstr1 != NULL)
		{
			tnode = addtmdb(1, tnode);
			if(tnode == NULL)
			{
				err("no mem");
				free(tmpstr); tmpstr = NULL;
				return tmdb;
			}
			count++;

			if(ostrstr(tmpstr1, "<name>") != NULL)
				tnode->title = string_resub("<name>", "</name>", tmpstr1, 0);

			if(ostrstr(tmpstr1, "<language>") != NULL)
				tnode->language = string_resub("<language>", "</language>", tmpstr1, 0);

			if(ostrstr(tmpstr1, "<type>") != NULL)
				tnode->type = string_resub("<type>", "</type>", tmpstr1, 0);

			if(ostrstr(tmpstr1, "<original_name>") != NULL)
				tnode->orgname = string_resub("<original_name>", "</original_name>", tmpstr1, 0);

			if(ostrstr(tmpstr1, "<score>") != NULL)
				tnode->score = string_resub("<score>", "</score>", tmpstr1, 0);

			if(ostrstr(tmpstr1, "<rated>") != NULL)
				tnode->rated = string_resub("<rated>", "</rated>", tmpstr1, 0);

			if(ostrstr(tmpstr1, "<released>") != NULL)
				tnode->released = string_resub("<released>", "</released>", tmpstr1, 0);

			if(ostrstr(tmpstr1, "<categories>") != NULL)
			{
				char* tmpcat = string_resub("<categories>", "</categories>", tmpstr1, 0);
				char* ptmpcat = ostrstr(tmpcat, "<category ");
				while(ptmpcat != NULL)
				{
					tnode->genre = ostrcat(tnode->genre, string_resub("<category type=\"genre\" name=\"", "\" url=", ptmpcat, 0), 1, 1);
					ptmpcat += 5;
					ptmpcat = ostrstr(ptmpcat, "<category ");

					if(ptmpcat != NULL)
						tnode->genre = ostrcat(tnode->genre, ", ", 1, 0);
				}
				free(tmpcat); tmpcat = NULL;
			}

			if(ostrstr(tmpstr1, "<runtime>") != NULL)
				tnode->runtime = string_resub("<runtime>", "</runtime>", tmpstr1, 0);

			if(ostrstr(tmpstr1, "<overview>") != NULL)
				tnode->plot = string_resub("<overview>", "</overview>", tmpstr1, 0);

			if(ostrstr(tmpstr1, "size=\"thumb\"") != NULL)
				tnode->thumb = string_resub("<image type=\"poster\" url=\"", "\" size=\"thumb\"", tmpstr1, 1);

			if(ostrstr(tmpstr1, "size=\"cover\"") != NULL)
				tnode->cover = string_resub("<image type=\"poster\" url=\"", "\" size=\"cover\"", tmpstr1, 1);

			if(ostrstr(tmpstr1, "size=\"mid\"") != NULL)
				tnode->postermid = string_resub("<image type=\"poster\" url=\"", "\" size=\"mid\"", tmpstr1, 1);

			if(ostrstr(tmpstr1, "size=\"original\"") != NULL)
				tnode->backdrop = string_resub("<image type=\"backdrop\" url=\"", "\" size=\"original\"", tmpstr1, 0);

			if(ostrstr(tmpstr1, "<rating>") != NULL)
				tnode->rating = string_resub("<rating>", "</rating>", tmpstr1, 0);

			if(ostrstr(tmpstr1, "<votes>") != NULL)
				tnode->votes = string_resub("<votes>", "</votes>", tmpstr1, 0);

			if(ostrstr(tmpstr1, "<id>") != NULL)
				tnode->id = string_resub("<id>", "</id>", tmpstr1, 0);

			if(ostrstr(tmpstr1, "<imdb_id>") != NULL)
				tnode->imdbid = string_resub("<imdb_id>", "</imdb_id>", tmpstr1, 0);

debug(133, "vor save");
			if((flag1 == 0 && count == 1) || flag1 == 1)
			{
debug(133, "in save");
				savefile = savetmdbpic(tnode->imdbid, tnode->thumb, TMPTMDBPIC1, "_thumb.jpg", flag1);
				free(tnode->thumb);
				tnode->thumb = savefile;

				savefile = savetmdbpic(tnode->imdbid, tnode->cover, TMPTMDBPIC2, "_cover.jpg", flag1);
				free(tnode->cover);
				tnode->cover = savefile;

				savefile = savetmdbpic(tnode->imdbid, tnode->postermid, TMPTMDBPIC3, "_postermid.jpg", flag1);
				free(tnode->postermid);
				tnode->postermid = savefile;

				savefile = savetmdbpic(tnode->imdbid, tnode->backdrop, TMPTMDBPIC4, "_backdrop.jpg", flag1);
				free(tnode->backdrop);
				tnode->backdrop = savefile;
			}

			if(flag1 == 1 && tnode->backdrop != NULL && tnode->imdbid != NULL)
			{
				char* tmpmvi= NULL;
				tmpmvi = ostrcat(getconfig("mediadbpath", NULL), "/", 0, 0);
				tmpmvi = ostrcat(tmpmvi, tnode->imdbid, 1, 0);
				tmpmvi = ostrcat(tmpmvi, "_backdrop.mvi", 1, 0);

				if(!file_exist(tmpmvi))
				{
					char* cmd = NULL;
					cmd = ostrcat(cmd, "jpegtran -outfile /tmp/backdrop.resize.jpg -copy none ", 1, 0);
					cmd = ostrcat(cmd, tnode->backdrop, 1, 0);

					debug(133, "cmd %s", cmd);
					system(cmd);
					free(cmd); cmd = NULL;

					cmd = ostrcat(cmd, "ffmpeg -y -f image2 -i /tmp/backdrop.resize.jpg ", 1, 0);
					cmd = ostrcat(cmd, tmpmvi, 1, 0);
					debug(133, "cmd %s", cmd);
					system(cmd);
					free(cmd); cmd = NULL;
				}

				free(tnode->mvi);
				tnode->mvi = tmpmvi;
			}

			tmpstr1 += 5;
			tmpstr1 = ostrstr(tmpstr1, "<movie>");

			debug(133, "----------------------tmdb start----------------------");
			debug(133, "title: %s", tnode->title);
			debug(133, "language: %s", tnode->language);
			debug(133, "type: %s", tnode->type);
			debug(133, "orgname: %s", tnode->orgname);
			debug(133, "score: %s", tnode->score);
			debug(133, "rating: %s", tnode->rating);
			debug(133, "released: %s", tnode->released);
			debug(133, "genre: %s", tnode->genre);
			debug(133, "runtime: %s", tnode->runtime);
			debug(133, "plot: %s", tnode->plot);
			debug(133, "thumb: %s", tnode->thumb);
			debug(133, "cover: %s", tnode->cover);
			debug(133, "postermid: %s", tnode->postermid);
			debug(133, "backdrop: %s", tnode->backdrop);
			debug(133, "mvi: %s", tnode->mvi);
			debug(133, "rating: %s", tnode->rating);
			debug(133, "votes: %s", tnode->votes);
			debug(133, "id: %s", tnode->id);	
			debug(133, "imdbid: %s", tnode->imdbid);
			debug(133, "----------------------tmdb end----------------------");
		}
	}

	free(tmpstr); tmpstr = NULL;
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

	node = gettmdb(title, 0, 0);
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
				freetmdb(0); node = NULL;
				node = gettmdb(search, 0, 0);
				free(search); search = NULL;
				goto start;
			}
			drawscreen(tmdbskin, 0);
			continue;
		}
		if(rcret == getrcconfigint("rcgreen", NULL))
		{
			struct tmdb* tnode = tmdb;
			struct menulist* mlist = NULL, *mbox = NULL;

			while(tnode != NULL)
			{
				addmenulist(&mlist, tnode->title, tnode->imdbid, NULL, 0, 0);
				tnode = tnode->next;
			}

			mbox = menulistbox(mlist, NULL, NULL, NULL, NULL, 0, 0);
			if(mbox != NULL)
			{
				search = ostrcat(mbox->text, NULL, 0, 0);
				if(search != NULL)
				{
					freetmdb(0); node = NULL;
					node = gettmdb(search, 1, 0);
					free(search); search = NULL;
					freemenulist(mlist, 1); mlist = NULL, mbox = NULL;
					goto start;
				}
			}
			freemenulist(mlist, 1); mlist = NULL, mbox = NULL;
			drawscreen(tmdbskin, 0);
			continue;
		}
	}

	freetmdb(0); node = NULL;
	setosdtransparent(getskinconfigint("osdtransparent", NULL));
	status.hangtime = getconfigint("hangtime", NULL);
	clearscreen(tmdbskin);
}

#endif
