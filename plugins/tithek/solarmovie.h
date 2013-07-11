#ifndef SOLARMOVIE_H
#define SOLARMOVIE_H

// flag 1 = putlocker/sockshare
// flag 2 = filenuke

char* solarmovie(char* link, char* url, char* name, int flag)
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

int solarmovie_search(struct skin* grid, struct skin* listbox, struct skin* countlabel, struct skin* load, char* link, char* title, char* searchstr, int flag)
{
	int ret = 1;
	int incount = 0;
		
	if(listbox == NULL || listbox->select == NULL || listbox->select->handle == NULL)
		return ret;

	char* search = NULL;
	if(searchstr == NULL)
		search = textinputhist("Search", " ", "searchhist");
	else
		search = textinputhist("Search", searchstr, "searchhist");
	
	if(search != NULL)
	{
		drawscreen(load, 0, 0);
		search = stringreplacechar(search, ' ', '+');
		char* tmpstr = NULL;
		char* tmpstr1 = NULL;
		char* line = NULL;
		char* pic = NULL;
		char* title = NULL;
		char* url = NULL;
		char* ip = NULL;
		char* path = NULL;
		char* menu = NULL;
		char* file = NULL;
		char* file1 = NULL;

		ip = ostrcat("www.solarmovie.so", NULL, 0, 0);
		if(flag == 0)
			path = ostrcat("movie/search/", search, 0, 0);
		else
			path = ostrcat("tv/search/", search, 0, 0);
	
		tmpstr = gethttp(ip, path, 80, NULL, NULL, 10000, NULL, 0);
		if(getconfigint("debuglevel", NULL) == 99)
			writesys("/tmp/solarmovie_tmpstr", tmpstr, 0);

		while(ostrstr(tmpstr, "<div class=\"typicalGrey coverGroup\">") != NULL)
		{
			int rcret = waitrc(NULL, 10, 0);
			if(rcret == getrcconfigint("rcexit", NULL)) break;

			incount += 1;
			if(getconfigint("debuglevel", NULL) == 99)
			{
				file = ostrcat("/tmp/solarmovie_tmpstr", oitoa(incount), 0, 1);
				writesys(file, tmpstr, 0);
			}
				
			tmpstr1 = string_resub("<div class=\"typicalGrey coverGroup\">", "</div>", tmpstr, 0);

			if(getconfigint("debuglevel", NULL) == 99)
			{
				file1 = ostrcat("/tmp/solarmovie_tmpstr_resub", oitoa(incount), 0, 1);
				writesys(file1, tmpstr1, 0);
			}

			path = string_resub("href=\"", "\"", tmpstr1, 0);
			pic = string_resub("<img src=\"", "\"", tmpstr1, 0);
			title = string_resub("<a title=\"", "\"", tmpstr1, 0);
	
			url = ostrcat("http://www.solarmovie.so", path, 0, 0);
	
			debug(99, "---------------------------");
			debug(99, "pic: %s", pic);
			debug(99, "title: %s", title);
			debug(99, "url: %s", url);					
			debug(99, "---------------------------");
	
			if(url != NULL)
			{
				line = ostrcat(line, title, 1, 0);
				line = ostrcat(line, "#", 1, 0);
				line = ostrcat(line, url, 1, 0);
				line = ostrcat(line, "#", 1, 0);
				line = ostrcat(line, pic, 1, 0);
				line = ostrcat(line, "#solarmovie_search_", 1, 0);
				line = ostrcat(line, oitoa(incount + time(NULL)), 1, 0);
				line = ostrcat(line, ".jpg#SolarMovie - Search#28\n", 1, 0);
			}
			free(url), url = NULL;
			free(path), path = NULL;
			free(title), title = NULL;
			free(pic), pic = NULL;		
			free(file), file = NULL;
			free(file1), file1 = NULL;

			tmpstr = string_replace("typicalGrey coverGroup", "", tmpstr, 1);
			free(tmpstr1), tmpstr1 = NULL;
		}

		if(line != NULL)
		{
			menu = ostrcat("/tmp/tithek/solarmovie.search.list", NULL, 0, 0);
			writesys(menu, line, 0);
			struct tithek* tnode = (struct tithek*)listbox->select->handle;
			createtithek(tnode, tnode->title,  menu, tnode->pic, tnode->localname, tnode->menutitle, tnode->flag);
			ret = 0;
		}

		free(tmpstr), tmpstr = NULL;
		free(ip), ip = NULL;
	}
	free(search), search = NULL;
	return ret;
}

