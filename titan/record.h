#ifndef RECORD_H
#define RECORD_H

int recordcheckcrypt(struct dvbdev* fenode, int servicetype)
{
	struct service* servicenode = service;

	while(servicenode != NULL)
	{
		if(fenode == servicenode->fedev && servicenode->channel != NULL && servicenode->channel->crypt > 0)
		{
			if(servicenode->type == RECORDDIRECT || servicenode->type == RECORDTIMER) return 1;
			if(servicetype == RECORDSTREAM && servicenode->type == RECORDTIMESHIFT) return 1;
			if(servicetype == CHANNEL && servicenode->type == RECORDTIMESHIFT) return 1;
		}
		servicenode = servicenode->next;
	}
	return 0;
}

//flag bit 0 = with timeout
//flag bit 1 = show textbox
//flag bit 2 = print debug
//flag bit 3 = return text
char* recordcheckret(struct stimerthread* timernode, int ret, int flag)
{
	char* tmpstr = NULL;
	int timeout = 0;
	if(checkbit(flag, 0) == 1)
		timeout = 10;

	if(ret != 0)
	{
		switch(ret)
		{
			case 1:
				tmpstr = ostrcat(_("HDD not configured\ncan't find record path"), NULL, 0, 0);
				break;
			case 2:
				tmpstr = ostrcat(_("Not enought space"), NULL, 0, 0);
				break;
			case 3:
				tmpstr = ostrcat(_("Error create filename"), NULL, 0, 0);
				break;
			case 4:
				tmpstr = ostrcat(_("Can't open file"), NULL, 0, 0);
				break;
			case 5:
				tmpstr = ostrcat(_("Can't open FRONTEND device"), NULL, 0, 0);
				break;
			case 6:
				tmpstr = ostrcat(_("Can't open DMX device"), NULL, 0, 0);
				break;
			case 7:
				tmpstr = ostrcat(_("Pid's not ok"), NULL, 0, 0);
				break;
			case 8:
				tmpstr = ostrcat(_("Channel or Transponder is empty"), NULL, 0, 0);
				break;
			case 9:
				tmpstr = ostrcat(_("Write error"), NULL, 0, 0);
				break;
			case 10:
				tmpstr = ostrcat(_("No memory"), NULL, 0, 0);
				break;
			case 11:
				tmpstr = ostrcat(_("Failed open split file"), NULL, 0, 0);
				break;
			case 12:
				tmpstr = ostrcat(_("Frontend type unknown"), NULL, 0, 0);
				break;
			case 13:
				tmpstr = ostrcat(_("Tune to channel failed"), NULL, 0, 0);
				break;
			case 14:
				break;
			case 15:
				tmpstr = ostrcat(_("To many read error or end of file"), NULL, 0, 0);
				break;
			case 16:
				tmpstr = ostrcat(_("Can't create service"), NULL, 0, 0);
				break;
			case 17:
				tmpstr = ostrcat(_("No space left on device"), NULL, 0, 0);
				break;
		}
		if(tmpstr != NULL)
		{
			if(checkbit(flag, 2) == 1) err("%s", tmpstr);
			if(checkbit(flag, 1) == 1) textbox(_("Record / Timeshift / Stream"), tmpstr, _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, NULL, 0, 800, 200, timeout, 0);
		}
	}
	if(checkbit(flag, 3) == 0)
	{
		free(tmpstr);
		tmpstr = NULL;
	}
	return tmpstr;
}

void recordwriteepg(char* filename, struct channel* chnode, struct rectimer* rectimernode)
{
	struct epg* epgnode = NULL;
	FILE *fd = NULL;
	int ret = 0, count = 0;
	char* tmpstr = NULL, *epgfilename = NULL, *buf = NULL;
	struct tm *loctime = NULL;

	if(filename == NULL)
	{
		err("epg record filename = NULL");
		return;
	}

	epgfilename = changefilenameext(filename, ".epg");
	
	//check if epg is in rectimer time
	if(rectimernode != NULL)
	{
		epgnode = getepgakt(chnode);
		while(epgnode != NULL && (epgnode->starttime < rectimernode->begin || epgnode->starttime >= rectimernode->end))
			epgnode = epgnode->next;
	}
	else
		epgnode = getepgbytime(chnode, time(NULL) + 60);
	
	if(epgnode != NULL)
	{
		fd = fopen(epgfilename, "w");
		if(fd == NULL)
		{
			perr("open %s", epgfilename);
			free(epgfilename);
			return;
		}
		chmod(epgfilename, 0666);

		if(epgnode->title != NULL)
		{
			buf = malloc(MINMALLOC);
			loctime = localtime(&epgnode->starttime);
			strftime(buf, MINMALLOC, "%d-%m-%Y %H:%M", loctime);
			ret += fwrite(buf, strlen(buf), 1, fd);
			ret += fwrite(" - ", 3, 1, fd);
			ret += fwrite(epgnode->title, strlen(epgnode->title), 1, fd);
			ret += fwrite("\n", 1, 1, fd);
			count += 4;
			free(buf); buf = NULL;
		}
		if(epgnode->subtitle != NULL)
		{
			ret += fwrite(epgnode->subtitle, strlen(epgnode->subtitle), 1, fd);
			ret += fwrite("\n\n", 2, 1, fd);
			count += 2;
		}
		if(epgnode->desc != NULL)
		{
			tmpstr = epgdescunzip(epgnode);
			if(tmpstr != NULL)
			{
				ret += fwrite(tmpstr, strlen(tmpstr), 1, fd);
				count++;
				free(tmpstr); tmpstr = NULL;
			}
		}

		if(ret != count)
		{
			err("writting record epg file ret=%d, count=%d", ret, count);
		}

		fclose(fd);
	}
	free(epgfilename);
}

void createrecthumblastthread(struct stimerthread* self, char* dname, char* filename)
{
	debug(777, "createrecthumblast thread (record thumb) start");

//	if(status.mediadbthread != NULL || self == NULL) return;
	if(status.mediadbthread == NULL || self != NULL)
    {

	    debug(777, "createrecthumblast thread (record thumb) start ok");

	    status.mediadbthreadstatus = 1;
	    status.mediadbthread = self;
	    status.mediadbsavetime = 1;

	    if(dname != NULL && filename != NULL)
	    {
		    readmediadb(getconfig("mediadbfile", NULL), 0, 0);

		    debug(133, "path: %s",dname);
		    debug(133, "file: %s",filename);
		    debug(133, "type: 2");
	    
		    addconfigtmp("mediadbscantimeout", "0");
		    mediadbfindfilecb(dname, filename, 0, NULL, 0);
		    delconfigtmp("mediadbscantimeout");
	    }

	    free(dname); dname = NULL;
	    free(filename); filename = NULL;
	    status.mediadbsavetime = 0;
	    status.mediadbthread = NULL;
	    status.mediadbthreadstatus = 0;
    }
	if(status.mcaktiv == 2) status.mcaktiv = 1;

	debug(777, "createrecthumblast thread (record thumb) end");
}

