
import re
import sys
from lib.net import Net

class StreamcloudResolver(object):
    name = "streamcloud"
    domains = ["streamcloud.eu"]
    pattern = '(?://|\.)(streamcloud\.eu)/([0-9a-zA-Z]+)'

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
        resp = self.net.http_GET(web_url)
        html = resp.content
        post_url = resp.get_url()
        if re.search('>(File Not Found)<', html):
            print 'File Not Found or removed'

        form_values = {}
        for i in re.finditer('<input.*?name="(.*?)".*?value="(.*?)">', html):
            form_values[i.group(1)] = i.group(2).replace("download1", "download2")
        html = self.net.http_POST(post_url, form_data=form_values).content

        r = re.search('file: "(.+?)",', html)
        if r:
            print r.group(1)

    def get_url(self, host, media_id):
        return 'http://streamcloud.eu/%s' % (media_id)

sys.stdout = StreamcloudResolver()
