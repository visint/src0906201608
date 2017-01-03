/*
 *  基于udp通信 server端
 *
 * */
//arm-linux-gcc -o udpserver  udp_server.c -I../include -L=./ -L/home/visint/src/arm/lib -lvispace -lsqlite3 -liconv
//arm-linux-gcc -o heartbeat  udp_server.c -I../include -L=./ -L/home/visint/src/arm/lib -lvispace -lsqlite3 -liconv
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <public.h>
#include <unit.h>

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


#define ARM_LINUX 1
#define DEFAULT_PORT    2012

#define PORT    2012
#define BUFFER_SIZE 64
int slot_of_obp=1;

boolean_t setObpToProtected(uchar_t slot)
{
 int n;
 uchar_t uc=0,ut=0;
 char *values=NULL;
 
 getUnitClassType(0,slot,&uc,&ut);
 //printf("setObpToProtected slot=%d uc=%d,ut=%d\n",slot,uc,ut);
 if (!(uc==UC_OLP && ut>50))
    return FALSE;
 values=getInfoSetValues(0,slot,OLP_STATUS_INFOSET_ID,1);
 if (values==NULL)
    return FALSE;
 
 if (values[14]==2 && values[15]==2)
 {
     setConfValue(SYS_CONF_FILE,"hb_err","N"," ");
     printf("OBP in slot #%d was in protected mode\n",slot_of_obp);
     exit(0);
 }

 char message[512];
 memset(message,0,512*sizeof(char));
 message[0]='S';
 message[1]=OLP_STATUS_INFOSET_ID;
 message[2]=1;
 message[3]=8;
 message[4]=0;
 message[5]=2;
 message[6]=9;
 message[7]=0;
 message[8]=2;
 sendMessageToUnit(0,slot,message,300);
 for (n=0;n<5;n++)
 {
   sleep(1);
   values=getInfoSetValues(0,slot,OLP_STATUS_INFOSET_ID,1);
   if (values==NULL)
    return FALSE;
 
    if (values[14]==2 && values[15]==2)
    {
     setConfValue(SYS_CONF_FILE,"hb_err","N"," ");
     printf("OBP in slot #%d was switched to protected mode OK\n",slot_of_obp);
     exit(0);
    }
 }
 return FALSE;
}

static void sigHandle(int sig,struct siginfo *siginfo,void *myact)
{
  if (sig == SIGALRM)
  {
    /*
    if (setObpToProtected(slot_of_obp)!=FALSE)
      exit(0);
    */
    setObpToProtected(slot_of_obp);
    alarm(10);
  }
}
static void sigInit()
{
  struct sigaction act;
  sigemptyset(&act.sa_mask);  
  act.sa_flags=SA_SIGINFO;
  act.sa_sigaction=sigHandle;
  sigaction(SIGALRM,&act,NULL);
}

static void hearbeatHandle()
{
  char value[30]="";
  getConfValue(SYS_CONF_FILE,"hb_enable",value," ");
  printf("hearbeatHandle hb_enable=%c\n",value[0]);
  if (value[0]!='Y')
    exit(0);
  value[0]=0;
  /*
   getConfValue(SYS_CONF_FILE,"hb_err",value," ");
  printf("hearbeatHandle hb_err=%c\n",value[0]);
  if (value[0]=='N')
    return;
  */
  printf("hearbeatHandle\n");
  sigInit();
  alarm(1);
  while (1)
  {	
    pause();
  }
}


