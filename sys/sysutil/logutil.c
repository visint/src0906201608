#include <stdio.h>
#include <string.h>
#include <unit.h>
#include <sqlite3.h>
#include <dbutil.h>
#include <log.h>
#include <public.h>
int getOptLogId(uchar_t slot)
{
  uchar_t uclass,utype;
  if (TRUE!=getUnitClassType(0,slot,&uclass,&utype))
    return 0;
  int log_id=0;
  switch (uclass)
  {
    case UC_OTU:
          log_id=LOG_UNIT_OTU_CONF;
          break;
   case UC_OTAP:
          if (utype!=5)
             log_id=LOG_UNIT_OTAP_CONF;
          else
              log_id=LOG_UNIT_OSC_CONF;
          break;
    case UC_OEO:
          log_id=LOG_UNIT_OEO_CONF;
          break;
    case UC_OLP:
          if (utype<51)
           log_id=LOG_UNIT_OLP_CONF;
          else
            log_id=LOG_UNIT_OBP_CONF;
          break;
    case UC_OBP:
          log_id=LOG_UNIT_OBP_CONF;
          break;
    case UC_OSS:
          log_id=LOG_UNIT_OSS_CONF;
          break;
    case UC_EDFA:
          log_id=LOG_UNIT_EDFA_CONF;
          break;
   // case UC_ETU:
   //       strcpy(class_name,"ETU");
          break;
     case UC_DVOA:
          log_id=LOG_UNIT_DVOA_CONF;
          break;
  }
  return log_id;
}
int insertOperateLog(int group_id,int object_id,int user_id,int mode,int operate_type,int slot,int port)
{
  char sql[180];

  //sprintf(sql,"SELECT A.group_id FROM OptLog AS A,OptObjGroup AS B WHERE A.group_id=B.ID AND A.group_id=%d AND B.enable=1 LIMIT 1",group_id);
  sprintf(sql,"SELECT ID FROM OptObjGroup WHERE ID=%d AND enable=1 LIMIT 1",group_id);
  if (getRowCount(NULL,sql) <1)
  {
    return -1;
  }
  sprintf(sql,"DELETE FROM OptLog WHERE ID NOT IN (SELECT ID FROM OptLog order by operate_time desc LIMIT %d)",MAX_LOG_COUNT-1);
  if (SQLITE_OK!=execSql(NULL, sql,NULL,NULL))
        return -1;

  sprintf(sql,"INSERT INTO \"OptLog\" VALUES(NULL,%d,%d,%d,%d,%d,datetime('now','localtime'),%d,%d);",group_id,object_id,user_id,mode,operate_type,slot,port);
  //printf("sql:%s\r\n",sql);
  if (SQLITE_OK!=execSql(NULL, sql,NULL,NULL))
       return -1;
  else
       return 0;
}
static int max_slog_count=-1;

int addSysLog(short log_type_id,const char *fmt, ...)  
{         
  char message[256];          
  va_list pargv;
  va_start(pargv, fmt);
  vsnprintf(message, sizeof(message), fmt, pargv);
  va_end(pargv);
  
  return insertSysLog(log_type_id,message);
}

int insertSysLog(short log_type_id,char message[])
{
  //if (debug.is_debug_mode && debug.is_print_err)
  //  printf("Sys Error:%s\n",message);
  //printf("max_slog_count:%d\n",debug.max_syslog_count);
  /*
  if (max_slog_count<0)
  {
     char value[16]="";
     getConfValue(SYS_CONF_FILE,"max_slcount",value," ");
     max_slog_count=atoi(value);
  }
  */
  if (debug.max_syslog_count>0)
  {
  char sql[180];
  sprintf(sql,"DELETE FROM SysLog WHERE ID NOT IN (SELECT ID FROM SysLog order by occured_time desc  LIMIT %d)",(debug.max_syslog_count-1));
  execSql(NULL, sql,NULL,NULL);
  sprintf(sql,"INSERT INTO SysLog VALUES(NULL,'%s',datetime('now','localtime'));",message);
  //printf("insertSysLog sql:%s max_slog_count=%d\n",sql,debug.max_syslog_count);
  if (SQLITE_OK!=execSql(NULL, sql,NULL,NULL))
       return -1;
  else
       return 0;
 }
 return 1;
}
int insertUnitSettingLog(int object_id,int user_id,int mode,int slot,int port)
{
  int group_id=getOptLogId((uchar_t)slot);
  //printf("group_id=%d slot=%d\r\n",group_id,slot);
  if (group_id<1)
     return 0;
  int ret= insertOperateLog(group_id,object_id,user_id,mode,mod,slot,port);
  //printf("group_id=%d slot=%d ret=%d\r\n",group_id,slot,ret);
  return ret;
}
int deleteOperateLogById(int id)
{
  char sql[50]="DELETE FROM OptLog";

  if (id!=0)
      sprintf(sql,"DELETE FROM OptLog WHERE ID=%d;",id);
  //printf("sql=%s\n",sql);
  if (SQLITE_OK!=execSql(NULL, sql,NULL,NULL))
        return -1;
  else
        return 0;
 //printf("sql=%s\n",sql);
}
/*
int deleteOperateLogByIp(char * ip)
{
  char sql[60];
  sprintf(sql,"DELETE FROM \"Community\" WHERE ipadd='%s';",ip);
  if (SQLITE_OK!=execSql(NULL, sql,NULL,NULL))
        return -1;
  else
        return 0;
}
*/

