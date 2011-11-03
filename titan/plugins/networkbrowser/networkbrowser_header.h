#ifndef NETWORKBROWSER_HEADER_H
#define NETWORKBROWSER_HEADER_H

char* getnetworkbrowser_cifs(char* s, char* r, char* u, char* p);
char* getnetworkbrowser_nfs(char* s, char* r);
char* getnetworkbrowser_dns();
void screennetworkbrowser_addshare(int mode, char* sharename, char* ipaddresse, char* sharedir, char* username, char* password, char* rsize, char* wsize, char* protocol, char* options, char* hddreplacement, char* options1, char* options2, char* ssl, char* proxy, char* proxyip, char* proxyport, char* proxyuser, char* proxypass, char* ftpport, char* userauth, char* proxyauth, char* useproxy, char* usessl);
void screennetworkbrowser_readshare(int mode, char* input_sharename, char* input_shareline);

// networkbrowser_listbox
char* networkbrowser_listbox(char* defaultstr, char* str, char* skinname, char* skintitle, char* skinpath, int showpng);

#endif
