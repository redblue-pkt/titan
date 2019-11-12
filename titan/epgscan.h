#ifndef EPGSCAN_H
#define EPGSCAN_H

void epgscandeltimer()
{
	struct rectimer *recnode = rectimer, *prev = rectimer;

	while(recnode != NULL)
	{
		prev = recnode;
		recnode = recnode->next;

		if(prev->status == 4 || prev->status == 5)
			delrectimer(prev, 0, 0);
	}

	writerectimer(getconfig("rectimerfile", NULL), 0);
}

void epgscancreatetimer()
{
	int daylight = 0;
	struct tm* loctime = NULL;
	time_t akttime = time(NULL), starttime = 0;
	char* buf = NULL;
	struct rectimer *recnode = rectimer, *prev = rectimer;

	if(ostrcmp(getconfig("epg_refreshtime", NULL), "0") == 0)
		return;

	buf = malloc(12);
	if(buf == NULL)
	{
		err("no memory");
		return;
	}

	loctime = olocaltime(&akttime);
	if(loctime == NULL)
	{
		free(buf);
		return;
	}

	daylight = loctime->tm_isdst;
	loctime->tm_sec = 0;

	strftime(buf, MINMALLOC, "%d-%m-%Y ", loctime);
	buf = ostrcat(buf, getconfig("epg_refreshtime", NULL), 1, 0);

	loctime->tm_isdst = daylight;
	if(strptime(buf, "%d-%m-%Y %H:%M", loctime) != NULL)
		starttime = mktime(loctime);

	free(loctime); loctime = NULL;
	free(buf); buf = NULL;
	if(starttime < akttime) starttime += 86400;

	if(starttime > akttime)
	{
		//check if timer exists
		while(recnode != NULL)
		{
			prev = recnode;
			recnode = recnode->next;

			if(prev->status == 4 || prev->status == 5)
				delrectimer(prev, 0, 0);
		}

		//add akt epg scan timer
		buf = ostrcat(buf, "<epgscan begin=", 1, 0);
		buf = ostrcat(buf, olutoa(starttime - 300), 1, 1);
		buf = ostrcat(buf, " end=", 1, 0);
		buf = ostrcat(buf, olutoa(starttime - 300 + 180), 1, 1);
		buf = ostrcat(buf, " afterevent=", 1, 0);
		buf = ostrcat(buf, oitoa(getconfigint("epg_afterevent", NULL)), 1, 1);
		buf = ostrcat(buf, " status=4 timestamp=", 1, 0);
		buf = ostrcat(buf, gettimestamp(), 1, 1);
		buf = ostrcat(buf, "></epgscan>", 1, 0);
		addrectimer(buf);
		free(buf); buf = NULL;

		//add next epg scan timer
		buf = ostrcat(buf, "<epgscan begin=", 1, 0);
		buf = ostrcat(buf, olutoa((starttime + 86400) - 300), 1, 1);
		buf = ostrcat(buf, " end=", 1, 0);
		buf = ostrcat(buf, olutoa((starttime + 86400) - 300 + 180), 1, 1);
		buf = ostrcat(buf, " afterevent=", 1, 0);
		buf = ostrcat(buf, oitoa(getconfigint("epg_afterevent", NULL)), 1, 1);
		buf = ostrcat(buf, " status=4 timestamp=", 1, 0);
		buf = ostrcat(buf, gettimestamp(), 1, 1);
		buf = ostrcat(buf, "></epgscan>", 1, 0);
		addrectimer(buf);
		free(buf); buf = NULL;

		writerectimer(getconfig("rectimerfile", NULL), 0);
	}
}

void epgscanlistclearscantime()
{
	struct epgscanlist *node = epgscanlist;

	while(node != NULL)
	{
		node->scantime = 0;
		node = node->next;
	}
}

struct epgscanlist* getepgscanlistbytransponder(uint64_t transponderid)
{
	struct epgscanlist *node = epgscanlist;

	while(node != NULL)
	{
		if(node->transponderid == transponderid)
			return node;

		node = node->next;
	}
	debug(100, "epgscanlist not found (transponderid=%llu)", transponderid);
	return NULL;
}

