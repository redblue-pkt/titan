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
struct imdb* getimdb(struct imdb** first, char* input, int flag, int flag1, int flag2)
{
	char* tmpstr = NULL, *tmpstr1 = NULL, *tmpstr2 = NULL, *tmpsearch = NULL, *savefile = NULL, *pageposter = NULL, *title = NULL, *tmpurl = NULL;

	debug(133, "title: %s",input);
	debug(133, "flag: %d",flag);
	debug(133, "flag1: %d",flag1);
	debug(133, "flag2: %d",flag2);

	title = ostrcat(title, input, 1, 0);
		
start:
	debug(133, "title: %s",title);
//	tmpsearch = ostrcat("find?s=tt;q=", NULL, 0, 0);
	tmpsearch = ostrcat("find?q=", NULL, 0, 0);

	if(checkinternet() == 1)
	{
		debug(133, "Error, Internet Connection not found");
		return NULL;
	}

	if(flag == 0)
	{
		tmpsearch = ostrcat(tmpsearch, title, 1, 0);
		tmpsearch = ostrcat(tmpsearch, "&s=all", 1, 0);
	}
	else
		tmpsearch = ostrcat("/title/tt", title, 0, 0);
	tmpsearch = stringreplacechar(tmpsearch, ' ', '+');

	debug(133, "search: http://www.imdb.com/%s", tmpsearch);

	tmpurl = ostrcat("www.imdb.com/", tmpsearch, 0, 0);
	tmpstr = gethttps(tmpurl, NULL, NULL, NULL, NULL, NULL, 1);

//	tmpstr = gethttp("www.imdb.com", tmpsearch, 80, NULL, NULL, 5000, NULL, 0);
//	writesys("/var/usr/local/share/titan/plugins/imdb/tmpstr0", tmpstr, 1);

	debug(133, "tmpsearch: %s", tmpsearch);
	free(tmpsearch); tmpsearch = NULL;

