#ifndef HTTPDFUNC_H
#define HTTPDFUNC_H

time_t webcreatetimeline(char** buf, int* maxlen, int* pos)
{
	int i = 0;
	struct tm *loctime = NULL;
	int timeadd = 15 * 60;
	char* buf1 = NULL;
	time_t akttimeadd = 0;

#ifdef SIMULATE
	time_t akttime = 1307871000;
	akttime = 1315614961;
#else
	time_t akttime = time(NULL);
#endif

	akttime -= (((akttime) % 100));
	akttime -= (((akttime / 60) % 15) * 60);
	akttimeadd = akttime;

	buf1 = malloc(6);
	if(buf1 == NULL)
	{
		err("no mem");
		return akttime;
	}

	//not shown
	ostrcatbig(buf, "<th class=gepgchannel1> </th><th style=\"width: 1px; max-width: 1px; min-width: 1px;\"></th>", maxlen, pos);
	for(i = 0; i < 96; i++)
	{
		loctime = olocaltime(&akttimeadd);
		if(loctime != NULL)
			strftime(buf1, 6, "%H:%M", loctime);
		else
 			snprintf(buf1, 6, "00:00");
		free(loctime); loctime = NULL;

		ostrcatbig(buf, "<th class=gepgtimelinecell><div class=gepgcellborder>", maxlen, pos);
		ostrcatbig(buf, buf1, maxlen, pos);
		ostrcatbig(buf, "</div></th>", maxlen, pos);

		akttimeadd += timeadd;
	}

	free(buf1);
	return akttime / 60;
}

void webcreatetailbig(char** buf, int* maxlen, int* pos, int flag)
{
	ostrcatbig(buf, "</table></font></center></body></html>", maxlen, pos);
}


char* webcreatetail(char* buf, int flag)
{
	buf = ostrcat(buf, "</table></font></center></body></html>", 1, 0);
	return buf;
}

void webcreateheadbig(char** buf, int* maxlen, char* meta, int* pos, int flag)
{
	ostrcatbig(buf, "<html><head><meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\"><link rel=stylesheet type=text/css href=titan.css>", maxlen, pos);
	if(meta != NULL)
		ostrcatbig(buf, meta, maxlen, pos);
	ostrcatbig(buf, "</head><body class=body>", maxlen, pos);
	if(flag == 0)
		ostrcatbig(buf, "<center><table border=0 width=100%>", maxlen, pos);
	else
		ostrcatbig(buf, "<center><table border=0 width=100% height=100%>", maxlen, pos);
}

char* webcreatehead(char* buf, char* meta, int flag)
{
	buf = ostrcat(buf, "<html><head><meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\"><link rel=stylesheet type=text/css href=titan.css>", 1, 0);
	if(meta != NULL)
		buf = ostrcat(buf, meta, 1, 0);
	buf = ostrcat(buf, "</head><body class=body>", 1, 0);
	if(flag == 0)
		buf = ostrcat(buf, "<center><table border=0 width=100%>", 1, 0);
	else
		buf = ostrcat(buf, "<center><table border=0 width=100% height=100%>", 1, 0);
	return buf;
}

void webcreatechannelbody(char** buf, int line, struct channel* chnode, char* channellist, int* maxlen, int* pos, int flag, int fmt)
{
	char* tmpstr = NULL, *buf1 = NULL, *serviceid = NULL, *transponderid = NULL;
	struct epg* epgnode = getepgakt(chnode);
	int ret = 0, proz = 0;
	struct tm *loctime = NULL;
	time_t max = 0, akt = 0;

	if(chnode == NULL) return;
	ret = channelnottunable(chnode);

	buf1 = malloc(MINMALLOC);
	if(buf1 == NULL)
	{
		err("no mem");
		return;
	}

	if(fmt == 0) 
	{
		if(line == 0)
			ostrcatbig(buf, "<tr class=line1>", maxlen, pos);
		else
			ostrcatbig(buf, "<tr class=line2>", maxlen, pos);
	}

	if(epgnode != NULL)
	{
		proz = (time(NULL) - epgnode->starttime) * 100 / (epgnode->endtime - epgnode->starttime);
		if(proz > 100) proz = 100;
		if(proz < 0) proz = 0;
	}

	if(fmt == 0) 
	{
		ostrcatbig(buf, "<td width=100% valign=middle nowrap class=link><div class=timelineborder><div class=timelinebar style=\"width: ", maxlen, pos);
		tmpstr = oitoa(proz);
		ostrcatbig(buf, tmpstr, maxlen, pos);
		free(tmpstr); tmpstr = NULL;
		ostrcatbig(buf, "%;\"> </div></div>", maxlen, pos);
	}

	serviceid = oitoa(chnode->serviceid);
	transponderid = olutoa(chnode->transponderid);

	if(fmt == 0 && ret == 0)
	{
		ostrcatbig(buf, "<a target=aktservice class=link href=query?switch&", maxlen, pos);
		ostrcatbig(buf, serviceid, maxlen, pos);
		ostrcatbig(buf, "&", maxlen, pos);
		ostrcatbig(buf, transponderid, maxlen, pos);
		ostrcatbig(buf, "&", maxlen, pos);
		ostrcatbig(buf, channellist, maxlen, pos);
		ostrcatbig(buf, ">", maxlen, pos);
	}

	if(fmt == 0) 
	{
		ostrcatbig(buf, chnode->name, maxlen, pos);
		if(ret == 0)
			ostrcatbig(buf, "</a>", maxlen, pos);
	}
	else
	{
		*buf = ostrcat(*buf, chnode->name, 1, 0);
		*buf = ostrcat(*buf, "#", 1, 0);
		*buf = ostrcat(*buf, oitoa(proz), 1, 1);
		*buf = ostrcat(*buf, "#", 1, 0);
		*buf = ostrcat(*buf, serviceid, 1, 0);
		*buf = ostrcat(*buf, "#", 1, 0);
		*buf = ostrcat(*buf, transponderid, 1, 0);
		*buf = ostrcat(*buf, "#", 1, 0);
		*buf = ostrcat(*buf, channellist, 1, 0);
		*buf = ostrcat(*buf, "#", 1, 0);
		*buf = ostrcat(*buf, oitoa(chnode->servicetype), 1, 1);
		*buf = ostrcat(*buf, "#", 1, 0);
		if(epgnode != NULL)
			*buf = ostrcat(*buf, oitoa(epgnode->eventid), 1, 1);
		else
			*buf = ostrcat(*buf, "0", 1, 0);
		*buf = ostrcat(*buf, "\n", 1, 0);
	}

	//akt epg
	if(fmt == 0 && epgnode != NULL)
	{
		if(flag == 0)
			ostrcatbig(buf, "<br><a target=main class=smalllink href=query?getepg&", maxlen, pos);
		else
			ostrcatbig(buf, " <a target=main class=smalllink href=query?getepg&", maxlen, pos);

		ostrcatbig(buf, serviceid, maxlen, pos);
		ostrcatbig(buf, "&", maxlen, pos);
		ostrcatbig(buf, transponderid, maxlen, pos);
		ostrcatbig(buf, "&", maxlen, pos);
		tmpstr = oitoa(epgnode->eventid);
		ostrcatbig(buf, tmpstr, maxlen, pos);
		free(tmpstr); tmpstr = NULL;
		ostrcatbig(buf, ">", maxlen, pos);

		loctime = olocaltime(&epgnode->starttime);
		if(loctime != NULL)
			strftime(buf1, MINMALLOC, "%H:%M -", loctime);
		free(loctime); loctime = NULL;
		loctime = olocaltime(&epgnode->endtime);
		if(loctime != NULL)
			strftime(&buf1[7], MINMALLOC - 8, " %H:%M ", loctime);
		free(loctime); loctime = NULL;

		ostrcatbig(buf, buf1, maxlen, pos);
		ostrcatbig(buf, epgnode->title, maxlen, pos);

		max = (epgnode->endtime - epgnode->starttime) / 60;
		akt = (time(NULL) - epgnode->starttime) / 60;
		if(max < 0) max = 0;
		if(akt < 0) akt = 0;
		snprintf(buf1, MINMALLOC, " (%ld from %ld min, %d%%)", akt, max, proz);
		ostrcatbig(buf, buf1, maxlen, pos);

		ostrcatbig(buf, "</a>", maxlen, pos);

		//next epg
		epgnode = epgnode->next;
		if(flag == 0 && epgnode != NULL)
		{
			ostrcatbig(buf, "<br><a target=main class=smalllink href=query?getepg&", maxlen, pos);
			ostrcatbig(buf, serviceid, maxlen, pos);
			ostrcatbig(buf, "&", maxlen, pos);
			ostrcatbig(buf, transponderid, maxlen, pos);
			ostrcatbig(buf, "&", maxlen, pos);
			tmpstr = oitoa(epgnode->eventid);
			ostrcatbig(buf, tmpstr, maxlen, pos);
			free(tmpstr); tmpstr = NULL;
			ostrcatbig(buf, ">", maxlen, pos);

			loctime = olocaltime(&epgnode->starttime);
			if(loctime != NULL)
				strftime(buf1, 8, "%H:%M -", loctime);
			free(loctime); loctime = NULL;
			loctime = olocaltime(&epgnode->endtime);
			if(loctime != NULL)
				strftime(&buf1[7], 8, " %H:%M ", loctime);
			free(loctime); loctime = NULL;

			ostrcatbig(buf, buf1, maxlen, pos);
			ostrcatbig(buf, epgnode->title, maxlen, pos);
			ostrcatbig(buf, "</a>", maxlen, pos);
		}
	}

	//tv - radio
	if(fmt == 0) 
	{
		if(chnode->servicetype == 0)
			ostrcatbig(buf, "</td><td width=100 align=right valign=middle nowrap><img style=\"margin-left: 5\" border=0 src=img/tv.png alt=TV width=16 height=16>", maxlen, pos);
		else
			ostrcatbig(buf, "</td><td width=100 align=right valign=middle nowrap><img style=\"margin-left: 5\" border=0 src=img/radio.png alt=Radio width=16 height=16>", maxlen, pos);

		//single epg
		ostrcatbig(buf, "<a target=main href=query?getsingleepg&", maxlen, pos);
		ostrcatbig(buf, serviceid, maxlen, pos);
		ostrcatbig(buf, "&", maxlen, pos);
		ostrcatbig(buf, transponderid, maxlen, pos);
		ostrcatbig(buf, "><img style=\"margin-left: 5\" border=0 src=img/singleepg.png alt=\"Single EPG\" width=16 height=16></a>", maxlen, pos);

		//m3u stream + webstream
		if(ret == 0)
		{
			ostrcatbig(buf, "<a target=nothing href=query?getm3u&", maxlen, pos);
			ostrcatbig(buf, serviceid, maxlen, pos);
			ostrcatbig(buf, ",", maxlen, pos);
			ostrcatbig(buf, transponderid, maxlen, pos);
			ostrcatbig(buf, "><img style=\"margin-left: 5\" border=0 src=img/stream.png alt=Stream width=16 height=16></a>", maxlen, pos);

			ostrcatbig(buf, "<a target=_blank href=query?getvideo&", maxlen, pos);
			ostrcatbig(buf, serviceid, maxlen, pos);
			ostrcatbig(buf, ",", maxlen, pos);
			ostrcatbig(buf, transponderid, maxlen, pos);
			ostrcatbig(buf, "><img style=\"margin-left: 5\" border=0 src=img/webstream.png alt=WebStream width=16 height=16></a>", maxlen, pos);
		}
		else
			ostrcatbig(buf, "<img style=\"margin-left: 5\" border=0 src=img/cross.png alt=\"Channel not availabel\" width=16 height=16>", maxlen, pos);
		ostrcatbig(buf, "</td></tr>", maxlen, pos);
	}

	free(buf1);
	free(serviceid);
	free(transponderid);
}

char* webgetbouquetchannel(char* param, int fmt)
{
	char* buf = NULL, *tmpstr = NULL, *tmpstr1 = NULL;
	struct mainbouquet *mbouquet = NULL;
	struct bouquet *node = NULL;
	struct channel* chnode = NULL;
	int line = 0, maxlen = 0, pos = 0;

	if(param == NULL) return NULL;

	if(fmt == 0) webcreateheadbig(&buf, &maxlen, "<meta http-equiv=refresh content=15>", &pos, 0);

	mbouquet = getmainbouquet(param);
	if(mbouquet != NULL)
	{
		node = mbouquet->bouquet;
		tmpstr = ostrcat(tmpstr, "(BOUQUET)-", 1, 0);
		tmpstr = ostrcat(tmpstr, mbouquet->name, 1, 0);
		tmpstr1 = htmlencode(tmpstr);
		free(tmpstr); tmpstr = NULL;
		while(node != NULL)
		{
			chnode = getchannel(node->serviceid, node->transponderid);
			if(chnode != NULL)
			{
				webcreatechannelbody(&buf, line, chnode, tmpstr1, &maxlen, &pos, 0, fmt);
				if(line == 0)
					line = 1;
				else
					line = 0;
			}
			node = node->next;
		}
		free(tmpstr1); tmpstr1 = NULL;
	}
	if(fmt == 0) webcreatetailbig(&buf, &maxlen, &pos, 0);

	return buf;
}

