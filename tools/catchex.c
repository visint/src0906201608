#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <time.h>
#include <sys/time.h>
#include <unistd.h>
#include <public.h>
//arm-linux-gcc -o catchex catchex.c -I./../include -L/home/visint/src/arm/lib -lvispace -liconv -lsqlite3
  
void fault_trap(int n,struct siginfo *siginfo,void *myact)  
{  
        int i, num;  
        //char **calls;  
        printf("Fault address:%X\n",siginfo->si_addr); 
        /*    
        num = backtrace(buffer, SIZE);  
        calls = backtrace_symbols(buffer, num);  
        for (i = 0; i < num; i++)  
                printf("%s\n", calls[i]);
        */ 
        exit(1); 
}  
void setuptrap()  
{  
    struct sigaction act;  
    sigemptyset(&act.sa_mask);     
    act.sa_flags=SA_SIGINFO;      
    act.sa_sigaction=fault_trap;  
    sigaction(SIGSEGV,&act,NULL);  
}  

void sighandler(int signum)  
{  

     printf("Process %d got signal %d\n", getpid(), signum);  
     signal(signum, SIG_DFL);  
     //kill(getpid(), signum);  
 }  


static void sigHandle1(int sig,struct siginfo *siginfo,void *myact)
{
  pid_t pid;
  switch (sig)
  {
   case SIGALRM:
        pid=getPidByName("pidof watchdogd");
        if (pid<1)
        {
           printf("Reboot:watchdogd killed\n");
           insertSysLog(119,"wd killed");
           sendAlarm(115,0,getSlotOfNmu(0,5),0,0,0);
           system("reboot");
        }
        alarm(150);
        return;
        break;
    case SIGINT:
        insertSysLog(119,"SIGINT");
        break;
    case SIGQUIT:
        insertSysLog(119,"SIGQUIT");
        break;
    case SIGTSTP:
        insertSysLog(119,"SIGTSTP");
        break;
    case SIGTTIN:
        insertSysLog(119,"SIGTTIN");
        break;
   case SIGTERM:
        insertSysLog(119,"SIGTERM");
        break;
   case SIGKILL:
        insertSysLog(119,"SIGKILL");
        break;
   case SIGSEGV:
        insertSysLog(119,"SIGSEGV");
        break;
  }
  printf("Caught exception:sig=%d\n");
}
static void sigInit()
{
  /*
  struct sigaction act;
  sigemptyset(&act.sa_mask);  
  act.sa_flags=SA_SIGINFO;
  act.sa_sigaction=sigHandle;

  sigaction(SIGALRM,&act,NULL);
 
  sigaction(SIGINT,&act,NULL);
  sigaction(SIGQUIT,&act,NULL);
  sigaction(SIGTSTP,&act,NULL);
  sigaction(SIGTTIN,&act,NULL);
  sigaction(SIGTERM,&act,NULL);
  sigaction(SIGKILL,&act,NULL);
  sigaction(SIGUSR1,&act,NULL);
  sigaction(SIGUSR2,&act,NULL);
  sigaction(SIGSEGV,&act,NULL);
  */
  signal(SIGSEGV, sighandler);  
}

int main(int argc, char **argv)
{
  if (lockFile("/vispace/ce.lck")<0)
  {
     exit(0);
  }
  //sigInit();
  setuptrap();
  //alarm(150);
  while (1)
  {	
    pause();
  }
}
