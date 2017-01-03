#include <sys/sysinfo.h>
#include <time.h>
#include "../wsIntrn.h"
#include "../../include/unit.h"
#include <visipc.h>
#include <sqlite3.h>
#include <dbutil.h>
#include <log.h>
#include <otu.h>
#include <ddm.h>

int getSelectedUnit();

static int aspDdmInfo(int eid, webs_t wp, int argc, char_t **argv)
{
  int n,nBytesSent=0,slot;
  char_t *uclass,*group;
  char buf[256];
  uchar_t port_count;
  uchar_t uc,utype=1;
  slot=getSelectedUnit();
  
  if (ejArgs(argc, argv, T("%s %s"), &uclass, &group) < 2)
  //if (ejArgs(argc, argv, T("%s"), &uclass) < 1)
  {  
     websError(wp, 400, T("Insufficient args\n"));
     return -1;
  }
  uc=getUnitClass(0,slot);
  if (!(UC_OTU==uc || UC_OEO== uc ||UC_OTAP==uc))
  { 
     if (UC_OTU==uclass)
       websError(wp, 400, T("OTU not exists on slot #%d\n"),slot);
     else if (UC_OEO==uclass)
        websError(wp, 400, T("OEO not exists on slot #%d\n"),slot);
     else
        websError(wp, 400, T("OTAP not exists on slot #%d\n"),slot);
     return -1;
  }
  utype=getUnitType(0,slot);
  nBytesSent+=websWrite(wp, T("var uc=%d,ut=%d;\n"),uc,utype);
  if (utype==1)
    port_count=4;
  else  if(utype==OTU_TYPE_10G_SFP_PLUS_A)
  {
    char *pOtup;
    pOtup=getUnitProperty(0,slot);
    port_count=pOtup[9];
    //printf("port_count=%d...\n");
    if (!(port_count==4 || port_count==8))
        port_count=4;
  }
  else
    port_count=8;
  //printf("port_count=%d\n",port_count);
  char *values;
   otuUnitStatus_t *pOtuStatus=NULL;
   values=getInfoSetValues(0,slot,OTU_STATUS_INFOSET_ID,1);
   if (NULL!=values)
   {  
    pOtuStatus=(otuUnitStatus_t *)values;
   }
  if (!gstrcmp(group,"1"))
  {
   ddm_t *ddm;
   //char *values;
   //otuUnitStatus_t *pOtuStatus;
   //values=getInfoSetValues(0,slot,OTU_STATUS_INFOSET_ID,1);
   if (NULL!=values)
   {
    
    //pOtuStatus=(otuUnitStatus_t *)values;
    nBytesSent+=websWrite(wp, T("var p=new Array(%d);\n"),port_count);
    for (n=1;n<port_count+1;n++)
    {
     //printf("n=%d\n",n);
     ddm=getPortDdmInfo(0,slot,n);
     if (ddm!=NULL)
     {
       if (1==pOtuStatus->portStatus[n-1].on_module)
       {
         nBytesSent+=websWrite(wp, T("p[%d]= new Object();"),n-1);
         nBytesSent+=websWrite(wp, T("p[%d].rpower=%d;p[%d].tpower=%d;p[%d].temp=%d;p[%d].volt=%d;p[%d].bias=%d;"),n-1,ddm->rxPower,n-1,ddm->txPower,n-1,ddm->temperature,n-1,ddm->rxVolt,n-1,ddm->rxBiasCurrent);
         /*sprintf(buf,"<tr><td>%d</td><td>%.1f</td><td>%.1f</td><td>%.1f</td><td>%.1f</td><td>%d</td></tr>",n,0.1*ddm->rxPower,0.1*ddm->txPower,0.1*ddm->temperature,0.1*ddm->rxVolt,ddm->rxBiasCurrent);
        nBytesSent+=websWrite(wp, T("%s"),buf);
*/
       }
       //nBytesSent+=websWrite(wp, T("<tr><td>%d</td><td>%d</td><td>%d</td><td>%d</td><td>%d </td><td>%d</td></tr>"),n,ddm->rxPower,ddm->txPower,ddm->temperature,ddm->rxVolt,ddm->rxBiasCurrent);
     }
     //else
     //   nBytesSent=websWrite(wp, T("<tr><td>DDM not found!</td></tr>"));
    }
   }
   else
        nBytesSent+=websWrite(wp, "var p=null;");
 }
 else if (!gstrcmp(group,"2"))
 {
   ddmThreshold_t *ddmt;
   nBytesSent+=websWrite(wp,T("var rxMin=new Array(%d),rxMax=new Array(%d),txMin=new Array(%d),txMax=new Array(%d),tMin=new Array(%d),tMax=new Array(%d),vMin=new Array(%d),vMax=new Array(%d),bMin=new Array(%d),bMax=new Array(%d);\n"),port_count,port_count,port_count,port_count,port_count,port_count,port_count,port_count,port_count,port_count);
   //ddmt=getPortDdmThreshold(0, slot,1);
   for (n=1;n<port_count+1;n++)
   {
     ddmt=getPortDdmThreshold(0, slot,n);
     if (ddmt==NULL)
     {
        nBytesSent+=websWrite(wp, T("<tr><td>%d</td><td colspan='11'>DDM threshold NOT found!</td></tr>"),n);
        return nBytesSent;
     }

     if (utype!=2)
     {
       //nBytesSent+=websWrite(wp,"var ");
       if (1==pOtuStatus->portStatus[n-1].on_module)
       {
       nBytesSent+=websWrite(wp, T("rxMin[%d]=%d;rxMax[%d]=%d;"),n-1,ddmt->rxPowerMin,n-1,ddmt->rxPowerMax);
       nBytesSent+=websWrite(wp, T("txMin[%d]=%d;txMax[%d]=%d;"),n-1,ddmt->txPowerMin,n-1,ddmt->txPowerMax);
       nBytesSent+=websWrite(wp, T("tMin[%d]=%d;tMax[%d]=%d;"),n-1,ddmt->tempMin,n-1,ddmt->tempMax);
       nBytesSent+=websWrite(wp, T("vMin[%d]=%d;vMax[%d]=%d;"),n-1,ddmt->voltMin,n-1,ddmt->voltMax);
       nBytesSent+=websWrite(wp, T("bMin[%d]=%d;bMax[%d]=%d;\n"),n-1,ddmt->rxBiasMin,n-1,ddmt->rxBiasMax);
       }
       else
          nBytesSent+=websWrite(wp, T("rxMin[%d]=null;"),n-1);
     }

    /* if (utype==1)
     {
     nBytesSent+=websWrite(wp, T("<tr><form id='F%d' name='otuForm%d' method='post' action='/goform/ddmt' style='margin:0;' onsubmit='return app(0,%d,%d);'>"),n,n,slot,n);
     nBytesSent+=websWrite(wp, T("<td>%d</td>"),n);

     nBytesSent=websWrite(wp, T("<td><input type='text' id ='i%d1' value='%d' name='minrx' maxlength='3' size='3' onblur='check(this,1,%d);'/></td> \
     <td><input type='text' id ='i%d2' name='maxrx' value='%d' maxlength='3' size='3' onblur='check(this,2,%d);'/></td>"),n,ddmt->rxPowerMin,n,n,ddmt->rxPowerMax,n);

     nBytesSent+=websWrite(wp, T("<td><input type='text' id ='i%d3' value='%d' name='mintx' maxlength='3' size='3' onblur='check(this,3,%d);'/></td> \
     <td><input type='text' id ='i%d4' name='maxtx' value='%d' maxlength='3' size='3' onblur='check(this,4,%d);'/></td>"),n,ddmt->txPowerMin,n,n,ddmt->txPowerMax,n);

     nBytesSent+=websWrite(wp, T("<td><input type='text' id ='i%d5' name='mintemp' value='%d' maxlength='3' size='3' onblur='check(this,5,%d);'/></td>\
     <td><input type='text' id ='i%d6' name='maxtemp' value='%d' maxlength='3' size='3' onblur='check(this,6,%d);'/></td>"),n,ddmt->tempMin,n,n,ddmt->tempMax,n);

     nBytesSent+=websWrite(wp, T("<td><input type='text' id ='i%d7' name='minvolt' value='%d' maxlength='3' size='3' onblur='check(this,7,%d);'/></td>\
     <td><input type='text' id ='i%d8' value='%d' name='maxvolt' maxlength='3' size='3' onblur='check(this,8,%d);'/></td>"),n,ddmt->voltMin,n,n,ddmt->voltMax,n);

     nBytesSent+=websWrite(wp, T("<td><input type='text' id ='i%d9' name='minbias' value='%d' maxlength='3' size='3' onblur='check(this,9,%d);'/></td> \
     <td><input type='text' id ='i%d10' name='maxbias' value='%d' maxlength='3' size='3' onblur='check(this,10,%d);'/></td>"),n,ddmt->rxBiasMin,n,n,ddmt->rxBiasMax,n);

     nBytesSent+=websWrite(wp, T("<td><input type='hidden' name=uid value='%d'/> <input type='hidden' name=port value='%d'/><input type='submit' value='提交' style=\"background-image:url('images/pen.gif');\"/></td></form></tr>"),slot,n);
    }
    else
    {
     nBytesSent+=websWrite(wp, "<tr>");
     nBytesSent+=websWrite(wp, T("<td>%d</td>"),n);

     nBytesSent=websWrite(wp, T("<td>%d</td><td>%d</td>"),ddmt->rxPowerMin,ddmt->rxPowerMax);
     nBytesSent=websWrite(wp, T("<td>%d</td><td>%d</td>"),ddmt->txPowerMin,ddmt->txPowerMax);
     nBytesSent=websWrite(wp, T("<td>%d</td><td>%d</td>"),ddmt->tempMin,ddmt->tempMax);
     nBytesSent=websWrite(wp, T("<td>%d</td><td>%d</td>"),ddmt->voltMin,ddmt->voltMax);
     nBytesSent=websWrite(wp, T("<td>%d</td><td>%d</td>"),ddmt->rxBiasMin,ddmt->rxBiasMax);
nBytesSent+=websWrite(wp, "</tr>");
    }
*/
      //ddmt++;
   }
 }
 else
 {  
     websError(wp, 400, T("Insufficient args\n"));
     return -1;
  }
 return nBytesSent;
     
}

