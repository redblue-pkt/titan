#ifndef CA_H
#define CA_H

char oldpids[256];


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
		oldpids[0] = 0;
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
	int i = 0;

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
	if(checkrealbox("HD51") == 1 || checkrealbox("HD60") == 1 || checkrealbox("HD61") == 1)
		usleep(150000);
	for(i = 0; i < 10; i++)
	{
		ret = dvbwrite(fd, buf, count, tout);
		if(ret > 0)
			break;
		sleep(1);
	}
	if(checkbox("DM7020HD") == 1 || checkbox("DM7020HDV2") == 1 || checkbox("DM900") == 1 || checkbox("DM920") == 1 || checkbox("DM520") == 1 || checkbox("DM525") == 1 || checkrealbox("HD51") == 1 || checkrealbox("HD60") == 1 || checkrealbox("HD61") == 1)
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
	char* tmpstr = NULL, *tmpstr1 = NULL, *tmpstr2 = NULL;
	struct casession* casession = NULL;
	struct menulist* mlist = NULL, *mbox = NULL;
	int ca0autopin = 0;

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
				if(dvbnode->caslot->connid > 0)
				{
					tmpstr2 = ostrcat(tmpstr2, "ca", 1, 0);
				  tmpstr2 = ostrcat(tmpstr2, oitoa(dvbnode->caslot->connid - 1), 1, 1);
				  tmpstr2 = ostrcat(tmpstr2, "_pin", 1, 0);
				}
				if(ostrstr(str, "Bitte geben Sie Ihre Jugendschutz-PIN ein") != NULL)
				{ 
				  if(getconfig(tmpstr2, NULL) != NULL)
				  {
				  	ca0autopin = 1;
				  	tmpstr = ostrcat(tmpstr, getconfig(tmpstr2, NULL), 1, 0);
				  }
				}
				if(ostrstr(str, "Bitte versuchen Sie es erneut") != NULL)
				{
					delconfig(tmpstr2);
				}
				if(ca0autopin == 0)  	 
					tmpstr = textinputsave(str, tmpstr1, tmpstr2);
				if(tmpstr == NULL)
					cammicancelenq(dvbnode, sessionnr);
				else
				{
					if(strlen(tmpstr) > alen) tmpstr[alen] = '\0';
					cammianswerenq(dvbnode, sessionnr, tmpstr, strlen(tmpstr));
				}
				free(tmpstr); tmpstr = NULL;
				free(tmpstr1); tmpstr1 = NULL;
				free(tmpstr2); tmpstr2 = NULL;

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
						textbox(_(tmpstr), _(tmpstr1), NULL, getrcconfigint("rcok", NULL), NULL, getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 1000, 300, 7, 0);
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

//host funktions
#ifdef MIPSEL
int cahostaction(struct dvbdev* dvbnode, int sessionnr)
{
	struct casession* casession = NULL;

	if(dvbnode == NULL || dvbnode->caslot == NULL) return 0;
	casession = dvbnode->caslot->casession;

	debug(620, "cahostaction nr %d, stat %d", sessionnr, casession[sessionnr].state);

	switch (casession[sessionnr].state)
	{
		/* may god bless you */
		case CASESSIONSTART:
		{
			debug(620, "state casessionstart");
			unsigned char tag[] = {0x9f, 0x80, 0x20}; /* appl info enq */
			sendAPDU(dvbnode, sessionnr, tag, NULL, 0);
			casession[sessionnr].state = CASESSIONFINAL;
			break;
		}
		default:
			err("unknown state");
	}
	return 0;
}

int cahostAPDU(struct dvbdev* dvbnode, int sessionnr, unsigned char *tag, void *data, int len)
{
	debug(620, "host manager cahostAPDU start");

	struct casession* casession = NULL;

	if(dvbnode == NULL || dvbnode->caslot == NULL) return 0;
	casession = dvbnode->caslot->casession;

	debug(620, "host manager %02x %02x %02x", tag[0], tag[1], tag[2]);
	
	if ((tag[0] == 0x9f) && (tag[1] == 0x84))
	{
		switch (tag[2])
		{
			case 0x00: /* tune          */
				debug(620, "should TUNE!");         
				break;                         
		case 0x01: /* replace       */
			debug(620, "should REPLACE!");         
			break;                         
		case 0x02: /* clear replace */
			debug(620, "should CLEAR!");         
			break;                         
		case 0x03: /* ask release   */
			debug(620, "should RELEASE !");         
			break;                         
		default:
			debug(620, "unknown host ctrl apdu tag %02x\n", tag[2]);
		}
	}

	return 0;
}

