function showMenu(m)
{
  var menu= new Array("帮助","设备视图","模块列表","系统信息","告警管理","SNMP配置","用户管理","日志管理","系统管理","NMU","OTU","OEO","OLP","OBP","OSS","EDFA","DVOA");
  var page= new Array("index","chassis","sys_info","ml","alarm","snmp","user","log","sys","nmu","otu","oeo","olp","obp","oss","edfa","dvoa");
  var n,k;
  document.write("<ul id='nav'>");
  for (n=0;n<menu.length & n<12;n++)
  {
  	document.write("<li><a href='"+page[n]);
  	if (n!=1)
     document.write(".htm'");
    else
    	 document.write(".asp'");
    if (m==page[n])
       document.write(" class='current'");
    document.write(">"+menu[n]+"</a></li>");
  }
  document.write("</ul>");
  
  document.write("<ul id='nav'>");
  for (k=12;k<menu.length;k++)
  {
  	document.write("<li><a href='"+page[k]);
  	if (k!=1)
     document.write(".htm'");
    else
    	 document.write(".asp'");
    if (m==page[k])
       document.write(" class='current'");
    document.write(">"+menu[k]+"</a></li>");
  }
  document.write("</ul>");
}