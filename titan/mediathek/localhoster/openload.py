# -*- coding: utf-8 -*-
# Based on (root)/trunk/xbmc-addons/src/plugin.video.polishtv.live/hosts/ @ 419 - Wersja 636
# encoding=utf8  
import sys  

reload(sys)  
sys.setdefaultencoding('utf8')

import os
import hashlib
import sys
from lib.net import Net
import re
import lib.ol_gmu as ol_gmu
import lib.common as common
import json
from lib import jsunpack

###################################################
import re
import time
import urllib
import socket
import string
import base64
import math
import lib.jjdecode as JJDecoder

#OL_SOURCE = 'https://offshoregit.com/tvaresolvers/ol_gmu.py'
OL_PATH = ''

API_BASE_URL = 'https://api.openload.co/1'
INFO_URL = API_BASE_URL + '/streaming/info'
GET_URL = API_BASE_URL + '/streaming/get?file={media_id}'
#OL_PATH = os.path.join(common.plugins_path, 'ol_gmu.py')


import urllib
import urllib2
try: import ssl
except Exception: pass
import re
import string
import time
import htmlentitydefs
import cookielib
import unicodedata
#import urllib2_ssl
#try:    import json
#except Exception: import simplejson as json
#try:
#    try: from cStringIO import StringIO
#    except Exception: from StringIO import StringIO 
#    import gzip

class OpenLoadResolver(object):
    name = "openload"
#    domains = ["openload.io", "openload.co"]
#    pattern = '(?://|\.)(openload\.(?:io|co))/(?:embed|f)/([0-9a-zA-Z-_]+)'
    domains = ["openload.io", "openload.co", "oload.tv", "oload.stream"]
    pattern = '(?://|\.)(o(?:pen)??load\.(?:io|co|tv|stream))/(?:embed|f)/([0-9a-zA-Z-_]+)'

    def __init__(self):
        self.net = Net(cookie_file='/mnt/network/cookies', cloudflare=True)
        self.headers = {'User-Agent': common.ANDROID_USER_AGENT}
        url = str(sys.argv[1])
        host = self.get_host_and_id(url)[0]
        media_id = self.get_host_and_id(url)[1]

        return self.get_media_url(host, media_id)

    def get_host_and_id(self, url):
        r = re.search(self.pattern, url, re.I)
        if r:
            return r.groups()
        else:
            return False

    def i18n(string_id):
        try:
            return addon.getLocalizedString(strings.STRINGS[string_id]).encode('utf-8', 'ignore')
        except Exception as e:
#            log_utils.log('Failed String Lookup: %s (%s)' % (string_id, e))
            return string_id

    def get_ol_code(self):
        try:
            new_py = self.net.http_GET(OL_SOURCE).content
#            common.log_utils.log('new_py: %s' % (new_pya))

            if new_py:
                with open(OL_PATH, 'w') as f:
                    f.write(new_py)
        except Exception as e:
             print 'errormsg=Exception during openload code retrieve:'
#            common.log_utils.log_warning('Exception during openload code retrieve: %s' % e)

        def decode1(self, encoded):
            tab = encoded.split('\\')
            ret = ''
            for item in tab:
                try: ret += chr(int(item, 8))
                except Exception: 
                    ret += item
            return ret
        
        def base10toN(self, num,n):
            num_rep={10:'a', 11:'b',12:'c',13:'d',14:'e',15:'f',16:'g',17:'h',18:'i',19:'j',20:'k',21:'l',22:'m',23:'n',24:'o',25:'p',26:'q',27:'r',28:'s',29:'t',30:'u',31:'v',32:'w',33:'x',34:'y',35:'z'}
            new_num_string=''
            current=num
            while current!=0:
                remainder=current%n
                if 36>remainder>9:
                    remainder_string=num_rep[remainder]
                elif remainder>=36:
                    remainder_string='('+str(remainder)+')'
                else:
                    remainder_string=str(remainder)
                new_num_string=remainder_string+new_num_string
                current=current/n
            return new_num_string

    def decodeOpenLoad(self, aastring):
       # decodeOpenLoad made by mortael, please leave this line for proper credit :)
    #aastring = re.search(r"<video(?:.|\s)*?<script\s[^>]*?>((?:.|\s)*?)</script", html, re.DOTALL | re.IGNORECASE).group(1)

        aastring = aastring.replace("(ﾟДﾟ)[ﾟεﾟ]+(oﾟｰﾟo)+ ((c^_^o)-(c^_^o))+ (-~0)+ (ﾟДﾟ) ['c']+ (-~-~1)+","")
        aastring = aastring.replace("((ﾟｰﾟ) + (ﾟｰﾟ) + (ﾟΘﾟ))", "9")
        aastring = aastring.replace("((ﾟｰﾟ) + (ﾟｰﾟ))","8")
        aastring = aastring.replace("((ﾟｰﾟ) + (o^_^o))","7")
        aastring = aastring.replace("((c^_^o)-(c^_^o))","0")
        aastring = aastring.replace("((ﾟｰﾟ) + (ﾟΘﾟ))","5")
        aastring = aastring.replace("(ﾟｰﾟ)","4")
        aastring = aastring.replace("((o^_^o) - (ﾟΘﾟ))","2")
        aastring = aastring.replace("(o^_^o)","3")
        aastring = aastring.replace("(ﾟΘﾟ)","1")
        aastring = aastring.replace("(+!+[])","1")
        aastring = aastring.replace("(c^_^o)","0")
        aastring = aastring.replace("(0+0)","0")
        aastring = aastring.replace("(ﾟДﾟ)[ﾟεﾟ]","\\")  
        aastring = aastring.replace("(3 +3 +0)","6")
        aastring = aastring.replace("(3 - 1 +0)","2")
        aastring = aastring.replace("(!+[]+!+[])","2")
        aastring = aastring.replace("(-~-~2)","4")
        aastring = aastring.replace("(-~-~1)","3")
        aastring = aastring.replace("(-~0)","1")
        aastring = aastring.replace("(-~1)","2")
        aastring = aastring.replace("(-~3)","4")
        aastring = aastring.replace("(0-0)","0")
    
        aastring = aastring.replace("(ﾟДﾟ).ﾟωﾟﾉ","10")
        aastring = aastring.replace("(ﾟДﾟ).ﾟΘﾟﾉ","11")
        aastring = aastring.replace("(ﾟДﾟ)[\'c\']","12")
        aastring = aastring.replace("(ﾟДﾟ).ﾟｰﾟﾉ","13")
        aastring = aastring.replace("(ﾟДﾟ).ﾟДﾟﾉ","14")
        aastring = aastring.replace("(ﾟДﾟ)[ﾟΘﾟ]","15")
