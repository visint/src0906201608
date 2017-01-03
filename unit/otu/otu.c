#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <public.h>
#include <unit.h>
#include <infoset.h>
#include <visipc.h>
#include <ddm.h>
#include <unit.h>
#include <otu.h>

boolean_t getOtuInfoItems(uchar_t chassis,uchar_t slot,uchar_t infoSetID,uchar_t index,uchar_t infoItemsId[],char itemsValue[])
{
  uchar_t infoItemsId1[36];
}
sfp_status_t *getSfpStatus(uchar_t chassis,uchar_t slot)
{
  char *pval,n;
  pval=getInfoSetValues(0,slot,OTU_STATUS_INFOSET_ID,1);
  if (NULL==pval)
       return NULL;
  return (sfp_status_t *)(pval+4);
  /*pval+=4;
  for (n=0;n<8;n++)
  {
    sfp_status[n].port_status=pval;
    sfp_status[n].module_exist=pval+1;
    sfp_status[n].tx_enable=pval+2;
    pval+=3;
  }
 return TRUE;
*/
}

sfp_info_t *getSfpInfo(uchar_t chassis,uchar_t slot,uchar_t index)
{
  /*char *pval,n;
  pval=getInfoSetValues(0,slot,OTU_SFP_PROPERTY_INFOSET_ID,index);
  if (NULL==pval)
       return FALSE;
  for (n=0;n<8;n++)
  {
    sfp_info[n].distance=pval;
    sfp_info[n].max_speed=pval+2;
    sfp_info[n].wave_length=pval+4;
    sfp_info[n].current_speed=pval+6;
    pval+=8;
  }
  return TRUE;
*/
 return(sfp_info_t*)getInfoSetValues(0,slot,OTU_SFP_PROPERTY_INFOSET_ID,index);
}

