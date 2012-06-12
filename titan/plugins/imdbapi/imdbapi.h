#ifndef IMDBAPI_H
#define IMDBAPI_H

#define TMPIMDBAPIPIC "/tmp/tmpimdbapi.jpg"

void freeimdbapi(struct imdbapi** node, int flag)
{
	if(node == NULL || *node == NULL) return;

	free((*node)->title); (*node)->title = NULL;
	free((*node)->year); (*node)->year = NULL;
	free((*node)->rated); (*node)->rated = NULL;
	free((*node)->released); (*node)->released = NULL;
	free((*node)->genre); (*node)->genre = NULL;
	free((*node)->director); (*node)->director = NULL;
	free((*node)->writer); (*node)->writer = NULL;
	free((*node)->actors); (*node)->actors = NULL;
	free((*node)->plot); (*node)->plot = NULL;
	free((*node)->poster); (*node)->poster = NULL;
	free((*node)->runtime); (*node)->runtime = NULL;
	free((*node)->rating); (*node)->rating = NULL;
	free((*node)->votes); (*node)->votes = NULL;
	free((*node)->id); (*node)->id = NULL;
	unlink(TMPIMDBAPIPIC);

	free(*node); *node = NULL;
}

//flag 0 = title search
//flag 1 = imdbid search
//flag1 0 = save in tmp
//flag1: 1 = save pic in mediadb path if pic not exist
//flag2: 2 = save no pic
struct imdbapi* getimdbapi(struct imdbapi** first, char* title, int flag, int flag1)
{
	struct imdbapi* imdbapi = NULL;
	char* tmpstr = NULL, *ret = NULL;
	char* tmpsearch = NULL;
	char* savefile = NULL, *savethumb = NULL; 
	unsigned char* buf = NULL;
	int channels = 0;
	unsigned long width = 0, height = 0, rowbytes = 0; 

	if(flag == 0)
		tmpsearch = ostrcat("?i=&t=", title, 0, 0);
	else
		tmpsearch = ostrcat("?i=", title, 0, 0);

	tmpsearch = stringreplacechar(tmpsearch, ' ', '+');

	tmpstr = gethttp("www.imdbapi.com", tmpsearch, 80, NULL, NULL, NULL, 0);
	debug(133, "tmpsearch: %s", tmpsearch);
//	debug(133, "tmpstr: %s", tmpstr);

	free(tmpsearch); tmpsearch = NULL;

