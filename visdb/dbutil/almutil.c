#include <stdio.h>
#include <string.h>
#include <sys/msg.h>
#include <unistd.h>
#include <unit.h>
#include <sqlite3.h>
#include <public.h>
#include <dbutil.h>
#include <vispaceio.h>
#include <visipc.h>
#include <alarm.h>
#define MAX_ALARM_COUNT 300
#define MAX_ALARM_COUNT2 1000

#ifdef BIWEI_WUXI_VERSION
extern char olp_osw[];
#endif
//gcc -o /usr/local/bin/dbutil.o -lvisipc -lvisdb ../vislib/unit.o ../vislib/visstr.o dbutil.c  -I../include
//gcc -c almutil.c  -I../include & ar -r libdbutil.a almutil.o
#define MAX_ALARM_INFO_COUNT 50
//alarm_info_t alarms[MAX_ALARM_COUNT];
//int alarm_info_count=0;
//char has_new_alarm=0;
int setAlarmStatus(sqlite3 *db,int alarm_code,char chassis,char slot,char entity,char status)
{
  char sql[250],time[60]="";
  /*boolean_t auto_open=TRUE;
 
  if (db!=NULL)
  {
    auto_open=FALSE;
  }
  else
      db=openDB(DB_FILE_NAME);
  if (db==NULL)
    return -1;
  */
  if (slot<0 ||slot>MAX_UNIT_COUNT || entity<0 ||entity>8 || status<1 ||status>2)
    return -1;
  if (status==1)
   strcat(time,",clean_time=datetime('now','localtime')");
  else
   strcat(time,",ack_time=datetime('now','localtime')");
  sprintf(sql,"UPDATE AlarmLog SET alarm_status=alarm_status|%d %s WHERE alarm_code=%d AND slot=%d AND entity=%d;",status,time,alarm_code,slot,entity);
 
 //printf("%s\n",sql);

 if (SQLITE_OK!=execSql(db, sql,NULL,NULL))
     return -1;
  else
     return 0;
}
int setAlarmStatusById(sqlite3 *db,int alarm_id,char status)
{
  char sql[80];
  sprintf(sql,"UPDATE AlarmLog SET alarm_status=alarm_status|%d WHERE alarm_id=%d;",status,alarm_id);
  if (SQLITE_OK!=execSql(db, sql,NULL,NULL))
       return -1;
  return 0;
}

int deleteAlarmLogById(sqlite3 *db,int alarm_id)
{
  char sql[80];
  sprintf(sql,"DELETE FROM AlarmLog  WHERE alarm_id=%d;",alarm_id);
  if (SQLITE_OK!=execSql(db, sql,NULL,NULL))
       return -1;
  return 0;
}

int cleanAlarmLog(sqlite3 *db,int alarm_code,char chassis,char slot,char entity)
{
  return setAlarmStatus(db,alarm_code,chassis,slot,entity,1);
}

