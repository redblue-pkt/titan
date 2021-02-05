#ifndef NETWORK_H
#define NETWORK_H

int readwlan(const char* filename, char** type, char** ssid, char** key)
{
	FILE *fd = NULL;
	char *fileline = NULL, *tmpstr = NULL;

	fileline = malloc(MINMALLOC);
	if(fileline == NULL)
	{
		err("no memory");
		return 1;
	}

	fd = fopen(filename, "r");
	if(fd == NULL)
	{
		perr("can't open %s", filename);
		free(fileline);
		return 1;
	}

	while(fgets(fileline, MINMALLOC, fd) != NULL)
	{
		if(fileline[0] == '#' || fileline[0] == '\n')
			continue;
		if(fileline[strlen(fileline) - 1] == '\n')
			fileline[strlen(fileline) - 1] = '\0';
		if(fileline[strlen(fileline) - 1] == '\r')
			fileline[strlen(fileline) - 1] = '\0';

		tmpstr = ostrstrcase(fileline, "proto=");
		if(tmpstr != NULL)
		{
			tmpstr += 6;
			if(ostrcmp(tmpstr, "WPA") == 0)
			{
				free(*type); *type = NULL;
				*type = ostrcat("2", NULL, 0, 0);
			}
			if(ostrcmp(tmpstr, "RSN") == 0)
			{
				free(*type); *type = NULL;
				*type = ostrcat("3", NULL, 0, 0);
			}
		}
		tmpstr = ostrstrcase(fileline, "ssid=\"");
		if(tmpstr != NULL)
		{
			tmpstr += 6;
			if(tmpstr != NULL)
			{
				tmpstr[strlen(tmpstr) -1] = '\0';
				free(*ssid); *ssid = NULL;
				*ssid = ostrcat(tmpstr, NULL, 0, 0);
			}
		}
		tmpstr = ostrstrcase(fileline, "psk=\"");
		if(tmpstr != NULL)
		{
			tmpstr += 5;
			if(tmpstr != NULL)
			{
				tmpstr[strlen(tmpstr) -1] = '\0';
				free(*key); *key = NULL;
				*key = ostrcat(tmpstr, NULL, 0, 0);
			}
		}
		tmpstr = ostrstrcase(fileline, "wep_key0=");
		if(tmpstr != NULL)
		{
			tmpstr += 9;
			if(tmpstr != NULL)
			{
				tmpstr[strlen(tmpstr)] = '\0';
				free(*key); *key = NULL;
				*key = ostrcat(tmpstr, NULL, 0, 0);
				free(*type); *type = NULL;
				*type = ostrcat("1", NULL, 0, 0);
			}
		}
	}

	free(fileline);
	fclose(fd);
	return 0;
}

int writewlan(const char* filename, int type, char* ssid, char* key)
{
	char* savesettings = NULL;

	char* tmpstr = "\nkey_mgmt=NONE\nscan_ssid=1";

	if(type == 1) //WEP
		tmpstr = "\nkey_mgmt=NONE\nscan_ssid=1\nwep_tx_keyidx=0\nwep_key0=";
	if(type == 2) //WPA
		tmpstr = "\nkey_mgmt=WPA-PSK\nscan_ssid=1\nproto=WPA\npsk=";
	if(type == 3) //WPA2
		tmpstr = "\nkey_mgmt=WPA-PSK\nscan_ssid=1\nproto=RSN\npsk=";

	savesettings = ostrcat("ctrl_interface=DIR=/var/run/wpa_supplicant\nnetwork={\nssid=\"", NULL, 0, 0);
	savesettings = ostrcat(savesettings, ssid, 1, 0);
	savesettings = ostrcat(savesettings, "\"", 1, 0);
	savesettings = ostrcat(savesettings, tmpstr, 1, 0);

	if(type == 2 || type == 3)
		savesettings = ostrcat(savesettings, "\"", 1, 0);
	if(type == 1 || type == 2 || type == 3)
		savesettings = ostrcat(savesettings, key, 1, 0);
	if(type == 2 || type == 3)
		savesettings = ostrcat(savesettings, "\"", 1, 0);

	savesettings = ostrcat(savesettings, "\n}", 1, 0);

	FILE* fd = fopen(filename, "w");
	if(fd)
	{
		fprintf(fd, "%s\n", savesettings);
		fclose(fd);
	}
	free(savesettings); savesettings = NULL;

	return 0;
}

