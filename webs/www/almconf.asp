<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<script src="../js/checknum.js" type="text/javascript"></script>
<script src="../js/edit.js" type="text/javascript"></script>
<link rel="stylesheet" href="css/style.css" type="text/css" />
<link rel="stylesheet" href="css/button.css" type="text/css"/>
<title>告警配置管理</title>
</head>
<body>
<style type="text/css">
td {text-align:left;}
img {border-width:0px 0px 0px 0px;}
</style>
<script language="javascript">
<%Privilege();%>
<%AlarmConf();%>
function submit()   
{ 
  var n;
  var url="/goform/almconf?save=";
  var unit;

  //url+="&issave="+document.getElementById("issave").value;
  //url+="&save="+document.getElementById("severity").value;
  
  if (document.getElementById("issave").checked)
  {
   if (document.getElementById("save1").checked)
   {
   	   url+="1" + "&maxacount="+document.getElementById("max_acount1").value;
   	   if (document.getElementById("showcount").checked)
   	     url+="&showcount=Y";
   	   else
   	   	   url+="&showcount=N";
   }
   else
       url+="2" + "&max_acount="+document.getElementById("max_acount2").value;
  }
  else
  	    url+="0";
  url+="&max_rowcount="+document.getElementById("max_rowcount").value;	    
 //window.location.href(url);
 //alert(url);
 window.location.href(url);
 return false;
}

function check(flag) 
{
  var ena;
  var save1,save2,save_detail;
 
 if (flag==0)
 {
  ena=document.getElementById("issave").checked;
  save1=document.getElementById("save1");
  save2=document.getElementById("save2");
  //save_detail=document.getElementById("savedetail");
  if (ena!=true)
  {
    save1.disabled=true;
    save2.disabled=true;
    //save_detail.disabled=true;
    document.getElementById("max_acount1").disabled=true;
    document.getElementById("max_acount2").disabled=true;
  }
  else
  {
     save1.disabled=false;
     save2.disabled=false;
    // save_detail.disabled=false;
    if (save1.checked==true)
    {
       document.getElementById("max_acount1").disabled=false;
       document.getElementById("max_acount2").disabled=true;
    }
    else
    {
       document.getElementById("max_acount1").disabled=true;
       document.getElementById("max_acount2").disabled=false;
    }
  }
}
else
	{
		ena=document.getElementById("showcount");
		if (flag==2)
		{
			  ena.disabled=true;
			  document.getElementById("max_acount1").disabled=true;
        document.getElementById("max_acount2").disabled=false;
     }
		else
			{
			 ena.disabled=false;
			 document.getElementById("max_acount1").disabled=false;
       document.getElementById("max_acount2").disabled=true;
     }
	}
}
window.onload=function(){
var issave;
if (privilege<2)
{
	disableInputControls(privilege,true);
	return;
}
issave=document.getElementById("issave");

if (save_flag!=0)
{
	issave.checked=true;
	//issave=document.getElementById("savedetail");
	if (save_flag==2)
	{
		 // issave.checked=true;
		  document.getElementById("max_acount2").value=max_acount;
		  document.getElementById("save2").checked=true;
	}
	else
	{
		//issave.checked=false;
		document.getElementById("max_acount1").value=max_acount;
		document.getElementById("save1").checked=true;
	}
}
else
	issave.checked=false;

document.getElementById("showcount").checked=show_count;
document.getElementById("max_rowcount").value=max_rowcount;
check(0);
check(save_flag);
}
</script>

<ul id="nav">
 <li><span>告警管理=></span></li>
  <li><a href="alarm_list.asp">当前告警</a></li>
  <li><a href="alarm_his.asp">历史告警</a></li>
  <li><a href="alm_type.asp">告警策略</a></li>
  <li><a href="almconf.asp" class="current">告警配置</a></li>
  <li><a href="help/alarm.htm" target="new">帮助</a></li>
</ul>
<div id="content" align="center">
 <input type="hidden" name="operate" value="mod"/>
 <table  border="1" cellpadding="0" cellspacing="0" class="dt">
  <caption>告警配置</caption>
  <tr><th>是否保存告警</th><td><input type='checkbox' id='issave' name='issave' onclick='check(0);'/></td><td></td></tr>
  <tr><th>相同位置相同告警只保存一次</th><td><input type='radio' id='save1' checked=true  name='saveby' onclick='check(1);'/></td><td><input type='textbox' id ='max_acount1' name='max_acount1' value=300 maxlength='3' size='3' onblur='checkNumber(this,10,500,"条数");' size='3'/>条</td></tr>
  <tr><th>保存每次告警</th><td><input type='radio' id='save2' name='saveby' onclick='check(2);'/></td><td><input type='textbox' id='max_acount2' name='max_acount2' value=600 maxlength='4' size='4' onblur='checkNumber(this,10,1000,"保存每次告警条数");'/>条</td></tr>
  
  <tr><th>显示告警次数</th><td><input type='checkbox' id='showcount' checked=true  name='showcount'/></td><td>
   	
  <tr><th>显示条数/页</th><td></td><td><input type='textbox' value=50 id='max_rowcount' name='max_rowcount' onblur='checkNumber(this,5,100,"条数/页");' maxlength='3' size='3'/>条</td></tr>
 <!--<%AlarmConf();%>-->
</table>
<div align="center">
<script language="javascript">
	if (privilege>1)
	  document.write("<a class='button white medium' href='#' onclick='return submit();'>应用</a>");
</script>
<a class="button white medium" href="almconf.asp">刷新</a>
</div>
  </div>
</div>
</body>
</html>
