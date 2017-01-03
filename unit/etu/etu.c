#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unit.h>
#include <infoset.h>
#include <visipc.h>
#include <etu.h>

etuInfo_t *getEtuInfo(uchar_t chassis,uchar_t slot)
{
  char *values;
  etuInfo_t *etu=NULL;
  values=getInfoSetValues(chassis,slot,ETU_STATUS_INFOSET_ID,1);
  if (NULL!=values)
  {
      etu=(etuInfo_t*)(values);
      //printf("getChassisInfo NOT null!\n");  
  }
  else
      printf("#%d EtuInfo(#%d) is null!\n",slot,ETU_STATUS_INFOSET_ID);  
  return  etu;
}

boolean_t getEtuViewInfo(uchar_t chassis,uchar_t slot,uchar_t uclass,uchar_t utype,unitViewInfo_t *info)
{
  etuInfo_t *etu=getEtuInfo(chassis,slot);
  if (NULL==etu)
     return FALSE;
  memset(info,0,sizeof(unitViewInfo_t));
  strcpy(info->utype_text,"");
  strcpy(info->leds.leds_text[0][0],"1");
  strcpy(info->leds.leds_text[0][1],"2");

  strcpy(info->view_images[0],"epu");
  strcpy(info->view_images[1],"sfp");
  strcpy(info->view_images[2],"epu");
  strcpy(info->view_images[3],"sfp");
  info->ports.portCount=4;
  
   if (etu->lan1_speed!=1)
    info->leds.leds_status[0][0]=0;
   else
    info->leds.leds_status[0][0]=1;
   if (etu->lan1_link_status!=1)
     info->leds.leds_status[0][1]=0;
   else
     info->leds.leds_status[0][1]=1;
   if (etu->lan1_duplex!=1)
     info->leds.leds_status[0][2]=0;
   else
     info->leds.leds_status[0][2]=1;
   if (etu->sfp1_link_status!=1)
     info->leds.leds_status[0][3]=0;
   else
     info->leds.leds_status[0][3]=1;

   if (etu->lan2_speed!=1)
    info->leds.leds_status[1][0]=0;
   else
    info->leds.leds_status[1][0]=1;
   if (etu->lan2_link_status!=1)
     info->leds.leds_status[1][1]=0;
   else
     info->leds.leds_status[1][1]=1;
   if (etu->lan2_duplex!=1)
     info->leds.leds_status[1][2]=0;
   else
     info->leds.leds_status[1][2]=1;
   if (etu->sfp2_link_status!=1)
     info->leds.leds_status[1][3]=0;
   else
     info->leds.leds_status[1][3]=1;
  //info->leds.leds_status[1][2]=etuInfo->mta_led;
  
  info->ports.ports_status[0]=etu->lan1_link_status;
  if (etu->sfp1_exist==1)
     info->ports.ports_status[1]=etu->sfp1_link_status;
  else
     info->ports.ports_status[1]=0;
  info->ports.ports_status[2]=etu->lan2_link_status;
  if (etu->sfp2_exist==1)
     info->ports.ports_status[3]=etu->sfp2_link_status;
  else
     info->ports.ports_status[3]=0;
  info->uclass=UC_ETU;
  info->leds.col_count=4;
  strcpy(info->unit_name,"ETU");
  strcpy(info->ports.ports_text[0][0],"LAN1");
  strcpy(info->ports.ports_text[0][1],"SFP1");
  strcpy(info->ports.ports_text[0][2],"LAN2");
  strcpy(info->ports.ports_text[0][3],"SFP2");
}

etuStatus_t *getEtuStatus(uchar_t chassis,uchar_t slot)
{
  char *values;
  etuStatus_t *etu=NULL;
  values=getInfoSetValues(chassis,slot,ETU_STATUS_INFOSET_ID,1);
  if (NULL!=values)
  {
      etu=(etuStatus_t*)(values);
      //printf("getChassisInfo NOT null!\n");  
  }
  else
      printf("#%d EdfaInfo(#%d) is null!\n",slot,ETU_STATUS_INFOSET_ID);  
  return  etu;
}

int insertEtuInfoSets(uchar_t chassis,uchar_t slot)
{
  #define ETU_INFOSET_COUNT 1

  status_infoset_t infosets[1];
  //ddmThreshold_t *ddmt,*pddmt;

  int n;
  //deleteSlotNodes(chassis,0);

  for (n=0;n<ETU_INFOSET_COUNT;n++)
    memset(&infosets[n],0,sizeof(status_infoset_t));
  infosets[0].infoset_id=ETU_STATUS_INFOSET_ID;
  infosets[0].index=1;     
  infosets[0].infoset_type=1;
  infosets[0].infoset_size=20;
  infosets[0].item_count=20;
  /*
  infosets[1].index=1;
  infosets[1].infoset_type=3;
  infosets[1].infoset_size=28;
  infosets[1].item_count=12;
  infosets[1].infoset_id=EDFA_THRESHOLD_ID;
  */
  return insertInfoSet(chassis,slot,infosets,ETU_INFOSET_COUNT);
}
