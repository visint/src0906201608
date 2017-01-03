#include <sys/sysinfo.h>
#include <time.h>
#include "../wsIntrn.h"
#include "../../include/unit.h"
#include <visipc.h>
#include <sqlite3.h>
#include <dbutil.h>
#include <log.h>
#include <etu.h>
#include "../unitwebs.h"

boolean_t getEtuWebInfo(uchar_t chassis,uchar_t cclass,uchar_t slot,uchar_t uclass,uchar_t utype,web_unit_t *punit)
{
  punit->uclass=UC_ETU;
  punit->slotCount=1;
  strcpy(punit->height,"60%");
  strcpy(punit->led.run_text,"ETU");
  //info->port_groups[0].isTxtAtTwoSide=0;
  strcpy(punit->port_groups[0].pclass,"tm15 imgw30 imgh25");
  strcpy(punit->port_groups[0].pimg,"");
  
  punit->port_groups[0].port_count=4;
  //printf("getEtuWebInfo\n");
  return TRUE;
}

int aspEtuStatus(webs_t wp,uchar_t slot)
{
  int nBytes=0;
  uchar_t n,utype=1;
  etuStatus_t *etu;
   //printf("Edfa on slot %d\n",slot );

  slot=getSelectedUnit();
  etu=getEtuStatus(0,slot);

  if (NULL==etu)
    return 0;
  nBytes=websWrite(wp, T("var slot=%d, uc=%d, ut=%d;"),slot,getUnitClass(0,slot),utype);
  for(n=0;n<2;n++)
  {
    nBytes=websWrite(wp, T("etu[%d].speed=%d;etu[%d].tpLink=%d;etu[%d].duplex=%d;etu[%d].fpLink=%d;"),n,etu->tp_speed,n,etu->tp_link_status,n,etu->tp_duplex,n,etu->fp_link_status);
   etu++;
  }
   return nBytes;
}

int aspEtuView(webs_t wp, int slot,uchar_t cclass,uchar_t uclass,uchar_t utype)
{
  uchar_t n,speed[2]={1,1},tp_link[2]={1,1},duplex[2]={1,1},fp_link[2]={1,1};
  int nBytes=0;
  //printf("in aspEtuView slot=%\n",slot);
  
  etuStatus_t *etu;
   //printf("Edfa on slot %d\n",slot );
   etu=getEtuStatus(0,slot);
  etuInfo_t *etuInfo=getEtuInfo(0,slot);
  if (NULL==etuInfo)
    return 0;

  //if (etuInfo->lan1_link_status!=lan1_speed
   if (NULL==etu)
   {
     return 0;
   }
  for(n=0;n<2;n++)
  {
    if (etu->tp_speed!=1)
        speed[n]=3;
    if (etu->tp_link_status!=1)
        tp_link[n]=3;
    if (etu->tp_duplex!=1)
        duplex[n]=3;
    etu++;
  }
 
  if (etuInfo->sfp1_exist==1)
  {
    if (etuInfo->sfp1_link_status==1)
       fp_link[0]=1;
    else
      fp_link[0]=0;      
  }
  else
     fp_link[0]=0;
  if (etuInfo->sfp2_exist==1)
  {
    if (etuInfo->sfp2_link_status==1)
        fp_link[1]=1;
    else
      fp_link[1]=0;       
  }
  else
     fp_link[1]=0;
  nBytes +=websWrite(wp, T("\n\n<div class='ubd1'>\
<div class='un'><a href='/goform/slot?cid=0&uid=%d' target='mf'>ETU</a></div>\n\
<div class='leds'> <div class='vp'><span>PWR<img style='margin-left:2px;' src='images/led_1.png' /></span> </div>"),slot);

for(n=0;n<2;n++)
{
nBytes +=websWrite(wp, T("<div class='vp'><span>%d<img  src='images/led_%d.png' /><img src='images/led_%d.png' /><img src='images/led_%d.png' /><img src='images/led_%d.png' /></span></div>"),n+1,speed[n],tp_link[n],duplex[n],fp_link[n]);
}
/*
<div class='vp'><span>2<img  src='images/led_0.png' /><img src='images/led_0.png' /><img src='images/led_0.png' /><img src='images/led_0.png' /></span></div>1000 TL TFD FL</div>\n"));
*/
nBytes +=websWrite(wp,"1000 TL TFD FL</div>\n");
nBytes +=websWrite(wp, "<table height='180' style=\"font-size:10px;\">");

 for(n=0;n<2;n++)
  {
    if (speed[n]!=1)
        speed[n]=2;
    if (tp_link[n]!=1)
        tp_link[n]=2;
    if (duplex[n]!=1)
        duplex[n]=2;
  }
if (etuInfo->sfp1_exist==1)
{
    if (etuInfo->sfp1_link_status==1)
        fp_link[0]=1;
    else
        fp_link[0]=2;      
}
else
     fp_link[0]=0;
if (etuInfo->sfp2_exist==1)
{
    if (etuInfo->sfp2_link_status==1)
        fp_link[1]=1;
    else
       fp_link[1]=2;      
}
else
     fp_link[1]=0;

for(n=0;n<2;n++)
{
nBytes +=websWrite(wp, T("<tr><td style=\"padding-left:3px;\">LAN%d</td><td><img width='22' src='/images/epl_%d.gif'/></td></tr>"),n+1,tp_link[n]);
nBytes +=websWrite(wp, T("<tr><td style=\"padding-left:3px;\">SFP%d</td><td><img width='22px' height='30px' src='/images/p2_%d.gif'/></td></tr>"),n+1,fp_link[n]);
etu++;
}
nBytes +=websWrite(wp, "</table>");
nBytes +=websWrite(wp, "</div>");
  return nBytes;
}
/*
static int aspEtuInfo(int eid, webs_t wp, int argc, char_t **argv)
{
  int nBytes=0,slot;
  uchar_t n,utype=1;
  etuStatus_t *etu;
   //printf("Edfa on slot %d\n",slot );

  slot=getSelectedUnit();
  etu=getEtuStatus(0,slot);
  if (NULL==etu)
    return 0;
  nBytes=websWrite(wp, T("var slot=%d, utype=%d;"),slot,utype);
  for(n=0;n<2;n++)
  {
    nBytes=websWrite(wp, T("etu[%d].speed=%d;etu[%d].tpLink=%d;etu[%d].duplex=%d;etu[%d].fpLink=%d;"),n,etu->tp_speed,n,etu->tp_slink,n,etu->tp_duplex,n,etu->fp_link);
  }
   return nBytes;
}
*/