//flag 0: all
//flag 1: sat
//flag 2: provider
//flag 3: A-Z
char* webgetchannel(int param, int flag, int page, int fmt)
{
	char* buf = NULL, *tmpnr = NULL, *tmpstr = NULL, *tmpstr1 = NULL;
	struct channel* chnode = channel;
	int line = 0, maxcount = 0, maxlen = 0, pos = 0;

	if(fmt == 0) webcreateheadbig(&buf, &maxlen, "<meta http-equiv=refresh content=15>", &pos, 0);

	if(flag == 0) tmpstr1 = ostrcat(tmpstr1, "(ALL)", 1, 0);
	if(flag == 1)
	{
		struct sat* node = getsatbyorbitalpos(param);
		if(node != NULL)
		{
			tmpstr = ostrcat(tmpstr, "(SAT)-", 1, 0);
			tmpstr = ostrcat(tmpstr, node->name, 1, 0);
			tmpstr1 = htmlencode(tmpstr);
			free(tmpstr); tmpstr = NULL;
		}
	}
	if(flag == 2)
	{
		struct provider* node = getprovider(param);
		if(node != NULL)
		{
			tmpstr = ostrcat(tmpstr, "(PROVIDER)-", 1, 0);
			tmpstr = ostrcat(tmpstr, node->name, 1, 0);
			tmpstr1 = htmlencode(tmpstr);
			free(tmpstr); tmpstr = NULL;
		}
	}
	if(flag == 3)
	{
		tmpstr = malloc(2);
		if(tmpstr != NULL)
			snprintf(tmpstr, 2, "%c", param);
		tmpstr = ostrcat("(A-Z)-", tmpstr, 0, 1);
		tmpstr1 = htmlencode(tmpstr);
		free(tmpstr); tmpstr = NULL;
	}

	while(chnode != NULL)
	{
		if(chnode->transponder == NULL)
		{
			chnode = chnode->next;
			continue;
		}
		if(chnode->name == NULL)
		{
			chnode = chnode->next;
			continue;
		}
		if(flag == 1 && chnode->transponder->orbitalpos != param)
		{
			chnode = chnode->next;
			continue;
		}
		if(flag == 2 && chnode->providerid != param)
		{
			chnode = chnode->next;
			continue;
		}
		if(flag == 3 && chnode->name[0] != param && chnode->name[0] != param + 32)
		{
			chnode = chnode->next;
			continue;
		}

		maxcount++;
		if(maxcount <= (MAXHTMLLINE * page) - MAXHTMLLINE || maxcount > MAXHTMLLINE * page)
		{
			chnode = chnode->next;
			continue;
		}
		webcreatechannelbody(&buf, line, chnode, tmpstr1, &maxlen, &pos, 0, fmt);

		if(line == 0)
			line = 1;
		else
			line = 0;

		chnode = chnode->next;
	}
	free(tmpstr); tmpstr = NULL;

	if(fmt == 0 && maxcount > MAXHTMLLINE)
	{
		int i;
		ostrcatbig(&buf, "<tr><td align=center><br>", &maxlen, &pos);
		for(i = 1; i <= (int)ceil(((float)maxcount / MAXHTMLLINE)); i++)
		{
			if(page == i)
				ostrcatbig(&buf, "<a class=pagesellink href=query?getchannelpage&", &maxlen, &pos);
			else
				ostrcatbig(&buf, "<a class=pagelink href=query?getchannelpage&", &maxlen, &pos);
			tmpnr = oitoa(param);
			ostrcatbig(&buf, tmpnr, &maxlen, &pos);
			ostrcatbig(&buf, "&", &maxlen, &pos);
			free(tmpnr); tmpnr = NULL;
			tmpnr = oitoa(flag);
			ostrcatbig(&buf, tmpnr, &maxlen, &pos);
			ostrcatbig(&buf, "&", &maxlen, &pos);
			free(tmpnr); tmpnr = NULL;

			tmpnr = oitoa(i);
			ostrcatbig(&buf, tmpnr, &maxlen, &pos);
			ostrcatbig(&buf, ">", &maxlen, &pos);
			ostrcatbig(&buf, tmpnr, &maxlen, &pos);
			ostrcatbig(&buf, "</a>", &maxlen, &pos);
			free(tmpnr); tmpnr = NULL;
		}
		ostrcatbig(&buf, "</td></tr>", &maxlen, &pos);
	}

	if(fmt == 0) webcreatetailbig(&buf, &maxlen, &pos, 0);
	return buf;
}

char* webgetprovider(int fmt)
{
	char* buf = NULL, *tmpstr = NULL;
	struct provider* node = provider;
	int line = 0, maxlen = 0, pos = 0;

	if(fmt == 0) webcreateheadbig(&buf, &maxlen, NULL, &pos, 0);

	while(node != NULL)
	{
		if(fmt == 0) 
		{
			if(line == 0)
			{
				ostrcatbig(&buf, "<tr class=line1><td nowrap><a class=link href=\"query?getproviderchannel&", &maxlen, &pos);
				tmpstr = oitoa(node->providerid);
				ostrcatbig(&buf, tmpstr, &maxlen, &pos);
				free(tmpstr); tmpstr = NULL;
				ostrcatbig(&buf, "\">", &maxlen, &pos);
				line = 1;
			}
			else
			{
				ostrcatbig(&buf, "<tr class=line2><td nowrap><a class=link href=\"query?getproviderchannel&", &maxlen, &pos);
				tmpstr = oitoa(node->providerid);
				ostrcatbig(&buf, tmpstr, &maxlen, &pos);
				free(tmpstr); tmpstr = NULL;
				ostrcatbig(&buf, "\">", &maxlen, &pos);
				line = 0;
			}
			ostrcatbig(&buf, node->name, &maxlen, &pos);
			ostrcatbig(&buf, "</td></tr>", &maxlen, &pos);
		}
		else
		{
			buf = ostrcat(buf, node->name, 1, 0);
			buf = ostrcat(buf, "#", 1, 0);
			buf = ostrcat(buf, oitoa(node->providerid), 1, 1);
			buf = ostrcat(buf, "\n", 1, 0);
		}

		node = node->next;
	}

	if(fmt == 0) webcreatetailbig(&buf, &maxlen, &pos, 0);
	return buf;
}

char* webgetsat(int fmt)
{
	char* buf = NULL, *tmpstr = NULL;
	struct sat* node = sat;
	int line = 0, maxlen = 0, pos = 0;

	if(fmt == 0) webcreateheadbig(&buf, &maxlen, NULL, &pos, 0);

	while(node != NULL)
	{
		if(fmt == 0)
		{
			if(line == 0)
			{
				ostrcatbig(&buf, "<tr class=line1><td nowrap><a class=link href=\"query?getsatchannel&", &maxlen, &pos);
				tmpstr = oitoa(node->orbitalpos);
				ostrcatbig(&buf, tmpstr, &maxlen, &pos);
				free(tmpstr); tmpstr = NULL;
				ostrcatbig(&buf, "\">", &maxlen, &pos);
				line = 1;
			}
			else
			{
				ostrcatbig(&buf, "<tr class=line2><td nowrap><a class=link href=\"query?getsatchannel&", &maxlen, &pos);
				tmpstr = oitoa(node->orbitalpos);
				ostrcatbig(&buf, tmpstr, &maxlen, &pos);
				free(tmpstr); tmpstr = NULL;
				ostrcatbig(&buf, "\">", &maxlen, &pos);
				line = 0;
			}
			ostrcatbig(&buf, node->name, &maxlen, &pos);
			ostrcatbig(&buf, "</td></tr>", &maxlen, &pos);
		}
		else
		{
			buf = ostrcat(buf, node->name, 1, 0);
			buf = ostrcat(buf, "#", 1, 0);
			buf = ostrcat(buf, oitoa(node->orbitalpos), 1, 1);
			buf = ostrcat(buf, "\n", 1, 0);
		}

		node = node->next;
	}

	if(fmt == 0) webcreatetailbig(&buf, &maxlen, &pos, 0);
	return buf;
}

char* webgetaz(int fmt)
{
	char* buf = NULL, *tmpstr = NULL, *tmpnr = NULL;
	int line = 0, maxlen = 0, pos = 0, i;

	tmpstr = malloc(2);
	if(tmpstr == NULL)
	{
		err("no memory");
		return NULL;
	}

	if(fmt == 0) webcreateheadbig(&buf, &maxlen, NULL, &pos, 0);

	for(i = 65; i < 91; i++)
	{
		if(fmt == 0)
		{
			if(line == 0)
			{
				ostrcatbig(&buf, "<tr class=line1><td nowrap><a class=link href=\"query?getazchannel&", &maxlen, &pos);
				tmpnr = oitoa(i);
				ostrcatbig(&buf, tmpnr, &maxlen, &pos);
				free(tmpnr); tmpnr = NULL;
				ostrcatbig(&buf, "\">", &maxlen, &pos);
				line = 1;
			}
			else
			{
				ostrcatbig(&buf, "<tr class=line2><td nowrap><a class=link href=\"query?getazchannel&", &maxlen, &pos);
				tmpnr = oitoa(i);
				ostrcatbig(&buf, tmpnr, &maxlen, &pos);
				free(tmpnr); tmpnr = NULL;
				ostrcatbig(&buf, "\">", &maxlen, &pos);
				line = 0;
			}
			snprintf(tmpstr, 2, "%c", i);
			ostrcatbig(&buf, tmpstr, &maxlen, &pos);
			ostrcatbig(&buf, "</td></tr>", &maxlen, &pos);
		}
		else
		{
			snprintf(tmpstr, 2, "%c", i);
			buf = ostrcat(buf, tmpstr, 1, 0);
			buf = ostrcat(buf, "\n", 1, 0);
		}
	}

	if(fmt == 0) webcreatetailbig(&buf, &maxlen, &pos, 0);
	free(tmpstr);
	return buf;
}

char* webgetbouquet(int fmt)
{
	char* buf = NULL;
	struct mainbouquet* node = mainbouquet;
	int line = 0, maxlen = 0, pos = 0;

	if(fmt == 0) webcreateheadbig(&buf, &maxlen, NULL, &pos, 0);

	while(node != NULL)
	{
		if(fmt == 0)
		{
			if(line == 0)
			{
				ostrcatbig(&buf, "<tr class=line1><td width=100% nowrap><a class=link href=\"query?getbouquetchannel&", &maxlen, &pos);
				ostrcatbig(&buf, node->name, &maxlen, &pos);
				ostrcatbig(&buf, "\">", &maxlen, &pos);
				line = 1;
			}
			else
			{
				ostrcatbig(&buf, "<tr class=line2><td width=100% nowrap><a class=link href=\"query?getbouquetchannel&", &maxlen, &pos);
				ostrcatbig(&buf, node->name, &maxlen, &pos);
				ostrcatbig(&buf, "\">", &maxlen, &pos);
				line = 0;
			}
			ostrcatbig(&buf, node->name, &maxlen, &pos);
			ostrcatbig(&buf, "</a></td>", &maxlen, &pos);

			ostrcatbig(&buf, "<td width=50 nowrap align=right valign=middle><img style=\"margin-left: 5\" border=0 src=", &maxlen, &pos);

			if(node->type == 0)
				ostrcatbig(&buf, "img/tv.png width=16 height=16 alt=TV>", &maxlen, &pos);
			else
				ostrcatbig(&buf, "img/radio.png width=16 height=16 alt=Radio>", &maxlen, &pos);
			ostrcatbig(&buf, "<a href=\"query?getgmultiepg&", &maxlen, &pos);
			ostrcatbig(&buf, node->name, &maxlen, &pos);
			ostrcatbig(&buf, "\">", &maxlen, &pos);
			ostrcatbig(&buf, "<img style=\"margin-left: 5\" border=0 width=16 height=16 alt=\"Graphical Multi EPG\" src=img/gmultiepg.png></a>", &maxlen, &pos);
			ostrcatbig(&buf, "</td></tr>", &maxlen, &pos);
		}
		else
		{
			buf = ostrcat(buf, node->name, 1, 0);
			buf = ostrcat(buf, "#", 1, 0);
			buf = ostrcat(buf, oitoa(node->type), 1, 1);
			buf = ostrcat(buf, "\n", 1, 0);
		}

		node = node->next;
	}

	if(fmt == 0) webcreatetailbig(&buf, &maxlen, &pos, 0);
	return buf;
}

