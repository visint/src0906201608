#ifndef __UNITWEBS_H
#define __UNITWEBS_H 1
#include  <sys/types.h>
#include  <unit.h>

typedef struct
{
  uchar_t run_led;
  char run_text[12];
}web_led_t;
typedef struct
{
  uchar_t      pclass[32];
  char         topHtmlTex[256];
  char         pimg[12];
  char         port_count;
  char         tip[128];
  uchar_t      isTxtAtTwoSide;
}web_port_t;

typedef struct
{
  uchar_t uclass;
  uchar_t slotCount;
  char height[8];
  web_led_t led;
  web_port_t   port_groups[4];
  //uchar_t port_count[];
  //char         btext[64];
}web_unit_t;

extern boolean_t getUnitWebInfo(uchar_t chassis,uchar_t cclass,uchar_t slot,uchar_t uclass,uchar_t utype,web_unit_t *info);
extern boolean_t getOtuWebInfo(uchar_t chassis,uchar_t cclass,uchar_t slot,uchar_t uclass,uchar_t utype,web_unit_t *info);
boolean_t getOlpWebInfo(uchar_t chassis,uchar_t cclass,uchar_t slot,uchar_t uclass,uchar_t utype,web_unit_t *info);
boolean_t getOssWebInfo(uchar_t chassis,uchar_t cclass,uchar_t slot,uchar_t uclass,uchar_t utype,web_unit_t *info);
boolean_t getEdfaWebInfo(uchar_t chassis,uchar_t cclass,uchar_t slot,uchar_t uclass,uchar_t utype,web_unit_t *info);
#endif /*__UNITWEBS_H*/
