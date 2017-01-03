/*
 *  基于udp通信 server端
 *
 * */
//arm-linux-gcc -o heartbeat  heartbeat.c -I../include -L=./ -L/home/visint/src/arm/lib -lvispace -lsqlite3 -liconv
//arm-linux-gcc -o heartbeat  heartbeat.c -I../include -L=./ -L/home/visint/src/arm/lib -lvispace -lsqlite3 -liconv
//http://www.cnblogs.com/cobbliu/p/3627061.html

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <time.h>
#include <sys/time.h>
#include <signal.h>

#include <public.h>
#include <unit.h>
#include <sysinfo.h>
#include <visipc.h>
#include <chassis.h>
#include <olp.h>
#include <vispaceio.h>

#define DEFAULT_PORT    2012

typedef struct
{
  uchar_t slot;
  char accept_ips[6][20];
  uchar_t ip_count;
  char recieved_ips[6];
  char status;
  char enable;
  //unsigned long start_time;
  //unsigned long first_found_time;
}heart_beat_t;

typedef struct
{
 char unit_type;
 char working_line;
}obp_info_t;

void sendHeartbeat(uchar_t slot,char status);

heart_beat_t hb_source[15];
//static char heart_beat_status=0;//心跳状态:0-不正常,非0-正常
static char valid_heart_beat_count=0;
//static int slot_of_obp=1;
static int retry=3,second=1,msecond=10,waitting=10,interval=10,port=DEFAULT_PORT;
static int msg_type=1;//心跳方式:1-系统默认,2-用户指定
static long timeout_msec=10;
static int slot_count=0;
static int hb_slot_count=0;
//static char slot_list[15];
static char accept_ip_list[15][10][20]={""};

  //char soure_fail_count[10]={0};