char* websetmute(char* param, int fmt)
{
	char* buf = NULL;
	int mute = status.mute;

	if(param != NULL)
		mute = atoi(param);

	if(fmt == 0)
	{
		buf = ostrcat(buf, "<html><head><meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\"><link rel=stylesheet type=text/css href=titan.css></head><body class=body><center>", 1, 0);
		if(mute == 0)
			buf = ostrcat(buf, "<a href=query?setmute&1><img src=img/speak_on.png border=0 alt=\"Mute on\"></img></a>", 1, 0);
		else
			buf = ostrcat(buf, "<a href=query?setmute&0><img src=img/speak_off.png border=0 alt=\"Mute off\"></img></a>", 1, 0);
		buf = ostrcat(buf, "</center></body></html>", 1, 0);
	}
	else
	{
		if(mute == 0)
			buf = ostrcat(buf, "0", 1, 0);
		else
			buf = ostrcat(buf, "1", 1, 0);
	}

	if(mute != status.mute)
		screenmute(NULL, NULL, 0);
	
	return buf;
}

char* websetvol(char* param, int fmt)
{
	char* buf = NULL;
	int vol = 30;

	if(param == NULL)
		vol = getvol();
	else
		vol = atoi(param);

	if(fmt == 0)
	{
		buf = ostrcat(buf, "<html><head><meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\"><link rel=stylesheet type=text/css href=titan.css></head><body class=body><center>", 1, 0);
		if(vol >=10)
			buf = ostrcat(buf, "<a href=query?setvol&10><img src=img/led_on.png border=0></img></a> ", 1, 0);
		else
			buf = ostrcat(buf, "<a href=query?setvol&10><img src=img/led_off.png border=0></img></a> ", 1, 0);
		if(vol >=20)
			buf = ostrcat(buf, "<a href=query?setvol&20><img src=img/led_on.png border=0></img></a> ", 1, 0);
		else
			buf = ostrcat(buf, "<a href=query?setvol&20><img src=img/led_off.png border=0></img></a> ", 1, 0);
		if(vol >=30)
			buf = ostrcat(buf, "<a href=query?setvol&30><img src=img/led_on.png border=0></img></a> ", 1, 0);
		else
			buf = ostrcat(buf, "<a href=query?setvol&30><img src=img/led_off.png border=0></img></a> ", 1, 0);
		if(vol >=40)
			buf = ostrcat(buf, "<a href=query?setvol&40><img src=img/led_on.png border=0></img></a> ", 1, 0);
		else
			buf = ostrcat(buf, "<a href=query?setvol&40><img src=img/led_off.png border=0></img></a> ", 1, 0);
		if(vol >=50)
			buf = ostrcat(buf, "<a href=query?setvol&50><img src=img/led_on.png border=0></img></a> ", 1, 0);
		else
			buf = ostrcat(buf, "<a href=query?setvol&50><img src=img/led_off.png border=0></img></a> ", 1, 0);
		if(vol >=60)
			buf = ostrcat(buf, "<a href=query?setvol&60><img src=img/led_on.png border=0></img></a> ", 1, 0);
		else
			buf = ostrcat(buf, "<a href=query?setvol&60><img src=img/led_off.png border=0></img></a> ", 1, 0);
		if(vol >=70)
			buf = ostrcat(buf, "<a href=query?setvol&70><img src=img/led_on.png border=0></img></a> ", 1, 0);
		else
			buf = ostrcat(buf, "<a href=query?setvol&70><img src=img/led_off.png border=0></img></a> ", 1, 0);
		if(vol >=80)
			buf = ostrcat(buf, "<a href=query?setvol&80><img src=img/led_on.png border=0></img></a> ", 1, 0);
		else
			buf = ostrcat(buf, "<a href=query?setvol&80><img src=img/led_off.png border=0></img></a> ", 1, 0);
		if(vol >=90)
			buf = ostrcat(buf, "<a href=query?setvol&90><img src=img/led_on.png border=0></img></a> ", 1, 0);
		else
			buf = ostrcat(buf, "<a href=query?setvol&90><img src=img/led_off.png border=0></img></a> ", 1, 0);
		if(vol >=100)
			buf = ostrcat(buf, "<a href=query?setvol&100><img src=img/led_on.png border=0></img></a> ", 1, 0);
		else
			buf = ostrcat(buf, "<a href=query?setvol&100><img src=img/led_off.png border=0></img></a> ", 1, 0);
		buf = ostrcat(buf, "</center></body></html>", 1, 0);
	}
	else
		buf = ostrcat(buf, oitoa(vol), 1, 1);

	if(param != NULL)
	{
		setvol(vol);
		if(status.mute == 1)
			screenmute(NULL, NULL, 0);
	}
	return buf;
}

char* webgetaktservice(int fmt)
{
	char* buf = NULL;
	struct channel* chnode = status.aktservice->channel;
	int line = 0, maxlen = 0, pos = 0;

	if(fmt == 0) webcreateheadbig(&buf, &maxlen, NULL, &pos, 1);
	webcreatechannelbody(&buf, line, chnode, getconfig("channellist", NULL), &maxlen, &pos ,1, fmt);
	if(fmt == 0) webcreatetailbig(&buf, &maxlen, &pos, 1);

	return buf;
}

char* webswitch(char* param, int fmt)
{
	if(status.channelswitch == 1) goto end;

	int ret = 0;
	char* param1 = NULL, *param2 = NULL;
	struct channel* chnode = NULL;

	if(param == NULL) goto end;

	//create param1
	param1 = strchr(param, '&');
	if(param1 != NULL)
	{
		*param1++ = '\0';
		param2 = strchr(param1, '&');
		if(param2 != NULL)
			*param2++ = '\0';
	}

	if(param1 == NULL && param2 == NULL) goto end;

	chnode = getchannel(atoi(param), atoi(param1));
	if(chnode != NULL)
	{
		ret = channelnottunable(chnode);
		if(ret == 0)
			ret = servicestart(chnode, param2, NULL, 0);
	}

end:
	return webgetaktservice(fmt);
}

char* webgetm3u(char* param, int connfd, int fmt)
{
	char* buf = NULL, *ip = NULL, *tmpbuf = NULL;
	struct sockaddr_in sin;
	socklen_t len = sizeof(sin);

	if(param == NULL) return NULL;

	if(getsockname(connfd, &sin, &len) < 0)
	{
		perr("getsockname");
		return NULL;
	}

	ip = inet_ntoa(sin.sin_addr);
	if(ip == NULL) return NULL;

	buf = ostrcat(buf, "#EXTM3U\n", 1, 0);
	buf = ostrcat(buf, "#EXTVLCOPT--http-reconnect=true\n", 1, 0);
	buf = ostrcat(buf, "http://", 1, 0);
	buf = ostrcat(buf, ip, 1, 0);
	buf = ostrcat(buf, ":", 1, 0);
	buf = ostrcat(buf, getconfig("streamport", NULL), 1, 0);
	buf = ostrcat(buf, "/", 1, 0);

	tmpbuf = htmlencode(param);
	if(tmpbuf != NULL)
		param = tmpbuf;

	buf = ostrcat(buf, param, 1, 0);
	free(tmpbuf); tmpbuf = NULL;

	return buf;
}

char* webgetvideo(char* param, int connfd, int fmt)
{
	char* buf = NULL, *ip = NULL, *tmpbuf = NULL;
	struct sockaddr_in sin;
	socklen_t len = sizeof(sin);

	if(param == NULL) return NULL;

	if(getsockname(connfd, &sin, &len) < 0)
	{
		perr("getsockname");
		return NULL;
	}

	ip = inet_ntoa(sin.sin_addr);
	if(ip == NULL) return NULL;

	if(fmt == 0)
	{
		buf = ostrcat(buf, "<html><head><meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\"><link rel=stylesheet type=text/css href=titan.css></head><body class=body><table border=0 width=100% height=100%><tr><td align=center valign=middle>", 1, 0);
		buf = ostrcat(buf, "<embed width=100% height=100% type=application/x-vlc-plugin name=video autoplay=yes target=\"http://", 1, 0);
	}
	else
		buf = ostrcat(buf, "http://", 1, 0);

	buf = ostrcat(buf, ip, 1, 0);
	buf = ostrcat(buf, ":", 1, 0);
	buf = ostrcat(buf, getconfig("streamport", NULL), 1, 0);
	buf = ostrcat(buf, "/", 1, 0);

	tmpbuf = htmlencode(param);
	if(tmpbuf != NULL)
		param = tmpbuf;

	buf = ostrcat(buf, param, 1, 0);
	free(tmpbuf); tmpbuf = NULL;

	if(fmt == 0)
	{
		buf = ostrcat(buf, "\" />", 1, 0);
		//buf = ostrcat(buf, "<a href=javascript:; onclick='document.video.play()'>Play</a>", 1, 0);
		//buf = ostrcat(buf, "<a href=javascript:; onclick='document.video.stop()'>Stop</a>", 1, 0);
		//buf = ostrcat(buf, "<a href=javascript:; onclick='document.video.fullscreen()'>Fullscreen</a>", 1, 0);
		buf = ostrcat(buf, "</td></tr></body></html>", 1, 0);
	}

	return buf;
}

char* webvideo(char* param, int fmt)
{
	char* buf = NULL, *param1 = NULL, *tmpbuf = NULL;

	if(param == NULL) return NULL;
  
	//create param1
	param1 = strchr(param, '&');
	if(param1 != NULL)
		*param1++ = '\0';

	if(param1 == NULL) return NULL;

	tmpbuf = htmlencode(param1);
	if(tmpbuf != NULL)
	{
		if(status.play == 0 && status.webplayfile == NULL)
			status.webplayfile = ostrcat(tmpbuf, NULL, 0, 0);
		free(tmpbuf); tmpbuf = NULL;
	}
	tmpbuf = ostrcat("not in play mode", NULL, 0, 0);
    
	int count = 0;
	if(status.play == 0 && ostrcmp("play", param) == 0)
	{
		int count = 0;

		writerc(getrcconfigint("rcplay", NULL));
		while(status.play == 0 && count < 30)
		{
			usleep(100000);
			count++;
		}
	}
	else
		count = 31;

	if(count >= 30)
	{
		free(status.webplayfile); status.webplayfile = NULL;
		tmpbuf = ostrcat("can not start playback", NULL, 0, 0);
	}

	if(status.play == 1)
	{
		if(ostrcmp("stop", param) == 0)
			writerc(getrcconfigint("rcstop", NULL));

		if(ostrcmp("pause", param) == 0)
			writerc(getrcconfigint("rcpause", NULL));
    
		if(ostrcmp("ff", param) == 0)
			writerc(getrcconfigint("rcff", NULL));
    
		if(ostrcmp("fr", param) == 0)
			writerc(getrcconfigint("rcfr", NULL));

		free(tmpbuf); tmpbuf = NULL;
		tmpbuf = ostrcat(param, NULL, 0, 0);

		if(ostrcmp("getlen", param) == 0)
		{
			unsigned long len = 0;
			free(tmpbuf); tmpbuf = NULL;
    
			len = playergetlength();
			tmpbuf = ostrcat(buf, olutoa(len), 1, 1);
		}
  
		if(ostrcmp("getpos", param) == 0)
		{
			unsigned long pos = 0;
			free(tmpbuf); tmpbuf = NULL;
    
			pos = playergetpts() / 90000;
			tmpbuf = ostrcat(buf, olutoa(pos), 1, 1);
		}
  
		if(ostrcmp("getisplaying", param) == 0)
		{
			int playing = 0;
			free(tmpbuf); tmpbuf = NULL;
    
			playing = playerisplaying();
			tmpbuf = ostrcat(buf, oitoa(playing), 1, 1);
		}
  
		if(ostrcmp("getplayercan", param) == 0)
		{
			free(tmpbuf); tmpbuf = NULL;
			tmpbuf = ostrcat(buf, olutoa(status.playercan), 1, 1);
		}
	}

	if(fmt == 0)
	{
		buf = webcreatehead(buf, NULL, 1);
		buf = ostrcat(buf, "<tr><td align=center valign=top><font class=biglabel><br><br>Video ", 1, 0);
		buf = ostrcat(buf, tmpbuf, 1, 1);
		buf = ostrcat(buf, " !!!</font></td></tr>", 1, 0);
		buf = webcreatetail(buf, 1);
	}
	else
		buf = ostrcat(buf, tmpbuf, 1, 1);
  
	return buf;
}

char* webgetchannelpage(char* param, int fmt)
{
	char* param1 = NULL, *param2 = NULL;

	if(param == NULL) return NULL;

	//create param1 + 2
	param1 = strchr(param, '&');
	if(param1 != NULL)
	{
		*param1++ = '\0';
		param2 = strchr(param1, '&');
		if(param2 != NULL)
			*param2++ = '\0';
	}

	if(param1 == NULL || param2 == NULL) return NULL;

	return webgetchannel(atoi(param), atoi(param1), atoi(param2), fmt);
}

void webmessage(struct stimerthread* timernode, char* text, int flag)
{
	textbox(_("Message"), text, _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 0, 0);
	free(text);
}

