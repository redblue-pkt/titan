#ifndef CA_H
#define CA_H

//wait for a module inserted
void cawait(struct stimerthread* self, struct dvbdev* dvbnode, int tout)
{
	unsigned int i = 0;
	ca_slot_info_t info;

	if(dvbnode == NULL || self == NULL) return;
	info.num = dvbnode->devnr;

	while(self->aktion == START)
	{
		if(cagetslotinfo(dvbnode, &info) == 0)
		{
			if(info.flags & CA_CI_MODULE_READY)
				break;
		}
		sleep(1);
		i++;
		if(i >= tout) break;

		if(dvbnode->caslot != NULL && dvbnode->caslot->status == 100) break;
	}
}

//flag 0: don't flash buffer
//flag 1: flush buffer
void careseting(struct stimerthread* self, struct dvbdev* dvbnode, int flag)
{
	if(dvbnode != NULL)
	{
		struct queue* qe = NULL;
		unsigned char buf[256];

		ciclose(dvbnode, -1);
		usleep(300000);
		ciopen(dvbnode);

		//flush buffer
		if(flag == 1)
			while(dvbreadfd(dvbnode->fd, buf, 0, 256, -1, 0) > 0);

		//flush queue
		qe = getqueue(dvbnode->devnr);
		while(qe != NULL)
		{
			delqueue(qe, 0);
			qe = getqueue(dvbnode->devnr);
		}

		careset(dvbnode, dvbnode->devnr);
		cawait(self, dvbnode, 10);
	}
}

//wait for a while for some data und read it if some
int caread(struct dvbdev* dvbnode, unsigned char* buf, int* len)
{
	int ret = 0, c = 0;
	struct pollfd fds;

	if(dvbnode == NULL || dvbnode->fd < 0) return -1;

	fds.fd = dvbnode->fd;
	fds.events = POLLOUT | POLLPRI | POLLIN;

	ret = TEMP_FAILURE_RETRY(poll(&fds, 1, 300));

	if(ret < 0)
	{
		err("poll data");
		return -1; //error
	}
	else if(ret == 0)
		return -2; //timeout
	else if(ret > 0)
	{
		if(fds.revents & POLLIN)
		{
			int readret = 0;
retry:
			readret = TEMP_FAILURE_RETRY(read(dvbnode->fd, buf, *len));

			if(readret > 0)
			{
				if(debug_level == 620)
				{
					int i = 0;
					printf("CA Read (fd %d): > ", dvbnode->fd);
					for(i = 0; i < readret; i++)
						printf("%02x ", buf[i]);
					printf("\n");
				}

				*len = readret;
				return 0; //ready
			}
			if(readret < 0 && errno == EAGAIN && c < 10)
			{
				c++;
				usleep(300000);
				goto retry;
			}
			*len = 0;
			if(fds.revents & POLLOUT && c == 10 && getqueue(dvbnode->devnr) != NULL)
			{
				perr("caread but queue not empty, so test a write, ret = %d", readret);
				return 1;
			}
			perr("caread ret = %d", readret);
			return -1; //error
		}
		else if(fds.revents & POLLOUT)
			return 1; //write
		else if(fds.revents & POLLPRI)
			return 2; //change
	}

	perr("capoll ret = %d", ret);
	return -1; //error
}

int cawrite(struct dvbdev* dvbnode, int fd, unsigned char* buf, int count, int flag, int tout)
{
	int ret = 0;

	if(dvbnode != NULL)
	{
		if(debug_level == 620)
		{
			int i = 0;
			printf("CA Write (slot %d fd %d): > ", dvbnode->devnr, dvbnode->fd);
			for(i = 0; i < count; i++)
				printf("%02x ", buf[i]);
			printf("\n");
		}

		if(dvbnode->caslot != NULL && flag == 0) dvbnode->caslot->fastrun = getconfigint("camwait", NULL);
	}

	ret = dvbwrite(fd, buf, count, tout);
	if(checkbox("DM7020HD") == 1 || checkbox("DM7020HDV2") == 1)
		usleep(150000);
	if(ret >= 0 && ret == count && dvbnode != NULL && dvbnode->caslot != NULL) dvbnode->caslot->poll = 0;

	return ret;
}

//send some data on an fd, for a special slot and connid
int casend(struct dvbdev* dvbnode, unsigned char* buf, int len)
{
	unsigned char *tmpbuf = NULL;
	int flag = 0;

	if(dvbnode == NULL || dvbnode->caslot == NULL) return 1;
	tmpbuf = (unsigned char*) malloc(len + 10);
	if(tmpbuf == NULL)
	{
		err("no mem");
		return 1;
	}
	
#ifdef MIPSEL
	memcpy(tmpbuf, buf, len);
#else
	// should we send a data last ?
	if(buf != NULL)
	{
		if((buf[2] >= T_SB) && (buf[2] <= T_NEW_T_C))
			memcpy(tmpbuf, buf, len);
		else
		{
			//send data_last and data
			int lenfield = 0;

			tmpbuf[0] = dvbnode->devnr;
			tmpbuf[1] = dvbnode->caslot->connid;
			tmpbuf[2] = T_DATA_LAST;
			lenfield = writelengthfield(tmpbuf + 3, len + 1); //len
			tmpbuf[3 + lenfield] = dvbnode->caslot->connid; //transport connection identifier
			
			memcpy(tmpbuf + (4 + lenfield), buf, len);
			len += (4 + lenfield);
		}
	}
	else
	{
		//send a data last only
		tmpbuf[0] = dvbnode->devnr;
		tmpbuf[1] = dvbnode->caslot->connid;
		tmpbuf[2] = T_DATA_LAST;
		tmpbuf[3] = 1; //len
		tmpbuf[4] = dvbnode->caslot->connid; //transport connection identifier
		len = 5;
		flag = 1;
	}
#endif

	if(debug_level == 620)
	{
		int i = 0;
		printf("CA casend to queue (slot %d fd %d): > ", dvbnode->devnr, dvbnode->fd);
		for(i = 0; i < len; i++)
			printf("%02x ", tmpbuf[i]);
		printf("\n");
	}
	
#ifdef MIPSEL
	int ret = cawrite(dvbnode, dvbnode->fd, tmpbuf, len, flag, -1);
	if(ret < 0 || ret != len)
	{
		err("writing data to queue, slot %d", dvbnode->devnr);
		return 1; //error
	}
#else
	struct queue* qe = addqueue(dvbnode->devnr, tmpbuf, len, NULL, 0, flag, NULL);
	free(tmpbuf); tmpbuf = NULL;
	if(qe == NULL)
	{
		err("writing data to queue, slot %d", dvbnode->devnr);
		return 1; //error
	}
#endif

	return 0; //ready
}

void sendSPDU(struct dvbdev* dvbnode, unsigned char tag, void *data, int len, int sessionnr, void *apdu, int alen)
{
	unsigned char* buf = NULL;
	unsigned char *tmpbuf = NULL;

	buf = calloc(1, MINMALLOC);
	if(buf == NULL)
	{
		err("no mem");
		return;
	}

	debug(620, "send SPDU, nr %d", sessionnr);

	tmpbuf = buf;

	*tmpbuf++ = tag;
	tmpbuf += writelengthfield(tmpbuf, len + 2);

	if(data != NULL) memcpy(tmpbuf, data, len);
	tmpbuf += len;
	*tmpbuf++ = sessionnr >> 8;
	*tmpbuf++ = sessionnr;

	if(apdu != NULL) memcpy(tmpbuf, apdu, alen);
	tmpbuf += alen;

	casend(dvbnode, buf, tmpbuf - buf);
	free(buf); buf = NULL;
}

void sendAPDU(struct dvbdev* dvbnode, int sessionnr, unsigned char *tag, void *data, int len)
{
	debug(620, "send APDU, nr %d", sessionnr);

	unsigned char buf[len + 3 + 4];
	int lenfield = 0;

	memcpy(buf, tag, 3);
	lenfield = writelengthfield(buf + 3, len);

	if(data != NULL) memcpy(buf + 3 + lenfield, data, len);
	sendSPDU(dvbnode, 0x90, NULL, 0, sessionnr, buf, len + 3 + lenfield);
}

int parselenfield(unsigned char *buf, int* len)
{
	int i;

	*len = 0;

	if(!(*buf & 0x80))
	{
		*len = *buf;
		return 1;
	}

	for(i = 0; i < (buf[0] & 0x7F); ++i)
	{
		*len <<= 8;
		*len |= buf[i + 1];
	}

	return (buf[0] & 0x7F) + 1;
}

int asn1decode(uint16_t* lenret, unsigned char* asn1array, uint32_t asn1arraylen)
{
	uint8_t len;

	if(asn1arraylen < 1 || asn1array == NULL) return -1;
	len = asn1array[0];

	if(len < 0x80)
	{
		//there is only one word
		*lenret = len & 0x7f;
		return 1;
	}
	else if(len == 0x81)
	{
		if(asn1arraylen < 2) return -1;
		*lenret = asn1array[1];
		return 2;
	}
	else if(len == 0x82)
	{
		if(asn1arraylen < 3) return -1;
		*lenret = (asn1array[1] << 8) | asn1array[2];
		return 3;
	}

	return -1;
}

//mmi functions

int cammistop(struct dvbdev* dvbnode, int sessionnr)
{
	debug(620, "cammistop");

	unsigned char tag[] = {0x9f, 0x88, 0x00};
	unsigned char data[] = {0x00};
	sendAPDU(dvbnode, sessionnr, tag, data, 1);

	return 0;
}

int cammianswer(struct dvbdev* dvbnode, int sessionnr, int answer)
{
	debug(620, "cammianswer: %d", answer);

	unsigned char tag[] = {0x9f, 0x88, 0x0B};
	unsigned char data[] = {0x00};
	data[0] = answer & 0xff;
	sendAPDU(dvbnode, sessionnr, tag, data, 1);

	return 0;
}

int cammianswerenq(struct dvbdev* dvbnode, int sessionnr, char *answer, int len)
{
	debug(620, "cammianswerenq (%d): %s", len, answer);

	unsigned char data[len + 1];
	data[0] = 0x01; //answer ok
	memcpy(data + 1, answer, len);

	unsigned char tag[] = {0x9f, 0x88, 0x08};
	sendAPDU(dvbnode, sessionnr, tag, data, len + 1);

	return 0;
}

int cammicancelenq(struct dvbdev* dvbnode, int sessionnr)
{
	debug(620, "cammicancelenq");

	unsigned char tag[] = {0x9f, 0x88, 0x08};
	unsigned char data[] = {0x00}; // canceled
	sendAPDU(dvbnode, sessionnr, tag, data, 1);

	return 0;
}

int cammiaction(struct dvbdev* dvbnode, int sessionnr)
{
	struct casession* casession = NULL;

	if(dvbnode == NULL || dvbnode->caslot == NULL) return 0;
	casession = dvbnode->caslot->casession;

	debug(620, "cammiaction nr %d, stat %d", sessionnr, casession[sessionnr].state);

	switch(casession[sessionnr].state)
	{
		case CASESSIONSTART:
			debug(620, "state mmi sessionstart");
			casession[sessionnr].state = CAMMIIDLE;
			break;
		case CAMMIDISPLAYREPLAY:
		{
			debug(620, "state cammidisplayreplay");
			unsigned char tag[] = {0x9f, 0x88, 0x02};
			unsigned char data[] = {0x01, 0x01};
			sendAPDU(dvbnode, sessionnr, tag, data, 2);
			casession[sessionnr].state = CAMMIIDLE;
			//casession[sessionnr].state = CAMMIFAKEOK;
			//return 1;
			break;
		}
		case CAMMIFAKEOK:
		{
			debug(620, "state cammifakeok");
			unsigned char tag[] = {0x9f, 0x88, 0x0b};
			unsigned char data[] = {5};
			sendAPDU(dvbnode, sessionnr, tag, data, 1);
			casession[sessionnr].state = CAMMIIDLE;
			break;
		}
		case CAMMIIDLE:
			debug(620, "state cammiidle");
			break;
		default:
			break;
	}
	return 0;
}

int cammiAPDU(struct dvbdev* dvbnode, int sessionnr, unsigned char *tag, void *data, int len)
{
	char* tmpstr = NULL, *tmpstr1 = NULL;
	struct casession* casession = NULL;
	struct menulist* mlist = NULL, *mbox = NULL;

	if(dvbnode == NULL || dvbnode->caslot == NULL) return 0;
	casession = dvbnode->caslot->casession;

	debug(620, "mmi manager %02x %02x %02x", tag[0], tag[1], tag[2]);

	if(debug_level == 620)
	{
		int i = 0;
		printf("CA manager data (len %d): > ", len);
		for(i = 0; i < len; i++)
			printf("%02x ", ((unsigned char*)data)[i]);
		printf("\n");
	}

	if(tag[0] == 0x9f && tag[1] == 0x88)
	{
		switch(tag[2])
		{
			case 0x00: //close
				cammistop(dvbnode, sessionnr);
				break;
			case 0x01: //display control
				casession[sessionnr].state = CAMMIDISPLAYREPLAY;
				return 1;
				break;
			case 0x07: //menu enq
			{
				unsigned char *tmpdata = data;
				unsigned char *max = tmpdata + len;
				int textlen = len - 2;

				if(tmpdata + 2 > max) break;
				int blind = *tmpdata++ & 1;
				int alen = *tmpdata++;

				debug(620, "mmi manager text len: %d, blind: %d", textlen, blind);

				if(tmpdata + textlen > max) break;

				char* str = malloc(textlen + 1);
				if(str == NULL)
				{
					cammicancelenq(dvbnode, sessionnr);
					break;
				}

				memcpy(str, tmpdata, textlen);
				str[textlen] = '\0';
				str = string_newline(str);

				debug(620, "mmi manager text: %s", str);

				int i = 0;
				for(i = 0; i < alen; i++) tmpstr1 = ostrcat(tmpstr1, "0", 1, 0);
				tmpstr = textinput(str, tmpstr1);
				if(tmpstr == NULL)
					cammicancelenq(dvbnode, sessionnr);
				else
				{
					if(strlen(tmpstr) > alen) tmpstr[alen] = '\0';
					cammianswerenq(dvbnode, sessionnr, tmpstr, strlen(tmpstr));
				}
				free(tmpstr); tmpstr = NULL;
				free(tmpstr1); tmpstr1 = NULL;

			}
			break;
			case 0x09: //menu last
			case 0x0c: //list last
			{
				unsigned char *tmpdata = data;
				unsigned char *max= tmpdata + len;
				int pos = 0;

				if(tag[2] == 0x09)
				{
					debug(620, "mmi manager menu last");
				}
				else
				{
					debug(620, "mmi manager list last");
				}

				if(tmpdata > max) break;

				int n = *tmpdata++;

				if(n == 0xFF)
					n = 0;
				else
					n++;

				int i = 0;
				for(i = 0; i < (n + 3); ++i)
				{
					int textlen = 0;
					if(tmpdata + 3 > max) break;

					debug(620, "mmi text tag: %02x %02x %02x", tmpdata[0], tmpdata[1], tmpdata[2]);
					tmpdata += 3;
					tmpdata += parselenfield(tmpdata, &textlen);

					debug(620, "mmi manager text len: %d", textlen);
					if(tmpdata + textlen > max) break;

					char* str = malloc(textlen + 1);
					if(str == NULL) break;
					memcpy(str, tmpdata, textlen);
					str[textlen] = '\0';
					str = string_newline(str);

					int type = pos++;

					if(type == 0) // title
						casession->mmititle = ostrcat(casession->mmititle, str, 1, 0);
					else if(type == 1) // subtitle
						casession->mmisubtitle = ostrcat(casession->mmisubtitle, str, 1, 0);
					else if(type == 2) // bottom
						casession->mmibottom = ostrcat(casession->mmibottom, str, 1, 0);
					else // text
					{
						casession->mmitext = ostrcat(casession->mmitext, str, 1, 0);
						casession->mmitext = ostrcat(casession->mmitext, "\n", 1, 0);
					}

					if(debug_level == 620)
					{
						printf("MMI Text: > ");
						while(textlen--)
							printf("%c", *tmpdata++);
						printf("\n");
					}
					else
					{
						while(textlen--)
							tmpdata++;
					}

				}

				if(tag[2] == 0x09) //MENU
				{
					tmpstr = ostrcat(tmpstr, casession->mmititle, 1, 0);
					tmpstr = ostrcat(tmpstr, " - ", 1, 0);
					tmpstr = ostrcat(tmpstr, casession->mmisubtitle, 1, 0);
					tmpstr1 = ostrcat(tmpstr1, casession->mmitext, 1, 0);
					
					addmenulistall(&mlist, tmpstr1, NULL, 0, NULL);
					mbox = menulistbox(mlist, "menulist", tmpstr, NULL, NULL, NULL, 1, 0);
					if(mbox == NULL) //exit
						cammistop(dvbnode, sessionnr);
					else //got selnr
					{
						int i = 0, len = 0, slen = 0, selnr = 0, cmp = 1;

						if(casession->mmitext != NULL)
							len = strlen(casession->mmitext);
						slen = strlen(mbox->name);
						for(i = 0; i < len; i++)
						{
							if(cmp == 1)
							{
								cmp = 0;
								if(ostrncmp(mbox->name, &casession->mmitext[i], slen) == 0)
									break;
							}

							if(casession->mmitext[i] == '\n')
							{
								selnr++;
								cmp = 1;
							}
						}

						cammianswer(dvbnode, sessionnr, selnr + 1);
					}
					freemenulist(mlist, 1); mlist = NULL;
					free(tmpstr); tmpstr = NULL;
					free(tmpstr1); tmpstr1 = NULL;
					free(casession->mmititle); casession->mmititle = NULL;
					free(casession->mmisubtitle); casession->mmisubtitle = NULL;
					free(casession->mmitext); casession->mmitext = NULL;
					free(casession->mmibottom); casession->mmibottom = NULL;
				}
				else //LIST
				{
					tmpstr = ostrcat(tmpstr, casession->mmititle, 1, 0);
					if(casession->mmisubtitle != NULL)
					{
						tmpstr1 = ostrcat(tmpstr1, casession->mmisubtitle, 1, 0);
						tmpstr1 = ostrcat(tmpstr1, "\n\n", 1, 0);
					}
					if(casession->mmitext != NULL)
					{
						tmpstr1 = ostrcat(tmpstr1, casession->mmitext, 1, 0);
						tmpstr1 = ostrcat(tmpstr1, "\n\n", 1, 0);
					}
					tmpstr1 = ostrcat(tmpstr1, casession->mmibottom, 1, 0);
					if(getconfigint("nocamsg", NULL) == 0)
						textbox(_(tmpstr), _(tmpstr1), NULL, getrcconfigint("rcok", NULL), NULL, getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 300, 7, 0);
					free(tmpstr); tmpstr = NULL;
					free(tmpstr1); tmpstr1 = NULL;
					free(casession->mmititle); casession->mmititle = NULL;
					free(casession->mmisubtitle); casession->mmisubtitle = NULL;
					free(casession->mmitext); casession->mmitext = NULL;
					free(casession->mmibottom); casession->mmibottom = NULL;
					cammistop(dvbnode, sessionnr);
				}

			}
			break;
		}
	}
	return 0;
}

