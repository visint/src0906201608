/******************************** Description *********************************
* �ļ����ƣ� visshm.c.c
* ����������
*1����Ϣ�������ڴ�Ķ�д����ӡ�ɾ������
*2��0-16�ֽڴ��0��16��Ԫ�̵Ĵ��ࡢ17��32�ֽڴ��0��16��Ԫ�̵�С��
*4��32��63�ֽڴ������
*5��64��95�ֽڴ����ݱ���
*6��96��111�ֽڴ�ŵ�Ԫ����Ϣ�����ڴ��еĽڵ��λ��
*7��120��ʼ�����Ϣ��
* �޸�����        �汾��     �޸���       �޸�����
* -----------------------------------------------
* 2012/10/18        V1.0      ���ķ�        ����
*
* ��������:
*Ӧ���������:gcc -o ish  ../vislib/visstr.o visshm.c -I../include
*SO����:gcc -o ish  ../vislib/visstr.o visshm.c -I../include
gcc -shared -fpic -o libmsgq.so vismsg.c  -I../include
*Copyright (c) Visint Inc., 2002-2012. All Rights Reserved.
   1     2	3	4	5 ...	16
*  -------------------------------------------------------------------------------
  |   |
  |   |
  |___|
  |___|
*******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <error.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <typedefs.h>

#include <public.h>
#include <unit.h>
#include <infoset.h>
#include <visstr.h>
//�洢��
#define  INFO_SET_MAX_COUNT 221
#define  INFO_SET_MAX_COUNT_ERVERY_UNIT 13
#define  STATUS_SHM_KEY 76
#define  BASE_INFO_SHM_KEY 78
#define  TEMP_SHM_KEY 80
#define  INFO_HEAD_SIZE 200

#define  CMD_MESSAGE_OFFSET 36  //�����
#define  DATA_MESSAGE_OFFSET 72 //���ݱ���

//#define  INFO_SET_HEAD_OFFSET 96 //��ŵ�Ԫ��Infoset�Ľڵ㿪ʼλ��
//#define  INFO_SET_COUNT_OFFSET 112  //��ŵ�Ԫ��Infoset�Ľڵ���

#define  INFO_UNIT_PROPERTY_OFFSET 108 //��ŵ�Ԫ�����ԣ��粨������ÿ��Ԫ�̵�����Ϊ4���ֽ�
#define  INFO_UNIT_PROPERTY_SIZE 128

#define  INFO_SET_HEAD_OFFSET 236 //��ŵ�Ԫ��Infoset�Ľڵ����Ϣ����ڵ���
#define  INFO_SET_OFFSET 268  //��ŵ�Ԫ��Infoset�Ľڵ�

int find_infoset(char slot,char infoset_id,char index);
uchar_t *getUnitStatusMap(uchar_t chassis);
uchar_t getUnitClass(uchar_t chassis,uchar_t slot);

//status info of unit
static int   usi_shm_id = -1;
static char *usi_shm_add=NULL;

//basic info of unit
static int   ubi_shm_id = -1;
static char *ubi_shm_add=NULL;


static boolean_t initShmget(uchar_t chassis)
{
    char n;
    usi_shm_id = shmget(STATUS_SHM_KEY+chassis,INFO_HEAD_SIZE+sizeof(status_infoset_t)*INFO_SET_MAX_COUNT, IPC_CREAT | 0666);
    if(usi_shm_id==-1)
    {
        perror("shmget error");
        return FALSE;
    }
    usi_shm_add=(char*)shmat(usi_shm_id,NULL,0);
    if(usi_shm_add == (void *) -1)
    {
        perror("shmat");
        printf("map the shm error: %s.\n", strerror(errno));
        return FALSE;
    }
    return TRUE;
}

static boolean_t initUBIShmget(uchar_t chassis)
{
    char n;
    ubi_shm_id = shmget(BASE_INFO_SHM_KEY+chassis,(sizeof(unit_base_info_t)+INFO_UNIT_PROPERTY_SIZE)*(MAX_UNIT_COUNT+1) , IPC_CREAT | 0666);
    if(ubi_shm_id==-1)
    {
        perror("shmget error");
        return FALSE;
    }
    ubi_shm_add=(char*)shmat(ubi_shm_id,NULL,0);
    if(ubi_shm_add == (void *) -1)
    {
        perror("shmat");
        printf("map the shm error: %s.\n", strerror(errno));
        return FALSE;
    }
    return TRUE;
}

/*
 *����ָ����λ��ȡ������Ϣ�����ڴ��ָ��
 *���������
 *chassis�������
 *slot����λ��
 *�����ų��˿��У���=��ǰ�������ݣ��ޡ�=�������
 * �� �� ֵ���ɹ��򷵻���Ϣ�������ڴ��ָ�룬ʧ���򷵻�NULL
 */
unit_base_info_t *getBaseInfoMap(uchar_t chassis,uchar_t slot)
{
    if (slot<0 || slot>MAX_UNIT_COUNT)
        return NULL;
    if(ubi_shm_id==-1)
    {
        if (TRUE!=initUBIShmget(chassis))
            return NULL;
    }
    unit_base_info_t *bi;
    bi=(unit_base_info_t*)(ubi_shm_add+sizeof(unit_base_info_t)*slot);
    return bi;
}

char *getUnitPropertyMap(uchar_t chassis,uchar_t slot)
{
    char *property=NULL;

    if(ubi_shm_id==-1)
    {
        if (TRUE!=initUBIShmget(chassis))
            return NULL;
    }
    if (0==slot || getUnitClass(chassis,slot)>0)
    {
        property=(char*)(ubi_shm_add+sizeof(unit_base_info_t)*(MAX_UNIT_COUNT+1)+INFO_UNIT_PROPERTY_SIZE*slot);
    }
    return property;
}

