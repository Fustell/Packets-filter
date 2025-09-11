#include <linux/etherdevice.h>
#include "filter_mac.h"

static unsigned char mac_blacklist[MAX_BLACKLIST][ETH_ALEN];
static int mac_count = 0;

bool filter_mac_check(const unsigned char *mac)
{
    int i;
    for (i = 0; i < mac_count; i++) {
        if (ether_addr_equal(mac, mac_blacklist[i]))
            return true;
    }
    return false;
}

int add_mac_to_blacklist(const unsigned char *mac)
{
    if (mac_count >= MAX_BLACKLIST) {
        printk(KERN_WARNING "Blacklist is full\n");
        return -1;
    }
    memcpy(mac_blacklist[mac_count], mac, ETH_ALEN);
    mac_count++;
    printk(KERN_INFO "Added MAC to blacklist\n");
    return 0;
}

int del_mac_from_blacklist(const unsigned char *mac)
{
    int i;
    for (i = 0; i < mac_count; i++) {
        if (ether_addr_equal(mac, mac_blacklist[i])) {
            if (i < mac_count - 1) {
                memcpy(mac_blacklist[i], mac_blacklist[i + 1], (mac_count - i - 1) * ETH_ALEN);
            }
            mac_count--;
            printk(KERN_INFO "Removed MAC from blacklist\n");
            return 0;
        }
    }
    printk(KERN_WARNING "MAC not found in blacklist\n");
    return -1;
}

int get_mac_count(void)
{
    return mac_count;
}

const unsigned char *get_mac_blacklist(int index)
{
    if (index < 0 || index >= mac_count)
        return NULL;
    return mac_blacklist[index];
}