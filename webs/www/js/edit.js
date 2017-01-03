function disableAllControls(disable)
{ var inputs=document.getElementsByTagName("INPUT"); 
	var selects=document.getElementsByTagName("SELECT");
	//var cbs=document.getElementsByTagName("CHECKBOX");
	if(inputs)
	{ 
		for(var i=0;i <inputs.length;i++ ){
			if (inputs[i].id=="edit" || inputs[i].name=='refresh')
			 continue;
			if (disable)
			   inputs[i].style.backgroundColor="#C0C0C0";
			else
				  inputs[i].style.backgroundColor="white";
			if (inputs[i].id!="apply")
			{
			inputs[i].readOnly=disable;
			
			}
			else
				  inputs[i].disabled=disable;
		}
	} 
	if(selects){
		 for(var i=0;i <selects.length;i++)
		 {
		 		 selects[i].disabled=disable;
		 }
  }
  /*
  if(cbs!=null){
		 for(var i=0;i <cbs.length;i++){
		 	 cbs[i].disabled=disable;
  }
  */
} 

function disableInputControls(privilege,disable) 
{ 
  var ipt = document.getElementsByTagName("input");
  for(var i=0;i<ipt.length;i++)
  {
  	if (ipt[i].id=="edit" || ipt[i].name=='refresh')
			 continue;
    ipt[i].disabled=disable;
    if (disable)
			   ipt[i].style.backgroundColor="#C0C0C0";
			else
				  ipt[i].style.backgroundColor="white";
   } 
} 

function getItemName(items,val)
{
 var n;
 for (n=0;n<items.length;n++)
 {
 	if (items[n].val==val)
 	 return items[n].name;
 }
 return "";
}
function onCheckNumber(obj,minv,maxv,isInt)
{
	var msg="",o=document.getElementById("T"+obj.id);	
	if (o!=null)
	   msg=o.innerText;
	if (isInt)
	  return checkNumber(obj,minv,maxv,msg);
	else
		return checkFloatRange(obj,minv,maxv,msg);
}
function onCheckInt(obj,minv,maxv)
{
		return onCheckNumber(obj,minv,maxv,true);
}
function onCheckFloat(obj,minv,maxv)
{
	return onCheckNumber(obj,minv,maxv,false);
}
function checkAllInput()
{
	var inputs=document.getElementsByTagName("INPUT");
	for(var i=0;i <inputs.length;i++)
	{
		if (inputs[i].type=="text")
		{
			if (inputs[i].id >0)
			{
				if (inputs[i].id >0 && inputs[i].onblur!=null)
				{
					
				//  alert(inputs[i].onblur);
				// alert(inputs[i].id +":" +inputs[i].value);
				  if (inputs[i].onblur()!=true)
				  {
				  	//alert(inputs[i].id +":" +inputs[i].value);
				  	   return false;
				  }
				}
				}
		}
	}
	return true;
}
function getDecInput(item_id,val,ratio,size, maxlen,data_type,item_event)
{
	var t;
	t= F("<input type='text' id='{0}' value='{1}' ratio='{2}' size='{3}' maxlength='{4}'",item_id,val,ratio,size, maxlen);
	if (item_event!=null)
	   t+=" " +item_event;
	t+="/>";
	return t;
}
function getSelectInput(items,item_id,val,item_event)
{
	var n,items,txt="<select id='" + item_id + "'";
	if (item_event!=null)
	   txt+= " " +item_event;	
	txt+= "/>";
	for (n=0;n<items.length;n++)
  {
   txt+=F("<option value='{0}'",items[n][0]);
 	 if (items[n][0]==val)
 	 txt+=" selected";
 	 txt+="/>"+items[n][1];
  }
  txt+="</select>";
  return txt;
}
function writeItem(items,val,editable,data_type)
{
	if (editable)
	{
	switch (item_type)
	{
		case 0:
		break;
		case 1:
		break;
	}
 }
 else
 {
 	
 }
}
function setEditable()
{
	var item;
	disableAllControls(false);
	item=document.getElementById("edit");
	if (item!=null)
		  item.value="提交";
}
function modify(slot,infoset_id,idx,page,check)
{
	var n,item,val,url;
	
		if (check!=null)
		{
			if (check()!=true)
       return false;
    }
		if (true!=opt_ack(1,""))
     return false;
     disableAllControls(true);
     item=document.getElementById("edit");
		 if (item!=null)
		   item.disabled=true;
		 url="/goform/setinfo?cid=0&slot="+slot + "&infosetId=" + infoset_id + "&index=" +idx;
		 for (n=1;n<32;n++)
		 {
			 item=document.getElementById(n);
			 if (item!=null)
			 {
			 	if (item.attributes["ratio"]!=null && item.attributes["ratio"].nodeValue!=null)
			 	 val=item.value*item.attributes["ratio"].nodeValue;
			 	else
			 		 val=item.value;
			 	url+= "&id"+item.id+"=" +val;
			 }
		 }
		 url+="&page="+page;
		 window.document.body.style.cursor='wait';
		 showWait("ibox",true,"正在提交,请稍后...");
		 //alert(url);
     location.href=url;
}