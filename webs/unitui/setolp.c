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

//arm-linux-gcc -o setolp setolp.c -I./../include -L/home/visint/src/arm/lib -lvispace -liconv -lsqlite3
static void usage()
{
  printf("Description:\n");
  printf("set olp optical switch as positive or negative\nLast modified:%s\n\n",LAST_MODIFY_DATE);
  printf("Usage:initunit [option] \n");
  printf("Option Specification:\n");
  printf("-o: set olp optical switch as negative\n");
  printf("-d: display sys conf\n");
  printf("-c: clear olp optical switch conf\n");
  printf("-r: reboot ccenter\n");
  printf("-s: slot of unit\n");
  printf("-h for help.\n");
}
void main(int argc, char **argv)
{
 int n,m=0,oc;
 char ec;      /*无效的选项字符*/
 char *b_opt_arg;            /*选项参数字串 */
 char opt=0;
 uchar_t slot=0;
 char buf[512]="";
  while((oc = getopt(argc, argv, "ds:i:rph")) != -1)
 {
    switch(oc)
    {
      case 'd':
            is_debug_io=1;
            break;
      case 'c':
          opt='c';
          strcpy(property,optarg);
      break;
      case 's':
          slot=atoi(optarg);
          p = strtok(arg, " ");
          while(p != NULL)
          {
             slot=(uchar_t)atoi(p);
             unit_base_info_t *bi;
             bi=getBaseInfoMap(0,slot);
             if (bi!=NULL)
                printf("Unit %d SN :%s\n",n,bi->sn);
            //printf("the str is : %s\n",message);
             p = strtok(NULL," ");
           }
      opt='c';
      break;
      case 'r':
           system("killall ccenter");
           sleep(1);
           system("ccenter&");
           return;
      case 'h':
             usage();
             return;
      break;
    }
    
 }
switch(opt)
{
  case 'p':
      showProperty(slot);
      return;
  case 'i':
      setProperty(slot,property);
      return;
}
}

