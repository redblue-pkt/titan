#ifndef MOVIE2K_H
#define MOVIE2K_H

char* movie2k(char* link)
{
	debug(99, "link: %s", link);
	int debuglevel = getconfigint("debuglevel", NULL);
	char* tmpstr = NULL, *tmpstr1 = NULL, *tmpstr2 = NULL, *url = NULL, *streamurl = NULL, *tmphost = NULL;
	titheklog(debuglevel, "/tmp/movie2k_streamurl_url0", NULL, NULL, NULL, link);

	if(link == NULL || ostrncmp("http://", link, 7)) return NULL;

	tmpstr1 = gethttps(link, NULL, NULL, NULL, NULL, NULL, 1);
	titheklog(debuglevel, "/var/usr/local/share/titan/plugins/tithek//movie2k_streamurl_tmpstr1_a", NULL, NULL, NULL, tmpstr1);

	tmpstr = string_resub("question.png", "underplayer", tmpstr1, 0);
	titheklog(debuglevel, "/var/usr/local/share/titan/plugins/tithek//movie2k_streamurl_tmpstr1_b", NULL, NULL, NULL, tmpstr);

	free(tmpstr1), tmpstr1 = NULL;

	if(tmpstr == NULL)
	{
		textbox(_("Message"), _("Connection Error, try again later.") , _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 1200, 200, 0, 0);
		goto end;
	}

	url = oregex(".*<a href=\"(http://.*)\" target.*", tmpstr);
	titheklog(debuglevel, "/var/usr/local/share/titan/plugins/tithek//movie2k_streamurl_url1", NULL, NULL, NULL, url);

	if(url == NULL)
	{
		textbox(_("Message"), _("Can not parse Stream URL, try again later.") , _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 1200, 200, 0, 0);
		goto end;
	}

	streamurl = hoster(url);
	titheklog(debuglevel, "/var/usr/local/share/titan/plugins/tithek//movie2k_streamurl_url7", NULL, NULL, NULL, url);
end:
	free(url), url = NULL;
	free(tmpstr), tmpstr = NULL;
	free(tmpstr2), tmpstr2 = NULL;
	free(tmphost), tmphost = NULL;

	return streamurl;
}

int movie2k_search(struct skin* grid, struct skin* listbox, struct skin* countlabel, struct skin* load, char* link, char* title, char* searchstr, int flag)
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
		send = ostrcat(send, "Host: www.movie2k.to\r\nAccept-Language: de-DE,de;q=0.8,en-US;q=0.6,en;q=0.4\r\n", 1, 0);
		send = ostrcat(send, "Connection: close\r\nUser-Agent: Mozilla/5.0 (Windows; U; Windows NT 5.1; de-DE; rv:1.9.0.3) Gecko/2008092417 Firefox/3.0.3\r\n\r\n", 1, 0);
		debug(99, "send: %s", send);

		tmpstr = gethttpreal("www.movie2k.to", "/searchAutoCompleteNew.php?search=the", 80, NULL, NULL, NULL, 0, send, NULL, 5000, 1);
		free(send); send = NULL;
		debug(99, "tmpstr: %s", tmpstr);
		titheklog(debuglevel, "/tmp/movie2k_search", NULL, NULL, NULL, tmpstr);

		char* key = string_resub("securekey=", "&search", tmpstr, 0);
		debug(99, "key: %s", key);

		send = ostrcat(send, "GET /movies.php?list=search&securekey=", 1, 0);
		send = ostrcat(send, key, 1, 0);
		send = ostrcat(send, "&search=", 1, 0);
		send = ostrcat(send, search, 1, 0);
		send = ostrcat(send, " HTTP/1.1\r\nAccept-Encoding: identity\r\n", 1, 0);
		send = ostrcat(send, "Accept-Language: de-DE,de;q=0.8,en-US;q=0.6,en;q=0.4\r\n", 1, 0);
		send = ostrcat(send, "Host: www.movie2k.to\r\n", 1, 0);
		send = ostrcat(send, "User-Agent: Mozilla/5.0 (Windows; U; Windows NT 5.1; de-DE; rv:1.9.0.3) Gecko/2008092417 Firefox/3.0.3\r\n", 1, 0);
		send = ostrcat(send, "Accept-Charset: ISO-8859-1,utf-8;q=0.7,*;q=0.7\r\n", 1, 0);
		send = ostrcat(send, "Connection: close\r\nCookie: xxx2=ok;\r\n\r\n", 1, 0);
		debug(99, "send: %s", send);

		free(tmpstr), tmpstr = NULL;
		tmpstr = gethttpreal("movie2k.to", "/", 80, NULL, NULL, NULL, 0, send, NULL, 5000, 1);

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
					ostrcatbig(&line, "#http://www.movie2k.to/", &maxlen, &bigpos);
					ostrcatbig(&line, link, &maxlen, &bigpos);
					ostrcatbig(&line, "#", &maxlen, &bigpos);
					ostrcatbig(&line, pic, &maxlen, &bigpos);
					ostrcatbig(&line, "#movie2k_search_", &maxlen, &bigpos);
					char* cincount = oitoa(incount + time(NULL));
					ostrcatbig(&line, cincount, &maxlen, &bigpos);
					free(cincount); cincount = NULL;
					ostrcatbig(&line, ".jpg#movie2k - Search#", &maxlen, &bigpos);
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
			menu = ostrcat("/tmp/tithek/movie2k.search.", oitoa(time(NULL)), 0, 1);
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

