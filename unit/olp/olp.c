#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unit.h>
#include <visipc.h>
#include <olp.h>
#include <public.h>
boolean_t getOlpView(uchar_t chassis,uchar_t slot,uchar_t uclass,uchar_t utype,unitViewInfo_t *info)
{
  //printf("#%d %d\n",slot,value[31]);
  /*
  if (uclass==UC_OLP && utype==41)
  {
    char *value=getInfoSetValues(chassis,slot,OLP_STATUS_INFOSET_ID,1);
    if (NULL!=value)
    {
      printf("#%d %d\n",slot,value[31]);
      strcpy(info->unit_name,"OLP");
      info->ports.portCount=3;
      return TRUE; 
    }
    return FALSE;
  }
  */
  //printf("getOlpViewInfo #%d utype=%d\n",slot,utype);
  olp1P1Info_t *olp=getOlp1P1Info(chassis,slot);
  olp_property_t *property=getOlpProperty(chassis,slot);
  if (NULL==olp)
     return FALSE;
  if (property!=NULL)
  {
    if (utype<20 && property->port_type==1)
       utype=11;
  }
  //info->slotCount=1;
  //strcpy(info->height,"60%");
  memset(info,0,sizeof(unitViewInfo_t));
  /*
  if (utype<50)
  {
   if (1==utype||4==utype||11==utype) //11==utype:单纤双向
     strcpy(info->utype_text,"1+1");
   else
     strcpy(info->utype_text,"1:1");
  }
  */
  if (!(utype==20 || utype==21 || utype==13|| utype==31 || utype==32))
    strcpy(info->utype_text,"");
  else
  {
    if (property!=NULL)
    {
      if (property->struct_type==1)
       strcpy(info->utype_text,"1:1BM");
      else if (property->struct_type==2)
       strcpy(info->utype_text,"1:1BS");
      else
         strcpy(info->utype_text,"");
    }
  }
  if (!(utype==20 || utype==21 || utype==13|| utype==31 || utype==32))
    info->leds.leds_status[0][1]=olp->tx_led;
  else
    info->leds.leds_status[0][1]=olp->line;
  info->leds.leds_status[0][2]=olp->mode;
  info->leds.leds_status[1][1]=olp->rx1_led;
  info->leds.leds_status[1][2]=olp->rx2_led;
  memset(info->ports.ports_status,2,10);
  
  info->ports.portCount=6;
  info->leds.col_count=2;

  info->uclass=uclass;//UC_OLP;
  if (utype<50)
  {
    if (uclass==UC_OSS)
        strcpy(info->unit_name,"OSS");
    else
      strcpy(info->unit_name,"OLP");
    if (utype==20 || utype==21 ||utype==13|| utype==31 || utype==32 || utype==41)
        info->ports.portCount=3;
  }
  else
   strcpy(info->unit_name,"OBP");
  if (uclass!=UC_OSS)
 {
  if (3!=utype)
  {
    if (4!=utype)
    {
      if (11!=utype)
      {
        if (!(20==utype || utype==21  || 13==utype|| utype==31 || utype==32|| utype==41))
        {
          strcpy(info->ports.ports_text[0][0],"TX");
          strcpy(info->ports.ports_text[0][1],"RX");
          strcpy(info->ports.ports_text[0][2],"R1");
          strcpy(info->ports.ports_text[0][3],"T1");
          strcpy(info->ports.ports_text[0][4],"R2");
          strcpy(info->ports.ports_text[0][5],"T2");
        }
       else
       {
          strcpy(info->ports.ports_text[0][0],"TX");
          strcpy(info->ports.ports_text[0][1],"R1");
          strcpy(info->ports.ports_text[0][2],"R2");
          strcpy(info->ports.ports_text[1][0],"RX");
          strcpy(info->ports.ports_text[1][1],"T1");
          strcpy(info->ports.ports_text[1][2],"T2");
       }
      }
      else //单纤双向
      {
        strcpy(info->ports.ports_text[0][0],"RX");
        strcpy(info->ports.ports_text[0][1],"TX");
        strcpy(info->ports.ports_text[0][2],"Line1");
        strcpy(info->ports.ports_text[0][3],"Line2");
        strcpy(info->ports.ports_text[0][4],"N/A");
        strcpy(info->ports.ports_text[0][5],"N/A");
      }
    }
    else
    {
      strcpy(info->ports.ports_text[0][0],"TX");
      strcpy(info->ports.ports_text[1][0],"RX");
      strcpy(info->ports.ports_text[0][1],"R1");
      strcpy(info->ports.ports_text[1][1],"T1");
      strcpy(info->ports.ports_text[0][2],"R2");
      strcpy(info->ports.ports_text[1][2],"T2");
      strcpy(info->ports.ports_text[0][3],"N/A");
      strcpy(info->ports.ports_text[0][4],"N/A");
      strcpy(info->ports.ports_text[0][5],"N/A");
    }
    if (!(utype==20 || utype==21 || utype==13|| utype==31 || utype==32))
    {      
       strcpy(info->leds.leds_text[0][0],"TX");
    }
    else
    {
      strcpy(info->leds.run_text,"Set");
      strcpy(info->leds.leds_text[0][0],"Pri");
    }
    strcpy(info->leds.leds_text[1][0],"Auto");
    if (11!=utype)
    {
      strcpy(info->leds.leds_text[0][1],"R1");
      strcpy(info->leds.leds_text[1][1],"R2");
    }
    else
    {
      strcpy(info->leds.leds_text[0][1],"L1");
      strcpy(info->leds.leds_text[1][1],"L2");
    }
    if (!(utype==20 || utype==21 ||utype==13|| utype==31|| utype==32))
       info->leds.run_led=olp->line;
    else
        info->leds.run_led=3;
  }
  else
  {
    strcpy(info->ports.ports_text[0][0],"RX");
    strcpy(info->ports.ports_text[0][1],"R1");
    strcpy(info->ports.ports_text[0][2],"R2");
    strcpy(info->ports.ports_text[0][3],"SRX");
    strcpy(info->ports.ports_text[0][4],"O1RX");
    strcpy(info->ports.ports_text[0][5],"O2RX");
    
    strcpy(info->ports.ports_text[1][0],"TX");
    strcpy(info->ports.ports_text[1][1],"T1");
    strcpy(info->ports.ports_text[1][2],"T2");
    strcpy(info->ports.ports_text[1][3],"STX");
    strcpy(info->ports.ports_text[1][4],"O1TX");
    strcpy(info->ports.ports_text[1][5],"O2TX");
   
    strcpy(info->leds.run_text,"Set");
    strcpy(info->leds.leds_text[0][0],"Pri");
    strcpy(info->leds.leds_text[0][1],"R1");
    strcpy(info->leds.leds_text[1][0],"Auto");
    strcpy(info->leds.leds_text[1][1],"R2");

    info->leds.run_led=olp->tx_led;
    info->leds.leds_status[0][1]=olp->line;
   }
  }
  else
  {
    strcpy(info->leds.run_text,"RUN");
    info->leds.run_led=olp->tx_led;
    strcpy(info->leds.leds_text[0][0],"S1");
    strcpy(info->leds.leds_text[0][1],"CH1");
    strcpy(info->leds.leds_text[1][0],"S2");
    strcpy(info->leds.leds_text[1][1],"CH2");

     olp1P1Info_t *status;
     status=getOssChannelInfo(0,slot,1);
     info->leds.run_led=1;
     if (NULL!=status)
     {
     info->leds.leds_status[0][1]=status->rx1_led<2?1:0;
     info->leds.leds_status[0][2] =status->rx2_led<2?1:2;
     }
     status=getOssChannelInfo(0,slot,2);
     if (NULL!=status)
     {
       info->leds.leds_status[1][1]=status->rx1_led<2?1:0;
       info->leds.leds_status[1][2] =status->rx2_led<2?1:2;
     }
     strcpy(info->ports.ports_text[0][0],"R1_1");
     strcpy(info->ports.ports_text[0][1],"R1_2");
     strcpy(info->ports.ports_text[0][2],"CH1");
     strcpy(info->ports.ports_text[0][3],"R2_1");
     strcpy(info->ports.ports_text[0][4],"R2_2");
     strcpy(info->ports.ports_text[0][5],"CH2");
  }
  if (uclass==UC_OLP && utype==41)
  {
    char *value=getInfoSetValues(chassis,slot,OLP_STATUS_INFOSET_ID,1);
    if (NULL!=value)
    {
      char bits[16];
      bytetobits(value[31],bits);
      info->leds.row_count=3;
      info->leds.col_count=4;
      info->leds.leds_status[0][0]=1;
      info->leds.leds_status[0][1]=3;//SET
      strcpy(info->leds.leds_text[0][0],"");
      strcpy(info->leds.leds_text[0][1],"");
      strcpy(info->leds.leds_text[1][0],"");
      strcpy(info->leds.leds_text[1][1],"");

      if (value[29]!=2) //PRI LED
         info->leds.leds_status[0][2]=1;
      else
         info->leds.leds_status[0][2]=2;

      if (value[28]==1) //Auto LED
         info->leds.leds_status[0][3]=1;
      else if (value[28]==2) 
          info->leds.leds_status[0][3]=2;
      else
         info->leds.leds_status[0][3]=3;

      int n;
      info->leds.leds_status[1][0]=bits[7];//T1
      info->leds.leds_status[1][1]=bits[6];//T2
      info->leds.leds_status[1][2]=bits[5];//R1
      info->leds.leds_status[1][3]=bits[4];//R2
      for (n=0;n<4;n++)
      {
         if (info->leds.leds_status[1][n]!=1)
            info->leds.leds_status[1][n]=2;
      }
      info->leds.leds_status[2][0]=bits[3];//LINK
      info->leds.leds_status[2][1]=bits[2];//TX
      info->leds.leds_status[2][2]=bits[1];//RX
      if (info->leds.leds_status[2][0]!=1)
         info->leds.leds_status[2][0]=3;
      for (n=1;n<3;n++)
      {
         if (info->leds.leds_status[2][n]!=1)
            info->leds.leds_status[2][n]=2;
      }
      info->leds.leds_status[2][3]=10;

      //printf("#%d %d bits[0]=%d bits[7]=%d\n",slot,value[31],bits[0],bits[7]);
    }
    else
    //printf("getOlpViewInfo #%d utype=%d\n",slot,utype);
    return FALSE;
  }
  return TRUE;
}

