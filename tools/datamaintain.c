#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sqlite3.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
//arm-linux-gcc -o datamaintain datamaintain.c -I../include -L=./ -L/home/visint/src/arm/lib -lvispace -lsqlite3 -liconv
static void usage()
{
  printf("Description:\n");
  printf("Database maintain application ver 1.0.1\nLast modified:2014-03-20\n\n");
  printf("Usage:datamaintain [option] \n");
  printf("Option Specification:\n");
  printf("-b: backup\n");
  printf("-r: restore\n");
  //printf("-e: set language to englsih\n");
  //printf("-f: file name\n");
  printf("-d: delete backup\n");
  printf("-s: show all backup file\n");
  printf("-h for help.\n");
}
void filelist( char *path)
{
    DIR    *dir;
    struct    dirent    *ptr;
    struct stat statbuf; 
    char buf[512];
    dir = opendir(path);
    
    while((ptr = readdir(dir)) != NULL)
    {
     snprintf( buf, 512,"%s/%s", path, ptr->d_name); 
     stat(buf, &statbuf);
     //判断是目录还是文件
     if(!S_ISDIR(statbuf.st_mode) )
      printf("f_name: %s\n", ptr->d_name);
     }
    closedir(dir);
    return;

}
void finddir(char *dir, int depth)
{
    DIR *dp;
    struct dirent *entry;
    struct stat statbuf;

    if((dp = opendir(dir)) == NULL) {
        fprintf(stderr,"cannot open directory: %s\n", dir);
        return;
    }
    chdir(dir);
    while((entry = readdir(dp)) != NULL) {
        lstat(entry->d_name,&statbuf);
        if(S_ISDIR(statbuf.st_mode)) {
            /* Found a directory, but ignore . and .. */
            if(strcmp(".",entry->d_name) == 0 || 
                strcmp("..",entry->d_name) == 0)
                continue;
            printf("%*s%s/\n",depth,"",entry->d_name);
            /* Recurse at a new indent level */
            finddir(entry->d_name,depth+4);
        }
        else printf("%*s%s\n",depth,"",entry->d_name);
    }
    chdir("..");
    closedir(dp);
}

void main (int argc, char **argv)
{
  int n,m=0,oc;
  char ec;      /*无效的选项字符*/
  char *b_opt_arg;            /*选项参数字串 */
  int ch;
  char cmd[120]="";
  while((oc = getopt(argc, argv, "b:r:d:sh")) != -1)
  {
     switch(oc)
     {
       case 'b':
            printf("Are you sure to backup (Y/N)?");
            ch=getchar();
            //printf("ch=%c\n",ch);
            if (!strcmp(optarg,"vispace.db"))
            {
                printf("Backup flie name can't be 'vispace.db'\n");
                return;
            }
            if (ch=='Y')
            {
             doSystem("cp /vispace/www/data/vispace.db /vispace/www/data/%s",optarg);
            }
            break;
       case 'd':
            if (!strcmp(optarg,"vispace.db"))
            {
                printf("You can't delete 'vispace.db'\n");
                return;
            }
            printf("Are you sure to delete backup file (Y/N)?");
            ch=getchar();
            if (ch=='Y')
            {
             doSystem("rm /vispace/www/data/%s",optarg);
            }
            break;
       case 'r':
           /*
           if (!strcmp(optarg,"a"))
           {
               printf("Are you sure to restore all (Y/N)?");
           }
           else if (!strcmp(optarg,"u"))
           {
              printf("Are you sure to restore user data (Y/N)?");
           }
            */
           if (!strcmp(optarg,"vispace.db"))
           {
                printf("Restore flie name can't be 'vispace.db'\n");
                return;
           }
           printf("Are you sure to restore all (Y/N)?");
           ch=getchar();
           //printf("ch=%c\n",ch);
           if (ch=='Y')
           {
              if (restoreUserData(optarg))
                 printf("restore fail\n");
              else
                 printf("restore OK\n");
              if (restoreSnmpCoununity(optarg))
                 printf("restore fail\n");
              else
                 printf("restore OK\n");
              if (restoreSnmpDestHost(optarg))
                 printf("restore fail\n");
              else
                 printf("restore OK\n");
           }
        break;
        case 's':
              //finddir("/vispace/www/data/backup",0);
              filelist("/vispace/www/data");
        break;
        case 'h':
             usage();
             return;
     }
}
}
