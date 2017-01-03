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

static int asp4u_unit_ports_view(webs_t wp,int slot,int height,char port_count,char port_status[],char port_label_head[][12],char port_label[][MAX_PORT_COUNT][12],char isTxtAtTwoSide,char port_img[][8],char port_img_size[][2])
{
  int nBytes=0;
  char n,m;
  
  nBytes +=websWrite(wp, T("<table class=\"ports4u\" height='%d'>"),height);
  for (n=0;n<port_count;n++)
  {
    //printf("isTxtAtTwoSide=%d %s\n",isTxtAtTwoSide,port_label[1][n]);
    nBytes +=websWrite(wp, T("<tr><td>%s"),port_label[0][n]);
    if (!isTxtAtTwoSide && strlen(port_label[1][n])>0)
       nBytes +=websWrite(wp, T("</br></br>%s"),port_label[1][n]);
    //nBytes +=websWrite(wp, T("</td>"));
    /*if (strlen(port_label[1][n])>0)
      nBytes +=websWrite(wp, T("</br>%s"),port_label[1][n]);
    */
    nBytes +=websWrite(wp, T("</td><td><img width=%d height=%d src='images/%s%d.png'/></td>"),port_img_size[n][0],port_img_size[n][1],port_img[n],port_status[n]);
    if (isTxtAtTwoSide)
       nBytes +=websWrite(wp, T("<td>%s</td>"),port_label[1][n]);
    nBytes +=websWrite(wp, "</tr>");
  }
  nBytes +=websWrite(wp, "</table>");
}
static int asp4u_unit_head_view(webs_t wp,int slot,uchar_t uclass,uchar_t utype,char *uname,ledsInfo_t leds)
{
  int nBytes=0;
  char n,m;

  //nBytes=websWrite(wp, T("\n<div class='ubd1'>
nBytes=websWrite(wp, T("\n \
<div class='un'><a href='/goform/"));
if (!(uclass==UC_OTU || uclass==UC_OEO)) 
   nBytes=websWrite(wp, "slot");
else
    nBytes=websWrite(wp, "unit");
nBytes=websWrite(wp, T("?cid=0&uid=%d' target='mf'>%s</a></div>\n"),slot,uname);
if (uclass==UC_OLP && utype==41)
{
  //nBytes +=websWrite(wp, "<table width=\"40\" align=\"center\" class=\"leds4u\"><tr>");
  nBytes +=websWrite(wp, "<table width=\"40\" align=\"center\"><tr>");
  nBytes=websWrite(wp, "<td><img src='images/vled_1.png' /></td>");
  nBytes=websWrite(wp, "<td><img src='images/vled_3.png' /></td>");
  nBytes=websWrite(wp, T("<td><img src='images/vled_%d.png' /></td>"),leds.leds_status[0][2]);
  nBytes=websWrite(wp, T("<td><img src='images/vled_%d.png' /></td>"),leds.leds_status[0][3]);

  nBytes +=websWrite(wp, "<tr>");
  nBytes=websWrite(wp, T("<td><img src='images/vled_%d.png' /></td>"),leds.leds_status[1][0]);
  nBytes=websWrite(wp, T("<td><img src='images/vled_%d.png' /></td>"),leds.leds_status[1][1]);
  nBytes=websWrite(wp, T("<td><img src='images/vled_%d.png' /></td>"),leds.leds_status[1][2]);
  nBytes=websWrite(wp, T("<td><img src='images/vled_%d.png' /></td>"),leds.leds_status[1][3]);
  nBytes=websWrite(wp, "</tr>");

  nBytes +=websWrite(wp, "<tr>");
  nBytes=websWrite(wp, T("<td><img src='images/vled_%d.png' /></td>"),leds.leds_status[2][0]);
  nBytes=websWrite(wp, T("<td><img src='images/vled_%d.png' /></td>"),leds.leds_status[2][1]);
  nBytes=websWrite(wp, T("<td><img src='images/vled_%d.png' /></td>"),leds.leds_status[2][2]);
  nBytes=websWrite(wp, "<td></td>");
  nBytes=websWrite(wp, "</tr>");

 nBytes=websWrite(wp, "</tr></table>");
 return nBytes;
}
nBytes +=websWrite(wp, "<table class=\"leds4u\"><tr><td");
if (leds.col_count>2)
  nBytes +=websWrite(wp, " colspan=\"2\"");
nBytes +=websWrite(wp, ">PWR</td><td><img src='images/led_1.png' /></td>");
if (leds.run_led>-1 && leds.run_led<4)
{
 nBytes +=websWrite(wp, T("<td><img src='images/led_%d.png' /></td><td"),leds.run_led);
 if (leds.col_count>2)
  nBytes +=websWrite(wp, " colspan=\"2\"");
  nBytes +=websWrite(wp, T(">%s</td>"),leds.run_text);
}
nBytes +=websWrite(wp, "</tr>");
int start=0;
if (leds.col_count>3)
    leds.col_count=0;
else
{
    start=1;
}
//printf("leds.col_count=%d\r\n",leds.col_count);
for (n=0;n<2;n++)
{
  nBytes=websWrite(wp, T("<tr><td>%s</td>"),leds.leds_text[0][n]);
  for (m=0;m<2;m++)
  {
    //printf("leds.leds_status[%d][%d]=%d",n,start+m,leds.leds_status[n][start+m]);
    nBytes=websWrite(wp, T("<td><img src='images/led_%d.png' /></td>"),leds.leds_status[n][start+m]);
  }
//printf("\r\n");
  nBytes=websWrite(wp, T("<td>%s</td></tr>"),leds.leds_text[1][n]);
}
nBytes=websWrite(wp, "</table>");
return nBytes;
}
int asp4u_unit_view(webs_t wp,uchar_t slot,uchar_t uclass,uchar_t utype)
{
  unitViewInfo_t uvi;
  int nBytesSent=0,n;
  int height;
  char port_img[8][8]={"lcv1_","lcv1_","lcv1_","lcv1_","lcv1_","lcv1_"};
  char port_img_size[8][2]={{22,20},{22,20},{22,20},{22,20},{22,20},{22,20}};

  char isTxtAtTwoSide=1;
  //int port_count;
  if (utype==31)
  {
     /*
     if (utype==31)
          port_count=4;
     else
         port_count=3;
     */
     for (n=0;n<4;n++)
      strcpy(port_img[n],"sc1_");
  }
  else if (utype==32)
  {
    strcpy(port_img[0],"lcv1_");
    strcpy(port_img[1],"sc1_");
    strcpy(port_img[2],"sc1_");
  }
  else if (utype==41)
  {
    strcpy(port_img[0],"lcv2_");
    strcpy(port_img[1],"lcv2_");
    strcpy(port_img[2],"lcv2_");

    port_img_size[0][0]=20;
    port_img_size[0][1]=30;

    port_img_size[1][0]=20;
    port_img_size[1][1]=30;

    port_img_size[2][0]=20;
    port_img_size[2][1]=30;
    isTxtAtTwoSide=0;
  }
  getOlpViewInfo(0,slot,uclass,utype,&uvi);
  //for (n=0;n<2;n++)
  // printf("%d %d %d %d\r\n",uvi.leds.leds_status[0][1],uvi.leds.leds_status[0][2],uvi.leds.leds_status[1][1],uvi.leds.leds_status[1][2]);
 // if (utype==41)
   // nBytesSent=websWrite(wp, "\n<div>pwr Set Pri Auto</div><div>pwr Set Pri Auto</div>");
  nBytesSent=websWrite(wp, "\n<div class='ubd1'>");
  nBytesSent+= asp4u_unit_head_view(wp,slot,uclass,utype,uvi.unit_name,uvi.leds);
  //printf("#%d portCount%d\n",slot,uvi.ports.portCount);
 if (uclass==UC_OLP && utype>40 && utype<50)
 {
   nBytesSent +=websWrite(wp,"<img src='images/olpledl.png' />");
   height=150;
 }
 else
     height=200;
 nBytesSent+=asp4u_unit_ports_view(wp,slot,height,uvi.ports.portCount,uvi.ports.ports_status,uvi.ports.ports_htext,uvi.ports.ports_text,isTxtAtTwoSide, port_img,port_img_size);
 if (uvi.utype_text[0]!='\0')
   nBytesSent=websWrite(wp, T("\n<div style=\"text-align:center;\">%s</div>"),uvi.utype_text);
 nBytesSent+=websWrite(wp, "\n</div>");
 return nBytesSent;
}


