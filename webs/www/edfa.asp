<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" 
"http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
   <title>EDFA 基本信息</title>
   <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
   <script src="js/checknum.js" type="text/javascript"></script>
   <script src="js/checkfloat.js" type="text/javascript"></script>
   <script src="js/validate.js" type="text/javascript"></script>
   <script src="js/opt.js" type="text/javascript"></script>
   <script src="js/msg.js" type="text/javascript"></script>
   
   <link rel="stylesheet" href="css/grids-min.css" />
   <link rel="stylesheet" href="css/cview.css" type="text/css"/>
   <link rel="stylesheet" href="css/style.css" type="text/css" />
   <link rel="stylesheet" href="css/button.css" type="text/css"/>
</head>
<script language="javascript">
function toUrl(p)   
{ 
  location.href="/goform/slot?cid=0&uid=" +edfaForm.uid.value +"&page=" +p;   
} 

function valid(obj,i){
var msg="",minv=0,maxv=100;
msg=document.getElementById("T"+i).innerText;
if (i>3 && i<7)
{
  minv=-50;
  maxv=20;
}
else if (i==7)
{
  minv=0;
  maxv=999;
}
return checkNumber(obj,minv,maxv,msg);
}
function checkGain(obj)
{
	var ret;
	ret=checkFloatRange(obj,0.5,3.0,"增益步长");
	return ret;
}
function checkPower(obj)
{
  return checkFloatRange(obj,0, 3,"输出光功率");
}
function checkAll()
{
  var ret;
  var obj=document.getElementById("mval");
  //alert(work_mode);
  if (1==work_mode)
  {
  	return checkGain(obj);
  }
  else
  	return  checkPower(obj);
}
function app(act)
{
  return checkAll();
}

window.onload=function(){
 showWait("ibox",false,"");
 //checkmode();
}
</script>
<body class="yui3-skin-sam">

<ul id="nav">
    <li><span>EDFA=></span></li>
    <li><a href="#" id="base" name="navitem" onclick="javascript:toUrl('edfabi');">基本信息</a></li>
    <li><a href="#" id="edfa" name="navitem" class="current" onclick="javascript:toUrl('edfa');">EDFA 状态信息</a></li>
   <!-- <li><a href="#" id="edfat" name="navitem" onclick="javascript:toUrl('edfat');">EDFA 告警阈值</a></li>-->
    <li><a href="../help/edfa.htm" target="new">帮助</a></li>
</ul>
<div id="content">
<div class="yui3-g" id="bd">
<input type="hidden" id="pid" name="current_page" value="edfa.asp">
<input type="hidden" id="cid" name="cid" value="1"/>
<input type="hidden" id="uid" name="uid" value="<%UnitID();%>"/>
   <div class="yui3-u" id="side" style="padding-top:0;width:90px;">
       <div class="content" style="margin:0;">
            <div id="unitid" class="uh1"><%UnitID();%></div>
            <%UnitView(9);%>
      </div>

<div class="uf1"></div>
 </div>
 <div class="yui3-u" id="main" style="padding-top:5px;align-text:center;">
    <div id=edfa class="cbox" align='center' style="padding-top:5px;">
    <div class="ibox">第<%UnitID();%>槽--EDFA信息</div>
    <div id="ibox" class="ibox">EDFA信息</div>
    <div class="hbox">
    <form name="edfaForm" method="POST" action="/goform/edfaConf" onsubmit="return app(1);">
     <input type="hidden" id="pid" name="page" value="edfa">
     <input type="hidden" id="cid" name="cid" value="0"/>
     <input type="hidden" id="act" name="act" value="1"/>
     <input type="hidden" id="uid" name="uid" value="<%UnitID();%>"/>
    <script language="javascript">
   <%UnitInfo(9,1,1,1);%>
