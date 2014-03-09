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
struct imdbapi* getimdbapi(struct imdbapi** first, char* input, int flag, int flag1)
{
	debug(133, "title: %s",input);
	debug(133, "flag: %d",flag);
	debug(133, "flag1: %d",flag1);

//	char* tmpstr = NULL, *ret = NULL, *tmpsearch = NULL, *savefile = NULL, *savethumb = NULL, *title = NULL;
	char* tmpstr = NULL, *ret = NULL, *tmpsearch = NULL, *savefile = NULL, *title = NULL;

	if(checkinternet() == 1)
	{
		debug(133, "Error, Internet Connection not found");
		return NULL;
	}

	title = ostrcat(title, input, 1, 0);

//	unsigned char* buf = NULL;
//	int channels = 0;
//	unsigned long width = 0, height = 0, rowbytes = 0; 

	if(flag == 0)
		tmpsearch = ostrcat("?i=&t=", title, 0, 0);
	else
		tmpsearch = ostrcat("?i=", title, 0, 0);

	tmpsearch = stringreplacechar(tmpsearch, ' ', '+');
	debug(133, "search: http://www.imdbapi.com/%s", tmpsearch);

	tmpstr = gethttp("www.imdbapi.com", tmpsearch, 80, NULL, NULL, 5000, NULL, 0);
	debug(133, "tmpsearch: %s", tmpsearch);
//	debug(133, "tmpstr: %s", tmpstr);

	free(tmpsearch); tmpsearch = NULL;
 
	if(tmpstr != NULL && ostrstr(tmpstr, "Movie not found!") == NULL)
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
		(*first)->rating = getxmlentry(tmpstr, "\"imdbRating\":");
		(*first)->votes = getxmlentry(tmpstr, "\"imdbVotes\":");
		(*first)->id = getxmlentry(tmpstr, "\"imdbID\":");

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
					gethttp(ip, path, 80, savefile, NULL, 5000, NULL, 0);
				free((*first)->poster);
				(*first)->poster = savefile;

/*
				//create thumb 
				savethumb = ostrcat(getconfig("mediadbpath", NULL), "/", 0, 0); 
				savethumb = ostrcat(savethumb, (*first)->id, 1, 0); 
				savethumb = ostrcat(savethumb, "_thumb.jpg", 1, 0);
				if(file_exist(savefile) && !file_exist(savethumb))
				{
					buf = loadjpg(savefile, &width, &height, &rowbytes, &channels, 16);
					buf = savejpg(savethumb, width, height, channels, 91, 140, 70, buf);
				}
				free(savethumb); savethumb = NULL;
				free(buf); buf = NULL;

				//create cover
				savethumb = ostrcat(getconfig("mediadbpath", NULL), "/", 0, 0);
				savethumb = ostrcat(savethumb, (*first)->id, 1, 0);
				savethumb = ostrcat(savethumb, "_cover.jpg", 1, 0);
				if(file_exist(savefile) && !file_exist(savethumb))
				{
					buf = loadjpg(savefile, &width, &height, &rowbytes, &channels, 16);
					buf = savejpg(savethumb, width, height, channels, 185, 264, 70, buf);
				}
				free(savethumb); savethumb = NULL;
				free(buf); buf = NULL; 
*/
			}
			else if(flag == 0)
			{
				gethttp(ip, path, 80, TMPIMDBAPIPIC, NULL, 5000, NULL, 0);
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

void screenimdbapi(char* title, char* dummy1, char* dummy2, char* path, char* file)
{
	debug(133, "title: %s",title);
	debug(133, "path: %s",path);
	debug(133, "file: %s",file);	
	char* searchstr = NULL, *tmpstr = NULL;

	if(checkinternet() == 1)
	{
		textbox(_("Message"), _("Error, Internet Connection not found"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 1100, 500, 10, 0);
		return;
	}

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
	struct skin* skin_b2 = getscreennode(imdbapiskin, "b2");
	struct skin* load = getscreen("loading");
	struct skin* blackscreen = getscreen("blackscreen");

	struct imdbapi* node = NULL;
	char* search = NULL;

	if(path == NULL || file == NULL || !file_exist(getconfig("mediadbpath", NULL)))
		skin_b2->hidden = YES;
	else
		skin_b2->hidden = NO;

//	setfbtransparent(255);
	status.hangtime = 99999;

	if(title == NULL)
		searchstr = getepgakttitle(NULL);
	else
	{	
		int isrec = 0, iscam = 0;
		tmpstr = createshortname(file, &isrec, &iscam, 1);
		if(tmpstr != NULL)
			searchstr = ostrcat(searchstr, tmpstr, 1, 0);
		else
			searchstr = ostrcat(searchstr, title, 1, 0);
	}

	debug(133, "searchstr: %s",searchstr);

	drawscreen(blackscreen, 0, 0);
	drawscreen(load, 0, 0);

	node = getimdbapi(&node, searchstr, 0, 0);
	clearscreen(load);
	clearscreen(blackscreen);

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
		skin_cover->hidden = NO;
	}
	else
	{
		changetext(skin_plot, _("--plot--"));
		changetext(skin_title, _("--title--"));
		changetext(skin_director, _("--director--"));
		changetext(skin_writers, _("--writers--"));
		changetext(skin_genre, _("--genre--"));
		changetext(skin_releasetime, _("--releasetime--"));
		changetext(skin_actors, _("--actors--"));
		skin_cover->hidden = YES;
	}
	

	drawscreen(imdbapiskin, 0, 0);

	while(1)
	{
		rcret = waitrc(imdbapiskin, 0, 0);
	
		if(rcret == getrcconfigint("rcexit", NULL)) break;
		if(rcret == getrcconfigint("rcok", NULL)) break;

		if(rcret == getrcconfigint("rcred", NULL))
    	{
			search = textinput(_("Search"), searchstr);
			if(search != NULL)
			{
				freeimdbapi(&node, 0); node = NULL;
				drawscreen(blackscreen, 0, 0);
				drawscreen(load, 0, 0);
				node = getimdbapi(&node, search, 0, 0);
				clearscreen(load);
				clearscreen(blackscreen);
				free(search); search = NULL;
				goto start;
			}
			drawscreen(imdbapiskin, 0, 0);
			continue;
		}

		if(path != NULL && file != NULL && node != NULL && node->id != NULL && rcret == getrcconfigint("rcgreen", NULL))
		{
			drawscreen(blackscreen, 0, 0);
			drawscreen(load, 0, 0);
			debug(133, "path: %s",path);
			debug(133, "file: %s",file);
			debug(133, "type: 2");
			debug(133, "imdbid: %s",node->id);
			addconfigtmp("mediadbscantimeout", "0");			
			mediadbfindfilecb(path, file, 0, node->id, 3);
			delconfigtmp("mediadbscantimeout");
			clearscreen(load);
			clearscreen(blackscreen);
			break;
		}
	}

	free(searchstr), searchstr = NULL;
	freeimdbapi(&node, 0); node = NULL;
//	setosdtransparent(getskinconfigint("osdtransparent", NULL));
	status.hangtime = getconfigint("hangtime", NULL);
	clearscreen(imdbapiskin);
}

#endif
