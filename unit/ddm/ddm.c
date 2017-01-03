#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <public.h>
#include <unit.h>
#include <infoset.h>
#include <visipc.h>
#include <ddm.h>
#include <otu.h>

ddm_t *getPortDdmInfo(uchar_t chassis,uchar_t slot,uchar_t port)
{
  ddm_t *ddm=NULL;
  char *values;
  uchar_t group;
  
  //printf("getChassisInfo!%d\n",DDM_INFOSET_ID);
  if (port<1 || port>8)
  {
   //printf("getChassisInfo port must be 1 to 8!\n");
   return NULL;
  }
  group=(port+1)/2;
  values=getInfoSetValues(chassis,slot,DDM_INFOSET_ID,group);
  if (NULL!=values)
  { 
      ddm=(ddm_t *)(values);
      if (port % 2==0)
       ddm++;
      //printf("getChassisInfo NOT null!\n");  
  }
  else
      printf("getPortDdmInfo is null slot=%d group=%d port=%d!\n",slot,group,(port+1)%2);
  return ddm;
}

ddmThreshold_t *getPortDdmThreshold(uchar_t chassis,uchar_t slot,uchar_t port)
{
  ddmThreshold_t *ddmt=NULL;
  char *values;
  //uchar_t index;
 
  if (port<1 || port>8)
    return NULL;
  
  values=getInfoSetValues(chassis,slot,DDM_THRESHOLD_ID,(port-1)/2+1);
  if (NULL!=values)
  {
      ddmt=(ddmThreshold_t *)(values);
      if (0==(port %2))
          ddmt++;
      //printf("getChassisInfo NOT null!\n");  
  }
  return ddmt;
}

groupDdmThreshold_t *getGroupDdmThreshold(uchar_t chassis,uchar_t slot,uchar_t group)
{
  groupDdmThreshold_t *ddmt=NULL;
  char *values;
  
  if (group<1 || group>4)
    return NULL;
  values=getInfoSetValues(chassis,slot,DDM_THRESHOLD_ID,group);
  if (NULL!=values)
   {
      ddmt=(groupDdmThreshold_t *)(values); 
   }
  return ddmt;
}

//1、2光口的索引为1，3、4光口的索引为2，依次类推
#ifdef SETTABLE_INFOSET
int setDdmThreshold(setInfoSet_t infoset)
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
#endif
