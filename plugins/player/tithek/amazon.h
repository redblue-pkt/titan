#ifndef AMAZON_H
#define AMAZON_H

// imdb
//http://api-doc.imdbws.com/static-content/documents/v1/de-DE/video/lovefilm_de/amzn1.dv.vcid.183d4cd2-d2f0-4d5d-8712-771b05cb0feb/events:imdb.api.video.events/data.json?Expires=1442152236&Signature=ay5qtA6EvuUQ253Ch2-MIGLt~GgrFl1gFNxhvgME9nyd5BP9vmX9nnusTJYgnqXlbDxa0XlDkPxm0zEOOlaQaRhTbxT5VOV75KZuOlJXb9PQou75KbT8fbWfhY5LJpKrHeLkqU5xSuV8TQ0L2OK997GL5N7147vf2gs3eya8PyA_&Key-Pair-Id=APKAJHKHQ2OMFVLH6TKA

void amazon_init(char* titheklink, char* tithekfile)
{
	int ret = 0;
	char* tmpstr = NULL;
	
	if(ostrcmp("http://openaaf.dyndns.tv/mediathek/mainmenu.list", titheklink) == 0)
		writesys(tithekfile, "Amazon#/tmp/tithek/amazon.mainmenu.list#http://openaaf.dyndns.tv/mediathek/menu/amazon.jpg#amazon.jpg#TiThek#0", 3);

	if(ostrcmp("/tmp/tithek/amazon.mainmenu.list", titheklink) == 0)
	{
		if(amazonlogin == 0)
		{
			ret = amazon_login();
			if(ret == 0)
				textbox(_("Message"), _("Amazon login not successful! Please check Amazon Prime User/Pass in Tithek settings and try again."), _("OK"), getrcconfigint("rcok", NULL), NULL, 0, NULL, 0, NULL, 0, 1100, 200, 0, 0);
			else
			{
				textbox(_("Message"), _("Amazon Prime login successful!"), _("OK"), getrcconfigint("rcok", NULL), NULL, 0, NULL, 0, NULL, 0, 800, 200, 10, 0);
				amazonlogin = 1;
			}
		}
		unlink(tithekfile);
		printf("add amazon mainmenu entrys\n");


//		writesys(tithekfile, "Currently (Movie)#/tmp/tithek/amazon.currently.movie.list#http://openaaf.dyndns.tv/mediathek/menu/currently.movie.jpg#currently.movie.jpg#Amazon#86", 3);
		tmpstr = ostrcat(_("Currently"), " (", 0, 0);
		tmpstr = ostrcat(tmpstr, _("Movies"), 0, 0);
		tmpstr = ostrcat(tmpstr, ")#/tmp/tithek/amazon.currently.movie.list#http://openaaf.dyndns.tv/mediathek/menu/currently.movie.jpg#currently.movie.jpg#Amazon#86", 0, 0);
		writesys(tithekfile, tmpstr, 3);
		free(tmpstr), tmpstr = NULL;

//		writesys(tithekfile, "Popularity Kids (Movie)#/tmp/tithek/amazon.popularity.kids.movie.list#http://openaaf.dyndns.tv/mediathek/menu/popularity.kids.movie.jpg#popularity.kids.movie.jpg#Amazon#84", 3);
		tmpstr = ostrcat(_("Popularity Kids"), " (", 0, 0);
		tmpstr = ostrcat(tmpstr, _("Movies"), 0, 0);
		tmpstr = ostrcat(tmpstr, ")#/tmp/tithek/amazon.popularity.kids.movie.list#http://openaaf.dyndns.tv/mediathek/menu/popularity.kids.movie.jpg#popularity.kids.movie.jpg#Amazon#84", 0, 0);
		writesys(tithekfile, tmpstr, 3);
		free(tmpstr), tmpstr = NULL;

//		writesys(tithekfile, "Popularity Kids (Series)#/tmp/tithek/amazon.popularity.kids.tv.list#http://openaaf.dyndns.tv/mediathek/menu/popularity.kids.tv.jpg#popularity.kids.tv.jpg#Amazon#85", 3);
		tmpstr = ostrcat(_("Popularity Kids"), " (", 0, 0);
		tmpstr = ostrcat(tmpstr, _("Series"), 0, 0);
		tmpstr = ostrcat(tmpstr, ")#/tmp/tithek/amazon.popularity.kids.tv.list#http://openaaf.dyndns.tv/mediathek/menu/popularity.kids.tv.jpg#popularity.kids.tv.jpg#Amazon#85", 0, 0);
		writesys(tithekfile, tmpstr, 3);
		free(tmpstr), tmpstr = NULL;

//		writesys(tithekfile, "Popularity (Movie)#/tmp/tithek/amazon.popularity.rank.movie.list#http://openaaf.dyndns.tv/mediathek/menu/popularity.rank.movie.jpg#popularity.rank.movie.jpg#Amazon#81", 3);
		tmpstr = ostrcat(_("Popularity Rank"), " (", 0, 0);
		tmpstr = ostrcat(tmpstr, _("Movies"), 0, 0);
		tmpstr = ostrcat(tmpstr, ")#/tmp/tithek/amazon.popularity.rank.movie.list#http://openaaf.dyndns.tv/mediathek/menu/popularity.rank.movie.jpg#popularity.rank.movie.jpg#Amazon#81", 0, 0);
		writesys(tithekfile, tmpstr, 3);
		free(tmpstr), tmpstr = NULL;

//		writesys(tithekfile, "Popularity (Series)#/tmp/tithek/amazon.popularity.rank.tv.list#http://openaaf.dyndns.tv/mediathek/menu/popularity.rank.tv.jpg#popularity.rank.tv.jpg#Amazon#83", 3);
		tmpstr = ostrcat(_("Popularity Rank"), " (", 0, 0);
		tmpstr = ostrcat(tmpstr, _("Series"), 0, 0);
		tmpstr = ostrcat(tmpstr, ")#/tmp/tithek/amazon.popularity.rank.tv.list#http://openaaf.dyndns.tv/mediathek/menu/popularity.rank.tv.jpg#popularity.rank.tv.jpg#Amazon#83", 0, 0);
		writesys(tithekfile, tmpstr, 3);
		free(tmpstr), tmpstr = NULL;

//		writesys(tithekfile, "Genre (Movie)#/tmp/tithek/amazon.genre.movie.list#http://openaaf.dyndns.tv/mediathek/menu/genre.movie.jpg#watchlist.movie.jpg#Amazon#87", 3);
		tmpstr = ostrcat(_("Genre"), " (", 0, 0);
		tmpstr = ostrcat(tmpstr, _("Movies"), 0, 0);
		tmpstr = ostrcat(tmpstr, ")#/tmp/tithek/amazon.genre.movie.list#http://openaaf.dyndns.tv/mediathek/menu/genre.movie.jpg#watchlist.movie.jpg#Amazon#87", 0, 0);
		writesys(tithekfile, tmpstr, 3);
		free(tmpstr), tmpstr = NULL;

//		writesys(tithekfile, "Genre (Series)#/tmp/tithek/amazon.genre.tv.list#http://openaaf.dyndns.tv/mediathek/menu/genre.tv.jpg#watchlist.movie.jpg#Amazon#89", 3);
		tmpstr = ostrcat(_("Genre"), " (", 0, 0);
		tmpstr = ostrcat(tmpstr, _("Series"), 0, 0);
		tmpstr = ostrcat(tmpstr, ")#/tmp/tithek/amazon.genre.tv.list#http://openaaf.dyndns.tv/mediathek/menu/genre.tv.jpg#watchlist.movie.jpg#Amazon#89", 0, 0);
		writesys(tithekfile, tmpstr, 3);
		free(tmpstr), tmpstr = NULL;

//		writesys(tithekfile, "Watchlist (Movie)#/tmp/tithek/amazon.watchlist.movie.list#http://openaaf.dyndns.tv/mediathek/menu/watchlist.movie.jpg#watchlist.movie.jpg#Amazon#79", 3);
		tmpstr = ostrcat(_("Watchlist"), " (", 0, 0);
		tmpstr = ostrcat(tmpstr, _("Movies"), 0, 0);
		tmpstr = ostrcat(tmpstr, ")#/tmp/tithek/amazon.watchlist.movie.list#http://openaaf.dyndns.tv/mediathek/menu/watchlist.movie.jpg#watchlist.movie.jpg#Amazon#79", 0, 0);
		writesys(tithekfile, tmpstr, 3);
		free(tmpstr), tmpstr = NULL;

//		writesys(tithekfile, "Watchlist (Series)#/tmp/tithek/amazon.watchlist.tv.list#http://openaaf.dyndns.tv/mediathek/menu/watchlist.tv.jpg#watchlist.tv.jpg#Amazon#80", 3);
		tmpstr = ostrcat(_("Watchlist"), " (", 0, 0);
		tmpstr = ostrcat(tmpstr, _("Series"), 0, 0);
		tmpstr = ostrcat(tmpstr, ")#/tmp/tithek/amazon.watchlist.tv.list#http://openaaf.dyndns.tv/mediathek/menu/watchlist.tv.jpg#watchlist.tv.jpg#Amazon#80", 0, 0);
		writesys(tithekfile, tmpstr, 3);
		free(tmpstr), tmpstr = NULL;

//		writesys(tithekfile, "Search (Movie)#/tmp/tithek/amazon.search.movie.list#http://openaaf.dyndns.tv/mediathek/menu/search.jpg#search.jpg#Amazon#76", 3);
		tmpstr = ostrcat(_("Search"), " (", 0, 0);
		tmpstr = ostrcat(tmpstr, _("Movies"), 0, 0);
		tmpstr = ostrcat(tmpstr, ")#/tmp/tithek/amazon.search.movie.list#http://openaaf.dyndns.tv/mediathek/menu/search.jpg#search.jpg#Amazon#76", 0, 0);
		writesys(tithekfile, tmpstr, 3);
		free(tmpstr), tmpstr = NULL;

//		writesys(tithekfile, "Search (Tv)#/tmp/tithek/amazon.search.tv.list#http://openaaf.dyndns.tv/mediathek/menu/search.jpg#search.jpg#Amazon#77", 3);
		tmpstr = ostrcat(_("Search"), " (", 0, 0);
		tmpstr = ostrcat(tmpstr, _("Series"), 0, 0);
		tmpstr = ostrcat(tmpstr, ")#/tmp/tithek/amazon.search.tv.list#http://openaaf.dyndns.tv/mediathek/menu/search.jpg#search.jpg#Amazon#77", 0, 0);
		writesys(tithekfile, tmpstr, 3);
		free(tmpstr), tmpstr = NULL;
		
	}
	free(tmpstr), tmpstr = NULL;
}