int writeinterfaces()
{
	char* savesettings = NULL;
	struct inetwork* net = inetwork;

	savesettings = ostrcat(savesettings, "# automatically generated by titan\n", 1, 0);
	savesettings = ostrcat(savesettings, "# do NOT change manually!\n", 1, 0);
	savesettings = ostrcat(savesettings, "\n", 1, 0);
	savesettings = ostrcat(savesettings, "auto lo\n", 1, 0);
	savesettings = ostrcat(savesettings, "iface lo inet loopback\n", 1, 0);

	while(net != NULL)
	{
		if(ostrncmp(net->device, "lo", 2) == 0)
		{
			net = net->next;
			continue;
		}
		if(net->type == 0 && (net->ip == NULL || ostrcmp(net->ip, "000.000.000.000") == 0))
		{
			net = net->next;
			continue;
		}

		savesettings = ostrcat(savesettings, "\n", 1, 0);
		savesettings = ostrcat(savesettings, "auto ", 1, 0);
		savesettings = ostrcat(savesettings, net->device, 1, 0);
		savesettings = ostrcat(savesettings, "\n", 1, 0);

		savesettings = ostrcat(savesettings, "iface ", 1, 0);
		savesettings = ostrcat(savesettings, net->device, 1, 0);

		if(net->type == 1)
			savesettings = ostrcat(savesettings, " inet dhcp\n", 1, 0);
		else if(net->type == 2)
#ifdef OEBUILD
			savesettings = ostrcat(savesettings, " inet manual\n", 1, 0);
#else
			savesettings = ostrcat(savesettings, " inet off\n", 1, 0);
#endif
		else
		{
			savesettings = ostrcat(savesettings, " inet static\n", 1, 0);

			savesettings = ostrcat(savesettings, "\taddress ", 1, 0);
			savesettings = ostrcat(savesettings, fixip(net->ip, 1), 1, 1);
			if(net->netmask != NULL && ostrcmp(net->netmask, "000.000.000.000") != 0)
			{
				savesettings = ostrcat(savesettings, "\n\tnetmask ", 1, 0);
				savesettings = ostrcat(savesettings, fixip(net->netmask, 1), 1, 1);
			}

			if(status.gateway != NULL && ostrcmp(status.gateway, "000.000.000.000") != 0)
			{
				savesettings = ostrcat(savesettings, "\n\tgateway ", 1, 0);
				savesettings = ostrcat(savesettings, fixip(status.gateway, 1), 1, 1);
			}
			savesettings = ostrcat(savesettings, "\n", 1, 0);
		}
#ifdef OEBUILD
		if(ostrncmp(net->device, "wlan", 4) == 0)
		{
			savesettings = ostrcat(savesettings, "\n\tpre-up wpa_supplicant -i", 1, 0);
			savesettings = ostrcat(savesettings, net->device, 1, 0);
			savesettings = ostrcat(savesettings, " -c/mnt/network/wpa_supplicant.conf -B -dd -Dwext || true", 1, 0);
//			savesettings = ostrcat(savesettings, " -c/mnt/network/wpa_supplicant.", 1, 0);
//			savesettings = ostrcat(savesettings, net->device, 1, 0);
//			savesettings = ostrcat(savesettings, ".conf -B -dd -Dwext || true", 1, 0);

			savesettings = ostrcat(savesettings, "\n\tpre-down wpa_cli -i", 1, 0);
			savesettings = ostrcat(savesettings, net->device, 1, 0);
			savesettings = ostrcat(savesettings, " terminate || true", 1, 0);
		}
#endif
		int dnscount = 0;
		if(net->type == 0)
		{
			char* savedns = NULL;
			if(status.dnsserver1 != NULL && ostrcmp(status.dnsserver1, "000.000.000.000") != 0)
			{
				dnscount = 1;
				savedns = ostrcat(savedns, "nameserver ", 1, 0);
				savedns = ostrcat(savedns, fixip(status.dnsserver1, 1), 1, 1);
			}

			if(status.dnsserver2 != NULL && ostrcmp(status.dnsserver2, "000.000.000.000") != 0)
			{
				dnscount = 2;
				savedns = ostrcat(savedns, "\n", 1, 0);
				savedns = ostrcat(savedns, "nameserver ", 1, 0);
				savedns = ostrcat(savedns, fixip(status.dnsserver2, 1), 1, 1);
			}

			if(savedns != NULL)
			{
				debug(55, "[NETWORK] save resolv.conf: %s\n", savedns);

				savedns = ostrcat(savedns, "\n", 1, 0);
				if(dnscount == 1)
					savedns = ostrcat(savedns, "options timeout:3 attempts:1", 1, 0);
				else
					savedns = ostrcat(savedns, "options timeout:2 attempts:1", 1, 0);


				FILE* fd1 = fopen("/mnt/network/resolv.conf", "w");
				if(fd1)
				{
					fprintf(fd1, "%s\n", savedns);
					fclose(fd1);
				}
				free(savedns); savedns = NULL;
			}
		}
		net = net->next;
	}

	debug(55, "[NETWORK] save interfaces:\n %s", savesettings);

	FILE* fd = fopen("/mnt/network/interfaces", "w");
	if(fd)
	{
		fprintf(fd, "%s\n", savesettings);
		fclose(fd);
	}
	free(savesettings); savesettings = NULL;

	return 0;
}

void screennetwork_test()
{
	int rcret = 0;
	struct skin* network = getscreen("network_test");
	struct skin* internet = getscreennode(network, "internet");
	struct skin* lan = getscreennode(network, "lan");
	char* tmpstr = NULL;

	drawscreen(network, 0, 0);
	addscreenrc(network, lan);
	while(1)
	{
		rcret = waitrc(network, 0, 0);

		if(rcret == getrcconfigint("rcexit", NULL)) break;
		if(rcret == getrcconfigint("rcred", NULL))
		{
			changetext(lan, _("Wait"));
			changetext(internet, _("Wait"));
			drawscreen(network, 0, 0);

			tmpstr = ostrcat(tmpstr, "ping -c1 -W1 ", 1, 0);
			tmpstr = ostrcat(tmpstr, fixip(status.gateway, 1), 1, 0);
			if(system(tmpstr) == 0)
				changetext(lan, _("OK"));
			else
				changetext(lan, _("ERROR"));
			free(tmpstr); tmpstr=NULL;

			tmpstr = ostrcat(tmpstr, "ping -c1 -W1 ", 1, 0);
			tmpstr = ostrcat(tmpstr, "www.google.de", 1, 0);
			if(system(tmpstr) == 0)
				changetext(internet, _("OK"));
			else
				changetext(internet, _("ERROR"));
			free(tmpstr); tmpstr=NULL;
			drawscreen(network, 0, 0);
		}
	}
	delownerrc(network);
	clearscreen(network);
}

