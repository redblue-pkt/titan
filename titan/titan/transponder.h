#ifndef TRANSPONDER_H
#define TRANSPONDER_H

void changetransponderid(struct transponder* tpnode, unsigned long transponderid)
{
	if(tpnode == NULL) return;
	
	deltranspondercache(tpnode->id, tpnode);
	tpnode->id = transponderid;
	modifytranspondercache(tpnode->id, tpnode);
}

void deltranspondertunablestatus()
{
	struct transponder* node = transponder;

	while(node != NULL)
	{
		node->tunablestatus = 0;
		node = node->next;
	}
}

/*
char* transpondergetbandwidthstr(struct transponder* node, int flag)
{
	char* text = NULL;

	if(flag == 0)
	{
		if(node == NULL)
		{
			debug(1000, "out -> NULL detect");
			return NULL;
		}

		switch(node->bandwidth)
		{	
			case 0: text = ostrcat(text, _("8 Mhz"), 1, 0); break;
			case 1: text = ostrcat(text, _("7 Mhz"), 1, 0); break;
			case 2: text = ostrcat(text, _("6 Mhz"), 1, 0); break;
			case 3: text = ostrcat(text, _("auto"), 1, 0); break;
			default: text = ostrcat(text, _("unknown"), 1, 0);
		}
	}
	else if(flag == 1)
	{
		text = ostrcat(text, _("8 Mhz"), 1, 0);
		text = ostrcat(text, "\n", 1, 0);
		text = ostrcat(text, _("7 Mhz"), 1, 0);
		text = ostrcat(text, "\n", 1, 0);
		text = ostrcat(text, _("6 Mhz"), 1, 0);				
		text = ostrcat(text, "\n", 1, 0);
		text = ostrcat(text, _("auto"), 1, 0);				
	}
	else if(flag == 2)
		text = ostrcat(text, "0\n1\n2\n3", 1, 0);

	return text;
}

char* transpondergettransmissionstr(struct transponder* node, int flag)
{
	char* text = NULL;

	if(flag == 0)
	{
		if(node == NULL)
		{
			debug(1000, "out -> NULL detect");
			return NULL;
		}

		switch(node->transmission)
		{	
			case 0: text = ostrcat(text, _("2 Khz"), 1, 0); break;
			case 1: text = ostrcat(text, _("8 Khz"), 1, 0); break;
			case 2: text = ostrcat(text, _("auto"), 1, 0); break;
			default: text = ostrcat(text, _("unknown"), 1, 0);
		}
	}
	else if(flag == 1)
	{
		text = ostrcat(text, _("2 Khz"), 1, 0);
		text = ostrcat(text, "\n", 1, 0);
		text = ostrcat(text, _("8 Khz"), 1, 0);
		text = ostrcat(text, "\n", 1, 0);
		text = ostrcat(text, _("auto"), 1, 0);						
	}
	else if(flag == 2)
		text = ostrcat(text, "0\n1\n2", 1, 0);

	return text;
}

char* transpondergetguardintervalstr(struct transponder* node, int flag)
{
	char* text = NULL;

	if(flag == 0)
	{
		if(node == NULL)
		{
			debug(1000, "out -> NULL detect");
			return NULL;
		}

		switch(node->guardinterval)
		{	
			case 0: text = ostrcat(text, "1/32", 1, 0); break;
			case 1: text = ostrcat(text, "1/16", 1, 0); break;
			case 2: text = ostrcat(text, "1/8", 1, 0); break;
			case 3: text = ostrcat(text, "1/4", 1, 0); break;
			case 4: text = ostrcat(text, _("auto"), 1, 0); break;
			default: text = ostrcat(text, _("unknown"), 1, 0);
		}
	}
	else if(flag == 1)
	{
		text = ostrcat(text, "1/32\n1/16\n1/8\n1/4\n", 1, 0);
		text = ostrcat(text, _("auto"), 1, 0);
	}
	else if(flag == 2)
		text = ostrcat(text, "0\n1\n2\n3\n4", 1, 0);

	return text;
}

char* transpondergethieratchystr(struct transponder* node, int flag)
{
	char* text = NULL;

	if(flag == 0)
	{
		if(node == NULL)
		{
			debug(1000, "out -> NULL detect");
			return NULL;
		}

		switch(node->hierarchy)
		{	
			case 0: text = ostrcat(text, _("none"), 1, 0); break;
			case 1: text = ostrcat(text, "1", 1, 0); break;
			case 2: text = ostrcat(text, "2", 1, 0); break;
			case 3: text = ostrcat(text, "4", 1, 0); break;
			case 4: text = ostrcat(text, _("auto"), 1, 0); break;
			default: text = ostrcat(text, _("unknown"), 1, 0);
		}
	}
	else if(flag == 1)
	{
		text = ostrcat(text, _("none"), 1, 0);
		text = ostrcat(text, "\n1\n2\n4\n", 1, 0);
		text = ostrcat(text, _("auto"), 1, 0);
	}		
	else if(flag == 2)
		text = ostrcat(text, "0\n1\n2\n3\n4", 1, 0);

	return text;
}
*/

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
			case 0: text = ostrcat(text, _("off"), 1, 0); break;
			case 1: text = ostrcat(text, _("on"), 1, 0); break;
			case 2: text = ostrcat(text, _("auto"), 1, 0); break;
			default: text = ostrcat(text, _("unknown"), 1, 0);
		}
	}
	else if (flag == 1)
	{
		text = ostrcat(text, _("off"), 1, 0);
		text = ostrcat(text, "\n", 1, 0);
		text = ostrcat(text, _("on"), 1, 0);
		text = ostrcat(text, "\n", 1, 0);
		text = ostrcat(text, _("auto"), 1, 0);
		text = ostrcat(text, "\n", 1, 0);
	}
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
			case 1: text = ostrcat(text, "0.25", 1, 0); break;
			case 2: text = ostrcat(text, "0.20", 1, 0); break;
			case 3: text = ostrcat(text, _("auto"), 1, 0); break;
			default: text = ostrcat(text, _("unknown"), 1, 0);
		}
	}
	else if(flag == 1)
	{
		text = ostrcat(text, "0.35\n0.25\n0.20\n", 1, 0);
		text = ostrcat(text, _("auto"), 1, 0);
	}
	else if(flag == 2)
		text = ostrcat(text, "0\n1\n2\n3", 1, 0);

	return text;
}

