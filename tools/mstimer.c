#include <stdio.h>        //printf()
#include <unistd.h>        //pause()
#include <signal.h>        //signal()
#include <string.h>        //memset()
#include <sys/time.h>    //struct itimerval, setitimer()

#include<sys/types.h> 
#include<sys/socket.h> 
#include<netinet/in.h> 
#include<arpa/inet.h> 
#include<stdlib.h> 
#include<errno.h> 
#include<netdb.h> 
#include<stdarg.h> 

#include <public.h>
#include <unit.h>
#include <sysinfo.h>
#include <visipc.h>
#include <chassis.h>
#include <olp.h>
#include <vispaceio.h>
#include <dbutil.h>
#include <heartbeat.h>
//cc mstimer.c -o mstimer
//arm-linux-gcc -o heartbeat  mstimer.c -I../include -L=./ -L/home/visint/src/arm/lib -lvispace -lsqlite3 -liconv

#define DEFAULT_PORT 2012 
#define BUFFER_SIZE 20

#define _DEBUG__ 1
static  int ac_msg_len=2;//确认码长度
static  char ac_msg[16]={0x71,0x5a};//确认码
static  char ehco[16]={0};

static int count = 0;
static short invalid_heart_beat=0;
static short disabled_unit[16]={0};
static char enabled_hb_units[16]={0};
static char units_hb_state[16]={2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2};//存放当前心跳状态
static char units_hb_last_state[16]={2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2};//存放最后一次心跳状态

static char heart_beat_state=0;

static int seconds,mseconds;

static char hb_mode=1;
//static char is_busy;
static char first_run=1;
static char enabledReply;
static int  recv_count=0,err_msg_count=0,hb_count=0;
static long hb_msg_len=0;
static char exit_if_hb_fail=0;
static char is_setting=0;
static char enale_linkage=0;
static heart_beat_group_t hb_group[4];
static uchar_t max_group;
static int sigval=0;
#ifdef _DEBUG_
char messages[4][17]={
{0xef,0xef,0x00,0x00,0x0c,0x00,0x00,0xFF,0x16,0x00,0x01,0x00,0x00,0x3f,0x00,0x00,0x3f},
{0xef,0xef,0x00,0x00,0x0c,0x00,0x00,0xFF,0x16,0x00,0x01,0x00,0x03,0xc0,0x00,0x00,0xc3},
{0xef,0xef,0x00,0x00,0x0c,0x00,0x00,0xFF,0x16,0x00,0x01,0x00,0xfc,0x00,0x00,0x00,0xfc},
{0}
};
int  message_count[4]={0};
#endif
char is_first_start_timer;

int getCheckSum(char message[],unsigned char from,unsigned char to)
{
  unsigned char n;
  int sum=0;
  for (n=from;n<=to;n++)
  {
    sum+=(unsigned short)message[n];
  }
  return sum;
}

