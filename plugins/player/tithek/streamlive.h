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
	char* url1 = NULL;
	char* url2 = NULL;
	char* url3 = NULL;
	char* host = NULL;
	char* path = NULL;
	char* id = NULL;

	char* streamurl1 = NULL;
//	char* title = NULL;
//	char* pic = NULL;
	char* typemsg = NULL;

	typemsg = string_resub("http://", "/", link, 0);

/*
http://www.streamlive.to/embedplayer_new.php?width=640&height=480&channel=71562&autoplay=true
http://www.streamlive.to/view/72736/BT_Sport_2-live-stream-channel
*/
printf("111111111\n");

	if(ostrstr(link, "http://www.streamlive.to/view/") != NULL)
	{
printf("222222222\n");

		id = string_resub("/view/", "/", link, 0);
		url1 = ostrcat("http://www.streamlive.to/embedplayer_new.php?width=640&height=480&channel=", id, 0, 0);
		url1 = ostrcat(url1, "&autoplay=true", 1, 0);
	}
	else
	{
printf("333333333\n");

		url1 = ostrcat(link, NULL, 0, 0);
	}
printf("444444444\n");

	printf("############################################################\n");
	printf("url1234: %s\n", url1);
	printf("############################################################\n");



//<script type="text/javascript" src="http://www.streamlive.to/embed/69654&width=650&height=480&autoplay=true"></script>
	tmpstr = gethttps(url1, NULL, NULL, NULL, NULL, NULL, 1);
	host = string_resub("http://", "/", url1, 0);
	path = string_replace_all(host, "", url1, 0);

//	tmpstr = gethttps(link, NULL, NULL, NULL, NULL, NULL, 1);
//	host = string_resub("http://", "/", link, 0);
//	path = string_replace_all(host, "", link, 0);

	path = string_replace_all("http://", "", path, 1);
	path = string_replace_all(" ", "%20", path, 1);	
	titheklog(debuglevel, "/var/usr/local/share/titan/plugins/player/tithek/tvtoast4_tmpstr", oitoa(incount), host, path, tmpstr);

	if(ostrstr(tmpstr, "<h1>To prove that you are not bot, please answer the question to continue</h1>") != NULL)
	{
//		Question: Type this word in the box: walking dead<br /><br />
//		Question: Type the following word in the box: lovelivetv<br /><br />
//		Question: Type this Honda car SUV in the box: crvhonda<br
		printf("tmpstr: %s\n", tmpstr);

		char* data = oregex(".*Question:.*: (.*)<br /><br />.*", tmpstr);

		data = ostrcat("captcha=", data, 0, 1);
		printf("data: %s\n", data);

		free(tmpstr), tmpstr = NULL;
		tmpstr = gethttps(link, NULL, data, NULL, NULL, link, 1);
		free(data), data = NULL;

		host = string_resub("http://", "/", link, 0);
		path = string_replace_all(host, "", link, 0);
		path = string_replace_all("http://", "", path, 1);
		path = string_replace_all(" ", "%20", path, 1);	
		titheklog(debuglevel, "/var/usr/local/share/titan/plugins/player/tithek/tvtoast4a_tmpstr", oitoa(incount), host, path, tmpstr);
	}

	if(ostrstr(tmpstr, "'file': 'http") != NULL)
	{
		streamurl1 = string_resub("'file': 'http", "'", tmpstr, 0);
		streamurl1 = ostrcat("http", streamurl1, 0, 1);
		printf("1streamurl1: %s\n", streamurl1);
	}

	if(ostrstr(tmpstr, "rtmp") == NULL)
	{
//    document.write("<iframe src='http://www.streamlive.to/embedplayer_new.php?width=650&height=480&channel=69654&autoplay=true' frameborder='0' marginheight='0' marginwidth='0' scrolling='no' width='650' height='480'></iframe>");
		url2 = string_resub("src='", "'", tmpstr, 0);
		free(tmpstr), tmpstr = NULL;						
		tmpstr = gethttps(url2, NULL, NULL, NULL, NULL, url1, 1);
		free(host), host = NULL;
		free(path), path = NULL;
		host = string_resub("http://", "/", url2, 0);
		path = string_replace_all(host, "", url2, 0);
		path = string_replace_all("http://", "", path, 1);
		path = string_replace_all(" ", "%20", path, 1);	
		titheklog(debuglevel, "/var/usr/local/share/titan/plugins/player/tithek/tvtoast5_tmpstr", oitoa(incount), host, path, tmpstr);
	}

	if(ostrstr(tmpstr, "'file': 'http") != NULL)
	{
		streamurl1 = string_resub("'file': 'http", "'", tmpstr, 0);
		streamurl1 = ostrcat("http", streamurl1, 0, 1);
		printf("2streamurl1: %s\n", streamurl1);
	}

