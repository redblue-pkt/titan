"""
    urlresolver XBMC Addon
    Copyright (C) 2011 t0mm0

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
"""
import os
from net import Net, get_ua  # @UnusedImport
#from lib import log_utils  # @UnusedImport
#from lib.net import Net  # @UnusedImport
#from lib import cache  # @UnusedImport
#from lib import kodi

#addon_path = kodi.get_path()
#plugins_path = os.path.join(addon_path, 'lib', 'urlresolver', 'plugins')
#profile_path = kodi.translate_path(kodi.get_profile())
#settings_file = os.path.join(addon_path, 'resources', 'settings.xml')
#addon_version = kodi.get_version()
addon_version = '1.0'
#get_setting = kodi.get_setting
#set_setting = kodi.set_setting
#open_settings = kodi.open_settings
#has_addon = kodi.has_addon

profilePath = "/mnt/network"

RAND_UA = get_ua()
IE_USER_AGENT = 'Mozilla/5.0 (Windows NT 6.1; WOW64; Trident/7.0; AS; rv:11.0) like Gecko'
#FF_USER_AGENT = 'Mozilla/5.0 (Windows NT 6.3; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Maxthon/4.4.7.3000 Chrome/30.0.1599.101 Safari/537.36'
FF_USER_AGENT = 'Mozilla%2F5.0+%28Windows+NT+6.3%3B+rv%3A36.0%29+Gecko%2F20100101+Firefox%2F36.0'
OPERA_USER_AGENT = 'Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/47.0.2526.111 Safari/537.36 OPR/34.0.2036.50'
IOS_USER_AGENT = 'Mozilla/5.0 (iPhone; CPU iPhone OS 6_0 like Mac OS X) AppleWebKit/536.26 (KHTML, like Gecko) Version/6.0 Mobile/10A5376e Safari/8536.25'
ANDROID_USER_AGENT = 'Mozilla/5.0 (Linux; Android 4.4.2; Nexus 4 Build/KOT49H) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/34.0.1847.114 Mobile Safari/537.36'
EDGE_USER_AGENT = 'Mozilla/5.0 (Windows NT 10.0; Win64; x64; ServiceUI 13) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/64.0.3282.140 Safari/537.36 Edge/17.17134'
CHROME_USER_AGENT = 'Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/66.0.3359.181 Safari/537.36'
SMU_USER_AGENT = 'URLResolver for Kodi/%s' % (addon_version)
