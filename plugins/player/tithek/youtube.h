#ifndef YOUTUBE_H
#define YOUTUBE_H

// flag 1 = getstreamurl
//http://www.youtube.com/watch?v=LAr6oAKieHk
//http://www.youtube.com/get_video_info?&video_id=m-2jBo9pVf4
char* youtube_hoster(char* link)
{
	debug(99, "link: %s", link);
	char* streamurl = NULL;

	streamurl = hoster(link);
	debug(99, "streamurl1: %s", streamurl);

	streamurl = string_replace_all("amp;", "", streamurl, 1);
	debug(99, "streamurl2: %s", streamurl);

	return streamurl;
}

char* youtube(char* link)
{
	debug(99, "link: %s", link);
	char* ip = NULL, *pos = NULL, *path = NULL, *tmppath = NULL, *streamurl = NULL, *title = NULL, *tmpstr = NULL, *murl = NULL, *sig = NULL, *pic = NULL, *tmplink = NULL;

	ip = string_replace("http://", "", (char*)link, 0);

	if(ip != NULL)
		pos = strchr(ip, '/');
	if(pos != NULL)
	{
		pos[0] = '\0';
		path = pos + 1;
	}
	tmppath = ostrcat(path, NULL, 0, 0);
	tmppath = string_replace_all("watch?v=", "get_video_info?&video_id=", tmppath, 1);

	tmplink = ostrcat(link, NULL, 0, 0);
	tmplink = string_replace_all("watch?v=", "get_video_info?&video_id=", tmplink, 1);
	tmplink = string_replace_all("/embed/", "/get_video_info?&video_id=", tmplink, 1);
	tmplink = string_replace_all("http://", "https://", tmplink, 1);

//https://www.youtube.com/get_video_info?el=leanback&cplayer=UNIPLAYER&cos=Windows&height=1080&cbr=Chrome&hl=en_US&cver=4&ps=leanback&c=TVHTML5&video_id=17NnIkv2C3k&cbrver=40.0.2214.115&width=1920&cosver=6.1&ssl_stream=1
//https://www.youtube.com/embed/17NnIkv2C3k

/* spox.com

http://www.spox.com/de/sport/ussport/nba/live-stream/1310/miami-heat-washington-wizards-frank-buschmann.html
grep code:
<div id="spxliveplayer"><iframe frameborder="0" width="640px" height="360px" scrolling="no" src="http://www.youtube.com/embed/SjMEn0d6ByU" id="spxliveiframe" ></iframe></div>

and get to youtube
*/

	tmpstr = gethttps(tmplink, NULL, NULL, NULL, NULL, NULL, 1);
	writesys("/var/usr/local/share/titan/plugins/player/tithek/youtube_tmpstr", tmpstr, 0);
	free(tmplink), tmplink = NULL;

//	if(flag == 1)
//	{
		if(ostrstr(tmpstr, "&hlsvp=") != NULL)
		{
			printf("found NBA1\n");
			streamurl = string_resub("&hlsvp=", "&", tmpstr, 0);
			string_decode(streamurl,0);
			string_decode(streamurl,0);
			string_decode(streamurl,0);
			string_decode(streamurl,0);
			string_decode(streamurl,0);
			string_decode(streamurl,0);	
		}
		else if(ostrstr(tmpstr, "status=fail&") == NULL)
		{
 			struct splitstr* ret1 = NULL;
			struct menulist* mlist = NULL, *mbox = NULL;
			int count = 0, i = 0;
			tmpstr = ostrcat(tmpstr,"&",0,0);
			tmpstr = string_resub("url_encoded_fmt_stream_map=","&",tmpstr,0);

			if(tmpstr != NULL)
			{
				ret1 = strsplit(string_decode(tmpstr,0),",",&count);

				for(i = 0; i < count; i++)
				{
					if(ret1[i].part != NULL && strlen(ret1[i].part) > 1)
					{
						debug(99, "\nRound %d started, processing = %s",i,ret1[i].part);
						ret1[i].part = ostrcat(ret1[i].part,"&",0,0);
						
						murl = string_resub("url=","&",ret1[i].part,0);
//						sig = string_resub("sig=","&",ret1[i].part,0);

//						if (sig == NULL)
//							sig = string_resub("signature=","&",ret1[i].part,0);

						
						free(streamurl), streamurl = NULL;
//						if(murl != NULL && sig != NULL)
						if(murl != NULL)
						{					
							streamurl = ostrcat(murl, NULL, 0, 0);
//							streamurl = ostrcat(streamurl, "&signature=", 1, 0);
//							streamurl = ostrcat(streamurl, sig, 1, 0);
							streamurl = string_decode(streamurl,0);
															
							if(ostrstr(ret1[i].part, "itag=85") != NULL)
							{
								title = ostrcat("MP4 520p H.264 3D", NULL, 0, 0);
								pic = ostrcat("mp4.png", NULL, 0, 0);
							}
							else if(ostrstr(ret1[i].part, "itag=84") != NULL)
							{
								title = ostrcat("MP4 720p H.264 3D", NULL, 0, 0);
								pic = ostrcat("mp4.png", NULL, 0, 0);
							}
							else if(ostrstr(ret1[i].part, "itag=83") != NULL)
							{
								title = ostrcat("MP4 240p H.264 3D", NULL, 0, 0);
								pic = ostrcat("mp4.png", NULL, 0, 0);
							}
							else if(ostrstr(ret1[i].part, "itag=82") != NULL)
							{
								title = ostrcat("MP4 360p H.264 3D", NULL, 0, 0);
								pic = ostrcat("mp4.png", NULL, 0, 0);
							}
							else if(ostrstr(ret1[i].part, "itag=38") != NULL)
							{
								title = ostrcat("MP4 3072p H.264 High", NULL, 0, 0);
								pic = ostrcat("mp4.png", NULL, 0, 0);
							}
							else if(ostrstr(ret1[i].part, "itag=37") != NULL)
							{
								title = ostrcat("MP4 1080p H.264 High", NULL, 0, 0);
								pic = ostrcat("mp4.png", NULL, 0, 0);
							}
							else if(ostrstr(ret1[i].part, "itag=22") != NULL)
							{
								title = ostrcat("MP4 720p H.264 High", NULL, 0, 0);
								pic = ostrcat("mp4.png", NULL, 0, 0);
							}
							else if(ostrstr(ret1[i].part, "itag=18") != NULL)
							{
								title = ostrcat("MP4 360p H.264 Baseline", NULL, 0, 0);												
								pic = ostrcat("mp4.png", NULL, 0, 0);
							}
							else if(ostrstr(ret1[i].part, "itag=6") != NULL)
							{
								title = ostrcat("FLV 270p Sorenson H.263", NULL, 0, 0);
								pic = ostrcat("flv.png", NULL, 0, 0);
							}
							else if(ostrstr(ret1[i].part, "itag=5") != NULL)
							{
								title = ostrcat("FLV 240p Sorenson H.263", NULL, 0, 0);
								pic = ostrcat("flv.png", NULL, 0, 0);
							}
							else if(ostrstr(ret1[i].part, "itag=35") != NULL)
							{
								title = ostrcat("FLV 480p H.264 Main", NULL, 0, 0);
								pic = ostrcat("flv.png", NULL, 0, 0);
							}
							else if(ostrstr(ret1[i].part, "itag=34") != NULL)
							{
								title = ostrcat("FLV 360p H.264 Main", NULL, 0, 0);														
								pic = ostrcat("3gp.png", NULL, 0, 0);
							}
							else if(ostrstr(ret1[i].part, "itag=36") != NULL)
							{
								title = ostrcat("3GP 240p MPEG-4 Visual Simple", NULL, 0, 0);
								pic = ostrcat("3gp.png", NULL, 0, 0);
							}
							else if(ostrstr(ret1[i].part, "itag=17") != NULL)
							{
								title = ostrcat("3GP 144p MPEG-4 Visual Simple", NULL, 0, 0);
								pic = ostrcat("3gp.png", NULL, 0, 0);
							}
														
							if(title == NULL)
							{
								title = ostrcat(_("unknown"), NULL, 0, 0);
								debug(99, "(%d) title: %s streamurl: %s\n", i, title, streamurl);
								debug(99, "(%d) ret1 %s\n",i, (ret1[i]).part);
							}
							else
							{
								debug(99, "(%d) title: %s streamurl: %s\n", i, title, streamurl);																									
								addmenulist(&mlist, title, streamurl, pic, 0, 0);
							}
							free(title), title = NULL;
							free(pic), pic = NULL;
							free(streamurl), streamurl = NULL;
						  
						}
						free(murl), murl = NULL;
						free(sig), sig = NULL;
					}

					// TODO handle error while parsing
	
				}
				free(ret1), ret1 = NULL;

				if(mlist != NULL){
					mbox = menulistbox(mlist, NULL, _("Stream Menu"), _("Choose your Streaming Format from the following list"), NULL, NULL, 1, 0);
					if(mbox != NULL){
					    free(streamurl), streamurl = NULL;
		    
					    debug(99, "mbox->name %s", mbox->name);
					    debug(99, "mbox->text %s", mbox->text);
					    streamurl = ostrcat(mbox->text, NULL, 0, 0);
		    
					}
				}
			}
		}
		else
		{	
			tmpstr = string_resub("&reason=", "&errordetail", tmpstr, 1);
			tmpstr = string_replace_all("+", " ", tmpstr, 1);
			tmpstr = string_replace_all(", ", "\n", tmpstr, 1);
			tmpstr = string_replace("wiedergegeben", "\nwiedergegeben ", tmpstr, 1);
			tmpstr = string_replace("<br/><u><a href='", "\n\n", tmpstr, 1);
			tmpstr = string_replace("' target='_blank'>", "\n", tmpstr, 1);
			tmpstr = string_replace("</a></u>", "\n", tmpstr, 1);

			tmpstr = strstrip(tmpstr);
			if(tmpstr != NULL && strlen(tmpstr) > 0)
				textbox(_("Message"), tmpstr, _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 1200, 400, 0, 0);
		}
//	}

	free(tmpstr); tmpstr = NULL;
	free(title); title = NULL;
	free(ip), ip = NULL;

// segfault munmap_chunk(): invalid pointer
//	free(pos), pos = NULL;
	free(tmppath), tmppath = NULL;

	debug(99, "streamurl2: %s", streamurl);	
	return streamurl;
}

