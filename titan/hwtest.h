#ifndef HWTEST_H
#define HWTEST_H

//DVB-S2
#define TEST_S2_ORBITALPOS 0
#define TEST_S2_FREQ 11303000
#define TEST_S2_SYMBOL 22000000
#define TEST_S2_POL 0
#define TEST_S2_VIDEO_PID 3583
#define TEST_S2_AUDIO_PID 3587
#define TEST_S2_PCR_PID 3583

//test_channel.videoType = V_H264; // 1;
//test_channel.audioType = A_AC3; // 6;

//DVB-T 
#define TEST_T2_FREQ  4820
#define TEST_T2_BW   8
#define TEST_T2_VIDEO_PID 545
#define TEST_T2_AUDIO_PID 546
#define TEST_T2_PCR_PID 545

//test_channel.videoType = V_MP2; //0;
//test_channel.audioType = A_MP1; // 0;

//DVB-C
#define TEST_C2_FREQ   4500
#define TEST_C2_SYMBOL   6900
#define TEST_C2_VIDEO_PID  201
#define TEST_C2_AUDIO_PID  301
#define TEST_C2_PCR_PID  201
//#define TEST_C2_CAB_QAM     _QAMMODE_64

//test_channel.videoType = V_MP2; //0;
//test_channel.audioType = A_MP1; // 0;

