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
	transponderid = ollutoa(chnode->transponderid);

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
		{
			*buf = ostrcat(*buf, oitoa(epgnode->eventid), 1, 1);
			*buf = ostrcat(*buf, "#", 1, 0);
			*buf = ostrcat(*buf, olutoa(epgnode->starttime), 1, 1);
			*buf = ostrcat(*buf, "#", 1, 0);
			*buf = ostrcat(*buf, epgnode->title, 1, 0);
			*buf = ostrcat(*buf, "#", 1, 0);
			if(epgnode->next != NULL)
			{
				*buf = ostrcat(*buf, olutoa(epgnode->next->starttime), 1, 1);
				*buf = ostrcat(*buf, "#", 1, 0);
				*buf = ostrcat(*buf, olutoa(epgnode->next->endtime), 1, 1);
				*buf = ostrcat(*buf, "#", 1, 0);
				*buf = ostrcat(*buf, epgnode->next->title, 1, 0);
				*buf = ostrcat(*buf, "#", 1, 0);
				*buf = ostrcat(*buf, oitoa(epgnode->next->eventid), 1, 1);
			}
			else
			{
				*buf = ostrcat(*buf, olutoa(epgnode->endtime), 1, 1);
				*buf = ostrcat(*buf, "#", 1, 0);
				*buf = ostrcat(*buf, "#", 1, 0);
				*buf = ostrcat(*buf, "#", 1, 0);
				*buf = ostrcat(*buf, "0", 1, 0);
			}
		}
		else
		{
			*buf = ostrcat(*buf, "0", 1, 0);
			*buf = ostrcat(*buf, "#", 1, 0);
			*buf = ostrcat(*buf, "#", 1, 0);
			*buf = ostrcat(*buf, "#", 1, 0);
			*buf = ostrcat(*buf, "#", 1, 0);
			*buf = ostrcat(*buf, "#", 1, 0);
			*buf = ostrcat(*buf, "#", 1, 0);
			*buf = ostrcat(*buf, "0", 1, 0);
		}
		*buf = ostrcat(*buf, "#", 1, 0);

		if(ret == 1)
			*buf = ostrcat(*buf, "1", 1, 0);
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
		{
			ostrcatbig(buf, "</td><td width=100 align=right valign=middle nowrap><img style=\"margin-left: 5\" border=0 src=img/tv.png title=\"", maxlen, pos);
			ostrcatbig(buf, _("TV"), maxlen, pos);
			ostrcatbig(buf, "\" width=16 height=16>", maxlen, pos);
		}
		else
		{
			ostrcatbig(buf, "</td><td width=100 align=right valign=middle nowrap><img style=\"margin-left: 5\" border=0 src=img/radio.png title=\"", maxlen, pos);
			ostrcatbig(buf, _("Radio"), maxlen, pos);
			ostrcatbig(buf, "\" width=16 height=16>", maxlen, pos);
		}

		//single epg
		ostrcatbig(buf, "<a target=main href=query?getsingleepg&", maxlen, pos);
		ostrcatbig(buf, serviceid, maxlen, pos);
		ostrcatbig(buf, "&", maxlen, pos);
		ostrcatbig(buf, transponderid, maxlen, pos);
		ostrcatbig(buf, "><img style=\"margin-left: 5\" border=0 src=img/singleepg.png title=\"", maxlen, pos);
		ostrcatbig(buf, _("Single EPG"), maxlen, pos);
		ostrcatbig(buf, "\" width=16 height=16></a>", maxlen, pos);

		//m3u stream, transcode stream + webstream
		if(ret == 0)
		{
			ostrcatbig(buf, "<a target=nothing href=query?getm3u&", maxlen, pos);
			ostrcatbig(buf, serviceid, maxlen, pos);
			ostrcatbig(buf, ",", maxlen, pos);
			ostrcatbig(buf, transponderid, maxlen, pos);
			ostrcatbig(buf, "><img style=\"margin-left: 5\" border=0 src=img/stream.png title=\"", maxlen, pos);
			ostrcatbig(buf, _("Stream"), maxlen, pos);
			ostrcatbig(buf, "\" width=16 height=16></a>", maxlen, pos);

#ifdef MIPSEL
			if(file_exist("/proc/stb/encoder") &&  getconfigint("web_trans_transcode", NULL) == 1)
			{
				ostrcatbig(buf, "<a target=nothing href=query?gettranscodem3u&", maxlen, pos);
				ostrcatbig(buf, serviceid, maxlen, pos);
				ostrcatbig(buf, ",", maxlen, pos);
				ostrcatbig(buf, transponderid, maxlen, pos);
				ostrcatbig(buf, "><img style=\"margin-left: 5\" border=0 src=img/transstream.png title=\"", maxlen, pos);
				ostrcatbig(buf, _("Stream Transcode"), maxlen, pos);
				ostrcatbig(buf, "\" width=16 height=16></a>", maxlen, pos);
			}
#endif
			ostrcatbig(buf, "<a target=_blank href=query?getvideo&", maxlen, pos);
			ostrcatbig(buf, serviceid, maxlen, pos);
			ostrcatbig(buf, ",", maxlen, pos);
			ostrcatbig(buf, transponderid, maxlen, pos);
			ostrcatbig(buf, "><img style=\"margin-left: 5\" border=0 src=img/webstream.png title=\"", maxlen, pos);
			ostrcatbig(buf, _("WebStream"), maxlen, pos);
			ostrcatbig(buf, "\" width=16 height=16></a>", maxlen, pos);
		}
		else
		{
			ostrcatbig(buf, "<img style=\"margin-left: 5\" border=0 src=img/cross.png title=\"", maxlen, pos);
			ostrcatbig(buf, _("Channel not available"), maxlen, pos);
			ostrcatbig(buf, "\" width=16 height=16>", maxlen, pos);
		}
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

	if(fmt == 0) webcreateheadbig(&buf, &maxlen, "<meta http-equiv=refresh content=60>", &pos, 0);

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

	if(fmt == 0) webcreateheadbig(&buf, &maxlen, "<meta http-equiv=refresh content=60>", &pos, 0);

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
			{
				ostrcatbig(&buf, "img/tv.png title=\"", &maxlen, &pos);
				ostrcatbig(&buf, _("TV"), &maxlen, &pos);
			}
			else
			{
				ostrcatbig(&buf, "img/radio.png title=\"", &maxlen, &pos);
				ostrcatbig(&buf, _("Radio"), &maxlen, &pos);
			}

			ostrcatbig(&buf, "\" width=16 height=16>", &maxlen, &pos);

			ostrcatbig(&buf, "<a href=\"query?getgmultiepg&", &maxlen, &pos);
			ostrcatbig(&buf, node->name, &maxlen, &pos);
			ostrcatbig(&buf, "\">", &maxlen, &pos);
			ostrcatbig(&buf, "<img style=\"margin-left: 5\" border=0 src=img/gmultiepg.png title=\"", &maxlen, &pos);
			ostrcatbig(&buf, _("GRAPHIC MULTI EPG - Bouquets"), &maxlen, &pos);
			ostrcatbig(&buf, "\" width=16 height=16></a>", &maxlen, &pos);

			ostrcatbig(&buf, "<a href=\"query?getbouquetm3u&", &maxlen, &pos);
			ostrcatbig(&buf, node->name, &maxlen, &pos);
			ostrcatbig(&buf, "\">", &maxlen, &pos);
			ostrcatbig(&buf, "<img style=\"margin-left: 5\" border=0 src=img/bouquetstream.png title=\"", &maxlen, &pos);
			ostrcatbig(&buf, _("Playlist download"), &maxlen, &pos);
			ostrcatbig(&buf, "\" width=16 height=16></a>", &maxlen, &pos);

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
		if((vol >=10) && (status.mute == 0))
			buf = ostrcat(buf, "<a href=query?setvol&10><img src=img/led_on.png border=0></img></a> ", 1, 0);
		else
			buf = ostrcat(buf, "<a href=query?setvol&10><img src=img/led_off.png border=0></img></a> ", 1, 0);
		if((vol >=20) && (status.mute == 0))
			buf = ostrcat(buf, "<a href=query?setvol&20><img src=img/led_on.png border=0></img></a> ", 1, 0);
		else
			buf = ostrcat(buf, "<a href=query?setvol&20><img src=img/led_off.png border=0></img></a> ", 1, 0);
		if((vol >=30) && (status.mute == 0))
			buf = ostrcat(buf, "<a href=query?setvol&30><img src=img/led_on.png border=0></img></a> ", 1, 0);
		else
			buf = ostrcat(buf, "<a href=query?setvol&30><img src=img/led_off.png border=0></img></a> ", 1, 0);
		if((vol >=40) && (status.mute == 0))
			buf = ostrcat(buf, "<a href=query?setvol&40><img src=img/led_on.png border=0></img></a> ", 1, 0);
		else
			buf = ostrcat(buf, "<a href=query?setvol&40><img src=img/led_off.png border=0></img></a> ", 1, 0);
		if((vol >=50) && (status.mute == 0))
			buf = ostrcat(buf, "<a href=query?setvol&50><img src=img/led_on.png border=0></img></a> ", 1, 0);
		else
			buf = ostrcat(buf, "<a href=query?setvol&50><img src=img/led_off.png border=0></img></a> ", 1, 0);
		if((vol >=60) && (status.mute == 0))
			buf = ostrcat(buf, "<a href=query?setvol&60><img src=img/led_on.png border=0></img></a> ", 1, 0);
		else
			buf = ostrcat(buf, "<a href=query?setvol&60><img src=img/led_off.png border=0></img></a> ", 1, 0);
		if((vol >=70) && (status.mute == 0))
			buf = ostrcat(buf, "<a href=query?setvol&70><img src=img/led_on.png border=0></img></a> ", 1, 0);
		else
			buf = ostrcat(buf, "<a href=query?setvol&70><img src=img/led_off.png border=0></img></a> ", 1, 0);
		if((vol >=80) && (status.mute == 0))
			buf = ostrcat(buf, "<a href=query?setvol&80><img src=img/led_on.png border=0></img></a> ", 1, 0);
		else
			buf = ostrcat(buf, "<a href=query?setvol&80><img src=img/led_off.png border=0></img></a> ", 1, 0);
		if((vol >=90) && (status.mute == 0))
			buf = ostrcat(buf, "<a href=query?setvol&90><img src=img/led_on.png border=0></img></a> ", 1, 0);
		else
			buf = ostrcat(buf, "<a href=query?setvol&90><img src=img/led_off.png border=0></img></a> ", 1, 0);
		if((vol >=100) && (status.mute == 0))
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

char* webgetservice(char* param, int fmt)
{
	char* buf = NULL, *param1 = NULL;
	struct channel* chnode = NULL;
	int line = 0, maxlen = 0, pos = 0;

	if(param == NULL) return NULL;

	//create param1
	param1 = strchr(param, '&');
	if(param1 != NULL)
		*param1++ = '\0';

	if(param1 == NULL) return NULL;

	chnode = getchannel(atoi(param), strtoull(param1, NULL, 10));

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

	chnode = getchannel(atoi(param), strtoull(param1, NULL, 10));
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
	int extip = 1;
	char* buf = NULL, *ip = NULL, *tmpbuf = NULL;
	struct sockaddr_in sin;
	socklen_t len = sizeof(sin);

#ifdef MIPSEL
	status.webencode = 0;
#endif

	if(param == NULL) return NULL;

	if(getconfigint("webifip", NULL) == 1)
		ip = getispip();

	if(ip == NULL)
	{
		if(getsockname(connfd, &sin, &len) < 0)
		{
			perr("getsockname");
			return NULL;
		}

		extip = 0;
		ip = inet_ntoa(sin.sin_addr);
			if(ip == NULL) return NULL;
	}

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

	if(extip == 1) free(ip);
	return buf;
}

#ifdef MIPSEL
char* webgettranscodem3u(char* param, int connfd, int fmt)
{
	int extip = 1;
	char* buf = NULL, *ip = NULL, *tmpbuf = NULL;
	struct sockaddr_in sin;
	socklen_t len = sizeof(sin);

	status.webencode = 1;

	if(param == NULL) return NULL;

	if(getconfigint("webifip", NULL) == 1)
		ip = getispip();

	if(ip == NULL)
	{
		if(getsockname(connfd, &sin, &len) < 0)
		{
			perr("getsockname");
			return NULL;
		}

		extip = 0;
		ip = inet_ntoa(sin.sin_addr);
			if(ip == NULL) return NULL;
	}

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

	if(extip == 1) free(ip);
	return buf;
}
#endif

char* webgetbouquetm3u(char* param, int connfd, int fmt)
{
	int extip = 1;
	char* buf = NULL, *ip = NULL, *streamport = NULL;
	struct sockaddr_in sin;
	struct mainbouquet *mbouquet = NULL;
	struct bouquet *node = NULL;
	struct channel* chnode = NULL;
	socklen_t len = sizeof(sin);

	if(param == NULL) return NULL;

	if(getconfigint("webifip", NULL) == 1)
		ip = getispip();

	if(ip == NULL)
	{
		if(getsockname(connfd, &sin, &len) < 0)
		{
			perr("getsockname");
			return NULL;
		}

		extip = 0;
		ip = inet_ntoa(sin.sin_addr);
			if(ip == NULL) return NULL;
	}

	streamport = getconfig("streamport", NULL);

	buf = ostrcat(buf, "#EXTM3U\n", 1, 0);
	buf = ostrcat(buf, "#EXTVLCOPT--http-reconnect=true\n", 1, 0);

	mbouquet = getmainbouquet(param);
	if(mbouquet != NULL)
	{
		node = mbouquet->bouquet;
		while(node != NULL)
		{
			chnode = getchannel(node->serviceid, node->transponderid);
			if(chnode != NULL)
			{
				buf = ostrcat(buf, "#EXTINF:-1,", 1, 0);
				buf = ostrcat(buf, chnode->name, 1, 0);
				buf = ostrcat(buf, "\n", 1, 0);

				buf = ostrcat(buf, "http://", 1, 0);
				buf = ostrcat(buf, ip, 1, 0);
				buf = ostrcat(buf, ":", 1, 0);
				buf = ostrcat(buf, streamport, 1, 0);
				buf = ostrcat(buf, "/", 1, 0);
				buf = ostrcat(buf, oitoa(chnode->serviceid), 1, 1);
				buf = ostrcat(buf, "%2c", 1, 0);
				buf = ostrcat(buf, ollutoa(chnode->transponderid), 1, 1);
				buf = ostrcat(buf, "\n", 1, 0);
			}
			node = node->next;
		}
	}
	if(extip == 1) free(ip);
	return buf;
}

char* webgetvideo(char* param, int connfd, int fmt)
{
	int extip = 1;
	char* buf = NULL, *ip = NULL, *tmpbuf = NULL;
	struct sockaddr_in sin;
	socklen_t len = sizeof(sin);

	if(param == NULL) return NULL;

	if(getconfigint("webifip", NULL) == 1)
		ip = getispip();

	if(ip == NULL)
	{
		if(getsockname(connfd, &sin, &len) < 0)
		{
			perr("getsockname");
			return NULL;
		}

		extip = 0;
		ip = inet_ntoa(sin.sin_addr);
		if(ip == NULL) return NULL;
	}

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

	if(extip == 1) free(ip);
	return buf;
}

