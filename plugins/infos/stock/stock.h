#ifndef STOCK_H
#define STOCK_H

extern struct skin* skin;

struct stock
{
	char* symbol_lookup_url;
	char* company;
	char* exchange;
	char* exchange_closing;
	char* currency;
	char* last;
	char* high;
	char* low;
	char* volume;
	char* avg_volume;
	char* market_cap;
	char* open;
	char* y_close;
	char* change;
	char* perc_change;
	char* trade_timestamp;
};

int writestock(const char *filename, struct skin* listbox);

char* readstock(const char* filename, struct skin* stock, struct skin* listbox)
{
	debug(1000, "in");
	FILE *fd = NULL;
	char *fileline = NULL;
	char *name = NULL;
	struct skin* tmp = NULL;
	int len = 0;

	if(stock == NULL || listbox == NULL) return NULL;

	fileline = malloc(MINMALLOC);
	if(fileline == NULL)
	{
		err("no memory");
		return NULL;
	}

	fd = fopen(filename, "r");
	if(fd == NULL)
	{
		perr("can't open %s", filename);
		free(fileline);
		return NULL;
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

		tmp = addlistbox(stock, listbox, tmp, 1);
		if(tmp != NULL)
		{
			changetext(tmp, fileline);
			changename(tmp, fileline);
			if(name == NULL)
			{
				name = ostrcat(name, fileline, 1, 0);
				name = ostrshrink(name);
			}
		}

	}

	free(fileline);
	fclose(fd);
	return name;
}

void freestock(struct stock* node)
{
	if(node == NULL) return;

	free(node->symbol_lookup_url); node->symbol_lookup_url = NULL;
	free(node->company); node->company = NULL;
	free(node->exchange); node->exchange = NULL;
	free(node->exchange_closing); node->exchange_closing = NULL;
	free(node->currency); node->currency = NULL;
	free(node->last); node->last = NULL;
	free(node->high); node->high = NULL;
	free(node->low); node->low = NULL;
	free(node->volume); node->volume = NULL;
	free(node->avg_volume); node->avg_volume = NULL;
	free(node->market_cap); node->market_cap = NULL;
	free(node->open); node->open = NULL;
	free(node->y_close); node->y_close = NULL;
	free(node->change); node->change = NULL;
	free(node->perc_change); node->perc_change = NULL;
	free(node->trade_timestamp); node->trade_timestamp = NULL;

	free(node); node = NULL;
}

struct stock* getstock(char* name)
{
	struct stock* stock = NULL;
	char* tmpstr = NULL;
	char* tmpsearch = NULL;

	if(name == NULL) return NULL;

	tmpsearch = ostrcat("ig/api?stock=", name, 0, 0);
	//TODO: implement auto language (from titan.cfg)
	tmpsearch = ostrcat(tmpsearch, "&hl=de", 1, 0);
	tmpsearch = stringreplacechar(tmpsearch, ' ', '+');

	tmpstr = gethttp("www.google.com", tmpsearch, 80, NULL, NULL, 5000, NULL, 0);

	free(tmpsearch); tmpsearch = NULL;

	if(tmpstr != NULL)
	{
		stock = (struct stock*)malloc(sizeof(struct stock));
		if(stock == NULL)
		{
			err("no mem");
			free(tmpstr); tmpstr = NULL;
			return NULL;
		}
		memset(stock, 0, sizeof(struct stock));

		stock->symbol_lookup_url = getxmlentry(tmpstr, "symbol_lookup_url data=");
		stock->company = getxmlentry(tmpstr, "company data=");
		stock->exchange = getxmlentry(tmpstr, "exchange data=");
		stock->exchange_closing = getxmlentry(tmpstr, "exchange_closing data=");
		stock->currency = getxmlentry(tmpstr, "currency data=");
		stock->last = getxmlentry(tmpstr, "last data=");
		stock->high = getxmlentry(tmpstr, "high data=");
		stock->low = getxmlentry(tmpstr, "low data=");
		stock->volume = getxmlentry(tmpstr, "volume data=");
		stock->avg_volume = getxmlentry(tmpstr, "avg_volume data=");
		stock->market_cap = getxmlentry(tmpstr, "market_cap data=");
		stock->open = getxmlentry(tmpstr, "open data=");
		stock->y_close = getxmlentry(tmpstr, "y_close data=");
		stock->change = getxmlentry(tmpstr, "<change data=");
		stock->perc_change = getxmlentry(tmpstr, "perc_change data=");
		stock->trade_timestamp = getxmlentry(tmpstr, "trade_timestamp data=");
		
		free(tmpstr); tmpstr = NULL;
	}

