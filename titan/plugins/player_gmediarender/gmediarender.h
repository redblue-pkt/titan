#ifndef GMEDIARENDER_H
#define GMEDIARENDER_H

extern struct skin* skin;
struct fb *fb = NULL;
extern struct screensaver* screensaver;

void gmediarendergetpic(struct skin* gmediarender, char* buf)
{
	char *tmpip = NULL, *pos = NULL, *pos1 = NULL, *path = NULL;
	int port = 80;
	printf("dlna download link: %s\n", buf);
		
	if(buf == NULL) return;

	tmpip = string_replace("http://", "", buf, 0);

	if(tmpip != NULL)
	{
		pos = strchr(tmpip, '/');
		pos1 = strchr(tmpip, ':');
	}
	if(pos != NULL)
	{
		pos[0] = '\0';
		path = pos + 1;
	}
	if(pos1 != NULL)
	{
		pos1[0] = '\0';
		port = atoi(pos1 + 1);
	}

	if(cmpfilenameext(path, ".jpg") == 0)
	{
		unlink("/tmp/gmediarenderpic.jpg");
/*
		char* cmd = NULL;
		cmd = ostrcat(cmd, "wget ", 1, 0);
		cmd = ostrcat(cmd, buf, 1, 0);
		cmd = ostrcat(cmd, " -O /tmp/gmediarenderpic.jpg", 1, 0);
		printf("cmd: %s\n", cmd);
		system(cmd); 
		free(cmd), cmd = NULL;
*/
		gethttp(tmpip, path, port, "/tmp/gmediarenderpic.jpg", NULL, 5000, NULL, 0);
		changepic(gmediarender, "/tmp/gmediarenderpic.jpg");
		gmediarender->picwidth = 1;
		gmediarender->picheight = 1;
		gmediarender->bgcol = 0;
		gmediarender->picquality = getconfigint("mc_pp_picquality", NULL);
		drawscreen(gmediarender, 0, 4);
	}
	if(cmpfilenameext(path, ".png") == 0)
	{
		unlink("/tmp/gmediarenderpic.png");
/*
		char* cmd = NULL;
		cmd = ostrcat(cmd, "wget ", 1, 0);
		cmd = ostrcat(cmd, buf, 1, 0);
		cmd = ostrcat(cmd, " -O /tmp/gmediarenderpic.png", 1, 0);
		printf("cmd: %s\n", cmd);
		system(cmd); 
		free(cmd), cmd = NULL;
*/
		gethttp(tmpip, path, port, "/tmp/gmediarenderpic.png", NULL, 5000, NULL, 0);
		changepic(gmediarender, "/tmp/gmediarenderpic.png");
		gmediarender->picwidth = 1;
		gmediarender->picheight = 1;
		gmediarender->bgcol = 0;
		gmediarender->picquality = getconfigint("mc_pp_picquality", NULL);
		drawscreen(gmediarender, 0, 4);
	}
}

