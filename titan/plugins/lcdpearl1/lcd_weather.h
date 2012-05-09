
struct stimerthread* weatherthread = NULL;

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

	tmpsearch = ostrcat("ig/api?weather=", location, 0, 0);
	//TODO: implement auto language (from titan.cfg)
	tmpsearch = ostrcat(tmpsearch, "&hl=de", 1, 0);
	tmpsearch = stringreplacechar(tmpsearch, ' ', '+');

	tmpstr = gethttp("www.google.com", tmpsearch, 80, NULL, NULL, NULL, 0);

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

		tmpstr2 = strstr(tmpstr1, "<forecast_information>");
		if(tmpstr2 != NULL)
		{
			tmpstr1 = tmpstr2 + 5;
			weather->date = getxmlentry(tmpstr2, "forecast_date data=");
			weather->city = getxmlentry(tmpstr2, "postal_code data=");
		}

		tmpstr2 = strstr(tmpstr1, "<current_conditions>");
		if(tmpstr2 != NULL)
		{
			tmpstr1 = tmpstr2 + 5;
			weather->day0_temp = getxmlentry(tmpstr2, "temp_c data=");
			weather->day0_humidity = getxmlentry(tmpstr2, "humidity data=");
			weather->day0_wind = getxmlentry(tmpstr2, "wind_condition data=");
		}
		
		tmpstr2 = strstr(tmpstr1, "<forecast_conditions>");
		if(tmpstr2 != NULL)
		{
			tmpstr1 = tmpstr2 + 5;
			weather->day0 = getxmlentry(tmpstr2, "day_of_week data=");
			weather->day0_low = getxmlentry(tmpstr2, "low data=");
			weather->day0_high = getxmlentry(tmpstr2, "high data=");
			weather->day0_condition = getxmlentry(tmpstr2, "condition data=");
			weather->day0_icon = getxmlentry(tmpstr2, "icon data=");

		}

		tmpstr2 = strstr(tmpstr1, "<forecast_conditions>");
		if(tmpstr2 != NULL)
		{
			tmpstr1 = tmpstr2 + 5;
			weather->day1 = getxmlentry(tmpstr2, "day_of_week data=");
			weather->day1_low = getxmlentry(tmpstr2, "low data=");
			weather->day1_high = getxmlentry(tmpstr2, "high data=");
			weather->day1_condition = getxmlentry(tmpstr2, "condition data=");
			weather->day1_icon = getxmlentry(tmpstr2, "icon data=");

		}

		tmpstr2 = strstr(tmpstr1, "<forecast_conditions>");
		if(tmpstr2 != NULL)
		{
			tmpstr1 = tmpstr2 + 5;
			weather->day2 = getxmlentry(tmpstr2, "day_of_week data=");
			weather->day2_low = getxmlentry(tmpstr2, "low data=");
			weather->day2_high = getxmlentry(tmpstr2, "high data=");
			weather->day2_condition = getxmlentry(tmpstr2, "condition data=");
			weather->day2_icon = getxmlentry(tmpstr2, "icon data=");

		}

		tmpstr2 = strstr(tmpstr1, "<forecast_conditions>");
		if(tmpstr2 != NULL)
		{
			tmpstr1 = tmpstr2 + 5;
			weather->day3 = getxmlentry(tmpstr2, "day_of_week data=");
			weather->day3_low = getxmlentry(tmpstr2, "low data=");
			weather->day3_high = getxmlentry(tmpstr2, "high data=");
			weather->day3_condition = getxmlentry(tmpstr2, "condition data=");
			weather->day3_icon = getxmlentry(tmpstr2, "icon data=");

		}

		free(tmpstr); tmpstr = NULL;
	}

	return weather;
}

