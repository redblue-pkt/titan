#-*- coding: utf-8 -*-
# code von venom-xbmc-addons and VeNoMouS tnx

from __future__ import division
#import re, os, time, json, random, ssl, requests
import re, os, time, json, random, ssl
from cCFScrape import cCFScrape
from requests import *

from requests.adapters import HTTPAdapter
from collections import OrderedDict
from requests.sessions import Session
from jsunfuck import JSUnfuck
#from resources.lib import common
#import lib.common as common
import common

try:
    from urlparse import urlparse
except ImportError:
    from urllib.parse import urlparse

#old version
#from lib.requests.packages.urllib3.util.ssl_ import create_urllib3_context
from requests.urllib3.util.ssl_ import create_urllib3_context

class GestionCookie():
    PathCache = common.profilePath

    def DeleteCookie(self,Domain):
        Name = "/".join([self.PathCache, "Cookie_%s.txt"]) % (Domain)
        if os.path.exists(os.path.join(self.PathCache,Name)):
             os.remove(os.path.join(self.PathCache,Name))

    def SaveCookie(self,Domain,data):
        Name = os.path.join(self.PathCache,'Cookie_'+ str(Domain) +'.txt')
        file = open(Name,'w')
        file.write(data)
        file.close()

    def Readcookie(self,Domain):
        Name = os.path.join(self.PathCache,'Cookie_'+ str(Domain) +'.txt')
        try:
            file = open(Name,'r')
            data = file.read()
            file.close()
        except:
            return ''
        return data

class CipherSuiteAdapter(HTTPAdapter):
    def __init__(self, cipherSuite=None, **kwargs):
        self.cipherSuite = cipherSuite

        if hasattr(ssl, 'PROTOCOL_TLS'):
            self.ssl_context = create_urllib3_context(
                ssl_version=getattr(ssl, 'PROTOCOL_TLSv1_3', ssl.PROTOCOL_TLSv1_2),
                ciphers=self.cipherSuite
            )
        else:
            self.ssl_context = create_urllib3_context(ssl_version=ssl.PROTOCOL_TLSv1)
        super(CipherSuiteAdapter, self).__init__(**kwargs)

    def init_poolmanager(self, *args, **kwargs):
        kwargs['ssl_context'] = self.ssl_context
        return super(CipherSuiteAdapter, self).init_poolmanager(*args, **kwargs)

    def proxy_manager_for(self, *args, **kwargs):
        kwargs['ssl_context'] = self.ssl_context
        return super(CipherSuiteAdapter, self).proxy_manager_for(*args, **kwargs)

##########################################################################################################################################################

Mode_Debug = False

if (False):
    Mode_Debug = True
    import logging
    # These two lines enable debugging at httplib level (requests->urllib3->http.client)
    # You will see the REQUEST, including HEADERS and DATA, and RESPONSE with HEADERS but without DATA.
    # The only thing missing will be the response.body which is not logged.
    try:
        import http.client as http_client
    except ImportError:
        # Python 2
        import httplib as http_client
    http_client.HTTPConnection.debuglevel = 1

    # You must initialize logging, otherwise you'll not see debug output.
    logging.basicConfig()
    logging.getLogger().setLevel(logging.DEBUG)
    requests_log = logging.getLogger("requests.packages.urllib3")
    requests_log.setLevel(logging.DEBUG)
    requests_log.propagate = True

UA = 'Mozilla/5.0 (Windows NT 6.1; Win64; x64; rv:63.0) Gecko/20100101 Firefox/63.0'

def checklowerkey(key,dict):
    for i in dict:
        if str(i.lower()) == str(key.lower()):
            return i
    return False

def solvecharcode(chain,t):
    v = chain.find('t.charCodeAt') + 12
    if v == 11:
        return chain

    dat = checkpart(chain[v:],')')
    r = parseInt(dat)
    v = ord(t[int(r)])
    #print  ('value ' + str(r) + ' Result ' + str(v))
    chain = chain.replace('t.charCodeAt' + dat, '+' + str(v) )
    #Remove parzenthesis
    chain = chain.replace( '(' + '+' + str(v) + ')' , '+' + str(v))
    return chain

