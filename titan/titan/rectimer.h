#ifndef RECTIMER_H
#define RECTIMER_H

void debugrectimer()
{
	struct rectimer* node = rectimer;

	while(node != NULL)
	{
		printf("start: %lu end: %lu\n", node->begin, node->end);
		node = node->next;
	}
}

//flag 0 = set mutex
//flag 1 = don't set mutex
void changerectimerbegin(struct rectimer* recnode, time_t begin, int flag)
{
	struct rectimer* prev = NULL, *node = NULL;

	if(recnode == NULL) return;

	if(flag == 0) m_lock(&status.rectimermutex, 1);

	recnode->begin = begin;

	//aushaengen first node
	if(recnode->prev == NULL)
		rectimer = recnode->next;

	//aushaengen last node
	if(recnode->next == NULL && recnode->prev != NULL)
		recnode->prev->next = NULL;

	//recnode aushaengen
	if(recnode->prev != NULL)
		recnode->prev->next = recnode->next;
	if(recnode->next != NULL)
		recnode->next->prev = recnode->prev;

	node = rectimer;
	recnode->next = NULL;
	recnode->prev = NULL;

	//resort rectimer
	while(node != NULL && begin >= node->begin)
	{
		prev = node;
		node = node->next;
	}

	//recnode einhaengen
	if(prev == NULL)
		rectimer = recnode;
	else
	{
		prev->next = recnode;
		recnode->prev = prev;
	}
	recnode->next = node;
	if(node != NULL) node->prev = recnode;

	if(flag == 0) m_unlock(&status.rectimermutex, 1);
}

int checkrectimeradd(struct rectimer* recnode, char** ret)
{
	struct rectimer* node = rectimer;

	if(recnode == NULL)
	{
		*ret = _("undefined error");
		return 1;
	}
	if(recnode->begin == 0 || recnode->end == 0)
	{
		*ret = _("Timer starttime or endtime not ok");
		return 1;
	}
	if(recnode->end <= time(NULL))
	{
		*ret = _("Timer endtime shorter than aktual time");
		return 1;
	}
	if(recnode->end <= recnode->begin)
	{
		*ret = _("Timer endtime shorter than start time");
		return 1;
	}
	if(recnode->serviceid < 1)
	{
		*ret = _("Timer service ID not ok");
		return 1;
	}
	if(recnode->transponderid < 1)
	{
		*ret = _("Timer transponder ID not ok");
		return 1;
	}

	while(node != NULL)
	{
		if(recnode != node && node->status < 2)
		{
			if((recnode->begin >= node->begin && recnode->begin < node->end) || (recnode->end >= node->begin && recnode->end < node->end))
			{
				*ret = "Timer conflicts with other timer\nWould you add the timer?";
				return 2;
			}
		}
		node = node->next;
	}

	return 0;
}

//flag 0: set mutex in called funktions
//flag 1: not set mutex in called funktions
void recrepeatecalc(struct rectimer* node, int flag)
{
	time_t daysec = 24 * 60 * 60;
	struct tm* loctime = NULL;
	int rectimerday = -1, rep = 0, i;

	if(node->repeate == 0) return;

	//get rectimer start day
	loctime = olocaltime(&node->begin);
	rectimerday = loctime->tm_wday - 1;
	free(loctime); loctime = NULL;
	if(rectimerday == -1) rectimerday = 6;

	rep = (node->repeate & 0xff) | ((node->repeate  & 0xff) << 7);

	for(i = 0; i < 7; i++)
	{
		changerectimerbegin(node, node->begin + daysec, flag);
		node->end = node->end + daysec;
		rectimerday++;
		if(rectimerday > 6) rectimerday = 0;
		if(checkbit(rep, rectimerday) == 1)
			break;
	}

	//check if begin > akttime
	while(node->begin < time(NULL))
	{
		for(i = 0; i < 7; i++)
		{
			changerectimerbegin(node, node->begin + daysec, flag);
			node->end = node->end + daysec;
			rectimerday++;
			if(rectimerday > 6) rectimerday = 0;
			if(checkbit(rep, rectimerday) == 1)
				break;
		}
	}
}

//flag 0: set mutex in called funktions
//flag 1: not set mutex in called funktions
int checkrectimertime(struct rectimer* node, int flag)
{
	//don't load old timers
	if(time(NULL) - node->end > 604800 && (node->status == 2 || node->status == 3))
		return 1;

	if(time(NULL) > node->end && node->repeate == 0 && node->status == 0)
		return 1;

	//recalc old repeat timer
	if(time(NULL) > node->end && node->repeate != 0 && node->status == 0)
		recrepeatecalc(node, flag);

	return 0;
}

void copyrectimer(struct rectimer* src, struct rectimer* dst)
{
	dst->timestamp = ostrcat(src->timestamp, NULL, 0, 0);
	dst->name = ostrcat(src->name, NULL, 0, 0);
	changerectimerbegin(dst, src->begin, 1);
	dst->end = src->end;
	dst->repeate = src->repeate;
	dst->afterevent = src->afterevent;
	dst->disabled = src->disabled;
	dst->justplay = src->justplay;
	dst->recpath = ostrcat(src->recpath, NULL, 0, 0);
	dst->servicetype = src->servicetype;
	dst->channellist = ostrcat(src->channellist, NULL, 0, 0);
	dst->serviceid = src->serviceid;
	dst->transponderid = src->transponderid;
}

//flag 0: set mutex in called funktions
//flag 1: not set mutex in called funktions
struct rectimer* addrectimernode(char* line, struct rectimer* last, int flag)
{
	debug(1000, "in");
	char *ret = NULL;
	struct rectimer *newnode = NULL, *prev = NULL, *node = rectimer;
	
	newnode = (struct rectimer*)calloc(1, sizeof(struct rectimer));
	if(newnode == NULL)
	{
		err("no memory");
		return NULL;
	}

	status.writerectimer = 1;
	newnode->timestamp = NULL;

