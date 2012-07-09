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

extern struct inetwork* inetwork;

struct networkbrowser
{
	//mode 0 = cifs	
	//mode 1 = nfs
	//mode 2 = ftp
	char* mode;
	char* sharename;
	char* ip;
	char* sharedir;
	char* username;
	char* password;
	char* rsize;
	char* wsize;
	char* protocol;
	char* options;
	char* ssl;
	char* proxy;
	char* proxyip;
	char* proxyport;
	char* proxyuser;
	char* proxypass;
	char* ftpport;
	char* userauth;
	char* proxyauth;
	char* useproxy;
	char* usessl;
	struct networkbrowser* next;
};

struct networkbrowser *networkbrowser = NULL;

char* readnetworkbrowser(char* filename, int flag);
void screennetworkbrowser_addshare(struct networkbrowser* node, int newnode);

void debugnetworkbrowser(struct networkbrowser* node)
{
	if(node == NULL) return;
	
	debug(70, "mode: %s", node->mode);
	debug(70, "sharename: %s", node->sharename);
	debug(70, "ip: %s", node->ip);
	debug(70, "sharedir: %s", node->sharedir);
	debug(70, "username: %s", node->username);
	debug(70, "password: %s", node->password);
	debug(70, "rsize: %s", node->rsize);
	debug(70, "wsize: %s", node->wsize);
	debug(70, "protocol: %s", node->protocol);
	debug(70, "options: %s", node->options);
	debug(70, "ssl: %s", node->ssl);
	debug(70, "proxy: %s", node->proxy);
	debug(70, "proxyip: %s", node->proxyip);
	debug(70, "proxyport: %s", node->proxyport);
	debug(70, "proxyuser: %s", node->proxyuser);
	debug(70, "proxypass: %s", node->proxypass);
	debug(70, "ftpport: %s", node->ftpport);
	debug(70, "userauth: %s", node->userauth);
	debug(70, "proxyauth: %s", node->proxyauth);
	debug(70, "useproxy: %s", node->useproxy);
	debug(70, "usessl: %s", node->usessl);
}

void delnetworkbrowsercontent(struct networkbrowser* node)
{
	if(node == NULL) return;
	
	free(node->mode); node->mode = NULL;
	free(node->sharename); node->sharename = NULL;
	free(node->ip); node->ip = NULL;
	free(node->sharedir); node->sharedir = NULL;
	free(node->username); node->username = NULL;
	free(node->password); node->password = NULL;
	free(node->rsize); node->rsize = NULL;
	free(node->wsize); node->wsize = NULL;
	free(node->protocol); node->protocol = NULL;
	free(node->options); node->options = NULL;
	free(node->ssl); node->ssl = NULL;
	free(node->proxy); node->proxy = NULL;
	free(node->proxyip); node->proxyip = NULL;
	free(node->proxyport); node->proxyport = NULL;
	free(node->proxyuser); node->proxyuser = NULL;
	free(node->proxypass); node->proxypass = NULL;
	free(node->ftpport); node->ftpport = NULL;
	free(node->userauth); node->userauth = NULL;
	free(node->proxyauth); node->proxyauth = NULL;
	free(node->useproxy); node->useproxy = NULL;
	free(node->usessl); node->usessl = NULL;
}

void delnetworkbrowser(struct networkbrowser* netnode)
{
	struct networkbrowser *node = networkbrowser, *prev = networkbrowser;

	while(node != NULL)
	{
		if(node == netnode)
		{
			if(node == networkbrowser)
				networkbrowser = node->next;
			else
				prev->next = node->next;

			delnetworkbrowsercontent(node);

			free(node);
			node = NULL;
			break;
		}

		prev = node;
		node = node->next;
	}
}

void freenetworkbrowser()
{
	struct networkbrowser *node = networkbrowser, *prev = networkbrowser;

	while(node != NULL)
	{
		prev = node;
		node = node->next;
		if(prev != NULL)
			delnetworkbrowser(prev);
	}
}

struct networkbrowser* addnetworkbrowser(char *line, int count, struct networkbrowser* last)
{
	struct networkbrowser *newnode = NULL, *prev = NULL, *node = networkbrowser;
	int ret = 0, treffer = 0;
	char* tmpstr = NULL;

	newnode = (struct networkbrowser*)malloc(sizeof(struct networkbrowser));	
	if(newnode == NULL)
	{
		err("no memory");
		return NULL;
	}
	memset(newnode, 0, sizeof(struct networkbrowser));

