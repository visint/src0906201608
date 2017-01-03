//#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <typedefs.h>
#include <stdlib.h>
//gcc -c -o ../vislib/visstr.o visstr.c -I../include
/******************************************************************************/
/*
 *	Trim leading white space.
 */

#include <string.h>
#include <stdio.h>

void bytetobits(char byte,char bits[])
{
  int i;
  //printf("byte=%d\n",byte);
  for(i=0;i<8;i++)
  {
    //bits[i]=byte&(0x01<<i);
    bits[i]=(byte>>(7-i))&(0x01);
    //printf("[%d]=%d\n",i,bits[i]);
  }
}

void shorttobits(short value,char bits[])
{
  int i;
  //printf("byte=%d\n",byte);
  for(i=0;i<16;i++)
  {
    //bits[i]=byte&(0x01<<i);
    bits[i]=(value>>(15-i))&(0x01);
    //printf("[%d]=%d\n",i,bits[i]);
  }
}
/*将str1字符串中第一次出现的str2字符串替换成str3*/
void strtoa(char str[],char *delim,char val[][20])
{
 int n=0;
 char *p;
 char *buff;
 buff=str;
 p = strsep(&buff, delim);
 while(p)
 {
   strncpy(val[n], p,20);
   n++;
   p = strsep(&buff, delim);
 }
}
/*将str字符串转换成字节数组*/
int strtoca(char str[],char *delim,char val[],int base)
{
 int n=0;
 char *p;
 char *buff;
 buff=str;
 //printf("str=%s\n",str);
 p = strsep(&buff, delim);
 if (base<2 || base>36)
    base=10;
 while(p)
 {
   //printf("p=%s\n",p);
   /*if (strncmp(p,"0x",2))
       val[n]=(char)strtol(p,NULL, 10 );
   else
       val[n]=(char)strtol(p,NULL, 16 );
  */
   val[n]=(char)strtol(p,NULL, base);
   n++;
   p = strsep(&buff, delim);
 }
 return n;
}
/*将str1字符串中第一次出现的str2字符串替换成str3*/
void replaceFirst(char *str1,char *str2,char *str3)
{
	char str4[strlen(str1)+1];
	char *p;
        if (NULL==str1||NULL==str2||NULL==str3)
          return;
	strcpy(str4,str1);
	if((p=strstr(str1,str2))!=NULL)/*p指向str2在str1中第一次出现的位置*/
	{
		while(str1!=p&&str1!=NULL)/*将str1指针移动到p的位置*/
		{
			str1++;
		}
		str1[0]='\0';/*将str1指针指向的值变成/0,以此来截断str1,舍弃str2及以后的内容，只保留str2以前的内容*/
		strcat(str1,str3);/*在str1后拼接上str3,组成新str1*/
		strcat(str1,strstr(str4,str2)+strlen(str2));/*strstr(str4,str2)是指向str2及以后的内容(包括str2),strstr(str4,str2)+strlen(str2)就是将指针向前移动strlen(str2)位，跳过str2*/
	}
}
/*将str1出现的所有的str2都替换为str3*/
void replace(char *str1,char *str2,char *str3)
{
 if (NULL==str1||NULL==str2||NULL==str3)
      return;
 while(strstr(str1,str2)!=NULL)
 {
      replaceFirst(str1,str2,str3);
 }
}
/*截取src字符串中,从下标为start开始到end-1(end前面)的字符串保存在dest中(下标从0开始)*/
void substring(char *dest,char *src,int start,int end)
{
	char *p=src;
	int i=start;
	if(start>strlen(src))return;
	if(end>strlen(src))
		end=strlen(src);
	while(i<end)
	{	
		dest[i-start]=src[i];
		i++;
	}
	dest[i-start]='\0';
	return;
}
/*返回src中下标为index的字符*/
char charAt(char *src,int index)
{
	char *p=src;
	int i=0;
	if(index<0||index>strlen(src))
		return 0;
	while(i<index)i++;
	return p[i];
}
/*返回str2第一次出现在str1中的位置(下表索引),不存在返回-1*/
int indexOf(char *str1,char *str2)
{
	char *p=str1;
	int i=0;
	p=strstr(str1,str2);
	if(p==NULL)
		return -1;
	else{
		while(str1!=p)
		{
			str1++;
			i++;
		}
	}
	return i;
}
/*返回str1中最后一次出现str2的位置(下标),不存在返回-1*/
int lastIndexOf(char *str1,char *str2)
{
	char *p=str1;
	int i=0,len=strlen(str2);
	p=strstr(str1,str2);
	if(p==NULL)return -1;
	while(p!=NULL)
	{
		for(;str1!=p;str1++)i++;
		p=p+len;
		p=strstr(p,str2);
	}
	return i;
}

