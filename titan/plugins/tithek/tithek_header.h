#ifndef TITHEK_HEADER_H
#define TITHEK_HEADER_H

void screentithekplay(char* titheklink, char* title, int first);
void screentithek_settings();
char* oltostr(long val, unsigned base);
char* putlocker(char* host, char* file);
char* filenuke(char* host, char* file);
char* streamcloud(char* host, char* file);
char* kinox(char* link, char* url, char* name, int flag);
char* myvideo(char* link, char* url, char* name, int flag);
char* rtl2now(char* link, char* url, char* name, int flag);
char* youtube(char* link, char* url, char* name, int flag);
void myvideo_search(struct skin* grid, struct skin* listbox, struct skin* countlabel, struct skin* load, char* link, char* title);
void youtube_search(struct skin* grid, struct skin* listbox, struct skin* countlabel, struct skin* load, char* link, char* title);
void kinox_search(struct skin* grid, struct skin* listbox, struct skin* countlabel, struct skin* load, char* link, char* title);
void kinox_hoster(struct skin* grid, struct skin* listbox, struct skin* countlabel, struct skin* load, char* link, char* title);
void kinox_hoster_series(struct skin* grid, struct skin* listbox, struct skin* countlabel, struct skin* load, char* link, char* title);

#endif
