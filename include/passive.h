#ifndef __PASSIVE_H
#define __PASSIVE_H 1
#define PASSIVE_OSU_UNIT 1
#define PASSIVE_ODM_UNIT 2
#define PASSIVE_ODM2_UNIT 3
#define PASSIVE_DCM4_UNIT 4
#define PASSIVE_DCM1_UNIT 5
#define PASSIVE_NET_UNIT 6
#define PASSIVE_ODM3_UNIT 7 //带红蓝光
#define PASSIVE_OADM_UNIT 8
#define PASSIVE_DCM2_UNIT 9
#define PASSIVE_ODM12_UNIT 12
//#define PASSIVE_AWG_UNIT 5
//PASSIVE信息

/*
typedef struct
{
  char if_type;//接口属性,高四位为左接口,0为IN,非0为OUT,该值代表OUT的IN
  char if1_split_rate;//左接口分光比
  char if2_split_rate;//右接口分光比
}osu_if_property;//接口类型
*/
typedef struct
{
  uchar_t type;
  uchar_t systemType;//1:粗波CWDM 2:密波DWDM
  uchar_t upgrade;//1:有升级端口 2:没有升级端口
  uchar_t use;//1:OM复用 2 :OD解复用 3:ODM复用/解复用
  uchar_t channelCount;//波道数 1-18
  uchar_t direction;//传输技术 1:双纤双向 2:单纤双向 3:单纤单向
  uchar_t interfaceType;//光接口类型 1:LC/UPC 2  SC/UPC 3:LC/APC 4:SC/APC
  uchar_t leftWaveLength[12];//左边接口,：0:NA 200:IN 201:OUT 202：COM，203：COM Tx，204：COM Rx,205: Upg Tx,206:Upg Rx，207:PASS,208;leftWaveLength[10]:251:OM 252:OD;如果是粗波,则1,2,3,...n分别对应1270,1290,1310,1330...;如果是密波,则C15-C64;
  uchar_t rightWaveLength[12];//右边接口,：0:NA 200:IN 201:OUT 202：COM，203：COM Tx，204：COM Rx,205: Upg Rx,206:Upg Rx，207:PASS;251:OM 252:OD;如果是粗波,则1,2,3,...n分别对应1270,1290,1310,1330...;如果是密波,则C15-C64;
  //char hasOmOd;//1:有OM OD 2:没有
  char reserved[12];//预留
}odmProperty_t;

typedef struct
{
 char channelCount;//波道数 取值范围:1-48
 char beginWaveLength;// 起始波长:17-60
 char topPortCount;//顶部接口数,2:2口,0或4:4口
 uchar_t rightTopTextType;//右上角印字--0:NA 1:MON
 char reserved[6];//预留
}odm2Property_t;

typedef struct
{
  char  type;
  char  osu_type;//OSU 类型 1-FBT 2-PLC
  char  mode; //OSU 单多模.1--单模 3-多模
  char  band_width;//带宽.1-单窗口 2- 双窗口 3- 三窗口 4-全波段
  char  groups;//组数.1-6组
  char  in_if_type; //输入接口类型.1-LC/UPC 2-SC/UPC 3-LC/APC 4-SC/APC
  char  out_if_type;//输出接口类型.1-LC/UPC 2-SC/UPC 3-LC/APC 4-SC/APC
  char  split_rate[6][5];//存放6组分光比,每组第1个字节为组数,2-5字节存放分光比
  //char  if_property[20];
  //osu_if_property if_property[20];
  char reserved[11];//预留
}osuProperty_t;
extern boolean_t getPassiveViewInfo(uchar_t chassis,uchar_t slot,uchar_t uclass,uchar_t utype,unitViewInfo_t *info);
#endif /* __PASSIVE_H */
