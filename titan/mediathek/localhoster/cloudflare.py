
import urllib2
from lib import helpers
import sys
from lib.net import Net
import lib.common as common
import re
import cookielib
from lib.cCFScrape import cCFScrape
#from lib.cloudflare import cloudflare
import lib.cloudflare as cloudflare
#from lib.cCFScrape import cCFScrape

class CloudflareResolver(object):
    def __init__(self):
        self.net = Net(cookie_file='/mnt/network/cookies', http_debug=True, cloudflare=True)
#        self.net = Net(cookie_file='/mnt/network/cookies', cloudflare=True)

        url = str(sys.argv[1])
        return self.get_answer_code(url)

    def get_answer_code(self, web_url):
#        headers = {'User-Agent': common.FF_USER_AGENT}                 
#        headers = {'User-Agent': common.RAND_UA}

        Domain = re.sub(r'https*:\/\/([^/]+)(\/*.*)', '\\1', web_url)
        headers = {'User-agent': common.FF_USER_AGENT, 'Referer': web_url, 'Host': Domain,
                   'Accept': 'text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8',
                   'Content-Type': 'text/html; charset=utf-8'}

        CF = cloudflare.CloudflareBypass()
        html = CF.GetHtml(web_url)
        ret = self.net.save_cookies('/mnt/network/cookies')      

        print CF.GetReponseInfo()

        try:
            print "html", html.encode('utf8')
        except:
            print html

sys.stdout = CloudflareResolver()

