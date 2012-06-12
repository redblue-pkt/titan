#ifndef SUBTITLE_H
#define SUBTITLE

int subdisplaywidth = 720;
int subdisplayheight = 576;

static int map2to4bit[4];
static int map2to8bit[4];
static int map4to8bit[16];

//flag 0 = free oldsubpage
//flag 1 = free subpage
void subfree(int flag)
{
	struct subpage* page = NULL;
	struct subpage* tmppage = NULL;

	if(flag == 0) page = oldsubpage;
	if(flag == 1) page = subpage;

	while(page != NULL)
	{
		while(page->pageregions != NULL)
		{
			struct subpagereg *p = page->pageregions->next;
			free(page->pageregions);
			page->pageregions = p;
		}
		while(page->regions != NULL)
		{
			struct subreg *reg = page->regions;

			while(reg->objects != NULL)
			{
				struct subregobj *obj = reg->objects;
				reg->objects = obj->next;
				free(obj);
			}

			free(reg->buf);
			reg->buf = NULL;
			free(reg->palette);
			reg->palette = NULL;

			page->regions = reg->next;
			free(reg);
		}
		while(page->cluts != NULL)
		{
			struct subclut *clut = page->cluts;
			page->cluts = clut->next;
			free(clut);
		}

		tmppage = page->next;
		free(page);
		if(flag == 0) oldsubpage = NULL;
		if(flag == 1) subpage = NULL;
		page = tmppage;
	}
}

void subclear(int ontimeout)
{
	struct subpage* subnode = oldsubpage;
	struct subpagereg* pageregnode = NULL;
	time_t sec = 0;
	int stat = 0;

	if(subnode != NULL)
	{
		if(ontimeout == 1)
		{
			if(subnode->pagetimeout == 0) return;
			sec = time(NULL);
			if(subnode->pagetimeout >= sec) return;
		}

		pageregnode = subnode->pageregions;
		while(pageregnode != NULL)
		{
			struct subreg *regnode = subnode->regions;
			while(regnode != NULL)
			{
				if(regnode->regid == pageregnode->regid) break;
				regnode = regnode->next;
			}
			if(regnode != NULL)
			{
				stat = 1;
				int posx = pageregnode->scaleposx;
				int posy = pageregnode->scaleposy;

				if(posx < 0 || posy < 0) continue;
				clearrect(posx, posy, regnode->scalewidth, regnode->scaleheight);
			}
			pageregnode = pageregnode->next;
		}
	
		if(stat == 1) blitfb(0);
		subfree(0);
	}
}

void subdraw(unsigned long long subpts, struct subpage* page)
{
	debug(300, "subtitle draw");
	struct subpagereg* pageregnode = NULL;
	int y, x, stat = 0;
	uint64_t pts = 0;
	unsigned char* scalebuf = NULL;

	if(page == NULL) return;

	//wait for subtitle to display
#ifndef SIMULATE
	videogetpts(status.aktservice->videodev, &pts);
#else
	pts = 8275201296;
#endif
	debug(300, "pts = %lld, subpts = %lld", pts, subpts);
	while(status.subthreadaktion != STOP && status.subthreadaktion != PAUSE && subpts >= pts)
	{
		subclear(1);
		usleep(100000);
#ifndef SIMULATE
		videogetpts(status.aktservice->videodev, &pts);
#else
		pts += 100000;
#endif
	}
	if(status.subthreadaktion == STOP || status.subthreadaktion == PAUSE)
		return;
	debug(300, "pts = %lld, subpts = %lld", pts, subpts);

	subclear(0);

	pageregnode = page->pageregions;
	while(pageregnode != NULL)
	{

		struct subreg *regnode = page->regions;
		while(regnode != NULL)
		{
			if(regnode->regid == pageregnode->regid) break;
			regnode = regnode->next;
		}
		if(regnode != NULL && regnode->buf != NULL)
		{

			stat = 1;
			int posx = pageregnode->reghorizontaladdress * skinfb->width / subdisplaywidth;
			int posy = pageregnode->regverticaladdress * skinfb->height / subdisplayheight;
			unsigned long *palette = (unsigned long*)regnode->palette;

			pageregnode->scaleposx = posx;
			pageregnode->scaleposy = posy;

			//scale
			regnode->scalewidth = regnode->width * skinfb->width / subdisplaywidth;
			regnode->scaleheight = regnode->height * skinfb->height / subdisplayheight;

			if(accelfb != NULL && accelfb->varfbsize >= regnode->width * regnode->height * 4)
			{

				m_lock(&status.accelfbmutex, 16);
				for(y = 0; y < regnode->height; y++)
				{
					for(x = 0; x < regnode->width; x++)
					{
						if(regnode->buf[y * regnode->width + x] != 0)
							drawpixelfb(accelfb, (regnode->width * y) + x, 0, palette[regnode->buf[y * regnode->width + x]]);
						else
							drawpixelfb(accelfb, (regnode->width * y) + x, 0, 0);
					}
				}
				blitscale(posx, posy, regnode->width, regnode->height, regnode->scalewidth, regnode->scaleheight, 0);
				m_unlock(&status.accelfbmutex, 16);
			}
			else
			{
				if(regnode->scalewidth != regnode->width || regnode->scaleheight != regnode->height)
				{
					scalebuf = scale(regnode->buf, regnode->width, regnode->height, 1, regnode->scalewidth, regnode->scaleheight, 1);
					if(scalebuf != NULL) regnode->buf = scalebuf;
				}
				else
					scalebuf = regnode->buf;
/*
				//only for test no scale
				posx = pageregnode->reghorizontaladdress;
				posy = pageregnode->regverticaladdress;
				pageregnode->scaleposx = posx;
				pageregnode->scaleposy = posy;
				regnode->scalewidth = regnode->width;
				regnode->scaleheight = regnode->height;
*/

				for(y = 0; y < regnode->scaleheight; y++)
				{
					for(x = 0; x < regnode->scalewidth; x++)
					{
						if(regnode->buf[y * regnode->scalewidth + x] != 0)
						{
							drawpixel(posx + x, posy + y, palette[regnode->buf[y * regnode->scalewidth + x]]);
						}
					}
				}
			}
		}
		pageregnode = pageregnode->next;
	}
	if(stat == 1) blitfb(0);
}

