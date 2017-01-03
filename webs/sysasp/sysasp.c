#include <string.h>
#include <signal.h>
#include <sys/sysinfo.h>
#include <time.h>
#include "../wsIntrn.h"
#include "../../include/unit.h"
#include <public.h>
#include <visipc.h>
#include <sqlite3.h>
#include <dbutil.h>
#include <log.h>
#include <sysinfo.h>
#include <chassis.h>
#include <snmpconf.h>

static int aspNmuInfo(int eid, webs_t wp, int argc, char_t **argv)
{
  char vendor_cn[60]="",vendor_en[60]="";
  char url[60]="";
  char web_name[60]="";
  char sys_name[60]="";
  char logo[20]="";
  int nbytes=0;
  /**/getVendorInfo(VENDOR_CN_NAME,vendor_cn);
  getVendorInfo(VENDOR_EN_NAME,vendor_en);
  getVendorInfo(VENDOR_URL,url);
  getVendorInfo(VENDOR_LOGO,logo);
  getVendorInfo(WEB_NAME,web_name);
  strcat(web_name," ");
  strcat(web_name,WEBS_VERSION);
  //printf("vendoren=%s url=%s\n ver=%s",vendor_en,url,WEBS_VERSION);
  getConfValue(SYS_CONF_FILE,"sysname",sys_name," ");
  nbytes+=websWrite(wp,T("var sysname='%s';"),sys_name);
  nbytes+=websWrite(wp,T("var vendorcn='%s',vendoren='%s',url='%s';"),vendor_cn,vendor_en,url);
  nbytes+=websWrite(wp,T("var webname='%s',logo='%s';"),web_name,logo);
  /*#ifdef DEFAULT_LANG_CN
    nbytes+=websWrite(wp,T("var webname='%s',logo='%s';"),web_name,logo);
  #else
   nbytes+=websWrite(wp,T("var webname='%s',logo='../%s';"),web_name,logo);
  #endif
  */
  return nbytes;
}
static int aspDeviceIp(int eid, webs_t wp, int argc, char_t **argv)
{
  int nbytes=0;
  char ip_add[24]="",mask[24]="",gateway[24]="";

  
  getDeviceIp(ip_add,mask,gateway);
  /*nbytes=websWrite(wp,T("<tr><th>IP地址</th><td ><input size='15' id='ip' name='ip' value='%s'/></td></tr>"),ip_add);
  nbytes+=websWrite(wp,T("<tr><th>子网掩码</th><td ><input size='15' id='mask' name='mask' value='%s'/></td></tr>"),mask);
  nbytes+=websWrite(wp,T("<tr><th>默认网关</th><td ><input size='15' id='gw' name='gw' value='%s'/></td></tr>"),gateway);
  if (getMac(ip_add)!=-1)
    nbytes+=websWrite(wp,T("<tr><th>MAC 地址</th><td >%s</td></tr>"),ip_add);
  */
  nbytes+=websWrite(wp,T("var ip=\"%s\";"),ip_add);
  nbytes+=websWrite(wp,T("var mask=\"%s\";"),mask);
  nbytes+=websWrite(wp,T("var gateway=\"%s\";"),gateway);
  if (getMac(ip_add)!=-1)
     nbytes+=websWrite(wp,T("var mac=\"%s\";"),ip_add);
  return nbytes;
}
static void formIpConf(webs_t wp, char_t *path, char_t *query)
{
  char_t *ip_add,*mask,*gateway;
  
  ip_add= websGetVar(wp, T("ip"), T("192.168.1.228"));
  mask= websGetVar(wp, T("mask"), T("255.255.255.0"));
  gateway= websGetVar(wp, T("gw"), T("192.168.1.1"));
  setDeviceIp(ip_add,mask,gateway);
  insertOperateLog(LOG_GROUP_SYS_IP_CONF,0,0,WebMode,mod,0,0);
  websRedirect(wp, T("ip.asp"));
}
static void formSystem(webs_t wp, char_t *path, char_t *query)
{
  char_t *slot,*act;
  char unit[10];
  uchar_t uclass;
  uchar_t n;
  uchar_t slotOfNmu;
  //int n,status;
  //slot=websGetVar(wp, T("u16"), T("0"));
  //val=websGetVar(wp, T("sta"), T("0"));
  //group=websGetVar(wp, T("grp"), T("0"));
  //sprintf(unit,"u%d",unit);
  slot=websGetVar(wp, T("u16"), T("0"));
  act=websGetVar(wp, T("act"), T("0"));
  if (!gstrcmp(act,"1"))
  {
    //slotOfNmu=getSlotOfNmu(0,3);
    for (n=0;n<=16;n++)
    {
      sprintf(unit,"u%d",n);
      slot=websGetVar(wp, T(unit), T("0"));
      if (!gstrcmp(slot,"on"))
      {
         if (n<16)
         {
           uclass=getUnitClass(0,n);
           if (uclass!=0 && uclass!=UC_PASSIVE)
           {
            restoreUnitConf(0,n);
            sleep(1);
           }
         }
         else
         {
          if (!gstrcmp(websGetVar(wp, T("defaultip"), T("off")),"on"))
            restoreNmuConf(0,1,1);
          else
            restoreNmuConf(0,0,1); 
         }
      }
    }
    websRedirect(wp, T("/restore.asp"));
    return;
    //printf("slot=%s\n",slot);
  }
  else if (!gstrcmp(act,"2"))
  {
     slot=websGetVar(wp, T("u16"), T("0"));
     if (!gstrcmp(slot,"on"))
     {
       pid_t status;
       system("tail -20 /var/log/messages|grep vierr>>/vispace/sys.log");
       status=system("reboot -n -f");
       if (-1 == status)   
       {   
        printf("system error!");   
       } 
       printf("Reboot...\n");
     }
  }
  else if (!gstrcmp(act,"3"))
  {
   char_t *volt;
   short n=0,value;
   setInfoSet_t infoset;

   memset(&infoset,0,sizeof(setInfoSet_t));
   infoset.chassis=0;
   infoset.slot=0;
   infoset.infoset_id=CHASSIS_THRESHOLD_INFOSET_ID;
   infoset.index=1;
   infoset.useconds=30000;

   chassisThreshold_t *threshold=getChassisThreshold(0);
   if (NULL!=threshold)
    {
      volt=websGetVar(wp, T("minvolt"), T("0"));
      value=atoi(volt);
      if (value>1 && value<11)
      {
          //threshold->min_volt=value;
          infoset.infoItems[n].item_id=MIN_PWR_VOLT;
          infoset.infoItems[n].item_value=value;
          n++;
      }
      volt=websGetVar(wp, T("maxvolt"), T("0"));
      value=atoi(volt);
      if (value>9 && value<21)
      {
          //threshold->min_volt=value;
          infoset.infoItems[n].item_id=MAX_PWR_VOLT;
          infoset.infoItems[n].item_value=value;
      }
       setChassisThreshold(infoset);
      insertOperateLog(LOG_GROUP_SYS_SYS_INFO_CONF,0,0,WebMode,mod,0,0);
      //  printf("slot=%s,value=%d\n",slot,value);
      websRedirect(wp, T("/sys/chassis.asp"));
   }
  }
 else if (!gstrcmp(act,"4"))
 {
   char_t *info,*password;
   //char buf[60]="";
   info=websGetVar(wp, T("usrid"), T(""));
   password=websGetVar(wp, T("pw"), T(""));
   //printf("password:%s\n",password);
   if (gstrcmp(info,"visint2013") ||gstrcmp(password,"gzvisint"))
   {
     websRedirect(wp, T("/admin/default.asp"));
     return;
   }
   info=websGetVar(wp, T("logoid"), T(""));
   //printf("logoid:%s\n",info);
   if ('3'==info[0])//设置客户版本软件信息
   {
    info=websGetVar(wp, T("vendorcn"), T(""));
    setVendorInfo(VENDOR_CN_NAME,info);
    info=websGetVar(wp, T("vendoren"), T(""));
    setVendorInfo(VENDOR_EN_NAME,info);
    info=websGetVar(wp, T("url"), T(""));
    setVendorInfo(VENDOR_URL,info);
    info=websGetVar(wp, T("webname"), T(""));
    setVendorInfo(WEB_NAME,info);
    //info=websGetVar(wp, T("logo"), T("logo.jpg"));
    //setVendorInfo(VENDOR_LOGO,info);
   }
   else if ('1'==info[0])//设置中性版本软件信息
   {
     setVendorInfo(VENDOR_CN_NAME,"");
     setVendorInfo(VENDOR_EN_NAME,"");
     setVendorInfo(VENDOR_URL,"");
     setVendorInfo(WEB_NAME,"WEB NMS");
     setVendorInfo(VENDOR_LOGO,"logo.jpg");
   }
   else if ('2'==info[0]) //设置汇信特版本软件信息
   {
     setVendorInfo(VENDOR_CN_NAME,"汇信特通信技术");
     setVendorInfo(VENDOR_EN_NAME,"Visint Communication Tech");
     setVendorInfo(VENDOR_URL,"www.visint.com.cn");
     setVendorInfo(WEB_NAME,"WebRiver");
     setVendorInfo(VENDOR_LOGO,"vlogo.png");
   }
   websRedirect(wp, T("/admin/default.asp"));
 }
 else if (!gstrcmp(act,"10"))
 {
   slot=websGetVar(wp, T("slot"), T("0"));
   int val= atoi(slot);
   
   if (val>0)
   {
     int value;
     char_t *psize,*pcount;
     val=val<<24;
     psize=websGetVar(wp, T("size"), T("32"));
     pcount=websGetVar(wp, T("count"), T("10"));
     value= atoi(psize);
     value=value<<8;
     val|=value;
     value= atoi(pcount);
     val|=value;
     printf("sinval=%x\n",val);
     sendSigToProccess("ccenter",SIGINT,val);
   }
    websRedirect(wp, T("/datamon.asp"));
 }
 else if (!gstrcmp(act,"11"))
 {
    system("killall -9 ccenter");
    websRedirect(wp, T("/datamon.asp"));
 }
  //printf("slot=%s,act=%s\n",slot,act);
}
static void formDatetime(webs_t wp, char_t *path, char_t *query)
{
  char_t *timemode,*date,*temp;
  setInfoSet_t infoset;
  int n=0,value;
  char buf[20]; 
  pid_t status;
 
  timemode=websGetVar(wp, T("timesource"), T("1"));

  getConfValue(SYS_CONF_FILE,"timesource",buf," ");
  //printf("timesource:%s timemode=%s\n",buf,timemode);
  if (gstrcmp(timemode,buf)!=0)
  {
     /*
     system("setup -u crond");
     if (!gstrcmp(timemode,"2"))
         system("setup -i crond");
     */
     //printf("Timesource:%s timemode=%s\n",buf,timemode);
     setConfValue(SYS_CONF_FILE,"timesource",timemode," ");
  }

  //printf("timesource=%s\n",timemode);
  //setConfValue("/vispace/nmu.bi","timesource",timemode,"1");
  system("killall -9 crond");
  if (!gstrcmp(timemode,"1"))
  {
   date=websGetVar(wp, T("dt"), T("2014.12.12 12:59:59"));
   for (n=0;n<gstrlen(date) && n<19;n++)
   {
     if (date[n]=='-')
          buf[n]='.';
     else if (date[n]==' ')
          buf[n]='-';
     else
          buf[n]=date[n];
   }
  buf[n]='\0';
  //printf("date:%s\n",buf);
  doSystem("date -s '%s'",buf);
  updateUnitDate(3);
  }
  else
  {
    char_t *ntpserver=websGetVar(wp, T("ntpserver"), T("202.112.10.60"));
    setConfValue(SYS_CONF_FILE,"ntpserver",ntpserver," ");
    doSystem("ntpdate -u %s &",ntpserver);
    doSystem("echo \"0 */2 * * * /usr/bin/ntpdate %s\">/var/spool/cron/crontabs/root",ntpserver);
    //doSystem("echo \"*/120 * * * * * (%s) && (hwclock -w)\" > /etc/ntpcrontab",ntpserver);
    updateUnitDate(1);
    system("crond&");
  }
  //status=system("hwclock --systohc");
  status=system("hwclock -w");
  if (-1 == status)   
  {   
      printf("system hwclock error!");   
  }
  
  memset(&infoset,0,sizeof(setInfoSet_t));
  infoset.chassis=0;
  infoset.slot=0;
  infoset.infoset_id=CHASSIS_THRESHOLD_INFOSET_ID;
  infoset.index=1;
  infoset.useconds=30000;
  //infoset.infoItems[0].item_id=MIN_CHASSIS_TEMP;
  //infoset.infoItems[1].item_id=MAX_CHASSIS_TEMP;
  n=0;
  temp=websGetVar(wp, T("mintemp"), T("-50"));
   value=atoi(temp);
  if (value>-30 && value<30)
   {
     infoset.infoItems[n].item_id=MIN_CHASSIS_TEMP;
     infoset.infoItems[n].item_value=value;
     n++;
   }
  temp=websGetVar(wp, T("maxtemp"), T("120"));
  value=atoi(temp);
  if (value>-1 && value<101)
  {
     infoset.infoItems[n].item_id=MAX_CHASSIS_TEMP;
     infoset.infoItems[n].item_value=value;
       n++;
  }
   //printf("n=%d mintemp=%s maxtemp=%s\n",n,websGetVar(wp, T("mintemp"), T("-50")),websGetVar(wp, T("maxtemp"), T("-50")));
 if (n>0)
   setChassisThreshold(infoset);
  insertOperateLog(LOG_GROUP_SYS_SYS_INFO_CONF,0,0,WebMode,mod,0,0);
  websRedirect(wp, T("/sys/sys_info.asp"));
}
/*
static int onFetchDestHostList( void *wp, int n_column, char **column_value, char **column_name)
{
  //char enable[2][4]={"on","off"};
  char check[2][8]={"checked",""};
  int n;
  if (!gstrcmp(column_value[3],"1"))
     n=0;
  else
     n=1;
  websWrite((webs_t)wp,T("<input type='hidden' id='id%s' value='%s'/>"),column_value[0],column_value[0]);
  websWrite((webs_t)wp,T("<tr>"));
  websWrite((webs_t)wp,T("<td><input id='destIp%s' maxlength='15' size='15' value='%s'/></td>"),column_value[0],column_value[1]);
  websWrite((webs_t)wp,T("<td><input id='udp%s' maxlength='5' size='5' value='%s' onkeyup='uIntInput(this);'/></td>"),column_value[0],column_value[2]);
  websWrite((webs_t)wp,T("<td><INPUT TYPE='checkbox' %s id='ena%s'>启用</td>"),check[n],column_value[0]);
  websWrite((webs_t)wp,T("<td><a href='/goform/dhost?opt=del&id=%s' onclick='return ackdel();'><img class='imgl'src='../images/del.gif' /></a><a href='#'><img src='../images/pen.gif'class='imgr' onclick='apply(%s);'/></a></td></tr>"),column_value[0],column_value[0]);
 return 0;
}
*/
static int aspTrapDestHostList(int eid, webs_t wp, int argc, char_t **argv)
{
  sqlite3 *db;
  sqlite3_stmt* stmt;
  //char *zErrMsg = 0;
  int ret,nbytes=0;
  char sql[]="SELECT * FROM DestHost";
  char check[2][8]={"checked",""};
  int n;

  ret = sqlite3_open(DB_FILE_NAME, &db);
  if (NULL==db)
     return 0;
  ret=sqlite3_prepare_v2(db,sql,strlen(sql),&stmt,NULL);
  if(ret!=SQLITE_OK)
  {
    //printf("aspDestHostList Error:%s\n",sqlite3_errmsg(db));
    if (stmt)
       sqlite3_finalize(stmt);
    sqlite3_close(db);
    return 0;
  }
  ret=sqlite3_step(stmt);
  while (ret==SQLITE_ROW)
  {
    if (1!=sqlite3_column_int(stmt,3))
     n=1;
    else
     n=0;
    nbytes+=websWrite((webs_t)wp,T("<input type='hidden' id='id%d' value='%d'/>"),sqlite3_column_int(stmt,0),sqlite3_column_int(stmt,0));
  nbytes+=websWrite((webs_t)wp,T("<tr>"));
  nbytes+=websWrite((webs_t)wp,T("<td>%s</td>"),sqlite3_column_text(stmt,4));
  nbytes+=websWrite((webs_t)wp,T("<td><input id='destIp%d' maxlength='15' size='15' value='%s'/></td>"),sqlite3_column_int(stmt,0),sqlite3_column_text(stmt,1));
  nbytes+=websWrite((webs_t)wp,T("<td><input id='udp%d' maxlength='5' size='5' value='%d' onkeyup='uIntInput(this);'/></td>"),sqlite3_column_int(stmt,0),sqlite3_column_int(stmt,2));
 #if DEFAULT_LANG_CN
  nbytes+=websWrite((webs_t)wp,T("<td><INPUT TYPE='checkbox' %s id='ena%d'>启用</td>"),check[n],sqlite3_column_int(stmt,0));
 #else
  nbytes+=websWrite((webs_t)wp,T("<td><INPUT TYPE='checkbox' %s id='ena%d'>Enable</td>"),check[n],sqlite3_column_int(stmt,0));
 #endif
  nbytes+=websWrite((webs_t)wp,T("<td><a href='/goform/dhost?opt=del&id=%d' onclick='return ackdel();'><img class='imgl'src='../images/del.gif' /></a><a href='#'><img src='../images/pen.gif'class='imgr' onclick='apply(%d);'/></a></td></tr>"),sqlite3_column_int(stmt,0),sqlite3_column_int(stmt,0));
    ret=sqlite3_step(stmt);
  }
  sqlite3_finalize(stmt);
  sqlite3_close(db);
  return nbytes;
  /*rc = sqlite3_open(DB_FILE_NAME, &db);  
  
  rc = sqlite3_exec(db, sql, onFetchDestHostList, (void *)wp, &zErrMsg); 
  sqlite3_close(db);
  */
 //return websWrite(wp, T("当前告警信息"));
}

