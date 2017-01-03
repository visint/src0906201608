<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8"/>
<script src="../js/edit.js" type="text/javascript"></script>
<script src="../js/checknum.js" type="text/javascript"></script>
<script src="../js/validate.js" type="text/javascript"></script>
<script src="../js/math.js" type="text/javascript"></script>
<link rel="stylesheet" href="../css/style.css" type="text/css" />
<link rel="stylesheet" href="../css/button.css" type="text/css" />
<title>心跳配置</title>
</head>
<style type="text/css">
.c td{padding:3px;}
</style>
<script language="javascript">
var hb_groups= new Array(4);
<%Privilege();%>
<%UnitClass(0);%>
<%HeartBeat();%>
window.onload=function(){
	  
	  if (privilege<2)
	    disableInputControls(privilege,true);
     // disableAllControls(true);
}
String.prototype.trim = function()  
{  
   return this.replace(/(^\s*)|(\s*$)/g, "");  
}
function check_IP(ip)    
{  //var ip = document.getElementById('reg_ip').value;
   var re=/^(\d+)\.(\d+)\.(\d+)\.(\d+)$/;//正则表达式   
   if(re.test(ip))   
   {   
       if( RegExp.$1<256 && RegExp.$2<256 && RegExp.$3<256 && RegExp.$4<256) 
       return true;   
   }   
   //alert("IP有误！");   
   return false;    
}
function parseSlot()
{
	var n,m,o,g,url="",slots,group;
	for (n=1;n<5;n++)
	{
		// if (n>1)
		 //   url+="&";
		 //url+="hb_group"+n+"=";
		 url="";
		 group="";
		 g=document.getElementById("l" +n);
		 if (g!=null)
		 {
		 	 slots="";
		 	 //url+="&Linkage"+n+"=";
		 	 
		 	 	//url+="&slots"+n+"=";
		 	 for (m=1;m<maxslot;m++)
		 	 {
		 	 	  o=document.getElementById("r" +n +"_"+m);
			     if (o!=null &&o.checked)
			     slots+=","+m;
		 	 }
		 	 if (slots!="")
		 	 {
		 	 	if (g.checked)
		 	   url+="1";
		 	  else
		 	 	  url+="2";
		 	 	url+=":";
		 	 	slots=slots.substring(1);
		 	 	url+=slots;
		 	 	 o=document.getElementById("ip" +n);
		 	  url+=":"+o.value;
		 	 }		 	
		 }
		 o=document.getElementById("hb_group" +n);
		 o.value=url;
		// alert(url);
	}
	//alert(url);
}
function check()
{
	//return true;
  var obj;
  obj=document.getElementById("sec");
  if (true!=checkNumber(obj,0,50,"超时秒数"))
      return false;
  obj=document.getElementById("msec");
  if (true!=checkNumber(obj,0,999,"超时毫秒数"))
      return false;
  if (confirm("你真的要设置心跳参数吗?")!=true)
   return false;

  obj=document.getElementById("enabled");
  if (obj.checked!=true)
    obj.value=2;
  else
  	 obj.value=1;
  obj=document.getElementById("reply");
  //alert(obj.value);
  if (obj.checked!=true)
    obj.value="N";
  else
  	  obj.value="Y";
  //alert(obj.value);
  //var en=document.getElementById("enable");
  /*
  var slot=document.getElementById("slot").value;
  if (slot<1)
  {
  	alert("请选择OBP卡所在的槽位号");
  	return false;
  }
  var n,ss,buf="",idx=-1;
  ss=slots.split(";");
  //alert(en.checked);
  for (n=0;n<ss.length;n++)
  {
  	if (ss[n]==slot)
  	{
  		idx=n;
  		break;
  	}
  }
  */	
  /*
  if (en.checked)
  {
  	
  	if (acp_ip.length<6)
  	{
  		alert("监听IP不能为空!");
  		return false;
  	}
  	var ips =acp_ip.split(";")
    var n;
    for (n=0;n<ips.length;n++)
    {
  	 if (check_IP(ips[n])!=true)
  	 {
  		alert("存在无效的IP");
  		return false;
  	 }
    }
    
  	if (idx<0)
  	{
  		ss[ss.length]=slot;
  		ss.sort(function(a,b){
  			return (a-b)>0?1:-1
  			}
  		);//从小到大排序
  	}
 
  	for (n=0;n<ss.length;n++)
    {
     if (ss[n]!=slot ||ss[n]==slot && acp_ip!="")
  	 {
  		if (buf!="")
  		  buf+=";";
  		buf+=ss[n].trim();
  	 }
    }
 }
  else
  {
   for (n=0;n<ss.length;n++)
   {
   	if (ss[n]!=slot && acp_ip!="")
  	{
  		if (buf!="")
  		  buf+=";";
  		buf+=ss[n].trim();
  	}
   }
  }
  */
 // document.getElementById("slots").value=buf;
  //alert(document.getElementById("slots").value);
  parseSlot();
  return true;
}
function showAcceptIp(slot)
{
	if (slot<1)
	   return;
	var obj=document.getElementById("acp_ip");
	obj.value=ip_list[slot-1];
	var ss=slots.split(";");
	var n;
	for (n=0;n<ss.length;n++)
	{
		if (ss[n]==slot)
		{
			 //alert(n);
			 var en=document.getElementById("enable");
			 if (!en.checked)
			 {
			 	 en.value=2;
			 	 obj.disabled=true;
			 }
			 else
			 	   obj.disabled=false;
			 break;
		}
	}
}

