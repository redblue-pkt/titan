#!/usr/bin/env python2
import xml.etree.cElementTree
import socket
import glob
import os
import httplib
import copy

SATIPSERVERDATA = {}

DEVICE_ATTR = [ 
'friendlyName',
'manufacturer',
'manufacturerURL',
'modelDescription',
'modelName',
'modelNumber',
'modelURL',
'serialNumber',
'presentationURL'
]

#SATIP_CONFFILE = "/etc/vtuner.conf"
SATIP_CONFFILE = "/mnt/network/vtuner.conf"

SSDP_ADDR = "239.255.255.250";
#SSDP_ADDR = "10.0.0.255";
SSDP_PORT = 1900;
SSDP_MX = 1;
SSDP_ST = "urn:ses-com:device:SatIPServer:1";
ssdpRequest = "M-SEARCH * HTTP/1.1\r\n" + \
                "HOST: %s:%d\r\n" % (SSDP_ADDR, SSDP_PORT) + \
                "MAN: \"ssdp:discover\"\r\n" + \
                "MX: %d\r\n" % (SSDP_MX, ) + \
                "ST: %s\r\n" % (SSDP_ST, ) + "\r\n";
                
def getVtunerList():
	data = []
	for x in glob.glob('/dev/misc/vtuner*'):
		x = x.strip('/dev/misc/vtuner')
		data.append(x)
	data.sort()
	return data

VTUNER_IDX_LIST = getVtunerList()

def dataParse(data):
	serverData = {}
	for line in data.splitlines():
#		print "[*] line : ", line
		if line.find(':') != -1:
			(attr, value) = line.split(':', 1)
			attr = attr.strip().upper()
			if not serverData.has_key(attr):
				serverData[attr] = value.strip()

#	for (key, value) in serverData.items():
#		print "[%s] %s" % (key, value)
#	print "\n"
	return serverData
	
	
def xmlParse(location):
	def findChild(parent, tag, namespace):
		return parent.find('{%s}%s' % (namespace, tag))

	def getAttr(root, parent, tag, namespace):
		try:
			pElem = findChild(root, parent, namespace)
			if pElem is not None:
				child = findChild(pElem, tag, namespace)
				if child is not None:
					return child.text
		except:
			pass

		return None

	def getAttrN2(root, parent, tag, namespace_1 , namespace_2):
		try:
			pElem = findChild(root, parent, namespace_1)
			if pElem is not None:
				child = findChild(pElem, tag, namespace_2)
				if child is not None:
					return child.text
		except:
			pass

		return None

	def dumpData():
		print "\n######## SATIPSERVERDATA ########"
		for (k, v) in SATIPSERVERDATA.items():
#			prestr = "[%s]" % k
			prestr = ""
			for (k2, v2) in v.items():
				prestr2 = prestr + "[%s]" % k2
				if not isinstance(v2, dict):
					print "%s %s" % (prestr2, v2)
					continue
				for (k3, v3) in v2.items():
					prestr3 = prestr2 + "[%s]" % k3
					print "%s %s" % (prestr3, v3)
		print ""
		
	print "[SATIPClient] Parsing %s" % location

	address = ""
	port = "80"
	request = ""

	try:
		location = location.strip().split("http://")[1]
		AAA = location.find(':')
		BBB = location.find('/')
		if AAA == -1:
			address = location[AAA+1 : BBB]
			port = "80"
			request = location[BBB:]
		else:
			address = location[:AAA]
			port = location[AAA+1 : BBB]
			request = location[BBB:]

		#print "address2 : ", address
		#print "port2: " , port
		#print "request : ", request

		conn = httplib.HTTPConnection(address, int(port))
		conn.request("GET", request)
		res = conn.getresponse()
	except Exception, ErrMsg:
		print "http request error %s" % ErrMsg
		return -1

	if res.status != 200 or res.reason !="OK":
		print "response error"
		return -1

	data = res.read()
	conn.close()

	# parseing xml data
	root = xml.etree.cElementTree.fromstring(data)

	xmlns_dev = "urn:schemas-upnp-org:device-1-0"
	xmlns_satip = "urn:ses-com:satip"

	udn = getAttr(root, 'device', 'UDN', xmlns_dev)
	if udn is None:
		return -1;

	uuid = udn.strip('uuid:')
	SATIPSERVERDATA[uuid] = {}

	SATIPSERVERDATA[uuid]['ipaddress'] = address

	pTag = 'device'
	SATIPSERVERDATA[uuid][pTag] = {}
	for tag in DEVICE_ATTR:
		SATIPSERVERDATA[uuid][pTag][tag] = getAttr(root, pTag, tag, xmlns_dev)

	tagList = ['X_SATIPCAP']
	for tag in tagList:
		SATIPSERVERDATA[uuid][pTag][tag] = getAttrN2(root, pTag, tag, xmlns_dev, xmlns_satip)

	pTag = 'specVersion'
	SATIPSERVERDATA[uuid][pTag] = {}
	tagList = ['major', 'minor']
	for tag in tagList:
		SATIPSERVERDATA[uuid][pTag][tag] = getAttr(root, pTag, tag, xmlns_dev)

