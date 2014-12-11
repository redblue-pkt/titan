#ifndef WEATHER_H
#define WEATHER_H

struct weather
{
	char* city;
	char* date;
	char* day0;
	char* day0_low;
	char* day0_high;
	char* day0_condition;
	char* day0_icon;
	char* day0_temp;
	char* day0_humidity;
	char* day0_wind;
	char* day1;
	char* day1_low;
	char* day1_high;
	char* day1_condition;
	char* day1_icon;
	char* day2;
	char* day2_low;
	char* day2_high;
	char* day2_condition;
	char* day2_icon;
	char* day3;
	char* day3_low;
	char* day3_high;
	char* day3_condition;
	char* day3_icon;
};

int writeweather(const char *filename, struct skin* listbox);

char* readweather(const char* filename, struct skin* weather, struct skin* listbox)
{
	debug(1000, "in");
	FILE *fd = NULL;
	char *fileline = NULL;
	char *location = NULL;
	struct skin* tmp = NULL;
	int len = 0;

	if(weather == NULL || listbox == NULL) return NULL;

	fileline = malloc(MINMALLOC);
	if(fileline == NULL)
	{
		err("no memory");
		return NULL;
	}

	fd = fopen(filename, "r");
	if(fd == NULL)
	{
		perr("can't open %s", filename);
		free(fileline);
		return NULL;
	}

	while(fgets(fileline, MINMALLOC, fd) != NULL)
	{
		if(fileline[0] == '#' || fileline[0] == '\n')
			continue;
		len = strlen(fileline) - 1;
		if(len >= 0 && fileline[len] == '\n')
			fileline[len] = '\0';
		len--;
		if(len >= 0 && fileline[len] == '\r')
			fileline[len] = '\0';

		tmp = addlistbox(weather, listbox, tmp, 1);
		if(tmp != NULL)
		{
			changetext(tmp, fileline);
			changename(tmp, fileline);
			if(location == NULL)
			{
				location = ostrcat(location, fileline, 1, 0);
				location = ostrshrink(location);
			}
		}

	}

	free(fileline);
	fclose(fd);
	return location;
}

void freeweather(struct weather* node)
{
	if(node == NULL) return;

	free(node->city); node->city = NULL;
	free(node->date); node->date = NULL;

	free(node->day0); node->day0 = NULL;
	free(node->day0_low); node->day0_low = NULL;
	free(node->day0_high); node->day0_high = NULL;
	free(node->day0_condition); node->day0_condition = NULL;
	free(node->day0_icon); node->day0_icon = NULL;
	free(node->day0_temp); node->day0_temp = NULL;
	free(node->day0_humidity); node->day0_humidity = NULL;
	free(node->day0_wind); node->day0_wind = NULL;

	free(node->day1); node->day1 = NULL;
	free(node->day1_low); node->day1_low = NULL;
	free(node->day1_high); node->day1_high = NULL;
	free(node->day1_condition); node->day1_condition = NULL;
	free(node->day1_icon); node->day1_icon = NULL;

	free(node->day2); node->day2 = NULL;
	free(node->day2_low); node->day2_low = NULL;
	free(node->day2_high); node->day2_high = NULL;
	free(node->day2_condition); node->day2_condition = NULL;
	free(node->day2_icon); node->day2_icon = NULL;

	free(node->day3); node->day3 = NULL;
	free(node->day3_low); node->day3_low = NULL;
	free(node->day3_high); node->day3_high = NULL;
	free(node->day3_condition); node->day3_condition = NULL;
	free(node->day3_icon); node->day3_icon = NULL;

	free(node); node = NULL;
}

struct weather* getweather(char* location)
{
	struct weather* weather = NULL;
	char* tmpstr = NULL, *tmpstr1 = NULL, *tmpstr2 = NULL;
	char* tmpsearch = NULL;

	if(location == NULL) return NULL;

	char* lang = NULL;
	lang = ostrcat(_("en-GB"), NULL, 0, 0);

