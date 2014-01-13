#ifndef MARKER_H
#define MARKER_H

struct marker* addmarkernode(off64_t pos)
{
	
	struct marker *newnode = NULL;
	struct marker *oldnode = NULL;
	struct marker *prev = NULL;
	
	newnode = (struct marker*)calloc(1, sizeof(struct marker));
	if(newnode == NULL)
	{
		err("no memory");
		return NULL;
	}
	newnode->next = NULL;
	newnode->pos = pos;
	
	if(status.playmarker == NULL)
	{
		newnode->prev = NULL;
		status.playmarker = newnode;
		return newnode;
	}
	
	oldnode = status.playmarker;
	if(pos == -1)
	{
		while(oldnode->next != NULL)
		{
			oldnode = oldnode->next;
		}
		oldnode->next = newnode;
		newnode->prev = oldnode;
	}
	else
	{
		prev = oldnode;
		while(oldnode->next != NULL && oldnode->pos < pos)
		{
			prev = oldnode;
			oldnode = oldnode->next;
		}
		
		if(oldnode->prev == NULL)
		{
			if(oldnode->pos > pos)
			{
				newnode->next = oldnode;
				newnode->prev = NULL;
				oldnode->prev = newnode;
				status.playmarker = newnode;
				return newnode;
			}
		}
		if(oldnode->next == NULL)
		{
			if(oldnode->pos < pos)
			{
				newnode->next = NULL;
				newnode->prev = oldnode;
				oldnode->next = newnode;
				return newnode;
			}
		}
		newnode->next = prev->next;
		newnode->prev = prev;
		if(prev->next != NULL)
				prev->next->prev = newnode;
		prev->next = newnode;
	}
	
	return newnode;
}


int delmarkernode(off64_t pos)
{
	if(status.playmarker == NULL)
		return -1;
	
	struct marker* delnode;
	struct marker* node = status.playmarker;
	
	delnode = node;
	while(delnode != NULL)
	{
		node = delnode->next;
		if(pos == -1)
		{
			free(delnode->timetext);
			free(delnode);
			status.playmarker = NULL;
		}
		else if(delnode->pos == pos)
		{
			if(delnode->next != NULL)
				delnode->next->prev = delnode->prev;
			if(delnode->prev == NULL)
				status.playmarker = delnode->next;
			else
				delnode->prev->next = delnode->next;
			free(delnode->timetext);
			free(delnode);
			break;
		}
		delnode = node;
	}
	return 0;
}

int delmarker(char* timetext)
{
	struct marker* marker = status.playmarker;

	while(marker != NULL)
	{
		if(ostrcmp(marker->timetext, timetext) == 0)
			return delmarkernode(marker->pos);
		marker = marker->next;
	}
	return -1;
}

int getmarker(char* dateiname)
{
	off64_t pos;
	off64_t time;
	struct marker *node = NULL;
	
	FILE* datei = fopen(dateiname, "r");
	if(datei!= NULL)
	{
		while(!feof(datei))
		{
			fscanf(datei, "%lld,%lld", &pos, &time);
			node = addmarkernode(pos);
			if(node == NULL)
				return -1;
			node->time = time;
			node->pos = pos;
			node->timetext = convert_timesec(time);
		}
		fclose(datei);
	}
	else 
		return -1;
		
	return 0;
}

int putmarker(char* dateiname)
{
	struct marker *node = NULL;
	
	if(status.playmarker == NULL)
		return -1;	
	else
		node = status.playmarker;
	
	FILE* datei = fopen(dateiname, "w");
	if(datei != NULL)
	{
		while(node->next != NULL)
		{
			fprintf(datei, "%lld,%lld\n", node->pos, node->time);
			node = node->next;
		}
		fprintf(datei, "%lld,%lld", node->pos, node->time);	
		fclose(datei);
	}
	else 
		return -1;

	return 0;
}