sfp_loopback_t *getSfpLoopback(uchar_t chassis,uchar_t slot)
{
  //sfp_lb_info_t *plb;
  return(sfp_loopback_t*)getInfoSetValues(0,slot,OTU_SFP_FEC_INFOSET_ID,1);
  //     return plb;
}
boolean_t getOtuViewInfo(uchar_t chassis,uchar_t slot,uchar_t uclass,uchar_t utype,unitViewInfo_t *info)
{
  otuUnitStatus_t *pOtuStatus;
  otuPortProperty_t *pPortProperty;
  
  uchar_t n,max_port=8;
  pOtuStatus=getOtuStatus(0,slot);
  
  if (NULL==pOtuStatus)
       return FALSE;
 //printf("getOtuViewInfo OTU_TYPE_10G_SFP_PLUS_A:%d utype=%d..\n",OTU_TYPE_10G_SFP_PLUS_A,utype);
  memset(info,0,sizeof(unitViewInfo_t));
  pPortProperty=(otuPortProperty_t*)getInfoSetValues(0,slot,OTU_SFP_PROPERTY_INFOSET_ID,1);
  if (1==utype || 5==utype)
  {   
     max_port=4;
     if (1!=pOtuStatus->work_mode)
     {
      if (5!=utype)
        info->leds.run_led=3;
      else
        info->leds.run_led=0;
     }
     else
      info->leds.run_led=1;
  }
  if (OTU_TYPE_10G_SFP_PLUS_A==utype || UC_OEO==uclass && 7==utype)
  {
     char *pOtup=getUnitProperty(0,slot);
     max_port=*(pOtup+9);
     if (max_port!=8)
        max_port=4;
    // printf("max_port=%d %d",*(pOtup+9),max_port);
  }
  for (n=0;n<max_port;n++)
  {
/*
      if (2!=pOtuStatus->portStatus[n].on_module)
      {
          if (2!=pOtuStatus->portStatus[n].tx_enable)
          {
            if (2!=pOtuStatus->portStatus[n].port_status)
              info->ports.ports_status[n]=1;//pOtuStatus->portStatus[n].port_status;
            else
            {
              if (3!=pOtuStatus->portStatus[n].tx_enable)
                 info->ports.ports_status[n]=2;
              else
                  info->ports.ports_status[n]=3;
            }
          }
          else
          {
            info->ports.ports_status[n]=3;
          }
        //printf("port #%d enable=%d\n",n+1,pOtuStatus->portStatus[n].tx_enable);
      }
      else
          info->ports.ports_status[n]=0;
*/

      if (2!=pOtuStatus->portStatus[n].on_module)
      {
          if (2!=pOtuStatus->portStatus[n].port_status)
              info->ports.ports_status[n]=1;
          else
          {
            if (1!=pOtuStatus->portStatus[n].tx_enable)
            {
              info->ports.ports_status[n]=3;
            }
            else
            {
              info->ports.ports_status[n]=2;
            }
         }
        //printf("port #%d enable=%d\n",n+1,pOtuStatus->portStatus[n].tx_enable);
      }
      else
          info->ports.ports_status[n]=0;

      if (n==4)
         pPortProperty=(otuPortProperty_t*)getInfoSetValues(0,slot,OTU_SFP_PROPERTY_INFOSET_ID,2);
      if (pPortProperty!=NULL && info->ports.ports_status[n]>0 && info->ports.ports_status[n]<3)
      { 
         //strcpy(info->ports.ports_info_name[n][0],"波长");
         snprintf(info->ports.ports_info_val[n][0],15,"%.2fnm",0.05*pPortProperty->wave_lenght);
         //strcpy(info->ports.ports_info_name[n][1],"传输距离");
         snprintf(info->ports.ports_info_val[n][1],15,"%.2fKM",0.01*pPortProperty->distance);
         //strcpy(info->ports.ports_info_name[n][2],"当前速率");
         snprintf(info->ports.ports_info_val[n][2],15,"%.2fG",0.01*pPortProperty->distance);

         //snprintf(info->ports.ports_tip[n],62,"波长:%.2fnm&#10;传输距离%.2fKM&#10;当前速率%.2fG",0.05*pPortProperty->wave_lenght,0.01*pPortProperty->distance,0.01*pPortProperty->current_speed);
      }
  }
 
  strcpy(info->leds.leds_text[0][0],"L1");
  strcpy(info->leds.leds_text[0][1],"C1");
  if (OTU_TYPE_2Dot5G_SFP_A==utype||3==utype)
  {
     //strcpy(info->utype_text,"2.5G");
     strcpy(info->leds.leds_text[1][0],"L4");
     strcpy(info->leds.leds_text[1][1],"C4");
     //strcpy(info->leds.run_text,"RUN");
     //info->leds.run_led=pOtuStatus->run_status;
     
     info->leds.run_led=0;
     info->leds.col_count=4;

     for (n=0;n<8;n++)
     {
       if (info->ports.ports_status[n]!=1)
          info->leds.leds_status[n%2][n/2]=3;
       else
          info->leds.leds_status[n%2][n/2]=1;
     }
  }
  else 
  {
     char led_start;
     strcpy(info->utype_text,"10G");
     strcpy(info->leds.leds_text[1][0],"L2");
     strcpy(info->leds.leds_text[1][1],"C2");
     if (1==utype)
      strcpy(info->leds.run_text,"OPM");
     //info->leds.run_led=pOtuStatus->work_mode;
     if (5==utype ||7==utype ||15==utype)
     {
       info->leds.col_count=4;
     }
     else
     {
       info->leds.col_count=2;
     }
     if (utype<10)
     {
      if (max_port<5)
        led_start=1;
      else
        led_start=0;
      for (n=0;n<max_port;n++)
      {
       if (n%2)
         info->leds.leds_status[1][led_start+n/2]=info->ports.ports_status[n];
       else
         info->leds.leds_status[0][led_start+n/2]=info->ports.ports_status[n];
      }
     }
    else
    {
     char bits[8]={0};
     //uchar_t tx_enable[8];
     otu1XInfo_t *otu1x=(otu1XInfo_t*)pOtuStatus;
     bytetobits(otu1x->exist&0xff,bits);
     //tx_enable[0]=otu1x->tx_status & 0x03;
     //printf("exist=%0x led_status=%0x\n",otu1x->exist,otu1x->led_status);
     info->leds.led_type=2;
     info->ports.ports_status[0]=info->leds.leds_status[0][0]=otu1x->led_status & 0x03;
     info->ports.ports_status[2]=info->leds.leds_status[0][1]=(otu1x->led_status>>4) & 0x03;
     info->ports.ports_status[4]=info->leds.leds_status[0][2]=(otu1x->led_status>>8) & 0x03;
     info->ports.ports_status[6]=info->leds.leds_status[0][3]=(otu1x->led_status>>12) & 0x03;
     
     info->ports.ports_status[1]=info->leds.leds_status[1][0]=(otu1x->led_status>>2) & 0x03;
     info->ports.ports_status[3]=info->leds.leds_status[1][1]=(otu1x->led_status>>6) & 0x03;
     info->ports.ports_status[5]=info->leds.leds_status[1][2]=(otu1x->led_status>>10) & 0x03;
     info->ports.ports_status[7]=info->leds.leds_status[1][3]=(otu1x->led_status>>14) & 0x03;

     for (n=0;n<8;n++)
     {
        if (info->ports.ports_status[n]!=1)
        {
           if (bits[7-n]!=1)
              info->ports.ports_status[n]=0;
           else if (((otu1x->tx_status>>n*2) & 0x03)!=1)
              info->ports.ports_status[n]=3;
           else
              info->ports.ports_status[n]=2;
        }
        //led[n]=(otu1x->led_status>>n*2) & 0x03;
        //tx_enable[n]=((otu1x->tx_status>>n*2) & 0x03);
        //if (bits[8-n]==1)
        //printf("tx enable[#%d]=%d ",n+1,tx_enable[n]);
     }
     /*
     for (n=7;n>=0;n--)
     {
       if (bits[n]==1)
       {
         if (led[7-n]==1)
           port[7-n]=1;
         else
         {
           if (tx_enable[7-n]!=1)
              port[7-n]=3;
           else
               port[7-n]=2;
         }
       }
       else
         port[7-n]=0;
     }
     */
    }
  }
  //printf("getOtuViewInfo on slot:%d\n",slot);
  for (n=0;n<4;n++)
  {
    if (info->leds.leds_status[0][n]!=1)
        info->leds.leds_status[0][n]=3;
    if (info->leds.leds_status[1][n]!=1)
        info->leds.leds_status[1][n]=3;
    //printf("led[1][%d]=%d ",n,info->leds.leds_status[1][n]);
  }
  info->ports.portCount=max_port;
  if (UC_OEO!=uclass)
  {
     if (3!=utype)
        strcpy(info->unit_name,"OTU");
     else
        strcpy(info->unit_name,"EOTU");
     strcpy(info->leds.leds_text[0][0],"L1");
     strcpy(info->leds.leds_text[0][1],"C1");
     if (4!=max_port)
     {
        strcpy(info->leds.leds_text[1][0],"L4");
        strcpy(info->leds.leds_text[1][1],"C4");
     }
     else
     {
        strcpy(info->leds.leds_text[1][0],"L2");
        strcpy(info->leds.leds_text[1][1],"C2");
     }
     for (n=0;n<max_port;n++)
     {
      if (n%2!=1)
         sprintf(info->ports.ports_htext[n],"L%d",n/2+1);
      else
         sprintf(info->ports.ports_htext[n],"C%d",n/2+1);
    
      strcpy(info->ports.ports_text[0][n],"RX");
      strcpy(info->ports.ports_text[1][n],"TX");
     }
 }
 else
 {
    strcpy(info->unit_name,"OEO");
    strcpy(info->leds.leds_text[0][0],"C1");
    strcpy(info->leds.leds_text[0][1],"C2");
    if (OTU_TYPE_10G_XFP_A!=utype)
    {
        strcpy(info->leds.leds_text[1][0],"C7");
        strcpy(info->leds.leds_text[1][1],"C8");
    }
    else
    {
        strcpy(info->leds.leds_text[1][0],"C3");
        strcpy(info->leds.leds_text[1][1],"C4");
    }
    for (n=0;n<max_port;n++)
    {
      sprintf(info->ports.ports_htext[n],"C%d",n+1);
      strcpy(info->ports.ports_text[0][n],"RX");
      strcpy(info->ports.ports_text[1][n],"TX");
    }
 }
  #ifdef DEFAULT_LANG_CN
      strcpy(info->ports.ports_info_name[0],"波长");
      strcpy(info->ports.ports_info_name[1],"传输距离");
      strcpy(info->ports.ports_info_name[2],"当前速率");
  #else
     strcpy(info->ports.ports_info_name[0],"Wave length");
     strcpy(info->ports.ports_info_name[1],"Distance");
     strcpy(info->ports.ports_info_name[2],"Current speed");
  #endif
  strcpy(info->ports.ports_info_unit[0],"dBm");
  strcpy(info->ports.ports_info_unit[1],"km");
  strcpy(info->ports.ports_info_unit[2],"G");
 //printf("info->ports.ports_text[1][0]:%s\n",info->ports.ports_text[1][0]);
 return TRUE;
}