static void formDdmThreshold(webs_t wp, char_t *path, char_t *query)
{
  char_t *slot,*port,*value;
  //char set_cmd[50];
  int unit,index;
  //ddmThreshold_t *ddmt;
  //groupDdmThreshold_t *gddmt;
  slot = websGetVar(wp, T("uid"), T("0"));
  port = websGetVar(wp, T("port"), T("0"));
  unit=atoi(slot);
  index=atoi(port);
  //ddmt=getPortDdmThreshold(0, unit,index);
  //gddmt=getGroupDdmThreshold(0, unit,1);
  //gddmt=getPortDdmThreshold(0,unit,port)
  //printf("formDdmThreshold\n");
  if (unit>0 && unit<16 && index>0 && index<8)
  {  
    setInfoSet_t infoset;
    infoset.chassis=0;
    infoset.infoset_id=DDM_THRESHOLD_ID;
    
    infoset.index=(index-1)/2+1;
    infoset.slot=unit;
    infoset.infoset_type='S';
    index=((index-1) % 2)*10;
    value=websGetVar(wp, T("minrx"), T("0"));
    infoset.infoItems[0].item_id=1+index;
    infoset.infoItems[0].item_value=atoi(value) & 0xff;

    //printf("minrx=%s\n",value);
    //gddmt->port_ddm[index].rxPowerMin=atoi(value) & 0xff;
    value=websGetVar(wp, T("maxrx"), T("0"));
    infoset.infoItems[1].item_id=2+index;
    infoset.infoItems[1].item_value=atoi(value) & 0xff;

    //printf("maxrx=%s\n",value);
    //gddmt->port_ddm[index].rxPowerMax=atoi(value);
 
    value=websGetVar(wp, T("mintx"), T("0"));
    infoset.infoItems[2].item_id=3+index;
    infoset.infoItems[2].item_value=atoi(value) & 0xff;
    //gddmt->port_ddm[index].txPowerMin=atoi(value) & 0xff;
    value=websGetVar(wp, T("maxtx"), T("0"));
    infoset.infoItems[3].item_id=4+index;
    infoset.infoItems[3].item_value=atoi(value) & 0xff;
    //gddmt->port_ddm[index].txPowerMax=atoi(value) & 0xff;

    value=websGetVar(wp, T("minvolt"), T("0"));
    infoset.infoItems[4].item_id=5+index;
    infoset.infoItems[4].item_value=atoi(value) & 0xff;
    //gddmt->port_ddm[index].voltMin=atoi(value) & 0xff;
    value=websGetVar(wp, T("maxvolt"), T("0"));
    infoset.infoItems[5].item_id=6+index;
    infoset.infoItems[5].item_value=atoi(value) & 0xff;
    //gddmt->port_ddm[index].voltMax=atoi(value);

    value=websGetVar(wp, T("minbias"), T("0"));
    infoset.infoItems[6].item_id=7+index;
    infoset.infoItems[6].item_value=atoi(value) & 0xff;
    //gddmt->port_ddm[index].rxBiasMin=atoi(value) & 0xff;
    value=websGetVar(wp, T("maxbias"), T("0"));
    infoset.infoItems[7].item_id=8+index;
    infoset.infoItems[7].item_value=atoi(value) & 0xff;
    //gddmt->port_ddm[index].rxBiasMax=atoi(value) & 0xff;

    value=websGetVar(wp, T("mintemp"), T("0"));
    infoset.infoItems[8].item_id=9+index;
    infoset.infoItems[8].item_value=atoi(value) & 0xff;
    //gddmt->port_ddm[index].tempMin=atoi(value) & 0xff;
    value=websGetVar(wp, T("maxtemp"), T("0"));
    infoset.infoItems[9].item_id=10+index;
    infoset.infoItems[9].item_value=atoi(value) & 0xff;
    infoset.infoItems[10].item_id=0;
    setDdmThreshold(infoset);
    insertOperateLog(LOG_DDM_THRESHOLD_HCONF,0,0,WebMode,mod,unit,index);
    //gddmt->port_ddm[index].tempMax=atoi(value) & 0xff;

    /*
   if (unit>0 && index>0 && index<5)
    {
    
    mode = websGetVar(wp, T("mode"), T("0"));
    line = websGetVar(wp, T("line"), T("0"));
    rule = websGetVar(wp, T("rule"), T("0"));
    ret = websGetVar(wp, T("ret"), T("0"));
    rettime= websGetVar(wp, T("rettime"), T("0"));
    
    if (!gstrcmp(mode,"2"))
       sprintf(set_cmd,"uset -I -s %s -e 65 -b 1=%s/2=%s/3=%s/4=%s",slot,mode,line,rule,ret);
    else
       sprintf(set_cmd,"uset -I -s %s -e 65 -b 1=%s/3=%s/4=%s",slot,mode,rule,ret);
    //printf("%s\n",set_cmd);
    system(set_cmd);
   
   }
  */
 }
 else 
     printf("maxtx=NULL\n");
  sleep(1);
  websRedirect(wp,"otuddmt.asp");
}
void ddmAspDefine()
{
 websAspDefine(T("DDM"), aspDdmInfo);
 websFormDefine(T("ddmt"), formDdmThreshold);
}
