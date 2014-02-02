/* Copyright (c) 2009 Nordic Semiconductor. All Rights Reserved.
 *
 * The information contained herein is property of Nordic Semiconductor ASA.
 * Terms and conditions of usage are described in detail in NORDIC
 * SEMICONDUCTOR STANDARD SOFTWARE LICENSE AGREEMENT.
 *
 * Licensees are granted free, non-transferable use of the information. NO
 * WARRANTY of ANY KIND is provided. This heading must NOT be removed from
 * the file.
 *
 */

/** @file
*
* @defgroup nrf_dev_led_radio_tx_example_main main.c
* @{
* @ingroup nrf_dev_led_radio_tx_example
*
* @brief Radio Transmitter Example Application main file.
*
* This file contains the source code for a sample application using the NRF_RADIO peripheral.
*
*/

#include <stdint.h>
#include "radio_config.h"
#include "nrf_gpio.h"
#include "board.h"

static uint8_t packet[4];  ///< Packet to transmit

void init(void)
{
  nrf_gpio_cfg_output(LED);

  /* Start 16 MHz crystal oscillator */
  NRF_CLOCK->EVENTS_HFCLKSTARTED = 0;
  NRF_CLOCK->TASKS_HFCLKSTART = 1;

  /* Wait for the external oscillator to start up */
  while (NRF_CLOCK->EVENTS_HFCLKSTARTED == 0)
  {
  }

  // Set radio configuration parameters
  radio_configure();

  // Set payload pointer
  NRF_RADIO->PACKETPTR = (uint32_t)packet;
}

/**
 * @brief Function for application main entry.
 * @return 0. int return type required by ANSI/ISO standard.
 */
int main(void)
{
  init();
  nrf_gpio_pin_set(LED);

  char b = 0;
  while(1)
  {
    if (b)
      nrf_gpio_pin_set(LED);
    else
      nrf_gpio_pin_clear(LED);

    // Place the read character in the payload, enable the radio and
    // send the packet:
    packet[0] = b ? '1' : '0';
    NRF_RADIO->EVENTS_READY = 0U;
    NRF_RADIO->TASKS_TXEN = 1;
    while (NRF_RADIO->EVENTS_READY == 0U)
    {
    }
    NRF_RADIO->TASKS_START = 1U;
    NRF_RADIO->EVENTS_END = 0U;
    while(NRF_RADIO->EVENTS_END == 0U)
    {
    }
    NRF_RADIO->EVENTS_DISABLED = 0U;
    // Disable radio
    NRF_RADIO->TASKS_DISABLE = 1U;
    while(NRF_RADIO->EVENTS_DISABLED == 0U)
    {
    }

    b = !b;
    nrf_delay_ms(90);
  }
}

/**
 *@}
 **/
