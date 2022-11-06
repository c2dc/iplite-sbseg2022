/****************************************************************************
 * netutils/iptlite/iptlite.c
 ****************************************************************************/

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include "../../../nuttx/net/devif/devif.h"
#include <nuttx/config.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

/****************************************************************************
 * iptlite
 ****************************************************************************/

void listall_rules(){
  int rules_counter = nflite_get_rules_counter();
  char** table = nflite_listall();
  for(int i = 0; i < rules_counter; i++){
    for(int j = 0; j < RULE_MAX_SIZE; j++){
        printf("%c", table[i][j]);
    }
    printf("\n");
  }
}

int main(int argc, FAR char *argv[])
{
  int rule;
  in_addr_t srcipaddr, destipaddr;
  in_port_t srcport, destport;
  bool rule_added;

  if (argc < 2)
  {
    printf("Not enough arguments!\n");
    return -1;
  }

  if (strcmp(argv[1], "DROP") == 0 && argc == 6)
    rule = 0;
  else if (strcmp(argv[1], "ACCEPT") == 0 && argc == 6)
    rule = 1;
  else if (strcmp(argv[1], "FLUSHALL") == 0 && argc == 2)
  {
    rule = 2;
    nflite_flushall();
    return 0;
  }
  else if (strcmp(argv[1], "LISTALL") == 0 && argc == 2)
  {
    rule = 3;
    listall_rules();
    return 0;
  }
  else
  {
    printf("Invalid command! Verify command pattern.\n");
    return -1;
  }

  inet_pton(AF_INET, argv[2], &srcipaddr);
  inet_pton(AF_INET, argv[3], &destipaddr);
  srcport = htons(strtoul(argv[4], NULL, 10));
  destport = htons(strtoul(argv[5], NULL, 10));

  rule_added = nflite_addrule(
    rule, srcipaddr, destipaddr, srcport, destport);

  printf("rule_added? %s\n", rule_added ? "true" : "false");

  return 0;
}