static int aspKeepAliveDestHostList(int eid, webs_t wp, int argc, char_t **argv)
{
  sqlite3 *db;
  sqlite3_stmt* stmt;
  //char *zErrMsg = 0;
  int ret,nbytes=0;
  char sql[]="SELECT * FROM KeepAlive";
  char check[2][8]={"checked",""};
  int n=0;
  
  //nbytes=websWrite(wp,"var item=new Array();\n");
  ret = sqlite3_open(DB_FILE_NAME, &db);
  if (NULL==db)
     return 0;
  ret=sqlite3_prepare_v2(db,sql,strlen(sql),&stmt,NULL);
  if(ret!=SQLITE_OK)
  {
    printf("aspKeepAlive Error:%s\n",sqlite3_errmsg(db));
    sqlite3_close(db);
    return 0;
  }
  ret=sqlite3_step(stmt);
  if (ret==SQLITE_ROW)
  {
   nbytes+=websWrite(wp,T("var item=[[%d,'%s',%d,%d,'%s',%d]"),sqlite3_column_int(stmt,0),sqlite3_column_text(stmt,1),sqlite3_column_int(stmt,2),sqlite3_column_int(stmt,3),sqlite3_column_text(stmt,4),sqlite3_column_int(stmt,5));
   ret=sqlite3_step(stmt);
   n++;
  }
  while (ret==SQLITE_ROW)
  {
   nbytes+=websWrite(wp,T(",[%d,'%s',%d,%d,'%s',%d]"),sqlite3_column_int(stmt,0),sqlite3_column_text(stmt,1),sqlite3_column_int(stmt,2),sqlite3_column_int(stmt,3),sqlite3_column_text(stmt,4),sqlite3_column_int(stmt,5));
   ret=sqlite3_step(stmt);
   n++;
  }
  if (n>0)
      nbytes+=websWrite(wp,"];");
  else
     nbytes+=websWrite(wp,"var item=null;");
  sqlite3_finalize(stmt);
  sqlite3_close(db);
  return nbytes;
}
static int aspDestHostList(int eid, webs_t wp, int argc, char_t **argv)
{
 char_t *host=NULL,flag=1;
 if (ejArgs(argc, argv, T("%s"), &host)>0)
 {
   if (host!=NULL &&strstr(host,"2"))
       return aspKeepAliveDestHostList(eid, wp, argc,  argv);
   else
       return aspTrapDestHostList(eid, wp, argc,  argv);
 }
  return aspTrapDestHostList(eid, wp, argc,  argv);
}
/*
static int onFetchCommunity( void *wp, int n_column, char **column_value, char **column_name)
{
  char check[2][8]={"checked",""};
  char ro[]="readonly style='background:#C0C0C0';";
  //char readonly[2][8]={"checked",""};
  int n,m;
  if (!strcmp(column_value[6],"1"))
     n=0;
  else
     n=1;
  if (!strcmp(column_value[5],"1"))
     m=0;
  else
  {
    m=1;
    ro[0]='\0';
  }
  websWrite((webs_t)wp,T("<input type='hidden' id='id%s' value='%s'/>"),column_value[0],column_value[0]);
  websWrite((webs_t)wp,T("<tr>"));
  websWrite((webs_t)wp,T("<td><input id='hostIp%s' maxlength='15' size='15' value='%s'/></td>"),column_value[0],column_value[3]);
  websWrite((webs_t)wp,T("<td><input id='getc%s' maxlength='15' size='15' value='%s'/></td>"),column_value[0],column_value[1]);
  websWrite((webs_t)wp,T("<td><input id='setc%s' maxlength='15' size='15' value='%s' %s/></td>"),column_value[0],column_value[2],ro);
  websWrite((webs_t)wp,T("<td><input id='port%s' maxlength='5' size='5' value='%s' onkeyup='uIntInput(this);'/></td>"),column_value[0],"161");
  websWrite((webs_t)wp,T("<td><INPUT TYPE='checkbox' %s id='ro%s' onclick='javascript:ro(this,%s);'/></td>"),check[m],column_value[0],column_value[0]);
  websWrite((webs_t)wp,T("<td><INPUT TYPE='checkbox' %s id='ena%s'>启用</td>"),check[n],column_value[0]);
  websWrite((webs_t)wp,T("<td><a href='/goform/comm?opt=del&id=%s' onclick='return ackdel();'><img class='imgl'src='../images/del.gif' /></a><a href='#'><img src='../images/pen.gif'class='imgr' onclick='apply(%s);'/></a></td></tr>"),column_value[0],column_value[0]);
 return 0;
}
*/
static int aspCommunity(int eid, webs_t wp, int argc, char_t **argv)
{
  sqlite3 *db;
  sqlite3_stmt* stmt;
  //char *zErrMsg = 0;
  int ret,nbytes=0;
  //char *zErrMsg = 0;
  char check[2][8]={"checked",""};
  char ro[]="readonly style='background:#C0C0C0';";
  //char readonly[2][8]={"checked",""};
  int n,m;
  char can_add=1;
  char sql[]="SELECT * FROM Community ORDER BY ipadd";
  db=openDB(DB_FILE_NAME);
  
  if (NULL==db)
     return 0;
  //printf("aspOperateLog2 sq:%s DB_FILE_NAME=%s\n",sql,DB_FILE_NAME);
  ret=sqlite3_prepare_v2(db,sql,strlen(sql),&stmt,NULL);
  if(ret!=SQLITE_OK)
  {
    //printf("aspCommunity Error:%s\n",sqlite3_errmsg(db));
    sqlite3_close(db);
    return 0;
  }
  ret=sqlite3_step(stmt);
  while (ret==SQLITE_ROW)
  {
    if (1!=sqlite3_column_int(stmt,6))
       n=1;
    else
       n=0;
    if (1!=sqlite3_column_int(stmt,5))
    {
     m=1;
     ro[0]='\0';
    }
    else
       m=0;
     nbytes+=websWrite((webs_t)wp,T("<input type='hidden' id='id%d' value='%d'/>"),sqlite3_column_int(stmt,0),sqlite3_column_int(stmt,0));
  nbytes+=websWrite((webs_t)wp,T("<tr>"));
  nbytes+=websWrite((webs_t)wp,T("<td><input id='hostIp%d' maxlength='15' size='15' value='%s'/></td>"),sqlite3_column_int(stmt,0),sqlite3_column_text(stmt,3));
  nbytes+=websWrite((webs_t)wp,T("<td><input id='getc%d' maxlength='15' size='15' value='%s'/></td>"),sqlite3_column_int(stmt,0),sqlite3_column_text(stmt,1));
  nbytes+=websWrite((webs_t)wp,T("<td><input id='setc%d' maxlength='15' size='15' value='%s' %s/></td>"),sqlite3_column_int(stmt,0),sqlite3_column_text(stmt,2),ro);
  //nbytes+=websWrite((webs_t)wp,T("<td><input id='port%d' maxlength='5' size='5' value='%s' onkeyup='uIntInput(this);'/></td>"),sqlite3_column_int(stmt,0),"161");
 
  nbytes+=websWrite((webs_t)wp,T("<td><INPUT TYPE='checkbox' %s id='ro%d' onclick='javascript:ro(this,%d);'/></td>"),check[m],sqlite3_column_int(stmt,0),sqlite3_column_int(stmt,0));
 #if DEFAULT_LANG_CN
  nbytes+=websWrite((webs_t)wp,T("<td><INPUT TYPE='checkbox' %s id='ena%d'>启用</td>"),check[n],sqlite3_column_int(stmt,0));
 #else
  nbytes+=websWrite((webs_t)wp,T("<td><INPUT TYPE='checkbox' %s id='ena%d'>Enable</td>"),check[n],sqlite3_column_int(stmt,0));
 #endif
  nbytes+=websWrite((webs_t)wp,T("<td><a href='/goform/comm?opt=del&id=%d' onclick='return ackdel();'><img class='imgl'src='../images/del.gif' /></a><a href='#'><img src='../images/pen.gif'class='imgr' onclick='app(%d);'/></a></td></tr>"),sqlite3_column_int(stmt,0),sqlite3_column_int(stmt,0));
  if (strlen(sqlite3_column_text(stmt,3))<6)
  {
    can_add=0;
  }
    ret=sqlite3_step(stmt);
  }
  sqlite3_finalize(stmt);
  sqlite3_close(db);
  if (!can_add)
    nbytes+=websWrite(wp,T("<script type=\"text/javascript\">canAdd=false;</script>"));
  /* 
  rc = sqlite3_open(DB_FILE_NAME, &db);  
  
  rc = sqlite3_exec(db, sql, onFetchCommunity, (void *)wp, &zErrMsg); 
  sqlite3_close(db);
  */
  return nbytes;
}
static void formDestHost(webs_t wp, char_t *path, char_t *query)
{
  if (checkPrivilege(wp,"/desthost.asp")==1)
    return;
  char_t *id,*operate,*destIp,*udp,*ena;
  //char sql[100]="";
  //char where[20]="";
  int enable;
  id= websGetVar(wp, T("id"), T("0"));
  destIp= websGetVar(wp, T("destIp"), T("0"));
  udp= websGetVar(wp, T("udp"), T("162"));
  ena= websGetVar(wp, T("ena"), T("on"));
  
  operate= websGetVar(wp, T("opt"), T(""));
  if (!gstrcmp(ena,"on"))
      enable=1;
  else
     enable=2;
  if (!gstrcmp(operate,"mod"))
  {
     //sprintf(sql,"UPDATE DestHost SET dest_ipadd='%s',udp_port=%s,enable=%d WHERE ID=%s",destIp,udp,enable,id);
    // execSql(NULL,sql,NULL,NULL);
     if (!updateDestHost(atoi(id),destIp,atoi(udp),enable))
       insertOperateLog(LOG_GROUP_SYS_SNMP_DEST_HOST_CONF,0,0,WebMode,mod,0,0);
  }
  else  if (!gstrcmp(operate,"add"))
  {
     char_t *target_name,buf[32]="";
     target_name=websGetVar(wp, T("tname"), T(""));
     snprintf(buf,31,target_name);
     if (!insertDestHost(buf,destIp,atoi(udp),enable))
       insertOperateLog(LOG_GROUP_SYS_SNMP_DEST_HOST_CONF,0,0,WebMode,add,0,0);
  }
  else  if (!gstrcmp(operate,"del"))
  {
     //sprintf(sql,"DELETE FROM \"DestHost\" WHERE ID=%s;",id);
     //execSql(NULL,sql,NULL,NULL);
     if (!deleteDestHostById(atoi(id)))
       insertOperateLog(LOG_GROUP_SYS_SNMP_DEST_HOST_CONF,0,0,WebMode,del,0,0);
  }
  websRedirect(wp, T("desthost.asp"));
}
static void formCommunity(webs_t wp, char_t *path, char_t *query)
{
  char_t *id,*operate,*destIp,*getc,*setc,*port,*ro,*ena;
  //char sql[100]="";
  //char where[20]="";
  int commid,enable,uport,readonly;
  if (checkPrivilege(wp,"/snmpconf.asp")==1)
    return;
  id= websGetVar(wp, T("id"), T("0"));
  destIp= websGetVar(wp, T("hostIp"), T("0"));
  getc= websGetVar(wp, T("getc"), T("public"));
  setc= websGetVar(wp, T("setc"), T("private"));
  port= websGetVar(wp, T("port"), T("161"));
  ro= websGetVar(wp, T("ro"), T("off"));
  ena= websGetVar(wp, T("ena"), T("on"));
  operate= websGetVar(wp, T("opt"), T(""));
  if (!gstrcmp(ro,"on"))
      readonly=1;
  else
     readonly=2;
  if (!gstrcmp(ena,"on"))
      enable=1;
  else
     enable=2;
   uport=atoi(port);
  if (!gstrcmp(operate,"mod"))
  {
     int ret;
     commid=atoi(id);
     ret=updateCommunity(commid,destIp,getc,setc,uport,readonly,enable,0);
     insertOperateLog(LOG_GROUP_SYS_SNMP_COMMINITY_CONF,0,0,WebMode,mod,0,0);
     updateSnmpdConf(1);
     //printf("ret=%d commid=%d,destIp=%s,getc=%s,setc=%s,uport=%d\n",ret,commid,destIp,getc,setc,uport);
     //sprintf(sql,"UPDATE DestHost SET dest_ipadd='%s',udp_port=%s,enable=%d WHERE ID=%s",destIp,udp,enable,id);
     //execSql(NULL,sql,NULL,NULL);
  }
  else  if (!gstrcmp(operate,"add"))
  {
     //sprintf(sql,"INSERT INTO \"DestHost\" VALUES(NULL,'%s',%s,%d);",destIp,udp, enable);
     //execSql(NULL,sql,NULL,NULL);
     insertCommunity(destIp,getc,setc,uport,readonly,enable,0);
     insertOperateLog(LOG_GROUP_SYS_SNMP_COMMINITY_CONF,0,0,WebMode,add,0,0);
     updateSnmpdConf(1);
  }
  else  if (!gstrcmp(operate,"del"))
  {
     //sprintf(sql,"DELETE FROM \"DestHost\" WHERE ID=%s;",id);
     //execSql(NULL,sql,NULL,NULL);
     deleteCommunityById(atoi(id),0);
     updateSnmpdConf(1);
     insertOperateLog(LOG_GROUP_SYS_SNMP_COMMINITY_CONF,0,0,WebMode,del,0,0);
  }
  websRedirect(wp, T("snmpconf.asp"));
}

