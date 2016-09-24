
import re, time
from lib import jsunpack
import sys
from lib.net import Net

class FlashxResolver(object):
    name = "flashx"
    domains = ["flashx.tv"]
    pattern = '(?://|\.)(flashx\.tv)/(?:embed-|dl\?|embed.php\?c=)?([0-9a-zA-Z/-]+)'

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
        cfdcookie = resp._response.info()['set-cookie']
        cfduid = re.search('cfduid=(.*?);', cfdcookie).group(1)
        file_id = re.search("'file_id', '(.*?)'", html).group(1)
        aff = re.search("'aff', '(.*?)'", html).group(1)
        headers = { 'Referer': web_url,
                    'Cookie': '__cfduid=' + cfduid + '; lang=1'}
        surl = re.search('src="(.*?' + file_id + ')',html).group(1)
        dummy = self.net.http_GET(url=surl, headers=headers).content
        headers = { 'Referer': web_url,
                    'Cookie': '__cfduid=' + cfduid + '; lang=1; file_id=' + file_id + '; aff=' + aff }
        html = self.net.http_GET(url=web_url, headers=headers).content
        fname = re.search('name="fname" value="(.*?)"', html).group(1)
        hash = re.search('name="hash" value="(.*?)"', html).group(1)
        fdata = { 'op': 'download1',
                  'usr_login': '',
                  'id': media_id,
                  'fname': fname,
                  'referer': '',
                  'hash': hash,
                  'imhuman': 'Proceed to video' }
        furl = 'http://www.flashx.tv/dl?' + media_id
        time.sleep(5)
        html = self.net.http_POST(url=furl, form_data=fdata, headers=headers).content
        strhtml = jsunpack.unpack(re.search('(eval\(function.*?)</script>', html, re.DOTALL).group(1))
        stream = re.search('file:"([^"]*)",label', strhtml).group(1)

        if stream:
            print stream

    def get_url(self, host, media_id):
        return 'http://www.flashx.tv/%s.html' % media_id
 
sys.stdout = FlashxResolver()