int movie2k_search_local(struct skin* grid, struct skin* listbox, struct skin* countlabel, struct skin* load, char* link, char* title, char* searchstr, int flag)
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

		tmpstr = gethttp("openaaf.dyndns.tv", "/mediathek/movie2k/streams/movie2k.all-sorted.list", 80, NULL, HTTPAUTH, 5000, NULL, 0);

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
				menu = ostrcat("/tmp/tithek/movie2k.search.", oitoa(time(NULL)), 0, 1);
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

int movie2k_hoster(struct skin* grid, struct skin* listbox, struct skin* countlabel, struct skin* load, char* link, char* title)
{
	debug(99, "link: %s", link);
	int debuglevel = getconfigint("debuglevel", NULL);
	int ret = 1, maxlen = 0, bigpos = 0;
	char* tmpstr = NULL, *line = NULL, *url = NULL;
	char* tmphname = NULL;
	char* pichname = NULL;

	if(listbox == NULL || listbox->select == NULL || listbox->select->handle == NULL)
		return ret;
		
	drawscreen(load, 0, 0);

	tmpstr = gethttps(link, NULL, NULL, NULL, NULL, NULL, 1);
	titheklog(debuglevel, "/var/usr/local/share/titan/plugins/tithek/movie2k1_hoster_tmpstr", NULL, NULL, NULL, tmpstr);

	if(tmpstr != NULL)
	{	
		tmpstr = string_replace_all("\n", " ", tmpstr, 1);
		tmpstr = string_replace_all("\t", " ", tmpstr, 1);
		string_strip_whitechars(tmpstr);
		tmpstr = string_replace_all("<tr id=\"tablemoviesindex2\"", "\n<tr id=\"tablemoviesindex2\"", tmpstr, 1);
		tmpstr = string_replace_all("<select name=\"hosterlist\"", "\n<select name=\"hosterlist\"", tmpstr, 1);
		titheklog(debuglevel, "/var/usr/local/share/titan/plugins/tithek/movie2k2_hoster_tmpstr_replace", NULL, NULL, NULL, tmpstr);

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
printf("##############################################################\n");

printf("ret1[i].part: %s\n",ret1[i].part);


				if(ostrstr(ret1[i].part, "<tr id=\"tablemoviesindex2\"") != NULL)
				{
					printf("found: tablemoviesindex2\n");
					url = string_resub("<a href=\"", "\"", ret1[i].part, 0);
					printf("url: %s\n",url);
					tmphname = string_resub("title=\"", " ", ret1[i].part, 0);
					printf("tmphname: %s\n",tmphname);
					pichname = ostrcat(tmphname, NULL, 0, 0);
					string_tolower(pichname);
					pichname = stringreplacecharonce(pichname, '.', '\0');
					printf("pichname: %s\n",pichname);
/*
					char* tmpstr1 = NULL;
					tmpstr1 = gethttps(url, NULL, NULL, NULL, NULL, NULL, 1);
					if(ostrstr(tmpstr1, "<select name=\"hosterlist\"") != NULL)
					{
printf("-----------------------------------------------------\n");
						printf("found: hosterlist\n");
						tmpstr1 = string_replace_all("\n", " ", tmpstr1, 1);
						printf("found: hosterlist1\n");
						tmpstr1 = string_replace_all("\t", " ", tmpstr1, 1);
						printf("found: hosterlist2\n");
						string_strip_whitechars(tmpstr1);
						printf("found: hosterlist3\n");
						tmpstr1 = string_replace_all("<select name=\"hosterlist\"", "\n<select name=\"hosterlist\"", tmpstr1, 1);
						char* tmpstr2 = NULL;
						printf("found: hosterlist4\n");
						tmpstr2 = string_resub("<select name=\"hosterlist\"", "</select>", tmpstr1, 0);
						printf("tmpstr2: %s\n",tmpstr2);
						free(tmpstr2), tmpstr2 = NULL;
printf("-----------------------------------------------------\n");

					}
					free(tmpstr1), tmpstr1 = NULL;
*/
					int type = 94;
					
					debug(99, "(%d/%d) %s url: %s",i ,max ,tmphname, url);

					incount += 1;
					ostrcatbig(&line, tmphname, &maxlen, &bigpos);
					ostrcatbig(&line, "#", &maxlen, &bigpos);
					ostrcatbig(&line, url, &maxlen, &bigpos);
					ostrcatbig(&line, "#http://openaaf.dyndns.tv/mediathek/menu/", &maxlen, &bigpos);
					ostrcatbig(&line, pichname, &maxlen, &bigpos);
					ostrcatbig(&line, ".jpg#movie2k_", &maxlen, &bigpos);
					ostrcatbig(&line, pichname, &maxlen, &bigpos);
					ostrcatbig(&line, ".jpg#movie2k - ", &maxlen, &bigpos);
					ostrcatbig(&line, title, &maxlen, &bigpos);
					ostrcatbig(&line, "#", &maxlen, &bigpos);
					char* ctype = oitoa(type);
					ostrcatbig(&line, ctype, &maxlen, &bigpos);
					free(ctype), ctype = NULL;
					ostrcatbig(&line, "\n", &maxlen, &bigpos);
				}
				free(url); url = NULL;
			}
		}
		free(ret1); ret1 = NULL;
	}
	free(tmpstr); tmpstr = NULL;	

	if(line != NULL)
	{
		line = string_replace_all("http://openaaf.dyndns.tv/", "http://imageshack.us/md/up/grd/", line, 1);
		tmpstr = ostrcat("/tmp/tithek/movie2k.hoster.", oitoa(time(NULL)), 0, 1);
		tmpstr = ostrcat(tmpstr, ".list", 1, 0);

		writesys(tmpstr, line, 0);
		free(line); line = NULL;
					
		struct tithek* tnode = (struct tithek*)listbox->select->handle;
		createtithek(tnode, tnode->title,  tmpstr, tnode->pic, tnode->localname, tnode->menutitle, tnode->flag);
		ret = 0;
	}

	return ret;
}

