#ifndef IMDB_H
#define IMDB_H

extern struct skin* skin;

char* screensearch(char* input)
{
	char* lastline = NULL;
	char* tmpstr = NULL;
	if(input != NULL)
		lastline = textinput(_("Imdb Suche"), input);
	else
		lastline = textinput(_("Imdb Suche"), "");

	return lastline;
}

void imdb()
{
	int rcret = 0;
	drawscreen(skin, 0);
	setfbtransparent(255);
//NIT: muß wieder richtig gesetzt werden
	status.hangtime = 99999;
	struct skin* blackscreen = getscreen("blackscreen");
	drawscreen(blackscreen, 0);

	// main screen
	struct skin* imdbskin = getscreen("imdb");
	struct skin* skin_plot = getscreennode(imdbskin, "plot");
	struct skin* skin_title = getscreennode(imdbskin, "title");
	struct skin* skin_director = getscreennode(imdbskin, "director");
	struct skin* skin_writers = getscreennode(imdbskin, "writers");
	struct skin* skin_genre = getscreennode(imdbskin, "genre");
	struct skin* skin_tagline = getscreennode(imdbskin, "tagline");
	struct skin* skin_releasetime = getscreennode(imdbskin, "releasetime");
	struct skin* skin_bigcover = getscreennode(imdbskin, "bigcover");
	struct skin* skin_cover = getscreennode(imdbskin, "cover");
	struct skin* skin_cast = getscreennode(imdbskin, "cast");

//if(ostrcmp("vfdisplay", name) == 0) return 0;
//tmpstr = ostrcat(tmpstr, "", 1, 0);

	char* input = NULL;

	char* url = NULL;
	char* apiSearchTV = NULL;
	char* apiImdbLookup = NULL;
	char* apiSearch = NULL;
	char* localfile = NULL;
	char* fetchurl = NULL;
	char* search = NULL;
	char* tmpstr = NULL;
	char* imdbid = NULL;
	char* title = NULL;
	char* plot = NULL;
	char* director = NULL;
	char* writers = NULL;
	char* genre = NULL;
	char* tagline = NULL;
	char* releasetime = NULL;
	char* pagebigcover = NULL;
	char* bigcover = NULL;
	char* cover = NULL;
	char* cast = NULL;


	input = getepgakttitle();
//	input = screensearch("Rambo");
	printf("input: %s\n", input);
//	stringreplacechar(input, ' ', '%20');
	input = string_replace(" ", "%20", input, 1);
//	htmlencode(input);
	printf("input: %s\n", input);

//drawscreen(imdbskin, 0);
//imdbskin->hidden = YES;
//	input = ostrcat("", "Days+of+Summer", 0, 0);
	url = ostrcat(url, "www.imdb.de", 1, 0);

	apiSearchTV = ostrcat("", "/search/title?title=<title>&title_type=tv_series", 0, 0);
	apiImdbLookup = ostrcat("", "/title/tt", 0, 0);
	apiSearch = ostrcat("", "/find?s=tt;q=", 0, 0);
	localfile = ostrcat("", "/tmp/cache.getMovieByTitle.html", 0, 0);
	search = ostrcat(apiSearch, input, 0, 0);
	printf("search: %s\n", search);
	tmpstr = get_ip(url);
	printf("get_ip(url): %s\n", tmpstr);
	gethttp(tmpstr, search, 80, localfile, NULL, NULL, 0);
	free(tmpstr); tmpstr = NULL;
//	gethttp(url, search, 80, localfile, NULL, NULL, 0);


	tmpstr = readfiletomem(localfile, 1);

	if(string_find("<title>IMDb Title Search</title>",tmpstr))
	{
		imdbid = ostrcat(tmpstr, "", 0, 0);
		string_resub("<a href=\"/title/tt","/",imdbid);
		strstrip(imdbid);
		printf("imdbid: %s\n", imdbid);
	}

	free(tmpstr), tmpstr = NULL;
////////////////////
	search = ostrcat(apiImdbLookup, imdbid, 0, 0);
	search = ostrcat(search, "/", 0, 0);

	localfile = ostrcat("", "/tmp/cache.getMoviesByImdbId.html", 0, 0);
	printf("search: %s\n", search);
	printf("url: %s\n", get_ip(url));
	printf("localfile: %s\n", localfile);

//	gethttp(get_ip(url), search, 80, localfile, NULL, NULL, 0);
//	gethttp(url, search, 80, localfile, NULL, NULL, 0);
	unlink(localfile);
//cache.getMoviesByImdbId.html
	char* cmd = NULL;
	cmd = ostrcat(cmd, "wget http://", 1, 0);
	cmd = ostrcat(cmd, url, 1, 0);
	cmd = ostrcat(cmd, search, 1, 0);
	cmd = ostrcat(cmd, " -O ", 1, 0);
	cmd = ostrcat(cmd, localfile, 1, 0);
	printf("cmd: %s\n",cmd);
	system(cmd);
	free(cmd), cmd = NULL;
//	tmpstr = string_strip_whitechars(readfiletomem(localfile, 1));
	tmpstr = readfiletomem(localfile, 1);

	title = ostrcat(tmpstr, "", 0, 0);
	string_resub("<title>","</title>",title);
	printf("title: %s\n", title);
	title = string_decode(title,1);
	printf("title: %s\n", title);
	changetext(skin_title, title);

////////////////////
	director = ostrcat(tmpstr, "", 0, 0);
//	string_resub("Director:","</div>",director);	
	string_resub("Regisseur:","</div>",director);
	string_striptags(director);
	printf("director: %s\n", director);
	director = string_decode(director,1);
	printf("director: %s\n", director);
	changetext(skin_director, director);


////////////////////

	writers = ostrcat(tmpstr, "", 0, 0);
//	string_resub("<h5>Writers","</div>",writers);
	string_resub("<h5>Drehbuchautor","</div>",writers);
	printf("writers1: %s\n", writers);
	string_resub("<div class=\"info-content\">","</div>",writers);
	printf("writers2: %s\n", writers);

	string_striptags(writers);
	printf("writers3: %s\n", writers);
	writers = string_decode(writers,1);
	printf("writers: %s\n", writers);	
	changetext(skin_writers, writers);
////////////////////

	genre = ostrcat(tmpstr, "", 0, 0);
	string_resub("<h5>Genre:</h5>","</div>",genre);
	printf("genre1: %s\n", genre);
	string_striptags(genre);
	printf("genre2: %s\n", genre);
	genre = string_decode(genre,1);
	printf("genre: %s\n", genre);
	changetext(skin_genre, genre);

/*
	genre = string_replace("&#x", "%", genre, 1);
	printf("genre2222: %s\n", genre);
	char* genre_tmp = NULL;
	genre = string_replace(";", " ", genre, 1);
	genre_tmp = ostrcat(genre_tmp, genre, 1, 0);
	printf("genre333: %s\n", genre_tmp);		
//	free(genre),genre = NULL;

	htmldecode(genre,genre_tmp);
	printf("genre444: %s\n", genre);
//	free(genre_tmp),genre_tmp = NULL;		
	changetext(skin_genre, genre);
*/	
////////////////////

/*

	tagline = ostrcat(tmpstr, "", 0, 0);
	string_resub("<h4 class=\"inline\">Taglines:</h4>","<span class",tagline);

	strstrip(tagline);
	printf("tagline: %s\n", tagline);
	changetext(skin_tagline, tagline);

	releasetime = ostrcat(tmpstr, "", 0, 0);
	string_resub("<h4 class=\"inline\">Release Date:</h4>","</time>",releasetime);
	count2 = 0;	
	tmpstr2 = ostrcat(tmpstr2, releasetime, 1, 0);
	ret2 = strsplit(tmpstr2, "<>", &count2);
	printf("releasetime1: %s\n", (&ret2[2])->part);
	strstrip(releasetime);
	printf("releasetime: %s\n", releasetime);
	changetext(skin_releasetime, (&ret2[2])->part);
*/

	releasetime = ostrcat(tmpstr, "", 0, 0);
	string_resub("<h5>Premierendatum:</h5>","<a class=",releasetime);
	printf("releasetime1: %s\n", releasetime);

	string_striptags(releasetime);
	printf("releasetime2: %s\n", releasetime);
	releasetime = string_decode(releasetime,1);
	printf("releasetime: %s\n", releasetime);
	changetext(skin_releasetime, releasetime);

////////////////////

	cast = ostrcat(tmpstr, "", 0, 0);
	string_resub("<h3>Besetzung</h3>&nbsp;","</td></tr></table>",cast);
	string_resub("<div class=\"info-content block\"><table class=\"cast\">","</a></td></tr>",cast);

	cast = string_replace("...", "als", cast, 1);

	printf("cast1: %s\n", cast);

	string_striptags(cast);
	string_strip_whitechars(cast);
	printf("cast2: %s\n", cast);
	cast = string_decode(cast,1);
	printf("cast: %s\n", cast);
	changetext(skin_cast, cast);
	
////////////////////


	pagebigcover = ostrcat(tmpstr, "", 0, 0);
//	string_resub("href=\"/media/rm","/",pagebigcover);
	string_resub("/media/rm","/",pagebigcover);

	strstrip(pagebigcover);
	printf("pagebigcover: %s\n", pagebigcover);

/*
http://ia.media-imdb.com/images/M/MV5BMjE2MzU3Nzk4M15BMl5BanBnXkFtZTcwODcxNTI5Mw@@._V1_.jpg
<link rel="image_src" href="

//	char* tmpstr3 = NULL;
//	tmpstr3 = ostrcat("href=\"/media/rm", pagebigcover, 0, 0);
//	tmpstr3 = ostrcat(tmpstr3, "/tt", 1, 0);
//	tmpstr3 = ostrcat(tmpstr3, imdbid, 1, 0);
//	tmpstr3 = ostrcat(tmpstr3, "\"    ><img src=\"", 1, 0);

	cover = ostrcat(tmpstr, "", 0, 0);
	string_resub(tmpstr3,"\"",cover);

	printf("tmpstr3: %s\n", tmpstr3);
	strstrip(cover);
	printf("cover: %s\n", cover);

char* filename = NULL;
filename = ostrcat("wget ", cover, 0, 0);
filename = ostrcat(filename, " -O /tmp/cover.jpg", 1, 0);
system(filename);	
changepic(skin_cover, "/tmp/cover.jpg");
////////////////////

*/
	char* filename = NULL;
	search = ostrcat("/media/rm", pagebigcover, 0, 0);
	search = ostrcat(search, "/tt", 1, 0);
	search = ostrcat(search, imdbid, 1, 0);
	search = ostrcat(search, "/", 1, 0);

	localfile = ostrcat("", "/tmp/cache.getpageBigCoverHtml.html", 0, 0);
	printf("search: %s\n", search);
	printf("url: %s\n", get_ip(url));
	printf("localfile: %s\n", localfile);


//NIT: get_ip braucht ein free
	gethttp(get_ip(url), search, 80, localfile, NULL, NULL, 0);
//	gethttp(url, search, 80, localfile, NULL, NULL, 0);
	bigcover = readfiletomem(localfile, 1);
	string_resub("<link rel=\"image_src\" href=\"","\"",bigcover);
	strstrip(bigcover);
	printf("bigcover: %s\n", bigcover);

	unlink("/tmp/bigcover.jpg");
	filename = ostrcat("wget ", bigcover, 0, 0);
	filename = ostrcat(filename, " -O /tmp/bigcover.jpg", 1, 0);
	system(filename);

	rcret = servicestop(status.aktservice, 1, 0);
	if(rcret == 1) return;

changepic(skin_cover, "/tmp/bigcover.jpg");
////////////////////

//	free(tmpstr), tmpstr = NULL;
	search = ostrcat(apiImdbLookup, imdbid, 0, 0);
	search = ostrcat(search, "/plotsummary", 1, 0);

	localfile = ostrcat("", "/tmp/cache.getPlot.html", 0, 0);
	printf("search: %s\n", search);
	printf("url: %s\n", get_ip(url));
	printf("localfile: %s\n", localfile);
//	gethttp(get_ip(url), search, 80, localfile, NULL, NULL, 0);
//	gethttp(url, search, 80, localfile, NULL, NULL, 0);

	unlink(localfile);
	cmd = ostrcat(cmd, "wget http://", 1, 0);
	cmd = ostrcat(cmd, url, 1, 0);
	cmd = ostrcat(cmd, search, 1, 0);
	cmd = ostrcat(cmd, " -O ", 1, 0);
	cmd = ostrcat(cmd, localfile, 1, 0);
	printf("cmd: %s\n",cmd);
	system(cmd);
	free(cmd), cmd = NULL;
	
	
	plot = readfiletomem(localfile, 1);
	string_resub("<div id=\"swiki.2.1\">","</div>",plot);
	printf("plot1: %s\n", plot);
	string_striptags(plot);
	printf("plot: %s\n", plot);
	plot = string_decode(plot,1);
	printf("plot: %s\n", plot);
	changetext(skin_plot, plot);	
	
//	if(string_find("<p class=\"plotpar\">",plot))
//	{
//		string_resub("<p class=\"plotpar\">","<i>",plot);
//		strstrip(plot);
//		printf("plot: %s\n", plot);
//changetext(skin_plot, plot);
//	}

drawscreen(imdbskin, 0);
//imdbskin->hidden = NO;
	struct skin* tmp = NULL;
	while(1)
	{
//		addscreenrc(mc_videoplayer_settings, tmp);
		rcret = waitrc(imdbskin, 0, 0);
//		tmp = listbox->select;
	
		if(rcret == getrcconfigint("rcexit", NULL)) break;
		if(rcret == getrcconfigint("rcok", NULL))
		{
			break;
		}
  }


	printf("done:\n");

	free(tmpstr), tmpstr = NULL;
	free(url), url = NULL;
	free(apiSearchTV), apiSearchTV = NULL;
	free(apiImdbLookup), apiImdbLookup = NULL;
	free(apiSearch), apiSearch = NULL;
	free(localfile), localfile = NULL;
	free(fetchurl), fetchurl = NULL;
	free(search), search = NULL;
	free(imdbid), imdbid = NULL;
	free(plot), plot = NULL;
	free(director), director = NULL;
	free(writers), writers = NULL;
	free(genre), genre = NULL;
	free(tagline), tagline = NULL;
	free(releasetime), releasetime = NULL;
	free(pagebigcover), pagebigcover = NULL;
	free(bigcover), bigcover = NULL;
	free(cover), cover = NULL;
	free(cast), cast = NULL;

	delownerrc(imdbskin);
	clearscreen(imdbskin);
	drawscreen(skin, 0);

	setosdtransparent(getskinconfigint("osdtransparent", NULL));
	servicestart(status.lastservice->channel, NULL, NULL, 0);
	
//	return apiSearch;
	return;
}

#endif