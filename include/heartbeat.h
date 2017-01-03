#ifndef __HEARTBEAT_H
#define __HEARTBEAT_H

typedef struct
{
   uchar_t linkage;
   uchar_t is_linkage;
   uchar_t hb_slots[16];
   uchar_t obp_count;
   char source_ip[6];
   uchar_t bypass;
}heart_beat_group_t;

typedef struct
{
   char  auto_start;
   char  start;
   short unsigned delay;
   char  unsigned timeout_sec;
   short unsigned timeout_msec;
   short unsigned port;
   char reply_message[16];
   heart_beat_group_t  groups[4];
}heart_beat_t;
extern char getHeartbeatConf(heart_beat_t *);
extern void setHeartbeatConf(heart_beat_t);
#endif //__HEARTBEAT_H
