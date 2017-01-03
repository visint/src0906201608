<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<script src="../js/public.js" type="text/javascript"></script>
<script src="../js/str.js" type="text/javascript"></script>
<script src="../js/cookie.js" type="text/javascript"></script>
<script src="../js/checknum.js" type="text/javascript"></script>
<link rel="stylesheet" href="../css/style.css" type="text/css" />
<link rel="stylesheet" href="../css/uview.css" type="text/css" />
<link rel="stylesheet" href="../css/button.css" type="text/css"/>
<title>系统配置</title>
</head>
<body>
<script language="javascript">
<%SysConfInfo();%>

function check()
{
	var obj=document.getElementById("printErr");
	if (obj.checked)
	   obj.value="Y";
	else
		  obj.value="N";
		  
  obj=document.getElementById("saveIoErr");
	if (obj.checked)
	   obj.value="Y";
	else
		  obj.value="N";
	//alert(document.getElementById("usrid").value.length);
	if (document.getElementById("usrid").value.length<1 || document.getElementById("pw").value.length<1)
	{ 
		 alert("用户名或密码不能为空!");
		 return false;
	}
	return true;
}

function selectSlot(chassis)
{

 var obj = document.getElementById('slot');
 var length = obj.length;
 for (var j=length-1; j>=0; j--)
 {
  obj.options.remove(j);
 }
 
 obj.options.add( new Option("自动识别", "0"));
 if (chassis==1)
 {
 	 obj.options.add( new Option("第2槽", "2"));
 	 obj.options.add( new Option("第3槽", "3"));
 }
 else if (chassis==2)
 {
 	 obj.options.add( new Option("第6槽", "6"));
 	 obj.options.add( new Option("第7槽", "7"));
 }
 else if (chassis==4)
 {
 	obj.options.add( new Option("第15槽", "15"));
 	obj.options.add( new Option("第16槽", "16"));
 }
}
function onDebugChange(val)
{
	var obj=document.getElementById("printErr");

	if (val=="Y")
	  obj.disabled=false;
	else
		 obj.disabled=true;
	document.getElementById("printSlot").disabled=obj.disabled;
	document.getElementById("saveIoErr").disabled=obj.disabled;
	//document.getElementById("saveErr").disabled=obj.disabled;
	document.getElementById("maxErrors").disabled=obj.disabled;
	document.getElementById("isSaveErr").disabled=obj.disabled;
}
function onCheckChanged(obj)
{
	if (obj.checked)
	  obj.value="Y";
	else
		obj.value="N";
	//alert(obj.value);
	
}

function onCheckSaveError(obj)
{
	//checkNumber(obj,0,2000,"保存出错明细信息条数");
	onCheckChanged(obj);
	if (obj.checked)
	   document.getElementById("maxErrors").disabled=false;
	else
		  document.getElementById("maxErrors").disabled=true;
}

function setCookie()
{
    var u=document.getElementById("usrid");
    var p=document.getElementById("pw");
    cf_SetCookie("username",u.value,60);
    cf_SetCookie("password",p.value,60);
    return true;
}
function clearCookie()
{
  document.getElementById("username").value="";
  document.getElementById("password").value="";
  setCookie();
}
window.onload=function(){
//expire
var u=cf_GetCookie("username");
var p=cf_GetCookie("password");
	
if (u!=null)
  document.getElementById("usrid").value=u;
if (p!=null)
  document.getElementById("pw").value=p;
  
var obj;
obj=document.getElementById("debug");
 for(n=0;n<obj.options.length;n++)
 {
 	  if (obj.options[n].value==debug)
 	  {
 	  	obj[n].selected=true;
 	  	break;
 	  }
 }

 obj=document.getElementById("chassis");
 for(n=0;n<obj.options.length;n++)
 {
 	  if (obj.options[n].value==chassis)
 	  {
 	  	obj[n].selected=true;
 	  	selectSlot(chassis);
 	  	break;
 	  }
 }
 obj=document.getElementById("slot");
 for(n=0;n<obj.options.length;n++)
 {
 	  if (obj.options[n].value==slot)
 	  {
 	  	obj[n].selected=true;
 	  	break;
 	  }
 }
 obj=document.getElementById("printSlot");
 obj.options.add( new Option("不打印", 20));
 if (20==printSlot)
 	 obj[0].selected=true;
 for(n=0;n<16;n++)
 {
 	  if (n>0)
 	      obj.options.add( new Option("第" + n + "槽", n));
 	    else
 	    	obj.options.add( new Option("背板", 0));
 	  if (n==printSlot)
 	  {
 	  	obj[n+1].selected=true;
 	  }
 }
 if (debug=="Y")
 {
 	  if (printErr=="Y")
 	  {
 	  	obj=document.getElementById("printErr");
 	  	obj.checked=true;
 	  }
 	  if (saveIoErr=="Y")
 	  {
 	    obj=document.getElementById("saveIoErr");
 	    obj.checked=true;
 	  }
 }
 document.getElementById("maxErrors").value=maxErrors;
 if (maxErrors>0)
 {
 	   // document.getElementById("maxErrors").disabled=false;
 	   // document.getElementById("isSaveErr").disabled=false;
 	    document.getElementById("isSaveErr").checked=true;
 }
 
 else
 {
 	 document.getElementById("maxErrors").disabled=true;
 	 document.getElementById("isSaveErr").disabled=true;
 }
 onDebugChange(debug);
}
</script>
<ul id="nav">
  <li><span>系统配置=></span></li>
  <li><a href="sysconf.asp" class="current">系统配置</a></li>
