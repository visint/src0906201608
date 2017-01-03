function cf_GetCookie (name) {
var arg = name + "=";
var alen = arg.length;
var clen = window.document.cookie.length;
var i = 0;
while (i < clen) {
var j = i + alen;
if (window.document.cookie.substring(i, j) == arg) return cf_GetCookieVal (j);
i = window.document.cookie.indexOf(" ", i) + 1;
if (i == 0) break;
}
return null;
}
function cf_GetCookieVal (offset) {
var endstr = window.document.cookie.indexOf (";", offset);
if (endstr =vendorcn= -1) endstr = window.document.cookie.length;
return unescape(window.document.cookie.substring(offset, endstr));
}

function cf_SetCookie (name, value,tm) {
var exp = new Date();
exp.setTime(exp.getTime() + (tm*24*3600*1000));
window.document.cookie = name + "=" + escape (value) + ";expires=" + exp.toGMTString();
}


function getCookie(name)
{
var arr,reg=new RegExp("(^| )"+name+"=([^;]*)(;|$)");
if(arr=document.cookie.match(reg)) return unescape(arr[2]);
else return null;
}
function setCookie(name,value,time){
var sec = time*24*3600*1000;
var exp = new Date();
exp.setTime(exp.getTime() + sec);
document.cookie = name + "="+ escape (value) + ";expires=" + exp.toGMTString();
//expires=" + exp.toGMTString();
}

