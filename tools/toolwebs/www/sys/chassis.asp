<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<script src="../js/checknum.js" type="text/javascript"></script>
<script src="../js/math.js" type="text/javascript"></script>
<script src="../js/msg.js" type="text/javascript"></script>
<link rel="stylesheet" href="../css/style.css" type="text/css" />
<link rel="stylesheet" href="../css/button.css" type="text/css"/>
<title>电源/风扇</title>
</head>
<style type="text/css">
.c td{padding:3px;}
</style>
<script language="javascript">
function check()
{
  var minobj,maxobj;
  var m1="最低电压",m2="最高电压";
  minobj=document.getElementById("minvolt");
  if (true!=checkNumber(minobj,2,10,m1))
      return false;
  maxobj=document.getElementById("maxvolt");
  if (true!=checkNumber(maxobj,10,20,m2))
      return false;
  if (isMin(minobj,maxobj,m1,m2)!=true)
      return false;
  return true;
}
function app()
{
   alert("submit");
  if (true!=check())
    return false;
   return true;
}
</script>
<body>
<ul id="nav">
  <li><span>系统信息=></span></li>
  <li><a href="nmubi.asp">系统基本信息</a></li>
  <li><a href="sys_info.asp">系统信息</a></li>
  <li><a href="chassis.asp" class="current">电源/风扇</a></li>
  <li><a href="cpumem.htm">CPU/内存</a></li>
  <li><a href="disk.htm">Flash</a></li>
  <li><a href="proc.htm">进程状态</a></li>
  <li><a href="help/sys_info.htm" target="new">帮助</a></li>
</ul>
<div id="content">
<form name="genForm" method="POST" action="/goform/sys" onsubmit="return check();" style="margin-top:0;" >
<INPUT type="hidden" name="act" value="3">
<table border="1" align="center" cellpadding="0" cellspacing="0" class="dt">
  <caption>电源</caption>
  <tr>
    <th>电源</th>
    <th>在位状态</th>
    <th>运行状态</th>
    <th>电源类型</th>
    <th>当前电压(Volt)</th>
  </tr>
  
  <%UnitInfo(0,1);%>
</table>

<table  border="1" align="center" cellpadding="0" cellspacing="0" class="dt">
  <caption>风扇</caption>
  <tr>
      <%UnitInfo(0,2);%>
  </tr>
</table>
<table align="center">
 <tr>
   <td align="center">
      <input class="button white medium" type="button" value="刷新" onclick="window.location.href('chassis.asp');">
    </td>
   <td align="center">
        <input class="button white medium" type="submit" value="应用" name="apply">
    </td>
  </tr>	
</table>
</form>
</div>
</body>
</html>
<script ></script>