void createrecthumbfirstthread(struct stimerthread* self, char* dname, char* filename)
{
	debug(777, "createrecthumbfirst thread (record thumb) start");
	
	int count = 0;

	if(filename != NULL) count = 599;

	while(count < 600)
	{
		sleep(1);
		count++;
	}

	if(status.recording > 0 || (status.mcaktiv == 2 && filename != NULL && dname != NULL && (status.play == 1 || status.pause == 1)))
	{
		char* cmd = NULL;
        if(checkchipset("3798MV200") == 1)
            cmd = ostrcat("grab -v -j 100 -r 960 > /tmp/screenshot_backdrop1.jpg", NULL, 0, 0);
        else
            cmd = ostrcat("grab -v -j 100 -r 1280:720 /tmp/screenshot_backdrop1.jpg", NULL, 0, 0);
	
		if(cmd != NULL)
			system(cmd);

		debug(777, "cmd: %s", cmd);

		free(cmd); cmd = NULL;
	}

	count = 0;
	if(filename != NULL) count = 59;

	while(count < 60)
	{
		sleep(1);
		count++;
	}

	if(status.recording > 0 || (status.mcaktiv == 2 && filename != NULL && dname != NULL && (status.play == 1 || status.pause == 1)))
	{
		char* cmd = NULL;
        if(checkchipset("3798MV200") == 1)
            cmd = ostrcat("grab -v -j 100 -r 960 > /tmp/screenshot_cover.jpg", NULL, 0, 0);
        else
            cmd = ostrcat("grab -v -j 100 -r 500:400 /tmp/screenshot_cover.jpg", NULL, 0, 0);
	
		if(cmd != NULL)
			system(cmd);

		debug(777, "cmd: %s", cmd);

		free(cmd); cmd = NULL;
	}

	count = 0;
	if(filename != NULL) count = 9;
	while(count < 10)
	{
		sleep(1);
		count++;
	}

	if(status.recording > 0 || (status.mcaktiv == 2 && filename != NULL && dname != NULL && (status.play == 1 || status.pause == 1)))
	{
		char* cmd = NULL;
	    if(checkchipset("3798MV200") == 1)
            cmd = ostrcat("grab -v -j 100 -r 960 > /tmp/screenshot_thumb.jpg", NULL, 0, 0);
        else
            cmd = ostrcat("grab -v -j 100 -r 160:120 /tmp/screenshot_thumb.jpg", NULL, 0, 0);
	
		if(cmd != NULL)
			system(cmd);
		debug(777, "cmd: %s", cmd);

		free(cmd); cmd = NULL;
	}

	if(status.mcaktiv == 2 && filename != NULL && dname != NULL && (status.play == 1 || status.pause == 1))
    {
    	struct stimerthread *recthumblastthread = NULL;
        recthumblastthread = addtimer(&createrecthumblastthread, START, 1000, 1, (void*)ostrcat(dname, NULL, 0, 0), (void*)ostrcat(filename, NULL, 0, 0), NULL);
    }

	debug(777, "createrecthumbfirst thread (record thumb) end");

}

void recordstop(struct service* node, int ret)
{
	struct stimerthread *recthumblastthread = NULL;
	struct rectimer* rectimernode = NULL;
	int afterevent = 1, type = -1;

	if(node != NULL)
	{
		type = node->type;

		if(node->fedev != NULL && node->type != RECORDTIMESHIFT)
			node->fedev->felock--;

		m_lock(&status.rectimermutex, 1);
		rectimernode = getrectimerbyservice(node);
		if(rectimernode != NULL && node->recendtime != 2) //2 = manuall rec stop
			afterevent = rectimernode->afterevent;
		m_unlock(&status.rectimermutex, 1);

		char* dname = NULL, *filename = NULL;

		if(type != RECORDSTREAM && type != RECORDTIMESHIFT && type != RECORDPLAY)
		{
			dname = ostrcat(node->recname, NULL, 0, 0);
			dname = dirname(dname);
			filename = ostrcat(basename(node->recname), NULL, 0, 0);
		}
			
#ifdef MIPSEL
		if(node->encoderdev != NULL)
		{
			if(node->videodev != NULL)
			{
				videostop(node->videodev, 1);
				videoclose(node->videodev, -1);
				dmxclose(node->dmxvideodev, -1);
			}
			if(node->audiodev != NULL)
			{
				audiostop(node->audiodev);
				audioclose(node->audiodev, -1);
				dmxclose(node->dmxaudiodev, -1);
			}
			encoderclose(node->encoderdev, -1);
		}
#endif
		delservice(node, 0);

		if(type == RECORDSTREAM)
			status.streaming--;
		else if(type == RECORDTIMESHIFT)
		{
			if(ret != 0) // on error stop timeshift
				timeshiftstop(2);
			status.timeshift = 0;
		}
		else if(type == RECORDPLAY)
			status.playing = 0;
		else
		{
			int i;
			for(i = 0; i < 9; i++)
			{
				if(status.recchnode[i] == node->channel)
				{
					status.recchnode[i] = NULL;
					break;
				}
			}			
			status.recording--;
		}
	
		deltranspondertunablestatus();

		if(dname != NULL && filename != NULL && getconfigint("recordpicture", NULL) == 1)
			recthumblastthread = addtimer(&createrecthumblastthread, START, 1000, 1, (void*)ostrcat(dname, NULL, 0, 0), (void*)ostrcat(filename, NULL, 0, 0), NULL);

		if(file_exist(getconfig("skriptafterrec", NULL)))
		{
			char* cmd = NULL;
			cmd = ostrcat(getconfig("skriptafterrec", NULL), " \"", 0, 0);
			cmd = ostrcat(cmd, dname, 1, 0);
			cmd = ostrcat(cmd, "/", 1, 0);
			cmd = ostrcat(cmd, filename, 1, 0);
			cmd = ostrcat(cmd, "\" &", 1, 0);
			debug(250, "start cmd: %s", cmd);
			system(cmd);
			debug(250, "done cmd: %s", cmd);		
			free(cmd), cmd = NULL;
		}

		free(dname), dname = NULL;
		free(filename), filename = NULL;

		//afterevent: 0 = auto
		//afterevent: 1 = nothing
		//afterevent: 2 = standby 
		//afterevent: 3 = poweroff
		printf("recordstop -> afterevent: %i, standby: %i, startmode: %i\n", afterevent, status.standby, status.startmode);
		if(afterevent == 0)
		{
			//not needed
			//if(status.startmode == 1) afterevent = 2;
			//else
			if(getwaswakuptimer() == 1) afterevent = 3;
		}
		if(afterevent == 2 && status.standby == 0)
		{
			status.standby = 2;
			screenstandby();
		}
		if(afterevent == 3)
		{
			if(status.recording < 1)
			{
				//wait for recthumblastthread end before shutdown
				int count = 0;
				while(gettimer(recthumblastthread) != NULL && count < 60)
				{
					sleep(1);
					count++;
				}
				int ret = 0;
				if(status.standby == 0)
					ret = textbox(_("Message"), _("Box recording finished poweroff in 30sec !"), _("EXIT"), getrcconfigint("rcexit", NULL), _("OK"), getrcconfigint("rcok", NULL), NULL, 0, NULL, 0, 900, 200, 30, 0);
				if(ret == 0 || ret == 2)
					oshutdown(1, 3);
			}
		}
	}
}

//flag 0: record split
//flag 1: play split
int recordsplit(struct service* servicenode, int flag)
{
	int ret = 0;
	char* filename = NULL;

	filename = malloc(256);
	if(filename == NULL)
	{
		err("no mem");
		ret = 10;
		servicenode->recendtime = 1;
	}
	else
	{
		if(flag == 0)
		{
			fdatasync(servicenode->recdstfd);
			close(servicenode->recdstfd);
		}
		else
			close(servicenode->recsrcfd);

		servicenode->rectotal = 0;
		servicenode->reclastsync = 0;

		if(servicenode->reccount < 2)
		{
			if(strlen(servicenode->recname) > 3)
				servicenode->recname[strlen(servicenode->recname) - 3] = '\0';
		}
		else
		{
			if(strlen(servicenode->recname) > 7)
				servicenode->recname[strlen(servicenode->recname) - 7] = '\0';
		}
		snprintf(filename, 255, "%s.%03d.ts", servicenode->recname, servicenode->reccount++);
		free(servicenode->recname);
		servicenode->recname = filename;

		if(flag == 0)
		{
			debug(250, "split record file - Recording to %s...", filename);
		}
		else
		{
			debug(250, "split play file - Playing %s...", filename);
		}

		if(flag == 0)
			servicenode->recdstfd = open(filename, O_WRONLY | O_CREAT | O_LARGEFILE, 0644);
		else
			servicenode->recsrcfd = open(filename, O_RDONLY | O_LARGEFILE | O_NONBLOCK);

		if(flag == 0)
		{
			if(servicenode->recdstfd < 0)
			{
				debug(250, "split record file - can't open recording file!");
				ret = 11;
				servicenode->recendtime = 1;
			}
			else
				posix_fadvise(servicenode->recdstfd, 0, 0, POSIX_FADV_RANDOM);
		}
		else
		{
			if(servicenode->recsrcfd < 0)
			{
				debug(250, "split play file - can't open play file!");
				ret = 15;
				servicenode->recendtime = 1;
			}
		}
	}
	return ret;
}