void subcalc(int pageid, unsigned long long pts)
{
	int i, clutsize = 0;
	struct subpage *page = subpage;

	while(page != NULL)
	{
		if(page->pageid == pageid) break;
		page = page->next;
	}

	if(page == NULL) return;

	struct subpagereg *pagereg = page->pageregions;

	for(; pagereg; pagereg = pagereg->next)
	{
		struct subreg *reg = page->regions;
		while(reg != NULL)
		{
			if(reg->regid == pagereg->regid) break;
			reg = reg->next;
		}
		if(reg != NULL)
		{
			if(reg->committed) continue;

			int posx = pagereg->reghorizontaladdress;
			int posy = pagereg->regverticaladdress;

			if(posx < 0 || posy < 0) continue;

			/* find corresponding clut */
			struct subclut *clut = page->cluts;
			while(clut != NULL)
			{
				if(clut->clutid == reg->clutid) break;
				clut = clut->next;
			}

			if(reg->depth == 1)
			{
				clutsize = 4;
				reg->palette = (struct rgba*)malloc(clutsize * sizeof(struct rgba));
			}
			if(reg->depth == 2)
			{
				clutsize = 16;
				reg->palette = (struct rgba*)malloc(clutsize * sizeof(struct rgba));
			}
			if(reg->depth == 3)
			{
				clutsize = 256;
				reg->palette = (struct rgba*)malloc(clutsize * sizeof(struct rgba));
			}

			struct subclutentry *entries = NULL;
			switch(reg->depth)
			{
				case 1:
				{
					if(clut != NULL)
						entries = clut->entries2bit;
					memset(reg->palette, 0, 4 * sizeof(struct rgba));
					reg->palette[0].a = 0xFF;
					reg->palette[2].r = reg->palette[2].g = reg->palette[2].b = 0xFF;
					reg->palette[3].r = reg->palette[3].g = reg->palette[3].b = 0x80;
					break;
				}
				case 2:
				{
					if(clut != NULL)
						entries = clut->entries4bit;
					memset(reg->palette, 0, 16 * sizeof(struct rgba));
					for(i = 0; i < 16; ++i)
					{
						if(!i)
							reg->palette[i].a = 0xFF;
						else if(i & 8)
						{
							if(i & 1) reg->palette[i].r = 0x80;
							if(i & 2) reg->palette[i].g = 0x80;
							if(i & 4) reg->palette[i].b = 0x80;
						}
						else
						{
							if(i & 1) reg->palette[i].r = 0xFF;
							if(i & 2) reg->palette[i].g = 0xFF;
							if(i & 4) reg->palette[i].b = 0xFF;
						}
					}
					break;
				}
				case 3:
				{
					if(clut != NULL)
						entries = clut->entries8bit;
					memset(reg->palette, 0, 256 * sizeof(struct rgba));
					for(i = 0; i < 256; ++i)
					{
						switch(i & 17)
						{
							case 0:
							{
								if(!(i & 14))
								{
									if(!(i & 224))
										reg->palette[i].a = 0xFF;
									else
									{
										if(i & 128) reg->palette[i].r = 0xFF;
										if (i & 64) reg->palette[i].g = 0xFF;
										if(i & 32) reg->palette[i].b = 0xFF;
										reg->palette[i].a = 0xBF;
									}
									break;
								}
							}
							case 16:
							{
								if(i & 128) reg->palette[i].r = 0x55;
								if(i & 64) reg->palette[i].g = 0x55;
								if(i & 32) reg->palette[i].b = 0x55;
								if(i & 8) reg->palette[i].r += 0xAA;
								if(i & 4) reg->palette[i].g += 0xAA;
								if(i & 2) reg->palette[i].b += 0xAA;
								if(i & 16) reg->palette[i].a = 0x80;
								break;
							}
							case 1:
							{
								reg->palette[i].r = 0x80;
								reg->palette[i].g = 0x80;
								reg->palette[i].b = 0x80;
							}
							case 17:
							{
								if(i & 128) reg->palette[i].r += 0x2A;
								if(i & 64) reg->palette[i].g += 0x2A;
								if(i & 32) reg->palette[i].b += 0x2A;
								if(i & 8) reg->palette[i].r += 0x55;
								if(i & 4) reg->palette[i].g += 0x55;
								if(i & 2) reg->palette[i].b += 0x55;
								break;
							}

						}
						break;
					}
				}
			}

			for(i = 0; i < clutsize; ++i)
			{
				if(entries && entries[i].valid)
				{
					int y = entries[i].Y;
					int cr = entries[i].Cr;
					int cb = entries[i].Cb;

					if(y > 0)
					{
						y -= 16;
						cr -= 128;
						cb -= 128;
						reg->palette[i].r = OMAX(OMIN(((298 * y            + 460 * cr) / 256), 255), 0);
						reg->palette[i].g = OMAX(OMIN(((298 * y -  55 * cb - 137 * cr) / 256), 255), 0);
						reg->palette[i].b = OMAX(OMIN(((298 * y + 543 * cb           ) / 256), 255), 0);
						//reg->palette[i].a = (entries[i].T) & 0xFF;
						reg->palette[i].a = 0xFF;
					}
					else
					{
						reg->palette[i].r = 0;
						reg->palette[i].g = 0;
						reg->palette[i].b = 0;
						reg->palette[i].a = 0xFF;
					}
				}
			}

			reg->committed = 1;
		}
	}
	page->pagetimeout += time(NULL);
	subdraw(pts, page);
	subfree(0);
	oldsubpage = subpage;
	subpage = NULL;
}

