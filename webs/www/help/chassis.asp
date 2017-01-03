<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<link rel="stylesheet" href="../css/cview1.css" type="text/css" />
<link rel="stylesheet" href="../css/style.css" type="text/css" />
<script src='help.js' type='text/javascript'></script>
<title>帮助--设备视图</title>
</head>

<body onload="load()">
<script language="javascript">
<%UnitClass(0);%>
function load()
{
	if (us[0].uclass!=4)
  document.getElementById("4u").style.display = "none";
  else
	{document.getElementById("1uf").style.display = "none";
	 document.getElementById("1ub").style.display = "none"; 
  }
}
	showMenu("chassis");
	if (us[0].uclass!=4)
	{
		document.write("<link href=\"../css/not4uview.css\" rel=\"stylesheet\" type=\"text/css\">");
		document.write("<!--[if lte IE 8]>\n"+
"<style type=\"text/css\">\n"+
".sl,.ml {-ms-filter: \"progid:DXImageTransform.Microsoft.BasicImage(rotation=1)\";\n"+
   "filter: progid:DXImageTransform.Microsoft.BasicImage(rotation=1);}"+
".ml{height:28px;} .sl{height:25px} .un{width:25px;}"+
".leds .sl{height:16px;width:26px;font-size:10px;}"+
"</style>\n<![endif]-->");
  }
	else
	 document.write("<link href=\"../css/cview.css\" rel=\"stylesheet\" type=\"text/css\">");
</script>

  <h2>设备视图说明</h2>
  <p>
  	<ul>
  		<li>顶排为槽位号,点击槽位号,就会在功能操作区显示该业务板卡的基本信息,如果该槽位的业务板卡存在,点击板卡名称,就会在功能操作区显示该业务板卡的操作功能</li>
  		<li>面板中间显示业务板卡各端口的状态,把鼠标移到端口图标,会出现该端口的信息提示,如公里数、传输距离、波长等信息</li>
  		<li>如果单元盘有未清除或未确认的告警,面板下面会出现感叹号图标,点击该图标,在功能操作区显示告警信息</li>
  		<li>网管单元盘下面的闪灯指示设备有未清除或未确认的告警</li>
  		<li>设备视图定时刷新，默认刷新时间间隔为20秒,用户可以通过定时刷新设置指定</li>
  	</ul>
  </p>
  
