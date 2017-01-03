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
//arm-linux-gcc -o udps  udps.c -I../include -L=./ -L/home/visint/src/arm/lib -lvispace -lsqlite3 -liconv

#define DEFAULT_PORT 5001 
#define BUFFER_SIZE 60

static char password[16];
/*
认证码	
版本号	
命令标识	
机箱号	D0
槽位号 D1	
信息集ID D2	
信息集索引 D3	
获取信息标识 D4	
信息项开始偏移量 D5	
信息项个数 D6
*/
int getInfoset(char cmd_buf[],char infoset_buf[])
{
  uchar_t begin_offset=0;
  uchar_t infoset_count;
  int msg_len=1;
  char *pinfoset=getInfoSetValues(0,cmd_buf[1],cmd_buf[2],cmd_buf[3]);
  //printf("#%d InfoSetID=%d index=%d cmd_buf[4]=%d cmd_buf[5]=%d\n",cmd_buf[1],cmd_buf[2],cmd_buf[3],cmd_buf[4],cmd_buf[5]);
  if (cmd_buf[4]!=0)
  {
    msg_len=cmd_buf[4]* cmd_buf[6];
    if (msg_len<1)
        msg_len=1;
    else if (msg_len>36)
        msg_len=36;
    if (cmd_buf[5]>-1 && cmd_buf[5]<36)
       begin_offset= cmd_buf[5];
    //printf("msg_len=%d begin_offset=%d\n",msg_len,begin_offset);
    if (msg_len+begin_offset>36)
    {
       infoset_buf[0]=2;
       return msg_len+1;
    }
  }
  else
      msg_len=36;
  if (pinfoset!=NULL)
  {
    infoset_buf[0]=0;
    //printf("#%d begin_offset=%d,msg_len=%d\n",cmd_buf[1],begin_offset,msg_len);
    memcpy(infoset_buf+1,pinfoset+begin_offset,msg_len);
  }
  else
  {
    infoset_buf[0]=1;
    //printf("pinfoset==NULL\n");
  }
  return msg_len+1;
}
void start_udp(int port)
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
  char psw[10];
  bzero(buffer, BUFFER_SIZE);
  int n;
  if (recvfrom(server_socket_fd, buffer, BUFFER_SIZE,0,(struct sockaddr*)&client_addr, &client_addr_length)!=-1)
  {
    bzero(psw, 9);
    for (n=0;n<8;n++)
    {
      psw[n]=buffer[n];
    }
    if (!strcmp(psw,password))
    {
       //char *infoset=NULL;
       int msg_len=1;
       if (buffer[9]>0)
       {
           //printf("buffer[9]=%d buffer[10]=%d buffer[11]=%d\n",buffer[9],buffer[10],buffer[11]);
           if (buffer[9]<60)
           {
             /*
             if (buffer[9]==1)
              infoset=getInfoSetValues(0,buffer[11],buffer[12],buffer[13]);
             if (buffer[14]!=0 && infoset!=NULL)
             {
               infoset+=buffer[14];
               if (buffer[15]==1)
                  msg_len=1;
               else if (buffer[15]==2)
                  msg_len=2;
               else
                  msg_len=36;
            }
            */
            if (buffer[9]==1)
            {
              msg_len=getInfoset(buffer+10,buffer);
            }
            else
                buffer[0]=1;
          }
          else
          {
             buffer[0]=0;
             if (buffer[9]==64)
             {
                if (buffer[10]==1)
                  system("reboot");
             }
             else if (buffer[9]==65)
             {
                if (buffer[10]==100 && buffer[11]>0 && buffer[11]<5)
                {
                   //uchar_t= getSlotOfNmu(0);
                   //printf("set OBP to main line\n");
                   uchar_t *us=getUnitStatusMap(0);
                   if (us!=NULL)
                   {
                      char hb_slots[8]={0};
                       //printf("us!=NULL\n");
                       uchar_t n,count=0;
                       uchar_t slots[16]={0};
                       bytetobits(buffer[12],hb_slots);
                       for (n=0;n<8;n++)
                       {
                         
                         if (hb_slots[n]==1 && *(us+n+1)==UC_OLP)
                         {
                             slots[count++]=n+1;
                         }
                         //printf("[%d]:#%d ",n,slots[n]);
                       }
                       //printf("\n");                
                       bytetobits(buffer[13],hb_slots);
                       for (n=0;n<8;n++)
                       {
                         if (hb_slots[n]==1 && *(us+n+9)==UC_OLP)
                         {
                             slots[count++]=n+9;
                         }
                       }
                       /*
                       for (n=0;n<count;n++)
                          printf("[%d]:#%d ",n,slots[n]);
                       printf("\n");
                       
                       for (n=1;n<16;n++)
                       {
                         //printf("#%d [%d] #%d",n,*(us+n),slots[n]);
                         if (*(us+n)==UC_OLP)
                         {
                             slots[count++]=n;
                         }
                       }
                       */
                       if (count>0)
                        {
                         char message[512];
                         int ret;
                         memset(message,0,512*sizeof(char));
                         message[0]='S';
                         message[1]=OLP_STATUS_INFOSET_ID;
                         message[2]=1;
                         message[3]=10;
                         message[4]=0;
                         message[5]=buffer[11];
                         for (n=0;n<count;n++)
                             printf("set #%d to main line",slots[n]);
                       // printf("set hb_group #%d to main line");
                        ret=ioBroadcast(0,count,slots,message,3);
                        if (EXIT_SUCCESS!=ret)
                        {
                           buffer[0]=2;
                        }
                       }
                  }
                }           
                else if (buffer[10]<16)
                   setHeartBeatItem(buffer+10);
                else if (buffer[10]==16)
                   setHeartBeatSlots(buffer+11);
               
             }
          }
       }
       else
       {
         //setHeartBeatSlots(char group_count,char conf[]);
       }
       sendto(server_socket_fd,buffer,msg_len,0,(struct sockaddr*) &client_addr,client_addr_length);
     
       /*
       if (infoset!=NULL)
           sendto(server_socket_fd,buffer,msg_len,0,(struct sockaddr*) &client_addr,client_addr_length);
       else
       {
         buffer[0]=1;
         sendto(server_socket_fd,buffer,1,0,(struct sockaddr*) &client_addr,client_addr_length);
       }
       */
       //sendto(server_socket_fd,psw,strlen(psw),0,(struct sockaddr*) &client_addr,client_addr_length);
    }
  }
 }
}
void usage()
{
  printf("Description:\n");
  printf("-d for debug.\n");
  printf("-h for help.\n");
}
int main(int argc, char **argv)
{
  int oc;
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
  int port=getConfIntValue(SYS_CONF_FILE,"udpport"," ");
  if (port<1)
      port=DEFAULT_PORT;
  getConfValue(SYS_CONF_FILE,"udp_ac_msg",password," ");
  //printf("udpport:%d\n",port);
  start_udp(port);
}
