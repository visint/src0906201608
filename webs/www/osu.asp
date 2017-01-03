<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<script src="./js/public.js" type="text/javascript"></script>
<script src="./js/str.js" type="text/javascript"></script>
<link rel="stylesheet" href="css/grids-min.css" />
<link rel="stylesheet" href="css/style.css" type="text/css" />
<link rel="stylesheet" href="css/cview.css" type="text/css" />
<link rel="stylesheet" href="css/button.css" type="text/css"/>
<title>OSU信息</title>
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
}
function toUrl(p)   
{ 
  location.href="/goform/slot?cid=0&uid=" +<%UnitID();%> +"&page=" +p;   
} 
</script>
<body>
<ul id="nav">
  <li><span>槽位信息=></span></li>
  <li><a href="#" id="base" name="navitem" onclick="javascript:toUrl('osubi');">基本信息</a></li>
  <li><a href="#" id="osu" name="navitem" class="current" onclick="javascript:toUrl('osu');">OSU信息</a></li>
  <!--<li><a href="../help/osu.htm" target="new">帮助</a></li>-->
</ul>

<div id="content">
<div class="yui3-g" id="bd">
<input type="hidden" id="pid" name="current_page" value="osu.asp">
<input type="hidden" id="cid" name="cid" value="1"/>
<input type="hidden" id="uid" name="uid" value="<%UnitID();%>"/>
   <div class="yui3-u" id="side" style="padding-top:0;width:90px;">
       <div class="content" style="margin:0;">
            <div id="unitid" class="uh1"><%UnitID();%></div>
            <%UnitView();%>
      </div>
<div class="uf1"></div>
 </div>
 <div class="yui3-u" id="main" style="padding-top:5px;align-text:center;">
        <div id=otu class="cbox" align='center' style="padding-top:5px;">
        <div class="ibox">OSU信息</div>
         <div class="hbox">
         <table border="1" align="center" cellpadding="0" cellspacing="0" class="dt">
   <thead>
   <tr>
     <th colspan="2">OSU信息</th>
   </tr>
   </thead>
    <%PassiveInfo(3,1,1);%>
   <tr>
   <td align="center" colspan="2">
   <input class="button white medium" type="button" value="刷新" onclick="window.location.href('osu.asp');" name="refresh"/>
   </td>
   </tr>
   </table>
        </div>
        </div>         
    </div>
</div>
</div>
</body>
</html>
