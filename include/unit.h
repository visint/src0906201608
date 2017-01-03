/*
 *  unit.h -- Vispace Unit Management public header
 *
 *  Copyright (c) Visint Inc., 2002-2012. All Rights Reserved.
 *
 *  See the file "license.txt" for information on usage and redistribution
 *
 */

#ifndef __UNIT_H
#define __UNIT_H 1

/******************************** Description *********************************/

/*
 *  Vispace Unit header. This defines the Unit Management
 *  public APIs.  Include this header for files that contain access to
 *  unit inquiry or management.
 */

/********************************* Includes ***********************************/

#include  <typedefs.h>
#include  <sys/time.h>
#include  <infoset.h>
/********************************** Defines ***********************************/
//#define MAX_CHASSIS_COUNT 1	/*��������*/
#define MAX_UNIT_COUNT 17	/*��Ԫ�������*/
#define MAX_GROUP_PORT_COUNT 2	/*ÿ��˿������*/
#define UNIT_STATUS_INFO_SET_ID 1	/*��Ԫ��״̬��Ϣ��*/
#define UNIT_BASE_INFO_SET_ID 2

#define MAX_BASE_INFO_COUNT 7
#define MAX_PORT_COUNT 10
/*��Ԫ�̻�����Ϣ*/
#define UNIT_PROPERTY_ID 1
#define UNIT_HW_VER_ID 2
#define UNIT_FW_VER_ID 3
#define UNIT_MODEL_ID 4
#define UNIT_SN_ID 5
#define UNIT_CREATION_ID 6
#define UNIT_UPTIME_ID 7

#define SUPPORT_UNIT uclass==3 || 4==uclass || 9==uclass

/*��Ԫ���û���Ϣ*/
#define UNIT_NAME_ID 1
#define UNIT_LOC_ID 2
#define UNIT_CONTACT_ID 3
#define UNIT_UP_DEV_ID 4
#define UNIT_USER_DATA_ID 5
#define UNIT_SERVICE_ID 6

#define MAX_ITEM_SIZE 32

#define MAX_GROUP_COUNT 4

#define LED_GREEN 1
#define LED_RED 2
#define LED_WHITE 3
/**********************************************
*��Ԫ������
*/
typedef struct
{
  uchar_t unit_use_class;     //��Ԫ����;����
  uchar_t unit_use_type;      //��Ԫ����;С��
  uchar_t hardware_type;  //��Ԫ��Ӳ������
  uchar_t firmware_type;  //��Ԫ�̹̼�����
  uchar_t struct_type;    //��Ԫ�̽ṹ����
  uchar_t port_count;
  uchar_t port_type[8];
}unitProperty_t;

typedef struct
{
  uchar_t reserved1;   
  uchar_t reserved2;      
  uchar_t hardware_type;  //��Ԫ��Ӳ������
  uchar_t firmware_type;  //��Ԫ�̹̼�����
  uchar_t struct_type;    //��Ԫ�̽ṹ����
  uchar_t reserved3;     
  uchar_t reserved4; 
}unit_property_t;
/**********************************************
*��Ԫ�̻�����Ϣ
*������Ϣ����ڵ�Ԫ��,ֻ����Ȩ�û������޸�
*/
typedef struct{
  char property[MAX_ITEM_SIZE];
  char sn[16]; 	              //��Ԫ�����к�
  char model[MAX_ITEM_SIZE];  //��Ԫ�ͺ�
  char creation[12];          //��Ԫ�̴������ڣ���ʽΪ��YYYY-MM-DD
  char fwver[12];             //��Ԫ�̹̼��汾��
  char hwver[12];             //��Ԫ��Ӳ���汾��
  time_t uptime;              //��Ԫ������ʱ�䣬��λΪ��
}unit_base_info_t;

