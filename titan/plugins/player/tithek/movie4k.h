#ifndef MOVIE4K_H
#define MOVIE4K_H

// flag 1 = putlocker/sockshare
// flag 2 = filenuke

char* movie4k(char* link)
{
	debug(99, "link: %s", link);
	int debuglevel = getconfigint("debuglevel", NULL);
	char* tmpstr = NULL, *tmpstr1 = NULL, *tmpstr2 = NULL, *url = NULL, *streamurl = NULL, *tmppath = NULL, *tmphost = NULL, *pos = NULL;
	titheklog(debuglevel, "/tmp/movie4k_streamurl_url0", NULL, NULL, NULL, link);

	if(link == NULL || ostrncmp("http://", link, 7)) return NULL;

	tmphost = string_replace("http://", "", (char*)link, 0);

	if(tmphost != NULL)
		pos = strchr(tmphost, '/');
	if(pos != NULL)
	{
		pos[0] = '\0';
		tmppath = pos + 1;
	}

//	tmpstr1 = gethttp(tmphost, tmppath, 80, NULL, NULL, 10000, NULL, 0);
	tmpstr1 = gethttps(link, NULL, NULL, NULL, NULL, NULL, 1);

	titheklog(debuglevel, "/tmp/movie4k_streamurl_tmpstr1_a", NULL, NULL, NULL, tmpstr1);

	tmpstr = string_resub("question.png", "underplayer", tmpstr1, 0);
	titheklog(debuglevel, "/tmp/movie4k_streamurl_tmpstr1_b", NULL, NULL, NULL, tmpstr);

	free(tmpstr1), tmpstr1 = NULL;

	if(tmpstr == NULL)
	{
		textbox(_("Message"), _("Connection Error, try again later.") , _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 1200, 200, 0, 0);
		goto end;
	}
		
	url = string_resub("<a target=\"_blank\" href=\"", "\"><", tmpstr, 0);
	titheklog(debuglevel, "/tmp/movie4k_streamurl_url1", NULL, NULL, NULL, url);

	if(url == NULL)
		url = string_resub("<iframe src=\"", "\"", tmpstr, 0);

	if(url == NULL)
		url = oregex(".*src=\"(http://.*)&.*", tmpstr);
//		url = oregex(".*src=\"(http://.*)\".*", tmpstr);

	if(url == NULL)
		url = oregex(".*src=\"(https://.*)&.*", tmpstr);

	if(url == NULL)
		url = oregex(".*src=\"(https://.*)\".*", tmpstr);

	if(url == NULL)
	{
		textbox(_("Message"), _("Can not parse Main Stream URL, try again later.") , _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 1200, 200, 0, 0);
		goto end;
	}
	
	if(ostrstr(url, "http") == NULL)
	{
		free(url), url = NULL;
		url = string_resub("<iframe src=", "\" width", tmpstr, 0);

		tmpstr2 = ostrcat("<iframe src=", url, 0, 0);
		tmpstr2 = ostrcat(tmpstr2, "\" width", 1, 0);
		tmpstr = string_replace_all(tmpstr2, "", tmpstr, 1);

		titheklog(debuglevel, "/tmp/movie4k_streamurl_tmpstr2", NULL, NULL, NULL, tmpstr);
		titheklog(debuglevel, "/tmp/movie4k_streamurl_url2", NULL, NULL, NULL, url);
	}
	
	if(ostrstr(url, "http://www.facebook.com") != NULL)
	{
		free(url), url = NULL;
		url = oregex(".*<iframe width=.*(http://.*)&width.*", tmpstr);
		titheklog(debuglevel, "/tmp/movie4k_streamurl_url3", NULL, NULL, NULL, url);
	}	

	if(ostrstr(url, "http://clkrev.com") != NULL)
	{
		free(url), url = NULL;
		url = string_resub("<iframe src=\"", "\" width", tmpstr, 0);
		titheklog(debuglevel, "/tmp/movie4k_streamurl_url4", NULL, NULL, NULL, url);
	}
	
	url = string_replace_all("/embed/", "/file/", url, 1);
	url = string_replace_all("\"", "", url, 1);

	if(url == NULL)
	{
		free(url), url = NULL;
		url = oregex(".*<iframe width=\".*(http://.*)&width=600&height=480\".*", tmpstr);
		titheklog(debuglevel, "/tmp/movie4k_streamurl_url5", NULL, NULL, NULL, url);
	}
	titheklog(debuglevel, "/tmp/movie4k_streamurl_url6", NULL, NULL, NULL, url);

	if(url == NULL)
	{
		textbox(_("Message"), _("Can not parse Stream URL, try again later.") , _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 1200, 200, 0, 0);
		goto end;
	}

	streamurl = hoster(url);
	titheklog(debuglevel, "/tmp/movie4k_streamurl_url7", NULL, NULL, NULL, url);
end:
	free(url), url = NULL;
	free(tmpstr), tmpstr = NULL;
	free(tmpstr2), tmpstr2 = NULL;
	free(tmphost), tmphost = NULL;

	return streamurl;
}

