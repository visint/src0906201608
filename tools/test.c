//arm-linux-gcc -o test test.c
int main(int argc, char **argv)
{
  char hb_group[]="1:1,2,3,4,5,6,7,8,9,10,11,12,13,14,15:192.168.1.123";
  int linkage;
  char ip[24],slots[60];
  sscanf(hb_group,"%d:%[^:]:%[^:]",&linkage,slots,ip);
  printf("linkage:%d slots=%s ip=%s\n",linkage,slots,ip);
   printf("slots=%s ip=%s\n",slots,ip);
}
