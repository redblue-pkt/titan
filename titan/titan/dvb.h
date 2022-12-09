#ifndef DVB_H
#define DVB_H

int dvbwrite(int fd, unsigned char* buf, int count, int tout)
{
	struct timeval timeout;
	int ret = 0, usec = 0, sec = 0, tmpcount = count;
	unsigned char* buffer = NULL;

	if(fd < 0) return -1;

	if(tout == -1) tout = 5000 * 1000;
	usec = tout % 1000000;
	sec = (tout - usec) / 1000000;

	fd_set wfds;
 
	while (tmpcount > 0)
	{
		buffer = buf + (count - tmpcount);
		ret = write(fd, buffer, tmpcount);
		if(ret < 0)
		{
			if(errno == EINTR || errno == EAGAIN)
			{
				FD_ZERO(&wfds);
				FD_SET(fd, &wfds);
			
				timeout.tv_sec = sec;
				timeout.tv_usec = usec;
			
				ret = TEMP_FAILURE_RETRY(select(fd + 1, NULL, &wfds, NULL, &timeout));
			}
			
			if(ret == 0)
			{
				perr("dvbwrite timed out fd=%d", fd);
				return -1;
			}
			if(ret < 0)
			{
				perr("can't write fd=%d", fd);
				return errno * -1;
			}
		}
		else
			tmpcount -= ret;
	}

	return count;
}

//flag 0: eof with timeout
//flag 1: eof without timeout
int dvbreadfd(int fd, unsigned char *buf, int pos, int count, int tout, int flag)
{
	struct timeval timeout;
	int ret = 0, usec = 0, sec = 0;
	fd_set rfds;
	
	if(fd < 0) return -1;

	if(tout == -1) tout = 5000 * 1000;
	usec = tout % 1000000;
	sec = (tout - usec) / 1000000;

	timeout.tv_sec = sec;
	timeout.tv_usec = usec;
	FD_ZERO(&rfds);
	FD_SET(fd, &rfds);

	ret = TEMP_FAILURE_RETRY(select(fd + 1, &rfds , NULL, NULL, &timeout));

	if(ret == 1)
	{
retry:
		ret = TEMP_FAILURE_RETRY(read(fd, buf + pos, count));
		if(ret > 0)
			return ret;
		else if(ret == 0)
		{
#ifdef SIMULATE
			tout = tout - 100000;
			usleep(100000);
#else
			tout = tout - 1000;
			usleep(1000);
#endif
			if(flag == 0 && tout > 0) goto retry;
			debug(200, "dvb read timeout fd=%d", fd);
		}
		else if(ret < 0)
		{
			if((errno == EAGAIN || errno == EOVERFLOW) && tout > 0) 
			{
				if(errno != EAGAIN) perr("dvb read data fd=%d -> retry", fd);
#ifdef SIMULATE
				tout = tout - 100000;
				usleep(100000);
#else
				tout = tout - 1000;
				usleep(1000);
#endif
				goto retry;
			}
			perr("dvb read data fd=%d", fd);
		}
	}
	else if(ret == 0)
	{
		debug(200, "dvb select timeout fd=%d, tout=%d", fd, tout);
	}
	else
	{
		perr("dvb select fd=%d", fd);
	}

	return -1;
}

int dvbread(struct dvbdev* node, unsigned char *buf, int pos, int count, int tout)
{
	if(node == NULL) return -2;
	return dvbreadfd(node->fd, buf, pos, count, tout, 0);
}

int dvbfindpmtpid(int fd, int16_t *pmtpid, int *serviceid, int tssize)
{
	off_t pos = 0;
	int left = 5 * 1024 * 1024;

	if(fd < 0) return 1;

	while(left >= tssize)
	{
		unsigned char packet[tssize];

		lseek64(fd, pos, SEEK_SET);
		int ret = dvbreadfd(fd, packet, 0, tssize, -1, 0);
		if(ret != tssize)
		{
			err("read error");
			break;
		}
		left -= tssize;
		pos += tssize;

		if(packet[0] != 0x47)
		{
			int i = 0;
			while(i < tssize)
			{
				if(packet[i] == 0x47) break;
				--pos;
				++i;
			}
			continue;
		}

		int pid = ((packet[1] << 8) | packet[2]) & 0x1FFF;

		int pusi = !!(packet[1] & 0x40);
		if(!pusi) continue;
		
		// ok, now we have a PES header or section header
		unsigned char *sec;
		
		// check for adaption field
		if(packet[3] & 0x20)
		{
			if(packet[4] >= 183) continue;
			sec = packet + packet[4] + 4 + 1;
		}
		else
			sec = packet + 4;
		
		// table pointer, assumed to be 0
		if(sec[0]) continue;

		// program map section
		if(sec[1] == 0x02)
		{
			*pmtpid = pid;
			*serviceid = (sec[4] << 8) | sec[5];
			lseek64(fd, 0, SEEK_SET);
			return 0;
		}
	}
	
	lseek64(fd, 0, SEEK_SET);
	return 1;
}

int dvbgetpmtpid(unsigned char *buf, int serviceid)
{
	if(buf == NULL) return 1;

	int i=0;
	int length = (buf[1] & 0x0F) << 8 | ((buf[2] + 3) & 0xff);
	if(length > MINMALLOC) length = MINMALLOC;

	debug(200, "PAT length: %d", length);
	for (i = 8; i < length; i += 4)
	{
		if((((buf[i] & 0xff) << 8) | (buf[i + 1] & 0xff)) > 0)
		{
			if (serviceid == (((buf[i] & 0xff) << 8) | (buf[i + 1] & 0xff)))
			{
				int pmtpid = (((buf[i + 2] & 0x1f) << 8) | (buf[i + 3] & 0xff));
				debug(200, "PMT pid: 0x%X (%d)", pmtpid, pmtpid);
				return pmtpid;
			}
		}
	}
	return 0;
}

