#ifndef FACEBOOK_H
#define FACEBOOK_H

#include <curl/curl.h>

#define FACEBOOKURL "https://graph.facebook.com"
#define FACEBOOKAPPID "150792241632891"
#define FACEBOOKPICJPG "/tmp/facebook/facebookpic.jpg"
#define FACEBOOKPICPNG "/tmp/facebook/facebookpic.png"
#define FACEBOOKPATH "/tmp/facebook"
#define FACEMAXTOKEN 1000
#define FACEMAXENTRY "50"
#define FACEMAXFRIEND "200"

int curlretbufsize = 0;
char* curlretbuf = NULL;

void changefacetext(struct skin* node, char* text)
{
	text = string_replace("\\n", "", text, 0);
	htmldecode3(text, text);
	text = string_deltags(text);
	changetext(node, text);
	free(text);
}

void changefacetext2(struct skin* node, char* text)
{
	text = string_replace("\\n", "", text, 0);
	htmldecode3(text, text);
	text = string_deltags(text);
	changetext2(node, text);
	free(text);
}

void convertfacedate(char* buf)
{
	char* buf1 = NULL;
	struct tm *loctime = NULL;
	time_t t = 0;

	buf1 = calloc(1, 17);
	if(buf1 == NULL)
	{
		err("no mem");
		return;
	}

	if(buf != NULL)
	{
		t = strtoul(buf, NULL, 10);
		loctime = olocaltime(&t);
		if(loctime != NULL)
			strftime(buf1, 17, "%d-%m-%Y %H:%M", loctime);
		strncpy(buf, buf1, 17);
	}

	free(loctime); loctime = NULL;
	free(buf1); buf1 = NULL;
}