int readwritethread(struct stimerthread* stimer, struct service* servicenode, int flag)
{
	int readret = 0, writeret = 0, ret = 0, recbsize = 0, tmprecbsize = 0, i = 0, pktcount = 0, frbsize = 0, frmulti = 0, frmultiread = 0;
	int readtimeout = -1, writetimeout = -1;
	int recsync = 0, frcount = 0, count = 0;
	unsigned char* buf = NULL, *tmpbuf = NULL;
	char* retstr = NULL;

	debug(250, "start read-write thread");

	if(servicenode == NULL)
	{
		err("servicenode = NULL");
		return 1;
	}

	recsync = getconfigint("recsync", NULL);
	frbsize = servicenode->tssize * 3072; //aligned to 188 and 4096

	if(servicenode->type == RECORDDIRECT || servicenode->type == RECORDTIMER || servicenode->type == RECORDTIMESHIFT)
	{
		recbsize = servicenode->tssize * 1024; //aligned to 188 and 4096
		readtimeout = 7000000;
		writetimeout = 7000000; //5 sec
	}
		
	if(servicenode->type == RECORDPLAY || servicenode->type == RECORDSTREAM)
	{
		if(servicenode->recdstfd < 0)
		{
			err("destination fd not ok")
			return 1;
		}

		readtimeout = 7000000;
		writetimeout = 7000000;

		if(servicenode->type == RECORDPLAY)
		{
			recbsize = servicenode->tssize * 188;

			// recordplay work vuplus start
			if(getconfigint("recordplay_recbsize", NULL) != 0 && recbsize != getconfigint("recordplay_recbsize", NULL))
			{
				printf("recordplay1 recbsize: %d\n", recbsize);
				recbsize = getconfigint("recordplay_recbsize", NULL);
				printf("recordplay1 recbsize change to: %d\n", recbsize);
//				recbsize = 12032;
			}
			if(getconfigint("recordplay_writetimeout", NULL) != 0 && writetimeout != getconfigint("recordplay_writetimeout", NULL))
			{
				printf("recordplay1 writetimeout: %d\n", recbsize);
				writetimeout = getconfigint("recordplay_writetimeout", NULL);
				printf("recordplay1 writetimeout change to: %d\n", writetimeout);
//				writetimeout = 3000000;
			}
			// recordplay work vuplus end

			tmprecbsize = 188 * 188;
		}

		if(servicenode->type == RECORDSTREAM)
		{
			recbsize = servicenode->tssize * 1024; //aligned to 188 and 4096
			writetimeout = 60000000 * 30; // 30min if player is in pause
		}
	}
	
#ifdef SIMULATE
	int fd = open("simulate/record.ts", O_RDONLY | O_LARGEFILE);
	if(fd < 0)
	{
		perr("open simulate/record.ts");
		return 1;
	}
#endif

	buf = malloc(recbsize);
	if(buf == NULL)
	{
		err("no mem");
		return 1;
	}
	
	if(servicenode->type == RECORDPLAY && servicenode->tssize == 192)
	{
		tmpbuf = malloc(tmprecbsize);
		if(tmpbuf == NULL)
		{
			err("no mem");
			return 1;
		}
	}
	printf("record.h--> recdmx:%i\n", servicenode->recdmxstart);
	if(servicenode->recdmxstart == 0)
	{
		dmxstart(servicenode->dmxvideodev);
		servicenode->recdmxstart = 1;
	}
	while(1)
	{	
#ifdef SIMULATE
		servicenode->recsrcfd = fd;
		readret = dvbreadfd(servicenode->recsrcfd, buf, 0, recbsize, readtimeout, 0);
		usleep(1000);
#else
		if(servicenode->type == RECORDPLAY)
		{
			
			if(status.playspeed != 0)
			{
				if(status.videosize.w > 720)
				{
					frmulti = 16;
					frmultiread = 8;
				}
				else
				{
					frmulti = 8;
					frmultiread = 4;
				}
								
				if(status.playspeed == -4)
					frmulti = frmulti + (frmulti/2);
				else if(status.playspeed == -5)
					frmulti = frmultiread * 4;
				else if(status.playspeed == -6)
					frmulti = frmultiread * 8;
				else if(status.playspeed == 4)
					frmulti = frmulti + frmultiread ;
				else if(status.playspeed == 5)
					frmulti = frmultiread * 4;
				else if(status.playspeed == 6)
					frmulti = frmultiread * 8;				
			}			
			
			if(frcount == 0 && (status.playspeed < 0 || status.playspeed > 3))
			{				
				pthread_mutex_lock(&status.tsseekmutex);
				off64_t pos = 0;
				if(status.playspeed < 0)
					pos = lseek64(servicenode->recsrcfd, -(frbsize * frmulti), SEEK_CUR);
				else if(status.playspeed == 4 || status.playspeed == 5 || status.playspeed == 6)
					pos = lseek64(servicenode->recsrcfd, ((frbsize * frmulti) - ( frbsize * frmultiread )), SEEK_CUR);

				//begin of file
				if(pos <= 0)
				{
					//videoclearbuffer(status.aktservice->videodev);
					//audioclearbuffer(status.aktservice->audiodev);
					playerpausets();
					playercontinuets();
					playerresetts();

					status.playspeed = 0;
					status.pause = 0;
					status.play = 1;
				}
				else
				{
					if(status.playspeed < 0)
						videodiscontinuityskip(status.aktservice->videodev, -1);
					else
						videodiscontinuityskip(status.aktservice->videodev, 1);
				}
				pthread_mutex_unlock(&status.tsseekmutex);
			}

			if(frcount != 0 && status.playspeed == 0)
				frcount = 0;
			
			pthread_mutex_lock(&status.tsseekmutex);
			readret = dvbreadfd(servicenode->recsrcfd, buf, 0, recbsize, readtimeout, 1);
			if(readret <= 0 && status.timeshift > 0)
			{
				playerpausets();
				playercontinuets();
				//playerresetts();
				status.playspeed = 0;
				status.pause = 0;
				status.play = 1;
				playerseekts(servicenode, -3, 1);
				readret = dvbreadfd(servicenode->recsrcfd, buf, 0, recbsize, readtimeout, 1);
			}
			pthread_mutex_unlock(&status.tsseekmutex);
			if(vubox1 == 1)
				usleep(25000);				
		
			if(status.playspeed < 0 || status.playspeed > 0)
			{
				frcount += readret;
				if(frcount >= frbsize * frmultiread)
					frcount = 0;
			}
		}
		else
			readret = dvbreadfd(servicenode->recsrcfd, buf, 0, recbsize, readtimeout, 0);
#endif
		if(readret > 0)
		{
			if(servicenode->type == RECORDSTREAM)
			{
//Workaround scrambled Bits
				if(getconfigint("stream_scrambled", NULL) == 0)
				{
#ifdef MIPSEL
					debug(251, "data len %i", readret);
					if(servicenode->tssize == 188)
					{
						i = 0;
						if(buf[i] != 0x47)
						{
							debug(251, "no sync byte at beginn len %i", readret);
							i = 1;
							while(i <= 188)
							{
								if(buf[i] == 0x47)
								{
									 debug(251, "sync byte found at offset %i", i);
									 break;
								}
								i++;
							}
						}
						if(i <= 188)
						{
							while(i < readret-4)
							{
								if(buf[i] == 0x47)
								{
									buf[i+3] = buf[i+3] & 0x3f;
									i = i + 188;
								}
								else
								{
									debug(251, "no sync byte at data len %i", readret);
									while(i < readret-4)
									{
										i = i + 1;
										if(buf[i] == 0x47)
										{
											debug(251, "sync byte found at offset %i", i);
											buf[i+3] = buf[i+3] & 0x3f;
											i = i + 188;
											break;
										}
									}	
								}
							}
						}
					}
#endif
				}
//
				writeret = sockwrite(servicenode->recdstfd, buf, readret, writetimeout);
			}
			else
			{
				if(servicenode->type == RECORDPLAY && servicenode->tssize == 192)
				{
					// remove 4 bytes per paket from mts and m2ts streams
					pktcount = readret / 192;
					for(i = 0; i < pktcount; i++)
						memcpy(tmpbuf + (i * 188), buf + (i * 192) + 4, 188);
					writeret = dvbwrite(servicenode->recdstfd, tmpbuf, pktcount * 188, writetimeout);
					writeret = writeret + (pktcount * 4);
				}
				else
				{
				/*if(buf[0] != 0x47)
				{
					debug(200, "resync");
					i = 1;
					while(i < 188)
					{
						if(buf[i] == 0x47) break;
						i++;
					}
					if(i < 188)
					{
						memcpy(buf, buf + i, recbsize - i);
						dvbreadfd(servicenode->recsrcfd, buf, recbsize - i, i, readtimeout, 0);
					}
				}*/
//Workaround scrambled Bits
					if(getconfigint("rec_scrambled", NULL) == 0)
					{
#ifndef MIPSEL
						if(servicenode->type == RECORDPLAY)
						{
#endif			
							i = 0;
							if(buf[i] != 0x47)
							{
								i = 1;
								while(i <= 188)
								{
									if(buf[i] == 0x47) break;
									i++;
								}
							}
							if(i <= 188)
							{
								while(i < readret-4)
								{
									if(buf[i] == 0x47)
									{
										buf[i+3] = buf[i+3] & 0x3f;
										i = i + 188;
									}
									else
									{
										while(i < readret-4)
										{
											i = i + 1;
											if(buf[i] == 0x47)
											{
												buf[i+3] = buf[i+3] & 0x3f;
												i = i + 188;
												break;
											}
										}	
									}
								}
							}
#ifndef MIPSEL
						}						
#endif
					}
//*
					if((servicenode->type == RECORDDIRECT || servicenode->type == RECORDTIMER || servicenode->type == RECORDTIMESHIFT) && vubox1 ==1)
						usleep(25000);
					writeret = dvbwrite(servicenode->recdstfd, buf, readret, writetimeout);
					if((servicenode->type == RECORDDIRECT || servicenode->type == RECORDTIMER || servicenode->type == RECORDTIMESHIFT) && vubox1 ==1)
						usleep(25000);
				}
				
				//inject first pakets slower/smaler, so demux can start and read
				if(servicenode->type == RECORDPLAY && count < 20)
				{
					if(status.timeshift == 0)
						usleep(50000);
					else
						count = 19;
					if(count == 19)
					{
						recbsize = servicenode->tssize * 1024; //aligned to 188 and 4096
						tmprecbsize = 188 * 1024; //aligned to 188 and 4096

						// recordplay work vuplus start
						if(getconfigint("recordplay_recbsize", NULL) != 0 && recbsize != getconfigint("recordplay_recbsize", NULL))
						{
							printf("recordplay2 recbsize: %d\n", recbsize);
							recbsize = getconfigint("recordplay_recbsize", NULL);
							printf("recordplay2 recbsize change to: %d\n", recbsize);
//							recbsize = 12032;
						}
						// recordplay work vuplus end

						free(buf);
						buf = malloc(recbsize);
						if(buf == NULL)
						{
							err("no mem");
							servicenode->recendtime = 1;
						}

						if(servicenode->tssize == 192)
						{
							free(tmpbuf);
							tmpbuf = malloc(tmprecbsize);
							if(tmpbuf == NULL)
							{
								err("no mem");
								servicenode->recendtime = 1;
							}
						}
					}
					count++;
				}
			}

			if(writeret < 1)
			{
				ret = 9;
				if(writeret == -ENOSPC) ret = 17;
				servicenode->recendtime = 1;
			}
			else if(readret != writeret)
			{
				debug(250, "not all data written read=%d, written=%d", readret, writeret);
			}

			if(servicenode->type == RECORDDIRECT || servicenode->type == RECORDTIMER || servicenode->type == RECORDTIMESHIFT)
			{
				//sync
				if(recsync == 1)
				{
					servicenode->reclastsync += writeret;
					if(servicenode->reclastsync > 524288)
					{
						fdatasync(servicenode->recdstfd);
						posix_fadvise64(servicenode->recdstfd, 0, 0, POSIX_FADV_DONTNEED);
						servicenode->reclastsync = 0;
					}
				}

				if(status.recsplitsize && servicenode->type != RECORDTIMESHIFT)
				{
					servicenode->rectotal += writeret;
					if(servicenode->rectotal > status.recsplitsize)
						ret = recordsplit(servicenode, 0);
				}
			}
		}
		else if(readret <= 0)
		{
			if(readret == 0 && servicenode->type == RECORDPLAY)
			{
				if(getconfigint("playsplitfiles", NULL) == 1)
					ret = recordsplit(servicenode, 1);
				else
				{
					ret = 15;
					servicenode->recendtime = 1;
				}
			}
			else
			{
				ret = 15;
				servicenode->recendtime = 1;
			}

			if(readret < -1)
				perr("read");
		}

		if(servicenode->recendtime != 0 && servicenode->recendtime < time(NULL))
		{
			if(servicenode->type == RECORDDIRECT || servicenode->type == RECORDTIMER || servicenode->type == RECORDTIMESHIFT)
				fdatasync(servicenode->recdstfd);
			if(ret != 0) // error
			{
				if(servicenode->type == RECORDDIRECT || servicenode->type == RECORDTIMER || servicenode->type == RECORDTIMESHIFT)
					addtimer(&recordcheckret, START, 1000, 1, (void*)ret, (void*)3, NULL);
				retstr = recordcheckret(NULL, ret, 12);

				//if(servicenode->type == RECORDSTREAM)
				//	sockwrite(servicenode->recdstfd, (unsigned char*)retstr, strlen(retstr), -1);
				if(servicenode->type == RECORDTIMER)
				{
					m_lock(&status.rectimermutex, 1);
					struct rectimer* rectimernode = getrectimerbyservice(servicenode);
					if(rectimernode != NULL)
					{
						rectimernode->status = 3;
						free(rectimernode->errstr);
						rectimernode->errstr = ostrcat(retstr, NULL, 0, 0);
						status.writerectimer = 1;
						writerectimer(getconfig("rectimerfile", NULL), 1);
					}
					m_unlock(&status.rectimermutex, 1);
				}
				free(retstr); retstr = NULL;
			}
			recordstop(servicenode, ret);
			break;
		}
	}

#ifdef SIMULATE
	close(fd);
#endif
	if(buf != NULL) free(buf);
	if(tmpbuf != NULL) free(tmpbuf);
	debug(250, "stop read-write thread");
	return 0;
}

