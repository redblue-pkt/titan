//#ifndef CALLMON_MAIN_H
//#define CALLMON_MAIN_H

struct stimerthread* CallmonStartThread = NULL;
int coldstart = 0;

void callmonstart()
{
	char* tmpstr = NULL;
	
	tmpstr = createpluginpath("/callmonitor1/fritzbox_msg_new.sh", 0);
	tmpstr = ostrcat(tmpstr, " stop", 1, 0);
	system(tmpstr);
	free(tmpstr); tmpstr = NULL;
	
	if(coldstart == 1)
		sleep(10);
	else
		sleep(1); 
	
	tmpstr = createpluginpath("/callmonitor1/fritzbox_msg_new.sh", 0);
	tmpstr = ostrcat(tmpstr, " start", 1, 0);
	system(tmpstr);
	free(tmpstr); tmpstr = NULL;
	coldstart = 0;
	CallmonStartThread = NULL;
}

void callmontimeout()
{
	if(coldstart == 1)
		sleep(45);
	else
		sleep(35);
		
	if(CallmonStartThread != NULL)
	{
		system("killall fritzbox_msg_new.sh");
		textbox(_("Message"), _("Callmon Timeout !!!\n\ncheck your parameter "), _("OK"), getrcconfigint("rcok", NULL), NULL, 0, NULL, 0, NULL, 0, 650, 200,5, 0);
	}
}
	

