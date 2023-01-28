#ifndef SCAN_H
#define SCAN_H

uint8_t changeservicetype(uint8_t type)
{
	int ret = type;

  debug(500, "changeservicetype -> oldtype %02x", type);

	switch(type)
	{
		case 0x01:
		case 0x06:
		case 0x0B:
		case 0x11:
		case 0x16:
		case 0x19:
		case 0x1C:
		case 0x1F:
		case 0x9A:
		case 0x86:
		case 0xC3:
		case 0xC5:
		case 0xC6:
			ret = 0;
			break;
		case 0x02:
		case 0x07:
		case 0x0A:
			ret = 1;
			break;
	}

	return ret;
}

struct transponder* satsystemdesc(unsigned char* buf, uint64_t transportid, unsigned short onid, int orbitalpos)
{
	int polarization = 0, modulation = 0, system = 0;
	int rolloff = 0, fec = 0;
	unsigned int frequency = 0, symbolrate = 0;
	uint64_t id = 0;
	struct transponder *tpnode = NULL;

	if(buf == NULL) return NULL;

	frequency = (
		((buf[2] >> 4) * 100000000) +
		((buf[2] & 0x0F) * 10000000) +
		((buf[3] >> 4) * 1000000) +
		((buf[3] & 0x0F) * 100000) +
		((buf[4] >> 4) * 10000) +
		((buf[4] & 0x0F) * 1000) +
		((buf[5] >> 4) * 100) +
		((buf[5] & 0x0F) * 10)
	);

	rolloff = (buf[8] >> 4) & 0x03; //0=rolloff_0_35, 1=rolloff_0_25, 2=rolloff_0_20, 3=rolloff_auto
	system = (buf[8] >> 2) & 0x01; //0=DVB_S, 1=DVB_S2
	modulation = (buf[8]) & 0x03; //1=QPSK, 2=PSK_8, 3=QAM_16

	symbolrate = (
		((buf[9] >> 4) * 100000000) +
		((buf[9] & 0x0F) * 10000000) +
		((buf[10] >> 4) * 1000000) +
		((buf[10] & 0x0F) * 100000) +
		((buf[11] >> 4) * 10000) +
		((buf[11] & 0x0F) * 1000) +
		((buf[12] >> 4) * 100)
	);

	fec = (buf[12]) & 0x0F;

	switch(fec)
	{
		case 0x01:
			fec = FEC_1_2;
			break;
		case 0x02:
			fec = FEC_2_3;
			break;
		case 0x03:
			fec = FEC_3_4;
			break;
		case 0x04:
			fec = FEC_5_6;
			break;
		case 0x05:
			fec = FEC_7_8;
			break;
		case 0x06:
			fec = FEC_8_9;
			break;
		case 0x07:
			fec = FEC_3_5;
			break;
		case 0x08:
			fec = FEC_4_5;
			break;
		case 0x09:
			fec = FEC_9_10;
			break;
		case 0x0F:
			fec = FEC_NONE;
			break;
		default:
			fec = FEC_AUTO;
			break;
	}

	polarization = (buf[8] >> 5) & 0x03; //0=H, 1=V

	//workarounds for braindead broadcasters (e.g. on Telstar 12 at 15.0W)
	if(frequency >= 100000000) frequency /= 10;
	if(symbolrate >= 50000000) symbolrate /= 10;

	frequency = (int)1000 * (int)round((double)frequency / (double)1000);

	if(frequency > 15000000) return NULL;

	id = ((onid << 16) | transportid) & 0xffffffff;

	if(gettransponder(id) == NULL)
	{
		tpnode = createtransponder(id, FE_QPSK, orbitalpos, frequency, INVERSION_AUTO, symbolrate, polarization, fec, modulation, rolloff, 0, system);
		status.writetransponder = 1;
	}

	debug(500, "nitscan: id=%llu freq=%d sr=%d fec=%d pol=%d modulation=%d system=%d tpnode=%p", id, frequency, symbolrate, fec, polarization, modulation, system, tpnode);

	return tpnode;
}

struct transponder* cablesystemdesc(unsigned char* buf, uint64_t transportid, unsigned short onid, int orbitalpos)
{
	int modulation = 0, fec = 0;
	unsigned int frequency = 0, symbolrate = 0;
	uint64_t id = 0;
	struct transponder *tpnode = NULL;

	if(buf == NULL) return NULL;

	frequency = (
		((buf[2] >> 4) * 1000000000) +
		((buf[2] & 0x0F) * 100000000) +
		((buf[3] >> 4) * 10000000) +
		((buf[3] & 0x0F) * 1000000) +
		((buf[4] >> 4) * 100000) +
		((buf[4] & 0x0F) * 10000) +
		((buf[5] >> 4) * 1000) +
		((buf[5] & 0x0F) * 100)
	);

	modulation = buf[8]; //0=QAM_AUTO, 1=QAM_16, 2=QAM_32, 3=QAM_64, 4=QAM_128, 5=QAM_256

	symbolrate = (
		((buf[9] >> 4) * 100000000) +
		((buf[9] & 0x0F) * 10000000) +
		((buf[10] >> 4) * 1000000) +
		((buf[10] & 0x0F) * 100000) +
		((buf[11] >> 4) * 10000) +
		((buf[11] & 0x0F) * 1000) +
		((buf[12] >> 4) * 100)
	);

	fec = (buf[12]) & 0x0F;

	switch(fec)
	{
		case 0x01:
			fec = FEC_1_2;
			break;
		case 0x02:
			fec = FEC_2_3;
			break;
		case 0x03:
			fec = FEC_3_4;
			break;
		case 0x04:
			fec = FEC_5_6;
			break;
		case 0x05:
			fec = FEC_7_8;
			break;
		case 0x06:
			fec = FEC_8_9;
			break;
		case 0x07:
			fec = FEC_3_5;
			break;
		case 0x08:
			fec = FEC_4_5;
			break;
		case 0x09:
			fec = FEC_9_10;
			break;
		case 0x0F:
			fec = FEC_NONE;
			break;
		default:
			fec = FEC_AUTO;
			break;
	}

	id = ((onid << 16) | transportid) & 0xffffffff;
	id = id | ((uint64_t)1 << 32);

	if(gettransponder(id) == NULL)
	//if(gettransponderbydetail(id, FE_QAM, orbitalpos, frequency, INVERSION_AUTO, symbolrate, 0, fec, modulation, 0, 0, 0, 0) == NULL)
	{
		tpnode = createtransponder(id, FE_QAM, orbitalpos, frequency, INVERSION_AUTO, symbolrate, 0, fec, modulation, 0, 0, 0);
		status.writetransponder = 1;
	}

	debug(500, "nitscan: id=%llu freq=%d sr=%d fec=%d modulation=%d tpnode=%p", id, frequency, symbolrate, fec, modulation, tpnode);

	return tpnode;
}

struct transponder* terrsystemdesc(unsigned char* buf, uint64_t transportid, unsigned short onid, int orbitalpos)
{
	int modulation = 0, hp = 0, lp = 0;
	int bandwidth = 0, hierarchy = 0, guardinterval = 0;
	int transmission = 0;
	unsigned int frequency = 0;
	uint64_t id = 0;
	struct transponder *tpnode = NULL;

	if(buf == NULL) return NULL;

	frequency = (buf[2] << 24) | (buf[3] << 16);
	frequency |= (buf[4] << 8) | buf[5];
	frequency *= 10;

	bandwidth = BANDWIDTH_8_MHZ + ((buf[6] >> 5) & 0x3);
	modulation = (buf[7] >> 6) & 0x3;
	hierarchy = HIERARCHY_NONE + ((buf[7] >> 3) & 0x3);

	hp = (buf[7] & 0x7);
	switch(hp)
	{
		case 0x00:
			hp = FEC_1_2;
			break;
		case 0x01:
			hp = FEC_2_3;
			break;
		case 0x02:
			hp = FEC_3_4;
			break;
		case 0x03:
			hp = FEC_5_6;
			break;
		case 0x04:
			hp = FEC_7_8;
			break;
		default:
			hp = FEC_AUTO;
			break;
	}

	lp = ((buf[8] >> 5) & 0x7);
	switch(lp)
	{
		case 0x00:
			lp = FEC_1_2;
			break;
		case 0x01:
			lp = FEC_2_3;
			break;
		case 0x02:
			lp = FEC_3_4;
			break;
		case 0x03:
			lp = FEC_5_6;
			break;
		case 0x04:
			lp = FEC_7_8;
			break;
		default:
			lp = FEC_AUTO;
			break;
	}

	guardinterval = GUARD_INTERVAL_1_32 + ((buf[8] >> 3) & 0x3);

	transmission = (buf[8] & 0x2);
	switch(transmission)
	{
		case 0x00:
			transmission = TRANSMISSION_MODE_2K;
			break;
		case 0x01:
			transmission = TRANSMISSION_MODE_8K;
			break;
		case 0x02:
			transmission = TRANSMISSION_MODE_AUTO;
			break;
		default:
			transmission = TRANSMISSION_MODE_AUTO;
			break;
	}

	//other_frequency_flag = (buf[8] & 0x01);

	id = ((onid << 16) | transportid) & 0xffffffff;
	id = id | ((uint64_t)2 << 32);

	if(gettransponder(id) == NULL)
	{
		tpnode = createtransponder(id, FE_OFDM, orbitalpos, frequency, INVERSION_AUTO, bandwidth, lp, hp, modulation, guardinterval, transmission, hierarchy);
		status.writetransponder = 1;
	}

	debug(500, "nitscan: id=%llu freq=%d bandwidth=%d hp=%d lp=%d modulation=%d guard=%d trans=%d hierarchy=%d tpnode=%p", id, frequency, bandwidth, hp, lp, modulation, guardinterval, transmission, hierarchy, tpnode);
	debug(200, "nitscan: id=%llu freq=%d bandwidth=%d hp=%d lp=%d modulation=%d guard=%d trans=%d hierarchy=%d tpnode=%p", id, frequency, bandwidth, hp, lp, modulation, guardinterval, transmission, hierarchy, tpnode);

	return tpnode;
}

