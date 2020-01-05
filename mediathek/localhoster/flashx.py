
import re, time
from lib import jsunpack
import sys
from lib.net import Net
import lib.common as common
import urllib2
from lib import helpers

class FlashxResolver(object):
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
        if not url.endswith('.jsp') and url.endswith('.html'):
            if '.tv/embed-' not in url:
                url = url.replace('.tv/', '.tv/embed-')
            if not url.endswith('.html'):
                url += '.html'

#            print "url", url
            resp = self.net.http_GET(url)
            html = resp.content

            cfdcookie = resp._response.info()['set-cookie']
            cfduid = re.search('cfduid=(.*?);', cfdcookie).group(1)
            headers = {'User-Agent': common.IE_USER_AGENT,
                   'Referer': url,
                   'Cookie': '__cfduid=' + cfduid + '; lang=1'}

            self.__getJS(html, headers)

            #<input type="image" src="https://static.flashx.net/exp/playnow.png" onclick="location.href='https://www.flashx.net/playvideo-619d0419488c19e5216f83b2e010f3df.html?playvid'" style="margin-left:auto;margin-right:auto;display:block;margin-top:15%;margin-bottom:0%" />
            url = re.search('onclick="location.href=\'(.*?)\'', html).group(1)
#            print "url", url
            
            html = self.net.http_GET(url, headers).content

            js_data = re.findall('(eval\(function.*?)</script>', html.replace('\n', ''))
 
            for i in js_data:
                try: html += jsunpack.unpack(i)
                except: pass

   #         print "html", html
            print re.search("{src: '([^'].*)',type:", html).group(1)

            return

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
        cfdcookie = resp._response.info()['set-cookie']
        cfduid = re.search('cfduid=(.*?);', cfdcookie).group(1)
        file_id = re.search("'file_id', '(.*?)'", html).group(1)
        aff = re.search("'aff', '(.*?)'", html).group(1)
        headers = {'User-Agent': common.IE_USER_AGENT,
                   'Referer': url,
                   'Cookie': '__cfduid=' + cfduid + '; lang=1'}

#        surl = re.search('src="(.*?' + file_id + ')', html, re.IGNORECASE).group(1)
#        dummy = self.net.http_GET(url=surl, headers=headers).content
        headers = {'User-Agent': common.IE_USER_AGENT,
                   'Referer': url,
                   'Cookie': '__cfduid=' + cfduid + '; lang=1; file_id=' + file_id + '; aff=' + aff}
        html = self.net.http_GET(url=url, headers=headers).content

        fname = re.search('name="fname" value="(.*?)"', html).group(1)
        hash = re.search('name="hash" value="(.*?)"', html).group(1)
        id = re.search('name="id" value="(.*?)"', html).group(1)
        action = re.search('method="POST" action="(.*?)"', html).group(1)
        fdata = {'op': 'download1',
                 'usr_login': '',
                 'id': id,
                 'fname': fname,
                 'referer': '',
                 'hash': hash}
#                 'imhuman': 'Proceed to video'
#        furl = 'http://www.flashx.tv/dl'# + media_id

        sleeptime = re.search('>([0-9])</span> seconds<', html).group(1)
#        print "action: ", action
#        print "fdata: ", fdata
#        print "headers: ", headers

#        print "sleeptime", sleeptime

        self.__getJS(html, headers)

        time.sleep(8)
        html = self.net.http_POST(url=action, form_data=fdata, headers=headers).content
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


        #{src: 'https://flashx1.tv/cdn107/5k7x22t6vivfjuw5lwxyzw2fsdqgparrhy55dd75c464tmacqfoh7ooeg7mq/normal.mp4',type: 'video/mp4',label: 'SD',res: 360},

        print re.search("{src: '([^'].*)',type:", html).group(1)
                 
sys.stdout = FlashxResolver()
