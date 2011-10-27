#ifndef TRANSPONDER_H
#define TRANSPONDER_H

void deltranspondertunablestatus()
{
	struct transponder* node = transponder;

	while(node != NULL)
	{
		node->tunablestatus = 0;
		node = node->next;
	}
}

char* transpondergetpilotstr(struct transponder* node, int flag)
{
	char* text = NULL;

	if(flag == 0)
	{
		if(node == NULL)
		{
			debug(1000, "out -> NULL detect");
			return NULL;
		}

		switch(node->pilot)
		{	
			case 0: text = ostrcat(text, "Off", 1, 0); break;
			case 1: text = ostrcat(text, "On", 1, 0); break;
			case 2: text = ostrcat(text, "Auto", 1, 0); break;
			default: text = ostrcat(text, "unknown", 1, 0);
		}
	}
	else if (flag == 1)
		text = ostrcat(text, "Off\nOn\nAuto", 1, 0);
	else if (flag == 2)
		text = ostrcat(text, "0\n1\n2", 1, 0);

	return text;
}

char* transpondergetrolloffstr(struct transponder* node, int flag)
{
	char* text = NULL;

	if(flag == 0)
	{
		if(node == NULL)
		{
			debug(1000, "out -> NULL detect");
			return NULL;
		}

		switch(node->rolloff)
		{	
			case 0: text = ostrcat(text, "0.35", 1, 0); break;
			case 1: text = ostrcat(text, "0.20", 1, 0); break;
			case 2: text = ostrcat(text, "0.25", 1, 0); break;
			case 3: text = ostrcat(text, "Auto", 1, 0); break;
			default: text = ostrcat(text, "unknown", 1, 0);
		}
	}
	else if(flag == 1)
		text = ostrcat(text, "0.35\n0.20\n0.25\nAuto", 1, 0);
	else if(flag == 2)
		text = ostrcat(text, "0\n1\n2\n3", 1, 0);

	return text;
}

char* transpondergetfecstr(struct transponder* node, int flag)
{
	char* text = NULL;

	if(flag == 0)
	{
		if(node == NULL)
		{
			debug(1000, "out -> NULL detect");
			return NULL;
		}

		switch(node->fec)
		{	
			case FEC_AUTO: text = ostrcat(text, "AUTO", 1, 0); break;
			case FEC_1_2: text = ostrcat(text, "1/2", 1, 0); break;
			case FEC_2_3: text = ostrcat(text, "2/3", 1, 0); break;
			case FEC_3_4: text = ostrcat(text, "3/4", 1, 0); break;
			case FEC_5_6: text = ostrcat(text, "5/6", 1, 0); break;
			case FEC_7_8: text = ostrcat(text, "7/8", 1, 0); break;
			case FEC_8_9: text = ostrcat(text, "8/9", 1, 0); break;
			case FEC_4_5: text = ostrcat(text, "4/5", 1, 0); break;
			case FEC_NONE: text = ostrcat(text, "NONE", 1, 0); break;
			case FEC_6_7: text = ostrcat(text, "6/7", 1, 0); break;
#if DVB_API_VERSION <= 3
			case FEC_1_4: text = ostrcat(text, "1/4", 1, 0); break;
			case FEC_1_3: text = ostrcat(text, "1/3", 1, 0); break;
			case FEC_2_5: text = ostrcat(text, "2/5", 1, 0); break;
#endif
			case FEC_3_5: text = ostrcat(text, "3/5", 1, 0); break;
			case FEC_9_10: text = ostrcat(text, "9/10", 1, 0); break;
			default: text = ostrcat(text, "unknown", 1, 0);
		}
	}
	else if(flag == 1)
	{
		text = ostrcat(text, "AUTO\n1/2\n2/3\n3/4\n3/5\n4/5\n5/6\n6/7\n7/8\n8/9\n9/10", 1, 0);
#if DVB_API_VERSION <= 3
		text = ostrcat(text, "\n1/4\n1/3\n2/5", 1, 0);
#endif
	}
	else if(flag == 2)
	{
		text = ostrcat(text, oitoa(FEC_AUTO), 1, 1);
		text = ostrcat(text, "\n", 1, 0);
		text = ostrcat(text, oitoa(FEC_1_2), 1, 1);
		text = ostrcat(text, "\n", 1, 0);
		text = ostrcat(text, oitoa(FEC_2_3), 1, 1);
		text = ostrcat(text, "\n", 1, 0);
		text = ostrcat(text, oitoa(FEC_3_4), 1, 1);
		text = ostrcat(text, "\n", 1, 0);
		text = ostrcat(text, oitoa(FEC_3_5), 1, 1);
		text = ostrcat(text, "\n", 1, 0);
		text = ostrcat(text, oitoa(FEC_4_5), 1, 1);
		text = ostrcat(text, "\n", 1, 0);
		text = ostrcat(text, oitoa(FEC_5_6), 1, 1);
		text = ostrcat(text, "\n", 1, 0);
		text = ostrcat(text, oitoa(FEC_6_7), 1, 1);
		text = ostrcat(text, "\n", 1, 0);
		text = ostrcat(text, oitoa(FEC_7_8), 1, 1);
		text = ostrcat(text, "\n", 1, 0);
		text = ostrcat(text, oitoa(FEC_8_9), 1, 1);
		text = ostrcat(text, "\n", 1, 0);
		text = ostrcat(text, oitoa(FEC_9_10), 1, 1);
		text = ostrcat(text, "\n", 1, 0);
#if DVB_API_VERSION <= 3
		text = ostrcat(text, "\n", 1, 0);
		text = ostrcat(text, oitoa(FEC_1_4), 1, 1);
		text = ostrcat(text, "\n", 1, 0);
		text = ostrcat(text, oitoa(FEC_1_3), 1, 1);
		text = ostrcat(text, "\n", 1, 0);
		text = ostrcat(text, oitoa(FEC_2_5), 1, 1);
#endif
	}

	return text;
}