void epgscanlistthread(struct stimerthread* self)
{
	int epgdelete = 0, startmode = 0;
	struct epgscanlist* node = epgscanlist, *tmpepgscannode = NULL;
	struct dvbdev* fenode = NULL;
	struct channel* chnode = NULL;
	char* tmpstr = NULL, *chname = NULL;
	FILE* fd = NULL;

	debug(400, "epgscan thread start, wait for right time");

	//fd = fopen(EPGSCANLOG, "w");
	//if(fd == NULL)
	//{
	//	perr("can't open %s", EPGSCANLOG);
	//}
	//else
	//	fprintf(fd, "epgscan thread start, wait for right time\n");

	if(status.standby != 0) startmode = 1;

	if(status.standby == 0)
	{
		if(status.epglistmode == 0 || status.epglistmode == 2)
		{
			if(fd != NULL) fprintf(fd, "epgscan not configured to scan idle tuner\n");
			goto end;
		}
	}
	else if(getconfig("epg_refreshtime", NULL) == NULL)
	{
		if(fd != NULL) fprintf(fd, "epgscan not configured to scan in standby\n");
		goto end;
	}

	fd = fopen(EPGSCANLOG, "w");
	if(fd == NULL)
	{
		perr("can't open %s", EPGSCANLOG);
	}
	else
		fprintf(fd, "epgscan thread start, wait for right time\n");

	//wait for right time
	while(self->aktion != STOP && time(NULL) < 1072224000) // 01.01.2004
		usleep(1 * 1000000);

	debug(400, "epgscan time ok");
	if(fd != NULL)
	{
		tmpstr = ostrcat("epgscan time ok ", gettime(NULL, "%d-%m-%Y %H:%M"), 0, 1);
		if(tmpstr != NULL) fprintf(fd, "%s\n", tmpstr);
		free(tmpstr); tmpstr = NULL;
	}

	if(getconfigint("delepgbeforescan", NULL) == 1)
	{
		debug(400, "del epg before scan");
		if(fd != NULL) fprintf(fd, "del epg before scan\n");
		resetepg(1);
		epgdelete = 1;
	}

	while(node != NULL && self->aktion != STOP)
	{

		//wait for end recording
		while(self->aktion != STOP && (status.recording > 0 || status.timeshift > 0))
			usleep(1 * 1000000);
		if(self->aktion == STOP) break;

		chnode = getchannel(node->serviceid, node->transponderid);
		if(chnode == NULL)
		{
			debug(400, "epgscan channel not found sid=%d, tid=%llu", node->serviceid, node->transponderid);
			if(fd != NULL) fprintf(fd, "channel not found sid=%d, tid=%llu\n", node->serviceid, node->transponderid);
			node = node->next;
			continue;
		}

		if(chnode->name != NULL)
			chname = chnode->name;
		else
			chname = "unknown";

		if(epgdelete == 0 && chnode == status.aktservice->channel)
		{
			debug(400, "epgscan channel same as aktchannel channel=%s sid=%d, tid=%llu", chname, node->serviceid, node->transponderid);
			if(fd != NULL) fprintf(fd, "channel same as active channel channel=%s sid=%d, tid=%llu\n", chname, node->serviceid, node->transponderid);
			node = node->next;
			continue;
		}

		tmpepgscannode = getepgscanlistbytransponder(node->transponderid);
		if(tmpepgscannode != NULL && tmpepgscannode->scantime != 0)
		{
			debug(400, "epgscan transponer already scanned channel=%s tid=%llu", chname, node->transponderid);
			if(fd != NULL) fprintf(fd, "transponder already scanned channel=%s tid=%llu\n", chname, node->transponderid);
			node = node->next;
			continue;
		}

		if(status.standby > 0)
			fenode = fegetfree(chnode->transponder, 0, NULL, NULL);
		else
			fenode = fegetfree(chnode->transponder, 2, NULL, NULL);
		if(fenode == NULL || (status.standby == 0 && fenode == status.aktservice->fedev))
		{
			debug(400, "epgscan no free frontend found channel=%s", chname);
			if(fd != NULL) fprintf(fd, "no free frontend found channel=%s\n", chname);
			node = node->next;
			continue;
		}

		if(fenode->felock == 0)
		{
			//frontend tune
			if(fenode->feinfo->type == FE_QPSK)
			{
				feset(fenode, chnode->transponder);
				fetunedvbs(fenode, chnode->transponder);
			}
			else if(fenode->feinfo->type == FE_QAM)
				fetunedvbc(fenode, chnode->transponder);
			else if(fenode->feinfo->type == FE_OFDM)
				fetunedvbt(fenode, chnode->transponder);
			else
			{
				debug(400, "epgscan unknown frontend channel=%s", chname);
				if(fd != NULL) fprintf(fd, "unknown frontend channel=%s\n", chname);
				node = node->next;
				continue;
			}

			int festatus = fewait(fenode);
			if(festatus != 0)
			{
				debug(400, "epgscan frontend tune failed channel=%s", chname);
				if(fd != NULL) fprintf(fd, "frontend tune failed channel=%s\n", chname);
				node = node->next;
				continue;
			}

			debug(400, "epgscan scanning channel=%s, sid=%d, tid=%llu, frontend=%s", chname, chnode->serviceid, chnode->transponderid, fenode->feshortname);
			if(fd != NULL) fprintf(fd, "scanning channel=%s, sid=%d, tid=%llu, frontend=%s\n", chname, chnode->serviceid, chnode->transponderid, fenode->feshortname);

			if(chnode->transponder != NULL) chnode->transponder->lastepg = 0;
			readeit(self, chnode, fenode, 1);
			if(getconfigint("mhwepg", NULL) != 0)
				readmhw(self, chnode, fenode, 1);
			if(getconfigint("mhw2epg", NULL) != 0)
				readmhw2(self, chnode, fenode, 1);
			if(getconfigint("opentv", NULL) != 0)
				readopentv(self, chnode, fenode, 0);
			node->scantime = time(NULL);
		}
		else
		{
			debug(400, "epgscan frontend locked channel=%s frontend=%s", chname, fenode->feshortname);
			if(fd != NULL) fprintf(fd, "frontend locked channel=%s frontend=%s\n", chname, fenode->feshortname);
		}

		node = node->next;
	}

	//write only epg
	writeallconfig(2);

end:
	epgscanlistclearscantime();

	if(fd != NULL)
	{
		fprintf(fd, "epgscan thread end\n");
		fclose(fd);
	}

	if(status.standby != 0 && startmode == 1 && getconfig("epg_refreshtime", NULL) != NULL)
	{
		int afterevent = getconfigint("epg_afterevent", NULL);

		if(afterevent == 1)
		{
			if(status.recording < 1)
				oshutdown(1, 3);
		}
	}

	status.epgscanlistthread = NULL;
	debug(400, "epgscan thread end");
}

