#include <string.h>
#include <sys/sysinfo.h>
#include <time.h>
#include "../wsIntrn.h"
#include "../../include/unit.h"
#include <visipc.h>
#include <sqlite3.h>
#include <dbutil.h>
#include <log.h>
#include <unit.h>
#include <otu.h>
#include <olp.h>
#include <sysinfo.h>
#include <chassis.h>
#include <passive.h>
#include <public.h>
#include <visstr.h>
#include "../unitwebs.h"

int aspOtuInfo(webs_t wp, int slot,int uinfo,int utype,int group);
void olpAspDefine();
int aspOtuView(webs_t wp, int slot,uchar_t cclass,uchar_t uclass,int otu_type,char show_property);
int aspOtapView(webs_t wp, int slot,uchar_t cclass,uchar_t uclass,int otu_type,char show_property);
int aspOlpInfo(webs_t wp, int slot,uchar_t uclass,uchar_t utype,int group);
int aspOlpView(webs_t wp, uchar_t slot,uchar_t cclass,uchar_t uclass,uchar_t utype);
int aspEdfaInfo(webs_t wp, int slot,int otu_type);
int aspChassisInfo(webs_t wp,int group);
void ddmAspDefine();
void edfaAspDefine();
int aspPassiveInfo(int eid, webs_t wp, int argc, char_t **argv);
extern int aspInfoset(int eid, webs_t wp, int argc, char_t **argv);
extern void formSetInfoset(webs_t wp, char_t *path, char_t *query);
extern char show_alarm_count;

boolean_t getUnitWebInfo(uchar_t chassis,uchar_t cclass,uchar_t slot,uchar_t uclass,uchar_t utype,web_unit_t *info)
{
  boolean_t ret=FALSE;
  //printf("getUnitWebInfo..uclass=%d\n",uclass);
  switch (uclass)
  {
    case UC_OTU:
    case UC_OEO:
         ret=getOtuWebInfo(chassis,cclass,slot,uclass,utype,info);
      break;
    case UC_OLP:
    case UC_OSS:
         ret=getOlpWebInfo(chassis,cclass,slot,uclass,utype,info);
      break;
    case UC_EDFA:
         ret=getEdfaWebInfo(chassis,cclass,slot,uclass,utype,info);
      break;
    case UC_ETU:
         ret=getEtuWebInfo(chassis,cclass,slot,uclass,utype,info);
      break;
    case UC_DVOA:
         ret=getDvoaWebInfo(chassis,cclass,slot,uclass,utype,info);
      break;
    case UC_PASSIVE:
         ret=getPassiveWebInfo(chassis,cclass,slot,uclass,utype,info);
      break;
    default:
         ret=FALSE;
  }
  /*
  if (ret!=FALSE)
     printf("getUnitWebInfo..uclass=%d OK\n",uclass);
  else
     printf("getUnitWebInfo..uclass=%d Fail\n",uclass);
  */
  return ret;
}

void pageHeader(webs_t wp)
{
  websWrite(wp, T("<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Transitional//EN\" \"http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd\"> \
<html xmlns=\"http://www.w3.org/1999/xhtml\"> \
<head> \
<meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\" /> \
<link rel=\"stylesheet\" href=\"../css/style.css\" type=\"text/css\" /> \
<link rel=\"stylesheet\" href=\"../css/button.css\" type=\"text/css\" /> \
</head>"));
}
void pageMenu(webs_t wp,char *message)
{
  websWrite(wp, T("<ul id=\"nav\"> \
  <li><span>%s=></span></li> \
</ul>"),message);
}

int aspUnitList(int eid, webs_t wp, int argc, char_t **argv)
{
    int n,nBytesSent;
    uchar_t uclass,nmu_slot,max_slot;
    unit_base_info_t *shm_bi;
    unit_user_data_t udata;
    char buf[60];
    a_assert(wp);
    
   nBytesSent = 0;
   nmu_slot=getSlotOfNmu(0,3);
   max_slot=getMaxSlot(0);
   for(n=1;n<=max_slot;n++)
   {
    uclass= getUnitClass(0,n);    
    if (uclass<1)
    {
      //printf("Unit #%d uc=%d\n",n,uclass);
     continue;
    }
    //else
    //    printf("Unit #%d uc=%d\n",n,uclass);
    shm_bi=getBaseInfoMap(0,n);
    if (NULL!=shm_bi)
    {
      if (FALSE!=getUserDataFromDB(0,n,&udata))
         strcpy(buf,udata.name);
      else
          buf[0]='\0';
      nBytesSent +=websWrite(wp, T("<tr><td>#%d</td>\r\n<td>%s</td>\r\n<td>%s</td>\r\n<td>%s</td>\r\n<td>%s</td>\r\n"),n,buf,shm_bi->model,shm_bi->sn,shm_bi->creation);
      
      if (n!=nmu_slot)
      {
        nBytesSent +=websWrite(wp, T("<td>%s</td>\r\n<td>%s</td>"),shm_bi->hwver,shm_bi->fwver);
      }
      else
         nBytesSent +=websWrite(wp, T("<td>%s</td>\r\n<td>%s</td>"),NMU_HW_VER,NMU_FW_VER);
      nBytesSent +=websWrite(wp,"</tr>\r\n");
    }
  }      
  return nBytesSent;
}

static int unit_id,port_index;
int getSelectedUnit()
{
  return unit_id;
}
int getSelectedPort()
{
  return port_index;
}
static void aspUnitUrl(webs_t wp, char_t *path, char_t *query)
{
	//char_t	*chassis;
        char_t	*slot;
        uchar_t uclass,utype=0;
	//chassis = websGetVar(wp, T("cid"), T("0")); 
	slot = websGetVar(wp, T("uid"), T("1"));
        unit_id=atoi(slot);
        getUnitClassType(0,unit_id,&uclass,&utype);
      
        if (uclass==UC_OTU || uclass==UC_OEO)
        {
         /*if (utype==1 || utype==5 || utype==6)
          websRedirect(wp, T("otu.asp"));
         else
          websRedirect(wp, T("otu2.asp"));
         */
         websRedirect(wp, T("otu.asp"));
        }
        else if (uclass==UC_OEO)
        {
         if (utype==1 || utype==5 || utype==6)
          websRedirect(wp, T("oeo.asp"));
         else
          websRedirect(wp, T("oeo2.asp"));
        }
}

/*
static int checkAlarm( void *para, int n_column, char **column_value, char **column_name)
{
  if (para!=NULL)
  {
     int n;
     char *hasalarm=(char*)para;
     n=atoi(column_value[0]);
     if (n>=0 && n <=MAX_UNIT_COUNT)
         hasalarm[n]=1;
  }
  return 0;
}
*/


