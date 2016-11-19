
import re, time
from lib import jsunpack
import sys
from lib.net import Net
import lib.common as common
import lib.fx_gmu as fx_gmu
import lib.fx_gmu as fx_gmu

FX_SOURCE = 'https://offshoregit.com/tvaresolvers/fx_gmu.py'
FX_PATH = ''

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

    def get_fx_code(self):
        try:
            headers = self.net.http_HEAD(FX_SOURCE).get_headers(as_dict=True)
            old_etag = self.get_setting('etag')
            new_etag = headers.get('Etag', '')
            old_len = self.__old_length()
            new_len = int(headers.get('Content-Length', 0))
            if old_etag != new_etag or old_len != new_len:
                self.set_setting('etag', new_etag)
                new_py = self.net.http_GET(FX_SOURCE).content
                if new_py:
                    with open(FX_PATH, 'w') as f:
                        f.write(new_py)
            else:
                print 'Reusing existing fx_gmu.py: |%s|%s|%s|%s|' % (old_etag, new_etag, old_len, new_len)
        except Exception as e:
            print 'Exception during flashx code retrieve: %s' % e
            
    def __old_length(self):
        try:
            with open(FX_PATH, 'r') as f:
                old_py = f.read()
            old_len = len(old_py)
        except:
            old_len = 0
        return old_len

    def get_media_url(self, host, media_id):
#        try:
#            if self.get_setting('auto_update') == 'true':
#                self.get_fx_code()
#            with open(FX_PATH, 'r') as f:
#                py_data = f.read()
#            import fx_gmu
            web_url = self.get_url(host, media_id)
            print fx_gmu.get_media_url(web_url)
#        except Exception as e:
#            print 'error'
        
    def get_url(self, host, media_id):
        return 'http://%s/embed.php?c=%s' % (host, media_id)

sys.stdout = FlashxResolver()
