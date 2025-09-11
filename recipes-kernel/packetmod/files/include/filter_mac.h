#ifndef FILTER_DEFS_H
#define FILTER_DEFS_H

#include <linux/types.h>
#include <linux/if_ether.h>

#define MAX_DOMAIN_LEN 64
#define MAX_BLACKLIST 128

bool filter_mac_check(const unsigned char *mac);
int add_mac_to_blacklist(const unsigned char *mac);
int del_mac_from_blacklist(const unsigned char *mac);
int get_mac_count(void);
const unsigned char *get_mac_blacklist(int index);

#endif