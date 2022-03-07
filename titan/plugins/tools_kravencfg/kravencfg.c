#include "../titan/struct.h"
#include "../titan/debug.h"
#include "../titan/header.h"

#define MAX_KRAVENCFGS 20
#define MAX_VARIANTS 10

char pluginname[] = "kravencfg";
char plugindesc[] = "kravencfg";
char pluginpic[] = "%pluginpath%/kravencfg/plugin.png";

int pluginaktiv = 0;
int pluginversion = PLUGINVERSION;
//int pluginversion = 999999; // = nopluginversion

struct KravenCfg {
	char* Name;
	char* Variant[MAX_VARIANTS];
} KravenCfgs[MAX_KRAVENCFGS];

int check_kravencfg() {
	char *tmpstr = NULL;

	if(getconfig("skinpath", NULL) != NULL) {
		tmpstr = getconfig("skinpath", NULL);
		tmpstr = ostrstr(tmpstr, "KravenHD");
		if(tmpstr == NULL) {
			debug(10, "KravenHD not found!");
			return -1;
		} else {
			debug(10, "KravenHD found!");
			return 0;
		}
	} else return -1;
}

int filter_xml(const struct dirent *dir) {
	int len = strlen( dir->d_name );

	if(len < 4)
		return 0;

	return strcmp(dir->d_name+len-4,".xml") == 0;
}

