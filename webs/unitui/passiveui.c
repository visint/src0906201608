#include <string.h>
#include <sys/sysinfo.h>
#include <time.h>
#include "../wsIntrn.h"
#include "../../include/unit.h"
#include "../unitwebs.h"

#include <public.h>
#include <visipc.h>
#include <sqlite3.h>
#include <dbutil.h>
#include <log.h>
#include <chassis.h>
#include <passive.h>
#include <sysinfo.h>

static boolean_t getOdmWebInfo(uchar_t chassis,uchar_t cclass,uchar_t slot,uchar_t uclass,uchar_t utype,web_unit_t *punit)
{
  punit->uclass=PASSIVE_ODM_UNIT;
  punit->slotCount=1;
  punit->port_groups[0].isTxtAtTwoSide=1;
  strcpy(punit->height,"80%");
  strcpy(punit->port_groups[0].pclass,"tm15 imgw20h32");
  strcpy(punit->port_groups[0].pimg,"lcv2_");
  return TRUE;
}
static boolean_t getOadmWebInfo(uchar_t chassis,uchar_t cclass,uchar_t slot,uchar_t uclass,uchar_t utype,web_unit_t *punit)
{
  punit->uclass=PASSIVE_ODM_UNIT;
  punit->slotCount=1;
  punit->port_groups[0].isTxtAtTwoSide=1;
  strcpy(punit->height,"80%");
  strcpy(punit->port_groups[0].pclass,"tm15 imgw20h32");
  strcpy(punit->port_groups[0].pimg,"lcv2_");
  return TRUE;
}
boolean_t getPassiveWebInfo(uchar_t chassis,uchar_t cclass,uchar_t slot,uchar_t uclass,uchar_t utype,web_unit_t *punit)
{
  switch (utype){
    case PASSIVE_ODM_UNIT:
         return getOdmWebInfo(chassis,cclass,slot,uclass,utype,punit);
    //case PASSIVE_DCM1_UNIT:
    //     return getOdmWebInfo(chassis,cclass,slot,uclass,utype,punit);
    case PASSIVE_OADM_UNIT:
         return getOadmWebInfo(chassis,cclass,slot,uclass,utype,punit);
    default:
         return FALSE;
  }
  return FALSE;
}
static int writePassiveUnitView(webs_t wp,uchar_t slot,uchar_t utype,int portCount,char portType)
{
 char n,m,k,l,count,max,min,maxwl;
 char topPortCount;
 char rightTopTextType;
 char *property;
 int nBytesSent=0;
 nBytesSent +=websWrite(wp, T("<table"));
 if (!(PASSIVE_ODM2_UNIT==utype || PASSIVE_ODM3_UNIT==utype))
   nBytesSent +=websWrite(wp, T(" class='hport'"));
 nBytesSent +=websWrite(wp, T(">"));
 switch (utype)
 {
   case PASSIVE_OSU_UNIT:
       property=getUnitProperty(0,slot);
       if (NULL!=property)
       {
         osuProperty_t *osu=(osuProperty_t*)property;
         max=osu->groups;
       if (max<1 ||max>6)
          max=5;
       //nBytesSent +=websWrite(wp, T("<tr><td colspan='2'>OUT%d<br/>OUT%d</td></tr>"),n,n);
       for (n=6;n>0;n--)
       {
         nBytesSent +=websWrite(wp, T("<tr><td>"));
         if (n>max)
            nBytesSent +=websWrite(wp, T("<span class='na'>N/A<br/>N/A</span>"));
         else
           nBytesSent +=websWrite(wp, T("OUT%d<br/>OUT%d"),n,n);

         nBytesSent +=websWrite(wp, T("</td><td><img src='images/lc.gif'></td></tr>"));
       }
       nBytesSent +=websWrite(wp, T("<tr><td class='na'>N/A<br/>N/A</td><td><img src='images/lc.gif'></td></tr>"));
       for (n=3;n>0;n--)
       {
         nBytesSent +=websWrite(wp, T("<tr><td>"));
         if (n*2-1>max)
            nBytesSent +=websWrite(wp, T("<span class='na'>N/A</span>"));
         else
            nBytesSent +=websWrite(wp, T("IN%d"),n*2-1);
 
         if (n*2>max)
            nBytesSent +=websWrite(wp, T("<br/><span class='na'>N/A</span>"));
         else
            nBytesSent +=websWrite(wp, T("<br/>IN%d"),n*2);
         nBytesSent +=websWrite(wp, T("</td><td><img src='images/lc.gif'></td></tr>"),n,n);
         //nBytesSent +=websWrite(wp, T("<tr><td>IN%d<br/>IN%d</td>\
         <td><img src='images/lc.gif'></td></tr>"),n,n);
        }
       }
       break;
   case PASSIVE_ODM_UNIT:
       property=getUnitProperty(0,slot);
       if (NULL!=property)
       {
        char pText[10][8]={"IN","OUT","COM","CTX","CRX","UTX","URX","PASS"};
        odmProperty_t *odm=(odmProperty_t*)property;
        unitViewInfo_t uvinfo;


        if (getPassiveViewInfo(0,slot,UC_PASSIVE,PASSIVE_ODM_UNIT,&uvinfo)!=FALSE)
        { 
         for (n=0;n<10;n++)
         {
           nBytesSent +=websWrite(wp, "<tr><td>");
           if (strcmp(uvinfo.ports.ports_text[0][n],"N/A"))
              nBytesSent +=websWrite(wp, T("%s"),uvinfo.ports.ports_text[0][n]);
           else
              nBytesSent +=websWrite(wp, "<span class='na'>N/A</span>");
           nBytesSent +=websWrite(wp, T("<br/>"));
           if (strcmp(uvinfo.ports.ports_text[1][n],"N/A"))
              nBytesSent +=websWrite(wp, T("%s"),uvinfo.ports.ports_text[1][n]);
           else
              nBytesSent +=websWrite(wp, "<span class='na'>N/A</span>");

          nBytesSent +=websWrite(wp, T("</td><td><img src='images/lc.gif'></td></tr>"));
         }
        }

       if (odm->leftWaveLength[10]>0 ||odm->rightWaveLength[10]>0)
        {
         nBytesSent +=websWrite(wp, T("<tr><td></td><td>"));
         if (251==odm->leftWaveLength[10])
           nBytesSent +=websWrite(wp, T("OM"));
         else if (252==odm->leftWaveLength[10])
           nBytesSent +=websWrite(wp, T("OD"));
         nBytesSent +=websWrite(wp, T("  "));
         if (251==odm->rightWaveLength[10])
           nBytesSent +=websWrite(wp, T("OM"));
         else if (252==odm->rightWaveLength[10])
           nBytesSent +=websWrite(wp, T("OD"));
         nBytesSent +=websWrite(wp, T("</td></tr>"));
        }
       }
       break;
   case PASSIVE_ODM12_UNIT:
      property=getUnitProperty(0,slot);
      if (NULL!=property)
      {
          int n;
        for (n=0;n<4;n++)
        {
            nBytesSent +=websWrite(wp, T("<tr><td><img src='images/lc.gif'></td></tr>"));
        }
      }
      break;
   case PASSIVE_ODM2_UNIT:
       property=getUnitProperty(0,slot);
       if (NULL!=property)
       {
          odm2Property_t *odm;
          odm=(odm2Property_t *)property;
          //printf("#%d count=%d\n",slot,odm->channelCount);
          min=*(property+1);
          max=*property+min-1;
          topPortCount=odm->topPortCount;
          rightTopTextType=odm->rightTopTextType;
          if (topPortCount!=2)
             topPortCount=4;
          //printf("slot#%d min=%d max=%d\n",slot,min,max);
       }

       if (max>64 || max<15)
       {
          max=64;
       }
       if (min>64 || min<15)
       {
          min=15;
       }
       if (min!=15)
          maxwl=64;
       else
          maxwl=62;
       websWrite(wp, T("<tr><td colspan='3' style='text-align:center'>COM<img height=\"16\"  src='images/"));
       if (topPortCount>2)
            websWrite(wp, "lch.jpg");
       else
            websWrite(wp, "lc.gif");
       websWrite(wp, "'>");
       if (1!=rightTopTextType)
          websWrite(wp, "N/A");
       else if (1==rightTopTextType)
          websWrite(wp, "MON");
       websWrite(wp, "</td></tr>");
       for (n=0;n<4;n++)
       {
         nBytesSent +=websWrite(wp, T("<tr>"));
         for (m=0;m<3;m++)
         {
          k=maxwl-n*4-16*m;
          nBytesSent +=websWrite(wp, T("<td><table class='portl'><tr><td><table><tr>"));
          for (l=0;l<4;l++)
          {
           nBytesSent +=websWrite(wp, T("<tr><td"));
           /*if ((k-l)<=max && (k-l)>=min)
              nBytesSent +=websWrite(wp, T(">C%d</td>"),k-l);
           else
              nBytesSent +=websWrite(wp, T(" style='color:yellow;'>N/A</td>"));
           */
           if (k>(max+l) || k<(min+l))
              nBytesSent +=websWrite(wp, T(" class='na'>N/A</td>"));
           else
              nBytesSent +=websWrite(wp, T(">C%d</td>"),k-l);
           
          }
          nBytesSent +=websWrite(wp, T("</tr></table></td>"));
          nBytesSent +=websWrite(wp, T("<td><img height=\"40\" width='16' src='images/lcv.jpg'></td></tr></table></td>"));
         }
         nBytesSent +=websWrite(wp, T("</tr>"));
       }
      break;
     case PASSIVE_ODM3_UNIT:
          {
             //char[10][]={"COM1","Red","","","","","","","",""};
          }
           //nBytesSent +=websWrite(wp, T("<tr><td>COM1</td><td><img height=\"40\" width='16' src='images/sc.gif'></td></tr>"));
           nBytesSent +=websWrite(wp, T("<tr><td><img  src='images/odm3_v.png' height=\"230\" width='58' /></td></tr>"));
        break;
     case PASSIVE_NET_UNIT:
     break;
     case PASSIVE_OADM_UNIT:
          property=getUnitProperty(0,slot);
          if (NULL!=property)
          {
              char label1[16]="",label2[16]="";
              char *ptr=property+4;
              m=0;
              for (n=0;n<10;n++)
              {
                //label1[0]='\0';
                //label2[0]='\0';
                strcpy(label1,"N/A");
                strcpy(label2,"N/A");
                if (*(ptr+n*4)==0 && *(ptr+n*4+2)==0)
                {
                   strcpy(label1,"N/A");
                   strcpy(label2,"N/A");
                }
                else
                {
                 //printf("n=%d *(ptr+n*4)=%d *(ptr+n*4+1)=%d\n",n,*(ptr+n*4),*(ptr+n*4+1));
                 if (*(ptr+n*4)<13)
                 {
                   if (*(ptr+n*4)<11)
                   {
                     if (*(ptr+n*4)%2)
                       sprintf(label1,"IN&nbsp;&nbsp;%d",*(ptr+n*4)/2+1);
                     else
                       sprintf(label1,"OUT&nbsp;%d",*(ptr+n*4)/2+1);

                     if (*(ptr+n*4+2)%2)
                       sprintf(label2,"IN&nbsp;&nbsp;%d",*(ptr+n*4)/2+1);
                     else
                       sprintf(label2,"OUT&nbsp;%d",*(ptr+n*4)/2+1);
                   }
                   else
                   {
                      strcpy(label1,"IN");
                      strcpy(label2,"OUT");
                   }
                }
                else if (*(ptr+n*4)==21 || *(ptr+n*4)==22)
                {
                   //printf("slot=%d n=%d *(ptr+n*4)=%d *(ptr+n*4+1)=%d\n",slot,n,*(ptr+n*4),*(ptr+n*4+1));
                   char flag;
                   if (*(ptr+n*4)==21)
                       flag='A';
                   else
                       flag='D'; 
                   if (*(ptr+n*4+1)>29 && *(ptr+n*4+1)<50)
                      sprintf(label1,"%c/%d",flag,1270+(*(ptr+n*4+1)-30)*20);
                   else if (*(ptr+n*4+1)>49)
                   {
                     sprintf(label1,"%c/C%02d",flag,*(ptr+n*4+1)-50);
                     //printf("%s\n",label1);
                    }
                   else 
                      strcpy(label2,"N/A");
               }
              if (*(ptr+n*4+2)==21 || *(ptr+n*4+2)==22)
              {
                   char flag;
                   if (*(ptr+n*4+2)==21)
                       flag='A';
                   else
                       flag='D'; 
                   if (*(ptr+n*4+3)>29 && *(ptr+n*4+3)<50)
                      sprintf(label2,"%c/%d",flag,1270+(*(ptr+n*4+3)-30)*20);
                   else if (*(ptr+n*4+3)>49)
                      sprintf(label2,"%c/C%02d",flag,*(ptr+n*4+3)-50);
                   else
                       strcpy(label2,"N/A");
             }
            }
              nBytesSent +=websWrite(wp, T("<tr><td>%s</br>%s</td><td><img src='images/lcv1_2.png' height=\"30\" width='12' /></td></tr>"),label1,label2);
              }
          }
     break;
 }
nBytesSent +=websWrite(wp, T("</table>"));
return nBytesSent;
}

