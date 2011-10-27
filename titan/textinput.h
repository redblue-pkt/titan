#ifndef TEXTINPUT_H
#define TEXTINPUT_H

char* textinput(char* text)
{
	debug(1000, "in");
	int rcret = -1;
	struct skin* textinput = getscreen("textinput");
	struct skin* input = getscreennode(textinput, "input");
	char* ret = NULL;

	changeinput(input, text);

	drawscreen(textinput, 0);
	addscreenrc(textinput, input);

	while(1)
	{
		rcret = waitrc(textinput, 0, 0);
		if(rcret == getrcconfigint("rcexit", NULL)) break;
		if(rcret == getrcconfigint("rcok", NULL))
		{
			ret = ostrcat(input->input, NULL, 0, 0);
			break;
		}
	}

	delownerrc(textinput);
	clearscreen(textinput);
	drawscreen(skin, 0);

	debug(1000, "out");

	return ret;
}

#endif