char* dvbgethbbtvurl(unsigned char* buf)
{
	//int seclen = 0;
	int pos = 0;
	int pos2 = 0;
	int commondesclen = 0;
	int applooplen = 0;
	int appdesclen = 0;
	int desclen = 0;
	char* url = NULL;

	if(buf == NULL) return NULL;

	//seclen = ((buf[1] & 0x0F) << 8) + buf[2];
	commondesclen = ((buf[8] & 0x0F) << 8) + buf[9];
	pos = 10 + commondesclen;

	applooplen = ((buf[pos] & 0x0F) << 8) + buf[pos + 1];
	pos += 2;

	for(; pos < applooplen; pos += appdesclen + 9)
	{
		//long orgid = (buf[pos] << 24) + (buf[pos + 1] << 16) + (buf[pos + 2] << 8) + buf[pos + 3];
		//int appid = (buf[pos + 4] << 8) + buf[pos + 5];
		int appcode = buf[pos + 6];
		appdesclen = ((buf[pos + 7] & 0x0F) << 8) + buf[pos + 8];

		if(appcode != 1) continue;

		desclen = 0;
		for(pos2 = pos + 9; pos2 < appdesclen + pos + 9; pos2 += desclen + 2)
		{
			int desctag = buf[pos2];
			desclen = buf[pos2 + 1];

			switch(desctag)
			{
				case 0x00: //application desc
					break;
				case 0x01: //application name desc
					break;
				case 0x02: //transport protocol desc
				{
					int protocolid = buf[pos2 + 2] +  buf[pos2 + 3];
					switch(protocolid)
					{
						case 1: //object carousel
							break;
						case 2: //ip
							break;
						case 3: //interaction
						{
							free(url); url = NULL;
							url = malloc(desclen - 4);
							if(url != NULL)
							{
								strncpy(url, (char*)&buf[pos2 + 6], desclen - 5);
								url[desclen - 5]='\0';
							}
							break;
						}
					}
					break;
				}
				case 0x14: //graphics constraints desc
					break;
				case 0x15: //simple application location desc
				{
					if(url != NULL)
					{
						char* tmpurl = NULL;
						tmpurl = malloc(desclen + 1);
						strncpy(tmpurl, (char*)&buf[pos2 + 2], desclen);
						tmpurl[desclen]='\0';
						url = ostrcat(url, tmpurl, 1, 1);
					}
					break;
				}
				case 0x16: //application usage desc
					break;
				case 0x17: //simple application boundary desc
					break;
			}
		}
	}

	return url;
}

unsigned char* dvbget(struct dvbdev* fenode, int pid, int type, int secnr, int timeout)
{
	int length = -1;
	struct dvbdev* dmxnode = NULL;
	unsigned char* buf = NULL;

	buf = malloc(MINMALLOC);
	if(buf == NULL)
	{
		err("no memory");
		return NULL;
	}

	if(fenode == NULL)
	{
		err("no frontend dev");
		free(buf);
		return NULL;
	}

	dmxnode = dmxopen(fenode, 1);
	if(dmxnode == NULL)
	{
		err("open demux dev");
		free(buf);
		return NULL;
	}

	dmxsetsource(dmxnode, fenode->fedmxsource);
	dmxsetfilter(dmxnode, pid, secnr, type);

	length = dvbread(dmxnode, buf, 0, MINMALLOC, timeout);
	if(length < 0)
	{
		dmxclose(dmxnode, -1);
		free(buf);
		err("read");
		return NULL;
	}

	dmxclose(dmxnode, -1);
	return buf;
}

unsigned char* dvbgetait(struct dvbdev* fenode, int pid, int secnr, int timeout)
{
	return dvbget(fenode, pid, 14, secnr, timeout);
}

unsigned char* dvbgetsdt(struct dvbdev* fenode, int secnr, int timeout)
{
	return dvbget(fenode, 0x11, 11, secnr, timeout);
}

unsigned char* dvbgetpat(struct dvbdev* fenode, int timeout)
{
	return dvbget(fenode, 0, 1, 0, timeout);
}

unsigned char* dvbgetnit(struct dvbdev* fenode, int secnr, int timeout)
{
	return dvbget(fenode, 0x10, 4, secnr, timeout);
}

