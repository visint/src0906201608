<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
 <script src="js/checknum.js" type="text/javascript"></script>
 <script src="js/checkfloat.js" type="text/javascript"></script>
 <script src="js/validate.js" type="text/javascript"></script>
 <script src="js/opt.js" type="text/javascript"></script>
 <script src="js/dw.js" type="text/javascript"></script>
 <script src="js/msg.js" type="text/javascript"></script>
 <script src="js/infoset.js" type="text/javascript"></script>
 <script src="js/edit.js" type="text/javascript"></script>
 <script src="js/table.js" type="text/javascript"></script>
 
 <link rel="stylesheet" href="css/grids-min.css" />
 <link rel="stylesheet" href="css/cview.css" type="text/css"/>
 <link rel="stylesheet" href="css/style.css" type="text/css" />
 <link rel="stylesheet" href="css/button.css" type="text/css"/>
 <title>DVOA 基本信息</title>
</head>
<script language="javascript">
var editable=false,timer = "";
function toUrl(p)   
{ 
  location.href="/goform/slot?cid=0&uid=" +dvoaForm.uid.value +"&page=" +p;   
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

function checkLoss(obj)
{
  return checkFloatRange(obj,0, 50,"插损");
}
function checkAll()
{
  //var ret;
  var obj=document.getElementById("id5");
  return  checkLoss(obj);
}
function app()
{
  if (checkAll())
  {
  	if (confirm("是否真的要修改?"))
  	{
  		//alert("/goform/setinfo?cid=0&slot="+document.getElementById("uid").value +"&index=1"+ "&infosetId=65" + "&id4="+Math.round(10*document.getElementById("id4").value) + "&id5="+10*document.getElementById("id5").value+ "&id12="+document.getElementById("id12").value+"&page=dvoa");
  		 location.href="/goform/setinfo?cid=0&slot="+document.getElementById("uid").value +"&index=1"+ "&infosetId=65" + "&id4="+Math.round(10*document.getElementById("id4").value) + "&id5="+10*document.getElementById("id5").value+ "&id8="+document.getElementById("id8").value+"&page=dvoa";
  		return true;
  	}
  	else
  		return false;
  }
  return false;
}
function edit()
{
	clearTimeout(timer);
	if (!editable)
	{
		 editable=true;
		 setEditable();
	}
	else
		 app();
}
window.onload=function(){
showWait("ibox",false,"");
disableAllControls(true);
showWait("ibox",false,"");
timer=setTimeout("refreshPage();",3000);
}
</script>
<body class="yui3-skin-sam">

<ul id="nav">
    <li><span>DVOA=></span></li>
    <li><a href="#" id="base" name="navitem" onclick="javascript:toUrl('dvoabi');">基本信息</a></li>
    <li><a href="#" id="dvoa" name="navitem" class="current" onclick="javascript:toUrl('dvoa');">DVOA 状态信息</a></li>
   <!-- <li><a href="#" id="dvoat" name="navitem" onclick="javascript:toUrl('dvoat');">DVOA 告警阈值</a></li>-->
   <li><a href="../help/dvoa.htm" target="new">帮助</a></li>
</ul>
<div id="content">
<div class="yui3-g" id="bd">
<input type="hidden" id="pid" name="current_page" value="dvoa.asp">
<input type="hidden" id="cid" name="cid" value="1"/>
<input type="hidden" id="uid" name="uid" value="<%UnitID();%>"/>
 <!--   <div class="yui3-u" id="side" style="padding-top:0;width:90px;">
      <div class="content" style="margin:0;">
          <div id="unitid" class="uh1"><%UnitID();%></div>
           <%UnitView(11);%>
      </div>

<div class="uf1"></div>
 </div>
 -->
 <div class="yui3-u" id="main" style="padding-top:5px;align-text:center;">
        <div id=dvoa class="cbox" align='center' style="padding-top:5px;">
        <div class="ibox">第<%UnitID();%>槽--DVOA信息</div>
        <div id="ibox" class="ibox">DVOA信息</div>
         <div class="hbox">
         <form name="dvoaForm" method="POST" action="/goform/setinfoset" onsubmit="return app();">
          <input type="hidden" id="pid" name="page" value="dvoa">
          <input type="hidden" id="cid" name="cid" value="0"/>
          <input type="hidden" id="act" name="act" value="1"/>
          <input type="hidden" id="uid" name="uid" value="<%UnitID();%>"/>

         
         <script language="javascript">
         	<%UnitInfo(11,1,1,1);%>
         	if (utype>0)
         	{
         		//var t=[["IN 光功率",getInfoItemValue(65, 1,1,infoset,0.1,1)],["OUT 光功率",getInfoItemValue(65,1,2,infoset,0.1,1)],["设置输出光功率",getDecInput(4,getInfoItemValue(65, 1,4,infoset,0.1,1),10,3,5,2,true)+"dBm"],["设置插损",getDecInput(5,getInfoItemValue(65, 1,5,infoset,0.1,1),10,3,5,2,true)+"dBm"],["工作模式",getSelectInput([[1,"自动"],[2,"手动"]],8,getInfoItemValue(65, 1,8,infoset,0,0),true)]];
         		//var t=[["IN 光功率",0.1*ipower],["OUT 光功率",0.1*opower],["设置输出光功率",getDecInput(4,getInfoItemValue(65, 1,4,infoset,0.1,1),10,3,5,2,true)+"dBm"],["设置插损",getDecInput(5,getInfoItemValue(65, 1,5,infoset,0.1,1),10,3,5,2,true)+"dBm"],["工作模式",getSelectInput([[1,"自动"],[2,"手动"]],8,getInfoItemValue(65, 1,8,infoset,0,0),true)]];
            /*var t2=new Array(3);
            t2[0]=[["IN 光功率"],[ipower],["OUT 光功率"],[opower]];
            t2[1]=[["设置输出光功率"],[getDecInput(4,spower,10,3,5,2,true)+"dBm"],["设置插损"],[getDecInput(5,iloss,10,3,5,2,true)+"dBm"]];
            t2[2]=[["工作模式"],[getSelectInput([[1,"自动"],[2,"手动"]],8,mode,true)],[""],[""]];
           */
         		dw("<table class='dt'>");
         		dw(F("<tr><th align='center'>IN 光功率</th><td>{0}dBm</td></tr>",ipower));
         		dw(F("<tr><th align='center'>OUT 光功率</th><td>{0}dBm</td></tr>",opower));
         		dw(F("<tr><th align='center'>设置输出光功率</th><td><input type='text' id='id4' size='5' maxlength='5' value='{0}'/>dBm</td></tr>",spower));
         		dw(F("<tr><th align='center'>设置插损</th><td><input type='text' id='id5' size='5' maxlength='5' value='{0}'/>dB</td></tr>",iloss));
         		dw("<tr><th align='center'>工作模式</th><td>");
         		dw("<select id='id8'>");
         		dw("<option value='1'");
         		if (1==mode)
              dw(" selected");
            dw(">自动");
         		dw("<option value='2'");
         		if (2==mode)
              dw(" selected");
            dw(">手动");
            dw("</select></td></tr>");
            dw("<tr><th align='center'>锁定状态</th><td>");
            if (1==lock)
                dw("锁定");
            else if (2==lock)
            	  dw("输入光功率偏高");
            else if (3==lock)
            	  dw("输入光功率偏低");
         		dw("</td></tr></table>");
         	}
         	else
         		dw("该槽位板卡不存在或不匹配");
         	/*
         	else if (uclass<1)
         	{
         		dw("该槽位板卡不存在");
         	}
         	else if (uclass<1)
         	{
         		dw("该槽位板卡不匹配");
         	}
         	else
         	{
         		dw("信息集参数有误");
         	}
         	*/
         </script>
        </form>
         </script>
        </div>
       <div>
         <input class="button white medium" type="button" value="刷新" onclick="javascript:toUrl('dvoa');" name="refresh"/>
         <script language="javascript">
         if (utype>0)
          dw("<input class='button white medium' type='button' id='edit' value='修改' onclick='javascript:edit();'/>");
         </script>
       </div>
        </div>    
    </div>
</div>
</div>
</body>
</html>


