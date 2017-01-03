#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <public.h>
//arm-linux-gcc -o setup  install.c -I./../include -L/home/visint/src/arm/lib -lvispace -liconv -lsqlite3
static void usage()
{
  printf("Description:\n");
  printf("Install/uninstall application ver 1.0.8\nLast modified:2015-11-02\n\n");
  printf("Usage:install [option] \n");
  printf("Option Specification:\n");
  printf("-i: install\n");
  printf("-l: set language to chinese(c) or englsih(e)\n");
  //printf("-e: set language to englsih\n");
  //printf("-f: file name\n");
  printf("-u: uninstall\n");
  printf("-U: upgrade file\n");
  printf("-p: file path to save\n");
  printf("-f: file name to save\n");
  printf("-C: upgrade ccenter\n");
  printf("-W: upgrade webs\n");
  printf("-S: upgrade snmpd\n");
  printf("-v: show version\n");
  printf("-t host ip.\n");
  printf("-H set defaul host ip.\n");
  printf("-h for help.\n");
}

void main (int argc, char **argv)
{
  int n,m=0,oc;
  char ec;      /*无效的选项字符*/
  char *b_opt_arg;            /*选项参数字串 */
  int ch,opt=0;
  char cmd[120]="",path[60]="/",file[30]="nmu.tar.gz",hostIp[30]="192.168.1.238";
  char lang=0;
  //char ack='Y';
  getConfValue(SYS_CONF_FILE,"host",hostIp," ");
  while((oc = getopt(argc, argv, "a:Ci:t:u:Uf:H:p:hSW")) != -1)
  {
        switch(oc)
        {
          case 'l':
               if (!strcmp(optarg,"c"))
               {
                 lang='c';
                 printf("Are you sure to set language to chinese (Y/N)?");
               }
               else if (!strcmp(optarg,"e"))
               {
                 lang='e';
                 printf("Are you sure to set language to englsih (Y/N)?");
               }
               else
               {
                 printf("Usage:install -l [c|e]\nc--chinese e--englsih");
                 exit(0);
               }
               ch=getchar();
               printf("ch=%c\n",ch);
               if (ch=='Y')
               {
                system("sed -i '/webs/'d Svispace");//-----删除Svispace文件所有包含webs的行。            
                //system("sed -i '/$2 %s/ Svispace");
                //system("sed -i '5,$d' Svispace");
                sprintf(cmd,"sed -i '2a webs%cn' Svispace",lang);
                printf("%s\n",cmd);
                system(cmd);
                system("killall watchdogd && killall webscn && killall websen && cd /vispace/webs");
                sprintf(cmd,"./webs%cn",lang);
                system(cmd);
                
                system("sed -i '/watchdogd/'d Svispace");
                sprintf(cmd,"watchdogd %cn",lang);
                system(cmd);
                sprintf(cmd,"sed -i '3a watchdogd %cn' Svispace",lang);
                system(cmd);
                sprintf(cmd,"watchdogd %cn",lang);
                system(cmd);
                printf("Set language OK\n");
               }
          break;
          /*
          case 'e':
               printf("Are you sure to set language to englsih(Y/N)?");
               ch=getchar();
               printf("ch=%c\n",ch);
               if (ch=='Y')
               {
                sprintf(cmd,"sed -i '/%s/'d Svispace",optarg);//-----删除Svispace文件所有包含文本的行。            
                system(cmd);
                //system("sed -i '5,$d' Svispace");
                sprintf(cmd,"sed -i '$a %s' Svispace",optarg);
                printf("%s\n",cmd);
                system(cmd);
                //system(optarg);
                printf("install OK!please reoot\n");
               }
          break;
          */
          case 'i':
               //printf("Are you sure to install %s(Y/N)?",optarg);
               //ch=getchar();
               ch='Y';
               if (ch=='Y')
               {
                system("killall -9 watchdogd");
                sprintf(cmd,"sed -i '/%s/'d /etc/init.d/Svispace",optarg);//-----删除Svispace文件所有包含文本的行。            
                system(cmd);
                sprintf(cmd,"killall %s",optarg);
                system(cmd);
                if (strncmp(optarg,"ccenter",7))
                  sprintf(cmd,"sed -i '$a %s&' /etc/init.d/Svispace",optarg);//Svispace文件最后一行添加。            
                else
                  sprintf(cmd,"sed -i '2a %s&' /etc/init.d/Svispace",optarg);
                if (!strcmp(optarg,"olputil"))
                {
                   
                   setConfValue(SYS_CONF_FILE,"olputil","Y"," ");
                }
                else if (!strcmp(optarg,"hb_enable"))
                {
                   setConfValue(SYS_CONF_FILE,"hb_enable","N"," ");
                }
                printf("cmd=%s\n",cmd);
                system(cmd);
                sprintf(cmd,"%s&",optarg);
                system(cmd);
                sleep(3);
                system("watchdogd &");
                printf("Install %s OK!\n",optarg);
               }
          break;
          case 'u':
               //printf("Are you sure to uninstall %s(Y/N)?",optarg);
               //ch=getchar();
               //printf("ch=%c\n",ch);
               ch='Y';
               if (ch=='Y')
               {
                system("killall -9 watchdogd");
                sprintf(cmd,"sed -i '/%s/'d /etc/init.d/Svispace",optarg);//-----删除Svispace文件所有包含文本的行。            
                system(cmd);
                sprintf(cmd,"killall %s",optarg);
                system(cmd);
                if (!strcmp(optarg,"olputil"))
                {
                   setConfValue(SYS_CONF_FILE,"olputil","N"," ");
                }
                else if (!strcmp(optarg,"hb_enable"))
                {
                   setConfValue(SYS_CONF_FILE,"hb_enable","N"," ");
                }
                sleep(3);
                system("watchdogd &");
                printf("uninstall %s OK!\n",optarg);
               }
          break;
          case 'C':
          case 'S':
          case 'W':
                  if (oc=='W')
                  {
                    strcpy(file,"webs");
                    strcpy(path,"/vispace/webs");
                  }
                  else
                  {
                    if (oc=='C')
                       strcpy(file,"ccenter");
                    else if (oc=='S')
                       strcpy(file,"snmpd");
                    strcpy(path,"/bin");
                  }
                  printf("Are you sure to upgrade file:%s from %s (Y/N)?",file,hostIp);
                  ch=getchar();
               //printf("ch=%c\n",ch);
                  if (ch!='Y')
                    return;
                 doSystem("killall -9 %s",file);
                 doSystem("cd %s",path);
                 //printf("path=%s file=%s\n",path,file);
                 doSystem("cd %s && tftp %s -g -r %s",path,hostIp,file);
                 sleep(1);
                 doSystem("cd %s && chmod +x %s",path,file);
                 if (oc=='C' ||oc=='W')
                 {  
                   doSystem("cd %s && chmod +x %s&",path,file);
                   doSystem("cd %s && %s&",path,file);
                 }
                 else
                    system("snmpd -c /vispace/snmp/snmpd.conf");
                 printf("Upgrade file:%s Ok!\n",file);
          break;
          case 'U':
               opt='U';
               break;
          case 'H':
               setConfValue(SYS_CONF_FILE,"host",optarg," ");
               break;
          case 'p':
                 strcpy(path,optarg);
                break;
          case 'f':
                 strcpy(file,optarg);
                break;
          case 't':
                 strcpy(hostIp,optarg);
                break;
          case 'h':
                usage();
                return;
          case '?':
                ec = (char)optopt;
                printf("无效的选项字符 \' %c \'!\n", ec);
                break;

          case ':':
                printf("缺少选项参数！\n");
                usage();
                return;
        }
  }
  if (opt=='U')
  {
    printf("Are you sure to upgrade file:%s from %s (Y/N)?",file,hostIp);
    ch=getchar();
               //printf("ch=%c\n",ch);
    if (ch!='Y')
       return;
    doSystem("cd %s",path);
    printf("Loading down file:%s....\n",file);
    doSystem("killall -9 watchdogd");
    doSystem("killall -9 %s",file);
    doSystem("tftp %s -g -r %s",hostIp,file);
    printf("File:%s loaded down OK!\n",file);
    if (strstr(file,".tar.gz"))
    {
      printf("unpacking file:%s....\n",file);
      doSystem("tar -zxvf %s",file);
      printf("unpacking file:%s OK\n",file);
      printf("Do you want to delete %s/%s (Y/N)?",path,file);
      ch=getchar();
      printf("ch=%c\n",ch);
      if (ch=='Y')
       doSystem("rm %s/%s",path,file);
    }
    doSystem("watchdogd&");
  }
}
