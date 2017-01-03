<html>
<head>
    <meta http-equiv="content-type" content="text/html; charset=UTF-8" /> 
    <script src="js/dw.js" type="text/javascript"></script>
    <script src="js/unav.js" type="text/javascript"></script>
    <script src="js/unit.js" type="text/javascript"></script>
    <script id="otu" src="js/otu.js" type="text/javascript"></script>
    <script src="js/loadJS.js" type="text/javascript"></script>
    <script src="js/opt.js" type="text/javascript"></script>
    <script src="js/modify.js" type="text/javascript"></script>
    <script type="text/javascript" src="ymPrompt/ymPrompt.js"></script>
    <script src="js/msg.js" type="text/javascript"></script>
    <link rel="stylesheet" href="css/grids-min.css" />
    <link rel="stylesheet" href="css/style.css" type="text/css" />
    <link rel="stylesheet" href="css/cview.css" type="text/css"/>
    <link rel="stylesheet" href="css/button.css" type="text/css"/>
    <link rel="stylesheet" type="text/css" href="ymPrompt/skin/vista/ymPrompt.css" />
    <title>Mofify SFP</title>
</head>
<body class="yui3-skin-sam">
<script language="javascript">
function include(src) {
js = '<script language="javascript" src="' + src + '"><\/script>';
document.write(js);
}

<%UnitInfo(3,0,1,1);%>
if (uc!=13)
{
    //loadJS("otu","js\/otu.js");
	  showOtuNav(uc,ut,"otu");
}
else
{
	loadJS("otu","js\/otap.js");
	showOtapNav(uc,ut,"otap");
}
</script>
<div id="content">
<div id='ibox' style='display:none;'></div>
<table align='center' class='dt' style="border:0;">
<script language="javascript">
var retcode=2,n=0;
var port_status=new Array(1,2,3);
var s;
if (uc!=13)
{
s=new Array(new Object(),new Object(),new Object());
s[0].infosetId=51;
s[0].infoItems=[1];
s[0].vals=[1000];
n++;
}
else
	  s=new Array(new Object(),new Object());
s[n].infosetId=50;
s[n].infoItems=[1];
s[n].vals=[3];
n++
s[n].infosetId=52;
s[n].infoItems=[1];
s[n].vals=[1];

p.infosets=s;
p.idx=1;

function refreshPage()
{
	window.document.body.style.cursor="default";
	//alert("/goform/ModifyUnitInfo?slot="+slot+ "&index="+p.pn+"&page=modifyOtu.asp");
	location.href="/goform/ModifyUnitInfo?slot=" + slot + "&index=" + p.pn + "&page=modifyOtu.asp";
}
function ret()
{
	if (uc!=13)
	  location.href="/goform/unit?cid=0&uid=" + slot;
	else
		location.href="/goform/slot?cid=0&uid=" + slot;
}
function showPortList()
{
	var n;
	if (p.idx==0)
	  return true;
	if (port_status!=null)
	{
		var names= new Array(port_status.length);
		var vals= new Array(port_status.length);
		for (n=0;n<port_status.length;n++)
		{
			names[n]="Port"+port_status[n];
			vals[n]=port_status[n];
		}
		//showComb(names,vals,"pidx",p.idx,"refreshPage()");
		return true;
	}
	else
		return false;
}

function modifyInfo(pn,iid)
{
 var items= [0];
 var vals= new Array();
 var iname="",id=0,o,idx=1;
 if (iid==50)
 {
 	items[0]=((p.pn-1)%4)*4+4;
 	if (uc!=13)
 	  id=2;
 	else id=1;
 	iname="Speed rate";
 	if (pn>4)
 	  idx=2;
 }
 else if (iid==51)
 {
 	iname="Tx enable";
 	id=1;
 	items[0]=(p.pn-1)*3+7;
 }
 else if (iid==52)
 {
 	iname="Loopback";
 	if (uc!=13)
 	 id=3;
 	else
 		id=2;
 	items[0]=(p.pn-1)*4+1;
 }

 id="s"+id;
 o=document.getElementById(id);
 if (o!=null)
 {
 	 vals[0]=o.value;
 	 if (uc!=13)
     setShortInfoItem(slot,uc,ut,idx,iid,items,vals," Port #"+ pn," " +iname,"modifyOtu");
   else
   	 setShortInfoItem(slot,uc,ut,idx,iid,items,vals," Port #"+ pn,"的信息","modifyOtu");
 }
 else
 	  alert(iname+" object is null!");
}
function showUnitInfo(vlist)
{
	if (retcode>0)
	{
		//if (showResult(retcode)!=true)
		  //return;
	}
	var n;
	
  //alert(nv[1].val);
  dw(F("<tr><th>槽位号</th><td colspan='2'>{0}</td></tr><tr><th>单元盘类型</th><td colspan='2'>{1}</td></tr>",slot,getUnitName(uc,ut)));
  if (p.idx>0)
	{
		dw(F("<tr><th>端口号</th><td colspan='2'>{0}",port));
	  if (showPortList())
	   dw("</td><tr>");
	  else
	  {
	  	dw("</td><tr>");
	  	return false;
	  }
  }
	for (n=0;n<vlist.length;n++)
	{
		dw(F("<tr><th>{0}</th><td>",vlist[n].nv.iname));

		//showComb(vlist[n].nv.names,vlist[n].nv.vals,"s"+(n+1),vlist[n].val);
		showCombByGroup(vlist[n].nv,"s"+(n+1),vlist[n].val);
		dw(F("</td><td><a href='#'><img src='images/Editor.gif' onclick=\"modifyInfo({0},{1});\"/></a></td></tr>",p.pn,s[n].infosetId));
	}
}
var vl,names;
if (uc!=13)
{
 if (ut!=2)
 {
	vl = new Array(3);
  names= new Array(3);
 }
 else
 {
	vl = new Array(1);
  names= new Array(1);
 }
}
else
{
	vl = new Array(2);
  names= new Array(2);
}
n=0;
if (uc!=13)
{
vl[0]=new Object();
vl[0].nv=createTxOpen();
vl[0].val=p.ts;
names[0]="Tx enable";
n++;
}
if (uc!=13 &&ut!=2 || uc==13)
{
	names[n]="Speed rate";
	vl[n]=new Object();
	if (uc!=13)
	{
	 if (ut==1)
	   vl[n].nv=createOtu1Speed();
	 else if (ut==5 || ut==7)
		 vl[n].nv=createOtu5Speed();
	 else if (ut==6)
		 vl[n].nv=createOtu6Speed();
	}
	else
		 vl[n].nv=createOtu5Speed();
  vl[n].val=p.speed;
  n++;
  names[n]="Loopback";
  vl[n]=new Object();
  vl[n].nv=createLoopBack();
  //vl[2].val=p.infosets[2].vals[0];
  vl[n].val=p.lb;
}
showUnitInfo(vl);
</script>
<tr>
	<td colspan="3" align="center"  style="padding-top:30px;border:0;">
	<input class="button white medium" type="button" value="刷新" onclick="refreshPage();" name="refresh"/>
	<input class="button white medium" type="button" value="返回" onclick="ret();"/></td>
</tr>
</table>
</div>
</body>
</html>