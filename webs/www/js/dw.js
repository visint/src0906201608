F = function(src){  
    if (arguments.length == 0) return null;  
    var args = Array.prototype.slice.call(arguments, 1);  
    return src.replace(/\{(\d+)\}/g, function(m, i){  
        return args[i];  
    });  
};
function dw(s)
{
  document.write(s);
}
function showComb1(names,vals,iid,val,onchanged)
{
	var n;
	//alert(val+":"+vals[2]+":"+names.length);
	dw(F("<select id={0}",iid));
	if (onchanged!=null)
	   dw(" onchange='"+onchanged +"'");
	dw(">");
	for (n=0;n<names.length;n++)
	{
		//alert(F("vals[{0}] val={1}",vals[n],val));
		dw(F("<option value='{0}'",vals[n]));
		if (vals[n]==val)
		{   dw(" SELECTED");
		//	alert(val);
		}
		dw(F(">{0}</option>",names[n]));
	}
	dw("</select>");
}

function showComb(groups,iid,val,onchanged)
{
	var n,m;
	//alert(val+":"+vals[2]+":"+names.length);
	if (groups==null)
	return;
	dw(F("<select id={0}",iid));
	if (onchanged!=null)
	   dw(" onchange='"+onchanged +"'");
	dw(">");
	for (n=0;n<groups.length;n++)
	{
		if (groups[n].gname!=null)
		{
			dw("<optgroup label='"+groups[n].gname +"'>");
		}
		for (m=0;n<groups[n].nv.names.length;m++)
		dw(F("<option value='{0}'",groups[n].nv.vals[m]));
		if (groups[n].nv.vals[m]==val)
		{   dw(" SELECTED");
		//	alert(val);
		}
		dw(F(">{0}</option>",groups[n].nv.names[m]));
		if (groups[n].gname!=null)
			dw("</optgroup>");
	}
	dw("</select>");
}

function showCombByGroup(groups,iid,val,onchanged)
{
	var n,m;
	//alert(val+":"+vals[2]+":"+names.length);
	if (groups==null)
	return;
	dw(F("<select id={0}",iid));
	if (onchanged!=null)
	   dw(" onchange='"+onchanged +"'");
	dw(">");
	for (n=0;n<groups.grp.length;n++)
	{
		if (groups.grp[n].gname!="" && groups.grp[n].gname!=null)
		{
			dw("<optgroup label='"+groups.grp[n].gname +"'>");
		}
		for (m=0;m<groups.grp[n].nv.names.length;m++)
		{
			dw(F("<option value='{0}'",groups.grp[n].nv.vals[m]));
		  if (groups.grp[n].nv.vals[m]==val)
		  {   dw(" SELECTED");
		//	alert(val);
		  }
		  dw(F(">{0}</option>",groups.grp[n].nv.names[m]));
	  }
		if (groups.grp[n].gname!="" && groups.grp[n].gname!=null)
			dw("</optgroup>");
	}
	dw("</select>");
}
function getSelectedName(oid)
{
	var n,o=document.getElementById(oid);
	if (null==o)
	  return "";
	return o.options[o.selectedIndex].text;
}