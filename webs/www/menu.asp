<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<base target="mainFrame"/>
<title>WebRiver</title>
<style type="text/css">
<!--
.logo {font-size: 12px; font-weight:bold; text-align:center;}
.logo h2{ font-size:16px; color:#FFFFFF;}
h3{ font-size:14px;color:#FFFFFF; text-align:center;}
body {
    background-color: #C0C0C0;
    background-image:url("images/menubg.gif");
    background-repeat:repeat;
}
-->
</style>
</head>

<body>
<style  type="text/css">
a:link {
　　color: black;
　　text-decoration:none;
}
a:visited {color:black; text-decoration: none;}
.logo{margin:5px;}
.logo h2 {margin:0;}
.logo img{margin:5px 0;}
/*左侧菜单的样式*/
#leftmenu ul {
list-style-type:none;
MARGIN: 0px;
PADDING:0px;
BORDER:medium none;
WIDTH:auto;
}
#leftmenu li {BORDER: #9C9A9C 1px solid; MARGIN: 5px; BACKGROUND:#FFF;WIDTH:95%;}

#leftmenu li a {
DISPLAY: block;
PADDING:2px 0px 2px 33px;
BACKGROUND: url("images/thbg2.jpg") transparent repeat 0px 0px; 
TEXT-DECORATION: none;
OVERFLOW: hidden;
}

#leftmenu li a:hover {
COLOR: #FFF; 
BACKGROUND: url("images/thbg1.jpg");
FONT-WEIGHT:bold;
}
</style>
<script language="javascript">
<%Privilege();%>
<%NmuInfo();%>
var displayBar=true;
 function switchBar(obj){
  if (displayBar){
   parent.frame.cols="18,*";
   displayBar=false;
   obj.src="/images/menu_open.png";
   obj.title="打开左边管理菜单";
  }else{
   parent.frame.cols="200,*";
   displayBar=true;
   obj.src="/images/menu_close.png";
   obj.title="关闭左边管理菜单";
  }
 }

</script>
 <table width="100%" height="100%" border="0" cellpadding="0" cellspacing="0">
  <tr>
   <td><img src="/images/menu_close.png" style="cursor:hand" title="关闭左边管理菜单" onClick="switchBar(this)"/></td>
 <td>
<div class="logo" style="padding-left:10px;">
<script language="javascript">
	document.write("<h2>"+webname+"</span></h2>");
	document.write("<img src='images/" +logo + "' width='140' height='84' align='middle' /></div>");

</script>
<div id ="leftmenu">
<ul>
<li><a href="cv.asp" target="topFrame">刷新设备视图</a></li>
<li><a href="sys/sys_info.asp" target="mf">系统信息</a></li>
<li><a href="chassis.asp" target="mf">机箱信息</a></li>
<li><a href="module_list.asp" target="mf">模块列表</a></li>
<li><a href="alarm_list.asp" target="mf">告警管理</a></li>
<script language="javascript">
if (privilege>1)
{
	document.write("<li><a href='snmpconf.asp' target='mf'>SNMP配置</a></li>");
	document.write("<li><a href='udp.asp' target='mf'>UDP配置</a></li>");
	document.write("<li><a href='ip.asp' target='mf'>系统管理</a></li>");
	document.write("<li><a href='um/pw.htm' target='mf'>用户管理</a></li>");
	document.write("<li><a href='optlog.asp' target='mf'>操作日志</a></li>");
	document.write("<li><a href='syslog.asp' target='mf'>系统日志</a></li>");
}
</script>
<!--
<li><a href="snmpconf.asp" target="mf">SNMP配置</a></li>
<li><a href="ip.asp" target="mf">系统管理</a></li>
<li><a href="um/pw.htm" target="mf">用户管理</a></li>
<li><a href="optlog.asp" target="mf">操作日志</a></li>
<li><a href="syslog.asp" target="mf">系统日志</a></li>
-->
<li><a href="help/index.htm" target="new">帮助</a></li>
</ul>
</div>
<h3>
<script language="javascript">
 if (url.length>3)
 {
 	if (url.indexOf("http://")<0)
 	   url="http://" +url;
  document.write("<a href='" + url + "' target='new'>");
  document.write("<span id='vendor'>" + vendorcn +"</span></a>");
}
</script>
</h3>
<h3>Copyright 2013-2018</h3>
</td>
</tr>
 </table>
</body>
</html>
<script ></script>