//mmi app functions
int cammiappaction(struct dvbdev* dvbnode, int sessionnr)
{
	struct casession* casession = NULL;

	if(dvbnode == NULL || dvbnode->caslot == NULL) return 0;
	casession = dvbnode->caslot->casession;

	debug(620, "cammiappaction nr %d, stat %d", sessionnr, casession[sessionnr].state);

	switch (casession[sessionnr].state)
	{
		case CASESSIONSTART:
		{
			debug(620, "state casessionstart");
			unsigned char tag[] = {0x9f, 0x80, 0x20}; 
			sendAPDU(dvbnode, sessionnr, tag, NULL, 0);
			casession[sessionnr].state = CASESSIONFINAL;
			break;
		}
		default:
			err("unknown state");
	}
	return 0;
}

int cammiappAPDU(struct dvbdev* dvbnode, int sessionnr, unsigned char *tag, void *data, int len)
{
	debug(620, "host manager cammiappAPDU start");

	struct casession* casession = NULL;

	if(dvbnode == NULL || dvbnode->caslot == NULL) return 0;
	casession = dvbnode->caslot->casession;

	debug(620, "host manager %02x %02x %02x", tag[0], tag[1], tag[2]);
	
	if ((tag[0] == 0x9f) && (tag[1] == 0x80)) 
	{
		switch (tag[2])
		{
		case 0x21:
			debug(620, "received tag %02x", tag[2]);
			break;
		default:
			debug(620, "unknown apdu tag %02x", tag[2]);
		}
	}

	return 0;
}

//upgrade function
int caupgradeaction(struct dvbdev* dvbnode, int sessionnr)
{
	struct casession* casession = NULL;

	if(dvbnode == NULL || dvbnode->caslot == NULL) return 0;
	casession = dvbnode->caslot->casession;

	debug(620, "caupgradeaction nr %d, stat %d", sessionnr, casession[sessionnr].state);

	switch (casession[sessionnr].state)
	{
		case CASESSIONSTART:
		{
			debug(620, "state casessionstart");
			unsigned char tag[] = {0x9f, 0x80, 0x20}; 
			sendAPDU(dvbnode, sessionnr, tag, NULL, 0);
			casession[sessionnr].state = CASESSIONFINAL;
			break;
		}
		default:
			err("unknown state");
	}
	return 0;
}

int caupgradeAPDU(struct dvbdev* dvbnode, int sessionnr, unsigned char *tag, void *data, int len)
{
	debug(620, "host manager caupgradeAPDU start");

	struct casession* casession = NULL;

	if(dvbnode == NULL || dvbnode->caslot == NULL) return 0;
	casession = dvbnode->caslot->casession;

	debug(620, "host manager %02x %02x %02x", tag[0], tag[1], tag[2]);
	
	if ((tag[0] == 0x9f) && (tag[1] == 0x9d))
	{
		switch (tag[2])
		{
		case 0x01:
			{
			debug(620, "UPGRADE REQUEST starts");
			unsigned char tag[] = {0x9f, 0x9d, 0x02}; /* cam upgrade reply */
			sendAPDU(dvbnode, sessionnr, tag, NULL, 0);
			break;
			}
		case 0x03:
			{
			debug(620, "UPGRADE REQUEST continues");
      unsigned char tag[] = {0x9f, 0x9d, 0x02}; /* cam upgrade reply */
      sendAPDU(dvbnode, sessionnr, tag, NULL, 0);
			break;
			}
		case 0x04:
			{
			debug(620, "UPGRADE REQUEST completed");
      unsigned char tag[] = {0x9f, 0x9d, 0x02}; /* cam upgrade reply */
      sendAPDU(dvbnode, sessionnr, tag, NULL, 0);
			break;
			}
		default:
			debug(620, "unknown cam upgrade apdu tag %02x", tag[2]);
		}
	}

	return 0;
}

