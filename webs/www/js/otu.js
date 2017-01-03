function showOtuNav(uc,ut,cp)
{
 var up,un,menu,page;
 if (ut!=3)
 {
 	menu= new Array(3);
 	page= new Array(3);
 }
 else
 {
 	menu= new Array(2);
 	page= new Array(2);
 }
 if (uc==3)
 {
 	  un="OTU";
 }
 else if (uc==10)
 {
 	  un="OEO";
 }
 else
 	  un="OTAP";
 if (ut==1 || ut==5 ||ut==6 || uc==10 && ut==7)
 {
 	up="10G";
 	menu[1]="DDM 信息";
 	menu[2]="DDM 告警阈值";
 	page[1]="otuddm";
 	page[2]="otuddmt";
 }
 else
 {
 	up="2.5";
 	if (ut==2)
 	{
 	menu[1]="交叉矩阵";
 	menu[2]="DDM 信息";
 	page[1]="otuca";
 	page[2]="otuddm";
 }
 else
 	{
 	menu[1]="DDM 信息";
 	page[1]="otuddm";
 }
 }
 menu[0]=un + " 信息";
 page[0]="otu";
 showUnitNav(menu,page,un,up,cp);
}
function createOtu1Speed()
{
	gnames = new Array(null,"OTN","SDH/SONET","Ethernet","Fiber Channel");
	names = new Array(["10G速率扫描"],["OTU-1e","OTU-2","OTU-2e"],["STM-64/OC-192","STM-64/OC-192+FEC","STM-16/OC-48"],["10GbE","2.5GbE"],["10GFC","10GFC+FEC","8GFC","4GFC","2GFC"]);
	vals=Array([1000],[1104,1070,1109],[995,1179,249],[1031,250],[1051,1131,850,425,212]);
	return createNameValue("速率","(G)",[null,"OTN","SDH/SONET","Ethernet","Fiber Channel"],names,vals);
}
function createTxOpen()
{
	return createNameValue("发送端关断状态","",[""],new Array(["启用","禁用","接收无光自动关闭"]),new Array([1,2,3]));
}
function getOtuTxOpenItem(uc,ut,val)
{
	return getItem(createTxOpen(),val);
}
function createLoopBack()
{
	return createNameValue("自环","",[""],new Array(["自环","非自环","反向激活"]),new Array([1,2,3]));
}
function getOtuLoopBackItem(uc,ut,val)
{
	return getItem(createLoopBack(),val);
}
function createOtu5Speed()
{
	return createNameValue("速率","(G)",[""],new Array(["1G~9.95Gbps","9.95G~11.3Gbps"]),new Array([850,1000]));
}
function createOtu6Speed()
{
	return createNameValue("速率","",[""],new Array(["CPRI","SDH/SONET","ETHERNET","1Gbps~10.3Gbps"]),new Array([983,995,1031,-1]));
}
function getOtuPortSpeedItem(uc,ut,val)
{
	var s;
	if (ut==1)
	{
		return getItemByGroup(createOtu1Speed(),val);
	}
	else if (ut==2)
	{
		 return (val);
	}
	else if (ut==5 ||uc==10 && ut==7)
	{
		 return getItem(createOtu5Speed(),val);
	}
	else if (ut==6)
	{
		 return getItem(createOtu6Speed(),val);
	}
	return "";
}
function showOtuSpeed(uc,ut,val,readOnly)
{
	var group;
	var nv= new Object();
	if (ut==1)
	{
		group=new Array(8);
		group[0]=new Object();
		group[0]
		nv.names=new Array("","","","");
		nv.vals=new Array();
	}
}
function showOtuInfo(uc,ut,ports,privilege)
{
	var n,m,ch=0,o,p=ports,rs=false;
	dw("<tr><th>光口</th><th>传输距离(KM)</th><th>波长(nm)</th>");
	if (ut==1)
	 dw("<th>速率类型</th>");
	dw("<th>当前速率");
	if (ut==2||ut==3)
	 dw("(G)");
	dw("</th><th>工作状态</th> <th>发送端关断状态</th>");
	if (!(ut==2||ut==3))
	  dw("<th>自环</th>");
	if (privilege!=1)
	  dw("<th>修改</th>");
	dw("</tr>");
	var cwl= [1528.77,1529.55,1530.33,1531.12,1531.90,1532.68,1533.47,1534.25,1535.04,1535.82,1536.61,1537.40,1538.19,1538.98,1539.77,1540.56,1541.35,1542.14,1542.94,1543.73,1544.53,1545.32,1546.12,1546.92,1547.72,1548.51,1549.32,1550.12,1550.92,1551.72,1552.52,1553.33,1554.13,1554.94,1555.75,1556.55,1557.36,1558.17,1558.98,1559.79,1560.61,1561.42,1562.23,1563.05,1563.86 ,1564.68,1565.50];
  var wl;
  //alert(ports.length);
	for (n=0;n<ports.length;n++)
	{
		if (n>-1)
		{
		  if (p[n].pn%2!=1)
		  {
		  	if (n>0)
		  	{
		  	 	 if (p[n].pn==(p[n-1].pn+1))
		  	     ch=0;
		  	   else
		  		   ch=1;
		  	}
		  	else
		  	{
		  		
		  		ch=1;
		  	}
		  }
		  else
		  {
		  	
		  	if (n>0)
		  	{
		  	 	 if ((n+1)<ports.length && p[n].pn+1==p[n+1].pn)
		  	     ch=2;
		  	   else
		  		   ch=1;
		  	}
		  	else
		  	{
		  		if (ports.length>1 && p[n].pn+1==p[n+1].pn)
		  		  ch=2;
		  		else
		  		  ch=1;
		  	}  
		  }
		}
		dw(F("<tr><td>{0}</td> <td>{1}</td>",p[n].pn,p[n].d*0.01));
		if (p[n].wl>100)
		 wl=p[n].wl*0.05;
		else
		{
			wl=p[n].wl;
		}
		if (0!=(wl-1270)%20)
		{
			wl=wl.toFixed(2);
		}
		if (p[n].wl>100)
		  dw(F("<td>{0}</td>",wl));
		else
		{
			if (p[n].wl>28)
			   dw(F("<td>C{0}</td>",wl-20));
			else if (p[n].wl>1)
			   dw(F("<td>C{0}</td>",1270+(wl-10)*20));
			else
				dw("<td>850</td>");
		}
		if ((ut==5 || ut==6 || uc==10 && ut==7) && (n+1)<ports.length && (p[n].pn+1)==p[n+1].pn)
		  rs=true;
		else
			rs=false;
		if ((ut==5 || ut==6 || uc==10 && ut==7) &&ch>0)
		{
			dw("<td");

			if (ch>1)
			  dw(" rowspan='2'");
			dw(">");
			dw(getOtuPortSpeedItem(uc,ut,p[n].speed));
		  dw("</td>");
	  }
	  else if (ut==1)
	  {
	  	var st="",s=getOtuPortSpeedItem(uc,ut,p[n].speed);
	  	if (null!=s.gname)
	  	  st=s.gname;
	  	dw(F("<td>{0}</td>",st));
	  	dw(F("<td>{0}</td>",s.iname));
	  }
	  else if (ut==2 ||ut==3)
	  {
	  	dw(F("<td>{0}</td>",0.01*p[n].speed));
	  } 
		dw("<td>");
		if (p[n].ws!=1)
		  dw("不正常");
		else
			dw("正常");
	  dw("</td>");
	  dw("<td>");
		dw(getOtuTxOpenItem(uc,ut,p[n].ts));
	  dw("</td>");
	  if (!(ut==2||ut==3))
	  {
	   if (ut!=1)
	   {
	   if (ch>0)
		 { 
		 	dw("<td");
			if (ch>1)
			  dw(" rowspan='2'");
			dw(">");
			
			dw(getOtuLoopBackItem(uc,ut,p[n].lb));
	    dw("</td>");
		 }
		}
		else
		{
			dw("<td>");
			dw(getOtuLoopBackItem(uc,ut,p[n].lb));
	    dw("</td>");
		}
	 }
	 if (privilege!=1)
	 {
	  dw("<td>");
	    dw(F("<a href='#'><img src='images/Editor.gif' onclick=\"javascript:window.location.href='/goform/ModifyUnitInfo?slot={0}&index={1}&page=modifyOtu.asp';\"/></a>",slot,p[n].pn));
	    dw("</td>");
	  dw("</tr>");
	 }
	}
}