#	dumpData()

def isEmptyServerData():
	return isEmpty(SATIPSERVERDATA)

def getServerData():
	return SATIPSERVERDATA

def getServerKeys():
	return SATIPSERVERDATA.keys()

def getServerInfo(uuid, attr):
	if attr in ["ipaddress"]:
		return SATIPSERVERDATA[uuid][attr]

	elif attr in DEVICE_ATTR + ['X_SATIPCAP']:
		return SATIPSERVERDATA[uuid]["device"][attr]

	elif attr in ['major', 'minor']:
		return SATIPSERVERDATA[uuid]["specVersion"][attr]
	else:
		return "Unknown"

def getServerDescFromIP(ip):
	for (uuid, data) in SATIPSERVERDATA.items():
		if data.get('ipaddress') == ip:
			return data['device'].get('modelName')
	return 'Unknown'

def getUUIDFromIP(ip):
	for (uuid, data) in SATIPSERVERDATA.items():
		if data.get('ipaddress') == ip:
			return uuid
	return None


	server_choices = []

	server_default = None
	for uuid in getServerKeys():
		description = getServerInfo(uuid, "modelName")
		server_choices.append( (uuid, description) )
#		if vtuner_uuid == uuid:
		server_default = uuid

	if server_default is None:
		server_default = server_choices[0][0]
	
	return server_choices

def loadConfig():
	vtunerConfig = []
	for idx in vtunerIndex:
		vtunerConfig.append({'vtuner_type':"usb_tuner"})

	if os.access(SATIP_CONFFILE, os.R_OK):
		fd = open(SATIP_CONFFILE)
		confData = fd.read()
		fd.close()

		if confData:
			for line in confData.splitlines():
				if len(line) == 0 or line[0] == '#':
					continue


				data = line.split('=')
				if len(data) != 2:
					continue
				idx = data[0]

				try:
					vtunerConfig[int(idx)]
				except:
					continue

				data = data[1].split(',')
				if len(data) != 5:
					continue

				for x in data:
					s = x.split(':')
					if len(s) != 2:
						continue

					attr = s[0]
					value = s[1]
					vtunerConfig[int(idx)][attr] = value

	return vtunerConfig
	
def saveConfig():
	data = ""

	for idx in vtunerIndex:
		conf = vtunerConfig[int(idx)]
		if not conf:
			continue

		print "conf : ", conf

		attr = []
		for k in sorted(conf):
			attr.append("%s:%s" % (k, conf[k]))

		data += idx + '=' + ",".join(attr)+"\n"

	if data:
		fd = open(SATIP_CONFFILE, 'w')
		fd.write(data)
		fd.close()

def sortVtunerConfig():
	vtunerConfig.sort(reverse=True)
	
def addConf():
	z = -1
	for vtuner_idx in vtunerIndex:
		vtuner = vtunerConfig[int(vtuner_idx)]
		if vtuner['vtuner_type'] == "satip_client":
			for uuid in getServerKeys():
				if vtuner['uuid'] == uuid:
				  SATIPSERVERDATA[uuid] = "delete"
		else:
			if z == -1:
				z = int(vtuner_idx)
	for uuid in getServerKeys():
	  if SATIPSERVERDATA[uuid] <> "delete":
			vtunerConfig[z]['tuner_type'] = "DVB-S"
			vtunerConfig[z]['ipaddr'] = getServerInfo(uuid, "ipaddress")
			vtunerConfig[z]['uuid'] = uuid
			vtunerConfig[z]['vtuner_type'] = 'satip_client'
			vtunerConfig[z]['desc'] = getServerInfo(uuid, "modelName")
			z = z + 1
	

vtunerIndex = VTUNER_IDX_LIST
vtunerConfig = loadConfig()
#sortVtunerConfig()
old_vtunerConfig = copy.deepcopy(vtunerConfig)

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.settimeout(5)
sock.sendto(ssdpRequest, (SSDP_ADDR, SSDP_PORT))

while 1:
	try:
		data = sock.recv(1000)
	except:
		break
	serverData = dataParse(data)
	if serverData.has_key('LOCATION'):
		xmlParse(serverData['LOCATION'])

#print getServerKeys()
#for uuid in getServerKeys():
#	print getServerInfo(uuid, "modelName")

addConf()
saveConfig()
