#ifndef KINOX_H
#define KINOX_H

// flag 1 = putlocker/sockshare
// flag 2 = filenuke

char* kinox(char* link, char* url, char* name, int flag)
{
	debug(99, "link(%d): %s", flag, link);
	char* video_id = NULL, *source = NULL, *streamurl = NULL;

	if(flag == 1 || flag == 2 || flag == 3)
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

void kinox_search(struct skin* grid, struct skin* listbox, struct skin* countlabel, struct skin* load)
{
	char* search = textinputhist("Search", "NULL", "searchhist");
		
	if(search != NULL)
	{
		drawscreen(load, 0, 0);
		search = stringreplacechar(search, ' ', '+');
		char* tmpstr = NULL;
		char* tmpstr1 = NULL;
		char* tmpstr2 = NULL;
		char* line = NULL;
		char* pic = NULL;
		char* title = NULL;	
		char* lang = NULL;
		char* langck = NULL;
		char* from = NULL;
		char* url = NULL;
		char* ip = NULL;
		char* path = NULL;

		ip = ostrcat("kinox.to", NULL, 0, 0);
		path = ostrcat("Search.html?q=", search, 0, 0);
	
		tmpstr = gethttp(ip, path, 80, NULL, NULL, NULL, 0);
		tmpstr = string_resub("<div id=\"beep\" class=\"beep\"></div>", "</table>", tmpstr, 0);

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
																	
					tmpstr2 = gethttp("kinox.to", path, 80, NULL, NULL, NULL, 0);

					from = ostrcat("<div class=\"Grahpics\"><a href=\"", path, 0, 0);
					from = ostrcat(from, "\"><img src=\"", 1, 0);

					pic = string_resub(from, "\" alt=\"", tmpstr2, 0);
					
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
						line = ostrcat(line, oitoa(incount + time(NULL)), 1, 0);
						line = ostrcat(line, ".jpg#KinoX - Search#22\n", 1, 0);
					}
					free(url), url = NULL;
					free(path), path = NULL;
					free(title), title = NULL;
					free(pic), pic = NULL;		
					free(from), from = NULL;
					free(tmpstr2), tmpstr2 = NULL;
					free(lang), lang = NULL;
					free(langck), langck = NULL;	
				}
			}
			free(ret1), ret1 = NULL;
			if(line != NULL)
			{
				writesys("/tmp/tithek/kinox.search.list", line, 0);
				free(line), line = NULL;
				int pagecount = createtithekplay("/tmp/tithek/kinox.search.list", grid, listbox, countlabel);
				if(pagecount == 0) return;

				drawscreen(grid, 0, 0);
			}

		}
		free(tmpstr), tmpstr = NULL;
		free(ip), ip = NULL;
	}
	free(search), search = NULL;
}

