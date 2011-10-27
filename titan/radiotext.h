#ifndef RADIOTEXT_H
#define RADIOTEXT_H

#define SWAP(x) ((x << 8)|(x >> 8))
#define LO(x) (x & 0xFF)

int rtmsgptr = 0, rtbsflag = 0, rtqdarpos = 0, rttptr = 0, rtqdarmvishow = 0, rttype;
unsigned char rtmessage[66], rtlastmessage[66], rtdatamessage[256], rtpbuf[5], rtleninfo, rttextlen, rttextlen2, rtstate = '\0';
unsigned char rtpitem[64][64], rtplusosd[64]; //rtp
unsigned char rtqdar[60 * 1024]; //60 kB for holding Rass qdar archive
unsigned short rtcrc16, rtcrc;
long rtpart, rtparts, rtpartcnt;
unsigned char rtrasspicturemask[5];  // 40 bits... (10 * 4 pictures)

static int rtbitrate[3][3][16] = {
	{
		{-1,8000,16000,24000,32000,40000,48000,56000,64000,80000,96000,112000,128000,144000,160000,0}, //MPEG-2, L3
		{-1,8000,16000,24000,32000,40000,48000,56000,64000,80000,96000,112000,128000,144000,160000,0}, //MPEG-2, L2
		{-1,32000,48000,56000,64000,80000,96000,112000,128000,144000,160000,176000,192000,224000,256000,0} //MPEG-2, L1
	},
	{
		{-1,32000,40000,48000,56000,64000,80000,96000,112000,128000,160000,192000,224000,256000,320000,0}, //MPEG-1, L3
		{-1,32000,48000,56000,64000,80000,96000,112000,128000,160000,192000,224000,256000,320000,384000,0}, //MPEG-1, L2
		{-1,32000,64000,96000,128000,160000,192000,224000,256000,288000,320000,352000,384000,416000,448000,0} //MPEG-1, L1
	},
	{
		{-1,6000,8000,10000,12000,16000,20000,24000,28000,320000,40000,48000,56000,64000,80000,0}, //MPEG-2.5, L3??
		{-1,6000,8000,10000,12000,16000,20000,24000,28000,320000,40000,48000,56000,64000,80000,0}, //MPEG-2.5, L2
		{-1,8000,12000,16000,20000,24000,32000,40000,48000,560000,64000,80000,96000,112000,128000,0} //MPEG-2.5, L1
	}
};

static int rtfrequency[3][4] = {
	{22050,24000,16000,0}, //MPEG2 - 22.05, 24, 16khz
	{44100,48000,32000,0}, //MPEG1 - 44.1, 48, 32khz
	{11025,12000,8000,0} //MPEG2.5 - 11.025, 12, 8khz
};

/*
DEFINE_REF(eDVBRdsDecoder);

eDVBRdsDecoder::eDVBRdsDecoder(iDVBDemux *demux, int type)
	:rtmsgptr(0), rtbsflag(0), rtqdarpos(0), rttptr(0), rtqdarmvishow(0), rtstate(0)
	,rttype(type), m_pid(-1), m_abortTimer(eTimer::create(eApp))
{
	setStreamID(0xC0, 0xC0);

	memset(rtrasspicturemask, 0, sizeof(rtrasspicturemask));
	memset(rtpitem, 0, sizeof(rtpitem));

	if (demux->createPESReader(eApp, m_pes_reader))
		eDebug("failed to create PES reader!");
	else if (type == 0)
		m_pes_reader->connectRead(slot(*this, &eDVBRdsDecoder::processData), m_read_connection);
	else
		m_pes_reader->connectRead(slot(*this, &eDVBRdsDecoder::gotAncillaryData), m_read_connection);
	CONNECT(m_abortTimer->timeout, eDVBRdsDecoder::abortNonAvail);
}
*/

char* rtgetrass(int page, int subpage)
{
	char *fname = NULL;
	int val = 0;

	fname = malloc(50);
	if(fname == NULL)
	{
		err("no mem");
		return NULL;
	}
	
	switch(subpage)
	{
		case 0:
			val = page * 1000;
			break;
		case 1:
			val = page * 1100;
			break;
		case 2:
			val = page * 1110;
			break;
		case 3:
			val = page * 1111;
			break;
	}

	sprintf(fname, "/tmp/Rass%04d.mvi", val);
	return fname;
}