char *getUnitProperty(uchar_t chassis,uchar_t slot)
{
    char *property=getUnitPropertyMap(chassis,slot);
    if (NULL!=property)
        property=property+4;
}


/*********************************************************************************
* �������ƣ� getStatusInfoSetMap
* ���������� ��ȡ�����ڴ��е���Ϣ����
* ���ʵı� ��
* �޸ĵı� ��
* ��������� uchar_t chassis,uchar_t slot,uchar_t infoset_id,uchar_t index
* ��������� ��
* �� �� ֵ�� �ɹ�����status_infoset_t *,���󷵻�NULL
* ����˵���� ����CC���������̲�Ҫ����޸ķ��ص���Ϣ��������
* �޸�����        �汾��     �޸���       �޸�����
* -----------------------------------------------
* 2012/10/18        V1.0      ���ķ�        ����
************************************************************************************/
status_infoset_t *getStatusInfoSetMap(uchar_t chassis,uchar_t slot,uchar_t infoset_id,uchar_t index)
{
    status_infoset_t *sis;
    char n;
    uchar_t  *begin,*next,count;

    if (slot<0 || slot>MAX_UNIT_COUNT)
        return NULL;
    if(usi_shm_id==-1)
    {
        if (TRUE!=initShmget(chassis))
            return NULL;
    }
    //printf("Here3\n");
    begin=(uchar_t*)(usi_shm_add+INFO_SET_HEAD_OFFSET);
    if (slot>0)
    {
        count=*(begin+slot)-*(begin+slot-1);
        //printf("Here C\n");
        begin=begin+slot-1;
    }
    else
    {
        count=*begin;
    }
    //printf("Here count=%d\n",count);
    //printf("slot=%d count=%d begin=%d %d %d\n",slot,count,*begin,*(begin+slot-1),*(begin+slot-2));
    if (count<1)
        return NULL;
    //sis=(status_infoset_t*)(usi_shm_add+INFO_HEAD_SIZE+(*begin) * sizeof(status_infoset_t));
    //printf("Hereb\n");
    for (n=0; n<count; n++)
    {
        sis=(status_infoset_t*)(usi_shm_add+INFO_HEAD_SIZE+((*begin)+n) * sizeof(status_infoset_t));
        //sis=begin+n*sizeof(status_infoset_t);
        printf("getStatusInfoSetMap:slot=%d infoset_id=%d index=%d\n",slot,sis->infoset_id,sis->index);
        if (sis->infoset_id==infoset_id && sis->index==index)
            return sis;
    }
    //printf("Here 5\n");
    return NULL;
}

char *getInfoSetValues_old(uchar_t chassis,uchar_t slot,uchar_t infoset_id,uchar_t index)
{
    char  *values;
    char n;
    uchar_t  *begin,*next,count;
    status_infoset_t *sis;

    //printf("slot=%d infoset_id=%d index=%d\n",slot,infoset_id,index);
    if (slot<0 || slot>MAX_UNIT_COUNT)
        return NULL;
    if(usi_shm_id==-1)
    {
        if (TRUE!=initShmget(chassis))
            return NULL;
    }
    //printf("Here3\n");
    begin=(uchar_t*)(usi_shm_add+INFO_SET_HEAD_OFFSET);
    next=(uchar_t*)(usi_shm_add+INFO_HEAD_SIZE);
    for (n=0; n<38; n++)
        printf("%d:%d ",n,next[n]);
    if (slot>0)
    {
        count=*(begin+slot)-*(begin+slot-1);
        //printf("Here C\n");
        begin=begin+slot-1;
    }
    else
    {
        count=*begin;
    }
    //printf("Here count=%d\n",count);
    printf("\nslot=%d count=%d begin=%d %d %d\n",slot,count,*begin,*(begin+slot-1),*(begin+slot-2));
    if (count<1)
        return NULL;
    //sis=(status_infoset_t*)(usi_shm_add+INFO_HEAD_SIZE+(*begin) * sizeof(status_infoset_t));
    //printf("Hereb\n");
    for (n=0; n<count; n++)
    {
        //sis=(status_infoset_t*)(usi_shm_add+INFO_HEAD_SIZE+((*begin)+n) * sizeof(status_infoset_t));
        //printf("infoset_id=%d",sis->infoset_id);
        values=(char *)(usi_shm_add+INFO_HEAD_SIZE+((*begin)+n-1) * sizeof(status_infoset_t));
        //sis=begin+n*sizeof(status_infoset_t);
        printf("infoset_id=%d:%d index=%d:%d\n",infoset_id,values[0],index,values[2]);
        if (values[0]==infoset_id && values[2]==index)
            return values;
    }
    for (n=0; n<0; n++)
        printf("%d:%d |",n,values[n]);
    //printf("Here 5\n");
    return NULL;
}
/*********************************************************************************
* �������ƣ� getStatusInfoSetMap
* ���������� ��ȡ�����ڴ��е�Ԫ��ȫ����Ϣ����
* ���ʵı� ��
* �޸ĵı� ��
* ��������� uchar_t chassis,uchar_t slot,status_infoset_t **infoset
* ��������� infoset
* �� �� ֵ�� �ɹ����ص�Ԫ����Ϣ������ *,���󷵻�0
* ����˵���� ����CC���������̲�Ҫ����޸ķ��ص���Ϣ��������
* �޸�����        �汾��     �޸���       �޸�����
* -----------------------------------------------
* 2012/10/18        V1.0      ���ķ�        ����
************************************************************************************/
uchar_t getUnitInfoSetsMap(uchar_t chassis,uchar_t slot,status_infoset_t **infoset)
{
    uchar_t *node,infoset_count;
    status_infoset_t *pis;
    if (slot<0 || slot>MAX_UNIT_COUNT)
        return 0;
    if(usi_shm_id==-1)
    {
        if (TRUE!=initShmget(chassis))
            return 0;
    }
    node=(uchar_t*)(usi_shm_add+INFO_SET_HEAD_OFFSET+slot);
    if (slot>0)
    {
        if (*(node-1)<1)
            infoset_count=*node;
        else
            infoset_count=*(node)-*(node-1);
        if (infoset_count>0)
        {
            pis=(status_infoset_t*)(usi_shm_add+INFO_HEAD_SIZE+*(node-1)*sizeof(status_infoset_t));
            *infoset=pis;
        }
        return infoset_count;
    }
    else
    {
        if (*(node)>0)
            *infoset=(status_infoset_t*)(usi_shm_add+INFO_HEAD_SIZE);
        return infoset_count;
    }
}

