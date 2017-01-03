<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<link rel="stylesheet" href="css/style.css" type="text/css" />
<link rel="stylesheet" href="css/button.css" type="text/css" />
<title>数据库维护</title>
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
var dbfile="backup.db",hasBackup=false;
window.onload=function(){
var o=document.getElementById('dbfile');
if (dbfile!="")
   o.innerText=dbfile;
else
	 o.innerText="尚未上传数据库备份文件,请先上传数据库备份文件";
//document.getElementById('logo').src="images/"+logo;
}

function restoreDb(){
if (dbfile=="")
{
	alert("尚未上传数据库备份文件,请先上传数据库备份文件");
	return;
}
if (confirm("你真的要恢复数据库吗?")){
 
}
else
 return(false);
}
function delelteDb()
{
	if (confirm("你真的要删除备份数据库吗?")){
 
 }
}
function loadDb()
{
 if (!hasBackup)
 {
 	 if (confirm("下载之前先备份数据库"))
 	 {
 	 	 window.location.href("/data/vispace.db");
 	 }
 }
}
function uploadFileSubmit()
{
	if (dbfile!="")
	{
	  if (confirm("数据库备份文件已存在,是否继续上传?")!=true)
	    return false;
	 }
  var obj=document.getElementById("fileupload");
  var err="出错提示!\n数据库文件后缀必须为.db的文件\n"
  if ((obj.value.indexOf(".db", obj.value.length - ".db".length) !== -1))
  {
    return confirm("你真的要上传数据库文件吗?");
  }
  else
  {
    alert(err);
    return false;
  }
}

</script>

<ul id="nav">
  <li><span>数据库维护=></span></li>
  <li><a href="uploaddb.htm" class="current">备份/恢复数据库</a></li>
  <li><a href="../help/sys.htm" target="new">帮助</a></li>
</ul>
<div id="content">

<SPAN style="COLOR: #ff">
<div class="cbox">数据库恢复</div>  
<form id="softupdate" action=/goform/formUpload method=POST enctype="multipart/form-data" onsubmit="return uploadFileSubmit();">  
    <table align="center">
    	<tr>
    		<td>数据库备份文件</td><td id="dbfile">vispace.db</td>
    	</tr>
    	<tr> 
       <th>选择需要恢复的数据库文件:</th>
       <td class="button white medium">
       	<input id="fileupload" type="file" name="fileupload" accept=".db" size=20 value=""></td>  
       <td> <input id="fileuploadsubmit" class="button white medium" type="submit" name="update" value="上传并恢复数据库">  </td>  
      </tr>
      <tr>
      <td ><a class="button white medium"  href="/data/vispace.db"">下载数据库</a></td>
      <td><input class="button white medium" type="button" value="恢复数据库" onclick="javascript:restoreDb();"></td>
      <td><input class="button white medium" type="button" value="删除备份数据库文件" onclick="javascript:delelteDb();"></td>
      </tr>
    </table>  
</form>    
</SPAN>
</div>
</body>
</html>