int aspUnitAlarmView(int eid, webs_t wp,char slotCountOfUnit[])
{
   int n=0,nBytesSent=0;
   int ret;
   char sql[]="SELECT distinct slot FROM AlarmLog WHERE alarm_status<>3 LIMIT 16";
   char has_alarm[MAX_UNIT_COUNT+1]={0};
   char alm_flag=0;
   char slots=16;
   sqlite3 *db;
   sqlite3_stmt* stmt=NULL;
   
/*
   //char *zErrMsg = 0;
   uchar_t *us_shm,nmu_slot=16;
   uchar_t *utype;
 char pre_unit=1;
 //char nextSlotUnit=0,slotUnit=0;
 char slotCountOfUnit[16]={0};
 int slotCount=17;
 us_shm=getUnitStatusMap(0);
 if (NULL==us_shm)
  return 0;
 utype=us_shm+slotCount;//MAX_UNIT_COUNT;
 for (n=16;n>0;n--)
 {
   if (UC_PASSIVE==us_shm[n])
   {
     if (PASSIVE_ODM2_UNIT==utype[n])
       slotCountOfUnit[n-1]=2;
     else if (PASSIVE_DCM4_UNIT==utype[n])
       slotCountOfUnit[n-1]=4;
     else
       slotCountOfUnit[n-1]=1;
   }
   else if (us_shm[n]>0)
      slotCountOfUnit[n-1]=1;
   else
      slotCountOfUnit[n-1]=0;
 }
*/
 //  us_shm=getUnitStatusMap(0);
 //return 0;
  db=openDB(DB_FILE_NAME);
  
  if (NULL==db)
     return 0;
  //printf("aspOperateLog2 sq:%s DB_FILE_NAME=%s\n",sql,DB_FILE_NAME);
  ret=sqlite3_prepare_v2(db,sql,strlen(sql),&stmt,NULL);
  if(ret!=SQLITE_OK)
  {
    //printf("aspUnitAlarmView Error:%s\n",sqlite3_errmsg(db));
    if (stmt)
        sqlite3_finalize(stmt);
    sqlite3_close(db);
    return 0;
  }
  ret=sqlite3_step(stmt);
  while (ret==SQLITE_ROW)
  {
    n=sqlite3_column_int(stmt,0);
    if (n>-1 && n < MAX_UNIT_COUNT)
       has_alarm[n]=1;
    //nBytesSent +=websWrite(wp, T("<div class='uf'></div>\n"));
    ret=sqlite3_step(stmt);
  }
  sqlite3_finalize(stmt);
  sqlite3_close(db);
  for (n=16;n>0;n--)
  {
    if (slots!=n)
       continue;
    if (slotCountOfUnit[n-1]<2)
    {
      slots--;
      nBytesSent +=websWrite(wp, T("<div class='uf1'>"));
    }
    else
    {
      slots-=slotCountOfUnit[n-1];
      nBytesSent +=websWrite(wp, T("<div class='uf%d'>"),slotCountOfUnit[n-1]);
    }
    if (has_alarm[n]>0)
          nBytesSent +=websWrite(wp, T("<a href='/goform/almqry?flag=1&slot=%d'><img src='images/bell.gif'/></a>"),n);
        nBytesSent +=websWrite(wp, T("</div>"));
  }
/*
  for (n=15;n>0;n--)
  {
   if (slotCountOfUnit[n-1]>0)
   {
      for (m=pre_unit;m<(n-slotCountOfUnit[n-1]+1);m++)
      {
        nBytesSent +=websWrite(wp, T("<div class='uf1'>"));
        if (has_alarm[m]>0)
          nBytesSent +=websWrite(wp, T("<a href='/goform/almqry?flag=1&slot=%d'><img src='images/bell.gif'/></a>"),m);
        nBytesSent +=websWrite(wp, T("</div>"));
      }
      nBytesSent +=websWrite(wp, T("<div class='uf%d'>"),slotCountOfUnit[n-1]);
      if (has_alarm[n]>0)
         nBytesSent +=websWrite(wp, T("<a href='/goform/almqry?flag=1&slot=%d'><img src='images/bell.gif'/></a>"),n);
      nBytesSent +=websWrite(wp, T("</div>"));
      pre_unit=n+1;
   }
  }
*/
   return nBytesSent;
}
/*
static uchar_t getUrlParam(webs_t wp, char_t *query,int *chassis,int *slot)
{
  char_t *cp, *sp;
  cp = websGetVar(wp, T("cid"), T("0")); 
  sp = websGetVar(wp, T("uid"), T("0"));
  *chassis=atoi(cp);
  *slot=atoi(sp);
  return 0;
}
*/
void redirect(webs_t wp, char_t *query)
{
  //char_t *chassis;
  char_t *slot,*page;
  //chassis = websGetVar(wp, T("cid"), T("0")); 
  slot = websGetVar(wp, T("uid"), T("0"));
  page = websGetVar(wp, T("page"), T(""));
 //printf("chassis=%s slot=%s\n",chassis,slot);
  if (gstrcmp(slot,"0"))
  {  
    char path[30];
    unit_id=atoi(slot);
    if (!gstrcmp(page,""))
    {
      //page=getUnitName(0,slot,path,1);
      //printf("Chassis=%s slot=%s uid=%d page=%s\n",chassis,slot,unit_id,page);
      if (!getUnitName(0,atoi(slot),path,1))
      {
       //printf("path=%s\n",path);
       sprintf(path, "%s.asp", path);
      }
      else
         strcpy(path, "/ukw_unit.asp");
    }
    else
        sprintf(path, "%s.asp", page);   
    //page=strlwr(page);
    //printf("chassis=%s slot=%s uid=%d page=%s path=%s\n",chassis,slot,unit_id,page,path);
    websRedirect(wp, path);
 }
}

static void formUnit(webs_t wp, char_t *path, char_t *query)
{
  char_t *slot,*page,*lang;

 //chassis = websGetVar(wp, T("cid"), T("0")); 
 slot = websGetVar(wp, T("uid"), T("0"));
 page = websGetVar(wp, T("page"), T(""));
 lang = websGetVar(wp, T("lang"), T("1"));
 //printf("chassis=%s slot=%s\n",chassis,slot);
 //printf("slot=%s page=%s path=%s\n",slot,page,path);
 if (gstrcmp(slot,"0"))
 {  
    char uname[60]="",*un;
    uchar_t cclass,uclass=0,utype=0;

    unit_id=atoi(slot);
    //uclass=getUnitName(0,unit_id,uname,1);
    getUnitClassType(0,unit_id,&uclass,&utype);
    //printf("slot=%s uclass=%d,utype=%d\n",slot,uclass,utype);
    getUnitLabelName(uclass,utype,uname);
    un=strTolower(uname);
    cclass=getChassisClass(0);
    if (uclass && uclass!=UC_PASSIVE)
    {
       //printf("slot=%s uname=%s cclass:%d page:%s\n",slot,un,cclass,page);
       if (!gstrcmp(page,""))
       {
         if (utype<2)
         {
           if (UC_OEO!=uclass)
            sprintf(uname, "%s.asp", un);
           else
             strcpy(uname, "otu.asp");
           //printf("slot=%s uname=%s cclass:%d page:%s\n",slot,un,cclass,page);
         }
         else
         {
            if (UC_OLP!=uclass)
            {
                if ((UC_OTU==uclass || UC_OEO==uclass))
                {
                  strcpy(uname, "otu.asp");
                  /*if (UC_OEO==uclass)
                      strcpy(uname, "otu.asp");
                  else if (utype!=2)
                    sprintf(uname, "%s.asp", un);
                  else
                    sprintf(uname, "%s%d.asp", un,2);
                 */
                }
                else if (UC_OTAP!=uclass)
                     sprintf(uname, "%s.asp", un);             
                else
                  strcpy(uname, "otap.asp");
            }
            else
              strcpy(uname, "olp.asp");
         }
       }
       else
       {
         
         if (gstrstr(page,un))
         {
           //printf("slot=%s un=%s cclass:%d page:%s CC_4U=%d\n",slot,un,cclass,page,CC_4U);
           /*if (CC_4U!=cclass)
           {
              sprintf(uname, "%sh%d.asp", un,utype);
              sprintf(uname, "%s.asp", page);printf("4!=cclass slot=%s uname=%s\n",slot,uname);
           }
           else
           {   sprintf(uname, "%s.asp", page);
             //printf("4==cclass slot=%s uname=%s\n",slot,uname);
           }*/
           sprintf(uname, "%s.asp", page);
           //printf("slot=%s uname=%s cclass:%d page:%s CC_4U=%d\n",slot,uname,cclass,page,CC_4U);
         }
         else if (!gstrcmp(page,"bi"))
         {
           //if (cclass!=CC_4U)
           //   sprintf(uname, "%sh%dbi.asp", un,utype);
           //else
            //  sprintf(uname, "%sbi.asp", un);
            //printf("utype=%d\n",utype);
            if (utype==1)
            {
                if (UC_OEO!=uclass)
                  sprintf(uname, "%sbi.asp", un);
                else
                   strcpy(uname, "otubi.asp");
            }
            else
            {
              if (UC_OLP!=uclass)
              {
                if ((UC_OTU==uclass || UC_OEO==uclass))
                {
                  //if (utype<20)
                    strcpy(uname, "otubi.asp");
                  //else
                  //  strcpy(uname, "otapbi.asp");
                  /*
                  if (UC_OEO==uclass)
                     strcpy(uname, "otubi.asp");
                  else if (utype!=2)
                    sprintf(uname, "%sbi.asp", un);
                  else
                    sprintf(uname, "%s%dbi.asp", un,2);
                  */
                }
                else if (UC_OTAP!=uclass)
                  sprintf(uname, "%sbi.asp", un);
                else
                    strcpy(uname, "otapbi.asp");
              }
              else
               strcpy(uname, "olpbi.asp");
              //printf("uname=%s\n",uname);
            }
         }
         else if (!gstrcmp(page,"otuca"))
           strcpy(uname, "otuca.asp");
         else
          strcpy(uname, "ukw_unit.asp");
       }
    }
    else
    {
     // printf("slot=%s uname=%s utype:%d page:%s\n",slot,un,utype,page);
      if (uclass>0)
      {
       if (utype==3)
           strcpy(uname, "awgbi.asp");
       //else if (PASSIVE_DCM2_UNIT==utype)
        //   strcpy(uname, "dcm2bi.asp");
       else if (PASSIVE_DCM4_UNIT==utype)
           strcpy(uname, "dcm4bi.asp");
       else if (PASSIVE_DCM2_UNIT==utype)
           strcpy(uname, "dcm2bi.asp");
       else if (PASSIVE_ODM12_UNIT==utype)
           strcpy(uname, "odm12bi.asp");
       else if (gstrstr(page,un))
           sprintf(uname, "%s.asp", page);
       else if (!gstrcmp(page,"bi"))
       {
         if (utype!=PASSIVE_ODM3_UNIT)
           sprintf(uname, "%sbi.asp", un);
         else
            strcpy(uname,"bi.asp");
       }
       else  strcpy(uname, "ukw_unit.asp");
      }
      else
          strcpy(uname, "bi.asp");
    }
    //printf("uclass=%d uname=%s\n",uclass,uname);
    websRedirect(wp, uname);
 }
 // return 0;
}

static int aspUnitID(int eid, webs_t wp, int argc, char_t **argv)
{
 //printf("aspUnitID uid=%d\n",unit_id);
 return websWrite(wp, T("%d"),unit_id);
}

