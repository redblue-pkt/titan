
struct stimerthread* weatherthread = NULL;

struct weather
{
	char* city;
	char* date;
	char* day0;
	char* day0_long;
	char* day0_low;
	char* day0_high;
	char* day0_condition;
	char* day0_icon;
	char* day0_temp;
	char* day0_humidity;
	char* day0_wind;
	char* day1;
	char* day1_long;
	char* day1_low;
	char* day1_high;
	char* day1_condition;
	char* day1_icon;
	char* day2;
	char* day2_long;
	char* day2_low;
	char* day2_high;
	char* day2_condition;
	char* day2_icon;
	char* day3;
	char* day3_long;
	char* day3_low;
	char* day3_high;
	char* day3_condition;
	char* day3_icon;
};

void freeweather(struct weather* node)
{
	if(node == NULL) return;

	free(node->city); node->city = NULL;
	free(node->date); node->date = NULL;

	free(node->day0); node->day0 = NULL;
	free(node->day0_long); node->day0_long = NULL;
	free(node->day0_low); node->day0_low = NULL;
	free(node->day0_high); node->day0_high = NULL;
	free(node->day0_condition); node->day0_condition = NULL;
	free(node->day0_icon); node->day0_icon = NULL;
	free(node->day0_temp); node->day0_temp = NULL;
	free(node->day0_humidity); node->day0_humidity = NULL;
	free(node->day0_wind); node->day0_wind = NULL;

	free(node->day1); node->day1 = NULL;
	free(node->day1_long); node->day1_long = NULL;
	free(node->day1_low); node->day1_low = NULL;
	free(node->day1_high); node->day1_high = NULL;
	free(node->day1_condition); node->day1_condition = NULL;
	free(node->day1_icon); node->day1_icon = NULL;

	free(node->day2); node->day2 = NULL;
	free(node->day2_long); node->day2_long = NULL;
	free(node->day2_low); node->day2_low = NULL;
	free(node->day2_high); node->day2_high = NULL;
	free(node->day2_condition); node->day2_condition = NULL;
	free(node->day2_icon); node->day2_icon = NULL;

	free(node->day3); node->day3 = NULL;
	free(node->day3_long); node->day3_long = NULL;
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

	tmpsearch = ostrcat("data.aspx?src=vista&weadegreetype=C&culture=de-DE&weasearchstr=", location, 0, 0);
	//TODO: implement auto language (from titan.cfg)
	//tmpsearch = ostrcat(tmpsearch, "&hl=de", 1, 0);
//	tmpsearch = stringreplacechar(tmpsearch, ' ', '+');

	printf("[lcd_weather] tmpsearch: %s\n", tmpsearch);
	tmpstr = gethttp("weather.service.msn.com", tmpsearch, 80, NULL, NULL, 5000, NULL, 0);
	printf("[lcd_weather] tmpstr: %s\n", tmpstr);
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

		tmpstr2 = ostrstr(tmpstr1, "<current ");
		if(tmpstr2 != NULL)
		{
			tmpstr1 = tmpstr2 + 5;
			weather->day0_temp = getxmlentry(tmpstr2, "temperature=");
			weather->date = getxmlentry(tmpstr2, "date=");
			weather->day0_humidity = getxmlentry(tmpstr2, "humidity=");
			weather->day0_wind = getxmlentry(tmpstr2, "windspeed=");
			weather->day0_icon = getxmlentry(tmpstr2, "skycode=");
			weather->day0_condition = getxmlentry(tmpstr2, "skytext=");
			weather->day0 = getxmlentry(tmpstr2, " shortday=");
			weather->day0_long = getxmlentry(tmpstr2, " day=");
			
		}
		
		tmpstr2 = ostrstr(tmpstr1, "<forecast ");
		if(tmpstr2 != NULL)
		{
			tmpstr1 = tmpstr2 + 5;
			//day0 jetzt aktueller tag
			//weather->day0_low = getxmlentry(tmpstr2, "low=");
			//weather->day0_high = getxmlentry(tmpstr2, "high=");
			//weather->day0_icon = getxmlentry(tmpstr2, "skycodeday=");
			//weather->day0_condition = getxmlentry(tmpstr2, "skytextday=");
			//weather->day0 = getxmlentry(tmpstr2, " shortday=");
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
			weather->day1_long = getxmlentry(tmpstr2, " day=");
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
			weather->day2_long = getxmlentry(tmpstr2, " day=");
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
			weather->day3_long = getxmlentry(tmpstr2, " day=");
		}

		free(tmpstr); tmpstr = NULL;
	}

	return weather;
}