	if(tmpstr != NULL)
	{
		ret = getxmlentry(tmpstr, "\"Response\":");
		if(ostrcmp(ret, "False") == 0)
		{
			free(ret); ret = NULL;
			free(tmpstr); tmpstr = NULL;
			return NULL;
		}
		free(ret); ret = NULL;

		*first = (struct imdbapi*)calloc(1, sizeof(struct imdbapi));
		if(*first == NULL)
		{
			err("no mem");
			free(tmpstr); tmpstr = NULL;
			return NULL;
		}

		(*first)->title = getxmlentry(tmpstr, "\"Title\":");
		(*first)->year = getxmlentry(tmpstr, "\"Year\":");
		(*first)->rated = getxmlentry(tmpstr, "\"Rated\":");
		(*first)->released = getxmlentry(tmpstr, "\"Released\":");
		(*first)->genre = getxmlentry(tmpstr, "\"Genre\":");
		(*first)->director = getxmlentry(tmpstr, "\"Director\":");
		(*first)->writer = getxmlentry(tmpstr, "\"Writer\":");
		(*first)->actors = getxmlentry(tmpstr, "\"Actors\":");
		(*first)->plot = getxmlentry(tmpstr, "\"Plot\":");
		(*first)->poster = getxmlentry(tmpstr, "\"Poster\":");
		(*first)->runtime = getxmlentry(tmpstr, "\"Runtime\":");
		(*first)->rating = getxmlentry(tmpstr, "\"imdbapiRating\":");
		(*first)->votes = getxmlentry(tmpstr, "\"imdbapiVotes\":");
		(*first)->id = getxmlentry(tmpstr, "\"imdbapiID\":");

		if((*first)->poster != NULL)
		{
			char* ip = NULL, *pos = NULL, *path = NULL;
			ip = string_replace("http://", "", (*first)->poster, 0);

			if(ip != NULL)
				pos = strchr(ip, '/');
			if(pos != NULL)
			{
				pos[0] = '\0';
				path = pos + 1;
			}

			if(flag1 == 1 && (*first)->id != NULL)
			{
				savefile = ostrcat(getconfig("mediadbpath", NULL), "/", 0, 0);
				savefile = ostrcat(savefile, (*first)->id, 1, 0);
				savefile = ostrcat(savefile, "_poster.jpg", 1, 0);

				if(!file_exist(savefile))
					gethttp(ip, path, 80, savefile, NULL, NULL, 0);
				free((*first)->poster);
				(*first)->poster = savefile;
				//create thumb 
				savethumb = ostrcat(getconfig("mediadbpath", NULL), "/", 0, 0); 
				savethumb = ostrcat(savethumb, (*first)->id, 1, 0); 
				savethumb = ostrcat(savethumb, "_thumb.jpg", 1, 0);
				if(file_exist(savefile) && !file_exist(savethumb))
				{
					buf = loadjpg(savefile, &width, &height, &rowbytes, &channels, 16);
					savejpg(savethumb, 91, 140, buf);
				}
				free(buf); buf = NULL; 
				free(savethumb); savethumb = NULL;
			}
			else if(flag == 0)
			{
				gethttp(ip, path, 80, TMPIMDBAPIPIC, NULL, NULL, 0);
				free((*first)->poster);
				(*first)->poster = ostrcat(TMPIMDBAPIPIC, NULL, 0, 0);
			}

			free(ip); ip = NULL;
		}
		else
		{
			free((*first)->poster);
			(*first)->poster = NULL;
		}

		free(tmpstr); tmpstr = NULL;

		debug(133, "----------------------imdbapi start----------------------");
		debug(133, "id: %s", (*first)->id);
		debug(133, "title: %s", (*first)->title);
		debug(133, "genre: %s", (*first)->genre);
		debug(133, "writer: %s", (*first)->writer);
		debug(133, "director: %s", (*first)->director);
		debug(133, "released: %s", (*first)->released);
		debug(133, "actors: %s", (*first)->actors);
		debug(133, "plot: %s", (*first)->plot);
		debug(133, "poster: %s", (*first)->poster);
		debug(133, "rating: %s", (*first)->rating);
		debug(133, "votes: %s", (*first)->votes);
		debug(133, "runtime: %s", (*first)->runtime);
		debug(133, "year: %s", (*first)->year);
		debug(133, "rated: %s", (*first)->rated);
		debug(133, "----------------------imdbapi end----------------------");
	}

	return *first;
}

void screenimdbapi(char* title)
{
	int rcret = 0;
	struct skin* imdbapiskin = getscreen("imdbapi");
	struct skin* skin_plot = getscreennode(imdbapiskin, "plot");
	struct skin* skin_title = getscreennode(imdbapiskin, "title");
	struct skin* skin_director = getscreennode(imdbapiskin, "director");
	struct skin* skin_writers = getscreennode(imdbapiskin, "writers");
	struct skin* skin_genre = getscreennode(imdbapiskin, "genre");
	struct skin* skin_releasetime = getscreennode(imdbapiskin, "releasetime");
	struct skin* skin_cover = getscreennode(imdbapiskin, "cover");
	struct skin* skin_actors = getscreennode(imdbapiskin, "actors");
	struct imdbapi* node = NULL;
	char* search = NULL;

	setfbtransparent(255);
	status.hangtime = 99999;

	if(title == NULL) title = getepgakttitle(NULL);

	node = getimdbapi(&node, title, 0, 0);
start:
	if(node != NULL)
	{
		changetext(skin_plot, node->plot);
		changetext(skin_title, node->title);
		changetext(skin_director, node->director);
		changetext(skin_writers, node->writer);
		changetext(skin_genre, node->genre);
		changetext(skin_releasetime, node->released);
		changetext(skin_actors, node->actors);
		changepic(skin_cover, node->poster);
	}

	drawscreen(imdbapiskin, 0);

	while(1)
	{
		rcret = waitrc(imdbapiskin, 0, 0);
	
		if(rcret == getrcconfigint("rcexit", NULL)) break;
		if(rcret == getrcconfigint("rcok", NULL)) break;

		if(rcret == getrcconfigint("rcred", NULL))
    	{
			search = textinput("Search", NULL);
			if(search != NULL)
			{
				freeimdbapi(&node); node = NULL;
				node = getimdbapi(&search, 0, 0);
				free(search); search = NULL;
				goto start;
			}
			drawscreen(imdbapiskin, 0);
			continue;
		}
	}

	freeimdbapi(&node); node = NULL;
	setosdtransparent(getskinconfigint("osdtransparent", NULL));
	status.hangtime = getconfigint("hangtime", NULL);
	clearscreen(imdbapiskin);
}

#endif
