#include <string.h>
#include <sys/sysinfo.h>
#include <time.h>
#include "../wsIntrn.h"
#include "../../include/unit.h"
#include <public.h>
#include <visipc.h>
#include <sqlite3.h>
#include <dbutil.h>
#include <log.h>

extern char show_alarm_count;
//extern char save_alarm_detail;
static int almTypePage=0;

uchar_t getUnitAlarm(uchar_t chassis,uchar_t has_alarm[])
{
   int n=0;
   int ret;
   char sql[]="SELECT distinct slot FROM AlarmLog WHERE alarm_status<>3 LIMIT 16";
   //char has_alarm[MAX_UNIT_COUNT+1]={0};
   uchar_t alm_count=0;
   sqlite3 *db;
   sqlite3_stmt* stmt=NULL;
   db=openDB(DB_FILE_NAME);
  
 if (NULL==db)
 {
    return 0;
 }
 ret=sqlite3_prepare_v2(db,sql,strlen(sql),&stmt,NULL);
 if(ret!=SQLITE_OK)
  {
    //printf("getUnitAlarm Error:%s\n",sqlite3_errmsg(db));
    if (stmt)
        sqlite3_finalize(stmt);
    sqlite3_close(db);
    return 0;
  }
  //memset(&has_alarm,0,getMaxSlot(0));
  ret=sqlite3_step(stmt);
  while (ret==SQLITE_ROW)
  {
    n=sqlite3_column_int(stmt,0);
    if (n>-1 && n < MAX_UNIT_COUNT)
    {
      has_alarm[n]=1;
      alm_count++;
    }
    //printf("alm in slot #%d has_alarm=%d\n",n,has_alarm[n]);
    //nBytesSent +=websWrite(wp, T("<div class='uf'></div>\n"));
    ret=sqlite3_step(stmt);
  }
  sqlite3_finalize(stmt);
  sqlite3_close(db);
  //printf("alm_count:%d\n",alm_count);
  return alm_count;
}
int rows_per_page=20;
static int aspAlarmTypeList(int eid, webs_t wp, int argc, char_t **argv)
{
  sqlite3 *db;
  sqlite3_stmt* stmt=NULL;
  int ret,n;
  int nBytes=0,rowcount,page_count=0;
  char buf[30];
  int sid;

  #ifdef DEFAULT_LANG_CN
  char sql[120]="SELECT alarm_code,alarm_name,entity_name,severity,is_send,mask_slot,is_save FROM AlarmType WHERE usable<>2";
  //char severity[6][16]={"Unkown","紧急告警", "严重告警","主要告警" ,"次要告警" ,"提示告警"};
  char severity[6][16]={"Unkown","紧急告警", "主要告警","次要告警" ,"提示告警" ,"清除告警"};
  #else
  char sql[160]="SELECT alarm_code,alarm_name_en as alarm_name,entity_name_en as entity_name,severity,is_send,mask_slot,is_save FROM AlarmType WHERE usable<>2";
  char severity[6][16]={"Unkown","critical", "major","minor" ,"waring" ,"clean"};
  #endif

  db=openDB(DB_FILE_NAME);
  
  if (NULL==db)
     return 0;
  rowcount=getRowCount(db,sql);
  page_count=(rowcount + rows_per_page -1) / rows_per_page;
  sprintf(buf," LIMIT %d",rows_per_page);
  strcat(sql,buf);
  sprintf(buf," OFFSET %d",almTypePage*rows_per_page);
  strcat(sql,buf);
  //printf("rows_per_page=%d\n",rows_per_page);
  ret=sqlite3_prepare_v2(db,sql,strlen(sql),&stmt,NULL);
  if(ret!=SQLITE_OK)
  {
    printf("aspAlarmTypeError:%s\n",sqlite3_errmsg(db));
    if (stmt)
        sqlite3_finalize(stmt);
    sqlite3_close(db);
    return 0;
  }
  ret=sqlite3_step(stmt);
  while (ret==SQLITE_ROW)
  {
    nBytes+=websWrite((webs_t)wp,T("<tr>"));
  //if (strcmp(column_value[0],"0"))
    nBytes+=websWrite((webs_t)wp,T("<td>%d</td>"),sqlite3_column_int(stmt,0));
    nBytes+=websWrite((webs_t)wp,T("<td>%s</td>"),sqlite3_column_text(stmt,1));
    nBytes+=websWrite((webs_t)wp,T("<td>%s</td>"),sqlite3_column_text(stmt,2));
    sid=sqlite3_column_int(stmt,3);
    if (sid<1 || sid>5)
      sid=0;
    nBytes+=websWrite((webs_t)wp,T("<td class='s%d'>%s</td>"),sid,severity[sid]);
   #ifdef DEFAULT_LANG_CN
    if (1!=sqlite3_column_int(stmt,4))
      nBytes+=websWrite((webs_t)wp,T("<td>不发送</td>"));
    else
     nBytes+=websWrite((webs_t)wp,T("<td>发送</td>"));
  #else
    if (1!=sqlite3_column_int(stmt,4))
      nBytes+=websWrite((webs_t)wp,T("<td>Not</td>"));
    else
     nBytes+=websWrite((webs_t)wp,T("<td>Yes</td>"));
  #endif
    
    #ifdef DEFAULT_LANG_CN
    if (1!=sqlite3_column_int(stmt,6))
      nBytes+=websWrite((webs_t)wp,T("<td>不保存</td>"));
    else
     nBytes+=websWrite((webs_t)wp,T("<td>保存</td>"));
  #else
    if (1!=sqlite3_column_int(stmt,6))
      nBytes+=websWrite((webs_t)wp,T("<td>Not</td>"));
    else
     nBytes+=websWrite((webs_t)wp,T("<td>Yes</td>"));
  #endif
    nBytes+=websWrite((webs_t)wp,T("<td>"));
    if (sqlite3_column_int(stmt,5)!=0)
    {
        short mask=sqlite3_column_int(stmt,5);
        char m;
        for (m=1;m<16;m++)
        {
           if (0x1 & (mask>>(15-m)))
              nBytes+=websWrite((webs_t)wp,T(" %d"),m);
        }
    }

    //nBytes+=websWrite((webs_t)wp,T("<td>%d</td>"),sqlite3_column_int(stmt,5));
    nBytes+=websWrite((webs_t)wp,T("</td>"));
    if (umGetUserPrivilege(wp->userName)>1)
      nBytes+=websWrite((webs_t)wp,T("<td><a href='/goform/atype?ac=%d' target='mf'><img src='/images/pen.gif'/></a></td>"),sqlite3_column_int(stmt,0));
    /*
    #ifdef DEFAULT_LANG_CN
    nBytes+=websWrite((webs_t)wp,T("<td><a href='/goform/atype?ac=%d' target='mf'><img src='images/pen.gif'/></a></td>"),sqlite3_column_int(stmt,0));
    #else
     nBytes+=websWrite((webs_t)wp,T("<td><a href='/goform/atype?ac=%d' target='mf'><img src='../images/pen.gif'/></a></td>"),sqlite3_column_int(stmt,0));
    #endif
    */
    nBytes+=websWrite((webs_t)wp,T("</tr>"));

    ret=sqlite3_step(stmt);
  }
  sqlite3_finalize(stmt);
  sqlite3_close(db);
  #ifdef DEFAULT_LANG_CN
    nBytes+=websWrite(wp, T("<tr><td colspan='8'>告警类型数:共%d条 共%d页&nbsp;GO->"),rowcount,page_count);
  #else
    nBytes+=websWrite(wp, T("<tr><td colspan='8'>Sum of alarm type:Tol:%d %d Page(s)&nbsp;GO->"),rowcount,page_count);
  #endif
  for (n=0;n<page_count;n++)
  {
   if (almTypePage!=n)
   {
   #ifdef DEFAULT_LANG_CN
    nBytes+=websWrite(wp, T("<a style='margin-left:5px;' href =\"#\" onclick='query(%d);'>第%d页</a>"),n,n+1);
   #else
    nBytes+=websWrite(wp, T("<a style='margin-left:5px;' href =\"#\" onclick='query(%d);'>Page %d</a>"),n,n+1);
   #endif
   }

   else
   {
      #ifdef DEFAULT_LANG_CN
       nBytes+=websWrite(wp, T("&nbsp;&nbsp;第%d页"),n+1);
      #else
       nBytes+=websWrite(wp, T("&nbsp;&nbsp;Page %d"),n+1);
      #endif
   }
  }
  nBytes+=websWrite(wp, T("</td></tr>"));
  return nBytes;
}