	if(line != NULL)
	{
		newnode->sharename = malloc(256);
		if(newnode->sharename == NULL)
		{
			err("no memory");
			delnetworkbrowsercontent(newnode);
			free(newnode);
			return NULL;
		}
		memset(newnode->sharename, 0, 256);

		newnode->ip = malloc(256);
		if(newnode->ip == NULL)
		{
			err("no memory");
			delnetworkbrowsercontent(newnode);
			free(newnode);
			return NULL;
		}
		memset(newnode->ip, 0, 256);

		newnode->sharedir = malloc(256);
		if(newnode->sharedir == NULL)
		{
			err("no memory");
			delnetworkbrowsercontent(newnode);
			free(newnode);
			return NULL;
		}
		memset(newnode->sharedir, 0, 256);

		newnode->username = malloc(256);
		if(newnode->username == NULL)
		{
			err("no memory");
			delnetworkbrowsercontent(newnode);
			free(newnode);
			return NULL;
		}
		memset(newnode->username, 0, 256);

		newnode->password = malloc(256);
		if(newnode->password == NULL)
		{
			err("no memory");
			delnetworkbrowsercontent(newnode);
			free(newnode);
			return NULL;
		}
		memset(newnode->password, 0, 256);

		newnode->rsize = malloc(256);
		if(newnode->rsize == NULL)
		{
			err("no memory");
			delnetworkbrowsercontent(newnode);
			free(newnode);
			return NULL;
		}
		memset(newnode->rsize, 0, 256);

		newnode->wsize = malloc(256);
		if(newnode->wsize == NULL)
		{
			err("no memory");
			delnetworkbrowsercontent(newnode);
			free(newnode);
			return NULL;
		}
		memset(newnode->wsize, 0, 256);

		newnode->protocol = malloc(256);
		if(newnode->protocol == NULL)
		{
			err("no memory");
			delnetworkbrowsercontent(newnode);
			free(newnode);
			return NULL;
		}
		memset(newnode->protocol, 0, 256);

		newnode->options = malloc(256);
		if(newnode->options == NULL)
		{
			err("no memory");
			delnetworkbrowsercontent(newnode);
			free(newnode);
			return NULL;
		}
		memset(newnode->options, 0, 256);

		newnode->ssl = malloc(256);
		if(newnode->ssl == NULL)
		{
			err("no memory");
			delnetworkbrowsercontent(newnode);
			free(newnode);
			return NULL;
		}
		memset(newnode->ssl, 0, 256);

		newnode->proxy = malloc(256);
		if(newnode->proxy == NULL)
		{
			err("no memory");
			delnetworkbrowsercontent(newnode);
			free(newnode);
			return NULL;
		}
		memset(newnode->proxy, 0, 256);

		newnode->proxyip = malloc(256);
		if(newnode->proxyip == NULL)
		{
			err("no memory");
			delnetworkbrowsercontent(newnode);
			free(newnode);
			return NULL;
		}
		memset(newnode->proxyip, 0, 256);

		newnode->proxyport = malloc(256);
		if(newnode->proxyport == NULL)
		{
			err("no memory");
			delnetworkbrowsercontent(newnode);
			free(newnode);
			return NULL;
		}
		memset(newnode->proxyport, 0, 256);

		newnode->proxyuser = malloc(256);
		if(newnode->proxyuser == NULL)
		{
			err("no memory");
			delnetworkbrowsercontent(newnode);
			free(newnode);
			return NULL;
		}
		memset(newnode->proxyuser, 0, 256);

		newnode->proxypass = malloc(256);
		if(newnode->proxypass == NULL)
		{
			err("no memory");
			delnetworkbrowsercontent(newnode);
			free(newnode);
			return NULL;
		}
		memset(newnode->proxypass, 0, 256);

		newnode->ftpport = malloc(256);
		if(newnode->ftpport == NULL)
		{
			err("no memory");
			delnetworkbrowsercontent(newnode);
			free(newnode);
			return NULL;
		}
		memset(newnode->ftpport, 0, 256);

		newnode->userauth = ostrcat(newnode->userauth, "0", 1, 0);
		newnode->proxyauth = ostrcat(newnode->proxyauth, "0", 1, 0);
		newnode->useproxy = ostrcat(newnode->useproxy, "0", 1, 0);
		newnode->usessl = ostrcat(newnode->usessl, "0", 1, 0);

		//nfs parse
		if(ostrstr(line, "-fstype=nfs") != NULL)
		{
			treffer = 1;
			newnode->mode = ostrcat("1", NULL, 0, 0);
			ret = sscanf(line, "%s\t-fstype=nfs,%[^,],%*[^,],rsize=%[^,],wsize=%[^,],%s\t%[^:]://%s", newnode->sharename, newnode->options, newnode->rsize, newnode->wsize, newnode->protocol, newnode->ip, newnode->sharedir);
			if(ret != 7)
			{
				if(count > 0)
				{
					err("networkbrowser nfs line %d not ok", count);
				}
				else
				{
					err("add networkbrowser nfs");
				}
				delnetworkbrowsercontent(newnode);
				free(newnode);
				return NULL;
			}
		}

		//cifs parse
		if(ostrstr(line, "-fstype=cifs") != NULL)
		{
			treffer = 1;
			newnode->mode = ostrcat("0", NULL, 0, 0);
			if(ostrstr(line, "user=,") != NULL)
			{
				treffer = 6;
				ret = sscanf(line, "%s\t-fstype=cifs,%[^,],rsize=%[^,],wsize=%[^,],%*s\t://%[^/]/%s", newnode->sharename, newnode->options, newnode->rsize, newnode->wsize, newnode->ip, newnode->sharedir);
			}
			else
			{
				treffer = 8;
				free(newnode->userauth); newnode->userauth = NULL;
				newnode->userauth = ostrcat(newnode->userauth, "1", 1, 0);
				ret = sscanf(line, "%s\t-fstype=cifs,%[^,],rsize=%[^,],wsize=%[^,],user=%[^,],pass=%s\t://%[^/]/%s", newnode->sharename, newnode->options, newnode->rsize, newnode->wsize, newnode->username, newnode->password, newnode->ip, newnode->sharedir);
			}
			if(ret != treffer)
			{
				if(count > 0)
				{
					err("networkbrowser cifs line %d not ok", count);
				}
				else
				{
					err("add networkbrowser cifs");
				}
				delnetworkbrowsercontent(newnode);
				free(newnode);
				return NULL;
			}
		}

		//ftpfs parse
		if(ostrstr(line, "-fstype=ftpfs") != NULL)
		{
			treffer = 1;
			newnode->mode = ostrcat("2", NULL, 0, 0);

			ret = sscanf(line, "%s\t-fstype=ftpfs,%[^,\t]", newnode->sharename, newnode->options);
			if(ret != 2) treffer = 0;

			tmpstr = ostrstr(line, "ssl");
			if(tmpstr != NULL)
			{
				free(newnode->usessl); newnode->usessl = NULL;
				newnode->usessl = ostrcat(newnode->usessl, "1", 1, 0);
				ret = sscanf(tmpstr, "%[^,\t]", newnode->ssl);
				if(ret != 1) treffer = 0;
			}

			tmpstr = ostrstr(line, "proxy=");
			if(tmpstr != NULL)
			{
				free(newnode->useproxy); newnode->useproxy = NULL;
				newnode->useproxy = ostrcat(newnode->useproxy, "1", 1, 0);
				ret = sscanf(tmpstr, "proxy=%[^\\]\\:%[^,],%[^,\t]", newnode->proxyip, newnode->proxyport, newnode->proxy);
				if(ret != 3) treffer = 0;

				tmpstr = ostrstr(line, "proxy_user=");
				if(tmpstr != NULL)
				{
					free(newnode->proxyauth); newnode->proxyauth = NULL;
					newnode->proxyauth = ostrcat(newnode->proxyauth, "1", 1, 0);
					ret = sscanf(tmpstr, "proxy_user=%[^\\]\\:%[^,\t]", newnode->proxyuser, newnode->proxypass);
					if(ret != 2) treffer = 0;
				}
			}

			tmpstr = ostrstr(line, ":ftp\\://");
			if(tmpstr != NULL)
			{
				if(ostrstr(line, "@") != NULL)
				{
					free(newnode->userauth); newnode->userauth = NULL;
					newnode->userauth = ostrcat(newnode->userauth, "1", 1, 0);
					ret = sscanf(tmpstr, ":ftp\\://%[^\\]\\:%[^@]@%[^\\]\\:%s", newnode->username, newnode->password, newnode->ip, newnode->ftpport);
					if(ret != 5) treffer = 0;
				}
				else
				{
					ret = sscanf(tmpstr, ":ftp\\://%[^\\]\\:%s", newnode->ip, newnode->ftpport);
					if(ret != 3) treffer = 0;
				}
			}
			else
				treffer = 0;
        
      if(newnode->ftpport != NULL)
      {
        tmpstr = ostrstr(newnode->ftpport, "/");
        if(tmpstr != NULL)
        {
          tmpstr[0] = '\0';
          free(newnode->sharedir);
          newnode->sharedir = ostrcat(tmpstr + 1, NULL, 0, 0);               
        }
      }

			if(treffer == 0)
			{
				if(count > 0)
				{
					err("networkbrowser ftpfs line %d not ok", count);
				}
				else
				{
					err("add networkbrowser ftpfs");
				}
				delnetworkbrowsercontent(newnode);
				free(newnode);
				return NULL;
			}
		}

		if(treffer == 0)
		{
			delnetworkbrowsercontent(newnode);
			free(newnode);
			return NULL;
		}
	}
	