int movie2k_hoster_series(struct skin* grid, struct skin* listbox, struct skin* countlabel, struct skin* load, char* link, char* title)
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
	titheklog(debuglevel, "/tmp/movie2k2_tmpstr", NULL, NULL, NULL, tmpstr);

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
							ostrcatbig(&line, "#http://www.movie2k.to/", &maxlen, &bigpos);
							ostrcatbig(&line, link, &maxlen, &bigpos);
							ostrcatbig(&line, "#http://openaaf.dyndns.tv/mediathek/menu/s", &maxlen, &bigpos);
							ostrcatbig(&line, ci, &maxlen, &bigpos);
							ostrcatbig(&line, "e", &maxlen, &bigpos);
//							ostrcatbig(&line, cj, &maxlen, &bigpos);
							ostrcatbig(&line, episode, &maxlen, &bigpos);
							ostrcatbig(&line, ".jpg#movie2k_search_", &maxlen, &bigpos);
							ostrcatbig(&line, ci, &maxlen, &bigpos);
							ostrcatbig(&line, "e", &maxlen, &bigpos);
//							ostrcatbig(&line, cj, &maxlen, &bigpos);
							ostrcatbig(&line, episode, &maxlen, &bigpos);
							ostrcatbig(&line, ".jpg#movie2k - ", &maxlen, &bigpos);
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
			tmpstr = ostrcat("/tmp/tithek/movie2k.hoster.", oitoa(time(NULL)), 0, 1);
			tmpstr = ostrcat(tmpstr, ".list", 1, 0);
		}
		else
		{
			tmpstr = ostrcat("/tmp/tithek/movie2k.hoster.ser.", oitoa(time(NULL)), 0, 1);
			tmpstr = ostrcat(tmpstr, ".list", 1, 0);
		}
		writesys(tmpstr, line, 0);

		titheklog(debuglevel, "/tmp/movie2k8_line", NULL, NULL, NULL, line);
		free(line); line = NULL;
					
		struct tithek* tnode = (struct tithek*)listbox->select->handle;
		createtithek(tnode, tnode->title,  tmpstr, tnode->pic, tnode->localname, tnode->menutitle, tnode->flag);
		ret = 0;
	}
	return ret;
}