char* websendmessage(char* param, int fmt)
{
	char* text = NULL, *buf = NULL;

	if(param == NULL) return NULL;
	param = strchr(param, '=');
	if(param != NULL)
	{
		param++;
		stringreplacechar(param, '+', ' ');
		text = ostrcat(text, param, 1, 0);
	}
	addtimer(&webmessage, START, 1000, 1, (void*)text, NULL, NULL);

	if(fmt == 0)
	{
		buf = webcreatehead(buf, NULL, 1);
		buf = ostrcat(buf, "<tr><td align=center valign=top><font class=biglabel><br><br>Message Send !!!</font></td></tr>", 1, 0);
		buf = webcreatetail(buf, 1);
	}
	else
		buf = ostrcat(buf, "Message Send", 1, 0);
		
	return buf;
}

char* webgetsignal(int fmt)
{
	char* buf = NULL, *tmpnr = NULL;
	uint16_t snr = 0, signal = 0;
	uint32_t ber = 0, unc = 0;

	ber = fereadber(status.aktservice->fedev);
	unc = fereaduncorrectedblocks(status.aktservice->fedev);
	signal = fereadsignalstrength(status.aktservice->fedev);
	snr = fereadsnr(status.aktservice->fedev);
        snr = (snr * 100) / 0xffff;

	if(fmt == 0)
	{
		buf = webcreatehead(buf, "<meta http-equiv=refresh content=1>", 0);

		buf = ostrcat(buf, "<tr><td align=center><font class=biglabel><br><br>BER: ", 1, 0);
		tmpnr = oitoa(ber);
		buf = ostrcat(buf, tmpnr, 1, 0);
		free(tmpnr); tmpnr = NULL;

		buf = ostrcat(buf, "<br>UNC: ", 1, 0);
		tmpnr = oitoa(unc);
		buf = ostrcat(buf, tmpnr, 1, 0);
		free(tmpnr); tmpnr = NULL;

		buf = ostrcat(buf, "<br>SIG: ", 1, 0);
		tmpnr = oitoa(signal);
		buf = ostrcat(buf, tmpnr, 1, 0);
		free(tmpnr); tmpnr = NULL;

		buf = ostrcat(buf, "<br>SNR: ", 1, 0);
		tmpnr = oitoa(snr);
		buf = ostrcat(buf, tmpnr, 1, 0);
		free(tmpnr); tmpnr = NULL;

		buf = ostrcat(buf, "</font></td></tr>", 1, 0);
		buf = webcreatetail(buf, 0);
	}
	else
	{
		buf = ostrcat(buf, oitoa(ber), 1, 1);
		buf = ostrcat(buf, "#", 1, 0);
		buf = ostrcat(buf, oitoa(unc), 1, 1);
		buf = ostrcat(buf, "#", 1, 0);
		buf = ostrcat(buf, oitoa(signal), 1, 1);
		buf = ostrcat(buf, "#", 1, 0);
		buf = ostrcat(buf, oitoa(snr), 1, 1);
	}

	return buf;
}

char* webgetepg(char* param, int fmt)
{
	char* buf = NULL, *buf1 = NULL, *tmpstr = NULL, *param1 = NULL, *param2 = NULL;
	struct epg* epgnode = NULL;
	struct channel* chnode = NULL;
	struct tm *loctime = NULL;

	if(param == NULL) return NULL;

	//create param1 + 2
	param1 = strchr(param, '&');
	if(param1 != NULL)
	{
		*param1++ = '\0';
		param2 = strchr(param1, '&');
		if(param2 != NULL)
			*param2++ = '\0';
	}

	if(param1 == NULL || param2 == NULL) return NULL;

	chnode = getchannel(atoi(param), atoi(param1));
	if(chnode == NULL) return NULL;

	epgnode = getepg(chnode, atoi(param2), 0);
	if(epgnode == NULL) return NULL;

	if(fmt == 0) buf = webcreatehead(buf, NULL, 0);

	buf1 = malloc(MINMALLOC);
	if(buf1 == NULL)
	{
		err("no mem");
		return buf;
	}

	if(fmt == 0) buf = ostrcat(buf, "<tr><td><font class=biglabel>", 1, 0);
	if(epgnode->title != NULL)
	{
		buf = ostrcat(buf, epgnode->title, 1, 0);
		if(fmt == 0)
		{
			buf = ostrcat(buf, " (", 1, 0);
			loctime = olocaltime(&epgnode->starttime);
			if(loctime != NULL)
				strftime(buf1, MINMALLOC, "%H:%M -", loctime);
			free(loctime); loctime = NULL;
			loctime = olocaltime(&epgnode->endtime);
			if(loctime != NULL)
				strftime(&buf1[7], MINMALLOC - 8, " %H:%M", loctime);
			free(loctime); loctime = NULL;
			buf = ostrcat(buf, buf1, 1, 0);
			buf = ostrcat(buf, ")", 1, 0);
		}
	}
	if(fmt == 0)
	{
		buf = ostrcat(buf, "<br><br></font></td></tr>", 1, 0);
		buf = ostrcat(buf, "<tr><td><font class=label>", 1, 0);
	}
	else
	{
		buf = ostrcat(buf, "#", 1, 0);
		buf = ostrcat(buf, olutoa(epgnode->starttime), 1, 1);
		buf = ostrcat(buf, "#", 1, 0);
		buf = ostrcat(buf, olutoa(epgnode->endtime), 1, 1);
		buf = ostrcat(buf, "#", 1, 0);
	}

	if(epgnode->subtitle != NULL)
		buf = ostrcat(buf, epgnode->subtitle, 1, 0);

	if(fmt == 0) 
	{
		buf = ostrcat(buf, "<br><br></font></td></tr>", 1, 0);
		buf = ostrcat(buf, "<tr><td><font class=label>", 1, 0);
	}
	else
		buf = ostrcat(buf, "#", 1, 0);

	tmpstr = epgdescunzip(epgnode);
	if(tmpstr != NULL)
		buf = ostrcat(buf, tmpstr, 1, 0);
	free(tmpstr); tmpstr = NULL;

	if(fmt == 0)
	{
		buf = ostrcat(buf, "</font></td></tr>", 1, 0);
		buf = webcreatetail(buf, 0);
	}

	free(buf1);
	return buf;
}

char* webgetsingleepg(char* param, int fmt)
{
	int line = 0, maxlen = 0, pos = 0;
	char* buf = NULL, *buf1 = NULL, *buf2 = NULL, *param1 = NULL, *tmpstr = NULL;
	struct epg* epgnode = NULL;
	struct channel* chnode = NULL;
	struct tm *loctime = NULL;

	//create param1
	param1 = strchr(param, '&');
	if(param1 != NULL)
		*param1++ = '\0';

	if(param1 == NULL) return NULL;

	chnode = getchannel(atoi(param), atoi(param1));
	if(chnode == NULL) return NULL;
	epgnode = getepgakt(chnode);;

	if(fmt == 0) webcreateheadbig(&buf, &maxlen, NULL, &pos, 0);

	buf1 = malloc(MINMALLOC);
	if(buf1 == NULL)
	{
		err("no mem");
		return buf;
	}

	while(epgnode != NULL)
	{
		if(fmt == 0)
		{
			if(line == 0)
			{
				ostrcatbig(&buf, "<tr class=line1>", &maxlen, &pos);
				line = 1;
			}
			else
			{
				ostrcatbig(&buf, "<tr class=line2>", &maxlen, &pos);
				line = 0;
			}

			ostrcatbig(&buf, "<td nowrap><a target=main class=link href=query?getepg&", &maxlen, &pos);
			tmpstr = oitoa(chnode->serviceid);
			ostrcatbig(&buf, tmpstr, &maxlen, &pos);
			buf2 = ostrcat("<td nowrap><a target=main class=link href=query?addrectimer&",tmpstr, 0, 0);
			buf2 = ostrcat(buf2, "&", 0, 0);
			free(tmpstr); tmpstr = NULL;
			ostrcatbig(&buf, "&", &maxlen, &pos);
			tmpstr = olutoa(chnode->transponderid);
			ostrcatbig(&buf, tmpstr, &maxlen, &pos);
			buf2 = ostrcat(buf2, tmpstr, 0, 0);
			buf2 = ostrcat(buf2, "&", 0, 0);
			free(tmpstr); tmpstr = NULL;
			ostrcatbig(&buf, "&", &maxlen, &pos);
			tmpstr = oitoa(epgnode->eventid);
			ostrcatbig(&buf, tmpstr, &maxlen, &pos);
			buf2 = ostrcat(buf2, tmpstr, 0, 0);
			buf2 = ostrcat(buf2, ">", 0, 0);
			free(tmpstr); tmpstr = NULL;
			ostrcatbig(&buf, ">", &maxlen, &pos);

			loctime = olocaltime(&epgnode->starttime);
			if(loctime != NULL)
			strftime(buf1, MINMALLOC, "%d.%m _ %H:%M __ ", loctime);
			//strftime(buf1, MINMALLOC, "%H:%M -", loctime);
			free(loctime); loctime = NULL;
			//loctime = olocaltime(&epgnode->endtime);
			//if(loctime != NULL)
			//	strftime(&buf1[7], MINMALLOC - 8, " %H:%M ", loctime);
			//free(loctime); loctime = NULL;
			ostrcatbig(&buf, buf1, &maxlen, &pos);
			ostrcatbig(&buf, " ", &maxlen, &pos);
			ostrcatbig(&buf, epgnode->title, &maxlen, &pos);
			if(epgnode->subtitle != NULL)
			{
				ostrcatbig(&buf, " (", &maxlen, &pos);
				ostrcatbig(&buf, epgnode->subtitle, &maxlen, &pos);
				ostrcatbig(&buf, ")", &maxlen, &pos);
			}

			ostrcatbig(&buf, "</a></td>", &maxlen, &pos);
			ostrcatbig(&buf, buf2, &maxlen, &pos);
			ostrcatbig(&buf, "<img border=0 width=16 height=16 src=img/timer.png alt=\"set timer\"/>", &maxlen, &pos);
			ostrcatbig(&buf, "</a></td></tr>", &maxlen, &pos);
		}
		else
		{
			buf = ostrcat(buf, epgnode->title, 1, 0);
			buf = ostrcat(buf, "#", 1, 0);
			buf = ostrcat(buf, olutoa(epgnode->starttime), 1, 1);
			buf = ostrcat(buf, "#", 1, 0);
			buf = ostrcat(buf, olutoa(epgnode->endtime), 1, 1);
			buf = ostrcat(buf, "#", 1, 0);
			buf = ostrcat(buf, epgnode->subtitle, 1, 0);
			buf = ostrcat(buf, "\n", 1, 0);
		}
	
		epgnode = epgnode->next;
		free(buf2); buf2 = NULL;
	}

	if(fmt == 0) webcreatetailbig(&buf, &maxlen, &pos, 0);

	free(buf1);
	return buf;
}

//TODO: create rectimer line
void webcalcrecline(char** buf, int* maxlen, int* pos)
{
	//ostrcatbig(buf, "<table border=0 cellpadding=0 cellspacing=0 style=\"table-layout: fixed\"><tr><td width=500 style=\"background-color: c0c0c0; height: 2; border-left: 2px solid #303030;\"></td></tr></table>", maxlen, pos);
}