	if(last == NULL)
	{
		while(node != NULL)
		{
			prev = node;
			node = node->next;
		}
	}
	else
	{
		prev = last;
		node = last->next;
	}

	if(prev == NULL)
		networkbrowser = newnode;
	else
		prev->next = newnode;

	newnode->next = node;
	
	return newnode;
}

void setdefaultnetworkbrowser(struct networkbrowser* node)
{
	if(node == NULL) return;
	
	if(node->mode == NULL || node->mode[0] == '\0') node->mode = ostrcat(node->mode, "0", 1, 0);
	if(node->sharename == NULL || node->sharename[0] == '\0') node->sharename = ostrcat(node->sharename, "SHARENAME", 1, 0);
	if(node->ip == NULL || node->ip[0] == '\0') node->ip = ostrcat(node->ip, "192.168.178.1", 1, 0);
	if(node->sharedir == NULL || node->sharedir[0] == '\0') node->sharedir = ostrcat(node->sharedir, "sharedir", 1, 0);
	if(node->username == NULL || node->username[0] == '\0') node->username = ostrcat(node->username, "username", 1, 0);
	if(node->password == NULL || node->password[0] == '\0') node->password = ostrcat(node->password, "password", 1, 0);
	if(node->rsize == NULL || node->rsize[0] == '\0') node->rsize = ostrcat(node->rsize, "32768", 1, 0);
	if(node->wsize == NULL || node->wsize[0] == '\0') node->wsize = ostrcat(node->wsize, "32768", 1, 0);
	if(node->protocol == NULL || node->protocol[0] == '\0') node->protocol = ostrcat(node->protocol, "udp", 1, 0);
	if(node->options == NULL || node->options[0] == '\0') node->options = ostrcat(node->options, "rw", 1, 0);
	if(node->ssl == NULL || node->ssl[0] == '\0') node->ssl = ostrcat(node->ssl, "ssl_try", 1, 0);
	if(node->proxy == NULL || node->proxy[0] == '\0') node->proxy = ostrcat(node->proxy, "socks5", 1, 0);
	if(node->proxyip == NULL || node->proxyip[0] == '\0') node->proxyip = ostrcat(node->proxyip, "12.23.45.67", 1, 0);
	if(node->proxyport == NULL || node->proxyport[0] == '\0') node->proxyport = ostrcat(node->proxyport, "8080", 1, 0);
	if(node->proxyuser == NULL || node->proxyuser[0] == '\0') node->proxyuser = ostrcat(node->proxyuser, "proxyuser", 1, 0);
	if(node->proxypass == NULL || node->proxypass[0] == '\0') node->proxypass = ostrcat(node->proxypass, "proxypass", 1, 0);
	if(node->ftpport == NULL || node->ftpport[0] == '\0') node->ftpport = ostrcat(node->ftpport, "21", 1, 0);
	if(node->userauth == NULL || node->userauth[0] == '\0') node->userauth = ostrcat(node->userauth, "0", 1, 0);
	if(node->proxyauth == NULL || node->proxyauth[0] == '\0') node->proxyauth = ostrcat(node->proxyauth, "0", 1, 0);
	if(node->useproxy == NULL || node->useproxy[0] == '\0') node->useproxy = ostrcat(node->useproxy, "0", 1, 0);
	if(node->usessl == NULL || node->usessl[0] == '\0') node->usessl = ostrcat(node->usessl, "0", 1, 0);
}

void savenetworkbrowser(char* filename)
{
	struct networkbrowser* node = networkbrowser;
	char* tmpstr = NULL, *savesettings = NULL;

	if(filename == NULL) return;

	savesettings = readnetworkbrowser("/var/etc/automount/auto.misc", 1);

	while(node != NULL)
	{
 		savesettings = ostrcat(savesettings, node->sharename, 1, 0);

		if(ostrcmp(node->mode, "0") == 0)
		{
			savesettings = ostrcat(savesettings, "\t-fstype=cifs,rw,rsize=", 1, 0);
	 		savesettings = ostrcat(savesettings, node->rsize, 1, 0);
			savesettings = ostrcat(savesettings, ",wsize=", 1, 0);
 			savesettings = ostrcat(savesettings, node->wsize, 1, 0);
 			
 			if(ostrcmp(node->userauth, "1") == 0)
			{
				savesettings = ostrcat(savesettings, ",user=", 1, 0); 
				savesettings = ostrcat(savesettings, node->username, 1, 0);
				savesettings = ostrcat(savesettings, ",pass=", 1, 0);
 				savesettings = ostrcat(savesettings, node->password, 1, 0);
			}
			else
				savesettings = ostrcat(savesettings, ",user=,pass=", 1, 0); 
 			
			savesettings = ostrcat(savesettings, "\t://", 1, 0);
			tmpstr = fixip(node->ip, 1);
	 		savesettings = ostrcat(savesettings, tmpstr, 1, 0);
	 		free(tmpstr); tmpstr = NULL;
			savesettings = ostrcat(savesettings, "/", 1, 0);
			savesettings = ostrcat(savesettings, node->sharedir, 1, 0);
			savesettings = ostrcat(savesettings, "\n", 1, 0);
		}
		else if(ostrcmp(node->mode, "1") == 0)
		{
			savesettings = ostrcat(savesettings, "\t-fstype=nfs,", 1, 0);
 			savesettings = ostrcat(savesettings, node->options, 1, 0);
 			savesettings = ostrcat(savesettings, ",nolock", 1, 0);
			savesettings = ostrcat(savesettings, ",rsize=", 1, 0);
	 		savesettings = ostrcat(savesettings, node->rsize, 1, 0);
			savesettings = ostrcat(savesettings, ",wsize=", 1, 0);
 			savesettings = ostrcat(savesettings, node->wsize, 1, 0);
			savesettings = ostrcat(savesettings, ",", 1, 0); 
			savesettings = ostrcat(savesettings, node->protocol, 1, 0);
			savesettings = ostrcat(savesettings, "\t", 1, 0);
			tmpstr = fixip(node->ip, 1);
 			savesettings = ostrcat(savesettings, fixip(node->ip, 1), 1, 0);
 			free(tmpstr); tmpstr = NULL;
			savesettings = ostrcat(savesettings, "://", 1, 0);
 			savesettings = ostrcat(savesettings, node->sharedir, 1, 0);
			savesettings = ostrcat(savesettings, "\n", 1, 0);
		}
		else if(ostrcmp(node->mode, "2") == 0)
		{
			savesettings = ostrcat(savesettings, "\t-fstype=ftpfs,", 1, 0);
 			savesettings = ostrcat(savesettings, node->options, 1, 0);
 			savesettings = ostrcat(savesettings, ",nolock,allow_other,nonempty,nodev,noatime,kernel_cache,auto_cache,disable_epsv,skip_pasv_ip,no_verify_peer,no_verify_hostname", 1, 0);
			if(ostrcmp(node->usessl, "1") == 0)
			{
				savesettings = ostrcat(savesettings, ",", 1, 0);
		 		savesettings = ostrcat(savesettings, node->ssl, 1, 0);
			}

			if(ostrcmp(node->useproxy, "1") == 0)
			{
				savesettings = ostrcat(savesettings, ",proxy=", 1, 0);
				tmpstr = fixip(node->proxyip, 1);
		 		savesettings = ostrcat(savesettings, tmpstr, 1, 0);
		 		free(tmpstr); tmpstr = NULL;
				savesettings = ostrcat(savesettings, "\\:", 1, 0);
	 			savesettings = ostrcat(savesettings, node->proxyport, 1, 0);
			}
			if(ostrcmp(node->useproxy, "1") == 0)
			{
				savesettings = ostrcat(savesettings, ",", 1, 0);
		 		savesettings = ostrcat(savesettings, node->proxy, 1, 0);
			}

			if(ostrcmp(node->proxyauth, "1") == 0)
			{
				savesettings = ostrcat(savesettings, ",proxy_user=", 1, 0);
		 		savesettings = ostrcat(savesettings, node->proxyuser, 1, 0);
				savesettings = ostrcat(savesettings, "\\:", 1, 0);
	 			savesettings = ostrcat(savesettings, node->proxypass, 1, 0);
			}

			savesettings = ostrcat(savesettings, "\t:ftp\\://", 1, 0);
			if(ostrcmp(node->userauth, "1") == 0)
			{
		 		savesettings = ostrcat(savesettings, node->username, 1, 0);
				savesettings = ostrcat(savesettings, "\\:", 1, 0);
	 			savesettings = ostrcat(savesettings, node->password, 1, 0);
				savesettings = ostrcat(savesettings, "@", 1, 0);
			}

			tmpstr = fixip(node->ip, 1);
 			savesettings = ostrcat(savesettings, fixip(tmpstr, 1), 1, 0);
 			free(tmpstr); tmpstr = NULL;
			savesettings = ostrcat(savesettings, "\\:", 1, 0);
 			savesettings = ostrcat(savesettings, node->ftpport, 1, 0);
      
      if(ostrcmp(node->sharedir, "sharedir") != 0)
      {
        savesettings = ostrcat(savesettings, "/", 1, 0);
        savesettings = ostrcat(savesettings, node->sharedir, 1, 0);
      }
      
			savesettings = ostrcat(savesettings, "\n", 1, 0);
		}
		
		node = node->next;
	}

	if(savesettings != NULL)
	{
		writesys("/var/etc/automount/auto.misc", savesettings, 0);
		system("hotplug.sh first");
		system("rm -rf /mnt/automount ; mkdir /mnt/automount; cp -a /var/etc/automount/* /mnt/automount");
	}

	debugnetworkbrowser(node);
	debug(70, "savesettings: %s", savesettings);
	debug(70, "--------------------");

	free(savesettings); savesettings = NULL;
}

