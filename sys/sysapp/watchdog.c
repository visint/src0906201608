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

#include <visipc.h>
#include <chassis.h>
#include <public.h>

#define SNMPD_SERVICE_ID 2
#define WEBS_SERVICE_ID 3
#define CCENTER_SERVICE_ID 9
#define OLP_SERVICE_ID 10

//#define WEBS_SERVICE "/vispace/webs/webs&"
//#define WEBS_PATH "/home/visint/VisProj/src/webs/LINUX"
#define WEBS_PATH "/vispace/webs"
#define SNMPD_SERVICE "snmpd -c /vispace/snmp/snmpd.conf"
#define CLID_SERVICE "/vispace/webs/webs&"
//gcc -o watchdogd watchdog.c -I./../../include -L./../../linuxlib -lvispace -liconv -lsqlite3


//arm-linux-gcc -o watchdogd watchdog.c -I./../../include -L/home/visint/src/arm/lib -lvispace -liconv -lsqlite3
static unsigned short start_count=0,ccenter_restart_count=0;
static unsigned short webs_start_count=0;
static unsigned short snmp_start_count=0;
static io_get_err=0;
static char lang[3]="";
static char is_save_log=0,is_run_olp=0;
static void save_log(char *proccess)
{
 if (is_save_log)
 {
    time_t tt;
    time(&tt);
    doSystem("sed -i '1a start %s by wd at %s\n' /vispace/err.log",proccess,ctime(&tt));
 }  
}
static char current_oid=0;
static void startService(int proc)
{
  pid_t pid;
  if (WEBS_SERVICE_ID==proc)
  {
     pid=getPidByName("pidof webs");
     if (pid<1)
     {
        if (start_count<50)
         start_count++;
        printf("Starting webs...\n");
        if (is_save_log)
             save_log("webs");
        doSystem("cd %s && ./webs%s&",WEBS_PATH,lang);
        sleep(10);
        //system(WEBS_SERVICE);
      }
  }
  else if (SNMPD_SERVICE_ID==proc)
  {
    #define OID "1.3.6.1.4.1.10215.2.1."
    #define InfoSetHeadOid "5.88.1.4.1.3"
    #define HardwareVerHeadOid "2.1.2.1.1.2" //vsModuleBaseInfoTable/vsModuleHardwareVer
    #define ModuleNameHeadOid "2.1.2.2.1.2"  //vsModuleUserDataTable/vsModuleName
    #define FactoryDefaultHeadOid "2.1.2.3.1.1.9"   //vsNmuTable/vsNmuFactoryDefault
    /*
    #define InfoSetTailOid ".8.1"
    #define HardwareVerTailOid "" //vsModuleBaseInfoTable/vsModuleHardwareVer
    #define ModuleNameTailOid ""  //vsModuleUserDataTable/vsModuleName
    #define FactoryDefaultTailOid ""   //vsNmuTable/vsNmuFactoryDefault
    */

    char head_oids[][16]={InfoSetHeadOid,HardwareVerHeadOid,ModuleNameHeadOid,FactoryDefaultHeadOid};
    //char tail_oids[][16]={InfoSetTailOid,HardwareVerTailOid,ModuleNameTailOid,FactoryDefaultTailOid};
    //char oids[][16]={"5.88.1.4.1.3","2.1.2.1.1.2","2.1.2.2.1.2","2.1.2.3.1.1.9"};
    char buf_ps[128]="";
    char is_snmpd_ok=0;    
    //char ps[512]={0};
    FILE *ptr;
    //strcpy(ps, cmd);
    if (current_oid<4)
    {
       sprintf(buf_ps,"snmpget -m ALL -Ov -c public localhost %s%s.0.%d",OID,head_oids[current_oid],getSlotOfNmu(0,30));
     if (current_oid<1)
       strcat(buf_ps,".8.1");
    }
    else
       sprintf(buf_ps,"snmpget -m ALL -Ov -c public localhost 1.3.6.1.4.1.10215.2.1.5.88.1.1.1.1.0");
    //printf("%s\n",buf_ps);
    //if((ptr=popen("snmpget -m ALL -Ov -c public localhost 1.3.6.1.4.1.10215.2.1.5.88.1.1.1.1.0", "r"))!=NULL)
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
      snmp_start_count=0;
      if (current_oid<4)
       current_oid++;
      else
       current_oid=0;
      //printf("is_snmpd_ok %d\n",current_oid);
    }
    else
    {
       if (snmp_start_count>3)
       {
         sendAlarm(1,0,getSlotOfNmu(0,30),0);
         sleep(2);
         if (is_save_log)
            save_log("reboot");
         system("reboot");
       }
       else
       {
           sendAlarm(1,0,getSlotOfNmu(0,30),0);
           sleep(1);
          if (snmp_start_count<20)
           snmp_start_count++;
          system("killall -9 snmpd");
          if (is_save_log)
             save_log("snmpd");
          system(SNMPD_SERVICE);
          sleep(30);
          if (start_count<50)
           start_count++;
       }
    }
      /*
      pid=getPidByName("pidof snmpd");
      if (pid<1)
      {
         printf("Starting snmpd...\n");
         system(SNMPD_SERVICE);
         start_count++;
      }
      */
     // else
     //    printf("snmpd running pid=%d\n",pid);
  }
  else if (CCENTER_SERVICE_ID==proc) //restar ccenter
  {
      
      pid=getPidByName("pidof ccenter");
      if (pid<1)
      {
         if (ccenter_restart_count<5)
         {
          printf("Restarting ccenter...\n");
          if (is_save_log)
             save_log("ccenter");
          system("ccenter &");
          sleep(20);
          ccenter_restart_count++;
          start_count++;
         }
         else
         {
          //ccenter_restart_count=0;
          sendAlarm(1,0,getSlotOfNmu(0,30),0);
          sleep(1);
          if (is_save_log)
             save_log("reboot");
          system("reboot");
         }
      }
      
      /*printf("Restarting ccenter...\n");
      system("killall -9 ccenter");
      
      sleep(1);
      system("./ccenter&");
      */
    /*
    if (io_get_err>9)
      system("reboot");
    else if (io_get_err>2)
    {
      system("killall -9 ccenter");
      sleep(1);
      system("ccenter&");
    }
    if (io_get_err<100)
      io_get_err++;
    */
  }
 else if (OLP_SERVICE_ID==proc)
 {
     pid=getPidByName("pidof olputil");
      if (pid<1)
      {
         if (is_save_log)
             save_log("olputil");
          system("olputil &");
          sleep(20);
          start_count++;
      }
 }
 if (start_count>10)
 {   
   snmp_start_count=0;
   sendAlarm(1,0,getSlotOfNmu(0,30),0);
   system("reboot");
 }
} 
static void sigHandle(int sig,struct siginfo *siginfo,void *myact)
{

  //printf("sig=%d siginfo->si_int=%d SIGALRM=%d,SIGSEGV=%d\n",sig,siginfo->si_int,SIGALRM,SIGSEGV);
  if (sig == SIGALRM)
  {
    float sys_usage, user_usage,mem_usage;
    struct mem_usage_t memery;
    if (!getCpuUsage(&sys_usage,&user_usage))
    {
      //printf("CPU usage user:%.1f%%\tsys:%.1f%%\n",user_usage,sys_usage);
      
      if ((sys_usage+user_usage)>95)
      {
       printf("CPU usage user:%.1f%%\tsys:%.1f%% Over!\n",user_usage,sys_usage);
       sendAlarm(3,0,getSlotOfNmu(0,30),0);
       system("reboot");
      }
    }
    mem_usage=getMemUsage(&memery);
    //printf("%lu %lu %lu %lu %lu %lu\n",memery.total,memery.used,memery.free,memery.shared,memery.buffers,memery.cached);
    //printf("Mem usage:%.1f%%\n",mem_usage);
    if (mem_usage>95)
    {
       printf("%lu %lu %lu %lu %lu %lu\n",memery.total,
	memery.used,memery.free,memery.shared,memery.buffers,memery.cached);
       printf("Mem usage:%.1f%% over!\nreboot...\n",mem_usage);
       sendAlarm(5,0,getSlotOfNmu(0,30),0);
       system("reboot");
    }
    startService(SNMPD_SERVICE_ID);
    startService(WEBS_SERVICE_ID);
    startService(CCENTER_SERVICE_ID);
    if (is_run_olp)
       startService(OLP_SERVICE_ID);
    if (start_count<3)
       alarm(30);
    else
    {  
      if ( start_count<10)
        alarm(20);
      else
      {
        sendAlarm(3,0,getSlotOfNmu(0,30),0);
        system("reboot");
      }
    }
  }
  else if (sig == SIGSEGV)
  {
    sleep(1);
    startService(siginfo->si_int);
  }
}

