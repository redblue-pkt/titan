"""
    Plugin for UrlResolver
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

#import re
#from urlresolver.plugins.lib import helpers
#from urlresolver import common
#from urlresolver.resolver import UrlResolver, ResolverError

import re
import sys
import time
from lib.net import Net
import re, urllib, json
from lib import helpers
import lib.common as common

class EvoLoadResolver(object):
    name = "evoload"
    domains = ["evoload.io"]
    pattern = r'(?://|\.)(evoload\.io)/(?:e|f|v)/([0-9a-zA-Z]+)'

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
        surl = 'https://evoload.io/SecurePlayer'
        web_url = self.get_url(host, media_id)
        rurl = 'https://{0}/'.format(host)
        headers = {'User-Agent': common.FF_USER_AGENT,
                   'Referer': rurl}
        html = self.net.http_GET(web_url, headers).content
        passe = re.search('<div id="captcha_pass" value="(.+?)"></div>', html).group(1)
        headers.update({'Origin': rurl[:-1]})
        crsv = self.net.http_GET('https://csrv.evosrv.com/captcha?m412548', headers).content
        post = {"code": media_id, "csrv_token": crsv, "pass": passe, "token": "ok"}
        shtml = self.net.http_POST(surl, form_data=post, headers=headers, jdata=True).content
        r = json.loads(shtml).get('stream')
        if r:
            surl = r.get('backup') if r.get('backup') else r.get('src')
            if surl:
                print surl + helpers.append_headers(headers)
            else:
                print 'errormsg=File1 Not Found or removed'

        else:
            print 'errormsg=File Not Found or removed'

    def get_url(self, host, media_id):
        return 'https://%s/e/%s.html' % (host, media_id)

sys.stdout = EvoLoadResolver()
