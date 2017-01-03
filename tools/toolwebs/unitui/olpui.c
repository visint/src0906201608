#include <sys/sysinfo.h>
#include <time.h>
#include "../wsIntrn.h"
#include "../../include/unit.h"
#include <visipc.h>
#include <sqlite3.h>
#include <dbutil.h>
#include <log.h>
#include <olp.h>
#include "../unitwebs.h"
#include <public.h>
boolean_t getOlpWebInfo(uchar_t chassis,uchar_t cclass,uchar_t slot,uchar_t uclass,uchar_t utype,web_unit_t *punit)
{
  punit->uclass=cclass;
  punit->slotCount=1;
  //uchar_t cclass=getUnitClass(0,0);
  //printf("Run in getOlpWebInfo utype:%d\r\n",utype);
  if (3==utype || utype==20 || utype==21 || utype==13 || utype==32 ||utype>50)
  {
   if (cclass==1)
     punit->port_groups[0].isTxtAtTwoSide=1;
   else
     punit->port_groups[0].isTxtAtTwoSide=0;    
  }
  strcpy(punit->height,"70%");
  /*if (utype==1)
   strcpy(punit->btext,"1+1");
  else
   strcpy(punit->btext,"1:1");
  */
  if (utype<51)
  {
    if (uclass==UC_OSS)
    {
      strcpy(punit->port_groups[0].pimg,"scv1_");
    }
    else
    {
     strcpy(punit->led.run_text,"Pri");
     if (!(utype==13||utype==20 ||utype==21 ||utype==32))
         strcpy(punit->port_groups[0].pimg,"lcv2_");
     else
     {
         if (cclass==1)
            strcpy(punit->port_groups[0].pimg,"lcv2_");
         else
            strcpy(punit->port_groups[0].pimg,"lcv1_");
         //if (utype==32)
         //   strcpy(punit->port_groups[1].pimg,"scv1_");
     }
   }
  }
  else
  {
    strcpy(punit->led.run_text,"SET");
    if (cclass==1)
       strcpy(punit->port_groups[0].pimg,"lcv2_");
    else
        strcpy(punit->port_groups[0].pimg,"lcv1_");
  }
  //info->port_groups[0].isTxtAtTwoSide=0;
  strcpy(punit->port_groups[0].pclass,"tm15");
  
}

char getOlpLedsStatus(uchar_t chassis,uchar_t slot,char_t olp_type,signed char leds_status[][4])
{
  uchar_t n,m=0,uclass=0,utype=0;
  
  olp1P1Info_t *olp;
  if (TRUE!=getUnitClassType(0,slot,&uclass,&utype))
    return 0;
  if (UC_OLP!=uclass)
    return 0;
  memset(leds_status[0],-1,4);
  memset(leds_status[1],-1,4);
  memset(leds_status[2],-1,4);
 
  olp=getOlp1P1Info(chassis,slot);
  if (NULL==olp)
    return 0;
  leds_status[0][1]=1;
  leds_status[0][2]=olp->line;

  leds_status[1][1]=olp->tx_led;
  leds_status[1][2]=olp->mode;

  leds_status[2][1]=olp->rx1_led;
  leds_status[2][2]=olp->rx2_led;
  return 1;
}

