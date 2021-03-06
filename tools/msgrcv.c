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
//#include "../include/alarm.h"
// arm-linux-gcc -o msgrcv msgrcv.c

typedef struct _alarm_info_t
{
  short alarm_code;
  char chassis;
  char slot;
  char entity;
  char is_with_value;
  short alarm_value;
  unsigned short alarm_reason;
  time_t alarm_time;
  //void(*value2message)(short,char *);
} alarm_info_t;

typedef struct _alarm_msg_t
{
  long  msg_type;
  //alarm_info_t alarm_info;
  char alarm_info[sizeof(alarm_info_t)];
  //void(*value2message)(short,char *);
} alarm_msg_t;

void main()
{
    long int msgtype = 1; //注意1  
    int buf_size=sizeof(alarm_info_t);
    alarm_msg_t data;
    //建立消息队列  
    int almmsgid = msgget((key_t)1234, 0666 | IPC_CREAT);  
    
     if(almmsgid == -1)  
    {  
        fprintf(stderr, "msgget failed with error: %d\n", errno);  
        exit(EXIT_FAILURE);  
    }
    
    //从队列中获取消息，直到遇到end消息为止  
    int count=0;
    while(1)  
    {  
        if(msgrcv(almmsgid, (void*)&data, buf_size, msgtype,0) != -1)  
        {  
          printf("msgrcv count=%d\n",++count);
        }
    }
}
