
import re
import urllib
from lib import jsunpack
import sys
from lib.net import Net

MAX_TRIES = 5

class YouWatchResolver(object):
    name = "youwatch"
    domains = ["youwatch.org", "chouhaa.info"]
    pattern = '(?://|\.)(youwatch\.org|chouhaa\.info)/(?:embed-)?([A-Za-z0-9]+)'

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
        web_url = self.net.http_GET(web_url)._response.url
        headers = {'Referer': web_url}

        tries = 0
        while tries < MAX_TRIES:
            html = self.net.http_GET(web_url).content
            html = html.replace('\n', '')
            r = re.search('<iframe\s+src\s*=\s*"([^"]+)', html)
            if r:
                web_url = r.group(1)
            else:
                break
            tries += 1
        
        html = self.net.http_GET(web_url, headers=headers).content
        r = re.search('file\s*:\s*"([^"]+)', html)
        if r:
            print r.group(1) + '|' + urllib.urlencode({'Referer': web_url})

    def get_url(self, host, media_id):
        return 'http://youwatch.org/embed-%s.html' % media_id

sys.stdout = YouWatchResolver()
