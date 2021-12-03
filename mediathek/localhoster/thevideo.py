
import re
import json
import urllib2
import time
from lib import helpers
import sys
from lib.net import Net
import lib.common as common
from lib import jsunpack
import re, urllib, json

INTERVALS = 5

class TheVideoResolver(object):
    name = "thevideo"
    domains = ["thevideo.me"]
    pattern = '(?://|\.)(streamcrypt\.net|thevideo\.me|thevideo\.cc|vev\.io|tvad./me)/(?:embed-|download/)?([0-9a-zA-Z]+)'

    def __init__(self):
#        self.net = Net()
        self.net = Net(cookie_file='/mnt/network/cookies', http_debug = False)
        self.headers = {'User-Agent': common.ANDROID_USER_AGENT}
        url = str(sys.argv[1])
#        print "url", url
        host = self.get_host_and_id(url)[0]
        media_id = self.get_host_and_id(url)[1]
        return self.get_media_url(url)
#        return self.get_media_url(host, media_id)

    def get_host_and_id(self, url):
        r = re.search(self.pattern, url, re.I)
        if r:
            return r.groups()
        else:
            return False

    def get_media_url(self, url):
#        web_url = self.get_url(host, media_id)
        web_url = url

        headers = {
            'Referer': web_url
        }
        headers.update(self.headers)

        response = self.net.http_GET(web_url, headers=headers).content
        js_data = re.findall('(eval\(function.*?)</script>', response.replace('\n', ''))
#        print "js_data1", js_data
        for i in js_data:
            response += jsunpack.unpack(i)
#        print "js_data2", js_data

#        print "response", response
        ret = self.net.save_cookies('/mnt/network/cookies')
#        videoCode = self.getSearchGroups(response, r'''['"]video_code['"]\s*:\s*['"]([^'^"]+?)['"]''')[0]
        videoCode = re.findall("video/mp4.*'(.*?)\\\\'.*;", response)
#        print videoCode[0]

        sStreamUrl = 'https://thevideome.com/%s' % (videoCode[0])
        sUrl = self.redirectHoster(sStreamUrl)
        print sUrl

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
 
    def get_url(self, host, media_id):
        return 'http://%s/embed-%s-640x360.html' % (host, media_id)

    def redirectHoster(self, url):
        try:
            from urllib2 import build_opener, HTTPError
        except ImportError:
            from urllib.error import HTTPError
            from urllib.request import build_opener
        opener = build_opener()
        opener.addheaders = [('Referer', url)]
        try:
            resp = opener.open(url)
            if url != resp.geturl():
                return resp.geturl()
            else:
                return url
        except HTTPError as e:
            if e.code == 403:
                if url != e.geturl():
                    return e.geturl()
            raise

sys.stdout = TheVideoResolver()