def checkpart(s,end='+'):
    p = 0
    pos = 0
    try:
        while (1):
            c = s[pos]
            if (c == '('):
                p = p + 1
            if (c == ')'):
                p = p - 1
            pos = pos + 1
            if (c == end) and (p == 0) and (pos > 1):
                break   
    except:
        pass
    return s[:pos]

def parseInt(s):
    v = JSUnfuck(s).decode(False)
    v = re.sub('([^\(\)])\++', '\\1', v)
    v = eval(v)
    return v

def CheckIfActive(data):
    if 'Checking your browser before accessing' in str(data):
    #if ( "URL=/cdn-cgi/" in head.get("Refresh", "") and head.get("Server", "") == "cloudflare-nginx" ):
        return True
    return False

class CloudflareBypass(object):
    def __init__(self):
        self.state = False
        self.HttpReponse = None
        self.Memorised_Headers = None
        self.Memorised_PostData = None
        self.Memorised_Cookies = None
        self.Header = None
        self.RedirectionUrl = None

        #self.s = requests.Session()

    #Return param for head
    def GetHeadercookie(self,url):
        #urllib.parse.quote_plus()
        Domain = re.sub(r'https*:\/\/([^/]+)(\/*.*)','\\1',url)
        cook = GestionCookie().Readcookie(Domain.replace('.','_'))
        if cook == '':
            return ''
        return '|' + urllib.urlencode({'User-Agent':UA,'Cookie': cook })

    def ParseCookies(self,data):
        list = {}
        sPattern = '(?:^|[,;]) *([^;,]+?)=([^;,\/]+)'
        aResult = re.findall(sPattern,data)
        #print (str(aResult))
        if (aResult):
            for cook in aResult:
                if 'deleted' in cook[1]:
                    continue
                list[cook[0]]= cook[1]
                #cookies = cookies + cook[0] + '=' + cook[1]+ ';'
        #print (str(list))
        return list

    def SetHeader(self):
        head = OrderedDict()
        #Need to use correct order
        h = ['User-Agent','Accept','Accept-Language','Accept-Encoding','Connection','Upgrade-Insecure-Requests']
        v = [UA,'text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8','en-US,en;q=0.5','gzip, deflate','close','1']
        for i in enumerate(h):
            k = checklowerkey(i[1],self.Memorised_Headers)
            if k:
                head[i[1]] = self.Memorised_Headers[k]
            else:
                head[i[1]] = v[i[0]]

        #optional headers
        if 'Referer' in self.Memorised_Headers:
            head['Referer'] = self.Memorised_Headers['Referer']

        if (False):
            #Normalisation because they are not case sensitive:
            Headers = ['User-Agent','Accept','Accept-Language','Accept-Encoding','Cache-Control','Dnt','Pragma','Connexion']
            Headers_l = [x.lower() for x in Headers]
            head2 = dict(head)
            for key in head2:
                if not key in Headers and key.lower() in Headers_l:
                    p  = Headers_l.index(key.lower())
                    head[Headers[p]] = head[key]
                    del head[key]

        return head

    def GetResponse(self,htmlcontent,domain):
        #print (htmlcontent)

        #truc cache
        rq = re.search('<div style="display:none;visibility:hidden;" id="(.*?)">(.*?)<\/div>', str(htmlcontent),re.MULTILINE | re.DOTALL)
        id = rq.group(1)
        val = rq.group(2)
        #print  (str(id) + ' ' + str(val))

        htmlcontent = re.sub(
            r'function\(p\){var p = eval\(eval\(.+?return \+\(p\)}\(\);',
            "{};".format(rq.group(2)),
            str(htmlcontent)
        )

        #For compatibility
        if '+ t.length' not in htmlcontent:
            #domain = 
            pass

        line1 = re.findall('var s,t,o,p,b,r,e,a,k,i,n,g,f, (.+?)={"(.+?)":\+*(.+?)};',str(htmlcontent))

        varname = line1[0][0] + '.' + line1[0][1]
        calcul = parseInt(line1[0][2])

        t = domain

        js = htmlcontent
        #Cleaning
        js = re.sub(r"a\.value = ((.+).toFixed\(10\))?", r"\1", js)
        js = re.sub(r"\s{3,}[a-z](?: = |\.).+", "", js).replace("t.length", str(len(domain)))
        js = js.replace('; 121', '')
        js = re.sub(r'function\(p\){return eval\(\(true\+.+?}', 't.charCodeAt',js)
        js = re.sub(r"[\n\\']", "", js)
        js = solvecharcode(js,t)
        htmlcontent = js

        AllLines = re.findall(';' + varname + '([*\-+])=([^;]+)',str(htmlcontent))
        #print  ('\nFirst line : ' + str(line1[0][2]) )

        for aEntry in AllLines:

            #print  ('\nother lines : ' + str(aEntry))
            s = str(aEntry[0])
            v = parseInt(aEntry[1])

            calcul = eval( format(calcul,'.17g') + str(aEntry[0]) + format(v,'.17g'))
            #print (">>>>>>>>>>>>>>>>: " + format(calcul,'.17g')+ '\n')

        rep = calcul# + len(domain)
        ret = format(rep,'.10f')

        return (str(ret))

    def GetReponseInfo(self):
        return self.RedirectionUrl, self.Header

    def GetHtml(self,url,htmlcontent = '',cookies = '',postdata = None,Gived_headers = ''):

        #Memorise headers
        self.Memorised_Headers = Gived_headers

        #Memorise postdata
        self.Memorised_PostData = postdata

        #Memorise cookie
        self.Memorised_Cookies = cookies
        #print (cookies)

        #cookies in headers ?
        if Gived_headers != '':
            if Gived_headers.get('Cookie',None):
                if cookies:
                    self.Memorised_Cookies = cookies + '; ' + Gived_headers.get('Cookie')
                else:
                    self.Memorised_Cookies = Gived_headers['Cookie']

        #For debug
        if (Mode_Debug):
            print ('Headers vorhanden ' + str(Gived_headers))
            print ('url ' + url)
            if (htmlcontent):
                print ('code html ok')
            print ('cookies verfallen' + self.Memorised_Cookies)
            print ('post data :' + str(postdata))

        self.hostComplet = re.sub(r'(https*:\/\/[^/]+)(\/*.*)','\\1',url)
        self.host = re.sub(r'https*:\/\/','',self.hostComplet)
        self.url = url

        cookieMem = GestionCookie().Readcookie(self.host.replace('.', '_'))
        if not (cookieMem == ''):
            if (Mode_Debug):
                print ('cookies vorhanden :' + cookieMem )
            if not (self.Memorised_Cookies):
                cookies = cookieMem
            else:
                cookies = self.Memorised_Cookies + '; ' + cookieMem
        else:
            if (Mode_Debug):
                print ('Keine Cookies vorhanden ' )

        data = {}
        if postdata:
            method = 'POST'
            #Need to convert data to dictionnary
            d = postdata.split('&')
            for dd in d:
                ddd = dd.split('=')
                data[ddd[0]] = ddd[1]
        else:
            method = 'GET'

        s = CloudflareScraper()
        r = s.request(method,url,headers = self.SetHeader() , cookies = self.ParseCookies(cookies) , data = data )
        if r:
            sContent = r.text.encode("utf-8")
            self.RedirectionUrl = r.url
            self.Header = r.headers
        else:
            print ("Fehler, Cookie löschen" )
            sContent = ''
            #self.RedirectionUrl = r.url
            #self.Header = r.headers
            s.MemCookie = ''
            GestionCookie().DeleteCookie(self.host.replace('.', '_'))

        #fh = open('c:\\test.txt', "w")
        #fh.write(sContent)
        #fh.close()
            
        #Memorisation des cookies
        c = ''
        cookie = s.MemCookie
        if cookie:
            for i in cookie:
                c = c + i + '=' + cookie[i] + ';'
            #Write them
            GestionCookie().SaveCookie(self.host.replace('.', '_'),c)
            if Mode_Debug:
                print ("Sauvegarde cookies : " + str(c) )
        return sContent

