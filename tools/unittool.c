#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <time.h>
#include <sys/time.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sysinfo.h>

#include <unit.h>
#include <visipc.h>
#include <chassis.h>
#include <public.h>
//arm-linux-gcc -o utool unittool.c -I./../include -L/home/visint/src/arm/lib -lvispace -liconv -lsqlite3
void saveUnitBaseInfo(char slot)
{
  unit_base_info_t bi;
  char buf[100];
  FILE *fp;
  if (slot<1 || slot>2)
     return;
  srand(time(0)); 
  sprintf(bi.sn,"SN-1107%d%d%d%d%d",rand()%10,rand()%10,rand()%10,rand()%10,rand()%10);
  if (slot==1)
  {
    strcpy(bi.model,"OTU 2.5G");
    strcpy(bi.fwver,"01.10.24");
    strcpy(bi.hwver,"01.01.01");
  }
  else
  {
    strcpy(bi.model,"ODM");
    strcpy(bi.fwver,"01.10.22");
    strcpy(bi.hwver,"01.01.02");
  }
  sprintf(bi.creation,"2013-%d-%d",10+rand()%3,1+rand()%30);
  sprintf(buf,"/vispace/unit%d.bi",slot);
  fp=fopen(buf,"wb");
  if (fp==NULL)
  {
    printf("saveUnitBaseInfo of unit#%d fail\r\n",slot);
    return;
  }
     //printf("fp!=NULL\r\n");
   fwrite(&bi,sizeof(unit_base_info_t),1,fp);
   fclose(fp);
   printf("saveUnitBaseInfo of unit#%d OK\r\nSN:%s creation:%s\r\n",slot,bi.sn,bi.creation);
   sleep(2);
}

void saveUnitInfo(char slot)
{
 uchar_t *us_shm;

    //char property[32]= {0};
    //char other_unit_class=0;
     if (slot<1  || slot>2)
       return;
     unit_base_info_t *bi=NULL;
     bi=getBaseInfoMap(0,slot);
     FILE *fp;
     unit_base_info_t ubi;
     char tmp_buf[512]={0};
     char buf[60]={0};
     //char property[60]={0};
     char *property;
     char is_valid=0;
   
     if (bi==NULL)
     {
      printf("Save fail:#%d NOT EXIST!\r\n",slot);
      return;
     }
     if (slot==1)
     {
        if (strstr(bi->model,"OTU"))
         is_valid=1;
     }
     else if (slot==2)
     {
        if (strstr(bi->model,"ODM"))
         is_valid=1;
     }
     if (is_valid){
      sprintf(buf,"/vispace/unit%d.bi",slot);
     fp=fopen(buf,"wb");
     if (fp==NULL)
       return;
     //printf("fp!=NULL\r\n");
     fwrite(bi,sizeof(unit_base_info_t),1,fp);
     property=getUnitPropertyMap(0,slot);
     if (property!=NULL)
     {
        fwrite(property+4,48,1,fp);      
     }
     else
     {
       fwrite(buf,48,1,fp);
     }
     fclose(fp);
     }
     //printf("Slot=%d a\r\n",slot);
     if (slot==1 && is_valid)
     {
       char *shm=getTempShm();
        bi=getBaseInfoMap(0,1);
        if (bi==NULL)
         is_valid=0;
        else if (strstr(bi->model,"OTU"))
         is_valid=1;
        else 
          is_valid=0;
       if (is_valid){
       fp=fopen("/vispace/unit.dat","wb");
       if (fp!=NULL)
       {
        if (shm!=NULL)
        {
           fwrite(shm,512,1,fp);
        }
        else
           fwrite(&tmp_buf,512,1,fp);
        fclose(fp);
       }
      }
     }
}

static void usage()
{
  printf("Ver 1.0.1\nLast modified:2015-01-23\n\n");
  printf("Usage:utool [option] \n");
  printf("Option Specification:\n");
  printf("-d: debug\n");
  printf("-l: ls unit file\n");
  printf("-s: save unit info(FROM UNIT)  to file\n");
  printf("-a: save unit info(AUTO MAKE) to file\n");
}
int main(int argc, char **argv)
{
  int oc;
  while((oc = getopt(argc, argv, "l:hsa")) != -1)
  {
     switch(oc)
     {
       case 'l':
         doSystem("ls /vispace/%s",optarg);
         exit(0);
       break;
       case 's':
         saveUnitInfo(1);
         saveUnitInfo(2);
         //exit(0);
       break;
       case 'a':
       saveUnitBaseInfo(1);
       saveUnitBaseInfo(2);
       break;
       case 'h':
         usage();
       exit(0);
     }
  }
  system("ls /vispace/unit*");
  char tmp_buf[512];
  int n;
  FILE *fp;
  for (n=1;n<3;n++)
  {
    sprintf(tmp_buf,"/vispace/unit%d.bi",n);
    fp=fopen(tmp_buf,"rb");
    if(fp!=NULL)
    {
     unit_base_info_t bi;
     fread(&bi,sizeof(unit_base_info_t),1,fp);
     printf("Unit #%d SN:%s\r\n",n,bi.sn);
     fclose(fp);
    }
    else
        printf("Unit #%d Not Exist!\r\n",n);
  }
}