static char alarm_list_flag=1;
/*
static int onFetchAlarm( void *wp, int n_column, char **column_value, char **column_name)
{
  //webs_t *wp;
  
   //wp=(webs_t *)para;
  char severity[6][16]={"Unkown","紧急告警", "严重告警","主要告警" ,"次要告警" ,"提示告警"};
  int sid;
  char is_clean=1,is_ack=1;

  websWrite((webs_t)wp,T("<tr><td>"));
  if (strcmp(column_value[0],"0"))
    websWrite(wp,T("单元盘 #%s"),column_value[0]);
    //printf("\t");
  if (strcmp(column_value[1],"0"))
    websWrite(wp,T(" %s %s"),column_value[6],column_value[1]);
    //printf("\t");
  websWrite((webs_t)wp,T("</td><td>%s</td><td>"),column_value[5]);
  if (!strcmp(column_value[3],"0"))
  {
     websWrite((webs_t)wp,T("未清除未确认"));
     is_clean=0;
     is_ack=0;
  }
  else if (!strcmp(column_value[3],"1"))
  {
     websWrite((webs_t)wp,T("已清除未确认"));
     is_ack=0;
  }
  else if (!strcmp(column_value[3],"2"))
  {
      websWrite(wp,T("未清除已确认"));
      is_clean=0;
  }
  else if (!strcmp(column_value[3],"3"))
     websWrite((webs_t)wp,T("已清除已确认"));
  websWrite((webs_t)wp,T("</td><td>%s"),column_value[2]);
 
  sid=atoi(column_value[7]);
  if (sid<1 || sid>5)
      sid=0;
  websWrite((webs_t)wp,T("</td><td class='s%d'>%s"),sid,severity[sid]);
  if ( !is_ack && !is_clean)
     websWrite((webs_t)wp,T("<img class='new' src='images/new.gif'/>"));
  websWrite((webs_t)wp,T("</td>"));   
  if (alarm_list_flag!=2)
  {
  if (!is_clean)
    websWrite((webs_t)wp,T("<td><a href='/goform/alarm?operate=clr&aid=%s'>清除</a>"),column_value[4]);
  else
    websWrite((webs_t)wp,T("<td><img src='images/clr.gif'/>"));
  websWrite((webs_t)wp,T("</td><td >"));
  
  if (!is_ack)
    websWrite((webs_t)wp,T("<a href='/goform/alarm?operate=ack&aid=%s'>确认</a>"),column_value[4]);
  else
    websWrite((webs_t)wp,T("<img src='images/ok.gif'/>"));
  websWrite((webs_t)wp,T("</td></tr>\n"));
  }
  else
  websWrite((webs_t)wp,T("<td colspan='3'><a href='/goform/alarm?operate=del&aid=%s'><img src='images/del.gif'/></a></td></tr>"),column_value[4],column_value[4],column_value[4]);
  return 0;
}
*/
static int order_field=4;
static char order=1;
static char from_date[30]="",end_date[30]="";
static int pageNum=0;
static int selected_slot=-1;
static int and_or=1;
extern int rows_per_page;
static int aspAlarmList(int eid, webs_t wp, int argc, char_t **argv)
{
  //#define MAX_ROW_PER_PAGE 30
  sqlite3 *db;
  sqlite3_stmt* stmt=NULL;
  //char *zErrMsg = 0;
  int rc,nbytes=0,rowcount=0,n;
  char_t *flag;
  char buf[40];
  char const *ptr;
  #ifdef DEFAULT_LANG_CN
  char fields[5][15]={"B.slot","A.alarm_name","B.alarm_status","B.alarm_time","A.severity"};
  char sql[420]="SELECT B.slot,B.entity,B.alarm_time, B.alarm_status,B.ID,B.alarm_code,B.alarm_value,A.alarm_name,A.entity_name,A.severity,B.last_alarm_time,B.clean_time,B.ack_time,B.alarm_count,C.reason FROM AlarmType AS A,AlarmLog AS B,AlarmReason AS C WHERE A.alarm_code=B.alarm_code AND B.reason_code=C.reason_code";

  //select A.*,(ifnull(B.reaso_coden,0)) AS reason from [AlarmLog] A LEFT JOIN [AlarmReason] B ON A.reason_code=B.reason_code
  char severity[6][16]={"Unkown","紧急告警", "主要告警","次要告警" ,"提示告警" ,"信息提示"};
  #else
  char fields[5][20]={"B.slot","A.alarm_name_en","B.alarm_status","B.alarm_time","A.severity"};
  char sql[420]="SELECT B.slot,B.entity,B.alarm_time, B.alarm_status,B.ID,B.alarm_code,B.alarm_value,A.alarm_name_en,A.entity_name_en,A.severity,B.last_alarm_time,B.clean_time,B.ack_time,B.alarm_count,C.reason_en FROM AlarmType AS A,AlarmLog AS B,AlarmReason AS C WHERE A.alarm_code=B.alarm_code AND B.reason_code=C.reason_code";
  char severity[6][16]={"Unkown","critical", "major","minor" ,"waring" ,"clean"};
  #endif
  int sid,page_count=0;
  char is_clean=1,is_ack=1;
  short privilege=umGetUserPrivilege(wp->userName);
  rc = sqlite3_open(DB_FILE_NAME, &db);
  if (NULL==db)
     return 0;
  
  ejArgs(argc, argv, T("%s"), &flag);
  if (!gstrcmp(flag,"1"))
  {
     strcat(sql," AND B.alarm_status<>3");
     alarm_list_flag=1;
     rowcount=getRowCount(db,"SELECT slot FROM AlarmLog WHERE alarm_status<>3");
  }
  else
  {
     strcat(sql," AND B.alarm_status=3");
     alarm_list_flag=2;
     rowcount=getRowCount(db,"SELECT slot FROM AlarmLog WHERE alarm_status=3");
  }
 
 if (gstrlen(from_date)>5)
  {
     strcat(sql," AND B.alarm_time>='");
     strcat(sql,from_date);
     //if (gstrlen(end_date)<12)
     strcat(sql,"'");
  }
  if (gstrlen(end_date)>5)
  {
     strcat(sql," AND B.alarm_time<='");
     strcat(sql,end_date);
     if (gstrlen(end_date)<12)
        strcat(sql," 23:59:59");
     strcat(sql,"'");
     //strcat(sql," 23:59:00'");
  }
  if (selected_slot>-1)
  {
    //if (and_or==1)
      sprintf(buf," AND slot= %d",selected_slot);
    //else
      //sprintf(buf," OR slot= %d",selected_slot);
    strcat(sql,buf);
  }
  
  //page_count=(rowcount + MAX_ROW_PER_PAGE -1) / MAX_ROW_PER_PAGE;
  if (order_field>0 && order_field<6)
  {
     sprintf(buf," ORDER BY %s",fields[order_field-1]);
     strcat(sql,buf);
     if (order!=1)
        strcat(sql," desc");
  }
  else
      strcat(sql," ORDER BY alarm_time desc");
 //printf("sql:%s\n",sql);
  /*
  if (!gstrcmp(order,"2"))
     strcat(sql," desc");
  item_number=atoi(item);
  if (item_number>0 && item_number<6)
  {
     sprintf(buf," ORDER BY %s",order_field[item_number-1]);
     strcat(sql,buf);
  }
  if (!gstrcmp(order,"2"))
     strcat(sql," desc");
  */
  sprintf(buf," LIMIT %d",rows_per_page);
  strcat(sql,buf);
  sprintf(buf," OFFSET %d",pageNum*rows_per_page);
  strcat(sql,buf);
  //printf("%s\n",sql);
  rc=sqlite3_prepare_v2(db,sql,strlen(sql),&stmt,NULL);
  if(rc!=SQLITE_OK)
  {
     if (stmt)
        sqlite3_finalize(stmt);
     sqlite3_close(db);
     return 0;
  }
  //printf("sendSnmpTrap..:%s\n",sql);
  rc=sqlite3_step(stmt);
 
  while (rc==SQLITE_ROW)
  {
    is_clean=1;
    is_ack=1;
    //rowcount++;
    //doSystem("snmptrap -v 2c -c public %s:%d ' ' 1.3.6.1.4.1.10215.2.1.9.%d.%d.%d.%d 1.3.6.1.4.1.0 s '%s'",sqlite3_column_text(stmt,0),sqlite3_column_int(stmt,1),alarm_code,chassis,slot,entity,sqlite3_column_text(stmt,2));
    nbytes+=websWrite((webs_t)wp,T("<tr><td>"));
  if (0!=sqlite3_column_int(stmt,0))
    #ifdef DEFAULT_LANG_CN
    nbytes+=websWrite(wp,T("<a href='/goform/slot?cid=0&uid=%d&page=bi'>第%d槽</a>"),sqlite3_column_int(stmt,0),sqlite3_column_int(stmt,0));
    #else
    nbytes+=websWrite(wp,T("<a href='/goform/slot?cid=0&uid=%d&page=bi'>Slot #%d</a>"),sqlite3_column_int(stmt,0),sqlite3_column_int(stmt,0));
    #endif
    //printf("\t");
  if (0!=sqlite3_column_int(stmt,1))
  {
   nbytes+=websWrite(wp,T(" %s %d"),sqlite3_column_text(stmt,8),sqlite3_column_int(stmt,1));
  }
    //printf("\t");
  nbytes+=websWrite((webs_t)wp,T("</td><td>%s"),sqlite3_column_text(stmt,7));
  //if (sqlite3_column_int(stmt,5)==409 ||sqlite3_column_int(stmt,5)==411 ||sqlite3_column_int(stmt,5)==413)
  if (sqlite3_column_text(stmt,6)!=NULL && strlen(sqlite3_column_text(stmt,6))>0)
   nbytes+=websWrite(wp,T("(%s)"),sqlite3_column_text(stmt,6));
  nbytes+=websWrite((webs_t)wp,"</td><td>");

  if (0==sqlite3_column_int(stmt,3))
  {
     #ifdef DEFAULT_LANG_CN
     nbytes+=websWrite((webs_t)wp,T("未清除未确认"));
     #else
     nbytes+=websWrite((webs_t)wp,T("Unclean&UnAck"));
     #endif
     is_clean=0;
     is_ack=0;
  }
  else if (1==sqlite3_column_int(stmt,3))
  {
     #ifdef DEFAULT_LANG_CN
     nbytes+=websWrite((webs_t)wp,T("已清除未确认"));
     #else
     nbytes+=websWrite((webs_t)wp,T("Cleaned & UnAck"));
     #endif
     is_ack=0;
  }
  else if (2==sqlite3_column_int(stmt,3))
  {
      #ifdef DEFAULT_LANG_CN
      nbytes+=websWrite(wp,T("未清除已确认"));
      #else
      nbytes+=websWrite(wp,T("Unclean & Ack"));
      #endif
      is_clean=0;
  }
  else if (3==sqlite3_column_int(stmt,3))
  #ifdef DEFAULT_LANG_CN
     nbytes+=websWrite((webs_t)wp,T("已清除已确认"));
  #else
     nbytes+=websWrite((webs_t)wp,T("cleaned & Ack"));
  #endif
   if ( !is_ack && !is_clean)
     nbytes+=websWrite((webs_t)wp,T("<img class='new' src='images/new.gif'/>"));
  nbytes+=websWrite((webs_t)wp,T("</td><td>%s"),sqlite3_column_text(stmt,2));
  
  nbytes+=websWrite((webs_t)wp,T("</td><td>"));
  if (show_alarm_count!=2)//最后发生告警的时间
  {
   ptr=sqlite3_column_text(stmt,10);
   if (ptr!=NULL)
     nbytes+=websWrite((webs_t)wp,T("%s"),ptr);
   nbytes+=websWrite((webs_t)wp,T("</td><td>"));
  }
  
  //告警清除时间
  ptr=sqlite3_column_text(stmt,11);
  if (ptr!=NULL)
     nbytes+=websWrite((webs_t)wp,T("%s"),ptr);
  nbytes+=websWrite((webs_t)wp,T("</td><td>"));

  ptr=sqlite3_column_text(stmt,12);
  if (ptr!=NULL)
     nbytes+=websWrite((webs_t)wp,T("%s"),ptr);
  //printf("show_alarm_count=%d\n",show_alarm_count);
  if (show_alarm_count!=2) //告警次数
    nbytes+=websWrite((webs_t)wp,T("</td><td>%d"),sqlite3_column_int(stmt,13));
  sid=sqlite3_column_int(stmt,9);
  if (sid<1 || sid>5)
      sid=0;
  nbytes+=websWrite((webs_t)wp,T("</td><td class='s%d'>%s"),sid,severity[sid]);
  nbytes+=websWrite((webs_t)wp,T("</td>")); 
  
  nbytes+=websWrite((webs_t)wp,T("<td>%s"),sqlite3_column_text(stmt,14));
  
  nbytes+=websWrite((webs_t)wp,T("</td>"));
 
  if (alarm_list_flag!=2)
  {
  if (!is_clean)
  {
   if (privilege!=1)
   {
   #ifdef DEFAULT_LANG_CN
    nbytes+=websWrite((webs_t)wp,T("<td><a href='/goform/alarm?operate=clr&aid=%d'>清除</a>"),sqlite3_column_int(stmt,4));
   #else
     nbytes+=websWrite((webs_t)wp,T("<td><a href='/goform/alarm?operate=clr&aid=%d'>Clean</a>"),sqlite3_column_int(stmt,4));
   #endif
   }
  }
  else
    nbytes+=websWrite((webs_t)wp,T("<td><img src='images/clr.gif'/>"));
  nbytes+=websWrite((webs_t)wp,T("</td><td >"));
  
  if (!is_ack)
  {
   if (privilege!=1)
   {
   #ifdef DEFAULT_LANG_CN
    nbytes+=websWrite((webs_t)wp,T("<a href='/goform/alarm?operate=ack&aid=%d'>确认</a>"),sqlite3_column_int(stmt,4));
   #else
    nbytes+=websWrite((webs_t)wp,T("<a href='/goform/alarm?operate=ack&aid=%d'>Confirm</a>"),sqlite3_column_int(stmt,4));
   #endif
   }
  }
  else
    nbytes+=websWrite((webs_t)wp,T("<img src='images/ok.gif'/>"));
  nbytes+=websWrite((webs_t)wp,T("</td></tr>\n"));
  }
  else
  {
    if (privilege!=1)
       nbytes+=websWrite((webs_t)wp,T("<td colspan='3'><a href='/goform/alarm?operate=del&aid=%d'><img src='images/del.gif'/></a></td></tr>"),sqlite3_column_int(stmt,4));
    else
        nbytes+=websWrite((webs_t)wp,T("<td colspan='3'></td></tr>")); 
  }
  rc=sqlite3_step(stmt);
  }
  sqlite3_finalize(stmt);
  sqlite3_close(db);
  //rc = sqlite3_exec(db, sql, onFetchAlarm, (void *)wp, &zErrMsg); 
  //printf("sql:%s\nzErrMsg:%s flag=%s len=%d dlen=%d\n",sql,zErrMsg,flag,strlen(sql),strlen(from_date));
 // sqlite3_close(db);
 page_count=(rowcount + rows_per_page -1) / rows_per_page;
 #ifdef DEFAULT_LANG_CN
  nbytes+=websWrite(wp, T("<tr><td colspan='12'>告警数:共%d条 共%d页&nbsp;GO->"),rowcount,page_count);
#else
   nbytes+=websWrite(wp, T("<tr><td colspan='12'>Records:%d %d Pages&nbsp;GO->"),rowcount,page_count);
#endif 
  for (n=0;n<page_count;n++)
  {
   #ifdef DEFAULT_LANG_CN
   if (pageNum!=n)
    nbytes+=websWrite(wp, T("<a style='margin-left:5px;' href =\"#\" onclick='query(3,0,0,%d);'>第%d页</a>"),n,n+1);
   else
       nbytes+=websWrite(wp, T("&nbsp;&nbsp;第%d页"),n+1);
   #else
   if (pageNum!=n)
    nbytes+=websWrite(wp, T("<a style='margin-left:5px;' href =\"#\" onclick='query(3,0,0,%d);'>Page %d</a>"),n,n+1);
   else
       nbytes+=websWrite(wp, T("&nbsp;&nbsp;Page %d"),n+1);
   #endif
  }
  nbytes+=websWrite(wp, T("</td></tr><tr><td colspan='12'><INPUT TYPE='hidden' id='fdate' value='%s'/><INPUT TYPE='hidden' id='edate' value='%s'/><INPUT TYPE='hidden' id='pg' value='%d'/></td></tr>"),from_date,end_date,pageNum);
 from_date[0]='\0';
 end_date[0]='\0';
 pageNum=0;
 selected_slot=-1;
 and_or=1;
 return nbytes;
 //return websWrite(wp, T("当前告警信息"));
}