char* transpondergetinversionstr(struct transponder* node, int flag)
{
	char* text = NULL;

	if(flag == 0)
	{
		if(node == NULL)
		{
			debug(1000, "out -> NULL detect");
			return NULL;
		}

		switch(node->inversion)
		{	
			case INVERSION_OFF: text = ostrcat(text, "Off", 1, 0); break;
			case INVERSION_ON: text = ostrcat(text, "On", 1, 0); break;
			case INVERSION_AUTO: text = ostrcat(text, "Auto", 1, 0); break;
			default: text = ostrcat(text, "unknown", 1, 0);
		}
	}
	else if(flag == 1)
		text = ostrcat(text, "Off\nON\nAuto", 1, 0);
	else if(flag == 2)
	{
		text = ostrcat(text, oitoa(INVERSION_OFF), 1, 1);
		text = ostrcat(text, "\n", 1, 0);
		text = ostrcat(text, oitoa(INVERSION_ON), 1, 1);
		text = ostrcat(text, "\n", 1, 0);
		text = ostrcat(text, oitoa(INVERSION_AUTO), 1, 1);
		text = ostrcat(text, "\n", 1, 0);
	}

	return text;
}

char* transpondergetpolarizationstr(struct transponder* node, int flag)
{
	char* text = NULL;

	if(flag == 0)
	{
		if(node == NULL)
		{
			debug(1000, "out -> NULL detect");
			return NULL;
		}

		switch(node->polarization)
		{	
			case 0: text = ostrcat(text, "Horizontal", 1, 0); break;
			case 1: text = ostrcat(text, "Vertikal", 1, 0); break;
			default: text = ostrcat(text, "unknown", 1, 0);
		}
	}
	else if(flag == 1)
		text = ostrcat(text, "Horizontal\nVertikal", 1, 0);
	else if(flag == 2)
		text = ostrcat(text, "0\n1", 1, 0);

	return text;
}

