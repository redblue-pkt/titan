
import sys
import re
from lib.net import Net
import lib.common as common
from lib import helpers

class NowvideoResolver(object):
    name = "nowvideo"
    domains = ['nowvideo.eu', 'nowvideo.ch', 'nowvideo.sx', 'nowvideo.co', 'nowvideo.li', 'nowvideo.fo', 'nowvideo.at', 'nowvideo.ec']
    pattern = '(?://|\.)(nowvideo\.(?:eu|ch|sx|co|li|fo|at|ec))/(?:video/|embed\.php\?\S*v=|embed/\?v=)([A-Za-z0-9]+)'

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
        stream_url = ''
        html = self.net.http_GET(web_url).content
        try:
            r = re.search('flashvars.filekey=(.+?);', html)
            if r:
                r = r.group(1)

                try: filekey = re.compile('\s+%s="(.+?)"' % r).findall(html)[-1]
                except: filekey = r

                player_url = 'http://www.nowvideo.sx/api/player.api.php?key=%s&file=%s' % (filekey, media_id)

                html = self.net.http_GET(player_url).content

                r = re.search('url=(.+?)&', html)

                if r:
                    stream_url = r.group(1)
        except:
            print "no embedded urls found using first method"
            
        try:
            r = re.search('id="player".*?src="(.*?)"', html, re.DOTALL)
            if r:
                stream_url = r.group(1)
            
        except:
            print "no embedded urls found using second method"

        if stream_url:
            print stream_url + helpers.append_headers({'Referer': web_url})

    def get_url(self, host, media_id):
        return 'http://embed.nowvideo.sx/embed/?v=%s' % media_id

sys.stdout = NowvideoResolver()