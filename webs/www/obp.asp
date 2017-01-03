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
 <title>OBP 基本信息</title>
</head>
<script language="javascript">
var editable=false,timer = "";
function toUrl(p)   
{ 
  location.href="/goform/slot?cid=0&uid=" +obpForm.uid.value +"&page=" +p;   
} 

function valid(i){
var msg="",minv=0,maxv=100;
msg=document.getElementById("id"+i).innerText;
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

function checkPower()
{
	var i,obj;
	for (i=4;i<7;i++)
  {
  	msg=document.getElementById("id"+i).innerText;
  	obj=document.getElementById("id"+i);
  	if (checkFloatRange(obj,-50, 50,msg)!=true)
  	  return false;
  }
  return true;
}
function checkAll()
{
  return checkPower();
}
function app()
{
  if (checkAll())
  {
  	if (confirm("是否真的要修改?"))
  	{
  		var i,obj,val,url="/goform/setinfo?cid=0&slot="+document.getElementById("uid").value +"&index=1"+ "&infosetId=65";
  		for (i=1;i<20;i++)
  		{
  			obj=document.getElementById("id"+i);
  			if (obj!=null)
  			{
  				val=obj.value;
  				if (i>3 && i<7)
  				  val=Math.round(10*val);
  				url=url+"&id" + i +"="+val;
  			}
  		}
  		alert(url);
  		location.href=url;
  		//alert("/goform/setinfo?cid=0&slot="+document.getElementById("uid").value +"&index=1"+ "&infosetId=65" + "&id4="+Math.round(10*document.getElementById("id4").value) + "&id5="+10*document.getElementById("id5").value+ "&id12="+document.getElementById("id12").value+"&page=obp");
  		// location.href="/goform/setinfo?cid=0&slot="+document.getElementById("uid").value +"&index=1"+ "&infosetId=65" + "&id4="+Math.round(10*document.getElementById("id4").value) + "&id5="+10*document.getElementById("id5").value+ "&id8="+document.getElementById("id8").value+ "&id15="+document.getElementById("id15").value+"&page=obp";
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
timer=setTimeout("refreshPage();",5000);
}
</script>
<body class="yui3-skin-sam">

<ul id="nav">
    <li><span>OBP=></span></li>
    <li><a href="#" id="base" name="navitem" onclick="javascript:toUrl('obpbi');">基本信息</a></li>
    <li><a href="#" id="obp" name="navitem" class="current" onclick="javascript:toUrl('obp');">OBP 状态信息</a></li>
   <!-- <li><a href="#" id="obpt" name="navitem" onclick="javascript:toUrl('obpt');">OBP 告警阈值</a></li>-->
   <li><a href="../help/olp.htm" target="new">帮助</a></li>
</ul>
<div id="content">
<div class="yui3-g" id="bd">
<input type="hidden" id="pid" name="current_page" value="obp.asp">
<input type="hidden" id="cid" name="cid" value="1"/>
<input type="hidden" id="uid" name="uid" value="<%UnitID();%>"/>
 <div class="yui3-u" id="main" style="padding-top:5px;align-text:center;">
        <div id=obp class="cbox" align='center' style="padding-top:5px;">
        <div class="ibox">第<%UnitID();%>槽--OBP信息</div>
        <div id="ibox" class="ibox">OBP信息</div>
         <div class="hbox">
         <form name="obpForm" method="POST" action="/goform/setinfoset" onsubmit="return app();">
          <input type="hidden" id="pid" name="page" value="obp">
          <input type="hidden" id="cid" name="cid" value="0"/>
          <input type="hidden" id="act" name="act" value="1"/>
          <input type="hidden" id="uid" name="uid" value="<%UnitID();%>"/>
      
         <script language="javascript">
         	<%UnitInfo(4,51,1,1);%>
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
         		dw(F("<tr><th align='center'>R1 光功率</th><td>{0}dBm</td></tr>",r1p));
         		dw(F("<tr><th align='center'>R2 光功率</th><td>{0}dBm</td></tr>",r2p));
         		dw(F("<tr><th align='center'>TX 光功率</th><td>{0}dBm</td></tr>",txp));

         		dw(F("<tr><th align='center'>L1切换功率</th><td><input type='text' id='id4' size='5' maxlength='5' value='{0}'/>dBm</td></tr>",l1p));
         		dw(F("<tr><th align='center'>L2切换功率</th><td><input type='text' id='id5' size='5' maxlength='5' value='{0}'/>dBm</td></tr>",l2p));
         		dw(F("<tr><th align='center'>TX告警功率</th><td><input type='text' id='id6' size='5' maxlength='5' value='{0}'/>dBm</td></tr>",txap));
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
            dw("<tr><th align='center'>线路</th><td>");
         		dw("<select id='id9'>");
         		dw("<option value='1'");
         		if (1==line)
              dw(" selected");
            dw(">旁路");
         		dw("<option value='2'");
         		if (2==line)
              dw(" selected");
            dw(">保护线路");
            dw("</select></td></tr>");
            dw("<tr><th align='center'>切换规则</th><td>");
         		dw("<select id='id9'>");
         		dw("<option value='1'");
         		if (1==rule)
              dw(" selected");
            dw(">工作光纤优先");
         		dw("<option value='2'");
         		if (2==rule)
              dw(" selected");
            dw(">光功率大优先");
            dw("</select></td></tr>");
            dw("<tr><th align='center'>R1波长</th><td>");
         		dw("<select id='id15'>");
         		dw("<option value='1'");
         		if (1==r1wl)
              dw(" selected");
            dw(">1310");
         		dw("<option value='2'");
         		if (2==r1wl)
              dw(" selected");
            dw(">1550");
            dw("</select></td></tr>");
            dw("<tr><th align='center'>R2波长</th><td>");
         		dw("<select id='id16'>");
         		dw("<option value='1'");
         		if (1==r2wl)
              dw(" selected");
            dw(">1310");
         		dw("<option value='2'");
         		if (2==r2wl)
              dw(" selected");
            dw(">1550");
            dw("</select></td></tr>");
            dw("<tr><th align='center'>Tx波长</th><td>");
         		dw("<select id='id17'>");
         		dw("<option value='1'");
         		if (1==txwl)
              dw(" selected");
            dw(">1310");
         		dw("<option value='2'");
         		if (2==txwl)
              dw(" selected");
            dw(">1550");
            dw("</select></td></tr>");
            dw("</table>");
         	}
         	else
         		dw("该槽位板卡不存在或不匹配");
         </script>
        </form>
         </script>
        </div>
       <div>
         <input class="button white medium" type="button" value="刷新" onclick="javascript:toUrl('obp');" id="refresh"/>
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