void callmon_main()
{
	char* tmpstr = NULL;
	
	struct clist *myconfig[LISTHASHSIZE] = {NULL};
	char* callconf = NULL;
	callconf = createpluginpath("/callmonitor1/callmon.conf", 0);
	readconfig(callconf, myconfig);
		
	debug(1000, "in");
	status.hangtime = 99999;
	int rcret = 0;
	int hgreen = 0;
	int hblue = 0;

	struct skin* callmon_main = getscreen("callmon_main");
	struct skin* listbox = getscreennode(callmon_main, "listbox");
	struct skin* fritzversion = getscreennode(callmon_main, "fritzversion");
	struct skin* ipaddress = getscreennode(callmon_main, "ipaddress");
	struct skin* phonebook = getscreennode(callmon_main, "phonebook");
	struct skin* userid = getscreennode(callmon_main, "userid");
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
	
	addchoicebox(fritzversion, "50", _("nein"));
	addchoicebox(fritzversion, "60", _("ja"));
	setchoiceboxselection(fritzversion, getlist(myconfig, "FritzVersion", NULL));
	
	changemask(passwort, "abcdefghijklmnopqrstuvwxyz");
	changeinput(passwort, getlist(myconfig, "FritzPass", NULL));
	
	changemask(userid, "abcdefghijklmnopqrstuvwxyz");
	changeinput(userid, getlist(myconfig, "FritzUser", NULL));
	
	changemask(ipaddress, "000.000.000.000");
	tmpstr = fixip(getlist(myconfig, "FRITZBOXIP", NULL), 0);
	changeinput(ipaddress, tmpstr);
	free(tmpstr); tmpstr = NULL;
	
	changemask(rufnummer1, "0000000000000");
	changeinput(rufnummer1,getlist(myconfig, "Ziel", NULL));
	
	changemask(rufnummer2, "0000000000000");
	changeinput(rufnummer2,getlist(myconfig, "Ziel_2", NULL));
		
	addchoicebox(allnum, "ja", _("ja"));
	addchoicebox(allnum, "nein", _("nein"));
	if(ostrcmp(getlist(myconfig, "Alle", NULL), "1")  == 0)
	{
		setchoiceboxselection(allnum, "ja");
		rufnummer1->hidden = YES;
		rufnummer2->hidden = YES;
	}
	else {
		setchoiceboxselection(allnum, "nein");
		rufnummer1->hidden = NO;
		rufnummer2->hidden = NO;
	}
	
	//addlist(myconfig, "key", Value));
	//writelist(myconfig, callconf);
	//freelist(myconfig);
	
	addchoicebox(phonebook, "1", _("ja"));
	addchoicebox(phonebook, "0", _("nein"));
	setchoiceboxselection(phonebook, getlist(myconfig, "usePhoneBook", NULL));
	
	addchoicebox(eingehend, "1", _("ja"));
	addchoicebox(eingehend, "0", _("nein"));
	setchoiceboxselection(eingehend, getlist(myconfig, "monRing", NULL));
	
	addchoicebox(ausgehend, "1", _("ja"));
	addchoicebox(ausgehend, "0", _("nein"));
	setchoiceboxselection(ausgehend, getlist(myconfig, "monCall", NULL));
	
	addchoicebox(stumm, "1", _("ja"));
	addchoicebox(stumm, "0", _("nein"));
	setchoiceboxselection(stumm, getlist(myconfig, "muteRing", NULL));
	
	addchoicebox(wennaus, "1", _("puffern"));
	addchoicebox(wennaus, "0", _("verwerfen"));
	setchoiceboxselection(wennaus, getlist(myconfig, "anzeigewennaus", NULL));
	
	addchoicebox(atimeout, "3", _("3"));
	addchoicebox(atimeout, "4", _("4"));
	addchoicebox(atimeout, "5", _("5"));
	addchoicebox(atimeout, "6", _("6"));
	addchoicebox(atimeout, "8", _("8"));
	addchoicebox(atimeout, "10", _("10"));
	addchoicebox(atimeout, "12", _("12"));
	setchoiceboxselection(atimeout, getlist(myconfig, "anzeigetimeout", NULL));

	drawscreen(callmon_main, 0, 0);
	addscreenrc(callmon_main, listbox);

	tmp = listbox->select;
	while(1)
	{
		addscreenrc(callmon_main, tmp);
		rcret = waitrc(callmon_main, 1000, 0);
		tmp = listbox->select;
		
		hgreen=0;
		hblue=0;

		if(rcret == getrcconfigint("rcexit", NULL)) break;
	
		if((rcret == getrcconfigint("rcleft", NULL) || rcret == getrcconfigint("rcright", NULL)) && listbox->select != NULL && ostrcmp(listbox->select->name, "allnum") == 0)
		{
			rufnummer1->hidden = NO;
			rufnummer2->hidden = NO;
			if(ostrcmp(allnum->ret, "ja") == 0)
			{
				rufnummer1->hidden = YES;
				rufnummer2->hidden = YES;
			}
			drawscreen(callmon_main, 0, 0);
		}
		if(rcret == getrcconfigint("rcgreen", NULL))
			hgreen = 1;
		if(rcret == getrcconfigint("rcblue", NULL))
			hblue = 1;
		
		if(hgreen == 1 || hblue == 1)
		{	
			tmpstr = createpluginpath("/callmonitor1/fritzbox_msg_new.sh", 0);
			tmpstr = ostrcat(tmpstr, " stop", 1, 0);
			system(tmpstr);
			free(tmpstr); tmpstr = NULL;
			
			tmpstr = fixip(ipaddress->ret, 1);
			addlist(myconfig, "FRITZBOXIP", tmpstr);
			free(tmpstr); tmpstr = NULL;
			addlist(myconfig, "FritzVersion", fritzversion->ret);
			addlist(myconfig, "monRing", eingehend->ret);
			addlist(myconfig, "monCall", ausgehend->ret);
			addlist(myconfig, "muteRing", stumm->ret);
			addlist(myconfig, "anzeigewennaus", wennaus->ret);
			addlist(myconfig, "anzeigetimeout", atimeout->ret);
			addlist(myconfig, "usePhoneBook", phonebook->ret);
			addlist(myconfig, "FritzPass", passwort->ret);
			addlist(myconfig, "FritzUser", userid->ret);
			addlist(myconfig, "Ziel", rufnummer1->ret);
			addlist(myconfig, "Ziel_2", rufnummer2->ret);
			
			if(ostrcmp(allnum->ret, "ja")  == 0)
				addlist(myconfig, "Alle", "1");
			else
				addlist(myconfig, "Alle", "0");
			writelist(myconfig, callconf);
			
			if(hblue == 1 && ostrcmp(getlist(myconfig, "usePhoneBook", NULL), "1")  == 0)
			{
				tmpstr = createpluginpath("/callmonitor1/fritzbox_msg_new.sh", 0);
				tmpstr = ostrcat(tmpstr, " loadPhoneBook", 1, 0);
				system(tmpstr);
				free(tmpstr); tmpstr = NULL;
			}
			if(CallmonStartThread == NULL)
			{
				CallmonStartThread = addtimer(&callmonstart, START, 10000, 1, NULL, NULL, NULL);
				addtimer(&callmontimeout, START, 10000, 1, NULL, NULL, NULL);
			}
			free(tmpstr); tmpstr = NULL;
			
			break;
		}
	}
	delownerrc(callmon_main);
	clearscreen(callmon_main);
	freelist(myconfig);
	free(callconf); callconf = NULL;
	status.hangtime = getconfigint("hangtime", NULL);
}
	