//struct transponder* terrsystemdesc2(unsigned char* buf, uint64_t transportid, unsigned short onid, int orbitalpos)
int terrsystemdesc2(unsigned char* buf, uint64_t transportid, unsigned short onid, int orbitalpos)
{
	int modulation = 0, hp = 0, lp = 0;
	int bandwidth = 0, hierarchy = 0, guardinterval = 0;
	int transmission = 0;
	int inversion = 0;
	int plp_id = 0;
	unsigned int frequency = 0;
	uint64_t id = 0;
	struct transponder *tpnode = NULL;
	int addtrans = 0;
	int system = 0;
	
	if(buf == NULL) return -1;
		
	int dlen = buf[1];
	if (dlen < 5)
	{
		debug(500, "exit DVB-T2 desc... length < 5");
		return -1;	
	}

	bandwidth = ((buf[6] >> 2) & 0x0f);
	switch (bandwidth)
	{
		case 0: bandwidth = T_Bandwidth_8MHz; break;
		case 1: bandwidth = T_Bandwidth_7MHz; break;
		case 2: bandwidth = T_Bandwidth_6MHz; break;
		case 3: bandwidth = T_Bandwidth_5MHz; break;
		case 4: bandwidth = T_Bandwidth_1_712MHz; break;
		case 5: bandwidth = T_Bandwidth_10MHz; break;
		default: bandwidth = T_Bandwidth_Auto; break;
	}
	
	transmission = (buf[7] >> 2 & 0x3);
	switch (transmission)
	{
		case 0: transmission = T_TransmissionMode_2k; break;
		case 1: transmission = T_TransmissionMode_8k; break;
		case 2: transmission = T_TransmissionMode_4k; break;
		case 3: transmission = T_TransmissionMode_1k; break;
		case 4: transmission = T_TransmissionMode_16k; break;
		case 5: transmission = T_TransmissionMode_32k; break;
		default: transmission = T_TransmissionMode_Auto; break;
	}

	guardinterval = ((buf[7] >> 5) & 0x3);
	switch (guardinterval)
	{
		case 0: guardinterval = T_GuardInterval_1_32; break;
		case 1: guardinterval = T_GuardInterval_1_16; break;
		case 2: guardinterval = T_GuardInterval_1_8; break;
		case 3: guardinterval = T_GuardInterval_1_4; break;
		case 4: guardinterval = T_GuardInterval_1_128; break;
		case 5: guardinterval = T_GuardInterval_19_128; break;
		case 6: guardinterval = T_GuardInterval_19_256; break;
		case 7: guardinterval = T_GuardInterval_Auto; break;
	}
	
	plp_id = buf[3];
	hp = lp = T_FEC_Auto;
	hierarchy = T_Hierarchy_Auto;
	modulation = T_Modulation_Auto;
	inversion = T_Inversion_Unknown;
	system = System_DVB_T2; 
	
	unsigned char* loop1 = buf + 8;     //call_id
	unsigned char* loop2 = buf + 11;    //centre_frequency if Flag == 1
	unsigned char* loop3 = buf + 10;    //centre_frequency if Flag == 0
	unsigned int cfre = 0;
	int i1 = 0;
	int i2 = 0;
	int step1 = 0;
	int sillen = 0;
	int fllen = 0;
	
	int flag = (buf[7] & 0x1);

	id = ((onid << 16) | transportid) & 0xffffffff;
	id = id | ((uint64_t)2 << 32);

	if (flag == 0)
	{
		for(i1 = 0; i1 < dlen-6; i1=i1+step1)
		{
			step1 = 6;
		
			cfre = ((loop3[i1] << 24) & 0xff000000);
			cfre = cfre | ((loop3[i1+1] << 16) & 0xff0000);
			cfre = cfre | ((loop3[i1+2] << 8) & 0xff00);
			cfre = cfre | (loop3[i1+3] & 0xff);
			frequency = cfre * 10;
			debug(500, "nitscan DVB-T2 - Flag=%d -> id=%llu freq=%d bandwidth=%d hp=%d lp=%d modulation=%d guard=%d trans=%d hierarchy=%d tpnode=%p", flag, id, frequency, bandwidth, hp, lp, modulation, guardinterval, transmission, hierarchy, tpnode);
			debug(200, "nitscan DVB-T2 - Flag=%d -> id=%llu freq=%d bandwidth=%d hp=%d lp=%d modulation=%d guard=%d trans=%d hierarchy=%d tpnode=%p", flag, id, frequency, bandwidth, hp, lp, modulation, guardinterval, transmission, hierarchy, tpnode);
			tpnode = NULL;
			if(checktransponderonscan(frequency, orbitalpos) == 0)
			{
				tpnode = createtransponder(id, FE_OFDM, orbitalpos, frequency, inversion, bandwidth, lp, hp, modulation, guardinterval, transmission, system);
				debug(500, "nitscan DVB-T2 --> add transponder");
				//tpnode = createtransponder(0, FE_OFDM, orbitalpos, frequency, inversion, bandwidth, lp, hp, modulation, guardinterval, transmission, system);
			}
			if(tpnode != NULL)
				addtrans++;
			else
				printf("nitscan DVB-T2 --> transponder not added\n");
				//err("not add nitscan DVB-T2 - Flag=%d -> id=%llu frequency:%s", flag, id, frequency);
			sillen = loop3[i1+4];
			step1 = step1 + sillen+1;
		}
	}
	else
	{
		for(i1 = 0; i1 < dlen-6; i1=i1+step1)
		{	
			fllen = loop1[i1+2];
			step1 = 3;
			step1 = step1 + fllen; 
			for(i2 = 0; i2 < fllen; i2=i2+4)
			{
				cfre = ((loop2[i2] << 24) & 0xff000000);
				cfre = cfre | ((loop2[i2+1] << 16) & 0xff0000);
				cfre = cfre | ((loop2[i2+2] << 8) & 0xff00);
				cfre = cfre | (loop2[i2+3] & 0xff);
				frequency = cfre * 10;
				debug(500, "nitscan DVB-T2 - Flag=%d -> id=%llu freq=%d bandwidth=%d hp=%d lp=%d modulation=%d guard=%d trans=%d hierarchy=%d tpnode=%p", flag, id, frequency, bandwidth, hp, lp, modulation, guardinterval, transmission, hierarchy, tpnode);
				debug(200, "nitscan DVB-T2 - Flag=%d -> id=%llu freq=%d bandwidth=%d hp=%d lp=%d modulation=%d guard=%d trans=%d hierarchy=%d tpnode=%p", flag, id, frequency, bandwidth, hp, lp, modulation, guardinterval, transmission, hierarchy, tpnode);				
				tpnode = NULL;
				if(checktransponderonscan(frequency, orbitalpos) == 0)
				{
					tpnode = createtransponder(id, FE_OFDM, orbitalpos, frequency, inversion, bandwidth, lp, hp, modulation, guardinterval, transmission, system);
					debug(500, "nitscan DVB-T2 --> add transponder");
					//tpnode = createtransponder(0, FE_OFDM, orbitalpos, frequency, inversion, bandwidth, lp, hp, modulation, guardinterval, transmission, system);
				}
				if(tpnode != NULL)
					addtrans++;
				else
					printf("nitscan DVB-T2 --> transponder not added\n");
					//err("not add nitscan DVB-T2 - Flag=%d -> id=%llu frequency:%s", flag, id, frequency);
			}
			sillen = loop2[i2];
			step1 = step1 + sillen+1;
		}
	}
	//return tpnode;
	return addtrans;
}

int parsenit(unsigned char* buf, uint8_t* lastsecnr, int orbitalpos)
{
	int ret = 0;

	if(buf == NULL) return ret;

	//unsigned char buf[MINMALLOC];

	// position in buffer
	unsigned short pos = 0;
	unsigned short pos2 = 0;

	// network_information_section elements
	unsigned short seclen = 0;
	unsigned short desclen = 0;
	unsigned short tdesclen = 0;
	unsigned short looplen = 0;
	uint64_t transponderid = 0;
	unsigned short onid = 0;
	unsigned short nid = 0;
	unsigned char secnr = 0;

	seclen = ((buf[1] & 0x0F) << 8) + buf[2];
	nid = ((buf[3] << 8)| buf[4]);
	desclen = ((buf[8] & 0x0F) << 8) | buf[9];
	secnr = buf[6];
	*lastsecnr = buf[7];
	debug(500, "nitscan: section %d last %d nid %d", secnr, *lastsecnr, nid);

	for(pos = 10; pos < desclen + 10; pos += buf[pos + 1] + 2)
	{
		switch(buf[pos])
		{
			case 0x0F:
				//private_data_indicator_desc(buf + pos);
				break;
			case 0x40:
				//network_name_desc(buf + pos);
				break;
			case 0x4A:
				//linkage_desc(buf + pos);
				break;
			case 0x5B:
				//multilingual_networkname_desc(buf + pos);
				break;
			case 0x5F:
				//private_data_specifier_desc(buf + pos);
				break;
			case 0x80:
				break;
			case 0x90:
				break;
		}
	}

	looplen = ((buf[pos] & 0x0F) << 8) | buf[pos + 1];
	if (!looplen) return ret;

	for(pos += 2; pos < seclen - 3; pos += tdesclen + 6)
	{
		transponderid = (buf[pos] << 8) | buf[pos + 1];
		onid = (buf[pos + 2] << 8) | buf[pos + 3];
		tdesclen = ((buf[pos + 4] & 0x0F) << 8) | buf[pos + 5];

		for(pos2 = pos + 6; pos2 < pos + tdesclen + 6; pos2 += buf[pos2 + 1] + 2)
		{
			switch(buf[pos2])
			{
				case 0x41: //service_list_descriptor
					//servicelistdesc(buf + pos2, transponderid, onid);
					break;
				case 0x43: //satellite_delivery_system_descriptor
					if(satsystemdesc(buf + pos2, transponderid, onid, orbitalpos) != NULL)
					{
						scaninfo.tpnew++;
						if(scaninfo.scantype != 0)
							scaninfo.tpmax++;
					}
					break;
				case 0x44: //cable_delivery_system_descriptor
					if(cablesystemdesc(buf + pos2, transponderid, onid, orbitalpos) != NULL)
					{
						scaninfo.tpnew++;
						if(scaninfo.scantype != 0)
							scaninfo.tpmax++;
					}
					break;
				case 0x5A: //terrestrial_delivery_system_descriptor
					if(terrsystemdesc(buf + pos2, transponderid, onid, orbitalpos) != NULL)
					{
						scaninfo.tpnew++;
						if(scaninfo.scantype != 0)
							scaninfo.tpmax++;
					}
					break;
				case 0x62: //frequency_list_descriptor
					//frequencylistdesc(buf + pos2);
					break;
				case 0x7F: //extension_descriptor
					{
						switch(buf[pos2+2])
						case 0x04: //T2_delivery_system_descriptor
							ret = terrsystemdesc2(buf + pos2, transponderid, onid, orbitalpos);
							//if(terrsystemdesc2(buf + pos2, transponderid, onid, orbitalpos) > 0)
							if(ret > 0)
							{
								scaninfo.tpnew = scaninfo.tpnew + ret;
								if(scaninfo.scantype != 0)
									scaninfo.tpmax = scaninfo.tpmax + ret;
							}
							ret = 0;
							break;
					}
					break;
			}
		}
	}

	return ret;
}

//flag 0: from scan
//flag 1: from update channelname
int findchannel(struct dvbdev* fenode, struct transponder* tpnode, unsigned char *buf, uint8_t* lastsecnr, struct skin* scan, struct skin* listbox, int ichangename, int flag)
{
	int ret = -1, changed = 0;
	uint64_t transponderid = 0;
	struct skin* node = NULL;
	struct channel* chnode = NULL;

	// position in buffer
	unsigned short pos;
	unsigned short pos2;

	// service_description_section elements
	unsigned short seclen;
	uint8_t secnr;
	unsigned short onid, tid;
	unsigned short serviceid;
	unsigned short desclooplen;
	unsigned short running;
	int eitscheduleflag;
	int eitpresentfollowingflag;
	int camode;
	uint8_t servicetype = 0;
	uint8_t providerlen = 0;
	char* tmpstr = NULL, *tmpstr1 = NULL, *tmpstr2 = NULL;
	uint64_t* tmpuint64 = NULL;

	struct transponder* tphelp = NULL;

	if(buf == NULL || fenode == NULL || fenode->feinfo == NULL) return ret;

	seclen = ((buf[1] & 0x0F) << 8) | buf[2];
	secnr = buf[6];
	*lastsecnr = buf[7];
	tid = (buf[3] << 8) | buf[4];
	onid = (buf[8] << 8) | buf[9];

	transponderid = ((onid << 16) | tid) & 0xffffffff;
	if(fenode->feinfo->type == FE_QAM)
		transponderid = transponderid | ((uint64_t)1 << 32);
	else if(fenode->feinfo->type == FE_OFDM)
		transponderid = transponderid | ((uint64_t)2 << 32);

	//if(tpnode != NULL && (tpnode->id != transponderid || scaninfo.fenode->feinfo->type == FE_QAM || scaninfo.fenode->feinfo->type == FE_OFDM) && tpnode->id != 99)
	if(tpnode != NULL && tpnode->id != transponderid && tpnode->id != 99)
	{
		tphelp = gettransponder(transponderid);
		if(tphelp != NULL)
		{
			deltranspondercache(tphelp->id, tphelp);
			tphelp->id = 0;
			modifytranspondercache(tphelp->id, tphelp);
			debug(500, "set old tid: %llu to 0", transponderid);

			//changetransponderid(tphelp, 0);
			//debug(500, "set old tid: %llu to 0", transponderid);
		}
		changetransponderid(tpnode, transponderid);
		status.writetransponder = 1;
	}

	debug(500, "SDT nr: %d, lastnr: %d, len: %d, tid: %d, onid: %d, transponderid: %llu", secnr, *lastsecnr, seclen, tid, onid, transponderid);

	for(pos = 11; pos < seclen - 1; pos += desclooplen + 5)
	{
		serviceid = (buf[pos] << 8) | buf[pos + 1];
		eitscheduleflag = buf[pos + 2] & 0x02;
		eitpresentfollowingflag = buf[pos + 2] & 0x01;
		running = buf[pos + 3] & 0xE0;
		camode = buf[pos + 3] & 0x10;
		desclooplen = ((buf[pos + 3] & 0x0F) << 8) | buf[pos + 4];
		if(flag == 0 && scaninfo.onlyfree == 1 && camode > 0) continue;

		for(pos2 = pos + 5; pos2 < pos + desclooplen + 5; pos2 += buf[pos2 + 1] + 2)
		{
			switch(buf[pos2])
			{
				case 0x48:
					servicetype = buf[pos2 + 2];
					servicetype = changeservicetype(servicetype);
					providerlen = buf[pos2 + 3];

					//providername
					tmpstr1 = strndup((char*)&(buf[pos2 + 4]), providerlen);
					//channelname
					tmpstr2 = strndup((char*)&(buf[pos2 + 4 + providerlen + 1]), (2 + buf[pos2 + 1]) - (4 + providerlen + 1));

					tmpstr1 = string_strip_whitechars(tmpstr1);
					tmpstr2 = string_strip_whitechars(tmpstr2);
					if(tmpstr1 == NULL || strlen(tmpstr1) == 0) tmpstr1 = ostrcat(tmpstr1, "unknown", 1, 0);
					if(tmpstr2 == NULL || strlen(tmpstr2) == 0) tmpstr2 = ostrcat(tmpstr2, "unknown", 1, 0);

					tmpstr1 = strutf8(tpnode, tmpstr1, strlen(tmpstr1), 0, 1, 0);
					tmpstr1 = stringreplacechar(tmpstr1, '#', '_');
					tmpstr2 = strutf8(tpnode, tmpstr2, strlen(tmpstr2), 0, 1, 1);
					tmpstr2 = stringreplacechar(tmpstr2, '#', '_');

					//add to listbox
					chnode = getchannel(serviceid, transponderid);
					if(flag == 0) node = addlistbox(scan, listbox, node, 1);
					if(node != NULL)
					{
						switch(servicetype)
						{
							case 0:
								scaninfo.tvcount++;
								if(chnode == NULL) scaninfo.newtvcount++;
								break;
							case 1:
								scaninfo.radiocount++;
								if(chnode == NULL) scaninfo.newradiocount++;
								break;
							default:
								scaninfo.datacount++;
								if(chnode == NULL) scaninfo.newdatacount++;
								break;
						}
						tmpstr = ostrcat(tmpstr2, " (", 0, 0);
						tmpstr = ostrcat(tmpstr, tmpstr1, 1, 0);
						tmpstr = ostrcat(tmpstr, " - ", 1, 0);
						if(servicetype == 0)
							tmpstr = ostrcat(tmpstr, _("TV"), 1, 0);
						else if(servicetype == 1)
							tmpstr = ostrcat(tmpstr, _("Radio"), 1, 0);
						else
							tmpstr = ostrcat(tmpstr, _("Data"), 1, 0);

						changed = 0;
						if(chnode != NULL && chnode->name != NULL && strlen(chnode->name) > 0 && ostrcmp(tmpstr2, chnode->name) != 0)
						{
							tmpstr = ostrcat(tmpstr, " - ", 1, 0);
							tmpstr = ostrcat(tmpstr, chnode->name, 1, 0);
							if(ichangename == 1) changed = 1;
						}
						tmpstr = ostrcat(tmpstr, ")", 1, 0);
						changetext(node, tmpstr);

						if(chnode != NULL && chnode->transponder != NULL && changed == 0)
							node->fontcol = convertcol("deaktivcol");

						changeparam1(node, tmpstr1);
						changeparam2(node, tmpstr2);
						tmpuint64 = (uint64_t*)calloc(1, sizeof(uint64_t) * 3);
						if(tmpuint64 != NULL)
						{
							tmpuint64[0] = serviceid;
							tmpuint64[1] = transponderid;
							tmpuint64[2] = servicetype;
						}
						free(node->name);
						node->name = (char*)tmpuint64;
					}

					if(flag == 1 && chnode != NULL && ostrcmp(chnode->name, tmpstr2) != 0)
					{
						free(chnode->name);
						chnode->name = ostrcat(tmpstr2, NULL, 0, 0);
						status.writechannel = 1;
					}

					free(tmpstr); tmpstr = NULL;
					free(tmpstr1); tmpstr1 = NULL;
					free(tmpstr2); tmpstr2 = NULL;

					ret = 0;
					break;
			}
		}
	}

	return ret;
}