void screengmediarender()
{
	int rcret = -1, ret = 0, rcsaverwait = 0, count = 0, screensaver_delay = 0, rcwait = 1000;
	char* cmd = NULL, *buf = NULL, *tmpstr = NULL, *tmpstr1 = NULL;
	int fifo = -1;
	struct skin* gmediarender = getscreen("gmediarender");
	struct skin* servername = getscreennode(gmediarender, "servername");
	struct skin* connect = getscreennode(gmediarender, "connect");
	struct skin* loading = getscreen("loading");
	
	ret = servicestop(status.aktservice, 1, 1);
	if(ret == 1) return;

	buf = malloc(MINMALLOC);
	if(buf == NULL)
	{
		err("no mem");
		return;
	}

	//create fifo
	mkfifo("/tmp/gmediarender", 0666);

	//open fifo
	fifo = open("/tmp/gmediarender", O_RDONLY | O_NONBLOCK);

	drawscreen(skin, 0, 0);
	drawscreen(gmediarender, 0, 0);

	tmpstr = ostrcat(status.boxtype, NULL, 0, 0);
	struct inetwork* net = getinetworkbydevice("eth0");
	if(net != NULL)
	{
		tmpstr = ostrcat(tmpstr, "-", 1, 0);
		tmpstr = ostrcat(tmpstr, net->ip, 1, 0);
	}

	if(file_exist("/var/usr/local/share/titan/plugins/gmediarender/gmediarender.sh") == 1)
		cmd = ostrcat(cmd, "/var/usr/local/share/titan/plugins/gmediarender/gmediarender.sh ", 1, 0);
	else if(file_exist("/var/swap/usr/local/share/titan/plugins/gmediarender/gmediarender.sh") == 1)
		cmd = ostrcat(cmd, "/var/swap/usr/local/share/titan/plugins/gmediarender/gmediarender.sh ", 1, 0);
	else
		cmd = ostrcat(cmd, "/mnt/swapextensions/usr/local/share/titan/plugins/gmediarender/gmediarender.sh ", 1, 0);
	
	cmd = ostrcat(cmd, tmpstr, 1, 0);
	cmd = ostrcat(cmd, " &", 1, 0);

	//start renderer
	debug(10, "cmd: %s", cmd);
	system(cmd);
	free(cmd), cmd = NULL;

	tmpstr1 = ostrcat(tmpstr1, _("DLNA Renderer Name"), 1, 0);
	tmpstr1 = ostrcat(tmpstr1, ": ", 1, 0);
	tmpstr1 = ostrcat(tmpstr1, tmpstr, 1, 0);
	changetext(servername, tmpstr1);
	free(tmpstr), tmpstr = NULL;

	tmpstr = ostrcat(_("Wait for connect or press EXIT"), NULL, 0, 0);
	changetext(connect, tmpstr);

	changepic(gmediarender, "%pluginpath%/gmediarender/skin/background.jpg");					
	drawscreen(gmediarender, 0, 0);

//load screenserver
	if(getconfigint("screensaver", NULL) == 1)
	{
		screensaver_delay = getconfigint("screensaver_delay", NULL);
		initscreensaver();
	}
//
	while(1)
	{
		rcret = waitrcext(NULL, rcwait, 0, 0);

		if(fifo > -1)
		{
			ret = read(fifo, buf, MINMALLOC);
			if(ret > 0)
			{
				changetext(connect, _("DLNA Control Client connected !"));
				buf[ret] = '\0';

				if(ostrcmp(buf, "clear.png") == 0) //clear screen
				{
					servername->hidden = NO;
					connect->hidden = NO;
					count = 0;
					rcwait = 1000;
					rcsaverwait = 0;
					servicestop(status.aktservice, 1, 1);
					drawscreen(skin, 0, 0);
				}
				else if(ostrcmp(buf, "done.png") == 0) //clear screen
				{
					count = 0;
					rcwait = 1000;
					servicestop(status.aktservice, 1, 1);
					drawscreen(skin, 0, 0);
					changepic(gmediarender, "%pluginpath%/gmediarender/skin/background.jpg");
					drawscreen(gmediarender, 0, 0);
				}
				else if(ostrcmp(buf, "saver.png") == 0) //saver screen
				{
					servername->hidden = NO;
					connect->hidden = NO;

					if(getconfigint("screensaver", NULL) == 1)
					{
						count = 1;
						rcsaverwait = 0;
						drawscreen(skin, 0, 0);
					}
					else
					{
						servicestop(status.aktservice, 1, 1);
						changepic(gmediarender, "%pluginpath%/gmediarender/skin/background.jpg");
						drawscreen(gmediarender, 0, 0);
					}
				}
				else
				{
					count = 0;
					rcwait = 1000;
					servername->hidden = YES;
					connect->hidden = YES;
					drawscreen(skin, 0, 0);
					drawscreen(loading, 0, 0);
					gmediarendergetpic(gmediarender, buf);
				}
			}
//check screenserver

			if(getconfigint("screensaver", NULL) == 1)
			{
				if(count > 0 && screensaver != NULL)
				{
					count++;
				}

				if(count > screensaver_delay && screensaver != NULL)
				{						
					if(rcsaverwait == 0)
					{
						showscreensaver();
						rcsaverwait = screensaver->speed;
					}

					if(screensaver->type == 2)
					{
						rcwait = screensaver->speed;
						showscreensaver();
					}
					else
					{
						if(count * 1000 >= rcsaverwait)
						{
							showscreensaver();
							rcsaverwait += screensaver->speed;
						}
						rcwait = 1000;
					}					
				}
			}
		}

 		if(rcret == getrcconfigint("rcexit", NULL)) break;
		if(rcret == getrcconfigint("rcok", NULL)) break;
	}

	//stop renderer
	system("killall -9 gmediarender");

	//stop screenserver
	deinitscreensaver();
		
	if(status.lastservice != NULL)
		servicestart(status.lastservice->channel, NULL, NULL, 0);

	free(buf); buf = NULL;
	free(tmpstr), tmpstr = NULL;
	free(tmpstr1), tmpstr1 = NULL;

	//close fifo
	close(fifo);
	unlink("/tmp/gmediarender");
	unlink("/tmp/gmediarenderpic.jpg");
	unlink("/tmp/gmediarenderpic.png");
	delownerrc(gmediarender);
	clearscreen(loading);
	clearscreen(gmediarender);
}

#endif
