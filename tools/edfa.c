#include <stdio.h>
#include <string.h>
#include <typedefs.h>
#include <stdlib.h>

#include <signal.h>
#include <time.h>
#include <sys/time.h>
#include <unistd.h>


#include <visipc.h>
#include <chassis.h>
#include <olp.h>
#include <public.h>
#include <infoset.h>
#include <io.h>
//extern char is_debug_io;
//arm-linux-gcc -o edfa edfa.c -I./../include -L/home/visint/src/arm/lib -lvispace -liconv -lsqlite3

static void usage()
{
  printf("Description:\n");
  printf("Show EDFA property\nLast modified:%s\n\n",LAST_MODIFY_DATE);
  printf("Usage:otaptool [option] \n");
  printf("Option Specification:\n");
  //printf("-f: file name\n");
  printf("-d: debug\n");
  printf("-s: slot of unit\n");
  printf("-h for help.\n");
}

void showProperty(uchar_t slot)
{
   char *sn,*property,strct_type=1,sw_type=1,fw_type=1;
   int n;
   //printf("Unit #%d property!\n",slot);
   property=getUnitPropertyMap(0,slot);
   if (property!=NULL)
   {
      printf("#%d\t",slot);
      property+=12;
      //printf("%s\n",*property);
      for (n=0;n<30;n++)
      {
         if (*property!=0)
         {
            printf("%c",*property);
            property++;
         }
          else
             break;
      }
      //sn=property+8;
      //  printf("%s",n,sn);
      printf("\n");
      //fw_type=*(property+4);//获取单元盘结构属性
      //sw_type=*(property+5);//获取单元盘结构属性
      //strct_type=*(property+6);//获取单元盘结构属性
   }
 }
/*
属性的报文格式:107+000+data
*/
void setProperty(uchar_t slot,char opt,char *arg,short value)
{
 int n=5;
 
 char message[512];
 
 memset(message,0,512);
 if (opt!='a')
 {
  message[0]='S';
  message[1]=107;
  n=5;
 }
 else
 {
   message[0]=51;
   n=2;
 }
 char *p = NULL;

 //char delim = " ";

  //printf("Setting unit #%d property=%s OK!\n",slot,arg);
  p = strtok(arg, " ");
  while(p != NULL){
    message[n]=(char)atoi(p);
    printf("the byte[%d] is :%d\n",n,message[n]);
    //printf("the str is : %s\n",message);
    p = strtok(NULL," ");
    n++;
  }
  if (opt=='a')
  {
    memcpy(message+n,&value,2);
  }

 if (opt!='a')
 {
   if (sendMessageToUnit(0,slot,message,300,0,0,0)!=EXIT_SUCCESS)
   {
     printf("Setting unit #%d insertion loss fail!\n",slot);
   }
    else
      printf("Setting unit #%d insertion loss OK!\n",slot);
 }
 else
   ioSet(0,slot,message, cmd_set_runtime_2);
}

void main(int argc, char **argv)
{

 int n,oc;
 char ec;      /*无效的选项字符*/
 char *b_opt_arg;            /*选项参数字串 */
 char cmd[120]="",property[120]="";
 short value=0;
 char val,message[512];
 uchar_t slot=0;
 memset(message,0,512*sizeof(char));
 message[0]=51;
 //message[1]=107;
 b_opt_arg=NULL;
 //strcpy(b_opt_arg,"asdasd");
 while((oc = getopt(argc, argv, "rs:dh")) != -1)
 {
    switch(oc)
    {
      case 'd':
            debug.is_debug_mode=1;
            break;
      case 's':
          slot=atoi(optarg);
          //opt='s';
          //showProperty(slot);
      break;
      case 'h':
             usage();
             return;
      break;
    }  
 }
 // printf("#%d\n",slot);
 if (slot<1)
 {
   uchar_t uclass;
   for (slot=1;slot<16;slot++)
   {
    uclass=getUnitClass(0,slot);
    //printf("#%d uc=%d\n",slot,uclass);
    if (uclass==9)
          showProperty(slot);
   }
 }
 else
  showProperty(slot);
}

