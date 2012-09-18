#ifndef TITHEK_HEADER_H
#define TITHEK_HEADER_H

void screentithekplay(char* titheklink, char* title, int first);
char* getstreamurl(char* link, char* url, char* name, int flag);
void screentithek_settings();
char* oltostr(long val, unsigned base);
char* putlocker(char* host, char* file);
char* filenuke(char* host, char* file);

#endif
