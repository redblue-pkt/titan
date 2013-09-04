#ifndef EXTEPG_H
#define EXTEPG_H

//channel C+ DEMANDE for test on astra 19.2 (mhw)
//channel C+ PORTADA for test on astra 19.2 (mhw2)
//channel Sky Assist for test on hotbird (opentv)
//channel EPG Background Audio for test on 28.2 (opentv)

//global function

struct freesat
{
	unsigned int value;
	short bits;
	char next;
};

typedef struct opentv
{
	char *value;
	struct opentv *p0;
	struct opentv *p1;
} opentvnode;

opentvnode opentvroot;
static struct freesat *freesattable[2][128];
static int freesattablesize[2][128];

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

void freeopentvnode(opentvnode *node)
{
	if(node->p0 != NULL)
	{
		freeopentvnode(node->p0);
		free(node->p0); node->p0 = NULL;
	}

	if(node->p1 != NULL)
	{
		freeopentvnode(node->p1);
		free(node->p1); node->p1 = NULL;
	}

	free(node->value); node->value = NULL;
}

void freeopentv()
{
	freeopentvnode(&opentvroot);
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
int loadopentv(char *file)
{
	FILE *fd = NULL;
	char line[512];
	char value[256];
	char code[256];
	opentvnode *node;
	int len = 0;
	int count = 0;
	int i = 0;

	opentvroot.value = NULL;
	opentvroot.p0 = NULL;
	opentvroot.p1 = NULL;

	fd = fopen(file, "r");
	if(!fd)
	{
		perr("load %s", file);
		return 1;
	}

	while(fgets(line, sizeof(line), fd))
	{
		memset(value, 0, sizeof(value));
		memset(code, 0, sizeof(code));

		if(sscanf(line, "%c=%[^\n]\n", value, code) != 2)
		{
			if(sscanf(line, "%[^=]=%[^\n]\n", value, code) != 2)
			{
				if(sscanf(line, "=%[^\n]\n", code) != 1)
					continue;
				else
					memset(value, 0, sizeof (value));
			}
		}

		node = &opentvroot;
		len = strlen(code);

		for(i = 0; i < len; i++)
		{
			switch(code[i])
			{
				case '0':
					if(node->p0 == NULL)
					{
						node->p0 = malloc(sizeof(opentvnode));
						node = node->p0;
						node->value = NULL;
						node->p0 = NULL;
						node->p1 = NULL;
						if(len == (i + 1))
						{
							node->value = malloc(sizeof(char) * (strlen(value) + 1));
							sprintf(node->value, "%s", value);
							count++;
						}
					}
					else
					{
						node = node->p0;
						if((node->value != NULL) || (len == (i + 1)))
							err("huffman prefix code '%s' already exist", code);
					}
					break;

				case '1':
					if(node->p1 == NULL)
					{
						node->p1 = malloc(sizeof(opentvnode));
						node = node->p1;
						node->value = NULL;
						node->p0 = NULL;
						node->p1 = NULL;
						if(len == (i + 1))
						{
							node->value = malloc(sizeof (char) * (strlen(value) + 1));
							sprintf(node->value, "%s", value);
							count++;
						}
					}
					else
					{
						node = node->p1;
						if((node->value != NULL) || (len == (i + 1)))
							err("huffman prefix code '%s' already exist", code);
					}
					break;
			}
		}
	}

	fclose(fd);
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
int skyhuffmandecode(unsigned char *buf, int len, unsigned char *result, int resultmaxlen)
{
	opentvnode *node = &opentvroot;
	unsigned char byte = 0;
	unsigned char mask = 0;
	int index = 0;
	int toolong = 0;
	int i = 0;

	if(resultmaxlen > MINMALLOC) resultmaxlen = MINMALLOC;

	for(i = 0; i < len; i++)
	{
		byte = buf[i];
		if(i == 0) mask = 0x20;
		else mask = 0x80;

		do
		{
			if((byte & mask) == 0)
			{
				if(node->p0 != NULL) node = node->p0;
				else
				{
					err("cannot decode huffman data");
					return 1;
				}
			}
			else
			{
				if (node->p1 != NULL) node = node->p1;
				else
				{
					err("cannot decode huffman data");
					return 1;
				}
			}

			if(node->value != NULL)
			{
				int size = 0;

				if((index + strlen(node->value)) >= (resultmaxlen - 1))
				{
					size = resultmaxlen - len - 1;
					toolong = 1;
				}
				else size = strlen(node->value);

				memcpy(result + index, node->value, size);
				index += size;
				node = &opentvroot;
			}

			if(toolong) break;

			mask = mask >> 1;
		} while(mask > 0);

		if(toolong == 1)
		{
			debug(400, "huffman string is too long. truncated");
			break;
		}
	}

	result[index] = '\0';

	return 0;
}

struct extepgcache* addextepgcache(int id, struct epg* epgnode, struct extepgcache* last)
{
	struct extepgcache *newnode = NULL, *prev = NULL, *node = extepgcache;

	newnode = (struct extepgcache*)calloc(1, sizeof(struct extepgcache));
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
		extepgcache = newnode;
	else
		prev->next = newnode;
	newnode->next = node;

	return newnode;
}

void delextepgcache(struct extepgcache* cache)
{
	struct extepgcache *node = extepgcache, *prev = extepgcache;

	while(node != NULL)
	{
		if(node == cache)
		{
			if(node == extepgcache)
				extepgcache = node->next;
			else
				prev->next = node->next;

			free(node);
			node = NULL;
			break;
		}

		prev = node;
		node = node->next;
	}
}

void freeextepgcache()
{
	struct extepgcache *node = extepgcache, *prev = extepgcache;

	while(node != NULL)
	{
		prev = node;
		node = node->next;
		if(prev != NULL)
			delextepgcache(prev);
	}
}

struct extepgcache* getextepgcache(int id)
{
	struct extepgcache *node = extepgcache;

	while(node != NULL)
	{
		if(node->id == id)
			return node;

		node = node->next;
	}
	debug(100, "extepgcache not found (extepgcache=%d)", id);
	return NULL;
}

struct extepgchannel* addextepgchannel(int id, int serviceid, off64_t transponderid, struct extepgchannel* last)
{
	struct extepgchannel *newnode = NULL, *prev = NULL, *node = extepgchannel;

	newnode = (struct extepgchannel*)calloc(1, sizeof(struct extepgchannel));
	if(newnode == NULL)
	{
		err("no memory");
		return NULL;
	}

	newnode->id = id;
	newnode->serviceid = serviceid;
	newnode->transponderid = transponderid;

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
		extepgchannel = newnode;
	else
		prev->next = newnode;
	newnode->next = node;

	return newnode;
}

void delextepgchannel(struct extepgchannel* channel)
{
	struct extepgchannel *node = extepgchannel, *prev = extepgchannel;

	while(node != NULL)
	{
		if(node == channel)
		{
			if(node == extepgchannel)
				extepgchannel = node->next;
			else
				prev->next = node->next;

			free(node);
			node = NULL;
			break;
		}

		prev = node;
		node = node->next;
	}
}

void freeextepgchannel()
{
	struct extepgchannel *node = extepgchannel, *prev = extepgchannel;

	while(node != NULL)
	{
		prev = node;
		node = node->next;
		if(prev != NULL)
			delextepgchannel(prev);
	}
}

struct extepgchannel* getextepgchannel(int id)
{
	struct extepgchannel *node = extepgchannel;

	while(node != NULL)
	{
		if(node->id == id)
			return node;

		node = node->next;
	}
	debug(100, "extepgchannel not found (extepgchannel=%d)", id);
	return NULL;
}

struct extepgconfig* addextepgconfig(char *line, int count, struct extepgconfig* last)
{
	struct extepgconfig *newnode = NULL, *prev = NULL, *node = extepgconfig;
	char *file = NULL;
	int ret = 0;

	if(line == NULL) return NULL;

	newnode = (struct extepgconfig*)calloc(1, sizeof(struct extepgconfig));
	if(newnode == NULL)
	{
		err("no memory");
		return NULL;
	}

	file = malloc(MINMALLOC);
	if(file == NULL)
	{
		err("no memory");
		free(newnode);
		return NULL;
	}

	ret = sscanf(line, "%llu#%d#%[^#]#%d#%d#%d#%d#%d#%d#%d#%d#%d#%d#%d#%d#%d#%d#%d#%d#%d", &newnode->transponderid, &newnode->type, file, &newnode->channelpid, \
		&newnode->titlepid[0], &newnode->titlepid[1], &newnode->titlepid[2], &newnode->titlepid[3], \
		&newnode->titlepid[4], &newnode->titlepid[5], &newnode->titlepid[6], &newnode->titlepid[7], \
		&newnode->summarypid[0], &newnode->summarypid[1], &newnode->summarypid[2], &newnode->summarypid[3], \
		&newnode->summarypid[4], &newnode->summarypid[5], &newnode->summarypid[6], &newnode->summarypid[7]);
	if(ret != 20)
	{
		if(count > 0)
		{
			err("extepgconfig line %d not ok or double", count);
		}
		else
		{
			err("add extepgconfig");
		}
		free(file);
		free(newnode);
		return NULL;
	}

	newnode->file = ostrshrink(file);

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
		extepgconfig = newnode;
	else
		prev->next = newnode;
	newnode->next = node;

	return newnode;
}

int readextepgconfig(char* filename)
{
	FILE *fd = NULL;
	char *fileline = NULL;
	int linecount = 0, len = 0;
	struct extepgconfig* last = NULL, *tmplast = NULL;

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
		len = strlen(fileline) - 1;
		if(len >= 0 && fileline[len] == '\n')
			fileline[len] = '\0';
		len--;
		if(len >= 0 && fileline[len] == '\r')
			fileline[len] = '\0';

		linecount++;

		if(last == NULL) last = tmplast;
		last = addextepgconfig(fileline, linecount, last);
		if(last != NULL) tmplast = last;
	}

	free(fileline);
	fclose(fd);
	return 0;
}

void delextepgconfig(struct extepgconfig* config)
{
	struct extepgconfig *node = extepgconfig, *prev = extepgconfig;

	while(node != NULL)
	{
		if(node == config)
		{
			if(node == extepgconfig)
				extepgconfig = node->next;
			else
				prev->next = node->next;

			free(node->file);
			node->file = NULL;

			free(node);
			node = NULL;
			break;
		}

		prev = node;
		node = node->next;
	}
}

void freeextepgconfig()
{
	struct extepgconfig *node = extepgconfig, *prev = extepgconfig;

	while(node != NULL)
	{
		prev = node;
		node = node->next;
		if(prev != NULL)
			delextepgconfig(prev);
	}
}

struct extepgconfig* getextepgconfig(uint64_t transponderid, int type)
{
	struct extepgconfig *node = extepgconfig;

	while(node != NULL)
	{
		if(node->transponderid == transponderid && node->type == type)
			return node;

		node = node->next;
	}
	debug(100, "extepgconfig not found");
	return NULL;
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

int readmhwchannel(struct stimerthread* self, struct dvbdev* fenode, unsigned char* channelbuf, int pid)
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

	dmxsetfilter(dmxnode, pid, 0, 16);

	readlen = dvbread(dmxnode, channelbuf, 0, MINMALLOC, 2000000);
	if(readlen <= 0)
	{
		dmxclose(dmxnode, -1);
		return -1;
	}

	dmxclose(dmxnode, -1);
	return readlen;
}

//flag 0 = from epg thread
//flag 1 = from epg scan
int readmhwtitle(struct stimerthread* self, struct dvbdev* fenode, struct channel* chnode, unsigned char* channelbuf, int channelcount, int pid, int flag)
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
	struct extepgcache* cache = NULL;

	if(chnode == NULL) return 1;

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

	dmxsetfilter(dmxnode, pid, 0, 15);
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
				continue;

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
				m_unlock(&status.epgmutex, 4);
				continue;
			}

			epgnode->title = ostrcat((char*)mhwtitle->title, NULL, 0, 0);
			epgnode->title = strstrip(epgnode->title);
			cache = addextepgcache(HILO32(mhwtitle->program_id), epgnode, cache);

			m_unlock(&status.epgmutex, 4);
		}
	}

	dmxclose(dmxnode, -1);
	free(buf);
	free(firstbuf);
	return 0;
}