char* recordcreatefilename(char* path, char* channelname, char* moviename, int type)
{
	time_t sec;
	struct tm *loctime;
	char *buf = NULL, *buf1 = NULL;
	char* tmpstr = NULL;
	int recordnamefmt = getconfigint("recordnamefmt", NULL);

	if(path == NULL)
		return NULL;

	tmpstr = ostrcat(path, "/", 0, 0);
	if(type != RECTIMESHIFT && recordnamefmt == 0)
	{
		tmpstr = ostrcat(tmpstr, "(", 1, 0);
		if(channelname == NULL || strlen(channelname) == 0)
			tmpstr = ostrcat(tmpstr, "unknown", 1, 0);
		else
			tmpstr = ostrcat(tmpstr, channelname, 1, 0);
		tmpstr = ostrcat(tmpstr, ") ", 1, 0);
	}

	if(moviename == NULL || strlen(moviename) == 0)
		tmpstr = ostrcat(tmpstr, "unknown", 1, 0);
	else
		tmpstr = ostrcat(tmpstr, moviename, 1, 0);

	if(type != RECTIMESHIFT && recordnamefmt == 1)
	{
		tmpstr = ostrcat(tmpstr, " (", 1, 0);
		if(channelname == NULL || strlen(channelname) == 0)
			tmpstr = ostrcat(tmpstr, "unknown", 1, 0);
		else
			tmpstr = ostrcat(tmpstr, channelname, 1, 0);
	}

	sec = time(NULL);
	loctime = localtime(&sec);

	buf = malloc(MINMALLOC);
	if(buf == NULL)
	{
		err("no memory");
		return NULL;
	}

	strftime(buf, MINMALLOC, "%Y%m%d%H%M", loctime);
	buf1 = ostrcat(buf, NULL, 1, 0);

	if(type != RECTIMESHIFT && recordnamefmt == 1)
		tmpstr = ostrcat(tmpstr, "-", 1, 0);
	else
		tmpstr = ostrcat(tmpstr, " (", 1, 0);	
	tmpstr = ostrcat(tmpstr, buf1, 1, 1);
	tmpstr = ostrcat(tmpstr, ")", 1, 0);
	if(ostrcmp(channelname, "HDMIIN") == 0)
		tmpstr = ostrcat(tmpstr, ".mpeg", 1, 0);
	else
		tmpstr = ostrcat(tmpstr, ".ts", 1, 0);

	return tmpstr;
}