static void formCommand(webs_t wp, char_t *path, char_t *query)
{
  char_t *id;
  system("killall -9 watchdogd");
  id= websGetVar(wp, T("id"), T("0"));
  if (!gstrcmp(id,"1"))
     websRedirect(wp, T("upgrade2.htm"));
  else
     websRedirect(wp, T("uupgrade2.asp"));
}
/*
static int onFetchOperateLog( void *wp, int n_column, char **column_value, char **column_name)
{
  char mode_name[4][5]={"","CLI","WEB","SNMP"};
  //char operate_name[6][8]={"","修改","添加","删除","清除","确认"};
  //char readonly[2][8]={"checked",""};
  int mode,operate;
  mode=atoi(column_value[1]);

  if (mode <0 || mode >3)
     mode=0;
  operate=atoi(column_value[2]);
  if (operate <0 || operate >5)
     operate=0;
  //websWrite((webs_t)wp,T("<input type='hidden' id='id%s' value='%s'/>"),column_value[0],column_value[0]);
  websWrite((webs_t)wp,T("<tr>"));
  websWrite((webs_t)wp,T("<td>%s</td>"),column_value[4]);
  //websWrite((webs_t)wp,T("<td>%s</td>"),operate_name[operate]);
  websWrite((webs_t)wp,T("<td>%s</td>"),column_value[3]);
  websWrite((webs_t)wp,T("<td>%s</td>"),mode_name[mode]);
  websWrite((webs_t)wp,T("<td><a href='/goform/log?grp=1&opt=del&id=%s' onclick='return del();'><img src='../images/del.gif' /></a></td></tr>"),column_value[0]);
 return 0;
}
*/
static int aspSysLog(int eid, webs_t wp, int argc, char_t **argv)
{
  int ret,nbytes=0,page=0;
  char_t *grp;

   if (ejArgs(argc, argv, T("%s"), &grp) >0) {
	page=atoi(grp);
   }
  if (page==0)
  {
  int count=0;
  sqlite3_stmt* stmt;
  sqlite3 *db;
  char sql[]="SELECT * FROM SysLog ORDER BY occured_time desc";
  db=openDB(DB_FILE_NAME);
  
  if (NULL==db)
     return 0;
  ret=sqlite3_prepare_v2(db,sql,strlen(sql),&stmt,NULL);
  if(ret!=SQLITE_OK)
  {
    //printf("aspOperateLog Error:%s\n",sqlite3_errmsg(db));
    if (stmt)
        sqlite3_finalize(stmt);
    sqlite3_close(db);
    return 0;
  }
  ret=sqlite3_step(stmt);
  while (ret==SQLITE_ROW)
  {
    nbytes+=websWrite(wp,T("<tr>"));
    nbytes+=websWrite(wp,T("<td>%s</td><td>%s</td>"),sqlite3_column_text(stmt,1),sqlite3_column_text(stmt,2));
    nbytes+=websWrite(wp,T("<td><a href='/goform/syslogman?opt=del&id=%d' onclick='return del();'><img src='../images/del.gif' /></a></td></tr>"),sqlite3_column_int(stmt,0));
    nbytes+=websWrite(wp,T("</tr>"));
    count++;
    ret=sqlite3_step(stmt);
  }
   nbytes+=websWrite(wp,T("<tr><td>Records</td><td colspan='2'>%d</td></tr>"),count);
   sqlite3_finalize(stmt);
   sqlite3_close(db);
  }
  else if (page==1)
  {
   //system("tail -100 /var/log/messages |grep");
   char value[16]="";
   //int max_count=0;
   getConfValue(SYS_CONF_FILE,"max_slcount",value," ");  
   nbytes+=websWrite(wp,T("var max_count=%d;"),atoi(value));
  }
   return nbytes;
}
static int aspSysConfInfo(int eid, webs_t wp, int argc, char_t **argv)
{
  int nbytes=0;
  char value[16]="";
  int val;
   //int max_count=0;
   getConfValue(SYS_CONF_FILE,"debug",value," ");
   if (value[0]!='Y')
      value[0]='N';
   nbytes+=websWrite(wp,T("var debug='%c';\n"),value[0]);
   value[0]='\0';
   getConfValue(SYS_CONF_FILE,"printErr",value," ");
   if (value[0]!='Y')
      value[0]='N';
   nbytes+=websWrite(wp,T("var printErr='%c';\n"),value[0]);
   value[0]='\0';
   getConfValue(SYS_CONF_FILE,"saveIoErr",value," ");
   if (value[0]!='Y')
      value[0]='N';
   nbytes+=websWrite(wp,T("var saveIoErr='%c';\n"),value[0]);
   
   nbytes+=websWrite(wp,T("var maxErrors=%d;\n"),getConfIntValue(SYS_CONF_FILE,"maxErrors"," "));

   nbytes+=websWrite(wp,T("var printSlot=%d;\n"),getConfIntValue(SYS_CONF_FILE,"printSlot"," "));
   strcpy(value,"0");
   //getConfValue("/vispace/nmu.bi","cclass",value," ");
   val=getConfIntValue("/vispace/nmu.bi","cclass"," ");
   if (val<1)
      val=getConfIntValue("/vispace/sys.conf","cclass"," ");
   if (val<0)
      val=0;
   nbytes+=websWrite(wp,T("var chassis=%d,"),val);
   //strcpy(value,"0");
   //getConfValue("/vispace/nmu.bi","nslot",value," ");
   val=getConfIntValue("/vispace/nmu.bi","nslot"," ");
   if (val<1)
      val=getConfIntValue("/vispace/sys.conf","nslot"," ");
   if (val<0)
      val=0;
   nbytes+=websWrite(wp,T("slot=%d;\n"),val);
   
   return nbytes;
}
static int aspFireWallConf(int eid, webs_t wp, int argc, char_t **argv)
{
  int nbytes=0;
  char value[512]="";
  char_t *item;
  //item= websGetVar(wp, T("ipwl"), T(""));
  if (ejArgs(argc, argv, T("%s"), &item)>0)
  {
  //printf("FireWall=%s\n",item);
  if (!gstrcmp(item,"1"))
    getConfValue(SYS_CONF_FILE,"ipwl",value," ");
  else
    getConfValue(SYS_CONF_FILE,"macwl",value," ");
  }
  return websWrite(wp,T("%s"),value);
}
static void formFireWallConf(webs_t wp, char_t *path, char_t *query)
{
  char_t *id,*value;
  char wl[512]="";

  id= websGetVar(wp, T("id"), T("0"));
  value= websGetVar(wp, T("wl"), T("0"));
  printf("acl type=%s wl=%s\n",id,value);
  if (!gstrcmp(id,"1"))
     setConfValue(SYS_CONF_FILE,"ipwl",value," ");
  else
    setConfValue(SYS_CONF_FILE,"macwl",value," ");
}
static void formSysConf(webs_t wp, char_t *path, char_t *query)
{
  char_t *item;
  item= websGetVar(wp, T("user"), T(""));
  //printf("user=%s\n",item);
  if (!gstrcmp(item,"gzvisint"))
  {
    item= websGetVar(wp, T("pw"), T(""));
    //printf("pw=%s\n",item);
    if (!gstrcmp(item,"visint2015"))
    {
      item= websGetVar(wp, T("debug"), T("N"));
      //printf("debug=%s\n",item);
      setConfValue(SYS_CONF_FILE,"debug",item," ");
      if (!gstrcmp(item,"Y"))
      {
        item= websGetVar(wp, T("printSlot"), T("0"));
        //printf("printSlot=%s\n",item);
        setConfValue(SYS_CONF_FILE,"printSlot",item," ");

        item= websGetVar(wp, T("printErr"), T("N"));
        //printf("printIoErr=%s\n",item);
        setConfValue(SYS_CONF_FILE,"printErr",item," ");

        item= websGetVar(wp, T("isSaveErr"), T("N"));
        printf("isSaveErr=%s\n",item);
        if (!gstrcmp(item,"Y"))
        {
         item= websGetVar(wp, T("maxErrors"), T("0"));
         setConfValue(SYS_CONF_FILE,"maxErrors",item," ");
        }
        else
            setConfValue(SYS_CONF_FILE,"maxErrors","0"," ");
        item= websGetVar(wp, T("saveIoErr"), T("N"));
        //printf("saveIoErr=%s\n",item);
        setConfValue(SYS_CONF_FILE,"saveIoErr",item," ");
      }
      else
      {
        setConfValue(SYS_CONF_FILE,"printSlot","20"," ");
        setConfValue(SYS_CONF_FILE,"printErr","N"," ");
        setConfValue(SYS_CONF_FILE,"maxErrors","0"," ");
        setConfValue(SYS_CONF_FILE,"saveIoErr","N"," ");
      }
      //char buf[60];
      item= websGetVar(wp, T("chassis"), T("0"));
      system("sed -i '/cclass/'d /vispace/nmu.bi");
      system("sed -i '/nslot/'d /vispace/nmu.bi");
      doSystem("sed -i '1i cclass %s' /vispace/nmu.bi",item);
      //setConfValue("/vispace/nmu.bi","cclass",item," ");
      item= websGetVar(wp, T("slot"), T("0"));
      doSystem("sed -i '1a nslot %s' /vispace/nmu.bi",item);
      //setConfValue("/vispace/nmu.bi","nslot",item," ");
    }
  }
  else
  {
    char_t *sysname;
    sysname= websGetVar(wp, T("sysname"), T(""));
    setConfValue(SYS_CONF_FILE,"sysname",sysname," ");
    websRedirect(wp, T("chassisbi.asp"));
    return;
  }
  websRedirect(wp, T("admin/sysconf.asp"));
}
static void formSysLog(webs_t wp, char_t *path, char_t *query)
{
  char_t *operate;
  operate= websGetVar(wp, T("opt"), T(""));
  //printf("formSysLog:operate=%s\n",operate);
  if (!gstrcmp(operate,"del"))
  {
    char_t * id= websGetVar(wp, T("id"), T("0"));
    char sql[36]="DELETE FROM SysLog";
    if (gstrcmp(id,"0"))
       sprintf(sql,"%s WHERE ID=%s",sql,id);
     execSql(NULL,sql,NULL,NULL);
     websRedirect(wp, T("syslog.asp"));
  }
  else if (!gstrcmp(operate,"clr"))
  {
    system(":> /var/log/messages");
    system(":> /vispace/sys.log");
    websRedirect(wp, T("errlog.asp"));
  }
  else if (!gstrcmp(operate,"conf"))
  {
    char_t * mcount=websGetVar(wp, T("maxcount"), T("0"));
    //printf("mcount:%s\n",mcount);
    setConfValue(SYS_CONF_FILE,"max_slcount",mcount," ");
    websRedirect(wp, T("syslogconf.asp"));
  }
  else
      websRedirect(wp, T("syslog.asp"));
}

