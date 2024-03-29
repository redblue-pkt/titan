#ifndef AUDIOTRACK_H
#define AUDIOTRACK_H

struct audiotrack* checkaudiotrack(struct channel* chnode, struct audiotrack* atrack)
{
	struct audiotrack* node = NULL;

	if(chnode != NULL)
	{
		node = chnode->audiotrack;
		while(node != NULL)
		{
			if(node == atrack)
				return node;
			node = node->next;
		}
	}
	return NULL;
}

void screenaudiotrack()
{
	int rcret = 0, treffer = 0;
	struct skin* audiotrack = getscreen("audiotrack");
	struct skin* listbox = getscreennode(audiotrack, "listbox");
	struct skin* b4 = getscreennode(audiotrack, "b4");
	struct skin* tmp = NULL;
	struct audiotrack* node = NULL;

	listbox->aktline = 1;
	listbox->aktpage = -1;

	if(status.aktservice->channel != NULL)
	{
		m_lock(&status.audiotrackmutex, 7);
		node = status.aktservice->channel->audiotrack;

		while(node != NULL)
		{
			tmp = addlistbox(audiotrack, listbox, tmp, 1);
			if(tmp != NULL)
			{
				changetext(tmp, node->name);
				tmp->type = CHOICEBOX;
				tmp->del = 1;
				tmp->handle = (char*)node;

				if(status.aktservice->channel->audiopid == node->audiopid)
				{
					changeinput(tmp, _("running"));
					treffer = 1;
				}
				else
					changeinput(tmp, "");
			}
			node = node->next;
		}	
		m_unlock(&status.audiotrackmutex, 7);
	}

	if(treffer == 0) listbox->aktline = 1;
		
	if(status.downmix == 1)
		changetext(b4, "passthrough");
	else
		changetext(b4, getconfig("av_ac3mode", NULL));

	drawscreen(audiotrack, 0, 0);
	addscreenrc(audiotrack, listbox);

	while(1)
	{
		rcret = waitrc(audiotrack, 0, 0);
	
		if(rcret == getrcconfigint("rcexit", NULL)) break;
		if(rcret == getrcconfigint("rcok", NULL))
		{
			if(listbox->select != NULL && listbox->select->handle != NULL)
			{
				m_lock(&status.audiotrackmutex, 7);
				if(checkaudiotrack(status.aktservice->channel, (struct audiotrack*)listbox->select->handle) != NULL)
					servicechangeaudio(status.aktservice->channel, (struct audiotrack*)listbox->select->handle);
				m_unlock(&status.audiotrackmutex, 7);
			}
			break;
		}
		if(rcret == getrcconfigint("rcyellow", NULL))
		{
			clearscreen(audiotrack);
			screensubtitle();
			break;
		}
		if(rcret == getrcconfigint("rcblue", NULL))
		{
			char* ac3dev = NULL;
			ac3dev = getconfig("ac3dev", NULL);
			int ret = 0;
			if(ac3dev != NULL)
			{
				if(status.downmix == 1)
				{
					ret = writesys(ac3dev, "passthrough", 0);
					if(ret == 0)
						status.downmix = 0; 
					else
						printf("ERROR: set AC3_mode to passthrough\n");
				}
				else
				{
					ret = writesys(ac3dev, getconfig("av_ac3mode", NULL), 0);
					if(ret == 0)
						status.downmix = 1;
					else
						printf("ERROR: set AC3_mode to %s\n", getconfig("av_ac3mode", NULL)); 
				}
				if(status.play == 1)
					playerresetts();
			}
			break;
		}
	}

	delmarkedscreennodes(audiotrack, 1);
	delownerrc(audiotrack);
	clearscreen(audiotrack);
}

struct audiotrack* addaudiotrack(struct channel* chnode, char* langdesc, int pid, int audiocodec, struct audiotrack* last)
{
	struct audiotrack *newnode = NULL, *prev = NULL, *node = NULL;
	char *tmpstr = NULL;

	if(chnode == NULL)
	{
		err("NULL detect");
		return NULL;
	}

	newnode = (struct audiotrack*)calloc(1, sizeof(struct audiotrack));
	if(newnode == NULL)
	{
		err("no memory");
		return NULL;
	}

	newnode->audiopid = pid;
	newnode->audiocodec = audiocodec;
	if(ostrcmp(langdesc, "und") == 0)
		tmpstr = ostrcat(tmpstr, _("undefined"), 1, 0);
	else
		tmpstr = ostrcat(tmpstr, _(langdesc), 1, 0);
	switch(audiocodec)
	{
		case AC3: tmpstr = ostrcat(tmpstr, " (DOLBY DIGITAL)", 1, 0); break;
		case MPEGA: tmpstr = ostrcat(tmpstr, " (STEREO)", 1, 0); break;
		case DTS: tmpstr = ostrcat(tmpstr, " (DTS)", 1, 0); break;
		case DTSHD: tmpstr = ostrcat(tmpstr, " (DTSHD)", 1, 0); break;
		case LPCM: tmpstr = ostrcat(tmpstr, " (LPCM)", 1, 0); break;
		case AAC: tmpstr = ostrcat(tmpstr, " (AAC)", 1, 0); break;
		case AACHE: tmpstr = ostrcat(tmpstr, " (AACHE)", 1, 0); break;
		case DDP: tmpstr = ostrcat(tmpstr, " (DDP/AC3Plus)", 1, 0); break;
	}

	newnode->name = tmpstr;

	m_lock(&status.audiotrackmutex, 7);
	node = chnode->audiotrack;
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
		chnode->audiotrack = newnode;
	else
		prev->next = newnode;

	newnode->next = node;

	m_unlock(&status.audiotrackmutex, 7);
	return newnode;
}

void freeaudiotrack(struct channel* chnode)
{
	struct audiotrack *node = NULL, *prev = NULL;

	if(chnode == NULL)
	{
		err("NULL detect");
		return;
	}

	m_lock(&status.audiotrackmutex, 7);
	node = chnode->audiotrack;
	prev = chnode->audiotrack;

	while(node != NULL)
	{
		prev = node;
		node = node->next;
		chnode->audiotrack = node;

		free(prev->name);
		prev->name = NULL;

		free(prev);
		prev = NULL;

	}
	m_unlock(&status.audiotrackmutex, 7);
}

#endif