static int insertNode(void *dest,const void *src,size_t index,size_t node_count,size_t count)
{
    size_t n;
    //printf("index=%d node_count=%d\n",index,node_count);
    for (n=node_count; n>index; n--)
    {
        memcpy(dest+n*count,dest+(n-1)*count,count);
    }
    memset(dest+index*count,0,count);
    memcpy(dest+index*count,src,count);
}

void deleteSlotNodes(uchar_t chassis,uchar_t slot)
{
    char n;
    uchar_t sum,count;
    uchar_t  *from_node,*to_node,*iset_off;
    status_infoset_t *pis,*from_infoset,*to_infoset;

    if (slot<0 || slot>MAX_UNIT_COUNT)
        return;
    // printf("deleteSlotNodes\n");
    if(usi_shm_id==-1)
    {
        if (TRUE!=initShmget(chassis))
            return;
    }
    // printf("deleteSlotNodes\n");
    count=getUnitInfoSetsMap(chassis,slot,&pis);
    if (count<1)
        return;

    iset_off=(uchar_t*)(usi_shm_add+INFO_SET_HEAD_OFFSET);
    sum=*(iset_off);
    //printf("deleteSlotNodes\n");
    for (n=0; n<=MAX_UNIT_COUNT; n++)
    {
        sum+=*(iset_off+n)-*(iset_off+n);
    }
    printf("sum=%d count=%d slot=%d\n",sum,count,slot);
    if (sum<=count)
        return;
    memcpy(pis,pis+count,sizeof(status_infoset_t)*(sum-count));
    //printf("deleteSlotNodes 3\n");
    pis=(status_infoset_t*)(usi_shm_add+INFO_HEAD_SIZE+sizeof(status_infoset_t)*(sum-count));
    memset(pis,0,sizeof(status_infoset_t)*count);

    //printf("count=%d slot=%d\n",count,slot);
    for (n=slot; n<=MAX_UNIT_COUNT; n++)
    {
        //printf("count=%d slot=%d *(iset_off+%d)=%d\n",count,slot,*(iset_off+n),n);
        if (*(iset_off+n)>=count)
            *(iset_off+n)=*(iset_off+n)-count;
    }
}

