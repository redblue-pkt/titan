
import sys
from lib.net import Net

import re, urllib, json
from lib import helpers
import lib.common as common

class StreamangoResolver(object):
    name = "streamango"
    domains = ['streamango.com', "streamcherry.com"]
    pattern = '(?://|\.)(stream(?:ango|cherry)\.com)/(?:v/d|f|embed)/([0-9a-zA-Z]+)'

    def __init__(self):
#        self.net = Net()
#        self.net = Net(cookie_file='/mnt/network/cookies', http_debug = True)
        self.net = Net(cookie_file='/mnt/network/cookies', http_debug = False)
 
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

    def get_media_url1(self, host, media_id):
        web_url = self.get_url(host, media_id)
        link = self.net.http_GET(web_url).content
        if 'FILE WAS DELETED' in link:
            print 'File deleted.'
        else:
            video_link = str(re.compile('file[: ]*"(.+?)"').findall(link)[0])

            if len(video_link) > 0:
                print video_link
            else:
                print 'errormsg=No playable video found.'

    def get_media_url(self, host, media_id):
        web_url = self.get_url(host, media_id)
        headers = {'User-Agent': common.RAND_UA}
#        headers = {'User-Agent': common.FF_USER_AGENT}
        html = self.net.http_GET(web_url, headers=headers).content

        if html:
#           srces.push( {type:"video/mp4",src:d('keDN2p3bx6LdzqrO1JcSxaDTkZoRyuDbzaLN0KzR2qHQza7NyJ/U4N8Lld4IltoMlN3Cn98Sl90JkN=SlpHQ/K3YnqnW3uENltwA',93),height:360,bitrate:576});
            encoded = re.search('''srces\.push\({type:"video/mp4",src:\w+\('([^']+)',(\d+)''', html)
            if not encoded:
                encoded = re.search('''srces\.push\( {type:"video/mp4",src:\w+\('([^']+)',(\d+)''', html)

            if encoded:
                source = self.decode(encoded.group(1), int(encoded.group(2)))
                if source:
                    source = "http:%s" % source if source.startswith("//") else source
                    source = source.split("/")
                    if not source[-1].isdigit():
                      source[-1] = re.sub('[^\d]', '', source[-1])
                    source = "/".join(source)

                    headers.update({'Referer': web_url})
                    print source + helpers.append_headers(headers)

#<h1 style="text-align: center !important;">Sorry!</h1>
#<p class="lead">We are unable to find the video you're looking for. There could be several reasons for this, for example it got removed by the owner.</p>
#</div>
#</div>

    def decode(self, encoded, code):
        _0x59b81a = ""
        k = 'ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/='
        k = k[::-1]

        count = 0

        for index in range(0, len(encoded) - 1):
            while count <= len(encoded) - 1:
                _0x4a2f3a = k.index(encoded[count])
                count += 1
                _0x29d5bf = k.index(encoded[count])
                count += 1
                _0x3b6833 = k.index(encoded[count])
                count += 1
                _0x426d70 = k.index(encoded[count])
                count += 1

                _0x2e4782 = ((_0x4a2f3a << 2) | (_0x29d5bf >> 4))
                _0x2c0540 = (((_0x29d5bf & 15) << 4) | (_0x3b6833 >> 2))
                _0x5a46ef = ((_0x3b6833 & 3) << 6) | _0x426d70
                _0x2e4782 = _0x2e4782 ^ code

                _0x59b81a = str(_0x59b81a) + chr(_0x2e4782)

                if _0x3b6833 != 64:
                    _0x59b81a = str(_0x59b81a) + chr(_0x2c0540)
                if _0x3b6833 != 64:
                    _0x59b81a = str(_0x59b81a) + chr(_0x5a46ef)

        return _0x59b81a

    def get_url(self, host, media_id):
        if host.lower() == 'streamango.com':
            host = 'fruitstreams.com'
        return 'http://%s/embed/%s' % (host, media_id)

sys.stdout = StreamangoResolver()
