"""
    Plugin for URLResolver
    Copyright (C) 2021 gujal
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

#from six.moves import urllib_parse
#from urlresolver.plugins.lib import helpers
#from urlresolver import common
#from urlresolver.resolver import UrlResolver, ResolverError

import lib.six
from lib.six.moves import urllib_parse, urllib_request

import re
import sys
import time
from lib.net import Net
import re, urllib, json
from lib import helpers
import lib.common as common

class ABCVideoResolver(object):
    name = "abcvideo"
    domains = ['abcvideo.cc']
    pattern = r'(?://|\.)(abcvideo\.cc)/(?:embed-)?([0-9a-zA-Z]+)'

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
        surl = 'https://abcvideo.cc/dl'
        domain = 'aHR0cHM6Ly9hYmN2aWRlby5jYzo0NDM.'
        web_url = self.get_url(host, media_id)
        rurl = 'https://{0}/'.format(host)
        headers = {'User-Agent': common.FF_USER_AGENT,
                   'Referer': rurl}
        html = self.net.http_GET(web_url, headers).content
        token = helpers.girc(html, rurl, domain)
        if token:
            data = {'op': 'video_src',
                    'file_code': media_id,
                    'g-recaptcha-response': token}
            headers.update({'X-Requested-With': 'XMLHttpRequest'})
            shtml = self.net.http_GET('{0}?{1}'.format(surl, urllib_parse.urlencode(data)), headers=headers).content
            sources = helpers.scrape_sources(shtml)

            if sources:
                headers.pop('X-Requested-With')
                print helpers.pick_source(sources) + helpers.append_headers(headers)
            else:
                print 'errormsg=File Not Found or removed'
        else:
             print 'errormsg=Token Not Found or removed'
#        raise ResolverError('File Not Found or removed')

#    def get_url(self, host, media_id):
#        return self._default_get_url(host, media_id, template='https://{host}/embed-{media_id}.html')

    def get_url(self, host, media_id):
        return 'https://%s/embed-%s.html' % (host, media_id)

sys.stdout = ABCVideoResolver()
