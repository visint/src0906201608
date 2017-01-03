#include <string.h>
#include <sys/sysinfo.h>
#include <time.h>
#include "../wsIntrn.h"
#include "../../include/unit.h"
#include <visipc.h>
#include <sqlite3.h>
#include <dbutil.h>
#include <log.h>
#include <unit.h>
#include <otu.h>
#include <olp.h>
#include <sysinfo.h>
#include <chassis.h>
#include <passive.h>
#include <public.h>

static int parseInfoset(webs_t wp,char *msg,char *infoset_ptr[],char data_type,char *infoset_index,char item_index[])
{
  char *p,n,m,index=0;
  short item_id,item_offset,item_count;
  int nBytes=0;
  p = strtok(msg, "|");
  while(p != NULL){
    sscanf(p,"%hd %hd %hd",&item_id,&item_offset,&item_count);
    *infoset_index=0;
    for (n=0;n<4;n++)
    {
      if (NULL!=infoset_ptr[n])
      {
       for (m=0;m<item_count;m++)
       {
         printf("items[%d][%d]={id:%d,val:",*infoset_index,item_index[n],item_id+m);
         nBytes+=websWrite(wp,T("items[%d][%d]={id:%d,val:"),*infoset_index,item_index[n],item_id+m);
         if (data_type!=2)
           printf("%d};\n",*(infoset_ptr[n]+item_offset+m));
         else
         {
           //value=(short)(*(infoset_ptr[n]+item_offset+m*2));
           printf("%d};\n",(short)(*(infoset_ptr[n]+item_offset+m*2)));
           nBytes+=websWrite(wp,T("%d};\n"),(short)(*(infoset_ptr[n]+item_offset+m*2)));
         }
         item_index[n]++;
       }
       *infoset_index+=1;
      }
      else
      {
       //printf("infoset_ptr[%d] is null\n",n);
       break;
      }
    }
    p = strtok(NULL,"|"); 
  }
 return nBytes;
}
/************************************************************
*功能:信息集查询,根据参数查询信息集并输出信息集的Javascript变量或对象脚本
*参数格式:信息集ID [信息集索引1 信息集索引2...] S|B=信息项ID 信息项偏移量 信息项个数
*参数说明:
*可以有多个信息集ID，不同信息集ID之间用$隔开;相同信息集ID可以指定多个索引或信息项,
不同信息项之间用|隔开;
*B:信息项为字节型 S:信息项为双字节型,不同类型信息项用&隔开
*例子1:
*查询参数:65 [1 2] S=1 0 5|12 14 4|16 20 4&B=8 12 7$66 B=8 12 7&S=1 0 5
*信息集65 1:infoset[36]={208,255,218,255,228,255,203,255,206,255,88,0,1,1,1,1,1,2,2}
*信息集65 2:infoset[36]={208,255,218,255,228,255,203,255,206,255,88,0,1,1,1,1,1,2,2}
信息集查询结果:
items= new Array();
items[0]= new Array();
items[1]= new Array();

items[0][0]={id:1,val:-48};
items[0][1]={id:2,val:-38};
items[0][2]={id:3,val:-28};
items[0][3]={id:4,val:-53};
items[0][4]={id:5,val:-50};
items[1][2]={id:1,val:-48};
items[1][3]={id:2,val:-38};
items[1][4]={id:3,val:-28};
items[1][5]={id:4,val:-53};
items[1][6]={id:5,val:-50};
items[0][5]={id:8,val:1};
items[0][6]={id:9,val:1};
items[0][7]={id:10,val:1};
items[0][8]={id:11,val:1};
items[0][9]={id:12,val:1};
items[0][10]={id:13,val:2};
items[0][11]={id:14,val:2};
items[1][1]={id:8,val:1};
items[1][2]={id:9,val:1};
items[1][3]={id:10,val:1};
items[1][4]={id:11,val:1};
items[1][5]={id:12,val:1};
items[1][6]={id:13,val:2};
items[1][7]={id:14,val:2};
infoset[0]=createInfoset(65,1,items[0]);
infoset[1]=createInfoset(65,2,items[1]);

items= new Array();
items[0]= new Array();
items[0][0]={id:8,val:1};
items[0][1]={id:9,val:1};
items[0][2]={id:10,val:1};
items[0][3]={id:11,val:1};
items[0][4]={id:12,val:1};
items[0][5]={id:13,val:2};
items[0][6]={id:14,val:2};
infoset[2]=createInfoset(66,1,items[0]);
*************************************************************/
int aspInfoset(int eid, webs_t wp, int argc, char_t **argv)
{
  int nBytes=0;
  char_t *p;
  uchar_t slot,uclass=0,utype=0,count=0;
  char *infoset_ptr[4];//,infoset[36]={208,255,218,255,228,255,203,255,206,255,88,0,1,1,1,1,1,2,2};
  char *p1,*p2 = NULL;
  char *items_head;
  int in=0,n;
  short infoset_id=0,index[4]={0};

  char item_index[4]={0};
  char infoset_index=0;
  char buf[120]="";
  char *ptr[16];
  char data_type,*buf_ptr;

  
  if (ejArgs(argc, argv, T("%s"), &buf_ptr) < 1) {
    //websError(wp, 400, T("Insufficient args\n"));
    nBytes=websWrite(wp,"\nvar err=1;\n");
    return nBytes;
  }
  if (buf_ptr==NULL)
  {
    //websError(wp, 400, T("Insufficient args\n"));
    //return -1;
    nBytes=websWrite(wp,"\nvar err=2;\n");
    return nBytes;
  }
  slot=(uchar_t)getSelectedUnit();
  getUnitClassType(0,slot,&uclass,&utype);
  nBytes=websWrite(wp,T("\nvar err=0,slot=%d,uclass=%d,utype=%d;\n"),slot,uclass,utype);
  if (uclass<1 || utype<1 ||slot<1 ||slot>16)
  {
    return nBytes;
  }
  while((p[in] = strtok_r(buf_ptr, "$", &p1))!=NULL)   
  {  
    buf_ptr=p[in];
    strcpy(buf,buf_ptr);
    sscanf(buf_ptr,"%hd",&infoset_id);
    //printf("infoset_id=%d\n",infoset_id);
    infoset_index=0;
    memset(item_index,0,4);
  
    items_head=strstr(p[in],"[");
    if (items_head!=NULL)
    {
       sscanf(items_head,"[%hd %hd %hd %hd]",&index[0],&index[1],&index[2],&index[3]);
      //parseIndex(items_head+1);
    }
    else
    {
      index[0]=1;
      index[1]=0;
    }
    printf("\nitems= new Array();\n");
    nBytes+=websWrite(wp,"\nitems= new Array();\n");
    for (n=0;n<4;n++)
    {
       if (index[n]>0)
       {
          //infoset_ptr[n]=infoset;
          infoset_ptr[n]=getInfoSetValues(0,slot,infoset_id,index[n]);
          printf("\nitems[%d]= new Array();\n",index[n]-1);
          nBytes+=websWrite(wp,T("\nitems[%d]= new Array();\n"),index[n]-1);
       }
       else
          infoset_ptr[n]=NULL;
    }
    //printf("index:%d %d %d %d ",index[0],index[1],index[2],index[3]);
    while((p[in]=strtok_r(buf_ptr, "&", &p2))!=NULL)   
    {  
        if (strchr(p[in],'S'))
            data_type=2;
        else
            data_type=1;
        items_head=strstr(p[in],"=");
        if (items_head!=NULL)
             nBytes+=parseInfoset(wp,items_head+1,infoset_ptr,data_type,item_index,
&infoset_index);
        //printf(" {%s}",p[in]);
        in++;
        buf_ptr=NULL;
    }
    for (n=0;n<4;n++)
    {
      if (index[n]>0)
      {
        printf("infoset[%d]=createInfoset(%d,%d,items[%d]);\n",count,infoset_id,index[n],index[n]-1);
        nBytes+=parseInfoset(wp,T("infoset[%d]=createInfoset(%d,%d,items[%d]);\n"),count,infoset_id,index[n],index[n]-1);
        count++;
      }
    }
    //   count++;
    printf("\n");
    
    buf_ptr=NULL;  
}
}
void formSetInfoset(webs_t wp, char_t *path, char_t *query)
{
  char_t *pslot,*pindex,*ppage,*pflag,*puc,*put,*pinfosetId,*pitemId,*pvalue;
  //int selected_slot;
  //short values[16];
  infoItem_t items[16];
  uchar_t n,item_count=0,slot,infoset_id,index=0;
  char buf[20];
  pslot = websGetVar(wp, T("slot"), T("0"));
  pindex= websGetVar(wp, T("index"), T("0"));
  pinfosetId= websGetVar(wp, T("infosetId"), T("0"));
  //pitemId= websGetVar(wp, T("item"), T("0"));
  //pvalue= websGetVar(wp, T("val"), T("0"));
  ppage= websGetVar(wp, T("page"), T("0"));
  
  if (NULL==pslot ||NULL==pinfosetId)
  {
     websRedirect(wp,ppage);
     return;
  }
  //printf("formSetInfoset ??? pslot=%s page=%s\n",pslot,ppage);
  slot=(uchar_t)atoi(pslot);
  infoset_id=(uchar_t)atoi(pinfosetId);
  if (NULL!=pindex)
    index=(uchar_t)atoi(pindex);
  //printf("formSetInfoset...pslot=%s infoset_id=%d page=%s\n",pslot,infoset_id,ppage);
  if (slot<1 || slot>16 || infoset_id<1)
  {
     websRedirect(wp,"unk_unit.asp");
     return;
  }
  //printf("formSetInfoset...pslot=%s page=%s\n",pslot,ppage);
  memset(&items,0,sizeof(infoItem_t)*16);
  for (n=0;n<36;n++)
  {
     sprintf(buf,"id%d",n+1);
     pvalue= websGetVar(wp, T(buf), NULL);
     if (NULL!=pvalue)
     {
       if (item_count>16)
          break;
       items[item_count].item_id=n+1;
       items[item_count].item_value=(short)atoi(pvalue);
       
       //printf("formSetInfoset...pslot=%s id=%d pvalue=%s val=%d \n",pslot,n+1,pvalue,items[item_count].item_value);
       item_count++;
     }
  }
  
  //printf("formSetInfoset...@@slot=%d item_count=%d infoset_id=%d,index=%d \n",slot,item_count,infoset_id,index);
  /*
   for (n=0;n<item_count;n++)
   printf("#%d Item_id=%d value=%d\n",n,items[n].item_id,items[n].item_value);
  */
  if (item_count>0)
  {
    setShortInfoItemsValues(0,slot,infoset_id,index,items,item_count,300,1,0,0);
    /*
    uchar_t uclass;
    setShortInfoItemsValues(0,slot,infoset_id,index,items,item_count,300,1,0,0);
    uclass=getUnitClass(0,slot);
    switch (uclass)
    {
      case UC_OTU:
        insertOperateLog(LOG_UNIT_OTU_CONF,0,0,WebMode,mod,slot,index);
      break;
      case UC_OEO:
        insertOperateLog(LOG_UNIT_OEO_CONF,0,0,WebMode,mod,slot,index);
      break;
      case UC_OLP:
        insertOperateLog(LOG_UNIT_OLP_CONF,0,0,WebMode,mod,slot,index);
      break;
      case UC_EDFA:
        insertOperateLog(LOG_UNIT_EDFA_CONF,0,0,WebMode,mod,slot,index);
      break;
    }
    */
    insertUnitSettingLog(0,0,WebMode,slot,0);
  }
  /*
  setShortInfoItemValue(0,(uchar_t)atoi(pslot),(uchar_t)atoi(pinfosetId),(uchar_t)atoi(pindex),(uchar_t)atoi(pitemId),value,300);

  printf("formSetInfoset slot=%s pInfosetId=%s pindex=%s pitemId=%s pvalue=%s  ppage=%s\n",pslot,pinfosetId,pindex,pitemId,pvalue,ppage);
  value=atoi(pvalue);
  setShortInfoItemValue(0,(uchar_t)atoi(pslot),(uchar_t)atoi(pinfosetId),(uchar_t)atoi(pindex),(uchar_t)atoi(pitemId),value,300);
 */
  //printf("formSetInfoset slot=%s pInfosetId=%s pindex=%s ppage=%s\n",pslot,pinfosetId,pindex,ppage);
  sleep(2);
  sprintf(buf,"%s.asp",ppage);
  //printf("formSetInfoset slot=%s pInfosetId=%s pindex=%s ppage=%s buf=%s\n",pslot,pinfosetId,pindex,ppage,buf);
  websRedirect(wp,buf);
}
