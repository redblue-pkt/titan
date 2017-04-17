/*
 * Container handling for all stream's handled by ffmpeg
 * konfetti 2010; based on code from crow
 *
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 *
 */
 
#define MINMALLOC 4096
#include <ctype.h>

int file_exist(char* filename)
{
	if(access(filename, F_OK) == 0)
		return 1;
	else
		return 0;
}

char* strstrip(char *text)
{
	char* tmpstr = text;

	if(text == NULL) return NULL;
	int len = strlen(text);

	while(isspace(tmpstr[len - 1])) tmpstr[--len] = '\0';
	while(*tmpstr && isspace(*tmpstr)) ++tmpstr, --len;

	if(text != tmpstr) memmove(text, tmpstr, len + 1);

	return text;
}

char* string_strip_whitechars(char *text)
{
	char *p1 = text, *p2 = text;

	if(text == NULL)
		return NULL;

	while(*p1 != '\0')
	{
		if(*p1 == ' ' && *(p1 + 1) == ' ')
			++p1;
		else
			*p2++ = *p1++;
	}
	*p2 = '\0';

	return text;
}

char* readfiletomem(const char* filename, int flag)
{
	FILE *fd = NULL;
	char *fileline = NULL, *buf = NULL, *tmpbuf = NULL;
	int bufsize = 0, bufoldsize = 0;

	fileline = malloc(MINMALLOC);
	if(fileline == NULL)
	{
//		err("no mem");
		return NULL;
	}

	fd = fopen(filename, "r");
	if(fd == NULL)
	{
//		perr("can't open %s", filename);
		free(fileline);
		return NULL;
	}

	while(fgets(fileline, MINMALLOC, fd) != NULL)
	{
		if(flag == 1)
			if(fileline[0] == '#' || fileline[0] == '\n')
				continue;

		bufoldsize = bufsize;
		bufsize += strlen(fileline);
		tmpbuf = buf;	buf = realloc(buf, bufsize + 1);
		if(buf == NULL)
		{
//			err("no mem");
			free(fileline);
			free(tmpbuf);
			fclose(fd);
			return NULL;
		}

		sprintf(buf + bufoldsize, "%s", fileline);
	}

	free(fileline);
	fclose(fd);
	return buf;
}

struct splitstr
{
	char* part;
};

struct splitstr* strsplit(char *str, char *tok, int* count)
{
	char *tmpstr = NULL;
	struct splitstr *array = NULL, *tmparray = NULL;
	*count = 0;

	if(str == NULL || tok == NULL)
		return NULL;

	tmpstr = strtok(str, tok);
	while(tmpstr != NULL)
	{
		*count = *count + 1;
		tmparray = array; array = (struct splitstr*)realloc(array, sizeof(struct splitstr*) * (*count));
		if(array == NULL)
		{
//			err("no mem");
			free(tmparray);
			return NULL;
		}
		
		(&array[(*count) - 1])->part = tmpstr;
		tmpstr = strtok(NULL, tok);
	}

	return array;
}

char* ostrcat(char* value1, char* value2, int free1, int free2)
{
	int len = 0, len1 = 0, len2 = 0;
	char* buf = NULL;

	if(value1 == NULL && value2 == NULL) return NULL;

	if(value1 != NULL) len1 = strlen(value1);
	if(value2 != NULL) len2 = strlen(value2);

	len = len1 + len2 + 1;

	if(free1 == 1)
		buf = realloc(value1, len);
	else
		buf = malloc(len);
	if(buf == NULL)
	{
		if(free1 == 1) free(value1);
		if(free2 == 1) free(value2);
		return NULL;
	}

	if(free1 == 0 && len1 > 0) memcpy(buf, value1, len1);
	if(len2 > 0) memcpy(buf + len1, value2, len2);
	buf[len - 1] = '\0';

	if(free2 == 1) free(value2);

	//helpfull for memleak detect
	//if(buf != NULL && strlen(buf) == 0x0b - 0x01)
	//	printf("******** memleak string (%s) (%p) ********\n", buf, buf);

	return buf;
}

char* ostrstr(char* str, char* search)
{
	char* ret = NULL;

	if(str == NULL || search == NULL) return NULL;
	ret = strstr(str, search);

	return ret;
}

char* string_replace(char *search, char *replace, char *string, int free1)
{
	char* searchpos = NULL;
	char* tmpstr = NULL;

	if(string == NULL || search == NULL)
	{
		tmpstr = ostrcat(tmpstr, string, 1, 0);
		if(free1 == 1) free(string);
		return tmpstr;
	}

	searchpos = ostrstr(string, search);

	if(searchpos == NULL)
	{
		tmpstr = ostrcat(tmpstr, string, 1, 0);
		if(free1 == 1) free(string);
		return tmpstr;
	}

	tmpstr = strndup(string, searchpos - string);
	if(replace != NULL)
		tmpstr = ostrcat(tmpstr, replace, 1, 0);
	tmpstr = ostrcat(tmpstr, string + (searchpos - string) + strlen(search), 1, 0);

	if(free1 == 1) free(string);

	return tmpstr;
}

char* stringreplacechar(char *str, char c1, char c2)
{
	char *p1 = str;

	if(str == NULL) return NULL;

	while(*p1 != '\0')
	{
		if(*p1 == c1) *p1 = c2;
		p1++;
	}

	return str;
}

char* string_replace_all(char *search, char *replace, char *string, int free1)
{
	char* tmpstr = NULL;
	char* searchpos = NULL;

	if(string == NULL || search == NULL)
	{
		tmpstr = ostrcat(tmpstr, string, 1, 0);
		if(free1 == 1) free(string);
		return tmpstr;
	}

	searchpos = strstr(string, search);
	if(searchpos == NULL)
	{
		tmpstr = ostrcat(tmpstr, string, 1, 0);
		if(free1 == 1) free(string);
		return tmpstr;
	}

	int count = 0;
	int stringlen = strlen(string);
	int searchlen = strlen(search);
	int replacelen = strlen(replace);

	while(searchpos != NULL)
	{
		count++;
		searchpos = strstr(searchpos + searchlen, search);
	}

	int len = stringlen - (searchlen * count) + (replacelen * count);
	tmpstr = calloc(1, len + 1);
	if(tmpstr == NULL)
	{
//		err("no mem");
		tmpstr = ostrcat(tmpstr, string, 1, 0);
		if(free1 == 1) free(string);
		return tmpstr;
	}

	len = 0;	
	char* str = string;
	char* tmp = tmpstr;
	searchpos = strstr(str, search);
	while(searchpos != NULL)
	{
		len = searchpos - str;
		memcpy(tmp, str, len);
		memcpy(tmp + len, replace, replacelen);
		tmp += len + replacelen;
		str += len + searchlen;
		searchpos = strstr(str, search);
	}
	memcpy(tmp, str, strlen(str));

	if(free1 == 1) free(string);

	return tmpstr;
}

int ostrcmp(char* value1, char* value2)
{
	int ret = 1;

	if(value1 != NULL && value2 != NULL)
		ret = strcmp(value1, value2);

	return ret;
}
