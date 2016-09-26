
import re
import sys
from lib.net import Net

class VodlockerResolver(object):
    name = "vodlocker.com"
    domains = ["vodlocker.com"]
    pattern = '(?://|\.)(vodlocker\.com)/(?:embed-)?([0-9a-zA-Z]+)'

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
        link = self.net.http_GET(web_url).content
        if 'FILE WAS DELETED' in link:
            print 'File deleted.'
        else:
            video_link = str(re.compile('file[: ]*"(.+?)"').findall(link)[0])

            if len(video_link) > 0:
                print video_link
            else:
                print 'No playable video found.'

    def get_url(self, host, media_id):
        return 'http://vodlocker.com/embed-%s-640x400.html' % media_id

sys.stdout = VodlockerResolver()
