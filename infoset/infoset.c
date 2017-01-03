#include <stdio.h>
#include <string.h>
#include <visipc.h>
#include <chassis.h>
#include <ddm.h>
#include <olp.h>
#include <otu.h>

#include <public.h>
#include <unit.h>
#include <infoset.h>
#include <passive.h>
#include <sysinfo.h>
#include <dbutil.h>
#include <vispaceio.h>
#include <log.h>

//extern void sendAlarm(int alarm_code,char chassis,char slot,char entity,char is_with_value,short alarm_value,unsigned short alarm_reason,time_t alarm_time);
//char is_debug=0;
//char is_print_io_err=0;
//char is_save_io_err=0;
//uchar_t print_slot=20;

/*
typedef struct
{
uchar_t uclass;
uchar_t utype;
uchar_t dtype;
uchar_t infoSetID;
uchar_t itemCount;
const uchar_t *itemsOff;
}infoSetDef_t;


const uchar_t otu_status_items[]={1,1,1,1,1};
infoSetDef_t infoSetDef[]={
{UC_OTU,1,1,OTU_STATUS_INFOSET_ID,5,otu_status_items},
{UC_OTU,1,1,52,10,{1,1,1,1,1}}
};
*/
/*******************************************
*
********************************************/
/*
static char allow_ok_alarm=0;
static char allow_fail_alarm=0;
static char allow_log=0;
int sendMessageToUnit1(uchar_t chassis,uchar_t slot,char message[],unsigned int useconds,char is_send_alarm,char from,char is_print)
{
	char cmd_set;
	if (!allow_log)
	{
		if (getRowCount(NULL,"select count(*) FROM AlarmType WHERE alarm_code=7 AND (enable=1 OR is_save=1)")>0)
			allow_fail_alarm=1;
		if (getRowCount(NULL,"select count(*) FROM AlarmType WHERE alarm_code=9 AND (enable=1 OR is_save=1)")>0)
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

int setShortInfoItemValue1(uchar_t chassis,uchar_t slot,uchar_t infoset_id,uchar_t index,uchar_t item_id,short value,unsigned int useconds,char is_send_alarm,char from,char is_print)
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


int setShortInfoItemsValues1(uchar_t chassis,uchar_t slot,uchar_t infoset_id,uchar_t index,infoItem_t items[],uchar_t item_count,unsigned int useconds,char is_send_alarm,char from,char is_print)
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
*/
void printfMessage(uchar_t slot,char *message,char printHex, int size)
{
    int n;
    printf("Data of slot #%d\r\n",slot);
    for (n=0; n<size; n++)
    {
        if (printHex)
            printf("D%d=%0x ",n,message[n]);
        else
            printf("D%d=%0d ",n,message[n]);
    }
    printf("\r\n");
}

boolean_t addInfoSet(status_infoset_t infosets[],uchar_t max_count,uchar_t infoset_id,uchar_t index,uchar_t infoset_type,uchar_t infoset_size,uchar_t item_count)
{
    uchar_t n;
    //printf("run in addInfoSet\n");
    for (n=0; n<max_count; n++)
    {
        if (infosets[n].infoset_id<1)
            break;
    }
    //printf("n=%d max_count=%d on addInfoSet\n",n,max_count);
    if (n<max_count)
    {
        //printf("infoset count=%d in addInfoSet infoset_id=%d\n",n,infoset_id);
        infosets[n].infoset_id=infoset_id;
        infosets[n].index=index;
        infosets[n].infoset_type=infoset_type;
        infosets[n].infoset_size=infoset_size;
        infosets[n].item_count=item_count;
        return TRUE;
    }
    return FALSE;
}

boolean_t getByteInfoItems(uchar_t chassis,uchar_t slot,uchar_t infoSetID,uchar_t index,uchar_t infoItemsId[],char itemsValue[])
{
    char n,*pval,*pinfoset;
    pval=getInfoSetValues(0,slot,infoSetID,index);
    pinfoset=(char*)getInfoSet(chassis,slot,infoSetID,index);
    if (NULL==pinfoset)
        return FALSE;
    for (n=0; n<36; n++)
    {
        if (infoItemsId[n]>0 && infoItemsId[n]<37)
            itemsValue[n]=*(pval+infoItemsId[n]-1);
        else
            break;
    }
    return TRUE;
}

