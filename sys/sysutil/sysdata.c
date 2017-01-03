#include <stdio.h>
#include <string.h>
#include <signal.h>

#include <unistd.h>   
#include <sys/msg.h>  
#include <errno.h>

#include <vispaceio.h>

#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/sysinfo.h>
#include <sqlite3.h>
#include <public.h>
#include  <chassis.h>
#include  <unit.h>
#include  <sysinfo.h>
#include  <iniconf.h>
#include  <visipc.h>
#include  <dbutil.h>
#include  <service.h>


#define DEFAULT_IP "192.168.1.228"
#define DEFAULT_SUBNET_MASK "255.255.255.0"
#define DEFAULT_GATEWAY "192.168.1.1"
#define NMU_BASE_INFO_FILE "/vispace/nmu.bi"


void restoreNmuConf(int chassis,char isRestoreIp,char isReboot)
{
  if (isRestoreIp)
     setDeviceIp(DEFAULT_IP,DEFAULT_SUBNET_MASK,DEFAULT_GATEWAY);
  system("cp /vispace/webs/umconfig.def /vispace/webs/umconfig.txt");
  system("cp /vispace/sys.def /vispace/sys.conf");
  
  restoreSnmpdConf(0);
  // printf("Reboot ...\n");
  if (isReboot)
  {
   system("reboot");
  }
}

void restoreUnitConf(int chassis,int slot)
{
  if (slot<0 || slot>=MAX_UNIT_COUNT)
     return;
    char message[512];
    short value=1;
    message[0]='S';
    message[1]=UNIT_COMMON_CMD_INFOSET_ID;
    message[2]=2;
    message[3]=1;
    memcpy(&message[4],&value,sizeof(short));
    message[6]=0;
    printf("Restore unit #%d...\n",slot);
    sendMessageToUnit(chassis,slot,message,300,1,0,0);
  /*if (MAX_UNIT_COUNT==slot)
  {
   //printf("restore 1 ...\n");
   setDeviceIp(DEFAULT_IP,DEFAULT_SUBNET_MASK,DEFAULT_GATEWAY);
   //printf("restore 2 ...\n");
   system("cp /vispace/webs/umconfig.def /vispace/webs/umconfig.txt");
   //printf("restore 3 ...\n");
   //system("cp /vispace/snmpd/snmpd.def /vispace/snmpd/snmpd.txt");
   restoreSnmpdConf(0);
   printf("Reboot ...\n");
   system("reboot");
  }
  else
  {
    char message[512];
    short value=1;
    message[0]='S';
    message[1]=UNIT_COMMON_CMD_INFOSET_ID;
    message[2]=2;
    message[3]=1;
    memcpy(&message[4],&value,sizeof(short));
    message[6]=0;
    printf("Restore unit #%d...\n",slot);
    sendMessageToUnit(chassis,slot,message,300);
  }
  */
}

/*
int initNmuBaseInfo1(char chassis,int slot)
{
  unit_base_info_t *nmu_bi;
  struct timeval tpstart;
  char value[30]="";
  nmu_bi=getBaseInfoMap(0,slot);
  if (NULL==nmu_bi)
    return -1;
  getConfValue(SYS_CONF_FILE,"lasttime",value," ");
  //strcpy(value,"2015-11-01|00:00:00");
  
  if (strlen(value)>10)
  {
     time_t tt;
     struct tm tb={0};
     //struct tm* p;
     char format[]="%Y-%m-%d|%H:%M:%S"; 
     //char* timeStr="1970-1-1 0:1";//1970年1月1日0时1分 
     printf("lasttime=%s\n",value); 
     if (strptime(value, format, &tb) != NULL) 
     {
          //struct tm *p=localtime(&timep);
          //tt=mktime(&tb);
          //printf("year=%d\n",tb.tm_year); 
          nmu_bi->uptime=mktime(&tb);
          gettimeofday(&tpstart,NULL);
          //printf("year=%d\n",tb.tm_year); 
          if (tpstart.tv_sec<nmu_bi->uptime)
          {
             nmu_bi->uptime=tpstart.tv_sec;
          }
         strcpy(value,"0");
         setConfValue(SYS_CONF_FILE,"lasttime",value," ");
     }
  }
  else
  {
  gettimeofday(&tpstart,NULL);
  nmu_bi->uptime=tpstart.tv_sec;
  }
  #ifdef _DEBUG_
     printf("NMU Uptime:%d\n",tpstart.tv_sec);
  #endif
  getConfValue(NMU_BASE_INFO_FILE,"model",nmu_bi->model," ");
  //printf("NMU slot:%d model=%s\n",slot,nmu_bi->model);
  getConfValue(NMU_BASE_INFO_FILE,"sn",nmu_bi->sn," ");
  getConfValue(NMU_BASE_INFO_FILE,"creation",nmu_bi->creation," ");
  strcpy(nmu_bi->fwver,NMU_FW_VER);
  strcpy(nmu_bi->hwver,NMU_HW_VER);
  return 0;
}

int setNmuBaseInfo(int chassis,char *sn,char *hwver,char *creation,char *model)
{
  if (setConfValue(NMU_BASE_INFO_FILE,"hwver",hwver," "))
     return -1;
 // if (setConfValue(NMU_BASE_INFO_FILE,"nmu","fw_ver",NMU_FW_VERSION))
  //   return -1;
  if (setConfValue(NMU_BASE_INFO_FILE,"sn",sn," "))
     return -1;
  if (setConfValue(NMU_BASE_INFO_FILE,"creation",creation," "))
     return -1;
  if (setConfValue(NMU_BASE_INFO_FILE,"model",model," "))
     return -1;
  initNmuBaseInfo(0,16);
  return 0;

int getVendorInfo(int item_id,char *info)
{
  int ret=-1;
  switch (item_id)
  {
   case VENDOR_CN_NAME:
       ret=getConfValue(NMU_BASE_INFO_FILE,"vendorcn",info," ");
       break;
   case VENDOR_EN_NAME:
       ret=ret=getConfValue(NMU_BASE_INFO_FILE,"vendoren",info," ");
       break;
   case VENDOR_URL:
       ret=getConfValue(NMU_BASE_INFO_FILE,"url",info," ");
       break;
   case WEB_NAME:
       ret=getConfValue(NMU_BASE_INFO_FILE,"webname",info," ");
       break;
   case VENDOR_LOGO:
       ret=getConfValue(NMU_BASE_INFO_FILE,"logo",info," ");
       break;
 }
  return ret;
}
*/


