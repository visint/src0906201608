String.format = function(src){  
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
function showLeds(us)
{
var n,m,v1,v2;
dw("<div class='leds'><table width='100%'><tr>");
for (n=2;n>0;n--)
   dw(String.format("<th><div class='ltext'><div class='sl'>{0}</div></div></th>",us.led.txt[0][n-1]));
dw("<th><div class='ltext'><div class='sl'>PWR");
if(us.uclass==1)
  dw("1");
dw("</div></div></th></tr>");
if (us.led.colCount==2)
{
	dw("<tr><td style='height:3px;'></td></tr>");
	dw(String.format("<tr><td><img src='images/led_{0}.png'/></td><td><img src='images/led_{1}.png'/></td><td><img src='images/led_",us.led.leds[1][1],us.led.leds[0][1]));
	if(us.uclass!=1)
	  dw("1");
	else
		dw(String.format("{0}",nmu.ps1));
	dw(".png'/></td></tr>");
	dw(String.format("<tr><td><img src='images/led_{0}.png'/></td><td><img src='images/led_{1}.png'/></td>",us.led.leds[1][2],us.led.leds[0][2]));
	if(us.led.runLed!=5)
  dw(String.format("<td><img src='images/led_{0}.png'/></td>",us.led.runLed));
}
else
{
	dw(String.format("<tr><td><img src='images/led_{0}.png'/></td><td><img src='images/led_{1}.png'/></td></tr>",us.led.leds[1][0],us.led.leds[0][0]));
	dw(String.format("<tr><td><img src='images/led_{0}.png'/></td><td><img src='images/led_{1}.png'/></td><td><img src='images/led_1.png'/></td></tr>",us.led.leds[1][1],us.led.leds[0][1]));
	dw(String.format("<tr><td><img src='images/led_{0}.png'/></td><td><img src='images/led_{1}.png'/></td>",us.led.leds[1][2],us.led.leds[0][2]));
	if(us.led.runLed!=5)
	   dw(String.format("<td><img src='images/led_{0}.png'/></td>",us.led.runLed));
	dw(String.format("</tr><tr><td><img src='images/led_{0}.png'/></td><td><img src='images/led_{1}.png'/></td>",us.led.leds[1][3],us.led.leds[0][3]));
}
dw("</tr><tr>")
for (n=2;n>0;n--)
   dw(String.format("<th><div class='ltext'><div class='sl'>{0}</div></div></th>",us.led.txt[1][n-1]));
if(us.led.runLed!=5)
  dw(String.format("<th><div class='ltext'><div class='sl'>{0}</div></div></th>",us.led.runTxt));
dw("</tr>")
dw("</table></div>");
}
function showLeds1(us)
{
var n,m,v1,v2;
dw("<div class='leds'><table width='100%'><tr>");
for (n=2;n>0;n--)
   dw(String.format("<th><div class='ltext'><div class='sl'>{0}</div></div></th>",us.led.txt[0][n-1]));
dw("<th><div class='ltext'><div class='sl'>PWR");
if(us.uclass==1)
  dw("1");
dw("</div></div></th></tr>");
if (us.led.colCount==2)
{
	dw("<tr><td style='height:3px;'></td></tr>");
	dw(String.format("<tr><td><img src='images/led_{0}.png'/></td><td><img src='images/led_{1}.png'/></td><td><img src='images/led_",us.led.leds[1][1],us.led.leds[0][1]));
	if(us.uclass!=1)
	  dw("1");
	else
		dw(String.format("{0}",nmu.ps1));
	dw(".png'/></td></tr>");
	dw(String.format("<tr><td><img src='images/led_{0}.png'/></td><td><img src='images/led_{1}.png'/></td>",us.led.leds[1][2],us.led.leds[0][2]));
	if(us.led.runLed!=5)
  dw(String.format("<td><img src='images/led_{0}.png'/></td>",us.led.runLed));
}
else
{
	dw(String.format("<tr><td><img src='images/led_{0}.png'/></td><td><img src='images/led_{1}.png'/></td></tr>",us.led.leds[1][0],us.led.leds[0][0]));
	dw(String.format("<tr><td><img src='images/led_{0}.png'/></td><td><img src='images/led_{1}.png'/></td><td><img src='images/led_1.png'/></td></tr>",us.led.leds[1][1],us.led.leds[1][0]));
	dw(String.format("<tr><td><img src='images/led_{0}.png'/></td><td><img src='images/led_{1}.png'/></td>",us.led.leds[1][3],us.led.leds[0][0]));
	if(us.led.runLed!=5)
	   dw(String.format("<td><img src='images/led_{0}.png'/></td>",us.led.runLed));
	dw(String.format("</tr><tr><td><img src='images/led_{0}.png'/></td><td><img src='images/led_{1}.png'/></td>",us.led.leds[1][1],us.led.leds[1][0]));
}
dw("</tr><tr>")
for (n=2;n>0;n--)
   dw(String.format("<th><div class='ltext'><div class='sl'>{0}</div></div></th>",us.led.txt[1][n-1]));
if(us.led.runLed!=5)
  dw(String.format("<th><div class='ltext'><div class='sl'>{0}</div></div></th>",us.led.runTxt));
dw("</tr>")
dw("</table></div>");
}
function showLeds2(us)
{
var n,m;
dw("<div class='leds'><table>");
dw("<tr><th colspan='2'><div style='width:22px;'><div class='sl' style='height:20px;font-size:10px;'>PWR1</div></div></th><td><img src='images/led_1.png'/></td>");
if (us.led.runLed>-1)
   dw(String.format("<td><img src='images/led_{0}.png'/></td><td colspan='2'>{1}</td>",us.led.runLed,us.led.runTxt));
else
	 dw("<td colspan='3'>");
dw("</tr>");
for (n=0;n<2;n++)
{
dw("<tr>");
dw(String.format("<td>{0}</td>",us.led.txt[n][0]));
if (us.led.colCount==2)
   dw("<td></td>");
for (m=0;m<us.led.colCount;m++)
{
 dw(String.format("<td><img src='images/led_{0}.png'/></td>",us.led.leds[n][m]));
}
dw(String.format("<td>{0}</td>",us.led.txt[n][1]));
dw("</tr>");
}

dw("</table></div>");
}

function showPorts(ports,wi,hi)
{
	var n,m,k,s,l;
	
	for (n=0;n<ports.length;n++)
	{
	 l=ports[n].pl.length;
	if (ports[n].isTxtAtTwoSide!=null)
	  s=ports[n].isTxtAtTwoSide;
	else
		s=false;
	
	dw("<table");
	if (wi!=null)
	  dw(String.format(" width='{0}' ",wi));
	if (hi!=null)
	  dw(String.format(" height='{0}' ",hi));
	if (ports[n].pclass!=null)
	{
	 dw(String.format(" class='{0}'",ports[n].pclass));
	}
	dw(" border='0' cellpadding='0' cellspacing='0'>");
	if(ports[n].plh!=null)
	{
		dw("<tr>");
		for (m=ports[n].count;m>0;m--)
	  {
		 dw(String.format("<th><div class='sl'>{0}</div></th>",ports[n].plh[m-1]));
	  }
	  dw("</tr>");
	}
	dw("<tr>");
  for (m=ports[n].count;m>0;m--)
	{
		 dw("<td><div class='");
		 if (!s && l>1)
		  dw("m");
		 else
		 	dw("s");
		 dw(String.format("l'>{0}",ports[n].pl[0][m-1]));
		 if (s!=true && l>1)
		  dw(String.format("<br/>{0}",ports[n].pl[1][m-1]));
		 dw("</div></td>");
	 }
	dw("</tr>");
	dw("<tr>");
	for (m=ports[n].count;m>0;m--)
	{
		dw(String.format("<td><img src='images/{0}{1}.png'",ports[n].img,ports[n].pstatus[m-1]));
		if (ports[n]!=null && ports[n].pi!=null)
		{
			 dw(" title='");
			 if (null!=ports[n].pi && ports[n].pi.val[m-1]!=null)
			 {
			 	for(k=0;k<ports[n].pi.tname.length;k++)
			 	{
			 		if (ports[n].pi.val[m-1][k]!="")
			 		 dw(String.format(" {0}:{1}{2}&#10;",ports[n].pi.tname[k],ports[n].pi.val[m-1][k],ports[n].pi.unit[k]));
			 	}
			 }
			  dw("'");
		}
		dw("/></td>");
	}
	if (s)
	{
		dw("<tr height='30'>");
		for (m=ports[n].count;m>0;m--)
		{
			dw(String.format("<td><div class='sl'>{0}</div></td>",ports[n].pl[1][m-1]));	
		}
		dw("</tr>");
	}
	dw("</tr></table>");
 }
}
function showPorts1(us)
{
 var n,m;
 for (n=0;n<us.port.count;n++)
 {
 dw("<div class='port'>");
dw("<div class='box'>");
dw("<div class='");
if (us.port.isTxtAtTwoSide!=false)
   dw("l");
dw(String.format("text'>{0}",us.port.txt[0][n]));
if (us.port.isTxtAtTwoSide!=true &&us.port.txt[1][n]!="")
 dw(String.format("<BR/>{0}",us.port.txt[1][n]));
dw("</div>");
dw("</div>");
dw(String.format("<img src='images/p2_{0}.gif'/>",us.port.pstatus[n]));
if (us.port.isTxtAtTwoSide!=false)
    dw(String.format("<div class='box'><div class='rtext'>{0}</div></div>",us.port.txt[1][n]));
dw("</div><div class='vs'></div>");
 }
}
function showTable(t)
{
	var n,m;
	dw("<table cellpadding='15' cellspacing='15' width='95%'>");
	for (n=0;n<t.rows;n++)
	{
		dw(String.format("<tr><td>{0}</td>",t.lables[0][n]));
		for (m=0;m<t.cols;m++)
		   dw(String.format("<td><img src='images/{0}'/></td>","p2_1.gif"));
		dw(String.format("<td>{0}</td></tr>",t.lables[1][n]));
	}
	dw("</table>");
}
function showPortsTable(t)
{
	var n,m,k,txtpos=1;
	if (t.txtpos!=null)
	  txtpos=t.txtpos;
	dw(String.format("<div class='ports %s'>",t.pclass));
	for (n=0;n<t.table.length;n++)
	{
	 dw("<table");
	 if (t.style!=null)
	    dw(String.format(" style='{0}'",t.style));
	 dw("><tr>");
	 if(t.plh!=null)
	   dw(String.format("<th>{0}</th>",t.plh[n][0]));
	 dw(String.format("<th>{0}",t.pl[0][0]));
	 if (txtpos==1 && t.pl.length>1)
	   dw(String.format("<br/>{0}",t.pl[1][0]));
	 dw("</th><td");
	 if (t.border>0)
	  dw(" class='tb'");
	 dw("><img");
	 
	 dw("  src='images/p2_1.gif'/></td>");
	 if (txtpos==2 && t.pl.length>1)
	    dw(String.format("<th>{0}</th>",t.pl[1][0]));
	 dw("</tr>");
	 k=1;
	 for (m=1;m<t.table[n].length-1;m++)
	 {
	  dw("<tr>");
	  if(t.plh!=null)
	   dw(String.format("<th>{0}</th>",t.plh[n][m]));
	  dw(String.format("<th>{0}",t.pl[0][k]));
	  if (txtpos==1 && t.pl.length>1)
	    dw(String.format("<br/>{0}",t.pl[1][k]));
	  dw("</th><td");
	  if (t.border>0)
	  dw(" class='mb'");
	  dw("><img src='images/p2_1.gif'/></td>");
	  if (txtpos==2 && t.pl.length>1)
	    dw(String.format("<th>{0}</th>",t.pl[1][k]));
	  dw("</tr>");
	  k++;
   }
   dw("<tr>");
   if(t.plh!=null)
	   dw(String.format("<th>{0}</th>",t.plh[n][m]));
	 dw(String.format("<th>{0}",t.pl[0][k]));
	 if (txtpos==1 && t.pl.length>1)
	   dw(String.format("<br/>{0}",t.pl[1][k]));
	 dw("</th>");
	 dw("<td");
	 if (t.border>0)
	  dw(" class='bb'");
	 dw("><img src='images/p2_1.gif'/></td>");
	 if (txtpos==2 && t.pl.length>1)
	    dw(String.format("<th>{0}</th>",t.pl[1][k]));
	 dw("</tr></table>");
	 //k++;
 }
  dw("</div>");
}

function showUnit(slot,us,alm)
{
 var val=1;
 var uclass;
 
 if (us!=null)
 {
 	  if (us.slotCount!=null && us.slotCount>0 && us.slotCount<5)
 	   val=us.slotCount;
 }
 else
 {
 	 dw("<div class='puv1'>");
 	 dw(String.format("<div class='uh'><a href='/goform/slot?cid=0&uid={0}&page=bi'><div class='slot'><div class='sl'>{0}</div></div></a></div><div class='ubd'>",slot));
 	 dw("</div><div class='uf'></div></div>");
 	 return;
 }

 dw("<div class='");
 if (us.uclass==55)
    dw(String.format("puv{0}'>",val));
 else
 	  dw(String.format("uv{0}'>",val));
 dw("<div class='uh'><div class='slot'><div class='sl'>");
 dw("<a href='");
 if (us.uclass!=1)
   dw(String.format("/goform/slot?cid=0&uid={0}&page=bi'>",slot));
 else
 	 dw("sys/nmubi.asp'>");
 	
 dw(String.format("{0}</a></div></div>",slot));
 
 if (us.uclass!=55)
 {
 	   dw("<div class='un'><div class='sl'><a href='/");
 	   if (us.uclass!=1)
 	   {
 	   	 dw("goform/");
 	     if (us.uclass!=3)
 	         dw("slot");
 	     else
 	     	  dw("unit");
 	     dw(String.format("?cid=0&uid={0}",slot));
 	   } 
 	   else
 	   	    dw("sys/sys_info.asp");
 	   dw(String.format("'>{0}</a></div></div>",us.un));
 }
 else
 	   dw(String.format("<div class='un'><div class='sl'>{0}</div></div>",us.un));
 dw("</div><div class='ubd'>");
 if (us.uclass!=55)
    showLeds(us);
 if (us!=null)
 {
 	 if (us.topHtmlText!=null)
     dw(us.topHtmlText); 
 	 if (us.tports!=null)
 	    showPorts(us.tports,null,110);
 	 if (us.uclass!=1)
 	 {
 	 	  showPorts(us.ports,us.hi);
 	 }
 	 else
 	   showDisPorts(us.ports);
 }
 //showPortsTable(us.ports);
 if(us.btext!=null)
   dw(String.format("<div class='un'><div class='sl'>{0}</div></div>",us.btext));
 dw("</div>");
 dw("<div class='uf'>");
 if (alm!=null && alm[slot]==1)
   dw(String.format("<a href='/goform/almqry?flag=1&slot={0}'><img src='images/bell.gif'/></a>",slot));
 dw("</div></div>");
}
function showPs(ps,fan)
{
	var n;
	//dw("<div style='width:30px;height:120px;float:left;'></div>");
	dw("<div class='vspace'></div>");
	dw("<div class='psv'><table height='60'><tr><td>");
	
	for(n=0;n<2;n++)
	{
		dw("<div class='ps'>");
		if (ps[n]!=null)
		{
			if (ps[n].run!=1)
			  ps[n].run=3;
		  dw(String.format("<table align='center' width='70%'><tr><td ><img height='60' src='images/psfan.png'/></td><td>PWR{0}<img src='images/led_{1}.png'/><br/><img  width='60' src='images/ptype_{2}.gif'/></td></tr></table>",n+1,ps[n].run,ps[n].ptype));
	  }
		dw("</div>");
	}
	//dw("</td></tr></table>");
	dw(String.format("</td></tr><tr><td><fieldset><legend>侧边风扇</legend><table align='center' width='80%'><tr><td>FAN1</td><td><img height='40' src='images/fan{0}.gif'/></td><td>FAN2</td><td><img height='40' src='images/fan{1}.gif'/></td><td>FAN3</td><td><img height='40' src='images/fan{2}.gif'/></td><td>FAN4</td><td><img height='40' src='images/fan{3}.gif'/></td></tr></td></tr></table></fieldset></table>",fan[0],fan[1],fan[2],fan[3]));
	dw("</div>");
}
function showDisPorts(p)
{
	var n;
	dw("<table ");
	if (p.hi!=null)
	   dw(String.format(" width={0}",p.hi));
	if (p.pclass!=null)
	{
	 dw(String.format(" class='{0}'",p.pclass));
	}
	dw("><tr>");
	for (n=p.length;n>0;n--)
	{
		dw(String.format("<td><div class='sl'>{0}</div></td>",p.pl[0][n-1]));
	}
	dw("</tr>");
	for (n=p.length;n>0;n--)
	{
		dw(String.format("<td><img src='images/{0}{1}.png'/></td>",p.pimg[0][n-1],p.pstatus[n-1]));
	}
	dw("</tr></table>");
}
function showNmu(nmu)
{
	showLeds(nmu);
}
function showChassis(units,nmu,ps,fan,alm)
{
dw("<div class='oneUchassis'>");
dw("<div class='oneUR'></div><div class='oneUL'></div><div class='oneUC'>");
showUnit(1,units[0],alm);
dw("<div style='width:5px;height:100px;float:left;'></div>");
if (nmu.slot==2)
   showUnit(2,nmu,alm);
else
  showUnit(2,units[1],alm);
dw("</div></div>");
dw("<div class='oneUchassis'>");
dw("<div class='oneUC' style='width:940px;margin-left:40px;'>");
showPs(ps,fan);
dw("<div style='width:-5px;height:100px;float:left;'></div>");
if (nmu.slot!=3)
  showUnit(3,null,alm);
else
	showUnit(3,nmu,alm);
dw("</div></div>");
}