#ifndef STREAMLIVE_H
#define STREAMLIVE_H

char* streamlive(char* link, int incount)
{
	debug(99, "streamlive(%d) link=%s", incount, link);

	int debuglevel = getconfigint("debuglevel", NULL);
	char* streamurl = NULL, *tmpstr = NULL, *tmpstr1 = NULL;
	char* streamer = NULL;
	char* playpath = NULL;
	char* swfurl = NULL;
	char* app = NULL;
	char* token = NULL;
	char* tmpurl = NULL;
	char* host = NULL;
	char* path = NULL;
	char* id = NULL;
/*
	char* streamurl1 = NULL;
	char* title = NULL;
	char* pic = NULL;
	char* typemsg = NULL;

	typemsg = string_resub("http://", "/", link, 0);
*/
/*
http://www.streamlive.to/embedplayer_new.php?width=640&height=480&channel=71562&autoplay=true
http://www.streamlive.to/view/72736/BT_Sport_2-live-stream-channel
*/
	if(ostrstr(link, "http://www.streamlive.to/view/") != NULL)
	{
		id = string_resub("/view/", "/", link, 0);
		tmpurl = ostrcat("http://www.streamlive.to/embedplayer_new.php?width=640&height=480&channel=", id, 0, 0);
		tmpurl = ostrcat(tmpurl, "&autoplay=true", 1, 0);
	}
	else
		tmpurl = ostrcat(link, NULL, 0, 0);

	tmpstr = gethttps(tmpurl, NULL, NULL, NULL, NULL, NULL, 1);
	host = string_resub("http://", "/", tmpurl, 0);
	path = string_replace_all(host, "", tmpurl, 0);
	path = string_replace_all("http://", "", path, 1);
	path = string_replace_all(" ", "%20", path, 1);	
	titheklog(debuglevel, "/var/usr/local/share/titan/plugins/tithek/tvtoast4_tmpstr", oitoa(incount), host, path, tmpstr);
	free(tmpurl), tmpurl = NULL;

	if(ostrstr(tmpstr, "rtmp") == NULL)
	{
		tmpurl = string_resub("src='", "'", tmpstr, 0);
		free(tmpstr), tmpstr = NULL;						
		
		tmpstr = gethttps(tmpurl, NULL, NULL, NULL, NULL, NULL, 1);
		free(host), host = NULL;
		free(path), path = NULL;
		host = string_resub("http://", "/", tmpurl, 0);
		path = string_replace_all(host, "", tmpurl, 0);
		path = string_replace_all("http://", "", path, 1);
		path = string_replace_all(" ", "%20", path, 1);	
		titheklog(debuglevel, "/var/usr/local/share/titan/plugins/tithek/tvtoast5_tmpstr", oitoa(incount), host, path, tmpstr);
	}
/*
	if(ostrstr(tmpstr, "'file': 'http") != NULL)
	{
		streamurl1 = string_resub("'file': 'http", "'", tmpstr, 0);
		streamurl1 = ostrcat("http", streamurl1, 0, 1);
	}
*/
	streamer = string_resub("streamer: \"", "\"", tmpstr, 0);
	streamer = string_replace_all("\\", "", streamer, 1);
	debug(99, "tvtoast5_tmpstr%d streamer=%s", incount, streamer)
	if(streamer != NULL)
	{
		playpath = string_resub("file: \"", "\"", tmpstr, 0);
		playpath = string_replace_all(".flv", "", playpath, 1);
		debug(99, "tvtoast5_tmpstr%d playpath=%s", incount, playpath)
	
		swfurl = string_resub("type: 'flash', src: '", "'", tmpstr, 0);
		swfurl = string_replace_all("//", "http://", swfurl, 1);
		debug(99, "tvtoast5_tmpstr%d swfurl=%s", incount, swfurl)
	
		app = ostrcat(streamer, NULL, 0, 0);
		tmpstr1 = string_resub("rtmp://", "/", app, 0);
		app = string_replace(tmpstr1, "", app, 0);
		free(tmpstr1), tmpstr1 = NULL;
		app = string_replace_all("rtmp:///", "", app, 1);
		debug(99, "tvtoast5_tmpstr%d app5=%s", incount, app)
	
		tmpurl = string_resub("getJSON(\"", "\"", tmpstr, 0);
		tmpurl = string_replace_all("//", "http://", tmpurl, 1);
		free(tmpstr), tmpstr = NULL;
	
		tmpstr = gethttps(tmpurl, NULL, NULL, NULL, NULL, NULL, 1);
		free(host), host = NULL;
		free(path), path = NULL;
		host = string_resub("http://", "/", tmpurl, 0);
		path = string_replace_all(host, "", tmpurl, 0);
		path = string_replace_all("http://", "", path, 1);
		path = string_replace_all(" ", "%20", path, 1);
		titheklog(debuglevel, "/var/usr/local/share/titan/plugins/tithek/tvtoast6_tmpstr", oitoa(incount), host, path, tmpstr);
	
		token = string_resub("{\"token\":\"", "\"", tmpstr, 0);
		free(tmpstr), tmpstr = NULL;
	
		streamurl = ostrcat(streamer, " app=", 0, 0);
		streamurl = ostrcat(streamurl, app, 1, 0);
		streamurl = ostrcat(streamurl, " playpath=", 1, 0);
		streamurl = ostrcat(streamurl, playpath, 1, 0);
		streamurl = ostrcat(streamurl, " swfUrl=", 1, 0);
		streamurl = ostrcat(streamurl, swfurl, 1, 0);
		streamurl = ostrcat(streamurl, " live=1", 1, 0);
		streamurl = ostrcat(streamurl, " timeout=15", 1, 0);
		streamurl = ostrcat(streamurl, " token=", 1, 0);
		streamurl = ostrcat(streamurl, token, 1, 0);
		streamurl = ostrcat(streamurl, " pageUrl=", 1, 0);
		streamurl = ostrcat(streamurl, tmpurl, 1, 0);
	
	/*
		rtmp://163.172.8.19:1935/edge/_definst_/?xs=_we_Mzcxamx1aXJsamNzamVufDE0NDY5MDkxODF8OTUuOTEuNi4yNXw1NjNjYzM3ZGIxNTU1fGQzOTYwODlmYjQyOGViYjRhZGQ0YjU3NDgwODY1Zjk2Nzk5ZTE4OWU. app=edge/_definst_/?xs=_we_Mzcxamx1aXJsamNzamVufDE0NDY5MDkxODF8OTUuOTEuNi4yNXw1NjNjYzM3ZGIxNTU1fGQzOTYwODlmYjQyOGViYjRhZGQ0YjU3NDgwODY1Zjk2Nzk5ZTE4OWU. playpath=371jluirljcsjen swfUrl=http://www.streamlive.to/ads/embed/player_ilive_embed.swf live=1 timeout=15 token=Qb582083d22804a892ce37092f7f3d2d pageUrl=http://www.streamlive.to/server.php?id=1446822781
	*/
	
	/*
		http://163.172.8.19:1935/edge/_definst_/r3t8xd8ss6pdequ/playlist.m3u8?xs=_we_cjN0OHhkOHNzNnBkZXF1fDE0NDcwNzY3Mjh8OTUuOTEuNi4yNXw1NjNmNTFmODVkZTRmfGNiMDZjMWNhN2UwNTAxYmQzNThkM2I3ZGJjOTVhNmY2YmNlNDg1NGY.
	
		streamurl = ostrcat(streamurl, "\n", 1, 0);
		if(streamurl1 != NULL)
		{
			streamurl = ostrcat(streamurl, streamurl1, 1, 0);
			tmpstr = ostrcat(streamurl, NULL, 0, 0);
		}
	
		if(incount == -1 && streamurl1 != NULL)
		{
		
			incount = 0;
			int count = 0, i = 0;	
			struct splitstr* ret1 = NULL;
			struct menulist* mlist = NULL, *mbox = NULL;
			ret1 = strsplit(tmpstr, "\n", &count);
			for(i = 0; i < count; i++)
			{
				incount++;
	
				free(streamurl), streamurl = NULL;
				streamurl = ostrcat(ret1[i].part, NULL, 0, 0);
	
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
					debug(99, "(%d) title: %s streamurl: %s\n", i, title, streamurl);																									
					addmenulist(&mlist, title, streamurl, pic, 0, 0);
				}
		
				free(title), title = NULL;
				free(pic), pic = NULL;
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
		}
		free(streamurl1), streamurl1 = NULL;
	*/
	}
	else
	{
		free(streamurl), streamurl = NULL;
		if(ostrstr(tmpstr, "'file': 'http") != NULL)
			streamurl = string_resub("'file': 'http", "'", tmpstr, 0);
	}
	free(streamer), streamer = NULL;
	free(app), app = NULL;
	free(playpath), playpath = NULL;
	free(swfurl), swfurl = NULL;
	free(token), token = NULL;
	free(host), host = NULL;
	free(path), path = NULL;
	free(id), id = NULL;

	debug(99, "streamurl %s", streamurl);
	return streamurl;
}

#endif