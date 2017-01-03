#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <public.h>
//arm-linux-gcc -o getconf getconf.c -I./../include -L/home/visint/src/arm/lib -lvispace -liconv -lsqlite3
int main(int argc, char **argv)
{
  //printf("Usage:getconf conf_item %d\n",argc);
  if (argc<2)
  {
    printf("Usage:getconf conf_item\n");
    return 0;
  }
  //printf("conf_item %s\n",argv[1]);
  char value[60]="";
  getConfValue(SYS_CONF_FILE,argv[1],value," ");
  printf(value);

  //getConfValue(SYS_CONF_FILE,"hb_enable",value," ");
  //printf("hb_enable:%s",value);
  return 1;
}
