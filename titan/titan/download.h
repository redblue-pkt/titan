#ifndef DOWNLOAD_H
#define DOWNLOAD_H

int screendownload(char* title, char* host, char* page, int port, char* filename, char* auth, int timeout, int flag)
{
	int rcret = -1, count = 0, ret = 0, fromthread = 0, sleeptime = 2;
	struct skin* download = getscreen("download");
	struct skin* progress = getscreennode(download, "progress");
	struct skin* file = getscreennode(download, "file");
	struct skin* maxkb = getscreennode(download, "maxkb");
	struct skin* aktkb = getscreennode(download, "aktkb");
	struct skin* framebuffer = getscreen("framebuffer");
	char* bg = NULL, *tmpstr = NULL;
	struct download* dnode = NULL;

	if(pthread_self() != status.mainthread)
		fromthread = 1;

	if(filename == NULL)
	{
		err("filename = NULL");
		return 1;
	}

	changetitle(download, title);
	changetext(file, filename);
	progress->progresssize = 0;
	changetext(maxkb, NULL);
	changetext(aktkb, NULL);

	if(fromthread == 1)
	{
		delrc(getrcconfigint("rcvolup", NULL), NULL, NULL);
		delrc(getrcconfigint("rcvoldown", NULL), NULL, NULL);
		delrc(getrcconfigint("rcmute", NULL), NULL, NULL);
		delrc(getrcconfigint("rcmute1", NULL), NULL, NULL);
		m_lock(&status.drawingmutex, 0);
		m_lock(&status.rcmutex, 10);
		setnodeattr(download, framebuffer, 2);
		status.rcowner = download;
		bg = savescreen(download);
		drawscreen(download, 0, 2);
	}
	else
		drawscreen(download, 0, 0);


	dnode = calloc(1, sizeof(struct download));
	if(dnode == NULL)
	{
		err("no mem");
		return 1;
	}

	dnode->host = host;
	dnode->page = page;
	dnode->port = port;
	dnode->filename = filename;
	dnode->auth = auth;
	dnode->connfd = -1;
	dnode->ret = -1;
	dnode->timeout = timeout;
	
	addtimer(&gethttpstruct, START, 1000, 1, (void*)dnode, NULL, NULL);

	while(1)
	{
		rcret = waitrc(download, 1000, 0);

		if(rcret == RCTIMEOUT)
		{
			progress->progresssize = dnode->proz;
			tmpstr = oitoa(dnode->maxkb / 1024);
			tmpstr = ostrcat(_("Filesize (KB): "), tmpstr, 0, 1);
			changetext(maxkb, tmpstr);
			free(tmpstr); tmpstr = NULL;
			tmpstr = oitoa(dnode->aktkb / 1024);
			tmpstr = ostrcat(_("Download (KB): "), tmpstr, 0, 1);
			changetext(aktkb, tmpstr);
			free(tmpstr); tmpstr = NULL;
		}
		drawscreen(download, 0, 0);
		if(rcret == getrcconfigint("rcexit", NULL)) break;
		if(dnode->ret == 0)
		{
			progress->progresssize = dnode->proz;
			tmpstr = oitoa(dnode->aktkb / 1024);
			tmpstr = ostrcat(_("Download (KB): "), tmpstr, 0, 1);
			changetext(aktkb, tmpstr);
			free(tmpstr); tmpstr = NULL;
			break;
		}
		if(dnode->ret > 0) break;
	}

	if(dnode->ret > 0 && rcret != getrcconfigint("rcexit", NULL))
	{
		changetext(file, _("download error"));
		sleeptime = 5;
	}
	else
		changetext(file, _("wait for stopping download"));
	drawscreen(download, 0, 0);
	sockclose(&dnode->connfd);
	sleep(sleeptime);
	count = 0;
	while(dnode->ret < 0 && count < 10)
	{
		count++;
		sleep(1);
	}
	ret = dnode->ret;
	
	if(count < 10)
	{
		free(dnode);
		dnode = NULL;
	}
	else
		addoldentry((void*)dnode, 1, time(NULL) + 7200, NULL);

	if(fromthread == 1)
	{
		clearscreennolock(download);
		restorescreen(bg, download);
		blitfb(0);
		sleep(1);
		status.rcowner = NULL;
		m_unlock(&status.rcmutex, 10);
		m_unlock(&status.drawingmutex, 0);
		addrc(getrcconfigint("rcvolup", NULL), screenvolumeup, NULL, NULL);
		addrc(getrcconfigint("rcvoldown", NULL), screenvolumedown, NULL, NULL);
		addrc(getrcconfigint("rcmute", NULL), screenmute, NULL, NULL);
		addrc(getrcconfigint("rcmute1", NULL), screenmute, NULL, NULL);
	}
	else
	{
		clearscreen(download);
		drawscreen(skin, 0, 0);
	}

	return ret;
}

#endif
