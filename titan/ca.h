#ifndef CA_H
#define CA_H

//wait for a module inserted
void cawait(struct stimerthread* self, struct dvbdev* dvbnode, int tout)
{
	unsigned int i = 0;
	ca_slot_info_t info;

	if(dvbnode == NULL) return;
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
				usleep(100000);
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

		if(dvbnode->caslot != NULL && flag == 0) dvbnode->caslot->fastrun = 25;
	}

	ret = dvbwrite(fd, buf, count, tout);
	if(ret >= 0 && ret == count) dvbnode->caslot->poll = 0;
	return ret;
}

//send some data on an fd, for a special slot and connid
int casend(struct dvbdev* dvbnode, unsigned char* buf, int len)
{
	unsigned char *tmpbuf = NULL;
	int flag = 0;

	if(dvbnode == NULL || dvbnode->caslot == NULL) return 1;
	tmpbuf = (unsigned char*) malloc(len + 5);
	if(tmpbuf == NULL)
	{
		err("no mem");
		return 1;
	}

	// should we send a data last ?
	if(buf != NULL)
	{
		if((buf[2] >= T_SB) && (buf[2] <= T_NEW_T_C))
			memcpy(tmpbuf, buf, len);
		else
		{
			//send data_last and data
			memcpy(tmpbuf + 5, buf, len);

			tmpbuf[0] = dvbnode->devnr;
			tmpbuf[1] = dvbnode->caslot->connid;
			tmpbuf[2] = T_DATA_LAST;
			tmpbuf[3] = len + 1; //len
			tmpbuf[4] = dvbnode->caslot->connid; //transport connection identifier
			len += 5;
		}
	}
	else
	{
		//send a data last only
		tmpbuf[0] = dvbnode->devnr;
		tmpbuf[1] = dvbnode->caslot->connid;
		tmpbuf[2] = T_DATA_LAST;
		tmpbuf[3] = len + 1; //len
		tmpbuf[4] = dvbnode->caslot->connid; //transport connection identifier
		len = 5;
		flag = 1;
	}

	struct queue* qe = addqueue(dvbnode->devnr, tmpbuf, len, NULL, 0, flag, NULL);
	free(tmpbuf); tmpbuf = NULL;
	if(qe == NULL)
	{
		err("writing data to queue, slot %d", dvbnode->devnr);
		return 1; //error
	}

	return 0; //ready
}

void sendSPDU(struct dvbdev* dvbnode, unsigned char tag, void *data, int len, int sessionnr, void *apdu, int alen)
{
	unsigned char* buf = NULL;
	unsigned char *tmpbuf = NULL;

	buf = malloc(MINMALLOC);
	if(buf == NULL)
	{
		err("no mem");
		return;
	}

	debug(620, "send SPDU, nr %d", sessionnr);

	memset(buf, 0, MINMALLOC);
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
							*tmpdata++;
					}

				}

				if(tag[2] == 0x09) //MENU
				{
					tmpstr = ostrcat(tmpstr, casession->mmititle, 1, 0);
					tmpstr = ostrcat(tmpstr, " - ", 1, 0);
					tmpstr = ostrcat(tmpstr, casession->mmisubtitle, 1, 0);
					tmpstr1 = ostrcat(tmpstr1, casession->mmitext, 1, 0);
					
					addmenulistall(&mlist, tmpstr1, NULL, 0, NULL);
					mbox = menulistbox(mlist, "menulist", tmpstr, NULL, NULL, 1, 0);
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
			case 0x31:
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
			debug(620, "create session mmi manager");
			break;
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
		default:
			debug(620, "unknown tpdu tag 0x%0x (slot %d, conn %d)", tpdutag, dvbnode->devnr, canode->connid);
	}
}