/*������Ϣ�����Vispace.db���ݿ�,�û������޸�*/
typedef struct{
  char name[MAX_ITEM_SIZE];       //��Ԫ������
  char location[MAX_ITEM_SIZE];   //��Ԫ��λ��
  char contact[MAX_ITEM_SIZE];    //��Ԫ����ϵ��
  char updev[MAX_ITEM_SIZE];      //��Ԫ�������豸
  char userdata[MAX_ITEM_SIZE];   //��Ԫ���û��Զ�����Ϣ
  char service[MAX_ITEM_SIZE];    //��Ԫ�̿�ͨҵ��
}unit_user_data_t;

enum UnitClass
{
  UC_UNKOWN=0,
  UC_NMU=1,
  UC_MCU=2,
  UC_OTU=3,
  UC_OLP=4,
  UC_OBP=5,
  UC_OSS=6,
  UC_AWG=7,
  UC_TDM=8,
  UC_EDFA=9,
  UC_OEO=10,
  UC_DVOA=11,
  UC_ETU=12,
  UC_OTAP=13,
  UC_PASSIVE=55
};

enum PortClass
{
  PC_UNKOWN=0,
  PC_eport=1,
  PC_OPORT1X9=2,
  PC_SFP=3,
  PC_SFP_PLUS=4,
  PC_XFP=5,
  PC_ADAPTER=9
};

enum PortType
{
  PT_UNKOWN=0,
};

typedef struct
{
 uchar_t col_count;
 uchar_t row_count;
 uchar_t led_type;
 uchar_t run_led;
 char run_text[6];
 uchar_t leds_status[3][4];
 char leds_text[2][2][12];
}ledsInfo_t;

typedef struct
{
 uchar_t portCount;
 uchar_t ports_status[MAX_PORT_COUNT];
 char ports_htext[MAX_PORT_COUNT][12];//�˿���ͷ
 char ports_text[2][MAX_PORT_COUNT][12];//�˿�������SFP1
 char has_info;//�Ƿ��ж˿���Ϣ
 char ports_info_name[3][16];//�˿���Ϣ��
 char ports_info_val[MAX_PORT_COUNT][3][10];//�˿���Ϣֵ
 char ports_info_unit[3][4];//��λ����dBm��G
}porstInfo_t;

typedef struct
{
  uchar_t uclass;
  uchar_t utype;
  ledsInfo_t leds;
  porstInfo_t ports;
  char unit_name[6];
  char utype_text[64];
  char view_images_count;
  char view_images[4][16];
}unitViewInfo_t;

typedef struct
{
  char group_name[16];
  uchar_t port_count;
  enum PortClass port_class[MAX_GROUP_PORT_COUNT];
  uchar_t port_type[MAX_GROUP_PORT_COUNT];
  uchar_t port_status[MAX_GROUP_PORT_COUNT];
}portGroup_t;

typedef struct
{
  char unit_name[10];
  uchar_t group_count;
  portGroup_t port_group[MAX_GROUP_PORT_COUNT];
}panelInfo_t;

typedef struct
{
  char unit_name[10];
  uchar_t port_count;
  uchar_t port_type[MAX_GROUP_PORT_COUNT];
  uchar_t port_status[MAX_GROUP_PORT_COUNT];
  //uchar_t port_info[MAX_GROUP_PORT_COUNT][20];
}unitInfo_t;

/*
typedef struct
{
 char wave_length[8];
 char distance[6];
 char max_speed[6];
 char current_speed[6];
 uchar_t port_count;
 uchar_t rx_status[2];
 uchar_t tx_status[2];
 uchar_t tx_enable[2];
}channel_t;

typedef struct
{
  uchar_t channel_count;
  channel_t channel[OTU_MAX_CHANNEL_COUNT];
}otuInfo_t;
*/
/*
typedef struct
{
  short rx1_power,rx2_power,tx_power;
  short l1_power,l2_power,tx_alm_power;
}olpPowerInfo_t;

typedef struct
{
  olpPowerInfo_t power;
  short ret_time;
}olp1Data_t;

typedef struct
{
  uchar_t  operating_mode;//����ģʽ:1--�Զ� 2--�ֶ�
  uchar_t  rule;
  uchar_t  line;//1:�л�����·(L1)  2:�л�����·(L2);����ģʽΪ�ֶ�ʱ��������
  uchar_t  ret_mode;
}olp1StatusInfo_t;

typedef struct
{
  uchar_t  operating_mode;
  uchar_t  line;
}olp2StatusInfo_t;
*/


