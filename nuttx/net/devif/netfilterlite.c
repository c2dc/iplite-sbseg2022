#include <netinet/in.h>
#include <nuttx/net/ip.h>
#include "devif.h"
#include <stdlib.h>

typedef enum rules {
    DROP,
    ACCEPT,
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

void netfilterlite_initialize(void) {
    chain_head = (chain*) malloc(sizeof(chain));
    chain_head->next = NULL;

    last_rule = chain_head;
}

bool netfilterlite_addrule(int rule, in_addr_t srcipaddr, in_addr_t destipaddr, in_port_t srcport, in_port_t destport) {
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

// TODO: code all missing verifications
bool netfilterlite_verify_ipv4(FAR struct ipv4_hdr_s *buf) {
    in_addr_t destipaddr;
    in_addr_t srcipaddr;

    FAR struct ipv4_hdr_s *ipv4 = buf;

    destipaddr = net_ip4addr_conv32(ipv4->destipaddr);
    srcipaddr = net_ip4addr_conv32(ipv4->srcipaddr);

    chain *current_rule = chain_head->next;
    while(current_rule) {
        if (current_rule->destipaddr == destipaddr || current_rule->srcipaddr == srcipaddr)
            return true;
        current_rule = current_rule->next;
    }

    return false;
}
