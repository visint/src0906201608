#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <public.h>
#include  <visipc.h>
#include  <sysinfo.h>
//arm-linux-gcc -o sysconf  sysconf.c -I../include -L=./ -L/home/visint/src/arm/lib -lvispace -liconv -lsqlite3
static void usage()
{
  printf("Description:\n");
  printf("System configure ver 1.0.16\nLast modified:%s\n\n",LAST_MODIFY_DATE);
  printf("Usage:sysconf [option] [value]\n");
  printf("Option Specification:\n");
  printf("-D [Y|N]: set debug mode.\neg:sysconf -D Y\n");
  printf("-I [Y|N]: set IO setting debug mode\n");
  printf("-r: remove log(a all)\n");
  printf("-d: show sys config\n");
  printf("-i: show sys config item value\n");
  printf("-p: pakage\n");
  printf("-s: show the debug infomation in log\n");
  //printf("-f: show the configure infomation\n");
  printf("-e: show the debug infomation(e-err.log) in log\n");
  printf("-E: save(default Not save) the err infomation to log\n");
  printf("-B: save(default Not save) the debug infomation to log\n");
  printf("-c: interval count(default:50) for saving the debug infomation to log\n");
  printf("-C: Clean the debug configuration and info\n");
  printf("-m: the max count(default:1000) to save the debug infomation to log\n");
  printf("-U [0|1|2|4|11|12]: set chassis type(0 as auto detect)\n11--1U 4 slots 12--BBU\n");
  printf("-u: show chassis type and slof of nmu(0 as auto detect)\n");
  printf("-S [0|2|3|6|7|15|16]: set slot of NMU(0 as auto detect)\n");
  printf("-T [0..10]: set type of NMU\n");
  printf("-F [Y|N]: set data from flash.\neg:sysconf -F Y\n");
  printf("-h for help.\n");
}

