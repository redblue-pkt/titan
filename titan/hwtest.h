#ifndef HWTEST_H
#define HWTEST_H

//DVB-S
//#define TEST_S11_FREQ   10900000 // 4000, 22Khz off
//#define TEST_S11_SYMBOL  28125000
//#define TEST_S11_POL    _POLAR_HOR
//#define TEST_S11_VIDEO_PID  380 
//#define TEST_S11_AUDIO_PID    381 
//#define TEST_S11_PCR_PID  380
//test_channel.videoType = V_MP2;
//test_channel.audioType = A_MP1;

//ASIASAT-5, 4000H28125 - France 24 English (fec: 3/4)
//testtransponder1s=0#4000000#0#0#28125000#?#3#?#?#?#0
//testchannel1s=8#0#1#380#381

//Sat 19.2 (ProSieben)
//testtransponder1s=0#12544750#0#192#22000000#0#4#2#0#2#0
//testchannel1s=17501#0#1#511#512

//DVB-S2
//#define TEST_S12_FREQ   10767000 // 4132, 22Khz off
//#define TEST_S12_SYMBOL  10670000
//#define TEST_S12_POL    _POLAR_HOR
//#define TEST_S12_VIDEO_PID  711 
//#define TEST_S12_AUDIO_PID    731 
//#define TEST_S12_PCR_PID  711
//test_channel.videoType = V_H264;
//test_channel.audioType = A_MP1;

//ASIASAT-5, 4132H10670 - Trace Urban English (modulation: 8PSK, fec: 3/5)
//testtransponder2s=0#4132000#0#0#10670000#2#7#?#?#?#1
//testchannel2s=15#1#1#711#731

//Sat 19.2 (ZDF HD)
//testtransponder2s=0#11361750#0#192#22000000#2#2#2#0#2#1
//testchannel2s=11110#1#1#6110#6120

//DVB-T
//#define TEST_T1_FREQ   5060000
//#define TEST_T1_BW    8
//#define TEST_T1_VIDEO_PID  305
//#define TEST_T1_AUDIO_PID  306
//#define TEST_T1_PCR_PID  305
//test_channel.videoType = V_MP2;
//test_channel.audioType = A_MP1;

//DVB-C
//#define TEST_C1_FREQ   3540000
//#define TEST_C1_SYMBOL   6900000
//#define TEST_C1_VIDEO_PID  305
//#define TEST_C1_AUDIO_PID  306
//#define TEST_C1_PCR_PID  305
//#define TEST_C1_CAB_QAM   _QAMMODE_64
//test_channel.videoType = V_MP2;
//test_channel.audioType = A_MP1;

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
		int ret = sscanf(tmpstr, "%"SCNu8"#%d#%"SCNu8"#%d#%d#%"SCNu8"#%"SCNu8"#%"SCNu8"#%"SCNu8"#%"SCNu8"#%"SCNu8, &fetype, &frequency, &polarization, &orbitalpos, &symbolrate, &modulation, &fec, &pilot, &rolloff, &inversion, &system);
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
					int ret = sscanf(tmpstr1, "%d#%"SCNu8"#%"SCNu8"#%"SCNu16"#%"SCNu16, &serviceid, &videocodec, &audiocodec, &videopid, &audiopid);
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
