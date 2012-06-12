#ifndef ZAP_H
#define ZAP_H

struct channel* nextprev(int zapdir, struct bouquet* node)
{
	int rcret = 0;
	struct skin* infobar = getscreen("infobar");
	struct channel* tmpchnode = status.aktservice->channel;
	struct bouquet* tmpnode = node, *lastnode = node;
	int virtualzap = getconfigint("virtualzap", NULL) * 1000;

	if(node == NULL) return NULL;
	struct channel* chnode = node->channel;

start:
	if(zapdir == 0) //UP
	{				
		while(tmpnode != NULL)
		{
			tmpnode = tmpnode->next;
			if(tmpnode != NULL && tmpnode->channel != NULL && tmpnode->channel->servicetype == status.servicetype && channelnottunable(tmpnode->channel) == 0)
			{
				lastnode = tmpnode;
				chnode = tmpnode->channel;
				break;
			}
		}
	}
	else if(zapdir == 1) //DOWN
	{
		while(tmpnode != NULL)
		{
			tmpnode = tmpnode->prev;
			if(tmpnode != NULL && tmpnode->channel != NULL && tmpnode->channel->servicetype == status.servicetype && channelnottunable(tmpnode->channel) == 0)
			{
				lastnode = tmpnode;
				chnode = tmpnode->channel;
				break;
			}
		}
	}

	if(virtualzap == 0) return chnode;
	if(tmpnode == NULL) tmpnode = lastnode;

	status.aktservice->type = TMPOTHER;
	status.aktservice->channel = chnode;
	drawscreen(infobar, 0, 0);
	while(1)
	{
		rcret = waitrc(infobar, virtualzap, 0);
		if(rcret == getrcconfigint("rcchup", NULL))
		{
			zapdir = 0;
			goto start;
		}
		else if(rcret == getrcconfigint("rcchdown", NULL))
		{
			zapdir = 1;
			goto start;
		}
		else if(rcret == RCTIMEOUT || rcret == getrcconfigint("rcok", NULL))
			break;
		else if(rcret == getrcconfigint("rcexit", NULL))
		{
			chnode = tmpchnode;
			break;
		}
	}
	status.aktservice->channel = tmpchnode;
	status.aktservice->type = CHANNEL;

	return chnode;
}

int zapall(int zapdir)
{
	int rcret = 0;
	struct channel* node = status.aktservice->channel, *lastnode = status.aktservice->channel;
	struct skin* infobar = getscreen("infobar");
	struct channel* tmpchnode = status.aktservice->channel;
	int virtualzap = getconfigint("virtualzap", NULL) * 1000;

	if(node == NULL) return 1;

start:
	if(zapdir == 0) //UP
	{
		node = node->next;
		while(node != NULL && (node->servicetype != status.servicetype || channelnottunable(node) == 1))
			node = node->next;
	}
	else if(zapdir == 1) //DOWN
	{
		node = node->prev;
		while(node != NULL && (node->servicetype != status.servicetype || channelnottunable(node) == 1))
			node = node->prev;
	}

	if(virtualzap != 0)
	{
		if(node != NULL)
			lastnode = node;
		else
			node = lastnode;

		status.aktservice->type = TMPOTHER;
		status.aktservice->channel = node;
		drawscreen(infobar, 0, 0);
		while(1)
		{
			rcret = waitrc(infobar, virtualzap, 0);
			if(rcret == getrcconfigint("rcchup", NULL))
			{
				zapdir = 0;
				goto start;
			}
			else if(rcret == getrcconfigint("rcchdown", NULL))
			{
				zapdir = 1;
				goto start;
			}
			else if(rcret == RCTIMEOUT || rcret == getrcconfigint("rcok", NULL))
				break;
			else if(rcret == getrcconfigint("rcexit", NULL))
			{
				node = tmpchnode;
				break;
			}
		}
		status.aktservice->channel = tmpchnode;
		status.aktservice->type = CHANNEL;
	}
	
	if(node != NULL)
		servicecheckret(servicestart(node, NULL, NULL, 0), 0);

	return 0;
}

