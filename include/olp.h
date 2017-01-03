#ifndef __OLP_H
#define __OLP_H 1


#define OLP1_L1_SW_POWER 4
#define OLP1_L2_SW_POWER 5
#define OLP1_TX_ALM_POWER 6
#define OLP1_RET_TIME 7

#define OLP1_MODE 8
#define OLP1_LINE 9
#define OLP1_RULE 10
#define OLP1_RET_MODE 11

#define OLP1_R1_WAVE_LEN 15
#define OLP1_R2_WAVE_LEN 16
#define OLP1_TX_WAVE_LEN 17
#define OLP1_MASTER 18
#define OLP1_BF_WAVE_LEN 19

#define OLP_MIN_RET_TIME 0
#define OLP_MAX_RET_TIME 999

#define OLP_MIN_L1_SW_POWER -500
#define OLP_MAX_L1_SW_POWER 100

#define OLP_MIN_L2_SW_POWER -500
#define OLP_MAX_L2_SW_POWER 100

#define OLP_MIN_TX_ALM_POWER -500
#define OLP_MAX_TX_ALM_POWER 100

#define OLP_MIN_RET_TIME 0
#define OLP_MAX_RET_TIME 999
#pragma pack(2) /* n = 1, 2, 4, 8, 16 */
enum Olp_Type
{
  Olp_Type13=13
};
typedef struct
{
  uchar_t reserved[11]; //1-单纤双向 非1-双纤双向  
  uchar_t port_type; //接口类型
  uchar_t olp_type; //OLP 类型  1-1+1   2-1:1
  uchar_t struct_type; //结构类型 1-长距 2-短距
}olp_property_t;

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
  uchar_t  operating_mode;//工作模式:1--自动 2--手动
  uchar_t  rule;
  uchar_t  line;//1:切换到主路(L1)  2:切换到备路(L2);工作模式为手动时才能设置
  uchar_t  ret_mode;
}olp1StatusInfo_t;

typedef struct
{
  uchar_t  operating_mode;
  uchar_t  line;
}olp2StatusInfo_t;
typedef struct
{
  short rx1_power; //R1光功率
  short rx2_power; //R2光功率
  short tx_power;  //Tx光功率
  short l1_power;
  short l2_power;
  short tx_alm_power;
  short ret_time;  //返回时间:0-999

  uchar_t  mode;//工作模式:1--自动 2--手动
  uchar_t  line;//线路选择:1:切换到主路(L1)  2:切换到备路(L2);工作模式为手动时才能设置
  uchar_t  rule; //切换规则
  uchar_t  ret_mode;//返回模式
  //uchar_t  unused;
  uchar_t rx1_led;
  uchar_t rx2_led;
  uchar_t tx_led;
  uchar_t rx1_wave_len;
  uchar_t rx2_wave_len;
  uchar_t tx_wave_len;
  
  uchar_t master;
  uchar_t reserved;
}__attribute__((packed))olp_base_t;

typedef struct
{
  short rx1_power; //R1光功率
  short rx2_power; //R2光功率
  short tx_power;  //Tx光功率
  short l1_power;
  short l2_power;
  short tx_alm_power;
  short ret_time;  //返回时间:0-999

  uchar_t  mode;//工作模式:1--自动 2--手动
  uchar_t  line;//线路选择:1:切换到主路(L1)  2:切换到备路(L2);工作模式为手动时才能设置
  uchar_t  rule; //切换规则
  uchar_t  ret_mode;//返回模式
  //uchar_t  unused;
  uchar_t rx1_led;
  uchar_t rx2_led;
  uchar_t tx_led;
  uchar_t rx1_wave_len;
  uchar_t rx2_wave_len;
  uchar_t tx_wave_len;
  
  uchar_t master;
  uchar_t reserved;
  short rx_power; //Rx光功率
  short t1_power;
  short t2_power;
  /*
   uchar_t backup_fiber_wave_len;
  short  backup_fiber_power;
  */
}__attribute__((packed))olp1P1Info_t;

