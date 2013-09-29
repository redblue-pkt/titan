#ifndef EIT_H
#define EIT_H

void debugepg()
{
	struct channel* chnode = channel;
	struct epg* epgnode = NULL;

	while(chnode != NULL)
	{
		if(chnode->epg != NULL)
		{
			printf("\nChannel %s\n", chnode->name);
			epgnode = chnode->epg;
			while(epgnode != NULL)
			{
				printf("EventID: %d\n", epgnode->eventid);
				printf("Start: %ld\n", epgnode->starttime);
				printf("Stop: %ld\n", epgnode->endtime);
				printf("Title: %s\n", epgnode->title);
				printf("Subtitle: %s\n", epgnode->subtitle);
				//printf("Desc: %s\n", epgnode->desc);
				epgnode = epgnode->next;
			}
			printf("\n");
		}
		chnode = chnode->next;
	}
}

char* createwriteepg(int* buflen)
{
	struct channel *chnode = channel;
	struct epg* epgnode = NULL;
	char* buf = NULL;
	int len = 0, newlen = 0;

	m_lock(&status.epgmutex, 4);

	while(chnode != NULL)
	{
		epgnode = chnode->epg;
		while(epgnode != NULL)
		{
			if(*buflen + (MINMALLOC * 2) >= newlen)
			{
				newlen = *buflen + (MINMALLOC * 100);
				buf = realloc(buf, newlen);
				if(buf == NULL)
				{
					err("no mem");
					*buflen = 0;
					m_unlock(&status.epgmutex, 4);
					return NULL;
				}
			}

			memcpy(buf + *buflen, &chnode->serviceid, sizeof(int));
			*buflen += sizeof(int);
			memcpy(buf + *buflen, &chnode->transponderid, sizeof(uint64_t));
			*buflen += sizeof(uint64_t);
			memcpy(buf + *buflen, &epgnode->eventid, sizeof(int));
			*buflen += sizeof(int);
			memcpy(buf + *buflen, &epgnode->version, sizeof(int));
			*buflen += sizeof(int);
			memcpy(buf + *buflen, &epgnode->parentalrating, sizeof(int));
			*buflen += sizeof(int);
			memcpy(buf + *buflen, &epgnode->starttime, sizeof(time_t));
			*buflen += sizeof(int);
			memcpy(buf + *buflen, &epgnode->endtime, sizeof(time_t));
			*buflen += sizeof(int);

			len = 0;
			if(epgnode->title != NULL)
				len = strlen(epgnode->title);
			memcpy(buf + *buflen, &len, sizeof(int));
			*buflen += sizeof(int);
			if(epgnode->title != NULL && len > 0)
			{
				memcpy(buf + *buflen, epgnode->title, len);
				*buflen += len;
			}

			len = 0;
			if(epgnode->subtitle != NULL)
				len = strlen(epgnode->subtitle);
			memcpy(buf + *buflen, &len, sizeof(int));
			*buflen += sizeof(int);
			if(epgnode->subtitle != NULL && len > 0)
			{
				memcpy(buf + *buflen, epgnode->subtitle, len);
				*buflen += len;
			}

			if(epgnode->desc == NULL)
			{
				epgnode->desclen = 0;
				epgnode->desccomplen = 0;
			}
			memcpy(buf + *buflen, &epgnode->desclen, sizeof(int));
			*buflen += sizeof(int);
			memcpy(buf + *buflen, &epgnode->desccomplen, sizeof(int));
			*buflen += sizeof(int);

			len = epgnode->desccomplen;
			if(len == 0)
				len = epgnode->desclen;
			if(epgnode->desc != NULL)
			{
				memcpy(buf + *buflen, epgnode->desc, len);
				*buflen += len;
			}

			epgnode = epgnode->next;
		}

		chnode = chnode->next;
	}

	m_unlock(&status.epgmutex, 4);
	return buf;
}

int writeepgfast(const char* filename, char* buf, int buflen)
{
	FILE *fd = NULL;
	int ret;

	if(buf == NULL || buflen == 0) return 1;

	fd = fopen(filename, "wbt");
	if(fd == NULL)
	{
		perr("can't open %s", filename);
		return 1;
	}

	unsigned long long freespace = getfreespace((char*)filename);
	unsigned long long epgfreespace = getconfigint("epgfreespace", NULL) * 1024;

	if(freespace - buflen < epgfreespace)
	{
		err("fastwrite out of space freespace=%llu epgfreespace=%llu (%s)", freespace - buflen, epgfreespace, filename);
		fclose(fd);
		return 1;
	}

	ret = fwrite(buf, buflen, 1, fd);

	if(ret != 1)
	{
		perr("fast writting file %s", filename);
		fclose(fd);
		return 1;
	}

	fclose(fd);
	return 0;
}