function enableReply(obj)
{
	/*
	var obj=document.getElementById("default_mode");
	alert(obj.checked);
	if (obj.checked!=true)
	{
	 obj=document.getElementById("ehco");
	 if (!en.checked)
	 {
		obj.disabled=true;
	 }
	 else
		obj.disabled=false;
	}
	*/
	if (obj.checked)
	 obj.value=1;
	else
		obj.value=2;
		alert(obj.value);
}

function showObpList()
{
	var n,m;
	var slot;
	var selected;
	for (n=1;n<maxslot;n++)
	{
		slot="s"+n;
		selected=false;
		document.write("#"+n + "<input type='checkbox' name='"+slot +"' id='"+slot + "'");
		for (m=0;m<hb_slots.length;m++)
		{
			if (hb_slots[m]==n)
		  {
		  	selected=true;
		  	break;
		  }
		}
		if (selected)
		 document.write(" checked");
		//else
		//	document.write("");
		document.write("/>");
	}
}
function showSlotHeartbeat()
{
	var slot=document.getElementById("slot").value;
	var en=document.getElementById("enable");
	var ips=document.getElementById("acp_ip");
	var ss=slots.split(";");
	var n;
	var en=document.getElementById("enable");
	
	var n;
	for (n=1;n<maxslot;n++)
	{
		
	}
	showAcceptIp(slot);
	for (n=0;n<ss.length;n++)
	{
		if (ss[n]==slot)
		{
			 if (!en.checked)
			 {
			 	 en.value=1;
			 	 en.checked=true;
			 	 ips.disabled=false;
			 }
			 return;
		}
	}
	en.value=2;
	en.checked=false;
	ips.disabled=true;
}
function msgTypeChange(mt)
{
	var oAcMsg=document.getElementById("acp_msg");
	//var oEhco=document.getElementById("ehco");
	if (mt!=1)
	{
		oAcMsg.disabled=false;
		//oEhco.disabled=false;
	}
	else
	{
			oAcMsg.disabled=true;
			//oEhco.disabled=true;
	}
}

