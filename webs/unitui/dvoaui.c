#include <sys/sysinfo.h>
#include <time.h>
#include "../wsIntrn.h"
#include "../../include/unit.h"
#include <visipc.h>
#include <sqlite3.h>
#include <dbutil.h>
#include <log.h>
#include <dvoa.h>
#include "../unitwebs.h"

boolean_t getDvoaWebInfo(uchar_t chassis,uchar_t cclass,uchar_t slot,uchar_t uclass,uchar_t utype,web_unit_t *punit)
{
  //printf("run into getDvoaWebInfo..\n");
  punit->uclass=UC_DVOA;
  punit->slotCount=1;
  strcpy(punit->height,"60%");
  strcpy(punit->led.run_text,"OPM");
  //info->port_groups[0].isTxtAtTwoSide=0;
  strcpy(punit->port_groups[0].pclass,"tm15 imgw30 imgh30");
  strcpy(punit->port_groups[0].pimg,"upc_");
  
  return TRUE;
}

int aspDvoaStatus(webs_t wp,uchar_t slot)
{
  //char buf[300];
  char power[10]; //IN光功率
  char out_power[10]; //Out光功率
  int bytes=0;

  uchar_t utype;
  //short value;
  slot=getSelectedUnit();
  //printf("aspDvoaStatus..\n");
  dvoa_info_t *dvoa;
  dvoa=getDvoaInfo(0,slot);
  if (dvoa==NULL)
  {
    bytes=websWrite(wp,"var utype=0;");
    return bytes;
  }

  utype=getUnitType(0,slot);
  bytes=websWrite(wp,T("var utype=%d"),utype);
  bytes=websWrite(wp,T(",mode=%d"),dvoa->mode);
  bytes=websWrite(wp,T(",wl=%d"),dvoa->wave_length);
  bytes=websWrite(wp,T(",lock=%d"),dvoa->lock_status);
  sprintf(power,"%.1f",0.1*dvoa->in_power);
  bytes=websWrite(wp,T(",ipower=%s"),power);
  sprintf(power,"%.1f",0.1*dvoa->out_power);
  bytes=websWrite(wp,T(",opower=%s"),power);
  sprintf(power,"%.1f",0.1*dvoa->setting_otu_power);
  bytes=websWrite(wp,T(",spower=%s"),power);
  sprintf(power,"%.1f",0.1*dvoa->insertion_loss);
  bytes=websWrite(wp,T(",iloss=%s;"),power);
  return bytes;
 /*
  if (pumpcount<1)
     pumpcount=1;
  else if (pumpcount>2)
     pumpcount=2;
  char pump_working_current[2][10]={""};
  char pump_power[2][10]={""};
  char pump_temp[2][10]={""};
  char pump_cool_current[2][10]={""};

  if (NULL!=et)
  {
    pump_status[0]=et->pump1_status;
    pump_status[1]=et->pump2_status;
    if (1==et->work_mode)
      sprintf(gain,"%.1f",0.1*et->agc_or_apc_value);
    else
      sprintf(gain,"%.1f",0.1*et->outputPower);
    //printf("gain=%s gain=%d %.1f\n",gain,et->agc_or_apc_value,0.1*et->agc_or_apc_value);
    //value=et->agc_or_apc_value;
    sprintf(ipower,"%.1f",0.1*et->inputPower);
    sprintf(opower,"%.1f",0.1*et->outputPower);
    sprintf(temp,"%.1f",0.1*et->temperature);
    for (n=0;n<pumpcount;n++)
    {
      sprintf(pump_working_current[n],"%d",et->pump_status[n].working_current);
      sprintf(pump_power[n],"%.1f",0.1*(et->pump_status[n].power));
      sprintf(pump_temp[n],"%.1f",0.1*(et->pump_status[n].temperature));
      sprintf(pump_cool_current[n],"%d",et->pump_status[n].cool_current);
    }
    //printf("working_current:%s power:%s cool_current:%s temperature:%s\n",pump_working_current[0],pump_power[0],pump_cool_current[0],pump_temp[0]);
  }
  else
  {
    return websWrite(wp,T("var work_mode=0;"));
  }
  //printf("working_current:%s power:%s cool_current:%s temperature:%s\n",pump_working_current[0],pump_power[0],pump_cool_current[0],pump_temp[0]);
  bytes=websWrite(wp,T("var utype=%d,pumpcount=%d;\
  var pump=new Array(%d);\
  var ipower=%s,opower=%s,temp=%s,work_mode=%d,val=%s;"),utype,pumpcount,pumpcount,ipower,opower,temp,et->work_mode,gain);
  for (n=0;n<pumpcount;n++)
  {
   bytes+=websWrite(wp,T("pump[%d]={working_current:%s,power:%s,temperature:%s,cool_current:%s,pump_status:%d};"),n,pump_working_current[n],pump_power[n],pump_temp[n],pump_cool_current[n],pump_status[n]);
  }
  */
  return bytes;
}
static int aspDvoaInfo(int eid, webs_t wp, int argc, char_t **argv)
{
  //char buf[500];
  int i;
  char pumpcount=1;
  int nBytesSent=0,slot;
  uchar_t utype=1;
  //dvoaProperty_t ep;
  //dvoaThreshold_t *getDvoaThreshold(uchar_t chassis,uchar_t slot);
  slot=getSelectedUnit();
  /*
  dvoaProperty_t  *ep=getDvoaProperty(0,slot);
  dvoaThreshold_t *et=getDvoaThreshold(0,slot);
  if (NULL==et)
    return 0;
  if (NULL!=ep)
  {
    utype=ep->dvoa_type;
    pumpcount=ep->pump_num;
  }
  if (pumpcount>2)
     pumpcount=2;
  //sprintf(buf,"var minipower=%d,maxipower=%d,minopower=%d,maxopower=%d,temp=60;",);
  nBytesSent+=websWrite(wp, T("var utype=%d, pumpcount=%d;"),utype,pumpcount);
  nBytesSent+=websWrite(wp, T("var minipower=%d,minopower=%d,mintemp=%d,maxtemp=%d;"),et->minInputPower,et->minOutputPower,-40,85);

nBytesSent+=websWrite(wp, T("\nvar pumpt=new Array(%d);"),pumpcount);
//nBytesSent+=websWrite(wp, T("var mintemp=-40,maxtemp=85,minipower=-30,minopower=3"));
nBytesSent+=websWrite(wp, T("pumpt[0]={mintemp:-20,maxtemp:85,mincurrent:200,maxcurrent:300};"));
if (pumpcount>1)
 nBytesSent+=websWrite(wp, T("pumpt[1]={mintemp:-35,maxtemp:80,mincurrent:230,maxcurrent:350};"));
*/
  return nBytesSent;
}