int writeepgslow(const char* filename)
{
	FILE *fd = NULL;
	struct channel *chnode = channel;
	struct epg* epgnode = NULL;
	int ret = 0, len = 0, count = 0, checkcount = 0, err = 0;
	
	m_lock(&status.epgmutex, 4);

	fd = fopen(filename, "wbt");
	if(fd == NULL)
	{
		perr("can't open %s", filename);
		m_unlock(&status.epgmutex, 4);
		return 1;
	}

	unsigned long long freespace = getfreespace((char*)filename);
	unsigned long long epgfreespace = getconfigint("epgfreespace", NULL) * 1024;

	while(chnode != NULL)
	{
		epgnode = chnode->epg;
		while(epgnode != NULL)
		{	
			ret = 0, count = 0;
			ret += fwrite(&chnode->serviceid, sizeof(int), 1, fd); count++;
			freespace -= sizeof(int);
			ret += fwrite(&chnode->transponderid, sizeof(uint64_t), 1, fd); count++;
			freespace -= sizeof(uint64_t);
			ret += fwrite(&epgnode->eventid, sizeof(int), 1, fd); count++;
			freespace -= sizeof(int);
			ret += fwrite(&epgnode->version, sizeof(int), 1, fd); count++;
			freespace -= sizeof(int);
			ret += fwrite(&epgnode->parentalrating, sizeof(int), 1, fd); count++;
			freespace -= sizeof(int);
			ret += fwrite(&epgnode->starttime, sizeof(time_t), 1, fd); count++;
			freespace -= sizeof(time_t);
			ret += fwrite(&epgnode->endtime, sizeof(time_t), 1, fd); count++;
			freespace -= sizeof(time_t);

			len = 0;
			if(epgnode->title != NULL)
				len = strlen(epgnode->title);
			ret += fwrite(&len, sizeof(int), 1, fd); count++;
			freespace -= sizeof(int);
			if(epgnode->title != NULL && len > 0)
			{
				ret += fwrite(epgnode->title, len, 1, fd);
				freespace -= len;
				count++;
			}

			len = 0;
			if(epgnode->subtitle != NULL)
				len = strlen(epgnode->subtitle);
			ret += fwrite(&len, sizeof(int), 1, fd); count++;
			freespace -= sizeof(int);
			if(epgnode->subtitle != NULL && len > 0)
			{
				ret += fwrite(epgnode->subtitle, len, 1, fd);
				freespace -= len;
				count++;
			}

			if(epgnode->desc == NULL)
			{
				epgnode->desclen = 0;
				epgnode->desccomplen = 0;
			}
			ret += fwrite(&epgnode->desclen, sizeof(int), 1, fd); count++;
			freespace -= sizeof(int);
			ret += fwrite(&epgnode->desccomplen, sizeof(int), 1, fd); count++;
			freespace -= sizeof(int);

			len = epgnode->desccomplen;
			if(len == 0)
				len = epgnode->desclen;
			if(epgnode->desc != NULL)
			{
				ret += fwrite(epgnode->desc, len, 1, fd);
				freespace -= len;
				count++;
			}

			if(ret != count)
			{
				perr("writting file %s", filename);
				if(errno == ENOSPC)
				{
					err = 1;
					break;
				}
			}
			
			//recheck freespace
			checkcount++;
			if(checkcount > 10) 
			{
				checkcount = 0;
				freespace = getfreespace((char*)filename);
			}

			if(freespace < epgfreespace)
				break;
			epgnode = epgnode->next;
		}
		if(err == 1) break;
		if(freespace < epgfreespace)
		{
			err("not all data written freespace=%llu epgfreespace=%llu (%s)", freespace, epgfreespace, filename);
			break;
		}
		chnode = chnode->next;
	}

	fclose(fd);
	if(err == 1) unlink(filename);
	m_unlock(&status.epgmutex, 4);
	return 0;
}

int writeepg(const char* filename)
{
	int ret = 0, buflen = 0;
	char* buf = NULL;

	buf = createwriteepg(&buflen);
	if(buf != NULL)
	{
		ret = writeepgfast(filename, buf, buflen);
		free(buf); buf = NULL;
    
    if(ret != 0) ret = writeepgslow(filename);
	}
	else
		ret = writeepgslow(filename);

	return ret;
}

char* epgdescunzip(struct epg* epgnode)
{
	int ret = 0, zlen = 0;
	char* zbuf = NULL;

	if(epgnode == NULL)
	{
		err("NULL detect");
		return NULL;
	}

	if(epgnode->desccomplen < 1)
		return ostrcat(epgnode->desc, NULL, 0, 0);

	ret = ounzip(epgnode->desc, epgnode->desccomplen, &zbuf, &zlen, MINMALLOC, 0);
	if(ret == 0)
		return zbuf;

	return NULL;
}

struct epg* getepgbytime(struct channel* chnode, time_t akttime)
{
	m_lock(&status.epgmutex, 4);

	if(chnode == NULL || chnode->epg == NULL)
	{
		m_unlock(&status.epgmutex, 4);
		return NULL;
	}

	struct epg *node = chnode->epg;
	while(node != NULL)
	{
		if(node->starttime <= akttime && node->endtime > akttime) 
		{
			m_unlock(&status.epgmutex, 4);
			return node;
		}

		node = node->next;
	}

	m_unlock(&status.epgmutex, 4);
	return NULL;
}

struct epg* getepgnext(struct channel* chnode)
{
	time_t akttime = time(NULL);

	m_lock(&status.epgmutex, 4);

	if(chnode == NULL || chnode->epg == NULL)
	{
		m_unlock(&status.epgmutex, 4);
		return NULL;
	}

	struct epg *node = chnode->epg;
	while(node != NULL)
	{
		if(node->starttime >= akttime)
		{
			m_unlock(&status.epgmutex, 4);
			return node;
		}

		node = node->next;
	}

	m_unlock(&status.epgmutex, 4);
	return NULL;
}

struct epg* getepgakt(struct channel* chnode)
{
#ifdef SIMULATE
	time_t akttime = 1307871000;
	//akttime = 1315614900;
	//akttime = 1317927300;
	//akttime = 1307971000;
#else
	time_t akttime = time(NULL);
#endif
	return getepgbytime(chnode, akttime);
}

//flag 0: lock
//flag 1: nolock
struct epg* getepg(struct channel* chnode, int eventid, int flag)
{
	if(chnode == NULL) return NULL;

	if(flag == 0) m_lock(&status.epgmutex, 4);

	struct epg *node = chnode->epg;

	while(node != NULL)
	{
		if(node->eventid == eventid)
		{
			if(flag == 0) m_unlock(&status.epgmutex, 4);
			return node;
		}

		node = node->next;
	}
	if(flag == 0) m_unlock(&status.epgmutex, 4);
	return NULL;
}

//flag 0: lock
//flag 1: nolock
struct epg* updateepg(struct channel* chnode, struct epg* epgnode, int eventid, int version, time_t starttime, time_t endtime, int flag)
{
//	debug(1000, "in");
	int changetime = 0;

	if(chnode == NULL || epgnode == NULL) return NULL;

	//if(epgnode->version >= version) return epgnode;
	if(endtime <= time(NULL)) return epgnode;

	if(flag == 0) m_lock(&status.epgmutex, 4);

	struct epg *prev = NULL, *node = chnode->epg;

	epgnode->eventid = eventid;
	epgnode->version = version;
	if(epgnode->starttime != starttime)
	{
		changetime = 1;
		epgnode->starttime = starttime;
	}
	epgnode->endtime = endtime;

