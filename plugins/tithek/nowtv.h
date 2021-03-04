#ifndef RTL2NOW_H
#define RTL2NOW_H

// flag 1 = get getstreamurl http
// flag 2 = getstreamurl

char* nowtv(char* link)
{
	char* ip = NULL, *pos = NULL, *path = NULL, *streamurl = NULL;
	int debuglevel = getconfigint("debuglevel", NULL);

	ip = string_replace("https://", "", (char*)link, 0);

	if(ip != NULL)
		pos = strchr(ip, '/');
	if(pos != NULL)
	{
		pos[0] = '\0';
		path = pos + 1;
	}

	char* tmpstr = NULL, *tmpstr1 = NULL, *tmpstr2 = NULL, *tmpstr3 = NULL, *title = NULL, *pic = NULL, *app = NULL;

	tmpstr = gethttps(link, NULL, NULL, NULL, NULL, NULL, 1);
	titheklog(debuglevel, "/tmp/tithek/nowtv_get1", NULL, NULL, NULL, tmpstr);	
	
//	tmpstr = gethttp(ip, path, 80, NULL, NULL, 10000, NULL, 0);
	tmpstr1 = string_resub("{\"items\":[", "],\"total\":", tmpstr, 0);
	titheklog(debuglevel, "/tmp/tithek/nowtv_get1_replace1", NULL, NULL, NULL, tmpstr1);	

	tmpstr1 = string_replace_all("},{", "},\n{", tmpstr1, 1);
	titheklog(debuglevel, "/tmp/tithek/nowtv_get1_replace2", NULL, NULL, NULL, tmpstr1);	

 	struct menulist* mlist = NULL, *mbox = NULL;
 		
	int count1 = 0, i = 0;	
	struct splitstr* ret1 = NULL;
	ret1 = strsplit(tmpstr1, "\n", &count1);
	if(ret1 != NULL)
	{
		int max = count1;
		for(i = 0; i < max; i++)
		{
			tmpstr2 = string_resub("\"path\":\"", "\",\"", ret1[i].part, 0);

			tmpstr2 = string_replace_all("\\", "", tmpstr2, 1);
/*
			streamurl = ostrcat("http://hls.fra.rtlnow.de/hls-vod-enc", tmpstr2, 0, 0); 
			streamurl = ostrcat(streamurl, ".m3u8", 1, 0); 
			streamurl = string_replace_all("/rtlnow/", "/rtlnow/videos/", streamurl, 1);
			streamurl = string_replace_all("/rtl2now/", "/rtl2now/videos/", streamurl, 1);
			streamurl = string_replace_all("/superrtlnow/", "/superrtlnow/videos/", streamurl, 1);
			streamurl = string_replace_all("/voxnow/", "/voxnow/videos/", streamurl, 1);
			streamurl = string_replace_all("/ntvnow/", "/ntvnow/videos/", streamurl, 1);
			streamurl = string_replace_all("/nitronow/", "/nitronow/videos/", streamurl, 1);
*/
			tmpstr3 = oregex(".*/([0-9]{1,6}/.*).*", tmpstr2);

			if(ostrstr(tmpstr2, "rtlnow") != NULL)
				app = ostrcat("rtlnow", NULL, 0, 0);
			else if(ostrstr(tmpstr2, "rtl2now") != NULL)
				app = ostrcat("rtl2now", NULL, 0, 0);
			else if(ostrstr(tmpstr2, "superrtlnow") != NULL)
				app = ostrcat("superrtlnow", NULL, 0, 0);
			else if(ostrstr(tmpstr2, "voxnow") != NULL)
				app = ostrcat("voxnow", NULL, 0, 0);
			else if(ostrstr(tmpstr2, "ntvnow") != NULL)
				app = ostrcat("ntvnow", NULL, 0, 0);
			else if(ostrstr(tmpstr2, "nitronow") != NULL)
				app = ostrcat("nitronow", NULL, 0, 0);
		
			streamurl = ostrcat("rtmpe://fms.rtl.de swfVfy=1 playpath=mp4:", tmpstr3, 0, 0);
			streamurl = ostrcat(streamurl, " app=", 1, 0);
			streamurl = ostrcat(streamurl, app, 1, 0);
			streamurl = ostrcat(streamurl, " swfUrl=http://cdn.static-fra.de/now/vodplayer.swf pageUrl=http://", 1, 0);
			streamurl = ostrcat(streamurl, app, 1, 0);
			streamurl = ostrcat(streamurl, ".rtl.de timeout=120", 1, 0);

			tmpstr3 = string_resub("\"bitrate\":", ",\"", ret1[i].part, 0);

			title = ostrcat("Bitrate: ", tmpstr3, 0, 0); 

			if(ostrstr(ret1[i].part, "mp4") != NULL)
				pic = ostrcat("mp4.png", NULL, 0, 0);
			else if(ostrstr(ret1[i].part, "3gp") != NULL)
				pic = ostrcat("3gp.png", NULL, 0, 0);
			else if(ostrstr(ret1[i].part, "flv") != NULL)
				pic = ostrcat("flv.png", NULL, 0, 0);
					
			if(app != NULL)
			{
				debug(99, "(%d) title: %s streamurl: %s\n", i, title, streamurl);
				addmenulist(&mlist, title, streamurl, pic, 0, 0);
			}
			free(title), title = NULL;
			free(pic), pic = NULL;
			free(streamurl), streamurl = NULL;
			free(app), app = NULL;
			free(tmpstr2), tmpstr2 = NULL;
			free(tmpstr3), tmpstr3 = NULL;
		}
		free(ret1), ret1 = NULL;				
		free(tmpstr1), tmpstr1 = NULL;				
		free(tmpstr), tmpstr = NULL;				

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
		
	}				
	return streamurl;
}

int nowtv_search_local(struct skin* grid, struct skin* listbox, struct skin* countlabel, struct skin* load, char* link, char* title, char* searchstr, int flag)
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

		tmpstr = gethttp("openaaf.dyndns.tv", "/mediathek/nowtv.de/streams/nowtv.de.all-sorted.list", 80, NULL, HTTPAUTH, 5000, NULL, 0);

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
				menu = ostrcat("/tmp/tithek/nowtv.de.search.list", NULL, 0, 0);
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
