#ifndef PANEL_HEADER_H
#define PANEL_HEADER_H

// panel_ipk.h
char* ipklistbox(char* defaultstr, char* str, char* skinname, char* skintitle, char* skinpath, int showpng);

void screenplugin();

// panel_main.h
int panel_menucall(struct skin* panel_menuentry);

#endif