int main(int argc, char **argv)
{
  int n,m=0,oc,count=5,max_count=0;
  char opt=0,is_save=0,debug_flag='N',io_debug_flag='N';
  char buf[512]="";
  char *p;
  while((oc = getopt(argc, argv, "D:c:Cdoum:i:I:hsB:e:p:E:r:U:S:F:TV")) != -1)
  {
    if (oc=='V')
    {
          unit_base_info_t *nmu_bi;
          uchar_t slot=getSlotOfNmu(0,5);
          //printf("slot:%d\n",slot);
          if (slot<2 || slot>16)
              exit(0);
          nmu_bi=getBaseInfoMap(0,slot);
          if (NULL!=nmu_bi)
          {
            strcpy(nmu_bi->fwver,NMU_FW_VER);
            //printf("NMU_FW_VER:%s\n",NMU_FW_VER);
          }
          exit(0);
    }
    switch(oc)
    {
       case 'D':
           opt='D';
           if (!strcmp(optarg,"Y"))
             debug_flag='Y';
            break;
       case 'I':
           opt='I';
           if (!strcmp(optarg,"Y"))
             io_debug_flag='Y';
            break;
       case 'r':
          if (!strcmp(optarg,"a"))
           system("echo \"\">/vispace/err.log");
          else
           doSystem("echo \"\">%s",optarg);
       return;
       case 'c':
          count=atoi(optarg);
          if (count<3)
            count=3;
       break;
       case 'm':
          max_count=atoi(optarg);
          if (max_count<5)
            max_count=5;
       break;
       case 'd':
          printf("===sys.conf===\n");
          doSystem("cat %s",SYS_CONF_FILE);

          printf("\r\n===nmu.bi===\n");
          doSystem("cat %s","/vispace/nmu.bi");

          printf("\r\n===init.d===\n");
          system("cat /etc/init.d/Svispace");
          return;
       case 'i':
             getConfValue(SYS_CONF_FILE,optarg,buf," ");
             //printf("%s:%s",optarg,buf);
             printf("%s",buf);
             return;
       case 'e':
          if (!strcmp(optarg,"e"))
           doSystem("cat %s","/vispace/err.log");
          else
            doSystem("cat %s",optarg);        
          return;
       case 'p':
        strcpy(buf,"cd / && tar -zcf nmu");
        if (strlen(NMU_FW_VER)<12)
        {
          #ifdef DEFAULT_LANG_CN
            strcat(buf,"cn");
          #else
            strcat(buf,"en");
          #endif
        }
        else
        {
          sprintf(buf,"%s%c%c",buf,NMU_FW_VER[0],NMU_FW_VER[2]);
        }
        {
          char str[4][16]={""};
          int val[4]={0};
          n=sscanf(NMU_FW_VER,"%d.%d.%d.%d",&val[0],&val[1],&val[2],&val[3]);
          sprintf(buf,"%s%02d%02d.",buf,val[2],val[3]);
          sscanf(LAST_MODIFY_DATE,"%d-%d-%d",&val[0],&val[1],&val[2]);
          sprintf(buf,"%s%02d%02d.tar.gz ",buf,val[1],val[2]);
        }
        //n=strlen(NMU_FW_VER);
        //p=NMU_FW_VER+(n-5);
        strcat(buf,"bin/clish etc/xml-examples bin/snmpd bin/ccenter bin/watchdogd  bin/snmpget bin/snmpwalk bin/setup bin/sysconf bin/initup bin/initu bin/olputil bin/heartbeat vispace/sys.conf vispace/snmp/snmpd.conf vispace/webs/webs vispace/www");
        printf("%s\n", buf);
        system("cd /");
        system(buf);
          return;
       case 'E':
          if (!strcmp(optarg,"Y") || !strcmp(optarg,"N"))
          setConfValue(SYS_CONF_FILE,"save_err",optarg," ");
       break;
       case 'B':
          if (!strcmp(optarg,"Y") || !strcmp(optarg,"N"))
          setConfValue(SYS_CONF_FILE,"save_dbg",optarg," ");
          return;
       break;
       case 'C':
          setConfValue(SYS_CONF_FILE,"debug","N"," ");
          setConfValue(SYS_CONF_FILE,"save_dbg","N"," ");
          setConfValue(SYS_CONF_FILE,"save_err","N"," ");
          system("echo \"\">/vispace/err.log");
          system("killall -9 ccenter");
          sleep(1);
          system("ccenter&");
          return;
       break;
       case 'o':
          setConfValue(SYS_CONF_FILE,"debug","N"," ");
          system("killall -9 ccenter");
          sleep(2);
          system("ccenter&");
          return;
       case 'u':
            system("sed -n '1,2p' /vispace/nmu.bi");
           return;
       case 'U':
          //system("sed -i '/cclass/'d /vispace/nmu.bi");
          //system("sed -i '/nslot/'d /vispace/nmu.bi");

          if (!strcmp(optarg,"0") || !strcmp(optarg,"1") || !strcmp(optarg,"2") ||!strcmp(optarg,"4") ||!strcmp(optarg,"11")||!strcmp(optarg,"12"))
          {
            system("sed -i '/cclass/'d /vispace/nmu.bi");
            if (strcmp(optarg,"0"))
            {
            doSystem("sed -i '1i cclass %s' /vispace/nmu.bi",optarg);
            //setConfValue("/vispace/nmu.bi","cclass",optarg," ");
            printf("Chassis type set as %sU\r\n",optarg);
            }
            else //if (!strcmp(optarg,"0"))
            {
              system("sed -i '/nslot/'d /vispace/nmu.bi");
              system("sed -i '1i cclass 0' /vispace/nmu.bi");
              system("sed -i '1a nslot 0' /vispace/nmu.bi");
             //setConfValue("/vispace/nmu.bi","cclass","0"," ");
             //setConfValue("/vispace/nmu.bi","nslot","0"," ");
            }
          }
          
          else
              printf("Chassis type must be 0,1,2,4,11,12\r\n");
          return;
       case 'S':
          n=getConfIntValue("/vispace/nmu.bi","cclass"," ");
          if (n==0 ||n==1|| n==2 || n==4|| n==11|| n==12)
          {
              system("sed -i '/nslot/'d /vispace/nmu.bi");
          }
          else
          {
            return;
          }
          if (n==1)
          {
            if (!strcmp(optarg,"2") || !strcmp(optarg,"3"))
            {
             //setConfValue("/vispace/nmu.bi","nslot",optarg," ");
             //setConfValue("/vispace/nmu.bi","nslot",optarg," ");
             printf("Slot of NMU set as %s\r\n",optarg);
            }
            else
            {
              printf("Slot of NMU in 1U Chassis must be 2,3\n");
              return;
            }
          }
          else if (n==2)
          {
            if (!strcmp(optarg,"6") || !strcmp(optarg,"7"))
            {
             //setConfValue(SYS_CONF_FILE,"nslot",optarg," ");
             //setConfValue("/vispace/nmu.bi","nslot",optarg," ");
             printf("Slot of NMU set as %s\r\n",optarg);
            }
            else
            {
                printf("Slot of NMU in 2U Chassis must be 6,7\r\n");
                return;
            }
          }
          else if (n==4)
          {
            if (!strcmp(optarg,"15") || !strcmp(optarg,"16"))
            {
             //setConfValue(SYS_CONF_FILE,"nslot",optarg," ");
             //setConfValue("/vispace/nmu.bi","nslot",optarg," ");
             printf("Slot of NMU set as %s\r\n",optarg);
            }
            else
            {
               printf("Slot of NMU in 4U Chassis must be 15,16\r\n");
               return;
            }
          }
          doSystem("sed -i '1a nslot %s' /vispace/nmu.bi",optarg);
          return;
       case 'F':
          if (!strcmp(optarg,"Y") || !strcmp(optarg,"N"))
          setConfValue(SYS_CONF_FILE,"fromflash",optarg," ");
       return;
       case 'T':
          setConfValue("/vispace/nmu.bi","nmu_type",optarg," ");
       return;
       case 'h':
          usage();
          return;
    }
  }
  switch(opt)
  {
    char value[60]="";
    case 'I':
     if (io_debug_flag=='Y')
       setConfValue(SYS_CONF_FILE,"debugio","Y"," ");
     else
       setConfValue(SYS_CONF_FILE,"debugio","N"," ");
     return;
    case 'D':
     if (debug_flag=='Y')
       setConfValue(SYS_CONF_FILE,"debug","Y"," ");
     else
       setConfValue(SYS_CONF_FILE,"debug","N"," ");
     sprintf(value,"%d",max_count);
     setConfValue(SYS_CONF_FILE,"max_count",value," ");
     sprintf(value,"%d",count);
     setConfValue(SYS_CONF_FILE,"count",value," ");
     system("killall -9 ccenter");
     sleep(2);
     system("ccenter&");
     return;
  }
  printf("Invalid param!\n");
}
