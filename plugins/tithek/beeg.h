#ifndef BEEG_H
#define BEEG_H

char* beeg(char* link)
{
	debug(99, "link %s", link);
	int debuglevel = getconfigint("debuglevel", NULL);
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
	titheklog(debuglevel, "/tmp/beeg1_tmpstr", NULL, tmpstr);

	if(tmpstr != NULL)
	{
		streamurl = string_resub("'file': '","',",tmpstr,0);
	}

	free(tmpstr); tmpstr = NULL;
	free(ip), ip = NULL;

  titheklog(debuglevel, "/tmp/beeg2_streamurl", NULL, streamurl);

// segfault munmap_chunk(): invalid pointer
//	free(pos), pos = NULL;
//	free(path), path = NULL;

	debug(99, "streamurl: %s", streamurl);	
	return streamurl;
}

#endif