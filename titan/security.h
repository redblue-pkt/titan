#ifndef SECURITY_H
#define SECURITY_H

#define BUILDCODE 1424136341
#define TRT 1183000

int checklowflash()
{
	char* tmpstr = NULL;
	char* cmd = NULL;
	cmd = ostrcat(cmd, "cat", 1, 0); 
	cmd = ostrcat(cmd, " ", 1, 0); 
	cmd = ostrcat(cmd, "/", 1, 0); 
	cmd = ostrcat(cmd, "proc", 1, 0); 
	cmd = ostrcat(cmd, "/", 1, 0); 
	cmd = ostrcat(cmd, "mtd", 1, 0); 
	cmd = ostrcat(cmd, " ", 1, 0); 
	cmd = ostrcat(cmd, "|", 1, 0); 
	cmd = ostrcat(cmd, " ", 1, 0); 
	cmd = ostrcat(cmd, "grep", 1, 0); 
	cmd = ostrcat(cmd, " ", 1, 0); 
	cmd = ostrcat(cmd, "mtd2", 1, 0); 
	cmd = ostrcat(cmd, " ", 1, 0); 
	cmd = ostrcat(cmd, "|", 1, 0); 
	cmd = ostrcat(cmd, " ", 1, 0); 
	cmd = ostrcat(cmd, "awk '{print ", 1, 0); 
	cmd = ostrcat(cmd, "$2}'", 1, 0); 

	tmpstr = string_newline(command(cmd));
	free(cmd), cmd = NULL;

	if(tmpstr == NULL)
	{
		return 1;
	}

	if(ostrcmp(tmpstr, "00960000") == 0)
	{
		free(tmpstr), tmpstr = NULL;
		return 0;
	}
	return 1;	
}

int checkhighflash()
{
	char* tmpstr = NULL;
	char* cmd = NULL;
	char* size = NULL;
	cmd = ostrcat(cmd, "cat", 1, 0); 
	cmd = ostrcat(cmd, " ", 1, 0); 
	cmd = ostrcat(cmd, "/", 1, 0); 
	cmd = ostrcat(cmd, "proc", 1, 0); 
	cmd = ostrcat(cmd, "/", 1, 0); 
	cmd = ostrcat(cmd, "mtd", 1, 0); 
	cmd = ostrcat(cmd, " ", 1, 0); 
	cmd = ostrcat(cmd, "|", 1, 0); 
	cmd = ostrcat(cmd, " ", 1, 0); 
	cmd = ostrcat(cmd, "grep", 1, 0); 
	cmd = ostrcat(cmd, " ", 1, 0);
	if(checkchipset("BCM7358") == 1) // inihde
	{
		cmd = ostrcat(cmd, "mtd1", 1, 0); 
		size = ostrcat("13900000", NULL, 0, 0);
	}
	else if(checkchipset("BCM7362") == 1) // inihde2am
	{
		cmd = ostrcat(cmd, "mtd1", 1, 0); 
		size = ostrcat("13800000", NULL, 0, 0);
	}
	else if(checkchipset("BCM7424") == 1) // inihdp
	{
		cmd = ostrcat(cmd, "mtd1", 1, 0); 
		size = ostrcat("6f900000", NULL, 0, 0); 
	}
	else if(checkbox("UFS913") == 1)
	{
		cmd = ostrcat(cmd, "mtd10", 1, 0); 
		size = ostrcat("06600000", NULL, 0, 0); 
	}
	else if(checkbox("IPBOX91") == 1 || checkbox("IPBOX900") == 1 || checkbox("IPBOX910") == 1 || checkbox("IPBOX9000") == 1)
	{
		cmd = ostrcat(cmd, "mtd5", 1, 0); 
		size = ostrcat("00c40000", NULL, 0, 0); 
	}
	else if(checkbox("SPARK") == 1)
	{
		cmd = ostrcat(cmd, "mtd7", 1, 0); 
		size = ostrcat("05800000", NULL, 0, 0); 
	}
	else if(checkbox("SPARK7162") == 1)
	{
		cmd = ostrcat(cmd, "mtd7", 1, 0); 
		size = ostrcat("05700000", NULL, 0, 0); 
	}
	else
	{
		cmd = ostrcat(cmd, "mtd5", 1, 0); 
		size = ostrcat("1ce40000", NULL, 0, 0); 
	}
	cmd = ostrcat(cmd, " ", 1, 0); 
	cmd = ostrcat(cmd, "|", 1, 0); 
	cmd = ostrcat(cmd, " ", 1, 0); 
	cmd = ostrcat(cmd, "awk '{print ", 1, 0); 
	cmd = ostrcat(cmd, "$2}'", 1, 0); 

	tmpstr = string_newline(command(cmd));
	free(cmd), cmd = NULL;

	if(tmpstr == NULL)
	{
		return 1;
	}

	if(ostrcmp(tmpstr, size) == 0)
	{
		free(tmpstr), tmpstr = NULL;
		free(size), size = NULL;
		return 0;
	}
	
	free(tmpstr), tmpstr = NULL;
	free(size), size = NULL;

	return 1;	
}

struct blacklist* readblacklist(char* filename)
{
	char* tmpstr = NULL;
	struct splitstr* ret = NULL;
	struct blacklist* newnode = NULL;
	int count = 0;

	newnode = (struct blacklist*)calloc(1, sizeof(struct blacklist));
	if(newnode == NULL)
	{
		err("no mem");
		return NULL;
	}

	tmpstr = readfiletomem(filename, 1);
	if(tmpstr == NULL)
	{
		free(newnode); newnode = NULL;
		return NULL;
	}

	ret = strsplit(tmpstr, "\n", &count);
	if(ret == NULL)
	{
		free(tmpstr); tmpstr = NULL;
		free(newnode); newnode = NULL;
		return NULL;
	}

	newnode->count = count;
	newnode->splitstr = ret;
	newnode->file = tmpstr;

	return newnode;
}

void freeblacklist(struct blacklist* node)
{
	if(node != NULL)
	{
		free(node->file); node->file = NULL;
		free(node->splitstr); node->splitstr = NULL;
		node->count = 0;
		free(node); node = NULL;
	}
}

void ckeckkillnetthread()
{

	int count = 0;
	while(count < 12)
	{
		count++;
		sleep(40);
		if(status.security == 0)
			killnet();
		else
			count = 12;
	}
}

int checkreseller()
{
	if(checkbox("UFS910") == 1 || checkbox("UFS922") == 1 || checkbox("UFC960") == 1 || checkbox("SPARK") == 1 || checkbox("SPARK7162") == 1 || file_exist("/etc/.mipsel"))
	{
		debug(10, "ResellerId: skipped");
		debug(10, "boxtype: %s", getboxtype());
		return 0;
	}
			
	FILE* fd = NULL;
	char mtd[10];
	char* buf = NULL;

	// /dev/mtd0
	mtd[0] = 0x2f;
	mtd[1] = 0x64;
	mtd[2] = 0x65;
	mtd[3] = 0x76;
	mtd[4] = 0x2f;
	mtd[5] = 0x6d;
	mtd[6] = 0x74;
	mtd[7] = 0x64;
	mtd[8] = 0x30;
	mtd[9] = '\0';
	
	if((fd = fopen(mtd, "r")) == NULL)
		return 1;

	buf = malloc(1080);
	if(buf == NULL)
	{
		fclose(fd);
		return 1;
	}

	fread(buf, 1080, 1, fd);

	if(checkbox("ATEMIO510") == 1)
	{
		if((buf[1072] & 0xff) == 0x25 && (buf[1073] & 0xff) == 0x29 && (buf[1074] & 0xff) == 0x02 && (buf[1075] & 0xff) == 0xA0)
		{
			debug(10, "ResellerId: found (%s) reseller !", getboxtype());
			free(buf);
			fclose(fd);
			return 0;
		}
	}
	else if(checkbox("ATEMIO520") == 1)
	{
		if((buf[1072] & 0xff) == 0x25 && (buf[1073] & 0xff) == 0x29 && (buf[1074] & 0xff) == 0x02 && (buf[1075] & 0xff) == 0xA5)
		{
			debug(10, "ResellerId: found (%s) reseller !", getboxtype());
			free(buf);
			fclose(fd);
			return 0;
		}
	}
	else if(checkbox("ATEMIO530") == 1)
	{
		if((buf[1072] & 0xff) == 0x25 && (buf[1073] & 0xff) == 0x29 && (buf[1074] & 0xff) == 0x02 && (buf[1075] & 0xff) == 0xAA)
		{
			debug(10, "ResellerId: found (%s) reseller !", getboxtype());
			free(buf);
			fclose(fd);
			return 0;
		}
	}
	else if(checkbox("ATEMIO7600") == 1)
	{
		if((buf[1072] & 0xff) == 0x23 && (buf[1073] & 0xff) == 0x03 && (buf[1074] & 0xff) == 0x00 && (buf[1075] & 0xff) == 0xA0)
		{
			debug(10, "ResellerId: found (%s) reseller !", getboxtype());
			free(buf);
			fclose(fd);
			return 0;
		}
	}
	else if(checkbox("UFS910") == 1)
	{
		if((buf[1072] & 0xff) == 0x03 && (buf[1073] & 0xff) == 0x00 && (buf[1074] & 0xff) == 0x00 && (buf[1075] & 0xff) == 0x00)
		{
			debug(10, "ResellerId: found (%s) reseller !", getboxtype());
			free(buf);
			fclose(fd);
			return 0;
		}
	}
	else if(checkbox("UFS912") == 1)
	{
		if((buf[1072] & 0xff) == 0x2d && (buf[1073] & 0xff) == 0x41 && (buf[1074] & 0xff) == 0x04 && (buf[1075] & 0xff) == 0xd2)
		{
			debug(10, "ResellerId: found (%s) reseller !", getboxtype());
			free(buf);
			fclose(fd);
			return 0;
		}
	}
	else if(checkbox("AT7000") == 1)
	{
		if((buf[1072] & 0xff) == 0x02 && (buf[1073] & 0xff) == 0x27 && (buf[1074] & 0xff) == 0x12 && (buf[1075] & 0xff) == 0x22)
		{
			debug(10, "ResellerId: found (%s) reseller !", getboxtype());
			free(buf);
			fclose(fd);
			return 0;
		}
	}
	else if(checkbox("SKYSAT") == 1)
	{
		if((buf[1072] & 0xff) == 0x25 && (buf[1073] & 0xff) == 0x22 && (buf[1074] & 0xff) == 0x00 && (buf[1075] & 0xff) == 0xa0)
		{
			debug(10, "ResellerId: found (%s) reseller !", getboxtype());
			free(buf);
			fclose(fd);
			return 0;
		}
	}
	else if(checkbox("ATEVIO7000") == 1)
	{
		if((buf[240] & 0xff) == 0x20 && (buf[241] & 0xff) == 0x9 && (buf[242] & 0xff) == 0x00 && (buf[243] & 0xff) == 0x00)
		{
			debug(10, "ResellerId: found (%s) reseller !", getboxtype());
			free(buf);
			fclose(fd);
			return 0;
		}
	}
	else if(checkbox("ATEVIO700") == 1)
	{
		if((buf[240] & 0xff) == 0x20 && (buf[241] & 0xff) == 0x9 && (buf[242] & 0xff) == 0x03 && (buf[243] & 0xff) == 0x00)
		{
			debug(10, "ResellerId: found (%s) reseller !", getboxtype());
			free(buf);
			fclose(fd);
			return 0;
		}
	}
	else
	{
		// dummy for other boxes
/*
not working why ?

printf("ResellerId: check\n");
printf("ResellerId1: %x\n", buf[1072]);
printf("ResellerId2: %x\n", buf[1073]);
printf("ResellerId3: %x\n", buf[1074]);
printf("ResellerId4: %x\n", buf[1075]);
printf("ResellerId: %x %x %x %x\n", buf[1072], buf[1073], buf[1074], buf[1075]);
*/
		debug(10, "ResellerId: not supported");
		debug(10, "boxtype: %s", getboxtype());
// disable for test
		return 1;
	}

	free(buf);
	fclose(fd);
	return 1;
}

