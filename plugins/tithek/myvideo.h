#ifndef MYVIDEO_H
#define MYVIDEO_H

// flag 1 = getstreamurl
//http://www.myvideo.de/dynamic/get_player_video_xml.php?domain=www.myvideo.de&flash_playertype=D&ds=1&autorun=yes&ID=1770991

//http://www.myvideo.de/dynamic/get_player_video_xml.php?domain=www.myvideo.de&flash_playertype=D&ds=1&autorun=yes&ID=9693945

char* myvideo_hoster(char* link)
{
	debug(99, "link: %s", link);
	char* streamurl = NULL;

	streamurl = hoster(link);
	debug(99, "streamurl1: %s", streamurl);

	streamurl = string_replace_all("amp;", "", streamurl, 1);
	debug(99, "streamurl2: %s", streamurl);

	return streamurl;
}

char* myvideo(char* link)
{
	debug(99, "link: %s", link);
	int debuglevel = getconfigint("debuglevel", NULL);
	char* ip = NULL, *pos = NULL, *path = NULL, *tmplink = NULL, *pageUrl = NULL, *playpath = NULL, *video_id = NULL, *source = NULL, *streamurl = NULL, *tmpstr_uni = NULL, *b64 = NULL, *key = NULL, *newurl = NULL, *tmpstr = NULL, *tmppath = NULL, *error = NULL;
	int flag = 1;

/*
	if(flag == 1)
	{
		int count = 0;
		struct splitstr* ret1 = NULL;
		ret1 = strsplit(input, ";", &count);
		if(ret1 != NULL && count >= 4)
		{
			link = ostrcat(ret1[0].part, NULL, 0, 0);
			pageUrl = ostrcat(pageUrl, ret1[1].part, 1, 0);
			playpath = ostrcat(playpath, ret1[2].part, 1, 0);
			video_id = ostrcat(video_id, ret1[3].part, 1, 0);									
		}
		free(ret1), ret1 = NULL;
	}	
*/
	stringreplacechar(link, ';', '\0');

	if(ostrstr(link, "/watch/") != NULL)
	{
//		video_id = oregex(".*/watch/.*(.*)/.*", link);
		video_id = string_resub("/watch/", "/", link, 0);
		tmplink = ostrcat("http://www.myvideo.de/dynamic/get_player_video_xml.php?domain=www.myvideo.de&flash_playertype=D&ds=1&autorun=yes&ID=", video_id, 0, 0);
		tmplink = ostrcat(tmplink, "&_countlimit=4;", 1, 0);
		free(video_id), video_id = NULL;
	}
	else
		tmplink = ostrcat(link, NULL, 0, 0);

	ip = string_replace("http://", "", tmplink, 0);

	if(ip != NULL)
		pos = strchr(ip, '/');
	if(pos != NULL)
	{
		pos[0] = '\0';
		path = pos + 1;
	}

	video_id = string_resub("&ID=", "&", tmplink, 0);

	tmppath = ostrcat("watch/", video_id , 0, 0);
	tmppath = ostrcat(tmppath, "/" , 1, 0);
	tmpstr = gethttp(ip, tmppath, 80, NULL, NULL, 10000, NULL, 0);
	error = string_resub("<div class='lContent lContNoBorder error sBold sCenter'>", "</div>", tmpstr, 0);

	unlink("/tmp/myvideo_tmpstr");
	unlink("/tmp/myvideo_tmpstr_uni");
	unlink("/tmp/myvideo_tmpstr_error");

	titheklog(debuglevel, "/var/usr/local/share/titan/plugins/tithek/myvideo_tmpstr", NULL, NULL, NULL, tmpstr);

	titheklog(debuglevel, "/tmp/myvideo_tmpstr_error", NULL, NULL, NULL, tmpstr);
				
	if(ostrstr(error, "<div class='error sBold' id='error_screen_body'>") == NULL && error != NULL)
	{
		textbox(_("Message"), _(error) , _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 1200, 200, 0, 0);
		debug(99, "error msg: %s", error);
	}	
	
	debug(99, "error link: http://%s/%s", ip, tmppath);
	free(tmpstr), tmpstr = NULL;
	free(tmppath), tmppath = NULL;
	free(error), error = NULL;
	
// not working tmpstr hat zusatzdaten im string
	tmpstr = gethttp(ip, path, 80, NULL, NULL, 10000, NULL, 0);

	debug(99, "link: http://%s/%s", ip, path);
	titheklog(debuglevel, "/var/usr/local/share/titan/plugins/tithek/myvideo_tmpstr2", NULL, NULL, NULL, tmpstr);

	titheklog(debuglevel, "/tmp/myvideo_tmpstr", NULL, NULL, NULL, tmpstr);

/*
// work start
	unlink("/tmp/myvideo_tmp");
	char* cmd = NULL;
	cmd = ostrcat("wget \"", input, 0, 0);
	cmd = ostrcat(cmd, "\" -O /tmp/myvideo_tmp", 1, 0);
	system(cmd);
	free(cmd), cmd = NULL;
	tmpstr = readfiletomem("/tmp/myvideo_tmp", 0);
// work end
*/
	if(flag == 1)
	{
		int count = 0;
		struct splitstr* ret1 = NULL;
		ret1 = strsplit(tmpstr, "=", &count);
		int hlen = 0;

		if(ret1 != NULL && count >= 2)
		{
//			debug(99, "ret1[1].part=%s", (ret1[1]).part);
			hlen = strlen(ret1[1].part);
			tmpstr_uni = unhexlify(ret1[1].part);
		}
		free(ret1), ret1 = NULL;

		if(video_id != NULL)
			b64 = ostrcat("c8407a08b3c71ea418ec9dc662f2a56e40cbd6d5a114aa50fb1e1079e17f2b83", MDString(video_id), 0, 1);
		debug(99, "b64=%s", b64);

		if(b64 != NULL)
			key = MDString(b64);
		int slen = 0;
		int klen = 0;
		if(tmpstr_uni != NULL) slen = strlen(tmpstr_uni);
		if(key != NULL) klen = strlen(key);
		
		if(tmpstr_uni != NULL)
		{
			debug(99, "hexlen=%d", hlen);
			hlen /= 2;
			debug(99, "binlen=%d", hlen);
			debug(99, "keylen=%d", klen);
			debug(99, "b64=%s", b64);
			debug(99, "key=%s", key);

			rc4(tmpstr_uni, hlen, key, klen);

			debug(99, "encrypted=%s", tmpstr_uni);
			debug(99, "pageUrl: %s\n", pageUrl);
			debug(99, "playpath: %s\n", playpath);
			debug(99, "video_id: %s\n", video_id);
			//printf("tmpstr_uni: %s\n",tmpstr_uni);

			htmldecode(tmpstr_uni, tmpstr_uni);

			titheklog(debuglevel, "/tmp/myvideo_tmpstr_uni", NULL, NULL, NULL, tmpstr_uni);

			if(ostrstr(tmpstr_uni, "connectionurl='rtmp"))
			{
				printf("found rtmpe:// stream\n");
				source = string_resub("source='", ".flv'", tmpstr_uni, 0);

				newurl = string_resub("connectionurl='", "'", tmpstr_uni, 0);

/*
				if(ostrstr(newurl, "myvideo2flash"))
				{
					printf("change to rtmpt:// stream\n");
					newurl = string_replace("rtmpe://", "rtmpt://", newurl, 1);
				}
*/
				newurl = string_replace("rtmpe://", "rtmp://", newurl, 1);
	
				streamurl = ostrcat(newurl, NULL, 0, 0);
				streamurl = ostrcat(streamurl, " ", 1, 0);
				streamurl = ostrcat(streamurl, "tcUrl=", 1, 0);
				streamurl = ostrcat(streamurl, newurl, 1, 0);
				streamurl = ostrcat(streamurl, " swfVfy=http://is4.myvideo.de/de/player/mingR11q/ming.swf ", 1, 0);
				streamurl = ostrcat(streamurl, pageUrl, 1, 0);
				streamurl = ostrcat(streamurl, " ", 1, 0);
				streamurl = ostrcat(streamurl, "playpath=flv:", 1, 0);
				streamurl = ostrcat(streamurl, source, 1, 0);		
			}
			else
			{		
				printf("rtmpe not found, change to *.flv stream\n");
				source = string_resub("source='", "'", tmpstr_uni, 0);
	
				newurl = string_resub("path='", "'", tmpstr_uni, 0);
	
				streamurl = ostrcat(newurl, source, 0, 0);
			}
		}
	}

	free(key), key = NULL;		
	free(b64), b64 = NULL;
	free(tmpstr_uni), tmpstr_uni = NULL;		
	free(tmplink), tmplink = NULL;
	free(source), source = NULL;
	free(tmpstr), tmpstr = NULL;
	free(pageUrl), pageUrl = NULL;		
	free(playpath), playpath = NULL;
	free(ip), ip = NULL;
	free(video_id), video_id = NULL;
	free(newurl), newurl = NULL;
// segfault munmap_chunk(): invalid pointer
//	free(pos), pos = NULL;
//	free(path), path = NULL;

//	debug(99, "streamurl2: %s", streamurl);	
	return streamurl;
}

