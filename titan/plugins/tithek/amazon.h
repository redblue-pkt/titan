#ifndef AMAZON_H
#define AMAZON_H

#include <curl/curl.h>

int curlretbufsize = 0;
char* curlretbuf = NULL;

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

char* gethttps_post(char* url, char* data, char* cookie)
{
	CURL *curl;
	CURLcode res;

	free(curlretbuf); curlretbuf = NULL;
	curlretbufsize = 0;

	curl = curl_easy_init();
	if(curl)
	{
		curl_easy_setopt(curl, CURLOPT_URL, url);
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data);
//		curl_easy_setopt(curl, CURLOPT_POST, 1);
		curl_easy_setopt(curl, CURLOPT_HEADER, 1L);
		curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 5);
		curl_easy_setopt(curl, CURLOPT_TIMEOUT, 20);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curl_write);
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
//		curl_easy_setopt(curl, CURLOPT_VERBOSE, 1);
		curl_easy_setopt(curl, CURLOPT_COOKIEFILE, "/var/usr/local/share/titan/plugins/tithek/cookie.txt");
		curl_easy_setopt(curl, CURLOPT_COOKIEJAR, "/var/usr/local/share/titan/plugins/tithek/cookie.txt");
		curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
		curl_easy_setopt(curl, CURLOPT_AUTOREFERER, 1L);

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

char* gethttps_get(char* url, char* data, char* cookie)
{
	CURL *curl;
	CURLcode res;

	free(curlretbuf); curlretbuf = NULL;
	curlretbufsize = 0;

	struct curl_slist *list = NULL;

	curl = curl_easy_init();
	if(curl)
	{
		curl_easy_setopt(curl, CURLOPT_URL, url);
		curl_easy_setopt(curl, CURLOPT_HTTPGET, 1L);
		curl_easy_setopt(curl, CURLOPT_HEADER, 1L);
		curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 5);
		curl_easy_setopt(curl, CURLOPT_TIMEOUT, 20);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curl_write);
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
//		curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
		curl_easy_setopt(curl, CURLOPT_COOKIEFILE, "/var/usr/local/share/titan/plugins/tithek/cookie.txt");
		curl_easy_setopt(curl, CURLOPT_COOKIEJAR, "/var/usr/local/share/titan/plugins/tithek/cookie.txt");
		curl_easy_setopt(curl, CURLOPT_ACCEPT_ENCODING, "identity");
		curl_easy_setopt(curl, CURLOPT_USERAGENT, "Mozilla/5.0 (X11; U; Linux i686; en-EN) AppleWebKit/533.4 (KHTML, like Gecko) Chrome/5.0.375.127 Large Screen Safari/533.4 GoogleTV/ 162671");
		curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
		curl_easy_setopt(curl, CURLOPT_AUTOREFERER, 1L);

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

