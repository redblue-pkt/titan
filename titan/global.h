#ifndef GLOBAL_H
#define GLOBAL_H

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

//can use to disable a menu for a box (node type must be MENU)
int checkmenuforbox(char *name)
{
	if(status.expertmodus == 1) return 1;
	if(checkbox("ATEMIO500") == 1)
	{
		if(ostrcmp("vfdisplay", name) == 0) return 0;
		if(ostrcmp("savesettings", name) == 0) return 0;
		if(ostrcmp("recordpath", name) == 0) return 0;
		if(ostrcmp("videosettings", name) == 0) return 0;
		if(ostrcmp("scartrecorder", name) == 0) return 0;
		if(ostrcmp("information", name) == 0) return 0;
		if(ostrcmp("plugins", name) == 0) return 0;
		if(ostrcmp("vfdisplay", name) == 0) return 0;
		if(ostrcmp("rotorsettings", name) == 0) return 0;
		if(ostrcmp("satconfig", name) == 0) return 0;
		if(ostrcmp("satfinder", name) == 0) return 0;
		if(ostrcmp("configurehdd", name) == 0) return 0;
		if(ostrcmp("panel_settings_overclocking", name) == 0) return 0;
		if(ostrcmp("panel_settings_fancontrol", name) == 0) return 0;
		if(ostrcmp("panel_settings_automount", name) == 0) return 0;
		if(ostrcmp("panel_settings_autostart", name) == 0) return 0;
		if(ostrcmp("panel_settings_videotune", name) == 0) return 0;
		if(ostrcmp("panel_system_update_usb_online", name) == 0) return 0;
		if(ostrcmp("panel_system_update_usb_tmp", name) == 0) return 0;
		if(ostrcmp("panel_extensions_menu", name) == 0) return 0;
		if(ostrcmp("panel_system_backup", name) == 0) return 0;
		if(ostrcmp("sambasettings", name) == 0) return 0;
		if(ostrcmp("nfssettings", name) == 0) return 0;
		if(ostrcmp("panel_infos_kernel", name) == 0) return 0;
		if(ostrcmp("panel_system_wizard", name) == 0) return 0;
		if(ostrcmp("panel_sysinfos_module", name) == 0) return 0;
	}
	return 1;
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

char* createpiconpath(struct channel* chnode)
{
	char* tmpstr = NULL, *tmpnr = NULL;

	if(chnode != NULL)
	{
		tmpstr = ostrcat(tmpstr, getconfig("piconpath", NULL), 1, 0);
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
		tmpstr = ostrcat(tmpstr, getconfig("piconpath", NULL), 1, 0);
		tmpstr = ostrcat(tmpstr, "/", 1, 0);
		tmpstr = ostrcat(tmpstr, "default.png", 1, 0);
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
int writeallconfig(int flag)
{
	debug(1000, "in");
	char* tmpstr = NULL;
	int ret = 0;

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