char parsekHeartBeatMessage(char message[])
{
 int n,m;
 /*
 if (msg_type!=1)
 {
   if (!memcmp(message,ac_msg,ac_msg_len))
   {
     for (n=0;n<slots_count;n++)
          slots[n]=1;
     return 1;
   }
   else
     return 0;
 }
 */
  //printf("Run in check_source...hb_slot_count:%d fromip=%s ac_msg:%0x%0x\r\n",hb_slot_count,fromip,message[0],message[1]);
/*示例:
发送：发送盘1、2、3、4 心跳(或第一路心跳)
ef ef 00 00 0c 00 00 FF 16 00 01 00 00 0f 00 00 0f
返回：FE FE 00 00 16 00 00 FF 16 00 01 28 04 04 01 01 00 02 01 00 03 01 00 04 01 00 66
*/
  char *ptr,flag[]={0xEF,0xEF,0x00},comm[]={0xFF,0x16};
  int check_sum=0;
  //char hb_state;
  if (memcmp(message,flag,3)!=0)
  {
     printf("FLAG1 Error\n");
     invalid_heart_beat=1;
     return 0;
  }
  ptr=message+7;

  if (memcmp(ptr,comm,2)!=0)
  {
     printf("COMM Error\n");
     invalid_heart_beat=1;
     return 0;
  }
  int sum =getCheckSum(message,0,10);
  if ((0xff & sum)!=message[11])
  {
    if (debug.is_debug_mode)
    {
       printf("HSUM Error(sum=%d %0x checkSum=%0x)\n",sum,(0xff & sum),message[11]);
       for (n=0;n<12;n++)
         printf("%0x ",message[n]);
       printf("\n");
    }
    return 0;
  }
  for (n=11;n<16;n++)
     sum+=(unsigned char)message[n];
  if ((0xff & sum)!=message[16])
  {
    if (debug.is_debug_mode)
    {
       printf("SUM Error(sum=%d %0x checkSum=%0x)\n",sum,(0xff & sum),message[16]);
       for (n=0;n<16;n++)
         printf("%0x ",message[n]);
       printf("\n");
    }
    return 0;
  }

  for (n=0;n<8;n++)
  {
     if (units_hb_state[n]!=1) //1-8槽心跳状态
        units_hb_state[n]=(message[13]>>n) & 0x01;

     if (units_hb_state[n+8]!=1) //9-16槽心跳状态
        units_hb_state[n+8]=(message[12]>>n) & 0x01;
     //printf("#%d=%d #%d=%d ",n+1,units_hb_state[n],n+8,units_hb_state[n+8]);
  }
  //printf("\n");
  if (invalid_heart_beat!=0)
    invalid_heart_beat=0;
  return 1;
}

void start(char is_first)
{
  struct itimerval tick;
  is_first_start_timer=1;
  memset(&tick, 0, sizeof(tick));

    //Timeout to run first time
    /*
    if (is_first)
    {
     sleep(1);
    }
    else
      usleep(1000*mseconds);
   */
    //usleep(50000);
    tick.it_value.tv_sec = seconds;
    tick.it_value.tv_usec = 1000*mseconds;

    //After first, the Interval time for clock
    tick.it_interval.tv_sec = seconds;
    tick.it_interval.tv_usec = 1000*mseconds;

    if(setitimer(ITIMER_REAL, &tick, NULL) < 0)
        printf("Set timer failed!\n");
   // printf("Start value.tv_sec=%d\n",tick.it_value.tv_sec);
}
void stop()
{
 struct itimerval tick;
 memset(&tick, 0, sizeof(tick));
 tick.it_value.tv_sec = 0;
 tick.it_value.tv_usec = 0;
 tick.it_interval = tick.it_value;
 setitimer(ITIMER_REAL, &tick, NULL);
}
/*
void value2message(short value,char *message)
{
  int n;
  short val;
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
*/
typedef struct _broadcast_arg_t
{
 uchar_t hb_slot_count;
 char *slots;
 char status;
 char retry;
}broadcast_arg_t;

void *broadcastHeartbeat(broadcast_arg_t arg)
{
 //uchar_t uc=0,ut=0;
 //char *values=NULL;

 char message[512];
 memset(message,0,512*sizeof(char));
 message[0]='S';
 message[1]=OLP_STATUS_INFOSET_ID;
 message[2]=1;
 message[3]=22;
 message[4]=0;
 if (arg.status!=1)
 {
  message[5]=2;
  //if (is_debug)
  // printf("Heartbeat of OBP in slot #%d timeout!\r\n",slot);
 }
 else
 {
   message[5]=1;
   //if (is_debug)
   // printf("Heartbeat of OBP in slot #%d is normal!\r\n",slot);
 }
  /*
  int n;
  printf("BroadcastHeartbeat status=%d hb_slot_count=%d slot_list:",arg.status,arg.hb_slot_count);
   for (n=0;n<arg.hb_slot_count;n++)
   {
    printf("%d ",arg.slots[n]);
   }
   printf("\n");
 */
 is_setting=1;
 #ifdef BROADCAST_MODE
 ioBroadcast(0,arg.hb_slot_count,arg.slots,message,arg.retry);
 #else
 uchar_t n;
 uchar_t uc=0,ut=0;
 char *values=NULL;
 
 for (n=0;n<arg.hb_slot_count;n++)
 {
   getUnitClassType(0,arg.slots[n],&uc,&ut);
   if (uc==UC_OLP && ut>50)
     sendMessageToUnit(0,arg.slots[n],message,600,0,0,0);
   //if (arg.slots[n]>13)
    // printf("sendMessageToUnit:%d\n",arg.slots[n]);
 }
 #endif
 is_setting=0;
 //sendMessageToUnit(0,slot,message,600,is_debug,is_debug,is_debug);
 //is_setting=3;
}
short get_hb_slots(char slot_count,char *slot_list)
{
  short value=0;
  short hb_slots=0;
  char n;
  for (n=0;n<slot_count;n++)
  {
       value=1;
       if (slot_list[n]>1)
            value=value<< slot_list[n]-1;
       hb_slots|=value;
  }
  return hb_slots;
}