int movie4k_search(struct skin* grid, struct skin* listbox, struct skin* countlabel, struct skin* load, char* link, char* title, char* searchstr, int flag)
{
	int ret = 1, maxlen = 0, bigpos = 0;
	int incount = 0;
	char* tmpstr = NULL;
	char* from = NULL;
	char* folgen = NULL;
	char* folgentmp = NULL;
	char* name = NULL;
	char* lang = NULL;
	char* season = NULL;
	char* episode = NULL;
	char* line = NULL;
	char* menu = NULL;
	char* str = NULL;
	char* pic = NULL;
	char* type = NULL;

	int debuglevel = getconfigint("debuglevel", NULL);

	if(listbox == NULL || listbox->select == NULL || listbox->select->handle == NULL)
		return ret;

	char* search = NULL;
	if(flag == 0)
	{
		if(searchstr == NULL)
			search = textinputhist(_("Search"), " ", "searchhist");
		else
			search = textinputhist(_("Search"), searchstr, "searchhist");
	}
	else
		search = ostrcat(link, NULL, 0, 0);

	if(search != NULL)
	{
		drawscreen(load, 0, 0);
		search = strstrip(search);
		search = stringreplacechar(search, ' ', '+');
		debug(99, "search: %s", search);

		char* send = NULL;
		send = ostrcat(send, "GET /searchAutoCompleteNew.php?search=the HTTP/1.1\r\n", 1, 0);
		send = ostrcat(send, "Accept-Encoding: identity\r\nAccept-Charset: ISO-8859-1,utf-8;q=0.7,*;q=0.7\r\n", 1, 0);
		send = ostrcat(send, "Host: movie4k.to\r\nAccept-Language: de-DE,de;q=0.8,en-US;q=0.6,en;q=0.4\r\n", 1, 0);
		send = ostrcat(send, "Connection: close\r\nUser-Agent: Mozilla/5.0 (Windows; U; Windows NT 5.1; de-DE; rv:1.9.0.3) Gecko/2008092417 Firefox/3.0.3\r\n\r\n", 1, 0);
		debug(99, "send: %s", send);

		tmpstr = gethttpreal("www.movie4k.to", "/searchAutoCompleteNew.php?search=the", 80, NULL, NULL, NULL, 0, send, NULL, 5000, 1);
		free(send); send = NULL;
		debug(99, "tmpstr: %s", tmpstr);
		titheklog(debuglevel, "/tmp/movie4k_search", NULL, NULL, NULL, tmpstr);

		char* key = string_resub("securekey=", "&search", tmpstr, 0);
		debug(99, "key: %s", key);

		send = ostrcat(send, "GET /movies.php?list=search&securekey=", 1, 0);
		send = ostrcat(send, key, 1, 0);
		send = ostrcat(send, "&search=", 1, 0);
		send = ostrcat(send, search, 1, 0);
		send = ostrcat(send, " HTTP/1.1\r\nAccept-Encoding: identity\r\n", 1, 0);
		send = ostrcat(send, "Accept-Language: de-DE,de;q=0.8,en-US;q=0.6,en;q=0.4\r\n", 1, 0);
		send = ostrcat(send, "Host: movie4k.to\r\n", 1, 0);
		send = ostrcat(send, "User-Agent: Mozilla/5.0 (Windows; U; Windows NT 5.1; de-DE; rv:1.9.0.3) Gecko/2008092417 Firefox/3.0.3\r\n", 1, 0);
		send = ostrcat(send, "Accept-Charset: ISO-8859-1,utf-8;q=0.7,*;q=0.7\r\n", 1, 0);
		send = ostrcat(send, "Connection: close\r\nCookie: xxx2=ok;\r\n\r\n", 1, 0);
		debug(99, "send: %s", send);

		free(tmpstr), tmpstr = NULL;
		tmpstr = gethttpreal("movie4k.to", "/", 80, NULL, NULL, NULL, 0, send, NULL, 5000, 1);

		folgen = string_resub("<TABLE id=\"tablemoviesindex\">", "</TABLE>", tmpstr, 0);
		folgen = string_replace_all("1000\r\n", "", folgen, 1);
		folgen = string_replace_all("2000\r\n", "", folgen, 1);
		stringreplacechar(folgen, '\n', ' ');
		stringreplacechar(folgen, '\t', ' ');
		string_strip_whitechars(folgen);
		folgen = string_replace_all("</TD> </TR> <TR id=\"coverPreview", "</TD> </TR>\n<TR id=\"cover1Preview", folgen, 1);
		if(folgen != NULL)
		{
			int count = 0;
			int j;
			struct splitstr* ret1 = NULL;
			ret1 = strsplit(folgen, "\n", &count);

			if(ret1 != NULL && count > 0)
			{
				int max = count;
				for(j = 0; j < max; j++)
				{
					int rcret = waitrc(NULL, 10, 0);
					if(rcret == getrcconfigint("rcexit", NULL)) break;

					incount++;
					link = string_resub("<a href=\"", "\">", ret1[j].part, 0);
					name = string_resub(".html\">", "</a>", ret1[j].part, 0);
					char* id = string_resub("online-film-", ".html", link, 0);
					if(id == NULL)
						id = string_resub("watch-movie-", ".html", link, 0);
					if(id == NULL)
						id = oregex(".*tvshows-(.*[0-9]{4,10})-.*", link);


					from = ostrcat("#coverPreview", id, 0, 0);
					pic = string_resub(from, from, tmpstr, 0);	
					pic = string_resub("<img src='", "' alt=", pic, 1);	
					if(pic == NULL)
						pic = ostrcat("http://openaaf.dyndns.tv/mediathek/menu/default.jpg", NULL, 0, 0);

					if(ostrstr(link, "online-serie-") != NULL)
						type = ostrcat("40", NULL, 0, 0);
					else
						type = ostrcat("34", NULL, 0, 0);


					if(ostrstr(ret1[j].part, "us_ger_small.png") != NULL)
						lang = ostrcat(" (de)", NULL, 0, 0);
					else if(ostrstr(ret1[j].part, "us_flag_small.png") != NULL)
						lang = ostrcat(" (en)", NULL, 0, 0);
					else
						lang = ostrcat(" (\?\?)", NULL, 0, 0);
					
					ostrcatbig(&line, name, &maxlen, &bigpos);
					ostrcatbig(&line, lang, &maxlen, &bigpos);
					ostrcatbig(&line, "#http://movie4k.to/", &maxlen, &bigpos);
					ostrcatbig(&line, link, &maxlen, &bigpos);
					ostrcatbig(&line, "#", &maxlen, &bigpos);
					ostrcatbig(&line, pic, &maxlen, &bigpos);
					ostrcatbig(&line, "#movie4k_search_", &maxlen, &bigpos);
					char* cincount = oitoa(incount + time(NULL));
					ostrcatbig(&line, cincount, &maxlen, &bigpos);
					free(cincount); cincount = NULL;
					ostrcatbig(&line, ".jpg#Movie4k - Search#", &maxlen, &bigpos);
					ostrcatbig(&line, type, &maxlen, &bigpos);
					ostrcatbig(&line, "\n", &maxlen, &bigpos);  

					free(str), str = NULL;
											
					free(name), name = NULL;
					free(lang), lang = NULL;
					free(season), season = NULL;
					free(episode), episode = NULL;
					free(from), from = NULL;
					free(pic), pic = NULL;
					free(type), type = NULL;
					if(j == 60)
						break;
				}
			}
			free(ret1), ret1 = NULL;
		}				
		free(from), from = NULL;
		free(folgen), folgen = NULL;
		free(folgentmp), folgentmp = NULL;		

		if(line != NULL)
		{
			line = string_replace_all("http://openaaf.dyndns.tv/", "http://imageshack.us/md/up/grd/", line, 1);
			menu = ostrcat("/tmp/tithek/movie4k.search.", oitoa(time(NULL)), 0, 1);
			menu = ostrcat(menu, ".list", 1, 0);
			writesys(menu, line, 0);
			free(line); line = NULL;
			
			struct tithek* tnode = (struct tithek*)listbox->select->handle;
			createtithek(tnode, tnode->title,  menu, tnode->pic, tnode->localname, tnode->menutitle, tnode->flag);
			ret = 0;
			free(menu); menu = NULL;
		}

		free(tmpstr), tmpstr = NULL;
	}
	free(search), search = NULL;
	return ret;
}