#define ORG_DB_FILE "/vispace/www/data/vispace.db"
#define BACKUP_DB_PATH "/vispace/www/data"

/***********************************
*备份用户数据
***********************************/
int restoreUserData(char* fn)
{
  char sql[1024];
  int rc;
  sqlite3 *org_db=NULL;
  sqlite3 *backup_db=NULL;
  sqlite3_stmt* stmt;

  //if((access(BACKUP_DB_PATH,F_OK))!=0)
  //doSystem("cp %s %s/%s ",ORG_DB_FILE,BACKUP_DB_PATH,fn);
  org_db=openDB(ORG_DB_FILE);
  if (org_db==NULL)
    return -1;
  sprintf(sql,"%s/%s",BACKUP_DB_PATH,fn);
  backup_db=openDB(sql);
  if (backup_db==NULL)
  {
    sqlite3_close(org_db);
    return -1;
  }
  strcpy(sql,"SELECT chassis,slot,unit_name,contact,location,up_device,user_data,service FROM UnitUD");
  //strcpy(sql,"DELETE FROM UnitUD");
  //execSql(backup_db, sql,NULL,NULL);

  rc=sqlite3_prepare_v2(backup_db,sql,strlen(sql),&stmt,NULL);
  if(rc!=SQLITE_OK)
  {
     sqlite3_finalize(stmt);
     sqlite3_close(org_db);
     return -1;
  }
  //strcpy(sql,"DELETE FROM UnitUD");
  //execSql(org_db, sql,NULL,NULL);

  rc=sqlite3_step(stmt);
  while (rc==SQLITE_ROW)
  {
   sprintf(sql,"UPDATE UnitUD set unit_name='%s',contact='%s',location='%s',up_device='%s',user_data='%s',service='%s' WHERE chassis=%d AND slot=%d",sqlite3_column_text(stmt,2),sqlite3_column_text(stmt,3),sqlite3_column_text(stmt,4),sqlite3_column_text(stmt,5),sqlite3_column_text(stmt,6),sqlite3_column_text(stmt,7),sqlite3_column_int(stmt,0),sqlite3_column_int(stmt,1));
   execSql(org_db, sql,NULL,NULL);
   rc=sqlite3_step(stmt);
  }
  sqlite3_finalize(stmt);
  sqlite3_close(org_db);
  sqlite3_close(backup_db);
  return 0;
}