boolean_t getShortInfoItems(uchar_t chassis,uchar_t slot,uchar_t infoSetID,uchar_t index,uchar_t infoItemsId[],short itemsValue[])
{
    char n,*pval;
    pval=getInfoSetValues(0,slot,infoSetID,index);
    if (NULL==pval)
        return FALSE;
    for (n=0; n<18; n++)
    {
        if (infoItemsId[n]>0 && infoItemsId[n]<18)
        {
            //val=*(pval+(infoItemsId[n]-1)*2);
            itemsValue[n]=(short)*(pval+(infoItemsId[n]-1)*2);
        }
        else
            break;
    }
    return TRUE;
}
void fetchInfoItems(uchar_t chassis,uchar_t slot,uchar_t infoSetID,uchar_t index,uchar_t infoItemsId[],infosetHandler handler)
{
    short itemsValue[36];
    char n,*pval;
    status_infoset_t *pinfoset;
    printf("fetchInfoItems,slot=%d,infoSetID=%d,index=%d\n",slot,infoSetID,index);
    //pval=getInfoSetValues(0,slot,infoSetID,index);
    pinfoset=getInfoSet(chassis,slot,infoSetID,index);
    if (NULL==pinfoset)
    {
        printf("NULL==pinfoset ,slot=%d,infoSetID=%d,index=%d\n",slot,infoSetID,index);
        return;
    }
    for (n=0; n<36; n++)
    {
        if (infoItemsId[n]>0 && infoItemsId[n]<18)
        {
            if (pinfoset->infoset_type==1)
                itemsValue[n]=pinfoset->item_values[infoItemsId[n]-1];
            else if (pinfoset->infoset_type==2)
                itemsValue[n]=(short)(pinfoset->item_values[infoItemsId[n]-1]);
        }
        else
            break;
    }
    printf("NULL!=pinfoset ,slot=%d,infoSetID=%d,index=%d infoset_type=%d\n",slot,infoSetID,index,pinfoset->infoset_type);
//if (NULL!=infosetHandler)
    handler(index,infoItemsId,itemsValue);
}
/*int loadInfoset(uchar_t chassis,uchar_t slot,uchar_t unit_class,uchar_t unit_type)
{
  sqlite3 *db=NULL;
   int rc;
   char sql[200]="";
  char infoset_name[30]="";
  result_t result;
  int n,m,item_count=0,status_count=0;
  char   col_name[10][16]={ "","", "","", "","","", "",""};
  char   dataset[FIELD_MAX_COUNT][FIELD_MAX_SIZE]={ "","", "","", "","","", "",""};
}
*/
int insertNmuInfoSet(uchar_t  chassis,uchar_t slot)
{
    status_infoset_t infosets[2];
    memset(&infosets[0],0,sizeof(status_infoset_t));
    infosets[0].infoset_id=8;
    //infoset_count=1;
    infosets[0].index=1;
    infosets[0].infoset_type=0;
    infosets[0].infoset_size=12;
    infosets[0].item_count=12;
    infosets[0].item_values[0]=2;
    infosets[1].item_values[0]=2;
    infosets[3].item_values[0]=2;
    infosets[4].item_values[0]=2;
    return insertInfoSet(chassis,slot,infosets,1);
}
int insertBpuInfoSet(uchar_t  chassis)
{
    status_infoset_t infosets[2];
    int n,infoset_count=0;
    //deleteSlotNodes(chassis,0);
    //insertNmuInfoSet(chassis);
    memset(&infosets[0],0,sizeof(status_infoset_t));
    infosets[0].infoset_id=CHASSIS_INFOSET_ID;
    //infoset_count=1;
    infosets[0].index=1;
    infosets[0].infoset_type=0;
    infosets[0].infoset_size=34;
    infosets[0].item_count=25;

    memset(&infosets[1],0,sizeof(status_infoset_t));
    infosets[1].infoset_id=CHASSIS_THRESHOLD_INFOSET_ID;
    infosets[1].index=1;
    infosets[1].infoset_type=2;
    infosets[1].infoset_size=12;
    infosets[1].item_count=8;

    for (n=0; n<MAX_ITEM_COUNT; n++)
    {
        infosets[0].item_values[n]=1;
        infosets[1].item_values[n]=1;
    }
    if (insertInfoSet(chassis,0,infosets,2)!=-1)
    {
        chassisThreshold_t *pThreshold;
        pThreshold=getChassisThreshold(0);
        if (NULL!=pThreshold)
        {
            pThreshold->min_temperature=-20;
            pThreshold->max_temperature=90;
            pThreshold->min_volt=3;
            pThreshold->max_volt=20;
        }
        return 1;
    }
    else
        return -1;
}
/*
int insertNmuInfoSet(uchar_t  chassis)
{
  #define NMU_BASE_INFO_FILE "/vispace/nmu.bi"
  unit_base_info_t *nmu_bi;
  struct timeval tpstart;
  nmu_bi=getBaseInfoMap(0,16);
  if (NULL==nmu_bi)
    return -1;
  gettimeofday(&tpstart,NULL);
  nmu_bi->uptime=tpstart.tv_sec;

  if (getConfValue(NMU_BASE_INFO_FILE,"hwver",nmu_bi->hwver," "))
      strcpy(nmu_bi->hwver,NMU_HW_VER);
  getConfValue(NMU_BASE_INFO_FILE,"model",nmu_bi->model," ");
  getConfValue(NMU_BASE_INFO_FILE,"sn",nmu_bi->sn," ");
  getConfValue(NMU_BASE_INFO_FILE,"creation",nmu_bi->creation," ");
  strcpy(nmu_bi->fwver,NMU_FW_VER);
  return 0;
}
*/
int insertOtuInfoSet(uchar_t chassis,uchar_t slot,uchar_t unit_type)
{
    status_infoset_t infosets[4];
    ddmThreshold_t *ddmt,*pddmt;

    int n,infoset_count=0;
    //deleteSlotNodes(chassis,0);

    memset(&infosets[0],0,sizeof(status_infoset_t));
    memset(&infosets[1],0,sizeof(status_infoset_t));
    memset(&infosets[2],0,sizeof(status_infoset_t));
    memset(&infosets[3],0,sizeof(status_infoset_t));
    //memset(&infosets[4],0,sizeof(status_infoset_t));

    infosets[0].infoset_id=UNIT_PROPERTY_INFOSET_ID;
    infosets[1].infoset_id=OTU_STATUS_INFOSET_ID;
    infosets[2].infoset_id=DDM_INFOSET_ID;
    infosets[3].infoset_id=DDM_THRESHOLD_ID;
    //infosets[4].infoset_id=DDM_THRESHOLD_ID;

    //infoset_count=1;
    infosets[0].index=1;
    infosets[0].infoset_type=0;
    infosets[0].infoset_size=sizeof(otuProperty_t);
    infosets[0].item_count=14;

    infosets[1].index=1;
    infosets[1].infoset_type=1;
    infosets[1].infoset_size=21;
    infosets[1].item_count=21;

    infosets[2].index=1;
    infosets[2].infoset_type=2;
    infosets[2].infoset_size=20;
    infosets[2].item_count=10;

    infosets[3].index=1;
    infosets[3].infoset_type=1;
    infosets[3].infoset_size=20;
    infosets[3].item_count=20;
    /*
    infosets[4].index=2;
    infosets[4].infoset_type=2;
    infosets[4].infoset_size=20;
    infosets[4].item_count=10;
    */

    for (n=0; n<MAX_ITEM_SIZE; n++)
    {
        infosets[0].item_values[n]=0;
        infosets[1].item_values[n]=0;
        infosets[2].item_values[n]=0;
        infosets[3].item_values[n]=0;
        //infosets[4].item_values[n]=0;
    }
    infosets[0].item_values[5]=3;
    infosets[0].item_values[6]=1;
    infosets[0].item_values[7]=1;
    infosets[0].item_values[8]=1;
    infosets[0].item_values[9]=1;
    infosets[0].item_values[10]=1;
    /*
    ddmt.rxPowerMax=1;   //< 输入最大光功率单位:0.1dBm
    ddmt.rxPowerMin=-500;  !< 输入最小光功率单位:0.1dBm
    ddmt.txPowerMax=1;  < 输出光功率单位:0.1dBm
    ddmt.txPowerMin=-500;  < 输出最小光功率单位:0.1dBm
    ddmt.tempMax=800;     < SFP  最大温度单位:0.1°C
    ddmt.tempMin=-20;     < SFP  最小温度单位:0.1°C
    ddmt.voltMax=8000;    < SFP  最大电压单位:毫伏
    ddmt.voltMin=3000;     < SFP  最小电压单位:毫伏
    ddmt.rxBiasMax=30000;   < SFP  最大偏置电流单位:毫安
    ddmt.rxBiasMin=5000;   < SFP  最小偏置电流单位:毫安
    memcpy(&infosets[2].item_values,&ddmt,sizeof(ddmThreshold_t));
    memcpy(&infosets[3].item_values,&ddmt,sizeof(ddmThreshold_t));
    */
    //return insertStatusInfoSet(chassis,0,infoset);
    //printf("insertOtuInfoSet %d\n",slot);
    if (insertInfoSet(chassis,slot,infosets,4)!=-1)
    {
        //groupDdmThreshold_t *gddmt=getGroupDdmThreshold(0,slot,1);
        pddmt=getPortDdmThreshold(0, slot,1);
        if (pddmt!=NULL)
        {
            for (n=1; n<3; n++)
            {
                pddmt=getPortDdmThreshold(0, slot,n);
                if (pddmt!=NULL)
                {
                    pddmt->rxPowerMax=1;  /*!< 输入最大光功率单位:dBm*/
                    pddmt->rxPowerMin=-50;  /*!< 输入最小光功率单位:dBm*/
                    pddmt->txPowerMax=1;  /*!< 输出光功率单位:dBm*/
                    pddmt->txPowerMin=-50;  /*!< 输出最小光功率单位:dBm*/
                    pddmt->voltMax=8;     /*!< SFP  最大电压单位:伏*/
                    pddmt->voltMin=3;     /*!< SFP  最小电压单位:伏*/
                    pddmt->rxBiasMax=30;   /*!< SFP  最大偏置电流单位:毫安*/
                    pddmt->rxBiasMin=5;   /*!< SFP  最小偏置电流单位:毫安*/
                    pddmt->tempMax=80;     /*!< SFP  最大温度单位:°C*/
                    pddmt->tempMin=-20;     /*!< SFP  最小温度单位:°C*/
                    pddmt++;
                }
            }
        }
        return 5;
    }
    else
        return -1;
}