//ca functions

int cacaaction(struct dvbdev* dvbnode, int sessionnr)
{
	struct casession* casession = NULL;

	if(dvbnode == NULL || dvbnode->caslot == NULL) return 0;
	casession = dvbnode->caslot->casession;

	debug(620, "cacaaction nr %d, stat %d", sessionnr, casession[sessionnr].state);

	switch(casession[sessionnr].state)
	{
		case CASESSIONSTART:
		{
			debug(620, "state ca sessionstart");
			unsigned char tag[3] = {0x9F, 0x80, 0x30}; //ca info enq
			sendAPDU(dvbnode, sessionnr, tag, NULL, 0);
			casession[sessionnr].state = CASESSIONFINAL;
			return 0;
		}
		case CASESSIONFINAL:
			debug(620, "state cacafinal and action is not ok");
		default:
			return 0;
	}
}

int cacaAPDU(struct dvbdev* dvbnode, int sessionnr, unsigned char *tag, void *data, int len)
{
	int i = 0;
	struct casession* casession = NULL;

	if(dvbnode == NULL || dvbnode->caslot == NULL) return 0;
	casession = dvbnode->caslot->casession;

	debug(620, "ca manager %02x %02x %02x", tag[0], tag[1], tag[2]);

	if(debug_level == 620)
	{
		i = 0;
		printf("CA manager data (len %d): > ", len);
		for(i = 0; i < len; i++)
			printf("%02x ", ((unsigned char*)data)[i]);
		printf("\n");
	}

	if(tag[0] == 0x9f && tag[1] == 0x80)
	{
		switch(tag[2])
		{
			case 0x31: //ca info
				if(debug_level == 620)
				{
					i = 0;
					printf("Ca info (len %d): > ", len);
					for(i = 0; i < len; i += 2)
					{
						printf("%04x ", (((unsigned char*)data)[i] << 8) | (((unsigned char*)data)[i + 1]));
					}
					printf("\n");
				}

				free(dvbnode->caslot->caids);
				dvbnode->caslot->caids = NULL;
				i = 0;
				for(i = 0; i < len; i += 2)
				{
					dvbnode->caslot->caids = ostrcat(dvbnode->caslot->caids, "#", 1, 0);
					dvbnode->caslot->caids = ostrcat(dvbnode->caslot->caids, oitoa((((unsigned char*)data)[i] << 8) | ((unsigned char*)data)[i + 1]), 1, 1);
					dvbnode->caslot->caids = ostrcat(dvbnode->caslot->caids, "#", 1, 0);
				}
				break;
			case 0x33: //ca pmt reply
				if(len > 3)
				{
					if(((unsigned char*)data)[3] == 0x81) //can descrambling
					{
						status.checkcamdecrypt = -1;
						debug(620, "cam say's he can handel decrypt");
					}
					else
					{
						status.checkcamdecrypt = -2;
						debug(620, "cam say's he can not handel decrypt");
					}
				}
				break;
			default:
				debug(620, "unknown APDU tag 9F 80 %02x\n", tag[2]);
				break;
		}
	}
	return 0;
}

int caccAPDU(struct dvbdev* dvbnode, int sessionnr, unsigned char *tag, void *data, int len)
{
	debug(620, "cc manager caccAPDU start");

	int i = 0;
	struct casession* casession = NULL;

	if(dvbnode == NULL || dvbnode->caslot == NULL) return 0;
	casession = dvbnode->caslot->casession;

	debug(620, "cc manager %02x %02x %02x", tag[0], tag[1], tag[2]);

	if(debug_level == 620)
	{
		i = 0;
		printf("CC manager data (len %d): > ", len);
		for(i = 0; i < len; i++)
			printf("%02x ", ((unsigned char*)data)[i]);
		printf("\n");
	}

	if(tag[0] == 0x9f && tag[1] == 0x90)
	{
		switch(tag[2])
		{
			case 0x01: ci_ccmgr_cc_open_cnf(dvbnode, sessionnr); break;
			case 0x03: ci_ccmgr_cc_data_req(dvbnode, sessionnr, (uint8_t*)data, len); break;
			case 0x05: ci_ccmgr_cc_sync_req(dvbnode, sessionnr); break;
			case 0x07: ci_ccmgr_cc_sac_data_req(dvbnode, sessionnr, (uint8_t*)data, len); break;
			case 0x09: ci_ccmgr_cc_sac_sync_req(dvbnode, sessionnr, (uint8_t*)data, len); break;
			default:
				debug(620, "unknown APDU tag 9F 80 %02x\n", tag[2]);
				break;
		}
	}
	return 0;
}

//datetime functions

int cadatetimeaction(struct dvbdev* dvbnode, int sessionnr)
{
	struct casession* casession = NULL;

	if(dvbnode == NULL || dvbnode->caslot == NULL) return 0;
	casession = dvbnode->caslot->casession;

	debug(620, "cadatetimeaction nr %d, stat %d", sessionnr, casession[sessionnr].state);

	switch(casession[sessionnr].state)
	{
		case CASESSIONSTART:
			debug(620, "state cadatetime sessionstart");
			return 0;
		case CADATETIMESEND:
		{
			debug(620, "state cadatetimesend");
			unsigned char tag[3] = {0x9f, 0x84, 0x41}; //datetime response
			unsigned char msg[7] = {0, 0, 0, 0, 0, 0, 0};
			sendAPDU(dvbnode, sessionnr, tag, msg, 7);
			return 0;
		}
		case CASESSIONFINAL:
			debug(620, "state cadatetimefinal and action is not ok");
		default:
			return 0;
	}
}

int cadatetimeAPDU(struct dvbdev* dvbnode, int sessionnr, unsigned char *tag, void *data, int len)
{
	struct casession* casession = NULL;

	if(dvbnode == NULL || dvbnode->caslot == NULL) return 0;
	casession = dvbnode->caslot->casession;

	debug(620, "datetime manager %02x %02x %02x", tag[0], tag[1], tag[2]);

	if(debug_level == 620)
	{
		int i = 0;
		printf("Datetime manager data (len %d): > ", len);
		for(i = 0; i < len; i++)
			printf("%02x ", ((unsigned char*)data)[i]);
		printf("\n");
	}

	if(tag[0] == 0x9f && tag[1] == 0x84)
	{
		switch(tag[2])
		{
			case 0x40:
				casession[sessionnr].state = CADATETIMESEND;
				return 1;
				break;
			default:
				debug(620, "unknown APDU tag 9F 84 %02x\n", tag[2]);
				break;
		}
	}

	return 0;
}

//res function

int caresaction(struct dvbdev* dvbnode, int sessionnr)
{
	struct casession* casession = NULL;

	if(dvbnode == NULL || dvbnode->caslot == NULL) return 0;
	casession = dvbnode->caslot->casession;

	debug(620, "caresaction nr %d, stat %d", sessionnr, casession[sessionnr].state);

	switch(casession[sessionnr].state)
	{
		case CASESSIONSTART:
		{
			debug(620, "state cares sessionstart");
			unsigned char tag[3] = {0x9F, 0x80, 0x10}; //profile enquiry
			checkcerts();
			sendAPDU(dvbnode, sessionnr, tag, NULL, 0);
			casession[sessionnr].state = CARESFIRSTENQUIRY;
			return 0;
		}
		case CARESFIRSTENQUIRY:
		{
			debug(620, "state caresfirstenquiry");
			unsigned char tag[3] = {0x9F, 0x80, 0x12}; //profile change
			sendAPDU(dvbnode, sessionnr, tag, NULL, 0);
			casession[sessionnr].state = CARESCHANGE;
			return 0;
		}
		case CARESCHANGE:
		{
			debug(620, "state careschange not ok");
			break;
		}
		case CARESENQUIRY:
		{
			debug(620, "state caresenquiry");
			unsigned char tag[3] = {0x9F, 0x80, 0x11};

			if(checkcerts() == 1)
			{
				unsigned char data[][4]=
				{
					{0x00, 0x01, 0x00, 0x41},	// resource
					{0x00, 0x02, 0x00, 0x41},	// application V1
					{0x00, 0x02, 0x00, 0x43},	// application V3
					{0x00, 0x03, 0x00, 0x41},	// conditional access
//					{0x00, 0x20, 0x00, 0x41},	// host control
					{0x00, 0x40, 0x00, 0x41},	// mmi
					{0x00, 0x24, 0x00, 0x41},	// date-time
					{0x00, 0x8c, 0x10, 0x01}	// content control
//					{0x00, 0x10, 0x00, 0x41}	// auth.
				};
				sendAPDU(dvbnode, sessionnr, tag, data, sizeof(data));
			}
			else
			{			
				unsigned char data[][4]=
				{
					{0x00, 0x01, 0x00, 0x41},
					{0x00, 0x02, 0x00, 0x41},
					{0x00, 0x03, 0x00, 0x41},
					//{0x00, 0x20, 0x00, 0x41}, //host control
					{0x00, 0x24, 0x00, 0x41},
					{0x00, 0x40, 0x00, 0x41}
					//{0x00, 0x10, 0x00, 0x41} //auth.
				};
				sendAPDU(dvbnode, sessionnr, tag, data, sizeof(data));
			};
			casession[sessionnr].state = CASESSIONFINAL;
			return 0;
		}
		case CASESSIONFINAL:
			debug(620, "state caresfinal and action is not ok");
		default:
			break;
	}

	return 0;
}

int caresAPDU(struct dvbdev* dvbnode, int sessionnr, unsigned char *tag, void *data, int len)
{
	struct casession* casession = NULL;

	if(dvbnode == NULL || dvbnode->caslot == NULL) return 0;
	casession = dvbnode->caslot->casession;

	debug(620, "res manager %02x %02x %02x", tag[0], tag[1], tag[2]);

	if(debug_level == 620)
	{
		int i = 0;
		printf("Res manager data (len %d): > ", len);
		for(i = 0; i < len; i++)
			printf("%02x ", ((unsigned char*)data)[i]);
		printf("\n");
	}

	if(tag[0] == 0x9f && tag[1] == 0x80)
	{
		switch(tag[2])
		{
			case 0x10:  // profile enquiry
				debug(620, "cam ask what can i");
				casession[sessionnr].state = CARESENQUIRY;
				return 1;
			case 0x11: // Tprofile
				if(debug_level == 620)
				{
					printf("My cam can: > ");
					if(!len)
						printf("nothing\n");
					else
					{
						int i = 0;
						for (i = 0; i < len; i++)
							printf("%02x ", ((unsigned char*)data)[i]);
					}
				}

				if(casession[sessionnr].state == CARESFIRSTENQUIRY)
				{
					// profile change
					return 1;
				}
				casession[sessionnr].state = CASESSIONFINAL;
				break;
			default:
				debug(620, "unknown APDU tag 9F 80 %02x\n", tag[2]);
		}
	}

	return 0;
}

//app functions

void caappmenu(struct dvbdev* dvbnode)
{
	//must be send to the app session
	debug(620, "caappmenu");
	unsigned char tag[3] = {0x9F, 0x80, 0x22};  //Tenter_menu
	int sessionnr = 0;

	sessionnr = getfreecasession(dvbnode, 1, 1);
	if(sessionnr > -1)
		sendAPDU(dvbnode, sessionnr, tag, NULL, 0);
}

int caappaction(struct dvbdev* dvbnode, int sessionnr)
{
	struct casession* casession = NULL;

	if(dvbnode == NULL || dvbnode->caslot == NULL) return 0;
	casession = dvbnode->caslot->casession;

	debug(620, "caappaction nr %d, stat %d", sessionnr, casession[sessionnr].state);

	switch(casession[sessionnr].state)
	{
		case CASESSIONSTART:
		{
			debug(620, "state app sessionstart");
			unsigned char tag[3] = {0x9F, 0x80, 0x20}; //app manager info

			sendAPDU(dvbnode, sessionnr, tag, NULL, 0);
			casession[sessionnr].state = CASESSIONFINAL;
			return 1;
		}
		case CASESSIONFINAL:
		{
			debug(620, "state app sessionfinal");
			return 0;
		}
		default:
			return 0;
	}
}

