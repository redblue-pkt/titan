
import re
import sys
from lib.net import Net
from lib import helpers
import lib.common as common

class VidtoResolver(object):
    name = "vidto"
    domains = ["vidto.me"]
    pattern = '(?://|\.)(vidto\.me)/(?:embed-)?([0-9a-zA-Z]+)'

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
        html = helpers.add_packed_data(html)
        sources = []
        for match in re.finditer('label:\s*"([^"]+)"\s*,\s*file:\s*"([^"]+)', html):
            label, stream_url = match.groups()
            sources.append((label, stream_url))

        sources = sorted(sources, key=lambda x: x[0])[::-1]
        print helpers.pick_source(sources) + helpers.append_headers(headers)
#        print helpers.pick_source(sources)

#        raise ResolverError("File Link Not Found")

#    def get_url(self, host, media_id):
#        return self._default_get_url(host, media_id)

    def get_url(self, host, media_id):
        return 'http://%s/embed-%s.html' % (host, media_id)

sys.stdout = VidtoResolver()
