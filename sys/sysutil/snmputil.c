#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <unit.h>
#include <sqlite3.h>
#include <dbutil.h>
#include <signal.h>
#include <visipc.h>

#define SNMPD_CONF_FILE "/vispace/snmp/snmpd.conf"
int updateSnmpdConf(char isRestart)
{
  int ret;
  sqlite3_stmt* stmt=NULL;
  sqlite3 *db;
  char sql[100]="SELECT rocommunity,rwcommunity,ipadd,read_only FROM Community WHERE enable=1";
  char line[80]="";
  FILE *fp;
  /*
  deleteCommunityById(0);
  insertCommunity("","public","private",161,2,1);
  deleteDestHost(NULL,0);
  insertDestHost("TN238","192.168.1.238",9162,1);
  */
  fp = fopen(SNMPD_CONF_FILE, "w");
   if ( !fp ) {
      return -1;
   }

  db=openDB(DB_FILE_NAME);
  
  if (NULL==db)
  {
    fclose(fp);
    return 0;
  }
  //printf("snmpdConf sq:%s\n",sql);
  ret=sqlite3_prepare_v2(db,sql,strlen(sql),&stmt,NULL);
  if(ret!=SQLITE_OK)
  {
    printf("snmpdConf Error:%s\n",sqlite3_errmsg(db));
    if (stmt)
        sqlite3_finalize(stmt);
    sqlite3_close(db);
    fclose(fp);
    return 0;
  }
  ret=sqlite3_step(stmt);
  //printf("snmpdConf 1\n");
  //int cols=sqlite3_column_count(stmt);
  int n;
  sprintf(line,"rocommunity public localhost\nrwcommunity private localhost\n");
  fputs(line,fp);
  /*
  if (ret==SQLITE_ROW)
  {
   if (strlen(sqlite3_column_text(stmt,2))>0)//指定IP主机皆可访问
   {
    sprintf(line,"rocommunity public localhost\nrwcommunity private localhost\n");
    fputs(line,fp);
   }
  }
  else//只有本机可访问
  {
    sprintf(line,"rocommunity public localhost\nrwcommunity private localhost\n");
    fputs(line,fp);
  }
  */
  while (ret==SQLITE_ROW)
  {
     //printf("rocommunity\n");
   //if (strlen(sqlite3_column_text(stmt,0)>0)
    sprintf(line,"rocommunity %s %s\n",sqlite3_column_text(stmt,0),sqlite3_column_text(stmt,2));
    fputs(line,fp);
    //printf("rocommunity:%s\n",line);
    if (1!=sqlite3_column_int(stmt,1))
    {
     sprintf(line,"rwcommunity %s %s\n",sqlite3_column_text(stmt,1),sqlite3_column_text(stmt,2));
     fputs(line,fp);
    }
    //printf("rwcommunity:%s\n",line);
    ret=sqlite3_step(stmt);
    // count++;
  }
  sqlite3_finalize(stmt);
  sqlite3_close(db);
  //sprintf(line,"rwcommunity %s");
  fputs("sysDescr G2 Integrated Managable Platform\n",fp);
  
  /*
  uchar_t slot=17;
  unit_user_data_t udata;
  slot= getSlotOfNmu(0,1);
  if (getUserDataFromDB(0,slot,&udata)!=FALSE)
  {
    sprintf(line,"sysName %s\n",udata.name);
    fputs(line,fp);
    sprintf(line,"sysContact %s\n",udata.contact);
    fputs(line,fp);
    sprintf(line,"sysLocation %s",udata.location);
    fputs(line,fp);
  }
  */
  fclose(fp);
  if (isRestart)
  {
  system("killall -9 snmpd");
  printf("Restarting snmpd ...\n");
  sleep(1);
  system("snmpd -c /vispace/snmp/snmpd.conf");
  printf("snmpd lauched\n");
  }
  //printf("snmpdConf count=%d\n",count);
}
int insertCommunity(const char *ipadd,const char *rocommunity,const char *rwcommunity,int port,int read_only,int enable,char isRestart)
{
  char sql[160];
  // printf("port=%d\n",port);
  if (read_only<1 || read_only>2 ||enable<1 || enable>2 ||strlen(rocommunity)>15 ||strlen(rwcommunity)>15 || port<1)
     return -1;
  //判断是否存在空的IP
  sprintf(sql,"SELECT ID FROM Community WHERE length(ipadd)<6 LIMIT 1",ipadd);
  if (getRowCount(NULL,sql)>0)
     return 1;
  if (strlen(ipadd)<6) //如果要添加空的IP，判断是否存在IP
  {
    sprintf(sql,"SELECT ID FROM Community LIMIT 1",ipadd);
    if (getRowCount(NULL,sql)>0)
     return 2;
  }
  //判断该IP是否存在
  sprintf(sql,"SELECT ID FROM Community WHERE ipadd='%s' AND port=%d",ipadd,port);
  if (getRowCount(NULL,sql)<1)
  {
     sprintf(sql,"INSERT INTO \"Community\" VALUES(NULL,'%s','%s','%s',%d,%d,%d)",rocommunity,rwcommunity,ipadd,port,read_only, enable);
     if (SQLITE_OK!=execSql(NULL, sql,NULL,NULL))
       return -1;
     else
     {
       updateSnmpdConf(isRestart);
       return 0;
     }
  }
  else
      return -1;
}

