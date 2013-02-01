#ifndef NEWS_H
#define NEWS_H

struct news
{
	char* title;
	char* url;
	char* snippet;
	char* source;
	char* date;
	struct news* prev;
	struct news* next;
};

extern struct skin* skin;
struct news* news = NULL;

int delnews(struct news* delnode)
{
	int ret = 1;
	struct news *node = news, *prev = news;

	while(node != NULL)
	{
		if(node == delnode)
		{
			ret = 0;
			if(node == news)
			{
				news = node->next;
				if(news != NULL)
					news->prev = NULL;
			}
			else
			{
				prev->next = node->next;
				if(node->next != NULL)
					node->next->prev = prev;
			}

			free(node->title); node->title = NULL;
			free(node->url); node->url = NULL;
			free(node->snippet); node->snippet = NULL;
			free(node->source); node->source = NULL;
			free(node->date); node->date = NULL;

			free(node); node = NULL;

			break;
		}

		prev = node;
		node = node->next;
	}

	return ret;
}

void freenews()
{
	struct news *node = news, *prev = news;

	while(node != NULL)
	{
		prev = node;
		node = node->next;
		if(prev != NULL)
			delnews(prev);
	}
}

struct news* addnews(char *line, int count, struct news* last)
{
	struct news *newnode = NULL, *prev = NULL, *node = news;

	if(line == NULL) return NULL;

	newnode = (struct news*)calloc(1, sizeof(struct news));
	if(newnode == NULL)
	{
		err("no memory");
		return NULL;
	}

	newnode->title = getxmlentry(line, "title data=");
	newnode->url = getxmlentry(line, "url data=");
	newnode->snippet = getxmlentry(line, "snippet data=");
	newnode->source = getxmlentry(line, "source data=");
	newnode->date = getxmlentry(line, "date data=");

	if(last == NULL)
	{
		while(node != NULL)
		{
			prev = node;
			node = node->next;
		}
	}
	else
	{
		prev = last;
		node = last->next;
	}

	if(prev == NULL)
		news = newnode;
	else
	{
		prev->next = newnode;
		newnode->prev = prev;
	}
	newnode->next = node;
	if(node != NULL) node->prev = newnode;

	return newnode;
}

int getnews()
{
	int linecount = 0;
	struct news* last = NULL, *tmplast = NULL;
	char* tmpstr = NULL, *tmpstr1 = NULL, *tmpstr2 = NULL;

	//TODO: got language from titan.cfg
	tmpstr = gethttp("www.google.com", "ig/api?news&hl=de", 80, NULL, NULL, 5000, NULL, 0);

	if(tmpstr != NULL)
	{

		while(ostrstr(tmpstr, "&amp;amp;quot;") != NULL)
			tmpstr = string_replace("&amp;amp;quot;", "\"", tmpstr, 1);
		tmpstr = string_decode(tmpstr, 0);

		tmpstr1 = tmpstr;
		tmpstr2 = tmpstr;

		tmpstr2 = ostrstr(tmpstr1, "<news_entry>");
		while(tmpstr2 != NULL)
		{
			linecount++;
			tmpstr1 = tmpstr2 + 5;
      
			if(last == NULL) last = tmplast;
			last = addnews(tmpstr2, linecount, last);
			if(last != NULL) tmplast = last;
			tmpstr2 = ostrstr(tmpstr1, "<news_entry>");
		}

		free(tmpstr); tmpstr = NULL;
	}

	return 0;
}

void screennews()
{
	int rcret = 0, ret = 0;
	struct skin* skin_news = getscreen("news");
	struct skin* title = getscreennode(skin_news, "title");
	struct skin* snippet = getscreennode(skin_news, "snippet");
	struct skin* source = getscreennode(skin_news, "source");
	struct skin* date = getscreennode(skin_news, "date");
	struct news* node = NULL;
	char* tmpstr = NULL;

	getnews();
	node = news;
  
start:

	if(node != NULL)
	{
		changetext(title, node->title);
		changetext(snippet, node->snippet);
		changetext(source, node->source);
		changetext(date, node->date);
	}

	drawscreen(skin_news, 0, 0);

	while(1)
	{
		rcret = waitrc(skin_news, 0, 0);
	
		if(rcret == getrcconfigint("rcexit", NULL)) break;
		if(rcret == getrcconfigint("rcok", NULL)) break;
    
		if(rcret == getrcconfigint("rcred", NULL))
		{
			if(node != NULL && node->prev != NULL) node = node->prev;
			goto start;
		}
    
		if(rcret == getrcconfigint("rcgreen", NULL))
		{
			if(node != NULL && node->next != NULL) node = node->next;
			goto start;
		}
    
		if(rcret == getrcconfigint("rcyellow", NULL))
		{
      ret = servicestop(status.aktservice, 1, 0);
      if(ret == 1) goto start;
      status.sec = 0; //deaktivate spinner
      setfbtransparent(255);
      
      drawscreen(skin, 0, 0);
      tmpstr = ostrcat("nsfb -f linux ", node->url, 0, 0);
      system(tmpstr);
      free(tmpstr); tmpstr = NULL;
      drawscreen(skin, 0, 0);
        
      setosdtransparent(getskinconfigint("osdtransparent", NULL));
      if(status.lastservice != NULL)
        servicestart(status.lastservice->channel, NULL, NULL, 0);
      flushrc(500);
      
			goto start;
		}
	}

	freenews(node); node = NULL;
	clearscreen(skin_news);
}

#endif