void broadcast_hb()
{
   char hb_ok_units[16]={0};
   char hb_fail_units[16]={0};  
   char hb_ok_count=0;
   char hb_fail_count=0;
   
   char n=0,index;

   for (n=0;n<16;n++)
   {
      //printf("ENABLE:%d-%d ",n,enabled_hb_units[n]);
      if (enabled_hb_units[n]>0)
      {
          index=enabled_hb_units[n]-1;
          if (index<0 || index>15)
            continue;
          if (units_hb_last_state[index]!=1)
          {
            hb_fail_units[hb_fail_count++]=enabled_hb_units[n];
          }
          else
             hb_ok_units[hb_ok_count++]=enabled_hb_units[n];
      }
      else
          break;
   }
   //printf("broadcast_hb:hb_fail_count=%d hb_ok_count=%d\n",hb_fail_count, hb_ok_count);
   if (hb_fail_count>0 || hb_ok_count>0)
   {
      stop();
      broadcast_arg_t arg;
      arg.retry=1;
      //sendMessageToUnit(0,slot,message,600,is_debug,is_debug,is_debug);
       if (hb_fail_count>0)
       {
          arg.status=0;
          arg.hb_slot_count=hb_fail_count;
          arg.slots=hb_fail_units;
          //#ifdef BROADCAST_MODE
          broadcastHeartbeat(arg);
          //#endif
          //broadcastHeartbeat(hb_fail_count,hb_fail_units,0,1);
       }
       if (hb_ok_count>0)
       {
          arg.status=1;
          arg.hb_slot_count=hb_ok_count;
          arg.slots=hb_ok_units;
          //#ifdef BROADCAST_MODE
          broadcastHeartbeat(arg);
          //#endif
          //broadcastHeartbeat(hb_ok_count,hb_ok_units,1,1);
       }
      start(0);
   }
}
int getGroupBySlot(uchar_t slot)
{
  uchar_t n,m;
 for (n=0;n<max_group;n++)
 {
   for (m=0;m<hb_group[n].obp_count;m++)
   {
     if (hb_group[n].hb_slots[m]==slot)
        return n;
   }
 }
 return -1;
}
void setObpGroupMode(char mode)
{
 int n,m,index;
 int count;
 uchar_t slots[16];

 for (n=0;n<max_group;n++)
 {
    if (debug.is_debug_mode==1)
      printf("hb_group[n].linkage=%d hb_group[n].bypass=%d\n",hb_group[n].linkage,hb_group[n].bypass);
   if (hb_group[n].linkage==1 &&hb_group[n].bypass==1)
   {
       hb_group[n].bypass=0;
       for (m=0;m<hb_group[n].obp_count;m++)
       {
         if (count<16)
           slots[count++]=hb_group[n].hb_slots[m];
      }
      hb_group[n].bypass=0;
   }
 }
 if (debug.is_debug_mode==1)
   printf("Set hb_group slot_count=%d to bypass\n",count);
 if (count>0)
 {
 char message[512];
 memset(message,0,512*sizeof(char));
 message[0]='S';
 message[1]=OLP_STATUS_INFOSET_ID;
 message[2]=1;
 message[3]=10;
 message[4]=0;
 message[5]=mode;
          //for (m=0;m<16;m++)
 if (debug.is_debug_mode==1)
   printf("set hb_group slot_count=%d to bypass\n",count);
 ioBroadcast(0,count,slots,message,5);
 }
}
void detectLinkage()
{
 uchar_t n,m,slot_count=0,linkage_status;
 
 uchar_t slots[16]={0};
 uchar_t working_line_slots[16]={0};
 //uchar_t uc=0,ut=0;
 uchar_t bypass_slots=0;
 char *values=NULL;
 
 uchar_t *us=getUnitStatusMap(0);
 if (us==NULL)
    return;
 
 for (n=0;n<max_group;n++)
 {
   //printf("detectLinkage linkage=%d\n",hb_group[n].linkage);
   if(hb_group[n].linkage==1)
   {
      //bypass_slots=0;
      char changed=0;
      slot_count=0;
      for (m=0;m<hb_group[n].obp_count;m++)
      {
          //printf("detectLinkage n=%d m=%d slot=%d UC=%d\n",n,m,hb_group[n].hb_slots[m],*(us+hb_group[n].hb_slots[m]));
          if(hb_group[n].hb_slots[m]>0 && hb_group[n].hb_slots[m] <16 && *(us+hb_group[n].hb_slots[m])==UC_OLP)
          {
             obpInfo_t *obp=getObpInfo(0,hb_group[n].hb_slots[m]);
             if (obp!=NULL)
             {
                //printf("detectLinkage slot=%d obp->line=%d\n",hb_group[n].hb_slots[m],obp->line);
                if (obp->mode!=3)
                {
                   //printf("detectLinkage obp->line=%d\n",obp->line);
                   if (changed!=1)
                   {
                    changed=1;
                    //if (obp->rx1_led==2||obp->rx2_led==2)
                     //if (obp->rx_power[0]<obp->l_switch_power[0] ||obp->rx_power[1]<obp->l_switch_power[1])
                     //{
                       // printf("detectLinkage Changed=%d\n",changed);
                      //  changed=1;
                     //}
                   }
                   //bypass_slots++;
                }
                else if (slot_count<16)
                {   
                 slots[slot_count++]=hb_group[n].hb_slots[m];
                }            
             }
             //getInfoSetValues(0,hb_group[n].slots[m],65,1);
             //getUnitClassType(0,arg.slots[n],&uc,&ut);
          }
          else
             break;
      }
      //printf("detectLinkage changed=%d\n",changed);
      if (changed && slot_count>0)
      {
          char message[512];
          memset(message,0,512*sizeof(char));
          message[0]='S';
          message[1]=OLP_STATUS_INFOSET_ID;
          message[2]=1;
          message[3]=10;
          message[4]=0;
          message[5]=3;
          //for (m=0;m<16;m++)
          printf("set hb_group #%d slot_count=%d to bypass",n+1,slot_count);
          ioBroadcast(0,slot_count,slots,message,3);
      }
   }
 }

 #ifdef BROADCAST_MODE
 //ioBroadcast(0,arg.hb_slot_count,arg.slots,message,arg.retry);
 #else
/*
 uchar_t n;
 uchar_t uc=0,ut=0;
 char *values=NULL;
 
 for (n=0;n<arg.hb_slot_count;n++)
 {
   getUnitClassType(0,arg.slots[n],&uc,&ut);
   if (uc==UC_OLP && ut>50)
     sendMessageToUnit(0,arg.slots[n],message,600,0,0,0);
 }
*/
 #endif
}
//time_t last_time;
char hb_stop_count=0;
char static poll_count=0;
void on_tick(int signo) //定时比较心跳状态
{
   char n=0,index;
   char has_hb=0;
   //char hb_state[16]={0};
   char hb_ok_units[16]={0};
   char hb_fail_units[16]={0};
   
   char hb_ok_count=0;
   char hb_fail_count=0;
   if (is_first_start_timer)
   {
      is_first_start_timer=0;
      return;
   }
   if (is_setting)
    return;
   /*
   while (is_busy)
   {
     usleep(20000);
     if (!is_busy)
      break;
     if (n<9)
        n++;
     else
         break;
   }
   if (is_busy)
     return;
   */
   //is_busy=1;

  if (sigval!=0)
  {
    /*
    if (sigval==52)
    {
      stop();
      usleep(200000);
      broadcast_hb();
      sigval=0;
      start(0);
    }
    else
    */
    //printf("sigval=%d\n",sigval);
    if (sigval>2)
    {
      //if (poll_count==2)
      {       
        stop();
        //detectLinkage();
        setObpGroupMode(3);
        sigval=0;
        //is_busy=0;
        //poll_count=0;
        start(0);
        //return;
      }
    }
    else
         sigval++;
    /*
    else if (sigval==1)
        sigval=2;
    else
        sigval=0;
    if (poll_count<2)
        poll_count++;
    else
        poll_count=0;
   */
    //is_busy=0;
    return;
  }
  /*
  if (debug.is_debug_mode)
  {
   struct timeval now;
   time_t tm;
   time(&tm);
   gettimeofday(&now, NULL);
   printf("on_tick: %s %ld ms\n",ctime(&tm), now.tv_usec/1000);
  }
  */
   //on_recv_hb();
   for (n=0;n<16;n++) //检测心跳状态是否有变化
   {
      //printf("ENABLE:%d-%d ",n,enabled_hb_units[n]);
      if (enabled_hb_units[n]>0)
      {
          index=enabled_hb_units[n]-1;
          if (index<0 || index>15)
            continue;
          if (units_hb_last_state[index]!=units_hb_state[index]/*||first_run*/)
          {
             //printf("hb_state #%d:%d-%d ",enabled_hb_units[n],units_hb_state[enabled_hb_units[n]-1],units_hb_last_state[enabled_hb_units[n]-1]);
             
             if (units_hb_state[index]!=1)
             {
                 if (units_hb_last_state[index]==1 /*||first_run*/)
                 {
                   hb_fail_units[hb_fail_count++]=enabled_hb_units[n];
                   units_hb_last_state[index]=2;
                 }
            }
            else if (units_hb_last_state[index]!=1)
            {
             //printf("HB_state #%d:%d-%d ",enabled_hb_units[n],units_hb_state[enabled_hb_units[n]-1],units_hb_last_state[enabled_hb_units[n]-1]);
             
               if (units_hb_state[index]==1)
               {
                 hb_ok_units[hb_ok_count++]=enabled_hb_units[n];
                 units_hb_last_state[index]=1;
               }
            }
          }
      }
      else
          break;
   }
   memset(units_hb_state,2,16);
   
   if (hb_fail_count>0 || hb_ok_count>0)//心跳状态有变化
   {
      stop();
      time_t now_time;
      time(&now_time);
     
      struct timeval tv;
      //pthread_t ok_thread;
      broadcast_arg_t arg;
      arg.retry=3;
      gettimeofday(&tv, NULL);
      
      if (debug.is_debug_mode)
       printf("before broadcast: %s %ld ms\n",ctime(&now_time), tv.tv_usec/1000);
      if (hb_ok_count>0)//存在心跳状态由无心跳变为有心跳的OBP卡
       {
         //printf("Heartbeat start..\n");
         arg.hb_slot_count=hb_ok_count;
         arg.slots=hb_ok_units;
         arg.status=1;
         //pthread_create(&ok_thread,NULL,broadcastHeartbeat,&arg);
         //#ifdef BROADCAST_MODE 
         broadcastHeartbeat(arg);//向OBP卡广播有心跳的状态
         //#endif
         if (!first_run)//往消息队列发送有心跳告警
           sendAlarm(20,0,getSlotOfNmu(0,2),0,1,get_hb_slots(hb_ok_count,hb_ok_units),0,now_time,NULL);
       }
       if (hb_fail_count>0) //存在心跳状态由有心跳变为无心跳的OBP卡
       {
        // printf("Heartbeat stop..\n");
         arg.hb_slot_count=hb_fail_count;
         arg.slots=hb_fail_units;
         arg.status=0;
         //#ifdef BROADCAST_MODE
         broadcastHeartbeat(arg);//向OBP卡广播无心跳的状态
         //#endif
         if (!first_run)//往消息队列发送无心跳告警
           sendAlarm(19,0,getSlotOfNmu(0,2),0,1,get_hb_slots(hb_fail_count,hb_fail_units),0,now_time,NULL);
       }

       if (debug.is_debug_mode)
       {
         struct timeval now;
         time_t tm;
         time(&tm);
         gettimeofday(&now, NULL);
         //printf("on_tick: %s %ld ms\n",ctime(&tm), now.tv_usec/1000);
         if (hb_fail_count>0)
         {
         printf("Heartbeat stop:recv_count=%d err_msg_count=%d hb_count=%d hb_msg_len=%ld\n",recv_count,err_msg_count,hb_count,hb_msg_len);
         //if (exit_if_hb_fail)
         //  exit(0);
         }
         else
            printf("Heartbeat start..\n");
        }
       start(0);
   }
   /*else if (poll_count>2)
   {
        poll_count=0;
        stop();
        detectLinkage();
        start(0);
   }
   */
   #ifdef BROADCAST_MODE
   else
   {
     time_t now_time;
     now_time=time(NULL);
     //printf("now_time(%ld) mod 20=%d debug_mode=%d\n",now_time,now_time % 20,debug.is_debug_mode);
     
     if (now_time % 20==0)//每20秒向OBP卡广播当前心跳的状态
     {
         struct timeval now;
         gettimeofday(&now, NULL);
         if (debug.is_debug_mode)
           printf("before set : %s %ld ms\n",ctime(&now_time), now.tv_usec/1000);
         broadcast_hb();
         if (debug.is_debug_mode)
         {
           now_time=time(NULL);
           
           gettimeofday(&now, NULL);
           printf("after set: %s %ld ms\n",ctime(&now_time), now.tv_usec/1000);
         }
     }
   }
  #endif
   if (first_run)
        first_run=0;
   if (invalid_heart_beat!=0)
   {
     invalid_heart_beat=0;
   }

   hb_count=0;
   //is_busy=0;

   err_msg_count=0;
  // if (poll_count<3)
  //   poll_count++;
}
void start_heart_beat(int port)
{
   /* 创建UDP套接口 */
 struct sockaddr_in server_addr; 
 bzero(&server_addr, sizeof(server_addr)); 
 server_addr.sin_family = AF_INET; 
 server_addr.sin_addr.s_addr = htonl(INADDR_ANY); 
 server_addr.sin_port = htons(port); 
  
 /* 创建socket */
 int server_socket_fd = socket(AF_INET, SOCK_DGRAM, 0); 
 if(server_socket_fd == -1) 
 { 
  perror("Create Socket Failed:"); 
  exit(1); 
 } 
  
 /* 绑定套接口 */
 if(-1 == (bind(server_socket_fd,(struct sockaddr*)&server_addr,sizeof(server_addr)))) 
 { 
  perror("Server Bind Failed:"); 
  exit(1); 
 } 
  
 /* 数据传输 */
 while(1) 
 {  
  /* 定义一个地址，用于捕获客户端地址 */
  struct sockaddr_in client_addr; 
  socklen_t client_addr_length = sizeof(client_addr); 
  
  /* 接收数据 */
  char buffer[BUFFER_SIZE]; 
  bzero(buffer, BUFFER_SIZE); 
  int recv_len=recvfrom(server_socket_fd, buffer, BUFFER_SIZE,0,(struct sockaddr*)&client_addr, &client_addr_length);//监听心跳包
  //printf("Receive Data len=%d\n",recv_len);
  if( recv_len!= -1) //接收到心跳包
  {
   if (debug.is_debug_mode) //调试模式
   {
     int n;
     hb_msg_len+=recv_len;
     recv_count++;
     hb_count++;
     
     struct timeval now;
     time_t tm;
     time(&tm);
     gettimeofday(&now, NULL);
     #ifdef _DEBUG_
      for (n=0;n<4;n++)
      {
        if (!memcmp(messages[n],buffer,17))
        {
           message_count[n]++;
           break;
        }
     }
     #endif
    //char *fromip = inet_ntoa(client_addr.sin_addr);
    //printf("Receive Data at: %s %ld ms (%d) from %s\n",ctime(&tm), now.tv_usec/1000,hb_count,fromip);
    }
   
   char ret=parsekHeartBeatMessage(buffer);//分析、提取心跳包信息
   if (!ret && debug.is_debug_mode)
        err_msg_count++;

  if (enabledReply)//回应信息
  {
       if (ret)
          sendto(server_socket_fd,ehco,strlen(ehco),0,(struct sockaddr*) &client_addr,client_addr_length);
        else
         sendto(server_socket_fd,"Error",strlen("Error"),0,(struct sockaddr*) &client_addr,client_addr_length);
  }

  } 
  else if (debug.is_debug_mode)
  {
      err_msg_count++;
      perror("Receive Data Failed:\n");
  }
 } 
 close(server_socket_fd); 
 return ; 
}

