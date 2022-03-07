#ifndef PANEL_GLOBAL_H
#define PANEL_GLOBAL_H

#define MAXTOKENS       256

char **str_split(char *string, char *delim) {
	char **tokens = NULL;
	char *working = NULL;
	char *token = NULL;
	int idx = 0;

	tokens  = malloc(sizeof(char *) * MAXTOKENS);
	if(tokens == NULL)
		return NULL;

	working = malloc(sizeof(char) * strlen(string) + 1);
	if(working == NULL)
		return NULL;

	 /* to make sure, copy string to a safe place */
	strcpy(working, string);
	for(idx = 0; idx < MAXTOKENS; idx++)
		tokens[idx] = NULL;

	token = strtok(working, delim);
	idx = 0;

	/* always keep the last entry NULL termindated */
	while((idx < (MAXTOKENS - 1)) && (token != NULL)) {
		tokens[idx] = malloc(sizeof(char) * strlen(token) + 1);
		if(tokens[idx] != NULL) {
			strcpy(tokens[idx], token);
			idx++;
			token = strtok(NULL, delim);
		}
	}

	free(working);
	return tokens;
}

#endif