int caappAPDU(struct dvbdev* dvbnode, int sessionnr, unsigned char *tag, void *data, int len)
{
	if(dvbnode == NULL || dvbnode->caslot == NULL) return 0;

	debug(620, "app manager %02x %02x %02x", tag[0], tag[1], tag[2]);

	if(debug_level == 620)
	{
		int i = 0;
		printf("App manager data (len %d): > ", len);
		for(i = 0; i < len; i++)
			printf("%02x ", ((unsigned char*)data)[i]);
		printf("\n");
	}

	if(tag[0] == 0x9f && tag[1] == 0x80)
	{
		switch (tag[2])
		{
			case 0x21:
			{
				int dl;
				debug(620, "app manager info:");
				debug(620, "len: %d", len);
				debug(620, "type: %d", ((unsigned char*)data)[0]);
				debug(620, "manufacturer: %02x %02x", ((unsigned char*)data)[2], ((unsigned char*)data)[1]);
				debug(620, "code: %02x %02x", ((unsigned char*)data)[4],((unsigned char*)data)[3]);

				dl = ((unsigned char*)data)[5];
				if(dl + 6 > len)
				{
					debug(620, "invalid length (%d vs %d)", dl + 6, len);
					dl = len - 6;
				}

				char str[dl + 1];
				memcpy(str, data + 6, dl);
				str[dl] = '\0';

				if(debug_level == 620)
				{
					int i = 0;
					printf("Menu string (len %d): > ", dl);
					for(i = 0; i < dl; ++i)
						printf("%c", ((unsigned char*)data)[i + 6]);
					printf("\n");
				}

				free(dvbnode->caslot->name);
				dvbnode->caslot->name = strstrip(ostrcat(str, NULL, 0, 1));
				debug(620, "set name %s on slot %d", dvbnode->caslot->name, dvbnode->devnr);
				break;
			}
			default:
			{
				debug(620, "unknown APDU tag 9F 80 %02x\n", tag[2]);
				break;
			}
		}
	}

	return 0;
}

//session functions

//inuse: 1 is only that the session is in use
//inuse: >1 the session is used for decrypt
int getfreecasession(struct dvbdev* dvbnode, int type, int value)
{
	int i;

	if(dvbnode != NULL && dvbnode->caslot != NULL)
	{
        	for(i = 0; i < MAXCASESSION; i++)
        	{
			if(type == 0 && dvbnode->caslot->casession[i].resmanager == 1 && dvbnode->caslot->casession[i].inuse == 1) //resmanager
			{
				dvbnode->caslot->casession[i].inuse = value;
				return i;
			}
			if(type == 1 && dvbnode->caslot->casession[i].appmanager == 1 && dvbnode->caslot->casession[i].inuse == 1) //appmanager
			{
				dvbnode->caslot->casession[i].inuse = value;
				return i;
			}
			if(type == 2 && dvbnode->caslot->casession[i].camanager == 1 && dvbnode->caslot->casession[i].inuse == 1) //camanager
			{
				dvbnode->caslot->casession[i].inuse = value;
				return i;
			}
			if(type == 3 && dvbnode->caslot->casession[i].datetimemanager == 1 && dvbnode->caslot->casession[i].inuse == 1) //datetimemanager
			{
				dvbnode->caslot->casession[i].inuse = value;
				return i;
			}
			if(type == 4 && dvbnode->caslot->casession[i].mmimanager == 1 && dvbnode->caslot->casession[i].inuse == 1) //mmimemanager
			{
				dvbnode->caslot->casession[i].inuse = value;
				return i;
			}
        	}
	}
	return -1;
}

void casessiondel(struct dvbdev* dvbnode, int sessionnr)
{
	if(dvbnode != NULL && dvbnode->caslot != NULL)
	{
		free(dvbnode->caslot->casession[sessionnr].mmititle);
		free(dvbnode->caslot->casession[sessionnr].mmisubtitle);
		free(dvbnode->caslot->casession[sessionnr].mmitext);
		free(dvbnode->caslot->casession[sessionnr].mmibottom);
		memset(&dvbnode->caslot->casession[sessionnr], 0, sizeof(struct casession));
	}
}

void casessionfree(struct dvbdev* dvbnode)
{
	int i = 0;

	for(i = 0; i < MAXCASESSION; i++)
		casessiondel(dvbnode, i);
}

int casessionpoll(struct dvbdev* dvbnode)
{
	int sessionnr = 0;
	struct casession* casession = NULL;

	if(dvbnode == NULL || dvbnode->caslot == NULL) return 0;
	casession = dvbnode->caslot->casession;

	for(sessionnr = 1; sessionnr < MAXCASESSION; ++sessionnr)
	{
		if(casession[sessionnr].inuse > 0)
		{
			if(casession[sessionnr].state == CASESSIONDEL)
			{
				unsigned char data[1] = {0x00};
				sendSPDU(dvbnode, 0x96, data, 1, sessionnr, NULL, 0);
				casessiondel(dvbnode, sessionnr);
				return 1;
			}
			else if(casession[sessionnr].action == 1)
			{
				if(casession[sessionnr].resmanager == 1)
					casession[sessionnr].action = caresaction(dvbnode, sessionnr);
				else if(casession[sessionnr].appmanager == 1)
					casession[sessionnr].action = caappaction(dvbnode, sessionnr);
				else if(casession[sessionnr].camanager == 1)
					casession[sessionnr].action = cacaaction(dvbnode, sessionnr);
				else if(casession[sessionnr].datetimemanager == 1)
					casession[sessionnr].action = cadatetimeaction(dvbnode, sessionnr);
				else if(casession[sessionnr].mmimanager == 1)
					casession[sessionnr].action = cammiaction(dvbnode, sessionnr);
				return 1;
			}
		}
	}
	return 0;
}

struct casession* casessioncreate(struct dvbdev* dvbnode, unsigned char* resid, unsigned char status, unsigned long newtag)
{
	unsigned long tag = 0;
	int sessionnr = 0;
	struct casession* casession = NULL;

	if(dvbnode == NULL || dvbnode->caslot == NULL) return NULL;
	casession = dvbnode->caslot->casession;

	for(sessionnr = 1; sessionnr < MAXCASESSION; ++sessionnr)
		if(casession[sessionnr].inuse == 0)
			break;

	if(sessionnr == MAXCASESSION)
	{
		status = 0xF3;
		debug(620, "no free session found nr = %d", sessionnr);
		return NULL;
	}

	debug(620, "use sessionnr = %d", sessionnr);
	if(newtag == 0)
	{
		tag = resid[0] << 24;
		tag |= resid[1] << 16;
		tag |= resid[2] << 8;
		tag |= resid[3];
	}
	else
		tag = newtag;

	switch(tag)
	{
		case 0x00010041:
			casession[sessionnr].inuse = 1;
			casession[sessionnr].resmanager = 1;
			debug(620, "create session res manager");
			break;
		case 0x00020041:
		case 0x00020043:
			casession[sessionnr].inuse = 1;
			casession[sessionnr].appmanager = 1;
			debug(620, "create session app manager");
			break;
		case 0x00030041:
			casession[sessionnr].inuse = 1;
			casession[sessionnr].camanager = 1;
			debug(620, "create session ca manager");
			break;
		case 0x00240041:
			casession[sessionnr].inuse = 1;
			casession[sessionnr].datetimemanager = 1;
			debug(620, "create session datetime manager");
			break;
		case 0x00400041:
			casession[sessionnr].inuse = 1;
			casession[sessionnr].mmimanager = 1;
			//neutrino sessions[session_nb - 1] = new eDVBCIMMISession(slot);
			debug(620, "create session mmi manager");
			break;
		if(checkcerts())
		{
			case 0x008c1001:
				casession[sessionnr].inuse = 1;
				casession[sessionnr].ccmanager = 1;
				//neutrino [session_nb - 1] = new eDVBCIContentControlManagerSession(slot);
				debug(620, "create session cc manager");
				break;
		}
		case 0x00100041:
			debug(620, "create session auth manager");
		case 0x00200041:
		default:
			status = 0xF0;
			if(resid != NULL)
				debug(620, "unknown resource type %02x %02x %02x %02x\n", resid[0], resid[1], resid[2], resid[3]);
			return NULL;
	}

	debug(620, "new session nr: %d", sessionnr);
	casession[sessionnr].sessionnr = sessionnr;
	casession[sessionnr].state = CASESSIONCREATE;
	status = 0;

	if(newtag != 0)
	{
		unsigned char data[4];
		data[0] = (newtag >> 24) & 0xff;
		data[1] = (newtag >> 16) & 0xff;
		data[2] = (newtag >> 8) & 0xff;
		data[3] = newtag & 0xff;
		sendSPDU(dvbnode, 0x93, data, 4, sessionnr, NULL, 0);
	}

	return &casession[sessionnr];
}

void casessionreceive(struct dvbdev* dvbnode, unsigned char *buf, size_t len)
{
	unsigned char *pkt = buf;
	unsigned char tag = *pkt++;
	int llen = 0, hlen = 0;
	struct casession* casession = NULL;
	int sessionnr = 0;

	if(dvbnode == NULL || dvbnode->caslot == NULL) return;

	if(debug_level == 620)
	{
		printf("Session Data (len %d): > ", len);
		int i = 0;
		for(i = 0; i < len; i++)
			printf("%02x ", buf[i]);
		printf("\n");
	}

	llen = parselenfield(pkt, &hlen);
	pkt += llen;

	if(tag == 0x91)
	{
		unsigned char status = 0;

		casession = casessioncreate(dvbnode, pkt, status, 0);

		//open session
		unsigned char data[6];
		data[0] = status;
		memcpy(data + 1, pkt, 4);
		if(casession != NULL)
		{
			sendSPDU(dvbnode, 0x92, data, 5, casession->sessionnr, NULL, 0);
			casession->state = CASESSIONSTART;
			casession->action = 1;
		}
		else
			sendSPDU(dvbnode, 0x92, data, 5, 0, NULL, 0);
	}
	else
	{
		sessionnr = pkt[hlen - 2] << 8;
		sessionnr |= pkt[hlen - 1] & 0xFF;
		debug(620, "tag = %x, hlen = %d, session = %d", tag, hlen, sessionnr);

		if(sessionnr < 1 || sessionnr >= MAXCASESSION)
		{
			debug(620, "illegal session number %d", sessionnr);
			return;
		}

		casession = &dvbnode->caslot->casession[sessionnr];
		if(casession->inuse == 0)
		{
			debug(620, "data on closed session %d", sessionnr);
			return;
		}

		switch(tag)
		{
			case 0x90:
				break;
			case 0x94:
				debug(620, "recv create session response %02x", pkt[0]);
				if(pkt[0] != 0)
					casessiondel(dvbnode, sessionnr);
				else
				{
					casession->state = CASESSIONSTART;
					casession->action = 1;
				}
				break;
			case 0x95:
				debug(620, "recv close session");
				casession->state = CASESSIONDEL;
				casession->action = 1;
				break;
			default:
				debug(620, "not supported tag %02x", tag);
				return;
		}
	}

	hlen += llen + 1; // lengthfield and tag

	pkt = buf + hlen;
	len -= hlen;

	if(casession != NULL)
	{
		debug(620, "len %d", len);
		while(len > 0)
		{
			int alen = 0;
			unsigned char *tag = pkt;
			pkt += 3; //tag
			len -= 3;
			hlen = parselenfield(pkt, &alen);
			pkt += hlen;
			len -= hlen;
			debug(620, "len = %d, hlen = %d, alen = %d", len, hlen, alen);

			if(len - alen > 0 && len - alen < 3)
				alen = len;

			debug(620, "got APDU tag = 0x%2x, len = %d", (int)tag, alen);

			if(casession->resmanager == 1)
			{
				if(caresAPDU(dvbnode, sessionnr, tag, pkt, alen))
					casession->action = 1;
			}
			else if(casession->appmanager == 1)
			{
				if(caappAPDU(dvbnode, sessionnr, tag, pkt, alen))
					casession->action = 1;
			}
			else if(casession->ccmanager == 1)
			{
				if(caccAPDU(dvbnode, sessionnr, tag, pkt, alen))
					casession->action = 1;
			}
			else if(casession->camanager == 1)
			{
				if(cacaAPDU(dvbnode, sessionnr, tag, pkt, alen))
					casession->action = 1;
			}
			else if(casession->datetimemanager == 1)
			{
				if(cadatetimeAPDU(dvbnode, sessionnr, tag, pkt, alen))
					casession->action = 1;
			}
			else if(casession->mmimanager == 1)
			{
				if(cammiAPDU(dvbnode, sessionnr, tag, pkt, alen))
					casession->action = 1;
			}

			pkt += alen;
			len -= alen;
		}

	}

	if(len != 0)
		debug(620, "warning, TL-Data has invalid length");
}

//slot function

//send a transport connection create request
int cacreatetc(struct dvbdev* dvbnode)
{
	int ret = 0;
	unsigned char* buf = NULL;

	if(dvbnode == NULL || dvbnode->caslot == NULL ) return 1;

	buf = (unsigned char*) malloc(sizeof(char) * 5);
	if(buf == NULL)
	{
		err("no mem");
		return 1;
	}

	buf[0] = dvbnode->devnr;
	buf[1] = dvbnode->caslot->connid;
	buf[2] = T_CREATE_T_C;
	buf[3] = 1;
	buf[4] = dvbnode->caslot->connid;

	ret = cawrite(dvbnode, dvbnode->fd, buf, 5, 0, -1);
	free(buf); buf = NULL;
	dvbnode->caslot->poll = 0;
	if(checkbox("DM7020HD") == 1 || checkbox("DM7020HDV2") == 1)
		sleep(1);
	return ret;
}

void caslotfree(struct dvbdev* dvbnode)
{
	if(dvbnode != NULL && dvbnode->caslot != NULL)
	{
		free(dvbnode->caslot->name);
		dvbnode->caslot->name = NULL;
		free(dvbnode->caslot->rbuf);
		dvbnode->caslot->rbuf = NULL;
		free(dvbnode->caslot->caids);
		dvbnode->caslot->caids = NULL;
		memset(dvbnode->caslot, 0, sizeof(struct caslot));
	}
	if(dvbnode != NULL) caservicedel(NULL, dvbnode->caslot);
}

void processtpdu(struct dvbdev* dvbnode, unsigned char tpdutag, unsigned char* buf, int asnlen)
{
	struct caslot* canode = NULL;

	if(dvbdev == NULL) return;
	canode = dvbnode->caslot;

	switch(tpdutag)
	{
		case T_C_T_C_REPLY:
			debug(620, "got ctc replay (slot %d, conn %d)", dvbnode->devnr, canode->connid);

			casend(dvbnode, NULL, 0);

			break;
		case T_DELETE_T_C:
			debug(620, "got delete transport connection (slot %d, conn %d)", dvbnode->devnr, canode->connid);

			//send the D_T_C_REPLAY
			unsigned char sendbuf[5];

			sendbuf[0] = dvbnode->devnr;
			sendbuf[1] = canode->connid;
			sendbuf[2] = T_D_T_C_REPLY;
			sendbuf[3] = 1;
			sendbuf[4] = canode->connid;

			cawrite(dvbnode, dvbnode->fd, sendbuf, 5, 0, -1);
			casessionfree(dvbnode);
			caslotfree(dvbnode);

			break;
		case T_D_T_C_REPLY:

			debug(620, "got delete transport connection replay (slot %d, conn %d)", dvbnode->devnr, canode->connid);

			casessionfree(dvbnode);
			caslotfree(dvbnode);
			break;

		case T_REQUEST_T_C:

			debug(620, "got request transport connection (slot %d, conn %d)", dvbnode->devnr, canode->connid);

			break;
		case T_DATA_MORE:
		{
			int newbuflen = canode->rlen + asnlen;

			debug(620, "got data more (slot %d, conn %d)", dvbnode->devnr, canode->connid);

			canode->rbuf = realloc(canode->rbuf, newbuflen);
			if(canode->rbuf != NULL)
			{
				memcpy(canode->rbuf + canode->rlen, buf, asnlen);
				canode->rlen = newbuflen;
			}

			break;
		}
		case T_DATA_LAST:
		{
			if(canode->rbuf == NULL)
			{
				debug(620, "got data last single package (slot %d, conn %d)", dvbnode->devnr, canode->connid);

				casessionreceive(dvbnode, buf, asnlen);
				casessionpoll(dvbnode);
			}
			else
			{
				debug(620, "got data last chained package (slot %d, conn %d)", dvbnode->devnr, canode->connid);
				int newbuflen = canode->rlen + asnlen;

				canode->rbuf = realloc(canode->rbuf, newbuflen);
				if(canode->rbuf != NULL)
				{
					memcpy(canode->rbuf + canode->rlen, buf, asnlen);
					canode->rlen = newbuflen;
					casessionreceive(dvbnode, canode->rbuf, canode->rlen);
					casessionpoll(dvbnode);
				}

				free(canode->rbuf);
				canode->rbuf = NULL;
				canode->rlen = 0;
			}
			break;
		}
		case T_SB:
		{
			//debug(620, "got sb (slot %d, conn %d, buf %02x)", dvbnode->devnr, canode->connid, buf[0]);

			if(buf[0] & 0x80)
			{
				debug(620, "data ready (slot %d)", dvbnode->devnr);

				//send the RCV and ask for the data
				unsigned char sendbuf[5];

				sendbuf[0] = dvbnode->devnr;
				sendbuf[1] = canode->connid;
				sendbuf[2] = T_RCV;
				sendbuf[3] = 1;
				sendbuf[4] = canode->connid;

				cawrite(dvbnode, dvbnode->fd, sendbuf, 5, 0, -1);
			}
			break;
		}
		case 0x13: //MIPSEL
		{
			debug(620, "got mipsel session data (slot %d, conn %d)", dvbnode->devnr, canode->connid);
			casessionreceive(dvbnode, buf, asnlen);
			casessionpoll(dvbnode);
			break;
		}
		default:
			debug(620, "unknown tpdu tag 0x%0x (slot %d, conn %d)", tpdutag, dvbnode->devnr, canode->connid);
	}
}