boolean_t getObpView(uchar_t chassis,uchar_t slot,uchar_t uclass,uchar_t utype,unitViewInfo_t *info)
{
  obpInfo_t * obp=getObpInfo(chassis,slot);
  //olp1P1Info_t *olp=getOlp1P1Info(chassis,slot);
  if (NULL==obp)
     return FALSE;
  //info->slotCount=1;
  //strcpy(info->height,"60%");
  memset(info,0,sizeof(unitViewInfo_t));
  strcpy(info->unit_name,"OBP");
  /*
  if (obp->mode>1 && obp->mode<4)
    info->leds.leds_status[0][1]=2;
  else
     info->leds.leds_status[0][1]=1;
  */
  info->leds.leds_status[0][1]=obp->line;
  info->leds.leds_status[0][2]=obp->auto_led;
  info->leds.leds_status[1][1]=obp->rx1_led;
  info->leds.leds_status[1][2]=obp->rx2_led;
  //printf("line=%d rx1_led=%d rx2_led=%d auto_led=%d set_led=%d\r\n",obp->line,obp->rx1_led,obp->rx2_led,obp->auto_led,obp->set_led);
  memset(info->ports.ports_status,2,10);
  
  info->leds.col_count=2;
  strcpy(info->ports.ports_text[0][0],"R1");
    strcpy(info->ports.ports_text[0][1],"R2");
    strcpy(info->ports.ports_text[0][2],"R3");
    strcpy(info->ports.ports_text[0][3],"R4");
    strcpy(info->ports.ports_text[1][3],"T4");
    if (utype<52)
    {
    info->ports.portCount=6;
    strcpy(info->ports.ports_text[0][4],"M/R3");
    strcpy(info->ports.ports_text[0][5],"M/R4");
   
    strcpy(info->ports.ports_text[1][4],"N/A");
    strcpy(info->ports.ports_text[1][5],"N/A");
    }
    else
        info->ports.portCount=4;
    strcpy(info->ports.ports_text[1][0],"T1");
    strcpy(info->ports.ports_text[1][1],"T2");
    strcpy(info->ports.ports_text[1][2],"T3");

    
   
    strcpy(info->leds.run_text,"Set");
    strcpy(info->leds.leds_text[0][0],"BPS");
    strcpy(info->leds.leds_text[0][1],"R1");
    strcpy(info->leds.leds_text[1][0],"Auto");
    strcpy(info->leds.leds_text[1][1],"R2");

    info->leds.run_led=obp->set_led;
    //info->leds.leds_status[0][1]=obp->line;
    /*
  
  if (3!=utype)
  {
    if (4!=utype)
    {
      strcpy(info->ports.ports_text[0][0],"RX");
      strcpy(info->ports.ports_text[0][1],"TX");
      strcpy(info->ports.ports_text[0][2],"R1");
      strcpy(info->ports.ports_text[0][3],"T1");
      strcpy(info->ports.ports_text[0][4],"R2");
      strcpy(info->ports.ports_text[0][5],"T2");
    }
    else
    {
      strcpy(info->ports.ports_text[0][0],"RX");
      strcpy(info->ports.ports_text[1][0],"TX");
      strcpy(info->ports.ports_text[0][1],"R1");
      strcpy(info->ports.ports_text[1][1],"T1");
      strcpy(info->ports.ports_text[0][2],"R2");
      strcpy(info->ports.ports_text[1][2],"T2");
      strcpy(info->ports.ports_text[0][3],"N/A");
      strcpy(info->ports.ports_text[0][4],"N/A");
      strcpy(info->ports.ports_text[0][5],"N/A");
    }
    strcpy(info->leds.run_text,"PRI");
    strcpy(info->leds.leds_text[0][0],"TX");
    strcpy(info->leds.leds_text[0][1],"R1");
    strcpy(info->leds.leds_text[1][0],"Auto");
    strcpy(info->leds.leds_text[1][1],"R2");
    info->leds.run_led=olp->line;
  }
  else
  {
    strcpy(info->ports.ports_text[0][0],"RX");
    strcpy(info->ports.ports_text[0][1],"R1");
    strcpy(info->ports.ports_text[0][2],"R2");
    strcpy(info->ports.ports_text[0][3],"SRX");
    strcpy(info->ports.ports_text[0][4],"O1RX");
    strcpy(info->ports.ports_text[0][5],"O2RX");
    
    strcpy(info->ports.ports_text[1][0],"TX");
    strcpy(info->ports.ports_text[1][1],"T1");
    strcpy(info->ports.ports_text[1][2],"T2");
    strcpy(info->ports.ports_text[1][3],"STX");
    strcpy(info->ports.ports_text[1][4],"O1TX");
    strcpy(info->ports.ports_text[1][5],"O2TX");
   
    strcpy(info->leds.run_text,"Set");
    strcpy(info->leds.leds_text[0][0],"Pri");
    strcpy(info->leds.leds_text[0][1],"R1");
    strcpy(info->leds.leds_text[1][0],"Auto");
    strcpy(info->leds.leds_text[1][1],"R2");
    info->leds.run_led=olp->tx_led;
    info->leds.leds_status[0][1]=olp->line;
  }
  */
  return TRUE;
}
boolean_t getOlpViewInfo(uchar_t chassis,uchar_t slot,uchar_t uclass,uchar_t utype,unitViewInfo_t *info)
{
 if (utype<51)
  return getOlpView(chassis,slot,uclass,utype,info);
 else
  return getObpView(chassis,slot,uclass,utype,info);
}
/*
bool_t getOlp1P1Info(uchar_t chassis,uchar_t slot,olp1StatusInfo_t *info)
{
   char *values;
   chassis_t *cinfo=NULL;
   values=getInfoSetValues(chassis,0,CHASSIS_INFOSET_ID,0);
   if (NULL!=values)
   {
      cinfo=(chassis_t *)(values+5);
      //printf("getChassisInfo NOT null!\n");  
   }

  uchar_t n,m,uclass=0,utype=0;
  unit_base_info_t *bi;
  status_infoset_t *sis;
  //char *p,property[20]="",port_type[8]="";
  //char buf[30]="OTU111-25-10-80-1550";
  if (TRUE!=getUnitClassType(chassis,slot,&uclass,&utype) || uclass!=4)
      return FALSE;
  
  sis=getStatusInfoSetMap(chassis,slot,OLP_STATUS_INFOSET_ID,1);
  if (NULL==sis)
     return FALSE;
  //printf("uclass=%d\n",uclass);
  info->operating_mode=sis->item_values[1];
  info->line=sis->item_values[2];
  info->rule=sis->item_values[3];
  info->ret_mode=sis->item_values[4];
  return TRUE;
}

bool_t getOlp1P1Data(uchar_t chassis,uchar_t slot,olp1Data_t *info)
{
  uchar_t n,m,uclass=0,utype=0;
  unit_base_info_t *bi;
  status_infoset_t *sis;
  //char *p,property[20]="",port_type[8]="";
  //char buf[30]="OTU111-25-10-80-1550";
  if (TRUE!=getUnitClassType(chassis,slot,&uclass,&utype) || uclass!=4)
      return FALSE;
  
  sis=getStatusInfoSetMap(chassis,slot,OLP_DATA_INFOSET_ID,1);
  if (NULL==sis)
     return FALSE;
  //printf("uclass=%d\n",uclass);
  
  info->power.rx1_power=sis->item_values[1];
  info->power.rx1_power=info->power.rx1_power<<8;
  info->power.rx1_power|=sis->item_values[2];

  info->power.rx2_power=sis->item_values[3];
  info->power.rx2_power=info->power.rx2_power<<8;
  info->power.rx2_power|=sis->item_values[4];

  info->power.tx_power=sis->item_values[5];
  info->power.tx_power=info->power.tx_power<<8;
  info->power.tx_power|=sis->item_values[6];

  info->power.l1_power=sis->item_values[7];
  info->power.l1_power=info->power.l1_power<<8;
  info->power.l1_power|=sis->item_values[8];

  info->power.l2_power=sis->item_values[9];
  info->power.l2_power=info->power.l2_power<<8;
  info->power.l2_power|=sis->item_values[10];
  
  info->power.tx_alm_power=sis->item_values[11];
  info->power.tx_alm_power=info->power.tx_alm_power<<8;
  info->power.tx_alm_power|=sis->item_values[12];

  info->ret_time=sis->item_values[13];
  info->ret_time=info->ret_time<<8;
  info->ret_time|=sis->item_values[14];

  //printf("info->ret_time=%d\n",info->ret_time);
  return TRUE;
}
*/
olp1P1Info_t  *getOssChannelInfo(uchar_t chassis,uchar_t slot,uchar_t channel)
{
  char *values;
  short value;
  uchar_t uclass;
  olp1P1Info_t *olp=NULL;
  //printf("getOlp1P1Info #%d uc=%d\n",slot,getUnitClass(chassis,slot));
  uclass=getUnitClass(chassis,slot);
  if (UC_OLP==uclass ||UC_OSS==uclass)
  {
    values=getInfoSetValues(chassis,slot,OLP_STATUS_INFOSET_ID,channel);
    if (NULL!=values)
    {
      olp=(olp1P1Info_t*)(values);  
    }
    return  olp;
  }
  else
     return NULL;
}
olp1P1Info_t  *getOlp1P1Info(uchar_t chassis,uchar_t slot)
{
  return getOssChannelInfo(chassis,slot,1);
}

