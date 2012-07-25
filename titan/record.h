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
		}
		if(tmpstr != NULL)
		{
			if(checkbit(flag, 2) == 1) err("%s", tmpstr);
			if(checkbit(flag, 1) == 1) textbox(_("Record / Timeshift / Stream"), tmpstr, _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, NULL, 0, 600, 200, timeout, 0);
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

void recordstop(struct service* node)
{
	debug(1000, "in");
	struct rectimer* rectimernode = NULL;
	int afterevent = 1, type = -1;

	if(node != NULL)
	{
		type = node->type;

		if(node->fedev != NULL && node->type != RECORDTIMESHIFT)
			node->fedev->felock--;

		m_lock(&status.rectimermutex, 1);
		rectimernode = getrectimerbyservice(node);
		if(rectimernode != NULL)
			afterevent = rectimernode->afterevent;
		m_unlock(&status.rectimermutex, 1);
		
		delservice(node, 0);

		if(type == RECORDSTREAM)
			status.streaming--;
		else if(type == RECORDTIMESHIFT)
			status.timeshift = 0;
		else if(type == RECORDPLAY)
			status.playing = 0;
		else
			status.recording--;

		deltranspondertunablestatus();

		//afterevent: 0 = auto
		//afterevent: 1 = nothing
		//afterevent: 2 = standby 
		//afterevent: 3 = poweroff
		if(afterevent == 0)
		{
			if(status.startmode == 1) afterevent = 2;
			else if(getwaswakuptimer() == 1) afterevent = 3;

		}
		if(afterevent == 2 && status.standby == 0)
		{
			status.standby = 2;
			screenstandby();
		}
		if(afterevent == 3)
		{
			if(status.recording < 1)
				oshutdown(1, 3);
		}
	}

	debug(1000, "out");
}

int recordsplit(struct service* servicenode)
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
		fdatasync(servicenode->recdstfd);
		close(servicenode->recdstfd);
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

		debug(250, "split record file - Recording to %s...", filename);

		servicenode->recdstfd = open(filename, O_WRONLY|O_CREAT|O_LARGEFILE, 0644);
		if(servicenode->recdstfd < 0)
		{
			debug(250, "split record file - can't open recording file!");
			ret = 11;
			servicenode->recendtime = 1;
		}
		else
			posix_fadvise(servicenode->recdstfd, 0, 0, POSIX_FADV_RANDOM);
	}
	return ret;
}

