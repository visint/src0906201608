#include <string.h>
#include <sys/sysinfo.h>
#include <time.h>
#include "../wsIntrn.h"

#include <unit.h>
#include <visipc.h>
#include <sqlite3.h>
#include <dbutil.h>
#include <log.h>
#include <chassis.h>
#include <passive.h>
#include <sysinfo.h>
#include <otu.h>

int show2ColHLeds(webs_t wp,char slot,char leds[][2],char left_lable[][6], char right_lable[][6],char row_count)
{
 char n,m;
 int nBytes=0;
 for (n=0;n<2;n++)
 {
   if (n==0)
   {
     for (m=row_count-1;m>-1;m--)
        nBytes+=websWrite(wp, T("%s "),left_lable[m]);
     //printf("\n");
   }
   for (m=row_count-1;m>-1;m--)
   {
     nBytes+=websWrite(wp, T(" %d "),leds[m][n]);
   }
   if (n==1)
   {
     //printf("\n");
     for (m=row_count-1;m>-1;m--)
        nBytes+=websWrite(wp, T("%s "),right_lable[m]);
     //printf("\n");
   }
    //printf("\n");
 }
 return nBytes;
}


int show4ColHLeds(webs_t wp,char slot,char leds[][4],char left_lable[][6], char right_lable[][6],char row_count)
{
 char n,m;
 int nBytes;
 nBytes=websWrite(wp,"<div class='leds'>\n<table border='0'>\n");
 for (n=0;n<4;n++)
 {
   if (n==0)
   {
     nBytes+=websWrite(wp, T("<tr>\n"));
     for (m=row_count;m>0;m--)
     {
      nBytes+=websWrite(wp, T("<td><div class='pl'><span class='plabel'>%s</span></div></td>\n"),left_lable[m-1]);
     }
     nBytes+=websWrite(wp, T("</tr>\n"));
   }
   nBytes+=websWrite(wp, T("<tr>\n"));
   for (m=row_count;m>0;m--)
   {
     nBytes+=websWrite(wp, T("<td>\n"));
     if (leds[m-1][n]>-1 && leds[m][n]<4)
       nBytes+=websWrite(wp, T("<img src='led_%d.png'/>"),leds[m-1][n]);
     nBytes+=websWrite(wp, T("</td>\n"));
   }
   nBytes+=websWrite(wp, T("</tr>\n"));
   if (n==3)
   {
     nBytes+=websWrite(wp, T("<tr>\n"));
     for (m=row_count;m>0;m--)
        nBytes+=websWrite(wp, T("<td><div class='pl'><span class='plabel'>%s</span></div></td>\n"),right_lable[m-1]);
     nBytes+=websWrite(wp, T("</tr>\n"));
   }
 }
 nBytes+=websWrite(wp, T("</table></div>\n"));
 return nBytes;
}