//host_lac funktion
int cahostlacAPDU(struct dvbdev* dvbnode, int sessionnr, unsigned char *tag, void *data, int len)
{
	debug(620, "host lac manager cahostlacAPDU start");

	struct casession* casession = NULL;
	
	if(dvbnode == NULL || dvbnode->caslot == NULL) return 0;
	casession = dvbnode->caslot->casession;

	debug(620, "host lac manager %02x %02x %02x", tag[0], tag[1], tag[2]);
	
	uint8_t data_reply[4]; 

	/* check for German or French in titan settings, 
           if not found use English */	
	if(ostrcmp(getconfig("lang", NULL), "po/de") == 0)
	{
		data_reply[0] = 0x64; /* d */
		data_reply[1] = 0x65; /* e */
		data_reply[2] = 0x75; /* u */
	}
	else if(ostrcmp(getconfig("lang", NULL), "po/fr") == 0)
	{
		data_reply[0] = 0x66; /* f */
		data_reply[1] = 0x72; /* r */
		data_reply[2] = 0x61; /* a */
	}	
	else
	{
		data_reply[0] = 0x65; /* e */
		data_reply[1] = 0x6e; /* n */
		data_reply[2] = 0x67; /* g */
	}	

	if ((tag[0] == 0x9f) && (tag[1] == 0x81))
	{
		switch (tag[2])
		{
			case 0x00: /* country enquiry */
			{
				debug(620, "country answered with '%s'", data_reply);
				uint8_t tag[3] = { 0x9f, 0x81, 0x01 }; /* host country reply */
				sendAPDU(dvbnode, sessionnr, tag, data_reply, 3);
				break;
			}
		case 0x10: /* language enquiry */
		{
			debug(620, "language answered with '%s'", data_reply);
			uint8_t tag[3] = { 0x9f, 0x81, 0x11 }; /* host language reply */
			sendAPDU(dvbnode, sessionnr, tag, data_reply, 3);
			break;
		}
		default:
			debug(620, "unknown host lac apdu tag %02x", tag[2]);
		}
	}
	return 0;
}
int cahostlacaction(struct dvbdev* dvbnode, int sessionnr) 
{ 
	struct casession* casession = NULL; 
	if(dvbnode == NULL || dvbnode->caslot == NULL) return 0; 
		casession = dvbnode->caslot->casession; 

	debug(620, "cahostlacaction nr %d, stat %d", sessionnr, casession[sessionnr].state); 	
	
	switch (casession[sessionnr].state) 
	{ 
		case CASESSIONSTART: 
		{
			debug(620, "state casessionstart");
			unsigned char tag[] = {0x9f, 0x80, 0x20}; 
			sendAPDU(dvbnode, sessionnr, tag, NULL, 0);
			casession[sessionnr].state = CASESSIONFINAL;
			break;
		}
		default:
			err("unknown state");
	}
	return 0;
}	

#endif

