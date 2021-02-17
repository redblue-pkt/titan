#ifndef FRONTENDDEV_H
#define FRONTENDDEV_H

enum {
		T_Bandwidth_8MHz, T_Bandwidth_7MHz, T_Bandwidth_6MHz, T_Bandwidth_Auto, T_Bandwidth_5MHz, T_Bandwidth_1_712MHz, T_Bandwidth_10MHz
};

enum {
		T_FEC_1_2=0, T_FEC_2_3=1, T_FEC_3_4=2, T_FEC_5_6=3, T_FEC_7_8=4, T_FEC_Auto=5, T_FEC_6_7=6, T_FEC_8_9=7
};

enum {
		System_DVB_T_T2 = -1, System_DVB_T, System_DVB_T2
};

enum {
		T_TransmissionMode_2k, T_TransmissionMode_8k, T_TransmissionMode_Auto, T_TransmissionMode_4k, T_TransmissionMode_1k, T_TransmissionMode_16k, T_TransmissionMode_32k
};

enum {
		T_GuardInterval_1_32, T_GuardInterval_1_16, T_GuardInterval_1_8, T_GuardInterval_1_4, T_GuardInterval_Auto, T_GuardInterval_1_128, T_GuardInterval_19_128, T_GuardInterval_19_256
};

enum {
		T_Hierarchy_None, T_Hierarchy_1, T_Hierarchy_2, T_Hierarchy_4, T_Hierarchy_Auto
};

enum {
		T_Modulation_QPSK, T_Modulation_QAM16, T_Modulation_QAM64, T_Modulation_Auto, T_Modulation_QAM256
};

enum {
		T_Inversion_Off, T_Inversion_On, T_Inversion_Unknown
};

int calclof(struct dvbdev* node, struct transponder* tpnode, char* feaktnr, int flag)
{
	int loftype = 0;
	int lofl, lofh, lofthreshold;
	int satcrfrequ = 0;
	char* tmpstr = NULL;

	if(node == NULL || tpnode == NULL)
	{
		err("NULL detect");
		return -1;
	}

	if(node->feinfo->type != FE_QPSK)
		return 0;

	unsigned int frequency = tpnode->frequency;
	node->feunicable = 0;

	if(feaktnr == NULL) feaktnr = node->feaktnr;

	tmpstr = ostrcat(node->feshortname, "_lnb_loftype", 0, 0);
	loftype = getconfigint(tmpstr, feaktnr);
	free(tmpstr); tmpstr = NULL;
	switch(loftype)
	{
		case 1: //c-band
			lofl = 5150 * 1000;
			lofh = 5150 * 1000;
			lofthreshold = 5150 * 1000;
			break;
		case 2: //user
			tmpstr = ostrcat(node->feshortname, "_lnb_lofl", 0, 0);
			lofl = getconfigint(tmpstr, feaktnr) * 1000;
			free(tmpstr); tmpstr = NULL;
			tmpstr = ostrcat(node->feshortname, "_lnb_lofh", 0, 0);
			lofh = getconfigint(tmpstr, feaktnr) * 1000;
			free(tmpstr); tmpstr = NULL;
			tmpstr = ostrcat(node->feshortname, "_lnb_lofthreshold", 0, 0);
			lofthreshold = getconfigint(tmpstr, feaktnr) * 1000;
			free(tmpstr); tmpstr = NULL;
			break;
		case 3: //unicable
		case 4: //user unicable
			tmpstr = ostrcat(node->feshortname, "_lnb_lofl", 0, 0);
			lofl = getconfigint(tmpstr, feaktnr) * 1000;
			free(tmpstr); tmpstr = NULL;
			if(lofl == 0) lofl = 9750 * 1000;
			tmpstr = ostrcat(node->feshortname, "_lnb_lofh", 0, 0);
			lofh = getconfigint(tmpstr, feaktnr) * 1000;
			free(tmpstr); tmpstr = NULL;
			if(lofh == 0) lofh = 10600 * 1000;
			tmpstr = ostrcat(node->feshortname, "_lnb_lofthreshold", 0, 0);
			lofthreshold = getconfigint(tmpstr, feaktnr) * 1000;
			free(tmpstr); tmpstr = NULL;
			if(lofthreshold == 0) lofthreshold = 11700 * 1000;
			tmpstr = ostrcat(node->feshortname, "_lnb_satcrfrequ", 0, 0);
			satcrfrequ = getconfigint(tmpstr, feaktnr) * 1000;
			free(tmpstr); tmpstr = NULL;
			break;
		default: //standard lnb
			lofl = 9750 * 1000;
			lofh = 10600 * 1000;
			lofthreshold = 11700 * 1000;
	}

	if(lofthreshold && lofh && frequency >= lofthreshold)
	{
		if(flag == 1) return 1;
		node->feaktband = 1;
	}
	else
	{
		if(flag == 1) return 0;
		node->feaktband = 0;
	}

	node->feloffrequency_uni = 0;
	
	if(satcrfrequ == 0)
	{
		if(node->feaktband)
			node->feloffrequency = frequency - lofh;
		else
		{
			if(frequency < lofl)
				node->feloffrequency = lofl - frequency;
			else
				node->feloffrequency = frequency - lofl;
		}
	}
	else
	{
		tmpstr = ostrcat(node->feshortname, "_lnb_satcr", 0, 0);
		int satcr = getconfigint(tmpstr, node->feaktnr) - 1;
		free(tmpstr); tmpstr = NULL;
		if(satcr < 8)
		{
			int lof = (node->feaktband & 1) ? lofh : lofl;
			unsigned int tmp = (frequency - lof) + satcrfrequ;
			node->feloffrequency = (tmp / 4) - 350000;
			node->feunicable = 1;
		}
		else
		{
			int lof = (node->feaktband & 1) ? lofh : lofl;
			unsigned int tmp = (frequency - lof) - 100000;
			unsigned int tmp2 = (1000 + 2 * tmp) / (2 *1000); //round to multiple of 1000
			node->feloffrequency = tmp2;
			
			//node->feloffrequency_uni = satcrfrequ - (tmp - (1000 * tmp2));
			//node->feloffrequency_uni = frequency - lof -(satcrfrequ - (satcrfrequ - (tmp - (1000 * node->feloffrequency))));
			//node->feloffrequency_uni = lof + (satcrfrequ + lof + frequency);
			//node->feloffrequency_uni = (satcrfrequ + lof + frequency);
			//node->feloffrequency_uni = (satcrfrequ + frequency);
			//node->feloffrequency_uni = (satcrfrequ * 1000);
			
			//tmp = frequency + lof + satcrfrequ;
			//node->feloffrequency_uni = ((1000 + 2 * tmp) / (2 *1000)) * 1000; //round to multiple of 1000
			node->feloffrequency_uni = 0; //test
			
			node->feunicable = 1;
			
		}
	}

	debug(200, "tuning to freq %d (befor lof %d), band=%d, unicable=%d unicableoff=%d", node->feloffrequency, frequency, node->feaktband, node->feunicable, node->feloffrequency_uni);
	return node->feaktband;
}

char* fegettypestr(struct dvbdev* dvbnode)
{
	char* text = NULL;

	if(dvbnode == NULL)
	{
		err("NULL detect");
		return NULL;
	}

	switch(dvbnode->feinfo->type)
	{
		case FE_QPSK: text = ostrcat(text, "DVB-S", 1, 0); break;
		case FE_QAM: text = ostrcat(text, "DVB-C", 1, 0); break;
		case FE_OFDM: text = ostrcat(text, "DVB-T", 1, 0); break;
		default: text = ostrcat(text, "unknown", 1, 0);
	}

	return text;
}

struct dvbdev* fegetbyshortname(char* feshortname)
{
	struct dvbdev* dvbnode = dvbdev;
	
	while(dvbnode != NULL)
	{
		if(dvbnode->type == FRONTENDDEV && ostrcmp(dvbnode->feshortname, feshortname) == 0)
			return dvbnode;
		dvbnode = dvbnode->next;
	}
	return NULL;
}

void fegetconfig(struct dvbdev *dvbnode, struct transponder *tpnode, char** aktnr, char* tmpnr)
{
	char* tmpstr = NULL;

	if(dvbnode == NULL || tpnode == NULL)
	{
		err("NULL detect");
		return;
	}

	tmpstr = ostrcat(dvbnode->feshortname, "_satnr", 0, 0);
	*aktnr = getconfig(tmpstr, tmpnr);
	free(tmpstr); tmpstr = NULL;
}

struct dvbdev* fegetdummy()
{
	struct dvbdev* dvbnode = dvbdev;

	while(dvbnode != NULL)
	{
		if(dvbnode->type == FRONTENDDEVDUMMY)
			return dvbnode;
		dvbnode = dvbnode->next;
	}
	return NULL;
}

void settunerstatus()
{
	struct dvbdev* dvbnode = dvbdev;
	char *tmpstr = NULL;
	char *buf = NULL;
	int fbc = 0;
	while(dvbnode != NULL)
	{
		//FRONTENDDEV first in the list
		if(dvbnode->type != FRONTENDDEV) break;
		
		if(dvbnode != NULL && (ostrstr(dvbnode->feinfo->name, "BCM45208") != NULL || fbctuner == 1 ))
			fbc = 1;
		else
			fbc = 0;

		if((checkbox("DM900") == 1 || checkbox("DM920") == 1 || checkbox("DM520") == 1 || checkbox("DM525") == 1) && fbc != 1)
		{
			if(ostrcmp("fe_01", dvbnode->feshortname) == 0)
			{
				if(ostrcmp("fe_00", getconfig(dvbnode->feshortname, NULL)) == 0)
					system("echo internal > /proc/stb/frontend/1/rf_switch");
				else
					system("echo external > /proc/stb/frontend/1/rf_switch");
			}
		}
		if(fbc == 1)
		{
			buf = malloc(MINMALLOC);
			if(buf == NULL)
			{
				err("no memory");
				return;
			}
			sprintf(buf, "/proc/stb/frontend/%d/input",dvbnode->devnr);
			tmpstr = ostrcat(dvbnode->feshortname, "_fbc", 0, 0);
			if(ostrcmp("AM", getconfig(tmpstr, NULL)) == 0 || ostrcmp("AU", getconfig(tmpstr, NULL)) == 0 || ostrcmp("A", getconfig(tmpstr, NULL)) == 0)
				writesys(buf, "A", 1); 
			else
				writesys(buf, "B", 1);	
			free(buf); buf = NULL;
			free(tmpstr); tmpstr = NULL;
		}
		//check if tuner is deactivate
		if(ostrcmp("x", getconfig(dvbnode->feshortname, NULL)) == 0)
		{
			dvbnode->deactive = 1;
			if(dvbnode->fd > -1)
			{
				feclose(dvbnode, -1);
			}
		}
		else
		{
			dvbnode->deactive = 0;
			if(dvbnode->fd == -1)
			{
				feopen(dvbnode, NULL);
			}
		}
		dvbnode = dvbnode->next;
	}
}

//flag 0 = normal
//flag 1 = check only
//flag 2 = from record
//flag 3 = from rectimer
struct dvbdev* fegetfree(struct transponder* tpnode, int flag, struct dvbdev* dvbfirst, struct channel* chnode)
{
	struct dvbdev* dvbnode = NULL;
	struct dvbdev* tmpdvbnode = NULL;
	char* tmpstr = NULL, *tmpnr = NULL, *aktnr = NULL;
	int i, orbitalpos = 0, band = 0;
	int found = 0;
	int ciplus = 1;
	//char *CharPtrTmp[20];
	struct dvbdev* CharPtrTmp[20];
	
	if(dvbfirst != NULL)
		dvbnode = dvbfirst;
	else
		dvbnode = dvbdev;

	if(tpnode == NULL)
	{
		err("NULL detect");
		return NULL;
	}

	ciplus = 0;
	
