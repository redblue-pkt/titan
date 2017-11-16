
import re
import sys
from lib.net import Net

import re, urllib, json
from lib import helpers
import lib.common as common

class WaawResolver(object):
    name = "waaw"
    domains = ["waaw.tv", "hqq.watch", "netu.tv", "hqq.tv"]
    pattern = "(?://|\.)((?:waaw|netu|hqq)\.(?:tv|watch))/(?:watch_video\.php\?v|.+?vid)=([a-zA-Z0-9]+)"

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
                print 'No playable video found.'

    def get_media_url(self, host, media_id):
        web_url = self.get_url(host, media_id)
        headers = {'User-Agent': common.FF_USER_AGENT, 'Referer': 'https://waaw.tv/watch_video.php?v=%s&post=1' % media_id}
        html = self.net.http_GET(web_url, headers=headers).content
        ret = self.net.save_cookies('/mnt/network/cookies')      

        if html:
            try:
                wise = re.search('(eval\s*\(function.*?)</script>', html, re.DOTALL | re.I).groups()[0]
                data_unwise = self.jswise(wise).replace("\\", "")
                try: at = re.search('var at\s*=\s*"([^"]+)"', data_unwise, re.I).groups()[0]
                except: at = ""
                try: http_referer = re.search('var http_referer\s*=\s*"([^"]+)"', data_unwise, re.I).groups()[0]
                except: http_referer = ""

                player_url = "http://hqq.watch/sec/player/embed_player.php?iss=&vid=%s&at=%s&autoplayed=yes&referer=on&http_referer=%s&pass=&embed_from=&need_captcha=0&hash_from=" % (media_id, at, http_referer)
                headers.update({'Referer': web_url})
                data_player = self.net.http_GET(player_url, headers=headers).content
                data_unescape = re.findall('document.write\(unescape\("([^"]+)"', data_player)

                data = ""
                for d in data_unescape:
                    data += urllib.unquote(d)

                    
                data_unwise_player = ""
                wise = ""
                wise = re.search('(eval\s*\(function.*?)</script>', data_player, re.DOTALL | re.I)
                if wise:
                    data_unwise_player = self.jswise(wise.group(1)).replace("\\", "")
             
                try: vars_data = re.search('/player/get_md5.php",\s*\{(.*?)\}', data, re.DOTALL | re.I).groups()[0]
                except: vars_data = ""
                matches = re.findall('\s*([^:]+):\s*([^,]*)[,"]', vars_data)
                params = {}
                for key, value in matches:
                    if key == "adb":
                        params[key] = "0/"
                    elif '"' in value:
                        params[key] = value.replace('"', '')
                    else:
                        try: value_var = re.search('var\s*%s\s*=\s*"([^"]+)"' % value, data, re.I).groups()[0]
                        except: value_var = ""
                        if not value_var and data_unwise_player:
                            try: value_var = re.search('var\s*%s\s*=\s*"([^"]+)"' % value, data_unwise_player, re.I).groups()[0]
                            except: value_var = ""
                        params[key] = value_var
             
                params = urllib.urlencode(params)
                headers.update({'X-Requested-With': 'XMLHttpRequest', 'Referer': player_url})
                data= ""
                data = self.net.http_GET("http://hqq.watch/player/get_md5.php?" + params, headers=headers).content
                url_data = json.loads(data)
                media_url = self.tb(url_data["html5_file"].replace("#", ""))
                del headers["Referer"]
#                del headers["X-Requested-With"]
#                if media_url: return media_url + helpers.append_headers(headers)

                if media_url: print media_url + helpers.append_headers(headers)
#                headers = {'User-Agent': common.FF_USER_AGENT}
#                if media_url: print media_url + helpers.append_headers(headers)


                
            except Exception as e:
#                raise ResolverError(e)
                 print "errormsg=Video not found 2"        
#        raise ResolverError('Video not found')
#        print "errormsg=Video not found"

    def tb(self, b_m3u8_2):
        j = 0
        s2 = ""
        while j < len(b_m3u8_2):
            s2 += "\\u0" + b_m3u8_2[j:(j + 3)]
            j += 3

        return s2.decode('unicode-escape').encode('ASCII', 'ignore')

    ## loop2unobfuscated
    def jswise(self, wise):
        while True:
            wise = re.search("var\s.+?\('([^']+)','([^']+)','([^']+)','([^']+)'\)", wise, re.DOTALL)
            if not wise: break
            ret = wise = self.js_wise(wise.groups())
            
        return ret
        
    ## js2python
    def js_wise(self, wise):
        w, i, s, e = wise

        v0 = 0;
        v1 = 0;
        v2 = 0
        v3 = [];
        v4 = []

        while True:
            if v0 < 5:
                v4.append(w[v0])
            elif v0 < len(w):
                v3.append(w[v0])
            v0 += 1
            if v1 < 5:
                v4.append(i[v1])
            elif v1 < len(i):
                v3.append(i[v1])
            v1 += 1
            if v2 < 5:
                v4.append(s[v2])
            elif v2 < len(s):
                v3.append(s[v2])
            v2 += 1
            if len(w) + len(i) + len(s) + len(e) == len(v3) + len(v4) + len(e): break

        v5 = "".join(v3);
        v6 = "".join(v4)
        v1 = 0
        v7 = []

        for v0 in range(0, len(v3), 2):
            v8 = -1
            if ord(v6[v1]) % 2: v8 = 1
            v7.append(chr(int(v5[v0:v0 + 2], 36) - v8))
            v1 += 1
            if v1 >= len(v4): v1 = 0
            
        return "".join(v7)

    def get_url2(self, host, media_id):
        return 'http://vodlocker.com/embed-%s-640x400.html' % media_id

    def get_url(self, host, media_id):
#        return self._default_get_url(host, media_id, template='http://hqq.watch/player/embed_player.php?vid={media_id}&autoplay=no')
        return 'http://hqq.watch/player/embed_player.php?vid=%s&autoplay=no' % media_id

sys.stdout = WaawResolver()
