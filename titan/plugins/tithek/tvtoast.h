#ifndef TVTOAST_H
#define TVTOAST_H

char* zerocast(char* link, int incount)
{
	debug(99, "zerocast(%d) link=%s ", incount, link);
	char* streamurl = NULL;
	char* host = NULL;

	host = string_resub("http://", "/", link, 0);
	free(host), host = NULL;

	debug(99, "streamurl %s", streamurl);
	return streamurl;
}

char* usachannels(char* link, int incount)
{
	debug(99, "usachannels(%d) link=%s", incount, link);
	char* streamurl = NULL;
	char* host = NULL;

	host = string_resub("http://", "/", link, 0);
	free(host), host = NULL;

	debug(99, "streamurl %s", streamurl);
	return streamurl;
}

char* tvtoast(char* link)
{
	debug(99, "link %s", link);
	int debuglevel = getconfigint("debuglevel", NULL);
	char* streamurl = NULL, *tmpstr = NULL, *tmpstr1 = NULL;
	char* title = NULL, *pic = NULL;
	char* typemsg = NULL;
	char* host = NULL;
	char* path = NULL;
	char* url = NULL;

	system("rm -rf /var/usr/local/share/titan/plugins/tithek/tvtoast*");

	host = string_resub("http://", "/", link, 0);
	path = string_replace_all(host, "", link, 0);
	path = string_replace_all("http://", "", path, 1);
	path = string_replace_all(" ", "%20", path, 1);
	tmpstr = gethttps(link, NULL, NULL, NULL, NULL, NULL, 1);
	titheklog(debuglevel, "/var/usr/local/share/titan/plugins/tithek/tvtoast1_tmpstr", "1", host, path, tmpstr);

	tmpstr1 = string_resub("<ul class=\"player-top-links\">", "<div class=\"float-clear\"></div>", tmpstr, 0);
	titheklog(debuglevel, "/var/usr/local/share/titan/plugins/tithek/tvtoast2_tmpstr_tmpstr1_resub", "1", host, path, tmpstr1);
	free(host), host = NULL;
	free(path), path = NULL;
				
	free(tmpstr), tmpstr = NULL;
	printf("tmpstr1: %s\n",tmpstr1);

	int count = 0, i = 0, incount = 0;	
	struct splitstr* ret1 = NULL;
	struct menulist* mlist = NULL, *mbox = NULL;
	ret1 = strsplit(tmpstr1, "\n", &count);
	for(i = 0; i < count; i++)
	{
		if(ret1[i].part != NULL && strlen(ret1[i].part) > 1)
		{
			if(ostrstr(ret1[i].part, "http://tvtoast.com/") != NULL)
			{
				incount++;
				printf("####################### start (%d) ############################\n", incount);

				char* referer = string_resub("<a href=\"", "\"", ret1[i].part, 0);
				referer = string_replace_all(" ", "%20", referer, 1);
				tmpstr = gethttps(referer, NULL, NULL, NULL, NULL, NULL, 1);
				host = string_resub("http://", "/", url, 0);
				path = string_replace_all(host, "", url, 0);
				path = string_replace_all("http://", "", path, 1);
				path = string_replace_all(" ", "%20", path, 1);
				titheklog(debuglevel, "/var/usr/local/share/titan/plugins/tithek/tvtoast3_tmpstr", oitoa(incount), host, path, tmpstr);
				free(url), url = NULL;
				url = string_resub("src=\"", "\"", tmpstr, 0);				
				typemsg = string_resub("http://", "/", url, 0);

				if(ostrstr(url, "www.streamlive.to") != NULL)
					streamurl = streamlive(url, incount);
				else if(ostrstr(url, "zerocast.tv") != NULL)
					streamurl = zerocast(url, incount);
				else if(ostrstr(url, "usachannels.tv") != NULL)
					streamurl = usachannels(url, incount);
				else if(ostrstr(url, "cricfree.sx") != NULL)
					streamurl = cricfree(url, incount);
				else
				{
					printf("found unused (%d) url=%s\n", incount, url);
					tmpstr = gethttps(url, NULL, NULL, NULL, NULL, referer, 1);
					free(host), host = NULL;
					free(path), path = NULL;
					host = string_resub("http://", "/", url, 0);
					path = string_replace_all(host, "", url, 0);
					path = string_replace_all("http://", "", path, 1);
					path = string_replace_all(" ", "%20", path, 1);
					titheklog(debuglevel, "/var/usr/local/share/titan/plugins/tithek/tvtoast4_unused_tmpstr", oitoa(incount), host, path, tmpstr);
				}
				
				debug(99, "streamurl%d streamurl: %s", incount, streamurl);

////////////////////
				if(ostrstr(streamurl, "rtmp") != NULL)
					title = ostrcat("Rtmpe Stream (", NULL, 0, 0);
				else
					title = ostrcat("Http Stream (", NULL, 0, 0);

				title = ostrcat(title, oitoa(incount), 1, 1);
				title = ostrcat(title, ")", 1, 0);
				if(typemsg != NULL)
				{
					title = ostrcat(title, " (", 1, 0);
					title = ostrcat(title, typemsg, 1, 0);
					title = ostrcat(title, ")", 1, 0);
				}
				pic = ostrcat("flv.png", NULL, 0, 0);
				
				if(streamurl != NULL)
				{
					debug(199, "(%d) title: %s streamurl: %s\n", i, title, streamurl);																									
					addmenulist(&mlist, title, streamurl, pic, 0, 0);
				}

				free(title), title = NULL;
				free(pic), pic = NULL;
				free(streamurl), streamurl = NULL;
				free(typemsg), typemsg = NULL;
				free(url), url = NULL;
				free(host), host = NULL;
				free(path), path = NULL;
				printf("####################### stop (%d) ############################\n", incount);
			}
		}
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
	free(tmpstr); tmpstr = NULL;
	free(host), host = NULL;
	free(path), path = NULL;
	free(url), url = NULL;

	titheklog(debuglevel, "/var/usr/local/share/titan/plugins/tithek/tvtoast_tmpstr10_streamurl", NULL, NULL, NULL, tmpstr);

	debug(99, "streamurl: %s", streamurl);	
	return streamurl;
}

int tvtoast_search_local(struct skin* grid, struct skin* listbox, struct skin* countlabel, struct skin* load, char* link, char* title, char* searchstr, int flag)
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

		tmpstr = gethttp("atemio.dyndns.tv", "/mediathek/tvtoast/streams/tvtoast.all-sorted.list", 80, NULL, HTTPAUTH, 5000, NULL, 0);

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
				menu = ostrcat("/tmp/tithek/tvtoast.search.list", NULL, 0, 0);
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