//streamer: "rtmp:\/\/163.172.8.19:1935\/edge\/_definst_\/?xs=_we_eXVtN2d4emZubWNnYXd0fDE0NDc2ODg2ODR8OTUuOTEuNi4yNXw1NjQ4YTg2Yzg5ZWEwfDVhOTkxZWIzY2JiNzA0NjYyM2Y4ODVlODY0NDhhZTJmNDM0OTc4ODI.",
	streamer = string_resub("streamer: \"", "\"", tmpstr, 0);
	streamer = string_replace_all("\\", "", streamer, 1);
	debug(99, "tvtoast5_tmpstr%d streamer=%s", incount, streamer)
	if(streamer != NULL)
	{
		playpath = string_resub("file: \"", "\"", tmpstr, 0);
		playpath = string_replace_all(".flv", "", playpath, 1);
		debug(99, "tvtoast5_tmpstr%d playpath=%s", incount, playpath)
	
		swfurl = string_resub("type: 'flash', src: '", "'", tmpstr, 0);
//		swfurl = string_replace_all("//", "http://", swfurl, 1);
		debug(99, "tvtoast5_tmpstr%d swfurl=%s", incount, swfurl)
	
		app = ostrcat(streamer, NULL, 0, 0);
		tmpstr1 = string_resub("rtmp://", "/", app, 0);
		app = string_replace(tmpstr1, "", app, 0);
		free(tmpstr1), tmpstr1 = NULL;
		app = string_replace_all("rtmp:///", "", app, 1);
		debug(99, "tvtoast5_tmpstr%d app5=%s", incount, app)

//$.getJSON("//www.streamlive.to/server.php?id=1447602284", function(json){
		url3 = string_resub("getJSON(\"", "\"", tmpstr, 0);
//		url3 = string_replace_all("//", "http://", url3, 1);
		free(tmpstr), tmpstr = NULL;
	
		tmpstr = gethttps(url3, NULL, NULL, NULL, NULL, url2, 1);
		free(host), host = NULL;
		free(path), path = NULL;
		host = string_resub("http://", "/", url3, 0);
		path = string_replace_all(host, "", url3, 0);
		path = string_replace_all("http://", "", path, 1);
		path = string_replace_all(" ", "%20", path, 1);
		titheklog(debuglevel, "/var/usr/local/share/titan/plugins/player/tithek/tvtoast6_tmpstr", oitoa(incount), host, path, tmpstr);
	
		token = string_resub("{\"token\":\"", "\"", tmpstr, 0);
		free(tmpstr), tmpstr = NULL;

/*
	rtmp://163.172.8.19:1935/edge/_definst_/?xs=_we_Mzcxamx1aXJsamNzamVufDE0NDc2ODk3ODJ8OTUuOTEuNi4yNXw1NjQ4YWNiNjQ4MDNhfGE5ZjdhNzgyZmUwZmVhZWFhYjBhYTEyMjc4MTE5NGY3NWMxMjE4YmU. app=edge/_definst_/?xs=_we_Mzcxamx1aXJsamNzamVufDE0NDc2ODk3ODJ8OTUuOTEuNi4yNXw1NjQ4YWNiNjQ4MDNhfGE5ZjdhNzgyZmUwZmVhZWFhYjBhYTEyMjc4MTE5NGY3NWMxMjE4YmU. playpath=371jluirljcsjen swfUrl=http://www.streamlive.to/ads/embed/player_ilive_embed.swf live=1 timeout=15 token=D57b0914d73be4057f69d6443745ed28 pageUrl=http://www.streamlive.to/embedplayer_new.php?width=640&height=480&channel=68969&autoplay=true
*/
		if(streamer != NULL)
		{
// without app not working
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
			streamurl = ostrcat(streamurl, url1, 1, 0);
		}
	}

	free(streamer), streamer = NULL;
	free(app), app = NULL;
	free(playpath), playpath = NULL;
	free(swfurl), swfurl = NULL;
	free(token), token = NULL;
	free(url1), url1 = NULL;
	free(url2), url2 = NULL;
	free(url3), url3 = NULL;

	free(host), host = NULL;
	free(path), path = NULL;
	free(id), id = NULL;

	debug(99, "streamurl %s", streamurl);
	return streamurl;
}

#endif