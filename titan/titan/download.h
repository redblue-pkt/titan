#ifndef DOWNLOAD_H
#define DOWNLOAD_H

int screendownload(char* title, char* host, char* page, int port, char* filename, char* auth, int flag)
{
	debug(1000, "in");
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
		m_lock(&status.drawingmutex, 0);
		m_lock(&status.rcmutex, 10);
		setnodeattr(download, framebuffer, 2);
		status.rcowner = download;
		bg = savescreen(download);
		drawscreen(download, 0, 2);
	}
	else
		drawscreen(download, 0, 0);


	dnode = malloc(sizeof(struct download));
	if(dnode == NULL)
	{
		err("no mem");
		return 1;
	}
	memset(dnode, 0, sizeof(struct download));

	dnode->host = host;
	dnode->page = page;
	dnode->port = port;
	dnode->filename = filename;
	dnode->auth = auth;
	dnode->connfd = -1;
	dnode->ret = -1;
	
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
	free(dnode); dnode = NULL;

	if(fromthread == 1)
	{
		clearscreennolock(download);
		restorescreen(bg, download);
		blitfb(0);
		sleep(1);
		status.rcowner = NULL;
		m_unlock(&status.rcmutex, 10);
		m_unlock(&status.drawingmutex, 0);
	}
	else
	{
		clearscreen(download);
		drawscreen(skin, 0, 0);
	}

	debug(1000, "out");

	return ret;
}

#endif