void copynetworkbrowser(struct networkbrowser* from, struct networkbrowser* to, char* sharename)
{
	if(from == NULL || to == NULL) return;

	to->mode = ostrcat(from->mode, NULL, 0, 0);
	to->sharename = ostrcat(sharename, NULL, 0, 0);
	to->ip = ostrcat(from->ip, NULL, 0, 0);
	to->sharedir = ostrcat(from->sharedir, NULL, 0, 0);
	to->username = ostrcat(from->username, NULL, 0, 0);
	to->password = ostrcat(from->password, NULL, 0, 0);
	to->rsize = ostrcat(from->rsize, NULL, 0, 0);
	to->wsize = ostrcat(from->wsize, NULL, 0, 0);
	to->protocol = ostrcat(from->protocol, NULL, 0, 0);
	to->options = ostrcat(from->options, NULL, 0, 0);
	to->ssl = ostrcat(from->ssl, NULL, 0, 0);
	to->proxy = ostrcat(from->proxy, NULL, 0, 0);
	to->proxyip = ostrcat(from->proxyip, NULL, 0, 0);
	to->proxyport = ostrcat(from->proxyport, NULL, 0, 0);
	to->proxyuser = ostrcat(from->proxyuser, NULL, 0, 0);
	to->proxypass = ostrcat(from->proxypass, NULL, 0, 0);
	to->ftpport = ostrcat(from->ftpport, NULL, 0, 0);
	to->userauth = ostrcat(from->userauth, NULL, 0, 0);
	to->proxyauth = ostrcat(from->proxyauth, NULL, 0, 0);
	to->useproxy = ostrcat(from->useproxy, NULL, 0, 0);
	to->usessl = ostrcat(from->usessl, NULL, 0, 0);
}

//flag 0: read and add to struct
//flag 1: read and build string
char* readnetworkbrowser(char* filename, int flag)
{
	FILE *fd = NULL;
	char *fileline = NULL, *tmpstr = NULL;
	int linecount = 0;

	fileline = malloc(MINMALLOC);
	if(fileline == NULL)
	{
		err("no memory");
		return NULL;
	}

	fd = fopen(filename, "r");
	if(fd == NULL)
	{
		perr("can't open %s", filename);
		free(fileline);
		return NULL;
	}

	while(fgets(fileline, MINMALLOC, fd) != NULL)
	{
		if(flag == 0)
		{
			if(fileline[0] == '#' || fileline[0] == '\n')
				continue;
			if(fileline[strlen(fileline) - 1] == '\n')
				fileline[strlen(fileline) - 1] = '\0';
			if(fileline[strlen(fileline) - 1] == '\r')
				fileline[strlen(fileline) - 1] = '\0';
	
			linecount++;
		
			if(fileline != NULL)
				addnetworkbrowser(fileline, linecount, NULL);
		}
		if(flag == 1)
		{
			if(ostrstr(fileline, "-fstype=cifs") != NULL) continue;
			if(ostrstr(fileline, "-fstype=nfs") != NULL) continue;
			if(ostrstr(fileline, "-fstype=ftpfs") != NULL) continue;
			tmpstr = ostrcat(tmpstr, fileline, 1, 0);
		}
	}

	free(fileline);
	fclose(fd);
	return tmpstr;
}

void getnetworkbrowser_dns(struct inetwork* net, struct menulist** mlist)
{
	int i = 0;
	netinfo* nInfo;
	char *tmpstr = NULL, *s = NULL;

	if(net == NULL || mlist == NULL) return;

	s = ostrcat(s, net->ip, 1, 0);
	s = ostrcat(s, "/24", 1, 0);
	debug(70, "start scanning: %s", s);
	
	nInfo = newNetInfo();
	netzInfo(s, nInfo);
	
	for(i = 0; i < 256; i++)
	{
		if(nInfo[i].ip[0] == '\0')
			break;
		else
		{
			debug(70, "service: %s", nInfo[i].service);
			debug(70, "ip: %s", nInfo[i].ip);
			debug(70, "name: %s", nInfo[i].name);
			debug(70, "mac: %s", nInfo[i].mac);
			tmpstr = ostrcat(tmpstr , strstrip(nInfo[i].domain), 1, 0);
			tmpstr = ostrcat(tmpstr , " (", 1, 0);

			tmpstr = ostrcat(tmpstr , strstrip(nInfo[i].ip), 1, 0);
			tmpstr = ostrcat(tmpstr , ")", 1, 0);
			struct menulist* tmpmlist = addmenulist(mlist, tmpstr, NULL, "netbrowser_scanshares.png", 0, 0);
			changemenulistparam(tmpmlist, nInfo[i].ip, nInfo[i].name);
			free(tmpstr); tmpstr = NULL;

		}
	}

	freeNetInfo(nInfo);
	free(s); s = NULL;
}