int recordstartreal(struct channel* chnode, int filefd, int recordfd, int type, time_t endtime, struct rectimer* rectimernode, int tssize)
{
	int ret = 0, fd = -1, servicetype = RECORDDIRECT, festatus = 0, pcrpidmatch = 0;
	char* path = NULL, *chname = NULL, *filename = NULL, *moviename = NULL;
	unsigned char* patbuf = NULL, *pmtbuf = NULL;
	struct epg* epgnode = NULL;
	struct service* servicenode = NULL;
	struct dvbdev* fenode = NULL, *dmxnode = NULL;
#ifdef MIPSEL
	struct dvbdev* encnode = NULL, *videonode = NULL, *audionode = NULL;
#endif
	struct audiotrack* atrack = NULL;
	struct subtitle *subnode = NULL;
	char* tmpstr = NULL;
	struct transponder* tpnode = NULL;
	int input = DMX_IN_FRONTEND;

	struct cadesc *canode = NULL;

	//wakeup hdd work
	if(type != RECSTREAM && type != RECSTREAMENC)
		wakeup_record_device();

	if(chnode == NULL && filefd < 0)
	{
		ret = 8;
		goto end;
	}

	//hdmi record.. serviceid=65535
	if(filefd < 0 && chnode->serviceid != 65535)
	{
		tpnode = chnode->transponder;
		if(tpnode == NULL)
		{
			ret = 8;
			goto end;
		}
	}

	switch(type)
	{
		case RECPLAY:
			servicetype = RECORDPLAY;
			fd = recordfd;
			break;
		case RECSTREAM:
			servicetype = RECORDSTREAM;
			fd = recordfd;
			break;
		case RECSTREAMENC:
			servicetype = RECORDSTREAM;
			fd = recordfd;
			break;
		case RECTIMESHIFT:
			servicetype = RECORDTIMESHIFT;
			path = getconfig("rec_timeshiftpath", NULL);
			moviename = "timeshift";
			break;
		case RECTIMER:
			servicetype = RECORDTIMER;
			if(rectimernode != NULL && rectimernode->recpath != NULL)
				path = rectimernode->recpath;
			else
				path = getconfig("rec_timerpath", NULL);
			if(chnode != NULL)
			{
				chname = strstrip(chnode->name);
				delspezchar(chname, 2);
			}
			if(rectimernode != NULL && rectimernode->name != NULL)
			{
				moviename = strstrip(rectimernode->name);
				delspezchar(moviename, 2);
			}
			break;
		default:
			servicetype = RECORDDIRECT;
			path = getconfig("rec_path", NULL);
			if(chnode != NULL)
			{
				chname = strstrip(chnode->name);
				delspezchar(chname, 2);
			}
			epgnode = getepgbytime(status.aktservice->channel, time(NULL) + 60);
			if(epgnode != NULL)
			{
				moviename = strstrip(epgnode->title);
				delspezchar(moviename, 2);

			}
			break;
	}

	if(type != RECSTREAM && type != RECSTREAMENC && type != RECPLAY)
	{
		if(!isdir(path))
		{
			ret = 1;
			goto end;
/*
not needed we use wakeup_record_device on recordstartreal
			char *dev = NULL;
			dev = getmoviedev();
			if(dev == NULL)
			{
				ret = 1;
				goto end;
			}
			else
			{
				free(dev);dev=NULL;
				sleep(3);
				if(!isdir(path))
				{
					ret = 1;
					goto end;
				}
			}
*/
		}

		//check HDD free space
		//deaktivate, on my 500GB FAT32 HDD, this takes mor then 10 min
#ifdef MIPSEL
		if(getfreespace(path) < getconfigint("recordfreespace", NULL) * 1024 * 1024)
		{
			ret = 2;
			goto end;
		}
#endif

		filename = recordcreatefilename(path, chname, moviename, type);
		if(filename == NULL)
		{
			ret = 3;
			goto end;
		}

		fd = open(filename, O_WRONLY | O_CREAT | O_LARGEFILE, 0666);
		if(fd < 0)
		{
			char* fn = strrchr(filename, '/');
			if(fn == NULL)
				delspezchar(filename, 0);
			else
				delspezchar(fn + 1, 0);
	
			fd = open(filename, O_WRONLY | O_CREAT | O_LARGEFILE, 0666);
			if(fd < 0)
			{
				ret = 4;
				goto end;
			}
		}
		posix_fadvise(fd, 0, 0, POSIX_FADV_RANDOM); //turn off kernel cache
	}

	printf("record.h--> addservice\n");
	servicenode = addservice(NULL);
	if(servicenode == NULL)
	{
		ret = 16;
		goto end;
	}
	servicenode->tssize = tssize;
	servicenode->recdstfd = fd;
	servicenode->channel = chnode;
	servicenode->transponder = tpnode;
	if(rectimernode != NULL) servicenode->rectimestamp = ostrcat(rectimernode->timestamp, NULL, 0, 0);
	printf("record.h--> filefd:%i\n",filefd);
	if(filefd < 0 && chnode->serviceid != 65535)
	{
		//got frontend dev
		fenode = fegetfree(tpnode, 2, NULL, chnode);
		if(fenode == NULL)
		{
			if(status.standby == 0 && (type == RECSTREAM || type == RECSTREAMENC))
			{
				ret = 5;
				goto end;
			}
			else
			{
				if(status.standby == 0 && textbox(_("Message"), _("Can't find free Tuner for Record.\nSwitch to Recording/Timeshift Channel ?"), _("EXIT"), getrcconfigint("rcexit", NULL), _("OK"), getrcconfigint("rcok", NULL), NULL, 0, NULL, 0, 800, 200, 10, 0) == 1)
				{
					ret = 14;
					goto end;
				}
				else
				{
					if(rectimernode != NULL)
						servicestart(chnode, rectimernode->channellist, rectimernode->pincode, 0);
					else
						servicestart(chnode, NULL, NULL, 0);
					
					if(status.standby > 0) servicestop(status.aktservice, 1, 0);	
					
					fenode = fegetfree(tpnode, 2, NULL, chnode);
					if(fenode == NULL)
					{
						ret = 5;
						goto end;
					}
				}
			}
		}
		if(type != RECTIMESHIFT) fenode->felock++;

		//frontend tune
		if(fenode != status.aktservice->fedev || (status.standby > 0 && getconfigint("standbytuneroff", NULL) == 1))
		{
			if(fenode->feinfo->type == FE_QPSK)
			{
				feset(fenode, tpnode);
				fetunedvbs(fenode, tpnode);
			}
			else if(fenode->feinfo->type == FE_QAM)
				fetunedvbc(fenode, tpnode);
			else if(fenode->feinfo->type == FE_OFDM)
				fetunedvbt(fenode, tpnode);
			else
			{
				ret = 12;
				if(type != RECTIMESHIFT) fenode->felock--;
				goto end;
			}

			festatus = fewait(fenode);
			if(debug_level == 200) fereadstatus(fenode);
			if(festatus != 0)
			{
				ret = 13;
				if(type != RECTIMESHIFT) fenode->felock--;
				goto end;
			}
		}

		servicenode->fedev = fenode;

		//demux  start
		dmxnode = dmxopen(fenode, 1);
		if(dmxnode != NULL && dmxnode->fd >= 0)
		{
			servicenode->recsrcfd = dmxnode->fd;
			if(type == RECTIMESHIFT)
				dmxsetbuffersize(dmxnode, getconfigint("dmxtimeshiftbuffersize", NULL));
			else if(type == RECSTREAM || type == RECSTREAMENC )
				dmxsetbuffersize(dmxnode, getconfigint("dmxstreambuffersize", NULL));
			else
				dmxsetbuffersize(dmxnode, getconfigint("dmxrecordbuffersize", NULL));
			servicenode->dmxaudiodev = dmxnode;
			servicenode->dmxvideodev = dmxnode;
			dmxsetsource(dmxnode, fenode->fedmxsource);
#if DVB_API_VERSION > 3
			dmxsetpesfilter(dmxnode, 0, input, DMX_OUT_TSDEMUX_TAP, DMX_PES_OTHER, 1);
#else
			dmxsetpesfilter(dmxnode, 0, input, DMX_OUT_TAP, 0, 1);
#endif
			patbuf = dvbgetpat(fenode, -1);
			chnode->pmtpid = dvbgetpmtpid(patbuf, chnode->serviceid);

			m_lock(&status.servicemutex, 2);
			if(status.aktservice->channel != chnode)
			{
				//reset channel info
				serviceresetchannelinfo(chnode);
				pmtbuf = dvbgetpmt(fenode, patbuf, chnode->serviceid, &chnode->pmtpid, NULL, -1, 0);
				dvbgetinfo(pmtbuf, chnode);
			}

			if(status.pmtmode == 1)
			{
				if(recordcheckcrypt(fenode, servicetype) == 0)
					dvbwritepmt(servicenode, pmtbuf);
				else
					debug(250, "don't write pmt.tmp, another crypt channel use this frontend");
			}
			else
				sendcapmt(servicenode, 0, 3);

			m_unlock(&status.servicemutex, 2);

			free(pmtbuf); pmtbuf = NULL;
			free(patbuf); patbuf = NULL;
			if((chnode->audiopid < 1 && chnode->videopid < 1) || chnode->pmtpid < 1)
			{
#ifndef SIMULATE
				ret = 7;
				if(type != RECTIMESHIFT) fenode->felock--;
				goto end;
#endif
			}
#ifdef MIPSEL
			if(type == RECSTREAMENC)
			{
	
				dmxclose(dmxnode, -1);

				encnode = encoderopen(0);
				servicenode->encoderdev = encnode;
				
				servicenode->dmxaudiodev = dmxopen(fenode, 2);
				dmxsetbuffersize(servicenode->dmxaudiodev, getconfigint("dmxaudiobuffersize", NULL));
				dmxsetsource(servicenode->dmxaudiodev, fenode->fedmxsource);
				switch(encnode->decoder)
				{
					case 2: dmxsetpesfilter(servicenode->dmxaudiodev, chnode->audiopid, -1, DMX_OUT_DECODER, DMX_PES_AUDIO2, 0); break;
					case 3: dmxsetpesfilter(servicenode->dmxaudiodev, chnode->audiopid, -1, DMX_OUT_DECODER, DMX_PES_AUDIO3, 0); break;
				}

				audionode = audioopen(encnode->decoder);
				servicenode->audiodev = audionode;
				audioselectsource(servicenode->audiodev, AUDIO_SOURCE_DEMUX);
				if(chnode->audiocodec != AC3)
					audiosetbypassmode(servicenode->audiodev, 1);
				else
					audiosetbypassmode(servicenode->audiodev, 0);
				audioplay(servicenode->audiodev);
				
				servicenode->dmxvideodev = dmxopen(fenode, 2);
				dmxsetbuffersize(servicenode->dmxvideodev, getconfigint("dmxvideobuffersize", NULL));
				dmxsetsource(servicenode->dmxvideodev, fenode->fedmxsource);
				
				switch(encnode->decoder)
				{
					case 2: dmxsetpesfilter(servicenode->dmxvideodev, chnode->videopid, -1, DMX_OUT_DECODER, DMX_PES_VIDEO2, 0); break;
					case 3: dmxsetpesfilter(servicenode->dmxvideodev, chnode->videopid, -1, DMX_OUT_DECODER, DMX_PES_VIDEO3, 0); break;
				}
				
				videonode = videoopen(0, encnode->decoder);
				servicenode->videodev = videonode;
				videoselectsource(servicenode->videodev, VIDEO_SOURCE_DEMUX);
				setencoding(chnode, servicenode->videodev);
				videoplay(servicenode->videodev);
				
				if(checkchipset("HI3798MV200") == 1 || vubox1 == 1)
				{
					videoslowmotion(servicenode->videodev, 0);
					videofastforward(servicenode->videodev, 0);
					videocontinue(servicenode->videodev);
					audiocontinue(servicenode->audiodev);
				}
				 
				encnode->fd = encoderopendirect(encnode->dev);
				servicenode->recdmxstart = 1;
				servicenode->recsrcfd = encnode->fd;
			}
			else
			{
#endif
				//add audiopid
				if(chnode->audiopid > 0) dmxaddpid(dmxnode, chnode->audiopid);
				//add videopid
				if(chnode->videopid > 0) dmxaddpid(dmxnode, chnode->videopid);
				//add pmtpid
				dmxaddpid(dmxnode, chnode->pmtpid);

				//add all audiotracks
				atrack = chnode->audiotrack;
				while(atrack != NULL)
				{
					if(atrack->audiopid > 0 && atrack->audiopid != chnode->audiopid)
						dmxaddpid(dmxnode, atrack->audiopid);
					if(atrack->audiopid == chnode->pcrpid) pcrpidmatch = 1;
					atrack = atrack->next;
				}
				if(chnode->pcrpid > 0 && chnode->pcrpid != chnode->videopid && chnode->pcrpid != chnode->audiopid && pcrpidmatch == 0) dmxaddpid(dmxnode, chnode->pcrpid);
			
				//add all subtitle
				m_lock(&status.subtitlemutex, 8);
				subnode = chnode->subtitle;
				while(subnode != NULL)
				{
					if(subnode->pid > 0)
						dmxaddpid(dmxnode, subnode->pid);
					subnode = subnode->next;
				}
				m_unlock(&status.subtitlemutex, 8);

				//add epg pid
				if(getconfigint("epg2record", NULL) == 1) dmxaddpid(dmxnode, 0x12);	

                //add PAT
//				dmxaddpid(dmxnode, 0);
                //add TDT
				dmxaddpid(dmxnode, 0x14);

                if(getconfigint("stream_ecm", NULL) == 1)
                {
                    //add ECM
				    canode = chnode->cadesc;
				    while(canode != NULL)
				    {
                        printf("record: find ECM Pid %d\n", canode->capid);
                        printf("record: find ECM Pid 0x%04x\n", canode->capid);
                        //if g_settings.streaming_ecmmode
					    if(canode->capid > 0)
                        {
                            printf("record: skip add ECM Pid 0x%04x\n", canode->capid);
						    dmxaddpid(dmxnode, canode->capid);
                        }
					    canode = canode->next;
				    }
                }
#ifdef MIPSEL
			}
#endif
		}
		else
		{
			ret = 6;
			if(type != RECTIMESHIFT) fenode->felock--;
			goto end;
		}
	}
#ifdef MIPSEL
	if(filefd < 0 && chnode->serviceid == 65535)
	//else if(chnode->serviceid == 65535 && servicetype == RECORDDIRECT)
	{
		ret = encoderset(-1, 1, 1024*1024*8, 1280, 720, 25000, 0, 0);
		ret = 0;
		encnode = encoderopen(0);
		servicenode->encoderdev = encnode;
 		
 		videonode = videoopen(0, encnode->decoder);
		servicenode->videodev = videonode;
		videoselectsource(servicenode->videodev, VIDEO_SOURCE_HDMI);
		videoplay(servicenode->videodev);

 		audionode = audioopen(encnode->decoder);
		servicenode->audiodev = audionode;
		audioselectsource(servicenode->audiodev, AUDIO_SOURCE_HDMI);
		audioplay(servicenode->audiodev);

 		encnode->fd = encoderopendirect(encnode->dev);
		servicenode->recdmxstart = 1;
		servicenode->recsrcfd = encnode->fd;
	}
#endif
	if(rectimernode != NULL)
		rectimernode->servicenode = servicenode;

	servicenode->recendtime = endtime;
	servicenode->reccount = 1;
	servicenode->type = servicetype;

	if(filefd < 0)
		deltranspondertunablestatus();
	else
		servicenode->recsrcfd = filefd;
#ifdef MIPSEL
	if(type == RECSTREAMENC)
		servicenode->recsrcfd = encnode->fd;
#endif

	if(type == RECSTREAM || type == RECSTREAMENC)
	{
		status.streaming++;
		servicenode->recname = ostrcat("stream ", oitoa(recordfd), 0, 1);
	}
	else if(type == RECTIMESHIFT)
	{
		status.timeshift = 1;
		servicenode->recname = ostrcat(filename, NULL, 0, 0);
	}
	else if(type == RECPLAY)
	{
		status.playing = 1;
		servicenode->recdmxstart = 1;
	}
	else if(type == RECDIRECT || type == RECTIMER)
	{
		status.recording++;
		int i;
		for(i = 0; i < 9; i++)
		{
			if(status.recchnode[i] == NULL)
			{
		 		status.recchnode[i] = chnode;
		 		break;
		 	}
		}
		servicenode->recname = ostrcat(filename, NULL, 0, 0);
		if(VFD_Recordthread == NULL && getconfigint("vfdisplayrecord", NULL) != 0)
			VFD_Recordthread = addtimer(&vfdrecordthread, START, 10000, 1, NULL, NULL, NULL);
#ifdef MIPSEL
		if(chnode->serviceid == 65535)
			servicenode->recsrcfd = encnode->fd;
#endif
	}

	if(type != RECSTREAM && type != RECSTREAMENC && type != RECTIMESHIFT && type != RECPLAY)
		recordwriteepg(filename, chnode, rectimernode);
	printf("record.h--> readwritethread start\n");
	//start readwrite thread
	addtimer(&readwritethread, START, 1000, 1, (void*)servicenode, NULL, NULL);

	if(type == RECTIMER && status.standby == 0)
	{
		char* subfile = NULL;
		subfile = ostrstr(filename, "/movie/");
		if(subfile != NULL)
			subfile = subfile + 7;
		else
			subfile = filename;
		tmpstr = ostrcat(_("Timer Record start !"), subfile, 0, 0);
		textbox(_("Message"), tmpstr, _("OK"), getrcconfigint("rcok", NULL), NULL, 0, NULL, 0, NULL, 0, 900, 200, 4, 0);
		free(tmpstr); tmpstr = NULL;
	}
	
	if(filename != NULL) debug(250, "rec filename = %s\n", filename);

end:
	if(ret > 0)
	{
		delservice(servicenode, 1);
		if(filename != NULL) unlink(filename);
	}
	free(filename); filename = NULL;
	return ret;
}

