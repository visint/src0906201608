#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <public.h>
#include <unit.h>
#include <sqlite3.h>
#include <dbutil.h>
#include <log.h>

//gcc -o /usr/local/bin/dbm -lvisipc -lvisdb ../vislib/unit.o ../vislib/visstr.o dbman.c  -I../include
static void showUserData(uchar_t chassis,uchar_t slot)
{
  unit_user_data_t udata;
  if (TRUE!=getUserDataFromDB(chassis,slot,&udata))
    return;
  
}
static void usage()
{
  fprintf(stderr,"DB manager ver 1.0.10\nLast modified:%s\n",LAST_MODIFY_DATE);
  
  printf("Option Specification:\n");
  printf("-b [<1..16>]: show unit base info\n");
  printf("-u [1..16]: show unit user info\n");
  printf("-t table_name: show table\n");
  printf("-m : show info item\n");
  printf("-e <1..16>: show unit status\n");
  printf("-i :assign id\n");
  printf("-a : set alarm\nExmple 1:\n dbman -a -i 31 -d Exmple 2:\n dbman -a -i 31 -p \"2;3;\"\n");
  printf("-v : value(s) to set\n");
  printf("-S modify user data\n");
  printf("-E excute sql\n");
  printf("-U upgrade db\n");
  printf("-C : create DB\n");
  printf("-h for help.\n");
}

static void createUnitBITable(uchar_t classis)
{
  int n;
  sqlite3 *db=NULL;
  char *zErrMsg = 0;
  char sql[] = " CREATE TABLE UnitBI( \
  ID INTEGER PRIMARY KEY, \
  chassis TINYINT, \
  slot TINYINT, \
  property VARCHAR(32), \
  sn VARCHAR(16), \
  model   VARCHAR(32),\
  creation VARCHAR(12),\
  fwver VARCHAR(12),\
  hwver VARCHAR(12)\
  );";

execSql(NULL , "DROP TABLE UnitBI",NULL,NULL);
execSql(NULL , sql,NULL,NULL);


//插入数据
for (n=0;n<=MAX_UNIT_COUNT;n++)
{
 sprintf(sql,"INSERT INTO \"UnitBI\" VALUES(NULL,%d,%d,'OTU111-55-1470-11-%d','SN1--2012-12-%d', 'Model-123-12-%d','2012-12-%d','FV-1.1.1','HV1.1.1');",classis,n,n,n,n,n,n );
 execSql(NULL, sql,NULL,NULL);
}
sprintf(sql,"INSERT INTO \"UnitBI\" VALUES(NULL,%d,16,'NMU111-11-11','SN1--2012-12-%d', 'Model-123-12-%d','2012-12-%d','%s','HV1.1.1');",classis,n,n,n,NMU_FW_VER);
 execSql(NULL, sql,NULL,NULL);
 return;
}
static void createUnitUDTable()
{
  int n;
  char sql[] = " CREATE TABLE UnitUD( \
  ID INTEGER PRIMARY KEY, \
  chassis TINYINT, \
  slot TINYINT, \
  unit_name VARCHAR(32), \
  contact VARCHAR(32), \
  location VARCHAR(32), \
  up_device VARCHAR(32), \
  user_data   VARCHAR(32),\
  service   VARCHAR(32)\
  );";
  execSql(NULL,"DROP TABLE UnitUD;",NULL,NULL);
  execSql(NULL,sql,NULL,NULL);
  //插入数据
 for (n=0;n<=MAX_UNIT_COUNT;n++)
 {
   //sprintf(sql,"INSERT INTO \"UnitUD\" VALUES(NULL,%d,%d,'Unit #%d Unit', \
      'Unit #%d contact', 'Unit #%d location','Unit #%d uplink device','Unit #%d user data','Unit #%d open service');",0,n,n,n,n,n,n,n);
   sprintf(sql,"INSERT INTO \"UnitUD\" VALUES(NULL,%d,%d,'', \
      '', '','','','');",0,n);
   execSql(NULL, sql,NULL,NULL);
 }
 //sprintf(sql,"INSERT INTO \"UnitUD\" VALUES(NULL,%d,16,'NMU', \
      'NMU contact', 'NMU location','NMU uplink device','NMU user data','NMU open service');",0);
   sprintf(sql,"INSERT INTO \"UnitUD\" VALUES(NULL,%d,%d,'', \
      '', '','','','');",0,n);
   execSql(NULL, sql,NULL,NULL);
}

static void insertAlarmType(int alarm_code,char *alarm_name,char *alarm_name_en,char *entity_name,char *entity_name_en,uchar_t severity,char *usedUc,char *clean_alarm_ids)
{
  char sql[360];
  //printf("sql=%s\n",sql);
  sprintf(sql,"INSERT INTO \"AlarmType\" VALUES(%d,'%s','%s','%s','%s',%d,1,0,1,1,'','%s','%s');",alarm_code,alarm_name,alarm_name_en,entity_name,entity_name_en,severity,clean_alarm_ids,usedUc);
  execSql(NULL, sql,NULL,NULL);
 //if (alarm_code==69)
//printf("sql=%s\n",sql);
}
/*
void insertAlarmLog(int alarm_code,char chassis,char slot,char entity)
{
  char sql[200];
  int count;
  
  sprintf(sql,"SELECT alarm_code FROM AlarmType WHERE alarm_code=%d AND enable=1 AND mask_slot NOT LIKE '%c%d;%c' LIMIT 1;",alarm_code,'%',slot,'%');
  
  if (getRowCount(NULL,sql) >0 && slot>=0 && slot <=MAX_UNIT_COUNT)
  {
    sprintf(sql,"INSERT INTO \"AlarmLog\" VALUES(NULL,%d,%d,%d,%d,0,datetime('now'));",alarm_code,chassis,slot,entity);
    execSql(NULL, sql,NULL,NULL);
  }
  printf("sql:%s\n",sql,NULL,NULL);
}
*/

static void insertAlarmReason(int reason_code,char *reason,char *reason_en)
{
  char sql[320];
  //printf("sql=%s\n",sql);
  sprintf(sql,"INSERT INTO \"AlarmReason\" VALUES(%d,'%s','%s');",reason_code,reason,reason_en);
  execSql(NULL, sql,NULL,NULL);
 //if (alarm_code==69)
//printf("sql=%s\n",sql);
}