int movie4k_search_local(struct skin* grid, struct skin* listbox, struct skin* countlabel, struct skin* load, char* link, char* title, char* searchstr, int flag)
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

		tmpstr = gethttp("openaaf.dyndns.tv", "/mediathek/movie4k/streams/movie4k.all-sorted.list", 80, NULL, HTTPAUTH, 5000, NULL, 0);

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
				menu = ostrcat("/tmp/tithek/movie4k.search.", oitoa(time(NULL)), 0, 1);
				menu = ostrcat(menu, ".list", 1, 0);
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

int movie4k_hoster(struct skin* grid, struct skin* listbox, struct skin* countlabel, struct skin* load, char* link, char* title)
{
	debug(99, "link: %s", link);
	int debuglevel = getconfigint("debuglevel", NULL);
	int ret = 1, maxlen = 0, bigpos = 0;
	char* extra = NULL, *pos3 = NULL, *ip = NULL, *pos = NULL, *path = NULL, *tmpstr = NULL, *line = NULL, *url = NULL, *url2 = NULL;
	char* url3 = NULL, *url4 = NULL, *pathnew = NULL, *id = NULL, *logfile = NULL, *tmphname = NULL, *hnamein = NULL, *nolinks = NULL;
	char* tmpid = NULL, *pichname = NULL;
	if(listbox == NULL || listbox->select == NULL || listbox->select->handle == NULL)
		return ret;
		
	drawscreen(load, 0, 0);

	ip = string_replace("http://", "", (char*)link, 0);

	if(ip != NULL)
		pos = strchr(ip, '/');
	if(pos != NULL)
	{
		pos[0] = '\0';
		path = pos + 1;
	}

	tmpstr = gethttp(ip, path, 80, NULL, NULL, 10000, NULL, 0);
	titheklog(debuglevel, "/tmp/movie4k2_tmpstr", NULL, NULL, NULL, tmpstr);
	int a = 0;

	if(tmpstr != NULL)
	{	
		int countj = 0;
		char* cpart = ostrstr(tmpstr, "&part=");
		if(cpart != NULL)
		{
			if(ostrstr(cpart, "&part=4") != NULL)
				countj = 4;
			else if(ostrstr(cpart, "&part=3") != NULL)
				countj = 3;
			else if(ostrstr(cpart, "&part=2") != NULL)
				countj = 2;
			else if(ostrstr(cpart, "&part=1") != NULL)
				countj = 1;
				
			tmpid = string_resub("movie.php?id=", "&part=", tmpstr, 0);
		}
			
		if(ostrstr(tmpstr, "links\[") == NULL)
		{
//			hnamein = string_resub("width=\"16\"> &nbsp;", "</a></td><td align=", tmpstr, 0);
			hnamein = string_resub("width=\"16\"> &nbsp;", "</a></td>", tmpstr, 0);
			nolinks = ostrcat(tmpstr, NULL, 0, 0);
			if(ostrstr(tmpstr, "set a cookie which will expire in 3 days and be accessible site wide") != NULL)
			{
				textbox(_("Message"), _("set a cookie which will expire in 3 days and be accessible site wide") , _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 1200, 200, 0, 0);
				writesys("/tmp/movie4k.error.cookie.list", tmpstr, 1);
				return ret;
			}
		}
		else
		{
			unlink("/tmp/movie4k.list");
			writesys("/tmp/movie4k.list", tmpstr, 1);
			char* tmpstr1 = command("cat /tmp/movie4k.list | grep ^links");
			tmpstr = ostrcat(tmpstr1, NULL, 1, 0);
			char* cmd = ostrcat("cat /tmp/movie4k.list | grep ^'</SCRIPT>' | sed 's!</SCRIPT>!\\nlinks[!g' | sed 's!</td></tr>!\\nlinks[!g' | grep ^'links\\[<' | grep -v ^'links\\[<SCRIPT'", NULL, 0, 0);
			printf("cmd: %s\n",cmd);
			tmpstr1 = command(cmd);
			free(cmd), cmd = NULL;
			tmpstr = ostrcat(tmpstr, tmpstr1, 1, 0);
		}

		titheklog(debuglevel, "/var/usr/local/share/titan/plugins/tithek/thefile1_tmp", NULL, NULL, NULL, tmpstr);


		int count = 0;
		int incount = 0;
		int i;
		struct splitstr* ret1 = NULL;
		ret1 = strsplit(tmpstr, "\n", &count);		

		if(ret1 != NULL && count > 0)
		{
			int max = count;
			for(i = 0; i < max; i++)
			{
printf("ret1[i].part: %s\n",ret1[i].part);

				if((!ostrncmp("links", ret1[i].part, 5) && nolinks == NULL) || nolinks != NULL)
				{
					if(nolinks == NULL)
					{
						a++;
//						ret1[i].part = string_replace_all("</a>&nbsp;</td>", "</a></td>", ret1[i].part, 1);

						pathnew = string_resub("<a href=\\\"", "\\", ret1[i].part, 0);
						if(pathnew == NULL)
							pathnew = string_resub("<a href=\"", "\"", ret1[i].part, 0);

						tmphname = string_resub("title=\\\"", " ", ret1[i].part, 0);
						if(tmphname == NULL)
							tmphname = string_resub("title=\"", " ", ret1[i].part, 0);
						
						if(tmphname == NULL)
							tmphname = string_resub("&nbsp;", "</a", ret1[i].part, 0);

						pos3 = ostrstr(ret1[i].part, "Movie quality");
						extra = getxmlentry(pos3, "quality ");

						id = oregex(".*-online-film-(.*[0-9]{4,10}).html.*", pathnew);							
						if(id == NULL)
							id = string_resub("watch-movie-", ".html", pathnew, 0);
						if(id == NULL)
							id = oregex(".*tvshows-(.*[0-9]{4,10})-.*", pathnew);
						if(id == NULL)
							id = ostrcat(tmpid, NULL, 0, 0);

						debug(99, "(%d/%d/%d) pathnew: %s hname: %s id: %s",a ,i ,max ,pathnew , tmphname, id);

						if(ostrstr(pathnew, "tvshows-") != NULL)
						{
							pathnew = string_replace("tvshows-", "", pathnew, 1);
							pathnew = string_replace(id, "", pathnew, 1);
							pathnew = string_replace("-", "", pathnew, 1);
							pathnew = string_replace(".html", "", pathnew, 1);
							pathnew = ostrcat(pathnew, "-online-serie-", 1, 0);
							pathnew = ostrcat(pathnew, id, 1, 0);
							pathnew = ostrcat(pathnew, ".html", 1, 0);
							debug(99, "(%d/%d/%d) pathnew: %s hname: %s id: %s",a ,i ,max ,pathnew, tmphname, id);
						}	

						if(ostrstr(pathnew, "tvshow-") != NULL)
						{
							pathnew = string_replace("tvshow-", "", pathnew, 1);
							pathnew = string_replace(id, "", pathnew, 1);
							pathnew = string_replace("-", "", pathnew, 1);
							pathnew = string_replace(".html", "", pathnew, 1);
							pathnew = ostrcat(pathnew, "-online-serie-", 1, 0);
							pathnew = ostrcat(pathnew, id, 1, 0);
							pathnew = ostrcat(pathnew, ".html", 1, 0);
							debug(99, "(%d/%d/%d) pathnew: %s hname: %s id: %s",a ,i ,max ,pathnew, tmphname, id);
						}

//						logfile = ostrcat("/tmp/movie4k4_pathnew1", id, 0, 0);
//						logfile = ostrcat(logfile, "_", 1, 0);
//						titheklog(debuglevel, logfile, tmphname, NULL, NULL, pathnew);
//						free(logfile), logfile= NULL;
						
						url = ostrcat(pathnew, NULL, 0, 0);
					}
					else
					{
						id = oregex(".*-online-film-(.*[0-9]{4,10}).html.*", path);							
						if(id == NULL)
							id = string_resub("watch-movie-", ".html", path, 0);
						if(id == NULL)
							id = oregex(".*tvshows-(.*[0-9]{4,10})-.*", path);
						if(id == NULL)
							id = oregex(".*tvshow-(.*[0-9]{4,10})-.*", path);
						if(id == NULL)
							id = oregex(".*tvshows-(.*[0-9]{4,10})..*", path);
						if(id == NULL)
							id = oregex(".*tvshow-(.*[0-9]{4,10})..*", path);
						if(id == NULL)
							id = ostrcat(tmpid, NULL, 0, 0);
						tmphname = ostrcat(hnamein, NULL, 0, 0);

						pos3 = ostrstr(ret1[i].part, "Movie quality");
						extra = getxmlentry(pos3, "quality ");

						pathnew = ostrcat(path, NULL, 0, 0);
						if(ostrstr(pathnew, "tvshows-") != NULL)
						{
							pathnew = string_replace("tvshows-", "", pathnew, 1);
							pathnew = string_replace(id, "", pathnew, 1);
							pathnew = string_replace("-", "", pathnew, 1);
							pathnew = string_replace(".html", "", pathnew, 1);
							pathnew = ostrcat(pathnew, "-online-serie-", 1, 0);
							pathnew = ostrcat(pathnew, id, 1, 0);
							pathnew = ostrcat(pathnew, ".html", 1, 0);
							debug(99, "(%d/%d/%d) pathnew: %s hname: %s id: %s",a ,i ,max ,pathnew, tmphname, id);
						}	

						if(ostrstr(pathnew, "tvshow-") != NULL)
						{
							pathnew = string_replace("tvshow-", "", pathnew, 1);
							pathnew = string_replace(id, "", pathnew, 1);
							pathnew = string_replace("-", "", pathnew, 1);
							pathnew = string_replace(".html", "", pathnew, 1);
							pathnew = ostrcat(pathnew, "-online-serie-", 1, 0);
							pathnew = ostrcat(pathnew, id, 1, 0);
							pathnew = ostrcat(pathnew, ".html", 1, 0);
							debug(99, "(%d/%d/%d) pathnew: %s hname: %s id: %s",a ,i ,max ,pathnew, tmphname, id);
						}
						url = ostrcat(pathnew, NULL, 0, 0);
					}	

					pichname = ostrcat(tmphname, NULL, 0, 0);
					string_tolower(pichname);
					pichname = stringreplacecharonce(pichname, '.', '\0');

					if(id != NULL)
					{
						if(countj >= 1)
						{	
							free(url), url = NULL;
							free(pathnew), pathnew = NULL;
							pathnew = ostrcat("movie.php?id=", id, 0, 0);
							pathnew = ostrcat(pathnew, "&part=1", 1, 0);
							url = ostrcat(pathnew, NULL, 0, 0);
						}
						if(countj >= 2)
						{
							free(pathnew), pathnew = NULL;
							pathnew = ostrcat("movie.php?id=", id, 0, 0);
							pathnew = ostrcat(pathnew, "&part=2", 1, 0);
							url2 = ostrcat(pathnew, NULL, 0, 0);
						}
						if(countj >= 3)
						{
							free(pathnew), pathnew = NULL;
							pathnew = ostrcat("movie.php?id=", id, 0, 0);
							pathnew = ostrcat(pathnew, "&part=3", 1, 0);
							url3 = ostrcat(pathnew, NULL, 0, 0);

						}						
						if(countj >= 4)
						{
							free(pathnew), pathnew = NULL;
							pathnew = ostrcat("movie.php?id=", id, 0, 0);
							pathnew = ostrcat(pathnew, "&part=4", 1, 0);
							url4 = ostrcat(pathnew, NULL, 0, 0);
						}
					}

					int type = 41;
					
					if(ostrcmp(url, url2) != 0)
					{
						debug(99, "(%d/%d/%d) %s (Part1) extra: %s url: %s",a ,i ,max ,tmphname, extra, url);
					}
					else
					{
						debug(99, "(%d/%d/%d) %s extra: %s url: %s",a ,i ,max ,tmphname, extra, url);
					}
					incount += 1;
					ostrcatbig(&line, tmphname, &maxlen, &bigpos);
					if(url2 != NULL && ostrcmp(url, url2) != 0)
						ostrcatbig(&line, " (Part1)", &maxlen, &bigpos);
/*
					if(extra != NULL)
					{
						ostrcatbig(&line, " (", &maxlen, &bigpos);
						ostrcatbig(&line, extra, &maxlen, &bigpos);
						ostrcatbig(&line, ")", &maxlen, &bigpos);					
					}
*/
					ostrcatbig(&line, "#http://movie4k.to/", &maxlen, &bigpos);
					ostrcatbig(&line, url, &maxlen, &bigpos);
					ostrcatbig(&line, "#http://openaaf.dyndns.tv/mediathek/menu/", &maxlen, &bigpos);
					ostrcatbig(&line, pichname, &maxlen, &bigpos);
					ostrcatbig(&line, ".jpg#movie4k_", &maxlen, &bigpos);
					ostrcatbig(&line, pichname, &maxlen, &bigpos);
					ostrcatbig(&line, ".jpg#Movie4k - ", &maxlen, &bigpos);
					ostrcatbig(&line, title, &maxlen, &bigpos);
					ostrcatbig(&line, "#", &maxlen, &bigpos);
					char* ctype = oitoa(type);
					ostrcatbig(&line, ctype, &maxlen, &bigpos);
					free(ctype); ctype = NULL;
					ostrcatbig(&line, "\n", &maxlen, &bigpos);

					if(url2 != NULL && ostrcmp(url, url2) != 0)
					{						
						int type = 41;
	
						debug(99, "-------------------------------");
						debug(99, "(%d/%d/%d) %s (Part2) extra: %s url: %s",a ,i ,max ,tmphname, extra, url2);
				
						incount += 1;
						ostrcatbig(&line, tmphname, &maxlen, &bigpos);
						ostrcatbig(&line, " (Part2)", &maxlen, &bigpos);
/*
						if(extra != NULL)
						{
							ostrcatbig(&line, " (", &maxlen, &bigpos);
							ostrcatbig(&line, extra, &maxlen, &bigpos);
							ostrcatbig(&line, ")", &maxlen, &bigpos);					
						}
*/
						ostrcatbig(&line, "#http://movie4k.to/", &maxlen, &bigpos);
						ostrcatbig(&line, url2, &maxlen, &bigpos);
						ostrcatbig(&line, "#http://openaaf.dyndns.tv/mediathek/menu/", &maxlen, &bigpos);
						ostrcatbig(&line, pichname, &maxlen, &bigpos);
						ostrcatbig(&line, ".jpg#kinox_", &maxlen, &bigpos);
						ostrcatbig(&line, pichname, &maxlen, &bigpos);
						ostrcatbig(&line, ".jpg#Movie4k - ", &maxlen, &bigpos);
						ostrcatbig(&line, title, &maxlen, &bigpos);
						ostrcatbig(&line, "#", &maxlen, &bigpos);
						char* ctype = oitoa(type);
						ostrcatbig(&line, ctype, &maxlen, &bigpos);
						free(ctype); ctype = NULL;
						ostrcatbig(&line, "\n", &maxlen, &bigpos);
					}
					if(url3 != NULL && ostrcmp(url, url3) != 0)
					{
						int type = 41;
																
						debug(99, "-------------------------------");
						debug(99, "(%d/%d/%d) %s (Part3) extra: %s url: %s",a ,i ,max ,tmphname, extra, url3);
			
						incount += 1;
						ostrcatbig(&line, tmphname, &maxlen, &bigpos);
						ostrcatbig(&line, " (Part3)", &maxlen, &bigpos);
/*
						if(extra != NULL)
						{
							ostrcatbig(&line, " (", &maxlen, &bigpos);
							ostrcatbig(&line, extra, &maxlen, &bigpos);
							ostrcatbig(&line, ")", &maxlen, &bigpos);					
						}
*/
						ostrcatbig(&line, "#http://movie4k.to/", &maxlen, &bigpos);
						ostrcatbig(&line, url3, &maxlen, &bigpos);
						ostrcatbig(&line, "#http://openaaf.dyndns.tv/mediathek/menu/", &maxlen, &bigpos);
						ostrcatbig(&line, pichname, &maxlen, &bigpos);
						ostrcatbig(&line, ".jpg#movie4k_", &maxlen, &bigpos);
						ostrcatbig(&line, pichname, &maxlen, &bigpos);
						ostrcatbig(&line, ".jpg#Movie4k - ", &maxlen, &bigpos);
						ostrcatbig(&line, title, &maxlen, &bigpos);
						ostrcatbig(&line, "#", &maxlen, &bigpos);
						char* ctype = oitoa(type);
						ostrcatbig(&line, ctype, &maxlen, &bigpos);
						free(ctype); ctype = NULL;
						ostrcatbig(&line, "\n", &maxlen, &bigpos);
					}
					if(url4 != NULL && ostrcmp(url, url4) != 0)
					{
						int type = 41;
	
						debug(99, "-------------------------------");
						debug(99, "(%d/%d/%d) %s (Part4) extra: %s url: %s",a ,i ,max ,tmphname, extra, url4);
											
						incount += 1;
						ostrcatbig(&line, tmphname, &maxlen, &bigpos);
						ostrcatbig(&line, " (Part4)", &maxlen, &bigpos);
/*
						if(extra != NULL)
						{
							ostrcatbig(&line, " (", &maxlen, &bigpos);
							ostrcatbig(&line, extra, &maxlen, &bigpos);
							ostrcatbig(&line, ")", &maxlen, &bigpos);					
						}
*/
						ostrcatbig(&line, "#http://movie4k.to/", &maxlen, &bigpos);
						ostrcatbig(&line, url4, &maxlen, &bigpos);
						ostrcatbig(&line, "#http://openaaf.dyndns.tv/mediathek/menu/", &maxlen, &bigpos);
						ostrcatbig(&line, pichname, &maxlen, &bigpos);
						ostrcatbig(&line, ".jpg#movie4k_", &maxlen, &bigpos);
						ostrcatbig(&line, pichname, &maxlen, &bigpos);
						ostrcatbig(&line, ".jpg#Movie4k - ", &maxlen, &bigpos);
						ostrcatbig(&line, title, &maxlen, &bigpos);
						ostrcatbig(&line, "#", &maxlen, &bigpos);
						char* ctype = oitoa(type);
						ostrcatbig(&line, ctype, &maxlen, &bigpos);
						free(ctype); ctype = NULL;
						ostrcatbig(&line, "\n", &maxlen, &bigpos);
					}
					free(tmphname), tmphname = NULL;
					free(extra), extra = NULL;
					free(pichname), pichname = NULL;

					if(nolinks != NULL)
					{
						printf("break\n");
						
						free(url); url = NULL;
						free(url2); url2 = NULL;
						free(url3); url3 = NULL;
						free(url4); url4 = NULL;
						free(pathnew); pathnew = NULL;
						free(logfile); logfile = NULL;
						free(id); id = NULL;
						break;
					}
				}
printf("77777777\n");

				free(url); url = NULL;
				free(url2); url2 = NULL;
				free(url3); url3 = NULL;
				free(url4); url4 = NULL;
				free(pathnew); pathnew = NULL;
				free(logfile); logfile = NULL;
				free(id); id = NULL;					
			}
		}
		free(ret1); ret1 = NULL;
	}
printf("88888888\n");

	free(tmpid); tmpid = NULL;
	free(tmpstr); tmpstr = NULL;	

	if(line != NULL)
	{
		line = string_replace_all("http://openaaf.dyndns.tv/", "http://imageshack.us/md/up/grd/", line, 1);
		tmpstr = ostrcat("/tmp/tithek/movie4k.hoster.", oitoa(time(NULL)), 0, 1);
		tmpstr = ostrcat(tmpstr, ".list", 1, 0);

		writesys(tmpstr, line, 0);
		free(line); line = NULL;
					
		struct tithek* tnode = (struct tithek*)listbox->select->handle;
		createtithek(tnode, tnode->title,  tmpstr, tnode->pic, tnode->localname, tnode->menutitle, tnode->flag);
		ret = 0;
	}

	return ret;
}

