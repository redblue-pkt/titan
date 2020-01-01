
import urllib2
from lib import helpers
import sys
from lib.net import Net
import lib.common as common
import re
import lib.cloudflare as cloudflare

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


#        print "cloudflare.CheckIfActive: ", web_url

        try:
            CF = cloudflare.CloudflareBypass()
            html = CF.GetHtml(web_url)
            print CF.GetReponseInfo()
            print "html", html
        except:
            html = self.net.http_GET(web_url, headers=headers).content
            #html = self.request(web_url, cookie_file=/mnt/network/cookies, cloudflare=True)
            ret = self.net.save_cookies('/mnt/network/cookies')      
            print "html", html.encode('utf8')

sys.stdout = CloudflareResolver()


