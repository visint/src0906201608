#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <public.h>
//arm-linux-gcc -o ftransfer ftransfer.c -I../include -L=./ -L/home/visint/src/arm/lib -lvispace -liconv -lsqlite3

static void usage()
{
  printf("Description:\n");
  printf("Unit upgrade ver 1.0.2\nLast modified:%s\n\n",LAST_MODIFY_DATE);
  printf("Usage:upgrade [option] [value]\n");
  printf("Option Specification:\n");
  printf("-s: slots to upgrade\n");
  printf("-c: class of unit to upgrade\n");
  printf("-t: type of unit to upgrade\n");
  printf("-p: file path\n");
  printf("-f: file name\n");
  printf("-h for help.\n");
}

int main (int argc, char **argv)
{
  int oc,count=0;
  uchar_t slots[32]={0},uclass=0,utype=0;
  char path[64]="/tmp/";
  char fname[32]="olp51_2.bin";
  //int ret=0;
  //usage();
  while((oc = getopt(argc, argv, "c:f:p:s:t:h")) != -1)
  {
    switch(oc)
    {
       case 'p':
          strcpy(path,optarg);
          break;
       case 'f':
          strcpy(fname,optarg);
          break;
       case 's':
          count=strtoca(optarg," ",slots,10);
          break;
       case 'c':
          uclass=(uchar_t)atoi(optarg);
          break;
       case 't':
          utype=(uchar_t)atoi(optarg);
          break;
       case 'h':
          usage();
          return 0;
    }
  }

  int uc=0,ut=0;
  uchar_t n;
  if (uclass==0)
  {
    sscanf(fname, "%*[^-]-%d-%d", &uc, &ut);
    //printf("\nfname:%s uclass=%d, utype=%d\n",fname, uc, ut);
  }
  else
  {
    uc=uclass;
    ut=utype;
  }
  if (uc==0)
  {
    printf("\nUnkown unit class ...\n");
    return 3;
  }

  if (count<1)
  {
   uchar_t slotOfNmu;
   slotOfNmu=getSlotOfNmu(0);
   for (n=1;n<slotOfNmu;n++)
   {
     if (getUnitClassType(0,n,&uclass,&utype)!=FALSE)
     {
       if (uclass==uc)
       {
          if (ut==0 || ut==utype )
           slots[count++]=n;
       }
     }
   }
  }
  else
  {
    int sc=count;
    count=0;
    uchar_t ss[32];
    memcpy(ss,slots,32);
    memset(slots,0,32);
    for (n=0;n<sc;n++)
    {
     if (ss[n]> 0 && getUnitClassType(0,ss[n],&uclass,&utype)!=FALSE)
     {
       if (uclass==uc)
       {
          if (ut==0 || ut==utype )
           slots[count++]=ss[n];
       }
     }
   }
  }
  for (n=0;n<count;n++)
    printf("#%d ",slots[n]);
  
  if (count>0)
  {
    strcat(path,fname);
    printf("\nUpgrading file:%s...\n",path);
    if (!file_transfer(0xff,count,slots,0x04,path,0))
    {
      printf("Upgrade OK\n"); 
      return 0;
    }
    else
    {
      printf("Upgrade fail\n");
      return 2;
   }
  }
  else
      printf("Not unit to upgrade\n");
  return 1;
}
