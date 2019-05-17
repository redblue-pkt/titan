'''
    common XBMC Module
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
'''

from __future__ import division
import urllib, urllib2, re, sys
from time import sleep

import random
import cookielib
import gzip
import re
import StringIO
import urllib
import urllib2
import socket
from urlparse import urlparse
from urlparse import urlunparse
import time

BR_VERS = [
    ['%s.0' % i for i in xrange(18, 50)],
    ['37.0.2062.103', '37.0.2062.120', '37.0.2062.124', '38.0.2125.101', '38.0.2125.104', '38.0.2125.111', '39.0.2171.71', '39.0.2171.95', '39.0.2171.99', '40.0.2214.93', '40.0.2214.111',
     '40.0.2214.115', '42.0.2311.90', '42.0.2311.135', '42.0.2311.152', '43.0.2357.81', '43.0.2357.124', '44.0.2403.155', '44.0.2403.157', '45.0.2454.101', '45.0.2454.85', '46.0.2490.71',
     '46.0.2490.80', '46.0.2490.86', '47.0.2526.73', '47.0.2526.80', '48.0.2564.116', '49.0.2623.112', '50.0.2661.86'],
    ['11.0'],
    ['8.0', '9.0', '10.0', '10.6']]
WIN_VERS = ['Windows NT 10.0', 'Windows NT 7.0', 'Windows NT 6.3', 'Windows NT 6.2', 'Windows NT 6.1', 'Windows NT 6.0', 'Windows NT 5.1', 'Windows NT 5.0']
FEATURES = ['; WOW64', '; Win64; IA64', '; Win64; x64', '']
RAND_UAS = ['Mozilla/5.0 ({win_ver}{feature}; rv:{br_ver}) Gecko/20100101 Firefox/{br_ver}',
            'Mozilla/5.0 ({win_ver}{feature}) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/{br_ver} Safari/537.36',
            'Mozilla/5.0 ({win_ver}{feature}; Trident/7.0; rv:{br_ver}) like Gecko',
            'Mozilla/5.0 (compatible; MSIE {br_ver}; {win_ver}{feature}; Trident/6.0)']
def get_ua():
#    try: last_gen = int(kodi.get_setting('last_ua_create'))
    try: last_gen = 0
    except: last_gen = 0
#    if not kodi.get_setting('current_ua') or last_gen < (time.time() - (7 * 24 * 60 * 60)):
    index = random.randrange(len(RAND_UAS))
    versions = {'win_ver': random.choice(WIN_VERS), 'feature': random.choice(FEATURES), 'br_ver': random.choice(BR_VERS[index])}
    user_agent = RAND_UAS[index].format(**versions)
        # logger.log('Creating New User Agent: %s' % (user_agent), log_utils.LOGDEBUG)
#        kodi.set_setting('current_ua', user_agent)
#        kodi.set_setting('last_ua_create', str(int(time.time())))
#    else:
#        user_agent = kodi.get_setting('current_ua')
    return user_agent

class HeadRequest(urllib2.Request):
    '''A Request class that sends HEAD requests'''
    def get_method(self):
        return 'HEAD'

