#include <string.h>
#include <sys/sysinfo.h>
#include <time.h>
#include "../wsIntrn.h"
#include "../../include/unit.h"
#include <visipc.h>
#include <sqlite3.h>
#include <dbutil.h>
#include <log.h>
#include <chassis.h>
#include <passive.h>
#include <sysinfo.h>
#include "../unitwebs.h"

int aspOtuView(webs_t wp, int slot,uchar_t cclass,uchar_t uclass,int otu_type,char show_property);
int aspOlpView(webs_t wp, uchar_t slot,uchar_t cclass,uchar_t uclass,uchar_t utype);
int aspEdfaView(webs_t wp, int slot,uchar_t cclass,uchar_t uclass,int edfa_type);

char getNmuAndPowerSupplyStatus(uchar_t cclass,char *fan_led,char sfp_ports[2],char sfp_leds[2],char lan_ports[2],char ps_status[2],char *ps_type)
{
  char n;
  chassis_t *pChassis;  
  nmuStatus_t *nmu_status;
  
  pChassis=getChassisInfo(0);
  //printf("SlotOfNmu:%d\n",getSlotOfNmu(0,3));
  nmu_status=(nmuStatus_t*)getInfoSetValues(0,getSlotOfNmu(0,3),8,1);
  if (NULL==pChassis || NULL==nmu_status)
    return 0;
  if (1!=pChassis->fan[0].status || 1!=pChassis->fan[1].status || 1!=pChassis->fan[2].status || 1!=pChassis->fan[3].status)
        *fan_led=2;
    else
        *fan_led=1;
    if (nmu_status->sfp1_exist!=2)
    {
        if (nmu_status->sfp1_status!=2)
           sfp_ports[0]=1;
        else
           sfp_ports[0]=2;
    }
    else
        sfp_ports[0]=0;
    if (nmu_status->sfp2_exist!=2)
    {
        if (nmu_status->sfp2_status!=2)
           sfp_ports[1]=1;
        else
           sfp_ports[1]=2;
    }
    else
        sfp_ports[1]=0;
   ps_status[0]=pChassis->powersupply[0].status;
   ps_status[1]=pChassis->powersupply[1].status;
   if (NULL!=ps_type)
   {
     ps_type[0]=pChassis->powersupply[0].type;
     ps_type[1]=pChassis->powersupply[1].type;
   }
   for (n=0;n<2;n++)
   {
    if (sfp_ports[n]!=1)
      sfp_leds[n]=0;

    if (ps_status[n]!=1)
      ps_status[n]=0;
   }
   if (nmu_status->lan1_status!=1)
     lan_ports[0]=2;
   else
     lan_ports[0]=1;

   if (nmu_status->lan2_status!=1)
     lan_ports[1]=2;
   else
     lan_ports[1]=1;
   return(1);
}

char getNmuStatus(uchar_t cclass,char *fan_led,char sfp_ports[2],char sfp_leds[2],char lan_ports[2],char ps_status[2])
{
  return (getNmuAndPowerSupplyStatus(cclass,fan_led,sfp_ports,sfp_leds,lan_ports,ps_status,NULL));
}

int aspNmuStatus(int eid, webs_t wp, int argc, char_t **argv)
{
  chassis_t *pChassis;
  
  nmuStatus_t *nmu_status=(nmuStatus_t*)getInfoSetValues(0,getSlotOfNmu(0,3),8,1);
  pChassis=getChassisInfo(0);
  if (NULL!=pChassis && NULL!=nmu_status)
  {
    char fan;
    char sfp1,sfp2;
    if (1==pChassis->fan[0].status && 1==pChassis->fan[1].status && 1==pChassis->fan[2].status && 1==pChassis->fan[3].status)
        fan=1;
    else
        fan=2;
    if (nmu_status->sfp1_exist!=2)
    {
        if (nmu_status->sfp1_status!=2)
           sfp1=1;
        else
           sfp1=2;
    }
    else
        sfp1=0;
    if (nmu_status->sfp2_exist!=2)
    {
        if (nmu_status->sfp2_status!=2)
           sfp2=1;
        else
           sfp2=2;
    }
    else
        sfp2=0;
    //printf("pwr1:%d,pwr2:%d,fan:%d,run:1,sfp1:%d,sfp2:%d,lan1:%d,lan2:%d\n",pChassis->powersupply[0].status,pChassis->powersupply[1].status,fan,sfp1,sfp2,nmu_status->lan1_status,nmu_status->lan2_status);
    return websWrite(wp, T("pwr1:%d,pwr2:%d,fan:%d,run:1,sfp1:%d,sfp2:%d,lan1:%d,lan2:%d"),pChassis->powersupply[0].status,pChassis->powersupply[1].status,fan,sfp1,sfp2,nmu_status->lan1_status,nmu_status->lan2_status);
  }
  else
  {
   if (NULL==pChassis)
     printf("NULL!=pChassis\n");
   if ( NULL==nmu_status)
     printf("NULL==nmu_status\n");
   return websWrite(wp, T("pwr1:1,pwr2:1,fan:1,run:1,sfp1:0,sfp2:0"));
  }
}

int showNot4UNmuView(webs_t wp,uchar_t slot)
{
  chassis_t *pChassis;
  char fan_led=2;
  char sfp_ports[2]={2},sfp_leds[2]={2},lan_ports[2]={2},ps_status[2]={2};
  int nBytesSent=0;
  if (getNmuStatus(2,&fan_led,sfp_ports,sfp_leds,lan_ports,ps_status)<1)
    return 0;
}