static void createAlarmReason()
{
  //return;
  char sql[320];
  strcpy(sql," CREATE TABLE AlarmReason(reason_code SMALLINT PRIMARY KEY,reason VARCHAR(32),reason_en VARCHAR(32));");
  execSql(NULL,"DROP TABLE AlarmReason;",NULL,NULL);
  execSql(NULL,sql,NULL,NULL);
  
  insertAlarmReason(0,"","");
  insertAlarmReason(10,"按键设置","Set by key");
  insertAlarmReason(11,"网管设置","Set by NM");
  
  insertAlarmReason(31,"光功率偏低","Power too low");
  insertAlarmReason(32,"无心跳信号","Not heartbeat");
  insertAlarmReason(33,"按键操作旁路","Set to bypass by key");
  insertAlarmReason(34,"软件设置旁路","Set to bypass by NM");
  insertAlarmReason(35,"光功率恢复正常","Power return to normal");
  insertAlarmReason(36,"心跳与光功率正常","Heartbeat & power normal");
  insertAlarmReason(37,"按键操作主路","Set to main line by key");
  insertAlarmReason(38,"软件设置主路","Set to main line by NM");
  insertAlarmReason(40,"按键操作备路","Set to protected line by key");
  
  insertAlarmReason(41,"软件设置备路","Set to protected line by NM");
  insertAlarmReason(100,"同步切换","Synchronous switching");
  /*
  insertAlarmReason(310,"光功率偏低");
  insertAlarmReason(311,"光功率偏低(按键自动)");
  insertAlarmReason(312,"光功率偏低(软件自动)");
  insertAlarmReason(313,"光功率偏低(按键心跳)");
  insertAlarmReason(314,"光功率偏低(软件心跳)");
 
  insertAlarmReason(320,"无心跳信号");
  insertAlarmReason(321,"无心跳信号(按键心跳)");
  insertAlarmReason(324,"无心跳信号(软件心跳)");

  insertAlarmReason(350,"光功率正常");
  insertAlarmReason(351,"光功率正常(按键自动)");
  insertAlarmReason(352,"光功率正常(软件自动)");
  insertAlarmReason(353,"光功率正常(按键心跳)");
  insertAlarmReason(354,"光功率正常(软件心跳)");
  */
}
static void createAlarmTable()
{
  int n;
  char sql[512] = " CREATE TABLE AlarmType( \
  alarm_code SMALLINT PRIMARY KEY, \
  alarm_name VARCHAR(40), \
  alarm_name_en VARCHAR(40), \
  entity_name VARCHAR(16), \
  entity_name_en VARCHAR(16), \
  severity TINYINT,\
  is_send TINYINT DEFAULT 1, \
  mask_slot SMALLINT, \
  usable TINYINT,\
  is_save TINYINT DEFAULT 1, \
  mask_port VARCHAR(16),\
  clean_alarm_ids VARCHAR(16),\
  usedUnitClass VARCHAR(48)\
  );";
  execSql(NULL,"DROP TABLE AlarmType;",NULL,NULL);
  execSql(NULL,sql,NULL,NULL);
    
  insertAlarmType(7,"单元盘修改失败","Unit setting fail","单元盘","UNIT",4,"NMU","");
  insertAlarmType(9,"单元盘修改成功","Unit setting OK","单元盘","UNIT",4,"NMU","");
  insertAlarmType(11,"备光检测设置失败","Protected line setting fail","NMU","NMU",2,"NMU","");
  //insertAlarmType(13,"备光检测设置成功","Back fiber setting fail","NMU","NMU",2,"NMU","");
  insertAlarmType(15,"心跳源IP或报文无效","Heart beat message invlalid","NMU","NMU",2,"NMU","");
  //insertAlarmType(17,"心跳检测设置成功","Heart beat setting fail","NMU","NMU",2,"NMU","");
  //insertAlarmType(13,"备光检测设置成功","Back fiber setting fail","NMU","NMU",2,"NMU","");
  //insertAlarmType(13,"备光检测设置成功","Back fiber setting fail","NMU","NMU",2,"NMU","");

  insertAlarmType(17,"SFP异常","SFP is exception","NMU","NMU",2,"NMU","");
  insertAlarmType(18,"SFP正常","SFP is OK","NMU","NMU",5,"NMU","");
  insertAlarmType(19,"无心跳信号","Heart beat normal","NMU","NMU",2,"NMU","");
  insertAlarmType(20,"有心跳信号","Heart beat exception","NMU","NMU",5,"NMU","");

  insertAlarmType(21,"电源关闭","Power off","电源","Power supply",1,"Chassis","");
  insertAlarmType(22,"电源打开","Power on","电源","Power supply",5,"Chassis","");
  insertAlarmType(23,"电压超出阈值","Volt exceed threhold","电源","Power supply",2,"Chassis","");
  insertAlarmType(24,"电压恢复正常值","Voltage return to normal","电源","Power supply",5,"Chassis","");
  insertAlarmType(25,"风扇停转","Fan not working","风扇","Fan",1,"Chassis","");
  insertAlarmType(26,"风扇运转","Fan working","风扇","Fan",5,"Chassis","");
  insertAlarmType(27,"机箱温度超出阈值","Chassis temp exceed threhold","机箱","Chassis",2,"Chassis","");
  insertAlarmType(28,"机箱温度恢复正常值","Chassis temp return to normal","机箱","Chassis",5,"Chassis","");
  
  insertAlarmType(31,"接收光功率超出阈值","Rx power exceed threhold","光口","Fiber port",2,"OTU OEO","");
  insertAlarmType(32,"接收光功率恢复正常","Rx power return to normal","光口","Fiber port",5,"OTU OEO","31,63,65");
  insertAlarmType(33,"发送光功率超出阈值","Tx power exceed threhold","光口","Fiber port",2,"OTU OEO","");
  insertAlarmType(34,"发送光功率恢复正常","Tx power return to normal","光口","Fiber port",5,"OTU OEO","33,67,69");
  insertAlarmType(35,"偏置电流超出阈值","Bais exceed threhold","光口","Fiber port",3,"OTU OEO","");
  insertAlarmType(36,"偏置电流恢复正常","Bais return to normal","光口","Fiber port",5,"OTU OEO","");
  insertAlarmType(37,"电压超出阈值","Voltage exceed threhold","光口","Fiber port",3,"OTU OEO","");
  insertAlarmType(38,"电压恢复正常","Voltage return to normal","光口","Fiber port",5,"OTU OEO","");
  insertAlarmType(39,"温度超出阈值","Temperature exceed threhold","光口","Fiber port",3,"OTU OEO","");
  insertAlarmType(40,"温度恢复正常","Temperature return to normal","光口","Fiber port",5,"OTU OEO","");

  insertAlarmType(41,"RX LOS","RX LOS","光口","Fiber port",1,"","");
  insertAlarmType(42,"RX LOS恢复","RX LOS OK","光口","Fiber port",5,"","");

  insertAlarmType(51,"单元盘脱位","Unit removed","单元盘","Unit",1,"","");
  insertAlarmType(52,"单元盘在位","Unit inserted","单元盘","Unit",5,"","");
  insertAlarmType(61,"DDM异常","DDM Exception","光口","Fiber port",2,"","");
  insertAlarmType(63,"接收光功率偏低","Rx power relatively low","光口","Fiber port",1,"","");
  insertAlarmType(64,"接收光功率恢复正常","Rx power return to normal","光口","Fiber port",5,"","");
  insertAlarmType(65,"接收光功率偏高","Rx power relatively high","光口","Fiber port",1,"","");
  insertAlarmType(66,"接收光功率恢复正常","Rx power return to normal","光口","Fiber port",5,"","");
  insertAlarmType(67,"发送光功率偏低","Tx power relatively low","光口","Fiber port",1,"","");
  insertAlarmType(68,"发送光功率恢复正常","Tx power return to normal","光口","Fiber port",5,"","");
  insertAlarmType(69,"发送光功率偏高","Tx power relatively high","光口","Fiber port",1,"","");

  insertAlarmType(71,"光模块脱位","Fiber module removed","光口","Fiber port",1,"","");
  insertAlarmType(72,"光模块在位","Fiber module inserted","光口","Fiber port",5,"","");
  insertAlarmType(73,"接收无光","Rx LOS","光口","Fiber port",1,"OTU OEO","");
  insertAlarmType(74,"接收有光","Rx return to normal","光口","Fiber port",5,"OTU OEO","");
  insertAlarmType(75,"发送无光","Tx LOS","光口","Fiber port",1,"OTU OEO","");
  insertAlarmType(76,"发送有光","Tx return to normal","光口","Fiber port",5,"OTU OEO","");
  insertAlarmType(77,"发送关闭","Tx disable","光口","Fiber port",4,"OTU OEO","");
  insertAlarmType(78,"发送开启","Tx enable","光口","Fiber port",5,"OTU OEO","");
  insertAlarmType(79,"A组的接收光功率超出阈值","Group A power exceed threhold","OTAP","Fiber port",2,"OTAP","");
  insertAlarmType(80,"A组的接收光功率恢复正常","Group A power return to normal","OTAP","Fiber port",5,"OTAP","");
  insertAlarmType(81,"B组的接收光功率超出阈值","Group B power exceed threhold","OTAP","Fiber port",2,"OTAP","");
  insertAlarmType(82,"B组的接收光功率恢复正常","Group B power return to normal","OTAP","Fiber port",5,"OTAP","");
  //OSC 告警
  insertAlarmType(89,"接收光功率超出阈值","RX power exceed threhold","OSC","Fiber port",2,"OSC","");
  insertAlarmType(90,"接收光功率恢复正常","RX return to normal","OSC","Fiber port",5,"OSC","");

  insertAlarmType(101,"CPU使用率超出阈值","CPU exceed threhold","NMU","NMU",1,"NMU","");
  insertAlarmType(103,"内存使用率超出阈值","Memory exceed threhold","NMU","NMU",1,"NMU","");
  insertAlarmType(105,"snmpd重启","snmpd restart","NMU","NMU",4,"NMU","");
  insertAlarmType(107,"webs重启","webs restart","NMU","NMU",4,"NMU","");
  insertAlarmType(109,"ccenter重启","ccenter restart","NMU","NMU",4,"NMU","");
  insertAlarmType(111,"heartbeat重启","heartbeat restart","NMU","NMU",4,"NMU","");
  insertAlarmType(113,"olputil重启","olputil restart","NMU","NMU",4,"NMU","");
  insertAlarmType(115,"系统重启","reboot","NMU","NMU",4,"NMU","");

  insertAlarmType(400,"工作模式改变","working mode changed","OLP/OBP","OLP/OBP",4,"OLP/OBP","");

  //OLP 告警
  insertAlarmType(401,"OLP工作模式切换为手动","OLP working mode switch for manual","OLP","OLP",4,"OLP","");
  insertAlarmType(402,"OLP工作模式切换为自动","OLP working mode switch for auto","OLP","OLP",5,"OLP","");
  insertAlarmType(403,"OLP切换到备路(L2)","OLP switches to L2","OLP","OLP",3,"OLP","");
  insertAlarmType(404,"OLP切换到主路(L1)","OLP switches to L1","OLP","OLP",5,"OLP","");
  insertAlarmType(409,"OLP R1(主纤)光功率偏低","OLP R1 power relatively low","OLP","OLP",2,"OLP","");
  insertAlarmType(410,"OLP R1(主纤)光功率恢复正常","OLP R1 power return to normal","OLP","OLP",5,"OLP","");
  insertAlarmType(411,"OLP R2(备纤)光功率偏低","OLP R2 power relatively low","OLP","OLP",2,"OLP","");
  insertAlarmType(412,"OLP R2(备纤)光功率恢复正常","OLP R2 power return to normal","OLP","OLP",5,"OLP ","");
  insertAlarmType(413,"OLP TX光功率偏低","OLP TX power relatively low","OLP","OLP",2,"OLP","");
  insertAlarmType(414,"OLP TX光功率恢复正常","OLP TX power return to normal","OLP","OLP",5,"OLP","");

//OBP 告警
  insertAlarmType(501,"OBP 光开关切换到旁路","OBP switches to bypass line","OBP","OBP",3,"OBP","");
  insertAlarmType(502,"OBP 光开关切换到主路","OBP switches to main line","OBP","OBP",5,"OBP","");
  insertAlarmType(503,"OBP R1光功率偏低","OBP R1 power relatively low","OBP","OBP",2,"OBP","");
  insertAlarmType(504,"OBP R1光功率恢复正常","OBP R1 power return to normal","OBP","OBP",5,"OBP","");
  insertAlarmType(505,"OBP R2光功率偏低","OBP R2 power relatively low","OBP","OBP",2,"OBP","");
  insertAlarmType(506,"OBP R2光功率恢复正常","OBP  R2 power return to normal","OBP","OBP",5," OBP","");
  insertAlarmType(507,"OBP R3光功率偏低","OBP R3 power relatively low","OBP","OBP",2,"OBP","");
  insertAlarmType(508,"OBP R3光功率恢复正常","OBP R3 power return to normal","OBP","OBP",5,"OBP","");
  insertAlarmType(509,"OBP R4光功率偏低","OBP R4 power relatively low","OBP","OBP",2,"OBP","");
  insertAlarmType(510,"OBP R4光功率恢复正常","OBP R4 power return to normal","OBP","OBP",5,"OBP","");
  /*
  insertAlarmType(421,"OSS工作模式切换为手动","OSS mode switch to manual","OSS","OSS",4);
  insertAlarmType(422,"OSS工作模式切换为自动","OSS mode switch to auto","OSS","OSS",5);
  insertAlarmType(423,"OSS切换到备路(L2)","OSS mode switch to L2","OSS","OSS",3);
  insertAlarmType(424,"OSS切换到主路(L1)","OSS mode switch to L1","OSS","OSS",5);
  insertAlarmType(425,"OSS S1光功率偏低","Tx power relatively low","OSS","OSS",2);
  insertAlarmType(426,"OSS S1光功率恢复正常","Tx power return to normal","OSS","OSS",5);
  insertAlarmType(427,"OSS S2光功率偏低","S2 power relatively low","OSS","OSS",2);
  insertAlarmType(428,"OSS S2光功率恢复正常","S2 power return to normal","OSS","OSS",5);
  insertAlarmType(429,"OSS CH1光功率偏低","CH2 power relatively low","OSS","OSS",2);
  insertAlarmType(430,"OSS CH1光功率恢复正常","CH2 power return to normal","OSS","OSS",5);
  insertAlarmType(431,"OSS CH2光功率偏低","CH2 power relatively low","OSS","OSS",2);
  insertAlarmType(432,"OSS CH2光功率恢复正常","CH2 power return to normal","OSS","OSS",5,"");
  */

  insertAlarmType(421,"OSS CH1 切换到R1-2","OSS CH1 switch to R1-2","CH","CH",3,"OSS","");
  insertAlarmType(422,"OSS CH1 切换到R1-1","OSS CH1 switch to R1-1","CH","CH",5,"OSS","");
  insertAlarmType(423,"OSS CH2 切换到R2-2","OSS CH2 switch to R2-2","CH","CH",3,"OSS","");
  insertAlarmType(424,"OSS CH2 切换到R2-1","OSS CH2 switch to R2-1","CH","CH",5,"OSS","");
  insertAlarmType(425,"OSS CH1 光功率偏低","OSS CH1 power relatively low","CH","CH",2,"OSS","");
  insertAlarmType(426,"OSS CH1 光功率恢复正常","OSS CH1 power return to normal","CH","CH",5,"OSS","");
  insertAlarmType(427,"OSS CH2 光功率偏低","OSS CH2 power relatively low","CH","CH",2,"OSS","");
  insertAlarmType(428,"OSS CH2 光功率恢复正常","OSS CH2 power return to normal","CH","CH",5,"OSS","");

  //DVOA 告警
  insertAlarmType(431,"锁不住","Unlock","DVOA","DVOA",1,"DVOA","");
  insertAlarmType(432,"锁住","Locked","DVOA","DVOA",5,"DVOA","");
  //ETU 告警
  insertAlarmType(451,"链接中断","Link down","ETU 电口","Ethernet port",1,"ETU","");
  insertAlarmType(452,"链接","Link up","ETU 电口","Ethernet port",5,"ETU","");
  insertAlarmType(453,"链接中断","Link down","ETU 光口","Fiber port",1,"ETU","");
  insertAlarmType(454,"链接","Link up","ETU 光口","Fiber port",5,"ETU","");

  //EDFA告警
  insertAlarmType(901,"EDFA输入光功超出阈值","Input power exceed threhold","EDFA","EDFA",2,"EDFA","");
  insertAlarmType(902,"EDFA输入光功率恢复正常","Input power return to normal","EDFA","EDFA",5,"EDFA","");
  insertAlarmType(903,"EDFA输出光功超出阈值","Output power exceed threhold","EDFA","EDFA",2,"EDFA","");
  insertAlarmType(904,"EDFA输出光功率恢复正常","Output power return to normal","EDFA","EDFA",5,"EDFA","");
  insertAlarmType(905,"EDFA模块温度超出阈值","Module temperature exceed threhold","EDFA","EDFA",2,"EDFA","");
  insertAlarmType(906,"EDFA模块温度恢复正常","Module temperature return to normal","EDFA","EDFA",5,"EDFA","");
  insertAlarmType(907,"EDFA泵浦温度超出阈值","Pump temperature exceed threhold","泵浦","Pump",2,"EDFA","");
  insertAlarmType(908,"EDFA泵浦温度恢复正常","Pump temperature return to normal","泵浦","Pump",5,"EDFA","");
  insertAlarmType(909,"EDFA泵浦电流超出阈值","Pump current exceed threhold","泵浦","Pump",2,"EDFA","");
  insertAlarmType(910,"EDFA泵浦电流恢复正常","Pump current return to normal","泵浦","Pump",5,"EDFA","");


  strcpy(sql," CREATE TABLE AlarmLog( \
  ID INTEGER PRIMARY KEY, \
  alarm_code INTEGER,\
  chassis TINYINT, \
  slot TINYINT, \
  entity TINYINT, \
  alarm_status TINYINT, \
  alarm_time  DATETIME DEFAULT (datetime(CURRENT_TIMESTAMP,'localtime')), \
  alarm_value VARCHAR(36),\
  last_alarm_time  DATETIME DEFAULT (datetime(CURRENT_TIMESTAMP,'localtime')),\
  clean_time  DATETIME,\
  ack_time  DATETIME,\
  alarm_count INTEGER,\
  port_name VARCHAR(8),\
  reason_code INTEGER);");
  execSql(NULL,"DROP TABLE AlarmLog;",NULL,NULL);
  execSql(NULL,sql,NULL,NULL);
/*
strcpy(sql," CREATE TABLE Alarms( \
  ID INTEGER PRIMARY KEY, \
  alarm_code INTEGER,\
  chassis TINYINT, \
  slot TINYINT, \
  entity TINYINT, \
  alarm_status TINYINT, \
  alarm_time  DATETIME DEFAULT (datetime(CURRENT_TIMESTAMP,'localtime')), \
  alarm_value VARCHAR(12),\
  clean_time  DATETIME,\
  ack_time  DATETIME\
  );");
  execSql(NULL,"DROP TABLE Alarms;",NULL,NULL);
  */
  //execSql(NULL,sql,NULL,NULL);

  /*
  insertAlarmLog(1,0,0,1);
  insertAlarmLog(3,0,0,2);
  insertAlarmLog(11,0,1,0);
  insertAlarmLog(21,0,1,1);
  insertAlarmLog(23,0,1,2);
  
  insertAlarmLog(61,0,12,0);
  insertAlarmLog(63,0,8,0);
  */
}

