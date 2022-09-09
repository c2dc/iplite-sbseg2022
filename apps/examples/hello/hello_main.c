/****************************************************************************
 * apps/examples/hello/hello_main.c
 *
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.  The
 * ASF licenses this file to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance with the
 * License.  You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  See the
 * License for the specific language governing permissions and limitations
 * under the License.
 *
 ****************************************************************************/

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>
#include "../../../nuttx/net/devif/devif.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * iplite
 ****************************************************************************/

int main(int argc, FAR char *argv[])
{
  int rule;
  unsigned int srcipaddr, destipaddr;
  unsigned int srcport, destport;
  bool packet_dropped;

  if (strcmp(argv[1], "DROP") == 0)
    rule = 0;
  else if (strcmp(argv[1], "ACCEPT") == 0)
    rule = 1;
  else if (strcmp(argv[1], "FLUSHALL") == 0) {
    rule = 2;
    netfilterlite_flushall();

    return 0;
  }
  else {
    printf("Invalid rule!\n");
    return -1;
  }

  inet_pton(2, argv[2], &srcipaddr);
  inet_pton(2, argv[3], &destipaddr);
  srcport = htons(strtoul(argv[4], NULL, 10));
  destport = htons(strtoul(argv[5], NULL, 10));

  packet_dropped = netfilterlite_addrule(
    rule, srcipaddr, destipaddr, srcport, destport);

  printf("packet_dropped? %s\n", packet_dropped ? "true" : "false");

  return 0;
}