int setmarker()
{
	unsigned long long atime = 0, len = 0, startpos = 0, ret = 0, aktpts = 0;
	struct marker *node = NULL;
	
	struct service* snode = getservice(RECORDPLAY, 0);
	if(snode == NULL) return -1;

	off64_t pos = lseek64(snode->recsrcfd, 0, SEEK_CUR);
	playergetinfots(&len, &startpos, NULL, &atime, NULL, 0);
	ret = videogetpts(status.aktservice->videodev, &aktpts);
	if(ret == 0)
	{
		atime = (aktpts - startpos) / 90000;
		printf("marker-> new position \n"); 
		off64_t posn = playergetptspos(aktpts, pos, -1, 4, 0, 0, NULL);
		if(posn < -1)
			posn = playergetptspos(aktpts, pos - (posn *-1), -1, 4, 0, 0, NULL);
		if(posn < -1)
			posn = playergetptspos(aktpts, pos - (posn *-2), -1, 4, 0, 0, NULL);
		if(posn < -1)
			posn = playergetptspos(aktpts, pos - (posn *-3), -1, 4, 0, 0, NULL);
		if(posn < -1)
			posn = playergetptspos(aktpts, pos - (posn *-4), -1, 4, 0, 0, NULL);
		if(posn > 0)
			{
				printf("marker-> new position found. OLD: %lld NEW: %lld\n", pos, posn); 
				pos = posn;
			}
	}
	else
		atime = (atime - startpos) / 90000;
	if(atime == 0)
	{
		// syntax ??
		//int ret = textbox(_("Message"), _("ERROR... can't set marker in this file"), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, NULL, 0, 600, 200, 4, 0);
		textbox(_("Message"), _("ERROR... can't set marker in this file"), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, NULL, 0, 600, 200, 4, 0);
		return -1;
	}
	node = addmarkernode(pos);
	if(node != NULL)
	{
		node->pos = pos;
		node->time = atime;
		node->timetext = convert_timesec(atime);
		//char* filemarker = changefilenameext(snode->recname, ".ma");
		//putmarker(filemarker);
		//free(filemarker); filemarker=NULL;
		// syntax ??
		//int ret = textbox(_("Message"), _("Marker has been set."), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, NULL, 0, 600, 200, 2, 0);
		textbox(_("Message"), _("Marker has been set."), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, NULL, 0, 600, 200, 2, 0);
	}
	else
		return -1;
	
	return 0;
}

int jumpmarker(char* timetext)
{
	struct marker* marker = status.playmarker;

	while(marker != NULL)
	{
		if(ostrcmp(marker->timetext, timetext) == 0)
		{

			struct service* snode = getservice(RECORDPLAY, 0);
			if(snode == NULL) return -1;

			m_lock(&status.tsseekmutex, 15);
			lseek64(snode->recsrcfd, marker->pos, SEEK_SET);
			playerresetts();
			//videoclearbuffer(status.aktservice->videodev);
			//audioclearbuffer(status.aktservice->audiodev);
			////videodiscontinuityskip(status.aktservice->videodev, 0);
			//audiostop(snode->audiodev);
			//videostop(snode->videodev, 0);
			//videoplay(snode->videodev);
			//audioplay(snode->audiodev);
			m_unlock(&status.tsseekmutex, 15);

			return 0;
		}
		marker = marker->next;
	}
	return -1;
}
	
void screenmarker(char* file, char* showname, int* playinfobarstatus, int* playinfobarcount, int playertype, int flag)
{
	int rcret;
	//char* filemarker = NULL;
	//struct service* snode = getservice(RECORDPLAY, 0);
	
	struct skin* screen1 = getscreen("marker");
	struct skin* listbox = getscreennode(screen1, "listbox");
	struct skin* dummy = getscreennode(screen1, "dummy");
	struct skin* tmp = NULL;
	
	struct marker *marker = NULL;
	
	marker = status.playmarker;
	while(marker != NULL)
	{
		tmp = addlistbox(screen1, listbox, tmp, 1);
		if(tmp != NULL)
		{
			tmp->textposx = dummy->textposx;
			tmp->prozposx = dummy->prozposx;
			tmp->prozposy = dummy->prozposy;
			tmp->height = dummy->height;
			tmp->valign = dummy->valign;
			tmp->hspace = dummy->hspace;
			
			changename(tmp, marker->timetext);
			changetext(tmp, marker->timetext);
		}
		marker = marker->next;
	}

	drawscreen(screen1, 0, 0);
	addscreenrc(screen1, listbox);
	while (1)
	{
		rcret = waitrc(screen1, 0, 0);
		if(rcret==getrcconfigint("rcexit",NULL)) break;
		if(listbox->select != NULL && rcret==getrcconfigint("rcok",NULL))
		{
			if(jumpmarker(listbox->select->text) == 0)
				break;
		}
		if(listbox->select != NULL && rcret==getrcconfigint("rcred",NULL))
		{
			if(delmarker(listbox->select->text) == 0)
			{
				//filemarker = changefilenameext(snode->recname, ".ma");
				//putmarker(filemarker);
				//free(filemarker); filemarker=NULL;
				listbox->select->hidden = YES;
				clearscreen(screen1);
				drawscreen(screen1, 0, 0);
			}
		}
	}
	delownerrc(screen1);
	delmarkedscreennodes(screen1, 1);
	clearscreen(screen1);
	blitfb(0);
	if(rcret==getrcconfigint("rcok",NULL))
	{
		*playinfobarstatus = 1;
		*playinfobarcount = 0;
		screenplayinfobar(file, showname, 0, playertype, flag);
	}
}