static void createSnmpTable()
{
  char sql[250] = " CREATE TABLE Community( \
  ID INTEGER PRIMARY KEY, \
  rocommunity VARCHAR(16), \
  rwcommunity VARCHAR(16), \
  ipadd VARCHAR(24), \
  port INTEGER ,\
  read_only TINYINT,\
  enable TINYINT\
  );";
  execSql(NULL,"DROP TABLE Community;",NULL,NULL);
  execSql(NULL,sql,NULL,NULL);
  execSql(NULL,"INSERT INTO Community VALUES(NULL,\"public\",\"private\",\"\",161,2,1)",NULL,NULL);
  execSql(NULL,"DROP TABLE DestHost;",NULL,NULL);
  execSql(NULL,"CREATE TABLE DestHost( \
  ID INTEGER PRIMARY KEY, \
  dest_ipadd VARCHAR(24), \
  udp_port INTEGER,\
  enable TINYINT,targetname VARCHAR(36));",NULL,NULL);
  execSql(NULL,"INSERT INTO DestHost VALUES(NULL,\"192.168.1.238\",9162,1,\"Host 238\")",NULL,NULL);
  execSql(NULL,"DROP TABLE AgentAddr;",NULL,NULL);
  execSql(NULL,"DROP TABLE KeepAlive;",NULL,NULL);
  execSql(NULL,"CREATE TABLE KeepAlive( \
  ID INTEGER PRIMARY KEY, \
  dest_ipadd VARCHAR(24), \
  udp_port INTEGER,\
  enable TINYINT,targetname VARCHAR(36),Interval INTEGER);",NULL,NULL);
  execSql(NULL,"CREATE TABLE AgentAddr( \
  ID INTEGER PRIMARY KEY, \
  agent_ipadd VARCHAR(24), \
  udp_port INTEGER,\
  udp6_port INTEGER,\
  enable TINYINT);",NULL,NULL);
}

