#ifndef GLOBAL_H
#define GLOBAL_H

int checklowflash()
{
	char* cmd = NULL;
	char* tmpstr = NULL;
	cmd = ostrcat(cmd, "cat /proc/mtd | grep mtd5 | awk '{print $2}'", 1, 0);

	tmpstr = string_newline(command(cmd));
	free(cmd), cmd = NULL;

	if(tmpstr == NULL)
	{
		return 1;
	}

	if(ostrcmp(tmpstr, "00fc0000") == 0)
	{
		free(tmpstr), tmpstr = NULL;
		status.security = 1;
		setskinnodeslocked(0);

		return 0;
	}
	return 1;	
}

char* unhexlify(const char *hexstr)
{
	int len = 0, tmpint = 0;
	char *p, *q, *binstr = NULL;

	if(hexstr == NULL) return NULL;

	len = strlen(hexstr);
	if(len == 0 || len % 2 != 0) return NULL;

	binstr = calloc(1, (len / 2) + 1);
	if(binstr == NULL)
	{
		err("no mem");
		return NULL;
	}

	printf("%p\n", binstr);
	for(p = hexstr, q = binstr; *p; p += 2, q++)
	{
		sscanf(p, "%2x", &tmpint);
		*q = tmpint;
	}
	printf("%p\n", binstr);

	return binstr;
}

int getsupermagic(char* filename)
{
	struct statfs64 s;

	if(statfs64(filename, &s) >= 0)
		return s.f_type;

	return 0;
}

int getrandom(int max)
{
	srand(time(NULL));
	return (rand() % max);
}

//check if swap is in flash(1) or not(0)
int checkswaplink()
{
	int ret = 1;
	char* tmpstr = NULL;

	tmpstr = realpath("/var/swap", NULL);
	if(tmpstr != NULL && ostrcmp(tmpstr, "/mnt/swapextensions") != 0)
		ret = 0;

	free(tmpstr); tmpstr = NULL;
	return ret;
}

char* delmountpart(char* filename, int free1)
{
	struct mntent ent;
	FILE *fd = NULL;
	char* ret = NULL, *buf = NULL, *treffer = NULL, *rpath = NULL;
	int mntdirlen = 0;

	if(filename == NULL) return NULL;

	buf = malloc(MINMALLOC);
	if(buf == NULL)
	{
		err("no mem");
		return NULL;
	}

	rpath = realpath(filename, NULL);

	fd = setmntent("/proc/mounts", "r");
	if(fd != NULL)
	{
		while(getmntent_r(fd, &ent, buf, MINMALLOC) != 0)
		{
			if(ent.mnt_dir != NULL)
			{
				mntdirlen = strlen(ent.mnt_dir);
				if(mntdirlen > 1 && ostrstr(rpath, ent.mnt_dir) == rpath)
				{
					if(treffer == NULL || strlen(treffer) < mntdirlen)
					{
						free(treffer); treffer = NULL;
						treffer = ostrcat(ent.mnt_dir, NULL, 0, 0);
					}
				}
			}
		}
	}

	if(treffer != NULL)
		ret = string_replace(treffer, "", rpath, 0);

	endmntent(fd);
	if(free1 == 1) free(filename);
	free(buf);
	free(treffer);
	free(rpath);
	return ret;
}

char* addmountpart(char* filename, int free1)
{
	struct mntent ent;
	FILE *fd = NULL;
	char* ret = NULL, *buf = NULL;

	if(filename == NULL) return NULL;

	buf = malloc(MINMALLOC);
	if(buf == NULL)
	{
		err("no mem");
		return NULL;
	}

	fd = setmntent("/proc/mounts", "r");
	if(fd != NULL)
	{
		while(getmntent_r(fd, &ent, buf, MINMALLOC))
		{
			if(ent.mnt_dir != NULL && strlen(ent.mnt_dir) > 1)
			{
				ret = ostrcat(ent.mnt_dir, filename, 0, 0);
				if(file_exist(ret))
				{
					if(free1 == 1) free(filename);
					endmntent(fd);
					free(buf);
					return ret;
				}
				free(ret); ret = NULL;
			}
		}
	}

	endmntent(fd);
	free(buf);
	return ret;
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
		err("no memory");
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

unsigned int gethash(char* str)
{
	unsigned int hash = 0;
	int c = 0;

	if(str == NULL) return 0;

	while((c = *str++))
		hash = ((hash << 5) + hash) ^ c;

	return hash;
}

void freeregexstruct(struct regex* node)
{
	if(node == NULL) return;

	free(node->match1); node->match1 = NULL;
	free(node->match2); node->match2 = NULL;
	free(node->match3); node->match3 = NULL;
	free(node->match4); node->match4 = NULL;
	free(node->match5); node->match5 = NULL;
	free(node->match6); node->match6 = NULL;
	free(node->match7); node->match7 = NULL;
	free(node->match8); node->match8 = NULL;
	free(node->match9); node->match9 = NULL;
	free(node->match10); node->match10 = NULL;

	free(node); node = NULL;
}

struct regex* regexstruct(char* regex, char* str)
{
	regex_t preg = {NULL};
	regmatch_t pmatch[11] = {{0}};
	size_t rm = 0, i = 0, len = 0;
	char* ret = NULL;
	struct regex* node = NULL;

	if(regex == NULL || str == NULL) return NULL;

	rm = regcomp(&preg, regex, REG_EXTENDED);
	if(rm != 0) //error in regex
	{
		regfree(&preg);
		return NULL;
	}

	rm = regexec(&preg, str, 11, pmatch, 0);
	if(rm != 0) //no match
	{
		regfree(&preg);
		return NULL;
	}

	node = (struct regex*)calloc(1, sizeof(struct regex));
	if(node == NULL)
	{
		err("no mem");
		regfree(&preg);
		return NULL;
	}

	for(i = 1; !rm && i <= preg.re_nsub; i++)
	{
		len = pmatch[i].rm_eo - pmatch[i].rm_so;
		if(len < 1) continue;

		ret = malloc(len + 1);
		if(ret != NULL)
		{
			memcpy(ret, str + pmatch[i].rm_so, len);
			ret[len] = '\0';
		}

		if(i == 1) node->match1 = ret;
		else if(i == 2) node->match2 = ret;
		else if(i == 3) node->match3 = ret;
		else if(i == 4) node->match4 = ret;
		else if(i == 5) node->match5 = ret;
		else if(i == 6) node->match6 = ret;
		else if(i == 7) node->match7 = ret;
		else if(i == 8) node->match8 = ret;
		else if(i == 9) node->match9 = ret;
		else if(i == 10) node->match10 = ret;

		ret = NULL;
	}

	regfree(&preg);
	return node;
}

char* oregex(char* regex, char* str)
{
	regex_t preg = {NULL};
	regmatch_t pmatch[2] = {{0}};
	size_t rm = 0, len = 0;
	char* ret = NULL;

	if(regex == NULL || str == NULL) return NULL;

	rm = regcomp(&preg, regex, REG_EXTENDED);
	if(rm != 0) //error in regex
	{
		regfree(&preg);
		return NULL;
	}

	rm = regexec(&preg, str, 2, pmatch, 0);
	if(rm != 0) //no match
	{
		regfree(&preg);
		return NULL;
	}

	len = pmatch[1].rm_eo - pmatch[1].rm_so;
	if(len < 1)
	{
		regfree(&preg);
		return NULL;
	}

	ret = malloc(len + 1);
	if(ret != NULL)
	{
		memcpy(ret, str + pmatch[1].rm_so, len);
		ret[len] = '\0';
	}

	regfree(&preg);
	return ret;
}

int oregexint(char* regex, char* str)
{
	int iret = 0;
	char* ret = NULL;

	ret = oregex(regex, str);
	if(ret != NULL) iret = atoi(ret);
	free(ret);

	return iret;
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
	if(checkbox("ATEMIO510") == 0 && checkbox("ATEMIO7600") == 0)
	{
		printf("ResellerId: skipped\n");
		printf("boxtype: %s\n", status.boxtype);	
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
			printf("ResellerId: found (%s) reseller !\n", status.boxtype);
			free(buf);
			fclose(fd);
			return 0;
		}
	}
	else if(checkbox("ATEMIO7600") == 1)
	{
		if((buf[1072] & 0xff) == 0x23 && (buf[1073] & 0xff) == 0x03 && (buf[1074] & 0xff) == 0x00 && (buf[1075] & 0xff) == 0xA0)
		{
			printf("ResellerId: found (%s) reseller !\n", status.boxtype);
			free(buf);
			fclose(fd);
			return 0;
		}
	}
	else if(checkbox("UFS910") == 1)
	{
		if((buf[1072] & 0xff) == 0x03 && (buf[1073] & 0xff) == 0x00 && (buf[1074] & 0xff) == 0x00 && (buf[1075] & 0xff) == 0x00)
		{
			printf("ResellerId: found (%s) reseller !\n", status.boxtype);
			free(buf);
			fclose(fd);
			return 0;
		}
	}
	else if(checkbox("UFS912") == 1)
	{
		if((buf[1072] & 0xff) == 0x2d && (buf[1073] & 0xff) == 0x41 && (buf[1074] & 0xff) == 0x04 && (buf[1075] & 0xff) == 0xd2)
		{
			printf("ResellerId: found (%s) reseller !\n", status.boxtype);
			free(buf);
			fclose(fd);
			return 0;
		}
	}
	else if(checkbox("AT7000") == 1)
	{
		if((buf[1072] & 0xff) == 0x02 && (buf[1073] & 0xff) == 0x27 && (buf[1074] & 0xff) == 0x12 && (buf[1075] & 0xff) == 0x22)
		{
			printf("ResellerId: found (%s) reseller !\n", status.boxtype);
			free(buf);
			fclose(fd);
			return 0;
		}
	}
	else if(checkbox("AT7500") == 1)
	{
		if((buf[1072] & 0xff) == 0x09 && (buf[1073] & 0xff) == 0x00 && (buf[1074] & 0xff) == 0x09 && (buf[1075] & 0xff) == 0x00)
		{
			printf("ResellerId: found (%s) reseller !\n", status.boxtype);
			free(buf);
			fclose(fd);
			return 0;
		}
	}
	else if(checkbox("SKYSAT") == 1)
	{
		if((buf[1072] & 0xff) == 0x25 && (buf[1073] & 0xff) == 0x22 && (buf[1074] & 0xff) == 0x00 && (buf[1075] & 0xff) == 0xa0)
		{
			printf("ResellerId: found (%s) reseller !\n", status.boxtype);
			free(buf);
			fclose(fd);
			return 0;
		}
	}
	else
	{
		// dummy fpr other boxes
		printf("ResellerId: check\n");
		printf("ResellerId1: %x\n", buf[1072]);
		printf("ResellerId2: %x\n", buf[1073]);
		printf("ResellerId3: %x\n", buf[1074]);
		printf("ResellerId4: %x\n", buf[1075]);
		printf("ResellerId: %x %x %x %x\n", buf[1072], buf[1073], buf[1074], buf[1075]);
		printf("ResellerId: not supported\n");
		printf("boxtype: %s\n", status.boxtype);
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
	
	if((checkbox("UFS910") == 1) || (checkbox("UFS922") == 1) || (checkbox("AT700") == 1) || (checkbox("AT7000") == 1))
	{
		dev = ostrcat(dev, "3", 1, 0);
		dir = ostrcat(dir, "var", 1, 0);		
	}
	else if((checkbox("ATEMIO500") == 1) || (checkbox("ATEMIO510") == 1) || (checkbox("ATEMIO7600") == 1) || (checkbox("IPBOX91") == 1) || (checkbox("IPBOX900") == 1) || (checkbox("IPBOX910") == 1) || (checkbox("IPBOX9000") == 1))
	{
		dev = ostrcat(dev, "4", 1, 0);
		dir = ostrcat(dir, "var", 1, 0);
	}
	else if((checkbox("SKYSAT") == 1) || (checkbox("AT7500") == 1))
	{
		dev = ostrcat(dev, "2", 1, 0);
		dir = ostrcat(dir, "boot", 1, 0);
	}
	else if(checkbox("UFS912") == 1)
	{
		dev = ostrcat(dev, "3", 1, 0);
		dir = ostrcat(dir, "boot", 1, 0);
	}
	else
	{
		dev = ostrcat(dev, "9", 1, 0);
		dir = ostrcat(dir, "var", 1, 0);
	}
						
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

	tmpstr = string_newline(command(cmd));
	free(cmd), cmd = NULL;

	if(tmpstr == NULL)
	{
		free(dir), dir = NULL;
		return 1;
	}

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
	debug(1000, "in");

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

	debug(1000, "out");

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
	free(ret); ret = NULL;
	free(authbuf);
	
	//Blacklist check
	if(status.security == 1)
	{
		char* blackfile = NULL;
		blackfile = gethttp("atemio.dyndns.tv", "/svn/auth/blacklist", 80, NULL, HTTPAUTH, NULL, 0);
	
		count = 0;
		if(blackfile != NULL)
			ret = strsplit(blackfile, "\n", &count);
	
		for(i = 0; i < count; i++)
		{
	    (&ret[i])->part = stringreplacecharonce((&ret[i])->part, ',', '\0');
			if(ret != NULL && ostrcmp(input, (&ret[i])->part) == 0)
			{
				status.security = 0;
				break;
			}
		}
		free(ret); ret = NULL;
		free(blackfile);
	}

	if(status.security == 1)
	{
		char* cmd = NULL;
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
		if(!file_exist("/dev/ttyS0") == 1)
			mknod("/dev/ttyS0", S_IFCHR | 0666, makedev(204, 40));

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
		
	buf = malloc(MINMALLOC);
	if(buf == NULL)
	{
		fclose(fd);
		return;
	}
	memset(buf, 0, MINMALLOC);
	
	fwrite(buf, MINMALLOC, 1, fd);

	free(buf);
	fclose(fd);
}

int getmaxsat(char* feshortname)
{
	char *tmpstr = NULL;
	int maxsat = 1;

	if(feshortname != NULL)
	{
		tmpstr = ostrcat(feshortname, "_maxsat", 0, 0);
		maxsat = getconfigint(tmpstr, NULL);
		free(tmpstr); tmpstr = NULL;
	}
	if(maxsat < 0) maxsat = 1;

	return maxsat;
}

void autochangechannelname()
{
	uint8_t lastsecnr = 0xff;
	int secnr = 0;
	unsigned char* buf = NULL;

	if(status.aktservice->fedev == NULL) return;

	while(secnr <= lastsecnr && secnr <= 256)
	{
		buf = dvbgetsdt(status.aktservice->fedev, secnr, 2000000);
		if(buf != NULL)
			findchannel(NULL, buf, &lastsecnr, NULL, NULL, 1);
		else
			break;
		free(buf); buf = NULL;
		secnr++;
	}
}

//check if emu ist installed, if not it hiddes the menu
int checkemu()
{
	char* tmpstr = NULL;

	tmpstr = command("emu.sh list");
	if(tmpstr != NULL)
		return 1;

	return 0;
}

int checkpluginskip(char* name)
{
	if(ostrcmp(name, "3 Wins") == 0) return 1;
	else if(ostrcmp(name, "CallMonitor") == 0) return 1;
	else if(ostrcmp(name, "Hello") == 0) return 1;
	else if(ostrcmp(name, "DLNA") == 0) return 1;
	else if(ostrcmp(name, "DVD Player") == 0) return 1;
	else if(ostrcmp(name, "hbbtv Browser") == 0) return 1;		
	else if(ostrcmp(name, "Imdb") == 0) return 1;
	else if(ostrcmp(name, "IP-Kammera") == 0) return 1;
	else if(ostrcmp(name, "Internet Browser") == 0) return 1;
	else if(ostrcmp(name, "KeyLock") == 0) return 1;
	else if(ostrcmp(name, "LCD Pearl") == 0) return 1;
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
			else if(ostrcmp("callmon_main", child->name) == 0) child->locked = flag;
			else if(ostrcmp("cinterface", child->name) == 0) child->locked = flag;
			else if(ostrcmp("dlna", child->name) == 0) child->locked = flag;
			else if(ostrcmp("dvdplayer", child->name) == 0) child->locked = flag;			
			else if(ostrcmp("extensionsmenu", child->name) == 0) child->locked = flag;
			else if(ostrcmp("eraseswap", child->name) == 0) child->locked = flag;
			else if(ostrcmp("hbbtv", child->name) == 0) child->locked = flag;
			else if(ostrcmp("imdb", child->name) == 0) child->locked = flag;
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

			if(status.expertmodus > 9 && status.security == 1)
				tmpflag = 0;
			else
				tmpflag = 1;

			// dont show this menus
			if(checkbox("ATEMIO510") == 1)
			{
				if(ostrcmp("vfdisplay", child->name) == 0) child->locked = tmpflag;
			}
			if(checkbox("UFS910") == 1)
			{
				if(ostrcmp("unlock", child->name) == 0) child->locked = 1;
			}

			if(ostrcmp("savesettings", child->name) == 0) child->locked = tmpflag;
			else if(ostrcmp("scartrecorder", child->name) == 0) child->locked = tmpflag;
			else if(ostrcmp("system_update_usb_online", child->name) == 0) child->locked = tmpflag;
			else if(ostrcmp("system_update_usb_tmp", child->name) == 0) child->locked = tmpflag;
			else if(ostrcmp("sambasettings", child->name) == 0) child->locked = tmpflag;
			else if(ostrcmp("nfssettings", child->name) == 0) child->locked = tmpflag;
			else if(ostrcmp("mediaplayer", child->name) == 0) child->locked = tmpflag;
			
			child = child->next;
		}
		node = node->next;
	}
}