/*
otuInfo_t * getOtuInfo(uchar_t chassis,uchar_t slot,uchar_t channel)
{
  uchar_t n,m,uclass=0,utype=0;
  unit_base_info_t *bi;
  status_infoset_t *sis;
  char *p,property[20]="",port_type[8]="";

  char *values;
  otuInfo_t *otu=NULL;

  //char buf[30]="OTU111-25-10-80-1550";
  if (TRUE!=getUnitClassType(chassis,slot,&uclass,&utype) || uclass!=3)
      return FALSE;
  //printf("slot=%d uclass=%d\n",slot,uclass);
  bi=getBaseInfoMap(chassis,slot);
  if (bi==NULL)
     return FALSE;
  info->channel_count=1;
  //p=bi->property;
  p=strchr(bi->property,'/');
  while (p!=NULL)
   {
     info->channel_count++;
     p++;
     p=strchr(p,'/');
   }
   
  //info->channel_count=bi->property[3]-48;
  sscanf(bi->property,"%[0-9,A-Z]-%[0-9,A-Z]-%[0-9,A-Z]-%[0-9,A-Z]-%[0-9,A-Z]",property,port_type,info->channel[0].max_speed,info->channel[0].distance,info->channel[0].wave_length);
  info->channel[0].port_count=bi->property[7]-48;
   if (info->channel[0].port_count>2)
        info->channel[0].port_count=2;
  for (n=0;n<info->channel[0].port_count;n++)
  {
   values=getInfoSetValues(chassis,0,CHASSIS_INFOSET_ID,0);
   if (NULL!=values)
   {
      cinfo=(chassis_t *)(values+5);
      //printf("getChassisInfo NOT null!\n");  
   }
     //sis=getStatusInfoSetMap(chassis,slot,OPORT_INFOSET_ID,n+1);
     sis=getInfoSet(chassis, slot,OTU_STATUS_INFOSET_ID,n+1)
     if (NULL!=sis)
     {
       info->channel[0].rx_status[n]=sis->item_values[0];
       info->channel[0].tx_status[n]=sis->item_values[1];
     }
  }
  return TRUE;
}
*/
otuInfo_t *getOtuInfo(uchar_t chassis,uchar_t slot,uchar_t channel)
{
  char *values;
  otuInfo_t *otu=NULL;
  values=getInfoSetValues(chassis,0,OTU_STATUS_INFOSET_ID,channel);
  if (NULL!=values)
  {
      otu=(otuInfo_t*)(values);
      //printf("getChassisInfo NOT null!\n");  
  }
  return  otu;
}