/*********************************************************************************
* �������ƣ� insertStatusInfoSet
* ���������� �ڹ����ڴ��������Ϣ����
* ���ʵı� ��
* �޸ĵı� ��
* ��������� uchar_t chassis,uchar_t slot,status_infoset_t infoset
* ��������� ��
* �� �� ֵ�� �ɹ�����0,���󷵻�-1
* ����˵���� ��
* �޸�����        �汾��     �޸���       �޸�����
* -----------------------------------------------
* 2012/10/18        V1.0      ���ķ�        ����
*����:
* insertStatusInfoSet(0,1,infoset);
************************************************************************************/
/*
static int insertInfoSet_OLD(void *dest,const void *src,size_t index,size_t node_count,size_t count)
{
  size_t n;
  //printf("index=%d node_count=%d\n",index,node_count);
  for (n=node_count;n>index;n--)
  {
     memcpy(dest+n*count,dest+(n-1)*count,count);
  }
  memset(dest+index*count,0,count);
  memcpy(dest+index*count,src,count);
  return 0;
}
*/
int insertStatusInfoSet(uchar_t chassis,uchar_t slot,status_infoset_t infoset)
{
    uchar_t *iset_off;
    status_infoset_t *sis,*p;
    int n,m=0,count=0,sum=0;
    size_t index=0;
    uchar_t begin,end;

    uchar_t iset_count=0;
    if (slot<0 || slot>MAX_UNIT_COUNT)
        return -1;
    if(usi_shm_id==-1)
    {
        if (TRUE!=initShmget(chassis))
            return -1;
    }
    //printf("Here\n");
    if (NULL!=getStatusInfoSetMap(chassis,slot,infoset.infoset_id,infoset.index))
    {
        printf("Slot #%d infoset_id=%d index=%d is exists\n",slot,infoset.infoset_id,infoset.index);
        return -1;
    }
    //printf("*(iset_off)=%d\n",1);
    iset_off=(uchar_t*)(usi_shm_add+INFO_SET_HEAD_OFFSET);
    //iset_count=(uchar_t*)(usi_shm_add+INFO_SET_HEAD_OFFSET+16);
    //printf("*(iset_off)=%d\n",*(iset_off));
    sum=*(iset_off+slot);
    for (n=0; n<slot; n++)
    {
        sum+=*(iset_off+n)-*(iset_off+n-1);
    }
    for (n=slot+1; n<=MAX_UNIT_COUNT; n++) //��Ԫ�̺����Ϣ����
    {
        count+=*(iset_off+n)-*(iset_off+n-1);
    }
    sum+=count; //��Ϣ������
    //printf("sum=%d\n",sum);
    if (sum>INFO_SET_MAX_COUNT)
    {
        // printf("over SUM\n");
        return -1;
    }
    /*�Ӳ�����Ϣ���Ĳ�λ��ʼ��������Ϣ����������1*/
    for (n=slot; n<=MAX_UNIT_COUNT; n++)
    {
        if (*(iset_off+n)<=INFO_SET_MAX_COUNT)
            *(iset_off+n)+=1;
        else
            printf("over INFO_SET_MAX_COUNT\n");
    }

    //sis=(status_infoset_t*)(usi_shm_add+INFO_HEAD_SIZE);

    if (slot>0)
    {
        iset_off=(uchar_t*)(usi_shm_add+INFO_SET_HEAD_OFFSET);
        //count=*(iset_off+slot-1)-*(iset_off+slot-2);
        if (*(iset_off+slot)-*(iset_off+slot-1)>0)
            iset_off=(uchar_t*)(usi_shm_add+INFO_SET_HEAD_OFFSET+slot-1)+count;
        else
            iset_off=(uchar_t*)(usi_shm_add+INFO_SET_HEAD_OFFSET+slot-1);
    }
    else //���ڵ���Ϣ��
        iset_off=(uchar_t*)(usi_shm_add+INFO_SET_HEAD_OFFSET);
    //printf("sum=%d count=%d *iset_off=%d\n",sum,count,*iset_off);
    /* ������Ϣ�����������Ϣ������Ų*/
    //insertInfoSet(sis,&infoset,*iset_off-1,sum,sizeof(status_infoset_t));
    sis=(status_infoset_t*)(usi_shm_add+INFO_HEAD_SIZE);
    sis=sis+sum;
    printf("Sum=%d count=%d *iset_off=%d\n",sum,count,*iset_off);
    for (n=sum; n>(sum-count); n--)
    {
        memcpy(sis,sis-1,sizeof(status_infoset_t));
        sis--;
    }

    sis=(status_infoset_t*)(usi_shm_add+INFO_HEAD_SIZE);
    sis=sis+(sum-count);
    memset(sis,0,sizeof(status_infoset_t));
    memcpy(sis,&infoset,sizeof(status_infoset_t));
    //for (n=0;n<10;n++)
    printf("\n%d %d",sis->infoset_id,sis->index);
    return 0;
}
/*
int getInfoSets_old(uchar_t chassis,uchar_t slot,status_infoset_t **infoset)
{
  char *iset_off;
  int count,n;
  if (slot<0 || slot>MAX_UNIT_COUNT)
     return -1;
  if(usi_shm_id==-1)
  {
    if (TRUE!=initShmget(chassis))
       return -1;
  }

  iset_off=(char *)(usi_shm_add+INFO_SET_HEAD_OFFSET+(slot+1)*2);

  count=*(iset_off+1);
  if (*(iset_off)<0 || *(iset_off)>INFO_SET_MAX_COUNT)
  {
    infoset=NULL;
    return -1;
  }
  if (count<1)
  {
    infoset=NULL;
    return 0;
  }
  //printf("count=%d *(iset_off)=%d\n",count,*(iset_off));
  *infoset=(status_infoset_t *)(usi_shm_add+INFO_SET_OFFSET+*(iset_off)*sizeof(status_infoset_t));
  iset_off=(char *)(usi_shm_add+INFO_SET_OFFSET+*(iset_off)*sizeof(status_infoset_t));

  return count;
}
*/
int getInfoSets(uchar_t chassis,uchar_t slot,status_infoset_t **infoset)
{
    uchar_t *iset_off;
    int count,n;

    if (slot<0 || slot>MAX_UNIT_COUNT)
        return -1;
    if(usi_shm_id==-1)
    {
        if (TRUE!=initShmget(chassis))
            return -1;
    }
    
    iset_off=getUnitStatusMap(0);
    //printf("getInfoSets:slot=%d uclass=%d\n",slot,*(iset_off+slot));
    iset_off=(uchar_t *)(usi_shm_add+INFO_SET_HEAD_OFFSET);
    count=*(iset_off+slot);
    //*(iset_off+slot)=infoset_count;
    *infoset=(status_infoset_t*)(usi_shm_add+INFO_SET_OFFSET+(slot*INFO_SET_MAX_COUNT_ERVERY_UNIT)*sizeof(status_infoset_t));
    //*infoset=(status_infoset_t *)(usi_shm_add+INFO_SET_OFFSET+*(iset_off)*sizeof(status_infoset_t));
    return count;
}

status_infoset_t *getInfoSet(uchar_t chassis,uchar_t slot,uchar_t infoset_id,uchar_t index)
{
    status_infoset_t *infoset=NULL;
    int n,count;

    if (slot<0 || slot>MAX_UNIT_COUNT)
        return NULL;
    count=getInfoSets(chassis,slot,&infoset);
    for (n=0; n<count; n++)
    {
        if (infoset!=NULL)
        {
            //printf("%d:Infoset id=%d index=%d\n",n,infoset->infoset_id,infoset->index);
            if (infoset->infoset_id==infoset_id && infoset->index==index)
            {
                return infoset;
            }
        }
        infoset++;
    }
    return NULL;
}