//flag 0: with pat
//flag 1: don't use patbuf
unsigned char* dvbgetpmt(struct dvbdev* fenode, unsigned char* patbuf, int serviceid, int16_t* pmtpid, int* pmtlen, int timeout, int flag)
{
	int length = -1, i;
	struct dvbdev* dmxnode = NULL;
	unsigned char* buf = NULL;

#ifdef SIMULATE
	FILE* fd = NULL;
	
	buf = malloc(MINMALLOC);

	fd = fopen("simulate/pmt.tmp", "rb");
	if(fd == NULL)
	{
		err("open simulate/pmt.tmp");
	}
	else
		fread(buf, sizeof(unsigned char), MINMALLOC, fd);

	if(fd != NULL) fclose(fd);
	if(buf != NULL) return buf;
#endif

	if(flag == 0)
	{
		if(patbuf == NULL) return NULL;
		*pmtpid = dvbgetpmtpid(patbuf, serviceid);
	}

	if(*pmtpid < 1)
	{
		err("pmt pid not found sid=%d, pmtpid=%d", serviceid, *pmtpid);
		return NULL;
	}

	buf = malloc(MINMALLOC);
	if(buf == NULL)
	{
		err("no memory");
		return NULL;
	}

	if(fenode == NULL)
	{
		err("no frontend dev");
		return NULL;
	}

	dmxnode = dmxopen(fenode, 1);
	if(dmxnode == NULL)
	{
		err("open demux dev");
		free(buf);
		return NULL;
	}
	// error
//	if(checkbox("ATEMIO-NEMESIS") != 1)
		dmxsetsource(dmxnode, fenode->fedmxsource);

	dmxsetfilter(dmxnode, *pmtpid, 0, 2);

	for (i = 0; i < 64; i++)
	{
		if((length = dvbread(dmxnode, buf, 0, MINMALLOC, timeout)) < 0)
		{
			err("read pmt pid=%d", *pmtpid);
			break;
		}
		if(serviceid == (((buf[3] & 0xff) << 8) + (buf[4] & 0xff)))
		{
			debug(200, "sid: %d = buf", serviceid);
			dmxclose(dmxnode, -1);
			if(pmtlen != NULL)
			{
				*pmtlen =((buf[1] & 0xf) << 8) + ((buf[2] + 3) & 0xff);
				debug(200, "pmtlen=%d", *pmtlen);
				buf = realloc(buf, *pmtlen);
			}
			return buf;
		}
	}

	err("find pmt");
	dmxclose(dmxnode, -1);
	free(buf);
	if(flag == 0) *pmtpid = 0;
	return NULL;
}

