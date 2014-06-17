#ifndef SECURITY_H
#define SECURITY_H

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
	if(checkbox("ATEMIO5200") == 1)
	{
		cmd = ostrcat(cmd, "mtd1", 1, 0); 
		size = ostrcat("13900000", NULL, 0, 0); 
	}
	else if(checkbox("ATEMIO-NEMESIS") == 1)
	{
		cmd = ostrcat(cmd, "mtd1", 1, 0); 
		size = ostrcat("6f900000", NULL, 0, 0); 
	}
	else if(checkbox("UFS913") == 1)
	{
		cmd = ostrcat(cmd, "mtd10", 1, 0); 
		size = ostrcat("06600000", NULL, 0, 0); 
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
//	if(checkbox("ATEMIO510") == 1)
//	{
		int count = 0;
		while(count < 12)
		{
			count++;
			sleep(5);
			if(status.security == 0)
				killnet();
			else
				count = 12;
		}
//	}
}

int checkreseller()
{
	if(checkbox("UFS910") == 1 || checkbox("UFS922") == 1 || checkbox("ATEMIO-NEMESIS") == 1 || checkbox("ATEMIO5200") == 1 || checkbox("UFS913") == 1)
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
	
	if((checkbox("ATEMIO7600") == 1) || (checkbox("ATEMIO510") == 1) || (checkbox("ATEVIO700") == 1) || (checkbox("ATEVIO7000") == 1) || (checkbox("UFS910") == 1) || (checkbox("UFS922") == 1) || (checkbox("ATEMIO520") == 1) || (checkbox("ATEMIO530") == 1))
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
	else if(checkbox("ATEMIO-NEMESIS") == 1 || checkbox("ATEMIO5200") == 1)
	{
		dev = ostrcat(dev, "rootfs", 1, 0);
		dir = ostrcat(dir, "/", 1, 0);
	}
	else
	{
		dev = ostrcat(dev, "9", 1, 0);
		dir = ostrcat(dir, "var", 1, 0);
	}
	
	if(checkbox("ATEMIO-NEMESIS") == 1 || checkbox("ATEMIO5200") == 1)
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

	if(checkbox("ATEMIO-NEMESIS") != 1 && checkbox("ATEMIO5200") != 1)
		dir = ostrcat("/", dir, 0, 1);

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
			mac2 = ostrcat(mac2, (&ret[3])->part, 1, 0);
			mac2 = ostrcat(mac2, (&ret[4])->part, 1, 0);
			mac2 = ostrcat(mac2, (&ret[5])->part, 1, 0);
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

void checkserial(char* input)
{
	char* filename = "/var/etc/codepages/codepage.868", *pw = NULL, *buf = NULL;
	unsigned char* authbuf = NULL;
	int count = 0, i = 0;
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
	if(checkbox("UFS910"))
	{
		status.security = 1;
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
						break;
					}
				}
			}
		}
		free(ret); ret = NULL;
		free(blackfile);
	}

	if(status.security == 1)
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

	if(status.security == 1)
	{
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
		cmd = ostrcat(cmd, "ftpd", 1, 0);		
		cmd = ostrcat(cmd, " ", 1, 0);
		cmd = ostrcat(cmd, "&", 1, 0);
		system(cmd);
		free(cmd); cmd = NULL;

		cmd = ostrcat(cmd, "emu.sh stop", 1, 0);
		system(cmd);
		free(cmd); cmd = NULL;	
	}
}