int readmhwsummary(struct stimerthread* self, struct dvbdev* fenode, int pid)
{
	int readlen = 0, first = 1, len = 0;
	unsigned char *buf = NULL, *firstbuf = NULL;
	struct dvbdev* dmxnode;
	time_t akttime = 0;
	char* tmpstr = NULL;
	struct mhwsummary* mhwsummary = NULL;
	char* zbuf = NULL;
	int zlen = 0, ret = 0;
	struct extepgcache* cache = NULL;

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

	dmxsetfilter(dmxnode, pid, 0, 17);
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

		cache = getextepgcache(HILO32(mhwsummary->program_id));
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

//flag 0 = from epg thread
//flag 1 = from epg scan
int readmhw(struct stimerthread* self, struct channel* chnode, struct dvbdev* fenode, int flag)
{
	int ret = 0, channelcount = 0, i = 0;
	unsigned char* channelbuf = NULL;
	struct extepgconfig* extepgconfignode = NULL;

	if(self == NULL) return 1;

	if(chnode == NULL) chnode = status.aktservice->channel;
	if(chnode == NULL || (flag == 0 && chnode == status.aktservice->channel && status.aktservice->type != CHANNEL))
		return 1;

	extepgconfignode = getextepgconfig(chnode->transponderid, 0);
	if(extepgconfignode == NULL) extepgconfignode = getextepgconfig(0, 0);
	if(extepgconfignode == NULL)
	{
		debug(400, "transponder not found in extepgconfig");
		return 1;
	}

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

	channelcount = readmhwchannel(self, fenode, channelbuf, extepgconfignode->channelpid);
	if(channelcount <= 0 || self->aktion == STOP || self->aktion == PAUSE)
	{
		debug(400, "mhwepg no channels found");
		ret = 1;
		goto end;
	}

	ret = 1;
	for(i = 0; i < 8; i++)
	{
		if(extepgconfignode->titlepid[i] < 0) continue;
		if(self->aktion == STOP || self->aktion == PAUSE) break;
		int tmpret = readmhwtitle(self, fenode, chnode, channelbuf, channelcount, extepgconfignode->titlepid[i], flag);
		if(tmpret == 0) ret = 0;
	}
	if(ret != 0 || self->aktion == STOP || self->aktion == PAUSE)
	{
		debug(400, "mhwepg no titles found");
		ret = 1;
		goto end;
	}

	ret = 1;
	for(i = 0; i < 8; i++)
	{
		if(extepgconfignode->summarypid[i] < 0) continue;
		if(self->aktion == STOP || self->aktion == PAUSE) break;
		int tmpret = readmhwsummary(self, fenode, extepgconfignode->summarypid[i]);
		if(tmpret == 0) ret = 0;
	}
	if(ret != 0 || self->aktion == STOP || self->aktion == PAUSE)
	{
		debug(400, "mhwepg no summary found");
		ret = 1;
		goto end;
	}

	ret = 0;

end:

	free(channelbuf); channelbuf = NULL;
	freeextepgcache();
	return ret;
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

int readmhw2channel(struct stimerthread* self, struct dvbdev* fenode, unsigned char* channelbuf, int pid)
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

	dmxsetfilter(dmxnode, pid, 0, 19);

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

//flag 0 = from epg thread
//flag 1 = from epg scan
int readmhw2title(struct stimerthread* self, struct dvbdev* fenode, struct channel* chnode, unsigned char* channelbuf, int pid, int flag)
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
	struct extepgcache* cache = NULL;

	if(chnode == NULL) return 1;

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

	dmxsetfilter(dmxnode, pid, 0, 18);
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
					m_unlock(&status.epgmutex, 4);
					pos += 3;
					continue;
				}

				epgnode->title = ostrcat(tmpstr, NULL, 0, 0);
				cache = addextepgcache(progid, epgnode, cache);

				m_unlock(&status.epgmutex, 4);

			}
			pos += 3;
		}
	}

	dmxclose(dmxnode, -1);
	free(buf);
	return 0;
}

