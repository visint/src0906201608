#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <time.h>
#include <sys/time.h>
#include <unistd.h>
#include <errno.h>  
#include <sys/msg.h> 
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sysinfo.h>

#include <unit.h>
#include <visipc.h>
#include <chassis.h>
#include <public.h>
#include <log.h>
#include <dbutil.h>
#include <alarm.h>

//arm-linux-gcc -o alarmd alarmd.c -I./../include -L/home/visint/src/arm/lib -lvispace -liconv -lsqlite3
#define MAX_ALARM_COUNT 50
static int alarm_count=0;
static char has_new_alarm=0;
static char hb_enable=0;
static alarm_info_t alarms[MAX_ALARM_COUNT];

void heart_beat_message(short value,char *message)
{
  int n;
  short val;
  //printf("heart_beat_message\n");
  strcpy(message,"");
  char buf[8]="";
  for (n=0;n<16;n++)
  {
    val=value>>n;
    val&=0x1;
    if (val!=0)
    {
      sprintf(buf,"%d,",n+1);
      strcat(message,buf);
    }
  }
  n=strlen(message);
  if (n>1)
   message[n-1]='\0';
}
void save_alarm()
{
  int n=0;
   sqlite3 *db=NULL;
   sqlite3_open(DB_FILE_NAME, &db);
   if (db!=NULL)
   {
    
    //printf("alarm count: %d\n",alarm_count);
   // handleAlarm  (db,alarm.alarm_code,alarm.chassis,alarm.slot,alarm.entity,alarm.is_with_value,alarm.alarm_value,alarm.alarm_reason,alarm.alarm_time,alarm.value2message);
   for (n=0;n<alarm_count;n++)
   {
      //printf("#%d alarm code: %d is_with_value=%d time=%d\n",alarms[n].slot,alarms[n].alarm_code,alarms[n].is_with_value,alarms[n].alarm_time);
      if (alarms[n].alarm_code==19 || alarms[n].alarm_code==20)
      {
       handleAlarm(db,alarms[n].alarm_code,alarms[n].chassis,alarms[n].slot,alarms[n].entity,alarms[n].is_with_value,alarms[n].alarm_value,alarms[n].alarm_reason,alarms[n].alarm_time,heart_beat_message);
      }
      else 
         handleAlarm(db,alarms[n].alarm_code,alarms[n].chassis,alarms[n].slot,alarms[n].entity,alarms[n]. is_with_value,alarms[n].alarm_value,alarms[n].alarm_reason,alarms[n].alarm_time,NULL);
   }
    has_new_alarm=0;
    alarm_count=0;
   sqlite3_close(db);
  }
}
void sigHandler1(int sig,struct siginfo *psiginfo,void *act)
{
  printf("sigHandle:sig=%d\n",sig);
 init_dest_host();
}
void sighandler(int signo, siginfo_t *info,void *ctx)
{
   if (SIG_DEST_HOST_MODIFIED_ID==info->si_int)
     init_dest_host();
    //以下两种方式都能获得sigqueue发来的数据
    //printf("receive the data from siqueue by info->si_int is %d\n",info->si_int);
    //printf("receive the data from siqueue by info->si_value.sival_int is %d\n",info->si_value.sival_int);

}
void sigHandle(int sig,struct siginfo *psiginfo,void *act)
{
  // printf("sigHandle:alarm_count=%d has_new_alarm=%d\n",alarm_count,has_new_alarm);
  if (alarm_count<1)
    return;
  
  if (has_new_alarm)
  {
    has_new_alarm=0;
    alarm(5);
    return;
  }
  
  if (alarm_count<MAX_ALARM_COUNT && !has_new_alarm ||alarm_count==MAX_ALARM_COUNT)
  {
    save_alarm();
  }
    /*
    for (n=0;n<alarm_info_count;n++)
    {
      addAlarm(alarms[n].alarm_code,alarms[n].chassis,alarms[n].slot,alarms[n].entity,alarms[n]. is_with_value,alarms[n].alarm_value,alarms[n].alarm_reason,alarms[n].alarm_time,alarms[n].value2message);
    }
    has_new_alarm=0;
    alarm_info_count=0;
    */
  else if (has_new_alarm)
  {
    has_new_alarm=0;
    alarm(5);
    return;
  }
  //alarm(20);
}