char* transpondergetmodulationstr(struct transponder* node, int flag)
{
	char* text = NULL;

	if(flag == 0)
	{
		if(node == NULL)
		{
			debug(1000, "out -> NULL detect");
			return NULL;
		}

		switch(node->modulation)
		{
			case QPSK: text = ostrcat(text, "QPSK", 1, 0); break;
			case QAM_16: text = ostrcat(text, "QAM_16", 1, 0); break;
			case QAM_32: text = ostrcat(text, "QAM_32", 1, 0); break;
			case QAM_64: text = ostrcat(text, "QAM_64", 1, 0); break;
			case QAM_128: text = ostrcat(text, "QAM_128", 1, 0); break;
			case QAM_256: text = ostrcat(text, "QAM_256", 1, 0); break;
			case VSB_8: text = ostrcat(text, "VSB_8", 1, 0); break;
			case VSB_16: text = ostrcat(text, "VSB_16", 1, 0); break;
#if DVB_API_VERSION >= 5
			case PSK_8: text = ostrcat(text, "PSK8", 1, 0); break;
#else
			case QPSK_S2: text = ostrcat(text, "QPSK_S2", 1, 0); break;
			case PSK8: text = ostrcat(text, "PSK8", 1, 0); break;
#endif
			default: text = ostrcat(text, "unknown", 1, 0);
		}
	}
	else if(flag == 1)
	{
		text = ostrcat(text, "QPSK\nQAM_16\nQAM_32\nQAM_64\nQAM_128\nQAM_256\nVSB_8\nVSB_16\nPSK8", 1, 0);
#if DVB_API_VERSION < 5
		text = ostrcat(text, "\nQPSK_S2", 1, 0);
#endif

	}
	else if(flag == 2)
	{
		text = ostrcat(text, oitoa(QPSK), 1, 1);
		text = ostrcat(text, "\n", 1, 0);
		text = ostrcat(text, oitoa(QAM_16), 1, 1);
		text = ostrcat(text, "\n", 1, 0);
		text = ostrcat(text, oitoa(QAM_32), 1, 1);
		text = ostrcat(text, "\n", 1, 0);
		text = ostrcat(text, oitoa(QAM_64), 1, 1);
		text = ostrcat(text, "\n", 1, 0);
		text = ostrcat(text, oitoa(QAM_128), 1, 1);
		text = ostrcat(text, "\n", 1, 0);
		text = ostrcat(text, oitoa(QAM_256), 1, 1);
		text = ostrcat(text, "\n", 1, 0);
		text = ostrcat(text, oitoa(VSB_8), 1, 1);
		text = ostrcat(text, "\n", 1, 0);
		text = ostrcat(text, oitoa(VSB_16), 1, 1);
		text = ostrcat(text, "\n", 1, 0);
#if DVB_API_VERSION >= 5
		text = ostrcat(text, oitoa(PSK_8), 1, 1);
#else
		text = ostrcat(text, oitoa(PSK8), 1, 1);
		text = ostrcat(text, "\n", 1, 0);
		text = ostrcat(text, oitoa(QPSK_S2), 1, 1);
#endif
	}

	return text;
}

char* transpondergetsystemstr(struct transponder* node, int flag)
{
	char* text = NULL;

	if(flag == 0)
	{
		if(node == NULL)
		{
			debug(1000, "out -> NULL detect");
			return NULL;
		}

		switch(node->system)
		{	
			case 0: text = ostrcat(text, "DVBS", 1, 0); break;
			case 1: text = ostrcat(text, "DVBS2", 1, 0); break;
			default: text = ostrcat(text, "unknown", 1, 0);
		}
	}
	else if(flag == 1)
		text = ostrcat(text, "DVBS\nDVBS2", 1, 0);
	else if(flag == 2)
		text = ostrcat(text, "0\n1", 1, 0);

	return text;
}

struct transponder* getlasttransponder(struct transponder* node)
{
	debug(1000, "in");
	struct transponder *prev = NULL;

	while(node != NULL)
	{
		prev = node;
		node = node->next;
	}

	debug(1000, "out");
	return prev;
}

struct transponder* addtransponder(char* line, int count, struct transponder* last)
{
	//debug(1000, "in");
	struct transponder *newnode = NULL, *prev = NULL, *node = transponder;
	int ret = 0;

