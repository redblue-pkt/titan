#ifndef INETWORK_H
#define INETWORK_H

struct inetwork* getinetworkbydevice(char* device)
{
	debug(1000, "in");
	struct inetwork *node = inetwork;

	while(node != NULL)
	{
		if(ostrcmp(node->device, device) == 0)
		{
			debug(1000, "out");
			return node;
		}

		node = node->next;
	}
	debug(100, "inetwork not found (name=%s)", device);
	return NULL;
}

void delinetwork(int inetworkid)
{
	debug(1000, "in");
	struct inetwork *node = inetwork, *prev = inetwork;
	
	while(node != NULL)
	{
		if(node->id == inetworkid)
		{
			if(node == inetwork)
			{
				inetwork = node->next;
				if(inetwork != NULL)
					inetwork->prev = NULL;
			}
			else
			{
				prev->next = node->next;
				if(node->next != NULL)
					node->next->prev = prev;
			}

			free(node->device); node->device = NULL;
			free(node->ip); node->ip = NULL;
			free(node->netmask); node->netmask = NULL;
			free(node->mac); node->mac = NULL;
			free(node->broadcast); node->broadcast = NULL;

			free(node);
			node = NULL;
			break;
		}

		prev = node;
		node = node->next;
	}
	debug(1000, "out");
}

struct inetwork* getinetwork(int id)
{
	debug(1000, "in");
	struct inetwork *node = inetwork;

	while(node != NULL)
	{
		if(node->id == id)
		{
			debug(1000, "out");
			return node;
		}

		node = node->next;
	}
	debug(100, "inetwork not found (id=%d)", id);
	return NULL;
}

struct inetwork* addinetwork(int id, char* device, char* ip, char* netmask, char* mac, char* broadcast, int dhcp, struct inetwork* last)
{
	debug(1000, "in");
	struct inetwork *newnode = NULL, *prev = NULL, *node = inetwork;

	newnode = (struct inetwork*)malloc(sizeof(struct inetwork));	
	if(newnode == NULL)
	{
		err("no memory");
		return NULL;
	}

	memset(newnode, 0, sizeof(struct inetwork));

	newnode->id = id;
	newnode->device = device;
	newnode->ip = ip;
	newnode->netmask = netmask;
	newnode->mac = mac;
	newnode->broadcast = broadcast;
	newnode->dhcp = dhcp;
	
	debug(50,"\n[NETWORK] device-id: %d", id);
	debug(50,"[NETWORK] device: %s", device);
	debug(50,"[NETWORK] ip: %s", ip);
	debug(50,"[NETWORK] netmask: %s", netmask);
	debug(50,"[NETWORK] mac: %s", mac);
	debug(50,"[NETWORK] broadcast: %s", broadcast);
	debug(50,"[NETWORK] dhcp: %d\n", dhcp);

	if(last == NULL)
	{
		while(node != NULL && strcoll(newnode->device, node->device) > 0)
		{
			prev = node;
			node = node->next;
		}
	}
	else
	{
		prev = last;
		node = last->next;
	}

	if(prev == NULL)
		inetwork = newnode;
	else
	{
		prev->next = newnode;
		newnode->prev = prev;
	}
	newnode->next = node;

	debug(1000, "out");
	return newnode;
}

