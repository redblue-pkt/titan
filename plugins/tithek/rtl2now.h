#ifndef RTL2NOW_H
#define RTL2NOW_H

// flag 1 = get getstreamurl http
// flag 2 = getstreamurl

char* now(char* link, char* url, char* name, char* title, int flag)
{
	debug(99, "link(%d): %s", flag, link);
	char* ip = NULL, *pos = NULL, *path = NULL, *streamurl = NULL, *newlink = NULL, *newpath = NULL;

	ip = string_replace("http://", "", (char*)link, 0);

	if(ip != NULL)
		pos = strchr(ip, '/');
	if(pos != NULL)
	{
		pos[0] = '\0';
		path = pos + 1;
	}

	char* tmpstr = NULL;
	tmpstr = gethttp(ip, path, 80, NULL, NULL, 10000, NULL, 0);
printf("00000000\n");

	if(flag == 1)
	{
printf("111111111\n");

//		if(ostrstr(tmpstr, "<!-- 3-->") == NULL)
//			textbox(_("Message"), _("Found Pay Stream visit the official Website and Order this Stream and try this again !") , _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 1200, 400, 0, 0);

		if(ostrstr(title, "Euro)") != NULL)
			textbox(_("Message"), _("Found Pay Stream visit the official Website and Order this Stream and try this again !") , _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 1200, 400, 0, 0);

		tmpstr = string_resub("data:'", "',", tmpstr, 0);
		debug(99, "tmpstr: %s", tmpstr);

		htmldecode(tmpstr, tmpstr);
		tmpstr = ostrcat(url, tmpstr, 0, 1);
		debug(99, "streamurl: %s", tmpstr);
		streamurl = now(tmpstr, url, name, title, 2);
		free(tmpstr); tmpstr = NULL;
	}
	else if(flag == 2)
	{
printf("22222222\n");

		if(ostrstr(tmpstr, "rtmpe://") != NULL)
		{
printf("22222222aaaaaaaaaaa\n");

			tmpstr = string_resub("rtmpe://", ".f4v", tmpstr, 0);

			char* tmpstr9 = NULL;
			tmpstr9 = ostrcat(tmpstr9, tmpstr, 1, 0);
			free(tmpstr), tmpstr = NULL;
			tmpstr = ostrcat("rtmpe://", tmpstr9, 0, 0);
			tmpstr = ostrcat(tmpstr, ".f4v", 1, 0);		
			free(tmpstr9), tmpstr9 = NULL;
	
			debug(99, "tmpstr: %s", tmpstr);
	
			int count = 0;
			int i = 0;
			struct splitstr* ret1 = NULL;
			ret1 = strsplit(tmpstr, "/", &count);
			if(ret1 != NULL)
			{
				int max = count;
				for(i = 0; i < max; i++)
				{
					if(i < 3)
					{
						if(count > i)
							newlink = ostrcat(newlink, (&ret1[i])->part, 1, 0);
	
						if(i == 0)
							newlink = ostrcat(newlink, "//", 1, 0);
						else
							newlink = ostrcat(newlink, "/", 1, 0);
					}
					else
					{
						if(count > i)
							newpath = ostrcat(newpath, (&ret1[i])->part, 1, 0);
						if(i != max - 1)
							newpath = ostrcat(newpath, "/", 1, 0);
					}
				}
				free(ret1), ret1 = NULL;
	
				debug(99, "newlink: %s", newlink);
				debug(99, "newpath: %s", newpath);
		
				streamurl = ostrcat(newlink, " swfVfy=1 playpath=mp4:", 0, 0);
				streamurl = ostrcat(streamurl, newpath, 1, 0);
				streamurl = ostrcat(streamurl, " app=", 1, 0);
				streamurl = ostrcat(streamurl, name, 1, 0);
				streamurl = ostrcat(streamurl, "/_definst_ pageUrl=", 1, 0);
				streamurl = ostrcat(streamurl, url, 1, 0);
				streamurl = ostrcat(streamurl, "/p/ tcUrl=", 1, 0);
				streamurl = ostrcat(streamurl, newlink, 1, 0);
				streamurl = ostrcat(streamurl, " swfUrl=", 1, 0);
				streamurl = ostrcat(streamurl, url, 1, 0);
				streamurl = ostrcat(streamurl, "/includes/vodplayer.swf", 1, 0);
			}
			free(tmpstr); tmpstr = NULL;
			debug(99, "streamurl: %s", streamurl);
		}
		else
		{	
printf("2222222bbbbbbbbbb\n");

			tmpstr = string_resub("<filename", "</filename>", tmpstr, 1);
			tmpstr = string_resub("<![CDATA[", "]]", tmpstr, 1);

			int count2 = 0;	
			struct splitstr* ret2 = NULL;
			ret2 = strsplit(tmpstr, "/", &count2);
			if(count2 > 6)
			{
//				ret2[6].part = string_replace_remove_last_chars(".f4m?ts=", "", ret2[6].part, 0);
				ret2[6].part = string_replace_remove_last_chars(".f4m", "", ret2[6].part, 0);

printf("2222222cccccccccc\n");

				srand(time(NULL));
				int r = rand() % 34;
				printf("random.randint1: %d\n",r);
				if(r == 0)
				{
					sleep(1);
					srand(time(NULL));
					r = rand() % 34;
					printf("random.randint2: %d\n",r);
				}
				if(r == 0)
				{
					sleep(1);
					srand(time(NULL));
					r = rand() % 34;
					printf("random.randint3: %d\n",r);
				}
				if(r == 0)
				{
					sleep(1);
					srand(time(NULL));
					r = rand() % 34;
					printf("random.randint4: %d\n",r);
				}

				streamurl = ostrcat("rtmpe://fms-fra", oitoa(r), 0, 0);
//				streamurl = ostrcat("rtmpe://fms-fra26.rtl.de/", ret2[3].part, 0, 0);
				streamurl = ostrcat(streamurl, ".rtl.de/", 1, 0);
				streamurl = ostrcat(streamurl, ret2[3].part, 1, 0);
				streamurl = ostrcat(streamurl, "/", 1, 0);
				streamurl = ostrcat(streamurl, " playpath=mp4:", 1, 0);
				streamurl = ostrcat(streamurl, ret2[5].part, 1, 0);
				streamurl = ostrcat(streamurl, "/", 1, 0);
				streamurl = ostrcat(streamurl, ret2[6].part, 1, 0);
				streamurl = ostrcat(streamurl, " swfVfy=1", 1, 0);
				streamurl = ostrcat(streamurl, " swfUrl=http://", 1, 0);
				streamurl = ostrcat(streamurl, ip, 1, 0);
				streamurl = ostrcat(streamurl, "/includes/vodplayer.swf", 1, 0);
				streamurl = ostrcat(streamurl, " app=", 1, 0);
				streamurl = ostrcat(streamurl, ret2[3].part, 1, 0);
				streamurl = ostrcat(streamurl, "/_definst_", 1, 0);
				streamurl = ostrcat(streamurl, " pageUrl=", 1, 0);			
				streamurl = ostrcat(streamurl, link, 1, 0);
			}
			else if(count2 > 5)
			{
//				ret2[6].part = string_replace_remove_last_chars(".f4m?ts=", "", ret2[6].part, 0);
				ret2[5].part = string_replace_remove_last_chars(".f4m", "", ret2[5].part, 0);

printf("2222222ccccccc11111111111\n");

				srand(time(NULL));
				int r = rand() % 34;
				printf("random.randint1: %d\n",r);
				if(r == 0)
				{
					sleep(1);
					srand(time(NULL));
					r = rand() % 34;
					printf("random.randint2: %d\n",r);
				}
				if(r == 0)
				{
					sleep(1);
					srand(time(NULL));
					r = rand() % 34;
					printf("random.randint3: %d\n",r);
				}
				if(r == 0)
				{
					sleep(1);
					srand(time(NULL));
					r = rand() % 34;
					printf("random.randint4: %d\n",r);
				}

				streamurl = ostrcat("rtmpe://fms-fra", oitoa(r), 0, 0);
//				streamurl = ostrcat("rtmpe://fms-fra26.rtl.de/", ret2[3].part, 0, 0);
				streamurl = ostrcat(streamurl, ".rtl.de/", 1, 0);
				streamurl = ostrcat(streamurl, ret2[3].part, 1, 0);
				streamurl = ostrcat(streamurl, "/", 1, 0);
				streamurl = ostrcat(streamurl, " playpath=mp4:", 1, 0);
				streamurl = ostrcat(streamurl, ret2[5].part, 1, 0);
				streamurl = ostrcat(streamurl, "/", 1, 0);
//		streamurl = ostrcat(streamurl, ret2[5].part, 1, 0);
				streamurl = ostrcat(streamurl, " swfVfy=1", 1, 0);
				streamurl = ostrcat(streamurl, " swfUrl=http://", 1, 0);
				streamurl = ostrcat(streamurl, ip, 1, 0);
				streamurl = ostrcat(streamurl, "/includes/vodplayer.swf", 1, 0);
				streamurl = ostrcat(streamurl, " app=", 1, 0);
				streamurl = ostrcat(streamurl, ret2[3].part, 1, 0);
				streamurl = ostrcat(streamurl, "/_definst_", 1, 0);
				streamurl = ostrcat(streamurl, " pageUrl=", 1, 0);			
				streamurl = ostrcat(streamurl, link, 1, 0);
			}
			
			free(ret2), ret2 = NULL;
		}
	}


	free(ip), ip = NULL;
	free(newlink), newlink = NULL;
	free(newpath), newpath = NULL;
// segfault munmap_chunk(): invalid pointer
//	free(pos), pos = NULL;
//	free(path), path = NULL;
	
	debug(99, "streamurl2: %s", streamurl);	
	return streamurl;
}