int checkamazonerror()
{
	if(curlretbuf == NULL || strstr(curlretbuf, "\"error\":") != NULL)
	{
		textbox(_("Message"), _("Can't get data from Amazon"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 0, 0);
		free(curlretbuf); curlretbuf = NULL;
		curlretbufsize = 0;
		return 1;
	}
	return 0;
}

char* amazon_hoster(char* link)
{
	int debuglevel = getconfigint("debuglevel", NULL);

printf("amazon hoster...\n");
	debug(99, "link: %s", link);
	char* streamurl = NULL;

	streamurl = hoster(link);
	debug(99, "streamurl1: %s", streamurl);

	streamurl = string_replace_all("amp;", "", streamurl, 1);
	debug(99, "streamurl2: %s", streamurl);

	return streamurl;
}

int random_number(int min_num, int max_num);

int random_number(int min_num, int max_num)
{
    int result=0,low_num=0,hi_num=0;
    if(min_num<max_num)
    {
        low_num=min_num;
        hi_num=max_num+1; // this is done to include max_num in output.
    }
    else
    {
        low_num=max_num+1;// this is done to include max_num in output.
        hi_num=min_num;
    }
    srand(time(NULL));
    result = (rand()%(hi_num-low_num))+low_num;
    return result;
}
        
char* amazon(char* link)
{
	int debuglevel = getconfigint("debuglevel", NULL);
	char* url = NULL, *customerid = NULL, *marketplaceid = NULL, *matchtoken = NULL, *devicetypeid = NULL, *apimain = NULL, *token = NULL;
	char* tmpstr = NULL, *title = NULL, *pic = NULL, *streamurl = NULL, *bitrate = NULL;

	unlink("/tmp/amazon_streamurl_get1");
	unlink("/tmp/amazon_streamurl_get2");
	unlink("/tmp/amazon_streamurl_get3");
	unlink("/tmp/amazon_streamurl_get4");

	url = ostrcat("http://www.amazon.de/dp/", link, 0, 0);
	url = ostrcat(url, "/?_encoding=UTF8", 1, 0);

//////////////////////////////////////////////////
	debug(99, "url: %s", url);
	curlretbuf = gethttps_get(url, NULL, NULL);
	titheklog(debuglevel, "/tmp/amazon_streamurl_get1", NULL, NULL, NULL, curlretbuf);	
	free(url), url = NULL;
//////////////////////////////////////////////////	

	customerid = string_resub("\"customerID\":\"", "\"", curlretbuf, 0);
	marketplaceid = string_resub("\"marketplaceID\":\"", "\"", curlretbuf, 0);
	matchtoken = string_resub("\"csrfToken\":\"", "\"", curlretbuf, 0);
	devicetypeid = ostrcat("A35LWR0L7KC0TJ", NULL, 0, 0);

//	apiMain = ["atv-ps", "atv-ps-eu", "atv-ps-eu"][int(siteVersion)]
	apimain = ostrcat("atv-ps-eu", NULL, 0, 0);

	url = ostrcat("https://", apimain, 0, 0);
	url = ostrcat(url, ".amazon.com/cdp/catalog/GetASINDetails?version=2&format=json&asinlist=", 1, 0);
	url = ostrcat(url, link, 1, 0);
	url = ostrcat(url, "&deviceID=", 1, 0);
	url = ostrcat(url, customerid, 1, 0);
	url = ostrcat(url, "&includeRestrictions=true&deviceTypeID=", 1, 0);
	url = ostrcat(url, devicetypeid, 1, 0);
	url = ostrcat(url, "&firmware=WIN%2017,0,0,188%20PlugIn&NumberOfResults=1", 1, 0);

	printf("customerid: %s\n", customerid);
	printf("marketplaceid: %s\n", marketplaceid);
	printf("matchtoken: %s\n", matchtoken);
	printf("devicetypeid: %s\n", devicetypeid);
	printf("apimain: %s\n", apimain);

//////////////////////////////////////////////////	
	debug(99, "url: %s", url);
	curlretbuf = gethttps_get(url, NULL, NULL);
	titheklog(debuglevel, "/tmp/amazon_streamurl_get2", NULL, NULL, NULL, curlretbuf);	
	free(url), url = NULL;
//////////////////////////////////////////////////	

	url = ostrcat("https://", "www.amazon.de", 0, 0);
	url = ostrcat(url, "/gp/video/streaming/player-token.json?callback=jQuery1640", 1, 0);
	// todo
//	url = ostrcat(url, "563195325634321267", 1, 0); //join(random.choice(string.digits) for x in range(18))
	url = ostrcat(url, ollutoa(random_number(100000000,999999999)), 1, 1); //join(random.choice(string.digits) for x in range(18))
	url = ostrcat(url, ollutoa(random_number(100000000,999999999)), 1, 1); //join(random.choice(string.digits) for x in range(18))
	url = ostrcat(url, "_", 1, 0);
	// todo
//	url = ostrcat(url, "1441140895131", 1, 0); //str(int(time.time()*1000))
	url = ostrcat(url, olutoa(time(NULL)*1000), 1, 1);
	url = ostrcat(url, "&csrftoken=", 1, 0);
	url = ostrcat(url, matchtoken, 1, 0);
	url = ostrcat(url, "_", 1, 0);
	// todo
//	url = ostrcat(url, "1441140895131", 1, 0); //str(int(time.time()*1000))
	url = ostrcat(url, olutoa(time(NULL)*1000), 1, 1);

//////////////////////////////////////////////////	
	debug(99, "url: %s", url);
	curlretbuf = gethttps_get(url, NULL, NULL);
	titheklog(debuglevel, "/tmp/amazon_streamurl_get3", NULL, NULL, NULL, curlretbuf);	
	free(url), url = NULL;
//////////////////////////////////////////////////	

	token = string_resub("\"token\":\"", "\"", curlretbuf, 0);
	debug(99, "token: %s", token);

	url = ostrcat("https://", apimain, 0, 0);
	url = ostrcat(url, ".amazon.com/cdp/catalog/GetStreamingUrlSets?version=1&format=json&firmware=WIN%2011,7,700,224%20PlugIn&marketplaceID=", 1, 0);
// trailer
//	url = ostrcat(url, ".amazon.com/cdp/catalog/GetStreamingTrailerUrls?version=1&format=json&firmware=WIN%2011,7,700,224%20PlugIn&marketplaceID=", 1, 0);
	url = ostrcat(url, marketplaceid, 1, 0);
	url = ostrcat(url, "&token=", 1, 0);
	url = ostrcat(url, token, 1, 0);
	url = ostrcat(url, "&deviceTypeID=", 1, 0);
	url = ostrcat(url, devicetypeid, 1, 0);
	url = ostrcat(url, "&asin=", 1, 0);
	url = ostrcat(url, link, 1, 0);
	url = ostrcat(url, "&customerID=", 1, 0);
	url = ostrcat(url, customerid, 1, 0);
	url = ostrcat(url, "&deviceID=", 1, 0);
	// todo
//	url = ostrcat(url, "A3T8NY6VBAVZZA1441140895392B00HDWUOE0", 1, 0); //'&deviceID='+urllib.quote_plus(matchCID[0].encode("utf8"))+str(int(time.time()*1000))+videoID
	url = ostrcat(url, customerid, 1, 0);
	url = ostrcat(url, olutoa(time(NULL)*1000), 1, 1);
	url = ostrcat(url, link, 1, 0);

//////////////////////////////////////////////////	
	debug(99, "url: %s", url);
	curlretbuf = gethttps_get(url, NULL, NULL);
	titheklog(debuglevel, "/tmp/amazon_streamurl_get4", NULL, NULL, NULL, curlretbuf);	
	free(url), url = NULL;
//////////////////////////////////////////////////	

	tmpstr = string_replace_all("{\"bitrate\":", "\n{\"bitrate\":", curlretbuf, 0);

	int count = 0, i = 0;	
	struct splitstr* ret1 = NULL;
	struct menulist* mlist = NULL, *mbox = NULL;
	ret1 = strsplit(string_decode(tmpstr, 0), "\n", &count);
	for(i = 0; i < count; i++)
	{
		if(ostrstr(ret1[i].part, "f4m") != NULL)
			pic = ostrcat("f4m.png", NULL, 0, 0);
		else if(ostrstr(ret1[i].part, "mp4") != NULL)
			pic = ostrcat("mp4.png", NULL, 0, 0);
		else if(ostrstr(ret1[i].part, "3gp") != NULL)
			pic = ostrcat("3gp.png", NULL, 0, 0);
		else if(ostrstr(ret1[i].part, "flv") != NULL)
			pic = ostrcat("flv.png", NULL, 0, 0);
		bitrate = string_resub("\"bitrate\":", ",", ret1[i].part, 0);
		streamurl = string_resub("\"url\":\"", "\"", ret1[i].part, 0);
		streamurl = string_replace("rtmpe://azeufms-vodfs.fplive.net/azeufms-vod/mp4:", "http://azeufms-vodfs.fplive.net/", streamurl, 1);

		if(ostrstr(streamurl, "http://") != NULL)
		{
			title = ostrcat("Http Stream (Bitrate ", NULL, 0, 0);
			title = ostrcat(title, bitrate, 1, 0);
			title = ostrcat(title, ")", 1, 0);

			debug(99, "(%d) title: %s streamurl: %s\n", i, title, streamurl);																									
			addmenulist(&mlist, title, streamurl, pic, 0, 0);
		}

		free(bitrate), bitrate = NULL;
		free(title), title = NULL;
		free(pic), pic = NULL;
		free(streamurl), streamurl = NULL;
	}
	free(ret1), ret1 = NULL;

	if(mlist != NULL)
	{
		mbox = menulistbox(mlist, NULL, _("Stream Menu"), _("Choose your Streaming Format from the following list"), NULL, NULL, 1, 0);
		if(mbox != NULL)
		{
			free(streamurl), streamurl = NULL;
			debug(99, "mbox->name %s", mbox->name);
			debug(99, "mbox->text %s", mbox->text);
			streamurl = ostrcat(mbox->text, NULL, 0, 0);
		}
	}

	free(url), url = NULL;
	free(customerid), customerid = NULL;
	free(marketplaceid), marketplaceid = NULL;
	free(matchtoken), matchtoken = NULL;
	free(devicetypeid), devicetypeid = NULL;
	free(apimain), apimain = NULL;
	free(token), token = NULL;
	free(tmpstr), tmpstr = NULL;
	free(title), title = NULL;
	free(pic), pic = NULL;
	free(bitrate), bitrate = NULL;

///////////////////////////////////
	free(curlretbuf); curlretbuf = NULL;
	curlretbufsize = 0;
///////////////////////////////////

	debug(99, "streamurl: %s", streamurl);
	return streamurl;
}

int login()
{
	int ret = 0;
	int debuglevel = getconfigint("debuglevel", NULL);

	char* hash = NULL;

	char* user = getconfig("amazon_user", NULL);
	char* pass = getconfig("amazon_pass", NULL);

	unlink("/tmp/amazon_tmpstr_get1_blank");
	unlink("/tmp/amazon_tmpstr_get2_sign-in");
	unlink("/tmp/amazon_tmpstr_post1");
	unlink("/tmp/amazon_tmpstr_get3_blank");
	unlink("/tmp/amazon_tmpstr_get4_blank");

// logout
//	curlretbuf = gethttps_get("https://www.amazon.de/ap/signin?_encoding=UTF8&openid.assoc_handle=deflex&openid.claimed_id=http%3A%2F%2Fspecs.openid.net%2Fauth%2F2.0%2Fidentifier_select&openid.identity=http%3A%2F%2Fspecs.openid.net%2Fauth%2F2.0%2Fidentifier_select&openid.mode=checkid_setup&openid.ns=http%3A%2F%2Fspecs.openid.net%2Fauth%2F2.0&openid.ns.pape=http%3A%2F%2Fspecs.openid.net%2Fextensions%2Fpape%2F1.0&openid.pape.max_auth_age=0&openid.return_to=https%3A%2F%2Fwww.amazon.de%2Fgp%2Fyourstore%2Fhome%3Fie%3DUTF8%26action%3Dsign-out%26path%3D%252Fgp%252Fyourstore%252Fhome%26ref_%3Dnav__gno_signout%26signIn%3D1%26useRedirectOnSuccess%3D1", NULL, NULL);
//	titheklog(debuglevel, "/tmp/amazon_tmpstr_get0_logout", NULL, NULL, NULL, curlretbuf);	

	curlretbuf = gethttps_get("https://www.amazon.de/", NULL, NULL);
//	debug(99, "tmpstr: %s", curlretbuf);
	titheklog(debuglevel, "/tmp/amazon_tmpstr_get1_blank", NULL, NULL, NULL, curlretbuf);	

	char* login = string_resub("'config.signOutText',", ");", curlretbuf, 0);	
//	debug(99, "login: %s", login);
	strstrip(login);
//	debug(99, "login: %s", login);

	if(ostrcmp("null", login) == 0)
	{
		debug(99, "Amazon Login Error");
		ret = 0;
	}
	else
	{
		debug(99, "Amazon Login Successful: %s", login);
		ret = 1;
	}
	free(login), login = NULL;

	if(ret == 0)
	{
		curlretbuf = gethttps_get("https://www.amazon.de/gp/sign-in.html", NULL, NULL);
		titheklog(debuglevel, "/tmp/amazon_tmpstr_get2_sign-in", NULL, NULL, NULL, curlretbuf);	

		char* pos1 = ostrstr(curlretbuf, "<input type=\"hidden\" name=\"appActionToken\" value=");
		char* tmp1 = getxmlentry(pos1, "value=");
		debug(99, "appActionToken: %s", tmp1);
	
		char* pos2 = ostrstr(curlretbuf, "<input type=\"hidden\" name=\"appAction\" value=");
		char* tmp2 = getxmlentry(pos2, "value=");
		debug(99, "appActionToken: %s", tmp2);
	
		char* pos3 = ostrstr(curlretbuf, "<input type=\"hidden\" name=\"openid.pape.max_auth_age\" value=");
		char* tmp3 = getxmlentry(pos3, "value=");
		debug(99, "openid.pape.max_auth_age: %s", tmp3);
	
		char* pos4 = ostrstr(curlretbuf, "<input type=\"hidden\" name=\"openid.return_to\" value=");
		char* tmp4 = getxmlentry(pos4, "value=");
		debug(99, "openid.return_to: %s", tmp4);
	
		char* pos5 = ostrstr(curlretbuf, "<input type=\"hidden\" name=\"prevRID\" value=");
		char* tmp5 = getxmlentry(pos5, "value=");
		debug(99, "prevRID: %s", tmp5);
	
		char* pos6 = ostrstr(curlretbuf, "<input type=\"hidden\" name=\"openid.identity\" value=");
		char* tmp6 = getxmlentry(pos6, "value=");
		debug(99, "openid.identity: %s", tmp6);
	
		char* pos7 = ostrstr(curlretbuf, "<input type=\"hidden\" name=\"openid.assoc_handle\" value=");
		char* tmp7 = getxmlentry(pos7, "value=");
		debug(99, "openid.assoc_handle: %s", tmp7);
	
		char* pos8 = ostrstr(curlretbuf, "<input type=\"hidden\" name=\"openid.mode\" value=");
		char* tmp8 = getxmlentry(pos8, "value=");
		debug(99, "openid.mode: %s", tmp8);
	
		char* pos9 = ostrstr(curlretbuf, "<input type=\"hidden\" name=\"openid.ns.pape\" value=");
		char* tmp9 = getxmlentry(pos9, "value=");
		debug(99, "openid.ns.pape: %s", tmp9);
	
		char* pos10 = ostrstr(curlretbuf, "<input type=\"hidden\" name=\"openid.claimed_id\" value=");
		char* tmp10 = getxmlentry(pos10, "value=");
		debug(99, "openid.claimed_id: %s", tmp10);
	
		char* pos11 = ostrstr(curlretbuf, "<input type=\"hidden\" name=\"pageId\" value=");
		char* tmp11 = getxmlentry(pos11, "value=");
		debug(99, "pageId: %s", tmp11);
	
		char* pos12 = ostrstr(curlretbuf, "<input type=\"hidden\" name=\"openid.ns\" value=");
		char* tmp12 = getxmlentry(pos12, "value=");
		debug(99, "openid.ns: %s", tmp12);
	
		hash = ostrcat(hash, "appActionToken=", 1, 0);
		hash = ostrcat(hash, tmp1, 1, 1);
		hash = ostrcat(hash, "&appAction=", 1, 0);
		hash = ostrcat(hash, tmp2, 1, 1);
		hash = ostrcat(hash, "&openid.pape.max_auth_age=", 1, 0);
		hash = ostrcat(hash, tmp3, 1, 1);
		hash = ostrcat(hash, "&openid.return_to=", 1, 0);
		hash = ostrcat(hash, tmp4, 1, 1);
		hash = ostrcat(hash, "&prevRID=", 1, 0);
		hash = ostrcat(hash, tmp5, 1, 1);
		hash = ostrcat(hash, "&openid.identity=", 1, 0);
		hash = ostrcat(hash, tmp6, 1, 1);
		hash = ostrcat(hash, "&openid.assoc_handle=", 1, 0);
		hash = ostrcat(hash, tmp7, 1, 1);
		hash = ostrcat(hash, "&openid.mode=", 1, 0);
		hash = ostrcat(hash, tmp8, 1, 1);
		hash = ostrcat(hash, "&openid.ns.pape=", 1, 0);
		hash = ostrcat(hash, tmp9, 1, 1);
		hash = ostrcat(hash, "&openid.claimed_id=", 1, 0);
		hash = ostrcat(hash, tmp10, 1, 1);
		hash = ostrcat(hash, "&pageId=", 1, 0);
		hash = ostrcat(hash, tmp11, 1, 1);
		hash = ostrcat(hash, "&openid.ns=", 1, 0);
		hash = ostrcat(hash, tmp12, 1, 1);
		hash = ostrcat(hash, "&email=", 1, 0);
		hash = ostrcat(hash, user, 1, 1);
		hash = ostrcat(hash, "&create=0", 1, 0);
		hash = ostrcat(hash, "&password=", 1, 0);
		hash = ostrcat(hash, pass, 1, 1);
	
		curlretbuf = gethttps_post("https://www.amazon.de/ap/signin", hash, NULL);
//		debug(99, "ret=%s", curlretbuf);
		titheklog(debuglevel, "/tmp/amazon_tmpstr_post1", NULL, NULL, NULL, curlretbuf);	
		if(checkamazonerror() == 1) return 1;
	
		curlretbuf = gethttps_get("https://www.amazon.de/", NULL, NULL);
//		debug(99, "tmpstr: %s", curlretbuf);
		titheklog(debuglevel, "/tmp/amazon_tmpstr_get3_blank", NULL, NULL, NULL, curlretbuf);	
	
	
		curlretbuf = gethttps_get("https://www.amazon.de/", NULL, NULL);
//		debug(99, "tmpstr: %s", curlretbuf);
		titheklog(debuglevel, "/tmp/amazon_tmpstr_get4_blank", NULL, NULL, NULL, curlretbuf);	
	
		login = string_resub("'config.signOutText',", ");", curlretbuf, 0);	
//		debug(99, "login: %s", login);
		strstrip(login);
//		debug(99, "login: %s", login);
	
		if(ostrcmp("null", login) == 0)
		{
			debug(99, "Amazon Login Error");
			ret = 0;
		}
		else
		{
			debug(99, "Amazon Login Successful: %s", login);
			ret = 1;
		}
	}

	return ret;
}

int amazon_search(struct skin* grid, struct skin* listbox, struct skin* countlabel, struct skin* load, char* link, char* title, char* searchstr, int flag)
{
	int ret = 1;
	int debuglevel = getconfigint("debuglevel", NULL);

	if(listbox == NULL || listbox->select == NULL || listbox->select->handle == NULL)
		return ret;

	unlink("/tmp/amazon_search_tmpstr_get");

	char* search = NULL;

	if(flag == 0)
	{ 
		if(searchstr == NULL)
			search = textinputhist(_("Search"), " ", "searchhist");
		else
			search = textinputhist(_("Search"), searchstr, "searchhist");
	}

	char* send = NULL;
	char* tmppath = NULL;
	char* tmphost = NULL;
	char* tmpstr = NULL;
	char* line = NULL;

	int loginret = 0;
	loginret = login();
	printf("loginret=%d\n", loginret);
	if(loginret == 0)
		textbox(_("Message"), _("Please Add your Amazon Prime User/Pass in Tithek Settings, and try agaun later !"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 1100, 200, 0, 0);
	else
		textbox(_("Message"), _("Amazon Prime Login Successful!"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 800, 200, 0, 0);

//http://www.amazon.de/gp/video/watchlist/movie/?ie=UTF8&show=all&sort=DATE_ADDED_DESC

	if(search != NULL || flag > 0)
	{
		drawscreen(load, 0, 0);
		search = stringreplacechar(search, ' ', '+');
		char* url = ostrcat("http://www.amazon.de/mn/search/ajax/?_encoding=UTF8&url=node%3D3356018031&field-keywords=", search, 0, 0);
		
		curlretbuf = gethttps_get(url, NULL, NULL);
//		debug(99, "tmpstr: %s", curlretbuf);
		titheklog(debuglevel, "/tmp/amazon_search_tmpstr_get", NULL, NULL, NULL, curlretbuf);		
/////////
char* tmpstr2 = NULL;
		char* id = NULL;
		char* streamurl = NULL;
		char* pic = NULL;
		char* title = NULL;
		char* year = NULL;
		char* runtime = NULL;
		char* menu = NULL;
		int count1 = 0;
		int j = 0;

////////////
//		tmpstr2 = string_resub("\"value\" : \"<div id=\\\"centerMinus\\\"", "\"tagId\" : \"centerMinus\"", curlretbuf, 0);	
//		tmpstr2 = string_resub("\"value\" : \"<div id=\\\"centerBelowPlus\\\"", "\"tagId\" : \"centerBelowPlus\"", curlretbuf, 0);	
		tmpstr2 = ostrcat(curlretbuf, NULL, 0, 0);
//		printf("tmpstr2: %s\n", tmpstr2);


		tmpstr2 = string_replace_all("<li id=\\\"result_", "\n<li id=\\\"result_", tmpstr2, 1);

		count1 = 0;
		j = 0;
		struct splitstr* ret1 = NULL;
		ret1 = strsplit(tmpstr2, "\n", &count1);
		int incount = 0;

		if(ret1 != NULL && count1 > 0)
		{
			int max = count1;
			for(j = 0; j < max; j++)
			{
				if(ostrstr(ret1[j].part, "result_") != NULL)
				{
//					printf("(%d) ret1[j].part: %s\n", j, ret1[j].part);

					streamurl = string_resub("\" href=\\\"", "\\\">", ret1[j].part, 0);
					pic = string_resub("\" src=\\\"", "\\\"", ret1[j].part, 0);
					title = string_resub("\" title=\\\"", "\\\"", ret1[j].part, 0);
					year = string_resub("<span class=\\\"a-size-small a-color-secondary\\\">", "</span>", ret1[j].part, 0);
					runtime = oregex(".*a-size-small a-color-secondary.*>(.*)</span>.*", ret1[j].part);
					id = oregex("http.*//.*/.*/(.*)/ref.*", streamurl);

//					printf("(%d) streamurl: %s\n", j, streamurl);
					free(streamurl), streamurl = NULL;
					streamurl = ostrcat("http://www.amazon.de/dp/", id, 0, 0);
					streamurl = ostrcat(streamurl, "/?_encoding=UTF8", 1, 0);
//					printf("(%d) streamurl changed: %s\n", j, streamurl);
//					printf("(%d) id: %s\n", j, id);
//					printf("(%d) pic: %s\n", j, pic);
//					printf("(%d) title: %s\n", j, title);
//					printf("(%d) year: %s\n", j, year);
//					printf("(%d) runtime: %s\n", j, runtime);
//					printf("----------------------\n");

					if(streamurl != NULL)
					{
						incount += 1;
						line = ostrcat(line, title, 1, 0);
						line = ostrcat(line, " (", 1, 0);
						line = ostrcat(line, year, 1, 0);
						line = ostrcat(line, ") ", 1, 0);
						line = ostrcat(line, runtime, 1, 0);
						line = ostrcat(line, "#", 1, 0);
						line = ostrcat(line, id, 1, 0);
						line = ostrcat(line, "#", 1, 0);
						line = ostrcat(line, pic, 1, 0);
						line = ostrcat(line, "#amazon_search_", 1, 0);
						line = ostrcat(line, oitoa(incount + time(NULL)), 1, 0);
						line = ostrcat(line, ".jpg#Amazon Instant Video - Search#75\n", 1, 0);
					}
					
					free(id), id = NULL;
					free(streamurl), streamurl = NULL;
					free(pic), pic = NULL;
					free(title), title = NULL;
					free(year), year = NULL;
					free(runtime), runtime = NULL;
				}
			}
		}


		if(line != NULL)
		{
			menu = ostrcat("/tmp/tithek/amazon.search.list", NULL, 0, 0);
			writesys(menu, line, 0);
			struct tithek* tnode = (struct tithek*)listbox->select->handle;
			createtithek(tnode, tnode->title, menu, tnode->pic, tnode->localname, tnode->menutitle, tnode->flag);
			ret = 0;
		}

/*
		int i = 0, len = 0, treffer = 0, amazontoken = 1000;
		jsmn_parser parser;
	//	jsmntok_t tokens[FACEMAXTOKEN]; //TODO
		jsmntok_t tokens[amazontoken]; //TODO
		char* buf = NULL;

		if(buf == NULL)
		{
			err("no mem");
			free(curlretbuf); curlretbuf = NULL;
			curlretbufsize = 0;
			return 1;
		}
	
		jsmn_init(&parser);
	
	//	ret = jsmn_parse(&parser, curlretbuf, tokens, FACEMAXTOKEN);
		int ret1 = jsmn_parse(&parser, curlretbuf, tokens, amazontoken);
		if(ret1 == JSMN_SUCCESS)
		{
			for(i = 0; i < amazontoken; i++)
			{
				if(tokens[i].start == -1) break;
	
				len = tokens[i].end - tokens[i].start;
				if(len > MINMALLOC) len = MINMALLOC;
				char* ptr = curlretbuf + tokens[i].start;
	
				printf("tokens[i].type=%d ptr=%s buf=%s\n", tokens[i].type, ptr, buf);
			}
		}
*/
///////
	}

	free(search), search = NULL;
	return ret;
}

int amazon_search_local(struct skin* grid, struct skin* listbox, struct skin* countlabel, struct skin* load, char* link, char* title, char* searchstr, int flag)
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

		tmpstr = gethttp("atemio.dyndns.tv", "/mediathek/amazon/streams/amazon.all-sorted.list", 80, NULL, HTTPAUTH, 5000, NULL, 0);

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
				menu = ostrcat("/tmp/tithek/amazon.search.list", NULL, 0, 0);
				writesys(menu, line, 0);
				struct tithek* tnode = (struct tithek*)listbox->select->handle;
				createtithek(tnode, tnode->title, menu, tnode->pic, tnode->localname, tnode->menutitle, tnode->flag);
				ret = 0;
			}
		}
		free(tmpstr), tmpstr = NULL;
	}
	free(search), search = NULL;

///////////////////////////////////
	free(curlretbuf); curlretbuf = NULL;
	curlretbufsize = 0;
///////////////////////////////////

	return ret;
}

#endif
