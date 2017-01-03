<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<script src="js/checknum.js" type="text/javascript"></script>
<script src="js/opt.js" type="text/javascript"></script>
<link rel="stylesheet" href="css/style.css" type="text/css" />
<link rel="stylesheet" href="css/cview.css" type="text/css" />
<link rel="stylesheet" href="css/button.css" type="text/css"/>
<title>OLP 基本信息</title>
</head>
<script language="javascript">
function toUrl(p)   
{ 
  //alert(olpForm.uid.value);
  location.href="/unit?+cid=0&uid=" +olpForm.uid.value +"&page=" +p;   
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
if (i>8 && i<12)
{
  minv=-50;
  maxv=20;
}
else if (i==5)
{
  minv=0;
  maxv=999;
}

return checkNumber(obj,minv,maxv,msg);
/*
if(parseInt(obj.value)==obj.value && parseInt(obj.value)>=0 && parseInt(obj.value)<1000)
{}
else
  alert('请输入数字,范围为0-999');
*/
}
function apply()
{
  return opt_ack(1,"OLP 信号");
}
window.onload=function(){
 checkmode();
 check();
}
</script>
<body>
<ul id="nav">
  <li><span>OLP=></span></li>
  <li><a href="#" id="base" name="navitem" onclick="javascript:toUrl('olpbi');">基本信息</a></li>
  <li><a href="#" id="olp" name="navitem" class="current" onclick="javascript:toUrl('olp');">OLP 信息</a></li>
  <li><a href="../help/olp.htm" target="new">帮助</a></li>
</ul>
<div id="content">
<form name="olpForm" method="POST" action="/goform/olp" onsubmit="return apply();">
<input type="hidden" id="pid" name="page" value="olp">
<input type="hidden" id="cid" name="cid" value="1"/>
<input type="hidden" id="uid" name="uid" value="<%UnitID();%>"/>
<table width="100%" align="center" cellpadding="0" cellspacing="0">
    <tr>
    <td width="100px">
     <table align="center" cellpadding="0" cellspacing="0" class="ut">
      <tr>
       <th scope="col" id="unitid" class="uh"><%UnitID();%></th>
      </tr>
      <tr>
    	<td><%UnitView(4);%></td>
        </tr>
      <td class="uf"></td>
      </tr>
     </table>
    </td>
   <td valign="top" class="vst10">
   <table border="1" align="center" cellpadding="0" cellspacing="0" class="dt vst10">
   <thead>
   <tr>
     <th colspan="6">OLP 信号</th>
   </tr>
   </thead>
   <%UnitInfo(4,1);%>
   <tr>
     <td align="center" colspan="6">
          <input class="button gray medium" type="button" value="刷新" onclick="window.location.href('olp.asp');" name="refresh"/><input class="button gray medium" type="submit" value="应用" name="apply" />
     </td>
   </tr>
   </table>   
   </td>
  </tr>
</table>
</form>
</div>
</body>
</html>