int asp4UNmuView(webs_t wp,uchar_t slot)
{
  chassis_t *pChassis;
  char fan=2;
  char sfp1=2,sfp2=2,sfp1_led=2,sfp2_led,lan[2]={2},power[2]={2};
  int nBytesSent=0;
  nmuStatus_t *nmu_status=(nmuStatus_t*)getInfoSetValues(0,slot,8,1);
  //printf("aspNmuView on slot:%d\n",slot);
  pChassis=getChassisInfo(0);
  /*if (NULL==pChassis || NULL==nmu_status)
    printf("aspNmuView is null\n");
  */
  if (NULL!=pChassis && NULL!=nmu_status)
  {
    if (1==pChassis->fan[0].status && 1==pChassis->fan[1].status && 1==pChassis->fan[2].status && 1==pChassis->fan[3].status)
        fan=1;
    else
        fan=2;
    if (nmu_status->sfp1_exist!=2)
    {
        if (nmu_status->sfp1_status!=2)
           sfp1=1;
        else
           sfp1=2;
    }
    else
        sfp1=0;
    if (nmu_status->sfp2_exist!=2)
    {
        if (nmu_status->sfp2_status!=2)
           sfp2=1;
        else
           sfp2=2;
    }
    else
        sfp2=0;
    if (sfp1!=1)
       sfp1_led=0;
    else
       sfp1_led=1;

    if (sfp2!=1)
       sfp2_led=0;
    else
       sfp2_led=1;
    power[0]=pChassis->powersupply[0].status;
    power[1]=pChassis->powersupply[1].status;
    lan[0]=nmu_status->lan1_status;
    lan[1]=nmu_status->lan2_status;
    //printf("lan[0]=%d lan[1]=%d\n",lan[0],lan[1]);
    if (lan[0]<1 ||lan[0]>2)
       lan[0]=2;
    if (lan[1]<1 ||lan[1]>2)
       lan[1]=2;
    if (power[0]>1)
         power[0]=0;
    if (power[1]>1)
         power[1]=0;
   // return websWrite(wp, T("pwr1:%d,pwr2:%d,fan:%d,run:1,sfp1:%d,sfp2:%d,lan1:%d,lan2:%d"),pChassis->powersupply[0].status,pChassis->powersupply[1].status,fan,sfp1,sfp2,nmu_status->lan1_status,nmu_status->lan2_status);
  }
  else
  {
    printf("NmuView on slot #%d is null\n",slot);
    return 0;
  }
  //  return websWrite(wp, T("pwr1:1,pwr2:1,fan:1,run:1,sfp1:1,sfp2:1"));
  nBytesSent=websWrite(wp, T("<div class=\"ubd1\"> \
   <div class=\"un\"><a href='sys/sys_info.asp' target='mf'>NMU</a></div> \
   <div class=\"leds\"> \
   <div class=\"ll\">PWR1</div>"));
 nBytesSent+=websWrite(wp, T("<div class=\"led\"><img id=\"pwr1\" src=\"images/led_%d.png\" /></div> \
    <div class=\"led\"><img id=\"pwr2\" src=\"images/led_%d.png\" /></div> \
    <div class=\"ll\">PWR2</div> \
    <div class=\"ll\">FAN</div> \
    <div class=\"led\"><img id=\"fan\" src=\"images/led_%d.png\" /></div> \
    <div class=\"led\"><img id=\"run\" src=\"images/led_1.png\" /></div> \
    <div class=\"ll\">RUN</div> \
    <div class=\"ll\">SFP1</div> \
    <div class=\"led\"><img id=\"osc1\" src=\"images/led_%d.png\" /></div> \
    <div class=\"led\"><img id=\"osc2\" src=\"images/led_%d.png\" /></div> \
    <div class=\"ll\">SFP2</div> \
    <div class=\"clr\"></div> \
   </div>"),power[0],power[1],fan,sfp1_led,sfp2_led);

  nBytesSent+=websWrite(wp, T(" <div class=\"port\"> \
  <ul>\
   <li>&nbsp;</li> \
   <li>SFP1</li> \
  <li>&nbsp;</li>\
  </ul>\
 <div class=\"pimg\"><img id=\"sfp1\" style=\"width:20px;height:28px;\" src='images/p2_%d.gif'/></div> \
    </div> \
	<div class=\"clr\"></div> \
    <div class=\"port\">\
    <ul>\
	<li>&nbsp;</li>\
	<li>SFP2</li>\
	<li>&nbsp;</li>\
    </ul>\
    <div class=\"pimg\"><img id=\"sfp2\" style=\"width:20px;height:28px;\" src=\"images/p2_%d.gif\"/></div> \
    </div> \
    <div class=\"clr\"></div> \
    <div class=\"adpter vsb0\"> \
	 <div class=\"pl\">LAN1</div> \
	 <div class=\"pimg\"><img id=\"lan1\" src='images/epl_%d.gif'/></div> \
    </div> \
    <div class=\"clr\"></div> \
    <div class=\"adpter vsb0\"> \
      <div class=\"pl\">LAN2</div> \
	<div class=\"pimg\"><img id=\"lan2\" src='images/epl_%d.gif'/></div> \
    </div> \
    <div class=\"clr\"></div> <div class=\"adpter vsb0\"> \
	<div class=\"pl\">CON</div>"),sfp1,sfp2,lan[0],lan[1]);
   nBytesSent+=websWrite(wp, T("<div class=\"pimg\"><img src='images/p1_2.gif'/></div> \
    </div> \
    <div class=\"clr\"></div> <div class=\"adpter vsb0\"> \
	<div class=\"pl\">Stack</div> \
	<div class=\"pimg\"><img src='images/p1_2.gif'/></div> \
    </div> \
    <div class=\"clr\"></div></div>"));
   return nBytesSent;
}

int aspNot4UNmuView(webs_t wp,uchar_t slot)
{
  char n;
  //uchar_t slot_of_nmu;
  int nBytes=0;
  signed char leds_status[3][4]={-1};
  signed char ports_status[8];

  char left_led_lbl[][6]={"PWR1","FAN","SFP1"};
  char right_led_lbl[][6]={"PWR2","RUN","SFP2"};

  char fan_led,sfp_ports[2]={2},sfp_leds[2]={2},lan_ports[2]={2},ps_status[2]={2};
  int nBytesSent=0;
  
  //slot_of_nmu=getSlotOfNmu(0,3);
  getNmuStatus(2,&fan_led,sfp_ports,sfp_leds,lan_ports,ps_status);
  
  leds_status[0][2]=ps_status[1];

  leds_status[0][0]=-1;
  leds_status[0][3]=-1;
  leds_status[1][0]=-1;
  leds_status[1][3]=-1;
  leds_status[2][0]=-1;
  leds_status[2][3]=-1;

  leds_status[0][1]=ps_status[0];
  leds_status[0][2]=ps_status[1];
  leds_status[1][1]=fan_led;
  leds_status[1][2]=1;//RUN
  leds_status[2][1]=sfp_leds[0];
  leds_status[2][2]=sfp_leds[1];
  //printf("fan_led=%d sfp1:%d sfp2:%d ps_1:%d ps_2:%d\n",fan_led,sfp_leds[0],sfp_leds[1],ps_status[0],ps_status[1]);
  nBytes+=showHLeds(wp,leds_status[0],leds_status[1],leds_status[2],left_led_lbl, right_led_lbl,4);
  
  for (n=0;n<2;n++)
  {
  nBytes+=websWrite(wp, T("<div class=\"port\" style=\"width:36px;\">\n \
<div class=\"pls\">\n \
<div class=\"pl\"><span class=\"sl\">SFP%d</span></div>\n \
</div>"),n+1);
  nBytes+=websWrite(wp, T("<div class=\"pimg\"><span><img  src=\"images/sfph%d.png\"/></span></div></div>"),sfp_ports[n]);
  }
  for (n=0;n<2;n++)
  {
  nBytes+=websWrite(wp, T("<div class=\"port\" style=\"width:36px;\">\n \
<div class=\"pls\">\n \
<div class=\"pl\"><span class=\"sl\">LAN%d</span></div>\n \
</div>"),n+1);
  nBytes+=websWrite(wp, T("<div class=\"pimg\"><span><img src=\"images/epu%d.png\"/></span></div></div>"),lan_ports[n]);
  }
  nBytes+=websWrite(wp, "<div class=\"port\" style=\"width:36px;\">\n \
<div class=\"pls\">\n \
<div class=\"pl\"><span class=\"sl\">CON</span></div>\n \
</div>");
  nBytes+=websWrite(wp, "<div class=\"pimg\"><span><img src=\"images/epd2.png\"/></span></div></div>");
  
  nBytes+=websWrite(wp, "<div class=\"port\" style=\"width:36px;\">\n \
<div class=\"pls\">\n \
<div class=\"pl\"><span class=\"sl\">Stack</span></div>\n \
</div>");
  nBytes+=websWrite(wp, "<div class=\"pimg\"><span><img src=\"images/epd2.png\"/></span></div></div>");
  return nBytes;
}
int aspNmuView(webs_t wp,uchar_t uclass,uchar_t slot)
{
 if (CC_4U==uclass)
    return (asp4UNmuView(wp,slot));
 else
    return (aspNot4UNmuView(wp,slot));
}
char isTwoSlotUnit(uchar_t uclass,uchar_t utype)
{
  if (!(UC_PASSIVE==uclass && 3==utype))
    return 0;
  else
    return 1;
  
}