otuProperty_t *getOtuProperty(uchar_t chassis,uchar_t slot,uchar_t index)
{
  char *values;
  otuProperty_t *otu=NULL;
  if (3!=getUnitClass(chassis,slot))
     return NULL;
  values=getInfoSetValues(chassis,slot,UNIT_PROPERTY_INFOSET_ID,index);
  if (NULL!=values)
  {
      otu=(otuProperty_t*)(values+5);  
  }
  return  otu;
}

otuChannelStatus_t *getOtuChannelStatus(uchar_t chassis,uchar_t slot,uchar_t group)
{
  char *values;
  otuChannelStatus_t *otu=NULL;
  if (3!=getUnitClass(chassis,slot))
     return NULL;
  values=getInfoSetValues(chassis,slot,OTU_STATUS_INFOSET_ID,group);
  if (NULL!=values)
  {
      otu=(otuChannelStatus_t*)(values);  
  }
  else
       printf("getOtuChannelStatus is null\n");
  return  otu;
}

/*
int insertOtuInfoSets1(uchar_t chassis,uchar_t slot,otuProperty_t otu,uchar_t utype)
{
  uchar_t otu_infoset_count=10;
  uchar_t index=0;
  ddmThreshold_t *ddmt,*pddmt;
  int n,m,infoset_count=0;

  if (2==utype)
    otu_infoset_count=13;
  status_infoset_t infosets[otu_infoset_count];
  for (n=0;n<otu_infoset_count;n++)
    memset(&infosets[n],0,sizeof(status_infoset_t));
  
}
*/
int insertOtuInfoSets(uchar_t chassis,uchar_t slot,otuProperty_t property,uchar_t utype)
{
  //#define OTU_INFOSET_COUNT 10
  uchar_t otu_infoset_count=13;
 
  uchar_t index=0;

  //if (2==utype)
  //  otu_infoset_count=13;
  status_infoset_t infosets[otu_infoset_count];
  ddmThreshold_t *ddmt,*pddmt;

  int n,m,infoset_count=0;
  //deleteSlotNodes(chassis,0);

  //printf("run in insertOtuInfoSets slot=%d\n",slot);
  for (n=0;n<otu_infoset_count;n++)
    memset(&infosets[n],0,sizeof(status_infoset_t));
  //addInfoSet(infosets,otu_infoset_count,UNIT_PROPERTY_INFOSET_ID,1,1,22,22);
  addInfoSet(infosets,otu_infoset_count,DDM_INFOSET_ID,1,2,20,10);
  addInfoSet(infosets,otu_infoset_count,DDM_INFOSET_ID,2,2,20,10);
  addInfoSet(infosets,otu_infoset_count,DDM_THRESHOLD_ID,1,1,20,20);
  addInfoSet(infosets,otu_infoset_count,DDM_THRESHOLD_ID,2,1,20,20);
  addInfoSet(infosets,otu_infoset_count,OTU_SFP_PROPERTY_INFOSET_ID,1,2,32,16);
  addInfoSet(infosets,otu_infoset_count,OTU_SFP_PROPERTY_INFOSET_ID,2,2,32,16);
  addInfoSet(infosets,otu_infoset_count,OTU_STATUS_INFOSET_ID,1,1,36,36);

  infoset_count=7;
  if (1!=utype)
  {
    addInfoSet(infosets,otu_infoset_count,DDM_INFOSET_ID,3,2,20,10);
    addInfoSet(infosets,otu_infoset_count,DDM_INFOSET_ID,4,2,20,10);
    addInfoSet(infosets,otu_infoset_count,DDM_THRESHOLD_ID,3,1,20,20);
    addInfoSet(infosets,otu_infoset_count,DDM_THRESHOLD_ID,4,1,20,20);
    //addInfoSet(infosets,otu_infoset_count,OTU_SFP_PROPERTY_INFOSET_ID,3,2,32,8);
    //addInfoSet(infosets,otu_infoset_count,OTU_SFP_PROPERTY_INFOSET_ID,4,2,32,8);
    //addInfoSet(infosets,otu_infoset_count,OTU_STATUS_INFOSET_ID,2,1,36,36);
    infoset_count+=4;
  }

  if (1==utype || 5==utype || 6==utype ||utype==7||utype>14)
  {
   addInfoSet(infosets,otu_infoset_count,OTU_SFP_FEC_INFOSET_ID,1,1,32,32);
   infoset_count+=1;
   //infosets[8].infoset_id=OTU_SFP_FEC_INFOSET_ID;
   //infosets[9].infoset_id=SFP_ERROR_CODE_TEST_ID;
  }
  //infoset_count=insertInfoSet(chassis,slot,infosets,otu_infoset_count);
  infoset_count=insertInfoSet(chassis,slot,infosets,infoset_count);
  //printf("InsertOtuInfoSets infoset_count=%d infosets[0].infoset_id=%d\n",infoset_count,infosets[0].infoset_id);
  return infoset_count;
 /*
  infosets[0].infoset_id=UNIT_PROPERTY_INFOSET_ID;
  infosets[1].infoset_id=DDM_INFOSET_ID;
  infosets[2].infoset_id=DDM_INFOSET_ID;
  infosets[3].infoset_id=DDM_THRESHOLD_ID;
  infosets[4].infoset_id=DDM_THRESHOLD_ID;
  infosets[5].infoset_id=OTU_SFP_PROPERTY_INFOSET_ID;
  infosets[6].infoset_id=OTU_SFP_PROPERTY_INFOSET_ID;
  infosets[7].infoset_id=OTU_STATUS_INFOSET_ID;
  
  if (2==utype)
  {
    infosets[8].infoset_id=OTU_STATUS_INFOSET_ID;
    infosets[9].infoset_id=DDM_INFOSET_ID;
    infosets[10].infoset_id=DDM_INFOSET_ID;
    infosets[11].infoset_id=DDM_THRESHOLD_ID;
    infosets[12].infoset_id=DDM_THRESHOLD_ID;
  }
  else if (1==utype || 5==utype || 6==utype ||utype==7)
  {
   infosets[8].infoset_id=OTU_SFP_FEC_INFOSET_ID;
   infosets[9].infoset_id=SFP_ERROR_CODE_TEST_ID;
  }
  infosets[0].index=1;     
  infosets[0].infoset_type=1;
  infosets[0].infoset_size=21;
  infosets[0].item_count=21;

  infosets[1].index=1;
  infosets[1].infoset_type=1;
  infosets[1].infoset_size=20;
  infosets[1].item_count=10;
  
  infosets[2].index=2;     
  infosets[2].infoset_type=1;
  infosets[2].infoset_size=20;
  infosets[2].item_count=10;

  infosets[3].index=1;     
  infosets[3].infoset_type=1;
  infosets[3].infoset_size=20;
  infosets[3].item_count=10;
  
  infosets[4].index=2;     
  infosets[4].infoset_type=1;
  infosets[4].infoset_size=20;
  infosets[4].item_count=10;
   
  infosets[5].index=1;     
  infosets[5].infoset_type=2;
  infosets[5].infoset_size=32;
  infosets[5].item_count=8;

  infosets[6].index=2;     
  infosets[6].infoset_type=2;
  infosets[6].infoset_size=32;
  infosets[6].item_count=8;

  infosets[7].index=1;     
  infosets[7].infoset_type=1;
  if (5==utype ||6==utype)
  {
  infosets[7].infoset_size=36;
  infosets[7].item_count=36;
  }
  else
  {
  infosets[7].infoset_size=28;
  infosets[7].item_count=28;
  }
  if (1==utype || 5==utype || 6==utype || 7==utype)
  {
  infosets[8].index=1;
  infosets[8].infoset_type=1;
  infosets[8].infoset_size=32;
  infosets[8].item_count=8;

  infosets[9].index=1;
  infosets[9].infoset_type=2;
  infosets[9].infoset_size=32;
  infosets[9].item_count=8;
  }
  else if (2==utype ||utype==7)
  {
    infosets[8].index=2;     
    infosets[8].infoset_type=1;
    infosets[8].infoset_size=28;
    infosets[8].item_count=28;

    infosets[9].index=3;
    infosets[9].infoset_type=2;
    infosets[9].infoset_size=20;
    infosets[9].item_count=10;
  
    infosets[10].index=4;     
    infosets[10].infoset_type=2;
    infosets[10].infoset_size=20;
    infosets[10].item_count=10;

    infosets[11].index=3;     
    infosets[11].infoset_type=1;
    infosets[11].infoset_size=20;
    infosets[11].item_count=10;
  
    infosets[12].index=4;     
    infosets[12].infoset_type=1;
    infosets[12].infoset_size=20;
    infosets[12].item_count=10;
  }
   if (insertInfoSet(chassis,slot,infosets,otu_infoset_count)!=-1)
   {
    //printf("insertOtuInfoSets OK\n");
    return otu_infoset_count;
   }
   else
   {
     //printf("insertOtuInfoSets fail\n");
     return -1;
   }
   return otu_infoset_count;
 
  for (n=0;n<MAX_ITEM_SIZE;n++)
  {
     for (m=0;m<otu_infoset_count;m++)
       infosets[m].item_values[n]=0;
  }
  
  ddmt.rxPowerMax=1;   //< 输入最大光功率单位:0.1dBm
  ddmt.rxPowerMin=-500;  !< 输入最小光功率单位:0.1dBm
  ddmt.txPowerMax=1;  < 输出光功率单位:0.1dBm
  ddmt.txPowerMin=-500;  < 输出最小光功率单位:0.1dBm
  ddmt.tempMax=800;     < SFP  最大温度单位:0.1°C
  ddmt.tempMin=-20;     < SFP  最小温度单位:0.1°C
  ddmt.voltMax=8000;    < SFP  最大电压单位:毫伏
  ddmt.voltMin=3000;     < SFP  最小电压单位:毫伏
  ddmt.rxBiasMax=30000;   < SFP  最大偏置电流单位:毫安
  ddmt.rxBiasMin=5000;   < SFP  最小偏置电流单位:毫安
  memcpy(&infosets[2].item_values,&ddmt,sizeof(ddmThreshold_t));
  memcpy(&infosets[3].item_values,&ddmt,sizeof(ddmThreshold_t));


  if (insertInfoSet(chassis,slot,infosets,otu_infoset_count)!=-1)
  {
    //groupDdmThreshold_t *gddmt=getGroupDdmThreshold(0,slot,1);
    //printf("insertOtuInfoSets #%d OK\n",slot);
    otuProperty_t *otu_p;
    otu_p=getOtuProperty(0,slot,1);
    if (NULL!=otu_p)
    {
       //memcpy(otu_p,&otu,sizeof(otuProperty_t));
       //memcpy(&otu_p->channel_property[0],&otu.channel_property[0],sizeof(channel_t));
       otu_p->unit_property.unit_use_class=otu.unit_property.unit_use_class;
       otu_p->unit_property.unit_use_type=otu.unit_property.unit_use_type;
       otu_p->unit_property.firmware_type=otu.unit_property.firmware_type;
       otu_p->unit_property.hardware_type=otu.unit_property.hardware_type;
       otu_p->unit_property.struct_type=otu.unit_property.struct_type;
 
       otu_p->channel_count=otu.channel_count;
       otu_p->channel_port_count=otu.channel_port_count;
       otu_p->port_type=otu.port_type;
       otu_p->channel_property[0].distance=otu.channel_property[0].distance;
       otu_p->channel_property[0].max_speed=otu.channel_property[0].max_speed;
       strncpy(otu_p->channel_property[0].wave_length,otu.channel_property[0].wave_length,6);
       //printf("#%d distance=%d speed=%d\n",slot,otu.channel_property[0].distance,otu.channel_property[0].max_speed);
    }
    //  memcpy(otu_p,&otu,sizeof(otuProperty_t));
    pddmt=getPortDdmThreshold(0, slot,1);
    if (pddmt!=NULL)
      {
      for (n=1;n<3;n++)
       {
       pddmt=getPortDdmThreshold(0, slot,n);
       if (pddmt!=NULL)
         {
         pddmt->rxPowerMax=1;  //!< 输入最大光功率单位:dBm
         pddmt->rxPowerMin=-50;  //!< 输入最小光功率单位:dBm
         pddmt->txPowerMax=1;  //!< 输出光功率单位:dBm
         pddmt->txPowerMin=-50;  //!< 输出最小光功率单位:dBm
         pddmt->voltMax=8;     //!< SFP  最大电压单位:伏
         pddmt->voltMin=3;     //!< SFP  最小电压单位:伏
         pddmt->rxBiasMax=30;   //!< SFP  最大偏置电流单位:毫安
         pddmt->rxBiasMin=5;   //!< SFP  最小偏置电流单位:毫安
         pddmt->tempMax=80;     //!< SFP  最大温度单位:°C
         pddmt->tempMin=-20;     //!< SFP  最小温度单位:°C
         pddmt++;
         }
       }
     }
   return otu_infoset_count;
  }
  else
    return -1;
 */
}

