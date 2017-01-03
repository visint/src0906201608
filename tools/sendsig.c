//arm-linux-gcc -o sendsig sendsig.c -I./../include -L/home/visint/src/arm/lib -lvispace 
#include <io.h> 
#include <stdlib.h> 
#include <stdio.h>
#include <signal.h>
#include <visipc.h>
void main(void)
{
   union sigval val;
   val.sival_int = SIG_DEST_HOST_MODIFIED_ID;
   if (sendSig(findPid("alarmd"),SIGINT,val)!=0)
   {
        perror("sigqueue error");
        exit(EXIT_FAILURE);
    }
/*
    struct sigaction act;
    act.sa_sigaction = sighandler;
    sigemptyset(&act.sa_mask);
    act.sa_flags = SA_SIGINFO;//信息传递开关
    if(sigaction(SIGINT,&act,NULL) == -1){
        perror("sigaction error");
        exit(EXIT_FAILURE);
    }
    sleep(2);
    union sigval mysigval;
    mysigval.sival_int = 100;
    if(sigqueue(findPid("alarmd"),SIGINT,mysigval) == -1){
        perror("sigqueue error");
        exit(EXIT_FAILURE);
    }
    return 0;
*/
}

