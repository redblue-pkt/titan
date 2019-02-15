#ifndef INETWORK_H
#define INETWORK_H

void delinetworkfound()
{
	struct inetwork *node = inetwork;

	while(node != NULL)
	{
		node->found = 0;
		node = node->next;
	}
}

struct inetwork* getinetworkfirstwlan()
{
	struct inetwork *node = inetwork;

	while(node != NULL)
	{
		if(ostrcmp(node->device, "ra0") == 0)
			return node;
		if(ostrcmp(node->device, "wlan0") == 0)
			return node;

		node = node->next;
	}
	return NULL;
}

struct inetwork* getinetworkbydevice(char* device)
{
	struct inetwork *node = inetwork;

	while(node != NULL)
	{
		if(ostrcmp(node->device, device) == 0)
			return node;

		node = node->next;
	}
	debug(100, "inetwork not found (name=%s)", device);
	return NULL;
}

void delinetwork(char* device)
{
	struct inetwork *node = inetwork, *prev = inetwork;
	
	while(node != NULL)
	{
		if(ostrcmp(node->device, device) == 0)
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
}

void delinetworknotfound()
{
	struct inetwork *node = inetwork, *prev = inetwork;

	while(node != NULL)
	{
		prev = node;
		node = node->next;
		if(prev != NULL && prev->found == 0)
			delinetwork(prev->device);
	}
}

//flag 0: change all
//flag 1: check only values from getifaddrs (called from timer)
struct inetwork* changeinetwork(char* device, char* ip, char* netmask, char* mac, char* broadcast, int type, struct inetwork* node, int flag)
{
	if(node == NULL) return NULL;

	if(ostrcmp(node->device, device) != 0)
	{
		free(node->device);
		node->device = device;
	}
	else
		free(device);

	if(ostrcmp(node->ip, ip) != 0)
	{
		free(node->ip);
		node->ip = ip;
	}
	else
		free(ip);

	if(ostrcmp(node->netmask, netmask) != 0)
	{
		free(node->netmask);
		node->netmask = netmask;
	}
	else
		free(netmask);

	if(ostrcmp(node->broadcast, broadcast) != 0)
	{
		free(node->broadcast);
		node->broadcast = broadcast;
	}
	else
		free(broadcast);

	if(flag == 0)
	{
		if(ostrcmp(node->mac, mac) != 0)
		{
			free(node->mac);
			node->mac = mac;
		}
		else
			free(mac);

		node->type = type;
	}
	else
		free(mac);
	
	node->found = 1;

	return node;
}

struct inetwork* addinetwork(char* device, char* ip, char* netmask, char* mac, char* broadcast, int type, struct inetwork* last)
{
	struct inetwork *newnode = NULL, *prev = NULL, *node = inetwork;

	newnode = (struct inetwork*)malloc(sizeof(struct inetwork));	
	if(newnode == NULL)
	{
		err("no memory");
		return NULL;
	}

	memset(newnode, 0, sizeof(struct inetwork));

	newnode->device = device;
	newnode->ip = ip;
	newnode->netmask = netmask;
	newnode->mac = mac;
	newnode->broadcast = broadcast;
	newnode->type = type;
	newnode->found = 1;
	
	if(ostrcmp(device, "eth0") == 0)
		newnode->flag = 1;
	if(ostrcmp(device, "ra0") == 0)
		newnode->flag = 1;
	if(ostrcmp(device, "wlan0") == 0)
		newnode->flag = 1;
	
	debug(50,"[NETWORK] device: %s", device);
	debug(50,"[NETWORK] ip: %s", ip);
	debug(50,"[NETWORK] netmask: %s", netmask);
	debug(50,"[NETWORK] mac: %s", mac);
	debug(50,"[NETWORK] broadcast: %s", broadcast);
	debug(50,"[NETWORK] type: %d\n", type);

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

	return newnode;
}

//self == NULL: check all
//self != NULL: check only values from getifaddrs (called from timer)
int addinetworkall(struct stimerthread* self)
{
	int ret = 0;
	struct ifaddrs *ifa = NULL, *tmpifa = NULL;
	void* tmpaddr = NULL;
	char* buf = NULL, *tmpstr = NULL;
	char* tmp_gateway = NULL;
	char* tmp_dnsserver1 = NULL;
	char* tmp_dnsserver2 = NULL;
	char* cmd = NULL;

	m_lock(&status.inetworkmutex, 21);
	struct inetwork* node = inetwork;

	ret = getifaddrs(&ifa);
	if(ret != 0)
	{
		err("get network interfaces");
		m_unlock(&status.inetworkmutex, 21);
		return 1;
	}

	buf = malloc(MINMALLOC);
	if(buf == NULL)
	{
		err("no mem");
		m_unlock(&status.inetworkmutex, 21);
		return 1;
	}

	delinetworkfound();

	for(tmpifa = ifa; tmpifa != NULL; tmpifa = tmpifa->ifa_next)
	{
		char* tmp_ipaddress = NULL;
		char* tmp_netmask = NULL;
		char* tmp_mac = NULL;
		char* tmp_broadcast = NULL;
		char* tmp_device = NULL;
		int tmp_type = 0;

		if(tmpifa->ifa_addr == NULL)
			continue;

		//Don't add IPV6 ot other
		if(tmpifa->ifa_addr->sa_family != AF_INET && tmpifa->ifa_addr->sa_family != AF_PACKET)
			continue;

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
			tmp_ipaddress = string_newline(ostrcat(tmpstr, NULL, 1, 0));
		}
		else
			tmp_ipaddress = ostrcat(tmp_ipaddress, "000.000.000.000", 1, 0);

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
			tmp_netmask = string_newline(ostrcat(tmpstr, NULL, 1, 0));
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
			tmp_broadcast = string_newline(ostrcat(tmpstr, NULL,  1, 0));
		}
		else
			tmp_broadcast = ostrcat(tmp_broadcast, "000.000.000.000", 1, 0);

		if(self == NULL)
		{
			// MAC
			tmp_mac = getmac(tmp_device);
			if(tmp_mac == NULL)
				tmp_mac = ostrcat(tmp_mac, "00:00:00:00:00:00", 1, 0);

			// DHCP
			if(tmp_type == 0)
			{
				cmd = ostrcat(cmd, "cat /mnt/network/interfaces | grep ", 1, 0);
				cmd = ostrcat(cmd, tmp_device, 1, 0);
				cmd = ostrcat(cmd, " | grep dhcp | wc -l", 1, 0);
				tmpstr = command(cmd);
				if(tmpstr != NULL && atoi(tmpstr) == 1)
					tmp_type = 1;
				free(tmpstr); tmpstr = NULL;
			}
			free(cmd); cmd = NULL;

			// OFF
			if(tmp_type == 0)
			{
				cmd = ostrcat(cmd, "cat /mnt/network/interfaces | grep ", 1, 0);
				cmd = ostrcat(cmd, tmp_device, 1, 0);
#ifdef OEBUILD
				cmd = ostrcat(cmd, " | grep manual | wc -l", 1, 0);
#else
				cmd = ostrcat(cmd, " | grep off | wc -l", 1, 0);
#endif
				tmpstr = command(cmd);
				if(tmpstr != NULL && atoi(tmpstr) == 1)
					tmp_type = 2;
				free(tmpstr); tmpstr = NULL;
			}
			free(cmd); cmd = NULL;
		}

		struct inetwork* tmpinetwork = getinetworkbydevice(tmp_device);
		if(tmpinetwork != NULL)
		{
			if(self == NULL)
				node = changeinetwork(tmp_device, tmp_ipaddress, tmp_netmask, tmp_mac, tmp_broadcast, tmp_type, tmpinetwork, 0);
			else
				node = changeinetwork(tmp_device, tmp_ipaddress, tmp_netmask, tmp_mac, tmp_broadcast, tmp_type, tmpinetwork, 1);
		}
		else
			node = addinetwork(tmp_device, tmp_ipaddress, tmp_netmask, tmp_mac, tmp_broadcast, tmp_type, node);
	}

	freeifaddrs(ifa); ifa = NULL;

	// GATEWAY
	if(self == NULL)
	{
		free(status.gateway); status.gateway = NULL;
		tmp_gateway = getdefaultgw();
		if(tmp_gateway != NULL)
		{
			tmpstr = fixip(tmp_gateway, 0);
			status.gateway = string_newline(ostrcat(tmpstr, NULL, 1, 0));
		}
		else
			status.gateway = ostrcat(tmp_gateway, "000.000.000.000", 1, 0);

		// DNSSERVER1
		free(status.dnsserver1); status.dnsserver1 = NULL;
		cmd = ostrcat(cmd, "cat /mnt/network/resolv.conf | grep nameserver | awk '{ print $2 }' | sed -n 1p", 1, 0);
		tmp_dnsserver1 = ostrcat(tmp_dnsserver1 , command(cmd), 1, 1);
		if(tmp_dnsserver1 != NULL)
		{
			tmpstr = fixip(tmp_dnsserver1, 0);
			free(tmp_dnsserver1); tmp_dnsserver1 = NULL;
			status.dnsserver1 = string_newline(ostrcat(tmpstr, NULL, 1, 0));
		}
		else
			status.dnsserver1 = ostrcat(tmp_dnsserver1, "000.000.000.000", 1, 0);
		free(cmd); cmd = NULL;

		// DNSSERVER2
		free(status.dnsserver2); status.dnsserver2 = NULL;
		cmd = ostrcat(cmd, "cat /mnt/network/resolv.conf | grep nameserver | awk '{ print $2 }' | sed -n 2p", 1, 0);
		tmp_dnsserver2 = ostrcat(tmp_dnsserver2 , command(cmd), 1, 1);
		if(tmp_dnsserver2 != NULL)
		{
			tmpstr = fixip(tmp_dnsserver2, 0);
			free(tmp_dnsserver2); tmp_dnsserver2 = NULL;
			status.dnsserver2 = string_newline(ostrcat(tmpstr, NULL, 1, 0));
		}
		else
			status.dnsserver2 = ostrcat(tmp_dnsserver2, "000.000.000.000", 1, 0);
	}
	free(cmd); cmd = NULL;
	free(buf);

	delinetworknotfound();
	m_unlock(&status.inetworkmutex, 21);
	return 0;
}

void freeinetwork()
{
	struct inetwork *node = inetwork, *prev = inetwork;

	while(node != NULL)
	{
		prev = node;
		node = node->next;
		if(prev != NULL)
			delinetwork(prev->device);
	}
}

#endif
