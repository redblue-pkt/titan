#ifndef TVTOAST_H
#define TVTOAST_H

char* zerocast(char* link, int incount)
{
	int debuglevel = getconfigint("debuglevel", NULL);

	debug(99, "zerocast(%d) link=%s ", incount, link);
	char* streamurl = NULL;
	char* tmpstr = NULL;
	char* path = NULL;
	char* host = NULL;
	char* url1 = NULL;
	char* url2 = NULL;
	char* streamer = NULL;

//<script type="text/javascript" src="http://zerocast.tv/channel.php?a=305&width=650&height=480&autostart=true"></script>
	host = string_resub("http://", "/", link, 0);
	tmpstr = gethttps(link, NULL, NULL, NULL, NULL, link, 1);
	host = string_resub("http://", "/", link, 0);
	path = string_replace_all(host, "", link, 0);
	path = string_replace_all("http://", "", path, 1);
	path = string_replace_all(" ", "%20", path, 1);
	titheklog(debuglevel, "/var/usr/local/share/titan/plugins/tithek/tvtoast4_tmpstr", oitoa(incount), host, path, tmpstr);

//var url = 'http://zerocast.tv/embed.php?a=305&id=&width=650&height=480&autostart=true&strech=exactfit';
	url1 = string_resub("var url = '", "'", tmpstr, 0);
	host = string_resub("http://", "/", url1, 0);
	tmpstr = gethttps(url1, NULL, NULL, NULL, NULL, NULL, 1);
	host = string_resub("http://", "/", url1, 0);
	path = string_replace_all(host, "", url1, 0);
	path = string_replace_all("http://", "", path, 1);
	path = string_replace_all(" ", "%20", path, 1);
	titheklog(debuglevel, "/var/usr/local/share/titan/plugins/tithek/tvtoast5_tmpstr", oitoa(incount), host, path, tmpstr);

//curl = "cnRtcDovLzM3LjQ4LjgzLjY6MTkzNS9nb0xpdmUvP3dtc0F1dGhTaWduPWMyVnlkbVZ5WDNScGJXVTlNVEV2TVRVdk1qQXhOU0F6T2pFMU9qQTNJRkJOSm1oaGMyaGZkbUZzZFdVOVdUa3ZiazVtZFVaMWVUWk9Ra3RUVTJ4ME5rbG9VVDA5Sm5aaGJHbGtiV2x1ZFhSbGN6MDEvOXB4R0N0UDNhRW5KWWdvQkZGSDg=";
	streamer = string_resub("curl = \"", "\"", tmpstr, 0);
	if(streamer == NULL) goto end;
	printf("streamer input: %s\n", streamer);
	b64dec(streamer, streamer);
	printf("streamer decod: %s\n", streamer);

//<iframe src="online.php?c=fox" frameborder="0" width="10" height="10" scrolling="no"></iframe>
	url2 = string_resub("<iframe src=\"", "\"", tmpstr, 0);
	url2 = ostrcat("http://zerocast.tv/", url2, 0, 1);

	host = string_resub("http://", "/", url2, 0);
	tmpstr = gethttps(url2, NULL, NULL, NULL, NULL, url1, 1);
	host = string_resub("http://", "/", url2, 0);
	path = string_replace_all(host, "", url2, 0);
	path = string_replace_all("http://", "", path, 1);
	path = string_replace_all(" ", "%20", path, 1);
	titheklog(debuglevel, "/var/usr/local/share/titan/plugins/tithek/tvtoast6_tmpstr", oitoa(incount), host, path, tmpstr);

/*
	rtmp://37.48.83.6:1935/goLive/?wmsAuthSign=c2VydmVyX3RpbWU9MTEvMTUvMjAxNSAyOjI0OjE5IFBNJmhhc2hfdmFsdWU9aFMremI4MmZCc0FtbFFkUWFYNXlpdz09JnZhbGlkbWludXRlcz01/9pxGCtP3aEnJYgoBFFH8 swfUrl=http://p.jwpcdn.com/6/12/jwplayer.flash.swf flashver=WIN\2019,0,0,226 timeout=15 live=true swfVfy=1 pageUrl=http://zerocast.tv/embed.php?a=305&id=&width=640&height=480&autostart=true&strech=exactfit
*/
	if(streamer != NULL)
	{
		streamurl = ostrcat(streamer, NULL, 0, 0);
		streamurl = ostrcat(streamurl, " swfUrl=http://p.jwpcdn.com/6/12/jwplayer.flash.swf", 1, 0);
//(titan:10759): GStreamer-WARNING **: Trying to set string on structure field 'uri', but string is not valid UTF-8. Please file a bug.
//		streamurl = ostrcat(streamurl, " flashver=WIN\2019,0,0,226", 1, 0);
		streamurl = ostrcat(streamurl, " timeout=15", 1, 0);
		streamurl = ostrcat(streamurl, " live=true", 1, 0);
		streamurl = ostrcat(streamurl, " swfVfy=1", 1, 0);
		streamurl = ostrcat(streamurl, " pageUrl=", 1, 0);
		streamurl = ostrcat(streamurl, url1, 1, 0);
	}
end:

	free(host), host = NULL;
	free(path), path = NULL;
	free(tmpstr), tmpstr = NULL;
	free(url1), url1 = NULL;
	free(url2), url2 = NULL;
	free(streamer), streamer = NULL;

	debug(99, "streamurl %s", streamurl);
	return streamurl;
}