#        print "aastring", aastring
        decodestring = re.search(r"\\\+([^(]+)", aastring, re.DOTALL | re.IGNORECASE).group(1)
#        decodestring = re.search(r"\\\+([^(]+)", aastring, re.DOTALL | re.IGNORECASE) 
        decodestring = "\\+"+ decodestring
        decodestring = decodestring.replace("+","")
        decodestring = decodestring.replace(" ","")

        decodestring = self.decode1(decodestring)
        decodestring = decodestring.replace("\\/","/")
    
        if 'toString' in decodestring:
            base = re.compile(r"toString\(a\+(\d+)", re.DOTALL | re.IGNORECASE).findall(decodestring)[0]
            base = int(base)
            match = re.compile(r"(\(\d[^)]+\))", re.DOTALL | re.IGNORECASE).findall(decodestring)
            for repl in match:
                match1 = re.compile(r"(\d+),(\d+)", re.DOTALL | re.IGNORECASE).findall(repl)
                base2 = base + int(match1[0][0])
                repl2 = self.base10toN(int(match1[0][1]),base2)
                decodestring = decodestring.replace(repl,repl2)
            decodestring = decodestring.replace("+","")
            decodestring = decodestring.replace("\"","")
        print "decodestring", decodestring
        return decodestring

#    def OPENLOADIO_decryptPlayerParams(p, a, c, k, e, d):
    def decryptPlayerParams(self, p, a, c, k, e, d):
        def e1(c):
            return c
        def e2(t=None):
            return '\\w+'
        def k1(matchobj):
            return d[int(matchobj.group(0))]
        e = e1
        if True:
            while c != 0:
                c -= 1
                d[c] = k[c]
                if c < len(k):
                    d[c] = k[c]
                else:
                    d[c] = c
            c = 1
            k = [k1]
            e = e2
        while c != 0:
            c -= 1
            if k[c]:
                reg = '\\b' + e(c) + '\\b'
                p = re.sub(reg, k[c], p)
        return p

    def getAllItemsBeetwenNodes(self, data, node1, node2, withNodes=True, numNodes=-1, caseSensitive=True):
        if len(node1) < 2 or len(node2) < 2:
            return []
        itemsTab = []
        n1S = node1[0]
        n1E = node1[1]
        if len(node1) > 2: n1P = node1[2]
        else: n1P = None
        n2S = node2[0]
        n2E = node2[1]
        if len(node2) > 2: n2P = node2[2]
        else: n2P = None
        lastIdx = 0
        search = 1
        
        if caseSensitive:
            sData = data
        else:
            sData = data.lower()
            n1S = n1S.lower()
            n1E = n1E.lower()
            if n1P != None: n1P = n1P.lower()
            n2S = n2S.lower()
            n2E = n2E.lower()
            if n2P != None: n2P = n2P.lower()
            
        while True:
            if search == 1:
                # node 1 - start
                idx1 = sData.find(n1S, lastIdx)
                if -1 == idx1: return itemsTab
                lastIdx = idx1 + len(n1S)
                idx2 = sData.find(n1E, lastIdx)
                if -1 == idx2: return itemsTab
                lastIdx = idx2 + len(n1E)
                if n1P != None and sData.find(n1P, idx1 + len(n1S), idx2) == -1:
                    continue
                search = 2
            else:
                # node 2 - end
                tIdx1 = sData.find(n2S, lastIdx)
                if -1 == tIdx1: return itemsTab
                lastIdx = tIdx1 + len(n2S)
                tIdx2 = sData.find(n2E, lastIdx)
                if -1 == tIdx2: return itemsTab
                lastIdx = tIdx2 + len(n2E)

                if n2P != None and sData.find(n2P, tIdx1 + len(n2S), tIdx2) == -1:
                    continue

                if withNodes:
                    idx2 = tIdx2 + len(n2E)
                else:
                    idx1 = idx2 + len(n1E)
                    idx2 = tIdx1
                search = 1
                itemsTab.append(data[idx1:idx2])
            if numNodes > 0 and len(itemsTab) == numNodes:
                break
        return itemsTab

    def getAllItemsBeetwenMarkers(self, data, marker1, marker2, withMarkers=True, caseSensitive=True):
        itemsTab = []
        if caseSensitive:
            sData = data
        else:
            sData = data.lower()
            marker1 = marker1.lower()
            marker2 = marker2.lower()
        idx1 = 0
        while True:
            idx1 = sData.find(marker1, idx1)
            if -1 == idx1: return itemsTab
            idx2 = sData.find(marker2, idx1 + len(marker1))
            if -1 == idx2: return itemsTab
            tmpIdx2 = idx2 + len(marker2) 
            if withMarkers:
                idx2 = tmpIdx2
            else:
                idx1 = idx1 + len(marker1)
            itemsTab.append(data[idx1:idx2])
            idx1 = tmpIdx2
        return itemsTab

    def getSearchGroups(self, data, pattern, grupsNum=1, ignoreCase=False):
        tab = []
        if ignoreCase:
            match = re.search(pattern, data, re.IGNORECASE)
        else:
            match = re.search(pattern, data)
        
        for idx in range(grupsNum):
            try:    value = match.group(idx + 1)
            except Exception: value = ''
            tab.append(value)
        return tab
        
    def getPage(self, url, addParams = {}, post_data = None):
        ''' wraps getURLRequestData '''
        try:
            addParams['url'] = url
            if 'return_data' not in addParams:
                addParams['return_data'] = True
            response = self.getURLRequestData(addParams, post_data)
            status = True
        except urllib2.HTTPError, e:
