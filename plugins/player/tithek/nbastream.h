#ifndef NBASTREAM_H
#define NBASTREAM_H

char* nbastream(char* link)
{
	debug(99, "link %s", link);
	int debuglevel = getconfigint("debuglevel", NULL);
	char* streamurl = NULL, *tmpstr = NULL, *tmpstr1 = NULL;
	char* host = NULL;
	char* path = NULL;
	char* url1 = NULL;
	char* url2 = NULL;
	char* url3 = NULL;
	char* url4 = NULL;
	char* url5 = NULL;
	char* id = NULL;
	char* width = NULL;
	char* height = NULL;
	char* curlstring = NULL;
	char* token = NULL;

	system("rm -rf /var/usr/local/share/titan/plugins/tithek/nbastream*");
/*
	http://nbastream.net/philadelphia-76ers-at-oklahoma-city-thunder-live-stream.html
	<iframe  src="streams/nbanhl8.html" width="680" height="400" scrolling="no" frameborder="0" allowfullscreen></iframe><br>
*/
	host = string_resub("http://", "/", link, 0);
	path = string_replace_all(host, "", link, 0);
	path = string_replace_all("http://", "", path, 1);
	path = string_replace_all(" ", "%20", path, 1);
	tmpstr = gethttps(link, NULL, NULL, NULL, NULL, NULL, 1);
	titheklog(debuglevel, "/var/usr/local/share/titan/plugins/tithek/nbastream1_tmpstr", NULL, host, path, tmpstr);


	url1 = string_resub("<iframe  src=\"", "\"", tmpstr, 0);
	url1 = ostrcat("http://nbastream.net/", url1, 0, 1);

/*
	http://nbastream.net/streams/nbanhl8.html
	<script type='text/javascript'>id='504917'; width='670'; height='390';</script><script type='text/javascript' src='http://js.p2pcast.tv/p2pcast/player.js'></script>
*/
	free(tmpstr), tmpstr = NULL;
	tmpstr = gethttps(url1, NULL, NULL, NULL, NULL, NULL, 1);
	free(host), host = NULL;
	free(path), path = NULL;
	host = string_resub("http://", "/", url1, 0);
	path = string_replace_all(host, "", url1, 0);
	path = string_replace_all("http://", "", path, 1);
	path = string_replace_all(" ", "%20", path, 1);
	titheklog(debuglevel, "/var/usr/local/share/titan/plugins/tithek/nbastream2_tmpstr", NULL, host, path, tmpstr);

	url2 = string_resub("src='", "'", tmpstr, 0);
	id = string_resub("id='", "'", tmpstr, 0);
	width = string_resub("id='", "'", tmpstr, 0);
	height = string_resub("id='", "'", tmpstr, 0);

///////////////
	char* url7 = ostrcat("http://www.google-analytics.com/analytics.js", NULL, 0, 0);
	free(tmpstr), tmpstr = NULL;
	tmpstr = gethttps(url7, NULL, NULL, NULL, NULL, link, 1);
	free(host), host = NULL;
	free(path), path = NULL;
	host = string_resub("http://", "/", url7, 0);
	path = string_replace_all(host, "", url7, 0);
	path = string_replace_all("http://", "", path, 1);
	path = string_replace_all(" ", "%20", path, 1);
	titheklog(debuglevel, "/var/usr/local/share/titan/plugins/tithek/nbastream2a_tmpstr", NULL, host, path, tmpstr);
///////////////

/*
	http://js.p2pcast.tv/p2pcast/player.js
	document.write('<iframe allowfullscreen width="'+width+'" height="'+height+'" scrolling="no" frameborder="0" marginwidth="0" marginheight="0" scrolling="no" allowtransparency="true" src="http://p2pcast.tv/stream.php?id='+id+'&live='+live+'&p2p='+p2p+'&stretching='+stretching+'"></iframe>');
*/

	free(tmpstr), tmpstr = NULL;
	tmpstr = gethttps(url2, NULL, NULL, NULL, NULL, url1, 1);
	free(host), host = NULL;
	free(path), path = NULL;
	host = string_resub("http://", "/", url2, 0);
	path = string_replace_all(host, "", url2, 0);
	path = string_replace_all("http://", "", path, 1);
	path = string_replace_all(" ", "%20", path, 1);
	titheklog(debuglevel, "/var/usr/local/share/titan/plugins/tithek/nbastream3_tmpstr", NULL, host, path, tmpstr);


	char* url8 = ostrcat("http://ax2.adit-media.com/asw/fm.js?c=1033&a=0&f=&n=2019&r=141&d=9&adm=&q=&$=&s=901&ct=&z=0.4985966447275132&tt=0&tz=1&pu=http%3A%2F%2Fnbastream.net%2Fstreams%2Fnbanhl2.html&ru=http%3A%2F%2Fnbastream.net%2Forlando-magic-at-washington-wizards-live-stream.html&pi=1447548879638&ce=UTF-8&zpu=nbastream.net_nbastream.net__0_2&tpu=0", NULL, 0, 0);
	free(tmpstr), tmpstr = NULL;
	tmpstr = gethttps(url8, NULL, NULL, NULL, NULL, url1, 1);
	free(host), host = NULL;
	free(path), path = NULL;
	host = string_resub("http://", "/", url7, 0);
	path = string_replace_all(host, "", url7, 0);
	path = string_replace_all("http://", "", path, 1);
	path = string_replace_all(" ", "%20", path, 1);
	titheklog(debuglevel, "/var/usr/local/share/titan/plugins/tithek/nbastream3a_tmpstr", NULL, host, path, tmpstr);

/*
	http://p2pcast.tv/stream.php?id=504917&live=0&p2p=0&stretching=uniform
    curl = "aHR0cDovL2xpdmUucDJwY2FzdC50djo4MDEwL3R2Mi81MDQ5MTcubTN1OD9zZj1OVFl5TW1SbFpXTmpaVEV5TlE9PSZ0b2tlbj0=";
	<script src="http://js.p2pcast.tv/p2pcast/pplayer.10.js"></script>
*/
/*

	url3 = string_resub("src=\"", "\"", tmpstr, 0);
	url3 = string_replace_all("'+id+'", id, url3, 1);
	url3 = string_replace_all("'+live+'", "0", url3, 1);
	url3 = string_replace_all("'+p2p+'", "0", url3, 1);
	url3 = string_replace_all("'+stretching+'", "uniform", url3, 1);
*/
	url3 = ostrcat("http://p2pcast.tv/stream.php?id=", id, 0, 0);
	url3 = ostrcat(url3, "&live=0&p2p=0&stretching=uniform", 1, 0);

	free(tmpstr), tmpstr = NULL;
	tmpstr = gethttps(url3, NULL, NULL, NULL, NULL, url1, 1);
	free(host), host = NULL;
	free(path), path = NULL;
	host = string_resub("http://", "/", url3, 0);
	path = string_replace_all(host, "", url3, 0);
	path = string_replace_all("http://", "", path, 1);
	path = string_replace_all(" ", "%20", path, 1);
	titheklog(debuglevel, "/var/usr/local/share/titan/plugins/tithek/nbastream4_tmpstr", NULL, host, path, tmpstr);

	if(tmpstr == NULL || ostrstr(tmpstr, "<html lang=\"en\" class=\"explore\">") != NULL)
	{
		tmpstr1 = string_resub("<h2>", "</h2>", tmpstr, 0);
		textbox(_("Message"), tmpstr1 , _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 1200, 200, 0, 0);
//		goto end;
	}

/*
	http://js.p2pcast.tv/p2pcast/pplayer.10.js
	var Base64={_keyStr:"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/=",encode:function(e){var t="";var n,r,i,s,o,u,a;var f=0;e=Base64._utf8_encode(e);while(f<e.length){n=e.charCodeAt(f++);r=e.charCodeAt(f++);i=e.charCodeAt(f++);s=n>>2;o=(n&3)<<4|r>>4;u=(r&15)<<2|i>>6;a=i&63;if(isNaN(r)){u=a=64}else if(isNaN(i)){a=64}t=t+this._keyStr.charAt(s)+this._keyStr.charAt(o)+this._keyStr.charAt(u)+this._keyStr.charAt(a)}return t},decode:function(e){var t="";var n,r,i;var s,o,u,a;var f=0;e=e.replace(/[^A-Za-z0-9\+\/\=]/g,"");while(f<e.length){s=this._keyStr.indexOf(e.charAt(f++));o=this._keyStr.indexOf(e.charAt(f++));u=this._keyStr.indexOf(e.charAt(f++));a=this._keyStr.indexOf(e.charAt(f++));n=s<<2|o>>4;r=(o&15)<<4|u>>2;i=(u&3)<<6|a;t=t+String.fromCharCode(n);if(u!=64){t=t+String.fromCharCode(r)}if(a!=64){t=t+String.fromCharCode(i)}}t=Base64._utf8_decode(t);return t},_utf8_encode:function(e){e=e.replace(/\r\n/g,"\n");var t="";for(var n=0;n<e.length;n++){var r=e.charCodeAt(n);if(r<128){t+=String.fromCharCode(r)}else if(r>127&&r<2048){t+=String.fromCharCode(r>>6|192);t+=String.fromCharCode(r&63|128)}else{t+=String.fromCharCode(r>>12|224);t+=String.fromCharCode(r>>6&63|128);t+=String.fromCharCode(r&63|128)}}return t},_utf8_decode:function(e){var t="";var n=0;var r=c1=c2=0;while(n<e.length){r=e.charCodeAt(n);if(r<128){t+=String.fromCharCode(r);n++}else if(r>191&&r<224){c2=e.charCodeAt(n+1);t+=String.fromCharCode((r&31)<<6|c2&63);n+=2}else{c2=e.charCodeAt(n+1);c3=e.charCodeAt(n+2);t+=String.fromCharCode((r&15)<<12|(c2&63)<<6|c3&63);n+=3}}return t}}
	var xurl = Base64.decode(curl);
*/
	curlstring = string_resub("curl = \"", "\"", tmpstr, 0);
	if(curlstring == NULL)
		curlstring = ostrcat("aHR0cDovL2xpdmUucDJwY2FzdC50djo4MDEwL3R2Mi81MDQ5MTcubTN1OD9zZj1OVFl5TW1SbFpXTmpaVEV5TlE9PSZ0b2tlbj0=", NULL, 0 ,0);
	printf("curlstring input: %s\n", curlstring);
	b64dec(curlstring, curlstring);
	printf("curlstring decod: %s\n", curlstring);

//aHR0cDovL2xpdmUucDJwY2FzdC50djo4MDEwL3R2Mi81MDQ5MTcubTN1OD9zZj1OVFl5TW1SbFpXTmpaVEV5TlE9PSZ0b2tlbj0=
//decode base64 = http://live.p2pcast.tv:8010/tv2/504917.m3u8?sf=NTYyMmRlZWNjZTEyNQ==&token=
/////////////////////
/*
//	http://js.p2pcast.tv/p2pcast/live.htm?channel=504715
	url4 = ostrcat("http://js.p2pcast.tv/p2pcast/live.htm?channel=", id, 0, 0);
	free(tmpstr), tmpstr = NULL;
	tmpstr = gethttps(url4, NULL, NULL, NULL, NULL, url3, 1);
	free(host), host = NULL;
	free(path), path = NULL;
	host = string_resub("http://", "/", url4, 0);
	path = string_replace_all(host, "", url4, 0);
	path = string_replace_all("http://", "", path, 1);
	path = string_replace_all(" ", "%20", path, 1);
	titheklog(debuglevel, "/var/usr/local/share/titan/plugins/tithek/nbastream5_tmpstr", NULL, host, path, tmpstr);
*/
//	url = oregex(".*<script src=\"(http://.*)\"></script>.*", tmpstr);
	url5 = ostrcat("http://p2pcast.tv/getToken.php", NULL, 0, 0);
	free(tmpstr), tmpstr = NULL;
	tmpstr = gethttps(url5, NULL, NULL, NULL, NULL, url3, 1);
	free(host), host = NULL;
	free(path), path = NULL;
	host = string_resub("http://", "/", url5, 0);
	path = string_replace_all(host, "", url5, 0);
	path = string_replace_all("http://", "", path, 1);
	path = string_replace_all(" ", "%20", path, 1);
	titheklog(debuglevel, "/var/usr/local/share/titan/plugins/tithek/nbastream6_tmpstr", NULL, host, path, tmpstr);

//	{"token":"I-OAZrLqbye4LgcL6nhHkQ"}
	token = string_resub("{\"token\":\"", "\"", tmpstr, 0);
	printf("token: %s\n", token);

	streamurl = ostrcat(curlstring, NULL, 0, 0);
	streamurl = ostrcat(streamurl, token, 1, 0);

	free(tmpstr), tmpstr = NULL;
	tmpstr = gethttps(streamurl, NULL, NULL, NULL, NULL, "http://cdn.p2pcast.tv/jwplayer.flash.swf", 1);
	free(host), host = NULL;
	free(path), path = NULL;
	host = string_resub("http://", "/", streamurl, 0);
	path = string_replace_all(host, "", streamurl, 0);
	path = string_replace_all("http://", "", path, 1);
	path = string_replace_all(" ", "%20", path, 1);
	titheklog(debuglevel, "/var/usr/local/share/titan/plugins/tithek/nbastream7_tmpstr", NULL, host, path, tmpstr);


	if(streamurl != NULL)
		streamurl = ostrcat(streamurl, "|User-Agent=libcurl-agent/1.0|Referer=http://cdn.p2pcast.tv/jwplayer.flash.swf", 1, 0);

//end:

	free(tmpstr), tmpstr = NULL;
	free(tmpstr1), tmpstr1 = NULL;
	free(host), host = NULL;
	free(path), path = NULL;
	free(url1), url1 = NULL;
	free(url2), url2 = NULL;
	free(url3), url3 = NULL;
	free(url4), url4 = NULL;
	free(url5), url5 = NULL;
	free(id), id = NULL;
	free(width), width = NULL;
	free(height), height = NULL;
	free(curlstring), curlstring = NULL;
	free(token), token = NULL;

	debug(99, "streamurl: %s", streamurl);	
	return streamurl;
}

int nbastream_search_local(struct skin* grid, struct skin* listbox, struct skin* countlabel, struct skin* load, char* link, char* title, char* searchstr, int flag)
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

		tmpstr = gethttp("openaaf.dyndns.tv", "/mediathek/nbastream/streams/nbastream.all-sorted.list", 80, NULL, HTTPAUTH, 5000, NULL, 0);

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
				menu = ostrcat("/tmp/tithek/nbastream.search.list", NULL, 0, 0);
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