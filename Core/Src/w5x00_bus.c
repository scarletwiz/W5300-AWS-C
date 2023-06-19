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

#if defined(STM32F207xx)
#include "stm32f2xx_hal.h"
#elif defined(STM32F429xx) || (STM32F439xx)
#include "stm32f4xx_hal.h"
#elif defined(STM32F722xx) || (STM32F756xx) || (STM32F767xx)
#include "stm32f7xx_hal.h"
#endif

#include "wizchip_conf.h"
#include "w5x00_bus.h"

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
static inline void wizchip_select(void)
{
  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_7, GPIO_PIN_RESET);
}

static inline void wizchip_deselect(void)
{
  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_7, GPIO_PIN_SET);
}

static inline uint16_t wizchip_read(uint32_t addr)
{
  return _W5300_DATA(addr);
}

static inline void wizchip_write(uint32_t addr, uint16_t tx_data)
{
  _W5300_DATA(addr) = tx_data;
}

void wizchip_initialize(void)
{
  /* Deselect the FLASH : chip select high */
  wizchip_deselect();

  /* CS function register */
  reg_wizchip_cs_cbfunc(wizchip_select, wizchip_deselect);

  /* BUS function register */
  reg_wizchip_bus_cbfunc(wizchip_read, wizchip_write);

  /* W5x00 initialize */
#if (_WIZCHIP_ == W5100S)
  uint8_t memsize[2][4] = {{2, 2, 2, 2}, {2, 2, 2, 2}};
#elif (_WIZCHIP_ == W5300)
  uint8_t memsize[2][8] = {{8, 8, 8, 8, 8, 8, 8, 8}, {8, 8, 8, 8, 8, 8, 8, 8}};
#elif (_WIZCHIP_ == W5500)
  uint8_t memsize[2][8] = {{2, 2, 2, 2, 2, 2, 2, 2}, {2, 2, 2, 2, 2, 2, 2, 2}};
#endif

  if (ctlwizchip(CW_INIT_WIZCHIP, (void *)memsize) == -1)
  {
    printf(" W5x00 initialized fail\n");

    return;
  }

#if (_WIZCHIP_ != W5300)
  uint8_t temp;

  /* Check PHY link status */
  do
  {
    if (ctlwizchip(CW_GET_PHYLINK, (void *)&temp) == -1)
    {
      printf(" Unknown PHY link status\n");

      return;
    }
  } while (temp == PHY_LINK_OFF);
#endif
}

void wizchip_reset(void)
{
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, GPIO_PIN_RESET);
  HAL_Delay(100);

  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, GPIO_PIN_SET);
  HAL_Delay(100);
}

void wizchip_check(void)
{
#if (_WIZCHIP_ == W5100S)
  /* Read version register */
  if (getVER() != 0x51)
  {
    printf(" ACCESS ERR : VERSION != 0x51, read value = 0x%02x\n", getVER());

    while (1)
      ;
  }
#elif (_WIZCHIP_ == W5300)
  // for further use
#elif (_WIZCHIP_ == W5500)
  /* Read version register */
  if (getVERSIONR() != 0x04)
  {
    printf(" ACCESS ERR : VERSION != 0x04, read value = 0x%02x\n", getVERSIONR());

    while (1)
      ;
  }
#endif
}
