"""
    Plugin for UrlResolver
    Copyright (C) 2020 gujal, Anis3

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

#from urlresolver.plugins.__generic_resolver__ import GenericResolver
#from urlresolver.plugins.lib import helpers
import re
import sys
import time
from lib.net import Net
import re, urllib, json
from lib import helpers
import lib.common as common

class UpStreamResolver(object):
    name = "upstream.to"
    domains = ['upstream.to']
    pattern = r'(?://|\.)(upstream\.to)/(?:embed-)?([0-9a-zA-Z]+)'

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
        print helpers.get_media_url(self.get_url(host, media_id),
                                    patterns=[r'''sources:\s*\[(?:{file:)?\s*"(?P<url>[^"]+)'''],
                                    generic_patterns=False, referer=False)

    def get_url(self, host, media_id):
        return 'https://%s/embed-%s.html' % (host, media_id)

#    def get_url(self, host, media_id):
#        return self._default_get_url(host, media_id, template='https://{host}/embed-{media_id}.html')

sys.stdout = UpStreamResolver()
