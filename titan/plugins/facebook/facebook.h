#ifndef FACEBOOK_H
#define FACEBOOK_H

#include <curl/curl.h>

#define FACEBOOKURL "https://graph.facebook.com"
#define FACEBOOKAPPID "150792241632891"
#define FACEBOOKPIC "/tmp/facebookpic.jpg"

char* curlretbuf = NULL;

int getfacefoto(char* url)
{
	char* ip = NULL, *pos = NULL, *path = NULL;

	unlink(FACEBOOKPIC);

	if(url == NULL) return 1;

	ip = string_replace("http://", "", (char*)url, 0);

	if(ip != NULL)
		pos = strchr(ip, '/');
	if(pos != NULL)
	{
		pos[0] = '\0';
		path = pos + 1;
	}

	gethttp(ip, path, 80, FACEBOOKPIC, NULL, NULL, 0);

	return 0;
}

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
		textbox(_("Message"), _("Can't get data from facebook"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 0, 0);
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
		changeret(tmp, "me()");
	}
	if(id == NULL) return 1;

	tmpstr = ostrcat(FACEBOOKURL, "/fql?q=SELECT+name,id+FROM+profile+WHERE+id+IN(SELECT+uid2+FROM+friend+WHERE+uid1=", 0, 0);
	tmpstr = ostrcat(tmpstr, id, 1, 0);
	tmpstr = ostrcat(tmpstr, ")&access_token=", 1, 0);
	tmpstr = ostrcat(tmpstr, getconfig("facebooktoken", NULL), 1, 0);
	curlretbuf = gethttps(tmpstr);
	free(tmpstr); tmpstr = NULL;
	if(curlretbuf == NULL || strstr(curlretbuf, "\"error\":") != NULL)
	{
		textbox(_("Message"), _("Can't get data from facebook"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 0, 0);
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
				if(len > 0 && ostrncmp(ptr, "name", len) == 0)
					treffer = 1;
				else if(len > 0 && ostrncmp(ptr, "id", len) == 0)
					treffer = 2;
			}
		}
	}

	free(curlretbuf); curlretbuf = NULL;
	free(buf); buf = NULL;
	return 0;
}

int getfaceuser(struct skin* name, struct skin* status, struct skin* gender, struct skin* foto, char* id)
{
	char* tmpstr = NULL;

	changetext(name, NULL);
	changetext(status, NULL);
	changetext(gender, NULL);

	if(id == NULL) return 1;

	tmpstr = ostrcat(FACEBOOKURL, "/fql?q=SELECT+name,sex,status,pic+FROM+user+WHERE+uid=", 0, 0);
	tmpstr = ostrcat(tmpstr, id, 1, 0);
	tmpstr = ostrcat(tmpstr, "&access_token=", 1, 0);
	tmpstr = ostrcat(tmpstr, getconfig("facebooktoken", NULL), 1, 0);
	curlretbuf = gethttps(tmpstr);
	free(tmpstr); tmpstr = NULL;
	if(curlretbuf == NULL || strstr(curlretbuf, "\"error\":") != NULL)
	{
		textbox(_("Message"), _("Can't get data from facebook"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 0, 0);
		free(curlretbuf); curlretbuf = NULL;
		return 1;
	}

	tmpstr = getxmlentry(curlretbuf, "\"name\":");
	changetext(name, tmpstr);
	free(tmpstr); tmpstr = NULL;

	tmpstr = getxmlentry(curlretbuf, "\"sex\":");
	changetext(gender, tmpstr);
	free(tmpstr); tmpstr = NULL;

	tmpstr = getxmlentry(curlretbuf, "\"status\":");
	changetext(status, tmpstr);
	free(tmpstr); tmpstr = NULL;

	tmpstr = getxmlentry(curlretbuf, "\"pic\":");
	tmpstr = string_replace_all("\\", "", tmpstr, 1);
	getfacefoto(tmpstr);
	changepic(foto, FACEBOOKPIC);
	free(tmpstr); tmpstr = NULL;

	free(curlretbuf); curlretbuf = NULL;
	return 0;
}

int getfacestream(struct skin* facebook, struct skin* listbox, char* id)
{
	int ret = -1, i = 0, len = 0, treffer = 0;
	jsmn_parser parser;
	jsmntok_t tokens[100]; //TODO
	char* buf = NULL, *tmpstr = NULL;
	struct skin* tmp = NULL;

	delmarkedscreennodes(facebook, 1);
	if(id == NULL) return 1;

	tmpstr = ostrcat(FACEBOOKURL, "/fql?q=SELECT+message,post_id+FROM+stream+WHERE+source_id=", 0, 0);
	tmpstr = ostrcat(tmpstr, id, 1, 0);
	tmpstr = ostrcat(tmpstr, "&access_token=", 1, 0);
	tmpstr = ostrcat(tmpstr, getconfig("facebooktoken", NULL), 1, 0);
	curlretbuf = gethttps(tmpstr);
printf("%s\n", tmpstr);
printf("%s\n", curlretbuf);
	free(tmpstr); tmpstr = NULL;
	if(curlretbuf == NULL || strstr(curlretbuf, "\"error\":") != NULL)
	{
		textbox(_("Message"), _("Can't get data from facebook"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 0, 0);
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
				tmp = addlistbox(facebook, listbox, tmp, 1);
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
				if(len > 0 && ostrncmp(ptr, "message", len) == 0)
					treffer = 1;
				else if(len > 0 && ostrncmp(ptr, "post_id", len) == 0)
					treffer = 2;
			}
		}
	}

	free(curlretbuf); curlretbuf = NULL;
	free(buf); buf = NULL;
	return 0;
}

