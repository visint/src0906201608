#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <public.h>
#include <heartbeat.h>
/*
void bytesToString(char bytes[],char[] buf)
{

}
*/
void setHeartBeatSlots(char conf[])
{
  char group_count=0;
  char n,m;
  char value[60]="";
  char item_name[16]="";
  char buf[16];
  char count;
  for (n=0;n<4;n++)
  {
    if (conf[n*9]!=0)
    {
      count=0;
      bytetobits(conf[n*9+1],buf);
      sprintf(value,"%d",conf[n*9]);
      for (m=0;m<8;m++)
      {
        if (buf[m]==1)
        {
           if (count>0)
             sprintf(value,"%s,%d",value,m+1);
           else
           {
             sprintf(value,"%s:%d",value,m+1);
             count++;
           }
        }
      }
      bytetobits(conf[n*9+2],buf);
      for (m=0;m<8;m++)
      {
        if (buf[m]==1)
        {
           if (count>0)
             sprintf(value,"%s,%d",value,m+9);
           else
           {
             sprintf(value,"%s:%d",value,m+9);
             count++;
           }
        }
      }
      if (count<1)
          strcpy(value,"");
      else
      {
         sprintf(value,"%s:%d.%d.%d.%d",value,conf[n*9+3],conf[n*9+4],conf[n*9+5],conf[n*9+6]);
      }
    }
    else
        strcpy(value,"");
    sprintf(item_name,"hb_group%d",n+1);
    //printf("%d:%d item_name:%s value=%s\n",n+1,conf[n*9],item_name,value);
    setConfValue(SYS_CONF_FILE,item_name,value," ");
  }
  /*
  if (group_count>0 && group_count<5)
  {
    //heart_beat_group_t *group=(heart_beat_group_t *)(conf);
    //printf("setHeartBeatSlots:group_count=%d\n",group_count);
    for (n=0;n<group_count;n++)
    {
      sprintf(item_name,"hb_group%d",n+1);
      sprintf(value,"%d",group->linkage);

      //printf("setHeartBeatSlots:slots[0]=%d slots[1]=%d\n",group->slots[0],group->slots[1]);
      bytetobits(group->hb_slots[0],buf);
      for (m=0;m<8;m++)
      {
       if (buf[m]==1)
        sprintf(value,"%s,%d",value,m+1);
      }
      bytetobits(group->hb_slots[1],buf);
      for (m=0;m<8;m++)
      {
       if (buf[m]==1)
        sprintf(value,"%s,%d",value,m+9);
      }
      //printf("setHeartBeatSlots:value=%s\n",value);
      if (!strchr(value,','))
      {
          value[0]='\0';
      }
      else
      {
      //printf("setHeartBeatSlots:value=%s\n",value);
      sprintf(value,"%s:%u.%u.%u.%u",value,group->source_ip[0],group->source_ip[1],group->source_ip[2],group->source_ip[3]);
      }
      //printf("setHeartBeatSlots:item_name=%s value=%s\n",item_name,value);
      setConfValue(SYS_CONF_FILE,item_name,value," ");
      group++;
   }
  }
  */
}
void setHeartBeatItem(char conf[])
{
  char value[60]="";
  char item_name[16]="";
  int n;
  switch (conf[0])
  {
   case 2:
    if (conf[1]==1)
      value[0]='Y';
    else
      value[0]='N';
    strcpy(item_name,"hb_enable");
   break;
  }
   if (value[0]!='\0')
      setConfValue(SYS_CONF_FILE,item_name,value," ");
}
