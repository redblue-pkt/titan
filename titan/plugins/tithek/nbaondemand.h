#ifndef NBAONDEMAND_H
#define NBAONDEMAND_H

char* nbaondemand(char* link)
{
 int incount = 0;
	int debuglevel = getconfigint("debuglevel", NULL);

	debug(99, "nbaondemand(%d) link=%s ", incount, link);
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

#endif