void alarmrcv()
{
   //int running = 1;  
   // int msgid = -1;  
    alarm_msg_t data;  
    long int msgtype = 1; //注意1  
    int buf_size=sizeof(alarm_info_t);
    //建立消息队列  
    //msgid = msgget((key_t)1234, 0666 | IPC_CREAT);  
    /*
     if(msgid == -1)  
    {  
        fprintf(stderr, "msgget failed with error: %d\n", errno);  
        exit(EXIT_FAILURE);  
    }
   */  
    //从队列中获取消息，直到遇到end消息为止  
    while(1)  
    {  
        if(msgrcv(almmsgid, (void*)&data, buf_size, msgtype,0) != -1)  
        {  
            //fprintf(stderr, "msgrcv failed with errno: %d\n", errno);  
            //exit(EXIT_FAILURE); 
            
            alarm_info_t *pai=(alarm_info_t*)data.alarm_info;
            // alarm_count++;
            
            if (pai->alarm_code<1)
               continue;
             if (debug.is_debug_mode==1)
               printf("AlarmRcv: #%d entity=%d alarm_count=%d alarm_code=%d alarm_reason=%d time=%d\n",pai->slot,pai->entity,alarm_count,pai->alarm_code,pai->alarm_reason,pai->alarm_time);
            if (pai->alarm_code==501 && hb_enable==1/* ||pai->alarm_code==52*/)
             {
                  int val,slot;
                  slot=pai->slot;
                  slot=slot<<24;
                  //val=pai->entity;
                  //val=val<<2;
                  val=slot|pai->alarm_code;
                  //printf("pai->slot=%d slot=%d val=%d pai->alarm_code=%d\n",pai->slot,slot,val,pai->alarm_code);
                  sendSigToProccess("heartbeat",SIGINT,val);
                  //sendSig(hb_pid,SIGINT,val);
                 // printf("%d pai->slot=%d val=%d\n",sizeof(int),pai->slot,val);
             }
            if (pai->alarm_reason<0 || pai->alarm_reason>100)
                pai->alarm_reason=0;
            if (pai->alarm_code==19 || pai->alarm_code==20)
            {
             addAlarm(pai->alarm_code,pai->chassis,pai->slot,pai->entity,pai->is_with_value,pai->alarm_value,pai->alarm_reason,pai->alarm_time,heart_beat_message);
           }
           else 
             addAlarm(pai->alarm_code,pai->chassis,pai->slot,pai->entity,pai->is_with_value,pai->alarm_value,pai->alarm_reason,pai->alarm_time,NULL);
             
             usleep(100000);
            /*
            if (alarm_count<MAX_ALARM_COUNT)
            {
               alarms[alarm_count].alarm_code=pai->alarm_code;
               alarms[alarm_count].chassis=pai->chassis;
               alarms[alarm_count].slot=pai->slot;
               alarms[alarm_count].entity=pai->entity;
               alarms[alarm_count].is_with_value=pai->is_with_value;
               alarms[alarm_count].alarm_value=pai->alarm_value;
               alarms[alarm_count].alarm_reason=pai->alarm_reason;
               alarms[alarm_count].alarm_time=pai->alarm_time;
               //alarms[alarm_count].value2message=data.value2message;
               alarm_count++;
               if (alarm_count<MAX_ALARM_COUNT)
               {
                   has_new_alarm=1;
               
                   //if (alarm_count<2)
                   //  alarm(5);
               }
               else {
                //save_alarm();
                alarm_count=0;
                printf("MAX_ALARM_COUNT: #%d alarm_count=%d alarm_code=%d\n",pai->slot,alarm_count,pai->alarm_code); 
              }
            }
            */
        }
        else
         {
           //printf("Alarmrcv fail!errno=%d EINTR=%d\n",errno,EINTR);
           system("logger -p 1 -t vierr \"alamd fail\"");
           if (EINTR == errno)
           {
              continue;
           }
           else
              exit(0);
        }
    } 
}
void usage()
{
  printf("Description:\n");
  printf("alarmd.\nver 1.1.20\nLast modifed:%s\n",LAST_MODIFY_DATE);
  printf("-d for debug.\n");
  printf("-h for help.\n");
}
void init_alatrmd(int argc, char **argv)
{
  int oc;
  char value[16]="";
  //int waitting=-1;
  while((oc = getopt(argc, argv, "dh")) != -1)
  {
        switch(oc)
        {
          case 'd':
               debug.is_debug_mode=1;
             break;
          case 'h':
             usage();
             exit(0);
        }
  }
  if (lockFile("/vispace/ad.lck")<0)
  {
     exit(0);
  }
  initAlarmConf();
  getConfValue(SYS_CONF_FILE,"hb_enable",value," ");
  if (value[0]=='Y')
  {
   hb_enable=1;
   /*
   int n;
   for (n=0;n<20;n++)
   {
     hb_pid=findPid("heartbeat");
     if (hb_pid>0)
         break;
   }
   */
   //printf("hb_pid=%d\n",hb_pid);
  }
  init_dest_host();
  init_alarm_type();
struct sigaction act;
    act.sa_sigaction = sighandler;
    sigemptyset(&act.sa_mask);
    act.sa_flags = SA_SIGINFO;//信息传递开关
    if(sigaction(SIGINT,&act,NULL) == -1){
        perror("sigaction error");
        exit(EXIT_FAILURE);
    }

 /*
  struct sigaction act;
  sigemptyset(&act.sa_mask);  
  act.sa_flags=SA_SIGINFO;
  act.sa_sigaction=sigHandler;
  sigaction(SIGALRM,&act,NULL);
 */
}
int main(int argc, char **argv)
{
  init_alatrmd(argc,argv);
  alarmrcv();
}
