String.format = function(src) {
	if (arguments.length == 0) return null;
	var args = Array.prototype.slice.call(arguments, 1);
	return src.replace(/\{(\d+)\}/g, function(m, i) {
		return args[i];
	});
};

function dw(s) {
	document.write(s);
}

function showLeds(us) {
	var n, m, v1, v2;
	dw("<div class='leds'><table width='100%'><tr>");
	for (n = 2; n > 0; n--)
	dw(String.format("<th><div class='ltext'><div class='sl'>{0}</div></div></th>", us.led.txt[0][n - 1]));
	dw("<th><div class='ltext'><div class='sl'>PWR");
	if (us.uclass == 1) dw("1");
	dw("</div></div></th></tr>");
	if (us.led.colCount == 2) {
		dw("<tr><td style='height:3px;'></td></tr>");
		dw(String.format("<tr><td><img src='images/led_{0}.png'/></td><td><img src='images/led_{1}.png'/></td><td><img src='images/led_", us.led.leds[1][1], us.led.leds[0][1]));
		if (us.uclass != 1) dw("1");
		else dw(String.format("{0}", nmu.ps1));
		dw(".png'/></td></tr>");
		dw(String.format("<tr><td><img src='images/led_{0}.png'/></td><td><img src='images/led_{1}.png'/></td>", us.led.leds[1][2], us.led.leds[0][2]));
		if (us.led.runLed != 0) 
		   dw(String.format("<td><img src='images/led_{0}.png'/></td>", us.led.runLed));
	} else {
		dw(String.format("<tr><td><img src='images/led_{0}.png'/></td><td><img src='images/led_{1}.png'/></td></tr>", us.led.leds[1][0], us.led.leds[0][0]));
		dw(String.format("<tr><td><img src='images/led_{0}.png'/></td><td><img src='images/led_{1}.png'/></td><td><img src='images/led_1.png'/></td></tr>", us.led.leds[1][1], us.led.leds[0][1]));
		dw(String.format("<tr><td><img src='images/led_{0}.png'/></td><td><img src='images/led_{1}.png'/></td>", us.led.leds[1][2], us.led.leds[0][2]));
		if (us.led.runLed != 0)
		  dw(String.format("<td><img src='images/led_{0}.png'/></td>", us.led.runLed));
		dw(String.format("</tr><tr><td><img src='images/led_{0}.png'/></td><td><img src='images/led_{1}.png'/></td>", us.led.leds[1][3], us.led.leds[0][3]));
	}
	dw("</tr><tr>");
	for (n = 2; n > 0; n--)
	dw(String.format("<th><div class='ltext'><div class='sl'>{0}</div></div></th>", us.led.txt[1][n - 1]));
	if (us.led.runLed != 0) dw(String.format("<th><div class='ltext'><div class='sl'>{0}</div></div></th>", us.led.runTxt));
	dw("</tr>");
	dw("</table></div>");
}

function showOdmPorts(ports)
{
		var n,port_count=5;
		for (n = 1; n <5; n++)
		{
				if (ports[0].pl[0][n*2]=="N/A")
				{
					port_count=n;
					break;
				}
		}
		//dw("<table style='margin-top:20px'  border='1' cellpadding='1' cellspacing='1'><tr>");
		dw("<table style='margin-top:20px;margin-right:-25px;'><tr>");
		//dw("<table style='margin-top:20px;'><tr>");
		for (n = port_count-1; n >=0; n--) {
			//dw("<table width='30' border='1' cellpadding='1' cellspacing='1'><tr>");
			//dw(String.format("<th style='padding-left:6px;'><div style='font-size:8px' class='sl'>{0} {1}<br/></div></th>", ports[0].pl[1][n*2+1], ports[0].pl[0][n*2+1]))
			//dw(String.format("<th style='padding-left:6px;'><div style='font-size:8px'class='sl'>{0} {1}<br/></div></th>", ports[0].pl[1][n*2], ports[0].pl[0][n*2]));

      dw(String.format("<th style='padding-left:6px;'><div style='font-size:7px'>{0} {1}<br/></div></th>", ports[0].pl[1][n*2+1], ports[0].pl[0][n*2+1]))
			dw(String.format("<th style='padding-left:6px;'><div style='font-size:7px'>{0} {1}<br/></div></th>", ports[0].pl[1][n*2], ports[0].pl[0][n*2]));
		}
		dw("</tr><tr>");
		for (n = port_count; n >0; n--) {
			dw("<td width='60' colspan='2' style='padding-top:10px;'><img src='images/lch.jpg' width=60'></td>");
		}
		dw("</tr></table>");
}

