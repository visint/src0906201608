<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<script src="js/opt.js" type="text/javascript"></script>
<script src="js/cookie.js" type="text/javascript"></script>
<script language="javascript" type="text/javascript" src="My97DatePicker/WdatePicker.js"></script>
<link rel="stylesheet" href="css/style.css" type="text/css" />
<link rel="stylesheet" href="css/button.css" type="text/css"/>
<title>告警信息</title>
</head>
<body>
<style type="text/css">
td {text-align:left;}
.s1 {color:red;}
.s2 {color:orange;}
.s3 {color:yellow;}
.s4 {color:blue;}
.s5 {color:black;}
img {border-width:0px;}
fieldset{margin:10px 0 0 10px;}
.w180{width:110px;margin-top:0px;}
a:link {
　　color: #874B46;
　　text-decoration:none;
}
a{color:#874B46; text-decoration: none}
a:visited {color:#874B46; text-decoration: none}
</style>
<script language="javascript">
	var is_show_count=false;
	<%Privilege();%>
  <%AlarmShowConf();%>
function query(act,field,order,pg)
{
  var url="/goform/almqry?flag=2";
  var fdate,edate;
  if (act==0)
  {
  	document.getElementById('fromdate').value="";
  	document.getElementById('enddate').value="";
  	document.getElementById('slot').selectedIndex=0;
  	setCookie();
  	return;
  }
  if (act!=0)
  {
  fdate=document.getElementById('fromdate').value;
  edate=document.getElementById('enddate').value;
  url+="&fd="+fdate;
  url+="&ed=" +edate;
  if (act!=1)
     url+="&field="+field+"&order="+order;
  else
   {
     if (fdate.length>8 && edate.length>8 && fdate>edate)
       {
        alert("起始日期必须小于等于截止日期!");
        return;
       }
   }
   url+="&pg=";
   if (pg>0)
      url+=pg;
   else if (-1==pg) 
      url+=document.getElementById('pg').value;
   else 
      url+="0";
   url+="&slot=" + document.getElementById('slot').value;
  }
  setCookie();
  //alert(url);
  window.location.href(url);
}
function del_alarm(flag)
{
 var msg;
 if (flag==0)
    msg="全部告警";
 else
    msg="满足条件的告警";
 if (true!=opt_ack(3,msg))
     return false;
 var url="/goform/alarm?operate=del";
 if (flag==0)
   url+="&aid=0";
 else
 {
   var fdate,edate,slot;
   //url+="";
   fdate=document.getElementById('fromdate').value;
   edate=document.getElementById('enddate').value;
   slot=document.getElementById('slot').value;
   if (fdate.length>8)
    url+="&fd="+fdate;
   if (edate.length>8)
    url+="&ed="+edate;
   if (slot>-1)
     url+="&slot="+slot;
 }
 //alert(url);
 window.location.href(url);
}
function setCookie()
{
    var q=document.getElementById("fromdate");
    cf_SetCookie("hafromdate",q.value,1);
    q=document.getElementById("enddate");
    cf_SetCookie("haenddate",q.value,1);
    q=document.getElementById("slot");
    cf_SetCookie("haslot",q.selectedIndex,1);
    return true;
}
window.onload=function(){
if (privilege<2)
	  	document.getElementById('operate').style.display="none";
var val;
val=cf_GetCookie("hafromdate");
if (val!=null)
   document.getElementById('fromdate').value=val;

val=cf_GetCookie("haenddate");
if (val!=null)
   document.getElementById('enddate').value=val;
val=cf_GetCookie("haslot");
if (val!=null)
   document.getElementById('slot').selectedIndex=val;
   
}
</script>
<ul id="nav">
  <li><span>告警管理=></span></li>
  <li><a href="alarm_list.asp">当前告警</a></li>
  <li><a href="alarm_his.asp" class="current">历史告警</a></li>
  <li><a href="alm_type.asp">告警策略</a></li>
  <li><a href="almconf.asp">告警配置</a></li>
  <li><a href="help/alarm.htm" target="new">帮助</a></li>
</ul>
<div id="content" align="center">
<table  border="0" cellpadding="0" cellspacing="0">
<tr>
<td valign="top">
<table  border="1" cellpadding="0" cellspacing="0" class="dt">
  <caption>历史告警信息</caption>
  <tr class="toptitle">
    <th>位置&nbsp;&nbsp;<a href="#" onclick="query(2,1,1,-1);"><img src='images/up.gif'/></a>&nbsp;&nbsp;<a href="#" onclick="query(2,1,2,-1);"><img src='images/down.gif'/></a></th>
    <th>告警信息&nbsp;&nbsp;<a href="#" onclick="query(2,2,1,-1);"><img src='images/up.gif'/></a>&nbsp;&nbsp;<a href="#" onclick="query(2,2,2,-1);"><img src='images/down.gif'/></a></th>
    <th>告警状态&nbsp;&nbsp;<a href="#" onclick="query(2,3,1,-1);"><img src='images/up.gif'/></a>&nbsp;&nbsp;<a href="#" onclick="query(2,3,2,-1);"><img src='images/down.gif'/></a></th>
    <!--<th>首次发生时间&nbsp;&nbsp;<a href="#" onclick="query(2,4,1,-1);"><img src='images/up.gif'/></a>&nbsp;&nbsp;<a href="#" onclick="query(2,4,2,-1);"><img src='images/down.gif'/></a></th>-->
    <th>
    <script language="javascript">
	    if ( is_add_once)
       document.write("首次");
    </script>
    发生时间&nbsp;&nbsp;<a href="#" onclick="query(2,4,1,-1);"><img src='images/up.gif'/></a>&nbsp;&nbsp;<a href="#" onclick="query(2,4,2,-1);"><img src='images/down.gif'/></a></th>
    
    <script language="javascript">
	    if ( is_add_once)
       document.write("<th>最后发生时间</th>");
    </script>
    <!--<th>最后发生时间</th>-->
    <th>清除时间</th>
    <th>确认时间</th>
    <script language="javascript">
	    if (is_show_count)
       document.write("<th>次数</th>");
    </script>
    <th>告警级别&nbsp;&nbsp;<a href="#" onclick="query(2,5,1,-1);"><img src='images/up.gif'/></a>&nbsp;&nbsp;<a href="#" onclick="query(2,5,2,-1);"><img src='images/down.gif'/></a></th>
    <th>告警原因</th>
<th>删除</th>
</tr>
  <%AlarmList(2,4,1);%>
</table>
</td>
<td  valign="top" style="width:192px;">
<fieldset>
   <legend>查询条件:</legend>
   <label for="fromdate">起始日期:</label>
   <input id="fromdate" class="Wdate" type="text" onClick="WdatePicker({maxDate:'%y-%M-%d'})">
   <br/>
   <label for="enddate">截止日期:</label>
   <input id="enddate" class="Wdate" type="text" onClick="WdatePicker({minDate:'#F{$dp.$D(\'fromdate\')}',maxDate:'%y-%M-%d'})">
   <br/>
   <label for="slot">选择槽号:</label>
<select name="slot" id="slot">
  <option value="-1">全部</option>
  <option value="0">机箱</option>
  <option value="1">第1槽</option>
  <option value="2">第2槽</option>
  <option value="3">第3槽</option>
  <option value="4">第4槽</option>
  <option value="5">第5槽</option>
  <option value="6">第6槽</option>
  <option value="7">第7槽</option>
  <option value="8">第8槽</option>
  <option value="9">第9槽</option>
  <option value="10">第10槽</option>
  <option value="11">第11槽</option>
  <option value="12">第12槽</option>
  <option value="13">第13槽</option>
  <option value="14">第14槽</option>
  <option value="15">第15槽</option>
  <option value="16">第16槽</option>
</select>
<br/>
   <a class="button white medium" href="#" onclick="query(1,0,0,0);">查询</a><a class="button white medium" href="#" onclick="query(0,0,0,0);">重置</a>
</fieldset>
<fieldset id="operate">
   <legend>操作:</legend>
<br/>
<a class="button white medium w180" href="#" onclick="return del_alarm(1);">删除满足条件的告警</a>
<br/>
<a class="button white medium w180" href="#" onclick="return del_alarm(0);">删除全部告警</a>
<br/>
</fieldset>
</td>
</tr>
</table>
</div>
</body>
</html>
