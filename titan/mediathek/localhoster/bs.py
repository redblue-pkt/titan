
import re, time
from lib import jsunpack
import sys
from lib.net import Net
import lib.common as common
from lib.bs_finalizer import *

class BsResolver(object):
    name = "flashx"
    domains = ["flashx.tv"]
    pattern = '(?://|\.)(flashx\.tv)/(?:embed-|dl\?|embed.php\?c=)?([0-9a-zA-Z/-]+)'

    def __init__(self):
#        print "1111111111111"
        self.net = Net()
#        print "2222222222222"

        url = str(sys.argv[1])
#        print "3333333333333"

 #       host = self.get_host_and_id(url)[0]
#        print "4444444444444"

 #       media_id = self.get_host_and_id(url)[1]
#        print "5555555555555"

#        mod_request(request, urlPart)
#        mod_request("request", url)
#	curl -L -H "BS-Token:JwdWJsaWNfa2V5IjogIlVQUEthT2UxVm9zcU1iY0ZEUWVNZnN4MDI1QUEzQnVjIiwgImhtYWMiOiAiNDA4NzU2ZTMyNzhkZGUwZDVhZGQyMzMxMzNkZTYwM2RhZGEwN2I5MDU5MmM0MDM0OGQ2ZDQwYzdkYjNjZDI4ZSIsICJ0aW1lc3RhbXAiOiAxNTA0OTc4MjcyfQ==" https://www.bs.to/api/watch/2977318

        return mod_request("request", url)

sys.stdout = BsResolver()
