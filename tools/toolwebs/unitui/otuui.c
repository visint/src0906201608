#include <sys/sysinfo.h>
#include <time.h>
#include "../wsIntrn.h"
#include "../../include/unit.h"
#include <visipc.h>
#include <vispaceio.h>
#include <sqlite3.h>
#include <dbutil.h>
#include <log.h>
#include <otu.h>
#include "../unitwebs.h"

boolean_t getOtuWebInfo(uchar_t chassis,uchar_t cclass,uchar_t slot,uchar_t uclass,uchar_t utype,web_unit_t *punit)
{
  punit->uclass=UC_OTU;
  punit->slotCount=1;
  punit->port_groups[0].isTxtAtTwoSide=0;
  strcpy(punit->port_groups[0].pclass,"imgw30 imgh20");
  strcpy(punit->port_groups[0].pimg,"sfp2_");
  if (utype==1)
    strcpy(punit->height,"180px");
  else
    strcpy(punit->height,"270px");
  
  /*if (utype==1)
   strcpy(punit->btext,"10G");
  else
   strcpy(punit->btext,"1.25G");
  */
  return TRUE;
}

char getOtuLedAndPortStatus(char slot,char_t otu_type,signed char leds_status[][4],signed char ports_status[])
{
  uchar_t n,m=0,uclass=0,utype=0;
  uchar_t port_count=0;
  if (TRUE!=getUnitClassType(0,slot,&uclass,&utype))
    return 0;
  
  memset(leds_status[0],-1,4);
  memset(leds_status[1],-1,4);
  memset(leds_status[2],-1,4);
  memset(ports_status,0,8);
  if (UC_OTU==uclass)
  {
    otuUnitStatus_t *pOtuStatus;
    pOtuStatus=getOtuStatus(0,slot);
    if (NULL==pOtuStatus)
        return 0;
    if (2==utype)
    {
      port_count=8;
      //leds_status[0][2]=-1;
    }
    else
    {
      port_count=4;
      leds_status[0][2]=0;
      //leds_status[0][2]=-1;
    }
    //leds_status[0][0]=-1;
    leds_status[0][1]=1;
    //leds_status[0][3]=-1;
    //printf("slot:%d,port_count:%d,uclass:%d,utype:%d\n",slot,port_count,uclass,utype);
    
    for (n=0;n<port_count;n++)
    {
      //printf("slot #%d portStatus[%d].on_module:%d tx_enable:%d port_status:%d\n",slot,n,pOtuStatus->portStatus[n].on_module,pOtuStatus->portStatus[n].tx_enable,pOtuStatus->portStatus[n].port_status);
      if (2!=pOtuStatus->portStatus[n].on_module)
      {
          if (pOtuStatus->portStatus[n].port_status==1)
              ports_status[n]=1;
          else
          {
            if (1!=pOtuStatus->portStatus[n].tx_enable)
            {
             ports_status[n]=3;
            }
            else
            {
             ports_status[n]=2;
            }
         }
        //printf("port #%d ports_status=%d\n",n+1,ports_status[n]);
      }
      else
          ports_status[n]=0;
   
      if (port_count>4)
      {
         //for (n=0;n<8;n++)
         {
            if (ports_status[n]!=1)
               leds_status[n%2+1][n/2]=0;
            else
               leds_status[n%2+1][n/2]=ports_status[n];
         }
         /*
         leds_status[1][0]=ports_status[0];
         leds_status[1][1]=ports_status[2];
         leds_status[1][2]=ports_status[4];
         leds_status[1][3]=ports_status[6];

         leds_status[2][0]=ports_status[1];
         leds_status[2][1]=ports_status[3];
         leds_status[2][2]=ports_status[5];
         leds_status[2][3]=ports_status[7];
         
         for (n=0;n<port_count;n++)
         {
            leds_status[n%d+1][m]=ports_status[n];
         }
         */
      }
      else
      {
         /*
         for (n=0;n<4;n++)
         {
            if (ports_status[n]!=1)
              leds_status[n/2+1][n%2+1]=0;
            else
              leds_status[n/2+1][n%2+1]=ports_status[n];
         }
         */
       
         if (ports_status[n]!=1)
             leds_status[n%2+1][n/2+1]=0;
         else
             leds_status[n%2+1][n/2+1]=1;
         /*
         if (ports_status[0]!=1)
            leds_status[1][1]=0;
         else
            leds_status[1][1]=1;

         if (ports_status[1]!=1)
            leds_status[2][1]=0;
         else
            leds_status[2][1]=1;

         if (ports_status[2]!=1)
            leds_status[1][2]=0;
         else
            leds_status[1][2]=1;

         if (ports_status[3]!=1)
            leds_status[2][2]=0;
         else
            leds_status[2][2]=1;
         */
       }
    }
  }
 /*
 if (port_count==8)
 { for (n=0;n<8;n++)
    printf("getOtuLedAndPortStatus: %d:%d\n",n,ports_status[n]);
 }
 */
  return port_count;
}

static int writeOtuPortView(webs_t wp,uchar_t uclass,int otu_type,int portCount,char port[8],otuPortProperty_t* portProperty)
{
  char n;
  int nBytesSent=0;
  char portsLabels[2][4][3]={{"L1","C1","L2","C2"},{"L3","C3","L4","C4"}};
  //printf("writeOtuPortView otu_type=%d\n",otu_type);
  if (UC_OEO==uclass)
  {
    for (n=0;n<4;n++)
    {
       sprintf(portsLabels[0][n],"C%d",n+1);
       sprintf(portsLabels[1][n],"C%d",n+5);
    }
 }
for (n=0;n<2;n++)
{
   nBytesSent +=websWrite(wp, T("<table class='sfpgr'><tr><td><table>"));
	nBytesSent +=websWrite(wp, T("<tr>\
	<td>%s RX<br/>%s TX</td>\
	</tr>\
	<tr>\
	<td>%s RX<br/>%s TX</td>\
	</tr>\
	<tr>\
	<td>%s RX<br/>%s TX</td>\
	</tr>\
	<tr>\
	<td>%s RX<br/>%s TX</td>\
	</tr>"),portsLabels[n][0],portsLabels[n][0],portsLabels[n][1],portsLabels[n][1],portsLabels[n][2],portsLabels[n][2],portsLabels[n][3],portsLabels[n][3]);
nBytesSent +=websWrite(wp, T("</table>\
	</td>\
<td>\
	<table border='2'>\
	<tr>\
	<td><img class='sfp' src='images/p2_%d.gif'></td>\
	</tr>\
	<tr>\
	<td><img class='sfp' src='images/p2_%d.gif'></td>\
	</tr>\
<tr>\
<td><img class='sfp' src='images/p2_%d.gif'></td>\
</tr>\
	<tr>\
<td><img class='sfp' src='images/p2_%d.gif'></td>\
</tr>\
	</table>\
</td>\
</tr>\
</table>\
  "),port[n*4],port[n*4+1],port[n*4+2],port[n*4+3]);
}
return nBytesSent;
}