int showUnitBodyView(webs_t wp,int slot,uchar_t cclass,uchar_t uclass,uchar_t utype)
{
  int nBytes=0;
  //printf("Run in showUnitBodyView slot=%d utype==%d\r\n",slot,utype);
  switch (uclass)
   {
      case UC_OTU:
      case UC_OEO:
               nBytes +=aspOtuView(wp,slot,cclass,uclass,utype,0);
        break;
      case UC_OLP:
      case UC_OSS:
               nBytes +=aspOlpView(wp,slot,cclass,uclass,utype);
        break;
      case UC_OTAP:
               nBytes +=aspOtapView(wp,slot,cclass,uclass,utype,0);
        break;
      case UC_EDFA:
               nBytes +=aspEdfaView(wp,slot,cclass,uclass,utype);
        break;
      case UC_ETU:
               nBytes +=aspEtuView(wp,slot,cclass,utype);
        break;
      case UC_DVOA:
               nBytes +=aspDvoaView(wp,slot,cclass,utype);
        break;
      case UC_PASSIVE:

               nBytes +=aspPassiveUnitView(wp,slot,cclass,uclass,utype);
        break;
      case UC_NMU:
              nBytes +=aspNmuView(wp,cclass,slot);
        break;
   }
  return nBytes;
}

