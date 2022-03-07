#include "../../titan/struct.h"
#include "../../titan/debug.h"
#include "../../titan/header.h"

#include "lcd_weather.h"

char pluginname[] = "LCD Samsung SPF..";
char plugindesc[] = "Extensions";
char pluginpic[] = "%pluginpath%/lcdsamsung/lcdsamsung.png";

int pluginaktiv = 0;
//int pluginversion = PLUGINVERSION;
int pluginversion = 999999;

struct stimerthread* LCD_Samsung1thread = NULL;

int firststart = 0;

void weather_getline(FILE* fd, char* fileline)
{
	fgets(fileline, 256, fd);
	if(fileline[0] == '#' || fileline[0] == '\n')
		return;
	if(fileline[strlen(fileline) - 1] == '\n')
		fileline[strlen(fileline) - 1] = '\0';
	if(fileline[strlen(fileline) - 1] == '\r')
		fileline[strlen(fileline) - 1] = '\0';
}

void LCD_start_lcd4linux()
{
	int count = 0;
	char* startlcd = NULL;
	
	if(ostrcmp(getconfig("lcd_samsung_plugin_type", NULL), "spf75h") == 0)
	{
		startlcd = createpluginpath("/lcdsamsung/start.sh", 0);
		startlcd = ostrcat(startlcd, " 1", 1, 0);
	}
	else if(ostrcmp(getconfig("lcd_samsung_plugin_type", NULL), "spf83h") == 0)
	{
		startlcd = createpluginpath("/lcdsamsung/start.sh", 0);
		startlcd = ostrcat(startlcd, " 6", 1, 0);
	}
	else if(ostrcmp(getconfig("lcd_samsung_plugin_type", NULL), "spf83m") == 0)
	{
		startlcd = createpluginpath("/lcdsamsung/start.sh", 0);
		startlcd = ostrcat(startlcd, " 10", 1, 0);
	}
	else if(ostrcmp(getconfig("lcd_samsung_plugin_type", NULL), "spf87hold") == 0)
	{
		startlcd = createpluginpath("/lcdsamsung/start.sh", 0);
		startlcd = ostrcat(startlcd, " 3", 1, 0);
	}
	else if(ostrcmp(getconfig("lcd_samsung_plugin_type", NULL), "spf87h") == 0)
	{
		startlcd = createpluginpath("/lcdsamsung/start.sh", 0);
		startlcd = ostrcat(startlcd, " 2", 1, 0);
	}
	else if(ostrcmp(getconfig("lcd_samsung_plugin_type", NULL), "spf105p") == 0)
	{
		startlcd = createpluginpath("/lcdsamsung/start.sh", 0);
		startlcd = ostrcat(startlcd, " 5", 1, 0);
	}
	else if(ostrcmp(getconfig("lcd_samsung_plugin_type", NULL), "spf107h") == 0)
	{
		startlcd = createpluginpath("/lcdsamsung/start.sh", 0);
		startlcd = ostrcat(startlcd, " 4", 1, 0);
	}
	else if(ostrcmp(getconfig("lcd_samsung_plugin_type", NULL), "spf72h") == 0)
	{
		startlcd = createpluginpath("/lcdsamsung/start.sh", 0);
		startlcd = ostrcat(startlcd, " 7", 1, 0);
	}
	else if(ostrcmp(getconfig("lcd_samsung_plugin_type", NULL), "spf85h") == 0)
	{
		startlcd = createpluginpath("/lcdsamsung/start.sh", 0);
		startlcd = ostrcat(startlcd, " 8", 1, 0);
	}
	else if(ostrcmp(getconfig("lcd_samsung_plugin_type", NULL), "spf85p") == 0)
	{
		startlcd = createpluginpath("/lcdsamsung/start.sh", 0);
		startlcd = ostrcat(startlcd, " 9", 1, 0);
	}
	else if(ostrcmp(getconfig("lcd_samsung_plugin_type", NULL), "none") == 0)
	{
		startlcd = createpluginpath("/lcdsamsung/start.sh", 0);
		startlcd = ostrcat(startlcd, " 99", 1, 0);
	}
	else if(ostrcmp(getconfig("lcd_samsung_plugin_type", NULL), "d320") == 0)
	{
		startlcd = createpluginpath("/lcdsamsung/start.sh", 0);
		startlcd = ostrcat(startlcd, " 98 ", 1, 0);
		startlcd = ostrcat(startlcd, getconfig("lcd_samsung_ip", NULL), 1, 0);
	}
	else
	{
		startlcd = createpluginpath("/lcdsamsung/start.sh", 0);
		startlcd = ostrcat(startlcd, " 2", 1, 0);
	}
	printf("cmd2: %s\n", startlcd);
	
	
	if(LCD_Samsung1thread == NULL)
		return;
	while (LCD_Samsung1thread->aktion != STOP && (system("ps | grep -v grep | grep fbread") != 0 && system("ps -A | grep -v grep | grep fbread") != 0)) {
		system(startlcd);
		sleep(6);
		count ++;
		if(LCD_Samsung1thread == NULL)
			break;
		if(count == 101) {
			textbox(_("Message"), _("ERROR cant start LCD driver"), _("OK"), getrcconfigint("rcok", NULL), NULL, 0, NULL, 0, NULL, 0, 600, 200, 5, 0);
			LCD_Samsung1thread->aktion = STOP;
		}
	}
	free(startlcd);startlcd=NULL;	
}