int ackAlarmLog(sqlite3 *db,int alarm_code,char chassis,char slot,char entity)
{
  return setAlarmStatus(db,alarm_code,chassis,slot,entity,2);
}
//char save_alarm_flag=0;
//int  unsigned max_alarm_count=300;
/*
int  unsigned alarm_count=0;
static void init_conf()
{
       char value[16]="";
         getConfValue(SYS_CONF_FILE,"save_alarms",value," ");
         if (value[0]=='2')
            save_alarm_flag=2;
         else if (value[0]=='0')
            save_alarm_flag=0;
         else
             save_alarm_flag=1;
         //printf("save_alarms=%s save_alarm_flag=%d\r\n",value,save_alarm_flag);
         getConfValue(SYS_CONF_FILE,"max_acount",value," ");
         max_alarm_count=atoi(value);
         if (max_alarm_count<1)
             max_alarm_count=300;
         if (-1==getColIntValue(NULL,"SELECT COUNT(*) FROM AlarmLog",&alarm_count))
           alarm_count=1;
        // printf("max_acount=%s max_alarm_count=%d\r\n",value,save_alarm_flag);
}
*/
static short alarm_count=0,acount=0;
int addAlarmLog(sqlite3 *db,int alarm_code,char chassis,char slot,char entity,char is_with_value,short alarm_value,unsigned short alarm_reason,time_t alarm_time,void(*value2message)(short,char *))
{
  char sql[320];
  char value[36]="";
  //acount++;
 // printf("AlarmConf.alarm_count=%d AlarmConf.max_alarm_count=%d alarm_value=%d acount=%d\n",AlarmConf.alarm_count,AlarmConf.max_alarm_count,alarm_value,acount);

  //printf("addAlarmLog:#%d alarm_code=%d acount=%d %s alarm_time=%d\n",slot,alarm_code,acount,ctime(alarm_time),alarm_time);
  if (alarm_count<20)
     alarm_count++;
  else
  {
    alarm_count=0;
    if (1!=getColIntValue(NULL,"SELECT COUNT(*) FROM AlarmLog",&AlarmConf.alarm_count))
           AlarmConf.alarm_count=1;
    if (AlarmConf.alarm_count<1)
       AlarmConf.alarm_count=1;
  }
  if (AlarmConf.alarm_count>=AlarmConf.max_alarm_count)
  {
        sprintf(sql,"DELETE FROM AlarmLog WHERE ID NOT IN (SELECT ID FROM AlarmLog order by alarm_time desc LIMIT %d)",AlarmConf.max_alarm_count-20);
        //printf("sql:%s\r\n",sql);
        if (SQLITE_OK!=execSql(db, sql,NULL,NULL))
        {
          printf("execSql fail:%s\n",sql);
          //return -1;
        }
        AlarmConf.alarm_count=AlarmConf.max_alarm_count-20;
  }
  //if (alarm_code==409 || alarm_code==411 ||alarm_code==413)
  // if (alarm_value!=0xffff)
  if (is_with_value)
  {
      if (value2message==NULL)
         sprintf(value,"%.1f",0.1*alarm_value);
      else
      {
        value2message(alarm_value,value);
        //printf("Alarm_value=%s\n",value);
      }
  }
  //printf("Alarm_value=%s\n",value);
  //if (alarm_code==19 || alarm_code==20)
   // printf("slot=%d alarm_code=%d alarm_value=%d is_with_value=%d\n",slot,alarm_code,alarm_value,is_with_value);
  
  //if (abs(timer-alarm_time)>20)
  if (alarm_time<180)
  {
    time_t now_time;
    time(&now_time);
    //printf("alarm_time=%d\n",alarm_time);
    alarm_time=now_time;
    //time(&alarm_time);
  }
  struct tm now_time = *localtime(&alarm_time);
  char datetime[30]="";
  sprintf(datetime,"%d-%02d-%02d %02d:%02d:%02d", now_time.tm_year + 1900, now_time.tm_mon + 1, now_time.tm_mday, now_time.tm_hour, now_time.tm_min, now_time.tm_sec);
  //printf("alarm_time=%d Alarm time:%s\n",alarm_time,datetime);
  sprintf(sql,"INSERT INTO \"AlarmLog\" VALUES(NULL,%d,%d,%d,%d,0,'%s','%s',NULL,NULL,NULL,1,'',%d);",alarm_code,chassis,slot,entity,datetime,value,alarm_reason);

 //sprintf(sql,"INSERT INTO \"AlarmLog\" VALUES(NULL,%d,%d,%d,%d,0,datetime('now','localtime'),'%s',NULL,NULL,NULL,1,'');",alarm_code,chassis,slot,entity,value);

  //if (alarm_code==19) 
  //printf("sql:%s\r\n",sql);
  if (SQLITE_OK!=execSql(db, sql,NULL,NULL))
  {
        printf("execSql :%s fail!\n",sql);
        doSystem("logger -p 1 -t vierr \"Add alarm #%d ac=%d\"",slot,alarm_code);
        return -1;
  }
  else
  {
        if (AlarmConf.alarm_count<1000)
            AlarmConf.alarm_count++;
        return 0;
  }
  /*
  if (AlarmConf.alarm_count>=AlarmConf.max_alarm_count)
  {
    sprintf(sql,"DELETE FROM AlarmLog WHERE ID NOT IN (SELECT ID FROM AlarmLog order by alarm_time desc LIMIT %d)",AlarmConf.max_alarm_count-1);
    if (SQLITE_OK!=execSql(db, sql,NULL,NULL))
        return -1;
  }
  */
}
int insertAlarmLog(sqlite3 *db,int alarm_code,char chassis,char slot,char entity,char is_with_value,short alarm_value,unsigned short alarm_reason,time_t alarm_time,void(*value2message)(short,char *))
{
  if (alarm_count<20)
    alarm_count++;
  else
  {
    alarm_count=0;
    if (1!=getColIntValue(NULL,"SELECT COUNT(*) FROM AlarmLog",&AlarmConf.alarm_count))
           AlarmConf.alarm_count=1;
    if (AlarmConf.alarm_count<1)
       AlarmConf.alarm_count=1;
  }
  
  //if (abs(timer-alarm_time)>20)
   /*
   if (alarm_time<100)
   {
       time_t timer;
       timer= time(NULL);
       //printf("Alarm_time=0\n");
       //time(&alarm_time);
       alarm_time=timer;
   }
   */
   //  printf("alarm_reason=%d Alarm_time=%s\n",alarm_reason,ctime(&alarm_time));
  //printf("alarm_count=%d\r\n",alarm_count);
  //printf("insertAlarmLog:Alarm_code=%d,slot=%d entity=%d is_add_once=%d alarm_count=%d is_with_value=%d\n",alarm_code,slot,entity,AlarmConf.is_add_once,AlarmConf.alarm_count,is_with_value);
  //if (AlarmConf.is_add_once==0)
  //    return 0;
  if (alarm_time<180)
  {
    time_t now_time;
    time(&now_time);
    //printf("alarm_time=%d\n",alarm_time);
    alarm_time=now_time;
    //time(&alarm_time);
  }
  if (!AlarmConf.is_add_once)
     return addAlarmLog(db,alarm_code, chassis,slot,entity,is_with_value,alarm_value,alarm_reason,alarm_time,value2message);
  //if (alarm_code==19)
  //     printf("alarm_code==19--2\r\n");
  if (alarm_code %2)
  {
    char sql[300];
    char value[36]="";
    int count;
    //sqlite3 *conn;
    //sqlite3_stmt* stmt=NULL;
    int ret;

    if (slot<0 ||slot>16 || entity<0 ||entity>8)
     return -1;
     
     //printf("Here!save_alarm_flag=%d max_alarm_count=%d\r\n",save_alarm_flag,max_alarm_count);
     /*
     if (save_alarm_flag==2)
     {
        if (alarm_count>=max_alarm_count)
         {
        sprintf(sql,"DELETE FROM AlarmLog WHERE ID NOT IN (SELECT ID FROM AlarmLog order by alarm_time desc LIMIT %d)",max_alarm_count-1);
        //printf("sql:%s\r\n",sql);
        if (SQLITE_OK!=execSql(db, sql,NULL,NULL))
         return -1;
        }
        if (alarm_code==409 || alarm_code==411 ||alarm_code==413)
          sprintf(value,"%.1f",0.1*alarm_value);
        sprintf(sql,"INSERT INTO \"AlarmLog\" VALUES(NULL,%d,%d,%d,%d,0,datetime('now','localtime'),'%s',NULL,NULL,NULL,1);",alarm_code,chassis,slot,entity,value);
      // printf("sql:%s\r\n",sql);
      if (SQLITE_OK!=execSql(db, sql,NULL,NULL))
      {
        //printf("execSql :%s fail!\r\n",sql);
        return -1;
      }
      else
      {
        if (alarm_count<max_alarm_count)
            alarm_count++;
        return 0;
      }
    }
    */
    //if (alarm_code==19)
    //   printf("alarm_code==19--3\r\n");
    if (AlarmConf.alarm_count>=AlarmConf.max_alarm_count)
    {
    sprintf(sql,"DELETE FROM AlarmLog WHERE ID NOT IN (SELECT ID FROM AlarmLog order by alarm_time desc LIMIT %d)",AlarmConf.max_alarm_count-21);
    //SQLITE_OK!=execSql(db, sql,NULL,NULL);
    if (SQLITE_OK!=execSql(db, sql,NULL,NULL))
    {
      if (debug.is_debug_mode)
        printf("execSql fail:%s\n",sql);
      //return -1;
    }
   }


     int status=-1;
     sprintf(sql,"SELECT alarm_status FROM AlarmLog WHERE alarm_code=%d AND alarm_status<>3 AND slot=%d AND entity=%d ORDER BY alarm_status LIMIT 1;",alarm_code,slot,entity);
     ret=getColIntValue(db,sql,&status);
     
     //printf("Alarm_code=%d,slot=%d status=%d ret=%d\n",alarm_code,slot,status,ret);
     if (ret==-1)
     {
       //sqlite3_close(conn);
       if (debug.is_debug_mode)
        printf("execSql fail:%s\n",sql);
       //return -1;
     }
     
     //if (alarm_code==409 || alarm_code==411 ||alarm_code==413)
     //if (alarm_value!=0x7fff)
      if (is_with_value)
      {
          if (value2message==NULL)
            sprintf(value,"%.1f",0.1*alarm_value);
          else
            value2message(alarm_value,value);
      }
      // printf("Alarm_value=%s\n",value);
       //time_t t = time(NULL);
       struct tm now_time = *localtime(&alarm_time);
       
       //struct tm tm alarm_time
       char datetime[30]="";
       sprintf(datetime,"%d-%02d-%02d %02d:%02d:%02d", now_time.tm_year + 1900, now_time.tm_mon + 1, now_time.tm_mday, now_time.tm_hour, now_time.tm_min, now_time.tm_sec);
       //printf("alarm_time=%d alarm time:%s\n",alarm_time,datetime);
       //sprintf(sql,"INSERT INTO \"AlarmLog\" VALUES(NULL,%d,%d,%d,%d,0,datetime('now','localtime'),'%s',NULL,NULL,NULL,1,'');",alarm_code,chassis,slot,entity,value);
       //sprintf(sql,"INSERT INTO \"AlarmLog\" VALUES(NULL,%d,%d,%d,%d,0,'%s','%s',NULL,NULL,NULL,1,'');",alarm_code,chassis,slot,entity,datetime,value);
       if (AlarmConf.alarm_count<AlarmConf.max_alarm_count)
            AlarmConf.alarm_count++;
      if (status==-1 || status==3) //记录不存在或为已清除已确认状态
      {
         sprintf(sql,"INSERT INTO \"AlarmLog\" VALUES(NULL,%d,%d,%d,%d,0,'%s','%s',NULL,NULL,NULL,1,'',%d);",alarm_code,chassis,slot,entity,datetime,value,alarm_reason);
     }
     else
     {
//sprintf(sql,"UPDATE \"AlarmLog\"  SET alarm_status=0,last_alarm_time=datetime('now','localtime'),alarm_count=(alarm_count+1),alarm_value=\"%s\" WHERE alarm_status!=3 AND alarm_code=%d AND slot=%d AND entity=%d;",value,alarm_code,slot,entity);
      sprintf(sql,"UPDATE \"AlarmLog\"  SET alarm_status=0,last_alarm_time='%s',alarm_count=(alarm_count+1),alarm_value=\"%s\",reason_code=%d WHERE alarm_status!=3 AND alarm_code=%d AND slot=%d AND entity=%d;",datetime,value,alarm_reason,alarm_code,slot,entity);
     }
     /*
     if (status==-1 || status==3) //记录不存在或为已清除已确认状态
     {
       if (alarm_code==409 || alarm_code==411 ||alarm_code==413)
        sprintf(value,"%.1f",0.1*alarm_value);
       //printf("Alarm_value=%s\n",value);
       sprintf(sql,"INSERT INTO \"AlarmLog\" VALUES(NULL,%d,%d,%d,%d,0,datetime('now','localtime'),'%s',NULL,NULL,NULL,1,'');",alarm_code,chassis,slot,entity,value);
       if (AlarmConf.alarm_count<AlarmConf.max_alarm_count)
            AlarmConf.alarm_count++;
     }
     else
     {
       
       if (!((alarm_code==409 || alarm_code==411 || alarm_code==413)))//带值告警
       {
          strcpy(value,"");
       }
       else
          sprintf(value,"%.1f",0.1*alarm_value);
         
    sprintf(sql,"UPDATE \"AlarmLog\"  SET alarm_status=0,last_alarm_time=datetime('now','localtime'),alarm_count=(alarm_count+1),alarm_value=\"%s\" WHERE alarm_status!=3 AND alarm_code=%d AND slot=%d AND entity=%d;",value,alarm_code,slot,entity);
      // sprintf(sql,"UPDATE \"AlarmLog\"  SET alarm_status=0,last_alarm_time=datetime('now','localtime'),alarm_count=(alarm_count+1), WHERE alarm_status!=3 AND alarm_code=%d AND slot=%d AND entity=%d;",alarm_code,slot,entity);
     }
     */
    //printf("insert alarm:%s\n",sql);
    if (SQLITE_OK!=execSql(db, sql,NULL,NULL))
     {
       printf("execSql fail\n");
       return -1;
     }
  }
  else if (AlarmConf.is_add_once)
  {
     //printf("setAlarmStatus alarm_code=%d,slot=%d entity=%d\n",alarm_code,slot,entity);
     //if (enable==1)
     //sqlite3_close(conn);
     return setAlarmStatus(db,alarm_code-1,chassis,slot,entity,1);
  }
  //sqlite3_close(conn);
  return 0;
}

