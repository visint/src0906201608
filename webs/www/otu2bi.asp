<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<script src="./js/public.js" type="text/javascript"></script>
<script src="./js/str.js" type="text/javascript"></script>
<link rel="stylesheet" href="css/style.css" type="text/css" />
<link rel="stylesheet" href="css/cview.css" type="text/css" />
<link rel="stylesheet" href="css/button.css" type="text/css"/>
<title>OTU Base Info</title>
</head>
<script language="javascript">
window.onload=function(){
}
function toUrl(p)   
{ 
  location.href="/goform/slot?cid=0&uid=" +otu2Form.uid.value +"&page=" +p;   
} 
</script>
<body>
<ul id="nav">
  <li><span>OTU(2.5G)=></span></li>
  <li><a href="#" id="base" name="navitem"  class="current" onclick="javascript:toUrl('otu2bi');">基本信息</a></li>
  <li><a href="#" id="otu2" name="navitem" onclick="javascript:toUrl('otu2');">OTU 信息</a></li>
  <li><a href="#" id="otu2ca" name="navitem" onclick="javascript:toUrl('otu2ca');">交叉矩阵</a></li>
  <li><a href="#" id="ddm" name="navitem" onclick="javascript:toUrl('otu2ddm');">DDM 信息</a></li>
 <!--<li><a href="#" id="ddmt" name="navitem" onclick="javascript:toUrl('otu2ddmt');">DDM 告警阈值</a></li>-->
  <li><a href="../help/otu2.htm" target="new">帮助</a></li>
</ul>
<div id="content">
<form name="otu2Form" method="GET" action="/goform/formUD" onsubmit="return checkud();">
<input type="hidden" id="pid" name="page" value="otu2bi"/>
<input type="hidden" id="cid" name="cid" value="0"/>
<input type="hidden" id="uid" name="uid" value="<%UnitID();%>"/>
<table width="100%" align="center" cellpadding="0" cellspacing="0">
 <tr>
    <td width="100px">
    	<table align="center" cellpadding="0" cellspacing="0" class="ut">
    	<tr>
        <th id="unitid" class="uh1"><%UnitID();%></th>
      </tr>
      <tr>
      <td><%UnitView(3);%></td>
      </tr>
      <tr class="uf1">
      <td></td>
      </tr>
    	</table>
    </td>
    <td valign="top">
   <table border="1" align="center" cellpadding="0" cellspacing="0" class="dt">
     <thead>
   <tr>
     <th colspan="4">OTU单元盘基本信息</th>
   </tr>
   </thead>
    <%UserData();%>
   <tr>
   <td colspan="4" align="center">
    <input class="button white medium" type="button" value="刷新" onclick="window.location.href('otu2bi.asp');" name="refresh"/>
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
