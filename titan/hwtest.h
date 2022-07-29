#ifndef HWTEST_H
#define HWTEST_H

#ifndef IOCTL_GET_IS_CARD_PRESENT 
#define IOCTL_GET_IS_CARD_PRESENT	_IOW('s', 8, uint32_t)
#endif

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
//testtransponder1s=0#4000000#0#192#28125000#?#3#?#?#?#0
//testchannel1s=8#0#1#380#381
//testzap("0#10900000#0#192#28125000#1#3#2#3#2#0", "8#0#1#380#381#-1"); 

//Sat 19.2 (ProSieben LIVE)
//testtransponder1s=0#12544750#0#192#22000000#0#4#2#0#2#0
//testchannel1s=17501#0#1#511#512
//testzap("0#12544750#0#192#22000000#0#4#2#0#2#0", "17501#0#1#511#512#-1"); 

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
//testtransponder2s=0#4132000#0#192#10670000#2#7#?#?#?#1
//testchannel2s=15#1#1#711#731

//Sat 19.2 (ZDF HDneu)
//testzap("0#11362000#0#192#22000000#2#2#2#0#2#1", "11110#1#1#6110#6122#-1");
//testtransponder2s=0#11362000#0#192#22000000#2#2#2#0#2#1
//testchannel2s=11110#1#1#6110#6122