char* transpondergetfecstr(struct transponder* node, int flag)
{
	char* text = NULL;
	
	if(node == NULL)
	{
		debug(1000, "out -> NULL detect");
		return NULL;
	}

	if(flag == 0)
	{
		if(node->fetype == FE_QPSK)
		{
			switch(node->fec)
			{	
				case 0: text = ostrcat(text, _("auto"), 1, 0); break;
				case 1: text = ostrcat(text, "1/2", 1, 0); break;
				case 2: text = ostrcat(text, "2/3", 1, 0); break;
				case 3: text = ostrcat(text, "3/4", 1, 0); break;
				case 4: text = ostrcat(text, "5/6", 1, 0); break;
				case 5: text = ostrcat(text, "7/8", 1, 0); break;
				case 6: text = ostrcat(text, "8/9", 1, 0); break;
				case 7: text = ostrcat(text, "3/5", 1, 0); break;
				case 8: text = ostrcat(text, "4/5", 1, 0); break;
				case 9: text = ostrcat(text, "9/10", 1, 0); break;
				case 15: text = ostrcat(text, _("none"), 1, 0); break;
				default: text = ostrcat(text, _("unknown"), 1, 0);
			}
		}
		if(node->fetype == FE_QAM)
		{
			switch(node->fec)
			{	
				case 0: text = ostrcat(text, _("auto"), 1, 0); break;
				case 1: text = ostrcat(text, "1/2", 1, 0); break;
				case 2: text = ostrcat(text, "2/3", 1, 0); break;
				case 3: text = ostrcat(text, "3/4", 1, 0); break;
				case 4: text = ostrcat(text, "5/6", 1, 0); break;
				case 5: text = ostrcat(text, "7/8", 1, 0); break;
				case 6: text = ostrcat(text, "8/9", 1, 0); break;
				case 15: text = ostrcat(text, _("none"), 1, 0); break;
				default: text = ostrcat(text, _("unknown"), 1, 0);
			}
		}
		if(node->fetype == FE_OFDM)
		{
			switch(node->fec)
			{	
				case 0: text = ostrcat(text, "1/2", 1, 0); break;
				case 1: text = ostrcat(text, "2/3", 1, 0); break;
				case 2: text = ostrcat(text, "3/4", 1, 0); break;
				case 3: text = ostrcat(text, "5/6", 1, 0); break;
				case 4: text = ostrcat(text, "7/8", 1, 0); break;
				case 5: text = ostrcat(text, _("auto"), 1, 0); break;
				default: text = ostrcat(text, _("unknown"), 1, 0);
			}
		}
	}
	else if(flag == 1)
	{
		if(node->fetype == FE_QPSK)
		{
			text = ostrcat(text, _("auto"), 1, 0);
			text = ostrcat(text, "\n1/2\n2/3\n3/4\n5/6\n7/8\n8/9\n3/5\n4/5\n9/10\n", 1, 0);
			text = ostrcat(text, _("none"), 1, 0);
		}
		else if(node->fetype == FE_QAM)
		{
			text = ostrcat(text, _("auto"), 1, 0);
			text = ostrcat(text, "\n1/2\n2/3\n3/4\n5/6\n7/8\n8/9\n", 1, 0);
			text = ostrcat(text, _("none"), 1, 0);
		}
		else if(node->fetype == FE_OFDM)
		{
			text = ostrcat(text, "1/2\n2/3\n3/4\n5/6\n7/8\n", 1, 0);
			text = ostrcat(text, _("auto"), 1, 0);
		}
	}
	else if(flag == 2)
	{
		if(node->fetype == FE_QPSK)
			text = ostrcat(text, "0\n1\n2\n3\n4\n5\n6\n7\n8\n9\n10", 1, 0);
		else if(node->fetype == FE_QAM)
			text = ostrcat(text, "0\n1\n2\n3\n4\n5\n6\n7", 1, 0);
		else if(node->fetype == FE_OFDM)
			text = ostrcat(text, "0\n1\n2\n3\n4\n5", 1, 0);
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
			case 0: text = ostrcat(text, _("off"), 1, 0); break;
			case 1: text = ostrcat(text, _("on"), 1, 0); break;
			case 2: text = ostrcat(text, _("auto"), 1, 0); break;
			default: text = ostrcat(text, _("unknown"), 1, 0);
		}
	}
	else if(flag == 1)
	{
		text = ostrcat(text, _("off"), 1, 0);
		text = ostrcat(text, "\n", 1, 0);
		text = ostrcat(text, _("on"), 1, 0);
		text = ostrcat(text, "\n", 1, 0);
		text = ostrcat(text, _("auto"), 1, 0);
	}
	else if(flag == 2)
		text = ostrcat(text, "0\n1\n2", 1, 0);

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
			case 0: text = ostrcat(text, _("Horizontal"), 1, 0); break;
			case 1: text = ostrcat(text, _("Vertikal"), 1, 0); break;
			default: text = ostrcat(text, _("unknown"), 1, 0);
		}
	}
	else if(flag == 1)
	{
		text = ostrcat(text, _("Horizontal"), 1, 0);
		text = ostrcat(text, "\n", 1, 0);
		text = ostrcat(text, _("Vertikal"), 1, 0);
	}
	else if(flag == 2)
		text = ostrcat(text, "0\n1", 1, 0);

	return text;
}

