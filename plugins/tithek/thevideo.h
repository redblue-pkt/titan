#ifndef THEVIDEO_H
#define THEVIDEO_H

char* thevideo(char* link)
{
	debug(99, "link: %s", link);
	int debuglevel = getconfigint("debuglevel", NULL);
	char* tmphost = NULL, *tmplink = NULL, *tmppath = NULL, *tmpstr = NULL, *streamlink = NULL, *pos = NULL, *path = NULL, *url = NULL;
	char* vhash = NULL, *gfk = NULL, *fname = NULL, *hash = NULL, *post = NULL, *inhu = NULL, *op = NULL, *vt = NULL, *tmpstr1 = NULL, *tmpstr2 = NULL, *tmpstr3 = NULL;

	if(link == NULL) return NULL;

	unlink("/tmp/thevideo1_get");
	unlink("/tmp/thevideo2_post");
	unlink("/tmp/thevideo3_get");
	unlink("/tmp/thevideo4_streamlink");
/////////////

	tmplink = ostrcat(link, NULL, 0, 0);

	if(ostrstr(link, "/Out/?s=") != NULL)
	{
		tmplink = string_replace("/Out/?s=", "", tmplink, 1);
		debug(99, "remove out string: %s", tmplink);
	}

	if(tmplink == NULL || ostrncmp("http", tmplink, 4))
	{
		textbox(_("Message"), _("Hoster Url not http:// or https://") , _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 1200, 200, 0, 0);
		goto end;
	}

	tmphost = string_replace("http://", "", tmplink, 0);
	tmphost = string_replace("https://", "", tmphost, 1);
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

//	tmppath = ostrcat("/", path, 0, 0);

/*
http://thevideo.me/g5oi83o7260u
'GET /embed-g5oi83o7260u.html HTTP/1.1\r\nHost: thevideo.me\r\nUser-Agent: Mozilla/5.0 (X11; Linux i686) AppleWebKit/535.1 (KHTML, like Gecko) Chrome/13.0.782.99 Safari/535.1\r\nConnection: close\r\nAccept-Encoding: gzip\r\n\r\n'
'GET /ikjtbmjr5woammfvg77fchotfr76hz35ahh6bglfezhodqxskyt544voxnpq/v.mp4 HTTP/1.1\r\nAccept-Encoding: identity\r\nHost: d2171.thevideo.me:8777\r\nCookie: __cfduid=de1d2f326ba9713a1f3a8497c518c5ee01441986583\r\nConnection: close\r\nUser-Agent: Python-urllib/2.7\r\n\r\n'
http://d2171.thevideo.me:8777/ikjtbmjr5woammfvg77fchotfr76hz35ahh6bglfezhodqxskyt544voxnpq/v.mp4
http://d2171.thevideo.me:8777/ikjtbmjr5woammfvg77fchotfr76hz35ahh6bglfezhodqxskyt544voxnpq/v.mp4|User-Agent=Mozilla/5.0 (Windows; U; Windows NT 5.1; de-DE; rv:1.9.0.3) Gecko/2008092417 Firefox/3.0.3 -> application/octet-stream
*/
	debug(99, "tmppath: %s", tmppath);
	debug(99, "tmphost: %s", tmphost);
	url = ostrcat(tmphost, tmppath, 0, 0);
	debug(99, "url: %s", url);
	tmpstr = gethttps(link, NULL, NULL, NULL, NULL, NULL, 1);
	titheklog(debuglevel, "/tmp/thevideo1_get", NULL, NULL, NULL, tmpstr);	

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

	vhash = string_resub("'_vhash', value: '", "'", tmpstr, 0);
	gfk = string_resub("'gfk', value: '", "'", tmpstr, 0);
	fname = string_resub("name=\"fname\" value=\"", "\"", tmpstr, 0);
	hash = string_resub("name=\"hash\" value=\"", "\"", tmpstr, 0);
	op = string_resub("name=\"op\" value=\"", "\"", tmpstr, 0);
	inhu = string_resub("name=\"inhu\" value=\"", "\"", tmpstr, 0);
	free(tmpstr); tmpstr = NULL;

	post = ostrcat("_vhash=", vhash, 0, 0);
	post = ostrcat(post, "&gfk=", 1, 0);
	post = ostrcat(post, gfk, 1, 0);
	post = ostrcat(post, "&op=", 1, 0);
	post = ostrcat(post, op, 1, 0);
	post = ostrcat(post, "&usr_login=&id=", 1, 0);
	post = ostrcat(post, path, 1, 0);
	post = ostrcat(post, "&fname=", 1, 0);
	post = ostrcat(post, fname, 1, 0);
	post = ostrcat(post, "&referer=http%3A%2F%2Fthevideo.me%2F", 1, 0);
	post = ostrcat(post, path, 1, 0);
	post = ostrcat(post, "&hash=", 1, 0);
	post = ostrcat(post, hash, 1, 0);
	post = ostrcat(post, "&inhu=", 1, 0);
	post = ostrcat(post, inhu, 1, 0);
	post = ostrcat(post, "&imhuman=", 1, 0);

	debug(99, "post: %s", post);

	debug(99, "url: %s", url);
	tmpstr = gethttps(link, NULL, post, NULL, NULL, link, 1);
	titheklog(debuglevel, "/tmp/thevideo2_post", NULL, NULL, NULL, tmpstr);	

	streamlink = string_resub("'label' : '360p', 'file' : '", "'", tmpstr, 0);
	titheklog(debuglevel, "/tmp/thevideo2_post_streamlink1", NULL, NULL, NULL, streamlink);	

	if(streamlink == NULL)
		streamlink = string_resub("'label' : '240p', 'file' : '", "'", tmpstr, 0);		
	titheklog(debuglevel, "/tmp/thevideo2_post_streamlink2", NULL, NULL, NULL, streamlink);	

	if(streamlink == NULL)
		streamlink = string_resub("label: '240p', file: '", "'", tmpstr, 0);		
	titheklog(debuglevel, "/tmp/thevideo2_post_streamlink3", NULL, NULL, NULL, streamlink);	

	if(streamlink == NULL)
		streamlink = string_resub("label: '360p', file: '", "'", tmpstr, 0);
	titheklog(debuglevel, "/tmp/thevideo2_post_streamlink4", NULL, NULL, NULL, streamlink);	


//  sources: [{"file":"https://d3861.thevideo.me:8777/kgjtaznnpgoammfvg54fmh4ihf6iyptt3axejg7zooshqaa3rl72oia2ejcvbrt3zezduyuhrnsu4yyrb72hjkhujsdhg4rasehxwt7whmlzyd6wf64y5r47h2ejyzqv454juyd5yfxocddcstfngy67yio7zymsqvnlljjjyybcjt4xzduxi6dpzsa24ioimmsajr27mzxgbdqvtz76rbms3txhjitcquabsj5mbnfryf5a362fxqlo5awjzdieb5uypjhvcg53rjuvc7ei2pm4oipa/v.mp4","label":"240p"}],

	if(streamlink == NULL)		
		streamlink = oregex(".*sources:.*\"file\":\"(http.*v.mp4).*", tmpstr);
//		streamlink = oregex(".*sources: [{"file":"(http://.*v.mp4).*", tmpstr);
	titheklog(debuglevel, "/tmp/thevideo2_post_streamlink5", NULL, NULL, NULL, streamlink);	

	if(streamlink == NULL)
		streamlink = oregex(".*(http://.*v.mp4).*", tmpstr);
	titheklog(debuglevel, "/tmp/thevideo2_post_streamlink6", NULL, NULL, NULL, streamlink);	

//	var mpri_Key='LCw2QFU8N0hRPFdBWi4zLVgK';
//	eval(function(p,a,c,k,e,d){e=function(c){return c};if(!''.replace(/^/,String)){while(c--){d[c]=k[c]||c}k=[function(e){return d[e]}];e=function(){return'\\w+'};c=1};while(c--){if(k[c]){p=p.replace(new RegExp('\\b'+e(c)+'\\b','g'),k[c])}}return p}('3.2(\'<0 4\'.1(\'8="\'+\'/5\'.1(\'/\'+7+\'">\\6/0>\')))',9,9,'script|concat|write|document|s|jwv|x3C|mpri_Key|rc'.split('|'),0,{}))
	tmpstr = jsunpack(tmpstr);
	titheklog(debuglevel, "/tmp/thevideo3_post_jsunpack", NULL, NULL, NULL, tmpstr);	

	tmpstr1 = string_resub("<div class=\"container main-container\">", "</div>", tmpstr, 0);
	free(tmpstr), tmpstr = NULL;
	debug(99, "tmpstr1: %s", tmpstr1);
	url = string_resub("<script src=\"", "\"></script>", tmpstr1, 0);

	if(url == NULL)
	{
		tmpstr2 = string_resub("+\\'", "\\'.concat", tmpstr1, 0);
//		tmpstr3 = string_resub("var tksucks='", "';", tmpstr1, 0);
		tmpstr3 = oregex("var .*='(.*)';.*", tmpstr1);

		url = ostrcat("http://thevideo.me", tmpstr2, 0, 0);
		url = ostrcat(url, "/", 1, 0);
		url = ostrcat(url, tmpstr3, 1, 0);
		free(tmpstr2), tmpstr2 = NULL;
		free(tmpstr3), tmpstr3 = NULL;
	}

/*
//old
	if(url == NULL)
	{
		tmpstr2 = string_resub(",9,9,'", "'.split", tmpstr1, 0);
	
		int count = 0;
		int j;
		struct splitstr* ret1 = NULL;
		ret1 = strsplit(tmpstr2, "|", &count);
		if(ret1 != NULL && count > 0)
		{
			for(j = 0; j < count; j++)
			{
				if(ostrcmp(ret1[j].part, "script") != 0 && ostrcmp(ret1[j].part, "concat") != 0 && ostrcmp(ret1[j].part, "write") != 0 && ostrcmp(ret1[j].part, "document") != 0 && ostrcmp(ret1[j].part, "s") != 0 && ostrcmp(ret1[j].part, "x3C") != 0 && ostrcmp(ret1[j].part, "mpri_Key") != 0 && ostrcmp(ret1[j].part, "rc") != 0)
					tmpstr3 = ostrcat("/", ret1[j].part, 0, 0);
			}
		}
		free(ret1); ret1 = NULL;
	//  script|concat|write|document|s|jwv|x3C|mpri_Key|rc

		tmpstr4 = oregex("var .*='(.*)';.*", tmpstr1);

		url = ostrcat("http://thevideo.me", tmpstr3, 0, 0);
		url = ostrcat(url, "/", 1, 0);
		url = ostrcat(url, tmpstr4, 1, 0);
		free(tmpstr2), tmpstr2 = NULL;
		free(tmpstr3), tmpstr3 = NULL;
		free(tmpstr4), tmpstr4 = NULL;
	}

// old	
	if(url == NULL)
	{
		tmpstr2 = string_resub("' + '", "'.concat", tmpstr1, 0);
//		tmpstr3 = string_resub("var tksucks='", "';", tmpstr1, 0);
		tmpstr3 = oregex("var .*='(.*)';.*", tmpstr1);

		url = ostrcat("http://thevideo.me", tmpstr2, 0, 0);
		url = ostrcat(url, "/", 1, 0);
		url = ostrcat(url, tmpstr3, 1, 0);
		free(tmpstr2), tmpstr2 = NULL;
		free(tmpstr3), tmpstr3 = NULL;
	}
*/	

	free(tmpstr1), tmpstr1 = NULL;
	
/*
	<script src="https://thevideo.me/jwjsv/sfqlqb288ft2"></script>
	https://thevideo.me/jwjsv/sfqlqb288ft2


	var tksucks='LDw2JVcsJyVSPEchUz4zMUcK';document.write('<script s'.concat( 'rc="' + '/jwv'.concat( '/'+tksucks+'">\x3C/script>' ) ) )
	http://thevideo.me/jwv/LDlHJFE+RihWPEMtUz5DKUM
*/

	tmpstr = gethttps(url, NULL, NULL, NULL, NULL, link, 1);
	titheklog(debuglevel, "/tmp/thevideo3_get", NULL, NULL, NULL, tmpstr);	

	tmpstr1 = string_resub("[],30,'", "'.split('|')", tmpstr, 0);
	int count = 0;
	int j;
	struct splitstr* ret1 = NULL;
	ret1 = strsplit(tmpstr1, "|", &count);

	if(ret1 != NULL && count > 0)
	{
		for(j = 0; j < count; j++)
		{
			if(strlen(ret1[j].part) > 50)
				vt = ostrcat(ret1[j].part, NULL, 0, 0);
		}
	}
	free(ret1); ret1 = NULL;

/*
	https://d4242.thevideo.me:8777/gwjta3agywoammfvg7sfgcmaglluse7vcwc6gn6frivojkuv2dk6h7fk77bpqtu5v6uxlvg6okwz2hdqdykueqnd5hllm5bnwwveiqijxa5b6wzozls26gn3f7ci6pvr44jhdqjt3ndyur5ecocf2pz4mlwtmdzps4xtk7jdce3ezweuna7ww2u4pmujltietcuedpvveh4tc6sjv3oxljsajrq5hz22vtwthssqknhd3v5psrp2rur2gxroejzewfek5dbr/v.mp4?direct=false&ua=1&vt=osv3jha2zaetuu35h7nm4hpodzb7raf5clkelbe2gmgew3qcowp257vjk347pb6fshhj3uku2vggcte3ix4tqwp6gy5twendw4wrkj76ms437ghk76hhb5iimrpmjgt4uee4hdhctjvy72pfx7us3usbvyiwxpizl5mbo2alkrb4g253pznnrqn5ub4a7w36p3lirdnfoxogogs7pnsspgmxi6wqinys7orguna
*/

	if(streamlink != NULL)
	{
		streamlink = ostrcat(streamlink, "?direct=false&ua=1&vt=", 1, 0);
		if(vt != NULL)
			streamlink = ostrcat(streamlink, vt, 1, 0);
		else
			streamlink = ostrcat(streamlink, "1", 1, 0);
	}

	titheklog(debuglevel, "/tmp/thevideo4_streamlink", NULL, NULL, NULL, streamlink);

end:

	free(vhash); vhash = NULL;
	free(gfk); gfk = NULL;
	free(fname); fname = NULL;
	free(hash); hash = NULL;
	free(post); post = NULL;
	free(inhu); inhu = NULL;
	free(op); op = NULL;
	free(vt); vt = NULL;
	free(url); url = NULL;
	free(tmphost); tmphost = NULL;
	free(tmppath); tmppath = NULL;
	free(tmplink); tmplink = NULL;
	free(tmpstr); tmpstr = NULL;
	free(tmpstr1); tmpstr1 = NULL;

	return streamlink;
}

#endif