int aspDvoaView(webs_t wp, int slot,uchar_t cclass,uchar_t uclass,uchar_t utype)
{
   int nBytesSent=0,n;
   char run_led=2,in_led=2,out_led=2,pta_led=2,mta_led=2;
   char power[10]="";
   char rt[4][5]={"MON","IN","OUT"};
   char port[3];
   dvoa_info_t *dvoa;

   //printf("Dvoa on slot %d\n",slot );
   dvoa=getDvoaInfo(0,slot);
   if (NULL!=dvoa)
   {
     /*
     run_led=dvoa->run_led<2?1:2;
     in_led=dvoa->in_led<2?1:2;
     out_led=dvoa->out_led<2?1:2;
     pta_led=dvoa->pta_led<2?1:2;
     mta_led=dvoa->mta_led<2?1:2;
    */
   }
   //else
   //    printf("NULL!=dvoa\n");
   //printf("Dvoa On Slot #%d\n",slot );
   nBytesSent +=websWrite(wp, T("\n\n<div class='ubd1'>\
<div class='un'><a href='/goform/slot?cid=0&uid=%d' target='mf'>EDFA</a></div>\n\
<div class='leds'>\n\
	<div class='ll'>PWR</div>\n\
	<div class='led'><img src='images/led_1.png' /></div>\n\
	<div class='led'><img src='images/led_%d.png' /></div>\n\
	<div class='ll'>RUN</div>\n\
	<div class='ll'>IN</div>\n\
	<div class='led'><img src='images/led_%d.png' /></div>\n\
	<div class='led'><img src='images/led_%d.png' /></div>\n\
	<div class='ll'>OUT</div>\n\
	<div class='ll'>PTA</div>\n\
	<div class='led'><img src='images/led_%d.png' /></div>\n\
	<div class='led'><img src='images/led_%d.png' /></div>\n\
	<div class='ll'>MTA</div>\n\
	<div class='clr'></div>\n\
</div>\n"),slot,run_led,in_led,out_led,pta_led,mta_led);

port[0]=2;
port[1]=in_led;
port[2]=out_led;
/*
for (n=0;n<3;n++)
 {
  nBytesSent +=websWrite(wp, T("<div class='adpter' style='margin-bottom:40px;'>\
  <div class='pl'>%s</div>\n\
  <div class='pimg'><img src='images/fc-upc-%d.gif'"),rt[n],port[n]);

  if (NULL!=dvoa)
  {
    if (1==n)
    {
     sprintf(power,"%.1f",dvoa->inputPower*0.1);
     nBytesSent +=websWrite(wp, T(" title='输入光功率:%sdBm'"),power);
    }
    else if (2==n)
    {
     sprintf(power,"%.1f",dvoa->outputPower*0.1);
     nBytesSent +=websWrite(wp, T(" title='输出光功率:%sdBm'"),power);
    }
  }
  nBytesSent +=websWrite(wp, T("></div>\n\
  </div>\n\
<div class='clr'></div>"));
}
nBytesSent +=websWrite(wp, T("</div>"));
*/
return nBytesSent;
}
/*
static void formDvoaThreshold(webs_t wp, char_t *path, char_t *query)
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
  websRedirect(wp,"dvoat.asp");
}
*/
static void formDvoaConf(webs_t wp, char_t *path, char_t *query)
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
      websRedirect(wp,"dvoa.asp");
   // printf("slot=%d mode=%d value=%d ps1=%d\n",infoset.slot,infoset.infoItems[0].item_value,infoset.infoItems[1].item_value,infoset.infoItems[2].item_value);
    setShortInfoItems(infoset);
    insertOperateLog(LOG_UNIT_DVOA_CONF,0,0,WebMode,mod,unit,0);
    sleep(3);
  }
  websRedirect(wp,"dvoa.asp");
}
void dvoaAspDefine()
{
 websAspDefine(T("EDFAT"), aspDvoaInfo);
 websFormDefine(T("dvoaConf"), formDvoaConf);
}
