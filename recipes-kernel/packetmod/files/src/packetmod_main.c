#include <linux/module.h>
#include <linux/skbuff.h>
#include <linux/netfilter.h>
#include <linux/netfilter_ipv4.h>
#include <linux/etherdevice.h>
#include <linux/netlink.h>
#include <linux/ip.h>
#include <net/netlink.h>
#include <linux/inet.h> 
#include "filter_mac.h"
#include "filter_ipv4.h"
#include "packetmod.h"

#define NETLINK_USER 31

static struct sock *nl_sk = NULL;

static unsigned int hook_func(void *priv,
                             struct sk_buff *skb,
                             const struct nf_hook_state *state)
{
    struct ethhdr *eth;
    struct iphdr *iph ;

    if (!skb_mac_header_was_set(skb)) {
        return NF_ACCEPT;
    }

    eth = (struct ethhdr *)skb_mac_header(skb);
    
    if (filter_mac_check(eth->h_source) || filter_mac_check(eth->h_dest)) {
        return NF_DROP;
    }
    
    if (eth->h_proto != htons(ETH_P_IP)) {
        return NF_ACCEPT;
    }
    
    iph = ip_hdr(skb);

    if (!iph) {
        printk(KERN_ERR "packetmod: ip_hdr returned NULL\n");
        return NF_ACCEPT;
    }

    if (filter_ipv4_check(iph->saddr) || filter_ipv4_check(iph->daddr))
    {
        return NF_DROP;
    }

    return NF_ACCEPT;
}
static void nl_recv_msg(struct sk_buff *skb)
{
    struct nlmsghdr *nlh = nlmsg_hdr(skb);
    unsigned char *mac;
    int cmd;

    if (nlh->nlmsg_len < NLMSG_HDRLEN || skb->len < nlh->nlmsg_len)
        return;

    cmd = *(int *)nlmsg_data(nlh);
    mac = (unsigned char *)(nlmsg_data(nlh) + sizeof(int));
    // ipv4 = (unsigned char *)(nlmsg_data(nlh) + sizeof(int) + ETH_ALEN);

    if (cmd == CMD_ADD_MAC || cmd == CMD_DEL_MAC) {
        if (nlh->nlmsg_len < NLMSG_SPACE(sizeof(int) + ETH_ALEN)) {
            printk(KERN_ERR "packetmod: Insufficient payload for MAC command\n");
            return;
        }
    } 
    // else if (cmd == CMD_ADD_IPV4 || cmd == CMD_DEL_IPV4) {
    //     // if (nlh->nlmsg_len < NLMSG_SPACE(sizeof(int) + ETH_ALEN + 4)) {
    //     //     printk(KERN_ERR "packetmod: Insufficient payload for IPv4 command\n");
    //     //     return;
    //     // }
    //     printk(KERN_DEBUG "packetmod: Received IPv4: ");
    //     print_ip(*(u32 *)ipv4);
    // } 
    else {
        printk(KERN_WARNING "packetmod: Unknown command: %d\n", cmd);
        return;
    }

    switch (cmd) {
    case CMD_ADD_MAC:
        add_mac_to_blacklist(mac);
        break;
    case CMD_DEL_MAC:
        del_mac_from_blacklist(mac);
        break;
    // case CMD_ADD_IPV4:
    //     add_ipv4_to_blacklist((__be32 *)ipv4);
    //     break;
    // case CMD_DEL_IPV4:
    //     del_ipv4_from_blacklist((__be32 *)ipv4);
    //     break;
    default:
        printk(KERN_WARNING "Unknown command: %d\n", cmd);
    }
}

static struct nf_hook_ops nfho = {
    .hook = hook_func,
    .pf = PF_INET,
    .hooknum = NF_INET_PRE_ROUTING,
    .priority = NF_IP_PRI_FIRST,
};

static int __init packetmod_init(void)
{
    struct netlink_kernel_cfg cfg = {
        .input = nl_recv_msg,
    };
    int ret;

    __be32 ip = in_aton("192.168.7.1"); 
    add_ipv4_to_blacklist(&ip);   

    nl_sk = netlink_kernel_create(&init_net, NETLINK_USER, &cfg);
    if (!nl_sk) {
        printk(KERN_ERR "Failed to create netlink socket\n");
        return -ENOMEM;
    }

    ret = nf_register_net_hook(&init_net, &nfho);
    if (ret) {
        netlink_kernel_release(nl_sk);
        printk(KERN_ERR "Failed to register netfilter hook: %d\n", ret);
        return ret;
    }
    printk(KERN_INFO "Packet Modifier Module loaded\n");
    return 0;
}

static void __exit packetmod_exit(void)
{
    nf_unregister_net_hook(&init_net, &nfho);
    if (nl_sk)
        netlink_kernel_release(nl_sk);
    printk(KERN_INFO "Packet Modifier Module unloaded\n");
}

module_init(packetmod_init);
module_exit(packetmod_exit);

MODULE_LICENSE("Proprietary");
MODULE_DESCRIPTION("TCP Packet Modifier Kernel Module");
MODULE_AUTHOR("Roman Dykyi");