#include "../titan/struct.h"
#include "../titan/debug.h"
#include "../titan/header.h"

char pluginname[] = "Sudoku";
char plugindesc[] = "Sudoku Game";
char pluginpic[] = "%pluginpath%/sudoku/plugin.png";
int pluginaktiv = 0;
int pluginversion = 999999;

char* g_sudoku_puzzle[81];
char* g_sudoku_solved[81];
char* g_SudokuCorePath = NULL;
int g_initialized = 0;
int g_checks = 0;

int gColInput = 16777215;
int gColPuzzle = 1;
int gColOK = 3193392;//65280;
int gColNG = 12067104;//16711680;
int gColSelect = 7041655;

void sudokunew(struct skin* sudoku_grid, struct skin* sudoku_checks, struct skin* sudoku_level, int iLevel) {
	struct skin* node = sudoku_grid;
	struct skin* checks = sudoku_checks;
	struct skin* level = sudoku_level;

	int count=0;
	int i=0;
	char* tmpstr = NULL;
	char* tmpstr1 = NULL;

	FILE *ptr_file;
	char buf[10];

	g_checks = 0;
	if(checks != NULL) {
		tmpstr1 = ostrcat(oitoa(g_checks), " Checks", 0, 0);
		changetext(checks, tmpstr1);
	}

	if(level != NULL) {
		if ( iLevel == 1 ) {
			changetext(level, "Level: Hard");
		} else {
			changetext(level, "Level: Easy");
		}
	}

	if ( iLevel == 1 ) {
		tmpstr = ostrcat(g_SudokuCorePath, " --create-hard >/tmp/.sudoku_puzzle", 0, 0);
	} else {
		tmpstr = ostrcat(g_SudokuCorePath, " --create-easy >/tmp/.sudoku_puzzle", 0, 0);
	}
	//debug(10, "(%s)",tmpstr);
	system(tmpstr);
	free(tmpstr); tmpstr = NULL;

	tmpstr = ostrcat(g_SudokuCorePath, " </tmp/.sudoku_puzzle >/tmp/.sudoku_solved", 0, 0);
	//debug(10, "%s",tmpstr);
	system(tmpstr);
	free(tmpstr); tmpstr = NULL;

	ptr_file =fopen("/tmp/.sudoku_puzzle","r");
	if (!ptr_file) {
		debug(10, "shit");
		return;
	}
	i = 0;
	while ( (fgets(buf,2, ptr_file)!=NULL) && (i < 81) ) {
		g_sudoku_puzzle[i] = ostrcat(buf, NULL, 0, 0);
		//debug(10, "sudoku_puzzle %d: %s", i, g_sudoku_puzzle[i]);
		i++;
	}
	fclose(ptr_file);
	system("rm /tmp/.sudoku_puzzle");

	ptr_file =fopen("/tmp/.sudoku_solved","r");
	if (!ptr_file) {
		debug(10, "shit");
		return;
	}
	i = 0;
	while ( (fgets(buf,2, ptr_file)!=NULL) && (i < 81) ) {
		g_sudoku_solved[i] = ostrcat(buf, NULL, 0, 0);
		//debug(10, "sudoku_solved %d: %s", i, g_sudoku_solved[i]);
		i++;
	}
	fclose(ptr_file);
	system("rm /tmp/.sudoku_solved");

//	for(i = 0; i < 81; i++) {
//		debug(10, "%d: %s - %s", i+1, g_sudoku_puzzle[i], g_sudoku_solved[i]);
//	}

	count = 0;
	while(node != NULL) {
		if(ostrcmp(node->parent, "grid") == 0) {
			if ( ostrcmp(g_sudoku_puzzle[count], "-") != NULL ) {
				changetext(node, g_sudoku_puzzle[count]);
				node->fontcol = gColPuzzle;
			} else {
				changetext(node, "");
				node->fontcol = gColInput;
			}
			count++;
		}
		node = node->next;
	}

	free(tmpstr); tmpstr = NULL;
	free(tmpstr1); tmpstr1 = NULL;
	g_initialized = 1;
}

void sudokucheck(struct skin* sudoku_grid, struct skin* sudoku_checks) {
	struct skin* node = sudoku_grid;
	struct skin* checks = sudoku_checks;
	int count=0;
	char* tmpstr = NULL;
	char* tmpstr1 = NULL;

	g_checks++;
	if(checks != NULL) {
		tmpstr1 = ostrcat(oitoa(g_checks), " Checks", 0, 0);
		changetext(checks, tmpstr1);
	}

	count = 0;
	while(node != NULL) {
		if(ostrcmp(node->parent, "grid") == 0) {
			if( (ostrcmp(node->text, "") != 0) && (node->fontcol != gColPuzzle) ) {
				tmpstr = oitoa(node->fontcol);
				//debug(10, "%d: %s (%s)", count+1, node->text, tmpstr);
				if( ostrcmp(g_sudoku_solved[count], node->text) == 0 ) {
					node->fontcol = gColOK;
				} else {
					node->fontcol = gColNG;
				}
			} else if (ostrcmp(node->text, "") == 0) {
				node->fontcol = gColInput;
			}
			count++;
		}
		node = node->next;
	}

	free(tmpstr); tmpstr = NULL;
	free(tmpstr1); tmpstr1 = NULL;
}

