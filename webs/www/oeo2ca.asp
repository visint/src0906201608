<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
    <meta charset="utf-8">
    <script src="js/public.js" type="text/javascript"></script>
    <link rel="stylesheet" href="css/grids-min.css" />
    <link rel="stylesheet" href="css/style.css" type="text/css" />
    <link rel="stylesheet" href="css/cview.css" type="text/css"/>
    <link rel="stylesheet" href="css/button.css" type="text/css"/>
    <title>OEO 交叉矩阵</title>
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

function mod_otu2(id,isid,port,iid)
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
  //alert("/goform/otu2?cid=0&uid=" +document.getElementById("uid").value +"&page=" +"otu2.asp" +"&isid=" +isid +"&port=" +port +"&iid=" +iid+"&val=" +val);
 location.href="/goform/oeo2?cid=0&uid=" +document.getElementById("uid").value +"&page=" +"oeo2ca.asp" +"&isid=" +isid +"&port=" +port +"&iid=" +iid+"&val=" +val;  
}
</script>
<body class="yui3-skin-sam">

<ul id="nav">
  <li><span>OEO(2.5G)=></span></li>
  <li><a href="#" id="base" name="navitem" onclick="javascript:toUrl('oeo2bi');">基本信息</a></li>
  <li><a href="#" id="otu2" name="navitem" onclick="javascript:toUrl('oeo2');">OEO 信息</a></li>
  <li><a href="#" id="otu2ca" name="navitem" class="current" onclick="javascript:toUrl('oeo2ca');">交叉矩阵</a></li>
  <li><a href="#" id="ddm" name="navitem" onclick="javascript:toUrl('oeo2ddm');">DDM 信息</a></li>
   <li><a href="#" id="ddmt" name="navitem" onclick="javascript:toUrl('oeo2ddmt');">DDM 告警阈值</a></li>
  <li><a href="../help/oeo.htm" target="new">帮助</a></li>
</ul>
<div id="content">
<div class="yui3-g" id="bd">
<input type="hidden" id="pid" name="current_page" value="oeo2ca.asp">
<input type="hidden" id="cid" name="cid" value="1"/>
<input type="hidden" id="uid" name="uid" value="<%UnitID();%>"/>
   <div class="yui3-u" id="side" style="padding-top:0;width:90px;">
       <div class="content" style="margin:0;">
            <div id="unitid" class="uh1"><%UnitID();%></div>
            <%UnitView(10);%>
      </div>
<div class="uf1"></div>
 </div>
 <div class="yui3-u" id="main" style="padding-top:5px;align-text:center;">
      <div id=otu2 class="cbox" align='center' style="padding-top:5px;">
        <div class="ibox">OTU 交叉矩阵</div>
         <div class="hbox">
         <form  name="otuca" method="POST" action="/goform/otuconf">
         	<input type="hidden" id="pid" name="page" value="oeo2ca">
          <input type="hidden" id="cid" name="cid" value="0"/>
          <input type="hidden" id="act" name="act" value="1"/>
          <input type="hidden" id="uid" name="uid" value="<%UnitID();%>"/>
          <table>
          <tr><td>
         	<%UnitInfo(10,2,2,1);%>
         	</td></tr>
         <tr><td>
         	<input class="button white medium" type="button" value="刷新" onclick="javascript:toUrl('oeo2ca');" name="refresh"/>
          <input class="button white medium" type="submit" value="应用" name="apply" />
         </td></tr>
         </table>
         </form>
        </div>
        </div>         
    </div>
</div>
</div>
</body>
</html>