uint64_t findtransponderid(struct dvbdev* fenode, unsigned char *buf)
{
	uint64_t transponderid = 0;
	unsigned short onid, tid;

	if(buf == NULL || fenode == NULL || fenode->feinfo == NULL) return 0;

	tid = (buf[3] << 8) | buf[4];
	onid = (buf[8] << 8) | buf[9];

	transponderid = ((onid << 16) | tid) & 0xffffffff;

	if(fenode->feinfo->type == FE_QAM)
		transponderid = transponderid | ((uint64_t)1 << 32);
	else if(fenode->feinfo->type == FE_OFDM)
		transponderid = transponderid | ((uint64_t)2 << 32);

	debug(500, "found SDT transponderid: %llu", transponderid);

	return transponderid;
}

unsigned int satblindscan(struct stimerthread* timernode, int onlycalc)
{
	int festatus = 0;
	uint64_t transponderid = 0;
	unsigned char* buf = NULL;
	struct dvbdev* fenode = NULL;
	struct transponder* tpnode = NULL;

	unsigned int frequency = 0, symbolrate = 0;
	int polarization = 0, modulation = 0, system = 0, fec = 0;

	unsigned int minfrequency = getconfigint("blindminfrequency", NULL) * 1000;
	unsigned int maxfrequency = getconfigint("blindmaxfrequency", NULL) * 1000;
	unsigned int stepfrequency = getconfigint("blindstepfrequency", NULL) * 1000;
	unsigned int minsymbolrate = getconfigint("blindminsignalrate", NULL) * 1000;
	unsigned int maxsymbolrate = getconfigint("blindmaxsignalrate", NULL) * 1000;
	unsigned int stepsymbolrate = getconfigint("blindstepsignalrate", NULL) * 1000;
	unsigned int usedefaultsr = getconfigint("blindusedefaultsr", NULL);
	unsigned int onlydvbs = getconfigint("blindonlydvbs", NULL);
	unsigned int usedefaultfec = getconfigint("blindusedefaultfec", NULL);

	int minmodulation = 1, maxmodulation = 2, stepmodulation = 1;
	int minpolarization = 0, maxpolarization = 1, steppolarization = 1;
	int minsystem = 0, maxsystem = 1, stepsystem = 1;
	int minfec = 0, maxfec = 8, stepfec = 1;

	if(onlydvbs == 1)
	{
		maxmodulation = 0;
		maxsystem = 0;
	}

	if(usedefaultsr == 1)
	{
		minsymbolrate = 0;
		maxsymbolrate = 3;
		stepsymbolrate = 1;
	}

	if(usedefaultfec == 1)
		maxfec = 6;

	int countfrequency = ((maxfrequency + stepfrequency) - minfrequency) / stepfrequency;
	int countsymbolrate = ((maxsymbolrate + stepsymbolrate) - minsymbolrate) / stepsymbolrate;
	int countmodulation = ((maxmodulation + stepmodulation) - minmodulation) / stepmodulation;
	int countpolarization = ((maxpolarization + steppolarization) - minpolarization) / steppolarization;
	int countfec = ((maxfec + stepfec) - minfec) / stepfec;
	int systemcount = ((maxsystem + stepsystem) - minsystem) / stepsystem;

	if(onlycalc == 1) return systemcount * countpolarization * countmodulation * countsymbolrate * countfrequency * countfec;

	if(scaninfo.fenode == NULL || timernode == NULL) return -1;
	int timeout = scaninfo.timeout / 5;
	if(timeout < 1000000) timeout = 1000000;
	scaninfo.blindmax += systemcount * countpolarization * countmodulation * countsymbolrate * countfrequency * countfec;

	for(frequency = minfrequency; frequency <= maxfrequency; frequency += stepfrequency)
	{

		int csymbolrate = 0;
		for(csymbolrate = minsymbolrate; csymbolrate <= maxsymbolrate; csymbolrate += stepsymbolrate)
		{
			if(usedefaultsr == 1)
			{
				switch(csymbolrate)
				{
					case 0:
						symbolrate = 22000 * 1000;
						break;
					case 1:
						symbolrate = 27500 * 1000;
						break;
					case 2:
						symbolrate = 30000 * 1000;
						break;
				}
			}
			else
				symbolrate = csymbolrate;

			for(modulation = minmodulation; modulation <= maxmodulation; modulation += stepmodulation)
			{

				for(polarization = minpolarization; polarization <= maxpolarization; polarization += steppolarization)
				{

					for(fec = minfec; fec <= maxfec; fec += stepfec)
					{

						for(system = minsystem; system <= maxsystem; system += stepsystem)
						{
							scaninfo.blindcount++;

							tpnode = createtransponder(99, scaninfo.fenode->feinfo->type, scaninfo.orbitalpos, frequency, INVERSION_AUTO, symbolrate, polarization, fec, modulation, 0, 0, system);

							debug(500, "blindscan: frequ=%d, sr=%d, modulation=%d, fec=%d, system=%d, orbitalpos=%d", frequency, symbolrate, modulation, fec, system, scaninfo.orbitalpos);

							if(tpnode != NULL)
							{

								fenode = fegetfree(tpnode, 0, scaninfo.fenode, NULL);
								if(fenode == NULL )
								{
									debug(500, "Frontend for scan not free");
									deltransponderbyid(99);
									continue;
								}

								//frontend tune
								if(fenode->feinfo->type == FE_QPSK)
								{
									feset(fenode, tpnode);
									fetunedvbs(fenode, tpnode);
								}
								else
								{
									debug(500, "Frontend type unknown");
									deltransponderbyid(99);
									continue;
								}

								festatus = fewait(fenode);
								if(debug_level == 200) fereadstatus(fenode);
								if(festatus != 0)
								{
									debug(500, "tuning failed");
									deltransponderbyid(99);
									continue;
								}

								buf = dvbgetsdt(fenode, 0, timeout);
								transponderid = findtransponderid(fenode, buf);
								free(buf); buf = NULL;

								if(transponderid == 0 || gettransponder(transponderid) != NULL)
								{
									deltransponderbyid(99);
									continue;
								}

								if(tpnode->id != transponderid)
								{
									struct transponder* tphelp = gettransponder(transponderid);
									if(tphelp != NULL)
									{
										deltransponderbyid(transponderid);
										debug(500, "delete old tid: %llu", transponderid);
									}
									scaninfo.newblindcount++;
									changetransponderid(tpnode, transponderid);
									status.writetransponder = 1;
								}
							}
							deltransponderbyid(99);
							if(timernode->aktion != START) break;
						}
						if(timernode->aktion != START) break;
					}
					if(timernode->aktion != START) break;
				}
				if(timernode->aktion != START) break;
			}
			if(timernode->aktion != START) break;
		}
		if(timernode->aktion != START) break;
	}

	return 0;
}

unsigned int cableblindscan(struct stimerthread* timernode, int onlycalc)
{
	int festatus = 0;
	uint64_t transponderid = 0;
	unsigned char* buf = NULL;
	struct dvbdev* fenode = NULL;
	struct transponder* tpnode = NULL;

	unsigned int frequency = 0, symbolrate = 0;
	int modulation = 0, fec = 0;

	unsigned int minfrequency = getconfigint("cblindminfrequency", NULL) * 1000;
	unsigned int maxfrequency = getconfigint("cblindmaxfrequency", NULL) * 1000;
	unsigned int stepfrequency = getconfigint("cblindstepfrequency", NULL) * 1000;
	unsigned int minsymbolrate = getconfigint("cblindminsignalrate", NULL) * 1000;
	unsigned int maxsymbolrate = getconfigint("cblindmaxsignalrate", NULL) * 1000;
	unsigned int stepsymbolrate = getconfigint("cblindstepsignalrate", NULL) * 1000;
	unsigned int usedefaultsr = getconfigint("cblindusedefaultsr", NULL);
	unsigned int usedefaultfec = getconfigint("cblindusedefaultfec", NULL);

	int minmodulation = 0, maxmodulation = 4, stepmodulation = 1;
	int minfec = 0, maxfec = 8, stepfec = 1;

	if(usedefaultsr == 1)
	{
		minsymbolrate = 0;
		maxsymbolrate = 3;
		stepsymbolrate = 1;
	}

	if(usedefaultfec == 1)
		maxfec = 6;

	int countfrequency = ((maxfrequency + stepfrequency) - minfrequency) / stepfrequency;
	int countsymbolrate = ((maxsymbolrate + stepsymbolrate) - minsymbolrate) / stepsymbolrate;
	int countmodulation = ((maxmodulation + stepmodulation) - minmodulation) / stepmodulation;
	int countfec = ((maxfec + stepfec) - minfec) / stepfec;

	if(onlycalc == 1) return countmodulation * countsymbolrate * countfrequency * countfec;

	if(scaninfo.fenode == NULL || timernode == NULL) return -1;
	int timeout = scaninfo.timeout / 5;
	if(timeout < 1000000) timeout = 1000000;
	scaninfo.blindmax += countmodulation * countsymbolrate * countfrequency * countfec;

	for(frequency = minfrequency; frequency <= maxfrequency; frequency += stepfrequency)
	{

		int csymbolrate = 0;
		for(csymbolrate = minsymbolrate; csymbolrate <= maxsymbolrate; csymbolrate += stepsymbolrate)
		{
			if(usedefaultsr == 1)
			{
				switch(csymbolrate)
				{
					case 0:
						symbolrate = 22000 * 1000;
						break;
					case 1:
						symbolrate = 27500 * 1000;
						break;
					case 2:
						symbolrate = 30000 * 1000;
						break;
				}
			}
			else
				symbolrate = csymbolrate;

			for(modulation = minmodulation; modulation <= maxmodulation; modulation += stepmodulation)
			{

				for(fec = minfec; fec <= maxfec; fec += stepfec)
				{
					scaninfo.blindcount++;

					tpnode = createtransponder(99, scaninfo.fenode->feinfo->type, scaninfo.orbitalpos, frequency, INVERSION_AUTO, symbolrate, 0, fec, modulation, 0, 0, 0);

					debug(500, "blindscan: frequ=%d, sr=%d, modulation=%d, fec=%d, orbitalpos=%d", frequency, symbolrate, modulation, fec, scaninfo.orbitalpos);

					if(tpnode != NULL)
					{

						fenode = fegetfree(tpnode, 0, scaninfo.fenode, NULL);
						if(fenode == NULL )
						{
							debug(500, "Frontend for scan not free");
							deltransponderbyid(99);
							continue;
						}

						//frontend tune
						if(fenode->feinfo->type == FE_QAM)
							fetunedvbc(fenode, tpnode);
						else
						{
							debug(500, "Frontend type unknown");
							deltransponderbyid(99);
							continue;
						}

						festatus = fewait(fenode);
						if(debug_level == 200) fereadstatus(fenode);
						if(festatus != 0)
						{
							debug(500, "tuning failed");
							deltransponderbyid(99);
							continue;
						}

						buf = dvbgetsdt(fenode, 0, timeout);
						transponderid = findtransponderid(fenode, buf);
						free(buf); buf = NULL;

						if(transponderid == 0 || gettransponder(transponderid) != NULL)
						{
							deltransponderbyid(99);
							continue;
						}

						if(tpnode->id != transponderid)
						{
							struct transponder* tphelp = gettransponder(transponderid);
							if(tphelp != NULL)
							{
								deltransponderbyid(transponderid);
								debug(500, "delete old tid: %llu", transponderid);
							}
							scaninfo.newblindcount++;
							changetransponderid(tpnode, transponderid);
							status.writetransponder = 1;
						}
					}
					deltransponderbyid(99);
					if(timernode->aktion != START) break;
				}
				if(timernode->aktion != START) break;
			}
			if(timernode->aktion != START) break;
		}
		if(timernode->aktion != START) break;
	}

	return 0;
}