int showHLeds(webs_t wp,
signed char *leds1,
signed char *leds2,
signed char *leds3,char tlable[][6], char blable[][6],char col_count)
{
 char n,m;
 int nBytes;

 nBytes=websWrite(wp,"<div class='leds'>\n<table border='0'>\n<tr>");
 for (n=3;n>0;n--)
  nBytes=websWrite(wp,T("<td><div class='pl'><span class='plabel'>%s</span></div></td>"),tlable[n-1]);
  //nBytes=websWrite(wp,T("<td><div class='pl'><span class='plabel'>%s</span></div></td>\n</tr>\n"),tlable[0]);
 for (n=0;n<4;n++)
 {
   nBytes=websWrite(wp,"<tr>\n<td>");
   if (leds3[n]>-1 && leds3[n]<3)
   {
    nBytes=websWrite(wp,T("<img src='images/led_%d.png'/>"),leds3[n]);
    //printf("leds3[%d]=%d\n",n,leds3[n]);
   }
   else
      nBytes=websWrite(wp, "&nbsp;");
   nBytes=websWrite(wp,"</td>\n<td>");
 
   if (leds2[n]>-1 && leds2[n]<3)
   {
    nBytes=websWrite(wp,T("<img src='images/led_%d.png'/>"),leds2[n]);
    //printf("leds2[%d]=%d\n",n,leds2[n]);
   }
   else
      nBytes=websWrite(wp, "&nbsp;");
   nBytes=websWrite(wp,"</td><td>");

   if (leds1[n]>-1 && leds1[n]<3)
   {
     //if ((signed char)leds1[n-1]>=0 && (signed char)leds1[n-1]<3)
     nBytes=websWrite(wp,T("<img src='images/led_%d.png'/>"),leds1[n]);
     //printf("leds1[%d]=%d\n",n,leds1[n]);
   }
   else
      nBytes=websWrite(wp, "&nbsp;");
   nBytes=websWrite(wp,"</td>");

   nBytes=websWrite(wp,"\n</tr>\n");
 }
 
nBytes=websWrite(wp,"<tr>\n");
 for (n=3;n>1;n--)
  nBytes=websWrite(wp,T("<td><div class='pl'><span class='plabel'>%s</span></div></td>"),blable[n-1]);
 nBytes=websWrite(wp,"<td><div class='pl'><span class='plabel'>");
 if (strlen(blable[0])>0)
    nBytes=websWrite(wp,T("%s"),blable[0]);
 else
    nBytes=websWrite(wp, "&nbsp;");
 nBytes=websWrite(wp, "</span></div></td>\n</tr>\n\n</table>\n</div>\n");
 /*
 for (n=col_count;n>0;n--)
 {
   nBytes=websWrite(wp,"<td>");
   if (leds1[n-1]>-1 && leds1[n-1]<3)
    nBytes=websWrite(wp,T("<img src='images/led_%d.png'/>"),leds1[n-1]);
   nBytes=websWrite(wp,"</td>");
 }
 nBytes=websWrite(wp,"<td><img src='images/led_1.png'/></td>\n</tr>\n<tr>\n");
 for (n=col_count;n>0;n--)
 {
   nBytes=websWrite(wp,"<td>");
   if (leds2[n-1]>-1 && leds2[n-1]<3)
    nBytes=websWrite(wp,T("<img src='images/led_%d.png'/>"),leds2[n-1]);
   nBytes=websWrite(wp,"<td>");
 }
  nBytes=websWrite(wp,"<td>");
 }
 if (run_led>-1)
   nBytes=websWrite(wp,T("<img src='images/led_%d.png'/>"),run_led);
 nBytes=websWrite(wp,"</td>\n</tr>\n<tr>\n<td>\n");
 for (n=2;n>0;n--)
  nBytes=websWrite(wp,T("<div class='pl'><span class='plabel'>%s</span></div>"),blable[n-1]);
 nBytes=websWrite(wp,"</td>\n<td>");
 if (run_led>-1)
  nBytes=websWrite(wp,T("\n<div class='pl'><span class='plabel'>%s</span></div>"),runlbl);
 nBytes=websWrite(wp,"</td>\n</tr>\n</table>\n</div>\n");
 */
 return nBytes;
}

int showHUPort(webs_t wp,char port_count,char port_status,char port_lbls[][12],char lbl_count,char port_lbl_pos,char *img_name,char *img_postfix
,char port_width,char port_height)
{
 char n,tlbl_count=1,blbl_count=0;
 int nBytes=0;
 
 if (lbl_count!=1)
 {
   if (port_lbl_pos!=2)
    tlbl_count=lbl_count;
   else
     blbl_count=1;
 }
 nBytes+=websWrite(wp, "<div class=\"port\" style=\"width:32px;\">\n<div class=\"pls\">\n");
 if (tlbl_count>1)
 {
   //for (n=0;n<tlbl_count;n++)
   for (n=tlbl_count;n>0;n--)
    nBytes+=websWrite(wp, T("<div class=\"pl\"><span class=\"ml\">%s</span></div>\n"),port_lbls[n-1]);
 }
 else
    nBytes+=websWrite(wp, T("<div class=\"pl\"><span class=\"sl\">%s</span></div>\n"),port_lbls[0]);


 nBytes+=websWrite(wp, T("</div>\n<div class=\"pimg\"><span><img  src=\"images/%s%d.%s\"/></span></div>"),img_name,port_status,img_postfix);

 if (blbl_count>0)
 {
   nBytes+=websWrite(wp, T("<div class=\"pls\">\n<div class=\"pl\"><span class=\"sl\">%s</span></div>\n</div>"),port_lbls[1]);
 }
 nBytes+=websWrite(wp, "</div>");
 return nBytes;
}

