<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<script src="js/public.js" type="text/javascript"></script>
<link rel="stylesheet" href="css/style.css" type="text/css" />
<link rel="stylesheet" href="css/uview.css" type="text/css" />
<link rel="stylesheet" href="css/button.css" type="text/css"/>
<title>DDM 信息</title>
</head>
<script language="javascript">
window.onload=function(){
//cf_InitUrlParm();
}
function toUrl(p)   
{ 
  //alert(otuForm.uid.value);
  location.href="/unit?+cid=0&uid=" +otuForm.uid.value +"&page=" +p;   
} 
</script>
<body>
<ul id="nav">
  <li><span>OTU=></span></li>
  <li><a href="#" id="base" name="navitem" onclick="javascript:toUrl('otubi');">基本信息</a></li>
  <li><a href="#" id="otu" name="navitem" onclick="javascript:toUrl('otu');">OTU 信息</a></li>
  <li><a href="#" id="ddm" name="navitem" class="current" onclick="javascript:toUrl('otuddm');">DDM 信息</a></li>
  <li><a href="../help/otu.htm" target="new">帮助</a></li>
</ul>
<div id="content">
<form name="otuForm" method="post" action="otu.asp" style="margin-top:0;>
<input type="hidden" id="pid" name="current_page" value="otu.asp">
<input type="hidden" id="cid" name="cid" value="1"/>
<input type="hidden" id="uid" name="uid" value="<%UnitID();%>"/>
<table width="100%" align="center" cellpadding="0" cellspacing="0">
    <tr>
    <td width="100px">
     <table align="center" cellpadding="0" cellspacing="0" class="ut">
      <tr>
       <th scope="col" id="unitid" class="uh"><%UnitID();%></th>
      </tr>
      <tr>
    	<td>OTU</td>
      </tr>
        <%UnitView(3);%>
      <tr class="ft">
      <td></td>
      </tr>
     </table>
    </td>
    <td>
   <table border="1" align="center" cellpadding="0" cellspacing="0" class="dt">
   <thead>
   <tr>
     <th colspan="6">光口 2  DDM 信息</th>
   </tr>
   </thead>
   <%DDM(3);%>
   </table>   
   </td>
  </tr>
</table>
<div align="center">
<a class="button gray medium" href="otuddm.asp">刷新</a>
</div>
</form>
</div>
</body>
</html>