typedef struct
{
  short rx1_power; //R1光功率
  short rx2_power; //R2光功率
  short tx_power;  //Tx光功率
  short l1_power;
  short l2_power;
  short tx_alm_power;
  short ret_time;  //返回时间:0-999

  uchar_t  mode;//工作模式:1--自动 2--手动
  uchar_t  line;//线路选择:1:切换到主路(L1)  2:切换到备路(L2);工作模式为手动时才能设置
  uchar_t  rule; //切换规则
  uchar_t  ret_mode;//返回模式
  //uchar_t  unused;
  uchar_t rx1_led;
  uchar_t rx2_led;
  uchar_t tx_led;
  uchar_t rx1_wave_len;
  uchar_t rx2_wave_len;
  uchar_t tx_wave_len;
  
  uchar_t master;
  uchar_t backup_fiber_wave_len;
  short  backup_fiber_power;
  short  mf_power_switch;
  short  bf_power_switch;
}__attribute__((packed))olpInfoWithBakupFiber_t;

typedef struct
{
  short rx_power[4]; //R1-R4光功率
  /*short rx1_power; //R1光功率
  short rx2_power; //R2光功率
  short rx3_power; //R3光功率
  short rx4_power; //R4光功率
*/
  //short tx_power;  //Tx光功率
  short l_switch_power[2]; //R1-R2切换功率
  short rx_alm_power[2];  //R3-R4告警功率
  //short r4_alm_power;  //R4告警功率
  short ret_time;  //返回时间:0-999

  uchar_t  mode;//工作模式:1--自动 2--手动主路 3-手动旁路 4-自动+心跳
  uchar_t  line;//线路选择:1:切换到主路(L1)  2:切换到旁路(L2);工作模式为手动时才能设置
  uchar_t  rule; //切换规则
  uchar_t  ret_mode;//返回模式 0-R1和R2 无光切换 1-R1 无光切换 2-R2 无光切换 3-R1或R2无光切换

  //uchar_t  unused;
  uchar_t rx1_led;
  uchar_t rx2_led;
  uchar_t auto_led;
  uchar_t set_led;
  /*
  uchar_t rx2_led;
  uchar_t rx3_led;
  uchar_t rx4_led;
  */
  uchar_t rx_wave_len[4];
  /*uchar_t rx2_wave_len;
  uchar_t rx3_wave_len;
  uchar_t rx4_wave_len;
*/
  uchar_t heart_beat;
}__attribute__((packed))obpInfo_t;

typedef struct
{
  short t1_power;
  short t2_power;
  short r1_power;
  short r2_power;
  short tx_power;
  short rx_power;
  short  mf_bp_alm_power;//主纤备光告警功率
  short  bf_bp_alm_power;//备纤备光告警功率
  short  tx_alm_power;
  short  mf_switch_power;//主纤业务切换功率
  short  bf_switch_power;//备纤业务切换功率
  short  mf_loss;//主纤插损切换阈值
  short  bf_loss;//备纤插损切换阈值

  short ret_time;  //返回时间:0-999

  uchar_t  mode;//工作模式:1--自动 2--手动
  uchar_t  line;//线路选择:1:切换到主路(L1)  2:切换到备路(L2);工作模式为手动时才能设置
  uchar_t  ret_mode;//返回模式
  //uchar_t  unused;
  uchar_t led_status;
  uchar_t key_func;
  uchar_t r1_wave_len;
  uchar_t r2_wave_len;
  uchar_t tx_wave_len;
}__attribute__((packed))olp4XInfo_t;

typedef struct
{
  olp1P1Info_t olp_base;
  short t2_1490_power;
  uchar_t isolation;//隔离度
}__attribute__((packed))olpWithIsolation_t;

olp_property_t *getOlpProperty(uchar_t chassis,uchar_t slot);
obpInfo_t *getObpInfo(uchar_t chassis,uchar_t slot);
olp1P1Info_t *getOlp1P1Info(uchar_t chassis,uchar_t slot);
olp1P1Info_t *getOssChannelInfo(uchar_t chassis,uchar_t slot,uchar_t channel);

extern boolean_t getOlpViewInfo(uchar_t chassis,uchar_t slot,uchar_t uclass,uchar_t utype,unitViewInfo_t *info);
int setOlpInfo(setInfoSet_t infoset);
#pragma pack(2)
#endif /* __OLP_H */
