var firststart=true;
function show(das) {
  if (document.getElementById(das).style.display=='none') {
    document.getElementById(das).style.display='block';
  }
  else {
    document.getElementById(das).style.display='none';
 }
}
function vertAl(){
	var ipadstream = document.getElementById('ipadstream');
	if(!(navigator.userAgent.match(/iPhone/i) || navigator.userAgent.match(/iPod/i) || navigator.userAgent.match(/iPad/i))) {
		ipadstream.style.display = 'none';
	}
  breite = window.innerWidth;
  if (breite <= 1140){
    document.getElementById("eins").style.left = 680;
    document.getElementById("eins").style.top = 140;
    document.getElementById("zwei").style.left = 680;
    document.getElementById("zwei").style.top = 140;
  } else {
    document.getElementById("eins").style.left = 1000;
    document.getElementById("zwei").style.left = 1000;
  }
  if (breite > 1360){
    divPos = ((breite)/2)-657;
    webIFPos = ((breite)/2)-490;
    document.getElementById("eins").style.left = divPos;
    document.getElementById("zwei").style.left = divPos -40;
    document.getElementById("webif").style.left = webIFPos;
  }
}

function ipadStream(){
var tds = document.getElementById('aktserviceframe').contentWindow.document.getElementsByTagName('td');
var streamlink = ((getCookie('tiTan3').length>0)?getCookie('tiTan3'):'goodplayer')+'://'+window.location.href.substring(0,window.location.href.length-1)+':'+((getCookie('tiTan4').length>0)?getCookie('tiTan4'):'22222')+'/'+tds[1].childNodes[3].href.split('&')[1];
window.location = streamlink;
}

function rcopen_virtual() {
  pwin = window.open("rc_virtual.html","_blank","toolbar=0,status=0,scrollbars=0,resizable=0,width=220,height=470,top=50,left=50");
  pwin.blur();
  window.focus();
  javascript:show('eins');
}

function saveLastIframe(actQuery){
	if(firststart){
	 showLastIframe();
   firststart=false;
  }else{
    var temp;
    try{
      if(((temp=actQuery.href.toString()).indexOf('getepg'))<=0 && (temp.indexOf('query?get')>0)){
        var lastQuery = actQuery.href.toString().split("/");
		setCookie('tiTan2',lastQuery[3]);
      }
    }catch(err){

    }
  }
}

function showLastIframe(){
  var temp;
  if((temp=getCookie('tiTan2'))!=null){
  document.getElementById('main').src=temp; }
}
function rcopen_org() {
  pwin = window.open("rc.html","_blank","toolbar=0,status=0,scrollbars=0,resizable=0,width=220,height=670,top=50,left=50");
  pwin.blur();
  window.focus();
  javascript:show('zwei');
}
function rcswitch() {
cookiewert = GetCookie('tiTan');
if(cookiewert == 'VirtualRemote')
	{
	setCookie('tiTan','OriginalRemote');
	javascript:show('eins');
	javascript:show('zwei');
    } else {
		setCookie('tiTan','VirtualRemote');
	javascript:show('zwei');
	javascript:show('eins');
 }
}
function rcshow() {
cookiewert = getCookie('tiTan');
if(cookiewert == 'VirtualRemote')
	{
	javascript:show('eins');
    } else 	{
	javascript:show('zwei');
    }
}
function getCookie(sName)
{
  // cookies are separated by semicolons
  var aCookie = document.cookie.split("; ");
  for (var i=0; i < aCookie.length; i++)
  {
    // a name/value pair (a crumb) is separated by an equal sign
    var aCrumb = aCookie[i].split("=");
    if (sName == aCrumb[0])
      return unescape(aCrumb[1]);
  }
  return null;
}

function setCookie(Name,Value){
	var a = new Date();
	a = new Date(a.getTime() +1000*60*60*24*365);
	document.cookie = Name+'='+Value+'; expires='+a.toGMTString()+';';
}

function streamConfigSubmit(){
		try{
			setCookie('tiTan3',document.getElementById("main").contentWindow.document.streamconfig.player.value);
			setCookie('tiTan4',document.getElementById("main").contentWindow.document.streamconfig.streamport.value);
			alert('Settings successfully saved');
			return true;
		}catch(err){
			alert('Error occured, try again');
			return false;
		}
}

function validateStreamPort(){
	if(document.getElementById("main").contentWindow.document.streamconfig.streamport.value=='' || document.getElementById("main").contentWindow.document.streamconfig.streamport.value.length<=1){
		alert('Streamport is required.');
		document.getElementById("main").contentWindow.document.streamconfig.streamport.value.focus();
		return false;
	}else {
		streamConfigSubmit();
	 	return true;
	}
}


function initializeForm(player,streamport) {
	if(getCookie('tiTan3').length > 0) {
		for(var i = 0; i < player.length; i++)
			if(player.options[i].value == getCookie('tiTan3'))
				player.selectedIndex = i;
	}
	if(getCookie('tiTan4').length > 0)
		streamport.value = getCookie('tiTan4');
}

function hideStreamingConfig(iosstreamconfig){
	if(!(navigator.userAgent.match(/iPhone/i) || navigator.userAgent.match(/iPod/i) || navigator.userAgent.match(/iPad/i))) {
	iosstreamconfig.style.display = 'none';
	}
}