int aspNot4UOlpView(webs_t wp, int slot,uchar_t olp_type)
{
  uchar_t n,port_count;
  signed char leds_status[3][4]={0};
  signed char ports_status[8]={2,2,2,2,2,2,2,2};

  char left_led_lbl[][6]={"PWR","TX","R1"};
  char right_led_lbl[][6]={"Pri","Auto","R2"};
  int bytes=0;
  char port_lbls[8][2][12]={{"RX",""},{"TX",""},{"R1",""},{"T1",""},{"R2",""},{"T2",""},{"",""},{"",""}};
  char port_lbls2[][12]={"","","","","","","",""};
 
  getOlpLedsStatus(0,slot,olp_type, leds_status);
  bytes=showHLeds(wp,leds_status[0],leds_status[1],leds_status[2],left_led_lbl, right_led_lbl,4);
  bytes+=showHUPorts(wp,slot,6,ports_status,port_lbls,1,1,"scv","png",25,15);
  return bytes;
}
/*
int static get4UOlpView(webs_t wp, uchar_t slot,uchar_t utype)
{
  asp4u_unit_view(wp,slot,UC_OLP,utype);
}
*/
int static get4UObpView(webs_t wp, uchar_t slot,uchar_t utype)
{
  return asp4u_unit_view(wp,slot,UC_OLP,utype);
  /*
  int nBytesSent=0,n;
  char rt[6][8]={"RX","TX","R1","T1","R2","T2"};
  char ledl[5][6]={"Pri","TX","Auto","R1","R2"};

  char bps_led,run_led,auto_led,r1_led,r2_led;
  char uname[6]="OBP";
  obpInfo_t *obp=getObpInfo(0,slot);
  //printf("get4UObpView:%d\r\n",slot);
  nBytesSent +=websWrite(wp, T("\n<div class='ubd1'>\
 <div class='un'><a href='/goform/slot?cid=0&uid=%d' target='mf'>%s</a></div>\n"),slot,uname);
 if (NULL==obp)
 {
   nBytesSent +=websWrite(wp, T("<div class='ui'>unkown</div>\n</div>\n"));
   return nBytesSent;
 }
 bps_led=(obp->line>0 && obp->line<4)?obp->line:1;
 auto_led=(obp->auto_led>0 && obp->auto_led<4)?obp->auto_led:1;
 r1_led=(obp->rx1_led>0 && obp->rx1_led<4)?obp->rx1_led:1;
 r2_led=(obp->rx2_led>0 && obp->rx2_led<4)?obp->rx2_led:1;
 run_led=(obp->set_led>0 && obp->set_led<4)?obp->set_led:1;
 printf("r1_led=%d obp->rx1_led=%d\r\n",r1_led,obp->rx1_led);
 nBytesSent +=websWrite(wp, T("<div class='leds'>\
 <div class='ll'>PWR</div>\n\
 <div class='led'><img src='images/led_1.png' /></div>\n\
 <div class='led'><img src='images/led_%d.png' /></div>\n\
 <div class='ll'>%s</div>\n\
 <div class='ll'>%s</div>\n\
 <div class='led'><img src='images/led_%d.png' /></div>\n\
 <div class='led'><img src='images/led_%d.png' /></div>\n\
 <div class='ll'>%s</div>\n\
 <div class='ll'>%s</div>\n\
 <div class='led'><img src='images/led_%d.png' /></div>\n\
 <div class='led'><img src='images/led_%d.png' /></div>\n\
 <div class='ll'>%s</div>\n\
 <div class='clr'></div>\n\
</div>\n"),run_led,"SET","BPS",bps_led,auto_led,"Auto","R1",r1_led,r2_led,"R2");
 return nBytesSent;
*/
}
int get4UOlpView(webs_t wp, uchar_t slot,uchar_t uclass,uchar_t utype)
{
  int nBytesSent=0,n;
  char rt[6][8]={"RX","TX","R1","T1","R2","T2"};
  char ledl[5][6]={"Pri","TX","Auto","R1","R2"};

  int line,mode,tx,r1,r2;
  char uname[6]="OLP";
  //char pimage[6]="p9_1";
  olp1P1Info_t *status;
  char strct_type=0;
  olp_property_t *property=getOlpProperty(0,slot);

  if (property!=NULL)
  {
    if (property->port_type==1)
       utype=11;
  }

  if (UC_OLP==uclass)
    status=getOlp1P1Info(0,slot);
  else
    status=getOssChannelInfo(0,slot,1);
  if (UC_OSS==uclass)
  {
    strcpy(uname,"OSS");
    //strcpy(pimage,"sc");
    strcpy(ledl[0],"RUN");
    strcpy(ledl[1],"S1");
    strcpy(ledl[2],"CH1");
    strcpy(ledl[3],"S2");
    strcpy(ledl[4],"CH2");
   
    strcpy(rt[0],"R1_1");
    strcpy(rt[1],"R1_2");
    strcpy(rt[2],"CH1");
    strcpy(rt[3],"R2_1");
    strcpy(rt[4],"R2_2");
    strcpy(rt[5],"CH2");
  }
  if (1==utype)
  {
     if (UC_OLP==uclass && strct_type==2)
     {
       strcpy(ledl[3],"Line1");
       strcpy(ledl[4],"Line2");
       strcpy(rt[2],"Line1");
       strcpy(rt[3],"Line2");
       strcpy(rt[4],"N/A");
       strcpy(rt[5],"N/A");
     }
  }
  else if (3==utype)
  {
     strcpy(ledl[0],"Set");
     strcpy(ledl[1],"Pri");
     strcpy(rt[0],"RX/TX");
     strcpy(rt[1],"R1/T1");
     strcpy(rt[2],"R2/T2");
     strcpy(rt[3],"SRX/STX");
     strcpy(rt[4],"O1RX/TX");
     strcpy(rt[5],"O2RX/TX");
  }
  else if (4==utype)
  {
     /*
     strcpy(ledl[0],"Set");
     strcpy(ledl[1],"Pri");
     */
     strcpy(rt[0],"RX/TX");
     strcpy(rt[1],"R1/T1");
     strcpy(rt[2],"R2/T2");
     strcpy(rt[3],"N/A");
     strcpy(rt[4],"N/A");
     strcpy(rt[5],"N/A");
  }
  else if (11==utype)//单纤双向
  {
     strcpy(ledl[3],"L1");
     strcpy(ledl[4],"L2");
     strcpy(rt[0],"RX");
     strcpy(rt[1],"TX");
     strcpy(rt[2],"Line1");
     strcpy(rt[3],"Line2");
     strcpy(rt[4],"N/A");
     strcpy(rt[5],"N/A");
  }
  nBytesSent +=websWrite(wp, T("\n<div class='ubd1'>\
 <div class='un'><a href='/goform/slot?cid=0&uid=%d' target='mf'>%s</a></div>\n"),slot,uname);
 if (NULL==status)
 {
   nBytesSent +=websWrite(wp, T("<div class='ui'>unkown</div>\n</div>\n"));
   return nBytesSent;
 }
if (UC_OLP==uclass)
{
 
 if (status->mode!=1)
   mode=2;
 else
   mode=1;
 if (status->rx1_led!=1)
     r1=2;
 else
     r1=1;
 if (status->rx2_led!=1)
     r2=2;
 else
     r2=1;
 //printf("rx2:%d lw:%d\n",status->rx2_power , status->l2_power);
 /*if (status->tx_led!=1)
     tx=2;
 else
     tx=1;
*/
 //printf("strct_type:%d\n",strct_type);
 if (strct_type!=1)
 {
  line=(status->line>0 && status->line<4)?status->line:1;
  //tx=(status->tx_led>0 && status->tx_led<4)?status->tx_led:1;
  tx=status->line;
 }
 else
 {
  //tx=(status->line>0 && status->line<4)?status->line:1;
  tx=status->line;
  line=(status->tx_led>0 && status->tx_led<4)?status->tx_led:1;
 }
  if (utype==3)
      tx=0;
 // printf("strct_type:%d utype=%d tx=%d status->tx_led:%d\n",strct_type,utype,tx,status->tx_led);
}
else
{
  /*
  tx=status->rx1_led<2?1:0;
  mode =status->rx2_led<2?1:2;
  */
  tx=1;
  line=status->rx1_led<2?1:0;
  mode =status->rx2_led<2?1:2;
  status=getOssChannelInfo(0,slot,2);
  if (NULL!=status)
  {
    r1=status->rx1_led<2?1:0;
    r2=status->rx2_led<2?1:2;
  }
  //line=(status->tx_led>0 && status->tx_led<4)?status->tx_led:1;
}
//if (utype==3)
    //  ledl[0]=tx;
 // printf("strct_type:%d utype=%d tx=%d status->tx_led:%d line=%d\n",strct_type,utype,tx,status->tx_led,line);

 nBytesSent +=websWrite(wp, T("<div class='leds'>\
 <div class='ll'>PWR</div>\n\
 <div class='led'><img src='images/led_1.png' /></div>\n\
 <div class='led'><img src='images/led_%d.png' /></div>\n\
 <div class='ll'>%s</div>\n\
 <div class='ll'>%s</div>\n\
 <div class='led'><img src='images/led_%d.png' /></div>\n\
 <div class='led'><img src='images/led_%d.png' /></div>\n\
 <div class='ll'>%s</div>\n\
 <div class='ll'>%s</div>\n\
 <div class='led'><img src='images/led_%d.png' /></div>\n\
 <div class='led'><img src='images/led_%d.png' /></div>\n\
 <div class='ll'>%s</div>\n\
 <div class='clr'></div>\n\
</div>\n"),tx,ledl[0],ledl[1],line,mode,ledl[2],ledl[3],r1,r2,ledl[4]);
 //printf("line=%d status->tx_led=%d status->line=%d\n",line,status->tx_led,status->line);
 char adpter[16];
 if (utype!=3)
    strcpy(adpter,"sc.gif");
 else
    strcpy(adpter,"lcv1_2.png");
 for (n=1;n<6;n++)
 {
  nBytesSent +=websWrite(wp, "<div class='adpter'");
  if (utype==3)
     nBytesSent +=websWrite(wp, " style='font-size:7px;'");
  nBytesSent +=websWrite(wp, T("><div class='pl'>%s</div>\n\
<div class='pimg'><img src='images/%s'></div>\n\
</div>\n\
<div class='clr'></div>"),rt[n-1],adpter);
}
nBytesSent +=websWrite(wp, "<div class='adpter'");
  if (utype==3)
     nBytesSent +=websWrite(wp, " style='font-size:7px;'");
 nBytesSent +=websWrite(wp, T(" vsb0'>\
<div class='pl'>%s</div>\n\
<div class='pimg'><img src='images/%s'></div>\n\
</div>\n\
<div class='clr'></div>"),rt[n-1],adpter);
if (uclass!=UC_OSS)
{
if (1==utype)
  nBytesSent +=websWrite(wp, T("<div class='ui'>1+1</div>"));
else if (2==utype ||3==utype)
  nBytesSent +=websWrite(wp, T("<div class='ui'>1:1</div>"));
}
nBytesSent +=websWrite(wp, T("</div>"));
return nBytesSent;
}

