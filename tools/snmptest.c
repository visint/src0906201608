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
#include <log.h>

#define OID "1.3.6.1.4.1.10215.2.1."
#define InfoSetHeadOid "5.88.1.4.1.3"
#define HardwareVerHeadOid "2.1.2.1.1.2" //vsModuleBaseInfoTable/vsModuleHardwareVer
#define ModuleNameHeadOid "2.1.2.2.1.2"  //vsModuleUserDataTable/vsModuleName
#define FactoryDefaultHeadOid "2.1.2.3.1.1.9"   //vsNmuTable/vsNmuFactoryDefault

//arm-linux-gcc -o snmptest snmptest.c -I./../include -L/home/visint/src/arm/lib -lvispace -liconv -lsqlite3
static void usage()
{
  printf("Description:\n");
  printf("snmp test\nLast modified:%s\n\n",LAST_MODIFY_DATE);
  printf("Usage:snmptest[option] \n");
  printf("Option Specification:\n");
  printf("-d: debug\n");
  printf("-e: max errors to save\n");
  printf("-s: sleep\n");
}
static char current_oid=0;
char ping()
{
  char buf_ps[128]="";
  char ping_ok=1;    
    //char ps[512]={0};
   FILE *ptr;
   if((ptr=popen("ping 127.0.0.1 -c 1 -W 2", "r"))!=NULL)
   {
       while(fgets(buf_ps, 64, ptr)!=NULL)
       {
           if (strstr(buf_ps,", 0 received"))
           {
               ping_ok=0;
               break;
           }
       }
      pclose(ptr);
   }
   else
       ping_ok=0;
   return ping_ok;
}
int snmp_fail_count=0;

void snmpget()
{
    char head_oids[][16]={InfoSetHeadOid,HardwareVerHeadOid,ModuleNameHeadOid,FactoryDefaultHeadOid};
    char buf_ps[128]="";
    char is_snmpd_ok=0;    
    FILE *ptr;
    //strcpy(ps, cmd);
    //printf("\nSlotOfNmu:%d\n",getSlotOfNmu(0,10));
    if (current_oid<4)
    {
       sprintf(buf_ps,"snmpget -m ALL -Ov -c public localhost %s%s.0.%d",OID,head_oids[current_oid],getSlotOfNmu(0,1));
     if (current_oid<1)
     {
       strcat(buf_ps,".8.1");
      // printf("\n%s\n",buf_ps);
     }
    }
    else
       sprintf(buf_ps,"snmpget -m ALL -Ov -c public localhost 1.3.6.1.4.1.10215.2.1.5.88.1.1.1.1.0");
if((ptr=popen(buf_ps, "r"))!=NULL)
    {    
        while(fgets(buf_ps, 32, ptr)!=NULL)
        {    
           //strcat(result, buf_ps);
           //if(strlen(result)>1024)
             //printf("result=%s len=%d\n",buf_ps,strlen(buf_ps));
             if (current_oid==0 || current_oid==3)
             {
               if (strstr(buf_ps,"INTEGER"))
               {
                  is_snmpd_ok=1;
                  //snmp_start_count--;
                  break;
               }
              // else
              //      printf("result=%s current_oid=%d\n",buf_ps,current_oid);
             }
             
             else if (strstr(buf_ps,"STRING")||strlen(buf_ps) <6) 
             {
                  //printf("Snmpd is Ok!\n");
                  is_snmpd_ok=1;
                  //snmp_start_count--;
                  break;
             }   
        }    
        pclose(ptr);    
        ptr = NULL;
    }
    if (is_snmpd_ok)
    {
      //snmp_start_count=0;
      snmp_fail_count=0;
      //printf("is_snmpd_ok %d\n",current_oid);
    }
    else
    {
      snmp_fail_count++;
      if (ping())
      {
         addSysLog(SYS_lOG_SNMP_TIMEOUT,"snmpd(%d/%d) timeout!CT:%d NS:%d",current_oid,snmp_fail_count,getChassisClass(0),getSlotOfNmu(0,1));
               //insertSysLog(SYS_lOG_SNMP_TIMEOUT,message);
      }
       else
       {
           insertSysLog(SYS_LOG_PING_FAIL,"ping fail");
           printf("ping timeout\n");
       }
    }
    if (current_oid<4)
       current_oid++;
    else
       current_oid=0;
}
static void sigHandle(int sig,struct siginfo *siginfo,void *myact)
{
  //printf("sig=%d siginfo->si_int=%d SIGALRM=%d,SIGSEGV=%d\n",sig,siginfo->si_int,SIGALRM,SIGSEGV);
  if (sig == SIGALRM)
  {
     snmpget();
     alarm(5);
  }
}

static void sigInit()
{
  //int i;
  struct sigaction act;
  sigemptyset(&act.sa_mask);  
  act.sa_flags=SA_SIGINFO;
  act.sa_sigaction=sigHandle;

  sigaction(SIGALRM,&act,NULL);
}
static void init_snmptest(int argc, char **argv)
{
   int oc;
   int waitting=60;
  //printf("Auto reboot testing...0\r\n");
  while((oc = getopt(argc, argv, "de:s:h")) != -1)
  {
     //printf("oc=%d\r\n",oc);
     switch(oc)
     {
       case 'd':
         debug.is_debug_mode=1;
       break;
       case 'e':
         debug.max_err_count=atoi(optarg);
       break;
       case 's':
         waitting=atoi(optarg);
       break;
       case 'h':
         usage();
       exit(0);
     }
  }
  initAlarmConf();
  if (debug.is_debug_mode!=1)
  initDebug();
  sigInit();
  alarm(waitting);
}
void main(int argc, char **argv)
{
  init_snmptest(argc,argv);
  while (1)
  {	
    pause();
  }
}
