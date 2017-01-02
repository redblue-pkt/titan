
import os
import hashlib
import sys
from lib.net import Net
import re
import lib.ol_gmu as ol_gmu
import lib.common as common

#OL_SOURCE = 'https://offshoregit.com/tvaresolvers/ol_gmu.py'
OL_PATH = ''

API_BASE_URL = 'https://api.openload.co/1'
INFO_URL = API_BASE_URL + '/streaming/info'
GET_URL = API_BASE_URL + '/streaming/get?file={media_id}'
#OL_PATH = os.path.join(common.plugins_path, 'ol_gmu.py')


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

    def get_host_and_id(self, url):
        r = re.search(self.pattern, url, re.I)
        if r:
            return r.groups()
        else:
            return False

    def i18n(string_id):
        try:
            return addon.getLocalizedString(strings.STRINGS[string_id]).encode('utf-8', 'ignore')
        except Exception as e:
#            log_utils.log('Failed String Lookup: %s (%s)' % (string_id, e))
            return string_id

    def get_ol_code(self):
        try:
            new_py = self.net.http_GET(OL_SOURCE).content
#            common.log_utils.log('new_py: %s' % (new_pya))

            if new_py:
                with open(OL_PATH, 'w') as f:
                    f.write(new_py)
        except Exception as e:
             print 'Exception during openload code retrieve:'
#            common.log_utils.log_warning('Exception during openload code retrieve: %s' % e)
            
    def get_media_url(self, host, media_id):
        video_url = ""
        try:
            self._auto_update(self.get_setting('url'), OL_PATH, self.get_setting('key'))
            reload(ol_gmu)
            return ol_gmu.get_media_url(self.get_url(host, media_id))  # @UndefinedVariable
        except Exception as e:
#            common.log_utils.log_debug('Exception during openload resolve parse: %s' % (e))
            try:
                video_url = self.__check_auth(media_id)
                if not video_url:
                    video_url = self.__auth_ip(media_id)
            #except ResolverError:
            except Exception as e:
                print "raise"
               # raise
            
            if video_url:
                print video_url

    def get_url(self, host, media_id):
        return 'http://openload.co/embed/%s' % (media_id)

    def __auth_ip(self, media_id):
        js_data = self.__get_json(INFO_URL)
        pair_url = js_data.get('result', {}).get('auth_url', '')
        if pair_url:
            pair_url = pair_url.replace('\/', '/')
            header = i18n('ol_auth_header')
            line1 = i18n('auth_required')
            line2 = i18n('visit_link')
            line3 = i18n('click_pair') % (pair_url)
            with common.kodi.CountdownDialog(header, line1, line2, line3) as cd:
                return cd.start(self.__check_auth, [media_id])
        
    def __check_auth(self, media_id):
        try:
            js_data = self.__get_json(GET_URL.format(media_id=media_id))
#        except ResolverError as e:
        except Exception as e:
            status, msg = e
            if status == 403:
                return
#            else:
#                raise ResolverError(msg)
        
        return js_data.get('result', {}).get('url')
    
    def __get_json(self, url):
        result = self.net.http_GET(url).content
        js_result = json.loads(result)
#        common.log_utils.log_debug(js_result)
#        if js_result['status'] != 200:
#            raise ResolverError(js_result['status'], js_result['msg'])
        return js_result


sys.stdout = OpenLoadResolver()