int aspMakeJscript(int eid, webs_t wp, int argc, char_t **argv,char_t cclass)
{
   unitViewInfo_t info;
   chassis_t *pchassis;
   nmuStatus_t *pnmu;

   web_unit_t uweb;
   uchar_t slotCount,max_slot_count=16;
   int n,m,nBytes=0;
   uchar_t alarms[17]={0};
   uchar_t sfp[2]={3,3},lan[2]={2,2},fan_led=1,sfp_led[2]={3,3},pwr[2]={3,3};

   cclass=getUnitClass(0,0);
   slotCount=getSlotOfNmu(0,3);
   if (1==cclass && slotCount>4)
       slotCount=4;
   if (cclass==4 && slotCount>16)
       max_slot_count=16;
   //printf("aspMakeJscript...slotCount=%d\n",slotCount);
   pnmu=(nmuStatus_t*)getInfoSetValues(0,slotCount,8,1);
   if (pnmu!=NULL)
   {
    if (pnmu->sfp1_exist!=2)
    {
        if (pnmu->sfp1_status!=2)
           sfp[0]=1;
        else
           sfp[0]=2;
    }
    else
        sfp[0]=0;
    if (pnmu->sfp2_exist!=2)
    {
        if (pnmu->sfp2_status!=2)
           sfp[1]=1;
        else
           sfp[1]=2;
    }
    else
         sfp[1]=0;
    if (pnmu->lan1_status!=2)
      lan[0]=1;
    if (pnmu->lan2_status!=2)
      lan[1]=1;
   }
   if (sfp[0]!=1)
     sfp_led[0]=3;
   else
     sfp_led[0]=1;
   if (sfp[1]!=1)
     sfp_led[1]=3;
   else
     sfp_led[1]=1;
   nBytes+=websWrite(wp,"<td>\n<script language=\"javascript\">\n");
   nBytes+=websWrite(wp,T("var n,u,l,p,nmu,ps,nv;\nvar cclass=%d,units=new Array(%d);\nnmu=new Object();\nvar alm=null;\n"),cclass,slotCount-1);
   //else
     //  var alm
   //nBytes+=websWrite(wp,T("var n,u,l,p,nmu,ps,nv;\nvar units=new Array(%d);\nnmu=new Object();\n"),slotCount-1,1);
   nBytes+=websWrite(wp,T("nmu.slot=%d;\nnmu.un=\"NMU\";nmu.vicount=0;\nnmu.slotCount=1;\nnmu.uclass=1;\np=new Array(6);\n"),slotCount);
   
   nBytes+=websWrite(wp,"p.pl=new Array([\"SFP1\",\"SFP2\",\"LAN1\",\"LAN2\",\"CON\",\"Stack\"]);\np.pimg=new Array([\"sfp2_\",\"sfp2_\",\"epu\",\"epu\",\"epd\",\"epd\"]);\n");
   nBytes+=websWrite(wp,T("p.pstatus=new Array(%d,%d,%d,%d,2,2);\n"),sfp[0],sfp[1],lan[0],lan[1]);
   nBytes+=websWrite(wp,"p.pclass=\"tm15 imgw30\";p.hi=\"250px\";");
   
   pchassis=getChassisInfo(0);
   if (NULL!=pchassis)
   {
    fan_led=pchassis->fan_led;
    if (fan_led!=2)
        fan_led=1;
    if (pchassis->powersupply[0].status==1)
       pwr[0]=1;
    if (pchassis->powersupply[1].status==1)
       pwr[1]=1;
    if (slotCount<15)
    {
     nBytes+=websWrite(wp,"ps=new Array(2);\n");
     for (n=0;n<2;n++)
     {
        if (pchassis->powersupply[n].exist==1)
        {
          nBytes+=websWrite(wp,T("ps[%d]=new Object();\nps[%d].ptype=%d;\nps[%d].run=%d;\n"),n,n,pchassis->powersupply[n].type,n,pchassis->powersupply[n].status);
        }
     }
      nBytes+=websWrite(wp,T("fan=new Array(%d,%d,%d,%d);\n"),pchassis->fan[0].status,pchassis->fan[1].status,pchassis->fan[2].status,pchassis->fan[3].status);
     nBytes+=websWrite(wp,T("nmu.ps1=%d;\n"),pwr[0]);
    }
    else
        nBytes+=websWrite(wp,T("nmu.ps1=%d;\nnmu.ps2=%d;\n"),pwr[0],pwr[1]);
  }
   
   nBytes+=websWrite(wp,"l=new Object();\n");
   nBytes+=websWrite(wp,T("l.leds=new Array([0,%d,1,0],[0,%d,%d,0]);\n"),fan_led,sfp_led[0],sfp_led[1]);
   nBytes+=websWrite(wp,"l.txt=new Array([\"FAN\",\"SFP1\"],[\"RUN\",\"SFP2\"]);\n");
   nBytes+=websWrite(wp,T("l.colCount=2;\nl.runLed=%d;\nl.runTxt=\"PWR2\";\nnmu.led=l;\nnmu.ports=p;\n"),pwr[1]);
   //printf("aspMakeJscript..slotCount=%d\n",slotCount);
   for (n=0;n<max_slot_count;n++)
   {
    if (getUnitViewInfo(0,n+1,&info)!=FALSE)
    { 
      memset(&uweb,0,sizeof(web_unit_t));
      nBytes+=websWrite(wp,"u=new Object();\n");
      nBytes+=websWrite(wp,"u.slotCount=1;\n");
      nBytes+=websWrite(wp,T("u.uclass=%d;u.utype=%d;\n"),info.uclass,info.utype);
      nBytes+=websWrite(wp,T("u.un=\"%s\";u.vicount=%d;\n"),info.unit_name,info.view_images_count);
      if (info.view_images_count>0)
      {
        nBytes+=websWrite(wp,T("u.vimages=new Array(%d);\n"),info.view_images_count);
        if (info.view_images_count>4)
            info.view_images_count=4;
        for (m=0;m<info.view_images_count;m++)
        {
             nBytes+=websWrite(wp,T("u.vimages[%d]='%s';\n"),m,info.view_images[m]);
        }
        nBytes+=websWrite(wp,T("units[%d]=u;\n"),n);
        continue;
      }
      if (info.utype_text[0]!='\0')
          nBytes+=websWrite(wp,T("u.btext=\"%s\";\n"),info.utype_text);

      nBytes+=websWrite(wp,"p=new Array(1);\np[0]=new Object();\n");
      if(FALSE!=getUnitWebInfo(0,cclass,n+1,info.uclass,info.utype,&uweb))
      {
         if (uweb.height[0]!='\0')
           nBytes+=websWrite(wp,T("u.hi=\"%s\";\n"),uweb.height);

         if (uweb.port_groups[0].pclass[0]!='\0')
          nBytes+=websWrite(wp,T("p[0].pclass=\"%s\";\n"),uweb.port_groups[0].pclass);
         if (uweb.port_groups[1].port_count<2)
         {
             if (info.uclass==UC_ETU)
             {
                nBytes+=websWrite(wp,T("p[0].img=\"\";\r\np[0].imges= new Array();\n"));
                nBytes+=websWrite(wp,T("p[0].imges[0]=\"%s\";\n"),"epu");
                nBytes+=websWrite(wp,T("p[0].imges[1]=\"%s\";\n"),"sfp2_");
                nBytes+=websWrite(wp,T("p[0].imges[2]=\"%s\";\n"),"epu");
                nBytes+=websWrite(wp,T("p[0].imges[3]=\"%s\";\n"),"sfp2_");
             }
             else
             {
               if (!(info.uclass==UC_OLP && info.utype==32))
                nBytes+=websWrite(wp,T("p[0].img=\"%s\";\n"),uweb.port_groups[0].pimg);
               else
               {
                nBytes+=websWrite(wp,T("p[0].img=\"\";\r\np[0].imges= new Array();\n"));
                nBytes+=websWrite(wp,T("p[0].imges[0]=\"%s\";\n"),"lcv2_");
                nBytes+=websWrite(wp,T("p[0].imges[1]=\"%s\";\n"),"scv1_");
                nBytes+=websWrite(wp,T("p[0].imges[2]=\"%s\";\n"),"scv1_");
               }
             }
         }
         else
         {
            nBytes+=websWrite(wp,T("p[0].img=\"\";\r\np[0].imges= new Array();\n"));
            for (m=0;m<uweb.port_groups[0].port_count;m++)
               nBytes+=websWrite(wp,T("p[0].imges[%d]=\"%s\";\n"),m,uweb.port_groups[0].pimg);
            for (m=0;m<uweb.port_groups[1].port_count;m++)
               nBytes+=websWrite(wp,T("p[0].imges[%d]=\"%s\";\n"),uweb.port_groups[0].port_count +m,uweb.port_groups[1].pimg);
         }
         if (uweb.port_groups[0].isTxtAtTwoSide==1)
           nBytes+=websWrite(wp,"p[0].isTxtAtTwoSide=1;\n");
      }
      nBytes+=websWrite(wp,T("p[0].count=%d;\n"),info.ports.portCount);
      nBytes+=websWrite(wp,T("p[0].pstatus=new Array(%d"),info.ports.ports_status[0]);
      for (m=1;m<info.ports.portCount;m++)
       nBytes+=websWrite(wp,T(",%d"),info.ports.ports_status[m]);
      nBytes+=websWrite(wp,");\n");
      if (info.ports.ports_htext[0][0]!='\0')
      {
        nBytes+=websWrite(wp,T("p[0].plh=new Array(\"%s\""),info.ports.ports_htext[0]);
        for (m=1;m<info.ports.portCount;m++)
         nBytes+=websWrite(wp,T(",\"%s\""),info.ports.ports_htext[m]);
        nBytes+=websWrite(wp,");\n");
      }
      
      if (info.ports.has_info)
      {
        uchar_t k;
        nBytes+=websWrite(wp,"p[0].pi=new Object();\n");
        nBytes+=websWrite(wp,T("p[0].pi.tname=new Array(\"%s\",\"%s\",\"%s\");\n"),info.ports.ports_info_name[0],info.ports.ports_info_name[1],info.ports.ports_info_name[2]);
        nBytes+=websWrite(wp,T("p[0].pi.unit=new Array(\"%s\",\"%s\",\"%s\");\n"),info.ports.ports_info_unit[0],info.ports.ports_info_unit[1],info.ports.ports_info_unit[2]);
        nBytes+=websWrite(wp,"p[0].pi.val=new Array(");
        
        for (m=0;m<info.ports.portCount;m++)
        {
         if (m>0)
             nBytes+=websWrite(wp,",");
         //nBytes+=websWrite(wp,"[");
         //if (info.ports.ports_info_val[n][0][0]!='\0')
         nBytes+=websWrite(wp,T("[\"%s\""),info.ports.ports_info_val[m][0]);
         //else
         //    nBytes+=websWrite(wp,"\"\"");
         for(k=1;k<3;k++)
         {
           nBytes+=websWrite(wp,T(",\"%s\""),info.ports.ports_info_val[m][k]);
         }
         nBytes+=websWrite(wp,"]");
        }
        nBytes+=websWrite(wp,");\n");
      }
      nBytes+=websWrite(wp,T("p[0].pl=new Array([\"%s\""),info.ports.ports_text[0][0]);
      for (m=1;m<info.ports.portCount;m++)
      { 
        nBytes+=websWrite(wp,T(",\"%s\""),info.ports.ports_text[0][m]);
        //printf("ports_text[1][%d]=%s\n",m,info.ports.ports_text[1][m]);
      }
      //printf("ports_text[1][0]=%s\n",info.ports.ports_text[1][0]);
      if (strlen(info.ports.ports_text[1][0])>0)
      {
        nBytes+=websWrite(wp,T("],[\"%s\""),info.ports.ports_text[1][0]);
        for (m=1;m<info.ports.portCount;m++)
        {
         nBytes+=websWrite(wp,T(",\"%s\""),info.ports.ports_text[1][m]);
        }
      }
      
      nBytes+=websWrite(wp,"]);\n");
      nBytes+=websWrite(wp,"u.ports=p;\n");
      if (info.uclass!=UC_PASSIVE)
      { 
         nBytes+=websWrite(wp,"l=new Object();\n");
         nBytes+=websWrite(wp,T("l.leds=new Array([%d,%d,%d,%d],[%d,%d,%d,%d]);\n"),info.leds.leds_status[0][0],info.leds.leds_status[0][1],info.leds.leds_status[0][2],info.leds.leds_status[0][3],info.leds.leds_status[1][0],info.leds.leds_status[1][1],info.leds.leds_status[1][2],info.leds.leds_status[1][3]);
         //nBytes+=websWrite(wp,T("l.txt=new Array([\"%s\""),info.ports.ports_text[0][0]);
         nBytes+=websWrite(wp,T("\nl.colCount=%d;\nl.runLed=%d;\nl.runTxt=\"%s\";\n"),info.leds.col_count,info.leds.run_led,info.leds.run_text);   
         nBytes+=websWrite(wp,T("l.txt=new Array([\"%s\",\"%s\"],[\"%s\",\"%s\"]);\nu.led=l;\n"),info.leds.leds_text[0][0],info.leds.leds_text[0][1],info.leds.leds_text[1][0],info.leds.leds_text[1][1]);
      
      }
     nBytes+=websWrite(wp,T("units[%d]=u;\n"),n);
    }
   }
   if (getUnitAlarm(0,alarms)>0)
   {
     
     nBytes+=websWrite(wp,T("alm=new Array(%d"),alarms[0]);
     for (n=1;n<=slotCount;n++)
     {
      //printf("alarms[%d]=%d\n",n,alarms[n]);
      nBytes+=websWrite(wp,T(",%d"),alarms[n]);
     }
     nBytes+=websWrite(wp,");\n");
   }
   #ifdef DEFAULT_LANG_CN
   nBytes+=websWrite(wp,"showChassis(units,nmu,ps,fan,alm,1);\n</script>\n</td>");
   #else
   nBytes+=websWrite(wp,"showChassis(units,nmu,ps,fan,alm,2);\n</script>\n</td>");
   #endif
   return nBytes;
}
int asp1UChassisView(int eid, webs_t wp, int argc, char_t **argv,char_t cclass)
{
   return aspMakeJscript(eid, wp, argc, argv,cclass);
}
int asp2UChassisView(int eid, webs_t wp, int argc, char_t **argv,char_t cclass)
{
   return aspMakeJscript(eid, wp, argc, argv,cclass);
}
/*
int asp1UChassisView2(int eid, webs_t wp, int argc, char_t **argv)
{
 int n,m,nBytes=0;//,m,k,chassis=0,slot,pclass,ptype,pstatus,count;
 uchar_t utype,uclass;
 uchar_t slotCount;
 //uchar_t slot_of_nmu;
 char *un;
 char ret;
 char uname[60]="";
 char fan_led=2;
 char sfp_ports[2]={2},sfp_leds[2]={2},lan_ports[2]={2},ps_status[2]={2},ps_type[2]={2};

 slotCount=getSlotOfNmu(0,3);
 nBytes=websWrite(wp,"<td><div class=\"oneUchassis\">\n<div class=\"oneUL\"></div>\n");
 for (n=1;n<3;n++)
 {
   ret=getUnitClassType(0,n,&uclass,&utype);
   nBytes=websWrite(wp,"<div class=\"hupane\">\n<div class=\"uh\">\n");
   nBytes=websWrite(wp,T("<div class=\"slot\">\n \
    <div class=\"pl\"><span class=\"plabel\"><a href='"));
   if (n!=slotCount)
      nBytes+=websWrite(wp,T("goform/slot?cid=0&uid=%d&page=bi'>%d"),n,n);
    else
      nBytes+=websWrite(wp,"/sys/nmubi.asp'>2");

   nBytes+=websWrite(wp,"</a></span></div>\n</div>\n");
   //printf("Run in asp1UChassisView #%d uclass=%d,utype=%d\n",n,uclass,utype);
   if (FALSE!=ret &&getUnitLabelName(uclass,utype,uname)>0)
   {
    char leds_status[4][4];
    char ports_status[8];

    char left_lable[4][6]={"PWR","S1","S2",""};
    char right_lable[4][6]={"RUN","CH1","CH2",""};
    //un=strToUpper(uname);
    nBytes=websWrite(wp,"<div class=\"un\">\n<div class=\"pl\"><span class=\"plabel\"><a href='");
    if (n!=slotCount)
      nBytes+=websWrite(wp,T("/goform/slot?cid=0&uid=%d"),n);
    else
      nBytes+=websWrite(wp,"/sys/sys_info.asp");
    if (UC_PASSIVE==uclass)
    {
     char page[10];
     strncpy(page,uname,3);
     nBytes=websWrite(wp,T("&page=%s"),strTolower(page));
    }
    nBytes=websWrite(wp,T("'>%s</a></span></div>\n \
</div>\n</div>\n"),uname);
     //show4ColHLeds(n,leds_status,left_lable,right_lable,2);
     nBytes +=showUnitBodyView(wp,n, 1,uclass,utype);
   }
   else
    nBytes+=websWrite(wp,"</div>\n");
  nBytes+=websWrite(wp,"<div class=\"uf\"></div>\n");
  nBytes+=websWrite(wp,"</div>\n");
 }

 getNmuAndPowerSupplyStatus(2,&fan_led,sfp_ports,sfp_leds,lan_ports,ps_status,ps_type);
 nBytes+=websWrite(wp,"<div class=\"oneUR\"></div>\n</div>\n");
 nBytes+=websWrite(wp,"<div class=\"oneUchassis\">\n<div class=\"oneUL\"></div><div class=\"hupane\">\n");
 nBytes+=websWrite(wp,"<div style=\"width:220px;float:left;padding:10px 5px;\">");
 if (ps_status[0]==1)
 {
 nBytes+=websWrite(wp,"<table><tr><td><img src='images/psfan.png'/></td>");
 nBytes+=websWrite(wp,T("<td>PWR1<img src='images/led_1.png'/><br/><img src='images/ptype_%d.gif'/></td>"),ps_type[0]);
 nBytes+=websWrite(wp,"</tr></table>\n");
 }
 nBytes+=websWrite(wp,"</div>\n<div style=\"width:220px;float:left;padding:10px 5px;\">");
 if (ps_status[1]==1)
 {
 nBytes+=websWrite(wp,"<table><tr><td><div style='margin-left:10px;'><img src='images/psfan.png'/></td></div>");
 nBytes+=websWrite(wp,T("<td><div style='margin-left:10px;'>PWR2<img src='images/led_1.png'/><br/><img src='images/ptype_%d.gif'/></div></td>"),ps_type[1]);
 nBytes+=websWrite(wp,"</tr></table>\n");
 }
 nBytes+=websWrite(wp,"</div></div>");
 
 nBytes+=websWrite(wp,"\n<div class=\"hupane\">\n");
 nBytes+=websWrite(wp,"\n<div class=\"uh\"><div class=\"slot\">\n<div class=\"pl\"><span class=\"plabel\">3</span></div>\n</div>\n");
 if (3==slotCount)
 {
  nBytes+=websWrite(wp,"<div class=\"un\"><div class=\"pl\"><span class=\"plabel\"><a href='/sys/sys_info.asp'>NMU</a></span></div></div>/div>");
  nBytes+=aspNmuView(wp,CC_1U,3);
 }
 else
     nBytes+=websWrite(wp,"</div>");
 nBytes+=websWrite(wp,"<div class=\"uf\"></div>");
 nBytes+=websWrite(wp,"</div></div></td>");
//<table><tr><td>FAN1</td><td><img src='images/psfan.png'/></td><td>FAN1</td><td><img src='images/ptype_1.gif'/></td><td>FAN1</td><td><img src='images/fan1.gif'/></td><td>FAN1</td><td><img src='images/fan1.gif'/></td></tr></table>\n</div>");
 return nBytes;
 //return websWrite(wp, T("This is 1U Chassis"));
}
*/
int asp4UChassisView(int eid, webs_t wp, int argc, char_t **argv)
{
 int nBytesSent=0,n,m,nBytes=0;//,m,k,chassis=0,slot,pclass,ptype,pstatus,count;
 uchar_t *uclass,*utype;
 char slots=16;
 //char nextSlotUnit=0,slotUnit=0;
 char slotCountOfUnit[16]={0};
 int slotCount=17;
 uclass=getUnitStatusMap(0);
 if (NULL==uclass)
  return websWrite(wp, T("<td><div class='panebd'></div></td>"));
 nBytesSent =websWrite(wp,"<td width=\"37\" border=\"0\" background=\"images/lhanger.png\"></td>\n<td>\n");
 utype=uclass+slotCount;//MAX_UNIT_COUNT;
 for (n=1;n<slotCount;n++)
 {
   if (UC_PASSIVE==uclass[n])
   {
     if (PASSIVE_ODM2_UNIT==utype[n]||PASSIVE_DCM2_UNIT==utype[n])
       slotCountOfUnit[n-1]=2; 
     else if (PASSIVE_DCM4_UNIT==utype[n])
       slotCountOfUnit[n-1]=4;
     else
       slotCountOfUnit[n-1]=1;
   }
   else if (uclass[n]>0)
      slotCountOfUnit[n-1]=1;
   else
      slotCountOfUnit[n-1]=0;
 }
 
 for (n=16;n>0;n--)
 {
   if (n!=slots)
     continue;
   if (slotCountOfUnit[n-1]<2)
   {
     slots--;
     nBytesSent +=websWrite(wp, T("<div class='uh1"));
   }
   else
   {
    slots-=slotCountOfUnit[n-1];
    nBytesSent +=websWrite(wp, T("<div class='uh%d"),slotCountOfUnit[n-1]);
   }
   if (1!=uclass[n])
      nBytesSent +=websWrite(wp, T("'><a href='/goform/slot?cid=0&uid=%d&page=bi' target='mf'>%d&nbsp;</a></div>"),n,n);
   else
      nBytesSent +=websWrite(wp, T("'><a href='/sys/nmubi.asp' target='mf'>%d&nbsp;</a></div>"),n);
     
  }
  nBytesSent +=websWrite(wp, T("<div class=\"clr\"></div>"));
  slots=16;
  for (n=16;n>0;n--)
  {
    if (n!=slots)
     continue;
    if (slotCountOfUnit[n-1]<1)
    {
      slots--;
      nBytesSent +=websWrite(wp, T("<div class='ubd1'></div>"));
    }
    else
    {
       slots-=slotCountOfUnit[n-1];
        nBytesSent +=showUnitBodyView(wp,n, 4,uclass[n],utype[n]);
   }
  }
  nBytesSent +=websWrite(wp, T("<div class=\"clr\"></div>"));
  nBytesSent +=websWrite(wp, T("<div class=\"clr\"></div>"));
  nBytesSent +=aspUnitAlarmView(eid,wp,slotCountOfUnit);
  nBytesSent =websWrite(wp,"</td><td width=\"37\" border=\"0\" background=\"images/rhanger.png\"></td>\n");
   return nBytesSent;
}
/*
int asp4UChassisView_14_10_09(int eid, webs_t wp, int argc, char_t **argv)
{
 int nBytesSent=0,n,m,nBytes=0;//,m,k,chassis=0,slot,pclass,ptype,pstatus,count;
 uchar_t *uclass,*utype;
 char pre_unit=1;
 //char nextSlotUnit=0,slotUnit=0;
 char slotCountOfUnit[16]={0};
 int slotCount=17;
 uclass=getUnitStatusMap(0);
 if (NULL==uclass)
  return websWrite(wp, T("<td><div class='panebd'></div></td>"));
 nBytesSent =websWrite(wp,"<td width=\"37\" border=\"0\" background=\"images/lhanger.png\"></td>\n<td>\n");
 utype=uclass+slotCount;//MAX_UNIT_COUNT;
 for (n=1;n<slotCount;n++)
 {
   if (UC_PASSIVE==uclass[n])
   {
     if (PASSIVE_ODM2_UNIT==utype[n])
       slotCountOfUnit[n-1]=2; 
     else if (PASSIVE_DCM4_UNIT==utype[n])
       slotCountOfUnit[n-1]=4;
     else
       slotCountOfUnit[n-1]=1;
   }
   else if (uclass[n]>0)
      slotCountOfUnit[n-1]=1;
   else
      slotCountOfUnit[n-1]=0;
 }
 
 for (n=1;n<slotCount;n++)
 {
   if (slotCountOfUnit[n-1]>0)
   {
     for (m=pre_unit;m<(n-slotCountOfUnit[n-1]+1);m++)
     {
        nBytesSent +=websWrite(wp, T("<div class='uh1"));
        nBytesSent +=websWrite(wp, T("'><a href='/goform/slot?cid=0&uid=%d&page=bi' target='mf'>%d&nbsp;</a></div>"),m,m);
     }
     if (slotCountOfUnit[n-1]<2)
        nBytesSent +=websWrite(wp, T("<div class='uh1"));
     else
        nBytesSent +=websWrite(wp, T("<div class='uh%d"),slotCountOfUnit[n-1]);
     if (1!=uclass[n])
          nBytesSent +=websWrite(wp, T("'><a href='/goform/slot?cid=0&uid=%d&page=bi' target='mf'>%d&nbsp;</a></div>"),n,n);
     else
     {
      nBytesSent +=websWrite(wp, T("'><a href='/sys/nmubi.asp' target='mf'>%d&nbsp;</a></div>"),n);
      if (uclass[15]==1)
        nBytesSent +=websWrite(wp, "<div class='uh1'>16</div>");
     }
    pre_unit=n+1;
   }
 }
 nBytesSent +=websWrite(wp, T("<div class=\"clr\"></div>"));
 pre_unit=1;
 for (n=1;n<slotCount;n++)
 {
   if (slotCountOfUnit[n-1]>0)
   {
      for (m=pre_unit;m<(n-slotCountOfUnit[n-1]+1);m++)
      {
        nBytesSent +=websWrite(wp, T("<div class='ubd1'></div>"));
      }
      nBytesSent +=showUnitBodyView(wp,n, 4,uclass[n],utype[n]);
      //nBytesSent +=websWrite(wp, T("<div class='ubd'></div>"));
      pre_unit=n+1;
   }
 }
 if (uclass[15]==1)
       nBytesSent +=websWrite(wp,"<div class='ubd1'></div>");
 nBytesSent +=websWrite(wp, T("<div class=\"clr\"></div>"));
 nBytesSent +=aspUnitAlarmView(eid,wp,slotCountOfUnit);
 if (uclass[15]==1)
        nBytesSent +=websWrite(wp, "<div class='uf1'></div>");
 nBytesSent =websWrite(wp,"</td><td width=\"37\" border=\"0\" background=\"images/rhanger.png\"></td>\n");
 return nBytesSent;
}
*/
int asp4UChassisView_(int eid, webs_t wp, int argc, char_t **argv)
{
 int nBytesSent=0,n,m,nBytes=0;//,m,k,chassis=0,slot,pclass,ptype,pstatus,count;
 uchar_t *uclass,*utype;
 char pre_unit=1;
 //char nextSlotUnit=0,slotUnit=0;
 char slotCountOfUnit[16]={0};
 int slotCount=17;
 uclass=getUnitStatusMap(0);
 if (NULL==uclass)
  return websWrite(wp, T("<td><div class='panebd'></div></td>"));
 nBytesSent =websWrite(wp,"<td width=\"37\" border=\"0\" background=\"images/lhanger.png\"></td>\n<td>\n");
 utype=uclass+slotCount;//MAX_UNIT_COUNT;
 for (n=1;n<15;n++)
 {
   //if (uclass[n]>0)
   //  printf("slot #%d uclass=%d\n",n,uclass[n]);
   if (uclass[n]>2 && uclass[n]!=8)
   {
   if (UC_PASSIVE==uclass[n])
   {
     if (PASSIVE_ODM2_UNIT==utype[n])
       slotCountOfUnit[n-1]=2;
     else if (PASSIVE_DCM4_UNIT==utype[n])
       slotCountOfUnit[n-1]=4;
     else
       slotCountOfUnit[n-1]=1;
   }
   else if (uclass[n]>1)
      slotCountOfUnit[n-1]=1;
   }
   else
      slotCountOfUnit[n-1]=0;
 }
 if (uclass[15]==1)
    slotCountOfUnit[15]=1;
 if (uclass[16]==1)
    slotCountOfUnit[16]=1;

 for (n=1;n<16;n++)
 {
   if (slotCountOfUnit[n-1]>0)
   {
     for (m=pre_unit;m<(n-slotCountOfUnit[n-1]+1);m++)
     {
        nBytesSent +=websWrite(wp, T("<div class='uh1"));
        nBytesSent +=websWrite(wp, T("'><a href='/goform/slot?cid=0&uid=%d&page=bi' target='mf'>%d&nbsp;</a></div>"),m,m);
     }
     if (slotCountOfUnit[n-1]<2)
        nBytesSent +=websWrite(wp, T("<div class='uh1"));
     else
        nBytesSent +=websWrite(wp, T("<div class='uh%d"),slotCountOfUnit[n-1]);
     if (1!=uclass[n])
          nBytesSent +=websWrite(wp, T("'><a href='/goform/slot?cid=0&uid=%d&page=bi' target='mf'>%d&nbsp;</a></div>"),n,n);
     else
     {
      nBytesSent +=websWrite(wp, T("'><a href='/sys/nmubi.asp' target='mf'>%d&nbsp;</a></div>"),n);
      if (uclass[15]==1)
        nBytesSent +=websWrite(wp, "<div class='uh1'>16</div>");
     }
    pre_unit=n+1;
   }
 }
 nBytesSent +=websWrite(wp, T("<div class=\"clr\"></div>"));
 pre_unit=1;
 for (n=1;n<slotCount;n++)
 {
   if (slotCountOfUnit[n-1]>0)
   {
      for (m=pre_unit;m<(n-slotCountOfUnit[n-1]+1);m++)
      {
        nBytesSent +=websWrite(wp, T("<div class='ubd1'></div>"));
      }
      nBytesSent +=showUnitBodyView(wp,n, 4,uclass[n],utype[n]);
      //nBytesSent +=websWrite(wp, T("<div class='ubd'></div>"));
      pre_unit=n+1;
   }
 }
 if (uclass[15]==1)
      nBytesSent +=websWrite(wp,"<div class='ubd1'></div>");
 nBytesSent +=websWrite(wp, T("<div class=\"clr\"></div>"));
 nBytesSent +=aspUnitAlarmView(eid,wp);
 if (uclass[15]==1)
        nBytesSent +=websWrite(wp, "<div class='uf1'></div>");
 nBytesSent =websWrite(wp,"</td><td width=\"37\" border=\"0\" background=\"images/rhanger.png\"></td>\n");
 return nBytesSent;
 /*for (n=1;n<MAX_UNIT_COUNT;n++)
 {
   nextSlotUnit=isTwoSlotUnit(uclass[n+1],utype[n+1]);
   if (!nextSlotUnit)
   {
    //twoSlotUnit=isTwoSlotUnit(uclass[n+1],utype[n+1]);
    nBytesSent +=websWrite(wp, T("<div class='uh"));
    if (slotUnit)
      nBytesSent +=websWrite(wp, T("2"));
   }
   else
   {
     slotUnit=nextSlotUnit;
     if (n<16)
     continue;
   }
   slotUnit=nextSlotUnit;
   if (1!=uclass[n])
     nBytesSent +=websWrite(wp, T("'><a href='/goform/slot?cid=0&uid=%d&page=bi' target='mf'>%d&nbsp;</a></div>"),n,n);
   else
     nBytesSent +=websWrite(wp, T("'><a href='/sys/nmubi.asp' target='mf'>16&nbsp;</a></div>"));
 }
 websWrite(wp, T("<div class=\"clr\"></div>"));
 for (n=1;n<MAX_UNIT_COUNT;n++)
 {
  nBytes=0;
  if (n <16)
  {
   if (0==uclass[n]  && !isTwoSlotUnit(uclass[n+1],utype[n+1]))
     nBytesSent +=websWrite(wp, T("<div class='ubd1'></div>"));
  }
  switch (uclass[n])
   {
      case UC_OTU:
               nBytes +=aspOtuView(wp,n,utype[n],0);
        break;
      case UC_OLP:
      case UC_OSS:
               nBytes +=aspOlpView(wp,n,uclass[n],utype[n]);
        break;
      case UC_EDFA:
               nBytes +=aspEdfaView(wp,n,1);
        break;
      case UC_PASSIVE:
              // if (isTwoSlotUnit(uclass[n],utype[n]))
              //   nBytesSent +=websWrite(wp, T("<div class='ubd'></div>"));
               nBytes +=aspPassiveUnitView(wp,n,uclass[n],utype[n]);
        break;
      case UC_NMU:
              nBytes +=aspNmuView(wp,n);
        break;
   }
  //if (isTwoSlotUnit(uclass[n],utype[n]))
  //  pre_unit=1;
  if (nBytes<1 && n >15)
  {
     //pre_unit=0;
     nBytesSent +=websWrite(wp, T("<div class='ubd1'></div>"));
    //nBytesSent +=websWrite(wp, T("<div class='panebd'></div>"));
     //nBytesSent +=websWrite(wp, T("<div class='ubd'></div>"));
  }
  
  nBytesSent +=nBytes;//+websWrite(wp, T(""));
  
 }
 return nBytesSent;
*/
}