void rtdelrass()
{
	int page = 0;

	// delete cached rass slides
	for(page=0; page < 10; ++page)
	{
		unsigned char mask = rtrasspicturemask[(page * 4) / 8];
		if(page % 2) mask >>= 4;
		int subpage = 0;

		while(mask)
		{
			if(mask & 1)
			{
				char* filename = rtgetrass(page, subpage);
				if(filename != NULL && strlen(filename) > 0) unlink(filename);
				free(filename); filename = NULL;
			}
			mask >>= 1;
			++subpage;
		}
	}
	unlink("/tmp/RassLast.mvi");
}

unsigned short rt_crc_ccitt_byte(unsigned short crc, unsigned char c)
{
	rtcrc = SWAP(rtcrc) ^ c;
	rtcrc = rtcrc ^ (LO(rtcrc) >> 4);
	rtcrc = rtcrc ^ (SWAP(LO(rtcrc)) << 4) ^ (LO(rtcrc) << 5);
	return rtcrc;
}

/*
void eDVBRdsDecoder::connectEvent(const Slot1<void, int> &slot, ePtr<eConnection> &connection)
{
	connection = new eConnection(this, m_event.connect(slot));
}
*/

void rtaddpicturemask(int id)
{
	int page = id / 1000;
	int tmp = page > 0 ? id / page : id;
	int subpage = 0;

	while(tmp > 1000)
	{
		++subpage;
		tmp -= 1000;
		tmp *= 10;
	}

	int index = (page * 4 + subpage) / 8;
	int val = (page % 2) ? 16 * (1 << subpage) : (1 << subpage);
	if(rtrasspicturemask[index] & val) //already have this picture
		return;
	rtrasspicturemask[index] |= val;
	// m_event(RassInteractivePicMaskChanged);
}

void rtdelpicturemask(int id)
{
	int page = id / 1000;
	int tmp = page > 0 ? id / page : id;
	int subpage = 0;

	while(tmp > 1000)
	{
		++subpage;
		tmp -= 1000;
		tmp *= 10;
	}

	int index = (page * 4) / 8;
	int val = (page % 2) ? 16 * (1 << subpage) : (1 << subpage);
	if(rtrasspicturemask[index] & val) // have this picture
	{
		rtrasspicturemask[index] &= ~val;
		// m_event(RassInteractivePicMaskChanged);
	}
}

void rtprocessqdar(unsigned char *buf)
{
	if(buf[0] == 0x40 && buf[1] == 0xDA)
	{
		unsigned int item, cnt, ctrl, item_type;
		unsigned long item_length, id, item_no, ptr, tmp;
		unsigned short crc_qdar, crc_read;
		char fname[50];

		ptr=4; cnt=0;
		item = buf[2] << 8; //Number of Items
		item |= buf[3];
		
		while(cnt++ < item) //read in items
		{
			id = buf[ptr++] << 8; //QDarID
			id |= buf[ptr++];
			
			item_no = buf[ptr++] << 8; //Item Number
			item_no |= buf[ptr++];
			
			ctrl = buf[ptr++]; //controlbyte
			item_type = buf[ptr++]; //item type
			
			item_length = buf[ptr++] << 24; // Item length
			item_length |= buf[ptr++] << 16;
			item_length |= buf[ptr++] << 8;
			item_length |= buf[ptr++];
			
			ptr = ptr + 4; //rfu Bytes ... not used
			tmp = ptr; //calc crc
			crc_qdar = 0xFFFF;

			while(tmp < ptr + item_length)
				crc_qdar = rt_crc_ccitt_byte(crc_qdar, buf[tmp++]);
		
			crc_read = buf[ptr + item_length] << 8;
			crc_read |= buf[ptr + item_length + 1];
			//debug(270, "RDS/Rass CRC read: %04X calculated: %04X", crc_read, crc_qdar ^ 0xFFFF);

			if(crc_read == (crc_qdar ^ 0xFFFF)) //process item
			{
				switch(item_type)
				{
					case 0x01: //Stillframe
						if (ctrl & 0x01) //display slide
						{
							sprintf(fname,"/tmp/RassLast.mvi");
							FILE *fh = fopen(fname, "wb");
							fwrite(buf + ptr, 1, item_length - 2, fh);
							fclose(fh);
							// m_event(RecvRassSlidePic);
							rtqdarmvishow = 1;
						}
						if (ctrl & 0x02) //save slide for interactive mode
						{
							if(id == 0 || id >= 1000)
							{
								sprintf(fname, "/tmp/Rass%04d.mvi", (int)id);
								FILE *fh = fopen(fname, "wb");
								fwrite(buf + ptr, 1, item_length - 2, fh);
								fclose(fh);
								rtaddpicturemask(id);
							}
							else
								debug(270, "ignore recv interactive picture id %lu", id);
						}
						if(ctrl & 0x04) // display slide if nothing had been displayed yet
						{
							if(rtqdarmvishow != 1)
							{
								sprintf(fname, "/tmp/RassLast.mvi");
								FILE *fh = fopen(fname, "wb");
								fwrite(buf + ptr, 1, item_length - 2, fh);
								fclose(fh);
								// m_event(RecvRassSlidePic);
								rtqdarmvishow = 1;
							}
						}
						if(ctrl & 0x08) //delete slide
						{
							debug(270, "delete slide id %lu, item_no %lu", id, item_no);
							if(id == 0 || id >= 1000)
							{
								debug(270, "delete %lu", id);
								rtdelpicturemask(id);
								sprintf(fname, "/tmp/Rass%04d.mvi", (int)id); //was item_no ? ! ?
								unlink(fname);
							}
							else
								debug(270, "ignore del interactive picture id %lu", id);
						}
						break;
					default: //nothing more yet defined
						break;
				}
			} 
			else
			{
				debug(270, "RDS/Rass CRC error, skip Rass-Qdar-Item");
			}
			
			ptr =+ item_length;
		}
	}
	else
	{
		debug(270, "RDS/Rass No Rass-QDAR archive (%02X %02X) so skipping !", buf[0], buf[1]);
	}
}

