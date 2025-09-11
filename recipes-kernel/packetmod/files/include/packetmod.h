#ifndef PACKETMOD_H
#define PACKETMOD_H

#include <linux/types.h>

struct traffic_stat {
    unsigned char mac[6];
    __be32 ip;
    __u64 packets;
    __u64 bytes;
    __u64 last_seen;
};

#define NETLINK_TRAFFIC 31

struct nl_msg {
    __u32 cmd;
    __u8 data[128];
};

enum pm_command {
    CMD_ADD_MAC,
    CMD_DEL_MAC,
    CMD_ADD_IPV4,
    CMD_DEL_IPV4,
    CMD_ADD_DOMAIN,
    CMD_DEL_DOMAIN,
    CMD_GET_STATS,
};

#define MAX_DOMAIN_LEN 64

#endif
