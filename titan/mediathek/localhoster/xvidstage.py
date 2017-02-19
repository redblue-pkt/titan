from lib import helpers
import sys
import re
from lib.net import Net
import lib.common as common

class XvidstageResolver(object):
    name = "xvidstage"
    domains = ["xvidstage.com"]
    pattern = '(?://|\.)(xvidstage\.com)/(?:embed-|)?([0-9A-Za-z]+)'

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
        data = helpers.get_hidden(html)
        headers['Cookie'] = response.get_headers(as_dict=True).get('Set-Cookie', '')
        html = self.net.http_POST(web_url, headers=headers, form_data=data).content
        sources = helpers.scrape_sources(html, result_blacklist='tmp')
        print helpers.pick_source(sources) + helpers.append_headers(headers)

    def get_url(self, host, media_id):
        return 'http://www.xvidstage.com/%s' % media_id

sys.stdout = XvidstageResolver()