int getfacenote(struct skin* facebook, struct skin* listbox, char* id)
{
	int ret = -1, i = 0, len = 0, treffer = 0;
	jsmn_parser parser;
	jsmntok_t tokens[100]; //TODO
	char* buf = NULL, *tmpstr = NULL;
	struct skin* tmp = NULL;

	delmarkedscreennodes(facebook, 1);
	if(id == NULL) return 1;

	tmpstr = ostrcat(FACEBOOKURL, "/fql?q=SELECT+title,content,note_id+FROM+note+WHERE+uid=", 0, 0);
	tmpstr = ostrcat(tmpstr, id, 1, 0);
	tmpstr = ostrcat(tmpstr, "&access_token=", 1, 0);
	tmpstr = ostrcat(tmpstr, getconfig("facebooktoken", NULL), 1, 0);
	curlretbuf = gethttps(tmpstr);
printf("%s\n", tmpstr);
printf("%s\n", curlretbuf);
	free(tmpstr); tmpstr = NULL;
	if(curlretbuf == NULL || strstr(curlretbuf, "\"error\":") != NULL)
	{
		textbox(_("Message"), _("Can't get data from facebook"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 0, 0);
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
				tmp = addlistbox(facebook, listbox, tmp, 1);
				if(tmp != NULL)
				{
					strncpy(buf, ptr, len);
					buf[len] = '\0';
					changetext(tmp, buf);
				}
			}

			if(treffer == 2)
			{
				tmp = addlistbox(facebook, listbox, tmp, 1);
				if(tmp != NULL)
				{
					strncpy(buf, ptr, len);
					buf[len] = '\0';
					changetext2(tmp, buf);
				}
			}

			if(treffer == 3)
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
				if(len > 0 && ostrncmp(ptr, "title", len) == 0)
					treffer = 1;
				if(len > 0 && ostrncmp(ptr, "content", len) == 0)
					treffer = 2;
				else if(len > 0 && ostrncmp(ptr, "note_id", len) == 0)
					treffer = 3;
			}
		}
	}

	free(curlretbuf); curlretbuf = NULL;
	free(buf); buf = NULL;
	return 0;
}