static  char ac_msg[36]={0x71,0x5a};//确认码
static  char ehco[16]="";
static  int soure_count[15]={0};
static  int ac_msg_len=2;//确认码长度
//static long first_start=0;
static is_setting=0;
static char exit_after_switch=0;//切换后是否停止心跳监控
obp_info_t obp_info[16];
static uchar_t slots_count;
static uchar_t unit_count;
static char is_send_ehco=1;
static char is_debug=0;
static char invalid_message=0;
void reset_heart_beat(uchar_t slot)
{
  if (slot>15)
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
char getCheckSum(char message[],unsigned char from,unsigned char to)
{
  unsigned char n;
  int sum=0;
  for (n=from;n<=to;n++)
    sum+=(char unsigned)message[n];
  return 0xff & sum;
}
/*
char getHeartBeatStatus(char message[],char status[])
{
  status[0]=message[12];
  status[1]=message[13];
  status[2]=message[14];
  status[3]=message[15];
  return 1;
}
*/
char parsekHeartBeatMessage(char message[],char slots[])
{
 int n,m;
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

  //printf("Run in check_source...hb_slot_count:%d fromip=%s ac_msg:%0x%0x\r\n",hb_slot_count,fromip,message[0],message[1]);
/*示例:
发送：发送盘1、2、3、4 心跳(或第一路心跳)
ef ef 00 00 0c 00 00 FF 16 00 01 00 00 0f 00 00 0f
返回：FE FE 00 00 16 00 00 FF 16 00 01 28 04 04 01 01 00 02 01 00 03 01 00 04 01 00 66
*/
  char *ptr,flag[]={0xEF,0xEF,0x00},comm[]={0xFF,0x16};
  int check_sum=0;

  for (n=0;n<8;n++)
  {
     slots[n]=(message[13]>>n) & 0x01;
     slots[n+8]=(message[12]>>n) & 0x01;
  }

  if (memcmp(message,flag,3)!=0)
  {
     if (is_debug)
       printf("FLAG2 is Error\r\n");
     return 0;
  }
  ptr=message+7;

  if (memcmp(ptr,comm,2)!=0)
  {
     if (is_debug)
     printf("COMM is Error\r\n");
     return 0;
  }
  /*
  status[0]=message[12];
  status[1]=message[13];
  status[2]=message[14];
  status[3]=message[15];
  */
  return 1;
}
void check_source(char *fromip,char message[],char slots[])
{
  int n,m;
  //printf("Run in check_source...hb_slot_count:%d fromip=%s ac_msg:%0x%0x\r\n",hb_slot_count,fromip,message[0],message[1]);
 //FE FE 00 01 00 00 00  FF 16 00 03 00 03
/*示例:
发送：发送盘1、2、3、4 心跳(或第一路心跳)
ef ef 00 00 0c 00 00 FF 16 00 01 00 00 0f 00 00 0f
返回：FE FE 00 00 16 00 00 FF 16 00 01 28 04 04 01 01 00 02 01 00 03 01 00 04 01 00 66
*/
  /*char *ptr,flag[]={0xEF,0xEF,0x00},comm[]={0xFF,0x16};
  char slots[16]={0};
  int check_sum=0;
  printf("FLAG2=%x%x%x\r\n",message[0],message[1],message[2]);
  printf("COMM=%x%x\r\n",message[7],message[8]);
  printf("DATA=%X %X\r\n",message[12],message[13]);
  for (n=0;n<8;n++)
  {
     slots[n]=(message[13]>>n) & 0x01;
     slots[n+8]=(message[12]>>n) & 0x01;
  }
  for (n=0;n<16;n++)
  {
  //printf("%d ",slots[n]);
  // if (n==11)
    //    printf("\r\nHSUM:%0x\r\n",check_sum);
   check_sum+=(char unsigned)message[n];
  }
  printf("\r\nHSUM:%0x SUM:%0x\n",getCheckSum(message,0,10),getCheckSum(message,0,15));
  //printf("\r\nSUM:%0x\r\n",check_sum);
  for (n=0;n<17;n++)
    printf("%X ",message[n]);
  printf("\r\n");
  if (memcmp(message,flag,3)==0)
    printf("FLAG2 is OK\r\n");
  else
    printf("FLAG2 is Error\r\n");
  ptr=message+7;

  if (memcmp(ptr,comm,2)==0)
    printf("COMM is OK\r\n");
  else
    printf("COMM is Error\r\n");
  */
  /*
  for (n=0;n<ac_msg_len;n++)
  {
    if (ac_msg[n]!=message[n])
      return;
  }
  */
  //printf("ac_msg is OK\r\n");
  invalid_message=1;
  for (n=0;n<hb_slot_count;n++)
  {
    if (slots[hb_source[n].slot-1]==1)
    {
      for (m=0;m<hb_source[n].ip_count;m++)
      {
        if (!strcmp(hb_source[n].accept_ips[m],fromip))
        {
          if (!hb_source[n].recieved_ips[m])
            hb_source[n].recieved_ips[m]=1;
          if (invalid_message!=0)
              invalid_message=0;
          //if (hb_source[n].slot==13)
          //printf("m:%d Slot=%d fromip=%s recieved_ips[%d]=%d\r\n",m,hb_source[n].slot,fromip,m,hb_source[n].recieved_ips[m]);
          //break;
       }
       //if (hb_source[n].slot==13)
      //     printf("hb_source[n].ip_count:%d IP:(%s) from ip(%s)\r\n",hb_source[n].ip_count,hb_source[n].accept_ips[m],fromip);
    }
   }
  }
}
char heartbeat_state=0;

void check_heart_beat()
{
  uchar_t n,m;
  uchar_t uc=0,ut=0;
  char status;
  time_t now_time;
  time(&now_time);
  printf("check_heart_beat:hb_slot_count=%d\n",hb_slot_count);
  for (n=0;n<hb_slot_count;n++)
  {
    uc=0;
    ut=0;
    status=1;
    getUnitClassType(0,hb_source[n].slot,&uc,&ut);
    printf("check_heart_beat:#%d uc=%d ut=%d\n",hb_source[n].slot,uc,ut);
    if (!(uc==UC_OLP && ut>50))
       continue;
    printf("check_heart_beat:#%d\r\n",hb_source[n].slot);
    for (m=0;m<hb_source[n].ip_count;m++)
    {
     // printf("slot=%d hb_source[n].recieved_ips[m]=%d\r\n",hb_source[n].slot,hb_source[n].recieved_ips[m]);
      if (hb_source[n].recieved_ips[m]!=1)
      {
         if (hb_source[n].status!=2)
         {
           if (is_debug)
            printf("Not recieved from %s of slot #%d on time!\r\nheartbeat_state=%d\r\n",hb_source[n].accept_ips[m],hb_source[n].slot,heartbeat_state);
           sendHeartbeat(hb_source[n].slot,0);
           hb_source[n].status=2;
           if (heartbeat_state!=2)
           {
                if (heartbeat_state==1)
                {
                  printf("heartbeat state is not OK\r\n");
                  //sendAlarm(19,0,getSlotOfNmu(0,2),0,0,0);
                  sendAlarm(19,0,getSlotOfNmu(0,2),0,1,0,0,now_time,NULL);
                  /*if (invalid_message)
                  {
                    sendAlarm(15,0,getSlotOfNmu(0,2),0,0,0);
                    if (is_debug)
                    {
                      if (invalid_message==1)
                          printf("Source IP invalid\n");
                      else if (invalid_message==2)
                          printf("Message invalid\n");
                    }
                    invalid_message=0;
                  }
                  */
                }
              heartbeat_state=2;
           }
         }
         status=0;
         //hb_source[n].first_found_time=0;
         break;
      }
    }
    //printf("Slot=%d Status=%d hb_source[n].status=%d\r\n",hb_source[n].slot,status,hb_source[n].status);
    if (status)
    {
       if (hb_source[n].status!=1)
       {
          if (is_debug)
            printf("HB of slot #%d is OK!\r\nheartbeat_state=%d\r\n",hb_source[n].slot,heartbeat_state);
          sendHeartbeat(hb_source[n].slot,1);
          if (heartbeat_state!=1)
          {
              if (heartbeat_state==2)
                  sendAlarm(20,0,getSlotOfNmu(0,2),0,1,0,0,now_time,NULL);
                  //sendAlarm(20,0,getSlotOfNmu(0,2),0,0,0);
              heartbeat_state=1;
          }
          hb_source[n].status=1;
       }
    }
  }
  for (n=0;n<hb_slot_count;n++)
  {
    for (m=0;m<hb_source[n].ip_count;m++)
    {
       if (hb_source[n].recieved_ips[m])
           hb_source[n].recieved_ips[m]=0;
    }
  }
}


void sendHeartbeat(uchar_t slot,char status)
{
 uchar_t uc=0,ut=0;
 char *values=NULL;
 /*
 if (is_setting)
 {
   printf("is setting...\r\n");
   return;
 }
 else
     is_setting=1;
*/
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
 if (status!=1)
 {
  message[5]=2;
  if (is_debug)
   printf("Heartbeat of OBP in slot #%d timeout!\r\n",slot);
 }
 else
 {
   message[5]=1;
   if (is_debug)
    printf("Heartbeat of OBP in slot #%d is normal!\r\n",slot);
 }
 int n,m;

 for (n=0;n<hb_slot_count;n++)
 {
   if (hb_source[n].slot==slot)
   {
    for (m=0;m<hb_source[n].ip_count;m++)
    {
       hb_source[n].recieved_ips[m]=0;
       //printf("recieved_ips[m]=0\r\n");
    }
    if (status!=1)
       hb_source[n].status=1;
    else
       hb_source[n].status=2;
    //hb_source[n].start_time=0;
    break;
   }
 }
 
 //printf("setting...\r\n");
 //sleep(5);

 sendMessageToUnit(0,slot,message,600,is_debug,is_debug,is_debug);
/*
 for (n=0;n<3;n++)
 {
  if (ret!=EXIT_SUCCESS)
  {
   usleep(100000);
   ret=sendMessageToUnit(0,slot,message,600,is_debug,is_debug,is_debug);
  }
  else
  {
    break;
  }
 }
 */
 /*
 if (status)
 {
  sendAlarm(20,0,getSlotOfNmu(0,2),0,0,0);
  //insertOperateLog(LOG_HEARTBEAT_WD_FAIL,0,0,System,unkown_opt,n,0);
 }
 else
 {
   sendAlarm(19,0,getSlotOfNmu(0,2),0,0,0);
  //insertOperateLog(LOG_HEARTBEAT_WD_OK,0,0,System,unkown_opt,n,0);
 }
*/
 //printf("setting Ok...\r\n");
 //sendMessageToUnit(0,slot,message,600);
 is_setting=3;
}


void onTimeOut()
{
  uchar_t n;
  //printf("onTimeOut\r\n");
  for (n=0;n<hb_slot_count;n++)
  {
    //printf("status[%d]=%d \r\n",n,hb_source[n].status);
    if (hb_source[n].status!=2)
    {
       hb_source[n].status=2;
       sendHeartbeat(n+1,0);
    }
  }
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
void startListening(long second,long msecond,char is_debug)
{
  //#define PORT    2012
  #define BUFFER_SIZE 18
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
  /*
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
  */
  timeout_msec=second*1000+msecond;
  setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,&re_flag,re_len);

  while(1)        
  {
    FD_ZERO(&readfds);
    FD_SET(sockfd,&readfds);
    tv.tv_sec = second;  //设置超时时间
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
             char slots[16],status[4];
             //printf("Received from %s\r\n",fromip);
             if (parsekHeartBeatMessage(buf,slots))
             {
               if (msg_type==1)
               {
               uchar_t n,m;
               //char message[80]={0XFE,0XFE,0X00,0X00,0X16,0X00,0X00,0XFF,0X16,0X00,0X01,0X28};
               char message[80]={0xfe,0Xfe,0X00,0X00,0X16,0X00,0X00,0Xff,0X16,0X00,0X01,0X28,0X02,0X01,0X01,0X01,0X00,0X02,0X01,0X01};
               //printf("SUM=%d\r\n",getCheckSum(message,0,13+slots_count*3));
               check_source(fromip,buf,slots);
               //unit_count;
               message[12]=slots_count;
               message[13]=unit_count;
               for (n=0;n<slots_count;n++)
               {
                  message[14+n*3]=n+1;
                  message[14+n*3+1]=1;
                  message[14+n*3+2]=obp_info[n].working_line;
               }
               message[14+slots_count*3]=getCheckSum(message,0,13+slots_count*3);
               sendto(sockfd,message,15+slots_count*3,0,(struct sockaddr*) &client_addr,len);
              }
              else
              {
                 if (ehco[0]!='\0')
                     sendto(sockfd,ehco,strlen(ehco),0,(struct sockaddr*) &client_addr,len);
              }
              //invalid_message=1;
             }
             else
             {
                 if (is_debug)
                  printf("Received heart beat message is invalid!\r\n");
                 invalid_message=2;
             }
          }
          else
          {
            if (!is_setting)
              onTimeOut();
            else if (is_setting==3)
              is_setting=0;
          }
          //printf("Client %s connect OK\n",fromip);
          
          //printf("received:%0x %0x from %s \n",buf[0],buf[1],fromip);
         // if (ehco[0]!='\0')
         //  sendto(sockfd,ehco,strlen(ehco),0,(struct sockaddr*) &client_addr,len);
      }
    }
      else //3s内没有收到客户端的消息，超时
      {
        //printf("Timeout,left time %d s,%d us\n",(int)tv.tv_sec,(int)tv.tv_usec);
         if (!is_setting)
           onTimeOut();
         else if (is_setting==3)
          is_setting=0;
      }
  }
  /* 结束通信 */
  close( sockfd );
  return;
}