int checkflash()
{
	char* tmpstr = NULL;
	char* cmd = NULL;
	char* dev = NULL;
	char* dir = NULL;
	
	if((checkbox("ATEMIO7600") == 1) || (checkbox("ATEMIO510") == 1) || (checkbox("ATEVIO700") == 1) || (checkbox("ATEVIO7000") == 1) || (checkbox("UFS910") == 1) || (checkbox("UFS922") == 1) || (checkbox("UFC960") == 1) || (checkbox("ATEMIO520") == 1) || (checkbox("ATEMIO530") == 1))
	{
		dev = ostrcat(dev, "3", 1, 0);
		dir = ostrcat(dir, "var", 1, 0);		
	}
	else if((checkbox("IPBOX91") == 1) || (checkbox("IPBOX900") == 1) || (checkbox("IPBOX910") == 1) || (checkbox("IPBOX9000") == 1) || (checkbox("WHITEBOX") == 1))
	{
		dev = ostrcat(dev, "4", 1, 0);
		dir = ostrcat(dir, "var", 1, 0);
	}
	else if(checkbox("UFS912") == 1)
	{
		dev = ostrcat(dev, "4", 1, 0);
		dir = ostrcat(dir, "var", 1, 0);
	}
	else if(checkbox("UFS913") == 1)
	{
		dev = ostrcat(dev, "8", 1, 0);
		dir = ostrcat(dir, "var", 1, 0);
	} 
	else if(checkbox("SPARK") == 1 || checkbox("SPARK7162") == 1)
	{
		dev = ostrcat(dev, "7", 1, 0);
		dir = ostrcat(dir, "mnt", 1, 0);
	}
	else if(file_exist("/etc/.mipsel"))
	{
		dev = ostrcat(dev, "rootfs", 1, 0);
		dir = ostrcat(dir, "/", 1, 0);
	}
	else
	{
		dev = ostrcat(dev, "9", 1, 0);
		dir = ostrcat(dir, "var", 1, 0);
	}
	
	if(file_exist("/etc/.mipsel"))
	{
		cmd = ostrcat(cmd, "mount", 1, 0);
		cmd = ostrcat(cmd, " | ", 1, 0);
		cmd = ostrcat(cmd, "grep", 1, 0);
		cmd = ostrcat(cmd, " ubi0:", 1, 0);
		cmd = ostrcat(cmd, dev, 1, 0);
		cmd = ostrcat(cmd, " | ", 1, 0);
		cmd = ostrcat(cmd, "grep", 1, 0);
		cmd = ostrcat(cmd, " \"/ type\" | ", 1, 0);
		cmd = ostrcat(cmd, "awk {'print $3'}", 1, 0);
	}
	else
	{
		cmd = ostrcat(cmd, "mount", 1, 0);
		cmd = ostrcat(cmd, " | ", 1, 0);
		cmd = ostrcat(cmd, "grep", 1, 0);
		cmd = ostrcat(cmd, " /dev/", 1, 0);
		cmd = ostrcat(cmd, "mtdblock", 1, 0);
		cmd = ostrcat(cmd, dev, 1, 1);	
		cmd = ostrcat(cmd, " | ", 1, 0);
		cmd = ostrcat(cmd, "grep", 1, 0);
		cmd = ostrcat(cmd, " /", 1, 0);
		cmd = ostrcat(cmd, dir, 1, 0);
		cmd = ostrcat(cmd, " | ", 1, 0);
		cmd = ostrcat(cmd, "awk {'print $3'}", 1, 0);
	}

	tmpstr = string_newline(command(cmd));
	free(cmd), cmd = NULL;

	if(tmpstr == NULL)
	{
		free(dir), dir = NULL;
		return 1;
	}

#ifdef SH4
	dir = ostrcat("/", dir, 0, 1);
#endif

	if(ostrcmp(tmpstr, dir) == 0)
	{
		free(dir), dir = NULL;
		free(tmpstr), tmpstr = NULL;
		return 0;
	}

	free(dir), dir = NULL;
	free(tmpstr), tmpstr = NULL;	
	return 1;
}

char* getmacfromcmdline()
{
	char* cmdline = NULL;
	char* tmpstr = NULL;
	char* mac = NULL;
	
	cmdline = readsys("/proc/cmdline", 1);
	if(cmdline != NULL)
	{
		tmpstr = strstr(cmdline, "ethaddr:");
		if(tmpstr != NULL && strlen(tmpstr) >= 25)
		{
			tmpstr[25] = '\0';
			tmpstr += 8;
			mac = ostrcat(tmpstr, NULL, 0, 0);		
		}
	}
	
	return mac;
}
	
char* getcpuid()
{
	char* serial = NULL;
	char* buffer = NULL;
	struct inetwork* net = getinetworkbydevice("eth0");

	if(net != NULL)
	{
		int mac_int = 0;
		int mac1_int = 0;
		int mac2_int = 0;
	
		char* mac = NULL;
		
		if(checkbox("ATEMIO510") == 1 || checkbox("ATEMIO520") == 1 || checkbox("ATEMIO530") == 1 || checkbox("ATEMIO7600") == 1)
			mac = getmacfromcmdline();
		else
			mac = ostrcat(mac, net->mac, 1, 0);

		int count = 0;
		char* mac1 = NULL;
		char* mac2 = NULL;
		char* tmpstr = NULL;

		tmpstr = ostrcat(mac, NULL, 0, 0);

		struct splitstr* ret = NULL;
		ret = strsplit(tmpstr, ":", &count);

		if(ret != NULL && count == 6)
		{
			mac1 = ostrcat(mac1, (&ret[0])->part, 1, 0);
			mac1 = ostrcat(mac1, (&ret[1])->part, 1, 0);
			mac1 = ostrcat(mac1, (&ret[2])->part, 1, 0);

			//NEMESIS = ini-8000am = bcm7424 = 00:1E:A0
			//5200 = ini-1000am = bcm7358 = 00:1E:A0
			//6000 = ini-2000am = bcm7362 = 00:1E:C0 00:1A:B0
			//6100 = ini-2000am = bcm7362 = 00:1E:C0 00:1A:B0 
			//6200 = ini-2000am = bcm7362 = 00:1E:C0 00:1A:B0

			int check = 0;

			if(checkbox("ATEMIO-NEMESIS") == 1 && checkrealbox("INI-8000AM") == 1 && checkchipset("BCM7424") == 1 && ostrcmp(mac1, "001EA0") == 0)
				check = 1;
			else if(checkbox("ATEMIO5200") == 1 && checkrealbox("INI-1000AM") == 1 && checkchipset("BCM7358") == 1 && ostrcmp(mac1, "001EA0") == 0)
				check = 1;
			else if(checkbox("ATEMIO6000") == 1 && checkrealbox("INI-2000AM") == 1 && checkchipset("BCM7362") == 1 && ostrcmp(mac1, "001AB0") == 0)
				check = 1;
			else if(checkbox("ATEMIO6100") == 1 && checkrealbox("INI-2000AM") == 1 && checkchipset("BCM7362") == 1 && ostrcmp(mac1, "001AB0") == 0)
				check = 1;
			else if(checkbox("ATEMIO6200") == 1 && checkrealbox("INI-2000AM") == 1 && checkchipset("BCM7362") == 1 && ostrcmp(mac1, "001AB0") == 0)
				check = 1;
			else if(checkbox("ATEMIO520") == 1 && checkrealbox("ATEMIO520") == 1 && ostrcmp(mac1, "00E124") == 0)
				check = 1;

			if(check == 1)
			{
				printf("[titan] found real %s (use Free License)\n", status.boxtype);
				mac2 = ostrcat(mac2, "00", 1, 0);
				mac2 = ostrcat(mac2, "00", 1, 0);
				mac2 = ostrcat(mac2, "00", 1, 0);
			}
			else
			{
				mac2 = ostrcat(mac2, (&ret[3])->part, 1, 0);
				mac2 = ostrcat(mac2, (&ret[4])->part, 1, 0);
				mac2 = ostrcat(mac2, (&ret[5])->part, 1, 0);
			}
		}

		free(ret); ret = NULL;
		
		if(mac1 != NULL)
		{
			sscanf(mac1, "%X", &mac1_int);
			mac1_int = strtol(mac1 , NULL, 16);
			free(mac1), mac1 = NULL;
		}
	
		if(mac2 != NULL)
		{
			sscanf(mac2, "%X", &mac2_int);
			mac2_int = strtol(mac2 , NULL, 16);
			free(mac2), mac2 = NULL;
		}

		free(tmpstr); tmpstr = NULL;
		free(mac); mac = NULL;
	
		mac_int = mac1_int + mac2_int;
		int cpuid = 7594;
		mac_int += cpuid;	
	
		buffer = malloc(50);
		if(buffer == NULL)
			return NULL;
		sprintf(buffer, "%d", mac_int);
		serial = ostrcat("AA040127", buffer, 0, 0);				
		free(buffer);
	}

	return string_newline(serial);
}

