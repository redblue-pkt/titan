#include "../../titan/struct.h"
#include "../../titan/debug.h"
#include "../../titan/header.h"

char pluginname[] = "Auto Timer";
char plugindesc[] = "Extensions";
char pluginpic[] = "%pluginpath%/autotimer/plugin.png";

int pluginaktiv = 0;
int pluginversion = 999999;
//struct skin* pluginmenu = NULL;
//int pluginflag = 1; //don't show the plugin in pluginmanager

struct rectimer *rectimer;

struct searchoption
{
	int starttime;
	int endtime;
	char* search;
	char* channelname;
};

struct stimerthread* autotimerthread = NULL;


void autotimer_thread()
{
	char* tmpstr = NULL;
	char* buf = NULL;
	char* result = NULL;
	struct searchoption* search1 = NULL;
	struct channel *channel1 = NULL;
	struct epg* epgnode = NULL;
	struct tm *loctime = NULL;
	struct rectimer *node = NULL;
	struct rectimer* recnode = rectimer;
	int channelfind = 0;
	int epg_starttime = 0;
	//int epg_endtime = 0;
	int tageswechsel = 0;
	int write_note = 0;
	int i = 0;
	int konflikt = 0;
	
	buf = malloc(MINMALLOC);
	if(buf == NULL)
	{
		err("no mem");
		return;
	}
	search1 = (struct searchoption*)calloc(1, sizeof(struct searchoption));
	if(search1 == NULL)
	{
		err("no memory");
		return;
	}
	search1->search = getconfig("at1_search", NULL);
	if(search1->search == NULL)
	{
		autotimerthread->aktion = STOP;
	}
	search1->channelname = getconfig("at1_channelname", NULL);
	if(search1->channelname == NULL)
	{
		autotimerthread->aktion = STOP;
	}
	tmpstr = ostrcat(tmpstr, getconfig("at1_starttime", NULL), 1, 0);
	if(tmpstr == NULL)
	{
		autotimerthread->aktion = STOP;
	}
	else
	{
 		tmpstr[2] = '\0';
		tmpstr[5] = '\0';
		search1->starttime = (atoi(tmpstr) * 100) + atoi(tmpstr+3);
	}
	free(tmpstr); tmpstr = NULL;
       
	tmpstr = ostrcat(tmpstr, getconfig("at1_endtime", NULL), 1, 0);
	if(tmpstr == NULL)
	{
 		autotimerthread->aktion = STOP;
 	}
	else
	{
		tmpstr[2] = '\0';
		tmpstr[5] = '\0';
		search1->endtime = (atoi(tmpstr) * 100) + atoi(tmpstr+3);
	}
	free(tmpstr); tmpstr = NULL;

	if( search1->starttime > search1->endtime) {
		search1->endtime = search1->endtime + 2400;
		tageswechsel = 1;
	}
	
	channel1 = channel;
	channelfind = 0;
	while(channel1 != NULL)
	{
		if(ostrcmp(channel1->name, search1->channelname) == 0)
			break;

		channel1 = channel1->next;
	}
	if(channel1 == NULL)
	{
		autotimerthread->aktion = STOP;
	}
	while (autotimerthread->aktion != STOP)
	{
		sleep(10);
		i = 0;
		node = NULL;
		write_note = 0;
		epgnode = getepgakt(channel1);
		while(epgnode != NULL)
		{					
			result = NULL;
			loctime = olocaltime(&epgnode->starttime);
			strftime(buf, MINMALLOC, "%H%M", loctime);
			epg_starttime = atoi(buf);
			free(loctime); loctime = NULL;
			if(tageswechsel == 1 && epg_starttime < search1->starttime)
				epg_starttime = epg_starttime + 2400;
			if(epg_starttime >= search1->starttime &&  epg_starttime <= search1->endtime)
			{
				if(epgnode->title != NULL)
						result = ostrstrcase(epgnode->title, search1->search);
				if(result != NULL)
				{
					if(node == NULL)
					{ 
						node = addrectimernode(NULL, NULL);
						node->disabled = 1;
					}
					else if(node->disabled == 0)
					{
						node = addrectimernode(NULL, NULL);
						node->disabled = 1;
					}
	
					node->begin = epgnode->starttime;
					node->begin -= getconfigint("recforerun" , NULL) * 60;
					node->begin -= (node->begin % 60);
					node->end = epgnode->endtime;
					node->end += getconfigint("recoverrun" , NULL) * 60;
					node->end -= (node->end % 60);
					node->transponderid = channel1->transponderid;
					
					konflikt = 0;
					while(recnode != NULL)
					{
						if(recnode != node && recnode->status < 2)
						{
							if((node->begin >= recnode->begin && node->begin < recnode->end) || (node->end >= recnode->begin && node->end < recnode->end))
							{
								konflikt = 1;
								break;
							}
						}
						recnode = recnode->next;
					}
						
					if(konflikt == 0)
					{
						node->pincode = ostrcat("0000", NULL, 0, 0);
						node->recpath = ostrcat(NULL, getconfig("rec_path", NULL), 0, 0);
						node->afterevent = 0;
						node->repeate = 0;
						node->justplay = getconfigint("at1_event", NULL);
						node->afterevent = 0;
						node->serviceid = channel1->serviceid;
						node->servicetype = channel1->servicetype;
						node->disabled = 0;
						node->name = ostrcat(epgnode->title, NULL, 0, 0);
						write_note = 1;
					}
				}
			}
			epgnode = epgnode->next;
		}
		if(node != NULL)
		{
			if(write_note == 1)
			{
				if(node->disabled == 1)
					delrectimer(node, 1, 0);
				else
				{
					status.writerectimer = 1;
					writerectimer(getconfig("rectimerfile", NULL), 0);
				}
			}
			else
				delrectimer(node, 0, 0);
		}
		while (autotimerthread->aktion != STOP && i < 43200)
		{
			sleep(2);
		}
	}
	free(buf); buf = NULL;
	search1->channelname = NULL;
	search1->search = NULL;
	free(search1); search1 = NULL;
	autotimerthread = NULL;
  return;
}


