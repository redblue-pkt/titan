#ifndef OPTIMIZE_H
#define OPTIMIZE_H

extern struct transponder* transponder;
extern struct channel* channel;
extern struct mainbouquet* mainbouquet;
extern struct epgscanlist* epgscanlist;
extern struct sat* sat;
extern struct provider* provider;

void delmarkedsat(struct skin* listbox)
{
	struct skin* node = listbox;

	while(node != NULL)
	{
		if(node->del == 1 && ostrcmp(node->ret, "1") == 0)
		{
			delsat(node->handle);
			node->handle = NULL;
		}
		node = node->next;
	}
}

void delunusedtransponder()
{
	struct transponder* node = transponder;
	struct transponder* prev = NULL;

	while(node != NULL)
	{
		prev = node;
		node = node->next;

		if(getsatbyorbitalpos(prev->orbitalpos) == NULL)
			deltransponder(prev);
	}
}

void delunusedchannel()
{
	struct channel* node = channel;
	struct channel* prev = NULL;

	while(node != NULL)
	{
		prev = node;
		node = node->next;

		if(prev->transponder == NULL)
			delchannel(prev->serviceid, prev->transponderid, 0);
	}
}

void delunusedbouquet()
{
	struct mainbouquet* mnode = mainbouquet;
	struct bouquet* first = NULL;
	struct bouquet* node = NULL;
	struct bouquet* prev = NULL;

	while(mnode != NULL)
	{
		first = mnode->bouquet;
		node = mnode->bouquet;
		while(node != NULL)
		{
			prev = node;	
			node = node->next;
			if(getchannel(prev->serviceid, prev->transponderid) == NULL)
				delbouquet(prev->serviceid, prev->transponderid, &first);
		}
		mnode = mnode->next;
	}
}

void delunusedepgchannel()
{
	struct epgscanlist* node = epgscanlist;
	struct epgscanlist* prev = NULL;

	while(node != NULL)
	{
		prev = node;	
		node = node->next;
		if(getchannel(prev->serviceid, prev->transponderid) == NULL)
			delepgscanlist(node->serviceid, node->transponderid);
	}
}

void delunusedprovider()
{
	struct provider* node = provider;
	struct provider* prev = NULL;

	while(node != NULL)
	{
		prev = node;	
		node = node->next;
		delprovidernotused(prev);
	}
}

int getsatcount()
{
	int count = 0;
	struct sat* node = sat;

	while(node != NULL)
	{
		count++;
		node = node->next;
	}

	return count;
}

int gettranspondercount(int orbitalpos)
{
	int count = 0;
	struct transponder* node = transponder;

	while(node != NULL)
	{
		if(orbitalpos == 0 || node->orbitalpos == orbitalpos)
			count++;
		node = node->next;
	}

	return count;
}

int getchannelcount(uint64_t transponderid)
{
	int count = 0;
	struct channel* node = channel;

	while(node != NULL)
	{
		if(transponderid == 0 || node->transponderid == transponderid)
			count++;
		node = node->next;
	}

	return count;
}

int getmainbouquetcount()
{
	int count = 0;
	struct mainbouquet* node = mainbouquet;

	while(node != NULL)
	{
		count++;
		node = node->next;
	}

	return count;
}

int getbouquetcount()
{
	int count = 0;
	struct mainbouquet* mnode = mainbouquet;
	struct bouquet* node = NULL;

	while(mnode != NULL)
	{
		node = mnode->bouquet;
		while(node != NULL)
		{
			count++;
			node = node->next;
		}
		mnode = mnode->next;
	}

	return count;
}

int getepgchannelcount()
{
	int count = 0;
	struct epgscanlist* node = epgscanlist;

	while(node != NULL)
	{
		count++;
		node = node->next;
	}

	return count;
}

int getprovidercount()
{
	int count = 0;
	struct provider* node = provider;

	while(node != NULL)
	{
		count++;
		node = node->next;
	}

	return count;
}