#            printExc()
            response = e
            status = False
        except Exception:
#            printExc()
            response = None
            status = False
        
        if addParams['return_data'] and status and not isinstance(response, basestring):
            status = False
            
        return (status, response)

    def GetTmpDir(self, file = ''):
        path = "/tmp/"
        path = path.replace('//', '/')
        self.mkdirs(path)
        return path + '/' + file

    def GetDukPath(self):
#        return "/tmp/localhoster/bin/duk.arm"
	arch = self.command("cat /etc/.arch")
#        print "arch", arch
        return "/tmp/localhoster/bin/duk.%s" % (arch)
 #       return "/tmp/localhoster/bin/duk.sh4"

#        return config.plugins.iptvplayer.dukpath.value

    def iptv_js_execute(self, jscode):
        sts, tmpPath = self.CreateTmpFile('.iptv_js.js', jscode)
        if sts:
            cmd =  self.GetDukPath() + ' ' + tmpPath + ' 2> /dev/null'
 #           printDBG("iptv_js_execute cmd[%s]" % cmd)
 #           print "iptv_js_execute cmd[%s]" % cmd
 #           test = self.command(cmd);
 #           print "test", test

            ret = self.command(cmd);
#            print "test3", os.system(cmd);

 #           ret = iptv_execute()( cmd )
#            ret = {'sts':False, 'code':-12, 'data':''}
            # leave last script for debug purpose
 #           if getDebugMode() == '':
 #               rm(tmpPath)
        else:
            ret = {'sts':False, 'code':-12, 'data':''}
#        printDBG('iptv_js_execute cmd ret[%s]' % ret)
#        print 'iptv_js_execute cmd ret[%s]' % ret

        return ret

    def mkdirs(self, newdir):
        """ Create a directory and all parent folders.
            Features:
            - parent directories will be created
            - if directory already exists, then do nothing
            - if there is another filsystem object with the same name, raise an exception
        """
#        printDBG('mkdirs: "%s"' % newdir)
 #       print 'mkdirs: "%s"' % newdir
        try:
            if os.path.isdir(newdir):
                pass
            elif os.path.isfile(newdir):
