
import re
import json
import urllib
import urlparse
from lib import helpers
import sys
from lib.net import Net
from lib.common import common

class AllmyvideosResolver(object):
    name = "allmyvideos"
    domains = ["allmyvideos.net"]
    pattern = '(?://|\.)(allmyvideos\.net)/(?:embed-)?([0-9a-zA-Z]+)'

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
        url = self.get_url1st(host, media_id)
        headers = {'User-Agent': common.IE_USER_AGENT, 'Referer': url}
        html = self.net.http_GET(url, headers=headers).content
        stream_url = self.__get_best_source(html)
        if stream_url:
#            xbmc.sleep(2000)
            print stream_url

        url = self.get_url(host, media_id)
        headers = {'User-Agent': common.IE_USER_AGENT, 'Referer': url}
        html = self.net.http_GET(url, headers=headers).content

        data = helpers.get_hidden(html)
        html = self.net.http_POST(url, data, headers=headers).content
        stream_url = self.__get_best_source(html)
        if stream_url:
#           xbmc.sleep(2000)
            print stream_url

    def __get_best_source(self, html):
        r = re.search('"sources"\s*:\s*(\[.*?\])', html, re.DOTALL)
        if r:
            sources = json.loads(r.group(1))
            max_label = 0
            stream_url = ''
            for source in sources:
                if 'label' in source and int(re.sub('[^0-9]', '', source['label'])) > max_label:
                    stream_url = source['file']
                    max_label = int(re.sub('[^0-9]', '', source['label']))
            if stream_url:
                stream_url = '%s?%s&direct=false&ua=false' % (stream_url.split('?')[0], urlparse.urlparse(stream_url).query)
                return stream_url + '|' + urllib.urlencode({'User-Agent': common.IE_USER_AGENT})

    def get_url(self, host, media_id):
        return 'http://allmyvideos.net/%s' % media_id

    def get_url1st(self, host, media_id):
        return 'http://allmyvideos.net/embed-%s.html' % media_id

sys.stdout = AllmyvideosResolver()