function showHeartBeatSlots()
{
	var n,m,r,g,o,v;
	if (maxslot!=16)
	  r=1;
	else
		r=4;
	document.write("<table border='1' align='center'>");
	document.write("<tr>");
	if (maxslot==16)
	{
	document.write("<td>");
	document.write("组号");
	document.write("</td>");
  }
	document.write("<td>");
	document.write("联动");
	document.write("</td>");
	
	for (n=1;n<maxslot;n++)
	{
		document.write("<td align='center'>");
		document.write(n);
		document.write("</td>");
	}
	document.write("<td>");
	document.write("源IP");
	document.write("</td>")
	document.write("</tr>");
	for (n=0;n<r;n++)
	{

		document.write("<tr>");
		if (maxslot==16)
		{
			document.write("<td align='center'>");
		  document.write(n+1);
		  document.write("</td>");
	  }
		document.write("<td align='center'>");
		document.write("<input type='checkbox' id='l" +(n+1) +"' name='g" +m+ "' />");
		document.write("</td>");
		for (m=1;m<maxslot;m++)
		{
		document.write("<td>");
		document.write("<input type='radio' id='r" +(n+1)+"_"+m +"' name='g" +m+ "' />");
		document.write("</td>");
	  }
	  document.write("<td>");
		document.write("<input type='text' id='ip"+(n+1) +"' name='ip" +m+ "' />");
		document.write("</td>");
	  document.write("</tr>");
	}
	document.write("<tr>");
	if (maxslot==16)
	   document.write("<td colspan='2'>不选</td>");
	else
		 document.write("<td>不选</td>");
	for (n=1;n<maxslot;n++)
	{
		document.write("<td>");
		document.write("<input type='radio' id='r0_"+ n+"' name='g" +n+ "' />");
		document.write("</td>");
	}
	document.write("<td></td>");
	document.write("</tr>");
	document.write("<tr>");
	document.write("<td align='center' colspan='");
	if (maxslot==16)
	   document.write("18'");
	else
		 document.write((maxslot+1)+"'");
	//document.write("><input type='button' name='refresh' value='刷新' onclick=\"location.href='hb.html';\" />");
	/*
	document.write("><form method='post' action='/goform/SetHeartBeat' onsubmit='return check();'>");
	document.write("<input type='hidden' id='hb_group1' name='hb_group1' value=''/>");
	document.write("<input type='hidden' id='hb_group2' name='hb_group2' value=''/>");
	document.write("<input type='hidden' id='hb_group3' name='hb_group3' value=''/>");
	document.write("<input type='hidden' id='hb_group4' name='hb_group4' value=''/>");
	document.write("<input type='button' name='refresh' value='刷新' onclick=\"location.href='hb.html';\" />");
	document.write("<input type='submit' name='apply' value='应用'/>");
	document.write("</form>");
	*/
	//document.write("<input type='button' name='refresh' value='刷新' onclick=\"location.href='hb.html';\" />");
	//document.write("<input type='button' name='apply' value='应用' onclick='javascript:setConf()'/>");
	document.write("</td>");
	document.write("<tr>");
	document.write("</table>");
	for (n=0;n<r;n++)
	{
		if (hb_groups[n]!=null)
		{
			if (hb_groups[n].Linkage==1)
			{
			o=document.getElementById("l" +(n+1));
			  if (o!=null)
			   o.checked=true;
			}
			for (m=0;m<hb_groups[n].Slots.length;m++)
			{
			  o=document.getElementById("r" +(n+1) +"_"+hb_groups[n].Slots[m]);
			  if (o!=null)
			   o.checked=true;
			}
			o=document.getElementById("ip" +(n+1));
			if (o!=null)
			 o.value=hb_groups[n].Ip;
		}
	}
	
}
function showHeartBeat()
{
	document.write("<tr><td>是否启用</td><td><input type='checkbox' name='enabled' id='enabled' value='1'");
	if (enabled==1)
	  document.write(" checked=true");
	document.write("/>启用</td>");
	
	document.write("<td>启动状态</td><td>");
	if (is_start==1)
	  document.write("已");
	else
		document.write("未");
	document.write("启动</td>");
	document.write("<td>开机后心跳延时启动时间</td><td><input type='text' id='waitting' name='waitting' maxlength='2' size='2' value="+waitting+" onkeyup='uIntInput(this);'>秒</td></tr>");
	document.write("<tr><td>超时时间</td><td><input type='text' id='sec' name='sec' maxlength='2' size='2' value="+sec +" onkeyup='uIntInput(this);'>秒<input type='text' id='msec' name='msec' maxlength='3' size='3' value="+msec +" onkeyup='uIntInput(this);'>毫秒</td>");
	document.write("<td>监听端口</td><td><input type='text' id='port' name='port' maxlength='6' size='6' value="+port +" onkeyup='uIntInput(this);'></td>");
	document.write("<td>心跳报文</td><td><input type='radio' name='hb_mode' id='default_mode' value='1' onchange = \"msgTypeChange(1);\"");
	
	if (hb_mode==1)
	  document.write(" checked=true");
	document.write("/>系统默认<input type='radio' name='hb_mode' value='2' onchange =\"msgTypeChange(2);\"");
	if (hb_mode!=1)
	   document.write(" checked=true");
	document.write("/>用户指定</td></tr>");
	document.write("<tr><td>认证码</td><td><input type='text' id='acp_msg' name='acp_msg' maxlength='16' size='16' value="+acp_msg +"></td>");
	document.write("<td>是否回应</td><td><input type='checkbox' id='reply' name='reply' value='Y'")
	if (enabledReply)
	    document.write(" checked=true");
	document.write("/>回应</td>");
	   
	document.write("<td>回应信息</td><td><input type='text' id='ehco' name='ehco' maxlength='12' size='12' value="+ehco +"></td></tr>");
	document.write("<tr><td>选择心跳槽位</td><td colspan='6'>");
	//showObpList();
	document.write("</td></tr>");
	
	document.write("<tr><td colspan='6'>");
	showHeartBeatSlots();
	document.write("</td></tr>");
	document.write("<tr><td>说明</td><td colspan='5'>配置后需要重启系统方能生效</br>认证码为16进制字节,最多16个字节，字节之间用分号';'隔开</td></tr>");
	//showSlotHeartbeat();
	if (hb_mode==1)
	  msgTypeChange(1);
	else
		msgTypeChange(2);
}
</script>
<body>
<ul id="nav">
  <li><span>系统信息=></span></li>
  <li><a href="nmubi.asp">系统基本信息</a></li>
  <li><a href="sys_info.asp">系统信息</a></li>
  <li><a href="chassis.asp">电源/风扇</a></li>
  <li><a href="heartbeat.asp" class="current">心跳配置</a></li>
  <li><a href="cpumem.htm">CPU/内存</a></li>
  <li><a href="disk.htm">Flash</a></li>
  <li><a href="proc.htm">进程状态</a></li>
  <!--<li><a href="logusr.htm">登录用户</a></li>-->
  <li><a href="/help/sys_info.htm" target="new">帮助</a></li>
</ul>
<div id="content">
<form name="genForm" method="post" action="/goform/SetHeartBeat" onsubmit="return check();">
<input type="hidden" id="pid" name="current_page" value="/sys/heartbeat.asp"/>
<input type="hidden" id="slots" name="slots" value=""/>

<input type='hidden' id='hb_group1' name='hb_group1' value=""/>
<input type='hidden' id='hb_group2' name='hb_group2' value=""/>
<input type='hidden' id='hb_group3' name='hb_group3' value=""/>
<input type='hidden' id='hb_group4' name='hb_group4' value=""/>
 
<table border="1" align="center" cellpadding="0" cellspacing="0" class="dt">
  <caption>
    心跳配置
  </caption>
  <script language="javascript">
  showHeartBeat();
  </script>
</table>
<table align="center">
<tr>
 <td>
  <input class="button white medium" type="button" value="刷新" onclick="window.location.href('heartbeat.asp');" name="refresh"/>
 </td>
 <td>
  <input class="button white medium" type="submit" value="应用" name="apply"/>
 </td>
 </tr>	
</table>
</form>
</div>
</body>
</html>
