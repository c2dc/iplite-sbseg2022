#include <netinet/in.h>
#include <nuttx/net/ip.h>
#include <nuttx/net/tcp.h>
#include <nuttx/net/netconfig.h>
#include <nuttx/net/netdev.h>
#include "devif.h"
#include <stdlib.h>

typedef enum rules {
    DROP,
    ACCEPT,
    FLUSHALL
} rules;

typedef struct chain chain;

struct chain {
    rules     rule;
    in_addr_t srcipaddr;
    in_addr_t destipaddr;
    in_port_t srcport;
    in_port_t destport;
    chain     *next;
};

chain *chain_head;
chain *last_rule;

void nflite_initialize(void) {
    chain_head = (chain*) malloc(sizeof(chain));
    chain_head->next = NULL;

    last_rule = chain_head;
}

bool nflite_addrule(int rule, in_addr_t srcipaddr, in_addr_t destipaddr,
                            in_port_t srcport, in_port_t destport) {
    chain *new_chainrule = (chain*) malloc(sizeof(chain));
    if (new_chainrule == NULL)
        return false;

    new_chainrule->rule = rule;
    new_chainrule->srcipaddr = srcipaddr;
    new_chainrule->destipaddr = destipaddr;
    new_chainrule->srcport = srcport;
    new_chainrule->destport = destport;
    new_chainrule->next = NULL;

    last_rule->next = new_chainrule;
    last_rule = last_rule->next;
    
    return true;
}

bool nflite_verify_ipv4(FAR struct net_driver_s *dev) {
    FAR struct ipv4_hdr_s *ipv4;
    FAR struct tcp_hdr_s *tcp;
    in_addr_t destipaddr;
    in_addr_t srcipaddr;
    in_port_t srcport;
    in_port_t destport;
    uint16_t iphdrlen;


    ipv4 = ((FAR struct ipv4_hdr_s *)&dev->d_buf[NET_LL_HDRLEN(dev)]);
    iphdrlen = (ipv4->vhl & IPv4_HLMASK) << 2;
    tcp = (FAR struct tcp_hdr_s *)&dev->d_buf[iphdrlen + NET_LL_HDRLEN(dev)];

    destipaddr = net_ip4addr_conv32(ipv4->destipaddr);
    srcipaddr = net_ip4addr_conv32(ipv4->srcipaddr);
    srcport = tcp->srcport;
    destport = tcp->destport;


    chain *current_rule = chain_head->next;
    while(current_rule) {
        /* Verify incoming tuple */
        if ((current_rule->destipaddr == 0 || current_rule->destipaddr == destipaddr) && (current_rule->srcipaddr == 0 || current_rule->srcipaddr == srcipaddr) && (current_rule->destport == 0 || current_rule->destport == destport) && (current_rule->srcport == 0 || current_rule->srcport == srcport))
            return false;
        current_rule = current_rule->next;
    }

    return true;
}

void nflite_flushall(void) {
    chain *current_rule = chain_head->next;
    chain *head = chain_head->next;
    while (head != NULL){
        current_rule = head;
        head = head->next;
        free(current_rule);
    }

    chain_head->next = NULL;

    last_rule = chain_head;
}