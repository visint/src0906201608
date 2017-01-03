#include "../wsIntrn.h"
#include "../../include/unit.h"

int aspOtapInfo(webs_t wp, int slot,int uinfo,int utype,int group)
{
 short *power=(short*)getInfoSetValues(0,slot,TAP_POWER_INFOSET_ID,1);
 int nBytes=0,n,port_count;
 //printf("run into aspOtapInfo...\r\n");
 nBytes=websWrite(wp, "var power=new Array(new Array(),new Array());\r\n");
 if (utype==1)
    port_count=10;
 else if (utype==6)
    port_count=11;
 else
    port_count=13;
 if (power!=NULL)
 {
   for (n=0;n<port_count;n++)
   {
      nBytes+=websWrite(wp, T("power[0][%d]=%d;"),n,*power);
      power++;
   }
 }

 power=(short*)getInfoSetValues(0,slot,TAP_POWER_INFOSET_ID,2);
 if (power!=NULL)
 {
   for (n=0;n<port_count;n++)
   {
      nBytes+=websWrite(wp, T("power[1][%d]=%d;"),n,*power);
      power++;
   }
 }
   return nBytes;
}
int aspOtapView(webs_t wp, int slot,uchar_t cclass,uchar_t uclass,int otu_type,char show_property)
{
   int nBytes;
   char unit_name[6];
   char *pStatus=getInfoSetValues(0,slot,OTU_STATUS_INFOSET_ID,1);
   char led_A,led_B;
   if (pStatus!=NULL)
   {
    if (*(pStatus+28)!=2)
      led_A=1;
    else
      led_A=2;
   if (*(pStatus+29)!=2)
      led_B=1;
   else
      led_B=2;
  }
  else
  {
  led_A=1;
  led_B=1;
  }
   nBytes=websWrite(wp, "\n<div class='ubd1'><div class='un'>");
   if (otu_type!=5)
   {
    if (otu_type==1)
      strcpy(unit_name,"OTAP");
    else
      strcpy(unit_name,"MON");
    nBytes=websWrite(wp, T("<a href='/goform/slot?cid=0&uid=%d' target='mf'>%s</a>"),slot,unit_name);
    strcpy(unit_name,"otap");
   }
   else
   {
    strcpy(unit_name,"OSC");
    nBytes=websWrite(wp, "OSC");
   }
   nBytes=websWrite(wp,"</div>\n");
  // printf("Run in aspOtapView:%s\n",unit_name);
  // nBytes=websWrite(wp, T("\n<div class='ubd1'>\
<div class='un'><a href='/goform/slot?cid=0&uid=%d' target='mf'>%s</a></div>\n"),slot,unit_name);

   nBytes=websWrite(wp, "<div><img style='width:60px;background:url(images/bd1.png);' src='images/%s_v_led%d_%d.png'/></div>",strTolower(unit_name),led_A,led_B);
   nBytes=websWrite(wp, "<div style='margin:0;padding:0;boder:0;background:url(images/bd1.png);'><img style='margin-top:-2px;boder:0;width:60px;height:220px;background:url(images/bd1.png);' src='images/");
if (otu_type!=5)
   nBytes=websWrite(wp, T("otap_v_port_%d.png"),otu_type);
else
   nBytes=websWrite(wp, "osc_v_port_1.png");
nBytes=websWrite(wp, "'/></div></div>");
}
