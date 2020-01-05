
import re, time
from lib import jsunpack
import sys
from lib.net import Net
import lib.common as common
import urllib2

class FlashxResolver(object):
    name = "flashx"
    domains = ["flashx.tv"]
    pattern = '(?://|\.)(flashx\.tv)/(?:embed-|dl\?|embed.php\?c=)?([0-9a-zA-Z/-]+)'

    def __init__(self):
        self.net = Net()
        url = str(sys.argv[1])
        return self.get_media_url(url)

    def get_media_url(self, url):
        html = self.net.http_GET(url).content
        js_data = re.findall('(eval\(function.*?)</script>', html.replace('\n', ''))
 
        for i in js_data:
            try: html += jsunpack.unpack(i)
            except: pass
        print "html", html
                        
sys.stdout = FlashxResolver()