</ul>
<div id="content">
<form name="sysForm" method="post" action="/goform/sysconf"  onSubmit="return check();">
<input type="hidden" id="pid" name="page" value="/admin/default">
<input type="hidden" id="cid" name="cid" value="0"/>
<input type="hidden" id="uid" name="uid" value="16"/>
<input type="hidden" id="act" name="act" value="4"/>
<table width="100%" align="center" cellpadding="0" cellspacing="0">
 <tr>
 <td>
 <table border="1" align="center" cellpadding="0" cellspacing="0" class="dt">
 <tr>
 <th>机箱类型</th>
 <td>
  <select id="chassis" name="chassis" onchange="selectSlot(this.value);">
  <option value="0">自动识别</option>
  <option value="1">1U 机箱</option>
  <option value="2">2U 机箱</option>
  <option value="4">4U 机箱</option>
 </select>
 </td>
</tr>
 <tr>
<th>网管卡所在槽位号</th>
<td>
  <select id="slot" name="slot">

 </select>
 </td>
</tr>
 <tr>
 <th>调试模式</th>
 <td>
  <select id="debug" name="debug" onchange="onDebugChange(this.value);">
  <option value="Y">调试模式</option>
  <option value="N">非调试模式</option>
 </select>
 </td>
</tr>
 <tr>
 <th>打印出错信息</th>
 <td>
  <input type="checkbox" id="printErr"  name="printErr" value="N" onchange="onCheckChanged(this);" />打印
 </td>
</tr>
 <tr>
 <th>打印指定槽号信息</th>
 <td>
  <select id="printSlot" name="printSlot">

 </select>
 </td>
</tr>
 <tr>
 <th>保存IO出错信息</th>
 <td>
  <input type="checkbox" id="saveIoErr" name="saveIoErr" value="N" onchange="onCheckChanged(this);" />保存(注:系统日志设置为保存,该项才有效)
 </td>
</tr>
<tr>
 <th><input type="checkbox" id="isSaveErr" name="isSaveErr" value="N" onchange="onCheckSaveError(this);" />保存出错明细信息条数</th>
 <td>
  <input type='textbox' id='maxErrors' name='maxErrors' value=300 maxlength='4' size='4' onblur='checkNumber(this,0,2000,"保存出错明细信息条数");'/>条
 </td>
</tr>
 <tr> <th>用户名</th>
  <td><input type="text" id="usrid" name="user" size='20' maxlength='20' value=""/></td>
</tr>
 <tr>
 	<th>密码</th>
  <td>
   <input type="password" id="pw" name="pw" size='20' maxlength='20' value="" />
   <input type="button" value="保存" name="apply" onClick="setCookie();"/>
   <input type="button" value="清除" name="clear" onClick="clearCookie();"/>
  </td>
  </tr>
<tr>
 <td height="29" colspan="2" align="center">
    <input class="button white medium" type="button" value="刷新" onclick="window.location.href('sysconf.asp');"/>
    <input class="button white medium" type="submit" value="应用" name="apply"  />
   </td>
  </tr>
  <tr>
 <td height="29" colspan="2" align="center">
   注意:配置后要重启系统才生效
   </td>
  </tr>
  </table>
 </td>
  </tr>
</table>
   </td>
  </tr>
</table>
</form>
</div>
</body>
</html>