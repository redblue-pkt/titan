#ifndef IMDB_H
#define IMDB_H

#define TMPIMDBPIC1 "/tmp/tmpimdb1.jpg"
#define TMPIMDBPIC2 "/tmp/tmpimdb2.jpg"

void freeimdb(struct imdb** node, int flag)
{
	if(node == NULL || *node == NULL) return;

	free((*node)->title); (*node)->title = NULL;
	free((*node)->year); (*node)->year = NULL;
	free((*node)->rated); (*node)->rated = NULL;
	free((*node)->released); (*node)->released = NULL;
	free((*node)->genre); (*node)->genre = NULL;
	free((*node)->director); (*node)->director = NULL;
	free((*node)->writer); (*node)->writer = NULL;
	free((*node)->actors); (*node)->actors = NULL;
	free((*node)->plot); (*node)->plot = NULL;
	free((*node)->poster); (*node)->poster = NULL;
	free((*node)->runtime); (*node)->runtime = NULL;
	free((*node)->rating); (*node)->rating = NULL;
	free((*node)->votes); (*node)->votes = NULL;
	free((*node)->id); (*node)->id = NULL;

	unlink(TMPIMDBPIC1);
	unlink(TMPIMDBPIC2);

	free(*node); *node = NULL;
}

// flag 0 = title search
// flag 1 = iimdbid search
// flag 2 = iimdbid search and save
struct imdb* getimdb(struct imdb** first, char* title, int flag, int flag1, int flag2)
{
	char* tmpstr = NULL;
	char* tmpsearch = NULL;
	char* savefile = NULL;
	char* pageposter = NULL;
	
start:
	tmpsearch = ostrcat("find?s=tt;q=", NULL, 0, 0);
	if(flag == 0)
		tmpsearch = ostrcat(tmpsearch, title, 1, 0);
	else
		tmpsearch = ostrcat(tmpsearch, "/title/tt", 1, 0);
	tmpsearch = stringreplacechar(tmpsearch, ' ', '+');

	tmpstr = gethttp("www.imdb.de", tmpsearch, 80, NULL, NULL, NULL, 0);
	
	debug(133, "tmpsearch: %s", tmpsearch);
//	debug(133, "tmpstr: %s", tmpstr);
				
	free(tmpsearch); tmpsearch = NULL;

