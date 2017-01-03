#include <public.h>
#include <heartbeat.h>

char getHeartbeatConf(heart_beat_t *conf)
{
  char buf[128];
  char unsigned slotOfNmu;
  getConfValue(SYS_CONF_FILE,"hb_enable",buf," ");
  if (buf[0]=='Y')
    conf->auto_start=1;
  else
    conf->auto_start=0;

  if (getPidByName("pidof heartbeat")<1)
      conf->start=0;
  else
      conf->start=1;

  slotOfNmu=getSlotOfNmu(0,3);
  
  conf->port=getConfIntValue(SYS_CONF_FILE,"port"," ");
  conf->delay=getConfIntValue(SYS_CONF_FILE,"waitting"," "); 
  conf->timeout_sec=getConfIntValue(SYS_CONF_FILE,"sec"," ");
  conf->timeout_msec=getConfIntValue(SYS_CONF_FILE,"msec"," ");
  getConfValue(SYS_CONF_FILE,"hb_group",buf," ");
  return 1;
}

void setHeartbeatConf(heart_beat_t conf)
{
  
}