static int asp4u_unit_port_view(webs_t wp,int slot,char port_count,char port_status[],char port_label_head[][8],char port_label[][2][8],char port_img[][8],char port_img_size[][2])
{
  int nBytes=0;
  char n,m;
  
  nBytes +=websWrite(wp, "<table class=\"ports4u\">");
  for (n=0;n<port_count;n++)
  {
    nBytes +=websWrite(wp, T("<tr><td>%s"),port_label[n][0]);
    if (strlen(port_label[n][1])>0)
      nBytes +=websWrite(wp, T("</br>%s"),port_label[n][1]);
    nBytes +=websWrite(wp, T("</td><td><img width=%d height=%d src='images/%s%d.gif'/></td></tr>"),port_img_size[n][0],port_img_size[n][1],port_img[n],port_status[n]);
  }
  nBytes +=websWrite(wp, "</table>");
}
static int asp4u_unit_head_view(webs_t wp,int slot,char *uname,char run_led,char *run_led_label,char row_count,char col_count,char leds[][4],char led_labels[][3][6])
{
  int nBytes=0;
  char n,m;
  
  nBytes=websWrite(wp, T("\n<div class='ubd1'>\
<div class='un'><a href='/goform/unit?cid=0&uid=%d' target='mf'>%s</a></div>\n"),slot,uname);
nBytes +=websWrite(wp, "<table class=\"leds4u\"><tr><td");
if (col_count>2)
  nBytes +=websWrite(wp, " colspan=\"2\"");
nBytes +=websWrite(wp, ">PWR</td><td><img src='images/led_1.png' /></td>");
if (run_led>-1 && run_led<4)
{
 nBytes +=websWrite(wp, T("<td><img src='images/led_%d.png' /></td><td"),run_led);
 if (col_count>2)
  nBytes +=websWrite(wp, " colspan=\"2\"");
  nBytes +=websWrite(wp, T(">%s</td>"),run_led_label);
}
nBytes +=websWrite(wp, "</tr>");
for (n=0;n<row_count;n++)
{
  nBytes=websWrite(wp, T("<tr><td>%s</td>"),led_labels[n][0]);
  for (m=0;m<col_count;m++)
  {
    nBytes=websWrite(wp, T("<td><img src='images/led_%d.png' /></td>"),leds[n][m]);
  }
  nBytes=websWrite(wp, T("<td>%s</td></tr>"),led_labels[n][1]);
}
nBytes=websWrite(wp, "</table>");
return nBytes;
/*
nBytes +=websWrite(wp, "<div class='vp'><span style='padding-left:10px;'>PWR<img style='margin-left:2px;' src='images/led_1.png' width='9px' /></span> </div>");
nBytes=websWrite(wp, "<table  style=\"width:60;font-size:9px;\"");
for (n=0;n<row_count;n++)
{
  nBytes=websWrite(wp, T("<tr><td>%s</td>"),led_labels[n][0]);
  for (m=0;m<col_count;m++)
  {
    nBytes=websWrite(wp, T("<td><img src='images/led_%d.png' /></td>"),leds[n][m]);
  }
  nBytes=websWrite(wp, T("<td>%s</td>"),led_labels[n][1]);
  nBytes=websWrite(wp, "</tr>");
}
nBytes=websWrite(wp, "</table>");
*/
}
static int asp4u_otu_10g_sfp_plus_view(webs_t wp,int slot,uchar_t uclass,uchar_t otu_type,char *property,char show_property)
{
  int nBytes=0,n;
  char *pOtup,uname[6]="OTU";
  char leds[4][4]={0};
  char col_count;
  char run_led=-1;
  char run_led_label[6]="";
  char led_labels[2][3][6]={{"L1","L4"},{"C1","C4"}};
  char port_status[]={0,0,0,0,0,0,0,0};
  char port_label_head[][8]={""};
  char port_label[][2][8]={{"L1 RX","L1 TX"},{"C1 RX","C1 TX"},{"L2 RX","L2 TX"},{"C2 RX","C2 TX"},{"L3 RX","L3 TX"},{"C3 RX","C3 TX"},{"L4 RX","L4 TX"},{"C4 RX","C4 TX"}};
  char port_img[][8]={"p2_","p2_","p2_","p2_","p2_","p2_","p2_","p2_"};
  char port_img_size[][2]={{20,22},{20,22},{20,22},{20,22},{20,22},{20,22},{20,22},{20,22},{20,22}};
  otuUnitStatus_t *pOtuStatus;
  uchar_t max_port=4,struct_type=0;
  if (property!=NULL)
  {
     //for (n=0;n<12;n++)
     // printf("D%d=%d ",n,*(property+n));
     max_port=*(property+9);
     if (max_port!=8)
        max_port=4;
     //printf("slot=#%d max_port=%d in asp4u_otu_10g_sfp_plus_view\n",slot,max_port);
  }
 //printf("Max_port=%d in asp4u_otu_10g_sfp_plus_view",max_port);
 if (max_port!=8)
 {
   col_count=2;
   led_labels[0][1][1]='2';
   led_labels[1][1][1]='2';
 }
 else
   col_count=4;
 //printf("col_count=%d\n",col_count);
 pOtuStatus=getOtuStatus(0,slot);
 if (NULL!=pOtuStatus)
 {
   for (n=0;n<max_port;n++)
    {
      if (2!=pOtuStatus->portStatus[n].on_module)
      {
          //printf("port=%d enable=%d status=%d\n",n+1,pOtuStatus->portStatus[n].tx_enable,pOtuStatus->portStatus[n].port_status);
          if (2!=pOtuStatus->portStatus[n].tx_enable)
          {
            if (n%2)
            {
              if (1!=pOtuStatus->portStatus[n].port_status)
                leds[1][(n+1)/2-1]=3;
              else
                leds[1][(n+1)/2-1]=1; 
            }
            else
            {
              if (1!=pOtuStatus->portStatus[n].port_status)
                leds[0][(n+1)/2]=3;
              else
                leds[0][(n+1)/2]=1; 
            }
            //port[n]=led[n];
            if (2==pOtuStatus->portStatus[n].port_status)
            {
                if (3==pOtuStatus->portStatus[n].tx_enable)
                    port_status[n]=3;
                else
                    port_status[n]=2;
            }
            else
               port_status[n]=1;//pOtuStatus->portStatus[n].port_status;
          }
          else
          {
           if (n%2)
             leds[1][n%4]=3;
           else
             leds[0][n%4]=3;
           //port[n]=3;
           port_status[n]=3;
          }
        //printf("port #%d enable=%d\n",n+1,pOtuStatus->portStatus[n].tx_enable);
      }
  }
 }
 if (UC_OEO==uclass && 7==otu_type)
 {
  strcpy(uname,"OEO");
  led_labels[0][0][0]='C';
  /*strcpy(led_labels[0][1],"C7");
  strcpy(led_labels[1][0],"C2");
  strcpy(led_labels[1][1],"C8");
  */
  
  sprintf(led_labels[0][1],"C%d",max_port-1);
  //sprintf(led_labels[1][0],"C%d",max_port/4+1);
  strcpy(led_labels[1][0],"C2");
  sprintf(led_labels[1][1],"C%d",max_port);
  /*
  if (max_port<8)
  {
    
    sprintf(led_labels[1][1],"C%d",max_port/4);
  }
  */

  for (n=0;n<max_port;n++)
  {
    sprintf(port_label[n][0],"C%d RX",n+1);
    sprintf(port_label[n][1],"C%d TX",n+1);
  }
 }
 nBytes=asp4u_unit_head_view(wp,slot,uname,run_led,run_led_label,2,col_count,leds,led_labels);
 nBytes+=asp4u_unit_port_view(wp,slot,max_port,port_status,port_label_head,port_label,port_img,port_img_size);
nBytes +=websWrite(wp, T("</div>"));
return nBytes;
/*s
   nBytes=websWrite(wp, T("\n<div class='ubd1'>\
<div class='un'><a href='/goform/unit?cid=0&uid=%d' target='mf'>OTU</a></div>\n\
<div class='leds'>\n"),slot);
nBytes +=websWrite(wp, "<div class='vp'><span style='padding-left:10px;'>PWR<img style='margin-left:2px;' src='images/led_1.png' /></span> </div>");
nBytes+=websWrite(wp, T("<div class='ll'>S1</div>\n\
	<div class='led'><img src='images/led_%d.png' /></div>\n\
	<div class='led'><img src='images/led_%d.png' /></div>\n\
	<div class='ll'>S4</div>\n\
	<div class='ll'>M1</div>\n\
	<div class='led'><img src='images/led_%d.png' /></div>\n\
	<div class='led'><img src='images/led_%d.png' /></div>\n\
	<div class='ll'>M4</div>\n\
	<div class='clr'></div>\n\
</div>\n"),led[0],led[2],led[1],led[3]);
return (nBytes);
*/
}