void autotimer_main()
{
	if(getconfigint("at1_running", NULL) == 1)
	{
		if(autotimerthread == NULL)
			autotimerthread = addtimer(&autotimer_thread, START, 10000, 1, NULL, NULL, NULL);
	}
	else
	{
		if(autotimerthread != NULL)
			autotimerthread->aktion = STOP;
	}
}	
			
//wird beim laden ausgefuehrt
void init(void)
{
	char* tmpstr = NULL;
	pluginaktiv = 1;
	tmpstr = ostrcat(getconfig("pluginpath", NULL), "/autotimer/skin.xml", 0, 0);
	readscreen(tmpstr, 120, 1);
	free(tmpstr); tmpstr = NULL;
	debug(10, "Autot Timer Plugin loadet !!!");
	autotimer_main();
}

//wird beim entladen ausgefuehrt
void deinit(void)
{
	if(autotimerthread != NULL)
		autotimerthread->aktion = STOP;
	delmarkedscreen(120);
	pluginaktiv = 0;
	debug(10, "Autot Timer removed !!!");
}

//wird in der Pluginverwaltung bzw Menue ausfeguehrt
void start(void)
{
	char* tmpstr = NULL;
	int rcret;
 
	struct skin* autotimer = getscreen("autotimer");
	struct skin* autotimer_listbox = getscreennode(autotimer, "listbox");
	struct skin* autotimer_channel = getscreennode(autotimer, "channel");
	struct skin* autotimer_search = getscreennode(autotimer, "search");
	struct skin* autotimer_begin = getscreennode(autotimer, "begin");
	struct skin* autotimer_event = getscreennode(autotimer, "event");
	struct skin* autotimer_end = getscreennode(autotimer, "end");
	struct skin* autotimer_blue = getscreennode(autotimer, "blue");
	struct skin* tmp = NULL;
	
	if(autotimerthread == NULL)
		changetext(autotimer_blue, "start");
	else
		changetext(autotimer_blue, "stop");

	changemask(autotimer_channel, "abcdefghijklmnopqrstuvwxyz");
	changeinput(autotimer_channel, getconfig("at1_channelname", NULL));
	
	changemask(autotimer_search, "abcdefghijklmnopqrstuvwxyz");
	changeinput(autotimer_search, getconfig("at1_search", NULL));

	changemask(autotimer_begin, "00:00");
	if(getconfig("at1_starttime", NULL)== NULL)
		changeinput(autotimer_begin, "00:00");
	else
		changeinput(autotimer_begin, getconfig("at1_starttime", NULL));
	
	changemask(autotimer_end, "00:00");
	if(getconfig("at1_endtime", NULL) == NULL)
		changeinput(autotimer_end, "23:59");
	else
		changeinput(autotimer_end, getconfig("at1_endtime", NULL));
	
	addchoicebox(autotimer_event, "0", _("record"));
	addchoicebox(autotimer_event, "1", _("switch channel"));
	setchoiceboxselection(autotimer_event, getconfig("at1_event", NULL));
	
	drawscreen(autotimer, 0, 0);
	addscreenrc(autotimer, autotimer_listbox);
	tmp = autotimer_listbox->select;
	
	while(1)
	{
		addscreenrc(autotimer, tmp);
		rcret = waitrc(autotimer, 0, 0);
		tmp = autotimer_listbox->select;
		
		if (rcret == getrcconfigint("rcexit", NULL)) break;
		if (rcret == getrcconfigint("rcblue", NULL))
		{
			if(autotimerthread != NULL)
			{
				autotimerthread->aktion = STOP;
				sleep(3);
				if(autotimerthread == NULL)
				{
					textbox(_("Stop AutoTimer"), _("AutoTimer succesfull stopped"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 0, 0);	
					changetext(autotimer_blue, "start");
					addconfigint("at1_running", 0);
				}
				else
				{
					textbox(_("Stop AutoTimer"), _("ERROR... AutoTimer not stopped"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 0, 0);		
					addconfigint("at1_running", 1);
				}
			}
			else
			{	
				addconfig("at1_channelname", autotimer_channel->ret);
				addconfig("at1_search", autotimer_search->ret);
				addconfig("at1_starttime", autotimer_begin->ret);
				addconfig("at1_endtime", autotimer_end->ret);
				addconfig("at1_event", autotimer_event->ret);
				autotimerthread = addtimer(&autotimer_thread, START, 10000, 1, NULL, NULL, NULL);
				sleep(1);
				if(autotimerthread != NULL)
				{
					textbox(_("Start AutoTimer"), _("AutoTimer succesfull started"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 0, 0);	
					changetext(autotimer_blue, "stop");
					addconfigint("at1_running", 1);
				}
				else
				{
					addconfigint("at1_running", 0);
					textbox(_("Start AutoTimer"), _("ERROR... AutoTimer not started"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 0, 0);		
				}
			}
		}
		if (rcret == getrcconfigint("rcgreen", NULL))
		{
			addconfig("at1_channelname", autotimer_channel->ret);
			addconfig("at1_search", autotimer_search->ret);
			addconfig("at1_starttime", autotimer_begin->ret);
			addconfig("at1_endtime", autotimer_end->ret);
			addconfig("at1_event", autotimer_event->ret);
			if(getconfigint("at1_running", NULL) == 1)
			{
				autotimerthread->aktion = STOP;
				sleep(2);
				if(autotimerthread == NULL)
					autotimerthread = addtimer(&autotimer_thread, START, 10000, 1, NULL, NULL, NULL);
			}
			textbox(_("Save Settings"), _("Settings succesfull saved"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 0, 0);
		}
		if (rcret == getrcconfigint("rcepg", NULL))
		{
			if(status.aktservice->channel != NULL)
			{
				tmpstr = ostrcat(tmpstr, status.aktservice->channel->name, 1, 0);
				changeinput(autotimer_channel, tmpstr);
				tmpstr = NULL;
			}
		}
			

		drawscreen(autotimer, 0, 0);
	}
	delownerrc(autotimer);
	clearscreen(autotimer);	
}