unsigned int terrblindscan(struct stimerthread* timernode, int onlycalc)
{
	int festatus = 0;
	uint64_t transponderid = 0;
	unsigned char* buf = NULL;
	struct dvbdev* fenode = NULL;
	struct transponder* tpnode = NULL;

	unsigned int frequency = 0;
	int hp = 0, lp = 0, modulation = 0, bandwidth = 0, transmission = 0, guardinterval = 0, hierarchy = 0;

	unsigned int minfrequency = getconfigint("tblindminfrequency", NULL) * 1000;
	unsigned int maxfrequency = getconfigint("tblindmaxfrequency", NULL) * 1000;
	unsigned int stepfrequency = getconfigint("tblindstepfrequency", NULL) * 1000;

	int minhp = 0, maxhp = 3, stephp = 1;
	int minlp = 0, maxlp = 3, steplp = 1;
	int minmodulation = 0, maxmodulation = 2, stepmodulation = 1;
	int minbandwidth = 0, maxbandwidth = 2, stepbandwidth = 1;
	int mintransmission = 0, maxtransmission = 1, steptransmission = 1;
	int minguardinterval = 0, maxguardinterval = 3, stepguardinterval = 1;
	int minhierarchy = 0, maxhierarchy = 3, stephierarchy = 1;

	int countfrequency = ((maxfrequency + stepfrequency) - minfrequency) / stepfrequency;
	int counthp = ((maxhp + stephp) - minhp) / stephp;
	int countlp = ((maxlp + steplp) - minlp) / steplp;
	int countmodulation = ((maxmodulation + stepmodulation) - minmodulation) / stepmodulation;
	int countbandwidth = ((maxbandwidth + stepbandwidth) - minbandwidth) / stepbandwidth;
	int counttransmission = ((maxtransmission + steptransmission) - mintransmission) / steptransmission;
	int countguardinterval = ((maxguardinterval + stepguardinterval) - minguardinterval) / stepguardinterval;
	int counthierarchy = ((maxhierarchy + stephierarchy) - minhierarchy) / stephierarchy;

	if(onlycalc == 1) return counthierarchy * countguardinterval * countmodulation * counttransmission * countfrequency * countbandwidth * countlp * counthp;

	if(scaninfo.fenode == NULL || timernode == NULL) return -1;
	int timeout = scaninfo.timeout / 5;
	if(timeout < 1000000) timeout = 1000000;
	scaninfo.blindmax += counthierarchy * countguardinterval * countmodulation * counttransmission * countfrequency * countbandwidth * countlp * counthp;

	for(frequency = minfrequency; frequency <= maxfrequency; frequency += stepfrequency)
	{

		for(hp = minhp; hp <= maxhp; hp += stephp)
		{

			for(lp = minlp; lp <= maxlp; lp += steplp)
			{

				for(modulation = minmodulation; modulation <= maxmodulation; modulation += stepmodulation)
				{

					for(bandwidth = minbandwidth; bandwidth <= maxbandwidth; bandwidth += stepbandwidth)
					{

						for(transmission = mintransmission; transmission <= maxtransmission; transmission += steptransmission)
						{

							for(guardinterval = minguardinterval; guardinterval <= maxguardinterval; guardinterval += stepguardinterval)
							{

								for(hierarchy = minhierarchy; hierarchy <= maxhierarchy; hierarchy += stephierarchy)
								{
									scaninfo.blindcount++;

									tpnode = createtransponder(99, scaninfo.fenode->feinfo->type, scaninfo.orbitalpos, frequency, INVERSION_AUTO, bandwidth, lp, hp, modulation, guardinterval, transmission, hierarchy);

									debug(500, "blindscan: frequ=%d, modulation=%d, hp=%d, lp=%d, bandwidth=%d, guardinterval=%d, transmission=%d, hierarchy=%d, orbitalpos=%d", frequency, modulation, hp, lp, bandwidth, guardinterval, transmission, hierarchy, scaninfo.orbitalpos);

									if(tpnode != NULL)
									{

										fenode = fegetfree(tpnode, 0, scaninfo.fenode, NULL);
										if(fenode == NULL )
										{
											debug(500, "Frontend for scan not free");
											deltransponderbyid(99);
											continue;
										}

										//frontend tune
										if(fenode->feinfo->type == FE_OFDM)
										{
											feset(fenode, tpnode);
											fetunedvbs(fenode, tpnode);
										}
										else
										{
											debug(500, "Frontend type unknown");
											deltransponderbyid(99);
											continue;
										}

										festatus = fewait(fenode);
										if(debug_level == 200) fereadstatus(fenode);
										if(festatus != 0)
										{
											debug(500, "tuning failed");
											deltransponderbyid(99);
											continue;
										}

										buf = dvbgetsdt(fenode, 0, timeout);
										transponderid = findtransponderid(fenode, buf);
										free(buf); buf = NULL;

										if(transponderid == 0 || gettransponder(transponderid) != NULL)
										{
											deltransponderbyid(99);
											continue;
										}

										if(tpnode->id != transponderid)
										{
											struct transponder* tphelp = gettransponder(transponderid);
											if(tphelp != NULL)
											{
												deltransponderbyid(transponderid);
												debug(500, "delete old tid: %llu", transponderid);
											}
											scaninfo.newblindcount++;
											changetransponderid(tpnode, transponderid);
											status.writetransponder = 1;
										}
									}
									deltransponderbyid(99);
									if(timernode->aktion != START) break;
								}
								if(timernode->aktion != START) break;
							}
							if(timernode->aktion != START) break;
						}
						if(timernode->aktion != START) break;
					}
					if(timernode->aktion != START) break;
				}
				if(timernode->aktion != START) break;
			}
			if(timernode->aktion != START) break;
		}
		if(timernode->aktion != START) break;
	}

	return 0;
}

void blindscan(struct stimerthread* timernode)
{
	if(scaninfo.fenode == NULL) return;

	if(scaninfo.fenode->feinfo->type == FE_QPSK)
		satblindscan(timernode, 0);
	else if(scaninfo.fenode->feinfo->type == FE_QAM)
		cableblindscan(timernode, 0);
	else if(scaninfo.fenode->feinfo->type == FE_OFDM)
		terrblindscan(timernode, 0);
}

void doscan(struct stimerthread* timernode)
{
	int festatus = 0, secnr = 0;
	uint8_t lastsecnr = 0xff;
	unsigned char* buf = NULL;
	struct transponder* tpnode = NULL;
	struct dvbdev* fenode = NULL;
	//struct channel* chnode = NULL;
	struct sat* satnode = sat;
	int nitscan = 1;

	if(scaninfo.fenode == NULL || scaninfo.tpnode == NULL || timernode == NULL)
	{
		scaninfo.threadend = 1;
		return;
	}

	debug(500, "channel scan thread start");

	//sat loop
	satnode = sat;
	while(satnode != NULL && timernode->aktion == START)
	{
		if(satnode->scan == 0)
		{
			satnode = satnode->next;
			continue;
		}
		scaninfo.satname = satnode->name;

		if(scaninfo.scantype == 0)
			tpnode = scaninfo.tpnode;
		else if(scaninfo.scantype == 1)
			tpnode = transponder;
		else if(scaninfo.scantype == 2 || scaninfo.scantype == 3)
		{
			tpnode = transponder;
			scaninfo.orbitalpos = satnode->orbitalpos;
		}

		if(scaninfo.blindscan == 1) blindscan(timernode);

		nitscan = 1;
		//transponder loop
		while(tpnode != NULL && timernode->aktion == START)
		{
			if(scaninfo.orbitalpos != tpnode->orbitalpos)
			{
				tpnode = tpnode->next;
				if(scaninfo.scantype == 0) break;
				continue;
			}

			fenode = fegetfree(tpnode, 0, scaninfo.fenode, NULL);
			if(fenode == NULL || (scaninfo.scantype != 3 && fenode != scaninfo.fenode))
			{
				scaninfo.tpcount++;
				tpnode = tpnode->next;
				debug(500, "Frontend for scan not free");
				if(scaninfo.scantype == 0) break;
				continue;
			}

			//frontend tune
			if(fenode->feinfo->type == FE_QPSK)
			{
				feset(fenode, tpnode);
				//workaround fur fec Tuner um ein gutes Ergenis zu erziehlen
				if(ostrstr(fenode->feinfo->name, "BCM45208") != NULL || fbctuner == 1)
				{
					debug(200, "workaround for fbc... set fec and pilot to auto");
					tpnode->fec = 0;   //FEC_AUTO
					tpnode->pilot = 2; //PILOT_AUTO
				}
				if(fetunedvbs(fenode, tpnode) != 0)
				{
					scaninfo.tpcount++;
					if(scaninfo.cleartransponder == 1)
					{
						debug(500, "delete tuning failed id=%llu freq=%d orbitalpos=%d tpnode=%p", tpnode->id, tpnode->frequency, tpnode->orbitalpos, tpnode);
						deltransponderonscan(tpnode->id, tpnode->frequency, tpnode->orbitalpos);
					}
					tpnode = tpnode->next;
					debug(500, "tuning failed");
					if(scaninfo.scantype == 0) break;
					continue;
				}
			}
			else if(fenode->feinfo->type == FE_QAM)
			{
				if(fetunedvbc(fenode, tpnode) != 0)
				{
					scaninfo.tpcount++;
					if(scaninfo.cleartransponder == 1)
					{
						debug(500, "delete tuning failed id=%llu freq=%d orbitalpos=%d tpnode=%p", tpnode->id, tpnode->frequency, tpnode->orbitalpos, tpnode);
						deltransponderonscan(tpnode->id, tpnode->frequency, tpnode->orbitalpos);
					}
					tpnode = tpnode->next;
					debug(500, "tuning failed");
					if(scaninfo.scantype == 0) break;
					continue;
				}
			}
			else if(fenode->feinfo->type == FE_OFDM)
			{
				if(fetunedvbt(fenode, tpnode) != 0)
				{
					scaninfo.tpcount++;
					if(scaninfo.cleartransponder == 1)
					{
						debug(500, "delete tuning failed id=%llu freq=%d orbitalpos=%d tpnode=%p", tpnode->id, tpnode->frequency, tpnode->orbitalpos, tpnode);
						deltransponderonscan(tpnode->id, tpnode->frequency, tpnode->orbitalpos);
					}
					tpnode = tpnode->next;
					debug(500, "tuning failed");
					if(scaninfo.scantype == 0) break;
					continue;
				}
			}
			else
			{
				scaninfo.tpcount++;
				tpnode = tpnode->next;
				debug(500, "Frontend type unknown");
				if(scaninfo.scantype == 0) break;
				continue;
			}
			festatus = fewait(fenode);
			printf("----> end fewait: %d", festatus);
			if(debug_level == 200) fereadstatus(fenode);
			/*if(fenode->feinfo->type == FE_OFDM && festatus != 0 &&  tpnode->system == 0) //DVB-T2 scan
			{
				tpnode->system = 1;
				debug(200, "scan after DVB-T, DVB-T2");
				if(fetunedvbt(fenode, tpnode) != 0)
					festatus = -1;
				else
					festatus = fewait(fenode);
			}
			if(debug_level == 200) fereadstatus(fenode);*/
			if(festatus != 0)
			{
				scaninfo.tpcount++;
				if(scaninfo.cleartransponder == 1)
				{
					debug(500, "delete tuning failed id=%llu freq=%d orbitalpos=%d tpnode=%p", tpnode->id, tpnode->frequency, tpnode->orbitalpos, tpnode);
					deltransponderonscan(tpnode->id, tpnode->frequency, tpnode->orbitalpos);
				}
				tpnode = tpnode->next;
				debug(500, "tuning failed last");
				if(scaninfo.scantype == 0) break;
				continue;
			}

			//del channels from transponder if selected
			//if(scaninfo.scantype != 3 && scaninfo.clear == 1)
			//{
			//	struct channel* tmpchannel = NULL;
			//	chnode = channel;
			//	while(chnode != NULL)
			//	{
			//		tmpchannel = chnode->next;
			//		if(chnode->transponder == tpnode)
			//			delchannel(chnode->serviceid, chnode->transponderid, 1);
			//		chnode = tmpchannel;
			//	}
			//}

			//get nit
			if(scaninfo.networkscan == 1 && nitscan == 1)
			{
				lastsecnr = 0xff;
				secnr = 0;
				while(secnr <= lastsecnr && secnr <= 256)
				{
					if(timernode->aktion != START) break;
					buf = dvbgetnit(fenode, secnr, scaninfo.timeout * 2);
					if(buf != NULL)
					{
						parsenit(buf, &lastsecnr, satnode->orbitalpos);
						nitscan = 1; //1 = scan all transponder for nit / 0 = scan only first transponder for nit
					}
					else
						break;
					free(buf); buf = NULL;
					secnr++;
				}
			}

			lastsecnr = 0xff;
			secnr = 0;
			while(secnr <= lastsecnr && secnr <= 256)
			{
				if(timernode->aktion != START) break;
#ifndef SIMULATE
				if(checkrealbox("DM920") == 1)
					buf = dvbgetsdt(fenode, secnr, scaninfo.timeout * 2);
				else
					buf = dvbgetsdt(fenode, secnr, scaninfo.timeout);
#endif
				if(buf != NULL)
					findchannel(fenode, tpnode, buf, &lastsecnr, scaninfo.scanscreen, scaninfo.listbox, scaninfo.changename, 0);
				else
					break;
				free(buf); buf = NULL;
				secnr++;
			}
			printf("----> end getsdt: %d", secnr);

			scaninfo.tpcount++;
			if(scaninfo.scantype == 0) break;
			tpnode = tpnode->next;
		}
		if(scaninfo.scantype == 0 || scaninfo.scantype == 1) break;
		satnode = satnode->next;
	}
	scaninfo.threadend = 1;
	debug(500, "channel scan thread end");
}

