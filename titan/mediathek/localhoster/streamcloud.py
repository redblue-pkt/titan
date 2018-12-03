
import re
import sys
import time
from lib.net import Net
import re, urllib, json
from lib import helpers
import lib.common as common

class StreamcloudResolver(object):
    name = "streamcloud"
    domains = ["streamcloud.eu"]
    pattern = '(?://|\.)(streamcloud\.eu)/([0-9a-zA-Z]+)'

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
        response = self.net.http_GET(web_url, headers=headers)
        html = response.content
        if re.search('>(File Not Found)<', html):
            print 'errormsg=File Not Found or removed'

        cnt = 10
        match = re.search('count\s*=\s*(\d+);', html)
        if match:
            cnt = int(match.group(1))
        cnt += 1

        data = helpers.get_hidden(html)
        headers.update({'Referer': web_url})
#        common.kodi.sleep(cnt * 1000)
        time.sleep(10)
        html = self.net.http_POST(response.get_url(), form_data=data, headers=headers).content
        sources = helpers.scrape_sources(html, patterns=['''file\s*:\s*["'](?P<url>[^"']+)'''])
#        return helpers.pick_source(sources) + helpers.append_headers(headers)
        print helpers.pick_source(sources) + helpers.append_headers(headers)
#        print "%s%s" % (helpers.pick_source(sources), helpers.append_headers(headers))


    def get_url(self, host, media_id):
        return 'http://streamcloud.eu/%s' % (media_id)

sys.stdout = StreamcloudResolver()