static void formAlarmTypeQuery(webs_t wp, char_t *path, char_t *query)
{
  char_t *pg;
  pg= websGetVar(wp, T("pg"), T("0"));
  almTypePage=atoi(pg);
  websRedirect(wp, T("alm_type.asp"));
}
static int alarmcode;

static int onFetchAlarmType( void *wp, int n_column, char **column_value, char **column_name)
{
  #ifdef DEFAULT_LANG_CN
  char severity[5][16]={"紧急告警", "主要告警","次要告警" ,"提示告警" ,"信息提示"};
  #else
  char severity[5][16]={"critical", "major","minor" ,"waring" ,"infomation"};
  #endif
  
   char n;
   int sid=atoi(column_value[2]);

   websWrite((webs_t)wp,T("<input type='hidden' id='ac' name='ac' value='%s'/>"),column_value[0]);
   websWrite((webs_t)wp,T("<input type='hidden' id='slots' name='slots' value='%s'/>"),column_value[4]);
   #ifdef DEFAULT_LANG_CN
   websWrite((webs_t)wp,T("<tr><th>告警代码:</th><td>%s</td> </tr>"),column_value[0]);
   websWrite((webs_t)wp,T("<tr><th>告警信息:</th><td><input type=text id=aname title='告警信息' size=16 value='%s'></td> </tr>"),column_value[1]);
   websWrite((webs_t)wp,T("<tr><th>告警级别:</th><td><select id=\"severity\">"));
   #else
   websWrite((webs_t)wp,T("<tr><th>Alarm code:</th><td>%s</td> </tr>"),column_value[0]);
   websWrite((webs_t)wp,T("<tr><th>Alarm info:</th><td><input type=text id=aname title='Alarm info' size=16 value='%s'></td> </tr>"),column_value[1]);
   websWrite((webs_t)wp,T("<tr><th>Alarm serverity:</th><td><select id=\"severity\">"));
   #endif

   for (n=1;n<6;n++)
   {
      websWrite((webs_t)wp,T("<option value=\"%d\""),n);
      if (n==sid)
         websWrite((webs_t)wp,T(" selected"));
      websWrite((webs_t)wp,T(">%s"),severity[n-1]);
   }
   websWrite((webs_t)wp,T("</select></td> </tr>"));
   #ifdef DEFAULT_LANG_CN
   websWrite((webs_t)wp,T("<tr><th>启用状态:</th><td><input type=checkbox id=enable title='启用状态'"));
   #else
     //  websWrite((webs_t)wp,T("<tr><th>Enable:</th><td><input type=checkbox id=enable title='Enable'"));
   #endif
   if (!strcmp(column_value[3],"1"))
      websWrite((webs_t)wp,T(" checked=true"));
   websWrite((webs_t)wp,T( " onclick='checkEnable();'/></td> </tr>"));
   return 0;
}