	//workaround fuer CI+ bei mehr wie einen Tuner
	//if(checkbox("DM900") == 1 && chnode != NULL && ciplusrun == 1)
	if(checkbox("DM900") == 1 && chnode != NULL && getconfigint("ciplusrun", NULL) == 1)
	{
		struct channelslot *channelslotnode = channelslot;
		ciplus = 1;
		while(channelslotnode != NULL)
		{
			if(channelslotnode->transponderid == chnode->transponderid && channelslotnode->serviceid == chnode->serviceid)
			{
				ciplus = 2;
				debug(200, "CI+ reserviert Tuner 0");
				break;
			}
			channelslotnode = channelslotnode->next;			
		}
	}
	else
	{ 
		ciplus = 0;
		debug(200, "CI+ workaround nicht aktiv");
	}
		 
	
	//suche tuner der auf der gleichen orbitalpos/frequency/pol/band ist
	while(dvbnode != NULL)
	{
		//FRONTENDDEV first in the list
		if(dvbnode->type != FRONTENDDEV) break;

		//check if tuner is deactivate
		if(dvbnode->deactive == 1)
		{
			dvbnode = dvbnode->next;
			continue;
		}
		
		//check CI+ workaround
		if(ciplus == 1 && dvbnode->devnr == 0)
		{
			dvbnode = dvbnode->next;
			continue;
		}
		if(ciplus == 2 && dvbnode->devnr != 0)
		{
			dvbnode = dvbnode->next;
			continue;
		}
		
		if(getconfigint("debuglevel", NULL) == 200 && dvbnode->type == FRONTENDDEV)
			debug(200, "fegetfree: dvbnode->feinfo-typ:%d tpnode->fetype:%d dvbnode->felock:%d dvbnode->devnr:%d dvbnode->feshortname:%s dvbnode->fehyprid:%s", dvbnode->feinfo->type, tpnode->fetype, dvbnode->felock, dvbnode->devnr, dvbnode->feshortname, dvbnode->fehyprid);
		if(dvbnode->type == FRONTENDDEV && dvbnode->feinfo->type == tpnode->fetype)
		{
			if(getconfigint("debuglevel", NULL) == 200 && dvbnode->feakttransponder != NULL)
				debug(200, "dvbnode->feakttransponder->orbitalpos:%d tpnode->orbitalpos:%d dvbnode->feakttransponder->frequency:%d tpnode->frequency:%d dvbnode->feaktpolarization:%d tpnode->polarization:%d", dvbnode->feakttransponder->orbitalpos, tpnode->orbitalpos, dvbnode->feakttransponder->frequency, tpnode->frequency, dvbnode->feaktpolarization, tpnode->polarization);
			if(dvbnode->feakttransponder != NULL && dvbnode->felock == 0 && status.aktservice->fedev != dvbnode)
			{
				if(flag != 1) debug(200, "clear tuner %s", dvbnode->feshortname);
				dvbnode->felasttransponder = dvbnode->feakttransponder;
				dvbnode->feakttransponder = NULL;
			}	
			if(dvbnode->feakttransponder != NULL && dvbnode->feakttransponder->orbitalpos == tpnode->orbitalpos && dvbnode->feakttransponder->frequency == tpnode->frequency && dvbnode->feaktpolarization == tpnode->polarization)
			{
				band = calclof(dvbnode, tpnode, dvbnode->feaktnr, 1);
				if(dvbnode->feaktband != band)
				{
					dvbnode = dvbnode->next;
					continue;
				}
				dvbnode->felasttransponder = dvbnode->feakttransponder;
				dvbnode->feakttransponder = tpnode;
				if(flag != 1) debug(200, "found tuner with same orbitalpos/frequency/pol/band %s", dvbnode->feshortname);
				return(dvbnode);
			}
		}
		dvbnode = dvbnode->next;
	}
	if(dvbfirst != NULL)
		dvbnode = dvbfirst;
	else
		dvbnode = dvbdev;

	//suche tuner der die gewuenschte orbitalpos kann und belegt ist und dessen looptuner es kann
	while(dvbnode != NULL)
	{
		//FRONTENDDEV first in the list
		if(dvbnode->type != FRONTENDDEV) break;

		//check if tuner is deactivate
		if(dvbnode->deactive == 1)
		{
			dvbnode = dvbnode->next;
			continue;
		}
		
		
		if(dvbnode->type == FRONTENDDEV && dvbnode->feinfo->type == tpnode->fetype && (dvbnode->felock != 0 || (flag == 2 && status.aktservice->fedev == dvbnode)))
		{
			//check if tuner is main tuner
			if(getconfig(dvbnode->feshortname, NULL) != NULL)
			{
				dvbnode = dvbnode->next;
				continue;				
			}
			if(dvbnode->feaktpolarization != tpnode->polarization || dvbnode->feakttransponder->orbitalpos != tpnode->orbitalpos)
			{
				dvbnode = dvbnode->next;
				continue;				
			}
			
			found = 0;
			int ciplusc = 0;
			//check looptuner is free
			tmpstr = getconfigbyval(dvbnode->feshortname, NULL);
			CharPtrTmp[0] = NULL;
			while(tmpstr != NULL) //found loop tuner
			{
				tmpdvbnode = fegetbyshortname(tmpstr);
				if(tmpdvbnode != NULL)
				{
					//check CI+ workaround
					if(ciplus != 0)
					{
						if(ciplus == 1 && tmpdvbnode->devnr == 0)
						{
							CharPtrTmp[found] = tmpdvbnode;
							found = found + 1;
							tmpstr = getconfigbyval(tmpstr, NULL); //loop tuner also loop  ?	
							continue;
						}
						if(ciplus == 2 && tmpdvbnode->devnr != 0)
						{
							CharPtrTmp[found] = tmpdvbnode;
							found = found + 1;
							tmpstr = getconfigbyval(tmpstr, NULL); //loop tuner also loop  ?	
							continue;
						}
					}
					if(tmpdvbnode->feakttransponder == NULL || (tmpdvbnode->felock == 0 && status.aktservice->fedev != tmpdvbnode))
					{
						found = 99;
						break;
					}
					else
					{
						CharPtrTmp[found] = tmpdvbnode;
						found = found + 1;
					}
				}
				tmpstr = getconfigbyval(tmpstr, NULL); //loop tuner also loop  ?
			}
			CharPtrTmp[0] = NULL;
			if(found != 99)
			{
				dvbnode = dvbnode->next;
				continue;
			}
			tmpstr = ostrcat(dvbnode->feshortname, "_sat", 0, 0); 
			for(i = 1; i <= getmaxsat(dvbnode->feshortname); i++)
			{
				tmpnr = oitoa(i);
				orbitalpos = getconfigint(tmpstr, tmpnr);
				if(orbitalpos == tpnode->orbitalpos)
				{
					fegetconfig(dvbnode, tpnode, &aktnr, tmpnr);
					if(flag == 3)
						band = calclof(dvbnode, tpnode, aktnr, 0);
					else
						band = calclof(dvbnode, tpnode, aktnr, 1);
					found = 0;
					if(dvbnode->feaktband != band)
					{
						found = -1;
						break;
					}
					if(flag == 1)
					{
						free(tmpstr); tmpstr = NULL;
						free(tmpnr); tmpnr = NULL;
						return tmpdvbnode;
					}
					if(tmpdvbnode->feakttransponder != NULL)
						tmpdvbnode->felasttransponder = tmpdvbnode->feakttransponder;
					tmpdvbnode->feakttransponder = tpnode;
					tmpdvbnode->feaktpolarization = tpnode->polarization;
					tmpdvbnode->feaktband = band;
					free(tmpdvbnode->feaktnr);
					if(aktnr != NULL)
						tmpdvbnode->feaktnr = ostrcat(aktnr, NULL, 0, 0);
					else
						tmpdvbnode->feaktnr = NULL;

					free(tmpstr); tmpstr = NULL;
					free(tmpnr); tmpnr = NULL;
					if(flag != 1) debug(200, "found free tuner witch same orbitalpos %s", dvbnode->feshortname);
					return tmpdvbnode;
				}
				free(tmpnr); tmpnr = NULL;
			}
			free(tmpstr); tmpstr = NULL;
		}
		dvbnode = dvbnode->next;
	}
	if(dvbfirst != NULL)
		dvbnode = dvbfirst;
	else
		dvbnode = dvbdev;

	//suche tuner der die gewuenschte orbitalpos kann und nicht belegt ist
	while(dvbnode != NULL)
	{
		//FRONTENDDEV first in the list
		if(dvbnode->type != FRONTENDDEV) break;

		//check if tuner is deactivate
		if(dvbnode->deactive == 1)
		{
			dvbnode = dvbnode->next;
			continue;
		}
		
		//check CI+ workaround
		if(ciplus == 1 && dvbnode->devnr == 0)
		{
			dvbnode = dvbnode->next;
			continue;
		}
		if(ciplus == 2 && dvbnode->devnr != 0)
		{
			dvbnode = dvbnode->next;
			continue;
		}
		
		if(dvbnode->type == FRONTENDDEV && dvbnode->feinfo->type == tpnode->fetype && dvbnode->felock == 0)
		{
			//check if tuner is main tuner
			if(getconfig(dvbnode->feshortname, NULL) != NULL)
			{
				dvbnode = dvbnode->next;
				continue;				
			}
			if(flag == 2 && status.aktservice->fedev == dvbnode)
			{
				dvbnode = dvbnode->next;
				continue;
			}
			found = 0;
			//check if tuner is loop and looptuner is locked
			tmpstr = getconfigbyval(dvbnode->feshortname, NULL);
			CharPtrTmp[0] = NULL;
			while(tmpstr != NULL) //found loop tuner
			{
				tmpdvbnode = fegetbyshortname(tmpstr);
				if(tmpdvbnode != NULL)
				{
					if(tmpdvbnode->feakttransponder != NULL && (tmpdvbnode->feaktpolarization != tpnode->polarization || tmpdvbnode->feakttransponder->orbitalpos != tpnode->orbitalpos) && (tmpdvbnode->felock != 0 || (flag == 2 && tmpdvbnode->felock == 0)))
					{
						found = -1;
						break;
					}
					else
					{
						CharPtrTmp[found] = tmpdvbnode;
						found = found + 1;
					}
				}
				tmpstr = getconfigbyval(tmpstr, NULL); //loop tuner also loop  ?
			}
			if(found == -1)
			{
				dvbnode = dvbnode->next;
				continue;
			}
			CharPtrTmp[found] = NULL;
			tmpstr = ostrcat(dvbnode->feshortname, "_sat", 0, 0); 
			for(i = 1; i <= getmaxsat(dvbnode->feshortname); i++)
			{
				tmpnr = oitoa(i);

				orbitalpos = getconfigint(tmpstr, tmpnr);
				if(orbitalpos == tpnode->orbitalpos)
				{
					fegetconfig(dvbnode, tpnode, &aktnr, tmpnr);
					if(flag == 3)
						band = calclof(dvbnode, tpnode, aktnr, 0);
					else
						band = calclof(dvbnode, tpnode, aktnr, 1);
					found = 0;
					while(CharPtrTmp[found] != NULL)
					{
						if(CharPtrTmp[found]->feakttransponder != NULL)
						{
							if(CharPtrTmp[found]->feaktband != band && (CharPtrTmp[found]->felock != 0 || (flag == 2 && CharPtrTmp[found]->felock == 0)))
							{
								found = 99;
								break;
							}
						}
						found = found + 1;
					}
					if(found == 99)
					{
						found = 0;
						free(tmpnr); tmpnr = NULL;
						continue;
					}	
					if(flag == 1)
					{
						free(tmpstr); tmpstr = NULL;
						free(tmpnr); tmpnr = NULL;
						return dvbnode;
					}
					found = 0;
					while(CharPtrTmp[found] != NULL)
					{
						CharPtrTmp[found]->feaktband = band;
						CharPtrTmp[found]->feaktpolarization = tpnode->polarization;
						found = found + 1;
					}
					dvbnode->felasttransponder = dvbnode->feakttransponder;
					dvbnode->feakttransponder = tpnode;
					dvbnode->feaktpolarization = tpnode->polarization;
					free(dvbnode->feaktnr);
					if(aktnr != NULL)
						dvbnode->feaktnr = ostrcat(aktnr, NULL, 0, 0);
					else
						dvbnode->feaktnr = NULL;
					free(tmpstr); tmpstr = NULL;
					free(tmpnr); tmpnr = NULL;
					if(flag != 1) debug(200, "found free tuner witch same orbitalpos %s", dvbnode->feshortname);
					return dvbnode;
				}
				free(tmpnr); tmpnr = NULL;
			}
			free(tmpstr); tmpstr = NULL;
		}
		dvbnode = dvbnode->next;
	}
	if(dvbfirst != NULL)
		dvbnode = dvbfirst;
	else
		dvbnode = dvbdev;

