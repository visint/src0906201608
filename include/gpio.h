#ifndef _GPIO_H_
#define _GPIO_H_

#define own_addr_0     32+22   //PB22
#define own_addr_1     32+23   //PB23
#define own_addr_2     32+24   //PB24
#define own_addr_3     32+25    //PB25

#define bp_int_line    64+15   //PC15
#define reset_bp_cpu   32+20   //PB20

#define led_pwr1        32+1   //PB1
#define led_pwr2        32+3   //Pb3
#define led_fan_green   32     //PB0
#define led_fan_red     32+2   //PB2
#define led_run_green   28     //PA28
#define led_run_red     29     //PA29

#define sfp1_exist_pin        7   //PA7
#define sfp2_exist_pin        6   //PA6

#define port_lan2_link_up_down 11   //port 2
#define port_lan1_link_up_down 8  //port 3

#define port_sfp2_link_up_down   10
#define port_sfp1_link_up_down   9


#define setup_require    4     //PA4
#define setup_respone    5     //PA5

#define led_green_state    1 
#define led_red_state      2 
#define led_off_state      3 

#define led_pwr1_num   1  
#define led_pwr2_num   2
#define led_fan_num    3
#define led_run_num    4

#define SYSFS_GPIO_DIR "/sys/class/gpio"

#define MAX_BUF 64
#endif