int getfacealbum(struct skin* facebook, struct skin* listbox, char* id)
{
	int ret = -1, i = 0, len = 0, treffer = 0;
	jsmn_parser parser;
	jsmntok_t tokens[100]; //TODO
	char* buf = NULL, *tmpstr = NULL;
	struct skin* tmp = NULL;

	delmarkedscreennodes(facebook, 1);
	if(id == NULL) return 1;

	tmpstr = ostrcat(FACEBOOKURL, "/fql?q=SELECT+name,aid+FROM+album+WHERE+owner=", 0, 0);
	tmpstr = ostrcat(tmpstr, id, 1, 0);
	tmpstr = ostrcat(tmpstr, "&access_token=", 1, 0);
	tmpstr = ostrcat(tmpstr, getconfig("facebooktoken", NULL), 1, 0);
	curlretbuf = gethttps(tmpstr);
printf("%s\n", tmpstr);
printf("%s\n", curlretbuf);
	free(tmpstr); tmpstr = NULL;
	if(curlretbuf == NULL || strstr(curlretbuf, "\"error\":") != NULL)
	{
		textbox(_("Message"), _("Can't get data from facebook"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 0, 0);
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
				tmp = addlistbox(facebook, listbox, tmp, 1);
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
				if(len > 0 && ostrncmp(ptr, "name", len) == 0)
					treffer = 1;
				if(len > 0 && ostrncmp(ptr, "aid", len) == 0)
					treffer = 2;
			}
		}
	}

	free(curlretbuf); curlretbuf = NULL;
	free(buf); buf = NULL;
	return 0;
}

int getfacecomment(struct skin* facebook, struct skin* listbox, char* id)
{
	int ret = -1, i = 0, len = 0, treffer = 0;
	jsmn_parser parser;
	jsmntok_t tokens[100]; //TODO
	char* buf = NULL, *tmpstr = NULL;
	struct skin* tmp = NULL;

	delmarkedscreennodes(facebook, 1);
	if(id == NULL) return 1;

	tmpstr = ostrcat(FACEBOOKURL, "/fql?q=SELECT+username,text+FROM+comment+WHERE+object_id=", 0, 0);
	tmpstr = ostrcat(tmpstr, id, 1, 0);
	tmpstr = ostrcat(tmpstr, "&access_token=", 1, 0);
	tmpstr = ostrcat(tmpstr, getconfig("facebooktoken", NULL), 1, 0);
	curlretbuf = gethttps(tmpstr);
printf("%s\n", tmpstr);
printf("%s\n", curlretbuf);
	free(tmpstr); tmpstr = NULL;
	if(curlretbuf == NULL || strstr(curlretbuf, "\"error\":") != NULL)
	{
		textbox(_("Message"), _("Can't get data from facebook"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 0, 0);
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
				tmp = addlistbox(facebook, listbox, tmp, 1);
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
					changetext2(tmp, buf);
				}
			}

			treffer = 0;
			if(tokens[i].type == 3)
			{
				if(len > 0 && ostrncmp(ptr, "username", len) == 0)
					treffer = 1;
				if(len > 0 && ostrncmp(ptr, "text", len) == 0)
					treffer = 2;
			}
		}
	}

	free(curlretbuf); curlretbuf = NULL;
	free(buf); buf = NULL;
	return 0;
}

void screenfacecomment(char* id)
{
	int rcret = -1;

	struct skin* facecomment = getscreen("facecomment");
	struct skin* listbox = getscreennode(facecomment, "listbox");

	if(id == NULL) return;

	getfacecomment(facecomment, listbox, id);
	drawscreen(facecomment, 0, 0);
	addscreenrc(facecomment, listbox);

	while(1)
	{
		rcret = waitrc(facecomment, 0, 0);

		if(rcret == getrcconfigint("rcexit", NULL)) break;
		if(rcret == getrcconfigint("rcok", NULL)) break;
	}

	delownerrc(facecomment);
	delmarkedscreennodes(facecomment, 1);
	clearscreen(facecomment);
}