void startListening(char is_debug)
{
  int oc,n,retry=3,second=1,msecond=10,waitting=3,interval=10,port=DEFAULT_PORT;
  char accept_ip_list[10][20]={""};
  //char soure_fail_count[10]={0};
  char ac_msg[20]={0x71,0x5a};
  int soure_count=0;
  int ac_msg_len=0;
  
  #ifdef ARM_LINUX
  if (lockFile("/vispace/hb.lck")<0)
  {
     return;
  }
  
  char value[30]="";
  getConfValue(SYS_CONF_FILE,"hb_enable",value," ");
 
  if (value[0]!='Y')
    exit(0);
  value[0]=0;
  /*
   getConfValue(SYS_CONF_FILE,"hb_err",value," ");
  if (value[0]=='Y')
    return;
  */
  soure_count=getConfItems(SYS_CONF_FILE,"accept_ip",accept_ip_list,10,";");
  if (soure_count<1)
  {
      printf("Not any source Ip to listen!Please add source ip %d!\n",soure_count);
      return;
  }

  getConfValue(SYS_CONF_FILE,"hb_msg",value," ");
  ac_msg_len=strtoca(value,";",ac_msg);
  /*
   for(n=0;n<ac_msg_len;n++)
    printf("msg_len:%d ac_msg:%0x\n",ac_msg_len,ac_msg[n]);
  */
    slot_of_obp=getConfIntValue(SYS_CONF_FILE,"slots"," ");

    waitting=getConfIntValue(SYS_CONF_FILE,"waitting"," ");
    if (waitting<1)
        waitting=1;
    retry=getConfIntValue(SYS_CONF_FILE,"retry"," ");
    if (retry<1)
        retry=1;
    second=getConfIntValue(SYS_CONF_FILE,"sec"," ");
    if (second<0)
        second=0;
    if (second>10)
        second=10;
    msecond=getConfIntValue(SYS_CONF_FILE,"msec"," ");
    if (second<1 && msecond<10)
       msecond=10;

    interval=getConfIntValue(SYS_CONF_FILE,"interval"," ");
    port=getConfIntValue(SYS_CONF_FILE,"port"," ");
 
   
  #endif
  if (is_debug)
  {
    printf("Waitting for request(%d) from:",soure_count);
    for (n=0;n<soure_count;n++)
    {
       printf("%s ",accept_ip_list[n]);
    }
    printf("\nConfigure param:\nslot of OBP=%d\nport=%d\nwaitting=%d\nretry=%d\nsecond=%d\nmsecond=%d\ninterval=%d\n",slot_of_obp,port,waitting,retry,second,msecond,interval);
  }
  sleep(waitting);
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
  server_addr.sin_port = htons(PORT);
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
  int count=0;
  struct timeval t_start,t_end;
  long start,end,timeout;

  timeout=retry*(second*1000+msecond+interval);
  if (is_debug)
    printf("Starting heartbeat.\nWaitting for request from client...\n");

  setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,&re_flag,re_len);
  gettimeofday(&t_start, NULL); 
  start = ((long)t_start.tv_sec)*1000+(long)t_start.tv_usec/1000;
  char heart_beat_status=0;//心跳状态:0-不正常,非0-正常
  char valid_heart_beat_count=0;
  while(1)        
  {
    FD_ZERO(&readfds);
    FD_SET(sockfd,&readfds);
    tv.tv_sec = second;  //设置超时时间为3s
    tv.tv_usec = msecond*1000;


    select(sockfd+1,&readfds,NULL,NULL,&tv);

    if(FD_ISSET(sockfd,&readfds))
    {
      if((recvfrom(sockfd,buf,BUFFER_SIZE,0,(struct sockaddr*) &client_addr,&len)) >= 0)   //3s内收到客户端发来的消息，响应
      {
           printf("Run here!\r\n");
           printf("From ip:%s in time,left time %d s,%d us\n",inet_ntoa(client_addr.sin_addr),(int)tv.tv_sec,(int)tv.tv_usec);
          if (count<10)
          {
              count++;
              gettimeofday(&t_start, NULL);
              //start = ((long)t_start.tv_sec)*1000+(long)t_start.tv_usec/1000;
              continue;
          }
          /*
          if (ac_msg_len>0)
          {
            if (memcmp(buf,ac_msg,ac_msg_len)!=0)
            {
               gettimeofday(&t_end, NULL);
               end = ((long)t_end.tv_sec)*1000+(long)t_end.tv_usec/1000;
               if (end>(start+timeout))
               {
                 printf("timeout = %d\n", end-start);
                 break;
               }
               continue;
            } 
          }
          */
          if (is_debug)
           printf("in time,left time %d s,%d us\n",(int)tv.tv_sec,(int)tv.tv_usec);

          char *fromip = inet_ntoa(client_addr.sin_addr);   //获取发送方的IP地址
          //if (is_debug)
            printf("fromip:%s in time,left time %d s,%d us\n",fromip,(int)tv.tv_sec,(int)tv.tv_usec);
          for (n=0;n<soure_count;n++)//判断发送方的IP地址是否在IP列表里
          {
             if (strstr(accept_ip_list[n],fromip))
             {
               //soure_fail_count[n]=0;
               if (valid_heart_beat_count<1)
                  gettimeofday(&t_start, NULL);
               if (valid_heart_beat_count<soure_count)
                  valid_heart_beat_count++;
               //start = ((long)t_start.tv_sec)*1000+(long)t_start.tv_usec/1000;
               break;
             }
          }
          if (valid_heart_beat_count>0 && soure_count>1)
          {
             gettimeofday(&t_end, NULL);
             start = ((long)t_start.tv_sec)*1000+(long)t_start.tv_usec/1000;
             end = ((long)t_end.tv_sec)*1000+(long)t_end.tv_usec/1000; 

             if (end>(start+timeout))
             {
                if (heart_beat_status!=0)
                {
                    heart_beat_status=0;
                    valid_heart_beat_count=1;
                    gettimeofday(&t_start, NULL);
                    printf("At least one heart stop beat!\r\n");
                    continue;
                }
             }
          }
          printf("All heart start beat!%d %d\r\n",valid_heart_beat_count,soure_count);
          if (valid_heart_beat_count==soure_count)
          {
             if (heart_beat_status!=1)
             {               
                 heart_beat_status=1;
                 printf("All heart start beat!\r\n");
             }
             valid_heart_beat_count=0;
             gettimeofday(&t_start, NULL);
          }
          /*
          if (n==soure_count)
          {
            
            gettimeofday(&t_end, NULL);
            start = ((long)t_start.tv_sec)*1000+(long)t_start.tv_usec/1000;
            end = ((long)t_end.tv_sec)*1000+(long)t_end.tv_usec/1000; 

            if (end>(start+timeout))
            {
              if (is_debug)
                printf("timeout = %d\n", end-start);
              //setConfValue(SYS_CONF_FILE,"hb_err","Y"," ");
              break;
            }
            if (is_debug)
            printf("Client:%s connet ok,but access denied\n",fromip);
          }
          if (is_debug)
          {
           printf("Client:%s contected Ok!\n",fromip);
           //printf("Recv message:%s\n",buf); 
           //sendto(sockfd,"welcome",8,0,(struct sockaddr*) &client_addr,len);
          }
          */
      }
    }
    else //3s内没有收到客户端的消息，超时
    {
      
       gettimeofday(&t_end, NULL);
       start = ((long)t_start.tv_sec)*1000+(long)t_start.tv_usec/1000;
       end = ((long)t_end.tv_sec)*1000+(long)t_end.tv_usec/1000; 
       printf("timeout = %d(ms)\n", end-start);
       if (end>(start+timeout))
       {
          if (heart_beat_status!=0)
          {               
              heart_beat_status=1;
              printf("All heart stop beat!\r\n");
          }
          if (is_debug)
              printf("timeout = %d(ms)\n", end-start);
            //setConfValue(SYS_CONF_FILE,"hb_err","Y"," ");
            //break;
       }
       if (is_debug)
       {
             // printf(" (end-start)=%d timeout %d,left time %d s,%d us.\n",end-start,timeout,(int)tv.tv_sec,(int)tv.tv_usec);             
             // break;
       }
       if (heart_beat_status!=0)
       {
           printf("heart_beat stop! (end-start)=%d timeout %d,left time %d s,%d us.\n",end-start,timeout,(int)tv.tv_sec,(int)tv.tv_usec); 
       }
      /*
      for (n=0;n<soure_count;n++)
      { 
         if (soure_fail_count[n]<retry)
            soure_fail_count[n]++;
         else
         {
           if (is_debug)
           {
              printf("source ip:%s timeout,left time %d s,%d us.\n",accept_ip_list[n],(int)tv.tv_sec,(int)tv.tv_usec);    
              break;
           }
         }
      }
      printf("timeout,left time %d s,%d us n=%d soure_count=%d\n",(int)tv.tv_sec,(int)tv.tv_usec,n,soure_count);
      if (n==soure_count)
      {
           //setConfValue(SYS_CONF_FILE,"hb_err","Y"," ");
           printf("Timeout,left time %d s,%d us.\n",(int)tv.tv_sec,(int)tv.tv_usec);
           break;
      }
      */
    }
  }
  /* 结束通信 */
  close( sockfd );
  return;
}
void main(int argc, char **argv)
{
  int oc;
  char is_save=0,is_debug=0;
   while((oc = getopt(argc, argv, "a:b:c:de:hl:r:n:m:p:s:tw:v")) != -1)
  {
        switch(oc)
        {
          case 'd':
            is_debug=1;
          break;
        }
  }
  startListening(is_debug);
  hearbeatHandle();
}

