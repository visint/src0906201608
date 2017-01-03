<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<script src="../../js/public.js" type="text/javascript"></script>
<script src="../../js/str.js" type="text/javascript"></script>
<script src="js/dw.js" type="text/javascript"></script>
 <script src="js/unav.js" type="text/javascript"></script>
 <script src="js/olp.js" type="text/javascript"></script>
<link rel="stylesheet" href="css/style.css" type="text/css" />
<link rel="stylesheet" href="css/cview.css" type="text/css" />
<link rel="stylesheet" href="css/button.css" type="text/css"/>
<title>OLP Base Info</title>
</head>
<script language="javascript">
var uclass=0,utype=0;
<%UnitClass(4);%>
window.onload=function(){
}

function toUrl(p)   
{ 
  location.href="/goform/slot?cid=0&uid=" +olpForm.uid.value +"&page=" +p;   
}
function ask()
{
	
}
</script>
<body>
<script language="javascript">
showOlpNav(utype,"bi");
</script>
<div id="content">
<form name="olpForm" method="GET" action="/goform/formUD" onsubmit="return confirm('你真的要修改单元的基本信息?');">
<input type="hidden" id="pid" name="page" value="olpbi">
<input type="hidden" id="cid" name="cid" value="0"/>
<input type="hidden" id="uid" name="uid" value="<%UnitID();%>"/>
<table width="100%" align="center" cellpadding="0" cellspacing="0">
 <tr>
    <td width="100px">
    	<table align="center" cellpadding="0" cellspacing="0" class="ut">
    	<tr>
        <th id="unitid" class="uh1" scope="col"><%UnitID();%></th>
      </tr>
      <tr><td><%UnitView(4);%></td></tr>
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
          <input class="button white medium" type="button" value="刷新" onclick="window.location.href('olpbi.asp');" name="refresh"/><input class="button white medium" type="submit" value="应用" name="apply" />
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
