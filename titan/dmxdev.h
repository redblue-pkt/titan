#ifndef DMXDEV_H
#define DMXDEV_H

#if DVB_API_VERSION > 3
#ifndef DMX_ADD_PID
#define DMX_ADD_PID             _IOW('o', 51, __u16)
#define DMX_REMOVE_PID          _IOW('o', 52, __u16)
#endif
#else
#define DMX_ADD_PID              _IO('o', 51)
#define DMX_REMOVE_PID           _IO('o', 52)
#endif

struct dvbdev* dmxgetlast(int adapter)
{
	struct dvbdev* node = dvbdev;
	struct dvbdev* lastnode = NULL;

	while(node != NULL)
	{
		if(node->type == DEMUXDEV && node->adapter == adapter)
			lastnode = node;
		node = node->next;
	}

	return lastnode;
}

//flag: 0=alt, 1=ohne NONBLOCK r, 2=ohne NONBLOCK rw
struct dvbdev* dmxopen(struct dvbdev* fenode, int flag)
{
	int fd = -1;

	if(fenode == NULL) return NULL;
		
	m_lock(&status.dmxdevmutex, 9);
  struct dvbdev* node = dvbdev;
	
	while(node != NULL)
	{
		if(node->fd == -1 && node->type == DEMUXDEV && node->adapter == fenode->adapter && node->devnr == fenode->devnr)
			break;
		node = node->next;
	}

	if(node != NULL)
	{
		debug(200, "open dmx %s", node->dev);
		if(flag == 0)
			fd = open(node->dev, O_RDWR);
		if(flag == 1)
			fd = open(node->dev, O_RDONLY | O_CLOEXEC);
		if(flag == 2)
			fd = open(node->dev, O_RDWR | O_CLOEXEC);
		if(fd < 0)
		{
			debug(200, "open dmx failed %s", node->dev);
			node = NULL;
		}
		else
		{
			node->fd = fd;
			if(flag == 0)
			{
				fcntl(fd, F_SETFL, fcntl(fd, F_GETFL) | O_NONBLOCK);
				closeonexec(fd);
			}
		}
	}

	m_unlock(&status.dmxdevmutex, 9);
	return node;
}

int dmxopendirect(char *dmxdev)
{
	int fd = -1;
	
	debug(200, "open direct dmx %s", dmxdev);
	if((fd = open(dmxdev, O_RDWR)) < 0)
	{
		debug(200, "open dmx failed %s", dmxdev);
	}
	else
		fcntl(fd, F_SETFL, fcntl(fd, F_GETFL) | O_NONBLOCK);
	
	closeonexec(fd);
	return fd;
}

void dmxclose(struct dvbdev* node, int fd)
{
	if(node != NULL)
	{
		close(node->fd);
		node->fd = -1;
	}
	else
		close(fd);
}

int dmxstop(struct dvbdev* node)
{
	if(node == NULL)
	{
		debug(200, "NULL detect");
		return 1;
	}

	if(ioctl(node->fd, DMX_STOP) < 0)
	{
		perr("DMX_STOP");
		return 1;
	}
	
	return 0;
}

int dmxstart(struct dvbdev* node)
{
	
	debug(200, "start dmx ");
	if(node == NULL)
	{
		err("NULL detect");
		return 1;
	}

	if(ioctl(node->fd, DMX_START) < 0)
	{
		perr("DMX_START");
		return 1;
	}
	
	return 0;
}

int dmxsetbuffersize(struct dvbdev* node, unsigned long size)
{
	if(node == NULL)
	{
		err("NULL detect");
		return 1;
	}

	if(size == 0)
		size = getconfigint("dmxvideobuffersize", NULL);

	debug(200, "DMX_SET_BUFFER_SIZE=%ld", size);
	if(ioctl(node->fd, DMX_SET_BUFFER_SIZE, size) < 0)
	{
		perr("DMX_SET_BUFFER_SIZE");
		return 1;
	}
	
	return 0;
}