static void insertOptObjGroup(int group_id,char *group_name,char *group_name_en,char enable)
{
  char sql[200];
  
  sprintf(sql,"INSERT INTO \"OptObjGroup\" VALUES(%d,'%s','%s',%d);",group_id,group_name,group_name_en,enable);
  execSql(NULL, sql,NULL,NULL);
}

static void insertErrLog(int err_code,char *err_info)
{
  char sql[200];
  
  sprintf(sql,"INSERT INTO \"ErrLog\" VALUES(%d,'%s',0);",err_code,err_info);
  execSql(NULL, sql,NULL,NULL);
}
static void insertOptObj(int group_id,int object_code,char *object_name,char *object_name_en)
{
  char sql[100];
  
  sprintf(sql,"INSERT INTO \"OptObj\" VALUES(NULL,%d,%d,'%s','%s');",group_id,object_code,object_name,object_name_en);
  execSql(NULL, sql,NULL,NULL);
}

static void createSysLogTable()
{
  char sql[]=" CREATE TABLE SysLog( \
  ID INTEGER PRIMARY KEY, \
  msg VARCHAR(128), \
  occured_time  DATETIME DEFAULT (datetime(CURRENT_TIMESTAMP,'localtime')) \
  );";
  execSql(NULL,"DROP TABLE SysLog",NULL,NULL);
  execSql(NULL,sql,NULL,NULL);
}
static void createLogTable()
{
  int id;
  execSql(NULL,"DROP TABLE OptObj;",NULL,NULL);
  execSql(NULL,"DROP TABLE OptLog;",NULL,NULL);
  execSql(NULL,"DROP TABLE OptObjGroup;",NULL,NULL);
  execSql(NULL," CREATE TABLE OptLog( \
  ID INTEGER PRIMARY KEY, \
  group_id INTEGER,\
  object_id INTEGER,\
  user_id TINYINT,\
  mode TINYINT,\
  operate_type TINYINT,\
  operate_time  DATETIME DEFAULT (datetime(CURRENT_TIMESTAMP,'localtime')),\
  slot TINYINT,\
  port TINYINT\
  );",NULL,NULL);
 //execSql(NULL,"DROP TABLE OptObjGroup",NULL,NULL);
  execSql(NULL,"CREATE TABLE OptObjGroup( \
  ID INTEGER PRIMARY KEY, \
  group_name VARCHAR(18),\
  group_name_en VARCHAR(22),\
  enable TINYINT \
  );",NULL,NULL);

  execSql(NULL,"CREATE TABLE OptObj( \
  ID INTEGER PRIMARY KEY, \
  group_id INTEGER,\
  object_code TINYINT,\
  object_name VARCHAR(16), object_name_en VARCHAR(20));",NULL,NULL);
  
  execSql(NULL,"DROP TABLE ErrLog",NULL,NULL);
  execSql(NULL,"CREATE TABLE ErrLog( \
  err_code SMALLINT PRIMARY KEY, \
  err_info VARCHAR(32),\
  err_count INTEGER);",NULL,NULL);

  /*
  execSql(NULL,"DROP TABLE SysLog",NULL,NULL);
  execSql(NULL,"CREATE TABLE SysLog( \
  ID SMALLINT PRIMARY KEY, \
  message VARCHAR(128),\
  log_time  DATETIME DEFAULT (datetime(CURRENT_TIMESTAMP,'localtime'));",NULL,NULL);
  */
  /*
  insertErrLog(1,"io get");
  insertErrLog(2,"io set");
  insertErrLog(3,"sys restart");
  */
  insertOptObjGroup(LOG_GROUP_SYS_IP_CONF,"IP地址配置","IP Config",1);
  
  id=LOG_GROUP_SYS_SYS_INFO_CONF;
  if (LOG_GROUP_SYS_SYS_INFO_CONF==2)
  printf("LOG_GROUP_SYS_SYS_INFO_CONF:%d id =%d\n",LOG_GROUP_SYS_SYS_INFO_CONF,id);
  else
   printf("LOG_GROUP_SYS_SYS_INFO_CONF!=2 id =%d\n",id);

  insertOptObjGroup(id,"系统信息","System info",1);
  insertOptObjGroup(LOG_GROUP_SYS_USER_CONF,"用户配置","User configure",1);
  insertOptObjGroup(LOG_GROUP_SYS_PASSWORD_CONF,"用户密码","User password",1);
  insertOptObjGroup(LOG_GROUP_SYS_USER_ACL_CONF,"用户访问控制","User access limit",1);
  insertOptObjGroup(LOG_GROUP_SYS_USER_GROUP_CONF,"用户组管理","User group management",1);
  insertOptObjGroup(LOG_GROUP_SYS_SAVE_USER_CONF,"保存用户配置","Save user",1);

  insertOptObjGroup(LOG_GROUP_SYS_SNMP_COMMINITY_CONF,"SNMP 配置","SNMP config",1);
  insertOptObjGroup(LOG_GROUP_SYS_SNMP_DEST_HOST_CONF,"告警目标主机","Alarm host",1);
  insertOptObjGroup(LOG_GROUP_SYS_CURRENT_ALARM_CONF,"当前告警","Current alarm",1);
  insertOptObjGroup(LOG_GROUP_SYS_HISTORY_ALARM_CONF,"历史告警","HIstory alarm",2);
  insertOptObjGroup(LOG_GROUP_SYS_SNMP_ALARM_TYPE_CONF,"告警类型","Alarm type",2);
  insertOptObjGroup(LOG_GROUP_SYS_OPT_LOG_CONF,"操作日志","Log",2);

  //insertOptObjGroup(LOG_UNIT_CONF,"单元盘","UNIT",1);
  insertOptObjGroup(LOG_UNIT_OTU_CONF,"OTU信息","OTU",1);
  insertOptObjGroup(LOG_UNIT_OEO_CONF,"OEO信息","OEO",1);
  insertOptObjGroup(LOG_UNIT_OLP_CONF,"OLP信息","OLP",1);
  insertOptObjGroup(LOG_UNIT_OBP_CONF,"OBP信息","OLP",1);
  insertOptObjGroup(LOG_UNIT_OSS_CONF,"OSS信息","OSS",1);
  insertOptObjGroup(LOG_UNIT_OTAP_CONF,"OTAP信息","OSS",1);
  insertOptObjGroup(LOG_UNIT_DVOA_CONF,"DVOA信息","DVOA",1);
  insertOptObjGroup(LOG_UNIT_EDFA_CONF,"EDFA信息","EDFA",1);
  insertOptObjGroup(LOG_DDM_THRESHOLD_HCONF,"DDM告警阈值","DDM",2);
  

  insertOptObjGroup(LOG_OLP_WD_OK,"OLP探测正常","OLP Detect OK",2);
  insertOptObjGroup(LOG_OLP_WD_FAIL,"OLP探测异常","OLP Detect Fail",2);
  insertOptObjGroup(LOG_HEARTBEAT_WD_OK,"心跳正常","Heartbeat OK",2);
  insertOptObjGroup(LOG_HEARTBEAT_WD_FAIL,"心跳异常","Heartbeat Fail",2);
  /*
  insertOptObjGroup(LOG_SNMP_KILLED,"SNMP 宕死","SNMP killed",2);
  insertOptObjGroup(LOG_SNMP_TIMEOUT,"SNMP 超时","SNMP Timeout",2);
  insertOptObjGroup(LOG_SNMP_EXIT,"SNMP退出","Self Ping fail",2);
  insertOptObjGroup(LOG_SELF_PING_FAIL,"自Ping失败","Self Ping fail",2);
  */
  createSysLogTable();
}