	tmpsearch = ostrcat("data.aspx?src=vista&weadegreetype=C&culture=", lang, 0, 0);
	tmpsearch = ostrcat(tmpsearch, "&weasearchstr=", 1, 0);
	tmpsearch = ostrcat(tmpsearch, location, 1, 0);
	free(lang), lang = NULL;   

	//tmpsearch = ostrcat("data.aspx?src=vista&weadegreetype=C&culture=de-DE&weasearchstr=", location, 0, 0);
	//tmpsearch = ostrcat("ig/api?weather=", location, 0, 0);
	//TODO: implement auto language (from titan.cfg)
	//tmpsearch = ostrcat(tmpsearch, "&hl=de", 1, 0);
	//tmpsearch = stringreplacechar(tmpsearch, ' ', '+');

	tmpstr = gethttp("weather.service.msn.com", tmpsearch, 80, NULL, NULL, 5000, NULL, 0);
	//tmpstr = gethttp("www.google.com", tmpsearch, 80, NULL, NULL, 5000, NULL, 0);

	free(tmpsearch); tmpsearch = NULL;

	if(tmpstr != NULL)
	{
		weather = (struct weather*)malloc(sizeof(struct weather));
		if(weather == NULL)
		{
			err("no mem");
			free(tmpstr); tmpstr = NULL;
			return NULL;
		}
		memset(weather, 0, sizeof(struct weather));

		tmpstr1 = tmpstr;
		tmpstr2 = tmpstr;

		tmpstr2 = ostrstr(tmpstr1, "<weather weatherlocationcode=");
		if(tmpstr2 != NULL)
		{
			tmpstr1 = tmpstr2 + 5;
			weather->city = getxmlentry(tmpstr2, "weatherlocationname=");
		}

		tmpstr2 = ostrstr(tmpstr1, "<current_conditions>");
		tmpstr2 = ostrstr(tmpstr1, "<current ");
		if(tmpstr2 != NULL)
		{
			tmpstr1 = tmpstr2 + 5;
			weather->day0_temp = getxmlentry(tmpstr2, "temperature=");
			weather->date = getxmlentry(tmpstr2, "date=");
			weather->day0_humidity = getxmlentry(tmpstr2, "humidity=");
			weather->day0_wind = getxmlentry(tmpstr2, "windspeed=");
		}

		tmpstr2 = ostrstr(tmpstr1, "<forecast ");
		if(tmpstr2 != NULL)
		{
			tmpstr1 = tmpstr2 + 5;
			weather->day0_low = getxmlentry(tmpstr2, "low=");
			weather->day0_high = getxmlentry(tmpstr2, "high=");
			weather->day0_icon = getxmlentry(tmpstr2, "skycodeday=");
			weather->day0_condition = getxmlentry(tmpstr2, "skytextday=");
			weather->day0 = getxmlentry(tmpstr2, " shortday=");
		}

		tmpstr2 = ostrstr(tmpstr1, "<forecast ");
		if(tmpstr2 != NULL)
		{
			tmpstr1 = tmpstr2 + 5;
			weather->day1_low = getxmlentry(tmpstr2, "low=");
			weather->day1_high = getxmlentry(tmpstr2, "high=");
			weather->day1_icon = getxmlentry(tmpstr2, "skycodeday=");
			weather->day1_condition = getxmlentry(tmpstr2, "skytextday=");
			weather->day1 = getxmlentry(tmpstr2, " shortday=");
		}

		tmpstr2 = ostrstr(tmpstr1, "<forecast ");
		if(tmpstr2 != NULL)
		{
			tmpstr1 = tmpstr2 + 5;
			weather->day2_low = getxmlentry(tmpstr2, "low=");
			weather->day2_high = getxmlentry(tmpstr2, "high=");
			weather->day2_icon = getxmlentry(tmpstr2, "skycodeday=");
			weather->day2_condition = getxmlentry(tmpstr2, "skytextday=");
			weather->day2 = getxmlentry(tmpstr2, " shortday=");
		}

		tmpstr2 = ostrstr(tmpstr1, "<forecast ");
		if(tmpstr2 != NULL)
		{
			tmpstr1 = tmpstr2 + 5;
			weather->day3_low = getxmlentry(tmpstr2, "low=");
			weather->day3_high = getxmlentry(tmpstr2, "high=");
			weather->day3_icon = getxmlentry(tmpstr2, "skycodeday=");
			weather->day3_condition = getxmlentry(tmpstr2, "skytextday=");
			weather->day3 = getxmlentry(tmpstr2, " shortday=");
		}

		free(tmpstr); tmpstr = NULL;
	}