/*
 *����ָ����λ����Ϣ�š���Ϣ�����Ż�ȡ��Ϣ�������ڴ��ָ��
 *���������
 *chassis�������
 *slot����λ��
 *infoset_id����Ϣ��Id
 *index:��Ϣ������
 *�����ų��˿��У���=��ǰ�������ݣ��ޡ�=�������
 * �� �� ֵ���ɹ��򷵻���Ϣ�������ڴ��ָ�룬ʧ���򷵻�NULL
 */
char *getInfoSetValues(uchar_t chassis,uchar_t slot,uchar_t infoset_id,uchar_t index)
{
    uchar_t *iset_off;
    status_infoset_t *infoset;
    int count,n;

    if (slot<0 || slot>MAX_UNIT_COUNT)
        return NULL;
    if(usi_shm_id==-1)
    {
        if (TRUE!=initShmget(chassis))
            return NULL;
    }
    infoset=getInfoSet(chassis,slot,infoset_id,index);
    if (infoset!=NULL)
    {
        return (infoset->item_values);
    }
    else
    {
        //printf("getInfoSetValues:chassis=%d,slot=%d,infoset_id=%d,index%d=\n",chassis,slot,infoset_id,index);
        return NULL;
    }
}
void clearShm(char flag)
{
    uchar_t *iset_off;
    if(usi_shm_id==-1)
    {
        if (TRUE!=initShmget(0))
            return;
    }
    iset_off=(uchar_t *)(usi_shm_add);
    if (flag==0)
        memset(iset_off,0,INFO_HEAD_SIZE+sizeof(status_infoset_t)*INFO_SET_MAX_COUNT);
    else if (flag==1)
        memset(iset_off,0,INFO_HEAD_SIZE);
    else if (flag==2)
        memset(iset_off+INFO_SET_HEAD_OFFSET,0,sizeof(status_infoset_t)*INFO_SET_MAX_COUNT);
}
/*
int insertInfoSet_old(uchar_t chassis,uchar_t slot,status_infoset_t infoset[],int infoset_count)
{
  int n,m,count=0,sum,last_node;
  uchar_t *iset_off,node_off;
  status_infoset_t *sis;
  if (slot<0 || slot>MAX_UNIT_COUNT ||infoset_count<1 || infoset_count>INFO_SET_MAX_COUNT_ERVERY_UNIT)
     return -1;
  if(usi_shm_id==-1)
  {
    if (TRUE!=initShmget(chassis))
       return -1;
  }
  iset_off=(uchar_t *)(usi_shm_add+INFO_SET_HEAD_OFFSET);
  sum=*(iset_off);
  last_node=*(iset_off+1);
  count=*(iset_off+slot*2+3);
  printf("sum=%d last_node=%d count=%d infoset_count=%d\n",sum,last_node,count,infoset_count);
  if (count==infoset_count)
  {
    node_off=*(iset_off+slot*2+2);
    sis=(status_infoset_t*)(usi_shm_add+INFO_SET_OFFSET+node_off*sizeof(status_infoset_t));
    for (n=0;n<count;n++)
      memcpy(sis+n,&infoset[n],sizeof(status_infoset_t));
    return;
  }
  else
  {
    int step=infoset_count-count;
    *(iset_off)=*(iset_off)+step;
    for (n=slot+1;n<MAX_UNIT_COUNT;n++)
    {
        if (*(iset_off+n*2+3)>0)
           *(iset_off+n*2+3)=*(iset_off+n*2+3)+step;
    }
    if (count<infoset_count)
    {
      step=infoset_count-count;
      *(iset_off+slot*2+3)=infoset_count;
      node_off=*(iset_off+slot*2+2)+infoset_count-count;
      sis=(status_infoset_t*)(usi_shm_add+INFO_SET_OFFSET+(last_node+step)*sizeof(status_infoset_t));
      for (n=last_node-node_off;n>0;n--)
      {
         memcpy(sis,sis-1,sizeof(status_infoset_t));
         sis--;
      }
    }
    printf("*(iset_off+slot*2+3)=%d\n",*(iset_off+slot*2+2));
    sis=(status_infoset_t*)(usi_shm_add+INFO_SET_OFFSET+*(iset_off+slot*2+2)*sizeof(status_infoset_t));
    for (n=0;n<infoset_count;n++)
        memcpy(sis+n,&infoset[n],sizeof(status_infoset_t));
   }

  return 0;
}
*/
int insertInfoSet(uchar_t chassis,uchar_t slot,status_infoset_t infoset[],int infoset_count)
{
    int n,m,count=0,sum,last_node;
    uchar_t *iset_off,node_off;
    status_infoset_t *sis;
    if (slot<0 || slot>MAX_UNIT_COUNT ||infoset_count<1 || infoset_count>INFO_SET_MAX_COUNT_ERVERY_UNIT)
        return -1;
    if(usi_shm_id==-1)
    {
        if (TRUE!=initShmget(chassis))
            return -1;
    }
    iset_off=(uchar_t *)(usi_shm_add+INFO_SET_HEAD_OFFSET);
    *(iset_off+slot)=infoset_count;
    sis=(status_infoset_t*)(usi_shm_add+INFO_SET_OFFSET+(slot*INFO_SET_MAX_COUNT_ERVERY_UNIT)*sizeof(status_infoset_t));
    for (n=0; n<infoset_count; n++)
    {
        memcpy(sis+n,&infoset[n],sizeof(status_infoset_t));
        //printf("insertInfoSet:infoset_id=%d\n",infoset[n].infoset_id);
    }
    return 0;
}