int movie4k_hoster_series(struct skin* grid, struct skin* listbox, struct skin* countlabel, struct skin* load, char* link, char* title)
{
	debug(99, "link: %s", link);
	int debuglevel = getconfigint("debuglevel", NULL);
	int ret = 1, series = 0, maxlen = 0, bigpos = 0;
	char* ip = NULL, *pos = NULL, *id = NULL, *tpath = NULL, *path = NULL, *tmpstr = NULL, *line = NULL, *episode = NULL;
	char* from = NULL;
	char* folgen = NULL;
	char* folgentmp = NULL;

	if(listbox == NULL || listbox->select == NULL || listbox->select->handle == NULL)
		return ret;
		
	drawscreen(load, 0, 0);

	ip = string_replace("http://", "", (char*)link, 0);

	if(ip != NULL)
		pos = strchr(ip, '/');
	if(pos != NULL)
	{
		pos[0] = '\0';
		path = pos + 1;
	}

	if(!ostrncmp("tvshows-", path, 8))
	{
		debug(99, "path: %s",path);
		tpath = ostrcat(path, NULL, 0, 0);
		id = oregex(".*tvshows-(.*[0-9]{4,10})-.*", path);
		tpath = string_replace("tvshows-season-", "", tpath, 1);
		tpath = string_replace(".html", "", tpath, 1);
		tpath = ostrcat(tpath, "-online-serie-", 1, 0);
		tpath = ostrcat(tpath, id, 1, 0);
		tpath = ostrcat(tpath, ".html", 1, 0);
		debug(99, "convertpath: %s",tpath);
	}
	else
		tpath = ostrcat(path, NULL, 0, 0);

	tmpstr = gethttp(ip, tpath, 80, NULL, NULL, 10000, NULL, 0);
	free(tpath), tpath = NULL;
	titheklog(debuglevel, "/tmp/movie4k2_tmpstr", NULL, NULL, NULL, tmpstr);

	if(tmpstr != NULL)
	{
		if(ostrstr(tmpstr, "episodeform") != NULL)
		{
			int i;
			for(i = 1; i < 30; i++)
			{
				from = ostrcat(from, "<FORM name=\"episodeform", 1, 0);
				from = ostrcat(from, oitoa(i), 1, 1);
				from = ostrcat(from, "\">", 1, 0);
				folgen = string_resub(from, "</FORM>", tmpstr, 0);
				folgen = string_resub("<OPTION></OPTION>", "</SELECT>", folgen, 1);
				folgen = string_replace_all("><", ">\n<", folgen, 1);
				folgentmp = ostrcat(folgen, NULL, 0, 0);

				if(folgen != NULL)
				{
					int count = 0;
					int incount = 0;
					int j;
					struct splitstr* ret1 = NULL;
					ret1 = strsplit(folgen, "\n", &count);
						
					if(ret1 != NULL && count > 0)
					{
						int max = count + 1;
						for(j = 1; j < max; j++)
						{
							link = string_resub("<OPTION value=\"", "\"", ret1[j-1].part, 0);
							id = oregex(".*tvshows-(.*[0-9]{4,10})-.*", link);
							episode = oregex(".*>Episode (.*[0-9]{1,10})</OPTION>.*", ret1[j-1].part);

 							debug(99, "(S%d/E%s)(%d) link: %s id: %s", i, episode, j, link, id);
							link = string_replace("tvshows-", "", link, 1);
							link = string_replace(id, "", link, 1);
							link = string_replace("-", "", link, 1);
							link = string_replace(".html", "", link, 1);
							link = ostrcat(link, "-online-serie-", 1, 0);
							link = ostrcat(link, id, 1, 0);
							link = ostrcat(link, ".html", 1, 0);
							debug(99, "(S%d/E%d) convertlink: %s id: %s", i, j, link, id);

							incount += 1;
							
							char* ci = oitoa(i);
//							char* cj = oitoa(j);
							ostrcatbig(&line, _("Season"), &maxlen, &bigpos);
							ostrcatbig(&line, " ", &maxlen, &bigpos);
							ostrcatbig(&line, ci, &maxlen, &bigpos);
							ostrcatbig(&line, " ", &maxlen, &bigpos);
							ostrcatbig(&line, _("Episode"), &maxlen, &bigpos);
							ostrcatbig(&line, " ", &maxlen, &bigpos);							
//							ostrcatbig(&line, cj, &maxlen, &bigpos);
							ostrcatbig(&line, episode, &maxlen, &bigpos);
							ostrcatbig(&line, "#http://movie4k.to/", &maxlen, &bigpos);
							ostrcatbig(&line, link, &maxlen, &bigpos);
							ostrcatbig(&line, "#http://openaaf.dyndns.tv/mediathek/menu/s", &maxlen, &bigpos);
							ostrcatbig(&line, ci, &maxlen, &bigpos);
							ostrcatbig(&line, "e", &maxlen, &bigpos);
//							ostrcatbig(&line, cj, &maxlen, &bigpos);
							ostrcatbig(&line, episode, &maxlen, &bigpos);
							ostrcatbig(&line, ".jpg#movie4k_search_", &maxlen, &bigpos);
							ostrcatbig(&line, ci, &maxlen, &bigpos);
							ostrcatbig(&line, "e", &maxlen, &bigpos);
//							ostrcatbig(&line, cj, &maxlen, &bigpos);
							ostrcatbig(&line, episode, &maxlen, &bigpos);
							ostrcatbig(&line, ".jpg#Movie4k - ", &maxlen, &bigpos);
							ostrcatbig(&line, title, &maxlen, &bigpos);
							ostrcatbig(&line, "#34\n", &maxlen, &bigpos);

							free(id); id = NULL;
							free(episode); episode = NULL;
							free(ci); ci = NULL;
//						free(cj); cj = NULL;
						}
					}
					free(ret1), ret1 = NULL;
				}				
				free(from), from = NULL;
				free(folgen), folgen = NULL;
				free(folgentmp), folgentmp = NULL;
			}
		}
	}
	free(tmpstr), tmpstr = NULL;	

	if(line != NULL)
	{
		line = string_replace_all("http://openaaf.dyndns.tv/", "http://imageshack.us/md/up/grd/", line, 1);
		if(series == 0)
		{
			tmpstr = ostrcat("/tmp/tithek/movie4k.hoster.", oitoa(time(NULL)), 0, 1);
			tmpstr = ostrcat(tmpstr, ".list", 1, 0);
		}
		else
		{
			tmpstr = ostrcat("/tmp/tithek/movie4k.hoster.ser.", oitoa(time(NULL)), 0, 1);
			tmpstr = ostrcat(tmpstr, ".list", 1, 0);
		}
		writesys(tmpstr, line, 0);

		titheklog(debuglevel, "/tmp/movie4k8_line", NULL, NULL, NULL, line);
		free(line); line = NULL;
					
		struct tithek* tnode = (struct tithek*)listbox->select->handle;
		createtithek(tnode, tnode->title,  tmpstr, tnode->pic, tnode->localname, tnode->menutitle, tnode->flag);
		ret = 0;
	}
	return ret;
}

