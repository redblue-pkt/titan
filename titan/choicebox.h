#ifndef CHOICEBOX_H
#define CHOICEBOX_H

int addchoicebox(struct skin* choicebox, char* value, char* text)
{
	debug(1000, "in");
	char* tmp = NULL;

	if(choicebox == NULL || value == NULL || text == NULL || strlen(value) == 0)
	{
		debug(1000, "out -> NULL detect");
		return 1;
	}

	if(choicebox->input == NULL || choicebox->choiceboxvalue == NULL)
	{
		changeinput(choicebox, NULL);
		changechoiceboxvalue(choicebox, NULL);
		tmp = ostrcat(tmp, text, 1, 0);
		changeinput(choicebox, tmp);
		free(tmp); tmp = NULL;
		tmp = ostrcat(tmp, value, 1, 0);
		changechoiceboxvalue(choicebox, tmp);
	}
	else
	{
		//check once
		if(ostrcmp(choicebox->choiceboxvalue, value) == 0) return 0;
		//check first
		tmp = ostrcat(value, "\n", 0, 0);
		if(ostrstr(choicebox->choiceboxvalue, tmp) == choicebox->choiceboxvalue)
		{
			free(tmp); tmp = NULL;
			return 0;
		}
		free(tmp); tmp = NULL;
		//check mitddle
		tmp = ostrcat("\n", value, 0, 0);
		tmp = ostrcat(tmp, "\n", 1, 0);
		if(ostrstr(choicebox->choiceboxvalue, tmp) != NULL)
		{
			free(tmp); tmp = NULL;
			return 0;
		}
		free(tmp); tmp = NULL;
		//check end
		tmp = ostrcat("\n", value, 0, 0);
		char* c = ostrstr(choicebox->choiceboxvalue, tmp);
		if(c != NULL && choicebox->choiceboxvalue + strlen(choicebox->choiceboxvalue) == c + strlen(tmp))
		{
			free(tmp); tmp = NULL;
			return 0;
		}
		free(tmp); tmp = NULL;

		tmp = ostrcat(choicebox->choiceboxvalue, "\n", 0, 0);
		tmp = ostrcat(tmp, value, 1, 0);
		changechoiceboxvalue(choicebox, tmp);
		free(tmp); tmp = NULL;

		tmp = ostrcat(choicebox->input, "\n", 0, 0);
		tmp = ostrcat(tmp, text, 1, 0);
		changeinput(choicebox, tmp);
	}
	
	free(tmp);

	return 0;
	debug(1000, "out");
}

int setchoiceboxselection(struct skin* choicebox, char* value)
{
	debug(1000, "in");
	int treffer = 0;
	char* pos = NULL, *epos = NULL, *end = NULL;

	if(choicebox == NULL || choicebox->input == NULL)
	{
		debug(1000, "out -> NULL detect");
		return 1;
	}

	choicebox->aktpage = 0;

	if(value == NULL)
	{
		debug(1000, "out -> NULL detect");
		return 1;
	}

	if(choicebox->choiceboxvalue != NULL)
		pos = choicebox->choiceboxvalue;
	else
		pos = choicebox->input;

	if(pos != NULL) end = pos + strlen(pos) + 1;

	while(pos != NULL)
	{
		choicebox->aktpage++;

		if(strncmp(pos, value, strlen(value)) == 0)
		{
			epos = pos + strlen(value);
			if(epos > end) break;
			if(*epos == '\n' || *epos == '\0')
			{
				changeret(choicebox, value);
				treffer = 1;
				break;
			}
		}

		pos = strchr(pos, '\n');
		if(pos != NULL) pos++;
	}

	if(treffer == 0)
	{
		if(choicebox->choiceboxvalue != NULL)
			pos = choicebox->choiceboxvalue;
		else
			pos = choicebox->input;

		if(pos != NULL)
		{
			epos = strchr(pos, '\n');
			if(epos == NULL)
				changeret(choicebox, pos);
			else
			{
				char c = 0;
				c = *epos;
				*epos = '\0';
				changeret(choicebox, pos);
				*epos = c;
			}
			choicebox->aktpage = 1;
		}
	}

	debug(1000, "out");
	return 0;
}

#endif