void cacheck(struct stimerthread* self, struct dvbdev* dvbnode)
{
	int ret = 0, len = MINMALLOC;
	ca_slot_info_t info;
	struct caslot* canode = NULL;
	unsigned char* buf = NULL, *tmpbuf = NULL;

	if(dvbnode == NULL) return;
	canode = dvbnode->caslot;

	buf = malloc(MINMALLOC);
	if(buf == NULL)
	{
		err("no mem");
		return;
	}
	memset(buf, 0, MINMALLOC);

	switch(canode->status)
	{
		case 0: //idle
		{
			cawait(self, dvbnode, -1);
			debug(620, "status: no, slot %d", dvbnode->devnr);

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
						dvbnode->caslot->fastrun = 25;
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
			if(ret == 0) //ready
			{
				//debug(620, "read, slot %d, ret %d", dvbnode->devnr, ret);
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
			}
			else if(ret == 1 && canode->poll == 1) //write
			{
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
			}
			else if(ret == 1 && canode->poll == 0)
			{
				debug(620, "write but poll=0, slot %d, ret %d", dvbnode->devnr, ret);
			}
			else if(ret == 2) //change
			{
				debug(620, "change, slot %d, ret %d", dvbnode->devnr, ret);
				info.num = dvbnode->devnr;

				if(cagetslotinfo(dvbnode, &info) == 1 || !(info.flags & CA_CI_MODULE_READY))
				{
					debug(620, "flags %d %d %d, slot %d", info.flags, CA_CI_MODULE_READY, info.flags & CA_CI_MODULE_READY, dvbnode->devnr);
					debug(620, "cam (slot %d) status changed, cam not present", dvbnode->devnr);

					casessionfree(dvbnode);
					caslotfree(dvbnode);
					canode->fastrun = 0;
					sleep(1);
				}
			}
			else if(ret == -2) //timeout
			{
				debug(620, "timeout slot %d, ret %d", dvbnode->devnr, ret);
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
			m_lock(&status.servicemutex, 2);
			sendcapmt(status.aktservice, 0, 2);
			m_unlock(&status.servicemutex, 2);
		}
	}

	free(buf); buf = NULL;
}

void cathread(struct stimerthread* self, struct dvbdev* dvbnode)
{
	if(dvbnode == NULL || dvbnode->caslot == NULL) return;
	debug(620, "CA thread start (slot %d)", dvbnode->devnr);

	while(self != NULL && self->aktion != STOP && self->aktion != PAUSE)
	{
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

int sendcapmttocam(struct service* node, unsigned char* buf, int len, int caservicenr)
{
	char* tmpstr = NULL;
	struct dvbdev *dvbnode = dvbdev;

	while(dvbnode != NULL)
	{
		if(dvbnode->type == CIDEV && dvbnode->fd > -1 && dvbnode->caslot != NULL && dvbnode->caslot->status == 2 && dvbnode->caslot->caids != NULL)
		{
			//check if crypt can onyl handle single service
			tmpstr = ostrcat("camtype_", oitoa(dvbnode->devnr), 0, 1);
			if(getconfigint(tmpstr, NULL) == 0 && getcaservicebyslot(dvbnode->caslot, 1) > -1)
			{
				debug(620, "cam is singel and is in use");
				free(tmpstr); tmpstr = NULL;
				return 1;
			}
			free(tmpstr); tmpstr = NULL;

			//TODO: if record is running input should not changed
			//change ciX_input and inputX
			if(node->fedev != NULL)
			{
				debug(620, "set ci slot %d to tuner %d\n", dvbnode->devnr, node->fedev->devnr);
				switch(node->fedev->devnr)
				{
					case 0:
						setciinput(dvbnode->devnr, "A");
						setcisource(dvbnode->devnr, "CI0");
						break;
					case 1:
						setciinput(dvbnode->devnr, "B");
						setcisource(dvbnode->devnr, "CI1");
						break;
					case 2:
						setciinput(dvbnode->devnr, "C");
						setcisource(dvbnode->devnr, "CI2");
						break;
					case 3:
						setciinput(dvbnode->devnr, "D");
						setcisource(dvbnode->devnr, "CI3");
						break;
				}
			}

			//got free camanager
			if(caservice[caservicenr].camanager == -1)
				caservice[caservicenr].camanager = getfreecasession(dvbnode, 2, 2);

			if(caservice[caservicenr].camanager > -1)
			{
				sendSPDU(dvbnode, 0x90, NULL, 0, caservice[caservicenr].camanager, buf, len);
				caservice[caservicenr].caslot = dvbnode->caslot;
			}
			else
			{
				debug(620, "no free camanager found");
				//int status = 0;
				//casessioncreate(dvbnode, NULL, &status, 0x00030041);
			}

			return 1;
		}
		dvbnode = dvbnode->next;
	}
	return 1;
}

#endif
