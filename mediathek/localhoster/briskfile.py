
import re
import json
import urllib2
from lib import helpers
import sys
from lib.net import Net
import lib.common as common

class BriskfileResolver(object):
    name = "briskfile"
    domains = ["briskfile.com"]
    pattern = '(?://|\.)(briskfile\.com)/(?:l|e)/([0-9A-Za-z\-]+)'

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
        match = re.search('''val\(\)\s*\+\s*['"]([^"']+)''', html)
        suffix = match.group(1) if match else ''
        data = helpers.get_hidden(html)
        for name in data:
            data[name] = data[name] + suffix
        
        headers['Referer'] = web_url
        html = self.net.http_POST(web_url, form_data=data, headers=headers).content
        html = re.compile(r'clip\s*:\s*\{.*?(?:url|src)\s*:\s*[\"\'](.+?)[\"\']', re.DOTALL).search(html)
        if not html:
            print 'File Not Found or removed'
        stream_url = html.group(1)
        req = urllib2.Request(stream_url)
        for key in headers:
            req.add_header(key, headers[key])
        stream_url = urllib2.urlopen(req).geturl()
        print stream_url + '|User-Agent=%s&Referer=%s' % (common.FF_USER_AGENT, web_url)

    def get_url(self, host, media_id):
        return 'http://www.briskfile.com/l/%s' % (media_id)

sys.stdout = BriskfileResolver()