void screen_kravencfg() {
	char *tmpstr = NULL, *skinpath = NULL, *snippetspath = NULL, *kravencfg_sh = NULL, *token = NULL, *saveptr = NULL;
	int i = 0, n = 0, x = 0, y = 0;
	struct dirent **filelist;

	skinpath = getconfig("skinpath", NULL);
	snippetspath = ostrcat(skinpath, "/snippets", 0, 0);
	kravencfg_sh = createpluginpath("/kravencfg/kravencfg.sh", 0);

	//debug(10, "********************************************************");
	//debug(10, "skinpath: %s", skinpath);
	//debug(10, "snippetspath: %s", snippetspath);
	//debug(10, "kravencfg_sh: %s", kravencfg_sh);
	//debug(10, "********************************************************");

	n = scandir(snippetspath, &filelist, filter_xml, alphasort);
	//debug(10, "scandir n = %d", n);

	if (n < 1) {
		perror("scandir");
		textbox(_("Message"), _("No skin snippets found!"), _("OK"), getrcconfigint("rcok", NULL), NULL, 0, NULL, 0, NULL, 0, 600, 200, 0, 0);
		return;
	} else {
		for (i = 0; i < n; i++) {
			tmpstr = strdup(filelist[i]->d_name);
			//debug(10, "tmpstr(%d/%d): %s", i, n-1, tmpstr);

			token = strtok_r(tmpstr, "-" , &saveptr);
			//debug(10, "token: %s", token);

			if(token != NULL) {
				if(i > 0) {
					//debug(10, "KravenCfgs[%d].Name: %s", x, KravenCfgs[x].Name);
					if(ostrcmp(KravenCfgs[x].Name, token) != 0) {
						x++;
						y = 0;
						KravenCfgs[x].Name = token;
						//debug(10, "KravenCfgs[%d].Name: %s", x, KravenCfgs[x].Name);
					}
				} else {
					KravenCfgs[x].Name = token;
					//debug(10, "KravenCfgs[%d].Name: %s", x, KravenCfgs[x].Name);
				}
			}

			while(token != NULL) {
				if(ostrcmp(token, "xml") != 0 && ostrcmp(KravenCfgs[x].Name, token) != 0) {
					//debug(10, "token: %s", token);
					KravenCfgs[x].Variant[y] = token;
					//debug(10, "KravenCfgs[%d].Variant[%d]: %s", x, y, KravenCfgs[x].Variant[y]);
					y++;
				}
				token=strtok_r(NULL, ".", &saveptr);
			}

			free(filelist[i]);
		}
		free(filelist);
	}

	//debug(10, "size of KravenCfgs[x].Variant[y]: %d", sizeof(KravenCfgs));
	//debug(10, "********************************************************");


	int rcret = 0;
	struct skin* skin_kravencfg = getscreen("kravencfg");
	struct skin* skin_listbox = getscreennode(skin_kravencfg, "listbox");
	struct skin* skin_png = getscreennode(skin_kravencfg, "png");
	struct skin* skin_dummy = getscreennode(skin_kravencfg, "dummy");
	struct skin* skin_tmp = NULL;

	delmarkedscreennodes(skin_kravencfg, 1);
	skin_listbox->aktpage = -1;
	skin_listbox->aktline = 1;

	for (x = 0; x < MAX_KRAVENCFGS; x++) {
		if(KravenCfgs[x].Name != NULL) {
			skin_tmp = addlistbox(skin_kravencfg, skin_listbox, skin_tmp, 1);
			if(skin_tmp != NULL) {
				skin_tmp->type = CHOICEBOX;
				changename(skin_tmp, KravenCfgs[x].Name);
				changetext(skin_tmp, KravenCfgs[x].Name);
				if(skin_dummy != status.skinerr) {
					skin_tmp->hspace = skin_dummy->hspace;
					skin_tmp->vspace = skin_dummy->vspace;
					skin_tmp->halign = skin_dummy->halign;
					skin_tmp->valign = skin_dummy->valign;
					skin_tmp->width = skin_dummy->width;
					skin_tmp->height = skin_dummy->height;
				}
			}
			for (y = 0; y < MAX_VARIANTS; y++) {
				if(KravenCfgs[x].Variant[y] != NULL) {
					addchoicebox(skin_tmp, KravenCfgs[x].Variant[y], _(KravenCfgs[x].Variant[y]));
					//debug(10, "KravenCfgs[%d].Variant[%d]: %s %s", x, y, KravenCfgs[x].Name, KravenCfgs[x].Variant[y]);
				}
			}
		setchoiceboxselection(skin_tmp, getskinconfig(KravenCfgs[x].Name, NULL));
		//debug(10, "getskinconfig (%s) KravenCfgs[%d].Name (%s): %s", skin_tmp->name, x, KravenCfgs[x].Name, getskinconfig(KravenCfgs[x].Name, NULL));
		}
	}

	changepic(skin_png, NULL);
	drawscreen(skin_kravencfg, 0, 0);
	addscreenrc(skin_kravencfg, skin_listbox);
	skin_tmp = skin_listbox->select;

	int count = 0;

	while(1) {
		addscreenrc(skin_kravencfg, skin_tmp);
		if(count > 0) rcret = waitrc(skin_kravencfg, 0, 0);
		skin_tmp = skin_listbox->select;

		if(count == 0 || rcret == getrcconfigint("rcup", NULL) || rcret == getrcconfigint("rcdown", NULL) || rcret == getrcconfigint("rcleft", NULL) || rcret == getrcconfigint("rcright", NULL)) {
			//debug(10, "skin_listbox->aktline = %d", skin_listbox->aktline);
			//debug(10, "skin_listbox->aktline = %s", skin_listbox->select->name);
			//debug(10, "skin_listbox->aktline = %s", skin_tmp->name);
			//debug(10, "skin_listbox->aktline = %s", skin_listbox->select->ret);
			//debug(10, "skin_listbox->aktline = %s", skin_tmp->ret);

			tmpstr = ostrcat(snippetspath, "/", 0, 0);
			tmpstr = ostrcat(tmpstr, skin_listbox->select->name, 0, 0);
			tmpstr = ostrcat(tmpstr, "-", 0, 0);
			tmpstr = ostrcat(tmpstr, skin_listbox->select->ret, 0, 0);
			tmpstr = ostrcat(tmpstr, ".png", 0, 0);

			//debug(10, "changepic: %s", tmpstr);

			changepic(skin_png, tmpstr);
			drawscreen(skin_kravencfg, 0, 0);
			tmpstr = NULL;

			count = 1;
			//debug(10, "********************************************************");
		}

		if(rcret == getrcconfigint("rcexit", NULL)) break;

		if(rcret == getrcconfigint("rcok", NULL)) {
			//debug(10, "********************************************************");
			while(skin_listbox != NULL) {
				if(skin_listbox->type == CHOICEBOX) {
					//debug(10, "%s-%s", skin_listbox->name, skin_listbox->ret);
					addskinconfigscreencheck(skin_listbox->name, skin_listbox, "0");

					tmpstr = ostrcat(kravencfg_sh, " ", 0, 0);
					tmpstr = ostrcat(tmpstr, skin_listbox->name, 0, 0);
					tmpstr = ostrcat(tmpstr, " ", 0, 0);
					tmpstr = ostrcat(tmpstr, skin_listbox->ret, 0, 0);
					tmpstr = ostrcat(tmpstr, " ", 0, 0);
					tmpstr = ostrcat(tmpstr, skinpath, 0, 0);

					//debug(10, "*********************** start cmd here *********************************");
					//debug(10, "%s", tmpstr);
					system(tmpstr);
					tmpstr = NULL;
					//debug(10, "************************************************************************");

				}
				skin_listbox = skin_listbox->next;
			}
			//debug(10, "********************************************************");

			writeskinconfigtmp();

			textbox(_("Message"), _("Titan will be restarted!"), _("OK"), getrcconfigint("rcok", NULL), NULL, 0, NULL, 0, NULL, 0, 600, 200, 0, 0);
			oshutdown(3, 0);

			break;
		}
	}

	delmarkedscreennodes(skin_kravencfg, 1);
	delownerrc(skin_kravencfg);
	clearscreen(skin_kravencfg);
}

//wird beim Laden ausgefuehrt
void init(void) {
	char *tmpstr = NULL;

	tmpstr = createpluginpath("/kravencfg/skin.xml", 0);
	readscreen(tmpstr, 211, 1);
	free(tmpstr); tmpstr = NULL;

	pluginaktiv = 1;
	debug(10, "kravencfg plugin loaded!");
}

//wird beim Entladen ausgefuehrt
void deinit(void) {
	delmarkedscreen(211);
	pluginaktiv = 0;
	debug(10, "kravencfg plugin removed!");
}

//wird in der Pluginverwaltung bzw Menue ausgefuehrt
void start(void) {
	if(check_kravencfg() == 0) {
		screen_kravencfg();
	} else {
		textbox(_("Message"), _("KravenHD skin isn't active!"), _("OK"), getrcconfigint("rcok", NULL), NULL, 0, NULL, 0, NULL, 0, 600, 200, 0, 0);
	}
}
