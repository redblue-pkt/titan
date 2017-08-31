
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

    def get_media_url(self, host, media_id):
        web_url = self.get_url(host, media_id)
        headers = {
            'Referer': web_url
        }
        headers.update(self.headers)
        html = self.net.http_GET(web_url, headers=headers).content

        r = re.search('sources:\s*(\[.*?\])', html, re.DOTALL)

        if r:
            sources = json.loads(r.group(1))
            max_label = 0
            stream_url = ''
            for source in sources:
                if 'label' in source and int(re.sub('[^0-9]', '', source['label'])) > max_label:
                    stream_url = source['file']
                    max_label = int(re.sub('[^0-9]', '', source['label']))

        varname = re.search('''concat\(\s*['"]/["']\s*\+([^\+]+?)\+''', html).group(1)

#        authkey = re.search('''var lets_play_a_game=\'(.*)\'''', html).group(1)
#        print "authkey2", authkey

        authkey = re.search(r"=\'(.*)\'", html).group(1)

#        my_regex = r"var " + re.escape(varname) + r"""\s*=\s*['"]([^'^"]+?)['"]"""
#        my_regex = re.escape(varname) + r"""\s*=\s*['"]([^'^"]+?)['"]"""
#
#        test = re.search(my_regex, html, re.IGNORECASE)
#        print "test", test

        web_url = "https://thevideo.me/vsign/player/" + authkey
        html = self.net.http_GET(web_url, headers=headers).content

        js_data = jsunpack.unpack(html)
        for match in re.finditer('(eval\(function.*?)\{\}\)\)', html, re.DOTALL):
            js_data = jsunpack.unpack(match.group(1))
            ua = re.search('"ua=(.*?)"', js_data).group(1)
            vt = re.search('"vt=(.*?)"', js_data).group(1)
            print '%s?direct=false&ua=%s&vt=%s' % (stream_url, ua, vt)

        
    def get_url(self, host, media_id):
        return 'http://%s/embed-%s-640x360.html' % (host, media_id)

sys.stdout = TheVideoResolver()