#                raise OSError("cannot create directory, file already exists: '%s'" % newdir)
                raise #print "cannot create directory, file already exists: '%s'" % newdir
            else:
                head, tail = os.path.split(newdir)
                if head and not os.path.isdir(head) and not os.path.ismount(head) and not os.path.islink(head):
                   mkdirs(head)
                if tail:
                    os.mkdir(newdir)
            return True
        except Exception:
#            printExc('!!!!!!!!!! EXCEPTION mkdirs["%s"]' % newdir)
            print '!!!!!!!!!! EXCEPTION mkdirs["%s"]' % newdir
            return False

    def CreateTmpFile(self, filename, data=''):
        sts = False
        filePath = self.GetTmpDir(filename)
        try:
            with open(filePath, 'w') as f:
                f.write(data)
                sts = True
        except Exception:
            printExc()
        return sts, filePath

    def getDataBeetwenMarkers(self, data, marker1, marker2, withMarkers=True, caseSensitive=True):
        if caseSensitive:
            idx1 = data.find(marker1)
        else:
            idx1 = data.lower().find(marker1.lower())
        if -1 == idx1: return False, ''
        if caseSensitive:
            idx2 = data.find(marker2, idx1 + len(marker1))
        else:
            idx2 = data.lower().find(marker2.lower(), idx1 + len(marker1))
        if -1 == idx2: return False, ''
        
        if withMarkers:
            idx2 = idx2 + len(marker2)
        else:
            idx1 = idx1 + len(marker1)

        return True, data[idx1:idx2]

	# edit bb , touch commands.getouput with this def #
    def command(self, comandline, strip=1):
        comandline = comandline + " >/tmp/command.txt"
        os.system(comandline)
        text = ""
        if os.path.exists("/tmp/command.txt") is True:
            file = open("/tmp/command.txt", "r")
            if strip == 1:
                for line in file:
                    text = text + line.strip() + '\n'
            else:
                for line in file:
                    text = text + line
                    if text[-1:] != '\n': text = text + "\n"
            file.close
        # if one or last line then remove linefeed
        if text[-1:] == '\n': text = text[:-1]
        comandline = text
        os.system("rm /tmp/command.txt")
        return comandline

    def decode(self,code, parseInt, _0x59ce16, _1x4bfb36):
          
        import math

        _0x1bf6e5 = ''
        ke = []

        for i in range(0, len(code[0:9*8]),8):
            ke.append(int(code[i:i+8],16))

        _0x439a49 = 0
        _0x145894 = 0

        while _0x439a49 < len(code[9*8:]):
            _0x5eb93a = 64
            _0x896767 = 0
            _0x1a873b = 0
            _0x3c9d8e = 0
            while True:
                if _0x439a49 + 1 >= len(code[9*8:]):
                    _0x5eb93a = 143;

                _0x3c9d8e = int(code[9*8+_0x439a49:9*8+_0x439a49+2], 16)
                _0x439a49 +=2

                if _0x1a873b < 6*5:
                    _0x332549 = _0x3c9d8e & 63
                    _0x896767 += _0x332549 << _0x1a873b
                else:
                    _0x332549 = _0x3c9d8e & 63
                    _0x896767 += int(_0x332549 * math.pow(2, _0x1a873b))

                _0x1a873b += 6
                if not _0x3c9d8e >= _0x5eb93a: break

            # _0x30725e = _0x896767 ^ ke[_0x145894 % 9] ^ _0x59ce16 ^ parseInt ^ _1x4bfb36
            _0x30725e = _0x896767 ^ ke[_0x145894 % 9] ^ parseInt ^ _1x4bfb36
            _0x2de433 = _0x5eb93a * 2 + 127

            for i in range(4):
                _0x3fa834 = chr(((_0x30725e & _0x2de433) >> (9*8/ 9)* i) - 1)
                if _0x3fa834 != '$':
                    _0x1bf6e5 += _0x3fa834
                _0x2de433 = (_0x2de433 << (9*8/ 9))

            _0x145894 += 1


        url = "https://openload.co/stream/%s?mime=true" % _0x1bf6e5
        return url

    def get_media_url(self, host, media_id):
        web_url = self.get_url(host, media_id)
#        web_url = "https://openload.co/embed/SVMiyZR_yM4/Game_of_Thrones_S07E07_Der_Drache_und_der_Wolf_German_AC3_Dubbed_AmazonHD.mkv"
#        web_url = "https://openload.co/embed/8ATYb_yCakU/pso-atomic_ts.sd.mkv"
        headers = {
            'Referer': web_url
        }
        headers.update(self.headers)
        data = self.net.http_GET(web_url, headers=headers).content
        ret = self.net.save_cookies('/mnt/network/cookies')

