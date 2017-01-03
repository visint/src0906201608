void main()
{
  char slots[60]="";
  sscanf("2:1,2,3:a", "%*[^:](%[^:]s", slots);
  sscanf("(115.239.210.27)", "[^(](%[^)]s", slots);
  printf("slots:%s\n",slots);

char abd[80]="2:1,2,3:";
char buf1[10]="",buf2[10]="",buf3[10]="";
sscanf(abd, "%[^:]:%[^:]:%[^:]", buf1,NULL,NULL); //通过ssanf获得文件名及后缀名
printf("buf1=%s buf2=%s buf3=%s",buf1, buf2, buf3);
}