static boolean_t insertInfoSet(uchar_t infoset_id,char infoset_type,const char *iset_name,
                             char *include_class,char *exclude_type,char item_count)
{
  int n;
  char sql[160];
  sprintf(sql,"SELECT ID FROM InfoSet WHERE infoset_id=%d AND include_class like '%c%s%c'",infoset_id,'%',include_class,'%');
  
  n=getRowCount(NULL,sql);
  printf("count=%d sql=%s\n",n,sql);
  if (n>0)
  {
    printf("InfoSet exist\n");
    return FALSE;
  }
  //printf("count=%d sql=%s\n",n,sql);
  //return TRUE;
  sprintf(sql,"INSERT INTO \"InfoSet\" VALUES(NULL,'%d','%d','%s','%s','%s',%d);"
          ,infoset_id,infoset_type,include_class,exclude_type,iset_name,item_count);
  printf("count=%d sql=%s\n",n,sql);
  execSql(NULL,sql,NULL,NULL);
  return TRUE;
}
static boolean_t createInfoSetTable()
{
  char sql[] = " CREATE TABLE InfoSet( \
  ID INTEGER PRIMARY KEY, \
  infoset_id SMALLINT, \
  infoset_type TINYINT, \
  include_class VARCHAR(12), \
  exclude_type  VARCHAR(12), \
  infoset_name VARCHAR(20), \
  item_count INT1 \
  );";
  execSql(NULL,"DROP TABLE InfoSet;",NULL,NULL);
  execSql(NULL,sql,NULL,NULL);
  insertInfoSet(1,3,"单元盘基本信息","0","",3);
  insertInfoSet(2,3,"单元盘用户信息","0","",6);
  insertInfoSet(3,1,"系统信息","1","",6); //电源、风扇状态信息
  insertInfoSet(4,2,"温度、电压告警阈值","1","",8);
  insertInfoSet(11,2,"光口%sDDM诊断信号","1;3;","1:3;",5);/*支持的单元盘:收发器、OTU*,但不支持IP 113收发器*/

  insertInfoSet(OLP_STATUS_INFOSET_ID,1,"OLP(1+1)状态信息","4;","4:2;",4); /*1+1 OLP*/
  insertInfoSet(OLP_STATUS_INFOSET_ID,1,"OLP(1:1)状态信息","4;","4:1;",2); /*1:1 OLP*/
  insertInfoSet(OLP_DATA_INFOSET_ID,2,"OLP(1+1)光功率信息","4;","4:2;",7);  /*1+1 OLP*/
  insertInfoSet(OLP_DATA_INFOSET_ID,2,"OLP(1:1)光功率信息","4;","4:1;",6);  /*1:1 OLP*/

  insertInfoSet(OPORT_INFOSET_ID,1,"光口%s状态","1;3;","",4);      /*支持的单元盘:收发器、OTU*/
  insertInfoSet(22,1,"收发器电口信息","1;","",5);
  return TRUE;
}