int aspChassisView(int eid, webs_t wp, int argc, char_t **argv)
{
 uchar_t cclass;//,*pcclass;
 //pcclass=getUnitStatusMap(0);
 cclass=getChassisClass(0);
 /*if (NULL==pcclass)
   return websWrite(wp, T("Chassis map is null!"));
 */
 // printf("cclass=%d BP:%d\n",cclass,getUnitClass(0,0));
 //return asp4UChassisView(eid, wp, argc, argv);
 switch (cclass)
 {
   case 4:
          return asp4UChassisView(eid, wp, argc, argv);
   case 1:
   case 11:
   case 12:
          return asp1UChassisView(eid, wp, argc, argv,cclass);
   break;
   case 2:
           return asp2UChassisView(eid, wp, argc, argv,cclass);
   break;
 }
  /*
 if (4==cclass)
   return asp4UChassisView(eid, wp, argc, argv);
 else if (1==cclass)
   return asp1UChassisView(eid, wp, argc, argv);
 else if (2==cclass)
   return asp2UChassisView(eid, wp, argc, argv);
 else
 */
    return websWrite(wp, T("This is Unkown Chassis(%d)!"),cclass);
}

/*
int aspChassisView_(int eid, webs_t wp, int argc, char_t **argv)
{
 int nBytesSent=0,n,nBytes=0;//,m,k,chassis=0,slot,pclass,ptype,pstatus,count;
 uchar_t *uclass,*utype;
 char pre_unit=1;
 char nextSlotUnit=0,slotUnit=0;
 char slotCountOfUnit[16]={0};
 uclass=getUnitStatusMap(0);
 if (NULL==uclass)
  return websWrite(wp, T("<td><div class='panebd'></div></td>"));
 utype=uclass+MAX_UNIT_COUNT;
 for (n=1;n<MAX_UNIT_COUNT;n++)
 {
   if (UC_PASSIVE==uclass && 3==utype)
     slotCountOfUnit[n-1]=2;
 }
 for (n=1;n<MAX_UNIT_COUNT;n++)
 {
   nextSlotUnit=isTwoSlotUnit(uclass[n+1],utype[n+1]);
   if (!nextSlotUnit)
   {
    //twoSlotUnit=isTwoSlotUnit(uclass[n+1],utype[n+1]);
    nBytesSent +=websWrite(wp, T("<div class='uh"));
    if (slotUnit)
      nBytesSent +=websWrite(wp, T("2"));
   }
   else
   {
     slotUnit=nextSlotUnit;
     if (n<16)
     continue;
   }
   slotUnit=nextSlotUnit;
   if (1!=uclass[n])
     nBytesSent +=websWrite(wp, T("'><a href='/goform/slot?cid=0&uid=%d&page=bi' target='mf'>%d&nbsp;</a></div>"),n,n);
   else
     nBytesSent +=websWrite(wp, T("'><a href='/sys/nmubi.asp' target='mf'>16&nbsp;</a></div>"));
 }
 websWrite(wp, T("<div class=\"clr\"></div>"));
 for (n=1;n<MAX_UNIT_COUNT;n++)
 {
  nBytes=0;
  if (n <16)
  {
   if (0==uclass[n]  && !isTwoSlotUnit(uclass[n+1],utype[n+1]))
     nBytesSent +=websWrite(wp, T("<div class='ubd1'></div>"));
  }
  switch (uclass[n])
   {
      case UC_OTU:
               nBytes +=aspOtuView(wp,n,utype[n],0);
        break;
      case UC_OLP:
      case UC_OSS:
               nBytes +=aspOlpView(wp,n,uclass[n],utype[n]);
        break;
      case UC_EDFA:
               nBytes +=aspEdfaView(wp,n,1);
        break;
      case UC_PASSIVE:
              // if (isTwoSlotUnit(uclass[n],utype[n]))
              //   nBytesSent +=websWrite(wp, T("<div class='ubd'></div>"));
               nBytes +=aspPassiveUnitView(wp,n,uclass[n],utype[n]);
        break;
      case UC_NMU:
              nBytes +=aspNmuView(wp,n);
        break;
   }
  //if (isTwoSlotUnit(uclass[n],utype[n]))
  //  pre_unit=1;
  if (nBytes<1 && n >15)
  {
     //pre_unit=0;
     nBytesSent +=websWrite(wp, T("<div class='ubd1'></div>"));
    //nBytesSent +=websWrite(wp, T("<div class='panebd'></div>"));
     //nBytesSent +=websWrite(wp, T("<div class='ubd'></div>"));
  }
  
  nBytesSent +=nBytes;//+websWrite(wp, T(""));
  
 }
 return nBytesSent;
}
*/

