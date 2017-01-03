#ifndef _CCENTER_H_
#define _CCENTER_H_

#define _NMU_V1_3_

#define MODIFY_BY_LIWF 1

#define POLL_TIMEOUT (1 * 1000) // 3 seconds


#ifdef BIWEI_WUXI_VERSION
#define NMU_FW_VER "BW1.06.08.77"
#else
#ifdef IS_TEMP_VERSION
#define NMU_FW_VER "KG1.06.08.77"
#else
#define NMU_FW_VER "1.06.08.78"
#endif
#endif

#define NMU_HW_VER "1.1.0.2"
#define LAST_MODIFY_DATE "2015-12-31"
#define SYS_CONF_FILE "/vispace/sys.conf"

typedef struct
{
    char is_debug_mode;
    char is_print_err;
    char is_save_io_err;
    char slot_to_print;
    int max_err_count;
    int max_syslog_count;
} debug_t;
extern debug_t debug;

typedef struct
{
    unsigned char type;// 1:v1.2 before 2:after 1.3
} nmu_hw_info_t;
#endif