static void formAlarmQuery(webs_t wp, char_t *path, char_t *query)
{
  char_t *flag,*item,*porder,*pg,*slot,*ao;
  
  flag= websGetVar(wp, T("flag"), T("1"));
  item= websGetVar(wp, T("field"), T("0"));
  porder= websGetVar(wp, T("order"), T("0"));
  pg= websGetVar(wp, T("pg"), T("0"));
  slot= websGetVar(wp, T("slot"), T("-1"));
  ao= websGetVar(wp, T("ao"), T("1"));
  gstrcpy(from_date, websGetVar(wp, T("fd"), T("")));
  gstrcpy(end_date, websGetVar(wp, T("ed"), T("")));
  order_field=atoi(item);
  order=atoi(porder);
  pageNum=atoi(pg);
  selected_slot=atoi(slot);
  and_or=atoi(ao);
  if (gstrcmp(flag,"2"))
     websRedirect(wp, T("alarm_list.asp"));
  else
     websRedirect(wp, T("alarm_his.asp"));
}
static void formAlarm(webs_t wp, char_t *path, char_t *query)
{
  char_t *aid,*slot,*operate,*fdate,*edate;
  char sql[200]="";
  char where[100]="",dt[100]="";
  //int unit;
  
  aid = websGetVar(wp, T("aid"), T(""));
  operate= websGetVar(wp, T("operate"), T("0"));
  slot= websGetVar(wp, T("slot"), T(""));
  if (!gstrcmp(aid,""))
  {
     fdate= websGetVar(wp, T("fd"), T(""));
     edate= websGetVar(wp, T("ed"), T(""));
     if (gstrcmp(fdate,""))
     {
        //sprintf(buf," alarm_time>='%s'",fdate);
        if (gstrcmp(edate,""))
           sprintf(dt," alarm_time>='%s' AND alarm_time<='%s'",fdate,edate);
        else
           sprintf(dt,"alarm_time>='%s'",fdate);
     }
     else if (gstrcmp(edate,""))
         sprintf(dt," alarm_time<='%s'",edate); 
  }
  if (!gstrcmp(operate,"clr"))
  {
      strcpy(sql,"UPDATE AlarmLog SET alarm_status=alarm_status|1,clean_time=datetime('now','localtime')");
      
      if (!gstrcmp(aid,""))
      {
         if (strstr(dt,"alarm_time"))
         {
           sprintf(where," WHERE %s",dt);
           strcat(sql,where);
         }
       /* if (gstrcmp(fdate,""))
          {
          sprintf(where," alarm_time>='%'",fdate);
          if (gstrcmp(fdate,""))
           sprintf(where," AND alarm_time<='%'",edate);
          }
        else sprintf(where," alarm_time<='%'",edate);
          */       
      }
      else if (gstrcmp(aid,"0"))
      {
        sprintf(where," WHERE ID=%s",aid);
        strcat(sql,where);
       }
          
     insertOperateLog(LOG_GROUP_SYS_CURRENT_ALARM_CONF,0,0,WebMode,clr,0,0);
  }
  else if (!gstrcmp(operate,"ack"))
  {
    strcpy(sql,"UPDATE AlarmLog SET alarm_status=alarm_status|2,ack_time=datetime('now','localtime')");
    if (!gstrcmp(aid,""))
     {
       if (strstr(dt,"alarm_time"))
       {
         sprintf(where," WHERE %s",dt);
         strcat(sql,where);
       }
       //printf("sql:%s\noperate=acl\n",sql);
      /*
       sprintf(where," WHERE ");
        if (gstrcmp(fdate,""))
          {
          sprintf(where," alarm_time>='%'",fdate);
          if (gstrcmp(fdate,""))
           sprintf(where," AND alarm_time<='%'",edate);
          }
        else sprintf(where," alarm_time<='%'",edate);
        */        
     }
    else if (gstrcmp(aid,"0"))
     {
       sprintf(where," WHERE ID=%s",aid);
       strcat(sql,where);
     }
     insertOperateLog(LOG_GROUP_SYS_CURRENT_ALARM_CONF,0,0,WebMode,ack,0,0);
     //printf("sql=%s\n",sql);
  }
  else if (!gstrcmp(operate,"del"))
  {
     strcpy(sql,"DELETE FROM AlarmLog WHERE alarm_status=3");
     if (!gstrcmp(aid,""))
     {
        if (strstr(dt,"alarm_time"))
        {
          sprintf(where," AND %s",dt);
          strcat(sql,where);
        }
        //printf("sql=%s\n",sql);
        /*sprintf(where," WHERE ");
        if (gstrcmp(fdate,""))
          {
          sprintf(where," alarm_time>='%'",fdate);
          if (gstrcmp(fdate,""))
           sprintf(where," AND alarm_time<='%'",edate);
          }
        else sprintf(where," alarm_time<='%'",edate);
          */    
       }
      else if (gstrcmp(aid,"0"))
      {
        sprintf(where," AND ID=%s",aid);
        strcat(sql,where);
     }
     insertOperateLog(LOG_GROUP_SYS_HISTORY_ALARM_CONF,0,0,WebMode,del,0,0);
     //printf("sql=%s\n",sql);
  }
  if (gstrcmp(slot,""))
  {
     char buf[30]="";
     if (strstr(sql,"WHERE"))
     {
       sprintf(buf," AND slot=%s",slot);
     }
     else
     {
       sprintf(buf," WHERE slot=%s",slot);
     }
     strcat(sql,buf);
  }
   // sprintf(sql,"DELETE FROM AlarmLog WHERE ID=%s",sql,aid);
  //printf("sql:%s \nlen=%d operate=%s slot=%s\n",sql,strlen(sql),operate,slot);
  //sql_exec(NULL,sql);
  execSql(NULL, sql,NULL,NULL);
  if (!gstrcmp(operate,"del"))
     websRedirect(wp, T("alarm_his.asp"));
  else
     websRedirect(wp, T("alarm_list.asp"));
}

