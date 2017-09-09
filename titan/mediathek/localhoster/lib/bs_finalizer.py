# coding: UTF-8

import sys

l1lll1 = sys.version_info[0] == 2
l11 = 26
l1l1l1 = 2048
l11l = 7
l1l1 = False


def l1111(ll):
    global l1lll1
    global l11
    global l1l1l1
    global l11l

    l1ll11 = ord(ll[-1]) - l1l1l1
    ll = ll[:-1]

    if ll:
        l111l1 = (l1ll11) % len(ll)
    else:
        l111l1 = 0

    if l1lll1:
        l111 = u''.join([unichr(ord(l1111l) - l1l1l1 - (l1l11l + l1ll11) % l11l) for l1l11l, l1111l in
                         enumerate(ll[:l111l1] + ll[l111l1:])])
    else:
        l111 = ''.join([unichr(ord(l1111l) - l1l1l1 - (l1l11l + l1ll11) % l11l) for l1l11l, l1111l in
                        enumerate(ll[:l111l1] + ll[l111l1:])])

    if l1l1:
        return str(l111)
    else:
        return l111


import time
import json as j
import base64 as l1
import hmac as l11ll1
import hashlib as l1ll

l1ll1l = l1111(u"࡫ࡸ࡯࡮ࠢࡵࡩࡶࡻࡥࡴࡶࡋࡥࡳࡪ࡬ࡦࡴࠣ࡭ࡲࡶ࡯ࡳࡶࠣࡧࡗ࡫ࡱࡶࡧࡶࡸࡍࡧ࡮ࡥ࡮ࡨࡶࠚ")

#try:
#    exec (l1ll1l)
#    l11l1l = l1111 (u"ࡗࡓࡔࡐࡧࡏࡦ࠳࡙ࡳࡸࡷࡍࡣࡥࡉࡈࡖ࡫ࡍࡧࡵࡻ࠴࠷࠻ࡁࡂ࠵ࡅࡹࡨࠥ")
#    l1l111 = l1111 (u"࠹ࡎࡋࡳࡕࡪ࡚ࡵࡅࡍࡍࡺ࡫ࡓࡪࡺ࠻࠵ࡻࡊࡊࡩࡒࡵࡑࡼࡢࡍࡖ࠼ࡵࡔ࠳")
#except:
#    pass

#exec (l1ll1l)
l11l1l = l1111 (u"ࡗࡓࡔࡐࡧࡏࡦ࠳࡙ࡳࡸࡷࡍࡣࡥࡉࡈࡖ࡫ࡍࡧࡵࡻ࠴࠷࠻ࡁࡂ࠵ࡅࡹࡨࠥ")
l1l111 = l1111 (u"࠹ࡎࡋࡳࡕࡪ࡚ࡵࡅࡍࡍࡺ࡫ࡓࡪࡺ࠻࠵ࡻࡊࡊࡩࡒࡵࡑࡼࡢࡍࡖ࠼ࡵࡔ࠳")

def mod_request(request, string):
#    print "1111111111", l1111(u"ࡄࡖ࠱࡙ࡵ࡫ࡦࡰࠥ")
#    print "2222222222", l111ll(string)
#    print "3333333333", l1111(u"ࡘࡷࡪࡸ࠭ࡂࡩࡨࡲࡹࠦ")
 #   print "4444444444", l1111(u"ࡧࡹ࠮ࡢࡰࡧࡶࡴ࡯ࡤ࠽")
    print "%s: %s" % (l1111(u"ࡄࡖ࠱࡙ࡵ࡫ࡦࡰࠥ"), l111ll(string))


#    request.addHeaderEntry(l1111(u"ࡄࡖ࠱࡙ࡵ࡫ࡦࡰࠥ"), l111ll(string))
#    request.addHeaderEntry(l1111(u"ࡘࡷࡪࡸ࠭ࡂࡩࡨࡲࡹࠦ"), l1111(u"ࡧࡹ࠮ࡢࡰࡧࡶࡴ࡯ࡤ࠽"))

def l111ll(l1lll):
    l11l11 = int(time.time())
    l11lll = {}
    l11lll[l1111(u"ࡱࡷࡥࡰ࡮ࡩ࡟࡬ࡧࡼࠫ")] = l11l1l
    l11lll[l1111(u"ࡸ࡮ࡳࡥࡴࡶࡤࡱࡵࡑ")] = l11l11
    l11lll[l1111(u"ࡩ࡯ࡤࡧࡇ")] = l1l11(l11l11, l1lll)
    return l1.b64encode(j.dumps(l11lll).encode(l1111(u"ࡻࡴࡧ࠯࠻ࠩ")))


def l1l11(l11l11, l1l1l):
    l1ll1 = l1l111.encode(l1111(u'ࡺࡺࡦ࠮࠺ࡒ'))
    l1l1ll = str(l11l11) + l1111(u'࠵ࠛ') + str(l1l1l)
    l1l1ll = l1l1ll.encode(l1111(u'ࡦࡹࡣࡪ࡫࠯'))
    l1lllll = l11ll1.new(l1ll1, l1l1ll, digestmod=l1ll.sha256)
    return l1lllll.hexdigest()
