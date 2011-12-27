#ifndef IMDB_H
#define IMDB_H

char* string_resub(char* str,char* str2, char* filename)
{
	debug(1000, "in");
	int i, len_str, len_filename;

	if(str == NULL || str2 == NULL || filename == NULL) return 0;

	len_str = strlen(str);
	len_filename = strlen(filename);

	int count = 0;
	for(i = 0; (i + len_str) < len_filename; i++)
	{
		if(strncmp(filename + i, str, len_str) == 0 && count == 0)
		{
			count = i + len_str;
			filename[i] = ' ';
		}
		else if(count == i && count != 0)
		{
			count = i - len_str;
			break;
		}
		else
			filename[i] = ' ';
	}
	len_str = strlen(str2);

	for(i = 0; (i + len_str) < len_filename; i++)
	{
		if(strncmp(filename + i, str2, len_str) == 0 && i >= count)
		{
			filename[i] = '\0';
			break;
		}
	}	

	debug(1000, "out");
	return filename;
}


int imdb(char* input)
{
//if(ostrcmp("vfdisplay", name) == 0) return 0;
//tmpstr = ostrcat(tmpstr, "", 1, 0);

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

	input = ostrcat("", "Days+of+Summer", 0, 0);
	url = ostrcat(url, "german.imdb.com", 1, 0);

	apiSearchTV = ostrcat("", "/search/title?title=<title>&title_type=tv_series", 0, 0);
	apiImdbLookup = ostrcat("", "/title/tt", 0, 0);
	apiSearch = ostrcat("", "/find?s=tt;q=", 0, 0);
	localfile = ostrcat("", "/tmp/cache.getMovieByTitle.html", 0, 0);

	search = ostrcat(apiSearch, input, 0, 0);
	gethttp(get_ip(url), search, 80, localfile, NULL);

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

	gethttp(get_ip(url), search, 80, localfile, NULL);
	tmpstr = readfiletomem(localfile, 1);

	title = ostrcat(tmpstr, "", 0, 0);
	string_resub("<title>","</title>",title);
	strstrip(title);
	printf("title: %s\n", title);
////////////////////
	director = ostrcat(tmpstr, "", 0, 0);
//	string_resub("<div id=\"director-info\" class=\"info\">","</div>",director);
	string_resub("Director:","</div>",director);
	char* tmpstr2 = NULL;
	struct splitstr* ret2 = NULL;
	int count2 = 0;	
	tmpstr2 = ostrcat(tmpstr2, director, 1, 0);
	ret2 = strsplit(tmpstr2, "<>", &count2);
	printf("director1: %s\n", (&ret2[4])->part);
	strstrip(director);
	printf("director: %s\n", director);

////////////////////

	writers = ostrcat(tmpstr, "", 0, 0);
//	string_resub("<div id=\"director-info\" class=\"info\">","</div>",writers);
	string_resub("Writers:\n  </h4>","</div>",writers);
//	char* tmpstr2 = NULL;
//	struct splitstr* ret2 = NULL;
	count2 = 0;	
	tmpstr2 = ostrcat(tmpstr2, writers, 1, 0);
	ret2 = strsplit(tmpstr2, "<>", &count2);
	printf("writers1: %s\n", (&ret2[2])->part);
	printf("writers2: %s\n", (&ret2[6])->part);
//	printf("writers3: %s\n", (&ret2[10])->part);
	strstrip(writers);
	printf("writers: %s\n", writers);
////////////////////

	genre = ostrcat(tmpstr, "", 0, 0);
	string_resub("<h4 class=\"inline\">Genre","</div>",genre);
	count2 = 0;	
	tmpstr2 = ostrcat(tmpstr2, genre, 1, 0);
	ret2 = strsplit(tmpstr2, "<>", &count2);
	printf("genre1: %s\n", (&ret2[4])->part);
	printf("genre2: %s\n", (&ret2[12])->part);
	strstrip(genre);
	printf("genre: %s\n", genre);
////////////////////

	tagline = ostrcat(tmpstr, "", 0, 0);
	string_resub("<h4 class=\"inline\">Taglines:</h4>","<span class",tagline);

	strstrip(tagline);
	printf("tagline: %s\n", tagline);

////////////////////

	releasetime = ostrcat(tmpstr, "", 0, 0);
	string_resub("<h4 class=\"inline\">Release Date:</h4>","</time>",releasetime);
	count2 = 0;	
	tmpstr2 = ostrcat(tmpstr2, releasetime, 1, 0);
	ret2 = strsplit(tmpstr2, "<>", &count2);
	printf("releasetime1: %s\n", (&ret2[2])->part);
	strstrip(releasetime);
	printf("releasetime: %s\n", releasetime);

////////////////////

	cast = ostrcat(tmpstr, "", 0, 0);
	string_resub("<h4 class=\"inline\">Stars:</h4>","</div>",cast);
	count2 = 0;	
	tmpstr2 = ostrcat(tmpstr2, cast, 1, 0);
	ret2 = strsplit(tmpstr2, "<>", &count2);
	printf("cast1: %s\n", (&ret2[2])->part);
	printf("cast2: %s\n", (&ret2[6])->part);
	printf("cast3: %s\n", (&ret2[10])->part);
	strstrip(cast);
	printf("cast: %s\n", cast);

////////////////////

	pagebigcover = ostrcat(tmpstr, "", 0, 0);
	string_resub("href=\"/media/rm","/",pagebigcover);

	strstrip(pagebigcover);
	printf("pagebigcover: %s\n", pagebigcover);

	char* tmpstr3 = NULL;
	tmpstr3 = ostrcat("href=\"/media/rm", pagebigcover, 0, 0);
	tmpstr3 = ostrcat(tmpstr3, "/tt", 1, 0);
	tmpstr3 = ostrcat(tmpstr3, imdbid, 1, 0);
	tmpstr3 = ostrcat(tmpstr3, "\"    ><img src=\"", 1, 0);

	cover = ostrcat(tmpstr, "", 0, 0);
	string_resub(tmpstr3,"\"",cover);

	printf("tmpstr3: %s\n", tmpstr3);
	strstrip(cover);
	printf("cover: %s\n", cover);

////////////////////
	search = ostrcat("/media/rm", pagebigcover, 0, 0);
	search = ostrcat(search, "/tt", 1, 0);
	search = ostrcat(search, imdbid, 1, 0);
	search = ostrcat(search, "/", 1, 0);

	localfile = ostrcat("", "/tmp/cache.getpageBigCoverHtml.html", 0, 0);
	printf("search: %s\n", search);
	printf("url: %s\n", get_ip(url));
	printf("localfile: %s\n", localfile);



	gethttp(get_ip(url), search, 80, localfile, NULL);
	bigcover = readfiletomem(localfile, 1);
	string_resub("<link rel=\"image_src\" href=\"","\"",bigcover);
	strstrip(bigcover);
	printf("bigcover: %s\n", bigcover);


////////////////////

//	free(tmpstr), tmpstr = NULL;
	search = ostrcat(apiImdbLookup, imdbid, 0, 0);
	search = ostrcat(search, "/plotsummary", 1, 0);

	localfile = ostrcat("", "/tmp/cache.getPlot.html", 0, 0);
	printf("search: %s\n", search);
	printf("url: %s\n", get_ip(url));
	printf("localfile: %s\n", localfile);
	gethttp(get_ip(url), search, 80, localfile, NULL);

	plot = readfiletomem(localfile, 1);
//htmldecode(plot, plot);
//htmlencode(plot);
	if(string_find("<p class=\"plotpar\">",plot))
	{
		string_resub("<p class=\"plotpar\">","<i>",plot);
		strstrip(plot);
		printf("plot: %s\n", plot);
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

	return apiSearch;
}

#endif