document.write("<table class='dt'><tr><td align='center' colspan='3'>");
document.write("模块状态</td></tr><tr><th>输入光功率</th><th>输出光功率</th><th>温度</th></tr>");
document.write("<tr><td>"+ipower+"dBm</td><td>"+opower+"dBm</td><td>"+temp+ "°C</td></tr>");
document.write("</table>");
document.write("<br/><span>工作模式:");
if (1==work_mode)
{
	document.write("AGC");
}
else
	document.write("APC");
document.write("<input type='hidden' name='wm' value='" +work_mode + "'>");
//work_mode=2;
if (1==work_mode)
{
	document.write("<br/>增益:"+val);
	document.write("dB");
	//if (utype!=1)
	// document.write("&nbsp;&nbsp;步长:<input type=text id='mval' name='mval' maxlength='3' size='3' value='0.5' onkeyup='inputNumber(this,1,true);' onblur='checkGain(this);'>");
}
else
{
	document.write("<br/>光功率:");
	/*if (utype!=1)
	document.write("<input type=text id='mval' name='mval'  maxlength=3 size=3 value="+val + " onkeyup='inputNumber(this,1,true);' onblur='checkPower(this);'>");
	else
	*/
  document.write(val);
	document.write("dBm");
}

document.write("</span>");
/*if (utype!=1)
  document.write("<input class='button white medium' type='submit' value='应用' name='apply'>");
*/
document.write("</form><br/>泵浦状态:<br/>");
document.write("<table class='dt'><tr>");
if (pumpcount>1)
document.write("<th>泵浦</th>");
document.write("<th>工作电流</th><th>制冷电流</th><th>功率</th><th>温度</th><th>状态</th></tr>");
for (var n=0;n<pumpcount;n++)
{
 document.write("<form name='edfaPowerForm' method='POST' action='/goform/edfaConf'>");
 document.write("<input type='hidden' name='current_page' value='edfa.asp'>");
 document.write("<input type='hidden' name='cid' value='1'/>");
 document.write("<input type='hidden' name='uid' value='<%UnitID();%>'/>");
 document.write("<input type='hidden' name='act' value='2'/>");
 document.write("<tr>");
 if (pumpcount>1)
  document.write("<td>泵浦"+(n+1)+"</td>");
  document.write("<td>"+pump[n].working_current+"mA</td><td>"+pump[n].cool_current+"mA</td><td>"+pump[n].power+"dBm</td><td>"+pump[n].temperature+"°C</td><td>");
  document.write("<select name='ps" +String(n+1)+ "'>");
  document.write("<option value='1'");
  if (1==pump[n].pump_status)
    document.write(" selected");
  document.write(">打开");
  document.write("<option value='2'");
  if (2==pump[n].pump_status)
    document.write(" selected");
  document.write(">关闭");
  document.write("<input class='button white medium' type='submit' value='应用' name='apply'>");
  document.write("</select></td></tr>");
  document.write("</form>");
}
document.write("</table>");
if (utype==3)
{
	document.write("<br/><table border='1' align='center' cellpadding='0' cellspacing='0' class='dt'> <thead>   <tr>  <th colspan='6'>光口 DDM 信息</th>  </tr>  </thead>  <tr class='toptitle'>  <th>接收光功率</th>  <th>发送光功率</th> <th>温度</th> <th>电压</th> <th>偏置电流</th> </tr>");
	document.write("<tr class='toptitle'>  <th>"+(sfprpower*0.1).toFixed(1) +" dBm</th>  <th>"+(sfptpower*0.1).toFixed(1) +" dBm</th> <th>"+(sfptemp*0.1).toFixed(1) +"℃</th> <th>"+(sfpvolt*0.1).toFixed(1) +"V</th> <th>"+sfpbias +"mA</th> </tr>");
	
	document.write("</table>");
}
 </script>
        </div>
         <div>
         <input class="button white medium" type="button" value="刷新" onclick="javascript:toUrl('edfa');" name="refresh"/>
        </div>
        </div>    
    </div>
</div>
</div>
</body>
</html>


