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

#include <time.h> 

struct itimerval tick;
char is_first_start_timer;

void start()
{
  struct itimerval tick;
  is_first_start_timer=1;
  time_t now_time=time(NULL);
  printf("Start timer:%s\n",ctime(&now_time));
  memset(&tick, 0, sizeof(tick));

    //Timeout to run first time
    tick.it_value.tv_sec = 3;
    tick.it_value.tv_usec = 000000;

    //After first, the Interval time for clock
    tick.it_interval.tv_sec = 0;
    tick.it_interval.tv_usec = 500000;

    if(setitimer(ITIMER_REAL, &tick, NULL) < 0)
         printf("Set timer failed!\n");
}

void stop()
{
 struct itimerval tick;
 memset(&tick, 0, sizeof(tick));
 tick.it_value.tv_sec = 0;
 tick.it_value.tv_usec = 0;
 tick.it_interval = tick.it_value;
 //tick.it_interval.tv_sec=0;
 //tick.it_interval.tv_usec=0;
 setitimer(ITIMER_REAL, &tick, NULL);
}
int count;


void check_heart_beat(int signo)
{
  if (is_first_start_timer)
  {
    is_first_start_timer=0;
    return;
  }
  time_t now;
  if (count<5)
   {
     count++;
     time_t now=time(NULL);
     printf("HB start %s\n",ctime(&now));
   }
  else
  {
    count=0;
    sleep(5);
    stop();
    start();
    time_t now=time(NULL);
    printf("HB stop %s\n",ctime(&now));
  }
  //time_t now=time(NULL);
  //printf("HB %s\n",ctime(&now));
}
int count;
#define BUFFER_SIZE 17
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
 }
}
int main()
{
 time_t now_time;
 signal(SIGALRM, check_heart_beat);
 //if (count%2)
 now_time=time(NULL);
 printf("Start timer:%s\n",ctime(&now_time));
 start();
 start_heart_beat(9001);
 /*
 while(1)
 {
   pause();
   now_time=time(NULL);
   printf("At main:%s\n",ctime(&now_time));
   sleep(3);
 }
*/
}