int rtl2now_search_local(struct skin* grid, struct skin* listbox, struct skin* countlabel, struct skin* load, char* link, char* title, char* searchstr, int flag)
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

		tmpstr = gethttp("openaaf.dyndns.tv", "/mediathek/rtl2now/streams/rtl2now.all-sorted.list", 80, NULL, HTTPAUTH, 5000, NULL, 0);

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
				menu = ostrcat("/tmp/tithek/rtl2now.search.list", NULL, 0, 0);
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

int rtlnow_search_local(struct skin* grid, struct skin* listbox, struct skin* countlabel, struct skin* load, char* link, char* title, char* searchstr, int flag)
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

		tmpstr = gethttp("openaaf.dyndns.tv", "/mediathek/rtl-now/streams/rtl-now.all-sorted.list", 80, NULL, HTTPAUTH, 5000, NULL, 0);

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
				menu = ostrcat("/tmp/tithek/rtl-now.search.list", NULL, 0, 0);
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

int superrtlnow_search_local(struct skin* grid, struct skin* listbox, struct skin* countlabel, struct skin* load, char* link, char* title, char* searchstr, int flag)
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

		tmpstr = gethttp("openaaf.dyndns.tv", "/mediathek/superrtlnow/streams/superrtlnow.all-sorted.list", 80, NULL, HTTPAUTH, 5000, NULL, 0);

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
				menu = ostrcat("/tmp/tithek/superrtlnow.search.list", NULL, 0, 0);
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