int movie2k_series(struct skin* grid, struct skin* listbox, struct skin* countlabel, struct skin* load, char* link, char* title)
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
	titheklog(debuglevel, "/tmp/movie2k2_tmpstr", NULL, NULL, NULL, tmpstr);

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
					ostrcatbig(&line, "#http://www.movie2k.to/", &maxlen, &bigpos);
					ostrcatbig(&line, link, &maxlen, &bigpos);
					ostrcatbig(&line, "#http://openaaf.dyndns.tv/mediathek/menu/s", &maxlen, &bigpos);
					ostrcatbig(&line, season, &maxlen, &bigpos);
					ostrcatbig(&line, ".jpg#s", &maxlen, &bigpos);
					ostrcatbig(&line, season, &maxlen, &bigpos);
					ostrcatbig(&line, ".jpg#movie2k - ", &maxlen, &bigpos);
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
		tmpstr = ostrcat("/tmp/tithek/movie2k.series.", oitoa(time(NULL)), 0, 1);
		tmpstr = ostrcat(tmpstr, ".list", 1, 0);

		writesys(tmpstr, line, 0);

		titheklog(debuglevel, "/tmp/movie2k8_line", NULL, NULL, NULL, line);
		free(line); line = NULL;
					
		struct tithek* tnode = (struct tithek*)listbox->select->handle;
		createtithek(tnode, tnode->title,  tmpstr, tnode->pic, tnode->localname, tnode->menutitle, tnode->flag);
		ret = 0;
	}
	return ret;
}

int movie2k_series_listed(struct skin* grid, struct skin* listbox, struct skin* countlabel, struct skin* load, char* link, char* title)
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
	titheklog(debuglevel, "/tmp/movie2k2_tmpstr", NULL, NULL, NULL, tmpstr);

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
					ostrcatbig(&line, "#http://www.movie2k.to/", &maxlen, &bigpos);
					ostrcatbig(&line, link, &maxlen, &bigpos);
					ostrcatbig(&line, "#http://openaaf.dyndns.tv/mediathek/menu/s", &maxlen, &bigpos);
					ostrcatbig(&line, season, &maxlen, &bigpos);
					ostrcatbig(&line, "e", &maxlen, &bigpos);
					ostrcatbig(&line, episode, &maxlen, &bigpos);
					ostrcatbig(&line, ".jpg#s", &maxlen, &bigpos);
					ostrcatbig(&line, season, &maxlen, &bigpos);
					ostrcatbig(&line, "e", &maxlen, &bigpos);
					ostrcatbig(&line, episode, &maxlen, &bigpos);
					ostrcatbig(&line, ".jpg#movie2k - ", &maxlen, &bigpos);
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
		tmpstr = ostrcat("/tmp/tithek/movie2k.series.listed.", oitoa(time(NULL)), 0, 1);
		tmpstr = ostrcat(tmpstr, ".list", 1, 0);

		writesys(tmpstr, line, 0);

		titheklog(debuglevel, "/tmp/movie2k8_line", NULL, NULL, NULL, line);
		free(line); line = NULL;
					
		struct tithek* tnode = (struct tithek*)listbox->select->handle;
		createtithek(tnode, tnode->title,  tmpstr, tnode->pic, tnode->localname, tnode->menutitle, tnode->flag);
		ret = 0;
	}
	return ret;
}


#endif