void usage()
{
  printf("Description:\n");
  #ifdef BROADCAST_MODE
    printf("Heart beat.\r\nver B2.0.13\r\nLast modifed:%s\n",LAST_MODIFY_DATE);
  #else
    printf("Heart beat.\r\nver 2.0.13\r\nLast modifed:%s\n",LAST_MODIFY_DATE);
  #endif
  printf("-s for sleep.\n");
  printf("-d for debug.\n");
  printf("-h for help.\n");
}
void sighandler(int signo, siginfo_t *info,void *ctx)
{
   static int count=0;
   short alarm_code;
   uchar_t slot;
   int group;
   if (debug.is_debug_mode==1)
     printf("enale_linkage=%d #%d alarm_code=%d\n",enale_linkage,info->si_int>>24,info->si_int& 0xffff);
   
   if (signo!=SIGINT)
       return;
   alarm_code=info->si_int& 0xffff;
   /*
   if (alarm_code==52)
   {
      sigval=52;
      return;
   }
   */
   if (alarm_code==501)
   {
   //printf("Run here enale_linkage=%d\n",enale_linkage);
   if (enale_linkage!=1)
       return;
 
   //printf("Run here2 alarm_code=%d\n",info->si_int& 0xffff);
   slot=(uchar_t)(info->si_int>>24);
   group=getGroupBySlot(slot);
   if (group>-1 && group<4)
      hb_group[group].bypass=1;
   //alarm_code=info->si_int& 0xffff;
   //printf("enale_linkage=%d slot=%d alarm_code=%d\n",enale_linkage,slot,info->si_int& 0xffff);
   //printf("Run here3\n");
   sigval=1;
   }
   /*stop();
   usleep(500000);
   detectLinkage();
   start(0);
   */
  // printf("receive the data (%d) from siqueue by info->si_int is %d\n",count++,info->si_int);
  // printf("receive the data from siqueue by info->si_value.sival_int is %d\n",info->si_value.sival_int);

}
void init_hb_conf(int argc, char **argv)
{
  char slots[15][20]={""};
  char accept_ips[6][20];
  int slot_count;
  int n,m=0,slot,count;

  char value[128]="";

  int oc;
  //int waitting=-1;
  while((oc = getopt(argc, argv, "dehs:")) != -1)
  {
        switch(oc)
        {
          case 's':
             seconds=atoi(optarg);
             break;
          case 'd':
               debug.is_debug_mode=1;
             break;
          case 'e':
                  exit_if_hb_fail=1;
                break;
          case 'h':
             usage();
             exit(0);
        }
  }

  
  getConfValue(SYS_CONF_FILE,"enabledReply",value," ");
  if (value[0]=='Y')
  {
    enabledReply=1;
    //printf("Heart beat is disabled\n");
    //exit(0);
  }
  //printf("enabledReply=%s %d\n",value,enabledReply);
  if (seconds<0)
  {
     seconds=getConfIntValue(SYS_CONF_FILE,"waitting"," ");
  }
  if (seconds>60)
      seconds=60;
  if (seconds>0);
      sleep(seconds);

    seconds=getConfIntValue(SYS_CONF_FILE,"sec"," ");
    mseconds=getConfIntValue(SYS_CONF_FILE,"msec"," ");
    if (seconds<0)
       seconds=0;
    if (mseconds<0)
        mseconds=0;
    if (mseconds>999)
        mseconds=999;

  hb_mode=getConfIntValue(SYS_CONF_FILE,"hb_mode"," ");
  if (hb_mode!=1)
  {
    getConfValue(SYS_CONF_FILE,"ac_msg",value," ");
    ac_msg_len=strtoca(value,";",ac_msg,16);
    if (ac_msg_len>16)
      ac_msg_len=16;
  }
  if (hb_mode<1 || hb_mode>2)
  {
    hb_mode=1;
  }
  char index=0;
  for (n=1;n<5;n++)
  {
     char group[20]="";
     int linkage=0;
     char hb_slots[60]="";
     char ips[60]="";
     sprintf(group,"hb_group%d",n);
     getConfValue(SYS_CONF_FILE,group,value," ");
     if (strlen(value)>2)
     {
         char val[16]={0};
         //int count;
         
         sscanf(value,"%d:%[^:]:%[^:]",&linkage,hb_slots,ips);
         //printf("n=%d slots:%s\n",n,hb_slots);
         if (linkage==1)
         {
            enale_linkage=1;
            hb_group[max_group].linkage=1;
         }
         hb_group[max_group].obp_count=strtoca(hb_slots,",",val,10);
         
         //printf("n=%d count=%d slots:%s\n",n,count,hb_slots);
         for (m=0;m<hb_group[max_group].obp_count;m++)
         {
           //printf("val[%d]=%d ",m,val[m]);
           if (val[m]>0 && val[m]<16)
           {
              int k;
              hb_group[max_group].hb_slots[m]=val[m];
              for (k=0;k<16;k++)
              {
                 if (enabled_hb_units[k]==0 || enabled_hb_units[k]==val[m])
                   break;
              }
              //printf("%d:%d ",enabled_hb_units[k],val[m]);
              if (enabled_hb_units[k]!=val[m] && index<16)
              {
               enabled_hb_units[index++]=val[m];
               //printf("%d:%d ",enabled_hb_units[index-1],val[m]);
              }
           }
           //printf("%d ",val[m]);
         }
         max_group++;
         //printf("\n");
     }
  }
/*
  printf("Here 1\n");
  for (n=0;n<16;n++)
  {
    if (enabled_hb_units[n]>0)
       printf("#%d ",enabled_hb_units[n]);
  }
 printf("Here2\n");
 */
 /*
 slot_count=getConfItems(SYS_CONF_FILE,"hb_slots",slots,15,",");
  if (slot_count>15)
     slot_count=15;
  for (n=0;n<slot_count;n++)
  {
    slot=atoi(slots[n]);
    if (slot<1 || slot>15)
      continue;  
    enabled_hb_units[n]=slot;
  }
  */
  getConfValue(SYS_CONF_FILE,"ehco",ehco," ");
  if (enale_linkage==1)
  {
    //detectLinkage();
    struct sigaction act;
    act.sa_sigaction = sighandler;
    sigemptyset(&act.sa_mask);
    act.sa_flags = SA_SIGINFO;//信息传递开关
    int sig; 

    if(sigaction(SIGINT,&act,NULL) == -1){
        perror("sigaction error");
        //exit(EXIT_FAILURE);
    }
  }
}


int main(int argc, char **argv)
{
   initAlarmConf();
   debug.max_syslog_count=200;
   init_hb_conf(argc,argv);

   signal(SIGALRM, on_tick);

    start(1);
    int port=getConfIntValue(SYS_CONF_FILE,"port"," ");
    if (port<1)
      port=DEFAULT_PORT;
    start_heart_beat(port);
    return 0;
}