	if(changetime == 1)
	{
		if(chnode->epg == epgnode)
		{
			chnode->epg = epgnode->next;
			node = chnode->epg;
		}

		if(epgnode->prev != NULL) epgnode->prev->next = epgnode->next;
		if(epgnode->next != NULL) epgnode->next->prev = epgnode->prev;
		epgnode->prev = NULL;
		epgnode->next = NULL;

		while(node != NULL && epgnode->starttime >= node->starttime)
		{
			prev = node;
			node = node->next;
		}

		//check if new epg overlapps prev epg
		if(prev != NULL && epgnode->starttime < prev->endtime)
		{
			struct epg *tmp = prev->prev;
			addoldentryepg(chnode, prev, 1);
			prev = tmp;
		}
		//check if new epg overlaps next epg
		while(node != NULL && epgnode->endtime > node->starttime)
		{
			struct epg *tmp = node;
			node = node->next;
			addoldentryepg(chnode, tmp, 1);
		}

		if(prev == NULL)
			chnode->epg = epgnode;
		else
		{
			prev->next = epgnode;
			epgnode->prev = prev;
		}
		epgnode->next = node;
		if(node != NULL) node->prev = epgnode;
	}

	if(flag == 0) m_unlock(&status.epgmutex, 4);
	return epgnode;
}

//flag 0: lock
//flag 1: nolock
struct epg* addoldentryepg(struct channel* chnode, struct epg* newnode, int flag)
{
	if(newnode == NULL) return NULL;

	if(flag == 0) m_lock(&status.epgmutex, 4);

	clearepgentry(newnode);

	if(newnode == chnode->epg)
	{
		chnode->epg = newnode->next;
		if(chnode->epg != NULL)
			chnode->epg->prev = NULL;
	}
	else
	{
		if(newnode->prev != NULL)
		{
			newnode->prev->next = newnode->next;
			if(newnode->next != NULL)
				newnode->next->prev = newnode->prev;
		}
	}

	if(flag == 0) m_unlock(&status.epgmutex, 4);

	addoldentry((void*)newnode, 0, time(NULL) + 3600, NULL);

	return newnode;
}

//flag 0: lock
//flag 1: nolock
struct epg* addepg(struct channel* chnode, int eventid, int version, time_t starttime, time_t endtime, struct epg* last, int flag)
{
	if(chnode == NULL) return NULL;

	if(flag == 0) m_lock(&status.epgmutex, 4);

	struct epg *newnode = NULL, *prev = NULL, *node = chnode->epg;

	newnode = (struct epg*)malloc(sizeof(struct epg));	
	if(newnode == NULL)
	{
		err("no memory");
		if(flag == 0) m_unlock(&status.epgmutex, 4);
		return NULL;
	}

	memset(newnode, 0, sizeof(struct epg));

	newnode->eventid = eventid;
	newnode->version = version;
	newnode->starttime = starttime;
	newnode->endtime = endtime;

