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
	int rcret = -1, ret = 0;
	char* cmd = NULL, *buf = NULL;
	char* tmpstr = NULL;
	int fifo = -1;
	struct skin* gmediarender = getscreen("gmediarender");
	
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

	if(file_exist("/var/usr/local/share/titan/plugins/gmediarender/gmediarender.sh") == 1)
		cmd = ostrcat(cmd, "/var/usr/local/share/titan/plugins/gmediarender/gmediarender.sh gmediarender-", 1, 0);
	else if(file_exist("/var/swap/usr/local/share/titan/plugins/gmediarender/gmediarender.sh") == 1)
		cmd = ostrcat(cmd, "/var/swap/usr/local/share/titan/plugins/gmediarender/gmediarender.sh gmediarender-", 1, 0);
	else
		cmd = ostrcat(cmd, "/mnt/swapextensions/usr/local/share/titan/plugins/gmediarender/gmediarender.sh gmediarender-", 1, 0);
	
	cmd = ostrcat(cmd, status.boxtype, 1, 0);
	cmd = ostrcat(cmd, " &", 1, 0);

	//start renderer
	debug(10, "cmd: %s", cmd);
	system(cmd);
	free(cmd), cmd = NULL;

	tmpstr = ostrcat(tmpstr, _("Servername"), 1, 0);
	tmpstr = ostrcat(tmpstr, ": gmediarender-", 1, 0);
	tmpstr = ostrcat(tmpstr, status.boxtype, 1, 0);
	tmpstr = ostrcat(tmpstr, "\n\n", 1, 0);
	tmpstr = ostrcat(tmpstr, _("Wait for connect or press EXIT"), 1, 0);

	textbox(_("Message"), tmpstr, _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 7, 0);
	free(tmpstr), tmpstr = NULL;
	drawscreen(gmediarender, 0, 0);

//load screenserver
	int count = 0, screensaver_delay = 0, saver = 0, rcwait = 1000;
	if(getconfigint("screensaver", NULL) == 1)
	{
		screensaver_delay = getconfigint("screensaver_delay", NULL);
		initscreensaver();
	}
//	
	while(1)
	{
		rcret = waitrc(NULL, rcwait, 0);

		if(fifo > -1)
		{
			ret = read(fifo, buf, MINMALLOC);
			if(ret > 0)
			{
				buf[ret] = '\0';
				if(ostrcmp(buf, "clear.png") == 0) //clear screen
				{
					count = 0;
					rcwait = 1000;
//					if(getconfigint("screensaver", NULL) == 1)
//						drawscreen(skin, 0, 0);
//					else
						drawscreen(gmediarender, 0, 0);
					printf("clear.png set count=%d set rcwait=%d\n", count, rcwait);
				}
				else if(ostrcmp(buf, "saver.png") == 0) //saver screen
				{
					if(getconfigint("screensaver", NULL) == 1)
					{
						count = 1;
						drawscreen(skin, 0, 0);
					}
					else
					{
//						count = 0;
						changepic(gmediarender, "%pluginpath%/gmediarender/skin/background.jpg");
						drawscreen(gmediarender, 0, 0);
					}

					printf("11saver.png set count=%d\n", count);
				}
				else
				{
					count = 0;
					rcwait = 1000;
					drawscreen(skin, 0, 0);
					gmediarendergetpic(gmediarender, buf);
					printf("buf: %s set count=%d\n set rcwait=%d", buf, count, rcwait);
				}
			}
//check screenserver

			if(getconfigint("screensaver", NULL) == 1)
			{
				if(count > 0 && screensaver != NULL)
				{
					count++;
					printf("count++ %d\n", count);
				}
	
				if(count > screensaver_delay && screensaver != NULL)
				{
					printf("showscreensaver %d\n", count);
					showscreensaver();
					rcwait = screensaver->speed;
				}
			}
//////
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

	//close fifo
	close(fifo);
	unlink("/tmp/gmediarender");
	unlink("/tmp/gmediarenderpic.jpg");
	unlink("/tmp/gmediarenderpic.png");
}

#endif
