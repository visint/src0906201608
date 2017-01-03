#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <net/if_arp.h>
#include <net/if.h>
//arm-linux-gcc -o macchange  macchange.c -I../include -L=./ -L/home/visint/src/arm/lib
int main(int argc, char **argv) {
    struct ifreq ifr;
    int s;
    char mac_char[] = "12:34:56:78:12:34";
 
    sscanf(mac_char, "%hhx:%hhx:%hhx:%hhx:%hhx:%hhx",
        &ifr.ifr_hwaddr.sa_data[0],
        &ifr.ifr_hwaddr.sa_data[1],
        &ifr.ifr_hwaddr.sa_data[2],
        &ifr.ifr_hwaddr.sa_data[3],
        &ifr.ifr_hwaddr.sa_data[4],
        &ifr.ifr_hwaddr.sa_data[5]
        );
 
    s = socket(AF_INET, SOCK_DGRAM, 0);
    //assert(s != -1);
 
    strcpy(ifr.ifr_name, "eth0");
    ifr.ifr_hwaddr.sa_family = ARPHRD_ETHER;
    ioctl(s, SIOCSIFHWADDR, &ifr);
    //assert(ioctl(s, SIOCSIFHWADDR, &ifr) != -1);
 
    return 0;
}