int readmhw2summary(struct stimerthread* self, struct dvbdev* fenode, int pid)
{
	int readlen = 0, len = 0, first = 1;
	unsigned char *buf = NULL, *firstbuf = NULL;
	struct dvbdev* dmxnode;
	time_t akttime = 0;
	char* zbuf = NULL;
	int zlen = 0, ret = 0;
	char tmpstr[MINMALLOC];
	struct extepgcache* cache = NULL;

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

	dmxsetfilter(dmxnode, pid, 0, 20);
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
			free(firstbuf);
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

		cache = getextepgcache(progid);
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

//flag 0 = from epg thread
//flag 1 = from epg scan
int readmhw2(struct stimerthread* self, struct channel* chnode, struct dvbdev* fenode, int flag)
{
	int ret = 0, i = 0;
	unsigned char* channelbuf = NULL;
	struct extepgconfig* extepgconfignode = NULL;

	if(self == NULL) return 1;

	if(chnode == NULL) chnode = status.aktservice->channel;
	if(chnode == NULL || (flag == 0 && chnode == status.aktservice->channel && status.aktservice->type != CHANNEL))
		return 1;

	extepgconfignode = getextepgconfig(chnode->transponderid, 1);
	if(extepgconfignode == NULL) extepgconfignode = getextepgconfig(0, 1);
	if(extepgconfignode == NULL)
	{
		debug(400, "transponder not found in extepgconfig");
		return 1;
	}

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

	ret = readmhw2channel(self, fenode, channelbuf, extepgconfignode->channelpid);
	if(ret != 0 || self->aktion == STOP || self->aktion == PAUSE)
	{
		debug(400, "mhw2epg no channel found");
		ret = 1;
		goto end;
	}

	ret = 1;
	for(i = 0; i < 8; i++)
	{
		if(extepgconfignode->titlepid[i] < 0) continue;
		if(self->aktion == STOP || self->aktion == PAUSE) break;
		int tmpret = readmhw2title(self, fenode, chnode, channelbuf, extepgconfignode->titlepid[i], flag);
		if(tmpret == 0) ret = 0;
	}
	if(ret != 0 || self->aktion == STOP || self->aktion == PAUSE)
	{
		debug(400, "mhw2epg no titles found");
		ret = 1;
		goto end;
	}

	ret = 1;
	for(i = 0; i < 8; i++)
	{
		if(extepgconfignode->summarypid[i] < 0) continue;
		if(self->aktion == STOP || self->aktion == PAUSE) break;
		int tmpret = readmhw2summary(self, fenode, extepgconfignode->summarypid[i]);
		if(tmpret == 0) ret = 0;
	}
	if(ret != 0 || self->aktion == STOP || self->aktion == PAUSE)
	{
		debug(400, "mhw2epg no summary found");
		ret = 1;
		goto end;
	}

	ret = 0;

end:

	free(channelbuf); channelbuf = NULL;
	freeextepgcache();
	return 0;
}

//opentv
//flag 0 = from epg thread
//flag 1 = from epg scan
int createopentvchannel(unsigned char* channelbuf, struct channel* chnode, int flag)
{
	if(channelbuf == NULL) return 1;

	if(chnode == NULL) return 1;

	int bouquetdesclen = ((channelbuf[8] & 0x0f) << 8) | channelbuf[9];
	int looplen = ((channelbuf[bouquetdesclen + 10] & 0x0f) << 8) | channelbuf[bouquetdesclen + 11];
	int p1 = bouquetdesclen + 12;

	while(looplen > 0)
	{
		unsigned short int tid = (channelbuf[p1] << 8) | channelbuf[p1 + 1];
		unsigned short int nid = (channelbuf[p1 + 2] << 8) | channelbuf[p1 + 3];
		int transpdesclen = ((channelbuf[p1 + 4] & 0x0f) << 8) | channelbuf[p1 + 5];
		int p2 = p1 + 6;

		p1 += transpdesclen + 6;
		looplen -= (transpdesclen + 6);
		while(transpdesclen > 0)
		{
			unsigned char desctag = channelbuf[p2];
			int desclen = channelbuf[p2 + 1];
			int p3 = p2 + 2;

			p2 += desclen + 2;
			transpdesclen -= (desclen + 2);
			if(desctag == 0xb1)
			{
				p3 += 2;
				desclen -= 2;
				while(desclen > 0)
				{
					unsigned short int sid = (channelbuf[p3] << 8) | channelbuf[p3 + 1];
					unsigned short int channelid = (channelbuf[p3 + 3] << 8) | channelbuf[p3 + 4];
					if(getextepgchannel(channelid) == NULL)
					{
						uint64_t transponderid = ((nid << 16) | tid) & 0xffffffff;
						if(chnode->transponder != NULL)
						{
							if(chnode->transponder->fetype == FE_QAM)
								transponderid = transponderid | ((uint64_t)1 << 32);
							else if(chnode->transponder->fetype == FE_OFDM)
								transponderid = transponderid | ((uint64_t)2 << 32);
						}
						addextepgchannel(channelid, sid, transponderid, NULL);
					}
					p3 += 9;
					desclen -= 9;
				}
			}
		}
	}

	return 0;
}

//flag 0 = from epg thread
//flag 1 = from epg scan
int readopentvchannel(struct stimerthread* self, struct dvbdev* fenode, struct channel* chnode, unsigned char* channelbuf, int pid, int flag)
{
	int readlen = 0, first = 1;
	unsigned short len = 0;
	struct dvbdev* dmxnode;
	char* firstbuf = NULL;
	time_t akttime = 0;

	if(channelbuf == NULL) return 1;

	if(chnode == NULL) return 1;

	firstbuf = calloc(1, MINMALLOC);
	if(firstbuf == NULL)
	{
		err("no memory");
		return 1;
	}

	if(fenode == NULL) fenode = status.aktservice->fedev;
	if(fenode == NULL)
	{
		debug(400, "no frontend dev in service");
		free(firstbuf);
		return 1;
	}

	dmxnode = dmxopen(fenode);
	if(dmxnode == NULL)
	{
		err("open demux dev");
		free(firstbuf);
		return 1;
	}

	dmxsetbuffersize(dmxnode, getconfigint("dmxepgbuffersize", NULL));
	dmxsetsource(dmxnode, fenode->fedmxsource);

	dmxsetfilter(dmxnode, pid, 0, 21);
	akttime = time(NULL);

	while(self->aktion != STOP && self->aktion != PAUSE)
	{
		readlen = dvbread(dmxnode, channelbuf, 0, 3, 5000000);
		if(readlen <= 0)
		{
			dmxclose(dmxnode, -1);
			free(firstbuf);
			return 1;
		}
		readlen = 0;
		len = channelbuf[2] | ((channelbuf[1] & 0x0f) << 8);
		if(len + 3 <= MINMALLOC)
			readlen = dvbread(dmxnode, channelbuf, 3, len, 2000000);
		if(readlen <= 0)
		{
			dmxclose(dmxnode, -1);
			free(firstbuf);
			return 1;
		}

		//check for end
		if(first == 1)
		{
			first = 0;
			memcpy(firstbuf, channelbuf, readlen + 3);
		}
		else
		{
			if(memcmp(firstbuf, channelbuf, readlen + 3) == 0)
			{
				debug(400, "opentv no more new data, wait for next run");
				break;
			}

			//stop epgscan after 2 min
			if(akttime + 120 < time(NULL))
			{
				debug(400, "opentv timeout");
				break;
			}
		}

		createopentvchannel(channelbuf, channel, flag);
	}

	dmxclose(dmxnode, -1);
	free(firstbuf);
	return 0;
}

//flag 0 = from epg thread
//flag 1 = from epg scan
int readopentvtitle(struct stimerthread* self, struct dvbdev* fenode, struct channel* chnode, unsigned char* channelbuf, int pid, int flag)
{
	int readlen = 0, first = 1, ret = 1, len = 0, count = 0;
	unsigned char *buf = NULL, *firstbuf = NULL;
	struct dvbdev* dmxnode;
	uint64_t transponderid = 0;
	int serviceid = 0;
	struct channel* tmpchnode = NULL;
	struct epg* epgnode = NULL;
	time_t dvbtime = 0, starttime = 0, endtime = 0, akttime = 0;
	time_t epgmaxsec = status.epgdays * 24 * 60 * 60;
	struct extepgcache* cache = NULL;

	if(chnode == NULL) return 1;

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

	dmxsetfilter(dmxnode, pid, 0, 22);
	akttime = time(NULL);

	while(self->aktion != STOP && self->aktion != PAUSE)
	{
start:
		readlen = dvbread(dmxnode, buf, 0, 3, 2000000);
		if(readlen != 3)
		{
			count++;
			if(count < 5 && self->aktion != STOP && self->aktion != PAUSE) goto start;
			break;
		}

		readlen = 0;
		len = buf[2] | ((buf[1] & 0x0f) << 8);
		if(len + 3 <= MINMALLOC)
			readlen = dvbread(dmxnode, buf, 3, len, 2000000);
		if(readlen <= 0)
			break;

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
				debug(400, "opentv no more new data, wait for next run");
				break;
			}

			//stop epgscan after 2 min
			if(akttime + 120 < time(NULL))
			{
				debug(400, "opentv timeout");
				break;
			}
		}

		int p = 0;
		unsigned short int channelid = 0;
		unsigned short int mjdtime = 0;

		channelid = (buf[3] << 8) | buf[4];
		mjdtime = ((buf[8] << 8) | buf[9]);
		if(channelid > 0 && mjdtime > 0)
		{
			p = 10;
			while((p + 11) < readlen + 3)
			{
				unsigned short int eventid = 0;
				unsigned char	desclen = 0;
				unsigned short int packetlen = ((buf[p + 2] & 0x0f) << 8) | buf[p + 3];

				if((buf[p + 4] != 0xb5) || ((packetlen + p) > readlen + 3)) break;

				eventid = (buf[p] << 8) | buf[p + 1];
				p += 4;
				desclen = buf[p + 1] - 7;

				if((p + 9 + desclen) > readlen + 3) break;

				struct extepgchannel* extepgchannelnode = getextepgchannel(channelid);
				if(extepgchannelnode != NULL)
				{
					unsigned char tmpstr[256];

					if(skyhuffmandecode(buf + p + 9, desclen, tmpstr, 256) != 0)
						tmpstr[0] = '\0';
					else
					{
						ret = 0;

						//get start/endtime
						dvbtime = ((mjdtime - 40587) * 86400) + ((buf[p + 2] << 9) | (buf[p + 3] << 1));
						starttime = dvbtime;
						endtime = starttime + ((buf[p + 4] << 9) | (buf[p + 5] << 1));

						tmpchnode = getchannel(extepgchannelnode->serviceid, extepgchannelnode->transponderid);
						if(tmpchnode == NULL)
						{
							p += packetlen;
							continue;
						}

						//look in epglist if channel exists
						//only if epglist is marked as whitelist
						if(status.epglistmode == 2 || status.epglistmode == 3)
						{
							if(getepgscanlist(serviceid, transponderid) == NULL)
							{
								p += packetlen;
								continue;
							}
						}

						m_lock(&status.epgmutex, 4);

						epgnode = getepg(tmpchnode, eventid, 1);

#ifndef SIMULATE
						if(endtime < time(NULL) || starttime > time(NULL) + epgmaxsec)
						{
							m_unlock(&status.epgmutex, 4);
							p += packetlen;
							continue;
						}
#endif

						if(epgnode == NULL)
							epgnode = addepg(tmpchnode, eventid, 0, starttime, endtime, NULL, 1);
						else
							updateepg(tmpchnode, epgnode, eventid, 0, starttime, endtime, 1);

						if(epgnode == NULL)
						{
							m_unlock(&status.epgmutex, 4);
							p += packetlen;
							continue;
						}

						epgnode->title = ostrcat((char*)tmpstr, NULL, 0, 0);
						cache = addextepgcache((channelid << 16) | eventid, epgnode, cache);

						m_unlock(&status.epgmutex, 4);
					}
				}

				p += packetlen;
			}
		}
	}

	dmxclose(dmxnode, -1);
	free(buf);
	free(firstbuf);
	return ret;
}