char* webgetgmultiepg(char* param, int fmt)
{
	int page = 1, i, line = 0, treffer = 0, maxlen = 0, pos = 0;
	char* buf = NULL, *tmpnr = NULL, *param1 = NULL;
	struct mainbouquet *mbouquet = NULL;
	struct bouquet *node = NULL;
	struct channel* chnode = NULL;
	struct epg* epgnode = NULL;
	time_t akttime = 0, difftime = 0, starttime = 0, endtime = 0, lastendtime = 0;

	if(param == NULL) return NULL;

	//create param1
	param1 = strchr(param, '&');
	if(param1 != NULL)
		*param1++ = '\0';

	if(param1 != NULL) page = atoi(param1);

	ostrcatbig(&buf, "<html><head><meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\"><link rel=stylesheet type=text/css href=titan.css><script type=text/javascript src=titan.js></script></head><body class=body><center><table width=100% height=100%><tr><td height=100%><table cellpadding=0 cellspacing=0 border=0 id=gmultiepg class=scrolltable style=\"table-layout: fixed;\" cellpadding=0><thead><tr>", &maxlen, &pos);

	akttime = webcreatetimeline(&buf, &maxlen, &pos);
	akttime += ((page - 1) * 1440);
	ostrcatbig(&buf, "</tr></thead><tbody>", &maxlen, &pos);

	mbouquet = getmainbouquet(param);
	if(mbouquet != NULL)
	{
		node = mbouquet->bouquet;
		while(node != NULL)
		{
			chnode = getchannel(node->serviceid, node->transponderid);
			if(chnode != NULL)
			{
				if(line == 0)
				{
					ostrcatbig(&buf, "<tr><th class=gepgchannel1>", &maxlen, &pos);
					ostrcatbig(&buf, chnode->name, &maxlen, &pos);
					ostrcatbig(&buf, "</th><td class=gepgchannelcell colspan=1000>", &maxlen, &pos);
					webcalcrecline(&buf, &maxlen, &pos);
					ostrcatbig(&buf, "<table border=0 cellpadding=0 cellspacing=0 style=\"table-layout: fixed;\"><tr class=line1>", &maxlen, &pos);
					line = 1;
				}
				else
				{
					ostrcatbig(&buf, "<tr><th class=gepgchannel2>", &maxlen, &pos);
					ostrcatbig(&buf, chnode->name, &maxlen, &pos);
					ostrcatbig(&buf, "</th><td class=gepgchannelcell colspan=1000>", &maxlen, &pos);
					webcalcrecline(&buf, &maxlen, &pos);

					ostrcatbig(&buf, "<table border=0 cellpadding=0 cellspacing=0 style=\"table-layout: fixed;\"><tr class=line2>", &maxlen, &pos);
					line = 0;
				}

				treffer = 0;
				lastendtime = 0;
				epgnode = getepgakt(chnode);
				while(epgnode != NULL)
				{
					treffer = 1;
					starttime = epgnode->starttime / 60;
					endtime = epgnode->endtime / 60;

					if(endtime <= starttime || starttime >= akttime + 1440)
					{
						epgnode = epgnode->next;
						continue;
					}

					//check if old endtime > new starttime
					if(lastendtime > 0 && starttime > lastendtime)
					{
						difftime = (starttime - lastendtime) * 5;

						if(difftime <= 0)
						{
							epgnode = epgnode->next;
							continue;
						}

						ostrcatbig(&buf, "<td class=gepgcellblank style=\"", &maxlen, &pos);
						tmpnr = oitoa(difftime);
						ostrcatbig(&buf, "width:", &maxlen, &pos);
						ostrcatbig(&buf, tmpnr, &maxlen, &pos);
						ostrcatbig(&buf, "px;", &maxlen, &pos);
						ostrcatbig(&buf, "min-width:", &maxlen, &pos);
						ostrcatbig(&buf, tmpnr, &maxlen, &pos);
						ostrcatbig(&buf, "px;", &maxlen, &pos);
						ostrcatbig(&buf, "max-width:", &maxlen, &pos);
						ostrcatbig(&buf, tmpnr, &maxlen, &pos);
						ostrcatbig(&buf, "px;", &maxlen, &pos);
						free(tmpnr); tmpnr = NULL;

						ostrcatbig(&buf, "\"> </td>", &maxlen, &pos);
					}

					//check if starttime < last endtime
					if(starttime < lastendtime)
						starttime = lastendtime;

					//check if endtime is in next day
					if(endtime > akttime + 1440)
						endtime = endtime - (endtime - (akttime + 1440));

					//check if starttime is in prev day
					if(starttime < akttime)
						difftime = (endtime - akttime) * 5;
					else
						difftime = (endtime - starttime) * 5;

					if(difftime <= 0)
					{
						epgnode = epgnode->next;
						continue;
					}

					lastendtime = endtime;

					ostrcatbig(&buf, "<td class=gepgcell style=\"", &maxlen, &pos);
					tmpnr = oitoa(difftime);
					ostrcatbig(&buf, "width:", &maxlen, &pos);
					ostrcatbig(&buf, tmpnr, &maxlen, &pos);
					ostrcatbig(&buf, "px;", &maxlen, &pos);
					ostrcatbig(&buf, "min-width:", &maxlen, &pos);
					ostrcatbig(&buf, tmpnr, &maxlen, &pos);
					ostrcatbig(&buf, "px;", &maxlen, &pos);
					ostrcatbig(&buf, "max-width:", &maxlen, &pos);
					ostrcatbig(&buf, tmpnr, &maxlen, &pos);
					ostrcatbig(&buf, "px;", &maxlen, &pos);
					free(tmpnr); tmpnr = NULL;

					ostrcatbig(&buf, "\"><div class=gepgcellborder>", &maxlen, &pos);
					ostrcatbig(&buf, epgnode->title, &maxlen, &pos);
					ostrcatbig(&buf, "</div></div></td>", &maxlen, &pos);
					
					epgnode = epgnode->next;
				}
				if(treffer == 0)
					ostrcatbig(&buf, "<td class=gepgcellblank> </td>", &maxlen, &pos);
			}
			ostrcatbig(&buf, "</tr></table></td></tr>", &maxlen, &pos);

			node = node->next;
		}
	}
	ostrcatbig(&buf, "<script type=text/javascript>if(typeof tableScroll == 'function'){tableScroll('gmultiepg');}</script></tbody></table></td></tr><tr><td align=center>", &maxlen, &pos);

	for(i = 1; i <= 14; i++)
	{
		if(page == i)
			ostrcatbig(&buf, "<a class=pagesellink href=\"query?getgmultiepg&", &maxlen, &pos);
		else
			ostrcatbig(&buf, "<a class=pagelink href=\"query?getgmultiepg&", &maxlen, &pos);
		ostrcatbig(&buf, param, &maxlen, &pos);
		ostrcatbig(&buf, "&", &maxlen, &pos);
		tmpnr = oitoa(i);
		ostrcatbig(&buf, tmpnr, &maxlen, &pos);
		ostrcatbig(&buf, "\">", &maxlen, &pos);
		ostrcatbig(&buf, tmpnr, &maxlen, &pos);
		ostrcatbig(&buf, "</a>", &maxlen, &pos);
		free(tmpnr); tmpnr = NULL;
	}
	
	ostrcatbig(&buf, "</td></tr></table></center></body></html>", &maxlen, &pos);
	return buf;
}

void webgetshoot(char* param, int fmt)
{
	char* cmd = NULL, *tmpstr = NULL;

	if(status.aktservice->channel != NULL)
	{
		cmd = ostrcat(cmd, "grab.sh ", 1, 0);
		cmd = ostrcat(cmd, param, 1, 0);
		cmd = ostrcat(cmd, " ", 1, 0);
		cmd = ostrcat(cmd, getconfig("streamport", NULL), 1, 0);
		cmd = ostrcat(cmd, " ", 1, 0);
		tmpstr = oitoa(status.aktservice->channel->serviceid);
		cmd = ostrcat(cmd, tmpstr, 1, 0);
		free(tmpstr); tmpstr = NULL;
		cmd = ostrcat(cmd, ",", 1, 0);
		tmpstr = olutoa(status.aktservice->channel->transponderid);
		cmd = ostrcat(cmd, tmpstr, 1, 0);
		free(tmpstr); tmpstr = NULL;
		cmd = ostrcat(cmd, " titan", 1, 0);
	}

	if(cmd != NULL)
		system(cmd);
	free(cmd);
}

char* webgetepgsearch(char* query, char* param, int fmt)
{
	int line = 0, maxlen = 0, pos = 0, maxcount = 0, page = 1;
	char* buf = NULL, *buf1 = NULL, *buf2 = NULL, *tmpstr = NULL, *tmpnr = NULL, * param1 = NULL;
	struct channel* chnode = channel;
	struct epg* epgnode = NULL;
	struct tm *loctime = NULL;

	if(query == NULL) return NULL;

	if(param == NULL)
	{
		query = strchr(query, '=');
		if(query != NULL)
		{
			query++;
			stringreplacechar(param, '+', ' ');
			param = query;
		}
	}
	else
	{
		//create param1
		param1 = strchr(param, '&');
		if(param1 != NULL)
		{
			*param1++ = '\0';
			page = atoi(param1);
		}	
	}

	buf1 = malloc(MINMALLOC);
	if(buf1 == NULL)
	{
		err("no mem");
		return buf;
	}

	if(fmt == 0) webcreateheadbig(&buf, &maxlen, NULL, &pos, 0);
	while(chnode != NULL)
	{
		epgnode = getepgakt(chnode);
		while(epgnode != NULL)
		{
			if(epgnode->title != NULL && ostrstrcase(epgnode->title, param) != NULL)
			{

				maxcount++;
				if(maxcount <= (MAXHTMLLINE * page) - MAXHTMLLINE || maxcount > MAXHTMLLINE * page)
				{
					epgnode = epgnode->next;
					continue;
				}

				if(fmt == 0)
				{
					if(line == 0)
					{
						ostrcatbig(&buf, "<tr class=line1>", &maxlen, &pos);
						line = 1;
					}
					else
					{
						ostrcatbig(&buf, "<tr class=line2>", &maxlen, &pos);
						line = 0;
					}
				}

				if(fmt == 0)
				{
					ostrcatbig(&buf, "<td nowrap><a target=main class=link href=query?getepg&", &maxlen, &pos);
					tmpstr = oitoa(chnode->serviceid);
					buf2 = ostrcat("<td nowrap><a target=main class=link href=query?addrectimer&", tmpstr, 0, 0);
					buf2 = ostrcat(buf2, "&", 0, 0);
					ostrcatbig(&buf, tmpstr, &maxlen, &pos);
					free(tmpstr); tmpstr = NULL;
					ostrcatbig(&buf, "&", &maxlen, &pos);
					tmpstr = olutoa(chnode->transponderid);
					ostrcatbig(&buf, tmpstr, &maxlen, &pos);
					buf2 = ostrcat(buf2, tmpstr, 0, 0);
					buf2 = ostrcat(buf2, "&", 0, 0);
					free(tmpstr); tmpstr = NULL;
					ostrcatbig(&buf, "&", &maxlen, &pos);
					tmpstr = oitoa(epgnode->eventid);
					ostrcatbig(&buf, tmpstr, &maxlen, &pos);
					buf2 = ostrcat(buf2, tmpstr, 0, 0);
					buf2 = ostrcat(buf2, ">", 0, 0);
					free(tmpstr); tmpstr = NULL;
					ostrcatbig(&buf, ">", &maxlen, &pos);
				}
	
				loctime = olocaltime(&epgnode->starttime);
				if(loctime != NULL)
					strftime(buf1, MINMALLOC, "%d.%m _ %H:%M __ ", loctime);
					//strftime(buf1, MINMALLOC, "%H:%M -", loctime);
				free(loctime); loctime = NULL;
				//loctime = olocaltime(&epgnode->endtime);
				//if(loctime != NULL)
				//	strftime(&buf1[7], MINMALLOC - 8, " %H:%M ", loctime);
				//free(loctime); loctime = NULL;
				ostrcatbig(&buf, buf1, &maxlen, &pos);
				ostrcatbig(&buf, " ", &maxlen, &pos);
	
				ostrcatbig(&buf, epgnode->title, &maxlen, &pos);
				if(chnode->name != NULL)
				{
					ostrcatbig(&buf, " (", &maxlen, &pos);
					ostrcatbig(&buf, chnode->name, &maxlen, &pos);
					ostrcatbig(&buf, ")", &maxlen, &pos);
				}
				if(fmt == 0)
					ostrcatbig(&buf, "</a><br><font class=smalllabel1>", &maxlen, &pos);
				else
					ostrcatbig(&buf, "#", &maxlen, &pos);
				ostrcatbig(&buf, epgnode->subtitle, &maxlen, &pos);
				if(fmt == 0)
				{
					ostrcatbig(&buf, "</font></td>", &maxlen, &pos);
					ostrcatbig(&buf, buf2, &maxlen, &pos);
					ostrcatbig(&buf, "<img border=0 width=16 height=16 src=img/timer.png alt=\"set timer\"/>", &maxlen, &pos);
					ostrcatbig(&buf, "</a></td></tr>", &maxlen, &pos);
				}
				else
					ostrcatbig(&buf, "#", &maxlen, &pos);
			}
			epgnode = epgnode->next;
			free(buf2); buf2 = NULL;
		}
		chnode = chnode->next;
	}

	if(maxcount > MAXHTMLLINE && fmt == 0)
	{
		int i;
		ostrcatbig(&buf, "<tr><td align=center><br>", &maxlen, &pos);
		for(i = 1; i <= (int)ceil(((float)maxcount / MAXHTMLLINE)); i++)
		{
			if(page == i)
				ostrcatbig(&buf, "<a class=pagesellink href=query?getepgsearch&", &maxlen, &pos);
			else
				ostrcatbig(&buf, "<a class=pagelink href=query?getepgsearch&", &maxlen, &pos);
			ostrcatbig(&buf, param, &maxlen, &pos);
			ostrcatbig(&buf, "&", &maxlen, &pos);
			free(tmpnr); tmpnr = NULL;

			tmpnr = oitoa(i);
			ostrcatbig(&buf, tmpnr, &maxlen, &pos);
			ostrcatbig(&buf, ">", &maxlen, &pos);
			ostrcatbig(&buf, tmpnr, &maxlen, &pos);
			ostrcatbig(&buf, "</a>", &maxlen, &pos);
			free(tmpnr); tmpnr = NULL;
		}
		ostrcatbig(&buf, "</td></tr>", &maxlen, &pos);
	}

	if(fmt == 0) webcreatetailbig(&buf, &maxlen, &pos, 0);
	free(buf1);
	return buf;
}