obpInfo_t *getObpInfo(uchar_t chassis,uchar_t slot)
{
  obpInfo_t *obp;
  obp=(obpInfo_t *)getInfoSetValues(chassis,slot,OLP_STATUS_INFOSET_ID,1);  
  return  obp;
}
#ifdef SETTABLE_INFOSET
int setOlpInfo(setInfoSet_t infoset)
{
  char n,m=3,flag;
  char message[512];
  short value;
  olp1P1Info_t  *olp;
  
  olp=getOlp1P1Info(0,infoset.slot);
  if (olp==NULL)
    return -1;
  memset(message,0,512*sizeof(char));
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
       case OLP1_MASTER:
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
      if (m>42)
        break;
   }
  }
   //printf("set olp l1_p=%d l2_p=%d tx=%d\n",olp->l1_power,olp->l2_power,olp->tx_alm_power);
  if (m>3)
   {
     message[0]='S';
     message[1]=infoset.infoset_id;
     message[2]=infoset.index;
     /*for (m=0;m<48;m++)
       printf("%0x ",message[m]);
     printf("\nsetOlpInfo OK\n");
     */
     sendMessageToUnit(infoset.chassis,infoset.slot,message,infoset.useconds);
   }
}
#endif
olp_property_t *getOlpProperty(uchar_t chassis,uchar_t slot)
{
  return (olp_property_t *)getUnitProperty(chassis,slot);
}

char checkOlpMessage(uchar_t slot,uchar_t uclass,uchar_t utype,uchar_t infoset_id,uchar_t index,char *message,char is_print,char is_save)
{
  char ret=0;
  //printf("slot=%d infoset_id=%d index=%d\n",slot,infoset_id,index);
  if (OLP_STATUS_INFOSET_ID==infoset_id && utype<50)
  {
    int n;
    ret=1;
    for (n=14;n<25;n++)
    {
      if (message[n]<1 || message[n]>3)
      {
         if (debug.is_debug_mode)
           printf("slot=%d infoset_id=%d D%d=%d is invalid\n",slot,infoset_id,n,message[n]);
         #ifdef ENABLE_LOG
         if (is_save)
         {
           char mess[20]="";
           sprintf(mess,"#%d id=%d idx=%d %d",slot,infoset_id,index,message[n]);
           insertSysLog(101,mess);
         }
         #endif
         return 2;
      }
    }
  }
  return ret;
}