static int aspOperateLog(int eid, webs_t wp, int argc, char_t **argv)
{
  /*
  sqlite3 *db;
  char *zErrMsg = 0;
  int rc;
  char sql[]="SELECT A.ID,A.mode,A.operate_type,A.operate_time,B.gourp_name FROM OptLog AS A,OptObjGroup AS B WHERE A.group_id=B.ID";
   
  rc = sqlite3_open(DB_FILE_NAME, &db);  
  
  rc = sqlite3_exec(db, sql, onFetchOperateLog, (void *)wp, &zErrMsg);  
  sqlite3_close(db);
  */
  int ret,nbytes=0;
  sqlite3_stmt* stmt;
  sqlite3 *db;
  #ifdef DEFAULT_LANG_CN
  char sql[]="SELECT A.ID,A.mode,A.operate_type,A.operate_time,A.slot,A.port,B.group_name FROM OptLog AS A,OptObjGroup AS B WHERE A.group_id=B.ID ORDER BY A.operate_time DESC";
  #else
  char sql[]="SELECT A.ID,A.mode,A.operate_type,A.operate_time,A.slot,A.port,B.group_name_en as group_name FROM OptLog AS A,OptObjGroup AS B WHERE A.group_id=B.ID ORDER BY A.operate_time DESC";
  #endif
  //sprintf(sql,"SELECT property,sn,model,creation,fwver,hwver FROM UnitBI WHERE slot=%d Limit 1",slot);

  db=openDB(DB_FILE_NAME);
  
  if (NULL==db)
     return 0;
  //printf("aspOperateLog2 sq:%s DB_FILE_NAME=%s\n",sql,DB_FILE_NAME);
  ret=sqlite3_prepare_v2(db,sql,strlen(sql),&stmt,NULL);
  if(ret!=SQLITE_OK)
  {
    //printf("aspOperateLog Error:%s\n",sqlite3_errmsg(db));
    if (stmt)
        sqlite3_finalize(stmt);
    sqlite3_close(db);
    return 0;
  }
  ret=sqlite3_step(stmt);
  //printf("aspOperateLog\n");
  //int cols=sqlite3_column_count(stmt);
  int n,count=0;
  char mode_name[5][5]={"","SYS","CLI","WEB","SNMP"};
  #ifdef DEFAULT_LANG_CN
    char operate_name[6][8]={"","修改","添加","删除","清除","确认"};
  #else
    char operate_name[6][8]={"","Modify","Add","Del","Clean","Ack"};
  #endif
  while (ret==SQLITE_ROW)
  {
    nbytes+=websWrite(wp,T("<tr>"));
    nbytes+=websWrite(wp,T("<td>"));
    if (sqlite3_column_int(stmt,4)>0)
    {
      #ifdef DEFAULT_LANG_CN
      nbytes+=websWrite(wp,T("单元盘 #%d"),sqlite3_column_int(stmt,4));
      if (sqlite3_column_int(stmt,5)>0)
        nbytes+=websWrite(wp,T("&nbsp;/端口 #%d"),sqlite3_column_int(stmt,5));
      #else
      nbytes+=websWrite(wp,T("Slot #%d"),sqlite3_column_int(stmt,4));
      if (sqlite3_column_int(stmt,5)>0)
        nbytes+=websWrite(wp,T("&nbsp;/Port #%d"),sqlite3_column_int(stmt,5));
      #endif
    }
    else
    {
     #ifdef DEFAULT_LANG_CN
        nbytes+=websWrite(wp,T("系统"));
     #else
        nbytes+=websWrite(wp,T("System"));
     #endif
    }
    nbytes+=websWrite(wp,T("</td>"));
    //nbytes+=websWrite((webs_t)wp,T("<td>%d</td>"),sqlite3_column_int(stmt,0));
    nbytes+=websWrite(wp,T("<td>%s</td>"),sqlite3_column_text(stmt,6));
    n=sqlite3_column_int(stmt,2);
    if (n<0 || n>5)
        n=0;
    nbytes+=websWrite(wp,T("<td>%s</td>"),operate_name[n]);
    nbytes+=websWrite(wp,T("<td>%s</td>"),sqlite3_column_text(stmt,3));
    n=sqlite3_column_int(stmt,1);
    if (n<0 || n>4)
        n=0;
    nbytes+=websWrite(wp,T("<td>%s</td>"),mode_name[n]);
    nbytes+=websWrite(wp,T("<td><a href='/goform/log?grp=1&opt=del&id=%d' onclick='return del();'><img src='../images/del.gif' /></a></td></tr>"),sqlite3_column_int(stmt,0));
    count++;
    ret=sqlite3_step(stmt);
  }
  #ifdef DEFAULT_LANG_CN
  nbytes+=websWrite(wp,T("<tr><td colspan='6'>操作记录总数:%d</td></tr>"),count);
  #else
  nbytes+=websWrite(wp,T("<tr><td colspan='6'>Recorders:%d</td></tr>"),count);
  #endif
  sqlite3_finalize(stmt);
  sqlite3_close(db);
  return nbytes;
}