int amazon_search(struct skin* grid, struct skin* listbox, struct skin* countlabel, struct skin* load, char* link, char* title, char* searchstr, int flag)
{
	printf("amazon_search start flag: %d\n", flag);
	int ret = 1, type = 0;
	int debuglevel = getconfigint("debuglevel", NULL);

	if(listbox == NULL || listbox->select == NULL || listbox->select->handle == NULL)
		return ret;

	char* remove = NULL, *tmpstr2 = NULL, *tmpstr3 = NULL, *filename = NULL, *tmpstr = NULL, *search = NULL, *line = NULL, *url = NULL, *id = NULL, *streamurl = NULL, *pic = NULL, *year = NULL, *runtime = NULL, *menu = NULL;
	char* page = NULL, *pages = NULL, *nextpage = NULL, *tmpstr4 = NULL, *atitle = NULL;

	if(flag == 0 || flag == 1)
	{
		if(searchstr == NULL)
			search = textinputhist(_("Search"), " ", "searchhist");
		else
			search = textinputhist(_("Search"), searchstr, "searchhist");
	}

	if(search != NULL || flag > 0)
	{
		drawscreen(load, 0, 0);
		search = stringreplacechar(search, ' ', '+');
		if(flag == 0)
		{
			filename = ostrcat("movie", NULL, 0, 0);
			type = 75;
			url = ostrcat("http://www.amazon.de/mn/search/ajax/?_encoding=UTF8&url=node%3D3356018031&field-keywords=", search, 0, 0);
		}
		else if(flag == 1)
		{
			filename = ostrcat("season", NULL, 0, 0);
			type = 78;
			url = ostrcat("http://www.amazon.de/mn/search/ajax/?_encoding=UTF8&url=node%3D3356019031&field-keywords=", search, 0, 0);
		}
		else if(flag == 2)
		{
			filename = ostrcat("episode", NULL, 0, 0);
			type = 75;
			url = ostrcat("http://www.amazon.de/dp/", link, 0, 0);
			url = ostrcat(url, "/?_encoding=UTF8", 1, 0);		
		}
		else if(flag == 3)
		{
			filename = ostrcat("watchlist.movie", NULL, 0, 0);
			type = 75;
			url = ostrcat("http://www.amazon.de/gp/video/watchlist/movie?ie=UTF8&show=all&sort=DATE_ADDED_DESC", NULL, 0, 0);
		}
		else if(flag == 4)
		{
			filename = ostrcat("watchlist.tv", NULL, 0, 0);
			type = 78;
			url = ostrcat("http://www.amazon.de/gp/video/watchlist/tv?ie=UTF8&show=all&sort=DATE_ADDED_DESC", NULL, 0, 0);
		}
		else if(flag == 5)
		{
			filename = ostrcat("popularity.rank.movie", NULL, 0, 0);
			type = 75;
			url = ostrcat("http://www.amazon.de/gp/search/ajax/?_encoding=UTF8&rh=n%3A3010075031%2Cn%3A3356018031&sort=popularity-rank", NULL, 0, 0);
		}
		else if(flag == 6)
		{
			filename = ostrcat("popularity.rank.tv.nextpage", NULL, 0, 0);
			type = 75;
			url = ostrcat(link, NULL, 0, 0);
		}
		else if(flag == 7)
		{
			filename = ostrcat("popularity.rank.tv", NULL, 0, 0);
			type = 78;
			url = ostrcat("http://www.amazon.de/gp/search/ajax/?_encoding=UTF8&rh=n%3A3010075031%2Cn%3A3356019031&sort=popularity-rank", NULL, 0, 0);
		}
		else if(flag == 8)
		{
			filename = ostrcat("popularity.kids.movie", NULL, 0, 0);
			type = 75;
			url = ostrcat("http://www.amazon.de/gp/search/ajax/?rh=n%3A3010075031%2Cn%3A!3010076031%2Cn%3A3015915031%2Cp_n_theme_browse-bin%3A3015972031%2Cp_85%3A3282148031&ie=UTF8", NULL, 0, 0);
		}
		else if(flag == 9)
		{
			filename = ostrcat("popularity.kids.tv", NULL, 0, 0);
			type = 78;
			url = ostrcat("http://www.amazon.de/gp/search/ajax/?rh=n%3A3010075031%2Cn%3A!3010076031%2Cn%3A3015916031%2Cp_n_theme_browse-bin%3A3015972031%2Cp_85%3A3282148031&ie=UTF8", NULL, 0, 0);
		}
		else if(flag == 10)
		{
			filename = ostrcat("currently.movie", NULL, 0, 0);
			type = 75;
			url = ostrcat("http://www.amazon.de/s/?n=4963842031&_encoding=UTF", NULL, 0, 0);
		}
		else if(flag == 11)
		{
			filename = ostrcat("genre.movie", NULL, 0, 0);
			type = 88;
			url = ostrcat("http://www.amazon.de/gp/search/other/?rh=n%3A3279204031%2Cn%3A!3010076031%2Cn%3A3356018031&pickerToList=theme_browse-bin&ie=UTF8", NULL, 0, 0);
		}
		else if(flag == 12)
		{
			filename = ostrcat("genre.movie.listed", NULL, 0, 0);
			type = 75;
			url = ostrcat(link, NULL, 0, 0);
		}
		else if(flag == 13)
		{
			filename = ostrcat("genre.tv", NULL, 0, 0);
			type = 90;
			url = ostrcat("http://www.amazon.de/gp/search/other/?rh=n%3A3279204031%2Cn%3A!3010076031%2Cn%3A3356019031&pickerToList=theme_browse-bin&ie=UTF8", NULL, 0, 0);
		}
		else if(flag == 14)
		{
			filename = ostrcat("genre.tv.listed", NULL, 0, 0);
			type = 78;
			url = ostrcat(link, NULL, 0, 0);
		}

///////////////////////////
		debug(99, "url: %s", url);
		tmpstr = gethttps(url, NULL, NULL, NULL, NULL, NULL, 1);
///////////////
		pages = string_resub("<span class=\\\"pagnDisabled\\\">", "</span>", tmpstr, 0);	
		nextpage = string_resub("<span class=\\\"pagnLink\\\"><a href=\\\"", "\\\"", tmpstr, 0);	

		if(pages == NULL)
			pages = string_resub("<span class=\"pagnDisabled\">", "</span>", tmpstr, 0);

		if(nextpage == NULL)
		{
			tmpstr4 = string_resub("class=\"pagnNext\"", ">", tmpstr, 0);	
			nextpage = string_resub("href=\"", "\"", tmpstr4, 0);	
			free(tmpstr4), tmpstr4 = NULL;
		}

		page = oregex(".*/ref=.*([0-9]{2,2})\\?rh=.*", nextpage);
		if(page == NULL)
			page = oregex(".*/ref=.*([0-9]{1,2})\\?rh=.*", nextpage);

		if(pages == NULL)
		{
			tmpstr4 = string_resub("pagnCur", "pagnRA", tmpstr, 0);
			pages = oregex(".*ref=sr_pg_([0-9]{1,2})\\?rh=.*", tmpstr4);
			free(tmpstr4), tmpstr4 = NULL;
		}

		nextpage = string_replace_all("&amp;", "&", nextpage, 1);
//////////////////////

		tmpstr2 = ostrcat("/tmp/amazon_search_tmpstr_get", filename, 0, 0);
		tmpstr2 = ostrcat(tmpstr2, "_page_", 1, 0);
		tmpstr2 = ostrcat(tmpstr2, page, 1, 0);
		tmpstr2 = ostrcat(tmpstr2, "_pages_", 1, 0);
		tmpstr2 = ostrcat(tmpstr2, pages, 1, 0);
		unlink(tmpstr2);

		tmpstr3 = ostrcat(tmpstr2, "_replace", 0, 0);
		unlink(tmpstr3);

///////////////////////////
		titheklog(debuglevel, tmpstr2, NULL, NULL, NULL, tmpstr);		
		free(url), url = NULL;
///////////////////////////

		int count1 = 0;
		int j = 0;

		if(flag == 10)
			tmpstr = stringreplacechar(tmpstr, '\n', ' ');
//			tmpstr = string_replace_all("\n", " ", tmpstr, 1);

		if(flag == 0 || flag == 1 || flag == 5 || flag == 7 || flag == 8 || flag == 9 || flag == 12 || flag == 14)
			tmpstr = string_replace_all("<li id=\\\"result_", "\n<li id=\\\"result_", tmpstr, 1);
		else if(flag == 6 || flag == 10)
			tmpstr = string_replace_all("<li id=\"result_", "\n<li id=\"result_", tmpstr, 1);
		else if(flag == 2)
		{
			tmpstr = string_replace_all("\n", " ", tmpstr, 1);			
			tmpstr = string_replace_all("<li class=\"selected-episode", "\n<li class=\"\">", tmpstr, 1);
			tmpstr = string_replace_all("<li class=\"\">", "\n<li class=\"\">", tmpstr, 1);
//			tmpstr = string_replace_all("<li class=\"last-episode", "\n<li class=\"last-episode", tmpstr, 1);
			tmpstr = string_replace_all("<li class=\"last-episode", "\n<li class=\"\">", tmpstr, 1);
			tmpstr = string_replace_all("<li class=\"first-episode", "\n<li class=\"\">", tmpstr, 1);
			tmpstr = string_replace_all("<li class=\"has-watched", "\n<li class=\"\">", tmpstr, 1);
			tmpstr = string_replace_all("</div> </div>", "\n</div> </div>", tmpstr, 1);	
		}
		else if(flag == 3 || flag == 4)
		{
			tmpstr = string_replace_all("\n", " ", tmpstr, 1);
			tmpstr = string_replace_all("<div class=\"grid-list-item downloadable_", "\n<div class=\"grid-list-item downloadable_", tmpstr, 1);
			tmpstr = string_replace_all("<script type=\"text/javascript\">", "\n<script type=\"text/javascript\">", tmpstr, 1);	
		}
		else if(flag == 11 || flag == 13)
		{
			tmpstr = string_replace_all("\n", " ", tmpstr, 1);
			tmpstr = string_replace_all("<a href=\"/s/ref=sr_in_", "\n<a href=\"/s/ref=sr_in_", tmpstr, 1);
			tmpstr = string_replace_all("</div></div>", "\n</div></div>", tmpstr, 1);
		}

		titheklog(debuglevel, tmpstr3, NULL, NULL, NULL, tmpstr);		

		count1 = 0;
		j = 0;
		struct splitstr* ret1 = NULL;
		ret1 = strsplit(tmpstr, "\n", &count1);
		int incount = 0;

		if(ret1 != NULL && count1 > 0)
		{
			int max = count1;
			for(j = 0; j < max; j++)
			{
				if(ostrstr(ret1[j].part, "<li id=\\\"result_") != NULL)
				{
					debug(99, "(%d) ret1[j].part: %s", j, ret1[j].part);
					streamurl = string_resub("\" href=\\\"", "\\\">", ret1[j].part, 0);
					pic = string_resub("\" src=\\\"", "\\\"", ret1[j].part, 0);
					if(pic == NULL)
						pic = ostrcat(pic, "http://openaaf.dyndns.tv/mediathek/menu/default.jpg", 1, 0);
					atitle = string_resub("\" title=\\\"", "\\\"", ret1[j].part, 0);
					year = string_resub("<span class=\\\"a-size-small a-color-secondary\\\">", "</span>", ret1[j].part, 0);
					runtime = oregex(".*a-size-small a-color-secondary.*>(.*)</span>.*", ret1[j].part);
					id = oregex("http.*//.*/.*/(.*)/ref.*", streamurl);
				}
				if(ostrstr(ret1[j].part, "<li id=\"result_") != NULL)
				{
					debug(99, "(%d) ret1[j].part: %s", j, ret1[j].part);
					streamurl = string_resub("\" href=\"", "\">", ret1[j].part, 0);
					pic = string_resub("\" src=\"", "\"", ret1[j].part, 0);
					if(pic == NULL)
						pic = ostrcat(pic, "http://openaaf.dyndns.tv/mediathek/menu/default.jpg", 1, 0);
					atitle = string_resub("\" title=\"", "\"", ret1[j].part, 0);
					year = string_resub("<span class=\"a-size-small a-color-secondary\">", "</span>", ret1[j].part, 0);
					runtime = oregex(".*a-size-small a-color-secondary.*>(.*)</span>.*", ret1[j].part);
					id = oregex("http.*//.*/.*/(.*)/ref.*", streamurl);
				}
				else if(ostrstr(ret1[j].part, "<li class=\"\">") != NULL)
				{
					debug(99, "(%d) ret1[j].part: %s", j, ret1[j].part);
					streamurl = string_resub("href=\"", "\"", ret1[j].part, 0);
					pic = string_resub("src=\"", "\"", ret1[j].part, 0);
					if(pic == NULL)
						pic = ostrcat(pic, "http://openaaf.dyndns.tv/mediathek/menu/default.jpg", 1, 0);
					atitle = string_resub("<span class=\"episode-title\">", "</span>", ret1[j].part, 0);
					year = string_resub("<span class=\"dv-badge release-date\">", "</span>", ret1[j].part, 0);
					runtime = string_resub("<span class=\"dv-badge runtime\">", "</span>", ret1[j].part, 0);
					id = oregex("http.*//.*/.*/(.*)/ref.*", streamurl);
				}
				else if(ostrstr(ret1[j].part, "<div class=\"grid-list-item downloadable_") != NULL)
				{
					debug(99, "(%d) ret1[j].part: %s", j, ret1[j].part);
					streamurl = string_resub("href=\"", "\"", ret1[j].part, 0);
					pic = string_resub("src=\"", "\"", ret1[j].part, 0);
					if(pic == NULL)
						pic = ostrcat(pic, "http://openaaf.dyndns.tv/mediathek/menu/default.jpg", 1, 0);
					atitle = string_resub("data-title=\"", "\"", ret1[j].part, 0);
					year = string_resub("<span class=\"dv-badge release-date\">", "</span>", ret1[j].part, 0);
					runtime = string_resub("<span class=\"dv-badge runtime\">", "</span>", ret1[j].part, 0);
					id = string_resub("id=\"", "\"", ret1[j].part, 0);
					remove = string_resub("data-action=\"remove\" href=\"", "\"", ret1[j].part, 0);					
					debug(99, "(%d) remove: %s", j, remove);
				}
				else if(ostrstr(ret1[j].part, "<span class=\\\"pagnLink\\\">") != NULL)
				{
					debug(99, "(%d) ret1[j].part: %s", j, ret1[j].part);
					streamurl = string_resub("\" href=\\\"", "\\\">", ret1[j].part, 0);
					pic = string_resub("\" src=\\\"", "\\\"", ret1[j].part, 0);
					if(pic == NULL)
						pic = ostrcat(pic, "http://openaaf.dyndns.tv/mediathek/menu/default.jpg", 1, 0);
					atitle = string_resub("\" title=\\\"", "\\\"", ret1[j].part, 0);
					year = string_resub("<span class=\\\"a-size-small a-color-secondary\\\">", "</span>", ret1[j].part, 0);
					runtime = oregex(".*a-size-small a-color-secondary.*>(.*)</span>.*", ret1[j].part);
					id = oregex("http.*//.*/.*/(.*)/ref.*", streamurl);
				}
				else if(ostrstr(ret1[j].part, "<a href=\"/s/ref=sr_in_") != NULL)
				{
					debug(99, "(%d) ret1[j].part: %s", j, ret1[j].part);
					streamurl = string_resub("<a href=\"", "\">", ret1[j].part, 0);
					streamurl = string_replace_all("/s/", "https://www.amazon.de/mn/search/ajax/", streamurl, 1);
					streamurl = string_replace_all("&amp;", "&", streamurl, 1);
					pic = ostrcat(pic, "http://openaaf.dyndns.tv/mediathek/menu/default.jpg", 1, 0);
					atitle = string_resub("<span class=\"refinementLink\">", "</span>", ret1[j].part, 0);
//					id = oregex("http.*//.*/.*/(.*)/ref.*", streamurl);
					id = string_resub("bbn=", "&", streamurl, 0);
				}

				if(ostrstr(ret1[j].part, "result_") != NULL || ostrstr(ret1[j].part, "<li class=\"\">") != NULL || ostrstr(ret1[j].part, "<div class=\"grid-list-item downloadable_") != NULL || ostrstr(ret1[j].part, "<span class=\\\"pagnLink\\\">") != NULL || ostrstr(ret1[j].part, "<a href=\"/s/ref=sr_in_") != NULL)
				{
					if(atitle == NULL)
					{
						atitle = string_resub("http://www.amazon.de/", "/", streamurl, 0);
						atitle = string_replace_all("-", " ", atitle, 1);
					}
					if(atitle != NULL)
						atitle = string_decode3(atitle);

					if(atitle != NULL)
						htmldecode2(atitle, atitle);

					debug(99, "(%d) streamurl: %s", j, streamurl);
					if(flag != 11 && flag != 13)
					{
						free(streamurl), streamurl = NULL;					
						streamurl = ostrcat("http://www.amazon.de/dp/", id, 0, 0);
						streamurl = ostrcat(streamurl, "/?_encoding=UTF8", 1, 0);
						debug(99, "(%d) streamurl changed: %s", j, streamurl);
					}
					else
					{
						debug(99, "(%d) streamurl: %s", j, streamurl);
					}					
					debug(99, "(%d) id: %s", j, id);
					debug(99, "(%d) pic: %s", j, pic);
					debug(99, "(%d) atitle: %s", j, atitle);
					debug(99, "(%d) year: %s", j, year);
					debug(99, "(%d) runtime: %s", j, runtime);
					debug(99, "----------------------");

					if(id != NULL)
					{
						incount += 1;
						line = ostrcat(line, atitle, 1, 0);
						if(year != NULL)
						{
							line = ostrcat(line, " (", 1, 0);
							line = ostrcat(line, year, 1, 0);
							line = ostrcat(line, ") ", 1, 0);
						}
						if(runtime != NULL)
							line = ostrcat(line, runtime, 1, 0);
						line = ostrcat(line, "#", 1, 0);
						if(flag == 11 || flag == 13)
							line = ostrcat(line, streamurl, 1, 0);
						else
							line = ostrcat(line, id, 1, 0);
						line = ostrcat(line, "#", 1, 0);
						line = ostrcat(line, pic, 1, 0);
						line = ostrcat(line, "#amazon_search_", 1, 0);
						line = ostrcat(line, oitoa(incount + time(NULL)), 1, 1);
						line = ostrcat(line, ".jpg#Amazon - Search#", 1, 0);
						line = ostrcat(line, oitoa(type), 1, 1);
						line = ostrcat(line, "\n", 1, 0);
					}
					
					free(id), id = NULL;
					free(streamurl), streamurl = NULL;
					free(pic), pic = NULL;
					free(atitle), atitle = NULL;
					free(year), year = NULL;
					free(runtime), runtime = NULL;
					free(remove), remove = NULL;
				}
			}
		}
		free(ret1), ret1 = NULL;
		free(tmpstr), tmpstr = NULL;

		if(line != NULL)
		{
			if(nextpage != NULL)
			{
				type = 82;
				url = ostrcat("http://www.amazon.de", nextpage, 0, 0);
				incount += 1;
				line = ostrcat(line, _("Next Page"), 1, 0);
				line = ostrcat(line, " (", 1, 0);
				line = ostrcat(line, page, 1, 0);
				line = ostrcat(line, "/", 1, 0);
				line = ostrcat(line, pages, 1, 0);
				line = ostrcat(line, ")#", 1, 0);
				line = ostrcat(line, url, 1, 0);
				line = ostrcat(line, "#", 1, 0);
				line = ostrcat(line, "http://openaaf.dyndns.tv/mediathek/menu/next.jpg", 1, 0);
				line = ostrcat(line, "#amazon_search_", 1, 0);
				line = ostrcat(line, oitoa(incount + time(NULL)), 1, 1);
				line = ostrcat(line, ".jpg#Amazon - Search#", 1, 0);
				line = ostrcat(line, oitoa(type), 1, 1);
				line = ostrcat(line, "\n", 1, 0);
			}
			debug(99, "line: %s", line);
			menu = ostrcat("/tmp/tithek/amazon.", NULL, 0, 0);
			menu = ostrcat(menu, filename, 1, 0);
			menu = ostrcat(menu, ".page.", 1, 0);
			menu = ostrcat(menu, page, 1, 0);
			menu = ostrcat(menu, ".pages.", 1, 0);
			menu = ostrcat(menu, pages, 1, 0);
			menu = ostrcat(menu, ".nextpage.list", 1, 0);
			writesys(menu, line, 0);
			struct tithek* tnode = (struct tithek*)listbox->select->handle;
			createtithek(tnode, tnode->title, menu, tnode->pic, tnode->localname, tnode->menutitle, tnode->flag);
			ret = 0;
		}

/*
		int i = 0, len = 0, treffer = 0, amazontoken = 1000;
		jsmn_parser parser;
	//	jsmntok_t tokens[FACEMAXTOKEN]; //TODO
		jsmntok_t tokens[amazontoken]; //TODO
		char* buf = NULL;

		if(buf == NULL)
		{
			err("no mem");
			free(curlretbuf); curlretbuf = NULL;
			curlretbufsize = 0;
			return 1;
		}
	
		jsmn_init(&parser);
	
	//	ret = jsmn_parse(&parser, curlretbuf, tokens, FACEMAXTOKEN);
		int ret1 = jsmn_parse(&parser, curlretbuf, tokens, amazontoken);
		if(ret1 == JSMN_SUCCESS)
		{
			for(i = 0; i < amazontoken; i++)
			{
				if(tokens[i].start == -1) break;
	
				len = tokens[i].end - tokens[i].start;
				if(len > MINMALLOC) len = MINMALLOC;
				char* ptr = curlretbuf + tokens[i].start;
	
				printf("tokens[i].type=%d ptr=%s buf=%s\n", tokens[i].type, ptr, buf);
			}
		}
*/
///////
	}

	printf("amazon_search before free url\n");
	free(url), url = NULL;
	printf("amazon_search before free page\n");
	free(page), page = NULL;
	printf("amazon_search before free pages\n");
	free(pages), pages = NULL;
	printf("amazon_search before free nextpage\n");
	free(nextpage), nextpage = NULL;
	printf("amazon_search before free filename\n");
	free(filename), filename = NULL;
	printf("amazon_search before free runtime\n");
	free(runtime), runtime = NULL;
	printf("amazon_search before free year\n");
	free(year), year = NULL;
	printf("amazon_search before free atitle\n");
	free(atitle), atitle = NULL;
	printf("amazon_search before free pic\n");
	free(pic), pic = NULL;
	printf("amazon_search before free streamurl\n");
	free(streamurl), streamurl = NULL;
	printf("amazon_search before free id\n");
	free(id), id = NULL;
	printf("amazon_search before free tmpstr\n");
	free(tmpstr), tmpstr = NULL;
	printf("amazon_search before free tmpstr2\n");
	free(tmpstr2), tmpstr2 = NULL;
	printf("amazon_search before free tmpstr3\n");
	free(tmpstr3), tmpstr3 = NULL;
	printf("amazon_search before free line\n");
	free(line), line = NULL;
	printf("amazon_search before free search\n");
	free(search), search = NULL;
	printf("amazon_search before free menu\n");
	free(menu), menu = NULL;

	printf("amazon_search end flag: %d\n", flag);

	return ret;
}

