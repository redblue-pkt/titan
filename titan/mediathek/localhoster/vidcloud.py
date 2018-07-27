"""
    urlresolver XBMC Addon
    Copyright (C) 2016 jsergio

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
import lib.common as common
import re
import sys
from lib.net import Net
from lib import helpers

class VidCloudResolver(object):
    name = 'vidcloud'
    domains = ['vidcloud.co', 'loadvid.online']
    pattern = '(?://|\.)((?:vidcloud.co|loadvid.online))/(?:embed|v)/([0-9a-zA-Z]+)'

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
        headers = {'User-Agent': common.CHROME_USER_AGENT, 'Referer': 'https://vidcloud.co/embed/%s' % media_id}
        html = self.net.http_GET(web_url, headers=headers).content

        if html:
            sources = helpers.scrape_sources(html.replace("\\n", "").replace("\\", ""), patterns=[
                '''src":\s*"(?P<url>[^"]+)(?:[^}>\]]+)label":\s*"(?P<label>[^"]+)'''], generic_patterns=False)
            if sources:
                print helpers.pick_source(sources) + helpers.append_headers(headers)

        raise ResolverError("Unable to locate video")

    def get_url(self, host, media_id):
        return 'https://vidcloud.co/player?fid=%s&page=embed' % media_id

sys.stdout = VidCloudResolver()