#        print "data", data
        baseUrl = web_url
 #       baseUrl = "https://openload.co/embed/SVMiyZR_yM4/Game_of_Thrones_S07E07_Der_Drache_und_der_Wolf_German_AC3_Dubbed_AmazonHD.mkv"
        HTTP_HEADER= { 'User-Agent':"Mozilla/5.0", 'Referer':baseUrl}
        HTTP_HEADER = {'User-Agent': 'Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.11 (KHTML, like Gecko) Chrome/23.0.1271.64 Safari/537.11',
               'Accept': 'text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8',
               'Accept-Charset': 'ISO-8859-1,utf-8;q=0.7,*;q=0.3',
               'Accept-Encoding': 'none',
               'Accept-Language': 'en-US,en;q=0.8',
               'Referer':baseUrl} #'Connection': 'keep-alive'

#        sts, data = self.getPage(baseUrl, {'header':HTTP_HEADER})
 #       data = self.net.http_GET(web_url, headers=headers).content

        orgData = data
 #       print "data", data

####################
        videoTab = []
        code = self.getSearchGroups(data, '<p style="" id="[^"]+">(.*?)</p>' )[0]
        _0x59ce16 = eval(self.getSearchGroups(data, '_0x59ce16=([^;]+)')[0].replace('parseInt', 'int'))
        _1x4bfb36 = eval(self.getSearchGroups(data, '_1x4bfb36=([^;]+)')[0].replace('parseInt', 'int'))
        parseInt  = eval(self.getSearchGroups(data, '_0x30725e,(\(parseInt.*?)\),')[0].replace('parseInt', 'int'))
        url = self.decode(code, parseInt, _0x59ce16, _1x4bfb36)
        print url
        return None
        #url2 = self.cm.getPage(url).headers.get('location')
        #print "url2",url2
        #videoTab.append(('mp4',url))
        #videoTab.append({'url': url, 'name': 'openload.co (mp4)'})
        #print "videoTab",videoTab

        #return videoTab
#       sts, data = self.cm.getPage(url, defaultParams)
#       print "data",data

####################
#        print "222222222222222222222222"
        if re.search('Sorry!</h3>', data):
            print 'errormsg=%s %s' % (str(re.compile('<h3>(.*)</h3>').findall(data)[0]), str(re.compile('<h6>(.*)</h6>').findall(data)[0]))

        subTracksData = self.getAllItemsBeetwenMarkers(data, '<track ', '>', False, False)
        subTracks = []
        for track in subTracksData:
            if 'kind="captions"' not in track: continue
            subUrl = self.getSearchGroups(track, 'src="([^"]+?)"')[0]
