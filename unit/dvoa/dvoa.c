#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unit.h>
#include <infoset.h>
#include <visipc.h>
#include <dvoa.h>

int insertDvoaInfoSets(uchar_t chassis,uchar_t slot)
{
  status_infoset_t infosets[1];
  memset(&infosets[0],0,sizeof(status_infoset_t));
  infosets[0].infoset_id=DVOA_STATUS_INFOSET_ID;
 
  infosets[0].index=1;     
  infosets[0].infoset_type=2;
  infosets[0].infoset_size=32;
  infosets[0].item_count=20;
  return insertInfoSet(chassis,slot,infosets,1);
}

dvoa_info_t *getDvoaInfo(uchar_t chassis,uchar_t slot)
{
  dvoa_info_t *dvoa;
  dvoa=(dvoa_info_t *)getInfoSetValues(chassis,slot,DVOA_STATUS_INFOSET_ID,1);  
  return  dvoa;
}
boolean_t getDvoaViewInfo(uchar_t chassis,uchar_t slot,uchar_t uclass,uchar_t utype,unitViewInfo_t *info)
{
  dvoa_info_t *dvoa=getDvoaInfo(chassis,slot);
  if (NULL==dvoa)
     return FALSE;
  memset(info,0,sizeof(unitViewInfo_t));
  info->leds.leds_status[0][1]=dvoa->auto_led;
  info->leds.leds_status[0][2]=dvoa->los2_led;
  info->leds.leds_status[1][1]=dvoa->lock_led;
  info->leds.leds_status[1][2]=dvoa->los3_led;
  //printf();
  info->ports.ports_status[0]=2;
  info->ports.ports_status[1]=2;
  info->ports.portCount=2;

  info->uclass=UC_DVOA;
  info->leds.col_count=2;
  strcpy(info->unit_name,"DVOA");

  strcpy(info->ports.ports_text[0][0],"IN");
  strcpy(info->ports.ports_text[0][1],"OUT");

  info->leds.run_led=dvoa->los1_led;
  strcpy(info->leds.run_text,"H");
  strcpy(info->leds.leds_text[0][0],"Auto");
  strcpy(info->leds.leds_text[0][1],"Lock");
  strcpy(info->leds.leds_text[1][0],"M");
  strcpy(info->leds.leds_text[1][1],"L");
  /*
  info->ports.has_info=1;
  strcpy(info->ports.ports_info_name[0],"输入功率");
  strcpy(info->ports.ports_info_name[1],"输出功率");
  strcpy(info->ports.ports_info_unit[0],"dBm");
  strcpy(info->ports.ports_info_unit[1],"dBm");
  snprintf(info->ports.ports_info_val[1][0],8,"%.2f",edfa->inputPower*0.1);
  snprintf(info->ports.ports_info_val[2][1],8,"%.2f",edfa->outputPower*0.1);
  */
  return TRUE;
}
