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
	char* ip = NULL, *pos = NULL, *path = NULL, *tmppath = NULL, *streamurl = NULL, *title = NULL, *tmpstr = NULL, *murl = NULL, *sig = NULL, *pic = NULL;

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
	tmplink = string_replace_all("/youtu.be", "/www.youtube.com", tmplink, 1);

/* spox.com

http://www.spox.com/de/sport/ussport/nba/live-stream/1310/miami-heat-washington-wizards-frank-buschmann.html
grep code:
<div id="spxliveplayer"><iframe frameborder="0" width="640px" height="360px" scrolling="no" src="http://www.youtube.com/embed/SjMEn0d6ByU" id="spxliveiframe" ></iframe></div>

and get to youtube

// ssl workaround
		unlink("/tmp/.youtube.cache");
		char* cmd = NULL;
		cmd = ostrcat("wget --no-check-certificate \"http://", ip, 0, 0);
		cmd = ostrcat(cmd, "/", 1, 0);
		cmd = ostrcat(cmd, tmppath, 1, 0);
		cmd = ostrcat(cmd, "\" -O /tmp/.youtube.cache", 1, 0);
		debug(99, "cmd: %s", cmd);
		system(cmd);
		free(cmd), cmd = NULL;
		tmpstr = command("cat /tmp/.solar.cache");
		debug(99, "tmpstr: %s", tmpstr);
// ssl workaround end
//	tmpstr = gethttp(ip, tmppath, 80, NULL, NULL, 10000, NULL, 0);
*/

	tmpstr = gethttps(link, NULL, NULL, NULL, NULL, NULL, 1);
	writesys("/var/usr/local/share/titan/plugins/tithek/youtube_tmpstr", tmpstr, 0);

//13:50:32 T:2532  NOTICE: 'GET /youtube/v3/channels?part=snippet%2CcontentDetails%2CbrandingSettings&id=UC_fV2pzmw2SujuQgIm6YcbQ%2CUCWp1qqITrK2hQBpFTmE9uVg%2CUCClNRixXlagwAd--5MwJKCw%2CUCz6Lv-YT2Fjhi3GyXdXt4Vw%2CUC0g5OdpoCTfpHUelY9PLWSQ%2CUCXJDX1KK6t121Z9FLhu5o2A%2CUCboe4JAAUOI-OzKBj_pkYFg%2CUCT-_4GqC-yLY1xtTHhwY0hA%2CUCsW36751Gy-EAbHQwe9WBNw%2CUC1fIyfhQtm1fSljyKBf2uKA%2CUCy-dXLczRuq-ZtuWmUo52PA%2CUCq-Fj5jknLsUf-MWSy4_brA%2CUCe2r4-wNZjYmQhHrYAR2WUA%2CUC-v9ZU8TO8chuAOsh3UO-gQ%2CUCa6vGFO9ty8v5KZJXQxdhaw%2CUC5zGJZpxeZPFcds5gFcDE7Q%2CUC0y2acrGZ3NH-3ycSGYuIPQ%2CUCqyYNNx60mBgvNKKR5VhUCA%2CUCCgDVqiPU10shxzmwkMwJ6A%2CUCFeUyPY6W8qX8w2o6oSiRmw%2CUCl0kP-Cfe-GGic7Ilnk-u_Q%2CUCXkQVG6OdyB2ct4xOOZjmPQ%2CUCu17Sme-KE87ca9OTzP0p7g%2CUC2nZMhZ2qG5-xpqb440WLYg%2CUCYdIDs5a3Pt-o4SiD-ih24g&key=AIzaSyBAdxZCHbeJwnQ7dDZQJNfcaF46MdqJ24E HTTP/1.1\r\nHost: www.googleapis.com\r\nUser-Agent: Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/39.0.2171.36 Safari/537.36\r\nConnection: close\r\nAccept-Encoding: gzip, deflate\r\n\r\n'

//	tmpstr = gethttps(link, NULL, NULL, NULL, NULL, NULL, 1);
//	writesys("/var/usr/local/share/titan/plugins/tithek/youtube_tmpstr", tmpstr, 0);