static boolean_t insertInfoItem(uchar_t infoset_id,uchar_t unit_class,uchar_t unit_type,char item_id,
                 const char *item_name,const char *status_name,char *exclude_type,char readonly)
{
  int n;
  char sql[120];
 
  char value[16]="";
  /*column_t col;

  col.max_row=2;
  col.col_size=16;
  col.cols=(char *)cols;
  col.row=0;
  */
  //sprintf(sql,"SELECT ID FROM InfoSet WHERE infoset_id=%d AND unit_class=%d AND unit_type=%d",infoset_id,unit_class,unit_type);
  sprintf(sql,"SELECT ID FROM InfoSet WHERE infoset_id=%d AND include_class LIKE '%c%d;%c' AND NOT(exclude_type LIKE '%c%d:c%d;%c') LIMIT 1;",infoset_id,'%',unit_class,'%','%',unit_class,unit_type,'%');
  //n=get_col(NULL,sql,&col);
  getColValue(NULL,value);
  if (strlen(value)<1)
  {
    printf("InfoSet %d Not exist\nsql:%s\n",infoset_id,sql);
    return FALSE;
  }
  sprintf(sql,"INSERT INTO \"InfoItem\" VALUES(NULL,%s,%d,'%s','%s','%s',%d);"
          ,value,item_id,item_name,status_name,exclude_type,readonly);
  execSql(NULL,sql,NULL,NULL);
  return TRUE;
}
static boolean_t createInfoItemTable()
{
  //int n;
  sqlite3 *db=NULL;
  //char *zErrMsg = 0;
  int rc;
  char sql[] = " CREATE TABLE InfoItem( \
  ID INTEGER PRIMARY KEY, \
  PID INTEGER, \
  item_id SMALLINT, \
  item_name VARCHAR(20), \
  status_name VARCHAR(40), \
  exclude_type VARCHAR(8), \
  readonly INT1 \
  );";
  execSql(NULL,"DROP TABLE InfoItem;",NULL,NULL);
  execSql(NULL,sql,NULL,NULL);
  insertInfoItem(21,3,1,1,"接收","1=有光;2=无光","0",0);
  insertInfoItem(21,3,1,2,"发送","1=有光;2=无光","0",0);
  insertInfoItem(21,3,1,2,"发送关断","1=启用;2=禁用","0",0);
  insertInfoItem(21,3,1,3,"LOS","1=有信号;2=无信号","0",0);

  /*insertInfoItem(11,3,1,2,"接收光功率","1=无光;2=有光","0",0);
  insertInfoItem(11,3,1,2,"发送光功率","1=无光;2=有光","0",0);
  insertInfoItem(11,3,1,2,"电压","1=无光;2=有光","0",0);
  insertInfoItem(11,3,1,2,"电压","1=无光;2=有光","0",0);
  */
 return TRUE;
}
static int onFetchColValue( void *para, int n_column, char **column_value, char **column_name)
{
  if (strcmp(column_value[0],"0"))
    printf("Unit #%s",column_value[0]);
    printf("\t");
  if (strcmp(column_value[1],"0"))
    printf(" %s %s",column_value[5],column_value[1]);
    printf("\t");
  printf("%s\t",column_value[4]);
  if (strcmp(column_value[3],"1"))
     printf("unclean unack\t");
  else if (strcmp(column_value[3],"2"))
     printf("unclean ack\t");
  printf("%s\n",column_value[2]);
  return 0;
}
void showAlarm()
{
  sqlite3 *db;
  char *zErrMsg = 0;
  int rc;
  char sql[]="SELECT B.slot,B.entity,B.alarm_time, B.alarm_status,A.alarm_name,A.entity_name FROM AlarmType AS A,AlarmLog AS B WHERE A.alarm_code=B.alarm_code";
  rc = sqlite3_open(DB_FILE_NAME, &db);  
  
  rc = sqlite3_exec(db, sql, onFetchColValue, NULL, &zErrMsg); 
  printf("zErrMsg:%s\n",zErrMsg);
  sqlite3_close(db);
}
static int onFetchTable( void *para, int n_column, char **column_value, char **column_name)
{
  int n;
  for (n=0;n<n_column;n++)
  printf("%s:%s\n",column_name[n],column_value[n]);
  return 0;
}
/*
void showTable(char * table_name)
{
  sqlite3 *db=NULL;
  char *zErrMsg = 0;
  int rc;
  int i,j;
  int index=0;
  char cols_width[12]={5,5,12,16,16,12,12,12,5,10,10,10};
  int nrow = 0, ncol = 0;
  char **dbResult;
  char sql[100];
  rc = sqlite3_open(DB_FILE_NAME, &db); 
  if( rc )
  {
   fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
   sqlite3_close(db);
   return;
  }
  sprintf(sql,"SELECT * FROM %s",table_name);
  rc = sqlite3_exec(db, sql,  onFetchTable, NULL, &zErrMsg); 
  if (rc != SQLITE_OK)
  {
    printf("zErrMsg:%s\n",zErrMsg);
    //printTable("满足条件的信息项","table_name",dbResult,dbResult+ncol,nrow,ncol,cols_width);
  }
 sqlite3_close(db);
}
*/
static int onFetchInfoset( void *para, int n_column, char **column_value, char **column_name)
{
  int n;
  for (n=0;n<n_column;n++)
    printf("%s:%s\n",column_name[n],column_value[n]);
  return 0;
}
void showInfoset(int uclass,int utype)
{
  sqlite3 *db=NULL;
  char *zErrMsg = 0;
  int rc;

  char sql[100];
  rc = sqlite3_open(DB_FILE_NAME, &db); 
  if( rc )
  {
   fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
   sqlite3_close(db);
   return;
  }
  sprintf(sql,"SELECT * FROM InfoSet WHERE include_class like '%c%d;%c' AND exclude_type NOT LIKE '%c%d:%d;%c';",'%',uclass,'%','%',uclass,utype,'%');
  printf("sql:%s\n",sql);
  rc = sqlite3_exec(db, sql,  onFetchInfoset, NULL, &zErrMsg); 
  //printf("zErrMsg:%s\n",zErrMsg);
  sqlite3_close(db);
}

