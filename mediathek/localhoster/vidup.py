
import sys
import re
from lib import jsunpack
from lib.net import Net

class VidUpMeResolver(object):
    name = "vidup.me"
    domains = ["vidup.me", "beta.vidup.me"]
    pattern = '(?://|\.)(vidup\.me)/(?:embed-)?([0-9a-zA-Z]+)'

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

        js_data = re.findall('(eval\(function.*?)</script>', html.replace('\n', ''))

        for i in js_data:
            try: html += jsunpack.unpack(i)
            except: pass

        match = re.findall('''["']?sources['"]?\s*:\s*\[(.*?)\]''', html)

        if match:
            stream_url = re.findall('''['"]?file['"]?\s*:\s*['"]?([^'"]+)''', match[0])
            if stream_url:
                 print stream_url[-1]

    def get_url(self, host, media_id):
        return 'http://beta.vidup.me/embed-%s.html' % media_id


sys.stdout = VidUpMeResolver()
