#ifndef __ALARM_H
#define __ALARM_H
/*
#include <sqlite3.h>
int setAlarmStatus(sqlite3 *db,int alarm_code,char chassis,char slot,char entity,char status);
int setAlarmStatusById(sqlite3 *db,int alarm_id,char status);
int deleteAlarmLogById(sqlite3 *db,int alarm_id);
int cleanAlarmLog(sqlite3 *db,int alarm_code,char chassis,char slot,char entity);
int ackAlarmLog(sqlite3 *db,int alarm_code,char chassis,char slot,char entity);
int insertAlarmLog(sqlite3 *db,int alarm_code,char chassis,char slot,char entity,short alarm_value);
void sendSnmpTrap(int alarm_code,char chassis,char slot,char entity);
*/
#define ALARM_TYPE_COUNT 110
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
typedef struct _alarm_dest_host_t
{
 char dest_ip[16];
 unsigned int port;
}alarm_dest_host_t;

typedef struct _alarm_type_t
{
  unsigned short alarm_code;
  char is_send;
  char is_save;
  char mask_slot[16];
  char alarm_name[40];
  char entity_name[12];
}alarm_type_t;
//extern alarm_dest_host_t dest_hosts;
extern int almmsgid;
extern void sendAlarm(int alarm_code,char chassis,char slot,char entity,char is_with_value,short alarm_value,unsigned short alarm_reason,time_t alarm_time,void(*value2message)(short,char *));
extern void handleAlarm(sqlite3 *db,int alarm_code,char chassis,char slot,char entity,char is_with_value,short alarm_value,unsigned short alarm_reason,time_t alarm_time,void(*value2message)(short,char *));
#endif /* __ALARM_H */
