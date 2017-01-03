<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
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
function valid(obj,p,i){
var msg="",minv=0,maxv=100;
if (p>0)
 msg="泵浦" +p;
else
	msg="模块告警阈值:";
msg=msg + document.getElementById("T"+i).innerText;
if (5==i)
{
  minv=-40;
  maxv=50;
}
if (6==i)
{
  minv=0;
  maxv=85;
}
else if (i==1)
{
  minv=-40;
  maxv=10;
}
else if (i==2)
{
  minv=-20;
  maxv=30;
}
else if (i==3)
{
  minv=10;
  maxv=500;
}
else if (i==4)
{
  minv=200;
  maxv=999;
}
return checkNumber(obj,minv,maxv,msg);
}
/*
function checkPower(obj,i)
{
  var t;
  if (4==i)
     t="L1切换功率";
  else if (5==i)
     t="L2切换功率";
  else if (6==i)
     t="Tx告警功率";
  else
       return false;
  return checkFloatRange(obj,-50, 10,t);
}
*/
function checkAll()
{
  var n;
  var ret;
  var o1,o2;
  var v1,v2;
  for (n=0;n<3;n++)
  {
   	o1=document.getElementsByName("mint"+n);
   	o2=document.getElementsByName("maxt"+n);
   	if (o1!=null && o2!=null)
   	{
   		v1=parseInt(o1[0].value);
   		v2=parseInt(o2[0].value);
   		if (v1>=v2)
   		{
   			alert("最小温度要小于最大温度");
   			return false;
   		}
   	}
  }
   //alert("最小电流要小于最大电流");
  for (n=1;n<3;n++)
  {
   	//alert("最小电流要小于最大电流");
   	o1=document.getElementsByName("minc"+n);
   	o2=document.getElementsByName("maxc"+n);
   	//alert("最小电流要小于最大电流");
   	if (o1!=null && o2!=null)
   	{
   		//alert("最小电流要小于最大电流");
   		v1=parseInt(o1[0].value);
   		v2=parseInt(o2[0].value);
   		if (v1>=v2)
   		{
   			alert("最小电流要小于最大电流");
   			return false;
   		}
   	}
  }
   
   //alert("checkAll ok");
   return true; 
}
function app(act)
{
  var ret;
  
  if (1==act)
  {
    if (true!=checkAll())
       return false;
    ret=opt_ack(1,"EDFA 信号");
  }
  else if (2==act)
    ret=opt_ack(0,"恢复缺省设置");
  else
       return false;
  if (false!=ret)
  {
   showWait("ibox",true,"");
   document.body.style.cursor="wait";
   if (2==act)
    {
      //alert("/goform/edfa?act=2&cid=0&uid=" +edfaForm.uid.value);
       window.location.href="/goform/edfat?act=2&cid=0&uid=" +edfaForm.uid.value;
      return true;
    }
  }
  return ret;
}

window.onload=function(){
 showWait("ibox",false,"");
 //checkmode();
 //check();
}
</script>
<body class="yui3-skin-sam">

<ul id="nav">
  <li><span>EDFA=></span></li>

  <li><a href="#" id="base" name="navitem" onclick="javascript:toUrl('edfabi');">基本信息</a></li>
  <li><a href="#" id="edfa" name="navitem" onclick="javascript:toUrl('edfa');">EDFA 状态信息</a></li>
  <li><a href="#" id="edfat" name="navitem" class="current" onclick="javascript:toUrl('edfat');">EDFA 告警阈值</a></li>
  <li><a href="../help/edfa.htm" target="new">帮助</a></li>
</ul>
<div id="content">
<div class="yui3-g" id="bd">
<input type="hidden" id="pid" name="current_page" value="otu.asp">
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
         	<%EDFAT(9,1);%>
document.write("<center>模块告警阈值:</center>");
document.write("<table border='1' align='center' cellpadding='0' cellspacing='0' class='dt'><tr><td id='T1'>最小输入光功率</td>\
<td id='T2'>最小输出光功率</td><td id='T5'>最小温度</td><td id='T6'>最大温度</td></tr>");
document.write("<tr><td><input type='text' name='minip' maxlength='3' size='3' onkeyup=uIntInput(this); \
onblur=valid(this,0,1); value='"
+minipower +"'/>dBm</td><td><input type='text' name='minop' maxlength='3' size='3' \
onkeyup=uIntInput(this); onblur=valid(this,0,2) value='" + minopower +"'/>dBm</td>");

document.write("<td><input type='text' name='mint0' maxlength='3' size='3' onkeyup=uIntInput(this); \
 onblur=valid(this,0,5) value='" 
+ mintemp + "'/>°C</td><td><input type='text' name='maxt0' maxlength='3' size='3' onkeyup=uIntInput(this); onblur=valid(this,0,6) value='" + maxtemp +"'/>°C</td></tr>");

document.write("</table>");
document.write("<br/><center>泵浦告警阈值:</center>");
document.write("<table border='1' align='center' cellpadding='0' cellspacing='0' class='dt'><tr>");
if (pumpcount>1)
document.write("<td>泵浦</td>");
document.write("<td id='T3'>最小电流</td><td id='T4'>最大电流</td><td>最小温度</td><td>最大温度</td></tr>");
for (var n=0;n<pumpcount;n++)
{
 document.write("<tr>");
 if (pumpcount>1)
  document.write("<td>泵浦"+(n+1)+"</td>");
document.write("<td><input type='text' name='minc'+(n+1) maxlength='3' size='3' onkeyup=uIntInput(this); onblur=valid(this," +(n+1) +",3); value='"+pumpt[n].mincurrent +"'/>mA</td>\
<td><input type='text' name='maxc'+(n+1) maxlength='3' size='3' onkeyup=uIntInput(this); onblur=valid(this," +(n+1) +",4); value='" + pumpt[n].maxcurrent +"'/>mA</td>");
document.write("<td><input type='text' name='mint'+(n+1) maxlength='3' size='3' onkeyup=uIntInput(this); onblur=valid(this," +(n+1) +",5); value='" + pumpt[n].mintemp + "'/>°C</td>\
<td><input type='text' name='maxt'+(n+1) maxlength='3' size='3' onkeyup=uIntInput(this); onblur=valid(this," +(n+1) +",6); value='" + pumpt[n].maxtemp +"'/>°C</td></tr>");
}
document.write("</table>");
         </script>
        </div>
         <div>
         <input class="button white medium" type="button" value="刷新" onclick="javascript:toUrl('edfat');" name="refresh"/>
         <input class="button white medium" type="submit" value="应用" name="apply" />
        </div>
        </div>
       </form>     
    </div>
</div>
</div>
</body>
</html>