int readwritethread(struct stimerthread* stimer, struct service* servicenode, int flag)
{
	int readret = 0, writeret = 0, ret = 0, recbsize = 0, tmprecbsize = 0, i = 0, pktcount = 0;
	int readtimeout = -1, writetimeout = -1;
	unsigned char* buf = NULL, *tmpbuf = NULL;
	char* retstr = NULL;
	//off64_t pos = 0;

	debug(250, "start read-write thread");

	if(servicenode == NULL)
	{
		err("servicenode = NULL");
		return 1;
	}

	if(servicenode->type == RECORDDIRECT || servicenode->type == RECORDTIMER || servicenode->type == RECORDTIMESHIFT)
	{
		recbsize = servicenode->tssize * 2788;
		readtimeout = 5000000;
		writetimeout = 5000000; //5 sec
	}
		
	if(servicenode->type == RECORDPLAY || servicenode->type == RECORDSTREAM)
	{
		if(servicenode->recdstfd < 0)
		{
			err("destination fd not ok")
			return 1;
		}
		if(servicenode->type == RECORDPLAY)
		{
			recbsize = servicenode->tssize * 2788;
			tmprecbsize = 188 * 2788;
		}
		if(servicenode->type == RECORDSTREAM) recbsize = servicenode->tssize * 2788;
		readtimeout = 5000000;
		writetimeout = 5000000;
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
			pthread_mutex_lock(&status.tsseekmutex);
			readret = dvbreadfd(servicenode->recsrcfd, buf, 0, recbsize, readtimeout, 1);
			pthread_mutex_unlock(&status.tsseekmutex);
		}
		else
			readret = dvbreadfd(servicenode->recsrcfd, buf, 0, recbsize, readtimeout, 0);
#endif
		if(readret > 0)
		{
			if(servicenode->type == RECORDSTREAM)
				writeret = sockwrite(servicenode->recdstfd, buf, readret, writetimeout);
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
					writeret = dvbwrite(servicenode->recdstfd, buf, readret, writetimeout);
			}

			if(writeret < 1)
			{
				ret = 9;
				servicenode->recendtime = 1;
			}
			else if(readret != writeret)
			{
				debug(250, "not all data written read=%d, written=%d", readret, writeret);
			}

			if(servicenode->type == RECORDDIRECT || servicenode->type == RECORDTIMER || servicenode->type == RECORDTIMESHIFT)
			{
				//sync
				servicenode->reclastsync += writeret;
				if(servicenode->reclastsync > 524288)
				{
					int tosync = servicenode->reclastsync > 2097152 ? 2097152 : servicenode->reclastsync &~ 4095; //sync max 2MB
					//pos = lseek64(servicenode->recdstfd, 0, SEEK_CUR);
					//pos -= tosync;
					//posix_fadvise64(servicenode->recdstfd, pos, tosync, POSIX_FADV_DONTNEED);
					servicenode->reclastsync -= tosync;

					//split only after sync
					if(status.recsplitsize && servicenode->type != RECORDTIMESHIFT)
					{
						servicenode->rectotal += tosync;
						if(servicenode->rectotal > status.recsplitsize)
						ret = recordsplit(servicenode);
					}
				}
			}
		}
		else if(readret <= 0)
		{
			ret = 15;
			servicenode->recendtime = 1;
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
			recordstop(servicenode);
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

	if(path == NULL)
		return NULL;

	tmpstr = ostrcat(path, "/", 0, 0);
	if(type != RECTIMESHIFT)
	{
		if(channelname == NULL || strlen(channelname) == 0)
			tmpstr = ostrcat(tmpstr, "unknown", 1, 0);
		else
			tmpstr = ostrcat(tmpstr, channelname, 1, 0);
		tmpstr = ostrcat(tmpstr, "-", 1, 0);
	}
	if(moviename == NULL || strlen(moviename) == 0)
		tmpstr = ostrcat(tmpstr, "unknown", 1, 0);
	else
		tmpstr = ostrcat(tmpstr, moviename, 1, 0);

	sec = time(NULL);
	loctime = localtime(&sec);

	buf = malloc(MINMALLOC);
	if(buf == NULL)
	{
		err("no memory");
		return NULL;
	}

	strftime(buf, MINMALLOC, "%Y%m%d%H%M%S", loctime);
	buf1 = ostrcat(buf, NULL, 1, 0);

	tmpstr = ostrcat(tmpstr, "-", 1, 0);
	tmpstr = ostrcat(tmpstr, buf1, 1, 1);
	tmpstr = ostrcat(tmpstr, ".ts", 1, 0);

	debug(1000, "out");
	return tmpstr;
}

