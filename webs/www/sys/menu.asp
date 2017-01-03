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
<%NmuInfo();%>
window.onload=function(){
//document.getElementById('webname').href=webname;
//document.getElementById('logo').src="images/"+logo;
}
</script>
<div class="logo">
<script language="javascript">
	document.write("<h2>"+webname+"</span></h2>");
	document.write("<img src='images/" +logo + "' width='140' height='84' align='middle' /></div>");
/*
<h2><span id="webname">WebRiver 1.1.1</span></h2>
<img id="logo" src="" width="140" height="84" align="middle" /></div>
*/
</script>
<div id ="leftmenu">
<ul>
<li><a href="cv.asp" target="topFrame">Refresh View</a></li>
<li><a href="sys/sys_info.asp" target="mf">System Info</a></li>
<li><a href="module_list.asp" target="mf">Module List</a></li>
<li><a href="alarm_list.asp" target="mf">Alarm Management</a></li>
<li><a href="snmpconf.asp" target="mf">SNMP Configure</a></li>
<li><a href="ip.asp" target="mf">System Management</a></li>
<li><a href="um/pw.htm" target="mf">User Management</a></li>
<!--<li><a href="optlog.asp" target="mf">Log Management</a></li>
<li><a href="help/index.htm" target="new">帮助</a></li>-->
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
<h3>Copyright 2008-2013</h3>
</body>
</html>
<script ></script>
