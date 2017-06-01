#ifndef CEC_H
#define CEC_H

int cecon = 0;
int sreq = 0;

struct stimerthread* hdmiEventthread = NULL;


void sendName(unsigned char address)
{
	unsigned char cmd = 0x47;
	char data[16] = "TitanNit";
	
	data[8] = '\0';
	sendMessage(address, cmd, data, strlen(data));

}

void sendMenuInfo(unsigned char address)
{
	unsigned char cmd = 0x8e;
	char data[2];
		
	data[0] = 0x00;
	if(status.standby > 0 || getconfigint("cec_menue", NULL) == 0)
		data[0] = 0x01;
	data[1] = '\0';
	sendMessage(address, cmd, data, 1);
}

void sendVendorid(unsigned char address)
{
	unsigned char cmd = 0x87;
	char data[4];
	
	data[0] = 0x00;
	data[1] = 0x00;
	data[2] = 0x00;
	data[3] = '\0';
	sendMessage(address, cmd, data, 3);
}

void sendSource(unsigned char address)
{
	unsigned char cmd = 0x82;
	char data[4];
	
	data[0] = cec_physicalAddress[0];
	data[1] = cec_physicalAddress[1];
	data[2] = '\0';
	sendMessage(address, cmd, data, 2);
}

void sendVersion(unsigned char address)
{
	unsigned char cmd = 0x9e;
	char data[4];
	
	data[0] = 0x01;
	data[1] = '\0';
	sendMessage(address, cmd, data, 2);
}

void sendTVon()
{
	char data[3];
	unsigned char address = 0x00;
	unsigned char cmd = 0x04;
	data[0] = '\0';
	sendMessage(address, cmd, data, 0);
}

void sendswitch()
{	
	char data[3];
	unsigned char address = 0x0f;
	unsigned char cmd = 0x82;
	data[0] = cec_physicalAddress[0];
	data[1] = cec_physicalAddress[1];
	data[2] = '\0';
	//if(getconfigint("cec_tv_switch", NULL) == 1)
	sendMessage(address, cmd, data, 2);
}

int translateKey(unsigned char code)
{
	int key = 0;
	switch (code)
	{
		case 0x09:
			key = 0x8b;
			break;
		case 0x0A:
			key = 0x18e;
			break;
		case 0x0B:
			key = 0x166;
			break;
		case 0x32:
			key = 0x8b;
			break;
		case 0x20:
			key = 0x0b;
			break;
		case 0x21:
			key = 0x02;
			break;
		case 0x22:
			key = 0x03;
			break;
		case 0x23:
			key = 0x04;
			break;
		case 0x24:
			key = 0x05;
			break;
		case 0x25:
			key = 0x06;
			break;
		case 0x26:
			key = 0x07;
			break;
		case 0x27:
			key = 0x08;
			break;
		case 0x28:
			key = 0x09;
			break;
		case 0x29:
			key = 0x0a;
			break;
		case 0x30:
			key = 0x192;
			break;
		case 0x31:
			key = 0x193;
			break;
		case 0x44:
			key = 0xcf;
			break;
		case 0x45:
			key = 0x80;
			break;
		case 0x46:
			key = 0x77;
			break;
		case 0x47:
			key = 0xa7;
			break;
		case 0x48:
			key = 0xa8;
			break;
		case 0x49:
			key = 0xd0;
			break;
		case 0x53:
			key = 0x166;
			break;
		case 0x54:
			key = 0x16a;
			break;
		case 0x60:
			key = 0xcf;
			break;
		case 0x61:
			key = 0xa4;
			break;
		case 0x62:
			key = 0xa7;
			break;
		case 0x64:
			key = 0x80;
			break;
		case 0x00:
			key = 0x160;
			break;
		case 0x03:
			key = 0x69;
			break;
		case 0x04:
			key = 0x6a;
			break;
		case 0x01:
			key = 0x67;
			break;
		case 0x02:
			key = 0x6c;
			break;
		case 0x0d:
			key = 0xae;
			break;
		case 0x72:
			key = 0x18e;
			break;
		case 0x71:
			key = 0x191;
			break;
		case 0x73:
			key = 0x18f;
			break;
		case 0x74:
			key = 0x190;
			break;
		default:
			key = 0x8b;
			break;
	}
	return key;
}
	