int asp4UOtuView(webs_t wp, int slot,uchar_t uclass,int otu_type,char show_property)
{
  int nBytesSent=0,n;
  
  char port[8]={0},led[8]={0},mode=1;
  char ledsLabels[4][3]={""};

  char portsLabels[8][8]={"L1","C1","L2","C2","L3","C3","L4","C4"};
  unitProperty_t  *pUnitProperty;
  unit_property_t *pup;
  otuUnitStatus_t *pOtuStatus;
  otuPortProperty_t *pPortProperty;
  char *pOtup;
  char uname[8]="OTU";
  char distance[30]="";
  char speed[30]="";
  char wave_len[30]="";
  uchar_t max_port=4,struct_type=0;

  pOtup=getUnitProperty(0,slot);

  /*
  if (pOtup!=NULL)
  {
      printf("Port count=%d\n",*(pOtup+11));
      for (n=0;n<12;n++)
           printf("D[%d]=%d\n",n,*(pOtup+n));
  }
  */
  //printf("slot:%d uclass:%d otu_type=%d\n max_port=%d\n",slot,uclass,otu_type,max_port);
  if (UC_OEO==uclass && OEO_TYPE_10G_SFP_PLUS_A==otu_type /*||UC_OTU==uclass && OTU_TYPE_10G_SFP_PLUS_A!=otu_type*/)
    return asp4u_otu_10g_sfp_plus_view(wp, slot,uclass,otu_type,pOtup,show_property);
  if (OTU_TYPE_2Dot5G_SFP_A==otu_type || 3==otu_type)
     max_port=8;
  else if (OTU_TYPE_10G_SFP_PLUS_A==otu_type)
  {
    if (pOtup!=NULL)
    {
     //for (n=0;n<12;n++)
     // printf("D%d=%d ",n,*(property+n));
       max_port=*(pOtup+9);
       if (max_port!=8)
          max_port=4;
    }
    //return asp4u_otu_10g_sfp_plus_view(wp, slot,pOtup,show_property);
  }
  else if (6==otu_type)
    max_port=8;
  //printf("slot:#%d uclass:%d otu_type=%d\n max_port=%d\n",slot,uclass,otu_type,max_port);
  //pOtuStatus=(otuUnitStatus_t*)getInfoSetValues(0,slot,OTU_STATUS_INFOSET_ID,1);
  pOtuStatus=getOtuStatus(0,slot);
  pPortProperty=(otuPortProperty_t*)getInfoSetValues(0,slot,OTU_SFP_PROPERTY_INFOSET_ID,1);
  
  if (NULL==pOtuStatus || NULL==pPortProperty)
     return websWrite(wp, T("<div class='ubd1'></div>"));
  if (UC_OTU==uclass)
  {
      strcpy(ledsLabels[0],"L1");
      strcpy(ledsLabels[1],"C1");
      if (max_port<5)
      {
       strcpy(ledsLabels[2],"L2");
       strcpy(ledsLabels[3],"C2");
      }
      else
      {
       strcpy(ledsLabels[2],"L4");
       strcpy(ledsLabels[3],"C4");
      }
    /*else
    {
      max_port=3;
      for (n=0;n<3;n++)
        sprintf(ledsLabels[n],"C%d",n+1);
    }
   */
  }
  else if (UC_OEO==uclass)
  {
    
    strcpy(uname,"OEO");
    strcpy(ledsLabels[0],"C1");
    strcpy(ledsLabels[1],"C2");
    if (max_port<5)
    {
     strcpy(ledsLabels[2],"C3");
     strcpy(ledsLabels[3],"C4");
    }
    else
    {
     strcpy(ledsLabels[2],"C7");
     strcpy(ledsLabels[3],"C8");
    }
    for (n=0;n<max_port;n++)
    {
        sprintf(portsLabels[n],"C%d",n+1);
    }
  }
  if (otu_type==OTU_TYPE_10G_SFP_PLUS_A)
  {
    pup=(unit_property_t *)pOtup;
    struct_type=pup->struct_type;
    if (NULL!=pup && (struct_type==1 ||struct_type==2))
    {
    strcpy(ledsLabels[0],"S1");
    strcpy(ledsLabels[1],"M1");
    if (max_port<5)
    {
     strcpy(ledsLabels[2],"S2");
     strcpy(ledsLabels[3],"M2");
    }
    else
    {
     strcpy(ledsLabels[2],"S4");
     strcpy(ledsLabels[3],"M4");
    }
  }
     //pUnitProperty=(unitProperty_t *)getUnitProperty(0,slot);
     
     //pup=(unit_property_t *)getUnitProperty(0,slot);
     //pup=(unit_property_t *)pOtup;
     if (NULL!=pup)
     {
        //struct_type=pup->struct_type;
        if (struct_type==1)
        {
         strcpy(portsLabels[0],"S1 RX");
         strcpy(portsLabels[1],"L1 TX");
         strcpy(portsLabels[2],"M1 RX");
         strcpy(portsLabels[3],"N/A");
         strcpy(portsLabels[4],"S2 RX");
         strcpy(portsLabels[5],"L2 TX");
         strcpy(portsLabels[6],"M2 RX");
         strcpy(portsLabels[7],"N/A");

         strcat(uname,"/TX");
        }
        else if (struct_type==2)
        {
         strcat(uname,"/RX");
         strcpy(portsLabels[0],"L1 RX");
         strcpy(portsLabels[1],"S1 TX");
         strcpy(portsLabels[2],"N/A");
         strcpy(portsLabels[3],"M1 TX");
         strcpy(portsLabels[4],"L2 RX");
         strcpy(portsLabels[5],"S2 TX");
         strcpy(portsLabels[6],"N/A");
         strcpy(portsLabels[7],"M2 TX");
        }
       
    }
  }
  if (NULL!=pOtuStatus)
  {
    if (1!=pOtuStatus->work_mode)
         mode=0;
    for (n=0;n<max_port;n++)
    {
      if (2!=pOtuStatus->portStatus[n].on_module)
      {
          if (2!=otu_type)
          {
            if (pOtuStatus->leds[n]!=2)
              port[n]=1;
          }
          else
          {
             if (pOtuStatus->portStatus[n].port_status!=2)
              port[n]=1;
          }
          //   if (pOtuStatus->portStatus[n].port_status!=2)
          //if (pOtuStatus->leds[n]!=2)
           //   port[n]=1;
          if (port[n]!=1)
          //else
          {
            if (1==pOtuStatus->portStatus[n].tx_enable)
            {
            //if (pOtuStatus->portStatus[n].port_status>=0 && pOtuStatus->portStatus[n].port_status<4)
             //  port[n]=pOtuStatus->portStatus[n].port_status;
            //else
              port[n]=2;
            }
            else
            {
            //led[n]=2;
            port[n]=3;
            }
         }
        //printf("port #%d enable=%d\n",n+1,pOtuStatus->portStatus[n].tx_enable);
      }
      else
      {
         port[n]=0;
      }
      if (2!=otu_type)
      {
            if (pOtuStatus->leds[n]!=2)
              led[n]=1;
      }
      else
      {
         if (pOtuStatus->portStatus[n].port_status!=2)
             led[n]=1;
      }
      /*
      if (2!=pOtuStatus->portStatus[n].on_module)
        led[n]=pOtuStatus->portStatus[n].port_status;
      else
        led[n]=0;
      */
      /*
      if (otu_type==OTU_TYPE_10G_SFP_PLUS_A)
      {
        if (2!=pOtuStatus->portStatus[n].on_module)
           led[n]=pOtuStatus->leds[n];
        else
          led[n]=0;
      }
      */
      if (led[n]!=1)
          led[n]=0;
    }
  }
  if (max_port<5)
  {
  nBytesSent +=websWrite(wp, T("\n<div class='ubd1'>\
<div class='un'><a href='/goform/unit?cid=0&uid=%d' target='mf'>%s</a></div>\n\
<div class='leds'>\n"),slot,uname);
	//<div class='ll'>PWR</div>\n\
	//<div class='led'><img src='images/led_1.png'/></div>\n"),slot,uname);
        if (otu_type==1)
	{
         nBytesSent +=websWrite(wp, T("<div class='ll'>PWR</div>\n\
	<div class='led'><img src='images/led_1.png'/></div>\n\
        <div class='led'><img src='images/led_%d.png' /></div>\n\
	<div class='ll'>OPM</div>\n"),mode);
        }
        else
            nBytesSent +=websWrite(wp, "<div class='vp'><span style='padding-left:10px;'>PWR<img style='margin-left:2px;' src='images/led_1.png' /></span> </div>");
	nBytesSent +=websWrite(wp, T("<div class='ll'>%s</div>\n\
	<div class='led'><img src='images/led_%d.png' /></div>\n\
	<div class='led'><img src='images/led_%d.png' /></div>\n\
	<div class='ll'>%s</div>\n\
	<div class='ll'>%s</div>\n\
	<div class='led'><img src='images/led_%d.png' /></div>\n\
	<div class='led'><img src='images/led_%d.png' /></div>\n\
	<div class='ll'>%s</div>\n\
	<div class='clr'></div>\n\
</div>\n"),ledsLabels[0],led[0],led[2],ledsLabels[2],ledsLabels[1],led[1],led[3],ledsLabels[3]);
}
else
{
 /*
  nBytesSent +=websWrite(wp, T("<div class='ubd1'>\
  <div class='un'><a href='/goform/unit?cid=0&uid=%d' target='mf'>%s</a></div>\
  <div class='leds'> \
<div class='vp'><span>PWR<img style='margin-left:2px;' src='images/led_1.png' /><img style='margin-right:2px;'src='images/led_%d.png' />RUN</span> </div>\
<div class='vp'><span>%s<img  src='images/led_%d.png' /><img src='images/led_%d.png' /><img src='images/led_%d.png' /><img src='images/led_%d.png' />%s</span></div>\
<div class='vp'><span>%s<img  src='images/led_%d.png' /><img src='images/led_%d.png' /><img src='images/led_%d.png' /><img src='images/led_%d.png' />%s</span></div>\
</div>"),slot,uname,mode,ledsLabels[0],led[0],led[2],led[4],led[6],ledsLabels[2],ledsLabels[1],led[1],led[3],led[5],led[7],ledsLabels[3]);
*/
nBytesSent +=websWrite(wp, T("<div class='ubd1'>\
  <div class='un'><a href='/goform/unit?cid=0&uid=%d' target='mf'>%s</a></div>\
  <div class='leds'> \
<div class='vp'><span>PWR<img style='margin-left:2px;' src='images/led_1.png' /></span> </div>\
<div class='vp'><span>%s<img  src='images/led_%d.png' /><img src='images/led_%d.png' /><img src='images/led_%d.png' /><img src='images/led_%d.png' />%s</span></div>\
<div class='vp'><span>%s<img  src='images/led_%d.png' /><img src='images/led_%d.png' /><img src='images/led_%d.png' /><img src='images/led_%d.png' />%s</span></div>\
</div>"),slot,uname,ledsLabels[0],led[0],led[2],led[4],led[6],ledsLabels[2],ledsLabels[1],led[1],led[3],led[5],led[7],ledsLabels[3]);
}

if (max_port<5)
{
 for (n=1;n<5;n++)
 {
  char line;
  if (n%2)
    line='L';
  else
    line='C';
  if (NULL!=pPortProperty)
  {
    if (port[n-1]>0)
    {
    //sprintf(distance,"传输距离:%dKM ",(int)(0.01*pPortProperty->distance));
    #ifdef DEFAULT_LANG_CN
      sprintf(distance,"传输距离:%.2ufKM ",0.01*pPortProperty->distance);
    #else
       sprintf(distance,"Distance:%.2ufKM ",0.01*pPortProperty->distance);
    #endif
    //sprintf(wave_len,"波长:%.2fnm ",0.05*pPortProperty->wave_lenght);
    if (pPortProperty->wave_lenght>100)
      #ifdef DEFAULT_LANG_CN
        sprintf(wave_len,"波长:%.2ufnm ",(0.05*pPortProperty->wave_lenght));
      #else
        sprintf(wave_len,"Wave length:%.2ufnm ",(0.05*pPortProperty->wave_lenght));
      #endif
    else
    {
      #ifdef DEFAULT_LANG_CN
      if (pPortProperty->wave_lenght>28)
       sprintf(wave_len,"波长:C%d",(pPortProperty->wave_lenght-20));
      else if (pPortProperty->wave_lenght>1)
        sprintf(wave_len,"波长:%unm",(1270+(pPortProperty->wave_lenght-10)*20));
      else
         strcpy(wave_len,"波长:850nm");
      #else
      if (pPortProperty->wave_lenght>28)
       sprintf(wave_len,"Wave length:C%d",(pPortProperty->wave_lenght-20));
      else if (pPortProperty->wave_lenght>1)
        sprintf(wave_len,"Wave length:%unm",(1270+(pPortProperty->wave_lenght-10)*20));
      else
         strcpy(wave_len,"Wave length:850nm");
      #endif
    }
    #ifdef DEFAULT_LANG_CN
       sprintf(speed,"当前速率:%.2fG",0.01*pPortProperty->current_speed);
    #else
       sprintf(speed,"Current speed:%.2fG",0.01*pPortProperty->current_speed);
    #endif
    //printf("%s:%d ; %s:%d\n",distance,pPortProperty->distance,wave_len,pPortProperty->wave_lenght);
    }
    else
    {
      distance[0]='\0';
      wave_len[0]='\0';
      speed[0]='\0';
    }
    pPortProperty++;
  }

 nBytesSent +=websWrite(wp, "<div class='port2'>\n<ul style='font-size:7px;'>\n");
if (struct_type==0)
  nBytesSent +=websWrite(wp, T("<li>%s RX</li>\n<li>%s TX</li></ul>"),portsLabels[n-1],portsLabels[n-1]);
else
   nBytesSent +=websWrite(wp, T("<li>%s</li>\n<li>%s</li>\n\
</ul>"),portsLabels[2*(n-1)],portsLabels[2*(n-1)+1]);
if (port[n-1]>0)
{
  //nBytesSent +=websWrite(wp, T("<div class='pimg'><img src='images/p2_%d.gif'"),port[n-1]);
  nBytesSent +=websWrite(wp, T("<div class='pimg'><a href='/goform/slot?cid=0&uid=%d&page=otuddm'><img src='images/p2_%d.gif'"),slot,port[n-1]);
  nBytesSent +=websWrite(wp, T(" title='%s&#10;%s&#10;%s'/></a>"),wave_len,distance,speed);
}
else
  nBytesSent +=websWrite(wp, T("<div class='pimg'><img src='images/p2_%d.gif'/>"),port[n-1]);
 nBytesSent +=websWrite(wp, T("</div></div>"));
}
}
else
  nBytesSent+=writeOtuPortView(wp,uclass,otu_type,8,port,pPortProperty);
nBytesSent +=websWrite(wp, T("</div>")); 
return nBytesSent;

/*
  nBytesSent +=websWrite(wp, T("<div class='panebd'> \
<div class='pt'><a href='/unit?cid=0&uid=%d' target='mf'>%OTU</a></div> \
<div class='pwr'> \
<span><div class='led'> \
<img src='images/led_1.png' > \
PWR</span>\
</div></div>"),slot);
*/
/*
 for (n=1;n<3;n++)
 {
    nBytesSent +=websWrite(wp, T("<div class='ts-1'>"));
     if (n<2)
     {
      nBytesSent +=websWrite(wp, T("<div class='ts-1-1'>OTU</div> \
	<div class='ts-1-2'>PWR1<img src='images/led_1.png'></div>"));
     }
     
     nBytesSent +=websWrite(wp, T("<div class='ts-1-4'><img src='images/p9_1.png'></div> \
	<div class='ts-1-5'> \
	<div class='ts-2'> \
	<div class='ts-2-3'>RX</div> \
	 <div class='ts-2-2'><img src='images/led_1.png'></div> \
		<div class='ts-2-3'>TXS</div> \
	      <div class='ts-2-2'><img src='images/led_1.png'></div> \
		<div class='gen-1'></div> \
		<div class='ts-2-3'>TX1</div> \
		<div class='ts-2-2'><img src='images/led_1.png'></div> \
		<div class='gen-1'></div> \
	 </div> \
	</div> \
		<div class='gen-1'></div> \
	</div>"));
 }

 if(show_property)
 {
   otuProperty_t *property;
    property=getOtuProperty(0,slot,1);
  if (property!=NULL)
   {
     nBytesSent +=websWrite(wp, T("<div class='up'><div>%dG</div><div>20km</div> \
<div>1470nm</div></div>"),property->channel_property[0].max_speed);
   }
 }
*/
 // nBytesSent +=websWrite(wp, T("</div>"));
  /*
  nBytesSent +=websWrite(wp, T("\n<div class='pane'>\n"));
  nBytesSent +=websWrite(wp, T("\n<table> \
  <tr> \
  <td align=center class='uc'><div class='ut'><a href='/unit?cid=0&uid=%d'>%OTU</a></div> \
  <div class='led'>PWR<img src='images/led_1.png'></div> \
  </td> \
  </tr>\n"),slot);
  for (n=1;n<3;n++)
  {
  nBytesSent +=websWrite(wp, T("\n<tr>\
  <td>\
  	<div class='ubd'>\
  	<table border='0'>\
  	 <tr>\
       <td class='port'><img src='images/p2_2.png'></td>\
       <td>\
        <table border='0' class='led'>\
           <tr>\
        <td>MOD</td>\
        <td><img src='images/led_1.png'></td>\
       </tr>\
        <tr>\
        <td>RX</td>\
        <td><img src='images/led_1.png'></td>\
       </tr>\
       <tr>\
        <td>TX</td>\
        <td><img src='images/led_1.png'></td>\
       </tr>\
  	   </table>\
       </td>\
      </tr>	\
  	</table>\
  	</div>\
  </td>\
  </tr>\n"));
   }
   */
/*
   nBytesSent +=websWrite(wp, T("<div class='pane'>\
<div class='un'><a href='/unit?cid=0&uid=%d'>OTU</a></div>\
<div class='leds'>\
 <div class='ll'>PWR</div>\
 <div class='il'><img src='images/led_1.png'></div>\
 <div class='rl'>ERR</div>\
 <div class='ri'><img src='images/led_2.png'></div>\
 <div class='ll'>L1</div>\
 <div class='il'><img src='images/led_1.png'></div>\
 <div class='rl'>L2</div>\
 <div class='ri'><img src='images/led_2.png'></div>\
 <div class='ll'>C1</div>\
 <div class='il'><img src='images/led_1.png'></div>\
 <div class='rl'>C2</div>\
 <div class='ri'><img src='images/led_2.png'></div>\
</div>"),slot);

for (n=1;n<5;n++)
{
 char line;
 if (n%2)
    line='L';
 else
    line='C';
 nBytesSent +=websWrite(wp, T("<div class='port'>\
<div class='pl'>RX<br/>TX<br/>%c%d</div><img src='images/p2_2.png'>\
</div>"),line,(n-1)%2+1);
}
nBytesSent +=websWrite(wp, T("</div>"));
return nBytesSent;
 if(show_property>200)
 {
   otuProperty_t *property;
    property=getOtuProperty(0,slot,1);
  if (property!=NULL)
   {
     nBytesSent +=websWrite(wp, T("<tr><td><div class='ui'><h5>%d</h5><h5>%dkm</h5><h5>%snm</h5></div></td></tr>"),property->channel_property[0].max_speed,property->channel_property[0].distance,property->channel_property[0].wave_length);
   }
 }
  nBytesSent +=websWrite(wp, T(" </table></div>\n"));
  return nBytesSent;
*/
}