	if(line != NULL)
	{
		ret = getxmlentry(line, " begin=");
		if(ret != NULL)
		{
			newnode->begin = atol(ret);
			free(ret);
		}
		ret = getxmlentry(line, " end=");
		if(ret != NULL)
		{
			newnode->end = atol(ret);
			free(ret);
		}
		ret = getxmlentry(line, " repeated=");
		if(ret != NULL)
		{
			newnode->repeate = atoi(ret);
			free(ret);
		}
		ret = getxmlentry(line, " afterevent=");
		if(ret != NULL)
		{
			newnode->afterevent = atoi(ret);
			free(ret);
		}
		ret = getxmlentry(line, " pincode=");
		if(ret != NULL)
			newnode->pincode = ret;
		ret = getxmlentry(line, " disabled=");
		if(ret != NULL)
		{
			newnode->disabled = atoi(ret);
			free(ret);
		}
		ret = getxmlentry(line, " justplay=");
		if(ret != NULL)
		{
			newnode->justplay = atoi(ret);
			free(ret);
		}
		ret = getxmlentry(line, " serviceid=");
		if(ret != NULL)
		{
			newnode->serviceid = atoi(ret);
			free(ret);
		}
		ret = getxmlentry(line, " transponderid=");
		if(ret != NULL)
		{
			newnode->transponderid = atol(ret);
			free(ret);
		}
		ret = getxmlentry(line, " status=");
		if(ret != NULL)
		{
			newnode->status = atol(ret);
			if(newnode->status == 1) newnode->status = 0;
			free(ret);
		}
		ret = getxmlentry(line, " timestamp=");
		if(ret != NULL)
		{
			newnode->timestamp = ret;
		}
		ret = getxmlentry(line, " name=");
		if(ret != NULL)
			newnode->name = ret;
		ret = getxmlentry(line, " recpath=");
		if(ret != NULL)
			newnode->recpath = ret;
		ret = getxmlentry(line, " channellist=");
		if(ret != NULL)
			newnode->channellist = ret;
		ret = getxmlentry(line, " errstr=");
		if(ret != NULL)
			newnode->errstr = ret;

		if(checkrectimertime(newnode, flag) == 1)
		{
			free(newnode);
			return NULL;
		}
	}
	else
	{
		newnode->disabled = 1;
		newnode->afterevent = getconfigint("def_rectimer_after", NULL);
	}
	
	if(newnode->timestamp == NULL) 
		newnode->timestamp = gettimestamp();
	
	if(last == NULL)
	{
		while(node != NULL && newnode->begin >= node->begin)
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
		rectimer = newnode;
	else
	{
		prev->next = newnode;
		newnode->prev = prev;
	}
	newnode->next = node;
	if(node != NULL) node->prev = newnode;

	debug(1000, "out");
	return newnode;
}

struct rectimer* getrectimerbytimediff(time_t timediff)
{
	struct rectimer* node = rectimer;	
	time_t akttime = time(NULL);

	while(node != NULL)
	{
		if(node->begin > akttime && node->begin <= akttime + timediff)
			return node;

		node = node->next;
	}

	return NULL;
}

struct rectimer* getrectimerbytimestamp(char* timestamp)
{
	struct rectimer* node = rectimer;	

	while(node != NULL)
	{
		if(ostrcmp(node->timestamp, timestamp) == 0)
			return node;

		node = node->next;
	}

	return NULL;
}

struct rectimer* getrectimerbyservice(struct service* servicenode)
{
	struct rectimer* node = rectimer;	

	while(node != NULL)
	{
		if(node->servicenode == servicenode)
			return node;

		node = node->next;
	}

