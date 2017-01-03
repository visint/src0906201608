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
#include  <visdb.h>
#include  <service.h>


#define DEFAULT_IP "192.168.1.228"
#define DEFAULT_SUBNET_MASK "255.255.255.0"
#define DEFAULT_GATEWAY "192.168.1.1"
#define NMU_BASE_INFO_FILE "/vispace/nmu.bi"

extern void alarmHandle(int sig,struct siginfo *siginfo,void *act);
//static uchar_t cclass=0;
//char is_debug_mode=0;
debug_t debug;

alarmconf_t AlarmConf;
int almmsgid;
void initAlarmConf()
{
  char value[30]="";
  memset(&AlarmConf,0,sizeof(alarmconf_t));
  getConfValue(SYS_CONF_FILE,"save_alarms",value," ");
  if (value[0]=='2')
    AlarmConf.is_add_once=0;
  else
    AlarmConf.is_add_once=1;
  AlarmConf.max_alarm_count=getConfIntValue(SYS_CONF_FILE,"max_acount"," ");
  if (AlarmConf.max_alarm_count<1)
     AlarmConf.max_alarm_count=100;
  else if (AlarmConf.max_alarm_count<20)
     AlarmConf.max_alarm_count=20;
  else if (AlarmConf.max_alarm_count>2000)
     AlarmConf.max_alarm_count=2000;

  //建立消息队列  
    almmsgid = msgget((key_t)1234, 0666 | IPC_CREAT);
    //printf("almmsgid: %ld\n", almmsgid);
    if(almmsgid == -1)  
    {  
        printf("msgget failed with error: %d\n", errno);  
        //exit(EXIT_FAILURE);  
    }
   
  /*
  struct sigaction act;
  sigemptyset(&act.sa_mask);  
  act.sa_flags=SA_SIGINFO;
  act.sa_sigaction=alarmHandle;

  sigaction(SIGALRM,&act,NULL);
  */
  //printf("initAlarmConf:is_add_once=%d max_alarm_count=%d\r\n",AlarmConf.is_add_once,AlarmConf.max_alarm_count);
}
void initDebug()
{
  char value[30]="";
  memset(&debug,0,sizeof(debug_t));
  getConfValue(SYS_CONF_FILE,"debug",value," ");
  if (value[0]=='Y')
  {
   debug.is_debug_mode=1;
   getConfValue(SYS_CONF_FILE,"printErr",value," ");
   if (value[0]=='Y')
      debug.is_print_err=1;
   getConfValue(SYS_CONF_FILE,"saveIoErr",value," ");

   if (value[0]=='Y')
     debug.is_save_io_err=1;

   debug.slot_to_print=(uchar_t)getConfIntValue(SYS_CONF_FILE,"printSlot"," ");
   /*
   value[0]='\0';
   getConfValue(SYS_CONF_FILE,"printSlot",value," ");
   if (value[0]=='\0')
     debug.slot_to_print=20;
   else
     debug.slot_to_print=(uchar_t)atoi(value);
   */
   if (debug.slot_to_print<0 || debug.slot_to_print>20)
     debug.slot_to_print=20;
  }
  else
  {
    /*
    debug.is_debug_mode=0;
    debug.is_print_io_err=0;
    debug.is_save_io_err=0;
    debug.max_err_count=0;
    */
    //debug.max_syslog_count=300;
    debug.slot_to_print=20;
  }
  debug.max_syslog_count=getConfIntValue(SYS_CONF_FILE,"max_slcount"," ");
  if (debug.max_syslog_count<0)
     debug.max_syslog_count=0;
}