void subcalcall(unsigned long long pts)
{
#if 1
	struct subpage *page = subpage;

	while(page != NULL)
	{
		if(page->state != 0)
			subcalc(page->pageid, pts);
		page = page->next;
	}
#else
	struct subpage *page = subpage;

	debug(300, "end of display set");
	debug(300, "active pages:");
	while(page != NULL)
	{
		debug(300, "page_id %02x", page->pageid);
		debug(300, "page_version_number: %d", page->pageversionnumber);
		debug(300, "active regions:");
		{
			subpagereg *reg = page->pageregions;
			while(reg != NULL)
			{
				debug(300, "region_id: %04x", reg->regid);
				debug(300, "region_horizontal_address: %d", reg->reghorizontaladdress);
				debug(300, "region_vertical_address: %d", reg->regverticaladdress);

				reg = reg->next;
			}
		}

		subcalc(page->pageid);

		debug(300, "defined regions:");
		struct subregion *reg = page->regions;
		while(reg != NULL)
		{
			debug("region_id %04x, version %d, %dx%d", reg->regid, reg->versionnumber, reg->width, reg->height);

			struct subregobj *obj = reg->objects;
			while(object != NULL)
			{
				debug(300, "object %02x, type %d, %d:%d", obj->objid, obj->objtype, obj->objhorizontalpos, obj->objverticalpos);
				obj = obj->next;
			}
			reg = reg->next;
		}
		page = page->next;
	}
#endif
}

void bitstreaminit(struct bitstream *bit, const void *buf, int size)
{
	bit->data = (unsigned char*) buf;
	bit->size = size;
	bit->avail = 8;
	bit->consumed = 0;
}

int bitstreamget(struct bitstream *bit)
{
	int val;
	bit->avail -= bit->size;
	val = ((*bit->data) >> bit->avail) & ((1 << bit->size) - 1);
	if(!bit->avail)
	{
		bit->data++;
		bit->consumed++;
		bit->avail = 8;
	}
	return val;
}

int verifysubpes(unsigned char* pkt)
{
	int substart, pktlen;

	if(pkt == NULL)
	{
		debug(300, "NULL Paket");
		return 1;
	}

	/* Packet header == 0x000001 */
	if(pkt[0] != 0x00 || pkt[1] != 0x00 || pkt[2] != 0x01)
	{
		debug(300, "Invalid header");
		return 1;
	}

	/* stream_id == private_stream_1 */
	if(pkt[3] != 0xbd)
	{
		debug(300, "Not a private_stream_1 stream (%x)\n", pkt[3]);
		return 1;
	}

	/* data_alignment_indicator == 1 */
	if((pkt[6] & 0x04) == 0)
	{
		debug(300, "Data alignment indicator == 0");
		return 1;
	}

	/* PTS is present */
	if((pkt[7] & 0x80) == 0)
	{
		debug(300, "No PTS");
		return 1;
	}

	substart = 9 + pkt[8];
	pktlen = ((uint16_t)pkt[4] << 8) + pkt[5] + 6;

	/* data_identifier == 0x20, subtitle_stream_id == 0x00,
	end_of_PES_data_field_marker == 0xff */
	if(pkt[substart] != 0x20)
	{
		debug(300, "Data identifier != 0x20");
		return 1;
	}

	if(pkt[substart + 1] != 0x00)
	{
		debug(300, "Subtitle stream ID %x != 0x00\n", pkt[substart + 1]);
		return 1;
	}

	if(pkt[pktlen - 1] != 0xff)
	{
		debug(300, "Wrong packet length");
		return 1;
	}

	return 0;
}

int subgetpts(unsigned long long *pts, unsigned char *pkt)
{
	pkt += 7;
	int flags = *pkt++;

	pkt++; // header length

	if(flags & 0x80) /* PTS present? */
	{
		*pts = ((unsigned long long)(((pkt[0] >> 1) & 7))) << 30;
		*pts |= pkt[1] << 22;
	 	*pts |= (pkt[2] >> 1) << 15;
		*pts |= pkt[3] << 7;
		*pts |= (pkt[5] >> 1);

		return 0;
	} else
		return 1;
}

void subline(struct subreg *reg, struct subregobj *obj, int line, unsigned char *data, int len)
{
	int x = obj->objhorizontalpos;
	int y = obj->objverticalpos + line;

	if(x + len > reg->width) len = reg->width - x;
	if(len < 0) return;
	if(y >= reg->height) return;

	memcpy(reg->buf + reg->width * y + x, data, len);
}