int dvbgetinfo(unsigned char* pmtbuf, struct channel* chnode)
{
	int i, sectionlength, programinfolength, esinfolength, ret = 0;
	unsigned short pos, descriptortag, descriptorlength;
	int isac3 = 0, isdts = 0, isaac = 0, isddp = 0;
	int audiocodec = 0, videocodec = 0;
	int firstaudiopid = -1, firstaudiocodec = -1, audiochange = 1;
	int streamtype, pid, tsid, onid, pcrpid;
	unsigned char *tmpbuf = NULL;
	struct audiotrack* prevaudio = NULL;

	if(pmtbuf == NULL || chnode == NULL)
	{
		err("NULL detect");
		return -1;
	}

	sectionlength = ((pmtbuf[1] & 0x0F) << 8) + (pmtbuf[2] & 0xff);
	if(sectionlength > MINMALLOC) sectionlength = MINMALLOC;
	programinfolength = ((pmtbuf[10] & 0x0F) << 8) | (pmtbuf[11] & 0xff);
	if(programinfolength > MINMALLOC) programinfolength = MINMALLOC;
	
	pcrpid = chnode->pcrpid;
	chnode->pcrpid = ((pmtbuf[8] & 0x1F) << 8) + pmtbuf[9];
	debug(200, "add pcrpid %d", chnode->pcrpid);
	if(chnode->pcrpid != pcrpid) status.writechannel = 1;

	// pmt
	int programnumber = (pmtbuf[3] << 8) + pmtbuf[4];
	int versionnumber = (pmtbuf[5] >> 1) & 0x1F;
	int currentnextindicator = pmtbuf[5] & 0x01;
	addpmt(chnode, programnumber, versionnumber, currentnextindicator);

	if(programinfolength)
	{
		for(i = 12; i < 12 + programinfolength; i += pmtbuf[i + 1] + 2)
		{
			switch (pmtbuf[i])
			{
				case 0x09:
					chnode->crypt = 1;
					addcadesc(chnode, 0, &pmtbuf[i], NULL);
					break;
			}
		}
	}

	for(i = 12 + programinfolength; i < sectionlength - 1; i += esinfolength + 5)
	{
		tmpbuf = pmtbuf + i;
		if(tmpbuf > pmtbuf + MINMALLOC) break;
		streamtype = (tmpbuf[0] & 0xff);
		pid = ((tmpbuf[1] & 0x1F) << 8) | (tmpbuf[2] & 0xff);
		esinfolength = ((tmpbuf[3] & 0x0F) << 8) | (tmpbuf[4] & 0xff);
		isac3 = 0; isdts = 0; isaac = 0; isddp = 0, audiocodec = 0, videocodec = 0;
		char langdesc[4] = "---";
		int y = 0, descriptorcount = 0;
		
		addesinfo(chnode, streamtype, pid, NULL);

		for(pos = 5; pos < esinfolength + 5; pos += descriptorlength + 2)
		{
			descriptorcount++;
			descriptortag = tmpbuf[pos];
			descriptorlength = tmpbuf[pos + 1];
			switch (descriptortag)
			{
        case 0x6f: //get aitpid for hbbtv
					if(streamtype == 0x05)
          {
            chnode->aitpid = pid;
            debug(200, "add aitpid %d", pid);
          }
          break;
				case 0x05:
					if(descriptorlength >= 3)
						if(!ostrncmp((char*)&tmpbuf[pos + 2], "DTS", 3))
							isdts = 1;
					break;
				case 0x81:
					// private descr: dvb namespace
					tsid = tmpbuf[pos + 6] << 8 | tmpbuf[pos + 7];
					onid = (tmpbuf[pos + 8]) << 8 | tmpbuf[pos + 9];
					break;
				case 0x09:
					chnode->crypt = 1;
					addcadesc(chnode, pid, &tmpbuf[pos], NULL);
					break;
				case 0x0A:
					langdesc[0] = toupper(tmpbuf[pos + 2]);
					langdesc[1] = toupper(tmpbuf[pos + 3]);
					langdesc[2] = toupper(tmpbuf[pos + 4]);
					langdesc[3] = '\0';
					break;
				case 0x6A:
					isac3 = 1;
					break;
				case 0x7C:
					isaac = 1;
					break;
				case 0x7B:
					isdts = 1;
					break;
				case 0x7A:  
					isddp = 1;
				break;
				case 0x56: //teletext
					for (y = 0; y < descriptorlength / 5; y++)
					{
						langdesc[0] = toupper(tmpbuf[pos + 5 * y + 2]);
						langdesc[1] = toupper(tmpbuf[pos + 5 * y + 3]);
						langdesc[2] = toupper(tmpbuf[pos + 5 * y + 4]);
						langdesc[3] = '\0';
						unsigned char txttype = tmpbuf[pos + 5 * y + 5] >> 3;
						unsigned short txtmagazinenr = tmpbuf[pos + 5 * y + 5] & 7;
						unsigned short txtpagenr = tmpbuf[pos + 5 * y + 6];
						if(txttype == 0x02 || txttype == 0x05)
							addsubtitle(chnode, 1, langdesc , pid, txttype, txtmagazinenr, txtpagenr, NULL);

					}
					chnode->txtpid = pid;
					debug(200, "add txtpid %d", pid);
					break;
				case 0x59: //subtitle
					if(streamtype == 0x06)
					{
						for (y = 0; y < descriptorlength / 8; y++)
						{
							langdesc[0] = toupper(tmpbuf[pos + 8 * y + 2]);
							langdesc[1] = toupper(tmpbuf[pos + 8 * y + 3]);
							langdesc[2] = toupper(tmpbuf[pos + 8 * y + 4]);
							langdesc[3] = '\0';
							unsigned char subtype = tmpbuf[pos + 8 * y + 5];
							unsigned short subpageid = (tmpbuf[(pos + 8 * y + 6) + 1] << 8) | tmpbuf[pos + 8 * y + 6];
							endianswapshort(&subpageid);
							unsigned short subancillarypageid = (tmpbuf[(pos + 8 * y + 8) + 1] << 8) | tmpbuf[pos + 8 * y + 8];
							endianswapshort(&subancillarypageid);
							addsubtitle(chnode, 2, langdesc , pid, subtype, subpageid, subancillarypageid, NULL);
						}
						debug(200, "add subtile pid %d", pid);
					}
					break;
				//case 0x53: //ca identifier
			}
		}

		switch (streamtype)
		{
			case 0x01: // Mpeg1 Video (6)
			case 0x02: // Mpeg2 Video (0)
			case 0x10: // Mpeg4 Part2 Video (4)
			case 0x1b: // H264 (1)
			case 0x24: //	H265_HEVC
			case 0xea: // vc1 (10)
				if(streamtype == 0x1b)
					videocodec = H264;
				else if(streamtype == 0x24)
					videocodec = H265;
				else if(streamtype == 0x10)
					videocodec = MPEG4V;
				else if(streamtype == 0xea)
					videocodec = VC1;
				else
					videocodec = MPEGV;

				if(chnode->videopid != pid || chnode->videocodec != videocodec)
				{
					chnode->videopid = pid;
					chnode->videocodec = videocodec;
					status.writechannel = 1;
					ret = 1;

					debug(200, "set videopid to %d", pid);
					debug(200, "set videocode to %d (pid=%d)", videocodec, pid);
				}
				break;
			case 0x03: // Mpeg1 Audio (1)
			case 0x04: // Mpeg2 Audio (1)
			case 0x0f: // Mpeg2 AAC (8)
			case 0x11: // Mpeg4 AACHE (9)
			case 0x80: // LPCM (6)
			case 0xA0: // LPCM (6)
			case 0x81: // AC3 (0)
			case 0xA1: // AC3 (0)
			case 0x82: // DTS (2)
			case 0xA2: // DTS (2)
			case 0x85: // DTS-HD HRA
			case 0x86: // DTS-HD MA
			case 0xA6: // DTS-HD
			case 0x06:
				if(descriptorcount == 0 && streamtype == 0x06 && prevaudio != NULL)
				{
					prevaudio->rdspid = pid;
					debug(200, "set rdspid to %d", pid);
				}
				prevaudio = NULL;
				//if(streamtype == 0x81) esInfo->stream_type = 0x6;
				if(streamtype == 0x06 && !isac3 && !isdts && !isaac && !isddp)
					continue;

				if(streamtype == 0x06)
				{
					if(isac3)
						audiocodec = AC3;
					else if(isdts)
						audiocodec = DTS;
					else if(isaac)
						audiocodec = AAC;
					else if(isddp)
						audiocodec = DDP;
				}
				else if(streamtype == 0x0F)
					audiocodec = AAC;
				else if(streamtype == 0x11)
					audiocodec = AACHE;
				else if(streamtype == 0x80 || streamtype == 0xA0)
					audiocodec = LPCM;
				else if(streamtype == 0x81 || streamtype == 0xA1)
					audiocodec = AC3;
				else if(streamtype == 0x82 || streamtype == 0xA2)
					audiocodec = DTS;
				else if(streamtype == 0x85 || streamtype == 0x86 || streamtype == 0xA6)
					audiocodec = DTSHD;
				else
					audiocodec = MPEGA;
					
				if(audiocodec == DTSHD && getconfigint("av_can_dtshdmode", NULL) == NO)
				{
					audiocodec = 0;
					continue;
				}

				if(firstaudiopid == -1 || firstaudiocodec == -1)
				{
					firstaudiopid = pid;
					firstaudiocodec = audiocodec;
				}

				if(getconfigint("av_ac3default", NULL) == YES && audiocodec == AC3 && chnode->audiocodec != AC3)
				{
					chnode->audiopid = pid;
					chnode->audiocodec = audiocodec;
					status.writechannel = 1;
					debug(200, "ac3default -> set audiopid to %d", pid);
					debug(200, "ac3default -> set audiocode to %d (pid=%d)", chnode->audiocodec, pid);
				}
				
				if(chnode->audiopid == pid && chnode->audiocodec == audiocodec)
					audiochange = 0;

				prevaudio = addaudiotrack(chnode, langdesc, pid, audiocodec, NULL);
				break;
		}
	}
	
	if(audiochange == 1 && firstaudiopid != -1 && firstaudiocodec != -1)
	{
		chnode->audiopid = firstaudiopid;
		chnode->audiocodec = firstaudiocodec;
		status.writechannel = 1;
		ret = 1;

		debug(200, "set audiopid to %d", firstaudiopid);
		debug(200, "set audiocode to %d (pid=%d)", firstaudiocodec, firstaudiopid);
	}
	
	if(chnode->crypt != 0) debug(200, "stream is crypted");
	return ret;
}

