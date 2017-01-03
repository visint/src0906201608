#include <stdio.h>  
#include <stdlib.h>  
#include <sys/wait.h>  
#include <sys/types.h> 

//arm-linux-gcc -o testu testupgrade.c -I../include -L=./ -L/home/visint/src/arm/lib -lvispace -liconv -lsqlite3
int main (int argc, char **argv)
{
  pid_t status=system("ftransfer -f otu-3-5.bin");  
  //printf("result=%d\n",system("ftransfer -f otu-3-5.bin"));
  printf("wifexited(status):%d\n", WIFEXITED(status));  
  printf("WEXITSTATUS(status):%d\n", WEXITSTATUS(status));  
}
