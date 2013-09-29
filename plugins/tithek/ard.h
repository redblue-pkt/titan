#ifndef ARD_H
#define ARD_H

char* ard(char* link)
{
	debug(99, "link %s", link);
	char* ip = NULL, *pos = NULL, *path = NULL, *streamurl = NULL, *tmpstr = NULL;
	
	ip = string_replace("http://", "", (char*)link, 0);

	if(ip != NULL)
		pos = strchr(ip, '/');
	if(pos != NULL)
	{
		pos[0] = '\0';
		path = pos + 1;
	}

	tmpstr = gethttp(ip, path, 80, NULL, NULL, 10000, NULL, 0);
	if(getconfigint("debuglevel", NULL) == 99)
		writesys("/tmp/ard1_tmpstr", tmpstr, 0);

	if(tmpstr != NULL)
	{
/*
		streamurl = string_resub("'file': '","',",tmpstr,0);

mediaCollection.addMediaStream(0, 0, "", "http://http-stream.rbb-online.de/rbb/abendschau/abendschau_20130923_wahl_s_16_9_256x144.mp4", "akamai");
mediaCollection.addMediaStream(0, 1, "rtmp://ondemand.rbb-online.de/ondemand/", "mp4:rbb/abendschau/abendschau_20130923_wahl_m_16_9_512x288.mp4", "akamai");
mediaCollection.addMediaStream(1, 0, "", "http://http-stream.rbb-online.de/rbb/abendschau/abendschau_20130923_wahl_s_16_9_256x144.mp4", "akamai");
mediaCollection.addMediaStream(1, 1, "", "http://http-stream.rbb-online.de/rbb/abendschau/abendschau_20130923_wahl_m_16_9_512x288.mp4", "akamai");

*/

		streamurl = oregex(".*mediaCollection.addMediaStream.*(http://.*.mp4).*", tmpstr);
	}

	free(tmpstr); tmpstr = NULL;
	free(ip), ip = NULL;

	if(getconfigint("debuglevel", NULL) == 99)
		writesys("/tmp/ard2_streamurl", streamurl, 0);

// segfault munmap_chunk(): invalid pointer
//	free(pos), pos = NULL;
//	free(path), path = NULL;

	debug(99, "streamurl: %s", streamurl);	
	return streamurl;
}

#endif