	//suche loop tuner, wo der haupttuner 
	//die gewuenschte orbitalpos kann, nicht belegt ist
	//und auf der gleichen poarization/band ist, wo wir hintunen wollen
	while(dvbnode != NULL)
	{
		//FRONTENDDEV first in the list
		if(dvbnode->type != FRONTENDDEV) break;

		//check if tuner is deactivate
		if(dvbnode->deactive == 1)
		{
			dvbnode = dvbnode->next;
			continue;
		}
		
		if(dvbnode->type == FRONTENDDEV && dvbnode->feinfo->type == tpnode->fetype && dvbnode->felock == 0)
		{
			if(flag == 2 && status.aktservice->fedev == dvbnode)
			{
				dvbnode = dvbnode->next;
				continue;
			}
			if(getconfig(dvbnode->feshortname, NULL) == NULL)
			{
				dvbnode = dvbnode->next;
				continue;				
			}
			//check if tuner is loop and an other loopt is locked
			found = 0;
			tmpstr = getconfigbyval(dvbnode->feshortname, NULL);
			while(tmpstr != NULL) //found loop tuner
			{
				tmpdvbnode = fegetbyshortname(tmpstr);
				if(tmpdvbnode != NULL)
				{
					if(ciplus != 0)
					{
						if(ciplus == 1 && tmpdvbnode->devnr == 0)
						{
							CharPtrTmp[found] = tmpdvbnode;
							found = found + 1;
							tmpstr = getconfigbyval(tmpstr, NULL); //loop tuner also loop  ?	
							continue;
						}
						if(ciplus == 2 && tmpdvbnode->devnr != 0)
						{
							CharPtrTmp[found] = tmpdvbnode;
							found = found + 1;
							tmpstr = getconfigbyval(tmpstr, NULL); //loop tuner also loop  ?	
							continue;
						}
					}
					if(tmpdvbnode->feakttransponder != NULL && (tmpdvbnode->feaktpolarization != tpnode->polarization || tmpdvbnode->feakttransponder->orbitalpos != tpnode->orbitalpos) && (tmpdvbnode->felock != 0 || (flag == 2 && tmpdvbnode->felock == 0)))
					{
						found = -1;
						break;
					}
					else
					{
						CharPtrTmp[found] = tmpdvbnode;
						found = found + 1;
					}
				}
				tmpstr = getconfigbyval(tmpstr, NULL); //loop tuner also loop  ?
			}
			if(found == -1)
			{
				dvbnode = dvbnode->next;
				continue;
			}	
			//check if tuner is loop an looptuner is locked
			tmpstr = getconfig(dvbnode->feshortname, NULL);
			while(tmpstr != NULL) //found loop tuner
			{
				tmpdvbnode = fegetbyshortname(tmpstr);
				if(tmpdvbnode != NULL)
				{
					if(tmpdvbnode->feakttransponder != NULL && (tmpdvbnode->feaktpolarization != tpnode->polarization || tmpdvbnode->feakttransponder->orbitalpos != tpnode->orbitalpos) && (tmpdvbnode->felock != 0 || (flag == 2 && tmpdvbnode->felock == 0)))
					{
						found = -1;
						break;
					}
					else
					{
						CharPtrTmp[found] = tmpdvbnode;
						found = found + 1;
					}
				}
				tmpstr = getconfig(tmpstr, NULL);
			}
			CharPtrTmp[found] = NULL;
			if(found == -1)
			{
				dvbnode = dvbnode->next;
				continue;
			}
	
			tmpstr = ostrcat(tmpdvbnode->feshortname, "_sat", 0, 0);
			for(i = 1; i <= getmaxsat(tmpdvbnode->feshortname); i++)
			{
				tmpnr = oitoa(i);
				orbitalpos = getconfigint(tmpstr, tmpnr);
				if(orbitalpos == tpnode->orbitalpos)
				{ 
					fegetconfig(tmpdvbnode, tpnode, &aktnr, tmpnr);
					if(flag == 3)
						band = calclof(dvbnode, tpnode, aktnr, 0);
					else
						band = calclof(dvbnode, tpnode, aktnr, 1);
					found = 0;
					while(CharPtrTmp[found] != NULL)
					{
						if(CharPtrTmp[found]->feakttransponder != NULL && CharPtrTmp[found]->feaktband != band && (CharPtrTmp[found]->felock != 0 || (flag >= 2 && CharPtrTmp[found]->felock == 0)))	
						{
							found = 99;
							break;
						}
						found = found + 1;
					}
					if(found == 99)
					{
						found = 0;
						free(tmpnr); tmpnr = NULL;
						continue;
					}
						
					if(flag == 1)
					{
						free(tmpstr); tmpstr = NULL;
						free(tmpnr); tmpnr = NULL;
						return dvbnode;
					}
					found = 0;
					while(CharPtrTmp[found] != NULL)
					{
						CharPtrTmp[found]->feaktband = band;
						CharPtrTmp[found]->feaktpolarization = tpnode->polarization;
						found = found + 1;
					}
					dvbnode->felasttransponder = dvbnode->feakttransponder;
					dvbnode->feakttransponder = tpnode;
					dvbnode->feaktpolarization = tpnode->polarization;
					free(dvbnode->feaktnr);
					if(aktnr != NULL)
						dvbnode->feaktnr = ostrcat(aktnr, NULL, 0, 0);
					else
						dvbnode->feaktnr = NULL;
					free(tmpstr); tmpstr = NULL;
					free(tmpnr); tmpnr = NULL;
					if(flag != 1) debug(200, "found free looptuner witch same orbitalpos/polarization/band %s main: %s", dvbnode->feshortname, tmpdvbnode->feshortname);
					return dvbnode;
				}
				free(tmpnr); tmpnr = NULL;
			}
			free(tmpstr); tmpstr = NULL;
		}
		dvbnode = dvbnode->next;
	}

	return NULL;
}

int feopen(struct dvbdev* node, char *fedev)
{
	int fd = -1;

	if(node != NULL)
	{	
		if((fd = open(node->dev, O_RDWR | O_NONBLOCK)) < 0)
			//debug(200, "open frontend failed %s", node->dev);
			err("open1 frontend failed %s", node->dev);
		node->fd = fd;
	}
	else
	{
		if((fd = open(fedev, O_RDWR | O_NONBLOCK)) < 0)
			//debug(200, "open frontend failed %s", fedev);
			err("open2 frontend failed %s", fedev);
	}

	closeonexec(fd);
	return fd;
}

void feclose(struct dvbdev* node, int fd)
{
	if(node != NULL)
	{
		if(close(node->fd) < 0)
		{
			err("close frontend failed");
		}
		else
			node->fd = -1;
	}
	else
		close(fd);
}

int fegetunlock(struct dvbdev* node)
{
	fe_status_t status;

	if(node == NULL)
	{
		err("NULL detect");
		return 1;
	}

#ifdef SIMULATE
	return 0;
#endif

	if(ioctl(node->fd, FE_READ_STATUS, &status) == -1)
		perr("FE_READ_STATUS");

	if(status & FE_HAS_LOCK)
		return 0;
	else
		return 1;
}

int fewait(struct dvbdev* node)
{
	//struct dvb_frontend_event ev;
	fe_status_t status;
	fe_status_t status_m = 0;

	int count = 0;

	if(node == NULL)
	{
		err("NULL detect");
		return 1;
	}

#ifdef SIMULATE
	return 0;
#endif

	int timer = 500;

#ifdef MIPSEL
	timer = 2000;
#endif

#ifdef ARM
	timer = 500;
#endif

	if(checkchipset("3798MV200") == 1)
		timer = 5000;
	if(checkrealbox("HD51") == 1 || checkrealbox("HD60") == 1 || checkrealbox("HD61") == 1)
		timer = 1000;
	if(checkchipset("HI3798MV200") == 1)
		timer = 4000;
		//timer = 6000;
	if(checkrealbox("SPARK7162") == 1)
		timer = 5;

printf("fewait: timer=%d\n",timer);
	//wait for tuner ready
	debug(200, "wait for tuner start");
	while(count <= timer)
	{
		count++;

		//ioctl(node->fd, FE_GET_EVENT, &ev);
		//if(ev.status & FE_HAS_LOCK)
		//	return 0;
		ioctl(node->fd, FE_READ_STATUS, &status);
		if(status != 0)
		{
			if(status_m != status)
			{
				debug(200, "status=%02x, fe_lock=%02x, count=%d", status, FE_HAS_LOCK, count);
				status_m = status;
			}
		}

		if(errno == ERANGE)
		{
			usleep(1000);
			continue;
		}

		if(status & FE_HAS_LOCK)
		{
	//		if(FE_HAS_SYNC | FE_HAS_LOCK)
			debug(200, "wait for tuner end with 0");
			return 0;
		}
		if(node != NULL && (ostrstr(node->feinfo->name, "BCM45208") != NULL || fbctuner == 1))
		{
			if(status & FE_TIMEDOUT)
			{
				debug(200, "wait for tuner end with FE_TIMEDOUT");
				return 1;
			}
		}
		usleep(1000);
	}
	debug(200, "wait for tuner end");
	//if(ev.status & FE_HAS_LOCK)
	//	return 0;
	if(status & FE_HAS_LOCK)
//	if(FE_HAS_SYNC | FE_HAS_LOCK)
		return 0;
	else
		return 1;
}

void fegetfrontend(struct dvbdev* node)
{
	if(node == NULL)
	{
		err("NULL detect");
		return;
	}

#if DVB_API_VERSION >= 5
	struct dtv_property p[8];
	struct dtv_properties cmdseq;
	cmdseq.props = p;

	p[0].cmd = DTV_DELIVERY_SYSTEM;
	p[1].cmd = DTV_FREQUENCY;
	p[2].cmd = DTV_MODULATION;
	p[3].cmd = DTV_SYMBOL_RATE;
	p[4].cmd = DTV_INNER_FEC;
	p[5].cmd = DTV_INVERSION;
	p[6].cmd = DTV_ROLLOFF;
	p[7].cmd = DTV_PILOT;
	cmdseq.num = 8;
	
	if(ioctl(node->fd, FE_GET_PROPERTY, &cmdseq) < 0)
	{
		perr("FE_GET_PROPERTY");
	}
	else
	{
		debug(200, "frontend akt delivery system = %d", p[0].u.data);
		debug(200, "frontend akt frequency = %d", p[1].u.data);
		debug(200, "frontend akt inversion = %d", p[5].u.data);
		debug(200, "frontend akt symbol_rate = %d", p[3].u.data);
		debug(200, "frontend akt fec_inner = %d", p[4].u.data);
		debug(200, "frontend akt modulation = %d", p[2].u.data);
		debug(200, "frontend akt rolloff = %d", p[6].u.data);
		debug(200, "frontend akt pilot = %d", p[7].u.data);
	}
#else
	struct dvb_frontend_parameters fe_param;

	if(ioctl(node->fd, FE_GET_FRONTEND, &fe_param) < 0)
	{
		perr("FE_GET_FRONTEND");
	}
	else
	{
		debug(200, "frontend akt frequency = %d", fe_param.frequency);
		debug(200, "frontend akt inversion = %d", fe_param.inversion);
		debug(200, "frontend akt u.qpsk.symbol_rate = %d", fe_param.u.qpsk.symbol_rate);
		debug(200, "frontend akt u.qam.symbol_rate = %d", fe_param.u.qam.symbol_rate);
		debug(200, "frontend akt u.qpsk.fec_inner = %d", fe_param.u.qpsk.fec_inner);
		debug(200, "frontend akt u.qam.fec_inner = %d", fe_param.u.qam.fec_inner);
		debug(200, "frontend akt u.qam.modulation = %d", fe_param.u.qam.modulation);
	}
#endif
}

int fesettone(struct dvbdev* node, fe_sec_tone_mode_t tone, int wait)
{
	int ret = 0;
	
	if(node == NULL)
	{
		err("NULL detect");
		return 1;
	}

	debug(200, "FE_SET_TONE: %d (%s)", tone, node->feshortname);
	if(ioctl(node->fd, FE_SET_TONE, tone) == -1)
	{
		perr("FE_SET_TONE");
		ret = 1;
	}
	else
	{
		node->feakttone = tone;
		usleep(wait * 1000);
	}
	
	return ret;
}

//flag 0: reset tuner params on volt off
//flag 1: don't reset tuner params on volt off
int fesetvoltage(struct dvbdev* node, fe_sec_voltage_t volt, int wait)
{
	int ret = 0;
	int voltm = volt;
	
	if(node == NULL)
	{
		err("NULL detect");
		return 1;
	}

	//DVB-T setzt Volt auch beim tunen und da sollte node nicht geloescht werden (siehe voltm)
	if(voltm == 99)
		volt = SEC_VOLTAGE_OFF;
	
	debug(200, "FE_SET_VOLT: %d (%s)", volt, node->feshortname);
	if(ioctl(node->fd, FE_SET_VOLTAGE, volt) == -1)
	{
		perr("FE_SET_VOLTAGE");
		ret = 1;
	}
	else
	{
		node->feaktvolt = volt;
		if(wait > 0) usleep(wait * 1000);

		if(volt == SEC_VOLTAGE_OFF && voltm != 99)
		{
			node->feakttransponder = NULL;
			node->felasttransponder = NULL;
			node->feunicable = 0;
			node->feloffrequency = 0;
			node->feaktband = 0;
			node->feaktpolarization = 0;
			node->feakttone = 0;
		}
	}

	return ret;
}

void fediseqcsendburst(struct dvbdev* node, fe_sec_mini_cmd_t burst, int wait)
{
	if(node == NULL)
	{
		err("NULL detect");
		return;
	}

	debug(200, "FE_DISEQC_SEND_BURST: %d (%s)", burst, node->feshortname);
	if(ioctl(node->fd, FE_DISEQC_SEND_BURST, burst) == -1)
		perr("FE_DISEQC_SEND_BURST");
	usleep(wait * 1000);
}

void fediseqcsendmastercmd(struct dvbdev* node, struct dvb_diseqc_master_cmd *cmd, int wait)
{
	int i, repeat = 0, imsg = 0;
	char* tmpstr = NULL;

	if(node == NULL)
	{
		err("NULL detect");
		return;
	}
	
	if(cmd == NULL) return;
	if(cmd->msg_len == 0) return;

	tmpstr = ostrcat(node->feshortname, "_diseqc_repeat", 0, 0);
	repeat = getconfigint(tmpstr, node->feaktnr); 
	free(tmpstr); tmpstr = NULL;
	if(repeat < 1) repeat = 1;

	for(i = 0; i < repeat; i++)
	{
		if(ioctl(node->fd, FE_DISEQC_SEND_MASTER_CMD, cmd) == -1)
		{
			perr("FE_DISEQC_SEND_MASTER_CMD");
		}
		usleep(wait * 1000);
	}
	imsg = (cmd->msg[0] << 24) | (cmd->msg[1] << 16) | (cmd->msg[2] << 8) | cmd->msg[3];
	debug(200, "DISEQC Master cmd (%s -> %04X)", node->feshortname, imsg);
}

