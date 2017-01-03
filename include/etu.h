#ifndef __ETU_H
#define __ETU_H 1
//ETU信息
typedef struct
{
  uchar_t tp_speed;//电口速率
  uchar_t tp_link_status;//电口速率连接状态光口
  uchar_t tp_duplex;//电口双工
  uchar_t fp_link_status;//光口连接状态
  /*
  char   reserve1;
  char   reserve2;
  */
}etuStatus_t;
typedef struct
{
  uchar_t lan1_speed;//电口速率
  uchar_t lan1_link_status;//电口速率连接状态光口
  uchar_t lan1_duplex;//电口双工
  uchar_t sfp1_link_status;//光口连接状态
  uchar_t lan2_speed;//电口速率
  uchar_t lan2_link_status;//电口速率连接状态光口
  uchar_t lan2_duplex;//电口双工
  uchar_t sfp2_link_status;//光口连接状态
  uchar_t sfp1_exist;
  uchar_t sfp2_exist;
}
etuInfo_t;
extern etuStatus_t *getEtuStatus(uchar_t chassis,uchar_t slot);
#endif /* __ETU_H */
