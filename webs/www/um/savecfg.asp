<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<link rel="stylesheet" href="../css/style.css" type="text/css" />
<link rel="stylesheet" href="../css/button.css" type="text/css" />
<title>Save the User Configuration</title>
<% language=javascript %>
<script type="text/javascript">
function check(flag){
 if (flag==1)
   document.savecfg.ok.value="OK";
 savecfg.submit();
}
</script>
</head>

<body>
<ul id="nav">
  <li><span>用户管理=></span></li>
  <!--<li><a href="ugroup.asp">用户组</a></li>-->
  <li><a href="user.asp">用户列表 </a></li>
  <li><a href="adduser.asp">添加用户</a></li>
  <li><a href="pw.htm">修改密码</a></li>
  <li><a href="limit.asp">访问权限</a></li>
  <li><a href="savecfg.asp" class="current">保存</a></li>
  <li><a href="../help/user.htm" target="new">帮助</a></li>
</ul>
<div id="content" align="center">
<h3>保存用户配置</h3>
<form name="savecfg" action=/goform/SaveUserManagement method=POST>

<table>
<tr>
   <td></td>
   <td ALIGN="CENTER"> 
   <input type="hidden" name="ok" value="">
   <input class="button white medium bw" type=button value="提交" onClick="return check(1);">
   <input class="button white medium bw" type=button value="取消" onClick="return check(2);">
   </td>
</tr>
</table>
</form>
</div>
</body>
</html>
