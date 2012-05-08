#ifndef PANEL_FEED_H
#define PANEL_FEED_H

void screenpanel_feed()
{
	char* tmpstr = NULL, *line = NULL, *lastline = NULL;
	char* pos = NULL;

	tmpstr = readsys(getconfig("feed", NULL), 3); //line3
	tmpstr = string_replace("src/gz secret http://", "", tmpstr, 1);
	
	if(tmpstr != NULL)
		pos = strchr(tmpstr, '/');
	if(pos != NULL)
		pos[0] = '\0';

	if(tmpstr == NULL || ostrcmp(tmpstr, "") == 0 || ostrcmp(tmpstr, "\n") == 0)
		tmpstr = ostrcat(tmpstr, "", 1, 0);

	lastline = textinput(_("Feed"), tmpstr);
	
	if(lastline != NULL)
	{
		free(tmpstr); tmpstr = NULL;
		tmpstr = ostrcat(tmpstr, lastline, 1, 0);
		free(lastline); lastline = tmpstr;

		tmpstr = readsys(getconfig("feed", NULL), 1); //line1
		if(tmpstr == NULL || (tmpstr != NULL && strlen(tmpstr) == 0)) 
			line = ostrcat(line, "#", 1, 0);
		else
			line = ostrcat(line, tmpstr, 1, 0);
		free(tmpstr); tmpstr = NULL;

		if(line[strlen(line) - 1] != '\n')
			line = ostrcat(line, "\n", 1, 0);
		
		tmpstr = readsys(getconfig("feed", NULL), 2); //line2
		if(tmpstr == NULL || (tmpstr != NULL && strlen(tmpstr) == 0)) 
			line = ostrcat(line, "#\n", 1, 0);
		else
			line = ostrcat(line, tmpstr, 1, 0);
		free(tmpstr); tmpstr = NULL;

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
		tmpstr = ostrcat(tmpstr, ".mynonpublic.com/svn/ipk/sh4/titan", 1, 0);
		writesys(getconfig("feed", NULL), tmpstr, 0);
	}

	free(tmpstr);
	free(line);
	free(lastline);
}

#endif
