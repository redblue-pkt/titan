
import re
import sys
import time
from lib.net import Net
import re, urllib, json
from lib import helpers
import lib.common as common

class MixdropResolver(object):
    name = "mixdrop"
    domains = ["mixdrop.co", "mixdrop.to"]
    pattern = r'(?://|\.)(mixdrop\.[ct]o)/(?:f|e)/(\w+)'

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
        headers = {'User-Agent': common.FF_USER_AGENT}
        response = self.net.http_GET(web_url, headers=headers)

        html = self.net.http_GET(web_url, headers=headers).content

        if '(p,a,c,k,e,d)' in html:
            html = helpers.get_packed_data(html)
        r = re.search(r'vsr[^=]+=\s*"([^"]+)', html)
        if r:
            headers = {'User-Agent': common.RAND_UA}
            print "https:" + r.group(1) + helpers.append_headers(headers)
	else:
            print 'errormsg=File Not Found or removed'


    def get_url(self, host, media_id):
        return 'https://%s/e/%s' % (host, media_id)

sys.stdout = MixdropResolver()