int dmxsetfilter(struct dvbdev* node, int pid, int secnr, int flag)
{
	struct dmx_sct_filter_params sctflt;

	if(node == NULL)
	{
		err("NULL detect");
		return 1;
	}

	memset(&sctflt, 0, sizeof(sctflt));

	if(flag == 1) //pat filter
	{
		sctflt.timeout = 0;
		sctflt.pid = pid;
		sctflt.flags = DMX_IMMEDIATE_START | DMX_CHECK_CRC;
	}
	if(flag == 2) //pmt filter
	{
		sctflt.filter.filter[0] = 0x02;
		sctflt.filter.mask[0] = 0xff;
		sctflt.timeout = 0;
		sctflt.pid = pid;
		sctflt.flags = DMX_IMMEDIATE_START | DMX_CHECK_CRC;
	}
	if(flag == 3) //date filter
	{
		sctflt.filter.filter[0] = 0x70;
		sctflt.filter.mask[0] = 0xff;
		sctflt.timeout = 0;
		sctflt.pid = pid;
		// DMX_CHECK_CRC not working on mipsel
		sctflt.flags = DMX_IMMEDIATE_START;// | DMX_CHECK_CRC;
	}
	if(flag == 4) //nit filter (pid 0x10)
	{
		sctflt.filter.filter[0] = 0x40;
		sctflt.filter.mask[0] = 0xff;
		sctflt.filter.filter[1] = 0x00;
		sctflt.filter.mask[1] = 0x00;
		sctflt.filter.filter[2] = 0x00;
		sctflt.filter.mask[2] = 0x00;
		sctflt.filter.filter[3] = 0x00;
		sctflt.filter.mask[3] = 0x00;
		sctflt.filter.filter[4] = secnr;
		sctflt.filter.mask[4] = 0xff;
		sctflt.timeout = 0;
		sctflt.pid = pid;
		sctflt.flags = DMX_IMMEDIATE_START | DMX_CHECK_CRC;
	}
	if(flag == 5) //eit all filter (pid 0x12)
	{
		sctflt.filter.filter[0] = 0x40;
		sctflt.filter.mask[0] = 0x40;
		sctflt.timeout = 0;
		sctflt.pid = pid;
		sctflt.flags = DMX_IMMEDIATE_START | DMX_CHECK_CRC;
	}
	if(flag == 6) //eit schedule, current multiplex (pid 0x12)
	{
		sctflt.filter.filter[0] = 0x50;
		sctflt.filter.mask[0] = 0xf0;
		sctflt.timeout = 0;
		sctflt.pid = pid;
		sctflt.flags = DMX_IMMEDIATE_START | DMX_CHECK_CRC;
	}
	if(flag == 7) //eit schedule, other multiplex (pid 0x12)
	{
		sctflt.filter.filter[0] = 0x60;
		sctflt.filter.mask[0] = 0xf0;
		sctflt.timeout = 0;
		sctflt.pid = pid;
		sctflt.flags = DMX_IMMEDIATE_START | DMX_CHECK_CRC;
	}
	if(flag == 8) //eit now/next only filter (pid 0x12)
	{
		sctflt.filter.filter[0] = 0x4e;
		sctflt.filter.mask[0] = 0xfe;
		sctflt.timeout = 0;
		sctflt.pid = pid;
		sctflt.flags = DMX_IMMEDIATE_START | DMX_CHECK_CRC;
	}
	if(flag == 9) //eit now/next current multiplex (pid 0x12)
	{
		sctflt.filter.filter[0] = 0x4e;
		sctflt.filter.mask[0] = 0xff;
		sctflt.timeout = 0;
		sctflt.pid = pid;
		sctflt.flags = DMX_IMMEDIATE_START | DMX_CHECK_CRC;
	}
	if(flag == 10) //eit now/next other multiplex (pid 0x12)
	{
		sctflt.filter.filter[0] = 0x4f;
		sctflt.filter.mask[0] = 0xff;
		sctflt.timeout = 0;
		sctflt.pid = pid;
		sctflt.flags = DMX_IMMEDIATE_START | DMX_CHECK_CRC;
	}
	if(flag == 11) //sdt akt (pid 0x11)
	{
		sctflt.filter.filter[0] = 0x42;
		sctflt.filter.mask[0] = 0xff;
		sctflt.filter.filter[1] = 0x00;
		sctflt.filter.mask[1] = 0x00;
		sctflt.filter.filter[2] = 0x00;
		sctflt.filter.mask[2] = 0x00;
		sctflt.filter.filter[3] = 0x00;
		sctflt.filter.mask[3] = 0x00;
		sctflt.filter.filter[4] = secnr;
		sctflt.filter.mask[4] = 0xff;
		sctflt.timeout = 0;
		sctflt.pid = pid;
		sctflt.flags = DMX_IMMEDIATE_START | DMX_CHECK_CRC;
	}
	if(flag == 12) //sdt other (pid 0x11)
	{
		sctflt.filter.filter[0] = 0x46;
		sctflt.filter.mask[0] = 0xff;
		sctflt.filter.filter[1] = 0x00;
		sctflt.filter.mask[1] = 0x00;
		sctflt.filter.filter[2] = 0x00;
		sctflt.filter.mask[2] = 0x00;
		sctflt.filter.filter[3] = 0x00;
		sctflt.filter.mask[3] = 0x00;
		sctflt.filter.filter[4] = secnr;
		sctflt.filter.mask[4] = 0xff;
		sctflt.timeout = 0;
		sctflt.pid = pid;
		sctflt.flags = DMX_IMMEDIATE_START | DMX_CHECK_CRC;
	}
	if(flag == 13) //rst filter (pid 0x13)
	{
		sctflt.filter.filter[0] = 0x13;
		sctflt.filter.mask[0] = 0xff;
		sctflt.timeout = 0;
		sctflt.pid = pid;
		sctflt.flags = DMX_IMMEDIATE_START | DMX_CHECK_CRC;
	}
	if(flag == 14) //ait filter
	{
		sctflt.filter.filter[0] = 0x74;
		sctflt.filter.mask[0] = 0xff;
		sctflt.timeout = 0;
		sctflt.pid = pid;
		sctflt.flags = DMX_IMMEDIATE_START | DMX_CHECK_CRC;
	}
	if(flag == 15) //mhw title
	{
		sctflt.filter.filter[0] = 0x90;
		sctflt.filter.mask[0] = 0xff;
		sctflt.timeout = 0;
		sctflt.pid = pid;
		sctflt.flags = DMX_IMMEDIATE_START | DMX_CHECK_CRC;
	}
	if(flag == 16) //mhw channel
	{
		sctflt.filter.filter[0] = 0x91;
		sctflt.filter.mask[0] = 0xff;
		sctflt.timeout = 0;
		sctflt.pid = pid;
		sctflt.flags = DMX_IMMEDIATE_START | DMX_CHECK_CRC;
	}
	if(flag == 17) //mhw summary
	{
		sctflt.filter.filter[0] = 0x90;
		sctflt.filter.mask[0] = 0xff;
		sctflt.timeout = 0;
		sctflt.pid = pid;
		sctflt.flags = DMX_IMMEDIATE_START | DMX_CHECK_CRC;
	}
	if(flag == 18) //mhw2 title
	{
		sctflt.filter.filter[0] = 0xe6;
		sctflt.filter.mask[0] = 0xff;
		sctflt.timeout = 0;
		sctflt.pid = pid;
		sctflt.flags = DMX_IMMEDIATE_START | DMX_CHECK_CRC;
	}
	if(flag == 19) //mhw2 channel
	{
		sctflt.filter.filter[0] = 0xc8;
		sctflt.filter.mask[0] = 0xff;
		sctflt.timeout = 0;
		sctflt.pid = pid;
		sctflt.flags = DMX_IMMEDIATE_START | DMX_CHECK_CRC;
	}
	if(flag == 20) //mhw2 summary
	{
		sctflt.filter.filter[0] = 0x96;
		sctflt.filter.mask[0] = 0xff;
		sctflt.timeout = 0;
		sctflt.pid = pid;
		sctflt.flags = DMX_IMMEDIATE_START | DMX_CHECK_CRC;
	}
	if(flag == 21) //opentv channel
	{
		sctflt.filter.filter[0] = 0x4a;
		sctflt.filter.mask[0] = 0xff;
		sctflt.timeout = 0;
		sctflt.pid = pid;
		sctflt.flags = DMX_IMMEDIATE_START | DMX_CHECK_CRC;
	}
	if(flag == 22) //opentv title
	{
		sctflt.filter.filter[0] = 0xa0;
		sctflt.filter.mask[0] = 0xfc;
		sctflt.timeout = 0;
		sctflt.pid = pid;
		sctflt.flags = DMX_IMMEDIATE_START | DMX_CHECK_CRC;
	}
	if(flag == 23) //opentv summary
	{
		sctflt.filter.filter[0] = 0xa8;
		sctflt.filter.mask[0] = 0xfc;
		sctflt.timeout = 0;
		sctflt.pid = pid;
		sctflt.flags = DMX_IMMEDIATE_START | DMX_CHECK_CRC;
	}

	debug(200, "DMX_SET_FILTER pid=%d, flag=%d", pid, flag);
	if(ioctl(node->fd, DMX_SET_FILTER, &sctflt) < 0)
	{
		perr("DMX_SET_FILTER");
		return 1;
	}

	return 0;
}

