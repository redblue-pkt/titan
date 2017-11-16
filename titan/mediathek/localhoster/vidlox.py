
import sys
from lib.net import Net

import re, urllib, json
from lib import helpers
import lib.common as common

class VidloxResolver(object):
    name = "vidlox"
    domains = ['vidlox.tv']
    pattern = '(?://|\.)(vidlox\.tv)/(?:embed-)?([0-9a-zA-Z]+)'

    def __init__(self):
#        self.net = Net()
#        self.net = Net(cookie_file='/mnt/network/cookies', http_debug = True)
        self.net = Net(cookie_file='/mnt/network/cookies', http_debug = False)
 
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
        headers = {'User-Agent': common.IE_USER_AGENT, 'Referer': web_url}
        html = self.net.http_GET(web_url, headers=headers).content
        try: sources = re.search(r'sources\s*:\s*\[(.+?)\]', html).groups()[0]
        except: print 'errormsg=Unable to locate link'

        if sources:
            sources = helpers.scrape_sources(sources, patterns=['''["'](?P<url>http[^"']+)'''])
            if sources: print helpers.pick_source(sources) + helpers.append_headers(headers)
            
#        raise ResolverError('Unable to locate link')

    def get_url(self, host, media_id):
        return 'http://%s/embed-%s.html' % (host, media_id)

sys.stdout = VidloxResolver()