#            print "suburl", subUrl
            if subUrl.startswith('/'):
                subUrl = 'http://openload.co' + subUrl
            if subUrl.startswith('http'):
                subLang = self.getSearchGroups(track, 'srclang="([^"]+?)"')[0]
                subLabel = self.getSearchGroups(track, 'label="([^"]+?)"')[0]
                subTracks.append({'title':subLabel + '_' + subLang, 'url':subUrl, 'lang':subLang, 'format':'srt'})
       
        videoUrl = ''
        tmp = self.getAllItemsBeetwenNodes(data, ('<div', '>', 'display:none'), ('</div', '>'))
        for item in tmp:
            encTab = re.compile('''<[^>]+?id="[^"]*?"[^>]*?>([^<]+?)<''').findall(data)
            for e in encTab:
                if len(e) > 40:
                    encTab.insert(0, e)
                    break
        
        def __decode_k(enc, jscode):
            decoded = ''
            tmpPath = ''
            try:
                jscode = base64.b64decode('''ICAgICAgICAgICAgICAgICAgICB2YXIgaWQgPSAiJXMiDQogICAgICAgICAgICAgICAgICAgICAgLCBkZWNvZGVkDQogICAgICAgICAgICAgICAgICAgICAgLCBkb2N1bWVudCA9IHt9DQogICAgICAgICAgICAgICAgICAgICAgLCB3aW5kb3cgPSB0aGlzDQogICAgICAgICAgICAgICAgICAgICAgLCAkID0gZnVuY3Rpb24oKXsNCiAgICAgICAgICAgICAgICAgICAgICAgICAgcmV0dXJuIHsNCiAgICAgICAgICAgICAgICAgICAgICAgICAgICB0ZXh0OiBmdW5jdGlvbihhKXsNCiAgICAgICAgICAgICAgICAgICAgICAgICAgICAgIGlmKGEpDQogICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgIGRlY29kZWQgPSBhOw0KICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgZWxzZQ0KICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICByZXR1cm4gaWQ7DQogICAgICAgICAgICAgICAgICAgICAgICAgICAgfSwNCiAgICAgICAgICAgICAgICAgICAgICAgICAgICByZWFkeTogZnVuY3Rpb24oYSl7DQogICAgICAgICAgICAgICAgICAgICAgICAgICAgICBhKCkNCiAgICAgICAgICAgICAgICAgICAgICAgICAgICB9DQogICAgICAgICAgICAgICAgICAgICAgICAgIH0NCiAgICAgICAgICAgICAgICAgICAgICAgIH07DQogICAgICAgICAgICAgICAgICAgIChmdW5jdGlvbihkLCB3KXsNCiAgICAgICAgICAgICAgICAgICAgICB2YXIgZiA9IGZ1bmN0aW9uKCl7fTsNCiAgICAgICAgICAgICAgICAgICAgICB2YXIgcyA9ICcnOw0KICAgICAgICAgICAgICAgICAgICAgIHZhciBvID0gbnVsbDsNCiAgICAgICAgICAgICAgICAgICAgICB2YXIgYiA9IGZhbHNlOw0KICAgICAgICAgICAgICAgICAgICAgIHZhciBuID0gMDsNCiAgICAgICAgICAgICAgICAgICAgICB2YXIgZGYgPSBbJ2Nsb3NlJywnY3JlYXRlQXR0cmlidXRlJywnY3JlYXRlRG9jdW1lbnRGcmFnbWVudCcsJ2NyZWF0ZUVsZW1lbnQnLCdjcmVhdGVFbGVtZW50TlMnLCdjcmVhdGVFdmVudCcsJ2NyZWF0ZU5TUmVzb2x2ZXInLCdjcmVhdGVSYW5nZScsJ2NyZWF0ZVRleHROb2RlJywnY3JlYXRlVHJlZVdhbGtlcicsJ2V2YWx1YXRlJywnZXhlY0NvbW1hbmQnLCdnZXRFbGVtZW50QnlJZCcsJ2dldEVsZW1lbnRzQnlOYW1lJywnZ2V0RWxlbWVudHNCeVRhZ05hbWUnLCdpbXBvcnROb2RlJywnb3BlbicsJ3F1ZXJ5Q29tbWFuZEVuYWJsZWQnLCdxdWVyeUNvbW1hbmRJbmRldGVybScsJ3F1ZXJ5Q29tbWFuZFN0YXRlJywncXVlcnlDb21tYW5kVmFsdWUnLCd3cml0ZScsJ3dyaXRlbG4nXTsNCiAgICAgICAgICAgICAgICAgICAgICBkZi5mb3JFYWNoKGZ1bmN0aW9uKGUpe2RbZV09Zjt9KTsNCiAgICAgICAgICAgICAgICAgICAgICB2YXIgZG9fID0gWydhbmNob3JzJywnYXBwbGV0cycsJ2JvZHknLCdkZWZhdWx0VmlldycsJ2RvY3R5cGUnLCdkb2N1bWVudEVsZW1lbnQnLCdlbWJlZHMnLCdmaXJzdENoaWxkJywnZm9ybXMnLCdpbWFnZXMnLCdpbXBsZW1lbnRhdGlvbicsJ2xpbmtzJywnbG9jYXRpb24nLCdwbHVnaW5zJywnc3R5bGVTaGVldHMnXTsNCiAgICAgICAgICAgICAgICAgICAgICBkb18uZm9yRWFjaChmdW5jdGlvbihlKXtkW2VdPW87fSk7DQogICAgICAgICAgICAgICAgICAgICAgdmFyIGRzID0gWydVUkwnLCdjaGFyYWN0ZXJTZXQnLCdjb21wYXRNb2RlJywnY29udGVudFR5cGUnLCdjb29raWUnLCdkZXNpZ25Nb2RlJywnZG9tYWluJywnbGFzdE1vZGlmaWVkJywncmVmZXJyZXInLCd0aXRsZSddOw0KICAgICAgICAgICAgICAgICAgICAgIGRzLmZvckVhY2goZnVuY3Rpb24oZSl7ZFtlXT1zO30pOw0KICAgICAgICAgICAgICAgICAgICAgIHZhciB3YiA9IFsnY2xvc2VkJywnaXNTZWN1cmVDb250ZXh0J107DQogICAgICAgICAgICAgICAgICAgICAgd2IuZm9yRWFjaChmdW5jdGlvbihlKXt3W2VdPWI7fSk7DQogICAgICAgICAgICAgICAgICAgICAgdmFyIHdmID0gWydhZGRFdmVudExpc3RlbmVyJywnYWxlcnQnLCdhdG9iJywnYmx1cicsJ2J0b2EnLCdjYW5jZWxBbmltYXRpb25GcmFtZScsJ2NhcHR1cmVFdmVudHMnLCdjbGVhckludGVydmFsJywnY2xlYXJUaW1lb3V0JywnY2xvc2UnLCdjb25maXJtJywnY3JlYXRlSW1hZ2VCaXRtYXAnLCdkaXNwYXRjaEV2ZW50JywnZmV0Y2gnLCdmaW5kJywnZm9jdXMnLCdnZXRDb21wdXRlZFN0eWxlJywnZ2V0U2VsZWN0aW9uJywnbWF0Y2hNZWRpYScsJ21vdmVCeScsJ21vdmVUbycsJ29wZW4nLCdwb3N0TWVzc2FnZScsJ3Byb21wdCcsJ3JlbGVhc2VFdmVudHMnLCdyZW1vdmVFdmVudExpc3RlbmVyJywncmVxdWVzdEFuaW1hdGlvbkZyYW1lJywncmVzaXplQnknLCdyZXNpemVUbycsJ3Njcm9sbCcsJ3Njcm9sbEJ5Jywnc2Nyb2xsVG8nLCdzZXRJbnRlcnZhbCcsJ3NldFRpbWVvdXQnLCdzdG9wJ107DQogICAgICAgICAgICAgICAgICAgICAgd2YuZm9yRWFjaChmdW5jdGlvbihlKXt3W2VdPWY7fSk7DQogICAgICAgICAgICAgICAgICAgICAgdmFyIHduID0gWydkZXZpY2VQaXhlbFJhdGlvJywnaW5uZXJIZWlnaHQnLCdpbm5lcldpZHRoJywnbGVuZ3RoJywnb3V0ZXJIZWlnaHQnLCdvdXRlcldpZHRoJywncGFnZVhPZmZzZXQnLCdwYWdlWU9mZnNldCcsJ3NjcmVlblgnLCdzY3JlZW5ZJywnc2Nyb2xsWCcsJ3Njcm9sbFknXTsNCiAgICAgICAgICAgICAgICAgICAgICB3bi5mb3JFYWNoKGZ1bmN0aW9uKGUpe3dbZV09bjt9KTsNCiAgICAgICAgICAgICAgICAgICAgICB2YXIgd28gPSBbJ2FwcGxpY2F0aW9uQ2FjaGUnLCdjYWNoZXMnLCdjcnlwdG8nLCdleHRlcm5hbCcsJ2ZyYW1lRWxlbWVudCcsJ2ZyYW1lcycsJ2hpc3RvcnknLCdpbmRleGVkREInLCdsb2NhbFN0b3JhZ2UnLCdsb2NhdGlvbicsJ2xvY2F0aW9uYmFyJywnbWVudWJhcicsJ25hdmlnYXRvcicsJ29uYWJvcnQnLCdvbmFuaW1hdGlvbmVuZCcsJ29uYW5pbWF0aW9uaXRlcmF0aW9uJywnb25hbmltYXRpb25zdGFydCcsJ29uYmVmb3JldW5sb2FkJywnb25ibHVyJywnb25jYW5wbGF5Jywnb25jYW5wbGF5dGhyb3VnaCcsJ29uY2hhbmdlJywnb25jbGljaycsJ29uY29udGV4dG1lbnUnLCdvbmRibGNsaWNrJywnb25kZXZpY2Vtb3Rpb24nLCdvbmRldmljZW9yaWVudGF0aW9uJywnb25kcmFnJywnb25kcmFnZW5kJywnb25kcmFnZW50ZXInLCdvbmRyYWdsZWF2ZScsJ29uZHJhZ292ZXInLCdvbmRyYWdzdGFydCcsJ29uZHJvcCcsJ29uZHVyYXRpb25jaGFuZ2UnLCdvbmVtcHRpZWQnLCdvbmVuZGVkJywnb25lcnJvcicsJ29uZm9jdXMnLCdvbmhhc2hjaGFuZ2UnLCdvbmlucHV0Jywnb25pbnZhbGlkJywnb25rZXlkb3duJywnb25rZXlwcmVzcycsJ29ua2V5dXAnLCdvbmxhbmd1YWdlY2hhbmdlJywnb25sb2FkJywnb25sb2FkZWRkYXRhJywnb25sb2FkZWRtZXRhZGF0YScsJ29ubG9hZHN0YXJ0Jywnb25tZXNzYWdlJywnb25tb3VzZWRvd24nLCdvbm1vdXNlZW50ZXInLCdvbm1vdXNlbGVhdmUnLCdvbm1vdXNlbW92ZScsJ29ubW91c2VvdXQnLCdvbm1vdXNlb3ZlcicsJ29ubW91c2V1cCcsJ29ub2ZmbGluZScsJ29ub25saW5lJywnb25wYWdlaGlkZScsJ29ucGFnZXNob3cnLCdvbnBhdXNlJywnb25wbGF5Jywnb25wbGF5aW5nJywnb25wb3BzdGF0ZScsJ29ucHJvZ3Jlc3MnLCdvbnJhdGVjaGFuZ2UnLCdvbnJlc2V0Jywnb25yZXNpemUnLCdvbnNjcm9sbCcsJ29uc2Vla2VkJywnb25zZWVraW5nJywnb25zZWxlY3QnLCdvbnNob3cnLCdvbnN0YWxsZWQnLCdvbnN0b3JhZ2UnLCdvbnN1Ym1pdCcsJ29uc3VzcGVuZCcsJ29udGltZXVwZGF0ZScsJ29udG9nZ2xlJywnb250cmFuc2l0aW9uZW5kJywnb251bmxvYWQnLCdvbnZvbHVtZWNoYW5nZScsJ29ud2FpdGluZycsJ29ud2Via2l0YW5pbWF0aW9uZW5kJywnb253ZWJraXRhbmltYXRpb25pdGVyYXRpb24nLCdvbndlYmtpdGFuaW1hdGlvbnN0YXJ0Jywnb253ZWJraXR0cmFuc2l0aW9uZW5kJywnb253aGVlbCcsJ29wZW5lcicsJ3BhcmVudCcsJ3BlcmZvcm1hbmNlJywncGVyc29uYWxiYXInLCdzY3JlZW4nLCdzY3JvbGxiYXJzJywnc2VsZicsJ3Nlc3Npb25TdG9yYWdlJywnc3BlZWNoU3ludGhlc2lzJywnc3RhdHVzYmFyJywndG9vbGJhcicsJ3RvcCddOw0KICAgICAgICAgICAgICAgICAgICAgIHdvLmZvckVhY2goZnVuY3Rpb24oZSl7d1tlXT1vO30pOw0KICAgICAgICAgICAgICAgICAgICAgIHZhciB3cyA9IFsnbmFtZSddOw0KICAgICAgICAgICAgICAgICAgICAgIHdzLmZvckVhY2goZnVuY3Rpb24oZSl7d1tlXT1zO30pOw0KICAgICAgICAgICAgICAgICAgICB9KShkb2N1bWVudCwgd2luZG93KTsNCiAgICAgICAgICAgICAgICAgICAgJXM7DQogICAgICAgICAgICAgICAgICAgIHByaW50KGRlY29kZWQpOw==''') % (enc, jscode)                     