#ifdef MIPSEL
int recordstartencode(struct channel* chnode, int filefd, int recordfd, int type, time_t endtime, struct rectimer* rectimernode,	int bitrate, int width, int height, int framerate, int interlaced, int aspectratio)
{
	if(type == RECSTREAMENC)
	{
		if(bitrate == 0)
		{
			if(encodersetweb(-1, 1) != 0)
				return -1;
		}	
		else if(encoderset(-1, 1, bitrate, width, height, framerate, interlaced, aspectratio) != 0)
			return -1;
	}	
	return recordstartreal(chnode, filefd, recordfd, type, endtime, rectimernode, 188);
}
#endif
		
int recordstart(struct channel* chnode, int filefd, int recordfd, int type, time_t endtime, struct rectimer* rectimernode)
{
	return recordstartreal(chnode, filefd, recordfd, type, endtime, rectimernode, 188);
}

struct service* getrecordbyname(char* recname, int type)
{
	struct service* servicenode = service;

	if(recname == NULL)
	{
		err("NULL detect");
		return NULL;
	}

	while(servicenode != NULL)
	{
		if((type == -1 && (servicenode->type == RECORDDIRECT || servicenode->type == RECORDTIMER || servicenode->type == RECORDTIMESHIFT)) || type == servicenode->type)
		{
			if(servicenode->recname != NULL)
			{
				if(ostrstr(recname, servicenode->recname) != NULL)
					return servicenode;
			}
		}
		servicenode = servicenode->next;
	}