static int aspAlarmType(int eid, webs_t wp, int argc, char_t **argv)
{
     char sql[120]="";
     sqlite3 *db;
     sqlite3_stmt* stmt=NULL;
     //char *zErrMsg = 0;
     
     #ifdef DEFAULT_LANG_CN
      //char severity[5][16]={"紧急告警", "严重告警","主要告警" ,"次要告警" ,"提示告警"};
      char severity[5][16]={"紧急告警", "主要告警","次要告警" ,"提示告警" ,"清除告警"};
     #else
      char severity[5][16]={"critical", "major","minor" ,"waring" ,"clean"};
     #endif
     char n;
     int sid,nBytes=0;
     int ret;
     db=openDB(DB_FILE_NAME);
  
     if (NULL==db)
      return websWrite(wp, "AlarmType can not open!\n");
  //printf("aspOperateLog2 sq:%s DB_FILE_NAME=%s\n",sql,DB_FILE_NAME);
    #if DEFAULT_LANG_CN
     sprintf(sql,"SELECT alarm_code,alarm_name,severity,is_send,mask_slot,is_save FROM AlarmType WHERE alarm_code=%d LIMIT 1",alarmcode);
    #else
     sprintf(sql,"SELECT alarm_code,alarm_name_en,severity,is_send,mask_slot,is_save FROM AlarmType WHERE alarm_code=%d LIMIT 1",alarmcode);
    #endif
    ret=sqlite3_prepare_v2(db,sql,strlen(sql),&stmt,NULL);
    if(ret!=SQLITE_OK)
    {
      //printf("aspUnitAlarmView Error:%s\n",sqlite3_errmsg(db));
      if (stmt)
        sqlite3_finalize(stmt);
      sqlite3_close(db);
      return websWrite(wp, T("aspAlarmType Error:%s\n"),sqlite3_errmsg(db));
    }
    ret=sqlite3_step(stmt);
    while (ret==SQLITE_ROW)
    {
      sid=sqlite3_column_int(stmt,2);
      /*
      nBytes+=websWrite(wp,T("<input type='hidden' id='ac' name='ac' value='%d'/>"),sqlite3_column_int(stmt,0));
   nBytes+=websWrite(wp,T("<input type='hidden' id='slots' name='slots' value='%s'/>"),sqlite3_column_text(stmt,4));
   #ifdef DEFAULT_LANG_CN
   nBytes+=websWrite(wp,T("<tr><th>告警代码:</th><td>%d</td> </tr>"),sqlite3_column_int(stmt,0));
   nBytes+=websWrite(wp,T("<tr><th>告警信息:</th><td>%s</td> </tr>"),sqlite3_column_text(stmt,1));
   nBytes+=websWrite((webs_t)wp,T("<tr><th>告警级别:</th><td><select id=\"severity\">"));
   #else
   nBytes+=websWrite(wp,T("<tr><th>Alarm code:</th><td>%d</td> </tr>"),sqlite3_column_int(stmt,0));
   nBytes+=websWrite(wp,T("<tr><th>Alarm info:</th><td>%s</td> </tr>"),sqlite3_column_text(stmt,1));
   nBytes+=websWrite((webs_t)wp,T("<tr><th>Alarm serverity:</th><td><select id=\"severity\">"));
   #endif
   */
   nBytes+=websWrite(wp,T("var alarm_code=%d,alarm_info='%s',severity=%d,"),sqlite3_column_int(stmt,0),sqlite3_column_text(stmt,1),sid);
   nBytes+=websWrite(wp,T("is_send=%d,is_save=%d;\n"),sqlite3_column_int(stmt,3),sqlite3_column_int(stmt,5));
   nBytes+=websWrite(wp,T("var max_slot=%d;\n"),getSlotOfNmu(0,5));
   nBytes+=websWrite(wp,T("var mask_slots='"));
   short mask=sqlite3_column_int(stmt,4);
   if (mask!=0)
   {
       int n;
       char bits[20]={0};
       short value=0;
       char masks[60]="";
       char slot[6];
       //int count=strtoca(slots,";",vals,10);
       shorttobits(mask,bits);
       for (n=1;n<16;n++)
       {
          if (bits[n]!=0)
          {
              sprintf(slot,"%d",n);
              nBytes+=websWrite(wp,T("%s;"),slot);
          }
             //if (gstrstr(slots)>1)
       }
   }
   else
     nBytes+=websWrite(wp,T("%d"),0);
   nBytes+=websWrite(wp,T("';\n"));
   //nBytes+=websWrite(wp,T("var max_slot=%d,mask_slots='%s';\n"),getSlotOfNmu(0,5),sqlite3_column_text(stmt,4));
   //nBytes+=websWrite(wp,T("var alarm_info=%s;\n"),sqlite3_column_text(stmt,1));
   //nBytes+=websWrite((webs_t)wp,T("var serverity=%d;\n"),sid);
   /*
   for (n=1;n<6;n++)
   {
      nBytes+=websWrite(wp,T("<option value=\"%d\""),n);
      if (n==sid)
         nBytes+=websWrite(wp,T(" selected"));
      nBytes+=websWrite(wp,T(">%s"),severity[n-1]);
   }
   nBytes+=websWrite(wp,T("</select></td> </tr>"));
   #ifdef DEFAULT_LANG_CN
   nBytes+=websWrite(wp,T("<tr><th>是否发送:</th><td><input type=checkbox id=enable title='是否发送该类告警'"));
   #else
    nBytes+=websWrite(wp,T("<tr><th>Is send?</th><td><input type=checkbox id=enable title='Enable'"));
   #endif
   if (1==sqlite3_column_int(stmt,3))
      nBytes+=websWrite(wp,T(" checked=true"));
   nBytes+=websWrite(wp,T( " onclick='checkEnable();'/></td> </tr>"));

   #ifdef DEFAULT_LANG_CN
   nBytes+=websWrite(wp,T("<tr><th>是否保存?</th><td><input type=checkbox id=save title='是否保存该类告警'"));
   #else
    nBytes+=websWrite(wp,T("<tr><th>Is save?</th><td><input type=checkbox id=save title='Is save this alarm type'"));
   #endif
   if (1==sqlite3_column_int(stmt,5))
      nBytes+=websWrite(wp,T(" checked=true"));
   nBytes+=websWrite(wp,T( "'/></td> </tr>"));
   */
    ret=sqlite3_step(stmt);
   }
   sqlite3_finalize(stmt);
   sqlite3_close(db);

   //nBytes+=websWrite(wp,T("<input type='hidden' name='ac' value='%d'/>"),alarmcode);
   return nBytes;
}
static void  formAlarmConf(webs_t wp, char_t *path, char_t *query)
{
  char_t *save,*count;
  char buf[16];
  int save_flag;
  unsigned int acount;
  save= websGetVar(wp, T("save"), T("0"));
  save_flag=atoi(save);
  if (save_flag!=0)
  {
    count= websGetVar(wp, T("max_rowcount"), T("0"));
    rows_per_page=atoi(count);
    //printf("count=%s rows_per_page=%d\n",count,rows_per_page);
    if (rows_per_page<20)
        rows_per_page=20;
    sprintf(buf,"%d",rows_per_page);
    setConfValue(SYS_CONF_FILE,"maxrows",buf," ");
    count= websGetVar(wp, T("max_acount"), T("0"));
    acount=atoi(count);
    //printf("SYS_CONF_FILE=%s count=%s acount=%d buf=%s\r\n",SYS_CONF_FILE,count,acount,buf);
    if (acount<1)
        acount=300;
    sprintf(buf,"%d",acount);
    //printf("SYS_CONF_FILE=%s count=%s acount=%d buf=%s\r\n",SYS_CONF_FILE,count,acount,buf);
    setConfValue(SYS_CONF_FILE,"max_acount",buf," ");
    if (save_flag==1)
    {
        char_t *showcount;
        showcount= websGetVar(wp, T("showcount"), T("0"));
        if (!gstrcmp(showcount,"Y"))
        {
          setConfValue(SYS_CONF_FILE,"show_acount","1"," ");
          show_alarm_count=1;
        }
        else
        {
          setConfValue(SYS_CONF_FILE,"show_acount","2"," ");
          show_alarm_count=2;
       }
    }
    else
        show_alarm_count=2;
  }
  else
      show_alarm_count=2;

  setConfValue(SYS_CONF_FILE,"save_alarms",save," ");
  
  websRedirect(wp, T("almconf.asp"));
}
static void formAlarmType(webs_t wp, char_t *path, char_t *query)
{
  char_t *alarm_code,*operate;
  //char sql[100]="";
  //char where[20]="";
  //int unit;
  char sql[120];
  alarm_code= websGetVar(wp, T("ac"), T("0"));
  operate= websGetVar(wp, T("operate"), T("0"));
  if (!gstrcmp(operate,"0"))
  {
     sqlite3 *db;
     //char *zErrMsg = 0;
     int rc;    
     alarmcode=atoi(alarm_code);
     #ifdef DEFAULT_LANG_CN
        sprintf(sql,"SELECT alarm_code,alarm_name,severity,is_send,mask_slot FROM AlarmType WHERE alarm_code=%s",alarm_code);
     #else
        sprintf(sql,"SELECT alarm_code,alarm_name_en,severity,is_send,mask_slot FROM AlarmType WHERE alarm_code= FROM AlarmType WHERE alarm_code=%s",alarm_code);
     #endif
     rc = sqlite3_open(DB_FILE_NAME, &db);  
     
     rc = sqlite3_exec(db, sql, onFetchAlarmType, (void *)wp, NULL); 
  //printf("zErrMsg:%s\n",zErrMsg);
     sqlite3_close(db);
     websRedirect(wp, T("/alm_type_mod.asp"));
  }
  else if (!gstrcmp(operate,"mod"))
  {
      char_t *enabled,*save,*severity,*slots;
      int ena=1,is_save=1,sid;
      enabled=websGetVar(wp,T("enable"),T(""));
      save=websGetVar(wp,T("save"),T(""));
      severity=websGetVar(wp,T("severity"),T(""));
      slots=websGetVar(wp,T("slots"),T(""));
      if (enabled  && (gstrcmp(enabled, T("on"))))
          ena=2;
      if (save && (gstrcmp(save, T("on"))))
          is_save=2;
      sid=atoi(severity);
      if (sid>0 && sid <6)
      {
        short mask=0;
        if (gstrlen(slots)>1)
        {
           int n;
           //short slot;
           char vals[20]={0};
           short value=0;
           int count=strtoca(slots,";",vals,10);
           
           for (n=0;n<count;n++)
           {
             if (vals[n]>0 && vals[n]<16)
             {
              //slot=vals[n];
              value=1;
              value=value<<(15-vals[n]);
              mask=mask|value;
             }
             //if (gstrstr(slots)>1)
           }
        }
        sprintf(sql,"UPDATE AlarmType SET is_send=%d,is_save=%d,severity=%s,mask_slot=%d WHERE alarm_code=%s",ena,is_save,severity,mask,alarm_code);
        //printf("sql:%s\nmask_slots=%s\n",sql,slots);
        execSql(NULL,sql,NULL,NULL);
        insertOperateLog(LOG_GROUP_SYS_SNMP_ALARM_TYPE_CONF,0,0,WebMode,mod,0,0);
      }
      websRedirect(wp, T("alm_type.asp"));
     
     //printf("sql=%s\n",sql);
  }
   // sprintf(sql,"DELETE FROM AlarmLog WHERE ID=%s",sql,aid);
  //printf("sql:%s operate=%s\n",sql,operate);
}
static int aspAlarmShowConf(int eid, webs_t wp, int argc, char_t **argv)
{
   int nBytes=0;
   nBytes=websWrite((webs_t)wp,"var is_show_count=");
   if (show_alarm_count!=2)
       nBytes=websWrite((webs_t)wp,"true;");
   else
       nBytes=websWrite((webs_t)wp,"false;");
   nBytes=websWrite((webs_t)wp,"\nvar is_add_once=");
   if (AlarmConf.is_add_once)
       nBytes=websWrite((webs_t)wp,"true;");
   else
       nBytes=websWrite((webs_t)wp,"false;");
   return nBytes;
}
static int aspAlarmConf(int eid, webs_t wp, int argc, char_t **argv)
{
   int nBytes=0;
   char save_alarm_flag;
   char value[16]="";
   int max_count=300;
   getConfValue(SYS_CONF_FILE,"save_alarms",value," ");
   if (value[0]=='2')
         save_alarm_flag=2;
   else if (value[0]=='0')
         save_alarm_flag=0;
   else 
         save_alarm_flag=1;
   nBytes+=websWrite((webs_t)wp,T("var save_flag=%d;"),save_alarm_flag);
   if (show_alarm_count!=2)
      nBytes+=websWrite((webs_t)wp,"var show_count=true;");
   else
      nBytes+=websWrite((webs_t)wp,"var show_count=false;");
   getConfValue(SYS_CONF_FILE,"max_acount",value," ");
   max_count=atoi(value);
   if (max_count>2000)
      max_count=2000;
   else if (max_count<1)
      max_count=300;
   nBytes+=websWrite((webs_t)wp,T("var max_acount=%d;"),max_count);
   nBytes+=websWrite((webs_t)wp,T("var max_rowcount=%d;"),rows_per_page);
/*
   #ifdef DEFAULT_LANG_CN
   nBytes+=websWrite((webs_t)wp,T("<tr><th>是否保存告警:</th><td><input type='checkbox'id='issave' name='issave' onclick='check();'/></td><td></td></tr>"));
   nBytes+=websWrite((webs_t)wp,T("<tr><th>按条数:</th><td><input type='radio' id='by1' checked=true  name='saveby' onclick='check();'/></td><td><input type='textbox' id ='maxcnt'name='maxcnt' value=20 maxlength='3' size='3' onblur='checkNumber(this,0,500,\"条数\");' size='3'/></td></tr>"));
   nBytes+=websWrite((webs_t)wp,T("<tr><th>按天数:</th><td><input type='radio' id='by2' name='saveby' onclick='check();'/></td><td><input type='textbox' id='days' name='days' value=30 maxlength='3' size='3' onblur='checkNumber(this,0,100,\"天数\");'/></td></tr>"));
   nBytes+=websWrite((webs_t)wp,T("<tr><th>显示条数/页:</th><td></td><td><input type='textbox' value=50 id='showcnt' name='showcnt' onblur='checkNumber(this,5,100,\"条数/页\");' maxlength='3' size='3'/></td></tr>"));
  #else
    nBytes+=websWrite((webs_t)wp,T("<tr><th>Is save?:</th><td><input type='checkbox'id='issave' name='issave' onclick='check();'/></td><td></td></tr>"));
   nBytes+=websWrite((webs_t)wp,T("<tr><th>By count of items:</th><td><input type='radio' id='by1' checked=true  name='saveby' onclick='check();'/></td><td><input type='textbox' id ='maxcnt'name='maxcnt' value=20 maxlength='3' size='3' onblur='checkNumber(this,0,500);' size='3'/></td></tr>"));
   nBytes+=websWrite((webs_t)wp,T("<tr><th>By count of dayes:</th><td><input type='radio' id='by2' name='saveby' onclick='check();'/></td><td><input type='textbox' id='days' name='days' value=30 maxlength='3' size='3' onblur='checkNumber(this,0,100);'/></td></tr>"));
   nBytes+=websWrite((webs_t)wp,T("<tr><th>Show count of items:</th><td></td><td><input type='textbox' value=20 id='showcnt' name='showcnt' onblur='checkNumber(this,5,100);' maxlength='3' size='3'/></td></tr>"));
 #endif
*/
  return nBytes;
}

void alarmAspDefine()
{
  websAspDefine(T("AlarmTypeList"), aspAlarmTypeList);
  websAspDefine(T("AlarmType"), aspAlarmType);
  websFormDefine(T("atype"), formAlarmType);
  websFormDefine(T("atpg"),formAlarmTypeQuery);
  websFormDefine(T("almconf"),formAlarmConf);
  websAspDefine(T("AlarmConf"), aspAlarmConf);
  websAspDefine(T("AlarmShowConf"), aspAlarmShowConf);
}