	return NULL;
}

void checkrectimer()
{
	int ret = 0;
	struct rectimer* node = NULL, *newnode = NULL;
	struct channel* chnode = NULL;
	time_t t = time(NULL), begin = 0, end = 0;

	m_lock(&status.rectimermutex, 1);
	node = rectimer;
	
	while(node != NULL)
	{
		if(node->disabled != 0)
		{
			node = node->next;
			continue;
		}
		
		if(node->justplay == 1)
		{
			begin = node->begin;
			end = node->end + 120;
		}
		else
		{
			begin = node->begin;
			end = node->end;
		}

		if(t >= begin && t < end && node->disabled == 0 && node->status == 0)
		{
			if(node->repeate != 0)
			{
				//copy repeate timer
				newnode = addrectimernode(NULL, NULL, 1);
				if(newnode != NULL)
				{
					copyrectimer(node, newnode);
					recrepeatecalc(newnode, 1);
				}
				node->repeate = 0;
				status.writerectimer = 1;
			}
			chnode = getchannel(node->serviceid, node->transponderid);
			if(node->justplay == 1)
			{
				ret = servicestart(chnode, NULL, node->channellist, 0);
				if(ret == 20 || ret == 22) ret = 0;
			}
			else
			{
				ret = recordstart(chnode, -1, 0, RECTIMER, node->end, node);
				if(ret == 14) ret = 0;
			}

			if(ret != 0)
			{
				node->status = 3;
				status.writerectimer = 1;
				break;
			}
			else
			{
				if(node->justplay == 1)
					node->status = 2;
				else
					node->status = 1;
				status.writerectimer = 1;
			}
		}
		if(t > node->end && node->status == 0 && node->end != 0)
		{
			node->status = 3;
			free(node->errstr);
			node->errstr = ostrcat(_("not started akttime greater timer endtime"), NULL, 0, 0);
			status.writerectimer = 1;
		}
		node = node->next;
	}

	m_unlock(&status.rectimermutex, 1);

	if(node != NULL && node->disabled == 0)
	{
		if(node->justplay == 1)
		{
			char* retstr = servicecheckret(ret, 1);
			free(node->errstr);
			node->errstr = ostrcat(retstr, NULL, 0, 0);
			stringreplacecharonce(node->errstr, '\n', '\0');
			free(retstr); retstr = NULL;
		}
		else
		{
			char* retstr = recordcheckret(NULL, ret, 15);
			free(node->errstr);
			node->errstr = ostrcat(retstr, NULL, 0, 0);
			stringreplacecharonce(node->errstr, '\n', '\0');
			free(retstr); retstr = NULL;
		}
		status.writerectimer = 1;
	}

	writerectimer(getconfig("rectimerfile", NULL), 0);
}

int addrectimer(char *buf)
{
	debug(1000, "in");
	struct rectimer* node = NULL;
	char* buf1 = buf;
	char* line = NULL;

	line = malloc(MINMALLOC);
	if(line == NULL)
	{
		err("no memory");
		return 1;
	}

	while(*buf != '\0')
	{
		buf++;
		if(*buf == '\n' || *buf == '\0')
		{
			if(*buf == '\n')
			{
				buf++;
				snprintf(line, buf - buf1, "%s", buf1);
			}
			else
				snprintf(line, buf - buf1 + 1, "%s", buf1);
			buf1 = buf;

			if(ostrstr(line, "<timer ") != NULL || ostrstr(line, "<log "))
				node = addrectimernode(line, node, 0);
		}
	}
	free(line);
	debug(1000, "out");
	return 0;
}

int readrectimer(char *filename)
{
	debug(1000, "in");
	char *buf = NULL;

	buf = readfiletomem(filename, 1);

	if(buf != NULL)
	{
		addrectimer(buf);
		//timers can change on load, so we save it
		writerectimer(getconfig("rectimerfile", NULL), 0);
		free(buf);
	}
	debug(1000, "out");
	return 0;
}

//flag 0 = set mutex
//flag 1 = don't set mutex
void delrectimer(struct rectimer* rectimernode, int write, int flag)
{
	debug(1000, "in");

	if(flag == 0) m_lock(&status.rectimermutex, 1);
  struct rectimer *node = rectimer, *prev = rectimer;

	while(node != NULL)
	{
		if(node == rectimernode)
		{
			status.writerectimer = 1;

			//record is aktiv (del from stopservice)
			if(node->servicenode != NULL)
			{
				node->status = 2;
				free(node->errstr);
				node->errstr = ostrcat(_("stopped from user"), NULL, 0, 0);
				//stop recording
				node->servicenode->recendtime = 1;
				if(flag == 0)
					m_unlock(&status.rectimermutex, 1);
				return;
			}

			if(node == rectimer)
			{
				rectimer = node->next;
				if(rectimer != NULL)
					rectimer->prev = NULL;
			}
			else
			{
				prev->next = node->next;
				if(node->next != NULL)
					node->next->prev = prev;
			}

			free(node->name);
			node->name = NULL;
			
			free(node->timestamp);
			node->timestamp = NULL;

			free(node->recpath);
			node->recpath = NULL;

			free(node->channellist);
			node->channellist = NULL;

			free(node->errstr);
			node->errstr = NULL;
			
			free(node->pincode);
			node->pincode = NULL;

			free(node);
			node = NULL;
			break;
		}

		prev = node;
		node = node->next;
	}

	if(write == 1)
		writerectimer(getconfig("rectimerfile", NULL), 1);

	if(flag == 0)
		m_unlock(&status.rectimermutex, 1);
	debug(1000, "out");
}

void deloldrectimer()
{
	debug(1000, "in");
	struct rectimer *node = rectimer, *prev = rectimer;

	while(node != NULL)
	{
		prev = node;
		node = node->next;
		if(prev != NULL && checkrectimertime(prev, 0) == 1)
			delrectimer(prev, 1, 0);
	}
	debug(1000, "out");
}

void freerectimer()
{
	debug(1000, "in");
	struct rectimer *node = rectimer, *prev = rectimer;

	while(node != NULL)
	{
		prev = node;
		node = node->next;
		if(prev != NULL)
			delrectimer(prev, 0, 0);
	}
	debug(1000, "out");
}

//flag 0: lock
//flag 1: don't lock
int writerectimer(const char *filename, int flag)
{
	debug(1000, "in");
	FILE *fd = NULL;
	struct rectimer *node = NULL;
	int ret = 0;
	char* type = NULL;
	time_t curtime = time(NULL), rectime = 0x7FFFFFFF;

	if(status.writerectimer == 0) return 0;

	if(flag == 0) m_lock(&status.rectimermutex, 1);
  node = rectimer;

	fd = fopen(filename, "w");
	if(fd == NULL)
	{
		if(flag == 0) m_unlock(&status.rectimermutex, 1);
		status.writerectimer = 0;
		perr("can't open %s", filename);
		return 1;
	}

	while(node != NULL)
	{
		//don't write for del marked or old timers
		if(checkrectimertime(node, 1) == 1)
		{
			node = node->next;
			continue;
		}

		if(node->name == NULL) node->name = ostrcat(node->name, NULL, 1, 0);
		if(node->channellist == NULL) node->channellist = ostrcat(node->channellist, NULL, 1, 0);
		if(node->errstr == NULL) node->errstr = ostrcat(node->errstr, NULL, 1, 0);
		if(node->pincode == NULL) node->pincode = ostrcat(node->pincode, "0000", 1, 0);
		if(node->recpath == NULL) node->recpath = ostrcat(node->recpath, NULL, 1, 0);
		if(node->status == 0 || node->status == 1)
			type = "timer";
		else
			type = "log";

		ret = fprintf(fd, "<%s begin=\"%lu\" end=\"%lu\" serviceid=\"%d\" transponderid=\"%lu\" channellist=\"%s\" repeated=\"%d\" name=\"%s\" afterevent=\"%d\" pincode=\"%s\" disabled=\"%d\" justplay=\"%d\", recpath=\"%s\", timestamp=\"%s\", status=\"%d\" errstr=\"%s\">\n</%s>\n", type, node->begin, node->end, node->serviceid, node->transponderid, node->channellist, node->repeate, node->name, node->afterevent, node->pincode, node->disabled, node->justplay, node->recpath, node->timestamp, node->status, node->errstr, type);

		if(ret < 0)
		{
			perr("writting file %s", filename);
		}

		//get wakeup time
		if(node->status == 0 || node->status == 1)
		{
			if(node->begin < rectime && node->begin > curtime)
				rectime = node->begin;
		}
		node = node->next;
	}

	fclose(fd);
	//write wakeup time to proc
	setwakeuptimerdev(rectime);

	if(flag == 0) m_unlock(&status.rectimermutex, 1);

	status.writerectimer = 0;
	debug(1000, "out");
	return 0;
}

int recrepeateget(int newnode, struct skin* repeate, struct skin* repeatetype, struct skin* day, struct skin* mon, struct skin* tue, struct skin* wed, struct skin* thur, struct skin* fri, struct skin* sat, struct skin* sun)
{
	unsigned char ret = 0;
	int aktday = rectimergetaktday();

	if(repeate == NULL) return 0;
	if(repeatetype == NULL) return 0;

	if(ostrcmp(repeate->ret, "0") == 0) return 0;

	if(ostrcmp(repeatetype->ret, "0") == 0) return 127;
	if(ostrcmp(repeatetype->ret, "1") == 0)
	{
		if(day->ret == NULL) return aktday;
		if(ostrcmp(day->ret, "0") == 0)
		{
			if(newnode == 1)
				return aktday;
			else
				return 1;
		}
		if(ostrcmp(day->ret, "1") == 0) return 2;
		if(ostrcmp(day->ret, "2") == 0) return 4;
		if(ostrcmp(day->ret, "3") == 0) return 8;
		if(ostrcmp(day->ret, "4") == 0) return 16;
		if(ostrcmp(day->ret, "5") == 0) return 32;
		if(ostrcmp(day->ret, "6") == 0) return 64;
	}
	if(ostrcmp(repeatetype->ret, "2") == 0) return 31;
	if(ostrcmp(repeatetype->ret, "3") == 0)
	{
		if(mon == NULL || tue == NULL || wed == NULL || thur == NULL || fri == NULL || sat == NULL || sun == NULL) return aktday;
		if(ostrcmp(mon->ret, "1") == 0) ret = setbit(ret, 0);
		if(ostrcmp(tue->ret, "1") == 0) ret = setbit(ret, 1);
		if(ostrcmp(wed->ret, "1") == 0) ret = setbit(ret, 2);
		if(ostrcmp(thur->ret, "1") == 0) ret = setbit(ret, 3);
		if(ostrcmp(fri->ret, "1") == 0) ret = setbit(ret, 4);
		if(ostrcmp(sat->ret, "1") == 0) ret = setbit(ret, 5);
		if(ostrcmp(sun->ret, "1") == 0) ret = setbit(ret, 6);
		if(ret == 0) ret = aktday;
		return ret;
	}

	return 127;
}

void recjustplayhidden(int justplay, struct skin* end, struct skin* path, struct skin* after)
{
	if(justplay == 0)
	{
		end->hidden = NO;
		path->hidden = NO;
		after->hidden = NO;
	}
	else
	{
		end->hidden = YES;
		path->hidden = YES;
		after->hidden = YES;
	}
}

int rectimergetaktday()
{
	int ret = 1;
	struct tm* loctime = NULL;

	time_t akttime = time(NULL);
	loctime = olocaltime(&akttime);
	if(loctime->tm_wday == 0) ret = 64;
	if(loctime->tm_wday == 1) ret = 1;
	if(loctime->tm_wday == 2) ret = 2;
	if(loctime->tm_wday == 3) ret = 4;
	if(loctime->tm_wday == 4) ret = 8;
	if(loctime->tm_wday == 5) ret = 16;
	if(loctime->tm_wday == 6) ret = 32;
	free(loctime); loctime = NULL;

	return ret;
}

void recrepeatehidden(int repeate, struct skin* repeatetype, struct skin* day, struct skin* mon, struct skin* tue, struct skin* wed, struct skin* thur, struct skin* fri, struct skin* sat, struct skin* sun)
{
	repeatetype->hidden = YES;
	day->hidden = YES;
	mon->hidden = YES;
	tue->hidden = YES;
	wed->hidden = YES;
	thur->hidden = YES;
	fri->hidden = YES;
	sat->hidden = YES;
	sun->hidden = YES;

	if(repeate == 0) return;

	if(repeate == 127 && ostrcmp(repeatetype->ret, "3") != 0) //dayly
	{
		repeatetype->hidden = NO;
		setchoiceboxselection(repeatetype, "0");
		return;
	}
	if(repeate == 31 && ostrcmp(repeatetype->ret, "3") != 0) //workday
	{
		repeatetype->hidden = NO;
		setchoiceboxselection(repeatetype, "2");
		return;
	}
	if((repeate == 1 || repeate == 2 || repeate == 4 || repeate == 8 || repeate == 16 || repeate == 32 || repeate == 64) && ostrcmp(repeatetype->ret, "3") != 0) //weekly
	{
		repeatetype->hidden = NO;
		setchoiceboxselection(repeatetype, "1");
		day->hidden = NO;
		if(repeate == 1) setchoiceboxselection(day, "0");
		else if(repeate == 2) setchoiceboxselection(day, "1");
		else if(repeate == 4) setchoiceboxselection(day, "2");
		else if(repeate == 8) setchoiceboxselection(day, "3");
		else if(repeate == 16) setchoiceboxselection(day, "4");
		else if(repeate == 32) setchoiceboxselection(day, "5");
		else if(repeate == 64) setchoiceboxselection(day, "6");
		return;
	}

	//user defined
	repeatetype->hidden = NO;
	setchoiceboxselection(repeatetype, "3");
	mon->hidden = NO;
	tue->hidden = NO;
	wed->hidden = NO;
	thur->hidden = NO;
	fri->hidden = NO;
	sat->hidden = NO;
	sun->hidden = NO;
	if(checkbit(repeate, 0) == 1) setchoiceboxselection(mon, "1");
	if(checkbit(repeate, 1) == 1) setchoiceboxselection(tue, "1");
	if(checkbit(repeate, 2) == 1) setchoiceboxselection(wed, "1");
	if(checkbit(repeate, 3) == 1) setchoiceboxselection(thur, "1");
	if(checkbit(repeate, 4) == 1) setchoiceboxselection(fri, "1");
	if(checkbit(repeate, 5) == 1) setchoiceboxselection(sat, "1");
	if(checkbit(repeate, 6) == 1) setchoiceboxselection(sun, "1");
}

//flag 0: called from menu (manuel timer)
//flag 1: call from epg timer
void screenrectimerext(struct rectimer* node, int flag)
{
	int ret = 0, rcret = 0, edaylight = 0, bdaylight = 0, newnode = 0, tmpservicetype = 0, tmprepeate = 0;
	struct skin* rectimerext = getscreen("rectimerext");
	struct skin* listbox = getscreennode(rectimerext, "listbox");
	struct skin* name = getscreennode(rectimerext, "name");
	struct skin* justplay = getscreennode(rectimerext, "justplay");
	struct skin* repeate = getscreennode(rectimerext, "repeate");
	struct skin* repeatetype = getscreennode(rectimerext, "repeatetype");
	struct skin* day = getscreennode(rectimerext, "day");
	struct skin* mon = getscreennode(rectimerext, "mon");
	struct skin* tue = getscreennode(rectimerext, "tue");
	struct skin* wed = getscreennode(rectimerext, "wed");
	struct skin* thur = getscreennode(rectimerext, "thur");
	struct skin* fri = getscreennode(rectimerext, "fri");
	struct skin* sat = getscreennode(rectimerext, "sat");
	struct skin* sun = getscreennode(rectimerext, "sun");
	struct skin* begin = getscreennode(rectimerext, "begin");
	struct skin* end = getscreennode(rectimerext, "end");
	struct skin* recchannel = getscreennode(rectimerext, "channel");
	struct skin* path = getscreennode(rectimerext, "path");
	struct skin* after = getscreennode(rectimerext, "after");
	struct skin* pincode = getscreennode(rectimerext, "pincode");
	struct skin* tmp = NULL;
	char* tmpstr = NULL, *buf = NULL, *buf1 = NULL, *tmpchannellist = NULL;
	struct tm* loctime = NULL;
	time_t akttime = time(NULL);

	if(node == NULL)
	{
		newnode = 1;
		node = addrectimernode(NULL, NULL, 0);
		tmpservicetype = status.servicetype;
		if(tmpservicetype == 0)
			tmpchannellist = ostrcat(getconfig("channellist", NULL), NULL, 0, 0);
		else
			tmpchannellist = ostrcat(getconfig("rchannellist", NULL), NULL, 0, 0);
	}
	else
	{
		tmpservicetype = node->servicetype;
		tmpchannellist = ostrcat(node->channellist, NULL, 0, 0);
	}

	if(node == NULL)
	{
		err("NULL detect");
		return;
	}

	listbox->aktline = 1;
	listbox->aktpage = -1;
	begin->aktpage = 0;
	end->aktpage = 0;
	name->aktpage = 0;
	
	changeinput(name, node->name);

	addchoicebox(justplay, "0", _("record"));
	addchoicebox(justplay, "1", _("switch channel"));
	if(newnode == 0)
	{
		tmpstr = oitoa(node->justplay);
		setchoiceboxselection(justplay, tmpstr);
		free(tmpstr); tmpstr = NULL;
	}
	else
		setchoiceboxselection(justplay, "0");

	addchoicebox(repeate, "0", _("once"));
	addchoicebox(repeate, "1", _("repeate"));
	if(newnode == 0)
	{
		tmprepeate = node->repeate;
		if(node->repeate == 0)
			setchoiceboxselection(repeate, "0");
		else
			setchoiceboxselection(repeate, "1");
	}
	else
		setchoiceboxselection(repeate, "0");

	addchoicebox(repeatetype, "0", _("daily"));
	addchoicebox(repeatetype, "1", _("weekly"));
	addchoicebox(repeatetype, "2", _("workdays"));
	addchoicebox(repeatetype, "3", _("user defined"));
	changeret(repeatetype, NULL);
	setchoiceboxselection(repeatetype, "0");

	addchoicebox(day, "0", _("Monday"));
	addchoicebox(day, "1", _("Tuesday"));
	addchoicebox(day, "2", _("Wednesday"));
	addchoicebox(day, "3", _("Thursday"));
	addchoicebox(day, "4", _("Friday"));
	addchoicebox(day, "5", _("Saturday"));
	addchoicebox(day, "6", _("Sunday"));
	changeret(day, NULL);
	setchoiceboxselection(day, "0");

	addchoicebox(mon, "0", _("no"));
	addchoicebox(mon, "1", _("yes"));
	changeret(mon, NULL);
	setchoiceboxselection(mon, "0");

	addchoicebox(tue, "0", _("no"));
	addchoicebox(tue, "1", _("yes"));
	changeret(tue, NULL);
	setchoiceboxselection(tue, "0");

	addchoicebox(wed, "0", _("no"));
	addchoicebox(wed, "1", _("yes"));
	changeret(wed, NULL);
	setchoiceboxselection(wed, "0");

	addchoicebox(thur, "0", _("no"));
	addchoicebox(thur, "1", _("yes"));
	changeret(thur, NULL);
	setchoiceboxselection(thur, "0");

	addchoicebox(fri, "0", _("no"));
	addchoicebox(fri, "1", _("yes"));
	changeret(fri, NULL);
	setchoiceboxselection(fri, "0");

	addchoicebox(sat, "0", _("no"));
	addchoicebox(sat, "1", _("yes"));
	changeret(sat, NULL);
	setchoiceboxselection(sat, "0");

	addchoicebox(sun, "0", _("no"));
	addchoicebox(sun, "1", _("yes"));
	changeret(sun, NULL);
	setchoiceboxselection(sun, "0");

	recrepeatehidden(node->repeate, repeatetype, day, mon, tue, wed, thur, fri, sat, sun);
	recjustplayhidden(node->justplay, end, path, after);

	buf = malloc(MINMALLOC);
	if(buf == NULL)
	{
		err("no memory");
		return;
	}

	changemask(begin, "00:00 00-00-0000");
	if(newnode == 0)
		loctime = localtime(&node->begin);
	else
		loctime = localtime(&akttime);
	bdaylight = loctime->tm_isdst;
	strftime(buf, MINMALLOC, "%H:%M %d-%m-%Y", loctime);
	buf1 = ostrcat(buf, NULL, 0, 0);
	changeinput(begin, buf1);
	free(buf1); buf1 = NULL;

	changemask(end, "00:00 00-00-0000");
	if(newnode == 0)
		loctime = localtime(&node->end);
	else
	{
		akttime += 300;
		loctime = localtime(&akttime);
	}
	edaylight = loctime->tm_isdst;
	strftime(buf, MINMALLOC, "%H:%M %d-%m-%Y", loctime);
	buf1 = ostrcat(buf, NULL, 1, 0);
	changeinput(end, buf1);
	free(buf1); buf1 = NULL;

	if(flag == 1)
		recchannel->hidden = YES;
	else
		recchannel->hidden = NO;

	changeinput(recchannel, NULL);
	if(newnode == 0)
	{
		recchannel->handle = (char*)getchannel(node->serviceid, node->transponderid);
		if(recchannel->handle != NULL)
			addchoicebox(recchannel, ((struct channel*)recchannel->handle)->name, ((struct channel*)recchannel->handle)->name);
	}
	else
	{
		if(status.aktservice->channel != NULL)
		{
			addchoicebox(recchannel, status.aktservice->channel->name, status.aktservice->channel->name);
			recchannel->handle = (char*)status.aktservice->channel;
		}
	}

	if(newnode == 0)
		changeinput(path, node->recpath);
	else
		changeinput(path, getconfig("rec_path", NULL));

	addchoicebox(after, "0", _("auto"));
	addchoicebox(after, "1", _("nothing"));
	addchoicebox(after, "2", _("standby"));
	addchoicebox(after, "3", _("power off"));
	if(newnode == 0)
	{
		tmpstr = oitoa(node->afterevent);
		setchoiceboxselection(after, tmpstr);
		free(tmpstr); tmpstr = NULL;
	}
		
	changemask(pincode, "0000");
	if(newnode == 0)
		changeinput(pincode, node->pincode);
	else
		changeinput(pincode, "0000");

	drawscreen(rectimerext, 0, 0);
	addscreenrc(rectimerext, listbox);

	tmp = listbox->select;
	while(1)
	{
		addscreenrc(rectimerext, tmp);
		rcret = waitrc(rectimerext, 0, 2);
		tmp = listbox->select;

		if(listbox->select != NULL && listbox->select->ret != NULL && ostrcmp(listbox->select->name, "justplay") == 0)
		{
			recjustplayhidden(atoi(listbox->select->ret), end, path, after);
		}

		tmprepeate = recrepeateget(newnode, repeate, repeatetype, day, mon, tue, wed, thur, fri, sat, sun);
		if(listbox->select != NULL && (ostrcmp(listbox->select->name, "repeate") == 0 || ostrcmp(listbox->select->name, "repeatetype") == 0))
		{
			recrepeatehidden(tmprepeate, repeatetype, day, mon, tue, wed, thur, fri, sat, sun);
		}
		drawscreen(rectimerext, 0, 0);

		if(rcret == getrcconfigint("rcexit", NULL))
		{
			if(newnode == 1 || flag == 1) delrectimer(node, 1, 0);
			break;
		}
		if(rcret == getrcconfigint("rcok", NULL))
		{
			if(listbox->select != NULL && ostrcmp(listbox->select->name, "path") == 0)
			{
				clearscreen(rectimerext);
				tmpstr = screendir(listbox->select->ret, "", NULL, NULL, NULL, NULL, 0, "SELECT", 0, NULL, 0, NULL, 0, 700, 0, 650, 0, 0);
				if(tmpstr != NULL)
					changeinput(listbox->select, tmpstr);
				free(tmpstr); tmpstr = NULL;

				drawscreen(rectimerext, 0, 0);
				continue;
			}
			if(listbox->select != NULL && ostrcmp(listbox->select->name, "channel") == 0)
			{
				clearscreen(rectimerext);
				int saveservicetype = status.servicetype;
				struct channel* tmpchnode = (struct channel*)recchannel->handle;

				status.servicetype = tmpservicetype;
				if(tmpservicetype == 0)
				{
					addconfigtmp("channellist", tmpchannellist);
					if(tmpchnode != NULL)
					{
						addconfiginttmp("serviceid", tmpchnode->serviceid);
						addconfiglutmp("transponderid", tmpchnode->transponderid);
					}
				}
     				else
				{
					addconfigtmp("rchannellist", tmpchannellist);
					if(tmpchnode != NULL)
					{
						addconfiginttmp("rserviceid", tmpchnode->serviceid);
						addconfiglutmp("rtransponderid", tmpchnode->transponderid);
					}
				}

				screenchannellist(&tmpchnode, &tmpchannellist, 1);

				recchannel->handle = (char*)tmpchnode;

				status.servicetype = saveservicetype;
				delconfigtmp("channellist");
				delconfigtmp("rchannellist");
				delconfigtmp("serviceid");
				delconfigtmp("rserviceid");
				delconfigtmp("transponderid");
				delconfigtmp("rtransponderid");

				if((struct channel*)recchannel->handle != NULL)
				{
					changeinput(listbox->select, ((struct channel*)recchannel->handle)->name);
				}

				drawscreen(rectimerext, 0, 0);
				continue;
			}

			free(node->name); node->name = NULL;
			node->name = ostrcat(name->ret, NULL, 0, 0);
			if(justplay->ret != NULL)
				node->justplay = atoi(justplay->ret);
			if(begin->ret != NULL)
			{
				time_t tmpbegin = 0;

				loctime->tm_isdst = bdaylight;
				tmpstr = strptime(begin->ret, "%H:%M %d-%m-%Y", loctime); 
				if(tmpstr != NULL)
					tmpbegin = mktime(loctime);

				if((flag == 1 || newnode == 1) && node->justplay == 0)
					tmpbegin -= getconfigint("recforerun", NULL) * 60;

				tmpbegin -= (tmpbegin % 60);
				changerectimerbegin(node, tmpbegin, 0);

				if(node->justplay == 1)
					node->end = node->begin + 1;
			}
			if(end->ret != NULL && node->justplay == 0)
			{
				loctime->tm_isdst = edaylight;
				tmpstr = strptime(end->ret, "%H:%M %d-%m-%Y", loctime); 
				if(tmpstr != NULL)
					node->end = mktime(loctime);

				if((flag == 1 || newnode == 1) && node->justplay == 0)
					node->end += getconfigint("recoverrun", NULL) * 60;
				node->end -= (node->end % 60);
			}
			if(after->ret != NULL)
				node->afterevent = atoi(after->ret);
				
			if(pincode->ret != NULL)
				node->pincode = ostrcat(pincode->ret, NULL, 0, 0);
			else
				node->pincode = ostrcat("0000", NULL, 0, 0);

			if(tmprepeate > 127) tmprepeate = 0;
			node->repeate = tmprepeate;

			free(node->recpath); node->recpath = NULL;
			node->recpath = ostrcat(path->ret, NULL, 0, 0);

			free(node->channellist); node->channellist = NULL;
			node->channellist = tmpchannellist;

			node->servicetype = tmpservicetype;

			if(recchannel->handle != NULL)
			{
				node->serviceid = ((struct channel*)recchannel->handle)->serviceid;
				node->transponderid = ((struct channel*)recchannel->handle)->transponderid;
			}

			char* checkret = NULL;
			ret = checkrectimeradd(node, &checkret);
			if(ret != 0)
			{
				if(ret == 2)
				{
					ret = textbox(_("Message"), checkret, _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 0, 0);
					if(ret == 2)
						ret = 99;
					else
						ret = 0;
				}
				if(ret == 1 || ret == 99)
				{
					node->status = 3;
					free(node->errstr); node->errstr = NULL;
					node->errstr = ostrcat(checkret, NULL, 0, 0);
					stringreplacecharonce(node->errstr, '\n', '\0');
					if(ret == 1) textbox(_("Message"), checkret, _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 0, 0);
				}
			}
				
			if(newnode == 1 || flag == 1) node->disabled = 0;
			status.writerectimer = 1;
			writerectimer(getconfig("rectimerfile", NULL), 0);
			break;
		}
	}

	if(flag == 1) resettvpic();

	delownerrc(rectimerext);
	clearscreen(rectimerext);
}

//flag 0: show normal
//flag 1: show log
void screenrectimer()
{
	int rcret = 0, flag = 0;
	struct skin* recordtimer = getscreen("rectimer");
	struct skin* listbox = getscreennode(recordtimer, "listbox");
	struct skin* b1 = getscreennode(recordtimer, "b1");
	struct skin* b2 = getscreennode(recordtimer, "b2");
	struct skin* b3 = getscreennode(recordtimer, "b3");
	struct skin* b4 = getscreennode(recordtimer, "b4");
	struct skin* tmp = NULL;
	struct rectimer* rectimernode = NULL;
	struct tm *loctime = NULL;
	struct channel* chnode = NULL;
	char* buf = NULL, *tmpstr = NULL;

	buf = malloc(MINMALLOC);
	if(buf == NULL)
	{
		err("no memory");
		return;
	}

start:
	deloldrectimer();

	tmp = NULL;
	listbox->aktpage = -1;
	listbox->select = NULL;
	rectimernode = rectimer;

	while(rectimernode != NULL)
	{
		if((flag == 0 && (rectimernode->status == 2 || rectimernode->status == 3)) || (flag == 1 && (rectimernode->status == 0 || rectimernode->status == 1)))
		{
			rectimernode = rectimernode->next;
			continue;
		}

		tmp = addlistbox(recordtimer, listbox, tmp, 1);
		if(tmp != NULL)
		{
			loctime = localtime(&rectimernode->begin);
			strftime(buf, MINMALLOC, "%d-%m-%Y %H:%M", loctime);
			tmpstr = ostrcat(tmpstr, buf, 1, 0);
			if(rectimernode->justplay == 0)
			{
				tmpstr = ostrcat(tmpstr, " - ", 1, 0);
				loctime = localtime(&rectimernode->end);
				strftime(buf, MINMALLOC, "%d-%m-%Y %H:%M", loctime);
				tmpstr = ostrcat(tmpstr, buf, 1, 0);
				tmpstr = ostrcat(tmpstr, " (", 1, 0);
				tmpstr = ostrcat(tmpstr, _("rec"), 1, 0);
				tmpstr = ostrcat(tmpstr, " - ", 1, 0);
				if(rectimernode->repeate == 0)
					tmpstr = ostrcat(tmpstr, _("once"), 1, 0);
				else
					tmpstr = ostrcat(tmpstr, _("repeate"), 1, 0);
				tmpstr = ostrcat(tmpstr, ")", 1, 0);
			}
			else
			{
				tmpstr = ostrcat(tmpstr, " (", 1, 0);
				tmpstr = ostrcat(tmpstr, _("switch"), 1, 0);
				tmpstr = ostrcat(tmpstr, " - ", 1, 0);
				if(rectimernode->repeate == 0)
					tmpstr = ostrcat(tmpstr, _("once"), 1, 0);
				else
					tmpstr = ostrcat(tmpstr, _("repeate"), 1, 0);
				tmpstr = ostrcat(tmpstr, ")", 1, 0);
			}

			if(rectimernode->name != NULL)
			{
				tmpstr = ostrcat(tmpstr, "\n", 1, 0);
				if(rectimernode->name == NULL || strlen(rectimernode->name) == 0)
					tmpstr = ostrcat(tmpstr, "---", 1, 0);
				else
					tmpstr = ostrcat(tmpstr, rectimernode->name, 1, 0);
			}
			chnode = getchannel(rectimernode->serviceid, rectimernode->transponderid);
			if(chnode != NULL && chnode->name != NULL && strlen(chnode->name) > 0)
			{
				tmpstr = ostrcat(tmpstr, " (", 1, 0);
				tmpstr = ostrcat(tmpstr, chnode->name, 1, 0);
				tmpstr = ostrcat(tmpstr, ")", 1, 0);
			}

			if(rectimernode->status == 0)
			{
				tmpstr = ostrcat(tmpstr, "\n", 1, 0);
				tmpstr = ostrcat(tmpstr, _("waiting"), 1, 0);
			}
			else if(rectimernode->status == 1)
			{
				tmpstr = ostrcat(tmpstr, "\n", 1, 0);
				tmpstr = ostrcat(tmpstr, _("running"), 1, 0);
			}
			else if(rectimernode->status == 2)
			{
				tmpstr = ostrcat(tmpstr, "\n", 1, 0);
				tmpstr = ostrcat(tmpstr, _("success"), 1, 0);
			}
			else if(rectimernode->status == 3)
			{
				tmpstr = ostrcat(tmpstr, "\n", 1, 0);
				tmpstr = ostrcat(tmpstr, _("error"), 1, 0);
			}

			if(flag == 1 && rectimernode->errstr != NULL && strlen(rectimernode->errstr) != 0)
			{
				tmpstr = ostrcat(tmpstr, " (", 1, 0);
				tmpstr = ostrcat(tmpstr, rectimernode->errstr, 1, 0);
				tmpstr = ostrcat(tmpstr, ")", 1, 0);
			}
			
			changetext(tmp, tmpstr);
			free(tmpstr); tmpstr = NULL;
			tmp->handle = (char*)rectimernode;
			tmp->height = 110;
			tmp->type = TEXTBOX;

			if(rectimernode->disabled == 1)
				tmp->fontcol = convertcol("deaktivcol");
		}
		rectimernode = rectimernode->next;
	}

	if(flag == 0)
	{
		b1->hidden = NO;
		b2->hidden = NO;
		b3->hidden = NO;
		b4->hidden = NO;
	}
	else
	{
		b1->hidden = YES;
		b2->hidden = YES;
		b3->hidden = YES;
		b4->hidden = YES;
	}

	drawscreen(recordtimer, 0, 0);
	addscreenrc(recordtimer, listbox);

	while(1)
	{
		rcret = waitrc(recordtimer, 5000, 0);

		if(flag == 0 && rcret == getrcconfigint("rcblue", NULL))
		{
			flag = 1;
			delmarkedscreennodes(recordtimer, 1);
			goto start;
		}
		if(rcret == getrcconfigint("rcexit", NULL))
		{
			if(flag == 0)
				break;
			else
			{
				flag = 0;
				delmarkedscreennodes(recordtimer, 1);
				goto start;
			}
		}
		if(flag == 0 && rcret == getrcconfigint("rcgreen", NULL))
		{
			clearscreen(recordtimer);
			screenrectimerext(NULL, 0);
			delmarkedscreennodes(recordtimer, 1);
			goto start;
		}
		if(flag == 0 && listbox->select != NULL)
		{
			if(rcret == getrcconfigint("rcok", NULL))
			{
				clearscreen(recordtimer);
				screenrectimerext((struct rectimer*)listbox->select->handle, 0);
				delmarkedscreennodes(recordtimer, 1);
				goto start;
			}
			if(rcret == getrcconfigint("rcred", NULL))
			{
				delrectimer((struct rectimer*)listbox->select->handle, 1, 0);
				
				delmarkedscreennodes(recordtimer, 1);
				goto start;
			}
			if(rcret == getrcconfigint("rcyellow", NULL))
			{
				if(((struct rectimer*)listbox->select->handle)->disabled == 1)
					((struct rectimer*)listbox->select->handle)->disabled = 0;
				else
					((struct rectimer*)listbox->select->handle)->disabled = 1;

				delmarkedscreennodes(recordtimer, 1);
				goto start;
			}
			if(rcret == RCTIMEOUT)
			{
				delmarkedscreennodes(recordtimer, 1);
				goto start;
			}
		}
	}

	free(buf); buf = NULL;
	delmarkedscreennodes(recordtimer, 1);
	delownerrc(recordtimer);
	clearscreen(recordtimer);
}

int addrecepg(struct channel* chnode, struct epg* epgnode, char* channellist)
{
	struct rectimer* node = NULL;

	deloldrectimer();

	if(chnode == NULL || epgnode == NULL)
		return 1;

	node = addrectimernode(NULL, NULL, 0);
	if(node != NULL)
	{
		node->name = ostrcat(epgnode->title, NULL, 0, 0);
		changerectimerbegin(node, epgnode->starttime, 0);
		node->end = epgnode->endtime;

		node->repeate = 0;

		node->recpath = ostrcat(getconfig("rec_path", NULL), NULL, 0, 0);
		node->servicetype = status.servicetype;
		if(node->servicetype == 0)
		{
			if(channellist == NULL)
				node->channellist = ostrcat(getconfig("channellist", NULL), NULL, 0, 0);
			else
				node->channellist = ostrcat(channellist, NULL, 0, 0);
		}
		else
		{
			if(channellist == NULL)
				node->channellist = ostrcat(getconfig("rchannellist", NULL), NULL, 0, 0);
			else
				node->channellist = ostrcat(channellist, NULL, 0, 0);
		}
		node->serviceid = chnode->serviceid;
		node->transponderid = chnode->transponderid;
		screenrectimerext(node, 1);
	}

	return 0;
}

#endif