void screensudoku_settings() {
	int rcret = 0;
	struct skin* sudoku_settings = getscreen("sudoku_settings");
	struct skin* set_listbox = getscreennode(sudoku_settings, "listbox");
	struct skin* set_posx = getscreennode(sudoku_settings, "posx");
	struct skin* tmp = NULL;

	struct clist *myconfig[LISTHASHSIZE] = {NULL};
	char* sudokucfg = NULL;

	sudokucfg = createpluginpath("/sudoku/sudoku.cfg", 0);
	readconfig(sudokucfg, myconfig);
	//debug(10, "config: (%s)", sudokucfg);
	//debug(10, "config: (%s)", getlist(myconfig, "xpos", NULL));

	addchoicebox(set_posx, "left", _("left"));
	addchoicebox(set_posx, "center", _("center"));
	addchoicebox(set_posx, "right", _("right"));
	setchoiceboxselection(set_posx, getlist(myconfig, "xpos", NULL));

	if ( ostrcmp(getlist(myconfig, "xpos", NULL), "right")  == 0 ) {
		sudoku_settings->posx = RIGHT;
	} else if ( ostrcmp(getlist(myconfig, "xpos", NULL), "center")  == 0 ) {
		sudoku_settings->posx = CENTER;
	} else {
		sudoku_settings->posx = LEFT;
	}

	drawscreen(sudoku_settings, 0, 0);
	addscreenrc(sudoku_settings, set_listbox);

	tmp = set_listbox->select;
	while(1) {
		addscreenrc(sudoku_settings, tmp);
		rcret = waitrc(sudoku_settings, 0, 0);
		tmp = set_listbox->select;

		if ( rcret == getrcconfigint("rcexit", NULL) || rcret == getrcconfigint("rcmenu", NULL) ) break;

		if (rcret == getrcconfigint("rcok", NULL) ) {
			//debug(10, "posx new: (%s)", set_posx->ret);
			addlist(myconfig, "xpos", set_posx->ret);
			writelist(myconfig, sudokucfg);
			break;
		}
	}

	freelist(myconfig);
	free(sudokucfg); sudokucfg = NULL;

	delownerrc(sudoku_settings);
	clearscreen(sudoku_settings);
}

void screensudoku_help() {
	int rcret = 0;
	struct skin* sudoku_help = getscreen("sudoku_help");

	struct clist *myconfig[LISTHASHSIZE] = {NULL};
	char* sudokucfg = NULL;

	sudokucfg = createpluginpath("/sudoku/sudoku.cfg", 0);
	readconfig(sudokucfg, myconfig);

	if ( ostrcmp(getlist(myconfig, "xpos", NULL), "right")  == 0 ) {
		sudoku_help->posx = RIGHT;
	} else if ( ostrcmp(getlist(myconfig, "xpos", NULL), "center")  == 0 ) {
		sudoku_help->posx = CENTER;
	} else {
		sudoku_help->posx = LEFT;
	}

	freelist(myconfig);
	free(sudokucfg); sudokucfg = NULL;

	drawscreen(sudoku_help, 0, 0);

	while(1) {
		rcret = waitrc(sudoku_help, 0, 0);
		if ( rcret == getrcconfigint("rcexit", NULL) || rcret == getrcconfigint("rchelp", NULL) ) break;
	}

	delownerrc(sudoku_help);
	clearscreen(sudoku_help);
}