void setVolumeForward()
{
	unsigned char address = 0x00;
	unsigned char cmd = 0x7d;
	char data[2];
	data[0] = '\0';
	
	addconfiginttmp("ForwardVolume", 1);
	data[0] = '\0';	
	if(getconfigint("cec_volume", NULL) > 0)
	{
		if(getconfigint("cec_volume", NULL) == 1)
			address = 0x05;
		if(getconfigint("cec_volume", NULL) == 2)
			address = 0x00;
		
		//cmd = 0x72;
		//data[0] = 0x01;
		//data[1] = '\0';
		//m_lock(&status.cecmutex, 26);
		//sendMessage(address, cmd, data, strlen(data));
		//m_unlock(&status.cecmutex, 26);
		
		//cmd = 0x70;
		//data[0] = 0x02;
		//data[1] = 0x00;
		//data[2] = '\0';
		//m_lock(&status.cecmutex, 26);
		//sendMessage(address, cmd, data, 2);
		//m_unlock(&status.cecmutex, 26);
		
		cmd = 0x7d;
		data[0] = '\0';
		sendMessage(address, cmd, data, strlen(data));
	}
}

void hdmiEvent()
{
	fd_set rfds;
	struct timeval tv;
	struct cec_rx_message rxmessage;
	int ret = 0;
	int hasdata = 0;
	int keypressed = 0;
	int transkey = 0;
	unsigned char pressedkey = 0x00;
	
	while (hdmiEventthread->aktion != STOP)
	{
		printf("**** start Thread\n");
		tv.tv_sec = 0x7fffffff;
		tv.tv_usec = 0;
		FD_ZERO(&rfds);
		FD_SET(status.hdmiFd, &rfds);

		hasdata = 0;

		ret = select(status.hdmiFd + 1, &rfds, NULL, NULL, &tv);
		if(ret > 0)
		{
			if(FD_ISSET(status.hdmiFd, &rfds))
			{
#ifdef DREAMBOX
				if (ioctl(status.hdmiFd, 2, &rxmessage) >= 0)
					hasdata = 1;
				
				unsigned int val = 0;
				ioctl(status.hdmiFd, 4, &val);
#else
				if (read(status.hdmiFd, &rxmessage, 2) == 2)
				{
					if (read(status.hdmiFd, &rxmessage.data, rxmessage.length) == rxmessage.length)
						hasdata = 1;
				}
#endif
				if(hasdata)
				{
					printf("CEC: rxmessage ->");
					printf(" %02x ", rxmessage.address);
					for (int i = 0; i < rxmessage.length; i++)
						printf(" %02x", rxmessage.data[i]);
					printf("\n");
					
					switch (rxmessage.data[0])
					{
						case 0x7e:
						case 0x72: /* system audio mode status */
						{
							if(rxmessage.data[1] == 0x01)
							{
								addconfiginttmp("ForwardVolume", 1);
								printf("HDMI_CEC: Volume forward is acticated\n");	
							}
							else
							{
								printf("HDMI_CEC: Volume forward is not supported\n");	
								addconfiginttmp("ForwardVolume", 0);
							}
							break;
						}
						
						case 0x46: /* request name */
						{
							sendName(rxmessage.address);
							break;
						}
						case 0x8c: /* request vendor id */
						{
							sendVendorid(rxmessage.address);
							break;
						}
						case 0x8d: /* menu request */
						{
							//if(rxmessage.data[1] == 0x02)
								sendMenuInfo(rxmessage.address);
							break;
						}
						case 0x80:
						{
							if(rxmessage.data[3]== cec_physicalAddress[0] && rxmessage.data[4]== cec_physicalAddress[1])
							{
								//sendswitch();
								//setFixedPhysicalAddress(getconfigint("cec_fixedAddress", NULL));
								//reportPhysicalAddress(0);
								//sendMenuInfo(0x00);
								//setVolumeForward();
							}
							break;
						}
						case 0x82:
						{
						//cecon = 0;
						//sendTVon();
							if(rxmessage.data[1]== cec_physicalAddress[0] && rxmessage.data[2]== cec_physicalAddress[1])
							{
								sendswitch();
								setFixedPhysicalAddress(getconfigint("cec_fixedAddress", NULL));
								reportPhysicalAddress(0);
								sendMenuInfo(0x00);
								setVolumeForward();
							}
							else if(rxmessage.data[1]== 0x00 && rxmessage.data[2]== 0x00 && sreq == 1)
							{
								sendswitch();
								setFixedPhysicalAddress(getconfigint("cec_fixedAddress", NULL));
								reportPhysicalAddress(0);
								sendMenuInfo(0x00);
								setVolumeForward();
								sreq = 0;
							}
							break;
						}
						case 0x00: /* feature abort */
						{
							if(rxmessage.data[1] == 0x44 || rxmessage.data[1] == 0x7d)
							{
								addconfiginttmp("ForwardVolume", 0);
								printf("HdmiCec: volume forwarding not supported by device %02x\n", rxmessage.address);
							}
							break;
						}
						case 0x44: /* key pressed */
						{
							keypressed = 1;
							pressedkey = rxmessage.data[1];
							transkey = translateKey(pressedkey);
							writerc(transkey);
							break;
						}
						case 0x45: /* key released */
						{
							printf("HdmiCec: key received  -- %02x\n", pressedkey);
							transkey = translateKey(pressedkey);
							printf("HdmiCec: key TitanCode -- %i\n", transkey);
							//writerc(transkey);
							keypressed = 0;
							break;
						}
						case 0x83: /* request address */
						{
							reportPhysicalAddress(1);
							break;
						}
						case 0x85: /* request active source */
						{
							sreq = 1;
							sendSource(rxmessage.address);
							break;
						}
						case 0x9f: /* request cec version */
						{
							sendVersion(rxmessage.address);
							break;
						}
					}
				}
			}
			usleep(10000);
			continue;
		}
		else //timeout or error
		{
			printf("read error or timeout if ret is 0: ret=%d\n", ret);
			sleep(1);
		}		
	}
	hdmiEventthread = NULL;
	printf("**** stop Thread\n");	
}	


