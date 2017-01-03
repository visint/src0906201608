<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8"/>
<script src="../js/edit.js" type="text/javascript"></script>
<script src="../js/checknum.js" type="text/javascript"></script>
<script src="../js/validate.js" type="text/javascript"></script>
<script src="../js/math.js" type="text/javascript"></script>
<link rel="stylesheet" href="../css/style.css" type="text/css" />
<link rel="stylesheet" href="../css/button.css" type="text/css" />
<title>UDP配置</title>
</head>
<style type="text/css">
.c td{padding:3px;}
</style>
<script language="javascript">
<%Privilege();%>
<%UnitClass(0);%>
<%UdpConf();%>
window.onload=function(){
	  
	  if (privilege<2)
	    disableInputControls(privilege,true);
}
String.prototype.trim = function()  
{  
   return this.replace(/(^\s*)|(\s*$)/g, "");  
}

function check()
{
  var obj;  

  if (confirm("你真的要设置UDP的参数吗?")!=true)
   return false;
  obj=document.getElementById("enabled");
  //alert(obj.value);
  //alert(obj.checked);
  if (obj.checked!=true)
      obj.value=2;
  else
  	  obj.value=1;
  /*obj=document.getElementById("enabled");
  
  if (obj.checked!=true)
    obj.value=2;
  var en=document.getElementById("enable");
  */
  return true;
}

function showUdpConf()
{

	document.write("<tr><td>是否启用</td><td><input type='checkbox' name='enabled' id='enabled' value='1'");
	if (enabled==1)
	  document.write(" checked=true");
	document.write("/>启用</td></tr>");

	document.write("<tr><td>启动状态</td><td>");
	if (is_start!=1)
	  document.write("未");
	else
		document.write("已");
	document.write("启动</td></tr>");
	document.write("<tr><td>监听端口</td><td><input type='text' id='port' name='port' maxlength='6' size='6' value="+port +" onkeyup='uIntInput(this);'></td></tr>");

	document.write("<tr><td>认证码</td><td><input type='text' id='acp_msg' name='acp_msg' maxlength='48' size='48' value="+acp_msg +"></td></tr>");
	   
	document.write("<tr><td>说明</td><td>认证码最多8个字符</br></td></tr>");
}
</script>
<body>
<ul id="nav">
  <li><span>UDP配置=></span></li>
  <li><a href="udp.asp" class="current">UDP Server配置</a></li>
</ul>
<div id="content">
<form name="genForm" method="post" action="/goform/SetUdpConf" onsubmit="return check();">
<input type="hidden" id="pid" name="current_page" value="/sys/heartbeat.asp">
<input type="hidden" id="slots" name="slots" value=""/> 
<table border="1" align="center" cellpadding="0" cellspacing="0" class="dt">
  <caption>
    UDP Server配置
  </caption>
  <script language="javascript">
 showUdpConf();
  </script>
</table>
<table align="center">
<tr>
 <td>
  <input class="button white medium" type="button" value="刷新" onclick="window.location.href('udp.asp');" name="refresh"/>
 </td>
 <td>
      <input class="button white medium" type="submit" value="应用" name="apply"/>
 </td>
 </tr>	
</table>
</form>
</div>
</body>
</html>