void amazon_deinit()
{
	printf("amazon_deinit start\n");

	int debuglevel = getconfigint("debuglevel", NULL);
	char* tmpstr = NULL;

	unlink("/tmp/amazon_tmpstr_get0_logout");

	tmpstr = gethttps("https://www.amazon.de/ap/signin?_encoding=UTF8&openid.assoc_handle=deflex&openid.claimed_id=http%3A%2F%2Fspecs.openid.net%2Fauth%2F2.0%2Fidentifier_select&openid.identity=http%3A%2F%2Fspecs.openid.net%2Fauth%2F2.0%2Fidentifier_select&openid.mode=checkid_setup&openid.ns=http%3A%2F%2Fspecs.openid.net%2Fauth%2F2.0&openid.ns.pape=http%3A%2F%2Fspecs.openid.net%2Fextensions%2Fpape%2F1.0&openid.pape.max_auth_age=0&openid.return_to=https%3A%2F%2Fwww.amazon.de%2Fgp%2Fyourstore%2Fhome%3Fie%3DUTF8%26action%3Dsign-out%26path%3D%252Fgp%252Fyourstore%252Fhome%26ref_%3Dnav__gno_signout%26signIn%3D1%26useRedirectOnSuccess%3D1", NULL, NULL, NULL, NULL, NULL, 1);
	titheklog(debuglevel, "/tmp/amazon_tmpstr_get0_logout", NULL, NULL, NULL, tmpstr);	
	free(tmpstr), tmpstr = NULL;
	printf("amazon_deinit end\n");
}