void screenoptimize()
{
	int rcret = 0, count = 0;
	struct skin* optimize = getscreen("optimize");
	struct skin* listbox = getscreennode(optimize, "listbox");
	struct skin* c1 = getscreennode(optimize, "c1");
	struct skin* c2 = getscreennode(optimize, "c2");
	struct skin* c3 = getscreennode(optimize, "c3");
	struct skin* c4 = getscreennode(optimize, "c4");
	struct skin* c5 = getscreennode(optimize, "c5");
	struct skin* c6 = getscreennode(optimize, "c6");
	struct skin* c7 = getscreennode(optimize, "c7");
	//struct skin* c8 = getscreennode(optimize, "c8");
	//struct skin* c9 = getscreennode(optimize, "c9");
	struct skin* tmp = NULL;
	struct sat* satnode = NULL;
	char* tmpstr = NULL, *tmpnr = NULL;

start:
	satnode = sat;
	tmp = NULL;
	delmarkedscreennodes(optimize, 1);
	optimize->aktline = 1;
	optimize->aktpage = -1;
	
	while(satnode != NULL)
	{
		tmp = addlistbox(optimize, listbox, tmp, 1);
		if(tmp != NULL)
		{
			count = gettranspondercount(satnode->orbitalpos);
			tmpnr = oitoa(count);

			tmpstr = ostrcat(tmpstr, satnode->name, 1, 0);
			tmpstr = ostrcat(tmpstr, " - TP: ", 1, 0);
			tmpstr = ostrcat(tmpstr, tmpnr, 1, 0);

			changetext(tmp, tmpstr);
			free(tmpnr); tmpnr = NULL;
			free(tmpstr); tmpstr = NULL;

			tmp->type = CHOICEBOX;
			addchoicebox(tmp, "0", _("hold"));
			addchoicebox(tmp, "1", _("delete"));

			tmp->handle = satnode->name;
		}

		satnode = satnode->next;
	}

	count = getsatcount();
	tmpnr = oitoa(count);
	tmpstr = ostrcat(tmpstr, _("SatCount: "), 1, 0);
	tmpstr = ostrcat(tmpstr, tmpnr, 1, 0);
	changetext(c1, tmpstr);
	free(tmpnr); tmpnr = NULL;
	free(tmpstr); tmpstr = NULL;
	
	count = gettranspondercount(0);
	tmpnr = oitoa(count);
	tmpstr = ostrcat(tmpstr, _("Transponder: "), 1, 0);
	tmpstr = ostrcat(tmpstr, tmpnr, 1, 0);
	changetext(c2, tmpstr);
	free(tmpnr); tmpnr = NULL;
	free(tmpstr); tmpstr = NULL;
	
	count = getchannelcount(0);
	tmpnr = oitoa(count);
	tmpstr = ostrcat(tmpstr, _("ChannelCount: "), 1, 0);
	tmpstr = ostrcat(tmpstr, tmpnr, 1, 0);
	changetext(c3, tmpstr);
	free(tmpnr); tmpnr = NULL;
	free(tmpstr); tmpstr = NULL;
	
	count = getmainbouquetcount();
	tmpnr = oitoa(count);
	tmpstr = ostrcat(tmpstr, _("BouquetCount: "), 1, 0);
	tmpstr = ostrcat(tmpstr, tmpnr, 1, 0);
	changetext(c4, tmpstr);
	free(tmpnr); tmpnr = NULL;
	free(tmpstr); tmpstr = NULL;
	
	count = getbouquetcount();
	tmpnr = oitoa(count);
	tmpstr = ostrcat(tmpstr, _("BouquetEntrys: "), 1, 0);
	tmpstr = ostrcat(tmpstr, tmpnr, 1, 0);
	changetext(c5, tmpstr);
	free(tmpnr); tmpnr = NULL;
	free(tmpstr); tmpstr = NULL;
	
	count = getepgchannelcount();
	tmpnr = oitoa(count);
	tmpstr = ostrcat(tmpstr, _("EpgChannels: "), 1, 0);
	tmpstr = ostrcat(tmpstr, tmpnr, 1, 0);
	changetext(c6, tmpstr);
	free(tmpnr); tmpnr = NULL;
	free(tmpstr); tmpstr = NULL;
	
	count = getprovidercount();
	tmpnr = oitoa(count);
	tmpstr = ostrcat(tmpstr, _("ProviderCount: "), 1, 0);
	tmpstr = ostrcat(tmpstr, tmpnr, 1, 0);
	changetext(c7, tmpstr);
	free(tmpnr); tmpnr = NULL;
	free(tmpstr); tmpstr = NULL;
	
/*
	count = 0;
	tmpnr = oitoa(count);
	tmpstr = ostrcat(tmpstr, _("unused: "), 1, 0);
	tmpstr = ostrcat(tmpstr, tmpnr, 1, 0);
	changetext(c8, tmpstr);
	free(tmpnr); tmpnr = NULL;
	free(tmpstr); tmpstr = NULL;
	
	count = 0;
	tmpnr = oitoa(count);
	tmpstr = ostrcat(tmpstr, _("unused: "), 1, 0);
	tmpstr = ostrcat(tmpstr, tmpnr, 1, 0);
	changetext(c9, tmpstr);
	free(tmpnr); tmpnr = NULL;
	free(tmpstr); tmpstr = NULL;
*/
	
	drawscreen(optimize, 0, 0);
	addscreenrc(optimize, listbox);

	tmp = listbox->select;
	while(1)
	{
		addscreenrc(optimize, tmp);
		rcret = waitrc(optimize, 0, 0);
		tmp = listbox->select;

		if(rcret == getrcconfigint("rcexit", NULL)) break;
		if(rcret == getrcconfigint("rcok", NULL))
		{
			delmarkedsat(listbox);
			delunusedtransponder();
			delunusedchannel();
			delunusedbouquet();
			delunusedepgchannel();
			delunusedprovider();
			
			goto start;
		}
	}

	delmarkedscreennodes(optimize, 1);
	delownerrc(optimize);
	clearscreen(optimize);
}

#endif