	if(last == NULL)
	{
		while(node != NULL && newnode->starttime >= node->starttime)
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

	//check if new epg overlaps next epg
	while(node != NULL && newnode->endtime > node->starttime)
	{
		struct epg *tmp = node;
		node = node->next;
		addoldentryepg(chnode, tmp, 1);
	}
	//check if new epg overlapps prev epg
	if(prev != NULL && newnode->starttime < prev->endtime)
	{
		clearepgentry(prev);
		prev->eventid = eventid;
		prev->version = version;
		prev->starttime = starttime;
		prev->endtime = endtime;
		prev->parentalrating = 0;

		free(newnode); newnode = NULL;
		if(flag == 0) m_unlock(&status.epgmutex, 4);
		return prev;
	}

	if(prev == NULL)
		chnode->epg = newnode;
	else
	{
		prev->next = newnode;
		newnode->prev = prev;
	}
	newnode->next = node;
	if(node != NULL) node->prev = newnode;

	if(flag == 0) m_unlock(&status.epgmutex, 4);
	return newnode;
}

void deloldentryepg(struct epg* node)
{
	if(node == NULL) return;

	clearepgentry(node);

	free(node);
	node = NULL;
}

void clearepgentry(struct epg* node)
{
	if(node == NULL) return;

	free(node->title);
	node->title = NULL;
	free(node->subtitle);
	node->subtitle = NULL;
	free(node->desc);
	node->desc = NULL;
	node->desclen = 0;
	node->desccomplen = 0;
}

//flag 0: lock
//flag 1: nolock
void delepg(struct channel* chnode, struct epg* epgnode, int flag)
{
	if(chnode == NULL) return;

	if(flag == 0) m_lock(&status.epgmutex, 4);

	struct epg *node = chnode->epg, *prev = chnode->epg;

	while(node != NULL)
	{
		if(node == epgnode)
		{
			if(node == chnode->epg)
			{
				chnode->epg = node->next;
				if(chnode->epg != NULL)
					chnode->epg->prev = NULL;
			}
			else
			{
				prev->next = node->next;
				if(prev->next != NULL)
					prev->next->prev = prev;
			}

			clearepgentry(node);

			free(node);
			node = NULL;
			break;
		}

		prev = node;
		node = node->next;
	}
	if(flag == 0) m_unlock(&status.epgmutex, 4);
}

void freeepg(struct channel* chnode)
{
	if(chnode == NULL) return;

	m_lock(&status.epgmutex, 4);
	struct epg *node = chnode->epg, *prev = chnode->epg;

	while(node != NULL)
	{
		prev = node;
		node = node->next;
		if(prev != NULL)
			delepg(chnode, prev, 1);
	}
	m_unlock(&status.epgmutex, 4);
}

//flag 0: stop scanlist and delete epg.dat
//flag 1: don't stop scanlist, don't delete epg.dat
void resetepg(int flag)
{
	struct channel *node = channel;
	char* tmpstr = NULL;

	//stop epgscanlistthread
	if(flag == 0 && status.epgscanlistthread != NULL)
		status.epgscanlistthread->aktion = STOP;

	if(status.epgthread != NULL)
		status.epgthread->aktion = PAUSE;

	//wait a little for thread end/pause
	sleep(1);

	while(node != NULL)
	{
		freeepg(node);
		node = node->next;
	}

	if(flag == 0)
	{
		tmpstr = createpath(getconfig("epg_path", NULL), "epg.dat");
		unlink(tmpstr);
		free(tmpstr); tmpstr = NULL;
	}

	//set lastepg to 0 so, epg is running
	deltransponderlastepg();

	if(status.epgthread != NULL)
		status.epgthread->aktion = START;
}

void deloldepg()
{
	struct channel* chnode = channel;
	struct epg* epgnode = NULL;

	m_lock(&status.epgmutex, 4);
	while(chnode != NULL)
	{
		if(chnode->epg != NULL)
		{
			epgnode = channel->epg;
			while(epgnode != NULL)
			{
				if(epgnode->endtime + 60 < time(NULL))
					delepg(chnode, epgnode, 1);
				epgnode = epgnode->next;
			}
		}
		chnode = chnode->next;
	}
	m_unlock(&status.epgmutex, 4);

	deloldlinkedchannel();
}

/*
//Parse language-id translation file
char *eitlang(u_char *l)
{
	static union lookup_key lang;
	lang.c[0] = (char)l[0];
	lang.c[1] = (char)l[1];
	lang.c[2] = (char)l[2];
	lang.c[3] = '\0';

	char *c = lookup(languageid_table, lang.i);
	return c ? c : lang.c;
}
*/

//Parse 0x4D Short Event Descriptor
void eventdesc(struct channel* chnode, struct epg* epgnode, void *buf, unsigned char* lang)
{
	struct transponder* tpnode = NULL;
	struct eitshortevent *evtdesc = buf;
	char* title = NULL, *subtitle = NULL;
	int evtlen = evtdesc->event_name_length;
	int dsclen = 0;

	if(!evtlen) return;
	if(chnode != NULL) tpnode = chnode->transponder;

	//title
	title = malloc(evtlen + 1);
	if(title == NULL)
	{
		err("no mem");
		return;
	}
	
	lang[0] = evtdesc->lang_code1;
	lang[1] = evtdesc->lang_code2;
	lang[2] = evtdesc->lang_code3;
	
	memcpy(title, (char *)&evtdesc->data, evtlen);
	title[evtlen] = '\0';
	free(epgnode->title);
	epgnode->title = strutf8(tpnode, title, evtlen, 0, 1, 0);

	//subtitle
	dsclen = evtdesc->data[evtlen];
	subtitle = malloc(dsclen + 1);
	if(subtitle == NULL)
	{
		err("no mem");
		return;
	}
	memcpy(subtitle, (char *)&evtdesc->data[evtlen + 1], dsclen);
	subtitle[dsclen] = '\0';
	free(epgnode->subtitle);
	epgnode->subtitle = strutf8(tpnode, subtitle, dsclen, 0, 1, 0);
}

void epgadddesc(struct epg* epgnode, char* desc)
{
	int len = 0;
	
	if(desc != NULL) len = strlen(desc);
	epgnode->desclen += len;

	epgnode->desc = realloc(epgnode->desc, epgnode->desclen + 1);
	if(epgnode->desc == NULL)
	{
		err("no mem");
		epgnode->desclen = 0;
		return;
	}
	memcpy(epgnode->desc + epgnode->desclen - len, desc, len + 1);
}

// Parse 0x4E Extended Event Descriptor
void longeventdesc(struct channel* chnode, struct epg* epgnode, unsigned char *buf, unsigned char* lang)
{
	struct transponder* tpnode = NULL;
	struct eitlongevent *levt = (struct eitlongevent*)buf;
	char* desc = NULL;

  if(lang[0] != '\0')
	{
		if(lang[0] != levt->lang_code1 || lang[1] != levt->lang_code2 || lang[2] != levt->lang_code3)
			return;
	}

	if(chnode != NULL) tpnode = chnode->transponder;
	int nonempty = (levt->descriptor_number || levt->last_descriptor_number || levt->length_of_items || levt->data[0]);

	void *p = &levt->data;
	while(p < (void *)levt->data + levt->length_of_items)
	{
		struct eitlongeventitem *name = p;
		int namelen = name->item_description_length;
		desc = malloc(namelen + 1);
		if(desc == NULL)
		{
			err("no mem");
			return;
		}
		memcpy(desc, (char *)&name->data, namelen);
		desc[namelen] = '\0';
		desc = strutf8(tpnode, desc, namelen, 0, 1, 0);
		epgadddesc(epgnode, desc);
		free(desc); desc = NULL;
		p += EITLONGEVENTITEMLEN + namelen;

		struct eitlongeventitem *value = p;
		int valuelen = value->item_description_length;
		desc = malloc(valuelen + 1);
		if(desc == NULL)
		{
			err("no mem");
			return;
		}
		memcpy(desc, (char *)&value->data, valuelen);
		desc[valuelen] = '\0';
		desc = strutf8(tpnode, desc, valuelen, 0, 1, 0);
		epgadddesc(epgnode, desc);
		free(desc); desc = NULL;
		p += EITLONGEVENTITEMLEN + valuelen;
	}

	struct eitlongeventitem *text = p;
	int textlen = text->item_description_length;
	if(nonempty && textlen)
	{
		desc = malloc(textlen + 1);
		if(desc == NULL)
		{
			err("no mem");
			return;
		}
		memcpy(desc, (char *)&text->data, textlen);
		desc[textlen] = '\0';
		desc = strutf8(tpnode, desc, textlen, 0, 1, 0);
		epgadddesc(epgnode, desc);
		free(desc); desc = NULL;
	}

	free(desc); desc = NULL;
}

// Parse 0x55 Rating Descriptor
void ratingdescr(struct epg* epgnode, unsigned char *buf)
{
	struct parentalrating *pr = (struct parentalrating*)buf;
	void *p = NULL;

	for(p = &pr->data; p < (void*)buf + pr->descriptor_length; p += PARENTALRATINGITEMLEN)
	{
		struct parentalratingitem *pr = p;
		switch(pr->rating)
		{
			case 0x00: //undefined
				break;
			case 0x01 ... 0x0F:
				epgnode->parentalrating = pr->rating + 3;
				break;
			case 0x10 ... 0xFF: //broadcaster defined
				break;
		}
	}
}

// Parse 0x4A Linkage Descriptor
void linkagedesc(struct channel* chnode, struct epg* epgnode, void *buf)
{
	struct eitlinkage *evtlink = buf;
	uint64_t transponderid = 0;
	
	int tid = HILO(evtlink->transport_stream_id);
	int onid = HILO(evtlink->original_network_id);
	int serviceid = HILO(evtlink->service_id);
	
	transponderid = ((onid << 16) | tid) & 0xffffffff;

	if(chnode != NULL && chnode->transponder != NULL)
	{
		if(chnode->transponder->fetype == FE_QAM)
			transponderid = transponderid | ((uint64_t)1 << 32);
		else if(chnode->transponder->fetype == FE_OFDM)
			transponderid = transponderid | ((uint64_t)2 << 32);

		if(getlinkedchannel(chnode, serviceid, transponderid, epgnode->starttime, epgnode->endtime) == NULL)
			addlinkedchannel(chnode, serviceid, transponderid, epgnode->starttime, epgnode->endtime, NULL);
	}
}

#if 0
// Parse 0x50 Component Descriptor
void componentdescr(struct epg* epgnode, unsigned char *buf)
{
	struct eitcomponent *dc = buf;

	switch(dc->stream_content)
	{
		case 0x01: // Video Info
			//if ((dc->component_type - 1) & 0x08) //HD TV
			//if ((dc->component_type - 1) & 0x04) //30Hz else 25
			printf("aspect %s\n", lookup(aspect_table, (dc->component_type - 1) & 0x03));
			break;
		case 0x02: // Audio Info
			printf("stereo %s\n", lookup(audio_table, (dc->component_type)));
			printf("language %d\n", dc->lang_code1);
			break;
		case 0x03: // Teletext Info
			// if ((dc->component_type) & 0x10) //subtitles
			// if ((dc->component_type) & 0x20) //subtitles for hard of hearing
			printf("txt language %d\n", dc->lang_code1);
			break;
		// case 0x04: // AC3 info
	}
#if 0
	printf("StreamContent %d\n", dc->stream_content);
	printf("ComponentType %x\n", dc->component_type);
	printf("ComponentTag %x\n", dc->component_tag);
	printf("Length %d\n", dc->descriptor_length - 6);
#endif
}

inline void eitsetbit(int *bf, int b)
{
	int i = b / 8 / sizeof(int);
	int s = b % (8 * sizeof(int));
	bf[i] |= (1 << s);
}

inline bool eitgetbit(int *bf, int b)
{
	int i = b / 8 / sizeof(int);
	int s = b % (8 * sizeof(int));
	return bf[i] & (1 << s);
}

// Parse 0x54 Content Descriptor
void contentdescr(struct epg* epgnode, unsigned char *buf)
{
	struct eitcontent *dc = buf;

	int once[256/8/sizeof(int)] = {0,};
	void *p;
	for(p = &dc->data; p < data + dc->descriptor_length; p += NIBBLE_CONTENT_LEN)
	{
		struct nibble_content *nc = p;
		int c1 = (nc->content_nibble_level_1 << 4) + nc->content_nibble_level_2;
#ifdef CATEGORY_UNKNOWN
		int c2 = (nc->user_nibble_1 << 4) + nc->user_nibble_2;
#endif
		if(c1 > 0 && !eitgetbit(once, c1))
		{
			eitsetbit(once, c1);
			char *c = lookup(description_table, c1);
			if(c)
				if(c[0])
					printf("\t<category>%s</category>\n", c);
#ifdef CATEGORY_UNKNOWN
				else
					printf("\t<!--category>%s %02X %02X</category-->\n", c+1, c1, c2);
			else
				printf("\t<!--category>%02X %02X</category-->\n", c1, c2);
#endif
		}
		// This is weird in the uk, they use user but not content, and almost the same values
	}
}

// Parse 0x5F Private Data Specifier
int privatedataspecifier(struct epg* epgnode, unsigned char *buf)
{
	return GetPrivateDataSpecifier(data);
}

// Parse 0x76 Content Identifier Descriptor
void contentidentifierdesc(struct epg* epgnode, unsigned char *buf)
{
	struct descr_content_identifier *ci = data;
	void *p;
	for(p = &ci->data; p < data + ci->descriptor_length; /* at end */)
	{
		struct descr_content_identifier_crid *crid = p;
		struct descr_content_identifier_crid_local *crid_data;

		int crid_length = 3;

		char type_buf[32];
		char *type;
		char buf[256];

		type = lookup(crid_type_table, crid->crid_type);
		if(type == NULL)
		{
			type = type_buf;
			sprintf(type_buf, "0x%2x", crid->crid_type);
		}

		switch(crid->crid_location)
		{
			case 0x00: /* Carried explicitly within descriptor */
				crid_data = (descr_content_identifier_crid_local_t *)&crid->crid_ref_data;
				int cridlen = crid_data->crid_length;
				assert(cridlen < sizeof(buf));
				memcpy(buf, (char *)&crid_data->crid_byte, cridlen);
				buf[cridlen] = '\0';

				printf("\t<crid type='%s'>%s</crid>\n", type, xmlify(buf));
				crid_length = 2 + crid_data->crid_length;
				break;
			case 0x01: /* Carried in Content Identifier Table (CIT) */
				break;
			default:
				break;
		}
		p += crid_length;
	}
}
#endif

//Parse Descriptor
void parseeitdesc(struct channel* chnode, struct epg* epgnode, unsigned char *buf, int len, int nolongdesc)
{
	unsigned char *p;
	unsigned char lang[3] = {'\0', '\0', '\0'};
	
	for(p = buf; p < buf + len; p += 2 + p[1])
	{
		switch((int)p[0])
		{
			case 0:
				break;
			case 0x4A:
				linkagedesc(chnode, epgnode, p);
				break;
			case 0x4D:
				eventdesc(chnode, epgnode, p, lang);
				break;
			case 0x4E:
				if(nolongdesc == 0)
					longeventdesc(chnode, epgnode, p, lang);
				break;
			case 0x55:
				ratingdescr(epgnode, p);
				break;
/*
			case 0x50:
				componentdescr(epgnode, p, LANGUAGE);
				componentdescr(epgnode, p, VIDEO);
				componentdescr(epgnode, p, AUDIO);
				componentdescr(epgnode, p, SUBTITLES);
				break;
			case 0x53: // CA Identifier Descriptor
				break;
			case 0x54:
				contentdescr(epgnode, p);
				break;
			case 0x5f: // Private Data Specifier
				pds = privatedataspecifier(epgnode, p);
				break;
			case 0x64: // Data broadcast desc - Text Desc for Data components
				break;
			case 0x69: // Programm Identification Label
				break;
			case 0x81: // TODO ???
				if(pds == 5) // ARD_ZDF_ORF
					break;
			case 0x82: // VPS (ARD, ZDF, ORF)
				if(pds == 5) // ARD_ZDF_ORF
					// TODO: <programme @vps-start="???">
					break;
			case 0x4F: // Time Shifted Event
			case 0x52: // Stream Identifier Descriptor
			case 0x5E: // Multi Lingual Component Descriptor
			case 0x83: // Logical Channel Descriptor (some kind of news-ticker on ARD-MHP-Data?)
			case 0x84: // Preferred Name List Descriptor
			case 0x85: // Preferred Name Identifier Descriptor
			case 0x86: // Eacem Stream Identifier Descriptor
				break;
			case 0x76:
				contentidentifierdesc(epgnode, p);
				break;
*/
		}
	}
}

// Parse Event Information Table
//flag 0 = from epg thread
//flag 1 = from epg scan
void parseeit(struct channel* chnode, unsigned char *buf, int len, int flag)
{
	struct eit* eit = (struct eit*)buf;
	unsigned char *p;
	time_t dvbtime = 0, starttime = 0, endtime = 0;
	struct channel* tmpchnode = NULL;
	struct epg* epgnode = NULL;
	char* zbuf = NULL;
	int zlen = 0, ret = 0, eitlen = 0;
	time_t epgmaxsec = status.epgdays * 24 * 60 * 60;
	uint64_t transponderid = 0;
	int serviceid = 0, eventid = 0;
	int nolongdesc = 0;

	len -= 4; //remove CRC
	if(chnode == NULL) chnode = status.aktservice->channel;

	// For each event listing
	for (p = (unsigned char*)&eit->data; p < buf + len; p += EITEVENTLEN + GETEITDESCLEN(p))
	{
		nolongdesc = 0;
		struct eitevent* evt = (struct eitevent*)p;

		// No program info at end! Just skip it
		eitlen = GETEITDESCLEN(evt);
		if(eitlen == 0 || chnode == NULL)
			return;
		if(flag == 0 && chnode == status.aktservice->channel && status.aktservice->type != CHANNEL)
			return;
		
		transponderid = ((HILO(eit->original_network_id) << 16) | HILO(eit->transport_stream_id)) & 0xffffffff;
		if(chnode->transponder != NULL)
		{
			if(chnode->transponder->fetype == FE_QAM)
				transponderid = transponderid | ((uint64_t)1 << 32);
			else if(chnode->transponder->fetype == FE_OFDM)
				transponderid = transponderid | ((uint64_t)2 << 32);
		}

		serviceid = HILO(eit->service_id);
		tmpchnode = getchannel(serviceid, transponderid);
		if(tmpchnode == NULL)
		{
			debug(1000, "out -> NULL detect");
			continue;
		}

		//look in epglist if channel exists
		//only if epglist is marked as whitelist
		if(status.epglistmode == 2 || status.epglistmode == 3)
		{
			if(getepgscanlist(serviceid, transponderid) == NULL)
				continue;
		}

		m_lock(&status.epgmutex, 4);
		eventid = HILO(evt->event_id);
		epgnode = getepg(tmpchnode, eventid, 1);

		dvbtime = dvbconvertdate(&evt->mjd_hi, 0);
		starttime = dvbtime;
		starttime -= starttime % 60;

		dvbtime += bcdtoint(evt->duration_s & 0xff);
		dvbtime += (bcdtoint(evt->duration_m & 0xff) * 60);
		dvbtime += (bcdtoint(evt->duration_h & 0xff) * 60 * 60);
		endtime = dvbtime;
		endtime -= endtime % 60;

#ifndef SIMULATE
		if(endtime < time(NULL) || starttime > time(NULL) + epgmaxsec)
		{
			m_unlock(&status.epgmutex, 4);
			continue;
		}
#endif

		if(epgnode == NULL)
				epgnode = addepg(tmpchnode, eventid, eit->version_number, starttime, endtime, NULL, 1);
		else
		{
			updateepg(tmpchnode, epgnode, eventid, eit->version_number, starttime, endtime, 1);
			if(epgnode->desc != NULL)
				nolongdesc = 1;
		}

		if(epgnode == NULL)
		{
			m_unlock(&status.epgmutex, 4);
			continue;
		}

		//1 Airing, 2 Starts in a few seconds, 3 Pausing, 4 About to air
		//printf("RunningStatus %d\n", evt->running_status);

		parseeitdesc(tmpchnode, epgnode, (unsigned char*)&evt->data, eitlen, nolongdesc);
		//compress long desc
		if(nolongdesc == 0 && epgnode->desc != NULL)
		{
			ret = ozip(epgnode->desc, strlen(epgnode->desc) + 1, &zbuf, &zlen, 1);
			if(ret == 0)
			{
				free(epgnode->desc); epgnode->desc = NULL;
				epgnode->desc = zbuf;
				epgnode->desccomplen = zlen;
			}
		}
		m_unlock(&status.epgmutex, 4);
	}
}

int readepg(const char* filename)
{
	FILE *fd = NULL;
	struct channel *chnode = channel, *oldchnode = NULL;
	struct epg* epgnode = NULL;
	int ret = 0, len = 0, count = 0;
	time_t epgmaxsec = status.epgdays * 24 * 60 * 60;

	fd = fopen(filename, "rb");
	if(fd == NULL)
	{
		perr("can't open %s", filename);
		return 1;
	}
	
	m_lock(&status.epgmutex, 4);

	while(!feof(fd))
	{
		int serviceid = 0, eventid = 0, version = 0;
		uint64_t transponderid = 0;
		time_t starttime = 0, endtime = 0;
		int desclen = 0, desccomplen = 0, parentalrating = 0;
		char* title = NULL, *subtitle = NULL, *desc = NULL;

		ret = fread(&serviceid, sizeof(int), 1, fd);
		if(feof(fd)) break;
		ret = fread(&transponderid, sizeof(uint64_t), 1, fd);
		ret = fread(&eventid, sizeof(int), 1, fd);
		ret = fread(&version, sizeof(int), 1, fd);
		ret = fread(&parentalrating, sizeof(int), 1, fd);
		ret = fread(&starttime, sizeof(time_t), 1, fd);
		ret = fread(&endtime, sizeof(time_t), 1, fd);

		//if epg.dat has an IO Error it runs in endless loop
		//this stops the endless loop
		if(starttime == 0 && endtime == 0)
			count++;
		else
			count = 0;
		if(count > 50) break;

		len = 0;
		ret = fread(&len, sizeof(int), 1, fd);
		if(len > 0 && len <= 4096)
		{
			title = malloc(len + 1);
			if(title == NULL)
			{
				err("title no mem %d", len);
				free(title); title = NULL;
				free(subtitle); subtitle = NULL;
				free(desc); desc = NULL;
				break;
			}
			ret = fread(title, len, 1, fd);
			title[len] = '\0';
		}
		else if(len != 0)
		{
			err("epgfile defekt title len=%d", len);
			free(title); title = NULL;
			free(subtitle); subtitle = NULL;
			free(desc); desc = NULL;
			break;
		}
	
		len = 0;
		ret = fread(&len, sizeof(int), 1, fd);
		if(len > 0 && len <= 4096)
		{
			subtitle = malloc(len + 1);
			if(subtitle == NULL)	
			{
				err("subtitle no mem %d", len);
				free(title); title = NULL;
				free(subtitle); subtitle = NULL;
				free(desc); desc = NULL;
				break;
			}
			ret = fread(subtitle, len, 1, fd);
			subtitle[len] = '\0';
		}
		else if(len != 0)
		{
			err("epgfile defekt subtitle len=%d", len);
			free(title); title = NULL;
			free(subtitle); subtitle = NULL;
			free(desc); desc = NULL;
			break;
		}

		ret = fread(&desclen, sizeof(int), 1, fd);
		ret = fread(&desccomplen, sizeof(int), 1, fd);
	
		len = desccomplen;
		if(len == 0)
			len = desclen;
		if(len > 0 && len <= 4096)
		{
			if(desccomplen == 0)
				desc = malloc(len + 1);
			else
				desc = malloc(len);
			if(desc == NULL)
			{
				err("desc no mem %d", len);
				free(title); title = NULL;
				free(subtitle); subtitle = NULL;
				free(desc); desc = NULL;
				break;
			}
			ret = fread(desc, len, 1, fd);
			if(desccomplen == 0)
				desc[len] = '\0';
		}
		else if(len != 0)
		{
			err("epgfile defekt desc len=%d", len);
			free(title); title = NULL;
			free(subtitle); subtitle = NULL;
			free(desc); desc = NULL;
			break;
		}

		if(oldchnode != NULL && oldchnode->serviceid == serviceid && oldchnode->transponderid == transponderid)
			chnode = oldchnode;
		else
		{
			chnode = getchannel(serviceid, transponderid);
			if(chnode == NULL)
			{
				free(title); title = NULL;
				free(subtitle); subtitle = NULL;
				free(desc); desc = NULL;
				continue;
			}
			oldchnode = chnode;
			epgnode = NULL;
		}

#ifndef SIMULATE
		if(endtime < time(NULL) || starttime > time(NULL) + epgmaxsec)
		{
			free(title); title = NULL;
			free(subtitle); subtitle = NULL;
			free(desc); desc = NULL;
			continue;
		}
#endif

		epgnode = addepg(chnode, eventid, version, starttime, endtime, epgnode, 1);
		if(epgnode == NULL)
		{
			free(title); title = NULL;
			free(subtitle); subtitle = NULL;
			free(desc); desc = NULL;
			continue;
		}
		epgnode->parentalrating = parentalrating;
		epgnode->desclen = desclen;
		epgnode->desccomplen = desccomplen;
		epgnode->title = title;
		epgnode->subtitle = subtitle;
		epgnode->desc = desc;
	}

	fclose(fd);

	if(getconfigint("epg_del", NULL) == 1)
		unlink(filename);
		
	m_unlock(&status.epgmutex, 4);

	//epg.dat seems defekt IO Error
	if(count > 50) unlink(filename);

	return 0;
}

//Read EIT segments from DVB-demuxer
//flag 0 = from epg thread
//flag 1 = from epg scan
int readeit(struct stimerthread* self, struct channel* chnode, struct dvbdev* fenode, int flag)
{
	int readlen = 0, pos = 0, len = 0;
	unsigned char *buf = NULL, *head = NULL;
	struct dvbdev* dmxnode;
	struct eit* eit = NULL;
	unsigned long seen[16] = {0};
	unsigned long firstseen[16] = {0};
	int roundend = 0, round = 0x4e, i = 0;
	time_t akttime, roundstart;

	if(chnode == NULL) chnode = status.aktservice->channel;

	buf = malloc(MINMALLOC * 4);
	if(buf == NULL)
	{
		err("no memory");
		return 1;
	}
	head = buf;

	if(fenode == NULL) fenode = status.aktservice->fedev;
	if(fenode == NULL)
	{
		debug(400, "no frontend dev in service");
		free(buf);
		return 1;
	}

	dmxnode = dmxopen(fenode);
	if(dmxnode == NULL)
	{
		err("open demux dev");
		free(buf);
		return 1;
	}

	dmxsetbuffersize(dmxnode, getconfigint("dmxepgbuffersize", NULL));
	dmxsetsource(dmxnode, fenode->fedmxsource);

#ifdef SIMULATE
	int fd = open("simulate/epg.ts", O_RDONLY);
	if(fd == -1)
	{
		perr("open simulate/epg.ts");
		return 1;
	}
#endif

	for(i = 2; i < 16; i++) firstseen[i] = 1;
	roundstart = time(NULL);
	dmxsetfilter(dmxnode, 0x12, 0, 8);
	while(self->aktion != STOP && self->aktion != PAUSE)
	{
		akttime = time(NULL);
		roundend = 1;

		//check end of epg infos
		for(i = 0; i < 16; i++)
		{
			if(roundstart + 5 < akttime && firstseen[i] == 0)
				firstseen[i] = 1;
			if(roundstart + 30 < akttime && firstseen[i] != 1)
				firstseen[i] = 1;
			if(firstseen[i] != 1)
			{
				roundend = 0;
				break;
			}
		}
		
		if(roundend == 1)
		{
			for(i = 0; i < 16; i++)
			{
				seen[i] = 0;
				firstseen[i] = 0;
			}

			roundend = 0;
			pos = 0;
			head = buf;

			if(round == 0x60)
			{
				dmxstop(dmxnode);
				debug(400, "epg no more new data, wait for next run");

				if(chnode != NULL && chnode->transponder != NULL)
					chnode->transponder->lastepg = time(NULL) + 7700;

				goto end;
			}
			if(round == 0x4e)
			{
				dmxsetfilter(dmxnode, 0x12, 0, 6);
				round = 0x50;
			}
			else if(round == 0x50)
			{
				dmxsetfilter(dmxnode, 0x12, 0, 7);
				round = 0x60;
			}
			else if(round == 0x60)
			{
				dmxsetfilter(dmxnode, 0x12, 0, 8);
				for(i = 2; i < 16; i++) firstseen[i] = 1;
				round = 0x4e;
			}
			roundstart = time(NULL);
			continue;
		}
	
		if(pos < 3) goto read_more;

		eit = (struct eit*)head;

		len = 3 + GETEITSECLEN(eit);
		if(pos < len) goto read_more;
		
		if(eit->table_id - round < 0 || eit->table_id - round > 15 || dvbcrc32((uint8_t *)head, len) != 0)
		{
			err("epg crc or table id not ok");
			dmxstop(dmxnode);
			dmxstart(dmxnode);
			pos = 0;
			head = buf;
			goto read_more;
		}

		seen[eit->table_id - round] = (eit->table_id << 24) | (HILO(eit->service_id) << 8) | eit->section_number;

		if(firstseen[eit->table_id - round] == 0)
			firstseen[eit->table_id - round] = seen[eit->table_id - round];
		else if(firstseen[eit->table_id - round] == seen[eit->table_id - round])
			firstseen[eit->table_id - round] = 1;

		parseeit(chnode, head, len, flag);

		//remove packet
		pos -= len;
		head += len;
		continue;
read_more:
		//clear buffer
		if(pos == MINMALLOC * 4)
		{
			pos = 0;
			head = buf;
			dmxstop(dmxnode);
			dmxstart(dmxnode);
		}

		//move remaining data to front of buffer
		if (pos > 0) memmove(buf, head, pos);

		//fill with fresh data
#ifdef SIMULATE
		readlen = TEMP_FAILURE_RETRY(read(fd, buf + pos, (MINMALLOC * 4) - pos));
		usleep(200000);
#else
		readlen = dvbread(dmxnode, buf, pos, (MINMALLOC * 4) - pos, 500000);
		usleep(1000);
#endif
		if(readlen < 0) readlen = 0;
		head = buf;
		pos += readlen;
	}

end:
#ifdef SIMULATE
	close(fd);
#endif
	dmxclose(dmxnode, -1);
	free(buf);
	return 0;
}

void epgthreadfunc(struct stimerthread* self)
{
	char* tmpstr = NULL;
	time_t akttime = 0;
	struct channel* chnode = NULL;
	int skip = 0, count = 0;

	if(self == NULL) return;

	//wait for right time
	while(self->aktion != STOP && time(NULL) < 1072224000) // 01.01.2004
		usleep(1 * 1000000);

	debug(400, "start epg thread on aktiv channel");

	if(self->notfirst == 0)
	{
		debug(400, "read epg data from file");
		status.deloldepg = time(NULL);
		status.writeperiodicepg = time(NULL);
		tmpstr = createpath(getconfig("epg_path", NULL), "epg.dat");
		readepg(tmpstr);
		free(tmpstr); tmpstr = NULL;
	}

	if(status.epgscanlistthread == NULL)
	{
		akttime = time(NULL);
		chnode = status.aktservice->channel;

		//del old epg date
		if(status.deloldepg + (5 * 60 * 60) < time(NULL)) // 5 stunde
		{
			status.deloldepg = time(NULL);
			deloldepg();
		}

		//write epg periodic to medium
		if(status.writeperiodicepg + (2 * 60 * 60) < time(NULL)) // 2 stunde
		{
			status.writeperiodicepg = time(NULL);
			if(getconfigint("epgsave", NULL) == 0) //only write periodic if epgsave = allways
				writeallconfig(2);
		}

		if(chnode != NULL && chnode->transponder != NULL && chnode->transponder->lastepg > akttime && chnode->epg != NULL)
		{
			debug(400, "skip epg read (%ld < %ld)", akttime, chnode->transponder->lastepg);
			skip = (chnode->transponder->lastepg - akttime) * 2;
		}

		if(skip > 0)
		{
			while(self->aktion != STOP && self->aktion != PAUSE)
			{
				if(count > skip) break;
				count++;
				usleep(500000);
			}
			if(self->aktion == STOP || self->aktion == PAUSE) goto end;
			count = 0;
		}

		if(self->aktion != STOP && self->aktion != PAUSE)
			readeit(self, NULL, NULL, 0);
		if(self->aktion != STOP && self->aktion != PAUSE)
		{
			if(getconfigint("mhwepg", NULL) == 2)
				readmhw(self, NULL, NULL, 0);
		}
		if(self->aktion != STOP && self->aktion != PAUSE)
		{
			if(getconfigint("mhw2epg", NULL) == 2)
				readmhw2(self, NULL, NULL, 0);
		}
		if(self->aktion != STOP && self->aktion != PAUSE)
		{
			if(getconfigint("opentv", NULL) == 2)
				readopentv(self, NULL, NULL, 0);
		}

		//wait for next run
		while(self->aktion != STOP && self->aktion != PAUSE)
		{
			if(count > 14400) break; //2h
			count++;
			usleep(500000);
		}
	}

end:
	debug(400, "end epg thread on aktiv channel");
}

#endif