#                printDBG("+++++++++++++++++++++++  CODE  ++++++++++++++++++++++++")
#                printDBG(jscode)
#                printDBG("+++++++++++++++++++++++++++++++++++++++++++++++++++++++")
#                print "+++++++++++++++++++++++  CODE  ++++++++++++++++++++++++"
#                print jscode
#                print "+++++++++++++++++++++++++++++++++++++++++++++++++++++++"
#                return self.iptv_js_execute( jscode )
                return self.iptv_js_execute( jscode )

#                ret = self.iptv_js_execute( jscode )
#                if ret['sts'] and 0 == ret['code']:
#                    decoded = ret['data'].strip()
#                    printDBG('DECODED DATA -> [%s]' % decoded)
            except Exception:
#                printExc()
                print "__decode_k error"

            #rm(tmpPath)
            return decoded

#        marker = 'ﾟωﾟﾉ= /｀ｍ´）ﾉ'
        marker = 'ﾟωﾟﾉ= /｀ｍ'

#        marker = 'ﾟωﾟ'

        orgData = marker + self.getDataBeetwenMarkers(orgData, marker, marker, False)[1]
        orgData = re.sub('''if\s*\([^\}]+?typeof[^\}]+?\}''', '', orgData)
        orgData = re.sub('''if\s*\([^\}]+?document[^\}]+?\}''', '', orgData)