#----------------------------------------------------------------------------------------------------------------
# Code from https://github.com/VeNoMouS/cloudscraper
class CloudflareScraper(Session):
    def __init__(self, *args, **kwargs):
        super(CloudflareScraper, self).__init__(*args, **kwargs)
        self.cf_tries = 0
        self.GetCaptha = False

        self.firsturl = ''

        self.headers = {
                'User-Agent': UA,
                'Accept': 'text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8',
                'Accept-Language': 'en-US,en;q=0.5',
                'Connection': 'close',
                'Cache-Control': 'no-cache',
                'Pragma': 'no-cache',
                'DNT': '1'
            }

        self.cipherHeader = {
                'cipherSuite': [
                    "TLS_AES_128_GCM_SHA256",
                    "TLS_CHACHA20_POLY1305_SHA256",
                    "TLS_AES_256_GCM_SHA384",
                    "ECDHE-ECDSA-AES128-GCM-SHA256",
                    "ECDHE-RSA-AES128-GCM-SHA256",
                    "ECDHE-ECDSA-CHACHA20-POLY1305",
                    "ECDHE-ECDSA-AES256-GCM-SHA384",
                    "ECDHE-RSA-AES256-GCM-SHA384",
                    "ECDHE-ECDSA-AES256-SHA",
                    "ECDHE-ECDSA-AES128-SHA",
                    "ECDHE-RSA-AES128-SHA",
                    "ECDHE-RSA-AES256-SHA",
                    "DHE-RSA-AES128-SHA",
                    "AES128-SHA",
                    "AES256-SHA"
                ],
            }

        self.MemCookie = {}
        self.cipherSuite = None
        self.mount('https://', CipherSuiteAdapter(self.loadCipherSuite()))
        
    ##########################################################################################################################################################
    #
    #Thx again to to VeNoMouS for this code
    ##########################################################################################################################################################

    def loadCipherSuite(self):
        if self.cipherSuite:
            return self.cipherSuite
        self.cipherSuite = ''
        if hasattr(ssl, 'PROTOCOL_TLS'):
            ciphers = [
                'ECDHE-ECDSA-AES128-GCM-SHA256', 'ECDHE-ECDSA-CHACHA20-POLY1305-SHA256', 'ECDHE-RSA-CHACHA20-POLY1305-SHA256',
                'ECDHE-RSA-AES128-CBC-SHA', 'ECDHE-RSA-AES256-CBC-SHA', 'RSA-AES128-GCM-SHA256',
                'RSA-AES256-GCM-SHA384', 'RSA-AES256-SHA', '3DES-EDE-CBC'
            ]

            if hasattr(ssl, 'PROTOCOL_TLSv1_3'):
                ciphers.insert(0, ['GREASE_3A', 'GREASE_6A', 'AES128-GCM-SHA256', 'AES256-GCM-SHA256', 'AES256-GCM-SHA384', 'CHACHA20-POLY1305-SHA256'])

            ctx = ssl.SSLContext(getattr(ssl, 'PROTOCOL_TLSv1_3', ssl.PROTOCOL_TLSv1_2))

            for cipher in ciphers:
                try:
                    ctx.set_ciphers(cipher)
                    self.cipherSuite = '{}:{}'.format(self.cipherSuite, cipher).rstrip(':')
                except ssl.SSLError:
                    pass

        return self.cipherSuite

    ##########################################################################################################################################################

    def request(self, method, url, *args, **kwargs):
        
        if self.firsturl == '':
            self.firsturl = url
        
        if 'cookies' in kwargs:
            self.MemCookie.update( kwargs['cookies'] )
            
        if Mode_Debug:
            print ("Headers send : " + str(kwargs['headers']) )
            print ("Cookies send : " + str(kwargs['cookies']) )
            print ("url : " + url )
            print ("data send : " + str(kwargs.get('params','')) )
            print ("param send : " + str(kwargs.get('data','')) )
               
        resp = super(CloudflareScraper, self).request(method, url, *args, **kwargs)

        if Mode_Debug:
            print ( 'cookie recu ' + str(resp.cookies.get_dict())  )

        #save cookie
        self.MemCookie.update( resp.cookies.get_dict() )
        
        #bug
        kwargs['cookies'].update( resp.cookies.get_dict() )

        # Check if Cloudflare anti-bot is on
        if self.ifCloudflare(resp):
            
            print ('Page geschützt' )
            
            resp2 = self.solve_cf_challenge(resp, **kwargs)
            
            #self.MemCookie.update( resp.cookies.get_dict() )
            #print  ('cookie recu ' + str(self.MemCookie) )
        
            return resp2
            
        # Otherwise, no Cloudflare anti-bot detected
        if resp:
            print ('Page decodiert' )
        return resp

    def ifCloudflare(self, resp):
        if resp.headers.get('Server', '').startswith('cloudflare'):
            if self.cf_tries >= 3:
                print ('Failed to solve Cloudflare challenge!' )
            elif b'/cdn-cgi/l/chk_captcha' in resp.content:
                print ('Protect by Captcha' )
                #One more try ?
                if not self.GetCaptha:
                    self.GetCaptha = True
                    self.cf_tries = 0
                    #return 'captcha'
                    
            elif resp.status_code == 503:
                return True

            elif resp.status_code == 403:
                print ('403 Forbidden' )

            resp = False
            return False
        else:
            return False


    def solve_cf_challenge(self, resp, **original_kwargs):
        self.cf_tries += 1
        body = resp.text
        parsed_url = urlparse(resp.url)
        domain = parsed_url.netloc
        s_match = re.search('<form id="challenge-form" action="([^"]+)', body)
        if s_match:
            url_end = s_match.group(1)

        submit_url = "%s://%s%s" % (parsed_url.scheme, domain,url_end)

        cloudflare_kwargs = original_kwargs.copy( )
        #params = cloudflare_kwargs.setdefault("params", OrderedDict())
        data = OrderedDict()
        headers = cloudflare_kwargs.setdefault("headers", {})
        headers["Referer"] = str(resp.url)
        
        #fh = open('html.txt', "r")
        #body = fh.read()
        #fh.close()
        
        if Mode_Debug:
            print ('Trying decoding, pass ' + str(self.cf_tries) )
            
            #fh = open('c:\\test.txt', "w")
            #fh.write(body)
            #fh.close()
        
        try:
            cf_delay = float(re.search('submit.*?(\d+)', body, re.DOTALL).group(1)) / 1000.0

            form_index = body.find('id="challenge-form"')
            if form_index == -1:
                raise Exception('CF form not found')
            sub_body = body[form_index:]

            #s_match = re.search('name="s" value="(.+?)"', sub_body)
            #if s_match:
            #    params["s"] = s_match.group(1) # On older variants this parameter is absent.
                
            data["r"] = str(re.search(r'name="r" value="(.+?)"', sub_body).group(1))
            data["jschl_vc"] = str(re.search(r'name="jschl_vc" value="(\w+)"', sub_body).group(1))
            data["pass"] = str(re.search(r'name="pass" value="(.+?)"', sub_body).group(1)) 

            if body.find('id="cf-dn-', form_index) != -1:
                extra_div_expression = re.search('id="cf-dn-.*?>(.+?)<', sub_body).group(1)

            # Initial value.
            js_answer = self.cf_parse_expression(
                re.search('setTimeout\(function\(.*?:(.*?)}', body, re.DOTALL).group(1)
            )
            # Extract the arithmetic operations.
            builder = re.search("challenge-form'\);\s*;(.*);a.value", body, re.DOTALL).group(1)
            # Remove a function semicolon before splitting on semicolons, else it messes the order.
            lines = builder.replace(' return +(p)}();', '', 1).split(';')

            for line in lines:
                if len(line) and '=' in line:
                    heading, expression = line.split('=', 1)
                    if 'eval(eval(' in expression:
                        # Uses the expression in an external <div>.
                        expression_value = self.cf_parse_expression(extra_div_expression)
                    elif 'function(p' in expression:
                        # Expression + domain sampling function.
                        expression_value = self.cf_parse_expression(expression, domain)
                    else:
                        expression_value = self.cf_parse_expression(expression)
                    js_answer = self.cf_arithmetic_op(heading[-1], js_answer, expression_value)

            if '+ t.length' in body:
                js_answer += len(domain) # Only older variants add the domain length.

            data["jschl_answer"] = '%.10f' % js_answer

        except Exception as e:
            print  ('error')
            raise

        # Cloudflare requires a delay before solving the challenge.
        # Always wait the full delay + 1s because of 'time.sleep()' imprecision.
        time.sleep(cf_delay + 1.0)

        # Requests transforms any request into a GET after a redirect,
        # so the redirect has to be handled manually here to allow for
        # performing other types of requests even as the first request.
        method = resp.request.method
        cloudflare_kwargs["allow_redirects"] = False
        
        #print ('Trying :' + str(params))
        #print ('With :' + str(cloudflare_kwargs['cookies']))
        #print ('With :' + str(cloudflare_kwargs['headers']))

        #submit_url = 'http://httpbin.org/headers'
        
        cloudflare_kwargs['data'].update( data )
        method = 'POST'
        
        redirect = self.request(method, submit_url, **cloudflare_kwargs) 
        
        if not redirect:
            return False

        #self.MemCookie.update( redirect.cookies.get_dict() )
        
        print ( '>>>' + str( redirect.headers)   )
        
        if 'Location' in redirect.headers:
            redirect_location = urlparse(redirect.headers["Location"])
            
            #if not redirect_location.netloc:
            #    redirect_url = "%s://%s%s" % (parsed_url.scheme, domain, redirect_location.path)
            #    response = self.request(method, redirect_url, **original_kwargs)
            #    print ( '1' )

            if not redirect.headers["Location"].startswith('http'):
                redirect = 'https://'+domain+redirect.headers["Location"]
            else:
                redirect = redirect.headers["Location"]

            response = self.request(method, redirect, **original_kwargs)
        else:
            response = redirect

        # Reset the repeated-try counter when the answer passes.
        self.cf_tries = 0
        return response


    def cf_sample_domain_function(self, func_expression, domain):
        parameter_start_index = func_expression.find('}(') + 2
        # Send the expression with the "+" char and enclosing parenthesis included, as they are
        # stripped inside ".cf_parse_expression()'.
        sample_index = self.cf_parse_expression(
            func_expression[parameter_start_index : func_expression.rfind(')))')]
        )
        return ord(domain[int(sample_index)])

    def cf_arithmetic_op(self, op, a, b):
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

    def cf_parse_expression(self, expression, domain=None):

        def _get_jsfuck_number(section):
            digit_expressions = section.replace('!+[]', '1').replace('+!![]', '1').replace('+[]', '0').split('+')
            return int(
                # Form a number string, with each digit as the sum of the values inside each parenthesis block.
                ''.join(
                    str(sum(int(digit_char) for digit_char in digit_expression[1:-1])) # Strip the parenthesis.
                    for digit_expression in digit_expressions
                )
            )

        if '/' in expression:
            dividend, divisor = expression.split('/')
            dividend = dividend[2:-1] # Strip the leading '+' char and the enclosing parenthesis.

            if domain:
                # 2019-04-02: At this moment, this extra domain sampling function always appears on the
                # divisor side, at the end.
                divisor_a, divisor_b = divisor.split('))+(')
                divisor_a = _get_jsfuck_number(divisor_a[5:]) # Left-strip the sequence of "(+(+(".
                divisor_b = self.cf_sample_domain_function(divisor_b, domain)
                return _get_jsfuck_number(dividend) / float(divisor_a + divisor_b)
            else:
                divisor = divisor[2:-1]
                return _get_jsfuck_number(dividend) / float(_get_jsfuck_number(divisor))
        else:
            return _get_jsfuck_number(expression[2:-1])