char* changeweatherpic(char* icon)
{
	char* node = NULL;
	if(icon == NULL)
		return node;
	else if(strstr(icon, "/sunny.gif") != NULL)
		node = ostrcat(node, "%pluginpath%/lcdpearl1/skin/sunny60.png", 0, 0);
	else if(strstr(icon, "/mostly_sunny.gif") != NULL)
		node = ostrcat(node, "%pluginpath%/lcdpearl1/skin/mostly_sunny60.png", 0, 0);
  else if(strstr(icon, "/chance_of_rain.gif") != NULL)
  	node = ostrcat(node, "%pluginpath%/lcdpearl1/skin/chance_of_rain60.png", 0, 0);
  else if(strstr(icon, "/partly_cloudy.gif") != NULL)
  	node = ostrcat(node, "%pluginpath%/lcdpearl1/skin/partly_cloudy60.png", 0, 0);
  else if(strstr(icon, "/mostly_cloudy.gif") != NULL)
  	node = ostrcat(node, "%pluginpath%/lcdpearl1/skin/mostly_cloudy60.png", 0, 0);
  else if(strstr(icon, "/chance_of_storm.gif") != NULL)
  	node = ostrcat(node, "%pluginpath%/lcdpearl1/skin/chance_of_storm60.png", 0, 0);
	else if(strstr(icon, "/showers.gif") != NULL)
  	node = ostrcat(node, "%pluginpath%/lcdpearl1/skin/showers60.png", 0, 0);
  else if(strstr(icon, "/rain.gif") != NULL)
  	node = ostrcat(node, "%pluginpath%/lcdpearl1/skin/rain60.png", 0, 0);
  else if(strstr(icon, "/chance_of_snow.gif") != NULL)
  	node = ostrcat(node, "%pluginpath%/lcdpearl1/skin/chance_of_snow60.png", 0, 0);
  else if(strstr(icon, "/cloudy.gif") != NULL)
  	node = ostrcat(node, "%pluginpath%/lcdpearl1/skin/cloudy60.png", 0, 0);
  else if(strstr(icon, "/storm.gif") != NULL)
  	node = ostrcat(node, "%pluginpath%/lcdpearl1/skin/storm60.png", 0, 0);
  else if(strstr(icon, "/thunderstorm.gif") != NULL)
  	node = ostrcat(node, "%pluginpath%/lcdpearl1/skin/storm60.png", 0, 0);
  else if(strstr(icon, "/chance_of_tstorm.gif") != NULL)
  	node = ostrcat(node, "%pluginpath%/lcdpearl1/skin/chance_of_storm.png", 0, 0);
  else if(strstr(icon, "/sleet.gif") != NULL)
  	node = ostrcat(node, "%pluginpath%/lcdpearl1/skin/snow60.png", 0, 0);
  else if(strstr(icon, "/snow.gif") != NULL)
  	node = ostrcat(node, "%pluginpath%/lcdpearl1/skin/snow60.png", 0, 0);
  else if(strstr(icon, "/icy.gif") != NULL)
  	node = ostrcat(node, "%pluginpath%/lcdpearl1/skin/icy60.png", 0, 0);
  else if(strstr(icon, "/dust.gif") != NULL)
  	node = ostrcat(node, "%pluginpath%/lcdpearl1/skin/fog60.png", 0, 0);
  else if(strstr(icon, "/fog.gif") != NULL)
  	node = ostrcat(node, "%pluginpath%/lcdpearl1/skin/fog60.png", 0, 0);
  else if(strstr(icon, "/smoke.gif") != NULL)
  	node = ostrcat(node, "%pluginpath%/lcdpearl1/skin/fog60.png", 0, 0);
  else if(strstr(icon, "/haze.gif") != NULL)
  	node = ostrcat(node, "%pluginpath%/lcdpearl1/skin/fog60.png", 0, 0);
  else if(strstr(icon, "/flurries.gif") != NULL)
  	node = ostrcat(node, "%pluginpath%/lcdpearl1/skin/flurries60.png", 0, 0);
  return node;	
}

void lcd_writeweather()
{
	
	char* tmpstr = NULL;
	char* location = NULL;
	struct weather* node = NULL;
	
	location = ostrcat(location, getconfig("lcd_pearl1_plugin_wetter_ort", NULL), 0, 0);

	node = getweather(location);
	free(location); location = NULL;

	if(node != NULL)
	{
		FILE* ausg;
		ausg=fopen("/tmp/lcdweather","w");

		fprintf(ausg,"%s\n",node->date);
		fprintf(ausg,"%s\n",node->day0);
		fprintf(ausg,"%s\n",node->day0_low);
		fprintf(ausg,"%s C\n",node->day0_high);
		fprintf(ausg,"%s\n",node->day0_condition);
		tmpstr = changeweatherpic(node->day0_icon);
		fprintf(ausg,"%s\n",tmpstr);
		free(tmpstr); tmpstr = NULL;

		fprintf(ausg,"%s\n",node->day1);
		fprintf(ausg,"%s\n",node->day1_low);
		fprintf(ausg,"%s C\n",node->day1_high);
		fprintf(ausg,"%s\n",node->day1_condition);
		tmpstr = changeweatherpic(node->day1_icon);
		fprintf(ausg,"%s\n",tmpstr);
		free(tmpstr); tmpstr = NULL;
		
		fprintf(ausg,"%s\n",node->day2);
		fprintf(ausg,"%s\n",node->day2_low);
		fprintf(ausg,"%s C\n",node->day2_high);
		fprintf(ausg,"%s\n",node->day2_condition);
		tmpstr = changeweatherpic(node->day2_icon);
		fprintf(ausg,"%s\n",tmpstr);
		free(tmpstr); tmpstr = NULL;
		
		fprintf(ausg,"%s\n",node->day3);
		fprintf(ausg,"%s\n",node->day3_low);
		fprintf(ausg,"%s C\n",node->day3_high);
		fprintf(ausg,"%s\n",node->day3_condition);
		tmpstr = changeweatherpic(node->day3_icon);
		fprintf(ausg,"%s\n",tmpstr);
		free(tmpstr); tmpstr = NULL;
		
		fclose(ausg);
		freeweather(node);
		weatherthread = NULL;
	}
}
		