char* amazon(char* link)
{
	printf("amazon start link: %s\n", link);

	int debuglevel = getconfigint("debuglevel", NULL);
	char* url = NULL, *customerid = NULL, *marketplaceid = NULL, *matchtoken = NULL, *devicetypeid = NULL, *apimain = NULL, *token = NULL;
	char* tmpstr = NULL, *title = NULL, *pic = NULL, *streamurl = NULL, *bitrate = NULL, *random = NULL, *lang = NULL, *langtext = NULL, *tmpstr2 = NULL;
	
	unlink("/tmp/amazon_streamurl_get1");
	unlink("/tmp/amazon_streamurl_get2");
	unlink("/tmp/amazon_streamurl_get3");
	unlink("/tmp/amazon_streamurl_get4");

	url = ostrcat("http://www.amazon.de/gp/video/detail/", link, 0, 0);
	url = ostrcat(url, "?ie=UTF8&*Version*=1&*entries*=0", 1, 0);

	url = ostrcat("http://www.amazon.de/dp/", link, 0, 0);
	url = ostrcat(url, "/?_encoding=UTF8", 1, 0);

//////////////////////////////////////////////////
	tmpstr = gethttps(url, NULL, NULL, NULL, NULL, NULL, 1);
	titheklog(debuglevel, "/var/usr/local/share/titan/plugins/player/tithek/amazon_streamurl_get1", NULL, NULL, NULL, tmpstr);	
	free(url), url = NULL;
//////////////////////////////////////////////////	

	tmpstr2 = string_resub("class=\"dv-toggle-box dv-tb-closed\">", "<span id=\"dv-mta-submit-announce\"", tmpstr, 0);
	tmpstr2 = string_replace_all("\n", " ", tmpstr2, 1);
	tmpstr2 = string_replace_all("<option value=\"", "\n<option value=\"", tmpstr2, 1);

	int count = 0, i = 0;	
	struct splitstr* ret1 = NULL;
	struct menulist* mlist = NULL, *mbox = NULL;
	ret1 = strsplit(tmpstr2, "\n", &count);
	for(i = 0; i < count; i++)
	{
		printf("%d round %s\n", i, ret1[i].part);

		if(ostrstr(ret1[i].part, "<option value=\"") != NULL)
		{ 
			lang = oregex(".*<option value=\"(.*)\".*>", ret1[i].part);
			langtext = oregex(".*>(.*)</option>", ret1[i].part);
			strstrip(lang);
			strstrip(langtext);
			lang = stringreplacecharonce(lang, '\"', '\0');
			pic = ostrcat(lang, NULL, 0, 0);
			pic = stringreplacecharonce(pic, '_', '\0');
			if(pic != NULL)
				pic[2] = '\0'; // strip string from deu > de or eng > en
			pic = ostrcat(pic, ".png", 1, 0);

			if(ostrstr(lang, "_") != NULL && langtext != NULL)
			{
				debug(99, "(%d) langtext: %s lang: %s\n", i, langtext, lang);																									
				addmenulist(&mlist, langtext, lang, pic, 0, 0);
			}
			free(lang), lang = NULL;
			free(langtext), langtext = NULL;
			free(pic), pic = NULL;
		}
	}
	free(ret1), ret1 = NULL;
	free(tmpstr2), tmpstr2 = NULL;

	if(mlist != NULL)
	{
		mbox = menulistbox(mlist, NULL, _("Select Language"), _("Choose your Language from the following list"), NULL, NULL, 1, 0);
		if(mbox != NULL)
		{

			debug(99, "mbox->name %s", mbox->name);
			debug(99, "mbox->text %s", mbox->text);
			lang = ostrcat(mbox->text, NULL, 0, 0);
		}
	}
	freemenulist(mlist, 0); mlist = NULL;

//////////////////////////////////////////////////

	customerid = string_resub("\"customerID\":\"", "\"", tmpstr, 0);
	marketplaceid = string_resub("\"marketplaceID\":\"", "\"", tmpstr, 0);
	matchtoken = string_resub("\"csrfToken\":\"", "\"", tmpstr, 0);
	devicetypeid = ostrcat("A35LWR0L7KC0TJ", NULL, 0, 0);
	free(tmpstr), tmpstr = NULL;

//	apiMain = ["atv-ps", "atv-ps-eu", "atv-ps-eu"][int(siteVersion)]
	apimain = ostrcat("atv-ps-eu", NULL, 0, 0);

	url = ostrcat("https://", apimain, 0, 0);
	url = ostrcat(url, ".amazon.com/cdp/catalog/GetASINDetails?version=2&format=json&asinlist=", 1, 0);
	url = ostrcat(url, link, 1, 0);
	url = ostrcat(url, "&deviceID=", 1, 0);
	url = ostrcat(url, customerid, 1, 0);
	url = ostrcat(url, "&includeRestrictions=true&deviceTypeID=", 1, 0);
	url = ostrcat(url, devicetypeid, 1, 0);
	url = ostrcat(url, "&firmware=WIN%2017,0,0,188%20PlugIn&NumberOfResults=1", 1, 0);

	printf("customerid: %s\n", customerid);
	printf("marketplaceid: %s\n", marketplaceid);
	printf("matchtoken: %s\n", matchtoken);
	printf("devicetypeid: %s\n", devicetypeid);
	printf("apimain: %s\n", apimain);
	printf("lang: %s\n", lang);

//////////////////////////////////////////////////	
	tmpstr = gethttps(url, NULL, NULL, NULL, NULL, NULL, 1);
	titheklog(debuglevel, "/var/usr/local/share/titan/plugins/player/tithek/amazon_streamurl_get2", NULL, NULL, NULL, tmpstr);	
	free(url), url = NULL;
//////////////////////////////////////////////////	

	random = randomstring(18);
	url = ostrcat("https://www.amazon.de/gp/video/streaming/player-token.json?callback=jQuery1640", random, 0, 0);
	url = ostrcat(url, "_", 1, 0);
	url = ostrcat(url, olutoa(time(NULL)*1000), 1, 1);
	url = ostrcat(url, "&csrftoken=", 1, 0);
	url = ostrcat(url, matchtoken, 1, 0);
	url = ostrcat(url, "_", 1, 0);
	url = ostrcat(url, olutoa(time(NULL)*1000), 1, 1);

	free(tmpstr), tmpstr = NULL;

//////////////////////////////////////////////////	
	tmpstr = gethttps(url, NULL, NULL, NULL, NULL, NULL, 1);
	titheklog(debuglevel, "/var/usr/local/share/titan/plugins/player/tithek/amazon_streamurl_get3", NULL, NULL, NULL, tmpstr);	
	free(url), url = NULL;
//////////////////////////////////////////////////	

	token = string_resub("\"token\":\"", "\"", tmpstr, 0);
	free(tmpstr), tmpstr = NULL;

	debug(99, "token: %s", token);

	url = ostrcat("https://", apimain, 0, 0);
	if(amazonlogin == 1)
		url = ostrcat(url, ".amazon.com/cdp/catalog/GetStreamingUrlSets?version=1&format=json&firmware=WIN%2011,7,700,224%20PlugIn", 1, 0);
	else // trailer
		url = ostrcat(url, ".amazon.com/cdp/catalog/GetStreamingTrailerUrls?version=1&format=json&firmware=WIN%2011,7,700,224%20PlugIn", 1, 0);
	if(lang != NULL)
	{
		url = ostrcat(url, "&audioTrackId=", 1, 0);
		url = ostrcat(url, lang, 1, 0);
	}
	url = ostrcat(url, "&marketplaceID=", 1, 0);
	url = ostrcat(url, marketplaceid, 1, 0);
	url = ostrcat(url, "&token=", 1, 0);
	url = ostrcat(url, token, 1, 0);
	url = ostrcat(url, "&deviceTypeID=", 1, 0);
	url = ostrcat(url, devicetypeid, 1, 0);
	url = ostrcat(url, "&asin=", 1, 0);
	url = ostrcat(url, link, 1, 0);
	url = ostrcat(url, "&customerID=", 1, 0);
	url = ostrcat(url, customerid, 1, 0);
	url = ostrcat(url, "&deviceID=", 1, 0);
	url = ostrcat(url, customerid, 1, 0);
	url = ostrcat(url, olutoa(time(NULL)*1000), 1, 1);
	url = ostrcat(url, link, 1, 0);

//////////////////////////////////////////////////	
	tmpstr = gethttps(url, NULL, NULL, NULL, NULL, NULL, 1);
	titheklog(debuglevel, "/tmp/amazon_streamurl_get4", NULL, NULL, NULL, tmpstr);	
	free(url), url = NULL;
//////////////////////////////////////////////////	

	if(tmpstr == NULL || ostrstr(tmpstr, "\"statusCode\":\"ERROR\"") != NULL)
	{
		tmpstr = string_resub("\"message\":\"", "\"", tmpstr, 0);
		textbox(_("Message"), tmpstr , _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 1200, 200, 0, 0);
		goto end;
	}

	tmpstr = string_replace_all("{\"bitrate\":", "\n{\"bitrate\":", tmpstr, 1);

	count = 0;
	i = 0;
	ret1 = strsplit(tmpstr, "\n", &count);
	for(i = 0; i < count; i++)
	{
		free(streamurl), streamurl = NULL;

		if(ostrstr(ret1[i].part, "f4m") != NULL)
			pic = ostrcat("f4m.png", NULL, 0, 0);
		else if(ostrstr(ret1[i].part, "mp4") != NULL)
			pic = ostrcat("mp4.png", NULL, 0, 0);
		else if(ostrstr(ret1[i].part, "3gp") != NULL)
			pic = ostrcat("3gp.png", NULL, 0, 0);
		else if(ostrstr(ret1[i].part, "flv") != NULL)
			pic = ostrcat("flv.png", NULL, 0, 0);
		bitrate = string_resub("\"bitrate\":", ",", ret1[i].part, 0);
		streamurl = string_resub("\"url\":\"", "\"", ret1[i].part, 0);
		streamurl = string_replace("rtmpe://azeufms-vodfs.fplive.net/azeufms-vod/mp4:", "http://azeufms-vodfs.fplive.net/", streamurl, 1);

		if(ostrstr(streamurl, "http://") != NULL)
		{
			title = ostrcat("Http Stream (Bitrate ", NULL, 0, 0);
			title = ostrcat(title, bitrate, 1, 0);
			title = ostrcat(title, ")", 1, 0);
			debug(99, "(%d) title: %s streamurl: %s\n", i, title, streamurl);																									
			addmenulist(&mlist, title, streamurl, pic, 0, 0);
		}
		free(bitrate), bitrate = NULL;
		free(title), title = NULL;
		free(pic), pic = NULL;
	}
	free(ret1), ret1 = NULL;

	if(mlist != NULL)
	{
		mbox = menulistbox(mlist, NULL, _("Stream Menu"), _("Choose your Streaming Format from the following list"), NULL, NULL, 1, 0);
		if(mbox != NULL)
		{
			free(streamurl), streamurl = NULL;
			debug(99, "mbox->name %s", mbox->name);
			debug(99, "mbox->text %s", mbox->text);
			streamurl = ostrcat(mbox->text, NULL, 0, 0);
		}
	}
	freemenulist(mlist, 0); mlist = NULL;
end:
	printf("amazon before free lang\n");
	free(lang), lang = NULL;
	printf("amazon before free random\n");
	free(random), random = NULL;
	printf("amazon before free url\n");
	free(url), url = NULL;
	printf("amazon before free customerid\n");
	free(customerid), customerid = NULL;
	printf("amazon before free marketplaceid\n");
	free(marketplaceid), marketplaceid = NULL;
	printf("amazon before free matchtoken\n");
	free(matchtoken), matchtoken = NULL;
	printf("amazon before free devicetypeid\n");
	free(devicetypeid), devicetypeid = NULL;
	printf("amazon before free apimain\n");
	free(apimain), apimain = NULL;
	printf("amazon before free token\n");
	free(token), token = NULL;
	printf("amazon before free tmpstr\n");
	free(tmpstr), tmpstr = NULL;
	printf("amazon before free title\n");
	free(title), title = NULL;
	printf("amazon before free pic\n");
	free(pic), pic = NULL;
	printf("amazon before free bitrate\n");
	free(bitrate), bitrate = NULL;

	debug(99, "streamurl: %s", streamurl);
	printf("amazon end link: %s\n", link);

	return streamurl;
}

