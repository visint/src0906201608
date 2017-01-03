#include <sys/sysinfo.h>
#include <time.h>
#include "../wsIntrn.h"
#include "../../include/unit.h"

#include <public.h>
#include <visipc.h>
#include <sqlite3.h>
#include <dbutil.h>
#include <log.h>
#include <edfa.h>
#include <ddm.h>

#include "../unitwebs.h"
boolean_t getEdfaWebInfo(uchar_t chassis,uchar_t cclass,uchar_t slot,uchar_t uclass,uchar_t utype,web_unit_t *punit)
{
  punit->uclass=UC_EDFA;
  punit->slotCount=1;
  strcpy(punit->height,"60%");
  strcpy(punit->led.run_text,"OPM");
  //info->port_groups[0].isTxtAtTwoSide=0;
  strcpy(punit->port_groups[0].pclass,"tm15 imgw30 imgh30");
  strcpy(punit->port_groups[0].pimg,"upc_");
  punit->port_groups[0].port_count=3;

  /*if (utype==2)
  {
   strcpy(punit->port_groups[1].pimg,"upc_");
   punit->port_groups[1].port_count=2;
  }
  */
  return TRUE;
}

int aspEdfaStatus(webs_t wp,uchar_t slot)
{

  char pump_status[2];
  char pumpcount=1;
  char n;
  int bytes=0;
  short ipower,opower,temp,gain;
  edfaPumpInfo_t pump_info;
  uchar_t working_mode=0,utype;
  //short value;
  slot=getSelectedUnit();

  edfaInfo_t *et=getEdfaInfo(0,slot);
  utype=getUnitType(0,slot);
  if (pumpcount<1)
     pumpcount=1;
  else if (pumpcount>2)
     pumpcount=2;
  if (NULL!=et)
  {
    //printf("ut=%d\n",utype);
    if (utype<5)
    {
    working_mode=et->work_mode;
    pump_status[0]=et->pump1_status;
    pump_status[1]=et->pump2_status;
    if (1==et->work_mode)
       gain=et->agc_or_apc_value;
    else
        gain=et->outputPower;
    ipower=et->inputPower;
    opower=et->outputPower;
    temp=et->temperature;
    for (n=0;n<pumpcount;n++)
    {
      pump_info.working_current=et->pump_status[n].working_current;
      pump_info.power=et->pump_status[n].power;
      pump_info.temperature=et->pump_status[n].temperature;
      pump_info.cool_current=et->pump_status[n].cool_current;
    }
   }
   else
   {
       edfa5Info_t *edfa5=(edfa5Info_t*)et;
       working_mode=edfa5->work_mode;
       //short mvalue=edfa5->max_value;
       bytes+=websWrite(wp, T("setting_flag=%d;maxValue=%d;\n"),edfa5->setting_flag,edfa5->max_value);
       pump_status[0]=edfa5->pump_status[0];
       pump_status[1]=edfa5->pump_status[1];
       if (1==edfa5->work_mode)
         gain=edfa5->gain;
       else
         gain=edfa5->auto_power;
    //printf("gain=%s gain=%.1f opower=%.1f\n",gain,0.1*edfa5->gain,0.1*edfa5->outputPower);
       ipower=edfa5->inputPower;
       opower=edfa5->outputPower;
       temp=edfa5->temperature;
       for (n=0;n<pumpcount;n++)
       {
        pump_info.working_current=edfa5->pump_info[n].working_current;
        pump_info.power=edfa5->pump_info[n].power;
        pump_info.temperature=edfa5->pump_info[n].temperature;
        pump_info.cool_current=edfa5->pump_info[n].cool_current;
       }
   }
    //printf("working_current:%s power:%s cool_current:%s temperature:%s\n",pump_working_current[0],pump_power[0],pump_cool_current[0],pump_temp[0]);
  }
  else
  {
    return websWrite(wp,T("var work_mode=0;"));
  }
  //printf("working_current:%s power:%s cool_current:%s temperature:%s\n",pump_working_current[0],pump_power[0],pump_cool_current[0],pump_temp[0]);
  bytes=websWrite(wp,T("var slot=%d,utype=%d,pumpcount=%d;\
  var pump=new Array(%d);\
  var ipower=%d,opower=%d,temp=%d,work_mode=%d,val=%d;"),slot,utype,pumpcount,pumpcount,ipower,opower,temp,working_mode,gain);
  if (utype==EDFA_TYPE3_WITH_OSC)
  {
    ddm_t *ddm;
    ddm=getPortDdmInfo(0,slot,1);
    if (ddm!=NULL)
     bytes+=websWrite(wp, T("var sfprpower=%d,sfptpower=%d,sfptemp=%d,sfpvolt=%d,sfpbias=%d;"),ddm->rxPower,ddm->txPower,ddm->temperature,ddm->rxVolt,ddm->rxBiasCurrent);
  }
  for (n=0;n<pumpcount;n++)
  {
   bytes+=websWrite(wp,T("pump[%d]={working_current:%d,power:%d,temperature:%d,cool_current:%d,pump_status:%d};"),n,pump_info.working_current,pump_info.power,pump_info.temperature,pump_info.cool_current,pump_status[n]);
  }
  return bytes;
}
static int aspEdfaInfo(int eid, webs_t wp, int argc, char_t **argv)
{
  //char buf[500];
  int i;
  char pumpcount=1;
  int nBytesSent=0,slot;
  uchar_t utype=1;
  //edfaProperty_t ep;
  //edfaThreshold_t *getEdfaThreshold(uchar_t chassis,uchar_t slot);
  slot=getSelectedUnit();
  edfaProperty_t  *ep=getEdfaProperty(0,slot);
  edfaThreshold_t *et=getEdfaThreshold(0,slot);
  if (NULL==et)
    return 0;
  //utype=getUnitType(0,slot);
  /*if (NULL!=et)
    printf("MinTemperature=%d\n",et->minTemperature);
  else
    printf("EdfaThreshold on #%d is null!\n",slot);
  */
  if (NULL!=ep)
  {
    utype=ep->edfa_type;
    pumpcount=ep->pump_num;
  }
  if (pumpcount>2)
     pumpcount=2;
  //sprintf(buf,"var minipower=%d,maxipower=%d,minopower=%d,maxopower=%d,temp=60;",);
  nBytesSent+=websWrite(wp, T("var utype=%d, pumpcount=%d;"),utype,pumpcount);
  nBytesSent+=websWrite(wp, T("var minipower=%d,minopower=%d,mintemp=%d,maxtemp=%d;"),et->minInputPower,et->minOutputPower,-40,85);
  /*for (i=0;i<pumpcount;i++)
  {
    nBytesSent+=websWrite(wp, T("pump[i]={mintemp%d:123,maxtemp%d:12.3,mincurrent%d:123,maxcurrent%d:12.3,};"),i+1,i+1,i+1,i+1);
  }
*/
nBytesSent+=websWrite(wp, T("\nvar pumpt=new Array(%d);"),pumpcount);
//nBytesSent+=websWrite(wp, T("var mintemp=-40,maxtemp=85,minipower=-30,minopower=3"));
nBytesSent+=websWrite(wp, T("pumpt[0]={mintemp:-20,maxtemp:85,mincurrent:200,maxcurrent:300};"));
if (pumpcount>1)
 nBytesSent+=websWrite(wp, T("pumpt[1]={mintemp:-35,maxtemp:80,mincurrent:230,maxcurrent:350};"));
return nBytesSent;
}

