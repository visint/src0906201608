#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <time.h>
#include <sys/time.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
//#include <sysinfo.h>

#include <unit.h>
#include <visipc.h>
#include <chassis.h>
#include <public.h>
//gcc -o testfile testfile.c -I./../include
void read_data_from_file()
{
  FILE *fp=fopen("unit.data","rb");
  if (fp==NULL)
     return;
  unit_base_info_t bi;
  char tmp_buf[512]={0};
  int n;
  for (n=0;n<2;n++)
  {
   fread(&bi,sizeof(unit_base_info_t),1,fp);
   printf("Unit #%d %s\r\n",n+1,bi.model);
   fread(tmp_buf,60,1,fp);
  }
  
  fclose(fp);
}
void write_data_from_file()
{
  FILE *fp=fopen("unit.data","wb");
  if (fp==NULL)
     return;
  unit_base_info_t bi;
  char buf[60]={1,2,3,4,5,6,7,8,9};
  strcpy(bi.model,"OTU-123A");
  strcpy(bi.sn,"SN111-123A");
  strcpy(bi.fwver,"01.10.23");
  strcpy(bi.hwver,"01.01.01");
  strcpy(bi.creation,"OTU-123A");
  fwrite(&bi,sizeof(unit_base_info_t),1,fp);
  
  fwrite(buf,60,1,fp);
  strcpy(bi.model,"OTU-123");
  strcpy(bi.sn,"SN111-123");
  strcpy(bi.fwver,"01.10.13");
  strcpy(bi.hwver,"01.01.01");
  strcpy(bi.creation,"OTU-123");
  fwrite(&bi,sizeof(unit_base_info_t),1,fp);
  
  fwrite(buf,60,1,fp);
  fclose(fp);
}
void main()
{
  write_data_from_file();
  read_data_from_file();
}
