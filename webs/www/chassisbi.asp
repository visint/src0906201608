<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<script src="js/public.js" type="text/javascript"></script>
<script src="js/str.js" type="text/javascript"></script>
<link rel="stylesheet" href="css/style.css" type="text/css" />
<link rel="stylesheet" href="css/uview.css" type="text/css" />
<link rel="stylesheet" href="css/button.css" type="text/css"/>
<title>Chassis Base Info</title>
</head>
<script language="javascript">
window.onload=function(){
//cf_InitUrlParm();
}
function toUrl(p)   
{ 
  //alert(otuForm.uid.value);
  location.href="/unit?+cid=0&uid=" +otuForm.uid.value +"&page=" +p;   
} 
</script>
<body>
<ul id="nav">
  <li><span>机箱信息=></span></li>
  <li><a href="chassisbi.asp" class="current">机箱基本信息</a></li>
  <li><a href="chassis.asp">电源/风扇</a></li>
  <!--<li><a href="logusr.htm">登录用户</a></li>
  <li><a href="../help/sys_info.htm" target="new">帮助</a></li>-->
</ul>
<div id="content">
<form name="otuForm" method="get" action="/goform/formUD" onsubmit="return confirm('你真的要修改机箱的基本信息?');">
<input type="hidden" id="pid" name="page" value="sys/nmubi">
<input type="hidden" id="cid" name="cid" value="0"/>
<input type="hidden" id="uid" name="uid" value="0"/>
<table width="100%" align="center" cellpadding="0" cellspacing="0">
 <tr>
 <td>
 <table border="1" align="center" cellpadding="0" cellspacing="0" class="dt">
    <%UserData(0);%>
   <tr>
   	<td height="29" colspan="4" align="center"><font size="3">
        <input class="button white medium" type="button" value="刷新" onclick="window.location.href('chassisbi.asp');" name="refresh"/>
    </td>
    <!--
   	<td height="29" colspan="2" align="center"><font size="3">
        <input class="button white medium" type="submit" value="应用" name="apply" />
    </td>-->
   </tr>
   </table>
   </td>
  </tr>
</table>
</form>
</div>
</body>
</html>