int checkfaceerror()
{
	if(curlretbuf == NULL || strstr(curlretbuf, "\"error\":") != NULL)
	{
		textbox(_("Message"), _("Can't get data from facebook"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 0, 0);
		free(curlretbuf); curlretbuf = NULL;
		curlretbufsize = 0;
		return 1;
	}
	return 0;
}

int getfacefoto(char* url, char* name)
{
	char* ip = NULL, *pos = NULL, *path = NULL;

	unlink(name);

	if(url == NULL) return 1;

	ip = string_replace("http://", "", (char*)url, 0);

	if(ip != NULL)
		pos = strchr(ip, '/');
	if(pos != NULL)
	{
		pos[0] = '\0';
		path = pos + 1;
	}

	gethttp(ip, path, 80, name, NULL, 5000, NULL, 0);

	return 0;
}

size_t curl_write( void *ptr, size_t size, size_t nmemb, void *stream)
{
	curlretbuf = realloc(curlretbuf, (curlretbufsize + (size * nmemb)) + 1);
	if(curlretbuf != NULL)
	{
		memcpy(curlretbuf + curlretbufsize, ptr, size * nmemb);
		curlretbufsize += size * nmemb;
		curlretbuf[curlretbufsize] = '\0';
	}
	else
		curlretbufsize = 0;

	return size * nmemb;
}

char* gethttps1(char* url)
{
	CURL *curl;
	CURLcode res;

	free(curlretbuf); curlretbuf = NULL;
	curlretbufsize = 0;

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
	curlretbuf = gethttps1(tmpstr);
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
		
		curlretbuf = gethttps1(tmpstr);
		access_token = getxmlentry(curlretbuf, "\"access_token\":");
		if(access_token != NULL)
		{
			addconfig("facebooktoken", access_token);
			debug(369, "token=%s", access_token);
			changetext(key, "Register to facebook successful");
			break;
		}
	}

	free(curlretbuf); curlretbuf = NULL;
	curlretbufsize = 0;
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
	jsmntok_t tokens[FACEMAXTOKEN]; //TODO
	char* buf = NULL, *tmpstr = NULL;
	struct skin* tmp = NULL;

	if(id == NULL) return 1;

	tmpstr = ostrcat(FACEBOOKURL, "/fql?q=SELECT+name,id+FROM+profile+WHERE+id=me()+OR+id+IN(SELECT+uid2+FROM+friend+WHERE+uid1=", 0, 0);
	tmpstr = ostrcat(tmpstr, id, 1, 0);
	tmpstr = ostrcat(tmpstr, ")+LIMIT+0,", 1, 0);
	tmpstr = ostrcat(tmpstr, FACEMAXFRIEND, 1, 0);
	tmpstr = ostrcat(tmpstr, "&access_token=", 1, 0);
	tmpstr = ostrcat(tmpstr, getconfig("facebooktoken", NULL), 1, 0);
	curlretbuf = gethttps1(tmpstr);
	debug(369, "url=%s", tmpstr);
	debug(369, "ret=%s", curlretbuf);
	free(tmpstr); tmpstr = NULL;
	if(checkfaceerror() == 1) return 1;

	buf = malloc(MINMALLOC);
	if(buf == NULL)
	{
		err("no mem");
		free(curlretbuf); curlretbuf = NULL;
		curlretbufsize = 0;
		return 1;
	}

	jsmn_init(&parser);

	ret = jsmn_parse(&parser, curlretbuf, tokens, FACEMAXTOKEN);
	if(ret == JSMN_SUCCESS)
	{
		for(i = 0; i < FACEMAXTOKEN; i++)
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
					changefacetext(tmp, buf);
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
	curlretbufsize = 0;
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
	curlretbuf = gethttps1(tmpstr);
	debug(369, "url=%s", tmpstr);
	debug(369, "ret=%s", curlretbuf);
	free(tmpstr); tmpstr = NULL;
	if(checkfaceerror() == 1) return 1;

	tmpstr = getxmlentry(curlretbuf, "\"name\":");
	changefacetext(name, tmpstr);
	free(tmpstr); tmpstr = NULL;

	tmpstr = getxmlentry(curlretbuf, "\"sex\":");
	changefacetext(gender, tmpstr);
	free(tmpstr); tmpstr = NULL;

	tmpstr = getxmlentry(curlretbuf, "\"status\":");
	changefacetext(status, tmpstr);
	free(tmpstr); tmpstr = NULL;

	tmpstr = getxmlentry(curlretbuf, "\"pic\":");
	tmpstr = string_replace_all("\\", "", tmpstr, 1);
	if(ostrstr(tmpstr, ".jpg") != NULL)
	{
		getfacefoto(tmpstr, FACEBOOKPICJPG);
		changepic(foto, FACEBOOKPICJPG);
	}
	else
	{
		getfacefoto(tmpstr, FACEBOOKPICPNG);
		changepic(foto, FACEBOOKPICPNG);
	}
	free(tmpstr); tmpstr = NULL;

	free(curlretbuf); curlretbuf = NULL;
	curlretbufsize = 0;
	return 0;
}

int getfacestream(struct skin* facebook, struct skin* listbox, char* id)
{
	int ret = -1, i = 0, len = 0, treffer = 0, lockphoto = 1;
	jsmn_parser parser;
	jsmntok_t tokens[FACEMAXTOKEN]; //TODO
	char* buf = NULL, *tmpstr = NULL;
	struct skin* tmp = NULL;

	delmarkedscreennodes(facebook, 1);
	if(id == NULL) return 1;

	tmpstr = ostrcat(FACEBOOKURL, "/fql?q=SELECT+message,post_id,attachment,type,created_time+FROM+stream+WHERE+source_id=", 0, 0);
	tmpstr = ostrcat(tmpstr, id, 1, 0);
	tmpstr = ostrcat(tmpstr, "+LIMIT+0,", 1, 0);
	tmpstr = ostrcat(tmpstr, FACEMAXENTRY, 1, 0);
	tmpstr = ostrcat(tmpstr, "&access_token=", 1, 0);
	tmpstr = ostrcat(tmpstr, getconfig("facebooktoken", NULL), 1, 0);
	curlretbuf = gethttps1(tmpstr);
	debug(369, "url=%s", tmpstr);
	debug(369, "ret=%s", curlretbuf);
	free(tmpstr); tmpstr = NULL;
	if(checkfaceerror() == 1) return 1;

	buf = malloc(MINMALLOC);
	if(buf == NULL)
	{
		err("no mem");
		free(curlretbuf); curlretbuf = NULL;
		curlretbufsize = 0;
		return 1;
	}

	jsmn_init(&parser);

	ret = jsmn_parse(&parser, curlretbuf, tokens, FACEMAXTOKEN);
	if(ret == JSMN_SUCCESS)
	{
		for(i = 0; i < FACEMAXTOKEN; i++)
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
					tmp->type = TEXTBOX;
					tmp->wrap = YES;
					tmp->height = 100;
					strncpy(buf, ptr, len);
					buf[len] = '\0';
					changefacetext(tmp, buf);
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

			if(treffer == 3)
			{
				if(tmp != NULL)
				{
					strncpy(buf, ptr, len);
					buf[len] = '\0';
					changefacetext2(tmp, buf);
				}
			}

			if(treffer == 4)
			{
				if(lockphoto == 0 && tmp != NULL)
				{
					char* tmppic = NULL;
					char* tmpbuf = NULL;
          
					lockphoto = 1;
					tmp->textposx = 150;
					tmp->textposx2 = 150;

					strncpy(buf, ptr, len);
					buf[len] = '\0';
					tmpbuf = string_replace_all("\\", "", buf, 0);
					tmppic = ostrcat(FACEBOOKPATH, "/", 0, 0);
					tmppic = ostrcat(tmppic, tmp->ret, 1, 0);
					if(ostrstr(tmpbuf, ".jpg") != NULL)
						tmppic = ostrcat(tmppic, ".jpg", 1, 0);
					else
						tmppic = ostrcat(tmppic, ".png", 1, 0);
					getfacefoto(tmpbuf, tmppic);
					changepic(tmp, tmppic);
					free(tmppic); tmppic = NULL;
					free(tmpbuf); tmpbuf = NULL;
				}
			}

			if(treffer == 5)
			{
				strncpy(buf, ptr, len);
				buf[len] = '\0';
				
				if(ostrcmp(buf, "photo") == 0) lockphoto = 0;
				
				if(tmp != NULL && (tmp->text == NULL || (tmp->text != NULL && strlen(tmp->text) == 0)))
				{
					int type = 0;

					if(buf != NULL)
						type = atoi(buf);
					switch(type)
					{
						case 11: changetext(tmp, _("Group created")); break;
						case 12: changetext(tmp, _("Event created")); break;
						case 46: changetext(tmp, _("Status update")); break;
						case 56: changetext(tmp, _("Post on wall from another user")); break;
						case 66: changetext(tmp, _("Note created")); break;
						case 80: changetext(tmp, _("Link postet")); break;
						case 128: changetext(tmp, _("Video postet")); break;
						case 247: changetext(tmp, _("Photos postet")); break;
						case 237: changetext(tmp, _("App story")); break;
						case 257: changetext(tmp, _("Coment created")); break;
						case 272: changetext(tmp, _("App story")); break;
						case 285: changetext(tmp, _("Checkin to a place")); break;
						case 308: changetext(tmp, _("Post in group")); break;
					}
				}
			}

			if(treffer == 6)
			{
				if(tmp != NULL)
				{
					strncpy(buf, ptr, len);
					buf[len] = '\0';
					convertfacedate(buf);

					if(tmp->text != NULL && strlen(tmp->text) != 0)
						tmp->text = ostrcat(tmp->text, " - ", 1, 0);
					tmp->text = ostrcat(tmp->text, buf, 1, 0);
				}
			}

			treffer = 0;
			if(tokens[i].type == 3)
			{
				if(len > 0 && ostrncmp(ptr, "message", len) == 0)
				{
					treffer = 1;
					lockphoto = 1;
				}
				else if(len > 0 && ostrncmp(ptr, "post_id", len) == 0)
					treffer = 2;
				else if(len > 0 && ostrncmp(ptr, "description", len) == 0)
					treffer = 3;
				else if(len > 0 && ostrncmp(ptr, "src", len) == 0)
					treffer = 4;
				else if(len > 0 && ostrncmp(ptr, "type", len) == 0)
					treffer = 5;
				else if(len > 0 && ostrncmp(ptr, "created_time", len) == 0)
					treffer = 6;
			}
		}
	}

	free(curlretbuf); curlretbuf = NULL;
	curlretbufsize = 0;
	free(buf); buf = NULL;
	return 0;
}