void scanaddchannel(struct skin* node, int scantype, struct transponder* tp1, int ichangename)
{
	int serviceid = 0;
	uint64_t transponderid = 0;
	int servicetype = 0;
	int providerid = 0;
	struct provider* providernode = NULL;
	char* tmpstr = NULL;
	struct transponder* tp2 = NULL;
	struct channel* chnode = NULL;

	if(node == NULL || node->name == NULL) return;

	serviceid = ((uint64_t*)node->name)[0];
	transponderid = ((uint64_t*)node->name)[1];
	servicetype = ((uint64_t*)node->name)[2];

	chnode = getchannel(serviceid, transponderid);
	if(chnode == NULL || (chnode != NULL && chnode->transponder == NULL))
	{
		//check if provider valid
		providernode = getproviderbyname(node->param1);
		if(providernode != NULL)
			providerid = providernode->providerid;
		else
		{
			providerid = getlastproviderid() + 1;
			tmpstr = ostrcat(tmpstr, oitoa(providerid), 1, 1);
			tmpstr = ostrcat(tmpstr, "#", 1, 0);
			tmpstr = ostrcat(tmpstr, node->param1, 1, 0);
			tmpstr = ostrcat(tmpstr, "#0", 1, 0);
			addprovider(tmpstr, 1, NULL);
			free(tmpstr); tmpstr = NULL;
		}

		//check if transponder valid
		if(scantype == 0)
		{
			tp2 = gettransponder(transponderid);

			if(tp2 == NULL && tp1 != NULL)
			{
				tp2 = gettransponderbydetail(0, tp1->fetype, tp1->orbitalpos, tp1->frequency, tp1->inversion, tp1->symbolrate, tp1->polarization, tp1->fec, tp1->modulation, tp1->rolloff, tp1->pilot, tp1->system, 1);
				if(tp2 != NULL)
					changetransponderid(tp2, transponderid);
			}

			if(tp2 == NULL) //create new transponder
				copytransponder(tp1, tp2, transponderid);
			else //change transponder
				copytransponder(tp1, tp2, 99);
		}

		if(servicetype != 0 && servicetype != 1)
			servicetype = 0;

		if(chnode == NULL)
		{
			if(createchannel(node->param2, transponderid, providerid, serviceid, servicetype, 0, -1, -1, -1, -1, 0, -1, NULL, NULL) != NULL)
				node->fontcol = convertcol("deaktivcol");
		}
		else
			node->fontcol = convertcol("deaktivcol");
	}
	else
	{
		node->fontcol = convertcol("deaktivcol");
		//change channel name if selected
		if(ichangename == 1 && node->param2 != NULL && strlen(node->param2) > 0 && ostrcmp(node->param2, chnode->name) != 0)
		{
			free(chnode->name);
			chnode->name = ostrcat(node->param2, NULL, 0, 0);
			status.writetransponder = 1;
		}
	}
}

void scansetallsat(int fetype)
{
	int i = 0, orbitalpos = 0;
	struct sat* satnode = NULL;
	struct dvbdev* dvbnode = dvbdev;
	char* tmpstr = NULL, *tmpnr = NULL;

	while(dvbnode != NULL)
	{
		if(dvbnode->type == FRONTENDDEV && dvbnode->feinfo != NULL && dvbnode->felock < 1 && dvbnode->deactive == 0 && (fetype == -1 || dvbnode->feinfo->type == fetype))
		{

			tmpstr = ostrcat(dvbnode->feshortname, "_sat", 0, 0);
			for(i = 1; i <= getmaxsat(dvbnode->feshortname); i++)
			{
				tmpnr = oitoa(i);
				orbitalpos = getconfigint(tmpstr, tmpnr);
				free(tmpnr); tmpnr = NULL;

				satnode = getsatbyorbitalpos(orbitalpos);
				if(satnode != NULL && (fetype == -1 || satnode->fetype == fetype))
					satnode->scan = 1;
			}
			free(tmpstr); tmpstr = NULL;
		}
		dvbnode = dvbnode->next;
	}
}

void scansetmultisat(struct skin* scan)
{
	struct skin* node = scan;
	struct sat* satnode = NULL;

	while(node != NULL && node->name != NULL)
	{
		if(ostrcmp(node->ret, "1") == 0)
		{
			satnode = getsatbyorbitalpos(atoi(node->name));
			if(satnode != NULL)
				satnode->scan = 1;
		}
		node = node->next;
	}
}

void delchannelbysat(int orbitalpos)
{
	struct transponder* transpondernode = transponder;
	struct channel* chnode = NULL;

	while(transpondernode != NULL)
	{
		if(transpondernode->orbitalpos == orbitalpos)
		{
			struct channel* tmpchannel = NULL;
			chnode = channel;
			while(chnode != NULL)
			{
				tmpchannel = chnode->next;
				if(chnode->transponder == transpondernode)
					delchannel(chnode->serviceid, chnode->transponderid, 1);
				chnode = tmpchannel;
			}
		}
		transpondernode = transpondernode->next;
	}
}

void delchannelbymultisat()
{
	struct sat* satnode = sat;

	while(satnode != NULL)
	{
		if(satnode->scan == 1)
			delchannelbysat(satnode->orbitalpos);
		satnode = satnode->next;
	}
}