void screensudoku() {
	int rcret = 0, iLevel = 0;
	struct skin* sudoku = getscreen("sudoku");
	struct skin* grid = getscreennode(sudoku, "grid");
	struct skin* level = getscreennode(sudoku, "level");
	struct skin* checks = getscreennode(sudoku, "checks");
	struct skin* wait = getscreen("wait");


	struct clist *myconfig[LISTHASHSIZE] = {NULL};
	char* sudokucfg = NULL;

	sudokucfg = createpluginpath("/sudoku/sudoku.cfg", 0);
	readconfig(sudokucfg, myconfig);
	//debug(10, "config: (%s)", sudokucfg);
	//debug(10, "config: (%s)", getlist(myconfig, "xpos", NULL));

	if ( ostrcmp(getlist(myconfig, "xpos", NULL), "right")  == 0 ) {
		sudoku->posx = RIGHT;
		wait->posx = RIGHT;
	} else if ( ostrcmp(getlist(myconfig, "xpos", NULL), "center")  == 0 ) {
		sudoku->posx = CENTER;
		wait->posx = CENTER;
	} else {
		sudoku->posx = LEFT;
		wait->posx = LEFT;
	}

	freelist(myconfig);
	free(sudokucfg); sudokucfg = NULL;

	drawscreen(sudoku, 0, 0);
	if ( g_initialized == 0 ) {
		changetext(level, "");
		changetext(checks, "");
		drawscreen(wait, 0, 0);
		sudokunew(grid, checks, level, iLevel);
		clearscreen(wait);
		drawscreen(sudoku, 0, 0);
	}
	addscreenrc(sudoku, grid);

	while(1) {
		rcret = waitrc(sudoku, 0, 0);

		if(rcret == getrcconfigint("rchelp", NULL)) {
			screensudoku_help();
			drawscreen(sudoku, 0, 0);
			continue;
		}

		if(rcret == getrcconfigint("rcmenu", NULL)) {
			screensudoku_settings();

			sudokucfg = createpluginpath("/sudoku/sudoku.cfg", 0);
			readconfig(sudokucfg, myconfig);
			//debug(10, "config: (%s)", sudokucfg);
			//debug(10, "config: (%s)", getlist(myconfig, "xpos", NULL));

			if ( ostrcmp(getlist(myconfig, "xpos", NULL), "right")  == 0 ) {
				sudoku->posx = RIGHT;
				wait->posx = RIGHT;
			} else if ( ostrcmp(getlist(myconfig, "xpos", NULL), "center")  == 0 ) {
				sudoku->posx = CENTER;
				wait->posx = CENTER;
			} else {
				sudoku->posx = LEFT;
				wait->posx = LEFT;
			}

			freelist(myconfig);
			free(sudokucfg); sudokucfg = NULL;

			drawscreen(sudoku, 0, 0);
			continue;
		}

		if(rcret == getrcconfigint("rcexit", NULL)) break;

		if(rcret == getrcconfigint("rcok", NULL)) {
			sudokucheck(grid, checks);
			drawscreen(sudoku, 0, 0);
			continue;
		}

		if(rcret == getrcconfigint("rcred", NULL)) {
			drawscreen(wait, 0, 0);
			iLevel = 1;
			sudokunew(grid, checks, level, iLevel);
			clearscreen(wait);
			drawscreen(sudoku, 0, 0);
			continue;
		}

		if(rcret == getrcconfigint("rcgreen", NULL)) {
			drawscreen(wait, 0, 0);
			iLevel = 0;
			sudokunew(grid, checks, level, iLevel);
			clearscreen(wait);
			drawscreen(sudoku, 0, 0);
			continue;
		}

		if( rcret == getrcconfigint("rc0", NULL) || rcret == getrcconfigint("rc1", NULL) ||
			rcret == getrcconfigint("rc2", NULL) || rcret == getrcconfigint("rc3", NULL) ||
			rcret == getrcconfigint("rc4", NULL) || rcret == getrcconfigint("rc5", NULL) ||
			rcret == getrcconfigint("rc6", NULL) || rcret == getrcconfigint("rc7", NULL) ||
			rcret == getrcconfigint("rc8", NULL) || rcret == getrcconfigint("rc9", NULL) ) {

			if(grid->select != NULL && grid->select->fontcol != gColPuzzle && grid->select->fontcol != gColOK) {
				if( rcret == getrcconfigint("rc0", NULL) ) changetext(grid->select, "");
				if( rcret == getrcconfigint("rc1", NULL) ) changetext(grid->select, "1");
				if( rcret == getrcconfigint("rc2", NULL) ) changetext(grid->select, "2");
				if( rcret == getrcconfigint("rc3", NULL) ) changetext(grid->select, "3");
				if( rcret == getrcconfigint("rc4", NULL) ) changetext(grid->select, "4");
				if( rcret == getrcconfigint("rc5", NULL) ) changetext(grid->select, "5");
				if( rcret == getrcconfigint("rc6", NULL) ) changetext(grid->select, "6");
				if( rcret == getrcconfigint("rc7", NULL) ) changetext(grid->select, "7");
				if( rcret == getrcconfigint("rc8", NULL) ) changetext(grid->select, "8");
				if( rcret == getrcconfigint("rc9", NULL) ) changetext(grid->select, "9");
				grid->select->fontcol = gColInput;
				drawscreen(sudoku, 0, 0);
			}
			continue;
		}

	}

	delownerrc(sudoku);
	clearscreen(sudoku);
}

//wird beim Laden ausgefuehrt
void init(void) {
	char* tmpstr = NULL;

	g_SudokuCorePath = createpluginpath("/sudoku/sudoku_core", 0);

	tmpstr = createpluginpath("/sudoku/skin.xml", 0);
	readscreen(tmpstr, 208, 1);
	free(tmpstr); tmpstr = NULL;

	pluginaktiv = 1;
	debug(10, "sudoku plugin loaded!");
}

//wird beim Entladen ausgefuehrt
void deinit(void) {
	delmarkedscreen(208);
	pluginaktiv = 0;
	debug(10, "sudoku plugin removed!");
}

//wird in der Pluginverwaltung bzw Menue ausgefuehrt
void start(void) {
	long tmplistboxselectcol = status.listboxselectcol;
	status.listboxselectcol = gColSelect;

	screensudoku();

	//reset
	status.listboxselectcol = tmplistboxselectcol;
}