/***********************************
*恢复SNMP Coununity数据
***********************************/
int restoreSnmpCoununity(char* fn)
{
  char sql[1024];
  int rc;
  sqlite3 *org_db=NULL;
  sqlite3 *backup_db=NULL;
  sqlite3_stmt* stmt;

  //if((access(BACKUP_DB_PATH,F_OK))!=0)
  //doSystem("cp %s %s/%s ",ORG_DB_FILE,BACKUP_DB_PATH,fn);
  org_db=openDB(ORG_DB_FILE);
  if (org_db==NULL)
    return -1;
  sprintf(sql,"%s/%s",BACKUP_DB_PATH,fn);
  backup_db=openDB(sql);
  if (backup_db==NULL)
  {
    sqlite3_close(org_db);
    return -1;
  }
  strcpy(sql,"SELECT rocommunity, rwcommunity,ipadd,port,read_only,enable FROM Community");
  //strcpy(sql,"DELETE FROM UnitUD");
  //execSql(backup_db, sql,NULL,NULL);

  rc=sqlite3_prepare_v2(backup_db,sql,strlen(sql),&stmt,NULL);
  if(rc!=SQLITE_OK)
  {
     sqlite3_finalize(stmt);
     sqlite3_close(org_db);
     return -1;
  }
 
  execSql(NULL,"DELETE FROM Community;",NULL,NULL);
  rc=sqlite3_step(stmt);
  while (rc==SQLITE_ROW)
  {
   sprintf(sql,"INSERT Community VALUES(NULL,'%s','%s','%s',%d,%d,%d)",sqlite3_column_text(stmt,0),sqlite3_column_text(stmt,1),sqlite3_column_text(stmt,2),sqlite3_column_int(stmt,3),sqlite3_column_int(stmt,4),sqlite3_column_int(stmt,5));
   execSql(org_db, sql,NULL,NULL);
   rc=sqlite3_step(stmt);
  }
  sqlite3_finalize(stmt);
  sqlite3_close(org_db);
  sqlite3_close(backup_db);
  return 0;
}
/***********************************
*备份DestHost数据
***********************************/
int restoreSnmpDestHost(char* fn)
{
  char sql[1024];
  int rc;
  sqlite3 *org_db=NULL;
  sqlite3 *backup_db=NULL;
  sqlite3_stmt* stmt;

  //if((access(BACKUP_DB_PATH,F_OK))!=0)
  //doSystem("cp %s %s/%s ",ORG_DB_FILE,BACKUP_DB_PATH,fn);
  org_db=openDB(ORG_DB_FILE);
  if (org_db==NULL)
    return -1;
  sprintf(sql,"%s/%s",BACKUP_DB_PATH,fn);
  backup_db=openDB(sql);
  if (backup_db==NULL)
  {
    sqlite3_close(org_db);
    return -1;
  }
  strcpy(sql,"SELECT dest_ipadd, udp_port,enable FROM DestHost");
  //strcpy(sql,"DELETE FROM UnitUD");
  //execSql(backup_db, sql,NULL,NULL);

  rc=sqlite3_prepare_v2(backup_db,sql,strlen(sql),&stmt,NULL);
  if(rc!=SQLITE_OK)
  {
     sqlite3_finalize(stmt);
     sqlite3_close(org_db);
     return -1;
  }
 
  execSql(NULL,"DELETE FROM DestHost;",NULL,NULL);
  rc=sqlite3_step(stmt);
  while (rc==SQLITE_ROW)
  {
   sprintf(sql,"INSERT DestHost VALUES(NULL,'%s',%d,%d)",sqlite3_column_text(stmt,0),sqlite3_column_int(stmt,1),sqlite3_column_int(stmt,2));
   execSql(org_db, sql,NULL,NULL);
   rc=sqlite3_step(stmt);
  }
  sqlite3_finalize(stmt);
  sqlite3_close(org_db);
  sqlite3_close(backup_db);
  return 0;
}
/***********************************
*数据备份
***********************************/
void backupData(maintain_type type)
{
  char sql[200];
  sqlite3 *backup_db=NULL;

  if((access(BACKUP_DB_PATH,F_OK))!=0)
    doSystem("cp %s %s ",ORG_DB_FILE,BACKUP_DB_PATH);
  switch (type)
  {
    case mt_user_data:
    strcpy(sql,"DELETE FROM UnitUD");
    execSql(backup_db, sql,NULL,NULL);
    strcpy(sql,"SELECT chassis,slot,unit_name,contact,location,up_device,user_data,service FROM UnitUD");
    execSql(NULL, sql,NULL,NULL);
    break;
    case mt_snmp_conf:
    break;
    case mt_user:
    break;
    case mt_alarm:
    break;
    case mt_alarm_conf:
    break;
    case mt_log:
    break;
    case mt_log_conf:
    break;
  }
}
/***********************************
*数据备份
***********************************/
void restoreData(maintain_type type)
{
  switch (type)
  {
    case mt_user_data:
    break;
    case mt_snmp_conf:
    break;
    case mt_user:
    break;
    case mt_alarm:
    break;
    case mt_alarm_conf:
    break;
    case mt_log:
    break;
    case mt_log_conf:
    break;
  }
}
/***********************************
*数据维护
***********************************/
void maintainData(maintain_type type)
{

}


