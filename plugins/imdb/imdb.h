#ifndef IMDB_H
#define IMDB_H

#define TMPIMDBPIC1 "/tmp/tmpimdb1.jpg"
#define TMPIMDBPIC2 "/tmp/tmpimdb2.jpg"

extern struct imdb* imdb;

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

	unlink(TMPIMDBPIC1);
	unlink(TMPIMDBPIC2);

	free(node); node = NULL;
}

// flag 0 = title search
// flag 1 = iimdbid search
// flag 2 = iimdbid search and save

struct imdb* getimdb(char* title, int flag, int flag1, int flag2)
{
	struct imdb* imdb = NULL;
	char* tmpstr = NULL;
	char* tmpsearch = NULL;
	char* apikey = NULL;
	char* searchurl = NULL;
	char* lang = NULL;
	char* url = NULL;
	char* savefile = NULL;
	char* lookup = NULL;
	char* pageposter = NULL;

	url = ostrcat(url, "www.imdb.de", 1, 0);
	apikey = ostrcat(apikey, "7bcd34bb47bc65d20a49b6b446a32866", 1, 0);
	searchurl = ostrcat(searchurl, "find?s=tt;q=", 1, 0);
	lookup = ostrcat("/title/tt", NULL, 0, 0);
	lang = ostrcat(lang, "de", 1, 0);
	
start:
	tmpsearch = ostrcat(searchurl, NULL, 0, 0);
	if(flag == 0)
		tmpsearch = ostrcat(tmpsearch, title, 1, 0);
	else
		tmpsearch = ostrcat(tmpsearch, lookup, 1, 0);
	tmpsearch = stringreplacechar(tmpsearch, ' ', '+');

	tmpstr = gethttp(url, tmpsearch, 80, NULL, NULL, NULL, 0);
	
	debug(133, "tmpsearch: %s", tmpsearch);
//	debug(133, "tmpstr: %s", tmpstr);
				
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
		imdb->id = string_resub("<a href=\"/title/tt","/",tmpstr);

// todo - use Meistgesuchte Titel (default)
// Meistgesuchte Titel
// Titel (näherungsweise Übereinstimmung)
// Titel (genaue Übereinstimmung)

