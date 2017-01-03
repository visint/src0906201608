<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<script src="../../js/public.js" type="text/javascript"></script>
<script src="../../js/str.js" type="text/javascript"></script>
<link rel="stylesheet" href="css/style.css" type="text/css" />
<link rel="stylesheet" href="css/cview.css" type="text/css" />
<link rel="stylesheet" href="css/button.css" type="text/css"/>
<title>OSS Base Info</title>
</head>
<script language="javascript">
window.onload=function(){
}

function toUrl(p)
{ 
  location.href="/goform/slot?cid=0&uid=" +ossForm.uid.value +"&page=" +p;   
}
</script>
<body>
<ul id="nav">
  <li><span>OSS=></span></li>
  <li><a href="#" id="base" name="navitem" class="current" onclick="javascript:toUrl('ossbi');">基本信息</a></li>
  <li><a href="#" id="oss" name="navitem" onclick="javascript:toUrl('oss');">OSS 通道1信息</a></li>
  <li><a href="#" id="oss1_2" name="navitem" onclick="javascript:toUrl('oss1_2');">OSS 通道2信息</a></li>
  <li><a href="../help/oss.htm" target="new">帮助</a></li>
</ul>
<div id="content">
<form name="ossForm" method="GET" action="/goform/formUD" onsubmit="return checkud();">
<input type="hidden" id="pid" name="page" value="ossbi">
<input type="hidden" id="cid" name="cid" value="0"/>
<input type="hidden" id="uid" name="uid" value="<%UnitID();%>"/>
<table width="100%" align="center" cellpadding="0" cellspacing="0">
 <tr>
    <td width="100px">
    	<table align="center" cellpadding="0" cellspacing="0" class="ut">
    	<tr>
        <th id="unitid" class="uh1" scope="col"><%UnitID();%></th>
      </tr>
      <tr><td><%UnitView(6);%></td></tr>
      <tr class="uf1">
      <td></td>
      </tr>
    	</table>
    </td>
    <td valign="top">
   <table border="1" align="center" cellpadding="0" cellspacing="0" class="dt">
    <%UserData();%>
   <tr>
     <td align="center" colspan="4">
          <input class="button white medium" type="button" value="刷新" onclick="window.location.href('ossbi.asp');" name="refresh"/><input class="button white medium" type="submit" value="应用" name="apply" />
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
