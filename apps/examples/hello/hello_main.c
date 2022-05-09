/****************************************************************************
 * examples/hello/hello_main.c
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

#include "../../../nuttx/net/devif/devif.h"
#include <nuttx/config.h>
#include <stdio.h>
#include <stdbool.h>

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * hello_main
 ****************************************************************************/

int main(int argc, FAR char *argv[])
{
  bool packet_dropped;
  switch (argv[1][0])
  {
  case '1':
    packet_dropped = netfilterlite_addrule(0, 0, 0, 40046, 0);
    if (packet_dropped == 1)
      printf("Drop packets from origin port: 40046\n");
    else
      printf("ERROR 1\n");
    break;

  case '2':
    packet_dropped = netfilterlite_addrule(0, 0, 0, 0, 40046);
    if (packet_dropped == 1)
      printf("Drop packets from destination port: 40046\n");
    else
      printf("ERROR 2\n");
    break;
    
  case '3':
    packet_dropped = netfilterlite_addrule(0, 0, 0, 27002, 0);
    if (packet_dropped == 1)
      printf("Drop packets from origin port: 27002\n");
    else
      printf("ERROR 3\n");
    break;

  case '4':
    packet_dropped = netfilterlite_addrule(0, 0, 0, 0, 27002);
    if (packet_dropped == 1)
      printf("Drop packets from destination port: 27002\n");
    else
      printf("ERROR 4\n");
    break;

  default:
    printf("ERROR 5\n");
    break;
  }
    
  return 0;
}
