#include <unit.h>
#include <infoset.h>
#include <visipc.h>
#include <unit.h>
#include <passive.h>

boolean_t getPassiveViewInfo(uchar_t chassis,uchar_t slot,uchar_t uclass,uchar_t utype,unitViewInfo_t *info)
{
  char *property;
  property=getUnitProperty(0,slot);
  if (NULL==property)
    return FALSE;
  memset(info,0,sizeof(unitViewInfo_t));
}