void getnetworkbrowser_cifs(struct menulist** mlist, char* s, char* r, char* u, char* p)
{
	int i = 0;
	char* tmpstr = NULL;
	shareinfo* sInfo;

	if(s == NULL || r == NULL || u == NULL || p == NULL || mlist == NULL) return;
	
	sInfo = newShareInfo();
	smbInfo(s, r, u, p, sInfo);
	
	for (i = 0; i < 128; i++)
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
			struct menulist* tmpmlist = addmenulist(mlist, tmpstr, NULL, "netbrowser_cifs.png", 0, 0);
			changemenulistparam(tmpmlist, sInfo[i].sharename, NULL);
			free(tmpstr); tmpstr = NULL;
		}
	}

	freeShareInfo(sInfo);
}

void  getnetworkbrowser_nfs(struct menulist** mlist, char* s, char* r)
{
	char* tmpstr = NULL, *tmpstr1 = NULL;
	nfsinfo* nfsInfo;
	int i = 0, err = 0;

	if(s == NULL || r == NULL || mlist == NULL) return;
	
	nfsInfo = newNfsInfo();
	err = showNfsShare(s, nfsInfo);
	
	if (err == 0)
	{
		for (i = 0; i < 256; i++)
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
				struct menulist* tmpmlist = addmenulist(mlist, tmpstr, NULL, "netbrowser_nfs.png", 0, 0);
        tmpstr1 = nfsInfo[i].share;
        if(tmpstr1 != NULL && strlen(tmpstr1) > 0) tmpstr1++;
				changemenulistparam(tmpmlist, tmpstr1, NULL);
				free(tmpstr); tmpstr = NULL;
			}
		}
	}

	freeNfsInfo(nfsInfo);
}

void screennetworkbrowser_scan()
{
	struct menulist* mlist = NULL, *mbox = NULL;
	struct menulist* mlist1 = NULL, *mbox1 = NULL;
	struct skin* net_scan = getscreen("networkbrowser_scan");
	struct skin* b1 = getscreennode(net_scan, "b1");
	struct skin* b2 = getscreennode(net_scan, "b2");
	struct inetwork *net = inetwork;
	
	status.hangtime = 99999;
	changetitle(net_scan, _("Scanning Network Shares, please wait !!"));
	b1->hidden = YES;
	b2->hidden = YES;

	net = inetwork;
	drawscreen(net_scan, 0, 0);

	while(net != NULL)
	{
		if(net->flag == 1)
			getnetworkbrowser_dns(net, &mlist);
		net = net->next;
	}

start:
	b1->hidden = NO;
	b2->hidden = NO;

	mbox = menulistbox(mlist, "networkbrowser_scan", _("Networkbrowser - show scanned DNS-Shares"), "%pluginpath%/networkbrowser/skin/", NULL, 1, 0);

	if(mbox != NULL)
	{
		getnetworkbrowser_cifs(&mlist1, mbox->param, mbox->param1, "user", "pass");
		getnetworkbrowser_nfs(&mlist1, mbox->param, mbox->param1);
start1:
		mbox1 = menulistboxext(mlist1, "networkbrowser_scan", _("Networkbrowser - show scanned cifs/nfs-Shares"), "%pluginpath%/networkbrowser/skin/", NULL, 1, NULL, 0);
		if(mbox1 != NULL && mbox->name != NULL)
		{
			if(ostrstr(mbox1->name, "(cifs)") != NULL)
			{
				debug(70, "start addshare cifs");
				struct networkbrowser* node = addnetworkbrowser(NULL, 1, NULL);
				if(node != NULL)
				{
					node->mode = ostrcat("0", NULL, 0, 0);
					node->ip = ostrcat(mbox->param, NULL, 0, 0);
					node->sharename = ostrcat(mbox1->param, NULL, 0, 0);
					node->sharedir = ostrcat(mbox1->param, NULL, 0, 0);
					screennetworkbrowser_addshare(node, 1);
				}
			}
			else if(ostrstr(mbox1->name, "(nfs)") != NULL)
			{
				debug(70, "start addshare nfs");
				struct networkbrowser* node = addnetworkbrowser(NULL, 1, NULL);
				if(node != NULL)
				{
					node->mode = ostrcat("1", NULL, 0, 0);
					node->ip = ostrcat(mbox->param, NULL, 0, 0);
					node->sharename = ostrcat(mbox1->param, NULL, 0, 0);
					node->sharedir = ostrcat(mbox1->param, NULL, 0, 0);
					screennetworkbrowser_addshare(node, 1);
				}
			}
			goto start1;
		}
		freemenulist(mlist1, 1); mlist1 = NULL;
		goto start;
	}

	freemenulist(mlist, 1); mlist = NULL;
	freemenulist(mlist1, 1); mlist1 = NULL;
	delownerrc(net_scan);
	clearscreen(net_scan);
	status.hangtime = getconfigint("hangtime", NULL);
}

void addhddreplacement(char* sharename)
{
	char* tmpstr = NULL;

	if(sharename == NULL) return;
	unlink("/media/hdd");

	tmpstr = ostrcat("/media/net/", sharename, 0, 0);
	symlink(tmpstr, "/media/hdd");
	free(tmpstr); tmpstr = NULL;

	writesys("/var/etc/automount/.recordshare", sharename, 0);
	system("/sbin/hotplug.sh first &");
}

void delhddreplacement()
{
	unlink("/media/hdd");
	unlink("/var/etc/automount/.recordshare");
	system("/sbin/hotplug.sh first &");
}

int checkhddreplacement(char* sharename)
{
	char* recordshare = readfiletomem("/var/etc/automount/.recordshare", 0);

	if(recordshare != NULL)
	{
		if(ostrcmp(sharename, recordshare) == 0)
		{
      free(recordshare); recordshare = NULL;
			return 1;
		}
  }

  free(recordshare); recordshare = NULL;
	return 0;
}

int checknetworkbrowserexist(struct networkbrowser* node)
{
	struct networkbrowser* tmpbrowser = networkbrowser;

	if(node == NULL) return 0;

	while(tmpbrowser != NULL)
	{
		if(tmpbrowser != node && ostrcmp(tmpbrowser->sharename, node->sharename) == 0)
			return 1;
		tmpbrowser = tmpbrowser->next;
	}

	return 0;
}

