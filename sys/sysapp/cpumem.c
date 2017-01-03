#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <time.h>
#include <sys/time.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sysinfo.h>

//#define WEBS_SERVICE "/vispace/webs/webs&"
//#define WEBS_PATH "/home/visint/VisProj/src/webs/LINUX"
#define WEBS_PATH "/visipace/webs"
#define SNMPD_SERVICE "snmpd -c /vispace/snmp/snmpd.conf"
#define CLID_SERVICE "/vispace/webs/webs&"
//gcc -o watchdogd watchdog.c -I./../../include -L./../../linuxlib -lvispace -liconv -lsqlite3


//arm-linux-gcc -o cpumem cpumem.c -I./../../include -L/home/visint/src/arm/lib -lvispace -liconv -lsqlite3
void main()
{
 float sys_usage, user_usage,mem_usage;
    struct mem_usage_t memery;
    if (!getCpuUsage(&sys_usage,&user_usage))
    {
      printf("CPU usage user:%.1f%%\tsys:%.1f%% Over!\n",user_usage,sys_usage);
    }
  mem_usage=getMemUsage(&memery);
  if (mem_usage>0)
    {
      printf("%lu %lu %lu %lu %lu %lu\n",memery.total,
	memery.used,memery.free,memery.shared,memery.buffers,memery.cached);
       printf("Mem usage:%.1f%% over!\nreboot...\n",mem_usage);
    }
}
