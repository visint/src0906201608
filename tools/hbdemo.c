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

//cc mstimer.c -o mstimer
//arm-linux-gcc -o heartbeat  hbdemo.c -I../include -L=./ -L/home/visint/src/arm/lib -lvispace -lsqlite3 -liconv

#define DEFAULT_PORT 2012 
#define BUFFER_SIZE 18

#define _DEBUG__ 1
static  int ac_msg_len=2;//确认码长度
static  char ac_msg[16]={0x71,0x5a};//确认码
static  char ehco[16]={0};

static int count = 0;
static short invalid_heart_beat=0;
static short disabled_unit[16]={0};
static char enabled_hb_units[16]={0};
static char units_hb_state[16]={2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2};
static char units_hb_last_state[16]={2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2};

static char heart_beat_state=0;

static int seconds,mseconds;

char hb_mode=1;
char is_busy;
char first_run=1;
char enabledReply;
int  recv_count=0,err_msg_count=0,hb_count=0;
long hb_msg_len=0;
char exit_if_hb_fail=0;
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
  //unsigned short value;
  for (n=from;n<=to;n++)
  {
    //value=(unsigned short)message[n];
    sum+=(unsigned short)message[n];
  }
  return sum;
  //return 0xff & sum;
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
void broadcastHeartbeat(uchar_t hb_slot_count, char *slot_list,char status,char retry)
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
 if (status!=1)
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
  printf("BroadcastHeartbeat status=%d hb_slot_count=%d slot_list:",status,hb_slot_count);
   for (n=0;n<hb_slot_count;n++)
   {
    printf("%d ",slot_list[n]);
   }
   printf("\n");
 */
 ioBroadcast(0,hb_slot_count,slot_list,message,retry);

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
void on_recv_hb()
{
  char hb_ok_units[16]={0}; 
  char hb_ok_count=0;
  char n;
  //is_busy=1;
  for (n=0;n<16;n++)
  {
      if (enabled_hb_units[n]>0)
      {
          if (units_hb_last_state[enabled_hb_units[n]-1]!=units_hb_state[enabled_hb_units[n]-1])
          {
             //printf("HB_state #%d:%d-%d ",enabled_hb_units[n],units_hb_state[enabled_hb_units[n]-1],units_hb_last_state[enabled_hb_units[n]-1]);
             //units_hb_last_state[enabled_hb_units[n]-1]=units_hb_state[enabled_hb_units[n]-1];
             if (units_hb_state[enabled_hb_units[n]-1]==1)
             {
                 hb_ok_units[hb_ok_count++]=enabled_hb_units[n];
                 units_hb_state[enabled_hb_units[n]-1]=2;
                 units_hb_last_state[enabled_hb_units[n]-1]=1;
             }
          }
      }
      else
          break;
   }
   if (hb_ok_count>0)
   {
     time_t now_time;
     time(&now_time);
     stop();
       
     broadcastHeartbeat(hb_ok_count,hb_ok_units,1,3);
     if (debug.is_debug_mode)
     {
       printf("Send Heartbeat start to unit:");
       for (n=0;n<hb_ok_count;n++)
         printf("#%d ",hb_ok_units[n]);
       printf("\n");
     }
     if (!first_run)
       sendAlarm(20,0,getSlotOfNmu(0,2),0,1,get_hb_slots(hb_ok_count,hb_ok_units),0,now_time,value2message);
     start(0);
   }
   //is_busy=0;
   //memcpy(units_hb_last_state,units_hb_state,16);
   //memset(units_hb_state,2,16);
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
   if (hb_fail_count>0 || hb_ok_count>0)
   {
      stop();
      //sendMessageToUnit(0,slot,message,600,is_debug,is_debug,is_debug);
       if (hb_fail_count>0)
         broadcastHeartbeat(hb_fail_count,hb_fail_units,0,1);
       if (hb_ok_count>0)
         broadcastHeartbeat(hb_ok_count,hb_ok_units,1,1);
      start(0);
   }
}