int movie4k_series(struct skin* grid, struct skin* listbox, struct skin* countlabel, struct skin* load, char* link, char* title)
{
	debug(99, "link: %s", link);
	int debuglevel = getconfigint("debuglevel", NULL);
	int ret = 1, maxlen = 0, bigpos = 0;
	char* ip = NULL, *pos = NULL, *tpath = NULL, *path = NULL, *tmpstr = NULL, *line = NULL;
	char* from = NULL;
	char* folgen = NULL;
	char* folgentmp = NULL;
	char* name = NULL;
	char* lang = NULL;
	char* season = NULL;

	if(listbox == NULL || listbox->select == NULL || listbox->select->handle == NULL)
		return ret;
		
	drawscreen(load, 0, 0);

	ip = string_replace("http://", "", (char*)link, 0);

	if(ip != NULL)
		pos = strchr(ip, '/');
	if(pos != NULL)
	{
		pos[0] = '\0';
		path = pos + 1;
	}

	tmpstr = gethttp(ip, path, 80, NULL, NULL, 10000, NULL, 0);
	free(tpath), tpath = NULL;
	titheklog(debuglevel, "/tmp/movie4k2_tmpstr", NULL, NULL, NULL, tmpstr);

	if(tmpstr != NULL)
	{
		folgen = string_resub("<TABLE id=\"tablemoviesindex\">", "</TABLE>", tmpstr, 0);
		folgen = string_replace_all("\n", "", folgen, 1);
		folgen = string_replace_all("\t", "", folgen, 1);
		string_strip_whitechars(folgen);
		folgen = string_replace_all("</TD> </TR> <TR>", "</TD> </TR>\n<TR>", folgen, 1);

		if(folgen != NULL)
		{
			int count = 0;
			int j;
			struct splitstr* ret1 = NULL;
			ret1 = strsplit(folgen, "\n", &count);

			if(ret1 != NULL && count > 0)
			{
				int max = count;
				for(j = 0; j < max; j++)
				{
					link = string_resub("<a href=\"", "\">", ret1[j].part, 0);

					name = oregex(".*><a href=.*\">(.*)</a></TD>.*", ret1[j].part);

					if(ostrstr(ret1[j].part, "us_ger_small.png") != NULL)
						lang = ostrcat(" (de)", NULL, 0, 0);
					else if(ostrstr(ret1[j].part, "us_flag_small.png") != NULL)
						lang = ostrcat(" (en)", NULL, 0, 0);
					else
						lang = ostrcat(" (\?\?)", NULL, 0, 0);

					season = oregex(".*Season:(.*[0-9]{1,3}).*", name);
					season = strstrip(season);
					debug(99, "(S%s) input: %s%s",season, name, lang);

					ostrcatbig(&line, _("Season"), &maxlen, &bigpos);
					ostrcatbig(&line, " ", &maxlen, &bigpos);
					ostrcatbig(&line, season, &maxlen, &bigpos);
					ostrcatbig(&line, " ", &maxlen, &bigpos);
					
					ostrcatbig(&line, lang, &maxlen, &bigpos);
					ostrcatbig(&line, "#http://movie4k.to/", &maxlen, &bigpos);
					ostrcatbig(&line, link, &maxlen, &bigpos);
					ostrcatbig(&line, "#http://openaaf.dyndns.tv/mediathek/menu/s", &maxlen, &bigpos);
					ostrcatbig(&line, season, &maxlen, &bigpos);
					ostrcatbig(&line, ".jpg#s", &maxlen, &bigpos);
					ostrcatbig(&line, season, &maxlen, &bigpos);
					ostrcatbig(&line, ".jpg#Movie4k - ", &maxlen, &bigpos);
					ostrcatbig(&line, title, &maxlen, &bigpos);
					ostrcatbig(&line, "#39\n", &maxlen, &bigpos);

					free(name), name = NULL;
					free(lang), lang = NULL;
					free(season), season = NULL;
				}
			}
			free(ret1), ret1 = NULL;
		}				
		free(from), from = NULL;
		free(folgen), folgen = NULL;
		free(folgentmp), folgentmp = NULL;		
	}
	free(tmpstr), tmpstr = NULL;	

	if(line != NULL)
	{
		line = string_replace_all("http://openaaf.dyndns.tv/", "http://imageshack.us/md/up/grd/", line, 1);
		tmpstr = ostrcat("/tmp/tithek/movie4k.series.", oitoa(time(NULL)), 0, 1);
		tmpstr = ostrcat(tmpstr, ".list", 1, 0);

		writesys(tmpstr, line, 0);

		titheklog(debuglevel, "/tmp/movie4k8_line", NULL, NULL, NULL, line);
		free(line); line = NULL;
					
		struct tithek* tnode = (struct tithek*)listbox->select->handle;
		createtithek(tnode, tnode->title,  tmpstr, tnode->pic, tnode->localname, tnode->menutitle, tnode->flag);
		ret = 0;
	}
	return ret;
}