void testzap(char* testtransponder, char* testchannel)
{
	uint64_t tpid = 99;
	uint8_t fetype = 0, polarization = 0, modulation = 0, fec = 0, pilot = 0, rolloff = 0, inversion = 0, system = 0;
	int orbitalpos = 0;
	unsigned int frequency = 0, symbolrate = 0;
				
	int serviceid = 0;
	int8_t videocodec = 0, audiocodec = 0;
	int16_t videopid = 0, audiopid = 0;
				
	char* tmpstr = NULL, *tmpstr1 = NULL;
	struct transponder* tp = NULL;
	struct channel* chnode = NULL;
				
	tmpstr = ostrcat(testtransponder, NULL, 0, 0);
	if(tmpstr == NULL)
		textbox(_("Message"), _("testtransponder not found"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 0, 0);
	else
	{
		int ret = sscanf(line, "%"SCNu8"#%d#%"SCNu8"#%d#%d#%"SCNu8"#%"SCNu8"#%"SCNu8"#%"SCNu8"#%"SCNu8"#%"SCNu8, &fetype, &frequency, &polarization, &orbitalpos, &symbolrate, &modulation, &fec, &pilot, &rolloff, &inversion, &system);
		if(ret != 11)
			textbox(_("Message"), _("testtransponder entry not ok"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 0, 0);			
		else
		{
			tp = createtransponder(tpid, fetype, orbitalpos, frequency, inversion, symbolrate, polarization, fec, modulation, rolloff, pilot, system);			
			if(tp == NULL)
				textbox(_("Message"), _("can't create new transponer"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 0, 0);			
			else
			{
				tmpstr1 = ostrcat(testchannel, NULL, 0, 0);
				if(tmpstr1 == NULL)
					textbox(_("Message"), _("testchannel not found"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 0, 0);
				else
				{
					int ret = sscanf(line, "%d#%"SCNu8"#%"SCNu8"#%"SCNu16"#%"SCNu16, &newnode->serviceid, &newnode->videocodec, &newnode->audiocodec, &newnode->videopid, &newnode->audiopid);
					if(ret != 5)
						textbox(_("Message"), _("testchannel entry not ok"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 0, 0);			
					else
					{
						chnode = createchannel("testchannel", tpid, 0, serviceid, 99, 0, videocodec, audiocodec, videopid, audiopid, 0);
						if(chnode == NULL)
							textbox(_("Message"), _("can't create new channel"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 0, 0);			
						else
							servicecheckret(servicestart(chnode, NULL, NULL, 5), 0);
					}
				}
			}
		}
	}
	delchannel(serviceid, tpid, 1);
	deltransponder(tp);
	free(tmpstr); tmpstr = NULL;
	free(tmpstr1); tmpstr1 = NULL;
}

void screenhwtest()
{
	struct menulist* mlist = NULL, *mbox = NULL;
	struct skin* load = getscreen("loading");
	struct dvbdev* dvbnode = dvbdev;
	struct channel* lastchannel = status.aktservice->channel;
	
	while(dvbnode != NULL)
	{
		if(dvbnode->type == FRONTENDDEV && dvbnode->feinfo != NULL)
		{
			if(dvbnode->feinfo->type == FE_QPSK)
			{
				char* tmpstr = ostrcat("[S]Lock & Search", " (", 0, 0);
				tmpstr = ostrcat(tmpstr, dvbnode->feinfo->name, 1, 0);
				tmpstr = ostrcat(tmpstr, ")", 1, 0);
				addmenulist(&mlist, tmpstr, NULL, NULL, 0, 0);
			}
			if(dvbnode->feinfo->type == FE_OFDM)
			{
				char* tmpstr = ostrcat("[T]Lock & Search", " (", 0, 0);
				tmpstr = ostrcat(tmpstr, dvbnode->feinfo->name, 1, 0);
				tmpstr = ostrcat(tmpstr, ")", 1, 0);
				addmenulist(&mlist, tmpstr, NULL, NULL, 0, 0);
			}
			if(dvbnode->feinfo->type == FE_QAM)
			{
				char* tmpstr = ostrcat("[C]Lock & Search", " (", 0, 0);
				tmpstr = ostrcat(tmpstr, dvbnode->feinfo->name, 1, 0);
				tmpstr = ostrcat(tmpstr, ")", 1, 0);
				addmenulist(&mlist, tmpstr, NULL, NULL, 0, 0);
			}
		}
		
		dvbnode = dvbnode->next;
	}
	addmenulist(&mlist, "Front Display", NULL, NULL, 0, 0);
	addmenulist(&mlist, "Front Key", NULL, NULL, 0, 0);
	addmenulist(&mlist, "Color Bar", NULL, NULL, 0, 0);
	addmenulist(&mlist, "SCART 4:3 / 16:9", NULL, NULL, 0, 0);
	addmenulist(&mlist, "USB Port", NULL, NULL, 0, 0);
	addmenulist(&mlist, "CAM", NULL, NULL, 0, 0);
	
	while(1)
	{
		mbox = menulistbox(mlist, NULL, "Hardware Test", NULL, NULL, 1, 0);
		
		if(mbox != NULL)
		{
			setmenulistdefault(mlist, mbox->name);
			if(ostrstr(mbox->name, "[S]Lock & Search") == 0)
			{
				testzap(getconfig("testtransponder1s", NULL), getconfig("testchannel1s", NULL));
				sleep(2);
				testzap(getconfig("testtransponder2s", NULL), getconfig("testchannel2s", NULL));
			}
			
			if(ostrstr(mbox->name, "[T]Lock & Search") == 0)
			{
				//struct transponder* createtransponder(uint64_t id, FE_OFDM, int orbitalpos, TEST_S2_FREQ, int inversion, TEST_S2_SYMBOL, int polarization, int fec, int modulation, int rolloff, int pilot, int system);			
			}
			
			if(ostrstr(mbox->name, "[C]Lock & Search") == 0)
			{
				//struct transponder* createtransponder(uint64_t id, FE_QAM, int orbitalpos, TEST_S2_FREQ, int inversion, TEST_S2_SYMBOL, int polarization, int fec, int modulation, int rolloff, int pilot, int system);			
			}
			
			if(ostrcmp(mbox->name, "Front Display") == 0)
			{
				int i = 0;
				char* tmpstr = NULL;

				for(i = 0; i < 9; i++)
				{
					tmpstr = ostrcat(oitoa(i), oitoa(i), 1, 1);
					tmpstr = ostrcat(tmpstr, oitoa(i), 1, 1);
					tmpstr = ostrcat(tmpstr, oitoa(i), 1, 1);
					writevfd(tmpstr);
					free(tmpstr); tmpstr = NULL;
					usleep(300000);
				}
				free(tmpstr); tmpstr = NULL;
			}
			
			if(ostrcmp(mbox->name, "Front Key") == 0)
			{
				char* tmpload = ostrcat(load->text, NULL, 0, 0);
				while(1)
				{
					changetext(load, _("Press Front Key"));
					clearscreen(load);
					drawscreen(load, 0, 0);
					int rcret = waitrc(NULL, 0, 0);
					
					if(rcret == getrcconfigint("rcexit", NULL) || rcret == getrcconfigint("rcok", NULL))
						break;
					
					char* tmpstr = ostrcat(_("Got Keycode: "), oitoa(rcret), 0, 1);
					changetext(load, tmpstr);
					clearscreen(load);
					drawscreen(load, 0, 0);
					free(tmpstr); tmpstr = NULL;
						
					sleep(1); 
				}
				changetext(load, tmpload);
				free(tmpload); tmpload = NULL; 			
			}
			
			if(ostrcmp(mbox->name, "Color Bar") == 0)
			{
				char* tmpstr = NULL;
				char* tmpload = ostrcat(load->text, NULL, 0, 0);
				long tmpbgcol = load->bgcol;
				int i = 0;
				long col = 0;
				
				for(i = 0; i < 3; i++)
				{
					if(i == 0)
					{
						tmpstr = ostrcat(tmpstr, _("RED"), 1, 0);
						col = 0x00FF0000;
					}
					if(i == 1)
					{
						tmpstr = ostrcat(tmpstr, _("GREEN"), 1, 0);
						col = 0x0000FF00;
					}
					if(i == 2)
					{
						tmpstr = ostrcat(tmpstr, _("BLUE"), 1, 0);
						col = 0x000000FF;
					}
					changetext(load, tmpstr);
					load->bgcol = col;
					clearscreen(load);
					drawscreen(load, 0, 0);
					free(tmpstr); tmpstr = NULL;
					sleep(1);
				}
				
				free(tmpstr); tmpstr = NULL;
				changetext(load, tmpload);
				load->bgcol = tmpbgcol;			
			}
			
			if(ostrcmp(mbox->name, "SCART 4:3 / 16:9") == 0)
			{
				char* tmpload = ostrcat(load->text, NULL, 0, 0);
				char* tmpstr = NULL;

				tmpstr = ostrcat(tmpstr, "4:3", 1, 0);
				changetext(load, tmpstr);
				clearscreen(load);
				drawscreen(load, 0, 0);
				setaspect(tmpstr);
				sleep(1);

				free(tmpstr); tmpstr = NULL;
				tmpstr = ostrcat(tmpstr, "16:9", 1, 0);
				changetext(load, tmpstr);
				clearscreen(load);
				drawscreen(load, 0, 0);
				setaspect(tmpstr);
				sleep(1);

				free(tmpstr); tmpstr = NULL;
				changetext(load, tmpload);
			}
			
			if(ostrcmp(mbox->name, "USB Port") == 0)
			{
				int usbcount = 0;
				if(file_exist("/sys/bus/usb/devices/usb1") == 1) usbcount++;
				if(file_exist("/sys/bus/usb/devices/usb2") == 1) usbcount++;
				if(file_exist("/sys/bus/usb/devices/usb3") == 1) usbcount++;
				if(file_exist("/sys/bus/usb/devices/usb4") == 1) usbcount++;
				
				char* tmpstr = ostrcat(_("USB Ports found: "), oitoa(usbcount), 0, 1);
				textbox(_("Message"), tmpstr, _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 0, 0);			
				free(tmpstr); tmpstr = NULL;
			}
			
			if(ostrcmp(mbox->name, "CAM") == 0)
			{
				int cicount = 0;
				dvbnode = dvbdev;
	
				while(dvbnode != NULL)
				{
					if(dvbnode->type == CIDEV)
						cicount++;
						
					dvbnode = dvbnode->next;
				}
				
				char* tmpstr = ostrcat(_("CAM Ports found: "), oitoa(cicount), 0, 1);
				textbox(_("Message"), tmpstr, _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 0, 0);			
				free(tmpstr); tmpstr = NULL;			
			}
		}
		else
			break;
	}

	resettvpic();	
	servicestart(lastchannel, NULL, NULL, 0);	
	freemenulist(mlist, 1); mlist = NULL;
}

#endif
