#ifndef MIGHTYUPLOAD_H
#define MIGHTYUPLOAD_H

char* mightyupload(char* link)
{
	debug(99, "link: %s", link);
	int debuglevel = getconfigint("debuglevel", NULL);
	char* tmphost = NULL, *tmplink = NULL, *tmppath = NULL, *tmpstr = NULL, *streamlink = NULL, *pos = NULL, *path = NULL, *url = NULL;

	if(link == NULL) return NULL;

	unlink("/tmp/mightyupload1_get");
	unlink("/tmp/mightyupload2_streamlink");
/////////////

	tmplink = ostrcat(link, NULL, 0, 0);

	if(ostrstr(link, "/Out/?s=") != NULL)
	{
		tmplink = string_replace("/Out/?s=", "", tmplink, 1);
		debug(99, "remove out string: %s", tmplink);
	}

	if(tmplink == NULL || ostrncmp("http://", tmplink, 7))
	{
		textbox(_("Message"), _("Hoster Url not http://") , _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 1200, 200, 0, 0);
		goto end;
	}

	tmphost = string_replace("http://", "", tmplink, 0);
	free(tmplink) , tmplink = NULL;

	if(tmphost != NULL)
		pos = strchr(tmphost, '/');
	if(pos != NULL)
	{
		pos[0] = '\0';
		path = pos + 1;
	}

	tmppath = ostrcat("/embed-", path, 0, 0);
	tmppath = ostrcat(tmppath, ".html", 1, 0);

/*
GET /embed-a8vftxnrbb4f.html HTTP/1.1\r\nHost: www.mightyupload.com\r\nUser-Agent: Mozilla/5.0 (X11; Linux i686) AppleWebKit/535.1 (KHTML, like Gecko) Chrome/13.0.782.99 Safari/535.1\r\nCookie: __cfduid=d5477749af038852d91eb91ace5f3aacc1442153994\r\nConnection: close\r\nAccept-Encoding: gzip\r\n\r\n'
GET /d/5w75doudvle32chj4adc5s7gqdum2oy4psaxoeyzxy52kjfsrrvwj6iq/video.mkv HTTP/1.1\r\nAccept-Encoding: identity\r\nHost: 192.96.204.216:182\r\nConnection: close\r\nUser-Agent: Mozilla/5.0 (Windows NT 6.1; WOW64; Trident/7.0; AS; rv:11.0) like Gecko\r\n\r\n'
http://192.96.204.216:182/d/5w75doudvle32chj4adc5s7gqdum2oy4psaxoeyzxy52kjfsrrvwj6iq/video.mkv|User-Agent=Mozilla/5.0 (Windows NT 6.1; WOW64; Trident/7.0; AS; rv:11.0) like Gecko



eval(function(p,a,c,k,e,d){})

eval(function(p,a,c,k,e,d){while(c--)if(k[c])p=p.replace(new RegExp('\\b'+c.toString(a)+'\\b','g'),k[c]);return p}('1l.1k(\'<c 14="1j"1i="1h:1g-1f-1e-1d-1c"v="u"t="s"1b="3://h.9.g/f/1a.19"><2 1="z"0="y"/><2 1="x"0="w"/><2 1="a"0="3://8.7.6.5/r/i/q/p.o"/><2 1="l"0="4"><2 1="m"0="4"/><2 1="k"0="j"/><2 1="n"0="4"/><2 1="13"0="3://8.7.6.5:12/d/11/b.10"/><e 14="18"17="b/9"13="3://8.7.6.5:12/d/11/b.10"z="y"x="w"v="u"t="s"a="3://8.7.6.5/r/i/q/p.o"n="4"m="4"a=""l="4"k="j"16="3://h.9.g/f/15/"></e></c>\');',36,58,'value|name|param|http|false|216|204|96|192|divx|previewImage|video|object||embed|plugin|com|go||Play|previewMessage|allowContextMenu|bannerEnabled|autoPlay|jpg|a8vftxnrbb4f|00410|bc|353|height|645|width|transparent|wmode|Stage6|custommode|mkv|5w75doudvle32chj4adc5s7gqdum2oy4psaxoeyzxy52kjfsrtfz3tmr|182|src|id|download|pluginspage|type|np_vid|cab|DivXBrowserPlugin|codebase|CC0F21721616|9C46|41fa|D0AB|67DABFBF|clsid|classid|ie_vid|write|document'.split('|')))

document.write('<object id="ie_vid"classid="clsid:67DABFBF-D0AB-41fa-9C46-CC0F21721616"width="645"height="353"codebase="http://go.divx.com/plugin/DivXBrowserPlugin.cab"><param name="custommode"value="Stage6"/><param name="wmode"value="transparent"/><param name="previewImage"value="http://192.96.204.216/bc/i/00410/a8vftxnrbb4f.jpg"/><param name="allowContextMenu"value="false"><param name="bannerEnabled"value="false"/><param name="previewMessage"value="Play"/><param name="autoPlay"value="false"/><param name="src"value="http://192.96.204.216:182/d/5w75doudvle32chj4adc5s7gqdum2oy4psaxoeyzxy52kjfsrtfz3tmr/video.mkv"/><embed id="np_vid"type="video/divx"src="http://192.96.204.216:182/d/5w75doudvle32chj4adc5s7gqdum2oy4psaxoeyzxy52kjfsrtfz3tmr/video.mkv"custommode="Stage6"wmode="transparent"width="645"height="353"previewImage="http://192.96.204.216/bc/i/00410/a8vftxnrbb4f.jpg"autoPlay="false"bannerEnabled="false"previewImage=""allowContextMenu="false"previewMessage="Play"pluginspage="http://go.divx.com/plugin/download/"></embed></object>');


3://8.7.6.5:12/d/11/b.10
value|name|param|http|false|216|204|96|192|divx|previewImage|video|object||embed|plugin|com|go||Play|previewMessage|allowContextMenu|bannerEnabled|autoPlay|jpg|a8vftxnrbb4f|00410|bc|353|height|645|width|transparent|wmode|Stage6|custommode|mkv|5w75doudvle32chj4adc5s7gqdum2oy4psaxoeyzxy52kjfsrtfz3tmr|182|src|id|download|pluginspage|type|np_vid|cab|DivXBrowserPlugin|codebase|CC0F21721616|9C46|41fa|D0AB|67DABFBF|clsid|classid|ie_vid|write
http://192.96.204.216:12/d/11/b.10

1h:1g-1f-1e-1d-1c
3://h.9.g/f/1a.19
3://8.7.6.5/r/i/q/p.o
3://8.7.6.5:12/d/11/b.10
3://8.7.6.5:12/d/11/b.10
3://8.7.6.5/r/i/q/p.o
3://h.9.g/f/15/

0 value|
1 name
2 param
3 http
4 false
5 216
6 204
7 96
8 192
9 divx
a previewImage
b video
c object
d
e embed
f plugin
g com
h go
i
j
k Play
l previewMessage
m allowContextMenu
n bannerEnabled
o autoPlay
p jpg
q a8vftxnrbb4f
r 00410
s bc
t 353
u height
v 645
w width
x transparent
y wmode
z Stage6
custommode
10 mkv
11 5w75doudvle32chj4adc5s7gqdum2oy4psaxoeyzxy52kjfsrtfz3tmr
12 182
src
id
download
pluginspage
type
np_vid
cab
DivXBrowserPlugin
codebase
CC0F21721616
9C46
41fa
D0AB
67DABFBF
clsid
classid
ie_vid
write
*/

	char* id = oregex("http://.*/(.*)/.*", link);
	debug(99, "id: %s", id);

	debug(99, "tmppath: %s", tmppath);
	debug(99, "tmphost: %s", tmphost);
	url = ostrcat("www.mightyupload.com/embed-", id, 0, 0);
	url = ostrcat(url, ".html", 1, 0);

	debug(99, "url: %s", link);
	tmpstr = gethttps(url, NULL, NULL, NULL, NULL, 1);
	titheklog(debuglevel, "/var/usr/local/share/titan/plugins/tithek/mightyupload1_get", NULL, NULL, NULL, tmpstr);	
	tmpstr = jsunpack(tmpstr);
	titheklog(debuglevel, "/var/usr/local/share/titan/plugins/tithek/mightyupload2_get_jsunpack", NULL, NULL, NULL, tmpstr);	

//////////////////		
	if(tmpstr == NULL)
	{
		textbox(_("Message"), _("The page is temporarily unavailable") , _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 1200, 200, 0, 0);
		goto end;
	}

	if(ostrstr(tmpstr, "404 Not Found") != NULL)
	{
		textbox(_("Message"), _("The video no longer exists") , _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 1200, 200, 0, 0);
		goto end;
	}

	streamlink = string_resub("name=\"src\"value=\"", "\"", tmpstr, 0);
	if(streamlink == NULL)
		streamlink = string_resub("'label' : '240p', 'file' : '", "'", tmpstr, 0);		

	titheklog(debuglevel, "/tmp/mightyupload3_streamlink", NULL, NULL, NULL, streamlink);

end:

	free(url); url = NULL;
	free(tmphost); tmphost = NULL;
	free(tmppath); tmppath = NULL;
	free(tmplink); tmplink = NULL;
	free(tmpstr); tmpstr = NULL;

	return streamlink;
}


#endif