int getfacenote(struct skin* facebook, struct skin* listbox, char* id)
{
	int ret = -1, i = 0, len = 0, treffer = 0;
	jsmn_parser parser;
	jsmntok_t tokens[FACEMAXTOKEN]; //TODO
	char* buf = NULL, *tmpstr = NULL;
	struct skin* tmp = NULL;

	delmarkedscreennodes(facebook, 1);
	if(id == NULL) return 1;

	tmpstr = ostrcat(FACEBOOKURL, "/fql?q=SELECT+title,content,created_time,note_id+FROM+note+WHERE+uid=", 0, 0);
	tmpstr = ostrcat(tmpstr, id, 1, 0);
	tmpstr = ostrcat(tmpstr, "+LIMIT+0,", 1, 0);
	tmpstr = ostrcat(tmpstr, FACEMAXENTRY, 1, 0);
	tmpstr = ostrcat(tmpstr, "&access_token=", 1, 0);
	tmpstr = ostrcat(tmpstr, getconfig("facebooktoken", NULL), 1, 0);
	curlretbuf = gethttps1(tmpstr);
	debug(369, "url=%s", tmpstr);
	debug(369, "ret=%s", curlretbuf);
	free(tmpstr); tmpstr = NULL;
	if(checkfaceerror() == 1) return 1;

	buf = malloc(MINMALLOC);
	if(buf == NULL)
	{
		err("no mem");
		free(curlretbuf); curlretbuf = NULL;
		curlretbufsize = 0;
		return 1;
	}

	jsmn_init(&parser);

	ret = jsmn_parse(&parser, curlretbuf, tokens, FACEMAXTOKEN);
	if(ret == JSMN_SUCCESS)
	{
		for(i = 0; i < FACEMAXTOKEN; i++)
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
					tmp->type = TEXTBOX;
					tmp->wrap = YES;
					tmp->height = 100;
					strncpy(buf, ptr, len);
					buf[len] = '\0';
					changefacetext(tmp, buf);
				}
			}

			if(treffer == 2)
			{
				if(tmp != NULL)
				{
					strncpy(buf, ptr, len);
					buf[len] = '\0';
					changefacetext2(tmp, buf);
				}
			}

			if(treffer == 3)
			{
				if(tmp != NULL)
				{
					strncpy(buf, ptr, len);
					buf[len] = '\0';
					convertfacedate(buf);

					if(tmp->text != NULL && strlen(tmp->text) != 0)
						tmp->text = ostrcat(tmp->text, " - ", 1, 0);
					tmp->text = ostrcat(tmp->text, buf, 1, 0);
				}
			}

			if(treffer == 4)
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
				else if(len > 0 && ostrncmp(ptr, "created_time", len) == 0)
					treffer = 3;
				else if(len > 0 && ostrncmp(ptr, "note_id", len) == 0)
					treffer = 4;
			}
		}
	}

	free(curlretbuf); curlretbuf = NULL;
	curlretbufsize = 0;
	free(buf); buf = NULL;
	return 0;
}