int insertOlpInfoSet(uchar_t chassis,uchar_t slot,uchar_t unit_class)
{
    status_infoset_t infosets[3];
    int n,gcount=1;
    memset(&infosets[0],0,sizeof(status_infoset_t));
    memset(&infosets[1],0,sizeof(status_infoset_t));
    //memset(&infoset[1],0,sizeof(status_infoset_t));
    infosets[0].infoset_id=UNIT_PROPERTY_INFOSET_ID;
    infosets[0].index=1;
    infosets[0].infoset_type=1;
    infosets[0].infoset_size=6;
    infosets[0].item_count=11;
    infosets[0].item_values[0]=4;
    infosets[0].item_values[1]=1;
    infosets[0].item_values[2]=1;
    infosets[0].item_values[3]=1;
    infosets[0].item_values[4]=1;
    infosets[0].item_values[5]=4;
    infosets[0].item_values[6]=1;
    infosets[0].item_values[7]=1;
    infosets[0].item_values[8]=1;
    infosets[0].item_values[9]=1;
    infosets[0].item_values[10]=1;

    //printf("UC_OSS=%d unit_class=%d\n",UC_OSS,unit_class);
    if (UC_OSS==unit_class)
        gcount=2;
    for (n=1; n<gcount+1; n++)
    {
        infosets[n].infoset_id=OLP_STATUS_INFOSET_ID;
        infosets[n].index=n;
        infosets[n].infoset_type=0;
        infosets[n].infoset_size=36;
        infosets[n].item_count=20;
    }
    if (insertInfoSet(chassis,slot,infosets,gcount+1)!=-1)
    {
        /*
        olp1P1Info_t  *olp;
        olp=getOlp1P1Info(chassis,slot);
        if(olp!=NULL)
        {
           olp->mode=1;
           olp->line=1;
           olp->rule=1;
           olp->ret_mode=1;
           olp->ret_time=0;
           olp->rx1_power=-210;
           olp->rx2_power=-220;
           olp->tx_power=-120;
           olp->l1_power=-280;
           olp->l2_power=-280;
           olp->tx_alm_power=-280;
           olp->rx1_led=1;
           olp->rx2_led=1;
           olp->tx_led=1;
        }
        */
        return 1;
    }
    else
        return -1;
}

