//#ifndef INSTAR_H
//#define INSTAR_H

char* cam1 = NULL;
char* cam2 = NULL;
struct skin* instar_screen = NULL; 
int actcam = 1;

struct clist *myconfig[LISTHASHSIZE] = {NULL};
char* instarconf = NULL;

int show_control()
{
	char* tmpstr = NULL;
	char* tempcam = NULL;
	char* position = NULL;
	int rcret = 0;
	int rcode = 0;
	
	if(actcam == 1)
		tempcam = cam1;
	if(actcam == 2)
		tempcam = cam2;
	
	setfbtransparent(255);
	
	while(1)
	{
		if(cam1 != NULL)
		{
			tmpstr = ostrcat(cam1, "/snapshot.cgi", 0, 0);
			system(tmpstr);
			free(tmpstr); tmpstr = NULL;
		}
		if(cam2 != NULL)
		{
			tmpstr = ostrcat(cam2, "/snapshot.cgi", 0, 0);
			system(tmpstr);
			free(tmpstr); tmpstr = NULL;
		}
		
		drawscreen(instar_screen, 0);
		rcret = waitrc(instar_screen, 250, 0);

		if(rcret == getrcconfigint("rcexit", NULL)) break;
		
		if(rcret == getrcconfigint("rcinfo", NULL))
		{
			addlist(myconfig, "InstarCamAutostart", "nein");
			rcode = 1;
			break;
		}	
			
		if(rcret == getrcconfigint("rc1", NULL))
			position = ostrcat("/decoder_control.cgi?command=", "31", 0 , 0);
		if(rcret == getrcconfigint("rc2", NULL))
			position = ostrcat("/decoder_control.cgi?command=", "33", 0 , 0);
		if(rcret == getrcconfigint("rc3", NULL))
			position = ostrcat("/decoder_control.cgi?command=", "35", 0 , 0);
		if(rcret == getrcconfigint("rc4", NULL))
			position = ostrcat("/decoder_control.cgi?command=", "37", 0 , 0);
		if(rcret == getrcconfigint("rc5", NULL))
			position = ostrcat("/decoder_control.cgi?command=", "39", 0 , 0);
		if(rcret == getrcconfigint("rc6", NULL))
			position = ostrcat("/decoder_control.cgi?command=", "41", 0 , 0);
		if(rcret == getrcconfigint("rc7", NULL))
			position = ostrcat("/decoder_control.cgi?command=", "43", 0 , 0);
		if(rcret == getrcconfigint("rc8", NULL))
			position = ostrcat("/decoder_control.cgi?command=", "45", 0 , 0);
		if(rcret == getrcconfigint("rc9", NULL))
			position = ostrcat("/decoder_control.cgi?command=", "47", 0 , 0);
		if(rcret == getrcconfigint("rcup", NULL))
			position = ostrcat("/decoder_control.cgi?command=", "0&onestep=1", 0 , 0);
		if(rcret == getrcconfigint("rcdown", NULL))
			position = ostrcat("/decoder_control.cgi?command=", "2&onestep=1", 0 , 0);
		if(rcret == getrcconfigint("rcleft", NULL))
			position = ostrcat("/decoder_control.cgi?command=", "4&onestep=1", 0 , 0);
		if(rcret == getrcconfigint("rcright", NULL))
			position = ostrcat("/decoder_control.cgi?command=", "6&onestep=1", 0 , 0);
		
		
		if(position != NULL && tempcam != NULL)
		{
			tmpstr = ostrcat(tempcam, position, 0, 0);
			system(tmpstr);
			free(tmpstr); tmpstr = NULL;	
			free(position); position = NULL;
		}
	}

	clearscreen(instar_screen);
	setosdtransparent(getskinconfigint("osdtransparent", NULL));
	return rcode;
}

