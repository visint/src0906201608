#ifndef _IOUTIL_H_
#define _IOUTIL_H_

#define SHMKEY_SET 1236L

#define SHARE_EMTPY  0X55
#define SHARE_FULL     0XAA
#define SET_STATUS_GOOD 0x55
#define SET_STATUS_BAD 0xAA
#define MAX_ERR_COUNT 5

#define TIOCSRS485 0x542F

#define DEBUG_IO_ 1

//low level communication command
#define cmd_get_fix_info      0x01  //获取板卡固定信息
#define cmd_get_runtime       0x02  //获取板卡动态信息
#define cmd_get_alarm         0x03  //获取板卡告警信息

#define cmd_bp_valid_info     0x04

#define cmd_set_runtime_1 0x05  //设置信息集，预留7个
#define cmd_set_runtime_2 0x06
#define cmd_set_runtime_3 0x07
#define cmd_set_runtime_4 0x08
#define cmd_set_runtime_5 0x09
#define cmd_set_runtime_6 0x0a
#define cmd_set_runtime_7 0x0b

#define cmd_set_time_update 0x50  //time update

#define cmd_set_file_tranfer 0xa0  //file transfer

#define cmd_set_fix_para1 0x20  //设置固定信息，预留11条
#define cmd_set_fix_para2 0x21
#define cmd_set_fix_para3 0x22
#define cmd_set_fix_para4 0x23
#define cmd_set_fix_para5 0x24
#define cmd_set_fix_para6 0x25
#define cmd_set_fix_para7 0x26
#define cmd_set_fix_para8 0x27
#define cmd_set_fix_para9 0x28
#define cmd_set_fix_para10 0x29
#define cmd_set_fix_para11 0x2a


#define cmd_update_start 0x80
#define cmd_update_proc  0x81
#define cmd_update_end   0x82

#define BP_ADDR             17 //背板CPU  RS485地址
#define master_nmu_addr     0
#define slave_nmu_addr      15
#define BROADCAST_ADDR      0xff

#define RS485_TX_LEN  512    //发送长度
#define RS485_RX_LEN  512    //接收长度


struct solt_set
{
    char setbuf[512];
    unsigned char slot;
    unsigned char op_cmd;
    unsigned char set_flag;
    unsigned char stauts_flag;
	unsigned char run_led_keep_count;//add by liwf
	unsigned char run_led_status;//add by liwf

	//for ftc
	unsigned char ftc_flag;//0x55 busy, other idle
	int           ftc_err_code;
	unsigned char addr;
	unsigned int num_card;
	unsigned char addr_array[32];
	unsigned char prepare_sort;
	unsigned char path[128];
	unsigned int file_size;
};

#endif