#        dec = self.__decode_k(encTab[0], orgData)
        dec = __decode_k(encTab[0], orgData)
        
        videoUrl = 'https://openload.co/stream/{0}?mime=true'.format(dec)
        params = dict(HTTP_HEADER)
        params['external_sub_tracks'] = subTracks
        print videoUrl
#        print "params:", params


    def get_url(self, host, media_id):
        return 'https://openload.co/embed/%s' % (media_id)

    def __auth_ip(self, media_id):
        js_data = self.__get_json(INFO_URL)
        pair_url = js_data.get('result', {}).get('auth_url', '')
        if pair_url:
            pair_url = pair_url.replace('\/', '/')
            header = i18n('ol_auth_header')
            line1 = i18n('auth_required')
            line2 = i18n('visit_link')
            line3 = i18n('click_pair') % (pair_url)
            with common.kodi.CountdownDialog(header, line1, line2, line3) as cd:
                return cd.start(self.__check_auth, [media_id])
        
    def __check_auth(self, media_id):
        try:
            js_data = self.__get_json(GET_URL.format(media_id=media_id))
        except ResolverError as e:
#        except Exception as e:
            status, mscommandg = e
            if status == 403:
                print "errormsg=%s" % (e)
                return
            else:
                print "errormsg=%s" % (msg)
#                raise ResolverError(msg)
        
        return js_data.get('result', {}).get('url')
    
    def __get_json(self, url):
        result = self.net.http_GET(url).content
        js_result = json.loads(result)
#        common.log_utils.log_debug(js_result)
#        if js_result['status'] != 200:
#            raise ResolverError(js_result['status'], js_result['msg'])
        return js_result


sys.stdout = OpenLoadResolver()