char* transpondergetmodulationstr(struct transponder* node, int flag)
{
	char* text = NULL;
	
	if(node == NULL)
	{
		debug(1000, "out -> NULL detect");
		return NULL;
	}

	if(flag == 0)
	{
		if(node->fetype == FE_QPSK)
		{
			switch(node->modulation)
			{
				case 0: text = ostrcat(text, _("auto"), 1, 0); break;
				case 1: text = ostrcat(text, _("QPSK"), 1, 0); break;
				case 2: text = ostrcat(text, _("PSK 8"), 1, 0); break;
				case 3: text = ostrcat(text, _("QAM 16"), 1, 0); break;
				default: text = ostrcat(text, _("unknown"), 1, 0);
			}
		}
		if(node->fetype == FE_QAM)
		{
			switch(node->modulation)
			{
				case 0: text = ostrcat(text, _("QAM auto"), 1, 0); break;
				case 1: text = ostrcat(text, _("QAM 16"), 1, 0); break;
				case 2: text = ostrcat(text, _("QAM 32"), 1, 0); break;
				case 3: text = ostrcat(text, _("QAM 64"), 1, 0); break;
				case 4: text = ostrcat(text, _("QAM 128"), 1, 0); break;
				case 5: text = ostrcat(text, _("QAM 256"), 1, 0); break;
				default: text = ostrcat(text, _("unknown"), 1, 0);
			}
		}
		if(node->fetype == FE_OFDM)
		{
			switch(node->modulation)
			{
				case 0: text = ostrcat(text, _("QPSK"), 1, 0); break;
				case 1: text = ostrcat(text, _("QAM 16"), 1, 0); break;
				case 2: text = ostrcat(text, _("QAM 64"), 1, 0); break;
				case 3: text = ostrcat(text, _("QAM auto"), 1, 0); break;
				default: text = ostrcat(text, _("unknown"), 1, 0);
			}
		}
	}
	else if(flag == 1)
	{
		if(node->fetype == FE_QPSK)
		{
			text = ostrcat(text, _("auto"), 1, 0);
			text = ostrcat(text, "\n", 1, 0);
			text = ostrcat(text, _("QPSK"), 1, 0);
			text = ostrcat(text, "\n", 1, 0);
			text = ostrcat(text, _("PSK 8"), 1, 0);
			text = ostrcat(text, "\n", 1, 0);
			text = ostrcat(text, _("QAM 16"), 1, 0);
		}
		else if(node->fetype == FE_QAM)
		{
			text = ostrcat(text, _("QAM auto"), 1, 0);
			text = ostrcat(text, "\n", 1, 0);
			text = ostrcat(text, _("QAM 16"), 1, 0);
			text = ostrcat(text, "\n", 1, 0);
			text = ostrcat(text, _("QAM 32"), 1, 0);
			text = ostrcat(text, "\n", 1, 0);
			text = ostrcat(text, _("QAM 64"), 1, 0);
			text = ostrcat(text, "\n", 1, 0);
			text = ostrcat(text, _("QAM 128"), 1, 0);
			text = ostrcat(text, "\n", 1, 0);
			text = ostrcat(text, _("QAM 256"), 1, 0);
		}
		else if(node->fetype == FE_OFDM)
		{
			text = ostrcat(text, _("QPSK"), 1, 0);
			text = ostrcat(text, "\n", 1, 0);
			text = ostrcat(text, _("QAM 16"), 1, 0);
			text = ostrcat(text, "\n", 1, 0);
			text = ostrcat(text, _("QAM 64"), 1, 0);
			text = ostrcat(text, "\n", 1, 0);
			text = ostrcat(text, _("QAM auto"), 1, 0);
		}
	}
	else if(flag == 2)
	{
		if(node->fetype == FE_QPSK)
			text = ostrcat(text, "0\n1\n2\n3", 1, 0);
		if(node->fetype == FE_QAM)
			text = ostrcat(text, "0\n1\n2\n3\n4\n5", 1, 0);
		if(node->fetype == FE_OFDM)
			text = ostrcat(text, "0\n1\n2\n3", 1, 0);
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
			case 0: text = ostrcat(text, _("DVBS"), 1, 0); break;
			case 1: text = ostrcat(text, _("DVBS2"), 1, 0); break;
			default: text = ostrcat(text, _("unknown"), 1, 0);
		}
	}
	else if(flag == 1)
	{
		text = ostrcat(text, _("DVBS"), 1, 0);
		text = ostrcat(text, "\n", 1, 0);
		text = ostrcat(text, _("DVBS2"), 1, 0);
	}
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

	if(newnode->id != 99) status.writetransponder = 1;
	
	modifytranspondercache(newnode->id, newnode);

	if(last == NULL)
	{
		//while(node != NULL && newnode->id > node->id)
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
		transponder = newnode;
	else
		prev->next = newnode;
	newnode->next = node;


	//debug(1000, "out");
	return newnode;
}