int recordstartreal(struct channel* chnode, int filefd, int recordfd, int type, time_t endtime, struct rectimer* rectimernode, int tssize)
{
	int ret = 0, fd = -1, servicetype = RECORDDIRECT, festatus = 0;
	char* path = NULL, *chname = NULL, *filename = NULL, *moviename = NULL;
	unsigned char* patbuf = NULL, *pmtbuf = NULL;
	struct epg* epgnode = NULL;
	struct service* servicenode = NULL;
	struct dvbdev* fenode = NULL, *dmxnode = NULL;
	struct audiotrack* atrack = NULL;
	char* tmpstr = NULL;
	struct transponder* tpnode = NULL;
	int input = DMX_IN_FRONTEND;

	if(chnode == NULL && filefd < 0)
	{
		ret = 8;
		goto end;
	}

	if(filefd < 0)
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
				delspezchar(chname, 1);
			}
			if(rectimernode != NULL && rectimernode->name != NULL)
			{
				moviename = strstrip(rectimernode->name);
				delspezchar(moviename, 1);
			}
			break;
		default:
			servicetype = RECORDDIRECT;
			path = getconfig("rec_path", NULL);
			if(chnode != NULL)
			{
				chname = strstrip(chnode->name);
				delspezchar(chname, 1);
			}
			epgnode = getepgbytime(status.aktservice->channel, time(NULL) + 60);
			if(epgnode != NULL)
			{
				moviename = strstrip(epgnode->title);
				delspezchar(moviename, 1);
			}
			break;
	}

	if(type != RECSTREAM && type != RECPLAY)
	{
		if(!isdir(path))
		{
			ret = 1;
			goto end;
		}

		//check HDD free space
		//deaktivate, on my 500GB FAT32 HDD, this takes mor then 10 min
/*
		if(getfreespace(path) < getconfigint("recordfreespace", NULL) * 1024 * 1024)
		{
			ret = 2;
			goto end;
		}
*/

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
	
			fd = open(filename, O_WRONLY|O_CREAT|O_LARGEFILE, 0666);
			if(fd < 0)
			{
				ret = 4;
				goto end;
			}
		}
		posix_fadvise(fd, 0, 0, POSIX_FADV_RANDOM); //turn off kernel cache
	}

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

	if(filefd < 0)
	{
		//got frontend dev
		fenode = fegetfree(tpnode, 2, NULL);
		if(fenode == NULL)
		{
			if(type == RECSTREAM)
			{
				ret = 5;
				goto end;
			}
			else
			{
				if(textbox(_("Message"), _("Can't find free Tuner for Record.\nSwitch to Recording/Timeshift Channel ?"), _("EXIT"), getrcconfigint("rcexit", NULL), _("OK"), getrcconfigint("rcok", NULL), NULL, 0, NULL, 0, 600, 200, 10, 0) == 1)
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
					
					fenode = fegetfree(tpnode, 2, NULL);
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
		dmxnode = dmxopen(fenode);
		if(dmxnode != NULL && dmxnode->fd >= 0)
		{
			servicenode->recsrcfd = dmxnode->fd;
			if(type == RECTIMESHIFT)
				dmxsetbuffersize(dmxnode, getconfigint("dmxtimeshiftbuffersize", NULL));
			else if(type == RECSTREAM)
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
			if(chnode->audiopid > 0) dmxaddpid(dmxnode, chnode->audiopid);
			if(chnode->videopid > 0) dmxaddpid(dmxnode, chnode->videopid);
			dmxaddpid(dmxnode, chnode->pmtpid);
			//dmxaddpid(dmxnode, chnode->pcrpid);
			//add all audiotracks
			atrack = chnode->audiotrack;
			while(atrack != NULL)
			{
				if(atrack->audiopid > 0 && atrack->audiopid != chnode->audiopid)
					dmxaddpid(dmxnode, atrack->audiopid);
				atrack = atrack->next;
			}
		}
		else
		{
			ret = 6;
			if(type != RECTIMESHIFT) fenode->felock--;
			goto end;
		}
	}

	if(rectimernode != NULL)
		rectimernode->servicenode = servicenode;

	servicenode->recendtime = endtime;
	servicenode->reccount = 1;
	servicenode->type = servicetype;

	if(filefd < 0)
		deltranspondertunablestatus();
	else
		servicenode->recsrcfd = filefd;

	if(type == RECSTREAM)
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
		servicenode->recname = ostrcat(filename, NULL, 0, 0);
	}

	if(type != RECSTREAM && type != RECTIMESHIFT && type != RECPLAY)
		recordwriteepg(filename, chnode, rectimernode);
	if(type == RECTIMER)
	{
		tmpstr = ostrcat(_("Timer Record start !\n"), filename, 0, 0);
		textbox(_("Message"), tmpstr, _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, NULL, 0, 600, 200, 4, 0);
		free(tmpstr); tmpstr = NULL;
	}
	
	//start readwrite thread
	addtimer(&readwritethread, START, 1000, 1, (void*)servicenode, NULL, NULL);
	
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

int recordstart(struct channel* chnode, int filefd, int recordfd, int type, time_t endtime, struct rectimer* rectimernode)
{
	return recordstartreal(chnode, filefd, recordfd, type, endtime, rectimernode, 188);
}

struct service* getrecordbyname(char* recname, int type)
{
	debug(1000, "in");
	struct service* servicenode = service;

	if(recname == NULL)
	{
		debug(1000, "out -> NULL detect");
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
	debug(1000, "out");
}

int screenrecordduration(int minutes)
{
		int rcret = 0, ret = 0;
		struct skin* recordduration = getscreen("recordduration");
		char* tmpnr = NULL;

		changemask(recordduration, "0000");
		if(minutes < 0) minutes = 0;
		if(minutes > 9999) minutes = 9999;
		tmpnr = malloc(5);
		snprintf(tmpnr, 5, "%04d", minutes);
		changeinput(recordduration, tmpnr);
		free(tmpnr);

		drawscreen(recordduration, 0, 0);
		addscreenrc(recordduration, recordduration);

		while(1)
		{
			rcret = waitrc(recordduration, 0, 0);
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

	while(servicenode != NULL)
	{
		if((servicenode->type == RECORDDIRECT || servicenode->type == RECORDTIMER) && servicenode->recname != NULL)
		{
			tmpstr = ostrcat(tmpstr, _("stop"), 1, 0);
			tmpstr = ostrcat(tmpstr, " (", 1, 0);
			tmpstr = ostrcat(tmpstr, servicenode->recname, 1, 0);
			tmpstr = ostrcat(tmpstr, ")", 1, 0);
			
			tmpmbox = addmenulist(&mlist, tmpstr, NULL, NULL, 0, 0);
			free(tmpstr); tmpstr = NULL;
			if(tmpmbox != NULL)
			{
				tmpmbox->param = ostrcat(tmpmbox->param, "stop", 1, 0);
				tmpmbox->param = ostrcat(tmpmbox->param, " (", 1, 0);
				tmpmbox->param = ostrcat(tmpmbox->param, servicenode->recname, 1, 0);
				tmpmbox->param = ostrcat(tmpmbox->param, ")", 1, 0);
			}
		}
		servicenode = servicenode->next;
	}

	mbox = menulistbox(mlist, "recordlist", "Record", NULL, NULL, 0, 0);
	
	if(mbox != NULL && mbox->param != NULL && ostrstr(mbox->param, "stop") == mbox->param)
	{
		servicenode = getrecordbyname(mbox->param, RECORDDIRECT);
		if(servicenode != NULL)
			servicenode->recendtime = 1;
		else
		{
			servicenode = getrecordbyname(mbox->param, RECORDTIMER);
			if(servicenode != NULL)
				servicenode->recendtime = 1;
		}
	}
	
	freemenulist(mlist, 1); mlist = NULL;
}

void screenrecorddirect()
{
	char* tmpstr = NULL;
	int ret = 0, ret1 = 0;
	struct service* servicenode = service;
	struct epg* epgnode = NULL;
	struct menulist* mlist = NULL, *mbox = NULL, *tmpmbox = NULL;

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
				tmpmbox->param = ostrcat(tmpmbox->param, "stop", 1, 0);
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
				tmpmbox->param = ostrcat(tmpmbox->param, "change", 1, 0);
				tmpmbox->param = ostrcat(tmpmbox->param, " (", 1, 0);
				tmpmbox->param = ostrcat(tmpmbox->param, servicenode->recname, 1, 0);
				tmpmbox->param = ostrcat(tmpmbox->param, ")", 1, 0);
			}
		}
		servicenode = servicenode->next;
	}

	addmenulist(&mlist, "add recording (stop after current event)", NULL, NULL, 0, 0);
	addmenulist(&mlist, "add recording (indefinitely)", NULL, NULL, 0, 0);
	addmenulist(&mlist, "add recording (enter duration)", NULL, NULL, 0, 0);

	mbox = menulistbox(mlist, "recordlist", "Record", NULL, NULL, 0, 0);
	
	if(mbox != NULL)
	{
		if(mbox->param != NULL && ostrstr(mbox->param, "stop") == mbox->param)
		{
			servicenode = getrecordbyname(mbox->param, RECORDDIRECT);
			if(servicenode != NULL)
				servicenode->recendtime = 1;
		}
		if(mbox->param != NULL && ostrstr(mbox->param, "change") == mbox->param)
		{
			servicenode = getrecordbyname(mbox->param, RECORDDIRECT);
			if(servicenode != NULL)
			{
				ret1 = (servicenode->recendtime - time(NULL)) / 60;
				ret1 = screenrecordduration(ret1);
				if(ret1 > 0)
					servicenode->recendtime = time(NULL) + (ret1 * 60);
			}
		}
		if(ostrcmp(mbox->name, "add recording (stop after current event)") == 0)
		{
			epgnode = getepgbytime(status.aktservice->channel, time(NULL) + 60);
#ifndef SIMULATE
			if(epgnode != NULL && epgnode->endtime > time(NULL))
				ret = recordstart(status.aktservice->channel, -1, 0, RECDIRECT, epgnode->endtime, NULL);
			else
				textbox(_("Message"), _("Can't get EPG time or EPG endtime not ok"), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, NULL, 0, 600, 200, 0, 0);
#else
			ret = recordstart(status.aktservice->channel, -1,  0, RECDIRECT, time(NULL) + 5, NULL);
#endif
		}
		if(ostrcmp(mbox->name, "add recording (indefinitely)") == 0)
			ret = recordstart(status.aktservice->channel, -1, 0, RECDIRECT, 0, NULL);
		if(ostrcmp(mbox->name, "add recording (enter duration)") == 0)
		{
			ret1 = screenrecordduration(0);

			if(ret1 > 0)
				ret = recordstart(status.aktservice->channel, -1, 0, RECDIRECT, time(NULL) + (ret1 * 60), NULL);
		}

		recordcheckret(NULL, ret, 6);
	}
	freemenulist(mlist, 1); mlist = NULL;
}

#endif
