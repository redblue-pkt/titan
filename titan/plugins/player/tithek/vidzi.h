#ifndef VIDZI_H
#define VIDZI_H

char* vidzi(char* link)
{
	debug(99, "link: %s", link);
	int debuglevel = getconfigint("debuglevel", NULL);
	char* url = NULL, *tmplink = NULL, *tmpstr = NULL, *streamlink = NULL;

	if(link == NULL) return NULL;

	unlink("/tmp/vidzi1_get");
	unlink("/tmp/vidzi2_get");
	unlink("/tmp/vidzi3_get_jsunpack");
	unlink("/tmp/vidzi4_streamlink");
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

	tmpstr = gethttps(tmplink, NULL, NULL, NULL, NULL, NULL, 1);
	titheklog(debuglevel, "/tmp/vidzi1_get", NULL, NULL, NULL, tmpstr);	
	url = string_resub("<IFRAME SRC=\"", "\"", tmpstr, 0);

	free(tmpstr), tmpstr = NULL;
	tmpstr = gethttps(url, NULL, NULL, NULL, NULL, NULL, 1);
	titheklog(debuglevel, "/tmp/vidzi2_get", NULL, NULL, NULL, tmpstr);	

// <script type='text/javascript'>eval(function(p,a,c,k,e,d){while(c--)if(k[c])p=p.replace(new RegExp('\\b'+c.toString(a)+'\\b','g'),k[c]);return p}('4("2t").2s({2r:[{2q:"2://w.u.t.s:r/i/2p/q/9.2o",2n:"2://2m.6.8/2l/6.2k",2j:[{a:"2://6.8/9-p.2i?2h=1",2g:\'2f\'},{a:"2://w.u.t.s:r/2e/v.2d"}],2c:[{a:"2://6.8/2b/q/2a.o",29:"28",n:"27"},{a:"2://6.8/9-p.o",n:"26"}]}],25:24,23:"22",21:3,1z:1y,1x:1,1w:1v,1u:1t,1s:{1r:l,1q:\'1p\',1o:\'1n\',1m:20},1l:1k});h b;h g=0;h 7=0;4().1j(5(x){c(7>0)g+=x.m-7;7=x.m;c(0!=0&&g>=0){7=-1;4().1i();4().1h(l);$(\'#1g\').f();$(\'e.d\').f()}});4().1f(5(x){7=-1});4().1e(5(x){k(x)});4().1d(5(){$(\'e.d\').f()});5 k(x){$(\'e.d\').1c();c(b)1b;b=1;$.1a(\'2://6.8/j-19/18.j?17=16&15=9&14=13-12-11-10-z\',5(y){})}',36,102,'||http||jwplayer|function|vidzi|p05950811|tv|ct59exiwef1s|file|vvplay|if|video_ad|div|show|tt5950811|var||cgi|doPlay|false|position|kind|vtt|cccc5cbb7c636de0200615961aff8187|01190|8777|151|98|81||77||data|4846483105d5e6b594d1b24c9e81a8bc|1473857569|91|95|5950811|hash|file_code|ajax|op|index_dl|bin|get|return|hide|onComplete|onPlay|onSeek|play_limit_box|setFullscreen|stop|onTime|640|width|fontsize|Verdana|fontFamily|FFFFFF|color|back|captions|900|hls_maxbackbufferlength|300|hls_maxbufferlength|hls_startfromlevel|true|androidhls||hls_minbufferlength|flash|primary|360|height|thumbnails|subtitles|German|label|ct59exiwef1s_German|srt|tracks|mp4|avuqilkaq42qedz7niyrvgbqag6xqez357sria2ac2uxlsd5oudxnewz7psa|hls|type|embed|m3u8|sources|swf|nplayer|static|provider|jpg|01|image|playlist|setup|vplayer'.split('|')))

	tmpstr = jsunpack(tmpstr);
	titheklog(debuglevel, "/tmp/vidzi3_get_jsunpack", NULL, NULL, NULL, tmpstr);	

//<script type='text/javascript'>eval(function(p,a,c,k,e,d){while(c--)if(k[c])p=p.replace(new RegExp('\\b'+c.toString(a)+'\\b','g'),k[c]);return p}('2("2p").2o({2n:[{2m:"5://s.r.q.p:o/i/2l/2k/2j.2i",2h:"5://2g.7.8/2f/7.2e",2d:[{h:"5://7.8/9-n.2c?2b=1",2a:\'29\'},{h:"5://s.r.q.p:o/28/v.27"}],26:[{h:"5://7.8/9-n.25",24:"23"}]}],22:21,1z:"1y",1x:3,1w:1v,1u:1,1t:1s,1r:1q,1p:{1o:l,1n:\'1m\',1l:\'1k\',1j:20},1i:1h});g a;g f=0;g 6=0;2().1g(4(x){b(6>0)f+=x.m-6;6=x.m;b(0!=0&&f>=0){6=-1;2().1f();2().1e(l);$(\'#1d\').e();$(\'d.c\').e()}});2().1c(4(x){6=-1});2().1b(4(x){k(x)});2().1a(4(){$(\'d.c\').e()});4 k(x){$(\'d.c\').19();b(a)18;a=1;$.17(\'5://7.8/j-16/15.j?14=13&12=9&11=10-z-y-w-u\',4(t){})}',36,98,'||jwplayer||function|http|p06025455|vidzi|tv|i8lwtc202q8p|vvplay|if|video_ad|div|show|tt6025455|var|file||cgi|doPlay|false|position|e99a0c1ed2279ab1569e49d28b8f54ef|8777|92|103|46|89|data|f7e283b9d19535ead88bdd056e69cc00||1473867522||91|95|6025455|hash|file_code|ajax|op|index_dl|bin|get|return|hide|onComplete|onPlay|onSeek|play_limit_box|setFullscreen|stop|onTime|640|width|fontsize|Verdana|fontFamily|FFFFFF|color|back|captions|900|hls_maxbackbufferlength|300|hls_maxbufferlength|hls_startfromlevel|true|androidhls|hls_minbufferlength|flash|primary||360|height|thumbnails|kind|vtt|tracks|mp4|cfuqjgl5qy2qedz7nisbvsr5isr2ahy7drm44kp2c54ftwl6fbdo7kerhv2a|hls|type|embed|m3u8|sources|swf|nplayer|static|provider|jpg|1ypp4jd4p22x|01203|01|image|playlist|setup|vplayer'.split('|')))
//http://vidzi.tv/9sni3taa7zmh-3e21e62ede7116c77a7215a57bbbe505.m3u8?embed=
//Referer:http://static.vidzi.tv/nplayer/jwplayer.flash.swf

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

	if(ostrstr(tmpstr, "Video is processing now.") != NULL)
	{
		textbox(_("Message"), _("Video is processing now. Conversion stage...") , _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 1200, 200, 0, 0);
		goto end;
	}

//	streamlink = string_resub("[{l:\"", "\"", tmpstr, 0);		
	streamlink = oregex(".*(http://.*v.mp4).*", tmpstr);

	titheklog(debuglevel, "/tmp/vidzi4_streamlink", NULL, NULL, NULL, streamlink);

	free(tmpstr); tmpstr = NULL;

end:

	free(tmplink) , tmplink = NULL;
	free(tmpstr); tmpstr = NULL;
	free(url); url = NULL;

	return streamlink;
}

#endif
