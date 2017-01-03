function getUnitName(uc,ut)
{
  var name="";
  switch (uc){
  case 3:name="OTU";
  break;
  case 4:name="OLP";
  break;
  case 10:name="OEO";
  break;
  case 13:
      if (ut!=5)
     {
     	 if (ut==1)
     	  name="OTAP";
     	 else
     	 	name="MON";
     }
     else
     	 name="OSC";
  break;
 }
  return name;
}

function getItem(nv,val)
{
	var n,m,g;
	
	g= new Object();
	g.gname="";
	g.iname="";
	for (n=0;n<nv.grp.length;n++)
	{
		for (m=0;m<nv.grp[n].nv.names.length;m++)
		{
		 if (nv.grp[n].nv.vals[m]==val)
		 {
		 	  g.gname=nv.grp[n].nv.gname;
		 	  g.iname=nv.grp[n].nv.names[m];
		 	  return g.iname;
		 }
		}
	}
	return g.iname;
}
function getItemByGroup(nv,val)
{
	var n,m,g;
	
	g= new Object();
	g.gname="";
	g.iname="";
	for (n=0;n<nv.grp.length;n++)
	{
		for (m=0;m<nv.grp[n].nv.names.length;m++)
		{
		 if (nv.grp[n].nv.vals[m]==val)
		 {
		 	  g.gname=nv.grp[n].gname;
		 	  g.iname=nv.grp[n].nv.names[m];
		 	  return g;
		 }
		}
	}
	return g;
}
function ValuleList(names,vals)
{
	this.names=names;
	this.vals=vals;
}
function createNameValue(iname,unit,gnames,vnames,vals)
{
	var n,g,names,vals,nv;
	nv= new Object();
	nv.iname=iname;
	if (null!=unit)
	 nv.unit=unit;
	else
		nv.unit="";
	g=new Array(gnames.length);
	
	//gnames = new Array(null,"OTN","SDH/SONET","Ethernet","Fiber Channel");
	//names = new Array(["10G"],["OTU-1e","OTU-2","OTU-2e"],["STM-64/OC-192","STM-64/OC-192+FEC","STM-16/OC-48"],["10G","2.5G"],["10GFC","10GFC+FEC","8GFC+FEC","4GFC+FEC","2.5GFC+FEC"]);
	//vals=Array([1000],[1104,1070,1109],[995,1179,249],[1031,250],[1051,1131,850,425,212]);
	for (n=0;n<gnames.length;n++)
	{
		g[n]= new Object();
		g[n].gname= gnames[n];
		g[n].nv= new Object();
		g[n].nv.names= vnames[n];
		g[n].nv.vals= vals[n];
	}
	nv.grp=g;
	return nv;
}
function showModify(slot,uc,wi,hi,callback)
{
ymPrompt.win({title:"修改第"+slot+"槽"+getUnitName(uc)+"信息",width: wi,height:hi,fixPosition:true,maxBtn:true,minBtn:true,iframe:{id:'myId',name:'myName',src:'modify.htm'},handler:callback})
}