char* changeweatherpic(char* icon)
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
	
	char* node = NULL;
	if(icon == NULL)
		return node;

	else if(ostrstr(icon, "10") != NULL)
		node = createpluginpath("/lcdsamsung/skin/10.png", 0);
	else if(ostrstr(icon, "11") != NULL)
		node = createpluginpath("/lcdsamsung/skin/11.png", 0);
	else if(ostrstr(icon, "12") != NULL)
		node = createpluginpath("/lcdsamsung/skin/12.png", 0);		
	else if(ostrstr(icon, "13") != NULL)
		node = createpluginpath("/lcdsamsung/skin/13.png", 0);	
	else if(ostrstr(icon, "14") != NULL)
		node = createpluginpath("/lcdsamsung/skin/14.png", 0);			
	else if(ostrstr(icon, "15") != NULL)
		node = createpluginpath("/lcdsamsung/skin/15.png", 0);	
	else if(ostrstr(icon, "16") != NULL)
		node = createpluginpath("/lcdsamsung/skin/16.png", 0);	
	else if(ostrstr(icon, "17") != NULL)
		node = createpluginpath("/lcdsamsung/skin/17.png", 0);	
	else if(ostrstr(icon, "18") != NULL)
		node = createpluginpath("/lcdsamsung/skin/18.png", 0);	
	else if(ostrstr(icon, "19") != NULL)
		node = createpluginpath("/lcdsamsung/skin/19.png", 0);	
	else if(ostrstr(icon, "20") != NULL)
		node = createpluginpath("/lcdsamsung/skin/20.png", 0);	
	else if(ostrstr(icon, "21") != NULL)
		node = createpluginpath("/lcdsamsung/skin/21.png", 0);	
	else if(ostrstr(icon, "22") != NULL)
		node = createpluginpath("/lcdsamsung/skin/22.png", 0);	
	else if(ostrstr(icon, "23") != NULL)
		node = createpluginpath("/lcdsamsung/skin/23.png", 0);	
	else if(ostrstr(icon, "24") != NULL)
		node = createpluginpath("/lcdsamsung/skin/24.png", 0);	
	else if(ostrstr(icon, "25") != NULL)
		node = createpluginpath("/lcdsamsung/skin/25.png", 0);	
	else if(ostrstr(icon, "26") != NULL)
		node = createpluginpath("/lcdsamsung/skin/26.png", 0);	
	else if(ostrstr(icon, "27") != NULL)
		node = createpluginpath("/lcdsamsung/skin/27.png", 0);	
	else if(ostrstr(icon, "28") != NULL)
		node = createpluginpath("/lcdsamsung/skin/28.png", 0);
	else if(ostrstr(icon, "29") != NULL)
		node = createpluginpath("/lcdsamsung/skin/29.png", 0);	
	else if(ostrstr(icon, "30") != NULL)
		node = createpluginpath("/lcdsamsung/skin/30.png", 0);	
	else if(ostrstr(icon, "31") != NULL)
		node = createpluginpath("/lcdsamsung/skin/31.png", 0);	
	else if(ostrstr(icon, "32") != NULL)
		node = createpluginpath("/lcdsamsung/skin/32.png", 0);	
	else if(ostrstr(icon, "33") != NULL)
		node = createpluginpath("/lcdsamsung/skin/33.png", 0);	
	else if(ostrstr(icon, "34") != NULL)
		node = createpluginpath("/lcdsamsung/skin/34.png", 0);			
	else if(ostrstr(icon, "35") != NULL)
		node = createpluginpath("/lcdsamsung/skin/35.png", 0);	
	else if(ostrstr(icon, "36") != NULL)
		node = createpluginpath("/lcdsamsung/skin/36.png", 0);	
	else if(ostrstr(icon, "37") != NULL)
		node = createpluginpath("/lcdsamsung/skin/37.png", 0);	
	else if(ostrstr(icon, "38") != NULL)
		node = createpluginpath("/lcdsamsung/skin/38.png", 0);	
	else if(ostrstr(icon, "39") != NULL)
		node = createpluginpath("/lcdsamsung/skin/39.png", 0);			
	else if(ostrstr(icon, "40") != NULL)
		node = createpluginpath("/lcdsamsung/skin/40.png", 0);	
	else if(ostrstr(icon, "41") != NULL)
		node = createpluginpath("/lcdsamsung/skin/41.png", 0);	
	else if(ostrstr(icon, "42") != NULL)
		node = createpluginpath("/lcdsamsung/skin/42.png", 0);			
	else if(ostrstr(icon, "43") != NULL)
		node = createpluginpath("/lcdsamsung/skin/43.png", 0);	
	else if(ostrstr(icon, "44") != NULL)
		node = createpluginpath("/lcdsamsung/skin/44.png", 0);	
	else if(ostrstr(icon, "45") != NULL)
		node = createpluginpath("/lcdsamsung/skin/45.png", 0);	
	else if(ostrstr(icon, "46") != NULL)
		node = createpluginpath("/lcdsamsung/skin/46.png", 0);	
	else if(ostrstr(icon, "47") != NULL)
		node = createpluginpath("/lcdsamsung/skin/47.png", 0);
	else if(ostrstr(icon, "0") != NULL)
		node = createpluginpath("/lcdsamsung/skin/0.png", 0);	
	else if(ostrstr(icon, "1") != NULL)
		node = createpluginpath("/lcdsamsung/skin/1.png", 0);	
	else if(ostrstr(icon, "2") != NULL)
		node = createpluginpath("/lcdsamsung/skin/2.png", 0);	
	else if(ostrstr(icon, "3") != NULL)
		node = createpluginpath("/lcdsamsung/skin/3.png", 0);
	else if(ostrstr(icon, "4") != NULL)
		node = createpluginpath("/lcdsamsung/skin/4.png", 0);	
	else if(ostrstr(icon, "5") != NULL)
		node = createpluginpath("/lcdsamsung/skin/5.png", 0);	
	else if(ostrstr(icon, "6") != NULL)
		node = createpluginpath("/lcdsamsung/skin/6.png", 0);	
	else if(ostrstr(icon, "7") != NULL)
		node = createpluginpath("/lcdsamsung/skin/7.png", 0);	
	else if(ostrstr(icon, "8") != NULL)
		node = createpluginpath("/lcdsamsung/skin/8.png", 0);
	else if(ostrstr(icon, "9") != NULL)
		node = createpluginpath("/lcdsamsung/skin/9.png", 0);		
	else
		node = createpluginpath("/lcdsamsung/skin/na.png", 0);