static void formOperateLog(webs_t wp, char_t *path, char_t *query)
{
  char_t *id,*operate,*group;
  
  id= websGetVar(wp, T("id"), T("0"));
  group= websGetVar(wp, T("grp"), T("1"));
  operate= websGetVar(wp, T("opt"), T(""));
  
  if (!gstrcmp(operate,"del"))
  {
     if (!gstrcmp(group,"1"))
       deleteOperateLogById(atoi(id));
     //insertOperateLog(LOG_GROUP_SYS_OPT_LOG_CONF,0,0,WebMode,del);
     //printf("formOperateLog id=%s operate=%s\n",id,operate);
  }
  else if (!gstrcmp(operate,"mod"))
  {
    if (!gstrcmp(group,"2"))
    {
     char_t *ena;
     char sql[60]="";
     int enable;
     ena=websGetVar(wp, T("ena"), T("1"));
     if (!gstrcmp(ena,"1"))
         enable=1;
     else
         enable=2;
     sprintf(sql,"UPDATE  OptObjGroup SET enable=%d WHERE ID=%s",enable,id);
     execSql(NULL,sql,NULL,NULL);
    }
  } 
  if (!gstrcmp(group,"1"))    
    websRedirect(wp, T("optlog.asp"));
  else
    websRedirect(wp, T("logconf.asp"));
}
/*
static int onFetchLogConf( void *wp, int n_column, char **column_value, char **column_name)
{
  char enable[2][8]={"checked",""};
  int n;
  if (!gstrcmp(column_value[2],"1"))
     n=0;
  else
     n=1;
  //websWrite((webs_t)wp,T("<input type='hidden' id='id%s' value='%s'/>"),column_value[0],column_value[0]);
  websWrite((webs_t)wp,T("<tr>"));
  websWrite((webs_t)wp,T("<td>%s</td>"),column_value[0]);
  websWrite((webs_t)wp,T("<td>%s</td>"),column_value[1]);
  websWrite((webs_t)wp,T("<td><INPUT TYPE='checkbox' %s id='ena%s'></td>"),enable[n],column_value[0]);
  websWrite((webs_t)wp,T("<td><a href='#' onclick='return apply(%s);'><img src='../images/pen.gif' /></a></td></tr>"),column_value[0]);
 return 0;
}
*/
static int aspLogConf(int eid, webs_t wp, int argc, char_t **argv)
{
  sqlite3 *db;
  sqlite3_stmt* stmt;
  //char *zErrMsg = 0;
  int n,ret,nBytes;
  char enable[2][8]={"checked",""};

  #ifdef DEFAULT_LANG_CN
  char sql[]="SELECT ID,group_name,enable FROM OptObjGroup";
  #else
  char sql[]="SELECT ID,group_name_en,enable FROM OptObjGroup";
  #endif
  db=openDB(DB_FILE_NAME);
  if (NULL==db)
     return 0;
  ret=sqlite3_prepare_v2(db,sql,strlen(sql),&stmt,NULL);
  if(ret!=SQLITE_OK)
  {
    printf("aspUnitAlarmView Error:%s\n",sqlite3_errmsg(db));
    sqlite3_close(db);
    return 0;
  }
  ret=sqlite3_step(stmt);
  while (ret==SQLITE_ROW)
  {
    if (1==sqlite3_column_int(stmt,2))
       n=0;
    else
       n=1;
    nBytes+=websWrite((webs_t)wp,T("<tr>"));
    nBytes+=websWrite((webs_t)wp,T("<td>%d</td>"),sqlite3_column_int(stmt,0));
    nBytes+=websWrite((webs_t)wp,T("<td>%s</td>"),sqlite3_column_text(stmt,1));
    nBytes+=websWrite((webs_t)wp,T("<td><INPUT TYPE='checkbox' %s id='ena%d'></td>"),enable[n],sqlite3_column_int(stmt,0));
    nBytes+=websWrite((webs_t)wp,T("<td><a href='#' onclick='return apply(%d);'><img src='../images/pen.gif' /></a></td></tr>"),sqlite3_column_int(stmt,0));
    ret=sqlite3_step(stmt);
  }
  sqlite3_finalize(stmt);
  sqlite3_close(db);
  /*
  rc = sqlite3_open(DB_FILE_NAME, &db);  
  
  rc = sqlite3_exec(db, sql, onFetchLogConf, (void *)wp, &zErrMsg); 
  sqlite3_close(db);
*/
  return nBytes;
}
static int aspUdpConf(int eid, webs_t wp, int argc, char_t **argv)
{
  int nBytes;
  char buf[60]="";
  int value=0,n;
  /*
  FILE *ptr;
  if((ptr=popen("sed -n '/udps/p' /etc/init.d/Svispace", "r"))!=NULL)
  {
    if (fgets(buf, 32, ptr)!=NULL)
    {
        if (strstr(buf,"udps"))
            value=1;
    }
    pclose(ptr);
  }
  */
  getConfValue(SYS_CONF_FILE,"udp_enable",buf," ");
  if (buf[0]=='Y')
    value=1;
  nBytes=websWrite(wp,T("var enabled=%d;\r\n"),value);
  if (getPidByName("pidof udps")<1)
      value=0;
  else
      value=1;
  nBytes+=websWrite(wp,T("var is_start=%d;\r\n"),value);
  getConfValue(SYS_CONF_FILE,"udp_ac_msg",buf," ");
  nBytes+=websWrite(wp,T("var acp_msg=\"%s\";"),buf);
  value=getConfIntValue(SYS_CONF_FILE,"udpport"," ");
  if (value<1)
     value=5001;
  nBytes+=websWrite(wp,T("var port=%d;\r\n"),value);
  return nBytes;
}
static void formUdpConf(webs_t wp, char_t *path, char_t *query)
{
  char_t *pvalue;
  char buf[60]="",item_name[20];
  uchar_t n;
  pvalue=websGetVar(wp, T("port"), T("5001"));
  setConfValue(SYS_CONF_FILE,"udpport",pvalue," ");
  pvalue=websGetVar(wp, T("acp_msg"), T(""));
  setConfValue(SYS_CONF_FILE,"udp_ac_msg",pvalue," ");

  pvalue=websGetVar(wp, T("enabled"), T(""));
  system("killall -9 udps");
  if (!gstrcmp(pvalue,"1"))
  {
      setConfValue(SYS_CONF_FILE,"udp_enable","Y"," ");
      system("udps &");
      //system("setup -i udps");
  }
  else
  {
      setConfValue(SYS_CONF_FILE,"udp_enable","N"," ");
      //system("setup -u udps");
  }
}

