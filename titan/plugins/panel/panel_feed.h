#ifndef PANEL_FEED_H
#define PANEL_FEED_H

void screenpanel_feed()
{
	char* tmpstr = NULL, *line = NULL, *lastline = NULL;

	lastline = readsys(getconfig("feed", NULL), 3); //line3
	lastline = string_replace("src/gz secret http://", "", lastline, 1);

	if(lastline == NULL || ostrcmp(lastline, "") == 0 || ostrcmp(lastline, "\n") == 0)
		lastline = ostrcat(lastline, "00.00.00.00/svn/atemio", 1, 0);

	lastline = textinput(_("Feed"), lastline);
	
	if(lastline != NULL)
	{
		tmpstr = readsys(getconfig("feed", NULL), 1); //line1
		if(tmpstr == NULL || (tmpstr != NULL && strlen(tmpstr) == 0)) 
			line = ostrcat(line, "#", 1, 0);
		else
			line = ostrcat(line, tmpstr, 1, 0);
		free(tmpstr); tmpstr = NULL;

		if(line[strlen(line) - 1] != '\n')
			line = ostrcat(line, "\n", 1, 0);
printf("%s\n", line);
		
		tmpstr = readsys(getconfig("feed", NULL), 2); //line2
		if(tmpstr == NULL || (tmpstr != NULL && strlen(tmpstr) == 0)) 
			line = ostrcat(line, "#\n", 1, 0);
		else
			line = ostrcat(line, tmpstr, 1, 0);
		free(tmpstr); tmpstr = NULL;

printf("%s\n", line);

		if(line[strlen(line) - 1] == '\n')
			tmpstr = ostrcat(line, "src/gz secret http://", 0, 0);
		else
			tmpstr = ostrcat(line, "\nsrc/gz secret http://", 0, 0);
		
		if(strlen(lastline) == 0)
		{
			free(tmpstr);
			tmpstr = ostrcat(line, NULL, 0, 0);
		}

		tmpstr = ostrcat(tmpstr, lastline, 1, 0);
		writesys(getconfig("feed", NULL), tmpstr, 0);
	}

	free(tmpstr);
	free(line);
	free(lastline);
}

#endif