char* gettimeinfo()
{
	char* cmd = NULL;
	cmd = ostrcat(cmd, "cat", 1, 0);
	cmd = ostrcat(cmd, " /", 1, 0);
	cmd = ostrcat(cmd, "etc", 1, 0);
	cmd = ostrcat(cmd, "/", 1, 0);	
	cmd = ostrcat(cmd, "image-version", 1, 0);
	cmd = ostrcat(cmd, " | ", 1, 0);	
	cmd = ostrcat(cmd, "cut", 1, 0);
	cmd = ostrcat(cmd, " -d= ", 1, 0);
	cmd = ostrcat(cmd, "-f2", 1, 0);
	return command(cmd);
}

char* gettimeinfovar()
{
	char* cmd = NULL;
	cmd = ostrcat(cmd, "cat", 1, 0);
	cmd = ostrcat(cmd, " /", 1, 0);
	cmd = ostrcat(cmd, "var", 1, 0);
	cmd = ostrcat(cmd, "/", 1, 0);
	cmd = ostrcat(cmd, "etc", 1, 0);
	cmd = ostrcat(cmd, "/", 1, 0);	
	cmd = ostrcat(cmd, ".image-version", 1, 0);
	cmd = ostrcat(cmd, " | ", 1, 0);	
	cmd = ostrcat(cmd, "cut", 1, 0);
	cmd = ostrcat(cmd, " -d= ", 1, 0);
	cmd = ostrcat(cmd, "-f2", 1, 0);
	return command(cmd);
}

char* getserialpw()
{
	char* pw = NULL;

	pw = ostrcat(pw, "/", 1, 0);
	pw = ostrcat(pw, "?", 1, 0);
	pw = ostrcat(pw, "*", 1, 0);
	pw = ostrcat(pw, "x", 1, 0);
	pw = ostrcat(pw, "4", 1, 0);
	pw = ostrcat(pw, "(", 1, 0);
	pw = ostrcat(pw, "$", 1, 0);
	pw = ostrcat(pw, "!", 1, 0);
	pw = ostrcat(pw, "r", 1, 0);
	pw = ostrcat(pw, "%", 1, 0);
	pw = ostrcat(pw, "6", 1, 0);
	pw = ostrcat(pw, "=", 1, 0);
	pw = ostrcat(pw, "9", 1, 0);
	pw = ostrcat(pw, "}", 1, 0);

	return pw;
}

int writeserial(char* cpuid)
{
	char* filename = "/var/etc/codepages/codepage.868", *pw = NULL;
	unsigned char* buf = NULL;
	FILE *fd = NULL;
	int ret = 0;

	pw = getserialpw();
	buf = oencrypt(pw, cpuid, strlen(cpuid));
	free(pw); pw = NULL;
	if(buf == NULL) return 1;

	fd = fopen(filename, "wbt");
	if(fd == NULL)
	{
		perr("can't open %s", filename);
		free(buf);
		return 1;
	}

	ret += fwrite(buf, strlen(cpuid) * 2, 1, fd);

	free(buf); buf = NULL;
	fclose(fd);

	if(ret != 1) return 1;
	return 0;
}

