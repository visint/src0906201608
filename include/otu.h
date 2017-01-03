#ifndef __OTU_H
#define __OTU_H 1

/*OTU单元盘最大通道数*/
#define OTU_MAX_CHANNEL_COUNT 2
#define OTU_EVERY_CHANNEL_PORT_COUNT 2

#define OTU_TYPE_2Dot5G_SFP_A 2
#define OTU_TYPE_10G_XFP_A 1

#define OTU_TYPE_10G_SFP_PLUS_A 5
#define OTU_TYPE_10G_SFP_PLUS_1U 6 //for 1U OTU

#define OEO_TYPE_2Dot5G_SFP_A 2
#define OEO_TYPE_10G_XFP_A 1
#define OEO_TYPE_10G_SFP_PLUS_A 7

typedef struct
{
  unsigned short  distance;
  unsigned short  max_speed;
  char  wave_length[7];
}otuChannelProperty;

typedef struct
{
  unitProperty_t unit_property;
  //uchar_t otu_type;
  uchar_t channel_count;
  uchar_t channel_port_count;
  uchar_t port_type;
  otuChannelProperty channel_property[OTU_MAX_CHANNEL_COUNT];
}otuProperty_t;

typedef struct
{
  unit_property_t unit_property;
  uchar_t port_count;
  uchar_t port_type;

}otu_property_t;

typedef struct
{
  unsigned  short distance;//传输距离
  unsigned  short max_speed;//最大速率
  unsigned  short wave_lenght;//端口1波长
  short current_speed;//速率
}otuPortProperty_t;

typedef struct
{
 uchar_t port_status; //工作状态 1—正常 2—不正常
 uchar_t on_module; //在位状态
 uchar_t tx_enable; //TX关断状态 1—enable 2—disable
}otuPortStatus_t;

typedef struct
{
 unsigned short current_speed;
 otuPortStatus_t  port_status[OTU_EVERY_CHANNEL_PORT_COUNT];
}otuChannelStatus_t;

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
 uchar_t port_status; //工作状态 1—正常 2—不正常
 uchar_t module_exist; //在位状态 1—在位 2—不在位
 uchar_t tx_enable; //TX关断状态 1—enable 2—disable
}sfp_status_t;

typedef struct
{
 unsigned short  distance; //在位状态 1—在位 2—不在位
 unsigned short  max_speed;
 unsigned short  wave_length; //工作状态 1—正常 2—不正常
 short  current_speed;
}sfp_info_t;

typedef struct
{
 uchar_t loopback; //TX关断状态 1—enable 2—disable
 uchar_t fec; //在位状态 1—在位 2—不在位
 uchar_t reserve1;
 uchar_t reserve2;
}sfp_loopback_t;

typedef struct
{
 sfp_loopback_t lb[8];
}sfp_lb_info_t;

typedef struct
{
 sfp_status_t sfp_status[4];
 sfp_info_t sfp_info[4];
}otu_port_group_t;


typedef struct
{
  uchar_t channel_count;
  channel_t channel[OTU_MAX_CHANNEL_COUNT];
}otuInfo_t;

typedef struct
{
  uchar_t reserve1;
  uchar_t reserve2;
  uchar_t work_mode;
  uchar_t run_status; 
  otuPortStatus_t portStatus[8];
  uchar_t leds[8];
}otuUnitStatus_t;

typedef struct
{
  uchar_t loopback;
  uchar_t fec;
  uchar_t reserve1;
  uchar_t reserve2;
}otuPortFEC_t;

typedef struct
{
    short exist;//0 no, 1 exist
    short power_status;//0 off, 1 on
    int   led_status;// 2 bits per led, 00 off, 01 green, 10 red
    int   ddm_or_los;//00 ddm, 01 los
    int   tx_status;// 2 bits per optical, 00 off, 01 on, 10 auto
    short rx_status;//0 , 1
    short src_thld;//0 optical, 1 user
}__attribute__((packed))otu1XInfo_t;

extern boolean_t getOtuViewInfo(uchar_t chassis,uchar_t slot,uchar_t uclass,uchar_t utype,unitViewInfo_t *info);
//OTU单元盘SFP端口属性(报文)信息集 UNIT_SFP_PROPERTY_INFOSET_ID
otuPortProperty_t *getOtuPortProperty(int chassis,int slot,int port);
//OTU单元盘(报文)信息集OTU_STATUS_INFOSET_ID
otuUnitStatus_t *getOtuStatus(int chassis,int slot);
//OTU单元盘(报文)信息集 OTU_SFP_FEC_INFOSET_ID
otuPortFEC_t *getOtuFec(int chassis,int slot,int port);

sfp_status_t *getSfpStatus(uchar_t chassis,uchar_t slot);
sfp_info_t *getSfpInfo(uchar_t chassis,uchar_t slot,uchar_t index);
sfp_loopback_t *getSfpLoopback(uchar_t chassis,uchar_t slot);

//int getOtuPortGroupInfo(group,otu_port_group_t *port_group);
int insertOtuInfoSets(uchar_t chassis,uchar_t slot,otuProperty_t otu,uchar_t utype);
//bool_t getOtuInfo(uchar_t chassis,uchar_t slot,uchar_t channel,otuInfo_t *info);
otuInfo_t *getOtuInfo(uchar_t chassis,uchar_t slot,uchar_t channel);
otuProperty_t *getOtuProperty(uchar_t chassis,uchar_t slot,uchar_t index);
otuChannelStatus_t *getOtuChannelStatus(uchar_t chassis,uchar_t slot,uchar_t group);

/*设置OTU工作模式*/
int setOtuWorMode(int chassis,int slot,uchar_t mode);
/*设置OTU光模块速率*/
int setOtuPortSpeed(int chassis,int slot,int port,unsigned short speed);
/*设置OTU光模块发送端关断状态*/
int setOtuPortTx(int chassis,int slot,int port,uchar_t enale);
/*设置OTU光模块 发送端关断状态 自环及FEC ，loopback或fec为0时则不设置该项*/
int setOtuLoopBackAndFec(int chassis,int slot,int port,uchar_t loopback,uchar_t fec);


#endif /* __OTU_H */
