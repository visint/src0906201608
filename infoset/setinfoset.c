#include <stdio.h>
#include <string.h>
#include <visipc.h>


#include <public.h>
#include <unit.h>
#include <infoset.h>
#include <sysinfo.h>
#include <dbutil.h>
#include <vispaceio.h>
#include <log.h>

static char allow_ok_alarm=0;
static char allow_fail_alarm=0;
static char allow_log=0;
int sendMessageToUnit(uchar_t chassis,uchar_t slot,char message[],unsigned int useconds,char is_send_alarm,char from,char is_print)
{
	char cmd_set;
	if (!allow_log)
	{
		if (getRowCount(NULL,"select count(*) FROM AlarmType WHERE alarm_code=7 AND (is_send=1 OR is_save=1)")>0)
			allow_fail_alarm=1;
		if (getRowCount(NULL,"select count(*) FROM AlarmType WHERE alarm_code=9 AND (is_send=1 OR is_save=1)")>0)
			allow_ok_alarm=1;
		if (getRowCount(NULL,"select count(*) FROM OptObjGroup WHERE ID>29 AND ID <100 AND enable=1")>0)
			allow_log=1;
		if (debug.is_print_err)
			printf("ok_alarm=%d fail_alarm=%d allow_log=%d\r\n",allow_ok_alarm,allow_fail_alarm,allow_log);
	}
        

	if (message[1]!=107)
		cmd_set=cmd_set_runtime_1;
	else
		cmd_set=cmd_set_runtime_2;

	//cmd_set=cmd_set_runtime_1;
	int n=0;
	int ret;
        //printf("cmd_set=%d\n");
        if (cmd_set!=cmd_set_runtime_2)
            ret=ioSet(chassis,slot,message, cmd_set);
        else
            ret=ioSet(chassis,slot,message+1, cmd_set);
        while (ret!=EXIT_SUCCESS && n<3)
        {
          if (cmd_set!=cmd_set_runtime_2)
               ret=ioSet(chassis,slot,message, cmd_set);
          else
               ret=ioSet(chassis,slot,message+1, cmd_set);
          n++;
        }
        //printf("#%d err count=%d,debug.is_save_io_err=%d\n",slot,n,debug.is_save_io_err);
        if (debug.is_save_io_err && n>0)
           addSysLog(SYS_LOG_IO_ERR,"#%d ioset ERR:%d",slot,n);
        
	if (debug.is_print_err)
	{
		printf("cmd_set=%d slot=#%d flag=%c count of setting fail=%d\n",cmd_set,slot,message[0],n);
                if (cmd_set!=cmd_set_runtime_2)
                    n=0;
                 else
                     n=1;
		for (; n<32; n++)
			printf("0x%0x ",message[n]);
		printf("\nSet unit #%d ",slot);
		if (ret!=EXIT_SUCCESS)
		    printf("fail\n");
		else
		    printf("OK\n");
	}
	if (ret!=EXIT_SUCCESS)
	{
	   if (allow_fail_alarm && is_send_alarm)
		sendAlarm(7,0,slot,0,0,0,0,0,NULL);
	}
	else
	{
          // printf("Here 1..\n");
	   if (allow_ok_alarm && is_send_alarm)
	      sendAlarm(9,0,slot,0,0,0,0,0,NULL);
           //printf("Here 2..\n");
	   if (allow_log && from>0 && from<5)
	   {
	     //insertOperateLog(LOG_UNIT_CONF,0,0,from,mod,slot,0);
              //printf("from=%d mod=%d ,slot=%d\r\n",from,mod,slot);
              insertUnitSettingLog(0,0,from,slot,0);
	   }
	}

	return ret;
}
int ioBroadcast(uchar_t classis,uchar_t slot_count, char *slot_list,char *message,uchar_t retry)
{
   char setbuf[512];
   int n,ret;
   
   setbuf[0]= 0;
   setbuf[1]= slot_count;
   //printf("ioBroadcast slot_count=%d slot_list:",slot_count);
   for (n=0;n<slot_count;n++)
   {
    setbuf[n+2]=slot_list[n];
    //printf("%d ",slot_list[n]);
   }
   //printf("\n");
   
   memcpy(setbuf+slot_count+2,message,128-slot_count);

   if (retry<1)
       retry=1;
   for (n=0;n<retry;n++)
   {
    ret=ioSet(0,BROADCAST_ADDR,setbuf,cmd_set_runtime_1);
    if (EXIT_SUCCESS==ret)
      break;
    else
        usleep(100000);
   }
   if (n>1)
   {
     char buf[20];
     sprintf(buf,"BC fail:%d",n);
     printf("%s\n",buf);
     insertSysLog(SYS_LOG_IO_ERR,buf);
   }
   return ret;
}
int setShortInfoItemValue(uchar_t chassis,uchar_t slot,uchar_t infoset_id,uchar_t index,uchar_t item_id,short value,unsigned int useconds,char is_send_alarm,char from,char is_print)
{
	char message[512]={0};
	message[0]='S';
	message[1]=infoset_id;
	message[2]=index;
	message[3]=item_id;
	message[4]=(value & 0xff00)>>8;
	message[5]=value & 0xff;
	message[6]=0;
	//return ioSet(chassis,slot,message,cmd_set_runtime_1);
	return sendMessageToUnit(chassis,slot,message,useconds,is_send_alarm,from,is_print);
}


int setShortInfoItemsValues(uchar_t chassis,uchar_t slot,uchar_t infoset_id,uchar_t index,infoItem_t items[],uchar_t item_count,unsigned int useconds,char is_send_alarm,char from,char is_print)
{
	char n,m=0,message[512]={0};
	message[0]='S';
	message[1]=infoset_id;
	message[2]=index;
	if (item_count<1 ||item_count>36)
		return -1;
	for (n=0; n<item_count; n++)
	{
		if (items[n].item_id<1 || items[n].item_id>36)
			break;
		message[3+m]=items[n].item_id;
		message[4+m]=(items[n].item_value & 0xff00)>>8;
		message[5+m]=items[n].item_value & 0xff;
		//printf("#%d item_id=%d value=%d\n",n,items[n].item_id,items[n].item_value);
		m+=3;
	}
	//  return ioSet(chassis,slot,message,cmd_set_runtime_1);
	return sendMessageToUnit(chassis,slot,message,useconds,is_send_alarm,from,is_print);
}
int setShortInfoItems(setInfoSet_t infoset)
{
  char n,m=3,flag;
  char message[512]={0};
  short value;
 
  uchar_t item_id;
  for (n=0;n<MAX_SETTING_ITEM_COUNT;n++)
  {
    //flag=1;
    if (infoset.infoItems[n].item_id<1)
      break;
    value=infoset.infoItems[n].item_value;
    message[m]=infoset.infoItems[n].item_id;
    message[m+1]=(value & 0XFF00)>>8;
    message[m+2]= (value & 0XFF);
    m+=3;
    if (m>60)
        break;
  }
  if (m>3)
   {
     message[0]='S';
     message[1]=infoset.infoset_id;
     message[2]=infoset.index;
     sendMessageToUnit(infoset.chassis,infoset.slot,message,infoset.useconds,0,2,0);
     //sendMessageToUnit(uchar_t chassis,uchar_t slot,char message[],unsigned int useconds,char is_send_alarm,char from,char is_print)
     return 1;
   }
   return -1;
}