int aspNot4UPassiveUnitView(webs_t wp,uchar_t slot,uchar_t uclass,uchar_t utype)
{
  signed char ports_status[10]={2,2,2,2,2,2,2,2,2,2};

  char left_led_lbl[][6]={"PWR","TX","R1"};
  char right_led_lbl[][6]={"Pri","Auto","R2"};
  uchar_t *info;
  odmProperty_t *odm;
  
  char pText[10][10]={"IN","OUT","COM","COM Tx","COM Rx","Upg Tx","Upg Rx","PASS"," "};

  int bytes=0;
  char n;
  char port_lbls[10][2][12]={{"IN",""},{"OUT",""},{"COM",""},{"T1",""},{"R2",""},{"T2",""},{"PASS",""},{"Upg Tx",""}};
  //char port_lbls2[][12]={"","","","","","","",""};

  info=getUnitProperty(0,slot);
  if (NULL==info)
     return 0;
  odm=(odmProperty_t *)info;

  for (n=0;n<10;n++)
  {
   if (odm->leftWaveLength[n]<200 && odm->leftWaveLength[n]>0)
   {
    if (odm->systemType!=2)
          sprintf(port_lbls[n][0],"%d",1270+(odm->leftWaveLength[n]-1)*20);
     else
          sprintf(port_lbls[n][0], "C%d",odm->leftWaveLength[n]);
   }
   else if (odm->leftWaveLength[n]>199 && odm->leftWaveLength[n]<210)
           sprintf(port_lbls[n][0], "%s",pText[odm->leftWaveLength[n]-200]);
   else if (1==odm->leftWaveLength[n])
        strcpy(port_lbls[n][0], "N/A");
   else
        strcpy(port_lbls[n][0], " ");

   if (odm->rightWaveLength[n]<200 && odm->rightWaveLength[n]>0)
   {
    if (odm->systemType!=2)
          sprintf(port_lbls[n][1],"%d",1270+(odm->rightWaveLength[n]-1)*20);
     else
          sprintf(port_lbls[n][1], "C%d",odm->rightWaveLength[n]);
   }
   else if (odm->rightWaveLength[n]>199 && odm->rightWaveLength[n]<210)
           sprintf(port_lbls[n][1], "%s",pText[odm->rightWaveLength[n]-200]);
   else if (0==odm->rightWaveLength[n])
          strcpy(port_lbls[n][1], "N/A");
   else
       strcpy(port_lbls[n][1], " ");  
  }
  bytes+=showHUPorts(wp,slot,10,ports_status,port_lbls,2,2,"scv","png",25,15);
  return bytes;
}
int asp4UPassiveUnitView(webs_t wp,uchar_t slot,uchar_t uclass,uchar_t utype)
{
  char uname[8]="";
  int nBytesSent=0;
  char page[10];
  char n;
  getUnitLabelName(uclass,utype,uname);
  if (PASSIVE_OSU_UNIT==utype)
    strcpy(page,"osu");
  else if (PASSIVE_ODM_UNIT==utype)
    strcpy(page,"odm");
  else if (PASSIVE_ODM2_UNIT==utype)
    strcpy(page,"odm2");
  else if (PASSIVE_ODM12_UNIT==utype)
    strcpy(page,"odm12");
 // else if (PASSIVE_OADM_UNIT==utype)
 //   strcpy(page,"oadm");
  /*else if (PASSIVE_DCM1_UNIT==utype)
    strcpy(page,"dcm1");
  else if (PASSIVE_DCM4_UNIT==utype)
    strcpy(page,"dcm4");
  */
  else
    strcpy(page,"bi");
  //printf("asp4UPassiveUnitView:slot=%d utype=%d\r\n",slot,utype);
 if (PASSIVE_ODM2_UNIT==utype ||PASSIVE_DCM2_UNIT==utype)
 {
  nBytesSent +=websWrite(wp, T("<div class='ubd2'>"));
 }
 else if (PASSIVE_DCM4_UNIT==utype)
   nBytesSent +=websWrite(wp, T("<div class='ubd4'>"));
 else //if (PASSIVE_DCM1_UNIT==utype)
   nBytesSent +=websWrite(wp, T("<div class='ubd1'>"));


nBytesSent +=websWrite(wp, T("<div class='un'><a href='/goform/slot?cid=0&uid=%d&page=%s' target='mf'>%s</a></div>"),slot,page,uname);
nBytesSent +=writePassiveUnitView(wp,slot,utype,10,1);
if (PASSIVE_DCM4_UNIT==utype || PASSIVE_DCM1_UNIT==utype|| PASSIVE_DCM2_UNIT==utype)
{
  nBytesSent +=websWrite(wp, T("<div class='adpter' style='padding-top:65px;'>  <div class='pl'>IN</div> \
  <div class='pimg'><img src='images/fc-upc-2.gif'></div></div>"));
  nBytesSent +=websWrite(wp, T("<div class='adpter' style='padding-top:40px;'>  <div class='pl'>OTU</div> \
  <div class='pimg'><img src='images/fc-upc-2.gif'></div></div>"));
}
 nBytesSent +=websWrite(wp,T("</div>"));
 return nBytesSent;
}