//flagbit 0: restrict to path
//flagbit 1: show epg icon
//flagbit 2: show delete icon
//flagbit 3: show stream icon
//flagbit 4: show websteam icon
char* webgetfilelist(char* param, char* link, char* dellink, char* path, char* mask, int flag, int fmt)
{
	char* buf = NULL, *tmppath = NULL, *tmpnr = NULL, *param1 = NULL;
	int maxlen = 0, pos = 0, line = 0, maxcount = 0, page = 1;
	struct skin* webdir = getscreen("webdir");
	struct skin* filelist = getscreennode(webdir, "filelist");
	struct skin* filelistpath = getscreennode(webdir, "filelistpath");
	struct skin* node = NULL;

	if(param == NULL) param = path;

	//create param1
	param1 = strchr(param, '&');
	if(param1 != NULL)
		*param1++ = '\0';

	if(param1 != NULL) page = atoi(param1);

	if(strlen(param) == 0 || !isdir(param))
		tmppath = ostrcat(path, "", 0, 0);
	else
		tmppath = ostrcat(param, "", 0, 0);

	char* tmppath1 = createpath(tmppath, "");
	free(tmppath); tmppath = tmppath1;

	if(checkbit(flag, 0) == 1)
	{
		if(strstr(tmppath, path) != tmppath)
		{
			free(tmppath); tmppath = NULL;
			tmppath = ostrcat(path, "", 0, 0);
		}
	}

	changemask(filelist, mask);
	changeinput(filelist, tmppath);
	changetext(filelistpath, filelist->input);

	free(tmppath); tmppath = NULL;

	delmarkedscreennodes(webdir, FILELISTDELMARK);
	createfilelist(webdir, filelist, 0);

	if(fmt == 0) ostrcatbig(&buf, "<html><head><meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\"><link rel=stylesheet type=text/css href=titan.css><script type=text/javascript src=titan.js></script></head><body class=body><center><table width=100%>", &maxlen, &pos);

	node = filelist;
	while(node != NULL)
	{
		if(node->del == FILELISTDELMARK)
		{
			maxcount++;
			if(maxcount <= (MAXHTMLLINE * page) - MAXHTMLLINE || maxcount > MAXHTMLLINE * page)
			{
				node = node->next;
				continue;
			}

			if(fmt == 0)
			{
				if(line == 0)
				{
					ostrcatbig(&buf, "<tr class=line1>", &maxlen, &pos);
					line = 1;
				}
				else
				{
					ostrcatbig(&buf, "<tr class=line2>", &maxlen, &pos);
					line = 0;
				}

				ostrcatbig(&buf, "<td>", &maxlen, &pos);
			}
			if(node->input != NULL) //dir
			{
				if(fmt == 0)
				{
					ostrcatbig(&buf, "<img border=0 width=16 height=16 src=img/folder.png alt=Directory> <a class=link href=\"query?", &maxlen, &pos);
					ostrcatbig(&buf, link, &maxlen, &pos);
					ostrcatbig(&buf, "&", &maxlen, &pos);
					ostrcatbig(&buf, node->input, &maxlen, &pos);
					ostrcatbig(&buf, "\">", &maxlen, &pos);
					ostrcatbig(&buf, node->text, &maxlen, &pos);
					ostrcatbig(&buf, "</a><td> </td>", &maxlen, &pos);
				}
				else
				{
					ostrcatbig(&buf, node->text, &maxlen, &pos);
					ostrcatbig(&buf, "#0", &maxlen, &pos);
					ostrcatbig(&buf, "\n", &maxlen, &pos);
				}
			}
			else
			{
				if(fmt == 0)
				{
					ostrcatbig(&buf, "<font class=label1>", &maxlen, &pos);
					ostrcatbig(&buf, node->text, &maxlen, &pos);
					ostrcatbig(&buf, "</font></td>", &maxlen, &pos);

					ostrcatbig(&buf, "<td width=80 align=right>", &maxlen, &pos);

					//epg png
					if(checkbit(flag, 1) == 1)
					{
						ostrcatbig(&buf, "<a href=\"query?getmovieepg&", &maxlen, &pos);
						ostrcatbig(&buf, filelistpath->text, &maxlen, &pos);
						ostrcatbig(&buf, "/", &maxlen, &pos);
						ostrcatbig(&buf, node->text, &maxlen, &pos);
						ostrcatbig(&buf, "\">", &maxlen, &pos);
						ostrcatbig(&buf, "<img border=0 src=img/movieepg.png width=16 height=16 alt=EPG></a>", &maxlen, &pos);
					}
					//delete png
					if(checkbit(flag, 2) == 1)
					{
						ostrcatbig(&buf, "<img border=0 src=img/delete.png width=16 height=16 alt=Delete onclick='delquestion(\"", &maxlen, &pos);
						ostrcatbig(&buf, "query?", &maxlen, &pos);
						ostrcatbig(&buf, dellink, &maxlen, &pos);
						ostrcatbig(&buf, "&", &maxlen, &pos);
						ostrcatbig(&buf, filelistpath->text, &maxlen, &pos);
						ostrcatbig(&buf, "&", &maxlen, &pos);
						tmpnr = oitoa(page);
						ostrcatbig(&buf, tmpnr, &maxlen, &pos);
						free(tmpnr); tmpnr = NULL;
						ostrcatbig(&buf, "&", &maxlen, &pos);
						ostrcatbig(&buf, node->text, &maxlen, &pos);
						ostrcatbig(&buf, "\");'>", &maxlen, &pos);
					}
					//stream png
					if(checkbit(flag, 3) == 1)
					{
						ostrcatbig(&buf, "<a target=nothing href=\"query?getm3u&0,0,", &maxlen, &pos);
						ostrcatbig(&buf, filelistpath->text, &maxlen, &pos);
						ostrcatbig(&buf, "/", &maxlen, &pos);
						ostrcatbig(&buf, node->text, &maxlen, &pos);
						ostrcatbig(&buf, "\">", &maxlen, &pos);

						ostrcatbig(&buf, "<img border=0 src=img/stream.png width=16 height=16 alt=Stream></a>", &maxlen, &pos);
					}
					//webstream png
					if(checkbit(flag, 4) == 1)
					{
						ostrcatbig(&buf, "<a target=_blank href=\"query?getvideo&0,0,", &maxlen, &pos);
						ostrcatbig(&buf, filelistpath->text, &maxlen, &pos);
						ostrcatbig(&buf, "/", &maxlen, &pos);
						ostrcatbig(&buf, node->text, &maxlen, &pos);
						ostrcatbig(&buf, "\">", &maxlen, &pos);

						ostrcatbig(&buf, "<img border=0 src=img/webstream.png width=16 height=16 alt=WebStream></a>", &maxlen, &pos);

						ostrcatbig(&buf, "<a target=_blank href=\"", &maxlen, &pos);
						ostrcatbig(&buf, filelistpath->text, &maxlen, &pos);
						ostrcatbig(&buf, "/", &maxlen, &pos);
						ostrcatbig(&buf, node->text, &maxlen, &pos);
						ostrcatbig(&buf, "\">", &maxlen, &pos);
	
						ostrcatbig(&buf, "<img border=0 src=img/icon_restart.png width=16 height=16 alt=Download></a>", &maxlen, &pos);
					}
				}
				else
				{
					ostrcatbig(&buf, node->text, &maxlen, &pos);
					ostrcatbig(&buf, "#1", &maxlen, &pos);
					ostrcatbig(&buf, "\n", &maxlen, &pos);
				}
			}
			if(fmt == 0)
			{
				ostrcatbig(&buf, "</td>", &maxlen, &pos);
				ostrcatbig(&buf, "</tr>", &maxlen, &pos);
			}
		}

		node = node->next;
	}

	if(maxcount > MAXHTMLLINE)
	{
		int i;
		ostrcatbig(&buf, "<tr><td align=center><br>", &maxlen, &pos);
		for(i = 1; i <= (int)ceil(((float)maxcount / MAXHTMLLINE)); i++)
		{
			if(page == i)
			{
				ostrcatbig(&buf, "<a class=pagesellink href=query?", &maxlen, &pos);
				ostrcatbig(&buf, link, &maxlen, &pos);
				ostrcatbig(&buf, "&", &maxlen, &pos);
			}
			else
			{
				ostrcatbig(&buf, "<a class=pagelink href=query?", &maxlen, &pos);
				ostrcatbig(&buf, link, &maxlen, &pos);
				ostrcatbig(&buf, "&", &maxlen, &pos);
			}
			ostrcatbig(&buf, param, &maxlen, &pos);
			ostrcatbig(&buf, "&", &maxlen, &pos);

			tmpnr = oitoa(i);
			ostrcatbig(&buf, tmpnr, &maxlen, &pos);
			ostrcatbig(&buf, ">", &maxlen, &pos);
			ostrcatbig(&buf, tmpnr, &maxlen, &pos);
			ostrcatbig(&buf, "</a>", &maxlen, &pos);
			free(tmpnr); tmpnr = NULL;
		}
		ostrcatbig(&buf, "</td></tr>", &maxlen, &pos);
	}

	if(fmt == 0) webcreatetailbig(&buf, &maxlen, &pos, 0);
	delmarkedscreennodes(webdir, FILELISTDELMARK);
	return buf;
}

char* webgetmovieepg(char* param, char* path, int flag, int fmt)
{
	char* buf = NULL, *tmpstr = NULL;
	char tstr[2];
	char c = 0;
	int first = 1, maxlen = 0, pos = 0;
	FILE* fd = NULL;

	if(fmt == 0)
	{
		webcreateheadbig(&buf, &maxlen, NULL, &pos, 0);
		ostrcatbig(&buf, "<tr><td><font class=biglabel>", &maxlen, &pos);
	}

	if(flag == 1)
	{
		if(param != NULL && strstr(param, path) == param)
		{
			tmpstr = changefilenameext(param, ".epg");
			fd = fopen(tmpstr, "r");
		}
	}
	else
	{
		tmpstr = changefilenameext(param, ".epg");
		fd = fopen(tmpstr, "r");
	}

	if(fd != NULL)
	{
		while(c != EOF)
		{
			c = fgetc(fd);
			if(fmt == 0 && c == '\n')
			{
				if(first == 1)
				{
					ostrcatbig(&buf, "</font><br><font class=label>", &maxlen, &pos);
					first = 0;
				}
				ostrcatbig(&buf, "<br>", &maxlen, &pos);
				continue;
			}
			if(c != EOF)
			{
				sprintf(tstr, "%c", c);
				ostrcatbig(&buf, tstr, &maxlen, &pos);
			}
		}
		fclose(fd);
	}

	if(fmt == 0)
	{
		ostrcatbig(&buf, "</font></td></tr>", &maxlen, &pos);
		webcreatetailbig(&buf, &maxlen, &pos, 0);
	}

	free(tmpstr); tmpstr = NULL;
	return buf;
}

char* webdelfile(char* param, char* link, char* dellink, char* path, char* mask, int flag, int fmt)
{
	char* buf = NULL, *param1 = NULL, *param2 = NULL, *tmpparam = NULL;

	if(param == NULL) return NULL;

	//create param1 + 2
	param1 = strchr(param, '&');
	if(param1 != NULL)
	{
		*param1++ = '\0';
		param2 = strchr(param1, '&');
		if(param2 != NULL)
			*param2++ = '\0';
	}

	if(param1 == NULL || param2 == NULL) return NULL;

	tmpparam = createpath(param, param2);

	if(checkbit(flag, 0) == 1)
	{
		if(tmpparam != NULL && strstr(tmpparam, path) == tmpparam)
			unlink(tmpparam);
	}
	else
		unlink(tmpparam);

	free(tmpparam); tmpparam = NULL;
	tmpparam = ostrcat(tmpparam, param, 1, 0);
	tmpparam = ostrcat(tmpparam, "&", 1, 0);
	tmpparam = ostrcat(tmpparam, param1, 1, 0);

	buf = webgetfilelist(tmpparam, link, dellink, path, mask, flag, fmt);
	free(tmpparam); tmpparam = NULL;
	return buf;
}

void websendrc(char* param, int fmt)
{
	int rccode = 0;

	rccode = getrcconfigint(param, NULL);
	writerc(rccode);
}