char* usachannels(char* link, int incount)
{
	int debuglevel = getconfigint("debuglevel", NULL);

	debug(99, "usachannels(%d) link=%s", incount, link);
	char* streamurl = NULL;
	char* host = NULL;
	char* path = NULL;
	char* tmpstr = NULL;
	char* url1 = NULL;
	char* url2 = NULL;
	char* url3 = NULL;
	char* streamer = NULL;

//<iframe width="650" height="480" frameborder="0" src="http://usachannels.tv/fox_live.php?vw=650&vh=480" scrolling="no" allowtransparency="yes"></iframe>
	host = string_resub("http://", "/", link, 0);
	tmpstr = gethttps(link, NULL, NULL, NULL, NULL, NULL, 1);
	host = string_resub("http://", "/", link, 0);
	path = string_replace_all(host, "", link, 0);
	path = string_replace_all("http://", "", path, 1);
	path = string_replace_all(" ", "%20", path, 1);
	titheklog(debuglevel, "/var/usr/local/share/titan/plugins/tithek/tvtoast4_tmpstr", oitoa(incount), host, path, tmpstr);

//<script type="text/javascript" src="http://miplayer.net/embed.php?id=hboouu&width=630&height=470&autoplay=true"></script>
	url1 = string_resub("src=\"", "\"", tmpstr, 0);
	free(tmpstr), tmpstr = NULL;

	host = string_resub("http://", "/", url1, 0);
	tmpstr = gethttps(url1, NULL, NULL, NULL, NULL, link, 1);
	host = string_resub("http://", "/", url1, 0);
	path = string_replace_all(host, "", url1, 0);
	path = string_replace_all("http://", "", path, 1);
	path = string_replace_all(" ", "%20", path, 1);
	titheklog(debuglevel, "/var/usr/local/share/titan/plugins/tithek/tvtoast5_tmpstr", oitoa(incount), host, path, tmpstr);

//document.write("<iframe src='http://miplayer.net/embedplayer.php?width=630&height=470&id=hboouu&autoplay=true&strech=exactfit' frameborder='0' marginheight='0' marginwidth='0' scrolling='no' width='630' height='470' allowfullscreen></iframe>");
	url2 = string_resub("iframe src='", "'", tmpstr, 0);
	free(tmpstr), tmpstr = NULL;

	host = string_resub("http://", "/", url2, 0);
	tmpstr = gethttps(url2, NULL, NULL, NULL, NULL, url1, 1);
	host = string_resub("http://", "/", url2, 0);
	path = string_replace_all(host, "", url2, 0);
	path = string_replace_all("http://", "", path, 1);
	path = string_replace_all(" ", "%20", path, 1);
	titheklog(debuglevel, "/var/usr/local/share/titan/plugins/tithek/tvtoast6_tmpstr", oitoa(incount), host, path, tmpstr);

//curl = "cnRtcDovLzE4NS42My4yNTUuMTA6MTkzNS9saXZlZWRnZS8/d21zQXV0aFNpZ249YzJWeWRtVnlYM1JwYldVOU1URXZNVFF2TWpBeE5TQXhNVG94TlRveU5pQkJUU1pvWVhOb1gzWmhiSFZsUFhvdlJXd3dNSE5LYWtOS05VdGpZblZLZW5JeFRFRTlQU1oyWVd4cFpHMXBiblYwWlhNOU1UQT0vaGJvb3V1XzBkOXJxa2Mz";
	streamer = string_resub("curl = \"", "\"", tmpstr, 0);
	if(streamer == NULL) goto end;

	printf("streamer input: %s\n", streamer);
	b64dec(streamer, streamer);
	printf("streamer decod: %s\n", streamer);

//top.location = "http://miplayer.net/channel.php?id=fox998";
	url3 = string_resub("top.location = \"", "\"", tmpstr, 0);
	free(tmpstr), tmpstr = NULL;

	host = string_resub("http://", "/", url3, 0);
	tmpstr = gethttps(url3, NULL, NULL, NULL, NULL, url2, 1);
	host = string_resub("http://", "/", url3, 0);
	path = string_replace_all(host, "", url3, 0);
	path = string_replace_all("http://", "", path, 1);
	path = string_replace_all(" ", "%20", path, 1);
	titheklog(debuglevel, "/var/usr/local/share/titan/plugins/tithek/tvtoast7_tmpstr", oitoa(incount), host, path, tmpstr);
/*
	if(tmpstr == NULL || ostrstr(tmpstr, "This channel is domain protected") != NULL)
	{
		textbox(_("Message"), _("This channel is domain protected") , _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 1200, 200, 0, 0);
		goto end;
	}
*/

/*
rtmp://185.63.255.10:1935/liveedge/?wmsAuthSign=c2VydmVyX3RpbWU9MTEvMTUvMjAxNSAzOjA2OjExIFBNJmhhc2hfdmFsdWU9eUxSVjk2VytYMmh2RTFrazF1QVU3dz09JnZhbGlkbWludXRlcz0xMA==/fox998_kjz4ti73 swfUrl=http://otronivel.me/jw7/jwplayer.flash.swf flashver=WIN\2019,0,0,226 timeout=15 live=true swfVfy=1 pageUrl=http://miplayer.net/embedplayer.php?width=630&height=470&id=fox998&autoplay=true&strech=exactfit
*/
	if(streamer != NULL)
	{
		streamurl = ostrcat(streamer, NULL, 0, 0);
		streamurl = ostrcat(streamurl, " swfUrl=http://cdn.ibrod.tv/player/jwplayer.flash.swf", 1, 0);
//(titan:10759): GStreamer-WARNING **: Trying to set string on structure field 'uri', but string is not valid UTF-8. Please file a bug.
//		streamurl = ostrcat(streamurl, " flashver=WIN\2019,0,0,226", 1, 0);
		streamurl = ostrcat(streamurl, " live=1", 1, 0);
		streamurl = ostrcat(streamurl, " timeout=15", 1, 0);
		streamurl = ostrcat(streamurl, " swfVfy=1", 1, 0);
		streamurl = ostrcat(streamurl, " pageUrl=", 1, 0);
		streamurl = ostrcat(streamurl, url2, 1, 0);
	}
end:

	free(host), host = NULL;
	free(path), path = NULL;
	free(tmpstr), tmpstr = NULL;
	free(url1), url1 = NULL;
	free(url2), url2 = NULL;
	free(url3), url3 = NULL;
	free(streamer), streamer = NULL;

	debug(99, "streamurl %s", streamurl);
	return streamurl;
}

