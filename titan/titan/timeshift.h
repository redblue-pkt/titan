#ifndef TIMESHIFT_H
#define TIMESHIFT_H

void timeshiftpause()
{
	int ret = 0;

	if(status.timeshift == 0)
	{
		ret = servicestop(status.aktservice, 0, 2);
		if(ret == 0)
		{
			ret = recordstart(status.aktservice->channel, -1, 0, RECTIMESHIFT, 0, NULL);
			if(ret != 0)
			{
				status.timeshift = 0;
				servicecheckret(servicestart(status.aktservice->channel, NULL, NULL, 3), 0);
			}
			recordcheckret(NULL, ret, 6);
		}
	}
	else {
		if(status.timeshiftseek != 0) 
			status.timeshiftseek = 0;
		playerpausets();
	}
}

//flag 0: stop from rcstop
//flag 1: stop from servicestop
void timeshiftstop(int flag)
{
	int i = 0, ret = 0;
	char* file = NULL;
	struct service* snode = getservice(RECORDTIMESHIFT, flag);

	if(status.timeshiftseek != 0) { 
		status.timeshiftseek = 0;
		playerpausets();
		playercontinuets();
	}
	
	playerstopts(1, flag);

	if(snode != NULL)
	{
		file = ostrcat(file, snode->recname, 1, 0);
		snode->recendtime = 1;

		ret = textbox(_("Message"), _("Should Timeshift File deleted ?"), _("EXIT"), getrcconfigint("rcexit", NULL), _("OK"), getrcconfigint("rcok", NULL), NULL, 0, NULL, 0, 600, 400, 10, 0);

		if(ret == 0 || ret == 2) unlink(file);
		free(file); file = NULL;
	}

	if(flag == 0)
	{
		while(status.timeshift == 1)
		{
			usleep(100000);
			i++; if(i > 20) break;
		}
		servicecheckret(servicestart(status.aktservice->channel, NULL, NULL, 3), 0);
	}
}

void timeshiftplay()
{
	int ret = 1;
	struct service* snode = getservice(RECORDTIMESHIFT, 0);
	
	if(status.playing == 0)
	{
		if(snode != NULL) ret = playerstartts(snode->recname, 1);
		if(ret != 0)
		{
			textbox(_("Message"), _("Can't start timeshift play !"), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, NULL, 0, 600, 200, 0, 0);
		}
	}
	else {
		if(status.timeshiftseek != 0) {
			status.timeshiftseek = 0;
			playerpausets();
		}
		playercontinuets();
	}
}

void timeshiftscreen(struct stimerthread* self, struct service* servicenode)
{
	
	off64_t endoffile;
	off64_t currentpos;
	char* tmpstr = NULL;
	struct skin* framebuffer = getscreen("framebuffer");
	struct skin* timeshift = getscreen("timeshift");
	struct skin* seek = getscreennode(timeshift, "seek");
	struct skin* timeshiftbar = getscreennode(timeshift, "timeshiftbar");
	char* bg = NULL;
	struct service* snode = getservice(RECORDTIMESHIFT, 0);
	int fd = open(snode->recname, O_RDONLY | O_LARGEFILE);
	
	if(fd < 0)
	{
		err("timeshift fd not ok");
		return 1;
	}
	
	timeshiftbar->progresssize = 0;
	setnodeattr(timeshift, framebuffer);
	bg = savescreen(timeshift);
	
	while(status.timeshiftseek != 0) {
		endoffile = lseek64(fd , 0, SEEK_END);
		currentpos = lseek64(servicenode->recsrcfd, 0, SEEK_CUR);
		currentpos = currentpos - 5000000;
		timeshiftbar->progresssize = currentpos * 100 / endoffile;
		if(status.timeshiftseek > 100) {
			tmpstr = oitoa(status.timeshiftseek - 100);
			tmpstr = ostrcat(_(">> "), tmpstr, 0, 1);
			tmpstr = ostrcat(tmpstr,"x", 1, 0);
 			changetext(seek, tmpstr);
 			free(tmpstr); tmpstr = NULL;
 		}
 		if(status.timeshiftseek < 100 && status.timeshiftseek > -100) {
 			tmpstr = oitoa(status.timeshiftseek);
 			tmpstr = ostrcat(_("skip "), tmpstr, 0, 1);
 			tmpstr = ostrcat(tmpstr," sec", 1, 0);
 			changetext(seek, tmpstr);
 			free(tmpstr); tmpstr = NULL;
 		}
		drawscreen(timeshift, 0);
		sleep (1);
	}
	restorescreen(bg, timeshift);
	blitfb();
	close(fd);
}

void timeshiftseek(int sekunden)
{
	struct service* snode = getservice(RECORDPLAY, 0);
	
	if(status.timeshiftseek == 0)
		 addtimer(&timeshiftscreen, START, 10000, 1, (void*)snode, NULL, NULL);
	
	status.timeshiftseek = sekunden;
	if(snode != 0) {
		if(sekunden > 100) {
			if(sekunden >= 132) {
				status.timeshiftseek = 116;
				return;
			}
			status.timeshiftseek = sekunden;
			sekunden = sekunden - 100;
			recordffrwts(snode, sekunden);
		}	
		else {
			recordskipplay(snode, sekunden);
		}
	}
}


#endif
