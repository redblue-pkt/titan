#ifndef RECORDPATH_H
#define RECORDPATH_H

void screenrecordpath()
{
	int rcret = 0;
	struct skin* recordpath = getscreen("recordpath");
	struct skin* listbox = getscreennode(recordpath, "listbox");
	struct skin* moviepath = getscreennode(recordpath, "moviepath");
	struct skin* timerpath = getscreennode(recordpath, "timerpath");
	struct skin* recpath = getscreennode(recordpath, "recpath");
	struct skin* timeshiftpath = getscreennode(recordpath, "timeshiftpath");
	struct skin* streampath = getscreennode(recordpath, "streampath");
	char* ret = NULL;

	changeinput(moviepath, getconfig("rec_moviepath", NULL));
	changeinput(timerpath, getconfig("rec_timerpath", NULL));
	changeinput(recpath, getconfig("rec_path", NULL));
	changeinput(timeshiftpath, getconfig("rec_timeshiftpath", NULL));
	changeinput(streampath, getconfig("rec_streampath", NULL));

	drawscreen(recordpath, 0, 0);
	addscreenrc(recordpath, listbox);

	while(1)
	{
		rcret = waitrc(recordpath, 0, 0);

		if(rcret == getrcconfigint("rcexit", NULL)) break;
		if(rcret == getrcconfigint("rcok", NULL))
		{
			addconfigscreen("rec_moviepath", moviepath);
			addconfigscreen("rec_timerpath", timerpath);
			addconfigscreen("rec_path", recpath);
			addconfigscreen("rec_timeshiftpath", timeshiftpath);
			addconfigscreen("rec_streampath", streampath);
			break;
		}
		if(rcret == getrcconfigint("rcred", NULL))
		{
			if(listbox->select != NULL)
			{
				clearscreen(recordpath);
				ret = screendir(listbox->select->ret, "", NULL, NULL, NULL, NULL, 0, "SELECT", 0, NULL, 0, NULL, 0, 700, 0, 650, 0, 0);
				if(ret != NULL)
					changeinput(listbox->select, ret);
				free(ret);

				drawscreen(recordpath, 0, 0);
			}
		}
	}

	delownerrc(recordpath);
	clearscreen(recordpath);
}

#endif
