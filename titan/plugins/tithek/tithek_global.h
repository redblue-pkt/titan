#ifndef TITHEK_GLOBAL_H
#define TITHEK_GLOBAL

#include <curl/curl.h>

struct MemoryStruct {
  char *memory;
  size_t size;
};

static size_t
WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
  size_t realsize = size * nmemb;
  struct MemoryStruct *mem = (struct MemoryStruct *)userp;
 
  mem->memory = realloc(mem->memory, mem->size + realsize + 1);
  if(mem->memory == NULL) {
    /* out of memory! */ 
    printf("not enough memory (realloc returned NULL)\n");
    return 0;
  }
 
  memcpy(&(mem->memory[mem->size]), contents, realsize);
  mem->size += realsize;
  mem->memory[mem->size] = 0;
 
  return realsize;
}

char* gethttps(char* url, char* data)
{
	debug(99, "url: %s", url);

	int debuglevel = getconfigint("debuglevel", NULL);
	
	CURL *curl_handle;
	CURLcode res;
	
	struct MemoryStruct chunk;
	
	chunk.memory = malloc(1);  /* will be grown as needed by the realloc above */ 
	chunk.size = 0;    /* no data at this point */
	
	curl_global_init(CURL_GLOBAL_ALL);
	
	/* init the curl session */ 
	curl_handle = curl_easy_init();
	if(curl_handle)
	{
		/* specify URL to get */
		curl_easy_setopt(curl_handle, CURLOPT_URL, url);
		if(data == NULL)
			curl_easy_setopt(curl_handle, CURLOPT_HTTPGET, 1L);
		else
			curl_easy_setopt(curl_handle, CURLOPT_POSTFIELDS, data);
		curl_easy_setopt(curl_handle, CURLOPT_HEADER, 1L);
		curl_easy_setopt(curl_handle, CURLOPT_CONNECTTIMEOUT, 5);
		curl_easy_setopt(curl_handle, CURLOPT_TIMEOUT, 20);
		/* send all data to this function  */
		curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
		/* we pass our 'chunk' struct to the callback function */
		curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void *)&chunk);
		/* some servers don't like requests that are made without a user-agent field, so we provide one */
		curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, "libcurl-agent/1.0");
		curl_easy_setopt(curl_handle, CURLOPT_SSL_VERIFYPEER, 0L);
		curl_easy_setopt(curl_handle, CURLOPT_SSL_VERIFYHOST, 0L);
		if(debuglevel == 99)
			curl_easy_setopt(curl_handle, CURLOPT_VERBOSE, 1);
		curl_easy_setopt(curl_handle, CURLOPT_COOKIEFILE, "/var/usr/local/share/titan/plugins/tithek/cookie.txt");
		curl_easy_setopt(curl_handle, CURLOPT_COOKIEJAR, "/var/usr/local/share/titan/plugins/tithek/cookie.txt");
		curl_easy_setopt(curl_handle, CURLOPT_FOLLOWLOCATION, 1L);
		curl_easy_setopt(curl_handle, CURLOPT_AUTOREFERER, 1L);

		/* get it! */
		res = curl_easy_perform(curl_handle);
		/* check for errors */
		if(res != CURLE_OK)
		{
			err("failed: %s", curl_easy_strerror(res));
			printf("curl error\n");
		}
		else
		{
			/*
			 * Now, our chunk.memory points to a memory block that is chunk.size
			 * bytes big and contains the remote file.
			 *
			 * Do something nice with it!
			 */
			printf("%lu bytes retrieved\n", (long)chunk.size);
		}
		
		/* cleanup curl stuff */
		curl_easy_cleanup(curl_handle);
	}
	char* tmpstr = NULL;
	tmpstr = ostrcat(chunk.memory, NULL, 0, 0);
  	free(chunk.memory);
  	/* we're done with libcurl, so clean it up */
   	curl_global_cleanup();

	return tmpstr;
}