	if(tmpstr != NULL)
	{
		if(ostrstr(tmpstr, "No results found for") != NULL)
		{
			debug(133, "No results found for");
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

		if(flag == 0)
			(*first)->id = string_resub("<td class=\"primary_photo\"> <a href=\"/title/tt", "/", tmpstr, 0);
		else
			(*first)->id = ostrcat(title, NULL, 0, 0);

//		if(flag1 == 1)
		if(flag != 2 && flag1 == 1)
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
			char* tmp = tmpstr;
			tmpstr = string_resub("<p><b>Titel", "</td></tr></table> </p>", tmpstr, 0);
			free(tmp); tmp = NULL;
//			writesys("/var/usr/local/share/titan/plugins/imdb/tmpstr", tmpstr, 0);
			
			while(ostrstr(tmpstr, "</td><td valign=\"top\"><a href=\"/title/") != NULL)
			{
				tmpstr = string_replace("</td><td valign=\"top\"><a href=\"/title/", "\n\n", tmpstr, 1);
			}
//			writesys("/var/usr/local/share/titan/plugins/imdb/tmpstr1", tmpstr, 0);

			struct menulist* mlist = NULL, *mbox = NULL;
			
			int count = 0;
			struct splitstr* ret = NULL;
			ret = strsplit(tmpstr, "\n", &count);

			int max = count;
			int i = 0;
			for(i = 0; i < max; i++)
			{
//				tmpstr1 = string_resub("link=/title", "</a>", (&ret[i])->part, 0);
				tmpstr1 = string_resub("link=/title", "</td>", (&ret[i])->part, 0);
				debug(133, "tmpstr1(%d): %s\n", i, tmpstr1);
				tmpstr1 = string_replace("</a>", "", tmpstr1, 1);
				debug(133, "tmpstr1(%d): %s\n", i, tmpstr1);
	
				char* x = oregex("/tt([0-9]{7})", tmpstr1);
				char* y = oregex(";\">(.*)", tmpstr1);
				if(ostrstr(y, "<img src") == NULL && x != NULL && y != NULL)
				{
					debug(133, "x(%d): %s\n", i, x);
					debug(133, "y(%d): %s\n", i, y);				
					string_striptags(x);
					string_striptags(y);
					string_strip_whitechars(x);
					string_strip_whitechars(y);
					debug(133, "x(%d): %s\n", i, x);
					debug(133, "y(%d): %s\n", i, y);
					y = string_decode(y, 1);
					x = string_decode(x, 1);
					debug(133, "x(%d): %s\n", i, x);
					debug(133, "y(%d): %s\n", i, y);

					addmenulist(&mlist, y, x, NULL, 0, 0);
				}
				free(x), x = NULL;
				free(y), y = NULL;				
/*				
current not working
				struct regex* x = regexstruct("/tt([0-9]{7})", tmpstr1);
				struct regex* y = regexstruct(";\">(.*)", tmpstr1);
				if(x != NULL && y != NULL)
				{
					debug(133, "x->match2(%d): %s\n", i, x->match2);
					debug(133, "y->match2(%d): %s\n", i, y->match2);
					x->match2 = string_decode(x->match2, 1);
					y->match2 = string_decode(y->match2, 1);
					addmenulist(&mlist, y->match2, x->match2, NULL, 0, 0);
					debug(133, "x->match2(%d): %s\n", i, x->match2);
					debug(133, "y->match2(%d): %s\n", i, y->match2);
				}
				freeregexstruct(x); x = NULL;
				freeregexstruct(y); y = NULL;
*/
				free(tmpstr1),tmpstr1 = NULL;				
			}
			free(ret); ret = NULL;
				
			mbox = menulistbox(mlist, NULL, NULL, NULL, NULL, NULL, 1, 0);
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
		if(flag != 2)
			free(tmpstr), tmpstr = NULL;		
	}

	if(*first != NULL && flag != 2)
	{
		if((*first)->id == NULL)
			free(tmpstr), tmpstr = NULL;
		else
		{
			debug(133, "imdb->id: %s", (*first)->id);
			
			tmpsearch = ostrcat("/title/tt", NULL, 0, 0);
			tmpsearch = ostrcat(tmpsearch, (*first)->id, 1, 0);

			free(tmpurl), tmpurl = NULL;
			tmpurl = ostrcat("www.imdb.com/", tmpsearch, 0, 0);
			tmpstr = gethttps(tmpurl, NULL, NULL, NULL, NULL, NULL, 1);

//			tmpstr = gethttp("www.imdb.com", tmpsearch, 80, NULL, NULL, 5000, NULL, 0);
//			writesys("/var/usr/local/share/titan/plugins/imdb/tmpstrj", tmpstr, 0);
			
			debug(133, "tmpsearch: %s", tmpsearch);
			free(tmpsearch); tmpsearch = NULL;
		}
	}

	if(*first != NULL && tmpstr != NULL)
	{
		if(ostrstr(tmpstr, "<title>") != NULL)
		{		
			(*first)->title = string_resub("<title>", "</title>", tmpstr, 0);
			(*first)->title = string_decode((*first)->title, 1);
			string_strip_whitechars((*first)->title);
			strstrip((*first)->title);
		}

		if(ostrstr(tmpstr, "\"genre\">") != NULL)
		{
			tmpstr1 = string_resub("&nbsp;-&nbsp;", "</div>", tmpstr, 0);

			while(ostrstr(tmpstr1, "\"genre\"") != NULL)
			{
				tmpstr2 = string_resub("\"genre\">", "</span>", tmpstr1, 0);
				tmpstr1 = string_replace("\"genre\">", "", tmpstr1, 1);
				if((*first)->genre != NULL && tmpstr2 != NULL)
					(*first)->genre = ostrcat((*first)->genre, ",", 1, 0);

				(*first)->genre = ostrcat((*first)->genre, tmpstr2, 1, 0);
				free(tmpstr2), tmpstr2 = NULL;
			}
			free(tmpstr1), tmpstr1 = NULL;

			string_striptags((*first)->genre);
			string_strip_whitechars((*first)->genre);
			strstrip((*first)->genre);
		}
			
		if(ostrstr(tmpstr, ">Writers:</h4>") != NULL)
		{
			tmpstr1 = string_resub(">Writers:</h4>", "</div>", tmpstr, 0);

			while(ostrstr(tmpstr1, "\"name\"") != NULL)
			{
				tmpstr2 = string_resub("\"name\">", "</span>", tmpstr1, 0);
				tmpstr1 = string_replace("\"name\">", "", tmpstr1, 1);
				if((*first)->writer != NULL && tmpstr2 != NULL)
					(*first)->writer = ostrcat((*first)->writer, ",", 1, 0);

				(*first)->writer = ostrcat((*first)->writer, tmpstr2, 1, 0);
				free(tmpstr2), tmpstr2 = NULL;
			}
			free(tmpstr1), tmpstr1 = NULL;

			string_striptags((*first)->writer);
			string_strip_whitechars((*first)->writer);
			strstrip((*first)->writer);
		}

		if((*first)->writer == NULL && ostrstr(tmpstr, ">Writer:</h4>") != NULL)
		{
			tmpstr1 = string_resub(">Writer:</h4>", "</div>", tmpstr, 0);

			while(ostrstr(tmpstr1, "\"name\"") != NULL)
			{
				tmpstr2 = string_resub("\"name\">", "</span>", tmpstr1, 0);
				tmpstr1 = string_replace("\"name\">", "", tmpstr1, 1);
				if((*first)->writer != NULL && tmpstr2 != NULL)
					(*first)->writer = ostrcat((*first)->writer, ",", 1, 0);

				(*first)->writer = ostrcat((*first)->writer, tmpstr2, 1, 0);
				free(tmpstr2), tmpstr2 = NULL;
			}
			free(tmpstr1), tmpstr1 = NULL;

			string_striptags((*first)->writer);
			string_strip_whitechars((*first)->writer);
			strstrip((*first)->writer);
		}

		if(ostrstr(tmpstr, ">Director:</h4>") != NULL)
		{
			tmpstr1 = string_resub(">Director:</h4>", "</div>", tmpstr, 0);

			while(ostrstr(tmpstr1, "\"name\"") != NULL)
			{
				tmpstr2 = string_resub("\"name\">", "</span>", tmpstr1, 0);
				tmpstr1 = string_replace("\"name\">", "", tmpstr1, 1);
				if((*first)->director != NULL && tmpstr2 != NULL)
					(*first)->director = ostrcat((*first)->director, ",", 1, 0);

				(*first)->director = ostrcat((*first)->director, tmpstr2, 1, 0);
				free(tmpstr2), tmpstr2 = NULL;
			}
			free(tmpstr1), tmpstr1 = NULL;

			string_striptags((*first)->director);
			string_strip_whitechars((*first)->director);
			strstrip((*first)->director);
		}

		if((*first)->director == NULL && ostrstr(tmpstr, ">Directors:</h4>") != NULL)
		{
			tmpstr1 = string_resub(">Directors:</h4>", "</div>", tmpstr, 0);

			while(ostrstr(tmpstr1, "\"name\"") != NULL)
			{
				tmpstr2 = string_resub("\"name\">", "</span>", tmpstr1, 0);
				tmpstr1 = string_replace("\"name\">", "", tmpstr1, 1);
				if((*first)->director != NULL && tmpstr2 != NULL)
					(*first)->director = ostrcat((*first)->director, ",", 1, 0);

				(*first)->director = ostrcat((*first)->director, tmpstr2, 1, 0);
				free(tmpstr2), tmpstr2 = NULL;
			}
			free(tmpstr1), tmpstr1 = NULL;

			string_striptags((*first)->director);
			string_strip_whitechars((*first)->director);
			strstrip((*first)->director);
		}

		if((*first)->director == NULL && ostrstr(tmpstr, "<b>Director:</b>") != NULL)
		{
			(*first)->director = string_resub("<b>Director:</b>", "</div>", tmpstr, 0);
			string_striptags((*first)->director);
			string_strip_whitechars((*first)->director);
			strstrip((*first)->director);
		}

		if((*first)->director == NULL && ostrstr(tmpstr, ">Creator:</h4>") != NULL)
		{
			tmpstr1 = string_resub(">Creator:</h4>", "</div>", tmpstr, 0);

			while(ostrstr(tmpstr1, "\"name\"") != NULL)
			{
				tmpstr2 = string_resub("\"name\">", "</span>", tmpstr1, 0);
				tmpstr1 = string_replace("\"name\">", "", tmpstr1, 1);
				if((*first)->director != NULL && tmpstr2 != NULL)
					(*first)->director = ostrcat((*first)->director, ",", 1, 0);

				(*first)->director = ostrcat((*first)->director, tmpstr2, 1, 0);
				free(tmpstr2), tmpstr2 = NULL;
			}
			free(tmpstr1), tmpstr1 = NULL;

			string_striptags((*first)->director);
			string_strip_whitechars((*first)->director);
			strstrip((*first)->director);
		}

		if((*first)->director == NULL && ostrstr(tmpstr, ">Creators:</h4>") != NULL)
		{
			tmpstr1 = string_resub(">Creators:</h4>", "</div>", tmpstr, 0);

			while(ostrstr(tmpstr1, "\"name\"") != NULL)
			{
				tmpstr2 = string_resub("\"name\">", "</span>", tmpstr1, 0);
				tmpstr1 = string_replace("\"name\">", "", tmpstr1, 1);
				if((*first)->director != NULL && tmpstr2 != NULL)
					(*first)->director = ostrcat((*first)->director, ",", 1, 0);

				(*first)->director = ostrcat((*first)->director, tmpstr2, 1, 0);
				free(tmpstr2), tmpstr2 = NULL;
			}
			free(tmpstr1), tmpstr1 = NULL;

			string_striptags((*first)->director);
			string_strip_whitechars((*first)->director);
			strstrip((*first)->director);
		}

		if(ostrstr(tmpstr, ">Release Date:</h4>") != NULL)
		{
			(*first)->released = string_resub(">Release Date:</h4>", "<span", tmpstr, 0);
			string_striptags((*first)->released);
			string_strip_whitechars((*first)->released);
			strstrip((*first)->released);
		}

		if(ostrstr(tmpstr, ">Star:</h4>") != NULL)
		{
			tmpstr1 = string_resub(">Star:</h4>", "</div>", tmpstr, 0);

			while(ostrstr(tmpstr1, "\"name\"") != NULL)
			{
				tmpstr2 = string_resub("\"name\">", "</span>", tmpstr1, 0);
				tmpstr1 = string_replace("\"name\">", "", tmpstr1, 1);
				if((*first)->actors != NULL && tmpstr2 != NULL)
					(*first)->actors = ostrcat((*first)->actors, ",", 1, 0);

				(*first)->actors = ostrcat((*first)->actors, tmpstr2, 1, 0);
				free(tmpstr2), tmpstr2 = NULL;
			}
			free(tmpstr1), tmpstr1 = NULL;

			string_striptags((*first)->actors);
			string_strip_whitechars((*first)->actors);
			strstrip((*first)->actors);
		}

		if(ostrstr(tmpstr, ">Stars:</h4>") != NULL)
		{
			tmpstr1 = string_resub(">Stars:</h4>", "</div>", tmpstr, 0);

			while(ostrstr(tmpstr1, "\"name\"") != NULL)
			{
				tmpstr2 = string_resub("\"name\">", "</span>", tmpstr1, 0);
				tmpstr1 = string_replace("\"name\">", "", tmpstr1, 1);
				if((*first)->actors != NULL && tmpstr2 != NULL)
					(*first)->actors = ostrcat((*first)->actors, ",", 1, 0);

				(*first)->actors = ostrcat((*first)->actors, tmpstr2, 1, 0);
				free(tmpstr2), tmpstr2 = NULL;
			}
			free(tmpstr1), tmpstr1 = NULL;

			string_striptags((*first)->actors);
			string_strip_whitechars((*first)->actors);
			strstrip((*first)->actors);
		}

		if(ostrstr(tmpstr, "\"description\">") != NULL)
		{
			tmpstr1 = string_resub("\"description\">", "</div>", tmpstr, 0);
			(*first)->plot = string_resub("<p>", "</p>", tmpstr1, 0);
		}
	
		if(ostrstr(tmpstr, "<meta property='og:image' content=\"") != NULL)
		{
			(*first)->thumb = string_resub("<meta property='og:image' content=\"", "\"", tmpstr, 0);
//			(*first)->poster = string_resub("<meta property='og:image' content=\"", "\"", tmpstr, 0);
		}

		if(ostrstr(tmpstr, "/media/rm") != NULL)
		{
			pageposter = string_resub("/media/rm", "/", tmpstr, 0);
			strstrip(pageposter);
		}
		free(tmpstr), tmpstr = NULL;
	}	

	if(*first != NULL && pageposter != NULL)
	{
		tmpsearch = ostrcat("/media/rm", pageposter, 0, 0);
		tmpsearch = ostrcat(tmpsearch, "/tt", 1, 0);
		tmpsearch = ostrcat(tmpsearch, (*first)->id, 1, 0);
		tmpsearch = ostrcat(tmpsearch, "/", 1, 0);

		free(tmpurl), tmpurl = NULL;
		tmpurl = ostrcat("www.imdb.com/", tmpsearch, 0, 0);
		tmpstr = gethttps(tmpurl, NULL, NULL, NULL, NULL, NULL, 1);	

//		tmpstr = gethttp("www.imdb.com", tmpsearch, 80, NULL, NULL, 5000, NULL, 0);
		
		debug(133, "tmpsearch: %s", tmpsearch);
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

//	if(*first != NULL && (*first)->id == NULL)
	if(*first != NULL && (*first)->id != NULL)
	{
		tmpsearch = ostrcat("/title/tt", NULL, 0, 0);
		tmpsearch = ostrcat(tmpsearch, (*first)->id, 1, 0);
		tmpsearch = ostrcat(tmpsearch, "/plotsummary", 1, 0);

		free(tmpurl), tmpurl = NULL;
		tmpurl = ostrcat("www.imdb.com/", tmpsearch, 0, 0);
		tmpstr = gethttps(tmpurl, NULL, NULL, NULL, NULL, NULL, 1);

//		tmpstr = gethttp("www.imdb.com", tmpsearch, 80, NULL, NULL, 5000, NULL, 0);
//		writesys("/var/usr/local/share/titan/plugins/imdb/tmpstrplot", tmpstr, 0);
		
		debug(133, "tmpsearch: %s", tmpsearch);
					
		free(tmpsearch); tmpsearch = NULL;
	}

	if(*first != NULL && tmpstr != NULL)
	{
		if(ostrstr(tmpstr, "<p class=\"plotSummary\">") != NULL)
		{
			tmpstr1 = string_resub("<p class=\"plotSummary\">", "<span", tmpstr, 0);
			if(tmpstr1 != NULL)
			{
				(*first)->plot = ostrcat((*first)->plot, tmpstr1, 1, 0);
				string_striptags((*first)->plot);
				string_strip_whitechars((*first)->plot);
				strstrip((*first)->plot);
			}
			free(tmpstr1), tmpstr1 = NULL;
		}
		free(tmpstr), tmpstr = NULL;
	}
		
	if(flag2 == 0 && *first != NULL && (*first)->id != NULL)
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

				free(tmpurl), tmpurl = NULL;
				tmpurl = ostrcat("https:/", path, 0, 0);
				gethttps(tmpurl, savefile, NULL, NULL, NULL, NULL, 0);

//				gethttp(ip, path, 80, savefile, NULL, 5000, NULL, 0);
				free((*first)->poster);
				(*first)->poster = savefile;
			}
			else
			{
				free(tmpurl), tmpurl = NULL;
				tmpurl = ostrcat("https:/", path, 0, 0);
				gethttps(tmpurl, TMPIMDBPIC1, NULL, NULL, NULL, NULL, 0);

//				gethttp(ip, path, 80, TMPIMDBPIC1, NULL, 5000, NULL, 0);
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

				free(tmpurl), tmpurl = NULL;
				tmpurl = ostrcat("https:/", path, 0, 0);
				gethttps(tmpurl, savefile, NULL, NULL, NULL, NULL, 0);

//				gethttp(ip, path, 80, savefile, NULL, 5000, NULL, 0);
				free((*first)->thumb);
				(*first)->thumb = savefile;
			}
			else
			{
				free(tmpurl), tmpurl = NULL;
				tmpurl = ostrcat("https:/", path, 0, 0);
				gethttps(tmpurl, TMPIMDBPIC2, NULL, NULL, NULL, NULL, 0);

//				gethttp(ip, path, 80, TMPIMDBPIC2, NULL, 5000, NULL, 0);
				free((*first)->thumb);
				(*first)->thumb = ostrcat(TMPIMDBPIC2, NULL, 0, 0);
			}
	
			free(ip); ip = NULL;
		}
	}
	else if(*first != NULL)
	{
		if((*first)->thumb != NULL)
		{
			free((*first)->thumb);
			(*first)->thumb = NULL;
		}
		if((*first)->poster != NULL)
		{
			free((*first)->poster);
			(*first)->poster = NULL;
		}
	}

	if(*first != NULL && (*first)->id != NULL)
		(*first)->id = ostrcat("tt", (*first)->id, 0, 1);

	free(tmpstr); tmpstr = NULL;

	if(*first != NULL)
	{
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
	}

	return *first;
}

