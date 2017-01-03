/*
function writeTableBody(table,head,colCount)
{
var n,m;
dw("<tr>");
if (head!=null)
{
for (n=0;n<head.length;n++)
{
	dw("<th>" + head[n] +"</th>");
}
dw("</tr>");
}
for (n=0;n<table.length;n++)
{
	dw("<tr>");
	for (m=0;m<table[n].length;m++)
	  dw("<td>" + table[n][m] +"</td>");
	dw("</tr>");
}
}
*/
function writeTableBody(head,values,colCount)
{
var n,m,col=0;
var maxCount,mode=0;
if (head.length>values.length)
   maxCount=values.length;
else
	 maxCount=head.length;
mode=maxCount%colCount;
if ( colCount>1 && mode>0)
   maxCount--;
for (n=0;n<maxCount;n++)
{
	if (col==0)
	  dw("<tr>");
	dw("<th");
	if (head[n][1]>0)
	   dw(" id='T" +head[n][1] +"'");
	dw(">"+head[n][0] +"</th><td>"+values[n]+"</td>");
	if (col==colCount)
	  dw("</tr>");
	if (col<colCount-1)
	   col++;
	else
		 col=0;
}
if (mode>0)
{
	//alert(mode);
	if (maxCount%colCount==0)
	   dw("<tr>");
	dw("<th");
	if (head[n][1]>0)
	   dw(" id='T" +head[n][1] +"'");
	dw(">"+head[n][0] +"</th><td colspan="+ mode*2+1 +">"+values[n]+"</td></tr>");
}
else
	dw("</tr>");
}
function writeTable(table,head)
{

dw("<table class='dt'>");
writeTableBody(table,head);
dw("</table>");
}
function writeTableWithHead(t)
{
	var n,m;
 dw("<table class='dt'><tr>");
 for (n=0;n<t.length;n++)
 {
 	dw("<tr>");
 	for (m=0;m<t[n].length;m++)
 	{
 		if (m%2)
	    dw("<td>" + t[n][m] +"</td>");
	  else
	  	dw("<th>" + t[n][m] +"</th>"); 
	} 
	dw("</tr>");
 }
dw("</table>");
}