void screenface()
{
	if(getconfig("facebooktoken", NULL) == NULL)
		screenfaceregister();

	int rcret = -1, aktview = 0, aktlist = 0;

	struct skin* facetitle = getscreen("facetitle");
	struct skin* facefriends = getscreen("facefriends");
	struct skin* facefriendslist = getscreennode(facefriends, "facefriendslist");
	struct skin* facefoto = getscreen("facefoto");
	struct skin* facebook = getscreen("facebook");
	struct skin* facebooklist = getscreennode(facebook, "facebooklist");
	struct skin* facebutton = getscreen("facebutton");
	struct skin* faceuser = getscreen("faceuser");
	struct skin* facename = getscreennode(faceuser, "facename");
	struct skin* facestatus = getscreennode(faceuser, "facestatus");
	struct skin* facegender = getscreennode(faceuser, "facegender");

	getfacefriens(facefriends, facefriendslist, "me()");
	getfaceuser(facename, facestatus, facegender, facefoto, "me()");
	getfacestream(facebook, facebooklist, "me()");

	drawscreen(facetitle, 0, 1);
	drawscreen(facefriends, 0, 1);
	drawscreen(facefoto, 0, 1);
	drawscreen(facebutton, 0, 1);
	drawscreen(faceuser, 0, 1);
	drawscreen(facebook, 0, 0);

	addscreenrc(facefriends, facefriendslist);
	addscreenrc(facebook, facebooklist);

	while(1)
	{
		if(aktlist == 0)
			rcret = waitrc(facefriends, 0, 0);
		else
			rcret = waitrc(facebook, 0, 0);

		if(rcret == getrcconfigint("rcff", NULL) || rcret == getrcconfigint("rcfr", NULL))
		{
			if(aktlist == 0)
				aktlist = 1;
			else
				aktlist = 0;

			continue;
		}

		if(rcret == getrcconfigint("rcexit", NULL)) break;
		if(rcret == getrcconfigint("rcred", NULL))
		{
			if(aktview != 0)
			{
				getfacestream(facebook, facebooklist, facefriendslist->select->ret);
				drawscreen(facebook, 0, 0);
			}
			aktview = 0;
		}
		if(rcret == getrcconfigint("rcgreen", NULL))
		{
			if(aktview != 1)
			{
				getfacenote(facebook, facebooklist, facefriendslist->select->ret);
				drawscreen(facebook, 0, 0);
			}
			aktview = 1;
		}
		if(rcret == getrcconfigint("rcyellow", NULL))
		{
			if(aktview != 2)
			{
				getfacealbum(facebook, facebooklist, facefriendslist->select->ret);
				drawscreen(facebook, 0, 0);
			}
			aktview = 2;
		}
		if(aktlist == 0 && rcret == getrcconfigint("rcok", NULL) && facefriendslist->select != NULL)
		{
			getfaceuser(facename, facestatus, facegender, facefoto, facefriendslist->select->ret);
			if(aktview == 0)
				getfacestream(facebook, facebooklist, facefriendslist->select->ret);
			else if(aktview == 1)
				getfacenote(facebook, facebooklist, facefriendslist->select->ret);
			else if(aktview == 2)
				getfacealbum(facebook, facebooklist, facefriendslist->select->ret);

			drawscreen(faceuser, 0, 1);
			drawscreen(facebook, 0, 1);
			drawscreen(facefoto, 0, 0);

		}
		if(aktlist == 1 && rcret == getrcconfigint("rcok", NULL) && facebooklist->select != NULL)
		{
			screenfacecomment(facebooklist->select->ret);
			drawscreen(facetitle, 0, 1);
			drawscreen(facefriends, 0, 1);
			drawscreen(facefoto, 0, 1);
			drawscreen(facebutton, 0, 1);
			drawscreen(faceuser, 0, 1);
			drawscreen(facebook, 0, 0);
		}
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

/*
https://graph.facebook.com/fql?q=SELECT+object_id,username,text+FROM+comment+WHERE+object_id=207643296028886&access_token=AAAAAAITEghMBACankVwWxFeerpb1flmzioJpbJ1mdCM1luIr60wjI13GDLbtRQMP0WlTonwm8La9ZABpN8TaRbYDFw7ApVfkeprfm4IGSkX7QXyRS

*/