int aspNot4UOtuView(webs_t wp, int slot,int otu_type,char show_property)
{
  uchar_t n,port_count;
  signed char leds_status[3][4]={0};
  signed char ports_status[8];

  char left_led_lbl[][6]={"PWR","L1","C1"};
  char right_led_lbl[][6]={"","L4","C4"};
  
  char port_lbls[8][2][12]={{"L1&nbsp;Rx","L1&nbsp;Tx"},{"C1&nbsp;Rx","C1&nbsp;Tx"},{"L2&nbsp;Rx","L2&nbsp;Tx"},{"C2&nbsp;Rx","C2&nbsp;Tx"},{"L3&nbsp;Rx","L3&nbsp;Tx"},{"C3&nbsp;Rx","C3&nbsp;Tx"},{"L4&nbsp;Rx","L4&nbsp;Tx"},{"C4&nbsp;Rx","C4&nbsp;Tx"}};
  char port_lbls2[][12]={"L1&nbsp;Tx","C1&nbsp;Tx","L2&nbsp;Tx","C2&nbsp;Tx","L3&nbsp;Tx","C3&nbsp;RTx","L4&nbsp;Tx","C4&nbsp;Tx"};
 
  int bytes=0;

  //port_count=getUnitLedAndPortStatus(slot,leds_status,ports_status);
  port_count=getOtuLedAndPortStatus(slot,otu_type,leds_status,ports_status);
  /*if (port_count>5)
  {
   for (n=0;n<8;n++)
    printf("%d:%d\n",n,ports_status[n]);
  }
  */
  if (port_count<5)
  {
    strcpy(right_led_lbl[0],"OPM");
    strcpy(right_led_lbl[1],"L2");
    strcpy(right_led_lbl[2],"C2");
  }
  /*for (n=0;n<1;n++)
  {
     printf("%d %d %d %d\n",leds_status[1][0],leds_status[1][1],leds_status[1][2],leds_status[1][3]);
     printf("%d %d %d %d\n\n",leds_status[2][0],leds_status[2][1],leds_status[2][2],leds_status[2][3]);
  }
  */
  //printf("port_count=%d in aspNot4UOtuView\n",port_count);
  if (port_count<1)
    return -1;
  bytes=showHLeds(wp,leds_status[0],leds_status[1],leds_status[2],left_led_lbl, right_led_lbl,4);
  //show4ColHLeds(slot,leds4col,left_lable,right_lable,3);
  bytes+=showHUPorts(wp,slot,port_count,ports_status,port_lbls,2,1,"sfph","png",25,15);
  return bytes;
}
int aspOtuView(webs_t wp, int slot,uchar_t cclass,uchar_t uclass,int otu_type,char show_property)
{
   if (4!=cclass)
     return aspNot4UOtuView(wp, slot,otu_type,show_property);
   else 
     return asp4UOtuView(wp, slot,uclass,otu_type,show_property);
}
static int writeOtuCrossArray(webs_t wp, int slot)
{
  int nBytesSent=0;
  //int pos[2][4]={0};
  char n,m,k,val,to;
  char pos[4]={0},wm[2];
  char pl;
  otuUnitStatus_t *os=getOtuStatus(0,slot);
  
  if (NULL==os)
  {
    return websWrite(wp, T("ERROR!"));
  }
  /*
  val=(os->reserve1&0XC0)>>6;
  printf("%d ",val);

  val=(os->reserve1&0X30)>>4;
  printf("%d ",val);
  
  val=(os->reserve1&0X0C)>>2;
  printf("%d ",val);
  
  val=os->reserve1&0X03;
  printf("%d\n",val);

  val=(os->reserve2&0XC0)>>6;
  printf("%d ",val);

  val=(os->reserve2&0X30)>>4;
  printf("%d ",val);
  
  val=(os->reserve2&0X0C)>>2;
  printf("%d ",val);
  
  val=os->reserve2&0X03;
  printf("%d\n",val);
  printf("reserve1=%0x reserve2=%0x\n",os->reserve1,os->reserve2);
 
  pos[0][0]=1;
  pos[0][1]=2;
  pos[0][2]=3;
  pos[0][3]=4;

  pos[1][0]=4;
  pos[1][1]=3;
  pos[1][2]=2;
  pos[1][3]=1;
  */
  wm[0]=os->work_mode;
  wm[1]=os->run_status;
  for (n=0;n<2;n++)
  {
    if (0==n)
      val=os->reserve1;
    else
      val=os->reserve2;
    pos[0]=(val & 0XC0)>>6;
    pos[1]=(val & 0X30)>>4;
    pos[2]=(val & 0X0c)>>2;
    pos[3]=(val & 0X03);
    //printf("pos[0]=%d pos[1]=%d pos[2]=%d pos[3]=%d\n",pos[0],pos[1],pos[2],pos[3]);
    #ifdef DEFAULT_LANG_CN
      nBytesSent+=websWrite(wp, T("<table class='dt'><tr><td colspan='6'>交叉矩阵%d</td></tr>"),n+1);
    #else
      nBytesSent+=websWrite(wp, T("<table class='dt'><tr><td colspan='6'>Cross array%d</td></tr>"),n+1);
    #endif
    //websWrite(wp, T("<td rowspan='5'>手动%d</td>"),wm[n]);
    for (m=0;m<4;m++)
    {
      for (k=0;k<4;k++)
      {
        if (pos[m]==k)
           break;
      }
      //if (k>3)
      //   pos[m]=m;
      if (m%2)
        pl='C';
      else
        pl='L';
      websWrite(wp,"<tr>");
      websWrite(wp, T("<td>%c%d RX</td>"),pl,n*2+m/2+1);

      for (k=0;k<4;k++)
      {
         websWrite(wp, T("<td><input type='radio' name='p%d_%d' value='%d'"),n+1,k+1,m+1);
         if (pos[k]==m)
           websWrite(wp, T(" checked=true"));
         websWrite(wp, T("/></td>"));
      }
      if (m==0 && wm[n]>=0 && wm[n]<3)
         #ifdef DEFAULT_LANG_CN
            websWrite(wp, "<td>工作模式</td>");
         #else
            websWrite(wp, "<td>Working mode</td>");
         #endif
      else if (m==1)
      {
         if (wm[n]>=0 && wm[n]<3)
         {
           websWrite(wp,T("<td rowspan='4' align='left'><input type='radio' name='wk%d' value=1"),n+1);
           if (wm[n]==1)
              websWrite(wp," checked");
           #ifdef DEFAULT_LANG_CN
              websWrite(wp,"/>手动模式<br/>");
           #else
              websWrite(wp,"/>Manual<br/>");
           #endif
           websWrite(wp,T("<input type='radio' name='wk%d' value=2"),n+1);
           if (wm[n]==2)
              websWrite(wp," checked");
           #ifdef DEFAULT_LANG_CN
              websWrite(wp,"/>自动保护模式</td>");
           #else
              websWrite(wp,"/>Auto protected</td>");
           #endif
         }
      }
      websWrite(wp, T("</tr>"));
    }
    //websWrite(wp, T("<tr><td></td><td><img src='images/harr.gif'/></td><td><img src='images/harr.gif'/></td><td><img src='images/harr.gif'/></td><td><img src='images/harr.gif'/></td></tr>"));
    websWrite(wp, T("<tr><td></td><td><img src='images/varr.gif'/><br/>L%d TX</td><td><img src='images/varr.gif'/><br/>C%d TX</td><td><img src='images/varr.gif'/><br/>L%d TX</td><td><img src='images/varr.gif'/><br/>C%d TX</td></tr>"),n*2+1,n*2+1,n*2+2,n*2+2);
    nBytesSent+=websWrite(wp, T("</table>"));
  }
  return nBytesSent;
}