int asp4UOlpView(webs_t wp, uchar_t slot,uchar_t uclass,uchar_t utype)
{
  //printf("asp4UOlpView: utype=%d\r\n",utype);
  if (utype<51)
  {
     if (utype<20 && utype!=13/*!(utype==13 ||utype==31)*/)
        return get4UOlpView( wp, slot,uclass,utype);
     else
        return asp4u_unit_view(wp,slot,UC_OLP,utype);
  }
  else
    return get4UObpView( wp, slot,utype);
}

int aspOlpView(webs_t wp, uchar_t slot,uchar_t cclass,uchar_t uclass,uchar_t utype)
{
  //printf("Run in aspOlpView slot=%d utype=%d\n",slot,utype);
  if (4!=cclass)
     return aspNot4UOlpView(wp, slot,utype);
   else 
     return asp4UOlpView(wp, slot,uclass,utype);
}
int aspGetObpInfo(webs_t wp, int slot,uchar_t utype)
{
  int bytes=0;
  slot=getSelectedUnit();
  //printf("aspDvoaStatus..\n");
  obpInfo_t *obp;
  obp=getObpInfo(0,slot);
  if (obp==NULL)
  {
    bytes=websWrite(wp,"ut=0;");
    return bytes;
  }
  bytes+=websWrite(wp,T("ut=%d;\nvar obp= new Object();\n"),utype);
  bytes+=websWrite(wp,T("obp.mode=%d;\n"),obp->mode);
  bytes+=websWrite(wp,T("obp.ret_mode=%d;\n"),obp->ret_mode);
  bytes+=websWrite(wp,T("obp.line=%d;\n"),obp->line);
  bytes+=websWrite(wp,T("obp.rule=%d;\n"),obp->rule);
  bytes+=websWrite(wp,T("obp.ret_time=%d;\n"),obp->ret_time);

  bytes+=websWrite(wp,T("obp.rx_power=[%d,%d,%d,%d];\n"),obp->rx_power[0],obp->rx_power[1],obp->rx_power[2],obp->rx_power[3]);
  bytes+=websWrite(wp,T("obp.rx_wave_len=[%d,%d,%d,%d];\n"),obp->rx_wave_len[0],obp->rx_wave_len[1],obp->rx_wave_len[2],obp->rx_wave_len[3]);
  bytes+=websWrite(wp,T("obp.rx_led=[%d,%d,%d,%d];\n"),obp->rx1_led,obp->rx2_led,obp->auto_led,obp->set_led);
  bytes+=websWrite(wp,T("obp.l_switch_power=[%d,%d];\n"),obp->l_switch_power[0],obp->l_switch_power[1]);
  bytes+=websWrite(wp,T("obp.rx_alm_power=[%d,%d];\n"),obp->rx_alm_power[0],obp->rx_alm_power[1]);

  bytes=websWrite(wp,T("obp.heart_beat=%d;\n"),obp->heart_beat);

  //bytes=websWrite(wp,";\n");
  return bytes;
}
int aspGetOlpInfo(webs_t wp, int slot,uchar_t utype)
{
int bytes=0;
  //char power[10];

  //short value;
  slot=getSelectedUnit();
  //printf("aspDvoaStatus..\n");
  olp1P1Info_t  *olp;
  char *property,strct_type=1,sw_type=1,fw_type=1;
  
  olp=getOlp1P1Info(0,slot);
  if (olp==NULL)
  {
    bytes=websWrite(wp,"ut=0;");
    return bytes;
  }
  
  bytes+=websWrite(wp,T("ut=%d;\nvar olp= new Object();\n"),utype);
  property=getUnitPropertyMap(0,slot);
  if (property!=NULL)
  {
      fw_type=*(property+4);//获取单元盘结构属性
      sw_type=*(property+5);//获取单元盘结构属性
      strct_type=*(property+6);//获取单元盘结构属性
  }
  bytes+=websWrite(wp,T("olp.strct_type=%d;olp.fw_type=%d;olp.sw_type=%d;\n"),strct_type,fw_type,sw_type);
  bytes+=websWrite(wp,T("olp.master=%d;\n"),olp->master);
  bytes+=websWrite(wp,T("olp.mode=%d;\n"),olp->mode);
  bytes+=websWrite(wp,T("olp.ret_mode=%d;\n"),olp->ret_mode);
  bytes+=websWrite(wp,T("olp.line=%d;\n"),olp->line);
  bytes+=websWrite(wp,T("olp.rule=%d;\n"),olp->rule);
  bytes+=websWrite(wp,T("olp.ret_time=%d;\n"),olp->ret_time);
  
  bytes+=websWrite(wp,T("olp.rx_power=[%d,%d];\n"),olp->rx1_power,olp->rx2_power);
  bytes+=websWrite(wp,T("olp.tx_power=%d;\n"),olp->tx_power);
  bytes+=websWrite(wp,T("olp.l_switch_power=[%d,%d];\n"),olp->l1_power,olp->l2_power);
  bytes+=websWrite(wp,T("olp.tx_alm_power=%d;\n"),olp->tx_alm_power);
  bytes+=websWrite(wp,T("olp.rx_wave_len=[%d,%d];\n"),olp->rx1_wave_len,olp->rx2_wave_len);
  bytes+=websWrite(wp,T("olp.tx_wave_len=%d;\n"),olp->tx_wave_len);
  if (utype==21)
  {
    olpInfoWithBakupFiber_t *obf=(olpInfoWithBakupFiber_t *)olp;
    short power=-28;
    short  mf_power_switch=-28;
    short  bf_power_switch=-28;
    uchar_t wl=1;
    if (obf!=NULL)
    {
      power=obf->backup_fiber_power;
      wl=obf->backup_fiber_wave_len;
      mf_power_switch=obf->mf_power_switch;
      bf_power_switch=obf->bf_power_switch;
    }
    bytes+=websWrite(wp,T("olp.bf_wave_len=%d;\n"),wl);
    bytes+=websWrite(wp,T("olp.bf_power=%d;\n"),power);
    bytes+=websWrite(wp,T("olp.power_switch=[%d,%d];\n"),mf_power_switch,bf_power_switch);
  }
  else if (utype==13 ||utype==31 ||utype==32)
  {
    short rx_power=0,t1_power=0,t2_power=0,t2_power2=0;
    uchar_t isolation=1;
    olpWithIsolation_t  *owi=(olpWithIsolation_t *)olp;
    if (owi!=NULL)
    {
       rx_power=olp->rx_power;
       t1_power=olp->t1_power;
       t2_power=olp->t2_power;
       t2_power2=owi->t2_1490_power;
       isolation=owi->isolation;
    }
    bytes+=websWrite(wp,T("olp.RxPower=%d;\n"),rx_power);
    bytes+=websWrite(wp,T("olp.t1_power=%d;\n"),t1_power);
    bytes+=websWrite(wp,T("olp.t2_power=%d;\n"),t2_power);
    bytes+=websWrite(wp,T("olp.t2_power2=%d;\n"),t2_power2);
    bytes+=websWrite(wp,T("olp.isolation=%d;\n"),isolation);
    /*
    int n=0;
    //char *data=getInfoSet(0,slot,65,1);
    char *data=(char *)olp;
    printf("Data of unit #%d\n",slot);
    if (data!=NULL)
    {
    for (n=0;n<36;n++)
    {
      printf("[%d]=%d ",n,data[n]);
    }
    }
    //printf("\nData[ #%d\n",slot);
    */
  }
 return bytes;
}
int aspOlpInfo(webs_t wp, int slot,uchar_t uclass,uchar_t utype,int group)
{
  //printf("aspOlpInfo:uc=%d ut=%d\r\n",uclass,utype);
  if (uclass==UC_OLP)
  {
    if (utype<51)
      return aspGetOlpInfo(wp,slot,utype);
    else
    return aspGetObpInfo(wp,slot,utype);
  }
  int n,m,nBytesSent=0;
  olp1P1Info_t  *olp;
  char rx1_power[6];
  char rx2_power[6];
  char tx_power[6];
  char rx_power[6];
  char t1_power[6];
  char t2_power[6];

  char checked[2][13]={"","checked=true"};
  char rt[6][12]={"R1","TX","R1","T1","R2","T2"};
  char line[2][5]={"L1","L2"};
  char i,ch_count=1,colspan=5;
  //uchar_t utype=1;
  char *property,strct_type=1,sw_type=1,fw_type=1;
  property=getUnitPropertyMap(0,slot);
  if (property!=NULL)
  {
      fw_type=*(property+4);//获取单元盘结构属性
      sw_type=*(property+5);//获取单元盘结构属性
      strct_type=*(property+6);//获取单元盘结构属性
  }
  //printf("fw_type=%d sw_type=%d\n",fw_type,sw_type);
  //olp=getOlp1P1Info(0,slot);
  olp=getOssChannelInfo(0,slot,group);
  if (olp==NULL)
  {
    return websWrite(wp, T("<tr><td>Unit #%d is NOT OLP or not exsits!</td></tr>"),slot);
  }
  //printf("group=%d\n",group);
  if (UC_OSS==uclass)
  {
    sprintf(rt[0],"R%d_1",group);
    strcpy(rt[1],rt[0]);
    strcpy(rt[2],rt[0]);
    strcpy(rt[3],"R2_1");
    sprintf(rt[4],"R%d_2",group);
    strcpy(rt[5],"CH2");
    sprintf(line[0],"R%d_1",group);
    sprintf(line[1],"R%d_2",group);
    ch_count=2;
    colspan=2;
  }
  else 
  {
   if (3==utype)
       colspan=2;
   else if (2==strct_type)
   {
      strcpy(rt[0],"L1");
      strcpy(rt[2],"L1");
      strcpy(rt[4],"L2");
   }
  }
  sprintf(rx1_power,"%.1f",olp->rx1_power*0.1);
  sprintf(rx2_power,"%.1f",olp->rx2_power*0.1);
  sprintf(tx_power,"%.1f",olp->tx_power*0.1);
  
   nBytesSent+=websWrite(wp,T("<tr><th>%s"),rt[0]);
    if (2==strct_type)
    {
     #ifdef DEFAULT_LANG_CN
       nBytesSent+=websWrite(wp,"接收");
     #else
       nBytesSent+=websWrite(wp,"Rx");
     #endif
    }
    #ifdef DEFAULT_LANG_CN
       nBytesSent=websWrite(wp, T("光功率</th><td>%sdBm</td>"),rx1_power);
     #else
       nBytesSent=websWrite(wp, T(" power</th><td>%sdBm</td>"),rx1_power);
     #endif
    
    nBytesSent+=websWrite(wp,T("<th>%s"),rt[4]);
    if (2==strct_type)
    {
     #ifdef DEFAULT_LANG_CN
       nBytesSent+=websWrite(wp,"接收");
     #else
       nBytesSent+=websWrite(wp,"Rx");
     #endif
    }
     #ifdef DEFAULT_LANG_CN
       nBytesSent+=websWrite(wp,T("光功率</th><td>%sdBm</td>"),rx2_power);
     #else
       nBytesSent+=websWrite(wp,T(" power</th><td>%sdBm</td>"),rx2_power);
     #endif
    
   if (UC_OLP==uclass)
   {
    #ifdef DEFAULT_LANG_CN
       nBytesSent=websWrite(wp, T("<th>%s光功率</th><td>%sdBm</td>"),rt[1],tx_power);
     #else
       nBytesSent=websWrite(wp, T("<th>%s power</th><td>%sdBm</td>"),rt[1],tx_power);
     #endif
   }
   nBytesSent=websWrite(wp, T("</tr>"));
   sprintf(rx1_power,"%.1f",olp->l1_power*0.1);
   sprintf(rx2_power,"%.1f",olp->l2_power*0.1);
   sprintf(tx_power,"%.1f",olp->tx_alm_power*0.1);
   
   if (olp->rx1_wave_len!=1)
   {
      n=0;
      m=1;
   }
   else
   {
      n=1;
      m=0;
   }
   nBytesSent+=websWrite(wp, T("<tr><th>%s"),rt[2]);
   #ifdef DEFAULT_LANG_CN
     nBytesSent+=websWrite(wp, "波长");
   #else
     nBytesSent+=websWrite(wp, " wave length");
   #endif
  nBytesSent+=websWrite(wp, T("</th><td><input type='radio' %s name='r1wl' id='r1wl' value='1'>1310nm&nbsp;<input type='radio' %s name='r1wl' value='2'>1550nm</td>"),checked[n],checked[m]);
   
   if (olp->rx2_wave_len!=1)
   {
      n=0;
      m=1;
   }
   else
   {
      n=1;
      m=0;
   }
   nBytesSent+=websWrite(wp, T("<th>%s"),rt[4]);
   #ifdef DEFAULT_LANG_CN
     nBytesSent+=websWrite(wp, "波长");
   #else
     nBytesSent+=websWrite(wp, " wave length");
   #endif
   nBytesSent+=websWrite(wp, T("</th><td><input type='radio' %s name='r2wl' id='r2wl' value='1'>1310nm&nbsp;<input type='radio' %s name='r2wl' value='2'>1550nm</td>"),checked[n],checked[m]);
  if (UC_OLP==uclass)
  {
   getUnitClassType(0,slot,&uclass,&utype);
   if (olp->tx_wave_len!=1)
   {
      n=0;
      m=1;
   }
   else
   {
      n=1;
      m=0;
   }
   nBytesSent+=websWrite(wp, T("<th>%s"),"TX");
   #ifdef DEFAULT_LANG_CN
     nBytesSent+=websWrite(wp, "波长");
   #else
     nBytesSent+=websWrite(wp, " wave length");
   #endif
   nBytesSent+=websWrite(wp, T("</th><td><input type='radio' %s name='txwl' id='txwl' value='1'>1310nm&nbsp;<input type='radio' %s name='txwl' value='2'>1550nm</td>"),checked[n],checked[m]);
}
   nBytesSent+=websWrite(wp, T("</tr>"));
   nBytesSent+=websWrite(wp, T("<tr><th id='T4'>%s"),line[0]);
   #ifdef DEFAULT_LANG_CN
     nBytesSent+=websWrite(wp, "切换功率");
   #else
     nBytesSent+=websWrite(wp, " switch power");
   #endif
    nBytesSent+=websWrite(wp, T("</th><td><input type='text' name='L1p' id='i4' value='%s' maxlength='5' size='5' onkeyup='inputNumber(this,1,true);' onblur='checkPower(this,9);'>dBm</td> \
    <th id='T5'>%s"),rx1_power,line[1]);
    #ifdef DEFAULT_LANG_CN
     nBytesSent+=websWrite(wp, "切换功率");
   #else
     nBytesSent+=websWrite(wp, " switch power");
   #endif
    nBytesSent+=websWrite(wp, T("</th><td><input type='text' name='L2p' id='i5' value='%s' maxlength='5' size='5' onkeyup='inputNumber(this,1,true);' onblur='checkPower(this,10);'>dBm</td>"),rx2_power);
    if (UC_OLP==uclass)
    {
     nBytesSent+=websWrite(wp, "<th id='T6'>TX");
     #ifdef DEFAULT_LANG_CN
        nBytesSent+=websWrite(wp, "告警功率");
     #else
       nBytesSent+=websWrite(wp, " alarm power");
     #endif
    nBytesSent+=websWrite(wp, T("</th><td><input type='text' name='Txp' id='i6' value='%s' maxlength='5' size='5' onkeyup='inputNumber(this,1,true);' onblur='checkPower(this,11);'>dBm</td>"),tx_power);
   }
   nBytesSent+=websWrite(wp, T("</tr>"));
   if (olp->mode!=1)
   {
      n=0;
      m=1;
   }
   else
   {
      n=1;
      m=0;
   }
   //sw_type=2;
   if (fw_type==3)
  {
    sprintf(rx_power,"%.1f",olp->rx_power*0.1);
    sprintf(t1_power,"%.1f",olp->t1_power*0.1);
    sprintf(t2_power,"%.1f",olp->t2_power*0.1);
    
    nBytesSent+=websWrite(wp, "<tr><th>RX");
    #ifdef DEFAULT_LANG_CN
      nBytesSent+=websWrite(wp, "光功率");
    #else
      nBytesSent+=websWrite(wp, " power");
    #endif
    #ifdef DEFAULT_LANG_CN
    nBytesSent+=websWrite(wp, T("</th><td>%sdBm</td><th>T1光功率</th><td>%sdBm</td><th>T2光功率</th><td>%sdBm</td></tr>"),rx_power,t1_power,t2_power);
    #else
      nBytesSent+=websWrite(wp, T("</th><td>%sdBm</td><th>T1 power</th><td>%sdBm</td><th>T2 power</th><td>%sdBm</td></tr>"),rx_power,t1_power,t2_power);
    #endif
  }
   #ifdef DEFAULT_LANG_CN
   nBytesSent+=websWrite(wp, T("<tr><th>工作模式</th><td><input type='radio' %s name='mode' id='mode' value='1' \
    onclick='javascript:checkmode();'>自动&nbsp;<input type='radio' %s name='mode' value='2' onclick='javascript:checkmode();'>手动</td>"),checked[n],checked[m]);
   #else
      nBytesSent+=websWrite(wp, T("<tr><th>Working mode</th><td><input type='radio' %s name='mode' id='mode' value='1' \
    onclick='javascript:checkmode();'>Auto&nbsp;<input type='radio' %s name='mode' value='2' onclick='javascript:checkmode();'>Manual</td>"),checked[n],checked[m]);
   #endif
   if (olp->line!=1)
   {
      n=0;
      m=1;
   }
   else
   {
      n=1;
      m=0;
   }
  if (UC_OLP==uclass)
  {
   #ifdef DEFAULT_LANG_CN
   nBytesSent+=websWrite(wp, T("<td><span id='linesel'><input type='radio' %s name='line'id='L1' value='1'>主路(L1)&nbsp;<input type='radio' name='line'id='L2' %s value='2'>备路(L2)</span></td>"),checked[n],checked[m]);
  #else
       nBytesSent+=websWrite(wp, T("<td><span id='linesel'><input type='radio' %s name='line'id='L1' value='1'>Main line(L1)&nbsp;<input type='radio' name='line'id='L2' %s value='2'>Protected line(L2)</span></td>"),checked[n],checked[m]);
 #endif
  }
  else
  {
   #ifdef DEFAULT_LANG_CN
   nBytesSent+=websWrite(wp, T("<td colspan='2'><span id='linesel'><input type='radio' %s name='line'id='L1' value='1'>主路(%s)&nbsp;<input type='radio' name='line'id='L2' %s value='2'>备路(%s)</span></td>"),checked[n],line[0],checked[m],line[1]);
   #else
     nBytesSent+=websWrite(wp, T("<td colspan='2'><span id='linesel'><input type='radio' %s name='line'id='L1' value='1'>Main line(%s)&nbsp;<input type='radio' name='line'id='L2' %s value='2'>Protected line(%s)</span></td>"),checked[n],line[0],checked[m],line[1]);
   #endif
  }
   if (olp->rule!=1)
   {
      n=0;
      m=1;
   }
   else
   {
      n=1;
      m=0;
   }
  if (UC_OLP==uclass )
  {
   #ifdef DEFAULT_LANG_CN
   if (2!=utype)
   nBytesSent+=websWrite(wp, T("<th>切换规则</th><td colspan='2'><input type='radio' name='rule' value='1' \
    %s>工作光纤优先&nbsp;<input type='radio' name='rule' value='2' %s>光功率大优先</td></tr>"),checked[n],checked[m]);
   else
       nBytesSent+=websWrite(wp, T("<th>回切到主路</th><td colspan='2'><input type='radio' name='rule' value='1' \
    >回切&nbsp;<input type='radio' name='rule' value='2' checked=true>不回切</td></tr>"));
   #else
    if (2!=utype)
   nBytesSent+=websWrite(wp, T("<th>Switch rule</th><td colspan='2'><input type='radio' name='rule' value='1' \
    %s>Working fiber first&nbsp;<input type='radio' name='rule' value='2' %s>Power first</td></tr>"),checked[n],checked[m]);
   else
       nBytesSent+=websWrite(wp, T("<th>Return main line</th><td colspan='2'><input type='radio' name='rule' value='1' \
    >Return&nbsp;<input type='radio' name='rule' value='2' checked=true>Not return</td></tr>"));
   #endif
  }
  else
  {
    #ifdef DEFAULT_LANG_CN
    nBytesSent+=websWrite(wp, T("</tr><tr><th>切换规则</th><td colspan='3'><input type='radio' name='rule' value='1' \
    %s>工作光纤优先&nbsp;<input type='radio' name='rule' value='2' %s>光功率大优先</td></tr>"),checked[n],checked[m]);
    #else
       nBytesSent+=websWrite(wp, T("</tr><tr><th>Switch rule</th><td colspan='3'><input type='radio' name='rule' value='1' \
    %s>Working fiber first &nbsp;<input type='radio' name='rule' value='2' %s>Power first</td></tr>"),checked[n],checked[m]);
   #endif
  }
   if (olp->ret_mode!=1)
   {
      n=0;
      m=1;
   }
   else
   {
      n=1;
      m=0;
   }
   if (2!=utype)
   {
    if (3==utype)
       colspan=2;
     #ifdef DEFAULT_LANG_CN
     nBytesSent+=websWrite(wp, T("<tr><th>返回模式</th> \
    <td colspan='%d'><input type='radio' name='ret' id='ret' value='1' %s onclick='javascript:check(false);'>自动返回&nbsp;&nbsp;<span  id='T7'>返回时间(0-999分钟):<input type='text' name='rettime' id='rettime' value='%d' maxlength='3' size='3' onkeyup='uIntInput(this);' onblur='valid(this,7);'></span><input type='radio' %s name='ret' id='ret' value='2' onclick='javascript:check(true);'>手动返回</td>"),colspan,checked[n],olp->ret_time,checked[m]);
    #else
     nBytesSent+=websWrite(wp, T("<tr><th>Retuen mode</th> \
    <td colspan='%d'><input type='radio' name='ret' id='ret' value='1' %s onclick='javascript:check(false);'>Auto return&nbsp;&nbsp;<span  id='T7'>Return time(0-999Minutes):<input type='text' name='rettime' id='rettime' value='%d' maxlength='3' size='3' onkeyup='uIntInput(this);' onblur='valid(this,7);'></span><input type='radio' %s name='ret' id='ret' value='2' onclick='javascript:check(true);'>Manual return</td>"),colspan,checked[n],olp->ret_time,checked[m]);
  #endif
 if (3==utype)
 {
  nBytesSent+=websWrite(wp,"<th>");
  #ifdef DEFAULT_LANG_CN
    nBytesSent+=websWrite(wp,"主从");
  #else
     nBytesSent+=websWrite(wp,"Master-slave");
  #endif
  nBytesSent+=websWrite(wp,"</th><td colspan='2'><input type='radio' name='master' id='master1' value='1'");
  //printf("master:%d\n",olp->master);
  if (olp->master==1)
    nBytesSent+=websWrite(wp," checked");
  nBytesSent+=websWrite(wp,"/>");
  #ifdef DEFAULT_LANG_CN
    nBytesSent+=websWrite(wp,"主");
  #else
     nBytesSent+=websWrite(wp,"Master");
  #endif
  nBytesSent+=websWrite(wp,"<input type='radio' name='master' id='master2' value='2'");
  if (olp->master!=1)
    nBytesSent+=websWrite(wp," checked");
  nBytesSent+=websWrite(wp,"/>");
  #ifdef DEFAULT_LANG_CN
    nBytesSent+=websWrite(wp,"从");
  #else
     nBytesSent+=websWrite(wp,"Slave");
  #endif
  nBytesSent+=websWrite(wp,"</td>");
}
 nBytesSent+=websWrite(wp,"</tr>");
 }
//}
 return nBytesSent;
     
}


