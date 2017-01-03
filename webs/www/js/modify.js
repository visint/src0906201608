/*
function afterSubmit(tp){
			if(tp=='ok'){
				//alert("点击了'确定'按钮!");
				return true;
			}
			if(tp=='cancel'){
			//	alert("点击了'取消'按钮!");
				return false;
			}
			if(tp=='close'){
			//	alert("点击了'关闭'按钮!");
				//closeFn()
			}
			refreshPage();
}
*/
function confirmModify(slot,uc,infosetId,idx,pName,iName,val)
{
  var msg="";
  if (slot>0)
     msg="第"+slot+"槽"+getUnitName(uc);
  if (pName!=null)  
     msg+=pName;
  //if (idx>0)  
  //  msg+=idx;
  if (iName!=null)  
     msg+=iName;
 /* if (val!=null)  
     msg+="为:" +val;
     */
 if(opt_ack(1,msg)!=true)
  return false;
 else
 	return true;
 // alert("/goform/otu?cid=0&uid=" +slot +"&page=" +"otu.asp" +"&isid=" +isid +"&port=" +port +"&iid=" +iid+"&val=" +val);
 //location.href="/goform/otu?cid=0&uid=" +document.getElementById("uid").value +"&page=" +"oturet" +"&isid=" +isid +"&port=" +port +"&iid=" +iid+"&val=" +val;  
}

function setInfoItem(itemType,slot,uc,ut,idx,infosetId,infoItem,val,pName,iName,page)
{
	if (confirmModify(slot,uc,infosetId,idx,pName,iName,val)!=true)
	   return false;
	var url="/goform/setinfo?itype="+itemType+"&uc=" +uc+"&ut=" +ut+"&cid=0&slot=" +slot +"&page=" +page +"&infosetId=" +infosetId+"&index=" +idx +"&id" ;
	if( Object.prototype.toString.call(infoItem) === '[object Array]' )
	{
		var n,items,vals;
		items=infoItem[0];
		vals=val[0];
		for (n=1;n<infoItem.length;n++)
		{
			items+=","+infoItem[n];
			vals+=","+val[n];
		}
		url+=items;
		url+="=" +vals;
	}
	else
	{
		url+=infoItem;
		url+="=" +val;
	}
	//url+="&page=otu";
	//alert(url);
	/*var theBody = document.getElementsByTagName('body')[0]; 
  if (theBody!=null)
     theBody.disabled=true
     */
	//if (window.parent!=null)
	//   window.parent.enable=true;
	//setTimeout("showWait('ibox',true,'操作超时')",5000);
	window.document.body.style.cursor='wait';
	//alert(url);
	window.location.href=url;
	//ymPrompt.win({message:'<br><center>正在提交,请稍后</center>',height:50,titleBar:false,handler:refreshPage});
	//setTimeout(function(){ymPrompt.doHandler('close')},2000);
	//location.href="/goform/setInfoset?itemType="+itemType +"&cid=0&uid=" +slot +"&page=" +"oturet" +"&infosetId=" +infosetId +"&index=" +idx +"&infoItem=" +infoItem+"&val=" +val;
	
}
function setByteInfoItem(slot,uc,ut,idx,infosetId,infoItem,val,pName,iName,page)
{
	setInfoItem(1,slot,uc,ut,idx,infosetId,infoItem,val,pName,iName,page);
	//location.href="/goform/setInfoset?cid=0&uid=" +slot +"&page=" +"oturet" +"&infosetId=" +infosetId +"&index=" +idx +"&infoItem=" +infoItem+"&val=" +val;
}
function setShortInfoItem(slot,uc,ut,idx,infosetId,infoItem,val,pName,iName,page)
{
	setInfoItem(2,slot,uc,ut,idx,infosetId,infoItem,val,pName,iName,page);
	//location.href="/goform/setInfoset?cid=0&uid=" +slot +"&page=" +"oturet" +"&infosetId=" +infosetId +"&index=" +idx +"&infoItem=" +infoItem+"&val=" +val;
}

