#ifndef TITHEK_HEADER_H
#define TITHEK_HEADER_H

void screentithekplay(char* titheklink, char* title, int first);
char* getstreamurl(char* link, char* url, char* name, int flag);
char* MDString (char *string);
char* MDFile (char *string);
char* MDPrint (unsigned char digest[16]);
//void rc4(u_char *key, u_char *input, u_char *output);
//void rc4(unsigned char* ByteInput, unsigned char* pwd, unsigned char* ByteOutput);

#endif