int aspEdfaView(webs_t wp, int slot,uchar_t cclass,uchar_t uclass,uchar_t utype)
{
   int nBytesSent=0,n;
   char run_led=2,/*in_led=2,out_led=2,pta_led=2,mta_led=2,*/leds[2][2]={2};
   char power[10]="";
   char rt[4][5]={"MON","IN","OUT"};
   char run_led_label[6]="";
   char r2_led_label[2][6]={"PTA","MTA"};
   char port[3],pcount=3;
   edfaInfo_t *edfa;
   //printf("Edfa on slot %d\n",slot );
   if (utype==2)
       pcount=5;
   //printf("Edfa on slot %d utype=%d\n",slot,utype );
   edfa=getEdfaInfo(0,slot);
   if (NULL!=edfa)
   {
     if (utype!=5)
     {
      if (edfa->run_led>0 && edfa->run_led<4)
        run_led=edfa->run_led;
      else
        run_led=0;
      /*
      in_led=edfa->in_led<2?1:2;
      out_led=edfa->out_led<2?1:2;
      pta_led=edfa->pta_led<2?1:2;
      mta_led=edfa->mta_led<2?1:2;
      */
      leds[0][0]=edfa->in_led<4?edfa->in_led:0;
      leds[0][1]=edfa->out_led<4?edfa->out_led:0;
      leds[1][0]=edfa->pta_led<4?edfa->pta_led:0;
      leds[1][1]=edfa->mta_led<4?edfa->mta_led:0;
     }
     else
     {
        edfa5Info_t *edfa5=(edfa5Info_t*)edfa;
        run_led=edfa5->mta_led;
        leds[0][0]=edfa5->in_led<4?edfa5->in_led:0;
        leds[0][1]=edfa5->out_led<4?edfa5->out_led:0;
        leds[1][0]=edfa5->osc_leds[0]<4?edfa5->osc_leds[0]:0;
        leds[1][1]=edfa5->osc_leds[1]<4?edfa5->osc_leds[1]:0;
     }
   }
   //else
   //    printf("NULL!=edfa\n");
   //printf("Edfa On Slot #%d\n",slot );
   if (utype!=3)
   {
     if (utype!=5)
        strcpy(run_led_label,"RUN");
     else
     {
       strcpy(run_led_label,"MTA");
       strcpy(r2_led_label[0],"OSC1");
       strcpy(r2_led_label[1],"OSC2");
     }
   }
   else
      strcpy(run_led_label,"OSC");
   nBytesSent +=websWrite(wp, T("\n\n<div class='ubd1'>\
<div class='un'><a href='/goform/slot?cid=0&uid=%d' target='mf'>EDFA</a></div>\n\
<div class='leds'>\n\
	<div class='ll'>PWR</div>\n\
	<div class='led'><img src='images/led_1.png' /></div>\n\
	<div class='led'><img src='images/led_%d.png' /></div>\n\
	<div class='ll'>%s</div>\n\
	<div class='ll'>IN</div>\n\
	<div class='led'><img src='images/led_%d.png' /></div>\n\
	<div class='led'><img src='images/led_%d.png' /></div>\n\
	<div class='ll'>OUT</div>\n\
	<div class='ll'>%s</div>\n\
	<div class='led'><img src='images/led_%d.png' /></div>\n\
	<div class='led'><img src='images/led_%d.png' /></div>\n\
	<div class='ll'>%s</div>\n\
	<div class='clr'></div>\n\
</div>\n"),slot,run_led,run_led_label,leds[0][0],leds[0][1],r2_led_label[0],leds[1][0],leds[1][1],r2_led_label[1]);
port[0]=2;
port[1]=leds[0][0];//in_led;
if (port[1]!=1)
   port[1]=2;
port[2]=leds[0][1];//out_led;
if (port[2]!=1)
   port[2]=2;
for (n=0;n<pcount;n++)
 {
  if (n<3)
  nBytesSent +=websWrite(wp, T("<div class='adpter' style='margin-bottom:20px;'>\
  <div class='pl'>%s</div>\n\
  <div class='pimg'><img src='images/fc-upc-%d.gif'"),rt[n],port[n]);
  else
  {
    nBytesSent +=websWrite(wp, "<div class='adpter' style='margin-bottom:15px;'>");
    if (n>3)
        nBytesSent +=websWrite(wp, "<div class='pl'>ORX</div>\n");
    else
        nBytesSent +=websWrite(wp, "<div class='pl'>OTX</div>\n");
   nBytesSent +=websWrite(wp, "<div class='pimg'><img src='images/fc-upc-2.gif'");
  }
  if (NULL!=edfa)
  {
    if (1==n)
    {
     sprintf(power,"%.1f",edfa->inputPower*0.1);
     #ifdef DEFAULT_LANG_CN
     nBytesSent +=websWrite(wp, T(" title='输入光功率:%sdBm'"),power);
     #else
     nBytesSent +=websWrite(wp, T(" title='Input power:%sdBm'"),power);
     #endif
    }
    else if (2==n)
    {
     sprintf(power,"%.1f",edfa->outputPower*0.1);
     #ifdef DEFAULT_LANG_CN
          nBytesSent +=websWrite(wp, T(" title='输出光功率:%sdBm'"),power);
     #else
          nBytesSent +=websWrite(wp, T(" title='Output power:%sdBm'"),power);
     #endif
    }
  }
  nBytesSent +=websWrite(wp, T("></div>\n\
  </div>\n\
<div class='clr'></div>"));
}
nBytesSent +=websWrite(wp, T("</div>"));
return nBytesSent;
}
/*
static void formEdfaThreshold(webs_t wp, char_t *path, char_t *query)
{
  char_t *slot,*value;
  //char set_cmd[50];
  int unit;
  uchar_t index;
  //ddmThreshold_t *ddmt;
  //groupDdmThreshold_t *gddmt;
  slot = websGetVar(wp, T("uid"), T("0"));
  //port = websGetVar(wp, T("port"), T("0"));
  unit=atoi(slot);
  //index=atoi(port);

  if (unit>0 && unit<16)
  {  
    int i;
    char item[10];
    setInfoSet_t infoset;
    infoset.chassis=0;
    infoset.infoset_id=EDFA_THRESHOLD_ID;
    infoset.slot=unit;
    infoset.infoset_type='S';

    value=websGetVar(wp, T("minip"), T("-40"));
    infoset.infoItems[0].item_id=1;
    infoset.infoItems[0].item_value=atoi(value) & 0xff;
    
    value=websGetVar(wp, T("minop"), T("-40"));
    infoset.infoItems[1].item_id=2;
    infoset.infoItems[1].item_value=atoi(value) & 0xff;

    for (i=0;i<3;i++)
    {
      sprintf(item,"mint%d",i);
      value=websGetVar(wp, T(item), T("0"));
      if (0==i)
         index=3;
      else
         index=8+i;
      infoset.infoItems[i+2].item_id=index;
      infoset.infoItems[i+2].item_value=atoi(value) & 0xff;
      sprintf(item,"maxt%d",i);
      value=websGetVar(wp, T(item), T("0"));
      infoset.infoItems[i+3].item_id=index+1;
      infoset.infoItems[i+3].item_value=atoi(value) & 0xff;
    }
    index=2+i*2;
    for (i=1;i<3;i++)
    {
      sprintf(item,"minc%d",i);
      value=websGetVar(wp, T(item), T("0"));
      infoset.infoItems[i+index].item_id=7+(i-1)*4;
      infoset.infoItems[i+index].item_value=atoi(value);

      sprintf(item,"maxc%d",i);
      value=websGetVar(wp, T(item), T("0"));
      infoset.infoItems[i+index].item_id=8+(i-1)*4;
      infoset.infoItems[i+index].item_value=atoi(value);
    }
    setShortInfoItems(infoset);
    insertOperateLog(LOG_EDFA_THRESHOLD_HCONF,0,0,WebMode,mod,unit,0);
 }
 else 
     printf("maxtx=NULL\n");
  sleep(1);
  websRedirect(wp,"edfat.asp");
}
*/
static void formEdfaConf(webs_t wp, char_t *path, char_t *query)
{
  char_t *slot,*value,*mode;
  int unit;
  short val;
  float fval;
  int ival;
  //char mode;
  uchar_t uclass;
  slot = websGetVar(wp, T("uid"), T("0"));
  unit=atoi(slot);
  uclass=getUnitClass(0,unit);
  if (uclass<1)
  {
    websRedirect(wp,"ukw_unit.asp");
    return;
  }
  if (unit>0 && unit<16)
  {
    setInfoSet_t infoset;
    memset(&infoset,0,sizeof(setInfoSet_t));
    infoset.chassis=0;
    infoset.index=1;
    infoset.infoset_id=EDFA_STATUS_INFOSET_ID;
    infoset.slot=unit;
    infoset.infoset_type='S';
    value=websGetVar(wp, T("act"), T("0"));
    if (!gstrcmp(value,"1"))
    {
    //infoset.infoItems[0].item_id=8;
    mode=websGetVar(wp, T("wm"), T("1"));
    printf("mode=%s\n",mode);
   // infoset.infoItems[0].item_value=atoi(value) & 0xff;

    value=websGetVar(wp, T("mval"), T("0.5"));
    infoset.infoItems[0].item_value=atoi(value);
    fval=10*atof(value);
    infoset.infoItems[0].item_value=(short)fval;
    //val=(short)fval;
    //printf("slot=%d value=%s %f ival=%d\n",unit,value,fval,val);
    if (!gstrcmp(mode,"1"))
      infoset.infoItems[0].item_id=12;
    else
      infoset.infoItems[0].item_id=10;

    value=websGetVar(wp, T("ud"), T("1"));
    if (!gstrcmp(value,"2"))
    {
      //sprintf(buf,"-%s",
      infoset.infoItems[0].item_value=(short)(0-infoset.infoItems[0].item_value);
    }
  }
  else if (!gstrcmp(value,"2"))
  {
   // printf("slot=%d value=%d %0x:%0x\n",unit,infoset.infoItems[1].item_value,infoset.infoItems[1].item_value>>8,infoset.infoItems[1].item_value&0xff);
    //ival=(infoset.infoItems[1].item_value & 0xff)<<8;
    //ival=ival |  ((infoset.infoItems[1].item_value & 0xff00)>>8);
    infoset.infoItems[0].item_id=6;
    value=websGetVar(wp, T("ps1"), T("1"));
    
    infoset.infoItems[0].item_value=atoi(value) & 0xff;
    
    value=websGetVar(wp, T("ps2"), T("0"));
    if (gstrcmp(value,"0"))
    {
     infoset.infoItems[1].item_id=7;
     infoset.infoItems[1].item_value=atoi(value) & 0xff;
    }
  }
  else
      websRedirect(wp,"edfa.asp");
   // printf("slot=%d mode=%d value=%d ps1=%d\n",infoset.slot,infoset.infoItems[0].item_value,infoset.infoItems[1].item_value,infoset.infoItems[2].item_value);
    setShortInfoItems(infoset);
    insertOperateLog(LOG_UNIT_EDFA_CONF,0,0,WebMode,mod,unit,0);
    sleep(3);
  }
  websRedirect(wp,"edfa.asp");
}
void edfaAspDefine()
{
 websAspDefine(T("EDFAT"), aspEdfaInfo);
 websFormDefine(T("edfaConf"), formEdfaConf);
}
