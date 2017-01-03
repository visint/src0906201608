/*
 *  public.h -- Vispace public header
 *
 *  Copyright (c) Visint Inc., 2002-2012. All Rights Reserved.
 *
 *
 */

#ifndef __PUBLIC_H
#define __PUBLIC_H 1

/******************************** Description *********************************/


/********************************* Includes ***********************************/

#include  <typedefs.h>
/********************************** Defines ***********************************/
//#define _DEBUG_ 1
#define BROADCAST_MODE 1
#define IS_TEMP_VERSION_ 1 //this version is for KG 
#define BIWEI_WUXI_VERSION_ 1 //this version is for BiWei
#define SUCCESS 1
#define FAIL 0
#define TRUE 1
#define FAlSE 0

#define SETTABLE_INFOSET 1 //
#define ENABLED_LOG 1

#define DEFAULT_LANG_ 1
#define DEFAULT_LANG_CN 1
#define DEFAULT_SUPPLIER 1
#define DEFAULT_SUPPLIER_NAME "汇信特通信技术有限公司"
#define DEFAULT_SUPPLIER_URL  "http://visint.com.cn"
#ifdef BIWEI_WUXI_VERSION
  #define NMU_FW_VER "BW1.06.08.77"
#else
#ifdef IS_TEMP_VERSION
  #define NMU_FW_VER "KG1.06.08.77"
#else
  #define NMU_FW_VER "1.09.06"
#endif
#endif
#define NMU_HW_VER "1.1.0.2"
//#define WEBS_VERSION "1.3.2"
#define LAST_MODIFY_DATE "2016-07-07"
#define SYS_CONF_FILE "/vispace/sys.conf"
/*
typedef struct
{
  char is_debug_mode;
  char is_print_err;
  char is_save_io_err;
  char slot_to_print;
  int max_err_count;
  int max_syslog_count;
}sysconf_t;
extern sysconf_t sysconf;
*/
typedef struct
{
  char is_add_once;//1=相同位置相同告警只保存一次
  int max_alarm_count;//保存最大告警次数
  int unsigned alarm_count;
}alarmconf_t;
extern alarmconf_t AlarmConf;

//extern char is_debug_mode;
typedef struct
{
  char is_debug_mode;
  char is_print_err;
  char is_save_io_err;
  char slot_to_print;
  int max_err_count;
  int max_syslog_count;
}debug_t;
extern debug_t debug;
#endif /* __PUBLIC_H */