<div id="cv">
<table width="100%" id="4u">
<tr>
<td width="37" border="0" background="../images/lhanger.png"></td>
<td>
<div class='uh1'><a href='/goform/slot?cid=0&uid=1&page=bi' target='mf'>1&nbsp;</a></div><div class='uh1'><a href='/goform/slot?cid=0&uid=2&page=bi' target='mf'>2&nbsp;</a></div><div class='uh1'><a href='/goform/slot?cid=0&uid=3&page=bi' target='mf'>3&nbsp;</a></div><div class='uh1'><a href='/goform/slot?cid=0&uid=4&page=bi' target='mf'>4&nbsp;</a></div><div class='uh1'><a href='/goform/slot?cid=0&uid=5&page=bi' target='mf'>5&nbsp;</a></div><div class='uh1'><a href='/goform/slot?cid=0&uid=6&page=bi' target='mf'>6&nbsp;</a></div><div class='uh1'><a href='/goform/slot?cid=0&uid=7&page=bi' target='mf'>7&nbsp;</a></div><div class='uh1'><a href='/goform/slot?cid=0&uid=8&page=bi' target='mf'>8&nbsp;</a></div><div class='uh1'><a href='/goform/slot?cid=0&uid=9&page=bi' target='mf'>9&nbsp;</a></div><div class='uh1'><a href='/goform/slot?cid=0&uid=10&page=bi' target='mf'>10&nbsp;</a></div><div class='uh1'><a href='/goform/slot?cid=0&uid=11&page=bi' target='mf'>11&nbsp;</a></div><div class='uh1'><a href='/goform/slot?cid=0&uid=12&page=bi' target='mf'>12&nbsp;</a></div><div class='uh1'><a href='/goform/slot?cid=0&uid=13&page=bi' target='mf'>13&nbsp;</a></div><div class='uh1'><a href='/goform/slot?cid=0&uid=14&page=bi' target='mf'>14&nbsp;</a></div><div class='uh1'><a href='/goform/slot?cid=0&uid=15&page=bi' target='mf'>15&nbsp;</a></div><div class='uh1'><a href='/sys/nmubi.asp' target='mf'>16&nbsp;</a></div><div class="clr"></div><div class='ubd1'></div><div class='ubd1'><div class='un'><a href='/goform/slot?cid=0&uid=2&page=odm' target='mf'>ODM</a></div><table class='hport'><tr><td>OUT<br/>IN</td><td><img src='../images/lc.gif'></td></tr><tr><td>PASS<br/>PASS</td><td><img src='../images/lc.gif'></td></tr><tr><td>C17<br/>C28</td><td><img src='../images/lc.gif'></td></tr><tr><td>C16<br/>C21</td><td><img src='../images/lc.gif'></td></tr><tr><td>C17<br/>C26</td><td><img src='../images/lc.gif'></td></tr><tr><td>C22<br/>C17</td><td><img src='../images/lc.gif'></td></tr><tr><td>C25<br/>C29</td><td><img src='../images/lc.gif'></td></tr><tr><td>C17<br/>C19</td><td><img src='../images/lc.gif'></td></tr><tr><td>C17<br/>C18</td><td><img src='../images/lc.gif'></td></tr><tr><td>C39<br/>C38</td><td><img src='../images/lc.gif'></td></tr><tr><td></td><td>OD  OM</td></tr></table></div><div class='ubd1'></div><div class='ubd1'></div><div class='ubd1'>  <div class='un'><a href='/goform/unit?cid=0&uid=5' target='mf'>OTU</a></div>  <div class='leds'> <div class='vp'><span>PWR<img style='margin-left:2px;' src='../images/led_1.png' /><img style='margin-right:2px;'src='../images/led_1.png' />RUN</span> </div><div class='vp'><span>L1<img  src='../images/led_0.png' /><img src='../images/led_0.png' /><img src='../images/led_0.png' /><img src='../images/led_0.png' />L4</span></div><div class='vp'><span>C1<img  src='../images/led_0.png' /><img src='../images/led_0.png' /><img src='../images/led_0.png' /><img src='../images/led_0.png' />C4</span></div></div><table class='sfpgr'><tr><td><table><tr>	<td>L1 RX<br/>L1 TX</td>	</tr>	<tr>	<td>C1 RX<br/>C1 TX</td>	</tr>	<tr>	<td>L2 RX<br/>L2 TX</td>	</tr>	<tr>	<td>C2 RX<br/>C2 TX</td>	</tr></table>	</td><td>	<table border='2'>	<tr>	<td><img class='sfp' src='../images/p2_0.gif'></td>	</tr>	<tr>	<td><img class='sfp' src='../images/p2_0.gif'></td>	</tr><tr><td><img class='sfp' src='../images/p2_0.gif'></td></tr>	<tr><td><img class='sfp' src='../images/p2_0.gif'></td></tr>	</table></td></tr></table>  <table class='sfpgr'><tr><td><table><tr>	<td>L3 RX<br/>L3 TX</td>	</tr>	<tr>	<td>C3 RX<br/>C3 TX</td>	</tr>	<tr>	<td>L4 RX<br/>L4 TX</td>	</tr>	<tr>	<td>C4 RX<br/>C4 TX</td>	</tr></table>	</td><td>	<table border='2'>	<tr>	<td><img class='sfp' src='../images/p2_0.gif'></td>	</tr>	<tr>	<td><img class='sfp' src='../images/p2_0.gif'></td>	</tr><tr><td><img class='sfp' src='../images/p2_0.gif'></td></tr>	<tr><td><img class='sfp' src='../images/p2_0.gif'></td></tr>	</table></td></tr></table>  </div><div class='ubd1'></div>