int voxnow_search_local(struct skin* grid, struct skin* listbox, struct skin* countlabel, struct skin* load, char* link, char* title, char* searchstr, int flag)
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

		tmpstr = gethttp("openaaf.dyndns.tv", "/mediathek/voxnow/streams/voxnow.all-sorted.list", 80, NULL, HTTPAUTH, 5000, NULL, 0);

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
				menu = ostrcat("/tmp/tithek/voxnow.search.list", NULL, 0, 0);
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

int rtlnitronow_search_local(struct skin* grid, struct skin* listbox, struct skin* countlabel, struct skin* load, char* link, char* title, char* searchstr, int flag)
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

		tmpstr = gethttp("openaaf.dyndns.tv", "/mediathek/rtlnitronow/streams/rtlnitronow.all-sorted.list", 80, NULL, HTTPAUTH, 5000, NULL, 0);

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
				menu = ostrcat("/tmp/tithek/rtlnitronow.search.list", NULL, 0, 0);
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

int ntvnow_search_local(struct skin* grid, struct skin* listbox, struct skin* countlabel, struct skin* load, char* link, char* title, char* searchstr, int flag)
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

		tmpstr = gethttp("openaaf.dyndns.tv", "/mediathek/n-tvnow/streams/n-tvnow.all-sorted.list", 80, NULL, HTTPAUTH, 5000, NULL, 0);

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
				menu = ostrcat("/tmp/tithek/n-tvnow.search.list", NULL, 0, 0);
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