int getPhysicalAddress()
{
	return (cec_physicalAddress[0] << 8) | cec_physicalAddress[1];
}

//void sendMessage(struct cec_message &message)
void sendMessageReal(struct cec_message message)
{
	int i = 0;
	if (status.hdmiFd >= 0)
	{
		printf("HdmiCEC: send message\n");
		printf("%02x ", message.address);
		for(i = 0; i < message.length; i++)
		{
			printf(" %02X", message.data[i]);
		}
		printf("\n");
		int ret = 0;
#ifdef DREAMBOX
		message.flag = 1;
		ret = ioctl(status.hdmiFd, 3, &message);
		if(ret == 0)
			ret = 1 + message.length;
#else
		ret = write(status.hdmiFd, &message, 2 + message.length);
#endif
		printf("%i Bytes wurden gesendet\n", ret);
	}
}

void sendMessageReal_thread(struct stimerthread* timernode, struct cec_message* message, int flag)
{
	m_lock(&status.cecmutex, 26);
	int i = 0;
	if (status.hdmiFd >= 0)
	{
		printf("HdmiCEC: send message\n");
		printf("%02x ", message->address);
		for(i = 0; i < message->length; i++)
		{
			printf(" %02X", message->data[i]);
		}
		printf("\n");
		int ret = 0;
#ifdef DREAMBOX
		message->flag = 1;
		ret = ioctl(status.hdmiFd, 3, message);
		if(ret == 0)
			ret = 1 + message->length;
#else
		ret = write(status.hdmiFd, message, 2 + message->length);
#endif
		printf("%i Bytes wurden gesendet\n", ret);
	}
	free(message); message=NULL;
	usleep(100000);
	m_unlock(&status.cecmutex, 26);
}