class Net:
    '''
    This class wraps :mod:`urllib2` and provides an easy way to make http
    requests while taking care of cookies, proxies, gzip compression and 
    character encoding.
    
    Example::
    
        from addon.common.net import Net
        net = Net()
        response = net.http_GET('http://xbmc.org')
        print response.content
    '''
    IE_USER_AGENT = 'Mozilla/5.0 (Windows NT 6.1; WOW64; Trident/7.0; AS; rv:11.0) like Gecko'
    FF_USER_AGENT = 'Mozilla/5.0 (Windows NT 6.3; rv:36.0) Gecko/20100101 Firefox/36.0'
    IOS_USER_AGENT = 'Mozilla/5.0 (iPhone; CPU iPhone OS 6_0 like Mac OS X) AppleWebKit/536.26 (KHTML, like Gecko) Version/6.0 Mobile/10A5376e Safari/8536.25'
    ANDROID_USER_AGENT = 'Mozilla/5.0 (Linux; Android 4.4.2; Nexus 4 Build/KOT49H) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/34.0.1847.114 Mobile Safari/537.36'

    _cj = cookielib.MozillaCookieJar()

    _proxy = None
    _user_agent = 'Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/28.0.1500.72 Safari/537.36'
    _accept = 'text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8'
    _http_debug = False
    _socket_timeout = 60

    def __init__(self, cookie_file='', proxy='', user_agent='', 
                 http_debug=False, accept=_accept, socket_timeout=_socket_timeout, cloudflare=False):
        '''
        Kwargs:
            cookie_file (str): Full path to a file to be used to load and save
            cookies to.
            
            proxy (str): Proxy setting (eg. 
            ``'http://user:pass@example.com:1234'``)
            
            user_agent (str): String to use as the User Agent header. If not 
            supplied the class will use a default user agent (chrome)
            
            http_debug (bool): Set ``True`` to have HTTP header info written to
            the XBMC log for all requests.
            
            accept (str) : String to use as HTTP Request Accept header.
            
            socket_timeout (int): time in seconds for socket connections to wait until time out

            cloudflare (bool): Set ``True`` to check all requests that raise HTTPError 503 for Cloudflare challenge and solve
            This can be changed per request as well, see http_GET, http_PUSH
        '''
    
        #Set socket timeout - Useful for slow connections
        socket.setdefaulttimeout(socket_timeout)

        # empty jar for each instance rather than scope of the import
        self._cloudflare_jar = cookielib.MozillaCookieJar()

        self.cloudflare = cloudflare
        if cookie_file:
            self.set_cookies(cookie_file)
        if proxy:
            self.set_proxy(proxy)
        if user_agent:
            self.set_user_agent(user_agent)
        self._http_debug = http_debug
        self._update_opener()
        
    
    def set_cookies(self, cookie_file):
        '''
        Set the cookie file and try to load cookies from it if it exists.
        
        Args:
            cookie_file (str): Full path to a file to be used to load and save
            cookies to.
        '''
        try:
            self._cj.load(cookie_file, ignore_discard=True)
            self._update_opener()
            return True
        except:
            return False
        
    
    def get_cookies(self):
        '''Returns A dictionary containing all cookie information by domain.'''
        return self._cj._cookies


    def save_cookies(self, cookie_file):
        '''
        Saves cookies to a file.
        
        Args:
            cookie_file (str): Full path to a file to save cookies to.
        '''
        self._cj.save(cookie_file, ignore_discard=True)        

        
    def set_proxy(self, proxy):
        '''
        Args:
            proxy (str): Proxy setting (eg. 
            ``'http://user:pass@example.com:1234'``)
        '''
        self._proxy = proxy
        self._update_opener()

        
    def get_proxy(self):
        '''Returns string containing proxy details.'''
        return self._proxy
        
        
    def set_user_agent(self, user_agent):
        '''
        Args:
            user_agent (str): String to use as the User Agent header.
        '''
        self._user_agent = user_agent

        
    def get_user_agent(self):
        '''Returns user agent string.'''
        return self._user_agent


    def _update_opener(self, cloudflare_jar=False):
        """
        Builds and installs a new opener to be used by all future calls to
        :func:`urllib2.urlopen`.
        """
        if self._http_debug:
            http = urllib2.HTTPHandler(debuglevel=1)
        else:
            http = urllib2.HTTPHandler()

        if cloudflare_jar:
            self._cloudflare_jar = cookielib.MozillaCookieJar()
            jar = self._cloudflare_jar
        else:
            jar = self._cj

        if self._proxy:
            opener = urllib2.build_opener(urllib2.HTTPCookieProcessor(jar),
                                          urllib2.ProxyHandler({'http':
                                                                self._proxy}),
                                          urllib2.HTTPBasicAuthHandler(),
                                          http)

        else:
            opener = urllib2.build_opener(urllib2.HTTPCookieProcessor(jar),
                                          urllib2.HTTPBasicAuthHandler(),
                                          http)
        urllib2.install_opener(opener)


    def _parseJSString(self, s):
        """
        lambda
        plugin.video.genesis\resources\lib\libraries\cloudflare.py
        https://offshoregit.com/lambda81/
        """
        try:
            offset=1 if s[0]=='+' else 0
            val = int(eval(s.replace('!+[]','1').replace('!![]','1').replace('[]','0').replace('(','str(')[offset:]))
            return val
        except:
            raise Exception

    def checkpart(self, s, sens):
        number = 0
        p = 0
        if sens == 1:
            pos = 0
        else:
            pos = len(s) - 1

        try:
            while 1:
                c = s[pos]

                if ((c == '(') and (sens == 1)) or ((c == ')') and (sens == -1)):
                    p = p + 1
                if ((c == ')') and (sens == 1)) or ((c == '(') and (sens == -1)):
                    p = p - 1
                if (c == '+') and (p == 0) and (number > 1):
                    break

                number += 1
                pos = pos + sens
        except:
            pass
        if sens == 1:
            return s[:number], number
        else:
            return s[-number:], number

    def parseInt(self, s):
        offset = 1 if s[0] == '+' else 0
        chain = s.replace('!+[]', '1').replace('!![]', '1').replace('[]', '0').replace('(', 'str(')[offset:]

        if '/' in chain:
            val = chain.split('/')
            links, sizeg = self.checkpart(val[0], -1)
            rechts, sized = self.checkpart(val[1], 1)

            if rechts.startswith('+') or rechts.startswith('-'):
                rechts = rechts[1:]
            gg = eval(links)
            dd = eval(rechts)
            chain = val[0][:-sizeg] + str(gg) + '/' + str(dd) + val[1][sized:]
        val = float(eval(chain))
        return val

    def _extract_js_old(self, htmlcontent, domain):
        line1 = re.findall('var s,t,o,p,b,r,e,a,k,i,n,g,f, (.+?)={"(.+?)":\+*(.+?)};', htmlcontent)
        varname = line1[0][0] + '.' + line1[0][1]
        calc = self.parseInt(line1[0][2])
        AllLines = re.findall(';' + varname + '([*\-+])=([^;]+)', htmlcontent)

        for aEntry in AllLines:
            calc = eval(format(calc, '.17g') + str(aEntry[0]) + format(self.parseInt(aEntry[1]), '.17g'))
        rep = calc + len(domain)
        return format(rep, '.10f')

