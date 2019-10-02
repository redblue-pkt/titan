#ifndef IMDB_GLOBAL_H
#define IMDB_GLOBAL_H

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

// flag = 0 (without header in output)
// flag = 1 (with header in output)
char* gethttps(char* url, char* localfile, char* data, char* user, char* pass, char* referer, int flag)
{
	debug(99, "url: %s", url);

	int debuglevel = getconfigint("debuglevel", NULL);

	char* tmpstr = NULL;
    FILE *fp;

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
	    if(localfile != NULL)
		    fp = fopen(localfile,"wb");
	       
		/* specify URL to get */
		curl_easy_setopt(curl_handle, CURLOPT_URL, url);

		if(user != NULL && pass != NULL)
		{
			curl_easy_setopt(curl_handle, CURLOPT_USERNAME, user);
			curl_easy_setopt(curl_handle, CURLOPT_PASSWORD, pass);
			curl_easy_setopt(curl_handle, CURLOPT_HTTPAUTH, CURLAUTH_ANY);
		}
		if(data == NULL)
			curl_easy_setopt(curl_handle, CURLOPT_HTTPGET, 1L);
		else
		{
			curl_easy_setopt(curl_handle, CURLOPT_POST, 1);
			curl_easy_setopt(curl_handle, CURLOPT_POSTFIELDS, data);

			/* example.com is redirected, so we tell libcurl to send POST on 301, 302 and
		     303 HTTP response codes */
			curl_easy_setopt(curl_handle, CURLOPT_POSTREDIR, CURL_REDIR_POST_ALL);
		}
		if(flag == 1)
			curl_easy_setopt(curl_handle, CURLOPT_HEADER, 1L);
		curl_easy_setopt(curl_handle, CURLOPT_CONNECTTIMEOUT, 5);
		curl_easy_setopt(curl_handle, CURLOPT_TIMEOUT, 3);
//		curl_easy_setopt(curl_handle, CURLOPT_RETURNTRANSFER, 1);

		/* send all data to this function  */
	    if(localfile == NULL)
			curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
		else
			curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, NULL);

		/* we pass our 'chunk' struct to the callback function */
	    if(localfile == NULL)
			curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void *)&chunk);
		else
			curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, fp);

		/* some servers don't like requests that are made without a user-agent field, so we provide one */
//		curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, "libcurl-agent/1.0");
		curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, "Mozilla/5.0 (Windows NT 6.3; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Maxthon/4.4.7.3000 Chrome/30.0.1599.101 Safari/537.36");

		// This is occassionally required to stop CURL from verifying the peers certificate.
		// CURLOPT_SSL_VERIFYHOST may also need to be TRUE or FALSE if
		// CURLOPT_SSL_VERIFYPEER is disabled (it defaults to 2 - check the existence of a
		// common name and also verify that it matches the hostname provided)
#ifdef MIPSEL
		curl_easy_setopt(curl_handle, CURLOPT_SSL_VERIFYPEER, 1L);
#else
		curl_easy_setopt(curl_handle, CURLOPT_SSL_VERIFYPEER, 0L);
#endif

		curl_easy_setopt(curl_handle, CURLOPT_SSL_VERIFYHOST, 0L);
		if(debuglevel == 99)
			curl_easy_setopt(curl_handle, CURLOPT_VERBOSE, 1);
		curl_easy_setopt(curl_handle, CURLOPT_COOKIEFILE, "/mnt/network/cookies");
		curl_easy_setopt(curl_handle, CURLOPT_COOKIEJAR, "/mnt/network/cookies");
		/* enable redirect following */
		curl_easy_setopt(curl_handle, CURLOPT_FOLLOWLOCATION, 1L);
		/* allow three redirects */
		curl_easy_setopt(curl_handle, CURLOPT_MAXREDIRS, 3L);

		/* enable all supported built-in compressions */
		curl_easy_setopt(curl_handle, CURLOPT_ACCEPT_ENCODING, "");
  
		if(referer == NULL)
			curl_easy_setopt(curl_handle, CURLOPT_AUTOREFERER, 1L);
		else
			curl_easy_setopt(curl_handle, CURLOPT_REFERER, referer);

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
		if(localfile != NULL)
			fclose(fp);
	}

	tmpstr = ostrcat(chunk.memory, NULL, 0, 0);
  	free(chunk.memory);
  	/* we're done with libcurl, so clean it up */
   	curl_global_cleanup();

	if(localfile != NULL) 
		free(tmpstr), tmpstr = NULL;
	return tmpstr;
}

#endif
