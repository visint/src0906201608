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
#define SNMPD_SERVICE_ID 2
#define WEBS_SERVICE_ID 3
#define CCENTER_SERVICE_ID 9
#define OLP_SERVICE_ID 10
#define HEART_BEAT_SERVICE_ID 11
#define ALARM_SERVICE_ID 12
#define UDP_SERVICE_ID 13

//#define WEBS_SERVICE "/vispace/webs/webs&"
//#define WEBS_PATH "/home/visint/VisProj/src/webs/LINUX"
#define WEBS_PATH "/vispace/webs"
#define SNMPD_SERVICE "snmpd -c /vispace/snmp/snmpd.conf"
#define CLID_SERVICE "/vispace/webs/webs&"
//gcc -o watchdogd watchdog.c -I./../../include -L./../../linuxlib -lvispace -liconv -lsqlite3


//arm-linux-gcc -o watchdogd watchdog.c -I./../include -L/home/visint/src/arm/lib -lvispace -liconv -lsqlite3
//static char is_debug_mode=0;
static unsigned short start_count=0,ccenter_restart_count=0;
static unsigned short webs_start_count=0;
static unsigned short snmp_start_count=0,snmp_fail_count=0;
static char io_get_err=0;
static char lang[3]="";
static char is_save_log=0,is_run_olp=0,is_run_heartbeat=0,is_run_udp=0;
static char timer_start=0;
static char is_save_sys_log=0;
/*
static void save_log(char *proccess)
{
 if (is_save_log)
 {
    time_t tt;
    time(&tt);
    doSystem("sed -i '1a start %s by wd at %s\n' /vispace/err.log",proccess,ctime(&tt));
 }  
}
*/
static char current_oid=0;
static char snmp_restart_reason=0;
char ping()
{
  char buf_ps[512]="";
  char ping_ok=1;    
    //char ps[512]={0};
   FILE *ptr;
   if((ptr=popen("ping 127.0.0.1 -c 1", "r"))!=NULL)
   {
       while(fgets(buf_ps, 510, ptr)!=NULL)
       {
           if (strstr(buf_ps,", 0 received"))
           {
               ping_ok=0;
               printf("%s\n",buf_ps);
               break;
           }
          // else
          //     printf("%s\n",buf_ps);
       }
      pclose(ptr);
   }
   else
       ping_ok=0;
   return ping_ok;
}

int getProcessUsage(char* proc_name)
{
  char buf_ps[128]="";
  //char status=1;    
    //char ps[512]={0};
  int process_usage=0;
   sprintf(buf_ps,"top -n 1| grep %s",proc_name);
   //printf("top=%s\n",buf_ps);
   FILE *ptr;
   if((ptr=popen(buf_ps, "r"))!=NULL)
   {
       if(fgets(buf_ps, 126, ptr)!=NULL)
       {
           char *p;//=strstr(buf_ps,"%");
           //char *pos;
           //printf("\nbuf_ps=%s proc_name:%s\n",buf_ps,proc_name);
           p=strtok(buf_ps," ");
           if (p!=NULL)
           {
             //pos=p;
             int n;
             //printf("p!=NULL\n");
             char usage[16]="";
             for (n=0;n<4;n++)
             {
                 p=strtok(NULL," ");
                 if (p==NULL)
                    break;
                // else
                 //   printf("P=%s\n",p);
             }
             for (n=0;n<2;n++)
             {
                 p=strtok(NULL," ");
                 if (p!=NULL)
                 {
                    strcpy(usage,p);
                    if (strlen(usage)>1)
                      usage[strlen(usage)-1]='\0';
                    process_usage=atoi(usage);
                    if (process_usage>30)
                    {
                         //status=0;
                         break;
                    }
                    //printf("p=%s ",usage);
                 }
                 else
                    break;
             }
           }
       }
      pclose(ptr);
   }
   return process_usage;
}
void updateDate()
{
  /*
  char value[60]="";
  getConfValue(SYS_CONF_FILE,"timesource",value," ");
  //printf("updateDate %s\n",value);
  if (value[0]=='2')
  {
    char buf[60]="";
    getConfValue(SYS_CONF_FILE,"ntpserver",value," ");
    //printf("updateDate from ntp %s..\n",value);
    system("killall ntpdate");
    sprintf(buf,"ntpdate -u %s &",value);
    printf("begin ntp update ...\n");
    system(buf);
    //doSystem("ntpdate -u %s &",value);
    system("hwclock -w");
    printf("after ntp update ...\n");
  }
  */
  //printf("updateUnitDate ...\n");
  updateUnitDate(1);
  sleep(50);
}