//check dynamic change of pmt
void dvbgetpmtthread()
{
	int len = 0, change = 0;
	unsigned char* pmtbuf = NULL;

	//play activ?
	if(status.play != 0 || status.playspeed != 0) return;
	
	if(status.aktservice->type != CHANNEL || status.aktservice->channel == NULL) return;
	
	debug(200, "dvbgetpmtthread started");
	
	int serviceid = status.aktservice->channel->serviceid;
	uint64_t transponderid = status.aktservice->channel->transponderid;
	int16_t pmtpid = status.aktservice->channel->pmtpid;
	int16_t audiopid = status.aktservice->channel->audiopid;
	int16_t videopid = status.aktservice->channel->videopid;
	uint8_t crypt = status.aktservice->channel->crypt;
	int oldpmtlen = status.aktservice->pmtlen;
	
	if(pmtpid < 1) return;

	//outside of lock, can block a little
	pmtbuf = dvbgetpmt(status.aktservice->fedev, NULL, serviceid, &pmtpid, &len, -1, 1);
	if(pmtbuf == NULL) return;

	m_lock(&status.servicemutex, 2);

	//check if threre was a change from start funktion to lock
	if(status.aktservice->type != CHANNEL || status.aktservice->channel == NULL || status.aktservice->channel->serviceid != serviceid || status.aktservice->channel->transponderid != transponderid)
	{
		debug(200, "change from start");
		m_unlock(&status.servicemutex, 2);
		free(pmtbuf);
		return;
	}

	if(len != status.aktservice->pmtlen || memcmp(status.aktservice->pmtbuf, pmtbuf, len) != 0)
	{
		serviceresetchannelinfo(status.aktservice->channel);
		status.aktservice->channel->audiopid = -1;
		status.aktservice->channel->audiocodec = -1;
		status.aktservice->channel->videopid = -1;
		status.aktservice->channel->videocodec = -1;
		status.aktservice->channel->pmtpid = pmtpid;
		status.aktservice->channel->crypt = 0;
		dvbgetinfo(pmtbuf, status.aktservice->channel);

		debug(200, "pmt dynamic change oldapid=%d apid=%d oldvpid=%d vpid=%d oldcrypt=%d crypt=%d oldpmtlen=%d\n", audiopid, status.aktservice->channel->audiopid, videopid, status.aktservice->channel->videopid, status.aktservice->channel->crypt, crypt, oldpmtlen);

		if(status.aktservice->channel->audiopid != audiopid || status.aktservice->channel->videopid != videopid || oldpmtlen == -1 || status.aktservice->channel->crypt != crypt)
			change = 1;
	}

	m_unlock(&status.servicemutex, 2);
	free(pmtbuf);

	if(change == 1)
		servicestart(status.aktservice->channel, NULL, NULL, 3);
		
	debug(200, "dvbgetpmtthread ended... change=%i", change);
}

time_t dvbconvertdate(unsigned char *buf, int flag)
{
	int i = 0;
	long mjd;
	struct tm *time = NULL;
	time_t gmttime = 0;

	time = (struct tm*)malloc(sizeof(struct tm));
	if(time == NULL)
	{
		err("no mem");
		return 0;
	}

	memset(time, 0, sizeof(struct tm));

	mjd = ((buf[0] & 0xff) << 8) | (buf[1] & 0xff);
	time->tm_hour = bcdtoint(buf[2] & 0xff);
	time->tm_min = bcdtoint(buf[3] & 0xff);
	time->tm_sec = bcdtoint(buf[4] & 0xff);

	time->tm_year = (int) ((mjd - 15078.2) / 365.25);
	time->tm_mon = (int) ((mjd - 14956.1 - (int) (time->tm_year * 365.25)) / 30.6001);
	time->tm_mday = (int) mjd - 14956 - (int) (time->tm_year * 365.25) - (int) (time->tm_mon * 30.6001);
	if(time->tm_mon == 14 || time->tm_mon == 15) i = 1;
	time->tm_year += i;
	time->tm_mon = time->tm_mon - 2 - i * 12;

	time->tm_isdst = 0;
	time->tm_gmtoff = 0;

	if(flag == 0)
		gmttime = timegm(time);
	else
		gmttime = mktime(time);

	free(time);
	return gmttime;
}

