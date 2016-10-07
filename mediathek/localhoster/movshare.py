
import re
import urllib
import sys
from lib.net import Net

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
        html = self.net.http_GET(web_url).content
        try:
            stream_url = ''
            r = re.search('flashvars.filekey=(.+?);', html)
            if r:
                r = r.group(1)
                try: filekey = re.search('var\s+%s\s*=\s*"([^"]+)' % (r), html).group(1)
                except: filekey = r
                player_url = 'http://%s/api/player.api.php?key=%s&file=%s' % (host, urllib.quote(filekey), media_id)
                html = self.net.http_GET(player_url).content
                r = re.search('url=(.+?)&', html)
                if r:
                    stream_url = r.group(1)
            
        try:
            r = re.search('id="player"[^>]+src="([^"]+)', html, re.DOTALL)
            if r:
                stream_url = r.group(1)

        if stream_url:
            print '%s|Referer=%s' % (stream_url, web_url)

    def get_url(self, host, media_id):
        if 'vidgg' in host:
            return 'http://%s/embed/?id=%s' % (host, media_id)
        else:
            return 'http://%s/embed/?v=%s' % (host, media_id)

sys.stdout = MovshareResolver()
