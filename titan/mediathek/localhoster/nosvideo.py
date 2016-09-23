
import sys
import re
from net import Net

class NosvideoResolver(object):
    name = "nosvideo"
    domains = ["nosvideo.com", "noslocker.com"]
    pattern = '(?://|\.)(nosvideo.com|noslocker.com)/(?:\?v\=|embed/|.+?\u=)?([0-9a-zA-Z]+)'

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

        if 'File Not Found' in html:
            raise ResolverError('File Not Found')

        web_url = 'http://nosvideo.com/vj/video.php?u=%s&w=&h=530' % media_id

        html = self.net.http_GET(web_url).content

        smil_url = re.compile('\':\'(.+?)\'').findall(html)
        smil_url = [i for i in smil_url if '.smil' in i][0]

        html = self.net.http_GET(smil_url).content

        streamer = re.findall('base\s*=\s*"(.+?)"', html)[0]
        playpath = re.findall('src\s*=\s*"(.+?)"', html)[0]

        stream_url = '%s playpath=%s' % (streamer, playpath)

        print stream_url

    def get_url(self, host, media_id):
        return 'http://nosvideo.com/%s' % media_id

sys.stdout = NosvideoResolver()

