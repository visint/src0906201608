#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <public.h>
//arm-linux-gcc -o setacl  setacl.c -I../include -L=./ -L/home/visint/src/arm/lib -lvispace
//http://sinykk.iteye.com/blog/1217094
void main (int argc, char **argv)
{
  char value[512]="";
  getConfValue(SYS_CONF_FILE,"ipwl",value," ");
  char* const delim = ";";
  char *token, *cur = value;
  while (token = strsep(&cur, delim)) {
    if (!strstr(token,"-"))
    {
       printf("set ip range:%s\n", token);
       doSystem("iptables",token);
    }
    else if (!strstr(token,"/"))
    {
      printf("set ip seg=%s\n", token);
      doSystem("iptables -I INPUT -s %s -p tcp --dport 80 -j ACCEPT",token);
    }
    else
        //doSystem("iptables -I INPUT -s 192.168.1.201 -p tcp --dport 9889 -j ACCEPT",token);
    printf("set ip=%s\n", token);
    printf("%s\n", token);
  }
}