int showHUPorts(webs_t wp,char slot,char port_count,char port_status[],char port_lbls[][2][12],char lbl_count,char port_lbl_pos,char *img_name,char *img_postfix
,char port_width,char port_height)
{
 
 
 char n;
 int nBytes=0;
 for (n=0;n<port_count;n++)
 {
   nBytes+=showHUPort(wp,port_count,port_status[n],port_lbls[n],lbl_count, port_lbl_pos,img_name,img_postfix
, port_width,port_height);
 }
return nBytes;
 //printf("port_count=%d in showHUPorts\n",port_count);
 /*for (n=0;n<port_count;n++)
 {
  nBytes+=websWrite(wp, T("<div class=\"port\" style=\"width:32px;\">\n \
	<div class=\"pls\">\n \
	<div class=\"pl\"><span class=\"plabel\">%s</span></div>\n"),port_lbls1[n]);
  if (port_lbl_pos==2)
     nBytes+=websWrite(wp, T("<div class=\"pl\"><span class=\"plabel\">L2&nbsp;TX</span></div>\n"),port_lbls2[n]);

  nBytes+=websWrite(wp, T("</div>\n<div class=\"pimg\"><span><img width=\"%dpx\" src=\"images/%s0.%s\"/></span></div>"),img_width,img_name,img_postfix);

  if (port_lbl_pos==2)
     nBytes+=websWrite(wp, T("\n<div class=\"pls\"> \
	\n<div class=\"pl\"><span class=\"plabel\">%s</span></div>\n</div>\n"),port_lbls2[n]);

  nBytes+=websWrite(wp, "</div>");
 }
 return nBytes;
*/
}

char getUnitLedAndPortStatus(char slot,char leds_status[][4],char ports_status[])
{
  uchar_t n,m=0,uclass=0,utype=0;
  uchar_t port_count=0;
  if (TRUE!=getUnitClassType(0,slot,&uclass,&utype))
    return 0;
  if (UC_OTU==uclass)
  {
    otuUnitStatus_t *pOtuStatus;
    pOtuStatus=getOtuStatus(0,slot);
    if (NULL==pOtuStatus)
        return 0;
    if (2==utype)
      port_count=8;
    else
      port_count=4;
    for (n=0;n<port_count;n++)
    {
      if (2!=pOtuStatus->portStatus[n].on_module)
      {
          if (2!=pOtuStatus->portStatus[n].tx_enable)
          {
            ports_status[n]=pOtuStatus->portStatus[n].port_status;
            //leds_status[n][m]=pOtuStatus->portStatus[n].port_status;
            //ports_status[n]=leds_status[n][m];
          }
          else
          {
            ports_status[n]=3;
            //leds_status[n][m]=2;
            //ports_status[n]=3;
          }
        //printf("port #%d enable=%d\n",n+1,pOtuStatus->portStatus[n].tx_enable);
      }
      else
          ports_status[n]=0;
      if (port_count==8)
      {
         for (n=0;n<8;n++)
         {
            if (ports_status[n]!=1)
               leds_status[n/4+1][n%4]=0;
            else
               leds_status[n/4+1][n%4]=ports_status[n];
         }
         /*
         leds_status[1][0]=ports_status[0];
         leds_status[1][1]=ports_status[1];
         leds_status[1][2]=ports_status[2];
         leds_status[1][3]=ports_status[3];

         leds_status[2][0]=ports_status[4];
         leds_status[2][1]=ports_status[5];
         leds_status[2][2]=ports_status[6];
         leds_status[2][3]=ports_status[7];

         for (n=0;n<port_count;n++)
         {
            leds_status[n%d+1][m]=ports_status[n];
         }
         */
      }
       else
       {
         for (n=0;n<4;n++)
         {
            if (ports_status[n]!=1)
               leds_status[n/2+1][n%2]=0;
            else
              leds_status[n/2+1][n%4]=ports_status[n];
         }
       }
    }
  }
  return port_count;
}

int showHUnitView(webs_t wp ,uchar_t slot,uchar_t uclass,uchar_t utype)
{
  char leds_status[3][4];
  char ports_status[8];

  char port_lbls[8][2][12]={"PWR","S1","S2",""};
  char port_lbls2[8][2][12]={"RUN","CH1","CH2",""};

  uchar_t port_count;
  port_count=getUnitLedAndPortStatus(slot,leds_status,ports_status);
  printf("port_count=%d\n",port_count);
  if (port_count<1)
    return -1;
  //show4ColHLeds(slot,leds4col,left_lable,right_lable,3);
  
  return showHUPorts(wp,slot,port_count,ports_status,port_lbls,2,2,"sfph","jpg",25,15);
}