int readopentvsummary(struct stimerthread* self, struct dvbdev* fenode, int pid)
{
	int readlen = 0, first = 1, ret = 1, len = 0, zret = 0, zlen = 0;
	unsigned char *buf = NULL, *firstbuf = NULL;
	struct dvbdev* dmxnode;
	time_t akttime = 0;
	struct extepgcache* cache = NULL;
	char* zbuf = NULL;

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

	dmxsetfilter(dmxnode, pid, 0, 23);
	akttime = time(NULL);

	while(self->aktion != STOP && self->aktion != PAUSE)
	{
		readlen = dvbread(dmxnode, buf, 0, 3, 2000000);
		if(readlen != 3)
			break;

		readlen = 0;
		len = buf[2] | ((buf[1] & 0x0f) << 8);
		if(len + 3 <= MINMALLOC)
			readlen = dvbread(dmxnode, buf, 3, len, 2000000);
		if(readlen <= 0)
			break;

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
				debug(400, "opentv no more new data, wait for next run");
				break;
			}

			//stop epgscan after 2 min
			if(akttime + 120 < time(NULL))
			{
				debug(400, "opentv timeout");
				break;
			}
		}

		if(readlen + 3 < 20) continue;

		int p = 0;
		unsigned short int channelid = (buf[3] << 8) | buf[4];
		unsigned short int mjdtime = (buf[8] << 8) | buf[9];

		if((channelid > 0) && (mjdtime > 0))
		{
			p = 10;
			while(p < readlen + 3)
			{
				unsigned short int eventid = 0;
				unsigned char desclen = 0;
				unsigned short int packetlen = ((buf[p + 2] & 0x0f) << 8) | buf[p + 3];

				if((buf[p + 4] != 0xb9) || ((packetlen + p) > readlen + 3)) break;

				eventid = (buf[p] << 8) | buf[p + 1];
				p += 4;
				desclen = buf[p + 1];

				char tmpstr[MINMALLOC];
				memset(tmpstr, 0, MINMALLOC);

				if(skyhuffmandecode(buf + p + 2, desclen, (unsigned char*)tmpstr, MINMALLOC) != 0)
					tmpstr[0] = '\0';
				else
				{
					cache = getextepgcache((channelid << 16) | eventid);
					if(cache != NULL && cache->epgnode != NULL)
					{
						ret = 0;
						//tmpstr = stringreplacechar(tmpstr, '\n', ' ');

						//compress long desc
						if(tmpstr[0] != '\0')
						{
							zret = ozip(tmpstr, strlen(tmpstr) + 1, &zbuf, &zlen, 1);
							if(zret == 0)
							{
								free(cache->epgnode->desc); cache->epgnode->desc = NULL;
								cache->epgnode->desc = zbuf;
								cache->epgnode->desccomplen = zlen;
							}
						}
					}
				}

				p += packetlen;
			}
		}
	}

	dmxclose(dmxnode, -1);
	free(buf);
	free(firstbuf);
	return ret;
}

