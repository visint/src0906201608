#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

int sendSig(pid_t pid,int sig,int  sigval)
{
  union sigval sig_val;

  if (pid<1)
  {
    printf("Process not run!pid=%d\n",pid);
    return -1;
  }
  sig_val.sival_int=sigval;
  if(sigqueue(pid,sig,sig_val)==-1)
  {
     perror("sendSig error\n");
     return -1;
  }
  return 0;
}

int findPid(char *name)
{
     char buf[100];
     sprintf(buf, "ps -A | grep %s", name);
     FILE *fp = popen(buf, "r");
     if (fgets(buf, sizeof(buf), fp) != NULL)
      {
          pclose(fp);
          return atoi(buf); /* 转换成PID*/
      }
     pclose(fp);
     return 0; /*发送0作为没找结束标志*/
}
int main(int argc, char **argv)
{
int sigval=9;
int sival_int=SIGUSR1;
if (argc>0)
 return sendSig(findPid(argv[1]),sival_int,sigval);
//if(sigqueue(4403,SIGUSR1,value)==-1){//4403是目标进程pid
//perror("信号发送失败\n");
sleep(2);
return 0;
}