void screennetwork(int mode)
{
	textbox(_("Network"), _("comming soon..."), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 800, 600, 0, 0);
}

//flag 0 and 1: other message
//flag 2: no msg and no info
void screennetwork_restart(struct inetwork *net, int flag)
{
	struct skin* load = getscreen("loading");
	char* tmpstr = NULL, *cmd = NULL;

	if(flag == 0)
		tmpstr = ostrcat(_("Restart Network ?"), NULL, 0, 0);
	else if(flag == 1)
		tmpstr = ostrcat(_("Aktivate new network config ?"), NULL, 0, 0);

	if(flag == 2 || textbox(_("Network"), tmpstr, _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 0, 0) == 1)
	{
		debug(10, "%s", tmpstr);
		free(tmpstr), tmpstr = NULL;

		drawscreen(load, 0, 0);
		if(net == NULL)
		{
			net = inetwork;
			while(net != NULL)
			{
				if(net->flag == 1)
				{
#ifdef OEBUILD
					cmd = ostrcat(cmd, "/etc/init.d/networking restart", 1, 0);
#else
					cmd = ostrcat(cmd, "/etc/init.d/networking -i ", 1, 0);
					cmd = ostrcat(cmd, net->device, 1, 0);
					cmd = ostrcat(cmd, " restart", 1, 0);
#endif
					tmpstr = ostrcat(tmpstr, command(cmd), 1, 1);
					tmpstr = ostrcat(tmpstr, "\n\n", 1, 0);
					free(cmd); cmd = NULL;
				}
				net = net->next;
			}
		}
		else
		{
#ifdef OEBUILD
			cmd = ostrcat(cmd, "/etc/init.d/networking restart", 1, 0);
#else
			cmd = ostrcat(cmd, "/etc/init.d/networking -i ", 1, 0);
			cmd = ostrcat(cmd, net->device, 1, 0);
			cmd = ostrcat(cmd, " restart", 1, 0);
#endif
			tmpstr = ostrcat(tmpstr, command(cmd), 1, 1);
			free(cmd); cmd = NULL;
		}

		addinetworkall(NULL);
		clearscreen(load);

		if(flag != 2)
			textbox(_("Restart Network"), tmpstr, _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 800, 600, 0, 0);
		free(tmpstr); tmpstr = NULL;
	}
}

void nethidden(struct skin* ipaddress, struct skin* netmask, struct skin* gateway, struct skin* dnsserver1, struct skin* dnsserver2, int type)
{
	ipaddress->hidden = type;
	netmask->hidden = type;
	gateway->hidden = type;
	dnsserver1->hidden = type;
	dnsserver2->hidden = type;
}

