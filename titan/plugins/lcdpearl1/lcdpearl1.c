#include "../../titan/struct.h"
#include "../../titan/debug.h"
#include "../../titan/header.h"

char pluginname[] = "LCD Pearl";
char plugindesc[] = "Extensions";
char pluginpic[] = "%pluginpath%/lcdpearl1/lcdpearl.png";

int pluginaktiv = 0;
//struct skin* pluginmenu = NULL;
//int pluginflag = 1; //don't show the plugin in pluginmanager

struct stimerthread* LCD_Pearl1thread = NULL;
int firststart = 1;

void LCD_start_lcd4linux()
{
	int count = 0;
	char* startlcd = ostrcat(getconfig("pluginpath", NULL), "/lcdpearl1/start.sh", 0, 0);
	if(firststart == 1)
		sleep(3);
	while (LCD_Pearl1thread->aktion != STOP && system("ps | grep -v grep | grep lcd4linux ") != 0) {
		system(startlcd);
		sleep(6);
		count ++;
		if(count == 101) {
			textbox(_("Message"), _("ERROR cant start LCD driver"), _("OK"), getrcconfigint("rcok", NULL), NULL, 0, NULL, 0, NULL, 0, 600, 200, 5, 0);
			LCD_Pearl1thread->aktion = STOP;
		}
	}
	free(startlcd);startlcd=NULL;	
}

void LCD_Pearl1_thread()
{
	
	char* tmpstr = NULL, *tmpstr2 = NULL, *tmpstr3 = NULL, *timemerk = NULL, *sendermerk = NULL, *recmerk = NULL;
	struct skin* LCD_Pearl1 = getscreen("LCD_Pearl1");
	struct skin* akttime = getscreennode(LCD_Pearl1, "akttime");
	int put = 0;
	char* fbgrab = ostrcat(getconfig("pluginpath", NULL), "/lcdpearl1/fbgrab -f /tmp/titanlcd.raw -w 320 -h 240 -b 32 -i /tmp/.titanlcd1.png > /dev/null", 0, 0);

	if(firststart == 1)
		sleep(2);
	while (LCD_Pearl1thread->aktion != STOP) {
		put = 0;
		tmpstr = gettime("%H:%M"); 
		tmpstr2 = getaktchannelname();
		tmpstr3 = getrec(NULL);

		if(status.security == 1)
		{
			if(ostrcmp(tmpstr, timemerk) != 0)
			{
				free(timemerk);timemerk=NULL;
				timemerk = ostrcat(tmpstr, "", 0, 0);
				put = 1;
			} 
			if(ostrcmp(tmpstr2, sendermerk) != 0)
			{
				free(sendermerk);sendermerk=NULL;
				sendermerk = ostrcat(tmpstr2, "", 0, 0);
				put = 1;
			} 
			if(tmpstr3 == NULL && recmerk != NULL)
			{
				put = 1
				free(recmerk);recmerk=NULL;
			}
			else if(ostrcmp(tmpstr3, recmerk) != 0)
			{
				free(recmerk);recmerk=NULL;
				recmerk = ostrcat(tmpstr3, "", 0, 0);
				put = 1;
			}

			if(put == 1)
			{
				changetext(akttime, tmpstr);
				drawscreen(LCD_Pearl1, 0);
				system(fbgrab);
				system("mv /tmp/.titanlcd1.png /tmp/titanlcd.png");
				
				//system("/var/bin/fbgrab -f /tmp/titanlcd.raw -w 320 -h 240 -b 32 -i /tmp/titanlcd.png > /dev/null");
				system("xloadimage /tmp/titanlcd.png > /dev/null &");
			}
		}
		free(tmpstr); tmpstr = NULL;
		free(tmpstr2); tmpstr2 = NULL;
		free(tmpstr3); tmpstr3 = NULL;
		sleep(1);
	}
 	free(timemerk);timemerk=NULL;
 	free(sendermerk);sendermerk=NULL;
 	free(recmerk);recmerk=NULL;
 	free(fbgrab);fbgrab=NULL;
 	addconfig("lcd_pearl1_plugin_running", "no");
 	LCD_Pearl1thread = NULL;
 	return;
}

void LCD_Pearl1_main()
{
	if(LCD_Pearl1thread == NULL)
	{
		textbox(_("Message"), _("LCD Pearl1 starts ..."), _("OK"), getrcconfigint("rcok", NULL), NULL, 0, NULL, 0, NULL, 0, 600, 200, 5, 0);
		addconfig("lcd_pearl1_plugin_running", "yes");
		LCD_Pearl1thread = addtimer(&LCD_Pearl1_thread, START, 10000, 1, NULL, NULL, NULL);
		addtimer(&LCD_start_lcd4linux, START, 10000, 1, NULL, NULL, NULL);
		firststart = 0;
	}
	else
	{
		if (getrcconfigint("rcok", NULL) ==  textbox(_("Message"), _("Stop LCD Pearl1 ?"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("exit", NULL), NULL, 0, NULL, 0, 600, 200, 5, 0));
		{
			LCD_Pearl1thread->aktion = STOP;
			system("echo ende > /tmp/titanlcd.png");
			sleep(1);
			system("killall lcd4linux");
		}
	}
}	
			
//wird beim laden ausgefuehrt
void init(void)
{
	char* tmpstr = NULL;
	pluginaktiv = 1;
	firststart = 1;
	
	tmpstr = ostrcat(getconfig("pluginpath", NULL), "/lcdpearl1/skin.xml", 0, 0);
	readscreen(tmpstr, 115, 1);
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
	delmarkedscreen(115);
	pluginaktiv = 0;
	system("echo ende > /tmp/titanlcd.png");
	sleep(1);
	system("killall lcd4linux");
	debug(10, "LCD Pearl1 unloadet !!!");
}


//wird in der Pluginverwaltung bzw Menue ausfeguehrt
void start(void)
{
	LCD_Pearl1_main();
}
