<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<script src="./js/public.js" type="text/javascript"></script>
<script src="./js/str.js" type="text/javascript"></script>
<link rel="stylesheet" href="css/style.css" type="text/css" />
<link rel="stylesheet" href="css/cview.css" type="text/css" />
<link rel="stylesheet" href="css/button.css" type="text/css"/>
<title>ODM信息</title>
</head>
<script language="javascript">
window.onload=function(){
}
function toUrl(p)   
{ 
  location.href="/goform/slot?cid=0&uid=" +<%UnitID();%> +"&page=" +p;   
} 
</script>
<body>
<ul id="nav">
  <li><span>槽位信息=></span></li>
  <li><a href="#" id="base" name="navitem" onclick="javascript:toUrl('odm12bi');">基本信息</a></li>
  <li><a href="#" id="odm" name="navitem" class="current" onclick="javascript:toUrl('odm12');">ODM信息</a></li>
  <!--<li><a href="../help/index.htm" target="new">帮助</a></li>-->
</ul>
<div id="content">
<table width="100%" align="center" cellpadding="0" cellspacing="0">
 <tr>
    <!--<td width="100px">
    	<table align="center" cellpadding="0" cellspacing="0" class="ut">
    	<tr>
        <th id="unitid" class="uh1"><%UnitID();%></th>
      </tr>
      <tr><td><%UnitView();%></td></tr>
      <tr class="uf1">
      <td></td>
      </tr>
    	</table>
    </td>-->
    <td valign="top">
   <table border="1" align="center" cellpadding="0" cellspacing="0" class="dt">
   <thead>
   <tr>
     <th colspan="2">ODM信息</th>
   </tr>
   </thead>
   <%PassiveInfo(2,1,1);%>
   <tr>
   <td align="center" colspan="2">
   <input class="button white medium" type="button" value="刷新" onclick="window.location.href('odm12.asp');" name="refresh"/>
   </td>
   </tr>
   </table>
   </td>
  </tr>
</table>
</div>
</body>
</html>
