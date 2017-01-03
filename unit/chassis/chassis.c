#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <public.h>
#include <unit.h>
#include <infoset.h>
#include <visipc.h>
#include <chassis.h>

chassis_t *getChassisInfo(uchar_t chassis)
{
   char *values;
   chassis_t *cinfo=NULL;
   values=getInfoSetValues(chassis,0,CHASSIS_INFOSET_ID,1);
   if (NULL!=values)
   {
      cinfo=(chassis_t *)(values);
   }
   else
        printf("getChassisInfo is null!\n");
   return  cinfo;
}

chassisThreshold_t *getChassisThreshold(uchar_t chassis)
{
  char *values;
  chassisThreshold_t *threshold=NULL;
  values=getInfoSetValues(chassis,0,CHASSIS_THRESHOLD_INFOSET_ID,1);
  if (NULL!=values)
   {
      threshold=(chassisThreshold_t *)(values);  
   }
   return threshold;
}
#ifdef SETTABLE_INFOSET
int setChassisThreshold(setInfoSet_t infoset)
{
  char n,m=3,flag;
  char message[512];
  short value;
  memset(message,0,512);
  for (n=0;n<MAX_SETTING_ITEM_COUNT;n++)
  { 
    flag=1;
    if (infoset.infoItems[n].item_id<1)
      break;
    switch (infoset.infoItems[n].item_id)
    {
       case MIN_CHASSIS_TEMP:
         //threshold->min_temperature=infoset.infoItems[n].item_value;
         value=infoset.infoItems[n].item_value;
         break;
       case MAX_CHASSIS_TEMP :
         //threshold->max_temperature=infoset.infoItems[n].item_value;
         value=infoset.infoItems[n].item_value;
         break;
       case MIN_PWR_VOLT:
         //threshold->min_volt=infoset.infoItems[n].item_value;
         value=infoset.infoItems[n].item_value;
         break;
       case MAX_PWR_VOLT:
         //threshold->max_volt=infoset.infoItems[n].item_value;
         value=infoset.infoItems[n].item_value;
         break;
       default:
             flag=0;
    }
   if (flag)
    {
      message[m]=infoset.infoItems[n].item_id;
      //memcpy(&message[m+1],&value,sizeof(char));
      message[m+1]=(value & 0xff00)>>8;
      message[m+2]=value & 0xff;
      m+=3;
    }
  }
   if (m>3)
   {
     message[0]='S';
     message[1]=infoset.infoset_id;
     message[2]=infoset.index;
     sendMessageToUnit(infoset.chassis,17,message,infoset.useconds,0,0,0);
   }
  return 0;
}
#endif