void cacheck(struct stimerthread* self, struct dvbdev* dvbnode)
{
	int ret = 0, len = MINMALLOC;
	ca_slot_info_t info;
	struct caslot* canode = NULL;
	unsigned char* buf = NULL;
#ifdef SH4
	unsigned char* tmpbuf = NULL;
#endif
#ifdef SIMULATE
	unsigned char* tmpbuf = NULL;
#endif
	if(dvbnode == NULL) return;
	canode = dvbnode->caslot;

	buf = calloc(1, MINMALLOC);
	if(buf == NULL)
	{
		err("no mem");
		return;
	}

	switch(canode->status)
	{
		case 0: //idle
		case 100: //manuell reset
		{
			if(canode->status == 0)
				cawait(self, dvbnode, -1);
			canode->status = 0;
			debug(620, "status: no, slot %d", dvbnode->devnr);
			if(dvbnode->caslot->status == 101) break; //titan end

			//reset the module an wait max 10 sek
			careseting(self, dvbnode, 0);

			info.num = dvbnode->devnr;
			if(cagetslotinfo(dvbnode, &info) == 0)
			{
				debug(620, "flags %d %d %d, slot %d", info.flags, CA_CI_MODULE_READY, info.flags & CA_CI_MODULE_READY, dvbnode->devnr);

				if(info.flags & CA_CI_MODULE_READY)
				{
					debug(620, "cam (slot %d) status changed, cam now present", dvbnode->devnr);
					canode->connid = dvbnode->devnr + 1;
					if(cacreatetc(dvbnode) == 5)
					{
						casessionfree(dvbnode);
						caslotfree(dvbnode);
						canode->status = 1;
						canode->connid = dvbnode->devnr + 1;
						dvbnode->caslot->fastrun = getconfigint("camwait", NULL);
					}
					else
					{
						canode->connid = 0;
						sleep(1);
					}
				}
			}
			else
				sleep(1);
		}
		break;
		case 1: //active
		case 2: //active (ca and app ready)
		{
			//debug(620, "status: wait, slot %d", dvbnode->devnr);
			ret = caread(dvbnode, buf, &len);
			if(dvbnode->caslot->status == 101) break; //titan end
			if(ret == 0) //ready
			{
				//debug(620, "read, slot %d, ret %d", dvbnode->devnr, ret);
#ifdef MIPSEL
				canode->poll = 1;
				canode->connid = dvbnode->devnr + 1;
				processtpdu(dvbnode, 0x13, buf, len);
#else
				tmpbuf = buf;
				int buflen = len - 2;
				tmpbuf += 2; //remove leading slot and connection id
				while(buflen > 0)
				{
					unsigned char tpdutag = tmpbuf[0];
					unsigned short asnlen;
					int lenfieldlen;

					canode->poll = 1;
					lenfieldlen = asn1decode(&asnlen, tmpbuf + 1, buflen - 1);

					if(lenfieldlen < 0 || asnlen < 1 || asnlen > (buflen - (1 + lenfieldlen)))
					{
						err("received data with invalid asn from module on slot %d", dvbnode->devnr);
						break;
					}

					canode->connid = tmpbuf[1 + lenfieldlen];
					tmpbuf += 1 + lenfieldlen + 1;
					buflen -= (1 + lenfieldlen + 1);
					asnlen--;

					processtpdu(dvbnode, tpdutag, tmpbuf, asnlen);

					//skip over the consumed data
					tmpbuf += asnlen;
					buflen -= asnlen;
				}
#endif
			}
			else if(ret == 1 && canode->poll == 1) //write
			{
#ifdef MIPSEL
				int count = 0;
				struct queue* qe = getqueue(dvbnode->devnr);
				if(qe != NULL)
				{
					while(qe != NULL && count < 10)
					{
						//debug(620, "write (queue), slot %d, ret %d", dvbnode->devnr, ret);
						
						int writeret = cawrite(dvbnode, dvbnode->fd, qe->data, qe->len, qe->flag, -1);
						if(writeret >= 0 && writeret == qe->len)
						{
							delqueue(qe, 0);
							qe = getqueue(dvbnode->devnr);
						}
						else
							count++;
					}
				}
				else
				{
					//debug(620, "write (poll), slot %d, ret %d", dvbnode->devnr, ret);
					casend(dvbnode, NULL, 0);
					if(canode->fastrun > 0) canode->fastrun--;
				}
#else			
				struct queue* qe = getqueue(dvbnode->devnr);
				if(qe != NULL)
				{
					//debug(620, "write (queue), slot %d, ret %d", dvbnode->devnr, ret);
					
					int writeret = cawrite(dvbnode, dvbnode->fd, qe->data, qe->len, qe->flag, -1);
					if(writeret >= 0 && writeret == qe->len)
					{
						delqueue(qe, 0);
					}
				}
				else
				{
					//debug(620, "write (poll), slot %d, ret %d", dvbnode->devnr, ret);
					casend(dvbnode, NULL, 0);
					if(canode->fastrun > 0) canode->fastrun--;
				}
#endif
			}
			else if(ret == 1 && canode->poll == 0)
			{
				debug(620, "write but poll=0, slot %d, ret %d", dvbnode->devnr, ret);
				if(canode->fastrun > 0) canode->fastrun--;
			}
			else if(ret == 2) //change
			{
				debug(620, "change, slot %d, ret %d", dvbnode->devnr, ret);
				info.num = dvbnode->devnr;

				if(cagetslotinfo(dvbnode, &info) == 1 || !(info.flags & CA_CI_MODULE_READY))
				{
					debug(620, "flags %d %d %d, slot %d", info.flags, CA_CI_MODULE_READY, info.flags & CA_CI_MODULE_READY, dvbnode->devnr);
					debug(620, "cam (slot %d) status changed, cam not present", dvbnode->devnr);

					//reset routing
					debug(620, "reset routing tuner %d", dvbnode->devnr);
					switch(dvbnode->devnr)
					{
						case 0: setcisource(dvbnode->devnr, "A"); break;
						case 1: setcisource(dvbnode->devnr, "B"); break;
						case 2: setcisource(dvbnode->devnr, "C"); break;
						case 3: setcisource(dvbnode->devnr, "D"); break;
					}

					casessionfree(dvbnode);
					caslotfree(dvbnode);
					canode->fastrun = 0;

					sleep(1);
				}
			}
			else if(ret == -2) //timeout
			{
				debug(620, "timeout slot %d, ret %d", dvbnode->devnr, ret);
				if(canode->fastrun > 0) canode->fastrun--;
				break;
			}
			else if(ret < 0) //error
			{
				debug(620, "error slot %d, ret %d", dvbnode->devnr, ret);
				casessionfree(dvbnode);
				caslotfree(dvbnode);
				sleep(1);
			}
		}
		break;
		default:
		{
			debug(620, "unknown status");
		}
		break;
	}

	//declear as ready, but not complete
	if(canode->caids != NULL && canode->status == 1 && canode->fastrun < 1)
	{
		if(getfreecasession(dvbnode, 1, 1) > -1 && getfreecasession(dvbnode, 2, 1) > -1)
		{
			canode->status = 2;
			//m_lock(&status.servicemutex, 2);
			//sendcapmt(status.aktservice, 0, 2);
			//m_unlock(&status.servicemutex, 2);
		}
	}

	free(buf); buf = NULL;
}

void cathread(struct stimerthread* self, struct dvbdev* dvbnode)
{
	if(dvbnode == NULL || dvbnode->caslot == NULL) return;
	debug(620, "CA thread start (slot %d)", dvbnode->devnr);
//#ifdef MIPSEL
	dvbnode->caslot->status = 100;
//#endif
	while(self != NULL && self->aktion != STOP && self->aktion != PAUSE)
	{
		if(dvbnode->caslot->status != 101)
			cacheck(self, dvbnode);
		if(dvbnode->caslot->fastrun == 0)
			sleep(1);
		else
			usleep(100000);
	}
	debug(620, "CA thread end (slot %d)", dvbnode->devnr);
}

void castart()
{
	struct dvbdev* dvbnode = dvbdev;

	while(dvbnode != NULL)
	{
		if(dvbnode->type == CIDEV && dvbnode->fd > -1)
		{
			if(debug_level == 620)
			{
				struct ca_caps caps;
				ca_slot_info_t info;

				caps.slot_num = dvbnode->devnr;
				if(cagetcaps(dvbnode, &caps) == 0)
					debug(620, "caps: slotsum:%d, type:%d, descrnr:%d, descrtype:%d", caps.slot_num, caps.slot_type, caps.descr_num, caps.descr_type);

				info.num = dvbnode->devnr;
				if(cagetslotinfo(dvbnode, &info) == 0)
					debug(620, "info: slot:%d, type:%d, flag:%d", info.num, info.type, info.flags);
			}
			addtimer(&cathread, START, 1000, -1, (void*)dvbnode, NULL, NULL);
		}
		dvbnode = dvbnode->next;
	}
}

int sendcapmttocam(struct dvbdev* dvbnode, struct service* node, unsigned char* buf, int len, int caservicenr, int cmdpos, int clear)
{
	int i = 0;
	char* tmpstr = NULL, *blacklist = NULL;

	if(node == NULL && dvbnode == NULL) return 1;

	if(dvbnode->type == CIDEV && dvbnode->fd > -1 && dvbnode->caslot != NULL && dvbnode->caslot->status == 2 && dvbnode->caslot->caids != NULL)
	{
		//check if crypt can onyl handle single service
		tmpstr = ostrcat("camtype_", oitoa(dvbnode->devnr), 0, 1);
		if(clear == 0 && getconfigint(tmpstr, NULL) == 0 && getcaservicebyslot(dvbnode->caslot, 1) > -1)
		{
			debug(620, "cam is singel and is in use");
			free(tmpstr); tmpstr = NULL;
			return 1;
		}
		free(tmpstr); tmpstr = NULL;
		
		int foundcaid = 0;
		
    //check if channel used this slot
		if(node->channel != NULL)
		{
			struct channelslot *channelslotnode = channelslot; 
			while(channelslotnode != NULL)
			{
				if(channelslotnode->transponderid == node->channel->transponderid && channelslotnode->serviceid == node->channel->serviceid)
				{
          if(channelslotnode->slot == dvbnode->devnr)
          {
						debug(620, "channel support cam (channelslot=%d, slot=%d)", channelslotnode->slot, dvbnode->devnr);
						foundcaid = 1;
						break;
          }
          else
          {
          	debug(620, "channel not support cam (channelslot=%d, slot=%d)", channelslotnode->slot, dvbnode->devnr);
						return 1;
					}
				}			
				channelslotnode = channelslotnode->next;			
			}
		}

		//check if cam can caid
		if(foundcaid == 0)
		{
			tmpstr = ostrcat("camblacklist_", oitoa(dvbnode->devnr), 0, 1);
			blacklist = getconfig(tmpstr, NULL);
			free(tmpstr); tmpstr = NULL;
			if(node->channel != NULL)
			{
				struct cadesc *nodecadesc = node->channel->cadesc;
				while(nodecadesc != NULL)
				{
					debug(620, "cam-ciads=%s", dvbnode->caslot->caids);
					debug(620, "videopid=%d, audiopid=%d, ac3pid=%d, capid=%d, caid=%d", node->channel->videopid, node->channel->audiopid, node->channel->ac3audiopid, nodecadesc->pid, nodecadesc->systemid);
					tmpstr = oitoa(nodecadesc->systemid);
					if(ostrstr(dvbnode->caslot->caids, tmpstr) != NULL)
					{
						//check if caid is in cams blacklist
						if(blacklist == NULL || ostrstr(blacklist, tmpstr) == NULL)
						{
							foundcaid = 1;
							break;
						}
						else
							debug(620, "caid is in blacklist (%s -> %s)", tmpstr, blacklist);
					}
					free(tmpstr); tmpstr = NULL;
	    		nodecadesc = nodecadesc->next;
				}
			}
			free(tmpstr); tmpstr = NULL;
		}

		if(foundcaid == 0)
		{
			debug(620, "cam not supports caid");
			return 1;
		}

		//check if we can change input sources
		for(i = 0; i < MAXCASERVICE; i++)
		{
			if(caservice[i].caslot != NULL && caservice[i].service != NULL && caservice[i].service->fedev != NULL && node->fedev != NULL)
			{
				if(caservice[i].caslot == dvbnode->caslot && caservice[i].service->fedev->devnr != node->fedev->devnr && (caservice[i].service->type == RECORDDIRECT || caservice[i].service->type == RECORDTIMER))
				{
					debug(620, "can't change input sources");
					return 1;
				}
			}
		}

		//got free camanager
		if(caservice[caservicenr].camanager == -1)
		{
			caservice[caservicenr].camanager = getfreecasession(dvbnode, 2, 1);
			debug(620, "use camanager %d", caservice[caservicenr].camanager);
		}

		//check if cam can decrypt
		if(clear == 0 && caservice[caservicenr].camanager > -1 && getconfigint("checkcamdecrypt", NULL) == 1)
		{
			if(buf != NULL && len >= cmdpos)
			{
				status.checkcamdecrypt = 100;
				buf[cmdpos] = 0x03;
				buf[cmdpos - 6] = 0x04; //WA for alphacrypt, only delete all ca-pmt
				sendSPDU(dvbnode, 0x90, NULL, 0, caservice[caservicenr].camanager, buf, len);
				buf[cmdpos] = 0x01;
				buf[cmdpos - 6] = 0x03;
				while(status.checkcamdecrypt > 0)
				{
					status.checkcamdecrypt--;
					usleep(30000);
				}
    		if(status.checkcamdecrypt == -2)
				{
					dvbnode->caslot->casession[caservice[caservicenr].camanager].inuse = 1;
					caservice[caservicenr].camanager = -1;
				}
			}
		}

		//decrypt
		if(caservice[caservicenr].camanager > -1)
		{
			//change ciX_input and inputX
			if(clear == 0 && node->fedev != NULL)
			{
				char* ci = NULL;
				debug(620, "set ci slot %d to tuner %d", dvbnode->devnr, node->fedev->devnr);
				switch(node->fedev->devnr)
				{
					case 0:
						setciinput(dvbnode->devnr, "A");
						ci = ostrcat("CI", oitoa(dvbnode->devnr), 0, 1);
						setcisource(node->fedev->devnr, ci);
						free(ci); ci = NULL;
						break;
					case 1:
						setciinput(dvbnode->devnr, "B");
						ci = ostrcat("CI", oitoa(dvbnode->devnr), 0, 1);
						setcisource(node->fedev->devnr, ci);
						free(ci); ci = NULL;
						break;
					case 2:
						setciinput(dvbnode->devnr, "C");
						ci = ostrcat("CI", oitoa(dvbnode->devnr), 0, 1);
						setcisource(node->fedev->devnr, ci);
						free(ci); ci = NULL;
						break;
					case 3:
						setciinput(dvbnode->devnr, "D");
						ci = ostrcat("CI", oitoa(dvbnode->devnr), 0, 1);
						setcisource(node->fedev->devnr, ci);
						free(ci); ci = NULL;
						break;
				}
			}
			//send all saved capmt first
			int first = 0;
			for(i = 0; i < MAXCASERVICE; i++)
			{
				if(dvbnode->caslot == caservice[i].caslot && caservice[i].capmt != NULL && caservice[i].capmtlen > 0)
				{
					if(i == 0)
						caservice[i].capmt[caservice[i].cmdpos - 6] = 0x01;
					else
						caservice[i].capmt[caservice[i].cmdpos - 6] = 0x00;
					first = 1;
					sendSPDU(dvbnode, 0x90, NULL, 0, caservice[i].camanager, caservice[i].capmt, caservice[i].capmtlen);
				}
			}
			//send new capmt
			if(first == 1) buf[cmdpos - 6] = 0x02;
			sendSPDU(dvbnode, 0x90, NULL, 0, caservice[caservicenr].camanager, buf, len);
			caservice[caservicenr].caslot = dvbnode->caslot;
			//save new capmt
			char* tmpbuf = malloc(len);
			if(tmpbuf != NULL)
			{
				memcpy(tmpbuf, buf, len);
				free(caservice[caservicenr].capmt);
				caservice[caservicenr].capmt = tmpbuf;
				caservice[caservicenr].capmtlen = len;
				caservice[caservicenr].cmdpos = cmdpos;
			}
			debug(620, "found cam for decrypt (slot=%d)", dvbnode->devnr);
			return 0;
		}
		else
			debug(620, "no free camanager found");
	}
	return 1;
}