static int writeOtuInfo(webs_t wp, int slot,int utype,int group)
{
  int nBytes=0;
  uchar_t n,port_count,count=0;
  unsigned short port_speed[8]={0,0,0,0};
  otuUnitStatus_t *pOtuStatus;
  otuPortProperty_t *pPortProperty;
  otuPortFEC_t *pOtuPortFEC;
  
  //printf("group=%d in writeOtuInfo\n",group);
  pOtuStatus=(otuUnitStatus_t *)getInfoSetValues(0,slot,OTU_STATUS_INFOSET_ID,1);
  if (NULL==pOtuStatus)
  {
    return websWrite(wp, "var uc=0;");
  }
  pPortProperty=(otuPortProperty_t*)getInfoSetValues(0,slot,OTU_SFP_PROPERTY_INFOSET_ID,1);
  if (NULL==pPortProperty)
   return websWrite(wp, "var uc=0;");
  if (!(utype==2 || utype==3))
  {
     pOtuPortFEC=(otuPortFEC_t*)getInfoSetValues(0,slot,OTU_SFP_FEC_INFOSET_ID,1);
  }
  else
     pOtuPortFEC=NULL;

  //printf("utype=%d on slot %d\n",utype,slot);
  if (2==utype||3==utype ||6==utype)
    port_count=8;
  else if (OTU_TYPE_10G_SFP_PLUS_A==utype || 7==utype)
  {
      char *pOtup;
      pOtup=getUnitProperty(0,slot);
      port_count=pOtup[9];
      //printf("max_port=%d on slot %d\n",port_count,slot);
      if (!(port_count==4 || port_count==8))
          port_count=4;
  }
  else
    port_count=4;
  nBytes=websWrite(wp, T("var slot=%d;port=%d,uc=%d,ut=%d,p=new Array();\n"),slot,getSelectedPort(),getUnitClass(0,slot),utype);
  for (n=1;n<=port_count;n++)
  {
    if (n==5)
       pPortProperty=(otuPortProperty_t*)getInfoSetValues(0,slot,OTU_SFP_PROPERTY_INFOSET_ID,2);
    if (pOtuStatus->portStatus[n-1].on_module==1)
    {
      if (group<1)
      {
        nBytes=websWrite(wp, T("p[%d]= new Object();\n"),count);
        nBytes=websWrite(wp, T("p[%d].pn=%d;p[%d].d=%d;p[%d].wl=%d;p[%d].speed=%d;p[%d].ws=%d;p[%d].ts=%d;"),count,n,count,pPortProperty->distance,count,pPortProperty->wave_lenght,count,pPortProperty->current_speed,count,pOtuStatus->portStatus[n-1].port_status,count,pOtuStatus->portStatus[n-1].tx_enable);
       if (NULL!=pOtuPortFEC)
       {
        //pOtuPortFEC=pOtuPortFEC+(n-1)*4;
        nBytes=websWrite(wp, T("p[%d].lb=%d;"),count,pOtuPortFEC->loopback);
       }
      }
      else
      {
       if(n==getSelectedPort())
       {
        //nBytes=websWrite(wp, "port= new Object();\n"));
        nBytes=websWrite(wp, T("p.pn=%d;p.d=%d;p.wl=%d;p.speed=%d;p.ws=%d;p.ts=%d;"),n,pPortProperty->distance,pPortProperty->wave_lenght,pPortProperty->current_speed,pOtuStatus->portStatus[n-1].port_status,pOtuStatus->portStatus[n-1].tx_enable);
        if (NULL!=pOtuPortFEC)
        {
         //pOtuPortFEC+=(n-1)*4;
         nBytes=websWrite(wp, T("p.lb=%d;"),pOtuPortFEC->loopback);
        }
        return nBytes;
       }
       /*
       if (NULL!=pOtuPortFEC)
         pOtuPortFEC++;
       */
     }
     count++;
    }
   pPortProperty++;
   if (NULL!=pOtuPortFEC)
     pOtuPortFEC++;
  }
  return nBytes;
}