//cc functions
int caccaction(struct dvbdev* dvbnode, int sessionnr) 
{ 
	struct casession* casession = NULL; 
	if(dvbnode == NULL || dvbnode->caslot == NULL) return 0; 
		casession = dvbnode->caslot->casession; 

	debug(620, "caccaction nr %d, stat %d", sessionnr, casession[sessionnr].state); 

	switch (casession[sessionnr].state) 
	{ 
		case CASESSIONSTART: 
		{ 
			
#ifdef MIPSEL
			//const uint8_t tag[3] = { 0x9f, 0x90, 0x02 };
			//const uint8_t data = 0x01;
			//sendAPDU(dvbnode, sessionnr, tag, &data, 1);
#endif			
			casession[sessionnr].state = CASESSIONFINAL; 
			return 0; 
		} 
		case CASESSIONFINAL: 
			printf("stateFinal und action! kann doch garnicht sein ;)\n"); 
		default: 
			return 0; 
	} 
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
#ifdef MIPSEL
				unsigned char data[][4]=
				{
					{0x00, 0x01, 0x00, 0x41},	// resource
					{0x00, 0x01, 0x00, 0x42},	// resource2
					{0x00, 0x02, 0x00, 0x41},	// application V1
					{0x00, 0x02, 0x00, 0x42},	// application V2
					{0x00, 0x02, 0x00, 0x43},	// application V3
					{0x00, 0x03, 0x00, 0x41},	// conditional access
					{0x00, 0x20, 0x00, 0x41},	// host control
					{0x00, 0x20, 0x00, 0x42},	// host control2	
					{0x00, 0x24, 0x00, 0x41},	// date-time		
					{0x00, 0x40, 0x00, 0x41},	// mmi
					{0x00, 0x41, 0x00, 0x41},	// mmi app1
					{0x00, 0x41, 0x00, 0x42},	// mmi app2
					{0x00, 0x8c, 0x10, 0x01},	// content control
#ifdef BAD
					{0x00, 0x8c, 0x10, 0x02},	// content control 
#endif
					{0x00, 0x8d, 0x10, 0x01},	// host lac
					{0x00, 0x8e, 0x10, 0x01}	// upgrade
				};
#else
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
#endif
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
			if(type == 5 && dvbnode->caslot->casession[i].ccmanager == 1 && dvbnode->caslot->casession[i].inuse == 1) //ccmanager
			{
				dvbnode->caslot->casession[i].inuse = value;
				return i;
			}
#ifdef MIPSEL
			if(type == 6 && dvbnode->caslot->casession[i].hostmanager == 1 && dvbnode->caslot->casession[i].inuse == 1) //hostmanager
			{
				dvbnode->caslot->casession[i].inuse = value;
				return i;
			}
			if(type == 7 && dvbnode->caslot->casession[i].mmiappmanager == 1 && dvbnode->caslot->casession[i].inuse == 1) //mmiappmanager
			{
				dvbnode->caslot->casession[i].inuse = value;
				return i;
			}
			if(type == 8 && dvbnode->caslot->casession[i].hostlacmanager == 1 && dvbnode->caslot->casession[i].inuse == 1) //hostlacmanager
			{
				dvbnode->caslot->casession[i].inuse = value;
				return i;
			}
			if(type == 9 && dvbnode->caslot->casession[i].upgrademanager == 1 && dvbnode->caslot->casession[i].inuse == 1) //upgrademanager
			{
				dvbnode->caslot->casession[i].inuse = value;
				return i;
			}
#endif
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
				else if(casession[sessionnr].ccmanager == 1)
					casession[sessionnr].action = caccaction(dvbnode, sessionnr);
				else if(casession[sessionnr].camanager == 1)
					casession[sessionnr].action = cacaaction(dvbnode, sessionnr);
				else if(casession[sessionnr].datetimemanager == 1)
					casession[sessionnr].action = cadatetimeaction(dvbnode, sessionnr);
				else if(casession[sessionnr].mmimanager == 1)
					casession[sessionnr].action = cammiaction(dvbnode, sessionnr);
#ifdef MIPSEL					
				else if(casession[sessionnr].hostmanager == 1)
					casession[sessionnr].action = cahostaction(dvbnode, sessionnr);	
				else if(casession[sessionnr].mmiappmanager == 1)
					casession[sessionnr].action = cammiappaction(dvbnode, sessionnr);	
				else if(casession[sessionnr].upgrademanager == 1)
					casession[sessionnr].action = caupgradeaction(dvbnode, sessionnr);	
				else if(casession[sessionnr].hostlacmanager == 1)
					casession[sessionnr].action = cahostlacaction(dvbnode, sessionnr);	
#endif
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
		case 0x00010042:
			casession[sessionnr].inuse = 1;
			casession[sessionnr].resmanager = 1;
			debug(620, "create session res manager");
			break;
		case 0x00020041:
		case 0x00020042:
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
		case 0x008c1001:
		case 0x008c1002:
			if(checkcerts())
			{
#ifdef MIPSEL
				descrambler_open();
#endif				
				casession[sessionnr].inuse = 1;
				casession[sessionnr].ccmanager = 1;
				//neutrino [session_nb - 1] = new eDVBCIContentControlManagerSession(slot);
				debug(620, "create session cc manager");
			}
			break;
		case 0x00100041:
			debug(620, "create session auth manager");
		case 0x00200041:
#ifdef MIPSEL
		case 0x00200042:
			casession[sessionnr].inuse = 1;
			casession[sessionnr].hostmanager = 1;
#endif
			debug(620, "create session host manager");
			break;
#ifdef MIPSEL
		case 0x00410041:
		case 0x00410042:
			casession[sessionnr].inuse = 1;
			casession[sessionnr].mmiappmanager = 1;
			debug(620, "create session mmi app manager");
			break;
		case 0x008e1001:
			casession[sessionnr].inuse = 1;
			casession[sessionnr].upgrademanager = 1;
			debug(620, "create session upgrade manager");
			break;
		case 0x008d1001:
			casession[sessionnr].inuse = 1;
			casession[sessionnr].hostlacmanager = 1;
			debug(620, "create session host lac manager");
			break;
#endif
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
#ifdef MIPSEL
			else if(casession->hostmanager == 1)
			{
				if(cahostAPDU(dvbnode, sessionnr, tag, pkt, alen))
					casession->action = 1;
			}
			else if(casession->mmiappmanager == 1)
			{
				if(cammiappAPDU(dvbnode, sessionnr, tag, pkt, alen))
					casession->action = 1;
			}
			else if(casession->upgrademanager == 1)
			{
				if(caupgradeAPDU(dvbnode, sessionnr, tag, pkt, alen))
					casession->action = 1;
			}
			else if(casession->hostlacmanager == 1)
			{
				if(cahostlacAPDU(dvbnode, sessionnr, tag, pkt, alen))
					casession->action = 1;
			}
#endif

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
	if(checkbox("DM7020HD") == 1 || checkbox("DM7020HDV2") == 1 || checkbox("DM900") == 1 || checkbox("DM920") == 1 || checkbox("DM520") == 1 || checkbox("DM525") == 1 || checkrealbox("HD51") == 1 || checkrealbox("HD60") == 1 || checkrealbox("HD61") == 1)
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
					// cacc start
					dvbnode->caslot->ccmgr_ready = 0;
					debug(620, "set ccmgr_ready=%d", dvbnode->caslot->ccmgr_ready);
					// cacc end
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
	oldpids[0] = 0;
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

	// cacc
	/*if(dvbnode->caslot->scrambled == 1)
	{
		if(checkbox("DM900") == 1)
			descrambler_deinit();
	}*/
	dvbnode->caslot->scrambled = 0;
	debug(620, "set scrambled=%d", dvbnode->caslot->scrambled);

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
				
/*#ifdef MIPSEL
					setciclock(dvbnode->devnr, "high");
					debug(620, "set ci clock to high -> Slot: %i", dvbnode->devnr);
#endif*/

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
			// cacc
			dvbnode->caslot->scrambled = 1;
			debug(620, "set scrambled=%d", dvbnode->caslot->scrambled);
			debug(620, "scrambled=%d ccmgr_ready=%d camanager=%d caservicenr=%d", dvbnode->caslot->scrambled, dvbnode->caslot->ccmgr_ready, caservice[caservicenr].camanager, caservicenr);
//atemio5200 camanager=6
			if(dvbnode->caslot->ccmgr_ready == 1 && (caservice[caservicenr].camanager == 5 || caservice[caservicenr].camanager == 7))
			{
#ifdef MIPSEL
				char pid_out[256] = { 0 };
				char newpid[8];
				struct esinfo* esinfonode = status.aktservice->channel->esinfo;
				
				while(esinfonode != NULL)
				{
					sprintf(newpid, " %04x", esinfonode->pid);
					if (!strstr(oldpids, newpid))  /* check if pid is not in oldpids... */
					{
 							descrambler_set_pid(0, 1, esinfonode->pid);
 							debug(620, "PID + %04x\n", esinfonode->pid);
 					}
 					sprintf(pid_out, "%s %04x ", pid_out, esinfonode->pid);
 					esinfonode = esinfonode->next;
				}
				
				debug(620, "OLD   PIDs: %s\n", oldpids);
				debug(620, "NEW   PIDs: %s\n", pid_out);
				
			 	char *token = strtok(oldpids, " ");
 				while (token != NULL)
 				{
					if (!strstr(pid_out, token))
					{
 						unsigned int pid = (int)strtol(token, NULL, 16);
 						descrambler_set_pid(0, 0, pid);
  					debug(620, "PID - %04x\n", pid);
					}
			 		token = strtok(NULL, " ");
 				}
 				
 				strcpy(oldpids,pid_out);
				
				
				/*for (i = 0; i < 8192; i++)
					descrambler_set_pid(0, 1, i); //workaround... activate all pids
					
				descrambler_set_pid(0, 1, status.aktservice->channel->pmtpid);*/	

#endif
				resendKey(dvbnode);
			}
 
			return 0;
		}
		else
			debug(620, "no free camanager found");
	}
	return 1;
}

#endif