void init_conf()
{
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
    //printf("Slot=%d :%s\r\n",slot,slots[n]);
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
      hb_source[m].status=0;
      m++;
    }
  }
  hb_slot_count=m;
  //printf("hb_slot_count:%d\r\n",hb_slot_count);
  port=getConfIntValue(SYS_CONF_FILE,"port"," ");
  
  if (port<1)
    port=DEFAULT_PORT;

  msg_type=getConfIntValue(SYS_CONF_FILE,"msg_type"," ");
  if (msg_type!=1)
  {
    getConfValue(SYS_CONF_FILE,"ac_msg",value," ");
    ac_msg_len=strtoca(value,";",ac_msg,16);
    if (ac_msg_len>16)
      ac_msg_len=16;
  }
  //printf("%0x %0x\r\n",ac_msg[0],ac_msg[1]);
  getConfValue(SYS_CONF_FILE,"ehco",ehco," ");
  
  getConfValue(SYS_CONF_FILE,"switch_one",value," ");
  if (value[0]=='Y')
    exit_after_switch=1;
}
int count = 0;
void set_timer(long sec,long usec)
{
    struct itimerval itv, oldtv;
    if (sec<0)
        sec=0;
    else if (sec>10)
        sec=10;
    else if (sec==0)
    {
      if (usec<10000)
          usec=10000;
    }
    if (usec>999999)
        usec=999999;
    itv.it_interval.tv_sec = sec;
    itv.it_interval.tv_usec = usec;
    itv.it_value.tv_sec = sec;
    itv.it_value.tv_usec = usec;

    setitimer(ITIMER_REAL, &itv, &oldtv);
}
void check_obp()
{
   uchar_t n,uc=0,ut=0;
   unit_count=0;
   for (n=0;n<slots_count;n++)
      {
          getUnitClassType(0,hb_source[n].slot,&uc,&ut);
          if (!(uc==UC_OLP && ut>50))
          {
            obp_info[n].unit_type=0;
            obp_info[n].working_line=0;
          }
          else
          {
            obpInfo_t *obp=getObpInfo(0,n+1);
            
            if (obp!=NULL)
            {
               if (obp->line!=2)
                 obp_info[n].working_line=0;
               else
                  obp_info[n].working_line=1;
               unit_count++;
            }
            else
            {
               obp_info[n].unit_type=1;
               obp_info[n].working_line=0;
            }
          } 
      }
}
void sigalarm_handler(int sig)
{
    if (!is_setting)
    {
     // if (is_debug)
     //   printf("sigalarm_handler\n");
      check_heart_beat();
    }
    else if (is_setting==3)
          is_setting=0;
    if (count<10)
       count++;
    else
    {
      count=0;
      check_obp();
      /*
      uchar_t n,uc=0,ut=0;
      unit_count=0;
      for (n=0;n<slots_count;n++)
      {
          getUnitClassType(0,hb_source[n].slot,&uc,&ut);
          if (!(uc==UC_OLP && ut>50))
          {
            obp_info[n].unit_type=0;
            obp_info[n].working_line=0;
          }
          else
          {
            obpInfo_t *obp=getObpInfo(0,n+1);
            
            if (obp!=NULL)
            {
               if (obp->line!=2)
                 obp_info[n].working_line=0;
               else
                  obp_info[n].working_line=1;
               unit_count++;
            }
            else
            {
               obp_info[n].unit_type=1;
               obp_info[n].working_line=0;
            }
          } 
      }
       */
    }
    // printf("timer signal.. %d is_setting=%d\n", count,is_setting);
}
void usage()
{
  printf("Description:\n");
  printf("Heart beat.\r\nver 1.0.23\r\nLast modifed:%s\n",LAST_MODIFY_DATE);
  printf("-s for sleep.\n");
  printf("-d for debug.\n");
  printf("-h for help.\n");
}

