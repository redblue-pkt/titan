
import re
import urllib
from lib import jsunpack
import sys
from lib.net import Net

class VidziResolver(object):
    name = "vidzi"
    domains = ["vidzi.tv"]
    pattern = '(?://|\.)(vidzi\.tv)/(?:embed-)?([0-9a-zA-Z]+)'

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
        html = self.net.http_GET(web_url).content

        if '404 Not Found' in html:
            print 'File Not Found or removed'

        r = re.search('file\s*:\s*"([^"]+)', html)
        if r:
            return r.group(1) + '|' + urllib.urlencode({'Referer': 'http://vidzi.tv/nplayer/jwplayer.flash.swf'})
        else:
            for match in re.finditer('(eval\(function.*?)</script>', html, re.DOTALL):
                js_data = jsunpack.unpack(match.group(1))
                r = re.search('file\s*:\s*"([^"]+)', js_data)
                if r:
                    print r.group(1)

#        print 'Unable to locate link'

    def get_url(self, host, media_id):
        return 'http://%s/embed-%s.html' % (host, media_id)

sys.stdout = VidziResolver()
