#ifndef KINOX_H
#define KINOX_H

// flag 1 = putlocker/sockshare
// flag 2 = filenuke

char* kinox(char* link)
{
	debug(99, "link: %s", link);
	char* streamurl = NULL;

	streamurl = hoster(link);
//	debug(99, "streamurl1: %s", streamurl);

//	streamurl = string_replace_all("amp;", "", streamurl, 1);
//	debug(99, "streamurl2: %s", streamurl);

	return streamurl;
}

int kinox_search(struct skin* grid, struct skin* listbox, struct skin* countlabel, struct skin* load, char* link, char* title, char* searchstr, int flag)
{
	int ret = 1;
	int debuglevel = getconfigint("debuglevel", NULL);
	int localhoster = getconfigint("tithek_kinox_localhoster", NULL);
	int usepic = getconfigint("tithek_kinox_pic", NULL);

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
		char* newurl = NULL;
		char *cmd = NULL;

		search = strstrip(search);
		search = stringreplacechar(search, ' ', '+');
		debug(99, "search: %s", search);

		if(flag == 0)
			path = ostrcat("Search.html?q=", search, 0, 0);
		else
		{
			path = ostrcat(search, NULL, 0, 0);
//			path = string_replace_all("kinox.to", "", path, 0);
//			path = string_replace_all(getconfig("tithek_kinox_url", NULL), "", path, 0);
//			path = string_replace_all("http:///", "", path, 0);
//			path = string_replace_all("https:///", "", path, 0);
		}

		newurl = ostrcat(getconfig("tithek_kinox_url", NULL), "/", 0, 0);
		newurl = ostrcat(newurl, path, 1, 0);

		if(localhoster == 0)
			tmpstr = gethttp(ip, path, 80, NULL, NULL, 10000, NULL, 0);
		else if(localhoster == 1)
			tmpstr = gethttps(newurl, NULL, NULL, NULL, NULL, NULL, 1);
		else if(localhoster == 2)
		{
// new start
			cmd = ostrcat("/tmp/localhoster/hoster.sh get '", newurl, 0, 0);
			cmd = ostrcat(cmd, "'", 1, 0);
			debug(99, "cmd: %s", cmd);
			tmpstr = command(cmd);
			free(cmd), cmd = NULL;
// new end
		}
		free(newurl), newurl = NULL;

		if(!ostrncmp("errormsg=", tmpstr, 9))
		{
			tmpstr1 = ostrcat(_("Found error Msg:"), "\n\n", 0, 0);
			tmpstr1 = ostrcat(tmpstr1, tmpstr, 1, 0);
			tmpstr1 = string_replace("errormsg=", "", tmpstr1, 1);

			debug(99, "Found error Msg: %s", tmpstr);
			textbox(_("Message"), tmpstr1, _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 1100, 300, 0, 2);
			free(tmpstr); tmpstr = NULL;
			free(tmpstr1); tmpstr1 = NULL;
		}

		tmpstr = string_resub("<div id=\"beep\" class=\"beep\"></div>", "</table>", tmpstr, 0);

		titheklog(debuglevel, "/tmp/kinox1_tmpstr", NULL, NULL, NULL, tmpstr);
		
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
/*														
					newurl = ostrcat(getconfig("tithek_kinox_url", NULL), "/", 0, 0);
					newurl = ostrcat(newurl, path, 1, 0);


					if(usepic == 1)
					{
						if(localhoster == 0)
							tmpstr1 = gethttp("kinox.to", path, 80, NULL, NULL, 10000, NULL, 0);
						else if(localhoster == 1)
							tmpstr1 = gethttps(newurl, NULL, NULL, NULL, NULL, NULL, 1);
						else if(localhoster == 2)
						{
// new start
							cmd = ostrcat("/tmp/localhoster/hoster.sh get '", newurl, 0, 0);
							cmd = ostrcat(cmd, "'", 1, 0);
							debug(99, "cmd: %s", cmd);
// disable to slow currently
							tmpstr1 = command(cmd);
							free(cmd), cmd = NULL;
// new end
						}
					}
					free(newurl), newurl = NULL;

					from = ostrcat("<div class=\"Grahpics\"><a href=\"", path, 0, 0);
					from = ostrcat(from, "\"><img src=\"", 1, 0);

					pic = string_resub(from, "\" alt=\"", tmpstr1, 0);
					
					if(pic == NULL)
						pic = ostrcat("dummy", NULL, 0, 0);
*/
					title = ostrcat(path, NULL, 0, 0);
					title = string_replace_all("/Stream/", "", title, 1);
					title = string_replace_all(".html", "", title, 1);
				 	title = stringreplacechar(title, '_', ' ');
					title = ostrcat(title , lang, 1, 0);

					url = ostrcat(getconfig("tithek_kinox_url", NULL), "/", 0, 0);
					url = ostrcat(url, path, 1, 0);

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

						line = ostrcat(line, "echo ", 1, 0);
					    line = ostrcat(line, getconfig("tithek_kinox_url", NULL), 1, 0);
      					line = ostrcat(line, " | tr -d '\\n' && /tmp/localhoster/hoster.sh get '", 1, 0);
						line = ostrcat(line, url, 1, 0);
						line = ostrcat(line, "' | sed -nr 's/.*<div class=\\\"Grahpics\\\"><a href=\\\".*img src=\\\"([^\\\"]+)\\\".*/\\1/p' | tr -d '\\n'", 1, 0);
/*
						if(usepic == 1)
						{
							line = ostrcat(line, getconfig("tithek_kinox_url", NULL), 1, 0);
							line = ostrcat(line, "/", 1, 0);
							line = ostrcat(line, pic, 1, 0);
						}
						else
							line = ostrcat(line, "http://openaaf.dyndns.tv/mediathek/menu/default.jpg", 1, 0);
*/
						line = ostrcat(line, "#kinox_search_", 1, 0);
						line = ostrcat(line, oitoa(incount + time(NULL)), 1, 1);
//						line = ostrcat(line, ".jpg#KinoX - Search#22\n", 1, 0);
						line = ostrcat(line, ".jpg#KinoX - Search#22#/tmp/localhoster/hoster.sh get '", 1, 0);
						line = ostrcat(line, url, 1, 0);
						line = ostrcat(line, "' | sed -nr 's/.*<div class=\\\"Descriptore\\\">([^>]+)<.*/\\1/p'\n", 1, 0);
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
				line = string_replace_all("http://openaaf.dyndns.tv/", "http://imageshack.us/md/up/grd/", line, 1);
				menu = ostrcat("/tmp/tithek/kinox.search.", oitoa(time(NULL)), 0, 1);
				menu = ostrcat(menu, ".list", 1, 0);

				writesys(menu, line, 0);
				struct tithek* tnode = (struct tithek*)listbox->select->handle;
				createtithek(tnode, tnode->title,  menu, tnode->pic, tnode->localname, tnode->menutitle, tnode->description, tnode->flag);
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

		tmpstr = gethttp("openaaf.dyndns.tv", "/mediathek/kinox/streams/kinox.all-sorted.list", 80, NULL, HTTPAUTH, 5000, NULL, 0);

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
				menu = ostrcat("/tmp/tithek/kinox.search.", oitoa(time(NULL)), 0, 1);
				menu = ostrcat(menu, ".list", 1, 0);

				writesys(menu, line, 0);
				struct tithek* tnode = (struct tithek*)listbox->select->handle;
				createtithek(tnode, tnode->title, menu, tnode->pic, tnode->localname, tnode->menutitle, tnode->description, tnode->flag);
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
	int localhoster = getconfigint("tithek_kinox_localhoster", NULL);

	int ret = 1, series = 0;
	char* ip = NULL, *pos = NULL, *path = NULL, *tmpstr = NULL, *tmpstr1 = NULL, *tmpstr2 = NULL, *tmpstr3 = NULL, *pichname = NULL;
	char* tmpstr4 = NULL, *tmpstr5 = NULL, *line = NULL, *url = NULL, *url2 = NULL, *url3 = NULL, *url4 = NULL, *pathnew = NULL, *extra = NULL;
	char* newurl = NULL;
	char* cmd = NULL;

	if(listbox == NULL || listbox->select == NULL || listbox->select->handle == NULL)
		return ret;

//	ip = string_replace("http://", "", (char*)link, 0);
	ip = oregex("://(.*)/.*", link);

	if(ip != NULL)
		pos = strchr(ip, '/');
	if(pos != NULL)
	{
		pos[0] = '\0';
		path = pos + 1;
	}

	pathnew = oregex("://.*(/Stream.*).*", link);
	newurl = ostrcat(getconfig("tithek_kinox_url", NULL), "/", 0, 0);
	newurl = ostrcat(newurl, pathnew, 1, 0);

	if(localhoster == 0)
		tmpstr = gethttp(ip, path, 80, NULL, NULL, 10000, NULL, 0);
	else if(localhoster == 1)
		tmpstr = gethttps(newurl, NULL, NULL, NULL, NULL, NULL, 1);
	else if(localhoster == 2)
	{
// new start
		cmd = ostrcat("/tmp/localhoster/hoster.sh get '", newurl, 0, 0);
		cmd = ostrcat(cmd, "'", 1, 0);
		debug(99, "cmd: %s", cmd);
		tmpstr = command(cmd);
		free(cmd), cmd = NULL;
// new end
	}
	titheklog(debuglevel, "/tmp/kinox2_tmpstr", NULL, NULL, NULL, tmpstr);

	if(!ostrncmp("errormsg=", tmpstr, 9))
	{
		tmpstr1 = ostrcat(_("Found error Msg:"), "\n\n", 0, 0);
		tmpstr1 = ostrcat(tmpstr1, tmpstr, 1, 0);
		tmpstr1 = string_replace("errormsg=", "", tmpstr1, 1);

		debug(99, "Found error Msg: %s", tmpstr);
		textbox(_("Message"), tmpstr1, _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 1100, 300, 0, 2);
		free(tmpstr); tmpstr = NULL;
		free(tmpstr1); tmpstr1 = NULL;
	}

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
							line = ostrcat(line, "http://openaaf.dyndns.tv/mediathek/menu/s", 1, 0);
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
			titheklog(debuglevel, "/tmp/kinox3_tmpstr", NULL, NULL, NULL, tmpstr);
	
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
					char *mirrormaxtmp = NULL;
					char *pathnewtmp = NULL;
					int mirrormax = 1;
					int j = 0;

//					mirrormaxtmp = oregex("<b>Mirror</b>: .*/(.*)<br/>.*", ret1[i].part);
					mirrormaxtmp = oregex("<b>Mirror</b>: .*/(.*)<br.*", ret1[i].part);

					if(mirrormaxtmp != NULL)
						mirrormax = atoi(mirrormaxtmp);

					debug(99, "### START ####################################################");

					for(j = 1; j < mirrormax + 1; j++)
					{
						debug(99, "(%d/%d) (%d/%d) ret1[i].part: %s",i ,max ,j ,mirrormax, ret1[i].part);

						hnr = string_resub("<li id=\"Hoster_", "\"", ret1[i].part, 0);

						hlink = string_resub("rel=\"", "amp;Mirror", ret1[i].part, 0);

						if (hlink == NULL)
						{
							hlink = string_resub("rel=\"", "\">", ret1[i].part, 0);
							hlink = string_replace_all("amp;Hoster", "Hoster", hlink, 1);
							hname = string_resub("<div class=\"Named\">", "</div>", ret1[i].part, 0);
							pathnewtmp = ostrcat(getconfig("tithek_kinox_url", NULL), "/aGET/Mirror/", 0, 0);
							pathnewtmp = ostrcat(pathnewtmp, hlink, 1, 0);
							pathnew = ostrcat(pathnewtmp, NULL, 0, 0);
						}
						else
						{
							hlink = string_replace_all("amp;Hoster", "Hoster", hlink, 1);
							hlink = ostrcat(hlink, "Mirror=", 0, 0);
							hname = string_resub("<div class=\"Named\">", "</div>", ret1[i].part, 0);
							pathnewtmp = ostrcat(getconfig("tithek_kinox_url", NULL), "/aGET/Mirror/", 0, 0);
							pathnewtmp = ostrcat(pathnewtmp, hlink, 1, 0);
							pathnew = ostrcat(pathnewtmp, oitoa(j), 0, 1);
						}
						debug(99, "(%d/%d) (%d/%d)hnr: %s hlink: %s hname: %s pathnewtmp: %s",i ,max ,j ,mirrormax ,hnr ,hlink ,hname ,pathnewtmp);
						free(pathnewtmp), pathnewtmp= NULL;

						pichname = ostrcat(hname, NULL, 0, 0);
						string_tolower(pichname);
						pichname = stringreplacecharonce(pichname, '.', '\0');

						extra = string_resub("<b>Vom</b>: ", "</div>", ret1[i].part, 0);

						titheklog(debuglevel, "/tmp/kinox4_pathnew1", hname, NULL, NULL, pathnew);


						if(localhoster == 0)
							tmpstr1 = gethttp("kinox.to", pathnew, 80, NULL, NULL, 10000, NULL, 0);
						else if(localhoster == 1)
							tmpstr1 = gethttps(pathnew, NULL, NULL, NULL, NULL, NULL, 1);
						else if(localhoster == 2)
						{
							cmd = ostrcat("/tmp/localhoster/hoster.sh hoster '", pathnew, 0, 0);
							cmd = ostrcat(cmd, "'", 1, 0);
							debug(99, "cmd: %s", cmd);
							url = ostrcat(cmd, NULL, 0, 0);
						}
						free(pathnew), pathnew = NULL;
						titheklog(debuglevel, "/tmp/kinox5_tmpstr1", hname, NULL, NULL, tmpstr1);

						tmpstr1 = string_replace_all("\\", "", tmpstr1, 1);

						if(localhoster == 0 || localhoster == 1)
						{
							if(ostrstr(tmpstr1, "iframe src") != NULL)
								url = string_resub("<iframe src=\"", "\"", tmpstr1, 0);
							else
								url = string_resub("<a href=\"", "\"", tmpstr1, 0);
						}

						type = 14;
						if(localhoster == 2)
							type = 111;

						debug(99, "-------------------------------");
						debug(99, "(%d/%d) (%d/%d) %s url: %s extra: %s",i ,max ,j ,mirrormax ,hname ,url ,extra);
						debug(99, "-------------------------------");

						incount += 1;
						line = ostrcat(line, hname, 1, 0);

						if(url == NULL)
							line = ostrcat(line, " (Error)", 1, 0);
					
						if(url2 != NULL && ostrcmp(url, url2) != 0)
							line = ostrcat(line, " (Part1)", 1, 0);
						if(extra != NULL)
						{
							line = ostrcat(line, " (", 1, 0);					
							line = ostrcat(line, extra, 1, 0);					
							line = ostrcat(line, ")", 1, 0);					
						}

						line = ostrcat(line, " (Mirror ", 1, 0);					
						line = ostrcat(line, oitoa(j), 1, 1);
						line = ostrcat(line, "/", 1, 0);					
						line = ostrcat(line, oitoa(mirrormax), 1, 1);
						line = ostrcat(line, ")", 1, 0);					

				
						line = ostrcat(line, "#", 1, 0);
						if(!ostrncmp("//", url, 2))
							line = ostrcat(line, "http:", 1, 0);
						line = ostrcat(line, url, 1, 0);
						line = ostrcat(line, "#", 1, 0);
						line = ostrcat(line, "http://openaaf.dyndns.tv/mediathek/menu/", 1, 0);
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
					debug(99, "### END #####################################################");
				}
			}
			free(ret1), ret1 = NULL;
		}
	}
	free(tmpstr), tmpstr = NULL;	

	if(line != NULL)
	{
		line = string_replace_all("http://openaaf.dyndns.tv/", "http://imageshack.us/md/up/grd/", line, 1);
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

		titheklog(debuglevel, "/tmp/kinox8_line", NULL, NULL, NULL, line);
					
		struct tithek* tnode = (struct tithek*)listbox->select->handle;
		createtithek(tnode, tnode->title,  tmpstr, tnode->pic, tnode->localname, tnode->menutitle, tnode->description, tnode->flag);
		ret = 0;
	}

	return ret;
}

