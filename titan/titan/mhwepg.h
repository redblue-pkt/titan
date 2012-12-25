#ifndef MHWEPG_H
#define MHWEPG_H

//channel C+ DEMANDE for test on astra 19.2 (mhw)
//channel C+ PORTADA for test on astra 19.2 (mhw2)

//global function

struct freesat
{
	unsigned int value;
	short bits;
	char next;
};

struct skybox
{
	char *value;
	struct skybox *p0;
	struct skybox *p1;
};
typedef struct skybox skyboxnode;

static struct freesat *freesattable[2][128];
static int freesattablesize[2][128];
static skyboxnode* skyboxtable[2];

unsigned char resolvechar(char *str)
{
	int val = 0;

	if(str == NULL) return '\0';

	if(strcmp(str, "ESCAPE") == 0)
		return '\1';
	else if(strcmp(str, "STOP") == 0)
		return '\0';
	else if(strcmp(str, "START") == 0)
		return '\0';
	else if(sscanf(str, "0x%02x", &val) == 1)
		return val;

  return str[0];
}

unsigned long decodebinary(char *binary)
{
	unsigned long mask = 0x80000000;
	unsigned long maskval = 0;
	unsigned long val = 0;
	size_t i = 0;

	if(binary == NULL) return 0;

	for(i = 0; i < strlen(binary); i++)
	{
		if(binary[i] == '1')
			val |= mask;
		maskval |= mask;
		mask >>= 1;
	}
	return val;
}

void freefreesat()
{
	int i = 0;

	for(i = 0; i < 128; i++)
	{
		free(freesattable[0][i]);
		freesattable[0][i] = NULL;
	}

	for(i = 0; i < 128; i++)
	{
		free(freesattable[1][i]);
		freesattable[1][i] = NULL;
	}
}

void freeskyboxnode(skyboxnode* node)
{
	if(node != NULL)
	{
		free(node->value); node->value = NULL;
		if(node->p0 != NULL)
			freeskyboxnode(node->p0);
		free(node->p0); node->p0 = NULL;
		if(node->p1 != NULL)
			freeskyboxnode(node->p1);
		free(node->p1); node->p1 = NULL;
	}
}

void freeskybox()
{
	freeskyboxnode(skyboxtable[0]);
	freeskyboxnode(skyboxtable[1]);

	free(skyboxtable[0]);
	skyboxtable[0] = NULL;
	free(skyboxtable[1]);
	skyboxtable[1] = NULL;
}

int loadfreesat(int tableid, char *filename)
{
	char buf[1024];
	char *from = NULL, *to = NULL, *binary = NULL;
	FILE *fp = NULL;

	tableid--;
	if((fp = fopen(filename, "r")) != NULL)
	{
		debug(400, "loading table %d filename <%s>", tableid + 1, filename);

		while(fgets(buf, sizeof(buf), fp) != NULL)
		{
			from = binary = to = NULL;
			int elems = sscanf (buf, "%a[^:]:%a[^:]:%a[^:]:", &from, &binary, &to);
			if(elems == 3)
			{
				int len = strlen(binary);
				int fromchar = resolvechar(from);
				char tochar = resolvechar(to);
				unsigned long bin = decodebinary(binary);
				int i = freesattablesize[tableid][fromchar]++;

				freesattable[tableid][fromchar] = (struct freesat*)realloc(freesattable[tableid][fromchar], (i + 1) * sizeof (freesattable[tableid][fromchar][0]));
				freesattable[tableid][fromchar][i].value = bin;
				freesattable[tableid][fromchar][i].next = tochar;
				freesattable[tableid][fromchar][i].bits = len;

				free (from); from = NULL;
				free (to); to = NULL;
				free (binary); binary = NULL;
			}
		}
		fclose(fp);
	}
	else
	{
    perr("cannot load <%s> for table %d", filename, tableid + 1);
		return 1;
	}
	return 0;
}

//flag 0: SKY IT
//flag 1: SKY UK
int loadskybox(char *filename, int flag)
{
	FILE *fd = NULL;
	char *line = NULL;
	char buf[256];
	skyboxnode *nh = NULL;

	fd = fopen(filename, "r");
	if(fd == NULL)
	{
		perr("opening file %s", filename);
		return 1;
	}
	else
	{
		int i = 0;
		int len = 0;
		char string1[256];
		char string2[256];

		if(!skyboxtable[flag])
		{
			skyboxtable[flag] = (skyboxnode*)calloc(1, sizeof(skyboxnode));
			if(!skyboxtable[flag])
			{
				err("no mem");
				return 1;
			}
		}

		while((line = fgets(buf, sizeof(buf), fd)) != NULL)
		{
			if(line != NULL && strlen(line) > 0)
			{
				memset(string1, 0, sizeof(string1));
				memset(string2, 0, sizeof(string2));
				if(sscanf(line, "%c=%[^\n]\n", string1, string2) == 2 || (sscanf(line, "%[^=]=%[^\n]\n", string1, string2) == 2))
				{
 					nh = skyboxtable[flag];
					len = strlen(string2);
					for(i = 0; i < len; i++)
					{
						switch(string2[i])
						{
							case '0':
								if(nh->p0 == NULL)
								{
									nh->p0 = (skyboxnode*)calloc(1, sizeof(skyboxnode));
									if(nh->p0 != NULL)
									{
										nh = nh->p0;
										nh->value = NULL;
										nh->p0 = NULL;
										nh->p1 = NULL;
										if((len - 1) == i)
											asprintf (&nh->value, "%s", string1);
									}
								}
								else
								{
									nh = nh->p0;
									if(nh->value != NULL || (len - 1) == i)
									{
                  	debug(400 ,"huffman prefix code already exists for \"%s\"=%s with '%s'", string1, string2, nh->value);
									}
								}
								break;
							case '1':
								if(nh->p1 == NULL)
								{
									nh->p1 = (skyboxnode*)calloc(1, sizeof(skyboxnode));
									if(nh->p1 != NULL)
									{
										nh = nh->p1;
										nh->value = NULL;
										nh->p0 = NULL;
										nh->p1 = NULL;
										if((len - 1) == i)
											asprintf(&nh->value, "%s", string1);
									}
								}
								else
								{
									nh = nh->p1;
									if(nh->value != NULL || (len - 1) == i)
									{
										debug(400, "huffman prefix code already exists for \"%s\"=%s with '%s'", string1, string2, nh->value);
									}
								}
								break;
							default:
								break;
						}
					}
				}
			}
		}
		fclose(fd);
	}

	//check tree huffman nodes
	fd = fopen(filename, "r");
	if(fd)
	{
		int i = 0;
		int len = 0;
		char string1[256];
		char string2[256];

		while((line = fgets(buf, sizeof(buf), fd)) != NULL)
		{
			if(line != NULL && strlen(line) > 0)
			{
				memset(string1, 0, sizeof(string1));
				memset(string2, 0, sizeof(string2));
				if(sscanf(line, "%c=%[^\n]\n", string1, string2) == 2 || (sscanf(line, "%[^=]=%[^\n]\n", string1, string2) == 2))
				{
					nh = skyboxtable[flag];
					len = strlen(string2);
					for(i = 0; i < len; i++)
					{
						switch(string2[i])
						{
							case '0':
								if(nh->p0 != NULL)
									nh = nh->p0;
								break;
							case '1':
								if(nh->p1 != NULL)
									nh = nh->p1;
								break;
							default:
								break;
						}
					}
					if(nh->value != NULL)
					{
						if(memcmp(nh->value, string1, strlen(nh->value)) != 0)
						{
							debug(400, "huffman prefix value '%s' not equal to '%s'", nh->value, string1);
						}
					}
					else
					{
						debug(400, "huffman prefix value is not exists for \"%s\"=%s", string1, string2);
					}
				}
			}
		}
		fclose(fd);
	}

	return 0;
}

