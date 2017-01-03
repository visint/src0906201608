<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<link rel="stylesheet" href="../css/style.css" type="text/css" />
<link rel="stylesheet" href="../css/button.css" type="text/css" />
<title>恢复出厂设置</title>
<!--[if gte IE 7]><!-->
    <link id="artDialog-skin" href="css/green.css" rel="stylesheet" />
<!--<![endif]-->
<!--[if lt IE 7]>
    <link id="artDialog-skin" href="css/default.css" rel="stylesheet" />
<![endif]-->
<link id="artDialog-skin" href="css/dialog.css" rel="stylesheet" />
<script src="js/dialog.js" type="text/javascript"></script>
<style type="text/css">
<!--
body {
	background-color: #C0C0C0;
}
-->
</style></head>

<body>
<script language="javascript" type="text/javascript">
<%UnitClass(0);%>
function chkSelect(sel) 
{ 
   var a;
   for(var i=0;i<17;i++) 
   { 
      a= document.getElementById("u"+i);
      if(a!=null && a.type=="checkbox") 
      { 
      	if (sel==0)
      	  a.checked=false;
      	else if (sel==1)
      		a.checked=true;
      	else
      		a.checked=!a.checked;
      } 
   } 
}

function app(){
 if (confirm("你真的要恢复出厂设置吗?")!=true)
   return false;
 var n,u,c=0;
 for (n=0;n<17;n++)
 {
   u=document.getElementById("u"+n);
   if (u!=null)
   {
   if (u.checked!=true)
      u.value="off";
   else
   {
     u.value="on";
     c=c+1;
   }
 }
 }
 if (c<1)
 {
   alert("没有勾选单元盘!");
   return false;
 }
 u=document.getElementById("u16");
 if (u.checked==true)
 {
 	u=document.getElementById("defaultip");
 	if (u.checked==true)
 	{
 		if (confirm("恢复NMU的默认IP地址:192.168.1.228吗?")==true)
    { 
  	 u.value="on";
    }
 	}
 }
 return true;
}
function check(v)
{
	var p=document.getElementById("dip");
	if (v.checked)
	{
		  p.style.display = "block";
	}
	else
	{ p.style.display = "none";}
}
function showUnit()
{
	var n,count=0;
	document.write("<tr><td><INPUT type='checkbox' id='u0' name='u0' value='on'>机箱</td></tr>");
	for (n=1;n<us.length;n++)
	{
		if (us[n].uclass>0 && us[n].uclass!=55)
		document.write("<td><INPUT type='checkbox' id='u"+n +"' name='u"+n +"' value='on'>单元盘 #" + n+"</td></tr>");
	}
}
</script>
<ul id="nav">
  <li><span>系统管理=></span></li>
  <li><a href="ip.asp">网络配置</a></li>
  <li><a href="restore.asp" class="current">恢复出厂设置</a></li>
  <li><a href="reset.htm">重启</a></li>
  <li><a href="upgrade.htm">NMU升级</a></li>
  <li><a href="uupgrade.htm">单元盘升级</a></li>
  <li><a href="ie_conf.htm">定时刷新</a></li>
  <li><a href="../help/sys.htm" target="new">帮助</a></li>
</ul>
<div id="content">
<center>
<form NAME="default" METHOD="POST" ACTION="/goform/sys" style="margin-top:0;" onsubmit="return app();">
  <INPUT type="hidden" name="act" value="1">
  <!--<INPUT type="hidden" id ="defaultip" name="defaultip" value="no">-->
  <table border="1" align="center" cellpadding="0" cellspacing="0" class="dt" tyle="vertical-align:middle; text-align:center;">
   <tr> <th>请选择需要恢复出厂设置的单元盘 </td></tr>
  <br/>
  <script language="javascript" type="text/javascript">
  	showUnit();
  </script>
  <tr>
    <td><INPUT type="checkbox" id="u16" name="u16" value="on" onclick="check(this);" onchange="check(this);" >NMU<br/>
    <div id="dip"><INPUT type="checkbox" id="defaultip" name="defaultip" value="off">恢复出厂IP</div></td>
  </tr>
</table>
<br/>
<table align="center" cellpadding="0" cellspacing="0">
 <tr><td><input type="button" id="sel" class="button white medium"value="全选" onclick="chkSelect(1);"/></td>
    <td><input type="button" id="nosel" class="button white medium" value="全不选" onclick="chkSelect(0);"/></td>
    <td><input type="button" id="unsel" class="button white medium" value="反选" onclick="chkSelect(2);"/></td>
    <td><input type="submit" value="恢复出厂设置" id="b0" class="button white medium"/></td>
  </tr>
</table>
</body>
</html>