int solarmovie_hoster(struct skin* grid, struct skin* listbox, struct skin* countlabel, struct skin* load, char* link, char* title)
{
	debug(99, "link: %s", link);
	int ret = 1, series = 0;
	char* ip = NULL, *pos = NULL, *path = NULL, *etitle = NULL, *episode = NULL, *session = NULL, *update = NULL, *quality = NULL, *id = NULL, *line = NULL, *hname = NULL, *tmpstr = NULL, *tmpstr2 = NULL, *tmpstr3 = NULL, *cmd = NULL, *url = NULL, *tmpstr1 = NULL, *tmpfile = NULL, *tmphost = NULL;

	unlink("/tmp/tithek/get");
	unlink("/tmp/tithek/get_zcat");
	unlink("/tmp/tithek/get_zcat1");

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

	gethttp(ip, path, 80, "/tmp/tithek/get", NULL, 10000, NULL, 0);
	
	if(!file_exist("/tmp/tithek/get"))
	{
		textbox(_("Message"), _("This file doesn't exist, or has been removed") , _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 1200, 200, 0, 0);
		goto end;
	}
	if(getconfigint("debuglevel", NULL) == 99)
		system("cp -a /tmp/tithek/get /tmp/solarmovie1_tmpstr_get");

	tmpstr = command("cat /tmp/tithek/get");
	if(ostrstr(tmpstr, "<!DOCTYPE") == NULL)
	{
		cmd = ostrcat(cmd, "cat /tmp/tithek/get | zcat", 1, 0);
		debug(99, "cmd: %s", cmd);
		free(tmpstr), tmpstr = NULL;
		tmpstr = command(cmd);
		
		writesys("/tmp/tithek/get_zcat", tmpstr, 0);
		free(cmd), cmd = NULL;
	}
	else
		system("cp -a /tmp/tithek/get /tmp/tithek/get_zcat");

	if(getconfigint("debuglevel", NULL) == 99)
		writesys("/tmp/solarmovie2_tmpstr_zcat", tmpstr, 0);

	drawscreen(load, 0, 0);
	if(ostrstr(link, "/tv/") != NULL && ostrstr(link, "/season-") == NULL && ostrstr(link, "/episode-") == NULL)	
		series = 1;
	else
		series = 0;

	if(series == 0)
	{
		series = 0;
		if(file_exist("/tmp/tithek/get_zcat"))
		{
			free(tmpstr), tmpstr = NULL;
			tmpstr = command("cat /tmp/tithek/get_zcat");
			char* tmpcat = string_resub("<tbody>", "</tbody>", tmpstr, 0);
	
			char* ptmpcat = ostrstr(tmpcat, "<td class=\"qualit");
			int i = 0, incount = 0;
			while(ptmpcat != NULL)
			{
				i++;
				tmpstr1 = ostrcat(tmpstr1, string_resub("<td class=\"qualit", "<div class=\"linkDetails\">", ptmpcat, 0), 1, 1);
				ptmpcat += 5;
				ptmpcat = ostrstr(ptmpcat, "<td class=\"qualit");
	
				if(ptmpcat != NULL)
				{
					id = oregex(".*/link/show/(.*)/\">.*", tmpstr1);
					quality = string_resub("yCell\">", "</td>", tmpstr1, 0);
					update = string_resub("oddCell\">", "</td>", tmpstr1, 0);
					string_remove_whitechars(quality);
					strstrip(quality);
					hname = oregex(".*/\">(.*)</a>.*", tmpstr1);
					string_remove_whitechars(hname);
					strstrip(hname);
	
					tmphost = ostrcat("www.solarmovie.so", NULL, 0, 0);
					tmpfile = ostrcat("/link/play/", id, 0, 0);
		
					char* send = ostrcat(send, "GET /link/play/", 1, 0);
					send = ostrcat(send, id, 1, 0);
					send = ostrcat(send, " HTTP/1.1\r\nHost: www.solarmovie.so\r\nUser-Agent: Mozilla/5.0 (X11; Linux i686) AppleWebKit/535.1 (KHTML, like Gecko) Chrome/13.0.782.99 Safari/535.1\r\nConnection: close\r\nAccept-Encoding: gzip\r\n\r\n", 1, 0);	
					debug(99, "send: %s", send);
					unlink("/tmp/tithek/get1");
					gethttpreal(tmphost, tmpfile, 80, "/tmp/tithek/get1", NULL, NULL, 0, send, NULL, 5000, 0);
					free(send); send = NULL;
					free(tmphost); tmphost = NULL;
					free(tmpfile); tmpfile = NULL;
					if(!file_exist("/tmp/tithek/get1"))
					{
						printf("countinue\n");
						continue;
					}
					else
					{
						tmpstr1 = command("cat /tmp/tithek/get1");
						if(ostrstr(tmpstr1, "<!DOCTYPE") == NULL)
						{
							cmd = ostrcat(cmd, "cat /tmp/tithek/get1 | zcat", 1, 0);
							debug(99, "cmd: %s", cmd);
							free(tmpstr1), tmpstr1 = NULL;
							tmpstr1 = command(cmd);
							
							writesys("/tmp/tithek/get_zcat1", tmpstr1, 0);
							free(cmd), cmd = NULL;
						}
						else
							system("cp -a /tmp/tithek/get1 /tmp/tithek/get_zcat1");
			
			
						if(getconfigint("debuglevel", NULL) == 99)
							system("cp -a /tmp/tithek/get_zcat1 /tmp/solarmovie3_tmpstr_get_zcat1");
	
						char* tmpline = ostrcat("/tmp/solarmovie3_tmpstr_get_zcat_", oitoa(i), 0, 0);			
						if(getconfigint("debuglevel", NULL) == 99)
							writesys(tmpline, tmpstr1, 0);
						free(tmpline), tmpline = NULL;
	
						if(ostrstr(tmpstr1, "<div class=\"thirdPartyEmbContainer\">") != NULL)
						{
							tmpstr1 = string_resub("<div class=\"thirdPartyEmbContainer\">", "</div>", tmpstr1, 0);
							url = oregex(".*\"(http://.*)\".*", tmpstr1);
						}
						else
						{
							url = string_resub("<iframe name=\"service_frame\" src=\"", "\"", tmpstr1, 0);
							debug(99, "url: %s", url);
							url = string_replace_all("embed", "file", url, 1);
						}
						debug(99, "url: %s", url);
						tmpstr3 = ostrcat(url, NULL, 0, 0);
	
						int type = 0;
						int count2 = 0;
						struct splitstr* ret2 = NULL;
						ret2 = strsplit(tmpstr3, "= & / \"", &count2);
				
						if(ret2 != NULL && count2 > 3 && ostrcmp(hname, "sockshare.com") == 0)
						{
							tmpstr2 = ostrcat(ret2[3].part, NULL, 0, 0);
							hname = ostrcat("Sockshare.com", NULL, 0, 0);
							type = 14;
						}
						else if(ret2 != NULL && count2 > 3 && ostrcmp(hname, "putlocker.com") == 0)
						{
							url = ostrcat("http://www.putlocker.com/embed/", ret2[3].part, 0, 0);
							tmpstr2 = ostrcat(ret2[3].part, NULL, 0, 0);
							hname = ostrcat("Putlocker.com", NULL, 0, 0);
							type = 14;
						}
						else if(ret2 != NULL && count2 > 2 && ostrcmp(hname, "filenuke.com") == 0)
						{
							tmpstr2 = ostrcat(ret2[2].part, NULL, 0, 0);
							hname = ostrcat("FileNuke.com", NULL, 0, 0);
							type = 15;
						}
						else if(ret2 != NULL && count2 > 2 && ostrcmp(hname, "streamcloud.eu") == 0)
						{
							tmpstr2 = ostrcat(ret2[2].part, NULL, 0, 0);
							hname = ostrcat("StreamCloud.eu", NULL, 0, 0);
							type = 20;
						}
						else if(ret2 != NULL && count2 > 2 && ostrcmp(hname, "vidstream.in") == 0)
						{
							tmpstr2 = ostrcat(ret2[2].part, NULL, 0, 0);
							hname = ostrcat("VidStream.in", NULL, 0, 0);
							type = 25;
						}
						else if(ret2 != NULL && count2 > 3 && ostrcmp(hname, "flashx.tv") == 0)
						{
							tmpstr2 = ostrcat(ret2[3].part, NULL, 0, 0);
							hname = ostrcat("FlashX.tv", NULL, 0, 0);
							type = 24;
						}
						else if(ret2 != NULL && count2 > 3 && ostrcmp(hname, "primeshare.tv") == 0)
						{
							tmpstr2 = ostrcat(ret2[3].part, NULL, 0, 0);
							hname = ostrcat("PrimeShare.tv", NULL, 0, 0);
						}				
						else if(ret2 != NULL && count2 > 2 && ostrcmp(hname, "xvidstage.com") == 0)
						{
							tmpstr2 = ostrcat(ret2[2].part, NULL, 0, 0);
							tmpstr2 = string_replace_all("embed-", "", tmpstr2, 1);
							tmpstr2 = string_replace_all(".html", "", tmpstr2, 1);
							hname = ostrcat("XvidStage.com", NULL, 0, 0);
							type = 26;
						}
						else if(ret2 != NULL && count2 > 3 && ostrcmp(hname, "vidxden.com") == 0)
						{
							tmpstr2 = ostrcat(ret2[3].part, NULL, 0, 0);
							hname = ostrcat("Vidxden.com (DivX)", NULL, 0, 0);
						}
						else if(ret2 != NULL && count2 > 3 && ostrcmp(hname, "nowvideo.eu") == 0)
						{
							url = ostrcat("http://embed.nowvideo.eu/embed.php?v=", ret2[3].part, 0, 0);
							tmpstr2 = ostrcat(ret2[3].part, NULL, 0, 0);
							type = 27;
							hname = ostrcat("NowVideo.eu", NULL, 0, 0);
						}
						else if(ret2 != NULL && count2 > 2)
						{
							tmpstr2 = ostrcat(ret2[2].part, NULL, 0, 0);
	//						hname = ostrcat(hname, " (coming soon)", 0, 0);
							type = 66;
						}
						else
						{
							tmpstr2 = ostrcat("unknown", NULL, 0, 0);
	//						hname = ostrcat(hname, " (coming soon)", 0, 0);
							type = 66;
						}
	
	
						free(ret2), ret2 = NULL;
						debug(99, "-------------------------------");
						debug(99, "hname: %s url: %s id: %s", hname, url, tmpstr2);
	
						incount += 1;
						line = ostrcat(line, hname, 1, 0);
						if(quality != NULL)
						{
							line = ostrcat(line, " (", 1, 0);
							line = ostrcat(line, quality, 1, 0);
							line = ostrcat(line, ")", 1, 0);
						}
						if(update != NULL)
						{
							line = ostrcat(line, " (", 1, 0);
							line = ostrcat(line, update, 1, 0);
							line = ostrcat(line, ")", 1, 0);
						}
						line = ostrcat(line, "#", 1, 0);
						line = ostrcat(line, url, 1, 0);
						line = ostrcat(line, ";", 1, 0);
						line = ostrcat(line, tmpstr2, 1, 0);
						line = ostrcat(line, ";", 1, 0);				
						line = ostrcat(line, hname, 1, 0);
						line = ostrcat(line, "#", 1, 0);
						line = ostrcat(line, "http://atemio.dyndns.tv/mediathek/menu/", 1, 0);
						line = ostrcat(line, hname, 1, 0);
						line = ostrcat(line, ".jpg#solarmovie_", 1, 0);
						line = ostrcat(line, hname, 1, 0);
						line = ostrcat(line, ".jpg#Solarmovie - ", 1, 0);
						line = ostrcat(line, title, 1, 0);
						line = ostrcat(line, "#", 1, 0);
						line = ostrcat(line, oitoa(type), 1, 0);
						line = ostrcat(line, "\n", 1, 0);
	
						free(tmpstr3), tmpstr3 = NULL;
						free(tmpstr2), tmpstr2 = NULL;
						free(hname), hname = NULL;
						free(url), url = NULL;
						free(quality), quality = NULL;
						free(update), update = NULL;
					}
					free(hname), hname = NULL;
					free(id), id = NULL;		
				}
			}
			free(tmpcat); tmpcat = NULL;
		}	
	}
	else
	{
		series = 1;
		if(file_exist("/tmp/tithek/get_zcat"))
		{
			free(tmpstr), tmpstr = NULL;
			tmpstr = command("cat /tmp/tithek/get_zcat | grep episode- | grep -v Episode | grep -v 'linkCount typicalGrey'");
			writesys("/var/usr/local/share/titan/plugins/tithek/solarmovie8_tmpstr", tmpstr, 0);

			int count = 0;
			struct splitstr* ret1 = NULL;
			ret1 = strsplit(tmpstr, "\n", &count);
			int max = count;
			int i = 0, incount = 0;
			for(i = 0; i < max; i++)
			{
//<a href="/tv/avatar-the-last-airbender-2005/season-3/episode-2/">The Headband</a>
				session = oregex(".*/season-([0-9]{1,2})/episode-.*", ret1[i].part);
				episode = oregex(".*/episode-([0-9]{1,2})/.*", ret1[i].part);
				etitle = oregex(".*>(.*)<.*", ret1[i].part);
				url = string_resub("\"", "\"", ret1[i].part, 0);
printf("(s%se%s) (%s) = %s\n",session , episode, etitle, url);				
				incount += 1;
				line = ostrcat(line, _("Season"), 1, 0);
				line = ostrcat(line, " ", 1, 0);
				line = ostrcat(line, session, 1, 0);
				line = ostrcat(line, " ", 1, 0);
				line = ostrcat(line, _("Episode"), 1, 0);
				line = ostrcat(line, " ", 1, 0);
				line = ostrcat(line, episode, 1, 0);

//				line = ostrcat(line, "(s", 1, 0);
//				line = ostrcat(line, session, 1, 0);
//				line = ostrcat(line, "e", 1, 0);
//				line = ostrcat(line, episode, 1, 0);
//				line = ostrcat(line, ") ", 1, 0);
//				line = ostrcat(line, etitle, 1, 0);

				line = ostrcat(line, "#http://www.solarmovie.so", 1, 0);
				line = ostrcat(line, url, 1, 0);
//				line = ostrcat(line, ";", 1, 0);
//				line = ostrcat(line, session, 1, 0);
//				line = ostrcat(line, ";", 1, 0);
//				line = ostrcat(line, episode, 1, 0);
				line = ostrcat(line, "#", 1, 0);
				line = ostrcat(line, "http://atemio.dyndns.tv/mediathek/menu/s", 1, 0);
				line = ostrcat(line, session, 1, 0);
				line = ostrcat(line, "e", 1, 0);
				line = ostrcat(line, episode, 1, 0);
				line = ostrcat(line, ".jpg", 1, 0);																
				line = ostrcat(line, "#s", 1, 0);
				line = ostrcat(line, session, 1, 0);
				line = ostrcat(line, "e", 1, 0);
				line = ostrcat(line, episode, 1, 0);
				line = ostrcat(line, ".jpg#Solarmovie - ", 1, 0);
				line = ostrcat(line, title, 1, 0);
				line = ostrcat(line, "#28\n", 1, 0);
				free(session), session = NULL;
				free(episode), episode = NULL;
				free(url), url = NULL;	
				free(etitle), etitle = NULL;	
			}
		}
	}
	free(tmpstr), tmpstr = NULL;

	if(line != NULL)
	{
		if(series == 0)
			tmpstr = ostrcat("/tmp/tithek/solarmovie.hoster.list", NULL, 0, 0);
		else
			tmpstr = ostrcat("/tmp/tithek/solarmovie.hoster.ser.list", NULL, 0, 0);
		writesys(tmpstr, line, 0);

		if(getconfigint("debuglevel", NULL) == 99)
			writesys("/tmp/solarmovie8_line", line, 0);
					
		struct tithek* tnode = (struct tithek*)listbox->select->handle;
		createtithek(tnode, tnode->title,  tmpstr, tnode->pic, tnode->localname, tnode->menutitle, tnode->flag);
		ret = 0;
	}
end:
	free(ip), ip = NULL;
			
	return ret;
}
#endif