void sendNmuIpToUnit(unsigned char retry)
{
 uchar_t n,max_slot,uclass,utype;
 uchar_t slot_count=0,slots[16];
 //char message[512];
 
 max_slot=getSlotOfNmu(0,0);
 //printf("max_slot=%d\n",max_slot);
 if (max_slot<2 || max_slot>16)
    return;
 for (n=0;n<max_slot;n++)
 {
    if (TRUE!=getUnitClassType(0,n,&uclass,&utype))
          continue;
    if (uclass==UC_OLP && utype>40 && utype<50)
    {
      slots[slot_count]=n;
      slot_count++;
    }
 }
 //printf("slot_count=%d\n",slot_count);
  if (slot_count>0)
  {
     char message[512];
     memset(message,0,512*sizeof(char));
     message[0]='S';
     message[1]=UNIT_PROPERTY_INFOSET_ID;
     message[2]=1;
     message[3]=11;
   //message[4]=0;
     char ip_add[30];
     getDeviceIp(ip_add,NULL,NULL);
     int ip=(int)inet_addr(ip_add);
     //printf("ip=%d\n",ip);
     memcpy(message+4,&ip,4);
     ioBroadcast(0,slot_count,slots,message,retry);
  }
}
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
static void reBoot(const char *message)
{
  setLastTime();
  doSystem("logger -p 1 -t vierr \"%s\"",message);
  /*system("tail -20 /var/log/messages|grep vierr>>/var/log/temp");
  if (access("/vispace/sys.log",W_OK) !=0)
      system(":>/vispace/sys.log");
  */
  system("tail -20 /var/log/messages|grep vierr>>/vispace/sys.log");
  printf("%s\n",message);
  system("reboot -n -f");
}
static void startService(int proc)
{
  pid_t pid;
  //printf("startService:%d\r\n",proc);
  if (WEBS_SERVICE_ID==proc)
  {
     pid=getPidByName("pidof webs");
     if (pid<1)
     {
        if (start_count<50)
         start_count++;
        //if (is_debug_mode)
          printf("Restartting webs...\n");
        /*
        if (is_save_log)
             save_log("webs");
        */
        doSystem("cd %s && ./webs&",WEBS_PATH);
        sendAlarm(107,0,getSlotOfNmu(0,5),0,0,0,0,0,NULL);
        sleep(10);
        //system(WEBS_SERVICE);
    }
    else
    {
       int process_usage=getProcessUsage("webs");
       if (process_usage>10)
       {
          if (process_usage>80)
          {
            printf("Restartting webs(%d)...\n",process_usage);
            doSystem("kill -9 %ld",pid);
            doSystem("cd %s && ./webs&",WEBS_PATH);
            reBoot("webs usage more than 80%");
          }
          else if (debug.is_debug_mode==1)
              printf("webs CPU usage(%d)...\n",process_usage);
       }
    }
  }
  else if (SNMPD_SERVICE_ID==proc)
  {
    pid=getPidByName("pidof snmpd");
    if (pid<1)
    {
       printf("Restartting snmpd:snmpd was killed...\n");
       if (is_save_sys_log)
         insertSysLog(SYS_LOG_SNMP_KILLED,"snmpd killed");
       system(SNMPD_SERVICE);
       sleep(90);
       sendAlarm(105,0,getSlotOfNmu(0,1),0,0,0,0,0,NULL);
 
       if (snmp_start_count<10)
         snmp_start_count++;
       if (start_count<10)
         start_count++;
       return;
    }
    #define OID "1.3.6.1.4.1.10215.2.1."
    #define InfoSetHeadOid "5.88.1.4.1.3"
    #define HardwareVerHeadOid "2.1.2.1.1.2" //vsModuleBaseInfoTable/vsModuleHardwareVer
    #define ModuleNameHeadOid "2.1.2.2.1.2"  //vsModuleUserDataTable/vsModuleName
    #define FactoryDefaultHeadOid "2.1.2.3.1.1.9"   //vsNmuTable/vsNmuFactoryDefault

    char head_oids[][16]={InfoSetHeadOid,HardwareVerHeadOid,ModuleNameHeadOid,FactoryDefaultHeadOid};

    char buf_ps[128]="";
    char is_snmpd_ok=0;    
    //char ps[512]={0};
    FILE *ptr;
    //strcpy(ps, cmd);
    //printf("\nSlotOfNmu:%d\n",getSlotOfNmu(0,10));
    if (current_oid<4)
    {
       sprintf(buf_ps,"snmpget -m ALL -Ov -c public localhost %s%s.0.%d",OID,head_oids[current_oid],getSlotOfNmu(0,2));
     if (current_oid<1)
     {
       strcat(buf_ps,".8.1");
      // printf("\n%s\n",buf_ps);
     }
    }
    else
       sprintf(buf_ps,"snmpget -m ALL -Ov -c public localhost 1.3.6.1.4.1.10215.2.1.5.88.1.1.1.1.0");
       // vsUnitClass
   // if (debug.is_debug_mode)
    // printf("%s\n",buf_ps);

    if((ptr=popen(buf_ps, "r"))!=NULL)
    {    
        while(fgets(buf_ps, 32, ptr)!=NULL)
        {    
             if (current_oid==0 || current_oid==3)
             {
               if (strstr(buf_ps,"INTEGER"))
               {
                  is_snmpd_ok=1;
                  break;
               }
             }             
             else if (strstr(buf_ps,"STRING")||strlen(buf_ps) <6) 
             {
                  is_snmpd_ok=1;
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
    }
    else
    {
    	/*
       if (debug.is_debug_mode)
           printf("snmp get fail: %d:snmp_fail_count=%d,snmp_start_count=%d\n",current_oid,snmp_fail_count,snmp_start_count);
       if (debug.max_err_count>0)
       {
          if (current_oid<4)
          {
            sprintf(buf_ps,"snmpget -m ALL -Ov -c public localhost %s%s.0.%d",OID,head_oids[current_oid],getSlotOfNmu(0,1));
            if (current_oid<1)
              strcat(buf_ps,".8.1");
          }
          else
            sprintf(buf_ps,"snmpget -m ALL -Ov -c public localhost 1.3.6.1.4.1.10215.2.1.5.88.1.1.1.1.0");
          if((ptr=popen(buf_ps, "r"))!=NULL)
          {    
           while(fgets(buf_ps, 128, ptr)!=NULL)
           {
              printf("snmnpget:%s\n",buf_ps);
              saveError("@%d:%s",current_oid,buf_ps);
           }
          pclose(ptr);    
              ptr = NULL; 
         }
       }
       */
       
       if (snmp_fail_count>4 && snmp_fail_count%5==0 ||snmp_start_count%3==2)
       {
        // printf("current_oid: %d snmp_start_count:%d snmp_fail_count=%d is_snmpd_ok=%d is_save_sys_log=%d\n",current_oid,snmp_start_count,snmp_fail_count,is_snmpd_ok,is_save_sys_log);
         if (is_save_sys_log)
         {
            if (ping())
            {
               float sys_usage=0, user_usage=0,mem_usage=0;
               struct mem_usage_t memery;
               mem_usage=getMemUsage(&memery);
               getCpuUsage(&sys_usage,&user_usage);
               addSysLog(SYS_lOG_SNMP_TIMEOUT,"snmpd(%d/F:%d M:%.1f C:%.1f) timeout!CT=%d NS=%d",current_oid,snmp_fail_count,mem_usage,sys_usage+user_usage,getChassisClass(0),getSlotOfNmu(0,0));
               if (debug.max_err_count>0)
               {
                if (current_oid<4)
                {
                  sprintf(buf_ps,"snmpget -m ALL -Ov -c public localhost %s%s.0.%d",OID,head_oids[current_oid],getSlotOfNmu(0,2));
                  if (current_oid<1)
                    strcat(buf_ps,".8.1");
                }
                else
                  sprintf(buf_ps,"snmpget -m ALL -Ov -c public localhost 1.3.6.1.4.1.10215.2.1.5.88.1.1.1.1.0");
  
               if((ptr=popen(buf_ps, "r"))!=NULL)
               {    
                while(fgets(buf_ps, 128, ptr)!=NULL)
                 saveError(buf_ps);
                pclose(ptr);    
                ptr = NULL; 
               }
              }
            else
            {
               insertSysLog(SYS_LOG_PING_FAIL,"ping fail");
               printf("ping timeout\n");
            }
          }
        }
      }    
       //snmp_fail_count++;
       //printf("current_oid:%d snmp_start_count:%d snmp_fail_count=%d\n",current_oid,snmp_start_count,snmp_fail_count);
       if (snmp_start_count>4)
       {
         sendAlarm(115,0,getSlotOfNmu(0,2),0,0,0,0,0,NULL);
         sleep(2);
         /*
           if (is_save_log)
            save_log("reboot");
         */
        // printf("is_snmpd_fail %d snmp_start_count:%d reboot...\n",current_oid,snmp_start_count);
         //printf("reboot:snmpd timeout too much...\n");
         if (!debug.is_debug_mode)
         {
           //printf("reboot because snmpd die too much...\n");
           //setLastTime();
           //system("reboot");
           reBoot("snmp timeout");
         }
       }
       else if (snmp_fail_count>4)
       {
          if (snmp_fail_count%5==0)
          {
           sleep(1);
           //snmp_fail_count=0;
           printf("Restartting snmpd...\n");
           if (!debug.is_debug_mode)
           {
            system("killall -9 snmpd");
            system(SNMPD_SERVICE);
            sendAlarm(105,0,getSlotOfNmu(0,2),0,0,0,0,0,NULL);
            sleep(90);
           }
           else if (snmp_start_count%3==2)
           {
               sendAlarm(105,0,getSlotOfNmu(0,2),0,0,0,0,0,NULL);
               //snmp_fail_count=0;
           }
           if (snmp_start_count<20)
            snmp_start_count++;
          //if (start_count<50)
           start_count++;
          }
          //snmp_fail_count=0;
       }
       if (snmp_fail_count>9)
           snmp_fail_count=0;
       else
           snmp_fail_count++;
   }
   if (current_oid<4)
      current_oid++;
   else
      current_oid=0; 
  }
  else if (ALARM_SERVICE_ID==proc)
  {
     pid=getPidByName("pidof alarmd");
      //printf ("pidof ccenter=%d\r\n",pid);
      if (pid<1)
       {
         system("alarmd &");
         sleep(20);
       }
  }
  else if (CCENTER_SERVICE_ID==proc) //restar ccenter
  {
      
      pid=getPidByName("pidof ccenter");
      //printf ("pidof ccenter=%d\r\n",pid);
      if (pid<1)
      {
         if (ccenter_restart_count<5)
         {
          printf("Restartting ccenter...\n");
          if (!debug.is_debug_mode)
          {
            system("ccenter &");
            sleep(20);
            ccenter_restart_count++;
            start_count++;
          }
          sendAlarm(109,0,getSlotOfNmu(0,2),0,0,0,0,0,NULL);
         }
         else
         {
          //ccenter_restart_count=0;
          sendAlarm(115,0,getSlotOfNmu(0,2),0,0,0,0,0,NULL);
          sleep(1);
          // if (is_save_log)
          //   save_log("reboot");
          //printf("reboot:CC restart too much...\n");
          //setLastTime();
          //system("reboot");
          reBoot("CC restart");
         }
      }
      #ifdef IS_TEMP_VERSION
      else
      {
         uchar_t uc=0,ut=0,n;
         //printf ("%d uclass %d in IS_TEMP_VERSION",n,uc);
         for (n=1;n<3;n++){
           getUnitClassType(0,n,&uc,&ut);
         //printf ("%d uclass=%d  utype=%d in IS_TEMP_VERSION\r\n",n,uc,ut);
         if (uc<2)
         {
           /*
           uchar_t *us_shm;
           char *temp_shm;
           temp_shm=getTempShm();
           if (temp_shm!=NULL)
           {

           }
           us_shm=getUnitStatusMap(0);
           if (NULL!=us_shm)
            *(us_shm+1)=0;
           */
           FILE *fp;       
           unit_base_info_t ubi;
           char tmp_buf[512]={0};

           sprintf(tmp_buf,"/vispace/unit%d.bi",n);
           fp=fopen(tmp_buf,"rb");
           if(fp!=NULL)
           {
             unit_base_info_t *bi=getBaseInfoMap(0,n);
             if (bi!=NULL)
             {
               
               //char property[60]={0};
               fread(bi,sizeof(unit_base_info_t),1,fp);
               //srand((unsigned)time( NULL));
               //sprintf(tmp_buf,"SN111-%d%d%d%d",rand()%10,rand()%10,rand()%8+1,rand()%10);
               //strcpy(bi->sn,tmp_buf);
               //sprintf(tmp_buf,"2013-%d-%d",rand()%4+9,rand()%30+1);
               //strcpy(bi->creation,tmp_buf);
               fread(tmp_buf,48,1,fp);
               //printf("Unit #%d:%s\r\n",n,bi->model);
               if (n==2)
               {
                char odm[60]={0X01,0X01,0X01,0X03,0X02,0X01,0X01,0XCB,0XCE,0X0E,0X0D,0X00,0X00,0X00,0X00,0X00,0X00,0XFB,0X00,0XCC,0XCD,0X0E,0X0D,0X00,0X00,0X00,0X00,0X00,0X00,0XFC,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00};
                   
                 insertUnit(0,2,UC_PASSIVE,2,*bi,bi->model,odm,0);
               }
               else
                  insertUnit(0,1,UC_OTU,2,*bi,bi->model,tmp_buf,0);
               if (n==1)
               {
                 FILE *dfp;
                 dfp=fopen("/vispace/unit.dat","rb");
                 if (dfp!=NULL)
                 {  
                   fread(tmp_buf,512,1,dfp);
                   /*
                    int k;
                    for (k=0;k<32;k++)
                      printf("%d ",tmp_buf[k]);
                   */
                   saveMessageToShm(0,1,tmp_buf);
                   fclose(dfp);
                 }
               }
               /*bi++;
               fread(bi,sizeof(unit_base_info_t),1,fp);
               fread(tmp_buf,48,1,fp);
               */
               
             }
              
             fclose(fp);
           }
           else
           {
              strcpy(ubi.model,"OTU-123A");
              strcpy(ubi.sn,"SN111-123A");
              strcpy(ubi.fwver,"01.10.23");
              strcpy(ubi.hwver,"01.01.01");
              strcpy(ubi.creation,"OTU-123A");
           }
          }
         }
      }
      //printf("end of IS_TEMP_VERSION\r\n");
      #endif
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
         /*
         if (is_save_log)
             save_log("olputil");
          system("olputil &");
          sleep(20);
          start_count++;
          sendAlarm(113,0,getSlotOfNmu(0,5),0,0,0);
          */
          sendAlarm(113,0,getSlotOfNmu(0,2),0,0,0,0,0,NULL);
          //printf("reboot:olputil is die...\n");
          //setLastTime();
          //system("reboot");
          reBoot("olputil exited");
      }
 }
 else if (HEART_BEAT_SERVICE_ID==proc)
 {
     pid=getPidByName("pidof heartbeat");
      if (pid<1)
      {
         //if (is_save_log)
         //    save_log("heartbeat");
          /*
          system("heartbeat &");
          sleep(20);
          start_count++;
          */
          //sendAlarm(111,0,getSlotOfNmu(0,2),0,0,0,0,0,NULL);
          //printf("reboot:HB is die...\n");
          //setLastTime();
          //system("reboot");
          reBoot("HB exited");
      }
 }
 else if (UDP_SERVICE_ID==proc)
 {
   pid=getPidByName("pidof udps");
   if (pid<1)
   {
      // printf("reboot:udps is die...\n");
       //setLastTime();
       //system("reboot");
       reBoot("udps exited");
   }
 }
 if (start_count>10)
 {   
   snmp_start_count=0;
   sendAlarm(115,0,getSlotOfNmu(0,2),0,0,0,0,0,NULL);
   if (!debug.is_debug_mode)
   {
     //printf("reboot:restart too much...\n");
     //setLastTime();
     //system("reboot");
     reBoot("restart too much");
   }
 }
} 
static void sigHandle(int sig,struct siginfo *siginfo,void *myact)
{

  //printf("sig=%d siginfo->si_int=%d SIGALRM=%d,SIGSEGV=%d\n",sig,siginfo->si_int,SIGALRM,SIGSEGV);
  if (sig == SIGALRM)
  {
    float sys_usage, user_usage,mem_usage;
    struct mem_usage_t memery;

    time_t now; //实例化time_t结构
    struct tm *timenow;     //实例化tm结构指针
    time(&now);
    //time函数读取现在的时间(国际标准时间非北京时间)，然后传值给now
    timenow=localtime(&now);
    //printf("timenow->tm_hour=%d timenow->tm_min=%d\n",timenow->tm_hour,timenow->tm_min);
    if (timenow->tm_hour%2==0 && timenow->tm_min<1)
     {
       updateDate();
       sendNmuIpToUnit(2);
     }
    //每周三23点59分重启进程，每周六23点59分重启系统
    if (timer_start && timenow->tm_wday%3==0 && timenow->tm_hour==23 && timenow->tm_min>58)
    {
       sleep(60-timenow->tm_sec);
       if (timenow->tm_wday!=6)
       {
         //每周三23点59分重启olputil
         /*
         if (is_run_olp)
         {
           system("killall -9 olputil");
           sleep(5);
           system("olputil&");
         }
         
         //每周三23点59分重启heartbeat
         if (is_run_heartbeat)
         {
           system("killall -9 heartbeat");
           sleep(5);
           system("heartbeat&");
         }
        */
        //每周三23点59分重启webs
         pid_t pid=getPidByName("pidof webs");
         if (pid>0)
         {
           doSystem("kill-9 %ld",pid);
           system("killall -9 webs");
           sleep(3);
         }
         doSystem("cd %s && ./webs&",WEBS_PATH);
       }
       else //每周六23点59分重启系统
       {
         setLastTime();
         system("reboot -n -f");
        // reboot(0x01234567);
       }
    }
   // printf("Time:%d %d:%d %d\r\n",timenow->tm_wday,timenow->tm_hour,timenow->tm_min,timenow->tm_sec);
    if (!getCpuUsage(&sys_usage,&user_usage))
    {
      //printf("CPU usage user:%.1f%%\tsys:%.1f%%\n",user_usage,sys_usage);
      
      if ((sys_usage+user_usage)>95)
      {
       printf("CPU usage user:%.1f%%\tsys:%.1f%% Over!\n",user_usage,sys_usage);
       sendAlarm(101,0,getSlotOfNmu(0,0),0,0,0,0,0,NULL);
       //setConfValue(SYS_CONF_FILE,"lasttime","0");
       //setLastTime();
       //system("reboot");
       reBoot("CPU usage more than 95%");
      }
    }
    mem_usage=getMemUsage(&memery);
    //printf("%lu %lu %lu %lu %lu %lu\n",memery.total,memery.used,memery.free,memery.shared,memery.buffers,memery.cached);
    //printf("Mem usage:%.1f%%\n",mem_usage);
    if (mem_usage>95)
    {
       printf("%lu %lu %lu %lu %lu %lu\n",memery.total,
	     memery.used,memery.free,memery.shared,memery.buffers,memery.cached);
       //printf("Mem usage:%.1f%% over!\nreboot...\n",mem_usage);
       sendAlarm(103,0,getSlotOfNmu(0,0),0,0,0,0,0,NULL);
       //setLastTime();
       //system("reboot");
       reBoot("MEM usage more than 95%");
    }
    startService(SNMPD_SERVICE_ID);
    startService(WEBS_SERVICE_ID);
    startService(CCENTER_SERVICE_ID);
    startService(ALARM_SERVICE_ID);
    if (is_run_olp)
       startService(OLP_SERVICE_ID);
    if (is_run_heartbeat)
       startService(HEART_BEAT_SERVICE_ID);
    if (is_run_udp)
       startService(UDP_SERVICE_ID);
    if (snmp_fail_count>0)
        alarm(10);
    else if (start_count<3)
       alarm(30);
    else
    {  
      if ( start_count<10)
        alarm(20);
      else
      {
        sendAlarm(115,0,getSlotOfNmu(0,0),0,0,0,0,0,NULL);
        if (!debug.is_debug_mode)
        {
          //printf("reboot:restart too much...\n");
          //setLastTime();
          //system("reboot");
          reBoot("restart too much");
        }
        else
            start_count=0;
      }
    }
  }
  else if (sig == SIGSEGV)
  {
    sleep(1);
    printf("SIGSEGV\n");
    //startService(siginfo->si_int);
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
  if (debug.is_debug_mode)
    sigaction(SIGSEGV,&act,NULL);
}
static void usage()
{
  printf("ver 1.2.12\nLast modifed:%s\n",LAST_MODIFY_DATE);
  printf("Usage:watchdogd [option] \n");
  printf("Option Specification:\n");
  printf("-d: debug\n");
  printf("-e: max errors to save\n");
  printf("-s: sleep\n");
}

static void init_watchdogd(int argc, char **argv)
{
   int oc;
   int waitting=120;
  //printf("Auto reboot testing...0\r\n");
  while((oc = getopt(argc, argv, "de:s:h")) != -1)
  {
     //printf("oc=%d\r\n",oc);
     switch(oc)
     {
       case 'd':
         initDebug();
         debug.is_debug_mode=1;
         debug.max_syslog_count=300;
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
  //printf("Auto reboot testing\n");
  //printf("Auto reboot testing...3asd\n");

  
  if (lockFile("/vispace/wd.lck")<0)
  {
     exit(0);
  }
  
  char value[30]="";
  
  getConfValue(SYS_CONF_FILE,"timesource",value," ");
  if (value[0]=='2')
  {
    value[0]='\0';
    getConfValue(SYS_CONF_FILE,"ntpserver",value," ");
    if (strlen(value)>5)
    {
      char buf[120];
      system("mkdir -p /var/spool/cron/crontabs");
      sprintf(buf,"echo '* */1 * * * /usr/bin/ntpdate %s'>/var/spool/cron/crontabs/root",value);
      //printf("\n%s\n",buf);
      system(buf);
    }
  }
  initAlarmConf();
  value[0]='\0';
  getConfValue(SYS_CONF_FILE,"olputil",value," ");
  if (value[0]=='Y')
     is_run_olp=1;
  value[0]='\0';
  getConfValue(SYS_CONF_FILE,"hb_enable",value," ");
  if (value[0]=='Y')
     is_run_heartbeat=1;
  value[0]='\0';
  getConfValue(SYS_CONF_FILE,"udp_enable",value," ");
  if (value[0]=='Y')
     is_run_udp=1;
  if (debug.is_debug_mode!=1)
    initDebug();
  value[0]='\0';
  getConfValue(SYS_CONF_FILE,"timer_start",value," ");
  if (value[0]=='Y')
   timer_start=1;
  value[0]='\0';
  if (0==getConfValue(SYS_CONF_FILE,"max_slcount",value," "))
  {
    if (atoi(value)>0)
      is_save_sys_log=1;
  }
  //int ts=getConfIntValue(SYS_CONF_FILE,"timesource"," ");
  /*
  value[0]='\0';
  getConfValue(SYS_CONF_FILE,"timesource",value," ");
  if (value[1]=='2')
  {
    getConfValue(SYS_CONF_FILE,"ntpserver",value," ");
    doSystem("ntpdate -u %s",value);
    system("hwclock -w");
  }
  setUnitDate(3);
  */
  updateDate();
  sendNmuIpToUnit(2);
  sigInit();
  system("tail -500 /vispace/sys.log>>/vispace/sys.log");
  #ifdef IS_TEMP_VERSION
     alarm(120);
  #else
     if (waitting>0)
     alarm(waitting);
  #endif
  //printf("is_save_sys_log=%d\n",is_save_sys_log);
}
typedef struct _alarm_info_t
{
  int alarm_code;
  char chassis;
  char slot;
  char entity;
  char is_with_value;
  short alarm_value;
  unsigned short alarm_reason;
  time_t alarm_time;
  void(*value2message)(short,char *);
} alarm_info_t;

void alarmrcv()
{
   //int running = 1;  
    int msgid = -1;  
    alarm_info_t data;  
    long int msgtype = 0; //注意1  
  
    //建立消息队列  
    msgid = msgget((key_t)1234, 0666 | IPC_CREAT);  
    if(msgid == -1)  
    {  
        fprintf(stderr, "msgget failed with error: %d\n", errno);  
        exit(EXIT_FAILURE);  
    }  
    //从队列中获取消息，直到遇到end消息为止  
    while(1)  
    {  
        if(msgrcv(msgid, (void*)&data, BUFSIZ, msgtype, 0) != -1)  
        {  
            //fprintf(stderr, "msgrcv failed with errno: %d\n", errno);  
            //exit(EXIT_FAILURE); 
            printf("alarmrcv: %d\n",data.alarm_code);   
        }
        else
            printf("alarmrcv fail\n");
        //printf("You wrote: %s\n",data.text);  
        //遇到end结束  
       // if(strncmp(data.text, "end", 3) == 0)  
        //    running = 0;  
    }  
}
int main(int argc, char **argv)
{
  //getSlotOfNmu(0,10);
  /* Most of time it goes to sleep */
  //sendNmuIpToUnit(2);
  init_watchdogd(argc,argv);
  //sleep(30);
  //alarmrcv();
  while (1)
  {	
    pause();
  }
  return 1;
}
