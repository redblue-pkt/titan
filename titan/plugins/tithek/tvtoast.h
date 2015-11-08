#ifndef TVTOAST_H
#define TVTOAST_H

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

char* cricfree(char* link, int incount)
{
	debug(99, "cricfree(%d) link=%s", incount, link);
	int debuglevel = getconfigint("debuglevel", NULL);

	char* url = NULL;
	char* token = NULL;
	char* streamurl = NULL;
	char* tmpstr = NULL;
	char* fid = NULL;
	char* width = NULL;
	char* height = NULL;
	char* pageurl = NULL;
	char* swfurl = NULL;
	char* host = NULL;
	char* path = NULL;

	tmpstr = gethttps(link, NULL, NULL, NULL, NULL, NULL, 1);
	host = string_resub("http://", "/", link, 0);
	path = string_replace_all(host, "", link, 0);
	path = string_replace_all("http://", "", path, 1);
	path = string_replace_all(" ", "%20", path, 1);
	titheklog(debuglevel, "/var/usr/local/share/titan/plugins/tithek/tvtoast4_tmpstr", oitoa(incount), host, path, tmpstr);

//<script type='text/javascript'>id='espnuk'; width='620'; height='490';</script><script type='text/javascript' src='http://theactionlive.com/livegamecr.js'></script>
//document.write('<iframe width="'+width+'" height="'+height+'" scrolling="no" frameborder="0" marginheight="0" marginwidth="0" allowtransparency="true" src="http://theactionlive.com/livegamecr2.php?id='+id+'&width='+width+'&height='+height+'&stretching='+stretching+'"></iframe>');

	fid = string_resub("id='", "'", tmpstr, 0);
	width = string_resub("v_width=", ";", tmpstr, 0);
	height = string_resub("v_height=", ";", tmpstr, 0);
	url = string_resub("src='", "'", tmpstr, 0);

	if(width == NULL)
		width = string_resub("width='", "'", tmpstr, 0);
	if(height == NULL)
		height = string_resub("height='", "'", tmpstr, 0);

	if(width == NULL)
		width = string_resub("var width = '", "'", tmpstr, 0);
	if(height == NULL)
		height = string_resub("var height = '", "'", tmpstr, 0);

	debug(99, "fid=%s width=%s height=%s", fid, width, height);

	tmpstr = gethttps(url, NULL, NULL, NULL, NULL, link, 1);
	free(host), host = NULL;
	free(path), path = NULL;
	host = string_resub("http://", "/", url, 0);
	path = string_replace_all(host, "", url, 0);
	path = string_replace_all("http://", "", path, 1);
	path = string_replace_all(" ", "%20", path, 1);
	titheklog(debuglevel, "/var/usr/local/share/titan/plugins/tithek/tvtoast5_tmpstr", oitoa(incount), host, path, tmpstr);

	pageurl = string_resub("src=\"", "\"", tmpstr, 0);
	pageurl = string_replace_all(" ", "", pageurl, 1);
	pageurl = string_replace_all("+v_", "+", pageurl, 1);
	pageurl = string_replace("'+fid+'", "'+id+'", pageurl, 1);
	pageurl = string_replace("'+id+'", fid, pageurl, 1);
	pageurl = string_replace("'+width+'", width, pageurl, 1);
	pageurl = string_replace("'+height+'", height, pageurl, 1);
	pageurl = string_replace("'+stretching+'", "", pageurl, 1);

	tmpstr = gethttps(pageurl, NULL, NULL, NULL, NULL, link, 1);
	free(host), host = NULL;
	free(path), path = NULL;
	host = string_resub("http://", "/", pageurl, 0);
	path = string_replace_all(host, "", pageurl, 0);
	path = string_replace_all("http://", "", path, 1);
	path = string_replace_all(" ", "%20", path, 1);
	titheklog(debuglevel, "/var/usr/local/share/titan/plugins/tithek/tvtoast6_tmpstr", oitoa(incount), host, path, tmpstr);

	url = string_resub("file: \"", "\"", tmpstr, 0);

	if(url == NULL)
	{
//		biggestplayer.me
//		url: http://biggestplayer.me/playercr.js, file=tithek_global.h, func=gethttps, line=124

		fid = string_resub("id='", "'", tmpstr, 0);
		url = string_resub("src='", "'", tmpstr, 0);

		debug(99, "fid=%s url=%s", fid, url);

		url = string_replace_all(" ", "", url, 1);
		url = string_replace_all("+v_", "+", url, 1);
		url = string_replace_all("'+", "+", url, 1);
		url = string_replace_all("+'", "+", url, 1);
		url = string_replace("+fid+", "+id+", url, 1);
		url = string_replace("+id+", fid, url, 1);
		url = string_replace("+width+", width, url, 1);
		url = string_replace("+height+", height, url, 1);
		url = string_replace("+stretching+", "", url, 1);

		tmpstr = gethttps(url, NULL, NULL, NULL, NULL, link, 1);
		free(host), host = NULL;
		free(path), path = NULL;
		host = string_resub("http://", "/", url, 0);
		path = string_replace_all(host, "", url, 0);
		path = string_replace_all("http://", "", path, 1);
		path = string_replace_all(" ", "%20", path, 1);
		titheklog(debuglevel, "/var/usr/local/share/titan/plugins/tithek/tvtoast7_tmpstr", oitoa(incount), host, path, tmpstr);

//		url: http://biggestplayer.me/streamcrnoscrape.php?id=+id+&width=&height=,

		url = string_resub("src=\"", "\"", tmpstr, 0);		
		url = string_replace_all(" ", "", url, 1);
		url = string_replace_all("+v_", "+", url, 1);
		url = string_replace_all("'+", "+", url, 1);
		url = string_replace_all("+'", "+", url, 1);
		url = string_replace("+fid+", "+id+", url, 1);
		url = string_replace("+id+", fid, url, 1);
		url = string_replace("+width+", width, url, 1);
		url = string_replace("+height+", height, url, 1);
		url = string_replace("+stretching+", "", url, 1);

		tmpstr = gethttps(url, NULL, NULL, NULL, NULL, pageurl, 1);
		free(host), host = NULL;
		free(path), path = NULL;
		host = string_resub("http://", "/", url, 0);
		path = string_replace_all(host, "", url, 0);
		path = string_replace_all("http://", "", path, 1);
		path = string_replace_all(" ", "%20", path, 1);
		titheklog(debuglevel, "/var/usr/local/share/titan/plugins/tithek/tvtoast8_tmpstr", oitoa(incount), host, path, tmpstr);
		streamurl = string_resub("file: \"", "\"", tmpstr, 0);
/*
		http://178.18.31.52:8081/liverepeater/224731/playlist.m3u8?wmsAuthSign=c2VydmVyX3RpbWU9MTEvNi8yMDE1IDM6MDc6MDIgUE0maGFzaF92YWx1ZT1TUUJsM3JoVWNLKzIwVytTdVdRVThRPT0mdmFsaWRtaW51dGVzPTQ=
*/
	}
	else
	{

		token = string_resub("securetoken: \"", "\"", tmpstr, 0);
		debug(99, "token: %s", token);

		streamurl = ostrcat(streamurl, url, 1, 0);
		streamurl = ostrcat(streamurl, " swfUrl=http://p.jwpcdn.com/6/11/jwplayer.flash.swf", 1, 0);
	//	streamurl = ostrcat(streamurl, swfurl, 1, 0);
		streamurl = ostrcat(streamurl, " live=1", 1, 0);
		streamurl = ostrcat(streamurl, " token=", 1, 0);
		streamurl = ostrcat(streamurl, token, 1, 0);
		streamurl = ostrcat(streamurl, " timeout=15", 1, 0);
		streamurl = ostrcat(streamurl, " swfVfy=1", 1, 0);
		streamurl = ostrcat(streamurl, " pageUrl=", 1, 0);
		streamurl = ostrcat(streamurl, pageurl, 1, 0);
/*
		rtmp://31.220.2.148/redirect/espnusa swfUrl=http://p.jwpcdn.com/6/11/jwplayer.flash.swf live=1 token=%Zrey(nKa@#Z timeout=14 swfVfy=1 pageUrl=http://yotv.co/embed.php?live=espnusa&vw=620&vh=490
		rtmp://31.220.2.148/redirect/espn2 swfUrl=http://p.jwpcdn.com/6/11/jwplayer.flash.swf live=1 token=%Zrey(nKa@#Z timeout=15 swfVfy=1 pageUrl=http://www.yotv.co/embed.php?live=espn2&vw=620&vh=490
		rtmp://31.220.2.148/redirect/tsnx2 swfUrl=http://p.jwpcdn.com/6/11/jwplayer.flash.swf live=1 token=%Zrey(nKa@#Z timeout=15 swfVfy=1 pageUrl=http://www.yotv.co/embed.php?live=tsnx2&vw=620&vh=490
*/
	}

	free(host), host = NULL;
	free(path), path = NULL;
	free(url), url = NULL;
	free(token), token = NULL;
	free(tmpstr), tmpstr = NULL;
	free(fid), fid = NULL;
	free(width), width = NULL;
	free(height), height = NULL;
	free(pageurl), pageurl = NULL;
	free(swfurl), swfurl = NULL;
	
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