int kinox_hoster_series(struct skin* grid, struct skin* listbox, struct skin* countlabel, struct skin* load, char* link, char* title)
{
	debug(99, "link: %s", link);
	int debuglevel = getconfigint("debuglevel", NULL);
	int localhoster = getconfigint("tithek_kinox_localhoster", NULL);

	int ret = 1;
	char* ip = NULL, *pathnew = NULL, *seriesid = NULL, *searchname = NULL, *url = NULL, *session = NULL, *episode = NULL, *pos = NULL, *path = NULL, *tmpstr = NULL, *tmpstr1 = NULL, *tmpstr2 = NULL, *line = NULL;		
	char* pichname = NULL, *extra= NULL;
	char* newurl = NULL;
	char *cmd = NULL;

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

		searchname = string_replace("kinox.to", "", searchname, 0);
		searchname = string_replace(getconfig("tithek_kinox_url", NULL), "", searchname, 0);
		searchname = string_replace("http:////Stream/", "", searchname, 0);
		searchname = string_replace("http:///Stream/", "", searchname, 0);
		searchname = string_replace("http://Stream/", "", searchname, 0);
		searchname = string_replace("https:////Stream/", "", searchname, 0);
		searchname = string_replace("https:///Stream/", "", searchname, 0);
		searchname = string_replace("https://Stream/", "", searchname, 0);
		searchname = string_replace("//Stream/", "", searchname, 0);
		searchname = string_replace("/Stream/", "", searchname, 0);
		searchname = string_replace(".html", "", searchname, 0);
		debug(99, "searchname: %s", searchname);
		free(searchname), searchname = NULL;

		searchname = oregex("://.*/Stream/(.*).html.*", ret0[0].part);
		debug(99, "new searchname: %s", searchname);
	}
	free(ret0), ret0 = NULL;