int dvbgetdate(time_t* time, int timeout)
{
	int length = -1, sectionlength;
	struct dvbdev* dmxnode = NULL;
	unsigned char *buf = NULL;

	buf = malloc(MINMALLOC);
	if(buf == NULL)
	{
		err("no memory");
		return 1;
	}

	if(status.aktservice->fedev == NULL)
	{
		err("no frontend dev in aktservice");
		free(buf);
		return 1;
	}

	dmxnode = dmxopen(status.aktservice->fedev, 1);
	if(dmxnode == NULL)
	{
		err("open demux dev");
		free(buf);
		return 1;
	}

	dmxsetsource(dmxnode, status.aktservice->fedev->fedmxsource);
	dmxsetfilter(dmxnode, 0x14, 0, 3);

	if((length = dvbread(dmxnode, buf, 0, MINMALLOC, timeout)) < 0)
	if(length < 3)
	{
		dmxclose(dmxnode, -1);
		free(buf);
		err("read dvb date");
		return 1;
	}

	sectionlength = ((buf[1] & 0x0f) << 8) | (buf[2] & 0xff);
	if(length == sectionlength + 3)
		*time = dvbconvertdate(&(buf[3]), 0);
	else
	{
		dmxclose(dmxnode, -1);
		free(buf);
		err("read dvb date");
		return 1;
	}

	setrtctime(*time);

	dmxclose(dmxnode, -1);
	free(buf);
	return 0;
}

// getPTS extracts a pts value from any PID at a given offset.
int getpts(int fd, off64_t offset, int spid, int left, unsigned long long *pts, off64_t *findpos, int dir, int tssize)
{
	int first = 1;
	
	offset -= offset % tssize;
	if(dir < 0 && offset < tssize) offset = tssize * 2;
	if(dir < 0) offset *= -1;
	while(left >= tssize)
	{
		unsigned char packet[tssize];

		if(dir > -1)
			*findpos = lseek64(fd, offset, SEEK_SET);
		else
		{
			if(dir == -2)
			{
				if(first == 1)
				{
					first = 0;
					offset *= -1;
					*findpos = lseek64(fd, offset, SEEK_SET);
					offset = -tssize;
				}
				else 
				{
					if(offset % tssize != 0)
					{
						*findpos = lseek64(fd, (offset % tssize) - tssize, SEEK_CUR);
						offset = -tssize;
					}
					else
						*findpos = lseek64(fd, -tssize * 2, SEEK_CUR);
				}
			}
			else
				*findpos = lseek64(fd, offset, SEEK_END);
		}

		int ret = dvbreadfd(fd, packet, 0, tssize, -1, 0);
		if(ret != tssize && ret != 188)
		{
			err("read error");
			break;
		}

		if(packet[0] != 0x47)
		{
			debug(200, "resync");
			int i = 0;
			while(i < tssize)
			{
				if(packet[i] == 0x47) break;
				i++;
				offset++;
			}
			continue;
		}

		left -= tssize;
		if(dir > -1)
			offset += tssize;
		else
			offset -= tssize;
		
		int pid = ((packet[1] << 8) | packet[2]) & 0x1FFF;
		int pusi = !!(packet[1] & 0x40);
		
//		printf("PID %04x, PUSI %d\n", pid, pusi);

		unsigned char *payload;
		
		//check for adaption field
		if(packet[3] & 0x20)
		{
			if(packet[4] >= 183) continue;
			if(packet[4])
			{
				if(packet[5] & 0x10) //PCR present
				{
					*pts = ((unsigned long long)(packet[6] & 0xFF)) << 25;
					*pts |= ((unsigned long long)(packet[7] & 0xFF)) << 17;
					*pts |= ((unsigned long long)(packet[8] & 0xFE)) << 9;
					*pts |= ((unsigned long long)(packet[9] & 0xFF)) << 1;
					*pts |= ((unsigned long long)(packet[10] & 0x80)) >> 7;
					if(dir > -1)
					{
						offset -= tssize;
						*findpos -= tssize;
					}
					debug(200, "PCR %16llx found at %lld pid %02x (%02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x)", *pts, offset, pid, packet[0], packet[1], packet[2], packet[3], packet[4], packet[5], packet[10], packet[9], packet[8], packet[7], packet[6]);
					return 0;
				}
			}
			payload = packet + packet[4] + 4 + 1;
		} else
			payload = packet + 4;

		//if(spid >= 0 && pid != spid) continue;
		if(!pusi) continue;

		//somehow not a startcode. (this is invalid, since pusi was set.) ignore it.
		if (payload[0] || payload[1] || (payload[2] != 1))
			continue;

		//stream use extension mechanism def in ISO 13818-1 Amendment 2
		if(payload[3] == 0xFD)
		{
			if(payload[7] & 1) //PES extension flag
			{
				int offs = 0;
				if(payload[7] & 0x80) offs += 5; //pts avail
				if(payload[7] & 0x40) offs += 5; //dts avail
				if(payload[7] & 0x20) offs += 6; //escr avail
				if(payload[7] & 0x10) offs += 3; //es rate
				if(payload[7] & 0x8) offs += 1; //dsm trickmode
				if(payload[7] & 0x4) offs += 1; //additional copy info
				if(payload[7] & 0x2) offs += 2; //crc
				if(payload[8] < offs) continue;

				uint8_t pef = payload[9 + offs++]; //pes extension field
				if(pef & 1) //pes extension flag 2
				{
					if(pef & 0x80) offs += 16; //private data flag
					if(pef & 0x40) offs += 1; //pack header field flag
					if(pef & 0x20) offs += 2; //program packet sequence counter flag
					if(pef & 0x10) offs += 2; //P-STD buffer flag
					if(payload[8] < offs) continue;

					uint8_t stream_id_extension_len = payload[9 + offs++] & 0x7F;
					if(stream_id_extension_len >= 1)
					{
						if(payload[8] < (offs + stream_id_extension_len)) continue;
						//stream_id_extension_bit (should not set)
						if(payload[9 + offs] & 0x80) continue;
						switch(payload[9 + offs])
						{
							case 0x55 ... 0x5f: break; //VC-1
							case 0x71: break; //AC3 / DTS
							case 0x72: break; //DTS - HD
							default:
								debug(200, "skip unknwn stream_id_extension %02x\n", payload[9 + offs]);
								continue;
						}
					}
					else
						continue;
				}
				else
					continue;
			}
			else
				continue;
		}
		//drop non-audio, non-video packets because other streams
		//can be non-compliant.
		//0xC0 = audio, 0xE0 = video
		else if(((payload[3] & 0xE0) != 0xC0) && ((payload[3] & 0xF0) != 0xE0))
			continue;

		if(payload[7] & 0x80) //PTS
		{
			*pts = ((unsigned long long)(payload[9] & 0xE)) << 29;
			*pts |= ((unsigned long long)(payload[10] & 0xFF)) << 22;
			*pts |= ((unsigned long long)(payload[11] & 0xFE)) << 14;
			*pts |= ((unsigned long long)(payload[12] & 0xFF)) << 7;
			*pts |= ((unsigned long long)(payload[13] & 0xFE)) >> 1;

			if(dir > -1)
			{
				offset -= tssize;
				*findpos -= tssize;
			}
			debug(200, "PTS %16llx found at %lld pid %02x stream: %02x", *pts, offset, pid, payload[3]);

			return 0;
		}
	}
	
	return 1;
}