function showPorts(ports, wi, hi) {
	var n, m, k, s, l;

	for (n = 0; n < ports.length; n++) {
		l = ports[n].pl.length;
		if (ports[n].isTxtAtTwoSide != null) s = ports[n].isTxtAtTwoSide;
		else s = false;

		dw("<table style='font-size:10px;'");
		if (wi != null) dw(String.format(" width='{0}' ", wi));
		if (hi != null) dw(String.format(" height='{0}' ", hi));
		if (ports[n].pclass != null) {
			dw(String.format(" class='{0}'", ports[n].pclass));
		}
		dw(" border='0' cellpadding='0' cellspacing='0'>");
		if (ports[n].plh != null) {
			dw("<tr>");
			for (m = ports[n].count; m > 0; m--) {
				if (cclass!=1)
				   dw(String.format("<th>{0}&nbsp;</th>", ports[n].plh[m - 1]));
        else
				   dw(String.format("<th><div class='sl'>{0}</div></th>", ports[n].plh[m - 1]));
			}
			dw("</tr>");
		}
		dw("<tr>");
		for (m = ports[n].count; m > 0; m--){
			dw("<td>");
			if (cclass==1)
			{
				dw("<div class='");
			if (!s && l > 1)
			   dw("m");
			else
				 dw("s");
			dw(String.format("l'>{0}", ports[n].pl[0][m - 1]));
		 }
		 else
		 {
		 	   if (s != true && l > 1 && ports[n].pl.length>1)
		 	       dw(String.format("{0}", ports[n].pl[1][m - 1]));
		 	   else
		 	       dw(String.format("{0}", ports[n].pl[0][m - 1]));
		 }
			if (s != true && l > 1 && ports[n].pl.length>1)
			{
				if (cclass==1)
				  dw(String.format("<br/>{0}", ports[n].pl[1][m - 1]));
				else
					dw(String.format("&nbsp;&nbsp;&nbsp;{0}<br/><br/>", ports[n].pl[0][m - 1]));
			}
			if (cclass==1)
			dw("</div>");
			dw("</td>");
		}
		dw("</tr>");
		dw("<tr>");
		for (m = ports[n].count; m > 0; m--) {
			if (ports[n].img!="")
			  dw(String.format("<td><img src='images/{0}{1}.png'", ports[n].img, ports[n].pstatus[m - 1]));
			else
				dw(String.format("<td><img src='images/{0}{1}.png'", ports[n].imges[m-1], ports[n].pstatus[m - 1]));
			if (ports[n] != null && ports[n].pi != null) {
				dw(" title='");
				if (null != ports[n].pi && ports[n].pi.val[m - 1] != null) {
					for (k = 0; k < ports[n].pi.tname.length; k++) {
						if (ports[n].pi.val[m - 1][k] != "")
						dw(String.format(" {0}:{1}{2}&#10;", ports[n].pi.tname[k], ports[n].pi.val[m - 1][k], ports[n].pi.unit[k]));
					}
				}
				dw("'");
			}
			dw("/></td>");
		}
		if (s && ports[n].pl.length>1) {
			dw("<tr height='30'>");
			for (m = ports[n].count; m > 0; m--) {
				if (cclass!=1)
				  dw(String.format("<td>{0}</td>", ports[n].pl[1][m - 1]));
				else
					dw(String.format("<td><div class='sl'>{0}</div></td>", ports[n].pl[1][m - 1]));
			}
			dw("</tr>");
		}
		dw("</tr></table>");
	}
}