int instar_settings()
{
	
	int rcret = 0;
	int rcode = 0;
	
	struct skin* instar_einstellungen = getscreen("instar_einstellungen");
	struct skin* listbox = getscreennode(instar_einstellungen, "listbox");
	struct skin* autoCam = getscreennode(instar_einstellungen, "autoCam");
	struct skin* onCam1 = getscreennode(instar_einstellungen, "onCam1");
	struct skin* ipCam1 = getscreennode(instar_einstellungen, "ipCam1");
	struct skin* portCam1 = getscreennode(instar_einstellungen, "portCam1");
	struct skin* userCam1 = getscreennode(instar_einstellungen, "userCam1");
	struct skin* passCam1 = getscreennode(instar_einstellungen, "passCam1");
	struct skin* onCam2 = getscreennode(instar_einstellungen, "onCam2");
	struct skin* ipCam2 = getscreennode(instar_einstellungen, "ipCam2");
	struct skin* portCam2 = getscreennode(instar_einstellungen, "portCam2");
	struct skin* userCam2 = getscreennode(instar_einstellungen, "userCam2");
	struct skin* passCam2 = getscreennode(instar_einstellungen, "passCam2");
	
	struct skin* tmp = NULL;

  addchoicebox(autoCam, "nein", _("nein"));
	addchoicebox(autoCam, "ja", _("ja"));
	setchoiceboxselection(autoCam, getlist(myconfig, "InstarCamAutostart", NULL));
  
  addchoicebox(onCam1, "aus", _("aus"));
	addchoicebox(onCam1, "ein", _("ein"));
	setchoiceboxselection(onCam1, getlist(myconfig, "InstarCam1", NULL));	
	changemask(ipCam1, "abcdefghijklmnopqrstuvwxyz");
  changeinput(ipCam1, getlist(myconfig, "InstarCam1IP", NULL));
  changemask(portCam1, "0000");
  changeinput(portCam1, getlist(myconfig, "InstarCam1Port", NULL));
  changemask(userCam1, "abcdefghijklmnopqrstuvwxyz");
  changeinput(userCam1, getlist(myconfig, "InstarCam1User", NULL));
  changemask(passCam1, "abcdefghijklmnopqrstuvwxyz");
  changeinput(passCam1, getlist(myconfig, "InstarCam1Pass", NULL));
  
  addchoicebox(onCam2, "aus", _("aus"));
	addchoicebox(onCam2, "ein", _("ein"));
	setchoiceboxselection(onCam2, getlist(myconfig, "InstarCam2", NULL));	
	changemask(ipCam2, "abcdefghijklmnopqrstuvwxyz");
  changeinput(ipCam2, getlist(myconfig, "InstarCam2IP", NULL));
  changemask(portCam2, "0000");
  changeinput(portCam2, getlist(myconfig, "InstarCam2Port", NULL));
  changemask(userCam2, "abcdefghijklmnopqrstuvwxyz");
  changeinput(userCam2, getlist(myconfig, "InstarCam2User", NULL));
  changemask(passCam2, "abcdefghijklmnopqrstuvwxyz");
  changeinput(passCam2, getlist(myconfig, "InstarCam2Pass", NULL));
  
	drawscreen(instar_einstellungen, 0);
	addscreenrc(instar_einstellungen, listbox);
	tmp = listbox->select;
	
	while(1)
	{
		addscreenrc(instar_einstellungen, tmp);
		rcret = waitrc(instar_einstellungen, 1000, 0);
		tmp = listbox->select;
		
		if(rcret == getrcconfigint("rcexit", NULL)) break;
		if(rcret == getrcconfigint("rcgreen", NULL) || rcret == getrcconfigint("rcblue", NULL))
		{
			addlist(myconfig, "InstarCamAutostart", autoCam->ret);
			addlist(myconfig, "InstarCam1", onCam1->ret);
			addlist(myconfig, "InstarCam1IP", ipCam1->ret);
			addlist(myconfig, "InstarCam1Port", portCam1->ret);
			addlist(myconfig, "InstarCam1User", userCam1->ret);
			addlist(myconfig, "InstarCam1Pass", passCam1->ret);
			addlist(myconfig, "InstarCam2", onCam2->ret);
			addlist(myconfig, "InstarCam2IP", ipCam2->ret);
			addlist(myconfig, "InstarCam2Port", portCam2->ret);
			addlist(myconfig, "InstarCam2User", userCam2->ret);
			addlist(myconfig, "InstarCam2Pass", passCam2->ret);
			writelist(myconfig, instarconf);
			if(rcret == getrcconfigint("rcblue", NULL))
			{
				rcode = 1;
				break;
			}
		}
	}
	delownerrc(instar_einstellungen);
	clearscreen(instar_einstellungen);
	return rcode;	
}



void instar_main()
{
	int rcode = 0;

	instarconf = ostrcat(getconfig("pluginpath", NULL), "/instar/instar.conf", 0, 0);
	readconfig(instarconf, myconfig);
	
  if(ostrcmp(getlist(myconfig, "InstarCamAutostart", NULL), "ja") == 0)
		rcode = 1;
	else
		rcode = instar_settings();
	
	if(rcode == 1)
	{
		while(1)
		{
			if(ostrcmp(getlist(myconfig, "InstarCam1", NULL), "ein") == 0)
			{
				cam1 = ostrcat("wget --output-document=/tmp/instar1.jpg http://", getlist(myconfig, "InstarCam1User", NULL), 0, 0);
				cam1 = ostrcat(cam1, ":",1, 0);
				cam1 = ostrcat(cam1, getlist(myconfig, "InstarCam1Pass", NULL), 1, 0);
				cam1 = ostrcat(cam1, "@",1, 0);
				cam1 = ostrcat(cam1, getlist(myconfig, "InstarCam1IP", NULL), 1, 0);
				cam1 = ostrcat(cam1, ":",1, 0);
				cam1 = ostrcat(cam1, getlist(myconfig, "InstarCam1Port", NULL), 1, 0);
			}
			
		
			/*
			if(ostrcmp(getlist(myconfig, "InstarCam2", NULL), "ein") == 0)
			{
				cam2 = ostrcat("wget --user=", getlist(myconfig, "InstarCam2User", NULL), 0, 0);
				cam2 = ostrcat(cam2, " --password=",1, 0);
				cam2 = ostrcat(cam2, getlist(myconfig, "InstarCam2Pass", NULL), 1, 0);
				cam2 = ostrcat(cam2, " --output-document=/tmp/instar2.jpg http://",1, 0);
				cam2 = ostrcat(cam2, fixip(getlist(myconfig, "InstarCam2IP", NULL)), 1, 0);
				cam2 = ostrcat(cam2, ":", 1, 0);
				cam2 = ostrcat(cam2, getlist(myconfig, "InstarCam2Port", NULL), 1, 0);
			}
			*/
	
			instar_screen = getscreen("instar_full");
	
			actcam = 1;
	
			rcode = show_control();
	
			free(cam1), cam1 = NULL;
			free(cam2), cam2 = NULL;
			
			if(rcode == 0)
				break;
			rcode = 0;
			
			rcode = instar_settings();
			if(rcode == 0)
				break;
		}
	}
	writelist(myconfig, instarconf);
	freelist(myconfig);
	free(instarconf); instarconf = NULL;
}