#######################
# neu
#######################

    def cf_sample_domain_function(self, func_expression, domain):
#        print "cf_sample_domain_functio"
        parameter_start_index = func_expression.find('}(') + 2
        sample_index = self.cf_parse_expression(func_expression[parameter_start_index: func_expression.rfind(')))')])
        return ord(domain[int(sample_index)])

    def cf_arithmetic_op(self, op, a, b):
#        print "cf_arithmetic_op"

        if op == '+':
            return a + b
        elif op == '/':
            return a / float(b)
        elif op == '*':
            return a * float(b)
        elif op == '-':
            return a - b
        else:
            raise Exception('Unknown operation')

    def _get_jsfuck_number(self, section):
#        print "_get_jsfuck_number"

        digit_expressions = section.replace('!+[]', '1').replace('+!![]', '1').replace('+[]', '0').split('+')
        return int(''.join(str(sum(int(digit_char) for digit_char in digit_expression[1:-1]))
                               for digit_expression in digit_expressions))

    def cf_parse_expression(self, expression, domain=None):
#        print "cf_parse_expression"

        if '/' in expression:
            dividend, divisor = expression.split('/')
            dividend = dividend[2:-1]
            if domain:
                divisor_a, divisor_b = divisor.split('))+(')
                divisor_a = self._get_jsfuck_number(divisor_a[5:])
                divisor_b = self.cf_sample_domain_function(divisor_b, domain)
                return self._get_jsfuck_number(dividend) / float(divisor_a + divisor_b)
            else:
                divisor = divisor[2:-1]
                return self._get_jsfuck_number(dividend) / float(self._get_jsfuck_number(divisor))
        else:	
            return self._get_jsfuck_number(expression[2:-1])
	
#    @staticmethod
    def _extract_js(self, body, domain):
#        print "1 body", body
#        return "123"
#        print "2 domain", domain

        form_index = body.find('id="challenge-form"')
#        print "3 form_index", form_index

        sub_body = body[form_index:]
#        print "4 sub_body", sub_body

        if body.find('id="cf-dn-', form_index) != -1:
            extra_div_expression = re.search('id="cf-dn-.*?>(.+?)<', sub_body).group(1)
#            print "5 extra_div_expression", extra_div_expression

        js_answer = self.cf_parse_expression(re.search('setTimeout\(function\(.*?:(.*?)}', body, re.DOTALL).group(1)) 
#        print "6 js_answer", js_answer

        builder = re.search("challenge-form'\);\s*;(.*);a.value", body, re.DOTALL).group(1)
