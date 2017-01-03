<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<link rel="stylesheet" href="css/style.css" type="text/css" />
<link rel="stylesheet" href="css/button.css" type="text/css" />
 <script src="js/msg.js" type="text/javascript"></script>
<title>单元盘软件升级</title>
<style type="text/css">
<!--
body {
	background-color: #C0C0C0;
}
h2 { color:#FFFFFF;font-size:14px;}
-->
</style></head>
<body>
<script language="javascript" type="text/javascript">
<%UnitClass(0);%>
window.onload=function(){
 showWait("ibox",false,"");
}
var se,m=0,h=0,s=0,ss=1;
function showUnit()
{
	var n,count=0;
	for (n=1;n<us.length;n++)
	{
		if (us[n].uclass>0 && us[n].uclass!=55)
		document.write("<tr><td><INPUT type='checkbox' id='u"+n +"' name='u"+n +"' value='on'>单元盘 #" + n+"</td></tr>");
	}
}
function timedCount()  
{
if((ss%100)==0){s+=1;ss=1;}  
if(s>0 && (s%60)==0){m+=1;s=0;}  
if(m>0 && (m%60)==0){h+=1;m=0;}  
t=h+"时"+m+"分"+s+"秒"+ss;
document.getElementById('timer').innerText=t ;
ss+=1;
}  

function chkSelect(sel) 
{ 
   var a= document.getElementsByName("chk");
   for(var i=0;i<a.length;i++) 
   { 
      if(a[i].type=="checkbox") 

      { 
      	if (sel==0)
      	  a[i].checked=false;
      	else if (sel==1)
      		a[i].checked=true;
      	else
      		a[i].checked=!a[i].checked;
      } 
   } 
}

function Apply(flag){
var msg;
if (flag==1)
   msg="你真的要升级?";
else
   return(false);
if (confirm(msg)){
 document.getElementsByName("default_setting")[0].value=flag;
 showWait("ibox",true,"正在升级...<br/>升级过程中请确保网络畅通并停止操作！<br/>升级视网络情况,需要1-5分钟");
 document.setcfg.submit();
}
else
 return(false);
}
function uploadFileSubmit()
{
	if (check()!=true)
	    return false;
  var obj=document.getElementById("fileupload");
  var err="出错提示!\n单元盘升级文件必须为以下格式:\nOTU:otu-XX-XX.bin\nOEO:oeo-XX-XX.bin\nOLP:olp-XX-XX.bin\nOBP:obp-XX-XX.bin\nEDFA:edfa-XX-XX.bin\n"
  if ((obj.value.indexOf(".bin", obj.value.length - ".bin".length) !== -1))
  {
    if (obj.value.indexOf("olp")>-1 || obj.value.indexOf("obp")>-1 ||obj.value.indexOf("otu")>-1 ||obj.value.indexOf("oeo")>-1||obj.value.indexOf("edfa")>-1)
    {
    	 if (confirm("提示:\n1:升级过程中请确保网络畅通并停止操作！\n2:升级视网络情况,需要1-5分钟,升级成功后会有提示!\n你真的要升级单元盘固件吗?\n"))
    	 {
    	 	showWait("ibox",true,"正在升级...<br/>升级过程中请确保网络畅通并停止操作！<br/>升级视网络情况,需要1-5分钟");
    	 	se=setInterval("timedCount()",1); 
    	 	return true;
    	 }
    	 else
    	 	 return false;
    }
    else
    {
       alert(err);
       return false;
    }
  }
  else
  {
    alert(err);
    return false;
  }
}
function check(){
 var n,u,c=0;
 for (n=1;n<maxslot;n++)
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
 return true;
}
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
</script>

<ul id="nav">
  <li><span>系统管理=></span></li>
  <li><a href="ip.asp">网络配置</a></li>
  <li><a href="restore.asp">恢复出厂设置</a></li>
  <li><a href="reset.htm">重启</a></li>
  <li><a href="upgrade.htm">NMU升级</a></li>
  <li><a href="uupgrade.htm" class="current">单元盘升级</a></li>
  <li><a href="ie_conf.htm">定时刷新</a></li>
  <li><a href="../help/sys.htm" target="new">帮助</a></li>
</ul>
<div id="content">
<SPAN style="COLOR: #ff">
<div class="cbox">升级单元盘固件</div>  
<table align="center" cellpadding="0" cellspacing="0">
 <tr><td><input type="button" id="sel" class="button white medium"value="全选" onclick="chkSelect(1);"/></td>
    <td><input type="button" id="nosel" class="button white medium" value="全不选" onclick="chkSelect(0);"/></td>
    <td><input type="button" id="unsel" class="button white medium" value="反选" onclick="chkSelect(2);"/></td>
  </tr>
</table> 
<form id="softupdate" action=/goform/formUpload method=POST enctype="multipart/form-data" onsubmit="return uploadFileSubmit();">
    <table align="center">
    	      <tr><td>请选择需要升级的单元盘:</td></tr>
       <script language="javascript" type="text/javascript">
  	    showUnit();
       </script>
    	<tr> 
       <td>选择升级文件:</td> <td class="button white medium">
       	<input id="fileupload" type="file" name="fileupload" size=20 value=""></td>  
       <td> <input id="fileuploadsubmit" class="button white medium" type="submit" name="update" value="升级"> </td>  
      </tr> 
    </table>  
</form>  
</SPAN>
<div id="ibox" class="ibox" style="height:60px"></div>
<div id ="timer"></div>
</div>
</body>
</html>

