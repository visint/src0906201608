<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
   <script src="js/dw.js" type="text/javascript"></script>
   <script src="js/infoset.js" type="text/javascript"></script>
   <script src="js/edit.js" type="text/javascript"></script>
   <script src="js/table.js" type="text/javascript"></script>
   <script src="js/checknum.js" type="text/javascript"></script>
   <script src="js/checkfloat.js" type="text/javascript"></script>
   <script src="js/validate.js" type="text/javascript"></script>
   <script src="js/opt.js" type="text/javascript"></script>
   <script src="js/msg.js" type="text/javascript"></script>
   <script src="js/unav.js" type="text/javascript"></script>
   <script src="js/unit.js" type="text/javascript"></script>
   <script src="js/olp.js" type="text/javascript"></script>
   
   <link rel="stylesheet" href="css/grids-min.css" />
   <link rel="stylesheet" href="css/cview.css" type="text/css"/>
   <link rel="stylesheet" href="css/style.css" type="text/css" />
   <link rel="stylesheet" href="css/button.css" type="text/css"/>
   <title>单元盘信息</title>
</head>
<body>
<script language="javascript">
var editable=false,timer = "";
<%Privilege();%>
function toUrl(p)   
{ 
  location.href="/goform/slot?cid=0&uid=" +olpForm.uid.value +"&page=" +p;   
}

function onItemChanged(item)
{
	if (item==null || ut>50)
	 return;
	var line_id;
		line_id="9";
	if(item.value==1){
    document.getElementById(line_id).disabled=true;
 }
 else{
    document.getElementById(line_id).disabled=false;  
 }
}

function app()
{
	if (checkAllInput()!=true)
	{
		  return false;
	}
  modify(document.getElementById("uid").value,65,1,"olp",null);
	return true;
}
function SetEnable(f)
{
	clearTimeout(timer);
	if (!editable)
	{
		 editable=true;
		 setEditable();
		 var iid;
		 if (ut<51)
		     iid="1";
		 else
		 	   iid="10";
		 onItemChanged(document.getElementById(iid));
	}
	else
	{
		editable=false;
	  disableAllControls(true);
	  timer=setTimeout("toUrl('olp');",10000);
	}
}
window.onload=function(){
	
 if (ut>50)
 {
 var tag=document.getElementsByTagName("span");
 for(var i=0;i<tag.length;i++)
 { 
 	var att = tag[i].getAttribute("name");
    if (att == "uname") {
      tag[i].innerText="OBP"
    }
 }
}
 showWait("ibox",false,"");
 disableAllControls(true);
 //disableInputControls(privilege,true);
 timer=setTimeout("toUrl('olp');",10000);
}
var ut=0;
<%UnitInfo(4,1,1,1);%>
if (ut<50)
showOlpNav(ut,"olp");
else
	showOlpNav(ut,"obp");
</script>
<div id="content">
<div class="yui3-g" id="bd">
<input type="hidden" id="pid" name="current_page" value="otu.asp">
<input type="hidden" id="cid" name="cid" value="1"/>
<input type="hidden" id="uid" name="uid" value="<%UnitID();%>"/>
 <div class="yui3-u" id="side" style="padding-top:0;width:90px;">
  <div class="content" style="margin:0;">
     <div id="unitid" class="uh1"><%UnitID();%></div>
     <%UnitView(4);%>
  </div>

<div class="uf1"></div>
 </div>
 <div class="yui3-u" id="main" style="padding-top:5px;align-text:center;">
        <div id=olp class="cbox" align='center' style="padding-top:5px;">
        <div class="ibox">第<%UnitID();%>槽--<span name="uname">OLP</span>信息</div>
        <div id="ibox" class="ibox"><span name="uname">OLP</span>信息</div>
         <div class="hbox">
         <form name="olpForm" method="POST" action="/goform/olp" onsubmit="return app();">
          <input type="hidden" id="pid" name="page" value="olp">
          <input type="hidden" id="cid" name="cid" value="0"/>
          <input type="hidden" id="act" name="act" value="1"/>
          <input type="hidden" id="uid" name="uid" value="<%UnitID();%>"/>
         <table align='center' class='dt' style="text-align:left;">
         <script language="javascript">
         if (ut<51)
          showOlpInfo(olp,ut);
         else
          showObpInfo(obp);
         </script>
         </table>
        </div>
         <div>
         <input class="button white medium" type="button" value="刷新" onclick="javascript:toUrl('olp');" name="refresh"/>
         <!--<input class="button white medium" id='apply' type="button" value="应用" id="apply" name="apply" onclick="app();"/>-->
          <script language="javascript">
          if (privilege>1)
          {
          	dw("<input class='button white medium' id='apply' type='button' value='应用' id='apply' name='apply' onclick='app();'/>");
            dw("<input type='checkbox' id='edit' value='修改' onclick='javascript:SetEnable(this.checked);'/>修改");
          }
         </script>
        </div>
        </div>
       </form>     
    </div>
</div>
</div>
</body>
</html>