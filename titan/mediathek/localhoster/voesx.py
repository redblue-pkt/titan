"""
    Plugin for URLResolver
    Copyright (C) 2020  gujal

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
import base64
#from urlresolver import common
#from urlresolver.plugins.lib import helpers
#from urlresolver.resolver import UrlResolver

import sys
import time
from lib.net import Net
import re, urllib, json
from lib import helpers
import lib.common as common

class VoeResolver(object):
    name = "voe"
    domains = ["voe.sx"]
    pattern = r'(?://|\.)(voe\.sx)/(?:e/)?([0-9A-Za-z]+)'

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
        html = self.net.http_GET(web_url, headers=headers).content
        r = re.search(r'uttf0\((\[[^)]+)', html)
        if r:
            r = eval(r.group(1))
            r = base64.b64decode(''.join(r)[::-1].encode('utf8')).decode('utf8')
            print r + helpers.append_headers(headers)

        print helpers.get_media_url(web_url,
                                     patterns=[r'''hls":\s*"(?P<url>[^"]+)",\s*"video_height":\s*(?P<label>[^,]+)'''],
                                     generic_patterns=False)

    def get_url(self, host, media_id):
        return 'https://%s/e/%s' % (host, media_id)
                
sys.stdout = VoeResolver()