off64_t getcurrentpos(struct service* snode)
{
	unsigned long long aktpts = 0;
	int ret = 0;
	off64_t posn = -1;
		
	off64_t pos = lseek64(snode->recsrcfd, 0, SEEK_CUR);

	ret = videogetpts(status.aktservice->videodev, &aktpts);
	if(ret == 0)
	{
		posn = playergetptspos(aktpts, pos, -1, 4, 0, 0, NULL);
		if(posn < -1)
			posn = playergetptspos(aktpts, pos - (posn *-1), -1, 4, 0, 0, NULL);
		if(posn < -1)
			posn = playergetptspos(aktpts, pos - (posn *-2), -1, 4, 0, 0, NULL);
		if(posn < -1)
			posn = playergetptspos(aktpts, pos - (posn *-3), -1, 4, 0, 0, NULL);
		if(posn < -1)
			posn = playergetptspos(aktpts, pos - (posn *-4), -1, 4, 0, 0, NULL);
	}
	printf("return current pos: %lld \n", posn);
	return posn;
}
		
		

void markerautoseek_thread()
{
	struct seeker
	{
		off64_t spos;
		off64_t stime;
		off64_t epos;
		off64_t etime;
		struct seeker* prev;
		struct seeker* next;
	};
	
	struct seeker *seeker = NULL;
	struct seeker *seekerarb = NULL;
	int ret = 0, time = 0;
	int first = 1;
	unsigned long long pts = 0;
	unsigned long long hpos = 0, len = 0;
	unsigned long long startpos = 0;
	off64_t pos = 0;
	
	struct service* snode = getservice(RECORDPLAY, 0);
	if(snode == NULL) return;
	
	if(status.playmarker == NULL)
		return;	
	struct marker *marker = status.playmarker;
	
	
	seeker = (struct seeker*)calloc(1, sizeof(struct seeker));
	seeker->spos  = 0;
	seeker->stime = 0;
	seeker->epos  = marker->pos;
	seeker->etime = marker->time;
	seeker->next  = NULL;
	seekerarb = seeker;

	while(marker->next != NULL)
	{
		seekerarb->next = (struct seeker*)calloc(1, sizeof(struct seeker));
		seekerarb = seekerarb->next;
		marker = marker->next;
		seekerarb->spos  = marker->pos;
		seekerarb->stime = marker->time;
		if(marker->next == NULL)
		{
			seekerarb->epos  = 0;
			seekerarb->etime = 9999999;
			seekerarb->next  = NULL;
		}
		else {
			marker = marker->next;
			seekerarb->epos  = marker->pos;
			seekerarb->etime = marker->time;
			seekerarb->next  = NULL;
		}
	}
	
	ret = playergetinfots(&len, &startpos, NULL, &hpos, NULL, 0);
	
	while ((status.playspeed != 0 || status.play != 0 || status.pause != 0) && status.autoseek == 1)
	{
		ret = videogetpts(status.aktservice->videodev, &pts);
		if(status.playspeed == 0) {
			if(ret == 0) {
				time = (pts - startpos) / 90000;
				seekerarb = seeker;
				while(1)
				{
					if(time >= seekerarb->stime && time < seekerarb->etime) {
						m_lock(&status.tsseekmutex, 15);
						if(seekerarb->epos == 0)
							lseek64(snode->recsrcfd, 188+1024 , SEEK_END);
						else 
							lseek64(snode->recsrcfd, seekerarb->epos, SEEK_SET);
						playerresetts();
						m_unlock(&status.tsseekmutex, 15);
					}
					if(seekerarb->next != NULL)
						seekerarb = seekerarb->next;
					else
						break;
				}
			}
		}
		sleep(1);
	}
	
	while(1)
	{
		seekerarb = seeker->next;
		free(seeker); seeker = NULL;
		if(seekerarb == NULL)
			break;
		seeker = seekerarb;
	}
	status.autoseek = 0;
}

	


#endif