int dmxaddpid(struct dvbdev* node, int pid)
{
	if(node == NULL)
	{
		err("NULL detect");
		return 1;
	}

	debug(200, "DMX_ADD_PID=%d", pid);
	while(1)
	{ 
#if DVB_API_VERSION > 3
		__u16 p = pid;
		if(ioctl(node->fd, DMX_ADD_PID, &p) < 0)
#else
		if(ioctl(node->fd, DMX_ADD_PID, pid) < 0)
#endif
		{
			perr("DMX_ADD_PID");
			if(errno == EAGAIN || errno == EINTR)
				continue;
			return 1;
		}
		break;
	}

	return 0;
}

int dmxremovepid(struct dvbdev* node, int pid)
{
	if(node == NULL)
	{
		err("NULL detect");
		return 1;
	}

	debug(200, "DMX_REMOVE_PID=%d", pid);
	while(1)
	{ 
#if DVB_API_VERSION > 3
		__u16 p = pid;
		if(ioctl(node->fd, DMX_REMOVE_PID, &p) < 0)
#else
		if(ioctl(node->fd, DMX_REMOVE_PID, pid) < 0)
#endif
		{
			perr("DMX_REMOVE_PID");
			if(errno == EAGAIN || errno == EINTR)
				continue;
			return 1;
		}
		break;
	}

	return 0;
}