void screenscan(struct transponder* transpondernode, struct skin* mscan, char* tuner, int scantype, int orbitalpos, unsigned int frequency, int inversion, unsigned int symbolrate, int polarization, int fec, int modulation, int rolloff, int pilot, int networkscan, int onlyfree, int clear, int blindscan, int ichangename, int system, int favtype, int emptybouquet, int unusedbouquetchannels, int unusedsatprovider, int cleartransponder, int timeout, int flag)
{
	int rcret = 0, tpmax = 0, i = 0, alladded = 0, endmsgshow = 0, tpdel = 0;

	struct skin* scan = NULL;
	if(flag == 1)
		scan = getscreen("scanauto");
	else
		scan = getscreen("scanmanual");

	struct skin* progress = getscreennode(scan, "progress");
	struct skin* listbox = getscreennode(scan, "listbox");
	struct skin* satname = getscreennode(scan, "satname");
	struct skin* tpcount = getscreennode(scan, "tpcount");
	struct skin* foundtv = getscreennode(scan, "foundtv");
	struct skin* foundradio = getscreennode(scan, "foundradio");
	struct skin* founddata = getscreennode(scan, "founddata");
	struct skin* foundblind = getscreennode(scan, "foundblind");
	struct skin* b2 = getscreennode(scan, "b2");
	struct skin* b3 = getscreennode(scan, "b3");
	struct skin* load = getscreen("loading");
	struct transponder* tpnode = NULL;
	struct dvbdev* fenode = NULL, *dvbnode = dvbdev;
	struct stimerthread* timernode = NULL;
	struct sat* satnode = NULL;
	struct channel* chnode = NULL;
	char* tmpstr = NULL;

	listbox->aktline = 1;
	listbox->aktpage = -1;
	progress->progresssize = 0;
	memset(&scaninfo, 0, sizeof(struct scaninfo));

	b2->hidden = NO;
	b3->hidden = NO;

	addscreenrc(scan, listbox);
	if(scantype != 3)
	{
		while(dvbnode != NULL)
		{
			if(dvbnode->type == FRONTENDDEV && dvbnode->feinfo != NULL && dvbnode->felock < 1)
			{
				if(ostrcmp(dvbnode->feshortname, tuner) == 0)
				{
					fenode = dvbnode;
					break;
				}
			}
			dvbnode = dvbnode->next;
		}
		if(fenode == NULL) return;
	}
	else if(scantype == 3)
		fenode = dvbdev;

	if(scantype == 0) //single transponder
	{
		if(fenode != NULL && fenode->feinfo != NULL)
		{
			//del channels from transponder if selected
			if(clear == 1)
			{
				struct channel* tmpchannel = NULL;
				chnode = channel;
				while(chnode != NULL)
				{
					tmpchannel = chnode->next;
					if(chnode->transponder == transpondernode)
						delchannel(chnode->serviceid, chnode->transponderid, 1);
					chnode = tmpchannel;
				}
			}

			debug(500, "fetype=%d, orbitalpos=%d, frequ=%d, inverion=%d, symbolrate=%d, polarization=%d, fec=%d, modulation=%d, rolloff=%d, pilot=%d, system=%d", fenode->feinfo->type, orbitalpos, frequency, inversion, symbolrate, polarization, fec, modulation, rolloff, pilot, system);
			tpnode = createtransponder(99, fenode->feinfo->type, orbitalpos, frequency, inversion, symbolrate, polarization, fec, modulation, rolloff, pilot, system);
		}
		if(tpnode != NULL)
		{
			tpmax = 1;
			satnode = getsatbyorbitalpos(tpnode->orbitalpos);
			if(satnode != NULL) satnode->scan = 1;
		}
	}
	else if(scantype == 1) //single sat
	{
		if(clear == 1) delchannelbysat(orbitalpos);
		tpnode = transponder;
		while(tpnode != NULL)
		{
			if(tpnode->orbitalpos == orbitalpos)
				tpmax++;
			tpnode = tpnode->next;
		}
		tpnode = transponder;
		satnode = getsatbyorbitalpos(orbitalpos);
		if(satnode != NULL) satnode->scan = 1;
	}
	else if(scantype == 2 || scantype == 3) //2: multi sat, 3: all
	{
		if(scantype == 2)
		{
			scansetmultisat(mscan);
			if(clear == 1) delchannelbymultisat();
		}
		if(scantype == 3)
		{
			scansetallsat(-1);
			b2->hidden = YES;
			b3->hidden = YES;
			//del all channel for auto. search
			if(clear == 1)
			{
				freechannel(1);
				if(unusedsatprovider == 1)
				{
					delunusedsat();
					delunusedtransponder();
					delunusedchannel();
					//delunusedepgchannel();
					delunusedprovider();
				}
			}
		}

		satnode = sat;
		while(satnode != NULL)
		{
			if(satnode->scan != 0)
			{
				tpnode = transponder;
				while(tpnode != NULL)
				{
					if(tpnode->orbitalpos == satnode->orbitalpos)
						tpmax++;
					tpnode = tpnode->next;
				}
			}
			satnode = satnode->next;
		}
		tpnode = transponder;
	}

	scaninfo.fenode = fenode;
	scaninfo.tpnode = tpnode;
	scaninfo.scanscreen = scan;
	scaninfo.listbox = listbox;
	scaninfo.timeout = timeout;
	scaninfo.scantype = scantype;
	scaninfo.orbitalpos = orbitalpos;
	scaninfo.onlyfree = onlyfree;
	scaninfo.networkscan = networkscan;
	scaninfo.blindscan = blindscan;
	scaninfo.changename = ichangename;
	scaninfo.clear = clear;
	scaninfo.tpmax = tpmax;
	scaninfo.tpdel = tpdel;
	scaninfo.cleartransponder = cleartransponder;
	timernode = addtimer(&doscan, START, 1000, 1, NULL, NULL, NULL);

	while(1)
	{
		//satname
		tmpstr = ostrcat(tmpstr, _("Sat/Provider: "), 1, 0);
		tmpstr = ostrcat(tmpstr, scaninfo.satname, 1, 0);
		changetext(satname, tmpstr);
		free(tmpstr); tmpstr = NULL;

		//transpondercount
		tmpstr = ostrcat(tmpstr, _("Transponder: "), 1, 0);
		tmpstr = ostrcat(tmpstr, oitoa(scaninfo.tpcount), 1, 1);
		tmpstr = ostrcat(tmpstr, " / ", 1, 0);
		tmpstr = ostrcat(tmpstr, oitoa(scaninfo.tpmax), 1, 1);
		tmpstr = ostrcat(tmpstr, _(" New: "), 1, 0);
		tmpstr = ostrcat(tmpstr, oitoa(scaninfo.tpnew), 1, 1);
		tmpstr = ostrcat(tmpstr, _(" Del: "), 1, 0);
		tmpstr = ostrcat(tmpstr, oitoa(scaninfo.tpdel), 1, 1);
		changetext(tpcount, tmpstr);
		free(tmpstr); tmpstr = NULL;

		//tvcount
		tmpstr = ostrcat(tmpstr, _("TV: "), 1, 0);
		tmpstr = ostrcat(tmpstr, oitoa(scaninfo.newtvcount), 1, 1);
		tmpstr = ostrcat(tmpstr, " / ", 1, 0);
		tmpstr = ostrcat(tmpstr, oitoa(scaninfo.tvcount), 1, 1);
		changetext(foundtv, tmpstr);
		free(tmpstr); tmpstr = NULL;

		//radiocount
		tmpstr = ostrcat(tmpstr, _("Radio: "), 1, 0);
		tmpstr = ostrcat(tmpstr, oitoa(scaninfo.newradiocount), 1, 1);
		tmpstr = ostrcat(tmpstr, " / ", 1, 0);
		tmpstr = ostrcat(tmpstr, oitoa(scaninfo.radiocount), 1, 1);
		changetext(foundradio, tmpstr);
		free(tmpstr); tmpstr = NULL;

		//datacount
		tmpstr = ostrcat(tmpstr, _("Data: "), 1, 0);
		tmpstr = ostrcat(tmpstr, oitoa(scaninfo.newdatacount), 1, 1);
		tmpstr = ostrcat(tmpstr, " / ", 1, 0);
		tmpstr = ostrcat(tmpstr, oitoa(scaninfo.datacount), 1, 1);
		changetext(founddata, tmpstr);
		free(tmpstr); tmpstr = NULL;

		//blindcount
		tmpstr = ostrcat(tmpstr, _("Blindscan: "), 1, 0);
		tmpstr = ostrcat(tmpstr, oitoa(scaninfo.newblindcount), 1, 1);
		tmpstr = ostrcat(tmpstr, " / ", 1, 0);
		tmpstr = ostrcat(tmpstr, oitoa(scaninfo.blindcount), 1, 1);
		tmpstr = ostrcat(tmpstr, " / ", 1, 0);
		tmpstr = ostrcat(tmpstr, oitoa(scaninfo.blindmax), 1, 1);
		changetext(foundblind, tmpstr);
		free(tmpstr); tmpstr = NULL;

		if(scaninfo.tpmax == 0)
			progress->progresssize = 100;
		else
			progress->progresssize = (100 / (float)scaninfo.tpmax) * scaninfo.tpcount;

		drawscreen(scan, 0, 0);
		rcret = waitrc(scan, 1000, 0);

		if(scantype != 3 && scaninfo.threadend == 1 && endmsgshow == 0)
		{
			if(scaninfo.tvcount + scaninfo.radiocount + scaninfo.datacount == 0)
				textbox(_("Message"), _("Channel scan ended.\nNothing found."), _("OK"), getrcconfigint("rcok", NULL), NULL, 0, NULL, 0, NULL, 0, 800, 200, 0, 0);
			else
				textbox(_("Message"), _("Channel scan ended."), _("OK"), getrcconfigint("rcok", NULL), NULL, 0, NULL, 0, NULL, 0, 800, 200, 0, 0);
			endmsgshow = 1;
		}

		if(scantype != 3 && rcret == getrcconfigint("rcred", NULL))
			scanaddchannel(listbox->select, scantype, tpnode, ichangename);

		if((scantype != 3 && rcret == getrcconfigint("rcgreen", NULL)) || (scantype == 3 && scaninfo.threadend == 1 && alladded < 2))
		{
			struct skin* lnode = listbox;

			long deaktivcol = convertcol("deaktivcol");
			if(scantype == 3 && alladded == 0)
			{
				alladded = 1;
				continue;
			}
			alladded = 2;

			drawscreen(load, 0, 0);
			while(lnode != NULL)
			{
				if(lnode->fontcol != deaktivcol && lnode->del == 1)
					scanaddchannel(lnode, scantype, tpnode, ichangename);
				lnode = lnode->next;
			}
			clearscreen(load);
			if(scaninfo.tvcount + scaninfo.radiocount + scaninfo.datacount == 0)
				textbox(_("Message"), _("Channel scan ended.\nNothing found."), _("OK"), getrcconfigint("rcok", NULL), NULL, 0, NULL, 0, NULL, 0, 800, 200, 0, 0);
			else
				textbox(_("Message"), _("Scan completed, changes have been saved."), _("OK"), getrcconfigint("rcok", NULL), NULL, 0, NULL, 0, NULL, 0, 600, 200, 0, 0);
		}

		if(rcret == getrcconfigint("rcexit", NULL))
		{
			if(timernode != NULL && scaninfo.threadend == 0)
			{
				timernode->aktion = STOP;
				while(i < 4 && scaninfo.threadend == 0)
				{
					textbox(_("Message"), _("Wait for channel scan end"), NULL, 0, NULL, 0, NULL, 0, NULL, 0, 800, 200, 3, 0);
					i++;
				}
			}
			break;
		}
	}

	if(scantype == 0) deltransponderbyid(99);
	if(clear == 1)
	{
		// favtype 0 = unchanged
		// favtype 1 = create new
		// favtype 2 = delete

		if(favtype == 1)
		{
			freemainbouquet(1);
			struct provider *pnode = provider;

			while(pnode != NULL)
			{
				provider2bouquet(pnode->providerid, 1);
				pnode = pnode->next;
			}
		}
		else if(favtype == 2)
			freemainbouquet(1);

		if(emptybouquet == 1)
			delemptymainbouquet(1);

		if(unusedbouquetchannels == 1)
			delunusedbouquetchannels(0);
		else
			delunusedbouquetchannels(1);

		if(cleartransponder == 1 && unusedsatprovider == 1)
			deltransponderbyid(0);
	}
	delmarkedscreennodes(scan, 1);
	delownerrc(scan);
	clearscreen(scan);
	resetsatscan();
	drawscreen(load, 0, 0);
	sortchannel();
	sortprovider();
	clearscreen(load);
}

void changescantype(char* scantype, struct skin* scan, struct skin* listbox, struct skin* tuner, struct skin* satellite, struct skin* id, struct skin* system, struct skin* frequency, struct skin* inversion, struct skin* symbolrate, struct skin* polarization, struct skin* fec, struct skin* modulation, struct skin* rolloff, struct skin* pilot, struct skin* hp, struct skin* lp, struct skin* bandwidth, struct skin* transmission, struct skin* guardinterval, struct skin* hierarchy, struct skin* b4, struct skin* b5, int flag)
{
	struct sat* satnode = sat;
	struct skin* tmp = NULL;
	char* tmpstr = NULL, *tmpstr1 = NULL, *tmpnr = NULL;
	char* feshortname = NULL;
	int i, orbitalpos = 0;

	if(tuner != NULL) feshortname = tuner->ret;

	tuner->hidden = NO;
	id->hidden = NO;
	system->hidden = NO;
	frequency->hidden = NO;
	inversion->hidden = NO;
	symbolrate->hidden = NO;
	polarization->hidden = NO;
	fec->hidden = NO;
	modulation->hidden = NO;
	rolloff->hidden = NO;
	pilot->hidden = NO;
	hp->hidden = NO;
	lp->hidden = NO;
	bandwidth->hidden = NO;
	transmission->hidden = NO;
	guardinterval->hidden = NO;
	hierarchy->hidden = NO;
	satellite->hidden = NO;
	b4->hidden = NO;
	b5->hidden = NO;
	delmarkedscreennodes(scan, 1);

	if(flag == 0 && ostrcmp(scantype, "0") == 0 && ostrcmp(system->ret, "0") == 0)
	{
		modulation->hidden = YES;
		rolloff->hidden = YES;
		pilot->hidden = YES;
	}

	if(ostrcmp(scantype, "1") == 0 || ostrcmp(scantype, "2") == 0 || ostrcmp(scantype, "3") == 0)
	{
		id->hidden = YES;
		system->hidden = YES;
		frequency->hidden = YES;
		inversion->hidden = YES;
		symbolrate->hidden = YES;
		polarization->hidden = YES;
		fec->hidden = YES;
		modulation->hidden = YES;
		rolloff->hidden = YES;
		pilot->hidden = YES;
		hp->hidden = YES;
		lp->hidden = YES;
		bandwidth->hidden = YES;
		transmission->hidden = YES;
		guardinterval->hidden = YES;
		hierarchy->hidden = YES;
		b4->hidden = YES;
		b5->hidden = YES;
	}
	if(ostrcmp(scantype, "2") == 0 && feshortname != NULL)
	{
		satellite->hidden = YES;

		tmpstr = ostrcat(feshortname, "_sat", 0, 0);
		for(i = 1; i <= getmaxsat(feshortname); i++)
		{
			tmpnr = oitoa(i);
			orbitalpos = getconfigint(tmpstr, tmpnr);
			free(tmpnr); tmpnr = NULL;

			satnode = getsatbyorbitalpos(orbitalpos);
			if(satnode != NULL && satnode->fetype == FE_QPSK)
			{
				tmp = addlistbox(scan, listbox, tmp, 1);
				if(tmp != NULL)
				{
					tmpstr1 = oitoa(satnode->orbitalpos);
					changename(tmp, tmpstr1);
					free(tmpstr1); tmpstr1 = NULL;
					changetext(tmp, satnode->name);
					tmp->type = CHOICEBOX;
					addchoicebox(tmp, "0", _("no"));
					addchoicebox(tmp, "1", _("yes"));
				}
			}
		}
		free(tmpstr); tmpstr = NULL;
	}
	if(ostrcmp(scantype, "3") == 0)
	{
		tuner->hidden = YES;
		satellite->hidden = YES;
	}

	if(flag == 2)
	{
		system->hidden = YES;
		inversion->hidden = YES;
		polarization->hidden = YES;
		rolloff->hidden = YES;
		pilot->hidden = YES;
		satellite->hidden = YES;
	}

	if(flag == 3)
	{
		system->hidden = YES;
		polarization->hidden = YES;
		rolloff->hidden = YES;
		pilot->hidden = YES;
		satellite->hidden = YES;
		symbolrate->hidden = YES;
		fec->hidden = YES;
	}
	else
	{
		hp->hidden = YES;
		lp->hidden = YES;
		bandwidth->hidden = YES;
		transmission->hidden = YES;
		guardinterval->hidden = YES;
		hierarchy->hidden = YES;
	}
}

void scanchangesat(struct skin* sat, struct transponder* tpnode, char* feshortname)
{
	char* tmpstr = NULL;

	changeinput(sat, NULL);
	changechoiceboxvalue(sat, NULL);
	tmpstr = getsatstring(feshortname, FE_QPSK);
	changeinput(sat, tmpstr);
	free(tmpstr); tmpstr = NULL;
	tmpstr = getorbitalposstring(feshortname, FE_QPSK);
	changechoiceboxvalue(sat, tmpstr);
	free(tmpstr); tmpstr = NULL;
	if(tpnode != NULL)
	{
		tmpstr = oitoa(tpnode->orbitalpos);
		setchoiceboxselection(sat, tmpstr);
		free(tmpstr); tmpstr = NULL;
	}
}