static void formOlp(webs_t wp, char_t *path, char_t *query)
{
  char_t *page,*act,*slot,*mode,*line,*group,*rule,*retmode,*rettime,*power,*wave_len,*master;
  char pagefile[20];
  int unit,index;
  setInfoSet_t infoset;

  slot = websGetVar(wp, T("uid"), T("0"));
  act  = websGetVar(wp, T("act"), T("0"));
  page  = websGetVar(wp, T("page"), T("olp"));
  group=websGetVar(wp, T("group"), T("1"));
  sprintf(pagefile,"%sret.asp",page);
  unit=atoi(slot);
   
  if (unit>0)
  {
    //printf("set unit slot=%d act=%s\n",unit,act);
    memset(&infoset,0,sizeof(setInfoSet_t));
    infoset.infoset_id=OLP_STATUS_INFOSET_ID;
    infoset.slot=unit;
    infoset.index=atoi(group);
    /*
    infoset.infoItems[0].item_id=OLP1_MODE;
    infoset.infoItems[1].item_id=OLP1_LINE;
    infoset.infoItems[2].item_id=OLP1_RULE;
    infoset.infoItems[3].item_id=OLP1_RET_MODE;
    */
    infoset.infoItems[0].item_id=OLP1_L1_SW_POWER;
    infoset.infoItems[1].item_id=OLP1_L2_SW_POWER;
    infoset.infoItems[2].item_id=OLP1_TX_ALM_POWER;
    infoset.infoItems[3].item_id=OLP1_RET_TIME;
    infoset.infoItems[4].item_id=OLP1_MODE;
    
    if (!gstrcmp(act,"1"))
    {
      int n=5;
      mode = websGetVar(wp, T("mode"), T("0"));
      line = websGetVar(wp, T("line"), T("0"));
      rule = websGetVar(wp, T("rule"), T("0"));
      retmode = websGetVar(wp, T("ret"), T("0"));
      rettime= websGetVar(wp, T("rettime"), T("0"));
      master=  websGetVar(wp, T("master"), T("0"));
    
    //infoset.infoItems[0].item_id=OLP1_MODE;
    infoset.infoItems[4].item_value=atoi(mode);
    //infoset.infoItems[1].item_id=OLP1_LINE;
    if (gstrcmp(line,"0"))
    {
      infoset.infoItems[n].item_id=OLP1_LINE;
      infoset.infoItems[n].item_value=atoi(line);
      n++;
    }
    infoset.infoItems[n].item_id=OLP1_RULE;
    infoset.infoItems[n].item_value=atoi(rule);
    n++;
    infoset.infoItems[n].item_id=OLP1_RET_MODE;
    infoset.infoItems[n].item_value=atoi(retmode);
    
    n++;
    infoset.infoItems[n].item_id=OLP1_R1_WAVE_LEN;
    wave_len= websGetVar(wp, T("r1wl"), T("1"));
    infoset.infoItems[n].item_value=atoi(wave_len);

    n++;
    infoset.infoItems[n].item_id=OLP1_R2_WAVE_LEN;
    wave_len= websGetVar(wp, T("r2wl"), T("1"));
    infoset.infoItems[n].item_value=atoi(wave_len);

    n++;
    infoset.infoItems[n].item_id=OLP1_TX_WAVE_LEN;
    wave_len= websGetVar(wp, T("txwl"), T("1"));
    infoset.infoItems[n].item_value=atoi(wave_len);
    
    n++;
    infoset.infoItems[n].item_id=OLP1_BF_WAVE_LEN;
    wave_len= websGetVar(wp, T("bfwl"), T("1"));
    infoset.infoItems[n].item_value=atoi(wave_len);

    master= websGetVar(wp, T("master"), T("0"));
    if (gstrcmp(master,"0"))
    {
    n++;
    infoset.infoItems[n].item_id=OLP1_MASTER;
    //master= websGetVar(wp, T("master"), T("0"));
    infoset.infoItems[n].item_value=atoi(master);
    //printf("item id=%d master=%d\n",infoset.infoItems[n].item_id,infoset.infoItems[n].item_value);
    }
    //infoset.infoItems[4].item_id=OLP1_RET_TIME;
    
    //n++;
    power=websGetVar(wp, T("L1p"), T("-280"));
   // infoset.infoItems[5].item_id=OLP1_L1_SW_POWER;
    infoset.infoItems[0].item_value=(short)(atof(power)*10);
    // printf("value=%d power=%f l1p:%s\n",infoset.infoItems[5].item_value,atof(power),power);
    power=websGetVar(wp, T("L2p"), T("-280"));
    //infoset.infoItems[6].item_id=OLP1_L2_SW_POWER;
    infoset.infoItems[1].item_value=(short)(atof(power)*10);
    power=websGetVar(wp, T("Txp"), T("-280"));
    //infoset.infoItems[7].item_id=OLP1_TX_ALM_POWER;
    infoset.infoItems[2].item_value=(short)(atof(power)*10);
    infoset.infoItems[3].item_value=atoi(rettime);
    //for (n=0;n<7;n++)
    // printf("%d=%x ",n,infoset.infoItems[n].item_value & 0xffff);
    //printf("\n");
    //printf("set olp l1=%d l2=%d tx=%d\n",infoset.infoItems[5].item_value,infoset.infoItems[6].item_value,infoset.infoItems[7].item_value);
     }
    else if (!gstrcmp(act,"2"))
     {
      //printf("set olp act=2\n");
      infoset.infoItems[0].item_value=1;
      infoset.infoItems[1].item_value=1;
      infoset.infoItems[2].item_value=1;
      infoset.infoItems[3].item_value=1;
      infoset.infoItems[4].item_value=333;
      infoset.infoItems[5].item_value=-280;
      infoset.infoItems[6].item_value=-280;
      infoset.infoItems[7].item_value=-280;
     }
    else
     {
       websRedirect(wp,pagefile);
       return;
     }
    infoset.useconds=1000000;
    setOlpInfo(infoset);
    if(!gstrcmp(page,"olp"))
       insertOperateLog(LOG_UNIT_OLP_CONF,0,0,WebMode,mod,unit,0);
    else if(!gstrncmp(page,"oss",3))
       insertOperateLog(LOG_UNIT_OSS_CONF,0,0,WebMode,mod,unit,0);
  }
  websRedirect(wp,pagefile);
}
void olpAspDefine()
{
 //websAspDefine(T("OlpInfo"), aspOlpInfo);
 websFormDefine(T("olp"), formOlp);
}