int getfacepicture(struct skin* facebook, struct skin* listbox, struct skin* facefriendslist, char* id)
{
	int ret = -1, i = 0, len = 0, treffer = 0;
	jsmn_parser parser;
	jsmntok_t tokens[FACEMAXTOKEN]; //TODO
	char* buf = NULL, *tmpstr = NULL;
	struct skin* tmp = NULL, *node = NULL;

	delmarkedscreennodes(facebook, 1);
	if(id == NULL) return 1;

	tmpstr = ostrcat(FACEBOOKURL, "/fql?q=SELECT+created,caption,owner,object_id,src+FROM+photo+WHERE+aid=\"", 0, 0);
	tmpstr = ostrcat(tmpstr, id, 1, 0);
	tmpstr = ostrcat(tmpstr, "\"+LIMIT+0,", 1, 0);
	tmpstr = ostrcat(tmpstr, FACEMAXENTRY, 1, 0);
	tmpstr = ostrcat(tmpstr, "&access_token=", 1, 0);
	tmpstr = ostrcat(tmpstr, getconfig("facebooktoken", NULL), 1, 0);
	curlretbuf = gethttps1(tmpstr);
	debug(369, "url=%s", tmpstr);
	debug(369, "ret=%s", curlretbuf);
	free(tmpstr); tmpstr = NULL;
	if(checkfaceerror() == 1) return 1;

	buf = malloc(MINMALLOC);
	if(buf == NULL)
	{
		err("no mem");
		free(curlretbuf); curlretbuf = NULL;
		curlretbufsize = 0;
		return 1;
	}

	jsmn_init(&parser);

	ret = jsmn_parse(&parser, curlretbuf, tokens, FACEMAXTOKEN);
	if(ret == JSMN_SUCCESS)
	{
		for(i = 0; i < FACEMAXTOKEN; i++)
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
					tmp->type = TEXTBOX;
					tmp->wrap = YES;
					tmp->height = 100;
					tmp->textposx = 150;
					tmp->textposx2 = 150;

					strncpy(buf, ptr, len);
					buf[len] = '\0';
					convertfacedate(buf);
					changetext(tmp, buf);
				}
			}

			if(treffer == 2)
			{
				if(tmp != NULL)
				{
					strncpy(buf, ptr, len);
					buf[len] = '\0';
					changefacetext2(tmp, buf);
				}
			}

			if(treffer == 3)
			{
				if(tmp != NULL)
				{
					strncpy(buf, ptr, len);
					buf[len] = '\0';

					node = facefriendslist;
					while(node != NULL)
					{
						if(ostrcmp(node->ret, buf) == 0)
						{
							tmp->text = ostrcat(tmp->text, " - ", 1, 0);
							tmp->text = ostrcat(tmp->text, node->text, 1, 0);
							break;
						}
						node = node->next;
					}
				}
			}

			if(treffer == 4)
			{
				if(tmp != NULL)
				{
					strncpy(buf, ptr, len);
					buf[len] = '\0';
					changeret(tmp, buf);
				}
			}

			if(treffer == 5)
			{
				if(tmp != NULL)
				{
					char* tmppic = NULL;
					char* tmpbuf = NULL;

					strncpy(buf, ptr, len);
					buf[len] = '\0';
					tmpbuf = string_replace_all("\\", "", buf, 0);

					tmppic = ostrcat(FACEBOOKPATH, "/", 0, 0);
					tmppic = ostrcat(tmppic, tmp->ret, 1, 0);
					if(ostrstr(tmpbuf, ".jpg") != NULL)
						tmppic = ostrcat(tmppic, ".jpg", 1, 0);
					else
						tmppic = ostrcat(tmppic, ".png", 1, 0);
					getfacefoto(tmpbuf, tmppic);
					changepic(tmp, tmppic);
					free(tmppic); tmppic = NULL;
					free(tmpbuf); tmpbuf = NULL;
				}
			}

			treffer = 0;
			if(tokens[i].type == 3)
			{
				if(len > 0 && ostrncmp(ptr, "created", len) == 0)
					treffer = 1;
				if(len > 0 && ostrncmp(ptr, "caption", len) == 0)
					treffer = 2;
				else if(len > 0 && ostrncmp(ptr, "owner", len) == 0)
					treffer = 3;
				else if(len > 0 && ostrncmp(ptr, "object_id", len) == 0)
					treffer = 4;
				else if(len > 0 && ostrncmp(ptr, "src", len) == 0)
					treffer = 5;
			}
		}
	}

	free(curlretbuf); curlretbuf = NULL;
	curlretbufsize = 0;
	free(buf); buf = NULL;
	return 0;
}

