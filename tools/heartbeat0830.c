/*
 *  基于udp通信 server端
 *
 * */
//arm-linux-gcc -o heartbeat  heartbeat.c -I../include -L=./ -L/home/visint/src/arm/lib -lvispace -lsqlite3 -liconv
//arm-linux-gcc -o heartbeat  heartbeat.c -I../include -L=./ -L/home/visint/src/arm/lib -lvispace -lsqlite3 -liconv
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <public.h>
#include <unit.h>
#include <sysinfo.h>
#include <visipc.h>
#include <chassis.h>
#define DEFAULT_PORT    2012

typedef struct
{
  uchar_t slot;
  char accept_ips[6][20];
  uchar_t ip_count;
  char recieved_ips[6];
  char status;
  unsigned long first_found_time;
}heart_beat_t;

void sendHeartbeat(uchar_t slot,char status);

heart_beat_t hb_source[15];
static char heart_beat_status=0;//心跳状态:0-不正常,非0-正常
static char valid_heart_beat_count=0;
static int slot_of_obp=1;
static int retry=3,second=1,msecond=10,waitting=3,interval=10,port=DEFAULT_PORT;
static long timeout_msec=10;
static int slot_count=0;
static int hb_slot_count=0;
static char slot_list[15];
static char accept_ip_list[15][10][20]={""};

  //char soure_fail_count[10]={0};
static  char ac_msg[16]={0x71,0x5a};
static  int soure_count[15]={0};
static  int ac_msg_len=2;

void reset_heart_beat(uchar_t slot)
{
  if (slot>14)
     return;
  uchar_t n,index=0;
  for (n=0;n<hb_slot_count;n++)
  {
     if (hb_source[n].slot==slot)
     {
         index=n;
         break;
     }
  }
  for (n=0;n<hb_source[index].ip_count;n++)
    hb_source[index].recieved_ips[n]=0;
}
void reset_all_heart_beat()
{

}
void check_source(char *fromip,char message[])
{
  int n,m;
  //printf("hb_slot_count:%d fromip=%s ac_msg:%0x%0x\r\n",hb_slot_count,fromip,message[0],message[1]);
  
  for (n=0;n<ac_msg_len;n++)
  {
    if (ac_msg[n]!=message[n])
      return;
  }
  //printf("ac_msg is OK\r\n");
  for (n=0;n<hb_slot_count;n++)
  {
    for (m=0;m<hb_source[n].ip_count;m++)
    {
       if (strstr(hb_source[n].accept_ips[m],fromip))
       {
          if (hb_source[n].first_found_time<1)
          {
            struct timeval t_start;
            gettimeofday(&t_start, NULL);
            hb_source[n].first_found_time= ((unsigned)t_start.tv_sec)*1000+(unsigned)(t_start.tv_usec*0.001);
          }
          if (!hb_source[n].recieved_ips[m])
            hb_source[n].recieved_ips[m]=1;
          break;
       }
    }
  }
}

