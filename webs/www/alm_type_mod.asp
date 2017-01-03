<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<link rel="stylesheet" href="css/style.css" type="text/css" />
<link rel="stylesheet" href="css/button.css" type="text/css"/>
<title>告警策略管理</title>
</head>
<body>
<style type="text/css">
td {text-align:left;}
img {border-width:0px 0px 0px 0px;}
</style>
<script language="javascript">
	<%AlarmType();%>
function submit()   
{ 
  var n;
  var url="/goform/atype?operate=mod";
  var unit;

  url+="&ac="+alarm_code;
  url+="&severity="+document.getElementById("severity").value;
  if (document.getElementById("enable").checked)
     url+="&enable=on";
  else
     url+="&enable=off";
  if (document.getElementById("save").checked)
     url+="&save=on";
  else
     url+="&save=off";
  url+="&slots=";
  for (n=1;n<16;n++){
  	if (n<10)
     unit=document.getElementById("u0"+n);
    else
    	unit=document.getElementById("u"+n);
    if (unit!=null && unit.checked==true)
    {
    	if (n<10)
    	  url+="0";
    	url+=n+";";
    }
  }
//  alert(url);
 window.location.href(url);
 return false;
}
function checkEnable() 
{
 
  if (document.getElementById("enable").checked!=true && document.getElementById("save").checked!=true)
  {
  	var unit,slot;
   for(var n=1;n<16;n++)
   {
    if (n<10)
      slot="u0"+n;
    else
   	  slot="u"+n;
    unit=document.getElementById(slot);
    if (unit!=null)
    	 unit.checked=false;
   } 
  }
}
window.onload=function(){
 //var ss=s.split(";");
 var slots;
 var unit;
 slots=mask_slots.split(";");
 //alert(slots);
for(var i=0;i<slots.length && i<16;i++){
 // alert(slots[i]);
  if (slots[i].length>1)
  {
  	//alert(slots[i]);
  unit=document.getElementById("u"+slots[i]);
  if (unit!=null)
  {
  	  unit.checked=true;
  	  //alert("u"+slots[i]);
  }
}
}
//checkEnable();
}
</script>

<ul id="nav">
 <li><span>告警管理=></span></li>
  <li><a href="alarm_list.asp">当前告警</a></li>
  <li><a href="alarm_his.asp">历史告警</a></li>
  <li><a href="alm_type.asp" class="current">告警策略</a></li>
  <li><a href="almconf.asp">告警配置</a></li>
  <li><a href="help/alarm.htm" target="new">帮助</a></li>
</ul>
<div id="content" align="center">
 <input type="hidden" name="operate" value="mod"/>
 <table  border="1" cellpadding="0" cellspacing="0" class="dt">
  <caption>告警过滤配置</caption>
  <!--<%AlarmType();%>-->
  <script language="javascript">
   var s=["紧急告警", "主要告警","次要告警" ,"提示告警" ,"清除告警"];
   document.write("<tr><td>告警代码</td><td>"+alarm_code+"</td></tr>");
   document.write("<tr><td>告警信息</td><td>"+alarm_info+"</td></tr>");
   document.write("<tr><td>告警级别</td><td><select id=severity>");
   for (n=0;n<5;n++)
   {
   	 document.write("<option value=" + (n+1));
   	 if (severity==n+1)
   	   document.write(" selected ");
   	 document.write(">"+s[n]);
   }
   document.write("</select></td></tr>");
   document.write("<tr><td>是否发送?</td><td><input type=checkbox id=enable title='是否发送该类告警'");
   if (is_send==1)
      document.write(" checked=true ");
   document.write(" onclick='checkEnable()';/></td></tr>");
   
   document.write("<tr><td>是否保存?</td><td><input type=checkbox id=save title='是否是否保存该类告警'");
   if (is_save==1)
      document.write(" checked=true ");
   document.write(" onclick='checkEnable()';/></td></tr> <tr><th>屏蔽槽位</th><td>");
   var uid;
   for (n=1;n<max_slot;n++)
   {
   	if (n<10)
   	   uid="0"+n;
   	else
   		 uid=n;
   	document.write("<INPUT type='checkbox' id='u" +uid +"' name='chk' value='on'>#" +n);
   }
   document.write("</td></tr>");
  </script>
  <!--
  <tr>
   <th>屏蔽槽位:</th>
   <td>
      <INPUT type='checkbox' id='u01' name='chk' value='on'>#1
      <INPUT type='checkbox' id='u02' name='chk' value='on'>#2
      <INPUT type='checkbox' id='u03' name='chk' value='on'>#3
      <INPUT type='checkbox' id='u04' name='chk' value='on'>#4
      <INPUT type='checkbox' id='u05' name='chk' value='on'>#5
      <INPUT type='checkbox' id='u06' name='chk' value='on'>#6
      <INPUT type='checkbox' id='u07' name='chk' value='on'>#7
      <INPUT type='checkbox' id='u08' name='chk' value='on'>#8
      <INPUT type='checkbox' id='u09' name='chk' value='on'>#9
      <INPUT type='checkbox' id='u10' name='chk' value='on'>#10
      <INPUT type='checkbox' id='u11' name='chk' value='on'>#11
      <INPUT type='checkbox' id='u12' name='chk' value='on'>#12
      <INPUT type='checkbox' id='u13' name='chk' value='on'>#13
      <INPUT type='checkbox' id='u14' name='chk' value='on'>#14
      <INPUT type='checkbox' id='u15' name='chk' value='on'>#15
   </td>
  <tr>
  -->
</table>
<div align="center">
<a class="button white medium" href="#" onclick="return submit();">应用</a>
<a class="button white medium" href="alm_type.asp">返回</a>
</div>
  </div>
</div>
</body>
</html>