int getfacealbum(struct skin* facebook, struct skin* listbox, struct skin* facefriendslist, char* id)
{
	int ret = -1, i = 0, len = 0, treffer = 0;
	jsmn_parser parser;
	jsmntok_t tokens[FACEMAXTOKEN]; //TODO
	char* buf = NULL, *tmpstr = NULL;
	struct skin* tmp = NULL, *node = NULL;

	delmarkedscreennodes(facebook, 1);
	if(id == NULL) return 1;

	tmpstr = ostrcat(FACEBOOKURL, "/fql?q=SELECT+name,owner,aid+FROM+album+WHERE+owner=", 0, 0);
	tmpstr = ostrcat(tmpstr, id, 1, 0);
	tmpstr = ostrcat(tmpstr, "+LIMIT+0,", 1, 0);
	tmpstr = ostrcat(tmpstr, FACEMAXENTRY, 1, 0);
	tmpstr = ostrcat(tmpstr, "&access_token=", 1, 0);
	tmpstr = ostrcat(tmpstr, getconfig("facebooktoken", NULL), 1, 0);
	curlretbuf = gethttps1(tmpstr);
	debug(369, "url=%s", tmpstr);
	debug(369, "ret=%s", curlretbuf);
	free(tmpstr); tmpstr = NULL;
	if(checkfaceerror() == 1) return 1;

	buf = malloc(MINMALLOC);
	if(buf == NULL)
	{
		err("no mem");
		free(curlretbuf); curlretbuf = NULL;
		curlretbufsize = 0;
		return 1;
	}

	jsmn_init(&parser);

	ret = jsmn_parse(&parser, curlretbuf, tokens, FACEMAXTOKEN);
	if(ret == JSMN_SUCCESS)
	{
		for(i = 0; i < FACEMAXTOKEN; i++)
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
					changefacetext(tmp, buf);
				}
			}

			if(treffer == 2)
			{
				if(tmp != NULL)
				{
					strncpy(buf, ptr, len);
					buf[len] = '\0';

					node = facefriendslist;
					while(node != NULL)
					{
						if(ostrcmp(node->ret, buf) == 0)
						{
							tmp->text = ostrcat(tmp->text, " - ", 1, 0);
							tmp->text = ostrcat(tmp->text, node->text, 1, 0);
							break;
						}
						node = node->next;
					}
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
				if(len > 0 && ostrncmp(ptr, "name", len) == 0)
					treffer = 1;
				if(len > 0 && ostrncmp(ptr, "owner", len) == 0)
					treffer = 2;
				if(len > 0 && ostrncmp(ptr, "aid", len) == 0)
					treffer = 3;
			}
		}
	}

	free(curlretbuf); curlretbuf = NULL;
	curlretbufsize = 0;
	free(buf); buf = NULL;
	return 0;
}