char* webvideo(char* param, int fmt)
{
	char* buf = NULL, *param1 = NULL, *tmpbuf = NULL;

	if(param == NULL) return NULL;

	if(ostrcmp("status", param) == 0) // status is only available for queryraw
	{
		char buf[30];
		int active = (status.webplayfile != NULL ? 1 : 0);
		int st = 0; // status, 0: paused, 1: playing
		unsigned long pos = 0; // current position
		unsigned long len = 0; // stream length

		if(status.play == 1)
			st = 1;

		if(active == 1)
		{
			pos = playergetpts() / 90000;
			len = playergetlength();
		}

		snprintf(buf, 30, "%d#%d#%lu#%lu", active, st, pos, len);

		return ostrcat(buf, NULL, 0, 0);
	}

	//create param1
	param1 = strchr(param, '&');
	if(param1 != NULL)
		*param1++ = '\0';

	if(param1 == NULL) return NULL;

	htmldecode(param1, param1);
	if(param1 != NULL)
	{
		if(status.play == 0 && status.webplayfile == NULL)
		{
			if(ostrstr(param1, "url=") == param1)
				status.webplayfile = ostrcat(param1 + 4, NULL, 0, 0);
			else if(ostrstr(param1, "hosterurl=") == param1)
			{
				status.webplayfile = ostrcat(param1 + 10, NULL, 0, 0);

				// stream over tithek enable start
				printf("status.webplayfile: %s\n",status.webplayfile);
				struct skin* tithekplugin = getplugin("Titan Mediathek");
				if(tithekplugin != NULL)
				{
					struct tithek* (*startplugin)(char*);

					startplugin = dlsym(tithekplugin->pluginhandle, "hoster");
					if(startplugin != NULL)
					{
						status.webplayfile = (char*)startplugin(status.webplayfile);
						printf("status.webplayfile changed: %s\n",status.webplayfile);
					}
				}
				// stream over tithek enable end
			}
			else
				status.webplayfile = ostrcat(param1, NULL, 0, 0);
		}
	}
	tmpbuf = ostrcat("not in play mode", NULL, 0, 0);

	int count = 0;
	if(status.timeshift == 0 && status.play == 0 && (ostrcmp("play", param) == 0 || ostrcmp("play=", param) == 0))
	{
		int count = 0;

		int rcret = getrcconfigint("rcwebplay", NULL);
		if(rcret == 0) rcret = getrcconfigint("rcarchive", NULL);
		if(rcret == 0) rcret = getrcconfigint("rcplay", NULL);
		writerc(rcret);
		while(status.play == 0 && count < 30)
		{
			usleep(100000);
			count++;
		}
	}
	else
		count = 31;

	if(count >= 30 && status.play == 0)
	{
		free(status.webplayfile); status.webplayfile = NULL;
		free(tmpbuf); tmpbuf = NULL;
		tmpbuf = ostrcat("can not start playback", NULL, 0, 0);
	}

	if(status.timeshift == 0 && status.play == 1)
	{
		if(ostrcmp("stop", param) == 0 || ostrcmp("stop=", param) == 0)
			writerc(getrcconfigint("rcstop", NULL));

		if(ostrcmp("pause", param) == 0 || ostrcmp("pause=", param) == 0)
			writerc(getrcconfigint("rcpause", NULL));

		if(ostrcmp("ff", param) == 0 || ostrcmp("ff=", param) == 0)
			writerc(getrcconfigint("rcff", NULL));

		if(ostrcmp("fr", param) == 0 || ostrcmp("fr=", param) == 0)
			writerc(getrcconfigint("rcfr", NULL));

		free(tmpbuf); tmpbuf = NULL;
		tmpbuf = ostrcat(param, NULL, 0, 0);

		if(ostrcmp("getlen", param) == 0 || ostrcmp("getlen=", param) == 0)
		{
			unsigned long len = 0;
			free(tmpbuf); tmpbuf = NULL;

			len = playergetlength();
			tmpbuf = ostrcat(buf, olutoa(len), 1, 1);
		}

		if(ostrcmp("getpos", param) == 0 || ostrcmp("getpos=", param) == 0)
		{
			unsigned long pos = 0;
			free(tmpbuf); tmpbuf = NULL;

			pos = playergetpts() / 90000;
			tmpbuf = ostrcat(buf, olutoa(pos), 1, 1);
		}

		if(ostrcmp("getisplaying", param) == 0 || ostrcmp("getisplaying=", param) == 0)
		{
			int playing = 0;
			free(tmpbuf); tmpbuf = NULL;

			playing = playerisplaying();
			tmpbuf = ostrcat(buf, oitoa(playing), 1, 1);
		}

		if(ostrcmp("getplayercan", param) == 0 || ostrcmp("getplayercan=", param) == 0)
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

	chnode = getchannel(atoi(param), strtoull(param1, NULL, 10));
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

	if(fmt == 0)
	{
		buf = ostrcat(buf, "<tr class=channelname><td><h4 style=\"margin:2px 0px\">", 1, 0);
		buf = ostrcat(buf, chnode->name, 1, 0);
		buf = ostrcat(buf, "</h4></td></tr>", 1, 0);
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
	int line = 0, maxlen = 0, pos = 0, longdesc = 1;
	char* buf = NULL, *buf1 = NULL, *buf2 = NULL, *param1 = NULL, *param2 = NULL, *tmpstr = NULL;
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

	if(param1 == NULL) return NULL;
	if(param2 != NULL) longdesc = atoi(param2);

	chnode = getchannel(atoi(param), strtoull(param1, NULL, 10));
	if(chnode == NULL) return NULL;
	epgnode = getepgakt(chnode);;

	if(fmt == 0) webcreateheadbig(&buf, &maxlen, NULL, &pos, 0);

	buf1 = malloc(MINMALLOC);
	if(buf1 == NULL)
	{
		err("no mem");
		return buf;
	}

	if(fmt == 0)
	{
		ostrcatbig(&buf, "<tr class=channelname><td colspan=4><h4 style=\"margin:2px 0px\">", &maxlen, &pos);
		ostrcatbig(&buf, chnode->name, &maxlen, &pos);
		ostrcatbig(&buf, "</h4></td></tr>", &maxlen, &pos);
		ostrcatbig(&buf, "<tr class=tabledesc><td width=70px align=center>Datum</td><td width=70px align=center>Uhrzeit</td><td align=left>Beschreibung</td><td width=50px align=center>Timer</td></tr>", &maxlen, &pos);
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

			loctime = olocaltime(&epgnode->starttime);
			if(loctime != NULL)
				strftime(buf1, MINMALLOC, "%d.%m", loctime);
			ostrcatbig(&buf, "<td nowrap class=coldate>", &maxlen, &pos);
			ostrcatbig(&buf, buf1, &maxlen, &pos);
			if(loctime != NULL)
				strftime(buf1, MINMALLOC, "%H.%M", loctime);
			ostrcatbig(&buf, "</td><td nowrap class=coltime>", &maxlen, &pos);
			ostrcatbig(&buf, buf1, &maxlen, &pos);
			free(loctime); loctime = NULL;

			ostrcatbig(&buf, "</td><td nowrap><a target=main class=link href=query?getepg&", &maxlen, &pos);
			tmpstr = oitoa(chnode->serviceid);
			ostrcatbig(&buf, tmpstr, &maxlen, &pos);
			buf2 = ostrcat("<td nowrap class=linkimage><a target=main class=link href=query?addrectimer&", tmpstr, 0, 0);
			buf2 = ostrcat(buf2, "&", 0, 0);
			free(tmpstr); tmpstr = NULL;
			ostrcatbig(&buf, "&", &maxlen, &pos);
			tmpstr = ollutoa(chnode->transponderid);
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
			buf = ostrcat(buf, "#", 1, 0);
			if(longdesc == 1)
			{
				tmpstr = epgdescunzip(epgnode);
				if(tmpstr != NULL)
					buf = ostrcat(buf, tmpstr, 1, 0);
				free(tmpstr); tmpstr = NULL;
			}
			buf = ostrcat(buf, "#", 1, 0);
			buf = ostrcat(buf, oitoa(epgnode->eventid), 1, 1);
			buf = ostrcat(buf, "\n", 1, 0);
		}

		epgnode = epgnode->next;
		free(buf2); buf2 = NULL;
	}

	if(fmt == 0)
		webcreatetailbig(&buf, &maxlen, &pos, 0);
	else if(buf == NULL)
		buf = ostrcat("no data", NULL, 0, 0);

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

char* webgetdrawcount(char* param, int fmt)
{
	return oitoa(status.drawscreencount);
}

void webgetshoot(char* param, int fmt)
{
	printf("param: %s\n",param);
	printf("parm: %d\n",fmt);
	screenshoot(atoi(param));
}

char* webgetepgsearch(char* query, char* param, int fmt)
{
	int line = 0, maxlen = 0, pos = 0, maxcount = 0, page = 1, newchannel = 0, longepg = 0;
	char* buf = NULL, *buf1 = NULL, *buf2 = NULL, *tmpstr = NULL, *tmpnr = NULL, *param1 = NULL, *param2 = NULL;
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
		//create param1 + 2
		param1 = strchr(param, '&');
		if(param1 != NULL)
		{
			*param1++ = '\0';
			param2 = strchr(param1, '&');
			if(param2 != NULL)
				*param2++ = '\0';
		}
	}

	if(param1 != NULL) page = atoi(param1);
	if(param2 != NULL) longepg = atoi(param2);

	buf1 = malloc(MINMALLOC);
	if(buf1 == NULL)
	{
		err("no mem");
		return buf;
	}

	if(fmt == 0) webcreateheadbig(&buf, &maxlen, NULL, &pos, 0);
	while(chnode != NULL)
	{
		newchannel = 1;
		epgnode = getepgakt(chnode);
		while(epgnode != NULL)
		{
			if(epgnode->title != NULL && ostrstrcase(epgnode->title, param) != NULL)
			{

				maxcount++;

				if(page >= 0)
				{
					if(maxcount <= (MAXHTMLLINE * page) - MAXHTMLLINE || maxcount > MAXHTMLLINE * page)
					{
						epgnode = epgnode->next;
						continue;
					}
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

				if(fmt == 1 && newchannel == 1)
				{
					newchannel = 0;
					ostrcatbig(&buf, "BeginNewChannel", &maxlen, &pos);
					ostrcatbig(&buf, "#", &maxlen, &pos);
					ostrcatbig(&buf, chnode->name, &maxlen, &pos);
					ostrcatbig(&buf, "#", &maxlen, &pos);
					tmpstr = oitoa(chnode->serviceid);
					ostrcatbig(&buf, tmpstr, &maxlen, &pos);
					free(tmpstr); tmpstr = NULL;
					ostrcatbig(&buf, "#", &maxlen, &pos);
					tmpstr = ollutoa(chnode->transponderid);
					ostrcatbig(&buf, tmpstr, &maxlen, &pos);
					free(tmpstr); tmpstr = NULL;
					ostrcatbig(&buf, "#", &maxlen, &pos);
					tmpstr = oitoa(chnode->servicetype);
					ostrcatbig(&buf, tmpstr, &maxlen, &pos);
					free(tmpstr); tmpstr = NULL;
					ostrcatbig(&buf, "\n", &maxlen, &pos);
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
					tmpstr = ollutoa(chnode->transponderid);
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
					if(chnode->name != NULL)
					{
						ostrcatbig(&buf, " (", &maxlen, &pos);
						ostrcatbig(&buf, chnode->name, &maxlen, &pos);
						ostrcatbig(&buf, ")", &maxlen, &pos);
					}

					ostrcatbig(&buf, "</a><br><font class=smalllabel1>", &maxlen, &pos);
					ostrcatbig(&buf, epgnode->subtitle, &maxlen, &pos);

					ostrcatbig(&buf, "</font></td>", &maxlen, &pos);
					ostrcatbig(&buf, buf2, &maxlen, &pos);
					ostrcatbig(&buf, "<img border=0 width=16 height=16 src=img/timer.png alt=\"set timer\"/>", &maxlen, &pos);
					ostrcatbig(&buf, "</a></td></tr>", &maxlen, &pos);
				}
				else
				{
					ostrcatbig(&buf, epgnode->title, &maxlen, &pos);
					ostrcatbig(&buf, "#", &maxlen, &pos);

					tmpstr = olutoa(epgnode->starttime);
					ostrcatbig(&buf, tmpstr, &maxlen, &pos);
					free(tmpstr); tmpstr = NULL;
					ostrcatbig(&buf, "#", &maxlen, &pos);

					tmpstr = olutoa(epgnode->endtime);
					ostrcatbig(&buf, tmpstr, &maxlen, &pos);
					free(tmpstr); tmpstr = NULL;
					ostrcatbig(&buf, "#", &maxlen, &pos);

					ostrcatbig(&buf, epgnode->subtitle, &maxlen, &pos);
					ostrcatbig(&buf, "#", &maxlen, &pos);

					if(longepg == 1)
					{
						tmpstr = epgdescunzip(epgnode);
						if(tmpstr != NULL)
							ostrcatbig(&buf, tmpstr, &maxlen, &pos);
						free(tmpstr); tmpstr = NULL;
					}
					ostrcatbig(&buf, "#", &maxlen, &pos);

					tmpstr = oitoa(epgnode->eventid);
					ostrcatbig(&buf, tmpstr, &maxlen, &pos);
					free(tmpstr); tmpstr = NULL;
					ostrcatbig(&buf, "\n", &maxlen, &pos);
				}
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

	if(fmt == 0)
		webcreatetailbig(&buf, &maxlen, &pos, 0);
	else if(buf == NULL)
		buf = ostrcat("no data", NULL, 0, 0);

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
		tmppath = ostrcat(path, NULL, 0, 0);
	else
		tmppath = ostrcat(param, NULL, 0, 0);

	char* tmppath1 = createpath(tmppath, "");
	free(tmppath); tmppath = tmppath1;

	if(checkbit(flag, 0) == 1)
	{
		if(ostrstr(tmppath, path) != tmppath)
		{
			free(tmppath); tmppath = NULL;
			tmppath = ostrcat(path, NULL, 0, 0);
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
						node->text = string_replace_all("\"", "&#34;", node->text, 1);
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
						node->text = string_replace_all("'", "&#39;", node->text, 1);
						node->text = string_replace_all("\"", "&#34;", node->text, 1);
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
					//transstream png
#ifdef MIPSEL
					if(checkbit(flag, 3) == 1)
					{

						if(file_exist("/proc/stb/encoder") &&  getconfigint("web_trans_transcode", NULL) == 1)
						{
							ostrcatbig(&buf, "<a target=nothing href=\"query?gettranscodem3u&0,0,", &maxlen, &pos);
							ostrcatbig(&buf, filelistpath->text, &maxlen, &pos);
							ostrcatbig(&buf, "/", &maxlen, &pos);
							ostrcatbig(&buf, node->text, &maxlen, &pos);
							ostrcatbig(&buf, "\">", &maxlen, &pos);
							ostrcatbig(&buf, "<img border=0 src=img/transstream.png width=16 height=16 alt=Stream Transcode></a>", &maxlen, &pos);
						}
					}
#endif
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

	if(fmt == 0 && maxcount > MAXHTMLLINE)
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
		if(param != NULL && ostrstr(param, path) == param)
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
	int del = 0;
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
		if(tmpparam != NULL && ostrstr(tmpparam, path) == tmpparam)
			del = 1;
	}
	else
		del = 1;

	if(del == 1)
	{

		char* epgfilename = NULL;

		unlink(tmpparam);

		if(cmpfilenameext(tmpparam, ".ts") == 0)
		{

			epgfilename = changefilenameext(tmpparam, ".epg");
			unlink(epgfilename);
			free(epgfilename); epgfilename = NULL;
			epgfilename = changefilenameext(tmpparam, ".se");
			unlink(epgfilename);
			free(epgfilename); epgfilename = NULL;
			epgfilename = changefilenameext(tmpparam, ".ma");
			unlink(epgfilename);
			free(epgfilename); epgfilename = NULL;
		}
	}

	free(tmpparam); tmpparam = NULL;
	tmpparam = ostrcat(tmpparam, param, 1, 0);
	tmpparam = ostrcat(tmpparam, "&", 1, 0);
	tmpparam = ostrcat(tmpparam, param1, 1, 0);

	buf = webgetfilelist(tmpparam, link, dellink, path, mask, flag, fmt);
	free(tmpparam); tmpparam = NULL;
	return buf;
}

char* websendrc(char* param, int fmt)
{
	int rccode = 0;
	char* buf = NULL;

	rccode = getrcconfigint(param, NULL);
	writerc(rccode);

	buf = ostrcat("ok", NULL, 0, 0);
	return buf;
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
		if((flag == 0 && (node->status == 2 || node->status == 3)) || (flag == 1 && (node->status == 0 || node->status == 1)) || node->status == 4 || node->status == 5)
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
			buf = ostrcat(buf, ollutoa(node->transponderid), 1, 1);
			buf = ostrcat(buf, "#", 1, 0);
			buf = ostrcat(buf, oitoa(node->status), 1, 1);
			buf = ostrcat(buf, "#", 1, 0);
			buf = ostrcat(buf, node->errstr, 1, 0);
			buf = ostrcat(buf, "#", 1, 0);
			buf = ostrcat(buf, node->timestamp, 1, 0);
			buf = ostrcat(buf, "#", 1, 0);
			chnode = getchannel(node->serviceid, node->transponderid);
			if(chnode != NULL)
				buf = ostrcat(buf, chnode->name, 1, 0);
			buf = ostrcat(buf, "#", 1, 0);
			buf = ostrcat(buf, oitoa(node->afterevent), 1, 1);
			buf = ostrcat(buf, "\n", 1, 0);
		}

		node = node->next;
	}

	if(fmt == 0)
		webcreatetailbig(&buf, &maxlen, &pos, 0);
	else if(buf == NULL)
		buf = ostrcat("no data", NULL, 0, 0);

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
		chnode = getchannel(atoi(param), strtoull(param1, NULL, 10));
		if(chnode == NULL) return NULL;

		epgnode = getepg(chnode, atoi(param2), 0);
		if(epgnode == NULL) return NULL;
	}


	ostrcatbig(&buf, "<html><head><meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\"><link rel=\"stylesheet\" type=\"text/css\" href=\"titan.css\"></head>", &maxlen, &pos);
	ostrcatbig(&buf, "<body class=body ><center>", &maxlen, &pos);
	ostrcatbig(&buf, "<form name=F1 action=query method=get><br><br>", &maxlen, &pos);
	if(chnode == NULL)
		ostrcatbig(&buf, "<input type=\"hidden\" name=\"rectimercheck&node\" value=\"", &maxlen, &pos);
	else
		ostrcatbig(&buf, "<input type=\"hidden\" name=\"rectimersend&node\" value=\"", &maxlen, &pos);
	ostrcatbig(&buf, "0", &maxlen, &pos);
	ostrcatbig(&buf, "\">", &maxlen, &pos);
	ostrcatbig(&buf, "<table border=\"0\"><tr>", &maxlen, &pos);
	ostrcatbig(&buf, "<td><font class=label>Name:&nbsp;</font></td>", &maxlen, &pos);
	ostrcatbig(&buf, "<td><input class=inputbox type=\"text\" name=\"name\" value=\"", &maxlen, &pos);

	if(epgnode != NULL)
	{
		epgnode->title = strstrip(epgnode->title);
		delspezchar(epgnode->title, 2);
		ostrcatbig(&buf, epgnode->title, &maxlen, &pos);
	}
	else
		ostrcatbig(&buf, " ", &maxlen, &pos);
	ostrcatbig(&buf, "\" /></td></tr>", &maxlen, &pos);
	ostrcatbig(&buf, "<td><font class=label>Type:&nbsp;</font></td>", &maxlen, &pos);
	ostrcatbig(&buf, "<td><select name=\"type\" border=0><option selected>", &maxlen, &pos);
	if(status.pvr == 1)
	{
		ostrcatbig(&buf, "record", &maxlen, &pos);
		ostrcatbig(&buf, "<option>record<option>switch channel</select></td></tr>", &maxlen, &pos);
	}
	else
	{
		ostrcatbig(&buf, "switch channel", &maxlen, &pos);
		ostrcatbig(&buf, "<option>record</select></td></tr>", &maxlen, &pos);
	}

	buf2 = malloc(MINMALLOC);
	if(epgnode != NULL)
		loctime =	olocaltime(&epgnode->starttime);
	else
		loctime = olocaltime(&akttime);
	if(loctime != NULL)
		strftime(buf2, MINMALLOC, "%H:%M %d-%m-%Y", loctime);
	free(loctime); loctime = NULL;

	buf1 = ostrcat(buf2, NULL, 0, 0);
	ostrcatbig(&buf, "<td><font class=label>Begin:&nbsp;</font></td>", &maxlen, &pos);
	ostrcatbig(&buf, "<td><input class=inputbox type=\"text\" name=\"begin\" value=\"", &maxlen, &pos);
	ostrcatbig(&buf, buf1, &maxlen, &pos);
	ostrcatbig(&buf, "\" /></td></tr>", &maxlen, &pos);
	free(buf1); buf1 = NULL;

	if(epgnode != NULL)
		loctime =	olocaltime(&epgnode->endtime);
	else
		loctime = olocaltime(&akttime);
	if(loctime != NULL)
		strftime(buf2, MINMALLOC, "%H:%M %d-%m-%Y", loctime);
	free(loctime); loctime = NULL;

	buf1 = ostrcat(buf2, NULL, 0, 0);
	ostrcatbig(&buf, "<td><font class=label>End:&nbsp;</font></td>", &maxlen, &pos);
	ostrcatbig(&buf, "<td><input class=inputbox type=\"text\" name=\"end\" value=\"", &maxlen, &pos);
	ostrcatbig(&buf, buf1, &maxlen, &pos);
	ostrcatbig(&buf, "\" /></td></tr>", &maxlen, &pos);
	free(buf1); buf1 = NULL;
	free(buf2); buf2 = NULL;

	ostrcatbig(&buf, "<td><font class=label>Channel:&nbsp;</font></td>", &maxlen, &pos);
	if(chnode == NULL)
		ostrcatbig(&buf, "<td><input class=inputbox type=\"text\" name=\"channel\" value=\"", &maxlen, &pos);
	else
	{
		ostrcatbig(&buf, "<td>", &maxlen, &pos);
		ostrcatbig(&buf, "<input class=inputbox type=\"hidden\" name=\"sid\" value=\"", &maxlen, &pos);
		buf1 = oitoa(chnode->serviceid);
		ostrcatbig(&buf, buf1, &maxlen, &pos);
		free(buf1); buf1 = NULL;
		ostrcatbig(&buf, "\" />", &maxlen, &pos);
		ostrcatbig(&buf, "<input class=inputbox type=\"hidden\" name=\"tid\" value=\"", &maxlen, &pos);
		buf1 = ollutoa(chnode->transponderid);
		ostrcatbig(&buf, buf1, &maxlen, &pos);
		free(buf1); buf1 = NULL;
		ostrcatbig(&buf, "\" />", &maxlen, &pos);
		ostrcatbig(&buf, "<input readonly class=inputbox type=\"text\" name=\"channel\" value=\"", &maxlen, &pos);
	}
	if(chnode != NULL)
		ostrcatbig(&buf, chnode->name, &maxlen, &pos);
	else if(status.aktservice->channel != NULL)
		ostrcatbig(&buf, status.aktservice->channel->name, &maxlen, &pos);
	ostrcatbig(&buf, "\" /></td></tr>", &maxlen, &pos);
	free(buf1); buf1 = NULL;

	ostrcatbig(&buf, "<td><font class=label>after event:&nbsp;</font></td>", &maxlen, &pos);
	ostrcatbig(&buf, "<td><select name=\"afterevent\" border=0><option selected>", &maxlen, &pos);
	ostrcatbig(&buf, "auto", &maxlen, &pos);
	ostrcatbig(&buf, "<option>auto<option>nothing<option>standby<option>off</select></td></tr>", &maxlen, &pos);

	ostrcatbig(&buf, "</table><br><br><input class=button type=submit name=send value=\"Send\" onClick=\"return checkdaytime(begin.value, end.value)\"></input>&nbsp;<input class=button type=reset name=reset value=\"Reset\"></input></form></center></body></html>", &maxlen, &pos);

	//ostrcatbig(&buf, param, &maxlen, &pos);
	return buf;
}

char* webrectimercheck(char* param, int fmt)
{
	char* buf = NULL, *buf1 = NULL, *string = NULL, *param1 = NULL, *name = NULL, *begin = NULL, *end = NULL, *type = NULL, *anode = NULL, *channelname = NULL, *ext = NULL, *afterevent = NULL, *repeat = NULL;
	int maxlen = 0, pos = 0, channelfind = 0;
	struct channel *channel1 = NULL;

	anode = ostrstr(param, "node=");
	if(anode != NULL)
		anode = anode + 5;
	name = ostrstr(param, "name=");
	if(name != NULL)
		name = name + 5;
	begin = ostrstr(param, "begin=");
	if(begin != NULL)
		begin = begin + 6;
	end = ostrstr(param, "end=");
	if(end != NULL)
		end = end + 4;
	type = ostrstr(param, "type=");
	if(type != NULL)
		type = type + 5;
	channelname = ostrstr(param, "channel=");
	if(channelname != NULL)
		channelname = channelname + 8;
	ext = ostrstr(param, "ext=");
	if(ext != NULL)
		ext = ext + 4;
	afterevent = ostrstr(param, "afterevent=");
	if(afterevent != NULL)
		afterevent = afterevent + 11;
	repeat = ostrstr(param, "repeat=");
	if(repeat != NULL)
		repeat = repeat + 7;

	param1 = ostrcat(param, NULL, 0, 0);
	string = param;
	while(string != NULL)
	{
		string = strchr(string, '&');
		if(string != NULL)
			*string++ = '\0';
	}

	ostrcatbig(&buf, "<html><head><meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\"><link rel=\"stylesheet\" type=\"text/css\" href=\"titan.css\"></head>", &maxlen, &pos);
	ostrcatbig(&buf, "<body class=body ><center>", &maxlen, &pos);
	ostrcatbig(&buf, "<form name=F1 action=query method=get><br><br>", &maxlen, &pos);

	if(anode != NULL)
	{
		ostrcatbig(&buf, "<input type=\"hidden\" name=\"rectimersend&node\" value=\"", &maxlen, &pos);
		ostrcatbig(&buf, anode, &maxlen, &pos);
		ostrcatbig(&buf, "\" />", &maxlen, &pos);
	}

	if(name != NULL)
	{
		ostrcatbig(&buf, "<input type=\"hidden\" name=\"name\" value=\"", &maxlen, &pos);
		ostrcatbig(&buf, name, &maxlen, &pos);
		ostrcatbig(&buf, "\" />", &maxlen, &pos);
	}

	if(begin != NULL)
	{
		ostrcatbig(&buf, "<input type=\"hidden\" name=\"begin\" value=\"", &maxlen, &pos);
		ostrcatbig(&buf, begin, &maxlen, &pos);
		ostrcatbig(&buf, "\" />", &maxlen, &pos);
	}

	if(end != NULL)
	{
		ostrcatbig(&buf, "<input type=\"hidden\" name=\"end\" value=\"", &maxlen, &pos);
		ostrcatbig(&buf, end, &maxlen, &pos);
		ostrcatbig(&buf, "\" />", &maxlen, &pos);
	}

	if(type != NULL)
	{
		ostrcatbig(&buf, "<input type=\"hidden\" name=\"type\" value=\"", &maxlen, &pos);
		ostrcatbig(&buf, type, &maxlen, &pos);
		ostrcatbig(&buf, "\" />", &maxlen, &pos);
	}

	if(ext != NULL)
	{
		ostrcatbig(&buf, "<input type=\"hidden\" name=\"ext\" value=\"", &maxlen, &pos);
		ostrcatbig(&buf, ext, &maxlen, &pos);
		ostrcatbig(&buf, "\" />", &maxlen, &pos);
	}

	if(afterevent != NULL)
	{
		ostrcatbig(&buf, "<input type=\"hidden\" name=\"afterevent\" value=\"", &maxlen, &pos);
		ostrcatbig(&buf, afterevent, &maxlen, &pos);
		ostrcatbig(&buf, "\" />", &maxlen, &pos);
	}

	if(repeat != NULL)
	{
		ostrcatbig(&buf, "<input type=\"hidden\" name=\"repeat\" value=\"", &maxlen, &pos);
		ostrcatbig(&buf, repeat, &maxlen, &pos);
		ostrcatbig(&buf, "\" />", &maxlen, &pos);
	}

	ostrcatbig(&buf, "<table border=\"0\"><tr>", &maxlen, &pos);
	ostrcatbig(&buf, "<td><select name=sid size=10 border=0>", &maxlen, &pos);

	/*
	channel1 = channel;
	while(channel1 != NULL)
	{
		if(ostrcmp(channel1->name, channelname) == 0 && (channel1->servicetype == 0 || channel1->servicetype == 1))
		{
			if(channelfind == 0)
				ostrcatbig(&buf, "<option selected value=\"", &maxlen, &pos);
			else
				ostrcatbig(&buf, "<option value=\"", &maxlen, &pos);

			buf1 = oitoa(channel1->serviceid);
			ostrcatbig(&buf, buf1, &maxlen, &pos);
			free(buf1); buf1 = NULL;
			ostrcatbig(&buf, "&tid=", &maxlen, &pos);
			buf1 = ollutoa(channel1->transponderid);
			ostrcatbig(&buf, buf1, &maxlen, &pos);
			free(buf1); buf1 = NULL;
			ostrcatbig(&buf, "\">", &maxlen, &pos);

			ostrcatbig(&buf, channel1->name, &maxlen, &pos);
			ostrcatbig(&buf, " (", &maxlen, &pos);

			//get satname from channel
			if(channel1->transponder != NULL)
			{
				struct sat* snode = getsatbyorbitalpos(channel1->transponder->orbitalpos);
				if(snode != NULL)
					ostrcatbig(&buf, snode->name, &maxlen, &pos);
				else
					ostrcatbig(&buf, _("unknown"), &maxlen, &pos);

				if(channel1->provider != NULL)
					ostrcatbig(&buf, " - ", &maxlen, &pos);
			}

			//get provider from channel
			if(channel1->provider != NULL)
			{
				if(channel1->provider->name != NULL)
					ostrcatbig(&buf, channel1->provider->name, &maxlen, &pos);
				else
					ostrcatbig(&buf, _("unknown"), &maxlen, &pos);
			}

			ostrcatbig(&buf, ")", &maxlen, &pos);

			channelfind++;
		}
		channel1 = channel1->next;
	}
	*/

	if(channelfind == 0)
	{
		channel1 = channel;
		while(channel1 != NULL)
		{
			if(ostrstrcase(channel1->name, channelname) != NULL && (channel1->servicetype == 0 || channel1->servicetype == 1))
			{
				if(channelfind == 0)
					ostrcatbig(&buf, "<option selected value=\"", &maxlen, &pos);
				else
					ostrcatbig(&buf, "<option value=\"", &maxlen, &pos);

				buf1 = oitoa(channel1->serviceid);
				ostrcatbig(&buf, buf1, &maxlen, &pos);
				free(buf1); buf1 = NULL;
				ostrcatbig(&buf, "&tid=", &maxlen, &pos);
				buf1 = ollutoa(channel1->transponderid);
				ostrcatbig(&buf, buf1, &maxlen, &pos);
				free(buf1); buf1 = NULL;
				ostrcatbig(&buf, "\">", &maxlen, &pos);

				ostrcatbig(&buf, channel1->name, &maxlen, &pos);
				ostrcatbig(&buf, " (", &maxlen, &pos);

				//get satname from channel
				if(channel1->transponder != NULL)
				{
					struct sat* snode = getsatbyorbitalpos(channel1->transponder->orbitalpos);
					if(snode != NULL)
						ostrcatbig(&buf, snode->name, &maxlen, &pos);
					else
						ostrcatbig(&buf, _("unknown"), &maxlen, &pos);

					if(channel1->provider != NULL)
						ostrcatbig(&buf, " - ", &maxlen, &pos);
				}

				//get provider from channel
				if(channel1->provider != NULL)
				{
					if(channel1->provider->name != NULL)
						ostrcatbig(&buf, channel1->provider->name, &maxlen, &pos);
					else
						ostrcatbig(&buf, _("unknown"), &maxlen, &pos);
				}

				ostrcatbig(&buf, ")", &maxlen, &pos);

				channelfind++;
			}
			channel1 = channel1->next;
		}
	}

	ostrcatbig(&buf, "</td></tr></table><br><br></select><input class=button type=submit name=send value=\"Send\" onClick=\"return checkdaytime(begin.value, end.value)\"></input>&nbsp;<input class=button type=reset name=reset value=\"Reset\"></input></form></center></body></html>", &maxlen, &pos);

	if(channelfind < 2)
	{
		free(buf); buf = NULL;
		buf = webrectimersend(param1, fmt);
		free(param1); param1 = NULL;
	}

	return buf;
}

char* webrectimersend(char* param, int fmt)
{
	char* buf = NULL, *string = NULL, *name = NULL, *begin = NULL, *end = NULL, *type = NULL, *anode = NULL, *channelname = NULL, *sid = NULL, *tid = NULL, *ext = NULL, *afterevent = NULL, *repeat = NULL;
	int newnode = 0, channelfind = 0;
	struct rectimer *node = NULL;
	char* tmpstr = NULL;
	struct tm* loctime = NULL;
	struct channel *channel1 = NULL;

	anode = ostrstr(param, "node=");
	if(anode != NULL)
		anode = anode + 5;
	name = ostrstr(param, "name=");
	if(name != NULL)
		name = name + 5;
	begin = ostrstr(param, "begin=");
	if(begin != NULL)
		begin = begin + 6;
	end = ostrstr(param, "end=");
	if(end != NULL)
		end = end + 4;
	type = ostrstr(param, "type=");
	if(type != NULL)
		type = type + 5;
	channelname = ostrstr(param, "channel=");
	if(channelname != NULL)
		channelname = channelname + 8;
	sid = ostrstr(param, "sid=");
	if(sid != NULL)
		sid = sid + 4;
	tid = ostrstr(param, "tid=");
	if(tid != NULL)
		tid = tid + 4;
	ext = ostrstr(param, "ext=");
	if(ext != NULL)
		ext = ext + 4;
	afterevent = ostrstr(param, "afterevent=");
	if(afterevent != NULL)
		afterevent = afterevent + 11;
	repeat = ostrstr(param, "repeat=");
	if(repeat != NULL)
		repeat = repeat + 7;

	string = param;
	while(string != NULL)
	{
		string = strchr(string, '&');
		if(string != NULL)
			*string++ = '\0';
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
			if(ostrcmp(channel1->name, channelname) == 0 && (channel1->servicetype == 0 || channel1->servicetype == 1))
			{
					channelfind = 1;
					break;
			}
			channel1 = channel1->next;
		}
		if(channelfind == 0)
		{
			channel1 = channel;
			while(channel1 != NULL)
			{
				if(ostrstr(channel1->name, channelname) != NULL && (channel1->servicetype == 0 || channel1->servicetype == 1))
				{
						channelfind = 1;
						break;
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
			node->transponderid = strtoull(tid, NULL, 10);
			node->servicetype = 0;
		}

		free(node->name); node->name = NULL;
		node->name = ostrcat(name, NULL, 0, 0);

		if(ostrcmp(type, "record") == 0)
			node->justplay = 0;
		else
			node->justplay = 1;

		if(afterevent != NULL)
		{
			if(ostrcmp(afterevent, "auto") == 0)
				node->afterevent = 0;
			else if(ostrcmp(afterevent, "nothing") == 0)
				node->afterevent = 1;
			else if(ostrcmp(afterevent, "standby") == 0)
				node->afterevent = 2;
			else if(ostrcmp(afterevent, "off") == 0)
				node->afterevent = 3;
		}

		if(repeat != NULL)
			node->repeate = atoi(repeat);

		loctime = olocaltime(&node->begin);
		if(loctime != NULL && begin != NULL)
		{
			tmpstr = strptime(begin, "%H:%M %d-%m-%Y", loctime);
			if(tmpstr != NULL) {
				loctime->tm_isdst = -1;
				node->begin = mktime(loctime);
			}
		}
		if(node->justplay == 0 && newnode == 1) node->begin -= getconfigint("recforerun" , NULL) * 60;
		node->begin -= (node->begin % 60);
		tmpstr = NULL;
		free(loctime); loctime = NULL;

		loctime = olocaltime(&node->end);
		if(loctime != NULL && end != NULL)
		{
			tmpstr = strptime(end, "%H:%M %d-%m-%Y", loctime);
			if(tmpstr != NULL) {
				loctime->tm_isdst = -1;
				node->end = mktime(loctime);
			}
		}
		if(node->justplay == 0 && newnode == 1) node->end += getconfigint("recoverrun" , NULL) * 60;
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
	if(node->justplay == 0 && status.pvr == 1)
	{
		ostrcatbig(&buf, "record", &maxlen, &pos);
		ostrcatbig(&buf, "<option>record<option>switch channel</select></td></tr>", &maxlen, &pos);
	}
	else
	{
		ostrcatbig(&buf, "switch channel", &maxlen, &pos);
		ostrcatbig(&buf, "<option>switch channel</select></td></tr>", &maxlen, &pos);
	}

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

	buf2 = malloc(20);
	loctime = olocaltime(&node->begin);
	if(loctime != NULL && buf2 != NULL)
		strftime(buf2, 20, "%H:%M %d-%m-%Y", loctime);
	free(loctime); loctime = NULL;

	buf1 = ostrcat(buf2, NULL, 0, 0);
	ostrcatbig(&buf, "<td><font class=label>Begin:&nbsp;</font></td>", &maxlen, &pos);
	ostrcatbig(&buf, "<td><input class=inputbox type=\"text\" name=\"begin\" value=\"", &maxlen, &pos);
	ostrcatbig(&buf, buf1, &maxlen, &pos);
	ostrcatbig(&buf, "\" /></td></tr>", &maxlen, &pos);
	free(buf1); buf1 = NULL;

	loctime = olocaltime(&node->end);
	if(loctime != NULL && buf2 != NULL)
		strftime(buf2, 20, "%H:%M %d-%m-%Y", loctime);
	free(loctime); loctime = NULL;

	buf1 = ostrcat(buf2, NULL, 0, 0);
	ostrcatbig(&buf, "<td><font class=label>End:&nbsp;</font></td>", &maxlen, &pos);
	ostrcatbig(&buf, "<td><input class=inputbox type=\"text\" name=\"end\" value=\"", &maxlen, &pos);
	ostrcatbig(&buf, buf1, &maxlen, &pos);
	ostrcatbig(&buf, "\" /></td></tr>", &maxlen, &pos);
	free(buf1); buf1 = NULL;
	free(buf2); buf2 = NULL;

	ostrcatbig(&buf, "<td><font class=label>after event:&nbsp;</font></td>", &maxlen, &pos);
	ostrcatbig(&buf, "<td><select name=\"afterevent\" border=0><option selected>", &maxlen, &pos);
	if( node->afterevent == 0 )
		ostrcatbig(&buf, "auto", &maxlen, &pos);
	else if( node->afterevent == 1 )
		ostrcatbig(&buf, "nothing", &maxlen, &pos);
	else if( node->afterevent == 2 )
		ostrcatbig(&buf, "standby", &maxlen, &pos);
	else if( node->afterevent == 3 )
		ostrcatbig(&buf, "off", &maxlen, &pos);
	else
		ostrcatbig(&buf, "auto", &maxlen, &pos);
	ostrcatbig(&buf, "<option>auto<option>nothing<option>standby<option>off</select></td></tr>", &maxlen, &pos);

	ostrcatbig(&buf, "</table><br><br><input class=button type=submit name=send value=\"Send\" onClick=\"return checkdaytime(begin.value, end.value)\"></input>&nbsp;<input class=button type=reset name=reset value=\"Reset\"></input></form></center></body></html>", &maxlen, &pos);

	//ostrcatbig(&buf, param, &maxlen, &pos);
	return buf;
}

char* webdelrectimer(char* param, int fmt)
{
	char* buf = NULL, *string = NULL, *timerid = NULL;
	struct rectimer *node = NULL;
	int ext = 0;

	timerid = ostrstr(param, "timerid=");
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
		if(status.standby == 0)
			textbox(caption, body, _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 280, timeout, 0);
		else
		{
			FILE* datei = fopen("/tmp/textbox_standby", "a");

			body = string_replace_all("\n", "\t", body, 1);
			fprintf(datei, "%s\n%s\n%s\n%i\n%s\n%i\n%i\n%i\n%i\n%i\n%i\n%i\n%i\n%i\n", caption, body, _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), 0, 0, 0, 0, 600, 280, timeout, 0);
			fclose(datei);
		}
	}

	free(caption); free(body); free(captiontime); free(ret1);
}

void xmessage(char* filename)
{
	char* param=NULL, *param1 = NULL, *param2 = NULL, *param3 = NULL, *param4 = NULL;
	char* caption=NULL, *body=NULL;
	char* timeout=NULL;

	param4 = ostrstr(filename, "icon=");
	param3 = ostrstr(filename, "charset=");
	param2 = ostrstr(filename, "timeout=");
	param1 = ostrstr(filename, "caption=");
	param = ostrstr(filename, "body=");

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

char* webgetrecsteampath(int fmt)
{
	char* buf = NULL;

	if(getconfig("rec_streampath", NULL) == NULL)
		buf = ostrcat(buf, "/var/media/hdd/movie", 1, 0);
	else
		buf = ostrcat(buf, getconfig("rec_streampath", NULL), 1, 0);

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
	struct clist* node = NULL;

	m_lock(&status.clistmutex, 12);
	struct clist **clist = rcconfig;

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

	m_unlock(&status.clistmutex, 12);
	return buf;
}

char* webgetconfig(int fmt)
{
	char* buf = NULL;
	struct clist *node = NULL;
	int i = 0;

	m_lock(&status.clistmutex, 12);
	if(fmt == 0)
	{
		buf = ostrcat(buf, "<html><head><meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\">", 1, 0);
		buf = ostrcat(buf, "<link rel=stylesheet type=text/css href=titan.css><script type=text/javascript src=titan.js></script>", 1, 0);
		buf = ostrcat(buf, "</head><body class=body id=\"configinfo\">", 1, 0);
	}

	struct clist **clist = config;

	for(i = 0; i < LISTHASHSIZE; i++)
	{
		node = clist[i];

		while(node != NULL)
		{
			if(node->tmp != NULL && strlen(node->tmp) > 0)
			{
				buf = ostrcat(buf, node->key, 1, 0);
				buf = ostrcat(buf, "#", 1, 0);
				buf = ostrcat(buf, node->tmp, 1, 0);
				buf = ostrcat(buf, "\n", 1, 0);
			}
			else if(node->value != NULL)
			{
				buf = ostrcat(buf, node->key, 1, 0);
				buf = ostrcat(buf, "#", 1, 0);
				buf = ostrcat(buf, node->value, 1, 0);
				buf = ostrcat(buf, "\n", 1, 0);
			}
			else
			{
				buf = ostrcat(buf, node->key, 1, 0);
				buf = ostrcat(buf, "#", 1, 0);
				buf = ostrcat(buf, node->def, 1, 0);
				buf = ostrcat(buf, "\n", 1, 0);
			}

			node = node->next;
		}
	}

	buf = string_replace_all("\n", "<br>\n", buf, 1);

	if(fmt == 0)
	{
		buf = ostrcat(buf, "</body></html>", 1, 0);
	}

	m_unlock(&status.clistmutex, 12);
	return buf;
}

char* webgetrecfreesize(int fmt)
{
	char* buf = NULL;
	unsigned long long full = getfullspace(getconfig("rec_path", NULL));
	unsigned long long free = getfreespace(getconfig("rec_path", NULL));
	int proz = 0;

	if(full > 0 && full >= free) proz = ((full - free) * 100) / full;
	if(proz < 0) proz = 0;
	if(proz > 100) proz = 100;

	buf = ostrcat(buf, getconfig("rec_path", NULL), 1, 0);
	buf = ostrcat(buf, "#", 1, 0);
	buf = ostrcat(buf, ollutoa(full), 1, 1);
	buf = ostrcat(buf, "#", 1, 0);
	buf = ostrcat(buf, ollutoa(free), 1, 1);
	buf = ostrcat(buf, "#", 1, 0);
	buf = ostrcat(buf, oitoa(proz), 1, 1);
	buf = ostrcat(buf, " %", 1, 0);
	buf = ostrcat(buf, "#", 1, 0);

	return buf;
}

char* webgetsysteminfo(int fmt)
{
	char* buf = NULL;
	char* imgversion = NULL;

	if(fmt == 0)
	{
		buf = ostrcat(buf, "<html><head><meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\">", 1, 0);
		buf = ostrcat(buf, "<link rel=stylesheet type=text/css href=titan.css><script type=text/javascript src=titan.js></script>", 1, 0);
		buf = ostrcat(buf, "</head><body class=body id=\"systeminfo\">", 1, 0);
	}

	buf = ostrcat(buf, PROGNAME, 1, 0);
	buf = ostrcat(buf, "#", 1, 0);
	buf = ostrcat(buf, COPYRIGHT, 1, 0);
	buf = ostrcat(buf, "#", 1, 0);
	buf = ostrcat(buf, OVERSION, 1, 0);
	buf = ostrcat(buf, "#", 1, 0);

	if(isfile(getconfig("imagenamefile", NULL))	!= 0)
		imgversion = readsys(getconfig("imagenamefile", NULL), 1);
	else
		imgversion = ostrcat("unknown", NULL, 0, 0);

	buf = ostrcat(buf, imgversion, 1, 1);
	buf = ostrcat(buf, "#", 1, 0);
	buf = ostrcat(buf, getboxtype(), 1, 0);
	buf = ostrcat(buf, "#", 1, 0);
	buf = ostrcat(buf, ollutoa(time(NULL)), 1, 1);
	buf = ostrcat(buf, "#", 1, 0);

	if(status.standby == 0)
		buf = ostrcat(buf, "0", 1, 0);
	else
		buf = ostrcat(buf, "1", 1, 0);

  //transcode
  buf = ostrcat(buf, "#", 1, 0);
#ifdef MIPSEL
	if(file_exist("/proc/stb/encoder") && getconfigint("web_trans_transcode", NULL) == 1)
		buf = ostrcat(buf, "1", 1, 0);
	else
		buf = ostrcat(buf, "0", 1, 0);
#else
	buf = ostrcat(buf, "0", 1, 0);
#endif

	if(fmt == 0)
	{
		buf = ostrcat(buf, "</body></html>", 1, 0);
	}

	return buf;
}

char* webgetchannellock(char* param, int fmt)
{
	char* buf = NULL, *param1 = NULL;
	struct channel* chnode = NULL;

	if(param == NULL) return NULL;

	//create param1
	param1 = strchr(param, '&');
	if(param1 != NULL)
		*param1++ = '\0';

	if(param1 == NULL) return NULL;

	chnode = getchannel(atoi(param), strtoull(param1, NULL, 10));
	if(chnode != NULL)
	{
		buf = ostrcat(buf, chnode->name, 1, 0);
		buf = ostrcat(buf, "#", 1, 0);
		buf = ostrcat(buf, oitoa(chnode->serviceid), 1, 1);
		buf = ostrcat(buf, "#", 1, 0);
		buf = ostrcat(buf, ollutoa(chnode->transponderid), 1, 1);
		buf = ostrcat(buf, "#", 1, 0);

		if(channelnottunable(chnode) == 1)
			buf = ostrcat(buf, "1", 1, 0);
		else
			buf = ostrcat(buf, "0", 1, 0);
	}
	else
		buf = ostrcat("no data", NULL, 0, 0);

	return buf;
}

char* webgetbouquetepg(char* param, int fmt)
{
	char* buf = NULL, *tmpstr = NULL, *tmpstr1 = NULL;
	char* param1 = NULL, *param2 = NULL, *param3 = NULL;
	struct mainbouquet *mbouquet = NULL;
	struct bouquet *node = NULL;
	struct channel* chnode = NULL;
	struct epg* epgnode = NULL;
	int longepg = 0;
	time_t start = 0, end = 0;

	if(param == NULL) return NULL;

	//create param1 + 2 + 3
	param1 = strchr(param, '&');
	if(param1 != NULL)
	{
		*param1++ = '\0';
		param2 = strchr(param1, '&');
		if(param2 != NULL)
		{
			*param2++ = '\0';
			param3 = strchr(param2, '&');
			if(param3 != NULL)
				*param3++ = '\0';
		}
	}
	if(param1 == NULL || param2 == NULL || param3 == NULL) return NULL;

	longepg = atoi(param1);
	start = atoi(param2);
	end = atoi(param3);

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
				buf = ostrcat(buf, "BeginNewChannel", 1, 0);
				buf = ostrcat(buf, "#", 1, 0);
				buf = ostrcat(buf, chnode->name, 1, 0);
				buf = ostrcat(buf, "#", 1, 0);
				buf = ostrcat(buf, oitoa(chnode->serviceid), 1, 1);
				buf = ostrcat(buf, "#", 1, 0);
				buf = ostrcat(buf, ollutoa(chnode->transponderid), 1, 1);
				buf = ostrcat(buf, "#", 1, 0);
				buf = ostrcat(buf, oitoa(chnode->servicetype), 1, 1);
				buf = ostrcat(buf, "\n", 1, 0);

				epgnode = chnode->epg;
				while(epgnode != NULL)
				{
					if((epgnode->starttime >= start && epgnode->starttime < end) || (epgnode->endtime > start && epgnode->endtime <= end) || (epgnode->starttime < start && epgnode->endtime > end))
					{
						buf = ostrcat(buf, epgnode->title, 1, 0);
						buf = ostrcat(buf, "#", 1, 0);
						buf = ostrcat(buf, olutoa(epgnode->starttime), 1, 1);
						buf = ostrcat(buf, "#", 1, 0);
						buf = ostrcat(buf, olutoa(epgnode->endtime), 1, 1);
						buf = ostrcat(buf, "#", 1, 0);
						buf = ostrcat(buf, epgnode->subtitle, 1, 0);
						buf = ostrcat(buf, "#", 1, 0);

						if(longepg == 1)
						{
							tmpstr = epgdescunzip(epgnode);
							if(tmpstr != NULL)
								buf = ostrcat(buf, tmpstr, 1, 0);
							free(tmpstr); tmpstr = NULL;
						}

						buf = ostrcat(buf, "#", 1, 0);
						buf = ostrcat(buf, oitoa(epgnode->eventid), 1, 1);
						buf = ostrcat(buf, "\n", 1, 0);
					}

					epgnode = epgnode->next;
				}
			}
			node = node->next;
		}
		free(tmpstr1); tmpstr1 = NULL;
	}

	if(buf == NULL)
		buf = ostrcat("no data", NULL, 0, 0);
	return buf;
}

char* webadjust(char* param, int fmt)
{
printf("webadjust param: %s\n",param);
printf("webadjust fmt: %d\n",fmt);

	char* buf = NULL, *tmpstr = NULL, *tmpstr1 = NULL, *sendstr = NULL;
	int i = 0, max = 0, count = 0, maxlen = 0, pos = 0;
	struct splitstr* ret1 = NULL;


	ostrcatbig(&buf, "<html><head><meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\"><link rel=\"stylesheet\" type=\"text/css\" href=\"titan.css\"></head>", &maxlen, &pos);
	ostrcatbig(&buf, "<body class=body ><center>", &maxlen, &pos);
	ostrcatbig(&buf, "<form name=F1 action=query method=get><br><br>", &maxlen, &pos);
	ostrcatbig(&buf, "<input type=\"hidden\" name=\"adjustsend&node\" value=\"", &maxlen, &pos);
	ostrcatbig(&buf, "0", &maxlen, &pos);
	ostrcatbig(&buf, "\">", &maxlen, &pos);
//	ostrcatbig(&buf, "<input type=\"hidden\" name=\"adjustsend\">", &maxlen, &pos);
	ostrcatbig(&buf, "<table border=\"0\"><tr>", &maxlen, &pos);

	ostrcatbig(&buf, "<table>", &maxlen, &pos);

///////////////////
//	ostrcatbig(&buf, "<tr><td><font class=\"label\">Volbartimeout:&nbsp;</font></td><td><select name=\"volbartimeout\" border=\"0\"><option value=\"1\" selected>1</option><option value=\"2\">2</option><option value=\"3\">3</option><option value=\"4\">4</option><option value=\"5\">5</option><option value=\"6\">6</option><option value=\"7\">7</option><option value=\"8\">8</option><option value=\"9\">9</option><option value=\"10\">10</option></select></td></tr>", &maxlen, &pos);
	max = 10;
	ostrcatbig(&buf, "<tr><td><font class=\"label\">", &maxlen, &pos);
	ostrcatbig(&buf, _("Volumebar Timeout"), &maxlen, &pos);
	ostrcatbig(&buf, "&nbsp;</font></td><td><select name=\"volbartimeout\" border=\"0\">", &maxlen, &pos);
	for(i = 1; i <= max; i++)
	{
		ostrcatbig(&buf, "<option value=\"", &maxlen, &pos);
		ostrcatbig(&buf, oitoa(i), &maxlen, &pos);
		if(getconfigint("volbartimeout", NULL) == i)
			ostrcatbig(&buf, "\" selected>", &maxlen, &pos);
		else
			ostrcatbig(&buf, "\">", &maxlen, &pos);
		ostrcatbig(&buf, oitoa(i), &maxlen, &pos);
		ostrcatbig(&buf, "</option>", &maxlen, &pos);
	}
	ostrcatbig(&buf,"</select></td></tr>", &maxlen, &pos);
	sendstr = ostrcat(sendstr, "volbartimeout.value", 1 , 0);

///////////////////
//	ostrcatbig(&buf, "<tr><td><font class=\"label\">Infobartimeout:&nbsp;</font></td><td><select name=\"infobartimeout\" border=\"0\"><option value=\"1\" selected>1</option><option value=\"2\">2</option><option value=\"3\">3</option><option value=\"4\">4</option><option value=\"5\">5</option><option value=\"6\">6</option><option value=\"7\">7</option><option value=\"8\">8</option><option value=\"9\">9</option><option value=\"10\">10</option></select></td></tr>", &maxlen, &pos);
	tmpstr = ostrcat("1\n2\n3\n4\n5\n6\n7\n8\n9\n10\n20\n30\n60\n10000", NULL, 0, 0);

	ostrcatbig(&buf, "<tr><td><font class=\"label\">", &maxlen, &pos);
	ostrcatbig(&buf, _("Infobar Timeout"), &maxlen, &pos);
	ostrcatbig(&buf, "&nbsp;</font></td><td><select name=\"infobartimeout\" border=\"0\">", &maxlen, &pos);

	count = 0;
	ret1 = strsplit(tmpstr, "\n", &count);
	max = count - 1;

	if(ret1 != NULL)
	{
		for(i = 0; i <= max; i++)
		{
			ostrcatbig(&buf, "<option value=\"", &maxlen, &pos);
			ostrcatbig(&buf, ret1[i].part, &maxlen, &pos);
			tmpstr1 = ostrcat(ret1[i].part, NULL, 0, 0);
			if(getconfigint("infobartimeout", NULL) == atoi(tmpstr1))
				ostrcatbig(&buf, "\" selected>", &maxlen, &pos);
			else
				ostrcatbig(&buf, "\">", &maxlen, &pos);
			ostrcatbig(&buf, ret1[i].part, &maxlen, &pos);
			ostrcatbig(&buf, "</option>", &maxlen, &pos);
		}
	}
	ostrcatbig(&buf,"</select></td></tr>", &maxlen, &pos);
	sendstr = ostrcat(sendstr, ", infobartimeout.value", 1 , 0);
	free(ret1), ret1 = NULL;
	free(tmpstr), tmpstr = NULL;
	free(tmpstr1), tmpstr1 = NULL;

///////////////////
//	ostrcatbig(&buf, "<tr><td><font class=\"label\">Infobar Sleep:&nbsp;</font></td><td><select name=\"infobarsleep\" border=\"0\"><option value=\"0\">0</option><option value=\"1\" selected>1</option><option value=\"2\">2</option><option value=\"3\">3</option><option value=\"4\">4</option><option value=\"5\">5</option></select></td></tr>", &maxlen, &pos);
	max = 5;
	ostrcatbig(&buf, "<tr><td><font class=\"label\">", &maxlen, &pos);
	ostrcatbig(&buf, _("Infobar Sleep"), &maxlen, &pos);
	ostrcatbig(&buf, "&nbsp;</font></td><td><select name=\"infobarsleep\" border=\"0\">", &maxlen, &pos);
	for(i = 0; i <= max; i++)
	{
		ostrcatbig(&buf, "<option value=\"", &maxlen, &pos);
		ostrcatbig(&buf, oitoa(i), &maxlen, &pos);
		if(getconfigint("infobarsleep", NULL) == i)
			ostrcatbig(&buf, "\" selected>", &maxlen, &pos);
		else
			ostrcatbig(&buf, "\">", &maxlen, &pos);
		ostrcatbig(&buf, oitoa(i), &maxlen, &pos);
		ostrcatbig(&buf, "</option>", &maxlen, &pos);
	}
	ostrcatbig(&buf,"</select></td></tr>", &maxlen, &pos);
	sendstr = ostrcat(sendstr, ", infobarsleep.value", 1 , 0);

///////////////////
//	ostrcatbig(&buf, "<tr><td><font class=\"label\">Infobar Sleep:&nbsp;</font></td><td><select name=\"infobarsleep\" border=\"0\"><option value=\"0\">0</option><option value=\"1\" selected>1</option><option value=\"2\">2</option><option value=\"3\">3</option><option value=\"4\">4</option><option value=\"5\">5</option></select></td></tr>", &maxlen, &pos);
	max = 5;
	tmpstr = ostrcat("1\n3\n9\n12\n15\n18\n21\n24\n27\n30", NULL, 0, 0);

	ostrcatbig(&buf, "<tr><td><font class=\"label\">", &maxlen, &pos);
	ostrcatbig(&buf, _("Spinnerspeed"), &maxlen, &pos);
	ostrcatbig(&buf, "&nbsp;</font></td><td><select name=\"spinnerspeed\" border=\"0\">", &maxlen, &pos);

	count = 0;
	ret1 = strsplit(tmpstr, "\n", &count);
	max = count - 1;

	if(ret1 != NULL)
	{
		for(i = 0; i <= max; i++)
		{
			ostrcatbig(&buf, "<option value=\"", &maxlen, &pos);
			ostrcatbig(&buf, ret1[i].part, &maxlen, &pos);
			tmpstr1 = ostrcat(ret1[i].part, NULL, 0, 0);
			if(getconfigint("spinnerspeed", NULL) == atoi(tmpstr1))
				ostrcatbig(&buf, "\" selected>", &maxlen, &pos);
			else
				ostrcatbig(&buf, "\">", &maxlen, &pos);
			ostrcatbig(&buf, ret1[i].part, &maxlen, &pos);
			ostrcatbig(&buf, "</option>", &maxlen, &pos);
		}
	}
	ostrcatbig(&buf,"</select></td></tr>", &maxlen, &pos);
	sendstr = ostrcat(sendstr, ", spinnerspeed.value", 1 , 0);
	free(ret1), ret1 = NULL;
	free(tmpstr), tmpstr = NULL;
	free(tmpstr1), tmpstr1 = NULL;

///////////////////
	tmpstr = ostrcat("1\n2\n3\n4\n5\n6\n7\n8\n10000", NULL, 0, 0);

	ostrcatbig(&buf, "<tr><td><font class=\"label\">", &maxlen, &pos);
	ostrcatbig(&buf, _("Spinnertime"), &maxlen, &pos);
	ostrcatbig(&buf, "&nbsp;</font></td><td><select name=\"spinnertime\" border=\"0\">", &maxlen, &pos);

	count = 0;
	ret1 = strsplit(tmpstr, "\n", &count);
	max = count - 1;

	if(ret1 != NULL)
	{
		for(i = 0; i <= max; i++)
		{
			ostrcatbig(&buf, "<option value=\"", &maxlen, &pos);
			ostrcatbig(&buf, ret1[i].part, &maxlen, &pos);
			tmpstr1 = ostrcat(ret1[i].part, NULL, 0, 0);
			if(getconfigint("spinnertime", NULL) == atoi(tmpstr1))
				ostrcatbig(&buf, "\" selected>", &maxlen, &pos);
			else
				ostrcatbig(&buf, "\">", &maxlen, &pos);
			ostrcatbig(&buf, ret1[i].part, &maxlen, &pos);
			ostrcatbig(&buf, "</option>", &maxlen, &pos);
		}
	}
	ostrcatbig(&buf,"</select></td></tr>", &maxlen, &pos);
	sendstr = ostrcat(sendstr, ", spinnertime.value", 1 , 0);
	free(ret1), ret1 = NULL;
	free(tmpstr), tmpstr = NULL;
	free(tmpstr1), tmpstr1 = NULL;

///////////////////
	tmpstr = ostrcat("5\n10\n15\n30\n60\n120\n10000", NULL, 0, 0);

	ostrcatbig(&buf, "<tr><td><font class=\"label\">", &maxlen, &pos);
	ostrcatbig(&buf, _("Hangtime"), &maxlen, &pos);
	ostrcatbig(&buf, "&nbsp;</font></td><td><select name=\"hangtime\" border=\"0\">", &maxlen, &pos);

	count = 0;
	ret1 = strsplit(tmpstr, "\n", &count);
	max = count - 1;

	if(ret1 != NULL)
	{
		for(i = 0; i <= max; i++)
		{
			ostrcatbig(&buf, "<option value=\"", &maxlen, &pos);
			ostrcatbig(&buf, ret1[i].part, &maxlen, &pos);
			tmpstr1 = ostrcat(ret1[i].part, NULL, 0, 0);
			if(getconfigint("hangtime", NULL) == atoi(tmpstr1))
				ostrcatbig(&buf, "\" selected>", &maxlen, &pos);
			else
				ostrcatbig(&buf, "\">", &maxlen, &pos);
			ostrcatbig(&buf, ret1[i].part, &maxlen, &pos);
			ostrcatbig(&buf, "</option>", &maxlen, &pos);
		}
	}
	ostrcatbig(&buf,"</select></td></tr>", &maxlen, &pos);
	sendstr = ostrcat(sendstr, ", hangtime.value", 1 , 0);
	free(ret1), ret1 = NULL;
	free(tmpstr), tmpstr = NULL;
	free(tmpstr1), tmpstr1 = NULL;

///////////////////
//	ostrcatbig(&buf, "<tr><td><font class=\"label\">Second Infobar:&nbsp;</font></td><td><select name=\"secondinfobar\" border=\"0\"><option value=\"0\">no</option><option value=\"1\" selected>infobar</option><option value=\"2\">epg</option><option value=\"3\">channel epg</option></select></td></tr>", &maxlen, &pos);
	ostrcatbig(&buf, "<tr><td><font class=\"label\">", &maxlen, &pos);
	ostrcatbig(&buf, _("Second Infobar"), &maxlen, &pos);
	ostrcatbig(&buf, "&nbsp;</font></td><td><select name=\"secondinfobar\" border=\"0\">", &maxlen, &pos);

	if(getconfigint("secondinfobar", NULL) == 0)
		ostrcatbig(&buf, "<option value=\"0\" selected>", &maxlen, &pos);
	else
		ostrcatbig(&buf, "<option value=\"0\">", &maxlen, &pos);
	ostrcatbig(&buf, _("no"), &maxlen, &pos);
	ostrcatbig(&buf, "</option>", &maxlen, &pos);

	if(getconfigint("secondinfobar", NULL) == 1)
		ostrcatbig(&buf, "<option value=\"1\" selected>", &maxlen, &pos);
	else
		ostrcatbig(&buf, "<option value=\"1\">", &maxlen, &pos);
	ostrcatbig(&buf, _("infobar"), &maxlen, &pos);
	ostrcatbig(&buf, "</option>", &maxlen, &pos);

	if(getconfigint("secondinfobar", NULL) == 2)
		ostrcatbig(&buf, "<option value=\"2\" selected>", &maxlen, &pos);
	else
		ostrcatbig(&buf, "<option value=\"2\">", &maxlen, &pos);
	ostrcatbig(&buf, _("EPG"), &maxlen, &pos);
	ostrcatbig(&buf, "</option>", &maxlen, &pos);

	if(getconfigint("secondinfobar", NULL) == 3)
		ostrcatbig(&buf, "<option value=\"3\" selected>", &maxlen, &pos);
	else
		ostrcatbig(&buf, "<option value=\"3\">", &maxlen, &pos);
	ostrcatbig(&buf, _("Channel EPG"), &maxlen, &pos);
	ostrcatbig(&buf, "</option>", &maxlen, &pos);

	ostrcatbig(&buf,"</select></td></tr>", &maxlen, &pos);
	sendstr = ostrcat(sendstr, ", secondinfobar.value", 1 , 0);

///////////////////
	if(checkbox("ATEMIO7600") != 1)
	{
		ostrcatbig(&buf, "<tr><td><font class=\"label\">", &maxlen, &pos);
		ostrcatbig(&buf, _("Don't clear TV on zap (only with fastzap)"), &maxlen, &pos);
		ostrcatbig(&buf, "&nbsp;</font></td><td><select name=\"nozapclear\" border=\"0\">", &maxlen, &pos);

		if(getconfigint("nozapclear", NULL) == 0)
			ostrcatbig(&buf, "<option value=\"0\" selected>", &maxlen, &pos);
		else
			ostrcatbig(&buf, "<option value=\"0\">", &maxlen, &pos);
		ostrcatbig(&buf, _("no"), &maxlen, &pos);
		ostrcatbig(&buf, "</option>", &maxlen, &pos);

		if(getconfigint("nozapclear", NULL) == 1)
			ostrcatbig(&buf, "<option value=\"1\" selected>", &maxlen, &pos);
		else
			ostrcatbig(&buf, "<option value=\"1\">", &maxlen, &pos);
		ostrcatbig(&buf, _("yes"), &maxlen, &pos);
		ostrcatbig(&buf, "</option>", &maxlen, &pos);

		ostrcatbig(&buf,"</select></td></tr>", &maxlen, &pos);
		sendstr = ostrcat(sendstr, ", nozapclear.value", 1 , 0);
	}

///////////////////
	ostrcatbig(&buf, "<tr><td><font class=\"label\">", &maxlen, &pos);
	ostrcatbig(&buf, _("Fastzap"), &maxlen, &pos);
	ostrcatbig(&buf, "&nbsp;</font></td><td><select name=\"fastzap\" border=\"0\">", &maxlen, &pos);

	if(getconfigint("fastzap", NULL) == 0)
		ostrcatbig(&buf, "<option value=\"0\" selected>", &maxlen, &pos);
	else
		ostrcatbig(&buf, "<option value=\"0\">", &maxlen, &pos);
	ostrcatbig(&buf, _("no"), &maxlen, &pos);
	ostrcatbig(&buf, "</option>", &maxlen, &pos);

	if(getconfigint("fastzap", NULL) == 1)
		ostrcatbig(&buf, "<option value=\"1\" selected>", &maxlen, &pos);
	else
		ostrcatbig(&buf, "<option value=\"1\">", &maxlen, &pos);
	ostrcatbig(&buf, _("medium"), &maxlen, &pos);
	ostrcatbig(&buf, "</option>", &maxlen, &pos);

	if(getconfigint("fastzap", NULL) == 2)
		ostrcatbig(&buf, "<option value=\"2\" selected>", &maxlen, &pos);
	else
		ostrcatbig(&buf, "<option value=\"2\">", &maxlen, &pos);
	ostrcatbig(&buf, _("fast"), &maxlen, &pos);
	ostrcatbig(&buf, "</option>", &maxlen, &pos);

	ostrcatbig(&buf,"</select></td></tr>", &maxlen, &pos);
	sendstr = ostrcat(sendstr, ", fastzap.value", 1 , 0);

///////////////////
	ostrcatbig(&buf, "<tr><td><font class=\"label\">", &maxlen, &pos);
	ostrcatbig(&buf, _("Faststop"), &maxlen, &pos);
	ostrcatbig(&buf, "&nbsp;</font></td><td><select name=\"faststop\" border=\"0\">", &maxlen, &pos);

	if(getconfigint("faststop", NULL) == 0)
		ostrcatbig(&buf, "<option value=\"0\" selected>", &maxlen, &pos);
	else
		ostrcatbig(&buf, "<option value=\"0\">", &maxlen, &pos);
	ostrcatbig(&buf, _("no"), &maxlen, &pos);
	ostrcatbig(&buf, "</option>", &maxlen, &pos);

	if(getconfigint("faststop", NULL) == 1)
		ostrcatbig(&buf, "<option value=\"1\" selected>", &maxlen, &pos);
	else
		ostrcatbig(&buf, "<option value=\"1\">", &maxlen, &pos);
	ostrcatbig(&buf, _("yes"), &maxlen, &pos);
	ostrcatbig(&buf, "</option>", &maxlen, &pos);

	ostrcatbig(&buf,"</select></td></tr>", &maxlen, &pos);
	sendstr = ostrcat(sendstr, ", faststop.value", 1 , 0);

///////////////////
	ostrcatbig(&buf, "<tr><td><font class=\"label\">", &maxlen, &pos);
	ostrcatbig(&buf, _("Dirsort"), &maxlen, &pos);
	ostrcatbig(&buf, "&nbsp;</font></td><td><select name=\"dirsort\" border=\"0\">", &maxlen, &pos);

	if(getconfigint("dirsort", NULL) == 0)
		ostrcatbig(&buf, "<option value=\"0\" selected>", &maxlen, &pos);
	else
		ostrcatbig(&buf, "<option value=\"0\">", &maxlen, &pos);
	ostrcatbig(&buf, _("alpha"), &maxlen, &pos);
	ostrcatbig(&buf, "</option>", &maxlen, &pos);

	if(getconfigint("dirsort", NULL) == 1)
		ostrcatbig(&buf, "<option value=\"1\" selected>", &maxlen, &pos);
	else
		ostrcatbig(&buf, "<option value=\"1\">", &maxlen, &pos);
	ostrcatbig(&buf, _("reverse alpha"), &maxlen, &pos);
	ostrcatbig(&buf, "</option>", &maxlen, &pos);

	if(getconfigint("dirsort", NULL) == 2)
		ostrcatbig(&buf, "<option value=\"2\" selected>", &maxlen, &pos);
	else
		ostrcatbig(&buf, "<option value=\"2\">", &maxlen, &pos);
	ostrcatbig(&buf, _("size"), &maxlen, &pos);
	ostrcatbig(&buf, "</option>", &maxlen, &pos);

	if(getconfigint("dirsort", NULL) == 3)
		ostrcatbig(&buf, "<option value=\"3\" selected>", &maxlen, &pos);
	else
		ostrcatbig(&buf, "<option value=\"3\">", &maxlen, &pos);
	ostrcatbig(&buf, _("reverse size"), &maxlen, &pos);
	ostrcatbig(&buf, "</option>", &maxlen, &pos);

	if(getconfigint("dirsort", NULL) == 4)
		ostrcatbig(&buf, "<option value=\"4\" selected>", &maxlen, &pos);
	else
		ostrcatbig(&buf, "<option value=\"4\">", &maxlen, &pos);
	ostrcatbig(&buf, _("date"), &maxlen, &pos);
	ostrcatbig(&buf, "</option>", &maxlen, &pos);

	if(getconfigint("dirsort", NULL) == 5)
		ostrcatbig(&buf, "<option value=\"5\" selected>", &maxlen, &pos);
	else
		ostrcatbig(&buf, "<option value=\"5\">", &maxlen, &pos);
	ostrcatbig(&buf, _("reverse date"), &maxlen, &pos);
	ostrcatbig(&buf, "</option>", &maxlen, &pos);

	ostrcatbig(&buf,"</select></td></tr>", &maxlen, &pos);
	sendstr = ostrcat(sendstr, ", dirsort.value", 1 , 0);

///////////////////rcpower
	ostrcatbig(&buf, "<tr><td><font class=\"label\">", &maxlen, &pos);
	ostrcatbig(&buf, _("Remotecontrol Power Aktion"), &maxlen, &pos);
	ostrcatbig(&buf, "&nbsp;</font></td><td><select name=\"poweraktion\" border=\"0\">", &maxlen, &pos);

	if(getconfigint("poweraktion", NULL) == 0)
		ostrcatbig(&buf, "<option value=\"0\" selected>", &maxlen, &pos);
	else
		ostrcatbig(&buf, "<option value=\"0\">", &maxlen, &pos);
	ostrcatbig(&buf, _("Power Menu"), &maxlen, &pos);
	ostrcatbig(&buf, "</option>", &maxlen, &pos);

	if(getconfigint("poweraktion", NULL) == 1)
		ostrcatbig(&buf, "<option value=\"1\" selected>", &maxlen, &pos);
	else
		ostrcatbig(&buf, "<option value=\"1\">", &maxlen, &pos);
	ostrcatbig(&buf, _("Power Off"), &maxlen, &pos);
	ostrcatbig(&buf, "</option>", &maxlen, &pos);

	if(getconfigint("poweraktion", NULL) == 2)
		ostrcatbig(&buf, "<option value=\"2\" selected>", &maxlen, &pos);
	else
		ostrcatbig(&buf, "<option value=\"2\">", &maxlen, &pos);
	ostrcatbig(&buf, _("Standby"), &maxlen, &pos);
	ostrcatbig(&buf, "</option>", &maxlen, &pos);

	if(getconfigint("poweraktion", NULL) == 3)
		ostrcatbig(&buf, "<option value=\"3\" selected>", &maxlen, &pos);
	else
		ostrcatbig(&buf, "<option value=\"3\">", &maxlen, &pos);
	ostrcatbig(&buf, _("Restart"), &maxlen, &pos);
	ostrcatbig(&buf, "</option>", &maxlen, &pos);

	if(getconfigint("poweraktion", NULL) == 4)
		ostrcatbig(&buf, "<option value=\"4\" selected>", &maxlen, &pos);
	else
		ostrcatbig(&buf, "<option value=\"4\">", &maxlen, &pos);
	ostrcatbig(&buf, _("GUI Restart"), &maxlen, &pos);
	ostrcatbig(&buf, "</option>", &maxlen, &pos);

	ostrcatbig(&buf,"</select></td></tr>", &maxlen, &pos);
	sendstr = ostrcat(sendstr, ", poweraktion.value", 1 , 0);

///////////////////frontpower
	ostrcatbig(&buf, "<tr><td><font class=\"label\">", &maxlen, &pos);
	ostrcatbig(&buf, _("Frontpanel Power Aktion"), &maxlen, &pos);
	ostrcatbig(&buf, "&nbsp;</font></td><td><select name=\"frontpoweraktion\" border=\"0\">", &maxlen, &pos);

	if(getconfigint("frontpoweraktion", NULL) == 1)
		ostrcatbig(&buf, "<option value=\"1\" selected>", &maxlen, &pos);
	else
		ostrcatbig(&buf, "<option value=\"1\">", &maxlen, &pos);
	ostrcatbig(&buf, _("Power Off"), &maxlen, &pos);
	ostrcatbig(&buf, "</option>", &maxlen, &pos);

	if(getconfigint("frontpoweraktion", NULL) == 2)
		ostrcatbig(&buf, "<option value=\"2\" selected>", &maxlen, &pos);
	else
		ostrcatbig(&buf, "<option value=\"2\">", &maxlen, &pos);
	ostrcatbig(&buf, _("Standby"), &maxlen, &pos);
	ostrcatbig(&buf, "</option>", &maxlen, &pos);

	ostrcatbig(&buf,"</select></td></tr>", &maxlen, &pos);
	sendstr = ostrcat(sendstr, ", frontpoweraktion.value", 1 , 0);

///////////////////
	ostrcatbig(&buf, "<tr><td><font class=\"label\">", &maxlen, &pos);
	ostrcatbig(&buf, _("Virtualzap Timeout"), &maxlen, &pos);
	ostrcatbig(&buf, "&nbsp;</font></td><td><select name=\"virtualzap\" border=\"0\">", &maxlen, &pos);

	if(getconfigint("virtualzap", NULL) == 0)
		ostrcatbig(&buf, "<option value=\"0\" selected>", &maxlen, &pos);
	else
		ostrcatbig(&buf, "<option value=\"0\">", &maxlen, &pos);
	ostrcatbig(&buf, _("disable"), &maxlen, &pos);
	ostrcatbig(&buf, "</option>", &maxlen, &pos);

	if(getconfigint("virtualzap", NULL) == 1)
		ostrcatbig(&buf, "<option value=\"1\" selected>", &maxlen, &pos);
	else
		ostrcatbig(&buf, "<option value=\"1\">", &maxlen, &pos);
	ostrcatbig(&buf, _("1 sec"), &maxlen, &pos);
	ostrcatbig(&buf, "</option>", &maxlen, &pos);

	if(getconfigint("virtualzap", NULL) == 2)
		ostrcatbig(&buf, "<option value=\"2\" selected>", &maxlen, &pos);
	else
		ostrcatbig(&buf, "<option value=\"2\">", &maxlen, &pos);
	ostrcatbig(&buf, _("2 sec"), &maxlen, &pos);
	ostrcatbig(&buf, "</option>", &maxlen, &pos);

	if(getconfigint("virtualzap", NULL) == 3)
		ostrcatbig(&buf, "<option value=\"3\" selected>", &maxlen, &pos);
	else
		ostrcatbig(&buf, "<option value=\"3\">", &maxlen, &pos);
	ostrcatbig(&buf, _("endless"), &maxlen, &pos);
	ostrcatbig(&buf, "</option>", &maxlen, &pos);

	ostrcatbig(&buf,"</select></td></tr>", &maxlen, &pos);
	sendstr = ostrcat(sendstr, ", virtualzap.value", 1 , 0);

///////////////////
	ostrcatbig(&buf, "<tr><td><font class=\"label\">", &maxlen, &pos);
	ostrcatbig(&buf, _("Fast Text Render"), &maxlen, &pos);
	ostrcatbig(&buf, "&nbsp;</font></td><td><select name=\"fasttextrender\" border=\"0\">", &maxlen, &pos);

	if(getconfigint("fasttextrender", NULL) == 0)
		ostrcatbig(&buf, "<option value=\"0\" selected>", &maxlen, &pos);
	else
		ostrcatbig(&buf, "<option value=\"0\">", &maxlen, &pos);
	ostrcatbig(&buf, _("no"), &maxlen, &pos);
	ostrcatbig(&buf, "</option>", &maxlen, &pos);

	if(getconfigint("fasttextrender", NULL) == 1)
		ostrcatbig(&buf, "<option value=\"1\" selected>", &maxlen, &pos);
	else
		ostrcatbig(&buf, "<option value=\"1\">", &maxlen, &pos);
	ostrcatbig(&buf, _("yes"), &maxlen, &pos);
	ostrcatbig(&buf, "</option>", &maxlen, &pos);

	ostrcatbig(&buf,"</select></td></tr>", &maxlen, &pos);
	sendstr = ostrcat(sendstr, ", fasttextrender.value", 1 , 0);

///////////////////
	if(status.pvr == 1)
	{
		ostrcatbig(&buf, "<tr><td><font class=\"label\">", &maxlen, &pos);
		ostrcatbig(&buf, _("Record Split Size"), &maxlen, &pos);
		ostrcatbig(&buf, "&nbsp;</font></td><td><select name=\"recsplitsize\" border=\"0\">", &maxlen, &pos);

		if(getconfigint("recsplitsize", NULL) == 0)
			ostrcatbig(&buf, "<option value=\"0\" selected>", &maxlen, &pos);
		else
			ostrcatbig(&buf, "<option value=\"0\">", &maxlen, &pos);
		ostrcatbig(&buf, _("disable"), &maxlen, &pos);
		ostrcatbig(&buf, "</option>", &maxlen, &pos);

		if(getconfigint("recsplitsize", NULL) == 1)
			ostrcatbig(&buf, "<option value=\"1\" selected>", &maxlen, &pos);
		else
			ostrcatbig(&buf, "<option value=\"1\">", &maxlen, &pos);
		ostrcatbig(&buf, _("1 GB"), &maxlen, &pos);
		ostrcatbig(&buf, "</option>", &maxlen, &pos);

		if(getconfigint("recsplitsize", NULL) == 2)
			ostrcatbig(&buf, "<option value=\"2\" selected>", &maxlen, &pos);
		else
			ostrcatbig(&buf, "<option value=\"2\">", &maxlen, &pos);
		ostrcatbig(&buf, _("2 GB"), &maxlen, &pos);
		ostrcatbig(&buf, "</option>", &maxlen, &pos);

		if(getconfigint("recsplitsize", NULL) == 3)
			ostrcatbig(&buf, "<option value=\"3\" selected>", &maxlen, &pos);
		else
			ostrcatbig(&buf, "<option value=\"3\">", &maxlen, &pos);
		ostrcatbig(&buf, _("3 GB"), &maxlen, &pos);
		ostrcatbig(&buf, "</option>", &maxlen, &pos);

		if(getconfigint("recsplitsize", NULL) == 4)
			ostrcatbig(&buf, "<option value=\"4\" selected>", &maxlen, &pos);
		else
			ostrcatbig(&buf, "<option value=\"4\">", &maxlen, &pos);
		ostrcatbig(&buf, _("4 GB"), &maxlen, &pos);
		ostrcatbig(&buf, "</option>", &maxlen, &pos);

		ostrcatbig(&buf,"</select></td></tr>", &maxlen, &pos);
		sendstr = ostrcat(sendstr, ", recsplitsize.value", 1 , 0);

///////////////////
		tmpstr = ostrcat("0\n1\n2\n3\n4\n5\n6\n7\n8\n9\n10\n15\n20\n25\n30\n35\n40\n45\n50\n55\n60", NULL, 0, 0);

		ostrcatbig(&buf, "<tr><td><font class=\"label\">", &maxlen, &pos);
		ostrcatbig(&buf, _("Record Forerun (min)"), &maxlen, &pos);
		ostrcatbig(&buf, "&nbsp;</font></td><td><select name=\"recforerun\" border=\"0\">", &maxlen, &pos);

		count = 0;
		ret1 = strsplit(tmpstr, "\n", &count);
		max = count - 1;

		if(ret1 != NULL)
		{
			for(i = 0; i <= max; i++)
			{
				ostrcatbig(&buf, "<option value=\"", &maxlen, &pos);
				ostrcatbig(&buf, ret1[i].part, &maxlen, &pos);
				tmpstr1 = ostrcat(ret1[i].part, NULL, 0, 0);
				if(getconfigint("recforerun", NULL) == atoi(tmpstr1))
					ostrcatbig(&buf, "\" selected>", &maxlen, &pos);
				else
					ostrcatbig(&buf, "\">", &maxlen, &pos);
				ostrcatbig(&buf, ret1[i].part, &maxlen, &pos);
				ostrcatbig(&buf, "</option>", &maxlen, &pos);
			}
		}
		ostrcatbig(&buf,"</select></td></tr>", &maxlen, &pos);
		sendstr = ostrcat(sendstr, ", recforerun.value", 1 , 0);
		free(ret1), ret1 = NULL;
		free(tmpstr), tmpstr = NULL;
		free(tmpstr1), tmpstr1 = NULL;

///////////////////
		tmpstr = ostrcat("0\n1\n2\n3\n4\n5\n6\n7\n8\n9\n10\n15\n20\n25\n30\n35\n40\n45\n50\n55\n60", NULL, 0, 0);

		ostrcatbig(&buf, "<tr><td><font class=\"label\">", &maxlen, &pos);
		ostrcatbig(&buf, _("Record Overrun (min)"), &maxlen, &pos);
		ostrcatbig(&buf, "&nbsp;</font></td><td><select name=\"recoverrun\" border=\"0\">", &maxlen, &pos);

		count = 0;
		ret1 = strsplit(tmpstr, "\n", &count);
		max = count - 1;

		if(ret1 != NULL)
		{
			for(i = 0; i <= max; i++)
			{
				ostrcatbig(&buf, "<option value=\"", &maxlen, &pos);
				ostrcatbig(&buf, ret1[i].part, &maxlen, &pos);
				tmpstr1 = ostrcat(ret1[i].part, NULL, 0, 0);
				if(getconfigint("recoverrun", NULL) == atoi(tmpstr1))
					ostrcatbig(&buf, "\" selected>", &maxlen, &pos);
				else
					ostrcatbig(&buf, "\">", &maxlen, &pos);
				ostrcatbig(&buf, ret1[i].part, &maxlen, &pos);
				ostrcatbig(&buf, "</option>", &maxlen, &pos);
			}
		}
		ostrcatbig(&buf,"</select></td></tr>", &maxlen, &pos);
		sendstr = ostrcat(sendstr, ", recoverrun.value", 1 , 0);
		free(ret1), ret1 = NULL;
		free(tmpstr), tmpstr = NULL;
		free(tmpstr1), tmpstr1 = NULL;
	}
///////////////////
	tmpstr = ostrcat("15\n20\n30\n45\n60\n90\n120\n180\n300\n600\n900\n1200", NULL, 0, 0);

	ostrcatbig(&buf, "<tr><td><font class=\"label\">", &maxlen, &pos);
	ostrcatbig(&buf, _("Skip 1/3"), &maxlen, &pos);
	ostrcatbig(&buf, "&nbsp;</font></td><td><select name=\"skip13\" border=\"0\">", &maxlen, &pos);

	count = 0;
	ret1 = strsplit(tmpstr, "\n", &count);
	max = count - 1;

	if(ret1 != NULL)
	{
		for(i = 0; i <= max; i++)
		{
			ostrcatbig(&buf, "<option value=\"", &maxlen, &pos);
			ostrcatbig(&buf, ret1[i].part, &maxlen, &pos);
			tmpstr1 = ostrcat(ret1[i].part, NULL, 0, 0);
			if(getconfigint("skip13", NULL) == atoi(tmpstr1))
				ostrcatbig(&buf, "\" selected>", &maxlen, &pos);
			else
				ostrcatbig(&buf, "\">", &maxlen, &pos);
			ostrcatbig(&buf, ret1[i].part, &maxlen, &pos);
			ostrcatbig(&buf, "</option>", &maxlen, &pos);
		}
	}
	ostrcatbig(&buf,"</select></td></tr>", &maxlen, &pos);
	sendstr = ostrcat(sendstr, ", skip13.value", 1 , 0);
	free(ret1), ret1 = NULL;
	free(tmpstr), tmpstr = NULL;
	free(tmpstr1), tmpstr1 = NULL;

///////////////////
	tmpstr = ostrcat("15\n20\n30\n45\n60\n90\n120\n180\n300\n600\n900\n1200", NULL, 0, 0);

	ostrcatbig(&buf, "<tr><td><font class=\"label\">", &maxlen, &pos);
	ostrcatbig(&buf, _("Skip 4/6"), &maxlen, &pos);
	ostrcatbig(&buf, "&nbsp;</font></td><td><select name=\"skip46\" border=\"0\">", &maxlen, &pos);

	count = 0;
	ret1 = strsplit(tmpstr, "\n", &count);
	max = count - 1;

	if(ret1 != NULL)
	{
		for(i = 0; i <= max; i++)
		{
			ostrcatbig(&buf, "<option value=\"", &maxlen, &pos);
			ostrcatbig(&buf, ret1[i].part, &maxlen, &pos);
			tmpstr1 = ostrcat(ret1[i].part, NULL, 0, 0);
			if(getconfigint("skip46", NULL) == atoi(tmpstr1))
				ostrcatbig(&buf, "\" selected>", &maxlen, &pos);
			else
				ostrcatbig(&buf, "\">", &maxlen, &pos);
			ostrcatbig(&buf, ret1[i].part, &maxlen, &pos);
			ostrcatbig(&buf, "</option>", &maxlen, &pos);
		}
	}
	ostrcatbig(&buf,"</select></td></tr>", &maxlen, &pos);
	sendstr = ostrcat(sendstr, ", skip46.value", 1 , 0);
	free(ret1), ret1 = NULL;
	free(tmpstr), tmpstr = NULL;
	free(tmpstr1), tmpstr1 = NULL;

///////////////////
	tmpstr = ostrcat("15\n20\n30\n45\n60\n90\n120\n180\n300\n600\n900\n1200", NULL, 0, 0);

	ostrcatbig(&buf, "<tr><td><font class=\"label\">", &maxlen, &pos);
	ostrcatbig(&buf, _("Skip 7/9"), &maxlen, &pos);
	ostrcatbig(&buf, "&nbsp;</font></td><td><select name=\"skip79\" border=\"0\">", &maxlen, &pos);

	count = 0;
	ret1 = strsplit(tmpstr, "\n", &count);
	max = count - 1;

	if(ret1 != NULL)
	{
		for(i = 0; i <= max; i++)
		{
			ostrcatbig(&buf, "<option value=\"", &maxlen, &pos);
			ostrcatbig(&buf, ret1[i].part, &maxlen, &pos);
			tmpstr1 = ostrcat(ret1[i].part, NULL, 0, 0);
			if(getconfigint("skip79", NULL) == atoi(tmpstr1))
				ostrcatbig(&buf, "\" selected>", &maxlen, &pos);
			else
				ostrcatbig(&buf, "\">", &maxlen, &pos);
			ostrcatbig(&buf, ret1[i].part, &maxlen, &pos);
			ostrcatbig(&buf, "</option>", &maxlen, &pos);
		}
	}
	ostrcatbig(&buf,"</select></td></tr>", &maxlen, &pos);
	sendstr = ostrcat(sendstr, ", skip79.value", 1 , 0);
	free(ret1), ret1 = NULL;
	free(tmpstr), tmpstr = NULL;
	free(tmpstr1), tmpstr1 = NULL;

///////////////////
	ostrcatbig(&buf, "<tr><td><font class=\"label\">", &maxlen, &pos);
	ostrcatbig(&buf, _("Player for .ts"), &maxlen, &pos);
	ostrcatbig(&buf, "&nbsp;</font></td><td><select name=\"playertype\" border=\"0\">", &maxlen, &pos);

	if(getconfigint("playertype", NULL) == 0)
		ostrcatbig(&buf, "<option value=\"0\" selected>", &maxlen, &pos);
	else
		ostrcatbig(&buf, "<option value=\"0\">", &maxlen, &pos);
	ostrcatbig(&buf, _("extern"), &maxlen, &pos);
	ostrcatbig(&buf, "</option>", &maxlen, &pos);

	if(getconfigint("faststop", NULL) == 1)
		ostrcatbig(&buf, "<option value=\"1\" selected>", &maxlen, &pos);
	else
		ostrcatbig(&buf, "<option value=\"1\">", &maxlen, &pos);
	ostrcatbig(&buf, _("intern"), &maxlen, &pos);
	ostrcatbig(&buf, "</option>", &maxlen, &pos);

	ostrcatbig(&buf,"</select></td></tr>", &maxlen, &pos);
	sendstr = ostrcat(sendstr, ", playertype.value", 1 , 0);

///////////////////
	ostrcatbig(&buf, "<tr><td><font class=\"label\">", &maxlen, &pos);
	ostrcatbig(&buf, _("Change Channelname auto."), &maxlen, &pos);
	ostrcatbig(&buf, "&nbsp;</font></td><td><select name=\"autochangechannelname\" border=\"0\">", &maxlen, &pos);

	if(getconfigint("autochangechannelname", NULL) == 0)
		ostrcatbig(&buf, "<option value=\"0\" selected>", &maxlen, &pos);
	else
		ostrcatbig(&buf, "<option value=\"0\">", &maxlen, &pos);
	ostrcatbig(&buf, _("no"), &maxlen, &pos);
	ostrcatbig(&buf, "</option>", &maxlen, &pos);

	if(getconfigint("autochangechannelname", NULL) == 1)
		ostrcatbig(&buf, "<option value=\"1\" selected>", &maxlen, &pos);
	else
		ostrcatbig(&buf, "<option value=\"1\">", &maxlen, &pos);
	ostrcatbig(&buf, _("yes"), &maxlen, &pos);
	ostrcatbig(&buf, "</option>", &maxlen, &pos);

	ostrcatbig(&buf,"</select></td></tr>", &maxlen, &pos);
	sendstr = ostrcat(sendstr, ", autochangechannelname.value", 1 , 0);

///////////////////
	if(status.pvr == 1)
	{
		ostrcatbig(&buf, "<tr><td><font class=\"label\">", &maxlen, &pos);
		ostrcatbig(&buf, _("Set action after rec"), &maxlen, &pos);
		ostrcatbig(&buf, "&nbsp;</font></td><td><select name=\"def_rectimer_after\" border=\"0\">", &maxlen, &pos);

		if(getconfigint("def_rectimer_after", NULL) == 0)
			ostrcatbig(&buf, "<option value=\"0\" selected>", &maxlen, &pos);
		else
			ostrcatbig(&buf, "<option value=\"0\">", &maxlen, &pos);
		ostrcatbig(&buf, _("auto"), &maxlen, &pos);
		ostrcatbig(&buf, "</option>", &maxlen, &pos);

		if(getconfigint("def_rectimer_after", NULL) == 1)
			ostrcatbig(&buf, "<option value=\"1\" selected>", &maxlen, &pos);
		else
			ostrcatbig(&buf, "<option value=\"1\">", &maxlen, &pos);
		ostrcatbig(&buf, _("nothing"), &maxlen, &pos);
		ostrcatbig(&buf, "</option>", &maxlen, &pos);

		if(getconfigint("def_rectimer_after", NULL) == 2)
			ostrcatbig(&buf, "<option value=\"2\" selected>", &maxlen, &pos);
		else
			ostrcatbig(&buf, "<option value=\"2\">", &maxlen, &pos);
		ostrcatbig(&buf, _("standby"), &maxlen, &pos);
		ostrcatbig(&buf, "</option>", &maxlen, &pos);

		if(getconfigint("def_rectimer_after", NULL) == 3)
			ostrcatbig(&buf, "<option value=\"3\" selected>", &maxlen, &pos);
		else
			ostrcatbig(&buf, "<option value=\"3\">", &maxlen, &pos);
		ostrcatbig(&buf, _("power off"), &maxlen, &pos);
		ostrcatbig(&buf, "</option>", &maxlen, &pos);

		ostrcatbig(&buf,"</select></td></tr>", &maxlen, &pos);
		sendstr = ostrcat(sendstr, ", def_rectimer_after.value", 1 , 0);
	}
///////////////////
	ostrcatbig(&buf, "<tr><td><font class=\"label\">", &maxlen, &pos);
	ostrcatbig(&buf, _("Show Timeline in Channellist"), &maxlen, &pos);
	ostrcatbig(&buf, "&nbsp;</font></td><td><select name=\"showchanneltimeline\" border=\"0\">", &maxlen, &pos);

	if(getconfigint("showchanneltimeline", NULL) == 0)
		ostrcatbig(&buf, "<option value=\"0\" selected>", &maxlen, &pos);
	else
		ostrcatbig(&buf, "<option value=\"0\">", &maxlen, &pos);
	ostrcatbig(&buf, _("no"), &maxlen, &pos);
	ostrcatbig(&buf, "</option>", &maxlen, &pos);

	if(getconfigint("showchanneltimeline", NULL) == 1)
		ostrcatbig(&buf, "<option value=\"1\" selected>", &maxlen, &pos);
	else
		ostrcatbig(&buf, "<option value=\"1\">", &maxlen, &pos);
	ostrcatbig(&buf, _("yes"), &maxlen, &pos);
	ostrcatbig(&buf, "</option>", &maxlen, &pos);

	ostrcatbig(&buf,"</select></td></tr>", &maxlen, &pos);
	sendstr = ostrcat(sendstr, ", showchanneltimeline.value", 1 , 0);

///////////////////
#ifndef MIPSEL
	ostrcatbig(&buf, "<tr><td><font class=\"label\">", &maxlen, &pos);
	ostrcatbig(&buf, _("Animated Screens"), &maxlen, &pos);
	ostrcatbig(&buf, "&nbsp;</font></td><td><select name=\"screenanim\" border=\"0\">", &maxlen, &pos);

	if(getconfigint("screenanim", NULL) == 0)
		ostrcatbig(&buf, "<option value=\"0\" selected>", &maxlen, &pos);
	else
		ostrcatbig(&buf, "<option value=\"0\">", &maxlen, &pos);
	ostrcatbig(&buf, _("no"), &maxlen, &pos);
	ostrcatbig(&buf, "</option>", &maxlen, &pos);

	if(getconfigint("screenanim", NULL) == 1)
		ostrcatbig(&buf, "<option value=\"1\" selected>", &maxlen, &pos);
	else
		ostrcatbig(&buf, "<option value=\"1\">", &maxlen, &pos);
	ostrcatbig(&buf, _("anim. width"), &maxlen, &pos);
	ostrcatbig(&buf, "</option>", &maxlen, &pos);

	if(getconfigint("screenanim", NULL) == 2)
		ostrcatbig(&buf, "<option value=\"2\" selected>", &maxlen, &pos);
	else
		ostrcatbig(&buf, "<option value=\"2\">", &maxlen, &pos);
	ostrcatbig(&buf, _("anim. height"), &maxlen, &pos);
	ostrcatbig(&buf, "</option>", &maxlen, &pos);

	if(getconfigint("screenanim", NULL) == 3)
		ostrcatbig(&buf, "<option value=\"3\" selected>", &maxlen, &pos);
	else
		ostrcatbig(&buf, "<option value=\"3\">", &maxlen, &pos);
	ostrcatbig(&buf, _("anim. both"), &maxlen, &pos);
	ostrcatbig(&buf, "</option>", &maxlen, &pos);

	ostrcatbig(&buf,"</select></td></tr>", &maxlen, &pos);
	sendstr = ostrcat(sendstr, ", screenanim.value", 1 , 0);

///////////////////
	ostrcatbig(&buf, "<tr><td><font class=\"label\">", &maxlen, &pos);
	ostrcatbig(&buf, _("Animated Speed"), &maxlen, &pos);
	ostrcatbig(&buf, "&nbsp;</font></td><td><select name=\"screenanimspeed\" border=\"0\">", &maxlen, &pos);

	if(getconfigint("screenanimspeed", NULL) == 1)
		ostrcatbig(&buf, "<option value=\"1\" selected>", &maxlen, &pos);
	else
		ostrcatbig(&buf, "<option value=\"1\">", &maxlen, &pos);
	ostrcatbig(&buf, _("very fast"), &maxlen, &pos);
	ostrcatbig(&buf, "</option>", &maxlen, &pos);

	if(getconfigint("screenanimspeed", NULL) == 5)
		ostrcatbig(&buf, "<option value=\"5\" selected>", &maxlen, &pos);
	else
		ostrcatbig(&buf, "<option value=\"5\">", &maxlen, &pos);
	ostrcatbig(&buf, _("fast"), &maxlen, &pos);
	ostrcatbig(&buf, "</option>", &maxlen, &pos);

	if(getconfigint("screenanimspeed", NULL) == 10)
		ostrcatbig(&buf, "<option value=\"10\" selected>", &maxlen, &pos);
	else
		ostrcatbig(&buf, "<option value=\"10\">", &maxlen, &pos);
	ostrcatbig(&buf, _("normal"), &maxlen, &pos);
	ostrcatbig(&buf, "</option>", &maxlen, &pos);

	if(getconfigint("screenanimspeed", NULL) == 15)
		ostrcatbig(&buf, "<option value=\"15\" selected>", &maxlen, &pos);
	else
		ostrcatbig(&buf, "<option value=\"15\">", &maxlen, &pos);
	ostrcatbig(&buf, _("slow"), &maxlen, &pos);
	ostrcatbig(&buf, "</option>", &maxlen, &pos);

	if(getconfigint("screenanimspeed", NULL) == 20)
		ostrcatbig(&buf, "<option value=\"20\" selected>", &maxlen, &pos);
	else
		ostrcatbig(&buf, "<option value=\"20\">", &maxlen, &pos);
	ostrcatbig(&buf, _("very slow"), &maxlen, &pos);
	ostrcatbig(&buf, "</option>", &maxlen, &pos);

	ostrcatbig(&buf,"</select></td></tr>", &maxlen, &pos);
	sendstr = ostrcat(sendstr, ", screenanimspeed.value", 1 , 0);
#endif
///////////////////
	ostrcatbig(&buf, "<tr><td><font class=\"label\">", &maxlen, &pos);
	ostrcatbig(&buf, _("Channellist view"), &maxlen, &pos);
	ostrcatbig(&buf, "&nbsp;</font></td><td><select name=\"channellistview\" border=\"0\">", &maxlen, &pos);

	if(getconfigint("channellistview", NULL) == 0)
		ostrcatbig(&buf, "<option value=\"0\" selected>", &maxlen, &pos);
	else
		ostrcatbig(&buf, "<option value=\"0\">", &maxlen, &pos);
	ostrcatbig(&buf, _("hidden"), &maxlen, &pos);
	ostrcatbig(&buf, "</option>", &maxlen, &pos);

	if(getconfigint("channellistview", NULL) == 1)
		ostrcatbig(&buf, "<option value=\"1\" selected>", &maxlen, &pos);
	else
		ostrcatbig(&buf, "<option value=\"1\">", &maxlen, &pos);
	ostrcatbig(&buf, _("deaktiv"), &maxlen, &pos);
	ostrcatbig(&buf, "</option>", &maxlen, &pos);

	ostrcatbig(&buf,"</select></td></tr>", &maxlen, &pos);
	sendstr = ostrcat(sendstr, ", channellistview.value", 1 , 0);

///////////////////
	ostrcatbig(&buf, "<tr><td><font class=\"label\">", &maxlen, &pos);
	ostrcatbig(&buf, _("Show last pos Question"), &maxlen, &pos);
	ostrcatbig(&buf, "&nbsp;</font></td><td><select name=\"showlastpos\" border=\"0\">", &maxlen, &pos);

	if(getconfigint("showlastpos", NULL) == 0)
		ostrcatbig(&buf, "<option value=\"0\" selected>", &maxlen, &pos);
	else
		ostrcatbig(&buf, "<option value=\"0\">", &maxlen, &pos);
	ostrcatbig(&buf, _("no"), &maxlen, &pos);
	ostrcatbig(&buf, "</option>", &maxlen, &pos);

	if(getconfigint("showlastpos", NULL) == 1)
		ostrcatbig(&buf, "<option value=\"1\" selected>", &maxlen, &pos);
	else
		ostrcatbig(&buf, "<option value=\"1\">", &maxlen, &pos);
	ostrcatbig(&buf, _("yes"), &maxlen, &pos);
	ostrcatbig(&buf, "</option>", &maxlen, &pos);

	ostrcatbig(&buf,"</select></td></tr>", &maxlen, &pos);
	sendstr = ostrcat(sendstr, ", showlastpos.value", 1 , 0);

///////////////////
	if(status.pvr == 1)
	{
		ostrcatbig(&buf, "<tr><td><font class=\"label\">", &maxlen, &pos);
		ostrcatbig(&buf, _("Resync Recording immediately"), &maxlen, &pos);
		ostrcatbig(&buf, "&nbsp;</font></td><td><select name=\"recsync\" border=\"0\">", &maxlen, &pos);

		if(getconfigint("recsync", NULL) == 0)
			ostrcatbig(&buf, "<option value=\"0\" selected>", &maxlen, &pos);
		else
			ostrcatbig(&buf, "<option value=\"0\">", &maxlen, &pos);
		ostrcatbig(&buf, _("no"), &maxlen, &pos);
		ostrcatbig(&buf, "</option>", &maxlen, &pos);

		if(getconfigint("recsync", NULL) == 1)
			ostrcatbig(&buf, "<option value=\"1\" selected>", &maxlen, &pos);
		else
			ostrcatbig(&buf, "<option value=\"1\">", &maxlen, &pos);
		ostrcatbig(&buf, _("yes"), &maxlen, &pos);
		ostrcatbig(&buf, "</option>", &maxlen, &pos);

		ostrcatbig(&buf,"</select></td></tr>", &maxlen, &pos);
		sendstr = ostrcat(sendstr, ", recsync.value", 1 , 0);
///////////////////
		ostrcatbig(&buf, "<tr><td><font class=\"label\">", &maxlen, &pos);
		ostrcatbig(&buf, _("Recording name"), &maxlen, &pos);
		ostrcatbig(&buf, "&nbsp;</font></td><td><select name=\"recordnamefmt\" border=\"0\">", &maxlen, &pos);

		if(getconfigint("recordnamefmt", NULL) == 0)
			ostrcatbig(&buf, "<option value=\"0\" selected>", &maxlen, &pos);
		else
			ostrcatbig(&buf, "<option value=\"0\">", &maxlen, &pos);
		ostrcatbig(&buf, _("channel-movie"), &maxlen, &pos);
		ostrcatbig(&buf, "</option>", &maxlen, &pos);

		if(getconfigint("recordnamefmt", NULL) == 1)
			ostrcatbig(&buf, "<option value=\"1\" selected>", &maxlen, &pos);
		else
			ostrcatbig(&buf, "<option value=\"1\">", &maxlen, &pos);
		ostrcatbig(&buf, _("movie-channel"), &maxlen, &pos);
		ostrcatbig(&buf, "</option>", &maxlen, &pos);

		ostrcatbig(&buf,"</select></td></tr>", &maxlen, &pos);
		sendstr = ostrcat(sendstr, ", recordnamefmt.value", 1 , 0);
	}
///////////////////
	ostrcatbig(&buf, "<tr><td><font class=\"label\">", &maxlen, &pos);
	ostrcatbig(&buf, _("Newsletter"), &maxlen, &pos);
	ostrcatbig(&buf, "&nbsp;</font></td><td><select name=\"newsletter\" border=\"0\">", &maxlen, &pos);

	if(getconfigint("newsletter", NULL) == 0)
		ostrcatbig(&buf, "<option value=\"0\" selected>", &maxlen, &pos);
	else
		ostrcatbig(&buf, "<option value=\"0\">", &maxlen, &pos);
	ostrcatbig(&buf, _("no"), &maxlen, &pos);
	ostrcatbig(&buf, "</option>", &maxlen, &pos);

	if(getconfigint("newsletter", NULL) == 1)
		ostrcatbig(&buf, "<option value=\"1\" selected>", &maxlen, &pos);
	else
		ostrcatbig(&buf, "<option value=\"1\">", &maxlen, &pos);
	ostrcatbig(&buf, _("yes"), &maxlen, &pos);
	ostrcatbig(&buf, "</option>", &maxlen, &pos);

	ostrcatbig(&buf,"</select></td></tr>", &maxlen, &pos);
	sendstr = ostrcat(sendstr, ", newsletter.value", 1 , 0);

///////////////////
	ostrcatbig(&buf, "<tr><td><font class=\"label\">", &maxlen, &pos);
	ostrcatbig(&buf, _("Show hidden files"), &maxlen, &pos);
	ostrcatbig(&buf, "&nbsp;</font></td><td><select name=\"showhiddenfiles\" border=\"0\">", &maxlen, &pos);

	if(getconfigint("showhiddenfiles", NULL) == 0)
		ostrcatbig(&buf, "<option value=\"0\" selected>", &maxlen, &pos);
	else
		ostrcatbig(&buf, "<option value=\"0\">", &maxlen, &pos);
	ostrcatbig(&buf, _("no"), &maxlen, &pos);
	ostrcatbig(&buf, "</option>", &maxlen, &pos);

	if(getconfigint("showhiddenfiles", NULL) == 1)
		ostrcatbig(&buf, "<option value=\"1\" selected>", &maxlen, &pos);
	else
		ostrcatbig(&buf, "<option value=\"1\">", &maxlen, &pos);
	ostrcatbig(&buf, _("yes"), &maxlen, &pos);
	ostrcatbig(&buf, "</option>", &maxlen, &pos);

	ostrcatbig(&buf,"</select></td></tr>", &maxlen, &pos);
	sendstr = ostrcat(sendstr, ", showhiddenfiles.value", 1 , 0);

///////////////////
	ostrcatbig(&buf, "<tr><td><font class=\"label\">", &maxlen, &pos);
	ostrcatbig(&buf, _("Expert Modus"), &maxlen, &pos);
	ostrcatbig(&buf, "&nbsp;</font></td><td><select name=\"expertmodus\" border=\"0\">", &maxlen, &pos);

	if(getconfigint("expertmodus", NULL) == 0)
		ostrcatbig(&buf, "<option value=\"0\" selected>", &maxlen, &pos);
	else
		ostrcatbig(&buf, "<option value=\"0\">", &maxlen, &pos);
	ostrcatbig(&buf, _("no"), &maxlen, &pos);
	ostrcatbig(&buf, "</option>", &maxlen, &pos);

	if(getconfigint("expertmodus", NULL) == 10)
		ostrcatbig(&buf, "<option value=\"10\" selected>", &maxlen, &pos);
	else
		ostrcatbig(&buf, "<option value=\"10\">", &maxlen, &pos);
	ostrcatbig(&buf, _("yes"), &maxlen, &pos);
	ostrcatbig(&buf, "</option>", &maxlen, &pos);

	if(getconfigint("expertmodus", NULL) == 11)
		ostrcatbig(&buf, "<option value=\"11\" selected>", &maxlen, &pos);
	else
		ostrcatbig(&buf, "<option value=\"11\">", &maxlen, &pos);
	ostrcatbig(&buf, _("expert (11)"), &maxlen, &pos);
	ostrcatbig(&buf, "</option>", &maxlen, &pos);

	ostrcatbig(&buf,"</select></td></tr>", &maxlen, &pos);
	sendstr = ostrcat(sendstr, ", expertmodus.value", 1 , 0);

///////////////////
	ostrcatbig(&buf, "<tr><td><font class=\"label\">", &maxlen, &pos);
	ostrcatbig(&buf, _("Show infobar on program change"), &maxlen, &pos);
	ostrcatbig(&buf, "&nbsp;</font></td><td><select name=\"infobarprogram\" border=\"0\">", &maxlen, &pos);

	if(getconfigint("infobarprogram", NULL) == 0)
		ostrcatbig(&buf, "<option value=\"0\" selected>", &maxlen, &pos);
	else
		ostrcatbig(&buf, "<option value=\"0\">", &maxlen, &pos);
	ostrcatbig(&buf, _("no"), &maxlen, &pos);
	ostrcatbig(&buf, "</option>", &maxlen, &pos);

	if(getconfigint("infobarprogram", NULL) == 1)
		ostrcatbig(&buf, "<option value=\"1\" selected>", &maxlen, &pos);
	else
		ostrcatbig(&buf, "<option value=\"1\">", &maxlen, &pos);
	ostrcatbig(&buf, _("yes"), &maxlen, &pos);
	ostrcatbig(&buf, "</option>", &maxlen, &pos);

	ostrcatbig(&buf,"</select></td></tr>", &maxlen, &pos);
	sendstr = ostrcat(sendstr, ", infobarprogram.value", 1 , 0);

///////////////////
	ostrcatbig(&buf, "<tr><td><font class=\"label\">", &maxlen, &pos);
	ostrcatbig(&buf, _("Use cross control for fast zapping and vol"), &maxlen, &pos);
	ostrcatbig(&buf, "&nbsp;</font></td><td><select name=\"crosscontrol\" border=\"0\">", &maxlen, &pos);

	if(getconfigint("crosscontrol", NULL) == 0)
		ostrcatbig(&buf, "<option value=\"0\" selected>", &maxlen, &pos);
	else
		ostrcatbig(&buf, "<option value=\"0\">", &maxlen, &pos);
	ostrcatbig(&buf, _("no"), &maxlen, &pos);
	ostrcatbig(&buf, "</option>", &maxlen, &pos);

	if(getconfigint("crosscontrol", NULL) == 1)
		ostrcatbig(&buf, "<option value=\"1\" selected>", &maxlen, &pos);
	else
		ostrcatbig(&buf, "<option value=\"1\">", &maxlen, &pos);
	ostrcatbig(&buf, _("yes"), &maxlen, &pos);
	ostrcatbig(&buf, "</option>", &maxlen, &pos);

	if(getconfigint("crosscontrol", NULL) == 2)
		ostrcatbig(&buf, "<option value=\"2\" selected>", &maxlen, &pos);
	else
		ostrcatbig(&buf, "<option value=\"2\">", &maxlen, &pos);
	ostrcatbig(&buf, _("yes / vzap 1 sec"), &maxlen, &pos);
	ostrcatbig(&buf, "</option>", &maxlen, &pos);

	if(getconfigint("crosscontrol", NULL) == 3)
		ostrcatbig(&buf, "<option value=\"3\" selected>", &maxlen, &pos);
	else
		ostrcatbig(&buf, "<option value=\"3\">", &maxlen, &pos);
	ostrcatbig(&buf, _("yes / vzap 2 sec"), &maxlen, &pos);
	ostrcatbig(&buf, "</option>", &maxlen, &pos);

	if(getconfigint("crosscontrol", NULL) == 9999)
		ostrcatbig(&buf, "<option value=\"9999\" selected>", &maxlen, &pos);
	else
		ostrcatbig(&buf, "<option value=\"9999\">", &maxlen, &pos);
	ostrcatbig(&buf, _("yes / vzap endless"), &maxlen, &pos);
	ostrcatbig(&buf, "</option>", &maxlen, &pos);

	ostrcatbig(&buf,"</select></td></tr>", &maxlen, &pos);
	sendstr = ostrcat(sendstr, ", crosscontrol.value", 1 , 0);

///////////////////
	if(status.security >= 1)
	{
		ostrcatbig(&buf, "<tr><td><font class=\"label\">", &maxlen, &pos);
		ostrcatbig(&buf, _("Deactivate Crypt Support on Media Playback"), &maxlen, &pos);
		ostrcatbig(&buf, "&nbsp;</font></td><td><select name=\"emucontrol\" border=\"0\">", &maxlen, &pos);

		if(getconfigint("emucontrol", NULL) == 0)
			ostrcatbig(&buf, "<option value=\"0\" selected>", &maxlen, &pos);
		else
			ostrcatbig(&buf, "<option value=\"0\">", &maxlen, &pos);
		ostrcatbig(&buf, _("no"), &maxlen, &pos);
		ostrcatbig(&buf, "</option>", &maxlen, &pos);

		if(getconfigint("emucontrol", NULL) == 1)
			ostrcatbig(&buf, "<option value=\"1\" selected>", &maxlen, &pos);
		else
			ostrcatbig(&buf, "<option value=\"1\">", &maxlen, &pos);
		ostrcatbig(&buf, _("yes"), &maxlen, &pos);
		ostrcatbig(&buf, "</option>", &maxlen, &pos);

		ostrcatbig(&buf,"</select></td></tr>", &maxlen, &pos);
		sendstr = ostrcat(sendstr, ", emucontrol.value", 1 , 0);
	}

///////////////////
	ostrcatbig(&buf, "<tr><td><font class=\"label\">", &maxlen, &pos);
	ostrcatbig(&buf, _("Deactivate MiniTV"), &maxlen, &pos);
	ostrcatbig(&buf, "&nbsp;</font></td><td><select name=\"choiceminitv\" border=\"0\">", &maxlen, &pos);

	if(getconfigint("choiceminitv", NULL) == 0)
		ostrcatbig(&buf, "<option value=\"0\" selected>", &maxlen, &pos);
	else
		ostrcatbig(&buf, "<option value=\"0\">", &maxlen, &pos);
	ostrcatbig(&buf, _("no"), &maxlen, &pos);
	ostrcatbig(&buf, "</option>", &maxlen, &pos);

	if(getconfigint("choiceminitv", NULL) == 1)
		ostrcatbig(&buf, "<option value=\"1\" selected>", &maxlen, &pos);
	else
		ostrcatbig(&buf, "<option value=\"1\">", &maxlen, &pos);
	ostrcatbig(&buf, _("yes"), &maxlen, &pos);
	ostrcatbig(&buf, "</option>", &maxlen, &pos);

	ostrcatbig(&buf,"</select></td></tr>", &maxlen, &pos);
	sendstr = ostrcat(sendstr, ", choiceminitv.value", 1 , 0);

///////////////////
	if(checkbox("ATEMIO510") == 1 || checkbox("UFS912") == 1 || checkbox("ATEMIO7600") == 1 || checkbox("ATEMIO520") == 1 || checkbox("ATEMIO530") == 1 || checkbox("SPARK") == 1 || checkbox("SPARK7162") == 1)
	{
		ostrcatbig(&buf, "<tr><td><font class=\"label\">", &maxlen, &pos);
		ostrcatbig(&buf, _("Activate CEC"), &maxlen, &pos);
		ostrcatbig(&buf, "&nbsp;</font></td><td><select name=\"usecec\" border=\"0\">", &maxlen, &pos);

		if(getconfigint("usecec", NULL) == 0)
			ostrcatbig(&buf, "<option value=\"0\" selected>", &maxlen, &pos);
		else
			ostrcatbig(&buf, "<option value=\"0\">", &maxlen, &pos);
		ostrcatbig(&buf, _("no"), &maxlen, &pos);
		ostrcatbig(&buf, "</option>", &maxlen, &pos);

		if(getconfigint("usecec", NULL) == 1)
			ostrcatbig(&buf, "<option value=\"1\" selected>", &maxlen, &pos);
		else
			ostrcatbig(&buf, "<option value=\"1\">", &maxlen, &pos);
		ostrcatbig(&buf, _("yes"), &maxlen, &pos);
		ostrcatbig(&buf, "</option>", &maxlen, &pos);

		ostrcatbig(&buf,"</select></td></tr>", &maxlen, &pos);
		sendstr = ostrcat(sendstr, ", usecec.value", 1 , 0);
	}

///////////////////
	if(checkbox("ATEMIO7600") == 1)
	{
		ostrcatbig(&buf, "<tr><td><font class=\"label\">", &maxlen, &pos);
		ostrcatbig(&buf, _("SATA Connector"), &maxlen, &pos);
		ostrcatbig(&buf, "&nbsp;</font></td><td><select name=\"sataswitch\" border=\"0\">", &maxlen, &pos);

		if(getconfigint("sataswitch", NULL) == 0)
			ostrcatbig(&buf, "<option value=\"0\" selected>", &maxlen, &pos);
		else
			ostrcatbig(&buf, "<option value=\"0\">", &maxlen, &pos);
		ostrcatbig(&buf, _("extern"), &maxlen, &pos);
		ostrcatbig(&buf, "</option>", &maxlen, &pos);

		if(getconfigint("sataswitch", NULL) == 1)
			ostrcatbig(&buf, "<option value=\"1\" selected>", &maxlen, &pos);
		else
			ostrcatbig(&buf, "<option value=\"1\">", &maxlen, &pos);
		ostrcatbig(&buf, _("intern"), &maxlen, &pos);
		ostrcatbig(&buf, "</option>", &maxlen, &pos);

		ostrcatbig(&buf,"</select></td></tr>", &maxlen, &pos);
		sendstr = ostrcat(sendstr, ", sataswitch.value", 1 , 0);
	}

///////////////////
//	ostrcatbig(&buf, "<tr><td><font class=\"label\">Volbartimeout:&nbsp;</font></td><td><select name=\"volbartimeout\" border=\"0\"><option value=\"1\" selected>1</option><option value=\"2\">2</option><option value=\"3\">3</option><option value=\"4\">4</option><option value=\"5\">5</option><option value=\"6\">6</option><option value=\"7\">7</option><option value=\"8\">8</option><option value=\"9\">9</option><option value=\"10\">10</option></select></td></tr>", &maxlen, &pos);
	max = 10;
	ostrcatbig(&buf, "<tr><td><font class=\"label\">", &maxlen, &pos);
	ostrcatbig(&buf, _("Time to wait after seek for fill buffer"), &maxlen, &pos);
	ostrcatbig(&buf, "&nbsp;</font></td><td><select name=\"playerbufferseektime\" border=\"0\">", &maxlen, &pos);
	for(i = 1; i <= max; i++)
	{
		ostrcatbig(&buf, "<option value=\"", &maxlen, &pos);
		ostrcatbig(&buf, oitoa(i), &maxlen, &pos);
		if(getconfigint("playerbufferseektime", NULL) == i)
			ostrcatbig(&buf, "\" selected>", &maxlen, &pos);
		else
			ostrcatbig(&buf, "\">", &maxlen, &pos);
		ostrcatbig(&buf, oitoa(i), &maxlen, &pos);
		ostrcatbig(&buf, "</option>", &maxlen, &pos);
	}
	ostrcatbig(&buf, "</select></td></tr>", &maxlen, &pos);
	sendstr = ostrcat(sendstr, ", playerbufferseektime.value", 1 , 0);

///////////////////debuglevel
#ifdef BETA
	char* tmpstr2 = NULL;
	struct splitstr* ret2 = NULL;
	tmpstr = ostrcat("10\n40\n50\n55\n60\n70\n77\n80\n81\n90\n99\n100\n110\n130\n133\n150\n200\n201\n250\n270\n278\n300\n307\n333\n400\n401\n427\n444\n500\n555\n620\n777\n900\n913\n975\n4440", NULL, 0, 0);
	tmpstr2 = ostrcat(" - Minimal\n - MTD Operation/System Update\n - MediaCenter\n - Network Interface\n - Key Actions\n - Network Browser\n - Web Adjust\n - Harddisk\n - MultiImage\n - Screensaver\n - Tithek/HTTP Header/Community\n - Global\n - Stringconvert\n - TPK\n - MediaDB/IMDb/TMDb\n - ePlayer\n - DVB Devices\n - CA Device\n - HTTP Server/Stream/Record\n - Radiotext\n - ID3\n - Subtitle\n - Thumbnails\n - DVD Player\n - EPG Task\n - Old Entry\n - Newsletter\n - Framebuffer\n - Channel Scan\n - Draw Screen\n - CA\n - MediaDB\n - Mutex\n - File Operation\n - SMTP\n - PiP", NULL, 0, 0);

	ostrcatbig(&buf, "<tr><td><font class=\"label\">", &maxlen, &pos);
	ostrcatbig(&buf, _("Debuglevel"), &maxlen, &pos);
	ostrcatbig(&buf, "&nbsp;</font></td><td><select name=\"debuglevel\" border=\"0\">", &maxlen, &pos);

	count = 0;
	ret1 = strsplit(tmpstr, "\n", &count);
	count = 0;
	ret2 = strsplit(tmpstr2, "\n", &count);
	max = count - 1;

	if(ret1 != NULL)
	{
		for(i = 0; i <= max; i++)
		{
			ostrcatbig(&buf, "<option value=\"", &maxlen, &pos);
			ostrcatbig(&buf, ret1[i].part, &maxlen, &pos);
			tmpstr1 = ostrcat(ret1[i].part, NULL, 0, 0);
			if(getconfigint("debuglevel", NULL) == atoi(tmpstr1))
				ostrcatbig(&buf, "\" selected>", &maxlen, &pos);
			else
				ostrcatbig(&buf, "\">", &maxlen, &pos);
			ostrcatbig(&buf, ret1[i].part, &maxlen, &pos);
			ostrcatbig(&buf, ret2[i].part, &maxlen, &pos);
			ostrcatbig(&buf, "</option>", &maxlen, &pos);
		}
	}
	ostrcatbig(&buf,"</select></td></tr>", &maxlen, &pos);
	sendstr = ostrcat(sendstr, ", debuglevel.value", 1 , 0);
	free(ret1), ret1 = NULL;
	free(ret2), ret2 = NULL;
	free(tmpstr), tmpstr = NULL;
	free(tmpstr1), tmpstr1 = NULL;
	free(tmpstr2), tmpstr2 = NULL;
#endif

///////////////////
#ifdef MIPSEL
	if(file_exist("/usr/bin/enigma2"))
	{
		ostrcatbig(&buf, "<tr><td><font class=\"label\">", &maxlen, &pos);
		ostrcatbig(&buf, _("Dualboot Menu TitanNit / E2"), &maxlen, &pos);
		ostrcatbig(&buf, "&nbsp;</font></td><td><select name=\"dualboot\" border=\"0\">", &maxlen, &pos);

	//		if(getconfigint("dualboot", NULL) == 0)
		if(!file_exist("/mnt/config/dualboot"))
			ostrcatbig(&buf, "<option value=\"0\" selected>", &maxlen, &pos);
		else
			ostrcatbig(&buf, "<option value=\"0\">", &maxlen, &pos);
		ostrcatbig(&buf, _("no"), &maxlen, &pos);
		ostrcatbig(&buf, "</option>", &maxlen, &pos);

	//		if(getconfigint("dualboot", NULL) == 1)
		if(file_exist("/mnt/config/dualboot"))
			ostrcatbig(&buf, "<option value=\"1\" selected>", &maxlen, &pos);
		else
			ostrcatbig(&buf, "<option value=\"1\">", &maxlen, &pos);
		ostrcatbig(&buf, _("yes"), &maxlen, &pos);
		ostrcatbig(&buf, "</option>", &maxlen, &pos);

		ostrcatbig(&buf,"</select></td></tr>", &maxlen, &pos);
		sendstr = ostrcat(sendstr, ", dualboot.value", 1 , 0);
	}
#endif


//	ostrcatbig(&buf, "</table><br><br><input class=button type=submit name=send value=\"Send\" onClick=\"return checkdaytime(begin.value, end.value)\"></input>&nbsp;<input class=button type=reset name=reset value=\"Reset\"></input></form></center></body></html>", &maxlen, &pos);
//	ostrcatbig(&buf, "</table><br><br><input class=button type=submit name=send value=\"Send\" onClick=\"return volbartimeout.value, infobartimeout.value, infobarsleep.value, spinnerspeed.value, spinnertime.value, hangtime.value, secondinfobar.value, nozapclear.value, fastzap.value, faststop.value, dirsort.value, poweraktion.value, virtualzap.value, fasttextrender.value, recsplitsize.value, recforerun.value, recoverrun.value, skip13.value, skip46.value, skip79.value, playertype.value, autochangechannelname.value, def_rectimer_after.value, showchanneltimeline.value, screenanim.value, screenanimspeed.value, channellistview.value, showlastpos.value, recsync.value, recordnamefmt.value, newsletter.value, showhiddenfiles.value, expertmodus.value, infobarprogram.value, emucontrol.value, choiceminitv.value, usecec.value, playerbufferseektime.value, dualboot.value\"></input>&nbsp;<input class=button type=reset name=reset value=\"Reset\"></input></form></center></body></html>", &maxlen, &pos);
// sendstr verwenden
	ostrcatbig(&buf, "</table><br><br><input class=button type=submit name=send value=\"Send\" onClick=\"return ", &maxlen, &pos);
	ostrcatbig(&buf, sendstr, &maxlen, &pos);
	ostrcatbig(&buf, "\"></input>&nbsp;<input class=button type=reset name=reset value=\"Reset\"></input></form></center></body></html>", &maxlen, &pos);


//writesys("/tmp/tmpstr", buf, 1);
	//ostrcatbig(&buf, param, &maxlen, &pos);
	return buf;
}

char* webadjustsend(char* param, int fmt)
{
	debug(77, "fmt: %d param: %s",fmt , param);

	char* buf = NULL;
	char* tmpstr = NULL, *tmpstr1 = NULL;

	int count, count2,  max, i;

	tmpstr = ostrcat(param, NULL, 0, 0);

	count = 0;
	struct splitstr* ret1 = NULL;
	ret1 = strsplit(tmpstr, "&", &count);
	max = count - 1;

	if(ret1 != NULL)
	{
		for(i = 0; i <= max; i++)
		{
			if(ostrcmp(ret1[i].part, "node=0") == 0 || ostrcmp(ret1[i].part, "send=Send") == 0)
			{
				debug(77, "skip: %s", ret1[i].part);
				continue;
			}
			tmpstr1 = ostrcat(ret1[i].part, NULL, 0, 0);
			count2 = 0;
			struct splitstr* ret2 = NULL;
			ret2 = strsplit(tmpstr1, "=", &count2);
			if(ret2 != NULL && count2 > 1)
			{
				if(ret2[1].part != NULL && ostrcmp(ret2[0].part, "dualboot") == 0)
				{
					debug(77, "add %s: %s", ret2[0].part, ret2[1].part);
					if(ostrcmp(ret2[1].part, "0") == 0)
					{
						debug(77, "unlink /mnt/config/dualboot");
						unlink("/mnt/config/dualboot");
					}
					else
					{
						debug(77, "touch /mnt/config/dualboot");
						system("touch /mnt/config/dualboot");
					}
				}
				else if(ret2[1].part != NULL)
				{
					debug(77, "add %s: %s", ret2[0].part, ret2[1].part);
					addconfigtmp(ret2[0].part, ret2[1].part);
				}

			}
			free(ret2), ret2 = NULL;
			free(tmpstr1), tmpstr1 = NULL;
		}
	}
	free(ret1), ret1 = NULL;
	free(tmpstr), tmpstr = NULL;

	buf = webadjust(NULL, fmt);
	writeconfigtmp();
	writeallconfig(1);
#ifdef BETA
setdebuglevel();
#endif
	return buf;
}

char* webgetparamvalue(char* param, char* searchparam)
{
	char* buf = NULL;
	char* tmpstr = NULL;

	tmpstr = ostrcat(param, NULL, 0, 0);
	if(tmpstr != NULL) buf = ostrstr(tmpstr, searchparam);
	if(buf != NULL) buf = buf + strlen(searchparam);
	free(tmpstr); tmpstr = NULL;
	buf = stringreplacecharonce(buf, '&', '\0');
	debug(77, "webgetparamvalue param: %s", param);
	debug(77, "webgetparamvalue search: %s", searchparam);
	debug(77, "webgetparamvalue value: %s", buf);
	return buf;
}

char* webgetsystem(char* param, int fmt)
{
	char* buf = NULL;
	int ret = 0;
	if(param == NULL) return NULL;
	if(status.security == 0) return NULL;

//	if(fmt == 0)
//	{
		buf = ostrcat(buf, "<html><head><meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\">", 1, 0);
		buf = ostrcat(buf, "<link rel=stylesheet type=text/css href=titan.css><script type=text/javascript src=titan.js></script>", 1, 0);
		buf = ostrcat(buf, "</head><body class=body id=\"system\"><center>", 1, 0);
		buf = ostrcat(buf, "<h1>", 1, 0);
//	}

	debug(10, "cmd: %s", param);
	ret = system(param);

	if(ret != 0)
		buf = ostrcat(buf, _("Error, Program not started."), 1, 0);
	else
		buf = ostrcat(buf, _("Program started successfully"), 1, 0);
	buf = ostrcat(buf, "<br>", 1, 0);

	buf = string_replace_all("<br>", "<br>\n", buf, 1);

//	if(fmt == 0)
		buf = ostrcat(buf, "</h1></center></body></html>", 1, 0);

	return buf;
}

char* webgethelp(char* param, int fmt)
{
	char* buf = NULL, *tmpstr = NULL, *tmpstr1 = NULL, *helppath = NULL, *lang = NULL;
	if(param == NULL) return NULL;

//	if(fmt == 0)
//	{
		buf = ostrcat(buf, "<html><head><meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\">", 1, 0);
		buf = ostrcat(buf, "<link rel=stylesheet type=text/css href=titan.css><script type=text/javascript src=titan.js></script>", 1, 0);
		buf = ostrcat(buf, "</head><body class=body id=\"help\"><center>", 1, 0);
		buf = ostrcat(buf, "<br>", 1, 0);
		buf = ostrcat(buf, "<h1>", 1, 0);
		buf = ostrcat(buf, _(param), 1, 0);
		buf = ostrcat(buf, "</h1>", 1, 0);
		buf = ostrcat(buf, "<br>", 1, 0);
//	}

	helppath = getconfig("helppath", NULL);
	if(helppath == NULL) return NULL;

	//get language
	lang = ostrcat(getconfig("lang", NULL), NULL, 0, 0);
	lang = string_replace("po/", "", lang, 1);

	//create full filename
	tmpstr = ostrcat(helppath, "/", 0, 0);
	tmpstr = ostrcat(tmpstr, lang, 1, 0);
	tmpstr = ostrcat(tmpstr, "/", 1, 0);
	tmpstr = ostrcat(tmpstr, param, 1, 0);
	tmpstr = ostrcat(tmpstr, ".txt", 1, 0);

	debug(10, "file: %s", tmpstr);
	tmpstr1 = readfiletomem(tmpstr, 1);

	buf = ostrcat(buf, _(tmpstr1), 1, 0);
	buf = string_replace_all("\n", "<br>", buf, 1);

// segfault ??
//	free(helppath), helppath = NULL;

	free(lang), lang = NULL;
	free(tmpstr), tmpstr = NULL;
	free(tmpstr1), tmpstr1 = NULL;
	buf = string_replace_all("\n", "<br>", buf, 1);

//	if(fmt == 0)
		buf = ostrcat(buf, "</center></body></html>", 1, 0);

	return buf;
}

char* webgethelpchoices(int fmt)
{
	char* buf = NULL, *tmpstr = NULL, *helppath = NULL, *lang = NULL, *cmd = NULL;

//	if(fmt == 0)
//	{
		buf = ostrcat(buf, "<html><head><meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\">", 1, 0);
		buf = ostrcat(buf, "<link rel=stylesheet type=text/css href=titan.css><script type=text/javascript src=titan.js></script>", 1, 0);
		buf = ostrcat(buf, "</head><body class=body id=\"helpchoices\"><center>", 1, 0);
		buf = ostrcat(buf, "<br>", 1, 0);
		buf = ostrcat(buf, "<h1>", 1, 0);
		buf = ostrcat(buf, _("Help"), 1, 0);
		buf = ostrcat(buf, "</h1>", 1, 0);
		buf = ostrcat(buf, "<br>", 1, 0);
//	}

	helppath = getconfig("helppath", NULL);
	if(helppath == NULL) return NULL;

	//get language
	lang = ostrcat(getconfig("lang", NULL), NULL, 0, 0);
	lang = string_replace("po/", "", lang, 1);

	//create full filename
	cmd = ostrcat("ls -1 ", helppath, 0, 0);
	cmd = ostrcat(cmd, "/", 1, 0);
	cmd = ostrcat(cmd, lang, 1, 1);
	cmd = ostrcat(cmd, "/", 1, 0);

	debug(10, "cmd: %s", cmd);
	tmpstr = command(cmd);

	int count, i, max;
	count = 0;
	struct splitstr* ret1 = NULL;
	ret1 = strsplit(tmpstr, "\n", &count);
	max = count - 1;

	buf = ostrcat(buf, "<table cellpadding=5 cellspacing=5><tr><td nowrap>", 1, 0);

	if(ret1 != NULL)
	{
		for(i = 0; i <= max; i++)
		{
			buf = ostrcat(buf, "<a class=linelink2 href=queryraw?gethelp&", 1, 0);
			buf = ostrcat(buf, stringreplacecharonce(ret1[i].part, '.', '\0'), 1, 0);
			buf = ostrcat(buf, " target=main>", 1, 0);
			buf = ostrcat(buf, _(stringreplacecharonce(ret1[i].part, '.', '\0')), 1, 0);
			buf = ostrcat(buf, "</a>", 1, 0);
			buf = ostrcat(buf, "</br></br>", 1, 0);
		}
	}

// segfault ??
//	free(helppath), helppath = NULL;
	free(ret1), ret1 = NULL;

	free(tmpstr), tmpstr = NULL;
	buf = string_replace_all("<br>", "<br>\n", buf, 1);

//	if(fmt == 0)
		buf = ostrcat(buf, "</td></tr></table></center></body></html>", 1, 0);

	return buf;
}

char* webgettestpage(char* param, int fmt)
{
	char* buf = NULL, *tmpstr = NULL;
	if(param == NULL) return NULL;
	if(status.security == 0) return NULL;

//	if(fmt == 0)
//	{
		buf = ostrcat(buf, "<html><head><meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\">", 1, 0);
		buf = ostrcat(buf, "<link rel=stylesheet type=text/css href=titan.css><script type=text/javascript src=titan.js></script>", 1, 0);
		buf = ostrcat(buf, "</head><body class=body id=\"help\"><center>", 1, 0);
//	}

	debug(10, "file: %s", param);
	tmpstr = readfiletomem(param, 1);

	buf = ostrcat(buf, tmpstr, 1, 0);

	free(tmpstr), tmpstr = NULL;
	buf = string_replace_all("<br>", "<br>\n", buf, 1);

//	if(fmt == 0)
		buf = ostrcat(buf, "</center></body></html>", 1, 0);

	debug(10, "buf: %s", buf);

	return buf;
}

char* webgetsysteminfos(char* param, int fmt)
{
	char* buf = NULL, *tmpstr = NULL;
	int mode;

	mode = atoi(param);
	if(fmt == 0)
	{
		buf = ostrcat(buf, "<html><head><meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\">", 1, 0);
		buf = ostrcat(buf, "<link rel=stylesheet type=text/css href=titan.css><script type=text/javascript src=titan.js></script>", 1, 0);
		buf = ostrcat(buf, "</head><body class=body id=\"systeminfos\"><center>", 1, 0);
		buf = ostrcat(buf, "<br>", 1, 0);
		buf = ostrcat(buf, "<h1>", 1, 0);
		if(mode == 0)
			buf = ostrcat(buf, _("System Status"), 1, 0);
		else if(mode == 1)
			buf = ostrcat(buf, _("Free Space"), 1, 0);
		else if(mode == 2)
			buf = ostrcat(buf, _("Kernel"), 1, 0);
		else if(mode == 3)
			buf = ostrcat(buf, _("Mounts"), 1, 0);
		else if(mode == 4)
			buf = ostrcat(buf, _("Network"), 1, 0);
		else if(mode == 5)
			buf = ostrcat(buf, _("Ram"), 1, 0);

		buf = ostrcat(buf, "</h1>", 1, 0);
		buf = ostrcat(buf, "<br>", 1, 0);
	}

	tmpstr = system_infos(mode);
	tmpstr = string_replace_all("\n", "<br>", tmpstr, 1);

	buf = ostrcat(buf, tmpstr, 1, 0);

	free(tmpstr), tmpstr = NULL;
	buf = string_replace_all("<br>", "<br>\n", buf, 1);

	if(fmt == 0)
	{
		buf = ostrcat(buf, "</center></body></html>", 1, 0);
	}

	return buf;
}

char* webgetsysinfos(char* param, int fmt)
{
	char* buf = NULL, *tmpstr = NULL;
	int mode;

	mode = atoi(param);
	if(fmt == 0)
	{
		buf = ostrcat(buf, "<html><head><meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\">", 1, 0);
		buf = ostrcat(buf, "<link rel=stylesheet type=text/css href=titan.css><script type=text/javascript src=titan.js></script>", 1, 0);
		buf = ostrcat(buf, "</head><body class=body id=\"sysinfos\"><center>", 1, 0);
		buf = ostrcat(buf, "<br>", 1, 0);
		buf = ostrcat(buf, "<h1>", 1, 0);
		if(mode == 0)
			buf = ostrcat(buf, _("CPU"), 1, 0);
		else if(mode == 1)
			buf = ostrcat(buf, _("Memory"), 1, 0);
		else if(mode == 2)
			buf = ostrcat(buf, _("MTD"), 1, 0);
		else if(mode == 3)
			buf = ostrcat(buf, _("Module"), 1, 0);
		else if(mode == 4)
			buf = ostrcat(buf, _("Devices"), 1, 0);
		else if(mode == 5)
			buf = ostrcat(buf, _("Swap"), 1, 0);
		else if(mode == 6)
			buf = ostrcat(buf, _("Top"), 1, 0);
		else if(mode == 7)
			buf = ostrcat(buf, _("Prozesslist"), 1, 0);
		else if(mode == 8)
			buf = ostrcat(buf, _("USB"), 1, 0);

		buf = ostrcat(buf, "</h1>", 1, 0);
		buf = ostrcat(buf, "<br>", 1, 0);
	}

	tmpstr = system_infos_sysinfo(mode);
	tmpstr = string_replace_all("\n", "<br>", tmpstr, 1);

	buf = ostrcat(buf, tmpstr, 1, 0);

	free(tmpstr), tmpstr = NULL;
	buf = string_replace_all("<br>", "<br>\n", buf, 1);

	if(fmt == 0)
	{
		buf = ostrcat(buf, "</center></body></html>", 1, 0);
	}

	return buf;
}

char* webgetlogs(char* param, int fmt)
{
	char* buf = NULL, *tmpstr = NULL;
	int mode;

	mode = atoi(param);
	if(fmt == 0)
	{
		buf = ostrcat(buf, "<html><head><meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\">", 1, 0);
		buf = ostrcat(buf, "<link rel=stylesheet type=text/css href=titan.css><script type=text/javascript src=titan.js></script>", 1, 0);
		buf = ostrcat(buf, "</head><body class=body id=\"logs\"><center>", 1, 0);
		buf = ostrcat(buf, "<br>", 1, 0);
		buf = ostrcat(buf, "<h1>", 1, 0);
		if(mode == 0)
			buf = ostrcat(buf, _("Hotline"), 1, 0);
		else if(mode == 1)
			buf = ostrcat(buf, _("Svn Changelog"), 1, 0);
		else if(mode == 2)
			buf = ostrcat(buf, _("Git Changelog"), 1, 0);
		else if(mode == 3)
			buf = ostrcat(buf, _("Titan Log"), 1, 0);
		else if(mode == 4)
			buf = ostrcat(buf, _("Svn Changelog Full"), 1, 0);
		else if(mode == 5)
			buf = ostrcat(buf, _("Git Changelog Full"), 1, 0);

		buf = ostrcat(buf, "</h1>", 1, 0);
		buf = ostrcat(buf, "<br>", 1, 0);
	}

	tmpstr = system_logs(mode);
	tmpstr = string_replace_all("\n", "<br>", tmpstr, 1);

	buf = ostrcat(buf, tmpstr, 1, 0);

	free(tmpstr), tmpstr = NULL;
	buf = string_replace_all("<br>", "<br>\n", buf, 1);

	if(fmt == 0)
	{
		buf = ostrcat(buf, "</center></body></html>", 1, 0);
	}

	return buf;
}

char* webgetabout(int fmt)
{
	char* buf = NULL, *tmpstr = NULL;

	if(fmt == 0)
	{
		buf = ostrcat(buf, "<html><head><meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\">", 1, 0);
		buf = ostrcat(buf, "<link rel=stylesheet type=text/css href=titan.css><script type=text/javascript src=titan.js></script>", 1, 0);
		buf = ostrcat(buf, "</head><body class=body id=\"about\"><center>", 1, 0);
		buf = ostrcat(buf, "<br>", 1, 0);
		buf = ostrcat(buf, "<h1>", 1, 0);
		buf = ostrcat(buf, _("About"), 1, 0);
		buf = ostrcat(buf, "</h1>", 1, 0);
		buf = ostrcat(buf, "<br>", 1, 0);
	}

	tmpstr = getabout();
	tmpstr = string_replace_all("\n", "<br>", tmpstr, 1);

	buf = ostrcat(buf, tmpstr, 1, 0);

	free(tmpstr), tmpstr = NULL;
	buf = string_replace_all("<br>", "<br>\n", buf, 1);

	if(fmt == 0)
	{
		buf = ostrcat(buf, "</center></body></html>", 1, 0);
	}

	return buf;
}

char* webgetnewsletterchoices(int fmt)
{
	char* buf = NULL;

	if(fmt == 0)
	{
		buf = ostrcat(buf, "<html><head><meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\">", 1, 0);
		buf = ostrcat(buf, "<link rel=stylesheet type=text/css href=titan.css><script type=text/javascript src=titan.js></script>", 1, 0);
		buf = ostrcat(buf, "</head><body class=body id=\"newsletterchoices\"><center>", 1, 0);
		buf = ostrcat(buf, "<br>", 1, 0);
		buf = ostrcat(buf, "<h1>", 1, 0);
		buf = ostrcat(buf, _("Newsletter"), 1, 0);
		buf = ostrcat(buf, "</h1>", 1, 0);
		buf = ostrcat(buf, "<br>", 1, 0);
	}

	buf = ostrcat(buf, "<table cellpadding=5 cellspacing=5><tr><td nowrap>", 1, 0);

	struct newsletter* node = NULL;

	m_lock(&status.newslettermutex, 19);
	long unsigned lastnewsletter = getconfiglu("lastnewsletter", NULL);

	readnewsletter();
	node = newsletter;

	while(node != NULL)
	{
		buf = ostrcat(buf, "<a class=linelink2 href=queryraw?getnewsletter&", 1, 0);
		buf = ostrcat(buf, oitoa(node->nr), 1, 0);
		buf = ostrcat(buf, " target=main>", 1, 0);
		buf = ostrcat(buf, node->title, 1, 0);
		buf = ostrcat(buf, " - ", 1, 0);
		buf = ostrcat(buf, node->date, 1, 0);
		buf = ostrcat(buf, "</a>", 1, 0);
		buf = ostrcat(buf, "</br></br>", 1, 0);

		if(node->nr > lastnewsletter)
			addconfiglu("lastnewsletter", node->nr);

		node = node->next;
	}

	freenewsletter();
	m_unlock(&status.newslettermutex, 19);

	buf = string_replace_all("<br>", "<br>\n", buf, 1);

	if(fmt == 0)
		buf = ostrcat(buf, "</td></tr></table></center></body></html>", 1, 0);

	return buf;
}

char* webgetnewsletter(char* param, int fmt)
{
	char* buf = NULL, *tmpstr = NULL, *tmpstr1 = NULL;

//	if(fmt == 0)
//	{
		buf = ostrcat(buf, "<html><head><meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\">", 1, 0);
		buf = ostrcat(buf, "<link rel=stylesheet type=text/css href=titan.css><script type=text/javascript src=titan.js></script>", 1, 0);
		buf = ostrcat(buf, "</head><body class=body id=\"newsletter\"><center>", 1, 0);
//	}

	struct newsletter* node = NULL;

	m_lock(&status.newslettermutex, 19);

	readnewsletter();
	node = newsletter;

	while(node != NULL)
	{
		if(ostrcmp(oitoa(node->nr), param) == 0)
		{
			tmpstr1 = ostrcat(node->title, " - ", 0, 0);
			tmpstr1 = ostrcat(tmpstr1, node->date, 1, 0);
			tmpstr = ostrcat(node->text, "\n", 0, 0);
			break;
		}
		node = node->next;
	}

//	if(fmt == 0)
//	{
		buf = ostrcat(buf, "<br>", 1, 0);
		buf = ostrcat(buf, "<h1>", 1, 0);
		buf = ostrcat(buf, tmpstr1, 1, 0);
		buf = ostrcat(buf, "</h1>", 1, 0);
		buf = ostrcat(buf, "<br>", 1, 0);
//	}


	freenewsletter();
	m_unlock(&status.newslettermutex, 19);

	tmpstr = string_replace_all("\n", "<br>\n", tmpstr, 1);

	buf = ostrcat(buf, tmpstr, 1, 0);

	free(tmpstr), tmpstr = NULL;
	free(tmpstr1), tmpstr1 = NULL;
	buf = string_replace_all("<br>", "<br>\n", buf, 1);

//	if(fmt == 0)
		buf = ostrcat(buf, "</center></body></html>", 1, 0);

	return buf;
}

char* webgetstreamingchoices(int fmt)
{
	char* buf = NULL, *tmpstr = NULL;

	if(fmt == 0)
	{
		buf = ostrcat(buf, "<html><head><meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\">", 1, 0);
		buf = ostrcat(buf, "<link rel=stylesheet type=text/css href=titan.css><script type=text/javascript src=titan.js></script>", 1, 0);
		buf = ostrcat(buf, "</head><body class=body id=\"streamingchoices\"><center>", 1, 0);
		buf = ostrcat(buf, "<br>", 1, 0);
		buf = ostrcat(buf, "<h1>", 1, 0);
		buf = ostrcat(buf, _("Streaming"), 1, 0);
		buf = ostrcat(buf, "</h1>", 1, 0);
		buf = ostrcat(buf, "<br>", 1, 0);
	}

	int count = 0;
	struct service* servicenode = service;

	while(servicenode != NULL)
	{
		if(servicenode->type == RECORDSTREAM && servicenode->recname != NULL)
		{
			buf = ostrcat(buf, "<a class=linelink2 href=queryraw?getstreaming&", 1, 0);
			tmpstr = htmlencode(servicenode->recname);
			buf = ostrcat(buf, tmpstr, 1, 0);
			buf = ostrcat(buf, " target=main>", 1, 0);
			buf = ostrcat(buf, _("stop"), 1, 0);
			buf = ostrcat(buf, " - ", 1, 0);
			buf = ostrcat(buf, servicenode->recname, 1, 0);
			buf = ostrcat(buf, " (", 1, 0);
			if(servicenode->channel != NULL && servicenode->channel->name != NULL)
				buf = ostrcat(buf, servicenode->channel->name, 1, 0);
			else
				buf = ostrcat(buf, _("unknown"), 1, 0);
			buf = ostrcat(buf, ")", 1, 0);
			buf = ostrcat(buf, "</a>", 1, 0);
			buf = ostrcat(buf, "</br></br>", 1, 0);
			count++;
			free(tmpstr), tmpstr = NULL;
		}
		servicenode = servicenode->next;
	}

	if(count == 0)
		buf = ostrcat(buf, _("No Live Stream running"), 1, 0);

	buf = string_replace_all("<br>", "<br>\n", buf, 1);

	if(fmt == 0)
		buf = ostrcat(buf, "</center></body></html>", 1, 0);

	return buf;
}

char* webgetstreaming(char* param, int fmt)
{
	char* buf = NULL, *tmpstr = NULL;

//	if(fmt == 0)
//	{
		buf = ostrcat(buf, "<html><head><meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\">", 1, 0);
		buf = ostrcat(buf, "<link rel=stylesheet type=text/css href=titan.css><script type=text/javascript src=titan.js></script>", 1, 0);
		buf = ostrcat(buf, "</head><body class=body id=\"streaming\"><center>", 1, 0);
//	}

	htmldecode(param, param);

	struct service* servicenode = service;
	while(servicenode != NULL)
	{
		if(servicenode->recname != NULL && ostrcmp(servicenode->recname, param) == 0)
		{
			tmpstr = ostrcat(tmpstr, _("stop"), 1, 0);
			tmpstr = ostrcat(tmpstr, " ", 1, 0);
			tmpstr = ostrcat(tmpstr, servicenode->recname, 1, 0);
			tmpstr = ostrcat(tmpstr, " (", 1, 0);
			if(servicenode->channel != NULL && servicenode->channel->name != NULL)
				tmpstr = ostrcat(tmpstr, servicenode->channel->name, 1, 0);
			else
				tmpstr = ostrcat(tmpstr, _("unknown"), 1, 0);
			tmpstr = ostrcat(tmpstr, ")", 1, 0);
			break;
		}
		servicenode = servicenode->next;
	}

//	if(fmt == 0)
//	{
		buf = ostrcat(buf, "<br>", 1, 0);
		buf = ostrcat(buf, "<h1>", 1, 0);
		buf = ostrcat(buf, tmpstr, 1, 0);
		buf = ostrcat(buf, "</h1>", 1, 0);
		buf = ostrcat(buf, "<br>", 1, 0);
//	}

	servicenode = getrecordbyname(param, RECORDSTREAM);
	if(servicenode != NULL)
	{
		servicenode->recendtime = 1;
		buf = ostrcat(buf, _("Streaming succesfull stopped."), 1, 0);
	}
	else
		buf = ostrcat(buf, _("ERROR, Streaming can not be stopped."), 1, 0);

	buf = string_replace_all("<br>", "<br>\n", buf, 1);
	free(tmpstr) , tmpstr = NULL;

//	if(fmt == 0)
		buf = ostrcat(buf, "</center></body></html>", 1, 0);

	return buf;
}

char* webgetupdatelist(char* param, int fmt)
{
	if(status.security == 0) return NULL;

	char* buf = NULL, *tmpstr = NULL, *tmpstr1 = NULL, *cmd = NULL;
	int mode;

	mode = atoi(param);

	struct update* node = createupdatelist(mode);

	if(fmt == 0)
	{
		buf = ostrcat(buf, "<html><head><meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\">", 1, 0);
		buf = ostrcat(buf, "<link rel=stylesheet type=text/css href=titan.css><script type=text/javascript src=titan.js></script>", 1, 0);
		buf = ostrcat(buf, "</head><body class=body id=\"updatelist\">", 1, 0);
		buf = ostrcat(buf, "<br><center>", 1, 0);
		buf = ostrcat(buf, "<h1>", 1, 0);
		buf = ostrcat(buf, _("System Update"), 1, 0);
		buf = ostrcat(buf, "</h1>", 1, 0);
		buf = ostrcat(buf, "<br>", 1, 0);
	}

	tmpstr = getimgnamereal();
	if(tmpstr != NULL)
	{
		buf = ostrcat(buf, _("Installed:"), 1, 0);
		buf = ostrcat(buf, " ", 1, 0);
		buf = ostrcat(buf, tmpstr, 1, 1);
		buf = ostrcat(buf, "<br><br>", 1, 0);
	}
#ifdef OEBUILD
	if(mode == 0)
		cmd = ostrcat("ls -1 /tmp/online | grep '.nfi\\|.zip\\|.img\\|.tar.xz' | sort -r", NULL, 0, 0);
	else
	{
		if(file_exist("/var/backup"))
			cmd = ostrcat("ls -1 /var/backup/ | grep '.nfi\\|.zip\\|.img\\|.tar.xz' | sort -r", NULL, 0, 0);
		else
			cmd = ostrcat("ls -1 /tmp | grep '.nfi\\|.zip\\|.img\\|.tar.xz' | sort -r", NULL, 0, 0);
	}
	printf("cmd: %s\n", cmd);
	tmpstr = command(cmd);
	printf("tmpstr: %s\n", tmpstr);

	free(cmd), cmd = NULL;
#else
	if(checkrealbox("DM7020HD") == 1 || checkbox("DM7020HDV2") == 1)
	{
		if(mode == 0)
			tmpstr = command("ls -1 /tmp/online | grep .nfi | sort -r");
		else
		{
			if(file_exist("/var/backup"))
				tmpstr = command("ls -1 /var/backup/ | grep .nfi | sort -r");
			else
				tmpstr = command("ls -1 /tmp | grep .nfi | sort -r");
		}
	}
	else if(checkbox("DM900") == 1 || checkbox("DM920") == 1 || checkbox("DM520") == 1 || checkbox("DM525") == 1 || checkbox("WETEK") == 1)
	{
		if(mode == 0)
			tmpstr = command("ls -1 /tmp/online | grep *.zip | sort -r");
		else
		{
			if(file_exist("/var/backup"))
				tmpstr = command("ls -1 /var/backup/ | grep *.zip | sort -r");
			else
				tmpstr = command("ls -1 /tmp | grep *.zip | sort -r");
		}
	}
	else
	{
		if(mode == 0)
			tmpstr = command("ls -1 /tmp/online | grep .img | sort -r");
		else
		{
			if(file_exist("/var/backup"))
				tmpstr = command("ls -1 /var/backup/ | grep .img | sort -r");
			else
				tmpstr = command("ls -1 /tmp | grep .img | sort -r");
		}
	}
#endif
	int count, i, max;
	count = 0;
	struct splitstr* ret1 = NULL;
	ret1 = strsplit(tmpstr, "\n", &count);
	max = count - 1;

	buf = ostrcat(buf, "<table cellpadding=5 cellspacing=5><tr><td nowrap>", 1, 0);

	if(ret1 != NULL)
	{
		for(i = 0; i <= max; i++)
		{
			buf = ostrcat(buf, "<a class=linelink2 href=queryraw?getupdate&", 1, 0);

			if(ostrstr(ret1[i].part, "_FULL_") != NULL || ostrstr(ret1[i].part, "_FULLBACKUP.") != NULL || ostrstr(ret1[i].part, "_UPDATENFI_") != NULL || ostrstr(ret1[i].part, "_UPDATEUSB_") != NULL)

#ifdef OEBUILD
			if(ostrstr(ret1[i].part, "_FULL_") != NULL || ostrstr(ret1[i].part, "_FULLBACKUP.") != NULL || ostrstr(ret1[i].part, "_UPDATENFI_") != NULL || ostrstr(ret1[i].part, "_UPDATEUSB_") != NULL || ostrstr(ret1[i].part, ".zip") != NULL || ostrstr(ret1[i].part, ".nfi") != NULL || ostrstr(ret1[i].part, ".tar.gz") != NULL)
#else
			if(ostrstr(ret1[i].part, "_FULL_") != NULL || ostrstr(ret1[i].part, "_FULLBACKUP.") != NULL || ostrstr(ret1[i].part, "_UPDATENFI_") != NULL || ostrstr(ret1[i].part, "_UPDATEUSB_") != NULL)
#endif

			{
//				if(checkrealbox("DM900") == 1)
//					cmd = ostrcat(cmd, "/sbin/update2.sh ", 1, 0);
//				else
					cmd = ostrcat(cmd, "/sbin/update.sh ", 1, 0);
				cmd = ostrcat(cmd, node->type, 1, 0);
				cmd = ostrcat(cmd, " ", 1, 0);

				if(ostrstr(ret1[i].part, "_FULL_") != NULL)
					cmd = ostrcat(cmd, "full ", 1, 0);
#ifdef OEBUILD
				else if(ostrstr(ret1[i].part, ".zip") != NULL)
					cmd = ostrcat(cmd, "updateusb ", 1, 0);
				else if(ostrstr(ret1[i].part, ".nfi") != NULL)
					cmd = ostrcat(cmd, "updateusb ", 1, 0);
				else if(ostrstr(ret1[i].part, ".tar.gz") != NULL)
					cmd = ostrcat(cmd, "updateusb ", 1, 0);
#endif
				else if(ostrstr(ret1[i].part, "_FULLBACKUP.") != NULL)
					cmd = ostrcat(cmd, "fullbackup ", 1, 0);
				else if(ostrstr(ret1[i].part, "_UPDATENFI_") != NULL)
					cmd = ostrcat(cmd, "updatenfi ", 1, 0);
				else if(ostrstr(ret1[i].part, "_UPDATEUSB_") != NULL)
					cmd = ostrcat(cmd, "updateusb ", 1, 0);
				else if(ostrstr(ret1[i].part, ".zip") != NULL)
					cmd = ostrcat(cmd, "full ", 1, 0);

				if(mode == 0)
					cmd = ostrcat(cmd, "/tmp/online/", 1, 0);
				else
					cmd = ostrcat(cmd, "/tmp/", 1, 0);
				cmd = ostrcat(cmd, ret1[i].part, 1, 0);

				cmd = ostrcat(cmd, node->auth, 1, 0);
#ifdef OEBUILD
				if(node->imgtype == 1)
					cmd = ostrcat(cmd, " dev openaaf.dyndns.tv", 1, 0);
				else
					cmd = ostrcat(cmd, " release openaaf.dyndns.tv", 1, 0);

#else
				if(node->imgtype == 1)
					cmd = ostrcat(cmd, " dev beta.dyndns.tv", 1, 0);
				else
					cmd = ostrcat(cmd, " release atemio.dyndns.tv", 1, 0);
#endif
#ifdef OEBUILD
				if(file_exist("/media/hdd"))
				{
					if(!file_exist("/media/hdd/logs"))
						 mkdir("/media/hdd/logs", 777);

					if(file_exist("/media/hdd/logs"))
					{
						cmd = ostrcat(cmd, " > /media/hdd/logs/update_", 1, 0);		
						cmd = ostrcat(cmd, getboxtype(), 1, 0);
						cmd = ostrcat(cmd, "_debug.log 2>&1", 1, 0);		
					}
				}
				else if(file_exist("/var/backup"))
				{
					if(!file_exist("/var/backup/logs"))
						 mkdir("/var/backup/logs", 777);
				
					if(file_exist("/var/backup/logs"))
					{
						cmd = ostrcat(cmd, " > /var/backup/logs/update_", 1, 0);		
						cmd = ostrcat(cmd, getboxtype(), 1, 0);
						cmd = ostrcat(cmd, "_debug.log 2>&1", 1, 0);		
					}
				}
				else if(file_exist("/var/swap"))
				{
					if(!file_exist("/var/swap/logs"))
						 mkdir("/var/swap/logs", 777);
				
					if(file_exist("/var/swap/logs"))
					{
						cmd = ostrcat(cmd, " > /var/swap/logs/update_", 1, 0);		
						cmd = ostrcat(cmd, getboxtype(), 1, 0);
						cmd = ostrcat(cmd, "_debug.log 2>&1", 1, 0);		
					}
				}
#else
				if(file_exist("/var/swap"))
				{
					if(!file_exist("/var/swap/logs"))
						 mkdir("/var/swap/logs", 777);

					if(file_exist("/etc/.beta") && file_exist("/var/swap/logs"))
						cmd = ostrcat(cmd, " > /var/swap/logs/update_debug.log 2>&1", 1, 0);
				}
				else if(checkbox("ATEMIO510") != 1 && checkbox("UFS910") != 1 && checkbox("UFS922") != 1 && checkbox("ATEVIO700") != 1 && checkbox("ATEVIO7000") != 1 && checkbox("IPBOX91") != 1 && checkbox("IPBOX900") != 1 && checkbox("IPBOX910") != 1 && checkbox("IPBOX9000") != 1)
				{
					if(!file_exist("/mnt/logs"))
						 mkdir("/mnt/logs", 777);

					if(file_exist("/etc/.beta") && file_exist("/mnt/logs"))
						cmd = ostrcat(cmd, " > /mnt/logs/update_debug.log 2>&1", 1, 0);
				}
#endif
				tmpstr1 = htmlencode(cmd);
				buf = ostrcat(buf, tmpstr1, 1, 0);
				free(tmpstr1), tmpstr1 = NULL;
				free(cmd), cmd = NULL;
			}

			buf = ostrcat(buf, " target=main>", 1, 0);
//			buf = ostrcat(buf, _(stringreplacecharonce(ret1[i].part, '.', '\0')), 1, 0);
			buf = ostrcat(buf, ret1[i].part, 1, 0);
			buf = ostrcat(buf, "</a>", 1, 0);
			buf = ostrcat(buf, "</br></br>", 1, 0);
		}
	}

	free(tmpstr), tmpstr = NULL;
	free(ret1), ret1 = NULL;

	buf = string_replace_all("<br>", "<br>\n", buf, 1);

	if(fmt == 0)
		buf = ostrcat(buf, "</td></tr></table></center></body></html>", 1, 0);

// needed ??
//	freeupdatelist(node);

	return buf;
}

char* webgetupdate(char* param, int fmt)
{
	if(status.security == 0) return NULL;

	char* buf = NULL, *cmd = NULL;
	printf("param: %s\n",param);

//	if(fmt == 0)
//	{
		buf = ostrcat(buf, "<html><head><meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\">", 1, 0);
		buf = ostrcat(buf, "<link rel=stylesheet type=text/css href=titan.css><script type=text/javascript src=titan.js></script>", 1, 0);
		buf = ostrcat(buf, "</head><body class=body id=\"updatelist\"><center>", 1, 0);
		buf = ostrcat(buf, "<br>", 1, 0);
		buf = ostrcat(buf, "<h1 id=\"updatetitle\">", 1, 0);
		buf = ostrcat(buf, _("System Update"), 1, 0);
		buf = ostrcat(buf, "</h1>", 1, 0);
		buf = ostrcat(buf, "<br>", 1, 0);
//	}

	writeallconfig(1);

	cmd = htmlencode(param);

	buf = ostrcat(buf, "<title>", 1, 0);
	buf = ostrcat(buf, _("Message"), 1, 0);
	buf = ostrcat(buf, "</title>\n", 1, 0);
	buf = ostrcat(buf, " <script type=\"text/javascript\">\n", 1, 0);
	buf = ostrcat(buf, "var bRepeat=confirm(\"", 1, 0);
	buf = ostrcat(buf, _("starting Full Update ?"), 1, 0);
	buf = ostrcat(buf, "\");\n", 1, 0);
	buf = ostrcat(buf, "if(bRepeat==true)\n", 1, 0);
	buf = ostrcat(buf, "{\n", 1, 0);
	buf = ostrcat(buf, "document.getElementById(\"updatetitle\").innerHTML = \"", 1, 0);
	buf = ostrcat(buf, _("System Update started. This can take up to 5 minutes. More details on TV."), 1, 0);
	buf = ostrcat(buf, "\";\n", 1, 0);
	buf = ostrcat(buf, "window.location = \"../queryraw?getsystem&", 1, 0);
	buf = ostrcat(buf, cmd, 1, 0);
	buf = ostrcat(buf, "\";\n", 1, 0);
	buf = ostrcat(buf, "}\n", 1, 0);
	buf = ostrcat(buf, "else\n", 1, 0);
	buf = ostrcat(buf, "alert(\"exit\");\n", 1, 0);
	buf = ostrcat(buf, "</script>\n", 1, 0);

	buf = string_replace_all("<br>", "<br>\n", buf, 1);

	free(cmd), cmd = NULL;

//	if(fmt == 0)
		buf = ostrcat(buf, "</center></body></html>", 1, 0);

	return buf;
}

char* webgettpksection(int fmt)
{
	if(status.security == 0) return NULL;

	char* buf = NULL;

	if(fmt == 0)
	{
		buf = ostrcat(buf, "<html><head><meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\">", 1, 0);
		buf = ostrcat(buf, "<link rel=stylesheet type=text/css href=titan.css><script type=text/javascript src=titan.js></script>", 1, 0);
		buf = ostrcat(buf, "</head><body class=body id=\"tpksection\"><center>", 1, 0);
		buf = ostrcat(buf, "<br>", 1, 0);
		buf = ostrcat(buf, "<h1>", 1, 0);
		buf = ostrcat(buf, _("Tpk Install - select section"), 1, 0);
		buf = ostrcat(buf, "</h1>", 1, 0);
		buf = ostrcat(buf, "<br>", 1, 0);
	}

	status.hangtime = 99999;

	unlink(TPKLOG);
	tpkgetindex(0);
	tpklist();

	struct tpk* node = tpk;

	if(node == NULL) buf = ostrcat(buf, _("No Tpk Sections Found."), 1, 0);

	while(node != NULL)
	{
		if(findsectiondone(node->section) == 1)
		{
			node = node->next;
			continue;
		}

		node->done = 1;

		buf = ostrcat(buf, "<a class=linelink2 href=queryraw?gettpklist&", 1, 0);
		buf = ostrcat(buf, node->section, 1, 0);
		buf = ostrcat(buf, " target=main>", 1, 0);
		buf = ostrcat(buf, _(node->section), 1, 0);
		buf = ostrcat(buf, "</a>", 1, 0);
		buf = ostrcat(buf, "</br></br>", 1, 0);

		node = node->next;
	}

	freetpk();

	buf = string_replace_all("<br>", "<br>\n", buf, 1);

	if(fmt == 0)
	{
		buf = ostrcat(buf, "</center></body></html>", 1, 0);
	}

	tpkcleantmp(0);
	status.hangtime = getconfigint("hangtime", NULL);

	return buf;
}

char* webgettpklist(char* param, int fmt)
{
	if(status.security == 0) return NULL;
	int skip = 0;

	char* buf = NULL;

//	if(fmt == 0)
//	{
		buf = ostrcat(buf, "<html><head><meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\">", 1, 0);
		buf = ostrcat(buf, "<link rel=stylesheet type=text/css href=titan.css><script type=text/javascript src=titan.js></script>", 1, 0);
		buf = ostrcat(buf, "</head><body class=body id=\"tpklist\"><center>", 1, 0);
		buf = ostrcat(buf, "<br>", 1, 0);
		buf = ostrcat(buf, "<h1>", 1, 0);
		buf = ostrcat(buf, _("Tpk Install - select file"), 1, 0);
		buf = ostrcat(buf, "</h1>", 1, 0);
		buf = ostrcat(buf, "<br>", 1, 0);
//	}

	status.hangtime = 99999;

	unlink(TPKLOG);
	tpkgetindex(0);
	tpklist();

	struct tpk* node = tpk, *tpk_installed = NULL, *node_installed = NULL;

	if(node == NULL) buf = ostrcat(buf, _("No Tpk Files Found."), 1, 0);

	tpk = NULL;
	tpklistinstalled(0);
	tpk_installed = tpk;
	tpk = node;

	while(node != NULL)
	{
		node_installed = tpk_installed;
		skip = 0;
		while(node_installed != NULL)
		{
			if(ostrcmp(node->section, node_installed->section) == 0 && ostrcmp(node->showname, node_installed->showname) == 0)
			{
				skip = 1;
				break;
			}
			node_installed = node_installed->next;
		}

		//check if tpk is in section
		if(param != NULL && ostrcmp(node->section, param) != 0)
			skip = 1;

		if(skip == 1)
		{
			node = node->next;
			continue;
		}

		buf = ostrcat(buf, "<a class=linelink2 href=queryraw?gettpkinstallpath&", 1, 0);
		buf = ostrcat(buf, node->filename, 1, 0);
		buf = ostrcat(buf, " target=main>", 1, 0);
		buf = ostrcat(buf, _(node->showname), 1, 0);
		buf = ostrcat(buf, " v.", 1, 0);
		buf = ostrcat(buf, oitoa(node->version), 1, 1);
		buf = ostrcat(buf, "</a>", 1, 0);
		buf = ostrcat(buf, "</br></br>", 1, 0);

		node = node->next;
	}

	freetpk();
	tpkcleantmp(0);
	status.hangtime = getconfigint("hangtime", NULL);

	buf = string_replace_all("<br>", "<br>\n", buf, 1);

//	if(fmt == 0)
//	{
		buf = ostrcat(buf, "</center></body></html>", 1, 0);
//	}

	return buf;
}

char* webgettpkinstallpath(char* param, int fmt)
{
	if(status.security == 0) return NULL;

	char* buf = NULL, *tmpstr = NULL, *path = NULL, *url = NULL, *showname = NULL;
	int count = 0, size = 0;

	status.hangtime = 99999;

	unlink(TPKLOG);
	tpkgetindex(0);
	tpklist();

	struct tpk* node = tpk;

	while(node != NULL)
	{
		if(ostrcmp(node->filename, param) == 0)
		{
			size = node->size;
			showname = ostrcat(node->showname, NULL, 0, 0);
			url = htmlencode(node->url);
			path = ostrcat(node->usepath, NULL, 0, 0);
			break;
		}
		node = node->next;
	}

//	if(fmt == 0)
//	{
		buf = ostrcat(buf, "<html><head><meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\">", 1, 0);
		buf = ostrcat(buf, "<link rel=stylesheet type=text/css href=titan.css><script type=text/javascript src=titan.js></script>", 1, 0);
		buf = ostrcat(buf, "</head><body class=body id=\"tpkinstallpath\"><center>", 1, 0);
		buf = ostrcat(buf, "<br>", 1, 0);
		buf = ostrcat(buf, "<h1>", 1, 0);
		buf = ostrcat(buf, _("Choice Install Medium"), 1, 0);
		buf = ostrcat(buf, "</h1>", 1, 0);
		buf = ostrcat(buf, "<br>", 1, 0);
//	}

	if(path == NULL || path[0] == '*' || ostrstr(path, "mnt") != NULL)
	{
		if(tpkchecksize(NULL, "/mnt/swapextensions", size) == 0)
		{
			tmpstr = ostrcat(tmpstr, "<a class=linelink2 href=queryraw?gettpkinstall&", 1, 0);
			tmpstr = ostrcat(tmpstr, param, 1, 0);
			tmpstr = ostrcat(tmpstr, "&", 1, 0);
			tmpstr = ostrcat(tmpstr, url, 1, 0);
			tmpstr = ostrcat(tmpstr, "&", 1, 0);
			tmpstr = ostrcat(tmpstr, "/mnt/swapextensions", 1, 0);
			tmpstr = ostrcat(tmpstr, " target=main>", 1, 0);
			tmpstr = ostrcat(tmpstr, _("Install to MNT"), 1, 0);
			tmpstr = ostrcat(tmpstr, "</a>\n", 1, 0);
			tmpstr = ostrcat(tmpstr, "</br></br>\n", 1, 0);
			count++;
		}
	}

	if(path == NULL || path[0] == '*' || ostrstr(path, "var") != NULL)
	{
		if(tpkchecksize(NULL, "/var", size) == 0)
		{
			tmpstr = ostrcat(tmpstr, "<a class=linelink2 href=queryraw?gettpkinstall&", 1, 0);
			tmpstr = ostrcat(tmpstr, param, 1, 0);
			tmpstr = ostrcat(tmpstr, "&", 1, 0);
			tmpstr = ostrcat(tmpstr, url, 1, 0);
			tmpstr = ostrcat(tmpstr, "&", 1, 0);
			tmpstr = ostrcat(tmpstr, "/var", 1, 0);
			tmpstr = ostrcat(tmpstr, " target=main>", 1, 0);
			tmpstr = ostrcat(tmpstr, _("Install to FLASH"), 1, 0);
			tmpstr = ostrcat(tmpstr, "</a>\n", 1, 0);
			tmpstr = ostrcat(tmpstr, "</br></br>\n", 1, 0);
			count++;
		}
	}

	if(path == NULL || path[0] == '*' || ostrstr(path, "swap") != NULL)
	{
#ifdef OEBUILD
		if(file_exist("/media/.swapextensionsdev") == 1 || file_exist("/var/swap"))
#else
		if(file_exist("/tmp/.swapextensionsdev") == 1 || file_exist("/var/swap"))
#endif
		{
			if(tpkchecksize(NULL, "/var/swap", size) == 0)
			{
				tmpstr = ostrcat(tmpstr, "<a class=linelink2 href=queryraw?gettpkinstall&", 1, 0);
				tmpstr = ostrcat(tmpstr, param, 1, 0);
				tmpstr = ostrcat(tmpstr, "&", 1, 0);
				tmpstr = ostrcat(tmpstr, url, 1, 0);
				tmpstr = ostrcat(tmpstr, "&", 1, 0);
				tmpstr = ostrcat(tmpstr, "/var/swap", 1, 0);
				tmpstr = ostrcat(tmpstr, " target=main>", 1, 0);
				tmpstr = ostrcat(tmpstr, _("Install to SWAP"), 1, 0);
				tmpstr = ostrcat(tmpstr, "</a>", 1, 0);
				tmpstr = ostrcat(tmpstr, "</br></br>", 1, 0);
				count++;
			}
		}
	}

	if(node == NULL && tmpstr == NULL) buf = ostrcat(buf, _("No Tpk Files Found."), 1, 0);
	freetpk();

	if(count == 0)
	{
		buf = ostrcat(buf, "<br>", 1, 0);
		buf = ostrcat(buf, _("Can't install Package. Package to big."), 1, 0);
	}

	buf = ostrcat(buf, tmpstr, 1, 0);

	buf = string_replace_all("<br>", "<br>\n", buf, 1);

	free(tmpstr), tmpstr = NULL;
	free(showname), showname = NULL;
	free(url), url = NULL;
	free(path), path = NULL;

	tpkcleantmp(0);
	status.hangtime = getconfigint("hangtime", NULL);

//	if(fmt == 0)
//	{
		buf = ostrcat(buf, "</center></body></html>", 1, 0);
//	}

	return buf;
}

char* webgettpkinstall(char* param, int fmt)
{
	if(status.security == 0) return NULL;

	char* buf = NULL, *tmpstr = NULL, *param1 = NULL, *param2 = NULL;

	//create param1
	param1 = strchr(param, '&');
	if(param1 != NULL)
		*param1++ = '\0';

	//create param2
	param2 = strchr(param1, '&');
	if(param2 != NULL)
		*param2++ = '\0';

	if(param == NULL) return NULL;
	if(param1 == NULL) return NULL;
	if(param2 == NULL) return NULL;

	status.hangtime = 99999;

	char* log = NULL;
	int tpkret = tpkgetpackage(param, param1, param2, 0, 1);

	if(tpkret == 0)
		tmpstr = ostrcat(_("Tpk Install Info - Install OK"), NULL, 0, 0);
	else if(tpkret == 2)
		tmpstr = ostrcat(_("Tpk Install Info - Install ERROR"), NULL, 0, 0);
	else
		tmpstr = ostrcat(_("Tpk Install Info - Install ERROR"), NULL, 0, 0);

//	if(fmt == 0)
//	{
		buf = ostrcat(buf, "<html><head><meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\">", 1, 0);
		buf = ostrcat(buf, "<link rel=stylesheet type=text/css href=titan.css><script type=text/javascript src=titan.js></script>", 1, 0);
		buf = ostrcat(buf, "</head><body class=body id=\"tpkinstall\"><center>", 1, 0);
		buf = ostrcat(buf, "<br>", 1, 0);
		buf = ostrcat(buf, "<h1>", 1, 0);
		buf = ostrcat(buf, _(tmpstr), 1, 0);
		buf = ostrcat(buf, "</h1>", 1, 0);
		buf = ostrcat(buf, "<br>", 1, 0);
		buf = ostrcat(buf, "<br>", 1, 0);
//	}

	free(tmpstr), tmpstr = NULL;

	if(tpkret == 0)
	{
		log = gettpklog(param2, 0);
		log = string_replace_all("\n", "<br>\n", log, 1);
		buf = ostrcat(buf, "<br>", 1, 0);
		buf = ostrcat(buf, _("Install Log:"), 1, 0);
		buf = ostrcat(buf, "<br>", 1, 0);
		buf = ostrcat(buf, "<br>", 1, 0);
		buf = ostrcat(buf, log, 1, 0);
	}
	else if(tpkret == 2)
	{
		buf = ostrcat(buf, "<br>", 1, 0);
		buf = ostrcat(buf, _("Install Log:"), 1, 0);
		buf = ostrcat(buf, "<br>", 1, 0);
		buf = ostrcat(buf, "<br>", 1, 0);
		buf = ostrcat(buf, _("There may only be one package installed from this section. To install another package from this section, remove the installed one. If the package is not visible after a software update, perform a TPK update to: "), 1, 0);
	}
	else
	{
		log = gettpklog(param2, 1);
		log = string_replace_all("\n", "<br>\n", log, 1);
		buf = ostrcat(buf, "<br>", 1, 0);
		buf = ostrcat(buf, _("Install Log:"), 1, 0);
		buf = ostrcat(buf, "<br>", 1, 0);
		buf = ostrcat(buf, "<br>", 1, 0);
		buf = ostrcat(buf, log, 1, 0);
	}

	buf = string_replace_all("<br>", "<br>\n", buf, 1);

	loadplugin();
	free(log), log = NULL;
	unlink(TPKLOG);
	if(file_exist("/tmp/.tpk_needs_reboot"))
	{
// need other..
		textbox(_("Message"), _("TPK Install done, your system will reboot !"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 800, 200, 0, 0);
		buf = ostrcat(buf, "<br>", 1, 0);
		buf = ostrcat(buf, _("TPK Install done, your system will reboot !"), 1, 0);
		buf = string_replace_all("<br>", "<br>\n", buf, 1);
		//write only config file
		writeallconfig(3);
		oshutdown(2,2);
		system("init 6");
	}

	status.hangtime = getconfigint("hangtime", NULL);

//	if(fmt == 0)
//	{
		buf = ostrcat(buf, "</center></body></html>", 1, 0);
//	}

	return buf;
}

char* webgettpktmplist(char* param, int fmt)
{
	if(status.security == 0) return NULL;
	int mode = atoi(param);

	char* buf = NULL, *tmpstr = NULL, *tmpstr1 = NULL, *tmpstr2 = NULL, *tmpstr3 = NULL;

	if(fmt == 0)
	{
		buf = ostrcat(buf, "<html><head><meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\">", 1, 0);
		buf = ostrcat(buf, "<link rel=stylesheet type=text/css href=titan.css><script type=text/javascript src=titan.js></script>", 1, 0);
		buf = ostrcat(buf, "</head><body class=body id=\"tpktmplist\"><center>", 1, 0);
		buf = ostrcat(buf, "<br>", 1, 0);
		buf = ostrcat(buf, "<h1>", 1, 0);
		if(mode == 0)
			buf = ostrcat(buf, _("TPK tmp (tmp)"), 1, 0);
		else
			buf = ostrcat(buf, _("TPK media (media)"), 1, 0);

		buf = ostrcat(buf, "</h1>", 1, 0);
		buf = ostrcat(buf, "<br>", 1, 0);
	}

	if(mode == 0)
		tmpstr = gettpktmplist("/tmp");
	else
	{
		int treffer = 0;
		struct hdd *node = NULL;

		addhddall();
		node = hdd;

		while(node != NULL)
		{
			if(node->partition != 0)
			{
				tmpstr1 = ostrcat("/autofs/", node->device, 0, 0);
				tmpstr2 = gettpktmplist(tmpstr1);

				if(tmpstr2 != NULL)
				{
					treffer = 1;
//					screenextensions(2, tmpstr1, NULL, 1);
					tmpstr3 = ostrcat(tmpstr3, tmpstr1, 1, 0);
					tmpstr3 = ostrcat(tmpstr3, "\n", 1, 0);
				}

				free(tmpstr1); tmpstr1 = NULL;
				free(tmpstr2); tmpstr2 = NULL;
			}
			node = node->next;
		}

		tmpstr = gettpktmplist(tmpstr3);
		free(tmpstr3), tmpstr = NULL;
	}

	if(mode == 0)
	{
		if(tmpstr == NULL) buf = ostrcat(buf, _("No Tpk Files Found on /tmp Directory."), 1, 0);
	}
	else
	{
		if(tmpstr == NULL) buf = ostrcat(buf, _("No Tpk Files Found on Media Devices."), 1, 0);
	}

	int count, i, max;
	count = 0;
	struct splitstr* ret1 = NULL;
	ret1 = strsplit(tmpstr, "\n", &count);
	max = count - 1;

	buf = ostrcat(buf, "<table cellpadding=5 cellspacing=5><tr><td nowrap>", 1, 0);

	if(ret1 != NULL)
	{
		for(i = 0; i <= max; i++)
		{
			buf = ostrcat(buf, "<a class=linelink2 href=queryraw?gettpkinstallpath&", 1, 0);
			buf = ostrcat(buf, ret1[i].part, 1, 0);
			buf = ostrcat(buf, " target=main>", 1, 0);
			buf = ostrcat(buf, _(stringreplacecharonce(ret1[i].part, '.', '\0')), 1, 0);
			buf = ostrcat(buf, "</a>", 1, 0);
			buf = ostrcat(buf, "</br></br>", 1, 0);
		}
	}

	free(tmpstr), tmpstr = NULL;
	free(ret1), ret1 = NULL;

	buf = string_replace_all("<br>", "<br>\n", buf, 1);

	if(fmt == 0)
	{
		buf = ostrcat(buf, "</center></body></html>", 1, 0);
	}

	return buf;
}

char* webgettpkremovelist(int fmt)
{
	if(status.security == 0) return NULL;

	char* buf = NULL;
	int skip;

//	if(fmt == 0)
//	{
		buf = ostrcat(buf, "<html><head><meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\">", 1, 0);
		buf = ostrcat(buf, "<link rel=stylesheet type=text/css href=titan.css><script type=text/javascript src=titan.js></script>", 1, 0);
		buf = ostrcat(buf, "</head><body class=body id=\"tpkremovelist\"><center>", 1, 0);
		buf = ostrcat(buf, "<br>", 1, 0);
		buf = ostrcat(buf, "<h1>", 1, 0);
		buf = ostrcat(buf, _("Tpk Remove - select file"), 1, 0);
		buf = ostrcat(buf, "</h1>", 1, 0);
		buf = ostrcat(buf, "<br>", 1, 0);
//	}

	status.hangtime = 99999;

	unlink(TPKLOG);
	tpkgetindex(0);
	tpklist();

	struct tpk* node = tpk, *tpk_installed = NULL, *node_installed = NULL;

	if(node == NULL) buf = ostrcat(buf, _("No Tpk Files Found."), 1, 0);

	tpk = NULL;
	tpklistinstalled(0);
	tpk_installed = tpk;
	tpk = node;

	while(node != NULL)
	{
		node_installed = tpk_installed;
		skip = 1;
		while(node_installed != NULL)
		{
			if(ostrcmp(node->showname, node_installed->showname) == 0)
			{
				skip = 0;
				break;
			}
			node_installed = node_installed->next;
		}

		if(skip == 1)
		{
			node = node->next;
			continue;
		}

		buf = ostrcat(buf, "<a class=linelink2 href=queryraw?gettpkremove&", 1, 0);
		buf = ostrcat(buf, node->name, 1, 0);
		buf = ostrcat(buf, " target=main>", 1, 0);
		buf = ostrcat(buf, _(node->showname), 1, 0);
		buf = ostrcat(buf, " (", 1, 0);
		buf = ostrcat(buf, _(node->section), 1, 0);
		buf = ostrcat(buf, ")</a>", 1, 0);
		buf = ostrcat(buf, "</br></br>", 1, 0);

		node = node->next;
	}

	freetpk();
	tpkcleantmp(0);
	status.hangtime = getconfigint("hangtime", NULL);

	buf = string_replace_all("<br>", "<br>\n", buf, 1);

//	if(fmt == 0)
//	{
		buf = ostrcat(buf, "</center></body></html>", 1, 0);
//	}

	return buf;
}

char* webgettpkremove(char* param, int fmt)
{
	if(status.security == 0) return NULL;

	char* buf = NULL, *tmpstr = NULL;

	status.hangtime = 99999;

	char* log = NULL;

	int tpkret = tpkremove(param, 0, 0);

	if(tpkret == 0)
	{
		tmpstr = ostrcat(_("Tpk Remove Info - Remove OK"), NULL, 0, 0);
		if(param != NULL && param[0] != '*') delplugin(param);
	}
	else
		tmpstr = ostrcat(_("Tpk Remove Info - Remove ERROR"), NULL, 0, 0);

//	if(fmt == 0)
//	{
		buf = ostrcat(buf, "<html><head><meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\">", 1, 0);
		buf = ostrcat(buf, "<link rel=stylesheet type=text/css href=titan.css><script type=text/javascript src=titan.js></script>", 1, 0);
		buf = ostrcat(buf, "</head><body class=body id=\"tpkremove\"><center>", 1, 0);
		buf = ostrcat(buf, "<br>", 1, 0);
		buf = ostrcat(buf, "<h1>", 1, 0);
		buf = ostrcat(buf, _(tmpstr), 1, 0);
		buf = ostrcat(buf, "</h1>", 1, 0);
		buf = ostrcat(buf, "<br>", 1, 0);
		buf = ostrcat(buf, "<br>", 1, 0);
//	}

	free(tmpstr), tmpstr = NULL;

	if(tpkret == 0)
	{
		log = gettpklog(NULL, 2);
		log = string_replace_all("\n", "<br>\n", log, 1);
		buf = ostrcat(buf, "<br>", 1, 0);
		buf = ostrcat(buf, _("Remove Log:"), 1, 0);
		buf = ostrcat(buf, "<br>", 1, 0);
		buf = ostrcat(buf, "<br>", 1, 0);
		buf = ostrcat(buf, log, 1, 0);
	}
	else
	{
		log = gettpklog(NULL, 3);
		log = string_replace_all("\n", "<br>\n", log, 1);
		buf = ostrcat(buf, "<br>", 1, 0);
		buf = ostrcat(buf, _("Remove Log:"), 1, 0);
		buf = ostrcat(buf, "<br>", 1, 0);
		buf = ostrcat(buf, "<br>", 1, 0);
		buf = ostrcat(buf, log, 1, 0);
	}

	buf = string_replace_all("<br>", "<br>\n", buf, 1);

	loadplugin();
	free(log), log = NULL;
	unlink(TPKLOG);
	if(file_exist("/tmp/.tpk_needs_reboot"))
	{
// need other..
		textbox(_("Message"), _("TPK Remove done, your system will reboot !"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 800, 200, 0, 0);
		buf = ostrcat(buf, "<br>", 1, 0);
		buf = ostrcat(buf, _("TPK Install done, your system will reboot !"), 1, 0);
		buf = string_replace_all("<br>", "<br>\n", buf, 1);
		//write only config file
		writeallconfig(3);
		oshutdown(2,2);
		system("init 6");
	}

	status.hangtime = getconfigint("hangtime", NULL);

//	if(fmt == 0)
//	{
		buf = ostrcat(buf, "</center></body></html>", 1, 0);
//	}

	return buf;
}

char* webgettpkupgrade(int fmt)
{
	if(status.security == 0) return NULL;

	char* buf = NULL;

	status.hangtime = 99999;

	if(fmt == 0)
	{
		buf = ostrcat(buf, "<html><head><meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\">", 1, 0);
		buf = ostrcat(buf, "<link rel=stylesheet type=text/css href=titan.css><script type=text/javascript src=titan.js></script>", 1, 0);
		buf = ostrcat(buf, "</head><body class=body id=\"tpkupgrade\"><center>", 1, 0);
	}

	if(tpkgetindex(1) != 0)
	{
		buf = ostrcat(buf, "<br>", 1, 0);
		buf = ostrcat(buf, "<h1>", 1, 0);
		buf = ostrcat(buf, _("Tpk Update Info - Update ERROR"), 1, 0);
		buf = ostrcat(buf, "</h1>", 1, 0);
		buf = ostrcat(buf, "<br>", 1, 0);
		buf = ostrcat(buf, _("Can't get all TPK index !"), 1, 0);
		buf = ostrcat(buf, "<br>", 1, 0);
	}

	writesys("/tmp/.tpk_upgrade_start", "0", 0);
	if(tpkupdate(0) != 0)
	{
		buf = ostrcat(buf, "<br>", 1, 0);
		buf = ostrcat(buf, "<h1>", 1, 0);
		buf = ostrcat(buf, _("Tpk Update Info - Update ERROR"), 1, 0);
		buf = ostrcat(buf, "</h1>", 1, 0);
		buf = ostrcat(buf, "<br>", 1, 0);
		buf = ostrcat(buf, _("Can't update all packages !"), 1, 0);
		buf = ostrcat(buf, "<br>", 1, 0);
	}
	else
	{
		buf = ostrcat(buf, "<br>", 1, 0);
		buf = ostrcat(buf, "<h1>", 1, 0);
		buf = ostrcat(buf, _("Tpk Update Info - Update OK"), 1, 0);
		buf = ostrcat(buf, "</h1>", 1, 0);
		buf = ostrcat(buf, "<br>", 1, 0);
	}

	loadplugin();
	unlink(TPKLOG);

	if(file_exist("/tmp/.tpk_needs_reboot"))
	{
		textbox(_("Message"), _("TPK Upgrade done, your system will reboot !"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 0, 0);
		//write only config file
		writeallconfig(3);
		oshutdown(2,2);
		system("init 6");
	}
	unlink("/tmp/.tpk_upgrade_start");

	status.hangtime = getconfigint("hangtime", NULL);

	if(fmt == 0)
	{
		buf = ostrcat(buf, "</center></body></html>", 1, 0);
	}

	return buf;
}

char* webgetserviceinfo(int fmt)
{
	char* buf = NULL, *tmpstr = NULL;

	if(fmt == 0)
	{
		buf = ostrcat(buf, "<html><head><meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\">", 1, 0);
		buf = ostrcat(buf, "<link rel=stylesheet type=text/css href=titan.css><script type=text/javascript src=titan.js></script>", 1, 0);
		buf = ostrcat(buf, "</head><body class=body id=\"serviceinfo\"><center>", 1, 0);
		buf = ostrcat(buf, "<br>", 1, 0);
		buf = ostrcat(buf, "<h1>", 1, 0);
		buf = ostrcat(buf, _("Service"), 1, 0);
		buf = ostrcat(buf, "</h1>", 1, 0);
		buf = ostrcat(buf, "<br>", 1, 0);
	}

//	tmpstr = getabout();
//	readnewsletter();
	tmpstr = readfiletomem("/tmp/Service.txt", 0);
	tmpstr = ostrcat(tmpstr, "\ncomming soon...\n", 1, 0);

	tmpstr = string_replace_all("\n", "<br>\n", tmpstr, 1);

	buf = ostrcat(buf, tmpstr, 1, 1);

	if(fmt == 0)
	{
		buf = ostrcat(buf, "</center></body></html>", 1, 0);
	}

	return buf;
}

char* webgetbackup(int fmt)
{
	if(status.security == 0) return NULL;

	char* buf = NULL, *tmpstr = NULL;

	if(fmt == 0)
	{
		buf = ostrcat(buf, "<html><head><meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\">", 1, 0);
		buf = ostrcat(buf, "<link rel=stylesheet type=text/css href=titan.css><script type=text/javascript src=titan.js></script>", 1, 0);
		buf = ostrcat(buf, "</head><body class=body id=\"backup\"><center>", 1, 0);
		buf = ostrcat(buf, "<br>", 1, 0);
		buf = ostrcat(buf, "<h1>", 1, 0);
		buf = ostrcat(buf, _("System Backup"), 1, 0);
		buf = ostrcat(buf, "</h1>", 1, 0);
		buf = ostrcat(buf, "<br>", 1, 0);
	}


	tmpstr = create_backup(NULL, 0);

	tmpstr = string_replace_all("\n", "<br>\n", tmpstr, 1);
	buf = ostrcat(buf, tmpstr, 1, 1);
	buf = ostrcat(buf, "</br></br>", 1, 0);
	buf = ostrcat(buf, "<a class=linelink2 href=queryraw?getcreatebackup target=main>", 1, 0);
	buf = ostrcat(buf, _("create System Backup"), 1, 0);
	buf = ostrcat(buf, "</a>", 1, 0);
	buf = ostrcat(buf, "</br></br>", 1, 0);

	if(fmt == 0)
	{
		buf = ostrcat(buf, "</center></body></html>", 1, 0);
	}

	return buf;
}

char* webgetcreatebackup(int fmt)
{
	if(status.security == 0) return NULL;

	char* buf = NULL, *tmpstr = NULL;

	if(fmt == 0)
	{
		buf = ostrcat(buf, "<html><head><meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\">", 1, 0);
		buf = ostrcat(buf, "<link rel=stylesheet type=text/css href=titan.css><script type=text/javascript src=titan.js></script>", 1, 0);
		buf = ostrcat(buf, "</head><body class=body id=\"createbackup\"><center>", 1, 0);
		buf = ostrcat(buf, "<br>", 1, 0);
		buf = ostrcat(buf, "<h1>", 1, 0);
		buf = ostrcat(buf, _("create System Backup"), 1, 0);
		buf = ostrcat(buf, "</h1>", 1, 0);
		buf = ostrcat(buf, "<br>", 1, 0);
	}

#ifdef MIPSEL
	tmpstr = ostrcat(tmpstr, _("System Backup on your Box not Supported"), 1, 0);
#else
	tmpstr = create_backup("full", 2);
	tmpstr = string_replace_all("\n", "<br>\n", tmpstr, 1);
	tmpstr = ostrcat(tmpstr, "<br>", 1, 0);
	tmpstr = ostrcat(tmpstr, _("Backup created successfully"), 1, 0);
#endif
	buf = ostrcat(buf, tmpstr, 1, 1);

	if(fmt == 0)
	{
		buf = ostrcat(buf, "</center></body></html>", 1, 0);
	}
	return buf;
}

char* webgetrestore(int fmt)
{
	if(status.security == 0) return NULL;

	char* buf = NULL, *tmpstr = NULL;

	if(fmt == 0)
	{
		buf = ostrcat(buf, "<html><head><meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\">", 1, 0);
		buf = ostrcat(buf, "<link rel=stylesheet type=text/css href=titan.css><script type=text/javascript src=titan.js></script>", 1, 0);
		buf = ostrcat(buf, "</head><body class=body id=\"restore\"><center>", 1, 0);
		buf = ostrcat(buf, "<br>", 1, 0);
		buf = ostrcat(buf, "<h1>", 1, 0);
		buf = ostrcat(buf, _("Settings backup/restore"), 1, 0);
		buf = ostrcat(buf, "</h1>", 1, 0);
		buf = ostrcat(buf, "<br>", 1, 0);
	}

	tmpstr = create_backuprestore();

	tmpstr = string_replace_all("\n", "<br>\n", tmpstr, 1);
	buf = ostrcat(buf, tmpstr, 1, 1);
	buf = ostrcat(buf, "</br></br>", 1, 0);
	buf = ostrcat(buf, "<a class=linelink2 href=queryraw?getcreaterestore&1 target=main>", 1, 0);
	buf = ostrcat(buf, _("Restore"), 1, 0);
	buf = ostrcat(buf, "</a>", 1, 0);
	buf = ostrcat(buf, "</br></br>", 1, 0);

	buf = ostrcat(buf, "<a class=linelink2 href=queryraw?getcreaterestore&2 target=main>", 1, 0);
	buf = ostrcat(buf, _("Backup"), 1, 0);
	buf = ostrcat(buf, "</a>", 1, 0);
	buf = ostrcat(buf, "</br></br>", 1, 0);

	if(fmt == 0)
	{
		buf = ostrcat(buf, "</center></body></html>", 1, 0);
	}

	return buf;
}
/////////
char* webgetcreaterestore(char* param, int fmt)
{
	if(status.security == 0) return NULL;

	char* buf = NULL, *tmpstr = NULL, *cmd = NULL;

	int mode = atoi(param);

	char* BACKUPDIR = NULL;
#ifdef OEBUILD
	if(isfile("/media/.moviedev") || file_exist("/media/hdd"))
		BACKUPDIR = ostrcat(" /media/hdd", NULL, 0, 0);
	else if(isfile("/media/.backupdev") || file_exist("/var/backup"))
		BACKUPDIR = ostrcat(" /var/backup", NULL, 0, 0);
	else if(isfile("/media/.swapextensionsdev") || file_exist("/var/swap"))
		BACKUPDIR = ostrcat(" /var/swap", NULL, 0, 0);
#endif


	if(mode == 1)
		cmd = ostrcat("/sbin/settings.sh restore", BACKUPDIR, 0, 0);

	if(mode == 2)
	{
		cmd = ostrcat("/sbin/settings.sh backup", BACKUPDIR, 0, 0);
		writeallconfig(1);
	}

	printf("cmd: %s\n", cmd);
	tmpstr = command(cmd);
	free(cmd), cmd = NULL;

//	if(fmt == 0)
//	{
	buf = ostrcat(buf, "<html><head><meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\">", 1, 0);
	buf = ostrcat(buf, "<link rel=stylesheet type=text/css href=titan.css><script type=text/javascript src=titan.js></script>", 1, 0);
	buf = ostrcat(buf, "</head><body class=body id=\"createrestore\"><center>", 1, 0);
	buf = ostrcat(buf, "<br>", 1, 0);
	buf = ostrcat(buf, "<h1>", 1, 0);
	if(tmpstr == NULL)
		buf = ostrcat(buf, _("No backup folders found!!!\n\nAborting restoring..."), 1, 0);
	else
		buf = ostrcat(buf, _("Backup created successfully"), 1, 0);

	buf = ostrcat(buf, "</h1>", 1, 0);
	buf = ostrcat(buf, "<br>", 1, 0);
//	}

	buf = ostrcat(buf, tmpstr, 1, 1);

	buf = string_replace_all("\n", "<br>\n", buf, 1);

//	if(fmt == 0)
//	{
		buf = ostrcat(buf, "</center></body></html>", 1, 0);
//	}

	return buf;
}

char* webstartplugin(char* param, int fmt)
{
	void (*startplugin)(void);
	char* buf = NULL;

	struct skin* pluginnode = getplugin(param);

	if(pluginnode != NULL)
	{
		startplugin = dlsym(pluginnode->pluginhandle, "start");
		if(startplugin != NULL)
		{
			resettvpic();
			startplugin();
			resettvpic();
			buf = ostrcat(buf, "ok", 1, 0);
		}
		else
			buf = ostrcat(buf, "internal error", 1, 0);
	}
	else
		buf = ostrcat(buf, "wrong plugin name", 1, 0);

	return buf;
}

char* webinfobarplugin(char* param, int fmt)
{
	char* buf = NULL;

	struct skin* pluginnode = getplugin(param);

	if(pluginnode != NULL)
	{
			status.extplugin = ostrcat(param, NULL, 0, 0);
			buf = ostrcat(buf, "ok", 1, 0);
			writerc(getrcconfigint("rcexit", NULL));
	}
	else
		buf = ostrcat(buf, "wrong plugin name", 1, 0);

	return buf;
}

char* webgetrestoredefaultlist(int fmt)
{
	if(status.security == 0) return NULL;

	char* buf = NULL;

	if(fmt == 0)
	{
		buf = ostrcat(buf, "<html><head><meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\">", 1, 0);
		buf = ostrcat(buf, "<link rel=stylesheet type=text/css href=titan.css><script type=text/javascript src=titan.js></script>", 1, 0);
		buf = ostrcat(buf, "</head><body class=body id=\"restoredefaultlist\"><center>", 1, 0);
		buf = ostrcat(buf, "<br>", 1, 0);
		buf = ostrcat(buf, "<h1>", 1, 0);
		buf = ostrcat(buf, _("Restore default settings"), 1, 0);
		buf = ostrcat(buf, "</h1>", 1, 0);
		buf = ostrcat(buf, "<br>", 1, 0);
	}

	buf = ostrcat(buf, "</br></br>", 1, 0);
	buf = ostrcat(buf, "<a class=linelink2 href=queryraw?getrestoredefault&1 target=main>", 1, 0);
	buf = ostrcat(buf, _("Restore default settings"), 1, 0);
	buf = ostrcat(buf, "</a>", 1, 0);
	buf = ostrcat(buf, "</br></br>", 1, 0);

	buf = ostrcat(buf, "<a class=linelink2 href=queryraw?getrestoredefault&2 target=main>", 1, 0);
	buf = ostrcat(buf, _("Format MNT with Backup/Restore"), 1, 0);
	buf = ostrcat(buf, "</a>", 1, 0);
	buf = ostrcat(buf, "</br></br>", 1, 0);

	buf = ostrcat(buf, "<a class=linelink2 href=queryraw?getrestoredefault&3 target=main>", 1, 0);
	buf = ostrcat(buf, _("Format MNT (all)"), 1, 0);
	buf = ostrcat(buf, "</a>", 1, 0);

	buf = ostrcat(buf, "</br></br>", 1, 0);

	if(fmt == 0)
	{
		buf = ostrcat(buf, "</center></body></html>", 1, 0);
	}

	return buf;
}

char* webgetrestoredefault(char* param, int fmt)
{
	if(status.security == 0) return NULL;

	char* buf = NULL, *tmpstr = NULL;
	int mode = atoi(param);

//	if(fmt == 0)
//	{
		buf = ostrcat(buf, "<html><head><meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\">", 1, 0);
		buf = ostrcat(buf, "<link rel=stylesheet type=text/css href=titan.css><script type=text/javascript src=titan.js></script>", 1, 0);
		buf = ostrcat(buf, "</head><body class=body id=\"restoredefault\"><center>", 1, 0);
		buf = ostrcat(buf, "<br>", 1, 0);
		buf = ostrcat(buf, "<h1>", 1, 0);

	if(mode == 1)
	{
		tmpstr = getconfig("restorecmd", NULL);
		system(tmpstr);
		buf = ostrcat(buf, _("Restore default settings"), 1, 0);
		buf = ostrcat(buf, "<br>", 1, 0);
		buf = ostrcat(buf, _("Receiver reboots now !!!"), 1, 0);
		system("sync");
		system("sleep 10; init 6");
	}
	if(mode == 2)
	{
		system("touch /var/etc/.erasemtd");
		system("touch /var/etc/.backupmtd");
		buf = ostrcat(buf, _("Format MNT with Backup/Restore"), 1, 0);
		buf = ostrcat(buf, "<br>", 1, 0);
		buf = ostrcat(buf, _("Receiver reboots now !!!"), 1, 0);
		system("sync");
		system("sleep 10; init 6");
	}
	if(mode == 3)
	{
		system("touch /var/etc/.erasemtd");
		buf = ostrcat(buf, _("Format MNT (all)"), 1, 0);
		buf = ostrcat(buf, "<br>", 1, 0);
		buf = ostrcat(buf, _("Receiver reboots now !!!"), 1, 0);
		system("sync");
		system("sleep 10; init 6");
	}

		buf = ostrcat(buf, "</h1>", 1, 0);
		buf = ostrcat(buf, "<br>", 1, 0);
//	}

//	if(fmt == 0)
//	{
		buf = ostrcat(buf, "</center></body></html>", 1, 0);
//	}

	return buf;
}

char* webgetchannelsettingslist(int fmt)
{
	if(status.security == 0) return NULL;

	char* buf = NULL;

	if(fmt == 0)
	{
		buf = ostrcat(buf, "<html><head><meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\">", 1, 0);
		buf = ostrcat(buf, "<link rel=stylesheet type=text/css href=titan.css><script type=text/javascript src=titan.js></script>", 1, 0);
		buf = ostrcat(buf, "</head><body class=body id=\"channelsettingslist\"><center>", 1, 0);
		buf = ostrcat(buf, "<br>", 1, 0);
		buf = ostrcat(buf, "<h1>", 1, 0);
		buf = ostrcat(buf, _("Restore channel settings"), 1, 0);
		buf = ostrcat(buf, "</h1>", 1, 0);
		buf = ostrcat(buf, "<br>", 1, 0);
	}

	buf = ostrcat(buf, "<a class=linelink2 href=queryraw?getchannelsettings&1 target=main>", 1, 0);
	buf = ostrcat(buf, _("Restore Default Channel Settings"), 1, 0);
	buf = ostrcat(buf, "</a>", 1, 0);
	buf = ostrcat(buf, "</br></br>", 1, 0);

	buf = ostrcat(buf, "<a class=linelink2 href=queryraw?getchannelsettings&2 target=main>", 1, 0);
	buf = ostrcat(buf, _("Restore Default Bouquets"), 1, 0);
	buf = ostrcat(buf, "</a>", 1, 0);
	buf = ostrcat(buf, "</br></br>", 1, 0);

	buf = ostrcat(buf, "<a class=linelink2 href=queryraw?getchannelsettings&3 target=main>", 1, 0);
	buf = ostrcat(buf, _("Restore Default Channels"), 1, 0);
	buf = ostrcat(buf, "</a>", 1, 0);
	buf = ostrcat(buf, "</br></br>", 1, 0);

	buf = ostrcat(buf, "<a class=linelink2 href=queryraw?getchannelsettings&4 target=main>", 1, 0);
	buf = ostrcat(buf, _("Restore Default Providers"), 1, 0);
	buf = ostrcat(buf, "</a>", 1, 0);
	buf = ostrcat(buf, "</br></br>", 1, 0);

	buf = ostrcat(buf, "<a class=linelink2 href=queryraw?getchannelsettings&5 target=main>", 1, 0);
	buf = ostrcat(buf, _("Restore Default Sats"), 1, 0);
	buf = ostrcat(buf, "</a>", 1, 0);

	buf = ostrcat(buf, "</br></br>", 1, 0);

	if(fmt == 0)
	{
		buf = ostrcat(buf, "</center></body></html>", 1, 0);
	}

	return buf;
}

char* webgetchannelsettings(char* param, int fmt)
{
	if(status.security == 0) return NULL;

	char* buf = NULL;
	int mode = atoi(param);

//	if(fmt == 0)
//	{
		buf = ostrcat(buf, "<html><head><meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\">", 1, 0);
		buf = ostrcat(buf, "<link rel=stylesheet type=text/css href=titan.css><script type=text/javascript src=titan.js></script>", 1, 0);
		buf = ostrcat(buf, "</head><body class=body id=\"channelsettings\"><center>", 1, 0);
		buf = ostrcat(buf, "<br>", 1, 0);
		buf = ostrcat(buf, "<h1>", 1, 0);

	if(mode == 1)
	{
		system("rm -rf /mnt/settings/* > /dev/null 2>&1");
		system("cp -a /etc/titan.restore/mnt/settings/* /mnt/settings > /dev/null 2>&1");
		buf = ostrcat(buf, _("Restore Default Channel Settings"), 1, 0);
		buf = ostrcat(buf, "<br>", 1, 0);
		buf = ostrcat(buf, _("Receiver reboots now !!!"), 1, 0);
		system("sync");
		system("sleep 10; init 6");
	}
	if(mode == 2)
	{
		system("rm -f /mnt/settings/bouquets.* > /dev/null 2>&1");
		system("cp -a /etc/titan.restore/mnt/settings/bouquets.cfg /mnt/settings/bouquets.cfg > /dev/null 2>&1");
		system("cp -a /etc/titan.restore/mnt/settings/bouquets.*.tv /mnt/settings > /dev/null 2>&1");
		system("cp -a /etc/titan.restore/mnt/settings/bouquets.*.radio /mnt/settings > /dev/null 2>&1");
		buf = ostrcat(buf, _("Restore Default Bouquets"), 1, 0);
		buf = ostrcat(buf, "<br>", 1, 0);
		buf = ostrcat(buf, _("Receiver reboots now !!!"), 1, 0);
		system("sync");
		system("sleep 10; init 6");
	}
	if(mode == 3)
	{
		system("rm -f /mnt/settings/channel > /dev/null 2>&1");
		system("cp -a /etc/titan.restore/mnt/settings/channel /mnt/settings > /dev/null 2>&1");
		buf = ostrcat(buf, _("Restore Default Channels"), 1, 0);
		buf = ostrcat(buf, "<br>", 1, 0);
		buf = ostrcat(buf, _("Receiver reboots now !!!"), 1, 0);
		system("sync");
		system("sleep 10; init 6");
	}
	if(mode == 4)
	{
		system("rm -f /mnt/settings/provider > /dev/null 2>&1");
		system("cp -a /etc/titan.restore/mnt/settings/provider /mnt/settings > /dev/null 2>&1");
		buf = ostrcat(buf, _("Restore Default Providers"), 1, 0);
		buf = ostrcat(buf, "<br>", 1, 0);
		buf = ostrcat(buf, _("Receiver reboots now !!!"), 1, 0);
		system("sync");
		system("sleep 10; init 6");
	}
	if(mode == 5)
	{
		system("rm -f /mnt/settings/satellites > /dev/null 2>&1");
		system("cp -a /etc/titan.restore/mnt/settings/satellites /mnt/settings > /dev/null 2>&1");
		buf = ostrcat(buf, _("Restore Default Sats"), 1, 0);
		buf = ostrcat(buf, "<br>", 1, 0);
		buf = ostrcat(buf, _("Receiver reboots now !!!"), 1, 0);
		system("sync");
		system("sleep 10; init 6");
	}
		buf = ostrcat(buf, "</h1>", 1, 0);
		buf = ostrcat(buf, "<br>", 1, 0);
//	}

//	if(fmt == 0)
//	{
		buf = ostrcat(buf, "</center></body></html>", 1, 0);
//	}

	return buf;
}

char* webgettelnet(int fmt)
{
	char* buf = NULL, *tmpstr = NULL;
	if(status.security == 0) return NULL;

//	if(fmt == 0)
//	{
		buf = ostrcat(buf, "<html><head><meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\">", 1, 0);
		buf = ostrcat(buf, "<link rel=stylesheet type=text/css href=titan.css><script type=text/javascript src=titan.js></script>", 1, 0);
		buf = ostrcat(buf, "</head><body class=body id=\"command\"><center>", 1, 0);
//	}

	buf = ostrcat(buf, "<form action=query method=get>", 1, 0);
	buf = ostrcat(buf, "<br><br>", 1, 0);
	buf = ostrcat(buf, "<div class=label>", 1, 0);
	buf = ostrcat(buf, _("Send Telnet Command"), 1, 0);
	buf = ostrcat(buf, "</div>", 1, 0);

	buf = ostrcat(buf, "<br><br>", 1, 0);
	buf = ostrcat(buf, "<input hidden class=inputbox name=getcommand>", 1, 0);
//	buf = ostrcat(buf, "<input hidden class=inputbox name=play>", 1, 0);
	buf = ostrcat(buf, "<input class=inputbox style='width:360px' name=\"cmd\" style=\"overflow:hidden;\">", 1, 0);
	buf = ostrcat(buf, "<br><br>", 1, 0);
	buf = ostrcat(buf, "<input class=button type=submit value=\"", 1, 0);
	buf = ostrcat(buf, _("Send"), 1, 0);
	buf = ostrcat(buf, "\"></input>", 1, 0);

	buf = ostrcat(buf, "&nbsp;", 1, 0);
	buf = ostrcat(buf, "<input class=button type=reset name=reset value=\"", 1, 0);
	buf = ostrcat(buf, _("Reset"), 1, 0);
	buf = ostrcat(buf, "\"></input>", 1, 0);

	buf = ostrcat(buf, "</form>", 1, 0);


//	debug(10, "cmd: %s", param);
//	tmpstr = command(param);
	buf = ostrcat(buf, tmpstr, 1, 0);
	free(tmpstr), tmpstr = NULL;

	buf = string_replace_all("\n", "<br>", buf, 1);
	buf = string_replace_all("<br>", "<br>\n", buf, 1);

//	if(fmt == 0)
		buf = ostrcat(buf, "</center></body></html>", 1, 0);

	return buf;
}

char* webgetcommand(char* param, int fmt)
{
	char* buf = NULL, *tmpstr = NULL, *cmd = NULL;
	if(param == NULL) return NULL;
	if(status.security == 0) return NULL;

	cmd = ostrcat(param + 4, NULL, 0, 0);

	if(cmd == NULL) return NULL;
	htmldecode(cmd, cmd);
	cmd = string_replace_all("+", " ", cmd, 1);

//	if(fmt == 0)
//	{
		buf = ostrcat(buf, "<html><head><meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\">", 1, 0);
		buf = ostrcat(buf, "<link rel=stylesheet type=text/css href=titan.css><script type=text/javascript src=titan.js></script>", 1, 0);
		buf = ostrcat(buf, "</head><body class=body id=\"command\"><center>", 1, 0);
		buf = ostrcat(buf, "<br>", 1, 0);
		buf = ostrcat(buf, "<h1>", 1, 0);
		buf = ostrcat(buf, cmd, 1, 0);
		buf = ostrcat(buf, "</h1>", 1, 0);
//	}

	buf = ostrcat(buf, "<br><br>", 1, 0);

	debug(10, "cmd: %s", cmd);
	tmpstr = command(cmd);
	printf("webgetcommand: tmpstr: %s\n", tmpstr);

	buf = ostrcat(buf, tmpstr, 1, 0);
	free(tmpstr), tmpstr = NULL;
	free(cmd), cmd = NULL;

	buf = string_replace_all("\n", "<br>", buf, 1);
	buf = string_replace_all("<br>", "<br>\n", buf, 1);

//	if(fmt == 0)
		buf = ostrcat(buf, "</center></body></html>", 1, 0);

	return buf;
}

#endif