//time_t last_time;
char hb_stop_count=0;
void on_tick(int signo)
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
   is_busy=1;
   
  if (debug.is_debug_mode)
  {
   struct timeval now;
   time_t tm;
   time(&tm);
   gettimeofday(&now, NULL);
   //printf("on_tick: %s %ld ms\n",ctime(&tm), now.tv_usec/1000);
  }
   //on_recv_hb();
   for (n=0;n<16;n++)
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
             //units_hb_last_state[enabled_hb_units[n]-1]=units_hb_state[enabled_hb_units[n]-1];
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
             //units_hb_last_state[enabled_hb_units[n]-1]=units_hb_state[enabled_hb_units[n]-1];
               if (units_hb_state[index]==1)
               {
                 hb_ok_units[hb_ok_count++]=enabled_hb_units[n];
                 //units_hb_state[index]=2;
                 units_hb_last_state[index]=1;
               }
            }
         /*    else
             {
                 if (units_hb_last_state[index]!=1)
                 {
                   hb_ok_units[hb_ok_count++]=enabled_hb_units[n];
                   units_hb_last_state[index]=2;
                 }
             }
*/
             //else
            //    hb_ok_units[hb_ok_count++]=enabled_hb_units[n];
          
          //if (units_hb_last_state[enabled_hb_units[n]-1]!=1)
          //  hb_fail_units[hb_fail_count++]=1;
          }
      }
      else
          break;
   }
   //memcpy(units_hb_last_state,units_hb_state,16);
   memset(units_hb_state,2,16);
   
   if (hb_fail_count>0 || hb_ok_count>0)
   {
      stop();
      time_t now_time;
      time(&now_time);
      //sendMessageToUnit(0,slot,message,600,is_debug,is_debug,is_debug);
      struct timeval tv;
      gettimeofday(&tv, NULL);
      if (debug.is_debug_mode)
       printf("before broadcast: %s %ld ms\n",ctime(&now_time), tv.tv_usec/1000);
       if (hb_ok_count>0)
       {
         //printf("Heartbeat start..\n");
         broadcastHeartbeat(hb_ok_count,hb_ok_units,1,3);
          sleep(2);
         if (!first_run)
           sendAlarm(20,0,getSlotOfNmu(0,2),0,1,get_hb_slots(hb_ok_count,hb_ok_units),0,now_time,value2message);
         //gettimeofday(&now, NULL);
          //printf("After Heartbeat start: %s\n",ctime(&now_time));
       }
       if (hb_fail_count>0) {
        // printf("Heartbeat stop..\n");
         broadcastHeartbeat(hb_fail_count,hb_fail_units,0,3);
          sleep(3);
         if (!first_run)
           sendAlarm(19,0,getSlotOfNmu(0,2),0,1,get_hb_slots(hb_fail_count,hb_fail_units),0,now_time,value2message);
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
         //printf("Heartbeat stop:recv_count=%d err_msg_count=%d hb_count=%d hb_msg_len=%ld mc=%ld mc2=%ld,mc3=%ld\n",recv_count,err_msg_count,hb_count,hb_msg_len,message_count[0],message_count[1],message_count[2]);
         /*printf("Send heartbeat stop to unit:");
         for (n=0;n<hb_fail_count;n++)
            printf("#%d ",hb_fail_units[n]);
         printf("\n");
         */
         if (exit_if_hb_fail)
           exit(0);
         }
         else
            printf("Heartbeat start..\n");
        }
       start(0);
       //exit(0);
   }
   else
   {
     time_t now_time;
     now_time=time(NULL);
     //printf("now_time(%ld) mod 20=%d\n",now_time,now_time % 20);
     if (now_time % 20==0)
     {
         struct timeval now;
         gettimeofday(&now, NULL);
         if (debug.is_debug_mode)
           printf("before set : %s %ld ms\n",ctime(&now_time), now.tv_usec/1000);
         broadcast_hb();
         sleep(5);
         if (debug.is_debug_mode)
         {
           now_time=time(NULL);
           
           gettimeofday(&now, NULL);
           printf("after set: %s %ld ms\n",ctime(&now_time), now.tv_usec/1000);
         }
     }
   }
   //printf("first_run:%d\n",first_run);
  /*
   if (poll_count<40)
       poll_count++;
   else
   { 
     poll_count=0;
     if (hb_fail_count<1)
       broadcast_hb();
   }
 */  
   if (first_run)
        first_run=0;
   if (invalid_heart_beat!=0)
   {
     invalid_heart_beat=0;
   }
   /*
   struct itimerval tick;
   if (count%3==0)
   {
    tick.it_value.tv_sec = 0;
    tick.it_value.tv_usec = 0;
    tick.it_interval = tick.it_value;
    setitimer(ITIMER_REAL, &tick, NULL);
    sleep(1);  
   }
   */
   /*
   if (invalid_heart_beat>1)
   {
     if (heart_beat_state!=2)
     {
       heart_beat_state=2;
       srand((int) time(0));
       int delay=rand()%3+1;
       stop();
       printf("heart_beat stop,sleep %d ...\n",invalid_heart_beat);
       sleep(delay);
       start();
     }
    invalid_heart_beat=0;
   }
   else if (invalid_heart_beat<1 && heart_beat_state!=1)
   {
       srand((int) time(0));
       int delay=rand()%3+1;
       stop();
       heart_beat_state=1;
       printf("heart_beat start,sleep %d ...\n",delay);
       sleep(delay);
       start();
   }
   time_t now_time;
   time(&now_time);
   printf("Get a SIGALRM, %d counts!--%s\n", ++count,ctime(&now_time));
   */
  /*
   if (count%3==0)
   {
     tick.it_interval.tv_sec = 0;
     tick.it_interval.tv_usec = 500000;
     if(setitimer(ITIMER_REAL, &tick, NULL) < 0)
          printf("Set timer failed!\n");
   }
   */
   //printf("Get a SIGALRM, %d counts!\n", ++count);
   hb_count=0;
   is_busy=0;
   /*
   if (err_msg_count>1)
   {
      sendAlarm(15,0,getSlotOfNmu(0,2),0,0,0,0,0,NULL);
   }
   */
   err_msg_count=0;
   
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
  int recv_len=recvfrom(server_socket_fd, buffer, BUFFER_SIZE,0,(struct sockaddr*)&client_addr, &client_addr_length);
  //printf("Receive Data len=%d\n",recv_len);
  if( recv_len!= -1) 
  {
   if (debug.is_debug_mode)
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
    char *fromip = inet_ntoa(client_addr.sin_addr);
    printf("Receive Data at: %s %ld ms (%d) from %s\n",ctime(&tm), now.tv_usec/1000,hb_count,fromip);
    // printf("Receive Data at: %s %ld ms (%d--%d,%d,%d)\n",ctime(&tm), now.tv_usec/1000,hb_count,message_count[0],message_count[1],message_count[2]);
    }
   
   //printf("Receive Data OK\n");
   //if (!is_busy)
   {
     //is_busy=1;
     //hb_msg_len=hb_msg_len*17;
     
     char ret=parsekHeartBeatMessage(buffer);
     /*
     if (ret)
     {
        on_recv_hb();
     }
     else if (debug.is_debug_mode)
        err_msg_count++;
     */
      if (!ret && debug.is_debug_mode)
        err_msg_count++;

     if (enabledReply)
     {
       if (ret)
          sendto(server_socket_fd,ehco,strlen(ehco),0,(struct sockaddr*) &client_addr,client_addr_length);
        else
         sendto(server_socket_fd,"Error",strlen("Error"),0,(struct sockaddr*) &client_addr,client_addr_length);
     }
     //is_busy=0;
  }
  //else
  // printf("busy...\n"); 
   //exit(1); 
  } 
  else if (debug.is_debug_mode)
  {
      err_msg_count++;
      perror("Receive Data Failed:\n");
  }

  /* 从buffer中拷贝出file_name */
