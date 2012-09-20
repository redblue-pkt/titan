#ifndef MYVIDEO_H
#define MYVIDEO_H

// flag 1 = getstreamurl

char* myvideo(char* link, char* url, char* name, int flag)
{
	debug(99, "link(%d): %s", flag, link);
	char* ip = NULL, *pos = NULL, *path = NULL, *pageUrl = NULL, *playpath = NULL, *video_id = NULL, *source = NULL, *streamurl = NULL, *tmpstr_uni = NULL, *b64 = NULL, *key = NULL, *newurl = NULL;

	if(flag == 1)
	{
		int count = 0;
		struct splitstr* ret1 = NULL;
		ret1 = strsplit(link, ";", &count);
		if(ret1 != NULL && count >= 4)
		{
			link = ostrcat(ret1[0].part, NULL, 0, 0);
			pageUrl = ostrcat(pageUrl, ret1[1].part, 1, 0);
			playpath = ostrcat(playpath, ret1[2].part, 1, 0);
			video_id = ostrcat(video_id, ret1[3].part, 1, 0);									
		}
		free(ret1), ret1 = NULL;
	}	

	ip = string_replace("http://", "", (char*)link, 0);

	if(ip != NULL)
		pos = strchr(ip, '/');
	if(pos != NULL)
	{
		pos[0] = '\0';
		path = pos + 1;
	}

	char* tmpstr = NULL;
	tmpstr = gethttp(ip, path, 80, NULL, NULL, NULL, 0);
		
	if(flag == 1)
	{
		int count = 0;
		struct splitstr* ret1 = NULL;
		ret1 = strsplit(tmpstr, "=", &count);
		int hlen = 0;

		if(ret1 != NULL && count >= 2)
		{
			debug(99, "ret1[1].part=%s", (ret1[1]).part);
			hlen = strlen(ret1[1].part);
			tmpstr_uni = unhexlify(ret1[1].part);
		}
		free(ret1), ret1 = NULL;

		b64 = ostrcat("c8407a08b3c71ea418ec9dc662f2a56e40cbd6d5a114aa50fb1e1079e17f2b83", MDString(video_id), 0, 1);
		debug(99, "b64=%s", b64);

		key = MDString(b64);
		int slen = 0;
		int klen = 0;
		if(tmpstr_uni != NULL) slen = strlen(tmpstr_uni);
		if(key != NULL) klen = strlen(key);
		
		if(tmpstr_uni != NULL)
		{
			debug(99, "hexlen=%d", hlen);
			hlen /= 2;
			debug(99, "binlen=%d", hlen);
			debug(99, "keylen=%d", klen);
			debug(99, "b64=%s", b64);
			debug(99, "key=%s", key);

			rc4(tmpstr_uni, hlen, key, klen);

			debug(99, "encrypted=%s", tmpstr_uni);
			debug(99, "pageUrl: %s\n", pageUrl);
			debug(99, "playpath: %s\n", playpath);
			debug(99, "video_id: %s\n", video_id);
			//printf("tmpstr_uni: %s\n",tmpstr_uni);

			htmldecode(tmpstr_uni, tmpstr_uni);
	
			if(ostrstr(tmpstr_uni, "connectionurl='rtmp"))
			{
				printf("found rtmpe:// stream\n");
				source = string_resub("source='", ".flv'", tmpstr_uni, 0);

				newurl = string_resub("connectionurl='", "'", tmpstr_uni, 0);
	
				if(ostrstr(newurl, "myvideo2flash"))
				{
					printf("change to rtmpt:// stream\n");
					newurl = string_replace("rtmpe://", "rtmpt://", newurl, 1);
				}
	
				streamurl = ostrcat(newurl, NULL, 0, 0);
				streamurl = ostrcat(streamurl, " ", 1, 0);
				streamurl = ostrcat(streamurl, "tcUrl=", 1, 0);
				streamurl = ostrcat(streamurl, newurl, 1, 0);
				streamurl = ostrcat(streamurl, " swfVfy=http://is4.myvideo.de/de/player/mingR11q/ming.swf ", 1, 0);
				streamurl = ostrcat(streamurl, pageUrl, 1, 0);
				streamurl = ostrcat(streamurl, " ", 1, 0);
				streamurl = ostrcat(streamurl, "playpath=flv:", 1, 0);
				streamurl = ostrcat(streamurl, source, 1, 0);		
			}
			else
			{		
				printf("rtmpe not found, change to *.flv stream\n");
				source = string_resub("source='", "'", tmpstr_uni, 0);
	
				newurl = string_resub("path='", "'", tmpstr_uni, 0);
	
				streamurl = ostrcat(newurl, source, 0, 0);
			}
		}
	}

	free(key), key = NULL;		
	free(b64), b64 = NULL;
	free(tmpstr_uni), tmpstr_uni = NULL;		
	free(url), url = NULL;
	free(source), source = NULL;
	free(tmpstr), tmpstr = NULL;
	free(pageUrl), pageUrl = NULL;		
	free(playpath), playpath = NULL;
	free(ip), ip = NULL;
	free(video_id), video_id = NULL;
	free(newurl), newurl = NULL;
// segfault munmap_chunk(): invalid pointer
//	free(pos), pos = NULL;
//	free(path), path = NULL;

	debug(99, "streamurl2: %s", streamurl);	
	return streamurl;
}

#endif
