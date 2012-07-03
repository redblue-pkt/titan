#include "../../titan/struct.h"
#include "../../titan/debug.h"
#include "../../titan/header.h"

#include "lcd_weather.h"

char pluginname[] = "LCD Samsung SPF..";
char plugindesc[] = "Extensions";
char pluginpic[] = "%pluginpath%/lcdsamsung/lcdsamsung.png";

int pluginaktiv = 0;


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
		startlcd = ostrcat(getconfig("pluginpath", NULL), "/lcdsamsung/start.sh 1", 0, 0);
	else if(ostrcmp(getconfig("lcd_samsung_plugin_type", NULL), "spf87h") == 0)
		startlcd = ostrcat(getconfig("pluginpath", NULL), "/lcdsamsung/start.sh 2", 0, 0);
	else
		startlcd = ostrcat(getconfig("pluginpath", NULL), "/lcdsamsung/start.sh 2", 0, 0);
	
	
	if(LCD_Samsung1thread == NULL)
		return;
	while (LCD_Samsung1thread->aktion != STOP && system("ps | grep -v grep | grep fbread") != 0) {
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
	struct skin* LCD_Play = NULL;
	
	struct skin* day0_t = NULL;
	struct skin* day0_i = NULL;
	struct skin* day1_t = NULL;
	struct skin* day1_i = NULL;
	struct skin* day2_t = NULL;
	struct skin* day2_i = NULL;
	struct skin* day3_t = NULL;
	struct skin* day3_i = NULL;
	struct skin* akttime = NULL;
	
	
	char* tmpstr = NULL, *tmpstr2 = NULL, *tmpstr3 = NULL, *timemerk = NULL, *sendermerk = NULL, *recmerk = NULL;
	FILE *fd = NULL;
	char *fileline = NULL;
	int weatherwrite = 0;
	char* startlcd = NULL;
	
	if(ostrcmp(getconfig("lcd_samsung_plugin_wetter", NULL), "yes") == 0)
	{
		if(ostrcmp(getconfig("lcd_samsung_plugin_type", NULL), "spf75h") == 0)
			LCD_Samsung1 = getscreen("LCD_spf75_Wetter");
		else if(ostrcmp(getconfig("lcd_samsung_plugin_type", NULL), "spf87h") == 0)
			LCD_Samsung1 = getscreen("LCD_spf87_Wetter");
		else
			LCD_Samsung1 = getscreen("LCD_spf87_Wetter");
		day0_t = getscreennode(LCD_Samsung1, "day0_t");
		day0_i = getscreennode(LCD_Samsung1, "day0_i");
		day1_t = getscreennode(LCD_Samsung1, "day1_t");
		day1_i = getscreennode(LCD_Samsung1, "day1_i");
		day2_t = getscreennode(LCD_Samsung1, "day2_t");
		day2_i = getscreennode(LCD_Samsung1, "day2_i");
		day3_t = getscreennode(LCD_Samsung1, "day3_t");
		day3_i = getscreennode(LCD_Samsung1, "day3_i");
		if(file_exist("/tmp/lcdweather") == 1)
			system("rm /tmp/lcdweather");
	}
	else {
		if(ostrcmp(getconfig("lcd_samsung_plugin_type", NULL), "spf75h") == 0)
			LCD_Samsung1 = getscreen("LCD_spf75");
		else if(ostrcmp(getconfig("lcd_samsung_plugin_type", NULL), "spf87h") == 0)
			LCD_Samsung1 = getscreen("LCD_spf87");
		else
			LCD_Samsung1 = getscreen("LCD_spf87");	
		akttime = getscreennode(LCD_Samsung1, "akttime");
	}
	
	if(ostrcmp(getconfig("lcd_samsung_plugin_type", NULL), "spf75h") == 0)
		LCD_Play = getscreen("LCD_spf75_Play");
	else if(ostrcmp(getconfig("lcd_samsung_plugin_type", NULL), "spf87h") == 0)
		LCD_Play = getscreen("LCD_spf87_Play");
	else
		LCD_Play = getscreen("LCD_spf87_Play");
	struct skin* akttimeplay = getscreennode(LCD_Play, "akttime");
	struct skin* sprogress = getscreennode(LCD_Play, "progress");
	struct skin* stitle = getscreennode(LCD_Play, "title1");
	struct skin* spos = getscreennode(LCD_Play, "pos");
	struct skin* slen = getscreennode(LCD_Play, "len");
	struct skin* sreverse = getscreennode(LCD_Play, "reverse");
		
	if(ostrcmp(getconfig("lcd_samsung_plugin_type", NULL), "spf75h") == 0)
		startlcd = ostrcat(getconfig("pluginpath", NULL), "/lcdsamsung/start.sh 1", 0, 0);
	else if(ostrcmp(getconfig("lcd_samsung_plugin_type", NULL), "spf87h") == 0)
		startlcd = ostrcat(getconfig("pluginpath", NULL), "/lcdsamsung/start.sh 2", 0, 0);
	else
		startlcd = ostrcat(getconfig("pluginpath", NULL), "/lcdsamsung/start.sh 2", 0, 0);
	
	int put = 0, typemerk = 0, type = 0;
	int standby = 0;
	
	unsigned long long int pos = 0, len = 0, reverse = 0;
	int playertype = 0;
	int loopcount = 0;
	
	if(firststart == 1)
		sleep(8);
	firststart = 0;
	status.write_png = 0;
	//if(ostrcmp(getconfig("write_fb_to_jpg", NULL), "yes") == 0)
	//	status.write_png = 1;
	
	while (LCD_Samsung1thread->aktion != STOP) {

		tmpstr = gettime(NULL, "%H:%M");
		
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

		if(status.security == 1)
		{
			if(status.standby == 1 && standby == 0)
			{
				system("killall lcd4linux");
				standby = 1;
			}
			if(status.standby == 0 && standby == 1)
			{
				system(startlcd);
				standby = 0;
			}
		
			if(standby == 0)
			{
				if(ostrcmp(tmpstr, timemerk) != 0)
				{
					free(timemerk);timemerk=NULL;
					timemerk = ostrcat(tmpstr, "", 0, 0);
					put = 1;
				} 
				if(type == 1)
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
				
				if(put == 1)
				{
					if(type == 1)
					{
						// Wettervorhersage
						if(ostrcmp(getconfig("lcd_samsung_plugin_wetter", NULL), "yes") == 0)
						{
							if(weatherwrite == 0)
							{
								if(weatherthread == NULL)
								{
									if(file_exist("/tmp/lcdweather") == 0)
										weatherthread = addtimer(&lcd_writeweather, START, 10000, 1, NULL, NULL, NULL);
									else
									{
										fileline = malloc(256);
										if(fileline != NULL)
										{
											fd = fopen("/tmp/lcdweather", "r");
											if(fd != NULL)
											{
												weather_getline(fd, fileline);weather_getline(fd, fileline);weather_getline(fd, fileline);
												weather_getline(fd, fileline);
												changetext(day0_t, fileline);
												weather_getline(fd, fileline);
												weather_getline(fd, fileline);
												changepic(day0_i, fileline);
												
												weather_getline(fd, fileline);weather_getline(fd, fileline);
												weather_getline(fd, fileline);
												changetext(day1_t, fileline);
												weather_getline(fd, fileline);
												weather_getline(fd, fileline);
												changepic(day1_i, fileline);
												
												weather_getline(fd, fileline);weather_getline(fd, fileline);
												weather_getline(fd, fileline);
												changetext(day2_t, fileline);
												weather_getline(fd, fileline);
												weather_getline(fd, fileline);
												changepic(day2_i, fileline);
												
												weather_getline(fd, fileline);weather_getline(fd, fileline);
												weather_getline(fd, fileline);
												changetext(day3_t, fileline);
												weather_getline(fd, fileline);
												weather_getline(fd, fileline);
												changepic(day3_i, fileline);
												fclose(fd);
											}
											free(fileline); fileline=NULL;
										}
										weatherwrite = 1;
									}								
								}
							}		
						}
						
						changetext(akttime, tmpstr);
						if(drawscreen(LCD_Samsung1, 0, 0) == -2)
							printf("nicht genug Speicher fuer drawscreen\n");
							
					}
					else if(type == 2)
					{
						if(status.mcaktiv == 1)
							playertype = 0;
						else	
							playertype = getconfigint("playertype", NULL);
			
						if(playertype == 1)
						{
							unsigned long long int startpos = 0;
							playergetinfots(&len, &startpos, NULL, &pos, NULL);
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
						if(drawscreen(LCD_Samsung1, 0, 0) == -2)
							printf("nicht genug Speicher fuer drawscreen\n");
					}
				}
			}
		}
		free(tmpstr); tmpstr = NULL;
		free(tmpstr2); tmpstr2 = NULL;
		free(tmpstr3); tmpstr3 = NULL;
		//sleep(1);
		usleep(500000);
	}
 	free(timemerk);timemerk=NULL;
 	free(sendermerk);sendermerk=NULL;
 	free(recmerk);recmerk=NULL;
 	free(startlcd);startlcd=NULL;
 	addconfig("lcd_samsung_plugin_running", "no");
 	LCD_Samsung1thread = NULL;
 	if(drawscreen(LCD_Samsung1, 0, 0) == -2)
		printf("nicht genug Speicher fuer drawscreen\n");
 	//status.write_png = 0;
 	return;
}

void LCD_Samsung1_main()
{
	if(LCD_Samsung1thread == NULL)
	{
		char* tmpstr = NULL;
		tmpstr = ostrcat("cp ", getconfig("pluginpath", NULL), 0, 0);
		tmpstr = ostrcat(tmpstr, "/lcdsamsung/start.jpg", 1, 0);
		tmpstr = ostrcat(tmpstr, " /tmp/titanlcd.jpg", 1, 0);
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
	
	tmpstr = ostrcat(getconfig("pluginpath", NULL), "/lcdsamsung/skin.xml", 0, 0);
	readscreen(tmpstr, 119, 1);
	free(tmpstr); tmpstr = NULL;
	debug(10, "LCD Samsung SPF.. loadet !!!");
	tmpstr = getconfig("lcd_samsung_plugin_running", NULL);
	if(ostrcmp(tmpstr, "yes") == 0)
		LCD_Samsung1_main();
	tmpstr=NULL;
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
	struct skin* allmenu = getscreennode(samsung1_main, "allmenu");
	struct skin* wettervor = getscreennode(samsung1_main, "wettervor");
	struct skin* wettervorplz = getscreennode(samsung1_main, "wettervorplz");
	struct skin* wettervorland = getscreennode(samsung1_main, "wettervorland");
	struct skin* b3 = getscreennode(samsung1_main, "b3");
	struct skin* tmp = NULL;
	
	int rcret = 0;
	int startstop = 0;
	int restart = 0;
	
  if(getconfig("lcd_samsung_plugin_wetter", NULL) == NULL || ostrcmp(getconfig("lcd_samsung_plugin_wetter", NULL), "no")  == 0)
  {
  	wettervorplz->hidden = YES;
  	wettervorland->hidden = YES;
  }
    
  addchoicebox(lcdtype, "spf75h", _("SPF-75H"));
  addchoicebox(lcdtype, "spf87h", _("SPF-87H"));
	setchoiceboxselection(lcdtype, getconfig("lcd_samsung_plugin_type", NULL));
	
	addchoicebox(allmenu, "no", _("nein"));
	addchoicebox(allmenu, "yes", _("ja"));
	setchoiceboxselection(allmenu, getconfig("write_fb_to_jpg", NULL));
	
	addchoicebox(wettervor, "no", _("nein"));
  addchoicebox(wettervor, "yes", _("ja"));
	setchoiceboxselection(wettervor, getconfig("lcd_samsung_plugin_wetter", NULL));
	
	changemask(wettervorplz, "0000");
	if(getconfig("lcd_samsung_plugin_wetterplz", NULL) == NULL)
		changeinput(wettervorplz, "10407");
	else
		changeinput(wettervorplz, getconfig("lcd_samsung_plugin_wetterplz", NULL));
		
	changemask(wettervorland, "abcdefghijklmnopqrstuvwxyz");
	if(getconfig("lcd_samsung_plugin_wetterland", NULL) == NULL)
		changeinput(wettervorland, "Germany");
	else
		changeinput(wettervorland, getconfig("lcd_samsung_plugin_wetterland", NULL));
		
	
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
				wettervorplz->hidden = NO;
  			wettervorland->hidden = NO;
			} else {
				wettervorplz->hidden = YES;
  			wettervorland->hidden = YES;
  		}
			
			drawscreen(samsung1_main, 0, 0);
		}
					
		if(rcret == getrcconfigint("rcexit", NULL))
			break;
		if(rcret == getrcconfigint("rcgreen", NULL))
		{
			addconfig("lcd_samsung_plugin_type", lcdtype->ret);
			addconfig("write_fb_to_jpg", allmenu->ret);
			addconfig("lcd_samsung_plugin_wetter", wettervor->ret);
			addconfig("lcd_samsung_plugin_wetterplz", wettervorplz->ret);
			addconfig("lcd_samsung_plugin_wetterland", wettervorland->ret);
			restart = 1;
			break;
		}
			
		if(rcret == getrcconfigint("rcblue", NULL))
		{
			addconfig("lcd_samsung_plugin_type", lcdtype->ret);
			addconfig("write_fb_to_jpg", allmenu->ret);
			addconfig("lcd_samsung_plugin_wetter", wettervor->ret);
			addconfig("lcd_samsung_plugin_wetterplz", wettervorplz->ret);
			addconfig("lcd_samsung_plugin_wetterland", wettervorland->ret);
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