void LCD_Samsung1_thread()
{
	
	struct skin* LCD_Samsung1 = NULL;
	struct skin* LCD_Standby = NULL;
	struct skin* LCD_Play = NULL;
	struct skin* LCD_Music = NULL;
	
	
	struct skin* city = NULL;
	struct skin* wind = NULL;
	struct skin* luft = NULL;
	struct skin* day0_t = NULL;
	struct skin* day0_tm = NULL;
	struct skin* day0_i = NULL;
	struct skin* day0_d = NULL;
	struct skin* day0_dl = NULL;
	struct skin* day1_t = NULL;
	struct skin* day1_tm = NULL;
	struct skin* day1_i = NULL;
	struct skin* day1_d = NULL;
	struct skin* day1_dl = NULL;
	struct skin* day2_t = NULL;
	struct skin* day2_tm = NULL;
	struct skin* day2_i = NULL;
	struct skin* day2_d = NULL;
	struct skin* day2_dl = NULL;
	struct skin* day3_t = NULL;
	struct skin* day3_tm = NULL;
	struct skin* day3_i = NULL;
	struct skin* day3_d = NULL;
	struct skin* day3_dl = NULL;
	struct skin* akttime = NULL;
	struct skin* akttime_Standby = NULL;
	
	struct skin* n_stunde = NULL;
	struct skin* n_minute = NULL;
	struct skin* n_stunde_standby = NULL;
	struct skin* n_minute_standby = NULL;
	
	struct skin* n_stunde2 = NULL;
	struct skin* n_minute2 = NULL;
	struct skin* n_stunde2_standby = NULL;
	struct skin* n_minute2_standby = NULL;
	
	struct skin* scity = NULL;
	struct skin* swind = NULL;
	struct skin* sluft = NULL;
	struct skin* sday0_t = NULL;
	struct skin* sday0_tm = NULL;
	struct skin* sday0_i = NULL;
	struct skin* sday0_d = NULL;
	struct skin* sday0_dl = NULL;
	struct skin* sday1_t = NULL;
	struct skin* sday1_tm = NULL;
	struct skin* sday1_i = NULL;
	struct skin* sday1_d = NULL;
	struct skin* sday1_dl = NULL;
	struct skin* sday2_t = NULL;
	struct skin* sday2_tm = NULL;
	struct skin* sday2_i = NULL;
	struct skin* sday2_d = NULL;
	struct skin* sday2_dl = NULL;
	struct skin* sday3_t = NULL;
	struct skin* sday3_tm = NULL;
	struct skin* sday3_i = NULL;
	struct skin* sday3_d = NULL;
	struct skin* sday3_dl = NULL;
	
	char* tmpstr = NULL, *tmpstr2 = NULL, *tmpstr3 = NULL, *timemerk = NULL, *sendermerk = NULL, *recmerk = NULL;
	char* pichr = NULL, *picmin = NULL, *pichr_standby = NULL, *picmin_standby = NULL;
	char* pichr2 = NULL, *picmin2 = NULL, *pichr2_standby = NULL, *picmin2_standby = NULL;
	int digitaluhr = 0, digitaluhr_standby = 0;
	FILE *fd = NULL;
	char *fileline = NULL;
	int weatherwrite = 999;
	int weatherref = 0;
	char* startlcd = NULL;
	char* bild = NULL;
	
	printf("lcd_samsung thread started\n");
	
	if(ostrcmp(getconfig("lcd_samsung_plugin_wetter", NULL), "yes") == 0)
	{
		if(ostrcmp(getconfig("lcd_samsung_plugin_type", NULL), "spf75h") == 0)
			LCD_Samsung1 = getscreen("LCD_spf75_Wetter");
		else if(ostrcmp(getconfig("lcd_samsung_plugin_type", NULL), "spf87h") == 0)
			LCD_Samsung1 = getscreen("LCD_spf87_Wetter");
		else if(ostrcmp(getconfig("lcd_samsung_plugin_type", NULL), "spf83h") == 0 || ostrcmp(getconfig("lcd_samsung_plugin_type", NULL), "spf83m") == 0)
			LCD_Samsung1 = getscreen("LCD_spf83_Wetter");
		else if(ostrcmp(getconfig("lcd_samsung_plugin_type", NULL), "spf87hold") == 0)
			LCD_Samsung1 = getscreen("LCD_spf87_Wetter");
		else if(ostrcmp(getconfig("lcd_samsung_plugin_type", NULL), "spf105p") == 0)
			LCD_Samsung1 = getscreen("LCD_spf105_Wetter");
		else if(ostrcmp(getconfig("lcd_samsung_plugin_type", NULL), "spf107h") == 0)
			LCD_Samsung1 = getscreen("LCD_spf107_Wetter");
		else if(ostrcmp(getconfig("lcd_samsung_plugin_type", NULL), "spf72h") == 0)
			LCD_Samsung1 = getscreen("LCD_spf72_Wetter");
		else if(ostrcmp(getconfig("lcd_samsung_plugin_type", NULL), "spf85h") == 0)
			LCD_Samsung1 = getscreen("LCD_spf85_Wetter");
		else if(ostrcmp(getconfig("lcd_samsung_plugin_type", NULL), "spf85p") == 0)
			LCD_Samsung1 = getscreen("LCD_spf85_Wetter");
	  else if(ostrcmp(getconfig("lcd_samsung_plugin_type", NULL), "d320") == 0)
			LCD_Samsung1 = getscreen("LCD_spf_d320_Wetter");
		else
			LCD_Samsung1 = getscreen("LCD_spf87_Wetter");
		city = getscreennode(LCD_Samsung1, "city");
		wind = getscreennode(LCD_Samsung1, "wind");
		luft = getscreennode(LCD_Samsung1, "luft");
		day0_t = getscreennode(LCD_Samsung1, "day0_t");
		day0_tm = getscreennode(LCD_Samsung1, "day0_tm");
		day0_i = getscreennode(LCD_Samsung1, "day0_i");
		day0_d = getscreennode(LCD_Samsung1, "day0_d");
		day0_dl = getscreennode(LCD_Samsung1, "day0_dl");
		day1_t = getscreennode(LCD_Samsung1, "day1_t");
		day1_tm = getscreennode(LCD_Samsung1, "day1_tm");
		day1_i = getscreennode(LCD_Samsung1, "day1_i");
		day1_d = getscreennode(LCD_Samsung1, "day1_d");
		day1_dl = getscreennode(LCD_Samsung1, "day1_dl");
		day2_t = getscreennode(LCD_Samsung1, "day2_t");
		day2_tm = getscreennode(LCD_Samsung1, "day2_tm");
		day2_i = getscreennode(LCD_Samsung1, "day2_i");
		day2_d = getscreennode(LCD_Samsung1, "day2_d");
		day2_dl = getscreennode(LCD_Samsung1, "day2_dl");
		day3_t = getscreennode(LCD_Samsung1, "day3_t");
		day3_tm = getscreennode(LCD_Samsung1, "day3_tm");
		day3_i = getscreennode(LCD_Samsung1, "day3_i");
		day3_d = getscreennode(LCD_Samsung1, "day3_d");
		day3_dl = getscreennode(LCD_Samsung1, "day3_dl");
		akttime = getscreennode(LCD_Samsung1, "akttime");
		akttime = getscreennode(LCD_Samsung1, "akttime");
		n_stunde =  getscreennode(LCD_Samsung1, "stunde");
		if(n_stunde != NULL)
		{
			pichr = ostrcat(n_stunde->pic, "", 0, 0);
			if(ostrstr(pichr, "Digital") != NULL)
				digitaluhr = 1;
		}
		n_minute =  getscreennode(LCD_Samsung1, "minute");
		if(n_minute != NULL)
			picmin = ostrcat(n_minute->pic, "", 0, 0);
		if(digitaluhr == 1)
		{
			n_stunde2 =  getscreennode(LCD_Samsung1, "stunde2");
			n_minute2 =  getscreennode(LCD_Samsung1, "minute2");
			picmin2 = ostrcat(n_minute2->pic, "", 0, 0);
			pichr2 = ostrcat(n_stunde2->pic, "", 0, 0);
		}
		if(file_exist("/tmp/lcdweather"))
			system("rm /tmp/lcdweather");
		weatherwrite = 0;
	}
	else {
		if(ostrcmp(getconfig("lcd_samsung_plugin_type", NULL), "spf75h") == 0)
			LCD_Samsung1 = getscreen("LCD_spf75");
		else if(ostrcmp(getconfig("lcd_samsung_plugin_type", NULL), "spf83h") == 0 || ostrcmp(getconfig("lcd_samsung_plugin_type", NULL), "spf83m") == 0)
			LCD_Samsung1 = getscreen("LCD_spf83");
		else if(ostrcmp(getconfig("lcd_samsung_plugin_type", NULL), "spf87h") == 0)
			LCD_Samsung1 = getscreen("LCD_spf87");
		else if(ostrcmp(getconfig("lcd_samsung_plugin_type", NULL), "spf87hold") == 0)
			LCD_Samsung1 = getscreen("LCD_spf87");
		else if(ostrcmp(getconfig("lcd_samsung_plugin_type", NULL), "spf105p") == 0)
			LCD_Samsung1 = getscreen("LCD_spf105");
		else if(ostrcmp(getconfig("lcd_samsung_plugin_type", NULL), "spf107h") == 0)
			LCD_Samsung1 = getscreen("LCD_spf107");
		else if(ostrcmp(getconfig("lcd_samsung_plugin_type", NULL), "spf72h") == 0)
			LCD_Samsung1 = getscreen("LCD_spf72");
		else if(ostrcmp(getconfig("lcd_samsung_plugin_type", NULL), "spf85h") == 0)
			LCD_Samsung1 = getscreen("LCD_spf85");
		else if(ostrcmp(getconfig("lcd_samsung_plugin_type", NULL), "spf85p") == 0)
			LCD_Samsung1 = getscreen("LCD_spf85");
		else if(ostrcmp(getconfig("lcd_samsung_plugin_type", NULL), "d320") == 0)
			LCD_Samsung1 = getscreen("LCD_spf_d320");
		else
			LCD_Samsung1 = getscreen("LCD_spf87");	
		akttime = getscreennode(LCD_Samsung1, "akttime");
		n_stunde =  getscreennode(LCD_Samsung1, "stunde");
		if(n_stunde != NULL)
		{
			pichr = ostrcat(n_stunde->pic, "", 0, 0);
			if(ostrstr(pichr, "Digital") != NULL)
				digitaluhr = 1;
		}
		n_minute =  getscreennode(LCD_Samsung1, "minute");
		if(n_minute != NULL)
			picmin = ostrcat(n_minute->pic, "", 0, 0);
		if(digitaluhr == 1)
		{
			n_stunde2 =  getscreennode(LCD_Samsung1, "stunde2");
			n_minute2 =  getscreennode(LCD_Samsung1, "minute2");
			picmin2 = ostrcat(n_minute2->pic, "", 0, 0);
			pichr2 = ostrcat(n_stunde2->pic, "", 0, 0);
		}
	}
	
	if(ostrcmp(getconfig("lcd_samsung_plugin_type", NULL), "spf75h") == 0)
		LCD_Play = getscreen("LCD_spf75_Play");
	else if(ostrcmp(getconfig("lcd_samsung_plugin_type", NULL), "spf83h") == 0 || ostrcmp(getconfig("lcd_samsung_plugin_type", NULL), "spf83m") == 0)
		LCD_Play = getscreen("LCD_spf83_Play");
	else if(ostrcmp(getconfig("lcd_samsung_plugin_type", NULL), "spf87h") == 0)
		LCD_Play = getscreen("LCD_spf87_Play");
	else if(ostrcmp(getconfig("lcd_samsung_plugin_type", NULL), "spf87hold") == 0)
		LCD_Play = getscreen("LCD_spf87_Play");
	else if(ostrcmp(getconfig("lcd_samsung_plugin_type", NULL), "spf105p") == 0)
		LCD_Play = getscreen("LCD_spf105_Play");
	else if(ostrcmp(getconfig("lcd_samsung_plugin_type", NULL), "spf107h") == 0)
		LCD_Play = getscreen("LCD_spf107_Play");
	else if(ostrcmp(getconfig("lcd_samsung_plugin_type", NULL), "spf72h") == 0)
		LCD_Play = getscreen("LCD_spf72_Play");
	else if(ostrcmp(getconfig("lcd_samsung_plugin_type", NULL), "spf85h") == 0)
		LCD_Play = getscreen("LCD_spf85_Play");
	else if(ostrcmp(getconfig("lcd_samsung_plugin_type", NULL), "spf85p") == 0)
		LCD_Play = getscreen("LCD_spf85_Play");
	else if(ostrcmp(getconfig("lcd_samsung_plugin_type", NULL), "d320") == 0)
		LCD_Play = getscreen("LCD_spf_d320_Play");
	else
		LCD_Play = getscreen("LCD_spf87_Play");
	struct skin* akttimeplay = getscreennode(LCD_Play, "akttime");
	struct skin* sprogress = getscreennode(LCD_Play, "progress");
	struct skin* stitle = getscreennode(LCD_Play, "title1");
	struct skin* spos = getscreennode(LCD_Play, "pos");
	struct skin* slen = getscreennode(LCD_Play, "len");
	struct skin* sreverse = getscreennode(LCD_Play, "reverse");
	
	if(ostrcmp(getconfig("lcd_samsung_plugin_type", NULL), "spf75h") == 0)
		LCD_Music = getscreen("LCD_spf75_Music");
	else if(ostrcmp(getconfig("lcd_samsung_plugin_type", NULL), "spf83h") == 0 || ostrcmp(getconfig("lcd_samsung_plugin_type", NULL), "spf83h") == 0)
		LCD_Music = getscreen("LCD_spf83_Music");
	else if(ostrcmp(getconfig("lcd_samsung_plugin_type", NULL), "spf87h") == 0)
		LCD_Music = getscreen("LCD_spf87_Music");
	else if(ostrcmp(getconfig("lcd_samsung_plugin_type", NULL), "spf87hold") == 0)
		LCD_Music = getscreen("LCD_spf87_Music");
	else if(ostrcmp(getconfig("lcd_samsung_plugin_type", NULL), "spf105p") == 0)
		LCD_Music = getscreen("LCD_spf105_Music");
	else if(ostrcmp(getconfig("lcd_samsung_plugin_type", NULL), "spf107h") == 0)
		LCD_Music = getscreen("LCD_spf107_Music");
	else if(ostrcmp(getconfig("lcd_samsung_plugin_type", NULL), "spf72h") == 0)
		LCD_Music = getscreen("LCD_spf72_Music");
	else if(ostrcmp(getconfig("lcd_samsung_plugin_type", NULL), "spf85h") == 0)
		LCD_Music = getscreen("LCD_spf85_Music");
	else if(ostrcmp(getconfig("lcd_samsung_plugin_type", NULL), "spf85p") == 0)
		LCD_Music = getscreen("LCD_spf85_Music");
  else if(ostrcmp(getconfig("lcd_samsung_plugin_type", NULL), "d320") == 0)
		LCD_Music = getscreen("LCD_spf_d320_Music");
	else
		LCD_Music = getscreen("LCD_spf87_Music");
		
	struct skin* mthumb = getscreennode(LCD_Music, "lthumb");
	struct skin* mtitle = getscreennode(LCD_Music, "ltitle");
	struct skin* malbum = getscreennode(LCD_Music, "lalbum");
	struct skin* mactors = getscreennode(LCD_Music, "lactors");
	struct skin* mrealname = getscreennode(LCD_Music, "lrealname");
	struct skin* mgenre = getscreennode(LCD_Music, "lgenre");
	struct skin* myear = getscreennode(LCD_Music, "lyear");
	struct skin* makttimeplay = getscreennode(LCD_Music, "akttime");
	struct skin* mprogress = getscreennode(LCD_Music, "progress");
	struct skin* mtitle1 = getscreennode(LCD_Music, "title1");
	struct skin* mpos = getscreennode(LCD_Music, "pos");
	struct skin* mlen = getscreennode(LCD_Music, "len");
	struct skin* mreverse = getscreennode(LCD_Music, "reverse");
		
	if(ostrcmp(getconfig("lcd_samsung_plugin_type", NULL), "spf75h") == 0)
	{
		startlcd = createpluginpath("/lcdsamsung/start.sh", 0);
		startlcd = ostrcat(startlcd, " 1", 1, 0);
	}
	else if(ostrcmp(getconfig("lcd_samsung_plugin_type", NULL), "spf83h") == 0 || ostrcmp(getconfig("lcd_samsung_plugin_type", NULL), "spf83m") == 0)
	{
		startlcd = createpluginpath("/lcdsamsung/start.sh", 0);
		startlcd = ostrcat(startlcd, " 6", 1, 0);
	}
	else if(ostrcmp(getconfig("lcd_samsung_plugin_type", NULL), "spf87hold") == 0)
	{
		startlcd = createpluginpath("/lcdsamsung/start.sh", 0);
		startlcd = ostrcat(startlcd, " 3", 1, 0);
	}
	else if(ostrcmp(getconfig("lcd_samsung_plugin_type", NULL), "spf87h") == 0)
	{
		startlcd = createpluginpath("/lcdsamsung/start.sh", 0);
		startlcd = ostrcat(startlcd, " 2", 1, 0);
	}
	else if(ostrcmp(getconfig("lcd_samsung_plugin_type", NULL), "spf105p") == 0)
	{
		startlcd = createpluginpath("/lcdsamsung/start.sh", 0);
		startlcd = ostrcat(startlcd, " 5", 1, 0);
	}
	else if(ostrcmp(getconfig("lcd_samsung_plugin_type", NULL), "spf107h") == 0)
	{
		startlcd = createpluginpath("/lcdsamsung/start.sh", 0);
		startlcd = ostrcat(startlcd, " 4", 1, 0);
	}
	else if(ostrcmp(getconfig("lcd_samsung_plugin_type", NULL), "spf72h") == 0)
	{
		startlcd = createpluginpath("/lcdsamsung/start.sh", 0);
		startlcd = ostrcat(startlcd, " 7", 1, 0);
	}
	else if(ostrcmp(getconfig("lcd_samsung_plugin_type", NULL), "spf85h") == 0)
	{
		startlcd = createpluginpath("/lcdsamsung/start.sh", 0);
		startlcd = ostrcat(startlcd, " 8", 1, 0);
	}
	else if(ostrcmp(getconfig("lcd_samsung_plugin_type", NULL), "spf85p") == 0)
	{
		startlcd = createpluginpath("/lcdsamsung/start.sh", 0);
		startlcd = ostrcat(startlcd, " 9", 1, 0);
	}
	else if(ostrcmp(getconfig("lcd_samsung_plugin_type", NULL), "none") == 0)
	{
		startlcd = createpluginpath("/lcdsamsung/start.sh", 0);
		startlcd = ostrcat(startlcd, " 99", 1, 0);
	}
	else if(ostrcmp(getconfig("lcd_samsung_plugin_type", NULL), "d320") == 0)
	{
		startlcd = createpluginpath("/lcdsamsung/start.sh", 0);
		startlcd = ostrcat(startlcd, " 98 ", 1, 0);
		startlcd = ostrcat(startlcd, getconfig("lcd_samsung_ip", NULL), 1, 0);
	}
	else
	{
		startlcd = createpluginpath("/lcdsamsung/start.sh", 0);
		startlcd = ostrcat(startlcd, " 2", 1, 0);
	}
	printf("cmd1: %s\n", startlcd);
	
	
	if(ostrcmp(getconfig("lcd_samsung_plugin_standby", NULL), "yes") == 0) 
	{
		if(ostrcmp(getconfig("lcd_samsung_plugin_type", NULL), "spf75h") == 0)
			LCD_Standby = getscreen("LCD_spf75_Standby");
		else if(ostrcmp(getconfig("lcd_samsung_plugin_type", NULL), "spf83h") == 0 || ostrcmp(getconfig("lcd_samsung_plugin_type", NULL), "spf83h") == 0)
			LCD_Standby = getscreen("LCD_spf83_Standby");
		else if(ostrcmp(getconfig("lcd_samsung_plugin_type", NULL), "spf87h") == 0)
			LCD_Standby = getscreen("LCD_spf87_Standby");
		else if(ostrcmp(getconfig("lcd_samsung_plugin_type", NULL), "spf87hold") == 0)
			LCD_Standby = getscreen("LCD_spf87_Standby");
		else if(ostrcmp(getconfig("lcd_samsung_plugin_type", NULL), "spf105p") == 0)
			LCD_Standby = getscreen("LCD_spf105_Standby");
		else if(ostrcmp(getconfig("lcd_samsung_plugin_type", NULL), "spf107h") == 0)
			LCD_Standby = getscreen("LCD_spf107_Standby");
		else if(ostrcmp(getconfig("lcd_samsung_plugin_type", NULL), "spf72h") == 0)
			LCD_Standby = getscreen("LCD_spf72_Standby");
		else if(ostrcmp(getconfig("lcd_samsung_plugin_type", NULL), "spf85h") == 0)
			LCD_Standby = getscreen("LCD_spf85_Standby");
		else if(ostrcmp(getconfig("lcd_samsung_plugin_type", NULL), "spf85p") == 0)
			LCD_Standby = getscreen("LCD_spf85_Standby");
		else if(ostrcmp(getconfig("lcd_samsung_plugin_type", NULL), "d320") == 0)
			LCD_Standby = getscreen("LCD_spf_d320_Standby");
		else
			LCD_Standby = getscreen("LCD_spf87_Standby");
		akttime_Standby = getscreennode(LCD_Standby, "akttime"); 
		scity = getscreennode(LCD_Standby, "city");
		swind = getscreennode(LCD_Standby, "wind");
		sluft = getscreennode(LCD_Standby, "luft");
		sday0_t = getscreennode(LCD_Standby, "day0_t");
		sday0_tm = getscreennode(LCD_Standby, "day0_tm");
		sday0_i = getscreennode(LCD_Standby, "day0_i");
		sday0_d = getscreennode(LCD_Standby, "day0_d");
		sday0_dl = getscreennode(LCD_Standby, "day0_dl");
		sday1_t = getscreennode(LCD_Standby, "day1_t");
		sday1_tm = getscreennode(LCD_Standby, "day1_tm");
		sday1_i = getscreennode(LCD_Standby, "day1_i");
		sday1_d = getscreennode(LCD_Standby, "day1_d");
		sday1_dl = getscreennode(LCD_Standby, "day1_dl");
		sday2_t = getscreennode(LCD_Standby, "day2_t");
		sday2_tm = getscreennode(LCD_Standby, "day2_tm");
		sday2_i = getscreennode(LCD_Standby, "day2_i");
		sday2_d = getscreennode(LCD_Standby, "day2_d");
		sday2_dl = getscreennode(LCD_Standby, "day2_dl");
		sday3_t = getscreennode(LCD_Standby, "day3_t");
		sday3_tm = getscreennode(LCD_Standby, "day3_tm");
		sday3_i = getscreennode(LCD_Standby, "day3_i");
		sday3_d = getscreennode(LCD_Standby, "day3_d");
		sday3_dl = getscreennode(LCD_Standby, "day3_dl");
		
		n_stunde_standby =  getscreennode(LCD_Standby, "stunde");
		if(n_stunde_standby != NULL)
		{
			pichr_standby = ostrcat(n_stunde_standby->pic, "", 0, 0);
			if(ostrstr(pichr_standby, "Digital") != NULL)
				digitaluhr_standby = 1;
		}
		n_minute_standby =  getscreennode(LCD_Standby, "minute");
		if(n_minute_standby != NULL)
			picmin_standby = ostrcat(n_minute_standby->pic, "", 0, 0);
		if(digitaluhr_standby == 1)
		{
			n_stunde2_standby =  getscreennode(LCD_Standby, "stunde2");
			n_minute2_standby =  getscreennode(LCD_Standby, "minute2");
			picmin2_standby = ostrcat(n_minute2_standby->pic, "", 0, 0);
			pichr2_standby = ostrcat(n_stunde2_standby->pic, "", 0, 0);
		}
	} 
	
	
	int put = 0, typemerk = 0, type = 0;
	int standby = 0;
	
	unsigned long long int pos = 0, len = 0, reverse = 0;
	int playertype = 0;
	int loopcount = 0;
	
	int hr = 0, min = 0;
	
	if(firststart == 1)
		sleep(8);
	firststart = 0;
	status.write_png = 0;
	//if(ostrcmp(getconfig("write_fb_to_jpg", NULL), "yes") == 0)
	//	status.write_png = 1;
	
	while (LCD_Samsung1thread->aktion != STOP) {

		tmpstr = gettime(NULL, "%H:%M");
		hr = atoi(gettime(NULL, "%H"));
		min = atoi(gettime(NULL, "%M"));
		
		// TV Programm läuft
		if(status.infobaraktiv == 1)
		{
			if(ostrcmp(getconfig("write_fb_to_jpg", NULL), "yes") == 0) {
				//status.write_png = 1;
				if(type == 999)
					system("killall -3 fbread");
			}
			tmpstr2 = getaktchannelname(NULL);
			tmpstr3 = getrec(NULL, NULL);
			type = 1;
		}
		// Musik wird abgespielt
		else if((status.playspeed != 0 || status.play != 0 || status.pause != 0) && musicdat.act == 1 && LCD_Music != NULL)
		{
			if(type == 999)
				system("killall -3 fbread");
			//status.write_png = 0;
			loopcount++ ;
			type = 3;
		}
		// Aufzeichnung wird abgespielt
		else if(status.playspeed != 0 || status.play != 0 || status.pause != 0)
		{
			if(type == 999)
				system("killall -3 fbread");
			//status.write_png = 0;
			loopcount++ ;
			type = 2;
		}
		// Sonstige Anzeigen
		else if(type != 999)
		{
			if(ostrcmp(getconfig("write_fb_to_jpg", NULL), "yes") == 0) {
				//status.write_png = 1;
				system("killall -1 fbread");
			}
			type = 999;
		}
		else
			type = 999;
		
		if(typemerk != type)
		{
			put = 1;
			typemerk = type;
			free(sendermerk);sendermerk=NULL;
			free(recmerk);recmerk=NULL;
			loopcount = 0;
		}
		else
			put = 0;

		if(status.standby > 0 && standby == 0)
		{
			if(ostrcmp(getconfig("lcd_samsung_plugin_standby", NULL), "yes") == 0)
				standby = 2;
			else
			{
				tmpstr = createpluginpath("/lcdsamsung/standby.jpg", 0);
				tmpstr = ostrcat("cp ", tmpstr, 0, 1);
				tmpstr = ostrcat(tmpstr, " /tmp/titanlcd.png", 1, 0);
				system(tmpstr);
				free(tmpstr); tmpstr=NULL;
				sleep(3);
				
				tmpstr = createpluginpath("/lcdsamsung/black.jpg", 0);
				tmpstr = ostrcat("cp ", tmpstr, 0, 1);
				tmpstr = ostrcat(tmpstr, " /tmp/titanlcd.png", 1, 0);
				system(tmpstr);
				sleep(2);
				system("killall fbread");
				standby = 1;
			}
		}
		if(status.standby == 0 && standby > 0)
		{
			if(standby == 1)
				system(startlcd);
			standby = 0;
			put = 1;
		}
	
		if(weatherthread == NULL && weatherwrite == 0) 
		{ 
			if(file_exist("/tmp/lcdweather")) 
				put = 1; 
		}                        
		if(ostrcmp(tmpstr, timemerk) != 0)
		{
			free(timemerk);timemerk=NULL;
			timemerk = ostrcat(tmpstr, "", 0, 0);
			put = 1;
		} 

		if(standby == 0 || standby == 2 )
		{
			if(type == 1 && standby == 0)
			{
				if(ostrcmp(tmpstr2, sendermerk) != 0)
				{
					free(sendermerk);sendermerk=NULL;
					sendermerk = ostrcat(tmpstr2, "", 0, 0);
					put = 1;
				} 
				if(tmpstr3 == NULL && recmerk != NULL)
				{
					put = 1;
					free(recmerk);recmerk=NULL;
				}
				else if(tmpstr3 != NULL && recmerk == NULL)
				{
					free(recmerk);recmerk=NULL;
					recmerk = ostrcat(tmpstr3, "", 0, 0);
					put = 1;
				}
			}
			else if(type == 2)
			{
				if(loopcount >= 15)
				{
					put = 1;
					loopcount = 0;
				}
			}	
			else if(type == 3)
			{
				if(loopcount >= 15)
				{
					put = 1;
					loopcount = 0;
				}
			}	
			
			if(put == 1)
			{
				if(type == 1)
				{
					// Wettervorhersage
					printf("lcd_samsung wetter = %s\n", getconfig("lcd_samsung_plugin_wetter", NULL));
					if(ostrcmp(getconfig("lcd_samsung_plugin_wetter", NULL), "yes") == 0)
					{
						if(weatherwrite == 0)
						{
							printf("lcd_samsung weatherwrite=0\n");
							if(weatherthread == NULL)
							{
								printf("lcd_samsung weatherthread=NULL\n");
								if(!file_exist("/tmp/lcdweather"))
									weatherthread = addtimer(&lcd_writeweather, START, 10000, 1, NULL, NULL, NULL);
								else
								{
									fileline = malloc(256);
									if(fileline != NULL)
									{
										fd = fopen("/tmp/lcdweather", "r");
										if(fd != NULL)
										{
											if(ostrcmp(getconfig("lcd_samsung_plugin_standby", NULL), "yes") == 0) 
											{
												weather_getline(fd, fileline);weather_getline(fd, fileline);
												changetext(day0_d, fileline);
												changetext(sday0_d, fileline);
												weather_getline(fd, fileline);
												changetext(day0_tm, fileline);
												changetext(sday0_tm, fileline);
												weather_getline(fd, fileline);
												changetext(day0_t, fileline);
												changetext(sday0_t, fileline);
												weather_getline(fd, fileline);
												weather_getline(fd, fileline);
												changepic(day0_i, fileline);
												changepic(sday0_i, fileline);
											
												weather_getline(fd, fileline);
												changetext(day1_d, fileline);
												changetext(sday1_d, fileline);
												weather_getline(fd, fileline);
												changetext(day1_tm, fileline);
												changetext(sday1_tm, fileline);
												weather_getline(fd, fileline);
												changetext(day1_t, fileline);
												changetext(sday1_t, fileline);
												weather_getline(fd, fileline);
												weather_getline(fd, fileline);
												changepic(day1_i, fileline);
												changepic(sday1_i, fileline);
											
												weather_getline(fd, fileline);
												changetext(day2_d, fileline);
												changetext(sday2_d, fileline);
												weather_getline(fd, fileline);
												changetext(day2_tm, fileline);
												changetext(sday2_tm, fileline);
												weather_getline(fd, fileline);
												changetext(day2_t, fileline);
												changetext(sday2_t, fileline);
												weather_getline(fd, fileline);
												weather_getline(fd, fileline);
												changepic(day2_i, fileline);
												changepic(sday2_i, fileline);
											
												weather_getline(fd, fileline);
												changetext(day3_d, fileline);
												changetext(sday3_d, fileline);
												weather_getline(fd, fileline);
												changetext(day3_tm, fileline);
												changetext(sday3_tm, fileline);
												weather_getline(fd, fileline);
												changetext(day3_t, fileline);
												changetext(sday3_t, fileline);
												weather_getline(fd, fileline);
												weather_getline(fd, fileline);
												changepic(day3_i, fileline);
												changepic(sday3_i, fileline);
												
												weather_getline(fd, fileline);
												changepic(city, fileline);
												changepic(scity, fileline);
												weather_getline(fd, fileline);
												changetext(wind, fileline);
												changetext(swind, fileline);
												weather_getline(fd, fileline);
												changetext(luft, fileline);
												changetext(sluft, fileline);
												weather_getline(fd, fileline);
												changetext(day0_dl, fileline);
												changetext(sday0_dl, fileline);
												weather_getline(fd, fileline);
												changetext(day1_dl, fileline);
												changetext(sday1_dl, fileline);
												weather_getline(fd, fileline);
												changetext(day2_dl, fileline);
												changetext(sday2_dl, fileline);
												weather_getline(fd, fileline);
												changetext(day3_dl, fileline);
												changetext(sday3_dl, fileline);
												
											}
											else
											{
												weather_getline(fd, fileline);weather_getline(fd, fileline);
												changetext(day0_d, fileline);
												weather_getline(fd, fileline);
												changetext(day0_tm, fileline);
												weather_getline(fd, fileline);
												changetext(day0_t, fileline);
												weather_getline(fd, fileline);
												weather_getline(fd, fileline);
												changepic(day0_i, fileline);
											
												weather_getline(fd, fileline);
												changetext(day1_d, fileline);
												weather_getline(fd, fileline);
												changetext(day1_tm, fileline);
												weather_getline(fd, fileline);
												changetext(day1_t, fileline);
												weather_getline(fd, fileline);
												weather_getline(fd, fileline);
												changepic(day1_i, fileline);
											
												weather_getline(fd, fileline);
												changetext(day2_d, fileline);
												weather_getline(fd, fileline);
												changetext(day2_tm, fileline);
												weather_getline(fd, fileline);
												changetext(day2_t, fileline);
												weather_getline(fd, fileline);
												weather_getline(fd, fileline);
												changepic(day2_i, fileline);
											
												weather_getline(fd, fileline);
												changetext(day3_d, fileline);
												weather_getline(fd, fileline);
												changetext(day3_tm, fileline);
												weather_getline(fd, fileline);
												changetext(day3_t, fileline);
												weather_getline(fd, fileline);
												weather_getline(fd, fileline);
												changepic(day3_i, fileline);
												
												weather_getline(fd, fileline);
												changepic(city, fileline);
												weather_getline(fd, fileline);
												changetext(wind, fileline);
												weather_getline(fd, fileline);
												changetext(luft, fileline);
												weather_getline(fd, fileline);
												changetext(day0_dl, fileline);
												weather_getline(fd, fileline);
												changetext(day1_dl, fileline);
												weather_getline(fd, fileline);
												changetext(day2_dl, fileline);
												weather_getline(fd, fileline);
												changetext(day3_dl, fileline);
											}
											fclose(fd);
										}
										free(fileline); fileline=NULL;
									}
									weatherwrite = 1;
								}								
							}
						}		
					}
					if(standby == 0)
					{
						if(akttime != NULL)
							changetext(akttime, tmpstr);
						if(n_minute != NULL && digitaluhr == 0)
						{
							free(tmpstr);tmpstr=NULL;
							tmpstr = ostrcat("min_",gettime(NULL, "%M"), 0, 0);
							free(n_minute->pic);
							n_minute->pic = string_replace("min_mm", tmpstr, picmin, 0);
							free(tmpstr);tmpstr=NULL;
						}
						if(n_stunde != NULL && digitaluhr == 0)
						{
							free(tmpstr);tmpstr=NULL;
							if(hr >= 12)
								hr = hr - 12;
							if(hr < 10)
								tmpstr = ostrcat("hr_0",oitoa(hr), 0, 1);
							else
								tmpstr = ostrcat("hr_",oitoa(hr), 0, 1);
							if(min < 12)
								tmpstr = ostrcat(tmpstr,"00", 0, 0);
							else if(min < 24)
								tmpstr = ostrcat(tmpstr,"12", 0, 0);
							else if(min < 36)
								tmpstr = ostrcat(tmpstr,"24", 0, 0);
							else if(min < 48)
							 tmpstr = ostrcat(tmpstr,"36", 0, 0);
							else if(min < 60)
							 tmpstr = ostrcat(tmpstr,"48", 0, 0);								
							free(n_stunde->pic);
							n_stunde->pic = string_replace("hr_hhmm", tmpstr, pichr, 0);
							free(tmpstr);tmpstr=NULL;
						}
						if(digitaluhr == 1)
						{
							free(tmpstr);tmpstr=NULL;
							tmpstr = ostrcat("wert_",oitoa(hr/10), 0, 1);
							n_stunde->pic = string_replace("wert_w", tmpstr, pichr, 0);	
							free(tmpstr);tmpstr=NULL;
							tmpstr = ostrcat("wert_",oitoa(hr%10), 0, 1);
							n_stunde2->pic = string_replace("wert_w", tmpstr, pichr2, 0);	
							free(tmpstr);tmpstr=NULL;
							tmpstr = ostrcat("wert_",oitoa(min/10), 0, 1);
							n_minute->pic = string_replace("wert_w", tmpstr, picmin, 0);	
							free(tmpstr);tmpstr=NULL;
							tmpstr = ostrcat("wert_",oitoa(min%10), 0, 1);
							n_minute2->pic = string_replace("wert_w", tmpstr, picmin2, 0);
							free(tmpstr);tmpstr=NULL;
						}	
							
						m_lock(&status.drawingmutex, 0);		
						if(drawscreen(LCD_Samsung1, 0, 2) == -2)
							printf("nicht genug Speicher fuer drawscreen\n");
						m_unlock(&status.drawingmutex, 0);
					}
					else if(standby == 2)
					{
						if(akttime_Standby != NULL)
							changetext(akttime_Standby, tmpstr); 
						if(n_minute_standby != NULL && digitaluhr_standby == 0)
						{
							free(tmpstr);tmpstr=NULL;
							tmpstr = ostrcat("min_",gettime(NULL, "%M"), 0, 0);
							free(n_minute_standby->pic);
							n_minute_standby->pic = string_replace("min_mm", tmpstr, picmin_standby, 0);
							free(tmpstr);tmpstr=NULL;
						}
						if(n_stunde_standby != NULL && digitaluhr_standby == 0)
						{
							free(tmpstr);tmpstr=NULL;
							if(hr >= 12)
								hr = hr - 12;
							if(hr < 10)
								tmpstr = ostrcat("hr_0",oitoa(hr), 0, 1);
							else
								tmpstr = ostrcat("hr_",oitoa(hr), 0, 1);
							if(min < 12)
								tmpstr = ostrcat(tmpstr,"00", 0, 0);
							else if(min < 24)
								tmpstr = ostrcat(tmpstr,"12", 0, 0);
							else if(min < 36)
								tmpstr = ostrcat(tmpstr,"24", 0, 0);
							else if(min < 48)
							 tmpstr = ostrcat(tmpstr,"36", 0, 0);
							else if(min < 60)
							 tmpstr = ostrcat(tmpstr,"48", 0, 0);								
							free(n_stunde_standby->pic);
							n_stunde_standby->pic = string_replace("hr_hhmm", tmpstr, pichr_standby, 0);
							free(tmpstr);tmpstr=NULL;
						}	
						if(digitaluhr_standby == 1)
						{
							free(tmpstr);tmpstr=NULL;
							tmpstr = ostrcat("wert_",oitoa(hr/10), 0, 1);
							n_stunde_standby->pic = string_replace("wert_w", tmpstr, pichr_standby, 0);	
							free(tmpstr);tmpstr=NULL;
							tmpstr = ostrcat("wert_",oitoa(hr%10), 0, 1);
							n_stunde2_standby->pic = string_replace("wert_w", tmpstr, pichr2_standby, 0);	
							free(tmpstr);tmpstr=NULL;
							tmpstr = ostrcat("wert_",oitoa(min/10), 0, 1);
							n_minute_standby->pic = string_replace("wert_w", tmpstr, picmin_standby, 0);	
							free(tmpstr);tmpstr=NULL;
							tmpstr = ostrcat("wert_",oitoa(min%10), 0, 1);
							n_minute2_standby->pic = string_replace("wert_w", tmpstr, picmin2_standby, 0);
							free(tmpstr);tmpstr=NULL;
						}	
						m_lock(&status.drawingmutex, 0);
						drawscreen(LCD_Standby, 0, 2); 
						m_unlock(&status.drawingmutex, 0);
						put = 0;
					} 
				}
				else if(type == 2)
				{
					playertype = getconfigint("lastplayertype", NULL);
					//if(status.mcaktiv == 1)
						//playertype = 0;
					//else	
						////playertype = getconfigint("playertype", NULL);
						//playertype = 1;
		
					if(playertype == 1)
					{
						unsigned long long int startpos = 0;
						playergetinfots(&len, &startpos, NULL, &pos, NULL, 0);
						len = len / 90000;
						pos = (pos - startpos) / 90000;
					}
					else
					{
						pos = playergetpts() / 90000;
						len = playergetlength();
					}
					if(pos < 0) pos = 0;
					reverse = len - pos;
					if(len == 0)
						sprogress->progresssize = 0;
					else
						sprogress->progresssize = pos * 100 / len;
					
					tmpstr2 = convert_timesec(pos);
					changetext(spos, tmpstr2);
					free(tmpstr2); tmpstr2 = NULL;

					tmpstr2 = convert_timesec(len);
					changetext(slen, tmpstr2);
					free(tmpstr2); tmpstr2 = NULL;

					tmpstr2 = convert_timesec(reverse);
					changetext(sreverse, tmpstr2);
					free(tmpstr2); tmpstr2 = NULL;
					
					changetext(akttimeplay, tmpstr);
					changetext(stitle, basename(status.playfile));
					m_lock(&status.drawingmutex, 0);
					if(drawscreen(LCD_Play, 0, 2) == -2)
						printf("nicht genug Speicher fuer drawscreen\n");
					m_unlock(&status.drawingmutex, 0);
				}
				else if(type == 3)
				{
					pos = playergetpts() / 90000;
					len = playergetlength();
					if(pos < 0) pos = 0;
					reverse = len - pos;
					if(len == 0)
						mprogress->progresssize = 0;
					else
						mprogress->progresssize = pos * 100 / len;
					
					tmpstr2 = convert_timesec(pos);
					changetext(mpos, tmpstr2);
					free(tmpstr2); tmpstr2 = NULL;

					tmpstr2 = convert_timesec(len);
					changetext(mlen, tmpstr2);
					free(tmpstr2); tmpstr2 = NULL;

					tmpstr2 = convert_timesec(reverse);
					changetext(mreverse, tmpstr2);
					free(tmpstr2); tmpstr2 = NULL;
					
					changetext(makttimeplay, tmpstr);
					changetext(mtitle1, basename(status.playfile));
					
					changetext(mtitle, musicdat.title);
					changetext(malbum, musicdat.album);
					changetext(mrealname, musicdat.realname);
					changetext(mgenre, musicdat.genre);
					changetext(myear, musicdat.year);
					changetext(mactors, musicdat.actors);
					
					//if(file_exist(musicdat.thumb))
					if(file_exist("/tmp/mccover.jpg"))
					{
						mthumb->hidden = NO;
						changepic(mthumb, musicdat.thumb);
					}
					else
						mthumb->hidden = YES;
					
					m_lock(&status.drawingmutex, 0);
					if(drawscreen(LCD_Music, 0, 2) == -2)
						printf("nicht genug Speicher fuer drawscreen\n");
					m_unlock(&status.drawingmutex, 0);
				}
			}
		}
		else
		{
			if(standby == 2) 
			{        
				if(put == 1) 
				{        
					changetext(akttime_Standby, tmpstr);
					m_lock(&status.drawingmutex, 0);
					drawscreen(LCD_Standby, 0, 2); 
					m_unlock(&status.drawingmutex, 0);
					put = 0; 
				} 
			} 
		}

		free(tmpstr); tmpstr = NULL;
		free(tmpstr2); tmpstr2 = NULL;
		free(tmpstr3); tmpstr3 = NULL;
		//sleep(1);
		usleep(500000);
		
		if(ostrcmp(getconfig("lcd_samsung_plugin_wetter", NULL), "yes") == 0)
		{
			weatherref = weatherref + 1;
			if(weatherref == 7200)
			{
				weatherwrite = 0;
				system("rm /tmp/lcdweather");
				weatherref = 0;
			}
		}
	}
 	free(timemerk);timemerk=NULL;
 	free(sendermerk);sendermerk=NULL;
 	free(recmerk);recmerk=NULL;
 	free(startlcd);startlcd=NULL;
 	free(pichr);pichr=NULL;
 	free(picmin);picmin=NULL;
 	free(pichr_standby);pichr=NULL;
 	free(picmin_standby);picmin=NULL;
 	free(pichr2);pichr2=NULL;
 	free(picmin2);picmin2=NULL;
 	free(pichr2_standby);pichr2_standby=NULL;
 	free(picmin2_standby);picmin2_standby=NULL;
 	addconfig("lcd_samsung_plugin_running", "no");
 	LCD_Samsung1thread = NULL;
 	m_lock(&status.drawingmutex, 0);
 	if(drawscreen(LCD_Samsung1, 0, 2) == -2)
		printf("nicht genug Speicher fuer drawscreen\n");
	m_unlock(&status.drawingmutex, 0);
 	//status.write_png = 0;
 	return;
}