function showUnit(slot, us, alm) {
	var val = 1;
	var uclass;

	if (us != null)
	{
		if (us.slotCount != null && us.slotCount > 0 && us.slotCount < 5) 
		 val = us.slotCount;
	}
	else
	{
		dw("<div class='puv1'>");
		dw(String.format("<div class='uh'><div class='slot'><div class='sl'><a href='/goform/slot?cid=0&uid={0}&page=bi'>{0}</a></div></div></div><div class='ubd'>", slot));
		dw("<div class='uf'>");
		if (alm != null && alm[slot] == 1) dw(String.format("<a href='/goform/almqry?flag=1&slot={0}'><img src='images/bell.gif'/></a>", slot));
		dw("</div></div></div>");
		return;
	}
  
	dw("<div class='");
	if (us.uclass == 55) 
	  dw(String.format("puv{0}'>", val));
	else dw(String.format("uv{0}'>", val));
	dw("<div class='uh'><div class='slot'><div class='sl'>");
	if (!(us.uclass==1 && slot==3) && us.un != "NET")
	{
	dw("<a href='");
	if (us.uclass != 1) 
	{
		dw(String.format("/goform/slot?cid=0&uid={0}&page=bi'>", slot));
	}
	else dw("sys/nmubi.asp'>");

	dw(String.format("{0}</a>", slot));
  }
  else
  	dw(slot);
  dw("</div></div>");
	if (us.uclass != 55) {
		dw("<div class='un'><div class='sl'><a href='/");
		if (us.uclass != 1) {
			dw("goform/");
			if (us.uclass != 3) dw("slot");
			else dw("unit");
			dw(String.format("?cid=0&uid={0}", slot));
		} else dw("sys/sys_info.asp");
		dw(String.format("'>{0}</a></div></div>", us.un));
	} else dw(String.format("<div class='un'><div class='sl'>{0}</div></div>", us.un));
	
	dw("</div><div class='ubd'>");
  if (us.vicount != null && us.vicount<1)
  {
	if (us.uclass != 55) showLeds(us);
	if (us != null) {
		if (us.topHtmlText != null) 
		  dw(us.topHtmlText);
		if (us.tports != null) 
		    showPorts(us.tports, null, 110);
		if (us.uclass != 1) {
			if (!(us.uclass == 55 && us.utype==12))
			   showPorts(us.ports, us.hi, null);
			else
				  showOdmPorts(us.ports);
		} else showDisPorts(us.ports);
	}

	if (us.btext != null) dw(String.format("<div class='un'><div class='sl'>{0}</div></div>", us.btext));
  }
  else
  {
  	for (val=0;val<us.vicount;val++)
  	   dw(String.format("<img style='float:right;' src='images/{0}.png'/>", us.vimages[val]));
  }
	dw("<div class='uf'>");
	if (alm != null && alm[slot] == 1) dw(String.format("<a href='/goform/almqry?flag=1&slot={0}'><img src='images/bell.gif'/></a>", slot));
	dw("</div>");
	dw("</div></div>");
}

function showPs(ps, fan,l) {
	var n;
	dw("<div class='vspace'></div>");
	dw("<div class='psv'><table height='60'><tr><td>");

	for (n = 0; n < 2; n++) {
		dw("<div class='ps'>");
		if (ps[n] != null) {
			if (ps[n].run != 1) ps[n].run = 3;
			dw(String.format("<table align='center' width='70%'><tr><td ><img height='60' src='images/psfan.png'/></td><td>PWR{0}<img src='images/led_{1}.png'/><br/><img  width='60' src='images/ptype_{2}.gif'/></td></tr></table>", n + 1, ps[n].run, ps[n].ptype));
		}
		dw("</div>");
	}
	dw("</td></tr><tr><td><fieldset><legend>");
	if (l==null || l==1)
	  dw("侧边风扇");
	else
		dw("Side fan");
	dw(String.format("</legend><table align='center' width='80%'><tr><td>FAN1</td><td><img height='40' src='images/fan{0}.gif'/></td><td>FAN2</td><td><img height='40' src='images/fan{1}.gif'/></td><td>FAN3</td><td><img height='40' src='images/fan{2}.gif'/></td><td>FAN4</td><td><img height='40' src='images/fan{3}.gif'/></td></tr></td></tr></table></fieldset></table>", fan[0], fan[1], fan[2], fan[3]));
	dw("</div>");
}