void sendMessageReal_add(struct cec_message message)
{
	struct cec_message* smessage;
	smessage = calloc(1, sizeof(struct cec_message));
	if(smessage == NULL)
	{
		err("no mem");
		return;
	}
	memcpy(smessage, &message, sizeof(struct cec_message));
	addtimer(&sendMessageReal_thread, START, 1000, 1, (void*)smessage, NULL, NULL);
}
	

void sendMessage(unsigned char address, unsigned char cmd, char *data, int length)
{
	struct cec_message message;
	message.address = address;
	if (length > (int)(sizeof(message.data) - 1)) length = sizeof(message.data) - 1;
	message.length = length + 1;
	message.data[0] = cmd;
	memcpy(&message.data[1], data, length);
	sendMessageReal_add(message);
}

void reportPhysicalAddress(int flag)
{
	struct cec_message txmessage;
	txmessage.address = 0x0f; /* broadcast */
	txmessage.data[0] = 0x84; /* report address */
	txmessage.data[1] = cec_physicalAddress[0];
	txmessage.data[2] = cec_physicalAddress[1];
	txmessage.data[3] = cec_deviceType;
	txmessage.length = 4;
	sendMessageReal_add(txmessage);
}


void getAddressInfo()
{
	int hasdata = 0;
	
	if (status.hdmiFd >= 0)
	{
#if DREAMBOX
		struct
		{
			unsigned char physical[2];
			unsigned char logical;
			unsigned char type;
		} addressinfo;

		if (ioctl(status.hdmiFd, 1, &addressinfo) >= 0)
		{
			hasdata = 1;
			/* we do not get the device type, check the logical address to determine the type */
			switch (addressinfo.logical)
			{
			case 0x1:
			case 0x2:
			case 0x9:
				addressinfo.type = 1; /* recorder */
				break;
			case 0x3:
			case 0x6:
			case 0x7:
			case 0xa:
				addressinfo.type = 3; /* tuner */
				break;
			case 0x4:
			case 0x8:
			case 0xb:
				addressinfo.type = 4; /* playback */
				break;
			}
		}
#else
		struct
		{
			unsigned char logical;
			unsigned char physical[2];
			unsigned char type;
		} addressinfo;
		if (ioctl(status.hdmiFd, 1, &addressinfo) < 0)
		{
			printf("ERROR: cec read AddressInfo");
			return;
		}
		else
			hasdata = 1;
#endif
		if(hasdata == 1)
		{
			cec_deviceType = addressinfo.type;
			cec_logicalAddress = addressinfo.logical;
			printf("HdmiCEC: detected deviceType: %02X\n", addressinfo.type);
			printf("HdmiCEC: detected logical address: %02X\n", addressinfo.logical);
			printf("HdmiCEC: detected physical address: %02X%02X\n", addressinfo.physical[0], addressinfo.physical[1]);
			if (status.cec_fixedAddress == 0)
			{
				//if(cecon > 0)
				if (memcmp(cec_physicalAddress, addressinfo.physical, sizeof(cec_physicalAddress)))
				{
					printf("HdmiCEC: detected physical address change: %02X%02X --> %02X%02X\n", cec_physicalAddress[0], cec_physicalAddress[1], addressinfo.physical[0], addressinfo.physical[1]);
					memcpy(cec_physicalAddress, addressinfo.physical, sizeof(cec_physicalAddress));
					reportPhysicalAddress(0);
					/* emit */ //addressChanged((cec_physicalAddress[0] << 8) | cec_physicalAddress[1]);
				}
			}
		}
	}
}
		