//删除str左边第一个非空白字符前面的空白字符(空格符和横向制表符)
/*
void ltrim(char *str)
{
	int i=0,j,len=strlen(str);
	while(str[i]!='/0')
	{
		if(str[i]!=32&&str[i]!=9)break;//32:空格,9:横向制表符
		i++;
	}
	if(i!=0)
	for(j=0;j<=len-i;j++)
	{	
		str[j]=str[j+i];//将后面的字符顺势前移,补充删掉的空白位置
	}
}
//删除str最后一个非空白字符后面的所有空白字符(空格符和横向制表符)
void rtrim(char *str)
{
	char *p=str;
	int i=strlen(str)-1;
	while(i>=0)
	{
		if(p[i]!=32&&p[i]!=9)break;
		i--;
	}
	str[++i]='/0';
}
//删除str两端的空白字符
void trim(char *str)
{
	ltrim(str);
	rtrim(str);
}
 */
char *strTolower(char *s)  
{  
    char *str;  
    str = s;  //记录首地址位置,没有必要判断空值!   
    while(*str != '\0')  
    {  
        if(*str >= 'A' && *str <= 'Z'){  //大写字母则进行转换!   
            *str += 'a'-'A';  
        }  
        str++;  
    }  
    return s;  
}  

char *trim(char *str)
{
   while (isspace((int)*str)) {
	  str++;
   }
   return str;
}

/*
*去除字符串右端空格
*/
char *strtrimr(char *pstr)
{
int i;
i = strlen(pstr) - 1;
while (isspace(pstr[i]) && (i >= 0))
pstr[i--] = '\0';
return pstr;
}
/*
*去除字符串左端空格
*/
char *strtriml(char *pstr)
{
int i = 0,j;
j = strlen(pstr) - 1;
while (isspace(pstr[i]) && (i <= j))
i++;
if (0<i)
strcpy(pstr, &pstr[i]);
return pstr;
}
/*
*去除字符串两端空格
*/
char *strtrim(char *pstr)
{
char *p;
p = strtrimr(pstr);
return strtriml(p);
}

/*
*判断当前字符串是否是以另一个字符串"结尾"的,根据判断结果返回1 或0
*/
char strendwith(const char *src,const char *endwith)
{
char *p;

p=strstr(src,endwith);
if (p)
{
  if ((p-src)==(strlen(src)-strlen(endwith)))
    return 1;
  else
    return 0;
}
return 0;
}


int keyval(char *buf, char *sep,char **key, char **val)
{
  char *ptr = strstr(buf, sep); 
  if (ptr==NULL)  
  {  
    return -1;  
  }  
  *ptr++ = '\0';
  *key = trim(buf);
  *val = trim(ptr);
  return 0;
}
/*
char optsep(char *src,char *delim,uchar_t count,...)
{
   char *s = src;
   char *token;
   char n=0;
   token = strsep(&s, delim);
   while (token != NULL && n<max_count)
   {
     strcpy(opt[n],token);
     token = strsep(&s, delim);
     n++;
   }
   return n;
}
*/
char optsep(char *src,char *delim,char **opt,char max_count)
{

}

char getKeyValue(char *src,char *sep,char *delim,char *key,
                  char key_size,char *value,char value_size,char max_count)
{
   char *s = src,*k,*val;
   char *token;
   char n=0;
   token = strsep(&s, sep);
   while (token != NULL && n<max_count)
   {
    //k=key+n*key_size;
    //val=value+n*value_size;
    keyval(token,delim,&k,&val);
    strcpy(key+n*key_size,k);
    strcpy(value+n*value_size,val);
    //printf("token:%s key:%s val:%s\n",token,k,val);
    token = strsep(&s, sep);
    n++;
   }
   return n;
}

/*
size_t get_wchar_size(const char *str)
{   
  size_t len = strlen(str);   
  size_t size=0;   
  size_t i;   
  for(i=0; i < len; i++)   
  {      
   if( str[size] >= 0 && str[size] <= 127 ) //不是全角字符      
    size+=sizeof(wchar_t);     
   else //是全角字符，是中文      
   {        
    size+=sizeof(wchar_t); 
    i+=2;      
   }   
  }  
 return size; 
}
*/

