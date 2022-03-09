#include "../../titan/struct.h"
#include "../../titan/debug.h"
#include "../../titan/header.h"

#include "lcd_weather.h"

char pluginname[] = "LCD Pearl";
char plugindesc[] = "Extensions";
char pluginpic[] = "%pluginpath%/lcdpearl1/lcdpearl.png";

int pluginaktiv = 0;
int pluginversion = PLUGINVERSION;
//struct skin* pluginmenu = NULL;
//int pluginflag = 1; //don't show the plugin in pluginmanager

struct stimerthread* LCD_Pearl1thread = NULL;

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
	char* startlcd = createpluginpath("/lcdpearl1/start.sh", 0);

	if(LCD_Pearl1thread == NULL)
		return;
#ifndef MIPSEL
	while (LCD_Pearl1thread->aktion != STOP && system("ps | grep -v grep | grep lcd4linux ") != 0) {
#else
	while (LCD_Pearl1thread->aktion != STOP && system("ps -A | grep -v grep | grep lcd4linux ") != 0) {
#endif
		system(startlcd);
		sleep(6);
		count ++;
		if(LCD_Pearl1thread == NULL)
			break;
		if(count == 101) {
			textbox(_("Message"), _("ERROR cant start LCD driver"), _("OK"), getrcconfigint("rcok", NULL), NULL, 0, NULL, 0, NULL, 0, 600, 200, 5, 0);
			LCD_Pearl1thread->aktion = STOP;
		}
	}
	free(startlcd); startlcd=NULL;	
}