void fesdiseqcpoweron(struct dvbdev* node)
{
	struct dvb_diseqc_master_cmd cmd = {{0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 0};

	if(node == NULL)
	{
		err("NULL detect");
		return;
	}

	cmd.msg[0] = 0xE0;
	cmd.msg[1] = 0x00;
	cmd.msg[2] = 0x03;
	cmd.msg_len = 3;

	debug(200, "DISEQC Power on (%s)", node->feshortname);
	fediseqcsendmastercmd(node, &cmd, 100);
}

void fesdiseqcreset(struct dvbdev* node)
{
	struct dvb_diseqc_master_cmd cmd = {{0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 0};
	
	if(node == NULL)
	{
		err("NULL detect");
		return;
	}

	cmd.msg[0] = 0xE0;
	cmd.msg[1] = 0x00;
	cmd.msg[2] = 0x00;
	cmd.msg_len = 3;

	debug(200, "DISEQC Reset (%s)", node->feshortname);
	fediseqcsendmastercmd(node, &cmd, 100);
}

void fesdiseqcstandby(struct dvbdev* node)
{
	struct dvb_diseqc_master_cmd cmd = {{0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 0};
	
	if(node == NULL)
	{
		err("NULL detect");
		return;
	}
	
	cmd.msg[0] = 0xE0;
	cmd.msg[1] = 0x00;
	cmd.msg[2] = 0x02;
	cmd.msg_len = 3;

	debug(200, "DISEQC Standby (%s)", node->feshortname);
	fediseqcsendmastercmd(node, &cmd, 100);
}

void fediseqcrotor(struct dvbdev* node, struct transponder* tpnode, int pos, int flag)
{
	int orbitalpos = 0;
	fe_sec_voltage_t oldvolt = 0;
	fe_sec_tone_mode_t oldtone = 0;
	struct dvb_diseqc_master_cmd cmd = {{0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 0};
	
	if(node == NULL)
	{
		err("NULL detect");
		return;
	}
	
	fesdiseqcpoweron(node);
	
	oldvolt = node->feaktvolt;
	oldtone = node->feakttone;

	if(tpnode == NULL)
		orbitalpos = 0;
	else
		orbitalpos = tpnode->orbitalpos;
	
	//float speed13V = 1.5; //1.5 Grad pro sec
	float speed18V = 1; //2.4 Grad pro sek
	float degreesmov, waittime;
	
	switch(flag)
	{
		case 0: //stop move
			cmd.msg[0] = 0xE0; cmd.msg[1] = 0x31; cmd.msg[2] = 0x60; cmd.msg_len = 3;
			debug(200, "DISEQC Rotorpos stop move (%s)", node->feshortname);
			break;
		case 1: //disable limits
			cmd.msg[0] = 0xE0; cmd.msg[1] = 0x31; cmd.msg[2] = 0x63; cmd.msg_len = 3;
			debug(200, "DISEQC Rotorpos disable limits (%s)", node->feshortname);
			break;
		case 2: //enable limits
			cmd.msg[0] = 0xE0; cmd.msg[1] = 0x31; cmd.msg[2] = 0x6A; cmd.msg[3] = 0x00; cmd.msg_len = 4;
			debug(200, "DISEQC Rotorpos enable limits (%s)", node->feshortname);
			break;
		case 3: //set east limit
			cmd.msg[0] = 0xE0; cmd.msg[1] = 0x31; cmd.msg[2] = 0x66; cmd.msg_len = 3;
			debug(200, "DISEQC Rotorpos set east limit (%s)", node->feshortname);
			break;
		case 4: //set west limit
			cmd.msg[0] = 0xE0; cmd.msg[1] = 0x31; cmd.msg[2] = 0x67; cmd.msg_len = 3;
			debug(200, "DISEQC Rotorpos set west limit (%s)", node->feshortname);
			break;
		case 5: //move east cont.
			cmd.msg[0] = 0xE0; cmd.msg[1] = 0x31; cmd.msg[2] = 0x68; cmd.msg[3] = 0x00; cmd.msg_len = 4;
			debug(200, "DISEQC Rotorpos move east cont. (%s)", node->feshortname);
			break;
		case 6: //move west cont.
			cmd.msg[0] = 0xE0; cmd.msg[1] = 0x31; cmd.msg[2] = 0x69; cmd.msg[3] = 0x00; cmd.msg_len = 4;
			debug(200, "DISEQC Rotorpos move west cont. (%s)", node->feshortname);
			break;
		case 7: //store pos
			cmd.msg[0] = 0xE0; cmd.msg[1] = 0x31; cmd.msg[2] = 0x6A; cmd.msg[3] = pos; cmd.msg_len = 4;
			debug(200, "DISEQC Rotorpos store pos=%d (%s)", pos, node->feshortname);
			break;
		case 8: //goto pos
			cmd.msg[0] = 0xE0; cmd.msg[1] = 0x31; cmd.msg[2] = 0x6B; cmd.msg[3] = pos; cmd.msg_len = 4;
			debug(200, "DISEQC Rotorpos goto pos=%d (%s)", pos, node->feshortname);
			break;
		case 9: //step xx pos east
			cmd.msg[0] = 0xE0; cmd.msg[1] = 0x31; cmd.msg[2] = 0x68; cmd.msg[3] = 256 - pos; cmd.msg_len = 4;
			debug(200, "DISEQC Rotorpos step east pos=%d (%s)", pos, node->feshortname);
			break;
		case 10: //step xx pos west
			cmd.msg[0] = 0xE0; cmd.msg[1] = 0x31; cmd.msg[2] = 0x69; cmd.msg[3] = 256 - pos; cmd.msg_len = 4;
			debug(200, "DISEQC Rotorpos step west pos=%d (%s)", pos, node->feshortname);
			break;
		case 11: //goto xx
			cmd.msg[0] = 0xE0; cmd.msg[1] = 0x31; cmd.msg[2] = 0x6E; cmd.msg[3] = (pos >> 8) & 0xff; cmd.msg[4] = pos & 0xff; cmd.msg_len = 5;
			debug(200, "DISEQC Rotorpos goto xx pos=%d (%s)", pos, node->feshortname);
			break;
	}

	if(flag >= 0 && flag < 7)
	{
		fesetvoltage(node, SEC_VOLTAGE_18, 15);
		fesettone(node, SEC_TONE_OFF, 15);
		fediseqcsendmastercmd(node, &cmd, 100);
	}

	if((flag == 7 || flag == 9 || flag == 10) && pos != 0)
	{
		fesetvoltage(node, SEC_VOLTAGE_18, 15);
		fesettone(node, SEC_TONE_OFF, 15);
		fediseqcsendmastercmd(node, &cmd, 100);
	}

	if((flag == 8 || flag == 11) && (orbitalpos == 0 || status.rotoroldorbitalpos == 0 || orbitalpos != status.rotoroldorbitalpos))
	{
		fesetvoltage(node, SEC_VOLTAGE_18, 15);
		fesettone(node, SEC_TONE_OFF, 15);
		fediseqcsendmastercmd(node, &cmd, 100);

		if(status.rotoroldorbitalpos == 0 || orbitalpos == 0)
			waittime = 15;
		else
		{
			degreesmov = abs(orbitalpos - status.rotoroldorbitalpos) / 10;
			waittime = (int)ceil((float)degreesmov / (float)speed18V);
		}

		status.rotoroldorbitalpos = orbitalpos;
		sleep(waittime);
	}
	
	fesetvoltage(node, oldvolt, 15);
	fesettone(node, oldtone, 15);
}

void fesetunicable(struct dvbdev* node)
{
	int unicabletune = 0;
	char* tmpstr = NULL;
	struct dvb_diseqc_master_cmd cmd = {{0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 0};

	if(node == NULL)
	{
		err("NULL detect");
		return;
	}

	tmpstr = ostrcat(node->feshortname, "_lnb_satcr", 0, 0);
	int satcr = getconfigint(tmpstr, node->feaktnr) - 1;
	if(satcr < 0) satcr = 0;
	free(tmpstr); tmpstr = NULL;

	tmpstr = ostrcat(node->feshortname, "_diseqc", 0, 0);
	int aktdiseqc = getconfigint(tmpstr, node->feaktnr);
	if(aktdiseqc < 1) aktdiseqc = 1;
	free(tmpstr); tmpstr = NULL;

	if(satcr < 8)
	{
		int unicabletune = 0;

		unicabletune |= ((satcr & 0x7) << 13);
		unicabletune |= (((aktdiseqc - 1) & 0x1) << 12);
		unicabletune |= (((!node->feaktpolarization) & 0x1) << 11);
		unicabletune |= ((node->feaktband & 0x1) << 10);
		unicabletune |= ((node->feloffrequency / 1000) & 0x3ff);

		debug(200, "unicabletune %04X", unicabletune);
	
		if(status.firstunicablewait == 0)
		{
			status.firstunicablewait = getconfigint("firstunicablewait", NULL);
			if(status.firstunicablewait == 0)
				status.firstunicablewait = 1000;
		}		
	
		if(status.firstunicablewait > 0)
		{
			usleep(status.firstunicablewait * 1000);
			status.firstunicablewait = -1;
		}
	
		fesetvoltage(node, SEC_VOLTAGE_13, 15);
		fesetvoltage(node, SEC_VOLTAGE_18, 15);
		fesettone(node, SEC_TONE_OFF, 15);

		//feunicable
		//byte1 (bit 7/6/5) -> satcr number
		//byte1 (bit 4/3/2) -> lnb number
		//byte1 (bit 1/0) -> frequ
		//byte0 -> frequ
	
		cmd.msg[0] = 0xE0;
		cmd.msg[1] = 0x10;
		cmd.msg[2] = 0x5A;
		cmd.msg[3] = (unicabletune >> 8) & 0xff;
		cmd.msg[4] = unicabletune & 0xff;
		cmd.msg_len = 5;
	}
	else
	{
		uint64_t unicabletune = 0;

		unicabletune |= ((satcr & 0x1F) << 19);
		//unicabletune |= (((node->feloffrequency / 1000) & 0x7ff) << 8);
		unicabletune |= (((node->feloffrequency) & 0x7ff) << 8);
		unicabletune |= (((aktdiseqc - 1) & 0x1) << 2);
		unicabletune |= (((!node->feaktpolarization) & 0x1) << 1);
		unicabletune |= (node->feaktband & 0x1);
		
		debug(200, "unicabletune 0x%06x", unicabletune);
//		printf("uni2-> tunigword: 0x%06x\n", unicabletune);
	
		if(status.firstunicablewait == 0)
		{
			status.firstunicablewait = getconfigint("firstunicablewait", NULL);
			if(status.firstunicablewait == 0)
				status.firstunicablewait = 1000;
		}		
	
		if(status.firstunicablewait > 0)
		{
			usleep(status.firstunicablewait * 1000);
			status.firstunicablewait = -1;
		}
	
		fesetvoltage(node, SEC_VOLTAGE_13, 15);
		fesetvoltage(node, SEC_VOLTAGE_18, 15);
		fesettone(node, SEC_TONE_OFF, 15);

		//feunicable 2
		//byte2 (bit 7/6/5/4/3) -> satcr number
		//byte2 (bit 2/1/0) -> frequ
		//byte1 -> frequ
		//byte0 (bit 3/1/0) -> lnb number
		
		cmd.msg[0] = 0x70;
		cmd.msg[1] = (unicabletune >> 16) & 0xff;
		cmd.msg[2] = (unicabletune >> 8) & 0xff;
		cmd.msg[3] = unicabletune & 0xff;
		cmd.msg_len = 4;
	}	
	
	debug(200, "send diseqc unicable cmd (%s)", node->feshortname);
	fediseqcsendmastercmd(node, &cmd, 100);
	fesetvoltage(node, SEC_VOLTAGE_13, 15);
}

void fediseqcset(struct dvbdev* node, struct transponder* tpnode)
{
	char* tmpstr = NULL;
	int toneburst = 0, cmdorder = 0, input = 0, uinput = 0, diseqmode = 0, rotorpos = 0, latpos = 0, longpos = 0;
	float latitude = 0, longitude = 0;
	fe_sec_mini_cmd_t mini = -1;
	struct dvb_diseqc_master_cmd cmd = {{0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 0};
	struct dvb_diseqc_master_cmd ucmd = {{0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 0};
	
	if(node == NULL) return;
	
	tmpstr = ostrcat(node->feshortname, "_diseqc_committedcmd", 0, 0);
	input = getconfigint(tmpstr, node->feaktnr);
	free(tmpstr); tmpstr = NULL;
	tmpstr = ostrcat(node->feshortname, "_diseqc_uncommittedcmd", 0, 0);
	uinput = getconfigint(tmpstr, node->feaktnr);
	free(tmpstr); tmpstr = NULL;
	tmpstr = ostrcat(node->feshortname, "_diseqc_mode", 0, 0);
	diseqmode = getconfigint(tmpstr, node->feaktnr);
	free(tmpstr); tmpstr = NULL;
	tmpstr = ostrcat(node->feshortname, "_diseqc_cmdorder", 0, 0);
	cmdorder = getconfigint(tmpstr, node->feaktnr);
	free(tmpstr); tmpstr = NULL;
	tmpstr = ostrcat(node->feshortname, "_diseqc_toneburst", 0, 0);
	toneburst = getconfigint(tmpstr, node->feaktnr);
	free(tmpstr); tmpstr = NULL;
	tmpstr = ostrcat(node->feshortname, "_diseqc_rotorpos", 0, 0);
	rotorpos = getconfigint(tmpstr, node->feaktnr);
	free(tmpstr); tmpstr = NULL;
	
	latitude = getconfigfloat("latitude", NULL);
	longitude = getconfigfloat("longitude", NULL);
	latpos = getconfigint("latpos", NULL);
	longpos = getconfigint("longpos", NULL);

	tmpstr = ostrcat(node->feshortname, "_diseqc", 0, 0);
	int aktdiseqc = getconfigint(tmpstr, node->feaktnr);
	if(aktdiseqc < 1) aktdiseqc = 1;
	free(tmpstr); tmpstr = NULL;

	debug(200, "set diseqc: number=%d, band=%d, pol=%d, diseqmode=%d, input=%d, uinput=%d, cmdorder=%d, toneburst=%d (%s)", aktdiseqc, node->feaktband, node->feaktpolarization, diseqmode, input, uinput, cmdorder, toneburst, node->feshortname);
	 
	switch(toneburst)
	{
		case 1: mini = SEC_MINI_A; break;
		case 2: mini = SEC_MINI_B; break;
	}
	
	if(diseqmode == 100) // Tonburst A/B
	{
		debug(200, "set diseqc: Tonburst A/B (%s)", node->feshortname);
		if(mini == -1)
			mini = (aktdiseqc - 1) % 2 ? SEC_MINI_B : SEC_MINI_A;
		fediseqcsendburst(node, mini, 15);
		return;
	}
		
	if(diseqmode == 0 || diseqmode == 1) // Diseqc 1.0 + 1.1
	{
		debug(200, "set committed switch (%s)", node->feshortname);
		fesdiseqcpoweron(node);
		cmd.msg[0] = 0xE0;
		cmd.msg[1] = 0x10;
		cmd.msg[2] = 0x38;

		if(input == 0)
			cmd.msg[3] = 0xF0 | ((((aktdiseqc - 1) * 4) & 0x0F) | (node->feaktband ? 1 : 0) | (node->feaktpolarization ? 0 : 2));
		else
			cmd.msg[3] = 0xF0 + ((input - 1) & 0x0F);

		cmd.msg_len = 4;
	}

	if(diseqmode == 1) // Diseqc 1.1
	{
		if(uinput > 0)
		{
			debug(200, "set uncommitted switch (%s)", node->feshortname);
			fesdiseqcpoweron(node);
			ucmd.msg[0] = 0xE0;
			ucmd.msg[1] = 0x10;
			ucmd.msg[2] = 0x39;
			ucmd.msg[3] = 0xF0 + ((uinput - 1) & 0x0F);
			ucmd.msg_len = 4;
		}
	}
		 
	switch(cmdorder)
	{
		case 1:
			if(mini != -1) fediseqcsendburst(node, mini, 15);
			fediseqcsendmastercmd(node, &cmd, 100);
			break;
		case 2:
			fediseqcsendmastercmd(node, &cmd, 100);
			if(uinput > 0) fediseqcsendmastercmd(node, &ucmd, 100);
			if(mini != -1) fediseqcsendburst(node, mini, 15);
			break;
		case 3:
			if(mini != -1) fediseqcsendburst(node, mini, 15);
			fediseqcsendmastercmd(node, &cmd, 100);
			if(uinput > 0) fediseqcsendmastercmd(node, &ucmd, 100);
			break;
		case 4:
			if(uinput > 0) fediseqcsendmastercmd(node, &ucmd, 100);
			fediseqcsendmastercmd(node, &cmd, 100);
			if(mini != -1) fediseqcsendburst(node, mini, 15);
			break;
		case 5:
			if(mini != -1) fediseqcsendburst(node, mini, 15);
			if(uinput > 0) fediseqcsendmastercmd(node, &ucmd, 100);
			fediseqcsendmastercmd(node, &cmd, 100);
			break;
		default:
			fediseqcsendmastercmd(node, &cmd, 100);
			if(mini != -1) fediseqcsendburst(node, mini, 15);
			break;
	}
	
	if(diseqmode == 2) // Diseqc 1.2
	{
		fediseqcrotor(node, tpnode, rotorpos, 8);
	}
	
	if(diseqmode == 3) // Diseqc 1.3 (USALS)
	{
		double orbitalpos = tpnode->orbitalpos / 10.00;

		if(latpos == 1) // south
			latitude = -latitude;

		if(longpos == 1) // west
			longitude = 360 - longitude;

		double satHourAngle = calcSatHourangle(orbitalpos, latitude, longitude);
		int gotoXTable[10] = {0x00, 0x02, 0x03, 0x05, 0x06, 0x08, 0x0A, 0x0B, 0x0D, 0x0E};

		if(latitude >= 0) // Northern Hemisphere
		{
			int tmp = (int)round(fabs(180 - satHourAngle) * 10.0);
			rotorpos = (tmp / 10) * 0x10 + gotoXTable[tmp % 10];

			if(satHourAngle < 180) // the east
				rotorpos |= 0xE000;
			else // west
				rotorpos |= 0xD000;
		}
		else // Southern Hemisphere
		{
			if(satHourAngle < 180) // the east
			{
				int tmp = (int)round(fabs(satHourAngle) * 10.0);
				rotorpos = (tmp / 10) * 0x10 + gotoXTable[tmp % 10];
				rotorpos |= 0xD000;
			}
			else // west
			{
				int tmp = (int)round(fabs(360 - satHourAngle) * 10.0);
				rotorpos = (tmp / 10) * 0x10 + gotoXTable[tmp % 10];
				rotorpos |= 0xE000;
			}
		}
		debug(200, "orbitalpos=%f, latitude=%f, longitude=%f, rotorpos=%04x", orbitalpos, latitude, longitude, rotorpos);

		fediseqcrotor(node, tpnode, rotorpos, 11);
	}
}

void feset(struct dvbdev* node, struct transponder* tpnode)
{
	int voltagemode = 0, tonemode = 0;
	fe_sec_tone_mode_t tone;
	fe_sec_voltage_t volt;
	struct dvbdev* dvbnode = dvbdev;
	char* tmpstr = NULL;

	if(node == NULL)
	{
		err("NULL detect");
		return;
	}

	// set volage off from other unused frontend
	while(dvbnode != NULL)
	{
		if(dvbnode->type != FRONTENDDEV) break;
		if(dvbnode->type == FRONTENDDEV && dvbnode != node && dvbnode->felock == 0 && dvbnode != status.aktservice->fedev)
		{
			fesetvoltage(dvbnode, SEC_VOLTAGE_OFF, 0);
		}
		dvbnode = dvbnode->next;
	}

	calclof(node, tpnode, NULL, 0);

	tmpstr = ostrcat(node->feshortname, "lnb_voltagemode", 0, 0);
	voltagemode = getconfigint(tmpstr, node->feaktnr);

	free(tmpstr); tmpstr = NULL;
	switch(voltagemode)
	{
		case 1: volt = SEC_VOLTAGE_13; break;
		case 2: volt = SEC_VOLTAGE_18; break;
		default: volt = node->feaktpolarization ? SEC_VOLTAGE_13 : SEC_VOLTAGE_18;
		         if(node->feunicable == 1) volt = SEC_VOLTAGE_13;
	}
	fesetvoltage(node, volt, 15);

	tmpstr = ostrcat(node->feshortname, "_diseqc", 0, 0);
	if(getconfigint(tmpstr, node->feaktnr) == 0 || node->feunicable == 1)
	{
		debug(200, "don't use diseqc");
	}
	else
	{
		fesettone(node, SEC_TONE_OFF, 15);
		fediseqcset(node, tpnode);
	}
	free(tmpstr); tmpstr = NULL;

	tmpstr = ostrcat(node->feshortname, "_lnb_tonemode", 0, 0);
	tonemode = getconfigint(tmpstr, node->feaktnr); 
	free(tmpstr); tmpstr = NULL;
	switch(tonemode)
	{
		case 1: tone = SEC_TONE_ON; break;
		case 2: tone = SEC_TONE_OFF; break;
		default: tone = node->feaktband ? SEC_TONE_ON : SEC_TONE_OFF;
		         if(node->feunicable == 1) tone = SEC_TONE_OFF;
	}
	fesettone(node, tone, 15);
}

void fediscard(struct dvbdev* node)
{
	struct dvb_frontend_event ev;
	int count = 0;

	if(node == NULL)
	{
		err("NULL detect");
		return;
	}

	/* discard stale QPSK events */
	while(count < 20)
	{
		count++;
		if(ioctl(node->fd, FE_GET_EVENT, &ev) == -1)
			break;
	}
}

uint16_t fereadsnr(struct dvbdev* node)
{
	uint16_t snr = 0;
	if(node == NULL)
	{
		err("NULL detect");
		return 0;
	}
	int signalquality = 0;
	int signalqualitydb = 0;
//#ifdef ARM
#ifdef MIPSEL
//#if DVB_API_VERSION >= 5
#if DVB_API_VERSION > 5 || DVB_API_VERSION == 5 && DVB_API_VERSION_MINOR >= 10

	int test1 = 0;
	int test2 = 0;
	struct dtv_property prop[1];
	memset(prop, 0, sizeof(prop));
	prop[0].cmd = DTV_STAT_CNR;
	struct dtv_properties props;
	props.props = prop;
	props.num = 1;
	
	int wasopen = 0;
	
	if(node->fd == -1)
		node->fd = feopen(node, NULL);
	else
		wasopen = 1;
		
	if(ioctl(node->fd, FE_GET_PROPERTY,  &props) < 0 && errno != ERANGE)
	{
		perr("FE_GET_PROPERTY");
	}
	else
	{
		for(unsigned int i=0; i<prop[0].u.st.len; i++)
		{
			if (prop[0].u.st.stat[i].scale == FE_SCALE_DECIBEL && test1 == 0)
			{
				test1 = 1;
				signalqualitydb = prop[0].u.st.stat[i].svalue / 10;
				printf("***** new snr signalqualitydb:%d\n", signalqualitydb);
			}
			else if (prop[0].u.st.stat[i].scale == FE_SCALE_RELATIVE && test2 == 0)
			{
				test2 = 1;
				signalquality = prop[0].u.st.stat[i].svalue;
				printf("***** new snr signalquality:%d\n", signalquality);
			}
		}
	}
	if(wasopen != 1)
		feclose(node, -1);
#endif
	if(!signalquality && !signalqualitydb)
	{
		int ret = 0x12345678;
		int sat_max = 1600; // we assume a max of 16db here
		int ter_max = 2900; // we assume a max of 29db here
		int cab_max = 4200; // we assume a max of 42db here
		//int atsc_max = 4200; // we assume a max of 42db here
		
		ioctl(node->fd, FE_READ_SNR, &snr);
		
		printf("***** old snr signalquality\n");		
		
		if(ostrstr(node->feinfo->name, "Si2166B") != NULL)
		{
			ret = (snr * 240) >> 8;
		}
		else if (ostrstr(node->feinfo->name, "ATBM781x") != NULL)
		{
			ret = snr*10;
		}
		else if(ostrstr(node->feinfo->name, "BCM4506") != NULL || ostrstr(node->feinfo->name, "BCM4506 (internal)") != NULL || ostrstr(node->feinfo->name, "BCM4505") != NULL || ostrstr(node->feinfo->name, "BCM73625 (G3)") != NULL || ostrstr(node->feinfo->name, "BCM45208") != NULL || fbctuner == 1)
		{
			ret = (snr * 100) >> 8;
		}
		else if (ostrstr(node->feinfo->name, "Si216") != NULL) // all new Models with SI Tuners
		{
			ret = snr;
		}
		signalqualitydb = ret;
		if (ret == 0x12345678) // no snr db calculation avail.. return untouched snr value..
		{
			signalquality = snr;
		}
		else
		{
			if(node->feinfo->type == FE_QPSK)
				signalquality = (ret >= sat_max ? 65536 : ret * 65536 / sat_max);
			else if(node->feinfo->type == FE_QAM)
				signalquality = (ret >= cab_max ? 65536 : ret * 65536 / cab_max);
			else if(node->feinfo->type == FE_OFDM)
				signalquality = (ret >= ter_max ? 65536 : ret * 65536 / ter_max);
		}
	}
	debug(200, "frontend snr = %02x", signalquality );
	return signalquality;
#else
	ioctl(node->fd, FE_READ_SNR, &snr);
	debug(200, "frontend snr = %02x", (snr * 100) / 0xffff);
	return snr;
#endif
}

uint16_t fereadsignalstrength(struct dvbdev* node)
{
	uint16_t signal = 0;

	if(node == NULL)
	{
		err("NULL detect");
		return 0;
	}
//#ifdef ARM
//#ifdef MIPSEL	

#ifdef ARM             
	struct dtv_property prop[1];
	prop[0].cmd = DTV_STAT_SIGNAL_STRENGTH;
	struct dtv_properties props;
	props.props = prop;
	props.num = 1;
	ioctl(node->fd, FE_GET_PROPERTY, &props);
	for(unsigned int i=0; i<prop[0].u.st.len; i++)
	{
		if (prop[0].u.st.stat[i].scale == FE_SCALE_RELATIVE)
			signal = prop[0].u.st.stat[i].uvalue;
	}
	if (!signal)
	{
		ioctl(node->fd, FE_READ_SIGNAL_STRENGTH, &signal);
		printf("brutto STRENGTH %02x\n", signal);
		if(ostrstr(node->feinfo->name, "Si2166B") != NULL)
		{
			if(signal == 128)
				signal = 0xffff;
			else if(signal > 128)
				signal =  0xffff - ((signal - 128) / 2 * 655);
			else if(signal < 128)
				signal =  0xffff - ((128 - signal) / 2 * 655);
		}
		printf("frontend signal = %02x\n", (signal * 100) / 0xffff);
	}
	return signal;
#else   

#if DVB_API_VERSION > 5 || DVB_API_VERSION == 5 && DVB_API_VERSION_MINOR >= 10
	struct dtv_property prop[1];
	memset(prop, 0, sizeof(prop));
	prop[0].cmd = DTV_STAT_SIGNAL_STRENGTH;
	struct dtv_properties props;
	props.props = prop;
	props.num = 1;
	
	int wasopen = 0;
	
	if(node->fd == -1)
		node->fd = feopen(node, NULL);
	else
		wasopen = 1;
	
	if (ioctl(node->fd, FE_GET_PROPERTY, &props) < 0 && errno != ERANGE)
	{
		debug(200, "DTV_STAT_SIGNAL_STRENGTH failed");
	}
	else
	{
		unsigned int i = 0;
		for(i=0; i<prop[0].u.st.len; i++)
		{
			if (prop[0].u.st.stat[i].scale == FE_SCALE_RELATIVE)
				signal = prop[0].u.st.stat[i].uvalue;
		}
	}
	if (signal)
	{
		if(wasopen != 1)
			feclose(node, -1);
		printf("new STRENGTH %d\n", signal);
		return signal;
	}
	// fallback to old DVB API
//#endif
#endif	
	if(ioctl(node->fd, FE_READ_SIGNAL_STRENGTH, &signal) < 0 && errno != ERANGE)
	{
		debug(200, "FE_READ_SIGNAL_STRENGTH failed");
	}
	else
	{
		printf("brutto STRENGTH %d tuner:%s\n", signal, node->feinfo->name );  
		if(ostrstr(node->feinfo->name, "Si2166B") != NULL || ostrstr(node->feinfo->name, "BCM45208") != NULL || fbctuner == 1)
			signal = signal * 257;
		debug(200, "frontend signal = %02x", (signal * 100) / 0xffff);
	}
	if(signal == 0) 
	{ 
		fe_status_t status = fereadstatus(node); 
		if(status & FE_HAS_LOCK) signal = 1; 
	}
	printf("old STRENGTH %d\n", signal);       
	return signal;
#endif
}


uint32_t fereadber(struct dvbdev* node)
{
	uint32_t ber = 0;

	if(node == NULL)
	{
		err("NULL detect");
		return 0;
	}

	ioctl(node->fd, FE_READ_BER, &ber);
	debug(200, "frontend ber = %02x", ber);
	return ber;
}

uint32_t fereaduncorrectedblocks(struct dvbdev* node)
{
	uint32_t unc = 0;

	if(node == NULL)
	{
		err("NULL detect");
		return 0;
	}

	ioctl(node->fd, FE_READ_UNCORRECTED_BLOCKS, &unc);
	debug(200, "frontend unc = %02x", unc);
	return unc;
}

fe_status_t fereadstatus(struct dvbdev* node)
{
	fe_status_t status;

	if(node == NULL)
	{
		err("NULL detect");
		return -1;
	}

	if(ioctl(node->fd, FE_READ_STATUS, &status) == -1)
		perr("FE_READ_STATUS");

	debug(200, "frontend status = %02x", status);
	if(status & FE_HAS_LOCK) debug(200, "frontend = FE_HAS_LOCK");
	if(status & FE_HAS_SIGNAL) debug(200, "frontend = FE_HAS_SIGNAL");
	if(status & FE_HAS_CARRIER) debug(200, "frontend = FE_HAS_CARRIER");
	if(status & FE_HAS_VITERBI) debug(200, "frontend = FE_HAS_VITERBI");
	if(status & FE_HAS_SYNC) debug(200, "frontend = FE_HAS_SYNC");
	if(status & FE_TIMEDOUT) debug(200, "frontend = FE_TIMEDOUT");
	if(status & FE_REINIT) debug(200, "frontend = FE_REINIT");

	return status;
}

int fetunedvbs(struct dvbdev* node, struct transponder* tpnode)
{
	if(node == NULL || tpnode == NULL)
	{
		err("NULL detect");
		return 1;
	}
	
	if(node->feunicable == 1)
	{
		fesetunicable(node);
		if(node->feloffrequency_uni == 0)
		{
			char* tmpstr = ostrcat(node->feshortname, "_lnb_satcrfrequ", 0, 0);
			node->feloffrequency = getconfigint(tmpstr, node->feaktnr) * 1000;
			free(tmpstr); tmpstr = NULL;
	  }
	  else
	  	node->feloffrequency = node->feloffrequency_uni;
	}

#if DVB_API_VERSION >= 5
	struct dtv_property p[10];
	struct dtv_properties cmdseq;
	cmdseq.props = p;

	//convert transponderlist for dvbapi5
	int system = tpnode->system;
	switch(system)
	{
		case 0: system = SYS_DVBS; break;
		case 1: system = SYS_DVBS2; break;
		default: system = SYS_DVBS; break;
	}

	int fec = tpnode->fec;
	switch(fec)
	{
		case 0: fec = FEC_AUTO; break;
		case 1: fec = FEC_1_2; break;
		case 2: fec = FEC_2_3; break;
		case 3: fec = FEC_3_4; break;
		case 4: fec = FEC_5_6; break;
		case 5: fec = FEC_7_8; break;
		case 6: fec = FEC_8_9; break;
		case 7: fec = FEC_3_5; break;
		case 8: fec = FEC_4_5; break;
		case 9: fec = FEC_9_10; break;
		case 15: fec = FEC_NONE; break;
		default: fec = FEC_AUTO; break;
	}
	int pilot = tpnode->pilot;
	switch(pilot)
	{
		case 0: pilot = PILOT_OFF; break;
		case 1: pilot = PILOT_ON; break;
		case 2: pilot = PILOT_AUTO; break;
		default: pilot = PILOT_AUTO; break;
	}
	int rolloff = tpnode->rolloff;
	switch(rolloff)
	{
		case 0: rolloff = ROLLOFF_35; break;
		case 1: rolloff = ROLLOFF_25; break;
		case 2: rolloff = ROLLOFF_20; break;
		default: rolloff = ROLLOFF_35; break;
	}

	int modulation = tpnode->modulation;
	switch(modulation)
	{
		case 0: modulation = QPSK; break;
		case 1: modulation = QPSK; break;
		case 2: modulation = PSK_8; break;
		case 3: modulation = QAM_16; break;
		default: modulation = QPSK; break;
	}

	p[0].cmd = DTV_CLEAR;
	p[1].cmd = DTV_DELIVERY_SYSTEM,	p[1].u.data = system;
	p[2].cmd = DTV_FREQUENCY,	p[2].u.data = node->feloffrequency;
	p[3].cmd = DTV_MODULATION,	p[3].u.data = modulation;
	p[4].cmd = DTV_SYMBOL_RATE,	p[4].u.data = tpnode->symbolrate;
	p[5].cmd = DTV_INNER_FEC,	p[5].u.data = fec;
	p[6].cmd = DTV_INVERSION,	p[6].u.data = (fe_spectral_inversion_t) tpnode->inversion;
	if(system == SYS_DVBS2)
	{
		p[7].cmd = DTV_ROLLOFF,		p[7].u.data = rolloff;
		p[8].cmd = DTV_PILOT,		p[8].u.data = pilot;
		p[9].cmd = DTV_TUNE;
		cmdseq.num = 10;
	}
	else
	{
		p[7].cmd = DTV_TUNE;
		cmdseq.num = 8;
	}

	debug(200, "new dvbapi: frequ=%d, inversion=%d, pilot=%d, rolloff=%d, fec=%d, sr=%d, modulation=%d, system=%d (%s)", node->feloffrequency, tpnode->inversion, pilot, rolloff, fec, tpnode->symbolrate, modulation, system, node->feshortname);
#else
	struct dvb_frontend_parameters tuneto;
	fe_spectral_inversion_t inversion = tpnode->inversion;

	//convert transponderlist for dvbapi3
	int fec = tpnode->fec;
	if(tpnode->system == 1)
	{
		if(tpnode->modulation == 1) fec = fec + 9;
		if(tpnode->modulation == 2) fec = fec + 18;
	}

	inversion |= (tpnode->rolloff << 2) | inversion; // use bit 2..3 of inversion for rolloff
	inversion |= (tpnode->pilot << 4) | inversion; // use bit 4..5 of inversion for pilot

	tuneto.frequency = node->feloffrequency;
	tuneto.inversion = inversion;
	tuneto.u.qpsk.symbol_rate = tpnode->symbolrate;
	tuneto.u.qpsk.fec_inner = fec;

	debug(200, "old dvbapi: frequ=%d, inversion=%d, pilot=%d, rolloff=%d, fec=%d, sr=%d modulation=%d, system=%d (%s)", node->feloffrequency, inversion, tpnode->pilot, tpnode->rolloff, fec, tpnode->symbolrate, tpnode->modulation, tpnode->system, node->feshortname);
#endif

	fediscard(node);

#if DVB_API_VERSION >= 5
	if((ioctl(node->fd, FE_SET_PROPERTY, &cmdseq)) == -1)
	{
		perr("FE_SET_PROPERTY");
		return 1;
	}
#else
	if(ioctl(node->fd, FE_SET_FRONTEND, &tuneto) == -1)
	{
		perr("FE_SET_FRONTEND");
		return 1;
	}
#endif

	return 0;
}

int fetunedvbc(struct dvbdev* node, struct transponder* tpnode)
{
	if(node == NULL || tpnode == NULL)
	{
		err("NULL detect");
		return 1;
	}
	
	int fec = tpnode->fec;
	switch(fec)
	{
		case 0: fec = FEC_AUTO; break;
		case 1: fec = FEC_1_2; break;
		case 2: fec = FEC_2_3; break;
		case 3: fec = FEC_3_4; break;
		case 4: fec = FEC_5_6; break;
		case 5: fec = FEC_7_8; break;
		case 6: fec = FEC_8_9; break;
		case 7: fec = FEC_3_5; break;
		case 8: fec = FEC_4_5; break;
		case 9: fec = FEC_9_10; break;
		case 15: fec = FEC_NONE; break;
		default: fec = FEC_AUTO; break;
	}

	int modulation = tpnode->modulation;
	switch(modulation)
	{
		case 0: modulation = QAM_AUTO; break;
		case 1: modulation = QAM_16; break;
		case 2: modulation = QAM_32; break;
		case 3: modulation = QAM_64; break;
		case 4: modulation = QAM_128; break;
		case 5: modulation = QAM_256; break;
		default: modulation = QAM_AUTO; break;
	}

#if DVB_API_VERSION >= 5
	struct dtv_property p[8];
	struct dtv_properties cmdseq;
	cmdseq.props = p;
	
	int system = tpnode->system;

	switch(system)
	{	
#ifdef DREAMBOX
		//case 0: system = SYS_DVBC_ANNEX_A; break;
		//case 1: system = SYS_DVBC_ANNEX_C; break;
		//default: system = SYS_DVBC_ANNEX_A; break;
		case 0: system = 1; break;
		case 1: system = 18; break;
		default: system = 1; break;
#elif ARM
		case 0: system = SYS_DVBC_ANNEX_A; break;
		case 1: system = SYS_DVBC_ANNEX_C; break;
		default: system = SYS_DVBC_ANNEX_A; break;
#else
		case 0: system = 1; break;
		case 1: system = 3; break;
		default: system = 1; break;
#endif
	}
	//Workaround... neue scan Tabelle ist in Khz und nicht mehr in Hz 
	if(tpnode->frequency < 1000000)
		tpnode->frequency = tpnode->frequency * 1000;
	p[0].cmd = DTV_CLEAR;
	p[1].cmd = DTV_DELIVERY_SYSTEM, p[1].u.data = system;
	p[2].cmd = DTV_FREQUENCY,	p[2].u.data = tpnode->frequency;
	p[3].cmd = DTV_MODULATION,	p[3].u.data = modulation;
	p[4].cmd = DTV_SYMBOL_RATE,	p[4].u.data = tpnode->symbolrate;
	p[5].cmd = DTV_INVERSION,	p[5].u.data = (fe_spectral_inversion_t) tpnode->inversion;
	p[6].cmd = DTV_INNER_FEC,	p[6].u.data = fec;
	p[7].cmd = DTV_TUNE;
	cmdseq.num = 8;

	debug(200, "new dvbapi: frequ=%d, inversion=%d, fec=%d, sr=%d, modulation=%d, system=%d (%s)", tpnode->frequency, tpnode->inversion, fec, tpnode->symbolrate, modulation, tpnode->system, node->feshortname);
#else
	struct dvb_frontend_parameters tuneto;
	tuneto.frequency = tpnode->frequency;
	tuneto.inversion = tpnode->inversion;
	tuneto.u.qam.symbol_rate = tpnode->symbolrate;
	tuneto.u.qam.fec_inner = tpnode->fec;
	tuneto.u.qam.modulation = tpnode->modulation;

	debug(200, "old dvbapi: frequ=%d, inversion=%d, fec=%d, sr=%d, modulation=%d (%s)", tpnode->frequency, tpnode->inversion, fec, tpnode->symbolrate, modulation, node->feshortname);
#endif

	fediscard(node);

#if DVB_API_VERSION >= 5
	if((ioctl(node->fd, FE_SET_PROPERTY, &cmdseq)) == -1)
	{
		perr("FE_SET_PROPERTY");
		return 1;
	}
#else
	if(ioctl(node->fd, FE_SET_FRONTEND, &tuneto) == -1)
	{
		perr("FE_SET_FRONTEND");
		return 1;
	}
#endif

	return 0;
}

int fetunedvbt(struct dvbdev* node, struct transponder* tpnode)
{
	if(node == NULL || tpnode == NULL)
	{
		err("NULL detect");
		return 1;
	}
	debug(200, "transponder:frequ=%d, inversion=%d, bandwidth=%d, hp=%d, lp=%d, modulation=%d transmission=%d guardinterval=%d hierarchy=%d system=%d (%s)", tpnode->frequency, tpnode->inversion, tpnode->symbolrate, tpnode->fec, tpnode->polarization, tpnode->modulation, tpnode->pilot, tpnode->rolloff, tpnode->system, tpnode->system, node->feshortname);
	
	char* tmpstr = ostrcat(node->feshortname, "_terr_volt", 0, 0);
	if(getconfigint(tmpstr, NULL) == 1)
		fesetvoltage(node, SEC_VOLTAGE_13, 10);
	else
//		fesetvoltage(node, SEC_VOLTAGE_OFF, 10);
		//99 gleich SEC_VOLTAGE_OFF nur ohne loeschen node
		fesetvoltage(node, 99, 10);
	free(tmpstr); tmpstr = NULL;
	
	
	fe_delivery_system_t system = tpnode->system;
	
	int hp = tpnode->fec; //fec = hp on DVBT
	switch(hp)
	{
		case T_FEC_1_2: hp = FEC_1_2; break;
		case T_FEC_2_3: hp = FEC_2_3; break;
		case T_FEC_3_4: hp = FEC_3_4; break;
		case T_FEC_5_6: hp = FEC_5_6; break;
		case T_FEC_6_7: hp = FEC_6_7; break;	
		case T_FEC_7_8: hp = FEC_7_8; break;
		case T_FEC_8_9: hp = FEC_8_9; break;
		default: hp = FEC_AUTO; break;
	}

	int lp = tpnode->polarization; //polarization = lp on DVBT
	switch(lp)
	{
		case T_FEC_1_2: lp = FEC_1_2; break;
		case T_FEC_2_3: lp = FEC_2_3; break;
		case T_FEC_3_4: lp = FEC_3_4; break;
		case T_FEC_5_6: lp = FEC_5_6; break;
		case T_FEC_6_7: lp = FEC_6_7; break;	
		case T_FEC_7_8: lp = FEC_7_8; break;
		case T_FEC_8_9: lp = FEC_8_9; break;
		default: lp = FEC_AUTO; break;
	}

	int modulation = tpnode->modulation;
	switch(modulation)
	{
		case T_Modulation_QPSK: modulation = QPSK; break;
		case T_Modulation_QAM16: modulation = QAM_16; break;
		case T_Modulation_QAM64: modulation = QAM_64; break;
		case T_Modulation_QAM256: modulation = QAM_256; break;
		default: modulation = QAM_AUTO; break;
	}

	int bandwidth = tpnode->symbolrate; //symbolrate = bandwidth on DVBT
	switch(bandwidth)
	{
		case T_Bandwidth_8MHz: bandwidth = 8000000; break;
		case T_Bandwidth_7MHz: bandwidth = 7000000; break;
		case T_Bandwidth_6MHz: bandwidth = 6000000; break;
		case T_Bandwidth_5MHz: bandwidth = 5000000; break;
		case T_Bandwidth_1_712MHz: bandwidth = 1712000; break;
		case T_Bandwidth_10MHz: bandwidth = 10000000; break;	
		case T_Bandwidth_Auto: bandwidth = 0; break;
		default: bandwidth = 0; break;
	}
	
	int transmission = tpnode->pilot; //pilot = transmission on DVBT
	switch(transmission)
	{
		case T_TransmissionMode_2k: transmission = TRANSMISSION_MODE_2K; break;
		case T_TransmissionMode_4k: transmission = TRANSMISSION_MODE_4K; break;	
		case T_TransmissionMode_8k: transmission = TRANSMISSION_MODE_8K; break;
		case T_TransmissionMode_Auto: transmission = TRANSMISSION_MODE_AUTO; break;
#if defined TRANSMISSION_MODE_1K
		case T_TransmissionMode_1k: transmission = TRANSMISSION_MODE_1K; break;
		case T_TransmissionMode_16k: transmission = TRANSMISSION_MODE_16K; break;
		case T_TransmissionMode_32k: transmission = TRANSMISSION_MODE_32K; break;
#endif
		default: transmission = TRANSMISSION_MODE_AUTO; break;
	}

	int guardinterval = tpnode->rolloff; //rolloff = guardinterval on DVBT
	switch(guardinterval)
	{
		case T_GuardInterval_1_32: guardinterval = GUARD_INTERVAL_1_32; break;
		case T_GuardInterval_1_16: guardinterval = GUARD_INTERVAL_1_16; break;
		case T_GuardInterval_1_8: guardinterval = GUARD_INTERVAL_1_8; break;
		case T_GuardInterval_1_4: guardinterval = GUARD_INTERVAL_1_4; break;
		case T_GuardInterval_Auto: guardinterval = GUARD_INTERVAL_AUTO; break;
#if defined GUARD_INTERVAL_1_128
		case T_GuardInterval_1_128: guardinterval = GUARD_INTERVAL_1_128; break;
		case T_GuardInterval_19_128: guardinterval = GUARD_INTERVAL_19_128; break;
		case T_GuardInterval_19_256: guardinterval = GUARD_INTERVAL_19_256; break;
#endif
		default: guardinterval = GUARD_INTERVAL_AUTO; break;
	}
	
	int hierarchy = tpnode->system; //system = hierarchy on DVBT
	if(tpnode->system == System_DVB_T2) //system = DVB-T2 then hierarchy = HIERARCHY_AUTO
		hierarchy = T_Hierarchy_Auto;
	
	switch(hierarchy)
	{
		case T_Hierarchy_None: hierarchy = HIERARCHY_NONE;
		case T_Hierarchy_1: hierarchy = HIERARCHY_1;
		case T_Hierarchy_2: hierarchy = HIERARCHY_2;
		case T_Hierarchy_4: hierarchy = HIERARCHY_4;
		case T_Hierarchy_Auto: hierarchy = HIERARCHY_AUTO;
		default: hierarchy = HIERARCHY_AUTO; break;
	}
	
	int inversion = tpnode->inversion;
	switch(inversion)
	{
		case T_Inversion_Off: inversion = INVERSION_OFF;
		case T_Inversion_On: inversion = INVERSION_ON;
		default: inversion = INVERSION_AUTO; break;
	}

	int ret = 0;
#ifndef ARM
	fediscard(node);
#endif

#if DVB_API_VERSION >= 5
	struct dtv_property p[13];
	struct dtv_properties cmdseq;
	cmdseq.props = p;

// suchlauf geht an nemesis mit system=0
	switch(system)
	{
		case System_DVB_T: system = SYS_DVBT; break; //3
		case System_DVB_T2: system = SYS_DVBT2; break; //16
		default: system = SYS_DVBT; break;
	}

	p[0].cmd = DTV_CLEAR;
	p[1].cmd = DTV_DELIVERY_SYSTEM, p[1].u.data = system;
	p[2].cmd = DTV_FREQUENCY,	p[2].u.data = tpnode->frequency;
	p[3].cmd = DTV_INVERSION,	p[3].u.data = (fe_spectral_inversion_t) inversion;
	p[4].cmd = DTV_CODE_RATE_LP, p[4].u.data = (fe_code_rate_t) lp;
	p[5].cmd = DTV_CODE_RATE_HP, p[5].u.data = (fe_code_rate_t) hp;
	p[6].cmd = DTV_MODULATION, p[6].u.data = (fe_modulation_t) modulation;
	p[7].cmd = DTV_TRANSMISSION_MODE,	p[7].u.data = (fe_transmit_mode_t) transmission;
	p[8].cmd = DTV_GUARD_INTERVAL, p[8].u.data = (fe_guard_interval_t) guardinterval;
	p[9].cmd = DTV_HIERARCHY, p[9].u.data = (fe_hierarchy_t) hierarchy;
	p[10].cmd = DTV_BANDWIDTH_HZ, p[10].u.data = bandwidth;
		
	if(system == SYS_DVBT2)
	{
#if defined DTV_STREAM_ID
		p[11].cmd = DTV_STREAM_ID, p[11].u.data = 0;
		p[12].cmd = DTV_TUNE;
		cmdseq.num = 13;
#elif defined DTV_DVBT2_PLP_ID
		p[11].cmd = DTV_DVBT2_PLP_ID, p[11].u.data = 0;
		p[12].cmd = DTV_TUNE;
		cmdseq.num = 13;
#else
		p[11].cmd = DTV_TUNE;
		cmdseq.num = 12;
#endif
	}
	else
	{
		p[11].cmd = DTV_TUNE;
		cmdseq.num = 12;
	}
	
	if((ioctl(node->fd, FE_SET_PROPERTY, &cmdseq)) == -1)
	{
		perr("FE_SET_PROPERTY");
		ret = 1;
	}

	debug(200, "new dvbapi 1: frequ=%d, inversion=%d, bandwidth=%d, hp=%d, lp=%d, modulation=%d transmission=%d guardinterval=%d hierarchy=%d system=%d (%s)", tpnode->frequency, tpnode->inversion, bandwidth, hp, lp, modulation, transmission, guardinterval, hierarchy, system, node->feshortname);

#else
	struct dvb_frontend_parameters tuneto;

	tuneto.frequency = tpnode->frequency;
	tuneto.inversion = tpnode->inversion;
	tuneto.u.ofdm.bandwidth = bandwidth;
	tuneto.u.ofdm.code_rate_HP = hp;
	tuneto.u.ofdm.code_rate_LP = lp;
	tuneto.u.ofdm.constellation = modulation;
	tuneto.u.ofdm.transmission_mode = transmission;
	tuneto.u.ofdm.guard_interval = guardinterval;
	tuneto.u.ofdm.hierarchy_information = hierarchy;
	
	if(ioctl(node->fd, FE_SET_FRONTEND, &tuneto) == -1)
	{
		perr("FE_SET_FRONTEND");
		ret = 1;
	}

	debug(200, "old dvbapi 1: frequ=%d, inversion=%d, bandwidth=%d, hp=%d, lp=%d, modulation=%d transmission=%d guardinterval=%d hierarchy=%d system=%d (%s)", tpnode->frequency, tpnode->inversion, bandwidth, hp, lp, modulation, transmission, guardinterval, hierarchy, system, node->feshortname);

#endif
	return ret;
}

#ifdef SIMULATE
int tunercount = 0;
#endif
struct dvb_frontend_info* fegetinfo(struct dvbdev* node, int fd)
{
	struct dvb_frontend_info* feinfo = NULL;
	int tmpfd = -1;

	if(node != NULL)
		tmpfd = node->fd;
	else
		tmpfd = fd;

	feinfo = (struct dvb_frontend_info*)malloc(sizeof(struct dvb_frontend_info));
	if(feinfo == NULL)
	{
		err("no mem");
		return NULL;
	}

#ifndef SIMULATE
	if(ioctl(tmpfd, FE_GET_INFO, feinfo) < 0)
	{
		perr("FE_GET_INFO");
		free(feinfo);
		return NULL;
	}
#else
	tunercount++;
	if(tunercount == 1)
	{
		sprintf(feinfo->name, "%s", "Conax 7500 DVB-C");
		feinfo->type = FE_QAM;
	}
	else
	{
		sprintf(feinfo->name, "%s", "Conax 7500 DVB-S");
		feinfo->type = FE_QPSK;
		//feinfo->type = FE_QAM;
	}
#endif
	return feinfo;
}

#if defined DTV_ENUM_DELSYS
int fesetDeliverySystem(fe_delivery_system_t delsys, int fd)
{
	struct dtv_property p[2];
	memset(p, 0, sizeof(p));
	struct dtv_properties cmdseq;
	cmdseq.props = p;
	cmdseq.num = 2;
	p[0].cmd = DTV_CLEAR;
	p[1].cmd = DTV_DELIVERY_SYSTEM;
	p[1].u.data = delsys;
	if (ioctl(fd, FE_SET_PROPERTY, &cmdseq) == -1)
	{
		perr("[DTV_ENUM_DELSYS] FE_SET_PROPERTY failed");
		return 0;
	}
	return 1;
}
#endif

int fegetdev()
{
	int i, y, fd = -1, count = 0;
	char *buf = NULL, *frontenddev = NULL, *fehyprid = NULL, *tmpstr = NULL, *tmpstr1 = NULL;
	struct dvb_frontend_info* feinfo = NULL;
	struct dvbdev* dvbnode = NULL;

	frontenddev = getconfig("frontenddev", NULL);
	if(frontenddev == NULL)
	{
		err("NULL detect");
		return count;
	}

	buf = malloc(MINMALLOC);
	if(buf == NULL)
	{
		err("no memory");
		return count;
	}

#if defined DTV_ENUM_DELSYS	
	if(getconfig("tempnim", NULL) == NULL)
		addconfig("tempnim", "/tmp/tempnim");
	if(file_exist(getconfig("tempnim", NULL)) == 1)
		unlink(getconfig("tempnim", NULL)); 
#endif
	
	for(i = 0; i < MAXDVBADAPTER; i++)
	{
		for(y = 0; y < MAXFRONTENDDEV; y++)
		{
			sprintf(buf, frontenddev, i, y);
			fd = feopen(NULL, buf);
			if(fd >= 0)
			{
				
#if defined DTV_ENUM_DELSYS
				struct dtv_property p[1];
				memset(p, 0, sizeof(p));
				p[0].cmd = DTV_ENUM_DELSYS;
				struct dtv_properties cmdseq;
				cmdseq.num = 1;
				cmdseq.props = p;
				int type = -1;
				int mz = -1;
				struct dvb_frontend_info* t_feinfo = NULL;
					
				//tricky... tempor\E4re nim_sockets bauen da ansonsten viel im code ge\E4ndert werden m\FC\DFte
				free(tmpstr); tmpstr= NULL;
				free(tmpstr1); tmpstr1= NULL;
				tmpstr = ostrcat("NIM Socket " , oitoa(y), 0, 1);
				tmpstr = ostrcat(tmpstr , ":\n", 1, 0);
				
				t_feinfo = (struct dvb_frontend_info*)malloc(sizeof(struct dvb_frontend_info));
				if (ioctl(fd, FE_GET_PROPERTY, &cmdseq) >= 0)
				{
					for (; p[0].u.buffer.len > 0; p[0].u.buffer.len--)
					{
						memset(t_feinfo, 0, sizeof(struct dvb_frontend_info));
						fe_delivery_system_t delsys = (fe_delivery_system_t)p[0].u.buffer.data[p[0].u.buffer.len - 1];
						printf("--------> delsys %d devive: %d\n", delsys, y);
						fesetDeliverySystem(delsys, fd);
						if (ioctl(fd, FE_GET_INFO, t_feinfo) < 0)
						{
				  		perr("[DTV_ENUM_DELSYS] ioctl FE_GET_INFO failed");
				  	}
				  	else
				  	{
				  		printf("------> name %s %d\n", t_feinfo->name, t_feinfo->type);
				  		if(ostrstr(t_feinfo->name, "Si2166D") != NULL && y == 0) //tripple Tuner DM900
							{
				  			mz = 0;
				  			break;
				  		}
				  		if(type != t_feinfo->type)
				  		{
				  			type = t_feinfo->type;
				  			mz = mz + 1;
				  			switch (type)
								{
									case FE_QPSK:
									{
										tmpstr = ostrcat(tmpstr , "        Mode ", 1, 0);
										tmpstr = ostrcat(tmpstr , oitoa(feSatellite), 1, 1);
										tmpstr = ostrcat(tmpstr , ": DVB-S/S2\n", 1, 0);
										//tmpstr1 = ostrcat(tmpstr1 , oitoa(feSatellite), 1, 1);
										//tmpstr1 = ostrcat(tmpstr1 , "\n", 1, 0);
												
										break;
									}
									case FE_QAM:
									{
										tmpstr = ostrcat(tmpstr , "        Mode ", 1, 0);
										tmpstr = ostrcat(tmpstr , oitoa(feCable), 1, 1);
										tmpstr = ostrcat(tmpstr , ": DVB-C\n", 1, 0);
										//tmpstr1 = ostrcat(tmpstr1 , oitoa(feCable), 1, 1);
										//tmpstr1 = ostrcat(tmpstr1 , "\n", 1, 0);
										break;
									}
									case FE_OFDM:
									{
										tmpstr = ostrcat(tmpstr , "        Mode ", 1, 0);
										tmpstr = ostrcat(tmpstr , oitoa(feTerrestrial), 1, 1);
										tmpstr = ostrcat(tmpstr , ": DVB-T/T2\n", 1, 0);
										//tmpstr1 = ostrcat(tmpstr1 , oitoa(feTerrestrial), 1, 1);
										//tmpstr1 = ostrcat(tmpstr1 , "\n", 1, 0);
										break;
									}
				  			}
				  		}
						}
					}
					if(mz > 0)
					{
						tmpstr = ostrcat(tmpstr , "        Frontend_Device: ", 1, 0);	
						tmpstr = ostrcat(tmpstr , oitoa(y), 1, 1);
						tmpstr = ostrcat(tmpstr , "\n", 1, 0);		
						tmpstr = ostrcat(tmpstr , "        I2C_Device: x\n", 1, 0);	
						writesys(getconfig("tempnim", NULL), tmpstr, 2);
						//fehyprid = ostrcat(tmpstr1 , NULL, 0, 0);	
					}
					free(tmpstr); tmpstr = NULL;
					free(tmpstr1); tmpstr1 = NULL;
				}
				else
				{
					perr("[DTV_ENUM_DELSYS] ioctl FE_GET_PROPERTY failed");
				}
				free(t_feinfo); t_feinfo = NULL;
#endif				
				
				fehyprid = gethypridtunerchoicesvalue(y);
				
				if(fehyprid != NULL)
				{
					if(y < 10)
						tmpstr = ostrcat(tmpstr, "fe_0", 1, 0);
					else
						tmpstr = ostrcat(tmpstr, "fe_1", 1, 0);

					tmpstr = ostrcat(tmpstr, oitoa(y), 1, 1);
					tmpstr = ostrcat(tmpstr, "_hyprid", 1, 0);
					tmpstr1 = ostrcat(tmpstr, "_last", 0, 0);
					addconfig(tmpstr1, "999"); //first set
					free(tmpstr1), tmpstr1 = NULL;
#ifndef MIPSEL
					if(getconfig(tmpstr, NULL) != NULL)
						sethypridtuner(y, getconfig(tmpstr, NULL));
					free(tmpstr), tmpstr = NULL;
#endif
				}
				
				feinfo = fegetinfo(NULL, fd);

				if(feinfo != NULL)
				{
					count++;
					
					//workaround zu testen fuer mega
					/*if(checkbox("VUZERO4K") == 1)
					{
						sprintf(feinfo->name, "%s", "Mega Tuner DVB-S");
					}*/
					
					dvbnode = adddvbdev(buf, i, y, fd, FRONTENDDEV, feinfo, NULL, fehyprid, 0);
#ifdef MIPSEL
					if(fehyprid != NULL && getconfig(tmpstr, NULL) != NULL)
					{
						sethypridtunernew(dvbnode, getconfig(tmpstr, NULL));
						feinfo = fegetinfo(NULL, fd);
						dvbnode->feinfo = feinfo;
					}
					free(tmpstr), tmpstr = NULL;
#endif					
					if(dvbnode->feinfo->type == FE_QPSK)
						fesetvoltage(dvbnode, SEC_VOLTAGE_OFF, 15);
						
					if(y < 10)
						tmpstr = ostrcat(tmpstr, "fe_0", 1, 0);
					else
						tmpstr = ostrcat(tmpstr, "fe_1", 1, 0);
					tmpstr = ostrcat(tmpstr, oitoa(y), 1, 1);
					tmpstr = ostrcat(tmpstr, "_fbc", 1, 0);
					if(ostrstr(feinfo->name, "BCM45208") != NULL || fbctuner == 1) //fbc Tuner
					{
						if(getconfig(tmpstr, NULL) == NULL)
							addconfig(tmpstr, "A");
					}
					else
						addconfig(tmpstr, "");
					if((strstr(feinfo->name, "Si2166B") != NULL || ostrstr(feinfo->name, "Si2166D") != NULL) && y == 0 ) //test Tuner DM920 oder erster DM900
					{
#ifdef MIPSEL						
						fechangetype2(dvbnode, "DVB-S", "DVB-S");
#endif							
					}
					free(tmpstr), tmpstr = NULL;
				}
				if(tmpstr != NULL)
					free(tmpstr), tmpstr = NULL;
			}
		}
	}

	free(buf);
	return count;
}

int fecreatedummy()
{
	//create dummy frontend for playback
	char *buf = NULL, *frontenddev = NULL;
	struct dvbdev* dvbnode = NULL;

	frontenddev = getconfig("frontenddev", NULL);
	if(frontenddev == NULL)
	{
		err("NULL detect");
		return 1;
	}

	buf = malloc(MINMALLOC);
	if(buf == NULL)
	{
		err("no memory");
		return 1;
	}

	dvbnode = dmxgetlast(0);
	if(dvbnode != NULL)
	{
		sprintf(buf, frontenddev, 0, dvbnode->devnr);
		adddvbdev(buf, 0, dvbnode->devnr, -1, FRONTENDDEVDUMMY, NULL, NULL, NULL, 0);
	}

	free(buf);
	return 0;
}

int fegetlock(int tunernr)
{
	struct dvbdev* dvbnode = NULL;
	dvbnode = dvbdev;

	while(dvbnode != NULL)
	{
		if(dvbnode->deactive == 1)
		{
			dvbnode = dvbnode->next;
			continue;
		}
		if(dvbnode->type == FRONTENDDEV && dvbnode->devnr == tunernr)
		{
			return dvbnode->felock;
		}
		dvbnode = dvbnode->next;
	}
	return -1;
}

#ifdef MIPSEL
int fechangetype(struct dvbdev* tuner, char* value)
{
#if DVB_API_VERSION >= 5	

	struct dtv_property p[2];
	memset(p, 0, sizeof(p));
	struct dtv_properties cmdseq;
	cmdseq.props = p;
	cmdseq.num = 2;
	p[0].cmd = DTV_CLEAR;
	p[1].cmd = DTV_DELIVERY_SYSTEM;
	p[1].u.data = SYS_UNDEFINED;
	char* buf = NULL, *hypridtuner = NULL;
	int type = 0;
	int ret = 0;
	int wasopen = 0;
	char* realname = gethypridtunerchoicesvaluename(tuner->devnr, value);
	char* tmpstr = NULL;
	struct dvb_frontend_info* feinfo = NULL;
	
	if(tuner->fd == -1)
	{
		if(feopen(tuner, NULL) < 0)
		{
			err("open tuner dev");
		}
	}
	else
		wasopen = 1;
	
	printf("**** > realname: %s\n", realname);
	
	if(realname != NULL && ostrstr(realname, "DVB-S") != NULL)
		type = feSatellite;
	else if(realname != NULL && ostrstr(realname, "DVB-C") != NULL)
		type = feCable;
	else if(realname != NULL && ostrstr(realname, "DVB-T") != NULL)
		type = feTerrestrial;
	else
		type = -1;
	
	switch (type)
	{
		case feSatellite:
		{
			p[1].u.data = SYS_DVBS;
			break;
		}
		case feTerrestrial:
		{
			p[1].u.data = SYS_DVBT;
			break;
		}
		case feCable:
		{
			 fesetvoltage(tuner, SEC_VOLTAGE_OFF, 10);
#ifdef SYS_DVBC_ANNEX_A
			p[1].u.data = SYS_DVBC_ANNEX_A;
#else
			p[1].u.data = SYS_DVBC_ANNEX_AC;
#endif
			break;
		}
#ifdef feATSC
		case feATSC:
		{
			p[1].u.data = SYS_ATSC;
			break;
		}
#endif
		default:
			debug(200, "not supported delivery system type %i", type);
			return 0; //false
	}
	debug(200, "data %d",p[1].u.data );
	ret = 0;
	if (ioctl(tuner->fd, FE_SET_PROPERTY, &cmdseq) == -1)
	{
		perr("FE_SET_PROPERTY");
		err("FE_SET_PROPERTY failed -> use procfs to switch delivery system tuner %d mode %s type %d",tuner->devnr ,value, type);
		ret = -1;

		hypridtuner = getconfig("hypridtuner", NULL);
		if(hypridtuner != NULL)
		{
			buf = malloc(MINMALLOC);
			if(buf == NULL)
			{
				err("no memory");
				return 0;
			}
		}
		sprintf(buf, hypridtuner, tuner->devnr);
		if(file_exist(buf))
		{
			if(tuner->fd > -1)
			{
				feclose(tuner, -1);
				ret = writesys(buf, value, 0);
				tuner->fd = feopen(tuner, NULL);
			}
			else
				ret = writesys(buf, value, 0);
			printf("set %s to %s RC:%i\n", buf, value, ret);
		}
		else
			printf("set system tuner to %s ... file not found -> %s\n", value, buf);
		free(buf); buf = NULL;
	}
	else
	{
		printf("fe set property value %s data %d ... RC:%i\n", value, p[1].u.data, ret);
		feinfo = fegetinfo(NULL, tuner->fd);
		if(feinfo != NULL)
		{
			free(tuner->feinfo);
			tuner->feinfo = NULL;
			tuner->feinfo = feinfo;
		}
		
	}
	
	if(type == feTerrestrial)
	{
		tmpstr = ostrcat(tuner->feshortname, "_terr_volt", 0, 0);
		if(getconfigint(tmpstr, NULL) == 1)
			fesetvoltage(tuner, SEC_VOLTAGE_13, 10);
		else
			fesetvoltage(tuner, SEC_VOLTAGE_OFF, 10);
		free(tmpstr); tmpstr = NULL;
	}

	if(wasopen != 1)
		feclose(tuner, -1);
	if(ret > -1)
		return 1; //true
	else
		return 0; //false

#else //if DVB_API_VERSION < 5
	printf("Hinweis -> DVB API kleiner Version 5\n");
	if(sethypridtuner(tuner, value) == 0)
		return 1;
	return 0; //false
#endif
}		
#endif

#ifdef MIPSEL
int fechangetype2(struct dvbdev* tuner, char* value, char* realname)
{
	struct dtv_property p[2];
	memset(p, 0, sizeof(p));
	struct dtv_properties cmdseq;
	cmdseq.props = p;
	cmdseq.num = 2;
	p[0].cmd = DTV_CLEAR;
	p[1].cmd = DTV_DELIVERY_SYSTEM;
	p[1].u.data = SYS_UNDEFINED;
	char* buf = NULL, *hypridtuner = NULL;
	int type = 0;
	int ret = 0;
	int wasopen = 0;
	char* tmpstr = NULL;
	
	if(tuner->fd == -1)
	{
		if(feopen(tuner, NULL) < 0)
			err("open tuner dev");
	}
	else
		wasopen = 1;
	
	printf("**** > realname: %s\n", realname);
	
	if(realname != NULL && ostrstr(realname, "DVB-S") != NULL)
		type = feSatellite;
	else if(realname != NULL && ostrstr(realname, "DVB-C") != NULL)
		type = feCable;
	else if(realname != NULL && ostrstr(realname, "DVB-T") != NULL)
		type = feTerrestrial;
	else
		type = -1;
	
	switch (type)
	{
		case feSatellite:
		{
			p[1].u.data = SYS_DVBS;
			break;
		}
		case feTerrestrial:
		{
			p[1].u.data = SYS_DVBT;
			break;
		}
		case feCable:
		{
			 fesetvoltage(tuner, SEC_VOLTAGE_OFF, 10);
#ifdef SYS_DVBC_ANNEX_A
			p[1].u.data = SYS_DVBC_ANNEX_A;
#else
			p[1].u.data = SYS_DVBC_ANNEX_AC;
#endif
			break;
		}
#ifdef feATSC
		case feATSC:
		{
			p[1].u.data = SYS_ATSC;
			break;
		}
#endif
		default:
			debug(200, "not supported delivery system type %i", type);
			return 0; //false
	}
	debug(200, "data %d",p[1].u.data );
	ret = 0;
	if (ioctl(tuner->fd, FE_SET_PROPERTY, &cmdseq) == -1)
	{
		perr("FE_SET_PROPERTY");
	}
	else
	{
		printf("fe set property value %s data %d ... RC:%i\n", value, p[1].u.data, ret);
	}
	
	if(type == feTerrestrial)
	{
		tmpstr = ostrcat(tuner->feshortname, "_terr_volt", 0, 0);
		if(getconfigint(tmpstr, NULL) == 1)
			fesetvoltage(tuner, SEC_VOLTAGE_13, 10);
		else
			fesetvoltage(tuner, SEC_VOLTAGE_OFF, 10);
		free(tmpstr); tmpstr = NULL;
	}

	if(wasopen != 1)
		feclose(tuner, -1);
	if(ret > -1)
		return 1; //true
	else
		return 0; //false
}		
#endif
	

#endif
