
import re
import json
import urllib2
import time
from lib import helpers
import sys
from lib.net import Net
import lib.common as common
from lib import jsunpack

INTERVALS = 5

class TheVideoResolver(object):
    name = "thevideo"
    domains = ["thevideo.me"]
    pattern = '(?://|\.)(thevideo\.me)/(?:embed-|download/)?([0-9a-zA-Z]+)'

    def __init__(self):
        self.net = Net()
        self.headers = {'User-Agent': common.IE_USER_AGENT}
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

    def get_media_url(self, host, media_id):
        web_url = self.get_url(host, media_id)
        headers = {
            'Referer': web_url
        }
        headers.update(self.headers)
        headers = {'User-Agent': common.IE_USER_AGENT, 'Referer': web_url}

        html = self.net.http_GET(web_url, headers=headers).content
#        print "111111111111", html.encode('utf-8').strip()
        old = 0
        if old:
            vhash = re.search('\'_vhash\', value: \'(.*?)\'', html).group(1)
            gfk = re.search('\'gfk\', value: \'(.*?)\'', html).group(1)

            fname = re.search('name="fname" value="(.*?)"', html).group(1)
            op = re.search('name="op" value="(.*?)"', html).group(1)
            inhu = re.search('name="inhu" value="(.*?)"', html).group(1)
            usr_login = re.search('name="usr_login" value="(.*?)"', html).group(1)

            hash = re.search('name="hash" value="(.*?)"', html).group(1)
            fdata = {'_vhash': vhash,
                     'gfk': gfk,
                     'op': op,
                     'usr_login': usr_login,
                     'id': media_id,                 
                     'fname': fname,
                     'referer': '',
                     'hash': hash,
                     'imhuman': 'Proceed to video',
                     'inhu': inhu}

            html = self.net.http_POST(url=web_url, form_data=fdata, headers=headers).content
#            print "2222222222", html.encode('utf-8').strip()

#http://thevideo.me/jwv/LDonSU04MylZO1ZNSThGPUEK
        r = re.search('sources:\s*(\[.*?\])', html, re.DOTALL)

        if r:
            sources = json.loads(r.group(1))
            max_label = 0
            stream_url = ''
            for source in sources:
                if 'label' in source and int(re.sub('[^0-9]', '', source['label'])) > max_label:
                    stream_url = source['file']
                    max_label = int(re.sub('[^0-9]', '', source['label']))

        for match in re.finditer('(eval\(function.*?)</script>', html, re.DOTALL):
            js_data = jsunpack.unpack(match.group(1))
            path = re.search('\'rc=".*(/.*?)\\\'.concat', js_data).group(1)
            path = path.replace('\\', '')
            if path:
                break

        mpri_Key = re.search('var mpri_Key=\'(.*?)\'', html).group(1)
        web_url = self.get_aturl(host, path, mpri_Key)
        html = self.net.http_GET(web_url, headers=headers).content
#        print "3333333333", html.encode('utf-8').strip()

        js_data = jsunpack.unpack(html)
        for match in re.finditer('(eval\(function.*?)\{\}\)\)', html, re.DOTALL):
            js_data = jsunpack.unpack(match.group(1))
            vt = re.search('"vt=(.*?)"', js_data).group(1)
            print '%s?direct=false&ua=1&vt=%s' % (stream_url, vt)
        
    def get_url(self, host, media_id):
        return 'http://%s/%s' % (host, media_id)

    def get_aturl(self, host, path, key):
        return 'http://%s%s/%s' % (host, path, key)

sys.stdout = TheVideoResolver()