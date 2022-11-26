#ifndef CRICFREE_H
#define CRICFREE_H

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
/*
http://cricfree.sx/sky-sports-1-live-stream-5
http://cricfree.sx/update/tsn1.php,
*/

	if(ostrstr(link, "/update/") == NULL)
	{
		tmpstr = gethttps(link, NULL, NULL, NULL, NULL, NULL, 1);
		host = string_resub("http://", "/", link, 0);
		path = string_replace_all(host, "", link, 0);
		path = string_replace_all("http://", "", path, 1);
		path = string_replace_all(" ", "%20", path, 1);
		titheklog(debuglevel, "/var/usr/local/share/titan/plugins/player/tithek/tvtoast4_tmpstr", oitoa(incount), host, path, tmpstr);
//		url = string_resub("src=\"", "\"", tmpstr, 0);
		url = oregex(".*src=\"(http://cricfree.*)\" id=\"iframe\".*", tmpstr);	
//<iframe frameborder="0" marginheight="0" allowfullscreen="true" marginwidth="0" height="555" src="http://cricfree.sc/update/bbc2.php" id="iframe" name="iframe_a" scrolling="no" width="620">Your Browser Do not Support Iframe</iframe>

	}
	else
	{
		url = ostrcat(link, NULL, 0, 0);
	}

	tmpstr = gethttps(url, NULL, NULL, NULL, NULL, NULL, 1);
	host = string_resub("http://", "/", url, 0);
	path = string_replace_all(host, "", url, 0);
	path = string_replace_all("http://", "", path, 1);
	path = string_replace_all(" ", "%20", path, 1);
	titheklog(debuglevel, "/var/usr/local/share/titan/plugins/player/tithek/tvtoast5_tmpstr", oitoa(incount), host, path, tmpstr);
	free(url), url = NULL;

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
	titheklog(debuglevel, "/var/usr/local/share/titan/plugins/player/tithek/tvtoast6_tmpstr", oitoa(incount), host, path, tmpstr);

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
	titheklog(debuglevel, "/var/usr/local/share/titan/plugins/player/tithek/tvtoast7_tmpstr", oitoa(incount), host, path, tmpstr);

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
		titheklog(debuglevel, "/var/usr/local/share/titan/plugins/player/tithek/tvtoast8_tmpstr", oitoa(incount), host, path, tmpstr);
		tmpstr = jsunpack(tmpstr);
		titheklog(debuglevel, "/var/usr/local/share/titan/plugins/player/tithek/tvtoast8a_tmpstr_jsunpack", oitoa(incount), host, path, tmpstr);

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
// work start
		free(url), url = NULL;
		url = ostrcat("http://chupala.biggestplayer.me/streamcr.php?id=", fid, 0, 0);
		url = ostrcat(url, "&width=620&height=460", 1, 0);
// work end

//		http://chupala.biggestplayer.me/streamcr.php?id=234556&width=620&height=460

		tmpstr = gethttps(url, NULL, NULL, NULL, NULL, pageurl, 1);
		free(host), host = NULL;
		free(path), path = NULL;
		host = string_resub("http://", "/", url, 0);
		path = string_replace_all(host, "", url, 0);
		path = string_replace_all("http://", "", path, 1);
		path = string_replace_all(" ", "%20", path, 1);
		titheklog(debuglevel, "/var/usr/local/share/titan/plugins/player/tithek/tvtoast9_tmpstr", oitoa(incount), host, path, tmpstr);
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

#endif