/*
 *  unit.h -- Vispace Unit Management public header
 *
 *  Copyright (c) Visint Inc., 2002-2012. All Rights Reserved.
 *
 *  See the file "license.txt" for information on usage and redistribution
 *
 */

#ifndef __LOG_H
#define __LOG_H 1

/******************************** Description *********************************/

/*
 *  Vispace Unit header. This defines the Unit Management
 *  public APIs.  Include this header for files that contain access to
 *  unit inquiry or management.
 */

/********************************* Includes ***********************************/

//#include  <typedefs.h>

/********************************** Defines ***********************************/
#define MAX_LOG_COUNT 200	/*最大保存日志条数*/

#define LOG_GROUP_SYS_IP_CONF 11
#define LOG_GROUP_SYS_SYS_INFO_CONF 12
#define LOG_GROUP_SYS_USER_CONF 13
#define LOG_GROUP_SYS_USER_ACL_CONF 14
#define LOG_GROUP_SYS_USER_GROUP_CONF 15
#define LOG_GROUP_SYS_PASSWORD_CONF 16
#define LOG_GROUP_SYS_SAVE_USER_CONF 17

#define LOG_GROUP_SYS_SNMP_COMMINITY_CONF 20
#define LOG_GROUP_SYS_SNMP_DEST_HOST_CONF 21

#define LOG_GROUP_SYS_CURRENT_ALARM_CONF 22
#define LOG_GROUP_SYS_HISTORY_ALARM_CONF 23
#define LOG_GROUP_SYS_SNMP_ALARM_TYPE_CONF 24

#define LOG_GROUP_SYS_OPT_LOG_CONF 25

#define LOG_DDM_THRESHOLD_HCONF 30
#define LOG_UNIT_OTU_CONF  31
#define LOG_UNIT_OLP_CONF 32
#define LOG_UNIT_OBP_CONF 33
#define LOG_UNIT_OEO_CONF  34
#define LOG_UNIT_OSS_CONF 35
#define LOG_UNIT_OTAP_CONF 36
#define LOG_UNIT_OSC_CONF  37
#define LOG_UNIT_DVOA_CONF 38
#define LOG_UNIT_EDFA_CONF 39

//#define LOG_UNIT_CONF 100
#define LOG_OLP_WD_OK 101
#define LOG_OLP_WD_FAIL 102
#define LOG_HEARTBEAT_WD_OK 103
#define LOG_HEARTBEAT_WD_FAIL 104

#define SYS_LOG_SNMP_KILLED 110
#define SYS_lOG_SNMP_TIMEOUT 111
#define SYS_LOG_SNMP_EXIT 112
#define SYS_LOG_PING_FAIL 113
#define SYS_LOG_IO_ERR 120
#define SYS_LOG_DB_ERR 121

enum LogOptMode
{
  UnkownMode=0,
  System=1,
  CliMode=2,
  WebMode=3,
  SnmpMode=4
};
enum OperateType
{
  unkown_opt=0,
  mod=1,
  add=2,
  del=3,
  clr=4,
  ack=5,
  reset=6,
  test=7
};
typedef struct
{
  char group_name[16];
}operateLog_t;

int insertOperateLog(int group_id,int object_id,int user_id,int mode,int operate_type,int slot,int port);
int deleteOperateLogById(int id);
int getOptLogId(uchar_t slot);
int insertUnitSettingLog(int object_id,int user_id,int mode,int slot,int port);
int insertSysLog(short log_type_id,char message[]);
int addSysLog(short log_type_id,const char *fmt, ...);

#endif /* _H_LOG */
/******************************************************************************/