static int userDataWrite(webs_t wp,uchar_t chassis,uchar_t slot)
{ 
  unit_user_data_t udata;
  int nBytes=0;
  if (TRUE!=getUserDataFromDB(0,slot,&udata))
      return -1;
  //printf("name=%s ,userdata=%s\n",udata.name,udata.userdata);
  #ifdef DEFAULT_LANG_CN
  if (slot <1)
    return 0;
  nBytes+=websWrite(wp, T("<tr>\
  <th>单元盘名称</th>\n\
  <td><input name=\"uname\" size='30' maxlength='30' value=\"%s\"/></td>\
  <th>用户自定义数据</th>\n\
  <td><input name=\"udata\" size='30' maxlength='30' value=\"%s\" /></td>\
  </tr>"),udata.name,udata.userdata);
  nBytes+=websWrite(wp, T("<tr> \
  <th>单元盘位置</th> \
  <td><input name=\"uloc\" size='30' maxlength='30' value=\"%s\"/></td>\
  <th>上联设备</th> \
  <td><input name=\"updev\" size='30' maxlength='30' value=\"%s\"/></td>\
  </tr>\n"),udata.location,udata.updev);

  nBytes+=websWrite(wp, T("<tr>\
  <th>用户联系</th>\
  <td><input name=\"ucon\" size='30' maxlength='30' value=\"%s\" /></td>\
  <th>开通的业务</th>\
  <td><input name=\"userv\" size='30' maxlength='30' value=\"%s\" /></td>\
  </tr>"),udata.contact,udata.service);
  #else
      nBytes+=websWrite(wp, T("<tr>\
  <th>Unit name</th>\n\
  <td><input name=\"uname\" size='30' maxlength='30' value=\"%s\"/></td>\
  <th>User data</th>\n\
  <td><input name=\"udata\" size='30' maxlength='30' value=\"%s\" /></td>\
  </tr>"),udata.name,udata.userdata);
  nBytes+=websWrite(wp, T("<tr> \
  <th>Location</th> \
  <td><input name=\"uloc\" size='30' maxlength='30' value=\"%s\"/></td>\
  <th>Up device</th> \
  <td><input name=\"updev\" size='30' maxlength='30' value=\"%s\"/></td>\
  </tr>\n"),udata.location,udata.updev);

  nBytes+=websWrite(wp, T("<tr>\
  <th>Contact</th>\
  <td><input name=\"ucon\" size='30' maxlength='30' value=\"%s\" /></td>\
  <th>Open service</th>\
  <td><input name=\"userv\" size='30' maxlength='30' value=\"%s\" /></td>\
  </tr>"),udata.contact,udata.service);
  #endif
   return nBytes;
}


static int aspUserData(int eid, webs_t wp, int argc, char_t **argv)
{
     //char_t   *dbname, *tblname;
     //char sql[100]="";
     //sqlite3 *db;
     
    // char *zErrMsg = 0;
     char_t *slot;
     //uchar_t max_slot;
     int unit;
     //unit_user_data_t udata;
     int nBytes=0;

     //max_slot=getMaxSlot(0);
     if (ejArgs(argc, argv, T("%s"), &slot)>0)
     {
      //printf("slot=%s Unit=%d..!\n",slot,unit);
      if (!gstrcmp(slot,"16"))       
          unit=getSlotOfNmu(0,3);
      else if (!gstrcmp(slot,"0"))
          unit=0;
      else
          unit=unit_id;
     }
     else
          unit=unit_id;
     //printf("unit=%d\n",unit);
     nBytes=userDataWrite(wp,0,unit);
     if (nBytes<0)
       return websWrite(wp, T("<tr><td>Unit #%d not found!</td></tr>"),unit);
     if (unit<MAX_UNIT_COUNT && getUnitClass(0,unit)<1)
        return nBytes;
     //printf("slot=%s Unit=%d..\n",slot,unit);
     if (unit>=0 && unit!=getSlotOfNmu(0,3))
     {
     unit_base_info_t *bi;
     bi=getBaseInfoMap(0,unit);
     if (bi!=NULL)
     {
     struct timeval endtime;
     time_t utime;
     //char uptime[50];
     gettimeofday(&endtime,NULL);
     utime=endtime.tv_sec-bi->uptime;
     //sprintf(uptime,"%ld 小时%ld分%ld秒",utime/3600,utime%3600/60,utime%60);
     #ifdef DEFAULT_LANG_CN
     if (unit>0)
     {
     nBytes+=websWrite(wp, T("<tr> \
  <th>单元盘型号</th> \
  <td>%s</td> \
  <th>单元盘序列号</th> \
  <td>%s</td> \
  </tr>"),bi->model,bi->sn);
     }
     else
     {
     char_t sysname[60]="";
     getConfValue(SYS_CONF_FILE,"sysname",sysname," ");
     nBytes+=websWrite(wp, T("<tr> \
  <th>系统名称</th> \
  <td colspan=\"3\"><input id='sysname' name='sysname' size='30' maxlength='30' value='%s'</td></tr>"),sysname);
     nBytes+=websWrite(wp, T("<tr> \
  <th>背板型号</th> \
  <td>%s</td> \
  <th>背板序列号</th> \
  <td>%s</td> \
  </tr>"),bi->model,bi->sn);
     }
  nBytes+=websWrite(wp, T("<tr> \
  <th>硬件版本</th> \
  <td>%s</td> \
  <th>固件版本</th> \
  <td>%s</td> \
  </tr>"),bi->hwver,bi->fwver);
  nBytes+=websWrite(wp, T("<tr> \
  <th>创建日期</th> \
  <td>%s</td> \
  <th></th> \
  <td></td> \
  </tr>"),bi->creation);
  #else
  nBytes+=websWrite(wp, T("<tr> \
  <th>Model</th> \
  <td>%s</td> \
  <th>Serial number</th> \
  <td>%s</td> \
  </tr>"),bi->model,bi->sn);
 
  nBytes+=websWrite(wp, T("<tr> \
  <th>Hareware ver</th> \
  <td>%s</td> \
  <th>Firmware ver</th> \
  <td>%s</td> \
  </tr>"),bi->hwver,bi->fwver);
  nBytes+=websWrite(wp, T("<tr> \
  <th>Creation date</th> \
  <td>%s</td> \
  <th></th> \
  <td></td> \
  </tr>"),bi->creation);
  #endif
  /*
  nBytes+=websWrite(wp, T("<tr> \
  <th>创建日期</th> \
  <td>%s</td> \
  <th>运行时间</th> \
  <td>%ld 小时%ld分%ld秒</td> \
  </tr>"),bi->creation,utime/3600,utime%3600/60,utime%60);
  */
  }
 }
 else //if (unit==16)
 {
   unit_base_info_t *nmu_bi;
   nmu_bi=getBaseInfoMap(0,getSlotOfNmu(0,3));
   //printf("unit==16\n");
   //getNmuBaseInfo(0,&nmu_bi);
   if (NULL!=nmu_bi)
   //if (getNmuBaseInfo(0,&nmu_bi))
   {
   // printf("nmu_bi->model:%s\n",nmu_bi->model);
  #ifdef DEFAULT_LANG_CN
   nBytes+=websWrite((webs_t)wp, T("<tr> \
  <th>网管卡型号</th> \
  <td>%s</td> \
  <th>网管卡序列号</th> \
  <td>%s</td> \
  </tr>"),nmu_bi->model,nmu_bi->sn);
 
  nBytes+=websWrite(wp, T("<tr> \
  <th>硬件版本</th> \
  <td>%s</td> \
  <th>固件版本</th> \
  <td>%s</td> \
  </tr>"),NMU_HW_VER,NMU_FW_VER/*nmu_bi->hwver,nmu_bi->fwver*/);
  
  nBytes+=websWrite(wp, T("<tr> \
  <th>创建日期</th> \
  <td>%s</td> \
  <th>最后修改日期</th> \
  <td>%s</td> \
  </tr>"),nmu_bi->creation,LAST_MODIFY_DATE);
  #else
      nBytes+=websWrite((webs_t)wp, T("<tr> \
  <th>Model</th> \
  <td>%s</td> \
  <th>Serial number</th> \
  <td>%s</td> \
  </tr>"),nmu_bi->model,nmu_bi->sn);
 
  nBytes+=websWrite(wp, T("<tr> \
  <th>Hardware version</th> \
  <td>%s</td> \
  <th>Firmware version</th> \
  <td>%s</td> \
  </tr>"),NMU_HW_VER,NMU_FW_VER/*nmu_bi->hwver,nmu_bi->fwver*/);
  
  nBytes+=websWrite(wp, T("<tr> \
  <th>Creation date</th> \
  <td>%s</td> \
  <th>Last Modified</th> \
  <td>%s</td> \
  </tr>"),nmu_bi->creation,LAST_MODIFY_DATE);
  #endif
  }
 // else
  //    printf("NULL==nmu_bi\n");
 }
 return nBytes;
}

