#ifndef FACEBOOK_H
#define FACEBOOK_H

#include <curl/curl.h>

#define FACEBOOKURL "https://graph.facebook.com"
#define FACEBOOKAPPID "150792241632891"

char* curlretbuf = NULL;

size_t curl_write( void *ptr, size_t size, size_t nmemb, void *stream)
{
	curlretbuf = calloc(1, size * nmemb);
	if(curlretbuf != NULL)
		memcpy(curlretbuf, ptr, size * nmemb);
		//buffer.append((char*)ptr, size * nmemb);
	return size * nmemb;
}

char* gethttps(char* url)
{
	CURL *curl;
	CURLcode res;

	free(curlretbuf); curlretbuf = NULL;

	curl = curl_easy_init();
	if(curl)
	{
		curl_easy_setopt(curl, CURLOPT_URL, url);
		curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 5);
		curl_easy_setopt(curl, CURLOPT_TIMEOUT, 20);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curl_write);
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);

		//Perform the request, res will get the return code
		res = curl_easy_perform(curl);
		if(res != CURLE_OK)
		{
			err("failed: %s", curl_easy_strerror(res));
		}

		//always cleanup
		curl_easy_cleanup(curl);
	}

	return curlretbuf;
}

void screenfaceregister()
{
	int rcret = -1, count = 0;

	struct skin* faceregister = getscreen("faceregister");
	struct skin* key = getscreennode(faceregister, "key");
	struct skin* url = getscreennode(faceregister, "url");

	char* access_token = NULL;
	char* tmpstr = NULL;

	if(getconfig("facebooktoken", NULL) != NULL)
	{
		if(textbox(_("Message"), _("Facebook allready registered,\nMake new registration ?"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 0, 0) == 2)
			return;
	}

	tmpstr = ostrcat(FACEBOOKURL, "/oauth/device?type=device_code&client_id=", 0, 0);
	tmpstr = ostrcat(tmpstr, FACEBOOKAPPID, 1, 0);
	curlretbuf = gethttps(tmpstr);
	free(tmpstr); tmpstr = NULL;
	if(curlretbuf == NULL)
	{
		textbox(_("Message"), _("Connect to facebook failed"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 0, 0);
		return;
	}

	tmpstr = getxmlentry(curlretbuf, "\"verification_uri\":");
	tmpstr = string_replace_all("\\", "", tmpstr, 1);
	changetext(url, tmpstr);
	free(tmpstr); tmpstr = NULL;

	tmpstr = getxmlentry(curlretbuf, "\"user_code\":");
	changetext(key, tmpstr);
	free(tmpstr); tmpstr = NULL;

	tmpstr = ostrcat(FACEBOOKURL, "/oauth/device?type=device_token&client_id=", 0, 0);
	tmpstr = ostrcat(tmpstr, FACEBOOKAPPID, 1, 0);
	tmpstr = ostrcat(tmpstr, "&code=", 1, 0);
	tmpstr = ostrcat(tmpstr, getxmlentry(curlretbuf, "\"code\":"), 1, 1);

	drawscreen(faceregister, 0, 0);

	while(1)
	{
		count++;
		rcret = waitrc(NULL, 1000, 0);

		if(rcret == getrcconfigint("rcexit", NULL) || rcret == getrcconfigint("rcok", NULL) || count > 100)
			break;
		
		curlretbuf = gethttps(tmpstr);
		access_token = getxmlentry(curlretbuf, "\"access_token\":");
		if(access_token != NULL)
		{
			addconfig("facebooktoken", access_token);
printf("%s\n", access_token);
			changetext(key, "Register to facebook successful");
			break;
		}
	}

	free(curlretbuf); curlretbuf = NULL;
	free(access_token); access_token = NULL;
	free(tmpstr); tmpstr = NULL;

	if(count > 100) changetext(key, "Register to facebook not ok");
	drawscreen(faceregister, 0, 0);
	sleep(3);
	clearscreen(faceregister);
}

int getfacefriens(struct skin* facefriends, struct skin* listbox, char* id)
{
	int ret = -1, i = 0, len = 0, treffer = 0;
	jsmn_parser parser;
	jsmntok_t tokens[100]; //TODO
	char* buf = NULL, *tmpstr = NULL;
	struct skin* tmp = NULL;

	tmp = addlistbox(facefriends, listbox, tmp, 1);
	if(tmp != NULL)
	{
		changetext(tmp, "Home");
		changeret(tmp, "me");
	}

	tmpstr = ostrcat(FACEBOOKURL, "/", 0, 0);
	tmpstr = ostrcat(tmpstr, id, 1, 0);
	tmpstr = ostrcat(tmpstr, "/friends?access_token=", 1, 0);
	tmpstr = ostrcat(tmpstr, getconfig("facebooktoken", NULL), 1, 0);
	curlretbuf = gethttps(tmpstr);
	free(tmpstr); tmpstr = NULL;
	if(curlretbuf == NULL || strstr(curlretbuf, "\"error\":") != NULL)
	{
		textbox(_("Message"), _("Connect to facebook failed"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 0, 0);
		free(curlretbuf); curlretbuf = NULL;
		return 1;
	}

	buf = malloc(MINMALLOC);
	if(buf == NULL)
	{
		err("no mem");
		free(curlretbuf); curlretbuf = NULL;
		return 1;
	}

	jsmn_init(&parser);

	ret = jsmn_parse(&parser, curlretbuf, tokens, 100);
	if(ret == JSMN_SUCCESS)
	{
		for(i = 0; i < 100; i++)
		{
			if(tokens[i].start == -1) break;

			len = tokens[i].end - tokens[i].start;
			if(len > MINMALLOC) len = MINMALLOC;
			char* ptr = curlretbuf + tokens[i].start;

			if(treffer == 1)
			{
				tmp = addlistbox(facefriends, listbox, tmp, 1);
				if(tmp != NULL)
				{
					strncpy(buf, ptr, len);
					buf[len] = '\0';
					changetext(tmp, buf);
				}
			}

			if(treffer == 2)
			{
				if(tmp != NULL)
				{
					strncpy(buf, ptr, len);
					buf[len] = '\0';
					changeret(tmp, buf);
				}
			}

			treffer = 0;
			if(tokens[i].type == 3)
			{
				if(ostrncmp(ptr, "name", len) == 0)
					treffer = 1;
				else if(ostrncmp(ptr, "id", len) == 0)
					treffer = 2;
			}
		}
	}

	free(curlretbuf); curlretbuf = NULL;
	free(buf); buf = NULL;
	return 0;
}

int getfaceuser(struct skin* name, struct skin* username, struct skin* gender, char* id)
{
	char* tmpstr = NULL;

	changetext(name, NULL);
	changetext(username, NULL);
	changetext(gender, NULL);

	tmpstr = ostrcat(FACEBOOKURL, "/", 0, 0);
	tmpstr = ostrcat(tmpstr, id, 1, 0);
	tmpstr = ostrcat(tmpstr, "?access_token=", 1, 0);
	tmpstr = ostrcat(tmpstr, getconfig("facebooktoken", NULL), 1, 0);
	curlretbuf = gethttps(tmpstr);
	free(tmpstr); tmpstr = NULL;
	if(curlretbuf == NULL || strstr(curlretbuf, "\"error\":") != NULL)
	{
		textbox(_("Message"), _("Connect to facebook failed"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 0, 0);
		free(curlretbuf); curlretbuf = NULL;
		return 1;
	}

	tmpstr = getxmlentry(curlretbuf, "\"name\":");
	changetext(name, tmpstr);
	free(tmpstr); tmpstr = NULL;

	tmpstr = getxmlentry(curlretbuf, "\"username\":");
	changetext(username, tmpstr);
	free(tmpstr); tmpstr = NULL;

	tmpstr = getxmlentry(curlretbuf, "\"gender\":");
	changetext(gender, tmpstr);
	free(tmpstr); tmpstr = NULL;

	free(curlretbuf); curlretbuf = NULL;
	return 0;
}

void screenface()
{
	if(getconfig("facebooktoken", NULL) == NULL)
		screenfaceregister();

	int rcret = -1;

	struct skin* facetitle = getscreen("facetitle");
	struct skin* facefriends = getscreen("facefriends");
	struct skin* facefriendslist = getscreennode(facefriends, "listbox");
	struct skin* facefoto = getscreen("facefoto");
	struct skin* facebook = getscreen("facebook");
	struct skin* facebooklist = getscreennode(facebook, "facebooklist");
	struct skin* facebutton = getscreen("facebutton");
	struct skin* faceuser = getscreen("faceuser");
	struct skin* facename = getscreennode(faceuser, "facename");
	struct skin* faceusername = getscreennode(faceuser, "faceusername");
	struct skin* facegender = getscreennode(faceuser, "facegender");

	getfacefriens(facefriends, facefriendslist, "me");
	getfaceuser(facename, faceusername, facegender, "me");

	drawscreen(facetitle, 0, 1);
	drawscreen(facefriends, 0, 1);
	drawscreen(facefoto, 0, 1);
	drawscreen(facebutton, 0, 1);
	drawscreen(faceuser, 0, 1);
	drawscreen(facebook, 0, 0);

	addscreenrc(facefriends, facefriendslist);

	while(1)
	{
		rcret = waitrc(facefriends, 0, 0);

		if(rcret == getrcconfigint("rcexit", NULL)) break;
	}

	delownerrc(facefriends);
	delmarkedscreennodes(facefriends, 1);
	delownerrc(facebook);
	delmarkedscreennodes(facebook, 1);
	clearscreen(facetitle);
	clearscreen(facefriends);
	clearscreen(facefoto);
	clearscreen(facebutton);
	clearscreen(faceuser);
	clearscreen(facebook);
}

#endif
