#ifndef __DVOA_H
#define __DVOA_H 1
typedef struct
{
  short in_power; //IN光功率
  short out_power; //Out光功率
  uchar_t reserved1[2];
  short setting_otu_power;  //设置输出光功率
  short insertion_loss;//设置插损(手动)
  uchar_t reserved2[4];
  uchar_t mode;//工作模式:1--自动 2--手动
  uchar_t los1_led;//设置插损(手动)
  uchar_t lock_status;//锁定状态
  uchar_t los2_led;
  uchar_t lock_led;
  uchar_t los3_led;
  uchar_t auto_led;
  uchar_t wave_length;
  uchar_t reserved4[11];
}dvoa_info_t;

dvoa_info_t *getODvoaInfo(uchar_t chassis,uchar_t slot);
#endif /* __DVOA_H */
