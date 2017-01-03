<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
    <meta charset="utf-8">
    <script src="js/public.js" type="text/javascript"></script>
    <script src="js/dw.js" type="text/javascript"></script>
    <script src="js/unav.js" type="text/javascript"></script>
    <script src="js/otu.js" type="text/javascript"></script>
    <link rel="stylesheet" href="css/grids-min.css" />
    <link rel="stylesheet" href="css/style.css" type="text/css" />
    <link rel="stylesheet" href="css/cview.css" type="text/css"/>
    <link rel="stylesheet" href="css/button.css" type="text/css"/>
    <title>OTU 基本信息</title>
</head>
<style type="text/css">
.hbox{display:inline-block;margin:3px}
.yui3-u,.cbox {text-align: center; margin:0px auto; width:100%; } 
.cbox {height:auto;}
body{
	min-width: 800px; width:expression(document.body.clientWidth < 800? "800px": "100%" );
}
</style>
<script language="javascript">
window.onload=function(){
//cf_InitUrlParm();
}
function toUrl(p)   
{ 
  //alert("/goform/unit?cid=0&uid=" +document.getElementById("uid").value +"&page=" +p);
  location.href="/goform/slot?cid=0&uid=" +document.getElementById("uid").value +"&page=" +p;   
}

function mod_otu(id,isid,port,iid)
{
  var val=document.getElementById(id).value;
  if (null==val)
     return -1;
  if ("wm"==id)
  {
     if (true==document.getElementById(id).checked)
        val=1;
     else
        val=2;
  }
  //alert("/goform/otu?cid=0&uid=" +document.getElementById("uid").value +"&page=" +"otu.asp" +"&isid=" +isid +"&port=" +port +"&iid=" +iid+"&val=" +val);
 location.href="/goform/otu?cid=0&uid=" +document.getElementById("uid").value +"&page=" +"oturet" +"&isid=" +isid +"&port=" +port +"&iid=" +iid+"&val=" +val;  
}

var uc=12,ut=2,sp=2;
var o=new Array(3);
var sn=new Array("1G~9.95G","9.95G~11.3");
var sv=new Array(850,1000);
//var iid;
o[0]=new Object();
o[1]=new Object();
o[2]=new Object();
//o[3]=new Object();
o[0].p=1;
o[0].d=10;
o[0].s=850;
o[0].wl=1310;
o[0].ws=1;
o[0].te=1;
o[0].lb=1;

o[1].p=2;
o[1].d=25;
o[1].s=1000;
o[1].wl=1330;
o[1].ws=2;
o[1].te=1;
o[1].lb=1;

o[2].p=3;
o[2].d=10;
o[2].s=850;
o[2].wl=13300;
o[2].ws=2;
o[2].te=2;
o[2].lb=1;
/*
o[3].p=4;
o[3].d=10;
o[3].s=1000;
o[3].wl=13500;
o[3].ws=1;
o[3].te=3;
o[3].lb=2;
*/
showOtuNav(uc,ut,2);
function showOtuInfo()
{
	var n,ch=1;
	for (n=0;n<o.length;n++)
	{
		if (n>0)
		{
			if (Math.ceil((o[n].p)/2)==Math.ceil((o[n-1].p)/2))
		   ch=0;
		  else
		  	ch=1;
		}
		dw(F("<tr><td>{0}</td> <td>{1}</td> <td>{2}</td>",o[n].p,o[n].d,(o[n].wl*0.1).toFixed(2)));
		if (ch>0)
		{ 
			dw("<td rowspan='2'>");
		  showComb(sn,sv,"s"+n,o[n].s);
		  dw("</td>");
		}
		dw("<td>");
		showComb(new Array("正常","不正常"),new Array(1,2),"ws"+n,o[n].ws);

	  dw("</td><td>");
	  showComb(new Array("启用","禁用","发送无光自动关闭"),new Array(1,2,3),"ws"+n,o[n].te);
	  dw("</td>");
	  dw("</td>");
	  if (ch>0)
		{ 
			dw("<td rowspan='2'>");
		   showComb(new Array("自环","非自环"),new Array(1,2),"ws"+n,o[n].lb);
		  dw("</td>");
		}
	  
	  dw("<td><a href='#'><img src='images/Editor.gif'/></a></td></tr>");
	}
}
</script>
<body class="yui3-skin-sam">

<ul id="nav">
  <li><span>OTU(10G)=></span></li>
  <li><a href="#" id="base" name="navitem" onclick="javascript:toUrl('otubi');">基本信息</a></li>
  <li><a href="#" id="otu" name="navitem" class="current" onclick="javascript:toUrl('otu');">OTU 信息</a></li>
  <li><a href="#" id="ddm" name="navitem" onclick="javascript:toUrl('otuddm');">DDM 信息</a></li>
   <li><a href="#" id="ddmt" name="navitem" onclick="javascript:toUrl('otuddmt');">DDM 告警阈值</a></li>
  <li><a href="../help/otu.htm" target="new">帮助</a></li>
</ul>
<div id="content">
<div class="yui3-g" id="bd">
<input type="hidden" id="pid" name="current_page" value="otu.asp">
<input type="hidden" id="cid" name="cid" value="1"/>
<input type="hidden" id="uid" name="uid" value="<%UnitID();%>"/>
   <div class="yui3-u" id="side" style="padding-top:0;width:90px;">
       <div class="content" style="margin:0;">
            <div id="unitid" class="uh1"><%UnitID();%></div>
            <%UnitView(3);%>
      </div>
<div class="uf1"></div>
 </div>
 <div class="yui3-u" id="main" style="padding-top:5px;align-text:center;">
        <div id=otu class="cbox" align='center' style="padding-top:5px;">
        <div class="ibox">OTU信息</div>
         <div class="hbox">
         <%UnitInfo(3,1,1,1);%>
        </div>
         <div>
         <input class="button white medium" type="button" value="刷新" onclick="javascript:toUrl('otu');" name="refresh"/>
        </div>
        </div> 
<table>        
<script language="javascript">
showOtuInfo();
</table>
</script>       
</div>
</div>
</div>
</body>
</html>