int subpixeldata(struct subreg *reg, struct subregobj *obj, int *linenr, int *linep, unsigned char *data)
{
	int datatype = *data++, i = 0;
	static unsigned char line[1920];
	struct bitstream bit;

	bit.size = 0;
	switch(datatype)
	{
		case 0x10: // 2bit pixel data
		{
			bitstreaminit(&bit, data, 2);
			while(1)
			{
				int len = 0, col = 0;
				int code = bitstreamget(&bit);

				if(code)
				{
					col = code;
					len = 1;
				}
				else
				{
					code = bitstreamget(&bit);
					if(!code)
					{
						code = bitstreamget(&bit);
						if(code == 1)
						{
							col = 0;
							len = 2;
						}
						else if(code == 2)
						{
							len = bitstreamget(&bit) << 2;
							len |= bitstreamget(&bit);
							len += 12;
							col = bitstreamget(&bit);
						}
						else if(code == 3)
						{
							len = bitstreamget(&bit) << 6;
							len |= bitstreamget(&bit) << 4;
							len |= bitstreamget(&bit) << 2;
							len |= bitstreamget(&bit);
							len += 29;
							col = bitstreamget(&bit);
						}
						else
							break;
					}
					else if(code==1)
					{
						col = 0;
						len = 1;
					}
					else if(code & 2)
					{
						if(code & 1)
							len = 3 + 4 + bitstreamget(&bit);
						else
							len = 3 + bitstreamget(&bit);
						col = bitstreamget(&bit);
					}
				}

				uint8_t c = reg->depth == 1 ? map2to4bit[col] : reg->depth == 2 ? map2to8bit[col] : col;
				while(len && ((*linep) < subdisplaywidth))
				{
					line[(*linep)++] = c;
					len--;
				}
			}
			while(bit.avail != 8)
				bitstreamget(&bit);
			return bit.consumed + 1;
		}
		case 0x11: // 4bit pixel data
		{
			bitstreaminit(&bit, data, 4);
			while(1)
			{
				int len = 0, col = 0;
				int code = bitstreamget(&bit);
				if(code)
				{
					col = code;
					len = 1;
				}
				else
				{
					code = bitstreamget(&bit);
					if(!code)
						break;
					else if(code == 0xC)
					{
						col = 0;
						len = 1;
					}
					else if(code == 0xD)
					{
						col = 0;
						len = 2;
					}
					else if(code < 8)
					{
						col = 0;
						len = (code & 7) + 2;
					}
					else if((code & 0xC) == 0x8)
					{
						col = bitstreamget(&bit);
						len = (code & 3) + 4;
					}
					else if(code == 0xE)
					{
						len = bitstreamget(&bit) + 9;
						col = bitstreamget(&bit);
					}
					else if(code == 0xF)
					{
						len = bitstreamget(&bit) << 4;
						len |= bitstreamget(&bit);
						len += 25;
						col = bitstreamget(&bit);
					}
				}
				uint8_t c = reg->depth == 3 ? map4to8bit[col] : col;
				while(len && ((*linep) < subdisplaywidth))
				{
					line[(*linep)++] = c;
					len--;
				}
			}
			while(bit.avail != 8)
				bitstreamget(&bit);
			return bit.consumed + 1;
		}
		case 0x12: // 8bit pixel data
		{
			bitstreaminit(&bit, data, 8);
			while(1)
			{
				int len = 0, col = 0;
				int code = bitstreamget(&bit);
				if(code)
				{
					col = code;
					len = 1;
				}
				else
				{
					code = bitstreamget(&bit);
					if((code & 0x80) == 0x80)
					{
						len = code & 0x7F;
						col = bitstreamget(&bit);
					}
					else if(code & 0x7F)
					{
						len = code & 0x7F;
						col = 0;
					}
					else
						break;
				}
				while(len && ((*linep) < subdisplaywidth))
				{
					line[(*linep)++] = col;
					len--;
				}
			}
			return bit.consumed + 1;
		}
		case 0x20:
		{
			bitstreaminit(&bit, data, 4);
			for(i = 0; i < 4; ++i)
				map2to4bit[i] = bitstreamget(&bit);
			return bit.consumed + 1;
		}
		case 0x21:
		{
			bitstreaminit(&bit, data, 8);
			for(i = 0; i < 4; ++i)
				map2to8bit[i] = bitstreamget(&bit);
			return bit.consumed + 1;
		}
		case 0x22:
		{
			bitstreaminit(&bit, data, 8);
			for(i = 0; i < 16; ++i)
			map4to8bit[i] = bitstreamget(&bit);
			return bit.consumed + 1;
		}
		case 0xF0:
		{
			subline(reg, obj, *linenr, line, *linep);
			(*linenr) += 2; // interlaced
			*linep = 0;
			return 1;
		}
		default:
		{
			debug(300, "subtitle_process_pixel_data: invalid data_type %02x", datatype);
			return -1;
		}
	}
	return 0;
}