struct transponder* createtransponder(unsigned long id, int fetype, int orbitalpos, unsigned int frequency, int inversion, unsigned int symbolrate, int polarization, int fec, int modulation, int rolloff, int pilot, int system)
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

	if(tp1 != NULL && tp2 != NULL && transponderid == 99)
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
	else if(tp1 != NULL && tp2 == NULL && transponderid != 99)
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
	int linecount = 0, len = 0;
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
		len = strlen(fileline) - 1;
		if(len >= 0 && fileline[len] == '\n')
			fileline[len] = '\0';
		len--;
		if(len >= 0 && fileline[len] == '\r')
			fileline[len] = '\0';

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

int readtransponderencoding(const char* filename)
{
	debug(1000, "in");
	FILE *fd = NULL;
	char *fileline = NULL;
	int linecount = 0, len = 0;
	struct transponder* tpnode = NULL;
	
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
		tpnode = NULL;

		unsigned long transponderid = 0;
		int encoding = 0;
		if(sscanf(fileline, "%lu#%d", &transponderid, &encoding) == 2)
		{
			tpnode = gettransponder(transponderid);
			if(tpnode != NULL) tpnode->encoding = encoding;
		}
		else
		{
			err("encoding line %d not ok", linecount);
		}
	}

	free(fileline);
	fclose(fd);
	return 0;
}

