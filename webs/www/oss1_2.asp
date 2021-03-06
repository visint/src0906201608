﻿<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
   <title>OSS 通道2信息</title>
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
  location.href="/goform/slot?cid=0&uid=" +ossForm.uid.value +"&page=" +p;   
} 
function checkmode() {  
 if(document.getElementById('mode').checked==true){
    document.getElementById("L1").disabled=true;
    document.getElementById("L2").disabled=true;
  }
  else{
    document.getElementById('L1').disabled=false;  
    document.getElementById('L2').disabled=false; 
  }
}
function check(disabled) {  
 if(document.getElementById('ret').checked==true) 
  {
     document.getElementById('rettime').style.visibility="visible";
  }
  else{
      document.getElementById('rettime').style.visibility="hidden";     
    }
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
function checkAll()
{
  var n;
  var ret;
  for (n=4;n<7;n++)
   {
    if (true!=checkPower(document.getElementById("i"+n),n))
        return false;
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
    ret=opt_ack(1,"OSS 信号");
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
       window.location.href="/goform/olp?act=2&cid=0&uid=" +ossForm.uid.value;
      return true;
    }
  }
  return ret;
}

window.onload=function(){
 showWait("ibox",false,"");
 checkmode();
 check();
}
</script>
<body class="yui3-skin-sam">

<ul id="nav">
    <li><span>OSS=></span></li>
    <li><a href="#" id="base" name="navitem" onclick="javascript:toUrl('ossbi');">基本信息</a></li>
    <li><a href="#" id="oss" name="navitem"  onclick="javascript:toUrl('oss');">OSS 通道1信息</a></li>
    <li><a href="#" id="oss1_2" name="navitem" class="current" onclick="javascript:toUrl('oss1_2');">OSS 通道2信息</a></li>
    <li><a href="../help/oss.htm" target="new">帮助</a></li>
</ul>
<div id="content">
<div class="yui3-g" id="bd">
<input type="hidden" id="pid" name="current_page" value="oss1_2.asp">
<input type="hidden" id="cid" name="cid" value="1"/>
<input type="hidden" id="uid" name="uid" value="<%UnitID();%>"/>
   <div class="yui3-u" id="side" style="padding-top:0;width:90px;">
       <div class="content" style="margin:0;">
            <div id="unitid" class="uh1"><%UnitID();%></div>
            <%UnitView(6);%>
      </div>

<div class="uf1"></div>
 </div>
 <div class="yui3-u" id="main" style="padding-top:5px;align-text:center;">
        <div id=oss class="cbox" align='center' style="padding-top:5px;">
        <div class="ibox">第<%UnitID();%>槽--OSS 通道2信息</div>
        <div id="ibox" class="ibox">OSS通道1信息</div>
         <div class="hbox">
         <form name="ossForm" method="POST" action="/goform/olp" onsubmit="return app(1);">
          <input type="hidden" id="pid" name="page" value="oss1_2">
          <input type="hidden" id="cid" name="cid" value="0"/>
          <input type="hidden" id="act" name="act" value="1"/>
          <input type="hidden" id="uid" name="uid" value="<%UnitID();%>"/>
          <input type="hidden" id="group" name="group" value="2"/>
         <table align='center' class='dt'>
         <%UnitInfo(6,1,1,2);%>
         </table>
        </div>
         <div>
         <input class="button white medium" type="button" value="刷新" onclick="javascript:toUrl('oss1_2');" name="refresh"/><input class="button white medium" type="submit" value="应用" name="apply" />
        </div>
        </div>
       </form>     
    </div>
</div>
</div>
</body>
</html>


