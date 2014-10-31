#ifndef CEC_H
#define CEC_H

void hdmiEvent()
{
	fd_set rfds;
	struct timeval tv;
	struct cec_message rxmessage;
	int ret = 0, len = 0;
	
	while(1)
	{
		tv.tv_sec = 0x7fffffff;
		tv.tv_usec = 0;
		FD_ZERO(&rfds);
		FD_SET(status.hdmiFd, &rfds);

		ret = select(status.hdmiFd + 1, &rfds, NULL, NULL, &tv);
			
		if(ret > 0)
		{
			if(FD_ISSET(status.hdmiFd, &rfds))
			{
				len = read(status.hdmiFd, &rxmessage, sizeof(struct cec_message));

				if(len > 0)
				{
					printf("reveived adress: %02x datalen: %02x cmd: %02x\n", rxmessage.address, rxmessage.length, rxmessage.data[0]);
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
		printf("eHdmiCEC: send message\n");
		printf("%02x ", message.address);
		for(i = 0; i < message.length; i++)
		{
			printf(" %02X", message.data[i]);
		}
		printf("\n");
		int ret = write(status.hdmiFd, &message, 2 + message.length);
		printf("%i Bytes wurden gesendet\n", ret);
	}
}

void sendMessage(unsigned char address, unsigned char cmd, char *data, int length)
{
	struct cec_message message;
	message.address = address;
	if (length > (int)(sizeof(message.data) - 1)) length = sizeof(message.data) - 1;
	message.length = length + 1;
	message.data[0] = cmd;
	memcpy(&message.data[1], data, length);
	sendMessageReal(message);
}

void reportPhysicalAddress()
{
	struct cec_message txmessage;
	txmessage.address = 0x0f; /* broadcast */
	txmessage.data[0] = 0x84; /* report address */
	txmessage.data[1] = cec_physicalAddress[0];
	txmessage.data[2] = cec_physicalAddress[1];
	txmessage.data[3] = cec_deviceType;
	txmessage.length = 4;
	sendMessageReal(txmessage);
	sleep(1);
}


void getAddressInfo()
{
	if (status.hdmiFd >= 0)
	{
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
		cec_deviceType = addressinfo.type;
		cec_logicalAddress = addressinfo.logical;
		printf("eHdmiCEC: detected deviceType: %02X\n", addressinfo.type);
		printf("eHdmiCEC: detected logical address: %02X\n", addressinfo.logical);
		printf("eHdmiCEC: detected physical address: %02X%02X\n", addressinfo.physical[0], addressinfo.physical[1]);
		if (status.cec_fixedAddress == 0)
		{
			if (memcmp(cec_physicalAddress, addressinfo.physical, sizeof(cec_physicalAddress)))
			{
				printf("eHdmiCEC: detected physical address change: %02X%02X --> %02X%02X\n", cec_physicalAddress[0], cec_physicalAddress[1], addressinfo.physical[0], addressinfo.physical[1]);
				memcpy(cec_physicalAddress, addressinfo.physical, sizeof(cec_physicalAddress));
				reportPhysicalAddress();
				/* emit */ //addressChanged((cec_physicalAddress[0] << 8) | cec_physicalAddress[1]);
			}
		}
	}
}
		
void screencec()
{
	struct skin* tmp = NULL;
	struct skin* cec = getscreen("cec");
	struct skin* listbox = getscreennode(cec, "listbox");
	struct skin* cecon = getscreennode(cec, "cecon");
	struct skin* alloff = getscreennode(cec, "alloff");
	struct skin* allon = getscreennode(cec, "allon");
	struct skin* tvoff = getscreennode(cec, "tvoff");
	struct skin* tvon = getscreennode(cec, "tvon");
	struct skin* recoff = getscreennode(cec, "recoff");
	struct skin* recon = getscreennode(cec, "recon");
	struct skin* tvswitch = getscreennode(cec, "tvswitch");
	struct skin* cecfix = getscreennode(cec, "cecfix");
	int rcret = 0;
	int help = 0;	
	
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
		setchoiceboxselection(cecfix, 0);
	else
		setchoiceboxselection(cecfix, 1);
	
	
	if(getconfigint("cec_on", NULL) == 0)
	{
		alloff->hidden = YES;	
		allon->hidden = YES;		
		tvoff->hidden = YES;	
		tvon->hidden = YES;		
		recoff->hidden = YES;	
		recon->hidden = YES;	
		tvswitch->hidden = YES;	
		cecfix->hidden = YES;
	}
	else
	{
		alloff->hidden = NO;	
		allon->hidden = NO;		
		tvoff->hidden = NO;	
		tvon->hidden = NO;		
		recoff->hidden = NO;	
		recon->hidden = NO;	
		tvswitch->hidden = NO;
		cecfix->hidden = NO;	
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
			if(ostrcmp(cecon->ret, "1") == 0)
			{
				alloff->hidden = YES;	
				allon->hidden = YES;		
				tvoff->hidden = YES;	
				tvon->hidden = YES;		
				recoff->hidden = YES;	
				recon->hidden = YES;	
				tvswitch->hidden = YES;
			}
			else
			{
				alloff->hidden = NO;	
				allon->hidden = NO;		
				tvoff->hidden = NO;	
				tvon->hidden = NO;		
				recoff->hidden = NO;	
				recon->hidden = NO;	
				tvswitch->hidden = NO;	
			}
		}
		if(rcret == getrcconfigint("rcexit", NULL))
			break;
		if(rcret == getrcconfigint("rcok", NULL))	
		{
			addconfig("cec_on", cecon->ret);
			addconfig("cec_all_off", alloff->ret);	
			addconfig("cec_all_on", allon->ret);
			addconfig("cec_tv_off", tvoff->ret);	
			addconfig("cec_tv_on", tvon->ret);
			addconfig("cec_rec_off", recoff->ret);	
			addconfig("cec_rec_on", recon->ret);
			addconfig("cec_switch_on", tvswitch->ret);
			if(ostrcmp(cecfix->ret, "0") == 0)
			{
				addconfig("cec_fixedAddress", "0");
				setFixedPhysicalAddress(0);
			}
			else
			{
				help = 0;
				help = (cec_physicalAddress[0] << 8) | cec_physicalAddress[1] ;
				addconfigint("cec_fixedAddress", help);
				setFixedPhysicalAddress(help);
			}
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
		reportPhysicalAddress();
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
	
	status.hdmiFd = open("/dev/hdmi_cec", O_RDWR | O_NONBLOCK);
		
	ioctl(status.hdmiFd, 0); /* flush old messages */

	setFixedPhysicalAddress(getconfigint("cec_fixedAddress", NULL));
	
	return 0;
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
			sleep(1);
		}
		if(getconfigint("cec_tv_off", NULL) == 1)
		{
			address = 0x00;
			sendMessage(address, cmd, data, strlen(data));
			sleep(1);
		}
		if(getconfigint("cec_rec_off", NULL) == 1)
		{
			address = 0x05;
			sendMessage(address, cmd, data, strlen(data));
			sleep(1);
		}
	}
}

void cecwakeup()
{
	char data[3];
	unsigned char address = 0x00;
	unsigned char cmd = 0x04;
	
	data[0] = '\0';
	
	if(getconfigint("cec_on", NULL) == 1)
	{
		if(getconfigint("cec_all_on", NULL) == 1)
		{
			address = 0x0f;
			sendMessage(address, cmd, data, 0);
			sleep(1);
		}
		if(getconfigint("cec_rec_on", NULL) == 1)
		{
			address = 0x05;
			sendMessage(address, cmd, data, 0);
			sleep(1);
		}
		if(getconfigint("cec_tv_on", NULL) == 1)
		{	
			address = 0x00;
			sendMessage(address, cmd, data, 0);
			sleep(1);
		}
		if(getconfigint("cec_switch_on", NULL) == 1)
		{			
			address = 0x0f;
			cmd     = 0x82;
			data[0] = cec_physicalAddress[0];
			data[1] = cec_physicalAddress[1];
			data[2] = '\0';
			sendMessage(address, cmd, data, 2);
			sleep(1);
		}
	}
}

#endif
