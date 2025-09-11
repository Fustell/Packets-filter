#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <linux/netlink.h>
#include <linux/if_ether.h>
#include <unistd.h>
#include "packetmod.h"

#define NETLINK_USER 31
#define MAX_PAYLOAD 1024

int main(int argc, char *argv[])
{
    struct sockaddr_nl src_addr, dest_addr;
    struct nlmsghdr *nlh = NULL;
    struct iovec iov;
    struct msghdr msg;
    int sock_fd;
    unsigned char ipv4[ETH_ALEN];
    int cmd = CMD_ADD_IPV4;

    if (argc != 2) {
        fprintf(stderr, "Usage: %s <IPv4 address in x.x.x.x format>\n", argv[0]);
        return -1;
    }

    if (sscanf(argv[1], "%hhu.%hhu.%hhu.%hhu",
               &ipv4[0], &ipv4[1], &ipv4[2], &ipv4[3]) != 4) {
        fprintf(stderr, "Invalid IPv4 address format\n");
        return -1;
    }

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

    nlh = (struct nlmsghdr *)malloc(NLMSG_SPACE(MAX_PAYLOAD));
    if (!nlh) {
        perror("malloc");
        close(sock_fd);
        return -1;
    }
    memset(nlh, 0, NLMSG_SPACE(MAX_PAYLOAD));
    nlh->nlmsg_len = NLMSG_SPACE(sizeof(int) + ETH_ALEN);
    nlh->nlmsg_pid = getpid();
    nlh->nlmsg_flags = 0;

    *(int *)NLMSG_DATA(nlh) = cmd;
    memcpy(NLMSG_DATA(nlh) + sizeof(int), ipv4, ETH_ALEN);

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

    printf("Sent CMD_ADD_IPV4 for %d.%d.%d.%d\n",
           ipv4[0], ipv4[1], ipv4[2], ipv4[3]);

    free(nlh);
    close(sock_fd);
    return 0;
}