		if(flag1 == 0 && string_find("<title>IMDb Titelsuche</title>", tmpstr) && string_find("<p><b>Meistgesuchte Titel</b>", tmpstr) == 0)
		{
			tmpstr = string_resub("<p><b>Titel","</td></tr></table> </p>",tmpstr);
			
			while(string_find("</td><td valign=\"top\"><a href=\"/title/", tmpstr))
			{
				tmpstr = string_replace("</td><td valign=\"top\"><a href=\"/title/", "\n\n", tmpstr, 0);
			}

			struct menulist* mlist = NULL, *mbox = NULL;
			char* tmpstr1 = NULL;
			
			int count = 0;
			struct splitstr* ret = NULL;
			ret = strsplit(tmpstr, "\n", &count);

			int max = count;
			int i = 0;
			for( i = 0; i < max; i++)
			{
				tmpstr1 = string_resub("link=/title", "</a>", (&ret[i])->part);
				struct regex* x = regexstruct("/tt([0-9]{7})", tmpstr1);
				struct regex* y = regexstruct(";\">(.*)", tmpstr1);
				if(x != NULL && y != NULL)
				{
					x->match2 = string_decode(x->match2,1);
					y->match2 = string_decode(y->match2,1);
					addmenulist(&mlist, y->match2, x->match2, NULL, 0, 0);
					debug(133, "x->match2(%d): %s\n", i, x->match2);
					debug(133, "y->match2(%d): %s\n", i, y->match2);
				}
				freeregexstruct(x); x= NULL;
				freeregexstruct(y); y= NULL;

				free(tmpstr1),tmpstr1 = NULL;				
			}
			free(ret); ret = NULL;
				
			mbox = menulistbox(mlist, NULL, NULL, NULL, NULL, 1, 0);
			if(mbox != NULL)
			{
				free(title), title = NULL;
				free(tmpstr), tmpstr = NULL;

				debug(133, "mbox->name %s", mbox->name);
				debug(133, "mbox->text %s", mbox->text);
						
				title = ostrcat(mbox->text, NULL, 0, 0);
				flag = 1;
				goto start;
			}
		}
		free(tmpstr), tmpstr = NULL;		
	}

	if(imdb != NULL)
	{
		tmpsearch = ostrcat(lookup, NULL, 0, 0);
		tmpsearch = ostrcat(tmpsearch, imdb->id, 1, 0);

		tmpstr = gethttp(url, tmpsearch, 80, NULL, NULL, NULL, 0);
		
		debug(133, "tmpsearch: %s", tmpsearch);
//		debug(133, "tmpstr: %s", tmpstr);
					
		free(tmpsearch); tmpsearch = NULL;
	}

	if(tmpstr != NULL)
	{
		if(string_find("<title>", tmpstr))
		{
			imdb->title = string_resub("<title>","</title>",tmpstr);
			imdb->title = string_decode(imdb->title,1);
			string_strip_whitechars(imdb->title);
			strstrip(imdb->title);
		}

		if(string_find("<h5>Genre:</h5>", tmpstr))
		{
			imdb->genre = string_resub("<h5>Genre:</h5>","</div>",tmpstr);
			imdb->genre = string_decode(imdb->genre,1);
			string_striptags(imdb->genre);
			string_strip_whitechars(imdb->genre);
			strstrip(imdb->genre);
		}

		if(string_find("<h5>Drehbuchautor", tmpstr))
		{
			imdb->writer = string_resub("<h5>Drehbuchautor","</div>",tmpstr);
			imdb->writer = string_resub("<div class=\"info-content\">","</div>",imdb->writer);
			imdb->writer = string_resub(";\">","</a>",imdb->writer);
			imdb->writer = string_decode(imdb->writer,1);
			string_striptags(imdb->writer);
			string_strip_whitechars(imdb->writer);
			strstrip(imdb->writer);
		}

		if(string_find("Regisseur:", tmpstr))
		{
			imdb->director = string_resub("Regisseur:","</div>",tmpstr);
			imdb->director = string_resub(";\">","</a><br/>",imdb->director);			
			imdb->director = string_decode(imdb->director,1);
			string_striptags(imdb->director);
			string_strip_whitechars(imdb->director);
			strstrip(imdb->director);
		}
		
		if(string_find("<h5>Premierendatum:</h5>", tmpstr))
		{
			imdb->released = string_resub("<h5>Premierendatum:</h5>","<a class=",tmpstr);
			imdb->released = string_resub("<div class=\"info-content\">","<a class=",imdb->released);			
			imdb->released = string_decode(imdb->released,1);
			string_striptags(imdb->released);
			string_strip_whitechars(imdb->released);
			strstrip(imdb->released);
		}

		if(string_find("<h3>Besetzung</h3>", tmpstr))
		{
			imdb->actors = string_resub("<h3>Besetzung</h3>&nbsp;","</td></tr></table>",tmpstr);
			imdb->actors = string_resub("<div class=\"info-content block\"><table class=\"cast\">","</a></td></tr>",imdb->actors);			
			imdb->actors = string_replace("...", "als", imdb->actors, 0);
			imdb->actors = string_decode(imdb->actors,1);
			string_striptags(imdb->actors);
			string_strip_whitechars(imdb->actors);
			strstrip(imdb->actors);
		}

		if(string_find("<a name=\"poster\" href=\"", tmpstr))
		{
			imdb->thumb = string_resub("<a name=\"poster\" href=\"", "\" /></a>",tmpstr);
			imdb->thumb = string_resub("src=\"","\" /></a>",imdb->thumb);			
		}

		if(string_find("/media/rm", tmpstr))
		{
			pageposter = string_resub("/media/rm","/",tmpstr);
			strstrip(pageposter);
		}
		free(tmpstr), tmpstr = NULL;
	}	

	if(pageposter != NULL)
	{
		tmpsearch = ostrcat("/media/rm", pageposter, 0, 0);
		tmpsearch = ostrcat(tmpsearch, "/tt", 1, 0);
		tmpsearch = ostrcat(tmpsearch, imdb->id, 1, 0);
		tmpsearch = ostrcat(tmpsearch, "/", 1, 0);
	
		tmpstr = gethttp(url, tmpsearch, 80, NULL, NULL, NULL, 0);
		
		debug(133, "tmpsearch: %s", tmpsearch);
//		debug(133, "tmpstr: %s", tmpstr);
					
		free(tmpsearch); tmpsearch = NULL;
		if(tmpstr != NULL)
		{
			if(string_find("image_src", tmpstr))
			{
				imdb->poster = string_resub("<link rel=\"image_src\" href=\"","\"",tmpstr);
				strstrip(imdb->poster);
			}
			free(tmpstr), tmpstr = NULL;
		}
	}

	if(imdb != NULL)
	{
		tmpsearch = ostrcat(lookup, NULL, 0, 0);
		tmpsearch = ostrcat(tmpsearch, imdb->id, 1, 0);
		tmpsearch = ostrcat(tmpsearch, "/plotsummary", 1, 0);

		tmpstr = gethttp(url, tmpsearch, 80, NULL, NULL, NULL, 0);
		
		debug(133, "tmpsearch: %s", tmpsearch);
//		debug(133, "tmpstr: %s", tmpstr);
					
		free(tmpsearch); tmpsearch = NULL;
	}

	if(tmpstr != NULL)
	{
		if(string_find("swiki.2.1", tmpstr))
		{
			imdb->plot = string_resub("<div id=\"swiki.2.1\">","</div>",tmpstr);
			imdb->plot = string_decode(imdb->plot,1);
			string_striptags(imdb->plot);
			string_strip_whitechars(imdb->plot);
			strstrip(imdb->plot);
		}
		free(tmpstr), tmpstr = NULL;
	}

	if(flag2 == 0)
	{
		if(imdb->poster != NULL)
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
				gethttp(ip, path, 80, TMPIMDBPIC1, NULL, NULL, 0);
				free(imdb->poster);
				imdb->poster = ostrcat(TMPIMDBPIC1, NULL, 0, 0);
			}
	
			free(ip); ip = NULL;
		}
		if(imdb->thumb != NULL)
		{
			char* ip = NULL, *pos = NULL, *path = NULL;
			ip = string_replace("http://", "", imdb->thumb, 0);
	
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
				savefile = ostrcat(savefile, "_thumb.jpg", 1, 0);
				gethttp(ip, path, 80, savefile, NULL, NULL, 0);
				free(imdb->thumb);
				imdb->thumb = savefile;
			}
			else
			{
				gethttp(ip, path, 80, TMPIMDBPIC2, NULL, NULL, 0);
				free(imdb->thumb);
				imdb->thumb = ostrcat(TMPIMDBPIC2, NULL, 0, 0);
			}
	
			free(ip); ip = NULL;
		}
	}
	else
	{
		if(imdb->thumb == NULL)
		{
			free(imdb->thumb);
			imdb->thumb = NULL;
		}
		if(imdb->poster == NULL)
		{
			free(imdb->poster);
			imdb->poster = NULL;
		}
	}

	free(tmpstr); tmpstr = NULL;

	tmpstr = ostrcat("tt", imdb->id, 0, 0);
	imdb->id = tmpstr;
				
	debug(133, "id: %s", imdb->id);
	debug(133, "title: %s", imdb->title);
	debug(133, "genre: %s", imdb->genre);
	debug(133, "writer: %s", imdb->writer);
	debug(133, "director: %s", imdb->director);
	debug(133, "released: %s", imdb->released);
	debug(133, "actors: %s", imdb->actors);
	debug(133, "plot: %s", imdb->plot);
	debug(133, "poster: %s", imdb->poster);
	debug(133, "thumb: %s", imdb->thumb);
			
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
