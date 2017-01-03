/*
 *基于udp通信 client端
 *
 * */
//arm-linux-gcc -o udpclient udp_client.c -I../include -L=./ -L/home/visint/src/arm/lib -lvispace -lsqlite3 -liconv
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <public.h>

#define  DEFAULT_PORT  9001
static void usage()
{
  printf("Description:\n");
  printf("UDP heartbeat client.\nver 1.0.0\nLast modified:2014-04-22\n\n");
  printf("-s server-ip\tserver ip(default ip is 127.0.0.1).\n");
  printf("-p udp-port\tserver udp port(default port is 2012).\n");
  printf("-n number \tnumberof echo requests to send(default number is 3).\n");
  printf("-m message \tmessage to send(default:0x715a).\n");
  printf("-t \t\tsend to the specified host until stopped.\n");
  printf("-i interval\tinterval for each request(default interval is 10MS).\n");
  printf("-h \t\tfor help.\n");
}
void main(int argc, char **argv)
{
  int oc,i,count=3,port=DEFAULT_PORT,interval=10;
  char svr_ip[20]="192.168.2.222";
  //char snd_msg[10]={0x71,0x5a,1};
  char snd_msg[17]={0xef,0xef,0x00,0x00,0x0c,0x00,0x00,0xFF,0x16,0x00,0x01,0x01,0xff,0xff,0x00,0x00,0x0f};
  int  msg_len=0;
  while((oc = getopt(argc, argv, "n:dhm:p:s:ti:")) != -1)
  {
        switch(oc)
        {
          case 'n':
             count=atoi(optarg);
             break;
          case 'm':
             msg_len=strtoca(optarg,";",snd_msg);
          case 'p':
             port=atoi(optarg);
             break;
          case 'i':
             interval=atoi(optarg);
             break;
          case 's':
             strcpy(svr_ip,optarg);
             break;
          case 't':
             count=0;
             break;
          case 'h':
             usage();
             return;
        }
 }
  if (msg_len<1)
  {
    char value[60]="";
    msg_len=getConfValue(SYS_CONF_FILE,"hb_msg",value," ");
    printf("msg_len:%d\n",msg_len);
    //if (msg_len>0)
    msg_len=strtoca(value,";",snd_msg);
    printf("msg_len:%d\n",msg_len);
  }
  if (msg_len<1)
     msg_len=1;
  printf("message to send:\n");
  for(i=0;i<msg_len;i++)
  {
    printf("%0x ",snd_msg[i]);
  }
  printf("\n");
  int sockfd;
  int res;
  int sec=3;
  int msec=0;
  char buf[100] = {};
  /* 准备通信地址 */
  struct sockaddr_in server_addr;
  struct sockaddr_in client_addr;

  server_addr.sin_family = PF_INET;
  server_addr.sin_port = htons(port);
  server_addr.sin_addr.s_addr = inet_addr(svr_ip);
  
  socklen_t len = sizeof(struct sockaddr);
  /* 创建socket */
  i=-1;
   sockfd = socket( PF_INET,SOCK_DGRAM,0 );
  if( sockfd == -1 )    //创建失败
  {
    perror("socket");
    exit(-1);
  }
   struct timeval tv;

tv.tv_sec = sec;  /* 30 Secs Timeout */
tv.tv_usec = msec*1000;  // Not init'ing this can cause strange errors

setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (char *)&tv,sizeof(struct timeval));

  /* 连接服务端 */
  res = connect(sockfd,(struct sockaddr*) &server_addr,sizeof(server_addr));
  if( res == -1 )    //连接失败
  {
    printf("connect to %s fail!\n",svr_ip);
    exit(-1);
  }
  printf("connect to %s ok count=%d!\n",svr_ip,count);
  
  while(i<count)
  {
 

  /* 开始通信 */
  
  //socklen_t len = sizeof(struct sockaddr);
  
   //sockfd = socket( PF_INET,SOCK_DGRAM,0 );
   sendto(sockfd,snd_msg,msg_len,0,(struct sockaddr*) &server_addr,len);
   int recvd;
   recvd=recvfrom(sockfd,buf,100,0,(struct sockaddr*) &server_addr,&len);
   if(recvd==-1)
   {
      printf("timeout\n");
   }
   
   printf("Infomatin from server:%s\n",buf);
   
   if (interval>0)
     usleep(interval*1000);
   if (i<count && count >0)
      i++;
  }
  
  /* 通信结束 */
  close(sockfd);
}

