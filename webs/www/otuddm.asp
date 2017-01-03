<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<script src="js/public.js" type="text/javascript"></script>
<script src="js/dw.js" type="text/javascript"></script>
<script src="js/unav.js" type="text/javascript"></script>
<script src="js/unit.js" type="text/javascript"></script>
<script src="js/otu.js" type="text/javascript"></script>
<link rel="stylesheet" href="css/style.css" type="text/css" />
<link rel="stylesheet" href="css/cview.css" type="text/css" />
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
  location.href="/goform/slot?cid=0&uid=" +otuForm.uid.value +"&page=" +p;   
} 
function showDdmTable()
{
	if (null==p)
	  return;
	var i;
	for (i=0;i<p.length;i++)
	{
		if (p[i]!=null)
		{
		dw(F("<tr><td>{0}</td><td>",i+1));
		if (p[i].rpower>-1000)		
		   dw(F("{0}",(0.1*p[i].rpower).toFixed(1)));
		dw("</td>");
			 
		dw(F("<td>{0}</td><td>{1}</td><td>{2}</td><td>{3}</td></tr>",(0.1*p[i].tpower).toFixed(1),(0.1*p[i].temp).toFixed(1),(0.1*p[i].volt).toFixed(1),p[i].bias));
	  }
	}
}
</script>
<body>
<script language="javascript">
<%DDM(3,1);%>
showOtuNav(uc,ut,"otuddm");
</script>
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
        <th id="unitid" class="uh1"><%UnitID();%></th>
      </tr>
      <tr>
      <td><%UnitView(3);%></td>
      <tr class="uf1">
      <td></td>
      </tr>
     </table>
    </td>
    <td valign="top">
   <table border="1" align="center" cellpadding="0" cellspacing="0" class="dt">
   <thead>
   <tr>
     <th colspan="6">光口 DDM 信息</th>
   </tr>
   </thead>
   <tr class="toptitle">
    <th>光口</th>
    <th>接收光功率(dBm)</th>
    <th>发送光功率(dBm)</th>
    <th>温度℃</th>
    <th>电压(V)</th>
    <th>偏置电流(mA)</th>
  </tr>
    <script language="javascript">
  showDdmTable();
  </script>
   <tr>
   <td colspan="12" align="center"><a class="button white medium" href="otuddm.asp">刷新</a></td>
  </tr>
   </table>   
   </td>
  </tr>
</table>
</form>
</div>
</body>
</html>