void deltransponder(struct transponder* tpnode)
{
	debug(1000, "in");
	struct dvbdev* dvbnode = dvbdev;
	struct transponder *node = transponder, *prev = transponder;

	while(node != NULL)
	{
		if(node == tpnode)
		{
			if(node->id != 99) status.writetransponder = 1;
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

			if(node->id != 99) delchannelbytransponder(node->id);
			deltranspondercache(node->id, NULL);

			free(node);
			node = NULL;
			break;
		}

		prev = node;
		node = node->next;
	}
	debug(1000, "out");
}

void deltransponderbyid(unsigned long transponderid)
{
	struct transponder *node = transponder;
	struct transponder *prev = NULL;

	while(node != NULL)
	{
		prev = node;
		node = node->next;
		if(prev->id == transponderid)
		{
			deltransponder(prev);
			break;
		}
	}
}

/*
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
*/

void deltransponderbyorbitalpos(int orbitalpos)
{
	//debug(1000, "in");
	struct transponder *node = transponder, *prev = transponder;

	while(node != NULL)
	{
		prev = node;
		node = node->next;
		if(prev != NULL && prev->orbitalpos == orbitalpos)
			deltransponder(prev);
	}
	//debug(1000, "out");
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
			deltransponder(prev);
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
		if(node->id == 99)
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
