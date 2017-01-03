<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8"/>
<script src="../js/datetime.js" type="text/javascript"></script>
<script src="../js/edit.js" type="text/javascript"></script>
<script language="javascript" type="text/javascript" src="../My97DatePicker/WdatePicker.js"></script>
<script src="../js/checknum.js" type="text/javascript"></script>
<script src="../js/validate.js" type="text/javascript"></script>
<script src="../js/math.js" type="text/javascript"></script>
<link rel="stylesheet" href="../css/style.css" type="text/css" />
<link rel="stylesheet" href="../css/button.css" type="text/css" />
<title>System Info</title>
</head>

<body>
<script language="javascript">
<%Privilege();%>
<%UnitInfo(1,1,1,1);%>
function check()
{
  var minobj,maxobj;
  var m1="最低温度",m2="最高温度";
  minobj=document.getElementById("mintemp");
  if (true!=checkNumber(minobj,-20,10,m1))
      return false;
  maxobj=document.getElementById("maxtemp");
  if (true!=checkNumber(maxobj,0,100,m2))
      return false;
  if (isMin(minobj,maxobj,m1,m2)!=true)
      return false;
  if (document.getElementById("byntp").checked==true && document.getElementById("ntpserver").value.length<3)
  {
  	
  	alert("Ntp Server 无效");
  	return false;
  }
  return true;
}
function check_time(flag)
{
	if (flag==1)
	{
		document.getElementById("ntpserver").disabled=true;
		document.getElementById("dt").disabled=false;
	}
	else
	{
		document.getElementById("dt").disabled=true;
		document.getElementById("ntpserver").disabled=false;
	} 
}
window.onload=function(){
	  
	  if (privilege==1)
      disableAllControls(true);
		if (timemode!=2)
		   document.getElementById("bymanual").checked=true;
		else
			document.getElementById("byntp").checked=true;
		document.getElementById("dt").value=sysdate;
		document.getElementById("uptime").innerText=uptime;
		
		document.getElementById("currtemp").innerText=currtemp+"°C";
		document.getElementById("mintemp").innerText=mintemp;	
		document.getElementById("maxtemp").innerText=maxtemp;
		
		document.getElementById("ntpserver").innerText=ntpserver;
		
		check_time(timemode);
}
</script>
<ul id="nav">
  <li><span>系统信息=></span></li>
  <li><a href="nmubi.asp">系统基本信息</a></li>
  <li><a href="sys_info.asp" class="current">系统信息</a></li>
  <li><a href="chassis.asp">电源/风扇</a></li>
  <li><a href="heartbeat.asp">心跳配置</a></li>
  <li><a href="cpumem.htm">CPU/内存</a></li>
  <li><a href="disk.htm">Flash</a></li>
  <li><a href="proc.htm">进程状态</a></li>
  <li><a href="ifconf.htm">ifconfig</a></li>
  <li><a href="/help/sys_info.htm" target="new">帮助</a></li>
</ul>
<div id="content">
<form name="genForm" method="post" action="/goform/date" onsubmit="return check();">
<input type="hidden" id="pid" name="current_page" value="/sys/sys_info.asp">
<table border="1" align="center" cellpadding="0" cellspacing="0" class="dt">
  <caption>
    系统信息
  </caption>
  <tr><th colspan='2'>设置时间</th></tr>
  <tr>
  	<td><input type='radio' name='timesource' id='bymanual' value='1' onclick='check_time(1);'/>手动</td>
    <td><input type="text" readonly="true" class="Wdate" size="21" id='dt' name='dt' value='' onClick="WdatePicker({isShowClear:false,dateFmt:'yyyy-MM-dd HH:mm:ss'});"/>
    </td>
</tr>

<tr>
	<td>
  <input type='radio' name='timesource' id='byntp' value='2' onclick='check_time(2);'/>自动从互联网获取,NTP Server:
  </td>
  <td><input type='text' name='ntpserver' id='ntpserver' value='202.112.10.60'/></td></tr>
  <tr><th>系统运行时间</th><td id="uptime"></td></tr>
  <tr><th colspan='2'>机箱温度</th></tr>
  <tr><td>当前温度</td><td id='currtemp'></td></tr>
  <tr><td>最低温度</td><td><input type='text' name='mintemp' id='mintemp' value='-20' size='5' maxlength='3' onkeyup='inputNumber(this,0,true);' onblur='return check(this);'/>°C</td></tr>
  <tr><td>最高温度</td><td><input type='text' name='maxtemp' id='maxtemp' value='100' size='5' maxlength='3' onkeyup='inputNumber(this,0,true);' onblur='return check(this);'/>°C</td></tr>
</table>
<table align="center">
<tr>
 <td>
  <input class="button white medium" type="button" value="刷新" onclick="window.location.href('sys_info.asp');" name="refresh">
 </td>
 <td>
      <input class="button white medium" type="submit" value="应用" id="apply" name="apply">
 </td>
 </tr>	
</table>
</form>
</div>
</body>
</html>