void check_heart_beat()
{
  uchar_t n,m;
  unsigned long now;
  struct timeval t_end;
  uchar_t uc=0,ut=0;
  char status=1;
  gettimeofday(&t_end, NULL);
  now = ((unsigned)t_end.tv_sec)*1000+(unsigned)(t_end.tv_usec*0.001);
  for (n=0;n<hb_slot_count;n++)
  {
    getUnitClassType(0,hb_source[n].slot,&uc,&ut);
    if (!(uc==UC_OLP && ut>50))
       continue;
    //printf("slot=%d status=%d.1\r\n",hb_source[n].slot,status);
    if (hb_source[n].first_found_time>0 && now>(hb_source[n].first_found_time+timeout_msec))
    {
      hb_source[n].first_found_time=0;
      if (hb_source[n].status!=0)
      {
       printf("slot=%d timeout first_found=%d!\r\n",hb_source[n].slot,hb_source[n].first_found_time);
       hb_source[n].status=0;
       sendHeartbeat(hb_source[n].slot,0);
      }
      printf("slot=%d now=%d first_found_time:%d timeout:%d\r\n",hb_source[n].slot,now,hb_source[n].first_found_time,timeout_msec);
      continue;
    }
    //printf("slot=%d status=%d.\r\n",hb_source[n].slot,status);
    for (m=0;m<hb_source[n].ip_count;m++)
    {
      if (hb_source[n].recieved_ips[m]!=1)
      {
         if (hb_source[n].status!=0)
         {
           printf("sendHeartbeat\r\n");
           sendHeartbeat(n+1,0);
           hb_source[n].status=0;
         }
         status=0;
         hb_source[n].first_found_time=0;
         break;
      }
    }
    //printf("slot=%d status=%d\r\n",hb_source[n].slot,status);
    if (status)
    {
       hb_source[n].first_found_time=0;

       if (hb_source[n].status!=1)
       {
          sendHeartbeat(n+1,1);
          hb_source[n].status=1;
       }
    }
  }
}
void sendHeartbeat(uchar_t slot,char status)
{
 uchar_t uc=0,ut=0;
 char *values=NULL;
 reset_heart_beat(slot);
 getUnitClassType(0,slot,&uc,&ut);
 if (!(uc==UC_OLP && ut>50))
    return;
 char message[512];
 memset(message,0,512*sizeof(char));
 message[0]='S';
 message[1]=OLP_STATUS_INFOSET_ID;
 message[2]=1;
 message[3]=22;
 message[4]=0;
 if (status)
 {
  message[5]=1;
  printf("Heartbeat start!\r\n");
 }
 else
 {
   message[5]=2;
   printf("Heartbeat stop!\r\n");
 }
 sendMessageToUnit(0,slot,message,300);
}
/*
void onRecieved(char *fromip,char message[])
{
  static char start_flag=0,first_found=0;
  static struct timeval t_start;
  struct timeval t_end;
  static long start=0;
  char is_timeout=0;
  static char valid_source[10]={0};
 // if (!heart_beat_status)
  {
    int n,m;
    for (n=0;n<slot_count;n++)
    {
     for (m=0;m<soure_count[n];m++)//判断发送方的IP地址是否在IP列表里
     {
        if (strstr(accept_ip_list[n][m],fromip))
        {
               //soure_fail_count[n]=0;
               //if (valid_heart_beat_count<1)
               //   gettimeofday(&t_start, NULL);
             if (valid_heart_beat_count<soure_count)
                 valid_source[n]=1;
                  //valid_heart_beat_count++;
             if (!first_found)
             {
               first_found=1;
               start_flag=1;
               gettimeofday(&t_start, NULL);
               start= ((long)t_start.tv_sec)*1000+(long)(t_start.tv_usec*0.001);
             }
               //start = ((long)t_start.tv_sec)*1000+(long)t_start.tv_usec/1000;
             break;
         }
    }
   }
    if (start_flag==0)
    {
       gettimeofday(&t_start, NULL);
       start= ((long)t_start.tv_sec)*1000+(long)(t_start.tv_usec*0.001);
       if (start_flag!=1)
        start_flag=1;
    }
    if (start_flag!=0)
    {
      long end;
      gettimeofday(&t_end, NULL);
      end= ((long)t_end.tv_sec)*1000+(long)(t_end.tv_usec*0.001);
      if (end>(timeout_msec+start))
      {
         start_flag=0;
         is_timeout=1;
         printf("time:%d\r\n",end-start);
      }
    }
    if (!is_timeout)
    {
       int n;
        for (n=0;n<soure_count;n++)
        {
          if (valid_source[n]!=1)
              return;
        }
       if (start_flag!=0)
       {
        start_flag=0;
        first_found=0;
       }
       if (heart_beat_status!=1)
       {
        //start_flag=0;
        //first_found=0;
        heart_beat_status=1;
        for (n=0;n<soure_count;n++)
           valid_source[n]=0;
        sendHeartbeat(1);
       }
       
    }
    else
    {
       if (heart_beat_status!=0)
       {
        heart_beat_status=0;
        printf("is_timeout\r\n");
        sendHeartbeat(0);
       }
    }
  }
}
*/
void onTimeOut()
{
  uchar_t n;
  //printf("onTimeOut\r\n");
  for (n=0;n<hb_slot_count;n++)
  {
    printf("status[%d]=%d \r\n",n,hb_source[n].status);
    if (hb_source[n].status!=0)
    {
       hb_source[n].status=0;
       sendHeartbeat(n+1,0);
    }
  }
  /*
  if (heart_beat_status)
  {
     printf("Send heartbeat to stop\r\n");
     heart_beat_status=0;
     valid_heart_beat_count=0;
     sendHeartbeat(0,0);
  }
  */
}
//毫秒级别定时器
/*void milliseconds_sleep(unsigned long mSec){
	struct timeval tv;
	tv.tv_sec=mSec/1000;
	tv.tv_usec=(mSec%1000)*1000;
	int err;
	do{
	   err=select(0,NULL,NULL,NULL,&tv);
	}while(err<0 && errno==EINTR);
}
*/
void startListening(char is_debug)
{
  //#define PORT    2012
  #define BUFFER_SIZE 8
  /* 创建socket */
  int sockfd = socket( PF_INET,SOCK_DGRAM,0 );
  if( sockfd == -1 )   //创建socket失败
  {
    perror("socket");
    exit(-1);
  }

  /* 准备UDP通信地址 */
  struct sockaddr_in server_addr;
  struct sockaddr_in client_addr;
  
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(port);
  server_addr.sin_addr.s_addr = INADDR_ANY;    //本机IP
  memset(server_addr.sin_zero,0,8);
  
  /* 绑定通信地址 */
  int res = bind(sockfd,(struct sockaddr*) &server_addr,sizeof(server_addr));
  if(res == -1)   //bind失败
  {
    perror("bind");
    exit(-1);
  }
  printf("bind ok!\nwait for client...\n");   //bind成功
  
  /* 开始通信 */
  struct timeval tv;  //创建设置超时的结构体
  fd_set readfds;     //可读的文件描述符
  char buf[BUFFER_SIZE];     //用于接收客户端发来的消息
  socklen_t len;      //

  int addr_len = sizeof(struct sockaddr);
  int re_flag = 1;
  int re_len = sizeof(int);

  int second=1,msecond=10;
  second=getConfIntValue(SYS_CONF_FILE,"sec"," ");
    if (second<0)
        second=0;
    if (second>10)
        second=10;
    
    msecond=getConfIntValue(SYS_CONF_FILE,"msec"," ");
    //printf("second=%d msecond=%d\r\n",second,msecond);
    if (second<1 && msecond<10)
       msecond=10;
  timeout_msec=second*1000+msecond;
  setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,&re_flag,re_len);

  while(1)        
  {
    FD_ZERO(&readfds);
    FD_SET(sockfd,&readfds);
    tv.tv_sec = second;  //设置超时时间为3s
    tv.tv_usec = msecond*1000;
    //printf("tv_sec=%d,tv_usec=%d msecond=%d\r\n",tv.tv_sec,tv.tv_usec,msecond);
    select(sockfd+1,&readfds,NULL,NULL,&tv);
    if(FD_ISSET(sockfd,&readfds))
    {
      if((recvfrom(sockfd,buf,BUFFER_SIZE,0,(struct sockaddr*) &client_addr,&len)) >= 0)   //3s内收到客户端发来的消息，响应
      {
          //printf("in time,left time %d s,%d us\n",(int)tv.tv_sec,(int)tv.tv_usec);
                  
          if (tv.tv_sec>0 || tv.tv_usec>0)
          {
             char *fromip = inet_ntoa(client_addr.sin_addr);   //获取发送方的IP地址
             //printf("Received from %s\r\n:",fromip);
              //onRecieved(fromip,buf);
             check_source(fromip,buf);
             check_heart_beat();
          }
          else
              onTimeOut();
          //printf("Client %s connect OK\n",fromip);
          
          //printf("received:%0x %0x from %s \n",buf[0],buf[1],fromip);  
          sendto(sockfd,"welcome\n",9,0,(struct sockaddr*) &client_addr,len);
      }
    }
      else //3s内没有收到客户端的消息，超时
      {
        printf("Timeout,left time %d s,%d us\n",(int)tv.tv_sec,(int)tv.tv_usec);
        onTimeOut();
      }
  }
  /* 结束通信 */
  close( sockfd );
  return;
}