int addinetworkall()
{
	int ret = 0, i = 0;
	struct ifaddrs *ifa = NULL, *tmpifa = NULL;
	void* tmpaddr = NULL;
	char* buf = NULL, *tmpstr = NULL;
	struct inetwork* node = inetwork;
	char* tmp_gateway = NULL;
	char* tmp_dnsserver1 = NULL;
	char* tmp_dnsserver2 = NULL;
	char* cmd = NULL;

	ret = getifaddrs(&ifa);
	if(ret != 0)
	{
		err("get network interfaces");
		return 1;
	}

	buf = malloc(MINMALLOC);
	if(buf == NULL)
	{
		err("no mem");
		return 1;
	}

	for(tmpifa = ifa; tmpifa != NULL; tmpifa = tmpifa->ifa_next)
	{
		char* cmddev = NULL;
		char* tmp_ipaddresse = NULL;
		char* tmp_netmask = NULL;
		char* tmp_mac = NULL;
		char* tmp_broadcast = NULL;
		char* tmp_device = NULL;
		int tmp_dhcp = 0;


		//Don't add IPV6 ot other
		if(tmpifa->ifa_addr->sa_family != AF_INET && tmpifa->ifa_addr->sa_family != AF_PACKET)
			continue;

		cmddev = ostrcat(cmddev, tmpifa->ifa_name, 1, 0);
		cmddev = ostrcat("ifconfig ", cmddev, 0, 1);

		//Device Name
		tmp_device = string_newline(ostrcat(tmp_device, tmpifa->ifa_name, 1, 0));

		//IP ADDR
		memset(buf, 0, MINMALLOC);
		if(tmpifa->ifa_addr->sa_family == AF_INET)
			tmpaddr = &((struct sockaddr_in*)tmpifa->ifa_addr)->sin_addr;
		else
			tmpaddr = &((struct sockaddr_in6*)tmpifa->ifa_addr)->sin6_addr;
		inet_ntop(tmpifa->ifa_addr->sa_family, tmpaddr, buf, MINMALLOC);
		if(strlen(buf) != 0)
		{
			tmpstr = fixip(buf, 0);
			tmp_ipaddresse = string_newline(ostrcat(tmpstr, "", 1, 0));
		}
		else
			tmp_ipaddresse = ostrcat(tmp_ipaddresse, "000.000.000.000", 1, 0);

		//NETMASK
		memset(buf, 0, MINMALLOC);
		if(tmpifa->ifa_netmask != NULL)
		{
			if(tmpifa->ifa_netmask->sa_family == AF_INET)
				tmpaddr = &((struct sockaddr_in*)tmpifa->ifa_netmask)->sin_addr;
			else
				tmpaddr = &((struct sockaddr_in6*)tmpifa->ifa_netmask)->sin6_addr;
			inet_ntop(tmpifa->ifa_netmask->sa_family, tmpaddr, buf, MINMALLOC);

		}
		if(strlen(buf) != 0)
		{
			tmpstr = fixip(buf, 0);
			tmp_netmask = string_newline(ostrcat(tmpstr, "", 1, 0));
		}
		else
			tmp_netmask = ostrcat(tmp_netmask, "000.000.000.000", 1, 0);

		//BROADCAST / DSTADDR
		memset(buf, 0, MINMALLOC);
		if(tmpifa->ifa_ifu.ifu_broadaddr != NULL)
		{
			if(tmpifa->ifa_flags & IFF_POINTOPOINT)
			{
				if(tmpifa->ifa_ifu.ifu_dstaddr->sa_family == AF_INET)
					tmpaddr = &((struct sockaddr_in*)tmpifa->ifa_ifu.ifu_dstaddr)->sin_addr;
				else
					tmpaddr = &((struct sockaddr_in6*)tmpifa->ifa_ifu.ifu_dstaddr)->sin6_addr;
				inet_ntop(tmpifa->ifa_ifu.ifu_dstaddr->sa_family, tmpaddr, buf, MINMALLOC);
			}
			else
			{
				if(tmpifa->ifa_ifu.ifu_broadaddr->sa_family == AF_INET)
					tmpaddr = &((struct sockaddr_in*)tmpifa->ifa_ifu.ifu_broadaddr)->sin_addr;
				else
					tmpaddr = &((struct sockaddr_in6*)tmpifa->ifa_ifu.ifu_broadaddr)->sin6_addr;
				inet_ntop(tmpifa->ifa_ifu.ifu_broadaddr->sa_family, tmpaddr, buf, MINMALLOC);
			}
				
		}
		if(strlen(buf) != 0)
		{
			tmpstr = fixip(tmp_broadcast, 0);
			tmp_broadcast = string_newline(ostrcat(tmpstr, "",  1, 0));
		}
		else
			tmp_broadcast = ostrcat(tmp_broadcast, "000.000.000.000", 1, 0);

		// MAC
		if(ostrcmp(cmddev, "ifconfig lo") == 0)
			tmp_mac = ostrcat(tmp_mac, "00:00:00:00:00:00", 1, 0);
		else
		{
			cmd = ostrcat(cmd, " | grep -m1 'HWaddr' | awk '{ print $5 }'", 1, 0);
			cmd = ostrcat(cmddev, cmd, 0, 1);
			tmp_mac = command(cmd);
		
			if (tmp_mac != 0)
				tmp_mac = string_newline(ostrcat(tmp_mac, "", 1, 0));
			else
				tmp_mac = ostrcat(tmp_mac, "00:00:00:00:00:00", 1, 0);
		}
		free(cmd); cmd = NULL;

		// DHCP
		cmd = ostrcat(cmd, "cat /var/etc/network/interfaces | grep dhcp | wc -l", 1, 0);
		tmpstr = command(cmd);
		if(tmpstr != NULL)
			tmp_dhcp = atoi(tmpstr);
		free(tmpstr); tmpstr = NULL;


		struct inetwork* tmpinetwork = getinetworkbydevice(tmp_device);
		if(tmpinetwork != NULL)
			delinetwork(tmpinetwork->id);

		node = addinetwork(i, tmp_device, tmp_ipaddresse, tmp_netmask, tmp_mac,  tmp_broadcast, tmp_dhcp, node);
		i++;

		free(cmd); cmd = NULL;
		free(cmddev); cmddev = NULL;
	}

	freeifaddrs(ifa); ifa = NULL;

	// GATEWAY
	tmp_gateway = getdefaultgw();
	if (tmp_gateway != 0)
	{
		tmpstr = fixip(tmp_gateway, 0);
		status.gateway = string_newline(ostrcat(tmpstr, "", 1, 0));
	}
	else
		status.gateway = ostrcat(tmp_gateway, "000.000.000.000", 1, 0);

	// DNSSERVER1
	cmd = ostrcat(cmd, "cat /var/etc/resolv.conf | grep nameserver | awk '{ print $2 }' | head -n1 | tail -n1", 1, 0);
	tmp_dnsserver1 = ostrcat(tmp_dnsserver1 , command(cmd), 1, 1);
	if (tmp_dnsserver1 != 0)
	{
		tmpstr = fixip(tmp_dnsserver1, 0);
		free(tmp_dnsserver1); tmp_dnsserver1 = NULL;
		status.dnsserver1 = string_newline(ostrcat(tmpstr, "", 1, 0));
	}
	else
		status.dnsserver1 = ostrcat(tmp_dnsserver1, "000.000.000.000", 1, 0);
	free(cmd); cmd = NULL;

	// DNSSERVER2
	cmd = ostrcat(cmd, "cat /var/etc/resolv.conf | grep nameserver | awk '{ print $2 }' | head -n2 | tail -n1", 1, 0);
	tmp_dnsserver2 = ostrcat(tmp_dnsserver2 , command(cmd), 1, 1);
	if (tmp_dnsserver2 != 0)
	{
		tmpstr = fixip(tmp_dnsserver2, 0);
		free(tmp_dnsserver2); tmp_dnsserver2 = NULL;
		status.dnsserver2 = string_newline(ostrcat(tmpstr, "", 1, 0));
	}
	else
		status.dnsserver2 = ostrcat(tmp_dnsserver2, "000.000.000.000", 1, 0);
	free(cmd); cmd = NULL;
	free(buf);
	return 0;
}