#ifdef SH4

unsigned char dh_p[256] = {       /* prime */
	0xd6, 0x27, 0x14, 0x7a, 0x7c, 0x0c, 0x26, 0x63, 0x9d, 0x82, 0xeb, 0x1f, 0x4a, 0x18, 0xff, 0x6c,
	0x34, 0xad, 0xea, 0xa6, 0xc0, 0x23, 0xe6, 0x65, 0xfc, 0x8e, 0x32, 0xc3, 0x33, 0xf4, 0x91, 0xa7,
	0xcc, 0x88, 0x58, 0xd7, 0xf3, 0xb3, 0x17, 0x5e, 0xb0, 0xa8, 0xeb, 0x5c, 0xd4, 0xd8, 0x3a, 0xae,
	0x8e, 0x75, 0xa1, 0x50, 0x5f, 0x5d, 0x67, 0xc5, 0x40, 0xf4, 0xb3, 0x68, 0x35, 0xd1, 0x3a, 0x4c,
	0x93, 0x7f, 0xca, 0xce, 0xdd, 0x83, 0x29, 0x01, 0xc8, 0x4b, 0x76, 0x81, 0x56, 0x34, 0x83, 0x31,
	0x92, 0x72, 0x65, 0x7b, 0xac, 0xd9, 0xda, 0xa9, 0xd1, 0xd3, 0xe5, 0x77, 0x58, 0x6f, 0x5b, 0x44,
	0x3e, 0xaf, 0x7f, 0x6d, 0xf5, 0xcf, 0x0a, 0x80, 0x0d, 0xa5, 0x56, 0x4f, 0x4b, 0x85, 0x41, 0x0f,
	0x13, 0x41, 0x06, 0x1f, 0xf3, 0xd9, 0x65, 0x36, 0xae, 0x47, 0x41, 0x1f, 0x1f, 0xe0, 0xde, 0x69,
	0xe5, 0x86, 0x2a, 0xa1, 0xf2, 0x48, 0x02, 0x92, 0x68, 0xa6, 0x37, 0x9f, 0x76, 0x4f, 0x7d, 0x94,
	0x5d, 0x10, 0xe5, 0xab, 0x5d, 0xb2, 0xf3, 0x12, 0x8c, 0x79, 0x03, 0x92, 0xa6, 0x7f, 0x8a, 0x78,
	0xb0, 0xba, 0xc5, 0xb5, 0x31, 0xc5, 0xc8, 0x22, 0x6e, 0x29, 0x02, 0x40, 0xab, 0xe7, 0x5c, 0x23,
	0x33, 0x7f, 0xcb, 0x86, 0xc7, 0xb4, 0xfd, 0xaa, 0x44, 0xcd, 0x9c, 0x9f, 0xba, 0xac, 0x3a, 0xcf,
	0x7e, 0x31, 0x5f, 0xa8, 0x47, 0xce, 0xca, 0x1c, 0xb4, 0x77, 0xa0, 0xec, 0x9a, 0x46, 0xd4, 0x79,
	0x7b, 0x64, 0xbb, 0x6c, 0x91, 0xb2, 0x38, 0x01, 0x65, 0x11, 0x45, 0x9f, 0x62, 0x08, 0x6f, 0x31,
	0xcf, 0xc4, 0xba, 0xdc, 0xd0, 0x03, 0x91, 0xf1, 0x18, 0x1f, 0xcb, 0x4d, 0xfc, 0x73, 0x5a, 0xa2,
	0x15, 0xb8, 0x3c, 0x8d, 0x80, 0x92, 0x1c, 0xa1, 0x03, 0xd0, 0x83, 0x2f, 0x5f, 0xe3, 0x07, 0x69
};

unsigned char dh_g[256] = {       /* generator */
	0x95, 0x7d, 0xd1, 0x49, 0x68, 0xc1, 0xa5, 0xf1, 0x48, 0xe6, 0x50, 0x4f, 0xa1, 0x10, 0x72, 0xc4,
	0xef, 0x12, 0xec, 0x2d, 0x94, 0xbe, 0xc7, 0x20, 0x2c, 0x94, 0xf9, 0x68, 0x67, 0x0e, 0x22, 0x17,
	0xb5, 0x5c, 0x0b, 0xca, 0xac, 0x9f, 0x25, 0x9c, 0xd2, 0xa6, 0x1a, 0x20, 0x10, 0x16, 0x6a, 0x42,
	0x27, 0x83, 0x47, 0x42, 0xa0, 0x07, 0x52, 0x09, 0x33, 0x97, 0x4e, 0x30, 0x57, 0xd8, 0xb7, 0x1e,
	0x46, 0xa6, 0xba, 0x4e, 0x40, 0x6a, 0xe9, 0x1a, 0x5a, 0xa0, 0x74, 0x56, 0x92, 0x55, 0xc2, 0xbd,
	0x44, 0xcd, 0xb3, 0x33, 0xf7, 0x35, 0x46, 0x25, 0xdf, 0x84, 0x19, 0xf3, 0xe2, 0x7a, 0xac, 0x4e,
	0xee, 0x1a, 0x86, 0x3b, 0xb3, 0x87, 0xa6, 0x66, 0xc1, 0x70, 0x21, 0x41, 0xd3, 0x58, 0x36, 0xb5,
	0x3b, 0x6e, 0xa1, 0x55, 0x60, 0x9a, 0x59, 0xd3, 0x85, 0xd8, 0xdc, 0x6a, 0xff, 0x41, 0xb6, 0xbf,
	0x42, 0xde, 0x64, 0x00, 0xd0, 0xee, 0x3a, 0xa1, 0x8a, 0xed, 0x12, 0xf9, 0xba, 0x54, 0x5c, 0xdb,
	0x06, 0x24, 0x49, 0xe8, 0x47, 0xcf, 0x5b, 0xe4, 0xbb, 0xc0, 0xaa, 0x8a, 0x8c, 0xbe, 0x73, 0xd9,
	0x02, 0xea, 0xee, 0x8d, 0x87, 0x5b, 0xbf, 0x78, 0x04, 0x41, 0x9e, 0xa8, 0x5c, 0x3c, 0x49, 0xde,
	0x88, 0x6d, 0x62, 0x21, 0x7f, 0xf0, 0x5e, 0x2d, 0x1d, 0xfc, 0x47, 0x0d, 0x1b, 0xaa, 0x4e, 0x0d,
	0x78, 0x20, 0xfe, 0x57, 0x0f, 0xca, 0xdf, 0xeb, 0x3c, 0x84, 0xa7, 0xe1, 0x61, 0xb2, 0x95, 0x98,
	0x07, 0x73, 0x8e, 0x51, 0xc6, 0x87, 0xe4, 0xcf, 0xf1, 0x5f, 0x86, 0x99, 0xec, 0x8d, 0x44, 0x92,
	0x2c, 0x99, 0xf6, 0xc0, 0xf4, 0x39, 0xe8, 0x05, 0xbf, 0xc1, 0x56, 0xde, 0xfe, 0x93, 0x75, 0x06,
	0x69, 0x87, 0x83, 0x06, 0x51, 0x80, 0xa5, 0x6e, 0xa6, 0x19, 0x7d, 0x3b, 0xef, 0xfb, 0xe0, 0x4a
};


int aes_xcbc_mac_init(struct aes_xcbc_mac_ctx *ctx, const uint8_t *key)
{
	AES_KEY aes_key;
	int y, x;

	AES_set_encrypt_key(key, 128, &aes_key);

	for (y = 0; y < 3; y++) {
		for (x = 0; x < 16; x++)
			ctx->K[y][x] = y + 1;
		AES_ecb_encrypt(ctx->K[y], ctx->K[y], &aes_key, 1);
	}

	/* setup K1 */
	AES_set_encrypt_key(ctx->K[0], 128, &ctx->key);

	memset(ctx->IV, 0, 16);
	ctx->buflen = 0;

	return 0;
}

int aes_xcbc_mac_process(struct aes_xcbc_mac_ctx *ctx, const uint8_t *in, unsigned int len)
{
	while (len) {
		if (ctx->buflen == 16) {
			AES_ecb_encrypt(ctx->IV, ctx->IV, &ctx->key, 1);
			ctx->buflen = 0;
		}
		ctx->IV[ctx->buflen++] ^= *in++;
		--len;
	}

	return 0;
}

int aes_xcbc_mac_done(struct aes_xcbc_mac_ctx *ctx, uint8_t *out)
{
	int i;

	if (ctx->buflen == 16) {
		/* K2 */
		for (i = 0; i < 16; i++)
			ctx->IV[i] ^= ctx->K[1][i];
	} else {
		ctx->IV[ctx->buflen] ^= 0x80;
		/* K3 */
		for (i = 0; i < 16; i++)
			ctx->IV[i] ^= ctx->K[2][i];
	}

	AES_ecb_encrypt(ctx->IV, ctx->IV, &ctx->key, 1);
	memcpy(out, ctx->IV, 16);

	return 0;
}

static const char * FILENAME = "[descrambler]";

static const char *descrambler_filename = "/dev/dvb/adapter0/ca3";
static int desc_fd = -1;
static int desc_user_count = 0;

/* Byte 0 to 15 are AES Key, Byte 16 to 31 are IV */

int descrambler_set_key(int index, int parity, unsigned char *data)
{
	struct ca_descr_data d;

	printf("%s -> %s\n", FILENAME, __FUNCTION__);

	index |= 0x100;

	if (descrambler_open())
	{
		d.index = index;
// old
//		d.parity = (ca_descr_parity)parity;
// new start
		enum ca_descr_parity* e = NULL;
		d.parity = (e)[parity];
// new end

		d.data_type = CA_DATA_KEY;
		d.length = 32;
		d.data = data;

		printf("Index: %d Parity: (%d) -> ", d.index, d.parity);
		hexdump(d.data, 32);

		if (ioctl(desc_fd, CA_SET_DESCR_DATA, &d))
		{
			//printf("CA_SET_DESCR_DATA\n");
		}
		descrambler_close();
	}
	return 0;
}

/* we don't use this for ci cam ! */
/*
int descrambler_set_pid(int index, int enable, int pid)
{
	struct ca_pid p;
	if (enable)
		p.index = index;
	else
		p.index = -1;

	p.pid = pid;

	if (ioctl(desc_fd, CA_SET_PID, &p))
		printf("CA_SET_PID\n");

	return 0;
}
*/

//bool descrambler_open(void)
int descrambler_open(void)
{
	desc_fd = open(descrambler_filename, O_RDWR | O_NONBLOCK );
	if (desc_fd <= 0) {
		printf("cannot open %s\n", descrambler_filename);
//		return false;
		return 0;
	}
//	return true;
	return 1;
}

void descrambler_close(void)
{
	close(desc_fd);
	desc_fd = -1;
}

int descrambler_init(void)
{
	desc_user_count++;
	printf("%s -> %s %d\n", FILENAME, __FUNCTION__, desc_user_count);
	return 0;
}

void descrambler_deinit(void)
{
	desc_user_count--;
	if (desc_user_count <= 0 && desc_fd > 0)
		descrambler_close();
}

static int pkcs_1_mgf1(const uint8_t *seed, unsigned long seedlen, uint8_t *mask, unsigned long masklen)
{
	unsigned long hLen, x;
	uint32_t counter;
	uint8_t *buf;

	/* get hash output size */
	hLen = 20;      /* SHA1 */

	/* allocate memory */
	buf = (uint8_t*)malloc(hLen);
	if (buf == NULL) {
		printf("error mem\n");
		return -1;
	}

	/* start counter */
	counter = 0;

	while (masklen > 0) {
		/* handle counter */
		BYTE32(buf, counter);
		++counter;

		/* get hash of seed || counter */
		unsigned char buffer[0x18];
		memcpy(buffer, seed, seedlen);
		memcpy(buffer + 0x14, buf, 4);
		SHA1(buffer, 0x18, buf);

		/* store it */
		for (x = 0; x < hLen && masklen > 0; x++, masklen--)
			*mask++ = buf[x];
	}

	free(buf);
	return 0;
}

static int pkcs_1_pss_encode(const uint8_t *msghash, unsigned int msghashlen,
			     unsigned long saltlen, unsigned long modulus_bitlen,
			     uint8_t *out, unsigned int outlen)
{
	unsigned char *DB, *mask, *salt, *hash;
	unsigned long x, y, hLen, modulus_len;
	int err = -1;
	unsigned char *hashbuf;
	unsigned int hashbuflen;

	hLen = 20;      /* SHA1 */
	modulus_len = (modulus_bitlen >> 3) + (modulus_bitlen & 7 ? 1 : 0);

	/* allocate ram for DB/mask/salt/hash of size modulus_len */
	DB = (unsigned char*)malloc(modulus_len);
	mask = (unsigned char*)malloc(modulus_len);
	salt = (unsigned char*)malloc(modulus_len);
	hash = (unsigned char*)malloc(modulus_len);

	hashbuflen = 8 + msghashlen + saltlen;
	hashbuf = (unsigned char*)malloc(hashbuflen);

	if (!(DB && mask && salt && hash && hashbuf)) {
		printf("out of memory\n");
		goto LBL_ERR;
	}

	/* generate random salt */
	if (saltlen > 0) {
		if (get_random(salt, saltlen) != (long)saltlen) {
			printf("rnd failed\n");
			goto LBL_ERR;
		}
	}

	/* M = (eight) 0x00 || msghash || salt, hash = H(M) */
	memset(hashbuf, 0, 8);
	memcpy(hashbuf + 8, msghash, msghashlen);
	memcpy(hashbuf + 8 + msghashlen, salt, saltlen);
	SHA1(hashbuf, hashbuflen, hash);

	/* generate DB = PS || 0x01 || salt, PS == modulus_len - saltlen - hLen - 2 zero bytes */
	x = 0;
	memset(DB + x, 0, modulus_len - saltlen - hLen - 2);
	x += modulus_len - saltlen - hLen - 2;
	DB[x++] = 0x01;
	memcpy(DB + x, salt, saltlen);
	x += saltlen;

	err = pkcs_1_mgf1(hash, hLen, mask, modulus_len - hLen - 1);
	if (err)
		goto LBL_ERR;

	/* xor against DB */
	for (y = 0; y < (modulus_len - hLen - 1); y++)
		DB[y] ^= mask[y];

	/* output is DB || hash || 0xBC */
	if (outlen < modulus_len) {
		err = -1;
		printf("error overflow\n");
		goto LBL_ERR;
	}

	/* DB len = modulus_len - hLen - 1 */
	y = 0;
	memcpy(out + y, DB, modulus_len - hLen - 1);
	y += modulus_len - hLen - 1;

	/* hash */
	memcpy(out + y, hash, hLen);
	y += hLen;

	/* 0xBC */
	out[y] = 0xBC;

	/* now clear the 8*modulus_len - modulus_bitlen most significant bits */
	out[0] &= 0xFF >> ((modulus_len << 3) - (modulus_bitlen - 1));

	err = 0;
LBL_ERR:
	free(hashbuf);
	free(hash);
	free(salt);
	free(mask);
	free(DB);

	return err;
}