int zapbouquet(int zapdir, char* aktblist)
{
	// got pointer to mainbouquet
	struct mainbouquet* mainbouquetnode = NULL;

	mainbouquetnode = getmainbouquet(aktblist + 10);
	if(mainbouquetnode != NULL && mainbouquetnode->bouquet != NULL)
	{
		//find channel in bouquetlist
		struct bouquet* node = mainbouquetnode->bouquet;
		while(node != NULL)
		{
			if(node->channel == status.aktservice->channel)
			{
				struct channel* tmpchannel = NULL;
				
				tmpchannel = nextprev(zapdir, node);
				if(tmpchannel != NULL)
					servicecheckret(servicestart(tmpchannel, NULL, NULL, 0), 0);
				return 0;
			}
			node = node->next;
		}
	}
	return 0;
}

int zapaz(int zapdir, char* aktblist)
{
	int rcret = 0;
	struct channel* node = status.aktservice->channel, *lastnode = status.aktservice->channel;
	struct skin* infobar = getscreen("infobar");
	struct channel* tmpchnode = status.aktservice->channel;
	int virtualzap = getconfigint("virtualzap", NULL) * 1000;

	if(node == NULL)
		return 1;

start:
	if(zapdir == 0) //UP
	{
		node = node->next;
		while(node != NULL)
		{
			if(node->name != NULL && node->name[0] == aktblist[6] && node->servicetype == status.servicetype && channelnottunable(node) == 0)
				break;
			node = node->next;
		}
	}
	else if(zapdir == 1) //DOWN
	{
		node = node->prev;
		while(node != NULL)
		{
			if(node->name != NULL && node->name[0] == aktblist[6] && node->servicetype == status.servicetype && channelnottunable(node) == 0)
				break;
			node = node->prev;
		}
	}

	if(virtualzap != 0)
	{
		if(node != NULL)
			lastnode = node;
		else
			node = lastnode;

		status.aktservice->type = TMPOTHER;
		status.aktservice->channel = node;
		drawscreen(infobar, 0, 0);
		while(1)
		{
			rcret = waitrc(infobar, virtualzap, 0);
			if(rcret == getrcconfigint("rcchup", NULL))
			{
				zapdir = 0;
				goto start;
			}
			else if(rcret == getrcconfigint("rcchdown", NULL))
			{
				zapdir = 1;
				goto start;
			}
			else if(rcret == RCTIMEOUT || rcret == getrcconfigint("rcok", NULL))
				break;
			else if(rcret == getrcconfigint("rcexit", NULL))
			{
				node = tmpchnode;
				break;
			}
		}
		status.aktservice->channel = tmpchnode;
		status.aktservice->type = CHANNEL;
	}
	
	if(node != NULL)
		servicecheckret(servicestart(node, NULL, NULL, 0), 0);

	return 0;
}

int zapsat(int zapdir, char* aktblist)
{
	int rcret = 0;
	struct channel* node = status.aktservice->channel, *lastnode = status.aktservice->channel;
	struct sat* satnode = getsat(aktblist + 6);
	struct transponder* tpnode = NULL;
	struct skin* infobar = getscreen("infobar");
	struct channel* tmpchnode = status.aktservice->channel;
	int virtualzap = getconfigint("virtualzap", NULL) * 1000;

	if(node == NULL || satnode == NULL)
		return 1;

start:
	if(zapdir == 0) //UP
	{
		node = node->next;
		while(node != NULL)
		{
			tpnode = node->transponder;
			if(tpnode != NULL && tpnode->orbitalpos == satnode->orbitalpos && node->servicetype == status.servicetype && channelnottunable(node) == 0)
				break;
			node = node->next;
		}
	}
	else if(zapdir == 1) //DOWN
	{
		node = node->prev;
		while(node != NULL)
		{
			tpnode = node->transponder;
			if(tpnode != NULL && tpnode->orbitalpos == satnode->orbitalpos && node->servicetype == status.servicetype && channelnottunable(node) == 0)
				break;
			node = node->prev;
		}
	}

	if(virtualzap != 0)
	{
		if(node != NULL)
			lastnode = node;
		else
			node = lastnode;

		status.aktservice->type = TMPOTHER;
		status.aktservice->channel = node;
		drawscreen(infobar, 0, 0);
		while(1)
		{
			rcret = waitrc(infobar, virtualzap, 0);
			if(rcret == getrcconfigint("rcchup", NULL))
			{
				zapdir = 0;
				goto start;
			}
			else if(rcret == getrcconfigint("rcchdown", NULL))
			{
				zapdir = 1;
				goto start;
			}
			else if(rcret == RCTIMEOUT || rcret == getrcconfigint("rcok", NULL))
				break;
			else if(rcret == getrcconfigint("rcexit", NULL))
			{
				node = tmpchnode;
				break;
			}
		}
		status.aktservice->channel = tmpchnode;
		status.aktservice->type = CHANNEL;
	}

	if(node != NULL)
		servicecheckret(servicestart(node, NULL, NULL, 0), 0);
	
	return 0;
}