	return weather;
}

void changeweatherpic(struct skin* node, char* icon)
{

		/*
		0 => 'thunderstorm',
		1 => 'thunderstorm',
		2 => 'thunderstorm',
		3 => 'thunderstorm',
		4 => 'thunderstorm',
		5 => 'rain_snow',
		6 => 'sleet',
		7 => 'rain_snow',
		8 => 'icy',
		9 => 'icy',
		10 => 'rain_snow',
		11 => 'showers',
		12 => 'rain',
		13 => 'flurries',
		14 => 'snow',
		15 => 'snow',
		16 => 'snow',
		17 => 'thunderstorm',
		18 => 'showers',
		19 => 'dust',
		20 => 'fog',
		21 => 'haze',
		22 => 'haze',
		23 => 'windy',
		24 => 'windy',
		25 => 'icy',
		26 => 'cloudy',
		27 => 'mostly_cloudy',
		28 => 'mostly_cloudy',
		29 => 'partly_cloudy',
		30 => 'partly_cloudy',
		31 => 'sunny',
		32 => 'sunny',
		33 => 'mostly_sunny',
		34 => 'mostly_sunny',
		35 => 'thunderstorm',
		36 => 'hot',
		37 => 'chance_of_tstorm',
		38 => 'chance_of_tstorm',
		39 => 'chance_of_rain',
		40 => 'showers',
		41 - 'chance_of_snow',
		42 => 'snow',
		43 => 'snow',
		44 => 'na',
		45 => 'chance_of_rain',
		46 => 'chance_of_snow',
		47 => 'chance_of_tstorm');
		*/

	if(icon == NULL)
		return;

	else if(ostrstr(icon, "10") != NULL)
		changepic(node, "%pluginpath%/weather/skin/chance_of_snow.png");
	else if(ostrstr(icon, "11") != NULL)
		changepic(node, "%pluginpath%/weather/skin/showers.png");
	else if(ostrstr(icon, "12") != NULL)
		changepic(node, "%pluginpath%/weather/skin/rain.png");
	else if(ostrstr(icon, "13") != NULL)
		changepic(node, "%pluginpath%/weather/skin/flurries.png");
	else if(ostrstr(icon, "14") != NULL)
		changepic(node, "%pluginpath%/weather/skin/snow.png");
	else if(ostrstr(icon, "15") != NULL)
		changepic(node, "%pluginpath%/weather/skin/snow.png");
	else if(ostrstr(icon, "16") != NULL)
		changepic(node, "%pluginpath%/weather/skin/snow.png");
	else if(ostrstr(icon, "17") != NULL)
		changepic(node, "%pluginpath%/weather/skin/rain.png");
	else if(ostrstr(icon, "18") != NULL)
		changepic(node, "%pluginpath%/weather/skin/showers.png");
	else if(ostrstr(icon, "19") != NULL)
		changepic(node, "%pluginpath%/weather/skin/fog.png");
	else if(ostrstr(icon, "20") != NULL)
		changepic(node, "%pluginpath%/weather/skin/fog.png");
	else if(ostrstr(icon, "21") != NULL)
		changepic(node, "%pluginpath%/weather/skin/fog.png");
	else if(ostrstr(icon, "22") != NULL)
		changepic(node, "%pluginpath%/weather/skin/fog.png");
	else if(ostrstr(icon, "23") != NULL)
		changepic(node, "%pluginpath%/weather/skin/chance_of_storm.png");
	else if(ostrstr(icon, "24") != NULL)
		changepic(node, "%pluginpath%/weather/skin/chance_of_storm.png");
	else if(ostrstr(icon, "25") != NULL)
		changepic(node, "%pluginpath%/weather/skin/icy.png");
	else if(ostrstr(icon, "26") != NULL)
		changepic(node, "%pluginpath%/weather/skin/cloudy.png");
	else if(ostrstr(icon, "27") != NULL)
		changepic(node, "%pluginpath%/weather/skin/partly_cloudy.png");
	else if(ostrstr(icon, "28") != NULL)
		changepic(node, "%pluginpath%/weather/skin/partly_cloudy.png");
	else if(ostrstr(icon, "29") != NULL)
		changepic(node, "%pluginpath%/weather/skin/partly_cloudy.png");
	else if(ostrstr(icon, "30") != NULL)
		changepic(node, "%pluginpath%/weather/skin/partly_cloudy.png");
	else if(ostrstr(icon, "31") != NULL)
		changepic(node, "%pluginpath%/weather/skin/sunny.png");
	else if(ostrstr(icon, "32") != NULL)
		changepic(node, "%pluginpath%/weather/skin/sunny.png");
	else if(ostrstr(icon, "33") != NULL)
		changepic(node, "%pluginpath%/weather/skin/mostly_sunny.png");
	else if(ostrstr(icon, "34") != NULL)
		changepic(node, "%pluginpath%/weather/skin/mostly_sunny.png");
	else if(ostrstr(icon, "35") != NULL)
		changepic(node, "%pluginpath%/weather/skin/rain.png");
	else if(ostrstr(icon, "36") != NULL)
		changepic(node, "%pluginpath%/weather/skin/sunny.png");
	else if(ostrstr(icon, "37") != NULL)
		changepic(node, "%pluginpath%/weather/skin/chance_of_storm.png");
	else if(ostrstr(icon, "38") != NULL)
		changepic(node, "%pluginpath%/weather/skin/chance_of_storm.png");
	else if(ostrstr(icon, "39") != NULL)
		changepic(node, "%pluginpath%/weather/skin/chance_of_rain.png");
	else if(ostrstr(icon, "40") != NULL)
		changepic(node, "%pluginpath%/weather/skin/showers.png");
	else if(ostrstr(icon, "41") != NULL)
		changepic(node, "%pluginpath%/weather/skin/chance_of_snow.png");
	else if(ostrstr(icon, "42") != NULL)
		changepic(node, "%pluginpath%/weather/skin/snow.png");
	else if(ostrstr(icon, "43") != NULL)
		changepic(node, "%pluginpath%/weather/skin/snow.png");
	else if(ostrstr(icon, "44") != NULL)
		changepic(node, "%pluginpath%/weather/skin/xxxx.png");
	else if(ostrstr(icon, "45") != NULL)
		changepic(node, "%pluginpath%/weather/skin/chance_of_rain.png");
	else if(ostrstr(icon, "46") != NULL)
		changepic(node, "%pluginpath%/weather/skin/chance_of_snow.png");
	else if(ostrstr(icon, "47") != NULL)
		changepic(node, "%pluginpath%/weather/skin/chance_of_rain.png");
	else if(ostrstr(icon, "0") != NULL)
		changepic(node, "%pluginpath%/weather/skin/rain.png");
	else if(ostrstr(icon, "1") != NULL)
		changepic(node, "%pluginpath%/weather/skin/rain.png");
	else if(ostrstr(icon, "2") != NULL)
		changepic(node, "%pluginpath%/weather/skin/rain.png");
	else if(ostrstr(icon, "3") != NULL)
		changepic(node, "%pluginpath%/weather/skin/rain.png");
	else if(ostrstr(icon, "4") != NULL)
		changepic(node, "%pluginpath%/weather/skin/rain.png");
	else if(ostrstr(icon, "5") != NULL)
		changepic(node, "%pluginpath%/weather/skin/chance_of_snow.png");
	else if(ostrstr(icon, "6") != NULL)
		changepic(node, "%pluginpath%/weather/skin/rain.png");
	else if(ostrstr(icon, "7") != NULL)
		changepic(node, "%pluginpath%/weather/skin/chance_of_snow.png");
	else if(ostrstr(icon, "8") != NULL)
		changepic(node, "%pluginpath%/weather/skin/icy.png");
	else if(ostrstr(icon, "9") != NULL)
		changepic(node, "%pluginpath%/weather/skin/icy.png");
}