void main(int argc, char **argv)
{
  int oc;
  unsigned int seconds=-1;
  while((oc = getopt(argc, argv, "dhs:")) != -1)
  {
        switch(oc)
        {
          case 's':
             seconds=atoi(optarg);
             break;
          case 'd':
             //is_debug=atoi(optarg);
             //if (is_debug!=1)
               debug.is_debug_mode=1;
               is_debug=1;
             break;
          case 'h':
             usage();
             return;
        }
  }
    long second=1,msecond=10;
    //printf("Sleep %d sec...\r\n",seconds);
    init_conf();
    if (seconds<0)
    {
       seconds=getConfIntValue(SYS_CONF_FILE,"waitting"," ");
    }
    if (seconds>60)
        seconds=60;
    if (seconds>0);
      sleep(seconds);
    second=getConfIntValue(SYS_CONF_FILE,"sec"," ");
    msecond=getConfIntValue(SYS_CONF_FILE,"msec"," ");
    slots_count=getSlotOfNmu(0,15);
    slots_count--;
    if (slots_count<1)
        slots_count=1;
    if (slots_count>15)
        slots_count=15;
    initAlarmConf();
    check_obp();
    //printf("slots_count=%d\r\n",slots_count);
    signal(SIGALRM, sigalarm_handler);
    set_timer(second,msecond*1000);
    startListening(second,msecond,0);
}