int dmxgetstc(struct dvbdev* node, int64_t* stc)
{
	struct dmx_stc dmxstc;

	if(node == NULL)
	{
		err("NULL detect");
		return 1;
	}

	memset(&dmxstc, 0, sizeof(struct dmx_stc));
	dmxstc.num = 0;
	dmxstc.base = 1;

	debug(200, "DMX_GET_STC");
	if(ioctl(node->fd, DMX_GET_STC, &dmxstc) < 0)
	{
		perr("DMX_GET_STC");
		return 1;
	}
	else
		*stc = (int64_t)dmxstc.stc;

	return 0;
}

int dmxsetsource(struct dvbdev* node, int source)
{
	debug(200, "dmxsetsource startet... sorce=%d", source);
	if(node == NULL)
	{
		err("NULL detect");
		return 1;
	}
	if(checkrealbox("HD51") == 1 || checkrealbox("HD60") == 1 || checkrealbox("HD61") == 1 || checkchipset("3798MV200") == 1 || checkchipset("HI3798MV200") == 1 || vubox1 == 1)  //source darf bei dieser Box nur einmal gesetzt werden, l\F6scht ansonsten die Filter.
	{
		if(node->fedmxsource == source)
		{
			debug(200, "workaround mutant");
			return 0;
		}
	}
#ifndef SIMULATE 
#ifdef MIPSEL
	//Workaround da ansonsten DVR4 nicht funktioniert (Treiberproblem)
	
	if(source > DMX_SOURCE_DVR0 && status.setdvr0 == 0 && checkchipset("HI3798MV200") == 0)
	{ 
		int sourcehelp = DMX_SOURCE_DVR0;
		ioctl(node->fd, DMX_SET_SOURCE, &sourcehelp);
		debug(200, "woraround DMX_SET_SOURCE=%d", sourcehelp);
		status.setdvr0 = 1;
	}
#endif
//mipsel feinfo work
//#ifdef SH4
	debug(200, "DMX_SET_SOURCE=%d", source);
	if(ioctl(node->fd, DMX_SET_SOURCE, &source) < 0)
	{
		perr("DMX_SET_SOURCE");
	}
	else
	{
		// fixt dvb-t pixelsalat
		if(checkrealbox("HD51") == 1 || checkrealbox("HD60") == 1 || checkrealbox("HD61") == 1 || checkbox("DM920") == 1 || checkchipset("3798MV200") == 1 || checkchipset("HI3798MV200") == 1 || vubox1 == 1)
		{
		  	struct dvbdev* nodeh = dvbdev;
			while(nodeh != NULL)
			{
				if(nodeh->type == DEMUXDEV && nodeh->adapter == node->adapter && nodeh->devnr == node->devnr)
					nodeh->fedmxsource = source;
				nodeh = nodeh->next;
			}
		}
	}
//#endif
#endif
	debug(200, "dmxsetsource end");
	return 0;
}