/*
int setShortInfoItem(
    uchar_t chassis,
    uchar_t slot,
    //char_t infoset_type;//'s'-字节型 'S'-双字节 'W'-混合型
    uchar_t infoset_id,
    uchar_t index,
    uchar_t item_id,
    short value,
    unsigned int useconds)
{
  char message[MAX_MESSAGE_SIZE]={0};
  message[0]='S';
  message[1]=infoset_id;
  message[2]=index;
  message[3]=item_id;
  memcpy(&message[4],&value,sizeof(short));
  message[6]=0;
  return sendMessageToUnit(chassis,slot,message,useconds);
}
*/
/*
int setByteInfoItemValue(uchar_t chassis,uchar_t slot,uchar_t infoset_id,uchar_t index,uchar_t item_id,char value,unsigned int useconds)
{
  return setShortInfoItem(chassis,slot,infoset_id,index,item_id,value,useconds);
}
*/
/*
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
     sendMessageToUnit(infoset.chassis,infoset.slot,message,infoset.useconds);
     //sendMessageToUnit(uchar_t chassis,uchar_t slot,char message[],unsigned int useconds,char is_send_alarm,char from,char is_print)
     return 1;
   }
   return -1;
}
*/
/*
int setByteInfoItem(
    uchar_t chassis,
    uchar_t slot,
    //char_t infoset_type;//'s'-字节型 'S'-双字节 'W'-混合型
    uchar_t infoset_id,
    uchar_t index,
    uchar_t item_id,
    char value,
    unsigned int useconds)
{
  char message[MAX_MESSAGE_SIZE]={0};
  message[0]='s';
  message[1]=infoset_id;
  message[2]=index;
  message[3]=item_id;
  message[4]=value;
  message[5]=0;
  sendMessageToUnit(chassis,slot,message,useconds);
}
int setUnitInfoSet(setInfoSet_t infoset)
{
  int ret=-1;

  switch (infoset.infoset_id)
  {
    case OTU_STATUS_INFOSET_ID:
      break;
    case OLP_STATUS_INFOSET_ID:
       ret=setOlpInfo(infoset);
      break;
    case DDM_THRESHOLD_ID:
      break;
    case CHASSIS_THRESHOLD_INFOSET_ID:
      break;
    default:
     return -1;
  }
  if (ret==-1)
     return -1;
  return 0;
}
*/
static void initPropertyInfoset(uchar_t chassis,uchar_t slot,uchar_t uclass,uchar_t utype)
{
    char *property;
    property=getUnitProperty(chassis,slot);
    if (property==NULL)
        return;
    switch (uclass)
    {
    case UC_PASSIVE:
        if (PASSIVE_ODM_UNIT==utype)
        {
            property[0]=sizeof(odmProperty_t);
        }
    }
}
int loadInfosets(uchar_t chassis,uchar_t slot,uchar_t uclass,uchar_t utype,char property[])
{
    otuProperty_t *otu;
    //printf("uclass=%d\n",property[0]);
    switch (uclass)
    {
    case UC_OTU:
    case UC_OEO:
        otu=(otuProperty_t *)property;
        //printf("speed=%d wl:%s\n",otu->channel_property[0].max_speed,otu->channel_property[0].wave_length);
        insertOtuInfoSets(chassis,slot,*otu,utype);
        break;
    case UC_OTAP:
        insertOtapInfoSets(chassis,slot,utype);
        break;
    case UC_OLP:
    case UC_OSS:
        insertOlpInfoSet(0,slot,uclass);
        break;
    case UC_EDFA:
        insertEdfaInfoSets(0,slot,utype);
        break;
    case UC_ETU:
        insertEtuInfoSets(0,slot);
        break;
    case UC_DVOA:
        insertDvoaInfoSets(0,slot);
        break;
    case UC_PASSIVE:

        break;
    default:
        return 0;
    }
    initPropertyInfoset(chassis,slot,uclass, utype);
    return 1;
// printf("uc=%d\n",property[0]);
}