int aspPassiveUnitView(webs_t wp,uchar_t slot,uchar_t cclass,uchar_t uclass,uchar_t utype)
{
  if (4!=cclass)
     return aspNot4UPassiveUnitView(wp, slot,uclass,utype);
   else 
     return asp4UPassiveUnitView(wp, slot,uclass,utype);
}

int aspPassiveInfo(int eid, webs_t wp, int argc, char_t **argv)
{
  char_t *slot,*info;
  uchar_t n,m,uclass, utype,count;
  int unit;
  int nBytes=0;
  unitViewInfo_t uvinfo;

  unit=getSelectedUnit();
  if (TRUE!=getUnitClassType(0,unit,&uclass,&utype))
    return 0;
  info=getUnitProperty(0,unit);
  if (NULL==info)
     return 0;
  if (uclass!=55)
     return 0;

  if (PASSIVE_OSU_UNIT==utype){
    osuProperty_t *osu=(osuProperty_t *)info;
    #ifdef DEFAULT_LANG_CN
    nBytes +=websWrite(wp, T("<tr><th>类型</th><td>"));
    if (1==osu->osu_type)
      nBytes +=websWrite(wp, T("FBT"));
    else
      nBytes +=websWrite(wp, T("PLC"));
    nBytes +=websWrite(wp, T("</td></tr>"));
   
    nBytes +=websWrite(wp, T("<tr><th>单多模</th><td>"));
    if (1==osu->mode)
      nBytes +=websWrite(wp, T("单模"));
    else
      nBytes +=websWrite(wp, T("多模"));
    nBytes +=websWrite(wp, T("</td></tr>"));
    
    nBytes +=websWrite(wp, T("<tr><th>带宽</th><td>"));
    if (osu->band_width<2 ||osu->band_width>4)
      nBytes +=websWrite(wp, T("单窗口"));
    else if(2==osu->band_width)
      nBytes +=websWrite(wp, T("双窗口"));
    else if(3==osu->band_width)
      nBytes +=websWrite(wp, T("三窗口"));
    else if(4==osu->band_width)
      nBytes +=websWrite(wp, T("全波段"));
    
    #else
         nBytes +=websWrite(wp, T("<tr><th>Type</th><td>"));
    if (1==osu->osu_type)
      nBytes +=websWrite(wp, T("FBT"));
    else
      nBytes +=websWrite(wp, T("PLC"));
    nBytes +=websWrite(wp, T("</td></tr>"));
   
    nBytes +=websWrite(wp, T("<tr><th>Mode</th><td>"));
    if (1==osu->mode)
      nBytes +=websWrite(wp, T("Single mode"));
    else
      nBytes +=websWrite(wp, T("multi mode"));
    nBytes +=websWrite(wp, T("</td></tr>"));
    
    nBytes +=websWrite(wp, T("<tr><th>Band width</th><td>"));
    if (osu->band_width<2 ||osu->band_width>4)
      nBytes +=websWrite(wp, T("Single window"));
    else if(2==osu->band_width)
      nBytes +=websWrite(wp, T("Double window"));
    else if(3==osu->band_width)
      nBytes +=websWrite(wp, T("Three window"));
    else if(4==osu->band_width)
      nBytes +=websWrite(wp, T("Full-wave band"));
    #endif
    nBytes +=websWrite(wp, T("</td></tr>"));

    count=osu->groups;
    if (count<1 || count>6)
        count=5;
    #ifdef DEFAULT_LANG_CN
       nBytes +=websWrite(wp, T("<tr><th>组数</th><td>%d组</td></tr>"),count);
    #else
       nBytes +=websWrite(wp, T("<tr><th>Group count</th><td>Group #%d</td></tr>"),count);
    #endif
    count=0;
    for(n=0;n<6;n++)
    {
       if (osu->split_rate[n][0]>0)
         count++;
    }

    for(n=0;n<count;n++)
    {
      if (osu->split_rate[n][0]>0)
      {
        if (count>1)
        {
          #ifdef DEFAULT_LANG_CN
          nBytes +=websWrite(wp, T("<tr><th>分光比%d</th><td>"),n+1);
          #else
            nBytes +=websWrite(wp, T("<tr><th>optical divide ratio:%d</th><td>"),n+1);
          #endif
        }
        else
        {
          #ifdef DEFAULT_LANG_CN
          nBytes +=websWrite(wp, T("<tr><th>分光比</th><td>"));
          #else
            nBytes +=websWrite(wp, T("<tr><th>optical divide ratio</th><td>"));
          #endif
        }
        for (m=1;m<5;m++)
        {
           if (osu->split_rate[n][m]>0 && osu->split_rate[n][m]<100)
             nBytes +=websWrite(wp, T("%d%% "),osu->split_rate[n][m]);
           else
             break;
        }
        #ifdef DEFAULT_LANG_CN
        nBytes +=websWrite(wp, T(":%d组</td></tr>"),osu->split_rate[n][0]);
        #else
         nBytes +=websWrite(wp, T(":Group #%d</td></tr>"),osu->split_rate[n][0]);
        #endif      
     }
    }
  }
  else if (PASSIVE_ODM_UNIT==utype){
    odmProperty_t *odm=(odmProperty_t *)info;
    
   //200:IN 201:OUT 202：COM，203：COM Tx，204：COM Rx,205: Upg Tx,206:Upg Rx，207:PASS
    char pText[10][8]={"IN","OUT","COM","COM Tx","COM Rx","Upg Tx","Upg Rx","PASS"};
    #ifdef DEFAULT_LANG_CN
    nBytes +=websWrite(wp, T("<tr><th>波分类型</th><td>"));
    if (odm->systemType==1)
        nBytes +=websWrite(wp, T("粗波分"));
    else if (odm->systemType==2)
        nBytes +=websWrite(wp, T("密波分"));
    else if (odm->systemType==3)
        nBytes +=websWrite(wp, T("混合波分"));
    nBytes +=websWrite(wp, T("</td></tr><tr><th>复用/解复用</th><td>"));
    if (odm->use==1)
        nBytes +=websWrite(wp, T("OM复用"));
    else if (odm->use==2)
        nBytes +=websWrite(wp, T("OD解复用"));
    else
        nBytes +=websWrite(wp, T("ODM复用/解复用"));
    nBytes +=websWrite(wp, T("</td></tr><tr><th>波道数</th><td>%d波</td></tr>"),odm->channelCount);
    nBytes +=websWrite(wp, T("<tr><th>传输类型</th><td>"));
    if (odm->direction==1)
        nBytes +=websWrite(wp, T("双纤双向"));
    else if (odm->direction==2)
        nBytes +=websWrite(wp, T("单纤双向"));
    else
        nBytes +=websWrite(wp, T("单纤单向"));
    nBytes +=websWrite(wp, T("</td></tr>"));
  
    nBytes +=websWrite(wp, T("<tr><th>升级端口</th><td>"));
    if (odm->upgrade!=1)
        nBytes +=websWrite(wp, T("无升级端口"));
    else
        nBytes +=websWrite(wp, T("有升级端口"));
    nBytes +=websWrite(wp, T("</td></tr><tr><th colspan='2'>接口信息</th></tr>"));
    #else
    nBytes +=websWrite(wp, T("<tr><th>C&DWDM</th><td>"));
    if (odm->systemType==1)
        nBytes +=websWrite(wp, T("CWDM"));
    else if (odm->systemType==2)
        nBytes +=websWrite(wp, T("DWDM"));
    else if (odm->systemType==3)
        nBytes +=websWrite(wp, T("C&DWDM"));
    nBytes +=websWrite(wp, T("</td></tr><tr><th>OD/OM</th><td>"));
    if (odm->use==1)
        nBytes +=websWrite(wp, T("OM multiplex"));
    else if (odm->use==2)
        nBytes +=websWrite(wp, T("OD de-multiplex"));
    else
        nBytes +=websWrite(wp, T("ODM multiplex/de-multiplex"));
    nBytes +=websWrite(wp, T("</td></tr><tr><th>Number of channels </th><td>%d</td></tr>"),odm->channelCount);
    nBytes +=websWrite(wp, T("<tr><th>Transmission type</th><td>"));
    if (odm->direction==1)
        nBytes +=websWrite(wp, T("two-fiber bi-directional "));
    else if (odm->direction==2)
        nBytes +=websWrite(wp, T("single fiber bi-directional"));
    else
        nBytes +=websWrite(wp, T("single fiber single-directional"));
    nBytes +=websWrite(wp, T("</td></tr>"));
  
    nBytes +=websWrite(wp, T("<tr><th>Has upgrade port?</th><td>"));
    if (odm->upgrade!=1)
        nBytes +=websWrite(wp, T("Not"));
    else
        nBytes +=websWrite(wp, T("Yes"));
    nBytes +=websWrite(wp, T("</td></tr><tr><th colspan='2'>Interface info</th></tr>"));
    #endif
    if (getUnitViewInfo(0,unit,&uvinfo)!=FALSE)
    { 
      for (count=0;count<10;count++)
        nBytes +=websWrite(wp, T("<tr><td>%s</td><td>%s</td></tr>"),uvinfo.ports.ports_text[0][count],uvinfo.ports.ports_text[1][count]);
      return nBytes;
    }
    /*
    for (count=0;count<10;count++)
    {
      
      nBytes +=websWrite(wp, T("<tr><td>"));
      if (odm->leftWaveLength[count]<200 && odm->leftWaveLength[count]>0)
      {
        if (odm->systemType!=2)
           nBytes +=websWrite(wp, T("%d"),1270+(odm->leftWaveLength[count]-1)*20);
        else
           nBytes +=websWrite(wp, T("C%d"),odm->leftWaveLength[count]);
      }
      else if (odm->leftWaveLength[count]>199 && odm->leftWaveLength[count]<210)
           nBytes +=websWrite(wp, T("%s"),pText[odm->leftWaveLength[count]-200]);
      else
          nBytes +=websWrite(wp, T("N/A"));
      nBytes +=websWrite(wp, "</td>");
     
      
      nBytes +=websWrite(wp, T("<td>"));
      if (odm->rightWaveLength[count]<200 && odm->rightWaveLength[count]>0)
      {
        if (odm->systemType!=2)
           nBytes +=websWrite(wp, T("%d"),1270+(odm->rightWaveLength[count]-1)*20);
        else
           nBytes +=websWrite(wp, T("C%d"),odm->rightWaveLength[count]);
      }
      else if (odm->rightWaveLength[count]>199 && odm->rightWaveLength[count]<210)
           nBytes +=websWrite(wp, T("%s"),pText[odm->rightWaveLength[count]-200]);
      else
          nBytes +=websWrite(wp, T("N/A"));
      nBytes +=websWrite(wp, "</td></tr>");
    }
    for (count=20;count<10;count++)
    {
      if (count==1)
      {
        if (0xff==odm->leftWaveLength[count] || 0==odm->leftWaveLength[count] || 0xff==odm->rightWaveLength[count])
          continue;
      }
      nBytes +=websWrite(wp, T("<tr><td>"));
      if (odm->systemType!=2)
      {
       if (odm->leftWaveLength[count]>0)
         nBytes +=websWrite(wp, T("%d"),1270+(odm->leftWaveLength[count]-1)*20);
       else
          nBytes +=websWrite(wp, T("N/A"));
       nBytes +=websWrite(wp, T("</td><td>"));
       if (odm->rightWaveLength[count]>0)
         nBytes +=websWrite(wp, T("%d</td>"),1270+(odm->rightWaveLength[count]-1)*20);
       else
          nBytes +=websWrite(wp, T("N/A</td>"));
      }
      else
      {
        if (odm->leftWaveLength[count]>0)
           nBytes +=websWrite(wp, T("C%d"),odm->leftWaveLength[count]);
        else
           nBytes +=websWrite(wp, T("N/A"));
        nBytes +=websWrite(wp, T("</td><td>"));
        if (odm->rightWaveLength[count]>0)
          nBytes +=websWrite(wp, T("C%d"),odm->rightWaveLength[count]);
        else
          nBytes +=websWrite(wp, T("N/A"));
        nBytes +=websWrite(wp, T("</td>"));
      }
     nBytes +=websWrite(wp, T("</tr>"));
     }
   */
  }
  return nBytes;
}