int getfacecomment(struct skin* facecomment, struct skin* listbox, struct skin* facefriendslist, char* id)
{
	int ret = -1, i = 0, len = 0, treffer = 0;
	jsmn_parser parser;
	jsmntok_t tokens[FACEMAXTOKEN]; //TODO
	char* buf = NULL, *tmpstr = NULL;
	struct skin* tmp = NULL, *node = NULL;

	delmarkedscreennodes(facecomment, 1);
	if(id == NULL) return 1;

	tmpstr = ostrcat(FACEBOOKURL, "/fql?q=SELECT+time,text,fromid,object_id+FROM+comment+WHERE+object_id=", 0, 0);
	tmpstr = ostrcat(tmpstr, id, 1, 0);
	tmpstr = ostrcat(tmpstr, "+LIMIT+0,", 1, 0);
	tmpstr = ostrcat(tmpstr, FACEMAXENTRY, 1, 0);
	tmpstr = ostrcat(tmpstr, "&access_token=", 1, 0);
	tmpstr = ostrcat(tmpstr, getconfig("facebooktoken", NULL), 1, 0);
	curlretbuf = gethttps1(tmpstr);
	debug(369, "url=%s", tmpstr);
	debug(369, "ret=%s", curlretbuf);
	free(tmpstr); tmpstr = NULL;
	if(checkfaceerror() == 1) return 1;

	buf = malloc(MINMALLOC);
	if(buf == NULL)
	{
		err("no mem");
		free(curlretbuf); curlretbuf = NULL;
		curlretbufsize = 0;
		return 1;
	}

	jsmn_init(&parser);

	ret = jsmn_parse(&parser, curlretbuf, tokens, FACEMAXTOKEN);
	if(ret == JSMN_SUCCESS)
	{
		for(i = 0; i < FACEMAXTOKEN; i++)
		{
			if(tokens[i].start == -1) break;

			len = tokens[i].end - tokens[i].start;
			if(len > MINMALLOC) len = MINMALLOC;
			char* ptr = curlretbuf + tokens[i].start;

			if(treffer == 1)
			{
				tmp = addlistbox(facecomment, listbox, tmp, 1);
				if(tmp != NULL)
				{
					tmp->type = TEXTBOX;
					tmp->wrap = YES;
					tmp->height = 100;
					strncpy(buf, ptr, len);
					buf[len] = '\0';
					convertfacedate(buf);
					changetext(tmp, buf);
				}
			}

			if(treffer == 2)
			{
				if(tmp != NULL)
				{
					strncpy(buf, ptr, len);
					buf[len] = '\0';
					changefacetext2(tmp, buf);
				}
			}

			if(treffer == 3)
			{
				if(tmp != NULL)
				{
					strncpy(buf, ptr, len);
					buf[len] = '\0';
					
					node = facefriendslist;
					while(node != NULL)
					{
						if(ostrcmp(node->ret, buf) == 0)
						{
							tmp->text = ostrcat(tmp->text, " - ", 1, 0);
							tmp->text = ostrcat(tmp->text, node->text, 1, 0);
							break;
						}
						node = node->next;
					}
				}
			}

			if(treffer == 4)
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
				if(len > 0 && ostrncmp(ptr, "time", len) == 0)
					treffer = 1;
				if(len > 0 && ostrncmp(ptr, "text", len) == 0)
					treffer = 2;
				if(len > 0 && ostrncmp(ptr, "fromid", len) == 0)
					treffer = 3;
				if(len > 0 && ostrncmp(ptr, "object_id", len) == 0)
					treffer = 4;
			}
		}
	}

	free(curlretbuf); curlretbuf = NULL;
	curlretbufsize = 0;
	free(buf); buf = NULL;
	return 0;
}