void setStartUpService(char service_id,char boot,char watchdog)
{
  char cmd[256]="",service[20]="";
  switch (service_id)
  {
    case HEART_BEAT_SERVICE_ID:
          strcpy(service,"heartbeat");
    break;
    case OLP_SERVICE_ID:
          strcpy(service,"olputil");
    break;
    /*
    case WEBS_SERVICE_ID:
          strcpy(service,"heartbeat");
    break;
    case CCENTER_SERVICE_ID:
          strcpy(service,"heartbeat");
    break;
    case SNMPD_SERVICE_ID:
          strcpy(service,"heartbeat");
    break;
    */
    default:
     return;
  }

  if (boot==1 ||boot==2)
  {
   sprintf(cmd,"sed -i '/%s/'d /etc/init.d/Svispace",service);//-----删除Svispace文件所有包含文本的行。            
   system(cmd);
   if (boot==1)
   {
    sprintf(cmd,"sed -i '$a %s&' /etc/init.d/Svispace",service);//Svispace文件最后一行添加。
    system(cmd);
   }
  }
  if (watchdog==1 ||watchdog==2)
  {
    if (watchdog==1)
      setConfValue(SYS_CONF_FILE,"heartbeat","Y"," ");
    else
      setConfValue(SYS_CONF_FILE,"heartbeat","N"," ");
    system("killall -9 watchdogd");
  }
  doSystem("killall -9 %s",service);
  sleep(1);
  doSystem("%s &",service);
  if (watchdog==1 ||watchdog==2)
  {
   sleep(1);
   system("watchdogd &");
  }
}

boolean_t getNmuViewInfo(uchar_t chassis,uchar_t slot,uchar_t uclass,uchar_t utype,unitViewInfo_t *info)
{
  chassis_t *pchassis;
  char fan=2;
  char sfp1=2,sfp2=2,sfp1_led=2,sfp2_led,lan[2]={2},power[2]={2};
  
  nmuStatus_t *nmu=(nmuStatus_t*)getInfoSetValues(0,slot,8,1);
  pchassis=getChassisInfo(0);

  if (NULL==pchassis ||NULL==nmu)
     return FALSE;
  info->ports.portCount=6;
  if (pchassis->pwr1_led!=1)
    info->leds.leds_status[0][0]=3;
  else
    info->leds.leds_status[0][0]=1;
  if (pchassis->pwr2_led!=1)
    info->leds.run_led=3;
  else
    info->leds.run_led=1;

  info->leds.leds_status[0][1]=pchassis->fan_led;
  info->leds.leds_status[0][2]=LED_GREEN;

  if (pchassis->pwr1_led!=1)
    info->leds.leds_status[0][0]=LED_WHITE;
  else
    info->leds.leds_status[0][0]=LED_GREEN;
  
  if (nmu->sfp1_exist!=2)
  {
    if (nmu->sfp1_status!=1)
       info->ports.ports_status[0]=2;
    else
       info->ports.ports_status[0]=1;
  }
  else
       info->ports.ports_status[0]=0;
  if (nmu->sfp2_exist!=2)
  {
    if (nmu->sfp2_status!=1)
       info->ports.ports_status[1]=2;
    else
       info->ports.ports_status[1]=1;
  }
  else
       info->ports.ports_status[1]=0;

  if (info->ports.ports_status[0]!=1)
     info->leds.leds_status[1][1]=LED_WHITE;
  else
     info->leds.leds_status[1][1]=LED_GREEN;
  if (info->ports.ports_status[1]!=1)
     info->leds.leds_status[1][2]=LED_WHITE;
  else
     info->leds.leds_status[1][2]=LED_GREEN;

  if (pchassis->pwr2_led!=1)
   info->leds.run_led=LED_WHITE;
  else
   info->leds.run_led=LED_GREEN;

  info->ports.ports_status[2]=nmu->lan1_status;
  info->ports.ports_status[3]=nmu->lan2_status;

  info->uclass=UC_NMU;
  info->leds.col_count=2;
  strcpy(info->unit_name,"NMU");
  strcpy(info->ports.ports_text[0][0],"SFP1");
  strcpy(info->ports.ports_text[0][1],"SFP2");
  strcpy(info->ports.ports_text[0][2],"LAN1");
  strcpy(info->ports.ports_text[0][3],"LAN2");
  strcpy(info->ports.ports_text[0][4],"CON");
  strcpy(info->ports.ports_text[0][5],"Stack");
  
  strcpy(info->leds.run_text,"PWR2");
  strcpy(info->leds.leds_text[0][0],"FAN");
  strcpy(info->leds.leds_text[0][1],"RUN");
  strcpy(info->leds.leds_text[1][0],"SFP1");
  strcpy(info->leds.leds_text[1][1],"SFP2");
  return TRUE;
}