//Sat 19.2 (ZDF HD)
//testzap("0#11361750#0#192#22000000#2#2#2#0#2#1", "11110#1#1#6110#6120#-1");
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
	int16_t videopid = 0, audiopid = 0, pcrpid = 0;
				
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
					int ret = sscanf(tmpstr1, "%d#%"SCNu8"#%"SCNu8"#%"SCNu16"#%"SCNu16"#%"SCNu16, &serviceid, &videocodec, &audiocodec, &videopid, &audiopid, &pcrpid);
					if(ret != 6)
						textbox(_("Message"), _("testchannel entry not ok"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 0, 0);			
					else
					{
						chnode = createchannel("testchannel", tpid, 0, serviceid, 99, 0, videocodec, audiocodec, videopid, audiopid, 0, pcrpid, NULL, NULL);
						if(chnode == NULL)
							textbox(_("Message"), _("can't create new channel"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 0, 0);			
						else
							servicecheckret(servicestart(chnode, NULL, NULL, 5), 0);
					}
				}
			}
		}
	}

	//del channel from service, so we can delete it
	m_lock(&status.servicemutex, 2);
	struct service* snode = service;

	while(snode != NULL)
	{
		if(snode->channel == chnode)
			snode->channel = NULL;
		snode = snode->next;
	}
	m_unlock(&status.servicemutex, 2);

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
				char* tmpstr = ostrcat(_("[S]Lock & Search"), " (", 0, 0);
				tmpstr = ostrcat(tmpstr, dvbnode->feinfo->name, 1, 0);
				tmpstr = ostrcat(tmpstr, ")", 1, 0);
				addmenulist(&mlist, tmpstr, NULL, NULL, 0, 0);
			}
			if(dvbnode->feinfo->type == FE_OFDM)
			{
				char* tmpstr = ostrcat(_("[T]Lock & Search"), " (", 0, 0);
				tmpstr = ostrcat(tmpstr, dvbnode->feinfo->name, 1, 0);
				tmpstr = ostrcat(tmpstr, ")", 1, 0);
				addmenulist(&mlist, tmpstr, NULL, NULL, 0, 0);
			}
			if(dvbnode->feinfo->type == FE_QAM)
			{
				char* tmpstr = ostrcat(_("[C]Lock & Search"), " (", 0, 0);
				tmpstr = ostrcat(tmpstr, dvbnode->feinfo->name, 1, 0);
				tmpstr = ostrcat(tmpstr, ")", 1, 0);
				addmenulist(&mlist, tmpstr, NULL, NULL, 0, 0);
			}
		}
		
		dvbnode = dvbnode->next;
	}
	addmenulist(&mlist, _("Front Display"), NULL, NULL, 0, 0);
	addmenulist(&mlist, _("Front Key"), NULL, NULL, 0, 0);
	//addmenulist(&mlist, _("Color Bar"), NULL, NULL, 0, 0);
	//addmenulist(&mlist, _("SCART 4:3 / 16:9"), NULL, NULL, 0, 0);
	addmenulist(&mlist, _("Smartcard"), NULL, NULL, 0, 0);
	addmenulist(&mlist, _("Network"), NULL, NULL, 0, 0);
	addmenulist(&mlist, _("USB Port"), NULL, NULL, 0, 0);
	addmenulist(&mlist, _("CAM"), NULL, NULL, 0, 0);
	addmenulist(&mlist, _("RS232"), NULL, NULL, 0, 0);
	addmenulist(&mlist, _("LNB"), NULL, NULL, 0, 0);
	addmenulist(&mlist, _("Flash/Nand Check"), NULL, NULL, 0, 0);
	
	while(1)
	{
		mbox = menulistbox(mlist, NULL, _("Hardware Test"), NULL, NULL, NULL, 1, 0);
		
		if(mbox != NULL)
		{
			setmenulistdefault(mlist, mbox->name);
			if(ostrstr(mbox->name, _("[S]Lock & Search")) != NULL)
			{
				clearscreen(load);
				drawscreen(load, 0, 0);
				resettvpic();				
//				testzap(getconfig("testtransponder1s", NULL), getconfig("testchannel1s", NULL));
//				textbox(_("Message"), _("Press OK or EXIT"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 400, 0, 0);
//				testzap(getconfig("testtransponder2s", NULL), getconfig("testchannel2s", NULL));
//				textbox(_("Message"), _("Press OK or EXIT"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 400, 0, 0);
//alt				testzap("0#10900000#0#192#28125000#1#3#2#3#2#0", "8#0#1#380#381");
				testzap("0#10900000#0#192#28125000#1#3#2#3#2#0", "8#0#1#380#381#-1"); 
				textbox(_("Message"), _("Press OK or EXIT"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 400, 0, 0);
				servicestart(lastchannel, NULL, NULL, 0);	
			}
			
			if(ostrstr(mbox->name, _("[T]Lock & Search")) != NULL)
			{
				//struct transponder* createtransponder(uint64_t id, FE_OFDM, int orbitalpos, TEST_S2_FREQ, int inversion, TEST_S2_SYMBOL, int polarization, int fec, int modulation, int rolloff, int pilot, int system);			
			}
			
			if(ostrstr(mbox->name, _("[C]Lock & Search")) != NULL)
			{
				//struct transponder* createtransponder(uint64_t id, FE_QAM, int orbitalpos, TEST_S2_FREQ, int inversion, TEST_S2_SYMBOL, int polarization, int fec, int modulation, int rolloff, int pilot, int system);			
			}
			
			if(ostrcmp(mbox->name, _("Front Display")) == 0)
			{
				int i = 0;
				char* tmpstr = NULL;

				status.updatevfd = PAUSE;
				for(i = 0; i < 10; i++)
				{
					tmpstr = ostrcat(oitoa(i), oitoa(i), 1, 1);
					tmpstr = ostrcat(tmpstr, oitoa(i), 1, 1);
					tmpstr = ostrcat(tmpstr, oitoa(i), 1, 1);
					writevfd(tmpstr);
					free(tmpstr); tmpstr = NULL;
					usleep(600000);
				}
				free(tmpstr); tmpstr = NULL;
				status.updatevfd = START;
			}
			
			if(ostrcmp(mbox->name, _("Front Key")) == 0)
			{
				char* tmpload = ostrcat(load->text, NULL, 0, 0);
				char* tmpstr = NULL;
				while(1)
				{
					changetext(load, _("Press Front Key"));
					clearscreen(load);
					drawscreen(load, 0, 0);
					int rcret = waitrc(NULL, 0, 0);
					
					if(rcret == getrcconfigint("rcexit", NULL) || rcret == getrcconfigint("rcok", NULL))
						break;
					
					if(rcret == getrcconfigint("rcup", NULL)) tmpstr = ostrcat(tmpstr, "LEFT", 1, 0);
					if(rcret == getrcconfigint("rcdown", NULL)) tmpstr = ostrcat(tmpstr, "RIGHT", 1, 0);
					if(rcret == getrcconfigint("rcpower", NULL)) tmpstr = ostrcat(tmpstr, "POWER", 1, 0);
					changetext(load, tmpstr);
					clearscreen(load);
					drawscreen(load, 0, 0);
					free(tmpstr); tmpstr = NULL;
						
					sleep(1); 
				}
				changetext(load, tmpload);
				free(tmpload); tmpload = NULL; 			
			}
			
			if(ostrcmp(mbox->name, _("Color Bar")) == 0)
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
			
			if(ostrcmp(mbox->name, _("SCART 4:3 / 16:9")) == 0)
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
			
			if(ostrcmp(mbox->name, _("Smartcard")) == 0)
			{
				uint32_t status = 0;
				int smartcardcount = 0;
				char* tmpstr = NULL, *tmpstr1 = NULL;
				unsigned char buf[1] = {0};
				
				drawscreen(load, 0, 0);
				dvbnode = dvbdev;
				while(dvbnode != NULL)
				{
					if(dvbnode->type == SCDEV)
					{
						smartcardcount++;
						status = 0;
						dvbnode->fd = scopendirect(dvbnode->dev);
						scgetiscardpresent(dvbnode, &status);
						if(status == 1)
						{
							status = 0;
							if(scsetreset(dvbnode) == 0)
							{
								sciparameters params;
								params.ETU = 372;
								params.EGT = 3;
								params.fs = 5;
								params.T = 0;
								
								if(scsetparameters(dvbnode, &params) == 0)
								{
									if(dvbreadfd(dvbnode->fd, buf, 0, 1, -1, 1) == 1)
										status = 1;
								}
							}						
						}
						scclose(dvbnode, -1);
						
						if(status == 1)
						{
							tmpstr1 = ostrcat(tmpstr1, _("Smartcard"), 1, 0);
							tmpstr1 = ostrcat(tmpstr1, oitoa(smartcardcount), 1, 1);
							tmpstr1 = ostrcat(tmpstr1, " : ", 1, 0);
							tmpstr1 = ostrcat(tmpstr1, _("Present"), 1, 0);
							tmpstr1 = ostrcat(tmpstr1, "\n", 1, 0);
						}
						else
						{
							tmpstr1 = ostrcat(tmpstr1, ": ", 1, 0);
							tmpstr1 = ostrcat(tmpstr1, _("Not Present"), 1, 0);
							tmpstr1 = ostrcat(tmpstr1, "\n", 1, 0);
						}
					}
					
					dvbnode = dvbnode->next;
				}
				clearscreen(load);
				
				tmpstr = ostrcat(_("Smartcard Ports found: "), oitoa(smartcardcount), 0, 1);
				tmpstr = ostrcat(tmpstr, "\n\n", 1, 0);
				tmpstr = ostrcat(tmpstr, tmpstr1, 1, 0);
									
				textbox(_("Message"), tmpstr, _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 400, 0, 0);			
			
				free(tmpstr); tmpstr = NULL;
				free(tmpstr1); tmpstr1 = NULL;
			}
			
			if(ostrcmp(mbox->name, _("Network")) == 0)
			{
				struct inetwork* inode = inetwork;
				
				while(inode != NULL)
				{
					if(ostrcmp(inode->device, "eth0") == 0)
						break;
					inode = inode->next;
				}
				
				if(inode != NULL)
				{
					char* tmpstr = ostrcat(inode->device, ": ", 0, 0);
					tmpstr = ostrcat(tmpstr, inode->ip, 0, 0);
					textbox(_("Message"), tmpstr, _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 0, 0);			
					free(tmpstr); tmpstr = NULL;
				}
			}
			
			if(ostrcmp(mbox->name, _("USB Port")) == 0)
			{
				int usbcount = 0, found = 0, i = 0;
				struct hdd* hddnode = NULL;
				char* tmpstr = NULL;
				
				if(file_exist("/sys/bus/usb/devices/usb1") == 1) usbcount++;
				if(file_exist("/sys/bus/usb/devices/usb2") == 1) usbcount++;
				if(file_exist("/sys/bus/usb/devices/usb3") == 1) usbcount++;
				if(file_exist("/sys/bus/usb/devices/usb4") == 1) usbcount++;
				
				tmpstr = ostrcat(_("USB Ports found: "), oitoa(usbcount), 0, 1);
				tmpstr = ostrcat(tmpstr, "\n", 1, 0);
				addhddall();
				
				hddnode = hdd;
				while(hddnode != NULL)
				{
					if(hddnode->partition == 0)
					{
						found++;
						tmpstr = ostrcat(tmpstr, "\n", 1, 0);
						tmpstr = ostrcat(tmpstr, _("USB "), 1, 0);
						tmpstr = ostrcat(tmpstr, oitoa(found), 1, 1);
						tmpstr = ostrcat(tmpstr, " : ", 1, 0);
						tmpstr = ostrcat(tmpstr, hddnode->device, 1, 0);
						tmpstr = ostrcat(tmpstr, _(" - OKAY!"), 1, 0);
					}
					hddnode = hddnode->next;
				}
				
				found++; usbcount++;
				for(i = found; i < usbcount; i++)
				{
					tmpstr = ostrcat(tmpstr, "\n", 1, 0);
					tmpstr = ostrcat(tmpstr, _("USB "), 1, 0);
					tmpstr = ostrcat(tmpstr, oitoa(i), 1, 1);
					tmpstr = ostrcat(tmpstr, _(" : NOT OKAY!"), 1, 0);
				}
				
				textbox(_("Message"), tmpstr, _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 400, 0, 0);			
				free(tmpstr); tmpstr = NULL;
			}
			
			if(ostrcmp(mbox->name, _("CAM")) == 0)
			{
				int cicount = 0;
				char* tmpstr = NULL, *tmpstr1 = NULL;
				dvbnode = dvbdev;

				while(dvbnode != NULL)
				{
					if(dvbnode->type == CIDEV)
					{
						cicount++;
						
						tmpstr = ostrcat(tmpstr, _("Slot "), 1, 0);
						tmpstr = ostrcat(tmpstr, oitoa(cicount), 1, 1);
						tmpstr = ostrcat(tmpstr, ": ", 1, 0);
						if(dvbnode->caslot != NULL)
						{
							if(dvbnode->caslot->name == NULL || dvbnode->caslot->status == 0)
								tmpstr = ostrcat(tmpstr, _("unknown"), 1, 0);
							else
								tmpstr = ostrcat(tmpstr, dvbnode->caslot->name, 1, 0);
								
							tmpstr = ostrcat(tmpstr, "\n", 1, 0);
						}
					}

					dvbnode = dvbnode->next;
				}

				tmpstr1 = ostrcat(_("CAM Ports found: "), oitoa(cicount), 0, 1);
				tmpstr1 = ostrcat(tmpstr1, "\n\n", 1, 0);
				tmpstr = ostrcat(tmpstr1, tmpstr, 0, 1);
				textbox(_("Message"), tmpstr, _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 400, 0, 0);			
				free(tmpstr); tmpstr = NULL;
				free(tmpstr1); tmpstr1 = NULL;			
			}

			if(ostrcmp(mbox->name, _("RS232")) == 0)
			{
				int exist = 1, count = 0, okA = 0, okB = 0, okC = 0, okD = 0;
				unsigned char tmpwr[4] = {0};
				unsigned char tmprd[1] = {0};

				drawscreen(load, 0, 0);

				tmpwr[0] = 'A'; //65
				tmpwr[1] = 'B'; //66
				tmpwr[2] = 'C'; //67
				tmpwr[3] = 'D'; //68

				if(!file_exist(SERIALDEV))
				{
					mknod(SERIALDEV, S_IFCHR | 0666, makedev(204, 40));
					exist = 0;
				}

				int fd = open(SERIALDEV, O_RDWR | O_NOCTTY | O_NDELAY | O_NONBLOCK);
				if(fd >= 0)
				{
					//struct termios port_settings;      // structure to store the port settings in
					int ret = 0;

					//tcgetattr(fd, &port_settings);

					//cfsetispeed(&port_settings, B9600);    // set baud rates
					//cfsetospeed(&port_settings, B9600);

					//port_settings.c_cflag |= (CLOCAL | CREAD);
					//port_settings.c_cflag &= ~PARENB;    // set no parity, stop bits, data bits
					//port_settings.c_cflag &= ~CSTOPB;
					//port_settings.c_cflag &= ~CSIZE;
					//port_settings.c_cflag |= CS8;
					//tcsetattr(fd, TCSANOW, &port_settings);

					tcflush(fd, TCIOFLUSH);

					ret = dvbwrite(fd, tmpwr, 4, -1);
					if(ret != 4)
					{
						err("write %s (ret=%d)", SERIALDEV, ret);
					}

					while(dvbreadfd(fd, tmprd, 0, 1, -1, 0) == 1 && count < 200)
					{
						if(tmprd[0] == 'A') okA = 1;
						if(tmprd[0] == 'B') okB = 1;
						if(tmprd[0] == 'C') okC = 1;
						if(tmprd[0] == 'D') okD = 1;

						count++;	
					}

					close(fd);
				}
				else
				{
					perr("open %s", SERIALDEV);
				}
				
				if(exist == 0) unlink(SERIALDEV);
				
				clearscreen(load);

				if(okA == 1 && okB == 1 && okC == 1 && okD == 1)
					textbox(_("Message"), _("SUCESS Serial Port"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 0, 0);			
				else
					textbox(_("Message"), _("FAIL Serial Port"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 0, 0);			
			}
			
			if(ostrcmp(mbox->name, _("LNB")) == 0)
			{
				char* tmpstr = NULL;
				int ret1 = 0, ret2 = 0;

				ret1 = fesetvoltage(status.aktservice->fedev, SEC_VOLTAGE_OFF, 15);
				ret2 = fesettone(status.aktservice->fedev, SEC_TONE_OFF, 15);
				if(ret1 == 0 && ret2 == 0)
				{
					tmpstr = ostrcat(tmpstr, _("LNB off / 22K off: OK"), 1, 0);
					tmpstr = ostrcat(tmpstr, "\n", 1, 0);
				}
				else
				{
					tmpstr = ostrcat(tmpstr, _("LNB off / 22K off: NOT OK"), 1, 0);
					tmpstr = ostrcat(tmpstr, "\n", 1, 0);
				}
				textbox(_("Message"), tmpstr, _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 5, 0);

				free(tmpstr); tmpstr = NULL;

				ret1 = fesetvoltage(status.aktservice->fedev, SEC_VOLTAGE_18, 15);
				ret2 = fesettone(status.aktservice->fedev, SEC_TONE_OFF, 15);
				if(ret1 == 0 && ret2 == 0)
				{
					tmpstr = ostrcat(tmpstr, _("LNB 18V / 22K off: OK"), 1, 0);
					tmpstr = ostrcat(tmpstr, "\n", 1, 0);
				}
				else
				{
					tmpstr = ostrcat(tmpstr, _("LNB 18V / 22K off: NOT OK"), 1, 0);
					tmpstr = ostrcat(tmpstr, "\n", 1, 0);
				}
				textbox(_("Message"), tmpstr, _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 5, 0);
				free(tmpstr); tmpstr = NULL;

				ret1 = fesetvoltage(status.aktservice->fedev, SEC_VOLTAGE_13, 15);
				ret2 = fesettone(status.aktservice->fedev, SEC_TONE_OFF, 15);
				if(ret1 == 0 && ret2 == 0)
				{
					tmpstr = ostrcat(tmpstr, _("LNB 13V / 22K off: OK"), 1, 0);
					tmpstr = ostrcat(tmpstr, "\n", 1, 0);
				}
				else
				{
					tmpstr = ostrcat(tmpstr, _("LNB 13V / 22K off: NOT OK"), 1, 0);
					tmpstr = ostrcat(tmpstr, "\n", 1, 0);
				}
				textbox(_("Message"), tmpstr, _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 5, 0);
				free(tmpstr); tmpstr = NULL;
				
				ret1 = fesetvoltage(status.aktservice->fedev, SEC_VOLTAGE_13, 15);
				ret2 = fesettone(status.aktservice->fedev, SEC_TONE_ON, 15);
				if(ret1 == 0 && ret2 == 0)
				{
					tmpstr = ostrcat(tmpstr, _("LNB 13V / 22K on: OK"), 1, 0);
					tmpstr = ostrcat(tmpstr, "\n", 1, 0);
				}
				else
				{
					tmpstr = ostrcat(tmpstr, _("LNB 13V / 22K on: NOT OK"), 1, 0);
					tmpstr = ostrcat(tmpstr, "\n", 1, 0);
				}
				textbox(_("Message"), tmpstr, _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 5, 0);
				free(tmpstr); tmpstr = NULL;
				
				drawscreen(load, 0, 0);
				servicestart(lastchannel, NULL, NULL, 5);
				clearscreen(load);	
			}
			if(ostrcmp(mbox->name, _("Flash/Nand Check")) == 0)
			{
				int ret1 = 0, ret2 = 0, i = 0;
				char* tmpstr = NULL, *tmpstr1 = NULL;
				
				for(i = 0; i < 20; i++)
					tmpstr = ostrcat(tmpstr, "0123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789", 1, 0);

				//check /var
				ret1 = writesys("/var/hwtest.txt", tmpstr, 0);
				if(ret1 == 0)
				{
					tmpstr1 = readsys("/var/hwtest.txt", 1);
					if(ostrcmp(tmpstr, tmpstr1) != 0) ret1 = 1;
				}

				if(ret1 == 0)
					ret1 = unlink("/var/hwtest.txt");
				else
					unlink("/var/hwtest.txt");

				free(tmpstr1); tmpstr1 = NULL;

				//check /mnt
				ret2 = writesys("/mnt/hwtest.txt", tmpstr, 0);
				if(ret2 == 0)
				{
					tmpstr1 = readsys("/mnt/hwtest.txt", 1);
					if(ostrcmp(tmpstr, tmpstr1) != 0) ret2 = 1;
				}

				if(ret2 == 0)
					ret2 = unlink("/mnt/hwtest.txt");
				else
					unlink("/mnt/hwtest.txt");

				free(tmpstr); tmpstr = NULL;
				free(tmpstr1); tmpstr1 = NULL;

				if(ret1 == 0 && ret2 == 0)
					textbox(_("Message"), _("SUCESS Flash/Nand"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 0, 0);			
				else if(ret1 != 0 && ret2 != 0)
					textbox(_("Message"), _("FAIL Flash/Nand (/var and /mnt)"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 0, 0);
				else if(ret1 != 0)
					textbox(_("Message"), _("FAIL Flash/Nand (/var)"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 0, 0);
				else if(ret2 != 0)
					textbox(_("Message"), _("FAIL Flash/Nand (/mnt)"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 0, 0);			
			}
		}
		else
			break;
	}

	resettvpic();	
	freemenulist(mlist, 1); mlist = NULL;
}

#endif