static int writeOtuInfo1(webs_t wp, int slot,int utype,int group)
{
  int nBytesSent=0;//,rs=0,ts=0;
  //otuProperty_t *property;
  //otuChannelStatus_t *status;
  //char work_status[2][10]={"正常","不正常"};

  //char *values;
  unsigned short port_speed[8]={0,0,0,0};
  otuUnitStatus_t *pOtuStatus;
  
  pOtuStatus=(otuUnitStatus_t *)getInfoSetValues(0,slot,OTU_STATUS_INFOSET_ID,1);
  if (NULL!=pOtuStatus)
  {
    otuPortProperty_t *pPortProperty;
    otuPortFEC_t *pOtuPortFEC;

    pPortProperty=(otuPortProperty_t*)getInfoSetValues(0,slot,OTU_SFP_PROPERTY_INFOSET_ID,1);
    pOtuPortFEC=(otuPortFEC_t*)getInfoSetValues(0,slot,OTU_SFP_FEC_INFOSET_ID,1);
   // 工作模式尚未实现
   /*nBytesSent+=websWrite(wp, T("<form name='otumode' method='POST' action='/goform/olp'>工作模式:"));

    nBytesSent+=websWrite(wp, T("<input type='radio' id='wm' name='wm' value='1'"));
    if (1==pOtuStatus->work_mode)
    {
       nBytesSent+=websWrite(wp, T(" checked=true"));
    }
    nBytesSent+=websWrite(wp, T(">正常模式"));
    nBytesSent+=websWrite(wp, T("<input type='radio' name='wm' value='2'"));
    if (1!=pOtuStatus->work_mode)
    {
       nBytesSent+=websWrite(wp, T(" checked=true"));
    }
    nBytesSent+=websWrite(wp, T(">保护模式<br/><input class='button gray medium' type='button' value='修改' onclick=\"mod_otu('wm',%d,0,3);\"/></form>"),OTU_STATUS_INFOSET_ID);
  */
    if (NULL!=pPortProperty)
    {
       int n,m;
       char buf[256];
       char wave_len[12];
       uchar_t ch=1;
       //char distance[30];
       //unsigned short speed[5]={250,425,625,850,1000};
       //char speed_name[5][8]={"2.5G","4.25G","6.25G","8.5G","10G"};
       /*
1.	10G自适应 枚举值1000
2.	OTN:
OTU-1e:  枚举值1104
OTU-2:   枚举值1070
OTU-2e:  枚举值1109
3.	SDH/SONET:
STM-64/OC-192: 枚举值995
STM-64/OC-192+FEC: 枚举值1079
STM-16/OC-48: 枚举值249
4.	Ethernet
10GbE:  枚举值1031
2.5GbE: 枚举值250
5.	Fiber Channle
10GFC: 枚举值1051
10GFC+FEC: 枚举值1131
8GFC枚举值850
4GFC枚举值425
2GFC枚举值212
*/     
         
       //char group[][16]={"OTN","SDH/SONET","Ethernet","Fiber Channel"};
       //char group_idx[14]={-1,0,0,0,1,1,1,2,2,3,3,3,3,5};
       //unsigned short speed[14]={1000,1104,1070,1109,995,1079,249,1031,250,1051,1131,850,425,212};
       //char speed_name[14][18]={"10G自适应","OTU-1e","OTU-2","OTU-2e","STM-64/OC-192","STM-64/OC-192+FEC","STM-16/OC-48","10GbE","2.5GbE","10GFC","10GFC+FEC","8GFC","4GFC","2GFC"};

       //printf("utype=%d\n",utype);
       uchar_t max_port=4;
       if (2==utype ||6==utype)
          max_port=8;
       else if (OTU_TYPE_10G_SFP_PLUS_A==utype)
       {
          max_port=((char*)pPortProperty)[9];
          printf("max_port=%d on slot %d\n",max_port,slot);
          if (!(max_port==4 || max_port==8))
              max_port=4;
       }
       nBytesSent+=websWrite(wp, "<table align='center' class='dt'><tr>");
       //nBytesSent+=websWrite(wp, T("%s"),"<table align='center' class='dt'>
       #ifdef DEFAULT_LANG_CN
       nBytesSent+=websWrite(wp, T("%s"),  "\
            <th>光口</th>\
            <th>传输距离(KM)</th>\
            <th>波长(nm)</th>\
            <th>当前速率(G)</th>\
            <th>工作状态</th>\
            <th>发送端关断状态</th>"
       );
       #else
            nBytesSent+=websWrite(wp, T("%s"),  "\
            <th>Fiber port</th>\
            <th>Distance(KM)</th>\
            <th>Wave length(nm)</th>\
            <th>Curren speed(G)</th>\
            <th>Working status</th>\
            <th>TX is enable?</th>"
       );
       #endif
       if (1==utype ||5==utype||6==utype)
       {
          #ifdef DEFAULT_LANG_CN
             websWrite(wp, T("<th>自环</th>"));
          #else
            websWrite(wp, T("<th>Loop back</th>"));
          #endif
       }
       websWrite(wp, T("</tr>"));
       for (n=1;n<=max_port;n++)
       {
         if (5==utype || 6==utype)
         {
          if (n>1)
          {
            if (((n-1)/2)==(n-2)/2)
              ch=0;
            else
              ch=1;
          }
         }
         if (n==5)
             pPortProperty=(otuPortProperty_t*)getInfoSetValues(0,slot,OTU_SFP_PROPERTY_INFOSET_ID,2);
         if (pOtuStatus->portStatus[n-1].on_module==1)
         {
           //printf("wl=%d\n",pPortProperty->wave_lenght);
           if (pPortProperty->wave_lenght>100)
            sprintf(wave_len,"%.2f",0.05*pPortProperty->wave_lenght);
           else
           {
             if (pPortProperty->wave_lenght>29)
                sprintf(wave_len,"C%d",pPortProperty->wave_lenght-20);
             else
             {
                if (pPortProperty->wave_lenght>1)
                 sprintf(wave_len,"%d",1270+(pPortProperty->wave_lenght-10)*20);
                else if (pPortProperty->wave_lenght==1)
                 strcpy(wave_len,"850");
             }
           }
           sprintf(buf,"<tr><td>%d</td> <td>%.2f</td> <td>%s</td>",n,(0.01*pPortProperty->distance),wave_len);
           nBytesSent+=websWrite(wp, T("%s"),buf);
           if (!(ch<1 && (utype==5 ||utype==6)))
           {
            nBytesSent+=websWrite(wp, "<td");
              if (utype==5 ||utype==6)
               nBytesSent+=websWrite(wp, " rowspan='2'");
           nBytesSent+=websWrite(wp, T("><form name='speed' method='POST' action='/goform/otu' onsubmit='return app(1);'>"));
          if (2!=utype)
          {
           port_speed[n-1]=pPortProperty->current_speed;
           nBytesSent+=websWrite(wp, T("<select id='s%d' class='chosen-select'>"),n);
           if (1==utype)
   nBytesSent+=websWrite(wp,"<optgroup label='OTN'>\n\
   <option value='1104'>OTU-1e</option>\n\
   <option value='1070'>OTU-2</option>\n\
   <option value='1109'>OTU-2e</option>\n\
  </optgroup>\n\
  <optgroup label='SDH/SONET'>\n\
    <option value='995'>STM-64/OC-192</option>\n\
    <option value='1079'>STM-64/OC-192+FEC</option>\n\
    <option value='249'>STM-16/OC-48</option>\n\
  </optgroup>\n\
  <optgroup label='Ethernet'>\n\
    <option value='1031'>10GbE</option>\n\
    <option value='250'>2.5GbE</option>\n\
 </optgroup>\n\
 <optgroup label='Fiber Channel'>\n\
    <option value='1051'>10GFC</option>\n\
    <option value='1131'>10GFC+FEC</option>\n\
    <option value='850'>8GFC</option>\n\
    <option value='425'>4GFC</option>\n\
    <option value='212'>2GFC</option>\n\
  </optgroup>\n");
  //<option value='1000'>10G速率扫描</option>\n");
 else if (5==utype || 6==utype)
 {
   nBytesSent+=websWrite(wp,"<option value='850'");
   if (port_speed[n-1]==850)
     nBytesSent+=websWrite(wp," selected");
   nBytesSent+=websWrite(wp,">1G~9.95Gpbs</option><option value='1000'");
   if (port_speed[n-1]==1000)
     nBytesSent+=websWrite(wp," selected");
   nBytesSent+=websWrite(wp,">9.95G~11.3Gpbs</option>");
 }
nBytesSent+=websWrite(wp,"</select>");

           /*for (m=50;m<14;m++)
           {
             //printf("m=%d speed=%d\n",m,pPortProperty->current_speed);
             if (group_idx[m]>-1 && grp_idx!=group_idx[m])
             {
               nBytesSent+=websWrite(wp, "</optgroup>");
             }
             if (group_idx[m]>-1 && grp_idx!=group_idx[m])
             {
               nBytesSent+=websWrite(wp, T("<optgroup label=\"%s\">"),group[grp_idx]);
             }
             nBytesSent+=websWrite(wp, T("<option value='%d'"),speed[m]);
             if (speed[m]==pPortProperty->current_speed)
                nBytesSent+=websWrite(wp, T(" selected='selected'"));
             nBytesSent+=websWrite(wp, T(">%s"),speed_name[m]);
             
           }
          */
           if (utype==1)
           {
           #ifdef DEFAULT_LANG_CN
             nBytesSent+=websWrite(wp, T("<a href='#'><img src='../images/pen.gif' title='10G速率扫描' onclick=\"location.href='/goform/otu?cid=0&uid=%d&port=%d&isid=%d&iid=4&val=1000&page=oturet'\"/></a>"),slot,n,OTU_SFP_PROPERTY_INFOSET_ID);
            #else
              nBytesSent+=websWrite(wp, T("<a href='#'><img src='../images/pen.gif' title='10G Speed scan' onclick=\"location.href='/goform/otu?cid=0&uid=%d&port=%d&isid=%d&iid=4&val=1000&page=oturet'\"/></a>"),slot,n,OTU_SFP_PROPERTY_INFOSET_ID);
            #endif
           }
           nBytesSent+=websWrite(wp, T("<a href='#'><img src='../images/pen.gif' title='Set speed' onclick=\"mod_otu('s%d',%d,%d,4);\"/></a>"),n,OTU_SFP_PROPERTY_INFOSET_ID,n);
           /*if (utype==1)
             nBytesSent+=websWrite(wp, T("<a href='#'><img src='../images/pen.gif' title='10G速率扫描' onclick=\"location.href='/goform/otu?cid=0&uid=%d&port=%d&isid=%d&iid=4&val=1000&page=oturet'\"/></a>"),slot,n,OTU_SFP_PROPERTY_INFOSET_ID);
           nBytesSent+=websWrite(wp, T("<a href='#'><img src='../images/pen.gif' title='设置速率' onclick=\"mod_otu('s%d',%d,%d,4);\"/></a>"),n,OTU_SFP_PROPERTY_INFOSET_ID,n);
             */
           
         }
         else
         {
            char cspeed[20];
            sprintf(cspeed,"%.2f",0.01*pPortProperty->current_speed);
            nBytesSent+=websWrite(wp, T("%s"),cspeed);
         }
         if (!(ch<1 && (utype==5 ||utype==6)))
         nBytesSent+=websWrite(wp, T("</form></td>"));
       }
         //nBytesSent+=websWrite(wp, T("</select><a href='#'><img src='../images/pen.gif' onclick=\"mod_otu('s%d',%d,%d,4);\"/></a></form></td>"),n,OTU_SFP_PROPERTY_INFOSET_ID,n);
           
           #ifdef DEFAULT_LANG_CN
            if (1!=pOtuStatus->portStatus[n-1].port_status)
              nBytesSent+=websWrite(wp, T("<td>不正常</td>"));
            else
              nBytesSent+=websWrite(wp, T("<td>正常</td>"));
           #else
               if (1!=pOtuStatus->portStatus[n-1].port_status)
                  nBytesSent+=websWrite(wp, T("<td>Unnormal</td>"));
               else
                  nBytesSent+=websWrite(wp, T("<td>Normal</td>"));
           #endif
           nBytesSent+=websWrite(wp, T("<td><form name='tx_en' method='POST' action='/goform/olp' onsubmit='return app(1);'><select id='txen%d'> <option value='1'"),n);
           if (1==pOtuStatus->portStatus[n-1].tx_enable)
             nBytesSent+=websWrite(wp, T(" selected='selected'"));
           #ifdef DEFAULT_LANG_CN
              nBytesSent+=websWrite(wp, T(">启用"));
           #else
              nBytesSent+=websWrite(wp, T(">Enable"));
           #endif
           nBytesSent+=websWrite(wp, T("<option value='2'"));
           if (2==pOtuStatus->portStatus[n-1].tx_enable)
             nBytesSent+=websWrite(wp, T(" selected='selected'"));
           #ifdef DEFAULT_LANG_CN
              nBytesSent+=websWrite(wp, T(">禁用"));
           #else
              nBytesSent+=websWrite(wp, T(">Disable"));
           #endif
          
           nBytesSent+=websWrite(wp, T("<option value='3'"));
           if (3==pOtuStatus->portStatus[n-1].tx_enable)
             nBytesSent+=websWrite(wp, T(" selected='selected'"));
           #ifdef DEFAULT_LANG_CN
           nBytesSent+=websWrite(wp, T(">接收无光自动关闭</select><a href='#'><img src='../images/pen.gif' title='设置发送端关断' onclick=\"mod_otu('txen%d',%d,%d,%d);\"/></a></form></td>"),n,OTU_STATUS_INFOSET_ID,n,4+n*3);
           #else
                nBytesSent+=websWrite(wp, T(">Auto disable when rx is LOS</select><a href='#'><img src='../images/pen.gif' title='Set TX enable' onclick=\"mod_otu('txen%d',%d,%d,%d);\"/></a></form></td>"),n,OTU_STATUS_INFOSET_ID,n,4+n*3);
           #endif
           if (utype!=2 && NULL!=pOtuPortFEC)
           {
             if (!(ch<1 && (utype==5 ||utype==6)))
             {
             nBytesSent+=websWrite(wp, "<td");
              if (utype==5 ||utype==6)
               nBytesSent+=websWrite(wp, " rowspan='2'");
             nBytesSent+=websWrite(wp, T("><form name='lb' method='POST' action='/goform/olp' onsubmit='return app(1);'><select id='lb%d'> <option value='1'"),n);
           if (1==pOtuPortFEC->loopback)
             nBytesSent+=websWrite(wp, T(" selected='selected'"));
           #ifdef DEFAULT_LANG_CN
           nBytesSent+=websWrite(wp, T(">自环"));
           #else
            nBytesSent+=websWrite(wp, T(">Loop back"));
           #endif
           nBytesSent+=websWrite(wp, T("<option value='2'"));
           if (1!=pOtuPortFEC->loopback)
             nBytesSent+=websWrite(wp, T(" selected='selected'"));
           #ifdef DEFAULT_LANG_CN
           nBytesSent+=websWrite(wp, T(">非自环</select><a href='#'><img src='../images/pen.gif' title='设置自环' onclick=\"mod_otu('lb%d',%d,%d,%d);\"/></a></form></td>"),n,OTU_SFP_FEC_INFOSET_ID,n,4*n-3);
           #else
                nBytesSent+=websWrite(wp, T(">No loopback</select><a href='#'><img src='../images/pen.gif' title='Set loop back' onclick=\"mod_otu('lb%d',%d,%d,%d);\"/></a></form></td>"),n,OTU_SFP_FEC_INFOSET_ID,n,4*n-3);
           #endif
            }
           }
           nBytesSent+=websWrite(wp, T("</tr>"));
         }
         pPortProperty++;
         if (NULL!=pOtuPortFEC)
         pOtuPortFEC++;
       }
      nBytesSent+=websWrite(wp, T("</table>"));
    }
  }
  if (1==utype)
  {
    nBytesSent+=websWrite(wp, "<script language=\"javascript\">\n");
    nBytesSent+=websWrite(wp, T("var s= new Array(%d,%d,%d,%d);\nvar p;\n"),port_speed[0],port_speed[1],port_speed[2],port_speed[3]);
    if (1==utype)
    nBytesSent+=websWrite(wp, "for(var i=1;i<5;i++){\n\
      p=document.getElementById('s'+i);\n\
      if (p)\n\
      {\n\
         for(var j=0;j<p.options.length;j++){\n\
           if(p.options[j].value==s[i-1])\n\
           {\n\
              p.options[j].selected = true;\n\
              break;\n\
           }\n\
         }\n\
      }\n\
    }\n\
");
    nBytesSent+=websWrite(wp, "</script>\n");
  }
  return nBytesSent;
/*
  property=getOtuProperty(0,slot,1);
  if (property==NULL)
  {
     return 0;
  }
  
  status=getOtuChannelStatus(0,slot,1);
  if (status==NULL)
   {
     return nBytesSent+=websWrite(wp, T("<div class=cbox>Unit #%d is NOT OTU or not exsits!</div>"),slot);
   }

  if (status->port_status[0].rx_status!=1)
     rs=1;
  if (status->port_status[0].tx_status!=1)
     ts=1;
   nBytesSent+=websWrite(wp, T("<table align='center' class='dt'><tr>\
          <tr><thead><th colspan='6' align='center'>OTU 通道1信息</th></thead></tr>\
          <tr>\
            <th>传输距离</th>\
            <td>%dKM</td>\
            <th>速率</th>\
            <td>%dG</td>\
            <th>波长</th>\
            <td>%s</td>\
         </tr>\
          <th>光口1接收状态</th><td>%s</td><th>光口1发送状态</th><td>%s</td><th>光口1关断状态</th><td>启用</td></tr>"),property->channel_property[0].distance,property->channel_property[0].max_speed/100,property->channel_property[0].wave_length,power[rs],power[ts]);
     if (status->port_status[1].rx_status!=1)
       rs=1;
     if (status->port_status[1].tx_status!=1)
       ts=1;
         nBytesSent+=websWrite(wp, T(" <tr><th>光口2接收状态</th><td>%s</td><th>光口2发送状态</th><td>%s</td><th>光口2关断状态</th><td>禁用</td></tr>\
         </table>"),power[rs],power[ts]);
*/
   /*
   nBytesSent+=websWrite(wp, T("<table align='center' class='dt'><tr> \
    <tr><thead><th colspan='4' align='center'>OTU 信号</th></thead></tr> \
    <th>光口1接收状态</th> \
    <td>%s</td> \
    <th>光口1发送状态</th> \
    <td>%s</td> \
    </tr>"),power[rs],power[ts]);
 
     if (status->port_status[1].rx_status!=1)
       rs=1;
     if (status->port_status[1].tx_status!=1)
       ts=1;
    nBytesSent+=websWrite(wp, T("<tr> \
    <th>光口2接收状态</th> \
    <td>%s</td> \
    <th>光口2发送状态</th> \
    <td>%s</td> \
    </tr>"),power[rs],power[ts]);
     nBytesSent+=websWrite(wp, T("<tr> \
    <th>传输距离</th> \
    <td>%d</td> \
    <th>波长</th> \
    <td>%s</td> \
    </tr></table>"),property->channel_property[0].distance,property->channel_property[0].wave_length); 
*/
  return nBytesSent;
}
int aspOtuInfo(webs_t wp, int slot,int uinfo,int utype,int group)
{
  if (1==uinfo)
     return writeOtuInfo(wp, slot,utype,group);
  else if (2==utype && 2==uinfo)
     return writeOtuCrossArray(wp,slot);
  else
     return 0;
}
static void formOtu(webs_t wp, char_t *path, char_t *query)
{
  char_t *pSlot,*pPort,*pInfoset_id,*pItem,*pVal,*page;
  //char set_cmd[50];
  char ret_page[20];
  int slot,port,infoset_id,item_id,value;
  setInfoSet_t infoset;
  uchar_t uclass;
  
  page = websGetVar(wp, T("page"), T(""));
  sprintf(ret_page,"%s.asp",page);
  pSlot = websGetVar(wp, T("uid"), T("0"));
  
  slot=atoi(pSlot);
  if (slot<1)
  {
    websRedirect(wp,ret_page);
    return;
  }
  uclass=getUnitClass(0,(uchar_t)slot);
  pPort = websGetVar(wp, T("port"), T("0"));
  port=atoi(pPort);
  pInfoset_id= websGetVar(wp, T("isid"), T("0"));
  infoset_id=atoi(pInfoset_id);
  pItem= websGetVar(wp, T("iid"), T("0"));
  item_id=atoi(pItem);
  pVal= websGetVar(wp, T("val"), T("0"));
  value= 0xffff & atoi(pVal);
  //printf("slot=%d port=%d infoset_id=%d item_id=%d value=%d\n",slot,port,infoset_id,item_id,value);
  switch (infoset_id)
  {
     case OTU_SFP_PROPERTY_INFOSET_ID:
          if (item_id % 4==0)
             setOtuPortSpeed(0,slot,port,(unsigned short)(value & 0xfff));
          break;
     case OTU_STATUS_INFOSET_ID:
          if (3==item_id)
            setOtuWorMode(0,slot,value);
          else if ((item_id-1)%3==0)
            setOtuPortTx(0,slot,port,value & 0xff);
          break;
     case OTU_SFP_FEC_INFOSET_ID:
          if (item_id%4==1)
            setOtuLoopBackAndFec(0,slot,port,value & 0xff,0);
          else if (item_id%4==2)
            setOtuLoopBackAndFec(0,slot,port,0,value & 0xff);
          break;
  }
  if (UC_OTU==uclass)
   insertOperateLog(LOG_UNIT_OTU_CONF,0,0,WebMode,mod,slot,port);
  else if (UC_OEO==uclass)
   insertOperateLog(LOG_UNIT_OEO_CONF,0,0,WebMode,mod,slot,port);
  //printf("formOtu ok! ret_page=%s\n",ret_page);
  websRedirect(wp,ret_page);
}

