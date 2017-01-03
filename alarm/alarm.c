#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>
#include <alarm.h>
void sendAlarm(int alarm_code,char chassis,char slot,char entity,char is_with_value,short alarm_value,unsigned short alarm_reason,time_t alarm_time,void(*value2message)(short,char *))
{
  if (almmsgid!=-1)
  {
    alarm_msg_t alarm_msg;
    alarm_info_t alarm;
    alarm.alarm_code=(short)alarm_code;
    alarm.chassis=chassis;
    alarm.slot=slot;
    alarm.entity=entity;
    alarm.is_with_value=is_with_value;
    alarm.alarm_value=alarm_value;
    alarm.alarm_reason=alarm_reason;
    alarm.alarm_time=alarm_time;
    if (alarm.alarm_time==0)
    {
       time_t timer;
       timer= time(NULL);
       //printf("Alarm_time=%d\n",alarm_time);
       //time(&alarm_time);
       alarm.alarm_time=timer;
    }
    
    memcpy(alarm_msg.alarm_info,&alarm,sizeof(alarm_info_t));
    alarm_msg.msg_type=1;
    //printf("Alarm_time=%d\n",alarm_time);
    /*
    alarm_msg.alarm_info.alarm_code=alarm_code;
    alarm_msg.alarm_info.chassis=chassis;
    alarm_msg.alarm_info.slot=slot;
    alarm_msg.alarm_info.entity=entity;
    alarm_msg.alarm_info.alarm_value=alarm_value;
    alarm_msg.alarm_info.alarm_reason=alarm_reason;
    alarm_msg.alarm_info.alarm_time=alarm_time;
    */
    //alarm.value2message=value2message;
    //向队列发送数据  
    if(msgsnd(almmsgid, &alarm_msg, sizeof(alarm_info_t), 0) == -1)  
    {  
        printf("msgsnd failed\n");   
    }
   // else
    //    printf("msgsnd OK #%d %d is_with_value=%d\n",slot,alarm_code,is_with_value);
  }
}