	if(line == NULL) return NULL;

	newnode = (struct transponder*)malloc(sizeof(struct transponder));	
	if(newnode == NULL)
	{
		err("no memory");
		return NULL;
	}

	memset(newnode, 0, sizeof(struct transponder));

	ret = sscanf(line, "%lu#%"SCNu8"#%d#%"SCNu8"#%d#%d#%"SCNu8"#%"SCNu8"#%"SCNu8"#%"SCNu8"#%"SCNu8"#%"SCNu8, &newnode->id, &newnode->fetype, &newnode->frequency, &newnode->polarization, &newnode->orbitalpos, &newnode->symbolrate, &newnode->modulation, &newnode->fec, &newnode->pilot, &newnode->rolloff, &newnode->inversion, &newnode->system);
	if(ret != 12)
	{
		if(count > 0)
		{
			err("transponderlist line %d not ok", count);
		}
		else
		{
			err("add transponder");
		}
		free(newnode);
		return NULL;
	}

	if(newnode->id != 0) status.writetransponder = 1;

	if(last == NULL)
	{
		while(node != NULL && newnode->id > node->id)
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
		transponder = newnode;
	else
		prev->next = newnode;
	newnode->next = node;


	//debug(1000, "out");
	return newnode;
}

struct transponder* createtransponder(unsigned long id, int fetype, int orbitalpos, int frequency, int inversion, int symbolrate, int polarization, int fec, int modulation, int rolloff, int pilot, int system)
{
	struct transponder* tpnode = NULL;
	char* tmpstr = NULL;

	tmpstr = ostrcat(tmpstr, olutoa(id), 1, 1);
	tmpstr = ostrcat(tmpstr, "#", 1, 0);
	tmpstr = ostrcat(tmpstr, oitoa(fetype), 1, 1);
	tmpstr = ostrcat(tmpstr, "#", 1, 0);
	tmpstr = ostrcat(tmpstr, oitoa(frequency), 1, 1);
	tmpstr = ostrcat(tmpstr, "#", 1, 0);
	tmpstr = ostrcat(tmpstr, oitoa(polarization), 1, 1);
	tmpstr = ostrcat(tmpstr, "#", 1, 0);
	tmpstr = ostrcat(tmpstr, oitoa(orbitalpos), 1, 1);
	tmpstr = ostrcat(tmpstr, "#", 1, 0);
	tmpstr = ostrcat(tmpstr, oitoa(symbolrate), 1, 1);
	tmpstr = ostrcat(tmpstr, "#", 1, 0);
	tmpstr = ostrcat(tmpstr, oitoa(modulation), 1, 1);
	tmpstr = ostrcat(tmpstr, "#", 1, 0);
	tmpstr = ostrcat(tmpstr, oitoa(fec), 1, 1);
	tmpstr = ostrcat(tmpstr, "#", 1, 0);
	tmpstr = ostrcat(tmpstr, oitoa(pilot), 1, 1);
	tmpstr = ostrcat(tmpstr, "#", 1, 0);
	tmpstr = ostrcat(tmpstr, oitoa(rolloff), 1, 1);
	tmpstr = ostrcat(tmpstr, "#", 1, 0);
	tmpstr = ostrcat(tmpstr, oitoa(inversion), 1, 1);
	tmpstr = ostrcat(tmpstr, "#", 1, 0);
	tmpstr = ostrcat(tmpstr, oitoa(system), 1, 1);

	tpnode = addtransponder(tmpstr, 1, NULL);