void kinox_hoster(struct skin* grid, struct skin* listbox, struct skin* countlabel, struct skin* load, char* link, char* title, int flag)
{
	debug(99, "link: %s", link);
	char* ip = NULL, *pos = NULL, *path = NULL, *streamurl = NULL, *tmpstr = NULL, *tmpstr1 = NULL, *tmpstr2 = NULL, *line = NULL;		
	ip = string_replace("http://", "", (char*)link, 0);

	if(ip != NULL)
		pos = strchr(ip, '/');
	if(pos != NULL)
	{
		pos[0] = '\0';
		path = pos + 1;
	}

	tmpstr = gethttp(ip, path, 80, NULL, NULL, NULL, 0);

	if(ostrstr(tmpstr, "SeriesID") != NULL && flag == 0)
	{
		char* pos1 = NULL;
		char* from = NULL;
		char* folgen = NULL;
		int i;
	
		for(i = 1; i < 30; i++)
		{
			from = ostrcat(from, "<option value=\"", 1, 0);
			from = ostrcat(from, oitoa(i), 1, 0);
			from = ostrcat(from, "\" rel=", 1, 0);
		
			pos1 = ostrstr(tmpstr, from);
			folgen = getxmlentry(ostrstr(tmpstr, from), "rel=");
			printf("session %d folgen: %s\n",i , folgen);	
	

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
						line = ostrcat(line, _("Session"), 1, 0);
						line = ostrcat(line, " ", 1, 0);
						line = ostrcat(line, oitoa(i), 1, 0);
						line = ostrcat(line, " ", 1, 0);
						line = ostrcat(line, _("Episode"), 1, 0);
						line = ostrcat(line, " ", 1, 0);
						line = ostrcat(line, oitoa(j), 1, 0);					
						line = ostrcat(line, "#", 1, 0);
						line = ostrcat(line, link, 1, 0);
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
				char* pathnew = ostrcat("/aGET/Mirror/", hlink, 0, 0);

				tmpstr1 = gethttp("kinox.to", pathnew, 80, NULL, NULL, NULL, 0);
				free(pathnew), pathnew = NULL;

				tmpstr1 = string_replace_all("\\", "", tmpstr1, 1);
				tmpstr1 = string_resub("<a href=\"", "\"", tmpstr1, 0);

				char* url = ostrcat(tmpstr1, NULL, 0, 0);
				int type = 66;
				int count2 = 0;
				struct splitstr* ret2 = NULL;
				ret2 = strsplit(tmpstr1, "/", &count2);

				if(ret2 != NULL && count2 > 3 && ostrcmp(hname, "Sockshare.com") == 0)
				{
					tmpstr2 = ret2[3].part;
					type = 14;
				}
				else if(ret2 != NULL && count2 > 3 && ostrcmp(hname, "Putlocker.com") == 0)
				{
					tmpstr2 = ret2[3].part;
					type = 14;
				}
				else if(ret2 != NULL && count2 > 3 && ostrcmp(hname, "BitShare.com") == 0)
				{
					tmpstr2 = ret2[3].part;
				}
				else if(ret2 != NULL && count2 > 3 && ostrcmp(hname, "FlashX.tv") == 0)
				{
					tmpstr2 = ret2[3].part;
				}
				else if(ret2 != NULL && count2 > 3 && ostrcmp(hname, "PrimeShare.tv") == 0)
				{
					tmpstr2 = ret2[3].part;
				}
				else if(ret2 != NULL && count2 > 3 && ostrcmp(hname, "FileNuke.com") == 0)
				{
					tmpstr2 = ret2[3].part;
					type = 15;
				}
				else if(ret2 != NULL && count2 > 3 && ostrcmp(hname, "StreamCloud.eu") == 0)
				{
					tmpstr2 = ret2[3].part;
					type = 20;
				}
				else if(ret2 != NULL && count2 > 2)
					tmpstr2 = ret2[2].part;					

/*
	tmpstr2 = ret2[2].part;

	VidStream.in
	FileNuke.com
	StreamCloud.eu
	XvidStage.com
	Vidxden.com (DivX)
*/

				
				free(ret2), ret2 = NULL;
				printf("tmpstr2: %s\n",tmpstr2);	
									
				incount += 1;
				line = ostrcat(line, hname, 1, 0);
				line = ostrcat(line, "#", 1, 0);
				line = ostrcat(line, url, 1, 0);
				line = ostrcat(line, ";", 1, 0);
				line = ostrcat(line, tmpstr2, 1, 0);
				line = ostrcat(line, ";", 1, 0);				
				line = ostrcat(line, hname, 1, 0);
				line = ostrcat(line, "#", 1, 0);
				line = ostrcat(line, "http://atemio.dyndns.tv/mediathek/menu/", 1, 0);
				line = ostrcat(line, hname, 1, 0);
				line = ostrcat(line, ".jpg#kinox_", 1, 0);
				line = ostrcat(line, hname, 1, 0);
				line = ostrcat(line, ".jpg#KinoX - ", 1, 0);
				line = ostrcat(line, title, 1, 0);
				line = ostrcat(line, "#", 1, 0);
				line = ostrcat(line, oitoa(type), 1, 0);
				line = ostrcat(line, "\n", 1, 0);
	
				free(hname), hname = NULL;
				free(hnr), hnr = NULL;
				free(hlink), hlink = NULL;										
				free(url), url = NULL;	
			}
		}
		free(ret1), ret1 = NULL;
	}

	free(tmpstr), tmpstr = NULL;	
	if(line != NULL)
	{
		writesys("/tmp/tithek/kinox.search2.list", line, 0);
		free(line), line = NULL;
		int pagecount = createtithekplay("/tmp/tithek/kinox.search2.list", grid, listbox, countlabel);
		if(pagecount == 0) return;

		drawscreen(grid, 0, 0);
	}	
}

#endif