int getfacesubcomment(struct skin* facesubcomment, struct skin* listbox, char* id)
{
	int ret = -1, i = 0, len = 0, treffer = 0;
	jsmn_parser parser;
	jsmntok_t tokens[FACEMAXTOKEN]; //TODO
	char* buf = NULL, *tmpstr = NULL;
	struct skin* tmp = NULL;

	delmarkedscreennodes(facesubcomment, 1);
	if(id == NULL) return 1;

	tmpstr = ostrcat(FACEBOOKURL, "/fql?q=SELECT+comments+FROM+comment+WHERE+object_id=", 0, 0);
	tmpstr = ostrcat(tmpstr, id, 1, 0);
	tmpstr = ostrcat(tmpstr, "+LIMIT+0,", 1, 0);
	tmpstr = ostrcat(tmpstr, FACEMAXENTRY, 1, 0);
	tmpstr = ostrcat(tmpstr, "&access_token=", 1, 0);
	tmpstr = ostrcat(tmpstr, getconfig("facebooktoken", NULL), 1, 0);
	curlretbuf = gethttps1(tmpstr);
	debug(369, "url=%s", tmpstr);
	debug(369, "ret=%s", curlretbuf);
	free(tmpstr); tmpstr = NULL;
	if(checkfaceerror() == 1) return 1;

	buf = malloc(MINMALLOC);
	if(buf == NULL)
	{
		err("no mem");
		free(curlretbuf); curlretbuf = NULL;
		curlretbufsize = 0;
		return 1;
	}

	jsmn_init(&parser);

	ret = jsmn_parse(&parser, curlretbuf, tokens, FACEMAXTOKEN);
	if(ret == JSMN_SUCCESS)
	{
		for(i = 0; i < FACEMAXTOKEN; i++)
		{
			if(tokens[i].start == -1) break;

			len = tokens[i].end - tokens[i].start;
			if(len > MINMALLOC) len = MINMALLOC;
			char* ptr = curlretbuf + tokens[i].start;

			if(treffer == 1)
			{
				tmp = addlistbox(facesubcomment, listbox, tmp, 1);
				if(tmp != NULL)
				{
					tmp->type = TEXTBOX;
					tmp->wrap = YES;
					tmp->height = 100;
					strncpy(buf, ptr, len);
					buf[len] = '\0';
					changefacetext(tmp, buf);
				}
			}

			if(treffer == 2)
			{
				if(tmp != NULL)
				{
					strncpy(buf, ptr, len);
					buf[len] = '\0';
					changefacetext2(tmp, buf);
				}
			}

			if(treffer == 3)
			{
				if(tmp != NULL)
				{
					strncpy(buf, ptr, len);
					buf[len] = '\0';
					convertfacedate(buf);

					if(tmp->text != NULL && strlen(tmp->text) != 0)
						tmp->text = ostrcat(" - ", tmp->text, 0, 1);
					tmp->text = ostrcat(buf, tmp->text, 0, 1);
				}
			}

			treffer = 0;
			if(tokens[i].type == 3)
			{
				if(len > 0 && ostrncmp(ptr, "name", len) == 0)
					treffer = 1;
				if(len > 0 && ostrncmp(ptr, "message", len) == 0)
					treffer = 2;
				if(len > 0 && ostrncmp(ptr, "created_time", len) == 0)
					treffer = 3;
			}
		}
	}

	free(curlretbuf); curlretbuf = NULL;
	curlretbufsize = 0;
	free(buf); buf = NULL;
	return 0;
}

void screenfacesubcomment(char* id)
{
	int rcret = -1, ret = 0;

	struct skin* facesubcomment = getscreen("facesubcomment");
	struct skin* listbox = getscreennode(facesubcomment, "listbox");
	struct skin* load = getscreen("loading");
	char* pos = NULL;

	if(id == NULL) return;
	pos = strchr(id, '_');
	if(pos != NULL)
		pos = pos + 1;
	else
		pos = id;

	drawscreen(load, 0, 0);
	ret = getfacesubcomment(facesubcomment, listbox, pos);
	clearscreen(load);
	if(ret == 1) return;

	drawscreen(facesubcomment, 0, 0);
	addscreenrc(facesubcomment, listbox);

	while(1)
	{
		rcret = waitrc(facesubcomment, 0, 0);

		if(rcret == getrcconfigint("rcexit", NULL)) break;
		if(rcret == getrcconfigint("rcok", NULL)) break;
	}

	delownerrc(facesubcomment);
	delmarkedscreennodes(facesubcomment, 1);
	clearscreen(facesubcomment);
}

void screenfacecomment(struct skin* facefriendslist, char* id)
{
	int rcret = -1, ret = 0;

	struct skin* facecomment = getscreen("facecomment");
	struct skin* listbox = getscreennode(facecomment, "listbox");
	struct skin* load = getscreen("loading");
	char* pos = NULL;

	if(id == NULL) return;
	pos = strchr(id, '_');
	if(pos != NULL)
		pos = pos + 1;
	else
		pos = id;

	drawscreen(load, 0, 0);
	ret = getfacecomment(facecomment, listbox, facefriendslist, pos);
	clearscreen(load);
	if(ret == 1) return;

	drawscreen(facecomment, 0, 0);
	addscreenrc(facecomment, listbox);

	while(1)
	{
		rcret = waitrc(facecomment, 0, 0);

		if(rcret == getrcconfigint("rcexit", NULL)) break;
		if(rcret == getrcconfigint("rcok", NULL) && listbox->select != NULL)
		{
			screenfacesubcomment(listbox->select->ret);
			drawscreen(facecomment, 0, 0);
		}
	}

	delownerrc(facecomment);
	delmarkedscreennodes(facecomment, 1);
	clearscreen(facecomment);
}

void screenfacepicture(struct skin* facefriendslist, char* id)
{
	int rcret = -1, ret = 0;

	struct skin* facepicture = getscreen("facepicture");
	struct skin* listbox = getscreennode(facepicture, "listbox");
	struct skin* load = getscreen("loading");

	if(id == NULL) return;

	drawscreen(load, 0, 0);
	ret = getfacepicture(facepicture, listbox, facefriendslist, id);
	clearscreen(load);
	if(ret == 1) return;

	drawscreen(facepicture, 0, 0);
	addscreenrc(facepicture, listbox);

	while(1)
	{
		rcret = waitrc(facepicture, 0, 0);

		if(rcret == getrcconfigint("rcexit", NULL)) break;
		if(rcret == getrcconfigint("rcok", NULL) && listbox->select != NULL)
		{
			screenfacecomment(facefriendslist, listbox->select->ret);
			drawscreen(facepicture, 0, 0);
		}
	}

	delownerrc(facepicture);
	delmarkedscreennodes(facepicture, 1);
	clearscreen(facepicture);
}