<div class='ubd1'><div class='un'><a href='/goform/unit?cid=0&uid=7' target='mf'>OTU</a></div>
<div class='leds'>
	<div class='ll'>PWR</div>
	<div class='led'><img src='../images/led_1.png' /></div>
	<div class='led'><img src='../images/led_0.png' /></div>
	<div class='ll'>OPM</div>
	<div class='ll'>L1</div>
	<div class='led'><img src='../images/led_0.png' /></div>
	<div class='led'><img src='../images/led_0.png' /></div>
	<div class='ll'>L2</div>
	<div class='ll'>C1</div>
	<div class='led'><img src='../images/led_0.png' /></div>
	<div class='led'><img src='../images/led_0.png' /></div>
	<div class='ll'>C2</div>
	<div class='clr'></div>
</div>
<div class='port2'>
<ul style='font-size:7px;'>
 <li>L1 RX</li>
 <li>L1 TX</li>
</ul><div class='pimg'><img src='../images/p2_0.gif'/></div></div><div class='port2'>
<ul style='font-size:7px;'>
 <li>C1 RX</li>
 <li>C1 TX</li>
</ul><div class='pimg'><img src='../images/p2_0.gif'/></div></div><div class='port2'>
<ul style='font-size:7px;'>
 <li>L2 RX</li>
 <li>L2 TX</li>
</ul><div class='pimg'><img src='../images/p2_0.gif'/></div></div><div class='port2'>
<ul style='font-size:7px;'>
 <li>C2 RX</li>
 <li>C2 TX</li>
</ul><div class='pimg'><img src='../images/p2_0.gif'/></div></div></div>
<div class='ubd1'></div><div class='ubd1'></div><div class='ubd1'></div>

<div class='ubd1'><div class='un'><a href='/goform/unit?cid=0&uid=11' target='mf'>OTU</a></div>
<div class='leds'>
	<div class='ll'>PWR</div>
	<div class='led'><img src='../images/led_1.png' /></div>
	<div class='led'><img src='../images/led_0.png' /></div>
	<div class='ll'>OPM</div>
	<div class='ll'>L1</div>
	<div class='led'><img src='../images/led_1.png' /></div>
	<div class='led'><img src='../images/led_0.png' /></div>
	<div class='ll'>L2</div>
	<div class='ll'>C1</div>
	<div class='led'><img src='../images/led_0.png' /></div>
	<div class='led'><img src='../images/led_0.png' /></div>
	<div class='ll'>C2</div>
	<div class='clr'></div>
</div>
<div class='port2'>
<ul style='font-size:7px;'>
 <li>L1 RX</li>
 <li>L1 TX</li>
</ul><div class='pimg'><a href='/goform/slot?cid=0&uid=11&page=otuddm'><img src='../images/p2_1.gif' title='波长:1559.00nm &#10;传输距离:40.00KM &#10;当前速率:10.79G'/></a></div></div><div class='port2'>
<ul style='font-size:7px;'>
 <li>C1 RX</li>
 <li>C1 TX</li>
</ul><div class='pimg'><img src='../images/p2_0.gif'/></div></div><div class='port2'>
<ul style='font-size:7px;'>
 <li>L2 RX</li>
 <li>L2 TX</li>
</ul><div class='pimg'><img src='../images/p2_0.gif'/></div></div><div class='port2'>
<ul style='font-size:7px;'>
 <li>C2 RX</li>
 <li>C2 TX</li>