char *freesathuffmandecode(unsigned char *src, size_t size)
{
	int tableid = 0;

	if(src == NULL) return NULL;

	if(src[0] == 0x1f && (src[1] == 1 || src[1] == 2))
	{
		int uncompressedlen = 30, p = 0;
		char *uncompressed = NULL;
		unsigned value = 0, byte = 2, bit = 0;
		unsigned char lastch = '\0';

		uncompressed = calloc(1, uncompressedlen + 1);
		if(uncompressed == NULL)
		{
			err("no mem");
			return NULL;
		}

		tableid = src[1] - 1;
		while(byte < 6 && byte < size)
		{
			value |= src[byte] << ((5 - byte) * 8);
			byte++;
		}

		do
		{
			int found = 0;
			unsigned bitshift = 0;
			if(lastch == '\1')
			{
				char nextchar = (value >> 24) & 0xff;
				found = 1;
				//Encoded in the next 8 bits.
				//Terminated by the first ASCII character.
				bitshift = 8;
				if((nextchar & 0x80) == 0)
					lastch = nextchar;
				if(p >= uncompressedlen)
				{
					uncompressedlen += 10;
					uncompressed = (char *)realloc(uncompressed, uncompressedlen + 1);
				}
				uncompressed[p++] = nextchar;
				uncompressed[p] = '\0';
			}
			else
			{
				int j = 0;
				for(j = 0; j < freesattablesize[tableid][lastch]; j++)
				{
					unsigned mask = 0, maskbit = 0x80000000;
					short kk = 0;
					for(kk = 0; kk < freesattable[tableid][lastch][j].bits; kk++)
					{
						mask |= maskbit;
						maskbit >>= 1;
					}
					if((value & mask) == freesattable[tableid][lastch][j].value)
					{
 						char nextchar = freesattable[tableid][lastch][j].next;
						bitshift = freesattable[tableid][lastch][j].bits;
						if(nextchar != '\0' && nextchar != '\1')
						{
							if(p >= uncompressedlen)
							{
								uncompressedlen += 10;
								uncompressed = (char *)realloc(uncompressed, uncompressedlen + 1);
							}
							uncompressed[p++] = nextchar;
							uncompressed[p] = 0;
						}
						found = 1;
						lastch = nextchar;
						break;
					}
				}
			}
			if(found)
			{
				//Shift up by the number of bits.
				unsigned b = 0;
				for(b = 0; b < bitshift; b++)
				{
					value = (value << 1) & 0xfffffffe;
					if(byte < size)
						value |= (src[byte] >> (7 - bit)) & 1;
					if(bit == 7)
					{
						bit = 0;
						byte++;
					}
					else
						bit++;
				}
			}
			else
			{
				debug(400, "missing table %d entry: <%s>", tableid + 1, uncompressed);
				//Entry missing in table.
				return uncompressed;
			}
		} while(lastch != '\0' && value != 0);

		return uncompressed;
	}
	return NULL;
}

//flag 0: SKY IT
//flag 1: SKY UK
int skyhuffmandecode(unsigned char *buf, int len, unsigned char *decodetxt, int flag)
{
	skyboxnode *nh, h = (flag == 0) ? *skyboxtable[0] : *skyboxtable[1];
	int i = 0, p = 0, q = 0;
	int codeerr = 0, isfound = 0;
	unsigned char byte = 0, lastbyte = 0;
	unsigned char mask = 0, lastmask = 0;

	if(buf == NULL || decodetxt == NULL) return -1;

	nh = &h;
	decodetxt[0] = '\0';

	for(i = 0; i < len; i++)
	{
		byte = buf[i];
		mask = 0x80;
		if(i == 0)
		{
			mask = 0x20;
			lastbyte = i;
			lastmask = mask;
		}
loop1:
		if(isfound)
		{
			lastbyte = i;
			lastmask = mask;
			isfound = 0;
		}
		if((byte & mask) == 0)
		{
			if(codeerr)
			{
				q++;
				goto loop2;
			}
			if(nh->p0 != NULL)
			{
				nh = nh->p0;
				if(nh->value != NULL)
				{
					memcpy(&decodetxt[p], nh->value, strlen(nh->value));
					p += strlen(nh->value);
					nh = &h;
					isfound = 1;
				}
			}
			else
			{
				memcpy(&decodetxt[p], "<...?...>", 9);
				p += 9;
				i = lastbyte;
				byte = buf[lastbyte];
				mask = lastmask;
				codeerr = 1;
				goto loop1;
			}
		}
		else
		{
			if(codeerr)
			{
				q++;
				goto loop2;
			}
			if(nh->p1 != NULL)
			{
				nh = nh->p1;
				if(nh->value != NULL)
				{
					memcpy(&decodetxt[p], nh->value, strlen(nh->value));
					p += strlen(nh->value);
					nh = &h;
					isfound = 1;
				}
			}
			else
			{
				memcpy(&decodetxt[p], "<...?...>", 9);
				p += 9;
				i = lastbyte;
				byte = buf[lastbyte];
				mask = lastmask;
				codeerr = 1;
				goto loop1;
			}
		}
loop2:
		mask = mask >> 1;
		if(mask > 0)
			goto loop1;
	}

	decodetxt[p] = '\0';
	return p;
}

struct mhwcache* addmhwcache(int id, struct epg* epgnode, struct mhwcache* last)
{
	//debug(1000, "in");
	struct mhwcache *newnode = NULL, *prev = NULL, *node = mhwcache;

