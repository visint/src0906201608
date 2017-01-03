#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <public.h>
#include <unit.h>
#include <infoset.h>
#include <visipc.h>
#include <edfa.h>

boolean_t getEdfaViewInfo(uchar_t chassis,uchar_t slot,uchar_t uclass,uchar_t utype,unitViewInfo_t *info)
{
  edfaInfo_t *edfa=getEdfaInfo(chassis,slot);
  if (NULL==edfa)
     return FALSE;
  short iPower,oPower;
  memset(info,0,sizeof(unitViewInfo_t));
  if (utype<5)
  {
  info->leds.run_led=edfa->run_led;
  info->leds.leds_status[0][1]=edfa->in_led;
  info->leds.leds_status[0][2]=edfa->out_led;
  info->leds.leds_status[1][1]=edfa->pta_led;
  info->leds.leds_status[1][2]=edfa->mta_led;
  
  info->ports.ports_status[0]=2;
  info->ports.ports_status[1]=edfa->in_led;
  info->ports.ports_status[2]=edfa->out_led;
  iPower=edfa->inputPower;
  oPower=edfa->outputPower;
  }
  else
  {
   edfa5Info_t *edfa5=(edfa5Info_t*)edfa;
   info->leds.run_led=edfa5->mta_led;
   info->leds.leds_status[0][1]=edfa5->in_led;
   info->leds.leds_status[0][2]=edfa5->out_led;
   info->leds.leds_status[1][1]=edfa5->osc_leds[0];
   info->leds.leds_status[1][2]=edfa5->osc_leds[1];
  
   info->ports.ports_status[0]=2;
   info->ports.ports_status[1]=edfa5->in_led;
   info->ports.ports_status[2]=edfa5->out_led;

   iPower=edfa5->inputPower;
   oPower=edfa5->outputPower;
  }
  if (utype==1)
     info->ports.portCount=3;
  else if (utype==2)
  {
    info->ports.portCount=5;
    info->ports.ports_status[3]=2;
    info->ports.ports_status[4]=2;

    strcpy(info->ports.ports_text[0][3],"O-TX");
    strcpy(info->ports.ports_text[0][4],"O-RX");
  }
  else
      info->ports.portCount=3;
  info->uclass=UC_EDFA;
  info->leds.col_count=2;
  strcpy(info->unit_name,"EDFA");
  strcpy(info->ports.ports_text[0][0],"MON");
  strcpy(info->ports.ports_text[0][1],"IN");
  strcpy(info->ports.ports_text[0][2],"OUT");

 // info->leds.run_led=edfa->run_led;
  if (utype!=3)
  {
    if (utype<5)
      strcpy(info->leds.run_text,"RUN");
    else
      strcpy(info->leds.run_text,"MTA");
  }
   else
      strcpy(info->leds.run_text,"OSC");
  
  strcpy(info->leds.leds_text[0][0],"IN");
  
  strcpy(info->leds.leds_text[1][0],"OUT");
  if (utype<5)
  {
    strcpy(info->leds.leds_text[0][1],"PTA");
    strcpy(info->leds.leds_text[1][1],"MTA");
  }
  else
  {
    strcpy(info->leds.leds_text[0][1],"OSC1");
    strcpy(info->leds.leds_text[1][1],"OSC2");
  }
  info->ports.has_info=1;
  #ifdef DEFAULT_LANG_CN
  strcpy(info->ports.ports_info_name[0],"输入功率");
  strcpy(info->ports.ports_info_name[1],"输出功率");
  #else
  strcpy(info->ports.ports_info_name[0],"Input power");
  strcpy(info->ports.ports_info_name[1],"Output power");
  #endif
  strcpy(info->ports.ports_info_unit[0],"dBm");
  strcpy(info->ports.ports_info_unit[1],"dBm");
  snprintf(info->ports.ports_info_val[1][0],8,"%.1f",iPower*0.1);
  snprintf(info->ports.ports_info_val[2][1],8,"%.1f",oPower*0.1);
  return TRUE;
}

edfaProperty_t *getEdfaProperty(uchar_t chassis,uchar_t slot)
{
   return (edfaProperty_t*)getUnitProperty(chassis,slot);
}
edfaInfo_t *getEdfaInfo(uchar_t chassis,uchar_t slot)
{
  char *values;
  edfaInfo_t *edfa=NULL;
  values=getInfoSetValues(chassis,slot,EDFA_STATUS_INFOSET_ID,1);
  if (NULL!=values)
  {
      edfa=(edfaInfo_t*)(values);
      //printf("getChassisInfo NOT null!\n");  
  }
  else
      printf("#%d EdfaInfo(#%d) is null!\n",slot,EDFA_STATUS_INFOSET_ID);  
  return  edfa;
}

edfaThreshold_t *getEdfaThreshold(uchar_t chassis,uchar_t slot)
{
  edfaThreshold_t *edft=NULL;
  char *values;
  //uchar_t index;
 
  values=getInfoSetValues(chassis,slot,EDFA_THRESHOLD_ID,1);
  if (NULL!=values)
  {
      edft=(edfaThreshold_t *)(values);
      //printf("getEdfaThreshold NOT null!\n");  
  }
  return edft;
}

int insertEdfaInfoSets(uchar_t chassis,uchar_t slot,uchar_t utype)
{
  //#define EDFA_INFOSET_COUNT 2
   int infoset_count;
  status_infoset_t infosets[3];
  //ddmThreshold_t *ddmt,*pddmt;

  int n;
  //deleteSlotNodes(chassis,0);

  for (n=0;n<3;n++)
    memset(&infosets[n],0,sizeof(status_infoset_t));
  infosets[0].infoset_id=EDFA_STATUS_INFOSET_ID;
  infosets[0].index=1;     
  infosets[0].infoset_type=2;
  infosets[0].infoset_size=36;
  infosets[0].item_count=25;

  infosets[1].index=1;
  infosets[1].infoset_type=3;
  infosets[1].infoset_size=28;
  infosets[1].item_count=12;
  infosets[1].infoset_id=EDFA_THRESHOLD_ID;
  //printf("#%d EDFA TYPE=%d",slot,utype);
  if (utype==EDFA_TYPE3_WITH_OSC) //带OSC EDFA
  {
    infosets[2].infoset_id=DDM_INFOSET_ID;
    infosets[2].index=1;     
    infosets[2].infoset_type=1;
    infosets[2].infoset_size=20;
    infosets[2].item_count=10;
    infoset_count=3;
  }
  else
      infoset_count=2;
  return insertInfoSet(chassis,slot,infosets,infoset_count);
}

/*
int setEdfaThreshold(setInfoSet_t infoset)
{
  char n,m=3,flag;
  char message[512]={0};
  short value;
 
  uchar_t item_id;
  //printf("setDdmThreshold\n");
  for (n=0;n<MAX_SETTING_ITEM_COUNT;n++)
  {
    flag=1;
    if (infoset.infoItems[n].item_id<1)
      break;
    value=infoset.infoItems[n].item_value & 0xff;
    message[m]=infoset.infoItems[n].item_id;
    //memcpy(&message[m+1],&value,sizeof(short));
    message[m+1]=(value & 0XFF00)>>8;
    message[m+2]= (value & 0XFF);
    m+=3;
    if (m>60)
        break;
  }
  if (m>3)
   {
     message[0]='S';
     message[1]=infoset.infoset_id;
     message[2]=infoset.index;
     sendMessageToUnit(infoset.chassis,infoset.slot,message,infoset.useconds);
   }
}
*/