int youtube_search(struct skin* grid, struct skin* listbox, struct skin* countlabel, struct skin* load, char* link, char* title, char* searchstr)
{
	int ret = 1;

	if(listbox == NULL || listbox->select == NULL || listbox->select->handle == NULL)
		return ret;

	char* search = NULL;
	if(searchstr == NULL)
		search = textinputhist(_("Search"), " ", "searchhist");
	else
		search = textinputhist(_("Search"), searchstr, "searchhist");

	if(search != NULL)
	{
		drawscreen(load, 0, 0);
		search = stringreplacechar(search, ' ', '+');
		char* id = NULL;
		char* line = NULL;
		char* pic = NULL;
		char* title = NULL;
		char* menu = NULL;	
		char* ip = ostrcat("gdata.youtube.com", NULL, 0, 0);
		char* path = ostrcat("feeds/api/videos?q=", search, 0, 0);
		if(((struct tithek*)listbox->select->handle)->flag == 9)
			path = ostrcat(path, "&max-results=10", 1, 0);
		else if(((struct tithek*)listbox->select->handle)->flag == 10)
			path = ostrcat(path, "&max-results=25", 1, 0);
		else if(((struct tithek*)listbox->select->handle)->flag == 11)
			path = ostrcat(path, "&max-results=50", 1, 0);


		char* url = NULL;
		url = ostrcat("https://www.googleapis.com/youtube/v3/search?q=", search, 0, 0);
		url = ostrcat(url, "&regionCode=US&part=snippet&hl=en_US&key=AIzaSyBAdxZCHbeJwnQ7dDZQJNfcaF46MdqJ24E&type=video&maxResults=50", 1, 0);

//12:39:28 T:11340  NOTICE: 'GET /youtube/v3/search?q=pink&regionCode=US&part=snippet&hl=en_US&key=AIzaSyBAdxZCHbeJwnQ7dDZQJNfcaF46MdqJ24E&type=video&maxResults=50 HTTP/1.1\r\nHost: www.googleapis.com\r\nUser-Agent: Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/39.0.2171.36 Safari/537.36\r\nConnection: close\r\nAccept-Encoding: gzip, deflate\r\n\r\n'

		char* tmpstr = NULL;
		tmpstr = gethttps(url, NULL, NULL, NULL, NULL, NULL, 1);		
		writesys("/var/usr/local/share/titan/plugins/player/tithek/tmpstr1", tmpstr, 0);

		tmpstr = stringreplacechar(tmpstr, '\n', ' ');

		tmpstr = string_replace_all("\"etag\":", "\n\"etag\":", tmpstr, 1);

		tmpstr = string_replace_all("media:thumbnail", "\nthumbnail", tmpstr, 1);
		writesys("/var/usr/local/share/titan/plugins/player/tithek/tmpstr2", tmpstr, 0);

		int count = 0;
		int incount = 0;
		int i = 0;
		struct splitstr* ret1 = NULL;
		ret1 = strsplit(tmpstr, "\n", &count);

		if(ret1 != NULL)
		{
			int max = count;
			for(i = 0; i < max; i++)
			{
				debug(99, "ret1[i].part=%s", ret1[i].part);
				if(ostrstr(ret1[i].part, "videoId") != NULL)
				{
					pic = string_resub("\"url\": \"", "\"", ret1[i].part, 0);
					id = string_resub("\"videoId\": \"", "\"", ret1[i].part, 0);
					title = string_resub("\"title\": \"", "\"", ret1[i].part, 0);

					debug(99, "title=%s", title);
					debug(99, "id=%s", id);
					debug(99, "pic=%s", pic);

					int rcret = waitrc(NULL, 10, 0);
					if(rcret == getrcconfigint("rcexit", NULL)) break;

					if(id != NULL)
					{
						incount += 1;
						ip = ostrcat("www.youtube.com", NULL, 0, 0);
						path = ostrcat("watch?v=", id, 0, 0);

						line = ostrcat(line, title, 1, 0);
						line = ostrcat(line, "#http://www.youtube.com/get_video_info?&video_id=", 1, 0);
						line = ostrcat(line, id, 1, 0);
//						line = ostrcat(line, "#http://www.youtube.com/get_video_info?el=leanback&cplayer=UNIPLAYER&cos=Windows&height=1080&cbr=Chrome&hl=en_US&cver=4&ps=leanback&c=TVHTML5&video_id=", 1, 0);
//						line = ostrcat(line, id, 1, 0);
//						line = ostrcat(line, "&cbrver=40.0.2214.115&width=1920&cosver=6.1&ssl_stream=1", 1, 0);
						line = ostrcat(line, "#", 1, 0);
						line = ostrcat(line, pic, 1, 0);
						line = ostrcat(line, "#youtube_search_", 1, 0);
						line = ostrcat(line, oitoa(incount + time(NULL)), 1, 0);
						line = ostrcat(line, ".jpg#YouTube - Search#4\n", 1, 0);
						free(ip), ip = NULL;
						free(path), path = NULL;
						free(title), title = NULL;
					}
				}
			}
			free(ret1), ret1 = NULL;

			if(line != NULL)
			{
				menu = ostrcat("/tmp/tithek/youtube.search.list", NULL, 0, 0);
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

int youtube_search_local(struct skin* grid, struct skin* listbox, struct skin* countlabel, struct skin* load, char* link, char* title, char* searchstr, int flag)
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

		tmpstr = gethttp("openaaf.dyndns.tv", "/mediathek/youtube/streams/youtube.all-sorted.list", 80, NULL, HTTPAUTH, 5000, NULL, 0);

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
				menu = ostrcat("/tmp/tithek/youtube.search.list", NULL, 0, 0);
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