unsigned long long fixuppts(unsigned long long start, unsigned long long end)
{
		//pts can wrap to 0 in the middle
		if(end < start)
			end = end + 0x200000000LL;
		return end;
}

unsigned long long gettsbitrate(unsigned long long start, unsigned long long end, off64_t startfindpos, off64_t endfindpos)
{
	unsigned long long bitrate = 0;
	unsigned long long diff = 0;

	diff = end - start;
	if(diff <= 0) return 0;

	bitrate = (endfindpos - startfindpos) * 90000 * 8 / diff;
	if(bitrate < 10000 || bitrate > 100000000)
                return 0;
	else
		return bitrate;
}

int gettsinfo(int fd, unsigned long long* lenpts, unsigned long long* startpts, unsigned long long* endpts, unsigned long long* bitrate, int tssize)
{
	int ret = 0;
	unsigned long long start = 0, end = 0;
	off64_t startfindpos = 0, endfindpos = 0;

	if(startpts != NULL && *startpts > 0) start = *startpts;
	if(endpts != NULL && *endpts > 0) end = *endpts;

	if(start == 0 || (bitrate != NULL && bitrate == 0))
	{
		ret = getpts(fd, 0, 0, 256 * 1024, &start, &startfindpos, 1, tssize);
		//workaround fuer Aufnahmen mit fehlerhaften Anfang (springe 30 sekunden)
		if(ret!=0)
			ret = getpts(fd, 37000000, 0, 256 * 1024, &start, &startfindpos, 1, tssize);	 
	}

	if((ret == 0 && end == 0) || (bitrate != NULL && bitrate == 0))
		ret = getpts(fd, 0, 0, 256 * 1024, &end, &endfindpos, -1, tssize);

	if(ret == 0)
	{
		end = fixuppts(start, end);
		if(lenpts != NULL) *lenpts = end - start;
		if(startpts != NULL) *startpts = start;
		if(endpts != NULL) *endpts = end;
		if(bitrate != NULL && *bitrate == 0) *bitrate = gettsbitrate(start, end, startfindpos, endfindpos);
	}

	if(lenpts != NULL && *lenpts < 0) *lenpts = 0;
	if(startpts != NULL && *startpts < 0) *startpts = 0;
	if(endpts != NULL && *endpts < 0) *endpts = 0;
	return ret;
}

int getptspos(int fd, off64_t startfind, unsigned long long* pts, off64_t* findpos, int dir, int tssize)
{
	int ret = 0;
	unsigned long long pts1 = 0;
	off64_t findpos1 = 0;
	
	ret = getpts(fd, startfind, 0, 256 * 1024, &pts1, &findpos1, dir, tssize);
	if(pts != NULL) *pts = pts1;
	if(findpos != NULL) *findpos = findpos1;
	
	return ret;		
}


//rc = 1 --> mpeg
//rc = 2 --> h.264
int findcodec(unsigned char* buf, int len, int tssize)
{
	int i,i1;
	int rc = 2;
	
	for(i = 0; i < len-tssize; i = i + tssize)
	{
		if((buf[i+1]&0x40)>>6 == 0x01)
		{
			for(i1 = i+4; i1 < i + tssize-4; i1 = i1 + 1)
			{
				if (buf[i1] == 0x00 && buf[i1+1] == 0x00 && buf[i1+2] == 0x01 && buf[i1+3] == 0xB3)
				{
					return 1;
				}
			}
		}
	}
	return rc;
} 