	newnode = (struct mhwcache*)calloc(1, sizeof(struct mhwcache));
	if(newnode == NULL)
	{
		err("no memory");
		return NULL;
	}

	newnode->id = id;
	newnode->epgnode = epgnode;

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
		mhwcache = newnode;
	else
		prev->next = newnode;
	newnode->next = node;

	//debug(1000, "out");
	return newnode;
}

void delmhwcache(struct mhwcache* cache)
{
	//debug(1000, "in");
	struct mhwcache *node = mhwcache, *prev = mhwcache;

	while(node != NULL)
	{
		if(node == cache)
		{
			if(node == mhwcache)
				mhwcache = node->next;
			else
				prev->next = node->next;

			free(node);
			node = NULL;
			break;
		}

		prev = node;
		node = node->next;
	}
	//debug(1000, "out");
}

void freemhwcache()
{
	//debug(1000, "in");
	struct mhwcache *node = mhwcache, *prev = mhwcache;

	while(node != NULL)
	{
		prev = node;
		node = node->next;
		if(prev != NULL)
			delmhwcache(prev);
	}
	//debug(1000, "out");
}

struct mhwcache* getmhwcache(int id)
{
	//debug(1000, "in");
	struct mhwcache *node = mhwcache;

	while(node != NULL)
	{
		if(node->id == id)
		{
			//debug(1000, "out");
			return node;
		}

		node = node->next;
	}
	debug(100, "mhwcache not found (mhwcache=%d)", id);
	return NULL;
}

unsigned long getquad(void *ptr)
{
	unsigned char *data = ptr;
	return (data[0] << 24) | (data[1] << 16) | (data[2] << 8) | data[3];
}

unsigned short getdoub(void *ptr)
{
	unsigned char *data = ptr;
	return (data[0] << 8) | data[1];
}

time_t localtime2utc(time_t t)
{
	struct tm *temp;

	temp = gmtime(&t);
	temp->tm_isdst = -1;
	return mktime(temp);
}

time_t utc2localtime(time_t t)
{
	return 2 * t - localtime2utc(t);
}

int mhwtimeoffset(int *yesterday, time_t *yesterdayepoch)
{
	struct tm *curtime;
	time_t yesterdaytime = 0;

	// Get day of the week of yesterday (provider local time)
	yesterdaytime = utc2localtime(time(NULL) - 86400);
	curtime = gmtime(&yesterdaytime);
	*yesterday = curtime->tm_wday;

 	// Get epoch of yesterday 00:00 (provider local time)
	curtime->tm_hour = 0;
	curtime->tm_min = 0;
	curtime->tm_sec = 0;
	curtime->tm_isdst = -1;
	*yesterdayepoch = utc2localtime(mktime(curtime));

	return 0;
}

//mhw
struct mhwchannel* getmhwchannel(unsigned char* channelbuf, int channelcount, int id)
{
	struct mhwchannel* mhwchannel = NULL;
	int i = 0;

	if(channelbuf == NULL) return NULL;
	mhwchannel = (struct mhwchannel*)(channelbuf + 4);

	for(i = 0; i < channelcount; i++)
	{
		if(i == id - 1) return mhwchannel;
		mhwchannel++;
	}

	return NULL;
}

int readmhwchannel(struct stimerthread* self, struct dvbdev* fenode, unsigned char* channelbuf)
{
	int readlen = 0;
	struct dvbdev* dmxnode;

	if(channelbuf == NULL) return -1;

	if(fenode == NULL) fenode = status.aktservice->fedev;
	if(fenode == NULL)
	{
		debug(400, "no frontend dev in service");
		return -1;
	}

	dmxnode = dmxopen(fenode);
	if(dmxnode == NULL)
	{
		err("open demux dev");
		return -1;
	}

	dmxsetbuffersize(dmxnode, getconfigint("dmxepgbuffersize", NULL));
	dmxsetsource(dmxnode, fenode->fedmxsource);

	dmxsetfilter(dmxnode, 0xD3, 0, 16);

	readlen = dvbread(dmxnode, channelbuf, 0, MINMALLOC, 2000000);
	if(readlen <= 0)
	{
		dmxclose(dmxnode, -1);
		return -1;
	}

	dmxclose(dmxnode, -1);
	return readlen;
}