//flag 0: manual scan (DVB-S)
//flag 1: auto scan (all)
//flag 2: manual scan (DVB-C)
//flag 3: manual scan (DVB-T)
void screenscanconfig(int flag)
{
	int rcret = 0, fetype = -1;
	unsigned int ifrequency = -1, isymbolrate = -1;
	int iscantype = -1, isat = -1;
	int iinversion = -1, ipolarization = -1;
	int ifec = -1, imodulation = -1, irolloff = -1, ipilot = -1, isystem = -1;
	int inetworkscan = -1, ionlyfree = -1, iclear = -1, iblindscan = -1, ichangename = -1;
	int ifavtype = -1, iemptybouquet = -1, iunusedbouquetchannels = -1;
	int iunusedsatprovider = -1, icleartransponder = -1;

	int i = 0, treffer = 0, tunercount = 0;

	struct skin* scan = NULL;
	if(flag == 1)
		scan = getscreen("scanadjustauto");
	else
		scan = getscreen("scanadjustmanual");

	struct skin* listbox = getscreennode(scan, "listbox");
	struct skin* tuner = getscreennode(scan, "tuner");
	struct skin* scantype = getscreennode(scan, "scantype");
	struct skin* sat = getscreennode(scan, "sat");
	struct skin* id = getscreennode(scan, "id");
	struct skin* system = getscreennode(scan, "system");
	struct skin* frequency = getscreennode(scan, "frequency");
	struct skin* inversion = getscreennode(scan, "inversion");
	struct skin* symbolrate = getscreennode(scan, "symbolrate");
	struct skin* polarization = getscreennode(scan, "polarization");
	struct skin* fec = getscreennode(scan, "fec");
	struct skin* modulation = getscreennode(scan, "modulation");
	struct skin* rolloff = getscreennode(scan, "rolloff");
	struct skin* pilot = getscreennode(scan, "pilot");
	struct skin* hp = getscreennode(scan, "hp");
	struct skin* lp = getscreennode(scan, "lp");
	struct skin* bandwidth = getscreennode(scan, "bandwidth");
	struct skin* transmission = getscreennode(scan, "transmission");
	struct skin* guardinterval = getscreennode(scan, "guardinterval");
	struct skin* hierarchy = getscreennode(scan, "hierarchy");
	struct skin* networkscan = getscreennode(scan, "networkscan");
	struct skin* clear = getscreennode(scan, "clear");
	struct skin* onlyfree = getscreennode(scan, "onlyfree");
	struct skin* blindscan = getscreennode(scan, "blindscan");
	struct skin* changename = getscreennode(scan, "changename");
	struct skin* favtype = getscreennode(scan, "favtype");
	struct skin* emptybouquet = getscreennode(scan, "emptybouquet");
	struct skin* unusedsatprovider = getscreennode(scan, "unusedsatprovider");
	struct skin* cleartransponder = getscreennode(scan, "cleartransponder");
	struct skin* unusedbouquetchannels = getscreennode(scan, "unusedbouquetchannels");

	struct skin* b4 = getscreennode(scan, "b4");
	struct skin* b5 = getscreennode(scan, "b5");
	struct skin* tmp = NULL;
	char* tmpstr = NULL, *tmpnr = NULL, *feshortname = NULL;
	struct transponder* tpnode = NULL;
	struct dvbdev* dvbnode = dvbdev;

	listbox->aktline = 1;
	listbox->aktpage = -1;

	if(status.recording > 0 || (status.play != 2 && status.streaming > 0))
	{
		textbox(_("Message"), _("Scan is not allowed if record or stream is running !"), _("OK"), getrcconfigint("rcok", NULL), NULL, 0, NULL, 0, NULL, 0, 800, 200, 0, 0);
		return;
	}

	//del old values
	changeinput(tuner, NULL);
	changechoiceboxvalue(tuner, NULL);
	changeinput(scantype, NULL);
	changechoiceboxvalue(scantype, NULL);
	changeinput(sat, NULL);
	changechoiceboxvalue(sat, NULL);
	changeinput(system, NULL);
	changechoiceboxvalue(system, NULL);
	changeinput(inversion, NULL);
	changechoiceboxvalue(inversion, NULL);
	changeinput(polarization, NULL);
	changechoiceboxvalue(polarization, NULL);
	changeinput(fec, NULL);
	changechoiceboxvalue(fec, NULL);
	changeinput(modulation, NULL);
	changechoiceboxvalue(modulation, NULL);
	changeinput(rolloff, NULL);
	changechoiceboxvalue(rolloff, NULL);
	changeinput(pilot, NULL);
	changechoiceboxvalue(pilot, NULL);
	changeinput(hp, NULL);
	changeinput(lp, NULL);
	changeinput(bandwidth, NULL);
	changeinput(transmission, NULL);
	changeinput(guardinterval, NULL);
	changeinput(hierarchy, NULL);
	changechoiceboxvalue(hp, NULL);
	changechoiceboxvalue(lp, NULL);
	changechoiceboxvalue(bandwidth, NULL);
	changechoiceboxvalue(transmission, NULL);
	changechoiceboxvalue(guardinterval, NULL);
	changechoiceboxvalue(hierarchy, NULL);

	frequency->aktpage = 0;
	symbolrate->aktpage = 0;

	if(flag == 0) fetype = FE_QPSK;
	if(flag == 2) fetype = FE_QAM;
	if(flag == 3) fetype = FE_OFDM;

	//tuner
	while(dvbnode != NULL)
	{
		if(dvbnode->type == FRONTENDDEV && dvbnode->feinfo != NULL && dvbnode->felock < 1 && dvbnode->deactive == 0 && (flag == 1 || dvbnode->feinfo->type == fetype))
		{
			treffer = 0;
			if(dvbnode->feshortname != NULL)
			{
				tmpstr = ostrcat(dvbnode->feshortname, "_sat", 0, 0);
				for(i = 1; i <= getmaxsat(dvbnode->feshortname); i++)
				{
					tmpnr = oitoa(i);
					if(getconfigint(tmpstr, tmpnr) != 0)
					{
						treffer = 1;
						break;
					}
					free(tmpnr); tmpnr = NULL;
				}
				free(tmpnr); tmpnr = NULL;
				free(tmpstr); tmpstr = NULL;
			}

			if(treffer == 0)
			{
				dvbnode = dvbnode->next;
				continue;
			}

			tunercount++;
			if(feshortname == NULL) feshortname = dvbnode->feshortname;

			tmpstr = ostrcat(tmpstr, dvbnode->feinfo->name, 1, 0);
			tmpstr = ostrcat(tmpstr, " (", 1, 0);
			tmpstr = ostrcat(tmpstr, oitoa(dvbnode->adapter), 1, 1);
			tmpstr = ostrcat(tmpstr, "/", 1, 0);
			tmpstr = ostrcat(tmpstr, oitoa(dvbnode->devnr), 1, 1);
			tmpstr = ostrcat(tmpstr, ")", 1, 0);
			addchoicebox(tuner, dvbnode->feshortname, tmpstr);
			free(tmpstr); tmpstr = NULL;
		}
		dvbnode = dvbnode->next;
	}

	if(tunercount < 1)
	{
		textbox(_("Message"), _("No Tuner configured"), _("OK"), getrcconfigint("rcok", NULL), NULL, 0, NULL, 0, NULL, 0, 800, 200, 0, 0);
		return;
	}

	rcret = servicestop(status.aktservice, 1, 1);
	if(rcret == 1) return;

	if(status.aktservice->channel != NULL)
		tpnode = status.aktservice->channel->transponder;

	//clear akt and last channel, so all channel can delete
	freechannelhistory();
	status.lastservice->channel = NULL;
	status.aktservice->channel = NULL;

start:

	if(flag == 0)
	{
		addchoicebox(scantype, "0", _("Single Transponder"));
		addchoicebox(scantype, "1", _("Single Sat"));
		addchoicebox(scantype, "2", _("Multi Sat"));
		setchoiceboxselection(scantype, "0");
	}
	else if(flag == 2 || flag == 3)
	{
		addchoicebox(scantype, "0", _("Single Transponder"));
		addchoicebox(scantype, "1", _("Single Provider"));
		setchoiceboxselection(scantype, "0");
	}
	else
	{
		addchoicebox(scantype, "3", _("Auto Scan"));
		setchoiceboxselection(scantype, "3");
	}

	//sat
	scanchangesat(sat, tpnode, feshortname);

	//id
	if(tpnode != NULL)
		tmpstr = ollutoa(tpnode->id);
	changeinput(id, tmpstr);
	free(tmpstr); tmpstr = NULL;

	//system
	tmpstr = transpondergetsystemstr(tpnode, 1);
	changeinput(system, tmpstr);
	free(tmpstr); tmpstr = NULL;
	tmpstr = transpondergetsystemstr(tpnode, 2);
	changechoiceboxvalue(system, tmpstr);
	free(tmpstr); tmpstr = NULL;
	if(tpnode != NULL)
	{
		tmpstr = oitoa(tpnode->system);
		setchoiceboxselection(system, tmpstr);
		free(tmpstr); tmpstr = NULL;
	}

	//frequency
	if(tpnode != NULL)
		tmpstr = oitoa(tpnode->frequency / 1000);
	else
	{
		if(flag == 3)
			tmpstr = ostrcat(tmpstr, "000000", 1, 0);
		else
			tmpstr = ostrcat(tmpstr, "00000", 1, 0);
	}
	if(flag == 2 || flag == 3)
	{
		changemask(frequency, "000000");
		changeinput(frequency, tmpstr);
		frequency->input = mask(frequency->input, 6, "0");
	}
	else
	{
		changemask(frequency, "00000");
		changeinput(frequency, tmpstr);
		frequency->input = mask(frequency->input, 5, "0");
	}
	free(tmpstr); tmpstr = NULL;

	//inversion
	tmpstr = transpondergetinversionstr(tpnode, 1);
	changeinput(inversion, tmpstr);
	free(tmpstr); tmpstr = NULL;
	tmpstr = transpondergetinversionstr(tpnode, 2);
	changechoiceboxvalue(inversion, tmpstr);
	free(tmpstr); tmpstr = NULL;
	if(tpnode != NULL)
	{
		tmpstr = oitoa(tpnode->inversion);
		setchoiceboxselection(inversion, tmpstr);
		free(tmpstr); tmpstr = NULL;
	}

	//symbolrate
	if(tpnode != NULL)
		tmpstr = oitoa(tpnode->symbolrate / 1000);
	else
		tmpstr = ostrcat(tmpstr, "00000", 1, 0);
	changemask(symbolrate, "00000");
	changeinput(symbolrate, tmpstr);
	symbolrate->input = mask(symbolrate->input, 5, "0");
	free(tmpstr); tmpstr = NULL;

	//polarization
	tmpstr = transpondergetpolarizationstr(tpnode, 1);
	changeinput(polarization, tmpstr);
	free(tmpstr); tmpstr = NULL;
	tmpstr = transpondergetpolarizationstr(tpnode, 2);
	changechoiceboxvalue(polarization, tmpstr);
	free(tmpstr); tmpstr = NULL;
	if(tpnode != NULL)
	{
		tmpstr = oitoa(tpnode->polarization);
		setchoiceboxselection(polarization, tmpstr);
		free(tmpstr); tmpstr = NULL;
	}

	//fec
	tmpstr = transpondergetfecstr(tpnode, fetype, 1);
	changeinput(fec, tmpstr);
	free(tmpstr); tmpstr = NULL;
	tmpstr = transpondergetfecstr(tpnode, fetype, 2);
	changechoiceboxvalue(fec, tmpstr);
	free(tmpstr); tmpstr = NULL;
	if(tpnode != NULL)
	{
		tmpstr = oitoa(tpnode->fec);
		setchoiceboxselection(fec, tmpstr);
		free(tmpstr); tmpstr = NULL;
	}

	//modulation
	tmpstr = transpondergetmodulationstr(tpnode, fetype, 1);
	changeinput(modulation, tmpstr);
	free(tmpstr); tmpstr = NULL;
	tmpstr = transpondergetmodulationstr(tpnode, fetype, 2);
	changechoiceboxvalue(modulation, tmpstr);
	free(tmpstr); tmpstr = NULL;
	if(tpnode != NULL)
	{
		tmpstr = oitoa(tpnode->modulation);
		setchoiceboxselection(modulation, tmpstr);
		free(tmpstr); tmpstr = NULL;
	}

	//rolloff
	tmpstr = transpondergetrolloffstr(tpnode, 1);
	changeinput(rolloff, tmpstr);
	free(tmpstr); tmpstr = NULL;
	tmpstr = transpondergetrolloffstr(tpnode, 2);
	changechoiceboxvalue(rolloff, tmpstr);
	free(tmpstr); tmpstr = NULL;
	if(tpnode != NULL)
	{
		tmpstr = oitoa(tpnode->rolloff);
		setchoiceboxselection(rolloff, tmpstr);
		free(tmpstr); tmpstr = NULL;
	}

	//pilot
	tmpstr = transpondergetpilotstr(tpnode, 1);
	changeinput(pilot, tmpstr);
	free(tmpstr); tmpstr = NULL;
	tmpstr = transpondergetpilotstr(tpnode, 2);
	changechoiceboxvalue(pilot, tmpstr);
	free(tmpstr); tmpstr = NULL;
	if(tpnode != NULL)
	{
		tmpstr = oitoa(tpnode->pilot);
		setchoiceboxselection(pilot, tmpstr);
		free(tmpstr); tmpstr = NULL;
	}

	//hp
	tmpstr = transpondergetfecstr(tpnode, fetype, 1);
	changeinput(hp, tmpstr);
	free(tmpstr); tmpstr = NULL;
	tmpstr = transpondergetfecstr(tpnode, fetype, 2);
	changechoiceboxvalue(hp, tmpstr);
	free(tmpstr); tmpstr = NULL;
	if(tpnode != NULL)
	{
		tmpstr = oitoa(tpnode->fec);
		setchoiceboxselection(hp, tmpstr);
		free(tmpstr); tmpstr = NULL;
	}

	//lp
	tmpstr = transpondergetfecstr(tpnode, fetype, 1);
	changeinput(lp, tmpstr);
	free(tmpstr); tmpstr = NULL;
	tmpstr = transpondergetfecstr(tpnode, fetype, 2);
	changechoiceboxvalue(lp, tmpstr);
	free(tmpstr); tmpstr = NULL;
	if(tpnode != NULL)
	{
		tmpstr = oitoa(tpnode->polarization);
		setchoiceboxselection(lp, tmpstr);
		free(tmpstr); tmpstr = NULL;
	}

	//bandwidth
	tmpstr = transpondergetbandwidthstr(tpnode, 1);
	changeinput(bandwidth, tmpstr);
	free(tmpstr); tmpstr = NULL;
	tmpstr = transpondergetbandwidthstr(tpnode, 2);
	changechoiceboxvalue(bandwidth, tmpstr);
	free(tmpstr); tmpstr = NULL;
	if(tpnode != NULL)
	{
		tmpstr = oitoa(tpnode->symbolrate);
		setchoiceboxselection(bandwidth, tmpstr);
		free(tmpstr); tmpstr = NULL;
	}

	//guardinterval
	tmpstr = transpondergetguardintervalstr(tpnode, 1);
	changeinput(guardinterval, tmpstr);
	free(tmpstr); tmpstr = NULL;
	tmpstr = transpondergetguardintervalstr(tpnode, 2);
	changechoiceboxvalue(guardinterval, tmpstr);
	free(tmpstr); tmpstr = NULL;
	if(tpnode != NULL)
	{
		tmpstr = oitoa(tpnode->rolloff);
		setchoiceboxselection(guardinterval, tmpstr);
		free(tmpstr); tmpstr = NULL;
	}

	//transmission
	tmpstr = transpondergettransmissionstr(tpnode, 1);
	changeinput(transmission, tmpstr);
	free(tmpstr); tmpstr = NULL;
	tmpstr = transpondergettransmissionstr(tpnode, 2);
	changechoiceboxvalue(transmission, tmpstr);
	free(tmpstr); tmpstr = NULL;
	if(tpnode != NULL)
	{
		tmpstr = oitoa(tpnode->pilot);
		setchoiceboxselection(transmission, tmpstr);
		free(tmpstr); tmpstr = NULL;
	}

	//hierarchy
	tmpstr = transpondergethierarchystr(tpnode, 1);
	changeinput(hierarchy, tmpstr);
	free(tmpstr); tmpstr = NULL;
	tmpstr = transpondergethierarchystr(tpnode, 2);
	changechoiceboxvalue(hierarchy, tmpstr);
	free(tmpstr); tmpstr = NULL;
	if(tpnode != NULL)
	{
		tmpstr = oitoa(tpnode->system);
		setchoiceboxselection(hierarchy, tmpstr);
		free(tmpstr); tmpstr = NULL;
	}

	//networkscan
	addchoicebox(networkscan, "0", _("no"));
	addchoicebox(networkscan, "1", _("yes"));

	//clear
	addchoicebox(clear, "0", _("no"));
	addchoicebox(clear, "1", _("yes"));

	//only free
	addchoicebox(onlyfree, "0", _("no"));
	addchoicebox(onlyfree, "1", _("yes"));

	//blindscan
	addchoicebox(blindscan, "0", _("no"));
	addchoicebox(blindscan, "1", _("yes"));

	//changename
	addchoicebox(changename, "1", _("yes"));
	addchoicebox(changename, "0", _("no"));

	//favtype
	addchoicebox(favtype, "0", _("Unchanged"));
	addchoicebox(favtype, "1", _("Create new"));
	addchoicebox(favtype, "2", _("Delete All"));

	//emptybouquet
	addchoicebox(emptybouquet, "0", _("no"));
	addchoicebox(emptybouquet, "1", _("yes"));

	//unusedbouquetchannels
	addchoicebox(unusedbouquetchannels, "0", _("no"));
	addchoicebox(unusedbouquetchannels, "1", _("yes"));

	//unusedsatprovider
	addchoicebox(unusedsatprovider, "0", _("no"));
	addchoicebox(unusedsatprovider, "1", _("yes"));

	//cleartransponder
	addchoicebox(cleartransponder, "0", _("no"));
	addchoicebox(cleartransponder, "1", _("yes"));

	drawscreen(scan, 2, 0);
	changescantype(scantype->ret, scan, listbox, tuner, sat, id, system, frequency, inversion, symbolrate, polarization, fec, modulation, rolloff, pilot, hp, lp, bandwidth, transmission, guardinterval, hierarchy, b4, b5, flag);
	drawscreen(scan, 0, 0);
	addscreenrc(scan, listbox);

	tmp = listbox->select;
	while(1)
	{
		addscreenrc(scan, tmp);

		if(clear->ret != NULL && ostrcmp(clear->ret, "1") == 0)
		{
			emptybouquet->hidden = NO;
			unusedbouquetchannels->hidden = NO;
			unusedsatprovider->hidden = NO;
			cleartransponder->hidden = NO;
		}
		else
		{
			emptybouquet->hidden = YES;
			unusedbouquetchannels->hidden = YES;
			unusedsatprovider->hidden = YES;
			cleartransponder->hidden = YES;
		}
		drawscreen(scan, 0, 0);

		rcret = waitrc(scan, 0, 0);
		tmp = listbox->select;

		if(scantype->ret != NULL) iscantype = atoi(scantype->ret);
		if(sat->ret != NULL) isat = atoi(sat->ret);
		if(system->ret != NULL) isystem = atoi(system->ret);
		if(frequency->ret != NULL) ifrequency = atoi(frequency->ret) * 1000;
		if(inversion->ret != NULL) iinversion = atoi(inversion->ret);
		if(symbolrate->ret != NULL) isymbolrate = atoi(symbolrate->ret) * 1000;
		if(polarization->ret != NULL) ipolarization = atoi(polarization->ret);
		if(fec->ret != NULL) ifec = atoi(fec->ret);
		if(modulation->ret != NULL) imodulation = atoi(modulation->ret);
		if(rolloff->ret != NULL) irolloff = atoi(rolloff->ret);
		if(pilot->ret != NULL) ipilot = atoi(pilot->ret);

		if(flag == 3)
		{
			if(hp->ret != NULL) ifec = atoi(hp->ret);
			if(lp->ret != NULL) ipolarization = atoi(lp->ret);
			if(bandwidth->ret != NULL) isymbolrate = atoi(bandwidth->ret);
			if(transmission->ret != NULL) ipilot = atoi(transmission->ret);
			if(guardinterval->ret != NULL) irolloff = atoi(guardinterval->ret);
			if(hierarchy->ret != NULL) isystem = atoi(hierarchy->ret);
		}

		if(networkscan->ret != NULL) inetworkscan = atoi(networkscan->ret);
		if(onlyfree->ret != NULL) ionlyfree = atoi(onlyfree->ret);
		if(clear->ret != NULL) iclear = atoi(clear->ret);
		if(blindscan->ret != NULL) iblindscan = atoi(blindscan->ret);
		if(changename->ret != NULL) ichangename = atoi(changename->ret);
		if(favtype->ret != NULL) ifavtype = atoi(favtype->ret);
		if(emptybouquet->ret != NULL) iemptybouquet = atoi(emptybouquet->ret);
		if(unusedbouquetchannels->ret != NULL) iunusedbouquetchannels = atoi(unusedbouquetchannels->ret);
		if(unusedsatprovider->ret != NULL) iunusedsatprovider = atoi(unusedsatprovider->ret);
		if(cleartransponder->ret != NULL) icleartransponder = atoi(cleartransponder->ret);

		if(rcret == getrcconfigint("rcexit", NULL)) break;
		if(rcret == getrcconfigint("rcok", NULL)) break;
		if(listbox->select != NULL && ostrcmp(listbox->select->name, "tuner") == 0)
		{
			scanchangesat(sat, tpnode, listbox->select->ret);
			changescantype(scantype->ret, scan, listbox, tuner, sat, id, system, frequency, inversion, symbolrate, polarization, fec, modulation, rolloff, pilot, hp, lp, bandwidth, transmission, guardinterval, hierarchy, b4, b5, flag);
			drawscreen(scan, 0, 0);
		}
		if(listbox->select != NULL && ostrcmp(listbox->select->name, "scantype") == 0)
		{
			changescantype(scantype->ret, scan, listbox, tuner, sat, id, system, frequency, inversion, symbolrate, polarization, fec, modulation, rolloff, pilot, hp, lp, bandwidth, transmission, guardinterval, hierarchy, b4, b5, flag);
			drawscreen(scan, 0, 0);
		}
		if(listbox->select != NULL && ostrcmp(listbox->select->name, "system") == 0)
		{
			changescantype(scantype->ret, scan, listbox, tuner, sat, id, system, frequency, inversion, symbolrate, polarization, fec, modulation, rolloff, pilot, hp, lp, bandwidth, transmission, guardinterval, hierarchy, b4, b5, flag);
			drawscreen(scan, 0, 0);
		}
		if(rcret == getrcconfigint("rcred", NULL))
		{
			clearscreen(scan);
			screenscan(tpnode, scan->child, tuner->ret, iscantype, isat, ifrequency, iinversion, isymbolrate, ipolarization, ifec, imodulation, irolloff, ipilot, inetworkscan, ionlyfree, iclear, iblindscan, ichangename, isystem, ifavtype, iemptybouquet, iunusedbouquetchannels, iunusedsatprovider, icleartransponder, 5000000, flag);
			drawscreen(scan, 0, 0);
		}
		if(rcret == getrcconfigint("rcgreen", NULL) && tpnode != NULL && iscantype == 0)
		{
			struct transponder* tp1 = createtransponder(99, tpnode->fetype, isat, ifrequency, iinversion, isymbolrate, ipolarization, ifec, imodulation, irolloff, ipilot, isystem);
			copytransponder(tp1, tpnode, 99);
			deltransponderbyid(99);
			textbox(_("Message"), _("Transponder changed"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 5, 0);
			drawscreen(scan, 0, 0);
		}
		if(rcret == getrcconfigint("rcyellow", NULL) && iscantype == 0)
		{
			struct transponder* tp = tpchoicescreen(isat, flag);
			if(tp != NULL)
			{
				tpnode = tp;
				goto start;
			}
			else
				drawscreen(scan, 0, 0);
		}
		else if(rcret == getrcconfigint("rcok", NULL))
			break;
	}

	delmarkedscreennodes(scan, 1);
	delownerrc(scan);
	clearscreen(scan);
	resetsatscan();

	if(status.lastservice->channel == NULL)
	{
		if(getchannel(getconfigint("serviceid", NULL), getconfigllu("transponderid", NULL)) != NULL)
		{
			if(status.servicetype == 0)
				servicecheckret(servicestart(getchannel(getconfigint("serviceid", NULL), getconfigllu("transponderid", NULL)), getconfig("channellist", NULL), NULL, 0), 0);
			else
				servicecheckret(servicestart(getchannel(getconfigint("rserviceid", NULL), getconfigllu("rtransponderid", NULL)), getconfig("rchannellist", NULL),  NULL, 0), 0);
		}
	}
	else
	{
		tmpstr = ostrcat(status.lastservice->channellist, NULL, 0, 0);
		servicecheckret(servicestart(status.lastservice->channel, tmpstr, NULL, 0), 0);
		free(tmpstr); tmpstr = NULL;
	}

	//recalc channels
	struct channel* chnode = channel;
	while(chnode != NULL)
	{
		if(chnode->servicetype != 99)
		{
			chnode->transponder = gettransponder(chnode->transponderid);
			chnode->provider = getprovider(chnode->providerid);
		}
		chnode = chnode->next;
	}

	writeallconfig(1);
}

#endif