static void formOtuConf(webs_t wp, char_t *path, char_t *query)
{
  char_t *page,*act,*slot,*group,*pvalue;
  char pagefile[20];
  char item[20]="";
  int unit,index;
  char n,m;
  short ca[2]={0,0},wk[2]={0,0},value;
  //printf("formOtuConf\n");
  slot = websGetVar(wp, T("uid"), T("0"));
  act  = websGetVar(wp, T("act"), T("0"));
  page  = websGetVar(wp, T("page"), T("otu"));
  group=websGetVar(wp, T("group"), T("1"));
  sprintf(pagefile,"%s.asp",page);
  unit=atoi(slot);
  if (unit>0)
  {
    char message[512]={0};
    message[0]='S';
    message[1]=OTU_STATUS_INFOSET_ID;
    message[2]=1;
    for (n=1;n<3;n++)
    {
      for (m=1;m<5;m++)
      {
        sprintf(item,"p%d_%d",n,m);
        pvalue= websGetVar(wp, T(item), T("0"));
        value=0x3 & (atoi(pvalue)-1);
        value=value<<(8-m*2);
        ca[n-1] |=value;
        //printf("item=%s %d=%s ",item,m,value);
      }
      sprintf(item,"wk%d",n);
      pvalue= websGetVar(wp, T(item), T("1"));
      wk[n-1]=0xff & atoi(pvalue);
  
      //message[2+2*n-1]=n;
      //message[2+2*n+1]=ca[n-1] & 0XFF;
      //printf("ca%d value=%0x\n",n,message[2+2*n+1]);
    }
    message[3]=1;
    message[5]=ca[0] & 0XFF;
    message[6]=2;
    message[8]=ca[1] & 0XFF;
    message[9]=3;
    message[11]=wk[0];
    message[12]=4;
    message[14]=wk[1];
    sendMessageToUnit(0,unit,message,3000,1,0,0);
    sleep(2);
  }
  //printf("%s\n",pagefile);
  websRedirect(wp,pagefile);
}
void otuAspDefine()
{
 //websAspDefine(T("OlpInfo"), aspOlpInfo);
 websFormDefine(T("otu"), formOtu);
 websFormDefine(T("otuconf"), formOtuConf);
}
