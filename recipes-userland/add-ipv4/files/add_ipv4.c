#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <linux/netlink.h>
#include "packetmod.h"

#define NETLINK_USER 31

struct nl_ipv4_msg {
    int cmd;
    __be32 ip;
};

int main(int argc, char *argv[])
{
    struct sockaddr_nl src_addr, dest_addr;
    struct nlmsghdr *nlh = NULL;
    struct iovec iov;
    struct msghdr msg;
    int sock_fd;
    struct nl_ipv4_msg payload;
    struct in_addr ipv4;

    if (argc != 2) {
        fprintf(stderr, "Usage: %s <IPv4 address>\n", argv[0]);
        return -1;
    }

    if (inet_pton(AF_INET, argv[1], &ipv4) <= 0) {
        perror("inet_pton");
        return -1;
    }

    payload.cmd = CMD_ADD_IPV4;
    payload.ip = ipv4.s_addr;

    sock_fd = socket(PF_NETLINK, SOCK_RAW, NETLINK_USER);
    if (sock_fd < 0) {
        perror("socket");
        return -1;
    }

    memset(&src_addr, 0, sizeof(src_addr));
    src_addr.nl_family = AF_NETLINK;
    src_addr.nl_pid = getpid();

    if (bind(sock_fd, (struct sockaddr *)&src_addr, sizeof(src_addr)) < 0) {
        perror("bind");
        close(sock_fd);
        return -1;
    }

    memset(&dest_addr, 0, sizeof(dest_addr));
    dest_addr.nl_family = AF_NETLINK;
    dest_addr.nl_pid = 0;

    nlh = (struct nlmsghdr *)malloc(NLMSG_SPACE(sizeof(payload)));
    if (!nlh) {
        perror("malloc");
        close(sock_fd);
        return -1;
    }

    memset(nlh, 0, NLMSG_SPACE(sizeof(payload)));
    nlh->nlmsg_len = NLMSG_LENGTH(sizeof(payload));
    nlh->nlmsg_pid = getpid();
    nlh->nlmsg_flags = 0;

    memcpy(NLMSG_DATA(nlh), &payload, sizeof(payload));

    iov.iov_base = (void *)nlh;
    iov.iov_len = nlh->nlmsg_len;
    msg.msg_name = (void *)&dest_addr;
    msg.msg_namelen = sizeof(dest_addr);
    msg.msg_iov = &iov;
    msg.msg_iovlen = 1;

    if (sendmsg(sock_fd, &msg, 0) < 0) {
        perror("sendmsg");
        free(nlh);
        close(sock_fd);
        return -1;
    }

    printf("Sent CMD_ADD_IPV4 for %s\n", argv[1]);

    free(nlh);
    close(sock_fd);
    return 0;
}
