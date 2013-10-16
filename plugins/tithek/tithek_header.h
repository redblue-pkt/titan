#ifndef TITHEK_HEADER_H
#define TITHEK_HEADER_H

void titheklog(int debuglevel, char* name1, char* name2, char* content)
{
	char* tmpstr = NULL;
	
	if(debuglevel != 99) return;

	tmpstr = ostrcat(name1, name2, 0, 0);
	writesys(tmpstr, content, 0);
	free(tmpstr);
}

void screentithekplay(char* titheklink, char* title, int first);
void screentithek_settings();
char* oltostr(long val, unsigned base);
char* putlocker(char* host, char* file, char* hosterurl);
char* filenuke(char* host, char* file, char* hosterurl);
char* streamcloud(char* host, char* file, char* hosterurl);
char* myvideo(char* link, char* url, char* name, int flag);
char* rtl2now(char* link, char* url, char* name, int flag);
char* youtube(char* link, char* url, char* name, int flag);
char* flashx(char* host, char* file, char* hosterurl);
char* vidstream(char* host, char* file, char* hosterurl);
char* xvidstage(char* host, char* file, char* hosterurl);
char* nowvideo(char* host, char* file, char* hosterurl);
char* movshare(char* host, char* file, char* hosterurl);
char* movreel(char* host, char* file, char* hosterurl);
char* novamov(char* host, char* file, char* hosterurl);
int myvideo_search(struct skin* grid, struct skin* listbox, struct skin* countlabel, struct skin* load, char* link, char* title, char* searchstr, int flag);
int youtube_search(struct skin* grid, struct skin* listbox, struct skin* countlabel, struct skin* load, char* link, char* title, char* searchstr);
int kinox_search(struct skin* grid, struct skin* listbox, struct skin* countlabel, struct skin* load, char* link, char* title, char* searchstr, int flag);
int kinox_search_cast(struct skin* grid, struct skin* listbox, struct skin* countlabel, struct skin* load, char* link, char* title, char* searchstr);
int solarmovie_search(struct skin* grid, struct skin* listbox, struct skin* countlabel, struct skin* load, char* link, char* title, char* searchstr, int flag);
int kinox_hoster(struct skin* grid, struct skin* listbox, struct skin* countlabel, struct skin* load, char* link, char* title);
int kinox_hoster_series(struct skin* grid, struct skin* listbox, struct skin* countlabel, struct skin* load, char* link, char* title);
int solarmovie_hoster(struct skin* grid, struct skin* listbox, struct skin* countlabel, struct skin* load, char* link, char* title);
int movie4k_hoster(struct skin* grid, struct skin* listbox, struct skin* countlabel, struct skin* load, char* link, char* title);
int movie4k_search(struct skin* grid, struct skin* listbox, struct skin* countlabel, struct skin* load, char* link, char* title, char* searchstr, int flag);
int movie4k_hoster_series(struct skin* grid, struct skin* listbox, struct skin* countlabel, struct skin* load, char* link, char* title);
int movie4k_series(struct skin* grid, struct skin* listbox, struct skin* countlabel, struct skin* load, char* link, char* title);
int movie4k_series_listed(struct skin* grid, struct skin* listbox, struct skin* countlabel, struct skin* load, char* link, char* title);
char* beeg(char* link);
char* mlehd(char* link);
char* kinox(char* link);
char* movie4k(char* link);
char* xvideos(char* link);
char* solarmovie(char* link);
char* ard(char* link);
char* zdf(char* link);

#endif