void screennetwork_adapterext(int mode, char* interface)
{
	int rcret = 0;
	struct skin* network = getscreen("network");
	struct skin* listbox = getscreennode(network, "listbox");
	struct skin* type = getscreennode(network, "type");
	struct skin* ipaddress = getscreennode(network, "ipaddress");
	struct skin* netmask = getscreennode(network, "netmask");
	struct skin* gateway = getscreennode(network, "gateway");
	struct skin* dnsserver1 = getscreennode(network, "dnsserver1");
	struct skin* dnsserver2 = getscreennode(network, "dnsserver2");
	struct inetwork* net = getinetworkbydevice(interface);
	struct skin* wol = getscreennode(network, "wol");

	struct skin* tmp = NULL;

	if(net == NULL)
		return;

	char* tmp_ipaddress = ostrcat(net->ip, NULL, 0, 0);
	char* tmp_netmask = ostrcat(net->netmask, NULL, 0, 0);;
	char* tmp_gateway = ostrcat(status.gateway, NULL, 0, 0);;
	char* tmp_dnsserver1 = ostrcat(status.dnsserver1, NULL, 0, 0);;
	char* tmp_dnsserver2 = ostrcat(status.dnsserver2, NULL, 0, 0);;
	int tmp_type = net->type;

	changemask(ipaddress, "000.000.000.000");
	changeinput(ipaddress, net->ip);

	changemask(netmask, "000.000.000.000");
	changeinput(netmask, net->netmask);

	changemask(gateway, "000.000.000.000");
	changeinput(gateway, status.gateway);

	changemask(dnsserver1, "000.000.000.000");
	changeinput(dnsserver1, status.dnsserver1);

	changemask(dnsserver2, "000.000.000.000");
	changeinput(dnsserver2, status.dnsserver2);

	addchoicebox(type, "1", _("dhcp"));
	addchoicebox(type, "0", _("static"));
	addchoicebox(type, "2", _("off"));

	addchoicebox(wol, "0", _("disable"));
	addchoicebox(wol, "1", _("enable"));
	setchoiceboxselection(wol, getconfig("wol", NULL));

	if(ostrcmp(interface, "eth0") != 0 || file_exist("/proc/stb/fp/wol") == 0)
		wol->hidden = YES;
	else
		wol->hidden = NO;

	if(net->type == 1)
	{
		setchoiceboxselection(type, "1");
		nethidden(ipaddress, netmask, gateway, dnsserver1, dnsserver2, YES);
	}
	else if(net->type == 2)
	{
		setchoiceboxselection(type, "2");
		nethidden(ipaddress, netmask, gateway, dnsserver1, dnsserver2, YES);
	}
	else
	{
		setchoiceboxselection(type, "0");
		nethidden(ipaddress, netmask, gateway, dnsserver1, dnsserver2, NO);
	}

	listbox->aktline = 1;

	drawscreen(network, 0, 0);
	addscreenrc(network, listbox);

	int save = 0;

	tmp = listbox->select;
	while(1)
	{
		addscreenrc(network, tmp);
		rcret = waitrc(network, 0, 0);
		tmp = listbox->select;

		if(rcret == getrcconfigint("rcexit", NULL)) break;
		if(rcret == getrcconfigint("rcok", NULL))
		{
			if(textbox(_("Message"), _("save network settings ?"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 0, 0) == 1)
			{
				debug(10, "save network settings");
				save = 1;
				break;
			}
			drawscreen(network, 0, 0);
		}

		if(listbox->select != NULL)
		{
			if(ostrcmp(listbox->select->name, "type") == 0)
			{
				if(atoi(listbox->select->ret) == 0)
					nethidden(ipaddress, netmask, gateway, dnsserver1, dnsserver2, NO);
				else
					nethidden(ipaddress, netmask, gateway, dnsserver1, dnsserver2, YES);

				drawscreen(network, 0, 0);
				tmp_type = atoi(listbox->select->ret);
			}
			else if(ostrcmp(listbox->select->name, "ipaddress") == 0)
			{
				free(tmp_ipaddress);
				tmp_ipaddress = ostrcat(listbox->select->ret, NULL, 0, 0);
			}
			else if(ostrcmp(listbox->select->name, "netmask") == 0)
			{
				free(tmp_netmask);
				tmp_netmask = ostrcat(listbox->select->ret, NULL, 0, 0);
			}
			else if(ostrcmp(listbox->select->name, "gateway") == 0)
			{
				free(tmp_gateway);
				tmp_gateway = ostrcat(listbox->select->ret, NULL, 0, 0);
			}
			else if(ostrcmp(listbox->select->name, "dnsserver1") == 0)
			{
				free(tmp_dnsserver1);
				tmp_dnsserver1 = ostrcat(listbox->select->ret, NULL, 0, 0);
			}
			else if(ostrcmp(listbox->select->name, "dnsserver2") == 0)
			{
				free(tmp_dnsserver2);
				tmp_dnsserver2 = ostrcat(listbox->select->ret, NULL, 0, 0);
			}
		}
	}

	if(save == 1)
	{
		if(ostrcmp(interface, "eth0") == 0 && file_exist("/proc/stb/fp/wol") != 0)
		{
			addconfig("wol",wol->ret);
			if(getconfigint("wol", NULL) == 1)
				system("echo enable > /proc/stb/fp/wol");
			else
				system("echo disable > /proc/stb/fp/wol");
		}

		debug(55, "save settings");
		debug(55, "type: %i", tmp_type);
		debug(55, "ipaddress: %s", tmp_ipaddress);
		debug(55, "netmask: %s", tmp_netmask);
		debug(55, "gateway: %s", tmp_gateway);
		debug(55, "dnsserver1: %s", tmp_dnsserver1);
		debug(55, "dnsserver2: %s", tmp_dnsserver2);

		if(ostrcmp(net->ip, tmp_ipaddress) != 0)
		{
			free(net->ip);
			net->ip = ostrcat(tmp_ipaddress, NULL, 0, 0);
		}
		if(ostrcmp(net->netmask, tmp_netmask) != 0)
		{
			free(net->netmask);
			net->netmask = ostrcat(tmp_netmask, NULL, 0, 0);
		}
		if(ostrcmp(status.gateway, tmp_gateway) != 0)
		{
			free(status.gateway);
			status.gateway = ostrcat(tmp_gateway, NULL, 0, 0);
		}
		if(ostrcmp(status.dnsserver1, tmp_dnsserver1) != 0)
		{
			free(status.dnsserver1);
			status.dnsserver1 = ostrcat(tmp_dnsserver1, NULL, 0, 0);
		}
		if(ostrcmp(status.dnsserver2, tmp_dnsserver2) != 0)
		{
			free(status.dnsserver2);
			status.dnsserver2 = ostrcat(tmp_dnsserver2, NULL, 0, 0);
		}

		if(tmp_type != 0)
		{
			free(net->ip);
			net->ip = ostrcat("000.000.000.000", NULL, 0, 0);
			free(net->netmask);
			net->netmask = ostrcat("000.000.000.000", NULL, 0, 0);
		}

		net->type = tmp_type;

		writeinterfaces();
		screennetwork_restart(net, 1);
	}

	free(tmp_ipaddress);
	free(tmp_netmask);
	free(tmp_gateway);
	free(tmp_dnsserver1);
	free(tmp_dnsserver2);

	delownerrc(network);
	clearscreen(network);
}

void screenhostname()
{
	int ret = 0;
	char* hostname = NULL, *newhostname = NULL, *cmd = NULL;

	hostname = command("hostname");
	hostname = strstrip(hostname);
	newhostname = textinput(NULL, hostname);
	newhostname = strstrip(newhostname);

	if(newhostname != NULL && strlen(newhostname) > 0)
	{
		ret = writesys("/mnt/network/hostname", newhostname, 1);
		if(ret == 0)
		{
			cmd = ostrcat(cmd, "hostname \"", 1, 0);
			cmd = ostrcat(cmd, newhostname, 1, 0);
			cmd = ostrcat(cmd, "\"", 1, 0);
			ret = system(cmd);
		}
	}

	if(ret != 0)
			textbox(_("Message"), _("Can't change hostname !"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 0, 0);

	free(cmd); cmd = NULL;
	free(hostname); hostname = NULL;
	free(newhostname); newhostname = NULL;
}

void screennetwork_adapter()
{
	int rcret = 0, mode = 0;
	struct skin* interfacelist = getscreen("interfacelist");
	struct skin* listbox = getscreennode(interfacelist, "listbox");
	struct inetwork* inetworknode = NULL;
	struct skin* tmp = NULL;
	char* tmpstr = NULL;

start:
	tmp = NULL;
	listbox->aktpage = -1;
	listbox->aktline = 1;
	listbox->select = NULL;
	inetworknode = inetwork;

	while(inetworknode != NULL)
	{
		if(ostrncmp(inetworknode->device, "lo", 2) == 0)
		{
			inetworknode = inetworknode->next;
			continue;
		}
		tmp = addlistbox(interfacelist, listbox, tmp, 1);
		if(tmp != NULL)
		{
			tmpstr = ostrcat(inetworknode->device, " (", 0, 0);
			printf("inetworknode->device: %s inetworknode->ip: %s inetworknode->type: %d\n", inetworknode->device, inetworknode->ip, inetworknode->type);
			if(inetworknode->type == 2)
				tmpstr = ostrcat(tmpstr, _("disable"), 1, 0);
			else
				tmpstr = ostrcat(tmpstr, inetworknode->ip, 1, 0);
			tmpstr = ostrcat(tmpstr, ")", 1, 0);
			changetext(tmp, tmpstr);
			free(tmpstr); tmpstr = NULL;
			tmp->handle = inetworknode->device;
		}
		inetworknode = inetworknode->next;
	}

	drawscreen(interfacelist, 0, 0);
	addscreenrc(interfacelist, listbox);

	tmp = listbox->select;
	while(1)
	{
		addscreenrc(interfacelist, tmp);
		rcret = waitrc(interfacelist, 0, 0);
		tmp = listbox->select;

		if(rcret == getrcconfigint("rcexit", NULL)) break;
		if(listbox->select != NULL)
		{
			if(rcret == getrcconfigint("rcred", NULL))
			{
				clearscreen(interfacelist);
				screennetwork_adapterext(mode, listbox->select->handle);

				delmarkedscreennodes(interfacelist, 1);
				goto start;
			}
			if(rcret == getrcconfigint("rcgreen", NULL))
			{
				screenhostname();
				drawscreen(interfacelist, 0, 0);
			}
		}
	}

	delmarkedscreennodes(interfacelist, 1);
	delownerrc(interfacelist);
	clearscreen(interfacelist);
}

int wlanstart()
{
	int ret = 0;
#ifndef OEBUILD
	system("killall wpa_supplicant; sleep 2; killall -9 wpa_supplicant");
	ret = system("wlan.sh");
#endif

	addinetworkall(NULL);

	return ret;
}

void screennetwork_wlan()
{
	int rcret = -1, scan = 0, ret = 0;
	struct skin* wlan = getscreen("wlansettings");
	struct skin* listbox = getscreennode(wlan, "listbox");
	struct skin* startmode = getscreennode(wlan, "startmode");
	struct skin* ssid = getscreennode(wlan, "ssid");
	struct skin* type = getscreennode(wlan, "type");
	struct skin* key = getscreennode(wlan, "key");
	struct skin* b3 = getscreennode(wlan, "b3");
	struct skin* b4 = getscreennode(wlan, "b4");
	struct skin* b5 = getscreennode(wlan, "b5");
	struct skin* b6 = getscreennode(wlan, "b6");
	struct skin* b7 = getscreennode(wlan, "b7");
	struct skin* load = getscreen("loading");
	struct inetwork* net = NULL;
	struct skin* tmp = NULL, *tmp1 = NULL, *tmp2 = NULL;
	char* tmpstr = NULL;

	char* wtype = NULL, *wssid = NULL, *wkey = NULL;

	readwlan("/mnt/network/wpa_supplicant.conf", &wtype, &wssid, &wkey);

	addchoicebox(startmode, "n", _("no"));
	addchoicebox(startmode, "y", _("yes"));
	setchoiceboxselection(startmode, getownconfig("wlan"));

	changeinput(ssid, wssid);
	free(wssid); wssid = NULL;

	addchoicebox(type, "0", _("NONE"));
	addchoicebox(type, "1", _("WEP"));
	addchoicebox(type, "2", _("WPA"));
	addchoicebox(type, "3", _("WPA2"));
	setchoiceboxselection(type, wtype);
	free(wtype); wtype = NULL;

	changeinput(key, wkey);
	free(wkey); wkey = NULL;

	drawscreen(wlan, 0, 0);
	addscreenrc(wlan, listbox);

	tmp = listbox->select;
	tmp2 = listbox->select;
	while(1)
	{
		if(scan == 0) addscreenrc(wlan, tmp);
		rcret = waitrc(wlan, 1000, 0);
		if(scan == 0) tmp = listbox->select;

		if(rcret == getrcconfigint("rcexit", NULL))
		{
			if(scan == 1)
			{
				scan = 0;
				delmarkedscreennodes(wlan, 1);
				startmode->hidden = NO;
				ssid->hidden = NO;
				type->hidden = NO;
				key->hidden = NO;
				b3->hidden = NO;
				b4->hidden = NO;
				b5->hidden = NO;
				b6->hidden = NO;
				b7->hidden = NO;
				listbox->select = NULL;
				listbox->aktline = 1;
				listbox->aktpage = -1;
				tmp = tmp2;
				drawscreen(wlan, 0, 0);
			}
			else
				break;
		}

		if(rcret == getrcconfigint("rcok", NULL) || (scan == 0 && rcret == getrcconfigint("rcgreen", NULL)))
		{
			if(scan == 1)
			{
				if(listbox->select != NULL)
				{
					ssid->linecount = 0;
					ssid->pagecount = 0;
					ssid->poscount = 0;
					ssid->aktpage = 0;
					ssid->aktline = 0;
					changeinput(ssid, listbox->select->text);
				}
				scan = 0;
				delmarkedscreennodes(wlan, 1);
				startmode->hidden = NO;
				ssid->hidden = NO;
				type->hidden = NO;
				key->hidden = NO;
				b3->hidden = NO;
				b4->hidden = NO;
				b5->hidden = NO;
				b6->hidden = NO;
				b7->hidden = NO;
				listbox->select = NULL;
				listbox->aktline = 1;
				listbox->aktpage = -1;
				tmp = tmp2;
				drawscreen(wlan, 0, 0);
			}
			else
			{
				int tmptype = 0;
				if(type->ret != NULL) tmptype = atoi(type->ret);
				writewlan("/mnt/network/wpa_supplicant.conf", tmptype, ssid->ret, key->ret);
				if(startmode->ret != NULL) addownconfig("wlan", startmode->ret);
				if(rcret == getrcconfigint("rcok", NULL))
				{
					net = getinetworkbydevice("eth0");
					if(net != NULL && net->type != 2)
					{
						if(textbox(_("Message"), _("deaktivate ethernet interface ?"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 0, 0) == 1)
						{
							net->type = 2; //deaktivate
							writeinterfaces();
							system("ip addr flush dev eth0 scope global");
							screennetwork_restart(net, 2);
						}
					}

					if(textbox(_("Message"), _("Start Wlan ?"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 0, 0) == 1)
					{
						drawscreen(load, 0, 0);
						ret = wlanstart();
						clearscreen(load);

						if(ret == 0)
						{
							net = getinetworkfirstwlan();
							if(net != NULL)
							{
								net->type = 1; //dhcp
								screennetwork_adapterext(0, net->device);
							}
						}
					}
					break;
				}
			}
			if(rcret == getrcconfigint("rcgreen", NULL))
			{
				drawscreen(load, 0, 0);
				ret = wlanstart();
				clearscreen(load);
				if(ret == 0)
				{
					textbox(_("Message"), _("WLAN started.\n You can now configure the new interface."), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 10, 0);
					screennetwork_restart(NULL, 0);
				}
				else
					textbox(_("Message"), _("WLAN not started,\nPlease check your config."), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 10, 0);
				drawscreen(wlan, 0, 0);
			}
		}

		if(rcret == getrcconfigint("rcred", NULL) && scan == 0)
		{
			tmp1 = NULL;
			scan = 1;
			startmode->hidden = YES;
			ssid->hidden = YES;
			type->hidden = YES;
			key->hidden = YES;
			b3->hidden = YES;
			b4->hidden = YES;
			b5->hidden = YES;
			b6->hidden = YES;
			b7->hidden = YES;
			listbox->select = NULL;
			listbox->aktline = 1;
			listbox->aktpage = -1;
			tmp1 = addlistbox(wlan, listbox, tmp1, 1);
			if(tmp1 != NULL)
				changetext(tmp1, _("searching..."));
			drawscreen(wlan, 0, 0);
#ifndef OEBUILD
			system("wlan.sh notstart");
#endif
			tmpstr = command("iwlist scanning | grep 'ESSID:' | sed 's/ESSID:\\\"\\\"/ESSID:\\\"unknown\\\"/' | sed 's/[ ]*ESSID://' | tr -d \\\"");

			tmp1 = NULL;
			delmarkedscreennodes(wlan, 1);
			if(tmpstr != NULL)
			{
				int count = 0, i = 0;
				struct splitstr* splitret = NULL;

				splitret = strsplit(tmpstr, "\n", &count);
				for(i = 0; i < count; i++)
				{
					tmp1 = addlistbox(wlan, listbox, tmp1, 1);
					if(tmp1 != NULL)
						changetext(tmp1, (&splitret[i])->part);
				}
				free(splitret); splitret = NULL;
				free(tmpstr); tmpstr = NULL;
			}
			drawscreen(wlan, 0, 0);
		}

		if(rcret == getrcconfigint("rcyellow", NULL) && scan == 0)
		{
			drawscreen(load, 0, 0);
			system("killall wpa_supplicant; sleep 2; killall -9 wpa_supplicant");
			clearscreen(load);
			textbox(_("Message"), _("WLAN now stopped"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 10, 0);
			drawscreen(wlan, 0, 0);
		}

		if(rcret == getrcconfigint("rcblue", NULL) && scan == 0)
		{
			tmpstr = readfiletomem("/tmp/wlan.log", 0);
			textbox(_("WLAN LOG"), tmpstr, _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 800, 600, 0, 0);
			free(tmpstr); tmpstr = NULL;
			drawscreen(wlan, 0, 0);
		}

		if(rcret == getrcconfigint("rcinfo", NULL) && scan == 0)
		{
			ret = WEXITSTATUS(system("check_wlan_driver.sh > /tmp/check_wlan_driver"));
			if(ret == 0)
			{
				tmpstr = readfiletomem("/tmp/check_wlan_driver", 0);
				tmpstr = ostrcat(_("Found Wlan Stick and driver for: "), tmpstr, 0, 1);
			}
			else if(ret == 1)
			{
				tmpstr = readfiletomem("/tmp/check_wlan_driver", 0);
				tmpstr = ostrcat(_("Found Wlan Stick but no driver\nPlease install driver: "), tmpstr, 0, 1);
			}
			else
				tmpstr = ostrcat(_("Wlan Stick not found"), tmpstr, 0, 1);

			unlink("/tmp/check_wlan_driver");
			textbox(_("WLAN Info"), tmpstr, _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 0, 0);
			free(tmpstr); tmpstr = NULL;
			drawscreen(wlan, 0, 0);
		}

		if(rcret == RCTIMEOUT)
			drawscreen(wlan, 0, 0);
	}

	delmarkedscreennodes(wlan, 1);
	delownerrc(wlan);
	clearscreen(wlan);
}

void screennetwork_password()
{
	int rcret = 0;
	char* tmpstr = NULL;
	char* ret = NULL;
	char* cmd = NULL;

	struct skin* password = getscreen("passwordsettings");
	struct skin* listbox = getscreennode(password, "listbox");	
	struct skin* rootpass = getscreennode(password, "rootpass");
	struct skin* tmp = NULL;

	changemask(rootpass, "****");
	if(getconfig("rootpass", NULL) == NULL)
		changeinput(rootpass, getconfig("rootpass", NULL));
	else
		changeinput(rootpass, "****");

	drawscreen(password, 0, 0);
	addscreenrc(password, listbox);

	tmp = listbox->select;
	while(1)
	{
		addscreenrc(password, tmp);
		rcret = waitrc(password, 0, 0);
		tmp = listbox->select;

		if(rcret == getrcconfigint("rcexit", NULL)) break;
		if(rcret == getrcconfigint("rcok", NULL))
		{
			if(rootpass->ret != NULL && ostrcmp(rootpass->ret, "****") != 0)
			{
				//echo -e "linuxpassword\nlinuxpassword" | passwd root
				cmd = ostrcat("echo -e \"", rootpass->ret, 0, 0);
				cmd = ostrcat(cmd, "\\n", 1, 0);
				cmd = ostrcat(cmd, rootpass->ret, 1, 0);
				cmd = ostrcat(cmd, "\" | passwd root", 1, 0);
				printf("cmd: %s\n", cmd);

				tmpstr = command(cmd);
				printf("tmpstr: %s\n", tmpstr);
				textbox(_("Message"), tmpstr, _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 800, 400, 10, 0);
				free(cmd), cmd = NULL;
				free(tmpstr), tmpstr = NULL;

				debug(99, "rootpass: write");
				debug(99, "rootpass: %s", rootpass->ret);
				addconfigscreen("rootpass", rootpass);
			}
			else
			{
				debug(99, "rootpass: skipped");
			}
			writeallconfig(1);

			debug(99, "rootpass read: %s", getconfig("rootpass", NULL));

			break;
		}
	}
	delownerrc(password);
	clearscreen(password);
}

void screennetwork_ipsec()
{
	int rcret = 0;
	char* tmpstr = NULL;
	char* ret = NULL;
	char* cmd = NULL;
	char* user = NULL;
	char* pass = NULL;
	char* dyndns = NULL;
	char* rekey = NULL;
	char* psk = NULL;
	char* name = NULL;
	char* id = NULL;

	struct skin* ipsec = getscreen("ipsecsettings");
	struct skin* listbox = getscreennode(ipsec, "listbox");	
	struct skin* skin_name = getscreennode(ipsec, "name");
	struct skin* skin_user = getscreennode(ipsec, "user");
	struct skin* skin_pass = getscreennode(ipsec, "pass");
	struct skin* skin_psk = getscreennode(ipsec, "psk");
	struct skin* skin_dyndns = getscreennode(ipsec, "dyndns");
	struct skin* skin_id = getscreennode(ipsec, "id");
	struct skin* skin_rekey = getscreennode(ipsec, "rekey");
	struct skin* tmp = NULL;

	if(!file_exist("/mnt/network/ipsec.conf"))
		system("cp /etc/titan.restore/mnt/network/ipsec.conf /mnt/network/ipsec.conf");

	if(!file_exist("/mnt/network/ipsec.secret"))
		system("cp /etc/titan.restore/mnt/network/ipsec.secret /mnt/network/ipsec.secret");

	name = string_newline(command("cat /mnt/network/ipsec.conf | grep -v '^#' | grep -v %default | sed -nr 's/.*conn ([^ ]+).*/\\1/p'"));
	user = string_newline(command("cat /mnt/network/ipsec.conf | grep -v '^#' | sed -nr 's/.*xauth_identity=([^=]+).*/\\1/p'"));
	pass = string_newline(command("cat /mnt/network/ipsec.secrets | grep -v '^#' | sed -nr 's/.*: XAUTH \"([^\"]+)\".*/\\1/p'"));
	dyndns = string_newline(command("cat /mnt/network/ipsec.conf | grep -v '^#' | sed -nr 's/.*right=([^=]+).*/\\1/p'"));
	rekey = string_newline(command("cat /mnt/network/ipsec.conf | grep -v '^#' | sed -nr 's/.*rekey=([^=]+).*/\\1/p'"));
	psk = string_newline(command("cat /mnt/network/ipsec.secrets | grep -v '^#' | sed -nr 's/.*%any : PSK \"([^\"]+)\".*/\\1/p'"));
	id = string_newline(command("cat /mnt/network/ipsec.conf | grep -v '^#' | sed -nr 's/.*leftid=keyid:([^:]+).*/\\1/p'"));

	printf("name: %s\n", name);
	printf("user: %s\n", user);
	printf("pass: %s\n", pass);
	printf("dyndns: %s\n", dyndns);
	printf("rekey: %s\n", rekey);
	printf("psk: %s\n", psk);
	printf("id: %s\n", id);

	changemask(skin_name, "abcdefghijklmnopqrstuvwxyz");
	changeinput(skin_name, name);

	changemask(skin_user, "abcdefghijklmnopqrstuvwxyz");
	changeinput(skin_user, user);

	changemask(skin_pass, "abcdefghijklmnopqrstuvwxyz");
	changeinput(skin_pass, pass);

	changemask(skin_dyndns, "abcdefghijklmnopqrstuvwxyz");
	changeinput(skin_dyndns, dyndns);

	changemask(skin_psk, "abcdefghijklmnopqrstuvwxyz");
	changeinput(skin_psk, psk);

	changemask(skin_id, "abcdefghijklmnopqrstuvwxyz");
	changeinput(skin_id, id);

	addchoicebox(skin_rekey, "no", _("no"));
	addchoicebox(skin_rekey, "yes", _("yes"));
	setchoiceboxselection(skin_rekey, rekey);

	drawscreen(ipsec, 0, 0);
	addscreenrc(ipsec, listbox);

	tmp = listbox->select;
	while(1)
	{
		addscreenrc(ipsec, tmp);
		rcret = waitrc(ipsec, 0, 0);
		tmp = listbox->select;

		if(rcret == getrcconfigint("rcexit", NULL)) break;
		if(rcret == getrcconfigint("rcok", NULL))
		{
			if(skin_name->ret != NULL)
			{
				cmd = ostrcat("cat /mnt/network/ipsec.conf | sed 's/.*conn $%.*/conn ", skin_name->ret, 0, 0);
				cmd = ostrcat(cmd,"/'", 1, 0);
				printf("cmd: %s\n", cmd);
				system(cmd);
				free(cmd), cmd = NULL;
			}

			if(skin_user->ret != NULL)
			{
				cmd = ostrcat("cat /mnt/network/ipsec.conf | sed 's/.*xauth_identity=.*/xauth_identity=", skin_user->ret, 0, 0);
				cmd = ostrcat(cmd,"/'", 1, 0);
				printf("cmd: %s\n", cmd);
				system(cmd);
				free(cmd), cmd = NULL;
			}

			if(skin_id->ret != NULL)
			{
				cmd = ostrcat("cat /mnt/network/ipsec.conf | sed 's/.*leftid=keyid:.*/leftid=keyid:", skin_id->ret, 0, 0);
				cmd = ostrcat(cmd,"/'", 1, 0);
				printf("cmd: %s\n", cmd);
				system(cmd);
				free(cmd), cmd = NULL;
			}

			if(skin_dyndns->ret != NULL)
			{
				cmd = ostrcat("cat /mnt/network/ipsec.conf | sed 's/.*right=.*/right=", skin_dyndns->ret, 0, 0);
				cmd = ostrcat(cmd,"/'", 1, 0);
				printf("cmd: %s\n", cmd);
				system(cmd);
				free(cmd), cmd = NULL;
			}

			if(skin_rekey->ret != NULL)
			{
				cmd = ostrcat("cat /mnt/network/ipsec.conf | sed 's/.*rekey=.*/rekey=", skin_rekey->ret, 0, 0);
				cmd = ostrcat(cmd,"/'", 1, 0);
				printf("cmd: %s\n", cmd);
				system(cmd);
				free(cmd), cmd = NULL;
			}

			if(skin_pass->ret != NULL)
			{
				cmd = ostrcat("cat /mnt/network/ipsec.secrets | sed 's/.*: XAUTH \".*/: XAUTH \"", skin_pass->ret, 0, 0);
				cmd = ostrcat(cmd,"\"/'", 1, 0);
				printf("cmd: %s\n", cmd);
				system(cmd);
				free(cmd), cmd = NULL;
			}

			if(skin_psk->ret != NULL)
			{
				cmd = ostrcat("cat /mnt/network/ipsec.secrets | sed 's/%any : PSK \".*/%any : PSK \"", skin_psk->ret, 0, 0);
				cmd = ostrcat(cmd,"\"/'", 1, 0);
				printf("cmd: %s\n", cmd);
				system(cmd);
				free(cmd), cmd = NULL;
			}

			if(skin_user->ret != NULL)
			{
				cmd = ostrcat("cat /mnt/network/ipsec.secrets | sed 's/.* : XAUTH/", skin_user->ret, 0, 0);
				cmd = ostrcat(cmd," : XAUTH/'", 1, 0);
				printf("cmd: %s\n", cmd);
				system(cmd);
				free(cmd), cmd = NULL;
			}

			debug(99, "name: %s", skin_name->ret);
			debug(99, "user: %s", skin_user->ret);
			debug(99, "pass: %s", skin_pass->ret);
			debug(99, "dyndns: %s", skin_dyndns->ret);
			debug(99, "psk: %s", skin_psk->ret);
			debug(99, "id: %s", skin_id->ret);
			debug(99, "rekey: %s", skin_rekey->ret);

			break;
		}
	}
	delownerrc(ipsec);
	clearscreen(ipsec);
}

#endif