int zapprovider(int zapdir, char* aktblist)
{
	int rcret = 0;
	struct channel* node = status.aktservice->channel, *lastnode = status.aktservice->channel;
	struct provider* providernode = getproviderbyname(aktblist + 11);
	struct skin* infobar = getscreen("infobar");
	struct channel* tmpchnode = status.aktservice->channel;
	int virtualzap = getconfigint("virtualzap", NULL) * 1000;

	if(node == NULL || providernode == NULL)
		return 1;
	
start:
	if(zapdir == 0) //UP
	{
		node = node->next;
		while(node != NULL)
		{
			if(node->provider == providernode && node->servicetype == status.servicetype && channelnottunable(node) == 0)
				break;
			node = node->next;
		}
	}
	else if(zapdir == 1) //DOWN
	{
		node = node->prev;
		while(node != NULL)
		{
			if(node->provider == providernode && node->servicetype == status.servicetype && channelnottunable(node) == 0)
				break;
			node = node->prev;
		}
	}

	if(virtualzap != 0)
	{
		if(node != NULL)
			lastnode = node;
		else
			node = lastnode;

		status.aktservice->type = TMPOTHER;
		status.aktservice->channel = node;
		drawscreen(infobar, 0, 0);
		while(1)
		{
			rcret = waitrc(infobar, virtualzap, 0);
			if(rcret == getrcconfigint("rcchup", NULL))
			{
				zapdir = 0;
				goto start;
			}
			else if(rcret == getrcconfigint("rcchdown", NULL))
			{
				zapdir = 1;
				goto start;
			}
			else if(rcret == RCTIMEOUT || rcret == getrcconfigint("rcok", NULL))
				break;
			else if(rcret == getrcconfigint("rcexit", NULL))
			{
				node = tmpchnode;
				break;
			}
		}
		status.aktservice->channel = tmpchnode;
		status.aktservice->type = CHANNEL;
	}

	if(node != NULL)
		servicecheckret(servicestart(node, NULL, NULL, 0), 0);
	
	return 0;
}

void zap(int direction)
{
	char* tmpstr = NULL;
	int zapret = 0;

	if(status.servicetype == 0)
		tmpstr = getconfig("channellist", NULL);
	else
		tmpstr = getconfig("rchannellist", NULL);
	if(ostrncmp("(BOUQUET)-", tmpstr, 10) == 0 && strlen(tmpstr) > 10)
		zapret = zapbouquet(direction, tmpstr);
	else if(ostrncmp("(A-Z)-", tmpstr, 6) == 0 && strlen(tmpstr) > 6)
		zapret = zapaz(direction, tmpstr);
	else if(ostrncmp("(SAT)-", tmpstr, 6) == 0 && strlen(tmpstr) > 6)
		zapret = zapsat(direction, tmpstr);
	else if(ostrncmp("(PROVIDER)-", tmpstr, 11) == 0 && strlen(tmpstr) > 6)
		zapret = zapprovider(direction, tmpstr);
	else
		zapret = zapall(direction);
}

void zapup()
{
	zap(0);
}

void zapdown()
{
	zap(1);
}

#endif