#        print "7 builder", builder

        lines = builder.replace(' return +(p)}();', '', 1).split(';')
#        print "8 lines", lines
        for line in lines:
#            print "9 line", line
            if len(line) and '=' in line:
                heading, expression = line.split('=', 1)
                if 'eval(eval(' in expression:
                    expression_value = self.cf_parse_expression(extra_div_expression)
                elif 'function(p' in expression:
                    expression_value = self.cf_parse_expression(expression, domain)
                else:
                    expression_value = self.cf_parse_expression(expression)
#                print "10 js_answer"
                js_answer = self.cf_arithmetic_op(heading[-1], js_answer, expression_value)
#                print "11 js_answer", js_answer
#            print "12 js_answer"

        if '+ t.length' in body:
            js_answer += len(domain)
        
        ret = format(js_answer, '.10f')
#        print "end ret", ret
        return (str(ret))

############################

    def createUrl(self, sUrl, oRequest):
        parsed_url = urlparse(sUrl)
        netloc = parsed_url.netloc[4:] if parsed_url.netloc.startswith('www.') else parsed_url.netloc
        cfId = oRequest.getCookie('__cfduid', '.' + netloc)
        cfClear = oRequest.getCookie('cf_clearance', '.' + netloc)

        if cfId and cfClear and 'Cookie=Cookie:' not in sUrl:
            delimiter = '&' if '|' in sUrl else '|'
            sUrl = sUrl + delimiter + "Cookie=Cookie: __cfduid=" + cfId.value + "; cf_clearance=" + cfClear.value

        if 'User-Agent=' not in sUrl:
            delimiter = '&' if '|' in sUrl else '|'
            sUrl += delimiter + "User-Agent=" + oRequest.getHeaderEntry('User-Agent')
        print "sUrl", sUrl
        return sUrl

    def _cloudflare_challenge(self, url, challenge, form_data={}, headers={}, compression=True):
        """
        Use _set_cloudflare to call this, not intended to be called directly.
        Solve challenge and make request with cloudflare cookie jar

        Part from:
        lambda
        plugin.video.genesis\resources\lib\libraries\cloudflare.py
        https://offshoregit.com/lambda81/
        """

        jschl = re.compile('name="jschl_vc" value="(.+?)"/>').findall(challenge)[0]
        passw = re.compile('name="pass" value="(.+?)"/>').findall(challenge)[0]
        s = re.compile('name="s"\svalue="(?P<s_value>[^"]+)').findall(challenge)[0]

#        print "jschl", jschl
#        print "passw", passw
#        print "s", s

        js = self._extract_js(challenge, url)

#        body = response.read()
        body = challenge
        parsed_url = urlparse(url)
        submit_url = "%s://%s/cdn-cgi/l/chk_jschl" % (parsed_url.scheme, parsed_url.netloc)

        params = {}
#        print "123", submit_url

        try:
#            print "try", submit_url

            params["jschl_vc"] = re.search(r'name="jschl_vc" value="(\w+)"', body).group(1)
            params["pass"] = re.search(r'name="pass" value="(.+?)"', body).group(1)
            params["s"] = re.search(r'name="s"\svalue="(?P<s_value>[^"]+)', body).group(1)

            js = self._extract_js(body, parsed_url.netloc)
        except:
            print "except", submit_url

            return None

        params["jschl_answer"] = js
        sParameters = urllib.urlencode(params, True)

        request = urllib2.Request("%s?%s" % (submit_url, sParameters))

        for key in headers:
            request.add_header(key, headers[key])
        sleep(5)

        path = urlparse(url).path
        netloc = urlparse(url).netloc

        if not netloc:
            netloc = path

#        answer = decrypt_val + len(netloc)
        answer = js

        url = url.rstrip('/')
#        query = '%s/cdn-cgi/l/chk_jschl?jschl_vc=%s&jschl_answer=%s' % (url, jschl, answer)
        query = '%s/cdn-cgi/l/chk_jschl?s=%s&jschl_vc=%s&jschl_answer=%s' % (url, urllib.quote_plus(s), jschl, answer)
#        print "query1", query

        if 'type="hidden" name="pass"' in challenge:
            passval = re.compile('name="pass" value="(.*?)"').findall(challenge)[0]