void main()
{
  //#ifdef ARM_LINUX
  if (lockFile("/vispace/hb.lck")<0)
  {
     exit(-1);
  }
  char slots[15][20]={""};
  char accept_ips[6][20];
  //int slot_count;
  int n,m=0,slot,count;

  char value[30]="";
  slot_count=getConfItems(SYS_CONF_FILE,"slots",slots,15,";");
  //printf("slot_count=%d\r\n",slot_count);
  for (n=0;n<slot_count;n++)
  {
    slot=atoi(slots[n]);
    printf("Slot=%d :%s\r\n",slot,slots[n]);
    if (slot<1 || slot>15)
      continue;
    
    sprintf(value,"accept_ip%d",slot);
    
    count=getConfItems(SYS_CONF_FILE,value,hb_source[n].accept_ips,6,";");
    //printf("IPS=%s slot=%d,count=%d m=%d IP:%s\r\n",buf,slot,count,m,hb_source[n].accept_ips);
    if (slot>0 && count>0)
    {
      hb_source[m].slot=(uchar_t)slot;
     // strcpy(hb_source[n].accept_ips[m],accept_ips);
      hb_source[m].ip_count=count;
      hb_source[m].status=1;
      m++;
    }
  }
  hb_slot_count=m;
  /*
  for (n=0;n<hb_slot_count;n++)
  {
     printf("Slot=%d:\r\n",hb_source[n].slot);
     for (m=0;m<hb_source[n].ip_count;m++)
       printf("accept_ip_list:%s\r\n",hb_source[n].accept_ips[m]);
     printf("\r\n");
  }
  
  if (soure_count<1)
  {
      printf("Not any source Ip to listen.\r\nPlease add source ip %d!\n",soure_count);
      exit(-1);
  }
  
  for (n=0;n<5;n++)
    printf("accept_ip_list:%s\r\n",accept_ip_list[n]);
 */
  port=getConfIntValue(SYS_CONF_FILE,"port"," ");
 
  if (port<1)
    port=DEFAULT_PORT;

  getConfValue(SYS_CONF_FILE,"ac_msg",value," ");
  ac_msg_len=strtoca(value,";",ac_msg);
  if (ac_msg_len>16)
    ac_msg_len=16;
  printf("ac_msg_len=%d %0x%0x\r\n",ac_msg_len,ac_msg[0],ac_msg[1]);
  /*
  if (soure_count>15)
     soure_count=15;
  printf("soure_count=%d\r\n",soure_count);
  slot_of_obp=getConfIntValue(SYS_CONF_FILE,"slots"," ");
*/
  startListening(0);
}