struct epgscanlist* addepgscanlist(char *line, int count, struct epgscanlist* last)
{
	//debug(1000, "in");
	struct epgscanlist *newnode = NULL, *prev = NULL, *node = epgscanlist;
	int ret = 0;

	newnode = (struct epgscanlist*)calloc(1, sizeof(struct epgscanlist));
	if(newnode == NULL)
	{
		err("no memory");
		return NULL;
	}

	status.writeepgscanlist = 1;

	ret = sscanf(line, "%d#%llu", &newnode->serviceid, &newnode->transponderid);
	if(ret != 2)
	{
		if(count > 0)
		{
			err("epgscanlist line %d not ok", count);
		}
		else
		{
			err("add epgscanlist");
		}
		free(newnode);
		return NULL;
	}

	if(getchannel(newnode->serviceid, newnode->transponderid) == NULL)
	{
		err("epgscanlist line %d not ok (channel not found)", count);
		free(newnode);
		return NULL;
	}

	if(last == NULL)
	{
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
		epgscanlist = newnode;
	else
	{
		prev->next = newnode;
		newnode->prev = prev;
	}
	newnode->next = node;
	if(node != NULL) node->prev = newnode;

	//debug(1000, "out");
	return newnode;
}

int readepgscanlist(const char* filename)
{
	FILE *fd = NULL;
	char *fileline = NULL;
	int linecount = 0, len = 0;
	struct epgscanlist* last = NULL, *tmplast = NULL;

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
		last = addepgscanlist(fileline, linecount, last);
		if(last != NULL) tmplast = last;
	}

	status.writeepgscanlist = 0;
	free(fileline);
	fclose(fd);
	return 0;
}

int delepgscanlist(int serviceid, uint64_t transponderid)
{
	int ret = 1;
	struct epgscanlist *node = epgscanlist, *prev = epgscanlist;

	while(node != NULL)
	{
		if(node->serviceid == serviceid && node->transponderid == transponderid)
		{
			status.writeepgscanlist = 1;
			if(node == epgscanlist)
			{
				epgscanlist = node->next;
				if(epgscanlist != NULL)
					epgscanlist->prev = NULL;
			}
			else
			{
				prev->next = node->next;
				if(node->next != NULL)
					node->next->prev = prev;
			}

			free(node);
			node = NULL;
			ret = 0;
			break;
		}

		prev = node;
		node = node->next;
	}
	return ret;
}

struct epgscanlist* getepgscanlist(int serviceid, uint64_t transponderid)
{
	struct epgscanlist *node = epgscanlist;

	while(node != NULL)
	{
		if(node->serviceid == serviceid && node->transponderid == transponderid)
			return node;

		node = node->next;
	}
	debug(100, "epgscanlist not found (serviceid=%d, transponderid=%llu)", serviceid, transponderid);
	return NULL;
}

void freeepgscanlist()
{
	struct epgscanlist *node = epgscanlist, *prev = epgscanlist;

	while(node != NULL)
	{
		prev = node;
		node = node->next;
		if(prev != NULL)
			delepgscanlist(prev->serviceid, prev->transponderid);
	}
}

int writeepgscanlist(const char *filename)
{
	FILE *fd = NULL;
	struct epgscanlist *node = epgscanlist;
	int ret = 0;

	fd = fopen(filename, "w");
	if(fd == NULL)
	{
		perr("can't open %s", filename);
		return 1;
	}

	while(node != NULL)
	{
		ret = fprintf(fd, "%d#%llu\n", node->serviceid, node->transponderid);
		if(ret < 0)
		{
			perr("writing file %s", filename);
		}
		node = node->next;
	}

	fclose(fd);
	return 0;
}

#endif