int amazon_search_local(struct skin* grid, struct skin* listbox, struct skin* countlabel, struct skin* load, char* link, char* title, char* searchstr, int flag)
{
	char* tmpstr = NULL, *tmpstr1 = NULL, *line = NULL, *menu = NULL, *search = NULL;
	int ret = 1, count = 0, i = 0;

	if(listbox == NULL || listbox->select == NULL || listbox->select->handle == NULL)
		return ret;

	if(searchstr == NULL)
		search = textinputhist(_("Search"), " ", "searchhist");
	else
		search = textinputhist(_("Search"), searchstr, "searchhist");

	if(search != NULL)
	{
		drawscreen(load, 0, 0);

		strstrip(search);
		string_tolower(search);

		tmpstr = gethttp("openaaf.dyndns.tv", "/mediathek/amazon/streams/amazon.all-sorted.list", 80, NULL, HTTPAUTH, 5000, NULL, 0);

		struct splitstr* ret1 = NULL;
		ret1 = strsplit(tmpstr, "\n", &count);

		if(ret1 != NULL)
		{
			int max = count;
			for(i = 0; i < max; i++)
			{
			
				tmpstr1 = ostrcat(ret1[i].part, NULL, 0, 0);
				tmpstr1 = stringreplacecharonce(tmpstr1, '#', '\0');
				string_tolower(tmpstr1);

				if(ostrstr(tmpstr1, search) != NULL)
				{
					printf("found: %s\n", ret1[i].part);
					int rcret = waitrc(NULL, 10, 0);
					if(rcret == getrcconfigint("rcexit", NULL)) break;

					line = ostrcat(line, ret1[i].part, 1, 0);
					line = ostrcat(line, "\n", 0, 0);
				}
				free(tmpstr1), tmpstr1 = NULL;				
			}
			free(ret1), ret1 = NULL;

			if(line != NULL)
			{
				line = string_replace_all("http://openaaf.dyndns.tv/", "http://imageshack.us/md/up/grd/", line, 1);
				menu = ostrcat("/tmp/tithek/amazon.search.list", NULL, 0, 0);
				writesys(menu, line, 0);
				struct tithek* tnode = (struct tithek*)listbox->select->handle;
				createtithek(tnode, tnode->title, menu, tnode->pic, tnode->localname, tnode->menutitle, tnode->flag);
				ret = 0;
			}
		}
		free(tmpstr), tmpstr = NULL;
	}
	free(search), search = NULL;

	return ret;
}