//	ip = string_replace("http://", "", url, 0);
	ip = oregex("://(.*)/.*", link);

	if(ip != NULL)
		pos = strchr(ip, '/');
	if(pos != NULL)
	{
		pos[0] = '\0';
		path = pos + 1;
	}

//	pathnew = oregex("://.*/(.*).*", link);
	pathnew = oregex("://.*(/Stream.*).*", link);

	newurl = ostrcat(getconfig("tithek_kinox_url", NULL), "/", 0, 0);
	newurl = ostrcat(newurl, pathnew, 1, 0);

	if(localhoster == 0)
		tmpstr = gethttp(ip, path, 80, NULL, NULL, 10000, NULL, 0);
	else if(localhoster == 1)
		tmpstr = gethttps(newurl, NULL, NULL, NULL, NULL, NULL, 1);
	else if(localhoster == 2)
	{
// new start
		cmd = ostrcat("/tmp/localhoster/hoster.sh get '", newurl, 0, 0);
		cmd = ostrcat(cmd, "'", 1, 0);
		debug(99, "cmd: %s", cmd);
		tmpstr = command(cmd);
		free(cmd), cmd = NULL;
// new end
	}

	debug(99, "tmpstr: %s", tmpstr);
	free(newurl), newurl = NULL;

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
		newurl = ostrcat(getconfig("tithek_kinox_url", NULL), "/", 0, 0);
		newurl = ostrcat(newurl, pathnew, 1, 0);
		if(localhoster == 0)
			tmpstr = gethttp("kinox.to", pathnew, 80, NULL, NULL, 10000, NULL, 0);
		else if(localhoster == 1)		
			tmpstr = gethttps(newurl, NULL, NULL, NULL, NULL, NULL, 1);
		else if(localhoster == 2)
		{
// new start
			cmd = ostrcat("/tmp/localhoster/hoster.sh get '", newurl, 0, 0);
			cmd = ostrcat(cmd, "'", 1, 0);
			debug(99, "cmd: %s", cmd);
			tmpstr = command(cmd);
			free(cmd), cmd = NULL;
// new end
		}
		free(newurl), newurl = NULL;
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
					char *mirrormaxtmp = NULL;
					char *pathnewtmp = NULL;
					char *season = NULL;
					int mirrormax = 1;
					int j = 0;

					mirrormaxtmp = oregex("<b>Mirror</b>: .*/(.*)<br.*", ret1[i].part);
					
					if(mirrormaxtmp != NULL)
						mirrormax = atoi(mirrormaxtmp);
					free(mirrormaxtmp), mirrormaxtmp= NULL;

					debug(99, "### START ####################################################");

					for(j = 1; j < mirrormax + 1; j++)
					{
						debug(99, "(%d/%d) (%d/%d) ret1[i].part: %s",i ,max ,j ,mirrormax, ret1[i].part);

						hnr = string_resub("<li id=\"Hoster_", "\"", ret1[i].part, 0);

//						hlink = string_resub("rel=\"", "amp;Mirror", ret1[i].part, 0);
//						hlink = string_replace_all("amp;Hoster", "Hoster", hlink, 1);
//						hlink = ostrcat(hlink, "Mirror=", 0, 0);

//////////
						hlink = string_resub("rel=\"", "amp;Mirror", ret1[i].part, 0);

						if (hlink == NULL)
						{
							hlink = string_resub("rel=\"", "\">", ret1[i].part, 0);
							hlink = string_replace_all("amp;Hoster", "Hoster", hlink, 1);
							hname = string_resub("<div class=\"Named\">", "</div>", ret1[i].part, 0);
							pathnewtmp = ostrcat(getconfig("tithek_kinox_url", NULL), "/aGET/Mirror/", 0, 0);
							pathnewtmp = ostrcat(pathnewtmp, hlink, 1, 0);
							pathnew = ostrcat(pathnewtmp, NULL, 0, 0);
						}
						else
						{
							hlink = string_replace_all("amp;Hoster", "Hoster", hlink, 1);
							hlink = ostrcat(hlink, "Mirror=", 0, 0);
							hname = string_resub("<div class=\"Named\">", "</div>", ret1[i].part, 0);
							pathnewtmp = ostrcat(getconfig("tithek_kinox_url", NULL), "/aGET/Mirror/", 0, 0);
							pathnewtmp = ostrcat(pathnewtmp, hlink, 1, 0);
							pathnew = ostrcat(pathnewtmp, oitoa(j), 0, 1);
						}
						debug(99, "(%d/%d) (%d/%d)hnr: %s hlink: %s hname: %s pathnewtmp: %s",i ,max ,j ,mirrormax ,hnr ,hlink ,hname ,pathnewtmp);
						free(pathnewtmp), pathnewtmp = NULL;
//////////
//						hname = string_resub("<div class=\"Named\">", "</div>", ret1[i].part, 0);
//						pathnewtmp = ostrcat("/aGET/Mirror/", hlink, 0, 0);

						season = string_resub("&amp;Season=", "\">", ret1[i].part, 0);
						season = string_replace_all("amp;", "", season, 1);

//						pathnew = ostrcat(pathnewtmp, oitoa(j), 0, 1);
						pathnew = ostrcat(pathnew, "&Season=", 1, 0);
						pathnew = ostrcat(pathnew, season, 1, 0);

						debug(99, "(%d/%d) (%d/%d) hnr: %s hlink: %s season: %s hname: %s pathnewtmp: %s",i ,max ,j ,mirrormax ,hnr ,hlink ,season ,hname ,pathnewtmp);
 
						free(season), season= NULL;
						free(pathnewtmp), pathnewtmp = NULL;

						pichname = ostrcat(hname, NULL, 0, 0);
						string_tolower(pichname);
						pichname = stringreplacecharonce(pichname, '.', '\0');

						extra = string_resub("<b>Vom</b>: ", "</div>", ret1[i].part, 0);

						hlink = string_replace("kinox.to", "", hlink, 1);					
						hlink = string_replace(getconfig("tithek_kinox_url", NULL), "", hlink, 1);					
						hlink = string_replace("http:////Stream/", "", hlink, 1);					
						hlink = string_replace("http:///Stream/", "", hlink, 1);
						hlink = string_replace("https:////Stream/", "", hlink, 1);					
						hlink = string_replace("https:///Stream/", "", hlink, 1);

						if(localhoster == 0)
							tmpstr1 = gethttp("kinox.to", pathnew, 80, NULL, NULL, 10000, NULL, 0);
						else if(localhoster == 1)
							tmpstr1 = gethttps(pathnew, NULL, NULL, NULL, NULL, NULL, 1);
						else if(localhoster == 2)
						{
// new start
							cmd = ostrcat("/tmp/localhoster/hoster.sh hoster '", pathnew, 0, 0);
							cmd = ostrcat(cmd, "'", 1, 0);
							debug(99, "cmd: %s", cmd);
							url = ostrcat(cmd, NULL, 0, 0);
// new end

//							cmd = ostrcat("/tmp/localhoster/hoster.sh get '", pathnew, 0, 0);
//							cmd = ostrcat(cmd, "'", 1, 0);
//							debug(99, "cmd: %s", cmd);
//							tmpstr1 = command(cmd);
//							free(cmd), cmd = NULL;
						}
						tmpstr1 = string_replace_all("\\", "", tmpstr1, 1);
//						debug(99, "tmpstr1 2: %s", tmpstr1);

						if(localhoster == 0 || localhoster == 1)
						{
							if(ostrstr(tmpstr1, "<iframe src=") != NULL)
							{
								url = string_resub("<iframe src=\"", "\"", tmpstr1, 0);
	//							debug(99, "iframe1.1 url: %s", url);
							}
							else if(ostrstr(tmpstr1, "/iframe>") != NULL)
							{
								free(pathnew), pathnew = NULL;
								pathnew = ostrcat(getconfig("tithek_kinox_url", NULL), "/aGET/Mirror/", 0, 0);
								pathnew = ostrcat(pathnew, hlink, 1, 0);
	//							debug(99, "iframe2.1 pathnew: %s", pathnew);
								free(tmpstr1), tmpstr1 = NULL;
	//							tmpstr1 = gethttps(pathnew, NULL, NULL, NULL, NULL, NULL, 1);
	// new start
								cmd = ostrcat("/tmp/localhoster/hoster.sh get '", pathnew, 0, 0);
								cmd = ostrcat(cmd, "'", 1, 0);
								debug(99, "cmd: %s", cmd);
								tmpstr1 = command(cmd);
								free(cmd), cmd = NULL;
	// new end
	//							debug(99, "iframe2.2 tmpstr1: %s", tmpstr1);
								tmpstr1 = string_replace_all("\\", "", tmpstr1, 1);
	//							debug(99, "iframe2.3 tmpstr1: %s", tmpstr1);

								if(ostrstr(tmpstr1, "<iframe src=") != NULL)
								{
									url = string_resub("<iframe src=\"", "\"", tmpstr1, 0);
	//								debug(99, "iframe2.4 url: %s", url);
								}
							}
							else
							{
								tmpstr1 = string_resub("<a href=\"", "\"", tmpstr1, 0);
	//							debug(99, "tmpstr1 3: %s", tmpstr1);
								url = ostrcat(tmpstr1, NULL, 0, 0);
							}
						}
	
						type = 14;
						if(localhoster == 2)
							type = 111;

						debug(99, "-------------------------------");
						debug(99, "(%d/%d) (%d/%d) %s url: %s extra: %s", i, max, j, mirrormax, hname, url, extra);
						debug(99, "-------------------------------");
							
						incount += 1;

						line = ostrcat(line, hname, 1, 0);

						if(url == NULL)
							line = ostrcat(line, " (Error)", 1, 0);

						if(extra != NULL)
						{
							line = ostrcat(line, " (", 1, 0);					
							line = ostrcat(line, extra, 1, 0);					
							line = ostrcat(line, ")", 1, 0);					
						}

						line = ostrcat(line, " (Mirror ", 1, 0);					
						line = ostrcat(line, oitoa(j), 1, 1);
						line = ostrcat(line, "/", 1, 0);					
						line = ostrcat(line, oitoa(mirrormax), 1, 1);
						line = ostrcat(line, ")", 1, 0);


						line = ostrcat(line, "#", 1, 0);
						line = ostrcat(line, url, 1, 0);
						line = ostrcat(line, "#", 1, 0);
						line = ostrcat(line, "http://openaaf.dyndns.tv/mediathek/menu/", 1, 0);
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
					debug(99, "### END #####################################################");
				}
			}
			free(ret1), ret1 = NULL;
		}
	}

	free(tmpstr), tmpstr = NULL;	
	if(line != NULL)
	{
		line = string_replace_all("http://openaaf.dyndns.tv/", "http://imageshack.us/md/up/grd/", line, 1);
		tmpstr = ostrcat("/tmp/tithek/kinox.hoster.series.", oitoa(time(NULL)), 0, 1);
		tmpstr = ostrcat(tmpstr, ".list", 1, 0);
			
		writesys(tmpstr, line, 0);

		titheklog(debuglevel, "/tmp/kinox9_line", NULL, NULL, NULL, line);

		struct tithek* tnode = (struct tithek*)listbox->select->handle;
		createtithek(tnode, tnode->title,  tmpstr, tnode->pic, tnode->localname, tnode->menutitle, tnode->description, tnode->flag);
		ret = 0;
	}

	return ret;
}