char* getfilekey(char* w, char* i, char* s, char* e)
{
	char* ret = NULL;

	if(w == NULL || i == NULL || s == NULL || e == NULL)
		return NULL;

	int a = 0, b = 0, c = 0;
	int a1 = 0, b1 = 0;

	int lw = strlen(w);
	int li = strlen(i);
	int ls = strlen(s);
	
	if(lw < 5 || li < 5 || ls < 5)
		return NULL;

	char ca[lw + li + ls - 14];
	char cb[16];

	ca[lw + li + ls - 15] = '\0';
	cb[15] = '\0';
	
	while(1)
	{
		if(a < 5)
		{
			cb[b1] = w[a];
			b1++;
		}
		else if(a < lw)
		{
			ca[a1] = w[a];
			a1++;
		}
		a++;
		
		if(b < 5)
		{
			cb[b1] = i[b];
			b1++;
		}
		else if(b < li)
		{
			ca[a1] = i[b];
			a1++;
		}
		b++;
		
		if(c < 5)
		{
			cb[b1] = s[c];
			b1++;
		}
		else if(c < ls)
		{
			ca[a1] = s[c];
			a1++;
		}
		c++;
		
		if(lw + li + ls == a1 + b1)
			break;
	}

	b = 0;
	int d = 0;
	char cc[a1 / 2 + 1];
	char casub[3] = {'\0'};
	cc[a1 / 2] = '\0';
	
	for(a = 0; a < a1; a += 2)
	{
		int c = -1;
		
		if(cb[b] % 2) c = 1;

		casub[0] = ca[a];
		casub[1] = ca[a + 1];

		cc[d] = strtol(casub, '\0', 36) - c;
		b++; d++;
		if(b >= b1) b = 0;
	}

	char* r1 = NULL, *r2 = NULL, *r3 = NULL, *r4 = NULL;
	char* pos = ostrstr(cc, ");}('");
	if(pos != NULL)
	{
		r1 = string_resub(");}('", "'", pos, 0);
		pos = ostrstr(pos + 5, ",'");
		if(pos != NULL)
		{
			r2 = string_resub(",'", "'", pos, 0);
			pos = ostrstr(pos + 2, ",'");
			if(pos != NULL)
			{
				r3 = string_resub(",'", "'", pos, 0);
				pos = ostrstr(pos + 2, ",'");
				if(pos != NULL)
				{
					r4 = string_resub(",'", "'", pos, 0);
					ret = getfilekey(r1, r2, r3, r4);
				}
			}
		}
	}
	else
	{
		ret = string_resub("ll=\"", "\"", cc, 0);
		if(ret == NULL)
			ret = string_resub("filekey=\"", "\"", cc, 0);	
	}

  free(r1); r1 = NULL;
	free(r2); r2 = NULL;
	free(r3); r3 = NULL;
	free(r4); r4 = NULL;
					
	return ret;
}