/*
  char file_name[FILE_NAME_MAX_SIZE+1]; 
  bzero(file_name,FILE_NAME_MAX_SIZE+1); 
  strncpy(file_name, buffer, strlen(buffer)>FILE_NAME_MAX_SIZE?FILE_NAME_MAX_SIZE:strlen(buffer)); 
  printf("%s\n", file_name); 
  */
 } 
 close(server_socket_fd); 
 return ; 
}

void usage()
{
  printf("Description:\n");
  printf("Heart beat.\r\nver B2.0.6\r\nLast modifed:%s\n",LAST_MODIFY_DATE);
  printf("-s for sleep.\n");
  printf("-d for debug.\n");
  printf("-h for help.\n");
}

void init_hb_conf(int argc, char **argv)
{
  char slots[15][20]={""};
  char accept_ips[6][20];
  int slot_count;
  int n,m=0,slot,count;

  char value[30]="";

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
  slot_count=getConfItems(SYS_CONF_FILE,"hb_slots",slots,15,",");
  if (slot_count>15)
     slot_count=15;
  //printf("slot_count=%d\r\n",slot_count);
  for (n=0;n<slot_count;n++)
  {
    slot=atoi(slots[n]);
    //printf("Slot=%d :%s\r\n",slot,slots[n]);
    if (slot<1 || slot>15)
      continue;  
    enabled_hb_units[n]=slot;
  }

  //printf("%0x %0x\r\n",ac_msg[0],ac_msg[1]);
  getConfValue(SYS_CONF_FILE,"ehco",ehco," ");
  
  /*
  getConfValue(SYS_CONF_FILE,"switch_one",value," ");
  if (value[0]=='Y')
    exit_after_switch=1;
  */
}


int main(int argc, char **argv)
{
   //char enabled_hb_units[16]={1,2,3,4,5};
   /*
   int n;
   for (n=0;n<6;n++)
   {
      enabled_hb_units[n]=n+1;
   }
   printf("\n");
   */
   initAlarmConf();
   init_hb_conf(argc,argv);
   //enabled_hb_units[0]=1;
   signal(SIGALRM, on_tick);
  /*
    int res = 0;
    struct itimerval tick;
    
    signal(SIGALRM, printMes);
    memset(&tick, 0, sizeof(tick));

    //Timeout to run first time
    tick.it_value.tv_sec = 0;
    tick.it_value.tv_usec = 500000;

    //After first, the Interval time for clock
    tick.it_interval.tv_sec = 0;
    tick.it_interval.tv_usec = 500000;

    if(setitimer(ITIMER_REAL, &tick, NULL) < 0)
         printf("Set timer failed!\n");
    */
    
    start(1);
    //When get a SIGALRM, the main process will enter another loop for pause()
    /*
    while(1)
    {
      srand((int) time(0));
      int rd=rand();
      if (rd%10>7)
        invalid_heart_beat++;
      else if (rd%10==5)
        invalid_heart_beat=0;
      usleep(100000);
        //pause();
    }
    */
    int port=getConfIntValue(SYS_CONF_FILE,"port"," ");
    if (port<1)
      port=DEFAULT_PORT;
    start_heart_beat(port);
    return 0;
}