void LCD_Pearl1_thread()
{
	
	struct skin* LCD_Pearl1 = NULL;
	struct skin* LCD_Pearl1_Standby = NULL;
	struct skin* day0_t = NULL;
	struct skin* day0_i = NULL;
	struct skin* day1_t = NULL;
	struct skin* day1_i = NULL;
	struct skin* day2_t = NULL;
	struct skin* day2_i = NULL;
	struct skin* day3_t = NULL;
	struct skin* day3_i = NULL;
	struct skin* akttime = NULL;
	struct skin* akttime_Standby = NULL;
	
	
	char* tmpstr = NULL, *tmpstr2 = NULL, *tmpstr3 = NULL, *timemerk = NULL, *sendermerk = NULL, *recmerk = NULL;
	FILE *fd = NULL;
	char *fileline = NULL;
	int weatherwrite = 999;
	int weatherref = 0;
	
	if(ostrcmp(getconfig("lcd_pearl1_plugin_wetter", NULL), "yes") == 0)
	{
		LCD_Pearl1 = getscreen("LCD_Pearl1_Wetter");
		day0_t = getscreennode(LCD_Pearl1, "day0_t");
		day0_i = getscreennode(LCD_Pearl1, "day0_i");
		day1_t = getscreennode(LCD_Pearl1, "day1_t");
		day1_i = getscreennode(LCD_Pearl1, "day1_i");
		day2_t = getscreennode(LCD_Pearl1, "day2_t");
		day2_i = getscreennode(LCD_Pearl1, "day2_i");
		day3_t = getscreennode(LCD_Pearl1, "day3_t");
		day3_i = getscreennode(LCD_Pearl1, "day3_i");
		if(file_exist("/tmp/lcdweather"))
			system("rm /tmp/lcdweather");
		weatherwrite = 0;
	}
	else
		LCD_Pearl1 = getscreen("LCD_Pearl1");	
	akttime = getscreennode(LCD_Pearl1, "akttime");
	
	struct skin* LCD_Play = getscreen("LCD_Play");
	struct skin* akttimeplay = getscreennode(LCD_Play, "akttime");
	struct skin* sprogress = getscreennode(LCD_Play, "progress");
	struct skin* stitle = getscreennode(LCD_Play, "title1");
	struct skin* spos = getscreennode(LCD_Play, "pos");
	struct skin* slen = getscreennode(LCD_Play, "len");
	struct skin* sreverse = getscreennode(LCD_Play, "reverse");
	
	//struct skin* LCD_MC_Menu = getscreen("LCD_MC_Menu");
	//struct skin* akttimemc1 = getscreennode(LCD_MC_Menu, "akttime");
	
			
	if(ostrcmp(getconfig("lcd_pearl1_plugin_standby", NULL), "yes") == 0)
	{
		LCD_Pearl1_Standby = getscreen("LCD_Pearl1_Standby");
		akttime_Standby = getscreennode(LCD_Pearl1_Standby, "akttime");
	}
		
	int put = 0, typemerk = 0, type = 0;
	int standby = 0;
	
	char* fbgrab = createpluginpath("/lcdpearl1/fbgrab", 0);
	fbgrab = ostrcat(fbgrab, " -f /tmp/titanlcd.raw -w 320 -h 240 -b 32 -i /tmp/.titanlcd1.png > /dev/null", 1, 0);
	
	char* startlcd = createpluginpath("/lcdpearl1/start.sh", 0);

	unsigned long long int pos = 0, len = 0, reverse = 0;
	int playertype = 0;
	int loopcount = 0;
	
	if(firststart == 1)
		sleep(8);
	firststart = 0;
	status.write_png = 0;
	if(ostrcmp(getconfig("write_fb_to_png", NULL), "yes") == 0)
		status.write_png = 1;
	
	while (LCD_Pearl1thread->aktion != STOP) {

		tmpstr = gettime(NULL, "%H:%M");
		
		// TV Programm l�uft
		if(status.infobaraktiv == 1)
		{
			if(ostrcmp(getconfig("write_fb_to_png", NULL), "yes") == 0)
				status.write_png = 1;
			tmpstr2 = getaktchannelname(NULL);
			tmpstr3 = getrec(NULL, NULL);
			type = 1;
		}
		// Aufzeichnung wird abgespielt
		else if(status.playspeed != 0 || status.play != 0 || status.pause != 0)
		{
			status.write_png = 0;
			loopcount++ ;
			type = 2;
		}
		// Sonstige Anzeigen
		else
		{
			if(ostrcmp(getconfig("write_fb_to_png", NULL), "yes") == 0)
				status.write_png = 1;
			type = 999;
		}
		
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
			if(ostrcmp(getconfig("lcd_pearl1_plugin_standby", NULL), "yes") == 0)
			{
				standby = 2;
				put = 1;
			}
			else {
				system("killall lcd4linux");
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

		if(standby == 0)
		{
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
					if(ostrcmp(getconfig("lcd_pearl1_plugin_wetter", NULL), "yes") == 0)
					{
						if(weatherwrite == 0)
						{
							if(weatherthread == NULL)
							{
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
					drawscreen(LCD_Pearl1, 0, 0);

					//system(fbgrab);
					//system("mv /tmp/.titanlcd1.png /tmp/titanlcd.png");
			
					//system("/var/bin/fbgrab -f /tmp/titanlcd.raw -w 320 -h 240 -b 32 -i /tmp/titanlcd.png > /dev/null");
					//system("xloadimage /tmp/titanlcd.png > /dev/null &");
					
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
					drawscreen(LCD_Play, 0, 0);

				//else if(type == 999 && status.mcaktiv == 1) 
				//else if(type == 999)	
				//{
					//changetext(akttimemc1, tmpstr);
					//drawscreen(LCD_MC_Menu, 0, 3);
				//}
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
					drawscreen(LCD_Pearl1_Standby, 0, 0);
					put = 0;
				}
			}
		}

		free(tmpstr); tmpstr = NULL;
		free(tmpstr2); tmpstr2 = NULL;
		free(tmpstr3); tmpstr3 = NULL;

		do
		{
			//sleep(1);
			usleep(500000);
		}while ( status.aktservice->type == TMPOTHER ); //virtual zap
		
		if(ostrcmp(getconfig("lcd_pearl1_plugin_wetter", NULL), "yes") == 0)
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
 	free(fbgrab);fbgrab=NULL;
 	free(startlcd);startlcd=NULL;
 	addconfig("lcd_pearl1_plugin_running", "no");
 	LCD_Pearl1thread = NULL;
 	drawscreen(LCD_Pearl1, 0, 0);
 	status.write_png = 0;
 	return;
}

void LCD_Pearl1_main()
{
	if(LCD_Pearl1thread == NULL)
	{
		char* tmpstr = NULL;
		
		tmpstr = createpluginpath("/lcdpearl1/start.png", 0);
		tmpstr = ostrcat("cp ", tmpstr, 0, 1);
		tmpstr = ostrcat(tmpstr, " /tmp/titanlcd.png", 1, 0);
		
		system(tmpstr);
		free(tmpstr); tmpstr=NULL;
		//textbox(_("Message"), _("LCD Pearl1 starts ..."), _("OK"), getrcconfigint("rcok", NULL), NULL, 0, NULL, 0, NULL, 0, 600, 200, 5, 0);
		addconfig("lcd_pearl1_plugin_running", "yes");
		LCD_Pearl1thread = addtimer(&LCD_Pearl1_thread, START, 10000, 1, NULL, NULL, NULL);
		addtimer(&LCD_start_lcd4linux, START, 10000, 1, NULL, NULL, NULL);
		sleep(1);
	}
	else
	{
		//if (textbox(_("Message"), _("Stop LCD Pearl1 ?"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("exit", NULL), NULL, 0, NULL, 0, 600, 200, 5, 0) != 2);
		//{
			LCD_Pearl1thread->aktion = STOP;
			system("echo ende > /tmp/titanlcd.png");
			sleep(1);
			system("killall lcd4linux");
		//}
	}
}	
			
//wird beim laden ausgefuehrt
void init(void)
{
	char* tmpstr = NULL;
	pluginaktiv = 1;
	firststart = 1;

	tmpstr = ostrcat("/var/usr/local/share/titan/plugins/lcdpearl1/skin2.xml", NULL, 0, 0);
	if(!file_exist(tmpstr))
	{
		free(tmpstr); tmpstr = NULL;
		tmpstr = ostrcat("/var/swap/usr/local/share/titan/plugins/lcdpearl1/skin2.xml", NULL, 0, 0);
	}
	if(!file_exist(tmpstr))
	{
		free(tmpstr); tmpstr = NULL;
		tmpstr = ostrcat("/mnt/swapextensions/usr/local/share/titan/plugins/lcdpearl1/skin2.xml", NULL, 0, 0);
	}
	if(!file_exist(tmpstr))
	{
		free(tmpstr); tmpstr = NULL;
		tmpstr = createpluginpath("/lcdpearl1/skin.xml", 0);
	}

	printf("use skin: %s\n", tmpstr);

	readscreen(tmpstr, 116, 1);
	free(tmpstr); tmpstr = NULL;
	debug(10, "LCD Pearl loadet !!!");
	tmpstr = getconfig("lcd_pearl1_plugin_running", NULL);
	if(ostrcmp(tmpstr, "yes") == 0)
		LCD_Pearl1_main();
	tmpstr=NULL;
}

//wird beim entladen ausgefuehrt
void deinit(void)
{
	delmarkedscreen(116);
	pluginaktiv = 0;
	system("echo ende > /tmp/titanlcd.png");
	sleep(1);
	system("killall lcd4linux");
	debug(10, "LCD Pearl1 unloadet !!!");
}


//wird in der Pluginverwaltung bzw Menue ausfeguehrt
void start(void)
{
	struct skin* pearl1_main = getscreen("pearl1_main");
	struct skin* listbox = getscreennode(pearl1_main, "listbox");
	struct skin* allmenu = getscreennode(pearl1_main, "allmenu");
	struct skin* aktstandby = getscreennode(pearl1_main, "aktstandby");
	struct skin* wettervor = getscreennode(pearl1_main, "wettervor");
	struct skin* wettervorort = getscreennode(pearl1_main, "wettervorort");
	//struct skin* wettervorplz = getscreennode(pearl1_main, "wettervorplz");
	//struct skin* wettervorland = getscreennode(pearl1_main, "wettervorland");
	struct skin* b3 = getscreennode(pearl1_main, "b3");
	struct skin* tmp = NULL;
	
	int rcret = 0;
	int startstop = 0;
	int restart = 0;
	
  if(getconfig("lcd_pearl1_plugin_wetter", NULL) == NULL || ostrcmp(getconfig("lcd_pearl1_plugin_wetter", NULL), "no")  == 0)
  {
  	wettervorort->hidden = YES;
  	//wettervorplz->hidden = YES;
  	//wettervorland->hidden = YES;
  }
    
  addchoicebox(allmenu, "no", _("nein"));
  addchoicebox(allmenu, "yes", _("ja"));
	setchoiceboxselection(allmenu, getconfig("write_fb_to_png", NULL));
	
	addchoicebox(aktstandby, "no", _("nein"));
  addchoicebox(aktstandby, "yes", _("ja"));
	setchoiceboxselection(aktstandby, getconfig("lcd_pearl1_plugin_standby", NULL));
	
	addchoicebox(wettervor, "no", _("nein"));
  addchoicebox(wettervor, "yes", _("ja"));
	setchoiceboxselection(wettervor, getconfig("lcd_pearl1_plugin_wetter", NULL));
	
	//changemask(wettervorplz, "0000");
	//if(getconfig("lcd_pearl1_plugin_wetterplz", NULL) == NULL)
	//	changeinput(wettervorplz, "10407");
	//else
	//	changeinput(wettervorplz, getconfig("lcd_pearl1_plugin_wetterplz", NULL));
		
	//changemask(wettervorland, "abcdefghijklmnopqrstuvwxyz");
	//if(getconfig("lcd_pearl1_plugin_wetterland", NULL) == NULL)
	//	changeinput(wettervorland, "Germany");
	//else
	//	changeinput(wettervorland, getconfig("lcd_pearl1_plugin_wetterland", NULL));
		
	changemask(wettervorort, "abcdefghijklmnopqrstuvwxyz");
	if(getconfig("lcd_pearl1_plugin_wetterort", NULL) == NULL)
		changeinput(wettervorort, "Berlin        ");
	else
		changeinput(wettervorort, getconfig("lcd_pearl1_plugin_wetterort", NULL));
		
	
	if(LCD_Pearl1thread != NULL)
		changetext(b3, "STOP");
	else
		changetext(b3, "START");
		
	drawscreen(pearl1_main, 0, 0);
	addscreenrc(pearl1_main, listbox);
	tmp = listbox->select;
	
	while(1)
	{
		addscreenrc(pearl1_main, tmp);
		rcret = waitrc(pearl1_main, 0, 0);
		tmp = listbox->select;
		
		if((rcret == getrcconfigint("rcleft", NULL) || rcret == getrcconfigint("rcright", NULL)) && listbox->select != NULL && ostrcmp(listbox->select->name, "wettervor") == 0)
		{
			if(ostrcmp(wettervor->ret, "yes") == 0)
			{
				wettervorort->hidden = NO;
				//wettervorplz->hidden = NO;
  			//wettervorland->hidden = NO;
			} else {
				wettervorort->hidden = YES;
				//wettervorplz->hidden = YES;
  			//wettervorland->hidden = YES;
  		}
			
			drawscreen(pearl1_main, 0, 0);
		}
					
		if(rcret == getrcconfigint("rcexit", NULL))
			break;
		if(rcret == getrcconfigint("rcgreen", NULL))
		{
			addconfig("write_fb_to_png", allmenu->ret);
			addconfig("lcd_pearl1_plugin_standby", aktstandby->ret);
			addconfig("lcd_pearl1_plugin_wetter", wettervor->ret);
			addconfig("lcd_pearl1_plugin_wetterort", wettervorort->ret);
			//addconfig("lcd_pearl1_plugin_wetterplz", wettervorplz->ret);
			//addconfig("lcd_pearl1_plugin_wetterland", wettervorland->ret);
			restart = 1;
			break;
		}
			
		if(rcret == getrcconfigint("rcblue", NULL))
		{
			startstop = 1;
			break;
		}
	}
	delownerrc(pearl1_main);
	clearscreen(pearl1_main);
	
	if(LCD_Pearl1thread != NULL && restart == 1)
	{
		LCD_Pearl1thread->aktion = STOP;
		sleep(2);
		LCD_Pearl1thread = addtimer(&LCD_Pearl1_thread, START, 10000, 1, NULL, NULL, NULL);
		sleep(1);
		return;
	}
	if(startstop == 1)
	{
		if(LCD_Pearl1thread != NULL)
		{
			addconfig("lcd_pearl1_plugin_running", "no");
			LCD_Pearl1thread->aktion = STOP;
			sleep(1);
			system("killall lcd4linux");
			debug(10, "LCD Pearl1 unloadet !!!");	
			sleep(1);
		}
		else {
			addconfig("lcd_pearl1_plugin_running", "yes");
			firststart = 1;
			LCD_Pearl1_main();
			//LCD_Pearl1thread = addtimer(&LCD_Pearl1_thread, START, 10000, 1, NULL, NULL, NULL);
			//addtimer(&LCD_start_lcd4linux, START, 10000, 1, NULL, NULL, NULL);
			//sleep(1);
		}
	}
}
