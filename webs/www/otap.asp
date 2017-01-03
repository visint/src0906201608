<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
    <meta charset="utf-8">
    <script src="js/public.js" type="text/javascript"></script>
    <script src="js/dw.js" type="text/javascript"></script>
    <script src="js/unav.js" type="text/javascript"></script>
    <script src="js/unit.js" type="text/javascript"></script>
    <script src="js/otap.js" type="text/javascript"></script>
    <link rel="stylesheet" href="css/grids-min.css" />
    <link rel="stylesheet" href="css/style.css" type="text/css" />
    <link rel="stylesheet" href="css/cview.css" type="text/css"/>
    <link rel="stylesheet" href="css/button.css" type="text/css"/>
    <title>OTAP 基本信息</title>
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
  //alert("/goform/slot?cid=0&uid=" +document.getElementById("uid").value +"&page=" +p);
  location.href="/goform/slot?cid=0&uid=" +document.getElementById("uid").value;// +"&page=" +p;   
}

function mod_otap(id,isid,port,iid)
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
  //alert("/goform/otap?cid=0&uid=" +document.getElementById("uid").value +"&page=" +"otap.asp" +"&isid=" +isid +"&port=" +port +"&iid=" +iid+"&val=" +val);
 location.href="/goform/otap?cid=0&uid=" +document.getElementById("uid").value +"&page=" +"otapret" +"&isid=" +isid +"&port=" +port +"&iid=" +iid+"&val=" +val;  
}
<%UnitInfo(13,0,1,0);%>
</script>
<body class="yui3-skin-sam">
<script language="javascript">
function showPower()
{
	var n,m,g;
	for (n=0;n<2;n++)
	{
		if (n<1)
		  g="A";
		else
			g="B";
		dw(F("<tr><th>{0}组</th>",g));
		for (m=0;m<power[n].length;m++)
	  {
		  dw(F("<td>{0}</td>",(power[n][m]*0.1).toFixed(1)));
	  }
	  dw("</tr>");
	}
}
showOtapNav(13,ut,"otap");
</script>
<div id="content">
<div class="yui3-g" id="bd">
<input type="hidden" id="pid" name="current_page" value="otap.asp">
<input type="hidden" id="cid" name="cid" value="1"/>
<input type="hidden" id="uid" name="uid" value="<%UnitID();%>"/>
   <div class="yui3-u" id="side" style="padding-top:0;width:90px;">
       <div class="content" style="margin:0;">
            <div id="unitid" class="uh1"><%UnitID();%></div>
            <%UnitView(13);%>
      </div>
<div class="uf1"></div>
 </div>
 <div class="yui3-u" id="main" style="padding-top:5px;align-text:center;">
        <div id=otap class="cbox" align='center' style="padding-top:5px;">
        
        <div class="hbox">
        <script type="text/javascript">
         if (uc==13)
         {
         	  var n;
         	  if (ut!=5)
         	  {
         	  	var pc;
         	  	if (ut==1)
         	  	 pc=8;
         	  	else if (ut==6)
         	  	   pc=10;
         	  	else
         	  		pc=12;
         	  dw("<div class='ibox'>");
         	  if (ut==1)
         	   dw("OTAP");
         	  else
         	  	 dw("MON");
         	  dw(" 输出口光功率(dBm)</div>");
            dw("<table align='center' class='dt'><tr>");

            dw(F("<td td colspan=3></td><td colspan={0}>监控口(输出)</td></tr><tr>",pc));
            
            dw("<td>组号</td><td>接收口</td>");
            if (ut==1)
                dw("<td>网络口</td>");
            for (n=1;n<=pc;n++)
              dw(F("<td>#{0}</td>",n));
            dw("</tr>");
            showPower();
            dw("</table>");
            if (ut==1)
                dw("<div class='ibox'>OTAP 信息</div>");
            else
         	    dw("<div class='ibox'>MON 信息</div>");
         	  }
         	  else
         	  	  dw("<div class='ibox'>OSC 信息</div>");
         	  dw("<table align='center' class='dt'>");
	          showOtapInfo(uc,ut,p);
	          dw("</table>");
	       }
	       else
	       {
		      dw("单元盘不存在或不匹配");
	       }
        </script>
        </div>
         <div>
         <input class="button white medium" type="button" value="刷新" onclick="javascript:toUrl('otap');" name="refresh"/>
        </div>
        </div>         
    </div>
</div>
</div>
</body>
</html>