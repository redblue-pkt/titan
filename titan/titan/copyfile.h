#ifndef COPYFILE_H
#define COPYFILE_H

struct copyfile
{
	char* from;
	char* to;
	int proz;
	off64_t maxkb;
	off64_t aktkb;
	int ret;
	int flag;
};

int copyfile(char* from, char* to, struct copyfile* cnode)
{
	int fdfrom = -1, fdto = -1, ret = 0, readret = 0, writeret = 0;
	off64_t count = 0;
	unsigned char* buf = NULL;

	fdfrom = open(from, O_RDONLY);
	if(fdfrom < 0)
	{
		perr("open from %s", from);
		ret = 1;
		goto end;
	}
	
	ret = unlink(to);
	if(ret != 0 && errno != ENOENT)
	{
		perr("remove file %s", to);
		ret = 1;
		goto end;
	}
	else
		ret = 0;
		
	fdto = open(to, O_CREAT | O_TRUNC | O_WRONLY | O_LARGEFILE, 0777);
	if(fdto < 0)
	{
		perr("open to %s", to);
		ret = 1;
		goto end;
	}

	buf = malloc(MINMALLOC);
	if(buf == NULL)
	{
		err("no mem");
		ret = 1;
		goto end;
	}
	
	len = lseek64(fdfrom, 0, SEEK_END);
	if(len < 0)
	{
		perr("can't get filelen %s", from);
		ret = 1;
		goto end;
	}

  if(cnode != NULL) cnode->maxkb = len;  	

  while(len != 0)
	{		
		if(len - count > MINMALLOC)
			readret = dvbreadfd(fdfrom, buf, 0, MINMALLOC, -1, 1);
		else
			readret = dvbreadfd(fdfrom, buf, 0, len - count, -1, 1);
		if(readret <= 0)
		{
			err("read file %s", from);
			ret = 1;
			goto end;
		}

		writeret = dvbwrite(fdto, buf, readret, -1);
		if(writeret != readret)
		{
			err("write file %s", to);
			ret = 1;
			goto end;
		}

		count += readret;
		if(cnode != NULL) cnode->aktkb = count;
		
		if(count == len) break;
		if(count > len)
		{
			err("write more then filelen %s", to);
			ret = 1;
			goto end;
		}
	}
	
end:
	if(fdfrom >= 0)
		close(fdfrom);
	if(fdto >= 0)
	{
		close(fdto);
		if(flag == 0 && ret == 1) unlink(to);
	}
	free(buf);

  if(cnode != NULL) cnode->ret = ret;

	return ret;
}

int movefile(char* from, char* to, struct copyfile* cnode)
{
	int ret = 0;
	
	ret = copyfile(from, to, cnode);
	if(ret == 0)
	{
		ret = unlink(from);
		if(ret != 0 && errno != ENOENT)
		{
			perr("remove file %s", from);
			ret = 1;
		}
		else
			ret = 0;	
	}
	
	if(ret == 1)
		unlink(to);
	
	return ret;
}

void copyfilestruct(struct stimerthread* timernode, struct copyfile* node, int flag)
{
	if(node != NULL)
		copyfile(node->from, node->to, node);
}

void movefilestruct(struct stimerthread* timernode, struct copyfile* node, int flag)
{
	if(node != NULL)
		movefile(node->from, node->to, node);
}

