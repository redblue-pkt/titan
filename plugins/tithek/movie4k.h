#ifndef MOVIE4K_H
#define MOVIE4K_H

// flag 1 = putlocker/sockshare
// flag 2 = filenuke
//http://www.movie4k.to/movies.php?list=search&securekey=sk1014414788&search=Die+Monster+Uni

char* movie4k(char* link, char* url, char* name, int flag)
{
	debug(99, "link(%d): %s", flag, link);
	char* video_id = NULL, *source = NULL, *streamurl = NULL;

	if(flag == 1 || flag == 2 || flag == 3 || flag == 4 || flag == 5 || flag == 6 || flag == 7)
	{
		int count = 0;
		struct splitstr* ret1 = NULL;
		ret1 = strsplit(link, ";", &count);
		if(ret1 != NULL && count >= 3)
		{			
			video_id = ostrcat(video_id, ret1[1].part, 1, 0);
			debug(99, "video_id: %s", video_id);

			source = ostrcat(source, ret1[2].part, 1, 0);
			debug(99, "source: %s", source);

			if(flag == 1)
				streamurl = putlocker(source, video_id);
			else if(flag == 2)
				streamurl = filenuke(source, video_id);
			else if(flag == 3)
				streamurl = streamcloud(source, video_id);
			else if(flag == 4)
				streamurl = flashx(source, video_id);
			else if(flag == 5)
				streamurl = vidstream(source, video_id);
			else if(flag == 6)
				streamurl = xvidstage(source, video_id);
			else if(flag == 7)
				streamurl = nowvideo(source, video_id);

			debug(99, "streamurl1: %s", streamurl);

			streamurl = string_replace_all("amp;", "", streamurl, 1);
			debug(99, "streamurl2: %s", streamurl);
		}
		free(ret1), ret1 = NULL;
	}
	free(video_id), video_id = NULL;
	free(source), source = NULL;

	return streamurl;
}

int movie4k_search(struct skin* grid, struct skin* listbox, struct skin* countlabel, struct skin* load, char* link, char* title, char* searchstr, int flag)
{
	int ret = 1;

	if(listbox == NULL || listbox->select == NULL || listbox->select->handle == NULL)
		return ret;

	char* search = NULL;
	if(flag == 0)
	{
		if(searchstr == NULL)
			search = textinputhist("Search", " ", "searchhist");
		else
			search = textinputhist("Search", searchstr, "searchhist");
	}
	else
		search = ostrcat(link, NULL, 0, 0);

	if(search != NULL)
	{
		drawscreen(load, 0, 0);
		search = stringreplacechar(search, ' ', '+');
		char* tmpstr = NULL;
		char* tmpstr1 = NULL;
		char* line = NULL;
		char* pic = NULL;
		char* title = NULL;	
		char* lang = NULL;
		char* langck = NULL;
		char* from = NULL;
		char* url = NULL;
		char* ip = NULL;
		char* path = NULL;
		char* menu = NULL;

		ip = ostrcat("movie4k.to", NULL, 0, 0);
		if(flag == 0)
			path = ostrcat("Search.html?q=", search, 0, 0);
		else
			path = string_replace_all("http://movie4k.to/", "", search, 0);
		
		tmpstr = gethttp(ip, path, 80, NULL, NULL, 10000, NULL, 0);
		tmpstr = string_resub("<div id=\"beep\" class=\"beep\"></div>", "</table>", tmpstr, 0);

		if(getconfigint("debuglevel", NULL) == 99)
			writesys("/tmp/movie4k1_tmpstr", tmpstr, 0);


		int count = 0;
		int incount = 0;
		int i = 0;
		struct splitstr* ret1 = NULL;
		ret1 = strsplit(tmpstr, "\n", &count);

		if(ret1 != NULL)
		{
			int max = count;
			for(i = 0; i < max; i++)
			{
				if(oregex(".*src=\"/gr/sys/lng/(.*)\" alt=\"language\"></td>.*", ret1[i].part) != NULL)
				{
					free(langck), langck = NULL;
					langck = oregex(".*src=\"/gr/sys/lng/(.*)\" alt=\"language\"></td>.*", ret1[i].part);
				}
				
				if(oregex(".*<td class=\"Title\"><a href=\"(.*)\" onclick=.*", ret1[i].part) != NULL)
				{
					free(path), path = NULL;
					path = oregex(".*<td class=\"Title\"><a href=\"(.*)\" onclick=.*", ret1[i].part);

					if(ostrstr(langck, "1.png") != NULL)
						lang = ostrcat(lang, " (de)", 1, 0);
					else if(ostrstr(langck, "2.png") != NULL)
						lang = ostrcat(lang, " (en)", 1, 0);
					else
						lang = ostrcat(lang, " (\?\?)", 1, 0);
																	
					tmpstr1 = gethttp("movie4k.to", path, 80, NULL, NULL, 10000, NULL, 0);

					from = ostrcat("<div class=\"Grahpics\"><a href=\"", path, 0, 0);
					from = ostrcat(from, "\"><img src=\"", 1, 0);

					pic = string_resub(from, "\" alt=\"", tmpstr1, 0);
					
					if(pic == NULL)
						pic = ostrcat("dummy", NULL, 0, 0);

					title = ostrcat(path, NULL, 0, 0);
					title = string_replace_all("/Stream/", "", title, 1);
					title = string_replace_all(".html", "", title, 1);
				 	title = stringreplacechar(title, '_', ' ');
					title = ostrcat(title , lang, 1, 0);

					url = ostrcat("http://movie4k.to", path, 0, 0);

					debug(99, "---------------------------");
					debug(99, "langck: %s", langck);
					debug(99, "pic: %s", pic);
					debug(99, "title: %s", title);
					debug(99, "url: %s", url);					
					debug(99, "---------------------------");

					if(url != NULL)
					{
						incount += 1;
						line = ostrcat(line, title, 1, 0);
						line = ostrcat(line, "#", 1, 0);
						line = ostrcat(line, url, 1, 0);
						line = ostrcat(line, "#", 1, 0);
						line = ostrcat(line, pic, 1, 0);
						line = ostrcat(line, "#movie4k_search_", 1, 0);
						line = ostrcat(line, oitoa(incount + time(NULL)), 1, 1);
						line = ostrcat(line, ".jpg#movie4k - Search#22\n", 1, 0);
					}
					free(url), url = NULL;
					free(path), path = NULL;
					free(title), title = NULL;
					free(pic), pic = NULL;		
					free(from), from = NULL;
					free(tmpstr1), tmpstr1 = NULL;
					free(lang), lang = NULL;
					free(langck), langck = NULL;	
				}
			}
			free(ret1), ret1 = NULL;
			if(line != NULL)
			{
				menu = ostrcat("/tmp/tithek/movie4k.search.list", NULL, 0, 0);
				writesys(menu, line, 0);
				struct tithek* tnode = (struct tithek*)listbox->select->handle;
				createtithek(tnode, tnode->title,  menu, tnode->pic, tnode->localname, tnode->menutitle, tnode->flag);
				ret = 0;
			}

		}
		free(tmpstr), tmpstr = NULL;
		free(ip), ip = NULL;
	}
	free(search), search = NULL;
	return ret;
}