/**************************************************************
【功能】      插入单元盘
【参数说明】
            chassis      机箱号（0-MAX_CHASSIS_COUNT）,0为主机箱
            uchar_t      slot 槽位号（0-MAX_UNIT_COUNT）,0为背板（0-MAX_UNIT_COUNT）
【返回值说明】
【提示】
***************************************************************/
#ifdef BIWEI_WUXI_VERSION
char olp_osw[16]= {0};
#endif
/*
 *功能：
  1、插入单元盘
 *输入参数：
    chassis--机箱号，0为主机箱
    slot--槽位号，0为背板
    uclass-单元盘大类
    utype-单元盘小类
    bi-基本信息
    model--单元盘型号
    uproperty-单元盘属性
    send_trap -是否发送告警。0-不发送告警 1-发送告警
 */
void insertUnit(uchar_t chassis,uchar_t slot,enum UnitClass uclass,uchar_t utype,unit_base_info_t bi,char model[],char *uproperty,char send_trap)
{
    struct timeval tpstart;
    unit_base_info_t *pbi;
    uchar_t *us_shm;
    char property[32]= {0},*propertyMap;
    char unit_class_name[8]="";
    char *values;
    
    //if (slot >getSlotOfNmu(0,5))
    //    return;
    if (uclass<1 || uclass==0xff || utype<1 || utype==0xff)
    {
        printf("Class(%d) or type(%d) is invalid of Unit #%d model=%s!\r\n",uclass,utype,slot,bi.model);
        return;
    }
    us_shm=getUnitStatusMap(0);
    if (NULL==us_shm)
        return;
    pbi=getBaseInfoMap(0,slot);
    if (NULL==pbi)
        return;

    //memcpy(pbi,&bi,sizeof(unit_base_info_t));
    gettimeofday(&tpstart,NULL);
    pbi->uptime=tpstart.tv_sec;
#ifdef _DEBUG_
    printf("Unit #%d Uptime:%d\n",slot,tpstart.tv_sec);
#endif

    //add the class of unit
    *(us_shm+slot)=uclass;
    if (utype<1)
        utype=1;
    loadInfosets(0,slot,uclass,utype,uproperty);
	
    //add the sub class of unit
    *(us_shm +MAX_UNIT_COUNT+slot)=utype;
    
    getUnitLabelName(uclass,utype,unit_class_name);
    propertyMap=getUnitPropertyMap(0,slot);
    if (propertyMap!=NULL)
    {
        memcpy(propertyMap+4,uproperty,48);
        if (utype==41)
        {
            int n;
            for(n=0; n<16; n++)
                printf("%d:%d ",n,uproperty[n]);
            printf("\n");
        }
        if (UC_PASSIVE!=uclass)
        {
            if (UC_OTU!=uclass)
            {
                if (uproperty[2]>1)
                    *(us_shm +MAX_UNIT_COUNT*2+slot)=uproperty[2];
                else
                    *(us_shm +MAX_UNIT_COUNT*2+slot)=1;
            }
            else
                *(us_shm +MAX_UNIT_COUNT*2+slot)=1;
        }
        else
            *(us_shm +MAX_UNIT_COUNT*2+slot)=1;
    }
    else
        printf("property of slot %d is null\n",slot);

	//send alarm
    if (send_trap)
    {
        sendAlarm(52,0,slot,0,0,0,0,0,NULL);
        printf("%s(%s) on slot #%d inserted\n",unit_class_name,model,slot);
    }
    else
        printf("%s(%s) on slot #%d exist\n",unit_class_name,model,slot);
}

/*
 *功能：
  1、拔出单元盘
 *输入参数：
    chassis--机箱号，0为主机箱
    slot--槽位号，0为背板
 */