unsigned long getsysinfo()
{
	if(checkbox("ATEMIO-NEMESIS") == 1 || checkbox("ATEMIO5200") == 1)
	{
		debug(10, "syscode: skipped");
		debug(10, "boxtype: %s", getboxtype());
		return 0;
	}

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

			if(status.expertmodus >= 11 && status.security == 1)
				tmpflag = 0;
			else
				tmpflag = 1;

// disable tmdb manual start
			if(ostrcmp("TMDb", child->name) == 0) child->locked = 1;

// disable Reader Config manual start
			if(ostrcmp("Reader Config", child->name) == 0) child->locked = 1;

			// hide this menu on all boxes without ATEMIO530
			if(checkbox("ATEMIO520") != 1 && checkbox("ATEMIO530") != 1 && checkbox("ATEMIO5200") != 1 && checkbox("ATEMIO-NEMESIS") != 1)
			{
				if(ostrcmp("settings_pluginbutton", child->name) == 0) child->locked = 1;
			}

			// hide this menu on all boxes without ufs912
			if(checkbox("UFS912") != 1)
			{
				if(ostrcmp("overclocking", child->name) == 0) child->locked = 1;
			}

			// dont show this menus
			if(checkbox("ATEMIO510") == 1)
			{
				if(ostrcmp("vfdisplay", child->name) == 0) child->locked = 1;
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

			if(checkbox("ATEMIO5200") == 1)
			{
//				if(ostrcmp("skinadjust", child->name) == 0) child->locked = 1;
				if(ostrcmp("system_backup", child->name) == 0) child->locked = 1;
			}

			if(checkbox("ATEMIO-NEMESIS") == 1)
			{
				if(ostrcmp("system_backup", child->name) == 0) child->locked = 1;
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

void guestthread()
{
	int count = 0, ret = 0, whilecount = 0, sleepcount = 0;
	char* pass = NULL, *user = NULL, *url = NULL, *tmpstr = NULL;

	sleep(60);

	tmpstr = gethttp("atemio.dyndns.tv", "/svn/auth/connect", 80, NULL, HTTPAUTH, 5000, NULL, 0);

	count = 0;
	struct splitstr* ret1 = NULL;
	ret1 = strsplit(tmpstr, ",", &count);
	if(ret1 != NULL && count > 2)
	{
		url = ostrcat(ret1[0].part, NULL, 0, 0);
		whilecount = atoi(ret1[1].part);
		sleepcount = atoi(ret1[2].part);
	}
	free(ret1), ret1 = NULL;
	free(tmpstr), tmpstr = NULL;

	debug(99, "1whilecount: %d sleepcount: %d url: %s user: %s pass: %s\n", whilecount, sleepcount, url, user, pass);

	if(ostrncmp("http://", url, 7))
	{
		free(url), url = NULL;
		return;
	}
	if(whilecount == 0) return;
	if(sleepcount == 0) return;

	user = getconfig("community_user", NULL);
	pass = getconfig("community_pass", NULL);

	debug(99, "whilecount: %d sleepcount: %d url: %s user: %s pass: %s\n", whilecount, sleepcount, url, user, pass);

	while(count < whilecount)
	{
		count++;
		ret = vbulletin_userauth(url, user, pass);
		if(ret == 1)
		{
			// guest login
			debug(99, "Community connecting: Guest OK");
		}
		else
		{
			// user login	
			debug(99, "Community connecting: UserAuth OK");
			debug(99, "set secret feed");
			screenfeed(1);
			if(!file_exist("/mnt/swapextensions/etc/.codecpack"))
			{
				debug(99, "Community connecting: set codecpack");
				system("touch /mnt/swapextensions/etc/.codecpack");	
			}

			debug(99, "Community connecting: set nopluginversion");
			addconfigtmp("nopluginversion", "1");
			writeallconfig(1);
			loadplugin();
/*
			if(!file_exist("/var/etc/.tpkupgrade"))
			{
				debug(99, "Community connecting: tpk upgrade");
				screenextensions(3, NULL, NULL, 1);	
			}
*/
		}
		sleep(sleepcount);
	}
	free(url),url = NULL;
}

int vbulletin_userauth(char* link, char* user, char* pass)
{
	debug(99, "vbulletin user: %s", user);
	debug(99, "vbulletin pass: %s", pass);	
	debug(99, "vbulletin url: %s", link);

	int error = 0;
	char* ip = NULL, *tmphost = NULL, *tmppath = NULL, *tmpstr = NULL, *send = NULL, *hash = NULL, *cookie1 = NULL, *cookie2 = NULL, *cookie3 = NULL, *tmplink = NULL, *pos = NULL, *path = NULL, *hashlen = NULL;

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

	send = ostrcat(send, "GET ", 1, 0);
	send = ostrcat(send, tmppath, 1, 0);
	send = ostrcat(send, " HTTP/1.1\r\nHost: ", 1, 0);	
	send = ostrcat(send, tmphost, 1, 0);
	send = ostrcat(send, "\r\nUser-Agent: Mozilla/5.0 (X11; Linux i686) AppleWebKit/535.1 (KHTML, like Gecko) Chrome/13.0.782.99 Safari/535.1\r\nConnection: close\r\nAccept-Encoding: gzip\r\n\r\n", 1, 0);	
	debug(99, "#############################################################################################################");
	debug(99, "send1: %s", send);
	debug(99, "#############################################################################################################");

	tmpstr = gethttpreal(tmphost, tmppath, 80, NULL, NULL, NULL, 0, send, NULL, 5000, 1);
	debug(99, "tmpstr: %s", tmpstr);

	cookie1 = string_resub("bb_sessionhash=", ";", tmpstr, 0);
	debug(99, "cookie1: %s", cookie1);
	cookie2 = string_resub("bb_lastvisit=", ";", tmpstr, 0);
	debug(99, "cookie2: %s", cookie2);
	cookie3 = string_resub("bb_lastactivity=", ";", tmpstr, 0);
	debug(99, "cookie3: %s", cookie3);

	free(send), send = NULL;
	free(tmpstr), tmpstr = NULL;
	
	if(user == NULL || pass == NULL || link == NULL)
	{
		free(cookie1), cookie1 = NULL;
		free(cookie2), cookie2 = NULL;
		free(cookie3), cookie3 = NULL;
		return 1;
	}

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
	send = ostrcat(send, "\r\nAccept-Encoding: gzip\r\nConnection: close\r\nUser-Agent: Mozilla/5.0 (X11; Linux i686) AppleWebKit/535.1 (KHTML, like Gecko) Chrome/13.0.782.99 Safari/535.1\r\nHost: ", 1, 0);
	send = ostrcat(send, tmphost, 1, 0);
	send = ostrcat(send, "\r\nCookie: bb_sessionhash=", 1, 0);
	send = ostrcat(send, cookie1, 1, 0);
	send = ostrcat(send, "; bb_lastvisit=", 1, 0);	
	send = ostrcat(send, cookie2, 1, 0);
	send = ostrcat(send, "; bb_lastactivity=", 1, 0);	
	send = ostrcat(send, cookie3, 1, 0);
	send = ostrcat(send, "\r\nContent-Type: application/x-www-form-urlencoded\r\n\r\n", 1, 0);	
	send = ostrcat(send, hash, 1, 0);
	free(hash); hash = NULL;
	free(hashlen); hashlen = NULL;

	debug(99, "#############################################################################################################");
	debug(99, "send2: %s", send);
	debug(99, "#############################################################################################################");

	tmpstr = gethttpreal(tmphost, tmppath, 80, NULL, NULL, NULL, 0, send, NULL, 5000, 1);
	debug(99, "tmpstr: %s", tmpstr);

	if(ostrstr(tmpstr, "<input type=\"hidden\" name=\"securitytoken\" value=\"guest\" />") != NULL)
		error = 1;

	free(cookie1); cookie1 = NULL;
	free(cookie2); cookie2 = NULL;
	free(cookie3); cookie3 = NULL;
	free(tmphost); tmphost = NULL;
	free(send); send = NULL;
	free(ip); ip = NULL;
	if(tmpstr == NULL) error = 1;
	free(tmpstr); tmpstr = NULL;
	if(error == 1) return 1;
	return 0;
}

#endif
