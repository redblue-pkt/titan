#ifndef KINOX_H
#define KINOX_H

// flag 1 = putlocker/sockshare
// flag 2 = filenuke

char* kinox(char* link)
{
	debug(99, "link: %s", link);
	char* streamurl = NULL;

	streamurl = hoster(link);
	debug(99, "streamurl1: %s", streamurl);

	streamurl = string_replace_all("amp;", "", streamurl, 1);
	debug(99, "streamurl2: %s", streamurl);

	return streamurl;
}

int kinox_search(struct skin* grid, struct skin* listbox, struct skin* countlabel, struct skin* load, char* link, char* title, char* searchstr, int flag)
{
	int ret = 1;
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

		search = strstrip(search);
		search = stringreplacechar(search, ' ', '+');
		debug(99, "search: %s", search);

		ip = ostrcat("kinox.to", NULL, 0, 0);
		if(flag == 0)
			path = ostrcat("Search.html?q=", search, 0, 0);
		else
			path = string_replace_all("http://kinox.to/", "", search, 0);
		
		tmpstr = gethttp(ip, path, 80, NULL, NULL, 10000, NULL, 0);
		tmpstr = string_resub("<div id=\"beep\" class=\"beep\"></div>", "</table>", tmpstr, 0);

		titheklog(debuglevel, "/tmp/kinox1_tmpstr", NULL, tmpstr);
		
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
					int rcret = waitrc(NULL, 10, 0);
					if(rcret == getrcconfigint("rcexit", NULL)) break;
					free(path), path = NULL;
					path = oregex(".*<td class=\"Title\"><a href=\"(.*)\" onclick=.*", ret1[i].part);

					if(ostrstr(langck, "1.png") != NULL)
						lang = ostrcat(lang, " (de)", 1, 0);
					else if(ostrstr(langck, "2.png") != NULL)
						lang = ostrcat(lang, " (en)", 1, 0);
					else
						lang = ostrcat(lang, " (\?\?)", 1, 0);
																	
					tmpstr1 = gethttp("kinox.to", path, 80, NULL, NULL, 10000, NULL, 0);

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

					url = ostrcat("http://kinox.to", path, 0, 0);

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
						line = ostrcat(line, "#kinox_search_", 1, 0);
						line = ostrcat(line, oitoa(incount + time(NULL)), 1, 1);
						line = ostrcat(line, ".jpg#KinoX - Search#22\n", 1, 0);
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
				line = string_replace_all("http://atemio.dyndns.tv/", "http://imageshack.us/md/up/grd/", line, 1);
				menu = ostrcat("/tmp/tithek/kinox.search.", oitoa(time(NULL)), 0, 1);
				menu = ostrcat(menu, ".list", 1, 0);

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

int kinox_search_local(struct skin* grid, struct skin* listbox, struct skin* countlabel, struct skin* load, char* link, char* title, char* searchstr, int flag)
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

		tmpstr = gethttp("atemio.dyndns.tv", "/mediathek/kinox/streams/kinox.all-sorted.list", 80, NULL, HTTPAUTH, 5000, NULL, 0);

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
				line = string_replace_all("http://atemio.dyndns.tv/", "http://imageshack.us/md/up/grd/", line, 1);
				menu = ostrcat("/tmp/tithek/kinox.search.", oitoa(time(NULL)), 0, 1);
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

int kinox_hoster(struct skin* grid, struct skin* listbox, struct skin* countlabel, struct skin* load, char* link, char* title)
{
	debug(99, "link: %s", link);
	int debuglevel = getconfigint("debuglevel", NULL);
	int ret = 1, series = 0;
	char* ip = NULL, *pos = NULL, *path = NULL, *tmpstr = NULL, *tmpstr1 = NULL, *tmpstr2 = NULL, *tmpstr3 = NULL, *pichname = NULL;
	char* tmpstr4 = NULL, *tmpstr5 = NULL, *line = NULL, *url = NULL, *url2 = NULL, *url3 = NULL, *url4 = NULL, *pathnew = NULL, *extra = NULL;

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
	titheklog(debuglevel, "/tmp/kinox2_tmpstr", NULL, tmpstr);
	
	if(tmpstr != NULL)
	{
		drawscreen(load, 0, 0);
		if(ostrstr(tmpstr, "SeriesID") != NULL)
		{
			char* pos1 = NULL;
			char* from = NULL;
			char* folgen = NULL;
			int i;

			series = 1;
	
			for(i = 1; i < 30; i++)
			{
				from = ostrcat(from, "<option value=\"", 1, 0);
				from = ostrcat(from, oitoa(i), 1, 0);
				from = ostrcat(from, "\" rel=", 1, 0);
			
				pos1 = ostrstr(tmpstr, from);
				folgen = getxmlentry(ostrstr(tmpstr, from), "rel=");
				printf("Season %d folgen: %s\n",i , folgen);	

				if(folgen != NULL)
				{
					int count = 0;
					int incount = 0;
					int j;
					struct splitstr* ret1 = NULL;
					ret1 = strsplit(folgen, ",", &count);
						
					if(ret1 != NULL && count > 0)
					{
						int max = count + 1;
						for(j = 1; j < max; j++)
						{							
							incount += 1;
							line = ostrcat(line, _("Season"), 1, 0);
							line = ostrcat(line, " ", 1, 0);
							line = ostrcat(line, oitoa(i), 1, 0);
							line = ostrcat(line, " ", 1, 0);
							line = ostrcat(line, _("Episode"), 1, 0);
							line = ostrcat(line, " ", 1, 0);
							line = ostrcat(line, oitoa(j), 1, 0);					
							line = ostrcat(line, "#", 1, 0);
							line = ostrcat(line, link, 1, 0);
							line = ostrcat(line, ";", 1, 0);
							line = ostrcat(line, oitoa(i), 1, 0);
							line = ostrcat(line, ";", 1, 0);
							line = ostrcat(line, oitoa(j), 1, 0);
							line = ostrcat(line, "#", 1, 0);
							line = ostrcat(line, "http://atemio.dyndns.tv/mediathek/menu/s", 1, 0);
							line = ostrcat(line, oitoa(i), 1, 0);
							line = ostrcat(line, "e", 1, 0);
							line = ostrcat(line, oitoa(j), 1, 0);
							line = ostrcat(line, ".jpg", 1, 0);																
							line = ostrcat(line, "#kinox_search_", 1, 0);
							line = ostrcat(line, oitoa(i), 1, 0);
							line = ostrcat(line, "e", 1, 0);
							line = ostrcat(line, oitoa(j), 1, 0);
							line = ostrcat(line, ".jpg#KinoX - ", 1, 0);
							line = ostrcat(line, title, 1, 0);
							line = ostrcat(line, "#23\n", 1, 0);
						}
					}
					free(ret1), ret1 = NULL;
				}				
				free(from), from = NULL;
				free(folgen), folgen = NULL;
			}
		}
		else
		{
			tmpstr = string_resub("<ul id=\"HosterList\" class=\"Sortable\">", "</ul>", tmpstr, 0);
			titheklog(debuglevel, "/tmp/kinox3_tmpstr", NULL, tmpstr);
	
			int type = 14;
			int count = 0;
			int incount = 0;
			int i;
			struct splitstr* ret1 = NULL;
			ret1 = strsplit(tmpstr, "\n", &count);
	
			char* hname = NULL;
			char* hnr = NULL;
			char* hlink = NULL;

			if(ret1 != NULL && count > 0)
			{
				int max = count;
				for(i = 0; i < max; i++)
				{
//					printf("ret1[i].part: %s\n",ret1[i].part);
					hnr = string_resub("<li id=\"Hoster_", "\"", ret1[i].part, 0);
					hlink = string_resub("rel=\"", "\">", ret1[i].part, 0);
					hlink = string_replace_all("amp;", "", hlink, 1);
	
					hname = string_resub("<div class=\"Named\">", "</div>", ret1[i].part, 0);
					pathnew = ostrcat("/aGET/Mirror/", hlink, 0, 0);

					pichname = ostrcat(hname, NULL, 0, 0);
					string_tolower(pichname);
					pichname = stringreplacecharonce(pichname, '.', '\0');

					extra = string_resub("<b>Vom</b>: ", "</div>", ret1[i].part, 0);

					titheklog(debuglevel, "/tmp/kinox4_pathnew1", hname, pathnew);
					
					tmpstr1 = gethttp("kinox.to", pathnew, 80, NULL, NULL, 10000, NULL, 0);
					free(pathnew), pathnew = NULL;

					titheklog(debuglevel, "/tmp/kinox5_tmpstr1", hname, tmpstr1);

					tmpstr1 = string_replace_all("\\", "", tmpstr1, 1);
					tmpstr1 = string_resub("<a href=\"", "\"", tmpstr1, 0);
	
					url = ostrcat(tmpstr1, NULL, 0, 0);
////////////////
					free(pathnew), pathnew = NULL;
					pathnew = ostrcat("/aGET/Mirror/", hlink, 0, 0);
					pathnew = ostrcat(pathnew, "&Part=2", 1, 0);

					titheklog(debuglevel, "/tmp/kinox6_pathnew2", hname, pathnew);
					
					tmpstr3 = gethttp("kinox.to", pathnew, 80, NULL, NULL, 10000, NULL, 0);
					free(pathnew), pathnew = NULL;

					titheklog(debuglevel, "/tmp/kinox7_tmpstr3", hname, tmpstr3);

					tmpstr3 = string_replace_all("\\", "", tmpstr3, 1);
					tmpstr3 = string_resub("<a href=\"", "\"", tmpstr3, 0);
					
					url2 = ostrcat(tmpstr3, NULL, 0, 0);
////////////////
					free(pathnew), pathnew = NULL;
					pathnew = ostrcat("/aGET/Mirror/", hlink, 0, 0);
					pathnew = ostrcat(pathnew, "&Part=3", 1, 0);

					titheklog(debuglevel, "/tmp/kinox6_pathnew3", hname, pathnew);
					
					tmpstr4 = gethttp("kinox.to", pathnew, 80, NULL, NULL, 10000, NULL, 0);
					free(pathnew), pathnew = NULL;

					titheklog(debuglevel, "/tmp/kinox7_tmpstr4", hname, tmpstr4);

					tmpstr4 = string_replace_all("\\", "", tmpstr4, 1);
					tmpstr4 = string_resub("<a href=\"", "\"", tmpstr4, 0);
					
					url3 = ostrcat(tmpstr4, NULL, 0, 0);
////////////////
					free(pathnew), pathnew = NULL;
					pathnew = ostrcat("/aGET/Mirror/", hlink, 0, 0);
					pathnew = ostrcat(pathnew, "&Part=4", 1, 0);

					titheklog(debuglevel, "/tmp/kinox6_pathnew4", hname, pathnew);
					
					tmpstr5 = gethttp("kinox.to", pathnew, 80, NULL, NULL, 10000, NULL, 0);
					free(pathnew), pathnew = NULL;

					titheklog(debuglevel, "/tmp/kinox7_tmpstr5", hname, tmpstr5);

					tmpstr5 = string_replace_all("\\", "", tmpstr5, 1);
					tmpstr5 = string_resub("<a href=\"", "\"", tmpstr5, 0);
					
					url4 = ostrcat(tmpstr5, NULL, 0, 0);
////////////////
					type = 14;

					debug(99, "-------------------------------");
					if(ostrcmp(url, url2) != 0)
					{
						debug(99, "(%d/%d) %s (Part1) url: %s extra: %s",i ,max , hname, url, extra);
					}
					else
					{
						debug(99, "(%d/%d) %s url: %s extra: %s",i ,max , hname, url, extra);
					}
					
					incount += 1;
					line = ostrcat(line, hname, 1, 0);
					if(ostrcmp(url, url2) != 0)
						line = ostrcat(line, " (Part1)", 1, 0);
					if(extra != NULL)
					{
						line = ostrcat(line, " (", 1, 0);					
						line = ostrcat(line, extra, 1, 0);					
						line = ostrcat(line, ")", 1, 0);					
					}				
					line = ostrcat(line, "#", 1, 0);
					line = ostrcat(line, url, 1, 0);
					line = ostrcat(line, "#", 1, 0);
					line = ostrcat(line, "http://atemio.dyndns.tv/mediathek/menu/", 1, 0);
					line = ostrcat(line, pichname, 1, 0);
					line = ostrcat(line, ".jpg#kinox_", 1, 0);
					line = ostrcat(line, pichname, 1, 0);
					line = ostrcat(line, ".jpg#KinoX - ", 1, 0);
					line = ostrcat(line, title, 1, 0);
					line = ostrcat(line, "#", 1, 0);
					line = ostrcat(line, oitoa(type), 1, 0);
					line = ostrcat(line, "\n", 1, 0);

					if(ostrcmp(url, url2) != 0)
					{
						free(tmpstr2), tmpstr2 = NULL;
						free(hname), hname = NULL;
						hname = string_resub("<div class=\"Named\">", "</div>", ret1[i].part, 0);
						pichname = ostrcat(hname, NULL, 0, 0);
						string_tolower(pichname);
						pichname = stringreplacecharonce(pichname, '\n', '\0');

						type = 14;

						debug(99, "-------------------------------");
						debug(99, "(%d/%d) %s (Part2) url: %s extra: %s",i ,max, hname, url2, extra);

						incount += 1;
						line = ostrcat(line, hname, 1, 0);
						line = ostrcat(line, " (Part2)", 1, 0);
						if(extra != NULL)
						{
							line = ostrcat(line, " (", 1, 0);					
							line = ostrcat(line, extra, 1, 0);					
							line = ostrcat(line, ")", 1, 0);					
						}
						line = ostrcat(line, "#", 1, 0);
						line = ostrcat(line, url2, 1, 0);
						line = ostrcat(line, "#", 1, 0);
						line = ostrcat(line, "http://atemio.dyndns.tv/mediathek/menu/", 1, 0);
						line = ostrcat(line, pichname, 1, 0);
						line = ostrcat(line, ".jpg#kinox_", 1, 0);
						line = ostrcat(line, pichname, 1, 0);
						line = ostrcat(line, ".jpg#KinoX - ", 1, 0);
						line = ostrcat(line, title, 1, 0);
						line = ostrcat(line, "#", 1, 0);
						line = ostrcat(line, oitoa(type), 1, 0);
						line = ostrcat(line, "\n", 1, 0);
						free(tmpstr2), tmpstr2 = NULL;
					}

					if(ostrcmp(url, url3) != 0 && ostrcmp(url2, url3) != 0)
					{
						free(tmpstr2), tmpstr2 = NULL;
						free(hname), hname = NULL;
						hname = string_resub("<div class=\"Named\">", "</div>", ret1[i].part, 0);
						pichname = ostrcat(hname, NULL, 0, 0);
						string_tolower(pichname);
						pichname = stringreplacecharonce(pichname, '\n', '\0');
						type = 14;

						debug(99, "-------------------------------");
						debug(99, "(%d/%d) %s (Part3) url: %s extra: %s",i ,max , hname, url3, extra);
											
						incount += 1;
						line = ostrcat(line, hname, 1, 0);
						if(extra != NULL)
						line = ostrcat(line, " (Part3)", 1, 0);					
						if(extra != NULL)
						{
							line = ostrcat(line, " (", 1, 0);					
							line = ostrcat(line, extra, 1, 0);					
							line = ostrcat(line, ")", 1, 0);					
						}
						line = ostrcat(line, "#", 1, 0);
						line = ostrcat(line, url3, 1, 0);
						line = ostrcat(line, "#", 1, 0);
						line = ostrcat(line, "http://atemio.dyndns.tv/mediathek/menu/", 1, 0);
						line = ostrcat(line, pichname, 1, 0);
						line = ostrcat(line, ".jpg#kinox_", 1, 0);
						line = ostrcat(line, pichname, 1, 0);
						line = ostrcat(line, ".jpg#KinoX - ", 1, 0);
						line = ostrcat(line, title, 1, 0);
						line = ostrcat(line, "#", 1, 0);
						line = ostrcat(line, oitoa(type), 1, 0);
						line = ostrcat(line, "\n", 1, 0);
						free(tmpstr2), tmpstr2 = NULL;
					}

					if(ostrcmp(url, url4) != 0 && ostrcmp(url2, url4) != 0 && ostrcmp(url3, url4) != 0)
					{
						free(tmpstr2), tmpstr2 = NULL;
						free(hname), hname = NULL;
						hname = string_resub("<div class=\"Named\">", "</div>", ret1[i].part, 0);
						pichname = ostrcat(hname, NULL, 0, 0);
						string_tolower(pichname);
						pichname = stringreplacecharonce(pichname, '\n', '\0');
						type = 14;

						debug(99, "-------------------------------");
						debug(99, "(%d/%d) %s (Part4) url: %s extra: %s",i ,max , hname, url4, extra);

						incount += 1;
						line = ostrcat(line, hname, 1, 0);
						line = ostrcat(line, " (Part4)", 1, 0);
						if(extra != NULL)
						{
							line = ostrcat(line, " (", 1, 0);					
							line = ostrcat(line, extra, 1, 0);					
							line = ostrcat(line, ")", 1, 0);					
						}		
						line = ostrcat(line, "#", 1, 0);
						line = ostrcat(line, url4, 1, 0);
						line = ostrcat(line, "#", 1, 0);
						line = ostrcat(line, "http://atemio.dyndns.tv/mediathek/menu/", 1, 0);
						line = ostrcat(line, pichname, 1, 0);
						line = ostrcat(line, ".jpg#kinox_", 1, 0);
						line = ostrcat(line, pichname, 1, 0);
						line = ostrcat(line, ".jpg#KinoX - ", 1, 0);
						line = ostrcat(line, title, 1, 0);
						line = ostrcat(line, "#", 1, 0);
						line = ostrcat(line, oitoa(type), 1, 0);
						line = ostrcat(line, "\n", 1, 0);
						free(tmpstr2), tmpstr2 = NULL;
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
					free(pichname), pichname = NULL;
					free(extra), extra = NULL;
				}
			}
			free(ret1), ret1 = NULL;
		}
	}
	free(tmpstr), tmpstr = NULL;	

	if(line != NULL)
	{
		line = string_replace_all("http://atemio.dyndns.tv/", "http://imageshack.us/md/up/grd/", line, 1);
		if(series == 0)
		{
			tmpstr = ostrcat("/tmp/tithek/kinox.hoster.", oitoa(time(NULL)), 0, 1);
			tmpstr = ostrcat(tmpstr, ".list", 1, 0);
		}
		else
		{
			tmpstr = ostrcat("/tmp/tithek/kinox.hoster.ser.", oitoa(time(NULL)), 0, 1);
			tmpstr = ostrcat(tmpstr, ".list", 1, 0);
		}
		writesys(tmpstr, line, 0);

		titheklog(debuglevel, "/tmp/kinox8_line", NULL, line);
					
		struct tithek* tnode = (struct tithek*)listbox->select->handle;
		createtithek(tnode, tnode->title,  tmpstr, tnode->pic, tnode->localname, tnode->menutitle, tnode->flag);
		ret = 0;
	}

	return ret;
}

int kinox_hoster_series(struct skin* grid, struct skin* listbox, struct skin* countlabel, struct skin* load, char* link, char* title)
{
	debug(99, "link: %s", link);
	int debuglevel = getconfigint("debuglevel", NULL);
	int ret = 1;
	char* ip = NULL, *pathnew = NULL, *seriesid = NULL, *searchname = NULL, *url = NULL, *session = NULL, *episode = NULL, *pos = NULL, *path = NULL, *tmpstr = NULL, *tmpstr1 = NULL, *tmpstr2 = NULL, *line = NULL;		
	char* pichname = NULL, *extra= NULL;
	if(listbox == NULL || listbox->select == NULL || listbox->select->handle == NULL)
		return ret;

	int count0 = 0;
	struct splitstr* ret0 = NULL;
	ret0 = strsplit(link, ";", &count0);
	if(ret0 != NULL && count0 >= 3)
	{
		url = ostrcat(url, ret0[0].part, 1, 0);
		debug(99, "url: %s", url);

		session = ostrcat(session, ret0[1].part, 1, 0);
		debug(99, "Season: %s", session);

		episode = ostrcat(episode, ret0[2].part, 1, 0);
		debug(99, "episode: %s", episode);

		searchname = ostrcat(searchname, ret0[0].part, 1, 0);
		searchname = string_replace("http://kinox.to//Stream/", "", searchname, 0);
		searchname = string_replace("http://kinox.to/Stream/", "", searchname, 0);
		searchname = string_replace(".html", "", searchname, 0);
		debug(99, "searchname: %s", searchname);
	}
	free(ret0), ret0 = NULL;

	ip = string_replace("http://", "", url, 0);

	if(ip != NULL)
		pos = strchr(ip, '/');
	if(pos != NULL)
	{
		pos[0] = '\0';
		path = pos + 1;
	}

	tmpstr = gethttp(ip, path, 80, NULL, NULL, 10000, NULL, 0);

	if(tmpstr != NULL)
	{
		drawscreen(load, 0, 0);
		
		seriesid = string_resub("SeriesID=", "\"", tmpstr, 0);
		debug(99, "seriesid: %s", seriesid);
			
		pathnew = ostrcat("/aGET/MirrorByEpisode/&?Addr=", searchname, 0, 0);
		pathnew = ostrcat(pathnew, "&SeriesID=", 1, 0);
		pathnew = ostrcat(pathnew, seriesid, 1, 0);
		pathnew = ostrcat(pathnew, "&Season=", 1, 0);
		pathnew = ostrcat(pathnew, session, 1, 0);
		pathnew = ostrcat(pathnew, "&Episode=", 1, 0);
		pathnew = ostrcat(pathnew, episode, 1, 0);
		debug(99, "pathnew: %s", pathnew);
	
		free(tmpstr), tmpstr = NULL;
		tmpstr = gethttp("kinox.to", pathnew, 80, NULL, NULL, 10000, NULL, 0);
		free(pathnew), pathnew = NULL;
	
		if(tmpstr != NULL)
		{
			tmpstr = string_resub("<ul id=\"HosterList\" class=\"Sortable\">", "</ul>", tmpstr, 0);
		
			int type = 14;
			int count = 0;
			int incount = 0;
			int i;
			struct splitstr* ret1 = NULL;
			ret1 = strsplit(tmpstr, "\n", &count);
		
			char* hname = NULL;
			char* hnr = NULL;
			char* hlink = NULL;
			
			if(ret1 != NULL && count > 0)
			{
				int max = count;
				for(i = 0; i < max; i++)
				{
					hnr = string_resub("<li id=\"Hoster_", "\"", ret1[i].part, 0);
					hlink = string_resub("rel=\"", "\">", ret1[i].part, 0);
					hlink = string_replace_all("amp;", "", hlink, 1);
					hname = string_resub("<div class=\"Named\">", "</div>", ret1[i].part, 0);

					pichname = ostrcat(hname, NULL, 0, 0);
					string_tolower(pichname);
					pichname = stringreplacecharonce(pichname, '.', '\0');

					extra = string_resub("<b>Vom</b>: ", "</div>", ret1[i].part, 0);
			
					hlink = string_replace("http://kinox.to//Stream/", "", hlink, 1);					
					hlink = string_replace("http://kinox.to/Stream/", "", hlink, 1);
										
					pathnew = ostrcat("/aGET/Mirror/", hlink, 0, 0);
					debug(99, "pathnew: %s", pathnew);
	
					tmpstr1 = gethttp("kinox.to", pathnew, 80, NULL, NULL, 10000, NULL, 0);
					tmpstr1 = string_replace_all("\\", "", tmpstr1, 1);
					tmpstr1 = string_resub("<a href=\"", "\"", tmpstr1, 0);
	
					url = ostrcat(tmpstr1, NULL, 0, 0);

					type = 14;

					debug(99, "-------------------------------");
					debug(99, "(%d/%d) %s url: %s extra: %s",i ,max , hname, url, extra);
							
					incount += 1;
					line = ostrcat(line, hname, 1, 0);
					if(extra != NULL)
					{
						line = ostrcat(line, " (", 1, 0);					
						line = ostrcat(line, extra, 1, 0);					
						line = ostrcat(line, ")", 1, 0);					
					}
					line = ostrcat(line, "#", 1, 0);
					line = ostrcat(line, url, 1, 0);
					line = ostrcat(line, "#", 1, 0);
					line = ostrcat(line, "http://atemio.dyndns.tv/mediathek/menu/", 1, 0);
					line = ostrcat(line, pichname, 1, 0);
					line = ostrcat(line, ".jpg#kinox_", 1, 0);
					line = ostrcat(line, pichname, 1, 0);
					line = ostrcat(line, ".jpg#KinoX - ", 1, 0);
					line = ostrcat(line, title, 1, 0);
					line = ostrcat(line, "#", 1, 0);
					line = ostrcat(line, oitoa(type), 1, 0);
					line = ostrcat(line, "\n", 1, 0);

					free(tmpstr1), tmpstr1 = NULL;							
					free(tmpstr2), tmpstr2 = NULL;					
					free(hname), hname = NULL;
					free(hnr), hnr = NULL;
					free(hlink), hlink = NULL;										
					free(url), url = NULL;	
					free(pathnew), pathnew = NULL;
					free(pichname), pichname = NULL;
					free(extra), extra = NULL;
				}
			}
			free(ret1), ret1 = NULL;
		}
	}

	free(tmpstr), tmpstr = NULL;	
	if(line != NULL)
	{
		line = string_replace_all("http://atemio.dyndns.tv/", "http://imageshack.us/md/up/grd/", line, 1);
		tmpstr = ostrcat("/tmp/tithek/kinox.hoster.series.", oitoa(time(NULL)), 0, 1);
		tmpstr = ostrcat(tmpstr, ".list", 1, 0);
			
		writesys(tmpstr, line, 0);

		titheklog(debuglevel, "/tmp/kinox9_line", NULL, line);

		struct tithek* tnode = (struct tithek*)listbox->select->handle;
		createtithek(tnode, tnode->title,  tmpstr, tnode->pic, tnode->localname, tnode->menutitle, tnode->flag);
		ret = 0;
	}

	return ret;
}

int kinox_search_cast(struct skin* grid, struct skin* listbox, struct skin* countlabel, struct skin* load, char* link, char* title, char* searchstr)
{
	int ret = 1;

	if(listbox == NULL || listbox->select == NULL || listbox->select->handle == NULL)
		return ret;

	char* search = NULL;
	if(searchstr == NULL)
		search = textinputhist(_("Search"), " ", "searchhist");
	else
		search = textinputhist(_("Search"), searchstr, "searchhist");
	
	if(search != NULL)
	{
		drawscreen(load, 0, 0);
		search = strstrip(search);
		search = stringreplacechar(search, ' ', '+');
		debug(99, "search: %s", search);

		char* tmpstr = NULL;
		char* tmpstr1 = NULL;
		char* ip = NULL;
		char* path = NULL;

		char* line = NULL;
		char* menu = NULL;

		ip = ostrcat("www.imdb.com", NULL, 0, 0);
		path = ostrcat("find?q=", search, 0, 0);
		path = ostrcat(path, "&s=nm", 1, 0);

		tmpstr = gethttp(ip, path, 80, NULL, NULL, 10000, NULL, 0);
		writesys("/var/usr/local/share/titan/plugins/tithek/kinox_cast_tmpstr", tmpstr, 0);
		tmpstr = string_replace_all("<td class=\"primary_photo\"> <a href=\"/name/", "\nfound=\"", tmpstr, 1);
		writesys("/var/usr/local/share/titan/plugins/tithek/kinox_cast_tmpstr2", tmpstr, 0);

		tmpstr1 = string_resub("<table class=\"findList\">", "</div>", tmpstr, 0);
		writesys("/var/usr/local/share/titan/plugins/tithek/kinox_cast_tmpstr3", tmpstr1, 0);
				free(tmpstr), tmpstr = NULL;

		int count = 0;
		int incount = 0;
		int i;
		struct splitstr* ret1 = NULL;
		ret1 = strsplit(tmpstr1, "\n", &count);
	
		char* url = NULL;
		char* pic = NULL;
		char* name = NULL;
		
		if(ret1 != NULL && count > 0)
		{
			int max = count;
			for(i = 0; i < max; i++)
			{
				url = string_resub("found=\"", "/", ret1[i].part, 0);
				pic = string_resub("<img src=\"", "\"", ret1[i].part, 0);

				tmpstr = string_resub("found=\"", "<small>", ret1[i].part, 0);
//				name = oregex(".*<a href=\"/name/nm.*\ >(.*)</a>", tmpstr);
				name = oregex(".*<a href=\"/name/nm.* >(.*)</a>", tmpstr);

				debug(99, "(%d/%d) name: %s url: %s pic: %s\n",i, count, name, url, pic);
				debug(99, "-------------------------------");
									
				pic = string_replace_all(",", "%2C", pic, 1);

				if(url != NULL)
				{
					incount++;
					line = ostrcat(line, name, 1, 0);
					line = ostrcat(line, "#http://kinox.to/People/", 1, 0);
					line = ostrcat(line, url, 1, 0);
					line = ostrcat(line, "#", 1, 0);
					line = ostrcat(line, pic, 1, 0);
					line = ostrcat(line, "#kinox_search_", 1, 0);
					line = ostrcat(line, oitoa(incount + time(NULL)), 1, 1);
					line = ostrcat(line, ".jpg#KinoX - Cast Search#32\n", 1, 0);
				}
				
				free(url), url = NULL;
				free(pic), pic = NULL;
				free(name), name = NULL;				
				free(tmpstr), tmpstr = NULL;
			}

			free(ret1), ret1 = NULL;
			if(line != NULL)
			{
				line = string_replace_all("http://atemio.dyndns.tv/", "http://imageshack.us/md/up/grd/", line, 1);
				menu = ostrcat("/tmp/tithek/kinox.search.cast.", oitoa(time(NULL)), 0, 1);
				menu = ostrcat(menu, ".list", 1, 0);
			
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

#endif