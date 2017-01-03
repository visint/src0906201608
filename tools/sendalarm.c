#include <unistd.h>
#include <public.h>
#include <vispaceio.h>
#include <dbutil.h>
//arm-linux-gcc -o sendalarm sendalarm.c -I./../include -L/home/visint/src/arm/lib -lvispace -liconv -lsqlite3
extern char save_alarm_detail;
struct solt_set *solt_set_shm_addr;
static void usage()
{
  printf("ver 1.1.26\nLast modifed:%s\n",LAST_MODIFY_DATE);
  printf("Usage:sendalarm [option] \n");
  printf("Option Specification:\n");
  printf("-c: count to send\n");
  printf("-i: alarm code\n");
  printf("-s: slot\n");
  printf("-p: port\n");
  printf("-m: interval(mseconds)\n");
  printf("-v: alarm value\n");
}

int main(int argc, char **argv)
{
 int n,count=1,oc,alarm_code=51,interval=300000;
 short alarm_value=0;
 char *b_opt_arg;
 char is_with_value=0;
 uchar_t slot=1;
 uchar_t port=0;
  
 while((oc = getopt(argc, argv, "s:i:c:p:v:h")) != -1)
 {
  switch(oc)
  {
    case 'i':
          alarm_code =atoi(optarg);
    break;
    case 'c':
          count=atoi(optarg);
    break;
    case 's':
          slot =(uchar_t)atoi(optarg);
    break;
    case 'p':
          port =(uchar_t)atoi(optarg);
    break;
    case 'v':
          alarm_value =(short)atoi(optarg);
          is_with_value=3;
    break;
    case 'h':
       usage();
       exit(0);
  }
 }
initAlarmConf();
/*
char value[16]="";
         getConfValue(SYS_CONF_FILE,"save_alarms",value," ");
         if (value[0]=='2')
            save_alarm_detail=2;
         else if (value[0]=='0')
            save_alarm_detail=0;
         else
             save_alarm_detail=1;
*/
int shmid_set=shmget(SHMKEY_SET,0,0666);
    //shmid_set=shmget(SHMKEY_SET,0,0666);
    //struct solt_set *addr;
    if (shmid_set!=-1)
      solt_set_shm_addr=(struct solt_set*)shmat(shmid_set,0,0);
    else
      solt_set_shm_addr=NULL;
printf("(alarm_code=%d,slot=%d,port=%d,is_with_value=%d,alarm_value=%d\r\n);",alarm_code,slot,port,is_with_value,alarm_value);

for (n=0;n<count;n++)
 {
  //removeUnit(0,1);
  if (n%100==0)
    printf("n=%d\n",n);
  sendAlarm(alarm_code,0,slot,port,is_with_value,alarm_value,0,0,NULL);
  usleep(interval);
 }
 printf("printf Enter to exit\r\n");
 char key = getchar();
 printf("Exit!\r\n");
}