void removeUnit(char chassis,char slot)
{
    uchar_t *us_shm;

    //char property[32]= {0};
#ifdef IS_TEMP_VERSION
    return;
    //char other_unit_class=0;
    if (slot<1  || slot>2)
        return;
    us_shm=getUnitStatusMap(0);

    if (NULL!=us_shm)
    {
        if (slot >=getSlotOfNmu(0,5) || *(us_shm+slot)<2)
            return;
    }
    unit_base_info_t *bi=NULL;
    /*
    if (slot==1)
    {
      bi=getBaseInfoMap(0,2);
      other_unit_class=getUnitClass(2);
    }
    else if (slot==2)
    {
      bi=getBaseInfoMap(0,1);
      other_unit_class=getUnitClass(1);
    }
    */
    FILE *fp;
    unit_base_info_t ubi;
    char tmp_buf[512]= {0};
    char buf[60]= {0};
    //char property[60]={0};
    char *property;
    char is_valid=0;
    bi=getBaseInfoMap(0,slot);
    if (bi==NULL)
        return;
    if (slot==1)
    {
        if (strstr(bi->model,"OTU"))
            is_valid=1;
    }
    else if (slot==2)
    {
        if (strstr(bi->model,"ODM"))
            is_valid=1;
    }
    if (is_valid)
    {
        sprintf(buf,"/vispace/unit%d.bi",slot);
        fp=fopen(buf,"wb");
        if (fp==NULL)
            return;
        //printf("fp!=NULL\r\n");
        fwrite(bi,sizeof(unit_base_info_t),1,fp);
        property=getUnitPropertyMap(0,slot);
        if (property!=NULL)
        {
            /* int n;
              printf("property!=NULL..slot=%d\r\n",slot);
             for (n=4;n<32;n++)
              printf("%d ",*(property+n));
             */
            fwrite(property+4,48,1,fp);
        }
        else
        {
            fwrite(buf,48,1,fp);
        }
        fclose(fp);
    }
    //printf("OTU on Slot=%d model=%s removed\r\n",slot,bi->model);
    if (slot==1 && is_valid)
    {
        char *shm=getTempShm();
        //char *shm= getInfoSetValues(0,1,51,1);
        bi=getBaseInfoMap(0,1);
        if (bi==NULL)
            is_valid=0;
        else if (strstr(bi->model,"OTU"))
            is_valid=1;
        else
            is_valid=0;
        if (is_valid)
        {
            fp=fopen("/vispace/unit.dat","wb");
            if (fp!=NULL)
            {
                if (shm!=NULL)
                {
                    //printf("Save OTU status!\r\n");
                    int n;
                    //shm+=512*10;
                    /*
                    for (n=0;n<32;n++)
                         printf("D=%d ",*(shm+n));
                    printf("Save OTU OK!\r\n");
                    */
                    fwrite(shm,512,1,fp);
                }
                else
                    fwrite(&tmp_buf,512,1,fp);
                fclose(fp);
            }
        }
    }
    //printf("Slot=%d b\r\n",slot);
    /*
    if (other_unit_class<1)
    {
        int ret=0;
        fp=fopen("/vispace/unit.data","rb");
        if (fp!=NULL)
        {
          if (slot==1)
            ret=fseek(fp,sizeof(unit_base_info_t)+48+512,SEEK_SET);
          if (!ret)
          {
            fread(&ubi,sizeof(unit_base_info_t),1,fp);
            fread(property,48,1,fp);
            fread(tmp_buf,512,1,fp);
          }
          fclose(fp);
        }
    }
    else
    {
       memcpy(&ubi,bi,sizeof(unit_base_info_t));
    }
    fp=fopen("/vispace/unit.data","wb");
    if (fp==NULL)
      return;


    fwrite(bi,sizeof(unit_base_info_t),1,fp);
    if (property!=NULL)
      fwrite(property,48,1,fp);
    else
      fwrite(buf,48,1,fp);
    fwrite(shm+512*10,512,1,fp);

    bi=getBaseInfoMap(0,2);
    property=getUnitPropertyMap(0,2);
    if (bi!=NULL && property!=NULL)
    {
      fwrite(bi,sizeof(unit_base_info_t),1,fp);
      fwrite(property,48,1,fp);
    }
    else
    {
      unit_base_info_t ubi;
      char odm[60]={0X01,0X01,0X01,0X03,0X02,0X01,0X01,0XCB,0XCE,0X0E,0X0D,0X00,0X00,0X00,0X00,0X00,0X00,0XFB,0X00,0XCC,0XCD,0X0E,0X0D,0X00,0X00,0X00,0X00,0X00,0X00,0XFC,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00};

      strcpy(bi.model,"ODM-1111");
      strcpy(bi.sn,"SN111-123A");
      strcpy(bi.fwver,"01.10.23");
      strcpy(bi.hwver,"01.01.01");
      strcpy(bi.creation,"2014-01-06");
      fwrite(&ubi,sizeof(unit_base_info_t),1,fp);
      fwrite(odm,48,1,fp);
    }
    */
    /*
    us_shm=getUnitStatusMap(0);
    if (NULL!=us_shm)
     *(us_shm+slot)=0;
    */
    //fclose(fp);
    //shm+=512*10;
    //   if (i==0 && shm!=NULL)
    //printf("#%d removed!!\r\n",slot);
    return;
#endif
#ifdef BIWEI_WUXI_VERSION
    if (slot>0 && slot<16)
        olp_osw[slot-1]=0;
#endif
    us_shm=getUnitStatusMap(0);
    if (NULL!=us_shm)
    {
        *(us_shm+slot)=0;
        //printf("Unit %d removed\n",slot);
        sendAlarm(51,0,slot,0,0,0,0,0,NULL);
        /*
        if (save_err)
        {
           time_t atime;
           time(&atime);
           doSystem("sed -i '1a Pid=%d Unit %d removed %d at %s\n' /vispace/err.log",getpid(),slot,ctime(&atime));
        }
        */
    }
}
int count=0;
char  checkMessage(uchar_t slot,uchar_t uclass,uchar_t utype,uchar_t infoset_id,uchar_t index,char *message)
{
    //char ret=0;
    if (slot>0)
    {
        switch (uclass)
        {
        case UC_OTU:
            break;
        case UC_OLP:
            return checkOlpMessage(slot,uclass,utype,infoset_id,index,message,debug.is_print_err,debug.is_save_io_err);
            break;
        }
    }
    else
    {
        chassis_t *pc=(chassis_t *)message;
        if (pc->fan_led<1 || pc->fan_led>2)
            return 0;
    }
    return 1;
}