char* liveonlinetv(char* link, int incount)
{
	int debuglevel = getconfigint("debuglevel", NULL);

	debug(99, "liveonlinetv(%d) link=%s", incount, link);
	char* streamurl = NULL;
	char* host = NULL;
	char* path = NULL;
	char* tmpstr = NULL;

	host = string_resub("http://", "/", link, 0);
	tmpstr = gethttps(link, NULL, NULL, NULL, NULL, NULL, 1);
	host = string_resub("http://", "/", link, 0);
	path = string_replace_all(host, "", link, 0);
	path = string_replace_all("http://", "", path, 1);
	path = string_replace_all(" ", "%20", path, 1);
	titheklog(debuglevel, "/var/usr/local/share/titan/plugins/tithek/tvtoast4_tmpstr", oitoa(incount), host, path, tmpstr);

	streamurl = string_resub("hls: \"", "\"", tmpstr, 0);
	if(streamurl == NULL)
		streamurl = string_resub("progressive: \"", "\"", tmpstr, 0);

	free(tmpstr), tmpstr = NULL;
	free(host), host = NULL;
	free(path), path = NULL;

	debug(99, "streamurl %s", streamurl);
	return streamurl;
}

char* akamaistream(char* link, int incount)
{
	int debuglevel = getconfigint("debuglevel", NULL);

	debug(99, "akamaistream(%d) link=%s", incount, link);
	char* streamurl = NULL;
	char* host = NULL;
	char* path = NULL;
	char* streamurl1 = NULL;
	char* streamurl2 = NULL;
	char* tmpstr = NULL;
	char* tmpstr2 = NULL;
	char* title = NULL;
	char* pic = NULL;
	char* typemsg = NULL;

	typemsg = string_resub("http://", "/", link, 0);

	host = string_resub("http://", "/", link, 0);
	tmpstr = gethttps(link, NULL, NULL, NULL, NULL, NULL, 1);
	host = string_resub("http://", "/", link, 0);
	path = string_replace_all(host, "", link, 0);
	path = string_replace_all("http://", "", path, 1);
	path = string_replace_all(" ", "%20", path, 1);
	titheklog(debuglevel, "/var/usr/local/share/titan/plugins/tithek/tvtoast4_tmpstr", oitoa(incount), host, path, tmpstr);

	streamurl1 = string_resub("Ref1=", "\n", tmpstr, 0);
	streamurl2 = string_resub("Ref2=", "\n", tmpstr, 0);

	streamurl1 = string_replace_all("&amp;", "&", streamurl1, 0);
	streamurl2 = string_replace_all("&amp;", "&", streamurl2, 0);

printf("streamurl1: %s\n", streamurl1);
printf("streamurl2: %s\n", streamurl2);

	streamurl = ostrcat(streamurl1, NULL, 0, 0);
	if(streamurl == NULL)
		streamurl = ostrcat(streamurl2, NULL, 0, 0);

	if(incount == -1 && streamurl1 != NULL)
	{
		tmpstr2 = ostrcat(streamurl1, "\n", 0, 0);
		tmpstr2 = ostrcat(tmpstr2, streamurl2, 1, 0);

		int count = 0, i = 0;
		struct splitstr* ret1 = NULL;
		struct menulist* mlist = NULL, *mbox = NULL;
		ret1 = strsplit(tmpstr2, "\n", &count);
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
	free(streamurl2), streamurl2 = NULL;
	free(tmpstr), tmpstr = NULL;
	free(tmpstr2), tmpstr2 = NULL;
	free(title), title = NULL;
	free(pic), pic = NULL;
	free(typemsg), typemsg = NULL;

	free(host), host = NULL;
	free(path), path = NULL;

	debug(99, "streamurl %s", streamurl);
	return streamurl;
}

char* tvnanet(char* link, int incount)
{
	int debuglevel = getconfigint("debuglevel", NULL);

	debug(99, "tvnanet(%d) link=%s", incount, link);
	char* streamurl = NULL;
	char* host = NULL;
	char* path = NULL;
	char* tmpstr = NULL;
	char* url = NULL;
	char* streamer = NULL;
	char* pageurl = NULL;

	tmpstr = gethttps(link, NULL, NULL, NULL, NULL, NULL, 1);
	host = string_resub("http://", "/", link, 0);
	path = string_replace_all(host, "", link, 0);
	path = string_replace_all("http://", "", path, 1);
	path = string_replace_all(" ", "%20", path, 1);
	titheklog(debuglevel, "/var/usr/local/share/titan/plugins/tithek/tvtoast4_tmpstr", oitoa(incount), host, path, tmpstr);

	url = string_resub("SRC=\"", "\"", tmpstr, 0);
	free(tmpstr), tmpstr = NULL;
	free(host), host = NULL;
	free(path), path = NULL;
	tmpstr = gethttps(url, NULL, NULL, NULL, NULL, NULL, 1);
	host = string_resub("http://", "/", link, 0);
	path = string_replace_all(host, "", link, 0);
	path = string_replace_all("http://", "", path, 1);
	path = string_replace_all(" ", "%20", path, 1);
	titheklog(debuglevel, "/var/usr/local/share/titan/plugins/tithek/tvtoast5_tmpstr", oitoa(incount), host, path, tmpstr);

	streamer = string_resub("streamer=", "\"", tmpstr, 0);
	pageurl = ostrcat(link, NULL, 0, 0);

	streamurl = ostrcat(streamurl, streamer, 1, 0);
	streamurl = ostrcat(streamurl, " swfUrl=http://livecast.cc/live/player.swf", 1, 0);
	streamurl = ostrcat(streamurl, " live=1", 1, 0);
	streamurl = ostrcat(streamurl, " timeout=15", 1, 0);
	streamurl = ostrcat(streamurl, " swfVfy=1", 1, 0);
	streamurl = ostrcat(streamurl, " pageUrl=", 1, 0);
	streamurl = ostrcat(streamurl, pageurl, 1, 0);

//gst-launch-1.0 playbin uri=rtmpe://r.livecast.cc/rl/&autostart=true&bufferlength=1&displayclick=fullscreen&stretching=exactfit&&screencolor=000000  swfUrl=http://livecast.cc/ads/popunder.js

	free(tmpstr), tmpstr = NULL;
	free(host), host = NULL;
	free(path), path = NULL;
	free(url), url = NULL;
	free(streamer), streamer = NULL;
	free(pageurl), pageurl = NULL;

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
				else if(ostrstr(url, "cricfree.sx") != NULL)
					streamurl = cricfree(url, incount);
				else if(ostrstr(url, "liveonlinetv") != NULL)
					streamurl = liveonlinetv(url, incount);
				else if(ostrstr(url, "akamaistream") != NULL)
					streamurl = akamaistream(url, incount);
				else if(ostrstr(url, "tvnanet") != NULL)
					streamurl = tvnanet(url, incount);
				else if(ostrstr(url, "usachannels.tv") != NULL)
					streamurl = usachannels(url, incount);
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

		tmpstr = gethttp("openaaf.dyndns.tv", "/mediathek/tvtoast/streams/tvtoast.all-sorted.list", 80, NULL, HTTPAUTH, 5000, NULL, 0);

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