	free(tmpstr);
	return tpnode;
}

int copytransponder(struct transponder* tp1, struct transponder* tp2, unsigned long transponderid)
{
	int ret = 1;

	if(tp1 != NULL && tp2 != NULL && transponderid == 0)
	{
		tp2->fetype = tp1->fetype;
		tp2->polarization = tp1->polarization;
		tp2->modulation = tp1->modulation;
		tp2->fec = tp1->fec;
		tp2->pilot = tp1->pilot;
		tp2->rolloff = tp1->rolloff;
		tp2->inversion = tp1->inversion;
		tp2->system = tp1->system;
		tp2->flag = tp1->flag;
		tp2->orbitalpos = tp1->orbitalpos;
		tp2->frequency = tp1->frequency;
		tp2->symbolrate = tp1->symbolrate;
		status.writetransponder = 1;
		ret = 0;
	}
	else if(tp1 != NULL && tp2 == NULL && transponderid != 0)
	{
		if(gettransponder(transponderid) == NULL && createtransponder(transponderid, tp1->fetype, tp1->orbitalpos, tp1->frequency, tp1->inversion, tp1->symbolrate, tp1->polarization, tp1->fec, tp1->modulation, tp1->rolloff, tp1->pilot, tp1->system) != NULL)
		{
			status.writetransponder = 1;
			ret = 0;
		}
	}

	return ret;
}

int readtransponder(const char* filename)
{
	debug(1000, "in");
	FILE *fd = NULL;
	char *fileline = NULL;
	int linecount = 0;
	struct transponder* last = NULL, *tmplast = NULL;

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
		if(fileline[strlen(fileline) - 1] == '\n')
			fileline[strlen(fileline) - 1] = '\0';
		if(fileline[strlen(fileline) - 1] == '\r')
			fileline[strlen(fileline) - 1] = '\0';

		linecount++;
		
		if(last == NULL) last = tmplast;
		last = addtransponder(fileline, linecount, last);
		if(last != NULL) tmplast = last;
	}

	status.writetransponder = 0;
	free(fileline);
	fclose(fd);
	return 0;
}

void deltransponder(unsigned long transponderid)
{
	debug(1000, "in");
	struct dvbdev* dvbnode = dvbdev;
	struct transponder *node = transponder, *prev = transponder;

	while(node != NULL)
	{
		if(node->id == transponderid)
		{
			if(transponderid != 0) status.writetransponder = 1;
			if(node == transponder)
				transponder = node->next;
			else
				prev->next = node->next;

			while(dvbnode != NULL)
			{
				//FRONTENDDEV first in the list
				if(dvbnode->type != FRONTENDDEV) break;
				if(dvbnode->type == FRONTENDDEV)
				{
					if(dvbnode->felasttransponder == node) dvbnode->felasttransponder = NULL;
					if(dvbnode->feakttransponder == node) dvbnode->feakttransponder = NULL;

				}
				dvbnode = dvbnode->next;
			}

			free(node);
			node = NULL;
			break;
		}

		prev = node;
		node = node->next;
	}
	debug(1000, "out");
}

struct transponder* gettransponder(unsigned long transponderid)
{
	//debug(1000, "in");
	struct transponder *node = transponder;

	while(node != NULL)
	{
		if(node->id == transponderid)
		{
			//debug(1000, "out");
			return node;
		}

		node = node->next;
	}
	debug(100, "transponder not found (%lu)", transponderid);
	return NULL;
}

void freetransponder()
{
	debug(1000, "in");
	struct transponder *node = transponder, *prev = transponder;

	while(node != NULL)
	{
		prev = node;
		node = node->next;
		if(prev != NULL)
			deltransponder(prev->id);
	}
	debug(1000, "out");
}

int writetransponder(const char *filename)
{
	debug(1000, "in");
	FILE *fd = NULL;
	struct transponder *node = transponder;
	int ret = 0;

	fd = fopen(filename, "w");
	if(fd == NULL)
	{
		perr("can't open %s", filename);
		return 1;
	}

	while(node != NULL)
	{
		if(node->id == 0)
		{
			node = node->next;
			continue;
		}
		ret = fprintf(fd, "%lu#%d#%d#%d#%d#%d#%d#%d#%d#%d#%d#%d\n", node->id, node->fetype, node->frequency, node->polarization, node->orbitalpos, node->symbolrate, node->modulation, node->fec, node->pilot, node->rolloff, node->inversion, node->system);
		if(ret < 0)
		{
			perr("writting file %s", filename);
		}
		node = node->next;
	}

	fclose(fd);
	debug(1000, "out");
	return 0;
}

#endif
