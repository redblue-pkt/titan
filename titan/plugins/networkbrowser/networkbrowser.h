/**************************************************************************/
/* Name :   networkbrowser.h                                              */
/*                                                                        */
/* Author:  obi                                                           */
/*                                                                        */
/* Licence: This file is subject to the terms and conditions of the       */
/*          GNU General Public License version 2.                         */
/**************************************************************************/	

#ifndef NETWORKBROWSER_H
#define NETWORKBROWSER_H

void screennetworkbrowser()
{
	status.hangtime = 99999;
//	char* tmpstr = NULL;
//	tmpstr = ostrcat(getconfig("pluginpath", NULL), "/networkbrowser/skin.xml", 0, 0);
//	readscreen(tmpstr, 101, 1);
//	free(tmpstr); tmpstr = NULL;

	struct skin* net_addshare = getscreen("networkbrowser_main");
	char* title = NULL;
	title = ostrcat(title , _("Scanning Network Shares, please wait !!"), 1, 0);
	changetitle(net_addshare, title);

	int tmphangtime = 999999;
	status.hangtime = tmphangtime;
start:
	drawscreen(net_addshare, 0);

	char* skinname = "networkbrowser";
	char* skintitle = _("Networkbrowser - show scanned DNS-Shares");

	char* tmpdns = NULL;
	char* tmpcifs = NULL;
	char* tmpnfs = NULL;
	char* tmpshares = NULL;

	tmpdns = getnetworkbrowser_dns();
	char* dns = networkbrowser_listbox(NULL, tmpdns, skinname, skintitle, "%pluginpath%/networkbrowser/skin/", 3);

	if(dns != NULL)
	{
		int count = 0;
		struct splitstr* ret = NULL;
		ret = strsplit(dns, " ()", &count);
		char* user = "user";
		char* pass = "pass";
		skintitle = ostrcat("", _("Networkbrowser - show scanned cifs/nfs-Shares"), 0, 0);
		tmpcifs = getnetworkbrowser_cifs((&ret[1])->part, (&ret[0])->part, user, pass);
		tmpnfs = getnetworkbrowser_nfs((&ret[1])->part, (&ret[0])->part);
		tmpshares = ostrcat(tmpcifs, tmpnfs, 1, 1);
		char* shares = networkbrowser_listbox(NULL, tmpshares, skinname, skintitle, "%pluginpath%/networkbrowser/skin/", 4);
		if(shares != NULL){
			int count1 = 0;
			struct splitstr* ret1 = NULL;
			ret1 = strsplit(shares, " ():", &count1);
			debug(70, "save share: %s", shares);
			printf("ret1 0; %s\n", (&ret1[0])->part);
			if(ostrcmp((&ret1[0])->part, "cifs") == 0){
				debug(70, "start addshare cifs");
				screennetworkbrowser_addshare(0,(&ret[0])->part,(&ret[1])->part, (&ret1[2])->part, user, pass, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
			} else if(ostrcmp((&ret1[0])->part, "nfs") == 0){
				debug(70, "start addshare nfs");
				screennetworkbrowser_addshare(1,(&ret[0])->part,(&ret[1])->part, (&ret1[2])->part, user, pass, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
			} else if(ostrcmp((&ret1[0])->part, "ftp") == 0){
				debug(70, "start addshare ftp");
				screennetworkbrowser_addshare(2,(&ret[0])->part,(&ret[1])->part, (&ret1[2])->part, user, pass, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
			}
		} else {
			goto start;
		}
		free(shares); shares = NULL;
	} else {
		textbox(_("Message"), _("Can't get Network Infos"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 0, 0);
	}

	free(dns); dns = NULL;
	free(tmpdns); tmpdns = NULL;
	free(tmpshares); tmpshares = NULL;
	delownerrc(net_addshare);
	clearscreen(net_addshare);
	status.hangtime = getconfigint("hangtime", NULL);
}

char* getnetworkbrowser_dns()
{
	struct inetwork* net = getinetworkbydevice("eth0");
	char* tmpstr = NULL;
	netinfo *nInfo;
	char *s = NULL;
	int i;

	if(net == NULL) return NULL;

	s = ostrcat(s, net->ip, 1, 0);
	debug(70, "net->ip: %s", s);

	s = ostrcat(s, "/24", 1, 0);
	debug(70, "net->ip: %s", s);
	debug(70, "start scanning: %s", s);
	nInfo = newNetInfo();
	netzInfo(s, nInfo);
	for (i=0; i<256; i++)
	{
		if(nInfo[i].ip[0] == '\0'){
			break;
		}
		else
		{
			debug(70, "service: %s", nInfo[i].service);
			debug(70, "ip: %s", nInfo[i].ip);
			debug(70, "name: %s", nInfo[i].name);
			debug(70, "mac: %s", nInfo[i].mac);
			tmpstr = ostrcat(tmpstr , strstrip(nInfo[i].domain), 1, 0);
			tmpstr = ostrcat(tmpstr , " (", 1, 0);

			tmpstr = ostrcat(tmpstr , strstrip(nInfo[i].ip), 1, 0);
			tmpstr = ostrcat(tmpstr , ")\n", 1, 0);

		}
	}
	debug(70, "tmpstr: %s", tmpstr);
	freeNetInfo(nInfo);
	free(s); s = NULL;
	return tmpstr;	
}

char* getnetworkbrowser_cifs(char* s, char* r, char* u, char* p)
{
	char* tmpstr = NULL;
	shareinfo *sInfo;
	int i;
	sInfo = newShareInfo();
	smbInfo(s,r,u,p,sInfo);

	for (i=0; i<128; i++)
	{
		if(sInfo[i].sharename[0] == '\0'){
			break;
		}
		else
		{
			debug(70, "sharename: %s", sInfo[i].sharename);
			debug(70, "type: %s", sInfo[i].typ);
			debug(70, "comment: %s", sInfo[i].comment);
			debug(70, "rech: %s", r);
			debug(70, "rechip: %s", s);
			tmpstr = ostrcat(tmpstr , "(cifs) ", 1, 0);
			tmpstr = ostrcat(tmpstr , strstrip(s), 1, 0);
			tmpstr = ostrcat(tmpstr , ": /", 1, 0);
			tmpstr = ostrcat(tmpstr , strstrip(sInfo[i].sharename), 1, 0);
			tmpstr = ostrcat(tmpstr , "\n", 1, 0);
		}
	}

	debug(70, "tmpstr: %s", tmpstr);
	freeShareInfo(sInfo);
	return tmpstr;	
}

char* getnetworkbrowser_nfs(char* s, char* r)
{
	char* tmpstr = NULL;
	nfsinfo *nfsInfo;
	int i;
	int err = 0;
	nfsInfo = newNfsInfo();
	err = showNfsShare(s, nfsInfo);
	if (err == 0)
	{
		for (i=0; i<256; i++)
		{
			if(nfsInfo[i].ip[0] == '\0') {
				break;
			}
			else
			{
				debug(70, "ip: %s", nfsInfo[i].ip);
				debug(70, "share: %s", nfsInfo[i].share);
				debug(70, "nfsShare: nfsShare");
				debug(70, "rech: %s", r);
				debug(70, "rechip: %s", s);
				tmpstr = ostrcat(tmpstr , "(nfs) ", 1, 0);
				tmpstr = ostrcat(tmpstr , strstrip(s), 1, 0);
				tmpstr = ostrcat(tmpstr , ": ", 1, 0);
				tmpstr = ostrcat(tmpstr , strstrip(nfsInfo[i].share), 1, 0);
				tmpstr = ostrcat(tmpstr , "\n", 1, 0);
			}
		}
	}

	debug(70, "tmpstr: %s", tmpstr);
	freeNfsInfo(nfsInfo);
	return tmpstr;	
}

// mode 0 = cifs
// mode 1 = nfs
// mode 2 = ftp

void screennetworkbrowser_addshare(int mode, char* sharename, char* ipaddresse, char* sharedir, char* username, char* password, char* rsize, char* wsize, char* protocol, char* options, char* hddreplacement, char* options1, char* options2, char* ssl, char* proxy, char* proxyip, char* proxyport, char* proxyuser, char* proxypass, char* ftpport, char* userauth, char* proxyauth, char* useproxy, char* usessl)
{
	int rcret = 0;
	char* skinname = NULL;

	skinname = "networkbrowser_add_share";

	struct skin* net_addshare = getscreen(skinname);
	struct skin* listbox = getscreennode(net_addshare, "listbox");
	struct skin* tmp = NULL;

	struct skin* skin_sharename = getscreennode(net_addshare, "skin_sharename");
	struct skin* skin_ipaddresse = getscreennode(net_addshare, "skin_ipaddresse");
	struct skin* skin_sharedir = getscreennode(net_addshare, "skin_sharedir");
	struct skin* skin_username = getscreennode(net_addshare, "skin_username");
	struct skin* skin_password = getscreennode(net_addshare, "skin_password");
	struct skin* skin_wsize = getscreennode(net_addshare, "skin_wsize");
	struct skin* skin_rsize = getscreennode(net_addshare, "skin_rsize");
	struct skin* skin_hddreplacement = getscreennode(net_addshare, "skin_hddreplacement");

	struct skin* skin_protocol = getscreennode(net_addshare, "skin_protocol");
	struct skin* skin_options = getscreennode(net_addshare, "skin_options");

	struct skin* skin_options1 = getscreennode(net_addshare, "skin_options1");
	struct skin* skin_options2 = getscreennode(net_addshare, "skin_options2");
	struct skin* skin_ssl = getscreennode(net_addshare, "skin_ssl");
	struct skin* skin_proxy = getscreennode(net_addshare, "skin_proxy");
	struct skin* skin_proxyip = getscreennode(net_addshare, "skin_proxyip");
	struct skin* skin_proxyport = getscreennode(net_addshare, "skin_proxyport");
	struct skin* skin_proxyuser = getscreennode(net_addshare, "skin_proxyuser");
	struct skin* skin_proxypass = getscreennode(net_addshare, "skin_proxypass");
	struct skin* skin_ftpport = getscreennode(net_addshare, "skin_ftpport");

	struct skin* skin_useproxy = getscreennode(net_addshare, "skin_useproxy");
	struct skin* skin_usessl = getscreennode(net_addshare, "skin_usessl");

	struct skin* skin_userauth = getscreennode(net_addshare, "skin_userauth");
	struct skin* skin_proxyauth = getscreennode(net_addshare, "skin_proxyauth");

	struct skin* skin_mode = getscreennode(net_addshare, "skin_mode");

	addscreenrc(net_addshare, listbox);
	listbox->aktline = 1;
	listbox->aktpage = 1;

	char* tmp_sharename = NULL;
	char* tmp_sharedir = NULL;
	char* tmp_ipaddresse = NULL;
	char* tmp_username = NULL;
	char* tmp_password = NULL;
	char* tmp_wsize = NULL;
	char* tmp_rsize = NULL;
	char* tmp_hddreplacement = NULL;
	char* tmp_protocol = NULL;
	char* tmp_options = NULL;
	char* tmp_options1 = NULL;
	char* tmp_options2 = NULL;
	char* tmp_ssl = NULL;
	char* tmp_proxy = NULL;
	char* tmp_proxyip = NULL;
	char* tmp_proxyport = NULL;
	char* tmp_proxyuser = NULL;
	char* tmp_proxypass = NULL;
	char* tmp_ftpport = NULL;
	char* tmp_userauth = NULL;
	char* tmp_proxyauth = NULL;
	char* tmp_useproxy = NULL;
	char* tmp_usessl = NULL;
	char* tmp_mode = NULL;

	char* savesettings = NULL;
	char* cmd1 = NULL;
	char* cmd2 = NULL;
	char* read = NULL;
	read = command("cat /var/etc/automount/.recordshare");

	debug(70, "----------- input -----------");
	debug(70, "mode: %d", mode);
	debug(70, "sharename: %s", sharename);
	debug(70, "sharedir: %s", sharedir);
	debug(70, "ipaddresse: %s", ipaddresse);
	debug(70, "username: %s", username);
	debug(70, "password: %s", password);
	debug(70, "protocol: %s", protocol);
	debug(70, "rsize: %s", rsize);
	debug(70, "wsize: %s", wsize);
	debug(70, "options: %s", options);
	debug(70, "options1: %s", options1);
	debug(70, "options2: %s", options2);
	debug(70, "ssl: %s", ssl);
	debug(70, "proxy: %s", proxy);
	debug(70, "proxyip: %s", proxyip);
	debug(70, "proxyport: %s", proxyport);
	debug(70, "proxyuser: %s", proxyuser);
	debug(70, "proxypass: %s", proxypass);
	debug(70, "ftpport: %s", ftpport);
	debug(70, "userauth: %s", userauth);
	debug(70, "proxyauth: %s", proxyauth);

	debug(70, "useproxy: %s", useproxy);
	debug(70, "usessl: %s", usessl);

	debug(70, "hddreplacement: %s", hddreplacement);
	debug(70, "read hddreplacement: %s", read);
	debug(70, "-----------------------------");


	if (sharedir == NULL){
		if ( mode == 0 ){
			sharedir = ostrcat(sharedir, "/sharedir", 1, 0);
		} else if ( mode == 1 ){
			sharedir = ostrcat(sharedir, "/sharedir", 1, 0);
		} else if ( mode == 2 ){
			sharedir = ostrcat(sharedir, "sharedir", 1, 0);
		}
	}

	if (sharename == NULL){
		sharename = ostrcat(sharename, "SHARENAME", 1, 0);
	} else {
		if (read != NULL){
			if(ostrcmp(sharename, read) == 0){
				hddreplacement = ostrcat(hddreplacement, "1", 1, 0);
			}
		}
	}
	if (ipaddresse == NULL){
		ipaddresse = ostrcat(ipaddresse, "192.168.0.1", 1, 0);
	}
	if (username == NULL){
		username = ostrcat(username, "username", 1, 0);
	}
	if (password == NULL){
		password = ostrcat(password, "password", 1, 0);
	}
	if (protocol == NULL){
		protocol = ostrcat(protocol, "udp", 1, 0);
	}
	if (rsize == NULL){
		rsize = ostrcat(rsize, "32768", 1, 0);
	}
	if (wsize == NULL){
		wsize = ostrcat(wsize, "32768", 1, 0);
	}
	if (options == NULL){
		options = ostrcat(options, "rw,nolock", 1, 0);
	}
	if (hddreplacement == NULL){
		hddreplacement = ostrcat(hddreplacement, "0", 1, 0);
	}
 	if (options1 == NULL){
		options1 = ostrcat(options1, "allow_other", 1, 0);
	}
	if (options2 == NULL){
		options2 = ostrcat(options2, "nonempty", 1, 0);
	}
	if (proxy == NULL){
		proxy = ostrcat(proxy, "socks5", 1, 0);
	}
	if (proxyip == NULL){
		proxyip = ostrcat(proxyip, "proxyip", 1, 0);
	}
	if (proxyport == NULL){
		proxyport = ostrcat(proxyport, "proxyport", 1, 0);
	}
	if (proxyuser == NULL){
		proxyuser = ostrcat(proxyuser, "proxyuser", 1, 0);
	}
	if (proxypass == NULL){
		proxypass = ostrcat(proxypass, "proxypass", 1, 0);
	}
	if (ssl == NULL){
		ssl = ostrcat(ssl, "ssl_try", 1, 0);
	}
	if (proxyauth == NULL){
		proxyauth = ostrcat(proxyauth, "0", 1, 0);
	}
	if (userauth == NULL){
		userauth = ostrcat(userauth, "0", 1, 0);
	}
	if (usessl == NULL){
		usessl = ostrcat(usessl, "0", 1, 0);
	}
	if (useproxy == NULL){
		useproxy = ostrcat(useproxy, "0", 1, 0);
	}

	tmp_wsize = ostrcat(tmp_wsize, wsize, 1, 0);
	tmp_rsize = ostrcat(tmp_rsize, rsize, 1, 0);
	tmp_protocol = ostrcat(tmp_protocol, protocol, 1, 0);
	tmp_options = ostrcat(tmp_options, options, 1, 0);
	tmp_options1 = ostrcat(tmp_options1, options1, 1, 0);
	tmp_options2 = ostrcat(tmp_options2, options2, 1, 0);

	tmp_proxy = ostrcat(tmp_proxy, proxy, 1, 0);
	tmp_proxyauth = ostrcat(tmp_proxyauth, proxyauth, 1, 0);
	tmp_userauth = ostrcat(tmp_userauth, userauth, 1, 0);
	tmp_ssl = ostrcat(tmp_ssl, ssl, 1, 0);

	tmp_useproxy = ostrcat(tmp_useproxy, useproxy, 1, 0);
	tmp_usessl = ostrcat(tmp_usessl, usessl, 1, 0);

	tmp_hddreplacement = ostrcat(tmp_hddreplacement, hddreplacement, 1, 0);

	debug(70, "-------- set default --------");
	debug(70, "sharename: %s", sharename);
	debug(70, "sharedir: %s", sharedir);
	debug(70, "ipaddresse: %s", ipaddresse);
	debug(70, "username: %s", username);
	debug(70, "password: %s", password);
	debug(70, "protocol: %s", protocol);
	debug(70, "rsize: %s", rsize);
	debug(70, "wsize: %s", wsize);
	debug(70, "options: %s", options);
	debug(70, "options1: %s", options1);
	debug(70, "options2: %s", options2);
	debug(70, "ssl: %s", ssl);
	debug(70, "proxy: %s", proxy);
	debug(70, "proxyip: %s", proxyip);
	debug(70, "proxyport: %s", proxyport);
	debug(70, "proxyuser: %s", proxyuser);
	debug(70, "proxypass: %s", proxypass);
	debug(70, "ftpport: %s", ftpport);
	debug(70, "userauth: %s", userauth);
	debug(70, "proxyauth: %s", proxyauth);
	debug(70, "useproxy: %s", useproxy);
	debug(70, "usessl: %s", usessl);

	debug(70, "hddreplacement: %s", hddreplacement);
	debug(70, "read hddreplacement: %s", read);
	debug(70, "-----------------------------");

	if (mode == 0){
		tmp_mode = ostrcat("", "0", 0, 0);
	} else if (mode == 1){
		tmp_mode = ostrcat("", "1", 0, 0);
	} else if (mode == 2){
		tmp_mode = ostrcat("", "2", 0, 0);
	}

	addchoicebox(skin_mode, "0", _("cifs"));
	addchoicebox(skin_mode, "1", _("nfs"));
	addchoicebox(skin_mode, "2", _("ftpfs"));
	setchoiceboxselection(skin_mode, tmp_mode);

	tmp_sharename = ostrcat(tmp_sharename , sharename, 1, 0);
	changemask(skin_sharename, "abcdefghijklmnopqrstuvwxyz");
	changeinput(skin_sharename, sharename);

	tmp_ipaddresse = ostrcat(tmp_ipaddresse , ipaddresse, 1, 0);
	changemask(skin_ipaddresse, "000.000.000.000");
	changeinput(skin_ipaddresse, fixip(ipaddresse, 0));

	tmp_ftpport = ostrcat(tmp_ftpport , ftpport, 1, 0);
	changemask(skin_ftpport, "00000");
	changeinput(skin_ftpport, ftpport);

	tmp_sharedir = ostrcat(tmp_sharedir , sharedir, 1, 0);
	if ((mode == 0) || (mode == 1)){
		strcpy(tmp_sharedir, sharedir + 1);
	}
	changemask(skin_sharedir, "abcdefghijklmnopqrstuvwxyz");
	changeinput(skin_sharedir, tmp_sharedir);

	addchoicebox(skin_wsize, "32768", "32768");
	addchoicebox(skin_wsize, "49152", "49152");
	addchoicebox(skin_wsize, "65536", "65536");
	addchoicebox(skin_wsize, "4096", "4096");
	addchoicebox(skin_wsize, "8192", "8192");
	addchoicebox(skin_wsize, "16384", "16384");
	setchoiceboxselection(skin_wsize, wsize);

	addchoicebox(skin_rsize, "32768", "32768");
	addchoicebox(skin_rsize, "49152", "49152");
	addchoicebox(skin_rsize, "65536", "65536");
	addchoicebox(skin_rsize, "4096", "4096");
	addchoicebox(skin_rsize, "8192", "8192");
	addchoicebox(skin_rsize, "16384", "16384");
	setchoiceboxselection(skin_rsize, rsize);

	addchoicebox(skin_ssl, "ssl_try", _("ssl_try"));
	addchoicebox(skin_ssl, "ssl_control", _("ssl_control"));
	addchoicebox(skin_ssl, "ssl", _("ssl"));
	setchoiceboxselection(skin_ssl, ssl);

	addchoicebox(skin_proxy, "socks5", _("socks5"));
	addchoicebox(skin_proxy, "socks4", _("socks4"));
	addchoicebox(skin_proxy, "html", _("html"));
	setchoiceboxselection(skin_proxy, proxy);

	addchoicebox(skin_useproxy, "0", _("no"));
	addchoicebox(skin_useproxy, "1", _("yes"));
	setchoiceboxselection(skin_useproxy, useproxy);

	addchoicebox(skin_usessl, "0", _("no"));
	addchoicebox(skin_usessl, "1", _("yes"));
	setchoiceboxselection(skin_usessl, usessl);

	addchoicebox(skin_userauth, "0", _("no"));
	addchoicebox(skin_userauth, "1", _("yes"));
	setchoiceboxselection(skin_userauth, userauth);

	addchoicebox(skin_proxyauth, "0", "0");
	addchoicebox(skin_proxyauth, "1", _("yes"));
	setchoiceboxselection(skin_proxyauth, proxyauth);

	tmp_proxyip = ostrcat(tmp_proxyip , proxyip, 1, 0);
	changemask(skin_proxyip, "000.000.000.000");
	changeinput(skin_proxyip, fixip(proxyip, 0));

	tmp_proxyport = ostrcat(tmp_proxyport , proxyport, 1, 0);
	changemask(skin_proxyport, "00000");
	changeinput(skin_proxyport, proxyport);

	tmp_proxyuser = ostrcat(tmp_proxyuser , proxyuser, 1, 0);
	changemask(skin_proxyuser, "abcdefghijklmnopqrstuvwxyz");
	changeinput(skin_proxyuser, proxyuser);

	tmp_proxypass = ostrcat(tmp_proxypass , proxypass, 1, 0);
	changemask(skin_proxypass, "abcdefghijklmnopqrstuvwxyz");
	changeinput(skin_proxypass, proxypass);

	tmp_username = ostrcat(tmp_username , username, 1, 0);
	changemask(skin_username, "abcdefghijklmnopqrstuvwxyz");
	changeinput(skin_username, username);

	tmp_password = ostrcat(tmp_password , password, 1, 0);
	changemask(skin_password, "abcdefghijklmnopqrstuvwxyz");
	changeinput(skin_password, password);

	addchoicebox(skin_hddreplacement, "0", _("no"));
	addchoicebox(skin_hddreplacement, "1", _("yes"));
	setchoiceboxselection(skin_hddreplacement, hddreplacement);

	addchoicebox(skin_protocol, "udp", _("udp"));
	addchoicebox(skin_protocol, "tcp", _("tcp"));
	setchoiceboxselection(skin_protocol, protocol);

	addchoicebox(skin_options, "rw,nolock", _("rw,nolock"));
	addchoicebox(skin_options, "ro,nolock", _("ro,nolock"));
	setchoiceboxselection(skin_options, options);

	addchoicebox(skin_options1, "allow_other", _("allow_other"));
	setchoiceboxselection(skin_options1, options1);

	addchoicebox(skin_options2, "nonempty", _("nonempty"));
	setchoiceboxselection(skin_options2, options2);

	if (mode == 0){
		char* title = NULL;
		title = ostrcat(title , _("Add Cifs Network Share"), 1, 0);
		title = ostrcat(title , " (", 1, 0);
		title = ostrcat(title , strstrip(sharename), 1, 0);
		title = ostrcat(title , ")", 1, 0);
		changetitle(net_addshare, title);

		skin_username->hidden = NO;
		skin_password->hidden = NO;
		skin_protocol->hidden = YES;
		skin_rsize->hidden = NO;
		skin_wsize->hidden = NO;
		skin_options->hidden = YES;
		skin_options1->hidden = YES;
		skin_options2->hidden = YES;
		skin_ssl->hidden = YES;
		skin_proxy->hidden = YES;
		skin_proxyip->hidden = YES;
		skin_proxyport->hidden = YES;
		skin_proxyuser->hidden = YES;
		skin_proxypass->hidden = YES;
		skin_ftpport->hidden = YES;
		skin_userauth->hidden = YES;
		skin_proxyauth->hidden = YES;
		skin_useproxy->hidden = YES;
		skin_usessl->hidden = YES;
	} else if (mode == 1){
		char* title = NULL;
		title = ostrcat(title , _("Add Nfs Network Share"), 1, 0);
		title = ostrcat(title , " (", 1, 0);
		title = ostrcat(title , strstrip(sharename), 1, 0);
		title = ostrcat(title , ")", 1, 0);
		changetitle(net_addshare, title);

		skin_username->hidden = YES;
		skin_password->hidden = YES;
		skin_protocol->hidden = NO;
		skin_rsize->hidden = NO;
		skin_wsize->hidden = NO;
		skin_options->hidden = NO;
		skin_options1->hidden = YES;
		skin_options2->hidden = YES;
		skin_ssl->hidden = YES;
		skin_proxy->hidden = YES;
		skin_proxyip->hidden = YES;
		skin_proxyport->hidden = YES;
		skin_proxyuser->hidden = YES;
		skin_proxypass->hidden = YES;
		skin_ftpport->hidden = YES;
		skin_userauth->hidden = YES;
		skin_proxyauth->hidden = YES;
		skin_useproxy->hidden = YES;
		skin_usessl->hidden = YES;
	} else if (mode == 2){
		char* title = NULL;
		title = ostrcat(title , _("Add Ftpfs Network Share"), 1, 0);
		title = ostrcat(title , " (", 1, 0);
		title = ostrcat(title , strstrip(sharename), 1, 0);
		title = ostrcat(title , ")", 1, 0);
		changetitle(net_addshare, title);
		skin_protocol->hidden = YES;
		skin_rsize->hidden = YES;
		skin_wsize->hidden = YES;
		skin_options->hidden = YES;
		skin_options1->hidden = YES;
		skin_options2->hidden = YES;
		skin_userauth->hidden = NO;
		if(ostrcmp(tmp_userauth, "0") == 0){
			skin_username->hidden = YES;
			skin_password->hidden = YES;
		} else {
			skin_username->hidden = NO;
			skin_password->hidden = NO;
		}
		skin_usessl->hidden = NO;
		if(ostrcmp(tmp_usessl, "0") == 0){
			skin_ssl->hidden = YES;
		} else {
			skin_ssl->hidden = NO;
		}
		skin_useproxy->hidden = NO;
		if(ostrcmp(tmp_useproxy, "0") == 0){
			skin_proxy->hidden = YES;
			skin_proxyip->hidden = YES;
			skin_proxyport->hidden = YES;
			skin_proxyuser->hidden = YES;
			skin_proxypass->hidden = YES;
			skin_proxyauth->hidden = YES;
		} else {
			skin_proxy->hidden = NO;
			skin_proxyip->hidden = NO;
			skin_proxyport->hidden = NO;
			skin_proxyauth->hidden = NO;
			if(ostrcmp(tmp_proxyauth, "0") == 0){
				skin_proxyuser->hidden = YES;
				skin_proxypass->hidden = YES;
			} else {
				skin_proxyuser->hidden = NO;
				skin_proxypass->hidden = NO;
			}
		}
		skin_ftpport->hidden = NO;
	}

	drawscreen(net_addshare, 0);

	tmp = listbox->select;
	int save = 0;

	while(1)
	{
		addscreenrc(net_addshare, tmp);
		rcret = waitrc(net_addshare, 0, 0);
		tmp = listbox->select;

		if(listbox->select != NULL)
		{
			if(ostrcmp(listbox->select->name, "skin_sharename") == 0){
				tmp_sharename = ostrcat("", listbox->select->ret, 0, 0);
			} else if(ostrcmp(listbox->select->name, "skin_ipaddresse") == 0){
				tmp_ipaddresse = ostrcat("", listbox->select->ret, 0, 0);
			} else if(ostrcmp(listbox->select->name, "skin_sharedir") == 0){
				tmp_sharedir = ostrcat("", listbox->select->ret, 0, 0);
			} else if(ostrcmp(listbox->select->name, "skin_username") == 0){
				tmp_username = ostrcat("", listbox->select->ret, 0, 0);
			} else if(ostrcmp(listbox->select->name, "skin_password") == 0){
				tmp_password = ostrcat("", listbox->select->ret, 0, 0);
			} else if(ostrcmp(listbox->select->name, "skin_wsize") == 0){
				tmp_wsize = ostrcat("", listbox->select->ret, 0, 0);
			} else if(ostrcmp(listbox->select->name, "skin_rsize") == 0){
				tmp_rsize = ostrcat("", listbox->select->ret, 0, 0);
			} else if(ostrcmp(listbox->select->name, "skin_hddreplacement") == 0){
				tmp_hddreplacement = ostrcat("", listbox->select->ret, 0, 0); 
			} else if(ostrcmp(listbox->select->name, "skin_protocol") == 0){
				tmp_protocol = ostrcat("", listbox->select->ret, 0, 0);
			} else if(ostrcmp(listbox->select->name, "skin_options") == 0){
				tmp_options = ostrcat("", listbox->select->ret, 0, 0);
			} else if(ostrcmp(listbox->select->name, "skin_options1") == 0){
				tmp_options1 = ostrcat("", listbox->select->ret, 0, 0);
			} else if(ostrcmp(listbox->select->name, "skin_options2") == 0){
				tmp_options2 = ostrcat("", listbox->select->ret, 0, 0);
			} else if(ostrcmp(listbox->select->name, "skin_proxy") == 0){
				tmp_proxy = ostrcat("", listbox->select->ret, 0, 0);
			} else if(ostrcmp(listbox->select->name, "skin_proxyip") == 0){
				tmp_proxyip = ostrcat("", listbox->select->ret, 0, 0);
			} else if(ostrcmp(listbox->select->name, "skin_proxyport") == 0){
				tmp_proxyport = ostrcat("", listbox->select->ret, 0, 0);
			} else if(ostrcmp(listbox->select->name, "skin_proxyuser") == 0){
				tmp_proxyuser = ostrcat("", listbox->select->ret, 0, 0);
			} else if(ostrcmp(listbox->select->name, "skin_proxypass") == 0){
				tmp_proxypass = ostrcat("", listbox->select->ret, 0, 0);
			} else if(ostrcmp(listbox->select->name, "skin_ssl") == 0){
				tmp_ssl = ostrcat("", listbox->select->ret, 0, 0);
			} else if(ostrcmp(listbox->select->name, "skin_userauth") == 0){
				tmp_userauth = ostrcat("", listbox->select->ret, 0, 0);
				if(ostrcmp(listbox->select->ret, "0") == 0){
					skin_username->hidden = YES;
					skin_password->hidden = YES;
				} else {
					skin_username->hidden = NO;
					skin_password->hidden = NO;
				}
				drawscreen(net_addshare, 0);
			} else if(ostrcmp(listbox->select->name, "skin_proxyauth") == 0){
				tmp_proxyauth = ostrcat("", listbox->select->ret, 0, 0);
				if(ostrcmp(listbox->select->ret, "0") == 0){
					skin_proxyuser->hidden = YES;
					skin_proxypass->hidden = YES;
				} else {
					skin_proxyuser->hidden = NO;
					skin_proxypass->hidden = NO;
				}
				drawscreen(net_addshare, 0);
			} else if(ostrcmp(listbox->select->name, "skin_useproxy") == 0){
				tmp_useproxy = ostrcat("", listbox->select->ret, 0, 0);
				if(ostrcmp(listbox->select->ret, "0") == 0){
					skin_proxy->hidden = YES;
					skin_proxyip->hidden = YES;
					skin_proxyport->hidden = YES;
					skin_proxyuser->hidden = YES;
					skin_proxypass->hidden = YES;
					skin_proxyauth->hidden = YES;
				} else {
					skin_proxy->hidden = NO;
					skin_proxyip->hidden = NO;
					skin_proxyport->hidden = NO;
					skin_proxyauth->hidden = NO;
					if(ostrcmp(tmp_proxyauth, "0") == 0){
						skin_proxyuser->hidden = YES;
						skin_proxypass->hidden = YES;
					} else {
						skin_proxyuser->hidden = NO;
						skin_proxypass->hidden = NO;
					}
				}
				drawscreen(net_addshare, 0);
			} else if(ostrcmp(listbox->select->name, "skin_usessl") == 0){
				tmp_usessl = ostrcat("", listbox->select->ret, 0, 0);
				if(ostrcmp(listbox->select->ret, "0") == 0){
					skin_ssl->hidden = YES;
				} else {
					skin_ssl->hidden = NO;
				}
				drawscreen(net_addshare, 0);
			} else if(ostrcmp(listbox->select->name, "skin_mode") == 0){
				tmp_mode = ostrcat("", listbox->select->ret, 0, 0);
				if(ostrcmp(listbox->select->ret, "0") == 0){
					char* title = NULL;
					title = ostrcat(title , _("Add Cifs Network Share"), 1, 0);
					title = ostrcat(title , " (", 1, 0);
					title = ostrcat(title , strstrip(sharename), 1, 0);
					title = ostrcat(title , ")", 1, 0);
					changetitle(net_addshare, title);
			
					skin_username->hidden = NO;
					skin_password->hidden = NO;
					skin_protocol->hidden = YES;
					skin_rsize->hidden = NO;
					skin_wsize->hidden = NO;
					skin_options->hidden = YES;
					skin_options1->hidden = YES;
					skin_options2->hidden = YES;
					skin_ssl->hidden = YES;
					skin_proxy->hidden = YES;
					skin_proxyip->hidden = YES;
					skin_proxyport->hidden = YES;
					skin_proxyuser->hidden = YES;
					skin_proxypass->hidden = YES;
					skin_ftpport->hidden = YES;
					skin_userauth->hidden = YES;
					skin_proxyauth->hidden = YES;
					skin_useproxy->hidden = YES;
					skin_usessl->hidden = YES;
				} else if(ostrcmp(listbox->select->ret, "1") == 0){
					char* title = NULL;
					title = ostrcat(title , _("Add Nfs Network Share"), 1, 0);
					title = ostrcat(title , " (", 1, 0);
					title = ostrcat(title , strstrip(sharename), 1, 0);
					title = ostrcat(title , ")", 1, 0);
					changetitle(net_addshare, title);
			
					skin_username->hidden = YES;
					skin_password->hidden = YES;
					skin_protocol->hidden = NO;
					skin_rsize->hidden = NO;
					skin_wsize->hidden = NO;
					skin_options->hidden = NO;
					skin_options1->hidden = YES;
					skin_options2->hidden = YES;
					skin_ssl->hidden = YES;
					skin_proxy->hidden = YES;
					skin_proxyip->hidden = YES;
					skin_proxyport->hidden = YES;
					skin_proxyuser->hidden = YES;
					skin_proxypass->hidden = YES;
					skin_ftpport->hidden = YES;
					skin_userauth->hidden = YES;
					skin_proxyauth->hidden = YES;
					skin_useproxy->hidden = YES;
					skin_usessl->hidden = YES;
				} else if(ostrcmp(listbox->select->ret, "2") == 0){
					char* title = NULL;
					title = ostrcat(title , _("Add Ftpfs Network Share"), 1, 0);
					title = ostrcat(title , " (", 1, 0);
					title = ostrcat(title , strstrip(sharename), 1, 0);
					title = ostrcat(title , ")", 1, 0);
					changetitle(net_addshare, title);

					skin_protocol->hidden = YES;
					skin_rsize->hidden = YES;
					skin_wsize->hidden = YES;
					skin_options->hidden = YES;
					skin_options1->hidden = YES;
					skin_options2->hidden = YES;
					skin_userauth->hidden = NO;
					if(ostrcmp(tmp_userauth, "0") == 0){
						skin_username->hidden = YES;
						skin_password->hidden = YES;
					} else {
						skin_username->hidden = NO;
						skin_password->hidden = NO;
					}
					skin_usessl->hidden = NO;
					if(ostrcmp(tmp_usessl, "0") == 0){
						skin_ssl->hidden = YES;
					} else {
						skin_ssl->hidden = NO;
					}
					skin_useproxy->hidden = NO;
					if(ostrcmp(tmp_useproxy, "0") == 0){
						skin_proxy->hidden = YES;
						skin_proxyip->hidden = YES;
						skin_proxyport->hidden = YES;
						skin_proxyuser->hidden = YES;
						skin_proxypass->hidden = YES;
						skin_proxyauth->hidden = YES;
					} else {
						skin_proxy->hidden = NO;
						skin_proxyip->hidden = NO;
						skin_proxyport->hidden = NO;
						skin_proxyauth->hidden = NO;
						if(ostrcmp(tmp_proxyauth, "0") == 0){
							skin_proxyuser->hidden = YES;
							skin_proxypass->hidden = YES;
						} else {
							skin_proxyuser->hidden = NO;
							skin_proxypass->hidden = NO;
						}
					}
					skin_ftpport->hidden = NO;
				}
				drawscreen(net_addshare, 0);
			}
		}

		if(rcret == getrcconfigint("rcexit", NULL)) {
			break;
		}

		if(rcret == getrcconfigint("rcok", NULL)) {
			save=1;
			break;
		}
	}
	
	if(save == 1){
 		savesettings = ostrcat(savesettings, tmp_sharename, 1, 0);

		if(ostrcmp(tmp_mode, "0") == 0){
			savesettings = ostrcat(savesettings, "\t-fstype=cifs,rw,rsize=", 1, 0);
	 		savesettings = ostrcat(savesettings, tmp_rsize, 1, 0);
			savesettings = ostrcat(savesettings, ",wsize=", 1, 0);
 			savesettings = ostrcat(savesettings, tmp_wsize, 1, 0);
			savesettings = ostrcat(savesettings, ",user=", 1, 0); 
			savesettings = ostrcat(savesettings, tmp_username, 1, 0);
			savesettings = ostrcat(savesettings, ",pass=", 1, 0);
 			savesettings = ostrcat(savesettings, tmp_password, 1, 0);
			savesettings = ostrcat(savesettings, "\t://", 1, 0);
	 		savesettings = ostrcat(savesettings, fixip(tmp_ipaddresse, 1), 1, 0);
			savesettings = ostrcat(savesettings, "/", 1, 0);
			savesettings = ostrcat(savesettings, tmp_sharedir, 1, 0);
			savesettings = ostrcat(savesettings, "\n", 1, 0);
		} else if(ostrcmp(tmp_mode, "1") == 0){
			savesettings = ostrcat(savesettings, "\t-fstype=nfs,", 1, 0);
 			savesettings = ostrcat(savesettings, tmp_options, 1, 0);
			savesettings = ostrcat(savesettings, ",rsize=", 1, 0);
	 		savesettings = ostrcat(savesettings, tmp_rsize, 1, 0);
			savesettings = ostrcat(savesettings, ",wsize=", 1, 0);
 			savesettings = ostrcat(savesettings, tmp_wsize, 1, 0);
			savesettings = ostrcat(savesettings, ",", 1, 0); 
			savesettings = ostrcat(savesettings, tmp_protocol, 1, 0);
			savesettings = ostrcat(savesettings, "\t", 1, 0);
 			savesettings = ostrcat(savesettings, fixip(tmp_ipaddresse, 1), 1, 0);
			savesettings = ostrcat(savesettings, "://", 1, 0);
 			savesettings = ostrcat(savesettings, tmp_sharedir, 1, 0);
			savesettings = ostrcat(savesettings, "\n", 1, 0);
		} else if (ostrcmp(tmp_mode, "2") == 0){
			savesettings = ostrcat(savesettings, "\t-fstype=ftpfs,", 1, 0);
 			savesettings = ostrcat(savesettings, tmp_options, 1, 0);
			if(ostrcmp(tmp_useproxy, "1") == 0){
				savesettings = ostrcat(savesettings, ",proxy=", 1, 0);
		 		savesettings = ostrcat(savesettings, tmp_proxyip, 1, 0);
				savesettings = ostrcat(savesettings, "\\:", 1, 0);
	 			savesettings = ostrcat(savesettings, tmp_proxyport, 1, 0);
			}
			savesettings = ostrcat(savesettings, ",", 1, 0); 
			savesettings = ostrcat(savesettings, tmp_options1, 1, 0);

			if(ostrcmp(tmp_useproxy, "1") == 0){
				savesettings = ostrcat(savesettings, ",", 1, 0);
		 		savesettings = ostrcat(savesettings, tmp_proxy, 1, 0);
			}
			if(ostrcmp(tmp_proxyauth, "1") == 0){
				savesettings = ostrcat(savesettings, ",proxy_user=", 1, 0);
		 		savesettings = ostrcat(savesettings, tmp_proxyuser, 1, 0);
				savesettings = ostrcat(savesettings, "\\:", 1, 0);
	 			savesettings = ostrcat(savesettings, tmp_proxypass, 1, 0);
			}
			savesettings = ostrcat(savesettings, ",", 1, 0); 
			savesettings = ostrcat(savesettings, tmp_options2, 1, 0);

			if(ostrcmp(tmp_usessl, "1") == 0){
				savesettings = ostrcat(savesettings, ",", 1, 0);
		 		savesettings = ostrcat(savesettings, tmp_ssl, 1, 0);
			}

			savesettings = ostrcat(savesettings, "\t:ftp\\://g", 1, 0);
			if(ostrcmp(tmp_userauth, "1") == 0){
		 		savesettings = ostrcat(savesettings, tmp_username, 1, 0);
				savesettings = ostrcat(savesettings, "\\:", 1, 0);
	 			savesettings = ostrcat(savesettings, tmp_password, 1, 0);
				savesettings = ostrcat(savesettings, "@", 1, 0);
			}

 			savesettings = ostrcat(savesettings, fixip(tmp_ipaddresse, 1), 1, 0);
			savesettings = ostrcat(savesettings, "\\:", 1, 0);
 			savesettings = ostrcat(savesettings, tmp_ftpport, 1, 0);
			savesettings = ostrcat(savesettings, "/", 1, 0);
 			savesettings = ostrcat(savesettings, tmp_sharedir, 1, 0);
			savesettings = ostrcat(savesettings, "\n", 1, 0);
		}
		clearscreen(net_addshare);

		if(textbox(_("Message"), _("Are you sure you want to save this network mount?"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 0, 0) == 1){
			debug(70, "-----------------------------");
			debug(70, "tmp_sharename: %s", tmp_sharename);
			debug(70, "tmp_sharedir: %s", tmp_sharedir);
			debug(70, "tmp_ipaddresse: %s", tmp_ipaddresse);
			debug(70, "tmp_username: %s", tmp_username);
			debug(70, "tmp_password: %s", tmp_password);
			debug(70, "tmp_protocol: %s", tmp_protocol);
			debug(70, "tmp_rsize: %s", tmp_rsize);
			debug(70, "tmp_wsize: %s", tmp_wsize);
			debug(70, "tmp_options: %s", tmp_options);
			debug(70, "tmp_hddreplacement: %s", tmp_hddreplacement);
			debug(70, "tmp_options1: %s", tmp_options1);
			debug(70, "tmp_options2: %s", tmp_options2);
			debug(70, "tmp_ssl: %s", tmp_ssl);
			debug(70, "tmp_proxy: %s", tmp_proxy);
			debug(70, "tmp_proxyip: %s", tmp_proxyip);
			debug(70, "tmp_proxyport: %s", tmp_proxyport);
			debug(70, "tmp_proxyuser: %s", tmp_proxyuser);
			debug(70, "tmp_proxypass: %s", tmp_proxypass);
			debug(70, "tmp_ftpport: %s", tmp_ftpport);
			debug(70, "tmp_userauth: %s", tmp_userauth);
			debug(70, "tmp_proxyauth: %s", tmp_proxyauth);
			debug(70, "tmp_useproxy: %s", tmp_useproxy);
			debug(70, "tmp_usessl: %s", tmp_usessl);

			debug(70, "read hddreplacement: %s", read);
			debug(70, "-----------------------------");
			debug(70, "savesettings: %s", savesettings);
			debug(70, "-----------------------------");

			if(ostrcmp(tmp_hddreplacement, "1") == 0){
				system("rm /media/hdd");
				cmd1 = ostrcat(cmd1, "ln -s /media/net/", 1, 0);
				cmd1 = ostrcat(cmd1, tmp_sharename, 1, 0);
				cmd1 = ostrcat(cmd1, " /media/hdd", 1, 0);

				cmd2 = ostrcat(cmd2, "echo ", 1, 0);
				cmd2 = ostrcat(cmd2, tmp_sharename, 1, 0);
				cmd2 = ostrcat(cmd2, " > /var/etc/automount/.recordshare", 1, 0);

				system(cmd1);
				system(cmd2);
			}
			screennetworkbrowser_readshare(1, tmp_sharename, savesettings);
		}
	}

	free(tmp_sharename); tmp_sharename = NULL;
	free(tmp_sharedir); tmp_sharedir = NULL;
	free(tmp_ipaddresse); tmp_ipaddresse = NULL;
	free(tmp_username); tmp_username = NULL;
	free(tmp_password); tmp_password = NULL;
	free(tmp_sharename); tmp_protocol = NULL;
	free(tmp_options); tmp_options = NULL;

	free(tmp_options1); tmp_options1 = NULL;
	free(tmp_options2); tmp_options2 = NULL;
	free(tmp_ftpport); tmp_ftpport = NULL;
	free(tmp_proxy); tmp_proxy = NULL;
	free(tmp_proxyip); tmp_proxyip = NULL;
	free(tmp_proxyport); tmp_proxyport = NULL;
	free(tmp_proxyuser); tmp_proxyuser = NULL;
	free(tmp_proxypass); tmp_proxypass = NULL;
	free(tmp_ssl); tmp_ssl = NULL;
	free(tmp_proxyauth); tmp_proxyauth = NULL;
	free(tmp_userauth); tmp_userauth = NULL;
	free(tmp_useproxy); tmp_useproxy = NULL;
	free(tmp_usessl); tmp_usessl = NULL;

	free(savesettings); savesettings = NULL;
	free(cmd1); cmd1 = NULL;
	free(cmd2); cmd2 = NULL;

	delownconfigtmpall();
	delmarkedscreennodes(net_addshare, 1);
	delownerrc(net_addshare);
	clearscreen(net_addshare);

}

// mode 0 = list
// mode 1 = add
// mode 2 = del
// mode 3 = edit
// mode 4 = list (showshares after change)
// mode 5 = add (showshares after change)

void screennetworkbrowser_readshare(int mode, char* input_sharename, char* input_shareline)
{
	status.hangtime = 99999;
	debug(70, "mode: %d", mode);
	debug(70, "input_sharename: %s", input_sharename);
	debug(70, "input_shareline: %s", input_shareline);
	
//	char* tmpstr = NULL;
//	tmpstr = ostrcat(getconfig("pluginpath", NULL), "/networkbrowser/skin.xml", 0, 0);
//	readscreen(tmpstr, 101, 1);
//	free(tmpstr); tmpstr = NULL;

	char* tmpstr1 = NULL;
	char* tmpstr2 = NULL;
	char* tmpstr3 = NULL;
	char* tmpstr4 = NULL;
	char* tmpstr5 = NULL;
	char* tmpstr6 = NULL;
	char* tmpstr7 = NULL;
	char* tmpstr8 = NULL;
	char* tmpstr9 = NULL;
	char* tmpstr10 = NULL;
	char* tmpstr11 = NULL;
	char* tmpstr12 = NULL;
	char* tmpstr13 = NULL;

	char* tmp_sharename = NULL;
	char* tmp_ipaddresse = NULL;
	char* tmp_sharedir = NULL;
	char* tmp_sharedirlong = NULL;
	char* tmp_user = NULL;
	char* tmp_pass = NULL;
	char* tmp_wsize = NULL;
	char* tmp_rsize = NULL;
	char* tmp_hddreplacement = NULL;
	char* tmp_protocol = NULL;
	char* tmp_options = NULL;
	char* tmp_options1 = NULL;
	char* tmp_options2 = NULL;
	char* tmp_check = NULL;
	char* tmp_ssl = NULL;
	char* tmp_proxy = NULL;
	char* tmp_proxyip = NULL;
	char* tmp_proxyport = NULL;
	char* tmp_proxyuser = NULL;
	char* tmp_proxypass = NULL;
	char* tmp_ftpport = NULL;
	char* tmp_userauth = NULL;
	char* tmp_proxyauth = NULL;
	char* tmp_useproxy = NULL;
	char* tmp_usessl = NULL;

	char* outline = NULL;
	outline = ostrcat(outline, "# automatically generated by titan\n", 1, 0);
	outline = ostrcat(outline, "# do NOT change manually, all shares withe tab or gs on NetworkBrowser !\n", 1, 0);

	char* menu_sharelist = NULL;
	char* menu_sharetypes = NULL;
	char* menu_changeshare = NULL;

	char* text1 = NULL;
	text1 = ostrcat(text1, _("add new share"), 1, 0);
	char* text2 = NULL;
	text2 = ostrcat(text2, _("add cifs share"), 1, 0);
	char* text3 = NULL;
	text3 = ostrcat(text3, _("add nfs share"), 1, 0);
	char* text4 = NULL;
	text4 = ostrcat(text4, _("add ftp share"), 1, 0);
	char* text5 = NULL;
	text5 = ostrcat(text5, _("edit share"), 1, 0);
	char* text6 = NULL;
	text6 = ostrcat(text6, _("remove share"), 1, 0);

	char* sharetypes = NULL;
	sharetypes = ostrcat(sharetypes, text2, 1, 0);
	sharetypes = ostrcat(sharetypes, "\n", 1, 0);
	sharetypes = ostrcat(sharetypes, text3, 1, 0);
	sharetypes = ostrcat(sharetypes, "\n", 1, 0);
	sharetypes = ostrcat(sharetypes, text4, 1, 0);
	sharetypes = ostrcat(sharetypes, "\n", 1, 0);

	char* changeshare = NULL;
	changeshare = ostrcat(changeshare, text5, 1, 0);
	changeshare = ostrcat(changeshare, "\n", 1, 0);
	changeshare = ostrcat(changeshare, text6, 1, 0);
	changeshare = ostrcat(changeshare, "\n", 1, 0);

	char* sharelist = NULL;
	sharelist = ostrcat(sharelist, text1, 1, 0);
	sharelist = ostrcat(sharelist, "\n\t\n", 1, 0);

	char* tmp_sharedir_cifs = NULL;
	char* tmp_sharedir_nfs = NULL;

	tmp_wsize = ostrcat(tmp_wsize, "32768", 1, 0);
	tmp_rsize = ostrcat(tmp_wsize, "32768", 1, 0);
	tmp_user = ostrcat(tmp_user, "user", 1, 0);
	tmp_pass = ostrcat(tmp_pass, "pass", 1, 0);

	char* hddreplacement_ck = command("ls -al /media/hdd | tr '/' '\n' | tail -n1");

	tmpstr1 = command("cat /var/etc/automount/auto.misc");
	int count = 0;
	int i = 0;
	int save = 1;
	struct splitstr* ret1 = NULL;
	ret1 = strsplit(tmpstr1, "\n", &count);
	sleep(1);	
	int max = count;
	int update = 1;

	for( i = 0; i < max; i++){
		if (i < 2){
			continue;
		}

		free(tmp_sharename); tmp_sharename = NULL;
		free(tmp_sharedir); tmp_sharedir = NULL;
		free(tmp_sharedirlong); tmp_sharedirlong = NULL;
		free(tmp_ipaddresse); tmp_ipaddresse = NULL;
		free(tmp_user); tmp_user = NULL;
		free(tmp_pass); tmp_pass = NULL;
		free(tmp_sharename); tmp_sharename = NULL;
		free(tmp_protocol); tmp_protocol = NULL;
		free(tmp_options); tmp_options = NULL;
		free(tmp_options1); tmp_options1 = NULL;
		free(tmp_options2); tmp_options2 = NULL;
		free(tmp_check); tmp_check = NULL;
		free(tmp_ftpport); tmp_ftpport = NULL;
		free(tmp_proxy); tmp_proxy = NULL;
		free(tmp_proxyip); tmp_proxyip = NULL;
		free(tmp_proxyport); tmp_proxyport = NULL;
		free(tmp_proxyuser); tmp_proxyuser = NULL;
		free(tmp_proxypass); tmp_proxypass = NULL;
		free(tmp_hddreplacement); tmp_hddreplacement = NULL;

		struct splitstr* ret2 = NULL;
		int count2 = 0;
		tmpstr2 = NULL;
		tmpstr2 = ostrcat(tmpstr2, (&ret1[i])->part, 1, 0);
		ret2 = strsplit(tmpstr2, "\t", &count2);

		if((mode == 1) && (ostrcmp((&ret2[0])->part, input_sharename) == 0)){
			if(textbox(_("Message"), _("A mount entry with this name already exists!\nUpdate existing entry and continue?"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 0, 0) == 1){
				debug(70, "add share - continue set update=0 (%d) %s=%s mode=%d", i, (&ret2[0])->part, input_sharename, mode);
				debug(70, "add share - continue set update=0 (%d) %s", i, input_shareline);
				update = 0;
				outline = ostrcat(outline, input_shareline, 1, 0);
				outline = ostrcat(outline, "\n", 1, 0);
				mode = 5;
				continue;
			} else {
				save = 0;
			}
			mode = 5;
		} else if((mode == 2) && (ostrcmp((&ret2[0])->part, input_sharename) == 0)){
			if(textbox(_("Message"), _("Are you sure you want remove this share?"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 600, 200, 0, 0) == 1){
				debug(70, "remove share - continue: (%d) %s=%s mode=%d", i, (&ret2[0])->part, input_sharename, mode);
				debug(70, "remove share - continue: (%d) %s", i, input_shareline);
				mode = 4;
				continue;
			} else {
				save = 0;
			}
			mode = 4;
		}

		struct splitstr* ret3 = NULL;
		int count3 = 0;
		tmpstr3 = NULL;

		if (strlen((&ret2[0])->part) > 1){
			tmpstr3 = ostrcat(tmpstr3, (&ret2[1])->part, 1, 0);
			ret3 = strsplit(tmpstr3, ",", &count3);
			debug(71, "fstype: (%d) %s", i, (&ret3[0])->part);

			if(ostrcmp((&ret3[0])->part, "-fstype=cifs") == 0){
				if(ostrcmp((&ret3[2])->part, "rsize=") == 0){
					tmp_rsize = ostrcat("", tmp_rsize, 0, 0);
				} else {
					struct splitstr* ret4 = NULL;
					int count4 = 0;
					tmpstr4 = NULL;
					tmpstr4 = ostrcat(tmpstr4, (&ret3[2])->part, 1, 0);
					ret4 = strsplit(tmpstr4, "=", &count4);
					tmp_rsize = ostrcat("", (&ret4[1])->part, 0, 0);
					free(ret4); ret4 = NULL;
				}

				if(ostrcmp((&ret3[3])->part, "wsize=") == 0){
					tmp_wsize = ostrcat("", tmp_wsize, 0, 0);
				} else {
					struct splitstr* ret5 = NULL;
					int count5 = 0;
					tmpstr5 = NULL;
					tmpstr5 = ostrcat(tmpstr5, (&ret3[3])->part, 1, 0);
					ret5 = strsplit(tmpstr5, "=", &count5);
					tmp_wsize = ostrcat("", (&ret5[1])->part, 0, 0);
					free(ret5); ret5 = NULL;
				}

				if(ostrcmp((&ret3[4])->part, "user=") == 0){
					tmp_user = ostrcat("", tmp_user, 0, 0);
				} else {
					struct splitstr* ret6 = NULL;
					int count6 = 0;
					tmpstr6 = NULL;
					tmpstr6 = ostrcat(tmpstr6, (&ret3[4])->part, 1, 0);
					ret6 = strsplit(tmpstr6, "=", &count6);
					tmp_user = ostrcat("", (&ret6[1])->part, 0, 0);
					free(ret6); ret6 = NULL;
				}

				if(ostrcmp((&ret3[5])->part, "pass=") == 0){
					tmp_pass = ostrcat("", tmp_pass, 0, 0);
				} else {
					struct splitstr* ret7 = NULL;
					int count7 = 0;
					tmpstr7 = NULL;
					tmpstr7 = ostrcat(tmpstr7, (&ret3[5])->part, 1, 0);
					ret7 = strsplit(tmpstr7, "=", &count7);
					tmp_pass = ostrcat("", (&ret7[1])->part, 0, 0);
					free(ret7); ret7 = NULL;
				}

				struct splitstr* ret8 = NULL;
				int count8 = 0;
				tmpstr8 = NULL;
				tmpstr8 = ostrcat(tmpstr8, (&ret2[2])->part, 1, 0);
				ret8 = strsplit(tmpstr8, "/", &count8);
				tmp_ipaddresse = ostrcat("", (&ret8[1])->part, 0, 0);
				tmp_sharedir = ostrcat("", (&ret8[2])->part, 0, 0);

				tmp_sharename = ostrcat("", (&ret2[0])->part, 0, 0);

				if(ostrcmp((&ret2[0])->part, input_sharename) == 0){
					update = 0;
					debug(70, "set update=0 (%d) %s=%s", i, (&ret2[0])->part, input_sharename);
				} else {
					outline = ostrcat(outline, (&ret1[i])->part, 1, 0);
					outline = ostrcat(outline, "\n", 1, 0);
				}

				sharelist = ostrcat(sharelist, "(cifs) ", 1, 0);
				sharelist = ostrcat(sharelist, (&ret2[0])->part, 1, 0);
				sharelist = ostrcat(sharelist, " (", 1, 0);
				sharelist = ostrcat(sharelist, (&ret8[1])->part, 1, 0);
				sharelist = ostrcat(sharelist, ": /", 1, 0);
				sharelist = ostrcat(sharelist, (&ret8[2])->part, 1, 0);
				sharelist = ostrcat(sharelist, ")\n", 1, 0);
				debug(70, "sharelist: (%d) %s", i, sharelist);

				free(ret8); ret8 = NULL;

				if((mode == 3) && (ostrcmp((&ret2[0])->part, input_sharename) == 0)){
					if(ostrcmp((&ret2[0])->part, hddreplacement_ck) == 0){
						tmp_hddreplacement = ostrcat("", tmp_hddreplacement, 0, 1);
						tmp_hddreplacement = ostrcat(tmp_hddreplacement, "1", 1, 0);
					} else {
						tmp_hddreplacement = ostrcat("", tmp_hddreplacement, 0, 1);
						tmp_hddreplacement = ostrcat(tmp_hddreplacement, "0", 1, 0);
					}
					tmp_sharedir_cifs = ostrcat("/", tmp_sharedir, 0, 0);
					screennetworkbrowser_addshare(0, tmp_sharename, tmp_ipaddresse, tmp_sharedir_cifs, tmp_user, tmp_pass, tmp_rsize, tmp_wsize, NULL, NULL, tmp_hddreplacement, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
					save = 0;

					continue;
				}

			} else if(ostrcmp((&ret3[0])->part, "-fstype=nfs") == 0){
				tmp_options = ostrcat("", (&ret3[1])->part, 0, 0);
				tmp_options = ostrcat(tmp_options, ",", 1, 0);
				tmp_options = ostrcat(tmp_options, (&ret3[2])->part, 1, 0);

				if(ostrcmp((&ret3[3])->part, "rsize=") == 0){
					tmp_rsize = ostrcat("", tmp_rsize, 0, 0);
				} else {
					struct splitstr* ret4 = NULL;
					int count4 = 0;
					tmpstr4 = NULL;
					tmpstr4 = ostrcat(tmpstr4, (&ret3[3])->part, 1, 0);
					ret4 = strsplit(tmpstr4, "=", &count4);
					tmp_rsize = ostrcat("", (&ret4[1])->part, 0, 0);
					free(ret4); ret4 = NULL;
				}

				if(ostrcmp((&ret3[4])->part, "wsize=") == 0){
					tmp_wsize = ostrcat("", tmp_wsize, 0, 0);
				} else {
					struct splitstr* ret5 = NULL;
					int count5 = 0;
					tmpstr5 = NULL;
					tmpstr5 = ostrcat(tmpstr5, (&ret3[4])->part, 1, 0);
					ret5 = strsplit(tmpstr5, "=", &count5);
					tmp_wsize = ostrcat("", (&ret5[1])->part, 0, 0);
					free(ret5); ret5 = NULL;
				}

				tmp_protocol = ostrcat(",", (&ret3[5])->part, 0, 0);

				struct splitstr* ret6 = NULL;
				int count6 = 0;
				tmpstr6 = NULL;
				tmpstr6 = ostrcat(tmpstr6, (&ret2[2])->part, 1, 0);
				ret6 = strsplit(tmpstr6, ":", &count6);
				tmp_ipaddresse = ostrcat("", (&ret6[0])->part, 0, 0);
				tmp_sharedirlong = ostrcat("", (&ret6[1])->part, 0, 0);

				tmp_sharedir = ostrcat(tmp_sharedir, "", 1, 0);

				strcpy(tmp_sharedir, tmp_sharedirlong + 1);
				tmp_sharename = ostrcat("", (&ret2[0])->part, 0, 0);

				if(ostrcmp((&ret2[0])->part, input_sharename) == 0){
					update = 0;
				} else {
					outline = ostrcat(outline, (&ret1[i])->part, 1, 0);
					outline = ostrcat(outline, "\n", 1, 0);
				}

				sharelist = ostrcat(sharelist, "(nfs) ", 1, 0);
				sharelist = ostrcat(sharelist, (&ret2[0])->part, 1, 0);
				sharelist = ostrcat(sharelist, " (", 1, 0);
				sharelist = ostrcat(sharelist, (&ret6[0])->part, 1, 0);
				sharelist = ostrcat(sharelist, ": /", 1, 0);
				sharelist = ostrcat(sharelist, tmp_sharedir, 1, 0);
				sharelist = ostrcat(sharelist, ")\n", 1, 0);
				debug(70, "sharelist: (%d) %s", i, sharelist);

				free(ret6); ret6 = NULL;
				if((mode == 3) && (ostrcmp((&ret2[0])->part, input_sharename) == 0)){
					if(ostrcmp((&ret2[0])->part, hddreplacement_ck) == 0){
						tmp_hddreplacement = ostrcat("", tmp_hddreplacement, 0, 1);
						tmp_hddreplacement = ostrcat(tmp_hddreplacement, "1", 1, 0);
					} else {
						tmp_hddreplacement = ostrcat("", tmp_hddreplacement, 0, 1);
						tmp_hddreplacement = ostrcat(tmp_hddreplacement, "0", 1, 0);
					}
				

					screennetworkbrowser_addshare(1, tmp_sharename, tmp_ipaddresse, tmp_sharedir, NULL, NULL, tmp_rsize, tmp_wsize, tmp_protocol, tmp_options, tmp_hddreplacement, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);

					save = 0;
					continue;
				}

			} else if(ostrcmp((&ret3[0])->part, "-fstype=ftpfs") == 0){
				tmp_userauth = ostrcat("", "0", 0, 0);
				tmp_proxyauth = ostrcat("", "0", 0, 0);
				tmp_useproxy = ostrcat("", "0", 0, 0);
				tmp_usessl = ostrcat("", "0", 0, 0);

				tmp_options = ostrcat("", (&ret3[1])->part, 0, 0);
				tmp_options = ostrcat(tmp_options, ",", 1, 0);
				tmp_options = ostrcat(tmp_options, (&ret3[2])->part, 0, 0);

				if(ostrcmp((&ret3[3])->part, "allow_other") == 0){
					tmp_options1 = ostrcat("", (&ret3[3])->part, 0, 0);

					tmp_options2 = ostrcat("", (&ret3[4])->part, 0, 0);
					if(string_find(",nonempty,ssl_try",(&ret2[1])->part)){
						tmp_ssl = ostrcat("", (&ret3[5])->part, 0, 0);
						tmp_usessl = ostrcat("", "1", 0, 0);
					} else if(string_find(",nonempty,ssl_control",(&ret2[1])->part)){
						tmp_ssl = ostrcat("", (&ret3[5])->part, 0, 0);
						tmp_usessl = ostrcat("", "1", 0, 0);
					} else if(string_find(",nonempty,ssl",(&ret2[1])->part)){
						tmp_ssl = ostrcat("", (&ret3[5])->part, 0, 0);
						tmp_usessl = ostrcat("", "1", 0, 0);
					}
				} else {
					tmp_useproxy = ostrcat("", "1", 0, 0);
					struct splitstr* ret4 = NULL;
					int count4 = 0;
					tmpstr4 = ostrcat("", (&ret3[3])->part, 0, 0);
					ret4 = strsplit(tmpstr4, "=", &count4);
					tmp_check = ostrcat("", (&ret4[1])->part, 0, 0);
					free(ret4); ret4 = NULL;

					struct splitstr* ret5 = NULL;
					int count5 = 0;
					tmpstr5 = ostrcat("", tmp_check, 0, 0);
					ret5 = strsplit(tmpstr5, "\\:", &count5);
					tmp_proxyip = ostrcat("", (&ret5[0])->part, 0, 0);
					tmp_proxyport = ostrcat("", (&ret5[1])->part, 0, 0);
					free(ret5); ret5 = NULL;

					tmp_options1 = ostrcat("", (&ret3[4])->part, 0, 0);
				
					if(ostrcmp((&ret3[5])->part, "nonempty") == 0){
						tmp_options2 = ostrcat("", (&ret3[4])->part, 0, 0);
						if(string_find(",nonempty,ssl_try",(&ret2[1])->part)){
							tmp_ssl = ostrcat("", (&ret3[5])->part, 0, 0);
							tmp_usessl = ostrcat("", "1", 0, 0);
						} else if(string_find(",nonempty,ssl_control",(&ret2[1])->part)){
							tmp_ssl = ostrcat("", (&ret3[5])->part, 0, 0);
							tmp_usessl = ostrcat("", "1", 0, 0);
						} else if(string_find(",nonempty,ssl",(&ret2[1])->part)){
							tmp_ssl = ostrcat("", (&ret3[5])->part, 0, 0);
							tmp_usessl = ostrcat("", "1", 0, 0);
						}
					} else {
						tmp_proxy = ostrcat("", (&ret3[5])->part, 0, 0);
						if(ostrcmp((&ret3[6])->part, "nonempty") == 0){
							tmp_options2 = ostrcat("", (&ret3[6])->part, 0, 0);
							if(string_find(",nonempty,ssl_try",(&ret2[1])->part)){
								tmp_ssl = ostrcat("", (&ret3[7])->part, 0, 0);
								tmp_usessl = ostrcat("", "1", 0, 0); 
							} else if(string_find(",nonempty,ssl_control",(&ret2[1])->part)){
								tmp_ssl = ostrcat("", (&ret3[7])->part, 0, 0);
								tmp_usessl = ostrcat("", "1", 0, 0);
							} else if(string_find(",nonempty,ssl",(&ret2[1])->part)){
								tmp_ssl = ostrcat("", (&ret3[7])->part, 0, 0);
								tmp_usessl = ostrcat("", "1", 0, 0);
							}
						} else {
							tmp_proxyauth = ostrcat("", "1", 0, 0);

							struct splitstr* ret6 = NULL;
							int count6 = 0;
							tmpstr6 = ostrcat("", (&ret3[6])->part, 0, 0);
							ret6 = strsplit(tmpstr6, "=", &count6);
							tmp_check = ostrcat("", (&ret6[1])->part, 0, 0);
							free(ret6); ret6 = NULL;

							struct splitstr* ret7 = NULL;
							int count7 = 0;
							tmpstr7 = ostrcat("", tmp_check, 0, 0);
							ret7 = strsplit(tmpstr7, "\\:", &count7);
							tmp_proxyuser = ostrcat("", (&ret7[0])->part, 0, 0);
							tmp_proxypass = ostrcat("", (&ret7[1])->part, 0, 0);
							free(ret7); ret7 = NULL;
							tmp_options2 = ostrcat("", (&ret3[7])->part, 0, 0);

							if(string_find(",nonempty,ssl_try",(&ret2[1])->part)){
								tmp_ssl = ostrcat("", (&ret3[8])->part, 0, 0);
								tmp_usessl = ostrcat("", "1", 0, 0);
							} else if(string_find(",nonempty,ssl_control",(&ret2[1])->part)){
								tmp_ssl = ostrcat("", (&ret3[8])->part, 0, 0);
								tmp_usessl = ostrcat("", "1", 0, 0);
							} else if(string_find(",nonempty,ssl",(&ret2[1])->part)){
								tmp_ssl = ostrcat("", (&ret3[8])->part, 0, 0);
								tmp_usessl = ostrcat("", "1", 0, 0);
							}
						}
					}
				}

				if(string_find("@",(&ret2[2])->part)){
					tmp_userauth = ostrcat("", "1", 0, 0);
					debug(70, "foundf ftpuser with userauth");
					struct splitstr* ret8 = NULL;
					int count8 = 0;
					tmpstr8 = ostrcat("", (&ret2[2])->part, 0, 0);
					ret8 = strsplit(tmpstr8, "@", &count8);

					struct splitstr* ret9 = NULL;
					int count9 = 0;
					tmpstr9 = ostrcat("", (&ret8[0])->part, 0, 0);
					ret9 = strsplit(tmpstr9, "/\\:", &count9);
					tmp_user = ostrcat("", (&ret9[1])->part, 0, 0);
					tmp_pass = ostrcat("", (&ret9[2])->part, 0, 0);

					tmp_check = ostrcat("", (&ret8[1])->part, 0, 0);
					struct splitstr* ret10 = NULL;
					int count10 = 0;
					tmpstr10 = ostrcat("", tmp_check, 0, 0);
					ret10 = strsplit(tmpstr10, "\\:", &count10);
					tmp_ipaddresse = ostrcat("", (&ret10[0])->part, 0, 0);

					struct splitstr* ret11 = NULL;
					int count11 = 0;
					tmpstr11 = ostrcat("", (&ret10[1])->part, 0, 0);
					ret11 = strsplit(tmpstr11, "/", &count11);
					tmp_ftpport = ostrcat("", (&ret11[0])->part, 0, 0);
					tmp_sharedir = ostrcat("", (&ret11[1])->part, 0, 0);

					free(ret9); ret9 = NULL;
					free(ret10); ret10 = NULL;
					free(ret11); ret11 = NULL;
				} else {
					debug(70, "foundf ftpuser without userauth");
					struct splitstr* ret12 = NULL;
					int count12 = 0;
					tmpstr12 = ostrcat("", (&ret2[2])->part, 0, 0);
					ret12= strsplit(tmpstr12, "/\\:", &count12);

					tmp_ipaddresse = ostrcat("", (&ret12[1])->part, 0, 0);
					tmp_ftpport = ostrcat("", (&ret12[2])->part, 0, 0);
					tmp_sharedir = ostrcat("", (&ret12[3])->part, 0, 0);
					free(ret12); ret12 = NULL;
				}

				tmp_sharename = ostrcat("", (&ret2[0])->part, 0, 0);

				if(ostrcmp((&ret2[0])->part, input_sharename) == 0){
					update = 0;
				} else {
					outline = ostrcat(outline, (&ret1[i])->part, 1, 0);
					outline = ostrcat(outline, "\n", 1, 0);
				}

				sharelist = ostrcat(sharelist, "(ftpfs) ", 1, 0);
				sharelist = ostrcat(sharelist, (&ret2[0])->part, 1, 0);
				sharelist = ostrcat(sharelist, " (", 1, 0);
				sharelist = ostrcat(sharelist, tmp_ipaddresse, 1, 0);
				sharelist = ostrcat(sharelist, ": /", 1, 0);
				sharelist = ostrcat(sharelist, tmp_sharedir, 1, 0);
				sharelist = ostrcat(sharelist, ")\n", 1, 0);
				debug(70, "sharelist: (%d) %s", i, sharelist);

				if((mode == 3) && (ostrcmp((&ret2[0])->part, input_sharename) == 0)){
					if(ostrcmp((&ret2[0])->part, hddreplacement_ck) == 0){
						tmp_hddreplacement = ostrcat("", tmp_hddreplacement, 0, 1);
						tmp_hddreplacement = ostrcat(tmp_hddreplacement, "1", 1, 0);
					} else {
						tmp_hddreplacement = ostrcat("", tmp_hddreplacement, 0, 1);
						tmp_hddreplacement = ostrcat(tmp_hddreplacement, "0", 1, 0);
					}
					screennetworkbrowser_addshare(2, tmp_sharename, tmp_ipaddresse, tmp_sharedir, tmp_user, tmp_pass, NULL, NULL, NULL, tmp_options, tmp_hddreplacement, tmp_options1, tmp_options2, tmp_ssl, tmp_proxy, tmp_proxyip, tmp_proxyport, tmp_proxyuser, tmp_proxypass, tmp_ftpport, tmp_userauth, tmp_proxyauth, tmp_useproxy, tmp_usessl);

					save = 0;
					continue;
				}

			} else {
				outline = ostrcat(outline, (&ret1[i])->part, 1, 0);
				outline = ostrcat(outline, "\n", 1, 0);

			}

			free(ret3); ret3 = NULL;	
		}

		free(ret2); ret2 = NULL;
	}

	free(ret1); ret1 = NULL;

	if(mode == 0){
		char* skinname = "networkbrowser_shares";
		char* skintitle = _("Networkbrowser - show Shares");
		menu_sharelist = networkbrowser_listbox(NULL, sharelist, skinname, skintitle, "%pluginpath%/networkbrowser/skin/", 4);
		if(menu_sharelist != NULL){
			if(ostrcmp(menu_sharelist, text1) == 0){

				debug(70, "addshare new");
				screennetworkbrowser_addshare(0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
				save = 0;
			} else {
				menu_changeshare = networkbrowser_listbox(NULL, changeshare, skinname, skintitle, "%pluginpath%/networkbrowser/skin/", 1);
				if(menu_changeshare != NULL){
					struct splitstr* ret9 = NULL;
					int count9 = 0;
					tmpstr9 = NULL;
					tmpstr9 = ostrcat(tmpstr9, menu_sharelist, 1, 0);
					ret9 = strsplit(tmpstr9, " ", &count9);
					tmp_sharename = ostrcat("", (&ret9[1])->part, 0, 0);
					free(ret9); ret9 = NULL;
					if(ostrcmp(menu_changeshare, text5) == 0){
						//char* check = screennetworkbrowser_readshare(2, tmp_sharename, NULL);
						debug(70, "start readshare edit: %s", tmp_sharename);
						screennetworkbrowser_readshare(3, tmp_sharename, NULL);
						save = 0;
					} else if(ostrcmp(menu_changeshare, text6) == 0){
						debug(70, "start readshare remove: %s", tmp_sharename);
						screennetworkbrowser_readshare(2, tmp_sharename, NULL);
						save = 0;
					}
				}
			}
			
		}
	}

	debug(70, "vor save input_shareline: %s mode=%d update=%d", input_shareline, mode, update);
	if(((mode == 1) && (update == 1)) || ((mode == 5) && (update == 1))){
		debug(70, "add new share input_shareline: %s", input_shareline);
		outline = ostrcat(outline, input_shareline, 1, 0);
		outline = ostrcat(outline, "\n", 1, 0);
		mode = 4;
	}

	if(save == 1){
		debug(70, "save auto.misc");

		FILE* fd = fopen("/var/etc/automount/auto.misc", "w");
		if(fd) {
			debug(70, "save outline: %s",outline);
			fprintf(fd, "%s\n", outline);
			debug(70, "start hotplug.sh first");
			system("/sbin/hotplug.sh first &");
			fclose(fd);
		}	
	}

	free(tmp_sharedir_cifs); tmp_sharedir_cifs = NULL;
	free(tmp_sharedir_nfs); tmp_sharedir_nfs = NULL;

	free(menu_sharetypes); menu_sharetypes = NULL;
	free(menu_sharelist); menu_sharelist = NULL;
	free(menu_changeshare); menu_changeshare = NULL;

	free(sharelist); sharelist = NULL;
	free(changeshare); changeshare = NULL;
	free(sharetypes); sharetypes = NULL;

	free(tmp_sharename); tmp_sharename = NULL;
	free(tmp_sharedir); tmp_sharedir = NULL;
	free(tmp_sharedirlong); tmp_sharedirlong = NULL;
	free(tmp_ipaddresse); tmp_ipaddresse = NULL;
	free(tmp_user); tmp_user = NULL;
	free(tmp_pass); tmp_pass = NULL;
	free(tmp_sharename); tmp_sharename = NULL;
	free(tmp_protocol); tmp_protocol = NULL;
	free(tmp_options); tmp_options = NULL;
	free(tmp_options1); tmp_options1 = NULL;
	free(tmp_options2); tmp_options2 = NULL;
	free(tmp_check); tmp_check = NULL;
	free(tmp_ftpport); tmp_ftpport = NULL;
	free(tmp_proxy); tmp_proxy = NULL;
	free(tmp_proxyip); tmp_proxyip = NULL;
	free(tmp_proxyport); tmp_proxyport = NULL;
	free(tmp_proxyuser); tmp_proxyuser = NULL;
	free(tmp_proxypass); tmp_proxypass = NULL;
	free(tmp_ssl); tmp_ssl = NULL;
	free(tmp_proxyauth); tmp_proxyauth = NULL;
	free(tmp_userauth); tmp_userauth = NULL;
	free(tmp_useproxy); tmp_useproxy = NULL;
	free(tmp_usessl); tmp_usessl = NULL;
	free(tmp_hddreplacement); tmp_hddreplacement = NULL;

	free(text1); text1 = NULL;
	free(text2); text2 = NULL;
	free(text3); text3 = NULL;
	free(text4); text4 = NULL;
	free(text5); text5 = NULL;
	free(text6); text6 = NULL;

	free(outline); outline = NULL;
	free(tmpstr1); tmpstr1 = NULL;
	free(tmpstr2); tmpstr2 = NULL;
	free(tmpstr3); tmpstr3 = NULL;
	free(tmpstr4); tmpstr4 = NULL;
	free(tmpstr5); tmpstr5 = NULL;
	free(tmpstr6); tmpstr6 = NULL;
	free(tmpstr7); tmpstr7 = NULL;
	free(tmpstr8); tmpstr8 = NULL;
	free(tmpstr9); tmpstr9 = NULL;
	free(tmpstr10); tmpstr10 = NULL;
	free(tmpstr11); tmpstr11 = NULL;
	free(tmpstr12); tmpstr12 = NULL;
	free(tmpstr13); tmpstr13 = NULL;

	if((mode == 4) || (mode == 5)){
		debug(70, "start readshare list: mode=%d", mode);
		screennetworkbrowser_readshare(0, NULL, NULL);
	}
	status.hangtime = getconfigint("hangtime", NULL);
}

#endif
