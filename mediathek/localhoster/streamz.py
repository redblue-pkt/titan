
import sys
import re
from lib import jsunpack
from lib.net import Net

class VidUpMeResolver(object):
    name = "streamz"
    domains = ['streamz.cc']
    pattern = r'(?://|\.)(streamz\.cc)/([0-9a-zA-Z]+)'

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
        html = self.net.http_GET(web_url).content

        js_data = re.findall('(eval\(function.*?)</script>', html.replace('\n', ''))
#        print "js_data1", js_data
        for i in js_data:
            html += jsunpack.unpack(i)
#        print "js_data2", js_data

#        print "html", html
#var video=videojs("video_1");video.src({type:\'video/mp4\',src:\'https://streamz.cc/getlink-197df2a579744943a23671891d481fa3.dll\'});var 

#        var = re.search('video.src.*\[([^;]+)\];', html)

        match = re.findall('''["']?src:['"]?\s*:\s*\[(.*?)\]''', html)
#        print "match", match
        match = re.findall("var video(.+?)'", html)
#        print "match", match
        match = re.findall("var video(.+?)\}\);var", html)
#        print "match", match
        match = re.findall(",src:\\\\'(.+?)\\\\'", html)
        print match[0]
#        if match:
#            stream_url = re.findall('''['"]?file['"]?\s*:\s*['"]?([^'"]+)''', match[0])
#            if stream_url:
#                 print stream_url[-1]

    def get_url(self, host, media_id):
        return 'http://%s/%s' % (host,media_id)


sys.stdout = VidUpMeResolver()