int movie4k_hoster(struct skin* grid, struct skin* listbox, struct skin* countlabel, struct skin* load, char* link, char* title)
{
	debug(99, "link: %s", link);
	int ret = 1, series = 0;
	char* ip = NULL, *pos = NULL, *path = NULL, *tmpstr = NULL, *tmpstr1 = NULL, *tmpstr2 = NULL, *tmpstr3 = NULL, *tmpstr4 = NULL, *tmpstr5 = NULL, *line = NULL, *url = NULL, *url2 = NULL, *url3 = NULL, *url4 = NULL, *pathnew = NULL;

	if(listbox == NULL || listbox->select == NULL || listbox->select->handle == NULL)
		return ret;

	ip = string_replace("http://", "", (char*)link, 0);

	if(ip != NULL)
		pos = strchr(ip, '/');
	if(pos != NULL)
	{
		pos[0] = '\0';
		path = pos + 1;
	}

	tmpstr = gethttp(ip, path, 80, NULL, NULL, 10000, NULL, 0);
	if(getconfigint("debuglevel", NULL) == 99)
		writesys("/tmp/movie4k2_tmpstr", tmpstr, 0);

	if(tmpstr != NULL)
	{
		drawscreen(load, 0, 0);

		char* hname = NULL;
		char* hnr = NULL;
		char* hlink = NULL;
		char* id = NULL;
		char* logfile = NULL;
				
		char* nolinks = NULL;
		if(ostrstr(tmpstr, "links\[") == NULL)
		{
			hname = string_resub("width=\"16\"> &nbsp;", "</a></td><td align=", tmpstr, 0);
			nolinks = ostrcat(tmpstr, NULL, 0, 0);
		}
													
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
				if((!ostrncmp("links", ret1[i].part, 5) && nolinks == NULL) || nolinks != NULL)
				{
					if(nolinks == NULL)
					{
						ret1[i].part = string_replace_all("</a>&nbsp;</td>", "</a></td>", ret1[i].part, 0);

						pathnew = string_resub("<a href=\\\"", "\\", ret1[i].part, 0);
						hname = string_resub("&nbsp;", "</a", ret1[i].part, 0);
						id = oregex(".*tvshows-(.*[0-9]{1,10})-.*", pathnew);

						debug(99, "(%d) pathnew: %s hname: %s id: %s",i ,pathnew, hname, id);

						pathnew = string_replace("tvshows-", "", pathnew, 1);
						pathnew = string_replace(id, "", pathnew, 1);
						pathnew = string_replace("-", "", pathnew, 1);
						pathnew = string_replace(".html", "", pathnew, 1);
						pathnew = ostrcat(pathnew, "-online-serie-", 1, 0);
						pathnew = ostrcat(pathnew, id, 1, 0);
						pathnew = ostrcat(pathnew, ".html", 1, 0);

						debug(99, "(%d) convert pathnew: %s", i, pathnew);

						logfile = ostrcat("/tmp/movie4k4_pathnew1", id, 0, 0);
						logfile = ostrcat(logfile, "_", 1, 0);
						logfile = ostrcat(logfile, hname, 1, 0);

						if(getconfigint("debuglevel", NULL) == 99)
							writesys(logfile, pathnew, 0);
						free(logfile), logfile= NULL;
						
						tmpstr1 = gethttp("movie4k.to", pathnew, 80, NULL, NULL, 10000, NULL, 0);
						free(pathnew), pathnew = NULL;
					}
					else
					{
						tmpstr1 = ostrcat(nolinks, NULL, 0, 0);
						id = string_resub("online-film-", ".html", path, 0);
						if(id == NULL)
							id = string_resub("watch-movie-", ".html", path, 0);

					}	
					logfile = ostrcat("/tmp/movie4k5_tmpstr1", id, 0, 0);
					logfile = ostrcat(logfile, "_", 1, 0);
					logfile = ostrcat(logfile, hname, 1, 0);

					if(getconfigint("debuglevel", NULL) == 99)
						writesys(logfile, tmpstr1, 0);
					free(logfile), logfile= NULL;
					
					url = string_resub("<a target=\"_blank\" href=\"", "\"><", tmpstr1, 0);

					if(ostrstr(url, "http://") == NULL)
					{
						free(url), url = NULL;
						url = string_resub("<iframe src=", "\" width", tmpstr1, 0);
					}
					
					if(ostrstr(url, "http://www.facebook.com") != NULL)
					{
						free(url), url = NULL;
						url = oregex(".*<iframe width=.*(http://.*)&width.*", tmpstr1);
					}
					
					url = string_replace_all("/embed/", "/file/", url, 1);
					url = string_replace_all("\"", "", url, 1);

					int countj = 0;
					if(ostrstr(tmpstr1, "&part=1") != NULL)
						countj = 1;
					if(ostrstr(tmpstr1, "&part=2") != NULL)
						countj = 2;
					if(ostrstr(tmpstr1, "&part=3") != NULL)
						countj = 3;
					if(ostrstr(tmpstr1, "&part=4") != NULL)
						countj = 4;

					if(countj == 0)
						free(id), id = NULL;

					free(tmpstr1), tmpstr1 = NULL;
					tmpstr1 = ostrcat(url, NULL, 0, 0);

					if(id != NULL)
					{
						if(countj >= 2)
						{
							pathnew = ostrcat("movie.php?id=", id, 0, 0);
							pathnew = ostrcat(pathnew, "&part=2", 1, 0);
							tmpstr3 = gethttp("movie4k.to", pathnew, 80, NULL, NULL, 10000, NULL, 0);

							logfile = ostrcat("/tmp/movie4k_tmpstr3_", id, 0, 0);
							logfile = ostrcat(logfile, "_", 1, 0);
							logfile = ostrcat(logfile, hname, 1, 0);

							if(getconfigint("debuglevel", NULL) == 99)
								writesys(logfile, tmpstr3, 0);
							free(logfile), logfile= NULL;

							url2 = string_resub("<a target=\"_blank\" href=\"", "\"><", tmpstr3, 0);
		
							if(ostrstr(url2, "http://") == NULL)
							{
								free(url2), url2 = NULL;
								url2 = string_resub("<iframe src=", "\" width", tmpstr3, 0);
							}
							
							if(ostrstr(url2, "http://www.facebook.com") != NULL)
							{
								free(url2), url2 = NULL;
								url2 = oregex(".*<iframe width=.*(http://.*)&width.*", tmpstr3);
							}
							
							url2 = string_replace_all("/embed/", "/file/", url2, 1);
							url2 = string_replace_all("\"", "", url2, 1);
							free(tmpstr3), tmpstr3 = NULL;
							tmpstr3 = ostrcat(url2, NULL, 0, 0);
						}

						if(countj >= 3)
						{
							pathnew = ostrcat("movie.php?id=", id, 0, 0);
							pathnew = ostrcat(pathnew, "&part=3", 1, 0);
							tmpstr4 = gethttp("movie4k.to", pathnew, 80, NULL, NULL, 10000, NULL, 0);

							logfile = ostrcat("/tmp/movie4k_tmpstr4_", id, 0, 0);
							logfile = ostrcat(logfile, "_", 1, 0);
							logfile = ostrcat(logfile, hname, 1, 0);
					
							if(getconfigint("debuglevel", NULL) == 99)
								writesys(logfile, tmpstr4, 0);
							free(logfile), logfile= NULL;

							url3 = string_resub("<a target=\"_blank\" href=\"", "\"><", tmpstr4, 0);
		
							if(ostrstr(url3, "http://") == NULL)
							{
								free(url3), url3 = NULL;
								url3 = string_resub("<iframe src=", "\" width", tmpstr4, 0);
							}
							
							if(ostrstr(url3, "http://www.facebook.com") != NULL)
							{
								free(url3), url3 = NULL;
								url3 = oregex(".*<iframe width=.*(http://.*)&width.*", tmpstr4);
							}
							
							url3 = string_replace_all("/embed/", "/file/", url3, 1);
							url3 = string_replace_all("\"", "", url3, 1);
							free(tmpstr4), tmpstr4 = NULL;
							tmpstr4 = ostrcat(url3, NULL, 0, 0);
						}						

						if(countj >= 4)
						{
							pathnew = ostrcat("movie.php?id=", id, 0, 0);
							pathnew = ostrcat(pathnew, "&part=4", 1, 0);
							tmpstr5 = gethttp("movie4k.to", pathnew, 80, NULL, NULL, 10000, NULL, 0);

							logfile = ostrcat("/tmp/movie4k_tmpstr5_", id, 0, 0);
							logfile = ostrcat(logfile, "_", 1, 0);
							logfile = ostrcat(logfile, hname, 1, 0);
					
							if(getconfigint("debuglevel", NULL) == 99)
								writesys(logfile, tmpstr5, 0);
							free(logfile), logfile= NULL;

							url4 = string_resub("<a target=\"_blank\" href=\"", "\"><", tmpstr5, 0);
		
							if(ostrstr(url4, "http://") == NULL)
							{
								free(url4), url4 = NULL;
								url4 = string_resub("<iframe src=", "\" width", tmpstr5, 0);
							}
							
							if(ostrstr(url4, "http://www.facebook.com") != NULL)
							{
								free(url4), url4 = NULL;
								url4 = oregex(".*<iframe width=.*(http://.*)&width.*", tmpstr5);
							}
							
							url4 = string_replace_all("/embed/", "/file/", url4, 1);
							url4 = string_replace_all("\"", "", url4, 1);
							free(tmpstr5), tmpstr5 = NULL;
							tmpstr5 = ostrcat(url4, NULL, 0, 0);
						}
					}
						
					int type = 0;
					int count2 = 0;
					struct splitstr* ret2 = NULL;
					ret2 = strsplit(tmpstr1, "/", &count2);
		
					if(ret2 != NULL && count2 > 3 && ostrcmp(hname, "Sockshare") == 0)
					{
						free(hname), hname = NULL;
						hname = ostrcat("Sockshare.com", NULL, 0, 0);
						tmpstr2 = ostrcat(ret2[3].part, NULL, 0, 0);
						type = 14;
					}
					else if(ret2 != NULL && count2 > 3 && ostrcmp(hname, "Putlocker") == 0)
					{
						free(hname), hname = NULL;
						hname = ostrcat("Putlocker.com", NULL, 0, 0);
						tmpstr2 = ostrcat(ret2[3].part, NULL, 0, 0);
						type = 14;
					}
					else if(ret2 != NULL && count2 > 2 && ostrcmp(hname, "Filenuke") == 0)
					{
						free(hname), hname = NULL;
						hname = ostrcat("FileNuke.com", NULL, 0, 0);
						tmpstr2 = ostrcat(ret2[2].part, NULL, 0, 0);
						type = 15;
					}
					else if(ret2 != NULL && count2 > 2 && ostrcmp(hname, "Streamclou") == 0)
					{
						free(hname), hname = NULL;
						hname = ostrcat("StreamCloud.eu", NULL, 0, 0);
						tmpstr2 = ostrcat(ret2[2].part, NULL, 0, 0);
						type = 20;
					}
					else if(ret2 != NULL && count2 > 2 && ostrcmp(hname, "VidStream") == 0)
					{
						free(hname), hname = NULL;
						hname = ostrcat("VidStream.in", NULL, 0, 0);
						tmpstr2 = ostrcat(ret2[2].part, NULL, 0, 0);
						type = 25;
					}
					else if(ret2 != NULL && count2 > 3 && ostrcmp(hname, "Flashx") == 0)
					{
						free(hname), hname = NULL;
						hname = ostrcat("FlashX.tv", NULL, 0, 0);
						tmpstr2 = ostrcat(ret2[3].part, NULL, 0, 0);
						type = 24;
					}
					else if(ret2 != NULL && count2 > 3 && ostrcmp(hname, "PrimeShare") == 0)
					{
						free(hname), hname = NULL;
						hname = ostrcat("PrimeShare.tv", NULL, 0, 0);
						tmpstr2 = ostrcat(ret2[3].part, NULL, 0, 0);
					}				
					else if(ret2 != NULL && count2 > 2 && ostrcmp(hname, "Xvidstage") == 0)
					{
						free(hname), hname = NULL;
						hname = ostrcat("XvidStage.com", NULL, 0, 0);
						tmpstr2 = ostrcat(ret2[2].part, NULL, 0, 0);
						type = 26;
					}
					else if(ret2 != NULL && count2 > 3 && ostrcmp(hname, "Vidxden.com (DivX)") == 0)
					{
						tmpstr2 = ostrcat(ret2[3].part, NULL, 0, 0);
					}
					else if(ret2 != NULL && count2 > 3 && ostrcmp(hname, "Nowvideo") == 0)
					{
						free(hname), hname = NULL;
						hname = ostrcat("NowVideo.eu", NULL, 0, 0);
						tmpstr2 = ostrcat(ret2[3].part, NULL, 0, 0);
						type = 27;
					}
					else if(ret2 != NULL && count2 > 2)
					{
						tmpstr2 = ostrcat(ret2[2].part, NULL, 0, 0);
						hname = ostrcat(hname, " (coming soon)", 1, 0);
						type = 66;
					}
					else
					{
						tmpstr2 = ostrcat("unknown", NULL, 0, 0);
						hname = ostrcat(hname, " (coming soon)", 1, 0);
						type = 66;
					}
						
					free(ret2), ret2 = NULL;
					debug(99, "-------------------------------");
					if(ostrcmp(url, url2) != 0)
					{
						debug(99, "hname: %s (Part1) url: %s id: %s", hname, url, tmpstr2);
					}
					else
					{
						debug(99, "hname: %s url: %s id: %s", hname, url, tmpstr2);
					}
					
					incount += 1;
					line = ostrcat(line, hname, 1, 0);
					if(url2 != NULL && ostrcmp(url, url2) != 0)
						line = ostrcat(line, " (Part1)", 1, 0);					
					line = ostrcat(line, "#", 1, 0);
					line = ostrcat(line, url, 1, 0);
					line = ostrcat(line, ";", 1, 0);
					line = ostrcat(line, tmpstr2, 1, 0);
					line = ostrcat(line, ";", 1, 0);				
					line = ostrcat(line, hname, 1, 0);
					line = ostrcat(line, "#", 1, 0);
					line = ostrcat(line, "http://atemio.dyndns.tv/mediathek/menu/", 1, 0);
					line = ostrcat(line, hname, 1, 0);
					line = ostrcat(line, ".jpg#movie4k_", 1, 0);
					line = ostrcat(line, hname, 1, 0);
					line = ostrcat(line, ".jpg#Movie4k - ", 1, 0);
					line = ostrcat(line, title, 1, 0);
					line = ostrcat(line, "#", 1, 0);
					line = ostrcat(line, oitoa(type), 1, 0);
					line = ostrcat(line, "\n", 1, 0);


					if(url2 != NULL && ostrcmp(url, url2) != 0)
					{
						free(tmpstr2), tmpstr2 = NULL;
						free(hname), hname = NULL;
						if(nolinks == NULL)
							hname = string_resub("&nbsp;", "</a", ret1[i].part, 0);
						else
							hname = string_resub("width=\"16\"> &nbsp;", "</a></td><td align=", nolinks, 0);
						
						int type = 0;
						int count2 = 0;
						struct splitstr* ret2 = NULL;
						ret2 = strsplit(tmpstr3, "/", &count2);
				
						if(ret2 != NULL && count2 > 3 && ostrcmp(hname, "Sockshare") == 0)
						{
							free(hname), hname = NULL;
							hname = ostrcat("Sockshare.com", NULL, 0, 0);
							tmpstr2 = ostrcat(ret2[3].part, NULL, 0, 0);
							type = 14;
						}
						else if(ret2 != NULL && count2 > 3 && ostrcmp(hname, "Putlocker") == 0)
						{
							free(hname), hname = NULL;
							hname = ostrcat("Putlocker.com", NULL, 0, 0);
							tmpstr2 = ostrcat(ret2[3].part, NULL, 0, 0);
							type = 14;
						}
						else if(ret2 != NULL && count2 > 2 && ostrcmp(hname, "Filenuke") == 0)
						{
							free(hname), hname = NULL;
							hname = ostrcat("FileNuke.com", NULL, 0, 0);
							tmpstr2 = ostrcat(ret2[2].part, NULL, 0, 0);
							type = 15;
						}
						else if(ret2 != NULL && count2 > 2 && ostrcmp(hname, "Streamclou") == 0)
						{
							free(hname), hname = NULL;
							hname = ostrcat("StreamCloud.eu", NULL, 0, 0);
							tmpstr2 = ostrcat(ret2[2].part, NULL, 0, 0);
							type = 20;
						}
						else if(ret2 != NULL && count2 > 2 && ostrcmp(hname, "VidStream") == 0)
						{
							free(hname), hname = NULL;
							hname = ostrcat("VidStream.in", NULL, 0, 0);
							tmpstr2 = ostrcat(ret2[2].part, NULL, 0, 0);
							type = 25;
						}
						else if(ret2 != NULL && count2 > 3 && ostrcmp(hname, "Flashx") == 0)
						{
							free(hname), hname = NULL;
							hname = ostrcat("FlashX.tv", NULL, 0, 0);
							tmpstr2 = ostrcat(ret2[3].part, NULL, 0, 0);
							type = 24;
						}
						else if(ret2 != NULL && count2 > 3 && ostrcmp(hname, "PrimeShare") == 0)
						{
							free(hname), hname = NULL;
							hname = ostrcat("PrimeShare.tv", NULL, 0, 0);
							tmpstr2 = ostrcat(ret2[3].part, NULL, 0, 0);
						}				
						else if(ret2 != NULL && count2 > 2 && ostrcmp(hname, "Xvidstage") == 0)
						{
							free(hname), hname = NULL;
							hname = ostrcat("XvidStage.com", NULL, 0, 0);
							tmpstr2 = ostrcat(ret2[2].part, NULL, 0, 0);
							type = 26;
						}
						else if(ret2 != NULL && count2 > 3 && ostrcmp(hname, "Vidxden.com (DivX)") == 0)
						{
							tmpstr2 = ostrcat(ret2[3].part, NULL, 0, 0);
						}
						else if(ret2 != NULL && count2 > 3 && ostrcmp(hname, "Nowvideo") == 0)
						{
							free(hname), hname = NULL;
							hname = ostrcat("NowVideo.eu", NULL, 0, 0);
							tmpstr2 = ostrcat(ret2[3].part, NULL, 0, 0);
							type = 27;
						}
						else if(ret2 != NULL && count2 > 2)
						{
							tmpstr2 = ostrcat(ret2[2].part, NULL, 0, 0);
							hname = ostrcat(hname, " (coming soon)", 1, 0);
							type = 66;
						}
						else
						{
							tmpstr2 = ostrcat("unknown", NULL, 0, 0);
							hname = ostrcat(hname, " (coming soon)", 1, 0);
							type = 66;
						}
												
						free(ret2), ret2 = NULL;
						debug(99, "-------------------------------");
						debug(99, "hname: %s (Part2) url: %s id: %s", hname, url2, tmpstr2);
											
						incount += 1;
						line = ostrcat(line, hname, 1, 0);
						line = ostrcat(line, " (Part2)", 1, 0);					
						line = ostrcat(line, "#", 1, 0);
						line = ostrcat(line, url2, 1, 0);
						line = ostrcat(line, ";", 1, 0);
						line = ostrcat(line, tmpstr2, 1, 0);
						line = ostrcat(line, ";", 1, 0);				
						line = ostrcat(line, hname, 1, 0);
						line = ostrcat(line, "#", 1, 0);
						line = ostrcat(line, "http://atemio.dyndns.tv/mediathek/menu/", 1, 0);
						line = ostrcat(line, hname, 1, 0);
						line = ostrcat(line, ".jpg#kinox_", 1, 0);
						line = ostrcat(line, hname, 1, 0);
						line = ostrcat(line, ".jpg#Movie4k - ", 1, 0);
						line = ostrcat(line, title, 1, 0);
						line = ostrcat(line, "#", 1, 0);
						line = ostrcat(line, oitoa(type), 1, 0);
						line = ostrcat(line, "\n", 1, 0);
						free(tmpstr2), tmpstr2 = NULL;
					}

					if(url3 != NULL && ostrcmp(url, url3) != 0)
					{
						free(tmpstr2), tmpstr2 = NULL;
						free(hname), hname = NULL;
						if(nolinks == NULL)
							hname = string_resub("&nbsp;", "</a", ret1[i].part, 0);
						else
							hname = string_resub("width=\"16\"> &nbsp;", "</a></td><td align=", nolinks, 0);

						int type = 0;
						int count2 = 0;
						struct splitstr* ret2 = NULL;
						ret2 = strsplit(tmpstr4, "/", &count2);
				
						if(ret2 != NULL && count2 > 3 && ostrcmp(hname, "Sockshare") == 0)
						{
							free(hname), hname = NULL;
							hname = ostrcat("Sockshare.com", NULL, 0, 0);
							tmpstr2 = ostrcat(ret2[3].part, NULL, 0, 0);
							type = 14;
						}
						else if(ret2 != NULL && count2 > 3 && ostrcmp(hname, "Putlocker") == 0)
						{
							free(hname), hname = NULL;
							hname = ostrcat("Putlocker.com", NULL, 0, 0);
							tmpstr2 = ostrcat(ret2[3].part, NULL, 0, 0);
							type = 14;
						}
						else if(ret2 != NULL && count2 > 2 && ostrcmp(hname, "Filenuke") == 0)
						{
							free(hname), hname = NULL;
							hname = ostrcat("FileNuke.com", NULL, 0, 0);
							tmpstr2 = ostrcat(ret2[2].part, NULL, 0, 0);
							type = 15;
						}
						else if(ret2 != NULL && count2 > 2 && ostrcmp(hname, "Streamclou") == 0)
						{
							free(hname), hname = NULL;
							hname = ostrcat("StreamCloud.eu", NULL, 0, 0);
							tmpstr2 = ostrcat(ret2[2].part, NULL, 0, 0);
							type = 20;
						}
						else if(ret2 != NULL && count2 > 2 && ostrcmp(hname, "VidStream") == 0)
						{
							free(hname), hname = NULL;
							hname = ostrcat("VidStream.in", NULL, 0, 0);
							tmpstr2 = ostrcat(ret2[2].part, NULL, 0, 0);
							type = 25;
						}
						else if(ret2 != NULL && count2 > 3 && ostrcmp(hname, "Flashx") == 0)
						{
							free(hname), hname = NULL;
							hname = ostrcat("FlashX.tv", NULL, 0, 0);
							tmpstr2 = ostrcat(ret2[3].part, NULL, 0, 0);
							type = 24;
						}
						else if(ret2 != NULL && count2 > 3 && ostrcmp(hname, "PrimeShare") == 0)
						{
							free(hname), hname = NULL;
							hname = ostrcat("PrimeShare.tv", NULL, 0, 0);
							tmpstr2 = ostrcat(ret2[3].part, NULL, 0, 0);
						}				
						else if(ret2 != NULL && count2 > 2 && ostrcmp(hname, "Xvidstage") == 0)
						{
							free(hname), hname = NULL;
							hname = ostrcat("XvidStage.com", NULL, 0, 0);
							tmpstr2 = ostrcat(ret2[2].part, NULL, 0, 0);
							type = 26;
						}
						else if(ret2 != NULL && count2 > 3 && ostrcmp(hname, "Vidxden.com (DivX)") == 0)
						{
							tmpstr2 = ostrcat(ret2[3].part, NULL, 0, 0);
						}
						else if(ret2 != NULL && count2 > 3 && ostrcmp(hname, "Nowvideo") == 0)
						{
							free(hname), hname = NULL;
							hname = ostrcat("NowVideo.eu", NULL, 0, 0);
							tmpstr2 = ostrcat(ret2[3].part, NULL, 0, 0);
							type = 27;
						}
						else if(ret2 != NULL && count2 > 2)
						{
							tmpstr2 = ostrcat(ret2[2].part, NULL, 0, 0);
							hname = ostrcat(hname, " (coming soon)", 1, 0);
							type = 66;
						}
						else
						{
							tmpstr2 = ostrcat("unknown", NULL, 0, 0);
							hname = ostrcat(hname, " (coming soon)", 1, 0);
							type = 66;
						}
												
						free(ret2), ret2 = NULL;
						debug(99, "-------------------------------");
						debug(99, "hname: %s (Part3) url: %s id: %s", hname, url2, tmpstr2);
											
						incount += 1;
						line = ostrcat(line, hname, 1, 0);
						line = ostrcat(line, " (Part3)", 1, 0);					
						line = ostrcat(line, "#", 1, 0);
						line = ostrcat(line, url3, 1, 0);
						line = ostrcat(line, ";", 1, 0);
						line = ostrcat(line, tmpstr2, 1, 0);
						line = ostrcat(line, ";", 1, 0);				
						line = ostrcat(line, hname, 1, 0);
						line = ostrcat(line, "#", 1, 0);
						line = ostrcat(line, "http://atemio.dyndns.tv/mediathek/menu/", 1, 0);
						line = ostrcat(line, hname, 1, 0);
						line = ostrcat(line, ".jpg#movie4k_", 1, 0);
						line = ostrcat(line, hname, 1, 0);
						line = ostrcat(line, ".jpg#Movie4k - ", 1, 0);
						line = ostrcat(line, title, 1, 0);
						line = ostrcat(line, "#", 1, 0);
						line = ostrcat(line, oitoa(type), 1, 0);
						line = ostrcat(line, "\n", 1, 0);
						free(tmpstr2), tmpstr2 = NULL;
					}

					if(url4 != NULL && ostrcmp(url, url4) != 0)
					{
						free(tmpstr2), tmpstr2 = NULL;
						free(hname), hname = NULL;
						if(nolinks == NULL)
							hname = string_resub("&nbsp;", "</a", ret1[i].part, 0);
						else
							hname = string_resub("width=\"16\"> &nbsp;", "</a></td><td align=", nolinks, 0);

						int type = 0;
						int count2 = 0;
						struct splitstr* ret2 = NULL;
						ret2 = strsplit(tmpstr5, "/", &count2);
				
						if(ret2 != NULL && count2 > 3 && ostrcmp(hname, "Sockshare") == 0)
						{
							free(hname), hname = NULL;
							hname = ostrcat("Sockshare.com", NULL, 0, 0);
							tmpstr2 = ostrcat(ret2[3].part, NULL, 0, 0);
							type = 14;
						}
						else if(ret2 != NULL && count2 > 3 && ostrcmp(hname, "Putlocker") == 0)
						{
							free(hname), hname = NULL;
							hname = ostrcat("Putlocker.com", NULL, 0, 0);
							tmpstr2 = ostrcat(ret2[3].part, NULL, 0, 0);
							type = 14;
						}
						else if(ret2 != NULL && count2 > 2 && ostrcmp(hname, "Filenuke") == 0)
						{
							free(hname), hname = NULL;
							hname = ostrcat("FileNuke.com", NULL, 0, 0);
							tmpstr2 = ostrcat(ret2[2].part, NULL, 0, 0);
							type = 15;
						}
						else if(ret2 != NULL && count2 > 2 && ostrcmp(hname, "Streamclou") == 0)
						{
							free(hname), hname = NULL;
							hname = ostrcat("StreamCloud.eu", NULL, 0, 0);
							tmpstr2 = ostrcat(ret2[2].part, NULL, 0, 0);
							type = 20;
						}
						else if(ret2 != NULL && count2 > 2 && ostrcmp(hname, "VidStream") == 0)
						{
							free(hname), hname = NULL;
							hname = ostrcat("VidStream.in", NULL, 0, 0);
							tmpstr2 = ostrcat(ret2[2].part, NULL, 0, 0);
							type = 25;
						}
						else if(ret2 != NULL && count2 > 3 && ostrcmp(hname, "Flashx") == 0)
						{
							free(hname), hname = NULL;
							hname = ostrcat("FlashX.tv", NULL, 0, 0);
							tmpstr2 = ostrcat(ret2[3].part, NULL, 0, 0);
							type = 24;
						}
						else if(ret2 != NULL && count2 > 3 && ostrcmp(hname, "PrimeShare") == 0)
						{
							free(hname), hname = NULL;
							hname = ostrcat("PrimeShare.tv", NULL, 0, 0);
							tmpstr2 = ostrcat(ret2[3].part, NULL, 0, 0);
						}				
						else if(ret2 != NULL && count2 > 2 && ostrcmp(hname, "Xvidstage") == 0)
						{
							free(hname), hname = NULL;
							hname = ostrcat("XvidStage.com", NULL, 0, 0);
							tmpstr2 = ostrcat(ret2[2].part, NULL, 0, 0);
							type = 26;
						}
						else if(ret2 != NULL && count2 > 3 && ostrcmp(hname, "Vidxden.com (DivX)") == 0)
						{
							tmpstr2 = ostrcat(ret2[3].part, NULL, 0, 0);
						}
						else if(ret2 != NULL && count2 > 3 && ostrcmp(hname, "Nowvideo") == 0)
						{
							free(hname), hname = NULL;
							hname = ostrcat("NowVideo.eu", NULL, 0, 0);
							tmpstr2 = ostrcat(ret2[3].part, NULL, 0, 0);
							type = 27;
						}
						else if(ret2 != NULL && count2 > 2)
						{
							tmpstr2 = ostrcat(ret2[2].part, NULL, 0, 0);
							hname = ostrcat(hname, " (coming soon)", 1, 0);
							type = 66;
						}
						else
						{
							tmpstr2 = ostrcat("unknown", NULL, 0, 0);
							hname = ostrcat(hname, " (coming soon)", 1, 0);
							type = 66;
						}
												
						free(ret2), ret2 = NULL;
						debug(99, "-------------------------------");
						debug(99, "hname: %s (Part3) url: %s id: %s", hname, url2, tmpstr2);
											
						incount += 1;
						line = ostrcat(line, hname, 1, 0);
						line = ostrcat(line, " (Part4)", 1, 0);					
						line = ostrcat(line, "#", 1, 0);
						line = ostrcat(line, url4, 1, 0);
						line = ostrcat(line, ";", 1, 0);
						line = ostrcat(line, tmpstr2, 1, 0);
						line = ostrcat(line, ";", 1, 0);				
						line = ostrcat(line, hname, 1, 0);
						line = ostrcat(line, "#", 1, 0);
						line = ostrcat(line, "http://atemio.dyndns.tv/mediathek/menu/", 1, 0);
						line = ostrcat(line, hname, 1, 0);
						line = ostrcat(line, ".jpg#movie4k_", 1, 0);
						line = ostrcat(line, hname, 1, 0);
						line = ostrcat(line, ".jpg#Movie4k - ", 1, 0);
						line = ostrcat(line, title, 1, 0);
						line = ostrcat(line, "#", 1, 0);
						line = ostrcat(line, oitoa(type), 1, 0);
						line = ostrcat(line, "\n", 1, 0);
						free(tmpstr2), tmpstr2 = NULL;
					}
					if(nolinks != NULL)
					{
						printf("break\n");
						break;
					}
				}

				free(tmpstr1), tmpstr1 = NULL;
				free(tmpstr2), tmpstr2 = NULL;
				free(tmpstr3), tmpstr3 = NULL;
				free(tmpstr4), tmpstr4 = NULL;
				free(tmpstr5), tmpstr5 = NULL;
				free(hname), hname = NULL;
				free(hnr), hnr = NULL;
				free(hlink), hlink = NULL;										
				free(url), url = NULL;
				free(url2), url2 = NULL;
				free(url3), url3 = NULL;
				free(url4), url4 = NULL;
				free(pathnew), pathnew = NULL;
				free(logfile), logfile = NULL;					
			}
		}
		free(ret1), ret1 = NULL;
	}
	free(tmpstr), tmpstr = NULL;	

	if(line != NULL)
	{
		if(series == 0)
			tmpstr = ostrcat("/tmp/tithek/movie4k.hoster.list", NULL, 0, 0);
		else
			tmpstr = ostrcat("/tmp/tithek/movie4k.hoster.ser.list", NULL, 0, 0);
		writesys(tmpstr, line, 0);

		if(getconfigint("debuglevel", NULL) == 99)
			writesys("/tmp/movie4k8_line", line, 0);
					
		struct tithek* tnode = (struct tithek*)listbox->select->handle;
		createtithek(tnode, tnode->title,  tmpstr, tnode->pic, tnode->localname, tnode->menutitle, tnode->flag);
		ret = 0;
	}

	return ret;
}