boolean_t getStatusInfoSet(uchar_t chassis,uchar_t slot,status_infoset_t *infoset)
{
    status_infoset_t *sis;
    sis=getStatusInfoSetMap(chassis,slot,infoset->infoset_id,infoset->index);
    if (NULL==sis)
        return FALSE;
    //printf("Here 2\n");
    memcpy(infoset,sis,sizeof(status_infoset_t));
    return TRUE;
}

void showInfoSet(uchar_t chassis,uchar_t slot,uchar_t infoset_id,uchar_t index)
{
    status_infoset_t infoset;
    char n,m;
    infoset.infoset_id=infoset_id;
    infoset.index=index;
    if (TRUE!=getStatusInfoSet(chassis,slot,&infoset))
    {
        printf("slot=%d infoset_id=%d not found\n",slot,infoset_id);
        return;
    }

    for (m=0; m<infoset.item_count; m++)
    {
        printf("item id[%d]=%d  value=%d ",m,m+1,infoset.item_values[m]);
    }
    printf("\n");
}

boolean_t setStatusInfoSet(uchar_t chassis,uchar_t slot,status_infoset_t infoset)
{
    status_infoset_t *sis;
    sis=getStatusInfoSetMap(chassis,slot,infoset.infoset_id,infoset.index);
    if (NULL!=sis)
    {
        memcpy(sis,&infoset,sizeof(status_infoset_t));
        return TRUE;
    }
    //printf("slot=%d infoset_id=%d not found\n",slot,infoset.infoset_id);
    return FALSE;
}
void removeShmget(uchar_t chassis)
{
    int shmid;
    shmid = shmget(STATUS_SHM_KEY+chassis,INFO_HEAD_SIZE+sizeof(status_infoset_t)*INFO_SET_MAX_COUNT, IPC_CREAT | 0666);
    //ɾ�������ڴ��
    if(shmctl(shmid,IPC_RMID,NULL)<0)
        perror("error");

}

/**********************************************************************
* �������ƣ� getUnitStatusMap
* ���������� ��ȡ������0-15��Ԫ�̵���λ״̬������
* ���ʵı� ��
* �޸ĵı� ��
* ��������� uchar_t chassis�������
* ��������� ��
* �� �� ֵ�� 0-15����Ԫ�̵Ĵ���״̬λ,��Ӧ���ֽ�Ϊ0��15��Ϊ��0Ϊ��Ԫ�̵Ĵ��ࡢ����ڣ����򲻴��ڣ�16��31�ֽ�Ϊ��Ԫ�̵�С��
* ����˵���� ��
* �޸�����        �汾��     �޸���       �޸�����
* -----------------------------------------------
* 2012/10/19        V1.0      ���ķ�        ����
*
***********************************************************************/
uchar_t *getUnitStatusMap(uchar_t chassis)
{
    if (chassis>31)
        return NULL;
    if(usi_shm_id==-1)
    {
        if (TRUE!=initShmget(chassis))
            return NULL;
    }
    return (uchar_t *)(usi_shm_add);
}

uchar_t getChassisClass(uchar_t chassis)
{
    uchar_t cclass=0;
    uchar_t *pcclass;
    pcclass=getUnitStatusMap(0);
    if (NULL!=pcclass)
        cclass=*pcclass;
    return cclass;
}
char isValidChassisClass(uchar_t cclass)
{
    if (cclass>0 && cclass<3 || cclass==4 || cclass>10 && cclass<13)
        return 1;
    else
        return 0;
}
/*
uchar_t GetChassisClass(uchar_t chassis)
{
   uchar_t cclass getChassisClass(chassis);
}
*/
static uchar_t slotOfNmu=0;
uchar_t getSlotOfNmu(uchar_t chassis,int retry)
{
    if (slotOfNmu>0 && slotOfNmu<17)
        return slotOfNmu;
    else
    {
        uchar_t n,m;//,nmu_slot_max=16,nmu_slot_min=14;
        uchar_t *uc,uclass;
        if(usi_shm_add==NULL)
        {
            if (TRUE!=initShmget(chassis))
                return 0;
        }
        if(usi_shm_add==NULL)
            return 0;
        //int n;
        /*
        uc=(uchar_t *)(usi_shm_add);
        for (n=0;n<16;n++)
        {
         uclass=*(uc+n);
         printf("%d:%d ",n,uclass);
        }
        printf("\n");
        */
        if (retry<1)
            retry=1;
        //else
        //    printf("getSlotOfNmu retry:%d\n",retry);
        for (n=0; n<retry; n++)
        {
            for (m=16; m>1; m--)
            {
                uc=(uchar_t *)(usi_shm_add+m);
                uclass=*uc;
                //printf("getSlotOfNmu n=%d uc=%d\n",n,uclass);
                if (1==uclass)
                {
                    slotOfNmu=m;
                    return m;
                }
            }
            if (retry>1)
                sleep(1);
        }
    }
    return 0;
    /*
     for (n=0;n<retry;n++)
     {
       uc=(uchar_t *)(usi_shm_add);
       uclass=*uc;
       if (1==uclass)
       {
         nmu_slot_max=3;
         nmu_slot_min=1;
       }
       else if (4==uclass)
       {
         nmu_slot_max=16;
         nmu_slot_min=14;
       }
       else if (2==uclass)
       {
         nmu_slot_max=8;
         nmu_slot_min=1;
       }
       else
       {

          sleep(1);
          continue;
       }
       if (uclass!=0)
       {
        for (m=nmu_slot_max;m>nmu_slot_min;m--)
        {
         uc=(uchar_t *)(usi_shm_add+m);
         uclass=*uc;
         //printf("getSlotOfNmu n=%d uc=%d\n",n,uclass);
         if (1==uclass)
         {
          slotOfNmu=m;
          return m;
         }
        }
       }
       //sleep(1);
     }
     //printf("getSlotOfNmu m=%d uc=%d\n",m,uclass);
     if (uclass!=0)
       slotOfNmu=nmu_slot_max;
     return 0;
     }
    */
}

