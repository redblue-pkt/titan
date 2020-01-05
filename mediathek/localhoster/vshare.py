
import re, time
from lib import jsunpack
import sys
from lib.net import Net
import lib.common as common
import urllib2
from lib import helpers

class VshareResolver(object):
    name = "flashx"
    domains = ["flashx.tv"]
    pattern = '(?://|\.)(flashx\.net|flashx\.tv)/(?:embed-|dl\?|embed.php\?c=)?([0-9a-zA-Z/-]+)'

    def __init__(self):
        self.net = Net()
        url = str(sys.argv[1])
        return self.get_media_url(url)

    def isValidUrl(self, url):
        return url.startswith('http://') or url.startswith('https://')

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

    def __getJS(self, data, params):
        tmpUrls = re.compile("""<script[^>]+?src=['"]([^'^"]+?)['"]""", re.IGNORECASE).findall(data)
#        print "tmpUrls: ", tmpUrls
        codeUrl = 'https://www.flashx.tv/js/code.js'
        for tmpUrl in tmpUrls:
#            print "tmpUrl: ", tmpUrl

            if tmpUrl.startswith('.'):
                tmpUrl = tmpUrl[1:]
            if tmpUrl.startswith('//'):
                tmpUrl = 'https:' + tmpUrl
            if tmpUrl.startswith('/'):
                tmpUrl = 'https://www.flashx.tv' + tmpUrl
            if self.isValidUrl(tmpUrl): 
                if ('flashx' in tmpUrl and 'jquery' not in tmpUrl and '/code.js' not in tmpUrl and '/coder.js' not in tmpUrl): 
#                    print '$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$'
                    #sts, tmp = self.cm.getPage(tmpUrl.replace('\n', ''), params)
                    
                    try: tmp = self.net.http_GET(tmpUrl.replace('\n', ''), params).content
                    except: pass
                elif '/code.js' in tmpUrl or '/coder.js' in tmpUrl:
                    codeUrl = tmpUrl
            
#        sts, tmp = self.cm.getPage(codeUrl, params)
        tmp = self.net.http_GET(codeUrl, params).content
        tmp = self.getAllItemsBeetwenMarkers(tmp, 'function', ';');
        for tmpItem in tmp:
            tmpItem = tmpItem.replace(' ', '')
            if '!=null' in tmpItem:
                tmpItem   = self.getDataBeetwenMarkers(tmpItem, 'get(', ')')[1]
                tmpUrl    = self.getSearchGroups(tmpItem, """['"](https?://[^'^"]+?)['"]""")[0]
                if not self.isValidUrl(tmpUrl): continue
                getParams = self.getDataBeetwenMarkers(tmpItem, '{', '}', False)[1]
                getParams = getParams.replace(':', '=').replace(',', '&').replace('"', '').replace("'", '')
                tmpUrl += '?' + getParams
#                sts, tmp = self.cm.getPage(tmpUrl, params)
                tmp = self.net.http_GET(tmpUrl, params).content
                break

    def get_media_url(self, url):

        resp = self.net.http_GET(url)
        html = resp.content

        if re.search('>Security Check<', html):
            print 'errormsg=Too many views per minute. We think you are a spider, crawler, ddoser or something'
            return

#<Center>
#<b><font color="red">Security Check</font></b><br><br>
#<br><br>
#Too many views per minute. We think you are a spider, crawler, ddoser or something else.<br>
#If you are human, please contact <a href="/cdn-cgi/l/email-protection" class="__cf_email__" data-cfemail="fd9c9f888e9889989c90bd9b919c8e95a5d3898b">[email&#160;protected]</a> and <b>whitelist</b> your IP (91.137.28.121).<br>
#Or try it again a little bit later.
#<br><br>
#<center>
#<a href="http://trk.globwo.online/377b640b-3ab1-4b42-acec-3b6ff8ef1a2e" target="_blank"><img src="https://static.flashx.net/img/player.png" border="0"></a>
#</center>

#        print "# 1 start ##########################"
#        print html
#        print "# 1 end ##########################"

#       return
#        cfdcookie = resp._response.info()['set-cookie']
#        cfduid = re.search('cfduid=(.*?);', cfdcookie).group(1)
#        file_id = re.search("'file_id', '(.*?)'", html).group(1)
#        aff = re.search("'aff', '(.*?)'", html).group(1)
        headers = {'User-Agent': common.IE_USER_AGENT,
                   'Referer': url}

