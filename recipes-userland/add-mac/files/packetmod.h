#ifndef PACKETMOD_H
#define PACKETMOD_H

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