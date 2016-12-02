
import urllib2
from lib import helpers
import sys
from lib.net import Net
import lib.common as common

class GoldeselResolver(object):
    name = "briskfile"
    domains = ["goldesel.to"]
    pattern = '(?://|\.)(goldesel\.to)/(?:l|e)/([0-9A-Za-z\-]+)'

    def __init__(self):
#        self.net = Net(cookie_file='/mnt/network/cookies', http_debug=True, cloudflare=True)
        self.net = Net(cookie_file='/mnt/network/cookies', cloudflare=True)
        url = str(sys.argv[1])
        return self.get_answer_code(url)

    def get_answer_code(self, web_url):
        headers = {'User-Agent': common.FF_USER_AGENT}                 
        html = self.net.http_GET(web_url, headers=headers).content
        #html = self.request(web_url, cookie_file=/mnt/network/cookies, cloudflare=True)
        ret = self.net.save_cookies('/mnt/network/cookies')      
        print "html", html.encode('utf8')

sys.stdout = GoldeselResolver()