function showDisPorts(p) {
	var n;
	dw("<table ");
	if (p.hi != null) dw(String.format(" width={0}", p.hi));
	if (p.pclass != null) {
		dw(String.format(" class='{0}'", p.pclass));
	}
	dw("><tr>");
	for (n = p.length; n > 0; n--) {
		dw(String.format("<td><div class='sl'>{0}</div></td>", p.pl[0][n - 1]));
	}
	dw("</tr>");
	for (n = p.length; n > 0; n--) {
		dw(String.format("<td><img src='images/{0}{1}.png'/></td>", p.pimg[0][n - 1], p.pstatus[n - 1]));
	}
	dw("</tr></table>");
}

function showNmu(nmu) {
	showLeds(nmu);
}

function showBbuOtu(slot,unit)
{
	var n,state;
	dw("<table width=170px><tr>");

	for (n=1;n<4+1;n++)
	{
		dw("<td>L" +((slot-1)*4+n) +"  C" +((slot-1)*4+n)+"</td>");
	}

  dw("</tr><tr>");
	for (n=1;n<4+1;n++)
	{
		if (unit.ports[0].pstatus[(n-1)*2]==1)
		   state=1;
		else
		   state=0;
		dw("<td><img src='images/led_"+ state+".png'/>");
		if (unit.ports[0].pstatus[(n-1)*2+1]==1)
		   state=1;
		else
		   state=0;
		dw("<img src='images/led_"+ state+".png'/></td>");   
	}
	dw("</tr><tr>");
	for (n=1;n<4+1;n++)
	{
		if (unit.ports[0].pstatus[n*2-1]==1)
		   state=1;
		else if (unit.ports[0].pstatus[n*2-1]==2)
		   state=2;
		else if (unit.ports[0].pstatus[n*2-1]==3)
			state=3;
		else
		   state=0;
		dw("<td><a href='/goform/unit?cid=0&uid="+slot+"'> <img src='images/sfp2_"+ state+".png'/></a></td>");
	}
	dw("</tr></table>")
}

function showChassis(units, nmu, ps, fan, alm,l) {
	dw("<div class='oneUchassis' style='text-align:center;margin:0 auto;'>");
	dw("<div class='oneUR'></div><div class='oneUL'></div><div class='oneUC'>");
	if (cclass!=12)
	{
	showUnit(1, units[0], alm);
	dw("<div style='width:5px;height:100px;float:left;'></div>");
	if (cclass!=11)
	{
	 if (nmu.slot == 2) 
	   showUnit(2, nmu, alm);
	 else 
		 showUnit(2, units[1], alm);
	}
	else
	{
		//showUnit(2, units[1], alm);
	  if (nmu.slot == 3) 
	   showUnit(3, nmu, alm);
	 else
	 	   //showUnit(4, nmu, alm);
		 showUnit(3, units[2], alm);
	}

	dw("</div></div>");
	dw("<div class='oneUchassis' style='text-align:center;margin:0 auto;'>");
	dw("<div class='oneUC' style='width:940px;margin-left:40px;'>");

	dw("<div style='width:-5px;height:100px;float:left;'></div>");
	if (cclass==1)
	{
	  if (nmu.slot != 3) 
	    showUnit(3, null, alm);
	  else 
	  	showUnit(3, nmu, alm);
	   showPs(ps, fan,l);
  }
  if (cclass==11)
  {
  	showUnit(2, units[1], alm);
  	if (nmu.slot == 4) 
	     showUnit(4, nmu, alm);
	  else
	 	   showUnit(4, units[3], alm);
  }
 }
 else
 {
 	   var wd=0;
 	   for (n=0;n<3;n++)
 	   {
 	  	if (units[n]!=null)
 	  	  wd+=180;
 	   }
 	   if (wd<180)
 	      wd=180;
 	  dw("<table width=" +wd +"px height=120px><tr>");
 	  var n;
 	  for (n=0;n<3;n++)
 	  {
 	  	dw("<td>");
 	  	//alert(n+1);
 	  	if (units[n]!=null)
 	  	{
 	  		showBbuOtu(n+1,units[n]);
 	  	}
 	  	dw("</td>");
 	  }
 	  if (wd<480)
 	  {
 	  	dw("<td><DIV style='width:"+(480-wd)+"px;'>&nbsp;</DIV></td>");
 	  }
 	  dw("<td>");
 	  showUnit(4, nmu, alm);
 	  dw("</td>");
 	  dw("</tr></table>");
 }
 dw("</div></div>");
}