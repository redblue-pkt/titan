//#ifndef INSTAR_H
//#define INSTAR_H

char* cam1 = NULL;
char* cam2 = NULL;
char* cam11 = NULL;
char* cam21 = NULL;

char* cam3 = NULL;
char* cam4 = NULL;
char* cam31 = NULL;
char* cam41 = NULL;

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
	char* tempcam1 = NULL;
	char* position = NULL;
	char* positionstop = NULL;
	int rcret = 0;
	int rcode = 0;
	int ohnestop = 0;
	
	if(actcam == 1)
	{
		tempcam = cam1;
		tempcam1 = cam11;
	}
	if(actcam == 2)
	{
		tempcam = cam2;
		tempcam1 = cam21;
	}
	if(actcam == 3)
	{
		tempcam = cam3;
		tempcam1 = cam31;
	}
	if(actcam == 4)
	{
		tempcam = cam4;
		tempcam1 = cam41;
	}
	
	setfbtransparent(255);
	
	printf("[ip-cam] Aufruf1: %s\n", cam1);
	printf("[ip-cam] Aufruf2: %s\n", cam2);
	
	while(1)
	{
			if(cam1 != NULL)
			{
				if(getconfigint("instar_1", NULL) == 1)
					tmpstr = ostrcat(cam1, "/image/jpeg.cgi", 0, 0);
				else if(getconfigint("instar_1", NULL) == 2)
					tmpstr = ostrcat(cam1, "/tmpfs/auto.jpg", 0, 0);
				else if(getconfigint("instar_1", NULL) == 3)
					tmpstr = ostrcat(cam1, "/onvif/GetSnapshotUri", 0, 0);
				else if(getconfigint("instar_1", NULL) == 4)
					tmpstr = ostrcat(cam1, "/image", 0, 0);
				else if(getconfigint("instar_1", NULL) == 5)
				{
					tmpstr = ostrcat(cam1, "/cgi-bin/CGIProxy.fcgi?cmd%3DsnapPicture2", 0, 0);
					tmpstr = ostrcat(tmpstr, cam11, 1, 0);
				}
				else if(getconfigint("instar_1", NULL) == 6)
				{
					tmpstr = ostrcat(cam1, "/cgi-bin/images_cgi?channel=0", 0, 0);
					tmpstr = ostrcat(tmpstr, cam11, 1, 0);
				}
				else if(getconfigint("instar_1", NULL) == 7)
				{
					tmpstr = ostrcat(cam1, "/snapshot.jpg", 0, 0);
					tmpstr = ostrcat(tmpstr, cam11, 1, 0);
				}
				else
					tmpstr = ostrcat(cam1, "/snapshot.cgi", 0, 0);
				if(getconfigint("instar_alterwebif", NULL) == 1)
					tmpstr = ostrcat(tmpstr, " > /tmp/instar1.jpg", 1, 0);
				system(tmpstr);
				printf("-> %s\n", tmpstr);
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
				else if(getconfigint("instar_2", NULL) == 4)
					tmpstr = ostrcat(cam2, "/image", 0, 0);
				else if(getconfigint("instar_2", NULL) == 5)
				{
					tmpstr = ostrcat(cam2, "/cgi-bin/CGIProxy.fcgi?cmd%3DsnapPicture2", 0, 0);
					tmpstr = ostrcat(tmpstr, cam21, 1, 0);
				}
				else if(getconfigint("instar_2", NULL) == 6)
				{
					tmpstr = ostrcat(cam2, "/cgi-bin/images_cgi?channel=0", 0, 0);
					tmpstr = ostrcat(tmpstr, cam21, 1, 0);
				}
				else if(getconfigint("instar_2", NULL) == 7)
				{
					tmpstr = ostrcat(cam2, "/snapshot.jpg", 0, 0);
					tmpstr = ostrcat(tmpstr, cam21, 1, 0);
				}
				else
					tmpstr = ostrcat(cam2, "/snapshot.cgi", 0, 0);
				if(getconfigint("instar_alterwebif", NULL) == 1)
					tmpstr = ostrcat(tmpstr, " > /tmp/instar2.jpg", 1, 0);
				system(tmpstr);
				printf("-> %s\n", tmpstr);
				free(tmpstr); tmpstr = NULL;
			}
			if(cam3 != NULL)
			{
				if(getconfigint("instar_3", NULL) == 1)
					tmpstr = ostrcat(cam3, "/image/jpeg.cgi", 0, 0);
				else if(getconfigint("instar_3", NULL) == 2)
					tmpstr = ostrcat(cam3, "/tmpfs/auto.jpg", 0, 0);
				else if(getconfigint("instar_3", NULL) == 3)
					tmpstr = ostrcat(cam3, "/onvif/GetSnapshotUri", 0, 0);
				else if(getconfigint("instar_3", NULL) == 4)
					tmpstr = ostrcat(cam3, "/image", 0, 0);
				else if(getconfigint("instar_3", NULL) == 5)
				{
					tmpstr = ostrcat(cam3, "/cgi-bin/CGIProxy.fcgi?cmd%3DsnapPicture2", 0, 0);
					tmpstr = ostrcat(tmpstr, cam31, 1, 0);
				}
				else if(getconfigint("instar_3", NULL) == 6)
				{
					tmpstr = ostrcat(cam3, "/cgi-bin/images_cgi?channel=0", 0, 0);
					tmpstr = ostrcat(tmpstr, cam31, 1, 0);
				}
				else if(getconfigint("instar_3", NULL) == 7)
				{
					tmpstr = ostrcat(cam3, "/snapshot.jpg", 0, 0);
					tmpstr = ostrcat(tmpstr, cam31, 1, 0);
				}
				else
					tmpstr = ostrcat(cam3, "/snapshot.cgi", 0, 0);
				if(getconfigint("instar_alterwebif", NULL) == 1)
					tmpstr = ostrcat(tmpstr, " > /tmp/instar3.jpg", 1, 0);
				system(tmpstr);
				printf("-> %s\n", tmpstr);
				free(tmpstr); tmpstr = NULL;
			}
			if(cam4 != NULL)
			{
				if(getconfigint("instar_4", NULL) == 1)
					tmpstr = ostrcat(cam4, "/image/jpeg.cgi", 0, 0);
				else if(getconfigint("instar_4", NULL) == 2)
					tmpstr = ostrcat(cam4, "/tmpfs/auto.jpg", 0, 0);
				else if(getconfigint("instar_4", NULL) == 3)
					tmpstr = ostrcat(cam4, "/onvif/GetSnapshotUri", 0, 0);
				else if(getconfigint("instar_4", NULL) == 4)
					tmpstr = ostrcat(cam4, "/image", 0, 0);
				else if(getconfigint("instar_4", NULL) == 5)
				{
					tmpstr = ostrcat(cam4, "/cgi-bin/CGIProxy.fcgi?cmd%3DsnapPicture2", 0, 0);
					tmpstr = ostrcat(tmpstr, cam41, 1, 0);
				}
				else if(getconfigint("instar_4", NULL) == 6)
				{
					tmpstr = ostrcat(cam4, "/cgi-bin/images_cgi?channel=0", 0, 0);
					tmpstr = ostrcat(tmpstr, cam41, 1, 0);
				}
				else if(getconfigint("instar_4", NULL) == 7)
				{
					tmpstr = ostrcat(cam4, "/snapshot.jpg", 0, 0);
					tmpstr = ostrcat(tmpstr, cam41, 1, 0);
				}
				else
					tmpstr = ostrcat(cam4, "/snapshot.cgi", 0, 0);
				if(getconfigint("instar_alterwebif", NULL) == 1)
					tmpstr = ostrcat(tmpstr, " > /tmp/instar4.jpg", 1, 0);
				system(tmpstr);
				printf("-> %s\n", tmpstr);
				free(tmpstr); tmpstr = NULL;
			}
				
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
			if((actcam == 1 && getconfigint("instar_1", NULL) == 2) || (actcam == 2 && getconfigint("instar_2", NULL) == 2) || (actcam == 3 && getconfigint("instar_3", NULL) == 2)  || (actcam == 4 && getconfigint("instar_4", NULL) == 2))
			{
				position = ostrcat("/cgi-bin/hi3510/preset.cgi?-act%3Dgoto%26-status%3D1%26-number%3D", "0", 0 , 0);
				ohnestop = 1;			
			}
			else
			{
				position = ostrcat("/decoder_control.cgi?command=", "31", 0 , 0);
				ohnestop = 1;
			}
		}
		if(rcret == getrcconfigint("rc2", NULL)) {
			if((actcam == 1 && getconfigint("instar_1", NULL) == 2) || (actcam == 2 && getconfigint("instar_2", NULL) == 2) || (actcam == 3 && getconfigint("instar_3", NULL) == 2)  || (actcam == 4 && getconfigint("instar_4", NULL) == 2))
			{
				position = ostrcat("/cgi-bin/hi3510/preset.cgi?-act%3Dgoto%26-status%3D1%26-number%3D", "1", 0 , 0);
				ohnestop = 1;			
			}
			else
			{
				position = ostrcat("/decoder_control.cgi?command=", "33", 0 , 0);
				ohnestop = 1;
			}
		}
		if(rcret == getrcconfigint("rc3", NULL)) {
			if((actcam == 1 && getconfigint("instar_1", NULL) == 2) || (actcam == 2 && getconfigint("instar_2", NULL) == 2) || (actcam == 3 && getconfigint("instar_3", NULL) == 2)  || (actcam == 4 && getconfigint("instar_4", NULL) == 2))
			{
				position = ostrcat("/cgi-bin/hi3510/preset.cgi?-act%3Dgoto%26-status%3D1%26-number%3D", "2", 0 , 0);
				ohnestop = 1;			
			}
			else
			{
				position = ostrcat("/decoder_control.cgi?command=", "35", 0 , 0);
				ohnestop = 1;
			}
		}
		if(rcret == getrcconfigint("rc4", NULL)) {
			if((actcam == 1 && getconfigint("instar_1", NULL) == 2) || (actcam == 2 && getconfigint("instar_2", NULL) == 2) || (actcam == 3 && getconfigint("instar_3", NULL) == 2)  || (actcam == 4 && getconfigint("instar_4", NULL) == 2))
			{
				position = ostrcat("/cgi-bin/hi3510/preset.cgi?-act%3Dgoto%26-status%3D1%26-number%3D", "3", 0 , 0);
				ohnestop = 1;			
			}
			else
			{
				position = ostrcat("/decoder_control.cgi?command=", "37", 0 , 0);
				ohnestop = 1;
			}
		}
		if(rcret == getrcconfigint("rc5", NULL)) { 
			if((actcam == 1 && getconfigint("instar_1", NULL) == 2) || (actcam == 2 && getconfigint("instar_2", NULL) == 2) || (actcam == 3 && getconfigint("instar_3", NULL) == 2)  || (actcam == 4 && getconfigint("instar_4", NULL) == 2))
			{
				position = ostrcat("/cgi-bin/hi3510/preset.cgi?-act%3Dgoto%26-status%3D1%26-number%3D", "4", 0 , 0);
				ohnestop = 1;			
			}
			else
			{
				position = ostrcat("/decoder_control.cgi?command=", "39", 0 , 0);
				ohnestop = 1;
			}
		}
		if(rcret == getrcconfigint("rc6", NULL)) {
			if((actcam == 1 && getconfigint("instar_1", NULL) == 2) || (actcam == 2 && getconfigint("instar_2", NULL) == 2) || (actcam == 3 && getconfigint("instar_3", NULL) == 2)  || (actcam == 4 && getconfigint("instar_4", NULL) == 2))
			{
				position = ostrcat("/cgi-bin/hi3510/preset.cgi?-act%3Dgoto%26-status%3D1%26-number%3D", "5", 0 , 0);
				ohnestop = 1;			
			}
			else
			{
				position = ostrcat("/decoder_control.cgi?command=", "41", 0 , 0);
				ohnestop = 1;
			}
		}
		if(rcret == getrcconfigint("rc7", NULL)) {
			if((actcam == 1 && getconfigint("instar_1", NULL) == 2) || (actcam == 2 && getconfigint("instar_2", NULL) == 2) || (actcam == 3 && getconfigint("instar_3", NULL) == 2)  || (actcam == 4 && getconfigint("instar_4", NULL) == 2))
			{
				position = ostrcat("/cgi-bin/hi3510/preset.cgi?-act%3Dgoto%26-status%3D1%26-number%3D", "6", 0 , 0);
				ohnestop = 1;			
			}
			else
			{
				position = ostrcat("/decoder_control.cgi?command=", "43", 0 , 0);
				ohnestop = 1;
			}
		}
		if(rcret == getrcconfigint("rc8", NULL)) {
			if((actcam == 1 && getconfigint("instar_1", NULL) == 2) || (actcam == 2 && getconfigint("instar_2", NULL) == 2) || (actcam == 3 && getconfigint("instar_3", NULL) == 2)  || (actcam == 4 && getconfigint("instar_4", NULL) == 2))
			{
				position = ostrcat("/cgi-bin/hi3510/preset.cgi?-act%3Dgoto%26-status%3D1%26-number%3D", "7", 0 , 0);
				ohnestop = 1;			
			}
			else
			{
				position = ostrcat("/decoder_control.cgi?command=", "45", 0 , 0);
				ohnestop = 1;
			}
		}
		if(rcret == getrcconfigint("rc9", NULL)) {
			if((actcam == 1 && getconfigint("instar_1", NULL) == 2) || (actcam == 2 && getconfigint("instar_2", NULL) == 2) || (actcam == 3 && getconfigint("instar_3", NULL) == 2)  || (actcam == 4 && getconfigint("instar_4", NULL) == 2))
			{
				position = ostrcat("/cgi-bin/hi3510/preset.cgi?-act%3Dgoto%26-status%3D1%26-number%3D", "8", 0 , 0);
				ohnestop = 1;			
			}
			else
			{
				position = ostrcat("/decoder_control.cgi?command=", "47", 0 , 0);
				ohnestop = 1;
			}
		}
		if(positionstop == NULL) 
		{
			if(rcret == getrcconfigint("rcup", NULL)) {
				if((actcam == 1 && getconfigint("instar_1", NULL) == 2) || (actcam == 2 && getconfigint("instar_2", NULL) == 2) || (actcam == 3 && getconfigint("instar_3", NULL) == 2)  || (actcam == 4 && getconfigint("instar_4", NULL) == 2))
				{
					position = ostrcat("/cgi-bin/hi3510/ptzctrl.cgi?-step%3D0%26-act%3D", "up", 0 , 0);
					positionstop = ostrcat("/cgi-bin/hi3510/ptzctrl.cgi?-step%3D0%26-act%3D", "stop", 0 , 0);
				}
				else
				{
					position = ostrcat("/decoder_control.cgi?command=", "0", 0 , 0);
					positionstop = ostrcat("/decoder_control.cgi?command=", "1", 0 , 0);
				}
			}
			if(rcret == getrcconfigint("rcdown", NULL)) {
				if((actcam == 1 && getconfigint("instar_1", NULL) == 2) || (actcam == 2 && getconfigint("instar_2", NULL) == 2) || (actcam == 3 && getconfigint("instar_3", NULL) == 2)  || (actcam == 4 && getconfigint("instar_4", NULL) == 2))
				{
					position = ostrcat("/cgi-bin/hi3510/ptzctrl.cgi?-step%3D0%26-act%3D", "down", 0 , 0);
					positionstop = ostrcat("/cgi-bin/hi3510/ptzctrl.cgi?-step%3D0%26-act%3D", "stop", 0 , 0);
				}
				else
				{
					position = ostrcat("/decoder_control.cgi?command=", "2", 0 , 0);
					positionstop = ostrcat("/decoder_control.cgi?command=", "3", 0 , 0);
				}
			}
			if(rcret == getrcconfigint("rcleft", NULL)) {
				if((actcam == 1 && getconfigint("instar_1", NULL) == 2) || (actcam == 2 && getconfigint("instar_2", NULL) == 2) || (actcam == 3 && getconfigint("instar_3", NULL) == 2)  || (actcam == 4 && getconfigint("instar_4", NULL) == 2))
				{
					position = ostrcat("/cgi-bin/hi3510/ptzctrl.cgi?-step%3D0%26-act%3D", "left", 0 , 0);
					positionstop = ostrcat("/cgi-bin/hi3510/ptzctrl.cgi?-step%3D0%26-act%3D", "stop", 0 , 0);
				}
				else
				{
					position = ostrcat("/decoder_control.cgi?command=", "4", 0 , 0);
					positionstop = ostrcat("/decoder_control.cgi?command=", "5", 0 , 0);
				}
			}
			if(rcret == getrcconfigint("rcright", NULL)) {
				if((actcam == 1 && getconfigint("instar_1", NULL) == 2) || (actcam == 2 && getconfigint("instar_2", NULL) == 2) || (actcam == 3 && getconfigint("instar_3", NULL) == 2)  || (actcam == 4 && getconfigint("instar_4", NULL) == 2))
				{
					position = ostrcat("/cgi-bin/hi3510/ptzctrl.cgi?-step%3D0%26-act%3D", "right", 0 , 0);
					positionstop = ostrcat("/cgi-bin/hi3510/ptzctrl.cgi?-step%3D0%26-act%3D", "stop", 0 , 0);
				}
				else
				{
					position = ostrcat("/decoder_control.cgi?command=", "6", 0 , 0);
					positionstop = ostrcat("/decoder_control.cgi?command=", "7", 0 , 0);
				}
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
				if(tempcam1 != NULL)
					tmpstr = ostrcat(tmpstr, tempcam1, 1, 0);
				system(tmpstr);
				free(tmpstr); tmpstr = NULL;	
				free(positionstop); positionstop = NULL;
			}
			if(position != NULL)
			{
				tmpstr = ostrcat(tempcam, position, 0, 0);
				if(tempcam1 != NULL)
					tmpstr = ostrcat(tmpstr, tempcam1, 1, 0);
				system(tmpstr);
				free(tmpstr); tmpstr = NULL;	
				free(position); position = NULL;
			}
			else if(positionstop != NULL)
			{
				tmpstr = ostrcat(tempcam, positionstop, 0, 0);
				if(tempcam1 != NULL)
					tmpstr = ostrcat(tmpstr, tempcam1, 1, 0);
				system(tmpstr);
				free(tmpstr); tmpstr = NULL;	
				free(positionstop); positionstop = NULL;
			}
			if(rcret == getrcconfigint("rcred", NULL)) {
				if(actcam == 4 && cam1 != NULL)
				{
					actcam = 1;
					changepic(instar_pic1, "/tmp/instar1.jpg");
					changetext(instar_actcam1, "CAM1");
					tempcam = cam1;
					addlist(myconfig, "InstarActCam", "1");
				}
				else if(actcam == 1 && cam2 != NULL)
				{
					actcam = 2;
					changepic(instar_pic1, "/tmp/instar2.jpg");
					changetext(instar_actcam1, "CAM2");
					tempcam = cam2;
					addlist(myconfig, "InstarActCam", "2");
				}				
				else if(actcam == 2 && cam3 != NULL)
				{
					actcam = 3;
					changepic(instar_pic1, "/tmp/instar3.jpg");
					changetext(instar_actcam1, "CAM3");
					tempcam = cam3;
					addlist(myconfig, "InstarActCam", "3");
				}
				else if(actcam == 3 && cam4 != NULL)
				{
					actcam = 4;
					changepic(instar_pic1, "/tmp/instar4.jpg");
					changetext(instar_actcam1, "CAM4");
					tempcam = cam4;
					addlist(myconfig, "InstarActCam", "4");
				}
				else
				{
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
	struct skin* onCam3 = getscreennode(instar_einstellungen, "onCam3");
	struct skin* typCam3 = getscreennode(instar_einstellungen, "typCam3");
	struct skin* ipCam3 = getscreennode(instar_einstellungen, "ipCam3");
	struct skin* portCam3 = getscreennode(instar_einstellungen, "portCam3");
	struct skin* userCam3 = getscreennode(instar_einstellungen, "userCam3");
	struct skin* passCam3 = getscreennode(instar_einstellungen, "passCam3");
	struct skin* onCam4 = getscreennode(instar_einstellungen, "onCam4");
	struct skin* typCam4 = getscreennode(instar_einstellungen, "typCam4");
	struct skin* ipCam4 = getscreennode(instar_einstellungen, "ipCam4");
	struct skin* portCam4 = getscreennode(instar_einstellungen, "portCam4");
	struct skin* userCam4 = getscreennode(instar_einstellungen, "userCam4");
	struct skin* passCam4 = getscreennode(instar_einstellungen, "passCam4");
	
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
	addchoicebox(typCam1, "4", "4");
	addchoicebox(typCam1, "5", "5");
	addchoicebox(typCam1, "6", "6");
	addchoicebox(typCam1, "7", "7");
	
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
	addchoicebox(typCam2, "4", "4");
	addchoicebox(typCam2, "5", "5");
	addchoicebox(typCam2, "6", "6");
	addchoicebox(typCam2, "7", "7");
	setchoiceboxselection(typCam2, getconfig("instar_2", NULL));
	changemask(ipCam2, "abcdefghijklmnopqrstuvwxyz");
  changeinput(ipCam2, getlist(myconfig, "InstarCam2IP", NULL));
  changemask(portCam2, "0000");
  changeinput(portCam2, getlist(myconfig, "InstarCam2Port", NULL));
  changemask(userCam2, "abcdefghijklmnopqrstuvwxyz");
  changeinput(userCam2, getlist(myconfig, "InstarCam2User", NULL));
  changemask(passCam2, "abcdefghijklmnopqrstuvwxyz");
  changeinput(passCam2, getlist(myconfig, "InstarCam2Pass", NULL));
  
  addchoicebox(onCam3, "aus", _("aus"));
	addchoicebox(onCam3, "ein", _("ein"));
	setchoiceboxselection(onCam3, getlist(myconfig, "InstarCam3", NULL));	
	addchoicebox(typCam3, "0", "0");
	addchoicebox(typCam3, "1", "1");
	addchoicebox(typCam3, "2", "2");
	addchoicebox(typCam3, "3", "3");
	addchoicebox(typCam3, "4", "4");
	addchoicebox(typCam3, "5", "5");
	addchoicebox(typCam3, "6", "6");
	addchoicebox(typCam3, "7", "7");
	setchoiceboxselection(typCam3, getconfig("instar_3", NULL));
	changemask(ipCam3, "abcdefghijklmnopqrstuvwxyz");
  changeinput(ipCam3, getlist(myconfig, "InstarCam3IP", NULL));
  changemask(portCam3, "0000");
  changeinput(portCam3, getlist(myconfig, "InstarCam3Port", NULL));
  changemask(userCam3, "abcdefghijklmnopqrstuvwxyz");
  changeinput(userCam3, getlist(myconfig, "InstarCam3User", NULL));
  changemask(passCam3, "abcdefghijklmnopqrstuvwxyz");
  changeinput(passCam3, getlist(myconfig, "InstarCam3Pass", NULL));
  
  addchoicebox(onCam4, "aus", _("aus"));
	addchoicebox(onCam4, "ein", _("ein"));
	setchoiceboxselection(onCam4, getlist(myconfig, "InstarCam4", NULL));	
	addchoicebox(typCam4, "0", "0");
	addchoicebox(typCam4, "1", "1");
	addchoicebox(typCam4, "2", "2");
	addchoicebox(typCam4, "3", "3");
	addchoicebox(typCam4, "4", "4");
	addchoicebox(typCam4, "5", "5");
	addchoicebox(typCam4, "6", "6");
	addchoicebox(typCam4, "7", "7");
	setchoiceboxselection(typCam4, getconfig("instar_4", NULL));
	changemask(ipCam4, "abcdefghijklmnopqrstuvwxyz");
  changeinput(ipCam4, getlist(myconfig, "InstarCam4IP", NULL));
  changemask(portCam4, "0000");
  changeinput(portCam4, getlist(myconfig, "InstarCam4Port", NULL));
  changemask(userCam4, "abcdefghijklmnopqrstuvwxyz");
  changeinput(userCam4, getlist(myconfig, "InstarCam4User", NULL));
  changemask(passCam4, "abcdefghijklmnopqrstuvwxyz");
  changeinput(passCam4, getlist(myconfig, "InstarCam4Pass", NULL));
  
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
			addlist(myconfig, "InstarCam3", onCam3->ret);
			addlist(myconfig, "InstarCam3IP", ipCam3->ret);
			addlist(myconfig, "InstarCam3Port", portCam3->ret);
			addlist(myconfig, "InstarCam3User", userCam3->ret);
			addlist(myconfig, "InstarCam3Pass", passCam3->ret);
			addlist(myconfig, "InstarCam4", onCam4->ret);
			addlist(myconfig, "InstarCam4IP", ipCam4->ret);
			addlist(myconfig, "InstarCam4Port", portCam4->ret);
			addlist(myconfig, "InstarCam4User", userCam4->ret);
			addlist(myconfig, "InstarCam4Pass", passCam4->ret);
			writelist(myconfig, instarconf);
			addconfig("instar_1", typCam1->ret);
			addconfig("instar_2", typCam2->ret);
			addconfig("instar_3", typCam3->ret);
			addconfig("instar_4", typCam4->ret);
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
	int ext = 0;
	
	instarconf = createpluginpath("/instar/instar.conf", 0);
	readconfig(instarconf, myconfig);

	char* CURL = NULL;
	CURL = createpluginpath("/instar/curl", 0);
	if(!file_exist(CURL))
	{
		free(CURL); CURL = NULL;
		CURL = ostrcat("curl", NULL, 0, 0);
	}

	if(file_exist("/tmp/callcam1"))
	{
		ext = 1;
		system("rm /callcam*");
	}
  else if(file_exist("/tmp/callcam2"))
	{
		ext = 2;
		system("rm /callcam*");
	}
  else
  	ext = 0;
  
  if(ext > 0)
  	rcode = 1;
  else if(ostrcmp(getlist(myconfig, "InstarCamAutostart", NULL), "ja") == 0)
		rcode = 1;
	else
		rcode = instar_settings();
	
	if(rcode == 1)
	{
		if(checkbox("ATEMIO5200") == 1)
			addconfig("pic_scale", "1");
		while(1)
		{
			if(ostrcmp(getlist(myconfig, "InstarCam1", NULL), "ein") == 0 || ext == 1)
			{
				if(getconfigint("instar_1", NULL) != 5)
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
				}
				else
				{
					if(getconfigint("instar_alterwebif", NULL) == 0)
						cam1 = ostrcat("wget --output-document=/tmp/instar1.jpg http://", NULL, 0, 0);
					else
					{
						cam1 = ostrcat(CURL, " http://", 0, 0);
					}
					cam1 = ostrcat(cam1, getlist(myconfig, "InstarCam1IP", NULL), 1, 0);
					cam1 = ostrcat(cam1, ":",1, 0);
					cam1 = ostrcat(cam1, getlist(myconfig, "InstarCam1Port", NULL), 1, 0);
					cam11 = ostrcat("%26usr%3D", getlist(myconfig, "InstarCam1User", NULL), 0, 0);
					cam11 = ostrcat(cam11, "%26pwd%3D", 1, 0);
					cam11 = ostrcat(cam11, getlist(myconfig, "InstarCam1Pass", NULL), 1, 0);
					//cam11 = ostrcat(cam11, "&", 1, 0);
					
				}
			}
			
		
			
			if(ostrcmp(getlist(myconfig, "InstarCam2", NULL), "ein") == 0 || ext == 2)
			{
				if(getconfigint("instar_2", NULL) != 5)
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
				else
				{
					if(getconfigint("instar_alterwebif", NULL) == 0)
						cam2 = ostrcat("wget --output-document=/tmp/instar2.jpg http://", NULL, 0, 0);
					else
					{
						cam2 = ostrcat(CURL, " http://", 0, 0);
					}
					cam2 = ostrcat(cam2, getlist(myconfig, "InstarCam2IP", NULL), 1, 0);
					cam2 = ostrcat(cam2, ":",1, 0);
					cam2 = ostrcat(cam2, getlist(myconfig, "InstarCam2Port", NULL), 1, 0);
					cam21 = ostrcat("%26usr%3D", getlist(myconfig, "InstarCam2User", NULL), 0, 0);
					cam21 = ostrcat(cam21, "%26pwd%3D", 1, 0);
					cam21 = ostrcat(cam21, getlist(myconfig, "InstarCam2Pass", NULL), 1, 0);
					//cam21 = ostrcat(cam21, "&", 1, 0);
				}				
			}
			
			if(ostrcmp(getlist(myconfig, "InstarCam3", NULL), "ein") == 0)
			{
				if(getconfigint("instar_3", NULL) != 5)
				{				
					if(getconfigint("instar_alterwebif", NULL) == 0)
						cam3 = ostrcat("wget --output-document=/tmp/instar3.jpg http://", getlist(myconfig, "Instarcam3User", NULL), 0, 0);
					else
					{
						cam3 = ostrcat(CURL, " http://", 0, 0);
						cam3 = ostrcat(cam3, getlist(myconfig, "InstarCam3User", NULL), 1, 0);
					}
					cam3 = ostrcat(cam3, ":",1, 0);
					cam3 = ostrcat(cam3, getlist(myconfig, "InstarCam3Pass", NULL), 1, 0);
					cam3 = ostrcat(cam3, "@",1, 0);
					cam3 = ostrcat(cam3, getlist(myconfig, "InstarCam3IP", NULL), 1, 0);
					cam3 = ostrcat(cam3, ":",1, 0);
					cam3 = ostrcat(cam3, getlist(myconfig, "InstarCam3Port", NULL), 1, 0);
				}
				else
				{
					if(getconfigint("instar_alterwebif", NULL) == 0)
						cam3 = ostrcat("wget --output-document=/tmp/instar3.jpg http://", NULL, 0, 0);
					else
					{
						cam3 = ostrcat(CURL, " http://", 0, 0);
					}
					cam3 = ostrcat(cam3, getlist(myconfig, "InstarCam3IP", NULL), 1, 0);
					cam3 = ostrcat(cam3, ":",1, 0);
					cam3 = ostrcat(cam3, getlist(myconfig, "InstarCam3Port", NULL), 1, 0);
					cam31 = ostrcat("%26usr%3D", getlist(myconfig, "InstarCam3User", NULL), 0, 0);
					cam31 = ostrcat(cam31, "%26pwd%3D", 1, 0);
					cam31 = ostrcat(cam31, getlist(myconfig, "InstarCam3Pass", NULL), 1, 0);
					//cam31 = ostrcat(cam31, "&", 1, 0);
				}				
			}
			
			if(ostrcmp(getlist(myconfig, "InstarCam4", NULL), "ein") == 0)
			{
				if(getconfigint("instar_4", NULL) != 5)
				{				
					if(getconfigint("instar_alterwebif", NULL) == 0)
						cam4 = ostrcat("wget --output-document=/tmp/instar4.jpg http://", getlist(myconfig, "InstarCam3User", NULL), 0, 0);
					else
					{
						cam4 = ostrcat(CURL, " http://", 0, 0);
						cam4 = ostrcat(cam4, getlist(myconfig, "InstarCam4User", NULL), 1, 0);
					}
					cam4 = ostrcat(cam4, ":",1, 0);
					cam4 = ostrcat(cam4, getlist(myconfig, "InstarCam4Pass", NULL), 1, 0);
					cam4 = ostrcat(cam4, "@",1, 0);
					cam4 = ostrcat(cam4, getlist(myconfig, "InstarCam4IP", NULL), 1, 0);
					cam4 = ostrcat(cam4, ":",1, 0);
					cam4 = ostrcat(cam4, getlist(myconfig, "InstarCam4Port", NULL), 1, 0);
				}
				else
				{
					if(getconfigint("instar_alterwebif", NULL) == 0)
						cam4 = ostrcat("wget --output-document=/tmp/instar4.jpg http://", NULL, 0, 0);
					else
					{
						cam4 = ostrcat(CURL, " http://", 0, 0);
					}
					cam4 = ostrcat(cam4, getlist(myconfig, "InstarCam4IP", NULL), 1, 0);
					cam4 = ostrcat(cam4, ":",1, 0);
					cam4 = ostrcat(cam4, getlist(myconfig, "InstarCam4Port", NULL), 1, 0);
					cam41 = ostrcat("%26usr%3D", getlist(myconfig, "InstarCam4User", NULL), 0, 0);
					cam41 = ostrcat(cam41, "%26pwd%3D", 1, 0);
					cam41 = ostrcat(cam41, getlist(myconfig, "InstarCam4Pass", NULL), 1, 0);
					//cam41 = ostrcat(cam41, "&", 1, 0);
				}				
			}						
					
			instar_screen = getscreen("instar_full");
			instar_pic1 = getscreennode(instar_screen, "pic1");
			instar_actcam1 = getscreennode(instar_screen, "actcam1");
	
			
			if(ext == 1 || ostrcmp(getlist(myconfig, "InstarActCam", NULL), "1") == 0)
			{
				actcam = 1;
				changepic(instar_pic1, "/tmp/instar1.jpg");
				changetext(instar_actcam1, "CAM1");
			}		
			else if(ext == 2 || ostrcmp(getlist(myconfig, "InstarActCam", NULL), "2") == 0)
			{
				actcam = 2;
				changepic(instar_pic1, "/tmp/instar2.jpg");
				changetext(instar_actcam1, "CAM2");
			}
			else if(ostrcmp(getlist(myconfig, "InstarActCam", NULL), "3") == 0)
			{
				actcam = 3;
				changepic(instar_pic1, "/tmp/instar3.jpg");
				changetext(instar_actcam1, "CAM3");
			}
			else if(ostrcmp(getlist(myconfig, "InstarActCam", NULL), "4") == 0)
			{
				actcam = 4;
				changepic(instar_pic1, "/tmp/instar4.jpg");
				changetext(instar_actcam1, "CAM4");
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
			free(cam11), cam11 = NULL;
			free(cam21), cam21 = NULL;
			free(cam3), cam3 = NULL;
			free(cam4), cam4 = NULL;
			free(cam31), cam31 = NULL;
			free(cam41), cam41 = NULL;
			
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
