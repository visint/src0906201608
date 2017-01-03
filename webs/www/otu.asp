<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
    <meta charset="utf-8">
    <script src="js/public.js" type="text/javascript"></script>
    <script src="js/dw.js" type="text/javascript"></script>
    <script src="js/unav.js" type="text/javascript"></script>
    <script src="js/unit.js" type="text/javascript"></script>
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
  //alert("/goform/slot?cid=0&uid=" +document.getElementById("uid").value +"&page=" +p);
  location.href="/goform/slot?cid=0&uid=" +document.getElementById("uid").value;// +"&page=" +p;   
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
<%Privilege();%>
<%UnitInfo(3,0,1,0);%>
</script>
<body class="yui3-skin-sam">
<script language="javascript">
showOtuNav(uc,ut,"otu");
</script>
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
        <div class="ibox">
        <script type="text/javascript">
        if (uc==3)
         dw("OTU");
        else if (uc==10)
         dw("OEO");
        </script>
        信息</div>
         <div class="hbox">
        <!-- <%UnitInfo(3,1,1,1);%>-->
        <script type="text/javascript">
         if (uc==3 ||uc==10)
         {
         	  dw("<table align='center' class='dt'>");
	          showOtuInfo(uc,ut,p,privilege);
	          dw("</table>");
	       }
	       else
	       {
		      dw("单元盘不存在或不匹配");
	       }
        </script>
        </div>
         <div>
         <input class="button white medium" type="button" value="刷新" onclick="javascript:toUrl('otu');" name="refresh"/>
        </div>
        </div>         
    </div>
</div>
</div>
</body>
</html>