	return stock;
}

void screenstock()
{
	int rcret = 0, ret = 0;
	struct skin* stock = getscreen("stock");
	struct skin* listbox = getscreennode(stock, "listbox"); 
	struct skin* company = getscreennode(stock, "company");
	struct skin* exchange = getscreennode(stock, "exchange");
	struct skin* exchange_closing = getscreennode(stock, "exchange_closing");
	struct skin* currency = getscreennode(stock, "currency");
	struct skin* last = getscreennode(stock, "last");
	struct skin* high = getscreennode(stock, "high");
	struct skin* low = getscreennode(stock, "low");
	struct skin* volume = getscreennode(stock, "volume");
	struct skin* avg_volume = getscreennode(stock, "avg_volume");
	struct skin* market_cap = getscreennode(stock, "market_cap");
	struct skin* open = getscreennode(stock, "open");
	struct skin* y_close = getscreennode(stock, "y_close");
	struct skin* change = getscreennode(stock, "change");
	struct skin* perc_change = getscreennode(stock, "perc_change");
	struct skin* trade_timestamp = getscreennode(stock, "trade_timestamp");
	struct stock* node = NULL;
	char* tmpstr = NULL, *name = NULL;

	name = readstock(getconfig("stockfile", NULL), stock, listbox);
	addscreenrc(stock, listbox);

start:

	tmpstr = ostrcat(_("Stock"), " - ", 0, 0);
	tmpstr = ostrcat(tmpstr, name, 1, 0);
	changetitle(stock, tmpstr);
	free(tmpstr); tmpstr = NULL;

	node = getstock(name);
	free(name); name = NULL;

	if(node != NULL)
	{
		tmpstr = ostrcat(_("Company: "), node->company, 0, 0);
		changetext(company, tmpstr);
		free(tmpstr); tmpstr = NULL;

		tmpstr = ostrcat(_("Exchange: "), node->exchange, 0, 0);
		changetext(exchange, tmpstr);
		free(tmpstr); tmpstr = NULL;

		tmpstr = ostrcat(_("Exchange closing: "), node->exchange_closing, 0, 0);
		changetext(exchange_closing, tmpstr);
		free(tmpstr); tmpstr = NULL;

		tmpstr = ostrcat(_("Currency: "), node->currency, 0, 0);
		changetext(currency, tmpstr);
		free(tmpstr); tmpstr = NULL;

		tmpstr = ostrcat(_("Last: "), node->last, 0, 0);
		changetext(last, tmpstr);
		free(tmpstr); tmpstr = NULL;

		tmpstr = ostrcat(_("High: "), node->high, 0, 0);
		changetext(high, tmpstr);
		free(tmpstr); tmpstr = NULL;

		tmpstr = ostrcat(_("Low: "), node->low, 0, 0);
		changetext(low, tmpstr);
		free(tmpstr); tmpstr = NULL;

		tmpstr = ostrcat(_("Volume: "), node->volume, 0, 0);
		changetext(volume, tmpstr);
		free(tmpstr); tmpstr = NULL;

		tmpstr = ostrcat(_("Avg volume: "), node->avg_volume, 0, 0);
		changetext(avg_volume, tmpstr);
		free(tmpstr); tmpstr = NULL;

		tmpstr = ostrcat(_("Market cap: "), node->market_cap, 0, 0);
		changetext(market_cap, tmpstr);
		free(tmpstr); tmpstr = NULL;

		tmpstr = ostrcat(_("Open: "), node->open, 0, 0);
		changetext(open, tmpstr);
		free(tmpstr); tmpstr = NULL;

		tmpstr = ostrcat(_("Close: "), node->y_close, 0, 0);
		changetext(y_close, tmpstr);
		free(tmpstr); tmpstr = NULL;

		tmpstr = ostrcat(_("Change: "), node->change, 0, 0);
		changetext(change, tmpstr);
		free(tmpstr); tmpstr = NULL;

		tmpstr = ostrcat(_("Change in %: "), node->perc_change, 0, 0);
		changetext(perc_change, tmpstr);
		free(tmpstr); tmpstr = NULL;

		tmpstr = ostrcat(_("Trade time: "), node->trade_timestamp, 0, 0);
		changetext(trade_timestamp, tmpstr);
		free(tmpstr); tmpstr = NULL;
	}

	drawscreen(stock, 0, 0);

	while(1)
	{
		rcret = waitrc(stock, 0, 0);
	
		if(rcret == getrcconfigint("rcexit", NULL)) break;
		if(rcret == getrcconfigint("rcok", NULL)) break;

		if(rcret == getrcconfigint("rcred", NULL))
    {
			free(name); name = NULL;
			name = textinput(_("Name"), NULL);
      if(name != NULL)
      {
				struct skin* tmp = addlistbox(stock, listbox, NULL, 1);
				if(tmp != NULL)
				{
					changetext(tmp, name);
					changename(tmp, name);
				}
      }
      drawscreen(stock, 0, 0);
			if(name == NULL)
				continue;
			free(name); name = NULL;
    }

		if(rcret == getrcconfigint("rcyellow", NULL))
		{
			writestock(getconfig("stockfile", NULL), listbox);
			continue;
		}

		if(listbox->select != NULL && rcret == getrcconfigint("rcgreen", NULL))
		{
			delscreennode(stock, listbox->select->name);
			listbox->select = NULL;
			drawscreen(stock, 0, 0);
		}
    
    if(rcret == getrcconfigint("rcblue", NULL) && node->symbol_lookup_url != NULL)
    {
      ret = servicestop(status.aktservice, 1, 0);
      if(ret == 1) goto start;
      status.sec = 0; //deaktivate spinner
      setfbtransparent(255);
      
      drawscreen(skin, 0, 0);
      tmpstr = ostrcat("nsfb.sh --scale=100 -f linux -b 32 http://www.google.com", node->symbol_lookup_url, 0, 0);
      system(tmpstr);
      free(tmpstr); tmpstr = NULL;
      
      setosdtransparent(getskinconfigint("osdtransparent", NULL));
      if(status.lastservice != NULL)
        servicestart(status.lastservice->channel, NULL, NULL, 0);
      flushrc(500);
      
      drawscreen(stock, 0, 0);
      continue;
    }

		if(listbox->select != NULL)
		{
			freestock(node); node = NULL;
			name = ostrcat(name, listbox->select->text, 1, 0);
			goto start;
		}
	}

	delownerrc(stock);
	delmarkedscreennodes(stock, 1);
	freestock(node); node = NULL;
	clearscreen(stock);
}

int writestock(const char *filename, struct skin* listbox)
{
	debug(1000, "in");
	FILE *fd = NULL;
	struct skin* node = listbox;
	int ret = 0;

	fd = fopen(filename, "w");
	if(fd == NULL)
	{
		perr("can't open %s", filename);
		return 1;
	}

	while(node != NULL)
	{
		if(node->del == 1)
		{
			ret = fprintf(fd, "%s\n", node->text);
			if(ret < 0)
			{
				perr("writting file %s", filename);
			}
		}
		node = node->next;
	}

	fclose(fd);
	debug(1000, "out");
	return 0;
}

#endif