boolean_t getChassisViewInfo(uchar_t chassis,uchar_t slot,uchar_t uclass,uchar_t utype,unitViewInfo_t *info)
{
 
}

void restoreDefaultConf(char isReboot)
{
  setDeviceIp(DEFAULT_IP,DEFAULT_SUBNET_MASK,DEFAULT_GATEWAY);
  system("cp /vispace/webs/umconfig.def /vispace/webs/umconfig.txt");
  //system("cp /vispace/sys.def /vispace/sys.conf");
  
  if (isReboot)
  {
   system("reboot -n -f");
  }
}

int initNmuBaseInfo(char chassis,int slot)
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
     //printf("lasttime=%s\n",value); 
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
          //printf("%ld %ld \n",tpstart.tv_sec,nmu_bi->uptime);
          /*
          printf("nmu_bi->uptime=%ld\n",nmu_bi->uptime);
          gettimeofday(&tpstart,NULL);
          printf("tpstart.tv_sec:%ld\n",tpstart.tv_sec);

          printf("days:%ld\n",(tpstart.tv_sec-tt)/(60*60*24));
         */
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
  /*
  if (getConfValue(NMU_BASE_INFO_FILE,"hwver",nmu_bi->hwver," "))
      strcpy(nmu_bi->hwver,NMU_HW_VER);
  */
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
}

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
/*
int setFirmwareInfo(enum fw_info_item,char *info,char *vendor_en,char *url)
{
  int ret=0;
  if (NULL!=vendor_cn)
   ret=setConfValue(NMU_BASE_INFO_FILE,"vendorcn",vendor_cn," ");
  if (NULL!=vendor_en)
     ret=setConfValue(NMU_BASE_INFO_FILE,"vendoren",vendor_en," ");
  
  if (NULL!=url)
   ret=setConfValue(NMU_BASE_INFO_FILE,"url",url," ");

  return ret;
}
int getFirmWareInfo(char *web_name,char *vendor_ver,char *logo)
{
  getConfValue(NMU_BASE_INFO_FILE,"webname",web_name," ");
  getConfValue(NMU_BASE_INFO_FILE,"vendorver",vendor_ver," ");
  getConfValue(NMU_BASE_INFO_FILE,"logo",logo," ");
  return 0;
}
*/
int setVendorInfo(int item_id,char *info)
{
  int ret=-1;
  switch (item_id)
  {
   case VENDOR_CN_NAME:
       ret=setConfValue(NMU_BASE_INFO_FILE,"vendorcn",info," ");
       break;
   case VENDOR_EN_NAME:
       ret=setConfValue(NMU_BASE_INFO_FILE,"vendoren",info," ");
       break;
   case VENDOR_URL:
       ret=setConfValue(NMU_BASE_INFO_FILE,"url",info," ");
       break;
   case WEB_NAME:
       ret=setConfValue(NMU_BASE_INFO_FILE,"webname",info," ");
       break;
   case VENDOR_LOGO:
       ret=setConfValue(NMU_BASE_INFO_FILE,"logo",info," ");
       break;
  }
  return ret;
}
int getDeviceIp(char *ip_add,char *mask,char *gateway)
{
  
  if (NULL!=ip_add)
  {
       if (getConfValue("/etc/network/interfaces","address",ip_add," "))
     //if (visConfGetKey("DevIp", "ipAdd",ip_add))
          return -1;
  }
  if (NULL!=mask)
  {
     //if (visConfGetKey("DevIp", "mask",mask))
        if (getConfValue("/etc/network/interfaces","netmask",mask," "))
          return -1;
  }
  if (NULL!=gateway)
  {
     //if (visConfGetKey("DevIp", "gateway",gateway))
       if (getConfValue("/etc/network/interfaces","gateway",gateway," "))
          return -1;
  }

  return 0;
}
int setDeviceIp(char *ip_addr,char *mask,char *gateway)
{
  char modified=0;
  if (NULL!=ip_addr)
  {
     //doSystem("ifconfig eth0 %s",ip_addr);
     //doSystem(
     //if (visConfSetKey( "DevIp", "ipAdd",ip_addr))
      if (setConfValue("/etc/network/interfaces","address",ip_addr," "))
          return -1;
        modified=1;
  }
  if (NULL!=mask)
  {
     //if (visConfSetKey("DevIp", "mask",mask))
     if (setConfValue("/etc/network/interfaces","netmask",mask," "))
          return -1;
        modified=1;
  }
  if (modified)
  {
     char subnet[30]="";
     if (NULL==mask)
     {
       getDeviceIp(NULL,subnet,NULL);
       mask=subnet;
     }
     doSystem("ifconfig eth0 %s netmask %s up",ip_addr,mask);
  }
  if (NULL!=gateway)
  {
     //if (visConfSetKey("DevIp", "gateway",gateway))
       if (setConfValue("/etc/network/interfaces","gateway",gateway," "))
          return -1;
        doSystem("route add default gw %s",gateway);
  }
  return 0;
}