#            query = '%s/cdn-cgi/l/chk_jschl?pass=%s&jschl_vc=%s&jschl_answer=%s' % \
#                    (url, urllib.quote_plus(passval), jschl, answer)
            query = '%s/cdn-cgi/l/chk_jschl?s=%s&jschl_vc=%s&pass=%s&jschl_answer=%s' % \
                    (url, urllib.quote_plus(s), jschl, passval, answer)
#            print "query2", query

            time.sleep(9)

#        createUrl(sUrl, oRequest):
 
        self._update_opener()
        req = urllib2.Request(query)
        if form_data:
            form_data = urllib.urlencode(form_data)
            req = urllib2.Request(query, form_data)
        req.add_header('User-Agent', self._user_agent)
        for k, v in headers.items():
            req.add_header(k, v)
        if compression:
            req.add_header('Accept-Encoding', 'gzip')
        try:
            response = urllib2.urlopen(req)
        except urllib2.HTTPError as e:
            pass


    def _set_cloudflare(self, url, challenge, form_data={}, headers={}, compression=True):
        """
        Entry Point for _cloudflare_challenge
        Calls cloudflare_challenge on netloc, not full url w/ path
        Puts any cloudflare cookies in the main cookie jar
        Args:
            url (str): The URL to site of potential Cloudflare IUA.

            challenge (str): html contents of the page that raised 503, containing potential Cloudflare IUA Challenge
        Kwargs:
            form_data (dict): A dictionary of form data if pass-through from POST.

            headers (dict): A dictionary describing any headers you would like
            to add to the request. (eg. ``{'X-Test': 'testing'}``)

            compression (bool): If ``True`` (default), try to use gzip
            compression.
        """
#        print "_set_cloudflare"
        netloc = urlparse(url).netloc
        if not netloc:
            netloc = urlparse(url).path
        cloudflare_url = urlunparse((urlparse(url).scheme, netloc, '', '', '', ''))

        try:
#            print "_set_cloudflare try"
            self._cloudflare_challenge(cloudflare_url, challenge, form_data, headers, compression)
            for c in self._cloudflare_jar:
#                print "_set_cloudflare c: ", c
                self._cj.set_cookie(c)
            self._update_opener(cloudflare_jar=True)
        except:
            # make sure we update to main jar