otuPortProperty_t *getOtuPortProperty(int chassis,int slot,int port)
{
 uchar_t index=1;
otuPortProperty_t *pPortProperty=NULL;
if (port<1 || port>8)
    return NULL;
if (port>4)
 index=2;
pPortProperty=(otuPortProperty_t*)getInfoSetValues(0,slot,OTU_SFP_PROPERTY_INFOSET_ID,index);
if (NULL!=pPortProperty)
{
  pPortProperty=pPortProperty+(port-1)%4;
}
 return pPortProperty;
}
//OTU单元盘(报文)信息集OTU_STATUS_INFOSET_ID
otuUnitStatus_t *getOtuStatus(int chassis,int slot)
{
  return (otuUnitStatus_t *)getInfoSetValues(0,slot,OTU_STATUS_INFOSET_ID,1);
}

otuPortFEC_t *getOtuFec(int chassis,int slot,int port)
{
  otuPortFEC_t *fec=NULL;
  if (port<1 || port>8)
    return NULL;
  fec=(otuPortFEC_t*)getInfoSetValues(0,slot,OTU_SFP_FEC_INFOSET_ID,1);
  if (NULL!=fec)
  {
     fec=fec + (port-1);
  }
  return fec;
}
#ifdef SETTABLE_INFOSET
int setOtuWorMode(int chassis,int slot,uchar_t mode)
{
  char message[512]={0};
  if (slot<1 && slot>15 || mode<1 && mode>2)
     return -1;
   message[0]='S';
   message[1]=OTU_STATUS_INFOSET_ID;
   message[2]=1;
   message[3]=3;
   message[4]=0;
   message[5]=mode;
   message[6]=0;
   return sendMessageToUnit(chassis,slot,message,300);
}

