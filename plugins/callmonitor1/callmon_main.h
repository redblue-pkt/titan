//#ifndef CALLMON_MAIN_H
//#define CALLMON_MAIN_H


void callmon_main()
{
	char* tmpstr = NULL;
	
	struct clist *ownconfig[LISTHASHSIZE] = {NULL};
	char* callconf=NULL;
	callconf = ostrcat(getconfig("pluginpath", NULL), "/callmonitor1/callmon.conf", 0, 0);
	readconfig(callconf, ownconfig);
		
	debug(1000, "in");
	status.hangtime = 99999;
	int rcret = 0;
	int hgreen = 0;
	int hblue = 0;

	struct skin* callmon_main = getscreen("callmon_main");
	struct skin* listbox = getscreennode(callmon_main, "listbox");
	struct skin* ipaddresse = getscreennode(callmon_main, "ipaddresse");
	struct skin* phonebook = getscreennode(callmon_main, "phonebook");
	struct skin* passwort = getscreennode(callmon_main, "passwort");
	struct skin* rufnummer1 = getscreennode(callmon_main, "rufnummer1");
	struct skin* rufnummer2 = getscreennode(callmon_main, "rufnummer2");
	struct skin* allnum = getscreennode(callmon_main, "allnum");
	struct skin* ausgehend = getscreennode(callmon_main, "ausgehend");
	struct skin* eingehend = getscreennode(callmon_main, "eingehend");
	struct skin* stumm = getscreennode(callmon_main, "stumm");
	struct skin* wennaus = getscreennode(callmon_main, "wennaus");
	struct skin* atimeout = getscreennode(callmon_main, "atimeout");
	
	struct skin* tmp = NULL;
	
	changemask(passwort, "abcdefghijklmnopqrstuvwxyz");
	changeinput(passwort,getlist(ownconfig, "FritzPass", NULL));
	
	changemask(ipaddresse, "000.000.000.000");
	changeinput(ipaddresse,fixip(getlist(ownconfig, "FRITZBOXIP", NULL),0));
	
	changemask(rufnummer1, "0000000000000");
	changeinput(rufnummer1,"0000000000000");
	changemask(rufnummer2, "0000000000000");
	changeinput(rufnummer2,"0000000000000");
	
	addchoicebox(allnum, "ja", _("ja"));
	addchoicebox(allnum, "nein", _("nein"));
	if(ostrcmp(getlist(ownconfig, "Alle", NULL), "1")  == 0) {
		setchoiceboxselection(allnum, "ja");
		rufnummer1->hidden=YES;
		rufnummer2->hidden=YES;
	}
	else {
		setchoiceboxselection(allnum, "nein");
		rufnummer1->hidden=NO;
		rufnummer2->hidden=NO;
	}
	
	//addlist(ownconfig, "key", Value));
	//writelist(ownconfig, callconf);
	//freelist(ownconfig);
	
	addchoicebox(phonebook, "1", _("ja"));
	addchoicebox(phonebook, "0", _("nein"));
	setchoiceboxselection(phonebook, getlist(ownconfig, "usePhoneBook", NULL));
	
	addchoicebox(eingehend, "1", _("ja"));
	addchoicebox(eingehend, "0", _("nein"));
	setchoiceboxselection(eingehend, getlist(ownconfig, "monRing", NULL));
	
	addchoicebox(ausgehend, "1", _("ja"));
	addchoicebox(ausgehend, "0", _("nein"));
	setchoiceboxselection(ausgehend, getlist(ownconfig, "monCall", NULL));
	
	addchoicebox(stumm, "1", _("ja"));
	addchoicebox(stumm, "0", _("nein"));
	setchoiceboxselection(ausgehend, getlist(ownconfig, "muteRing", NULL));
	
	addchoicebox(wennaus, "1", _("puffern"));
	addchoicebox(wennaus, "0", _("verwerfen"));
	setchoiceboxselection(wennaus, getlist(ownconfig, "anzeigewennaus", NULL));
	
	addchoicebox(atimeout, "3", _("3"));
	addchoicebox(atimeout, "4", _("4"));
	addchoicebox(atimeout, "5", _("5"));
	addchoicebox(atimeout, "6", _("6"));
	addchoicebox(atimeout, "8", _("8"));
	addchoicebox(atimeout, "10", _("10"));
	addchoicebox(atimeout, "12", _("12"));
	setchoiceboxselection(atimeout, getlist(ownconfig, "anzeigetimeout", NULL));

	drawscreen(callmon_main, 0);
	addscreenrc(callmon_main, listbox);

	tmp = listbox->select;
	while(1)
	{
		
		addscreenrc(callmon_main, tmp);
		rcret = waitrc(callmon_main, 0, 0);
		tmp = listbox->select;
		
		hgreen=0;
		hblue=0;

		if (rcret == getrcconfigint("rcexit", NULL)) break;
	
		if((rcret == getrcconfigint("rcleft", NULL) || rcret == getrcconfigint("rcright", NULL)) && ostrcmp(listbox->select->name, "allnum") == 0)
		{
			rufnummer1->hidden=NO;
			rufnummer2->hidden=NO;
			if(ostrcmp(allnum->ret, "ja") == 0)
			{
				rufnummer1->hidden=YES;
				rufnummer2->hidden=YES;
			}
			drawscreen(callmon_main, 0);
		}
		if(rcret == getrcconfigint("rcgreen", NULL))
			hgreen = 1;
		if(rcret == getrcconfigint("rcblue", NULL))
			hblue = 1;
		
		if(hgreen == 1 || hblue == 1) {
			
			tmpstr = ostrcat(getconfig("pluginpath", NULL), "/callmonitor1/fritzbox_msg_new.sh stop", 0, 0);
			system(tmpstr);
			free(tmpstr); tmpstr = NULL;
			
			addlist(ownconfig, "FRITZBOXIP", fixip(ipaddresse->ret,1));
			addlist(ownconfig, "monRing", eingehend->ret);
			addlist(ownconfig, "monCall", ausgehend->ret);
			addlist(ownconfig, "muteRing", stumm->ret);
			addlist(ownconfig, "anzeigewennaus", wennaus->ret);
			addlist(ownconfig, "anzeigetimeout", atimeout->ret);
			addlist(ownconfig, "usePhoneBook", phonebook->ret);
			addlist(ownconfig, "FritzPass", passwort->ret);
			if(ostrcmp(allnum->ret, "ja")  == 0)
				addlist(ownconfig, "Alle", "1");
			else
				addlist(ownconfig, "Alle", "0");
			writelist(ownconfig, callconf);
			
			if(hblue == 1 && ostrcmp(getlist(ownconfig, "usePhoneBook", NULL), "1")  == 0) {
				tmpstr = ostrcat(getconfig("pluginpath", NULL), "/callmonitor1/fritzbox_msg_new.sh loadPhoneBook", 0, 0);
				system(tmpstr);
				free(tmpstr); tmpstr = NULL;
			}
			
			tmpstr = ostrcat(getconfig("pluginpath", NULL), "/callmonitor1/fritzbox_msg_new.sh start", 0, 0);
			system(tmpstr);
			free(tmpstr); tmpstr = NULL;
			
			break;
		}
	}
	delownerrc;
	clearscreen(callmon_main);
	freelist(ownconfig);
	free(callconf); callconf=NULL;
	status.hangtime= getconfigint("hangtime", NULL);
	
}
	