// fix warning
	return node;
}

void lcd_writeweather()
{
	
	char* tmpstr = NULL;
	char* location = NULL;
	struct weather* node = NULL;
	
	location = ostrcat(location, getconfig("lcd_samsung_plugin_wetterort", NULL), 0, 0);
	//location = ostrcat(location, getconfig("lcd_pearl1_plugin_wetterplz", NULL), 0, 0);
	//location = ostrcat(location, "-", 1, 0);
	//location = ostrcat(location, getconfig("lcd_pearl1_plugin_wetterland", NULL), 1, 0);
	
	printf("[lcd_weather] location: %s\n", location);
	node = getweather(location);
	free(location); location = NULL;

	if(node != NULL)
	{
		FILE* ausg;
		ausg=fopen("/tmp/lcdweather","w");

		fprintf(ausg,"%s\n",node->date);
		fprintf(ausg,"%s\n",node->day0);
		fprintf(ausg,"%s\n",node->day0_temp);
		fprintf(ausg,"%s C\n",node->day0_temp);
		//day0 jetzt aktueller Tag
		//fprintf(ausg,"%s\n",node->day0_low);
		//fprintf(ausg,"%s C\n",node->day0_high);
		fprintf(ausg,"%s\n",node->day0_condition);
		tmpstr = changeweatherpic(node->day0_icon);
		fprintf(ausg,"%s\n",tmpstr);
		free(tmpstr); tmpstr = NULL;
		
		fprintf(ausg,"%s\n",node->day1);
		fprintf(ausg,"%s\n",node->day1_low);
		//fprintf(ausg,"%s C\n",node->day1_high);
		fprintf(ausg,"%s\n",node->day1_high);
		fprintf(ausg,"%s\n",node->day1_condition);
		tmpstr = changeweatherpic(node->day1_icon);
		fprintf(ausg,"%s\n",tmpstr);
		free(tmpstr); tmpstr = NULL;
		
		fprintf(ausg,"%s\n",node->day2);
		fprintf(ausg,"%s\n",node->day2_low);
		//fprintf(ausg,"%s C\n",node->day2_high);
		fprintf(ausg,"%s\n",node->day2_high);
		fprintf(ausg,"%s\n",node->day2_condition);
		tmpstr = changeweatherpic(node->day2_icon);
		fprintf(ausg,"%s\n",tmpstr);
		free(tmpstr); tmpstr = NULL;
		
		fprintf(ausg,"%s\n",node->day3);
		fprintf(ausg,"%s\n",node->day3_low);
		//fprintf(ausg,"%s C\n",node->day3_high);
		fprintf(ausg,"%s\n",node->day3_high);
		fprintf(ausg,"%s\n",node->day3_condition);
		tmpstr = changeweatherpic(node->day3_icon);
		fprintf(ausg,"%s\n",tmpstr);
		free(tmpstr); tmpstr = NULL;
		
		//wegen compatibilitaet hinten dran
		fprintf(ausg,"%s\n",node->city);
		fprintf(ausg,"%s\n",node->day0_wind);
		fprintf(ausg,"%s\%\n",node->day0_humidity);
		fprintf(ausg,"%s\n",node->day0_long);
		fprintf(ausg,"%s\n",node->day1_long);
		fprintf(ausg,"%s\n",node->day2_long);
		fprintf(ausg,"%s\n",node->day3_long);
			
		
		
		
		
		fclose(ausg);
		freeweather(node);
	}
	weatherthread = NULL;
}
		