int updateCommunity(int id,const char *ipadd,const char *rocommunity,const char *rwcommunity,int port,int read_only,int enable,char isRestart)
{
  char sql[160];
  if (read_only<1 || read_only>2 ||enable<1 || enable>2 ||strlen(rocommunity)>15 ||strlen(rwcommunity)>15)
     return -1;
  //判断是否存在相同IP
  sprintf(sql,"SELECT ID FROM Community WHERE ipadd='%s' AND ID<>%d LIMIT 1",ipadd,id);
  if (getRowCount(NULL,sql)>0)
     return 1;
  sprintf(sql,"UPDATE \"Community\" SET ipadd ='%s',rocommunity='%s',rwcommunity='%s',port=%d,read_only=%d,enable=%d WHERE ID=%d;",ipadd,rocommunity,rwcommunity,port,read_only, enable,id);
  if (SQLITE_OK!=execSql(NULL, sql,NULL,NULL))
        return -1;
  else
  {
    updateSnmpdConf(isRestart);
    return 0;
  }
}
int deleteCommunityById(int id,char isRestart)
{
  char sql[60]="DELETE FROM \"Community\"";
  
  if (id<0)
     return -1;
  if (id>0)
  {
     char where[20]="";
     sprintf(where," WHERE ID=%d;",id);
     strcat(sql,where);
  }
  if (SQLITE_OK!=execSql(NULL, sql,NULL,NULL))
        return -1;
  else
  {
    updateSnmpdConf(isRestart);
    return 0;
  }
}
int deleteCommunityByIp(char * ip,char isRestart)
{
  char sql[60];
  sprintf(sql,"DELETE FROM \"Community\" WHERE ipadd='%s'",ip);
  if (SQLITE_OK!=execSql(NULL, sql,NULL,NULL))
        return -1;
  else
  {
     updateSnmpdConf(isRestart);
     return 0;
  }
}
int updateDestHost(int id,char *dest_ip,int udp_port,int enable)
{
  if (udp_port<1)
     return -1;
  char sql[120]="";
  sprintf(sql,"UPDATE DestHost SET dest_ipadd='%s',udp_port=%d,enable=%d WHERE ID=%d",dest_ip,udp_port,enable,id);
  if (SQLITE_OK!=execSql(NULL, sql,NULL,NULL))
        return -1;
  else
  {  
     union sigval val;
     val.sival_int = SIG_DEST_HOST_MODIFIED_ID;
     sendSig(findPid("alarmd"),SIGINT,val);
     return 0;
  }
}
int insertDestHost(const char *target_name,const char *dest_ipadd,int udp_port,int enable)
{
  char sql[180];
  //printf("insertDestHost target_name=%s\n",target_name);
  if (udp_port<0 || enable<1 || enable>2 ||strlen(dest_ipadd)>24 || udp_port<1 ||strlen(target_name)<1)
 {
    //printf("insertDestHost fail!\n");
    return -1;
 }
 
  sprintf(sql,"SELECT ID FROM DestHost WHERE dest_ipadd='%s' AND udp_port=%d",dest_ipadd,udp_port);
  //printf("insertDestHost\n");
  if (getRowCount(NULL,sql)<1)
  {
     sprintf(sql,"INSERT INTO \"DestHost\" VALUES(NULL,'%s',%d,%d,'%s');",dest_ipadd,udp_port, enable,target_name);
     if (SQLITE_OK!=execSql(NULL, sql,NULL,NULL))
       return -1;
     else
     {
       union sigval val;
       val.sival_int = SIG_DEST_HOST_MODIFIED_ID;
       sendSig(findPid("alarmd"),SIGINT,val);
       return 0;
     }
  }
  else
      return -1;
}
int insertDestHostById(int id,const char *target_name,const char *dest_ipadd,int udp_port,int enable)
{
  if (id <1)
     return insertDestHost(target_name,dest_ipadd,udp_port,enable);
  char sql[180];
   sprintf(sql,"SELECT ID FROM DestHost WHERE ID=%d OR dest_ipadd='%s' AND udp_port=%d",id,dest_ipadd,udp_port);
  if (getRowCount(NULL,sql)<1)
  {
     sprintf(sql,"INSERT INTO \"DestHost\" VALUES(%d,'%s',%d,%d,'%s');",id,dest_ipadd,udp_port, enable,target_name);
     if (SQLITE_OK!=execSql(NULL, sql,NULL,NULL))
       return -1;
     else
     {
      union sigval val;
      val.sival_int = SIG_DEST_HOST_MODIFIED_ID;
      sendSig(findPid("alarmd"),SIGINT,val);
      return 0;
     }
  }
  else
      return -1;
}
int deleteDestHost(const char *dest_ipadd,int udp_port)
{
  char sql[80]="DELETE FROM \"DestHost\"";
  char where[50]="";
  
  if (NULL!=dest_ipadd)
  {
    sprintf(where," WHERE dest_ipadd='%s' AND udp_port=%d",dest_ipadd,udp_port);
    strcat(sql,where);
  }
  if (SQLITE_OK!=execSql(NULL, sql,NULL,NULL))
        return -1;
  else
  {  
     union sigval val;
     val.sival_int = SIG_DEST_HOST_MODIFIED_ID;
     sendSig(findPid("alarmd"),SIGINT,val);
     return 0;
  }
}
int deleteDestHostById(int id)
{
  char sql[120]="";
  sprintf(sql,"DELETE FROM \"DestHost\" WHERE ID=%d;",id);
  if (SQLITE_OK!=execSql(NULL, sql,NULL,NULL))
        return -1;
  else
  {  
     union sigval val;
     val.sival_int = SIG_DEST_HOST_MODIFIED_ID;
     sendSig(findPid("alarmd"),SIGINT,val);
     return 0;
  }
}
int restoreSnmpdConf(char isRestart)
{
  deleteCommunityById(0,0);
  insertCommunity("","public","private",161,2,1,0);
  deleteDestHost(NULL,0);
  insertDestHost("TN238","192.168.1.238",9162,1);
  updateSnmpdConf(isRestart);
}
/*
int restoreSnmpdConf1(char isRestart)
{
  deleteCommunityById(0);
  insertCommunity("","public","private",161,2,1);
  deleteDestHost(NULL,0);
  insertDestHost("TN238","192.168.1.238",9162,1);
  restoreSnmpdConf(isRestart);
}
*/