//flag 0 = from epg thread
//flag 1 = from epg scan
int readopentv(struct stimerthread* self, struct channel* chnode, struct dvbdev* fenode, int flag)
{
	int ret = 0, i = 0;
	unsigned char* channelbuf = NULL;
	struct extepgconfig* extepgconfignode = NULL;

	if(self == NULL) return 1;

	if(chnode == NULL) chnode = status.aktservice->channel;
	if(chnode == NULL || (flag == 0 && chnode == status.aktservice->channel && status.aktservice->type != CHANNEL))
		return 1;

	extepgconfignode = getextepgconfig(chnode->transponderid, 2);
	if(extepgconfignode == NULL)
	{
		debug(400, "transponder not found in extepgconfig");
		return 1;
	}

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

	ret = loadopentv(extepgconfignode->file);
	if(ret != 0 || self->aktion == STOP || self->aktion == PAUSE)
	{
		debug(400, "opentv can't load dict");
		ret = 1;
		goto end;
	}

	ret = readopentvchannel(self, fenode, chnode, channelbuf, extepgconfignode->channelpid, flag);
	if(ret != 0 || self->aktion == STOP || self->aktion == PAUSE)
	{
		debug(400, "opentv epg no channel found");
		ret = 1;
		goto end;
	}

	ret = 1;
	for(i = 0; i < 8; i++)
	{
		if(extepgconfignode->titlepid[i] < 0) continue;
		if(self->aktion == STOP || self->aktion == PAUSE) break;
		int tmpret = readopentvtitle(self, fenode, chnode, channelbuf, extepgconfignode->titlepid[i], flag);
		if(tmpret == 0) ret = 0;
	}
	if(ret != 0 || self->aktion == STOP || self->aktion == PAUSE)
	{
		debug(400, "opentv epg no titles found");
		ret = 1;
		goto end;
	}

	ret = 1;
	for(i = 0; i < 8; i++)
	{
		if(extepgconfignode->summarypid[i] < 0) continue;
		if(self->aktion == STOP || self->aktion == PAUSE) break;
		int tmpret = readopentvsummary(self, fenode, extepgconfignode->summarypid[i]);
		if(tmpret == 0) ret = 0;
	}
	if(ret != 0 || self->aktion == STOP || self->aktion == PAUSE)
	{
		debug(400, "opentv epg no summary found");
		ret = 1;
		goto end;
	}

	ret = 0;

end:

	free(channelbuf); channelbuf = NULL;
	freeextepgchannel();
	freeextepgcache();
	freeopentv();
	return ret;
}

#endif
