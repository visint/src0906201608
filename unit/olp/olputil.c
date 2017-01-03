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
#include <olp.h>
#include <public.h>
#include <infoset.h>
#include <io.h>
#include <log.h>
//arm-linux-gcc -o olputil olputil.c -I./../../include -L/home/visint/src/arm/lib -lvispace -liconv -lsqlite3
//static unsigned int count=0;
static unsigned int fail_count=0,count=0;
static unsigned char continuous_fail=0;
static uchar_t slotOfNmu=16;
static char is_debug=0;
static unsigned int log_time=30;
char sfp1_last_satus=0,sfp2_last_satus=0;

static char allow_ok_alarm=0;
static char allow_fail_alarm=0;
static char allow_ok_log=0;
static char allow_fail_log=0;
static void usage()
{
  printf("Description:\n");
  printf("OLP with backup fiber inspection.\nver 1.0.18\nLast modified:%s\n\n",LAST_MODIFY_DATE);
  printf("-d for debug.\n");
  printf("-l for logging.\n");
  printf("-h for help.\n");
}
char is_break=0;
static void setLastTime()
{
 time_t now;
  struct tm  *local;
  time(&now);
  local = localtime(&now);
  if (local!=NULL)
  {
    char lasttime[30]="0";
    sprintf(lasttime,"%4d-%02d-%02d|%02d:%02d:%02d",local->tm_year+1900,local->tm_mon+1,local->tm_mday,local->tm_hour,local->tm_min,local->tm_sec);
    setConfValue(SYS_CONF_FILE,"lasttime",lasttime," ");
  }
}
static void sigHandle(int sig,struct siginfo *siginfo,void *myact)
{
 // printf("OLP sigHandle\r\n");
  //if (is_break)
  //  exit(0);
  if (sig == SIGALRM)
  {
    uchar_t n,m,uc,ut,sfp1,sfp2,have_olp=0;
    nmuStatus_t *nmu_status;
    char message[512];
   
    if (continuous_fail>20)
    {
       sendAlarm(113,0,getSlotOfNmu(0,5),0,0,0);
       printf("reboot(olp)...\r\n");
       system("reboot"); 
    }
    slotOfNmu=getSlotOfNmu(0,3);
    if (slotOfNmu<2 ||slotOfNmu>16)
    {
    	  slotOfNmu=16;
    	  /*
        continuous_fail++;
        alarm(5);
        return;
        */
    }
    nmu_status=(nmuStatus_t*)getInfoSetValues(0,slotOfNmu,8,1);
    if (NULL==nmu_status)
    {
     continuous_fail++;
     alarm(5);
     return;
    }

    if (nmu_status->sfp1_exist!=2 && nmu_status->sfp1_status!=2)
    {
        sfp1=1;
    }
    else
        sfp1=2;
   
    if (nmu_status->sfp2_exist!=2 && nmu_status->sfp2_status!=2)
    {
        sfp2=1;
    }
    else
        sfp2=2;
   if (sfp1_last_satus!=sfp1)
   {
     if (sfp1_last_satus!=0)
     {
       if (allow_ok_alarm==1 && sfp1==1)
        sendAlarm(18,slotOfNmu,1,0,0,0);
       else if (allow_fail_alarm==1 && sfp1==2)
        sendAlarm(17,slotOfNmu,1,0,0,0);
     }
     sfp1_last_satus=sfp1;
   }
   if (sfp2_last_satus!=sfp2)
   {
     if (sfp2_last_satus!=0)
     {
       if (allow_ok_alarm==1 && sfp2==1)
        sendAlarm(18,slotOfNmu,2,0,0,0);
       else if (allow_fail_alarm==1 && sfp2==2)
        sendAlarm(17,slotOfNmu,2,0,0,0);
     }
     sfp2_last_satus=sfp2;
   }
    for (n=1;n<slotOfNmu;n++)
    {
       if (getUnitClassType(0,n,&uc,&ut)!=FALSE)
       {
          //printf("#%d uc=%d UC_OLP%d\r\n",n,uc,UC_OLP);
          if (uc==UC_OLP)
          {
            memset(message,0,512*sizeof(char));
            message[0]='S';
            message[1]=OLP_STATUS_INFOSET_ID;
            message[2]=1;
            message[3]=30;
            message[4]=0;
            message[5]=sfp1;
            message[6]=31;
            message[7]=0;
            message[8]=sfp2;
            message[9]=0;
            
            if (sendMessageToUnit(0,n,message,300,0,0,0)!=EXIT_SUCCESS)
            {
             fail_count++;
             continuous_fail++;
             if(is_debug==1)
                  printf("Send sfp status(sfp1=%d,sfp2=%d) to OLP #%d;Fail count=%d continuous fail=%d\n",sfp1,sfp2,n,fail_count,continuous_fail);
             
             //printf("Send sfp status to OLP #%d; Count=%d Fail count=%d\n",n,count,fail_count);
            }
            else
            {
                continuous_fail=0;
                if(is_debug==1)
                  printf("Send sfp status(sfp1=%d,sfp2=%d) to OLP #%d OK;count of fail=%d\n",sfp1,sfp2,n,fail_count);
            }
            sleep(1);
            /*
            if (have_olp!=1)
            {
               if (count<log_time)
                count++;
               else
               {
                 count=0;
                 if (continuous_fail<2 && allow_ok_log)
                   insertOperateLog(LOG_OLP_WD_OK,0,0,System,unkown_opt,0,0);
                 else if (allow_fail_log)
                   insertOperateLog(LOG_OLP_WD_FAIL,0,0,System,unkown_opt,n,0);
               }
               have_olp=1;
            }
            */
          }
       }
    }
    if (continuous_fail>20 || fail_count>100)
    {
        //if (allow_fail_log==1)
        //   insertOperateLog(LOG_OLP_WD_FAIL,0,0,System,unkown_opt,n,0);
        //sendAlarm(113,0,getSlotOfNmu(0,5),0,0,0);
        printf("reboot(olpd)...\r\n");
        setLastTime();
        system("reboot");
    }
   alarm(20);
  }
  else if (sig == SIGSEGV)
  {
      is_break=1;
  }
}
static void sigInit()
{
  struct sigaction act;
  sigemptyset(&act.sa_mask);  
  act.sa_flags=SA_SIGINFO;
  act.sa_sigaction=sigHandle;

  sigaction(SIGALRM,&act,NULL);
  //sigaction(SIGSEGV,&act,NULL);
  //sigaction(SIGSEGV,&act,NULL);
}
void main(int argc, char **argv)
{
  int oc;
  is_debug=0;
  while((oc = getopt(argc, argv, "dl:h")) != -1)
  {
        switch(oc)
        {
          case 'd':
             is_debug=1;
             //is_debug_io=1;
             break;
          case 'l':
             log_time=atoi(optarg);
             if (log_time<3)
                log_time=3;
             break;
          case 'h':
             usage();
             exit(0);
        }
 }
  if (lockFile("/vispace/olpd.lck")<0)
  {
     exit(0);
  }
  slotOfNmu=getSlotOfNmu(0,3);
  if (!allow_ok_log)
  {
    if (getRowCount(NULL,"select count(*) FROM AlarmType WHERE alarm_code=17 AND (enable=1 OR is_save=1")>0)
         allow_fail_alarm=1;
    if (getRowCount(NULL,"select count(*) FROM AlarmType WHERE alarm_code=18 AND (enable=1 OR is_save=1")>0)
         allow_ok_alarm=1;
    if (getRowCount(NULL,"select count(*) FROM OptObjGroup WHERE ID=101 AND enable=1")>0)
         allow_ok_log=1;
    if (getRowCount(NULL,"select count(*) FROM OptObjGroup WHERE ID=102 AND enable=1")>0)
         allow_fail_log=1;
  }
  sigInit();
  alarm(10);
  //numOnslot=getSlotOfNmu(0,0);
  /* Most of time it goes to sleep */
  while (1)
  {	
    pause();
  }
}