void UpgradeDB()
{
  //char sql[200];alarm_count INTEGER
  //strcpy(sql,"alter TABLE AlarmLog add col clean_time  DATETIME");
  execSql(NULL,"alter TABLE AlarmLog add last_alarm_time  DATETIME",NULL,NULL);
  execSql(NULL,"alter TABLE AlarmLog add clean_time  DATETIME",NULL,NULL);
  execSql(NULL,"alter TABLE AlarmLog add ack_time  DATETIME",NULL,NULL);
  execSql(NULL,"alter TABLE AlarmLog add alarm_count INTEGER DEFAULT 1",NULL,NULL);

  execSql(NULL,"alter TABLE AlarmType add is_save TINYINT DEFAULT 1",NULL,NULL);
  execSql(NULL,"alter TABLE KeepAlive add Interval INTEGER DEFAULT 20",NULL,NULL);
}

void createDB()
{
  createAlarmTable();
  
  createAlarmReason();
  createUnitBITable(0);
  createUnitUDTable();
  createInfoSetTable();
  createInfoItemTable();
  createSnmpTable();
  createLogTable();
  
}

int dbman(int argc, char **argv)
{
    int oc,chassis=0,slot=0,infoset_id=0,item_id=0,item_value=0;                    
    char opt=0,rw=-1,field_value[32]="",*b_opt_arg;           
    char value[128]="";
    char buf[128]="";
    char ports[16]="";
    int  val=0;
    char enable=1;
    debug.is_debug_mode=1;
    while((oc = getopt(argc, argv, "abc:df:i:p:n:s:t:uv:ACE:I:GSU")) != -1)
    {
        switch(oc)
        {
            case 'a':
                opt='a';
                break;
            case 'c':
                chassis=atoi(optarg);
                printf("Chassis is %s.\n",optarg);
                break;
            case 'n':
                b_opt_arg = optarg;
                strncpy(field_value, optarg,30);
                printf("Unit name is %s\n", optarg);
                break;
            case 'd':
                enable=0;
                debug.is_debug_mode=1;
                break;
            case 'p':
                strcpy(ports,optarg);
                //port=atoi(optarg);
                printf("ports is %s.\n",optarg);
                break;
            case 's':
                b_opt_arg = optarg;
                slot=atoi(optarg);
                printf("Unit is #%s\n", optarg);
                break;
            case 'f':
                b_opt_arg = optarg;
                infoset_id=atoi(optarg);
                printf("infoset id is %s\n", optarg);
                break;
            case 'i':
                b_opt_arg = optarg;
                item_id=atoi(optarg);
                //printf("Unit data item id is %s\n", optarg);
                break;
          case 'u':
                printf("Set user data.\n");
                opt='u';
                break;
          case  'v':
                 strcpy(value,optarg);
          case 'G':
                printf("Get user data.\n");
                rw='G';
                break;
          case 'S':
                printf("Set user data.\n");
                rw='S';
                break;
          case 'E':
                printf("Excute SQL:%s.\n",optarg);
                execSql(NULL , optarg,NULL,NULL);
                return;
          case 'C':
                printf("Create DB.\n");
                createDB();
                return 0;
          case 'U':
                printf("Upgrade DB.\n");
                UpgradeDB();
                return 0;
          case 't':
                printf("Show table %s.\n",optarg);
                showTable(NULL,optarg);
                return 0;
          case 'A':
                printf("Show Alarm.\n");
                showAlarm();
                return 0;
         case 'I':
                printf("Show Infoset.\n");
                showInfoset(atoi(optarg),1);
                return 0;
          case '?':
                //ec = (char)optopt;
                printf("无效的选项字符 \' %c \'!\n", (char)optopt);
                usage();
                return 0;
          case ':':
                printf("缺少选项参数！\n");
                usage();
                return 0;

        }
    }
  printf("opt is %c rw=%c\n", opt,rw);
  switch(opt)
  {
     case 'u':
         if (rw=='G')
         {
           printf("Get user data.\n");
         }
         else if (rw=='S')
         {
            if (FALSE!=setUserDataItemToDB(chassis,slot,item_id,field_value))
              printf("Set user data OK.\n");
         }
         else
             usage();
     break;
     case 'a':
              //dbm -a -s 4 -f 21 -i 1
             //insertAlarmLog(infoset_id,chassis,slot,item_id,0);
           val=atoi(value);
           if (val<0 || val>2)
           {
             printf("The value must be 0 or 2");
             return 0;
           }
           else
           {
             char sql[128];
             strcpy(sql,"UPDATE AlarmType set");
             if (val>0 && val<3)
                 sprintf(sql,"%s usable=%d",sql,val);
             else
                 sprintf(sql,"%s usable=1,mask_port='%s'",sql,ports);
 
             if (item_id>0)
              sprintf(sql,"%s WHERE alarm_code=%d",sql,item_id);
             printf("sql:%s\n",sql);
             execSql(NULL , sql,NULL,NULL);
           }
           break;
     default:
             usage();
             return 0;
  }
   // printf("opterr=%d\n",opterr);
   return 0;

}

