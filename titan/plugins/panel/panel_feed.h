#ifndef PANEL_FEED_H
#define PANEL_FEED_H

void screenpanel_feed()
{
	char* tmpstr = NULL, *line1 = NULL, *line2 = NULL;

	line2 = readsys(getconfig("feed", NULL), 2);
	line2 = string_replace("src/gz secret http://", "", line2, 1);
	if(line2 == NULL)
		line2 = ostrcat(line2, "11.22.33.44/svn/atemio", 1, 0);
	line2 = textinput("Feed", line2);
	
	if(line2 != NULL)
	{
		line1 = readsys(getconfig("feed", NULL), 1);
		if(line1 == NULL) line1 = ostrcat("#", NULL, 0, 0);

		if(line1[strlen(line1) - 1] == '\n')
			tmpstr = ostrcat(line1, "src/gz secret http://", 0, 0);
		else
			tmpstr = ostrcat(line1, "\nsrc/gz secret http://", 0, 0);

		if(tmpstr != NULL && tmpstr[0] == '\n')
			tmpstr = ostrcat("#", tmpstr, 0, 1);
		
		if(strlen(line2) == 0)
		{
			free(tmpstr);
			tmpstr = ostrcat(line1, NULL, 0, 0);
		}

		tmpstr = ostrcat(tmpstr, line2, 1, 0);
		writesys(getconfig("feed", NULL), tmpstr, 0);
	}

	free(tmpstr);
	free(line1);
	free(line2);
}

#endif