int myvideo_search(struct skin* grid, struct skin* listbox, struct skin* countlabel, struct skin* load, char* link, char* title, char* searchstr, int flag)
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

	if(search != NULL || flag > 0)
	{
		drawscreen(load, 0, 0);
		search = stringreplacechar(search, ' ', '+');
		char* id = NULL;
		char* line = NULL;
		char* pic = NULL;
		char* title = NULL;
		char* menu = NULL;
		char* path = NULL;
		char* ip = ostrcat("www.myvideo.de", NULL, 0, 0);

		if(flag == 0)
			path = ostrcat("Videos_A-Z?searchWord=", search, 0, 0);
		else
			path = ostrcat("Top_100/Top_100_Single_Charts", NULL, 0, 0);
		
		char* tmpstr = NULL;
		char* tmpstr1 = NULL;
		tmpstr = gethttp(ip, path, 80, NULL, NULL, 10000, NULL, 0);
		titheklog(debuglevel, "/var/usr/local/share/titan/plugins/tithek/myvideo1_search_tmpstr", NULL, NULL, NULL, tmpstr);

		if(flag == 0)
		{
			tmpstr = string_replace_all("<", "\n", tmpstr, 1);
			tmpstr = string_replace_all(">", "\n", tmpstr, 1);
			titheklog(debuglevel, "/var/usr/local/share/titan/plugins/tithek/myvideo2_search_tmpstr_replace", NULL, NULL, NULL, tmpstr);
		}
		else
		{
			tmpstr1 = string_resub("MV.contentLists.chartlist = {", "</script>", tmpstr, 0);	
			titheklog(debuglevel, "/var/usr/local/share/titan/plugins/tithek/myvideo2_search_tmpstr1_resub", NULL, NULL, NULL, tmpstr1);
			free(tmpstr), tmpstr = NULL;
			tmpstr = ostrcat(tmpstr1, NULL, 0, 0);
			free(tmpstr1), tmpstr1 = NULL;			
		}
		
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
				debug(99, "1111ret1[i].part=%s", ret1[i].part);					

				if(ostrstr(ret1[i].part, "img id='i") != NULL)
				{
					debug(99, "---------------------------");
					debug(99, "ret1[i].part: %s", ret1[i].part);
					int rcret = waitrc(NULL, 10, 0);
					if(rcret == getrcconfigint("rcexit", NULL)) break;
					pic = oregex(".*longdesc='(.*)' class='vThumb.*", ret1[i].part);
					id = oregex(".*img id='i(.*)' onload=.*", ret1[i].part);
					title = oregex(".*alt='(.*)' onmouseover=.*", ret1[i].part);
					debug(99, "title: %s", title);
					debug(99, "pic: %s", pic);
					debug(99, "id: %s", id);
					debug(99, "---------------------------");
				}
				else if(ostrstr(ret1[i].part, "{\"id\":") != NULL)
				{
					debug(99, "---------------------------");
					debug(99, "ret1[i].part: %s", ret1[i].part);
					int rcret = waitrc(NULL, 10, 0);
					if(rcret == getrcconfigint("rcexit", NULL)) break;
//					pic = oregex(".*{\"id\":(.*),\".*", ret1[i].part);
//					id = oregex(".*{\"id\":(.*),\".*", ret1[i].part);
//					title = oregex(".*alt='(.*)' onmouseover=.*", ret1[i].part);

					id = string_resub("{\"id\":", ",\"", ret1[i].part, 0);	
					pic = string_resub("\"thumbnail\":\"", "\",", ret1[i].part, 0);	
					title = string_resub("\"title\":\"", "\",", ret1[i].part, 0);	
					pic = string_replace_all("\\", "", pic, 1);
			
					debug(99, "title: %s", title);
					debug(99, "pic: %s", pic);
					debug(99, "id: %s", id);
					debug(99, "---------------------------");
				}
				if(id != NULL)
				{
					incount += 1;
					line = ostrcat(line, title, 1, 0);
//						line = ostrcat(line, "#http://www.myvideo.de/dynamic/get_player_video_xml.php?flash_playertype=SER&ID=", 1, 0);
// de fix
					line = ostrcat(line, "#http://www.myvideo.de/dynamic/get_player_video_xml.php?domain=www.myvideo.de&flash_playertype=D&ds=1&autorun=yes&ID=", 1, 0);
					line = ostrcat(line, id, 1, 0);
//						line = ostrcat(line, "&_countlimit=4&autorun=yes;pageUrl=http://www.myvideo.de/watch/", 1, 0);										
// de fix
					line = ostrcat(line, "&_countlimit=4;pageUrl=http://www.myvideo.de/watch/", 1, 0);										
					line = ostrcat(line, id, 1, 0);
					line = ostrcat(line, "/;playpath=flv:movie24/a0/", 1, 0);
					line = ostrcat(line, id, 1, 0);
					line = ostrcat(line, ";", 1, 0);
					line = ostrcat(line, id, 1, 0);																				
					line = ostrcat(line, "#", 1, 0);
					line = ostrcat(line, pic, 1, 0);
					line = ostrcat(line, "#myvideo_search_", 1, 0);
					line = ostrcat(line, oitoa(incount + time(NULL)), 1, 0);
					line = ostrcat(line, ".jpg#MyVideo - Search#12\n", 1, 0);
					free(ip), ip = NULL;
					free(path), path = NULL;
					free(title), title = NULL;
				}
				free(title), title = NULL;
				free(pic), pic = NULL;
				free(id), id = NULL;
			}
			free(ret1), ret1 = NULL;

			if(line != NULL)
			{
				menu = ostrcat("/tmp/tithek/myvideo.search.list", NULL, 0, 0);
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

int myvideo_search_local(struct skin* grid, struct skin* listbox, struct skin* countlabel, struct skin* load, char* link, char* title, char* searchstr, int flag)
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

		tmpstr = gethttp("openaaf.dyndns.tv", "/mediathek/myvideo/streams/myvideo.all-sorted.list", 80, NULL, HTTPAUTH, 5000, NULL, 0);

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
				menu = ostrcat("/tmp/tithek/myvideo.search.list", NULL, 0, 0);
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

#endif
