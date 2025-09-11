#include <linux/etherdevice.h>
#include "filter_ipv4.h"

static unsigned char ipv4_blacklist[MAX_BLACKLIST][4];
static int ipv4_count = 0;

bool filter_ipv4_check(const unsigned char *ip)
{
    int i;
    for (i = 0; i < ipv4_count; i++) {
        if (memcmp(ip, ipv4_blacklist[i], 4) == 0)
            return true;
    }
    return false;
}

int add_ipv4_to_blacklist(const unsigned char *ip)
{
    if (ipv4_count >= MAX_BLACKLIST) {
        printk(KERN_WARNING "IPV4 Blacklist is full\n");
        return -1;
    }
    memcpy(ipv4_blacklist[ipv4_count], ip, 4);
    ipv4_count++;
    printk(KERN_INFO "Added IPV4 to blacklist\n");
    return 0;
}

int del_ipv4_from_blacklist(const unsigned char *ip)
{
    int i;
    for (i = 0; i < ipv4_count; i++) {
        if (memcmp(ip, ipv4_blacklist[i], 4) == 0) {
            if (i < ipv4_count - 1) {
                memcpy(ipv4_blacklist[i], ipv4_blacklist[i + 1], (ipv4_count - i - 1) * 4);
            }
            ipv4_count--;
            printk(KERN_INFO "Removed IPV4 from blacklist\n");
            return 0;
        }
    }
    printk(KERN_WARNING "IPV4 not found in blacklist\n");
    return -1;
}
int get_ipv4_count(void)
{
    return ipv4_count;
}
const unsigned char *get_ipv4_blacklist(int index)
{
    if (index < 0 || index >= ipv4_count)
        return NULL;
    return ipv4_blacklist[index];
}
