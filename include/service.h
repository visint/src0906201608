#ifndef __SERVICE_H
#define __SERVICE_H 1

#define SNMPD_SERVICE_ID 2
#define WEBS_SERVICE_ID 3
#define CCENTER_SERVICE_ID 9
#define OLP_SERVICE_ID 10
#define HEART_BEAT_SERVICE_ID 11 //心跳
/**********************************************************************
* 函数名称： setStartUpService
* 功能描述： 设置服务启动配置
* 访问的表： 无
* 修改的表： 无
* 输入参数： char service_id,char boot,char watchdog
*           service_id:服务ID
*	    boot:开机启动配置
*           	         0 --- 不设置 
*			 1 --- 开机启动 
*			 2 --- 开机不启动
*	    watchdog:看门狗监控
*           	         0 --- 不设置 
*			 1 --- 看门狗监控 
*			 2 --- 看门狗不监控
* 输出参数： 无
* 返 回 值： 无 
* 其它说明： 无
* 修改日期        版本号     修改人       修改内容
* -----------------------------------------------
* 2012/6/23        V1.0      李文峰        创建
* 
***********************************************************************/
void setStartUpService(char service_id,char boot,char watchdog);
#endif /* __PUBLIC_H */