void LCD_Samsung1_main()
{
	if(LCD_Samsung1thread == NULL)
	{
		char* tmpstr = NULL;
		
		tmpstr = createpluginpath("/lcdsamsung/start.png", 0);
		tmpstr = ostrcat("cp ", tmpstr, 0, 1);
		tmpstr = ostrcat(tmpstr, " /tmp/titanlcd.png", 1, 0);
		system(tmpstr);
		free(tmpstr); tmpstr=NULL;
		addconfig("lcd_samsung_plugin_running", "yes");
		LCD_Samsung1thread = addtimer(&LCD_Samsung1_thread, START, 10000, 1, NULL, NULL, NULL);
		addtimer(&LCD_start_lcd4linux, START, 10000, 1, NULL, NULL, NULL);
		sleep(1);
	}
	else
	{
			LCD_Samsung1thread->aktion = STOP;
			system("echo ende > /tmp/titanlcd.jpg");
			sleep(1);
			system("killall fbread");
	}
}	
			
//wird beim laden ausgefuehrt
void init(void)
{
	char* tmpstr = NULL;
	pluginaktiv = 1;
	firststart = 1;

	tmpstr = ostrcat("/var/usr/local/share/titan/plugins/lcdsamsung/skin2.xml", NULL, 0, 0);
	if(!file_exist(tmpstr))
	{
		free(tmpstr); tmpstr = NULL;
		tmpstr = ostrcat("/var/swap/usr/local/share/titan/plugins/lcdsamsung/skin2.xml", NULL, 0, 0);
	}
	if(!file_exist(tmpstr))
	{
		free(tmpstr); tmpstr = NULL;
		tmpstr = ostrcat("/mnt/swapextensions/usr/local/share/titan/plugins/lcdsamsung/skin2.xml", NULL, 0, 0);
	}
	if(!file_exist(tmpstr))
	{
		free(tmpstr); tmpstr = NULL;
		tmpstr = createpluginpath("/lcdsamsung/skin.xml", 0);
	}

	printf("use skin: %s\n", tmpstr);

	readscreen(tmpstr, 119, 1);
	free(tmpstr); tmpstr = NULL;
	
	tmpstr = createpluginpath("/lcdsamsung/black.jpg", 0);
	tmpstr = ostrcat("cp ", tmpstr, 0, 1);
	tmpstr = ostrcat(tmpstr, " /tmp/fbreadstop.jpg", 1, 0);
		
	system(tmpstr);
	free(tmpstr); tmpstr = NULL;
	system("killall fbread");
	debug(10, "LCD Samsung SPF.. loadet !!!");
	tmpstr = getconfig("lcd_samsung_plugin_running", NULL);
	if(ostrcmp(tmpstr, "yes") == 0)
		LCD_Samsung1_main();
	tmpstr = NULL;
}