#            print "_set_cloudflare except"

            self._update_opener()
            raise Exception


    def url_with_headers(self, url, referer=None, user_agent=None, cookies=None, proxy=None, connection_timeout=None,
                         encoding='', accept_charset='', sslcipherlist='', noshout='false', seekable='1'):
        '''
        Return url with Referer, User-Agent, Cookies, Proxy, Connection-Timeout, Encoding, Accept-Charset,
        SSLCipherList, NoShout and Seekable
        Based on: https://github.com/xbmc/xbmc/blob/master/xbmc/filesystem/CurlFile.cpp#L782
        Args:
            url (str): The URL to append headers to.

        Kwargs:
            referer (str): If None (default), urlunparse((urlparse(url).scheme, netloc, path, '', '', '')) is used and append if set

            user_agent (str): If None (default), self._user_agent is used and append if set

            cookies (bool): If ``None`` (default), use self.cloudflare as bool (False as default)
            Append cookies to URL as well

            proxy (str): If None (default), self.proxy is used and append if set

            connection_timeout (str): If None (default), self._socket_timeout is used and append if set

            encoding (str): append if set

            accept_charset (str): append if set

            sslcipherlist (str): append if set

            noshout (str): 'true'/'false', skip shout, append if 'true' ('false' is kodi default)

            seekable (str): '0'/'1', append if 0 ('1' is kodi default)
        Returns:
            http://example.com/myimage.png|Referer=%%%%%&User-Agent=%%%%%...
        '''
        kodi_schemes = ('special', 'plugin', 'script', 'profile')
        if ('://' not in url) or (url.startswith(kodi_schemes)):
            # don't waste time and return url
            return url

        _tmp = re.search('(.+?)(?:\|.*|$)', url)
        if _tmp:
            # trim any headers that may already be attached to url
            url = _tmp.group(1)

        if referer is not None:
            try:
                referer = str(referer)
            except:
                referer = None
        if referer is None:
            path = urlparse(url).path
            netloc = urlparse(url).netloc
            if not netloc:
                netloc = path
                path = ''
            referer = urlunparse((urlparse(url).scheme, netloc, path, '', '', ''))
            if referer == url:
                index = path.rfind('/')
                if index >= 0:
                    referer = urlunparse((urlparse(url).scheme, netloc, path[:index], '', '', ''))
        if user_agent is None:
            user_agent = self._user_agent
        else:
            try:
                user_agent = str(user_agent)
            except:
                user_agent = self._user_agent
        if cookies is None:
            cookies = self.cloudflare
        if proxy is None:
            proxy = self._proxy
        if connection_timeout is None:
            connection_timeout = self._socket_timeout
        try:
            connection_timeout = str(connection_timeout)
        except:
            connection_timeout = None
        try:
            if str(seekable) != '0':
                seekable = None
        except:
            seekable = None
        try:
            if str(noshout).lower() != 'true':
                noshout = None
        except:
            noshout = None

        url += '|Referer=' + urllib.quote_plus(referer) + '&User-Agent=' + urllib.quote_plus(user_agent)
        if proxy:
            try:
                url += '&HTTPProxy=' + urllib.quote_plus(str(proxy))
            except:
                pass
        if connection_timeout:
            url += '&Connection-Timeout=' + urllib.quote_plus(connection_timeout)
        if encoding:
            try:
                url += '&Encoding=' + urllib.quote_plus(str(encoding))
            except:
                pass
        if accept_charset:
            try:
                url += '&Accept-Charset=' + urllib.quote_plus(str(accept_charset))
            except:
                pass
        if sslcipherlist:
            try:
                url += '&SSLCipherList=' + urllib.quote_plus(str(sslcipherlist))
            except:
                pass
        if noshout:
            url += '&NoShout=' + urllib.quote_plus(str(noshout).lower())
        if seekable:
            url += '&Seekable=' + urllib.quote_plus(str(seekable))
        if cookies:
            cookie_string = ''
            for c in self._cj:
                if c.domain and (c.domain.lstrip('.') in url):
                    cookie_string += '%s=%s;' % (c.name, c.value)
            if cookie_string:
                url += '&Cookie=' + urllib.quote_plus(cookie_string)
        return url


    def http_GET(self, url, headers={}, compression=True, cloudflare=None):
        '''
        Perform an HTTP GET request.
        
        Args:
            url (str): The URL to GET.
            
        Kwargs:
            headers (dict): A dictionary describing any headers you would like
            to add to the request. (eg. ``{'X-Test': 'testing'}``)

            compression (bool): If ``True`` (default), try to use gzip 
            compression.

            cloudflare (bool): If ``None`` (default), use self.cloudflare as bool (False as default)
            On HTTPError 503 check for Cloudflare challenge and solve
        Returns:
            An :class:`HttpResponse` object containing headers and other 
            meta-information about the page and the page content.
        '''
#        print "http_GET"

        if cloudflare is None:
            cloudflare = self.cloudflare
        return self._fetch(url, headers=headers, compression=compression, cloudflare=cloudflare)
        

    def http_POST(self, url, form_data, headers={}, compression=True, cloudflare=None):
        '''
        Perform an HTTP POST request.
        
        Args:
            url (str): The URL to POST.
            
            form_data (dict): A dictionary of form data to POST.
            
        Kwargs:
            headers (dict): A dictionary describing any headers you would like
            to add to the request. (eg. ``{'X-Test': 'testing'}``)

            compression (bool): If ``True`` (default), try to use gzip 
            compression.

            cloudflare (bool): If ``None`` (default), use self.cloudflare as bool (False as default)
            On HTTPError 503 check for Cloudflare challenge and solve
        Returns:
            An :class:`HttpResponse` object containing headers and other 
            meta-information about the page and the page content.
        '''
        if cloudflare is None:
            cloudflare = self.cloudflare
        return self._fetch(url, form_data, headers=headers,
                           compression=compression, cloudflare=cloudflare)

    
    def http_HEAD(self, url, headers={}):
        '''
        Perform an HTTP HEAD request.
        
        Args:
            url (str): The URL to GET.
        
        Kwargs:
            headers (dict): A dictionary describing any headers you would like
            to add to the request. (eg. ``{'X-Test': 'testing'}``)
        
        Returns:
            An :class:`HttpResponse` object containing headers and other 
            meta-information about the page.
        '''
        req = HeadRequest(url)
        req.add_header('User-Agent', self._user_agent)
        req.add_header('Accept', self._accept)
        for k, v in headers.items():
            req.add_header(k, v)
        response = urllib2.urlopen(req)
        return HttpResponse(response)


    def _fetch(self, url, form_data={}, headers={}, compression=True, cloudflare=None):
        '''
        Perform an HTTP GET or POST request.
        
        Args:
            url (str): The URL to GET or POST.
            
            form_data (dict): A dictionary of form data to POST. If empty, the 
            request will be a GET, if it contains form data it will be a POST.
            
        Kwargs:
            headers (dict): A dictionary describing any headers you would like
            to add to the request. (eg. ``{'X-Test': 'testing'}``)

            compression (bool): If ``True`` (default), try to use gzip 
            compression.

            cloudflare (bool): If ``None`` (default), use self.cloudflare as bool (False as default)
            On HTTPError 503 check for Cloudflare challenge and solve
        Returns:
            An :class:`HttpResponse` object containing headers and other 
            meta-information about the page and the page content.
        '''
