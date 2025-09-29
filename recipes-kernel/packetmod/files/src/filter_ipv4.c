#include <linux/etherdevice.h>
#include "filter_ipv4.h"

#define IPV4_BLACKLIST_MAX 10

static __be32 ipv4_blacklist[IPV4_BLACKLIST_MAX];
static int ipv4_count = 0;

void print_ip(__be32 ip)
{
    unsigned char *bytes = (unsigned char *)&ip;
    printk(KERN_DEBUG "packetmod: IP=%u.%u.%u.%u\n", 
           bytes[0], bytes[1], bytes[2], bytes[3]);
}

bool filter_ipv4_check(__be32 ip)
{
    int i;

    if (ipv4_count == 0) {
        return false;
    }

    printk(KERN_DEBUG "packetmod: Checking IP ");
    print_ip(ip);

    for (i = 0; i < ipv4_count; i++) {
        printk(KERN_DEBUG "packetmod: Comparing with blacklist[%d] ", i);
        print_ip(ipv4_blacklist[i]);
        if (ipv4_blacklist[i] == ip)
            return true;
    }

    return false;
}


void add_ipv4_to_blacklist(__be32 *ip)
{
    if (ipv4_count >= IPV4_BLACKLIST_MAX) {
        printk(KERN_WARNING "packetmod: IPv4 blacklist is full\n");
        return;
    }
     ipv4_blacklist[ipv4_count] = *ip;
    printk(KERN_INFO "packetmod: Added IPv4 ");
    print_ip(ipv4_blacklist[ipv4_count]);
    ipv4_count++;
    printk(KERN_INFO "packetmod: Added IPv4 to blacklist, count=%d\n", ipv4_count);
}

int del_ipv4_from_blacklist(__be32 *ip)
{
    int i;
    for (i = 0; i < ipv4_count; i++) {
        if (ipv4_blacklist[i] == *ip) {
            if (i < ipv4_count - 1) {
                memmove(&ipv4_blacklist[i],
                        &ipv4_blacklist[i + 1],
                        (ipv4_count - i - 1) * sizeof(__be32));
            }
            ipv4_count--;
            printk(KERN_INFO "packetmod: Removed IPv4 from blacklist, count=%d\n", ipv4_count);
            return 0;
        }
    }
    printk(KERN_WARNING "packetmod: IPv4 not found in blacklist\n");
    return -1;
}

int get_ipv4_count(void)
{
    return ipv4_count;
}

const __be32 *get_ipv4_blacklist(int index)
{
    if (index < 0 || index >= ipv4_count)
        return NULL;
    return &ipv4_blacklist[index];
}

#ifdef TEST
void ipv4_blacklist_reset(void)
{
    ipv4_count = 0;
    memset(ipv4_blacklist, 0, sizeof(ipv4_blacklist));
}
#endif