//this method does not include
//network interfaces, that not
//configured
/*
int addinetworkall()
{
	char* buf = NULL, *tmpstr = NULL;
	int sck, nInterfaces, i;
	struct ifconf ifc;
	struct ifreq *ifr;
	struct inetwork* node = inetwork;
	char* tmp_gateway = NULL;
	char* tmp_dnsserver1 = NULL;
	char* tmp_dnsserver2 = NULL;
	char* cmd = NULL;

	buf = malloc(MINMALLOC);
	if(buf == NULL)
	{
		err("no mem");
		return 1;
	}

	// Get a socket handle.
	sck = socket(AF_INET, SOCK_DGRAM, 0);
	if(sck < 0)
	{
		perror("socket");
		return 0;
	}

	// Query available interfaces.
	ifc.ifc_len = MINMALLOC;
	ifc.ifc_buf = buf;
	if(ioctl(sck, SIOCGIFCONF, &ifc) < 0)
	{
		perror("ioctl(SIOCGIFCONF)");
		return 0;
	}

	// Iterate through the list of interfaces.
	ifr = ifc.ifc_req;
	nInterfaces = ifc.ifc_len / sizeof(struct ifreq);
	for(i = 0; i < nInterfaces; i++)
	{
		struct ifreq *item = &ifr[i];
		char* cmddev = NULL;
		char* tmp_ipaddresse = NULL;
		char* tmp_netmask = NULL;
		char* tmp_mac = NULL;
		char* tmp_broadcast = NULL;
		char* tmp_device = NULL;
		int tmp_dhcp = 0;

		cmddev = ostrcat(cmddev, item->ifr_name, 1, 0);
		cmddev = ostrcat("ifconfig ", cmddev, 0, 1);

		// IP
		cmd = ostrcat(cmd, " | grep -m1 'inet addr' | awk '{ print $2 }' | cut -d ':' -f2", 1, 0);
		cmd = ostrcat(cmddev, cmd, 0, 1);
		tmp_ipaddresse = ostrcat(tmp_ipaddresse, command(cmd), 1, 1);
		if (tmp_ipaddresse != 0)
		{
			tmpstr = fixip(tmp_ipaddresse, 0);
			free(tmp_ipaddresse); tmp_ipaddresse = NULL;
			tmp_ipaddresse = string_newline(ostrcat(tmpstr, "", 1, 0));
		}
		else
			tmp_ipaddresse = ostrcat(tmp_ipaddresse, "000.000.000.000", 1, 0);
		free(cmd); cmd = NULL;

		// NETMASK
		if(ostrcmp(cmddev, "ifconfig lo") == 0)
			cmd = ostrcat(cmd, " | grep -m1 'inet addr' | awk '{ print $3 }' | cut -d ':' -f2", 1, 0);
		else
			cmd = ostrcat(cmd, " | grep -m1 'inet addr' | awk '{ print $4 }' | cut -d ':' -f2", 1 ,0);
		cmd = ostrcat(cmddev, cmd, 0, 1);
		tmp_netmask = ostrcat(tmp_netmask , command(cmd), 1, 1);
		if (tmp_netmask != 0)
		{
			tmpstr = fixip(tmp_netmask, 0);
			free(tmp_netmask); tmp_netmask = NULL;
			tmp_netmask = string_newline(ostrcat(tmpstr, "", 1, 0));
		}
		else
			tmp_netmask = ostrcat(tmp_netmask, "000.000.000.000", 1, 0);
		free(cmd); cmd = NULL;

		// MAC
		if(ostrcmp(cmddev, "ifconfig lo") == 0)
			tmp_mac = ostrcat(tmp_mac, "00:00:00:00:00:00", 1, 0);
		else
		{
			cmd = ostrcat(cmd, " | grep -m1 'HWaddr' | awk '{ print $5 }'", 1, 0);
			cmd = ostrcat(cmddev, cmd, 0, 1);
			tmp_mac = command(cmd);
		
			if (tmp_mac != 0)
				tmp_mac = string_newline(ostrcat(tmp_mac, "", 1, 0));
			else
				tmp_mac = ostrcat(tmp_mac, "00:00:00:00:00:00", 1, 0);
		}
		free(cmd); cmd = NULL;

		// BROADCAST
		if(ostrcmp(cmddev, "ifconfig lo") == 0)
			tmp_broadcast = ostrcat(tmp_broadcast, "000.000.000.000", 1, 0);
		else
		{
			cmd = ostrcat(cmd, " | grep -m1 'inet addr' | awk '{ print $3 }' | cut -d ':' -f2", 1, 0);
			cmd = ostrcat(cmddev, cmd, 0, 1);
			tmp_broadcast = command(cmd);
		}
		if (tmp_broadcast != 0)
		{
			tmpstr = fixip(tmp_broadcast, 0);
			free(tmp_broadcast); tmp_broadcast = NULL;
			tmp_broadcast = string_newline(ostrcat(tmpstr, "",  1, 0));
		}
		else
			tmp_broadcast = ostrcat(tmp_broadcast, "000.000.000.000", 1, 0);
		free(cmd); cmd = NULL;

		// DHCP
		cmd = ostrcat(cmd, "cat /var/etc/network/interfaces | grep dhcp | wc -l", 1, 0);
		tmpstr = command(cmd);
		if(tmpstr != NULL)
			tmp_dhcp = atoi(tmpstr);
		free(tmpstr); tmpstr = NULL;

		// DEVICE NAME
		tmp_device = string_newline(ostrcat(tmp_device, item->ifr_name, 1, 0));

		node = addinetwork(i, tmp_device, tmp_ipaddresse, tmp_netmask, tmp_mac,  tmp_broadcast, tmp_dhcp, node);

		free(cmd); cmd = NULL;
		free(cmddev); cmddev = NULL;
	}

	// GATEWAY
	tmp_gateway = getdefaultgw();
	if (tmp_gateway != 0)
	{
		tmpstr = fixip(tmp_gateway, 0);
		status.gateway = string_newline(ostrcat(tmpstr, "", 1, 0));
	}
	else
		status.gateway = ostrcat(tmp_gateway, "000.000.000.000", 1, 0);

	// DNSSERVER1
	cmd = ostrcat(cmd, "cat /var/etc/resolv.conf | grep nameserver | awk '{ print $2 }' | head -n1 | tail -n1", 1, 0);
	tmp_dnsserver1 = ostrcat(tmp_dnsserver1 , command(cmd), 1, 1);
	if (tmp_dnsserver1 != 0)
	{
		tmpstr = fixip(tmp_dnsserver1, 0);
		free(tmp_dnsserver1); tmp_dnsserver1 = NULL;
		status.dnsserver1 = string_newline(ostrcat(tmpstr, "", 1, 0));
	}
	else
		status.dnsserver1 = ostrcat(tmp_dnsserver1, "000.000.000.000", 1, 0);
	free(cmd); cmd = NULL;

	// DNSSERVER2
	cmd = ostrcat(cmd, "cat /var/etc/resolv.conf | grep nameserver | awk '{ print $2 }' | head -n2 | tail -n1", 1, 0);
	tmp_dnsserver2 = ostrcat(tmp_dnsserver2 , command(cmd), 1, 1);
	if (tmp_dnsserver2 != 0)
	{
		tmpstr = fixip(tmp_dnsserver2, 0);
		free(tmp_dnsserver2); tmp_dnsserver2 = NULL;
		status.dnsserver2 = string_newline(ostrcat(tmpstr, "", 1, 0));
	}
	else
		status.dnsserver2 = ostrcat(tmp_dnsserver2, "000.000.000.000", 1, 0);
	free(cmd); cmd = NULL;

	free(buf);
	return 0;
}
*/

void freeinetwork()
{
	debug(1000, "in");
	struct inetwork *node = inetwork, *prev = inetwork;

	while(node != NULL)
	{
		prev = node;
		node = node->next;
		if(prev != NULL)
			delinetwork(prev->id);
	}
	debug(1000, "out");
}

#endif
