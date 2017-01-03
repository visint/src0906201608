#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <public.h>
#include <unit.h>
#include <infoset.h>
#include <visipc.h>
#include <ddm.h>
#include <unit.h>
#include <tap.h>
#include <otu.h>

int insertOtapInfoSets(uchar_t chassis,uchar_t slot,uchar_t utype)
{
 uchar_t tap_infoset_count=8;
 
  uchar_t index=0;

  status_infoset_t infosets[tap_infoset_count];
  //ddmThreshold_t *ddmt,*pddmt;

  int n;
  //printf("insertOtapInfoSets:#%d\r\n",slot);
  //printf("run in insertOtuInfoSets slot=%d\n",slot);
  for (n=0;n<tap_infoset_count;n++)
    memset(&infosets[n],0,sizeof(status_infoset_t));
  //addInfoSet(infosets,tap_infoset_count,UNIT_PROPERTY_INFOSET_ID,1,1,22,22);
  if (utype<6)
  {
  addInfoSet(infosets,tap_infoset_count,TAP_POWER_INFOSET_ID,1,2,20,10);
  addInfoSet(infosets,tap_infoset_count,TAP_POWER_INFOSET_ID,2,2,20,10);
  }
  else
  {
   addInfoSet(infosets,tap_infoset_count,TAP_POWER_INFOSET_ID,1,2,36,13);
   addInfoSet(infosets,tap_infoset_count,TAP_POWER_INFOSET_ID,2,2,36,13);
  }
  addInfoSet(infosets,tap_infoset_count,DDM_INFOSET_ID,1,2,20,10);
  addInfoSet(infosets,tap_infoset_count,DDM_INFOSET_ID,2,2,20,10);

  addInfoSet(infosets,tap_infoset_count,OTU_SFP_PROPERTY_INFOSET_ID,1,2,32,16);
  addInfoSet(infosets,tap_infoset_count,OTU_SFP_PROPERTY_INFOSET_ID,2,2,32,16);
  addInfoSet(infosets,tap_infoset_count,OTU_STATUS_INFOSET_ID,1,1,36,36);
  addInfoSet(infosets,tap_infoset_count,OTU_SFP_FEC_INFOSET_ID,1,1,32,32);

  return (insertInfoSet(chassis,slot,infosets,tap_infoset_count));
}
boolean_t getOtapViewInfo(uchar_t chassis,uchar_t slot,uchar_t uclass,uchar_t utype,unitViewInfo_t *info)
{
//printf("getOtapViewInfo:#%d\r\n",slot);
if (utype!=5)
{
  switch (utype)
  {
    case 1:
      strcpy(info->unit_name,"OTAP");
      break;
    case 6:
    case 7:
      strcpy(info->unit_name,"MON");
      break;
  }
}
else
  strcpy(info->unit_name,"OSC");
strcpy(info->leds.run_text,"B");
strcpy(info->utype_text,"");
info->view_images_count=2;
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
if (utype!=5)
{
sprintf(info->view_images[0],"otap_h_led%d_%d",led_A,led_B);
sprintf(info->view_images[1],"otap_h_port_%d",utype);
}
else
{
sprintf(info->view_images[0],"osc_h_led%d_%d",led_A,led_B);
strcpy(info->view_images[1],"osc_h_port_1");
}
return TRUE;
}