#        surl = re.search('src="(.*?' + file_id + ')', html, re.IGNORECASE).group(1)
#        dummy = self.net.http_GET(url=surl, headers=headers).content
        headers = {'User-Agent': common.IE_USER_AGENT,
                   'Referer': url}
        html = self.net.http_GET(url=url, headers=headers).content

        fname = re.search('name="fname" value="(.*?)"', html).group(1)
#        hash = re.search('name="hash" value="(.*?)"', html).group(1)
        id = re.search('name="id" value="(.*?)"', html).group(1)
        method_free = re.search('name="method_free" value="(.*?)"', html).group(1)
#        action = re.search('method="POST" action="(.*?)"', html).group(1)
        fdata = {'op': 'download1',
                 'usr_login': '',
                 'id': id,
                 'fname': fname,
                 'referer': '',
                 'method_free': method_free}
#                 'imhuman': 'Proceed to video'
#        furl = 'http://www.flashx.tv/dl'# + media_id
#                <Form method="POST" action='' class="center">
#                    <input type="hidden" name="op" value="download1">
#                    <input type="hidden" name="usr_login" value="">
#                    <input type="hidden" name="id" value="zg5c3tovyst1">
#                    <input type="hidden" name="fname" value="Hackers 2 Operation Takedown-kinox.to(575)-bySGexx.mp4.mp4">
#                    <input type="hidden" name="referer" value="">
#                    <input class="large button strong proceed" id="play" style="outline: none; width: 200px" type="submit" name="method_free" value="Proceed to video">
#                </Form>

#        sleeptime = re.search('>([0-9])</span> seconds<', html).group(1)
#        print "action: ", action
#        print "fdata: ", fdata
#op=download1&usr_login=&id=zg5c3tovyst1&fname=Hackers+2+Operation+Takedown-kinox.to%28575%29-bySGexx.mp4.mp4&referer=&method_free=Proceed+to+video
#<source src="https://vs096.vshare.eu:182/d/baemfeil7hnroluvocdqwo7wa6h4ufqgjdeesgubqcd6wm6xrz2o26bh/vid.mp4" type="video/mp4">

#        print "headers: ", headers

#        print "sleeptime", sleeptime

        self.__getJS(html, headers)

        time.sleep(8)
        html = self.net.http_POST(url=url, form_data=fdata, headers=headers).content
#        print "# 2 start ##########################"
#        print html
#        print "# 2 end ##########################"

        js_data = re.findall('(eval\(function.*?)</script>', html.replace('\n', ''))
#        print "# 3 start ##########################"
#        print js_data
#        print "# 3 end ##########################"

        for i in js_data:
            try: html += jsunpack.unpack(i)
            except: pass

#        print "# 3 start ##########################"
#        print html
#        print "# 3 end ##########################"

# <center><font size="4">You try to access the video with Kodi / XBMC / TV BOX or AdBlock.<br><br>
# <font color="red">This is prohibited!</font></font><br><br><br>
# <font size="2">If we made a mistake, please <a href="https://www.flashx.net/contact.html" target="_blank">contact us via contact form</a>!<br>
# Or disable your <b>adblock addon</b> and try again.<br><br>
# Or try to reload the page! <a href="https://www.flashx.net/reloadit.php?w=d&c=20988890&i=zsy14viyeh2j">!! <b><font color="red">Click here</font></b> !!</a><br><br></font></center>
# <br><br>

        if re.search('>You try to access the video with Kodi / XBMC / TV BOX or AdBlock.<', html):
            print 'errormsg=You try to access the video with Kodi / XBMC / TV BOX or AdBlock.'
            return

#<source src="https://vs096.vshare.eu:182/d/baemfeil7hnroluvocdqwo7wa6h4ufqgjdeesgubqcd6wm6xryzfmrfx/vid.mp4" type="video/mp4">
        #{src: 'https://flashx1.tv/cdn107/5k7x22t6vivfjuw5lwxyzw2fsdqgparrhy55dd75c464tmacqfoh7ooeg7mq/normal.mp4',type: 'video/mp4',label: 'SD',res: 360},

        print re.search('<source src="([^"].*)" type', html).group(1)
                 
sys.stdout = VshareResolver()
