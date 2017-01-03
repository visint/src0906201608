#include <sys/sysinfo.h>
#include <time.h>
#include "../wsIntrn.h"
#include "../../include/unit.h"
#include <visipc.h>
#include <sqlite3.h>
#include <dbutil.h>
#include <log.h>

int aspOtuView(webs_t wp, int slot,int otu_type)
{
  int nBytesSent=0,n;
  nBytesSent +=websWrite(wp, T("<div class='panebd'> \
<div class='pt'>OTU</div> \
<div class='pwr'> \
<div class='led'> \
<img src='images/led_1.gif' > \
<div>PWR</div>\
</div></div>"));

 for (n=1;n<3;n++)
 {
  nBytesSent +=websWrite(wp, T("<div class='port'> \
<div class='portimg'><img src='images/p2_1.gif'/></div> \
<div class='led'> \
<img src='images/led_1.gif' > \
<div>PWR</div> \
</div>\
<div class='led'>\
<img src='images/led_1.gif'>\
<div>RX</div>\
</div>\
<div class='led'> \
<img src='images/led_2.gif'> \
<div>TX</div> \
</div>\
</div>"));
 }
  nBytesSent +=websWrite(wp, T("</div>"));
  return nBytesSent;
}