int amazon_login()
{
	printf("amazon_login start\n");

	int ret = 0;
	int debuglevel = getconfigint("debuglevel", NULL);

	char* hash = NULL, *tmpstr = NULL, *login = NULL;

	char* user = getconfig("amazon_user", NULL);
	char* pass = getconfig("amazon_pass", NULL);

	unlink("/tmp/amazon_tmpstr_get1_blank");
	unlink("/tmp/amazon_tmpstr_get2_sign-in");
	unlink("/tmp/amazon_tmpstr_post1");
	unlink("/tmp/amazon_tmpstr_get3_blank");
	unlink("/tmp/amazon_tmpstr_get4_blank");

	tmpstr = gethttps("https://www.amazon.de/", NULL, NULL, NULL, NULL, NULL, 1);
//	debug(99, "tmpstr: %s", tmpstr);
	titheklog(debuglevel, "/tmp/amazon_tmpstr_get1_blank", NULL, NULL, NULL, tmpstr);	

	login = string_resub("'config.signOutText',", ");", tmpstr, 0);
	free(tmpstr), tmpstr = NULL;
	debug(99, "login: %s", login);
	strstrip(login);
	debug(99, "login: %s", login);

	if(login == NULL || ostrcmp("null", login) == 0)
	{
		debug(99, "Amazon Login Error");
		ret = 0;
	}
	else
	{
		debug(99, "Amazon Login Successful: %s", login);
		ret = 1;
	}
	free(login), login = NULL;

	if(ret == 0)
	{
		tmpstr = gethttps("https://www.amazon.de/gp/sign-in.html", NULL, NULL, NULL, NULL, NULL, 1);
		titheklog(debuglevel, "/tmp/amazon_tmpstr_get2_sign-in", NULL, NULL, NULL, tmpstr);	

		char* pos1 = ostrstr(tmpstr, "<input type=\"hidden\" name=\"appActionToken\" value=");
		char* tmp1 = getxmlentry(pos1, "value=");
		debug(99, "appActionToken: %s", tmp1);
	
		char* pos2 = ostrstr(tmpstr, "<input type=\"hidden\" name=\"appAction\" value=");
		char* tmp2 = getxmlentry(pos2, "value=");
		debug(99, "appActionToken: %s", tmp2);
	
		char* pos3 = ostrstr(tmpstr, "<input type=\"hidden\" name=\"openid.pape.max_auth_age\" value=");
		char* tmp3 = getxmlentry(pos3, "value=");
		debug(99, "openid.pape.max_auth_age: %s", tmp3);
	
		char* pos4 = ostrstr(tmpstr, "<input type=\"hidden\" name=\"openid.return_to\" value=");
		char* tmp4 = getxmlentry(pos4, "value=");
		debug(99, "openid.return_to: %s", tmp4);
	
		char* pos5 = ostrstr(tmpstr, "<input type=\"hidden\" name=\"prevRID\" value=");
		char* tmp5 = getxmlentry(pos5, "value=");
		debug(99, "prevRID: %s", tmp5);
	
		char* pos6 = ostrstr(tmpstr, "<input type=\"hidden\" name=\"openid.identity\" value=");
		char* tmp6 = getxmlentry(pos6, "value=");
		debug(99, "openid.identity: %s", tmp6);
	
		char* pos7 = ostrstr(tmpstr, "<input type=\"hidden\" name=\"openid.assoc_handle\" value=");
		char* tmp7 = getxmlentry(pos7, "value=");
		debug(99, "openid.assoc_handle: %s", tmp7);
	
		char* pos8 = ostrstr(tmpstr, "<input type=\"hidden\" name=\"openid.mode\" value=");
		char* tmp8 = getxmlentry(pos8, "value=");
		debug(99, "openid.mode: %s", tmp8);
	
		char* pos9 = ostrstr(tmpstr, "<input type=\"hidden\" name=\"openid.ns.pape\" value=");
		char* tmp9 = getxmlentry(pos9, "value=");
		debug(99, "openid.ns.pape: %s", tmp9);
	
		char* pos10 = ostrstr(tmpstr, "<input type=\"hidden\" name=\"openid.claimed_id\" value=");
		char* tmp10 = getxmlentry(pos10, "value=");
		debug(99, "openid.claimed_id: %s", tmp10);
	
		char* pos11 = ostrstr(tmpstr, "<input type=\"hidden\" name=\"pageId\" value=");
		char* tmp11 = getxmlentry(pos11, "value=");
		debug(99, "pageId: %s", tmp11);
	
		char* pos12 = ostrstr(tmpstr, "<input type=\"hidden\" name=\"openid.ns\" value=");
		char* tmp12 = getxmlentry(pos12, "value=");
		debug(99, "openid.ns: %s", tmp12);
		free(tmpstr), tmpstr = NULL;

		hash = ostrcat(hash, "appActionToken=", 1, 0);
		hash = ostrcat(hash, tmp1, 1, 0);
		hash = ostrcat(hash, "&appAction=", 1, 0);
		hash = ostrcat(hash, tmp2, 1, 0);
		hash = ostrcat(hash, "&openid.pape.max_auth_age=", 1, 0);
		hash = ostrcat(hash, tmp3, 1, 0);
		hash = ostrcat(hash, "&openid.return_to=", 1, 0);
		hash = ostrcat(hash, tmp4, 1, 0);
		hash = ostrcat(hash, "&prevRID=", 1, 0);
		hash = ostrcat(hash, tmp5, 1, 0);
		hash = ostrcat(hash, "&openid.identity=", 1, 0);
		hash = ostrcat(hash, tmp6, 1, 0);
		hash = ostrcat(hash, "&openid.assoc_handle=", 1, 0);
		hash = ostrcat(hash, tmp7, 1, 0);
		hash = ostrcat(hash, "&openid.mode=", 1, 0);
		hash = ostrcat(hash, tmp8, 1, 0);
		hash = ostrcat(hash, "&openid.ns.pape=", 1, 0);
		hash = ostrcat(hash, tmp9, 1, 0);
		hash = ostrcat(hash, "&openid.claimed_id=", 1, 0);
		hash = ostrcat(hash, tmp10, 1, 0);
		hash = ostrcat(hash, "&pageId=", 1, 0);
		hash = ostrcat(hash, tmp11, 1, 0);
		hash = ostrcat(hash, "&openid.ns=", 1, 0);
		hash = ostrcat(hash, tmp12, 1, 0);
		hash = ostrcat(hash, "&email=", 1, 0);
		hash = ostrcat(hash, user, 1, 0);
		hash = ostrcat(hash, "&create=0", 1, 0);
		hash = ostrcat(hash, "&password=", 1, 0);
		hash = ostrcat(hash, pass, 1, 0);
	
		tmpstr = gethttps("https://www.amazon.de/ap/signin", NULL, hash, NULL, NULL, NULL, 1);
//		debug(99, "ret=%s", tmpstr);
		titheklog(debuglevel, "/tmp/amazon_tmpstr_post1", NULL, NULL, NULL, tmpstr);	
		free(tmpstr), tmpstr = NULL;

		tmpstr = gethttps("https://www.amazon.de/", NULL, NULL, NULL, NULL, NULL, 1);
//		debug(99, "tmpstr: %s", tmpstr);
		titheklog(debuglevel, "/tmp/amazon_tmpstr_get3_blank", NULL, NULL, NULL, tmpstr);	
		free(tmpstr), tmpstr = NULL;

		tmpstr = gethttps("https://www.amazon.de/", NULL, NULL, NULL, NULL, NULL, 1);
//		debug(99, "tmpstr: %s", curlretbuf);
		titheklog(debuglevel, "/tmp/amazon_tmpstr_get4_blank", NULL, NULL, NULL, tmpstr);	
		free(tmpstr), tmpstr = NULL;
	
		login = string_resub("'config.signOutText',", ");", tmpstr, 0);	
		debug(99, "login: %s", login);
		strstrip(login);
		debug(99, "login: %s", login);
		free(tmpstr), tmpstr = NULL;
	
		if(login == NULL || ostrcmp("null", login) == 0)
		{
			debug(99, "Amazon Login Error");
			ret = 0;
		}
		else
		{
			debug(99, "Amazon Login Successful: %s", login);
			ret = 1;
		}

		printf("amazon_login before free tmp1\n");
		free(tmp1), tmp1 = NULL;
		printf("amazon_login before free tmp2\n");
		free(tmp2), tmp2 = NULL;
		printf("amazon_login before free tmp3\n");
		free(tmp3), tmp3 = NULL;
		printf("amazon_login before free tmp4\n");
		free(tmp4), tmp4 = NULL;
		printf("amazon_login before free tmp5\n");
		free(tmp5), tmp5 = NULL;
		printf("amazon_login before free tmp6\n");
		free(tmp6), tmp6 = NULL;
		printf("amazon_login before free tmp7\n");
		free(tmp7), tmp7 = NULL;
		printf("amazon_login before free tmp8\n");
		free(tmp8), tmp8 = NULL;
		printf("amazon_login before free tmp9\n");
		free(tmp9), tmp9 = NULL;
		printf("amazon_login before free tmp10\n");
		free(tmp10), tmp10 = NULL;
		printf("amazon_login before free tmp11\n");
		free(tmp11), tmp11 = NULL;
		printf("amazon_login before free tmp12\n");
		free(tmp12), tmp12 = NULL;
	}

	printf("amazon_login end\n");

	return ret;
}

#endif