uchar_t getUnitClass(uchar_t chassis,uchar_t slot)
{
    if (slot<0 || slot>MAX_UNIT_COUNT)
        return 0;
    if(usi_shm_id==-1)
    {
        if (TRUE!=initShmget(chassis))
            return 0;
    }

    uchar_t *uc,uclass;
    uc=(uchar_t *)(usi_shm_add+slot);
    uclass=*uc;
    return uclass;
}

uchar_t getUnitType(uchar_t chassis,uchar_t slot)
{
    //printf("slot=%d\n");
    if (slot<0 || slot>MAX_UNIT_COUNT)
        return 0;
    if(usi_shm_id==-1)
    {
        //printf("slot=%d\n");
        if (TRUE!=initShmget(chassis))
            return 0;
    }
    uchar_t *ut;
    //printf("slot=%d\n");
    ut=(uchar_t *)(usi_shm_add +MAX_UNIT_COUNT+slot);
    return *ut;
}

boolean_t getUnitClassType(uchar_t chassis,uchar_t slot,uchar_t *uclass,uchar_t *utype)
{
    if (slot<0 || slot>MAX_UNIT_COUNT)
        return FALSE;
    if(usi_shm_id==-1)
    {
        if (TRUE!=initShmget(chassis))
            return FALSE;
    }
    uchar_t *uc;
    //uc=(uchar_t *)(usi_shm_add+INFO_HEAD_SIZE +slot-1);
    uc=(uchar_t *)(usi_shm_add+slot);
    *uclass=*uc;
    *utype=*(uc+MAX_UNIT_COUNT);
    return TRUE;
}

uchar_t getUnitName(uchar_t chassis,uchar_t slot,char *unit_name,char flag)
{
    uchar_t uclass;
    uclass=getUnitClass(chassis,slot);
    if (uclass<1)
    {
        //printf("unit:%d\n",slot);
        return 0;
    }
    unit_base_info_t *bi;
    char *un;
    //printf("unit_name:?n");
    bi=getBaseInfoMap(chassis,slot);
    if (NULL!=bi)
    {
        strncpy(unit_name,bi->property,3);
        if (flag==1)
            un=strTolower(unit_name);
        unit_name[3]='\0';
        //printf("unit_name:%s\n",unit_name);
        return uclass;
    }
    else
        return 0;
}
void setUnitStatusToShm(uchar_t chassis,uchar_t slot,uchar_t uclass,uchar_t utype)
{
    if (slot<0 || slot>MAX_UNIT_COUNT)
        return;
    if(usi_shm_id==-1)
    {
        if (TRUE!=initShmget(chassis))
            return;
    }
    uchar_t *uc;
    uc=(uchar_t *)(usi_shm_add+INFO_HEAD_SIZE +slot);
    //ut=(uchar_t *)(usi_shm_add+INFO_HEAD_SIZE +15+slot);
    *uc=uclass;
    *(uc+16)=utype;
}
/*
static uchar_t *getMsgFromShm(uchar_t chassis,uchar_t msgflag)
{
   if (chassis>31 || msgflag>1)
     return NULL;
   if(usi_shm_id==-1)
    {
      if (TRUE!=initShmget(chassis))
       return NULL;
    }
    if (msgflag)
      return (uchar_t *)(usi_shm_add+CMD_MESSAGE_OFFSET);
    else
      return (uchar_t *)(usi_shm_add+DATA_MESSAGE_OFFSET);
}
static boolean_t setMsgToShm(uchar_t chassis,uchar_t msgflag,uchar_t *message)
{
   if (chassis>31 || msgflag>1)
     return FALSE;
   if(usi_shm_id==-1)
   {
     if (TRUE!=initShmget(chassis))
       return FALSE;
   }
   uchar_t *msg;
   if (msgflag)
      msg=(uchar_t *)(usi_shm_add+CMD_MESSAGE_OFFSET);
   else
      msg=(uchar_t *)(usi_shm_add+DATA_MESSAGE_OFFSET);
   memcpy(msg,message,32);
   return TRUE;
}
uchar_t *getCmdMsgFromShm(uchar_t chassis)
{
   return getMsgFromShm(chassis,1);
}
boolean_t setCmdMsgToShm(uchar_t chassis, uchar_t *message)
{
  return setMsgToShm(chassis,1,message);
}

uchar_t *getDataMsgFromShm(uchar_t chassis)
{
   return getMsgFromShm(chassis,0);
}
boolean_t setDataMsgToShm(uchar_t chassis, uchar_t *message)
{
   return setMsgToShm(chassis,0,message);
}
*/
void showAllInfoset()
{
    status_infoset_t *si,*p;
    uchar_t *node;
    char n;
    uchar_t  begin,*from_node,*to_node;

    if(usi_shm_id==-1)
    {
        if (TRUE!=initShmget(0))
            return;
    }
    si=(status_infoset_t*)(usi_shm_add+INFO_HEAD_SIZE);
    //si=p;
    for (n=0; n<=MAX_UNIT_COUNT; n++)
    {
        node=(uchar_t*)(usi_shm_add+INFO_SET_HEAD_OFFSET+n);
        //si=(status_infoset_t*)(usi_shm_add+INFO_HEAD_SIZE+sizeof(status_infoset_t)*n);
        if (*node>0)
            printf("n=%d *node=%d\n",n,*node);
        if (si->infoset_id>0)
            printf("n=%d infoset_id=%d index=%d\n",n,si->infoset_id,si->index);
        si++;
    }
}