void screencec()
{
	struct skin* tmp = NULL;
	struct skin* cec = getscreen("cec");
	struct skin* listbox = getscreennode(cec, "listbox");
	struct skin* workpink = getscreennode(cec, "workpink");
	struct skin* cecon = getscreennode(cec, "cecon");
	struct skin* alloff = getscreennode(cec, "alloff");
	struct skin* allon = getscreennode(cec, "allon");
	struct skin* tvoff = getscreennode(cec, "tvoff");
	struct skin* tvon = getscreennode(cec, "tvon");
	struct skin* recoff = getscreennode(cec, "recoff");
	struct skin* recon = getscreennode(cec, "recon");
	struct skin* tvswitch = getscreennode(cec, "tvswitch");
	struct skin* cecfix = getscreennode(cec, "cecfix");
	struct skin* volume = getscreennode(cec, "volume");
	struct skin* menue = getscreennode(cec, "menue");
	int rcret = 0;
	int help = 0;	
	
	addchoicebox(workpink, "0", _("off"));
	addchoicebox(workpink, "1", _("on"));
	setchoiceboxselection(workpink, getconfig("workpink", NULL));
	
	addchoicebox(cecon, "0", _("no"));
	addchoicebox(cecon, "1", _("yes"));
	setchoiceboxselection(cecon, getconfig("cec_on", NULL));
	
	addchoicebox(alloff, "0", _("no"));
	addchoicebox(alloff, "1", _("yes"));
	setchoiceboxselection(alloff, getconfig("cec_all_off", NULL));
	
	addchoicebox(allon, "0", _("no"));
	addchoicebox(allon, "1", _("yes"));
	setchoiceboxselection(allon, getconfig("cec_all_on", NULL));
	
	addchoicebox(tvoff, "0", _("no"));
	addchoicebox(tvoff, "1", _("yes"));
	setchoiceboxselection(tvoff, getconfig("cec_tv_off", NULL));
	
	addchoicebox(tvon, "0", _("no"));
	addchoicebox(tvon, "1", _("yes"));
	setchoiceboxselection(tvon, getconfig("cec_tv_on", NULL));
	
	addchoicebox(recoff, "0", _("no"));
	addchoicebox(recoff, "1", _("yes"));
	setchoiceboxselection(recoff, getconfig("cec_rec_off", NULL));
	
	addchoicebox(recon, "0", _("no"));
	addchoicebox(recon, "1", _("yes"));
	setchoiceboxselection(recon, getconfig("cec_rec_on", NULL));
	
	addchoicebox(tvswitch, "0", _("no"));
	addchoicebox(tvswitch, "1", _("yes"));
	setchoiceboxselection(tvswitch, getconfig("cec_tv_switch", NULL));
	
	addchoicebox(cecfix, "0", _("no"));
	addchoicebox(cecfix, "1", _("yes"));
	if(getconfigint("cec_fixedAddress", NULL) == 0)
		setchoiceboxselection(cecfix, "0");
	else
		setchoiceboxselection(cecfix, "1");
	
	addchoicebox(volume, "0", _("none"));
	addchoicebox(volume, "1", _("Receiver"));
	addchoicebox(volume, "2", _("TV"));
	setchoiceboxselection(volume, getconfig("cec_volume", NULL));
	
	addchoicebox(menue, "0", _("off"));
	addchoicebox(menue, "1", _("on"));
	setchoiceboxselection(menue, getconfig("cec_menue", NULL));
	
	
	if(getconfigint("cec_on", NULL) == 0)
	{
		workpink->hidden = YES;
		alloff->hidden = YES;	
		allon->hidden = YES;		
		tvoff->hidden = YES;	
		tvon->hidden = YES;		
		recoff->hidden = YES;	
		recon->hidden = YES;	
		tvswitch->hidden = YES;	
		cecfix->hidden = YES;
		volume->hidden = YES;
		menue->hidden = YES;
	}
	else
	{
		workpink->hidden = NO;
		alloff->hidden = NO;	
		allon->hidden = NO;		
		tvoff->hidden = NO;	
		tvon->hidden = NO;		
		recoff->hidden = NO;	
		recon->hidden = NO;	
		tvswitch->hidden = NO;
		cecfix->hidden = NO;	
		volume->hidden = NO;
		menue->hidden = NO;
	}

	drawscreen(cec, 0, 0);
	addscreenrc(cec, listbox);
	tmp = listbox->select;
	while(1)
	{
		addscreenrc(cec, tmp);
		rcret = waitrc(cec, 0, 0);
		tmp = listbox->select;
		
		if((rcret == getrcconfigint("rcleft", NULL) || rcret == getrcconfigint("rcright", NULL)) && listbox->select != NULL && ostrcmp(listbox->select->name, "cecon") == 0)
		{
			if(ostrcmp(cecon->ret, "0") == 0)
			{
				workpink->hidden = YES;
				alloff->hidden = YES;	
				allon->hidden = YES;		
				tvoff->hidden = YES;	
				tvon->hidden = YES;		
				recoff->hidden = YES;	
				recon->hidden = YES;	
				tvswitch->hidden = YES;
				cecfix->hidden = YES;	
				volume->hidden = YES;
				menue->hidden = YES;
			}
			else
			{
				workpink->hidden = NO;
				alloff->hidden = NO;	
				allon->hidden = NO;		
				tvoff->hidden = NO;	
				tvon->hidden = NO;		
				recoff->hidden = NO;	
				recon->hidden = NO;	
				tvswitch->hidden = NO;	
				cecfix->hidden = NO;	
				volume->hidden = NO;
				menue->hidden = NO;
			}
		}
		if(rcret == getrcconfigint("rcexit", NULL))
			break;
		if(rcret == getrcconfigint("rcok", NULL))	
		{
			addconfig("workpink", workpink->ret);
			addconfig("cec_on", cecon->ret);
			addconfig("cec_all_off", alloff->ret);	
			addconfig("cec_all_on", allon->ret);
			addconfig("cec_tv_off", tvoff->ret);	
			addconfig("cec_tv_on", tvon->ret);
			addconfig("cec_rec_off", recoff->ret);	
			addconfig("cec_rec_on", recon->ret);
			addconfig("cec_tv_switch", tvswitch->ret);
			addconfig("cec_volume", volume->ret);
			addconfig("cec_menue", menue->ret);
			if(ostrcmp(cecfix->ret, "0") == 0)
			{
				addconfig("cec_fixedAddress", "0");
				//setFixedPhysicalAddress(0);
			}
			else
			{
				help = 0;
				help = (cec_physicalAddress[0] << 8) | cec_physicalAddress[1] ;
				addconfigint("cec_fixedAddress", help);
				//setFixedPhysicalAddress(help);
			}
			if(getconfigint("cec_activ", NULL) == 0 || getconfigint("cec_on", NULL) == 0)
				cecinit();
			break;
		}
		drawscreen(cec, 0, 0);
	}
	delownerrc(cec);
	clearscreen(cec);
}