char* hoster(char* url)
{
	debug(99, "url: %s", url);
	char* streamurl = NULL, *tmplink = NULL;

/*
		if re.match(".*?http://www.putlocker.com/(file|embed)/", link, re.S):
		elif re.match(".*?http://www.sockshare.com/(file|embed)/", link, re.S):
		elif re.match(".*?http://www.videoslasher.com/embed/", link, re.S):
		elif re.match('.*?http://faststream.in', link, re.S):
		elif re.match('.*?http:/.*?flashx.tv', link, re.S):
		elif re.match('.*?http://streamcloud.eu', link, re.S):
		elif re.match('.*?http://vidstream.in', link, re.S):
		elif re.match('.*?http://xvidstage.com', link, re.S):
		elif re.match('.*?http://embed.nowvideo.eu', link, re.S):
		elif re.match('.*?.movshare.net', link, re.S):
		elif re.match('.*?(embed.divxstage.eu|divxstage.eu/video)', link, re.S):
		elif re.match('.*?videoweed.es', link, re.S):
		elif re.match('.*?novamov.com', link, re.S):
		elif re.match('.*primeshare', link, re.S):
		elif re.match('.*?videomega.tv', link, re.S):
		elif re.match('.*?bitshare.com', link, re.S):
		elif re.match('.*?http://movreel.com/', link, re.S):
		elif re.match('.*?uploadc.com', link, re.S):
		elif re.match('.*?http://filenuke.com', link, re.S):
		elif re.match('.*?http://www.youtube.com/watch', link, re.S):
		elif re.match('.*?http://www.mightyupload.com/embed', link, re.S):
		elif re.match('.*?180upload', link, re.S):
		elif re.match('.*?ecostream.tv', link, re.S):

	tmplink = ostrcat(url, NULL, 0, 0);

	if(ostrstr(tmpstr, "/Out/?s=") != NULL)
	{
		tmplink = string_replace("/Out/?s=", "", tmplink, 1);
		debug(99, "remove out string: %s", tmplink);
	}


						if(ostrcmp(tmphname, "Sockshare") == 0)
							hname = ostrcat("Sockshare.com", NULL, 0, 0);
						else if(ostrcmp(tmphname, "Putlocker") == 0)
							hname = ostrcat("Putlocker.com", NULL, 0, 0);
						else if(ostrcmp(tmphname, "Filenuke") == 0)
							hname = ostrcat("FileNuke.com", NULL, 0, 0);
						else if(ostrcmp(tmphname, "Streamclou") == 0)
							hname = ostrcat("StreamCloud.eu", NULL, 0, 0);
						else if(ostrcmp(tmphname, "Streamcloud") == 0)
							hname = ostrcat("StreamCloud.eu", NULL, 0, 0);
						else if(ostrcmp(tmphname, "VidStream") == 0)
							hname = ostrcat("VidStream.in", NULL, 0, 0);
						else if(ostrcmp(tmphname, "Flashx") == 0)
							hname = ostrcat("FlashX.tv", NULL, 0, 0);
						else if(ostrcmp(tmphname, "PrimeShare") == 0)
							hname = ostrcat("PrimeShare.tv", NULL, 0, 0);
						else if(ostrcmp(tmphname, "Xvidstage") == 0)
							hname = ostrcat("XvidStage.com", NULL, 0, 0);
						else if(ostrcmp(tmphname, "Nowvideo") == 0)
							hname = ostrcat("NowVideo.eu", NULL, 0, 0);
						else if(ostrcmp(tmphname, "Movshare") == 0)
							hname = ostrcat("MovShare.net", NULL, 0, 0);
						else if(ostrcmp(tmphname, "MovReel") == 0)
							hname = ostrcat("MovReel.com", NULL, 0, 0);
						else if(ostrcmp(tmphname, "NovaMov") == 0)
							hname = ostrcat("NovaMov", NULL, 0, 0);
						else if(ostrcmp(tmphname, "DivXStage") == 0)
							hname = ostrcat("DivXStage", NULL, 0, 0);
						else if(ostrcmp(tmphname, "PrimeShare") == 0)
							hname = ostrcat("PrimeShare.tv", NULL, 0, 0);
						else
						{
							hname = ostrcat(tmphname, " (coming soon)", 0, 0);
							type = 66;
						}												
*/

	tmplink = ostrcat(url, NULL, 0, 0);
	string_tolower(tmplink);

	if(ostrstr(tmplink, "sockshare") != NULL)
		streamurl = putlocker(url);
	else if(ostrstr(tmplink, "putlocker") != NULL)
		streamurl = firedrive(url);
	else if(ostrstr(tmplink, "filenuke") != NULL)
		streamurl = filenuke(url);
	else if(ostrstr(tmplink, "streamcloud") != NULL)
		streamurl = streamcloud(url);
	else if(ostrstr(tmplink, "vidstream") != NULL)
		streamurl = vidstream(url);
	else if(ostrstr(tmplink, "flashx") != NULL)
		streamurl = flashx(url);
	else if(ostrstr(tmplink, "xvidstage") != NULL)
		streamurl = xvidstage(url);
	else if(ostrstr(tmplink, "nowvideo") != NULL)
		streamurl = nowvideo(url);
	else if(ostrstr(tmplink, "movshare") != NULL)
		streamurl = movshare(url);
	else if(ostrstr(tmplink, "movreel") != NULL)
		streamurl = movreel(url);
	else if(ostrstr(tmplink, "novamov") != NULL)
		streamurl = novamov(url);
	else if(ostrstr(tmplink, "divxstage") != NULL || ostrstr(tmplink, "cloudtime") != NULL)
		streamurl = divxstage(url);
	else if(ostrstr(tmplink, "primeshare") != NULL)
		streamurl = primeshare(url);
	else if(ostrstr(tmplink, "faststream") != NULL || ostrstr(tmplink, "fastvideo") != NULL)
		streamurl = faststream(url);
	else if(ostrstr(tmplink, "played") != NULL)
		streamurl = played(url);
	else if(ostrstr(tmplink, "videoweed") != NULL)
		streamurl = videoweed(url);
	else if(ostrstr(tmplink, "firedrive") != NULL)
		streamurl = firedrive(url);
	else if(ostrstr(tmplink, "shared") != NULL)
		streamurl = shared(url);
	else if(ostrstr(tmplink, "thefile") != NULL)
		streamurl = thefile(url);
	else if(ostrstr(tmplink, "youtube") != NULL)
		streamurl = youtube(url);
	else if(ostrstr(tmplink, "myvideo") != NULL)
		streamurl = myvideo(url);
	else if(ostrstr(tmplink, "promptfile") != NULL)
		streamurl = promptfile(url);
	else if(ostrstr(tmplink, "letwatch") != NULL)
		streamurl = letwatch(url);
	else if(ostrstr(tmplink, "vidbull") != NULL)
		streamurl = vidbull(url);
	else if(ostrstr(tmplink, "vodlocker") != NULL)
		streamurl = vodlocker(url);
	else if(ostrstr(tmplink, "vidto") != NULL)
		streamurl = vidto(url);
	else if(ostrstr(tmplink, "amazon") != NULL)
		streamurl = amazon(url);
	else
		textbox(_("Message"), _("The hoster is not yet supported !"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 5, 0);


	debug(99, "Streamurl1: %s", streamurl);

	streamurl = string_replace_all("amp;", "", streamurl, 1);
	debug(99, "Streamurl2: %s", streamurl);

	free(tmplink), tmplink = NULL;
	if(ostrncmp("http", streamurl, 4) && ostrncmp("rtmp", streamurl, 4) && ostrncmp("mms", streamurl, 3) && ostrncmp("rtsp", streamurl, 4))
	{
		printf("Streamurl3: not http*|rtmp*|mms*|rtsp* Streamurl: %s\n", streamurl); 
		free(streamurl),streamurl = NULL;
	}

	return streamurl;
}

int screenlistbox(struct skin* grid, struct skin* listbox,struct skin* countlabel, char* title, char* titheklink, int* pagecount, int* tithekexit, int* oaktpage, int* oaktline, int* ogridcol, int flag, int cflag)
{
	char* tmpstr = NULL, *tmpstr1 = NULL, *tmpstr2 = NULL;

	*oaktpage = listbox->aktpage;
	*oaktline = listbox->aktline;
	*ogridcol = listbox->gridcol;
	tmpstr = ostrcat(((struct tithek*)listbox->select->handle)->link, NULL, 0, 0);

	if(title != NULL)
		tmpstr1 = ostrcat(title, " - ", 0, 0);
	else
		tmpstr1 = ostrcat(((struct tithek*)listbox->select->handle)->menutitle, " - ", 0, 0);	
	
	tmpstr2 = ostrcat(tmpstr1, ((struct tithek*)listbox->select->handle)->title, 1, 0);
	screentithekplay(tmpstr, tmpstr2, flag);
	free(tmpstr); tmpstr = NULL;
	free(tmpstr2); tmpstr2 = NULL;

	*pagecount = createtithekplay(titheklink, grid, listbox, countlabel, cflag);
//	if(pagecount == 0 || tithekexit == 1) break;
	if(*pagecount == 0 || *tithekexit == 1) return 0;
	
	listbox->aktpage = *oaktpage;
	listbox->aktline = *oaktline;
	listbox->gridcol = *ogridcol;
	addscreenrc(grid, listbox);

	return 1;
}

int all_search_local(struct skin* grid, struct skin* listbox, struct skin* countlabel, struct skin* load, char* link, char* title, char* searchstr, int flag)
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

		tmpstr = gethttp("atemio.dyndns.tv", "/mediathek/all/all-sorted.list", 80, NULL, HTTPAUTH, 5000, NULL, 0);

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
				menu = ostrcat("/tmp/tithek/all.search.list", NULL, 0, 0);
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