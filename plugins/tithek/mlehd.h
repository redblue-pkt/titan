#ifndef MLEHD_H
#define MLEHD_H

char* mlehd(char* link)
{
	debug(99, "link %s", link);
	int debuglevel = getconfigint("debuglevel", NULL);
	char* ip = NULL, *pos = NULL, *path = NULL, *streamurl = NULL, *tmpstr = NULL, *tmppath = NULL;
	
	ip = string_replace("http://", "", (char*)link, 0);

	if(ip != NULL)
		pos = strchr(ip, '/');
	if(pos != NULL)
	{
		pos[0] = '\0';
		path = pos + 1;
	}

	tmpstr = gethttp(ip, path, 80, NULL, NULL, 10000, NULL, 0);
	tmppath = string_resub("file: '", "'", tmpstr, 0);
	debug(99, "tmppath: %s", tmppath);
	free(tmpstr), tmpstr = NULL;

	if(tmppath == NULL) return streamurl;

	tmppath = string_replace("http://www.mle-hd.se/", "", tmppath, 1);
	debug(99, "tmppath: %s", tmppath);	
	tmpstr = gethttp(ip, tmppath, 80, NULL, NULL, 10000, NULL, 0);

	if(tmpstr != NULL)
	{
		streamurl = string_resub("<location>", "</location>", tmpstr, 0);
	}

	free(tmpstr); tmpstr = NULL;
	free(ip), ip = NULL;
	free(tmppath), tmppath = NULL;

  titheklog(debuglevel, "/tmp/mlehd2_streamurl", NULL, streamurl);

// segfault munmap_chunk(): invalid pointer
//	free(pos), pos = NULL;
//	free(path), path = NULL;

	debug(99, "streamurl: %s", streamurl);	
	return streamurl;
}

#endif