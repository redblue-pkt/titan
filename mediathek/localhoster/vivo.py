
import re
import urllib
from lib import jsunpack
import sys
from lib.net import Net
import base64
import json

class VivoResolver(object):
    name = "vivosx"
    domains = ["vivo.sx"]
    pattern = '(?://|\.)(vivo\.sx)/([0-9a-zA-Z]+)'

    def __init__(self):
        self.net = Net()
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

        # get landing page
        resp = self.net.http_GET(web_url, headers={'Referer': web_url})
        html = resp.content

        r = re.search(r'Core\.InitializeStream \(\'(.*?)\'\)', html)
        if not r: print 'page structure changed'

        b = base64.b64decode(r.group(1))
        j = json.loads(b)

        if len(j) == 0: print 'video not found'

        print j[0]

    def get_url(self, host, media_id):
        return 'http://%s/%s' % (host, media_id)

sys.stdout = VivoResolver()