	return NULL;
}

int screenrecordduration(int minutes, int nextmin)
{
		int rcret = 0, ret = 0;
		struct skin* recordduration = getscreen("recordduration");
		struct skin* b1 = getscreennode(recordduration, "b1");
		char* tmpnr = NULL;

		if(nextmin == 0)
			b1->hidden = YES;
		else
		{
			if(nextmin > 9999) nextmin = 9999;
			b1->hidden = NO;
		}

		changemask(recordduration, "0000");
		if(minutes < 0) minutes = 0;
		if(minutes > 9999) minutes = 9999;
		tmpnr = malloc(5);
		snprintf(tmpnr, 5, "%04d", minutes);
		changeinput(recordduration, tmpnr);
		free(tmpnr); tmpnr = NULL;

		drawscreen(recordduration, 0, 0);
		addscreenrc(recordduration, recordduration);

		while(1)
		{
			rcret = waitrc(recordduration, 0, 0);
			if(nextmin > 0 && rcret == getrcconfigint("rcred", NULL))
			{
				changemask(recordduration, "0000");
				tmpnr = malloc(5);
				snprintf(tmpnr, 5, "%04d", nextmin);
				changeinput(recordduration, tmpnr);
				changeret(recordduration, tmpnr);
				free(tmpnr); tmpnr = NULL;
				drawscreen(recordduration, 0, 0);
			}
			if(rcret == getrcconfigint("rcexit", NULL))
			{
				changeret(recordduration, NULL);
				break;
			}
			if(rcret == getrcconfigint("rcok", NULL))
				break;
		}

		if(recordduration->ret != NULL && ostrcmp(recordduration->ret, "0") != 0)
			ret = atoi(recordduration->ret);

		delownerrc(recordduration);
		clearscreen(recordduration);

		return ret;
}