void printChar(char *s,int count)
{
  int i;
  for (i=0;i<count;i++)
   printf("%s",s);
}
void printData(char *data_name,char **title,char **row,int nrow,int ncol)
{
  int i,n=0;
  if (nrow>1)
    printf("%s 共 %d 条\n",data_name,nrow);
  else
  	printf("%s\n",data_name);
  for (i=0;i<nrow*ncol;i++)
  {
    printf("%s:%s\n",title[n],row[i]);
    if (i%ncol!=ncol-1)
    {
     n++;
    }
    else
    {
     n=0;
     printf("\n");
    }
  }
}
/*********************************************************************************
* 函数名称： printArray
* 功能描述： 按列的方式打印字符串数组。 
* 访问的表： 无
* 修改的表： 无
* 输入参数： char *data_name,char *title：数据域标题,int title_size:标题字节长度,char *strarr:二维字符串数组首地址,int field_size:数据域字节长度,int nrow :数据域行数,int ncol:数据域列数
* 输出参数： 无
* 返 回 值： 无
* 其它说明： 无
* 修改日期        版本号     修改人       修改内容
* -----------------------------------------------
* 2012/10/15        V1.0      李文峰        创建
*例子:
*char title[][16]={"槽号","单元盘名称","SN"};
*char units[][32]={"1","单元盘名称","SN-A123456666,",
                   "2","单元盘名称","SN-B123456666,",
                   "3","单元盘名称","SN-C123456666,"};
* printArray("字符串数组表格打印",(char*)title,16,(char*)units,32,3,3);
************************************************************************************/
void printArray(char *data_name,char *title,int title_size,char *row,int col_size,int nrow,int ncol)
{
  int i,n=0;
  char *p=row;
  if (nrow>1)
      printf("%s 共 %d 条\n",data_name,nrow);
  else
      printf("%s\n",data_name);
  printChar("-",30);
  printf("\n");
  for (i=0;i<nrow*ncol;i++)
  {
    printf("%s:%s\n",title+(i%ncol)*title_size,p+col_size*i);
  }
  printChar("=",30);
  printf("\n");
}
/*********************************************************************************
* 函数名称： printArrTable
* 功能描述： 按表格的方式打印字符串数组。 
* 访问的表： 无
* 修改的表： 无
* 输入参数： char *data_name,char *title：数据域标题,int title_size:标题字节长度,char *strarr:二维字符串数组首地址,int field_size:数据域字节长度,int nrow :数据域行数,int ncol:数据域列数
* 输出参数： 无
* 返 回 值： 无
* 其它说明： 无
* 修改日期        版本号     修改人       修改内容
* -----------------------------------------------
* 2012/10/15        V1.0      李文峰        创建
*例子:
*char title[][16]={"槽号","单元盘名称","SN"};
*char units[][32]={"1","单元盘名称","SN-A123456666,",
                   "2","单元盘名称","SN-B123456666,",
                   "3","单元盘名称","SN-C123456666,"};
* printArrTable("字符串数组表格打印",(char*)title,16,(char*)units,32,5,4,cols_width);
************************************************************************************/
void printArrTable(char *data_name,char *title,int title_size,char *row,int col_size,int nrow,int ncol,uchar_t cols_width[])
{
  int i,n=0,len=0,col_wd=0;
  char *p=row;
  char fmt[10]="";
  size_t wd;

  if (nrow>0)
      printf("%s 共 %d 条\n",data_name,nrow);
  else
  {
     printf("%s\n",data_name);
     return;
  }
  for (i=0;i<ncol;i++)
  {
   len=len+cols_width[i];
   sprintf(fmt,"%s%ds","%-",cols_width[i]);
   printf(fmt,title+(i%ncol)*title_size);
  }
  len++;
  printf("\n");
  printChar("-",len);
  printf("\n");

  for (i=0;i<nrow*ncol;i++)
  {
    if (i%ncol==ncol-1)
      {
       printf("%s\n",p+col_size*i);
       n=0;
      }
      else
      {
       printf("%s",p+col_size*i);
       //wd=cols_width[n]-get_wchar_size(p+col_size*i)/3;
         wd=30;
       printChar(" ",wd);
       col_wd=strlen(p+col_size*i);
       if (col_wd<=cols_width[n])
          col_wd=cols_width[n]-col_wd;
       n++;
      }
  }
  printChar("-",len);
  printf("\n");
}
void printTable(char *table_name,char **titel,char **row,int nrow,int ncol,uchar_t cols_width[])
 {

  int i,n=0,len=0;
  char fmt[10]="";

  for (i=0;i<ncol;i++)
  {
   len=len+cols_width[i];
  }
  len++;
    printf("%s 共 %d 行 %d 列\n",table_name,nrow,ncol);
    for (i=0;i<ncol;i++)
    {
     sprintf(fmt,"%s%ds","%-",cols_width[i]);
     printf(fmt,titel[i]);
    }
    printf("\n");
    printChar("-",len);
    printf("\n");
    for (i=0;i<nrow*ncol;i++)
    {
      if (i%ncol==ncol-1)
      {
       printf("%s\n",row[i]);
       n=0;
      }
      else
      {
       sprintf(fmt,"%s%ds","%-",cols_width[n]);
       printf(fmt,row[i]);
       n++;
      }
     }
}

int _main()
{
  char n;
  char key[10][10]={"A","B"};
  char value[10][20];
  char kv[]="1=link;2=down";
  /*getKeyValue(kv,";","=",(char*)key,10,(char*)value,20,5);
  for (n=0;n<10;n++)
    printf("key:%s val:%s\n",key[n],value[n]);
  */
  char buf[]="snmp-1.2.3.tar.gz";
  printf("%d\n",strendwith(buf,".tar"));
}