static int aspUnitClassType(int eid, webs_t wp, int argc, char_t **argv)
{
  uchar_t uclass=4,utype=1;
  char_t slot=0;
  char_t *pslot;
  
  if (ejArgs(argc, argv, T("%s"), &pslot)>0) {
     slot=(char_t)atoi(pslot);
     if (slot>0)
         slot=unit_id;
  }
  else
      slot=unit_id;
  //printf("selected_slot=%d slot=%d uclass=%d ,utype=%d \n",unit_id,slot,uclass,utype);
  if (slot>0)
  {
   getUnitClassType(0,slot,&uclass,&utype);
   //printf("slot=%d uclass=%d ,utype=%d \n",slot,uclass,utype);
   return websWrite(wp, T("uclass=%d,utype=%d,maxslot=%d;\n"),uclass,utype,getSlotOfNmu(0,2));
  }
  else if (slot==0)
  {
    int bytes;
    char_t slot=0,n,m=0;
    slot=getSlotOfNmu(0,3);
    getUnitClassType(0,0,&uclass,&utype);
    websWrite(wp, T("var cclass=%d,ctype=%d,maxslot=%d;\n"),uclass,utype,slot);
    bytes=websWrite(wp, T("var us= new Array()\n"));
    for (n=0;n<slot;n++)
    {
       uclass=0;
       utype=0;
       getUnitClassType(0,n,&uclass,&utype);
       bytes=websWrite(wp, T("us[%d]= new Object();\nus[%d].uclass=%d;us[%d].utype=%d;\n"),n,n,uclass,n,utype);
    }
    return bytes;
  }
}
 
/**************************************************************
*功能：获取板卡功能信息
*格式：UInfo(3,2,2,1);
*说明：第一个参数为板卡大类，可以有1到多个，之间用’；‘隔开；第二个参数为板卡小类，
*第三个参数为信息集ID，第四个参数为信息集索引号，可以有多个，第五个参数为信息项数据类型，
1为字节型，2为双节型，*第六个参数为信息项ID
例子：
UInfo(3;10,1;2,#51#1;2#1#1-5;6;8;10-12|#52#1;2#1#1-5;6;8;10-12);

***************************************************************/
static int aspUInfo(int eid, webs_t wp, int argc, char_t **argv)
{
   int nBytesSent=0,uclass,utype,group,ui;
   char_t	*uc,*ut,*grp,*uinfo;
   char seps[]="&";
   char *token;
   char name[16]="",value[16]="";
   short slot=0,port=0;
   
   if (ejArgs(argc, argv, T("%s"),&uinfo) < 1) {
	websError(wp, 400, T("Insufficient args\n"));
	return -1;
   }
   token = strtok(uinfo, seps );
   while( token != NULL)
   {
    /* while there are tokens in "string" */
    if (strstr(token,"slot"))
    {
        token = strtok(token, "=" );
        if( token != NULL)
        {
            token = strtok( NULL, "=" );
            if( token != NULL)
            slot=atoi(token);
        }
    }
    //sscanf(token,"%*s%s",value);
    //printf( " %s\n", token );
    /* get next token: */
    token = strtok( NULL, seps );
   }
   sscanf("slot=1&port=2","%i&%d",&slot,&port);
   //printf("slot=#%d port=%d name:%s value:%s\n",slot,port,name,value);
   /*uclass=atoi(uc);
   if (uclass>0)
      slot=unit_id;
   else
      slot=0;
   utype=atoi(ut);
   ui=atoi(uinfo);//单元盘功能模块
   */
   nBytesSent+=websWrite(wp, T("var retcode=0,slot=%d,uc=%d,ut=%d;\n"),getRetCode(),slot,uclass,1);
   return nBytesSent;
}

