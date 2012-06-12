#ifndef HELP_H
#define HELP_H

void screenhelp(struct skin* node)
{
	int rcret = 0;
	struct skin* help = getscreen("helpbox");
	struct skin* textbox = getscreennode(help, "textbox");
	char* helppath = NULL, *tmpstr = NULL, *lang = NULL, *buf = NULL;

	if(node == NULL || node->name == NULL) return;

	helppath = getconfig("helppath", NULL);
	if(helppath == NULL) return;

	//get language
	lang = ostrcat(getconfig("lang", NULL), NULL, 0, 0);
	lang = string_replace("po/", "", lang, 1);

	//create full filename
	tmpstr = ostrcat(helppath, "/", 0, 0);
	tmpstr = ostrcat(tmpstr, lang, 1, 1);
	tmpstr = ostrcat(tmpstr, "/", 1, 0);
	tmpstr = ostrcat(tmpstr, node->name, 1, 0);

	buf = readfiletomem(tmpstr, 0);
	if(buf == NULL)
	{
		free(tmpstr); tmpstr = NULL;
		return;
	}
	changetext(textbox, buf);

	free(tmpstr); tmpstr = NULL;
	free(buf); buf = NULL;

	drawscreen(help, 0, 0);
	addscreenrc(help, textbox);
	delrc(getrcconfigint("rchelp", NULL), help, textbox);

	while(1)
	{
		rcret = waitrc(help, 0, 0);

		if(rcret == getrcconfigint("rcexit", NULL)) break;
	}

	changetext(textbox, NULL);
	delownerrc(help);
	clearscreen(help);
}

#endif