int movie4k_series_listed(struct skin* grid, struct skin* listbox, struct skin* countlabel, struct skin* load, char* link, char* title)
{
	debug(99, "link: %s", link);
	int debuglevel = getconfigint("debuglevel", NULL);
	int ret = 1, maxlen = 0, bigpos = 0;
	char* ip = NULL, *pos = NULL, *tpath = NULL, *path = NULL, *tmpstr = NULL, *line = NULL;
	char* from = NULL;
	char* folgen = NULL;
	char* folgentmp = NULL;
	char* name = NULL;
	char* lang = NULL;
	char* season = NULL;
	char* episode = NULL;

	if(listbox == NULL || listbox->select == NULL || listbox->select->handle == NULL)
		return ret;
		
	drawscreen(load, 0, 0);

	ip = string_replace("http://", "", (char*)link, 0);

	if(ip != NULL)
		pos = strchr(ip, '/');
	if(pos != NULL)
	{
		pos[0] = '\0';
		path = pos + 1;
	}

	tmpstr = gethttp(ip, path, 80, NULL, NULL, 10000, NULL, 0);
	free(tpath), tpath = NULL;
	titheklog(debuglevel, "/tmp/movie4k2_tmpstr", NULL, NULL, NULL, tmpstr);

	if(tmpstr != NULL)
	{
		folgen = string_resub("<TABLE id=\"tablemoviesindex\">", "</TABLE>", tmpstr, 0);
		folgen = string_replace_all("\n", "", folgen, 1);
		folgen = string_replace_all("\t", "", folgen, 1);
		string_strip_whitechars(folgen);
		folgen = string_replace_all("</TD> </TR> <TR>", "</TD> </TR>\n<TR>", folgen, 1);

		if(folgen != NULL)
		{
			int count = 0;
			int j;
			struct splitstr* ret1 = NULL;
			ret1 = strsplit(folgen, "\n", &count);

			if(ret1 != NULL && count > 0)
			{
				int max = count;
				for(j = 0; j < max; j++)
				{
					link = string_resub("<a href=\"", "\">", ret1[j].part, 0);
					name = oregex(".*><a href=.*\">(.*)</a></TD>.*", ret1[j].part);

					if(ostrstr(ret1[j].part, "us_ger_small.png") != NULL)
						lang = ostrcat(" (de)", NULL, 0, 0);
					else if(ostrstr(ret1[j].part, "us_flag_small.png") != NULL)
						lang = ostrcat(" (en)", NULL, 0, 0);
					else
						lang = ostrcat(" (\?\?)", NULL, 0, 0);

					season = oregex(".*Season:(.*),.*", name);
					season = strstrip(season);
					episode = oregex(".*Episode:(.*[0-9]{1,3}).*", name);
					episode = strstrip(episode);
					debug(99, "(S%s/E%s) input: %s%s",season, episode, name, lang);

					ostrcatbig(&line, _("Season"), &maxlen, &bigpos);
					ostrcatbig(&line, " ", &maxlen, &bigpos);
					ostrcatbig(&line, season, &maxlen, &bigpos);
					ostrcatbig(&line, " ", &maxlen, &bigpos);
					ostrcatbig(&line, _("Episode"), &maxlen, &bigpos);
					ostrcatbig(&line, " ", &maxlen, &bigpos);
					ostrcatbig(&line, episode, &maxlen, &bigpos);
					ostrcatbig(&line, " ", &maxlen, &bigpos);
					ostrcatbig(&line, lang, &maxlen, &bigpos);
					ostrcatbig(&line, "#http://movie4k.to/", &maxlen, &bigpos);
					ostrcatbig(&line, link, &maxlen, &bigpos);
					ostrcatbig(&line, "#http://openaaf.dyndns.tv/mediathek/menu/s", &maxlen, &bigpos);
					ostrcatbig(&line, season, &maxlen, &bigpos);
					ostrcatbig(&line, "e", &maxlen, &bigpos);
					ostrcatbig(&line, episode, &maxlen, &bigpos);
					ostrcatbig(&line, ".jpg#s", &maxlen, &bigpos);
					ostrcatbig(&line, season, &maxlen, &bigpos);
					ostrcatbig(&line, "e", &maxlen, &bigpos);
					ostrcatbig(&line, episode, &maxlen, &bigpos);
					ostrcatbig(&line, ".jpg#Movie4k - ", &maxlen, &bigpos);
					ostrcatbig(&line, title, &maxlen, &bigpos);
					ostrcatbig(&line, "#34\n", &maxlen, &bigpos); 

					free(name), name = NULL;
					free(lang), lang = NULL;
					free(season), season = NULL;
					free(episode), episode = NULL;
				}
			}
			free(ret1), ret1 = NULL;
		}				
		free(from), from = NULL;
		free(folgen), folgen = NULL;
		free(folgentmp), folgentmp = NULL;		
	}
	free(tmpstr), tmpstr = NULL;	

	if(line != NULL)
	{
		line = string_replace_all("http://openaaf.dyndns.tv/", "http://imageshack.us/md/up/grd/", line, 1);
		tmpstr = ostrcat("/tmp/tithek/movie4k.series.listed.", oitoa(time(NULL)), 0, 1);
		tmpstr = ostrcat(tmpstr, ".list", 1, 0);

		writesys(tmpstr, line, 0);

		titheklog(debuglevel, "/tmp/movie4k8_line", NULL, NULL, NULL, line);
		free(line); line = NULL;
					
		struct tithek* tnode = (struct tithek*)listbox->select->handle;
		createtithek(tnode, tnode->title,  tmpstr, tnode->pic, tnode->localname, tnode->menutitle, tnode->flag);
		ret = 0;
	}
	return ret;
}


#endif