int isbase64(char c)
{
	if((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9') || c == '+' || c == '/' || c == '=')
		return 1;
	return 0;
}

char decodebase64(char c)
{
	if(c >= 'A' && c <= 'Z') return(c - 'A');
	if(c >= 'a' && c <= 'z') return(c - 'a' + 26);
	if(c >= '0' && c <= '9') return(c - '0' + 52);
	if(c == '+') return(62);
	return 63;
}

int b64dec(char* dest, char* src)
{
	if(src && *src)
	{
		char* p = dest;
		int k, l = strlen(src) + 1;
		char* buf = NULL;

		buf = malloc(l);
		if(buf == NULL) return 0;

		for(k = 0, l = 0; src[k]; k++)
		{
			if(isbase64(src[k]))
				buf[l++] = src[k];
		}

		for(k = 0; k < l; k += 4)
		{
			char c1 = 'A', c2 = 'A', c3 = 'A', c4 = 'A';
			char b1 = 0, b2 = 0, b3 = 0, b4 = 0;

			c1 = buf[k];
			if(k + 1 < l)
				c2 = buf[k + 1];
			if(k + 2 < l)
				c3 = buf[k + 2];
			if(k + 3 < l)
				c4 = buf[k + 3];

			b1 = decodebase64(c1);
			b2 = decodebase64(c2);
			b3 = decodebase64(c3);
			b4 = decodebase64(c4);

			*p++ = ((b1 << 2) | (b2 >> 4));

			if(c3 != '=')
				*p++ = (((b2 & 0xf) << 4) | (b3 >> 2));
			if(c4 != '=')
				*p++ = (((b3 & 0x3) << 6) | b4);

		}

		free(buf);
		return(p - dest);
	}
	return 0;
}

void changechannellist(struct channel* chnode, char* channellist)
{
		char* tmpstr = NULL;

		if(chnode == NULL || status.aktservice->channel == NULL) return;

		status.servicetype = chnode->servicetype;
		if(status.servicetype == 0)
		{
			if(channellist != NULL)
			{
				tmpstr = ostrcat(channellist, NULL, 0, 0);
				free(status.aktservice->channellist);
				status.aktservice->channellist = tmpstr;
				addconfig("channellist", tmpstr);
			}
			tmpstr = oitoa(status.aktservice->channel->serviceid);
			addconfig("serviceid", tmpstr);
                	free(tmpstr); tmpstr = NULL;
			tmpstr = olutoa(status.aktservice->channel->transponderid);
			addconfig("transponderid", tmpstr);
                	free(tmpstr); tmpstr = NULL;
		}
		else
		{
			if(channellist != NULL)
			{
				tmpstr = ostrcat(channellist, NULL, 0, 0);
				free(status.aktservice->channellist);
				status.aktservice->channellist = tmpstr;
				addconfig("rchannellist", tmpstr);
			}
			tmpstr = oitoa(status.aktservice->channel->serviceid);
			addconfig("rserviceid", tmpstr);
                	free(tmpstr); tmpstr = NULL;
			tmpstr = olutoa(status.aktservice->channel->transponderid);
			addconfig("rtransponderid", tmpstr);
                	free(tmpstr); tmpstr = NULL;
		}
}

char* createpiconpath(struct channel* chnode, int flag)
{
	char* tmpstr = NULL, *tmpnr = NULL;

	if(chnode != NULL)
	{
		tmpstr = ostrcat(tmpstr, getconfig("piconpath", NULL), 1, 0);
		if(flag==1)
			tmpstr = ostrcat(tmpstr, "/alternate/", 1, 0);
		else
			tmpstr = ostrcat(tmpstr, "/", 1, 0);
		tmpnr = oitoa(chnode->serviceid);
		tmpstr = ostrcat(tmpstr, tmpnr, 1, 1);
		tmpnr = NULL;
		tmpstr = ostrcat(tmpstr, "-", 1, 0);
		tmpnr = olutoa(chnode->transponderid);
		tmpstr = ostrcat(tmpstr, tmpnr, 1, 1);
		tmpnr = NULL;
		tmpstr = ostrcat(tmpstr, ".png", 1, 0);
	}
	if(!isfile(tmpstr))
	{
		free(tmpstr); tmpstr = NULL;
		//tmpstr = ostrcat(tmpstr, getconfig("piconpath", NULL), 1, 0);
		//tmpstr = ostrcat(tmpstr, "/", 1, 0);
		//tmpstr = ostrcat(tmpstr, "default.png", 1, 0);
		tmpstr = ostrcat(tmpstr, getconfig("defskinpath", NULL), 1, 0);
		tmpstr = ostrcat(tmpstr, "/skin/", 1, 0);
		tmpstr = ostrcat(tmpstr, "defpicon.png", 1, 0);
	}

	return tmpstr;
}

int checkskin()
{
	int ret = 0;
	char* defskinpath = NULL;
	char* defskinconfig = NULL;
	char* defskinfile = NULL;

	if(file_exist(getconfig("skinpath", NULL)) == 0)
		ret = 1;
	else if(file_exist(getconfig("skinconfig", NULL)) == 0)
		ret = 2;
	else if(file_exist(getconfig("skinfile", NULL)) == 0)
		ret = 3;

	if(ret != 0)
	{
		err("skin not found code=%d, change to default", ret);

		defskinpath = getconfig("defskinpath", NULL);
		if(defskinpath != NULL) addconfig("skinpath", defskinpath);
		defskinconfig = getconfig("defskinconfig", NULL);
		if(defskinconfig != NULL) addconfig("skinconfig", defskinconfig);
		defskinfile = getconfig("defskinfile", NULL);
		if(defskinfile != NULL) addconfig("skinfile", defskinfile);

		ret = 0;
		if(file_exist(getconfig("skinpath", NULL)) == 0)
			ret = 1;
		else if(file_exist(getconfig("skinconfig", NULL)) == 0)
			ret = 2;
		else if(file_exist(getconfig("skinfile", NULL)) == 0)
			ret = 3;
	}

	if(ret != 0)
	{
		err("default skin not found code=%d", ret);
	}

	return ret;
}

int getaktvideosize()
{
	//wait 3 sek after zap, befor videsize is valid
	//driver takes a little time
	if(status.videosizevalid == 1)
		return 0;
	else if(status.videosizevalid + 3 > time(NULL))
		return 1;
	else
	{
#ifndef SIMULATE
		if(videoreadqwidth(status.aktservice->videodev) == 0)
#endif
			status.videosizevalid = 1;
	}

	return 1;
}

int ostrftime(char* buf, int count, char* format, struct tm* t)
{
	int ret = -1;

	if(format != NULL)
		ret = strftime(buf, count, format, t);
	else
		ret = strftime(buf, count, "%d-%m-%Y %H:%M", t);

	return ret;
}

//flag 0 = write all
//flag 1 = don't write epg
//flag 2 = only write epg
int writeallconfig(int flag)
{
	debug(1000, "in");
	char* tmpstr = NULL;
	int ret = 0;

	if(flag != 2)
	{
		if(status.writerectimer == 1)
			if(writerectimer(getconfig("rectimerfile", NULL), 0) != 0)
				ret = 1;
		if(status.writesat == 1)
			if(writesat(getconfig("satfile", NULL)) != 0)
				ret = 1;
		if(status.writeplaylist == 1)
			if(writeallplaylist() != 0)
				ret = 1;
		if(status.writemainplaylist == 1)
			if(writemainplaylist(getconfig("playlistfile", NULL)) != 0)
				ret = 1;
		if(status.writebouquet == 1)
			if(writeallbouquet() != 0)
				ret = 1;
		if(status.writemainbouquet == 1)
			if(writemainbouquet(getconfig("bouquetfile", NULL)) != 0)
				ret = 1;
		if(status.writechannel == 1)
			if(writechannel(getconfig("channelfile", NULL)) != 0)
				ret = 1;
		if(status.writetransponder == 1)
			if(writetransponder(getconfig("transponderfile", NULL)) != 0)
				ret = 1;
		if(status.writeprovider == 1)
			if(writeprovider(getconfig("providerfile", NULL)) != 0)
				ret = 1;
		if(status.writeownconfig == 1)
			if(writeownconfig(getconfig("ownconfig", NULL)) != 0)
				ret = 1;
		if(status.writeepgscanlist == 1)
			if(writeepgscanlist(getconfig("epgchannelfile", NULL)) != 0)
				ret = 1;
		if(status.writercconfig == 1)
			if(writercconfig(getconfig("rcconfig", NULL)) != 0)
				ret = 1;
		if(status.writeskinconfig == 1)
			if(writeskinconfig(getconfig("skinconfig", NULL)) != 0)
				ret = 1;
		if(status.writeconfig == 1)
		{
			//reset volume autochange before save
			if(status.volautochange > 0)
				addconfigint("vol", getvol() - status.volautochange);
			if(writeconfig(status.configfile) != 0)
				ret = 1;
			//restore volume autochange
			if(status.volautochange > 0)
				addconfigint("vol", getvol() + status.volautochange);
		}
		if(status.writemostzap == 1)
			if(writemostzap(getconfig("mostzapfile", NULL)) != 0)
				ret = 1;
	}
	if((flag == 0 || flag == 2) && time(NULL) > 1072224000) // 01.01.2004
	{
		tmpstr = createpath(getconfig("epg_path", NULL), "epg.dat");
		if(writeepg(tmpstr) != 0)
			ret = 1;
		free(tmpstr); tmpstr = NULL;
	}

	sync();

	debug(1000, "out");
	return ret;
}

void initmutex(int flag)
{
	if(flag == 1)
	{
		pthread_mutex_init(&status.drawingmutex, NULL);
		pthread_mutex_init(&status.rectimermutex, NULL);
		pthread_mutex_init(&status.servicemutex, NULL);
		pthread_mutex_init(&status.epgmutex, NULL);
		pthread_mutex_init(&status.vfdmutex, NULL);
		pthread_mutex_init(&status.channelmutex, NULL);
		pthread_mutex_init(&status.timerthreadmutex, NULL);
		pthread_mutex_init(&status.audiotrackmutex, NULL);
		pthread_mutex_init(&status.subtitlemutex, NULL);
		pthread_mutex_init(&status.dmxdevmutex, NULL);
		pthread_mutex_init(&status.rcmutex, NULL);
		pthread_mutex_init(&status.queuemutex, NULL);
		pthread_mutex_init(&status.clistmutex, NULL);
		pthread_mutex_init(&status.hddmutex, NULL);
		pthread_mutex_init(&status.linkedchannelmutex, NULL);
		pthread_mutex_init(&status.tsseekmutex, NULL);
		pthread_mutex_init(&status.accelfbmutex, NULL);
		pthread_mutex_init(&status.mediadbmutex, NULL);
	}
	else
	{
		pthread_mutex_destroy(&status.drawingmutex);
		pthread_mutex_destroy(&status.rectimermutex);
		pthread_mutex_destroy(&status.servicemutex);
		pthread_mutex_destroy(&status.epgmutex);
		pthread_mutex_destroy(&status.vfdmutex);
		pthread_mutex_destroy(&status.channelmutex);
		pthread_mutex_destroy(&status.timerthreadmutex);
		pthread_mutex_destroy(&status.audiotrackmutex);
		pthread_mutex_destroy(&status.subtitlemutex);
		pthread_mutex_destroy(&status.dmxdevmutex);
		pthread_mutex_destroy(&status.rcmutex);
		pthread_mutex_destroy(&status.queuemutex);
		pthread_mutex_destroy(&status.clistmutex);
		pthread_mutex_destroy(&status.hddmutex);
		pthread_mutex_destroy(&status.linkedchannelmutex);
		pthread_mutex_destroy(&status.tsseekmutex);
		pthread_mutex_destroy(&status.accelfbmutex);
		pthread_mutex_destroy(&status.mediadbmutex);
	}
}

int hexit(char c)
{
	if(c >= '0' && c <= '9')
		return c - '0';
	if(c >= 'a' && c <= 'f')
		return c - 'a' + 10;
	if(c >= 'A' && c <= 'F')
		return c - 'A' + 10;

	return 0;
}

char* htmlencode(char* from)
{
	int buflen = 0;
	char* buf = NULL, *to = NULL;

	buf = malloc(MINMALLOC);
	if(buf == NULL)
	{
		err("no mem");
		return NULL;
	}
	memset(buf, 0, MINMALLOC);
	to = buf;

	for(buflen = 0; *from != '\0' && buflen < MINMALLOC; ++from)
	{
		if(isalnum(*from) || strchr("/_.-~", *from) != NULL)
		{
			*to = *from;
			++to;
			++buflen;
		}
		else
		{
			sprintf(to, "%%%02x", (int)*from & 0xff);
			to += 3;
			buflen += 3;
		}
	}
	*to = '\0';

	buf = ostrcat(buf, NULL, 1, 0);
	return buf;
}

void htmldecode(char* to, char* from)
{
	if(to == NULL || from == NULL) return;

	for(; *from != '\0'; ++to, ++from)
	{
		if(from[0] == '%' && isxdigit(from[1]) && isxdigit(from[2]))
		{
			*to = hexit(from[1]) * 16 + hexit(from[2]);
			from += 2;
		}
		else
			*to = *from;
	}
	*to = '\0';
}

void htmldecode2(char* to, char* from)
{
	if(to == NULL || from == NULL) return;

	for(; *from != '\0'; ++to, ++from)
	{
		if(from[0] == '%' && isxdigit(from[1]) && isxdigit(from[2]) && isxdigit(from[3]) && isxdigit(from[4]))
		{
			*to = hexit(from[1]) * 16 + hexit(from[2]) * 16 + hexit(from[3]) * 16 + hexit(from[4]);
			from += 4;
		}
		else if(from[0] == '%' && isxdigit(from[1]) && isxdigit(from[2]) && isxdigit(from[3]))
		{
			*to = hexit(from[1]) * 16 + hexit(from[2]) * 16 + hexit(from[3]);
			from += 3;
		}
		else if(from[0] == '%' && isxdigit(from[1]) && isxdigit(from[2]))
		{
			*to = hexit(from[1]) * 16 + hexit(from[2]);
			from += 2;
		}
		else
			*to = *from;
	}
	*to = '\0';
}

void htmldecode3(char* to, char* from)
{
	if(to == NULL || from == NULL) return;

	for(; *from != '\0'; ++to, ++from)
	{
		if(from[0] == '\\' && from[1] == 'u' && isxdigit(from[2]) && isxdigit(from[3]) && isxdigit(from[4]) && isxdigit(from[5]) && isxdigit(from[6]) && isxdigit(from[7]))
		{
			*to = hexit(from[2]) * 1048576 + hexit(from[3]) * 65536 + hexit(from[4]) * 4096 + hexit(from[5]) * 256 + hexit(from[6]) * 16 + hexit(from[7]);
			from += 7;
		}
		if(from[0] == '\\' && from[1] == 'u' && isxdigit(from[2]) && isxdigit(from[3]) && isxdigit(from[4]) && isxdigit(from[5]))
		{
			*to = hexit(from[2]) * 4096 + hexit(from[3]) * 256 + hexit(from[4]) * 16 + hexit(from[5]);
			from += 5;
		}
		else
			*to = *from;
	}
	*to = '\0';
}

void setosdtransparent(int value)
{
	int ret = 255;
	if(value < 0) value = 0;
	if(value > 100) value = 100;

	ret -= value * 2.55;

	setfbtransparent(ret);
}

struct tm* olocaltime(time_t *value)
{
	struct tm *loctime, *loctime1 = NULL;

	loctime1 = (struct tm*)malloc(sizeof(struct tm));
	if(loctime1 == NULL)
	{
		err("no mem");
		return NULL;
	}
	loctime = localtime_r(value, loctime1);

	return loctime;
}

int setwakeuptimer(time_t waketime)
{
	debug(1000, "in");
	char* wakeuptimerprog = NULL;
	struct tm *loctime = NULL;
	char *buf = NULL;
	int ret = -1;

	wakeuptimerprog = getconfig("wakeuptimerprog", NULL);

	if(wakeuptimerprog == NULL)
	{
		debug(1000, "out -> NULL detect");
		return ret;
	}

	buf = malloc(MINMALLOC);
	if(buf == NULL)
	{
		err("no memory");
		return ret;
	}

	loctime = olocaltime(&waketime);

	if(loctime != NULL)
		ostrftime(buf, MINMALLOC, wakeuptimerprog, loctime);

	if(buf != NULL)
	{
		debug(100, "set wake up timer (%s)", buf);
		ret = system(buf);
	}

	free(loctime);
	free(buf);
	debug(1000, "out");
	return ret;
}

int autoresolution()
{
	debug(1000, "in");
	char *hotpluginfo = NULL;
	char *value = NULL;

	if(getconfig("av_videomode", NULL) != NULL || getconfig("av_colorformat", NULL))
		return 1;

	hotpluginfo = getconfig("hotpluginfo", NULL);

	if(hotpluginfo == NULL)
	{
		debug(1000, "out -> NULL detect");
		return 1;
	}

	value = readsys(hotpluginfo, 1);
	if(value == NULL)
	{
		debug(1000, "out -> NULL detect");
		return 1;
	}

	if(value[0] == 'y')
	{
		setvideomode("720p50", 0);
		changefbresolution("720p50");
		setcolorformat("hdmi_rgb");
	}
	else
	{
		setvideomode("576i50", 0);
		changefbresolution("576i50");
		setcolorformat("rgb");
	}

	free(value);
	debug(1000, "out");
	return 0;
}

//TODO: can remove if new function work
/*
int writelengthfield(unsigned char *buf, unsigned int len)
{
	if(len < 127)
	{
		*buf++ = len;
		return 1;
	}
	else if(len < 256)
	{
		*buf++ = 0x81;
		*buf++ = len;
		return 2;
	}
	else if(len < 65535)
	{
		*buf++ = 0x82;
		*buf++ = len >> 8;
		*buf++ = len;
		return 3;
	}

	return 0;
}
*/

size_t writelengthfield(unsigned char * buf, unsigned int len)
{
	if(buf == NULL) return 0;

	if(len < 128)
	{
		buf[0] = len;
		return 1;
	}
	else
	{
		unsigned int pos = 0;
		unsigned int shiftby = 8;
		unsigned char lenfieldsize = 1;

		while((len >> shiftby) != 0)
		{
			lenfieldsize++;
			shiftby += 8;
		}

		buf[pos++] = ((1 << 7) | lenfieldsize);

		while(shiftby != 0)
		{
			shiftby -= 8;
			buf[pos++] = len >> shiftby;
		}
		return pos;
	}
}


//flag 0: del non alpha/num
//flag 1: change all /
void delspezchar(char* text, int flag)
{
	char* tmpstr = text;

	while(*tmpstr != '\0')
	{
		if(flag == 1)
		{
			if(tmpstr[0] == '/')
				tmpstr[0] = '-';
		}
		else
		{
			if(!isalnum(tmpstr[0]) && tmpstr[0] != '-' && tmpstr[0] != '.')
				tmpstr[0] = ' ';
		}
		tmpstr++;
	}
}

void m_lock(pthread_mutex_t *mutex, int flag)
{
	switch(flag)
	{
		case 0: debug(900, "drawingmutex lock"); break;
		case 1: debug(900, "rectimermutex lock"); break;
		case 2: debug(900, "servicemutex lock"); break;
		case 3: debug(900, "vfdmutex lock"); break;
		case 4: debug(900, "epgmutex lock"); break;
		case 5: debug(900, "channelmutex lock"); break;
		case 6: debug(900, "timerthreadmutex lock"); break;
		case 7: debug(900, "audiotrackmutex lock"); break;
		case 8: debug(900, "subtitlemutex lock"); break;
		case 9: debug(900, "dmxdevmutex lock"); break;
		case 10: debug(900, "rcmutex lock"); break;
		case 11: debug(900, "queuemutex lock"); break;
		case 12: debug(900, "clistmutex lock"); break;
		case 13: debug(900, "hddmutex lock"); break;
		case 14: debug(900, "linkedchannelmutex lock"); break;
		case 15: debug(900, "tsseekmutex lock"); break;
		case 16: debug(900, "accelfbmutex lock"); break;
		case 17: debug(900, "mediadbmutex lock"); break;
		default: debug(900, "unknown mutex lock"); break;
	}
	pthread_mutex_lock(mutex);
}

void m_unlock(pthread_mutex_t *mutex, int flag)
{
	switch(flag)
	{
		case 0: debug(900, "drawingmutex unlock"); break;
		case 1: debug(900, "rectimermutex unlock"); break;
		case 2: debug(900, "servicemutex unlock"); break;
		case 3: debug(900, "vfdmutex unlock"); break;
		case 4: debug(900, "epgmutex unlock"); break;
		case 5: debug(900, "channelmutex unlock"); break;
		case 6: debug(900, "timerthreadmutex unlock"); break;
		case 7: debug(900, "audiotrackmutex unlock"); break;
		case 8: debug(900, "subtitlemutex unlock"); break;
		case 9: debug(900, "dmxdevmutex unlock"); break;
		case 10: debug(900, "rcmutex unlock"); break;
		case 11: debug(900, "queuemutex unlock"); break;
		case 12: debug(900, "clistmutex unlock"); break;
		case 13: debug(900, "hddmutex unlock"); break;
		case 14: debug(900, "linkedchannelmutex unlock"); break;
		case 15: debug(900, "tsseekmutex unlock"); break;
		case 16: debug(900, "accelfbmutex unlock"); break;
		case 17: debug(900, "mediadbmutex unlock"); break;
		default: debug(900, "unknown mutex unlock"); break;
	}
	pthread_mutex_unlock(mutex);
}

void debugstack(void* address, void* address1)
{
	Dl_info info, info1;
	void* trace[10];
	size_t size;
	size = backtrace(trace, 10);

	FILE* fd = NULL;
	void* akttrace[2];
	int i;
	char **strings;
	char **aktstring;
	time_t rawtime;

	char* boxversion = NULL;
	char* imgversion = NULL;
	struct stimerthread* tnode = NULL;

	if(isfile("/etc/model")	!= 0)
		boxversion = readsys("/etc/model", 1);
	else
		boxversion = ostrcat("unknown", NULL, 0, 0);

	if(isfile(getconfig("imagenamefile", NULL))	!= 0)
		imgversion = readsys(getconfig("imagenamefile", NULL), 1);
	else
		imgversion = ostrcat("unknown", NULL, 0, 0);

	tnode = gettimerbythread(pthread_self());
	if(tnode != NULL)
		dladdr(tnode->func, &info);

	dladdr(address, &info1);

	strings = backtrace_symbols(trace, size);
	akttrace[0] = (void*)address1;
	akttrace[1] = (void*)address;
	aktstring = backtrace_symbols(akttrace, 2); //get fault funktion name

	printf("--------------------------------------\n");
	printf("Box: %s\n", boxversion);
	printf("Image: %s\n", imgversion);
	if(info1.dli_fname != NULL)
		printf("Error in File: %s\n", info1.dli_fname);
	printf("MainThread: %x\n", status.mainthread);
	if(tnode != NULL && info.dli_sname != NULL)
		printf("Error in Thread: %x (%s)\n", pthread_self(), info.dli_sname);
	else
		printf("Error in Thread: %x\n", pthread_self());
	printf("Obtaining %zd stack frames:\n\n", size);

	for(i = 0; i < size; i++)
		printf("%s\n", strings[i]);

	printf("\nLast functions:\n\n");
	printf("%s\n", aktstring[0]);
	printf("%s\n", aktstring[1]);

	printf("--------------------------------------\n");

	fd = fopen(getconfig("tracelog", NULL), "a");
	if(fd != NULL)
	{
		time(&rawtime);
		fprintf(fd, "Date: %s", ctime(&rawtime));
		fprintf(fd, "Box: %s\n", boxversion);
		fprintf(fd, "Image: %s\n", imgversion);
		if(info1.dli_fname != NULL)
			printf("Error in File: %s\n", info1.dli_fname);
		fprintf(fd, "MainThread: %x\n", status.mainthread);
		if(tnode != NULL && info.dli_sname != NULL)
			fprintf(fd, "Error in Thread: %x (%s)\n", pthread_self(), info.dli_sname);
		else
			fprintf(fd, "Error in Thread: %x\n", pthread_self());
		fprintf(fd, "Obtaining %zd stack frames:\n\n", size);
		for(i = 1; i < size; i++)
			fprintf(fd, "%s\n", strings[i]);
		fprintf(fd, "\nLast functions:\n\n");
		fprintf(fd, "%s\n", aktstring[0]);
		fprintf(fd, "%s\n", aktstring[1]);
		fprintf(fd, "--------------------------------------\n\n");
		fclose(fd);
		sync();
	}
	else
		perr("open %s", getconfig("tracelog", NULL));

	free(imgversion);
	free(boxversion);
	free(strings);
	free(aktstring);
}

int mountauto(const char *file, const char *dir, unsigned long int flag, const void *data)
{
	int ret = -1;
	char* saveptr = NULL, *filesystems = NULL, *tmpfilesystems = NULL, *token = NULL;

	filesystems = getconfig("filesystems", NULL);
	tmpfilesystems = ostrcat(tmpfilesystems, filesystems, 0, 0);

	token = strtok_r(tmpfilesystems, ";", &saveptr);
	while(token != NULL)
	{
		ret = mount(file, dir, token, flag, data);
		if(ret == 0) break;
		token = strtok_r(NULL, ";", &saveptr);
	}

	free(tmpfilesystems);
	return ret;
}

int setwaswakuptimer(int value)
{
	debug(1000, "in");
	char *waswakeuptimerdev = NULL, *tmpstr = NULL;
	int ret = 0;

	waswakeuptimerdev = getconfig("waswakeuptimerdev", NULL);

	if(waswakeuptimerdev != NULL)
	{
		debug(100, "set %s to %d", waswakeuptimerdev, value);
		tmpstr = oitoa(value);
		ret = writesys(waswakeuptimerdev, tmpstr, 0);
		free(tmpstr); tmpstr = NULL;
		return ret;
	}

	debug(1000, "out");
	return 0;
}

int getwaswakuptimer()
{
	debug(1000, "in");
	int ret = 0;
	char *waswakeuptimerdev = NULL;
	char *value = NULL;

	waswakeuptimerdev = getconfig("waswakeuptimerdev", NULL);

	if(waswakeuptimerdev == NULL)
	{
		debug(1000, "out -> NULL detect");
		return 0;
	}

	value = readsys(waswakeuptimerdev, 1);
	if(value == NULL)
	{
		debug(1000, "out -> NULL detect");
		return 0;
	}

	// start from timer
	if(atoi(value) == 1) ret = 1;

	free(value);
	debug(1000, "out");
	return ret;
}

void checkboxstartthread(struct stimerthread* self)
{
	struct rectimer* node = rectimer;
	int timediff = getconfigint("rectimer_timediff", NULL);
	
	if(self == NULL || node == NULL) return; //no record

	debug(400, "boxstart rectimer thread start");

	//wait for right time
	while(self->aktion != STOP && time(NULL) < 1072224000) // 01.01.2004
		sleep(5);

	while(node != NULL)
	{
		if(node->status < 2)
		{
			time_t akttime = time(NULL);
			time_t begin = node->begin - getconfigint("wakeuptimerdevdiff", NULL);
			if(begin > akttime - timediff && begin < akttime + timediff)
			{
				debug(400, "found rectimer who has start the box");
				setwaswakuptimer(1);
			}
		}
		node = node->next;
	}

	debug(400, "boxstart rectimer thread end");
}

int setwakeuptimerdev(time_t value)
{
	debug(1000, "in");
	char* wakeuptimerdev, *tmpstr = NULL;
	int ret = 0;
	int diff = getconfigint("wakeuptimerdevdiff", NULL);

	if(value != 0x7FFFFFFF && value - diff > time(NULL))
		value -= diff;
	
	wakeuptimerdev = getconfig("wakeuptimerdev", NULL);

	if(wakeuptimerdev != NULL && value >= time(NULL))
	{
		debug(100, "set %s to %ld", wakeuptimerdev, value);
		tmpstr = olutoa(value);
		ret = writesys(wakeuptimerdev, tmpstr, 0);
		free(tmpstr); tmpstr = NULL;
		return ret;
	}

	debug(1000, "out");
	return 0;
}

int changepolicy()
{
	debug(1000, "in");
	char *tmppolicy = NULL, *tmpstr = NULL;

 	tmppolicy = getpolicy();

	if(!ostrncmp("letterbox", tmppolicy, 8))
		tmpstr = ostrcat(tmpstr, "panscan", 1, 0);
	else if(!ostrncmp("panscan", tmppolicy, 7))
		tmpstr = ostrcat(tmpstr, "non", 1, 0);
	else if(!ostrncmp("non", tmppolicy, 3))
		tmpstr = ostrcat(tmpstr, "bestfit", 1, 0);
	else if(!ostrncmp("bestfit", tmppolicy, 7))
		tmpstr = ostrcat(tmpstr, "letterbox", 1, 0);

	setpolicy(tmpstr);

	free(tmpstr); tmpstr = NULL;
	free(tmppolicy); tmppolicy = NULL;
	debug(1000, "out");
	return 0;
}

char* getdefaultgw()
{
	char* name = NULL;
	unsigned long def, gw, m;
	int flags, ref, use, metric, mtu, win, ir;
	struct in_addr ip;

	FILE *fd = fopen("/proc/net/route", "r");
	if(fd == NULL)
		return NULL;

	if(fscanf(fd, "%*[^\n]\n") < 0)
	{
		fclose(fd);
		return NULL;
	}

	name = malloc(64);
	if(name == NULL)
	{
		err("no mem");
		return NULL;
	}

	while(1)
	{
		int r = fscanf(fd, "%63s%lx%lx%x%d%d%d%lx%d%d%d\n", name, &def, &gw, &flags, &ref, &use, &metric, &m, &mtu, &win, &ir);
		if(def == 0) break;
		if(r != 11)
		{
			fclose(fd);
			free(name);
			return NULL;
		}
		if(!(flags & 0x0001)) continue; //is interface down
	}

	fclose(fd);
	free(name);

	ip.s_addr = gw;
	if(def == 0)
		return inet_ntoa(ip);

	return NULL;
}

char* changefilenameext(char* filename, char* ext)
{
	char* newfilename = NULL;
	char* zeichen = NULL;

	if(filename == NULL) return NULL;

	newfilename = ostrcat(filename, NULL, 0, 0);

	zeichen = strrchr(newfilename, '.');
	if(zeichen != NULL)
	{
		zeichen[0] = '\0';
		newfilename = ostrcat(newfilename, ext, 1, 0);
	}

	return newfilename;
}

int cmpfilenameext(char* filename, char* ext)
{
	char* zeichen = NULL;

	if(filename == NULL) return 1;

	zeichen = strrchr(filename, '.');
	if(zeichen != NULL)
	{
		if(ostrcasecmp(zeichen, ext) == 0) return 0;
	}

	return 1;
}

char* getfilenameext(char* filename)
{
	char* zeichen = NULL;

	if(filename == NULL) return NULL;

	zeichen = strrchr(filename, '.');
	if(zeichen != NULL)
	{
		return ostrcat(zeichen + 1, NULL, 0, 0);
	}

	return NULL;
}

char* getcurrentdir(char* path)
{
	char* zeichen = NULL;

	if(path == NULL) return NULL;

	zeichen = strrchr(path, '/');
	if(zeichen != NULL)
	{
		return ostrcat(zeichen + 1, NULL, 0, 0);
	}

	return NULL;
}

char* convert_timesec(int sec)
{
	int hour = 0, min = 0, seconds = 0;
	char* buf = NULL;

	buf = malloc(9);
	if(buf == NULL)
	{
		err("no mem");
		return NULL;
	}

	seconds = sec % 60;
	min = (sec / 60) % 60;
	hour = sec / 3600;

	if(seconds < 0) seconds = 0;
	if(min < 0) min = 0;
	if(hour < 0 || hour > 23)
	{
		hour = 0;
		min = 0;
		seconds = 0;
	}

	snprintf(buf, 9, "%02d:%02d:%02d", hour, min, seconds);

	return buf;
}

int checkdate()
{
	time_t dvbtime = 0;

	if(time(NULL) < 1072224000 || status.timeupdatecount > 3600) // 01.01.2004
	{
		if(dvbgetdate(&dvbtime, 10000000) == 0) //10 sek
		{
			setsystime(&dvbtime);
			status.timeupdatecount = 0;
			return 0;
		}
		return 1;
	}
	return 0;
}

void closeonexec(int fd)
{
	if(fd > -1)
		fcntl(fd, F_SETFD, fcntl(fd, F_GETFD) | FD_CLOEXEC);
}

void startthumb(flag)
{
	if(flag == 1)
	{
		if(getconfigint("createthumb", NULL) == 1 && status.thumbthread == NULL)
			status.thumbthread = addtimer(&thumbthread, START, 1000, 1, NULL, NULL, NULL);
	}
	else if(status.thumbthread != NULL)
	{
		status.thumbthread->aktion = STOP;
		status.thumbthread = NULL;
	}
}

void starthttpd(flag)
{
	if(flag == 1)
	{
		if(getconfigint("httpdstart", NULL) == 1 && status.httpthread == NULL)
		{
			status.httpthread = addtimer(&httpdthreadfunc, START, 10000, -1, NULL, NULL, NULL);
			if(status.httpthread != NULL)
				status.httpthread->flag = setbit(status.httpthread->flag, 0);
		}
	}
	else if(status.httpthread != NULL)
	{
		status.httpthread->aktion = STOP;
		status.httpthread = NULL;
	}
}

void startrguid(flag)
{
	if(flag == 1)
	{
		if(getconfigint("rguidstart", NULL) == 1 && status.rguithread == NULL)
		{
			status.rguithread = addtimer(&rguidthreadfunc, START, 10000, -1, NULL, NULL, NULL);
			if(status.rguithread != NULL)
				status.rguithread->flag = setbit(status.rguithread->flag, 0);
		}
	}
	else if(status.rguithread != NULL)
	{
		status.rguithread->aktion = STOP;
		status.rguithread = NULL;
	}
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

int setoverclockfreq(int mode)
{
	// mode 0 = Set Standby Freq
	// mode 1 = Set Freq

	if(ostrcmp(getownconfig("standby_overclock"), "0") == 0)
		return 1;

	if(isfile("/proc/cpu_frequ/pll0_ndiv_mdiv") == 0)
		return 1;

	char* tmpstr = NULL;

	if(mode == 0)
		tmpstr = getownconfig("standby_freq");
	else
		tmpstr = getownconfig("pll0_ndiv_mdiv");

	return writesys("/proc/cpu_frequ/pll0_ndiv_mdiv", tmpstr, 1);
}

int setsaturation(int value)
{
	debug(1000, "in");
	char* saturationdev;

	saturationdev = getconfig("saturationdev", NULL);

	if(saturationdev != NULL)
	{
		debug(100, "set %s to %d", saturationdev, value);
		return writesysint(saturationdev, value, 1);
	}

	debug(1000, "out");
	return 0;
}

int setbrightness(int value)
{
	debug(1000, "in");
	char* brightnessdev;

	brightnessdev = getconfig("brightnessdev", NULL);

	if(brightnessdev != NULL)
	{
		debug(100, "set %s to %d", brightnessdev, value);
		return writesysint(brightnessdev, value, 1);
	}

	debug(1000, "out");
	return 0;
}

int setvmpeg(struct dvbdev* node, int posx, int posy, int width, int height)
{
	debug(1000, "in");
	char* vmpegdev = NULL, *tmpstr = NULL, *buf = NULL;
	int ret = 0;

	if(node == NULL) return 1;
	vmpegdev = getconfig("vmpegalldev", NULL);

	if(vmpegdev != NULL)
	{
		buf = malloc(MINMALLOC);
		if(buf == NULL)
		{
			err("no mem");
			return 1;
		}
		
		tmpstr = malloc(MINMALLOC);
		if(tmpstr == NULL)
		{
			err("no mem");
			free(buf);
			return 1;
		}
		
		snprintf(buf, MINMALLOC, vmpegdev, node->devnr);
		snprintf(tmpstr, MINMALLOC, "%x %x %x %x", posx, posy, width, height);
		debug(100, "set %s to %s", buf, tmpstr);

		status.tvpic = 1;
		/*
		//workaround for driver crash
		if(status.tvpic > 0)
		{
			char* tmpstr1 = NULL;

			tmpstr1 = getaspect();
			if(ostrcmp(tmpstr1, "4:3") == 0)
				status.tvpic = 2;
			else
				status.tvpic = 1;
			free(tmpstr1); tmpstr1 = NULL;

			tmpstr1 = getvideomode();
			if(ostrcmp("pal", tmpstr1) == 0 || ostrncmp("576", tmpstr1, 3) == 0)
				setaspect("4:3");
			else
				setaspect("16:9");
			free(tmpstr1); tmpstr1 = NULL;
		}
		*/

		ret = writesys(buf, tmpstr, 1);

		//reset
		if(posx == 0 && posy == 0 && width == 0 && height == 0)
		{
			/*
			//workaround for driver crash
			if(status.tvpic == 1)
				setaspect("16:9");
			else
				setaspect("4:3");
			*/
			status.tvpic = 0;
		}
		
		free(tmpstr);
		free(buf);
		return ret;
	}

	debug(1000, "out");
	return 0;
}

//flag 0: wh = width
//flag 1: wh = height
int setvmpegrect(struct dvbdev* node, int posx, int posy, int wh, int flag)
{
	int ret = 0;

	if(flag == 0)
		ret = setvmpeg(node, posx, posy, wh, wh / 1.2);

	if(flag == 1)
		ret = setvmpeg(node, posx, posy, wh * 1.2, wh);

	return ret;
}

int resettvpic()
{
	int ret = 0;

	if(status.tvpic > 0 && status.aktservice != NULL)
		ret = setvmpeg(status.aktservice->videodev, 0, 0, 0, 0);

	return ret;
}

int setcontrast(int value)
{
	debug(1000, "in");
	char* contrastdev;

	contrastdev = getconfig("contrastdev", NULL);

	if(contrastdev != NULL)
	{
		debug(100, "set %s to %d", contrastdev, value);
		return writesysint(contrastdev, value, 1);
	}

	debug(1000, "out");
	return 0;
}

int settint(int value)
{
	debug(1000, "in");
	char* tintdev;

	tintdev = getconfig("tintdev", NULL);

	if(tintdev != NULL)
	{
		debug(100, "set %s to %d", tintdev, value);
		return writesysint(tintdev, value, 1);
	}

	debug(1000, "out");
	return 0;
}

int ozip(char* inbuf, int inlen, char** outbuf, int* outlen, int level)
{
	int ret = 0;
	z_stream stream;

	stream.zalloc = Z_NULL;
	stream.zfree = Z_NULL;
	stream.opaque = Z_NULL;

	ret = deflateInit(&stream, level);
	if(ret != Z_OK)
		return 1;

	*outbuf = malloc(inlen);
	if(*outbuf == NULL)
	{
		err("no mem");
		(void)deflateEnd(&stream);
		return 1;
	}

	stream.avail_in = inlen;
	stream.next_in = (void*)inbuf;

	do
	{
		stream.avail_out = inlen;
		stream.next_out = (void*)*outbuf;

		ret = deflate(&stream, Z_FINISH);
		if(ret == Z_STREAM_ERROR)
		{
			free(*outbuf); *outbuf = NULL;
			(void)deflateEnd(&stream);
			return 1;
		}

	}
	while(stream.avail_out == 0);
	*outlen = inlen - stream.avail_out;
	*outbuf = realloc(*outbuf, *outlen);

	(void)deflateEnd(&stream);
	return 0;
}

//flag 0: malloc mem
//flag 1: don't malloc mem
int ounzip(char* inbuf, int inlen, char** outbuf, int* outlen, int maxbuf, int flag)
{
	int ret = 0;
	z_stream stream;

	stream.zalloc = Z_NULL;
	stream.zfree = Z_NULL;
	stream.opaque = Z_NULL;
	stream.avail_in = 0;
	stream.next_in = Z_NULL;

	ret = inflateInit(&stream);
	if(ret != Z_OK)
		return 1;

	if(flag == 0)
	{
		*outbuf = malloc(maxbuf);
		if(*outbuf == NULL)
		{
			err("no mem");
			(void)inflateEnd(&stream);
			return 1;
		}
	}

	stream.avail_in = inlen;
	stream.next_in = (void*)inbuf;

	do
	{
		stream.avail_out = maxbuf;
		stream.next_out = (void*)*outbuf;

		ret = inflate(&stream, Z_NO_FLUSH);
		if(ret == Z_NEED_DICT || ret == Z_DATA_ERROR || ret == Z_MEM_ERROR)
		{
			free(*outbuf); *outbuf = NULL;
			(void)inflateEnd(&stream);
			return 1;
		}

	}
	while(stream.avail_out == 0);
	*outlen = maxbuf - stream.avail_out;
	if(flag == 0) *outbuf = realloc(*outbuf, *outlen);

	(void)inflateEnd(&stream);
	return 0;
}

void endianswapshort(unsigned short* value)
{
	*value = (*value >> 8) | (*value << 8);
}

int rcnumber(int rcret)
{
	if(rcret == getrcconfigint("rc0", NULL) ||
	rcret == getrcconfigint("rc1", NULL) ||
	rcret == getrcconfigint("rc2", NULL) ||
	rcret == getrcconfigint("rc3", NULL) ||
	rcret == getrcconfigint("rc4", NULL) ||
	rcret == getrcconfigint("rc5", NULL) ||
	rcret == getrcconfigint("rc6", NULL) ||
	rcret == getrcconfigint("rc7", NULL) ||
	rcret == getrcconfigint("rc8", NULL) ||
	rcret == getrcconfigint("rc9", NULL))
		return rcret;
	else
		return -9999;
}

int delallfiles(char* dir, char* ext)
{
	struct dirent *dirent = NULL;
	DIR *pdir = NULL;
	char* tmpstr = NULL;

	pdir = opendir(dir);
	if(pdir != NULL)
	{
		while((dirent = readdir(pdir)) != NULL)
		{
			if(ostrcmp(".", dirent->d_name) != 0 && ostrcmp("..", dirent->d_name) != 0)
			{
				if(ext == NULL || ostrstr(dirent->d_name, ext) != NULL)
				{
					tmpstr = ostrcat(dir, "/", 0, 0);
					tmpstr = ostrcat(tmpstr, dirent->d_name, 1, 0);
					unlink(tmpstr);
					free(tmpstr); tmpstr = NULL;
				}
			}
		}

		closedir(pdir);
	}

	return 0;
}

unsigned long getfilecount(char* dir)
{
	unsigned long count = 0;
	struct dirent *dirent = NULL;
	DIR *pdir = NULL;

	pdir = opendir(dir);
	if(pdir != NULL)
	{
		while((dirent = readdir(pdir)) != NULL)
			count++;

		closedir(pdir);
	}

	return count;
}

unsigned long long getfullspace(char* dir)
{
	struct statfs64 s;
	unsigned long long fullsize = 0;

	if(statfs64(dir, &s) >= 0)
	{
		fullsize = s.f_blocks;
		fullsize *= s.f_bsize;
	}

	return fullsize;
}

unsigned long long getfreespace(char* dir)
{
	struct statfs64 s;
	unsigned long long freesize = 0;

	if(statfs64(dir, &s) >= 0)
	{
		freesize = s.f_bfree;
		freesize *= s.f_bsize;
	}

	return freesize;
}

int checkbit(int value, int bitpos)
{
	int ret = 0;

	ret = value & (1 << bitpos) ? 1: 0;
	return ret;
}

int tooglebit(int value, int bitpos)
{
	value ^= 1 << bitpos;
	return value;
}

int clearbit(int value, int bitpos)
{
	value &= ~(1 << bitpos);
	return value;
}

int setbit(int value, int bitpos)
{
	value |= 1 << bitpos;
	return value;
}

int setsystime(time_t* newtime)
{
	if (stime(newtime))
	{
		err("can't set system time");
		return 1;
	}
	return 0;
}

off64_t getfilesize(char* name)
{
	struct stat64 sbuf;

	if(lstat64(name, &sbuf) == -1)
		return 0;

	return sbuf.st_size;
}

time_t getfiletime(char* name, int type)
{
	struct stat64 sbuf;

	if(lstat64(name, &sbuf) == -1)
		return 0;

	switch(type)
	{
		case 1: return sbuf.st_mtime;
		case 2: return sbuf.st_ctime;
		default: return sbuf.st_atime;
	}
}

int isfile(char* name)
{
	struct stat64 sbuf;

	if(lstat64(name, &sbuf) == -1)
		return 0;

	if(S_ISREG(sbuf.st_mode))
		return 1;

	return 0;
}

int isdir(char* name)
{
	struct stat64 sbuf;
	char *rpath = NULL;

	if(lstat64(name, &sbuf) == -1)
		return 0;

	if(S_ISDIR(sbuf.st_mode))
		return 1;

	if(S_ISLNK(sbuf.st_mode))
	{
		rpath = realpath(name, NULL);
		if(lstat64(rpath, &sbuf) == -1)
		{
			free(rpath);
			return 0;
		}
		free(rpath);
		if(S_ISDIR(sbuf.st_mode))
			return 1;
        }

	return 0;
}

char* getmaxsatstring(int maxsat)
{
	char* tmpstr = NULL, *tmpnr = NULL;
	int i;

	tmpstr = ostrcat(tmpstr, "1", 1, 0);
	for(i = 2; i <= maxsat; i++)
	{
		tmpnr = oitoa(i);
		tmpstr = ostrcat(tmpstr, "\n", 1, 0);
		tmpstr = ostrcat(tmpstr, tmpnr, 1, 1);
	}
	return tmpstr;
}

void ostrcatbig(char** value1, char* value2, int* maxlen, int* pos)
{
//	debug(1000, "in");
	int len = 0;

	if(value2 == NULL)
	{
		debug(1000, "out -> NULL detect");
		return;
	}
	len = strlen(value2);

	if(len > MINMALLOC)
	{
		err("string to long");
		return;
	}

	if(*value1 != NULL && maxlen == 0)
		*maxlen = strlen(*value1);

	if(*value1 == NULL || *pos + len + 1 > *maxlen)
	{
		*maxlen = *maxlen + (MINMALLOC * 10);
		*value1 = realloc(*value1, *maxlen);
		if(*value1 == NULL)
		{
			err("no memory");
			return;
		}
	}

	memcpy(*value1 + *pos, value2, len + 1);
	*pos = *pos + len;

	//debug(1000, "out");
}

char* ostrcat(char* value1, char* value2, int free1, int free2)
{
//	debug(1000, "in");
	int len = 0, len1 = 0, len2 = 0;
	char* buf = NULL;

	if(value1 == NULL && value2 == NULL) return NULL;

	if(value1 != NULL) len1 = strlen(value1);
	if(value2 != NULL) len2 = strlen(value2);

	len = len1 + len2 + 1;

	buf = malloc(len);
	if(buf == NULL)
	{
		err("no memory");
		return NULL;
	}

	if(len1 > 0) memcpy(buf, value1, len1);
	if(len2 > 0) memcpy(buf + len1, value2, len2);
	buf[len - 1] = '\0';

	if(free1 == 1) free(value1);
	if(free2 == 1) free(value2);

	//helpfull for memleak detect
	//if(buf != NULL && strlen(buf) == 0x0b - 0x01)
	//	printf("******** memleak string (%s) (%p) ********\n", buf, buf);

	//debug(1000, "out");
	return buf;
}

char* olutoa(unsigned long value)
{
	debug(1000, "in");
	char *buf = NULL, *buf1 = NULL;

	buf = malloc(MINMALLOC);
	if(buf == NULL)
	{
		err("no memory");
		return NULL;
	}

	sprintf(buf, "%lu", value);
	buf1 = ostrcat(buf, NULL, 1, 0);

	debug(1000, "out");
	return buf1;
}

char* oitoax(int value)
{
	debug(1000, "in");
	char *buf = NULL, *buf1 = NULL;

	buf = malloc(MINMALLOC);
	if(buf == NULL)
	{
		err("no memory");
		return NULL;
	}

	sprintf(buf, "%x", value);
	buf1 = ostrcat(buf, NULL, 1, 0);

	debug(1000, "out");
	return buf1;
}

char* oitoa(int value)
{
	debug(1000, "in");
	char *buf = NULL, *buf1 = NULL;

	buf = malloc(MINMALLOC);
	if(buf == NULL)
	{
		err("no memory");
		return NULL;
	}

	sprintf(buf, "%d", value);
	buf1 = ostrcat(buf, NULL, 1, 0);

	debug(1000, "out");
	return buf1;
}

char* oitoa64(off64_t value)
{
	debug(1000, "in");
	char *buf = NULL, *buf1 = NULL;

	buf = malloc(MINMALLOC);
	if(buf == NULL)
	{
		err("no memory");
		return NULL;
	}

	sprintf(buf, "%lld", value);
	buf1 = ostrcat(buf, NULL, 1, 0);

	debug(1000, "out");
	return buf1;
}

char* oftoa64(double value, char* count)
{
	debug(1000, "in");
	char *buf = NULL, *buf1 = NULL;
	char* tmpstr = NULL;

	buf = malloc(MINMALLOC);
	if(buf == NULL)
	{
		err("no memory");
		return NULL;
	}

	tmpstr = ostrcat("%.", count, 0, 0);
	tmpstr = ostrcat(tmpstr, "f", 1, 0);

	sprintf(buf, tmpstr, value);
	buf1 = ostrcat(buf, NULL, 1, 0);

	free(tmpstr);
	debug(1000, "out");
	return buf1;
}

int ostrncmp(char* value1, char* value2, int count)
{
	//debug(1000, "in");
	int ret = 1;

	if(value1 != NULL && value2 != NULL)
		ret = strncmp(value1, value2, count);

	//debug(1000, "out");
	return ret;
}

int ostrcmp(char* value1, char* value2)
{
//	debug(1000, "in");
	int ret = 1;

	if(value1 != NULL && value2 != NULL)
		ret = strcmp(value1, value2);

//	debug(1000, "out");
	return ret;
}

int ostrcasecmp(char* value1, char* value2)
{
//	debug(1000, "in");
	int ret = 1;

	if(value1 != NULL && value2 != NULL)
		ret = strcasecmp(value1, value2);

//	debug(1000, "out");
	return ret;
}

char* createpath(char* dir, char* file)
{
	debug(1000, "in");
	char *absdir = NULL;
	char *pos = NULL;
	char* tmpdir = NULL;

	if(dir == NULL || file == NULL)
	{
		debug(1000, "out -> NULL detect");
		return NULL;
	}

	tmpdir = ostrcat(dir, NULL, 0, 0);
	if(tmpdir == NULL)
	{
		debug(1000, "out -> NULL detect");
		return NULL;
	}

	while((tmpdir[strlen(tmpdir) - 1] == '.' && tmpdir[strlen(tmpdir) - 2] != '.') || tmpdir[strlen(tmpdir) - 1] == '/')
		tmpdir[strlen(tmpdir) - 1] = '\0';

	while(tmpdir[strlen(tmpdir) - 1] == '.' && tmpdir[strlen(tmpdir) - 2] == '.')
	{
		tmpdir[strlen(tmpdir) - 1] = '\0';
		tmpdir[strlen(tmpdir) - 1] = '\0';
		if(tmpdir[strlen(tmpdir) - 1] == '/')
			tmpdir[strlen(tmpdir) - 1] = '\0';
		pos = strrchr(tmpdir, '/');
		if(pos != NULL)
			pos[0] = '\0';
	}

	absdir = malloc(strlen(tmpdir) + strlen(file) + 2);
	if(absdir == NULL)
	{
		free(tmpdir);
		err("no memory");
		return NULL;
	}

	if(strlen(file) > 0 || strlen(tmpdir) == 0)
		sprintf(absdir, "%s/%s", tmpdir, file);
	else
		sprintf(absdir, "%s", tmpdir);

	debug(1000, "out");
	free(tmpdir);
	return absdir;
}

int settimezone(char* zone)
{
	debug(1000, "in");
	int ret = 0;
	char* tmpzone = NULL, *zonepath = NULL;

	zonepath = getconfig("zonepath", NULL);
	if(zonepath == NULL)
	{
		err("config zonepath not defined");
		return 1;
	}

	tmpzone = createpath(zonepath, zone);
	if(tmpzone == NULL)
		return 1;

	setenv("TZ", zone, 1);

	ret = unlink(getconfig("localtimefile", NULL));
	if(ret != 0)
		perr("unlink");
	ret = symlink(tmpzone, getconfig("localtimefile", NULL));
	if(ret != 0)
		perr("link");

	free(tmpzone);
	debug(1000, "out");
	return ret;
}

int delchar(char** text, int pos)
{
	debug(1000, "in");
	char *tmptext = NULL;
	int i = 0, y = 0, len = 0;

	if(text == NULL || *text == NULL)
	{
		debug(1000, "out -> NULL detect");
		return pos;
	}

	len = strlen(*text);
	if(len == 0) return pos;

	tmptext = malloc(strlen(*text));
	if(tmptext == NULL)
	{
		err("no memory");
		return pos;
	}

	for(i = 0; i < strlen(*text); i++)
	{
		if(i == pos - 1)
			y++;

		tmptext[i] = (*text)[y];
		y++;
	}

	if(i > 0) tmptext[i - 1] = '\0';
	if(pos >= strlen(tmptext)) pos = strlen(tmptext);

	free(*text);
	*text = tmptext;
	debug(1000, "out");
	return pos;
}


int insertchar(char** text, char zeichen, int pos)
{
	debug(1000, "in");
	char *tmptext = NULL;
	int i, y = 0;

	if(text == NULL || *text == NULL)
	{
		debug(1000, "out -> NULL detect");
		return pos;
	}

	tmptext = malloc(strlen(*text) + 2);
	if(tmptext == NULL)
	{
		err("no memory");
		return pos;
	}

	if(strlen(*text) == 0)
	{
		tmptext[0] = zeichen;
		i = 1;
		pos = 0;
	}
	else
	{
		for(i = 0; i < strlen(*text) + 1; i++)
		{
			if(i == pos)
				tmptext[i] = zeichen;
			else
			{
				tmptext[i] = (*text)[y];
				y++;
			}
		}
	}
	tmptext[i] = '\0';

	free(*text);
	*text = tmptext;
	debug(1000, "out");
	return pos + 1;
}

void calctext(char* buf, char* buf1, unsigned int* linecount, unsigned int* pagecount, unsigned int* poscount, int pagelen, int page)
{
	debug(1000, "in");
	unsigned int tmpposcount = 0;

	*poscount = 0;
	*linecount = 1;
	*pagecount = 1;

	page--;
	if(page < 0) page = 0;

	if(buf != NULL)
	{
		while(*buf != '\0')
		{
			if(*buf == '\n')
			{
				if(*(buf + 1) == '\0') break;
				(*linecount)++;

				if(*linecount - 1 == pagelen * page)
					*poscount = tmpposcount + 1;
			}
			buf++;
			tmpposcount++;
		}
	}

	if(buf1 != NULL)
	{
		while(*buf1 != '\0')
		{
			if(*buf1 == '\n')
			{
				if(*(buf1 + 1) == '\0') break;
				(*linecount)++;

				if(*linecount - 1 == pagelen * page)
					*poscount = tmpposcount + 1;
			}
			buf1++;
			tmpposcount++;
		}
	}

	if(pagelen > 0)
		*pagecount = (int)ceil(((float)*linecount / pagelen));

	debug(1000, "out");
}

int initlocale(char *localepath)
{
	debug(1000, "in");
	setlocale(LC_ALL, "");
	if(bindtextdomain(PROGNAME, localepath) == NULL)
	{
		err("set bindtextdomain");
		return 1;
	}
	if(textdomain(PROGNAME) == NULL)
	{
		err("set textdomain");
		return 1;
	}
	debug(1000, "out");
	return 0;
}

//for langage you must
//mkdir /usr/lib/locale
//ln -s %titanpath%/po /usr/lib/locale/po
//copy SYS_LC_MESSAGES from other language into LC_MESSAGE
int setlang(char *lang)
{
	debug(1000, "in");
	char *ret;

	setenv("LANG", lang, 1);
	setenv("LANGUAGE", lang, 1);
	ret = setlocale(LC_MESSAGES, lang);
	if(ret == NULL)
	{
		err("can't set LC_MESSAGES to %s", lang);
		return 1;
	}
	debug(1000, "out");
	return 0;
}

unsigned long readsysul(const char *filename, int line)
{
	debug(1000, "in");
	int i = 0, len = 0;
	unsigned long ret = 0;
	FILE *fd = NULL;
	char *fileline = NULL;
	char *buf1 = NULL;

	fileline = malloc(MINMALLOC);
	if(fileline == NULL)
	{
		err("no memory");
		return 0;
	}

	fd = fopen(filename, "r");
	if(fd == NULL)
	{
		perr("can't open %s", filename);
		free(fileline);
		return 0;
	}

	for(i = 0; i < line; i++)
	{
		memset(fileline, 0, MINMALLOC);
		fgets(fileline, MINMALLOC, fd);
	}

	len = strlen(fileline) - 1;
	if(len >= 0 && fileline[len] == '\n')
		fileline[len] = '\0';
	buf1 = ostrcat(fileline, NULL, 1, 0);

	fclose(fd);

	if(buf1 != NULL)
	{
		ret = strtoul(buf1, NULL, 10);
		free(buf1); buf1 = NULL;
	}

	debug(1000, "out");
	return ret;
}


char* readsys(const char *filename, int line)
{
	debug(1000, "in");
	int i = 0, len = 0;
	FILE *fd = NULL;
	char *fileline = NULL;
	char *buf1 = NULL;

	fileline = malloc(MINMALLOC);
	if(fileline == NULL)
	{
		err("no memory");
		return NULL;
	}

	fd = fopen(filename, "r");
	if(fd == NULL)
	{
		perr("can't open %s", filename);
		free(fileline);
		return NULL;
	}

	for(i = 0; i < line; i++)
	{
		memset(fileline, 0, MINMALLOC);
		fgets(fileline, MINMALLOC, fd);
	}

	len = strlen(fileline) - 1;
	if(len >= 0 && fileline[len] == '\n')
		fileline[len] = '\0';
	buf1 = ostrcat(fileline, NULL, 1, 0);

	fclose(fd);

	debug(1000, "out");
	return buf1;
}

//flag 0: without \n
//flag 1: with \n
int writesys(const char *filename, char *value, int flag)
{
	debug(1000, "in");
	FILE *fd = NULL;
	char* tmpstr = NULL;
	int ret;

	if(value == NULL)
	{
		debug(1000, "out -> NULL detect");
		return 1;
	}

	fd = fopen(filename, "w");
	if(fd == NULL)
	{
		perr("can't open %s", filename);
		return 1;
	}

	if(flag == 1)
		tmpstr = ostrcat(value, "\n", 0, 0);
	else
		tmpstr = ostrcat(value, NULL, 0, 0);

	ret = fwrite(tmpstr, strlen(tmpstr), 1, fd);
	if(ret != 1)
	{
		perr("writting to %s", filename);
		free(tmpstr);
		fclose(fd);
		return 1;
	}

	free(tmpstr);
	fclose(fd);
	debug(1000, "out");
	return 0;
}

int writesysint(const char *filename, int value, int flag)
{
	debug(1000, "in");
	char* tmpstr = NULL;
	int ret = 0;

	tmpstr = oitoa(value);
	ret = writesys(filename, tmpstr, flag);

	free(tmpstr);
	debug(1000, "out");
	return ret;
}

char* getdevcontent(char* devconfig)
{
	debug(1000, "in");
	char *dev = NULL;
	char *value = NULL;

	dev = getconfig(devconfig, NULL);

	if(dev == NULL)
	{
		debug(1000, "out -> NULL detect");
		return NULL;
	}

	value = readsys(dev, 1);
	if(value == NULL)
	{
		debug(1000, "out -> NULL detect");
		return NULL;
	}

	debug(1000, "out");
	return value;
}

char* convertspacetolf(char* value)
{
	debug(1000, "in");
	int i = 0;

	while(value[i] != '\0')
	{
		if(value[i] == ' ')
			value[i] = '\n';
		i++;
	}

	debug(1000, "out");
	return value;
}

char* getcolorformatchoices()
{
	debug(1000, "in");
	char *colorformatchoicesdev = NULL;
	char *value = NULL;

	colorformatchoicesdev = getconfig("colorformatchoicesdev", NULL);

	if(colorformatchoicesdev == NULL)
	{
		debug(1000, "out -> NULL detect");
		return NULL;
	}

	value = readsys(colorformatchoicesdev, 1);
	if(value == NULL)
	{
		debug(1000, "out -> NULL detect");
		return NULL;
	}

	value = convertspacetolf(value);

	debug(1000, "out");
	return value;
}

char* getaudiosourcechoices()
{
	debug(1000, "in");
	char *audiosourcechoicesdev = NULL;
	char *value = NULL;

	audiosourcechoicesdev = getconfig("audiosourcechoicesdev", NULL);

	if(audiosourcechoicesdev == NULL)
	{
		debug(1000, "out -> NULL detect");
		return NULL;
	}

	value = readsys(audiosourcechoicesdev, 1);
	if(value == NULL)
	{
		debug(1000, "out -> NULL detect");
		return NULL;
	}

	value = convertspacetolf(value);

	debug(1000, "out");
	return value;
}

char* getac3choices()
{
	debug(1000, "in");
	char *ac3choicesdev = NULL;
	char *value = NULL;

	ac3choicesdev = getconfig("ac3choicesdev", NULL);

	if(ac3choicesdev == NULL)
	{
		debug(1000, "out -> NULL detect");
		return NULL;
	}

	value = readsys(ac3choicesdev, 1);
	if(value == NULL)
	{
		debug(1000, "out -> NULL detect");
		return NULL;
	}

	value = convertspacetolf(value);

	debug(1000, "out");
	return value;
}

int setciclock(int slotnr, char* value)
{
	debug(1000, "in");
	char* ciclockdev = NULL, *tmpstr = NULL;
	int ret = 0;

	ciclockdev = getconfig("ciclockdev", NULL);

	if(ciclockdev != NULL && value != NULL)
	{
		tmpstr = malloc(MINMALLOC);
		if(tmpstr == NULL)
		{
			err("no mem");
			return 1;
		}

		snprintf(tmpstr, MINMALLOC, ciclockdev, slotnr);
		debug(100, "set %s to %s", tmpstr, value);
		ret = writesys(tmpstr, value, 0);

		free(tmpstr); tmpstr = NULL;
		return ret;
	}

	debug(1000, "out");
	return 0;
}

int setciinput(int slotnr, char* value)
{
	debug(1000, "in");
	char* ciinputdev = NULL, *tmpstr = NULL;
	int ret = 0;

	ciinputdev = getconfig("ciinputdev", NULL);

	if(ciinputdev != NULL && value != NULL)
	{
		tmpstr = malloc(MINMALLOC);
		if(tmpstr == NULL)
		{
			err("no mem");
			return 1;
		}

		snprintf(tmpstr, MINMALLOC, ciinputdev, slotnr);
		debug(100, "set %s to %s", tmpstr, value);
		ret = writesys(tmpstr, value, 0);

		free(tmpstr); tmpstr = NULL;
		return ret;
	}

	debug(1000, "out");
	return 0;
}

int setcisource(int tunernr, char* value)
{
	debug(1000, "in");
	char* cisourcedev = NULL, *tmpstr = NULL;
	int ret = 0;

	cisourcedev = getconfig("cisourcedev", NULL);

	if(cisourcedev != NULL && value != NULL)
	{
		tmpstr = malloc(MINMALLOC);
		if(tmpstr == NULL)
		{
			err("no mem");
			return 1;
		}

		snprintf(tmpstr, MINMALLOC, cisourcedev, tunernr);
		debug(100, "set %s to %s", tmpstr, value);
		ret = writesys(tmpstr, value, 0);

		free(tmpstr); tmpstr = NULL;
		return ret;
	}

	debug(1000, "out");
	return 0;
}

int setinput(char* value)
{
	debug(1000, "in");
	char* inputdev = NULL;
	int ret = 0;

	inputdev = getconfig("inputdev", NULL);

	if(inputdev != NULL && value != NULL)
	{
		debug(100, "set %s to %s", inputdev, value);
		ret = writesys(inputdev, value, 0);
		return ret;
	}

	debug(1000, "out");
	return 0;
}

char* getinput()
{
	debug(1000, "in");
	char *inputdev = NULL;
	char *value = NULL;

	inputdev = getconfig("inputdev", NULL);

	if(inputdev == NULL)
	{
		debug(1000, "out -> NULL detect");
		return NULL;
	}

	value = readsys(inputdev, 1);
	if(value == NULL)
	{
		debug(1000, "out -> NULL detect");
		return NULL;
	}

	debug(1000, "out");
	return value;
}

int setac3(char* value)
{
	debug(1000, "in");
	char* ac3dev = NULL;
	int ret = 0;

	ac3dev = getconfig("ac3dev", NULL);

	if(ac3dev != NULL && value != NULL)
	{
		debug(100, "set %s to %s", ac3dev, value);
		ret = writesys(ac3dev, value, 0);
		if(ret == 0) addconfig("av_ac3mode", value);
		return ret;
	}

	debug(1000, "out");
	return 0;
}

char* getac3()
{
	debug(1000, "in");
	char *ac3dev = NULL;
	char *value = NULL;

	ac3dev = getconfig("ac3dev", NULL);

	if(ac3dev == NULL)
	{
		debug(1000, "out -> NULL detect");
		return NULL;
	}

	value = readsys(ac3dev, 1);
	if(value == NULL)
	{
		debug(1000, "out -> NULL detect");
		return NULL;
	}

	debug(1000, "out");
	return value;
}

char* getpolicychoices()
{
	debug(1000, "in");
	char *policychoicesdev = NULL;
	char *value = NULL;

	policychoicesdev = getconfig("policychoicesdev", NULL);

	if(policychoicesdev == NULL)
	{
		debug(1000, "out -> NULL detect");
		return NULL;
	}

	value = readsys(policychoicesdev, 1);
	if(value == NULL)
	{
		debug(1000, "out -> NULL detect");
		return NULL;
	}

	value = convertspacetolf(value);

	debug(1000, "out");
	return value;
}

char* getpolicy()
{
	debug(1000, "in");
	char *policydev = NULL;
	char *value = NULL;

	policydev = getconfig("policydev", NULL);

	if(policydev == NULL)
	{
		debug(1000, "out -> NULL detect");
		return NULL;
	}

	value = readsys(policydev, 1);
	if(value == NULL)
	{
		debug(1000, "out -> NULL detect");
		return NULL;
	}

	debug(1000, "out");
	return value;
}

int setpolicy(char* value)
{
	debug(1000, "in");
	char* policydev;
	int ret = 0;

	policydev = getconfig("policydev", NULL);

	if(policydev != NULL && value != NULL)
	{
		debug(100, "set %s to %s", policydev, value);
		ret = writesys(policydev, value, 0);
		if(ret == 0) addconfig("av_policy", value);
		return ret;
	}

	debug(1000, "out");
	return 0;
}

char* getaspectchoices()
{
	debug(1000, "in");
	char *aspectchoicesdev = NULL;
	char *value = NULL;

	aspectchoicesdev = getconfig("aspectchoicesdev", NULL);

	if(aspectchoicesdev == NULL)
	{
		debug(1000, "out -> NULL detect");
		return NULL;
	}

	value = readsys(aspectchoicesdev, 1);
	if(value == NULL)
	{
		debug(1000, "out -> NULL detect");
		return NULL;
	}

	value = convertspacetolf(value);

	debug(1000, "out");
	return value;
}

char* getaspect()
{
	debug(1000, "in");
	char *aspectdev = NULL;
	char *value = NULL;

	aspectdev = getconfig("aspectdev", NULL);

	if(aspectdev == NULL)
	{
		debug(1000, "out -> NULL detect");
		return NULL;
	}

	value = readsys(aspectdev, 1);
	if(value == NULL)
	{
		debug(1000, "out -> NULL detect");
		return NULL;
	}

	debug(1000, "out");
	return value;
}

int setaspect(char* value)
{
	debug(1000, "in");
	char* aspectdev;
	int ret = 0;

	aspectdev = getconfig("aspectdev", NULL);

	if(aspectdev != NULL && value != NULL)
	{
		debug(100, "set %s to %s", aspectdev, value);
		ret = writesys(aspectdev, value, 0);
		if(ret == 0) addconfig("av_aspect", value);
		return ret;
	}

	debug(1000, "out");
	return 0;
}

char* getvideomodechoices()
{
	debug(1000, "in");
	char *videomodechoicesdev = NULL;
	char *value = NULL;

	videomodechoicesdev = getconfig("videomodechoicesdev", NULL);

	if(videomodechoicesdev == NULL)
	{
		debug(1000, "out -> NULL detect");
		return NULL;
	}

	value = readsys(videomodechoicesdev, 1);
	if(value == NULL)
	{
		debug(1000, "out -> NULL detect");
		return NULL;
	}

	if(status.expertmodus < 10)
	{
		value = string_replace("1080p60", "", value, 1);
		value = string_replace("  ", " ", value, 1);
		value = string_replace("1080p59", "", value, 1);
		value = string_replace("  ", " ", value, 1);
		value = string_replace("1080p30", "", value, 1);
		value = string_replace("  ", " ", value, 1);
		value = string_replace("1080p25", "", value, 1);
		value = string_replace("  ", " ", value, 1);
		value = string_replace("1080p24", "", value, 1);
		value = string_replace("  ", " ", value, 1);
		value = string_replace("1080i60", "", value, 1);
		value = string_replace("  ", " ", value, 1);
		value = string_replace("720p60", "", value, 1);
		value = string_replace("  ", " ", value, 1);
	}

	value = convertspacetolf(value);

	debug(1000, "out");
	return value;
}

char* getmode3dchoices()
{
	debug(1000, "in");
	char *mode3dchoicesdev = NULL;
	char *value = NULL;

	mode3dchoicesdev = getconfig("mode3dchoicesdev", NULL);

	if(mode3dchoicesdev == NULL)
	{
		return ostrcat("off\nsbs\ntab", "", 0, 0);
	}

	value = readsys(mode3dchoicesdev, 1);
	if(value == NULL)
	{
		debug(1000, "out -> NULL detect");
		return NULL;
	}

	value = convertspacetolf(value);

	debug(1000, "out");
	return value;
}

char* getmode3d()
{
	debug(1000, "in");
	char *mode3ddev = NULL;
	char *value = NULL;

	mode3ddev = getconfig("mode3ddev", NULL);

	if(mode3ddev == NULL)
	{
		debug(1000, "out -> NULL detect");
		return NULL;
	}

	value = readsys(mode3ddev, 1);
	if(value == NULL)
	{
		debug(1000, "out -> NULL detect");
		return NULL;
	}

	debug(1000, "out");
	return value;
}

int setmode3d(char* value)
{
	debug(1000, "in");
	char* mode3ddev;
	int ret = 0;

	mode3ddev = getconfig("mode3ddev", NULL);

	if(mode3ddev != NULL && value != NULL)
	{
		debug(100, "set %s to %s", mode3ddev, value);
		ret = writesys(mode3ddev, value, 0);
		if(ret == 0) addconfig("av_mode3d", value);
		return ret;
	}

	debug(1000, "out");
	return 0;
}

char* getvideomode()
{
	debug(1000, "in");
	char *videomodedev = NULL;
	char *value = NULL;

	videomodedev = getconfig("videomodedev", NULL);

	if(videomodedev == NULL)
	{
		debug(1000, "out -> NULL detect");
		return NULL;
	}

	value = readsys(videomodedev, 1);
	if(value == NULL)
	{
		debug(1000, "out -> NULL detect");
		return NULL;
	}

	debug(1000, "out");
	return value;
}

void switchvideomode()
{
	int rcret = 0;
	char* tmpstr = NULL;
	tmpstr = getvideomode();	
	struct skin* playpolicy = getscreen("playpolicy");
			
	if(tmpstr != NULL)
	{
		if(ostrcmp("pal", tmpstr) == 0 || ostrncmp("576", tmpstr, 3) == 0)
		{
			setvideomode("720p50", 0);
			changefbresolution("720p50");
			changetext(playpolicy, "720p50");
		}
		else if(ostrncmp("720", tmpstr, 3) == 0)
		{
			setvideomode("1080i50", 0);
			changefbresolution("1080i50");
			changetext(playpolicy, "1080i50");
		}
		else if(ostrncmp("1080", tmpstr, 4) == 0)
		{
			setvideomode("576i50", 0);
			changefbresolution("576i50");
			changetext(playpolicy, "576i50");
		}
		/*
		int ret = textbox(_("Message"), _("Is this Videomode ok ?"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 10, 0);
		if(ret == 0 || ret == 2)
		{
			setvideomode(tmpstr, 0);
			changefbresolution(tmpstr);
		}
		*/
		drawscreen(playpolicy, 0, 0);
		while(1)
		{
			rcret = waitrc(playpolicy, 5000, 0);
			break;
		}
		clearscreen(playpolicy);
	}
	free(tmpstr);
}

//flag 0: write videomode to config
//flag 1: don't write videomode to config
int setvideomode(char* value, int flag)
{
	debug(1000, "in");
	char* videomodedev;
	int ret = 0;

	videomodedev = getconfig("videomodedev", NULL);

	if(videomodedev != NULL && value != NULL)
	{
		debug(100, "set %s to %s", videomodedev, value);
		ret = writesys(videomodedev, value, 0);
		if(ret == 0 && flag == 0) addconfig("av_videomode", value);
		return ret;
	}

	debug(1000, "out");
	return 0;
}

int setcolorformat(char* value)
{
	debug(1000, "in");
	char* colorformatdev;
	int ret = 0;

	colorformatdev = getconfig("colorformatdev", NULL); 

	if(colorformatdev != NULL && value != NULL)
	{
		debug(100, "set %s to %s", colorformatdev, value);
		ret = writesys(colorformatdev, value, 0);
		if(ret == 0) addconfig("av_colorformat", value);
		return ret;
	}

	debug(1000, "out");
	return 0;
}

char* getcolorformat(int line)
{
	debug(1000, "in");
	char *colorformatdev = NULL;
	char *value = NULL;

	colorformatdev = getconfig("colorformatdev", NULL);

	if(colorformatdev == NULL)
	{
		debug(1000, "out -> NULL detect");
		return NULL;
	}

	value = readsys(colorformatdev, line);
	if(value == NULL)
	{
		debug(1000, "out -> NULL detect");
		return NULL;
	}

	debug(1000, "out");
	return value;
}

int setaudiosource(char* value)
{
	debug(1000, "in");
	char* audiosourcedev;
	int ret = 1;

	audiosourcedev = getconfig("audiosourcedev", NULL);

	if(audiosourcedev != NULL && value != NULL)
	{
		debug(100, "set %s to %s", audiosourcedev, value);
		ret = writesys(audiosourcedev, value, 0);
		if(ret == 0) addconfig("av_audiosource", value);
		return ret;
	}

	debug(1000, "out");
	return 0;
}

char* getaudiosource()
{
	debug(1000, "in");
	char *audiosourcedev = NULL;
	char *value = NULL;

	audiosourcedev = getconfig("audiosourcedev", NULL);

	if(audiosourcedev == NULL)
	{
		debug(1000, "out -> NULL detect");
		return NULL;
	}

	value = readsys(audiosourcedev, 1);
	if(value == NULL)
	{
		debug(1000, "out -> NULL detect");
		return NULL;
	}

	debug(1000, "out");
	return value;
}

int setprogress(value)
{
	debug(1000, "in");
	char *progressdev;

	progressdev = getconfig("progressdev", NULL);

	if(progressdev != NULL)
	{
		debug(100, "set %s to %d",progressdev, value);
		return writesysint(progressdev, value, 0);
	}

	debug(1000, "out");
	return 0;
}

int setmute(int value)
{
	debug(1000, "in");
	char* mutedev;
	int tmpvol;

	if(value == 2)
	{
		tmpvol = getvol();
		tmpvol = tmpvol * 50 / 100;
		setvol(tmpvol);
	} 
	else
	{
		mutedev = getconfig("mutedev", NULL);

		if(mutedev != NULL)
		{
			debug(100, "set %s to %d", mutedev, value);
			return writesysint(mutedev, value, 0);
		}
	}
	debug(1000, "out");
	return 0;
}

int setvol(int value)
{
	debug(1000, "in");
	char* voldev;
	int ret = 0, tmpvol = value;

	voldev = getconfig("voldev", NULL);

	if(voldev != NULL)
	{
		if(value > 100) value = 100;
		if(value < 0) value = 0;
		value = 63 - value * 63 / 100;
		debug(100, "set %s to %d", voldev, value);
		ret = writesysint(voldev, value, 0);
		if(ret == 0 && status.mute != 2) addconfigint("vol", tmpvol);
		return ret;
	}

	debug(1000, "out");
	return 0;
}

int getvol()
{
	debug(1000, "in");
	char *voldev = NULL;
	char *value = NULL;
	int tmpvol = 0;

	voldev = getconfig("voldev", NULL);

	if(voldev == NULL)
	{
		debug(1000, "out -> NULL detect");
		return 0;
	}

	value = readsys(voldev, 1);
	if(value == NULL)
	{
		debug(1000, "out -> NULL detect");
		return 0;
	}

	tmpvol = atoi(value);
	free(value);
	tmpvol = 100 - tmpvol * 100 / 63;

	debug(1000, "out");
	return tmpvol;
}

void setdebuglevel()
{
	debug(1000, "in");

	debug_level = getconfigint("debuglevel", NULL);
	debug(0, "set debug level to %d", debug_level);

	debug(1000, "out");
}

char* getxmlentry(char *line, char *searchstr)
{
	//debug(1000, "in");
	char *buf = NULL, *buf1 = NULL, *buf2 = NULL;

	buf = ostrstr(line, searchstr);
	if(buf == NULL)
	{
		//debug(1000, "out -> searchstr=\"%s\" not found in line=\"%s\"", searchstr, line);
		return NULL;
	}
	buf = buf + strlen(searchstr);
	if(buf[0] == '"')
	{
		buf = buf + 1;
		buf1 = ostrcat(buf, NULL, 0, 0);
		if(buf1 == NULL)
		{
			err("ostrcat failed");
			return NULL;
		}
		buf2 = strchr(buf1, '"');
		if(buf2 == NULL)
		{
			err("strchr returns NULL");
			free(buf1);
			return NULL;
		}
		buf2[0] = '\0';
	}
	else
	{
		buf1 = ostrcat(buf, NULL, 0, 0);
		if(buf1 == NULL)
		{
			err("ostrcat failed");
			return NULL;
		}
		buf2 = strchr(buf1, ' ');
		if(buf2 == NULL)
		{
			buf2 = ostrstr(buf1, "/>");
			if(buf2 == NULL) 
			{
				buf2 = strchr(buf1, '>');
				if(buf2 == NULL)
				{
					err("strchr returns NULL");
					free(buf1);
					return NULL;
				}
			}
		}
		buf2[0] = '\0';
	}
	//debug(1000, "out");
	return buf1;
}

char* readfiletomem(const char* filename, int flag)
{
	debug(1000, "in");
	FILE *fd = NULL;
	char *fileline = NULL, *buf = NULL, *buf1 = NULL;
	int buf1size = 0, buf1oldsize = 0;

	fileline = malloc(MINMALLOC);
	if(fileline == NULL)
	{
		err("no memory");
		return NULL;
	}

	fd = fopen(filename, "r");
	if(fd == NULL)
	{
		perr("can't open %s", filename);
		free(fileline);
		return NULL;
	}

	while(fgets(fileline, MINMALLOC, fd) != NULL)
	{
		buf = fileline;

		if(flag == 1)
			if(buf[0] == '#' || buf[0] == '\n')
				continue;

		buf1oldsize = buf1size;
		buf1size += strlen(buf);
		buf1 = realloc(buf1, buf1size + 1);
		if(buf1 == NULL)
		{
			err("no memory");
			free(fileline);
			fclose(fd);
			return NULL;
		}

		sprintf(buf1 + buf1oldsize, "%s", buf);
	}

	free(fileline);
	fclose(fd);
	debug(1000, "out");
	return buf1;
}

char* readeittomem(const char* filename)
{
	unsigned char byte;
	FILE *fil = NULL;
	char *zeichen = NULL, *buf = NULL, *buf1 = NULL;
	int buf1size = 0, buf1oldsize = 0;
	int Beschreibung;
  int len;

	zeichen = malloc(255);
	if(zeichen == NULL)
	{
		err("no memory");
		return NULL;
	}
	buf = malloc(255);
	if(buf == NULL)
	{
		free(zeichen);
		err("no memory");
		return NULL;
	}

	fil = fopen(filename, "r");
	if(fil == NULL)
	{
		err("can't open %s", filename);
		free(zeichen);
		free(buf);
		return NULL;
	}
	Beschreibung = 0;
	fseek(fil, 12, SEEK_SET); //ersten 12 Byte nicht relevant
	while(!feof(fil)) {
		byte=fgetc(fil);
      
		if (byte == 0x4D) {
			fseek(fil, 4,SEEK_CUR);
			byte=fgetc(fil);
			len = byte + 0;
      byte=fgetc(fil);
      fgets(zeichen,len,fil);
			if (byte != 0x05)
				sprintf(buf,"%c%s\n", byte,zeichen);
			else
				sprintf(buf,"%s\n", zeichen);

			buf1oldsize = buf1size;
			buf1size += strlen(buf);
			buf1 = realloc(buf1, buf1size + 1);
			if(buf1 == NULL)
			{
				err("no memory");
				free(zeichen);
				free(buf);
				fclose(fil);
				return NULL;
			}
     	sprintf(buf1 + buf1oldsize, "%s", buf);
       	
 			//printf("T %s\n", zeichen);
			byte=fgetc(fil);
			len = byte + 0;
			byte=fgetc(fil);
			fgets(zeichen,len,fil);
			if (byte != 0x05)
				sprintf(buf,"%c%s\n\n", byte,zeichen);
			else
				sprintf(buf,"%s\n\n", zeichen);
				
			buf1oldsize = buf1size;
			buf1size += strlen(buf);
			buf1 = realloc(buf1, buf1size + 1);
			if(buf1 == NULL)
			{
				err("no memory");
				free(zeichen);
				free(buf);
				fclose(fil);
				return NULL;
			}
     	sprintf(buf1 + buf1oldsize, "%s", buf);

		}
		else if (byte == 0x4E) {
			fseek(fil, 6,SEEK_CUR);
			byte=fgetc(fil);
			len = byte;
			byte=fgetc(fil);
			fgets(zeichen,len,fil);
			if (Beschreibung == 0) {
				if (byte != 0x05)
					sprintf(buf,"%c%s", byte,zeichen);
				else
					sprintf(buf,"%s", zeichen);
				Beschreibung = 1;
			}
 			else {
 				if (byte != 0x05)
					sprintf(buf,"%c%s", byte,zeichen);
				else
					sprintf(buf,"%s", zeichen);
			}	

			buf1oldsize = buf1size;
			buf1size += strlen(buf);
			buf1 = realloc(buf1, buf1size + 1);
			if(buf1 == NULL)
			{
				err("no memory");
				free(zeichen);
				free(buf);
				fclose(fil);
				return NULL;
			}
     	sprintf(buf1 + buf1oldsize, "%s", buf);      
        	  
		}  
		else {
			byte=fgetc(fil);
			len= byte;
			fgets(zeichen,len+1,fil);
		}  
	}
	free(zeichen);
	free(buf);
	fclose(fil);
	return buf1;
}

char* command(char* input)
{
	debug(1000, "in");
	char* tmpstr = NULL, *fileline = NULL;
	FILE *iopipe = NULL;
	
	if(input == NULL) return NULL;

	fileline = malloc(MINMALLOC);
	if(fileline == NULL)
	{
		err("no memory");
		return NULL;
	}

	if((iopipe = popen(input, "r")) == NULL)
	{
		free(fileline);
		return NULL;
	}

	while(!feof(iopipe))
	{
		if(fgets(fileline, MINMALLOC, iopipe) != NULL)
			tmpstr = ostrcat(tmpstr, fileline, 1, 0);
	}

	free(fileline);
	pclose(iopipe);
	debug(1000, "out");
	return tmpstr;
}

char* string_tolower(char *str)
{
	debug(1000, "in");
	int i;

	if(str == NULL) return NULL;

	for( i = 0; i < strlen(str); i++)
		str[i] = tolower(str[i]);

	debug(1000, "out");
	return str;
}

char* string_toupper(char *str)
{
	debug(1000, "in");
	int i;

	if(str == NULL) return NULL;

	for( i = 0; i < strlen(str); i++)
		str[i] = toupper(str[i]);

	debug(1000, "out");
	return str;
}

char* stringreplacecharonce(char *str, char c1, char c2)
{
	debug(1000, "in");
	int i;

	if(str == NULL) return NULL;

	for(i = 0; i < strlen(str); i++)
	{
		if(str[i] == c1)
		{
			str[i] = c2;
			break;
		}
	}

	debug(1000, "out");
	return str;
}

char* stringreplacechar(char *str, char c1, char c2)
{
	debug(1000, "in");
	int i;

	if(str == NULL) return NULL;

	for( i = 0; i < strlen(str); i++)
		if(str[i] == c1) str[i] = c2;

	debug(1000, "out");
	return str;
}

char* string_removechar(char *str)
{
	debug(1000, "in");
	int i;

	if(str == NULL) return NULL;

	for( i = 0; i < strlen(str); i++)
	{
		if(str[i] == '.') str[i] = ' ';
		if(str[i] == '-') str[i] = ' ';
		if(str[i] == '_') str[i] = ' ';
		if(str[i] == '/') str[i] = ' ';
	}
	debug(1000, "out");
	return str;
}

char* string_withchars2return(char *str)
{
	debug(1000, "in");
	int i;

	for( i = 0; i < strlen(str); i++)
	{
		if(str[i] == ' ') str[i] = '\n';
	}

	debug(1000, "out");
	return str;
}

char* string_remove_whitechars(char *text)
{
	debug(1000, "in");
	char *p1 = text, *p2 = text;

	while(*p1 != '\0')
	{
		if(*p1 == ' ')
			++p1;
		else
			*p2++ = *p1++;
	}
	*p2 = '\0';

	debug(1000, "out");
	return text;
}

char* strstrip(char *text)
{
	debug(1000, "in");
	char* tmpstr = text;

	if(text == NULL) return NULL;
	int len = strlen(text);

	while(isspace(tmpstr[len - 1])) tmpstr[--len] = '\0';
	while(*tmpstr && isspace(*tmpstr)) ++tmpstr, --len;

	memmove(text, tmpstr, len + 1);

	debug(1000, "out");
	return text;
}

char* string_strip_whitechars(char *text)
{
	debug(1000, "in");
	int i;

	if(text == NULL)
	{
		debug(1000, "out -> NULL detect");
		return NULL;
	}

//	text = string_removechar(text);

	for(i = 0; text[i] != '\0'; i++)
	{
		if(text[i] == ' ' && text[i + 1] == ' ')
		{
			int i2 = i + 1;
			for(; text[i2] != '\0'; i2++)
			{
				text[i] = text[i2];
				i++;
			}
			text[i2 - 1] = '\0';
			i = -1;
		}
	}

	debug(1000, "out");
	return text;
}

char* string_replace_all(char *search, char *replace, char *string, int free1)
{
	char* tmpstr = NULL;
	char* pos = NULL;

	if(string == NULL || search == NULL)
	{
		tmpstr = ostrcat(tmpstr, string, 1, 0);
		if(free1 == 1) free(string);
		return tmpstr;
	}

	tmpstr = ostrcat(tmpstr, string, 1, 0);
	pos = ostrstr(tmpstr, search);
	while(pos != NULL)
	{
		tmpstr = string_replace(search, replace, tmpstr, 1);
		pos = ostrstr(tmpstr, search);
	}

	if(free1 == 1) free(string);

	return tmpstr;
}

char* string_replace_remove_last_chars(char *search, char *replace, char *string, int free1)
{
	debug(1000, "in");
	char* searchpos = NULL;
	char* tmpstr = NULL;

	if(string == NULL || search == NULL)
	{
		tmpstr = ostrcat(tmpstr, string, 1, 0);
		if(free1 == 1) free(string);
		return tmpstr;
	}

	searchpos = ostrstr(string, search);

	if(searchpos == NULL)
	{
		tmpstr = ostrcat(tmpstr, string, 1, 0);
		if(free1 == 1) free(string);
		return tmpstr;
	}

	tmpstr = strndup(string, searchpos - string);
	if(replace != NULL)
		tmpstr = ostrcat(tmpstr, replace, 1, 0);

	if(free1 == 1) free(string);

	debug(1000, "out");
	return tmpstr;
}

char* string_replace(char *search, char *replace, char *string, int free1)
{
	debug(1000, "in");
	char* searchpos = NULL;
	char* tmpstr = NULL;

	if(string == NULL || search == NULL)
	{
		tmpstr = ostrcat(tmpstr, string, 1, 0);
		if(free1 == 1) free(string);
		return tmpstr;
	}

	searchpos = ostrstr(string, search);

	if(searchpos == NULL)
	{
		tmpstr = ostrcat(tmpstr, string, 1, 0);
		if(free1 == 1) free(string);
		return tmpstr;
	}

	tmpstr = strndup(string, searchpos - string);
	if(replace == NULL)
		tmpstr = ostrcat(tmpstr, NULL, 1, 0);
	else
		tmpstr = ostrcat(tmpstr, replace, 1, 0);
	tmpstr = ostrcat(tmpstr, string + (searchpos - string) + strlen(search), 1, 0);

	if(free1 == 1) free(string);

	debug(1000, "out");
	return tmpstr;
}

//flag 0: search full str
//flag 1: search only end of string
char* ostrrstrcase(char* str, char* search, int len, int flag)
{
	int slen = 0;
	char* tmpstr = NULL;

	if(str == NULL || search == NULL) return NULL;

	if(len == -1) len = strlen(str);
	slen = strlen(search);
	if(slen > len) return NULL;

	for(tmpstr = str + len - slen; tmpstr >= str; tmpstr--)
	{
		if(strncasecmp(tmpstr, search, slen) == 0)
			return tmpstr;
		if(flag == 1) return NULL;
	}

	return NULL;
}

//flag 0: search full str
//flag 1: search only end of string
char* ostrrstr(char* str, char* search, int len, int flag)
{
	int slen = 0;
	char* tmpstr = NULL;

	if(str == NULL || search == NULL) return NULL;

	if(len == -1) len = strlen(str);
	slen = strlen(search);
	if(slen > len) return NULL;

	for(tmpstr = str + len - slen; tmpstr >= str; tmpstr--)
	{
		if(strncmp(tmpstr, search, slen) == 0)
			return tmpstr;
		if(flag == 1) return NULL;
	}

	return NULL;
}

char* ostrstr(char* str, char* search)
{
	//debug(1000, "in");
	char* ret = NULL;

	if(str == NULL || search == NULL) return NULL;
	ret = strstr(str, search);

	//debug(1000, "out");
	return ret;
}

int file_exist(char* filename)
{
	debug(1000, "in");
	if (access(filename, F_OK) == 0)
		return 1;
	else
		return 0;
}

char* string_newline(char* str)
{
	debug(1000, "in");
	if(str == NULL) return NULL;

	int size = strlen(str);

	if(str[size - 1] == '\n')
		str[size - 1] = '\0';
	debug(1000, "out");
	return str;
}

char* string_quote(char* str)
{
	debug(1000, "in");
	char* tmpstr = NULL;

	tmpstr = ostrcat("\"", str, 0, 0);
	tmpstr = ostrcat(tmpstr, "\"", 1, 0);
	debug(1000, "out");
	return tmpstr;
}

struct splitstr* strsplit(char *str, char *tok, int* count)
{
	debug(1000, "in");
	char *tmpstr = NULL;
	struct splitstr *tmparray = NULL;
	*count = 0;

	if(str == NULL || tok == NULL)
		return NULL;

	tmpstr = strtok(str, tok);
	while(tmpstr != NULL)
	{
		*count = *count + 1;
		tmparray = (struct splitstr*)realloc(tmparray, sizeof(struct splitstr*) * (*count));
		if(tmparray == NULL)
			return NULL;
		(&tmparray[(*count) - 1])->part = tmpstr;
		tmpstr = strtok(NULL, tok);
	}

	debug(1000, "out");
	return tmparray;
}

char* string_shortname(char *tmpfilename, int mode)
{
	debug(50, "in %s",tmpfilename);

//	char replacelist[] = "avi mkv x264 se disc0 disc1 disc2 disc3 disc4 0disc 1disc 2disc 3disc 4disc season0 season1 season2 season3 season4 season5 season6 season7 season8 season9 hdtv 720p 1080i 1080p uncut cd0 cd1 cd2 cd3 cd4 cd5 cd6 cd7 cd8 cd9 dvd0 dvd1 dvd2 dvd3 dvd4 ac3d ac3 bdrip bluray cam camrip complete custom cut dc directors dl doku dts dvdr dvdrip dvdscr dvdscreener extended french finnish german hd hddvd hddvdrip hdtv int internal int ld limited multi multisubs nordic ntsc pal pl r1 r5 recut remastered repack rip screener se see special.edition sse stv subbed swedish staffel tc telecine telesync ts unrated ws xxx italian";
	char* str = NULL;

	if (mode==1)
	{
		char* replacelist = "avi mkv x264 xvid se uncut ac3d ac3hd ac3 bdrip bluray cam camrip complete custom cut dc directors dl doku dts dvdr dvdrip dvdscr dvdscreener ecc extended french finnish german hd hddvd hddvdrip hdtv int internal int ld limited multi multisubs nordic ntsc pal pl r1 r5 recut remastered repack rip screener se see special.edition sse stv subbed swedish staffel tc telecine telesync ts unrated ws xxx italian";
		str = ostrcat(str, replacelist, 1, 0);
	}
	else if (mode==2)
	{
		char* replacelist = "avi mkv x264 xvid se uncut ac3d ac3hd ac3 bdrip bluray cam camrip complete custom cut dc directors dl doku dts dvdr dvdrip dvdscr dvdscreener ecc extended french finnish german hd hddvd hddvdrip hdtv int internal int ld limited multi multisubs nordic ntsc pal pl r1 r5 recut remastered repack rip screener se see special.edition sse stv subbed swedish staffel tc telecine telesync ts unrated ws xxx italian disc0 disc1 disc2 disc3 disc4 0disc 1disc 2disc 3disc 4disc season0 season1 season2 season3 season4 season5 season6 season7 season8 season9 hdtv 720p 1080i 1080p cd0 cd1 cd2 cd3 cd4 cd5 cd6 cd7 cd8 cd9 dvd0 dvd1 dvd2 dvd3 dvd4";
		str = ostrcat(str, replacelist, 1, 0);
	}
	else
	{
		char* replacelist = "disc0 disc1 disc2 disc3 disc4 0disc 1disc 2disc 3disc 4disc season0 season1 season2 season3 season4 season5 season6 season7 season8 season9 hdtv 720p 1080i 1080p cd0 cd1 cd2 cd3 cd4 cd5 cd6 cd7 cd8 cd9 dvd0 dvd1 dvd2 dvd3 dvd4";
		str = ostrcat(str, replacelist, 1, 0);
	}

	char* replace = NULL;
	struct splitstr* ret1 = NULL;
	int count = 0;
	int i = 0;
	ret1 = strsplit(str, " ", &count);
	int max = count - 1;
	int first = 1;

	for(i = 0; i < max; i++)
	{
		struct splitstr* ret2 = NULL;
		int count2 = 0;
		int j = 0;
		char *tmpstr = NULL;
		tmpstr = ostrcat(tmpstr, tmpfilename, 1, 0);
		ret2 = strsplit(tmpstr, " ,.-_", &count2);

		for(j = 0; j < count2; j++)
		{
			if(j > 0)
			{					
				if(ostrcmp((&ret1[i])->part, (&ret2[j])->part) == 0)
				{
					if (mode==1)
					{
						tmpfilename = string_replace((&ret2[j])->part, replace, tmpfilename, 1);
						continue;
					}
					else if (mode==2)
					{
						tmpfilename = string_replace_remove_last_chars((&ret2[j])->part, replace, tmpfilename, 1);
						break;
					}
					else
					{
						tmpfilename = string_replace((&ret2[j])->part, replace, tmpfilename, 1);
						continue;
					}
				}
				else if (first == 1 && mode == 2)
				{
//					printf("zahl: %s\n", (&ret2[j])->part);
				 	int theCharacter = atoi((&ret2[j])->part);
					if(theCharacter != 0)
					{
//						printf("found zahl: %s\n", (&ret2[j])->part);
						if(theCharacter > 1800 && theCharacter < 2100)
						{
//							printf("found year: %s\n", (&ret2[j])->part);
							tmpfilename = string_replace_remove_last_chars((&ret2[j])->part, "", tmpfilename, 1);
							break;
						}
					}
				}				
			}
		}
		free(ret2); ret2 = NULL;
		first = 0;
		free(tmpstr), tmpstr = NULL;
	}

	free(ret1); ret1 = NULL;
	free(replace); replace = NULL;
	free(str); str = NULL;

	debug(50, "out %s", tmpfilename);
	return tmpfilename;
}

char* get_uuid(char* device)
{
	debug(60, "in %s", device);
	char* cmd = NULL, *tmpstr = NULL;
	
	if(device == NULL) return NULL;
	
	cmd = ostrcat(cmd, "/bin/blkid -w /dev/null -c /dev/null -s UUID /dev/", 1, 0);
	cmd = ostrcat(cmd, device, 1, 0);
	cmd = ostrcat(cmd, " | cut -d'\"' -f2", 1, 0);

	tmpstr = string_newline(command(cmd));

	if(ostrcmp(string_newline(tmpstr), "") == 0)
	{
		free(tmpstr); tmpstr = NULL;
	}

	debug(60, "out %s", cmd);
	free(cmd); cmd = NULL;
	return tmpstr;
}

char* get_label(char* device)
{
	debug(60, "in %s", device);
	char* cmd = NULL, *tmpstr = NULL;

	if(device == NULL) return NULL;

	cmd = ostrcat(cmd, "/bin/blkid -w /dev/null -c /dev/null -s LABEL /dev/", 1, 0);
	cmd = ostrcat(cmd, device, 1, 0);
	cmd = ostrcat(cmd, " | cut -d'\"' -f2", 1, 0);

	tmpstr = string_newline(command(cmd));

	if(tmpstr == NULL)
		tmpstr = ostrcat(tmpstr, "NONLABEL", 1, 0);

	if(ostrcmp(string_newline(tmpstr), "") == 0)
		tmpstr = ostrcat(tmpstr, "NONLABEL", 1, 0);

	debug(60, "out %s", cmd);
	free(cmd); cmd = NULL;
	return tmpstr;
}

char* get_filesystem(char* device)
{
	debug(60, "in %s", device);
	char* cmd = NULL, *tmpstr = NULL;

	if(device == NULL) return NULL;

	cmd = ostrcat(cmd, "/bin/blkid -w /dev/null -c /dev/null -s TYPE /dev/", 1, 0);
	cmd = ostrcat(cmd, device, 1, 0);
	cmd = ostrcat(cmd, " | cut -d'\"' -f2", 1, 0);

	tmpstr = string_newline(command(cmd));

	if(tmpstr == NULL)
		tmpstr = ostrcat(tmpstr, "NONTYPE", 1, 0);

	if(ostrcmp(string_newline(tmpstr), "") == 0)
		tmpstr = ostrcat(tmpstr, "NONTYPE", 1, 0);

	debug(60, "out %s", cmd);
	free(cmd); cmd = NULL;
	return tmpstr;
}

//flag 0: convert ip to 000.000.000.000
//flag 1: convert ip to 0.0.0.0
char* fixip(char* ipinput, int flag)
{
	debug(60, "in %s", ipinput);
	int ret = 0;
	char* ipout = NULL;
	unsigned char ip[4];
	
	ip[0] = 0;
	ip[1] = 0;
	ip[2] = 0;
	ip[3] = 0;

	if(ipinput == NULL) return NULL;
	ret = sscanf(ipinput, "%hhu.%hhu.%hhu.%hhu", &ip[0], &ip[1], &ip[2], &ip[3]);
	if(ret != 4) return NULL;

	ipout = malloc(16);
	if(ipout == NULL)
	{
		err("no mem");
		return NULL;
	}

	if(flag == 1)
		snprintf(ipout, 16, "%d.%d.%d.%d", ip[0], ip[1], ip[2], ip[3]);
	else
		snprintf(ipout, 16, "%03d.%03d.%03d.%03d", ip[0], ip[1], ip[2], ip[3]);

	return ipout;
}

void setfanspeed(int speed, int aktion)
{
	char* speedWert = NULL;
	char* speedSet = NULL;

	if(speed < 0)
	{
		speedWert = getconfig("fanspeed", NULL);
		if(speedWert == NULL)
			speedSet = ostrcat(speedSet, "170", 1, 0);
		else
			speedSet = ostrcat(speedSet, speedWert, 1, 0);
	}
	else
	{
		if(speed == 0)
			speedSet = ostrcat(speedSet, "115", 1, 0);
		else if(speed == 25)
			speedSet = ostrcat(speedSet, "130", 1, 0);
		else if(speed == 50)
			speedSet = ostrcat(speedSet, "145", 1, 0);
		else if(speed == 75)
			speedSet = ostrcat(speedSet, "155", 1, 0);
		else
			speedSet = ostrcat(speedSet, "170", 1, 0);
	}

	writesys("/proc/stb/fan/fan_ctrl", speedSet, 1);

	if(aktion == 1)
		addconfig("fanspeed", speedSet);

	free(speedSet); speedSet=NULL;
}

void setaktres()
{
	int m_width;
	char* res = NULL;
	char* res_akt = NULL;
	char* res_sd = NULL;
	int count=1;
	int sec = 0;

	if(status.restimer == NULL) return;
printf("1\n");
	sec = (int)status.restimer->param1;
printf("2\n");

	if(sec > 0)
	{
printf("3\n");
		while(status.restimer->aktion == START && count <= sec)
		{
			sleep(1);
			if(status.restimer->aktion != START)
			{
printf("4\n");
				status.restimer = NULL;
				return;
			}
			count++;
		}
	}

printf("5\n");
	if(videoreadqwidth(status.aktservice->videodev) == 0)
	{
printf("6\n");
		m_width = status.videosize.w;
printf("7\n");
		if(m_width == 720)
		{
printf("8\n");
			res_sd = getconfig("av_videomode_autores_sd", NULL);
			if(res_sd == NULL)
				res = ostrcat(res, "576i50", 1, 0);
			else
				res = ostrcat(res, res_sd, 1, 0);
printf("9\n");
		}
		else if (m_width == 1280)
			res = ostrcat(res, "720p50", 1, 0);
		else if (m_width == 1920)
			res = ostrcat(res, "1080i50", 1, 0);
		else
			m_width = 0;
printf("10\n");
		if(m_width > 0)
		{
printf("11\n");
			res_akt = getvideomode();
printf("12\n");
			if (ostrcmp(res_akt, res) != 0)
			{
printf("13\n");
				setvideomode(res, 1);
printf("14\n");
	  		changefbresolution(res);
printf("15\n");
				sleep(1);
printf("16\n");
				screenautores(res, 5, 0);
printf("17\n");
			}
		}
	}
	else
		textbox(_("Message"), _("ERROR cant read res"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 5, 0);

printf("18\n");
	free(res);
printf("19\n");
	res = NULL;
	status.restimer = NULL;
	return;
}

char* gettimestamp()
{
	char* timestamp = NULL;
	struct timeval numtime;
		
	gettimeofday(&numtime, 0);
	timestamp = ostrcat(timestamp, olutoa(numtime.tv_sec), 1, 1);
	timestamp = ostrcat(timestamp, olutoa(numtime.tv_usec), 1, 1);

	return timestamp;
} 

char* string_decode(char* input, int flag)
{
	if(input == NULL) return input;

	while(ostrstr(input, "\\u00") != NULL)
	{
		debug(210, "input: %s", input);
		input = string_replace("\\u00", "%", input, 1);
		debug(210, "input: %s", input);
	}

	while(ostrstr(input, "&amp;") != NULL)
	{
		debug(210, "input: %s", input);
		input = string_replace("&amp;", "und", input, 1);
		debug(210, "input: %s", input);
	}

	while(ostrstr(input, "&gt;") != NULL)
	{
		debug(210, "input: %s", input);
		input = string_replace("&gt;", ">", input, 1);
		debug(210, "input: %s", input);
	}

	while(ostrstr(input, "&lt;") != NULL)
	{
		debug(210, "input: %s", input);
		input = string_replace("&lt;", "<", input, 1);
		debug(210, "input: %s", input);
	}
	
	while(ostrstr(input, "&quot;") != NULL)
	{
		debug(210, "input: %s", input);
		input = string_replace("&quot;", "\"", input, 1);
		debug(210, "input: %s", input);
	}

	while(ostrstr(input, "&#x") != NULL)
	{
		debug(210, "out %s", input);
		input = string_replace("&#x", "%", input, 1);
		debug(210, "input: %s", input);
	}
	while(ostrstr(input, "&#") != NULL)
	{
		debug(210, "input: %s", input);
		input = string_replace("&#", "%", input, 1);
		debug(210, "input: %s", input);
	}
	
	if(flag == 1)
		htmldecode2(input, input);
	else
		htmldecode(input, input);
			
	while(ostrstr(input, ";") != NULL)
	{
		debug(210, "input: %s", input);
		input = string_replace(";", "", input, 1);
		debug(210, "input: %s", input);
	}

	debug(1000, "out");
	return input;
}

char* string_deltags(char* str)
{
	debug(1000, "in");
	int i = 0, y = 0, len = 0;

	if(str == NULL) return 0;

	len = strlen(str);

	int skip = 0;
	for(i = 0; i < len; i++)
	{
		if(str[i] == '<')
			skip = 1;
		else if(str[i] == '>')
			skip = 0;

		if(skip == 0 && str[i] != '>')
		{
			str[y] = str[i];
			y++;
		}
	}
	str[y] = '\0';

	debug(1000, "out");
	return str;
}

char* string_striptags(char* str)
{
	debug(1000, "in");
	int i = 0, len = 0;

	if(str == NULL) return 0;
	
	len = strlen(str);

	int skip =0;
	for(i = 0; i < len; i++)
	{
		if(str[i] == '<')
		{
			debug(210, "found < in string");
			skip = 1;
		}
		else if(str[i] == '>')
		{
			debug(210, "found > in string");
			skip = 0;
			str[i] = ' ';
		}
		if(skip == 1)
			str[i] = ' ';
	}	

	debug(1000, "out");
	return strstrip(str);
}

char* string_resub(char* str, char* str2, char* input, int dir)
{
	debug(1000, "in");
	int len;
	char* tmpstr = NULL, *pos = NULL, *pos2 = NULL;

	if(str == NULL || str2 == NULL || input == NULL) return NULL;

	if(dir == 0)
	{
		len = strlen(str);
		pos = ostrstr(input, str);
		if(pos == NULL) return NULL;
		pos += len;

		pos2 = ostrstr(pos, str2);
		if(pos2 == NULL) return NULL;
	}
	else
	{
		pos2 = ostrstr(input, str2);
		if(pos2 == NULL) return NULL;

		len = strlen(str);
		pos = ostrrstr(input, str, pos2 - input, 0);
		if(pos == NULL) return NULL;
		pos += len;
	}

	tmpstr = strndup(pos, pos2 - pos);

	debug(1000, "out");
	return strstrip(tmpstr);
}

char* ostrstrcase(char* str, char* sub)
{
	size_t len = 0;

	if(str == NULL || sub == NULL) return NULL;

	len = strlen(sub);
	while(*str)
	{
		if(toupper(*str) == toupper(*sub) && strncasecmp(str, sub, len) == 0)
			return str;	
		++str;
	}
	return NULL;
}

#endif