void deleteAlarmLog(char *where)
{
  
}
void sendNotification(const char *fmt, ...) 
{
  time_t timep;
  char strtime[20] = {0};
  struct tm *p_tm;
  char cmd_buf[512];
  int ret;
  char sql[]="SELECT dest_ipadd,udp_port FROM KeepAlive WHERE enable=1";
  sqlite3_stmt* stmt=NULL;
  sqlite3 *db;
  db=openDB(DB_FILE_NAME);
  if (db==NULL)
     return;

  va_list pargv;
  va_start(pargv, fmt);
  vsnprintf(cmd_buf, sizeof(cmd_buf), fmt, pargv);
  va_end(pargv);
  
  timep = time(NULL);  
  p_tm = localtime(&timep);  
  strftime(strtime, sizeof(strtime), "%m-%d %H:%M:%S", p_tm);  
  //doSystem("sed -i '$1 %s:%s' vispace/err.log",cmd_buf,strtime);
  //sprintf(sql,"SELECT dest_ipadd,udp_port FROM KeepAlive WHERE enable=1");
  ret=sqlite3_prepare_v2(db,sql,strlen(sql),&stmt,NULL);
  if(ret!=SQLITE_OK)
  { 
     if (stmt)
         sqlite3_finalize(stmt);
     sqlite3_close(db);
     return;
  }
  /*
  if (sqlite3_step(stmt) != SQLITE_DONE) {
    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return;
  }
  */
  while (ret==SQLITE_ROW)
  {
     doSystem("snmptrap -m \"\" -v 2c -c public %s:%d ' ' 1.3.6.1.2.1.1.6.0 s '%s'",sqlite3_column_text(stmt,0),sqlite3_column_int(stmt,1),cmd_buf,strtime);
    ret=sqlite3_step(stmt);
  }

  sqlite3_finalize(stmt);
  sqlite3_close(db);
}
//int shmid_set=-1;
struct solt_set *solt_set_shm_addr=NULL;
alarm_dest_host_t dest_hosts[10];
alarm_type_t alarm_types[ALARM_TYPE_COUNT];
void init_alarm_type()
{
  int ret,n=0;
  sqlite3 *db;
  sqlite3_stmt* stmt=NULL;
  char index=0;
  db=openDB(NULL);
  if (db==NULL)
    return;
  #ifdef DEFAULT_LANG_CN
  char sql[]="SELECT alarm_code,is_send,is_save,mask_slot,alarm_name,entity_name FROM AlarmType WHERE is_send=1 OR is_save=1";
  #else
  char sql[]="SELECT alarm_code,is_send,is_save,mask_slot,alarm_name_en,entity_name_en FROM AlarmType WHERE is_send=1 OR is_save=1";
  #endif
  ret=sqlite3_prepare_v2(db,sql,strlen(sql),&stmt,NULL);
  if(ret!=SQLITE_OK)
  {
     //sqlite3_finalize(stmt);
     if (stmt)
      sqlite3_finalize(stmt);
     printf("init_alarm_type Error!\n");
     //printf("sendAlarm Error:sql:%s\n",sqlite3_errmsg(db),sql);
     sqlite3_close(db);
     return;
  }
  else 
  {
    while(sqlite3_step(stmt) ==SQLITE_ROW && n<ALARM_TYPE_COUNT) {
      alarm_types[n].alarm_code=sqlite3_column_int(stmt,0);
      alarm_types[n].is_send=0xff & sqlite3_column_int(stmt,1);
      alarm_types[n].is_save=0xff &sqlite3_column_int(stmt,2);
      short mask_slot,alarm_code;
      mask_slot=sqlite3_column_int(stmt,3);
      index=0;
      if (mask_slot!=0)
      {
         int i;
         //char slot;
         alarm_code=alarm_types[n].alarm_code;
         //printf("\nMask slot=%d alarm code=[%d]:%d\n",mask_slot,alarm_code,alarm_types[n].alarm_code);
         for (i=1;i<16;i++)
         {
           //slot=0x1 & (mask_slot>>(15-i));
           //printf("\nMask slot=%d ",slot);
           if (0x1 & (mask_slot>>(15-i)))
           {  
             alarm_types[n].mask_slot[index++]=i;
           //alarm_types[n].mask_slot[index++]=0x1 & (mask_slot>>(15-m));
           //printf("Here %d\n",alarm_types[n].mask_slot[index-1]);
           }
         }
      }
      //else
      //    alarm_types[n].mask_slot[index++]=0;
      strncpy(alarm_types[n].alarm_name,sqlite3_column_text(stmt,4),40);
      strncpy(alarm_types[n].entity_name,sqlite3_column_text(stmt,5),12);
      //printf("ip=%s port=%d\n",dest_hosts[n].dest_ip,dest_hosts[n].port);
      //ret=sqlite3_step(stmt);
      n++;
   }
  }
  if (n<ALARM_TYPE_COUNT)
    alarm_types[n].alarm_code=0;

  if (stmt)
      sqlite3_finalize(stmt);
  sqlite3_close(db);
  //printf("alarm_type count=%d\n",n);
}
void init_dest_host()
{
  int ret,n;
  char sql[]="SELECT dest_ipadd,udp_port FROM DestHost";
  sqlite3 *db;
  sqlite3_stmt* stmt=NULL;
  db=openDB(NULL);
  if (db==NULL)
    return;
  
  //sprintf(sql,"SELECT dest_ipadd,udp_port FROM DestHost");
  ret=sqlite3_prepare_v2(db,sql,strlen(sql),&stmt,NULL);
  n=0;
  if(ret!=SQLITE_OK)
  {
     //sqlite3_finalize(stmt);
     if (stmt)
      sqlite3_finalize(stmt);
     //printf("init_dest_host:ret!=SQLITE_OK\n");
     return;
  }
  else 
  {
    while(sqlite3_step(stmt) ==SQLITE_ROW && n<10) {
      strncpy(dest_hosts[n].dest_ip,sqlite3_column_text(stmt,0),15);
      dest_hosts[n].port=sqlite3_column_int(stmt,1);
      //printf("ip=%s port=%d\n",dest_hosts[n].dest_ip,dest_hosts[n].port);
      //ret=sqlite3_step(stmt);
      n++;
   }
  }
  if (n<10)
    dest_hosts[n].port=0;

  if (stmt)
      sqlite3_finalize(stmt);
  sqlite3_close(db);
}
alarm_type_t *find_alarm_type(unsigned short alarm_code,char slot)
{
  int n;
  //alarm_type_t *atype;
  for (n=0;n<ALARM_TYPE_COUNT;n++)
  {
    if (alarm_types[n].alarm_code==alarm_code)
    {
      char m;
      for (m=0;m<16;m++)
      {
         if (alarm_types[n].mask_slot[m]==slot)
            return NULL;
         else if (alarm_types[n].mask_slot[m]==0)
         break;
      }
      //atype=alarm_types+n;
      return alarm_types+n;
    }
  }
  return NULL;
}
//unsigned int alarm_count=0;
/*
void sendAlarm1(int alarm_code,char chassis,char slot,char entity,char is_with_value,short alarm_value,unsigned short alarm_reason,time_t alarm_time,void(*value2message)(short,char *))
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
  
    
    memcpy(alarm_msg.alarm_info,&alarm,sizeof(alarm_info_t));
    alarm_msg.msg_type=1;
 
    if(msgsnd(almmsgid, &alarm_msg, sizeof(alarm_info_t), 0) == -1)  
    {  
        printf("msgsnd failed\n");   
    }
  }

  if (alarm_info_count<MAX_ALARM_INFO_COUNT)
  {
    alarms[alarm_info_count].alarm_code=alarm_code;
    alarms[alarm_info_count].chassis=chassis;
    alarms[alarm_info_count].slot=slot;
    alarms[alarm_info_count].entity=entity;
    alarms[alarm_info_count].alarm_value=alarm_value;
    alarms[alarm_info_count].alarm_reason=alarm_reason;
    alarms[alarm_info_count].alarm_time=alarm_time;
    alarms[alarm_info_count].value2message=value2message;
    alarm_info_count++;
    has_new_alarm=1;
    if (alarm_info_count<2)
        alarm(5);
  }
}
*/
void handleAlarm(sqlite3 *db,int alarm_code,char chassis,char slot,char entity,char is_with_value,short alarm_value,unsigned short alarm_reason,time_t alarm_time,void(*value2message)(short,char *))
{
 sqlite3_stmt* stmt=NULL;
 // sqlite3 *db;
  char *errmsg=NULL;
  int n,ret,enable=1,is_save=1;
  char sql[320]="";
  
  //char inbuf[]="中文";
  //size_t inlen=strlen(inbuf);
  char ut8buf[160];
  char gbkbuf[160]="";
  char bytes[6]="";
  char entity_name[20]="";
  char alarm_msg[120];
  #define SHMKEY_SET 1236L

 if (slot>16)
    slot=getSlotOfNmu(0,3);
 #ifndef IS_TEMP_VERSION
 /*
 if(solt_set_shm_addr==NULL)
 {
    int shmid_set=shmget(SHMKEY_SET,0,0666);
    //shmid_set=shmget(SHMKEY_SET,0,0666);
    //struct solt_set *addr;
    if (shmid_set!=-1)
      solt_set_shm_addr=(struct solt_set*)shmat(shmid_set,0,0);
    else
      solt_set_shm_addr=NULL;
 }
 */
   if(solt_set_shm_addr!=NULL)
   {
      if (slot>10)
       solt_set_shm_addr->run_led_keep_count=15;
      else if (slot>3)
       solt_set_shm_addr->run_led_keep_count=10;
      else
       solt_set_shm_addr->run_led_keep_count=5;
     
     if (alarm_code%2)
     {
      if (solt_set_shm_addr->run_led_status!=led_red_state)
      {
       solt_set_shm_addr->run_led_status=led_red_state;
       led_control(led_run_num,led_green_state);
       //usleep(200000);
       //led_control(led_run_num,led_red_state);
      }
     }
    else
    {
     if (solt_set_shm_addr->run_led_status!=led_green_state)
     {
       solt_set_shm_addr->run_led_status=led_green_state;
       led_control(led_run_num,led_green_state);
     }
    }
    //printf("#%d sendAlarm\r\n",slot);
    //led_control(led_run_num,led_red_state);
}
#endif

 #ifdef BIWEI_WUXI_VERSION
    // printf("Before alarm_code=%d..\n",alarm_code);
    if (alarm_code==403 ||alarm_code==404)
    {
     if (slot>0 && slot<16 && olp_osw[slot-1]==1)
     {
      if (alarm_code==403)
          alarm_code=404;
      else if (alarm_code==404)
          alarm_code=403;
     }
    }
    //printf("After alarm_code=%d..\n",alarm_code);
 #endif
  //printf("sendAlarm #%d alarm_code=%d alarm_value=%d %s\n",slot,alarm_code,alarm_value,ctime(&alarm_time));
  //ret=sqlite3_open(DB_FILE_NAME, &db);
  if (db==NULL)
  {
    printf("sqlite3_open Error:%s sql:%s\n",sqlite3_errmsg(db),sql);
    return;
  }
  //printf("sendAlarm:alarm_code=%d,chassis=%d,slot=%d,entity=%d\n",alarm_code,chassis,slot,entity);
  //sprintf(sql,"SELECT enable,is_save FROM AlarmType WHERE alarm_code=%d AND (enable=1 OR is_save=1) AND mask_slot LIKE '%c%02d;%c' AND mask_port NOT LIKE '%c%d;%c' LIMIT 1",alarm_code,'%',slot,'%','%',entity,'%');
  //sprintf(sql,"SELECT enable,is_save FROM AlarmType WHERE alarm_code=%d AND (enable=1 OR is_save=1) AND mask_slot LIKE '%c%02d;%c' LIMIT 1",alarm_code,'%',slot,'%');
  sprintf(sql,"SELECT enable,is_save,mask_slot FROM AlarmType WHERE alarm_code=%d LIMIT 1",alarm_code);
  //printf("%s\n",sql);
  //db=openDB(NULL);
  
  //sqlite3_close(db);
 //printf("sendAlarm 2\n");
  
  //return;
  //printf("sendAlarm #%d is_with_value=%d alarm_code=%d alarm_value=%d\n",slot,is_with_value,alarm_code,alarm_value);
  /*if (!(is_with_value==3 && (alarm_code==409 || alarm_code==411 || alarm_code==413|| alarm_code==503|| alarm_code==505 || alarm_code==507 || alarm_code==509)))//带值告警
  {
    is_with_value=0;
    alarm_value=0xffff;
  }
  */
  //printf("sendAlarm 1\n");
  ret=sqlite3_prepare_v2(db,sql,strlen(sql),&stmt,NULL);
  n=0;
  while (ret != SQLITE_OK && n<50)
  {
      if (SQLITE_LOCKED==ret || SQLITE_BUSY==ret)
      {
         usleep((1+n)*100000);
         ret=sqlite3_prepare_v2(db,sql,strlen(sql),&stmt,NULL);
         n++;
      }
      else
          break;
 }

  if(ret!=SQLITE_OK)
  { 
    // printf("errmsg=%s\n",errmsg);
     //sqlite3_free(errmsg);
     if (debug.is_debug_mode)
        printf("sqlite3_prepare_v2 Error:%s sql:%s\n",sqlite3_errmsg(db),sql);
     if (stmt)
         sqlite3_finalize(stmt);
     //sqlite3_close(db);
     return;
  }
  //ret=sqlite3_step(stmt);
  //sqlite3_finalize(stmt);
  //sqlite3_close(db);
 //printf("sendAlarm 2\n");
  
 // return;
  ret=sqlite3_step(stmt);
  if (ret==SQLITE_ROW)
  {
      /*
      if (strlen(sqlite3_column_text(stmt,2))>1)
        printf("mask_slot=%s\n",sqlite3_column_text(stmt,2));
      else
          printf("mask_slot=NULL");
      */
      if (strlen(sqlite3_column_text(stmt,2))>1)
      {
         char s[16];
         sprintf(s,"%02d;",slot);
         if (strstr(sqlite3_column_text(stmt,2),s)!=NULL)
         {
            enable=sqlite3_column_int(stmt,0);
            is_save=sqlite3_column_int(stmt,1);
         } 
      }
      else
      {
       enable=sqlite3_column_int(stmt,0);
       is_save=sqlite3_column_int(stmt,1);
      }
     // printf("value=%d enable=%d is_save=%d\n",sqlite3_column_int(stmt,0),enable,is_save);
  }
  /*
  else
  {
   sqlite3_finalize(stmt);
   sqlite3_close(db);
   //printf("sendAlarm 3\n");
   //setAlarmStatus(alarm_code-1,chassis,slot,entity,1);
   return;
  }
  */
  sqlite3_finalize(stmt);
 // printf("sendAlarm is_save=%d\n",is_save);
  //sqlite3_close(conn);
  //printf("sendSnmpTrap slot #%d alarm_code=%d is_save=%d enable=%d\n",slot,alarm_code,is_save,enable);
  if (is_save==2 && enable==2)
  {
    //sqlite3_close(db);
    return;
  }
  if (is_save==1)
  {
   //char port_name[8]="";
   /*if (insertAlarmLog(alarm_code,chassis,slot,entity,alarm_value)<0)
   {
    sqlite3_close(db);
    return;
   }
   */
    //printf("insertAlarmLog Slot=%d alarm_id=%d alarm_value=%x(%d)\r\n",slot,alarm_code,alarm_value,alarm_value);
    /*
    if (entity>0)
    {
       
       uchar_t uclass=0,utype=0;
       //getUnitCLassType(0,slot,&uclass,&utype);
       if (UC_OTU==uclass||UC_OLP==uclass)
       {
           if (UC_OTU==uclass)
               sprintf(port_name,"", entity%2+1);
            
       }
    }
    */
    /*
    if (value2message!=NULL)
    {
       char buf[32]="";
       value2message(alarm_value,buf);
       printf("alarm_value=%s\n",buf);
    }
   */
    insertAlarmLog(db,alarm_code,chassis,slot,entity,is_with_value,alarm_value,alarm_reason,alarm_time,value2message);
  }

  if (enable!=1)
  {
    //sqlite3_close(db);
    return;
  }
  //printf("sendSnmpTrap:\n");
  //return;
  /*
  db=openDB(DB_FILE_NAME);
  if (NULL==db)
     return;
 */
  sqlite3_stmt* stmt2;
  sprintf(sql,"SELECT A.dest_ipadd,A.udp_port,B.alarm_name_en,B.entity_name_en FROM DestHost AS A,AlarmType AS B WHERE A.enable=1 AND B.alarm_code=%d AND B.usable<>2",alarm_code);
  //printf("sendSnmpTrap:%s\n",sql);
  //sqlite3_close(db);
  //return;
  stmt2=NULL;
  ret=sqlite3_prepare_v2(db,sql,strlen(sql),&stmt2,NULL);
  if(ret!=SQLITE_OK)
  {
     //sqlite3_finalize(stmt);
     if (stmt2)
      sqlite3_finalize(stmt2);
     //printf("sendAlarm Error:sql:%s\n",sqlite3_errmsg(db),sql);
     //sqlite3_close(db);
     return;
  }
  //sqlite3_finalize(stmt);
  //sqlite3_close(db);
  //return;
  //printf("sendSnmpTrap..:%s\n",sql);
  ret=sqlite3_step(stmt2);
 /*
  if (ret != SQLITE_DONE) {
    sqlite3_finalize(stmt2);
    sqlite3_close(db);
    return;
  }
  */
  #ifdef BIWEI_WUXI_VERSION
  /*
   if (slot>0 && slot<16 && olp_osw[slot-1]==1)
   {
     if (alarm_code==404)
          alarm_code=403;
     else if (alarm_code==403)
          alarm_code=404;
   }
    //printf("alarm_code=%d..\n",alarm_code);
  */
  #endif
  uchar_t uclass=0,utype=0;
  
  getUnitClassType(0,slot,&uclass,&utype);
  while (ret==SQLITE_ROW)
  {
    //printf("sendSnmpTrap..:%d\n",n);
   /*
    if (n<-1)
    {
      if (entity>0)
          sprintf(entity_name,"%s %d",sqlite3_column_text(stmt,3),entity);
      else
          sprintf(entity_name,"%s",sqlite3_column_text(stmt,3));
      if (slot>0)
         //sprintf(ut8buf,"单元盘 #%d %s %s",slot,entity_name,sqlite3_column_text(stmt,2));
         sprintf(ut8buf,"Unit #%d %s %s",slot,entity_name,sqlite3_column_text(stmt,2));
      else
         sprintf(ut8buf,"%s %s",entity_name,sqlite3_column_text(stmt,3));
      //ret=u2g(ut8buf, strlen(ut8buf), gbkbuf,60);
      //if (ret)
      //{
       // strcpy(gbkbuf,ut8buf);
      //}
    }

    n++;
*/
    if (entity>0)
        sprintf(entity_name,"%s %d",sqlite3_column_text(stmt2,3),entity);
    else
       sprintf(entity_name,"%s",sqlite3_column_text(stmt2,3));
    if (slot>0)
         //sprintf(ut8buf,"单元盘 #%d %s %s",slot,entity_name,sqlite3_column_text(stmt,2));
        sprintf(ut8buf,"Unit #%d %s %s",slot,entity_name,sqlite3_column_text(stmt2,2));
    else
        sprintf(ut8buf,"%s %s",entity_name,sqlite3_column_text(stmt2,3));

    //doSystem("snmptrap -m \"\" -v 2c -c public %s:%d ' ' 1.3.6.1.4.1.10215.2.1.9.%d.%d.%d.%d 1.3.6.1.4.1.0 x '%s'",sqlite3_column_text(stmt,0),sqlite3_column_int(stmt,1),alarm_code,chassis,slot,entity,gbkbuf);
    //doSystem("snmptrap -m \"\" -v 2c -c public %s:%d ' ' 1.3.6.1.4.1.10215.2.1.9.%d  1.3.6.1.2.1.92.1.3.1.1.8.0 x '%2x%2x%2x%2x01%2x' 1.3.6.1.2.1.1.6.0 s '%s'",sqlite3_column_text(stmt,0),sqlite3_column_int(stmt,1),alarm_code,chassis,slot,entity,alarm_code &0xff,alarm_code>>8,ut8buf);
 /*if (alarm_code==409 || alarm_code==411 ||alarm_code==413)
   printf("alarm_code=%d alarm_value=%2f lo:%0x hi:%0x\n",alarm_code,0.1*alarm_value,alarm_value & 0xff,0xff & (alarm_value>>8));
*/
/*
 if (is_with_value!=3)
   doSystem("snmptrap -m \"\" -v 2c -c public %s:%d ' ' 1.3.6.1.4.1.10215.2.1.9.%d  1.3.6.1.2.1.92.1.3.1.1.8.0 x '%02x%02x%02x%02x%02x%02x' 1.3.6.1.2.1.1.6.0 s '%s'",sqlite3_column_text(stmt2,0),sqlite3_column_int(stmt2,1),alarm_code,chassis,slot,entity,is_with_value,alarm_code & 0xff,alarm_code>>8,ut8buf);
else
*/
   char dates[4];
   memcpy(dates,&alarm_time, 4);
   //printf("alarm_time=%ld\n",alarm_time);
   doSystem("snmptrap -m \"\" -v 2c -c public %s:%d ' ' 1.3.6.1.4.1.10215.2.1.9.%d  1.3.6.1.2.1.92.1.3.1.1.8.0 x '%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x' 1.3.6.1.2.1.1.6.0 s '%s'",sqlite3_column_text(stmt2,0),sqlite3_column_int(stmt2,1),alarm_code,chassis,slot,entity,is_with_value,alarm_code & 0xff,alarm_code>>8,alarm_value & 0xff,0xff & (alarm_value>>8),0,0,uclass,utype,dates[0],dates[1],dates[2],dates[3],ut8buf);
  
 // printf("snmptrap -m \"\" -v 2c -c public %s:%d ' ' 1.3.6.1.4.1.10215.2.1.9.%d  1.3.6.1.2.1.92.1.3.1.1.8.0 x '%02x%02x%02x%02x%02x%02x%02x%02x' 1.3.6.1.2.1.1.6.0 s '%s'",sqlite3_column_text(stmt2,0),sqlite3_column_int(stmt2,1),alarm_code,chassis,slot,entity,is_with_value,alarm_code & 0xff,alarm_code>>8,alarm_value & 0xff,0xff & (alarm_value>>8),ut8buf);
  
  // printf("snmptrap -m \"\" -v 2c -c public %s:%d ' ' 1.3.6.1.4.1.10215.2.1.9.%d  1.3.6.1.2.1.92.1.3.1.1.8.0 x '%02x %02x %02x 01 %02x %02x' 1.3.6.1.2.1.1.6.0 s '%s'",sqlite3_column_text(stmt,0),sqlite3_column_int(stmt,1),alarm_code,chassis,slot,entity,alarm_code &0xff,alarm_code>>8,ut8buf);
//printf("snmptrap -m \"\" -v 2c -c public %s:%d ' ' 1.3.6.1.4.1.10215.2.1.9.%d  1.3.6.1.2.1.92.1.3.1.1.8.0 x '%02x%02x%02x01%02x%02x' 1.3.6.1.2.1.1.6.0 s '%s'",sqlite3_column_text(stmt,0),sqlite3_column_int(stmt,1),alarm_code,chassis,slot,entity,alarm_code &0xff,alarm_code>>8,ut8buf);
   // printf("snmptrap -m \"\" -v 2c -c public %s:%d ' ' 1.3.6.1.4.1.10215.2.1.9.%d  1.3.6.1.2.1.92.1.3.1.1.8.0 s '%d %d %d %d' 1.3.6.1.2.1.1.6.0 x '%s'",sqlite3_column_text(stmt,0),sqlite3_column_int(stmt,1),alarm_code,chassis,slot,entity,alarm_code,gbkbuf);
    //printf("snmptrap -v 2c -c public %s:%d ' ' 1.3.6.1.4.1.10215.2.1.9.%d.%d.%d.%d 1.3.6.1.4.1.0 s 'unit changed'",sqlite3_column_text(stmt,0),sqlite3_column_int(stmt,1),alarm_code,chassis,slot,entity);
    ret=sqlite3_step(stmt2);
  }
  sqlite3_finalize(stmt2);
  //sqlite3_close(db);
}
void sendTrap(char *dest_ip,unsigned int port,unsigned short alarm_code,uchar_t slot,uchar_t entity,char is_with_value,short alarm_value,uchar_t uclass,uchar_t utype,char *dates,char *alarm_info)
{
doSystem("snmptrap -m \"\" -v 2c -c public %s:%d ' ' 1.3.6.1.4.1.10215.2.1.9.%d  1.3.6.1.2.1.92.1.3.1.1.8.0 x '%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x' 1.3.6.1.2.1.1.6.0 s '%s'",dest_ip,port,alarm_code,0,slot,entity,is_with_value,alarm_code & 0xff,alarm_code>>8,alarm_value & 0xff,0xff & (alarm_value>>8),0,0,uclass,utype,dates[0],dates[1],dates[2],dates[3],alarm_info);
}
void addAlarm(int alarm_code,char chassis,char slot,char entity,char is_with_value,short alarm_value,unsigned short alarm_reason,time_t alarm_time,void(*value2message)(short,char *))
{
  //sqlite3_stmt* stmt=NULL;
  //sqlite3 *db;
  //char *errmsg=NULL;
  int n;//,ret,enable=1,is_save=1;
  //char sql[320]="";
  
  //char inbuf[]="中文";
  //size_t inlen=strlen(inbuf);
  char ut8buf[160];
  //char gbkbuf[160]="";
  //char bytes[6]="";
  char entity_name[20]="";
  char dates[4];
  uchar_t uclass=0,utype=0;
  //char alarm_msg[120];
  #define SHMKEY_SET 1236L

  if (slot>16)
    slot=getSlotOfNmu(0,3);
  getUnitClassType(0,slot,&uclass,&utype);
  //printf("sendAlarm #%d alarm_code=%d alarm_value=%d %s\n",slot,alarm_code,alarm_value,ctime(&alarm_time));
  if (alarm_code>2000)
  {
    for (n=0;n<ALARM_TYPE_COUNT;n++)
    {
     if (dest_hosts[n].port<1)
       break;
      
      memcpy(dates,&alarm_time, 4);
      sendTrap(dest_hosts[n].dest_ip,dest_hosts[n].port,alarm_code,slot,entity,is_with_value,alarm_value,uclass,utype,dates,"");
    }
    return;
  }
  alarm_type_t *atype=find_alarm_type(alarm_code,slot);
  if (atype==NULL)
    return;

  //printf("sendAlarm #%d alarm_code=%d alarm_value=%d %s\n",slot,alarm_code,alarm_value,ctime(&alarm_time));
  /*
  ret=sqlite3_open(DB_FILE_NAME, &db);
  if (db==NULL)
  {
    printf("sqlite3_open Error:%s sql:%s\n",sqlite3_errmsg(db),sql);
    return;
  }

  sprintf(sql,"SELECT enable,is_save,mask_slot FROM AlarmType WHERE alarm_code=%d LIMIT 1",alarm_code);

  ret=sqlite3_prepare_v2(db,sql,strlen(sql),&stmt,NULL);
  n=0;
  while (ret != SQLITE_OK && n<50)
  {
      if (SQLITE_LOCKED==ret || SQLITE_BUSY==ret)
      {
         usleep((1+n)*100000);
         ret=sqlite3_prepare_v2(db,sql,strlen(sql),&stmt,NULL);
         n++;
      }
      else
          break;
 }

  if(ret!=SQLITE_OK)
  { 
     if (debug.is_debug_mode)
        printf("sqlite3_prepare_v2 Error:%s sql:%s\n",sqlite3_errmsg(db),sql);
     if (stmt)
         sqlite3_finalize(stmt);
     sqlite3_close(db);
     return;
  }

  ret=sqlite3_step(stmt);
  if (ret==SQLITE_ROW)
  {

      if (strlen(sqlite3_column_text(stmt,2))>1)
      {
         char s[16];
         sprintf(s,"%02d;",slot);
         if (strstr(sqlite3_column_text(stmt,2),s)!=NULL)
         {
            enable=sqlite3_column_int(stmt,0);
            is_save=sqlite3_column_int(stmt,1);
         } 
      }
      else
      
      {
       enable=sqlite3_column_int(stmt,0);
       is_save=sqlite3_column_int(stmt,1);
      }
  }
  sqlite3_finalize(stmt);
  */
  if (atype->is_save==2 && atype->is_send==2)
  {
    //sqlite3_close(db);
    return;
  }
  if (atype->is_save==1 && alarm_code<2000)
  {
   insertAlarmLog(NULL,alarm_code,chassis,slot,entity,is_with_value,alarm_value,alarm_reason,alarm_time,value2message);
  }
  
  if (atype->is_send!=1)
  {
    //sqlite3_close(db);
    return;
  }
  /*
  sqlite3_stmt* stmt2;
  sprintf(sql,"SELECT A.dest_ipadd,A.udp_port,B.alarm_name_en,B.entity_name_en FROM DestHost AS A,AlarmType AS B WHERE A.enable=1 AND B.alarm_code=%d AND B.usable<>2",alarm_code);

  stmt2=NULL;
  ret=sqlite3_prepare_v2(db,sql,strlen(sql),&stmt2,NULL);
  if(ret!=SQLITE_OK)
  {
     //sqlite3_finalize(stmt);
     if (stmt2)
      sqlite3_finalize(stmt2);
     //printf("sendAlarm Error:sql:%s\n",sqlite3_errmsg(db),sql);
     sqlite3_close(db);
     return;
  }

  ret=sqlite3_step(stmt2);
  */
  //uchar_t uclass=0,utype=0; 
  //getUnitClassType(0,slot,&uclass,&utype);
  
  for (n=0;n<ALARM_TYPE_COUNT;n++)
  {
    if (dest_hosts[n].port<1)
       break;
    if (entity>0)
        sprintf(entity_name,"%s %d",atype->entity_name,entity);
    else
       sprintf(entity_name,"%s",atype->entity_name);
    if (slot>0)
         //sprintf(ut8buf,"单元盘 #%d %s %s",slot,entity_name,sqlite3_column_text(stmt,2));
        sprintf(ut8buf,"Unit #%d %s %s",slot,entity_name,atype->alarm_name);
    else
        sprintf(ut8buf,"%s %s",entity_name,atype->alarm_name);
 
   memcpy(dates,&alarm_time, 4);
   //printf("alarm_time=%ld\n",alarm_time);
   doSystem("snmptrap -m \"\" -v 2c -c public %s:%d ' ' 1.3.6.1.4.1.10215.2.1.9.%d  1.3.6.1.2.1.92.1.3.1.1.8.0 x '%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x' 1.3.6.1.2.1.1.6.0 s '%s'",dest_hosts[n].dest_ip,dest_hosts[n].port,alarm_code,chassis,slot,entity,is_with_value,alarm_code & 0xff,alarm_code>>8,alarm_value & 0xff,0xff & (alarm_value>>8),0,0,uclass,utype,dates[0],dates[1],dates[2],dates[3],ut8buf);
  
  //  printf("snmptrap -m \"\" -v 2c -c public %s:%d ' ' 1.3.6.1.4.1.10215.2.1.9.%d  1.3.6.1.2.1.92.1.3.1.1.8.0 x '%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x' 1.3.6.1.2.1.1.6.0 s '%s'",dest_hosts[n].dest_ip,dest_hosts[n].port,alarm_code,chassis,slot,entity,is_with_value,alarm_code & 0xff,alarm_code>>8,alarm_value & 0xff,0xff & (alarm_value>>8),0,0,uclass,utype,dates[0],dates[1],dates[2],dates[3],ut8buf);
 //ret=sqlite3_step(stmt2);
  }
  //sqlite3_finalize(stmt2);
  //sqlite3_close(db);
}