int isValidUnitClass(uchar_t uclass,uchar_t utype);

/* 
 *���ܣ�
  1�����뵥Ԫ�� 
 *���������
    chassis--����ţ�0Ϊ������
    slot--��λ�ţ�0Ϊ����
    uclass-��Ԫ�̴���
    utype-��Ԫ��С��
    bi-������Ϣ
    model--��Ԫ���ͺ�
    uproperty-��Ԫ������
    send_trap -�Ƿ��͸澯��0-�����͸澯 1-���͸澯
 */
extern void insertUnit(uchar_t chassis,uchar_t slot,enum UnitClass uclass,uchar_t utype,unit_base_info_t bi,char model[],char *uproperty,char send_trap);

/* 
 *���ܣ�
  1���������Ĳ���ȡ��Ϣ��������Ч����Ϣ�����浽�����ڴ� 
  2����ȡ�澯��Ϣ��Ҫͨ����Ϣ���аѸ澯��Ϣ���͵���Ϣ����
 *���������
    chassis--����ţ�0Ϊ������
    slot--��λ�ţ�0Ϊ����
    message--Vispace���� 
 */
extern void saveMessageToShm(uchar_t chassis,uchar_t slot,char *message);
/* 
 *���ܣ�
  1���γ���Ԫ�� 
 *���������
    chassis--����ţ�0Ϊ������
    slot--��λ�ţ�0Ϊ����
 */
extern void removeUnit(char chassis,char slot);


boolean_t checkUnitClassType(unit_base_info_t bi,uchar_t *unit_class,uchar_t *unit_type);
/************************************************************************
�����ܡ���ȡ��Ԫ������
������˵����
            1��uchar_t slo chassis_number  ����ţ�0-MAX_CHASSIS_COUNT��,0Ϊ������
            2��uchar_t slot ��λ�ţ�0-MAX_UNIT_COUNT��,0Ϊ���壨0-MAX_UNIT_COUNT��
            3��uchar_t index  ���Լ�������һ��Ϊ1
������ֵ˵�����ɹ����ص�Ԫ������ָ�룬ʧ�ܷ���NULL;
��ʹ�÷�����
            unitProperty_t *pro;
            pro = getUnitProperty (0,1,1);
***********************************************************************/
extern boolean_t getUnitViewInfo(uchar_t chassis,uchar_t slot,unitViewInfo_t *info);

//unitProperty_t *getUnitProperty(uchar_t chassis,uchar_t slot,uchar_t index);
char *getUnitProperty(uchar_t chassis,uchar_t slot);
int getUnitClassName(uchar_t uclass,char *class_name);
boolean_t getUnitPanelInfo(uchar_t chassis,uchar_t slot,panelInfo_t *info);
status_infoset_ex_t *getMapStatusInfoSet(uchar_t classis,uchar_t slot,uchar_t infoset_id);
char isSupportUnit(uchar_t uclass,uchar_t utype);
//boolean_t getOlp1P1Info(uchar_t chassis,uchar_t slot,olp1StatusInfo_t *info);
//boolean_t getUnitType(uchar_t classis,uchar_t slot,uchar_t *unit_class,uchar_t *unit_type,char *unit_short_name);
//extern void insertUnit(uchar_t chassis,uchar_t slot,enum UnitClass uclass,uchar_t utype,unit_base_info_t bi,char *uproperty,char send_trap);
//extern void removeUnit(char chassis,char slot);
//extern void saveMessageToShm(uchar_t chassis,uchar_t slot,char *message);

#endif /* __UNIT_H */
/******************************************************************************/
