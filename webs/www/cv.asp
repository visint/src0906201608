<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<link rel="stylesheet" href="css/style.css" type="text/css" />
<%Include(1);%>
<script src="js/cookie.js" type="text/javascript"></script>
<base target="mf"/>
<title>Chaisiss View</title>
</head>
<style type="text/css">
body{
}
A:link {color:black; text-decoration: none}
A:visited {color:black; text-decoration: none}
</style>
<script language="javascript">
function setAlm()
{
 var alm = document.getElementById("severity");
 if (null!=alm)
 {
   alm = document.getElementById("dalm");
   alm.innerHTML="<embed src='" + "alarm1.wav" +"' hidden=true autostart='false' loop=\"" + "true"+ "\"/></embed>";
   alert(alm.innerHTML);
 }
}

window.onload=function(){
var poll=0;

var t=cf_GetCookie("rtime");
if(t!=null)
{
  if (t>0)
   poll=t;
}
else
   poll=20;
if (poll>0)
{
   setTimeout("self.location.reload();",1000*poll);
}
}
</script>
<body>
<div id="cv">
<table width="100%">
<tr>
<!--<td width="37" border="0"  background="images/lhanger.png" ></td>
<td>-->
<%ChassisView();%>
<!--</td>
</td><td width="37" border="0"  background="images/rhanger.png"/></td>-->
</tr>
</table>
</div>
</body>
</html>