int subsegment(unsigned char *seg, int id1, int id2, unsigned long long pts)
{
	int segtype, pageid, seglen, proclen;

	if(*seg++ != 0x0F)
	{
		debug(300, "segment out of sync");
		return -1;
	}

	segtype = *seg++;
	pageid  = *seg++ << 8;
	pageid |= *seg++;
	seglen = *seg++ << 8;
	seglen |= *seg++;

	if(segtype == 0xFF) return seglen + 6;

	if(pageid != id1 && pageid != id2)
		return seglen + 6;

	struct subpage *page = subpage, **ppage = &subpage;

	while(page != NULL)
	{
		if(page->pageid == pageid) break;
		page = page->next;
	}

	proclen = 0;

	switch(segtype)
	{
		case 0x10: // page composition segment
		{
			int pagetimeout = *seg++; proclen++;
			int pageversionnumber = *seg >> 4;
			int pagestate = (*seg >> 2) & 0x3;
			seg++;
			proclen++;

			if(page == NULL)
			{
				page = malloc(sizeof(struct subpage));
				if(page == NULL)
				{
					err("no mem");
					break;
				}
				memset(page, 0, sizeof(struct subpage));
				page->pageid = pageid;
				*ppage = page;
			}
			else
			{
				if(page->pcssize != seglen)
					page->pageversionnumber = -1;
				// if no update, just skip this data.
				if(page->pageversionnumber == pageversionnumber)
					break;
			}

			page->state = pagestate;

			if((pagestate == 1) || (pagestate == 2))
			{
				while(page->pageregions != NULL)
				{
					struct subpagereg *p = page->pageregions->next;
					free(page->pageregions);
					page->regions = NULL;
					page->pageregions = p;
				}
				while(page->regions)
				{
					struct subreg *p = page->regions->next;
					while(page->regions->objects)
					{
						struct subregobj *obj = page->regions->objects->next;
						free(page->regions->objects);
						page->regions->objects = NULL;
						page->regions->objects = obj;
					}
					free(page->regions);
					page->regions = NULL;
					page->regions = p;
				}

			}

			page->pagetimeout = pagetimeout;
			page->pageversionnumber = pageversionnumber;

			struct subpagereg **reg = &page->pageregions;

			while(*reg != NULL) reg = &(*reg)->next;

			if(proclen == seglen && !page->pageregions)
			{
				debug(300, "no regions in page.. clear screen!!");
				//TODO: ??
				//subcalc(page->pageid);
			}

			while(proclen < seglen)
			{
				struct subpagereg *preg;

				// append new entry to list
				preg = malloc(sizeof(struct subpagereg));
				if(preg == NULL)
				{
					err("no mem");
					proclen += 6;
					continue;
				}
				memset(preg, 0, sizeof(struct subpagereg));
				*reg = preg;
				reg = &preg->next;

				preg->regid = *seg++; proclen++;
				seg++; proclen++;

				preg->reghorizontaladdress = *seg++ << 8;
				preg->reghorizontaladdress |= *seg++;
				proclen += 2;

				preg->regverticaladdress = *seg++ << 8;
				preg->regverticaladdress |= *seg++;
				proclen += 2;
			}

			if(proclen != seglen)
				debug(300, "proclen %d != seglen %d", proclen, seglen);
			break;
		}
		case 0x11: // region composition segment
		{
			int regid = *seg++; proclen++;
			int versionnumber = *seg >> 4;
			int regfillflag = (*seg >> 3) & 1;
			seg++; proclen++;

			if(page == NULL)
			{
				debug(300, "ignoring region %x, since page %02x doesn't yet exist.", regid, pageid);
				break;
			}

			struct subreg *reg = page->regions, **preg = &page->regions;

			while(reg != NULL)
			{
				if(reg->regid == regid) break;
				preg = &reg->next;
				reg = reg->next;
			}

			if(reg == NULL)
			{
				*preg = reg = malloc(sizeof(struct subreg));
				if(reg == NULL)
				{
					err("no mem");
					break;
				}
				memset(reg, 0, sizeof(struct subreg));
				reg->committed = 0;
			}
			else if(reg->versionnumber != versionnumber)
			{
				struct subregobj *obj = reg->objects;
				while(obj != NULL)
				{
					struct subregobj *n = obj->next;
					free(obj); obj = NULL;
					obj = n;
				}
				free(reg->buf);
				reg->buf = NULL;
				free(reg->palette);
				reg->palette = NULL;

				reg->committed = 0;
			}
			else
				break;

			reg->regid = regid;
			reg->versionnumber = versionnumber;

			reg->width = *seg++ << 8;
			reg->width |= *seg++;
			proclen += 2;

			reg->height = *seg++ << 8;
			reg->height |= *seg++;
			proclen += 2;

			reg->buf = malloc(reg->width * reg->height);
			if(reg->buf == NULL)
			{
				err("no mem");
				break;
			}
			memset(reg->buf, 0, reg->width * reg->height);

			//int reglevelofcompatibility = (*seg >> 5) & 7;

			reg->depth = (*seg++ >> 2) & 7;
			proclen++;

			reg->clutid = *seg++; proclen++;

			int reg8bitpixel = *seg++; proclen++;
			int reg4bitpixel = *seg >> 4;
			int reg2bitpixel = (*seg++ >> 2) & 3;
			proclen++;

			if(regfillflag == 0)
			{
				reg2bitpixel = reg4bitpixel = reg8bitpixel = 0;
				regfillflag = 1;
			}

			if(regfillflag != 0)
			{
				if(reg->depth == 1)
					memset(reg->buf, reg2bitpixel, reg->height * reg->width);
				else if(reg->depth == 2)
					memset(reg->buf, reg4bitpixel, reg->height * reg->width);
				else if(reg->depth == 3)
					memset(reg->buf, reg8bitpixel, reg->height * reg->width);
				else
					debug(300, "invalid depth");
			}

			reg->objects = 0;
			struct subregobj **pobj = &reg->objects;

			while(proclen < seglen)
			{
				struct subregobj *obj;

				obj = malloc(sizeof(struct subregobj));
				if(obj == NULL)
				{
					err("no mem");
					proclen += 8;
					continue;
				}
				memset(obj, 0, sizeof(struct subregobj));

				*pobj = obj;
				pobj = &obj->next;

				obj->objid = *seg++ << 8;
				obj->objid |= *seg++; proclen += 2;

				obj->objtype = *seg >> 6;
				obj->objproviderflag = (*seg >> 4) & 3;
				obj->objhorizontalpos = (*seg++ & 0xF) << 8;
				obj->objhorizontalpos |= *seg++;
				proclen += 2;

				obj->objverticalpos = *seg++ << 8;
				obj->objverticalpos |= *seg++ ;
				proclen += 2;

				if((obj->objtype == 1) || (obj->objtype == 2))
				{
					obj->foregroundpixel = *seg++;
					obj->backgroundpixel = *seg++;
					proclen += 2;
				}
			}

			if(proclen != seglen)
				debug(300, "too less data! (%d < %d)", seglen, proclen);

			break;
		}
		case 0x12: // CLUT definition segment
		{
			int clutid, clutversionnumber;
			struct subclut *clut, **pclut;

			if(page == NULL) break;

			clutid = *seg++;
			clutversionnumber = *seg++ >> 4;
			proclen += 2;

			clut = page->cluts; pclut = &page->cluts;

			while(clut != NULL)
			{
				if(clut->clutid == clutid) break;
				pclut = &clut->next;
				clut = clut->next;
			}

			if(clut == NULL)
			{
				*pclut = clut = malloc(sizeof(struct subclut));
				if(clut == NULL)
				{
					err("no mem");
					break;
				}
				memset(clut, 0, sizeof(struct subclut));
				clut->clutid = clutid;
			}
			else if(clut->clutversionnumber == clutversionnumber)
				break;

			clut->clutversionnumber = clutversionnumber;

			memset(clut->entries2bit, 0, sizeof(clut->entries2bit));
			memset(clut->entries4bit, 0, sizeof(clut->entries4bit));
			memset(clut->entries8bit, 0, sizeof(clut->entries8bit));

			while(proclen < seglen)
			{
				int clutentryid, entryclutflag, fullrange;
				int Y, Cr, Cb, T;

				clutentryid = *seg++;
				fullrange = *seg & 1;
				entryclutflag = (*seg++ & 0xE0) >> 5;
				proclen += 2;

				if(fullrange)
				{
					Y = *seg++;
					Cr = *seg++;
					Cb = *seg++;
					T = *seg++;
					proclen += 4;
				}
				else
				{
					Y = *seg & 0xFC;
					Cr = (*seg++ & 3) << 6;
					Cr |= (*seg & 0xC0) >> 2;
					Cb = (*seg & 0x3C) << 2;
					T = (*seg++ & 3) << 6;
					proclen += 2;
				}

				if(entryclutflag & 1) // 8bit
				{
					clut->entries8bit[clutentryid].Y = Y;
					clut->entries8bit[clutentryid].Cr = Cr;
					clut->entries8bit[clutentryid].Cb = Cb;
					clut->entries8bit[clutentryid].T = T;
					clut->entries8bit[clutentryid].valid = 1;
				}
				if(entryclutflag & 2) // 4bit
				{
					if(clutentryid < 16)
					{
						clut->entries4bit[clutentryid].Y = Y;
						clut->entries4bit[clutentryid].Cr = Cr;
						clut->entries4bit[clutentryid].Cb = Cb;
						clut->entries4bit[clutentryid].T = T;
						clut->entries4bit[clutentryid].valid = 1;
					}
					else
						debug(300, "CLUT entry marked as 4 bit with id %d (>15)", clutentryid);
				}
				if(entryclutflag & 4) // 2bit
				{
					if(clutentryid < 4)
					{
						clut->entries2bit[clutentryid].Y = Y;
						clut->entries2bit[clutentryid].Cr = Cr;
						clut->entries2bit[clutentryid].Cb = Cb;
						clut->entries2bit[clutentryid].T = T;
						clut->entries2bit[clutentryid].valid = 1;
					}
					else
						debug(300, "CLUT entry marked as 2 bit with id %d (>3)", clutentryid);
				}
			}
			break;
		}
		case 0x13: // object data segment
		{
			int objid, objversionnumber, objcodingmethod, nonmodifyingcolorflag;

			if(page == NULL)
			{
				debug(300, "no page for object data found");
				break;
			}

			objid = *seg++ << 8;
			objid |= *seg++;
			proclen += 2;

			objversionnumber = *seg >> 4;
			objcodingmethod = (*seg >> 2) & 3;
			nonmodifyingcolorflag = (*seg++ >> 1) & 1;
			proclen++;

			struct subreg *reg = page->regions;
			while(reg != NULL)
			{
				struct subregobj *obj = reg->objects;
				while(obj != NULL)
				{
					if(obj->objid == objid)
					{
						if(objcodingmethod == 0)
						{
							int topfielddatablocklen, bottomfielddatablocklen;
							int i = 1, line, linep;

							topfielddatablocklen = *seg++ << 8;
							topfielddatablocklen |= *seg++;
							bottomfielddatablocklen = *seg++ << 8;
							bottomfielddatablocklen |= *seg++;
							proclen += 4;

							map2to4bit[0] = 0;
							map2to4bit[1] = 8;
							map2to4bit[2] = 7;
							map2to4bit[3] = 15;

							// this map is realy untested...
							map2to8bit[0] = 0;
							map2to8bit[1] = 0x88;
							map2to8bit[2] = 0x77;
							map2to8bit[3] = 0xff;

							map4to8bit[0] = 0;
							for(; i < 16; ++i)
								map4to8bit[i] = i * 0x11;
	
							i = 0; line = 0; linep = 0;
							while(i < topfielddatablocklen)
							{
								int len;
								len = subpixeldata(reg, obj, &line, &linep, seg);
								if(len < 0) return -1;
								seg += len;
								proclen += len;
								i += len;
							}

							line = 1; linep = 0;
							if(bottomfielddatablocklen)
							{
								i = 0;
								while(i < bottomfielddatablocklen)
								{
									int len;
									len = subpixeldata(reg, obj, &line, &linep, seg);
									if(len < 0) return -1;
									seg += len;
									proclen += len;
									i += len;
								}
							}
							else if(topfielddatablocklen)
								debug(300, "unimplemented: no bottom field! (%d : %d)", topfielddatablocklen, bottomfielddatablocklen);
	
							if((topfielddatablocklen + bottomfielddatablocklen) & 1)
							{
								seg++; proclen++;
							}
						}
						else if(objcodingmethod == 1)
							debug(300, "object_coding_method 1 unsupported!");
					}
					obj = obj->next;
				}
				reg = reg->next;
			}
			break;
		}
		case 0x14: // display definition segment
		{
			if(seglen > 4)
			{
				//int ddsversionnumber = seg[0] >> 4;
				int displaywindowflag = (seg[0] >> 3) & 1;
				int displaywidth = (seg[1] << 8) | (seg[2]);
				int displayheight = (seg[3] << 8) | (seg[4]);

				proclen += 5;
				subdisplaywidth = displaywidth + 1;
				subdisplayheight = displayheight +1;

				if(displaywindowflag)
				{
					if(seglen > 12)
					{
						//int displaywindowhorizontalposmin = (seg[4] << 8) | seg[5];
						//int displaywindowhorizontalposmax = (seg[6] << 8) | seg[7];
						//int displaywindowverticalposmin = (seg[8] << 8) | seg[9];
						//int displaywindowverticalposmax = (seg[10] << 8) | seg[11];
						proclen += 8;
					}
					else
						debug(300, "display window flag set but display definition segment to short %d!", seglen);
				}
			}
			else
				debug(300, "display definition segment to short %d!", seglen);
			break;
		}
		case 0x80: // end of display set segment
		{
			subcalcall(pts);
		}
		case 0xFF: // stuffing
			break;
		default:
			debug(300, "unhandled segment type %02x", segtype);
	}

	return seglen + 6;
}