void screenweather()
{
	int rcret = 0;
	struct skin* weather = getscreen("weather");
	struct skin* listbox = getscreennode(weather, "listbox");
	struct skin* date = getscreennode(weather, "date");
	struct skin* day0 = getscreennode(weather, "day0");
	struct skin* day0_low = getscreennode(weather, "day0_low");
	struct skin* day0_high = getscreennode(weather, "day0_high");
	struct skin* day0_condition = getscreennode(weather, "day0_condition");
	struct skin* day0_icon = getscreennode(weather, "day0_icon");
	struct skin* day1 = getscreennode(weather, "day1");
	struct skin* day1_low = getscreennode(weather, "day1_low");
	struct skin* day1_high = getscreennode(weather, "day1_high");
	struct skin* day1_condition = getscreennode(weather, "day1_condition");
	struct skin* day1_icon = getscreennode(weather, "day1_icon");
	struct skin* day2 = getscreennode(weather, "day2");
	struct skin* day2_low = getscreennode(weather, "day2_low");
	struct skin* day2_high = getscreennode(weather, "day2_high");
	struct skin* day2_condition = getscreennode(weather, "day2_condition");
	struct skin* day2_icon = getscreennode(weather, "day2_icon");
	struct skin* day3 = getscreennode(weather, "day3");
	struct skin* day3_low = getscreennode(weather, "day3_low");
	struct skin* day3_high = getscreennode(weather, "day3_high");
	struct skin* day3_condition = getscreennode(weather, "day3_condition");
	struct skin* day3_icon = getscreennode(weather, "day3_icon");
	struct weather* node = NULL;
	char* tmpstr = NULL, *location = NULL;

	location = readweather(getconfig("weatherfile", NULL), weather, listbox);
	addscreenrc(weather, listbox);

start:

	tmpstr = ostrcat(_("Weather"), " - ", 0, 0);
	tmpstr = ostrcat(tmpstr, location, 1, 0);
	changetitle(weather, tmpstr);
	free(tmpstr); tmpstr = NULL;

	node = getweather(location);
	free(location); location = NULL;

	if(node != NULL)
	{
		changetext(date, node->date);

		tmpstr = ostrcat(_("Day: "), node->day0, 0, 0);
		changetext(day0, tmpstr);
		free(tmpstr); tmpstr = NULL;

		tmpstr = ostrcat(_("Lowest temp: "), node->day0_low, 0, 0);
		changetext(day0_low, tmpstr);
		free(tmpstr); tmpstr = NULL;

		tmpstr = ostrcat(_("Highest temp: "), node->day0_high, 0, 0);
		changetext(day0_high, tmpstr);
		free(tmpstr); tmpstr = NULL;

		tmpstr = ostrcat(_("Condition: "), node->day0_condition, 0, 0);
		changetext(day0_condition, tmpstr);
		free(tmpstr); tmpstr = NULL;

		changeweatherpic(day0_icon, node->day0_icon);

		tmpstr = ostrcat(_("Day: "), node->day1, 0, 0);
		changetext(day1, tmpstr);
		free(tmpstr); tmpstr = NULL;

		tmpstr = ostrcat(_("Lowest temp: "), node->day1_low, 0, 0);
		changetext(day1_low, tmpstr);
		free(tmpstr); tmpstr = NULL;

		tmpstr = ostrcat(_("Highest temp: "), node->day1_high, 0, 0);
		changetext(day1_high, tmpstr);
		free(tmpstr); tmpstr = NULL;

		tmpstr = ostrcat(_("Condition: "), node->day1_condition, 0, 0);
		changetext(day1_condition, tmpstr);
		free(tmpstr); tmpstr = NULL;

		changeweatherpic(day1_icon, node->day1_icon);

		tmpstr = ostrcat(_("Day: "), node->day2, 0, 0);
		changetext(day2, tmpstr);
		free(tmpstr); tmpstr = NULL;

		tmpstr = ostrcat(_("Lowest temp: "), node->day2_low, 0, 0);
		changetext(day2_low, tmpstr);
		free(tmpstr); tmpstr = NULL;

		tmpstr = ostrcat(_("Highest temp: "), node->day2_high, 0, 0);
		changetext(day2_high, tmpstr);
		free(tmpstr); tmpstr = NULL;

		tmpstr = ostrcat(_("Condition: "), node->day2_condition, 0, 0);
		changetext(day2_condition, tmpstr);
		free(tmpstr); tmpstr = NULL;

		changeweatherpic(day2_icon, node->day2_icon);

		tmpstr = ostrcat(_("Day: "), node->day3, 0, 0);
		changetext(day3, tmpstr);
		free(tmpstr); tmpstr = NULL;

		tmpstr = ostrcat(_("Lowest temp: "), node->day3_low, 0, 0);
		changetext(day3_low, tmpstr);
		free(tmpstr); tmpstr = NULL;

		tmpstr = ostrcat(_("Highest temp: "), node->day3_high, 0, 0);
		changetext(day3_high, tmpstr);
		free(tmpstr); tmpstr = NULL;

		tmpstr = ostrcat(_("Condition: "), node->day3_condition, 0, 0);
		changetext(day3_condition, tmpstr);
		free(tmpstr); tmpstr = NULL;

		changeweatherpic(day3_icon, node->day3_icon);
	}

	drawscreen(weather, 0, 0);

	while(1)
	{
		rcret = waitrc(weather, 0, 0);

		if(rcret == getrcconfigint("rcexit", NULL)) break;

		if(rcret == getrcconfigint("rcred", NULL))
		{
			free(location); location = NULL;
			location = textinput(_("Location"), NULL);
			if(location != NULL)
			{
				struct skin* tmp = addlistbox(weather, listbox, NULL, 1);
				if(tmp != NULL)
				{
					changetext(tmp, location);
					changename(tmp, location);
				}
			}
			drawscreen(weather, 0, 0);
			if(location == NULL)
				continue;
			free(location); location = NULL;
    	}

		if(rcret == getrcconfigint("rcok", NULL))
		{
			writeweather(getconfig("weatherfile", NULL), listbox);
			continue;
		}

		if(listbox->select != NULL && rcret == getrcconfigint("rcgreen", NULL))
		{
			delscreennode(weather, listbox->select->name);
			listbox->select = NULL;
			drawscreen(weather, 0, 0);
		}

		if(listbox->select != NULL)
		{
			freeweather(node); node = NULL;
			location = ostrcat(location, listbox->select->text, 1, 0);
			goto start;
		}
	}

	delownerrc(weather);
	delmarkedscreennodes(weather, 1);
	freeweather(node); node = NULL;
	clearscreen(weather);
}

int writeweather(const char *filename, struct skin* listbox)
{
	debug(1000, "in");
	FILE *fd = NULL;
	struct skin* node = listbox;
	int ret = 0;

	fd = fopen(filename, "w");
	if(fd == NULL)
	{
		perr("can't open %s", filename);
		return 1;
	}

	while(node != NULL)
	{
		if(node->del == 1)
		{
			ret = fprintf(fd, "%s\n", node->text);
			if(ret < 0)
			{
				perr("writting file %s", filename);
			}
		}
		node = node->next;
	}

	fclose(fd);
	debug(1000, "out");
	return 0;
}

#endif
