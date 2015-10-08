//#ifndef INSTAR_H
//#define INSTAR_H

char* cam1 = NULL;
char* cam2 = NULL;
//char* cam11 = NULL;
//char* cam21 = NULL;
struct skin* instar_screen = NULL; 
struct skin* instar_pic1 = NULL;
struct skin* instar_actcam1 = NULL;
int actcam = 1;

struct clist *myconfig[LISTHASHSIZE] = {NULL};
char* instarconf = NULL;

int show_control()
{
	char* tmpstr = NULL;
	char* tempcam = NULL;
	char* position = NULL;
	char* positionstop = NULL;
	int rcret = 0;
	int rcode = 0;
	int ohnestop = 0;
	
	if(actcam == 1)
		tempcam = cam1;
	if(actcam == 2)
		tempcam = cam2;
	
	setfbtransparent(255);
	
	printf("[ip-cam] Aufruf1: %s\n", cam1);
	printf("[ip-cam] Aufruf2: %s\n", cam2);
	
	while(1)
	{
		//if(getconfigint("instar_1", NULL) == 2 && cam11 != NULL)
		//	system(cam11);
		//else
		//{
			if(cam1 != NULL)
			{
				if(getconfigint("instar_1", NULL) == 1)
					tmpstr = ostrcat(cam1, "/image/jpeg.cgi", 0, 0);
				else if(getconfigint("instar_1", NULL) == 2)
					tmpstr = ostrcat(cam1, "/tmpfs/auto.jpg", 0, 0);
				else if(getconfigint("instar_1", NULL) == 3)
					tmpstr = ostrcat(cam1, "/onvif/GetSnapshotUri", 0, 0);
				else
					tmpstr = ostrcat(cam1, "/snapshot.cgi", 0, 0);
				if(getconfigint("instar_alterwebif", NULL) == 1)
					tmpstr = ostrcat(tmpstr, " > /tmp/instar1.jpg", 1, 0);
				system(tmpstr);
				free(tmpstr); tmpstr = NULL;
			}
			if(cam2 != NULL)
			{
				if(getconfigint("instar_2", NULL) == 1)
					tmpstr = ostrcat(cam2, "/image/jpeg.cgi", 0, 0);
				else if(getconfigint("instar_2", NULL) == 2)
					tmpstr = ostrcat(cam2, "/tmpfs/auto.jpg", 0, 0);
				else if(getconfigint("instar_2", NULL) == 3)
					tmpstr = ostrcat(cam2, "/onvif/GetSnapshotUri", 0, 0);
				else
					tmpstr = ostrcat(cam2, "/snapshot.cgi", 0, 0);
				if(getconfigint("instar_alterwebif", NULL) == 1)
					tmpstr = ostrcat(tmpstr, " > /tmp/instar2.jpg", 1, 0);
				system(tmpstr);
				free(tmpstr); tmpstr = NULL;
			}
		//}
				
		drawscreen(instar_screen, 0, 0);
		rcret = waitrc(instar_screen, 100, 0);
		ohnestop = 0;

		if(rcret == getrcconfigint("rcexit", NULL)) break;
		
		if(rcret == getrcconfigint("rcinfo", NULL))
		{
			//addlist(myconfig, "InstarCamAutostart", "nein");
			rcode = 1;
			break;
		}	
			
		if(rcret == getrcconfigint("rc1", NULL)) {
			position = ostrcat("/decoder_control.cgi?command=", "31", 0 , 0);
			ohnestop = 1;
		}
		if(rcret == getrcconfigint("rc2", NULL)) {
			position = ostrcat("/decoder_control.cgi?command=", "33", 0 , 0);
			ohnestop = 1;
		}
		if(rcret == getrcconfigint("rc3", NULL)) {
			position = ostrcat("/decoder_control.cgi?command=", "35", 0 , 0);
			ohnestop = 1;
		}
		if(rcret == getrcconfigint("rc4", NULL)) {
			position = ostrcat("/decoder_control.cgi?command=", "37", 0 , 0);
			ohnestop = 1;
		}
		if(rcret == getrcconfigint("rc5", NULL)) { 
			position = ostrcat("/decoder_control.cgi?command=", "39", 0 , 0);
			ohnestop = 1;
		}
		if(rcret == getrcconfigint("rc6", NULL)) {
			position = ostrcat("/decoder_control.cgi?command=", "41", 0 , 0);
			ohnestop = 1;
		}
		if(rcret == getrcconfigint("rc7", NULL)) {
			position = ostrcat("/decoder_control.cgi?command=", "43", 0 , 0);
			ohnestop = 1;
		}
		if(rcret == getrcconfigint("rc8", NULL)) {
			position = ostrcat("/decoder_control.cgi?command=", "45", 0 , 0);
			ohnestop = 1;
		}
		if(rcret == getrcconfigint("rc9", NULL)) {
			position = ostrcat("/decoder_control.cgi?command=", "47", 0 , 0);
			ohnestop = 1;
		}
		if(positionstop == NULL) 
		{
			if(rcret == getrcconfigint("rcup", NULL)) {
				position = ostrcat("/decoder_control.cgi?command=", "0", 0 , 0);
				positionstop = ostrcat("/decoder_control.cgi?command=", "1", 0 , 0);
			}
			if(rcret == getrcconfigint("rcdown", NULL)) {
				position = ostrcat("/decoder_control.cgi?command=", "2", 0 , 0);
				positionstop = ostrcat("/decoder_control.cgi?command=", "3", 0 , 0);
			}
			if(rcret == getrcconfigint("rcleft", NULL)) {
				position = ostrcat("/decoder_control.cgi?command=", "4", 0 , 0);
				positionstop = ostrcat("/decoder_control.cgi?command=", "5", 0 , 0);
			}
			if(rcret == getrcconfigint("rcright", NULL)) {
				position = ostrcat("/decoder_control.cgi?command=", "6", 0 , 0);
				positionstop = ostrcat("/decoder_control.cgi?command=", "7", 0 , 0);
			}
			if(rcret == getrcconfigint("rcchup", NULL)) {
				position = ostrcat("/decoder_control.cgi?command=", "16", 0 , 0);
				positionstop = ostrcat("/decoder_control.cgi?command=", "17", 0 , 0);
			}
			if(rcret == getrcconfigint("rcchdown", NULL)) {
				position = ostrcat("/decoder_control.cgi?command=", "18", 0 , 0);
				positionstop = ostrcat("/decoder_control.cgi?command=", "19", 0 , 0);
			}
		}
		
		if(tempcam != NULL && rcret != RCTIMEOUT)
		{
			if(positionstop != NULL && ohnestop == 1)
			{
				tmpstr = ostrcat(tempcam, positionstop, 0, 0);
				system(tmpstr);
				free(tmpstr); tmpstr = NULL;	
				free(positionstop); positionstop = NULL;
			}
			if(position != NULL)
			{
				tmpstr = ostrcat(tempcam, position, 0, 0);
				system(tmpstr);
				free(tmpstr); tmpstr = NULL;	
				free(position); position = NULL;
			}
			else if(positionstop != NULL)
			{
				tmpstr = ostrcat(tempcam, positionstop, 0, 0);
				system(tmpstr);
				free(tmpstr); tmpstr = NULL;	
				free(positionstop); positionstop = NULL;
			}
			if(rcret == getrcconfigint("rcred", NULL)) {
				if(actcam == 1 && cam2 != NULL) {
					actcam = 2;
					changepic(instar_pic1, "/tmp/instar2.jpg");
					changetext(instar_actcam1, "CAM2");
					tempcam = cam2;
					addlist(myconfig, "InstarActCam", "2");
				}
				else if(actcam == 2 && cam1 != NULL) {
					actcam = 1;
					changepic(instar_pic1, "/tmp/instar1.jpg");
					changetext(instar_actcam1, "CAM1");
					tempcam = cam1;
					addlist(myconfig, "InstarActCam", "1");
				}
			}
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
	struct skin* alterwebif = getscreennode(instar_einstellungen, "alterwebif");
	struct skin* autoCam = getscreennode(instar_einstellungen, "autoCam");
	struct skin* onCam1 = getscreennode(instar_einstellungen, "onCam1");
	struct skin* typCam1 = getscreennode(instar_einstellungen, "typCam1");
	struct skin* ipCam1 = getscreennode(instar_einstellungen, "ipCam1");
	struct skin* portCam1 = getscreennode(instar_einstellungen, "portCam1");
	struct skin* userCam1 = getscreennode(instar_einstellungen, "userCam1");
	struct skin* passCam1 = getscreennode(instar_einstellungen, "passCam1");
	struct skin* onCam2 = getscreennode(instar_einstellungen, "onCam2");
	struct skin* typCam2 = getscreennode(instar_einstellungen, "typCam2");
	struct skin* ipCam2 = getscreennode(instar_einstellungen, "ipCam2");
	struct skin* portCam2 = getscreennode(instar_einstellungen, "portCam2");
	struct skin* userCam2 = getscreennode(instar_einstellungen, "userCam2");
	struct skin* passCam2 = getscreennode(instar_einstellungen, "passCam2");
	
	struct skin* tmp = NULL;

  addchoicebox(autoCam, "nein", _("nein"));
	addchoicebox(autoCam, "ja", _("ja"));
	setchoiceboxselection(autoCam, getlist(myconfig, "InstarCamAutostart", NULL));
	
	addchoicebox(alterwebif, "0", "off");
	addchoicebox(alterwebif, "1", "on");
	setchoiceboxselection(alterwebif, getconfig("instar_alterwebif", NULL));
  
  addchoicebox(onCam1, "aus", _("aus"));
	addchoicebox(onCam1, "ein", _("ein"));
	setchoiceboxselection(onCam1, getlist(myconfig, "InstarCam1", NULL));	
	addchoicebox(typCam1, "0", "0");
	addchoicebox(typCam1, "1", "1");
	addchoicebox(typCam1, "2", "2");
	addchoicebox(typCam1, "3", "3");
	setchoiceboxselection(typCam1, getconfig("instar_1", NULL));
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
	addchoicebox(typCam2, "0", "0");
	addchoicebox(typCam2, "1", "1");
	addchoicebox(typCam2, "2", "2");
	addchoicebox(typCam2, "3", "3");
	setchoiceboxselection(typCam2, getconfig("instar_2", NULL));
	changemask(ipCam2, "abcdefghijklmnopqrstuvwxyz");
  changeinput(ipCam2, getlist(myconfig, "InstarCam2IP", NULL));
  changemask(portCam2, "0000");
  changeinput(portCam2, getlist(myconfig, "InstarCam2Port", NULL));
  changemask(userCam2, "abcdefghijklmnopqrstuvwxyz");
  changeinput(userCam2, getlist(myconfig, "InstarCam2User", NULL));
  changemask(passCam2, "abcdefghijklmnopqrstuvwxyz");
  changeinput(passCam2, getlist(myconfig, "InstarCam2Pass", NULL));
  
	drawscreen(instar_einstellungen, 0, 0);
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
			addconfig("instar_1", typCam1->ret);
			addconfig("instar_2", typCam2->ret);
			addconfig("instar_alterwebif", alterwebif->ret);
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
	
	instarconf = createpluginpath("/instar/instar.conf", 0);
	readconfig(instarconf, myconfig);

	char* CURL = NULL;
	CURL = createpluginpath("/instar/curl", 0);
	if(!file_exist(CURL))
	{
		free(CURL); CURL = NULL;
		CURL = ostrcat("curl", NULL, 0, 0);
	}
	
  if(ostrcmp(getlist(myconfig, "InstarCamAutostart", NULL), "ja") == 0)
		rcode = 1;
	else
		rcode = instar_settings();
	
	if(rcode == 1)
	{
		if(checkbox("ATEMIO5200") == 1)
			addconfig("pic_scale", "1");
		while(1)
		{
			if(ostrcmp(getlist(myconfig, "InstarCam1", NULL), "ein") == 0)
			{
				if(getconfigint("instar_alterwebif", NULL) == 0)
					cam1 = ostrcat("wget --output-document=/tmp/instar1.jpg http://", getlist(myconfig, "InstarCam1User", NULL), 0, 0);
				else
				{
					cam1 = ostrcat(CURL, " http://", 0, 0);
					cam1 = ostrcat(cam1, getlist(myconfig, "InstarCam1User", NULL), 1, 0);
				}
				cam1 = ostrcat(cam1, ":",1, 0);
				cam1 = ostrcat(cam1, getlist(myconfig, "InstarCam1Pass", NULL), 1, 0);
				cam1 = ostrcat(cam1, "@",1, 0);
				cam1 = ostrcat(cam1, getlist(myconfig, "InstarCam1IP", NULL), 1, 0);
				cam1 = ostrcat(cam1, ":",1, 0);
				cam1 = ostrcat(cam1, getlist(myconfig, "InstarCam1Port", NULL), 1, 0);
				
						
				//if(getconfigint("instar_1", NULL) == 2)
				//{
				//	cam11 = ostrcat("wget --output-document=/tmp/instar1.jpg http://",getlist(myconfig, "InstarCam1IP", NULL), 0, 0);
				//	cam11 = ostrcat(cam11, ":", 1, 0);
				//	cam11 = ostrcat(cam11, getlist(myconfig, "InstarCam1Port", NULL), 1, 0);
				//	cam11 = ostrcat(cam11, "/tmpfs/auto.jpg?usr=", 1, 0);
				//	cam11 = ostrcat(cam11, getlist(myconfig, "InstarCam1User", NULL), 1, 0);
				//	cam11 = ostrcat(cam11, "&pwd=", 1, 0);
				//	cam11 = ostrcat(cam11, getlist(myconfig, "InstarCam1Pass", NULL), 1, 0);
				//}
			}
			
		
			
			if(ostrcmp(getlist(myconfig, "InstarCam2", NULL), "ein") == 0)
			{
				if(getconfigint("instar_alterwebif", NULL) == 0)
					cam2 = ostrcat("wget --output-document=/tmp/instar2.jpg http://", getlist(myconfig, "InstarCam2User", NULL), 0, 0);
				else
				{
					cam2 = ostrcat(CURL, " http://", 0, 0);
					cam2 = ostrcat(cam2, getlist(myconfig, "InstarCam2User", NULL), 1, 0);
				}
				cam2 = ostrcat(cam2, ":",1, 0);
				cam2 = ostrcat(cam2, getlist(myconfig, "InstarCam2Pass", NULL), 1, 0);
				cam2 = ostrcat(cam2, "@",1, 0);
				cam2 = ostrcat(cam2, getlist(myconfig, "InstarCam2IP", NULL), 1, 0);
				cam2 = ostrcat(cam2, ":",1, 0);
				cam2 = ostrcat(cam2, getlist(myconfig, "InstarCam2Port", NULL), 1, 0);
			}
					
			instar_screen = getscreen("instar_full");
			instar_pic1 = getscreennode(instar_screen, "pic1");
			instar_actcam1 = getscreennode(instar_screen, "actcam1");
	
			if(ostrcmp(getlist(myconfig, "InstarActCam", NULL), "2") == 0)
			{
				actcam = 2;
				changepic(instar_pic1, "/tmp/instar2.jpg");
				changetext(instar_actcam1, "CAM2");
			}
			else
			{
				actcam = 1;
				changepic(instar_pic1, "/tmp/instar1.jpg");
				changetext(instar_actcam1, "CAM1");
			}
			
			rcode = show_control();
	
			free(cam1), cam1 = NULL;
			free(cam2), cam2 = NULL;
			//free(cam11), cam11 = NULL;
			//free(cam21), cam21 = NULL;
			
			if(rcode == 0)
				break;
			rcode = 0;
			
			rcode = instar_settings();
			if(rcode == 0)
				break;
		}
		if(checkbox("ATEMIO5200") == 1)
			addconfig("pic_scale", "0");
	}
	writelist(myconfig, instarconf);
	freelist(myconfig);
	free(instarconf); instarconf = NULL;
	free(CURL); CURL = NULL;
}
