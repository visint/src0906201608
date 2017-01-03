#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unit.h>
#include <infoset.h>
#include <visipc.h>
#include <passive.h>

boolean_t getOdmViewInfo(char *property,uchar_t uclass,uchar_t utype,unitViewInfo_t *info)
{
  //char *property;
  char pText[10][8]={"IN","OUT","COM","CTX","CRX","UTX","URX","PASS","","GPON","OSC"};
  uchar_t n,m,wl;
  odmProperty_t *odm=(odmProperty_t*)property;
  if (NULL==odm)
    return FALSE;
  /*
   if (utype!=12)
     info->ports.portCount=10;
  else
     info->ports.portCount=5;
  */
  info->ports.portCount=10;
  strcpy(info->unit_name,"ODM");
  
  if (utype!=12)
  {
    if (251==odm->leftWaveLength[10])
      strcpy(info->utype_text,"OM");
    else if (252==odm->leftWaveLength[10])
      strcpy(info->utype_text,"OD");
  
    if (251==odm->rightWaveLength[10])
    {
      if (info->utype_text[0]!='\0')
        strcat(info->utype_text,"/");
       strcat(info->utype_text,"OM");
    }
    else if (252==odm->rightWaveLength[10])
    {
      if (info->utype_text[0]!='\0')
         strcat(info->utype_text,"/");
      strcat(info->utype_text,"OD");
    }
  }
  for (n=0;n<10;n++)
  {
    for (m=0;m<2;m++)
    {
     if (0==m)
        wl=odm->leftWaveLength[n];
     else
        wl=odm->rightWaveLength[n];

     if (wl>0)
     {
      if (wl>199 && wl<211)
         sprintf(info->ports.ports_text[m][n],"%s",pText[wl-200]);
      else if (wl>20 && wl<90)
        sprintf(info->ports.ports_text[m][n], "C%d",wl-6);
      else if (wl<20)
        sprintf(info->ports.ports_text[m][n], "%d",1250+wl*20);
      else if (wl==99)
         strcpy(info->ports.ports_text[m][n],"UNK");
      else
         strcpy(info->ports.ports_text[m][n]," ");
     }
     else if (wl==0)
         strcpy(info->ports.ports_text[m][n],"N/A");
     else
         strcpy(info->ports.ports_text[m][n]," ");
   }
 }
 return TRUE;
}

boolean_t getOadmViewInfo(char *property,uchar_t uclass,uchar_t utype,unitViewInfo_t *info)
{
  uchar_t n;
  odmProperty_t *odm=(odmProperty_t*)property;
  if (NULL==odm)
    return FALSE;
  info->ports.portCount=10;
  strcpy(info->unit_name,"OADM");
  char label1[16]="",label2[16]="";
  char *ptr=property+4;

  for (n=0;n<10;n++)
  {
     label1[0]='\0';
     label2[0]='\0';
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
                       sprintf(label1,"IN %d",*(ptr+n*4)/2+1);
                     else
                       sprintf(label1,"OUT %d",*(ptr+n*4)/2+1);

                     if (*(ptr+n*4+2)%2)
                       sprintf(label2,"IN %d",*(ptr+n*4)/2+1);
                     else
                       sprintf(label2,"OUT %d",*(ptr+n*4)/2+1);
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
                      label2[0]='\0';
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
                       label2[0]='\0';
             }
             
            }
           sprintf(info->ports.ports_text[0][n],"%s",label1);
           sprintf(info->ports.ports_text[1][n],"%s",label2);
              //nBytesSent +=websWrite(wp, T("<tr><td>%s</br>%s</td><td><img src='images/lcv1_2.png' height=\"30\" width='12' /></td></tr>"),label1,label2);
  }
  return TRUE;
}

boolean_t getPassiveViewInfo(uchar_t chassis,uchar_t slot,uchar_t uclass,uchar_t utype,unitViewInfo_t *info)
{
  char *property;
  uchar_t n,max_port=8;

  property=getUnitProperty(0,slot);
  if (NULL==property)
    return FALSE;
  memset(info,0,sizeof(unitViewInfo_t));
  info->uclass=UC_PASSIVE;
  memset(info->ports.ports_status,2,10);
  switch (utype){
  case PASSIVE_ODM_UNIT:
  case PASSIVE_ODM12_UNIT:
       return getOdmViewInfo(property,uclass,utype,info);
  case PASSIVE_ODM3_UNIT:
     info->view_images_count=1;
     strcpy(info->unit_name,"ODM");
     strcpy(info->view_images[0],"net_h_port_6");
     
     break;
  case PASSIVE_DCM1_UNIT:
       info->view_images_count=1;
       strcpy(info->unit_name,"DCM");
       strcpy(info->view_images[0],"dcm_h");
     break;
     // return getOdmViewInfo(property,uclass,utype,info);
  case PASSIVE_OADM_UNIT:
     //info->view_images_count=1;
     strcpy(info->unit_name,"OADM");
     return getOadmViewInfo(property,uclass,utype,info);
     //strcpy(info->view_images[0],"net_h_port_6");
     break;
  case PASSIVE_NET_UNIT:
     info->view_images_count=1;
     strcpy(info->unit_name,"NET");
     strcpy(info->view_images[0],"net_h_port_6");
     //printf("getPassiveViewInfo:NET\r\n");
     break;
  default:
       return FALSE;
  }
 return TRUE;
}