void subpes(unsigned char *pkt, int len, int id1, int id2)
{
	int ret = 0;
	unsigned long long pts = 0;

	ret = verifysubpes(pkt);
	if(ret != 0) return;

	ret = subgetpts(&pts, pkt);
	if(ret == 0)
	{
		pkt += 6; len -= 6;
		// skip PES header
		pkt++; len--;
		pkt++; len--;

		int hdrlen = *pkt++; len--;
		pkt += hdrlen; len -= hdrlen;

		pkt++; len--; // data identifier
		pkt++; len--; // stream id;

		if(len <= 0) return;

		while(len && *pkt == 0x0F)
		{
			int l = subsegment(pkt, id1, id2, pts);
			if(l < 0) break;
			pkt += l;
			len -= l;
		}

		if(len && *pkt != 0xFF) debug(300, "strange data at the end");

		subcalcall(pts);
	}
}

void* subthreadfunc(void *param)
{
	debug(1000, "in");
	struct dvbdev* dmxsubnode = NULL;
	int count = 0, len = 0, packlen = 0;
	unsigned char* buf = NULL;

	struct subtitle* subnode = (struct subtitle*)param;

	if(subnode == NULL || subnode->pid < 1)
	{
		debug(1000, "out -> NULL detect");
		pthread_exit(NULL);
	}

	dmxsubnode = dmxopen(status.aktservice->fedev);
	if(dmxsubnode == NULL)
	{
		err("no demux dev");
		pthread_exit(NULL);
	}
	dmxsetbuffersize(dmxsubnode,  128 * 1024);
	if(status.aktservice->fedev != NULL)
		dmxsetsource(dmxsubnode, status.aktservice->fedev->fedmxsource);
	else
		err("NULL detect");
	dmxsetpesfilter(dmxsubnode, subnode->pid, -1, DMX_OUT_TAP, DMX_PES_OTHER, 0);

#ifdef SIMULATE
	int fd = open("simulate/dvbsubtitle.ts", O_RDONLY);
	if(fd == -1)
	{
		perr("open simulate/dvbsubtitle.ts");
		pthread_exit(NULL);
	}
#endif

	buf = malloc(16);
	if(buf == NULL)
	{
		err("no mem");
		pthread_exit(NULL);
	}

	status.aktservice->dmxsubtitledev = dmxsubnode;

	debug(300, "start subtitle thread");
	status.subthreadstatus = ACTIVE;
	while(status.subthreadaktion != STOP)
	{
		if(status.subthreadaktion == PAUSE)
		{
			status.subthreadstatus = INPAUSE;
			usleep(100000);
			continue;
		}
		status.subthreadstatus = ACTIVE;
		count = 0;
		while(status.subthreadaktion != STOP && status.subthreadaktion != PAUSE)
		{
			debug(300, "sync subtitle");
#ifdef SIMULATE
			len = TEMP_FAILURE_RETRY(read(fd, &buf[count], 3 - count));
#else
			len = dvbread(dmxsubnode, &buf[count], 0, 3 - count, 100 * 1000);
#endif
			subclear(1);
			if(len < 0) continue;
			if(len == 3 - count)
			{
				if(buf[0] == 0x00 && buf[1] == 0x00 && buf[2] == 0x01)
				{
					count = 3;
					break;
				}
				buf[0] = buf[1];
				buf[1] = buf[2];
				count = 2;
				continue;
			}
			count = count + len;
		}

		while(status.subthreadaktion != STOP && status.subthreadaktion != PAUSE && count < 6)
		{
#ifdef SIMULATE
			len = TEMP_FAILURE_RETRY(read(fd, &buf[count], 6 - count));
#else
			len = dvbread(dmxsubnode, &buf[count], 0, 6 - count, 100 * 1000);
#endif
			subclear(1);
			if(len < 0) continue;
			count = count + len;
		}
		debug(300, "subtitle read peslen len=%d", len);

		packlen = (((uint16_t)buf[4] << 8) | buf[5]) + 6;
		buf = realloc(buf, packlen);

		while(status.subthreadaktion != STOP && status.subthreadaktion != PAUSE && count < packlen)
		{
#ifdef SIMULATE
			len = TEMP_FAILURE_RETRY(read(fd, &buf[count], packlen - count));
#else
			len = dvbread(dmxsubnode, &buf[count], 0, packlen - count, 100 * 1000);
#endif
			subclear(1);
			if(len <= 0) continue;
			count = count + len;

			if(count == packlen)
			{
				debug(300, "decode subtile len=%d", count);
				subpes(buf, count, subnode->id1, subnode->id2);
			}
		}
	}

	debug(300, "end subtitle thread");
	
	dmxclose(status.aktservice->dmxsubtitledev, -1);
	status.aktservice->dmxsubtitledev = NULL;

	free(buf);

#ifdef SIMULATE
	close(fd);
#endif

	status.subthreadstatus = DEACTIVE;
	subfree(0);
	subfree(1);

	debug(1000, "out");
	pthread_exit(NULL);
}