	if(tmpstr != NULL)
	{

		if(ostrstr(tmpstr, "<b>Keine Treffer.</b>") != NULL)
		{
			debug(133, "<b>Keine Treffer.</b>");
			free(tmpstr); tmpstr = NULL;
			return NULL;
		}

		*first = (struct imdb*)calloc(1, sizeof(struct imdb));
		if(*first == NULL)
		{
			err("no mem");
			free(tmpstr); tmpstr = NULL;
			return NULL;
		}

		(*first)->id = string_resub("<a href=\"/title/tt", "/", tmpstr, 0);

		if(flag1 == 1)
		{
			if((*first)->id != NULL)
				(*first)->id = ostrcat("tt", (*first)->id, 0, 1);

			debug(133, "----------------------imdb start----------------------");
			debug(133, "id: %s", (*first)->id);
			debug(133, "----------------------imdb end----------------------");
	
			free(tmpstr); tmpstr = NULL;	
			return *first;
		}
		
// todo - use Meistgesuchte Titel (default)
// Meistgesuchte Titel
// Titel (näherungsweise Übereinstimmung)
// Titel (genaue Übereinstimmung)

		if(flag1 == 0 && ostrstr(tmpstr, "<title>IMDb Titelsuche</title>") != NULL && ostrstr(tmpstr, "<p><b>Meistgesuchte Titel</b>") == NULL)
		{
			tmpstr = string_resub("<p><b>Titel", "</td></tr></table> </p>", tmpstr, 0);
			
			while(ostrstr(tmpstr, "</td><td valign=\"top\"><a href=\"/title/") != NULL)
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
				tmpstr1 = string_resub("link=/title", "</a>", (&ret[i])->part, 0);
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

	if(*first != NULL)
	{
		if((*first)->id == NULL)
			free(tmpstr),tmpstr = NULL;
		else
		{
			debug(133, "imdb->id: %s", (*first)->id);
			
			tmpsearch = ostrcat("/title/tt", NULL, 0, 0);
			tmpsearch = ostrcat(tmpsearch, (*first)->id, 1, 0);
	
			tmpstr = gethttp("www.imdb.de", tmpsearch, 80, NULL, NULL, NULL, 0);
			
			debug(133, "tmpsearch: %s", tmpsearch);
	//		debug(133, "tmpstr: %s", tmpstr);
						
			free(tmpsearch); tmpsearch = NULL;
		}
	}

	if(tmpstr != NULL)
	{
		if(ostrstr(tmpstr, "<title>") != NULL)
		{
			(*first)->title = string_resub("<title>", "</title>", tmpstr, 0);
			(*first)->title = string_decode((*first)->title,1);
			string_strip_whitechars((*first)->title);
			strstrip((*first)->title);
		}

		if(ostrstr(tmpstr, "<h5>Genre:</h5>") != NULL)
		{
			(*first)->genre = string_resub("<h5>Genre:</h5>", "</div>", tmpstr, 0);
			(*first)->genre = string_decode((*first)->genre,1);
			string_striptags((*first)->genre);
			string_strip_whitechars((*first)->genre);
			strstrip((*first)->genre);
		}

		if(ostrstr(tmpstr, "<h5>Drehbuchautor") != NULL)
		{
			(*first)->writer = string_resub("<h5>Drehbuchautor", "</div>", tmpstr, 0);
			(*first)->writer = string_resub("<div class=\"info-content\">", "</div>", (*first)->writer, 0);
			(*first)->writer = string_resub(";\">", "</a>", (*first)->writer, 0);
			(*first)->writer = string_decode((*first)->writer,1);
			string_striptags((*first)->writer);
			string_strip_whitechars((*first)->writer);
			strstrip((*first)->writer);
		}

		if(ostrstr(tmpstr, "Regisseur:") != NULL)
		{
			(*first)->director = string_resub("Regisseur:", "</div>", tmpstr, 0);
			(*first)->director = string_resub(";\">", "</a><br/>", (*first)->director, 0);
			(*first)->director = string_decode((*first)->director,1);
			string_striptags((*first)->director);
			string_strip_whitechars((*first)->director);
			strstrip((*first)->director);
		}
		
		if(ostrstr(tmpstr, "<h5>Premierendatum:</h5>") != NULL)
		{
			(*first)->released = string_resub("<h5>Premierendatum:</h5>", "<a class=", tmpstr, 0);
			(*first)->released = string_resub("<div class=\"info-content\">", "<a class=", (*first)->released, 0);
			(*first)->released = string_decode((*first)->released,1);
			string_striptags((*first)->released);
			string_strip_whitechars((*first)->released);
			strstrip((*first)->released);
		}

		if(ostrstr(tmpstr, "<h3>Besetzung</h3>") != NULL)
		{
			(*first)->actors = string_resub("<h3>Besetzung</h3>&nbsp;", "</td></tr></table>", tmpstr, 0);
			(*first)->actors = string_resub("<div class=\"info-content block\"><table class=\"cast\">", "</a></td></tr>", (*first)->actors, 0);
			(*first)->actors = string_replace("...", "als", (*first)->actors, 0);
			(*first)->actors = string_decode((*first)->actors,1);
			string_striptags((*first)->actors);
			string_strip_whitechars((*first)->actors);
			strstrip((*first)->actors);
		}

		if(ostrstr(tmpstr, "<a name=\"poster\" href=\"") != NULL)
		{
			(*first)->thumb = string_resub("<a name=\"poster\" href=\"", "\" /></a>", tmpstr, 0);
			(*first)->thumb = string_resub("src=\"", "\" /></a>", (*first)->thumb, 0);
		}

		if(ostrstr(tmpstr, "/media/rm") != NULL)
		{
			pageposter = string_resub("/media/rm", "/", tmpstr, 0);
			strstrip(pageposter);
		}
		free(tmpstr), tmpstr = NULL;
	}	

	if(pageposter != NULL)
	{
		tmpsearch = ostrcat("/media/rm", pageposter, 0, 0);
		tmpsearch = ostrcat(tmpsearch, "/tt", 1, 0);
		tmpsearch = ostrcat(tmpsearch, (*first)->id, 1, 0);
		tmpsearch = ostrcat(tmpsearch, "/", 1, 0);
	
		tmpstr = gethttp("www.imdb.de", tmpsearch, 80, NULL, NULL, NULL, 0);
		
		debug(133, "tmpsearch: %s", tmpsearch);
//		debug(133, "tmpstr: %s", tmpstr);
					
		free(tmpsearch); tmpsearch = NULL;
		if(tmpstr != NULL)
		{
			if(ostrstr(tmpstr, "image_src") != NULL)
			{
				(*first)->poster = string_resub("<link rel=\"image_src\" href=\"", "\"", tmpstr, 0);
				strstrip((*first)->poster);
			}
			free(tmpstr), tmpstr = NULL;
		}
	}

	if((*first) != NULL && (*first)->id == NULL)
	{
		tmpsearch = ostrcat("/title/tt", NULL, 0, 0);
		tmpsearch = ostrcat(tmpsearch, (*first)->id, 1, 0);
		tmpsearch = ostrcat(tmpsearch, "/plotsummary", 1, 0);

		tmpstr = gethttp("www.imdb.de", tmpsearch, 80, NULL, NULL, NULL, 0);
		
		debug(133, "tmpsearch: %s", tmpsearch);
//		debug(133, "tmpstr: %s", tmpstr);
					
		free(tmpsearch); tmpsearch = NULL;
	}

	if(tmpstr != NULL)
	{
		if(ostrstr(tmpstr, "swiki.2.1") != NULL)
		{
			(*first)->plot = string_resub("<div id=\"swiki.2.1\">", "</div>", tmpstr, 0);
			(*first)->plot = string_decode((*first)->plot,1);
			string_striptags((*first)->plot);
			string_strip_whitechars((*first)->plot);
			strstrip((*first)->plot);
		}
		free(tmpstr), tmpstr = NULL;
	}

	if(flag2 == 0 && (*first)->id != NULL)
	{
		if((*first)->poster != NULL)
		{
			char* ip = NULL, *pos = NULL, *path = NULL;
			ip = string_replace("http://", "", (*first)->poster, 0);
	
			if(ip != NULL)
				pos = strchr(ip, '/');
			if(pos != NULL)
			{
				pos[0] = '\0';
				path = pos + 1;
			}
	
			if(flag1 == 1)
			{
				savefile = ostrcat(getconfig("mediadbpath", NULL), "/tt", 0, 0);
				savefile = ostrcat(savefile, (*first)->id, 1, 0);
				savefile = ostrcat(savefile, "_poster.jpg", 1, 0);
				gethttp(ip, path, 80, savefile, NULL, NULL, 0);
				free((*first)->poster);
				(*first)->poster = savefile;
			}
			else
			{
				gethttp(ip, path, 80, TMPIMDBPIC1, NULL, NULL, 0);
				free((*first)->poster);
				(*first)->poster = ostrcat(TMPIMDBPIC1, NULL, 0, 0);
			}
	
			free(ip); ip = NULL;
		}
		if((*first)->thumb != NULL)
		{
			char* ip = NULL, *pos = NULL, *path = NULL;
			ip = string_replace("http://", "", (*first)->thumb, 0);
	
			if(ip != NULL)
				pos = strchr(ip, '/');
			if(pos != NULL)
			{
				pos[0] = '\0';
				path = pos + 1;
			}
	
			if(flag1 == 1)
			{
				savefile = ostrcat(getconfig("mediadbpath", NULL), "/tt", 0, 0);
				savefile = ostrcat(savefile, (*first)->id, 1, 0);
				savefile = ostrcat(savefile, "_thumb.jpg", 1, 0);
				gethttp(ip, path, 80, savefile, NULL, NULL, 0);
				free((*first)->thumb);
				(*first)->thumb = savefile;
			}
			else
			{
				gethttp(ip, path, 80, TMPIMDBPIC2, NULL, NULL, 0);
				free((*first)->thumb);
				(*first)->thumb = ostrcat(TMPIMDBPIC2, NULL, 0, 0);
			}
	
			free(ip); ip = NULL;
		}
	}
	else
	{
		if((*first)->thumb == NULL)
		{
			free((*first)->thumb);
			(*first)->thumb = NULL;
		}
		if((*first)->poster == NULL)
		{
			free((*first)->poster);
			(*first)->poster = NULL;
		}
	}

	if((*first)->id != NULL)
		(*first)->id = ostrcat("tt", (*first)->id, 0, 1);

	free(tmpstr); tmpstr = NULL;

	debug(133, "----------------------imdb start----------------------");
	debug(133, "id: %s", (*first)->id);
	debug(133, "title: %s", (*first)->title);
	debug(133, "genre: %s", (*first)->genre);
	debug(133, "writer: %s", (*first)->writer);
	debug(133, "director: %s", (*first)->director);
	debug(133, "released: %s", (*first)->released);
	debug(133, "actors: %s", (*first)->actors);
	debug(133, "plot: %s", (*first)->plot);
	debug(133, "poster: %s", (*first)->poster);
	debug(133, "thumb: %s", (*first)->thumb);
	debug(133, "----------------------imdb end----------------------");

	return *first;
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
	struct skin* load = getscreen("loading");
	struct imdb* node = NULL;
	char* search = NULL;

	setfbtransparent(255);
	status.hangtime = 99999;

	if(title == NULL) title = getepgakttitle(NULL);

	drawscreen(load, 0, 0);
	node = getimdb(&node, title, 0, 0, 0);
	clearscreen(load);

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

	drawscreen(imdbskin, 0, 0);

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
			freeimdb(&node, 0); node = NULL;
			drawscreen(load, 0, 0);
			node = getimdb(&node, search, 0, 0, 0);
			clearscreen(load);
			free(search); search = NULL;
			goto start;
		}
		drawscreen(imdbskin, 0, 0);
		continue;
		}
	}

	freeimdb(&node, 0); node = NULL;
	setosdtransparent(getskinconfigint("osdtransparent", NULL));
	status.hangtime = getconfigint("hangtime", NULL);
	clearscreen(imdbskin);
}

#endif
