#ifndef FACEBOOK_H
#define FACEBOOK_H

#include <curl/curl.h>

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

	curlretbuf = gethttps("https://graph.facebook.com/oauth/device?type=device_code&client_id=150792241632891");
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

	tmpstr = getxmlentry(curlretbuf, "\"code\":");
	tmpstr = ostrcat("https://graph.facebook.com/oauth/device?type=device_token&client_id=150792241632891&code=", tmpstr, 0, 1);

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

void screenfaceregister()
{
	if(getconfig("facebooktoken", NULL) == NULL)
		screenfaceregister();

	int rcret = -1;

	struct skin* facebook = getscreen("facebook");
	struct skin* listbox = getscreennode(facebook, "listbox");
	struct skin* info = getscreennode(facebook, "info");
	struct skin* text = getscreennode(facebook, "text");

	drawscreen(facebook, 0, 0);
	addscreenrc(facebook, listbox);

	while(1)
	{
		if(rcret == getrcconfigint("rcexit", NULL)) break;
	}

	delownerrc(facebook);
	clearscreen(facebook);
}

#endif