static int aspUnitInfo(int eid, webs_t wp, int argc, char_t **argv)
{
   int nBytesSent=0,uclass,utype,group,ui;
   char_t	*puc,*put,*grp,*uinfo;
   uchar_t slot,uc,ut;
   //char rx[2][10]={"有光","无光"},tx[2][10]={"有光","无光"};
  
   if (ejArgs(argc, argv, T("%s %s %s %s"), &puc,&put,&uinfo,&grp) < 1) {
	websError(wp, 400, T("Insufficient args\n"));
	return -1;
   }

   uclass=atoi(puc);
   if (uclass>0)
      slot=unit_id;
   else
      slot=0;
   utype=atoi(put);
   group=atoi(grp);
   if (uclass>1)
   {
   if (getUnitClassType(0,slot,&uc,&ut)!=TRUE)
   {
     //websRedirect(wp, "ukw_unit.asp");
     nBytesSent=websWrite(wp, T("Unit #%d Not exist!"),slot);
     return nBytesSent;
   }
   if (uc!=(uclass & 0xff) && UC_OTU!=uclass)
   {
     nBytesSent=websWrite(wp, T("Unit #%d Unkown!!"),slot);
     return nBytesSent;
   }
   }
   //printf("Unit #%d uclass=%d utype=%d\n",slot,uclass,utype);
   ui=atoi(uinfo);//单元盘功能模块
   if (uclass==UC_OTU || uclass==UC_OEO)
   {
      return aspOtuInfo(wp,slot,ui,ut,group);
   }
   else if (uclass==UC_OTAP)
   {
     nBytesSent=aspOtuInfo(wp,slot,ui,ut,group);;
     nBytesSent+=aspOtapInfo(wp,slot,uc,ut, group);
     return nBytesSent;
   }
   else if (uclass==UC_OLP || uclass==UC_OSS)
   {
     group=atoi(grp);
     return aspOlpInfo(wp,slot,uc,ut, group);
   }
   else if (uclass==UC_EDFA)
   {
     return aspEdfaStatus(wp,slot);
   }
   else if (uclass==UC_ETU)
   {
     return aspEtuStatus(wp,slot);
   }
   else if (uclass==UC_DVOA)
   {
     return aspDvoaStatus(wp,slot);
   }
   else if (uclass==UC_NMU)
   {
     unit_base_info_t *bi;
     time_t now;
     struct tm  *local;
     long updays, uphours, upminutes;
     struct sysinfo info;
     char buf[60]="";
     //time(&now);
    //local = localtime(&now);
    //return nBytesSent;
     //status_infoset_t *sis;
     /*getNmuBaseInfo(0,&nmubi);
     printf("NMU startup Time: %s",ctime(&nmubi.uptime));

     bi=getBaseInfoMap(0,4);
     printf("Unit 4 startup Time: %s",ctime(&bi->uptime));
     */
     bi=getBaseInfoMap(0,getSlotOfNmu(0,3));
     //printf("NMU startup Time: %s",ctime(&bi->uptime));
     //printf("Here1..!\n");
    
     if (bi!=NULL)
     {
       chassis_t *chassis_info;
       char  temp[10]="0";
       chassis_info=getChassisInfo(0);
       
       if (chassis_info!=NULL)
       {
         sprintf(temp,"%.1f",chassis_info->temp*0.1);
       }
      // printf("temp=%s\n",temp);
    //return nBytesSent;
    time(&now);
    local = localtime(&now);
    //return nBytesSent;
    //printf("%4d-%02d-%02d %02d:%02d:%02d\n",local->tm_year+1900,local->tm_mon+1,local->tm_mday,local->tm_hour,local->tm_min,local->tm_sec);
    //return nBytesSent;
   
   if (NULL!=local)
    {
    /*
    #ifdef DEFAULT_LANG_CN
    nBytesSent=websWrite(wp, T("<tr> \
    <th>系统日期/时间</th> \
    <td><input type=\"text\" readonly=\"true\" class=\"Wdate\" size=\"21\" id='dt' name='dt' value='%4d-%02d-%02d %02d:%02d:%02d' onClick=\"WdatePicker({isShowClear:false,dateFmt:'yyyy-MM-dd HH:mm:ss'});\"/></td> \
    "),local->tm_year+1900,local->tm_mon+1,local->tm_mday,local->tm_hour,local->tm_min,local->tm_sec);
    #else
        nBytesSent=websWrite(wp, T("<tr> \
    <th>System date and time</th> \
    <td><input type=\"text\" readonly=\"true\" class=\"Wdate\" size=\"21\" id='dt' name='dt' value='%4d-%02d-%02d %02d:%02d:%02d' onClick=\"WdatePicker({lang:'en',isShowClear:false,dateFmt:'yyyy-MM-dd HH:mm:ss'});\"/></td> \
    "),local->tm_year+1900,local->tm_mon+1,local->tm_mday,local->tm_hour,local->tm_min,local->tm_sec);
    #endif
    */
      int ts=getConfIntValue(SYS_CONF_FILE,"timesource"," ");
      char ntpserver[60]="";
      getConfValue(SYS_CONF_FILE,"ntpserver",ntpserver," ");

      if (ts<1)
          ts=1;
      else if (ts>2)
          ts=2;
      if (strlen(ntpserver)<2)
         strcpy(ntpserver,"202.112.10.60");
      nBytesSent=websWrite(wp, T("var timemode=%d,ntpserver='%s';\n"),ts,ntpserver);
      nBytesSent=websWrite(wp, T("var sysdate='%4d-%02d-%02d %02d:%02d:%02d';\n"),local->tm_year+1900,local->tm_mon+1,local->tm_mday,local->tm_hour,local->tm_min,local->tm_sec);
    }
    //return nBytesSent;
    //printf("RUN HERE1\n");
    //return nBytesSent;
    
    //sysinfo(&info);
    //printf("RUN HERE2\n");
    //updays = (long) (info.uptime / (60*60*24));
    //gettimeofday(&tpstart,NULL);
    updays =(now-bi->uptime)/86400;
    //printf("RUN HERE3\n");
    if (updays)
    {
      #ifdef DEFAULT_LANG_CN
	sprintf(buf,"%d 天 ", updays);
      #else
        sprintf(buf,"%d day%s ", updays, (updays > 1) ? "s" : "");
      #endif
    }
    //printf("updays:%s!\n",buf);
    upminutes = (long) ((now-bi->uptime) / 60);
    uphours = (upminutes / 60) % 24;
    upminutes %= 60;
    //printf("Here5!\n");
    if (uphours>0)
    {
     #ifdef DEFAULT_LANG_CN
	sprintf(buf,"%s %2d小时 %02d分钟",buf, uphours, upminutes);
     #else
        sprintf(buf,"%s %2dhour(s) %02d minute(s)",buf, uphours, upminutes);
     #endif
    }
    else
    {
     #ifdef DEFAULT_LANG_CN
	sprintf(buf,"%s %02d 分钟",buf, upminutes);
     #else
        sprintf(buf,"%s %02d minute(s)",buf, upminutes);
     #endif
    }
    
    
    chassisThreshold_t *threshold=getChassisThreshold(0);
    if(NULL!=threshold)
    {

    nBytesSent+=websWrite(wp, T("var uptime='%s';\nvar currtemp=%s;\n"),buf,temp);
    nBytesSent+=websWrite(wp, T("var mintemp=%d;\nvar maxtemp=%d;\n"),threshold->min_temperature,threshold->max_temperature);
/*
     #ifdef DEFAULT_LANG_CN
     //printf("NULL!=threshold\n");
     //printf("%d ,buf:%s,temp:%s\n",nBytesSent,buf,temp);
     //return nBytesSent;
     nBytesSent+=websWrite(wp, T(" \
    <th>系统运行时间</th> \
    <td colspan='3'>%s</td> \
    </tr><tr><th>机箱当前温度</th> \
    <td>%s°C</td>"),buf,temp);
    //return nBytesSent;
    nBytesSent+=websWrite(wp, T(" \
     <th>最低温度</th> \
    <td><input id ='mintemp' name='mintemp' value='%d' size='5' maxlength='3' onkeyup='inputNumber(this,0,true);' onblur='return check(this);'/>°C</td> \
    <th>最高温度</th> \
    <td><input id ='maxtemp' name='maxtemp' value='%d' size='5' maxlength='3' onkeyup='uIntInput(this);' onblur='return check(this);'/>°C</td> \
    </tr>"),threshold->min_temperature,threshold->max_temperature);
    //return nBytesSent;
    #else
      nBytesSent+=websWrite(wp, T(" \
    <th>Up time</th> \
    <td colspan='3'>%s</td> \
    </tr><tr><th>Chassis  temperature</th> \
    <td>%s°C</td>"),buf,temp);
   
    nBytesSent+=websWrite(wp, T(" \
     <th>Min temperature</th> \
    <td><input id ='mintemp' name='mintemp' value='%d' size='5' maxlength='3' onkeyup='inputNumber(this,0,true);' onblur='return check(this);'/>°C</td> \
    <th>Max temperature</th> \
    <td><input id ='maxtemp' name='maxtemp' value='%d' size='5' maxlength='3' onkeyup='uIntInput(this);' onblur='return check(this);'/>°C</td> \
    </tr>"),threshold->min_temperature,threshold->max_temperature);
    
    #endif
*/
    }
    }
   }
   else if (uclass==0)
   {
      nBytesSent+=aspChassisInfo(wp,group);
      //nBytesSent+=websWrite(wp, T("<tr><td>Back panel</td></tr>"));
   }
   else
      nBytesSent=websWrite(wp, T("<tr><td>Unkown</td></tr>"));
   //printf("UC=%d\n",uc);
   return nBytesSent;
}

/*
static int getOlpView(webs_t wp, char_t cclass)
{
   int nBytesSent=0;
   uchar_t uclass,utype;
   char model[6]="";
   
   if (FALSE!=getUnitClassType(0,unit_id,&uclass,&utype))
   {
      return aspOlpView(wp, unit_id,cclass,uclass,utype);
      if (utype==1)
         strcpy(model,"1+1");
      else if(utype==2)
         strcpy(model,"1:1");
      nBytesSent=websWrite(wp, T("<tr><td>%s</td></tr><tr><td><img src='../images/p9_1.gif'/></td></tr>\n<tr><td><img src='../images/p9_1.gif'/></td></tr>"),model);
   }
   else
        nBytesSent=websWrite(wp, T("<tr><td>The OLP is NOT exists!</td></tr><tr><td><tr>"));
   return nBytesSent;

}
*/
static int aspUnitView(int eid, webs_t wp, int argc, char_t **argv)
{
   int nBytesSent=-1,old_uc=0;
   //otuInfo_t info;
   uchar_t uclass=0,utype=0;
   char_t *uc;
   uchar_t cclass=4;
   /*cclass=getUnitStatusMap(0);
   if (NULL==cclass)
     return 0;
   */
   //printf("Run into aspUnitView\r\n");
   if (TRUE!=getUnitClassType(0,unit_id,&uclass,&utype))
   {
      char slotCountOfUnit;
      if (UC_PASSIVE==uclass && 3==utype)
        slotCountOfUnit=2;
      else if (uclass>0)
        slotCountOfUnit=1;
      else
        slotCountOfUnit=0;
      //nBytesSent=websWrite(wp, T("<div class='ubd'>Not exists</div>"));
      if (slotCountOfUnit<2)
        nBytesSent=websWrite(wp, T("<div class='ubd1'></div>"));
      else
         nBytesSent=websWrite(wp, T("<div class='ubd%d'></div>"),slotCountOfUnit);
      return nBytesSent;
   }
   if (ejArgs(argc, argv, T("%s"), &uc)>0)
   {
       if (strlen(uc)>0)
       {
         old_uc=atoi(uc);
         if (uclass!=old_uc && uclass!=UC_OEO)
          return websWrite(wp, T("<div class='ubd1'>Unkown</div>"));
       }
   }
   utype=getUnitType(0,unit_id);
   //printf("aspUnitView:slot #%d cclass=%d utype=%d\n",unit_id,cclass,utype);
   switch (uclass)
   {
      case UC_OTU:
      case UC_OEO:
          return aspOtuView(wp, unit_id,cclass,uclass,utype,1);
          //return getOtuView(eid, wp, argc,argv);
      case UC_OTAP:
          return aspOtapView(wp, unit_id,cclass,uclass,utype,1);
      case UC_OLP:
      case UC_OSS:
         return aspOlpView(wp,unit_id,cclass,uclass,utype) ;//getOlpView(wp, *cclass);
      case UC_EDFA:
         //printf("EDFA On Slot #%d\n",unit_id );
         return aspEdfaView(wp, unit_id,cclass,uclass,utype);
      case UC_ETU:
         //printf("ETU On Slot #%d\n",unit_id );
         return aspEtuView(wp, unit_id,cclass,utype);
      case UC_DVOA:
         //printf("EDFA On Slot #%d\n",unit_id );
         return aspDvoaView(wp, unit_id,cclass,1);
      case UC_PASSIVE:
          return aspPassiveUnitView(wp,unit_id,cclass,uclass,utype);
      default:
         return websWrite(wp, T("<div class='ubd1'></div>"));
   } 
}