//flag 0: get aktiv timer
//flag 1: get old timer
char* webgetrectimer(char* param, int flag, int fmt)
{
	char* buf = NULL, *buf1 = NULL;
	struct rectimer* node = rectimer;
	struct channel* chnode = NULL;
	int line = 0, maxlen = 0, pos = 0;
	struct tm *loctime = NULL;

	buf1 = malloc(MINMALLOC);
	if(buf1 == NULL)
	{
		err("no mem");
		return NULL;
	}

	if(fmt == 0) ostrcatbig(&buf, "<html><head><meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\"><link rel=stylesheet type=text/css href=titan.css><script type=text/javascript src=titan.js></script></head><body class=body><center><table width=100%>", &maxlen, &pos);

	while(node != NULL)
	{
		if((flag == 0 && (node->status == 2 || node->status == 3)) || (flag == 1 && (node->status == 0 || node->status == 1)))
		{
			node = node->next;
			continue;
		}
		if(fmt == 0) 
		{
			if(line == 0)
			{
				ostrcatbig(&buf, "<tr class=line1><td>", &maxlen, &pos);
				line = 1;
			}
			else
			{
				ostrcatbig(&buf, "<tr class=line2><td>", &maxlen, &pos);
				line = 0;
			}

			loctime = olocaltime(&node->begin);
			if(loctime != NULL)
				strftime(buf1, MINMALLOC, "%d-%m-%Y %H:%M -", loctime);
			free(loctime); loctime = NULL;
			loctime = olocaltime(&node->end);
			if(loctime != NULL)
				strftime(&buf1[18], MINMALLOC - 19, " %H:%M ", loctime);
			free(loctime); loctime = NULL;
	
			ostrcatbig(&buf, "<font class=label1>", &maxlen, &pos);
			ostrcatbig(&buf, buf1, &maxlen, &pos);

			ostrcatbig(&buf, " (", &maxlen, &pos);
			if(node->justplay == 0)
				ostrcatbig(&buf, "rec - ", &maxlen, &pos);
			else
				ostrcatbig(&buf, "switch - ", &maxlen, &pos);
			if(node->repeate == 0)
				ostrcatbig(&buf, "once", &maxlen, &pos);
			else
				ostrcatbig(&buf, "repeate", &maxlen, &pos);

			ostrcatbig(&buf, ")</font><br><font class=smalllabel1>", &maxlen, &pos);
			if(node->name == NULL || strlen(node->name) == 0)
				ostrcatbig(&buf, "---", &maxlen, &pos);
			else
				ostrcatbig(&buf, node->name, &maxlen, &pos);

			chnode = getchannel(node->serviceid, node->transponderid);
			if(chnode != NULL)
			{
				ostrcatbig(&buf, " (", &maxlen, &pos);
				ostrcatbig(&buf, chnode->name, &maxlen, &pos);
				ostrcatbig(&buf, ")", &maxlen, &pos);
			}
			ostrcatbig(&buf, "<br>", &maxlen, &pos);

			if(node->status == 0)
				ostrcatbig(&buf, "waiting", &maxlen, &pos);
			else if(node->status == 1)
				ostrcatbig(&buf, "running", &maxlen, &pos);
			else if(node->status == 2)
				ostrcatbig(&buf, "succes", &maxlen, &pos);
			else if(node->status == 3)
			{
				ostrcatbig(&buf, "error", &maxlen, &pos);
				if(node->errstr != NULL && strlen(node->errstr) > 0)
				{
					ostrcatbig(&buf, " (", &maxlen, &pos);
					ostrcatbig(&buf, node->errstr, &maxlen, &pos);
					ostrcatbig(&buf, ")", &maxlen, &pos);
				}
			}

			ostrcatbig(&buf, "</font>", &maxlen, &pos);

			if(flag == 0)
			{
				ostrcatbig(&buf, "</td><td width=40 align=right>", &maxlen, &pos);

				//edit png
				ostrcatbig(&buf, "<a target=main href=\"query?editrectimer", &maxlen, &pos);
				ostrcatbig(&buf, "&", &maxlen, &pos);
				ostrcatbig(&buf, node->timestamp, &maxlen, &pos);
				ostrcatbig(&buf, "\">", &maxlen, &pos);

				ostrcatbig(&buf, "<img border=0 src=img/edit.png width=16 height=16 alt=\"Edit Timer\"></a>", &maxlen, &pos);

				//delete png
				ostrcatbig(&buf, "<img border=0 src=img/delete.png width=16 height=16 alt=Delete onclick='delquestion(\"", &maxlen, &pos);
				ostrcatbig(&buf, "query?delrectimer", &maxlen, &pos);
				ostrcatbig(&buf, "&", &maxlen, &pos);
				ostrcatbig(&buf, node->timestamp, &maxlen, &pos);
				//ostrcatbig(&buf, filelistpath->text, &maxlen, &pos);
				//ostrcatbig(&buf, "&", &maxlen, &pos);
				//tmpnr = oitoa(page);
				//ostrcatbig(&buf, tmpnr, &maxlen, &pos);
				//free(tmpnr); tmpnr = NULL;
				//ostrcatbig(&buf, "&", &maxlen, &pos);
				//ostrcatbig(&buf, node->text, &maxlen, &pos);
				ostrcatbig(&buf, "\");'>", &maxlen, &pos);
			}

			ostrcatbig(&buf, "</td></tr>", &maxlen, &pos);
		}
		else
		{
			buf = ostrcat(buf, olutoa(node->begin), 1, 1);
			buf = ostrcat(buf, "#", 1, 0);
			buf = ostrcat(buf, olutoa(node->end), 1, 1);
			buf = ostrcat(buf, "#", 1, 0);
			buf = ostrcat(buf, oitoa(node->justplay), 1, 1);
			buf = ostrcat(buf, "#", 1, 0);
			buf = ostrcat(buf, oitoa(node->repeate), 1, 1);
			buf = ostrcat(buf, "#", 1, 0);
			buf = ostrcat(buf, node->name, 1, 0);
			buf = ostrcat(buf, "#", 1, 0);
			buf = ostrcat(buf, oitoa(node->serviceid), 1, 1);
			buf = ostrcat(buf, "#", 1, 0);
			buf = ostrcat(buf, olutoa(node->transponderid), 1, 1);
			buf = ostrcat(buf, "#", 1, 0);
			buf = ostrcat(buf, oitoa(node->status), 1, 1);
			buf = ostrcat(buf, "#", 1, 0);
			buf = ostrcat(buf, node->errstr, 1, 0);
			buf = ostrcat(buf, "#", 1, 0);
			buf = ostrcat(buf, node->timestamp, 1, 1);
			buf = ostrcat(buf, "\n", 1, 0);
		}

		node = node->next;
	}

	if(fmt == 0) webcreatetailbig(&buf, &maxlen, &pos, 0);

	free(buf1);
	return buf;
}

char* webaddrectimer(char* param, int fmt)
{
	char* buf = NULL, *buf1 = NULL, *buf2 = NULL, *param1 = NULL, *param2 = NULL;
	struct channel* chnode = NULL;
	struct epg* epgnode = NULL;
//	int maxlen = 0, pos = 0, tmpservicetype = 0;
	int maxlen = 0, pos = 0;
	struct tm* loctime = NULL;
	time_t akttime = time(NULL);
	
	if(param != NULL)
	{
		//create param1 + 2
		param1 = strchr(param, '&');
		if(param1 != NULL)
		{
			*param1++ = '\0';
			param2 = strchr(param1, '&');
			if(param2 != NULL)
				*param2++ = '\0';
		}
		if(param1 == NULL || param2 == NULL) return NULL;
		chnode = getchannel(atoi(param), atoi(param1));
		if(chnode == NULL) return NULL;

		epgnode = getepg(chnode, atoi(param2), 0);
		if(epgnode == NULL) return NULL;
	}

		
	ostrcatbig(&buf, "<html><head><meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\"><link rel=\"stylesheet\" type=\"text/css\" href=\"titan.css\"></head>", &maxlen, &pos);
	ostrcatbig(&buf, "<body class=body ><center>", &maxlen, &pos);
	ostrcatbig(&buf, "<form name=F1 action=query method=get><br><br>", &maxlen, &pos);
	ostrcatbig(&buf, "<input type=\"hidden\" name=\"rectimersend&node\" value=\"", &maxlen, &pos);
	ostrcatbig(&buf, "0", &maxlen, &pos);
	ostrcatbig(&buf, "\">", &maxlen, &pos);
	ostrcatbig(&buf, "<table border=\"0\"><tr>", &maxlen, &pos);
	ostrcatbig(&buf, "<td><font class=label>Name:&nbsp;</font></td>", &maxlen, &pos);
	ostrcatbig(&buf, "<td><input class=inputbox type=\"text\" name=\"name\" value=\"", &maxlen, &pos);
	if(epgnode != NULL)
		ostrcatbig(&buf, epgnode->title, &maxlen, &pos);
	else	
		ostrcatbig(&buf, " ", &maxlen, &pos);
	ostrcatbig(&buf, "\" /></td></tr>", &maxlen, &pos);
	ostrcatbig(&buf, "<td><font class=label>Type:&nbsp;</font></td>", &maxlen, &pos);
	ostrcatbig(&buf, "<td><select name=\"type\" border=0><option selected>", &maxlen, &pos);
	ostrcatbig(&buf, "record", &maxlen, &pos);
	ostrcatbig(&buf, "<option>record<option>switch channel</select></td></tr>", &maxlen, &pos);

	buf2 = malloc(MINMALLOC);
	if(epgnode != NULL)
		loctime =	olocaltime(&epgnode->starttime);
	else
		loctime = olocaltime(&akttime);
	strftime(buf2, MINMALLOC, "%H:%M %d-%m-%Y", loctime);
	free(loctime); loctime = NULL;

	buf1 = ostrcat(buf2, "", 0, 0);
	ostrcatbig(&buf, "<td><font class=label>Begin:&nbsp;</font></td>", &maxlen, &pos);
	ostrcatbig(&buf, "<td><input class=inputbox type=\"text\" name=\"begin\" value=\"", &maxlen, &pos);
	ostrcatbig(&buf, buf1, &maxlen, &pos);
	ostrcatbig(&buf, "\" /></td></tr>", &maxlen, &pos);
	free(buf1); buf1 = NULL;
	
	if(epgnode != NULL)
		loctime =	olocaltime(&epgnode->endtime);
	else
		loctime = olocaltime(&akttime);
	strftime(buf2, MINMALLOC, "%H:%M %d-%m-%Y", loctime);
	free(loctime); loctime = NULL;
	
	buf1 = ostrcat(buf2, "", 0, 0);
	ostrcatbig(&buf, "<td><font class=label>End:&nbsp;</font></td>", &maxlen, &pos);
	ostrcatbig(&buf, "<td><input class=inputbox type=\"text\" name=\"end\" value=\"", &maxlen, &pos);
	ostrcatbig(&buf, buf1, &maxlen, &pos);
	ostrcatbig(&buf, "\" /></td></tr>", &maxlen, &pos);
	free(buf1); buf1 = NULL;
	free(buf2); buf2 = NULL;

	ostrcatbig(&buf, "<td><font class=label>Channel:&nbsp;</font></td>", &maxlen, &pos);
	ostrcatbig(&buf, "<td><input class=inputbox type=\"text\" name=\"channel\" value=\"", &maxlen, &pos);
	if(chnode != NULL)
		ostrcatbig(&buf, chnode->name, &maxlen, &pos);
	else if(status.aktservice->channel != NULL)
		ostrcatbig(&buf, status.aktservice->channel->name, &maxlen, &pos);
	ostrcatbig(&buf, "\" /></td></tr>", &maxlen, &pos);
	free(buf1); buf1 = NULL;
	
	ostrcatbig(&buf, "</table><br><br><input class=button type=submit name=send value=\"Send\" onClick=\"return checkdaytime(begin.value, end.value)\"></input>&nbsp;<input class=button type=reset name=reset value=\"Reset\"></input></form></center></body></html>", &maxlen, &pos);

	//ostrcatbig(&buf, param, &maxlen, &pos);
	return buf;
}