int setOtuPortSpeed(int chassis,int slot,int port,unsigned short speed)
{
  char message[512]={0};
  if (slot<1 || slot>15 || port<1 || port>8)
     return -1;
   message[0]='S';
   message[1]=OTU_SFP_PROPERTY_INFOSET_ID;
   if (port<5)
     message[2]=1;
   else
     message[2]=2;
   message[3]=((port-1)%4)*4+4;
   message[4]=(speed & 0xff00)>>8;
   message[5]=speed & 0xff;
   message[6]=0;
   return sendMessageToUnit(chassis,slot,message,300);
}
int setOtuPortTx(int chassis,int slot,int port,uchar_t enale)
{
  char message[512]={0};
  if (slot<1 && slot>15 || port<1 && port>8 || enale<1 && enale>3)
     return -1;
   message[0]='S';
   message[1]=OTU_STATUS_INFOSET_ID;
   message[2]=1;
   message[3]=4+port*3;
   message[4]=0;
   message[5]=enale;
   message[6]=0;
   return sendMessageToUnit(chassis,slot,message,300);
}


int setOtuLoopBackAndFec(int chassis,int slot,int port,uchar_t loopback,uchar_t fec)
{
  char message[512]={0},n=3;
  if (slot<1 && slot>15 || port<1 && port>8)
     return -1;
   message[0]='S';
   message[1]=OTU_SFP_FEC_INFOSET_ID;
   message[2]=1;
   if (loopback>0 && loopback<3)
   {
      message[n++]=4*port-3;
      message[n++]=0;
      message[n++]=loopback;
   }
   if (fec>0 && fec<3)
   {
      message[n++]=4*port-2;
      message[n++]=0;
      message[n++]=fec;
   }
   message[n]=0;
   return sendMessageToUnit(chassis,slot,message,300);
}
#endif
/*
int setOtuPortInfo(setInfoSet_t infoset)
{
  char n,m=3,flag;
  char message[512];
  short value;
  olp1P1Info_t  *olp;
  
  olp=getOlp1P1Info(0,infoset.slot);
  if (olp==NULL)
    return -1;
  memset(message,0,MAX_MESSAGE_SIZE*sizeof(char));
  for (n=0;n<MAX_SETTING_ITEM_COUNT;n++)
  { 
    flag=1;
    if (infoset.infoItems[n].item_id<1)
      break;
    switch (infoset.infoItems[n].item_id)
    {
       case OLP1_MODE:
         //olp->mode=infoset.infoItems[n].item_value & 0x3;
         value=infoset.infoItems[n].item_value & 0x3;
         break;
       case OLP1_LINE :
         //olp->line=infoset.infoItems[n].item_value & 0x3;
         value=infoset.infoItems[n].item_value & 0x3;
         break;
       case OLP1_RULE:
         //olp->rule=infoset.infoItems[n].item_value & 0x3;
         value=infoset.infoItems[n].item_value & 0x3;
         break;
       case OLP1_RET_MODE:
         //olp->ret_mode=infoset.infoItems[n].item_value & 0x3;
         value=infoset.infoItems[n].item_value & 0x3;
         break;
       case OLP1_RET_TIME:
         if (infoset.infoItems[n].item_value<OLP_MIN_RET_TIME)
             value=OLP_MIN_RET_TIME;
         else if (infoset.infoItems[n].item_value>OLP_MAX_RET_TIME)
             value=OLP_MAX_RET_TIME;
         else
             value=infoset.infoItems[n].item_value;
         //value=olp->ret_time;
         break;
       case OLP1_L1_SW_POWER:
         if (infoset.infoItems[n].item_value<OLP_MIN_L1_SW_POWER)
             value=OLP_MIN_L1_SW_POWER;
         else if (infoset.infoItems[n].item_value>OLP_MAX_L1_SW_POWER)
             value=OLP_MAX_L1_SW_POWER;
         else
             value=infoset.infoItems[n].item_value;
             //value=olp->l1_power;
             //printf("n=%d L1_SW_POWER=%d l1_power=%d value=%d\n",n,infoset.infoItems[n].item_value,olp->l1_power,value);
         break;
       case OLP1_L2_SW_POWER :
         if (infoset.infoItems[n].item_value<OLP_MIN_L2_SW_POWER)
             value=OLP_MIN_L2_SW_POWER;
         else if (infoset.infoItems[n].item_value>OLP_MAX_L2_SW_POWER)
             value=OLP_MAX_L2_SW_POWER;
         else
             value=infoset.infoItems[n].item_value;
             //value=olp->l2_power;
         break;
      case OLP1_TX_ALM_POWER:
         if (infoset.infoItems[n].item_value<OLP_MIN_TX_ALM_POWER)
             value=OLP_MIN_TX_ALM_POWER;
         else if (infoset.infoItems[n].item_value>OLP_MAX_TX_ALM_POWER)
             value=OLP_MAX_TX_ALM_POWER;
         else
             value=infoset.infoItems[n].item_value;
             //olp->tx_alm_power=value;
         break;
       case OLP1_R1_WAVE_LEN:
       case OLP1_R2_WAVE_LEN:
       case OLP1_TX_WAVE_LEN:
         //olp->ret_mode=infoset.infoItems[n].item_value & 0x3;
         value=infoset.infoItems[n].item_value & 0x3;
         break;
       default:
             flag=0;
    }
   if (flag)
   {
      message[m]=infoset.infoItems[n].item_id;
      //memcpy(&message[m+1],&value,sizeof(short));
      message[m+1]=(value & 0XFF00)>>8;
      message[m+2]= (value & 0XFF);
      m+=3;
      if (m>29)
        break;
   }
  }
   //printf("set olp l1_p=%d l2_p=%d tx=%d\n",olp->l1_power,olp->l2_power,olp->tx_alm_power);
  if (m>3)
   {
     message[0]='S';
     message[1]=infoset.infoset_id;
     message[2]=infoset.index;
     sendMessageToUnit(infoset.chassis,infoset.slot,message,infoset.useconds);
   }
}
*/
