/**
 * Copyright (c) 2023 WIZnet Co.,Ltd
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/**
 * ----------------------------------------------------------------------------------------------------
 * Includes
 * ----------------------------------------------------------------------------------------------------
 */
#include <stdio.h>

#include "wizchip_conf.h"
#include "w5x00_bus.h"
#include "w5x00_network.h"

/**
 * ----------------------------------------------------------------------------------------------------
 * Macros
 * ----------------------------------------------------------------------------------------------------
 */

/**
 * ----------------------------------------------------------------------------------------------------
 * Variables
 * ----------------------------------------------------------------------------------------------------
 */

/**
 * ----------------------------------------------------------------------------------------------------
 * Functions
 * ----------------------------------------------------------------------------------------------------
 */
void wizchip_network_initialize(wiz_NetInfo *net_info)
{
  wizchip_initialize();
  wizchip_reset();
  wizchip_check();

  ctlnetwork(CN_SET_NETINFO, (void *)net_info);
}

void wizchip_network_information(wiz_NetInfo *net_info)
{
  uint8_t tmp_str[8] = {
      0,
  };

  ctlnetwork(CN_GET_NETINFO, (void *)net_info);
  ctlwizchip(CW_GET_ID, (void *)tmp_str);

  if (net_info->dhcp == NETINFO_DHCP)
  {
    printf("====================================================================================================\n");
    printf(" %s network configuration : DHCP\n\n", (char *)tmp_str);
  }
  else
  {
    printf("====================================================================================================\n");
    printf(" %s network configuration : static\n\n", (char *)tmp_str);
  }

  printf(" MAC         : %02X:%02X:%02X:%02X:%02X:%02X\n", net_info->mac[0], net_info->mac[1], net_info->mac[2], net_info->mac[3], net_info->mac[4], net_info->mac[5]);
  printf(" IP          : %d.%d.%d.%d\n", net_info->ip[0], net_info->ip[1], net_info->ip[2], net_info->ip[3]);
  printf(" Subnet Mask : %d.%d.%d.%d\n", net_info->sn[0], net_info->sn[1], net_info->sn[2], net_info->sn[3]);
  printf(" Gateway     : %d.%d.%d.%d\n", net_info->gw[0], net_info->gw[1], net_info->gw[2], net_info->gw[3]);
  printf(" DNS         : %d.%d.%d.%d\n", net_info->dns[0], net_info->dns[1], net_info->dns[2], net_info->dns[3]);
  printf("====================================================================================================\n\n");
}