int movie4k_hoster_series(struct skin* grid, struct skin* listbox, struct skin* countlabel, struct skin* load, char* link, char* title)
{
	debug(99, "link: %s", link);
	int ret = 1, series = 0;
	char* ip = NULL, *pos = NULL, *path = NULL, *tmpstr = NULL, *line = NULL;

	if(listbox == NULL || listbox->select == NULL || listbox->select->handle == NULL)
		return ret;

	ip = string_replace("http://", "", (char*)link, 0);

	if(ip != NULL)
		pos = strchr(ip, '/');
	if(pos != NULL)
	{
		pos[0] = '\0';
		path = pos + 1;
	}

	tmpstr = gethttp(ip, path, 80, NULL, NULL, 10000, NULL, 0);
	if(getconfigint("debuglevel", NULL) == 99)
		writesys("/tmp/movie4k2_tmpstr", tmpstr, 0);

	if(tmpstr != NULL)
	{
		drawscreen(load, 0, 0);

		if(ostrstr(tmpstr, "episodeform") != NULL)
		{
			char* from = NULL;
			char* folgen = NULL;
			char* folgentmp = NULL;
			char* id = NULL;

			int i;

			series = 1;
	
			for(i = 1; i < 30; i++)
			{
				from = ostrcat(from, "<FORM name=\"episodeform", 1, 0);
				from = ostrcat(from, oitoa(i), 1, 0);
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
							id = oregex(".*tvshows-(.*[0-9]{1,10})-.*", link);

							debug(99, "(S%d/E%d) link: %s id: %s", i, j, link, id);
							link = string_replace("tvshows-", "", link, 1);
							link = string_replace(id, "", link, 1);
							link = string_replace("-", "", link, 1);
							link = string_replace(".html", "", link, 1);
							link = ostrcat(link, "-online-serie-", 1, 0);
							link = ostrcat(link, id, 1, 0);
							link = ostrcat(link, ".html", 1, 0);
							debug(99, "(S%d/E%d) convertlink: %s id: %s", i, j, link, id);

							incount += 1;
							line = ostrcat(line, _("Season"), 1, 0);
							line = ostrcat(line, " ", 1, 0);
							line = ostrcat(line, oitoa(i), 1, 0);
							line = ostrcat(line, " ", 1, 0);
							line = ostrcat(line, _("Episode"), 1, 0);
							line = ostrcat(line, " ", 1, 0);
							line = ostrcat(line, oitoa(j), 1, 0);					
							line = ostrcat(line, "#http://www.movie4k.to/", 1, 0);
							line = ostrcat(line, link, 1, 0);
//							line = ostrcat(line, ";", 1, 0);
//							line = ostrcat(line, oitoa(i), 1, 0);
//							line = ostrcat(line, ";", 1, 0);
//							line = ostrcat(line, oitoa(j), 1, 0);
							line = ostrcat(line, "#", 1, 0);
							line = ostrcat(line, "http://atemio.dyndns.tv/mediathek/menu/s", 1, 0);
							line = ostrcat(line, oitoa(i), 1, 0);
							line = ostrcat(line, "e", 1, 0);
							line = ostrcat(line, oitoa(j), 1, 0);
							line = ostrcat(line, ".jpg", 1, 0);																
							line = ostrcat(line, "#movie4k_search_", 1, 0);
							line = ostrcat(line, oitoa(i), 1, 0);
							line = ostrcat(line, "e", 1, 0);
							line = ostrcat(line, oitoa(j), 1, 0);
							line = ostrcat(line, ".jpg#Movie4k - ", 1, 0);
							line = ostrcat(line, title, 1, 0);
							line = ostrcat(line, "#34\n", 1, 0);
							free(id), id = NULL;
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
		if(series == 0)
			tmpstr = ostrcat("/tmp/tithek/movie4k.hoster.list", NULL, 0, 0);
		else
			tmpstr = ostrcat("/tmp/tithek/movie4k.hoster.ser.list", NULL, 0, 0);
		writesys(tmpstr, line, 0);

		if(getconfigint("debuglevel", NULL) == 99)
			writesys("/tmp/movie4k8_line", line, 0);
					
		struct tithek* tnode = (struct tithek*)listbox->select->handle;
		createtithek(tnode, tnode->title,  tmpstr, tnode->pic, tnode->localname, tnode->menutitle, tnode->flag);
		ret = 0;
	}
	return ret;
}

#endif