#ifndef __FILTER_IPV4_H
#define __FILTER_IPV4_H

#include <linux/types.h>

#define IPV4_BLACKLIST_MAX 10

void print_ip(__be32 ip);
bool filter_ipv4_check(__be32 ip);
void add_ipv4_to_blacklist(__be32 *ip);
int del_ipv4_from_blacklist( __be32 *ip);
int get_ipv4_count(void);
const __be32 *get_ipv4_blacklist(int index);

#ifdef TEST
void ipv4_blacklist_reset(void);
#endif

#endif