void changemodenetworkbrowser(struct networkbrowser* node, struct skin* net_addshare, struct skin* skin_username, struct skin* skin_password, struct skin* skin_protocol, struct skin* skin_rsize, struct skin* skin_wsize, struct skin* skin_options, struct skin* skin_ssl, struct skin* skin_proxy, struct skin* skin_proxyip, struct skin* skin_proxyport, struct skin* skin_proxyuser, struct skin* skin_proxypass, struct skin* skin_ftpport, struct skin* skin_userauth, struct skin* skin_proxyauth, struct skin* skin_useproxy, struct skin* skin_usessl)
{
	char* tmpstr = NULL;
	if(node == NULL) return;

	if(ostrcmp(node->mode, "0") == 0)
	{
		tmpstr = ostrcat(tmpstr, _("Add Cifs Network Share"), 1, 0);
		tmpstr = ostrcat(tmpstr, " (", 1, 0);
		tmpstr = ostrcat(tmpstr, strstrip(node->sharename), 1, 0);
		tmpstr = ostrcat(tmpstr, ")", 1, 0);
		changetitle(net_addshare, tmpstr);
		free(tmpstr); tmpstr = NULL;

		skin_protocol->hidden = YES;
		skin_rsize->hidden = NO;
		skin_wsize->hidden = NO;
		skin_options->hidden = YES;
		skin_ssl->hidden = YES;
		skin_proxy->hidden = YES;
		skin_proxyip->hidden = YES;
		skin_proxyport->hidden = YES;
		skin_proxyuser->hidden = YES;
		skin_proxypass->hidden = YES;
		skin_ftpport->hidden = YES;
		skin_userauth->hidden = NO;
		skin_proxyauth->hidden = YES;
		skin_useproxy->hidden = YES;
		skin_usessl->hidden = YES;
		
		if(ostrcmp(node->userauth, "0") == 0)
		{
			skin_username->hidden = YES;
			skin_password->hidden = YES;
		}
		else
		{
			skin_username->hidden = NO;
			skin_password->hidden = NO;
		}
	}
	else if(ostrcmp(node->mode, "1") == 0)
	{
		tmpstr = ostrcat(tmpstr, _("Add Nfs Network Share"), 1, 0);
		tmpstr = ostrcat(tmpstr, " (", 1, 0);
		tmpstr = ostrcat(tmpstr, strstrip(node->sharename), 1, 0);
		tmpstr = ostrcat(tmpstr, ")", 1, 0);
		changetitle(net_addshare, tmpstr);
		free(tmpstr); tmpstr = NULL;

		skin_username->hidden = YES;
		skin_password->hidden = YES;
		skin_protocol->hidden = NO;
		skin_rsize->hidden = NO;
		skin_wsize->hidden = NO;
		skin_options->hidden = NO;
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
	}
	else if(ostrcmp(node->mode, "2") == 0)
	{
		tmpstr = ostrcat(tmpstr, _("Add Ftpfs Network Share"), 1, 0);
		tmpstr = ostrcat(tmpstr, " (", 1, 0);
		tmpstr = ostrcat(tmpstr, strstrip(node->sharename), 1, 0);
		tmpstr = ostrcat(tmpstr, ")", 1, 0);
		changetitle(net_addshare, tmpstr);
		free(tmpstr); tmpstr = NULL;
		
		skin_protocol->hidden = YES;
		skin_rsize->hidden = YES;
		skin_wsize->hidden = YES;
		skin_options->hidden = YES;
		skin_userauth->hidden = NO;
		skin_usessl->hidden = NO;
		skin_useproxy->hidden = NO;
		skin_ftpport->hidden = NO;
		
		if(ostrcmp(node->userauth, "0") == 0)
		{
			skin_username->hidden = YES;
			skin_password->hidden = YES;
		}
		else
		{
			skin_username->hidden = NO;
			skin_password->hidden = NO;
		}
		
		if(ostrcmp(node->usessl, "0") == 0)
			skin_ssl->hidden = YES;
		else
			skin_ssl->hidden = NO;

		if(ostrcmp(node->useproxy, "0") == 0)
		{
			skin_proxy->hidden = YES;
			skin_proxyip->hidden = YES;
			skin_proxyport->hidden = YES;
			skin_proxyuser->hidden = YES;
			skin_proxypass->hidden = YES;
			skin_proxyauth->hidden = YES;
		}
		else
		{
			skin_proxy->hidden = NO;
			skin_proxyip->hidden = NO;
			skin_proxyport->hidden = NO;
			skin_proxyauth->hidden = NO;
			if(ostrcmp(node->proxyauth, "0") == 0)
			{
				skin_proxyuser->hidden = YES;
				skin_proxypass->hidden = YES;
			}
			else
			{
				skin_proxyuser->hidden = NO;
				skin_proxypass->hidden = NO;
			}
		}
	}
}