void addAlarm2(int alarm_code,char chassis,char slot,char entity,char is_with_value,short alarm_value,unsigned short alarm_reason,time_t alarm_time,void(*value2message)(short,char *))
{
  sqlite3_stmt* stmt=NULL;
  sqlite3 *db;
  char *errmsg=NULL;
  int n,ret,enable=1,is_save=1;
  char sql[320]="";
  
  //char inbuf[]="中文";
  //size_t inlen=strlen(inbuf);
  char ut8buf[160];
  char gbkbuf[160]="";
  char bytes[6]="";
  char entity_name[20]="";
  char alarm_msg[120];
  #define SHMKEY_SET 1236L

 if (slot>16)
    slot=getSlotOfNmu(0,3);
 alarm_type_t *atype=find_alarm_type(alarm_code,slot);
 if (atype==NULL)
    return;
 /*if (atype!=NULL)
  printf("slot=%d alarm_code=%d\n",slot,alarm_code);
 else
  printf("atype==NULL slot=%d alarm_code=%d\n",slot,alarm_code);
*/
 #ifndef IS_TEMP_VERSION
 /*
 if(solt_set_shm_addr==NULL)
 {
    int shmid_set=shmget(SHMKEY_SET,0,0666);
    //shmid_set=shmget(SHMKEY_SET,0,0666);
    //struct solt_set *addr;
    if (shmid_set!=-1)
      solt_set_shm_addr=(struct solt_set*)shmat(shmid_set,0,0);
    else
      solt_set_shm_addr=NULL;
 }
 */
/*
   if(solt_set_shm_addr!=NULL)
   {
      if (slot>10)
       solt_set_shm_addr->run_led_keep_count=15;
      else if (slot>3)
       solt_set_shm_addr->run_led_keep_count=10;
      else
       solt_set_shm_addr->run_led_keep_count=5;
     
     if (alarm_code%2)
     {
      if (solt_set_shm_addr->run_led_status!=led_red_state)
      {
       solt_set_shm_addr->run_led_status=led_red_state;
       led_control(led_run_num,led_green_state);
      }
     }
    else
    {
     if (solt_set_shm_addr->run_led_status!=led_green_state)
     {
       solt_set_shm_addr->run_led_status=led_green_state;
       led_control(led_run_num,led_green_state);
     }
    }
    //printf("#%d sendAlarm\r\n",slot);
    //led_control(led_run_num,led_red_state);
}
*/
#endif

 #ifdef BIWEI_WUXI_VERSION
    // printf("Before alarm_code=%d..\n",alarm_code);
    if (alarm_code==403 ||alarm_code==404)
    {
     if (slot>0 && slot<16 && olp_osw[slot-1]==1)
     {
      if (alarm_code==403)
          alarm_code=404;
      else if (alarm_code==404)
          alarm_code=403;
     }
    }
    if (alarm_reason<0 || alarm_reason>100)
       alarm_reason=0;
    //printf("After alarm_code=%d..\n",alarm_code);
 #endif
  //printf("sendAlarm #%d alarm_code=%d alarm_value=%d %s\n",slot,alarm_code,alarm_value,ctime(&alarm_time));
  ret=sqlite3_open(DB_FILE_NAME, &db);
  if (db==NULL)
  {
    printf("sqlite3_open Error:%s sql:%s\n",sqlite3_errmsg(db),sql);
    return;
  }
  //printf("sendAlarm:alarm_code=%d,chassis=%d,slot=%d,entity=%d\n",alarm_code,chassis,slot,entity);
  //sprintf(sql,"SELECT enable,is_save FROM AlarmType WHERE alarm_code=%d AND (enable=1 OR is_save=1) AND mask_slot LIKE '%c%02d;%c' AND mask_port NOT LIKE '%c%d;%c' LIMIT 1",alarm_code,'%',slot,'%','%',entity,'%');
  //sprintf(sql,"SELECT enable,is_save FROM AlarmType WHERE alarm_code=%d AND (enable=1 OR is_save=1) AND mask_slot LIKE '%c%02d;%c' LIMIT 1",alarm_code,'%',slot,'%');
  sprintf(sql,"SELECT enable,is_save,mask_slot FROM AlarmType WHERE alarm_code=%d LIMIT 1",alarm_code);
  //printf("%s\n",sql);
  //db=openDB(NULL);
  
  //sqlite3_close(db);
 //printf("sendAlarm 2\n");
  
  //return;
  //printf("sendAlarm #%d is_with_value=%d alarm_code=%d alarm_value=%d\n",slot,is_with_value,alarm_code,alarm_value);
  /*if (!(is_with_value==3 && (alarm_code==409 || alarm_code==411 || alarm_code==413|| alarm_code==503|| alarm_code==505 || alarm_code==507 || alarm_code==509)))//带值告警
  {
    is_with_value=0;
    alarm_value=0xffff;
  }
  */
  //printf("sendAlarm 1\n");
  ret=sqlite3_prepare_v2(db,sql,strlen(sql),&stmt,NULL);
  n=0;
  while (ret != SQLITE_OK && n<50)
  {
      if (SQLITE_LOCKED==ret || SQLITE_BUSY==ret)
      {
         usleep((1+n)*100000);
         ret=sqlite3_prepare_v2(db,sql,strlen(sql),&stmt,NULL);
         n++;
      }
      else
          break;
 }

  if(ret!=SQLITE_OK)
  { 
    // printf("errmsg=%s\n",errmsg);
     //sqlite3_free(errmsg);
     if (debug.is_debug_mode)
        printf("sqlite3_prepare_v2 Error:%s sql:%s\n",sqlite3_errmsg(db),sql);
     if (stmt)
         sqlite3_finalize(stmt);
     sqlite3_close(db);
     return;
  }
  //ret=sqlite3_step(stmt);
  //sqlite3_finalize(stmt);
  //sqlite3_close(db);
 //printf("sendAlarm 2\n");
  
 // return;
  ret=sqlite3_step(stmt);
  if (ret==SQLITE_ROW)
  {
      /*
      if (strlen(sqlite3_column_text(stmt,2))>1)
        printf("mask_slot=%s\n",sqlite3_column_text(stmt,2));
      else
          printf("mask_slot=NULL");
      */
      if (strlen(sqlite3_column_text(stmt,2))>1)
      {
         char s[16];
         sprintf(s,"%02d;",slot);
         if (strstr(sqlite3_column_text(stmt,2),s)!=NULL)
         {
            enable=sqlite3_column_int(stmt,0);
            is_save=sqlite3_column_int(stmt,1);
         } 
      }
      else
      
      {
       enable=sqlite3_column_int(stmt,0);
       is_save=sqlite3_column_int(stmt,1);
      }
     // printf("value=%d enable=%d is_save=%d\n",sqlite3_column_int(stmt,0),enable,is_save);
  }
  /*
  else
  {
   sqlite3_finalize(stmt);
   sqlite3_close(db);
   //printf("sendAlarm 3\n");
   //setAlarmStatus(alarm_code-1,chassis,slot,entity,1);
   return;
  }
  */
  sqlite3_finalize(stmt);
 // printf("sendAlarm is_save=%d\n",is_save);
  //sqlite3_close(conn);
  //printf("sendSnmpTrap slot #%d alarm_code=%d is_save=%d enable=%d\n",slot,alarm_code,is_save,enable);
  if (is_save==2 && enable==2)
  {
    sqlite3_close(db);
    return;
  }
  if (is_save==1)
  {
   char port_name[8]="";
   /*if (insertAlarmLog(alarm_code,chassis,slot,entity,alarm_value)<0)
   {
    sqlite3_close(db);
    return;
   }
   */
    //printf("insertAlarmLog Slot=%d alarm_id=%d alarm_value=%x(%d)\r\n",slot,alarm_code,alarm_value,alarm_value);
    /*
    if (entity>0)
    {
       
       uchar_t uclass=0,utype=0;
       //getUnitCLassType(0,slot,&uclass,&utype);
       if (UC_OTU==uclass||UC_OLP==uclass)
       {
           if (UC_OTU==uclass)
               sprintf(port_name,"", entity%2+1);
            
       }
    }
    */
    /*
    if (value2message!=NULL)
    {
       char buf[32]="";
       value2message(alarm_value,buf);
       printf("alarm_value=%s\n",buf);
    }
   */
    insertAlarmLog(db,alarm_code,chassis,slot,entity,is_with_value,alarm_value,alarm_reason,alarm_time,value2message);
  }

  if (enable!=1)
  {
    sqlite3_close(db);
    return;
  }
  //printf("sendSnmpTrap:\n");
  //return;
  /*
  db=openDB(DB_FILE_NAME);
  if (NULL==db)
     return;
 */
  sqlite3_stmt* stmt2;
  sprintf(sql,"SELECT A.dest_ipadd,A.udp_port,B.alarm_name_en,B.entity_name_en FROM DestHost AS A,AlarmType AS B WHERE A.enable=1 AND B.alarm_code=%d AND B.usable<>2",alarm_code);
  //printf("sendSnmpTrap:%s\n",sql);
  //sqlite3_close(db);
  //return;
  stmt2=NULL;
  ret=sqlite3_prepare_v2(db,sql,strlen(sql),&stmt2,NULL);
  if(ret!=SQLITE_OK)
  {
     //sqlite3_finalize(stmt);
     if (stmt2)
      sqlite3_finalize(stmt2);
     //printf("sendAlarm Error:sql:%s\n",sqlite3_errmsg(db),sql);
     sqlite3_close(db);
     return;
  }
  //sqlite3_finalize(stmt);
  //sqlite3_close(db);
  //return;
  //printf("sendSnmpTrap..:%s\n",sql);
  ret=sqlite3_step(stmt2);
 /*
  if (ret != SQLITE_DONE) {
    sqlite3_finalize(stmt2);
    sqlite3_close(db);
    return;
  }
  */
  #ifdef BIWEI_WUXI_VERSION
  /*
   if (slot>0 && slot<16 && olp_osw[slot-1]==1)
   {
     if (alarm_code==404)
          alarm_code=403;
     else if (alarm_code==403)
          alarm_code=404;
   }
    //printf("alarm_code=%d..\n",alarm_code);
  */
  #endif
  uchar_t uclass=0,utype=0;
  
  getUnitClassType(0,slot,&uclass,&utype);
  while (ret==SQLITE_ROW)
  {
    //printf("sendSnmpTrap..:%d\n",n);
   /*
    if (n<-1)
    {
      if (entity>0)
          sprintf(entity_name,"%s %d",sqlite3_column_text(stmt,3),entity);
      else
          sprintf(entity_name,"%s",sqlite3_column_text(stmt,3));
      if (slot>0)
         //sprintf(ut8buf,"单元盘 #%d %s %s",slot,entity_name,sqlite3_column_text(stmt,2));
         sprintf(ut8buf,"Unit #%d %s %s",slot,entity_name,sqlite3_column_text(stmt,2));
      else
         sprintf(ut8buf,"%s %s",entity_name,sqlite3_column_text(stmt,3));
      //ret=u2g(ut8buf, strlen(ut8buf), gbkbuf,60);
      //if (ret)
      //{
       // strcpy(gbkbuf,ut8buf);
      //}
    }

    n++;
*/
    if (entity>0)
        sprintf(entity_name,"%s %d",sqlite3_column_text(stmt2,3),entity);
    else
       sprintf(entity_name,"%s",sqlite3_column_text(stmt2,3));
    if (slot>0)
         //sprintf(ut8buf,"单元盘 #%d %s %s",slot,entity_name,sqlite3_column_text(stmt,2));
        sprintf(ut8buf,"Unit #%d %s %s",slot,entity_name,sqlite3_column_text(stmt2,2));
    else
        sprintf(ut8buf,"%s %s",entity_name,sqlite3_column_text(stmt2,3));

    //doSystem("snmptrap -m \"\" -v 2c -c public %s:%d ' ' 1.3.6.1.4.1.10215.2.1.9.%d.%d.%d.%d 1.3.6.1.4.1.0 x '%s'",sqlite3_column_text(stmt,0),sqlite3_column_int(stmt,1),alarm_code,chassis,slot,entity,gbkbuf);
    //doSystem("snmptrap -m \"\" -v 2c -c public %s:%d ' ' 1.3.6.1.4.1.10215.2.1.9.%d  1.3.6.1.2.1.92.1.3.1.1.8.0 x '%2x%2x%2x%2x01%2x' 1.3.6.1.2.1.1.6.0 s '%s'",sqlite3_column_text(stmt,0),sqlite3_column_int(stmt,1),alarm_code,chassis,slot,entity,alarm_code &0xff,alarm_code>>8,ut8buf);
 /*if (alarm_code==409 || alarm_code==411 ||alarm_code==413)
   printf("alarm_code=%d alarm_value=%2f lo:%0x hi:%0x\n",alarm_code,0.1*alarm_value,alarm_value & 0xff,0xff & (alarm_value>>8));
*/
/*
 if (is_with_value!=3)
   doSystem("snmptrap -m \"\" -v 2c -c public %s:%d ' ' 1.3.6.1.4.1.10215.2.1.9.%d  1.3.6.1.2.1.92.1.3.1.1.8.0 x '%02x%02x%02x%02x%02x%02x' 1.3.6.1.2.1.1.6.0 s '%s'",sqlite3_column_text(stmt2,0),sqlite3_column_int(stmt2,1),alarm_code,chassis,slot,entity,is_with_value,alarm_code & 0xff,alarm_code>>8,ut8buf);
else
*/
   char dates[4];
   memcpy(dates,&alarm_time, 4);
   //printf("alarm_time=%ld\n",alarm_time);
   doSystem("snmptrap -m \"\" -v 2c -c public %s:%d ' ' 1.3.6.1.4.1.10215.2.1.9.%d  1.3.6.1.2.1.92.1.3.1.1.8.0 x '%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x' 1.3.6.1.2.1.1.6.0 s '%s'",sqlite3_column_text(stmt2,0),sqlite3_column_int(stmt2,1),alarm_code,chassis,slot,entity,is_with_value,alarm_code & 0xff,alarm_code>>8,alarm_value & 0xff,0xff & (alarm_value>>8),0,0,uclass,utype,dates[0],dates[1],dates[2],dates[3],ut8buf);
  
 // printf("snmptrap -m \"\" -v 2c -c public %s:%d ' ' 1.3.6.1.4.1.10215.2.1.9.%d  1.3.6.1.2.1.92.1.3.1.1.8.0 x '%02x%02x%02x%02x%02x%02x%02x%02x' 1.3.6.1.2.1.1.6.0 s '%s'",sqlite3_column_text(stmt2,0),sqlite3_column_int(stmt2,1),alarm_code,chassis,slot,entity,is_with_value,alarm_code & 0xff,alarm_code>>8,alarm_value & 0xff,0xff & (alarm_value>>8),ut8buf);
  
  // printf("snmptrap -m \"\" -v 2c -c public %s:%d ' ' 1.3.6.1.4.1.10215.2.1.9.%d  1.3.6.1.2.1.92.1.3.1.1.8.0 x '%02x %02x %02x 01 %02x %02x' 1.3.6.1.2.1.1.6.0 s '%s'",sqlite3_column_text(stmt,0),sqlite3_column_int(stmt,1),alarm_code,chassis,slot,entity,alarm_code &0xff,alarm_code>>8,ut8buf);
//printf("snmptrap -m \"\" -v 2c -c public %s:%d ' ' 1.3.6.1.4.1.10215.2.1.9.%d  1.3.6.1.2.1.92.1.3.1.1.8.0 x '%02x%02x%02x01%02x%02x' 1.3.6.1.2.1.1.6.0 s '%s'",sqlite3_column_text(stmt,0),sqlite3_column_int(stmt,1),alarm_code,chassis,slot,entity,alarm_code &0xff,alarm_code>>8,ut8buf);
   // printf("snmptrap -m \"\" -v 2c -c public %s:%d ' ' 1.3.6.1.4.1.10215.2.1.9.%d  1.3.6.1.2.1.92.1.3.1.1.8.0 s '%d %d %d %d' 1.3.6.1.2.1.1.6.0 x '%s'",sqlite3_column_text(stmt,0),sqlite3_column_int(stmt,1),alarm_code,chassis,slot,entity,alarm_code,gbkbuf);
    //printf("snmptrap -v 2c -c public %s:%d ' ' 1.3.6.1.4.1.10215.2.1.9.%d.%d.%d.%d 1.3.6.1.4.1.0 s 'unit changed'",sqlite3_column_text(stmt,0),sqlite3_column_int(stmt,1),alarm_code,chassis,slot,entity);
    ret=sqlite3_step(stmt2);
  }
  sqlite3_finalize(stmt2);
  sqlite3_close(db);
}
/*
void alarmHandle(int sig,struct siginfo *psiginfo,void *act)
{
  return;
  if (alarm_info_count<1)
    return;
  if (alarm_info_count<MAX_ALARM_INFO_COUNT && !has_new_alarm ||alarm_info_count==MAX_ALARM_INFO_COUNT)
  {
    int n=0;
    //printf("alarm count:%d\n",alarm_info_count);
    for (n=0;n<alarm_info_count;n++)
    {
      addAlarm(alarms[n].alarm_code,alarms[n].chassis,alarms[n].slot,alarms[n].entity,alarms[n]. is_with_value,alarms[n].alarm_value,alarms[n].alarm_reason,alarms[n].alarm_time,alarms[n].value2message);
    }
    has_new_alarm=0;
    alarm_info_count=0;
  }
  else if (has_new_alarm)
  {
    has_new_alarm=0;
    alarm(5);
    return;
  }
}
*/