static int aspHeartBeat(int eid, webs_t wp, int argc, char_t **argv)
{
  int nBytes;
  char buf[60]="";
  char ip_list[24]="";
  int value=0,n,slotOfNmu=16;
  
  /*
   FILE *ptr;
  if((ptr=popen("sed -n '/heartbeat/p' /etc/init.d/Svispace", "r"))!=NULL)
  {
    if (fgets(buf, 32, ptr)!=NULL)
    {
        if (strstr(buf,"heartbeat"))
            value=1;
    }
    pclose(ptr);
  }
  */
  for (n=1;n<5;n++)
  {
    char item_name[16];
    sprintf(item_name,"hb_group%d",n);
    getConfValue(SYS_CONF_FILE,item_name,buf," ");
    if (buf[0]!='\0')
    {
      value=0;
      char slots[40]="";
      sscanf(buf,"%d:%[^:]:%[^:]",&value,slots,ip_list);
      nBytes+=websWrite(wp,T("hb_groups[%d]= new Object();\n"),n-1);
      nBytes+=websWrite(wp,T("hb_groups[%d].Ip='%s';\n"),n-1,ip_list);
      nBytes+=websWrite(wp,T("hb_groups[%d].Linkage=%d;\n"),n-1,value);
      nBytes+=websWrite(wp,T("hb_groups[%d].Slots=[%s];\n"),n-1,slots);
      buf[0]='\0';
    }
  }
  getConfValue(SYS_CONF_FILE,"hb_enable",buf," ");
  if (buf[0]=='Y')
    value=1;
  else
    value=2;
  nBytes+=websWrite(wp,T("var enabled=%d;\r\n"),value);
  if (getPidByName("pidof heartbeat")<1)
      value=0;
  else
      value=1;
  slotOfNmu=getSlotOfNmu(0,3);
  nBytes+=websWrite(wp,T("var is_start=%d;\r\n"),value);
  value=getConfIntValue(SYS_CONF_FILE,"port"," ");
  nBytes+=websWrite(wp,T("var port=%d;\r\n"),value);

  value=getConfIntValue(SYS_CONF_FILE,"waitting"," ");
  nBytes+=websWrite(wp,T("var waitting=%d;"),value);

  value=getConfIntValue(SYS_CONF_FILE,"sec"," ");
  nBytes+=websWrite(wp,T("var sec=%d"),value);
  value=getConfIntValue(SYS_CONF_FILE,"msec"," ");
  nBytes+=websWrite(wp,T(",msec=%d;\r\n"),value);

  value=getConfIntValue(SYS_CONF_FILE,"hb_mode"," ");
  if (value<1 ||value>2)
     value=1;
 /*
  getConfValue(SYS_CONF_FILE,"msg_type",buf," ");
  if (buf[0]=='\0')
     strcpy(buf,"1");
 */
  nBytes+=websWrite(wp,T("var hb_mode=%d;"),value);

  getConfValue(SYS_CONF_FILE,"ac_msg",buf," ");
  nBytes+=websWrite(wp,T("var acp_msg=\"%s\","),buf);
  
  buf[0]='\0';
  getConfValue(SYS_CONF_FILE,"enabledReply",buf," ");
  if (buf[0]=='Y')
      nBytes+=websWrite(wp,T("enabledReply=true,"));
  else
     nBytes+=websWrite(wp,T("enabledReply=false,"));
  getConfValue(SYS_CONF_FILE,"ehco",buf," ");
  nBytes+=websWrite(wp,T("ehco=\"%s\","),buf);

  getConfValue(SYS_CONF_FILE,"hb_slots",buf," ");
  nBytes+=websWrite(wp,T("hb_slots=[%s],"),buf);

  /*
  getConfValue(SYS_CONF_FILE,"switch_one",buf," ");
  if (buf[0]=='Y')
    nBytes+=websWrite(wp,"switch_one=true,");
  else
    nBytes+=websWrite(wp,"switch_one=false,");
  */
  nBytes+=websWrite(wp,T("ip_list= new Array();\r\n"));
  for (n=0;n<slotOfNmu;n++)
  {
    buf[0]='\0';
    sprintf(ip_list,"accept_ip%d",n+1);
    getConfValue(SYS_CONF_FILE,ip_list,buf," ");
    nBytes+=websWrite(wp,T("ip_list[%d]=\"%s\";\r\n"),n,buf);
  }
  return nBytes;
}