void setFixedPhysicalAddress(int address)
{
	if (address > 0)
	{
		status.cec_fixedAddress = 1;
		cec_physicalAddress[0] = (address >> 8) & 0xff;
		cec_physicalAddress[1] = address & 0xff;
		/* report our (possibly new) address */
		reportPhysicalAddress(0);
	}
	else
	{
		status.cec_fixedAddress = 0;
		/* get our current address */
		getAddressInfo();
	}
}		

void cecinit()
{
	status.cec_fixedAddress = 0;
	status.hdmiFd = -1;
	cec_physicalAddress[0] = 0x10;
	cec_physicalAddress[1] = 0x00;
	cec_logicalAddress = 1;
	cec_deviceType = 1; /* default: recorder */
	
	sreq = 0;
	
	if(getconfigint("cec_on", NULL) == 0)
	{
		addconfiginttmp("cec_activ", 0);
		if(hdmiEventthread != NULL)
			hdmiEventthread->aktion = STOP;	
		return;
	}
	addconfiginttmp("cec_activ", 1);
	
	
#ifdef DREAMBOX	
	status.hdmiFd = open("/dev/misc/hdmi_cec0", O_RDWR | O_NONBLOCK);
	unsigned int val = 0;
	ioctl(status.hdmiFd, 4, &val);
#else
	status.hdmiFd = open("/dev/hdmi_cec", O_RDWR | O_NONBLOCK);
	ioctl(status.hdmiFd, 0); /* flush old messages */
#endif
	
	if(hdmiEventthread == NULL)
		hdmiEventthread = addtimer(&hdmiEvent, START, 10000, 1, NULL, NULL, NULL);
		
	getAddressInfo();
	
	if(checkbox("DM900") == 1 && getwaswakuptimer() != 1)
	{	
		sleep(2);
		cecwakeup();
	}
	
	return;
}

