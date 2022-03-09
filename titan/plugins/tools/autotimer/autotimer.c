#include "../../titan/struct.h"
#include "../../titan/debug.h"
#include "../../titan/header.h"

char pluginname[] = "Auto Timer";
char plugindesc[] = "Extensions";
char pluginpic[] = "%pluginpath%/autotimer/plugin.png";

int pluginaktiv = 0;
int pluginversion = PLUGINVERSION;
int autostart = 10;
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
	struct searchoption* search2 = NULL;
	struct searchoption* searchall = NULL;
	struct channel *channel1 = NULL;
	struct channel *channel2 = NULL;
	struct channel *channelall = NULL;
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
	int searchpos = 0;
	
	buf = malloc(MINMALLOC);
	if(buf == NULL)
	{
		err("no mem");
		return;
	}
	
	if(getconfigint("at1_akt", NULL) == 1)
	{
		addconfigint("at1_running", 1);
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
			search1->channelname = NULL;
			search1->search = NULL;
			free(search1); search1=NULL;
		}
	}

	if(getconfigint("at2_akt", NULL) == 1)
	{
		addconfigint("at1_running", 1);
		search2 = (struct searchoption*)calloc(1, sizeof(struct searchoption));
		if(search2 == NULL)
		{
			err("no memory");
			return;
		}
		search2->search = getconfig("at2_search", NULL);
		if(search2->search == NULL)
		{
			autotimerthread->aktion = STOP;
		}
		search2->channelname = getconfig("at2_channelname", NULL);
		if(search2->channelname == NULL)
		{
			autotimerthread->aktion = STOP;
		}
		tmpstr = ostrcat(tmpstr, getconfig("at2_starttime", NULL), 1, 0);
		if(tmpstr == NULL)
		{
			autotimerthread->aktion = STOP;
		}
		else
		{
 			tmpstr[2] = '\0';
			tmpstr[5] = '\0';
			search2->starttime = (atoi(tmpstr) * 100) + atoi(tmpstr+3);
		}
		free(tmpstr); tmpstr = NULL;
		tmpstr = ostrcat(tmpstr, getconfig("at2_endtime", NULL), 1, 0);	
		if(tmpstr == NULL)
		{
 			autotimerthread->aktion = STOP;
 		}
		else
		{
			tmpstr[2] = '\0';
			tmpstr[5] = '\0';
			search2->endtime = (atoi(tmpstr) * 100) + atoi(tmpstr+3);
		}
		free(tmpstr); tmpstr = NULL;
		if( search2->starttime > search2->endtime) {
			search2->endtime = search2->endtime + 2400;
			tageswechsel = 1;
		}
		channel2 = channel;
		channelfind = 0;
		while(channel2 != NULL)
		{
			if(ostrcmp(channel2->name, search2->channelname) == 0)
				break;
			channel2 = channel2->next;
		}
		if(channel2 == NULL)
		{
			search2->channelname = NULL;
			search2->search = NULL;
			free(search2); search1=NULL;
		}
	}
	
	if(search1 == NULL && search2 == NULL)
		autotimerthread->aktion = STOP;
		
	while (autotimerthread->aktion != STOP && autostart > 0)
	{
		sleep(1);
		autostart = autostart - 1;
	}
		
	while (autotimerthread->aktion != STOP)
	{
		searchall = NULL;
		channelall = NULL;
		searchpos = 0;
		if(search1 != NULL)
		{
			searchall = search1;
			channelall = channel1;
			searchpos = 1;
		}
		else if(search2 != NULL)
		{
			searchall = search2;
			channelall = channel2;
			searchpos = 2;
		}
		
		while (searchall != NULL)
		{
			i = 0;
			node = NULL;
			write_note = 0;
			epgnode = getepgakt(channelall);
			while(epgnode != NULL)
			{					
				result = NULL;
				loctime = olocaltime(&epgnode->starttime);
				strftime(buf, MINMALLOC, "%H%M", loctime);
				epg_starttime = atoi(buf);
				free(loctime); loctime = NULL;
				if(tageswechsel == 1 && epg_starttime < searchall->starttime)
					epg_starttime = epg_starttime + 2400;
				if(epg_starttime >= searchall->starttime &&  epg_starttime <= searchall->endtime)
				{
					if(epgnode->title != NULL)
							result = ostrstrcase(epgnode->title, searchall->search);
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
						node->transponderid = channelall->transponderid;
						recnode = rectimer;
						konflikt = 0;
						
						//gleiche Sendung, gleicher Sender?
						while(recnode != NULL)
						{
							if(recnode != node && recnode->status < 2)
							{
								if((node->begin >= recnode->begin && node->begin < recnode->end) || (node->end >= recnode->begin && node->end < recnode->end))
								{
									if(ostrcmp(epgnode->title, recnode->name) == 0 && channelall->transponderid == recnode->transponderid && channelall->servicetype == recnode->servicetype)
									{	
										konflikt = 1;
										break;
									}
								}
							}
							recnode = recnode->next;
						}
						
						//allgemeiner Konflikt?
						recnode = rectimer;
						if(konflikt == 0)
						{
							if(checkrectimerconflict(recnode) != 0) 
								konflikt = 1;
						}
						
						if(konflikt == 0)
						{
							node->pincode = ostrcat("0000", NULL, 0, 0);
							node->recpath = ostrcat(NULL, getconfig("rec_path", NULL), 0, 0);
							node->afterevent = 0;
							node->repeate = 0;
							if(searchpos == 1)
							{
								node->justplay = getconfigint("at1_event", NULL);
								node->afterevent = getconfigint("at1_afterevent", NULL);
							}
							else if(searchpos == 2)
							{
								node->justplay = getconfigint("at2_event", NULL);
								node->afterevent = getconfigint("at2_afterevent", NULL);
							}
							node->serviceid = channelall->serviceid;
							node->servicetype = channelall->servicetype;
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
			if(searchpos == 1)
			{
				if(search2 != NULL)
				{
					searchall = search2;
					channelall = channel2;
					searchpos = 2;
				}
				else
				{
					searchall = NULL;
					channelall = NULL;
				  searchpos = 0;
				}
			}
			else if(searchpos == 2)
			{
				searchall = NULL;
				channelall = NULL;
				searchpos = 0;
			}
		}
		while (autotimerthread->aktion != STOP && i < 43200)
		{
			sleep(2);
			i = i + 1;
		}
	}
	free(buf); buf = NULL;
	if(search1 != NULL) {
		search1->channelname = NULL;
		search1->search = NULL;
		free(search1); search1 = NULL;
	}
	if(search2 != NULL) {
		search2->channelname = NULL;
		search2->search = NULL;
		free(search2); search2 = NULL;
	}
	autotimerthread = NULL;
	addconfigint("at1_running", 0);
  return;
}


void autotimer_main()
{
	autostart = 10;
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
	tmpstr = createpluginpath("/autotimer/skin.xml", 0);
	readscreen(tmpstr, 120, 1);
	free(tmpstr); tmpstr = NULL;
	debug(10, "Auto Timer Plugin loaded !!!");
	if(getconfig("at1_akt", NULL) != NULL)
	{
		if(getconfigint("at1_running", NULL) == 1)
		{
			if(autotimerthread == NULL)
			{
				autostart = 120;
				autotimerthread = addtimer(&autotimer_thread, START, 10000, 1, NULL, NULL, NULL);
			}
		}
	}
	else
		addconfigint("at1_running", 0);
}

//wird beim entladen ausgefuehrt
void deinit(void)
{
	if(autotimerthread != NULL)
	{
		autotimerthread->aktion = STOP;
		sleep(2);
	}
	delmarkedscreen(120);
	pluginaktiv = 0;
	debug(10, "Auto Timer removed !!!");
}

//wird in der Pluginverwaltung bzw Menue ausfeguehrt
void start(void)
{
	char* tmpstr = NULL;
	int rcret;
 
	struct skin* autotimer = getscreen("autotimer");
	struct skin* autotimer_listbox = getscreennode(autotimer, "listbox");
	
	struct skin* autotimer_akt = getscreennode(autotimer, "akt");
	struct skin* autotimer_channel = getscreennode(autotimer, "channel");
	struct skin* autotimer_search = getscreennode(autotimer, "search");
	struct skin* autotimer_begin = getscreennode(autotimer, "begin");
	struct skin* autotimer_end = getscreennode(autotimer, "end");
	struct skin* autotimer_event = getscreennode(autotimer, "event");
	struct skin* autotimer_afterevent = getscreennode(autotimer, "afterevent");
	
	struct skin* autotimer_akt2 = getscreennode(autotimer, "akt2");
	struct skin* autotimer_channel2 = getscreennode(autotimer, "channel2");
	struct skin* autotimer_search2 = getscreennode(autotimer, "search2");
	struct skin* autotimer_begin2 = getscreennode(autotimer, "begin2");
	struct skin* autotimer_end2 = getscreennode(autotimer, "end2");
	struct skin* autotimer_event2 = getscreennode(autotimer, "event2");
	struct skin* autotimer_afterevent2 = getscreennode(autotimer, "afterevent2");
	
	struct skin* autotimer_blue = getscreennode(autotimer, "blue");
	struct skin* tmp = NULL;
	
	autostart = 10;
	
	if(autotimerthread == NULL)
		changetext(autotimer_blue, "Autostart");
	else
		changetext(autotimer_blue, "Stop");

	//**** Block1
	addchoicebox(autotimer_akt, "0", _("no"));
	addchoicebox(autotimer_akt, "1", _("yes"));
	setchoiceboxselection(autotimer_akt, getconfig("at1_akt", NULL));
	
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
	
	addchoicebox(autotimer_afterevent, "0", _("auto"));
	addchoicebox(autotimer_afterevent, "1", _("nothing"));
	addchoicebox(autotimer_afterevent, "2", _("standby"));
	addchoicebox(autotimer_afterevent, "3", _("power off"));
	setchoiceboxselection(autotimer_afterevent, getconfig("at1_afterevent", NULL));
	
	//**** Block2
	addchoicebox(autotimer_akt2, "0", _("no"));
	addchoicebox(autotimer_akt2, "1", _("yes"));
	setchoiceboxselection(autotimer_akt2, getconfig("at2_akt", NULL));
	
	changemask(autotimer_channel2, "abcdefghijklmnopqrstuvwxyz");
	changeinput(autotimer_channel2, getconfig("at2_channelname", NULL));
	
	changemask(autotimer_search2, "abcdefghijklmnopqrstuvwxyz");
	changeinput(autotimer_search2, getconfig("at2_search", NULL));

	changemask(autotimer_begin2, "00:00");
	if(getconfig("at2_starttime", NULL)== NULL)
		changeinput(autotimer_begin2, "00:00");
	else
		changeinput(autotimer_begin2, getconfig("at2_starttime", NULL));
	
	changemask(autotimer_end2, "00:00");
	if(getconfig("at2_endtime", NULL) == NULL)
		changeinput(autotimer_end2, "23:59");
	else
		changeinput(autotimer_end2, getconfig("at2_endtime", NULL));
	
	addchoicebox(autotimer_event2, "0", _("record"));
	addchoicebox(autotimer_event2, "1", _("switch channel"));
	setchoiceboxselection(autotimer_event2, getconfig("at2_event", NULL));
	
	addchoicebox(autotimer_afterevent2, "0", _("auto"));
	addchoicebox(autotimer_afterevent2, "1", _("nothing"));
	addchoicebox(autotimer_afterevent2, "2", _("standby"));
	addchoicebox(autotimer_afterevent2, "3", _("power off"));
	setchoiceboxselection(autotimer_afterevent2, getconfig("at2_afterevent", NULL));	
	
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
					textbox(_("Stop AutoTimer"), _("AutoTimer succesfully stopped"), _("OK"), getrcconfigint("rcok", NULL), NULL, 0, NULL, 0, NULL, 0, 600, 200, 0, 0);	
					changetext(autotimer_blue, "Autostart");
					addconfigint("at1_running", 0);
				}
				else
				{
					textbox(_("Stop AutoTimer"), _("ERROR... AutoTimer not stopped"), _("OK"), getrcconfigint("rcok", NULL), NULL, 0, NULL, 0, NULL, 0, 600, 200, 0, 0);		
					addconfigint("at1_running", 1);
				}
			}
			else
			{	
				addconfig("at1_akt", autotimer_akt->ret);
				addconfig("at1_channelname", autotimer_channel->ret);
				addconfig("at1_search", autotimer_search->ret);
				addconfig("at1_starttime", autotimer_begin->ret);
				addconfig("at1_endtime", autotimer_end->ret);
				addconfig("at1_event", autotimer_event->ret);
				addconfig("at1_afterevent", autotimer_afterevent->ret);
				addconfig("at2_akt", autotimer_akt2->ret);
				addconfig("at2_channelname", autotimer_channel2->ret);
				addconfig("at2_search", autotimer_search2->ret);
				addconfig("at2_starttime", autotimer_begin2->ret);
				addconfig("at2_endtime", autotimer_end2->ret);
				addconfig("at2_event", autotimer_event2->ret);
				addconfig("at2_afterevent", autotimer_afterevent2->ret);
				
				autotimerthread = addtimer(&autotimer_thread, START, 10000, 1, NULL, NULL, NULL);
				sleep(1);
				if(autotimerthread != NULL)
				{
					textbox(_("Start AutoTimer"), _("AutoTimer succesfully started"), _("OK"), getrcconfigint("rcok", NULL), NULL, 0, NULL, 0, NULL, 0, 600, 200, 0, 0);	
					changetext(autotimer_blue, "Stop");
					addconfigint("at1_running", 1);
				}
				else
				{
					addconfigint("at1_running", 0);
					textbox(_("Start AutoTimer"), _("ERROR... AutoTimer not started"), _("OK"), getrcconfigint("rcok", NULL), NULL, 0, NULL, 0, NULL, 0, 600, 200, 0, 0);		
				}
			}
		}
		if (rcret == getrcconfigint("rcgreen", NULL))
		{
			addconfig("at1_akt", autotimer_akt->ret);
			addconfig("at1_channelname", autotimer_channel->ret);
			addconfig("at1_search", autotimer_search->ret);
			addconfig("at1_starttime", autotimer_begin->ret);
			addconfig("at1_endtime", autotimer_end->ret);
			addconfig("at1_event", autotimer_event->ret);
			addconfig("at1_afterevent", autotimer_afterevent->ret);
			addconfig("at2_akt", autotimer_akt2->ret);
			addconfig("at2_channelname", autotimer_channel2->ret);
			addconfig("at2_search", autotimer_search2->ret);
			addconfig("at2_starttime", autotimer_begin2->ret);
			addconfig("at2_endtime", autotimer_end2->ret);
			addconfig("at2_event", autotimer_event2->ret);
			addconfig("at2_afterevent", autotimer_afterevent2->ret);
			
			if(getconfigint("at1_running", NULL) == 1)
			{
				autotimerthread->aktion = STOP;
				sleep(2);
				if(autotimerthread == NULL)
					autotimerthread = addtimer(&autotimer_thread, START, 10000, 1, NULL, NULL, NULL);
			}
			textbox(_("Save Settings"), _("Settings succesfully saved"), _("OK"), getrcconfigint("rcok", NULL), NULL, 0, NULL, 0, NULL, 0, 600, 200, 0, 0);
		}
		if (rcret == getrcconfigint("rcepg", NULL))
		{
			if(status.aktservice->channel != NULL)
			{
				tmpstr = ostrcat(tmpstr, status.aktservice->channel->name, 1, 0);
				if(autotimer_listbox->select != NULL)
				{
					if(ostrcmp(autotimer_listbox->select->name, "channel") == 0)
						changeinput(autotimer_channel, tmpstr);
					else if(ostrcmp(autotimer_listbox->select->name, "channel2") == 0)
						changeinput(autotimer_channel2, tmpstr);
				}
				tmpstr = NULL;
			}
		}
			

		drawscreen(autotimer, 0, 0);
	}
	delownerrc(autotimer);
	clearscreen(autotimer);	
}
