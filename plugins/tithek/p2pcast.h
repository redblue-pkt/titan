#ifndef P2PCAST_H
#define P2PCAST_H

char* p2pcast(char* link)
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

	system("rm -rf /var/usr/local/share/titan/plugins/tithek/p2pcast*");
/*
	http://p2pcast.net/philadelphia-76ers-at-oklahoma-city-thunder-live-stream.html
	<iframe  src="streams/nbanhl8.html" width="680" height="400" scrolling="no" frameborder="0" allowfullscreen></iframe><br>
*/
	id = string_resub("?id=", "&", link, 0);

	host = string_resub("http://", "/", link, 0);
	path = string_replace_all(host, "", link, 0);
	path = string_replace_all("http://", "", path, 1);
	path = string_replace_all(" ", "%20", path, 1);
	tmpstr = gethttps(link, NULL, NULL, NULL, NULL, NULL, 1);
	titheklog(debuglevel, "/var/usr/local/share/titan/plugins/tithek/p2pcast1_tmpstr", NULL, host, path, tmpstr);



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


char* tmpstr2 = string_resub("<iframe id=", "<script>", tmpstr, 0);
url3 = string_resub("<script src=\"", "\"", tmpstr2, 0);

	free(tmpstr), tmpstr = NULL;
	tmpstr = gethttps(url3, NULL, NULL, NULL, NULL, link, 1);
	free(host), host = NULL;
	free(path), path = NULL;
	host = string_resub("http://", "/", url3, 0);
	path = string_replace_all(host, "", url3, 0);
	path = string_replace_all("http://", "", path, 1);
	path = string_replace_all(" ", "%20", path, 1);
	titheklog(debuglevel, "/var/usr/local/share/titan/plugins/tithek/p2pcast3a_tmpstr", NULL, host, path, tmpstr);


token = string_resub(".getJSON(\"", "\"", tmpstr, 0);

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
	titheklog(debuglevel, "/var/usr/local/share/titan/plugins/tithek/p2pcast5_tmpstr", NULL, host, path, tmpstr);

//	url = oregex(".*<script src=\"(http://.*)\"></script>.*", tmpstr);
//	url5 = ostrcat("http://p2pcast.tv/getToken.php", NULL, 0, 0);
	url5 = ostrcat("http://p2pcast.tv/", token, 0, 0);
	free(token), token = NULL;

	free(tmpstr), tmpstr = NULL;
	tmpstr = gethttps(url5, NULL, NULL, NULL, NULL, link, 1);
	free(host), host = NULL;
	free(path), path = NULL;
	host = string_resub("http://", "/", url5, 0);
	path = string_replace_all(host, "", url5, 0);
	path = string_replace_all("http://", "", path, 1);
	path = string_replace_all(" ", "%20", path, 1);
	titheklog(debuglevel, "/var/usr/local/share/titan/plugins/tithek/p2pcast6_tmpstr", NULL, host, path, tmpstr);

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
	titheklog(debuglevel, "/var/usr/local/share/titan/plugins/tithek/p2pcast7_tmpstr", NULL, host, path, tmpstr);


	streamurl = string_replace("http://live.p2pcast.tv:8010", "http://e10.swfgames.xyz", streamurl, 1);

	if(streamurl != NULL)
		streamurl = ostrcat(streamurl, "|User-Agent=libcurl-agent/1.0|Referer=http://cdn.p2pcast.tv/jwplayer.flash.swf", 1, 0);



//http://e10.swfgames.xyz/tv2/510516.m3u8?sf=NTZhMDE2NDJjZGRhMA==&token=1PDk3ZvUclZZtHhBUTS7Kw
//http://live.p2pcast.tv:8010/tv2/504917.m3u8?sf=NTYyMmRlZWNjZTEyNQ==&token=I-OAZrLqbye4LgcL6nhHkQ

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

#endif