int dmxsetpesfilterfd(int fd, int pid, int input, int output, int pestype, int nostart)
{
	struct dmx_pes_filter_params pesflt;
	memset(&pesflt, 0, sizeof(pesflt));

	if(input == -1)
	{
		if(status.playing == 1)
			pesflt.input = DMX_IN_DVR;
		else
			pesflt.input = DMX_IN_FRONTEND;
	}
	else
		pesflt.input = input;

	pesflt.output = output;
	pesflt.pid = pid;
	if(nostart == 0)
		pesflt.flags = DMX_IMMEDIATE_START;
	pesflt.pes_type = pestype;

	debug(200, "DMX_SET_PES_FILTER pid=%d, pestype=%d, input=%d, output=%d", pid, pestype, pesflt.input, output);
	if(ioctl(fd, DMX_SET_PES_FILTER, &pesflt) < 0)
	{
		perr("DMX_SET_PES_FILTER");
		return 1;
	}

	return 0;
}

int dmxsetpesfilter(struct dvbdev* node, int pid, int input, int output, int pestype, int nostart)
{
	if(node == NULL)
	{
		err("NULL detect");
		return 1;
	}
	if(checkchipset("HI3798MV200") == 1 && (pestype == DMX_PES_AUDIO || pestype == DMX_PES_VIDEO))
		nostart = 0;
	int rc = dmxsetpesfilterfd(node->fd, pid, input, output, pestype, nostart);
	if(checkchipset("HI3798MV200") == 1 && (pestype == DMX_PES_AUDIO || pestype == DMX_PES_VIDEO))
	{
		dmxstart(node);
		if(pestype == DMX_PES_VIDEO)
			videofreeze(node);
		else
			audiopause(node);
		debug(200, "DMX_START test HI3798MV200");
	}
	return rc; 
}

int dmxgetdev()
{
	int i, y, z, fd = -1, count = 0;
	char *buf = NULL, *dmxdev = NULL;

	dmxdev = getconfig("dmxdev", NULL);
	if(dmxdev == NULL)
	{
		err("NULL detect");
		return count;
	}

	buf = malloc(MINMALLOC);
	if(buf == NULL)
	{
		err("no memory");
		return count;
	}
	
	//workaround HD51 kann nur max 4 dvr ... warum??
	int help = MAXDEMUXDEV;
	if(checkrealbox("HD51") == 1 || checkrealbox("HD60") == 1 || checkrealbox("HD61") == 1)
		help = 4;
	//

	for(i = 0; i < MAXDVBADAPTER; i++)
	{
		//for(y = 0; y < MAXDEMUXDEV; y++)
		for(y = 0; y < help; y++)
		{
			sprintf(buf, dmxdev, i, y);
			fd = dmxopendirect(buf);
			if(fd >= 0)
			{
				count++;
				for(z = 0; z < MAXDEMUXDEVOPEN; z++)
					adddvbdev(buf, i, y, -1, DEMUXDEV, NULL, NULL, NULL, 0);
				//without this, on atevio7500, if the second tuner is the maintuner
				//no picture (i don't know why)
				if(checkchipset("HI3798MV200") != 1)
					dmxsetpesfilterfd(fd, 0, -1, DMX_OUT_DECODER, DMX_PES_AUDIO, 0);
				dmxclose(NULL, fd);
			}
		}
	}

	free(buf);
	return count;
}

#endif
