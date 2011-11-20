#ifndef AAFPANEL_HEADER_H
#define AAFPANEL_HEADER_H

// aafpanel_ipk.h
char* ipklistbox(char* defaultstr, char* str, char* skinname, char* skintitle, char* skinpath, int showpng);

void screenplugin();

// aafpanel_main.h
int aafpanel_menucall(struct skin* aafpanel_menuentry);

#endif
