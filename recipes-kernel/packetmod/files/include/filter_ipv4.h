#ifndef FILTER_IPV4_H
#define FILTER_IPV4_H

#include <linux/types.h>
#include <linux/if_ether.h>

#define MAX_DOMAIN_LEN 64
#define MAX_BLACKLIST 128

bool filter_ipv4_check(const unsigned char *ip);
int add_ipv4_to_blacklist(const unsigned char *ip);
int del_ipv4_from_blacklist(const unsigned char *ip);
int get_ipv4_count(void);
const unsigned char *get_ipv4_blacklist(int index);

#endif