/* DH */

int dh_gen_exp(uint8_t *dest, int dest_len, uint8_t *dh_g, int dh_g_len, uint8_t *dh_p, int dh_p_len)
{
	DH *dh;
	int len;
	unsigned int gap;

	dh = DH_new();

	dh->p = BN_bin2bn(dh_p, dh_p_len, 0);
	dh->g = BN_bin2bn(dh_g, dh_g_len, 0);
	dh->flags |= DH_FLAG_NO_EXP_CONSTTIME;

	DH_generate_key(dh);

	len = BN_num_bytes(dh->priv_key);
	if (len > dest_len) {
		printf("len > dest_len\n");
		return -1;
	}

	gap = dest_len - len;
	memset(dest, 0, gap);
	BN_bn2bin(dh->priv_key, &dest[gap]);

	DH_free(dh);

	return 0;
}

/* dest = base ^ exp % mod */
int dh_mod_exp(uint8_t *dest, int dest_len, uint8_t *base, int base_len, uint8_t *mod, int mod_len, uint8_t *exp, int exp_len)
{
	BIGNUM *bn_dest, *bn_base, *bn_exp, *bn_mod;
	BN_CTX *ctx;
	int len;
	unsigned int gap;

	bn_base = BN_bin2bn(base, base_len, NULL);
	bn_exp = BN_bin2bn(exp, exp_len, NULL);
	bn_mod = BN_bin2bn(mod, mod_len, NULL);
	ctx = BN_CTX_new();

	bn_dest = BN_new();
	BN_mod_exp(bn_dest, bn_base, bn_exp, bn_mod, ctx);
	BN_CTX_free(ctx);


	len = BN_num_bytes(bn_dest);
	if (len > dest_len) {
		printf("len > dest_len\n");
		return -1;
	}

	gap = dest_len - len;
	memset(dest, 0, gap);
	BN_bn2bin(bn_dest, &dest[gap]);

	BN_free(bn_dest);
	BN_free(bn_mod);
	BN_free(bn_exp);
	BN_free(bn_base);

	return 0;
}

int dh_dhph_signature(uint8_t *out, uint8_t *nonce, uint8_t *dhph, RSA *r)
{
	unsigned char dest[302];
	uint8_t hash[20];
	unsigned char dbuf[256];

	dest[0x00] = 0x00;      /* version */
	dest[0x01] = 0x00;
	dest[0x02] = 0x08;      /* len (bits) */
	dest[0x03] = 0x01;      /* version data */

	dest[0x04] = 0x01;      /* msg_label */
	dest[0x05] = 0x00;
	dest[0x06] = 0x08;      /* len (bits) */
	dest[0x07] = 0x02;      /* message data */

	dest[0x08] = 0x02;      /* auth_nonce */
	dest[0x09] = 0x01;
	dest[0x0a] = 0x00;      /* len (bits) */
	memcpy(&dest[0x0b], nonce, 32);

	dest[0x2b] = 0x04;      /* DHPH - DH public key host */
	dest[0x2c] = 0x08;
	dest[0x2d] = 0x00;      /* len (bits) */
	memcpy(&dest[0x2e], dhph, 256);

	SHA1(dest, 0x12e, hash);

	if (pkcs_1_pss_encode(hash, 20, 20, 0x800, dbuf, sizeof(dbuf))) {
		printf("pss encode failed\n");
		return -1;
	}

	RSA_private_encrypt(sizeof(dbuf), dbuf, out, r, RSA_NO_PADDING);

	return 0;
}

void hexdump(const uint8_t *data, unsigned int len)
{
	while (len--)
		printf("%02x ", *data++);
	printf("\n");
}

int get_random(unsigned char *dest, int len)
{
	int fd;
	const char *urnd = "/dev/urandom";

	fd = open(urnd, O_RDONLY);
	if (fd <= 0) {
		printf("cannot open %s\n", urnd);
		return -1;
	}

	if (read(fd, dest, len) != len) {
		printf("cannot read from %s\n", urnd);
		close(fd);
		return -2;
	}

	close(fd);

	return len;
}

int parseLengthField(const unsigned char *pkt, int *len)
{
	int i;

	*len = 0;
	if (!(*pkt & 0x80)) {
		*len = *pkt;
		return 1;
	}
	for (i = 0; i < (pkt[0] & 0x7F); ++i) {
		*len <<= 8;
		*len |= pkt[i + 1];
	}
	return (pkt[0] & 0x7F) + 1;
}

int add_padding(uint8_t *dest, unsigned int len, unsigned int blocklen)
{
	uint8_t padding = 0x80;
	int count = 0;

	while (len & (blocklen - 1)) {
		*dest++ = padding;
		++len;
		++count;
		padding = 0;
	}

	return count;
}

static int get_bin_from_nibble(int in)
{
	if ((in >= '0') && (in <= '9'))
		return in - 0x30;

	if ((in >= 'A') && (in <= 'Z'))
		return in - 0x41 + 10;

	if ((in >= 'a') && (in <= 'z'))
		return in - 0x61 + 10;

	printf("fixme: unsupported chars in hostid\n");

	return 0;
}

void str2bin(uint8_t *dst, char *data, int len)
{
	int i;

	for (i = 0; i < len; i += 2)
		*dst++ = (get_bin_from_nibble(data[i]) << 4) | get_bin_from_nibble(data[i + 1]);
}

uint32_t UINT32(const unsigned char *in, unsigned int len)
{
	uint32_t val = 0;
	unsigned int i;

	for (i = 0; i < len; i++) {
		val <<= 8;
		val |= *in++;
	}

	return val;
}

int BYTE32(unsigned char *dest, uint32_t val)
{
	*dest++ = val >> 24;
	*dest++ = val >> 16;
	*dest++ = val >> 8;
	*dest++ = val;

	return 4;
}

int BYTE16(unsigned char *dest, uint16_t val)
{
	*dest++ = val >> 8;
	*dest++ = val;
	return 2;
}

static void CheckFile(char *file)
{
	if (access(file, F_OK) != 0) {
		printf("No File: %s\n", file);
		FILE* fd;
		fd = fopen(file, "w");
		fclose(fd);
	}
}

static void get_authdata_filename(char *dest, size_t len, unsigned int slot)
{
	snprintf(dest, len, "/etc/ci_auth_slot_%u.bin", slot);
	CheckFile(dest);
}

//static bool get_authdata(uint8_t *host_id, uint8_t *dhsk, uint8_t *akh, unsigned int slot, unsigned int index)
static int get_authdata(uint8_t *host_id, uint8_t *dhsk, uint8_t *akh, unsigned int slot, unsigned int index)
{
	char filename[FILENAME_MAX];
	int fd;
	uint8_t chunk[8 + 256 + 32];
	unsigned int i;

	printf("%s -> %s\n", FILENAME, __FUNCTION__);

	/* 5 pairs of data only */
	if (index > 5)
	{
//		return false;
		return 0;
	}

	get_authdata_filename(filename, sizeof(filename), slot);

	fd = open(filename, O_RDONLY);
	if (fd <= 0) {
		fprintf(stderr, "cannot open %s\n", filename);
//		return false;
		return 0;
	}

	for (i = 0; i < 5; i++) {
		if (read(fd, chunk, sizeof(chunk)) != sizeof(chunk)) {
			fprintf(stderr, "cannot read auth_data\n");
			close(fd);
//			return false;
			return 0;
		}

		if (i == index) {
			memcpy(host_id, chunk, 8);
			memcpy(dhsk, &chunk[8], 256);
			memcpy(akh, &chunk[8 + 256], 32);
			close(fd);
//			return true;
			return 1;
		}
	}

	close(fd);
//	return false;
	return 0;
}

//static bool write_authdata(unsigned int slot, const uint8_t *host_id, const uint8_t *dhsk, const uint8_t *akh)
static int write_authdata(unsigned int slot, const uint8_t *host_id, const uint8_t *dhsk, const uint8_t *akh)
{
	printf("%s -> %s\n", FILENAME, __FUNCTION__);

	char filename[FILENAME_MAX];
	int fd;
	uint8_t buf[(8 + 256 + 32) * 5];
	unsigned int entries;
	unsigned int i;
//	bool ret = false;
	int ret = 0;

	for (entries = 0; entries < 5; entries++) {
		int offset = (8 + 256 + 32) * entries;
		if (!get_authdata(&buf[offset], &buf[offset + 8], &buf[offset + 8 + 256], slot, entries))
			break;

		/* check if we got this pair already */
		if (!memcmp(&buf[offset + 8 + 256], akh, 32)) {
			printf("data already stored\n");
//			return true;
			return 1;
		}
	}

	//printf("got %d entries\n", entries);

	get_authdata_filename(filename, sizeof(filename), slot);

	fd = open(filename, O_CREAT | O_WRONLY | O_TRUNC, S_IRUSR | S_IWUSR);
	if (fd <= 0) {
		printf("cannot open %s for writing - authdata not stored\n", filename);
//		return false;
		return 0;
	}

	/* store new entry first */
	if (write(fd, host_id, 8) != 8) {
		fprintf(stderr, "error in write\n");
		goto end;
	}

	if (write(fd, dhsk, 256) != 256) {
		fprintf(stderr, "error in write\n");
		goto end;
	}

	if (write(fd, akh, 32) != 32) {
		fprintf(stderr, "error in write\n");
		goto end;
	}

	/* skip the last one if exists */
	if (entries > 3)
		entries = 3;

	for (i = 0; i < entries; i++) {
		int offset = (8 + 256 + 32) * i;
		if (write(fd, &buf[offset], (8 + 256 + 32)) != (8 + 256 + 32)) {
			fprintf(stderr, "error in write\n");
			goto end;
		}
	}

//	ret = true;
	ret = 1;
end:
	close(fd);

	return ret;
}

/* CI+ certificates */

struct cert_ctx {
	X509_STORE *store;

	/* Host */
	X509 *cust_cert;
	X509 *device_cert;

	/* Module */
	X509 *ci_cust_cert;
	X509 *ci_device_cert;
};

//static int verify_cb(int /*ok*/, X509_STORE_CTX *ctx)
static int verify_cb(int ok, X509_STORE_CTX *ctx)
{
	if (X509_STORE_CTX_get_error(ctx) == X509_V_ERR_CERT_NOT_YET_VALID) {
		time_t now = time(NULL);
		struct tm *t = localtime(&now);
		if (t->tm_year < 2015)
			//printf("seems our rtc is wrong - ignore!\n");
			return 1;
		//printf("wrong date!\n");
	}

	if (X509_STORE_CTX_get_error(ctx) == X509_V_ERR_CERT_HAS_EXPIRED)
		return 1;
	return 0;
}

static RSA *rsa_privatekey_open(const char *filename)
{
	FILE *fp;
	RSA *r = NULL;

	fp = fopen(filename, "r");
	if (!fp) {
		fprintf(stderr, "cannot open %s\n", filename);
		return NULL;
	}

	PEM_read_RSAPrivateKey(fp, &r, NULL, NULL);
	if (!r)
		fprintf(stderr, "read error\n");

	fclose(fp);

	return r;
}

static X509 *certificate_open(const char *filename)
{
	FILE *fp;
	X509 *cert;

	fp = fopen(filename, "r");
	if (!fp) {
		fprintf(stderr, "cannot open %s\n", filename);
		return NULL;
	}

	cert = PEM_read_X509(fp, NULL, NULL, NULL);
	if (!cert)
		fprintf(stderr, "cannot read cert\n");

	fclose(fp);

	return cert;
}

//static bool certificate_validate(struct cert_ctx *ctx, X509 *cert)
static int certificate_validate(struct cert_ctx *ctx, X509 *cert)
{
	X509_STORE_CTX *store_ctx;
	int ret;

	store_ctx = X509_STORE_CTX_new();

	X509_STORE_CTX_init(store_ctx, ctx->store, cert, NULL);
	X509_STORE_CTX_set_verify_cb(store_ctx, verify_cb);
	X509_STORE_CTX_set_flags(store_ctx, X509_V_FLAG_IGNORE_CRITICAL);

	ret = X509_verify_cert(store_ctx);

	if (ret != 1)
		fprintf(stderr, "%s\n", X509_verify_cert_error_string(store_ctx->error));

	X509_STORE_CTX_free(store_ctx);

	return ret == 1;
}

static X509 *certificate_load_and_check(struct cert_ctx *ctx, const char *filename)
{
	X509 *cert;

	if (!ctx->store) {
		/* we assume this is the first certificate added - so its root-ca */
		ctx->store = X509_STORE_new();
		if (!ctx->store) {
			fprintf(stderr, "cannot create cert_store\n");
			exit(-1);
		}

		if (X509_STORE_load_locations(ctx->store, filename, NULL) != 1) {
			fprintf(stderr, "load of first certificate (root_ca) failed\n");
			exit(-1);
		}

		return NULL;
	}

	cert = certificate_open(filename);
	if (!cert) {
		fprintf(stderr, "cannot open certificate %s\n", filename);
		return NULL;
	}

	if (!certificate_validate(ctx, cert)) {
		fprintf(stderr, "cannot vaildate certificate\n");
		X509_free(cert);
		return NULL;
	}

	/* push into store - create a chain */
	if (X509_STORE_load_locations(ctx->store, filename, NULL) != 1) {
		fprintf(stderr, "load of certificate failed\n");
		X509_free(cert);
		return NULL;
	}

	return cert;
}

static X509 *certificate_import_and_check(struct cert_ctx *ctx, const uint8_t *data, int len)
{
	X509 *cert;

	cert = d2i_X509(NULL, &data, len);
	if (!cert) {
		fprintf(stderr, "cannot read certificate\n");
		return NULL;
	}

	if (!certificate_validate(ctx, cert)) {
		fprintf(stderr, "cannot vaildate certificate\n");
		X509_free(cert);
		return NULL;
	}

	X509_STORE_add_cert(ctx->store, cert);

	return cert;
}


/* CI+ credentials */

#define MAX_ELEMENTS    33

uint32_t datatype_sizes[MAX_ELEMENTS] = {
	0, 50, 0, 0, 0, 8, 8, 0,
	0, 0, 0, 0, 32, 256, 256, 0,
	0, 256, 256, 32, 8, 8, 32, 32,
	0, 8, 2, 32, 1, 32, 1, 0,
	32
};

struct element {
	uint8_t *data;
	uint32_t size;
	/* buffer valid */
//	bool valid;
	int valid;
};

struct cc_ctrl_data {
	/* parent */
	//struct ci_session *session;
//	tSlot *slot;
	struct caslot* slot;
//	struct caservice* slot;

	/* ci+ credentials */
	struct element elements[MAX_ELEMENTS];

	/* DHSK */
	uint8_t dhsk[256];

	/* KS_host */
	uint8_t ks_host[32];

	/* derived keys */
	uint8_t sek[16];
	uint8_t sak[16];

