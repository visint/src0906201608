<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<script src="./js/public.js" type="text/javascript"></script>
<script src="./js/str.js" type="text/javascript"></script>
 <script src="js/dw.js" type="text/javascript"></script>
 <script src="js/unav.js" type="text/javascript"></script>
 <script src="js/unit.js" type="text/javascript"></script>
 <script src="js/edit.js" type="text/javascript"></script>
 <script src="js/otu.js" type="text/javascript"></script>
    
<link rel="stylesheet" href="css/style.css" type="text/css" />
<link rel="stylesheet" href="css/cview.css" type="text/css" />
<link rel="stylesheet" href="css/button.css" type="text/css"/>
<title>OTU Base Info</title>
</head>
<script language="javascript">
//var ul=<%UserLevel();%>;
<%Privilege();%>

window.onload=function(){
 if (privilege==1)
 {
  disableAllControls(true);
 }
}
function toUrl(p)   
{ 
	alert("/goform/slot?cid=0&uid=" +otuForm.uid.value +"&page=" +p);
  location.href="/goform/slot?cid=0&uid=" +otuForm.uid.value +"&page=" +p;   
}
<%UnitInfo(3,0,1,0);%>
</script>
<style type="text/css">
/*[disabled],input[type=text]:not([disabled]),input[disabled],input:disabled {background-color:#C0C0C0;}*/
</style>
<body>
<script language="javascript">
showOtuNav(uc,ut,"bi");
</script>
<div id="content">
<form name="otuForm" method="GET" action="/goform/formUD" onsubmit="return confirm('你真的要修改板卡的基本信息?');">
<input type="hidden" id="pid" name="page" value="otubi"/>
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
    <%UserData(1);%>
   <tr>
   <td colspan="4" align="center">
    <button class="button white medium" type="button"  onclick="window.location.href('otubi.asp');" name="refresh">刷新</button> 
    <input class="button white medium" type="submit" value="应用" id="apply" name="apply" />
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
