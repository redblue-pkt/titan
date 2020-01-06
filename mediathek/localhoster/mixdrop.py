
import re
import sys
import time
from lib.net import Net
import re, urllib, json
from lib import helpers
import lib.common as common
from lib import jsunpack

class MixdropResolver(object):
    name = "mixdrop"
    domains = ["mixdrop.co", "mixdrop.to"]
    pattern = r'(?://|\.)(mixdrop\.[ct]o)/(?:f|e)/(\w+)'

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
        headers = {'User-Agent': common.FF_USER_AGENT}
        response = self.net.http_GET(web_url, headers=headers)

        html = self.net.http_GET(web_url, headers=headers).content
#        print "html", html
        js_data = re.findall('(eval\(function.*?)</script>', html.replace('\n', ''))
#        print "js_data1", js_data
        for i in js_data:
            html += jsunpack.unpack(i)
#        print "html", html

        #MDCore.poster="//s-delivery10.mxdcontent.net/thumbs/18a6675ac1932f50e07887c3daf25a8b.jpg";MDCore.furl="//s-delivery10.mxdcontent.net/v/18a6675ac1932f50e07887c3daf25a8b.mp4?s=1W53dLLMYoBJ_x-Nv73_FA&e=1578351242";MDCore.vfile="18a6675ac1932f50e07887c3daf25a8b.mp4";MDCore.vserver="s-delivery10";MDCore.remotesub="";MDCore.chromeInject="0";MDCore.referrer="";$(document).ready(function(){setTimeout(function(){$("body").append("<iframe width=\\"1366\\" height=\\"768\\" src=\\"https://www.gearbest.com/smart-watch-phone/pp_009309925869.html?lkid=74203065\\" style=\\"display:none\\" sandbox=\\"allow-same-origin allow-scripts allow-popups allow-forms\\"></iframe>")},120000);setTimeout(function(){$("body").append("<iframe width=\\"1366\\" height=\\"768\\" src=\\"https://zap.buzz/6kr3yJp\\" style=\\"display:none\\" sandbox=\\"allow-same-origin allow-scripts allow-popups allow-forms\\"></iframe>")},120000);setTimeout(function(){$("body").append("<iframe width=\\"1366\\" height=\\"768\\" src=\\"https://uthorner.info/redirect?tid=814005&ref=mixdrop.co\\" style=\\"display:none\\" sandbox=\\"allow-same-origin allow-scripts allow-popups allow-forms\\"></iframe>")},120000);setTimeout(function(){$("body").append("<iframe width=\\"1366\\" height=\\"768\\" src=\\"https://pmzer.com/213479/9189\\" style=\\"display:none\\" sandbox=\\"allow-same-origin allow-scripts allow-popups allow-forms\\"></iframe>")},120000);setTimeout(function(){$("body").append("<iframe width=\\"1366\\" height=\\"768\\" src=\\"https://dynamicadx.com/213480/9190\\" style=\\"display:none\\" sandbox=\\"allow-same-origin allow-scripts allow-popups allow-forms\\"></iframe>")},120000);setTimeout(function(){$("body").append("<iframe width=\\"1366\\" height=\\"768\\" src=\\"https://zap.buzz/Y79rkqR\\" style=\\"display:none\\" sandbox=\\"allow-same-origin allow-scripts allow-popups allow-forms\\"></iframe>")},120000)});
        match = re.search('MDCore.furl="([^"].*)";MDCore.vfile=', html)
        if match:
             match = re.search('MDCore.furl="([^"].*)";MDCore.vfile=', html).group(1)
             headers = {'User-Agent': common.RAND_UA}
             if 'http:' not in match:
                  print "https:%s" % (match) + helpers.append_headers(headers)
             else:
                  print "%s" % (match) + helpers.append_headers(headers)
             return

        if '(p,a,c,k,e,d)' in html:
            html = helpers.get_packed_data(html)
        r = re.search(r'vsr[^=]+=\s*"([^"]+)', html)
        if r:
            headers = {'User-Agent': common.RAND_UA}
            print "https:" + r.group(1) + helpers.append_headers(headers)
	else:
            print 'errormsg=File Not Found or removed'


    def get_url(self, host, media_id):
        return 'https://%s/e/%s' % (host, media_id)

sys.stdout = MixdropResolver()