int readmhwtitle(struct stimerthread* self, struct dvbdev* fenode, struct channel* chnode, unsigned char* channelbuf, int channelcount)
{
	int readlen = 0, first = 1;
	unsigned char *buf = NULL, *firstbuf = NULL;
	struct dvbdev* dmxnode;
	struct mhwtitle* mhwtitle = NULL;
	struct mhwchannel* mhwchannel = NULL;
	uint64_t transponderid = 0;
	int serviceid = 0, eventid = 0, day = 0, hours = 0, yesterday = 0;
	struct channel* tmpchnode = NULL;
	struct epg* epgnode = NULL;
	time_t dvbtime = 0, starttime = 0, endtime = 0, akttime = 0, yesterdayepoch = 0;
	time_t epgmaxsec = status.epgdays * 24 * 60 * 60;
	struct mhwcache* cache = NULL;

	if(chnode == NULL) chnode = status.aktservice->channel;
	if(chnode == NULL || (chnode == status.aktservice->channel && status.aktservice->type != CHANNEL))
		return 1;

	mhwtimeoffset(&yesterday, &yesterdayepoch);

	buf = calloc(1, MHWTITLELEN);
	if(buf == NULL)
	{
		err("no memory");
		return 1;
	}
	mhwtitle = (struct mhwtitle*)buf;

	firstbuf = calloc(1, MHWTITLELEN);
	if(firstbuf == NULL)
	{
		err("no memory");
		free(buf);
		return 1;
	}

	if(fenode == NULL) fenode = status.aktservice->fedev;
	if(fenode == NULL)
	{
		debug(400, "no frontend dev in service");
		free(buf);
		free(firstbuf);
		return 1;
	}

	dmxnode = dmxopen(fenode);
	if(dmxnode == NULL)
	{
		err("open demux dev");
		free(buf);
		free(firstbuf);
		return 1;
	}

	dmxsetbuffersize(dmxnode, getconfigint("dmxepgbuffersize", NULL));
	dmxsetsource(dmxnode, fenode->fedmxsource);

	dmxsetfilter(dmxnode, 0xD2, 0, 15);
	akttime = time(NULL);

	while(self->aktion != STOP && self->aktion != PAUSE)
	{
		readlen = dvbread(dmxnode, buf, 0, MHWTITLELEN, 2000000);
		usleep(1000);
		if(readlen != MHWTITLELEN)
		{
			perr("while reading titles");
			dmxclose(dmxnode, -1);
			free(buf);
			free(firstbuf);
			return 1;
		}
		if(first == 1)
		{
			first = 0;
			memcpy(firstbuf, buf, MHWTITLELEN);
		}
		else
		{
			if(memcmp(firstbuf, buf, MHWTITLELEN) == 0)
			{
				debug(400, "mhwepg no more new data, wait for next run");

				if(chnode != NULL && chnode->transponder != NULL)
					chnode->transponder->lastepg = time(NULL) + 7700;
				break;
			}

			//stop epgscan after 2 min
			if(akttime + 120 < time(NULL))
			{
				debug(400, "mhwepg timeout");
				break;
			}
		}

		mhwchannel = getmhwchannel(channelbuf, channelcount, mhwtitle->channel_id);
		if(mhwchannel != NULL)
		{
			transponderid = ((HILO(mhwchannel->network_id) << 16) | HILO(mhwchannel->transponder_id)) & 0xffffffff;
 			if(chnode->transponder != NULL)
			{
				if(chnode->transponder->fetype == FE_QAM)
					transponderid = transponderid | ((uint64_t)1 << 32);
				else if(chnode->transponder->fetype == FE_OFDM)
					transponderid = transponderid | ((uint64_t)2 << 32);
			}

			serviceid = HILO(mhwchannel->channel_id);
			tmpchnode = getchannel(serviceid, transponderid);
			if(tmpchnode == NULL)
			{
				debug(1000, "NULL detect");
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

			eventid = (mhwtitle->channel_id << 16) | (mhwtitle->day << 13) | (mhwtitle->hours << 8) | mhwtitle->minutes;
			epgnode = getepg(tmpchnode, eventid, 1);

			//get start/endtime
			day = mhwtitle->day;
			hours = mhwtitle->hours;
			if(day == 7) day = 0;
			if(hours > 15) hours = hours - 4;
			else if(hours > 7) hours = hours - 2;
			else day = (day == 6) ? 0 : day + 1;

			dvbtime = (day - yesterday) * 86400 + hours * 3600 + mhwtitle->minutes * 60;
			if(dvbtime < 6 * 3600) dvbtime += 7 * 86400;	/* Next week */
			dvbtime += yesterdayepoch;
			starttime = localtime2utc(dvbtime);
			endtime = starttime + (HILO(mhwtitle->duration) * 60);

#ifndef SIMULATE
			if(endtime < time(NULL) || starttime > time(NULL) + epgmaxsec)
			{
				m_unlock(&status.epgmutex, 4);
				continue;
			}
#endif

			if(epgnode == NULL)
				epgnode = addepg(tmpchnode, eventid, 0, starttime, endtime, NULL, 1);
			else
				updateepg(tmpchnode, epgnode, eventid, 0, starttime, endtime, 1);

			if(epgnode == NULL)
			{
				debug(1000, "NULL detect");
				m_unlock(&status.epgmutex, 4);
				continue;
			}

			epgnode->title = ostrcat((char*)mhwtitle->title, NULL, 0, 0);
			epgnode->title = strstrip(epgnode->title);
			cache = addmhwcache(HILO32(mhwtitle->program_id), epgnode, cache);

			m_unlock(&status.epgmutex, 4);
		}
	}

	dmxclose(dmxnode, -1);
	free(buf);
	free(firstbuf);
	return 0;
}

int readmhwsummary(struct stimerthread* self, struct dvbdev* fenode)
{
	int readlen = 0, first = 1, len = 0;
	unsigned char *buf = NULL, *firstbuf = NULL;
	struct dvbdev* dmxnode;
	time_t akttime = 0;
	char* tmpstr = NULL;
	struct mhwsummary* mhwsummary = NULL;
	char* zbuf = NULL;
	int zlen = 0, ret = 0;
	struct mhwcache* cache = NULL;

	buf = calloc(1, MINMALLOC);
	if(buf == NULL)
	{
		err("no memory");
		return 1;
	}
	mhwsummary = (struct mhwsummary*)buf;

	firstbuf = calloc(1, MINMALLOC);
	if(firstbuf == NULL)
	{
		err("no memory");
		free(buf);
		return 1;
	}

	if(fenode == NULL) fenode = status.aktservice->fedev;
	if(fenode == NULL)
	{
		debug(400, "no frontend dev in service");
		free(buf);
		free(firstbuf);
		return 1;
	}

	dmxnode = dmxopen(fenode);
	if(dmxnode == NULL)
	{
		err("open demux dev");
		free(buf);
		free(firstbuf);
		return 1;
	}

	dmxsetbuffersize(dmxnode, getconfigint("dmxepgbuffersize", NULL));
	dmxsetsource(dmxnode, fenode->fedmxsource);

	dmxsetfilter(dmxnode, 0xD3, 0, 17);
	akttime = time(NULL);

	while(self->aktion != STOP && self->aktion != PAUSE)
	{
		readlen = dvbread(dmxnode, buf, 0, 3, 2000000);
		if(readlen != 3)
		{
			dmxclose(dmxnode, -1);
			free(buf);
			free(firstbuf);
			return 1;
		}
		readlen = 0;
		len = buf[2] | ((buf[1] & 0x0f) << 8);
		if(len + 3 <= MINMALLOC)
			readlen = dvbread(dmxnode, buf, 3, len, 2000000);
		if(readlen <= 0)
		{
			dmxclose(dmxnode, -1);
			free(buf);
			free(firstbuf);
			return 1;
		}

		//stop epgscan after 2 min
		if(akttime + 120 < time(NULL))
		{
			debug(400, "mhwepg timeout");
			break;
		}

		// Invalid Data
		if(readlen < 12 || buf[7] != 0xFF || buf[8] != 0xFF || buf[9] !=0xFF || buf[10] >= 10)
			continue;

		if(first == 1)
		{
			first = 0;
			memcpy(firstbuf, buf, readlen + 3);
		}
		else
		{
			if(memcmp(firstbuf, buf, readlen + 3) == 0)
			{
				debug(400, "mhwsummary no more new data, wait for next run");
				break;
			}
		}

		buf[readlen + 3] = '\0'; //String terminator

		cache = getmhwcache(HILO32(mhwsummary->program_id));
		if(cache != NULL && cache->epgnode != NULL)
		{

			//Index of summary text beginning
 			tmpstr = (char*)(buf + MHWSUMMARYLEN + mhwsummary->nb_replays * 7);
			tmpstr = stringreplacechar(tmpstr, '\n', ' ');

			//compress long desc
			if(tmpstr != NULL)
			{
				ret = ozip(tmpstr, strlen(tmpstr) + 1, &zbuf, &zlen, 1);
				if(ret == 0)
				{
					free(cache->epgnode->desc); cache->epgnode->desc = NULL;
					cache->epgnode->desc = zbuf;
					cache->epgnode->desccomplen = zlen;
				}
			}
			tmpstr = NULL;
		}
	}

	dmxclose(dmxnode, -1);
	free(buf);
	free(firstbuf);
	return 0;
}

int readmhw(struct stimerthread* self, struct dvbdev* fenode)
{
	int ret = 0, channelcount = 0;
	unsigned char* channelbuf = NULL;

	if(fenode == NULL) fenode = status.aktservice->fedev;
	if(fenode == NULL)
	{
		debug(400, "no frontend dev in service");
		return 1;
	}

	channelbuf = calloc(1, MINMALLOC);
	if(channelbuf == NULL)
	{
		err("no memory");
		return 1;
	}

	channelcount = readmhwchannel(self, fenode, channelbuf);
	if(channelcount <= 0)
	{
		debug(400, "mhwepg no channels found");
		free(channelbuf); channelbuf = NULL;
		return 1;
	}

	ret = readmhwtitle(self, fenode, NULL, channelbuf, channelcount);
	if(ret != 0)
	{
		debug(400, "mhwepg no titles found");
		free(channelbuf); channelbuf = NULL;
		freemhwcache();
		return 1;
	}

	ret = readmhwsummary(self, fenode);
	if(ret != 0)
	{
		debug(400, "mhwepg no summary found");
		free(channelbuf); channelbuf = NULL;
		freemhwcache();
		return 1;
	}

	free(channelbuf); channelbuf = NULL;
	freemhwcache();
	return 0;
}

//mhw2
struct mhw2channel* getmhw2channel(unsigned char* channelbuf, int id)
{
	struct mhw2channel* mhw2channel = NULL;
	int channelcount = 0;
	int i = 0;

	if(channelbuf == NULL) return NULL;
	mhw2channel = (struct mhw2channel*)(channelbuf + 121);

	channelcount = channelbuf[120];

	for(i = 0; i < channelcount; i++)
	{
		if(i == id - 1) return mhw2channel;
		mhw2channel++;
	}

 	return NULL;
}

int readmhw2channel(struct stimerthread* self, struct dvbdev* fenode, unsigned char* channelbuf)
{
	int readlen = 0, count = 0;
	unsigned short len = 0;
	struct dvbdev* dmxnode;

	if(channelbuf == NULL) return 1;

	if(fenode == NULL) fenode = status.aktservice->fedev;
	if(fenode == NULL)
	{
		debug(400, "no frontend dev in service");
		return 1;
	}

	dmxnode = dmxopen(fenode);
	if(dmxnode == NULL)
	{
		err("open demux dev");
		return 1;
	}

	dmxsetbuffersize(dmxnode, getconfigint("dmxepgbuffersize", NULL));
	dmxsetsource(dmxnode, fenode->fedmxsource);

	dmxsetfilter(dmxnode, 0x231, 0, 19);

start:
	readlen = dvbread(dmxnode, channelbuf, 0, 3, 2000000);
	if(readlen != 3)
	{
		dmxclose(dmxnode, -1);
		return 1;
	}
	readlen = 0;
	len = channelbuf[2] | ((channelbuf[1] & 0x0f) << 8);
	if(len + 3 <= MINMALLOC)
		readlen = dvbread(dmxnode, channelbuf, 3, len, 2000000);
	if(readlen <= 0)
	{
		dmxclose(dmxnode, -1);
		return 1;
	}

	count++;
	if(channelbuf[3] != 0 && count < 6) goto start;

	dmxclose(dmxnode, -1);

	if(count >= 6)
		return 1;
	else
		return 0;
}

int readmhw2title(struct stimerthread* self, struct dvbdev* fenode, struct channel* chnode, unsigned char* channelbuf)
{
	int readlen = 0, pos = 0, len = 0;
	unsigned char *buf = NULL;
	struct dvbdev* dmxnode;
	struct mhw2channel* mhw2channel = NULL;
	uint64_t transponderid = 0;
	int serviceid = 0, eventid = 0;
	struct channel* tmpchnode = NULL;
	unsigned long quad = 0, quad0 = 0;
	struct epg* epgnode = NULL;
	time_t dvbtime = 0, starttime = 0, endtime = 0, akttime = 0;
	time_t epgmaxsec = status.epgdays * 24 * 60 * 60;
	char tmpstr[65];
	struct mhwcache* cache = NULL;

	if(chnode == NULL) chnode = status.aktservice->channel;
	if(chnode == NULL || (chnode == status.aktservice->channel && status.aktservice->type != CHANNEL))
		return 1;

	buf = calloc(1, MINMALLOC);
	if(buf == NULL)
	{
		err("no memory");
		return 1;
	}

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

	dmxsetfilter(dmxnode, 0x234, 0, 18);
	akttime = time(NULL);

	while(self->aktion != STOP && self->aktion != PAUSE)
	{
		readlen = dvbread(dmxnode, buf, 0, 3, 2000000);
		if(readlen != 3)
		{
			dmxclose(dmxnode, -1);
			free(buf);
			return 1;
		}
		readlen = 0;
		len = buf[2] | ((buf[1] & 0x0f) << 8);
		if(len + 3 <= MINMALLOC)
			readlen = dvbread(dmxnode, buf, 3, len, 2000000);
		if(readlen <= 0)
		{
			dmxclose(dmxnode, -1);
			free(buf);
			return 1;
		}

		//check for end
		quad = getquad(buf + 3);
		if(quad0 == 0) quad0 = quad;
		else if(quad == quad0)
		{
			debug(400, "mhw2epg no more new data, wait for next run");

			if(chnode != NULL && chnode->transponder != NULL)
				chnode->transponder->lastepg = time(NULL) + 7700;
			break;
		}

		//stop epgscan after 2 min
		if(akttime + 120 < time(NULL))
		{
			debug(400, "mhw2epg timeout");
			break;
		}

		pos = 18;
		while(pos < len)
 		{
			int channelid = buf[pos];
			eventid = (buf[pos + 7] << 24) | (buf[pos + 8] << 16) | (buf[pos + 9] << 8) | buf[pos + 10];

			//get start/endtime
			pos += 11;
			dvbtime = (getdoub(buf + pos) - 40587) * 86400
			 + (((buf[pos + 2] & 0xf0) >> 4) * 10 + (buf[pos + 2] & 0x0f)) * 3600
			 + (((buf[pos + 3] & 0xf0) >> 4) * 10 + (buf[pos + 3] & 0x0f)) * 60;

			starttime = dvbtime;
			endtime = starttime + (getdoub(buf + pos + 5) >> 4) * 60;

			int lgr = buf[pos + 7] & 0x3f;
			if(lgr < 65)
			{
				memcpy(tmpstr, buf + pos + 8, lgr);
				tmpstr[lgr] = '\0';
			}
			else
				tmpstr[0] = '\0';
			pos += lgr + 8;
			int progid = getdoub(buf + pos + 1);

			mhw2channel = getmhw2channel(channelbuf, channelid);
			if(mhw2channel != NULL)
			{
				transponderid = ((HILO(mhw2channel->network_id) << 16) | HILO(mhw2channel->transponder_id)) & 0xffffffff;
				if(chnode->transponder != NULL)
				{
					if(chnode->transponder->fetype == FE_QAM)
						transponderid = transponderid | ((uint64_t)1 << 32);
					else if(chnode->transponder->fetype == FE_OFDM)
						transponderid = transponderid | ((uint64_t)2 << 32);
				}

				serviceid = HILO(mhw2channel->channel_id);
				tmpchnode = getchannel(serviceid, transponderid);
				if(tmpchnode == NULL)
				{
					debug(1000, "NULL detect");
					pos += 3;
					continue;
				}

				//look in epglist if channel exists
				//only if epglist is marked as whitelist
				if(status.epglistmode == 2 || status.epglistmode == 3)
				{
					if(getepgscanlist(serviceid, transponderid) == NULL)
					{
						pos += 3;
						continue;
					}
				}

				m_lock(&status.epgmutex, 4);

				epgnode = getepg(tmpchnode, eventid, 1);

#ifndef SIMULATE
				if(endtime < time(NULL) || starttime > time(NULL) + epgmaxsec)
				{
					m_unlock(&status.epgmutex, 4);
					pos += 3;
					continue;
				}
#endif

				if(epgnode == NULL)
					epgnode = addepg(tmpchnode, eventid, 0, starttime, endtime, NULL, 1);
				else
					updateepg(tmpchnode, epgnode, eventid, 0, starttime, endtime, 1);

				if(epgnode == NULL)
				{
					debug(1000, "NULL detect");
					m_unlock(&status.epgmutex, 4);
					pos += 3;
					continue;
				}

				epgnode->title = ostrcat(tmpstr, NULL, 0, 0);
				cache = addmhwcache(progid, epgnode, cache);

				m_unlock(&status.epgmutex, 4);

			}
			pos += 3;
		}
	}

	dmxclose(dmxnode, -1);
	free(buf);
	return 0;
}

int readmhw2summary(struct stimerthread* self, struct dvbdev* fenode)
{
	int readlen = 0, len = 0, first = 1;
	unsigned char *buf = NULL, *firstbuf = NULL;
	struct dvbdev* dmxnode;
	time_t akttime = 0;
	char* zbuf = NULL;
	int zlen = 0, ret = 0;
	char tmpstr[MINMALLOC];
	struct mhwcache* cache = NULL;

	buf = calloc(1, MINMALLOC);
	if(buf == NULL)
	{
		err("no memory");
		return 1;
	}

	firstbuf = calloc(1, MINMALLOC);
	if(firstbuf == NULL)
	{
		err("no memory");
		free(buf);
		return 1;
	}

	if(fenode == NULL) fenode = status.aktservice->fedev;
	if(fenode == NULL)
	{
		debug(400, "no frontend dev in service");
		free(buf);
		free(firstbuf);
		return 1;
	}

	dmxnode = dmxopen(fenode);
	if(dmxnode == NULL)
	{
		err("open demux dev");
		free(buf);
		free(firstbuf);
		return 1;
	}

	dmxsetbuffersize(dmxnode, getconfigint("dmxepgbuffersize", NULL));
	dmxsetsource(dmxnode, fenode->fedmxsource);

	dmxsetfilter(dmxnode, 0x236, 0, 20);
	akttime = time(NULL);

	while(self->aktion != STOP && self->aktion != PAUSE)
	{
		tmpstr[0] = '\0';

		readlen = dvbread(dmxnode, buf, 0, 3, 2000000);
		if(readlen != 3)
		{
			dmxclose(dmxnode, -1);
			free(buf);
			free(firstbuf);
			return 1;
		}
		readlen = 0;
		len = buf[2] | ((buf[1] & 0x0f) << 8);
		if(len + 3 <= MINMALLOC)
			readlen = dvbread(dmxnode, buf, 3, len, 2000000);
		if(readlen <= 0)
		{
			dmxclose(dmxnode, -1);
			free(buf);
			return 1;
		}

		//check for end
		if(first == 1)
		{
			first = 0;
			memcpy(firstbuf, buf, readlen + 3);
		}
		else
		{
			if(memcmp(firstbuf, buf, readlen + 3) == 0)
			{
				debug(400, "mhw2epg no more new data, wait for next run");
				break;
			}

			//stop epgscan after 2 min
			if(akttime + 120 < time(NULL))
			{
				debug(400, "mhw2epg timeout");
				break;
			}
		}

		int textlen = buf[14];
		int sumlen = textlen;
		int pos = 15;
		int loop = buf[pos + sumlen] & 0x0f;
		int progid = (buf[3] << 8) | buf[4];

		memcpy(tmpstr, (char*)&buf[pos], textlen);
		tmpstr[sumlen] = ' ';

		sumlen++;
		pos += (textlen + 1);
		if(loop > 0)
		{
			while(loop > 0)
			{
				textlen = buf[pos];
				pos += 1;
				if(pos + textlen < readlen + 3)
				{
					memcpy(&tmpstr[sumlen], (char*)&buf[pos], textlen);
					sumlen += textlen;
					if(loop > 1)
					{
						tmpstr[sumlen] = ' ';
						sumlen++;
					}
      	}
				else
					break;

				pos += textlen;
				loop--;
			}
		}
		tmpstr[sumlen] = '\0';

		cache = getmhwcache(progid);
		if(cache != NULL && cache->epgnode != NULL)
		{
			//tmpstr = stringreplacechar(tmpstr, '\n', ' ');

			//compress long desc
			if(tmpstr[0] != '\0')
			{
				ret = ozip(tmpstr, strlen(tmpstr) + 1, &zbuf, &zlen, 1);
				if(ret == 0)
				{
					free(cache->epgnode->desc); cache->epgnode->desc = NULL;
					cache->epgnode->desc = zbuf;
					cache->epgnode->desccomplen = zlen;
				}
			}
		}
	}

	dmxclose(dmxnode, -1);
	free(buf);
	free(firstbuf);
	return 0;
}

int readmhw2(struct stimerthread* self, struct dvbdev* fenode)
{
	int ret = 0;
	unsigned char* channelbuf = NULL;

	if(fenode == NULL) fenode = status.aktservice->fedev;
	if(fenode == NULL)
	{
		debug(400, "no frontend dev in service");
		return 1;
	}

	channelbuf = calloc(1, MINMALLOC);
	if(channelbuf == NULL)
	{
		err("no memory");
		return 1;
	}

	ret = readmhw2channel(self, fenode, channelbuf);
	if(ret != 0)
	{
		debug(400, "mhw2epg no channel found");
		free(channelbuf); channelbuf = NULL;
		return 1;
	}

	ret = readmhw2title(self, fenode, NULL, channelbuf);
	if(ret != 0)
	{
		debug(400, "mhw2epg no titles found");
		free(channelbuf); channelbuf = NULL;
		freemhwcache();
		return 1;
	}

	ret = readmhw2summary(self, fenode);
	if(ret != 0)
	{
		debug(400, "mhw2epg no summary found");
		free(channelbuf); channelbuf = NULL;
		freemhwcache();
		return 1;
	}

	free(channelbuf); channelbuf = NULL;
	freemhwcache();
	return 0;
}

//skybox
struct mhw2channel* getskyboxchannel(unsigned char* buf, int id)
{
	if(buf == NULL) return NULL;

	int bouquetdesclen = ((buf[8] & 0x0f) << 8) | buf[9];
	int looplen = ((buf[bouquetdesclen + 10] & 0x0f) << 8) | buf[bouquetdesclen + 11];
	int p1 = bouquetdesclen + 12;

	while(looplen > 0)
	{
		unsigned short int tid = (buf[p1] << 8) | buf[p1 + 1];
		unsigned short int nid = (buf[p1 + 2] << 8) | buf[p1 + 3];
		int transpdesclen = ((buf[p1 + 4] & 0x0f) << 8) | buf[p1 + 5];
		int p2 = p1 + 6;

		p1 += transpdesclen + 6;
		looplen -= transpdesclen + 6;
		while(transpdesclen > 0)
		{
			unsigned char desctag = buf[p2];
			int desclen = buf[p2 + 1];
			int p3 = p2 + 2;

			p2 += desclen + 2;
			transpdesclen -= (desclen + 2);
			switch(desctag)
			{
				case 0xb1:
				{
					p3 += 2;
					desclen -= 2;
					while(desclen > 0)
					{
						unsigned short int sid = (buf[p3] << 8) | buf[p3 + 1];
						unsigned short int channelid = (buf[p3 + 3] << 8) | buf[p3 + 4];
						unsigned short int skynr = (buf[p3 + 5] << 8) | buf[p3 + 6];
						if(skynr > 100 && skynr < 1000)
						{
							if(channelid > 0)
							{

							/*
                	sChannel *C;
                	if (ChannelSeq.count (ChannelId) == 0) { //not found
                  	C = &sChannels[nChannels];
                  	C->ChannelId = ChannelId;
                  	//C->NumberOfEquivalences = 1; //there is always an original channel. every equivalence adds 1
                  	C->Src = Source (); //assume all EPG channels are on same satellite, if not, manage this via equivalents!!!
                  	C->Nid = Nid;
                  	C->Tid = Tid;
                  	C->Sid = Sid;
                  	C->SkyNumber = SkyNumber;



                  ChannelSeq[C->ChannelId] = nChannels; //fill lookup table to go from channel-id to sequence nr in table
                  nChannels++;
									}
								 */
							}
						}
					}
					p3 += 9;
					desclen -= 9;
				}
				break;
			default:
				break;
			}
		}
	}

	return NULL;
}

int readskyboxchannel(struct stimerthread* self, struct dvbdev* fenode, unsigned char* buf)
{
	int readlen = 0;
	unsigned short len = 0;
	struct dvbdev* dmxnode;

	if(buf == NULL) return 1;

	if(fenode == NULL) fenode = status.aktservice->fedev;
	if(fenode == NULL)
	{
		debug(400, "no frontend dev in service");
		return 1;
	}

	dmxnode = dmxopen(fenode);
	if(dmxnode == NULL)
	{
		err("open demux dev");
		return 1;
	}

	dmxsetbuffersize(dmxnode, getconfigint("dmxepgbuffersize", NULL));
	dmxsetsource(dmxnode, fenode->fedmxsource);

	dmxsetfilter(dmxnode, 0x11, 0, 19); //0x4a

	readlen = dvbread(dmxnode, buf, 0, 3, 2000000);
	if(readlen != 3)
	{
		dmxclose(dmxnode, -1);
		return 1;
	}
	readlen = 0;
	len = buf[2] | ((buf[1] & 0x0f) << 8);
	if(len + 3 <= MINMALLOC)
		readlen = dvbread(dmxnode, buf, 3, len, 2000000);
	if(readlen <= 0)
	{
		dmxclose(dmxnode, -1);
		return 1;
	}

	dmxclose(dmxnode, -1);
	return 0;
}

int readskyboxtitle(struct stimerthread* self, struct dvbdev* fenode, struct channel* chnode, unsigned char* channelbuf)
{
	int readlen = 0, pos = 0, len = 0;
	unsigned char *buf = NULL;
	struct dvbdev* dmxnode;
	struct mhw2channel* mhw2channel = NULL;
	uint64_t transponderid = 0;
	int serviceid = 0, eventid = 0;
	struct channel* tmpchnode = NULL;
	unsigned long quad = 0, quad0 = 0;
	struct epg* epgnode = NULL;
	time_t dvbtime = 0, starttime = 0, endtime = 0, akttime = 0;
	time_t epgmaxsec = status.epgdays * 24 * 60 * 60;
	char tmpstr[65];
	struct mhwcache* cache = NULL;

	if(chnode == NULL) chnode = status.aktservice->channel;
	if(chnode == NULL || (chnode == status.aktservice->channel && status.aktservice->type != CHANNEL))
		return 1;

	buf = calloc(1, MINMALLOC);
	if(buf == NULL)
	{
		err("no memory");
		return 1;
	}

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

	//AddFilter (0x30, 0xa0, 0xfc); //SKY Titles batch 0 of 7
	dmxsetfilter(dmxnode, 0x234, 0, 18);
	akttime = time(NULL);

	while(self->aktion != STOP && self->aktion != PAUSE)
	{
		readlen = dvbread(dmxnode, buf, 0, 3, 2000000);
		if(readlen != 3)
		{
			dmxclose(dmxnode, -1);
			free(buf);
			return 1;
		}
		readlen = 0;
		len = buf[2] | ((buf[1] & 0x0f) << 8);
		if(len + 3 <= MINMALLOC)
			readlen = dvbread(dmxnode, buf, 3, len, 2000000);
		if(readlen <= 0)
		{
			dmxclose(dmxnode, -1);
			free(buf);
			return 1;
		}

		//check for end
		quad = getquad(buf + 3);
		if(quad0 == 0) quad0 = quad;
		else if(quad == quad0)
		{
			debug(400, "mhw2epg no more new data, wait for next run");

			if(chnode != NULL && chnode->transponder != NULL)
				chnode->transponder->lastepg = time(NULL) + 7700;
			break;
		}

		//stop epgscan after 2 min
		if(akttime + 120 < time(NULL))
		{
			debug(400, "mhw2epg timeout");
			break;
		}

		//////////////

	int p = 0;
	unsigned short int channelid = 0;
	unsigned short int mjdtime = 0;
	int len1 = 0;
	int len2 = 0;

  if(readlen + 3 < 20)
		return 1;

	/*
  if (memcmp (InitialTitle, Data, 20) == 0) //data is the same as initial title
    return 2;
  else {
    if (nTitles == 0)
      memcpy (InitialTitle, Data, 20); //copy data into initial title
	 */


		channelid = (buf[3] << 8) | buf[4];
    mjdtime = ((buf[8] << 8) | buf[9]);
		if(channelid > 0)
		{
			if(mjdtime > 0)
			{
				p = 10;
				do
				{
          //T->EventId = (buf[p] << 8) | buf[p + 1];
          len1 = ((buf[p + 2] & 0x0f) << 8) | buf[p + 3];
          if(buf[p + 4] != 0xb5)
					{
            err("data signature for title - buf[p + 4] != 0xb5");
						break;
					}
					if(len1 > readlen + 3)
					{
						err("data signature for title - len1 > readlen + 3");
						break;
					}
          p += 4;
          len2 = buf[p + 1] - 7;
          //T->StartTime = ((MjdTime - 40587) * 86400) + ((Data[p + 2] << 9) | (Data[p + 3] << 1));
          //T->Duration = ((Data[p + 4] << 9) | (Data[p + 5] << 1));

          unsigned char tmp[4096]; //TODO smarter
					len2 = skyhuffmandecode(&buf[p + 9], len2, tmp, 0);
					if(len2 == 0)
					{
						err("could not huffman-decode title-text, skipping title");
						//return 1; //non-fatal error
					}
					/*)
          T->Text = (unsigned char *) malloc (Len2 + 1);
          if (T->Text == NULL) {
            LogE(0, prep("Titles memory allocation error."));
            return 0;
          }
          T->Text[Len2] = '\0'; //end string with NULL character
          memcpy (T->Text, tmp, Len2);
          CleanString (T->Text);
          T->SummaryAvailable = 1; //TODO I assume this is true?
					*/
          p += len1;
        } while(p < readlen + 3);
      }
    }
  }

	dmxclose(dmxnode, -1);
	return 0;
}

int readskyboxsummary(struct stimerthread* self, struct dvbdev* fenode)
{
	int readlen = 0, pos = 0, len = 0;
	unsigned char *buf = NULL;
	struct dvbdev* dmxnode;
	struct mhw2channel* mhw2channel = NULL;
	uint64_t transponderid = 0;
	int serviceid = 0, eventid = 0;
	struct channel* tmpchnode = NULL;
	unsigned long quad = 0, quad0 = 0;
	struct epg* epgnode = NULL;
	time_t dvbtime = 0, starttime = 0, endtime = 0, akttime = 0;
	time_t epgmaxsec = status.epgdays * 24 * 60 * 60;
	char tmpstr[65];
	struct mhwcache* cache = NULL;

	int p = 0;
  unsigned short int channelid = 0;
  unsigned short int mjdtime = 0;
  int len1 = 0, len2 = 0;

  if(readlen + 3 < 20) {
    return 1; //non fatal error I assume
  }
 // if (memcmp (InitialSummary, Data, 20) == 0) //data is equal to initial buffer
  //  return 2;
//        else if (nSummaries < MAX_SUMMARIES) {
  else {
   // if (nSummaries == 0)
    //  memcpy (InitialSummary, buf, 20); //copy this data in initial buffer
    channelid = (buf[3] << 8) | buf[4];
		mjdtime = ((buf[8] << 8) | buf[9]);
    if(channelid > 0)
		{
      if (mjdtime > 0)
			{
        p = 10;
        do {
          //S->EventId = (buf[p] << 8) | buf[p + 1];
          len1 = ((buf[p + 2] & 0x0f) << 8) | buf[p + 3];
          if(buf[p + 4] != 0xb9)
					{
     //       LogD(5, prep("data signature for title - buf[p + 4] != 0xb5"));
            break;
          }
          if(len1 > readlen + 3) {
          //  LogD(5, prep("data signature for title - len1 > readlen + 3"));
            break;
          }
          p += 4;
          len2 = buf[p + 1];
          unsigned char tmp[4096]; //TODO can this be done better?
          len2 = skyhuffmandecode(&buf[p + 2], len2, tmp, 0);
          if(len2 == 0)
					{
            //LogE(0, prep("Warning, could not huffman-decode text, skipping summary."));
            return 1; //non-fatal error
          }
          //S->Text = (unsigned char *) malloc (Len2 + 1);
          //if (S->Text == NULL) {
          //  LogE(0, prep("Summaries memory allocation error."));
          //  return 0;
          //}
          //memcpy (S->Text, tmp, Len2);
          //S->Text[Len2] = '\0'; //end string with NULL character
          p += len1;
        } while (p < readlen + 3);
      }
    }
  }

	dmxclose(dmxnode, -1);
	return 0;
}

int readskybox(struct stimerthread* self, struct dvbdev* fenode)
{
	int ret = 0;
	unsigned char* channelbuf = NULL;

	if(fenode == NULL) fenode = status.aktservice->fedev;
	if(fenode == NULL)
	{
		debug(400, "no frontend dev in service");
		return 1;
	}

	channelbuf = calloc(1, MINMALLOC);
	if(channelbuf == NULL)
	{
		err("no memory");
		return 1;
	}

	ret = readskyboxchannel(self, fenode, channelbuf);
	if(ret != 0)
	{
		debug(400, "skybox epg no channel found");
		free(channelbuf); channelbuf = NULL;
		return 1;
	}

	ret = readskyboxtitle(self, fenode, NULL, channelbuf);
	if(ret != 0)
	{
		debug(400, "skybox epg no titles found");
		free(channelbuf); channelbuf = NULL;
		freemhwcache();
		return 1;
	}

	ret = readskyboxsummary(self, fenode);
	if(ret != 0)
	{
		debug(400, "skybox epg no summary found");
		free(channelbuf); channelbuf = NULL;
		freemhwcache();
		return 1;
	}

	free(channelbuf); channelbuf = NULL;
	freemhwcache();
	return 0;
}

#endif