void screenrecordstop()
{
	char* tmpstr = NULL;
	struct service* servicenode = service;
	struct menulist* mlist = NULL, *mbox = NULL, *tmpmbox = NULL;

	printf("record --> stop\n"); 
	while(servicenode != NULL)
	{
		printf("record --> type:%d\n", servicenode->type);
		if((servicenode->type == RECORDDIRECT || servicenode->type == RECORDTIMER) && servicenode->recname != NULL)
		{
			printf("record --> recname:%s\n", servicenode->recname);
			tmpstr = ostrcat(tmpstr, _("stop"), 1, 0);
			tmpstr = ostrcat(tmpstr, " (", 1, 0);
			tmpstr = ostrcat(tmpstr, servicenode->recname, 1, 0);
			tmpstr = ostrcat(tmpstr, ")", 1, 0);
			
			tmpmbox = addmenulist(&mlist, tmpstr, NULL, NULL, 0, 0);
			free(tmpstr); tmpstr = NULL;
			if(tmpmbox != NULL)
			{
				tmpmbox->param = ostrcat(tmpmbox->param, _("stop"), 1, 0);
				tmpmbox->param = ostrcat(tmpmbox->param, " (", 1, 0);
				tmpmbox->param = ostrcat(tmpmbox->param, servicenode->recname, 1, 0);
				tmpmbox->param = ostrcat(tmpmbox->param, ")", 1, 0);
			}
		}
		servicenode = servicenode->next;
		printf("record --> nextnode\n");
	}

	mbox = menulistbox(mlist, "recordlist", _("Record"), NULL, NULL, NULL, 0, 0);
	
	if(mbox != NULL && mbox->param != NULL && ostrstr(mbox->param, _("stop")) == mbox->param)
	{
		servicenode = getrecordbyname(mbox->param, RECORDDIRECT);
		if(servicenode != NULL)
			servicenode->recendtime = 2;
		else
		{
			servicenode = getrecordbyname(mbox->param, RECORDTIMER);
			if(servicenode != NULL)
				servicenode->recendtime = 2;
		}
	}
	
	freemenulist(mlist, 1); mlist = NULL;
}

void screenrecorddirect()
{
	char* tmpstr = NULL;
	int ret = 0, ret1 = 0, newstart = 0;
	struct service* servicenode = service;
	struct epg* epgnode = NULL;
	struct menulist* mlist = NULL, *mbox = NULL, *tmpmbox = NULL;
	struct stimerthread *recthumbfirstthread = NULL;

	while(servicenode != NULL)
	{
		if(servicenode->type == RECORDDIRECT && servicenode->recname != NULL)
		{
			tmpstr = ostrcat(tmpstr, _("stop"), 1, 0);
			tmpstr = ostrcat(tmpstr, " (", 1, 0);
			tmpstr = ostrcat(tmpstr, servicenode->recname, 1, 0);
			tmpstr = ostrcat(tmpstr, ")", 1, 0);
			
			tmpmbox = addmenulist(&mlist, tmpstr, NULL, NULL, 0, 0);
			free(tmpstr); tmpstr = NULL;
			if(tmpmbox != NULL)
			{
				tmpmbox->param = ostrcat(tmpmbox->param, _("stop"), 1, 0);
				tmpmbox->param = ostrcat(tmpmbox->param, " (", 1, 0);
				tmpmbox->param = ostrcat(tmpmbox->param, servicenode->recname, 1, 0);
				tmpmbox->param = ostrcat(tmpmbox->param, ")", 1, 0);
			}
			
			tmpstr = ostrcat(tmpstr, _("change"), 1, 0);
			tmpstr = ostrcat(tmpstr, " (", 1, 0);
			tmpstr = ostrcat(tmpstr, servicenode->recname, 1, 0);
			tmpstr = ostrcat(tmpstr, ")", 1, 0);
			
			tmpmbox = addmenulist(&mlist, tmpstr, NULL, NULL, 0, 0);
			free(tmpstr); tmpstr = NULL;
			if(tmpmbox != NULL)
			{
				tmpmbox->param = ostrcat(tmpmbox->param, _("change"), 1, 0);
				tmpmbox->param = ostrcat(tmpmbox->param, " (", 1, 0);
				tmpmbox->param = ostrcat(tmpmbox->param, servicenode->recname, 1, 0);
				tmpmbox->param = ostrcat(tmpmbox->param, ")", 1, 0);
			}
		}
		servicenode = servicenode->next;
	}

	// currently works only 1 rec or timeshift
//	if((checkbox("VUSOLO2") == 1 && status.recording == 0) || checkbox("VUSOLO2") == 0)
//	{
		//65535 gleich HDMIIN
		if(status.aktservice->channel->serviceid != 65535)
			addmenulist(&mlist, _("add recording (stop after current event)"), NULL, NULL, 0, 0);
		addmenulist(&mlist, _("add recording (indefinitely)"), NULL, NULL, 0, 0);
		addmenulist(&mlist, _("add recording (enter duration)"), NULL, NULL, 0, 0);
//	}

	mbox = menulistbox(mlist, "recordlist", _("Record"), NULL, NULL, NULL, 0, 0);
	
	if(mbox != NULL)
	{
		if(mbox->param != NULL && ostrstr(mbox->param, _("stop")) == mbox->param)
		{
			servicenode = getrecordbyname(mbox->param, RECORDDIRECT);
			if(servicenode != NULL)
				servicenode->recendtime = 2;
		}
		if(mbox->param != NULL && ostrstr(mbox->param, _("change")) == mbox->param)
		{
			servicenode = getrecordbyname(mbox->param, RECORDDIRECT);
			if(servicenode != NULL)
			{
				int nextmin = 0;
				epgnode = getepgbytime(servicenode->channel, time(NULL));
				if(epgnode != NULL) epgnode = epgnode->next;
				if(epgnode != NULL) nextmin = (epgnode->endtime - time(NULL)) / 60;
				if(nextmin < 0) nextmin = 0;
				ret1 = (servicenode->recendtime - time(NULL)) / 60;
				ret1 = screenrecordduration(ret1, nextmin);
				if(ret1 > 0)
					servicenode->recendtime = time(NULL) + (ret1 * 60);
			}
		}
		if(ostrcmp(mbox->name, _("add recording (stop after current event)")) == 0)
		{
			epgnode = getepgbytime(status.aktservice->channel, time(NULL) + 60);
#ifndef SIMULATE
			if(epgnode != NULL && epgnode->endtime > time(NULL))
				ret = recordstart(status.aktservice->channel, -1, 0, RECDIRECT, epgnode->endtime, NULL);
			else
				textbox(_("Message"), _("Can't get EPG time or EPG endtime not ok"), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, NULL, 0, 800, 200, 0, 0);
#else
			ret = recordstart(status.aktservice->channel, -1,  0, RECDIRECT, time(NULL) + 5, NULL);
#endif
			newstart = 1;
		}
		if(ostrcmp(mbox->name, _("add recording (indefinitely)")) == 0)
		{
			ret = recordstart(status.aktservice->channel, -1, 0, RECDIRECT, 0, NULL);
			newstart = 1;
		}
		if(ostrcmp(mbox->name, _("add recording (enter duration)")) == 0)
		{
			ret1 = screenrecordduration(0, 0);

			if(ret1 > 0)
			{
				ret = recordstart(status.aktservice->channel, -1, 0, RECDIRECT, time(NULL) + (ret1 * 60), NULL);
				newstart = 1;
			}
		}

		recordcheckret(NULL, ret, 6);
		if(ret == 0 && newstart == 1)
		{
			textbox(_("Message"), _("Record started"), _("OK"), getrcconfigint("rcok", NULL), NULL, 0, NULL, 0, NULL, 0, 600, 200, 7, 0);
			if(getconfigint("recordpicture", NULL) == 1)
				recthumbfirstthread = addtimer(&createrecthumbfirstthread, START, 1000, 1, NULL, NULL, NULL);
		}
	}
	freemenulist(mlist, 1); mlist = NULL;
}

#endif