//flag 0: copy
//flag 1: move
int screencopy(char* title, char* from, char* to, int flag)
{
	debug(1000, "in");
	int rcret = -1, count = 0, ret = 0, fromthread = 0, sleeptime = 2;
	struct skin* download = getscreen("copyfile");
	struct skin* progress = getscreennode(download, "progress");
	struct skin* filefrom = getscreennode(download, "filefrom");
	struct skin* fileto = getscreennode(download, "fileto");
	struct skin* maxkb = getscreennode(download, "maxkb");
	struct skin* aktkb = getscreennode(download, "aktkb");
	struct skin* framebuffer = getscreen("framebuffer");
	char* bg = NULL, *tmpstr = NULL;
	struct copyfile* cnode = NULL;

	if(pthread_self() != status.mainthread)
		fromthread = 1;

	if(from == NULL || to == NULL)
	{
		err("NULL detect");
		return 1;
	}

	changetitle(copyfile, title);
	changetext(filefrom, from);
	changetext(fileto, to);
	progress->progresssize = 0;
	changetext(maxkb, NULL);
	changetext(aktkb, NULL);

	if(fromthread == 1)
	{
		delrc(getrcconfigint("rcvolup", NULL), NULL, NULL);
		delrc(getrcconfigint("rcvoldown", NULL), NULL, NULL);
		delrc(getrcconfigint("rcmute", NULL), NULL, NULL);
		m_lock(&status.drawingmutex, 0);
		m_lock(&status.rcmutex, 10);
		setnodeattr(copyfile, framebuffer, 2);
		status.rcowner = copyfile;
		bg = savescreen(copyfile);
		drawscreen(copyfile, 0, 2);
	}
	else
		drawscreen(copyfile, 0, 0);

	cnode = calloc(1, sizeof(struct copyfile));
	if(cnode == NULL)
	{
		err("no mem");
		return 1;
	}

	cnode->from = from;
	cnode->to = to;
	dnode->ret = -1;
	
	if(flag == 1)
		addtimer(&movefilestruct, START, 1000, 1, (void*)cnode, NULL, NULL);
	else
		addtimer(&copyfilestruct, START, 1000, 1, (void*)cnode, NULL, NULL);

	while(1)
	{
		rcret = waitrc(copyfile, 1000, 0);

		if(rcret == RCTIMEOUT)
		{
			progress->progresssize = cnode->proz;
			tmpstr = oitoa(cnode->maxkb / 1024);
			tmpstr = ostrcat(_("Filesize (KB): "), tmpstr, 0, 1);
			changetext(maxkb, tmpstr);
			free(tmpstr); tmpstr = NULL;
			tmpstr = oitoa(cnode->aktkb / 1024);
			if(flag == 1)
				tmpstr = ostrcat(_("Move (KB): "), tmpstr, 0, 1);
			else
				tmpstr = ostrcat(_("Copy (KB): "), tmpstr, 0, 1);
			changetext(aktkb, tmpstr);
			free(tmpstr); tmpstr = NULL;
		}
		drawscreen(copyfile, 0, 0);
		if(rcret == getrcconfigint("rcexit", NULL)) break;
		if(cnode->ret == 0)
		{
			progress->progresssize = cnode->proz;
			tmpstr = oitoa(dnode->aktkb / 1024);
			if(flag == 1)
				tmpstr = ostrcat(_("Move (KB): "), tmpstr, 0, 1);
			else
				tmpstr = ostrcat(_("Copy (KB): "), tmpstr, 0, 1);
			changetext(aktkb, tmpstr);
			free(tmpstr); tmpstr = NULL;
			break;
		}
		if(cnode->ret > 0) break;
	}

	if(cnode->ret > 0 && rcret != getrcconfigint("rcexit", NULL))
	{	
		if(flag == 1)
			changetext(file, _("file move error"));
		else
			changetext(file, _("file copy error"));
		sleeptime = 5;
	}
	else
		changetext(file, _("wait for stopping"));
	drawscreen(copyfile, 0, 0);
	sleep(sleeptime);
	count = 0;
	while(cnode->ret < 0 && count < 10)
	{
		count++;
		sleep(1);
	}
	ret = cnode->ret;
	free(cnode); cnode = NULL;

	if(fromthread == 1)
	{
		clearscreennolock(copyfile);
		restorescreen(bg, copyfile);
		blitfb(0);
		sleep(1);
		status.rcowner = NULL;
		m_unlock(&status.rcmutex, 10);
		m_unlock(&status.drawingmutex, 0);
		addrc(getrcconfigint("rcvolup", NULL), screenvolumeup, NULL, NULL);
		addrc(getrcconfigint("rcvoldown", NULL), screenvolumedown, NULL, NULL);
		addrc(getrcconfigint("rcmute", NULL), screenmute, NULL, NULL);
	}
	else
	{
		clearscreen(copyfile);
		drawscreen(skin, 0, 0);
	}

	debug(1000, "out");

	return ret;
}

#endif