int main(int argc, char **argv)
{
  /*int n,ac;
  char *av[10];
  char **pav;
  char *delim=" ";
  char opt_str[]="dbm -i 1=2;2=1 -G -s 1 -u";
  char opt[10][32];
  
  for (n=0;n<10;n++)
   av[n]=opt[n];
  
  pav=av;
  ac=optsep(opt_str,delim,pav,10);
  
  printf("argc=%d %s %s\n",ac,pav[0],pav[1]);
  dbman(ac,pav);
*/
  debug.is_debug_mode=1;
  dbman(argc,argv);
}

/*
int main1(int argc, char **argv)
{
  result_t result;
  char   col_name[10][16]={ "","", "","", "","","", "",""};
  uchar_t cols_width[12]={5,22,22,16,16,16,12,6};
  char   dataset[FIELD_MAX_COUNT][FIELD_MAX_SIZE]={ "","", "","", "","","", "",""};
  char sql[200];
  int n;

  char key[10][10];
  char value[10][20];
  char kv[]="1=link;2=down";
  getKeyValue(kv,";","=",(char*)key,10,(char*)value,20,5);
 
  result.col_name=col_name;
  result.fields=FIELD_MAX_COUNT;
  result.results=dataset;
  result.max_row=16;
  result.row=0;
  result.col=0;

  
  if (argc<2)
  {
    usage();
    return 0;
  }
  if (!strcmp(argv[1],"-t"))
  {
     sql_query(NULL,"SELECT * FROM InfoSet",&result);
      printArrTable("信息集列表",(char*)col_name,16,(char*)dataset,FIELD_MAX_SIZE,result.row,result.col,cols_width);
 return 1;
  }
  else if (!strcmp(argv[1],"-c"))
  {
    createDB();
    return;
  }
  else if (!strcmp(argv[1],"-i"))
  {
    status_infoset_t infoset;
    infoset.infoset_id=21;

    infoset.infoitems[1].item_id=1;
    infoset.infoitems[2].item_id=2;
    infoset.infoitems[1].value=1;
    infoset.infoitems[2].value=2;
    status_infoset_ex_t iset_ex;
    iset_ex.unit_class=3;
    iset_ex.unit_type=1;
    iset_ex.infoset=&infoset;

    mapStatusInfoSet(0,1,&iset_ex);
    printf("%s %s\n",iset_ex.item_name[1],iset_ex.status_name[1]);
    return;
  }
  else if (!strcmp(argv[1],"-m"))
  {
     sql_query(NULL,"SELECT * FROM InfoItem",&result);
      printArrTable("信息项列表",(char*)col_name,16,(char*)dataset,FIELD_MAX_SIZE,result.row,result.col,cols_width);
 return 1;
  }
  else if (!strcmp(argv[1],"-e"))
  {
    if (argc<3)
    {
      usage();
      return 0;
    }
    status_infoset_t infoset;
    infoset.infoitems[0].item_id=1;
    infoset.infoitems[1].item_id=2;
    infoset.infoitems[0].value=1;
    infoset.infoitems[1].value=2;
    infoset.infoset_id=21;

    // sql_query(NULL,"SELECT * FROM ItemStatus",&result);
    //  printArrTable("信息状态表",(char*)col_name,16,(char*)dataset,FIELD_MAX_SIZE,result.row,result.col,cols_width);
   showStatusInfoSet(0,atoi(argv[2]),infoset);
   return 1;
  }
  else if (!strcmp(argv[1],"-u"))
  {
     sql_query(NULL,"SELECT * FROM UnitUD",&result);
      printArrTable("信用户数据表",(char*)col_name,16,(char*)dataset,FIELD_MAX_SIZE,result.row,result.col,cols_width);
   return 1;
  }
  else if (!strcmp(argv[1],"-b"))
  {
    if (argc<3)
    {
      sql_query(NULL,"SELECT slot,property,sn,model,creation,fwver,hwver FROM UnitBI Limit 16",&result);
      printArrTable("单元盘基本信息列表",(char*)col_name,16,(char*)dataset,FIELD_MAX_SIZE,result.row,result.col,cols_width);
 return 1;
    }
    else
    {
      n=atoi(argv[1]);
      sprintf(sql,"SELECT slot,property,sn,model,creation,fwver,hwver FROM UnitBI WHERE slot=%s ",argv[2]);
      sql_query(NULL,sql,&result);
      //printf("result.row=%d col=%d\n",result.row,result.col);
      printArray("单元盘基本信息",(char*)col_name,16,(char*)dataset,FIELD_MAX_SIZE,result.row,result.col);
      return 1;
    }
  }
}
*/