void rtgotancillarydata(const unsigned char *buf, int len)
{
	if(len <= 0) return;

	int pos = rttype ? 0 : len-1;
	while(len)
	{
		unsigned char c = buf[pos];

		pos += rttype ? 1 : -1;
		--len;

		if(rtbsflag == 1) // byte stuffing
		{
			rtbsflag = 2;
			switch(c)
			{
				case 0x00: c=0xFD; break;
				case 0x01: c=0xFE; break;
				case 0x02: c=0xFF; break;
			}
		}

		if(c == 0xFD && rtbsflag == 0) 
			rtbsflag = 1;
		else
			rtbsflag = 0;
					
		if(rtbsflag == 0) 
		{
			if(rtstate == 1)
				rtcrc = 0xFFFF;
			if((rtstate >= 1 && rtstate < 11) || (rtstate >=26 && rtstate < 36))
				rtcrc = rt_crc_ccitt_byte(rtcrc, c);

			switch(rtstate)
			{
				case 0:
					if(c == 0xFE) rtstate = 1; //startkennung
					break;
				case 1: // 10bit Site Address + 6bit Encoder Address
				case 2:
				case 3: // Sequence Counter
					++rtstate;
					break;
				case 4:
					rtleninfo = c;
					++rtstate;
					break;
				case 5:
					switch(c)
					{
						case 0x0A: // Radiotext
							++rtstate;
							break;
						case 0x46: // Radiotext Plus tags
							rtstate = 38;
							break;
						case 0xDA: // Rass
							rtstate = 26;
							break;
						default: // reset to state 0
							rtstate = 0;
					}
					break;

					// process Radiotext
				case 6: // Data Set Number ... ignore
				case 7: // Program Service Number ... ignore
					++rtstate;
					break;
				case 8: // Message Element Length
					rttextlen = c;
					if(!rttextlen || rttextlen > 65 || rttextlen > rtleninfo - 4)
						rtstate = 0;
					else
					{
						++rtstate;
						rttextlen -= 2;
						rtmsgptr = 0;
					}
					break;
				case 9: // Radio Text Status bit:
					// 0   = AB-flagcontrol
					// 1-4 = Transmission-Number
					// 5-6 = Buffer-Config
					++rtstate; // ignore ...
					break;
				case 10:
					// TODO build a complete radiotext charcode to UTF8 conversion table for all character > 0x80
					switch (c)
					{
						case 0 ... 0x7f: break;
						case 0x8d: c = 'ß'; break;
						case 0x91: c = 'ä'; break;
						case 0xd1: c = 'Ä'; break;
						case 0x97: c = 'ö'; break;
						case 0xd7: c = 'Ö'; break;
						case 0x99: c = 'ü'; break;
						case 0xd9: c = 'Ü'; break;
						default: c = ' '; break;  // convert all unknown to space
					}
					rtmessage[rtmsgptr++] = c;
					if(rttextlen)
						--rttextlen;
					else
						++rtstate;
					break;
				case 11:
					rtcrc16 = c << 8;
					++rtstate;
					break;
				case 12:
					rtcrc16 |= c;
					rtmessage[rtmsgptr--] = 0;
					while(rtmessage[rtmsgptr] == ' ' && rtmsgptr > 0)
						rtmessage[rtmsgptr--] = 0;
					if(rtcrc16 == (rtcrc ^ 0xFFFF))
					{
						debug(270, "radiotext: (%s)", rtmessage);
						// m_event(RadioTextChanged);
						memcpy(rtlastmessage, rtmessage, 66);
					}
					else
						err("invalid radiotext crc (%s)", rtmessage);
					rtstate = 0;
					break;

				// process Rass
				case 26: //MEL
					rttextlen = c;
					rttextlen2 = c;
					++rtstate;
					rttextlen -= 9;
					rttextlen2 -= 9;
					rttptr = 0;
					break;
				case 27: // SID not used atm
					++rtstate;
					break;
				case 28: // SID not used atm
					++rtstate;
					break;
				case 29: // PNR packet number
					rtpart = c << 16;
					++rtstate;
					break;
				case 30: // PNR packet number
					rtpart |= c << 8;
					++rtstate;
					break;
				case 31: // PNR packet number
					rtpart |= c;
					++rtstate;
					break;
				case 32: // NOP number of packets
					rtparts = c << 16;
					++rtstate;
					break;
				case 33: // NOP number of packets
					rtparts |= c << 8;
					++rtstate;
					break;
				case 34: // NOP number of packets
					rtparts |= c;
					++rtstate;
					break;
				case 35:
					rtdatamessage[rttptr++] = c;
					if(rttextlen) 
						--rttextlen;
					else
						++rtstate;
					break;
				case 36:
					rtcrc16 = c << 8;
					++rtstate;
					break;
				case 37:
					rtcrc16 |= c;
					//debug(270, "RDS/Rass CRC read: %04X CRC calculated: %04X", rtcrc16 , rtcrc ^ 0xFFFF);
					rtstate = 0;
					if(rtcrc16 == (rtcrc ^ 0xFFFF)) 
					{
						if(rtpartcnt == -1) 
							rtpartcnt = 1;
						if(rtpartcnt == rtpart)
						{
							memcpy(rtqdar + rtqdarpos, rtdatamessage, rttextlen2 + 1);
							rtqdarpos = rtqdarpos + rttextlen2 + 1;
							if(rtpartcnt == rtparts)
							{
								rtprocessqdar(rtqdar); // decode qdar archive
								rtqdarpos = 0;
								rtpartcnt = -1;
							}
							else
								++rtpartcnt;
						}
						else
						{
							rtqdarpos = 0;
							rtpartcnt = -1;
						}
					}
					else
					{
						debug(270, "RDS/Rass CRC error, skip Rass-Qdar-Packet");
						debug(270, "RDS/Rass CRC read: %04X CRC calculated: %04X", rtcrc16, rtcrc ^ 0xFFFF);
						rtpartcnt = -1;
					}
					rtstate=0;
					break;

				// process RT plus tags ... 
				case 38: // Message Element Length
					rttextlen = c;	
					++rtstate;
					break;
				case 39: // Application ID 
				case 40: // always 0x4BD7 so we ignore it ;)
				case 41: // Applicationgroup Typecode/PTY ... ignore
					++rtstate;
					break;
				case 42:
					rtpbuf[0] = c;
					++rtstate;
					break;
				case 43:
					rtpbuf[1] = c;
					++rtstate;
					break;
				case 44:
					rtpbuf[2] = c;
					++rtstate;
					break;
				case 45:
					rtpbuf[3] = c;
					++rtstate;
					break;
				case 46: // bit 10#4 = Item Togglebit
					// bit 10#3 = Item Runningbit
					// Tag1: bit 10#2..11#5 = Contenttype, 11#4..12#7 = Startmarker, 12#6..12#1 = Length
					rtpbuf[4] = c;
					if(rtlastmessage[0] == 0) // no rds message till now ? quit ...
						break;
					int rtp_typ[2], rtp_start[2], rtp_len[2];
					rtp_typ[0] = (0x38 & rtpbuf[0] << 3) | rtpbuf[1] >> 5;
					rtp_start[0] = (0x3e & rtpbuf[1] <<1 ) | rtpbuf[2] >> 7;
					rtp_len[0] = 0x3f & rtpbuf[2] >> 1;
					// Tag2: bit 12#0..13#3 = Contenttype, 13#2..14#5 = Startmarker, 14#4..14#0 = Length(5bit)
					rtp_typ[1] = (0x20 & rtpbuf[2] << 5) | rtpbuf[3] >> 3;
					rtp_start[1] = (0x38 & rtpbuf[3] << 3) | rtpbuf[4] >> 5;
					rtp_len[1] = 0x1f & rtpbuf[4];
									
					unsigned char rtplusosdtmp[64];
					
					if(rtp_start[0] < 66 && (rtp_len[0] + rtp_start[0]) < 66)
					{
						memcpy(rtpitem[rtp_typ[0]], rtlastmessage + rtp_start[0], rtp_len[0] + 1);
						rtpitem[rtp_typ[0]][rtp_len[0] + 1] = 0;
					}
									
					if(rtp_typ[0] != rtp_typ[1])
					{
						if(rtp_start[1] < 66 && (rtp_len[1] + rtp_start[1]) < 66)
						{
							memcpy(rtpitem[rtp_typ[1]], rtlastmessage + rtp_start[1], rtp_len[1] + 1);
							rtpitem[rtp_typ[1]][rtp_len[1] + 1] = 0;
						}
					}

					// main RTPlus item_types used by the radio stations:
					// 1 title
					// 4 artist
					// 24 info.date_time
					// 31 stationname
					// 32 program.now
					// 39 homepage
					// 41 phone.hotline
					// 46 email.hotline
					// todo: make a window to display all saved items ...
	
					//create RTPlus OSD for title/artist
					rtplusosd[0] = 0;
								
					if(rtpitem[4][0] != 0) //artist
						sprintf((char*)rtplusosdtmp, " (%s)", rtpitem[4]);
								
					if(rtpitem[1][0] != 0) //title
						sprintf((char*)rtplusosd, "%s%s", rtpitem[1], rtplusosdtmp);
									
					if(rtplusosd[0] != 0)
					{
						// m_event(RtpTextChanged);
						debug(270, "RTPlus: %s", rtplusosd);
					}
						
					rtstate = 0;
					break;
			}
		}
	}
}