int kinox_search_cast(struct skin* grid, struct skin* listbox, struct skin* countlabel, struct skin* load, char* link, char* title, char* searchstr)
{
	int debuglevel = getconfigint("debuglevel", NULL);
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

//		tmpstr = gethttp(ip, path, 80, NULL, NULL, 10000, NULL, 0);
		char* newurl = NULL;
		newurl = ostrcat("www.imdb.com", "/", 0, 0);
		newurl = ostrcat(newurl, path, 1, 0);
		tmpstr = gethttps(newurl, NULL, NULL, NULL, NULL, NULL, 1);
		free(newurl), newurl = NULL;
 
		titheklog(debuglevel, "/tmp/kinox10_cast_tmpstr1", NULL, NULL, NULL, tmpstr);
//		tmpstr = string_replace_all("<td class=\"primary_photo\"> <a href=\"/name/", "\nfound=\"", tmpstr, 1);
//		tmpstr = string_replace_all("href=\"/name/", "\nfound=\"", tmpstr, 1);
		tmpstr = string_replace_all("<li class=\"ipc-metadata-list-summary-item ", "\nfound=\"", tmpstr, 1);
		titheklog(debuglevel, "/tmp/kinox10_cast_tmpstr2", NULL, NULL, NULL, tmpstr);

//		tmpstr1 = string_resub("<table class=\"findList\">", "</div>", tmpstr, 0);
		tmpstr1 = string_resub("End INLINE20", "Begin INLINE40", tmpstr, 0);
		titheklog(debuglevel, "/tmp/kinox10_cast_tmpstr3", NULL, NULL, NULL, tmpstr1);
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
				url = oregex(".*href=\"/name(/nm.*)/.*\">.*</a>", ret1[i].part);
//				pic = string_resub("loading=\"lazy\" src=\"", "\"", ret1[i].part, 0);
				pic = string_resub(" src=\"", "\"", ret1[i].part, 0);
				name = oregex(".*href=\"/name/nm.*>(.*)</a>", ret1[i].part);

				debug(99, "(%d/%d) name: %s url: %s pic: %s\n",i, count, name, url, pic);
				debug(99, "-------------------------------");
									
				pic = string_replace_all(",", "%2C", pic, 1);

				if(url != NULL)
				{
					incount++;
					line = ostrcat(line, name, 1, 0);
					line = ostrcat(line, "#", 1, 0);
//					line = ostrcat(line, getconfig("tithek_kinox_url", NULL), 1, 0);
					line = ostrcat(line, "/People/", 1, 0);
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
				line = string_replace_all("http://openaaf.dyndns.tv/", "http://imageshack.us/md/up/grd/", line, 1);
				menu = ostrcat("/tmp/tithek/kinox.search.cast.", oitoa(time(NULL)), 0, 1);
				menu = ostrcat(menu, ".list", 1, 0);
			
				writesys(menu, line, 0);
				struct tithek* tnode = (struct tithek*)listbox->select->handle;
				createtithek(tnode, tnode->title,  menu, tnode->pic, tnode->localname, tnode->menutitle, tnode->description, tnode->flag);
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
