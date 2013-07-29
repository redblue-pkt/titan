#ifndef XVIDEOS_H
#define XVIDEOS_H

char* xvideos(char* link)
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
	
	writesys("/var/usr/local/share/titan/plugins/tithek/xvideos1_tmpstr", tmpstr, 0);
	
	htmldecode(tmpstr, tmpstr);
	writesys("/var/usr/local/share/titan/plugins/tithek/xvideos2_tmpstr", tmpstr, 0);

	
	if(getconfigint("debuglevel", NULL) == 99)
		writesys("/tmp/xvideos1_tmpstr", tmpstr, 0);

	if(tmpstr != NULL)
	{
		streamurl = string_resub("flv_url=","&amp;",tmpstr,0);
	}

	free(tmpstr); tmpstr = NULL;
	free(ip), ip = NULL;

	if(getconfigint("debuglevel", NULL) == 99)
		writesys("/tmp/xvideos2_streamurl", streamurl, 0);

// segfault munmap_chunk(): invalid pointer
//	free(pos), pos = NULL;
//	free(path), path = NULL;

	debug(99, "streamurl: %s", streamurl);	
	return streamurl;
}

#endif