int getMac(char* mac)
{
    struct ifreq tmp;
    int sock_mac;
    char mac_addr[30];
    sock_mac = socket(AF_INET, SOCK_STREAM, 0);
    if( sock_mac == -1){
        perror("create socket fail\n");
        return -1;
    }
    memset(&tmp,0,sizeof(tmp));
    strncpy(tmp.ifr_name,"eth0",sizeof(tmp.ifr_name)-1 );
    if( (ioctl( sock_mac, SIOCGIFHWADDR, &tmp)) < 0 ){
        printf("mac ioctl error\n");
        return -1;
    }
    
    sprintf(mac_addr, "%02x:%02x:%02x:%02x:%02x:%02x",
            (unsigned char)tmp.ifr_hwaddr.sa_data[0],
            (unsigned char)tmp.ifr_hwaddr.sa_data[1],
            (unsigned char)tmp.ifr_hwaddr.sa_data[2],
            (unsigned char)tmp.ifr_hwaddr.sa_data[3],
            (unsigned char)tmp.ifr_hwaddr.sa_data[4],
            (unsigned char)tmp.ifr_hwaddr.sa_data[5]
            );
    //printf("local mac:%s\n", mac_addr);
    close(sock_mac);
    memcpy(mac,mac_addr,strlen(mac_addr));
    return 0;
}
static short new_log_count=0;
/*************************************
功能：把字节串信息添加到sylog里
参数说明：
const char* message，字节串
**************************************/
void addMessageToLog(const char *message)
{
 if (new_log_count>20)
 {
   new_log_count=0;
   system("tail -200 /var/log/messages|grep vierr>>/var/log/messages");
 }
 else
     new_log_count++;
  doSystem("logger -p 1 -t vierr \"%s\"",message);
}
/*************************************
功能：把字节流转换成字符串，并添加到sylog里
参数说明：
const char *tag--信息标志字符串
const char message[]，信息字节串
char is_hex：非0 转换为十六进制，0--转换为十进制
int size：字节串长度
**************************************/
void addByteMessageToLog(const char *tag,const char message[],char is_hex, int size)
{
   if (size<1 || size>512)
     return;
   int n;
   char info[size*3+2];
   char data[4];
   //memset(info,0,size*3+2);
   if (tag!=NULL)
      sprintf(info,"%s:",tag);
   else
      info[0]='\0';
   if (is_hex)
       sprintf(data,"%x",message[0]);
   else
      sprintf(data,"%d",message[0]);
   strcat(info,data);
   for (n=1;n<size;n++)
   {
      if (is_hex)
        sprintf(data," %x",message[n]);
      else
        sprintf(data," %d",message[n]);
      strcat(info,data);
   }
  addMessageToLog(info);
}


