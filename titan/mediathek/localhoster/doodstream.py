"""
    plugin for URLResolver
    Copyright (C) 2020 gujal

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
"""
import re
import random
import string
import time
#from urlresolver.plugins.lib import helpers
#from urlresolver import common
#from urlresolver.resolver import UrlResolver, ResolverError

import sys
from lib.net import Net
import re, urllib, json
from lib import helpers
import lib.common as common


class DoodStreamResolver(object):
    name = "doodstream"
    domains = ['dood.watch', 'doodstream.com', 'dood.to', 'dood.so', 'dood.cx', 'dood.la', 'dood.ws']
    pattern = r'(?://|\.)(dood(?:stream)?\.(?:com|watch|to|so|cx|la|ws))/(?:d|e)/([0-9a-zA-Z]+)'

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
        headers = {'User-Agent': common.RAND_UA,
                   'Referer': 'https://{0}/'.format(host)}

        r = self.net.http_GET(web_url, headers=headers)
        if r.get_url() != web_url:
            host = re.findall(r'(?://|\.)([^/]+)', r.get_url())[0]
            web_url = self.get_url(host, media_id)
        headers.update({'Referer': web_url})

        html = r.content
        match = re.search(r'''dsplayer\.hotkeys[^']+'([^']+).+?function\s*makePlay.+?return[^?]+([^"]+)''', html, re.DOTALL)
        if match:
            token = match.group(2)
            url = 'https://{0}{1}'.format(host, match.group(1))
            html = self.net.http_GET(url, headers=headers).content
            print self.dood_decode(html) + token + str(int(time.time() * 1000)) + helpers.append_headers(headers)

        #raise ResolverError('Video Link Not Found')
        else:
            print 'errormsg=Video Link Not Found'

    def dood_decode(self, data):
        t = string.ascii_letters + string.digits
        return data + ''.join([random.choice(t) for _ in range(10)])

    def get_url(self, host, media_id):
        return 'https://%s/e/%s' % (host, media_id)

sys.stdout = DoodStreamResolver()