void screenface()
{
	if(getconfig("facebooktoken", NULL) == NULL)
		screenfaceregister();

	int rcret = -1, aktview = 0, aktlist = 0, aktuser = 1;

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
	struct skin* load = getscreen("loading");

	drawscreen(load, 0, 0);
	mkdir(FACEBOOKPATH, 0777);

	getfacefriens(facefriends, facefriendslist, "me()");
	getfaceuser(facename, facestatus, facegender, facefoto, "me()");
	getfacestream(facebook, facebooklist, "me()");
	clearscreen(load);

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

			if(aktuser != facefriendslist->aktline)
			{
				drawscreen(load, 0, 0);
				facebooklist->aktline = 1;
				facebooklist->aktpage = -1;
				getfaceuser(facename, facestatus, facegender, facefoto, facefriendslist->select->ret);
				if(aktview == 0)
					getfacestream(facebook, facebooklist, facefriendslist->select->ret);
				else if(aktview == 1)
					getfacenote(facebook, facebooklist, facefriendslist->select->ret);
				else if(aktview == 2)
					getfacealbum(facebook, facebooklist, facefriendslist, facefriendslist->select->ret);
				clearscreen(load);

				drawscreen(faceuser, 0, 1);
				drawscreen(facebook, 0, 1);
				drawscreen(facefoto, 0, 0);

				aktuser = facefriendslist->aktline;
			}

			continue;
		}

		if(rcret == getrcconfigint("rcexit", NULL)) break;
		if(rcret == getrcconfigint("rcred", NULL))
		{
			if(aktview != 0)
			{
				drawscreen(load, 0, 0);
				facebooklist->aktline = 1;
				facebooklist->aktpage = -1;
				getfacestream(facebook, facebooklist, facefriendslist->select->ret);
				clearscreen(load);
				drawscreen(facebook, 0, 0);
			}
			aktview = 0;
		}
		if(rcret == getrcconfigint("rcgreen", NULL))
		{
			if(aktview != 1)
			{
				drawscreen(load, 0, 0);
				facebooklist->aktline = 1;
				facebooklist->aktpage = -1;
				getfacenote(facebook, facebooklist, facefriendslist->select->ret);
				clearscreen(load);
				drawscreen(facebook, 0, 0);
			}
			aktview = 1;
		}
		if(rcret == getrcconfigint("rcyellow", NULL))
		{
			if(aktview != 2)
			{
				drawscreen(load, 0, 0);
				facebooklist->aktline = 1;
				facebooklist->aktpage = -1;
				getfacealbum(facebook, facebooklist, facefriendslist, facefriendslist->select->ret);
				clearscreen(load);
				drawscreen(facebook, 0, 0);
			}
			aktview = 2;
		}
		if(aktlist == 0 && rcret == getrcconfigint("rcok", NULL) && facefriendslist->select != NULL)
		{
			drawscreen(load, 0, 0);
			facebooklist->aktline = 1;
			facebooklist->aktpage = -1;
			getfaceuser(facename, facestatus, facegender, facefoto, facefriendslist->select->ret);
			if(aktview == 0)
				getfacestream(facebook, facebooklist, facefriendslist->select->ret);
			else if(aktview == 1)
				getfacenote(facebook, facebooklist, facefriendslist->select->ret);
			else if(aktview == 2)
				getfacealbum(facebook, facebooklist, facefriendslist, facefriendslist->select->ret);
			clearscreen(load);

			drawscreen(faceuser, 0, 1);
			drawscreen(facebook, 0, 1);
			drawscreen(facefoto, 0, 0);

			aktuser = facefriendslist->aktline;

		}
		if(aktlist == 1 && rcret == getrcconfigint("rcok", NULL) && facebooklist->select != NULL)
		{
			if(aktview == 2)
				screenfacepicture(facefriendslist, facebooklist->select->ret);
			else
				screenfacecomment(facefriendslist, facebooklist->select->ret);

			drawscreen(facetitle, 0, 1);
			drawscreen(facefriends, 0, 1);
			drawscreen(facefoto, 0, 1);
			drawscreen(facebutton, 0, 1);
			drawscreen(faceuser, 0, 1);
			drawscreen(facebook, 0, 0);
		}
	}

	delallfiles(FACEBOOKPATH, ".jpg");
	delallfiles(FACEBOOKPATH, ".png");
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
