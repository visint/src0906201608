int asp4u_unit_view(webs_t wp,int slot)
{
  int nBytes=0;
  uchart n,m,k;
  unitViewInfo_t info;
  if (getUnitViewInfo(0,slot,&info)!=TRUE)
  {
    return websWrite(wp, T("<div class='ubd1'></div>"));
  }
 //显示4U单元盘视图头及LED灯
  nBytes=websWrite(wp, T("\n<div class='ubd1'>\
<div class='un'><a href='/goform/unit?cid=0&uid=%d' target='mf'>%s</a></div>\n"),slot,info.unit_name);
nBytes +=websWrite(wp, "<table class=\"leds4u\"><tr><td");
if (info.leds.colCount>2)
  nBytes +=websWrite(wp, " colspan=\"2\"");
nBytes +=websWrite(wp, ">PWR</td><td><img src='images/led_1.png' /></td>");
if (info.run_led>-1 && info.run_led<4)
{
 nBytes +=websWrite(wp, T("<td><img src='images/led_%d.png' /></td><td"),run_led);
 if (info.leds.colCount>2)
  nBytes +=websWrite(wp, " colspan=\"2\"");
  nBytes +=websWrite(wp, T(">%s</td>"),info.leds.run_text);
}
nBytes +=websWrite(wp, "</tr>");
if (info.leds.colCount>2)
   k=0;
else
   k=1;
for (n=0;n<2;n++)
{
  nBytes=websWrite(wp, T("<tr><td>%s</td>"),info.leds.leds_text[n][0]);
  for (m=k;m<info.leds.colCount;m++)
  {
    nBytes=websWrite(wp, T("<td><img src='images/led_%d.png' /></td>"),info.leds.leds_status[n][m]);
  }
  nBytes=websWrite(wp, T("<td>%s</td></tr>"),info.leds.leds_text[n][1]);
}
nBytes=websWrite(wp, "</table>");
//显示4U单元盘视图端口
for (n=0;n<info.ports.portCount;n++)
{

}
}