//wird beim entladen ausgefuehrt
void deinit(void)
{
	delmarkedscreen(119);
	pluginaktiv = 0;
	system("echo ende > /tmp/titanlcd.png");
	sleep(1);
	system("killall fbread");
	debug(10, "LCD Samsung unloadet !!!");
}


//wird in der Pluginverwaltung bzw Menue ausfeguehrt
void start(void)
{
	struct skin* samsung1_main = getscreen("samsung1_main");
	struct skin* listbox = getscreennode(samsung1_main, "listbox");
	struct skin* lcdtype = getscreennode(samsung1_main, "lcdtype");
	struct skin* lcdip = getscreennode(samsung1_main, "lcdip");
	struct skin* allmenu = getscreennode(samsung1_main, "allmenu");
	struct skin* aktstandby = getscreennode(samsung1_main, "aktstandby");
	struct skin* wettervor = getscreennode(samsung1_main, "wettervor");
	struct skin* wettervorort = getscreennode(samsung1_main, "wettervorort"); 
	//struct skin* wettervorplz = getscreennode(samsung1_main, "wettervorplz");
	//struct skin* wettervorland = getscreennode(samsung1_main, "wettervorland");
	struct skin* b3 = getscreennode(samsung1_main, "b3");
	struct skin* tmp = NULL;
	
	int rcret = 0;
	int startstop = 0;
	int restart = 0;
	
  if(getconfig("lcd_samsung_plugin_wetter", NULL) == NULL || ostrcmp(getconfig("lcd_samsung_plugin_wetter", NULL), "no")  == 0)
  {
  	//wettervorplz->hidden = YES;
  	//wettervorland->hidden = YES;
  	wettervorort->hidden = YES;
  }
  changeinput(lcdip, getconfig("lcd_samsung_ip", NULL));
  if(getconfig("lcd_samsung_plugin_type", NULL) == NULL || ostrcmp(getconfig("lcd_samsung_plugin_type", NULL), "d320")  != 0)
  {
  	lcdip->hidden = YES;
  }
	if(ostrcmp(getconfig("lcd_samsung_plugin_type", NULL), "d320") == 0)
		addconfig("lcd_samsung_plugin_type", "spf72h");        
  addchoicebox(lcdtype, "spf72h", _("SPF-72H"));
  addchoicebox(lcdtype, "spf75h", _("SPF-75H"));
  addchoicebox(lcdtype, "spf83h", _("SPF-83H"));
  addchoicebox(lcdtype, "spf83m", _("SPF-83M"));
  addchoicebox(lcdtype, "spf85h", _("SPF-85H"));
  addchoicebox(lcdtype, "spf85p", _("SPF-85P"));
  addchoicebox(lcdtype, "spf87h", _("SPF-87H"));
  addchoicebox(lcdtype, "spf87hold", _("SPF-87H-old"));
  addchoicebox(lcdtype, "spf105p", _("SPF-105P"));
  addchoicebox(lcdtype, "spf107h", _("SPF-107H"));
  addchoicebox(lcdtype, "none", _("tiMote"));
  //addchoicebox(lcdtype, "d320", _("Dummy-320"));
	setchoiceboxselection(lcdtype, getconfig("lcd_samsung_plugin_type", NULL));
	
	addchoicebox(allmenu, "no", _("nein"));
	addchoicebox(allmenu, "yes", _("ja"));
	setchoiceboxselection(allmenu, getconfig("write_fb_to_jpg", NULL));
	
	addchoicebox(aktstandby, "no", _("nein")); 
	addchoicebox(aktstandby, "yes", _("ja")); 
	setchoiceboxselection(aktstandby, getconfig("lcd_samsung_plugin_standby", NULL)); 
	
	addchoicebox(wettervor, "no", _("nein"));
  addchoicebox(wettervor, "yes", _("ja"));
	setchoiceboxselection(wettervor, getconfig("lcd_samsung_plugin_wetter", NULL));
	
	//changemask(wettervorplz, "0000");
	//if(getconfig("lcd_samsung_plugin_wetterplz", NULL) == NULL)
	//	changeinput(wettervorplz, "10407");
	//else
	//	changeinput(wettervorplz, getconfig("lcd_samsung_plugin_wetterplz", NULL));
		
	//changemask(wettervorland, "abcdefghijklmnopqrstuvwxyz");
	//if(getconfig("lcd_samsung_plugin_wetterland", NULL) == NULL)
	//	changeinput(wettervorland, "Germany");
	//else
	//	changeinput(wettervorland, getconfig("lcd_samsung_plugin_wetterland", NULL));
	
	changemask(wettervorort, "abcdefghijklmnopqrstuvwxyz"); 
	if(getconfig("lcd_samsung_plugin_wetterort", NULL) == NULL) 
		changeinput(wettervorort, "Berlin        "); 
	else
		changeinput(wettervorort, getconfig("lcd_samsung_plugin_wetterort", NULL)); 	
	
	if(LCD_Samsung1thread != NULL)
		changetext(b3, "STOP");
	else
		changetext(b3, "START");
		
	drawscreen(samsung1_main, 0, 0);
	addscreenrc(samsung1_main, listbox);
	tmp = listbox->select;
		
	while(1)
	{
		addscreenrc(samsung1_main, tmp);
		rcret = waitrc(samsung1_main, 0, 0);
		tmp = listbox->select;
		
		if((rcret == getrcconfigint("rcleft", NULL) || rcret == getrcconfigint("rcright", NULL)) && listbox->select != NULL && ostrcmp(listbox->select->name, "wettervor") == 0)
		{
			if(ostrcmp(wettervor->ret, "yes") == 0)
			{
				//wettervorplz->hidden = NO;
  			//wettervorland->hidden = NO;
  			wettervorort->hidden = NO;
			} else {
				//wettervorplz->hidden = YES;
  			//wettervorland->hidden = YES;
  			wettervorort->hidden = YES;
  		}
			
			drawscreen(samsung1_main, 0, 0);
		}
		if((rcret == getrcconfigint("rcleft", NULL) || rcret == getrcconfigint("rcright", NULL)) && listbox->select != NULL && ostrcmp(listbox->select->name, "lcdtype") == 0)
		{
			if(ostrcmp(lcdtype->ret, "d320") == 0)
			{
  			lcdip->hidden = NO;
			} else {
  			lcdip->hidden = YES;
  		}
			
			drawscreen(samsung1_main, 0, 0);
		}			
		if(rcret == getrcconfigint("rcexit", NULL))
			break;
		if(rcret == getrcconfigint("rcgreen", NULL))
		{
			addconfig("lcd_samsung_plugin_type", lcdtype->ret);
			addconfig("write_fb_to_jpg", allmenu->ret);
			addconfig("lcd_samsung_plugin_standby", aktstandby->ret);
			addconfig("lcd_samsung_plugin_wetter", wettervor->ret);
			addconfig("lcd_samsung_plugin_wetterort", wettervorort->ret);
			addconfig("lcd_samsung_ip", lcdip->ret);
			//addconfig("lcd_samsung_plugin_wetterplz", wettervorplz->ret);
			//addconfig("lcd_samsung_plugin_wetterland", wettervorland->ret);
			restart = 1;
			break;
		}
			
		if(rcret == getrcconfigint("rcblue", NULL))
		{
			addconfig("lcd_samsung_plugin_type", lcdtype->ret);
			addconfig("write_fb_to_jpg", allmenu->ret);
			addconfig("lcd_samsung_plugin_standby", aktstandby->ret);
			addconfig("lcd_samsung_plugin_wetter", wettervor->ret);
			addconfig("lcd_samsung_plugin_wetterort", wettervorort->ret);
			addconfig("lcd_samsung_ip", lcdip->ret);
			//addconfig("lcd_samsung_plugin_wetterplz", wettervorplz->ret);
			//addconfig("lcd_samsung_plugin_wetterland", wettervorland->ret);
			startstop = 1;
			break;
		}
	}
	delownerrc(samsung1_main);
	clearscreen(samsung1_main);
	
	if(LCD_Samsung1thread != NULL && restart == 1)
	{
		LCD_Samsung1thread->aktion = STOP;
		system("killall fbread");	
		sleep(2);
		addtimer(&LCD_start_lcd4linux, START, 10000, 1, NULL, NULL, NULL);
		LCD_Samsung1thread = addtimer(&LCD_Samsung1_thread, START, 10000, 1, NULL, NULL, NULL);
		sleep(1);
		return;
	}
	if(startstop == 1)
	{
		if(LCD_Samsung1thread != NULL)
		{
			addconfig("lcd_samsung_plugin_running", "no");
			LCD_Samsung1thread->aktion = STOP;
			sleep(1);
			system("killall fbread");
			debug(10, "LCD Samsung unloadet !!!");	
			sleep(1);
		}
		else {
			addconfig("lcd_samsung_plugin_running", "yes");
			firststart = 1;
			LCD_Samsung1_main();
		}
	}
}
