#ifndef LINKEDCHANNEL_H
#define LINKEDCHANNEL_H

struct linkedchannel* getlinkedchannel(struct channel* chnode, int serviceid, unsigned long transponderid)
{
	if(chnode == NULL) return NULL;

	m_lock(&status.linkedchannelmutex, 14);
	struct linkedchannel* node = chnode->linkedchannel;

	while(node != NULL)
	{
		if(node->serviceid == serviceid && node->transponderid == transponderid)
		{
			m_unlock(&status.linkedchannelmutex, 14);
			return node;
		}
		node = node->next;
	}

	m_unlock(&status.linkedchannelmutex, 14);
	return NULL;
}

struct linkedchannel* checklinkedchannel(struct channel* chnode, struct linkedchannel* lchannel)
{
	struct linkedchannel* node = NULL;

	if(chnode != NULL)
	{
		node = chnode->linkedchannel;
		while(node != NULL)
		{
			if(node == lchannel)
				return node;
			node = node->next;
		}
	}
	return NULL;
}

void screenlinkedchannel()
{
	int rcret = 0;
	struct skin* linkedchannel = getscreen("linkedchannel");
	struct skin* listbox = getscreennode(linkedchannel, "listbox");
	struct skin* tmp = NULL;
	struct linkedchannel* node = NULL;
	struct channel* chnode = NULL;
	struct epg* epgnode = NULL;
	char* tmpstr = NULL;

	listbox->aktline = 1;
	listbox->aktpage = -1;

	if(status.aktservice->channel != NULL)
	{
		m_lock(&status.linkedchannelmutex, 14);
		node = status.aktservice->channel->linkedchannel;
		while(node != NULL)
		{
			chnode = getchannel(node->serviceid, node->transponderid);
			if(chnode != NULL)
			{
				tmp = addlistbox(linkedchannel, listbox, tmp, 1);
				if(tmp != NULL)
				{
					tmpstr = ostrcat(tmpstr, chnode->name, 1, 0);
					epgnode = getepgakt(chnode);
					if(epgnode != NULL)
					{
						tmpstr = ostrcat(tmpstr, " - ", 1, 0);
						tmpstr = ostrcat(tmpstr, epgnode->title, 1, 0);
					}
					changetext(tmp, tmpstr);
					free(tmpstr); tmpstr = NULL;
					tmp->handle = (char*)node;
					tmp->handle1 = (char*)chnode;

/*
					tmp->type = CHOICEBOX;
					if(status.aktservice->channel->audiopid == node->audiopid)
						changeinput(tmp, _("running"));
					else
						changeinput(tmp, "");
*/
				}
			}
			node = node->next;
		}	
		m_unlock(&status.linkedchannelmutex, 14);
	}

	drawscreen(linkedchannel, 0);
	addscreenrc(linkedchannel, listbox);

	while(1)
	{
		rcret = waitrc(linkedchannel, 0, 0);
	
		if(rcret == getrcconfigint("rcexit", NULL)) break;
		if(rcret == getrcconfigint("rcok", NULL))
		{
			if(listbox->select != NULL && listbox->select->handle != NULL)
			{
				m_lock(&status.linkedchannelmutex, 14);
				if(checklinkedchannel(status.aktservice->channel, (struct linkedchannel*)listbox->select->handle) != NULL)
					servicecheckret(servicestart((struct channel*)listbox->select->handle1, NULL, NULL, 0), 0);
				m_unlock(&status.linkedchannelmutex, 14);
			}
			break;
		}
	}

	delmarkedscreennodes(linkedchannel, 1);
	delownerrc(linkedchannel);
	clearscreen(linkedchannel);
}

struct linkedchannel* addlinkedchannel(struct channel* chnode, int serviceid, unsigned long transponderid, struct linkedchannel* last)
{
	debug(1000, "in");
	struct linkedchannel *newnode = NULL, *prev = NULL, *node = NULL;

	if(chnode == NULL)
	{
		debug(1000, "out -> NULL detect");
		return NULL;
	}

	newnode = (struct linkedchannel*)malloc(sizeof(struct linkedchannel));	
	if(newnode == NULL)
	{
		err("no memory");
		return NULL;
	}

	memset(newnode, 0, sizeof(struct linkedchannel));

	newnode->serviceid = serviceid;
	newnode->transponderid = transponderid;

	m_lock(&status.linkedchannelmutex, 14);
	node = chnode->linkedchannel;
	if(last == NULL)
	{
		while(node != NULL)
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
		chnode->linkedchannel = newnode;
	else
		prev->next = newnode;

	newnode->next = node;

	m_unlock(&status.linkedchannelmutex, 14);
	debug(1000, "out");
	return newnode;
}

void freelinkedchannel(struct channel* chnode)
{
	debug(1000, "in");
	struct linkedchannel *node = NULL, *prev = NULL;

	if(chnode == NULL)
	{
		debug(1000, "out -> NULL detect");
		return;
	}

	m_lock(&status.linkedchannelmutex, 14);
	node = chnode->linkedchannel;
	prev = chnode->linkedchannel;

	while(node != NULL)
	{
		prev = node;
		node = node->next;
		chnode->linkedchannel = node;

		free(prev);
		prev = NULL;

	}
	m_unlock(&status.linkedchannelmutex, 14);
	debug(1000, "out");
}

#endif