void cecstandby()
{
	char data[1];
	unsigned char address = 0x00;
	unsigned char cmd = 0x36;
	
	data[0] = '\0';
	
	if(getconfigint("cec_on", NULL) == 1)
	{
		if(getconfigint("cec_all_off", NULL) == 1)
		{
			address = 0x0f;
			sendMessage(address, cmd, data, strlen(data));
		}
		if(getconfigint("cec_tv_off", NULL) == 1)
		{
			address = 0x00;
			sendMessage(address, cmd, data, strlen(data));
			//sleep(1);
		}
		if(getconfigint("cec_rec_off", NULL) == 1)
		{
			address = 0x05;
			sendMessage(address, cmd, data, strlen(data));
			//sleep(1);
		}
	}
}

void cecwakeup()
{
	char data[3];
	unsigned char address = 0x00;
	unsigned char cmd = 0x04;
	
	data[0] = '\0';
	
	if(getconfigint("cec_on", NULL) == 1 && cecon == 0)
	{
		if(getconfigint("cec_all_on", NULL) == 1)
		{
			address = 0x0f;
			sendMessage(address, cmd, data, 0);
			//sleep(1);
		}
		if(getconfigint("cec_rec_on", NULL) == 1)
		{
			address = 0x05;
			sendMessage(address, cmd, data, 0);
			//sleep(1);
		}
		if(getconfigint("cec_tv_on", NULL) == 1)
		{	
			address = 0x00;
			sendMessage(address, cmd, data, 0);
			//sleep(1);
		}
		if(getconfigint("cec_tv_switch", NULL) == 1)
		{			
			address = 0x0f;
			cmd     = 0x82;
			data[0] = cec_physicalAddress[0];
			data[1] = cec_physicalAddress[1];
			data[2] = '\0';
			sendMessage(address, cmd, data, 2);
			//sleep(1);
		}
		cecon = 1;
		printf("**********wakeup\n");	
	}
	if(cecon < 2)
	{
		//setFixedPhysicalAddress(getconfigint("cec_fixedAddress", NULL));
		//setVolumeForward();
		//sendMenuInfo(0x00);
		cecon = 2;
	}
}

void forwardKey(int key)
{
	char data[3];
	unsigned char address = 0x00;
	unsigned char cmd = 0x44;
	
	data[0] = '\0';
	
	if(getconfigint("cec_volume", NULL) == 1)
			address = 0x05;
	else if(getconfigint("cec_volume", NULL) == 2)
			address = 0x00;
	
	if(key == getrcconfigint("rcvolup", NULL))
		data[0] = 0x41;
	else if(key == getrcconfigint("rcvoldown", NULL))
		data[0] = 0x42;
	else if(key == getrcconfigint("rcmute", NULL))
		data[0] = 0x43;
		
	data[1] = '\0';
	sendMessage(address, cmd, data, 1);
	data[0] = '\0';
	cmd = 0x45;
	sendMessage(address, cmd, data, 0); /* release */
}
	
#endif
