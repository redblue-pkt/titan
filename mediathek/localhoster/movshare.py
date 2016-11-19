
import random
import re
import urllib
import sys
from lib.net import Net
from lib import helpers

class MovshareResolver(object):
    name = "movshare"
    domains = ["movshare.net", 'wholecloud.net', 'vidgg.to']
    pattern = '(?://|\.)(movshare.net|wholecloud.net|vidgg.to)/(?:video/|embed(?:/|\.php)\?(?:v|id)=)([A-Za-z0-9]+)'

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
        html = self.net.http_GET(web_url, headers=headers).content
        stream_url = ''
        match = re.search('<video.*?</video>', html, re.DOTALL)
        if match:
            links = re.findall('<source[^>]+src="([^"]+)', match.group(0), re.DOTALL)
            if links:
                stream_url = random.choice(links)
        
        if not stream_url:
            match = re.search('fkzd="([^"]+)', html)
            if match:
                query = {'pass': 'undefined', 'key': match.group(1), 'cid3': 'undefined', 'cid': 0, 'numOfErrors': 0, 'file': media_id, 'cid2': 'undefined', 'user': 'undefined'}
                api_url = 'http://www.wholecloud.net//api/player.api.php?' + urllib.urlencode(query)
                html = self.net.http_GET(api_url, headers=headers).content
                match = re.search('url=([^&]+)', html)
                if match:
                    stream_url = match.group(1)

        if stream_url:
            headers.update({'Referer': web_url, })
            print stream_url + helpers.append_headers(headers)

    def get_url(self, host, media_id):
        if 'vidgg' in host:
            template = 'http://{host}/embed/?id={media_id}'
        else:
            template = 'http://{host}/embed/?v={media_id}'
        print self._default_get_url(host, media_id, template)

sys.stdout = MovshareResolver()
