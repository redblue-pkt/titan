"""
Plugin for ResolveUrl
Copyright (C) 2020 gujal

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program. If not, see <http://www.gnu.org/licenses/>.
"""

#import re
#from resolveurl.plugins.lib import helpers
#from resolveurl import common
#from resolveurl.resolver import ResolveUrl, ResolverError

import re
import sys
import time
from lib.net import Net
import re, urllib, json
from lib import helpers
import lib.common as common

class StreamTapeResolver(object):
    name = "streamtape"
    domains = ['streamtape.com', 'strtape.cloud', 'streamtape.net', 'streamta.pe', 'streamtape.site', 'strcloud.link']
    pattern = r'(?://|\.)(str(?:eam)?(?:tap?e?|cloud)\.(?:com|cloud|net|pe|site|link))/(?:e|v)/([0-9a-zA-Z]+)'

#    def __init__(self):
#        self.net = common.Net()

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
        headers = {'User-Agent': common.FF_USER_AGENT,
                   'Referer': 'https://{0}/'.format(host)}
        try:
            r = self.net.http_GET(web_url, headers=headers).content
        except urllib_error.HTTPError:
#            raise ResolverError('Video deleted or removed.')
            print 'errormsg=Video deleted or removed.'
            return
        src = re.findall(r'''ById\('.+?=\s*(["']//[^;<]+)''', r)
        if src:
            src_url = ''
            parts = src[-1].replace("'", '"').split('+')
            for part in parts:
                p1 = re.findall(r'"([^"]*)', part)[0]
                p2 = 0
                if 'substring' in part:
                    subs = re.findall(r'substring\((\d+)', part)
                    for sub in subs:
                        p2 += int(sub)
                src_url += p1[p2:]
            src_url += '&stream=1'
            src_url = 'https:' + src_url if src_url.startswith('//') else src_url
            print helpers.get_redirect_url(src_url, headers) + helpers.append_headers(headers)
#        raise ResolverError('Video cannot be located.')
        else
            print 'errormsg=Video cannot be located.'

    def get_url(self, host, media_id):
        return 'https://%s/e/%s' % (host, media_id)
                
sys.stdout = StreamTapeResolver()