static void sigInit()
{
  int i;
  struct sigaction act;
  sigemptyset(&act.sa_mask);  
  act.sa_flags=SA_SIGINFO;
  act.sa_sigaction=sigHandle;

  sigaction(SIGALRM,&act,NULL);
  //sigaction(SIGUSR1,&act,NULL);
  /*
  sigaction(SIGINT,&act,NULL);
  sigaction(SIGQUIT,&act,NULL);
  sigaction(SIGTSTP,&act,NULL);
  sigaction(SIGTTIN,&act,NULL);
  sigaction(SIGTERM,&act,NULL);
  sigaction(SIGKILL,&act,NULL);
  sigaction(SIGUSR1,&act,NULL);
  sigaction(SIGUSR2,&act,NULL);
  sigaction(SIGRTMIN,&act,NULL);
  */
  sigaction(SIGSEGV,&act,NULL);
}
static void usage()
{
  printf("Ver 1.0.6\nLast modified:2014-04-21\n\n");
  printf("Usage:watchdogd [option] \n");
  printf("Option Specification:\n");
  printf("-e: run english webs\n");
}
static void init()
{
  char value[10]="";
  getConfValue(SYS_CONF_FILE,"olputil",value," ");
  if (value[0]=='Y')
     is_run_olp=1;
  getConfValue(SYS_CONF_FILE,"save_err",value," ");
  if (value[0]=='Y')
   is_save_log=1;
}
int main(int argc, char **argv)
{
  int oc;
  while((oc = getopt(argc, argv, "eh")) != -1)
  {
     switch(oc)
     {
       case 'e':
         strcpy(lang,"en");
       break;
       case 'h':
         usage();
       exit(0);
     }
  }
  if (lockFile("/vispace/wd.lck")<0)
  {
     exit(0);
  }
  init();
  sigInit();
 
  alarm(30);
  getSlotOfNmu(0,60);
  /* Most of time it goes to sleep */
  while (1)
  {	
    pause();
  }
  return 1;
}