static void formModifyUnitInfo(webs_t wp, char_t *path, char_t *query)
{
  char_t *slot,*index,*page,*flag,*uc,*ut;
  //int selected_slot;
  slot = websGetVar(wp, T("slot"), T("0"));
  index= websGetVar(wp, T("index"), T("0"));
  page = websGetVar(wp, T("page"), T("0"));
  flag = websGetVar(wp, T("flag"), T("0"));
  
  //selected_slot=atoi(slot);
  //printf("formModifyUnitInfo:slot:%s,index:%s,page:%s\n",slot,index,page);
  unit_id=atoi(slot);
  port_index=atoi(index);
  if (!gstrcmp(flag,"0"))
  {
    websRedirect(wp,page);
    return;
  }
}

/*
static void formSetInfoset(webs_t wp, char_t *path, char_t *query)
{
  char_t *pslot,*pindex,*ppage,*pflag,*puc,*put,*pinfosetId,*pitemId,*pvalue;
  //int selected_slot;
  //short values[16];
  infoItem_t items[16];
  uchar_t n,item_count=0,slot,infoset_id,index=0;
  char buf[10];
  pslot = websGetVar(wp, T("slot"), T("0"));
  pindex= websGetVar(wp, T("index"), T("0"));
  pinfosetId= websGetVar(wp, T("infosetId"), T("0"));
  //pitemId= websGetVar(wp, T("item"), T("0"));
  pvalue= websGetVar(wp, T("val"), T("0"));
  ppage= websGetVar(wp, T("page"), T("0"));
  
  if (NULL==pslot ||NULL==pinfosetId)
  {
     websRedirect(wp,ppage);
     return;
  }
  
  slot=(uchar_t)atoi(pslot);
  infoset_id=(uchar_t)atoi(pinfosetId);
  if (NULL!=pindex)
    index=(uchar_t)atoi(pindex);

  if (slot<1 || slot>16 || infoset_id<1)
  {
     websRedirect(wp,ppage);
     return;
  }
  for (n=1;n<36;n++)
  {
     sprintf(buf,"id%d",n);
     pitemId= websGetVar(wp, T(buf), T("0"));
     if (NULL!=pitemId)
     {
       if (item_count>12)
          break;
       items[n].item_id=n;
       items[n].item_value=atoi(pvalue);
       item_count++;
     }
  }
  if (item_count>0)
   setShortInfoItemsValues(0,slot,infoset_id,index,items,item_count,300);
  sleep(2);
  websRedirect(wp,ppage);
}
*/
static void formUserData(webs_t wp, char_t *path, char_t *query)
{
  char_t *uname,*ucon,*uloc,*updev,*udata,*userv,*slot;
  char sql[600];
 
  int selected_slot;
  slot = websGetVar(wp, T("uid"), T("0"));
  selected_slot=atoi(slot);
  if (selected_slot>0)
  {
    if (selected_slot==16)
       selected_slot=getSlotOfNmu(0,3);
    /*
    for (n=0;n<6;n++)
    {
      sprintf(buf,"item%d",n+1);
      item=websGetVar(wp, T(buf), T(""));
      printf("%s\n",item);
    }
    */
    updev = websGetVar(wp, T("updev"), T(""));
    udata = websGetVar(wp, T("udata"), T(""));
    userv= websGetVar(wp, T("userv"), T(""));

    uname = websGetVar(wp, T("uname"), T(""));
    uloc = websGetVar(wp, T("uloc"), T(""));
    ucon = websGetVar(wp, T("ucon"), T(""));
    /*
    printf("uname:%s\nudata:%s\ncontact:%s\nuloc:%s\nupdev:%s\nservice:%s len=%d\n",uname,udata,ucon,uloc,updev,userv,strlen(userv));

    
    if (strlen(uname)>30)
         uname[30]='\0';
    
    if (strlen(contact)>30)
         contact[30]='\0';
    uloc = websGetVar(wp, T("uloc"), T("uloc"));
     if (strlen(uloc)>30)
         uloc[30]='\0';
    updev = websGetVar(wp, T("updevice"), T("up device"));
    if (strlen(updev)>30)
         updev[30]='\0';
    udata = websGetVar(wp, T("udata"), T("user data"));
    if (strlen(udata)>30)
         udata[30]='\0';
    service= websGetVar(wp, T("service"), T("service"));
    if (strlen(service)>30)
         service[30]='\0';
    */
    sprintf(sql,"UPDATE UnitUD SET unit_name='%s',contact='%s',location='%s',up_device='%s',user_data='%s',service='%s' WHERE slot=%d",uname,ucon,uloc,updev,udata,userv,selected_slot);
    //printf("sql len=%d\n",strlen(sql));
    
    execSql(NULL,sql,NULL,NULL);
    if (selected_slot==getSlotOfNmu(0,3))
    {
       if (getPidByName("pidof snmpd")!=-1)
       {
          doSystem("snmpset -v 2c -c private 127.0.0.1 system.sysName.0 s '%s'",uname);
          doSystem("snmpset -v 2c -c private 127.0.0.1 system.sysLocation.0 s '%s'",uloc);
          doSystem("snmpset -v 2c -c private 127.0.0.1 system.sysContact.0 s '%s'",ucon);
          //printf("doSystem\n");
       }
       //printf("doSystem 11\n");
    }
    //printf("doSystem unit=%d\n",unit);
  }
  redirect(wp,query);
}
/*
static void formOLP(webs_t wp, char_t *path, char_t *query)
{
  char_t *slot,*mode,*line,*rule,*retmode,*rettime,*power;
  char set_cmd[50];
  int unit;
  setInfoSet_t infoset;

  slot = websGetVar(wp, T("uid"), T("0"));
  unit=atoi(slot);
  if (unit>0)
  {
    memset(&infoset,0,sizeof(setInfoSet_t));
    mode = websGetVar(wp, T("mode"), T("0"));
    line = websGetVar(wp, T("line"), T("0"));
    rule = websGetVar(wp, T("rule"), T("0"));
    retmode = websGetVar(wp, T("ret"), T("0"));
    rettime= websGetVar(wp, T("rettime"), T("0"));
    infoset.infoset_id=OLP_STATUS_INFOSET_ID;
    infoset.slot=unit;
    infoset.index=1;
    infoset.infoItems[0].item_id=OLP1_MODE;
    infoset.infoItems[0].item_value=atoi(mode);
    infoset.infoItems[1].item_id=OLP1_LINE;
    infoset.infoItems[1].item_value=atoi(line);
    infoset.infoItems[2].item_id=OLP1_RULE;
    infoset.infoItems[2].item_value=atoi(rule);
    infoset.infoItems[3].item_id=OLP1_RET_MODE;
    infoset.infoItems[3].item_value=atoi(retmode);
    infoset.infoItems[4].item_id=OLP1_RET_TIME;
    infoset.infoItems[4].item_value=atoi(rettime);
    power=websGetVar(wp, T("L1p"), T("-28"));
    infoset.infoItems[5].item_id=OLP1_L1_SW_POWER;
    infoset.infoItems[5].item_value=atoi(power);
    
    power=websGetVar(wp, T("L2p"), T("-28"));
    infoset.infoItems[6].item_id=OLP1_L2_SW_POWER;
    infoset.infoItems[6].item_value=atoi(power);
    power=websGetVar(wp, T("Txp"), T("-28"));
    infoset.infoItems[7].item_id=OLP1_TX_ALM_POWER;
    infoset.infoItems[7].item_value=atoi(power);
    infoset.useconds=1000000;
    setOlpInfo(infoset);
  }
  websRedirect(wp,"olpret.asp");
}
*/
void unitAspDefine()
{
  websAspDefine(T("UInfo"), aspUInfo);
  websAspDefine(T("UnitInfo"), aspUnitInfo);
  websAspDefine(T("PassiveInfo"), aspPassiveInfo);
  //websAspDefine(T("UnitAlarmView"),aspUnitAlarmView);
  websAspDefine(T("UnitView"), aspUnitView);
  websAspDefine(T("UserData"), aspUserData);
  websAspDefine(T("AlarmList"), aspAlarmList);
  websFormDefine(T("slot"), formUnit);
  websFormDefine(T("alarm"), formAlarm);
  websFormDefine(T("almqry"), formAlarmQuery);
  websAspDefine(T("UnitID"), aspUnitID);
  websFormDefine(T("unit"), aspUnitUrl);
  websFormDefine(T("formUD"), formUserData);
  websFormDefine(T("ModifyUnitInfo"), formModifyUnitInfo);
  websAspDefine(T("InfoSet"),aspInfoset);
  websFormDefine(T("setinfo"), formSetInfoset);
  websAspDefine(T("UnitClass"), aspUnitClassType);
  //websFormDefine(T("olp"), formOLP);
  otuAspDefine();
  ddmAspDefine();
  olpAspDefine();
  edfaAspDefine();
}