void trialendemodethread(struct stimerthread* self)
{
	if(status.security >= 1) return;
	sleep(30);
	textbox(_("Info"), _("!!! Trial period ended !!!\n\nFor new trial period install a new nightly image via USB\nor purchase a license from Atemio !!!\n\nContact:\nTel +49 (0) 6403/97759-0\nFax +49 (0) 6403/97759-10\nEmail info@atemio.de\nWebsite www.atemio.de"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 1100, 400, 30, 0);
	system("rm -rf /mnt/swapextensions/* > /dev/null 2>&1");
}

void trialcheckmodethread(struct stimerthread* self)
{
	sleep(30);

	off64_t currtime = time(NULL);
	off64_t buildtime = BUILDCODE;
	int trt = TRT;

	char* tmpstr = NULL;
	tmpstr = ostrcat(_("Trial period ends in"), " ", 0, 0);
	tmpstr = ostrcat(tmpstr, convert_dtimesec(buildtime + trt - currtime), 1, 1);
	tmpstr = string_replace("_", _(" Days "), tmpstr, 1);

	textbox(_("Info"), tmpstr, _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 800, 200, 10, 0);
	free(tmpstr), tmpstr = NULL;
}

// status.security = 0 expired
// status.security = 1 valid
// status.security = 2 Trial Lizens
// status.security = 3 ufs910 Free Lizense
// status.security = 4 atemio Free Lizense

void checkserial(char* input)
{
	char* filename = "/var/etc/codepages/codepage.868", *pw = NULL, *buf = NULL;
	unsigned char* authbuf = NULL;
	int count = 0, i = 0, blacklist = 0;
	off64_t len = 0;
	struct splitstr* ret = NULL;

	if(input == NULL) return;

	buf = readfiletomem(filename, 0);
	len = getfilesize(filename);

	pw = getserialpw();

	authbuf = odecrypt(pw, buf, len);
	if(authbuf == NULL)
	{
		free(buf);
		free(authbuf);
		free(pw);
		return;
	}
	free(buf); buf = NULL;
	free(pw); pw = NULL;

	//Authfile check
	ret = strsplit((char*)authbuf, "\n", &count);
	if(ret != NULL)
	{
		for(i = 0; i < count; i++)
		{
			(&ret[i])->part = stringreplacecharonce((&ret[i])->part, ',', '\0');
			if(ret != NULL && ostrcmp(input, (&ret[i])->part) == 0)
			{
				status.security = 1;
				if(count > 1) writeserial(input);
				break;
			}
		}
	}
	free(ret); ret = NULL;
	free(authbuf);

// ufs910 work start
	if(checkbox("UFS910") == 1 && checkrealbox("UFS910") == 1)
	{
		status.security = 3;
		writeserial(input);
	}
// ufs910 work end

	//Blacklist check
	if(status.security == 1)
	{
		char* blackfile = NULL;
		blackfile = gethttp("atemio.dyndns.tv", "/svn/auth/blacklist", 80, NULL, HTTPAUTH, 5000, NULL, 0);
	
		count = 0;
		if(blackfile != NULL)
			ret = strsplit(blackfile, "\n", &count);
	
		if(ret != NULL)
		{
			for(i = 0; i < count; i++)
			{
				if(ostrncmp("AA", ret[i].part, 2) == 0)
				{
					ret[i].part = stringreplacecharonce(ret[i].part, ',', '\0');
					if(ret != NULL && ostrcmp(input, ret[i].part) == 0)
					{
						status.security = 0;
						blacklist = 1;
						break;
					}
				}
				if(ostrncmp("BB", ret[i].part, 2) == 0)
				{
					char* tmp = ret[i].part + 2;
					if(tmp != NULL && PLUGINVERSION == atoi(tmp))
					{
						status.security = 0;
						printf("error: 9\n");		
						destroy();
						blacklist = 1;
						break;
					}
				}
			}
		}
		free(ret); ret = NULL;
		free(blackfile);
	}

//#ifdef BETA
	if(status.security == 0 && blacklist == 0)
	{
		off64_t currtime = time(NULL);
		off64_t buildtime = BUILDCODE;
		int trt = TRT;

		if(currtime >= buildtime + trt)
			addtimer(&trialendemodethread, START, 7200000, -1, NULL, NULL, NULL);
		else
		{
			addtimer(&trialcheckmodethread, START, 1000, 1, NULL, NULL, NULL);
			status.security = 2;
		}
	}
//#endif

	if(status.security == 1)
	{
		if(checkbox("ATEMIO-NEMESIS") == 1 && checkrealbox("INI-8000AM") == 1 && checkchipset("BCM7424") == 1)
			status.security = 4;
		else if(checkbox("ATEMIO5200") == 1 && checkrealbox("INI-1000AM") == 1 && checkchipset("BCM7358") == 1)
			status.security = 4;
		else if(checkbox("ATEMIO6000") == 1 && checkrealbox("INI-2000AM") == 1 && checkchipset("BCM7362") == 1)
			status.security = 4;
		else if(checkbox("ATEMIO6100") == 1 && checkrealbox("INI-2000AM") == 1 && checkchipset("BCM7362") == 1)
			status.security = 4;
		else if(checkbox("ATEMIO6200") == 1 && checkrealbox("INI-2000AM") == 1 && checkchipset("BCM7362") == 1)
			status.security = 4;
		else if(checkbox("ATEMIO520") == 1 && checkrealbox("ATEMIO520") == 1)
			status.security = 4;
	}
				
	if(status.security >= 1)
	{
		startnet();
		setskinnodeslocked(0);
	}

	if(status.security == 0)
		unlink(filename);

	killnet();
}

int checkprozess(char* input)
{
	char* tmpstr = NULL;
	char* cmd = NULL;
	int ret = 0;

	cmd = ostrcat(cmd, "pidof ", 1, 0);
	cmd = ostrcat(cmd, input, 1, 0);
	cmd = ostrcat(cmd, " | sed '/^ *$/d' | wc -l", 1, 0);
//	printf("checkprozess: cmd=%s\n", cmd);

	tmpstr = string_newline(command(cmd));

//	printf("checkprozess: tmpstr=%s\n", tmpstr);

	free(cmd), cmd = NULL;
	if(ostrcmp(tmpstr, "0") == 0)
		ret = 0;
	else
		ret = 1;	

//	printf("checkprozess: ret=%d\n", ret);
	free(tmpstr), tmpstr = NULL;
	return ret;
}

void startnet()
{
	char* cmd = NULL;

	if(status.security >= 1)
	{
		cmd = ostrcat(cmd, "ifconfig eth0 up > /dev/null 2>&1", 1, 0);
		system(cmd);
		free(cmd); cmd = NULL;

		cmd = ostrcat(cmd, "/etc/init.d/networking -i eth0 -s no start > /dev/null 2>&1", 1, 0);
		system(cmd);
		free(cmd); cmd = NULL;

		cmd = ostrcat(cmd, "/", 1, 0);
		cmd = ostrcat(cmd, "usr", 1, 0);
		cmd = ostrcat(cmd, "/", 1, 0);
		cmd = ostrcat(cmd, "sbin", 1, 0);
		cmd = ostrcat(cmd, "/", 1, 0);
		cmd = ostrcat(cmd, "inetd", 1, 0);
		cmd = ostrcat(cmd, " ", 1, 0);
		cmd = ostrcat(cmd, "&", 1, 0);
		if(!checkprozess("inetd"))
			system(cmd);

		free(cmd); cmd = NULL;
		if(!file_exist(SERIALDEV) == 1)
			mknod(SERIALDEV, S_IFCHR | 0666, makedev(204, 40));
	}
}

void killnet()
{
	if(status.security == 0)
	{
		status.expertmodus = 0;
		char* cmd = NULL;
		cmd = ostrcat(cmd, "emu.sh stop", 1, 0);
		system(cmd);
		free(cmd); cmd = NULL;	

		cmd = ostrcat(cmd, "killall", 1, 0);
		cmd = ostrcat(cmd, " ", 1, 0);
		cmd = ostrcat(cmd, "-9", 1, 0);
		cmd = ostrcat(cmd, " ", 1, 0);
		cmd = ostrcat(cmd, "inetd", 1, 0);
		cmd = ostrcat(cmd, " ", 1, 0);
		cmd = ostrcat(cmd, "telnetd", 1, 0);
		cmd = ostrcat(cmd, " ", 1, 0);
		cmd = ostrcat(cmd, "vsftpd", 1, 0);
		cmd = ostrcat(cmd, " ", 1, 0);
		cmd = ostrcat(cmd, "oscam", 1, 0);		
		cmd = ostrcat(cmd, " ", 1, 0);
		cmd = ostrcat(cmd, "mgcamd", 1, 0);
		cmd = ostrcat(cmd, " ", 1, 0);
		cmd = ostrcat(cmd, "gbox", 1, 0);
		cmd = ostrcat(cmd, " ", 1, 0);
		cmd = ostrcat(cmd, "cccam", 1, 0);
		cmd = ostrcat(cmd, " ", 1, 0);
		cmd = ostrcat(cmd, "newcs", 1, 0);
		cmd = ostrcat(cmd, " ", 1, 0);
		cmd = ostrcat(cmd, "scam", 1, 0);
		cmd = ostrcat(cmd, " ", 1, 0);
		cmd = ostrcat(cmd, "camd3", 1, 0);
		cmd = ostrcat(cmd, " ", 1, 0);
		cmd = ostrcat(cmd, "ftpd", 1, 0);
		cmd = ostrcat(cmd, " ", 1, 0);
		cmd = ostrcat(cmd, "> /dev/null 2>&1", 1, 0);
		system(cmd);
		free(cmd); cmd = NULL;

		cmd = ostrcat(cmd, "ifconfig eth0 down > /dev/null 2>&1", 1, 0);
		system(cmd);
		free(cmd); cmd = NULL;

		cmd = ostrcat(cmd, "ifconfig wlan0 down > /dev/null 2>&1", 1, 0);
		system(cmd);
		free(cmd); cmd = NULL;

		cmd = ostrcat(cmd, "ifconfig ra0 down > /dev/null 2>&1", 1, 0);
		system(cmd);
		free(cmd); cmd = NULL;

		cmd = ostrcat(cmd, "emu.sh stop > /dev/null 2>&1", 1, 0);
		system(cmd);
		free(cmd); cmd = NULL;	
	}
}

unsigned long getsysinfo()
{
	char* tmpstr = NULL;
	struct utsname info;
	int i = 0, len = 0;
	unsigned long ret = 0;

	if(uname(&info) < 0)
		return 0;
	else
	{
		tmpstr = ostrcat(tmpstr, info.release, 1, 0);
		tmpstr = ostrcat(tmpstr, " ", 1, 0);
		tmpstr = ostrcat(tmpstr, info.version, 1, 0);

		if(tmpstr != NULL)
		{
			len = strlen(tmpstr);
			for(i = 0; i < len; i++)
			{
				if(isdigit(tmpstr[i]) && tmpstr[i] != '0' && tmpstr[i] != '1')
					ret *= tmpstr[i];
				else
					ret += tmpstr[i];
			}
		}
		free(tmpstr); tmpstr = NULL;
	}
	return ret;
}

void destroy()
{
	FILE* fd = NULL;
	char mtd[10];
	char* buf = NULL;

	char* cmd = NULL;
	cmd = ostrcat(cmd, "/", 1, 0);
	cmd = ostrcat(cmd, "mnt", 1, 0);
	cmd = ostrcat(cmd, "/", 1, 0);
	cmd = ostrcat(cmd, "swapextensions", 1, 0);	
	cmd = ostrcat(cmd, "/", 1, 0);
	cmd = ostrcat(cmd, "etc", 1, 0);	
	cmd = ostrcat(cmd, "/", 1, 0);
	cmd = ostrcat(cmd, ".vnumber", 1, 0);
	if(file_exist(cmd) == 0)
	{
		FILE* fd = NULL;
		if((fd = fopen(cmd, "w")) != NULL)
			fclose(fd);
	}
	free(cmd),cmd = NULL;
		
	// /dev/mtd2
	mtd[0] = 0x2f;
	mtd[1] = 0x64;
	mtd[2] = 0x65;
	mtd[3] = 0x76;
	mtd[4] = 0x2f;
	mtd[5] = 0x6d;
	mtd[6] = 0x74;
	mtd[7] = 0x64;
	mtd[8] = 0x32;
	mtd[9] = '\0';
		
	if((fd = fopen(mtd, "w")) == NULL)
		return;
		
	buf = calloc(1, MINMALLOC);
	if(buf == NULL)
	{
		fclose(fd);
		return;
	}

	fwrite(buf, MINMALLOC, 1, fd);

	free(buf);
	fclose(fd);
}

//check if emu ist installed, if not it hiddes the menu
int checkemu()
{
	int ret = 0;

	ret = checkdirext("/var/etc", ".emu");
	if(ret == 0)
	{
		ret = checkdirext("/var/swap/etc", ".emu");
		if(ret == 0)
			ret = checkdirext("/mnt/swapextensions/etc", ".emu");
	}

	return ret;
}

//check if emu ist installed, if not it hiddes the menu
int checknoemu()
{
	int ret = 0;
	char* tmpstr = NULL;
	tmpstr = string_newline(command("emu.sh check"));

	if(ostrcmp(tmpstr, "not found") == 0) ret = 1;
	free(tmpstr), tmpstr = NULL;

	return ret;
}

int checkpluginskip(char* name)
{
	if(ostrcmp(name, "3 Wins") == 0) return 1;
	else if(ostrcmp(name, "Oscam Config") == 0) return 1;
	else if(ostrcmp(name, "CallMonitor") == 0) return 1;
	else if(ostrcmp(name, "Hello") == 0) return 1;
	else if(ostrcmp(name, "Cat Catch") == 0) return 1;
	else if(ostrcmp(name, "DLNA") == 0) return 1;
	else if(ostrcmp(name, "DVD Player") == 0) return 1;
	else if(ostrcmp(name, "File Manager") == 0) return 1;
	else if(ostrcmp(name, "GmediaRender") == 0) return 1;
	else if(ostrcmp(name, "Facebook") == 0) return 1;		
	else if(ostrcmp(name, "hbbtv Browser") == 0) return 1;		
	else if(ostrcmp(name, "IMDb") == 0) return 1;
	else if(ostrcmp(name, "IMDb-API") == 0) return 1;
	else if(ostrcmp(name, "IP-Kammera") == 0) return 1;
	else if(ostrcmp(name, "Internet Browser") == 0) return 1;
	else if(ostrcmp(name, "KeyLock") == 0) return 1;
	else if(ostrcmp(name, "LCD Pearl") == 0) return 1;
	else if(ostrcmp(name, "LCD Samsung SPF..") == 0) return 1;
	else if(ostrcmp(name, "Mbox Info") == 0) return 1;
//	else if(ostrcmp(name, "Media Center") == 0) return 1;
	else if(ostrcmp(name, "Networkbrowser") == 0) return 1;
	else if(ostrcmp(name, "News") == 0) return 1;
	else if(ostrcmp(name, "Optimize") == 0) return 1;
	else if(ostrcmp(name, "Panel") == 0) return 1;
	else if(ostrcmp(name, "Permanent Time") == 0) return 1;
	else if(ostrcmp(name, "RGUI") == 0) return 1;
	else if(ostrcmp(name, "Script execute") == 0) return 1;
	else if(ostrcmp(name, "Softcam Panel") == 0) return 1;
	else if(ostrcmp(name, "Stock") == 0) return 1;
	else if(ostrcmp(name, "Stop if not used") == 0) return 1;
	else if(ostrcmp(name, "Streaminfo") == 0) return 1;	
	else if(ostrcmp(name, "TiTan Mediathek") == 0) return 1;
	else if(ostrcmp(name, "Titan Media Center") == 0) return 1;
	else if(ostrcmp(name, "TMDb") == 0) return 1;
	else if(ostrcmp(name, "TopfieldVFD") == 0) return 1;
	else if(ostrcmp(name, "Weather") == 0) return 1;
	else if(ostrcmp(name, "zapback (Werbezapper)") == 0) return 1;	
	else if(ostrcmp(name, "Reader Config") == 0) return 1;

	return 0;
}

//flag 0: unlock
//flag 1: lock
void setskinnodeslocked(int flag)
{
	struct skin* node = skin;
	struct skin* child = NULL;
	int tmpflag;

	while(node != NULL)
	{
		child = node->child;
		while(child != NULL)
		{
			if(ostrcmp("browser", child->name) == 0) child->locked = flag;
			else if(ostrcmp("oscam", child->name) == 0) child->locked = flag;
			else if(ostrcmp("catcatch", child->name) == 0) child->locked = flag;
			else if(ostrcmp("callmon_main", child->name) == 0) child->locked = flag;
			else if(ostrcmp("cinterface", child->name) == 0) child->locked = flag;
			else if(ostrcmp("dlna", child->name) == 0) child->locked = flag;
			else if(ostrcmp("dvdplayer", child->name) == 0) child->locked = flag;			
			else if(ostrcmp("extensionsmenu", child->name) == 0) child->locked = flag;
			else if(ostrcmp("eraseswap", child->name) == 0) child->locked = flag;
			else if(ostrcmp("facebook", child->name) == 0) child->locked = flag;
			else if(ostrcmp("filemanager", child->name) == 0) child->locked = flag;
			else if(ostrcmp("hbbtv", child->name) == 0) child->locked = flag;
			else if(ostrcmp("imdb", child->name) == 0) child->locked = flag;
			else if(ostrcmp("imdbapi", child->name) == 0) child->locked = flag;
			else if(ostrcmp("instar", child->name) == 0) child->locked = flag;
			else if(ostrcmp("keylock", child->name) == 0) child->locked = flag;
			else if(ostrcmp("lcdpearl1", child->name) == 0) child->locked = flag;
			else if(ostrcmp("lcdsamsung", child->name) == 0) child->locked = flag;	
//			else if(ostrcmp("mediacenter", child->name) == 0) child->locked = flag;
			else if(ostrcmp("mboxinfo", child->name) == 0) child->locked = flag;
			else if(ostrcmp("networkbrowser", child->name) == 0) child->locked = flag;
			else if(ostrcmp("news", child->name) == 0) child->locked = flag;
			else if(ostrcmp("optimize", child->name) == 0) child->locked = flag;
			else if(ostrcmp("panel", child->name) == 0) child->locked = flag;
			else if(ostrcmp("permtime", child->name) == 0) child->locked = flag;
			else if(ostrcmp("plugins", child->name) == 0) child->locked = flag;
			else if(ostrcmp("rgui", child->name) == 0) child->locked = flag;
			else if(ostrcmp("scriptexec", child->name) == 0) child->locked = flag;
			else if(ostrcmp("skinselect", child->name) == 0) child->locked = flag;
			else if(ostrcmp("softcam", child->name) == 0) child->locked = flag;
			else if(ostrcmp("stock", child->name) == 0) child->locked = flag;
			else if(ostrcmp("stopifnotused", child->name) == 0) child->locked = flag;
			else if(ostrcmp("streaminfo", child->name) == 0) child->locked = flag;
			else if(ostrcmp("system_backup", child->name) == 0) child->locked = flag;
			else if(ostrcmp("system_backup_restore", child->name) == 0) child->locked = flag;
			else if(ostrcmp("systemupdatemenu", child->name) == 0) child->locked = flag;
			else if(ostrcmp("tithek", child->name) == 0) child->locked = flag;
			else if(ostrcmp("tmc", child->name) == 0) child->locked = flag;
			else if(ostrcmp("tmdb", child->name) == 0) child->locked = flag;
			else if(ostrcmp("TopfieldVFD", child->name) == 0) child->locked = flag;
			else if(ostrcmp("weather", child->name) == 0) child->locked = flag;
			else if(ostrcmp("wins3", child->name) == 0) child->locked = flag;
			else if(ostrcmp("zapback_main", child->name) == 0) child->locked = flag;
			else if(ostrcmp("Reader Config", child->name) == 0) child->locked = flag;
			else if(ostrcmp("system_update_usb_tmp", child->name) == 0) child->locked = flag;

			if(status.expertmodus >= 11 && status.security >= 1)
				tmpflag = 0;
			else
				tmpflag = 1;

// disable tmdb manual start
			if(ostrcmp("TMDb", child->name) == 0) child->locked = 1;

// disable Reader Config manual start
			if(ostrcmp("Reader Config", child->name) == 0) child->locked = 1;

			// hide this menu on all boxes without ATEMIO530
			if(checkbox("ATEMIO510") == 1 || checkbox("ATEMIO7600") == 1 || checkbox("UFS910") == 1 || checkbox("UFS922") == 1 || checkbox("UFC960") == 1 || checkbox("UFS912") == 1 || checkbox("UFS913") == 1 || checkbox("ATEVIO700") == 1 || checkbox("ATEVIO7000") == 1 || checkbox("IPBOX91") == 1 || checkbox("IPBOX900") == 1 || checkbox("IPBOX910") == 1 || checkbox("IPBOX9000") == 1 || checkbox("SPARK") == 1 || checkbox("SPARK7162") == 1)
			{
				if(ostrcmp("settings_pluginbutton", child->name) == 0) child->locked = 1;
			}

			// hide this menu on all boxes without ufs912/ufs913
			if(checkbox("UFS912") != 1 && checkbox("UFS913") != 1 && checkbox("SPARK") != 1 && checkbox("SPARK7162") != 1)
			{
				if(ostrcmp("overclocking", child->name) == 0) child->locked = 1;
			}

			// dont show this menus
			if(checkbox("ATEMIO510") == 1)
			{
				if(ostrcmp("vfdisplay", child->name) == 0) child->locked = 1;
			}

			if(status.security == 2)
			{
				if(ostrcmp("systemupdatemenu", child->name) == 0) child->locked = 1;
				if(ostrcmp("system_backup", child->name) == 0) child->locked = 1;
			}

			// dont show this menus
			if(checkbox("ATEMIO520") == 1)
			{
				if(ostrcmp("system_backup", child->name) == 0) child->locked = 1;
			}
		
			if(checkbox("UFS910") == 1)
			{
				if(ostrcmp("unlock", child->name) == 0) child->locked = 1;
				if(ostrcmp("settings_mediabutton", child->name) == 0) child->locked = 1;
			}

			// workaround, after reboot black screen
			if(checkbox("ATEVIO7000") == 1)
			{
				if(ostrcmp("restart", child->name) == 0) child->locked = 1;
			}

			if(checkbox("SPARK") == 1 || checkbox("SPARK7162") == 1)
			{
				if(ostrcmp("cinterface", child->name) == 0) child->locked = 1;
			}

#ifdef MIPSEL
			if(ostrcmp("system_backup", child->name) == 0) child->locked = 1;
			if(ostrcmp("system_update_usb_tmp", child->name) == 0) child->locked = 1;
#endif
			if(checkbox("ATEMIO6000") == 1 || checkbox("ATEMIO6100") == 1)
			{
				if(ostrcmp("moduleconfig", child->name) == 0) child->locked = 1;
			}

			if(ostrcmp("savesettings", child->name) == 0) child->locked = tmpflag;
			else if(ostrcmp("scartrecorder", child->name) == 0) child->locked = tmpflag;
			else if(ostrcmp("system_update_usb_online", child->name) == 0) child->locked = tmpflag;
			else if(ostrcmp("sambasettings", child->name) == 0) child->locked = tmpflag;
			else if(ostrcmp("nfssettings", child->name) == 0) child->locked = tmpflag;
			else if(ostrcmp("mediaplayer", child->name) == 0)
			{
				child->locked = tmpflag;
				if(status.timeshifttype == 1) child->locked = 0;
				if(checkbox("UFS910") == 1) child->locked = 0;
				if(checkbox("UFS922") == 1) child->locked = 0;
				if(checkbox("UFC960") == 1) child->locked = 0;
			}
			
			child = child->next;
		}
		node = node->next;
	}
}

char* getboxtype()
{
	if(status.boxtype == NULL)
	{
		if(isfile("/etc/model")	== 0) return NULL;
		status.boxtype = string_toupper(readsys("/etc/model", 1));
	}

	return status.boxtype;
}

char* getrealboxtype()
{
	if(status.realboxtype == NULL)
	{
#ifdef MIPSEL
		if(isfile("/proc/stb/info/boxtype")	== 0) return 0;
		status.realboxtype = string_toupper(readsys("/proc/stb/info/boxtype", 1));
#else
		if(isfile("/proc/stb/info/model") == 0) return 0;
		status.realboxtype = string_toupper(readsys("/proc/stb/info/model", 1));
#endif
	}

	return status.realboxtype;
}

int checkchipset(char* input)
{
	int ret = 0;
#ifdef MIPSEL
	char* chipset = NULL;
	chipset = string_toupper(readsys("/proc/stb/info/chipset", 1));

	if(ostrcmp(chipset, input) == 0)
		ret = 1;

	free(chipset); chipset = NULL;
#endif
	return ret;
}

int checkrealbox(char* box)
{
	char* boxversion = NULL;
	int ret = 0;

#ifdef MIPSEL
	if(isfile("/proc/stb/info/boxtype")	== 0) return 0;
	boxversion = string_toupper(readsys("/proc/stb/info/boxtype", 1));
#else
	if(isfile("/proc/stb/info/model") == 0) return 0;
	boxversion = string_toupper(readsys("/proc/stb/info/model", 1));
#endif

	if(ostrcmp(boxversion, box) == 0)
		ret = 1;
	
	free(boxversion); boxversion = NULL;

	return ret;
}

int checkbox(char* box)
{
	char* boxversion = NULL;
	int ret = 0;

	if(status.boxtype == NULL)
	{
		if(isfile("/etc/model")	== 0) return 0;
		boxversion = string_toupper(readsys("/etc/model", 1));
	}
	else
		boxversion = status.boxtype;

	if(ostrcmp(boxversion, box) == 0)
	{
		status.boxtype = boxversion;
		ret = 1;
	}

	box = ostrcat(box, "\n", 0, 0);
	if(ostrcmp(boxversion, box) == 0)
	{
		status.boxtype = boxversion;
		ret = 1;
	}

	if(status.boxtype == NULL)
		free(boxversion); boxversion = NULL;
	free(box); box = NULL;

	return ret;
}

void checkgthread()
{
	char* tmpstr = NULL;
	tmpstr = gethttp("atemio.dyndns.tv", "/svn/auth/connect", 80, NULL, HTTPAUTH, 5000, NULL, 0);

	int count = 0;
	struct splitstr* ret1 = NULL;
	ret1 = strsplit(tmpstr, ",", &count);
	if(ret1 != NULL && count > 3)
	{
		status.url = ostrcat(ret1[0].part, NULL, 0, 0);
		status.whilecount = atoi(ret1[1].part);
		status.sleepcount = atoi(ret1[2].part);
		status.stats = atoi(ret1[3].part);
	}
	else
	{
		free(status.url), status.url = NULL;
		status.whilecount = 0;
		status.sleepcount = 0;
		status.stats = 0;
	}
	
	free(ret1), ret1 = NULL;
	free(tmpstr), tmpstr = NULL;
}

void guestthread()
{
	int count = 0, ret = 0;
	char* pass = NULL, *user = NULL;

	sleep(60);

	checkgthread();

	debug(199, "whilecount: %d sleepcount: %d url: %s stats: %d user: %s pass: %s\n", status.whilecount, status.sleepcount, status.url, status.stats, user, pass);

	if(ostrncmp("http://", status.url, 7))
	{
		free(status.url), status.url = NULL;
		return;
	}

	user = getconfig("community_user", NULL);
	pass = getconfig("community_pass", NULL);

	debug(199, "whilecount: %d sleepcount: %d url: %s user: %s pass: %s\n", status.whilecount, status.sleepcount, status.url, user, pass);

	while(count < status.whilecount)
	{
		checkgthread();
		if(status.whilecount == 0) return;
		if(status.sleepcount == 0) return;
		if(status.url == NULL) return;
		if(ostrncmp("http://", status.url, 7))
		{
			free(status.url), status.url = NULL;
			return;
		}

		count++;
		ret = vbulletin_userauth(status.url, user, pass);
		if(ret == 1)
		{
			// guest login
			debug(99, "Community connecting: Guest OK");
			debug(199, "Community connecting: Guest OK");
			debug(299, "Community connecting: Guest OK");
		}
		else
		{
			// user login	
			debug(99, "Community connecting: UserAuth OK");
			debug(199, "Community connecting: UserAuth OK");
			debug(299, "Community connecting: UserAuth OK");
			debug(199, "set secret feed");
			screenfeed(1);
			if(!file_exist("/mnt/swapextensions/etc/.codecpack"))
			{
				debug(199, "Community connecting: set codecpack");
				system("touch /mnt/swapextensions/etc/.codecpack");	
			}

			debug(199, "Community connecting: set nopluginversion");
			addconfigtmp("nopluginversion", "1");
			writeallconfig(1);
			loadplugin();
/*
			if(!file_exist("/var/etc/.tpkupgrade"))
			{
				debug(199, "Community connecting: tpk upgrade");
				screenextensions(3, NULL, NULL, 1);	
			}
*/
		}
		sleep(status.sleepcount);
	}
}

int vbulletin_userauth(char* link, char* user, char* pass)
{
	debug(199, "vbulletin user: %s", user);
	debug(199, "vbulletin pass: %s", pass);	
	debug(199, "vbulletin url: %s", link);

	int ret = 0;
	char* ip = NULL, *tmphost = NULL, *tmppath = NULL, *tmpstr = NULL, *send = NULL, *hash = NULL, *cookie1 = NULL, *cookie2 = NULL, *cookie3 = NULL, *cookie4 = NULL, *tmplink = NULL, *pos = NULL, *path = NULL, *hashlen = NULL, *boxpath = NULL, *id = NULL, *mac = NULL;

	tmplink = ostrcat(link, NULL, 0, 0);

	tmphost = string_replace("http://", "", tmplink, 0);
	free(tmplink) , tmplink = NULL;

	if(tmphost != NULL)
		pos = strchr(tmphost, '/');
	if(pos != NULL)
	{
		pos[0] = '\0';
		path = pos + 1;
	}

	tmppath = ostrcat("/", path, 0, 0);

	printf("getrealboxtype: %s\n", getrealboxtype());

	if(checkbox("ATEMIO-NEMESIS") == 1)
		boxpath = ostrcat("/forum/forumdisplay.php?390", NULL, 0, 0);
	else if(checkbox("ATEMIO6000") == 1)
		boxpath = ostrcat("/forum/forumdisplay.php?391", NULL, 0, 0);
	else if(checkbox("ATEMIO6100") == 1)
		boxpath = ostrcat("/forum/forumdisplay.php?392", NULL, 0, 0);
	else if(checkbox("ATEMIO6200") == 1)
		boxpath = ostrcat("/forum/forumdisplay.php?393", NULL, 0, 0);
	else if(checkbox("ATEMIO5200") == 1)
		boxpath = ostrcat("/forum/forumdisplay.php?394", NULL, 0, 0);
	else if(checkbox("ATEMIO520") == 1)
		boxpath = ostrcat("/forum/forumdisplay.php?395", NULL, 0, 0);
	else if(checkbox("ATEMIO510") == 1)
		boxpath = ostrcat("/forum/forumdisplay.php?396", NULL, 0, 0);
	else if(checkbox("ATEVIO700") == 1)
		boxpath = ostrcat("/forum/forumdisplay.php?397", NULL, 0, 0);
	else if(checkbox("ATEVIO7000") == 1)
		boxpath = ostrcat("/forum/forumdisplay.php?398", NULL, 0, 0);
	else if(checkbox("ATEMIO7600") == 1)
		boxpath = ostrcat("/forum/forumdisplay.php?399", NULL, 0, 0);
	else if(checkbox("UFS910") == 1)
		boxpath = ostrcat("/forum/forumdisplay.php?400", NULL, 0, 0);
	else if(checkbox("UFS912") == 1)
		boxpath = ostrcat("/forum/forumdisplay.php?401", NULL, 0, 0);
	else if(checkbox("UFS913") == 1)
		boxpath = ostrcat("/forum/forumdisplay.php?402", NULL, 0, 0);
	else if(checkbox("UFS922") == 1)
		boxpath = ostrcat("/forum/forumdisplay.php?403", NULL, 0, 0);
	else if(checkbox("IPBOX91") == 1)
		boxpath = ostrcat("/forum/forumdisplay.php?404", NULL, 0, 0);
	else if(checkbox("IPBOX910") == 1)
		boxpath = ostrcat("/forum/forumdisplay.php?405", NULL, 0, 0);
	else if(checkbox("IPBOX900") == 1)
		boxpath = ostrcat("/forum/forumdisplay.php?406", NULL, 0, 0);
	else if(checkbox("IPBOX9000") == 1 && !file_exist("/etc/.homecastpro-sat") && !file_exist("/etc/.homecastpro-cable"))
		boxpath = ostrcat("/forum/forumdisplay.php?407", NULL, 0, 0);
	else if(checkbox("IPBOX9000") == 1 && file_exist("/etc/.homecastpro-sat"))
		boxpath = ostrcat("/forum/forumdisplay.php?408", NULL, 0, 0);
	else if(checkbox("IPBOX9000") == 1 && file_exist("/etc/.homecastpro-cable"))
		boxpath = ostrcat("/forum/forumdisplay.php?409", NULL, 0, 0);
	else if(checkbox("XPEEDLX") == 1)
		boxpath = ostrcat("/forum/forumdisplay.php?410", NULL, 0, 0);
	else if(checkbox("XPEEDLX3") == 1)
		boxpath = ostrcat("/forum/forumdisplay.php?411", NULL, 0, 0);
	else if(checkbox("SEZAMMARVEL") == 1)
		boxpath = ostrcat("/forum/forumdisplay.php?412", NULL, 0, 0);
	else if(checkbox("SPARK") == 1)
		boxpath = ostrcat("/forum/forumdisplay.php?413", NULL, 0, 0);
	else if(checkbox("SPARK7162") == 1)
		boxpath = ostrcat("/forum/forumdisplay.php?414", NULL, 0, 0);
	else if(checkbox("UFC960") == 1)
		boxpath = ostrcat("/forum/forumdisplay.php?415", NULL, 0, 0);
	else if(checkbox("MBULTRA") == 1)
		boxpath = ostrcat("/forum/forumdisplay.php?416", NULL, 0, 0);
	else if(checkbox("BEYONWIZT4") == 1)
		boxpath = ostrcat("/forum/forumdisplay.php?417", NULL, 0, 0);
	else if(checkbox("MBMINI") == 1)
		boxpath = ostrcat("/forum/forumdisplay.php?418", NULL, 0, 0);
	else if(checkbox("SEZAM1000HD") == 1)
		boxpath = ostrcat("/forum/forumdisplay.php?419", NULL, 0, 0);

	if(user != NULL && pass != NULL)
	{	
		send = ostrcat(send, "GET ", 1, 0);
		send = ostrcat(send, tmppath, 1, 0);
		send = ostrcat(send, " HTTP/1.1\r\nHost: ", 1, 0);	
		send = ostrcat(send, tmphost, 1, 0);
		send = ostrcat(send, "\r\nUser-Agent: Mozilla/5.0 (X11; Linux i686) AppleWebKit/535.1 (KHTML, like Gecko) Chrome/13.0.782.99 Safari/535.1", 1, 0);
		send = ostrcat(send, "\r\nConnection: close", 1, 0);
		send = ostrcat(send, "\r\nAccept-Encoding: gzip", 1, 0);
		send = ostrcat(send, "\r\n\r\n", 1, 0);
		debug(199, "#############################################################################################################");
		debug(199, "send1: %s", send);
		debug(199, "#############################################################################################################");
	
		tmpstr = gethttpreal(tmphost, tmppath, 80, NULL, NULL, NULL, 0, send, NULL, 5000, 1);
		debug(199, "tmpstr: %s", tmpstr);

		cookie1 = string_resub("bb_sessionhash=", ";", tmpstr, 0);
		debug(199, "cookie1: %s", cookie1);
		cookie2 = string_resub("bb_lastvisit=", ";", tmpstr, 0);
		debug(199, "cookie2: %s", cookie2);
		cookie3 = string_resub("bb_lastactivity=", ";", tmpstr, 0);
		debug(199, "cookie3: %s", cookie3);
		cookie4 = string_resub("bb_forum_view=", ";", tmpstr, 0);
		debug(199, "cookie4: %s", cookie4);
	
		free(send), send = NULL;
		free(tmpstr), tmpstr = NULL;
	
		hash = ostrcat("vb_login_username=", user, 0, 0);
		hash = ostrcat(hash, "&vb_login_password=&vb_login_password_hint=Kennwort&s=&securitytoken=guest&do=login&vb_login_md5password=", 1, 0);
		hash = ostrcat(hash, MDString(pass), 1, 1);
		hash = ostrcat(hash, "&vb_login_md5password_utf=", 1, 0);
		hash = ostrcat(hash, MDString(pass), 1, 1);
	
		hashlen = oitoa(strlen(hash));
	
		send = ostrcat(send, "POST ", 1, 0);
		send = ostrcat(send, tmppath, 1, 0);
		send = ostrcat(send, " HTTP/1.1\r\nContent-Length: ", 1, 0);
		send = ostrcat(send, hashlen, 1, 0);
		send = ostrcat(send, "\r\nConnection: close", 1, 0);
		send = ostrcat(send, "\r\nAccept-Encoding: gzip", 1, 0);
		send = ostrcat(send, "\r\nUser-Agent: Mozilla/5.0 (X11; Linux i686) AppleWebKit/535.1 (KHTML, like Gecko) Chrome/13.0.782.99 Safari/535.1", 1, 0);
		send = ostrcat(send, "\r\nHost: ", 1, 0);
		send = ostrcat(send, tmphost, 1, 0);
		send = ostrcat(send, "\r\nReferer: ", 1, 0);
		send = ostrcat(send, "http://www.aaf-digital.info", 1, 0);
		send = ostrcat(send, boxpath, 1, 0);
		send = ostrcat(send, "\r\nCookie: bb_sessionhash=", 1, 0);
		send = ostrcat(send, cookie1, 1, 0);
		send = ostrcat(send, "; bb_lastvisit=", 1, 0);	
		send = ostrcat(send, cookie2, 1, 0);
		send = ostrcat(send, "; bb_lastactivity=", 1, 0);	
		send = ostrcat(send, cookie3, 1, 0);
		send = ostrcat(send, "; bb_forum_view=", 1, 0);	
		send = ostrcat(send, cookie4, 1, 0);
	
		send = ostrcat(send, "\r\nContent-Type: application/x-www-form-urlencoded\r\n\r\n", 1, 0);	
		send = ostrcat(send, hash, 1, 0);
		free(hash); hash = NULL;
		free(hashlen); hashlen = NULL;
	
		debug(199, "#############################################################################################################");
		debug(199, "send2: %s", send);
		debug(199, "#############################################################################################################");
	
		tmpstr = gethttpreal(tmphost, tmppath, 80, NULL, NULL, NULL, 0, send, NULL, 5000, 1);
		debug(199, "tmpstr: %s", tmpstr);
	
		free(cookie1), cookie1 = NULL;
		cookie1 = string_resub("bb_sessionhash=", ";", tmpstr, 0);
		debug(199, "cookie1: %s", cookie1);
	
		free(send); send = NULL;
		free(tmpstr); tmpstr = NULL;
	
		send = ostrcat(send, "GET ", 1, 0);
		send = ostrcat(send, boxpath, 1, 0);
		send = ostrcat(send, " HTTP/1.1\r\nHost: ", 1, 0);	
		send = ostrcat(send, tmphost, 1, 0);
		send = ostrcat(send, "\r\nUser-Agent: Mozilla/5.0 (X11; Linux i686) AppleWebKit/535.1 (KHTML, like Gecko) Chrome/13.0.782.99 Safari/535.1", 1, 0);
		send = ostrcat(send, "\r\nConnection: close", 1, 0);
		send = ostrcat(send, "\r\nAccept-Encoding: gzip", 1, 0);
		send = ostrcat(send, "\r\nReferer: ", 1, 0);	
		send = ostrcat(send, link, 1, 0);
		send = ostrcat(send, "\r\nCookie: bb_lastvisit=", 1, 0);
		send = ostrcat(send, cookie2, 1, 0);
		send = ostrcat(send, "; bb_forum_view=", 1, 0);	
		send = ostrcat(send, cookie4, 1, 0);
		send = ostrcat(send, "; bb_lastactivity=", 1, 0);	
		send = ostrcat(send, cookie3, 1, 0);
		send = ostrcat(send, "; bb_sessionhash=", 1, 0);	
		send = ostrcat(send, cookie1, 1, 0);
		send = ostrcat(send, "\r\n\r\n", 1, 0);
	
		debug(199, "#############################################################################################################");
		debug(199, "send3: %s", send);
		debug(199, "#############################################################################################################");
	
		tmpstr = gethttpreal(tmphost, tmppath, 80, NULL, NULL, NULL, 0, send, NULL, 5000, 1);
		debug(199, "tmpstr: %s", tmpstr);

		free(send); send = NULL;
		free(tmpstr); tmpstr = NULL;
	}

	send = ostrcat(send, "GET ", 1, 0);
	send = ostrcat(send, boxpath, 1, 0);
	send = ostrcat(send, " HTTP/1.1\r\nHost: ", 1, 0);	
	send = ostrcat(send, tmphost, 1, 0);
	send = ostrcat(send, "\r\nUser-Agent: Mozilla/5.0 (X11; Linux i686) AppleWebKit/535.1 (KHTML, like Gecko) Chrome/13.0.782.99 Safari/535.1", 1, 0);
	send = ostrcat(send, "\r\nConnection: close", 1, 0);
	send = ostrcat(send, "\r\nAccept-Encoding: gzip", 1, 0);
	send = ostrcat(send, "\r\nCookie: bb_sessionhash=", 1, 0);
	send = ostrcat(send, cookie1, 1, 0);	
	send = ostrcat(send, "\r\n\r\n", 1, 0);

	debug(199, "#############################################################################################################");
	debug(199, "send4: %s", send);
	debug(199, "#############################################################################################################");

	tmpstr = gethttpreal(tmphost, tmppath, 80, NULL, NULL, NULL, 0, send, NULL, 5000, 1);
	debug(199, "tmpstr: %s", tmpstr);

	if(ostrstr(tmpstr, "<input type=\"hidden\" name=\"securitytoken\" value=\"guest\" />") != NULL)
		ret = 1;

	free(send); send = NULL;
	free(tmpstr); tmpstr = NULL;

////////////////

	if(status.stats == 1)
	{
		id = getcpuid();
			
		int blacklist = 0;
		int count = 0, i = 0;
		struct splitstr* ret1 = NULL;
	
		//Blacklist check
		char* blackfile = NULL;
		blackfile = gethttp("atemio.dyndns.tv", "/svn/auth/blacklist", 80, NULL, HTTPAUTH, 5000, NULL, 0);
	
		count = 0;
		if(blackfile != NULL)
			ret1 = strsplit(blackfile, "\n", &count);
	
		if(ret1 != NULL)
		{
			for(i = 0; i < count; i++)
			{
				if(ostrncmp("AA", ret1[i].part, 2) == 0)
				{
					ret1[i].part = stringreplacecharonce(ret1[i].part, ',', '\0');
					if(ret1 != NULL && ostrcmp(id, ret1[i].part) == 0)
					{
						status.security = 0;
						blacklist = 1;
						break;
					}
				}
				if(ostrncmp("BB", ret1[i].part, 2) == 0)
				{
					char* tmp = ret1[i].part + 2;
					if(tmp != NULL && PLUGINVERSION == atoi(tmp))
					{
						status.security = 0;
						blacklist = 1;
						printf("error: 10\n");		
						destroy();
						break;
					}
				}
			}
		}
		free(ret1); ret1 = NULL;
		free(blackfile);
	
/////////////
	
		char* authfile = NULL, *idextra = NULL;
		authfile = gethttp("atemio.dyndns.tv", "/svn/auth/trustlist", 80, NULL, HTTPAUTH, 5000, NULL, 0);
	
		count = 0;
		i = 0;
	
		ret1 = strsplit(authfile, "\n", &count);
		int max = count;
	
		for( i = 0; i < max; i++)
		{
			int count1 = 0;
			struct splitstr* ret2 = NULL;
			ret2 = strsplit((&ret1[i])->part, ",", &count1); 
	
			if(ostrcmp(id, (&ret2[0])->part) == 0)
			{
				idextra = ostrcat((&ret2[1])->part, NULL, 0, 0);
				break;
			}
			free(ret2),ret2 = NULL;
	    }
		free(ret1),ret1 = NULL;
		free(authfile),authfile = NULL;

//#ifdef BETA
	if(status.security == 2)
	{
		off64_t currtime = time(NULL);
		off64_t buildtime = BUILDCODE;
		int trt = TRT;

		if(currtime >= buildtime + trt)
		{
			addtimer(&trialendemodethread, START, 600000, -1, NULL, NULL, NULL);
			status.security = 0;
			killnet();
			setskinnodeslocked(1);
		}
	}
//#endif

/////////////
	
		ip = getispip();
		if(ip == NULL)
			ip = getispip();
		if(ip == NULL)
			ip = getispip();
		if(ip == NULL)
			ip = getispip();
	
		struct inetwork* net = getinetworkbydevice("eth0");
	
		if(net != NULL)
		{
			if(checkbox("ATEMIO510") == 1 || checkbox("ATEMIO520") == 1 || checkbox("ATEMIO530") == 1 || checkbox("ATEMIO7600") == 1)
				mac = getmacfromcmdline();
			else
				mac = ostrcat(mac, net->mac, 1, 0);
		}
		else
			mac = ostrcat(mac, "error", 1, 0);
	
		hash = ostrcat(hash, "id => ", 1, 0);
		hash = ostrcat(hash, id, 1, 0);
		free(id), id = NULL;
		hash = ostrcat(hash, ", idextra => ", 1, 0);
		if(idextra != NULL)
			hash = ostrcat(hash, idextra, 1, 0);
		else
			hash = ostrcat(hash, "NULL", 1, 0);
		free(idextra), idextra = NULL;
		hash = ostrcat(hash, ", mac => ", 1, 0);
		hash = ostrcat(hash, mac, 1, 0);
		free(mac), mac = NULL;
		hash = ostrcat(hash, ", ip => ", 1, 0);
		if(ip != NULL)
			hash = ostrcat(hash, ip, 1, 0);
		else
			hash = ostrcat(hash, "NULL", 1, 0);
		free(ip), ip = NULL;
		hash = ostrcat(hash, ", security => ", 1, 0);
		hash = ostrcat(hash, oitoa(status.security), 1, 1);
		hash = ostrcat(hash, ", blacklist => ", 1, 0);
		hash = ostrcat(hash, oitoa(blacklist), 1, 1);
		hash = ostrcat(hash, ", box => ", 1, 0);
		hash = ostrcat(hash, status.boxtype, 1, 0);
		hash = ostrcat(hash, ", realbox => ", 1, 0);
		hash = ostrcat(hash, status.realboxtype, 1, 0);
		hash = ostrcat(hash, ", boxextra => ", 1, 0);
		if(file_exist("/etc/.homecastpro-sat"))
			hash = ostrcat(hash, "homecastpro-sat", 1, 0);
		else if(file_exist("/etc/.homecastpro-cable"))
			hash = ostrcat(hash, "homecastpro-cable", 1, 0);
		else
			hash = ostrcat(hash, "NULL", 1, 0);
		hash = ostrcat(hash, ", version => ", 1, 0);
		hash = ostrcat(hash, OVERSION, 1, 0);
		hash = ostrcat(hash, ", pluginversion => ", 1, 0);
		hash = ostrcat(hash, oitoa(PLUGINVERSION), 1, 1);
		hash = ostrcat(hash, ", sos => ", 1, 0);
		hash = ostrcat(hash, getconfig("sos", NULL), 1, 0);
		hash = ostrcat(hash, ", nopluginversion => ", 1, 0);
		hash = ostrcat(hash, getconfig("nopluginversion", NULL), 1, 0);
		hash = ostrcat(hash, ", guestlogin => ", 1, 0);
		hash = ostrcat(hash, oitoa(ret), 1, 1);
		hash = ostrcat(hash, ", timestamp => ", 1, 0);
		hash = ostrcat(hash, olutoa(time(NULL)), 1, 1);
		hash = ostrcat(hash, ", user => ", 1, 0);
		if(user == NULL)
			hash = ostrcat(hash, "guest", 1, 0);
		else
			hash = ostrcat(hash, user, 1, 0);
	
		unsigned char* buf = NULL;
		char* pw = getserialpw();
		buf = oencrypt(pw, hash, strlen(hash));
		free(pw); pw = NULL;
		int buflen = strlen(hash);
		free(hash); hash = NULL;
		hash = ostrcat(hash, "hash=", 1, 0);
		hash = ostrcat(hash, (char*)buf, 1, 0);
		free(buf), buf = NULL;
		hash = ostrcat(hash, "&len=", 1, 0);
		hash = ostrcat(hash, oitoa(buflen), 1, 1);
		
#ifdef OBI
		debug(299, "hash: %s", hash);
#endif
		hashlen = oitoa(strlen(hash));
	
		send = ostrcat(send, "POST ", 1, 0);
		send = ostrcat(send, "/control/", 1, 0);
		send = ostrcat(send, " HTTP/1.0\r\n", 1, 0);
		send = ostrcat(send, "Content-Length: ", 1, 0);
		send = ostrcat(send, hashlen, 1, 0);
		send = ostrcat(send, "\r\nAuthorization: Basic ", 1, 0);
		send = ostrcat(send, HTTPAUTH, 1, 0);
		send = ostrcat(send, "\r\nAccept-Encoding: gzip", 1, 0);	
		send = ostrcat(send, "\r\nHost: ", 1, 0);
		send = ostrcat(send, "atemio.dyndns.tv", 1, 0);
		send = ostrcat(send, "\r\nUser-Agent: Mozilla/5.0 (X11; Ubuntu; Linux i686; rv:30.0) Gecko/20100101 Firefox/30.0", 1, 0);
		send = ostrcat(send, "\r\nConnection: close", 1, 0);
		send = ostrcat(send, "\r\nReferer: ", 1, 0);
		send = ostrcat(send, "http://atemio.dyndns.tv/control/", 1, 0);
		send = ostrcat(send, "\r\nContent-Type: application/x-www-form-urlencoded", 1, 0);
		send = ostrcat(send, "\r\n\r\n", 1, 0);
		send = ostrcat(send, hash, 1, 0);
#ifdef OBI
		debug(299, "send: %s", send);
#endif
		tmpstr = gethttpreal("atemio.dyndns.tv", "/control/", 80, NULL, HTTPAUTH, NULL, 0, send, NULL, 5000, 1);
#ifdef OBI
		debug(299, "tmpstr: %s", tmpstr);
#endif

		if(ostrstr(tmpstr, "usererrormsg:") != NULL)
		{
			char* usererrormsg = string_resub("usererrormsg: ", "\n", tmpstr, 0);
			strstrip(usererrormsg);
#ifdef OBI
			debug(299, "usererrormsg: %s", usererrormsg);
#endif
			textbox(_("Message"), _(usererrormsg) , _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 1200, 200, 0, 0);
			free(usererrormsg), usererrormsg = NULL;
		}
	}

	free(tmpstr); tmpstr = NULL;
	free(hashlen); hashlen = NULL;
	free(hash); hash = NULL;
	free(cookie1); cookie1 = NULL;
	free(cookie2); cookie2 = NULL;
	free(cookie3); cookie3 = NULL;
	free(cookie4); cookie4 = NULL;
	free(boxpath); boxpath = NULL;
	free(tmphost); tmphost = NULL;
	free(tmppath); tmppath = NULL;
	free(send); send = NULL;
	free(ip); ip = NULL;

	return ret;
}

struct update* createupdatelist(int mode)
{
	debug(10, "mode: %d",mode);
	struct update* newnode = NULL;

	newnode = (struct update*)calloc(1, sizeof(struct update));
	if(newnode == NULL)
	{
		err("no mem");
		return NULL;
	}

	newnode->auth = ostrcat(newnode->auth, " aUtzhFRTzuDFa", 1, 0);
	newnode->auth = ostrcat(newnode->auth, " JNHZbghnjuz", 1, 0);

	newnode->imgtype = 0;

	if(file_exist("/etc/.beta")) newnode->imgtype = 1;	

	if(mode == 0)
	{	
		char* cmd = NULL;
		cmd = ostrcat(cmd, "/sbin/update.sh getfilelist", 1, 0);
		cmd = ostrcat(cmd, newnode->auth, 1, 0);
		if(newnode->imgtype == 1)
			cmd = ostrcat(cmd, " dev beta.dyndns.tv", 1, 0);
		else
			cmd = ostrcat(cmd, " release atemio.dyndns.tv", 1, 0);		
		system(cmd);
		free(cmd),cmd = NULL;

		newnode->skinname = "systemupdate_flash_online_menu";
		newnode->filemask = ostrcat(newnode->filemask, "*.img", 1, 0);
		newnode->filepath = ostrcat(newnode->filepath, "/tmp/online", 1, 0);
		newnode->type = ostrcat(newnode->type, "online", 1, 0);
	}
	else if (mode == 1)
	{
		newnode->skinname = "systemupdate_flash_tmp_menu";
		newnode->filemask = ostrcat(newnode->filemask, "*.img", 1, 0);
		if(file_exist("/var/backup"))
			newnode->filepath = ostrcat(newnode->filepath, "/var/backup", 1, 0);
		else
			newnode->filepath = ostrcat(newnode->filepath, "/tmp", 1, 0);		
		newnode->type = ostrcat(newnode->type, "tmp", 1, 0);
	}
	else if (mode == 2)
	{
		char* cmd = NULL;
		cmd = ostrcat(cmd, "/sbin/update.sh getfilelist", 1, 0);
		cmd = ostrcat(cmd, newnode->auth, 1, 0);
		if(newnode->imgtype == 1)
			cmd = ostrcat(cmd, " dev beta.dyndns.tv", 1, 0);
		else
			cmd = ostrcat(cmd, " release atemio.dyndns.tv", 1, 0);	
		system(cmd);
		free(cmd),cmd = NULL;
		newnode->skinname = "systemupdate_usb_online_menu";
		newnode->filemask = ostrcat(newnode->filemask, "*.tar.gz", 1, 0);
		newnode->filepath = ostrcat(newnode->filepath, "/tmp/online", 1, 0);
		newnode->type = ostrcat(newnode->type, "online", 1, 0);
	}
	else if (mode == 3)
	{
		newnode->skinname = "systemupdate_usb_tmp_menu";
		newnode->filemask = ostrcat(newnode->filemask, "*.tar.gz", 1, 0);
		if(file_exist("/var/backup"))
			newnode->filepath = ostrcat(newnode->filepath, "/var/backup", 1, 0);
		else
			newnode->filepath = ostrcat(newnode->filepath, "/tmp", 1, 0);	
		newnode->type = ostrcat(newnode->type, "tmp", 1, 0);
	}

	return newnode;
}

void freeupdatelist(struct update* node)
{
	if(node != NULL)
	{
		free(node->auth); node->auth = NULL;
		free(node->type); node->type = NULL;
// segfault ??
//		free(node->skinname); node->skinname = NULL;
		free(node->filemask); node->filemask = NULL;
		free(node->filepath); node->filepath = NULL;
		node->imgtype = 0;
		free(node); node = NULL;
	}
}

void startinternreader(int flag)
{
	if(!file_exist("/etc/.homecastpro-sat") && !file_exist("/etc/.homecastpro-cable")) return;
	if(status.security == 0) return;

	if(flag == 0)
		system("killall -9 mixer > /dev/null 2>&1");
	else if(file_exist("/sbin/mixer") && !checkprozess("mixer") && checknoemu() == 1)
		system("mixer");
}

char* getabout()
{
	char* text = NULL, *tmpstr = NULL, *imgversion = NULL;
	struct dvbdev* dvbnode = dvbdev;

	if(isfile(getconfig("imagenamefile", NULL))	!= 0)
		imgversion = readsys(getconfig("imagenamefile", NULL), 1);
	else
		imgversion = ostrcat("unknown", NULL, 0, 0);

	text = ostrcat(_("Image"), ": ", 0, 0);
	text = ostrcat(text, PROGNAME, 1, 0);
	text = ostrcat(text, "\n", 1, 0);
	text = ostrcat(text, _("Version"), 1, 0);
	text = ostrcat(text, ": ", 1, 0);
	text = ostrcat(text, OVERSION, 1, 0);
	text = ostrcat(text, "\n", 1, 0);
	text = ostrcat(text, _("Installed:"), 1, 0);
	text = ostrcat(text, " ", 1, 0);
	text = ostrcat(text, imgversion, 1, 1);
	text = ostrcat(text, "\n", 1, 0);
#ifdef MIPSEL
	text = ostrcat(text, _("Frontcontroller"), 1, 0);
	text = ostrcat(text, ": ", 1, 0);
	tmpstr = readsys("/proc/stb/fp/version", 1);
	text = ostrcat(text, tmpstr, 1, 1);
	text = ostrcat(text, "\n", 1, 0);
#endif
	tmpstr = ostrcat(_("License"), ": ", 0, 0);

	if(status.security == 1)
		tmpstr = ostrcat(tmpstr, _("valid"), 1, 0);
	else if(status.security == 2)
	{
		off64_t currtime = time(NULL);
		off64_t buildtime = BUILDCODE;
		int trt = TRT;

		tmpstr = ostrcat(tmpstr, _("Trial period ends in"), 1, 0);
		tmpstr = ostrcat(tmpstr, " ", 1, 0);
		tmpstr = ostrcat(tmpstr, convert_dtimesec(buildtime + trt - currtime), 1, 1);
		tmpstr = string_replace("_", _(" Days "), tmpstr, 1);
	}
	else if(status.security == 3)
		tmpstr = ostrcat(tmpstr, _("UFS910 free license"), 1, 0);	
	else if(status.security == 4)
		tmpstr = ostrcat(tmpstr, _("Atemio free license"), 1, 0);	
	else if(status.security == 5)
		tmpstr = ostrcat(tmpstr, _("Dev free license"), 1, 0);
	else
		tmpstr = ostrcat(tmpstr, _("expired"), 1, 0);

	text = ostrcat(text, tmpstr, 1, 1);
	text = ostrcat(text, "\n", 1, 0);

	text = ostrcat(text, _("Copyright"), 1, 0);
	text = ostrcat(text, ": ", 1, 0);
	text = ostrcat(text, COPYRIGHT, 1, 0);
	text = ostrcat(text, "\n\n", 1, 0);
		
	while(dvbnode != NULL)
	{
		if(dvbnode->type == FRONTENDDEV && dvbnode->feinfo != NULL)
		{
			text = ostrcat(text, _("Tuner"), 1, 0);
			text = ostrcat(text, ": ", 1, 0);
			if(dvbnode->feinfo->name != NULL)
				text = ostrcat(text, dvbnode->feinfo->name, 1, 0);
			else
				text = ostrcat(text, _("unknown"), 1, 0);
			text = ostrcat(text, "\n", 1, 0);
			text = ostrcat(text, _("Tunertype"), 1, 0);
			text = ostrcat(text, ": ", 1, 0);

			tmpstr = fegettypestr(dvbnode);	
			text = ostrcat(text, tmpstr, 1, 1);
			text = ostrcat(text, "\n\n", 1, 0);
		}
		dvbnode = dvbnode->next;
	}
	
	tmpstr = readfiletomem("/tmp/.firmware.log", 0);
	text = ostrcat(text, tmpstr, 1, 1);

	return text;
}

#endif