</ul><div class='pimg'><img src='../images/p2_0.gif'/></div></div></div>
<div class='ubd1'></div><div class='ubd1'></div><div class='ubd1'></div>
<div class='ubd1'></div>
<div class="ubd1">
<div class="un"><a href='sys/sys_info.asp' target='mf'>NMU</a></div>
<div class="leds">    <div class="ll">PWR1</div><div class="led"><img id="pwr1" src="../images/led_0.png" /></div>
<div class="led"><img id="pwr2" src="../images/led_1.png" /></div>     <div class="ll">PWR2</div>     <div class="ll">FAN</div>     <div class="led"><img id="fan" src="../images/led_1.png" /></div>     <div class="led"><img id="run" src="../images/led_1.png" /></div>     <div class="ll">RUN</div>     <div class="ll">SFP1</div>     <div class="led"><img id="osc1" src="../images/led_0.png" /></div>     <div class="led"><img id="osc2" src="../images/led_0.png" /></div>     <div class="ll">SFP2</div>     <div class="clr"></div>    </div>
<div class="port">   <ul>   <li>&nbsp;</li>    <li>SFP1</li>   <li>&nbsp;</li>  </ul> 
<div class="pimg"><img id="sfp1" style="width:20px;height:28px;" src='../images/p2_0.gif'/></div>     </div>
<div class="clr"></div> 
<div class="port">    <ul>	<li>&nbsp;</li>	<li>SFP2</li>	<li>&nbsp;</li>    </ul>    <div class="pimg"><img id="sfp2" style="width:20px;height:28px;" src="../images/p2_0.gif"/></div>     </div>
<div class="clr"></div>
<div class="adpter vsb0"> 	 <div class="pl">LAN1</div> 	 <div class="pimg"><img id="lan1" src='../images/epl_2.gif'/></div>     </div>     <div class="clr"></div>
<div class="adpter vsb0">       <div class="pl">LAN2</div> 	<div class="pimg"><img id="lan2" src='../images/epl_1.gif'/></div>     </div>
<div class="clr"></div> <div class="adpter vsb0"> 	<div class="pl">CON</div><div class="pimg"><img src='../images/p1_2.gif'/></div>     </div>
<div class="clr"></div> <div class="adpter vsb0"> 	<div class="pl">Stack</div> 	<div class="pimg"><img src='../images/p1_2.gif'/></div>     </div>
<div class="clr"></div>
</div>
<div class="clr"></div>
<div class='uf1'></div>
<div class='uf1'></div>
<div class='uf1'><a href='/goform/almqry?flag=1&slot=3'><img src='../images/bell.gif'/></a></div>
<div class='uf1'><a href='/goform/almqry?flag=1&slot=4'><img src='../images/bell.gif'/></a></div>
<div class='uf1'></div>
<div class='uf1'></div>
<div class='uf1'><a href='/goform/almqry?flag=1&slot=7'><img src='../images/bell.gif'/></a></div>
<div class='uf1'></div>
<div class='uf1'><a href='/goform/almqry?flag=1&slot=9'><img src='../images/bell.gif'/></a></div>
<div class='uf1'></div><div class='uf1'><a href='/goform/almqry?flag=1&slot=11'><img src='../images/bell.gif'/></a></div>
<div class='uf1'></div><div class='uf1'></div>
<div class='uf1'><a href='/goform/almqry?flag=1&slot=14'><img src='../images/bell.gif'/></a></div>
<div class='uf1'></div>
<div class='uf1'></div>
</td>
<td width="37" border="0" background="../images/rhanger.png"></td>
<!--</td>
</td><td width="37" border="0"  background="../images/rhanger.png"/></td>-->
</tr>
</table>
<div class="oneUchassis" id="1uf"><div style="width: 30px; height: 120px; float: left;"></div><div class="uv1">
<div class="uh"><div class="slot"><div class="sl">1</div></div></div>
<div class="ubd"><div class="un"><div class="sl">OLP</div></div><div class="leds"><table width="100%"><tbody><tr><th><div class="ltext"><div class="sl">R1</div></div></th><th><div class="ltext"><div class="sl">TX</div></div></th><th><div class="ltext"><div class="sl">PWR</div></div></th></tr><tr><td style="height: 3px;"></td></tr><tr><td><img src="../images/led_1.png"></td><td><img src="../images/led_2.png"></td><td><img src="../images/led_1.png"></td></tr><tr><td><img src="../images/led_1.png"></td><td><img src="../images/led_1.png"></td><td><img src="../images/led_1.png"></td></tr><tr><th><div class="ltext"><div class="sl">R2</div></div></th><th><div class="ltext"><div class="sl">Auto</div></div></th><th><div class="ltext"><div class="sl">PRI</div></div></th></tr></tbody></table></div><table class="tm15" border="0" cellSpacing="0" cellPadding="0" width="70%"><tbody><tr><td><div class="sl">T2</div></td><td><div class="sl">R2</div></td><td><div class="sl">T1</div></td><td><div class="sl">R1</div></td><td><div class="sl">TX</div></td><td><div class="sl">RX</div></td></tr><tr><td><img src="../images/scv1_2.png"></td><td><img src="../images/scv1_2.png"></td><td><img src="../images/scv1_2.png"></td><td><img src="../images/scv1_2.png"></td><td><img src="../images/scv1_2.png"></td><td><img src="../images/scv1_2.png"></td></tr></tbody></table><div class="un"><div class="sl">1+1</div></div></div>
<div class="uf"><img src="../images/bell.gif"></div></div><div style="width: 10px; height: 120px; float: left;"></div><div class="uv1">
<div class="uh"><div class="slot"><div class="sl">2</div></div></div>
<div class="ubd"><div class="un"><div class="sl">NMU</div></div><div class="leds"><table width="100%"><tbody><tr><th><div class="ltext"><div class="sl">SFP1</div></div></th><th><div class="ltext"><div class="sl">FAN</div></div></th><th><div class="ltext"><div class="sl">PWR1</div></div></th></tr><tr><td style="height: 3px;"></td></tr><tr><td><img src="../images/led_3.png"></td><td><img src="../images/led_1.png"></td><td><img src="../images/led_1.png"></td></tr><tr><td><img src="../images/led_3.png"></td><td><img src="../images/led_1.png"></td><td><img src="../images/led_1.png"></td></tr><tr><th><div class="ltext"><div class="sl">SFP2</div></div></th><th><div class="ltext"><div class="sl">RUN</div></div></th><th><div class="ltext"><div class="sl">PWR2</div></div></th></tr></tbody></table></div><table class="tm15 imgw30"><tbody><tr><td><div class="sl">Stack</div></td><td><div class="sl">CON</div></td><td><div class="sl">LAN2</div></td><td><div class="sl">LAN1</div></td><td><div class="sl">SFP2</div></td><td><div class="sl">SFP1</div></td></tr><tr><td><img src="../images/epd2.png"></td><td><img src="../images/epd2.png"></td><td><img src="../images/epu1.png"></td><td><img src="../images/epu2.png"></td><td><img src="../images/sfp2_2.png"></td><td><img src="../images/sfp2_2.png"></td></tr></tbody></table></div><div class="uf"></div></div></div>
<div class="oneUchassis" id="1ub"><div class="vspace"></div><div class="psv"><table height="60"><tbody><tr><td><div class="ps"><table width="70%" align="center"><tbody><tr><td><img src="../images/psfan.png" height="60"></td><td>PWR1<img src="../images/led_1.png"><br><img src="../images/ptype_2.gif" width="60"></td></tr></tbody></table></div><div class="ps"><table width="70%" align="center"><tbody><tr><td><img src="../images/psfan.png" height="60"></td><td>PWR2<img src="../images/led_1.png"><br><img src="../images/ptype_1.gif" width="60"></td></tr></tbody></table></div></td></tr><tr><td><fieldset><legend>侧边风扇</legend><table width="80%" align="center"><tbody><tr><td>FAN1</td><td><img src="../images/fan1.gif" height="40"></td><td>FAN2</td><td><img src="../images/fan1.gif" height="40"></td><td>FAN3</td><td><img src="../images/fan1.gif" height="40"></td><td>FAN4</td><td><img src="../images/fan1.gif" height="40"></td></tr></tbody></table></fieldset></tr></tbody></table></div><div class="vspace"></div><div class="puv1">
<div class="uh"><div class="slot"><div class="sl">3</div></div></div><div class="ubd"></div><div class="uf"></div></div></div></td>
</tr>
</tbody></table>
</div>
</div>
</body>
</html>
