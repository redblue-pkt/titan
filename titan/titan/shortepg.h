#ifndef SHORTEPG_H
#define SHORTEPG_H

void screenshortepg(struct channel* chnode, struct epg* epgnode, int flag)
{
	int rcret = 0, i = 0;
	struct skin* shortepg = getscreen("shortepg");
	struct skin* grid = getscreennode(shortepg, "grid");
	struct skin* pos1 = getscreennode(shortepg, "pos1");
	struct skin* pos2 = getscreennode(shortepg, "pos2");
	struct skin* pos3 = getscreennode(shortepg, "pos3");
	struct skin* pos4 = getscreennode(shortepg, "pos4");
	struct skin* pos5 = getscreennode(shortepg, "pos5");
	char* tmpstr = NULL, *buf = NULL;
	struct tm *loctime = NULL;

	if(chnode == NULL) chnode = status.aktservice->channel;
	if(chnode == NULL) return;
	if(epgnode == NULL) epgnode = getepgakt(chnode);
	if(epgnode == NULL) epgnode = getepgnext(chnode);

	buf = malloc(100);
	if(buf == NULL)
	{
		free(loctime);
		err("no memory");
		return;
	}

	while(epgnode != NULL)
	{
		loctime = olocaltime(&epgnode->starttime);
		ostrftime(buf, MINMALLOC, "%d.%m. (%a) %H:%M", loctime);
		tmpstr = ostrcat(tmpstr, buf, 1, 0);
		tmpstr = translate_time(tmpstr, 1);
		free(loctime); loctime = NULL;
		tmpstr = ostrcat(tmpstr, "\n", 1, 0);

		loctime = olocaltime(&epgnode->endtime);
		ostrftime(buf, MINMALLOC, "%d.%m. (%a) %H:%M", loctime);
		tmpstr = ostrcat(tmpstr, buf, 1, 0);
		tmpstr = translate_time(tmpstr, 1);

		free(loctime); loctime = NULL;
		tmpstr = ostrcat(tmpstr, "\n\n", 1, 0);

		tmpstr = ostrcat(tmpstr, epgnode->title, 1, 0);
		if(epgnode->subtitle != NULL && ostrcmp(epgnode->title, epgnode->subtitle) != 0)
		{
			tmpstr = ostrcat(tmpstr, "\n", 1, 0);
			tmpstr = ostrcat(tmpstr, epgnode->subtitle, 1, 0);
		}
		tmpstr = ostrcat(tmpstr, "\n\n", 1, 0);
		tmpstr = ostrcat(tmpstr, epgdescunzip(epgnode), 1, 1);

		if(i == 0) changetext(pos1, tmpstr);
		if(i == 1) changetext(pos2, tmpstr);
		if(i == 2) changetext(pos3, tmpstr);
		if(i == 3) changetext(pos4, tmpstr);
		if(i == 4) changetext(pos5, tmpstr);
		free(tmpstr); tmpstr = NULL;

		i++;
		if(i > 4) break;

		epgnode = epgnode->next;
	}

	free(buf); buf = NULL;
	drawscreen(shortepg, 0, 0);
	addscreenrc(shortepg, grid);

	while(1)
	{
		rcret = waitrc(shortepg, 0, 0);
		if(rcret == getrcconfigint("rcexit", NULL)) break;
		if(rcret == getrcconfigint("rcok", NULL)) break;
	}

	changetext(pos1, NULL);
	changetext(pos2, NULL);
	changetext(pos3, NULL);
	changetext(pos4, NULL);
	changetext(pos5, NULL);
	delownerrc(shortepg);
	clearscreen(shortepg);
}

#endif
