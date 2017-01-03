<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<script src="../js/checknum.js" type="text/javascript"></script>
<script src="../js/checkip.js" type="text/javascript"></script>
<script src="js/dw.js" type="text/javascript"></script>
<script src="js/error.js" type="text/javascript"></script>
<script src="js/table.js" type="text/javascript"></script>
<link rel="stylesheet" href="../css/style.css" type="text/css" />
<link rel="stylesheet" href="../css/button.css" type="text/css" />
<title>Keep alive Host</title>
</head>
<style type="text/css">
img {border-width:0px;}
.imgl {float:left;}
.imgr {float:right;padding:0; margin:0;}
</style>
<script type="text/javascript">
function check_addhost() {  
if(document.getElementById('addhost').checked==true) 
{
   document.getElementById('tname0').disabled=false;
   document.getElementById('destIp0').disabled=false;
   document.getElementById('ena0').disabled=false;
   document.getElementById('udp0').disabled=false;
   document.getElementById('addbtn').disabled=false;
   document.getElementById('destIp0').style.background="#FFFFFF";
   document.getElementById('tname0').style.background="#FFFFFF";
   document.getElementById('udp0').style.background="#FFFFFF";
}
else{
   document.getElementById('tname0').disabled=true;
   document.getElementById('destIp0').disabled=true;
   document.getElementById('udp0').disabled=true;
   document.getElementById('ena0').disabled=true;
   document.getElementById('addbtn').disabled=true;
   document.getElementById('tname0').style.background="#C0C0C0";
   document.getElementById('destIp0').style.background="#C0C0C0";
   document.getElementById('udp0').style.background="#C0C0C0";
  }
}
function ackdel()
{
  if(confirm("Are you sure to delete this Keep alive Host?")==false)
    return false;
  return true;
}
function apply(id){
  var ip=document.getElementById('destIp' +id).value;

  if (true!=checkIp(ip,"Keep alive host IP",0))
           return false;
  else
  {
     var url="/goform/dhost?opt=";
     if (true!=checkNumber(document.getElementById('udp' +id),1,65535,"Max port number"))
        return false;
     if (id!=0)
        url+="mod";
     else
        url+="add";
     url+="&destIp=" + document.getElementById('destIp' +id).value + "&udp=" + document.getElementById('udp' +id).value + "&ena=";
     if (document.getElementById('ena' +id).checked==true)
          url+="on";
     else
          url+="off";
     if (id!=0)
         url+="&id=" +document.getElementById('id' +id).value;
      else
      {
        if (document.getElementById('tname0').value=="")
        {
           alert("Keep alive host IP name cannot be empty!");
           return false;
        }
        url+="&id=0&tname=" +document.getElementById('tname0').value;
      }
     //alert(url);
     window.location.href(url);
     return true;
  }
}

window.onload=function(){check_addhost();}
</script>
<body>
<ul id="nav">
  <li><span>SNMP=></span></li>
  <li><a href="snmpconf.asp">SNMP Community Configure</a></li>
   <li><a href="desthost.asp">Destination Host</a></li>
  <li><a href="keepalive.asp" class="current">Keep alive Host</a></li>
  <li><a href="snmptest.htm">SNMP Test</a>
  <!--<li><a href="help/snmp.htm" target="new">帮助</a></li>-->
</ul>
<div id="content" align="center">
<table border="1" cellpadding="0" cellspacing="0" class="dt">
  <thead>
  <tr>
   <th colspan="4">Keep alive Host</th>
  </tr>
  </thead>
  <tr class="toptitle">
    <th>Keep alive Host Name</th>
    <th>Keep alive Host IP</th>
    <th>UDP port</th>
    <th>Interval(Second)</th>
    <th>Enable/Disable</th>
    <th>Operate</th>
  </tr>
  <script language="javascript">
  <%DestHostList(2);%>
  if (item!=null)
   writeTableBody(item,null);
  </script>
  <tr>
    <td><input id="tname0" name="tname0" maxlength='15' size="15" value=""/></td>
    <td><input id="destIp0" name="destIp0" maxlength='15' size="15" value=""/></td>
    <td><input id="udp0" name="udp0" maxlength='5' size="6" value="9163"  onkeyup="uIntInput(this);"/></td>
    <td><input id="val0" name="val0" maxlength='5' size="6" value="20"  onkeyup="uIntInput(this);"/></td>
    <td><INPUT TYPE="checkbox" id="ena0" checked  VALUE="on">Enable</td>
    <td><INPUT TYPE="checkbox" id ="addhost" VALUE="on" onclick="javascript:check_addhost();">Add&nbsp;&nbsp;
    <a href='#'><img src='../images/pen.gif' id="addbtn" onclick="apply(0);" /></a></td>
  </tr>
</table>

</div>
</body>
</html>