int aspChassisInfo(webs_t wp,int group)
{
  int n,m=0,l=0,e,nbytes=0;
  chassis_t *info;
  chassisThreshold_t *threshold;
  char ps_exist[3][8]={"","在位","脱位"};
  char ps_type[3][8]={"","AC","DC"};
  char ps_status[3][8]={"","ON","OFF"};
  char power[3][8]={""};
  char psinfo[120];
  info=getChassisInfo(0);
  threshold=getChassisThreshold(0);
  if ( NULL==info || NULL==threshold)
  {
    return websWrite(wp, T("<tr><td>back panel is NOT found!</td></tr>"));
  }
  nbytes=websWrite(wp, T("var cclass=%d,fs=[%d,%d,%d,%d];"),info->chassis_type,info->fan[0].status,info->fan[1].status,info->fan[2].status,info->fan[3].status);
  sprintf(psinfo,"var psinfo =new Array(new ps(%d,%d,%d,%.1f),new ps(%d,%d,%d,%.1f));",info->powersupply[0].exist,info->powersupply[0].status,info->powersupply[0].type,info->powersupply[0].volt*0.1,info->powersupply[1].exist,info->powersupply[1].status,info->powersupply[1].type,info->powersupply[1].volt*0.1);
  nbytes+=websWrite(wp,psinfo);
  nbytes+=websWrite(wp,T("var minvolt=%d,maxvolt=%d;"),threshold->min_volt,threshold->max_volt);;
  return nbytes;
 /* if (group==1)
  {
  for (n=0;n<2;n++)
  {
    e=info->powersupply[n].exist;
    if (e<0 ||e>2)
     e=0;
    
    l=info->powersupply[n].status;
    if (l<0 ||l>2)
     l=0;
    m=info->powersupply[n].type;
    if (m<0 ||m>2)
      m=0;
    if (l==1)
    {
      sprintf(power[0],"%.1f",info->powersupply[n].volt*0.1);
    }
    nbytes+=websWrite(wp, T("<tr> \
    <th>电源%d</th> \
    <td>%s</td> \
    <td>%s</td> \
    <td>%s</td> \
    <td>%s</td> \
    </tr>"),n+1,ps_exist[e],ps_status[l],ps_type[m],power[0]);
  }
   nbytes+=websWrite(wp, T("<tr><th>最低电压</th><td><input type='text' id ='minvolt' name='minvolt' maxlength='3'size='3' value='%d' onkeyup='uIntInput(this);' onblur='return check();'/>Volt</td><th>最高电压</th><td><input type='text' id ='maxvolt' name='maxvolt' maxlength='3'size='3' value='%d'  onkeyup='intInput(this);' onblur='return check();'/>Volt</td></td>"),threshold->min_volt,threshold->max_volt);
}
  else if (group==2)
  {
    for (n=0;n<MAX_FAN_COUNT;n++)
    {
      l=info->fan[n].status;
      if (l<1 ||l>2)
       l=2;
      nbytes+=websWrite(wp, T("<td>风扇%d</td><td><img src='../images/fan%d.gif'/></td>"),n+1,l);
    }
  }
  return nbytes;
*/
}