char* webrectimersend(char* param, int fmt)
{
	char* buf = NULL, *string = NULL, *name = NULL, *begin = NULL, *end = NULL, *type = NULL, *anode = NULL, *channelname = NULL, *sid = NULL, *tid = NULL, *ext = NULL;
//	int maxlen = 0, pos = 0, newnode = 0, channelfind = 0;
	int newnode = 0, channelfind = 0;
	struct rectimer *node = NULL;
	char* tmpstr = NULL;
	struct tm* loctime = NULL;
	struct channel *channel1 = NULL;
//	struct service *service1;

	anode = strstr(param, "node=");
	if(anode != NULL)
		anode = anode + 5;
	name = strstr(param, "name=");
	if(name != NULL)
		name = name + 5;
	begin = strstr(param, "begin=");
	if(begin != NULL)
		begin = begin + 6;
	end = strstr(param, "end=");
	if(end != NULL)
		end = end + 4;
	type = strstr(param, "type=");
	if(type != NULL)
		type = type + 5;
	channelname = strstr(param, "channel=");
	if(channelname != NULL)
		channelname = channelname + 8;
	sid = strstr(param, "sid=");
	if(sid != NULL)
		sid = sid + 4;
	tid = strstr(param, "tid=");
	if(tid != NULL)
		tid = tid + 4;
	ext = strstr(param, "ext=");
	if(ext != NULL)
		ext = ext + 4;
	
	string = param;	
	while(string != NULL)
	{	
		string = strchr(string, '&');
		if(string != NULL)
			*string++ = '\0';
	} 
	
	string = channelname;	
	while(string != NULL)
	{	
		string = strchr(string, '+');
		if(string != NULL)
			*string++ = ' ';
	} 

	string = name;	
	while(string != NULL)
	{	
		string = strchr(string, '+');
		if(string != NULL)
			*string++ = ' ';
	} 
	
	if((sid == NULL && tid != NULL) || (sid != NULL && tid == NULL))
	{
		buf = ostrcat(buf, "ERROR: sid and tid required or only channel", 1, 0);	
		return buf;
	}
		
	
	if(channelname != NULL && sid == NULL)
	{
		channelfind = 0;
		channel1 = channel;
		while(channel1 != NULL)
		{
			if(ostrcmp(channel1->name, channelname) == 0 && channel1->servicetype == 0) {
				if(channelnottunable(channel1) == 0)
				{
					channelfind = 1;
					break;
				}
			}
			channel1 = channel1->next;
		}
		if(channelfind == 0)
		{
			channel1 = channel;		
			while(channel1 != NULL)
			{
				if(strstr(channel1->name, channelname) != NULL && channel1->servicetype == 0)
				{
					if(channelnottunable(channel1) == 0)
					{
						channelfind = 1;
						break;
					}
				}
				channel1 = channel1->next;
			}
		}
		if(channelfind == 0)
		{
			buf = ostrcat(buf, "ERROR: channel not found", 1, 0);	
			return buf;
		}
	}
	
	newnode = 0;
	node = getrectimerbytimestamp(anode);
	if(node == NULL)
	{
		node = addrectimernode(NULL, NULL);
		if(node != NULL)
		{
			newnode = 1;
			node->pincode = ostrcat("0000", NULL, 0, 0);
			node->recpath = ostrcat(NULL, getconfig("rec_path", NULL), 0, 0);
			node->afterevent = 0;
			node->repeate = 0;
		}
	}
	
	if(node != NULL)
	{
		if(channelfind == 1 && channel1 != NULL)
		{
			node->serviceid = channel1->serviceid;
			node->servicetype = channel1->servicetype;
			node->transponderid = channel1->transponderid;
		}
	
		if(sid != NULL && tid != NULL)
		{
			node->serviceid = atoi(sid);
			node->transponderid = atol(tid);
			node->servicetype = 0;
		}
	
		free(node->name); node->name = NULL;
		node->name = ostrcat(name, "", 0, 0);
	
		if(ostrcmp(type, "record") == 0)
			node->justplay = 0;
		else
			node->justplay = 1;
	
		loctime = olocaltime(&node->begin);
		tmpstr = strptime(begin, "%H:%M+%d-%m-%Y", loctime); 
		if(tmpstr != NULL) {
			loctime->tm_isdst = -1;
			node->begin = mktime(loctime);
		}
		node->begin -= (node->begin % 60);
		tmpstr = NULL;
		free(loctime); loctime = NULL;

		loctime = olocaltime(&node->end);
		tmpstr = strptime(end, "%H:%M+%d-%m-%Y", loctime); 
		if(tmpstr != NULL) {
			loctime->tm_isdst = -1;
			node->end = mktime(loctime);
		}
		node->end -= (node->end % 60);
		tmpstr = NULL;
		free(loctime); loctime = NULL;

		if(newnode == 1)
			node->disabled = 0;
	
		status.writerectimer = 1;
		writerectimer(getconfig("rectimerfile", NULL), 0);
		
		if(ext == NULL)
			buf = webgetrectimer(NULL, 0, fmt);
		else
		{
			buf = ostrcat(buf, "ok -> TimerID=", 1, 0);
			buf = ostrcat(buf, node->timestamp, 1, 0);
		}
	}
	else
		buf = ostrcat(buf, "Timer not ok", 1, 0);

	return buf;
}

char* webeditrectimer(char* param, int fmt)
{
	char* buf = NULL, *buf1 = NULL, *buf2 = NULL;
	int maxlen = 0, pos = 0;
	struct rectimer *node = NULL;
	struct tm* loctime = NULL;

	node = getrectimerbytimestamp(param);
	if(node == NULL) return NULL;
		
	ostrcatbig(&buf, "<html><head><meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\"><link rel=\"stylesheet\" type=\"text/css\" href=\"titan.css\"></head>", &maxlen, &pos);
	ostrcatbig(&buf, "<body class=body ><center>", &maxlen, &pos);
	ostrcatbig(&buf, "<form name=F1 action=query method=get><br><br>", &maxlen, &pos);
	ostrcatbig(&buf, "<input type=\"hidden\" name=\"rectimersend&node\" value=\"", &maxlen, &pos);
	ostrcatbig(&buf, node->timestamp, &maxlen, &pos);
	ostrcatbig(&buf, "\">", &maxlen, &pos);
	ostrcatbig(&buf, "<table border=\"0\"><tr>", &maxlen, &pos);
	ostrcatbig(&buf, "<td><font class=label>Name:&nbsp;</font></td>", &maxlen, &pos);
	ostrcatbig(&buf, "<td><input class=inputbox type=\"text\" name=\"name\" value=\"", &maxlen, &pos);
	ostrcatbig(&buf, node->name, &maxlen, &pos);
	ostrcatbig(&buf, "\" /></td></tr>", &maxlen, &pos);

	ostrcatbig(&buf, "<td><font class=label>Type:&nbsp;</font></td>", &maxlen, &pos);
	ostrcatbig(&buf, "<td><select name=\"type\" border=0><option selected>", &maxlen, &pos);
	if( node->justplay == 0 )
		ostrcatbig(&buf, "record", &maxlen, &pos);
	else
		ostrcatbig(&buf, "switch channel", &maxlen, &pos);
	ostrcatbig(&buf, "<option>record<option>switch channel</select></td></tr>", &maxlen, &pos);
	
	/*ostrcatbig(&buf, "<td><font class=label>Repeate:&nbsp;</font></td>", &maxlen, &pos);
	ostrcatbig(&buf, "<td><select name=\"repeate\" border=0><option selected>", &maxlen, &pos);
	if( node->repeate == 0 )
		ostrcatbig(&buf, "once", &maxlen, &pos);
	else
		ostrcatbig(&buf, "repeate", &maxlen, &pos);
	ostrcatbig(&buf, "<option>once<option>repeate</select></td></tr>", &maxlen, &pos);	
	*/
	
	/*ostrcatbig(&buf, "<td><font class=label>Repeate type:&nbsp;</font></td>", &maxlen, &pos);
	ostrcatbig(&buf, "<td><select name=\"repeatetype\" border=0><option selected>", &maxlen, &pos);
	if( node->repeate == 0 )
		ostrcatbig(&buf, "daily", &maxlen, &pos);
	else if( node->repeate == 1 )
		ostrcatbig(&buf, "weekly", &maxlen, &pos);
	else if( node->repeate == 2 )
		ostrcatbig(&buf, "workdays", &maxlen, &pos);
	else
		ostrcatbig(&buf, "user defined", &maxlen, &pos);
	ostrcatbig(&buf, "<option>daily<option>weekly<option>workdays<option>user defined</select></td></tr>", &maxlen, &pos);	
	*/
		
	buf2 = malloc(MINMALLOC);
	loctime = olocaltime(&node->begin);
	strftime(buf2, MINMALLOC, "%H:%M %d-%m-%Y", loctime);
	free(loctime); loctime = NULL;

	buf1 = ostrcat(buf2, "", 0, 0);
	ostrcatbig(&buf, "<td><font class=label>Begin:&nbsp;</font></td>", &maxlen, &pos);
	ostrcatbig(&buf, "<td><input class=inputbox type=\"text\" name=\"begin\" value=\"", &maxlen, &pos);
	ostrcatbig(&buf, buf1, &maxlen, &pos);
	ostrcatbig(&buf, "\" /></td></tr>", &maxlen, &pos);
	free(buf1); buf1 = NULL;
	
	loctime = olocaltime(&node->end);
	strftime(buf2, MINMALLOC, "%H:%M %d-%m-%Y", loctime);
	free(loctime); loctime = NULL;

	buf1 = ostrcat(buf2, "", 0, 0);
	ostrcatbig(&buf, "<td><font class=label>End:&nbsp;</font></td>", &maxlen, &pos);
	ostrcatbig(&buf, "<td><input class=inputbox type=\"text\" name=\"end\" value=\"", &maxlen, &pos);
	ostrcatbig(&buf, buf1, &maxlen, &pos);
	ostrcatbig(&buf, "\" /></td></tr>", &maxlen, &pos);
	free(buf1); buf1 = NULL;
	free(buf2); buf2 = NULL;
	
	ostrcatbig(&buf, "</table><br><br><input class=button type=submit name=send value=\"Send\" onClick=\"return checkdaytime(begin.value, end.value)\"></input>&nbsp;<input class=button type=reset name=reset value=\"Reset\"></input></form></center></body></html>", &maxlen, &pos);
	
	//ostrcatbig(&buf, param, &maxlen, &pos);
	return buf;
}

char* webdelrectimer(char* param, int fmt)
{
	char* buf = NULL, *string = NULL, *timerid = NULL;
	struct rectimer *node = NULL;
	int ext = 0;
	
	timerid = strstr(param, "timerid=");
	if(timerid != NULL)
	{
		timerid = timerid + 8;
		ext = 1;
	}

	node = NULL;
	if(ext == 1)
	{
		string = param;	
		while(string != NULL)
		{	
			string = strchr(string, '&');
			if(string != NULL)
				*string++ = '\0';
		}
		node = getrectimerbytimestamp(timerid);
	}
	else
		node = getrectimerbytimestamp(param);

	if(node == NULL)
	{
		buf = ostrcat(buf, "ERROR: timer not found", 1, 0);	
		return buf;
	}
	
	delrectimer(node, 1, 0);
	
	if(ext == 1) 
		buf = ostrcat(buf, "ok -> timer deleted", 1, 0);
	else
		buf = webgetrectimer(NULL, 0, fmt);
	
	return buf;
}

void putxmessage(struct stimerthread* timernode, char* captiontime, char* body)
{
	struct splitstr* ret1 = NULL;
	char* caption = NULL;
	int count1 = 0;
	int timeout = 5;

	ret1 = strsplit(captiontime, "\t", &count1);

	if(count1 >= 2)
	{
		caption = ostrcat(caption, (&ret1[0])->part, 1, 0);
		if((&ret1[1])->part != NULL)
			timeout = atoi((&ret1[1])->part);
		textbox(caption, body, _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 280, timeout, 0);
	}

	free(caption); free(body); free(captiontime); free(ret1);
}

void xmessage(char* filename) 
{
	char* param=NULL, *param1 = NULL, *param2 = NULL, *param3 = NULL, *param4 = NULL;
	char* caption=NULL, *body=NULL;
	char* timeout=NULL;

	param4 = strstr(filename, "icon=");
	param3 = strstr(filename, "charset=");
	param2 = strstr(filename, "timeout=");
	param1 = strstr(filename, "caption=");
	param = strstr(filename, "body=");

	if(param4 != NULL)
	{
		param4 = param4 - 1;
		*param4 = '\0';
		param4 = param4 + 5;
		*param4++ = '\0';
	}
	if(param3 != NULL)
	{
		param3 = param3 - 1;
		*param3 = '\0';
		param3 = param3 + 8;
		*param3++ = '\0';
	}
	if(param2 != NULL)
	{
		param2 = param2 - 1;
		*param2 = '\0';
		param2 = param2 + 8;
		*param2++ = '\0';
	}
	if(param1 != NULL)
	{
		param1 = param1 - 1;
		*param1 = '\0';
		param1 = param1 + 8;
		*param1++ = '\0';
	}
	if(param != NULL)
	{
		param = param - 1;
		*param = '\0';
		param = param + 5;
		*param++ = '\0';
		body = ostrcat(body, param, 1, 0);
	}
	else
		body = ostrcat(body, " ", 1, 0);
		
	if(param2 != NULL)
		timeout = ostrcat(timeout, param2, 1, 0);
	else
		timeout = ostrcat(timeout, "5", 1, 0);
		
	if(param1 != NULL)
		caption = ostrcat(caption, param1, 1, 0);
	else
		caption = ostrcat(caption, "XMESSAGE", 1, 0);
	
	caption = ostrcat(caption, "\t", 1, 0);
	caption = ostrcat(caption, timeout, 1, 1);
		
	addtimer(&putxmessage, START, 1000, 1, (void*)caption, (void*)body, NULL);
	return;
}

char* webgetvol(int fmt)
{
	char* buf = NULL;

	buf = oitoa(getvol());
	return buf;
}

char* webgetmute(int fmt)
{
	char* buf = NULL;

	buf = oitoa(status.mute);
	return buf;
}

char* webgetrccodes(int fmt)
{
	int i = 0;
	char* buf = NULL;
	struct clist **clist = rcconfig;
	struct clist* node = NULL;

	for(i = 0; i < LISTHASHSIZE; i++)
	{
		node = clist[i];

		while(node != NULL)
		{
			buf = ostrcat(buf, node->key, 1, 0);
			buf = ostrcat(buf, "#", 1, 0);
			buf = ostrcat(buf, node->value, 1, 0);
			buf = ostrcat(buf, "\n", 1, 0);
			node = node->next;
		}
	}

	return buf;
}

#endif