void screennetworkbrowser_addshare(struct networkbrowser* node, int newnode)
{
	int rcret = 0, save = 0;
	char* skinname = "networkbrowser_add_share";
	struct skin* net_addshare = getscreen(skinname);
	struct skin* listbox = getscreennode(net_addshare, "listbox");
	struct skin* tmp = NULL;
	struct skin* skin_sharename = getscreennode(net_addshare, "skin_sharename");
	struct skin* skin_ip = getscreennode(net_addshare, "skin_ipaddresse");
	struct skin* skin_sharedir = getscreennode(net_addshare, "skin_sharedir");
	struct skin* skin_username = getscreennode(net_addshare, "skin_username");
	struct skin* skin_password = getscreennode(net_addshare, "skin_password");
	struct skin* skin_wsize = getscreennode(net_addshare, "skin_wsize");
	struct skin* skin_rsize = getscreennode(net_addshare, "skin_rsize");
	struct skin* skin_hddreplacement = getscreennode(net_addshare, "skin_hddreplacement");
	struct skin* skin_protocol = getscreennode(net_addshare, "skin_protocol");
	struct skin* skin_options = getscreennode(net_addshare, "skin_options");
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
	struct skin* b3 = getscreennode(net_addshare, "b3");
	char* tmpstr = NULL;

	listbox->aktline = 1;
	listbox->aktpage = 1;

	if(node == NULL)
	{
		node = addnetworkbrowser(NULL, 1, NULL);
		newnode = 1;
	}
	if(node == NULL) return;
	
	if(newnode == 0)
		b3->hidden = NO;
	else
		b3->hidden = YES;

	setdefaultnetworkbrowser(node);
	debug(70, "after set defaut");
	debugnetworkbrowser(node);
	debug(70, "--------------------");
	
	addchoicebox(skin_mode, "0", _("cifs"));
	addchoicebox(skin_mode, "1", _("nfs"));
	addchoicebox(skin_mode, "2", _("ftpfs"));
	setchoiceboxselection(skin_mode, node->mode);

	changemask(skin_sharename, "abcdefghijklmnopqrstuvwxyz");
	changeinput(skin_sharename, node->sharename);

	tmpstr = fixip(node->ip, 0);
	changemask(skin_ip, "000.000.000.000");
	changeinput(skin_ip, tmpstr);
	free(tmpstr); tmpstr = NULL;

	changemask(skin_ftpport, "00000");
	changeinput(skin_ftpport, node->ftpport);

	changemask(skin_sharedir, "abcdefghijklmnopqrstuvwxyz");
	changeinput(skin_sharedir, node->sharedir);

	addchoicebox(skin_wsize, "32768", "32768");
	addchoicebox(skin_wsize, "49152", "49152");
	addchoicebox(skin_wsize, "65536", "65536");
	addchoicebox(skin_wsize, "4096", "4096");
	addchoicebox(skin_wsize, "8192", "8192");
	addchoicebox(skin_wsize, "16384", "16384");
	setchoiceboxselection(skin_wsize, node->wsize);

	addchoicebox(skin_rsize, "32768", "32768");
	addchoicebox(skin_rsize, "49152", "49152");
	addchoicebox(skin_rsize, "65536", "65536");
	addchoicebox(skin_rsize, "4096", "4096");
	addchoicebox(skin_rsize, "8192", "8192");
	addchoicebox(skin_rsize, "16384", "16384");
	setchoiceboxselection(skin_rsize, node->rsize);

	addchoicebox(skin_ssl, "ssl_try", _("ssl_try"));
	addchoicebox(skin_ssl, "ssl_control", _("ssl_control"));
	addchoicebox(skin_ssl, "ssl", _("ssl"));
	setchoiceboxselection(skin_ssl, node->ssl);

	addchoicebox(skin_proxy, "socks5", _("socks5"));
	addchoicebox(skin_proxy, "socks4", _("socks4"));
	addchoicebox(skin_proxy, "html", _("html"));
	setchoiceboxselection(skin_proxy, node->proxy);

	addchoicebox(skin_useproxy, "0", _("no"));
	addchoicebox(skin_useproxy, "1", _("yes"));
	setchoiceboxselection(skin_useproxy, node->useproxy);

	addchoicebox(skin_usessl, "0", _("no"));
	addchoicebox(skin_usessl, "1", _("yes"));
	setchoiceboxselection(skin_usessl, node->usessl);

	addchoicebox(skin_userauth, "0", _("no"));
	addchoicebox(skin_userauth, "1", _("yes"));
	setchoiceboxselection(skin_userauth, node->userauth);

	addchoicebox(skin_proxyauth, "0", _("no"));
	addchoicebox(skin_proxyauth, "1", _("yes"));
	setchoiceboxselection(skin_proxyauth, node->proxyauth);

	tmpstr = fixip(node->proxyip, 0);
	changemask(skin_proxyip, "000.000.000.000");
	changeinput(skin_proxyip, tmpstr);
	free(tmpstr); tmpstr = NULL;

	changemask(skin_proxyport, "00000");
	changeinput(skin_proxyport, node->proxyport);

	changemask(skin_proxyuser, "abcdefghijklmnopqrstuvwxyz");
	changeinput(skin_proxyuser, node->proxyuser);

	changemask(skin_proxypass, "abcdefghijklmnopqrstuvwxyz");
	changeinput(skin_proxypass, node->proxypass);

	changemask(skin_username, "abcdefghijklmnopqrstuvwxyz");
	changeinput(skin_username, node->username);

	changemask(skin_password, "abcdefghijklmnopqrstuvwxyz");
	changeinput(skin_password, node->password);

	if(newnode == 0 && checkhddreplacement(node->sharename) == 1)
		tmpstr = ostrcat(tmpstr, "1", 1, 0);
	addchoicebox(skin_hddreplacement, "0", _("no"));
	addchoicebox(skin_hddreplacement, "1", _("yes"));
	setchoiceboxselection(skin_hddreplacement, tmpstr);
	free(tmpstr); tmpstr = NULL;

	addchoicebox(skin_protocol, "udp", _("udp"));
	addchoicebox(skin_protocol, "tcp", _("tcp"));
	setchoiceboxselection(skin_protocol, node->protocol);

	addchoicebox(skin_options, "rw", _("rw"));
	addchoicebox(skin_options, "ro", _("ro"));
	setchoiceboxselection(skin_options, node->options);

	changemodenetworkbrowser(node, net_addshare, skin_username, skin_password, skin_protocol, skin_rsize, skin_wsize, skin_options, skin_ssl, skin_proxy, skin_proxyip, skin_proxyport, skin_proxyuser, skin_proxypass, skin_ftpport, skin_userauth, skin_proxyauth, skin_useproxy, skin_usessl);

	addscreenrc(net_addshare, listbox);
	drawscreen(net_addshare, 0, 0);

	tmp = listbox->select;
	while(1)
	{
		addscreenrc(net_addshare, tmp);
		rcret = waitrc(net_addshare, 0, 0);
		tmp = listbox->select;

		free(node->mode); node->mode = ostrcat(skin_mode->ret, NULL, 0, 0);
		free(node->sharename); node->sharename = ostrcat(skin_sharename->ret, NULL, 0, 0);
		free(node->ip); node->ip = ostrcat(skin_ip->ret, NULL, 0, 0);
		free(node->ftpport); node->ftpport = ostrcat(skin_ftpport->ret, NULL, 0, 0);
		free(node->sharedir); node->sharedir = ostrcat(skin_sharedir->ret, NULL, 0, 0);
		free(node->wsize); node->wsize = ostrcat(skin_wsize->ret, NULL, 0, 0);
		free(node->rsize); node->rsize = ostrcat(skin_rsize->ret, NULL, 0, 0);
		free(node->ssl); node->ssl = ostrcat(skin_ssl->ret, NULL, 0, 0);
		free(node->proxy); node->proxy = ostrcat(skin_proxy->ret, NULL, 0, 0);
		free(node->useproxy); node->useproxy = ostrcat(skin_useproxy->ret, NULL, 0, 0);
		free(node->usessl); node->usessl = ostrcat(skin_usessl->ret, NULL, 0, 0);
		free(node->userauth); node->userauth = ostrcat(skin_userauth->ret, NULL, 0, 0);
		free(node->proxyauth); node->proxyauth = ostrcat(skin_proxyauth->ret, NULL, 0, 0);
		free(node->proxyip); node->proxyip = ostrcat(skin_proxyip->ret, NULL, 0, 0);
		free(node->proxyport); node->proxyport = ostrcat(skin_proxyport->ret, NULL, 0, 0);
		free(node->proxyuser); node->proxyuser = ostrcat(skin_proxyuser->ret, NULL, 0, 0);
		free(node->proxypass); node->proxypass = ostrcat(skin_proxypass->ret, NULL, 0, 0);
		free(node->username); node->username = ostrcat(skin_username->ret, NULL, 0, 0);
		free(node->password); node->password = ostrcat(skin_password->ret, NULL, 0, 0);
		free(node->protocol); node->protocol = ostrcat(skin_protocol->ret, NULL, 0, 0);
		free(node->options); node->options = ostrcat(skin_options->ret, NULL, 0, 0);

		if(listbox->select != NULL)
		{
			changemodenetworkbrowser(node, net_addshare, skin_username, skin_password, skin_protocol, skin_rsize, skin_wsize, skin_options, skin_ssl, skin_proxy, skin_proxyip, skin_proxyport, skin_proxyuser, skin_proxypass, skin_ftpport, skin_userauth, skin_proxyauth, skin_useproxy, skin_usessl);
			drawscreen(net_addshare, 0, 0);
		}

		if(rcret == getrcconfigint("rcexit", NULL)) break;
		
		if(newnode == 0 && rcret == getrcconfigint("rcred", NULL) && checknetworkbrowserexist(node) == 0)
		{
			struct networkbrowser* newshare = addnetworkbrowser(NULL, 1, NULL);
			if(newshare != NULL)
			{
				tmpstr = textinput(NULL, "SHARENAME");
				if(tmpstr != NULL && strlen(tmpstr) != 0)
				{
					//set to default if field is empty
					setdefaultnetworkbrowser(node);
					copynetworkbrowser(node, newshare, tmpstr);
					newnode = 1;
					node = newshare;
					changeinput(skin_sharename, node->sharename);
					b3->hidden = YES;
					changemodenetworkbrowser(node, net_addshare, skin_username, skin_password, skin_protocol, skin_rsize, skin_wsize, skin_options, skin_ssl, skin_proxy, skin_proxyip, skin_proxyport, skin_proxyuser, skin_proxypass, skin_ftpport, skin_userauth, skin_proxyauth, skin_useproxy, skin_usessl);
				}
				else
					delnetworkbrowser(newshare);
				free(tmpstr); tmpstr = NULL;
				drawscreen(net_addshare, 0, 0);
			}
			continue;
		}

		if(rcret == getrcconfigint("rcok", NULL))
		{
			if(node->sharename == NULL || strlen(node->sharename) == 0)
			{
				textbox(_("Message"), _("Sharename can't be empty!"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 800, 200, 0, 0);
				drawscreen(net_addshare, 0, 0);
				continue;
			}

			if(checknetworkbrowserexist(node) == 1)
			{
				textbox(_("Message"), _("A mount entry with this name already exists!"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 800, 200, 0, 0);
				drawscreen(net_addshare, 0, 0);
				continue;
			}

			if(textbox(_("Message"), _("Are you sure you want to save this network mount?"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 800, 200, 0, 0) == 1)
			{
				//set to default if field is empty
				setdefaultnetworkbrowser(node);
				if(ostrcmp(skin_hddreplacement->ret, "1") == 0)
					addhddreplacement(node->sharename);
				else if(checkhddreplacement(node->sharename) == 1)
					delhddreplacement();

				savenetworkbrowser("/var/etc/automount/auto.misc");
				save = 1;
			}
			break;
		}
	}

	if(newnode == 1 && save == 0) delnetworkbrowser(node);
	delownerrc(net_addshare);
	clearscreen(net_addshare);
}

void screennetworkbrowser()
{
	int rcret = 0;
	struct networkbrowser* node = NULL;
	struct menulist* mlist = NULL, *mbox = NULL;
	char* tmpstr = NULL, *tmppic = NULL;
	
	status.hangtime = 99999;
	system("cp -a /mnt/automount/* /var/etc/automount");

start:
	freenetworkbrowser(); networkbrowser = NULL;
	readnetworkbrowser("/var/etc/automount/auto.misc", 0);
	
	node = networkbrowser;
	while(node != NULL)
	{
		if(ostrcmp(node->mode, "0") == 0)
    {
			tmpstr = ostrcat(tmpstr, "(cifs) ", 1, 0);
      tmppic = ostrcat("netbrowser_cifs.png", NULL, 0, 0);
    }
		else if(ostrcmp(node->mode, "1") == 0)
    {
			tmpstr = ostrcat(tmpstr, "(nfs) ", 1, 0);
      tmppic = ostrcat("netbrowser_nfs.png", NULL, 0, 0);
    }
		else if(ostrcmp(node->mode, "2") == 0)
    {
			tmpstr = ostrcat(tmpstr, "(ftpfs) ", 1, 0);
      tmppic = ostrcat("netbrowser_ftpfs.png", NULL, 0, 0);
    }

		tmpstr = ostrcat(tmpstr, node->sharename, 1, 0);
		tmpstr = ostrcat(tmpstr, " (", 1, 0);
		tmpstr = ostrcat(tmpstr, node->ip, 1, 0);
		tmpstr = ostrcat(tmpstr, ":", 1, 0);
		tmpstr = ostrcat(tmpstr, node->ftpport, 1, 0);
		tmpstr = ostrcat(tmpstr, "/", 1, 0);
		tmpstr = ostrcat(tmpstr, node->sharedir, 1, 0);
		tmpstr = ostrcat(tmpstr, ")", 1, 0);
    
		struct menulist* tmpmlist = addmenulist(&mlist, tmpstr, NULL, tmppic, 0, 0);
		if(tmpmlist != NULL) tmpmlist->param = (char*)node;
		free(tmpstr); tmpstr = NULL;
    free(tmppic); tmppic = NULL;
		node = node->next;
	}
	
	mbox = menulistboxext(mlist, "networkbrowser", "Networkbrowser - show Shares", "%pluginpath%/networkbrowser/skin/", NULL, 1, &rcret, 0);

	if(rcret == getrcconfigint("rcred", NULL))
	{
		screennetworkbrowser_addshare(NULL, 1);
		freemenulist(mlist, 0); mlist = NULL;
		freenetworkbrowser(); networkbrowser = NULL;
		goto start;
	}

	if(rcret == getrcconfigint("rcblue", NULL))
	{
		screennetworkbrowser_scan();
		freemenulist(mlist, 0); mlist = NULL;
		freenetworkbrowser(); networkbrowser = NULL;
		goto start;
	}
	
	if(mbox != NULL)
	{
		if(rcret == getrcconfigint("rcok", NULL) || rcret == getrcconfigint("rcgreen", NULL))
			screennetworkbrowser_addshare((struct networkbrowser*)mbox->param, 0);
		if(rcret == getrcconfigint("rcyellow", NULL))
		{
			if(textbox(_("Message"), _("Are you sure you want remove this share?"), _("OK"), getrcconfigint("rcok", NULL), _("EXIT"), getrcconfigint("rcexit", NULL), NULL, 0, NULL, 0, 800, 200, 0, 0) == 1)
			{
				struct networkbrowser* delnode = (struct networkbrowser*)mbox->param;
				if(delnode != NULL)
				{
					if(checkhddreplacement(delnode->sharename) == 1)
						delhddreplacement();

					delnetworkbrowser(delnode);
					savenetworkbrowser("/var/etc/automount/auto.misc");
				}
			}
		}
		freemenulist(mlist, 0); mlist = NULL;
		freenetworkbrowser(); networkbrowser = NULL;
		goto start;
	}

	freemenulist(mlist, 0); mlist = NULL;
	freenetworkbrowser();

	status.hangtime = getconfigint("hangtime", NULL);
}

#endif
