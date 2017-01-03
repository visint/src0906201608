#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <public.h>
//arm-linux-gcc -o upgrade  upgrade.c -I../include -L=./ -L/home/visint/src/arm/lib -lvispace -liconv -lsqlite3
static void usage()
{
  printf("Description:\n");
  printf("System upgrade ver 1.1.3\nLast modifed:%s\n",LAST_MODIFY_DATE);
  printf("Usage:upgrade [option]\n");
  printf("Option Specification:\n");
  //printf("-D: print debug infomation\n");
  //printf("-o: stop debug and run the release version\n");
  printf("-p: host ip address\n");
  printf("-f: file name\n");
  printf("-s: slots\n");
  /*
  //printf("-f: show the configure infomation\n");
  printf("-S: save(default Not save) the debug infomation to log\n");
  printf("-c: interval count(default:50) for saving the debug infomation to log\n");
  printf("-C: Clean the debug infomation in log\n");
  printf("-m: the max count(default:1000) to save the debug infomation to log\n");
  printf("-v: show version\n");
*/
  printf("-h for help.\n");
}

void main (int argc, char **argv)
{
  /*
  int n,m=0,oc,count=5,max_count=0;
  char opt=0,is_save=0,debug_flag='N';
  */
  char host_ip[60]="";
  char file_name[60]="";
  char slots[17]={0};
  char unit_name[16]="";
  char *p = NULL;
  int oc,slot;
  char is_auto=1;
  while((oc = getopt(argc, argv, "f:p:s:h")) != -1)
  {
   switch (oc)
   {
    case 'p':
          strcpy(host_ip,optarg);
    break;
    case 'f':
          is_auto=0;
          strcpy(file_name,optarg);
    break;
    case 's':
          
 //char delim = " ";

  //printf("Setting unit #%d property=%s OK!\n",slot,arg);
    p = strtok(optarg, " ");
    while(p != NULL){
    slot=atoi(p);
    if (slot>=0 && slot<17)
       slots[slot]=1;
    p = strtok(NULL," ");
    }
    break;
    case 'h':
       usage();
       exit(0);
   }
  }
  if (strlen(file_name)<3)
    exit(0);
  strncpy(unit_name,file_name,3);
  printf("file_name=%s unit_name=%s\r\n",file_name,unit_name);
  uchar_t n;
  uchar_t uclass,utype;
  //uchar_t class_name[16];
  char *un;
  for (n=0;n<17;n++)
  {
    if (getUnitClassType(0,n,&uclass,&utype)!=FALSE)
    {
      getUnitLabelName(uclass,utype,unit_name);
      un=strTolower(unit_name);
      if (strstr(file_name,un))
      {
        if (is_auto || !is_auto && slots[n])
            printf("#%d unit_name=%s\r\n",n,unit_name );
      }
    }
  }
  printf("\n");
  //system("cd /tmp");
  if (strlen(host_ip)>6)
  {
    doSystem("cd /tmp && tftp %s -g -r %s",host_ip,file_name);
  }
}
