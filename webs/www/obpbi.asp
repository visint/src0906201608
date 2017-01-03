<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<script src="./js/public.js" type="text/javascript"></script>
<script src="./js/str.js" type="text/javascript"></script>
<link rel="stylesheet" href="css/style.css" type="text/css" />
<link rel="stylesheet" href="css/cview.css" type="text/css" />
<link rel="stylesheet" href="css/button.css" type="text/css"/>
<title>OBP 基本信息</title>
</head>
<script language="javascript">
window.onload=function(){
}
function toUrl(p)   
{ 
  location.href="/goform/slot?cid=0&uid=" +obpForm.uid.value +"&page=" +p;   
} 
</script>
<body>
<ul id="nav">
  <li><span>OBP=></span></li>
  <li><a href="#" id="base" name="navitem" class="current" onclick="javascript:toUrl('obpbi');">基本信息</a></li>
  <li><a href="#" id="obp" name="navitem" onclick="javascript:toUrl('obp');">OBP 状态信息</a></li>
  <!--<li><a href="#" id="obpt" name="navitem" onclick="javascript:toUrl('obpt');">OBP 告警阈值</a></li>-->
  <li><a href="../help/obp.htm" target="new">帮助</a></li>
</ul>
<div id="content">
<form name="obpForm" method="GET" action="/goform/formUD" onsubmit="return confirm('你真的要修改OBP的基本信息?');">
<input type="hidden" id="pid" name="page" value="obpbi"/>
<input type="hidden" id="cid" name="cid" value="0"/>
<input type="hidden" id="uid" name="uid" value="<%UnitID();%>"/>
<table width="100%" align="center" cellpadding="0" cellspacing="0">
 <tr>
    <td valign="top">
   <table border="1" align="center" cellpadding="0" cellspacing="0" class="dt">
     <thead>
   <tr>
     <th colspan="4">OBP单元盘基本信息</th>
   </tr>
   </thead>
    <%UserData();%>
   <tr>
   <td colspan="4" align="center">
    <input class="button white medium" type="button" value="刷新" onclick="window.location.href('obpbi.asp');" name="refresh"/>
    <input class="button white medium" type="submit" value="应用" name="apply" />
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
