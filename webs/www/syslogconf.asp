<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<script src="../js/checknum.js" type="text/javascript"></script>
<link rel="stylesheet" href="css/style.css" type="text/css" />
<link rel="stylesheet" href="css/button.css" type="text/css"/>
<title>系统日志配置</title>
</head>
<body>
	<style type="text/css">
td {text-align:left;}
img {border-width:0px 0px 0px 0px;}
</style>
<script language="javascript">
<%syslog(1);%>
function submit()   
{ 
  var n;
  var url="/goform/syslogman?opt=conf&maxcount=";
  var unit;
  
  if (document.getElementById("issave").checked)
  {
   url+=document.getElementById("max_count").value;
  }
  else
  	    url+="0";
  	    
 //window.location.href(url);
 //alert(url);
 window.location.href(url);
 return false;
}

function check(flag) 
{

 if (document.getElementById("issave").checked)
     document.getElementById("max_count").disabled=false;
 else
 	   document.getElementById("max_count").disabled=true;
}
window.onload=function(){
if (max_count!=0)
{
	document.getElementById("issave").checked=true;
	document.getElementById("max_count").value=max_count;
}
else
	document.getElementById("max_count").disabled=true;

}
</script>

<ul id="nav">
 <li><span>系统日志管理=></span></li>
  <li><a href="syslog.asp">系统日志</a></li>
  <li><a href="syslogconf.asp" class="current">系统日志配置</a></li>
</ul>
<div id="content" align="center">
 <input type="hidden" name="operate" value="mod"/>
 <table  border="1" cellpadding="0" cellspacing="0" class="dt">
  <caption>系统日志配置</caption>
  <tr><th>是否保存日志</th><td><input type='checkbox' id='issave' name='issave' onclick='check(0);'/></td><td><input type='textbox' id ='max_count' name='max_count' value=300 maxlength='3' size='3' onblur='checkNumber(this,10,1000,"条数");' size='3'/>条</td></tr>
 <!--<%AlarmConf();%>-->
</table>
<div align="center">
<a class="button white medium" href="#" onclick="return submit();">应用</a>
<a class="button white medium" href="syslogconf.asp">刷新</a>
</div>
</div>
</body>
</html>