int subtitlestart(struct subtitle* node)
{
	int ret;

	if(status.subthreadstatus != DEACTIVE)
	{
		err("subtitle thread in use");
		return 1;
	}

	status.subthreadaktion = START;
	pthread_attr_destroy(&status.subthreadattr);
	pthread_attr_init(&status.subthreadattr);
	pthread_attr_setstacksize(&status.subthreadattr, 50000);
	pthread_attr_setdetachstate(&status.subthreadattr, PTHREAD_CREATE_JOINABLE);
	ret = pthread_create(&status.subthread, &status.subthreadattr, subthreadfunc, node);
	if(ret)
	{
		err("create subtitle thread");
		return 1;
	}

	return 0;
}

int subtitlepause(int flag)
{
	int i = 0;

	if(flag == 0)
	{
		if(status.subthreadstatus == INPAUSE) 
			status.subthreadaktion = START;
	}
	else if(status.subthreadstatus == ACTIVE)
	{
		status.subthreadaktion = PAUSE;
		while(status.subthreadstatus != INPAUSE)
		{
			usleep(100000);
			i++; if(i > 20) break;
		}
		if(i > 20)
			err("detect hanging subthread");
		subclear(0);
	}

	return 0;
}

int subtitlestop(int flag)
{
	void* threadstatus;
	int i = 0;

	status.subthreadaktion = STOP;
	while(status.subthreadstatus != DEACTIVE)
	{
		usleep(100000);
		i++; if(i > 20) break;
	}
	subclear(0);
	
	if(i > 20)
	{
		err("detect hanging subthread");
	}
	else if(status.subthread != 0)
		pthread_join(status.subthread, &threadstatus);

	status.subthread = 0;
	pthread_attr_destroy(&status.subthreadattr);
	if(flag == 0)
		status.subthreadpid = 0;

	return 0;
}