int main_(int argc, char **argv)
{
    status_infoset_t infoset;
    int slot=0;
    uchar_t n,m;
    if (argc<2)
        return 0;
    memset(&infoset,0,sizeof(status_infoset_t));
    if (!strncmp(argv[1],"-a",2))
    {
        showAllInfoset();
    }
    else if (!strncmp(argv[1],"-i",2))
    {
        if (argc>6)
        {
            slot=atoi(argv[2]);
            infoset.infoset_id=atoi(argv[3]);
            infoset.infoset_type=atoi(argv[4]);
            infoset.index=atoi(argv[5]);
            //for (n=6;n<argc;n++)
            //  infoset.infoitems[n-6].item_id=atoi(argv[n]);
            infoset.item_count=argc-6;
            //printf("Slot=%d infoset.infoset_id=%d infoset.infoset_type=%d index=%d infoset.item_count:%d\n",slot,infoset.infoset_id,infoset.infoset_type,infoset.index,infoset.item_count);
            uchar_t *pus=getUnitStatusMap(0);
            if (pus!=NULL)
            {
                if (*(pus+slot-1)<1)
                    *(pus+slot-1)=3;
                //printf("infoset.item_count:%d\n",infoset.item_count);
                insertStatusInfoSet(0,slot,infoset);
            }
        }
    }
    else if (!strncmp(argv[1],"-d",2))
    {
        if (argc>4)
        {
            slot=atoi(argv[2]);
            showInfoSet(0,slot,atoi(argv[3]),atoi(argv[4]));
        }
        else if (argc>2)
        {
            uchar_t count=1,m;
            uchar_t *uc;
            slot=atoi(argv[2]);
            status_infoset_t *pis;

            uc=getUnitStatusMap(0);
            count=getUnitInfoSetsMap(0,slot,&pis);

            //add=&next;
            for (n=0; n<count; n++)
            {
                printf("slot=%d uclass=%d infoset id=%d index=%d item_count=%d\n",slot,*(uc+slot-1),pis->infoset_id,pis->index,pis->item_count);
                for (m=0; m<pis->item_count; m++)
                    printf("item id=%d value=%d\n",m+1,pis->item_values[m]);
                printf("\n");
                pis++;
                // for (m=0;m<pis->item_count;m++)
                //  printf("item id=%d item value=%d\n",m+1,pis->infoitems[0].value);
                //printf("\n");
            }
        }
    }
    else if (!strncmp(argv[1],"-s",2))
    {
        if (argc>5)
        {
            slot=atoi(argv[2]);
            infoset.infoset_id=atoi(argv[3]);
            infoset.index=atoi(argv[4]);
            if (TRUE!=getStatusInfoSet(0,slot,&infoset))
                return 1;
            for (n=5; n<argc; n++)
                infoset.item_values[n-5]=atoi(argv[n]);
            if (setStatusInfoSet(0,slot,infoset)!=FALSE)
                printf("OK\n");
        }
    }
    else if (!strncmp(argv[1],"-r",2))
    {
        slot=atoi(argv[2]);
        deleteSlotNodes(0,slot);
    }
    else if (!strncmp(argv[1],"-c",2))
    {
        removeShmget(0);
    }
    else if (!strncmp(argv[1],"-t",2))
    {
        if (argc>4)
        {
            slot=atoi(argv[2]);
            setUnitStatusToShm(0,slot,atoi(argv[3]),atoi(argv[4]));
        }
    }
    else if (!strncmp(argv[1],"-y",2))
    {
        uchar_t uc,ut,uclass;
        unit_base_info_t *bi;
        if (argc>2)
        {
            slot=atoi(argv[2]);
            getUnitClassType(0,slot,&uc,&ut);
            uclass=getUnitClass(0,slot);
            printf("slot=%d uc=%d ut=%d uclass=%d\n",slot,uc,ut,uclass);
            if (uc>0)
            {
                bi=getBaseInfoMap(0,slot);
                if (bi!=NULL)
                    printf("property=%s\n",bi->property);
            }
        }
        else
        {
            int n;
            for (n=0; n<=MAX_UNIT_COUNT; n++)
            {
                getUnitClassType(0,n+1,&uc,&ut);
                uclass=getUnitClass(0,n+1);

                if (uc>0)
                {
                    // printf("slot=%d uc=%d ut=%d uclass=%d\n",n+1,uc,ut,uclass);
                    bi=getBaseInfoMap(0,n+1);
                    if (bi!=NULL)
                        printf("#%d property=%s uclass=%d\n",n+1,bi->property,uc);
                }
            }
        }
    }
    else if (!strncmp(argv[1],"-b",2))
    {
        if (argc>2)
        {
            uchar_t uc,ut,uclass;
            slot=atoi(argv[2]);
            getUnitClassType(0,slot,&uc,&ut);
            uclass=getUnitClass(0,slot);
            printf("slot=%d uc=%d ut=%d uclass=%d\n",slot,uc,ut,uclass);
        }
    }
    return 1;
}