void screenimdb(char* title, char* dummy1, char* dummy2, char* path, char* file)
{
	debug(133, "title: %s",title);
	debug(133, "path: %s",path);
	debug(133, "file: %s",file);
	char* searchstr = NULL, *tmpstr = NULL;	
	int rcret = 0;

	if(checkinternet() == 1)
	{
		textbox(_("Message"), _("Error, Internet Connection not found"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 1100, 500, 10, 0);
		return;
	}

	struct skin* imdbskin = getscreen("imdb");
	struct skin* skin_plot = getscreennode(imdbskin, "plot");
	struct skin* skin_title = getscreennode(imdbskin, "title");
	struct skin* skin_director = getscreennode(imdbskin, "director");
	struct skin* skin_writers = getscreennode(imdbskin, "writers");
	struct skin* skin_genre = getscreennode(imdbskin, "genre");
	struct skin* skin_releasetime = getscreennode(imdbskin, "releasetime");
	struct skin* skin_cover = getscreennode(imdbskin, "cover");
	struct skin* skin_actors = getscreennode(imdbskin, "actors");
	struct skin* skin_b2 = getscreennode(imdbskin, "b2");
	struct skin* load = getscreen("loading");
	struct skin* blackscreen = getscreen("blackscreen");

	struct imdb* node = NULL;
	char* search = NULL;

	if(path == NULL || file == NULL || !file_exist(getconfig("mediadbpath", NULL)))
		skin_b2->hidden = YES;
	else
		skin_b2->hidden = NO;
			
//	setfbtransparent(255);
	status.hangtime = 99999;

	if(title == NULL)
		searchstr = getepgakttitle(NULL);
	else
	{
		int isrec = 0, iscam = 0;
		tmpstr = createshortname(file, &isrec, &iscam, 1);
		if(tmpstr != NULL)
			searchstr = ostrcat(searchstr, tmpstr, 1, 0);
		else
			searchstr = ostrcat(searchstr, title, 1, 0);
	}

	debug(133, "searchstr: %s",searchstr);

	drawscreen(blackscreen, 0, 0);
	drawscreen(load, 0, 0);

	node = getimdb(&node, searchstr, 0, 0, 0);

	clearscreen(load);
	clearscreen(blackscreen);

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
		skin_cover->hidden = NO;
	}
	else
	{
		changetext(skin_plot, _("--plot--"));
		changetext(skin_title, _("--title--"));
		changetext(skin_director, _("--director--"));
		changetext(skin_writers, _("--writers--"));
		changetext(skin_genre, _("--genre--"));
		changetext(skin_releasetime, _("--releasetime--"));
		changetext(skin_actors, _("--actors--"));
		skin_cover->hidden = YES;
	}

	drawscreen(imdbskin, 0, 0);

	while(1)
	{
		rcret = waitrc(imdbskin, 0, 0);
	
		if(rcret == getrcconfigint("rcexit", NULL)) break;
		if(rcret == getrcconfigint("rcok", NULL)) break;

		if(rcret == getrcconfigint("rcred", NULL))
		{
			search = textinput(_("Search"), searchstr);
			if(search != NULL)
			{
				freeimdb(&node, 0); node = NULL;
				drawscreen(blackscreen, 0, 0);
				drawscreen(load, 0, 0);
				node = getimdb(&node, search, 0, 0, 0);
				clearscreen(load);
				clearscreen(blackscreen);
				free(search), search = NULL;
				goto start;
			}
			drawscreen(imdbskin, 0, 0);
			continue;
		}
		
		if(path != NULL && file != NULL && node != NULL && node->id != NULL && rcret == getrcconfigint("rcgreen", NULL))
		{
			drawscreen(blackscreen, 0, 0);
			drawscreen(load, 0, 0);
			debug(133, "path: %s",path);
			debug(133, "file: %s",file);
			debug(133, "type: 2");
			debug(133, "imdbid: %s",node->id);

			addconfigtmp("mediadbscantimeout", "0");
			mediadbfindfilecb(path, file, 0, node->id, 1);
			delconfigtmp("mediadbscantimeout");

			clearscreen(load);
			clearscreen(blackscreen);
			break;
		}
	}

	free(searchstr), searchstr = NULL;
	freeimdb(&node, 0); node = NULL;
//	setosdtransparent(getskinconfigint("osdtransparent", NULL));
	status.hangtime = getconfigint("hangtime", NULL);
	clearscreen(imdbskin);
}

#endif