#        print "_fetch"
        if cloudflare is None:
            cloudflare = self.cloudflare
        encoding = ''
        req = urllib2.Request(url)
        if form_data:
            form_data = urllib.urlencode(form_data)
            req = urllib2.Request(url, form_data)
        req.add_header('User-Agent', self._user_agent)
        for k, v in headers.items():
            req.add_header(k, v)
        if compression:
            req.add_header('Accept-Encoding', 'gzip')
        if not cloudflare:
            response = urllib2.urlopen(req)
            return HttpResponse(response)
        else:
            try:
                response = urllib2.urlopen(req)
                return HttpResponse(response)
            except urllib2.HTTPError as e:
                if e.code == 503:
                    try:
                        self._set_cloudflare(url, e.read(), form_data, headers, compression)
                    except:
                        raise urllib2.HTTPError, e
                    req = urllib2.Request(url)

                    if form_data:
                        form_data = urllib.urlencode(form_data)
                        req = urllib2.Request(url, form_data)
                    req.add_header('User-Agent', self._user_agent)
                    for k, v in headers.items():
                        req.add_header(k, v)
                    if compression:
                        req.add_header('Accept-Encoding', 'gzip')
                    response = urllib2.urlopen(req)
                    return HttpResponse(response)
                else:
                    raise urllib2.HTTPError, e


class HttpResponse:
    '''
    This class represents a response from an HTTP request.
    
    The content is examined and every attempt is made to properly encode it to
    Unicode.
    
    .. seealso::
        :meth:`Net.http_GET`, :meth:`Net.http_HEAD` and :meth:`Net.http_POST` 
    '''
    
    content = ''
    '''Unicode encoded string containing the body of the response.'''
    
    
    def __init__(self, response):
        '''
        Args:
            response (:class:`mimetools.Message`): The object returned by a call
            to :func:`urllib2.urlopen`.
        '''
        self._response = response
        html = response.read()
        try:
            if response.headers['content-encoding'].lower() == 'gzip':
                html = gzip.GzipFile(fileobj=StringIO.StringIO(html)).read()
        except:
            pass
        
        try:
            content_type = response.headers['content-type']
            if 'charset=' in content_type:
                encoding = content_type.split('charset=')[-1]
        except:
            pass

        r = re.search('<meta\s+http-equiv="Content-Type"\s+content="(?:.+?);' +
                      '\s+charset=(.+?)"', html, re.IGNORECASE)
        if r:
            encoding = r.group(1) 
                   
        try:
            html = unicode(html, encoding)
        except:
            pass
        
        #try:
        #    if response.headers['content-encoding'].lower() == 'gzip':
        #        r = re.search('<meta\s+http-equiv="Content-Type"\s+content="(?:.+?);' + '\s+charset=(.+?)"', html, re.IGNORECASE)
        #        if r:
        #        	encoding = r.group(1) 
        #        	try:
        #        		html = unicode(html, encoding)
        #        	except:
        #        		pass
        #except:
        #    pass
            
        self.content = html
    
    
    def get_headers(self):
        '''Returns a List of headers returned by the server.'''
        return self._response.info().headers
    
        
    def get_url(self):
        '''
        Return the URL of the resource retrieved, commonly used to determine if 
        a redirect was followed.
        '''
        return self._response.geturl()