//http://r7---sn-h0j7snel.googlevideo.com/videoplayback?source=youtube&gcr=de&ratebypass=yes&upn=32kjep7VPuc&initcwndbps=688750&expire=1443472674&sver=3&key=yt6&mn=sn-h0j7snel&mm=31&ip=95.91.6.76&sparams=dur%2Cgcr%2Cid%2Cinitcwndbps%2Cip%2Cipbits%2Citag%2Clmt%2Cmime%2Cmm%2Cmn%2Cms%2Cmv%2Cpl%2Cratebypass%2Csource%2Cupn%2Cexpire&pl=17&mv=m&mt=1443450913&ms=au&mime=video%2Fmp4&dur=249.173&id=o-APp6mA0M_837ohVW8yheCE2x2Ajd9KZ5GhoFWpMykdLR&ipbits=0&itag=22&fexp=9407155%2C9408508%2C9408710%2C9409069%2C9415365%2C9415485%2C9416023%2C9416729%2C9418448%2C9419484%2C9420215%2C9420348%2C9420818%2C9420928%2C9421013&lmt=1429610403267549&signature=

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
			tmpstr = ostrcat(tmpstr, "&", 1, 0);
//			tmpstr = string_resub("url_encoded_fmt_stream_map=","&",tmpstr,0);

			if(tmpstr != NULL)
			{
				htmldecode(tmpstr,tmpstr);
				htmldecode(tmpstr,tmpstr);
				htmldecode(tmpstr,tmpstr);
				htmldecode(tmpstr,tmpstr);
				htmldecode(tmpstr,tmpstr);
				tmpstr = string_replace_all("\n", " ", tmpstr, 1);
				tmpstr = string_replace_all("&url=", "\n", tmpstr, 1);

				ret1 = strsplit(tmpstr,"\n",&count);

				for(i = 0; i < count; i++)
				{
					if(ret1[i].part != NULL && strlen(ret1[i].part) > 1)
					{
						debug(99, "\nRound %d started, processing = %s",i,ret1[i].part);
						ret1[i].part = ostrcat(ret1[i].part,"&",0,0);
						
						murl = string_resub("url=", "&", ret1[i].part, 0);
						sig = string_resub("sig=", "&", ret1[i].part, 0);
						if(sig == NULL)
							sig = string_resub("s=", "&", ret1[i].part, 0);

						streamurl = ostrcat(ret1[i].part, NULL, 0, 0);
						streamurl = string_replace_all(",itag=", "\0", streamurl, 1);

						if(streamurl != NULL)
						{
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
//		tmpstr = gethttp(ip, path, 80, NULL, NULL, 10000, NULL, 0);
		writesys("/var/usr/local/share/titan/plugins/tithek/tmpstr1", tmpstr, 0);
printf("ALLE\n");

		tmpstr = stringreplacechar(tmpstr, '\n', ' ');

		tmpstr = string_replace_all("\"etag\":", "\n\"etag\":", tmpstr, 1);

		tmpstr = string_replace_all("media:thumbnail", "\nthumbnail", tmpstr, 1);
		writesys("/var/usr/local/share/titan/plugins/tithek/tmpstr2", tmpstr, 0);
/*

//12:39:28 T:11340  NOTICE: 'GET /youtube/v3/videos?part=snippet%2CcontentDetails&id=eOxPED0TunI%2CsojYfBmdvlE%2Cj-ocHL0eudY%2CHQBia-CfqbE%2CmHLwXQoG4CU%2Cw5VFOKKAbQQ%2CvnoQ5lkBVhU%2CeocCPDxKq1o%2ChSjIz8oQuko%2CJDKGWaCglRM%2CbZsqdTrr1eM%2CyTCDVfMz15M%2CHq2DINl2MxI%2CnB2Hsh1XqyA%2C41aGCrXM20E%2Cuc8OpUnUNNU%2Cv4UkD7U88NQ%2CAlMa52Xkoj0%2CocDlOD1Hw9k%2CG6ZLZQGydPc%2CxXvBkwihOE8%2CNJWIbIe0N90%2CwCqs2uJWwZs%2CPl-TCZSRhy4%2C0TDTEEXZtLs%2CXIId4uESVeA%2ClHSl-yz5FqA%2CRzriJpYiGjw%2CjT5RwB5ML30%2C5r9vdqAFCY4%2CzsmUOdmm02A%2C8WmZW7JB3GY%2C3NrNWISh5CE%2CeDtMxamBxI8%2CBR4yQFZK9YM%2CRAcNZo7-f0g%2CXjVNlG5cZyQ%2CasaCQOZpqUQ%2CWWYLM9opelw%2CqOfkpu6749w%2CBJLWNWDqK-k%2COpQFFLBMEPI%2Cm-6ir-gFpbA%2C3stsDXki__U%2C57KVfhWcEk0%2CFJfFZqTlWrQ%2CSNjFTxaeWug%2CumM9Ls0SaiU%2Cw7JRWHCKkIY%2C6lyq05OWuVM&key=AIzaSyBAdxZCHbeJwnQ7dDZQJNfcaF46MdqJ24E HTTP/1.1\r\nHost: www.googleapis.com\r\nUser-Agent: Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/39.0.2171.36 Safari/537.36\r\nConnection: close\r\nAccept-Encoding: gzip, deflate\r\n\r\n'
/youtube/v3/videos?part=snippet%2CcontentDetails
&id=
eOxPED0TunI
%2
CsojYfBmdvlE
%2
Cj-ocHL0eudY
%2
CHQBia-CfqbE
%2
CmHLwXQoG4CU
%2
Cw5VFOKKAbQQ%2CvnoQ5lkBVhU%2CeocCPDxKq1o%2ChSjIz8oQuko%2CJDKGWaCglRM%2CbZsqdTrr1eM%2CyTCDVfMz15M%2CHq2DINl2MxI%2CnB2Hsh1XqyA
%2C41aGCrXM20E%2Cuc8OpUnUNNU%2Cv4UkD7U88NQ%2CAlMa52Xkoj0%2CocDlOD1Hw9k%2CG6ZLZQGydPc%2CxXvBkwihOE8%2CNJWIbIe0N90%2CwCqs2uJWwZs%2CPl-TCZSRhy4
%2C0TDTEEXZtLs%2CXIId4uESVeA%2ClHSl-yz5FqA%2CRzriJpYiGjw%2CjT5RwB5ML30%2C5r9vdqAFCY4%2CzsmUOdmm02A%2C8WmZW7JB3GY%2C3NrNWISh5CE%2CeDtMxamBxI8
%2CBR4yQFZK9YM%2CRAcNZo7-f0g%2CXjVNlG5cZyQ%2CasaCQOZpqUQ%2CWWYLM9opelw%2CqOfkpu6749w%2CBJLWNWDqK-k
%2COpQFFLBMEPI%2Cm-6ir-gFpbA
%2C3stsDXki__U%2C57KVfhWcEk0%2CFJfFZqTlWrQ%2CSNjFTxaeWug%2CumM9Ls0SaiU%2Cw7JRWHCKkIY%2C6lyq05OWuVM

&key=AIzaSyBAdxZCHbeJwnQ7dDZQJNfcaF46MdqJ24E HTTP/1.1\r\nHost: www.googleapis.com\r\nUser-Agent: Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/39.0.2171.36 Safari/537.36\r\nConnection: close\r\nAccept-Encoding: gzip, deflate\r\n\r\n'

"title": "
"videoId": "
"url": "
*/
		int count = 0;
		int incount = 0;
		int i = 0;
		struct splitstr* ret1 = NULL;
		ret1 = strsplit(tmpstr, "\n", &count);
//part="etag": "\"jOXstHOM20qemPbHbyzf7ztZ7rI/zJExHKkQXAGmI7-mKIkjbmndgJ0\"",    "id": {     "kind": "youtube#video",     "videoId": "57KVfhWcEk0"    },    "snippet": {     "publishedAt": "2015-09-19T02:06:39.000Z",     "channelId": "UCwehWhlhepFE8mFdlC1ebAw",     "title": "Pink - Mindless Self Indulgence FULL ALBUM",     "description": "JIMMY SAYS TO BUY THE FUCKING ALBUM Tracklist: 01. Personal Jesus 02. This Hurts 03. Be Like Superman 04. Memory Of Heaven 05. Vanity 06. Married ...",     "thumbnails": {      "default": {       "url": "https://i.ytimg.com/vi/57KVfhWcEk0/default.jpg"      },      "medium": {       "url": "https://i.ytimg.com/vi/57KVfhWcEk0/mqdefault.jpg"      },      "high": {       "url": "https://i.ytimg.com/vi/57KVfhWcEk0/hqdefault.jpg"      }     },     "channelTitle": "",     "liveBroadcastContent": "none"    }   }  ] }

		if(ret1 != NULL)
		{
			int max = count;
			for(i = 0; i < max; i++)
			{
				debug(99, "ret1[i].part=%s", ret1[i].part);
				if(ostrstr(ret1[i].part, "videoId") != NULL)
				{
//					pic = oregex(".*thumbnail url=\'(http://i.ytimg.com/vi/.*/.*.jpg).*media:title.*", ret1[i].part);
//					id = oregex(".*thumbnail url=\'http://i.ytimg.com/vi/(.*)/.*.jpg.*media:title.*", ret1[i].part);
//					title = oregex(".*<media:title type='plain'>(.*)</media:title>.*", ret1[i].part);

					pic = string_resub("\"url\": \"", "\"", ret1[i].part, 0);
					id = string_resub("\"videoId\": \"", "\"", ret1[i].part, 0);
					title = string_resub("\"title\": \"", "\"", ret1[i].part, 0);

					debug(99, "title=%s", title);
					debug(99, "id=%s", id);
					debug(99, "pic=%s", pic);

					int rcret = waitrc(NULL, 10, 0);
					if(rcret == getrcconfigint("rcexit", NULL)) break;
//www.googleapis.com/youtube/v3/videos?part=snippet%2CcontentDetails&id=eOxPED0TunI%2CsojYfBmdvlE%2Cj-ocHL0eudY%2CHQBia-CfqbE%2CmHLwXQoG4CU%2Cw5VFOKKAbQQ%2CvnoQ5lkBVhU%2CeocCPDxKq1o%2ChSjIz8oQuko%2CJDKGWaCglRM%2CbZsqdTrr1eM%2CyTCDVfMz15M%2CHq2DINl2MxI%2CnB2Hsh1XqyA%2C41aGCrXM20E%2Cuc8OpUnUNNU%2Cv4UkD7U88NQ%2CAlMa52Xkoj0%2CocDlOD1Hw9k%2CG6ZLZQGydPc%2CxXvBkwihOE8%2CNJWIbIe0N90%2CwCqs2uJWwZs%2CPl-TCZSRhy4%2C0TDTEEXZtLs%2CXIId4uESVeA%2ClHSl-yz5FqA%2CRzriJpYiGjw%2CjT5RwB5ML30%2C5r9vdqAFCY4%2CzsmUOdmm02A%2C8WmZW7JB3GY%2C3NrNWISh5CE%2CeDtMxamBxI8%2CBR4yQFZK9YM%2CRAcNZo7-f0g%2CXjVNlG5cZyQ%2CasaCQOZpqUQ%2CWWYLM9opelw%2CqOfkpu6749w%2CBJLWNWDqK-k%2COpQFFLBMEPI%2Cm-6ir-gFpbA%2C3stsDXki__U%2C57KVfhWcEk0%2CFJfFZqTlWrQ%2CSNjFTxaeWug%2CumM9Ls0SaiU%2Cw7JRWHCKkIY%2C6lyq05OWuVM&key=AIzaSyBAdxZCHbeJwnQ7dDZQJNfcaF46MdqJ24E
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

//www.youtube.com/get_video_info?el=leanback&cplayer=UNIPLAYER&cos=Windows&height=1080&cbr=Chrome&hl=en_US&cver=4&ps=leanback&c=TVHTML5&video_id=yTCDVfMz15M&cbrver=40.0.2214.115&width=1920&cosver=6.1&ssl_stream=1

//						line = ostrcat(line, "#https://www.googleapis.com/youtube/v3/videos?part=snippet%2CcontentDetails&id=", 1, 0);
//						line = ostrcat(line, id, 1, 0);
//						line = ostrcat(line, "&key=AIzaSyBAdxZCHbeJwnQ7dDZQJNfcaF46MdqJ24E", 1, 0);
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

		tmpstr = gethttp("atemio.dyndns.tv", "/mediathek/youtube/streams/youtube.all-sorted.list", 80, NULL, HTTPAUTH, 5000, NULL, 0);

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
				line = string_replace_all("http://atemio.dyndns.tv/", "http://imageshack.us/md/up/grd/", line, 1);
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