void rtprocesspes(unsigned char *data, int len)
{
	int pos = 9 + data[8]; // skip pes header

	while(pos < len)
	{
		if((0xFF & data[pos]) != 0xFF || (0xF0 & data[pos + 1]) != 0xF0)
			return;

		int padding_bit = (data[pos + 2] >> 1) & 1;
		int mode = (data[pos + 3] >> 6) & 3;
		int channel = mode == 3 ? 1 : 2;
		int id = (data[pos + 1] >> 3) & 1;
		int emphasis_bit = data[pos + 3] & 3;
		//int protection_bit = data[pos + 1] & 1;
		int rate = -1;
		int sample_freq = -1;
		int layer = -1;

		if(emphasis_bit == 2 && id == 1)
			id = 2;

		if((layer = (data[pos + 1] >> 1) & 3) < 1)
			return;

		if((rate = rtbitrate[id][layer - 1][(data[pos + 2] >> 4) & 0xf]) < 1)
			return;

		if((sample_freq = rtfrequency[id][(data[pos + 2] >> 2) & 3]) == 0)
			return;

		if(id == 1 && layer == 2)
		{
			if(rate / channel < 32000) return;
			if(rate / channel > 192000) return;
		}

		int frame_size = layer < 3 ?  (144 * rate / sample_freq) + padding_bit : ((12 * rate / sample_freq) * 4) + (4 * padding_bit);

		pos += frame_size;
		int offs = pos - 1;

		if (data[offs] == 0xFD)
		{
			//m_abortTimer->stop();
			int ancillary_len = 1 + data[offs - 1];
			offs -= ancillary_len;
			rtgotancillarydata(data + offs, ancillary_len -1 );
		}
	}
}

/*
int eDVBRdsDecoder::start(int pid)
{
	int ret = -1;
	if (m_pes_reader && !(ret = m_pes_reader->start(pid)) && rttype == 0)
		m_abortTimer->startLongTimer(20);
	m_pid = pid;
	return ret;
}

void eDVBRdsDecoder::abortNonAvail()
{
	eDebug("no ancillary data in audio stream... abort radiotext pes parser");
	if (m_pes_reader)
		m_pes_reader->stop();
}

ePyObject eDVBRdsDecoder::getRassPictureMask()
{
	ePyObject ret = PyTuple_New(5);
	PyTuple_SET_ITEM(ret, 0, PyInt_FromLong(rtrasspicturemask[0]));
	PyTuple_SET_ITEM(ret, 1, PyInt_FromLong(rtrasspicturemask[1]));
	PyTuple_SET_ITEM(ret, 2, PyInt_FromLong(rtrasspicturemask[2]));
	PyTuple_SET_ITEM(ret, 3, PyInt_FromLong(rtrasspicturemask[3]));
	PyTuple_SET_ITEM(ret, 4, PyInt_FromLong(rtrasspicturemask[4]));
	return ret;
}
*/

#endif
