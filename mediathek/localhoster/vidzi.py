
import re
import urllib
from lib import jsunpack
import sys
from lib.net import Net
from lib import helpers


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
        return helpers.get_media_url(self.get_url(host, media_id))

    def get_url(self, host, media_id):
        return 'http://%s/embed-%s.html' % (host, media_id)

sys.stdout = VidziResolver()