	/* AKH checks - module performs 5 tries to get correct AKH */
	unsigned int akh_index;

	/* authentication data */
	uint8_t dh_exp[256];

	/* certificates */
	struct cert_ctx *cert_ctx;

	/* private key of device-cert */
	RSA *rsa_device_key;
};


static struct element *element_get(struct cc_ctrl_data *cc_data, unsigned int id)
{
	/* array index */
	if ((id < 1) || (id >= MAX_ELEMENTS)) {
		fprintf(stderr, "element_get: invalid id\n");
		return NULL;
	}

	return &cc_data->elements[id];
}

static void element_invalidate(struct cc_ctrl_data *cc_data, unsigned int id)
{
	struct element *e;

	e = element_get(cc_data, id);
	if (e) {
		free(e->data);
		memset(e, 0, sizeof(struct element));
	}
}

static void element_init(struct cc_ctrl_data *cc_data)
{
	unsigned int i;

	for (i = 1; i < MAX_ELEMENTS; i++)
		element_invalidate(cc_data, i);
}

//static bool element_set(struct cc_ctrl_data *cc_data, unsigned int id, const uint8_t *data, uint32_t size)
static int element_set(struct cc_ctrl_data *cc_data, unsigned int id, const uint8_t *data, uint32_t size)
{
	struct element *e;

	e = element_get(cc_data, id);
	if (e == NULL)
	{
//		return false;
		return 0;
	}

	/* check size */
	if ((datatype_sizes[id] != 0) && (datatype_sizes[id] != size)) {
		fprintf(stderr, "size %d of datatype_id %d doesn't match\n", size, id);
//		return false;
		return 0;
	}

	free(e->data);
	e->data = (uint8_t*)malloc(size);
	memcpy(e->data, data, size);
	e->size = size;
//	e->valid = true;
	e->valid = 1;

	//printf("stored %d with len %d\n", id, size);

//	return true;
	return 1;
}

//static bool element_set_certificate(struct cc_ctrl_data *cc_data, unsigned int id, X509 *cert)
static int element_set_certificate(struct cc_ctrl_data *cc_data, unsigned int id, X509 *cert)
{
	unsigned char *cert_der = NULL;
	int cert_len;

	cert_len = i2d_X509(cert, &cert_der);
	if (cert_len <= 0) {
		printf("cannot get data in DER format\n");
//		return false;
		return 0;
	}

	if (!element_set(cc_data, id, cert_der, cert_len)) {
		printf("cannot store element (%d)\n", id);
//		return false;
		return 0;
	}

//	return true;
	return 1;
}

//static bool element_set_hostid_from_certificate(struct cc_ctrl_data *cc_data, unsigned int id, X509 *cert)
static int element_set_hostid_from_certificate(struct cc_ctrl_data *cc_data, unsigned int id, X509 *cert)
{
	X509_NAME *subject;
	int nid_cn = OBJ_txt2nid("CN");
	char hostid[20];
	uint8_t bin_hostid[8];

	if ((id != 5) && (id != 6)) {
		printf("wrong datatype_id for hostid\n");
//		return false;
		return 0;
	}

	subject = X509_get_subject_name(cert);
	X509_NAME_get_text_by_NID(subject, nid_cn, hostid, sizeof(hostid));

	if (strlen(hostid) != 16) {
		printf("malformed hostid\n");
//		return false;
		return 0;
	}

	str2bin(bin_hostid, hostid, 16);

	if (!element_set(cc_data, id, bin_hostid, sizeof(bin_hostid))) {
		printf("cannot set hostid\n");
//		return false;
		return 0;
	}

//	return true;
	return 1;
}

//static bool element_valid(struct cc_ctrl_data *cc_data, unsigned int id)
static int element_valid(struct cc_ctrl_data *cc_data, unsigned int id)
{
	struct element *e;

	e = element_get(cc_data, id);

	return e && e->valid;
}

static unsigned int element_get_buf(struct cc_ctrl_data *cc_data, uint8_t *dest, unsigned int id)
{
	struct element *e;

	e = element_get(cc_data, id);
	if (e == NULL)
		return 0;

	if (!e->valid) {
		fprintf(stderr, "element_get_buf: datatype %d not valid\n", id);
		return 0;
	}

	if (!e->data) {
		fprintf(stderr, "element_get_buf: datatype %d doesn't exist\n", id);
		return 0;
	}

	if (dest)
		memcpy(dest, e->data, e->size);

	return e->size;
}

static unsigned int element_get_req(struct cc_ctrl_data *cc_data, uint8_t *dest, unsigned int id)
{
	unsigned int len = element_get_buf(cc_data, &dest[3], id);

	if (len == 0) {
		fprintf(stderr, "cannot get element %d\n", id);
		return 0;
	}

	dest[0] = id;
	dest[1] = len >> 8;
	dest[2] = len;

	return 3 + len;
}

static uint8_t *element_get_ptr(struct cc_ctrl_data *cc_data, unsigned int id)
{
	struct element *e;

	e = element_get(cc_data, id);
	if (e == NULL)
		return NULL;

	if (!e->valid) {
		fprintf(stderr, "element_get_ptr: datatype %u not valid\n", id);
		return NULL;
	}

	if (!e->data) {
		fprintf(stderr, "element_get_ptr: datatype %u doesn't exist\n", id);
		return NULL;
	}

	return e->data;
}


/* content_control commands */

//static bool sac_check_auth(const uint8_t *data, unsigned int len, uint8_t *sak)
static int sac_check_auth(const uint8_t *data, unsigned int len, uint8_t *sak)
{
	struct aes_xcbc_mac_ctx ctx;
	uint8_t calced_signature[16];

	if (len < 16)
	{
//		return false;
		return 0;
	}
	aes_xcbc_mac_init(&ctx, sak);
	aes_xcbc_mac_process(&ctx, (uint8_t *)"\x04", 1);        /* header len */
	aes_xcbc_mac_process(&ctx, data, len - 16);
	aes_xcbc_mac_done(&ctx, calced_signature);

	if (memcmp(&data[len - 16], calced_signature, 16)) {
		fprintf(stderr, "signature wrong\n");
//		return false;
		return 0;
	}

	//printf("auth ok!\n");
//	return true;
	return 1;
}

static int sac_gen_auth(uint8_t *out, uint8_t *in, unsigned int len, uint8_t *sak)
{
	struct aes_xcbc_mac_ctx ctx;
	aes_xcbc_mac_init(&ctx, sak);
	aes_xcbc_mac_process(&ctx, (uint8_t *)"\x04", 1);        /* header len */
	aes_xcbc_mac_process(&ctx, in, len);
	aes_xcbc_mac_done(&ctx, out);

	return 16;
}

static void generate_key_seed(struct cc_ctrl_data *cc_data)
{
	/* this is triggered by new ns_module */

	/* generate new key_seed -> SEK/SAK key derivation */
	SHA256_CTX sha;

	SHA256_Init(&sha);
	SHA256_Update(&sha, &cc_data->dhsk[240], 16);
	SHA256_Update(&sha, element_get_ptr(cc_data, 22), element_get_buf(cc_data, NULL, 22));
	SHA256_Update(&sha, element_get_ptr(cc_data, 20), element_get_buf(cc_data, NULL, 20));
	SHA256_Update(&sha, element_get_ptr(cc_data, 21), element_get_buf(cc_data, NULL, 21));
	SHA256_Final(cc_data->ks_host, &sha);
}

static void generate_ns_host(struct cc_ctrl_data *cc_data)
{
	uint8_t buf[8];

	get_random(buf, sizeof(buf));
	element_set(cc_data, 20, buf, sizeof(buf));
}

static int generate_SAK_SEK(uint8_t *sak, uint8_t *sek, const uint8_t *ks_host)
{
	AES_KEY key;
	const uint8_t key_data[16] = { 0xea, 0x74, 0xf4, 0x71, 0x99, 0xd7, 0x6f, 0x35, 0x89, 0xf0, 0xd1, 0xdf, 0x0f, 0xee, 0xe3, 0x00 };
	uint8_t dec[32];
	int i;

	/* key derivation of sak & sek */

	AES_set_encrypt_key(key_data, 128, &key);

	for (i = 0; i < 2; i++)
		AES_ecb_encrypt(&ks_host[16 * i], &dec[16 * i], &key, 1);

	for (i = 0; i < 16; i++)
		sek[i] = ks_host[i] ^ dec[i];

	for (i = 0; i < 16; i++)
		sak[i] = ks_host[16 + i] ^ dec[16 + i];

	return 0;
}

static int sac_crypt(uint8_t *dst, const uint8_t *src, unsigned int len, const uint8_t *key_data, int encrypt)
{
	uint8_t iv[16] = { 0xf7, 0x70, 0xb0, 0x36, 0x03, 0x61, 0xf7, 0x96, 0x65, 0x74, 0x8a, 0x26, 0xea, 0x4e, 0x85, 0x41 };
	AES_KEY key;

	/* AES_ENCRYPT is '1' */
#if x_debug
	printf("%s -> %s\n", FILENAME, __FUNCTION__);
#endif
	if (encrypt)
		AES_set_encrypt_key(key_data, 128, &key);
	else
		AES_set_decrypt_key(key_data, 128, &key);

	AES_cbc_encrypt(src, dst, len, &key, iv, encrypt);

	return 0;
}

static X509 *import_ci_certificates(struct cc_ctrl_data *cc_data, unsigned int id)
{
	struct cert_ctx *ctx = cc_data->cert_ctx;
	X509 *cert;
	uint8_t buf[2048];
	unsigned int len;

	printf("%s -> %s\n", FILENAME, __FUNCTION__);

	len = element_get_buf(cc_data, buf, id);

	cert = certificate_import_and_check(ctx, buf, len);
	if (!cert) {
		printf("cannot read/verify DER cert\n");
		return NULL;
	}

	return cert;
}

static int check_ci_certificates(struct cc_ctrl_data *cc_data)
{
	struct cert_ctx *ctx = cc_data->cert_ctx;

	printf("%s -> %s\n", FILENAME, __FUNCTION__);

	/* check if both certificates are available before we push and verify them */

	/* check for CICAM_BrandCert */
	if (!element_valid(cc_data, 8))
		return -1;

	/* check for CICAM_DevCert */
	if (!element_valid(cc_data, 16))
		return -1;

#if 0
	{
		/* write ci device cert to disk */
		int fd = open("ci_cert.der", O_CREAT | O_WRONLY | O_TRUNC, S_IRUSR | S_IWUSR);
		write(fd, element_get_ptr(cc_data, 16), element_get_buf(cc_data, NULL, 16));
		close(fd);
	}
#endif

	/* import CICAM_BrandCert */
	if ((ctx->ci_cust_cert = import_ci_certificates(cc_data, 8)) == NULL) {
		printf("cannot import cert\n");
		return -1;
	}

	/* import CICAM_DevCert */
	if ((ctx->ci_device_cert = import_ci_certificates(cc_data, 16)) == NULL) {
		printf("cannot import cert\n");
		return -1;
	}

	/* everything seems to be fine here - so extract the CICAM_id from cert */
	if (!element_set_hostid_from_certificate(cc_data, 6, ctx->ci_device_cert)) {
		printf("cannot set cicam_id in elements\n");
		return -1;
	}

	return 0;
}

static int generate_akh(struct cc_ctrl_data *cc_data)
{
	uint8_t akh[32];
	SHA256_CTX sha;

	printf("%s>%s\n", FILENAME, __FUNCTION__);

	SHA256_Init(&sha);
	SHA256_Update(&sha, element_get_ptr(cc_data, 6), element_get_buf(cc_data, NULL, 6));
	SHA256_Update(&sha, element_get_ptr(cc_data, 5), element_get_buf(cc_data, NULL, 5));
	SHA256_Update(&sha, cc_data->dhsk, 256);
	SHA256_Final(akh, &sha);

	element_set(cc_data, 22, akh, sizeof(akh));

	return 0;
}

//static bool check_dh_challenge(struct cc_ctrl_data *cc_data)
static int check_dh_challenge(struct cc_ctrl_data *cc_data)
{
	printf("%s -> %s\n", FILENAME, __FUNCTION__);

	/* check if every element for calculation of DHSK & AKH is available */

	/* check for auth_nonce */
	if (!element_valid(cc_data, 19))
	{
//		return false;
		return 0;
	}
	/* check for CICAM_id */
	if (!element_valid(cc_data, 6))
	{
//		return false;
		return 0;
	}

	/* check for DHPM */
	if (!element_valid(cc_data, 14))
	{
//		return false;
		return 0;
	}

	/* check for Signature_B */
	if (!element_valid(cc_data, 18))
	{
//		return false;
		return 0;
	}

	/* calculate DHSK - DHSK = DHPM ^ dh_exp % dh_p */
	dh_mod_exp(cc_data->dhsk, 256, element_get_ptr(cc_data, 14), 256, dh_p, sizeof(dh_p), cc_data->dh_exp, 256);

	/* gen AKH */
	generate_akh(cc_data);

	/* disable 5 tries of startup -> use new calculated one */
	cc_data->akh_index = 5;

	/* write to disk */
//	write_authdata(cc_data->slot->slot, element_get_ptr(cc_data, 5), cc_data->dhsk, element_get_ptr(cc_data, 22));
//	write_authdata(cc_data->slot->caslot->connid, element_get_ptr(cc_data, 5), cc_data->dhsk, element_get_ptr(cc_data, 22));
	write_authdata(cc_data->slot->connid, element_get_ptr(cc_data, 5), cc_data->dhsk, element_get_ptr(cc_data, 22));

//	return true;
	return 1;
}

static int restart_dh_challenge(struct cc_ctrl_data *cc_data)
{
	uint8_t dhph[256], sign_A[256];
	struct cert_ctx *ctx;

	printf("%s -> %s\n", FILENAME, __FUNCTION__);

	if (!cc_data->cert_ctx) {
		ctx = (struct cert_ctx*)calloc(1, sizeof(struct cert_ctx));
		cc_data->cert_ctx = ctx;
	} else {
		ctx = cc_data->cert_ctx;
	}

	/* load certificates and device key */
	certificate_load_and_check(ctx, ROOT_CERT);
	ctx->cust_cert = certificate_load_and_check(ctx, CUSTOMER_CERT);
	ctx->device_cert = certificate_load_and_check(ctx, DEVICE_CERT);

	if (!ctx->cust_cert || !ctx->device_cert) {
		fprintf(stderr, "cannot loader certificates\n");
		return -1;
	}

	/* add data to element store */
	if (!element_set_certificate(cc_data, 7, ctx->cust_cert))
		fprintf(stderr, "cannot store cert in elements\n");

	if (!element_set_certificate(cc_data, 15, ctx->device_cert))
		fprintf(stderr, "cannot store cert in elements\n");

	if (!element_set_hostid_from_certificate(cc_data, 5, ctx->device_cert))
		fprintf(stderr, "cannot set hostid in elements\n");

	cc_data->rsa_device_key = rsa_privatekey_open(DEVICE_CERT);
	if (!cc_data->rsa_device_key) {
		fprintf(stderr, "cannot read private key\n");
		return -1;
	}

	/* invalidate elements */
	element_invalidate(cc_data, 6);
	element_invalidate(cc_data, 14);
	element_invalidate(cc_data, 18);
	element_invalidate(cc_data, 22); /* this will refuse a unknown cam */

	/* new dh_exponent */
	dh_gen_exp(cc_data->dh_exp, 256, dh_g, sizeof(dh_g), dh_p, sizeof(dh_p));

	/* new DHPH  - DHPH = dh_g ^ dh_exp % dh_p */
	dh_mod_exp(dhph, sizeof(dhph), dh_g, sizeof(dh_g), dh_p, sizeof(dh_p), cc_data->dh_exp, 256);

	/* store DHPH */
	element_set(cc_data, 13, dhph, sizeof(dhph));

	/* create Signature_A */
	dh_dhph_signature(sign_A, element_get_ptr(cc_data, 19), dhph, cc_data->rsa_device_key);

	/* store Signature_A */
	element_set(cc_data, 17, sign_A, sizeof(sign_A));

	return 0;
}

