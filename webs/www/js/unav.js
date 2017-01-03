function showUnitNav(menu,page,un,ut,cp,biurl,helpurl)
{
  var n,uname,bi,help;
  //uname=un.toLowerCase();
  
  document.write("<ul id='nav'>");
  document.write("<li><span>"+un);
  if (ut!="")
    document.write("(" +ut +")=>");
  document.write("</span></li>");
  if (biurl==null)
  {
    bi=un.toLowerCase();
    if (bi=="oeo")
     bi="otu";
    else if (bi=="obp")
     bi="olp";
  }
  else
  	  bi=biurl;
  //else if (ut==5 && uname=="osc")
  //	uname="otap";
  //alert(bi);
  document.write("<li><a href='"+bi+"bi.asp'");
  if (cp=="bi")
     document.write(" class='current'");
  document.write(">基本信息</a></li>");
  for (n=0;n<menu.length;n++)
  {
  	document.write("<li><a href='"+page[n] + ".asp'");
    if (cp==page[n])
       document.write(" class='current'");
    document.write(">" + menu[n] + "</a></li>");
  }

  if (helpurl==null)
  {
  	  help=un.toLowerCase();
  }
  else
  	 help=helpurl;
  if (help!=null && help!="")
    document.write("<li><a href='/help/"+ help + ".htm' target='new'>帮助</a></li>");
  document.write("</ul>");
}