static void formHeartBeat(webs_t wp, char_t *path, char_t *query)
{
  char_t *pvalue,*pslot;
  char buf[60]="",item_name[20];
  int slot;
  uchar_t n;
  uchar_t max_slot=getSlotOfNmu(0,2);
  for (n=1;n<max_slot;n++)
  {
    sprintf(item_name,"s%d",n);
    pslot=websGetVar(wp, T(item_name), T(""));
    if (pslot!=NULL && !gstrcmp(pslot,"on"))
    {
      if (buf[0]!='\0')
        sprintf(item_name,",%d",n);
      else
         sprintf(item_name,"%d",n);
      strcat(buf,item_name);
      //printf("#%d pslot=%s\n",n,pslot);
    }
  }
  //printf("hb_slots=%s\n",buf);
  setConfValue(SYS_CONF_FILE,"hb_slots",buf," ");
  for (n=1;n<5;n++)
  {
    sprintf(item_name,"hb_group%d",n);
    pslot=websGetVar(wp, item_name, T(""));
    setConfValue(SYS_CONF_FILE,item_name,pslot," ");
    //printf("hb group%d=%s\n",n,pslot);
  }
 // pvalue=websGetVar(wp, T("reply_msg"), T(""));
  //printf("reply_msg=%s\n",pvalue);

  //pslot=websGetVar(wp, T("slot"), T(""));
  //slot=atoi(pslot);
  //if (slot>0)
  {
    /*
    pvalue=websGetVar(wp, T("slots"), T(""));
    setConfValue(SYS_CONF_FILE,"slots",pvalue," ");

    pvalue=websGetVar(wp, T("acp_ip"), T(""));
    ///printf("acp_ip=%s\r\n",pvalue);
    sprintf(item_name,"accept_ip%d",slot);
    setConfValue(SYS_CONF_FILE,item_name,pvalue," ");
    */
    pvalue=websGetVar(wp, T("port"), T("2012"));
    setConfValue(SYS_CONF_FILE,"port",pvalue," ");

    pvalue=websGetVar(wp, T("hb_mode"), T("1"));
    setConfValue(SYS_CONF_FILE,"hb_mode",pvalue," ");

    if (gstrcmp(pvalue,"1"))
    {
     pvalue=websGetVar(wp, T("acp_msg"), T(""));
     setConfValue(SYS_CONF_FILE,"ac_msg",pvalue," ");
    }
    
     pvalue=websGetVar(wp, T("reply"), T(""));
     //preply=websGetVar(wp, T("reply"), T(""));
     //printf("pvalue=%s reply=%s\n",pvalue,preply);
     if (pvalue!=NULL)
     {
      if (gstrstr(pvalue,"Y"))
      {
        char_t *reply_msg;
        setConfValue(SYS_CONF_FILE,"enabledReply","Y"," ");
        pvalue=websGetVar(wp, T("ehco"), T(""));
        setConfValue(SYS_CONF_FILE,"ehco",pvalue," ");
        //reply_msg=websGetVar(wp, T("reply_msg"), T(""));
        //printf("reply_msg=%s\n",reply_msg);
      }
      else
         setConfValue(SYS_CONF_FILE,"enabledReply","N"," ");
     
    }
    pvalue=websGetVar(wp, T("waitting"), T("0"));
    setConfValue(SYS_CONF_FILE,"waitting",pvalue," ");

    pvalue=websGetVar(wp, T("sec"), T(""));
    setConfValue(SYS_CONF_FILE,"sec",pvalue," ");
    pvalue=websGetVar(wp, T("msec"), T(""));
    setConfValue(SYS_CONF_FILE,"msec",pvalue," ");

    /*
    pvalue=websGetVar(wp, T("enabled"), T(""));
    if (!gstrcmp(pvalue,"1"))
    {
      setConfValue(SYS_CONF_FILE,"hb_enable","Y"," ");
      //system("setup -i heartbeat");
    }
    else
    {
      setConfValue(SYS_CONF_FILE,"hb_enable","N"," ");
      //system("setup -u heartbeat");
    }
    pvalue=websGetVar(wp, T("start"), T(""));
    */
    //getConfValue(SYS_CONF_FILE,"enabled",buf," ");
    pvalue=websGetVar(wp, T("enabled"), T(""));
    //printf("enabled=%s\n",pvalue);
    if (!gstrcmp(pvalue,"1"))
       setConfValue(SYS_CONF_FILE,"hb_enable","Y"," ");
    else
       setConfValue(SYS_CONF_FILE,"hb_enable","N"," ");
    /*
    if (!gstrcmp(pvalue,"1"))
    {
      if (buf[0]!='Y')
      {
        setConfValue(SYS_CONF_FILE,"hb_enable","Y"," ");
        //system("killall watchdogd");
        //sleep(2);
        //system("watchdogd &");
      }
      //system("heartbeat -s 0 &");
      //printf("slot=%d start=%s\r\n",slot,pvalue);
    }
    else
    {
      if (buf[0]!='Y')
      {
        setConfValue(SYS_CONF_FILE,"hb_enable","N"," ");
        //system("killall watchdogd");
        //sleep(2);
        //system("watchdogd &");
      }
    }
   */
  }
  websRedirect(wp, T("/sys/heartbeat.asp"));
}
void sysAspDefine()
{
  websAspDefine(T("DeviceIp"),aspDeviceIp);
  websFormDefine(T("ipconf"), formIpConf);
  websAspDefine(T("HeartBeat"),aspHeartBeat);
  websFormDefine(T("SetHeartBeat"), formHeartBeat);
  websAspDefine(T("UdpConf"),aspUdpConf);
  websFormDefine(T("SetUdpConf"), formUdpConf);
  websFormDefine(T("sys"), formSystem);
  websFormDefine(T("date"), formDatetime);
  websAspDefine(T("DestHostList"), aspDestHostList);
  websFormDefine(T("dhost"), formDestHost);
  websAspDefine(T("Community"), aspCommunity);
  websFormDefine(T("comm"), formCommunity);
  websAspDefine(T("OperateLog"), aspOperateLog);
  websAspDefine(T("LogConf"), aspLogConf);
  websAspDefine(T("syslog"), aspSysLog);
  websAspDefine(T("SysConfInfo"), aspSysConfInfo);
  websFormDefine(T("syslogman"), formSysLog);
  websFormDefine(T("sysconf"), formSysConf);
  websFormDefine(T("log"), formOperateLog);
  websAspDefine(T("NmuInfo"),aspNmuInfo);
  websFormDefine(T("command"), formCommand);
  websAspDefine(T("WhiteList"), aspFireWallConf);
  websFormDefine(T("wlconf"), formFireWallConf);
  
}