struct subtitle* checksubtitle(struct channel* chnode, struct subtitle* strack)
{
	struct subtitle* node = NULL;

	if(chnode != NULL)
	{
		node = chnode->subtitle;
		while(node != NULL)
		{
			if(node == strack)
				return node;
			node = node->next;
		}
	}
	return NULL;
}

void screensubtitle()
{
	int rcret = 0, treffer = 0;
	struct skin* subtitle = getscreen("subtitle");
	struct skin* listbox = getscreennode(subtitle, "listbox");
	struct skin* tmp = NULL;
	struct subtitle* node = NULL;

	listbox->aktline = 1;
	listbox->aktpage = -1;

	if(status.aktservice->channel != NULL)
	{
		m_lock(&status.subtitlemutex, 8);
		node = status.aktservice->channel->subtitle;
		while(node != NULL)
		{
			tmp = addlistbox(subtitle, listbox, tmp, 1);
			if(tmp != NULL)
			{
				changetext(tmp, _(node->name));
				tmp->type = CHOICEBOX;
				tmp->del = 1;
				tmp->handle = (char*)node;

				if(node->pid == status.subthreadpid)
				{
					changeinput(tmp, _("running"));
					treffer = 1;
				}
				else
					changeinput(tmp, "");

				if(treffer == 0) listbox->aktline++;
			}
			node = node->next;
		}	
		m_unlock(&status.subtitlemutex, 8);
	}

	if(treffer == 0) listbox->aktline = 1;

	drawscreen(subtitle, 0, 0);
	addscreenrc(subtitle, listbox);

	while(1)
	{
		rcret = waitrc(subtitle, 0, 0);
	
		if(rcret == getrcconfigint("rcexit", NULL)) break;
		if(rcret == getrcconfigint("rcok", NULL))
		{
			if(listbox->select != NULL && listbox->select->handle != NULL)
			{
				subtitlestop(1);
				m_lock(&status.subtitlemutex, 8);
				if(checksubtitle(status.aktservice->channel, (struct subtitle*)listbox->select->handle) != NULL)
				{
					if(((struct subtitle*)listbox->select->handle)->pid != status.subthreadpid)
					{
						clearscreen(subtitle);
						drawscreen(skin, 0, 0);
						if(subtitlestart((struct subtitle*)listbox->select->handle) == 0)
							status.subthreadpid = ((struct subtitle*)listbox->select->handle)->pid;
					}
					else
						status.subthreadpid = 0;
				}
				else
					status.subthreadpid = 0;
				m_unlock(&status.subtitlemutex, 8);
			}
			break;
		}
	}

	delmarkedscreennodes(subtitle, 1);
	delownerrc(subtitle);
	clearscreen(subtitle);
}

struct subtitle* addsubtitle(struct channel* chnode, int subtype, char* langdesc, int pid, int type, int id1, int id2, struct subtitle* last)
{
	debug(1000, "in");
	struct subtitle *newnode = NULL, *prev = NULL, *node = NULL;
	char *tmpstr = NULL, *tmpnr = NULL;

	if(chnode == NULL)
	{
		debug(1000, "out -> NULL detect");
		return NULL;
	}

	newnode = (struct subtitle*)malloc(sizeof(struct subtitle));	
	if(newnode == NULL)
	{
		err("no memory");
		return NULL;
	}

	memset(newnode, 0, sizeof(struct subtitle));

	newnode->pid = pid;
	newnode->type = type;
	newnode->id1 = id1;
	newnode->id2 = id2;

	if(ostrcmp(langdesc, "und") == 0)
		tmpstr = ostrcat(tmpstr, _("undefined"), 1, 0);
	else
		tmpstr = ostrcat(tmpstr, _(langdesc), 1, 0);
	if(subtype == 1)
	{
		tmpstr = ostrcat(tmpstr, " (", 1, 0);
		tmpstr = ostrcat(tmpstr, "TXT Subtitle", 1, 0);
		tmpstr = ostrcat(tmpstr, " - ", 1, 0);
		tmpnr = oitoa(id2);
		tmpstr = ostrcat(tmpstr, tmpnr, 1, 1);
		tmpstr = ostrcat(tmpstr, ")", 1, 0);
	}
	if(subtype == 2) 
	{
		tmpstr = ostrcat(tmpstr, " (", 1, 0);
		tmpstr = ostrcat(tmpstr, "DVB Subtitle", 1, 0);
		tmpstr = ostrcat(tmpstr, ")", 1, 0);
	}
	newnode->name = tmpstr;

	m_lock(&status.subtitlemutex, 8);
	node = chnode->subtitle;
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
		chnode->subtitle = newnode;
	else
		prev->next = newnode;

	newnode->next = node;

	m_unlock(&status.subtitlemutex, 8);
	debug(1000, "out");
	return newnode;
}

void freesubtitle(struct channel* chnode)
{
	debug(1000, "in");
	struct subtitle *node = NULL, *prev = NULL;

	if(chnode == NULL)
	{
		debug(1000, "out -> NULL detect");
		return;
	}

	m_lock(&status.subtitlemutex, 8);
	node = chnode->subtitle;
	prev = chnode->subtitle;

	while(node != NULL)
	{
		prev = node;
		node = node->next;
		chnode->subtitle = node;

		free(prev->name);
		prev->name = NULL;

		free(prev);
		prev = NULL;

	}
	m_unlock(&status.subtitlemutex, 8);
	debug(1000, "out");
}

#endif