//static int alarm_count=0;
/*
 *功能：
  1、分析报文并提取信息集，把有效的信息集保存到共享内存
  2、提取告警信息并要通过消息队列把告警信息发送到消息队列
 *输入参数：
    chassis--机箱号，0为主机箱
    slot--槽位号，0为背板
    message--Vispace报文
 */
void saveMessageToShm(uchar_t chassis,uchar_t slot,char *message)
{
    uchar_t index;
    uchar_t *pInfoSet;
    uchar_t infoset_id;
    short infoset_count;
    short pInfoSet_size;
    int n,m;
    char *values;
    uchar_t uclass=0,utype=0;

    if (slot>0)
        getUnitClassType(0,slot,&uclass,&utype);
    else
    {
        if (count>10)
            count=0;
        else
            count++;
    }
    //if (slot>0)
    // printf("slot=%d,uclass=%d,utype=%d\n",slot,uclass,utype);
    infoset_count=(short )(*message);
    //if (uclass==4)
    //printf("slot=%d,infoset_count=%d\n",slot,infoset_count);
    if (infoset_count>13)
        infoset_count=13;
    //else if (infoset_count<1)
    //infoset_count++;
    pInfoSet=message+2;  //first message position

//if (print_slot==slot)
//printf("#%d infoset_count=%d count=%d\n",slot,infoset_count,count);

    unsigned char alarm_size=4;
    if (debug.slot_to_print<20 && debug.slot_to_print==slot && count%3==0)
        printfMessage(slot,message,0,128);
    for(m=0; m < infoset_count; m++)
    {
        infoset_id=*(pInfoSet);
        //if (uclass==4)
        // printf("slot=%d,infoset_id=%d\n",slot,infoset_id);
        index=*(pInfoSet+1);
        if (index<1)
            index=1;
        pInfoSet_size=(short)(*(pInfoSet+2));//信息集字节长度

        // if (slot==2)
        //   printf("#%d pInfoSet_size=%d MAX_MESSAGE_SIZE:%d\n",m,pInfoSet_size,MAX_MESSAGE_SIZE);
        if (pInfoSet_size>MAX_MESSAGE_SIZE)
            pInfoSet_size=MAX_MESSAGE_SIZE;
        if (infoset_id!=ALARM_INFOSET_ID)
        {
            // if (uclass==4)
            // printf("#%d infoset_id=%d\n",slot,infoset_id);
            if (infoset_id!=10)
                values=getInfoSetValues(0,slot,infoset_id,index);
            else
            {
                values=getUnitPropertyMap(0,slot);
                if (values!=NULL)
                    values+=4;
                //memcpy(propertyMap+4,uproperty,48);
                //if (values!=NULL)
                //   printf("#%d peer slot=%d ip=%d.%d.%d.%d\n",slot,values[8],values[9],values[10],values[11],values[12]);
                //else
                //   printf("#%d infoset_id=10 NULL\n",slot);
            }
            if (values!=NULL)
            {
                //if (slot==2)
                //printf("infoset_id=%d index=%d found!\n",infoset_id,index);
                if (pInfoSet_size>MAX_MESSAGE_SIZE)
                    pInfoSet_size=MAX_MESSAGE_SIZE;
                if (debug.is_debug_mode)
                    checkMessage(slot,uclass,utype,infoset_id,index,pInfoSet+4);
                memcpy(values,pInfoSet+4,pInfoSet_size);
            }
            else
            {
                if (debug.is_debug_mode)
                    printf("Slot=%d infoset_count=%d infoset_id=%d values=NULL\n",slot,infoset_count,infoset_id);
            }
        }


        /************************************************************
        告警格式:
        |99|等于36-单个告警字节长度为6,否则为告警条数|告警总字节长度 2字节 {[告警号 2字节| index | 告警值(1字节 or 3字节)]...}
        **************************************************************/
        else //if (ALARM_INFOSET_ID==infoset_id)
        {
            unsigned short alarm_id;
            short alarm_count=0,alarm_value=0;
            char is_with_value=0;
            time_t alarm_time;
            unsigned short alarm_reason;

            alarm_count=(short )(*(pInfoSet+2));//告警总字节长度
            if (36==*(pInfoSet+1))
                alarm_size=6;
            else if (37==*(pInfoSet+1))
                alarm_size=12;
            //printf("infoset_count=%d alarm_size=%d pInfoSet_size=%d\n",infoset_count,alarm_size,pInfoSet_size);
            //if (alarm_size<6 || alarm_size>10)
            //  alarm_size=4;
            //printf("D0:%d D2:%d D3:%d\n",*(pInfoSet),*(pInfoSet+2),*(pInfoSet+3));
            //if (alarm_size!=4)
            alarm_count/=alarm_size;//告警条数
            //if (infoset_count>0)
            //printf("alarm_count =%d alarm len=%d on slot %d\n",infoset_count,*(pInfoSet+1),slot);

            if (alarm_count>20)
                alarm_count=20;
            if (alarm_count<1)
                break;
            /*printf("Unit #%d alarm count=%d\n",slot,infoset_count);
            for (m=0;m<40;m++)
              printf("%x ",*(pInfoSet+m));
            printf("\n");
            */
            values=pInfoSet+4;

            for (n=0; n<alarm_count; n++)
            {
                alarm_id=*(short *)(values);
                is_with_value=0;
                alarm_time=0;
                if (alarm_size==12)
                {
                    memcpy(&alarm_time,values+8,4);
                    alarm_reason=*(short *)(values+6);
                    if (*(values+3)==1)
                    {
                        alarm_value=*(short *)(values+4);
                        is_with_value=3;
                    }
                }
                else
                {
                    alarm_time=0;
                    alarm_reason=0;
                    if (alarm_size==6)//带值告警
                    {
                        if (*(values+3)==1 || alarm_id==409 || alarm_id==411 || alarm_id==413)
                        {
                            alarm_value=*(short *)(values+4);
                            is_with_value=3;
                        }
                        else
                        {
                            alarm_value=0x7fff;
                        }
                    }
                    else
                    {
                        alarm_value=0x7fff;
                    }
                }

                //printf("v0:%d v1:%d v2:%d v3:%d v4:%d v5:%d\n",values[0],values[1],values[2],values[3],values[4],values[5]);
                /*
                if (is_debug)
                {
                  //time_t atime;
                  //time(&atime);
                  printf("alarm_size=%d alarm_id=%d alarm value=%d on slot %d\n",alarm_size,alarm_id,alarm_value,slot);
                  //doSystem("sed -i '1a C:alarm_id=%d alarm value=%d on slot %d at %s\n' /vispace/err.log",alarm_id,alarm_value,slot,ctime(&atime));

                  if (save_err)
                      saveError("alarm_size=%d alarm_id=%d alarm value=%d on slot %d\n",alarm_size,alarm_id,alarm_value,slot);
                  if (notify)
                      sendNotification("alarm_size=%d alarm_id=%d alarm value=%d on slot %d\n",alarm_size,alarm_id,alarm_value,slot);
                }
                */
                //printf("alarm_id=%0d index=%d\n",alarm_id,values[2]);
                if (alarm_id<1)
                    break;
                //printf("Alarm_id=%d index=%d is_with_value=%d alarm_value=%d\n",alarm_id,values[2],is_with_value,alarm_value);
                //printf("%0x %0x %0x %0x\n",values[0],values[1],values[2],values[3]);
                //printf("\nsendSnmpTrap...alarm_id=%0d index=%d\n",alarm_id,values[2]);
                /*
                uchar_t uclass=getUnitClass(0,1);
                char port_name[8]="";
                if (values[2]>0 && values[2]<9)
                {
                 if (UC_OTU==uclass||UC_OLP==uclass)
                 {
                   uchar_t port=0;
                   if (UC_OTU==uclass)
                   {
                      if (values[2]%2)
                        sprintf(port_name,"L%d", values[2]/2+1);
                      else
                        sprintf(port_name,"C%d", values[2]/2);
                   }
                 }
                }
                */
                time_t now_time;
                time(&now_time);
                if (abs(now_time-alarm_time)>180)
                {
                    alarm_time=now_time;
                }
                /*
                if (debug.is_debug_mode && (alarm_id==501 || alarm_id==502))
                  {
                    //printfMessage(slot,message,0,64);
                    //printf("slot=%d alarm_id=%d alarm_value=%x(%d)\r\n",slot,alarm_id,alarm_value,alarm_value,alarm_count++);
                   if (alarm_size==12)
                   {
                      time_t now_time;
                      //memcpy(&now_time,values+8,4);
                      printf("Unit #%d alarm_code=%d %s",slot,alarm_id,ctime(&alarm_time));
                      time (&now_time);
                      printf("NMU #%d alarm_code=%d %s",slot,alarm_id,ctime(&now_time));
                   }
                 }
                */
                sendAlarm(alarm_id,0,slot,values[2],is_with_value,alarm_value,alarm_reason,alarm_time,NULL);
                //time_t now_time;
                //time (&now_time);
                //printf("After sendAlarm #%d alarm_code=%d %s",slot,alarm_id,ctime(&now_time));
                values+=alarm_size;
            }
            //printf("\nsendSnmpTrap...\n");
        }
        //pInfoSet =pInfoSet+ ((short)*(pInfoSet-2))+4; //next message start position
        //printf("sendSnmpTrap pInfoSet_size=%d,alarm_size=%d\n",pInfoSet_size,alarm_size);
        pInfoSet =pInfoSet+pInfoSet_size+4;//alarm_size;
    }
    //printf("End SaveMessage slot=%d,uclass=%d,utype=%d\n",slot,uclass,utype);
}
