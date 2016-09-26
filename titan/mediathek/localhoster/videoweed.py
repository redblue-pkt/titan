
import re
import sys
from lib.net import Net

class VideoweedResolver(object):
    name = 'videoweed.es'
    domains = ['bitvid.sx', 'videoweed.es', 'videoweed.com']
    pattern = '(?://|\.)(bitvid.sx|videoweed.es|videoweed.com)/(?:mobile/video\.php\?id=|video/|embed/\?v=|embed\.php\?v=|file/)([0-9a-z]+)'

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
        stream_url = ''

        r = re.search('flashvars.filekey=(.+?);', html)
        if r:
            r = r.group(1)

            try: filekey = re.compile('\s+%s="(.+?)"' % r).findall(html)[-1]
            except: filekey = r

            player_url = 'http://www.bitvid.sx/api/player.api.php?key=%s&file=%s' % (filekey, media_id)

            html = self.net.http_GET(player_url).content

            r = re.search('url=(.+?)&', html)

            if r:
                stream_url = r.group(1)
#            else:
#                raise ResolverError('File Not Found or removed')

        print stream_url

    def get_url(self, host, media_id):
        return 'http://www.bitvid.sx/embed/?v=%s' % media_id

sys.stdout = VideoweedResolver()