//flag=0 -> future use
//flag=1 -> I-Slice
//flag=2 -> ende Frame
int findframeHD(unsigned char* buf, int len, int start, int tssize, int flag)
{
	int position = -1;
	int i = 0;
	int i1 = 0;

	len = start + len;
	
	while(1) 
	{
		for(i = start; i < len-3; i = i + 1)
		{
			//Picture header ?
			if (buf[i] == 0x00 && buf[i+1] == 0x00 && buf[i+2] == 0x01)
			{
				//SPS (Sequence Parameter Set)?
				if(buf[i+3] == 0x67 && flag == 1)
				{
					//PPS (Picture Parameter Set) ?
					for(i1 = i + 4; i1 < (i % tssize) + i; i1 = i1 + 1) 
					{
						if (buf[i1] == 0x00 && buf[i1+1] == 0x00 && buf[i1+2] == 0x01 && buf[i1+3] == 0x68)
						{
							position = i1;
							break;
						}
					}
					if(position != -1)
					{
						for(i1 = position + 4; i1 < (position % tssize) + position; i1 = i1 + 1)
						{
							if (buf[i1] == 0x00 && buf[i1+1] == 0x00 && buf[i1+2] == 0x01)
							{
								if(buf[i1+3] == 0x01 || buf[i1+3] == 0x21 || buf[i1+3] == 0x41 || buf[i1+3] == 0x61)
								{
									i = i1+4;
									position = -1;
									break;
								}
							}
						}
					}
					if(position != -1)
						break;					
				}
				//I-P-Slice oder I-P-B-Slice oder oder .....?
				else if((buf[i+3] == 0x01 || buf[i+3] == 0x21 || buf[i+3] == 0x41 || buf[i+3] == 0x61) && flag == 2 )
				{
				  position = i;
				 	break;
				}
   		}
   	}
   	return position;
	}
}


//flag=0 -> Sequence header
//flag=1 -> I-Fame
//flag=2 -> ende Frame
int findframe(unsigned char* buf, int len, int start, int tssize, int flag)
{	
	//#define	SEQ_START_CODE          0x000001b3
	//#define GOP_START_CODE          0x000001b8
	//#define PICTURE_START_CODE      0x00000100
	//#define SLICE_START_CODE        0x00000101
	//#define PACK_START_CODE         0x000001ba
	//#define VIDEO_ID                0x000001e0
	//#define AUDIO_ID                0x000001c0

	
	int position = -1;
	int i = 0;

	len = start + len;

	while(1) {

		for(i = start; i < len-3; i = i + 1)
		{

			//Picture header ?
			if (buf[i] == 0x00 && buf[i+1] == 0x00 && buf[i+2] == 0x01)
			{
	
				//Sequence header
				if(buf[i+3] == 0xB3 && flag == 0)
				{
					position = i;
					break;
				}	

				//Picture-Frame ?
				else if(buf[i+3] == 0x00)
				{

					//I-Frame ?
					if((buf[i+5] & 0x38)>>3 == 0x01 && flag == 1)
					{
					  position = i;
						break;
					}

					//other Frame ?
					else if((buf[i+5] & 0x38)>>3 != 0x00 && flag == 2) 
					{
					  position = i;
					 	break;
					}	
				}
   		}
   	}
   	return position;
	}
}

int findandposrew(int fd, int tssize, int skip)
{
	int i;
	int ret = 0;
	int framelen = -1;
	off64_t endframe = -1;
	off64_t startframe = -1;
	int readret = 0;
	int codec = 0;
	int buflen = tssize * 15000;
	unsigned char* buf = malloc(buflen);
	off64_t currentpos;
	off64_t skippos;
	
	//ret = videostop(status.aktservice->videodev,0);
	//ret = videoclearbuffer(status.aktservice->videodev);
	//ret = audioclearbuffer(status.aktservice->audiodev);
	//ret = audiosetavsync(status.aktservice->audiodev, 0);
	//ret = audiosetmute(status.aktservice->audiodev, 1);
	
	currentpos = lseek64(fd, 0, SEEK_CUR);
	skippos = currentpos;
	
	while(1) 
	{ 
		skippos = skippos - buflen;
		if(skippos < 0)
		{
			currentpos = lseek64(fd, currentpos, SEEK_SET);
			free(buf);
			return -1;
		}
		skippos = lseek64(fd, skippos, SEEK_SET);
		readret = dvbreadfd(fd, buf, 0, buflen, 5000000, 1);
		if(readret > 0)
		{
			codec = findcodec(buf, buflen, tssize);
			for(i = readret - tssize; i >= 0; i = i - tssize)
			{
				if(codec == 1)
					ret = findframe(buf, tssize, i, tssize, 0);
				else
					ret = findframeHD(buf, tssize, i, tssize, 1);
				if(ret > -1)
				{
					if(endframe == -1)
					{
						if(skip != 0)
							skip = skip - 1;
						else
							endframe = skippos + i;
					}
					else
					{
						startframe = skippos + i;
						framelen = endframe - startframe;
						currentpos = lseek64(fd, startframe, SEEK_SET);
						//readret = dvbreadfd(servicenode->recsrcfd, buf, 0, buflen, 5000000, 1);
						free(buf);
						return framelen;
					}
				}
			}
		}
		else
		{
			currentpos = lseek64(fd, currentpos, SEEK_SET);
			free(buf);
			return -1;
		}
	}
}

#endif
