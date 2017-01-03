function getInfoItemValue(infoset_id, idx,item_id,infoset,ratio,fix)
{
	var n,m,val=1.0;
	for(n=0;n<infoset.length;n++)
	{
		if (infoset[n].id==infoset_id && infoset[n].index==idx)
		{
		  for(m=0;m<infoset[n].items.length;m++)
		  {
			  if (infoset[n].items[m].id==item_id)
			  {
			  	//alert(infoset[n].items[m].val);
			  	if (ratio>0)
			  	{
			  		 val=ratio*infoset[n].items[m].val;
			  		 return val.toFixed(fix);
			  	}
			  	else
			  		return infoset[n].items[m].val;
			  }
		  }
	  }
	}
	return "";
}
function createItem(id, val) {
    var o = new Object;
    o.id = id;
    o.val = val;
    o.getItem = function (id) {
        alert(this.val)
    };
   return o;
}

function createInfoset(infoset_id, idx,items) {
    var o = new Object;
    o.id = infoset_id;
    o.index = idx;
    o.items=items;
   return o;
}