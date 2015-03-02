#include "../titan/struct.h"
#include "../titan/debug.h"
#include "../titan/header.h"

char pluginname[] = "Mbox Info";
char plugindesc[] = "Mbox Info V0.1    25/10/2011 AAF-tiTan";
char pluginpic[] = "%pluginpath%/mboxinfo/plugin.png";

int pluginaktiv = 0;
int pluginversion = PLUGINVERSION;

//wird beim laden ausgefuehrt
void init(void)
{
	char* tmpstr = NULL;

	tmpstr = createpluginpath("/mboxinfo/skin.xml", 0);
	readscreen(tmpstr, 194, 1);
	free(tmpstr); tmpstr = NULL;

	pluginaktiv = 1;
	debug(10, "Mbox Info Plugin loadet !!!");
}

//wird beim entladen ausgefuehrt
void deinit(void)
{
	delmarkedscreen(194);
	pluginaktiv = 0;
	debug(10, "Mbox Info Plugin removed !!!");
}

int show_info(char *titelname, char *filename)
{
	FILE* fd = NULL;
	char *fileline = NULL, *buf = NULL, *buf1 = NULL;
	int buf1size = 0, buf1oldsize = 0;

	fileline = malloc(MINMALLOC);
	if(fileline == NULL)
	{
		err("no memory");
		return 0;
	}

	fd =  fopen(filename, "r");
	if(fd == NULL)
	{
		textbox(_(titelname), _("File not found !!"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), '\0', 0, '\0', 0, 600, 200, 0, 0);
		free(fileline);
		return 0;
	}

	while(fgets(fileline, MINMALLOC, fd) != NULL)
	{
		buf = fileline;
		buf1oldsize = buf1size;
		buf1size += strlen(buf);
		buf1 = realloc(buf1, buf1size + 1);
		if(buf1 == NULL)
		{
			err("no memory");
			free(fileline);
			fclose(fd);
			return 0;
		}

		sprintf(buf1 + buf1oldsize, "%s", buf);
	}

	if(buf1 != NULL)
	{
		textbox(_(titelname), _(buf1), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), '\0', 0, '\0', 0, 900, 600, 0, 0);
	}
	else
	{
		textbox(_(titelname), (""), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), '\0', 0, '\0', 0, 600, 200, 0, 0);
	}

	fclose(fd);
	free(fileline);
	free(buf1);
	return 1;
}

//wird in der Pluginverwaltung bzw Menue ausfeguehrt
void start(void)
{
	int rcret = 0;
	struct skin* screen = getscreen("mboxinfo");
	struct skin* listbox = getscreennode(screen, "listbox");
	struct skin* tmp = NULL;
	char* tmpstr = NULL;

	delmarkedscreennodes(screen, 1);
	listbox->aktpage = -1;
	listbox->aktline = 1;
	changetitle(screen, "Mbox Info");

	tmp = addlistbox(screen, listbox, tmp, 1);
	if(tmp != NULL)
	{
		changetext(tmp, "ECM Info");
		changename(tmp, "ECM Info");
	}
	tmp = addlistbox(screen, listbox, tmp, 1);
	if(tmp != NULL)
	{
		changetext(tmp, "Share Online");
		changename(tmp, "Share Online");
	}
	tmp = addlistbox(screen, listbox, tmp, 1);
	if(tmp != NULL)
	{
		changetext(tmp, "Share Info");
		changename(tmp, "Share Info");
	}
	tmp = addlistbox(screen, listbox, tmp, 1);
	if(tmp != NULL)
	{
		changetext(tmp, "Mbox Version");
		changename(tmp, "Mbox Version");
	}
	tmp = addlistbox(screen, listbox, tmp, 1);
	if(tmp != NULL)
	{
		changetext(tmp, "Info");
		changename(tmp, "Info");
	}

	drawscreen(screen, 0, 0);
	addscreenrc(screen, listbox);

	while (1)
	{
		rcret = waitrc(screen, 0, 0);

		if(rcret == getrcconfigint("rcexit",NULL)) break;
		if(listbox->select != NULL && rcret == getrcconfigint("rcok",NULL))
		{
			tmpstr = ostrcat(listbox->select->name, NULL, 0, 0);
			debug(10, "[MBOX INFO] active menu = %s\n", tmpstr);
			clearscreen(screen);

			if(ostrcmp("ECM Info", listbox->select->name) == 0)
			{
				show_info(listbox->select->name, "/tmp/ecm.info");
			}
			else if(ostrcmp("Share Online", listbox->select->name) == 0)
			{
				show_info(listbox->select->name, "/tmp/share.onl");
			}
			else if(ostrcmp("Share Info", listbox->select->name) == 0)
			{
				show_info(listbox->select->name, "/tmp/share.info");
			}
			else if(ostrcmp("Mbox Version", listbox->select->name) == 0)
			{
				show_info(listbox->select->name, "/tmp/mbox.ver");
			}
			else if(ostrcmp("Info", listbox->select->name) == 0)
			{
				textbox(_(pluginname), _(plugindesc), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), '\0', 0, '\0', 0, 600, 200, 0, 0);
			}

			free(tmpstr); tmpstr = NULL;
			drawscreen(screen, 0, 0);
		}
	}

	delownerrc(screen);
	delmarkedscreennodes(screen, 1);
	clearscreen(screen);
}