static int generate_uri_confirm(struct cc_ctrl_data *cc_data, const uint8_t *sak)
{
	SHA256_CTX sha;
	uint8_t uck[32];
	uint8_t uri_confirm[32];

	printf("%s -> %s\n", FILENAME, __FUNCTION__);

	/* calculate UCK (uri confirmation key) */
	SHA256_Init(&sha);
	SHA256_Update(&sha, sak, 16);
	SHA256_Final(uck, &sha);

	/* calculate uri_confirm */
	SHA256_Init(&sha);
	SHA256_Update(&sha, element_get_ptr(cc_data, 25), element_get_buf(cc_data, NULL, 25));
	SHA256_Update(&sha, uck, 32);
	SHA256_Final(uri_confirm, &sha);

	element_set(cc_data, 27, uri_confirm, 32);

	return 0;
}

static void check_new_key(struct cc_ctrl_data *cc_data)
{
	const uint8_t s_key[16] = { 0x3e, 0x20, 0x15, 0x84, 0x2c, 0x37, 0xce, 0xe3, 0xd6, 0x14, 0x57, 0x3e, 0x3a, 0xab, 0x91, 0xb6 };
	AES_KEY aes_ctx;
	uint8_t dec[32];
	uint8_t *kp;
	uint8_t slot;
	unsigned int i;
#if x_debug
	printf("%s -> %s\n", FILENAME, __FUNCTION__);
#endif
	/* check for keyprecursor */
	if (!element_valid(cc_data, 12))
		return;

	/* check for slot */
	if (!element_valid(cc_data, 28))
		return;

	kp = element_get_ptr(cc_data, 12);
	element_get_buf(cc_data, &slot, 28);

	AES_set_encrypt_key(s_key, 128, &aes_ctx);
	for (i = 0; i < 32; i += 16)
		AES_ecb_encrypt(&kp[i], &dec[i], &aes_ctx, 1);

	for (i = 0; i < 32; i++)
	{
		dec[i] ^= kp[i];
		cc_data->slot->lastKey[i] = dec[i];
	}
	cc_data->slot->lastParity = slot;

//obi	if (cc_data->slot->scrambled)
//obi		cc_data->slot->ccmgrSession->resendKey(cc_data->slot);

	/* reset */
	element_invalidate(cc_data, 12);
	element_invalidate(cc_data, 28);
}

static int data_get_handle_new(struct cc_ctrl_data *cc_data, unsigned int id)
{
#if x_debug
	printf("%s -> %s ID = (%d)\n", FILENAME, __FUNCTION__, id);
#endif
	/* handle trigger events */

	/* depends on new received items */

	switch (id) {
	case 8:         /* CICAM_BrandCert */
	case 14:        /* DHPM */
	case 16:        /* CICAM_DevCert */
	case 18:        /* Signature_B */
		/* this results in CICAM_ID when cert-chain is verified and ok */
		if (check_ci_certificates(cc_data))
			break;
		/* generate DHSK & AKH */
		check_dh_challenge(cc_data);
		break;

	case 19:        /* auth_nonce - triggers new dh keychallenge - invalidates DHSK & AKH */
		/* generate DHPH & Signature_A */
		restart_dh_challenge(cc_data);
		break;

	case 21:        /* Ns_module - triggers SAC key calculation */
		generate_ns_host(cc_data);
		generate_key_seed(cc_data);
		generate_SAK_SEK(cc_data->sak, cc_data->sek, cc_data->ks_host);
		break;

	/* SAC data messages */

	case 12:                //keyprecursor
		check_new_key(cc_data);
		break;
	case 25:                //uri_message
		generate_uri_confirm(cc_data, cc_data->sak);
		break;
	case 28:                //key register
		check_new_key(cc_data);
		break;

	default:
		break;
	}

	return 0;
}

static int data_req_handle_new(struct cc_ctrl_data *cc_data, unsigned int id)
{
#if x_debug
	printf("%s -> %s ID = (%d)\n", FILENAME, __FUNCTION__, id);
#endif
	switch (id) {
	case 22:                /* AKH */
	{
		uint8_t akh[32], host_id[8];
		memset(akh, 0, sizeof(akh));
		if (cc_data->akh_index != 5) {
//			if (!get_authdata(host_id, cc_data->dhsk, akh, cc_data->slot->slot, cc_data->akh_index++))
			if (!get_authdata(host_id, cc_data->dhsk, akh, cc_data->slot->connid, cc_data->akh_index++))
				cc_data->akh_index = 5;
			if (!element_set(cc_data, 22, akh, 32))
				printf("cannot set AKH in elements\n");
			if (!element_set(cc_data, 5, host_id, 8))
				printf("cannot set host_id in elements\n");
		}
	}
	default:
		break;
	}

	return 0;
}

static int data_get_loop(struct cc_ctrl_data *cc_data, const unsigned char *data, unsigned int datalen, unsigned int items)
{
	unsigned int i;
	int dt_id, dt_len;
	unsigned int pos = 0;
#if x_debug
	printf("%s -> %s\n", FILENAME, __FUNCTION__);
#endif
	for (i = 0; i < items; i++) {
		if (pos + 3 > datalen)
			return 0;

		dt_id = data[pos++];
		dt_len = data[pos++] << 8;
		dt_len |= data[pos++];

		if (pos + dt_len > datalen)
			return 0;
#if x_debug
		printf("set element (%d) ", dt_id);
#if y_debug
		hexdump(&data[pos], dt_len);
#else
		printf("\n");
#endif
#endif
		element_set(cc_data, dt_id, &data[pos], dt_len);

		data_get_handle_new(cc_data, dt_id);

		pos += dt_len;
	}

	return pos;
}

static int data_req_loop(struct cc_ctrl_data *cc_data, unsigned char *dest, const unsigned char *data, unsigned int datalen, unsigned int items)
{
	int dt_id;
	unsigned int i;
	int pos = 0;
	int len;
#if x_debug
	printf("%s -> %s\n", FILENAME, __FUNCTION__);
#endif
	if (items > datalen)
		return -1;

	for (i = 0; i < items; i++) {
		dt_id = *data++;
#if x_debug
		printf("req element %d\n", dt_id);
#endif
		data_req_handle_new(cc_data, dt_id);    /* check if there is any action needed before we answer */
		len = element_get_req(cc_data, dest, dt_id);
		if (len == 0) {
			printf("cannot get element %d\n", dt_id);
			return -1;
		}
#if x_debug
		printf("element (%d) > ", dt_id);
#if y_debug
		for (int ic = 0; ic < len; ic++)
			printf("%02x ", dest[ic]);
#endif
		printf("\n");
#endif

		pos += len;
		dest += len;
	}

	return pos;
}

#endif

int checkcerts(void)
{
	if(status.certchecked == 0)
	{
		if (access(ROOT_CERT, F_OK) == 0 && access(ROOT_CERT, F_OK) == 0 && access(ROOT_CERT, F_OK) == 0)
			status.certok = 1;
		status.certchecked = 1;
	}
	return status.certok;
}

void ci_ccmgr_cc_open_cnf(struct dvbdev* dvbnode, int sessionnr)
{
	uint8_t tag[3] = { 0x9f, 0x90, 0x02 };
	uint8_t bitmap = 0x01;
	printf("%s -> %s\n", FILENAME, __FUNCTION__);

	ci_ccmgr_cc_data_initialize(dvbnode);

//	sendAPDU(dvbnode, sessionnr, tag, data, 1);
	sendAPDU(dvbnode, sessionnr, tag, &bitmap, 1);

//	sendAPDU(tag, &bitmap, 1);
}

int ci_ccmgr_cc_data_initialize(struct dvbdev* dvbnode)
{
	struct cc_ctrl_data *data;
	uint8_t buf[32], host_id[8];

	printf("%s -> %s\n", FILENAME, __FUNCTION__);

//	dvbnode->caslot->private_data = NULL;
	if (dvbnode->caslot->private_data) {
		fprintf(stderr, "strange private_data not null!\n");
		return 0;
	}

	data = (struct cc_ctrl_data*)calloc(1, sizeof(struct cc_ctrl_data));
	if (!data) {
		fprintf(stderr, "out of memory\n");
		return 0;
	}

	/* parent */
	data->slot = dvbnode->caslot;

	/* clear storage of credentials */
	element_init(data);

	/* set status field - OK */
	memset(buf, 0, 1);
	if (!element_set(data, 30, buf, 1))
		fprintf(stderr, "cannot set status in elements\n");

	/* set uri_versions */
	memset(buf, 0, 32);
	buf[31] = 1;
	if (!element_set(data, 29, buf, 32))
		fprintf(stderr, "cannot set uri_versions in elements\n");

	/* load first AKH */
	data->akh_index = 0;
	if (!get_authdata(host_id, data->dhsk, buf, dvbnode->caslot->connid, data->akh_index)) {
		/* no AKH available */
		memset(buf, 0, sizeof(buf));
		data->akh_index = 5;    /* last one */
	}

	if (!element_set(data, 22, buf, 32))
		fprintf(stderr, "cannot set AKH in elements\n");

	if (!element_set(data, 5, host_id, 8))
		fprintf(stderr, "cannot set host_id elements\n");

	dvbnode->caslot->private_data = data;

	return 1;
}

//bool eDVBCIContentControlManagerSession::ci_ccmgr_cc_data_req(tSlot *tslot, const uint8_t *data, unsigned int len)
int ci_ccmgr_cc_data_req(struct dvbdev* dvbnode, int sessionnr, uint8_t *data, unsigned int len)
{
//	struct cc_ctrl_data *cc_data = (struct cc_ctrl_data*)(tslot->private_data);
	struct cc_ctrl_data *cc_data = (struct cc_ctrl_data*)(dvbnode->caslot->private_data);
	uint8_t cc_data_cnf_tag[3] = { 0x9f, 0x90, 0x04 };
	uint8_t dest[2048 * 2];
	int dt_nr;
	int id_bitmask;
	int answ_len;
	unsigned int rp = 0;

	printf("%s -> %s\n", FILENAME, __FUNCTION__);

	if (len < 2)
		return 0;

	id_bitmask = data[rp++];

	/* handle data loop */
	dt_nr = data[rp++];
	rp += data_get_loop(cc_data, &data[rp], len - rp, dt_nr);

	if (len < rp + 1)
		return 0;

	/* handle req_data loop */
	dt_nr = data[rp++];

	dest[0] = id_bitmask;
	dest[1] = dt_nr;

	answ_len = data_req_loop(cc_data, &dest[2], &data[rp], len - rp, dt_nr);
	if (answ_len <= 0) {
		fprintf(stderr, "cannot req data\n");
		return 0;
	}

	answ_len += 2;

	sendAPDU(dvbnode, sessionnr, cc_data_cnf_tag, dest, answ_len);

	return 1;
}

void ci_ccmgr_cc_sync_req(struct dvbdev* dvbnode, int sessionnr)
{
	uint8_t tag[3] = { 0x9f, 0x90, 0x06 };
	uint8_t sync_req_status = 0x00;    /* OK */

	printf("%s -> %s\n", FILENAME, __FUNCTION__);
	sendAPDU(dvbnode, sessionnr, tag, &sync_req_status, 1);
}

//bool eDVBCIContentControlManagerSession::ci_ccmgr_cc_sac_data_req(tSlot *tslot, const uint8_t *data, unsigned int len)
int ci_ccmgr_cc_sac_data_req(struct dvbdev* dvbnode, int sessionnr, uint8_t *data, unsigned int len)
{
//	struct cc_ctrl_data *cc_data = (struct cc_ctrl_data*)(tslot->private_data);
	struct cc_ctrl_data *cc_data = (struct cc_ctrl_data*)(dvbnode->caslot->private_data);
	uint8_t data_cnf_tag[3] = { 0x9f, 0x90, 0x08 };
	uint8_t dest[2048];
	uint8_t tmp[len];
	int id_bitmask, dt_nr;
	unsigned int serial;
	int answ_len;
	int pos = 0;
	unsigned int rp = 0;
	printf("%s -> %s\n", FILENAME, __FUNCTION__);

	if (len < 10)
		return 0;

	memcpy(tmp, data, 8);
	sac_crypt(&tmp[8], &data[8], len - 8, cc_data->sek, AES_DECRYPT);
	data = tmp;
#if y_debug
	printf("decryted > ");
	for (unsigned int i = 0; i < len; i++)
		printf("%02x ", data[i]);
	printf("\n");
#endif
	if (!sac_check_auth(data, len, cc_data->sak)) {
		fprintf(stderr, "check_auth of message failed\n");
		return 0;
	}

	serial = UINT32(&data[rp], 4);

	/* skip serial & header */
	rp += 8;

	id_bitmask = data[rp++];

	/* handle data loop */
	dt_nr = data[rp++];
	rp += data_get_loop(cc_data, &data[rp], len - rp, dt_nr);

	if (len < rp + 1)
		return 0;

	dt_nr = data[rp++];

	/* create answer */
	pos += BYTE32(&dest[pos], serial);
	pos += BYTE32(&dest[pos], 0x01000000);

	dest[pos++] = id_bitmask;
	dest[pos++] = dt_nr;    /* dt_nbr */

	answ_len = data_req_loop(cc_data, &dest[pos], &data[rp], len - rp, dt_nr);
	if (answ_len <= 0) {
		fprintf(stderr, "cannot req data\n");
		return 0;
	}
	pos += answ_len;

	return ci_ccmgr_cc_sac_send(dvbnode, sessionnr, data_cnf_tag, dest, pos);
}

//bool eDVBCIContentControlManagerSession::ci_ccmgr_cc_sac_send(tSlot *tslot, const uint8_t *tag, uint8_t *data, unsigned int pos)
int ci_ccmgr_cc_sac_send(struct dvbdev* dvbnode, int sessionnr, uint8_t *tag, uint8_t *data, unsigned int pos)
{
//	struct cc_ctrl_data *cc_data = (struct cc_ctrl_data*)(tslot->private_data);
	struct cc_ctrl_data *cc_data = (struct cc_ctrl_data*)(dvbnode->caslot->private_data);

	printf("%s -> %s (%02X%02X%02X) \n", FILENAME, __FUNCTION__, tag[0], tag[1], tag[2]);

	if (pos < 8)
		return 0;

	pos += add_padding(&data[pos], pos - 8, 16);
	BYTE16(&data[6], pos - 8);      /* len in header */

	pos += sac_gen_auth(&data[pos], data, pos, cc_data->sak);
#if y_debug
	printf("Data for encrypt > ");
	for (unsigned int i = 0; i < pos; i++)
		printf("%02x ", data[i]);
	printf("\n");
#endif
	sac_crypt(&data[8], &data[8], pos - 8, cc_data->sek, AES_ENCRYPT);

//	sendAPDU(tag, data, pos);
	sendAPDU(dvbnode, sessionnr, tag, data, pos);

	return 1;
}

//void eDVBCIContentControlManagerSession::ci_ccmgr_cc_sac_sync_req(tSlot *tslot, const uint8_t *data, unsigned int len)
void ci_ccmgr_cc_sac_sync_req(struct dvbdev* dvbnode, int sessionnr, uint8_t *data, unsigned int len)
{
	uint8_t sync_cnf_tag[3] = { 0x9f, 0x90, 0x10 };
	uint8_t dest[64];
	unsigned int serial;
	int pos = 0;

	printf("%s -> %s\n", FILENAME, __FUNCTION__);
#if y_debug
	hexdump(data, len);
#endif
	serial = UINT32(data, 4);

	pos += BYTE32(&dest[pos], serial);
	pos += BYTE32(&dest[pos], 0x01000000);

	/* status OK */
	dest[pos++] = 0;

	ci_ccmgr_cc_sac_send(dvbnode, sessionnr, sync_cnf_tag, dest, pos);
//	tslot->ccmgr_ready = true;
	dvbnode->caslot->ccmgr_ready = 1;
}

#endif
