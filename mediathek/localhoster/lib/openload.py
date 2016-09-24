
import os
import hashlib
import sys
from lib.net import Net
import ol_gmu

OL_SOURCE = 'https://offshoregit.com/tvaresolvers/ol_gmu.py'
OL_PATH = ''#os.path.join(common.plugins_path, 'ol_gmu.py')

class OpenLoadResolver(object):
    name = "openload"
    domains = ["openload.io", "openload.co"]
    pattern = '(?://|\.)(openload\.(?:io|co))/(?:embed|f)/([0-9a-zA-Z-_]+)'

    def __init__(self):
        self.net = Net()
        url = str(sys.argv[1])
        host = self.get_host_and_id(url)[0]
        media_id = self.get_host_and_id(url)[1]

        return self.get_media_url(host, media_id)

    @common.cache.cache_method(cache_limit=8)
    def get_ol_code(self):
        try:
            new_py = self.net.http_GET(OL_SOURCE).content
#            common.log_utils.log('new_py: %s' % (new_pya))

            if new_py:
                with open(OL_PATH, 'w') as f:
                    f.write(new_py)
        except Exception as e:
            common.log_utils.log_warning('Exception during openload code retrieve: %s' % e)
            
    def get_media_url(self, host, media_id):
#        common.log_utils.log('host: %s' % (host))
#        common.log_utils.log('media_id: %s' % (media_id))
#        common.log_utils.log('OL_PATH: %s' % (OL_PATH))
        try:
#            if self.get_setting('auto_update') == 'true':
#                self.get_ol_code()
#            with open(OL_PATH, 'r') as f:
#                py_data = f.read()
#            common.log_utils.log('py_data: %s' % (py_data))
#            common.log_utils.log('ol_gmu hash: %s' % (hashlib.md5(py_data).hexdigest()))
#            import ol_gmu
            web_url = self.get_url(host, media_id)
#            common.log_utils.log('web_url: %s' % (web_url))
            return ol_gmu.get_media_url(web_url)
        except Exception as e:
#            common.log_utils.log_debug('Exception during openload resolve parse: %s' % e)
            raise

    def get_url(self, host, media_id):
        return 'http://openload.io/embed/%s' % media_id

    def get_settings_xml(cls):
        xml = super(cls, cls).get_settings_xml()
        xml.append('<setting id="%s_auto_update" type="bool" label="Automatically update resolver" default="true"/>' % (cls.__name__))
        return xml
